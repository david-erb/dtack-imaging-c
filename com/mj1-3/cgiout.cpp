
//--------------------------------------------------------------------------
// netout to stdout
//--------------------------------------------------------------------------

#include <windows.h>
#include "dtax.h"
#include <dtlist.h>
#include <dtipc.h>
#include <dtos.h>
#include <dtdg.h>
#define DTDEPEND
#include <dtmsw.h>
#include <dterrno.h>

#include "dtmj.h"

#include <streams.h>
#include <olectl.h>
#include "guid.h"
#include "command.h"
#include "push_prop.h"
#include "push.h"
#include "cgiout.h"

#include <winsock.h>

#define CR (0x0d)
#define LF (0x0a)

#define CONNECTED_NOT      DTMJ_PUSH_CONNECTED_NOT
#define CONNECTED_TEXTPUSH DTMJ_PUSH_CONNECTED_TEXTPUSH
#define CONNECTED_JPEGPUSH DTMJ_PUSH_CONNECTED_JPEGPUSH
#define CONNECTED_DISCONNECTED DTMJ_PUSH_CONNECTED_DISCONNECTED

//--------------------------------------------------------------------------
// DllRegisterServer
//
// Handles sample registry and unregistry
//
HRESULT DllRegisterServer()
{
    return AMovieDllRegisterServer();

} // DllRegisterServer


//--------------------------------------------------------------------------
// DllUnregisterServer
//
HRESULT DllUnregisterServer()
{
    return AMovieDllUnregisterServer();

} // DllUnregisterServer

//--------------------------------------------------------------------------

// Setup information

AMOVIESETUP_MEDIATYPE dtmj_cgiout_inpintypes =
{
    &MEDIATYPE_Stream,         		// major type
    &dtmj_subtype_guid   		// minor type
};

AMOVIESETUP_PIN dtmj_cgiout_pins[] =
{
    { L"Input",             // Pins string name
      TRUE,                 // Is it rendered
      FALSE,                // Is it an output
      FALSE,                // Are we allowed none
      FALSE,                // And allowed many
      &CLSID_NULL,          // Connects to filter
      NULL,                 // Connects to pin
      1,                    // Number of types
      &dtmj_cgiout_inpintypes           // Pin information
    }
};

AMOVIESETUP_FILTER dtmj_cgiout_filter =
{
    &dtmj_cgiout_guid,          	// filter CLSID
    L"Dtack Mjpeg cgiout filter",    	// string name
    MERIT_DO_NOT_USE,       		// filter merit
    1,                      		// number of pins
    dtmj_cgiout_pins   			// pin information
};

//--------------------------------------------------------------------------
// GetSetupData
//
LPAMOVIESETUP_FILTER 
dtmj_cgiout_c::GetSetupData()
{
  return &dtmj_cgiout_filter;
}

//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
// CPersistStream::GetClassID
//
STDMETHODIMP 
dtmj_cgiout_c::GetClassID(
  CLSID *pClsid)
{
  *pClsid = dtmj_cgiout_guid;
  return NOERROR;
}


//--------------------------------------------------------------------------
// List of class IDs and creator functions for the class factory. This
// provides the link between the OLE entry point in the DLL and an object
// being created. The class factory will call the static CreateInstance

CFactoryTemplate g_Templates[] = {
  {
    L"Dtack Mjpeg cgiout1-3 renderer filter", 
    &dtmj_cgiout_guid,
    dtmj_cgiout_c::CreateInstance
  },
  {
    L"Dtack Mjpeg cgiout1-3 renderer filter property page", 
    &dtmj_push_proppage_guid, 
    dtmj_push_prop_c::CreateInstance
  }
};
int g_cTemplates = sizeof(g_Templates) / sizeof(g_Templates[0]);


//--------------------------------------------------------------------------
// CreateInstance
//
// Provide the way for COM to create a filter object
//
CUnknown *
dtmj_cgiout_c::CreateInstance(
  LPUNKNOWN punk, 
  HRESULT *phr)
{
  dtmj_cgiout_c *pNewObject = 
    new dtmj_cgiout_c(
      dtmj_cgiout_guid,
      NAME("cgiout"), 
      punk, phr);
  if (pNewObject == NULL) 
    *phr = E_OUTOFMEMORY;
  return pNewObject;
} 

//--------------------------------------------------------------------------
// Constructor
//
dtmj_cgiout_c::dtmj_cgiout_c(
  CLSID clsid,
  TCHAR *tszName,
  LPUNKNOWN punk,
  HRESULT *phr):
dtmj_push_c(clsid, tszName, punk, phr)
{
}

//--------------------------------------------------------------------------
// Destructor
//
dtmj_cgiout_c::~dtmj_cgiout_c()
{
  int connected = this->get_connected();
  this->hangup(this->get_ctl(),		// tell client we are history
    &connected);
}

//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
/* wait for connection                                                      */

HRESULT 
dtmj_cgiout_c::accept(
  dt_ctl_t *ctl,                        /* environment control              */
  const char *dgspec,                   /* dg server spec                   */
  int *connected)                       /* returned true if we have a client*/
{
  DT_F("dtmj_cgiout_c::accept");
  static char *q;
  const char *variant = (const char *)ctl->user4;
  dt_rc_e rc = DT_RC_GOOD;
  
  if (variant == NULL ||		/* no variant string?               */
      strstr(variant, "textpush")) 	/* sender wants text only?          */
    *connected = CONNECTED_TEXTPUSH;
  else                                  /* sender wants real jpeg?          */
    *connected = CONNECTED_JPEGPUSH;
  
  dt_dbg(ctl, F, DT_DBG_MASK_COMMAND,
    "variant=\"%s\"", 
    variant? variant: "NULL");

  return rc == DT_RC_GOOD?
    NOERROR: E_FAIL;
}

