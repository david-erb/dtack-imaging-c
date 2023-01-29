#include <windows.h>
#include "dtax.h"
#include <dtlist.h>
#include <dtipc.h>
#include <dtos.h>
#include <dtdg.h>
#include <dtstr.h>
#define DTDEPEND
#include <dtmsw.h>

#include <streams.h>
#include <initguid.h>
#include <olectl.h>
#include "guid.h"
#include "dtmj.h"
#include "command.h"
#include "push_prop.h"
#include "push.h"
#include "resource.h"

//--------------------------------------------------------------------------
#define CR (0x0d)
#define LF (0x0a)

#define BOUNDARY "The-Data-Boundary"	/* the server push separator 		*/

#define CONNECTED_NOT          DTMJ_PUSH_CONNECTED_NOT
#define CONNECTED_TEXTPUSH     DTMJ_PUSH_CONNECTED_TEXTPUSH
#define CONNECTED_JPEGPUSH     DTMJ_PUSH_CONNECTED_JPEGPUSH
#define CONNECTED_DISCONNECTED DTMJ_PUSH_CONNECTED_DISCONNECTED

#define FORMAT(F, T) 			\
  if (this->format.F != format->F)	\
  {					\
    DTAX_C(dtstr_printf,(ctl, 		\
      (T), sizeof((T)), 		\
      "Mj-" #F ": %d\r\n", format->F))	\
    DTAX_C(this->stream,(ctl, 		\
      (T), strlen((T)),			\
      &this->connected));		\
    this->format.F = format->F;		\
  }

//--------------------------------------------------------------------------
// parse debug mask string
static
void
__dbgmask(
  dt_ctl_t *ctl,
  const char *dbgspec)
{
  DT_F("dtmj_push_c::__dbgmask");
  long dbg_mask;
  char message[80];
  dt_rc_e rc;
  rc = dt_dbg_parse(ctl, dbgspec,	// parse current debug mask
    &dbg_mask,
    message, sizeof(message));
  if (rc == DT_RC_GOOD)			// string is OK?
    ctl->dbg_mask = dbg_mask;
}


//--------------------------------------------------------------------------
// Constructor
//
dtmj_push_c::dtmj_push_c(
  CLSID clsid,
  TCHAR *tszName,
  LPUNKNOWN punk,
  HRESULT *phr):
CBaseRenderer(clsid, tszName, punk, phr),
CPersistStream(punk, phr),
connected(CONNECTED_NOT),
outbuf(NULL)
{
  DTAX_F("dtmj_push_c::dtmj_push_c");
  strcpy(this->prop.dgspec, "http://0:5713");
  strcpy(this->prop.dbgspec, "usr1");

  this->ctl = &this->_ctl;		// initial ctl environment
  memset(this->ctl, 0, 			// clear ctl structure
    sizeof(*this->ctl));

  this->ctl->msw = &this->_msw;
  memset(&this->_msw, 0, sizeof(this->_msw));

  this->_msw.outwin = (void *)(1);

  this->_msw.outbufm = 512;
  this->outbuf =
  this->_msw.outbuf = (char *)malloc(
    this->_msw.outbufm);

  __dbgmask(ctl, this->prop.dbgspec);	// check for new debug mask

  memset(&this->format, 0,		// clear local format copy
    sizeof(this->format));
} 

//--------------------------------------------------------------------------
// Destructor
//
dtmj_push_c::~dtmj_push_c()
{
  if (this->outbuf != NULL)
    free(this->outbuf);
  this->outbuf = NULL;
}


//--------------------------------------------------------------------------
// NonDelegatingQueryInterface
//
STDMETHODIMP 
dtmj_push_c::NonDelegatingQueryInterface(
  REFIID riid, 
  void **ppv)
{
  CheckPointer(ppv,E_POINTER);

  if (riid == dtmj_push_prop_i_guid)
    return GetInterface((dtmj_push_prop_i *)this, ppv);
  else 
  if (riid == IID_ISpecifyPropertyPages) 
    return GetInterface((ISpecifyPropertyPages *)this, ppv);
  else 
  if (riid == IID_IPersistStream)
    return GetInterface((IPersistStream *)this, ppv);
  else
    return CBaseRenderer::NonDelegatingQueryInterface(riid, ppv);

}

//--------------------------------------------------------------------------
// CPersistStream::SizeMax
//
int
dtmj_push_c::SizeMax()
{
  return sizeof(this->prop);
}

//--------------------------------------------------------------------------
// CPersistStream::GetSoftwareVersion
//
DWORD
dtmj_push_c::GetSoftwareVersion()
{
  return (DWORD)0x00010001;
}

//--------------------------------------------------------------------------
// CPersistStream::WriteToStream
//
HRESULT 
dtmj_push_c::WriteToStream(
  IStream *pStream)
{
  DTAX_F("dtmj_push_c::WriteToStream");
  DTAX_Q(pStream->Write,(&this->prop, sizeof(this->prop), NULL));
  return NOERROR;
}

//--------------------------------------------------------------------------
// CPersistStream::ReadFromStream
//
HRESULT 
dtmj_push_c::ReadFromStream(
  IStream *pStream)
{
  DTAX_F("dtmj_push_c::ReadFromStream");
  DTAX_Q(pStream->Read,(&this->prop, sizeof(this->prop), NULL)); 
  return NOERROR;
}


//--------------------------------------------------------------------------
// render
//
HRESULT 
dtmj_push_c::DoRenderSample(
  IMediaSample *sample)
{
  DTAX_F("dtmj_push_c::DoRenderSample");
  BYTE *ibuff;
  char tmp[128];
  AM_MEDIA_TYPE *type = NULL;
  CMediaType *media = NULL;
  dtmj_format_t *format = NULL;
  long size;
  dt_rc_e rc = DT_RC_GOOD;
  
  dt_dbg(ctl, F, DT_DBG_MASK_TRACE,
    "called");
  
  sample->GetMediaType(&type);		// get sample's media type
  if (type != NULL)			// we have a media type?
  {
    media = new CMediaType(*type);	// wrap a class around it
    if (media != NULL)
    {
      format = (dtmj_format_t *)	// address media type's format
        media->Format();
      if (format == NULL)
	rc = dt_err(ctl, F,
          "media->Format() gave NULL");
    }
    else
      rc = dt_err(ctl, F,
        "new CMediaType() gave NULL");
  }
  else
    rc = dt_err(ctl, F,
      "sample->GetMediaType() gave NULL");

  if (rc != DT_RC_GOOD)
    goto cleanup;

  size = sample->GetActualDataLength();	// size of the jpeg

  DTAX_C(sample->GetPointer,(&ibuff));	// access the data
  
  if (this->connected ==		// no client connected?
      CONNECTED_NOT)
  {
    DTAX_C(this->accept,(this->ctl, 	// wait for client to connect
      this->prop.dgspec,
      &this->connected));
    if (this->connected ==		// timed out?
	CONNECTED_NOT)
      goto cleanup;

    if (this->connected ==		// connected for text only?
	CONNECTED_TEXTPUSH)
    {
      dt_dbg(ctl, F, DT_DBG_MASK_USR2,
        "doing textpush");
      sprintf(tmp,			// text-only header
        "HTTP/1.0 200\r\n"
        "Content-type: text/plain\r\n"
        "\r\n");
      DTAX_C(this->stream,(ctl, 
        tmp, strlen(tmp),
        &this->connected));
    }

    sprintf(tmp,			// main header
      "HTTP/1.0 200\r\n"
      "Content-type: multipart/"
      "x-mixed-replace;boundary=%s\r\n"
      "\r\n", BOUNDARY);
    DTAX_C(this->stream,(ctl, 
      tmp, strlen(tmp),
      &this->connected));
  }

  DTAX_C(dtstr_printf,(ctl, 		// stream pre-image stuff
    tmp, sizeof(tmp),
    "--%s\r\n"
    "Content-type: image/jpeg\r\n"
    "Content-length: %ld\r\n",
    BOUNDARY, size));
  DTAX_C(this->stream,(ctl, 
    tmp, strlen(tmp),
    &this->connected));

  if (format != NULL)			// we have format information?
  {
    FORMAT(xe, tmp);			// stream format stuff
    FORMAT(ye, tmp);
    FORMAT(fb_contrast, tmp);	
    FORMAT(jpeg_quality, tmp);
    FORMAT(fb_contrast, tmp);
  }

  DTAX_C(this->stream,(ctl, 		// end of header block
    "\r\n", 2,
    &this->connected));

  if (this->connected ==		// connected for real data?
      CONNECTED_JPEGPUSH)
    DTAX_C(this->stream,(ctl,		// stream the data itself
      ibuff, size,	       
      &this->connected));

  sprintf(tmp,				// stream post-image stuff
    "\r\n"
    "--%s\r\n", BOUNDARY);
  DTAX_C(this->stream,(ctl, 
    tmp, strlen(tmp),
    &this->connected));

  while(1)
  {
    DTAX_C(this->feedback,(ctl, 	// check for feedback
      tmp, sizeof(tmp)));
    if (tmp[0] == '\0')			// no feedback?
      break;
    DTAX_C(dtmj_command_passthrough,(	// pass command up to sending filter(s)
      ctl, this, tmp));
  }

cleanup:
  if (media != NULL)
    delete media;
  if (type != NULL)
    CoTaskMemFree(type);

  if (this->connected ==		// became disconnected?
      CONNECTED_DISCONNECTED)
  {
    DTAX_I(this->hangup,(ctl,		// hang up client connection
      &this->connected));
    NotifyEvent(EC_COMPLETE, 0, 0);	// tell the filter graph complete
  }
  else
  if (rc != DT_RC_GOOD)			// error writing?
  {
    DTAX_I(this->hangup,(ctl,		// hang up client connection
      &this->connected));
    NotifyEvent(EC_ERRORABORT, 0, 0);	// tell the filter graph error
  }
  else					// no error?
  {
    if (this->connected ==		// connected for real jpeg?
	CONNECTED_JPEGPUSH)
      dt_dbg(ctl, F, DT_DBG_MASK_USR2,
        "jpeg %ld", size);
    else
    if (this->connected ==		// connected for text?
	CONNECTED_TEXTPUSH)
      dt_dbg(ctl, F, DT_DBG_MASK_USR2,
        "text %ld", size);
    else				// not connected?
      dt_dbg(ctl, F, DT_DBG_MASK_USR2,
        "dumped %ld", size);
  }

  return rc == DT_RC_GOOD?
    NOERROR: E_FAIL;
}

//--------------------------------------------------------------------------
// SetMediaType
//
HRESULT dtmj_push_c::SetMediaType(
  const CMediaType *media)
{
  DTAX_F("dtmj_push_c::SetMediaType");
  CAutoLock cInterfaceLock(&m_InterfaceLock);
  this->media = *media;
  return NOERROR;
} 

//--------------------------------------------------------------------------
// Check the input type is OK - return an error otherwise

HRESULT 
dtmj_push_c::CheckMediaType(
  const CMediaType *media)
{
  DTAX_F("dtmj_push_c::CheckMediaType");
  int istype = IsEqualGUID(*media->Type(), MEDIATYPE_Stream);
  int issubtype = IsEqualGUID(*media->Subtype(), dtmj_subtype_guid);
  int isformat = IsEqualGUID(*media->FormatType(), dtmj_format_guid);

  if (!istype || !issubtype || !isformat)
    return E_INVALIDARG;

  return NOERROR;
}


//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
// ISpecifyPropertyPages::GetPages
//
// Returns the clsid's of the property pages we support
//
STDMETHODIMP dtmj_push_c::GetPages(CAUUID *pPages)
{
  pPages->cElems = 1;
  pPages->pElems = (GUID *)CoTaskMemAlloc(sizeof(GUID));
  if (pPages->pElems == NULL) 
    return E_OUTOFMEMORY;
  *(pPages->pElems) = dtmj_push_proppage_guid;
  return NOERROR;
} 


//--------------------------------------------------------------------------
// dtmj_push_prop_i::get_prop
// return current jpeg encoding quality
// called by property page or VB
//
STDMETHODIMP 
dtmj_push_c::get_prop(
  dtmj_push_prop_t *prop)
{
  CAutoLock lock(&this->lock);
  CheckPointer(prop, E_POINTER);

  *prop = this->prop;

  return NOERROR;
}

//--------------------------------------------------------------------------
// dtmj_push_prop_i::put_prop
// change jpeg encoding quality starting with next frame
// called by property page or VB
//
STDMETHODIMP 
dtmj_push_c::put_prop(
  dtmj_push_prop_t *prop,
  char *message,
  int max)
{
  CAutoLock lock(&this->lock);

  this->prop = *prop;

  __dbgmask(ctl, this->prop.dbgspec);	// set new debug mask

  SetDirty(TRUE);			// needs to be written by IPersist
  return NOERROR;
}

//--------------------------------------------------------------------------
// dtmj_push_prop_i::put_ctl
// called by main filter graph program
//
STDMETHODIMP 
dtmj_push_c::put_ctl(
  dt_ctl_t *ctl)
{
  DT_F("dtmj_push_c::put_ctl");

  CAutoLock lock(&this->lock);

  this->ctl = ctl;
  
  dt_dbg(this->ctl, F, DT_DBG_MASK_TRACE,
    "new ctl set, insocket=0x%08lx, outsocket=0x%08lx, variant=\"%s\"",
    this->ctl->user2, this->ctl->user3, this->ctl->user4);

  return NOERROR;
}