//--------------------------------------------------------------------------
/* stream out data                                                          */

HRESULT 
dtmj_cgiout_c::stream(
  dt_ctl_t *ctl,
  const void *buffer,
  long length,
  int *connected)
{
  DT_F("dtmj_cgiout_c::c_stream");
  SOCKET h;
  long n;
  int got;
  dt_rc_e rc = DT_RC_GOOD;

  h = (SOCKET)ctl->user3;		// get socket handle from environment	    
  if (h == (SOCKET)0)
    *connected = CONNECTED_NOT;
					
  if (*connected == CONNECTED_NOT)	// not connected yet?
    goto cleanup;

  for (n=0; n<length; n+=got)
  {
    got = send(h,
      (char *)buffer + n, 
      length - n,
      0); 

    if (got == SOCKET_ERROR)
    {
      DWORD e = WSAGetLastError();
      if (e == WSAEDISCON ||		// just looks like browser closed?
	  e == WSAECONNRESET ||
	  e == WSAECONNABORTED)
      {
	dt_dbg(ctl, F, DT_DBG_MASK_IPC,
          "write error %d, "
          "socket presumed closed",
          e);
	got = 0;
      }
      else
      {
        char s[256];
        DTERRNO_LASTS(e, s, sizeof(s));
        rc = dt_err(ctl, F,
	  "send(0x%08lx, ...) e=%d"
          DTERRNO_LASTFS,
          h, e,
          DTERRNO_LASTES(e, s));
        goto cleanup;
      }
    }

    if (got == 0)                       /* end-of-file?                     */
    {
      *connected = CONNECTED_DISCONNECTED;
      break;
    }
  }

cleanup:
  return rc == DT_RC_GOOD?
    NOERROR: E_FAIL;
}

//--------------------------------------------------------------------------
/* check for feedback                                                       */

HRESULT 
dtmj_cgiout_c::feedback(
  dt_ctl_t *ctl,
  char *cmd,
  long max)
{
  DT_F("dtmj_cgiout_c::feedback");
  SOCKET h;
  fd_set fds;
  struct timeval timeout = {0};
  int sysrc;
  int l;
  dt_rc_e rc = DT_RC_GOOD;

  cmd[0] = '\0';			// clear command returned
  
  h = (SOCKET)ctl->user2;		// get socket handle from environment	    
  if (h == (SOCKET)0)
    goto cleanup;

  FD_ZERO(&fds);
  FD_SET(h, &fds);
  sysrc = select(h+1,			// wait for activity	
    &fds, NULL, NULL, &timeout);
  if (sysrc == -1)			// failure in select()?
  {
    int e = WSAGetLastError();
    rc = dt_err(ctl, F,
      "select(0x%08lx, ...)"
      " had socket error %d",
      e);
    goto cleanup;
  }

  if (!FD_ISSET(h, &fds))		// socket is not ready to read?
    goto cleanup;

  l = 0;
  while(1)				// read until end-of-line
  {
    int got;
    char c;

    got = recv(h, &c, 1, 0);		// get next character
    
    if (got == -1)			// error in recv?
    {
      int e = WSAGetLastError();
      rc = dt_err(ctl, F,
        "recv(0x%08lx, ...)"
        " had socket error %d",
        h, e);
      goto cleanup;
    }

    if (got == 0)                       // no more data?  
    {
      cmd[0] = '\0';			// no command returned
      goto cleanup;
    }

    if (l < max-1)                      // buffer not overfull? 
      cmd[l++] = c;

    if (c == '\n')			// end of line?
      break;
  }

  cmd[l] = '\0';

  while (l > 0 &&                       // nuke the crlf's in the feedback
         cmd[l-1] == LF ||
         cmd[l-1] == CR)
    cmd[--l] = '\0';

  dt_dbg(ctl, F, DT_DBG_MASK_COMMAND,
    "feedback \"%s\"", cmd);
  
cleanup:
  return rc == DT_RC_GOOD?
    NOERROR: E_FAIL;
}

//--------------------------------------------------------------------------
// quit client connection due to stream error

HRESULT 
dtmj_cgiout_c::hangup(
  dt_ctl_t *ctl,
  int *connected)
{
  DT_F("dtmj_cgiout_c::hangup");
  dt_rc_e rc = DT_RC_GOOD;

  if (*connected !=			// not already unconnected?
      CONNECTED_NOT)
  {
    SOCKET h;

    h = (SOCKET)ctl->user2;		// get input handle from environment	    
    if (h != (SOCKET)0)
    {
      closesocket(h);
    }
    ctl->user2 = NULL;

    h = (SOCKET)ctl->user3;		// get output handle from environment	    
    if (h != (SOCKET)0)
    {
      closesocket(h);
    }
    ctl->user3 = NULL;

    *connected = CONNECTED_NOT;
  }

  return rc == DT_RC_GOOD?
    NOERROR: E_FAIL;
}

