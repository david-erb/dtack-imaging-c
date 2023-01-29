/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
  ! Copyright (C) 1998 Dtack Inc. All Rights Reserved
  ! To use this file, you must have signed a license agreement with Dtack Inc.
  ! Under no circumstances may you redistribute this file.
  ! This software is provided AS IS with no warranty expressed or implied.
  ! Dtack Inc. accepts no liability for use or misuse of this file.
  ! http://www.dtack.com  dtack@dtack.com  telephone +360.417.6700
  ! Dtack Inc, P.O. Box 3061, Port Angeles, WA USA 98362
  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */




/*..........................................................................
| NAME
| Introduction - intro
|
| DESCRIPTION
| As yet undocumented.
| 
| RETURN VALUES
| All functions Return DTOS_RANWELL on success.  
| Any other return value indicates failure.
| 
| ERRORS
| As yet undocumented.
| 
| END
 *..........................................................................*/

#include <dtack/base.h>
DT_RCSID("source/low $RCSfile: dde.c,v $ $Revision: 1.1 $");
#include <dtack/ipc.h>

#include <dtack/os.h>
#include <dtack/msw.h>

#ifdef DTCONFIG_IMPLIED_MSW
#ifndef DTDEPEND
#  include <dde.h>
#endif
#endif

#if DTPRJ_DDE_IPC != DT_1
/*..........................................................................*/
dt_rc_e
DTCONFIG_API1
dtipc_dde_init(							/* init structure					*/
  dt_ctl_t *ctl,
  dtipc_t *ipc,
  long recv_max,
  unsigned long flags)
{
  DT_F("dtipc_dde_init");

  return dt_err_notlinked(ctl, F,
    "DDE");
}

#else

/*..........................................................................
 *..........................................................................*/
static
dt_rc_e
dtipc_dde_window(						/* new window						*/
  dt_ctl_t *ctl,
  dtipc_t *ipc)
{
  DT_F("dtipc_dde_window");
  dtmsw_t *msw = (dtmsw_t *)ctl->msw;	/* reference special msw structure 	*/
  WNDCLASS c;
  DT_MEMSET(&c, 0, sizeof(c));
  c.hInstance =							/* make sure invisible class exists */
    (HINSTANCE)msw->hinstance;
  c.lpszClassName = "dtipc_dde";
  RegisterClass(&c);
  ipc->ourwnd = (int)CreateWindow(		/* create the invisible window 		*/
    "dtipc_dde", NULL, 
	WS_OVERLAPPEDWINDOW,
    0, 0, 0, 0,
    (HWND)NULL, 
    (HMENU)NULL,
    (HINSTANCE)msw->hinstance, 
    NULL);
  if (ipc->ourwnd == 0)
    return dt_err(ctl, F,
      "CreateWindow failed");
  
  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/
static
dt_rc_e
dtipc_dde_waitack(						/* wait for ack						*/
  dt_ctl_t *ctl,
  dtipc_t *ipc,
  WORD *wparam,
  DWORD *lparam)
{
  DT_F("dtipc_dde_waitack");
  MSG msg;

  do {
    WaitMessage();						/* wait for any message 			*/
    PeekMessage(&msg, NULL,				/* peek at the message 				*/
      0, 0, PM_NOREMOVE);
	if (msg.message != WM_DDE_ACK)		/* not what we are waiting for? 	*/
      DT_Q(dtmsw_yield,(ctl))			/* handle it in yield function 		*/
    else								/* this is our ack? 				*/
      PeekMessage(&msg, NULL,			/* remove from the queue 			*/
        0, 0, PM_REMOVE);
  } while(msg.message != WM_DDE_ACK);

  *wparam = msg.wParam;
  *lparam = msg.lParam;
  
  return DT_RC_GOOD;
}

/*..........................................................................*/
static
dt_rc_e
dtipc_dde_register(						/* register service name			*/
  dt_ctl_t *ctl,
  dtipc_t *ipc,
  const char *name)
{
  DT_F("dtipc_dde_register");

  if (name == NULL)
    return dt_err(ctl, F,
      "NULL registered name");
  else
  if (strlen(name) == 0)
    return dt_err(ctl, F,
      "zero-length registered name");

  if (strlen(name) > DTIPC_NAME_MAXLEN)
    return dt_err(ctl, F,
      "ipc name \"%s\" too long",
      name);

  strcpy(ipc->name, name);              /* remember the ipc name            */
  ipc->type = DTIPC_TYPE_DDE;

  if (ipc->flags &						/* supposed to background ourself?	*/
      DTIPC_FLAG_SELF_BACKGROUND)
  {
    return dt_err(ctl, F,
      "cannot background in this OS");
  }

  ipc->selectfd = -1;					/* we can't select for ipc input	*/

  return DT_RC_GOOD;
}


/*..........................................................................*/
static
dt_rc_e
dtipc_dde_unregister(					/* unregister service name			*/
  dt_ctl_t *ctl,
  dtipc_t *ipc)
{
  DT_MEMSET(ipc->name, 0,					/* clear name					    */
    sizeof(ipc->name));

  ipc->type = DTIPC_TYPE_NONE;

  return DT_RC_GOOD;
}

/*..........................................................................*/
static
dt_rc_e
dtipc_dde_unfind(						/* unfind registered service name	*/
  dt_ctl_t *ctl,
  dtipc_t *ipc)
{
  PostMessage((HWND)ipc->partnerwnd,	/* tell server we are quitting 		*/
    WM_DDE_TERMINATE, 
    (WORD)ipc->ourwnd,
    0);

  DestroyWindow((HWND)ipc->ourwnd);

  ipc->ourwnd = 0;
  
  DT_MEMSET(ipc->name, 0,					/* clear name					    */
    sizeof(ipc->name));

  ipc->type = DTIPC_TYPE_NONE;

  return DT_RC_GOOD;
}


/*..........................................................................*/
static
dt_rc_e
dtipc_dde_done(							/* server is done with connection   */
  dt_ctl_t *ctl,
  dtipc_t *ipc)
{
  PostMessage((HWND)ipc->partnerwnd,	/* tell client we are quitting 		*/
    WM_DDE_TERMINATE, 
    (WORD)ipc->ourwnd,
    0);

  ipc->partnerwnd = 0;

  return DT_RC_GOOD;
}

/*..........................................................................*/
static
dt_rc_e
dtipc_dde_isipc(						/* see if it's one of ours			*/
  dt_ctl_t *ctl,
  dtipc_t *ipc,
  unsigned long id,
  dtipc_header_t *h,
  int *isipc)
{
  return DT_RC_GOOD;
}

/*..........................................................................
| This function should not be called by a server unless only dtipc
| connect requests are expected.
| Any kind of incoming message which is not recognized as dtipc
| is discarded, with no reply to the sender.
| This case actually occurs in the :dttwin: test program.
| It turns out that the unrecognized messages are actually proxies from
| QNX windows and can be safely ignored.
 *..........................................................................*/
static
dt_rc_e
dtipc_dde_wait(							/* wait for request					*/
  dt_ctl_t *ctl,
  dtipc_t *ipc)
{
  return DT_RC_GOOD;
}

/*..........................................................................*/
static
dt_rc_e
dtipc_dde_find(							/* find registered service name		*/
  dt_ctl_t *ctl,
  dtipc_t *ipc,
  const char *name,
  long timeout)
{
  DT_F("dtipc_dde_find");
  ATOM app;
  ATOM topic;
  WORD wparam;
  DWORD lparam;

  DT_Q(dtipc_dde_window,(ctl, ipc));	/* make an invisible window 		*/

  app = GlobalAddAtom(name);
  topic = GlobalAddAtom("any");
  SendMessage((HWND)(-1),				/* see who is out there 			*/
    WM_DDE_INITIATE,
    (WORD)ipc->ourwnd, 
    MAKELONG(app, topic));
  GlobalDeleteAtom(topic);
  GlobalDeleteAtom(app);

  DT_Q(dtipc_dde_waitack,(ctl, ipc,		/* wait for ack 					*/
    &wparam, &lparam));

  GlobalDeleteAtom(LOWORD(lparam));
  GlobalDeleteAtom(HIWORD(lparam));

  ipc->partnerwnd = wparam;				/* remember server window			*/

  strcpy(ipc->name, name);              /* remember the ipc name            */

  ipc->type = DTIPC_TYPE_DDE;

  return DT_RC_GOOD;
}

/*..........................................................................*/
static
dt_rc_e
dtipc_dde_send(							/* send data on connection			*/
  dt_ctl_t *ctl,
  dtipc_t *ipc,
  void *data,
  long length)
{
  DT_F("dtipc_dde_send");
  HGLOBAL handle;
  char *p;
  DWORD lparam;
  WORD wparam;

  handle = GlobalAlloc(
    GMEM_DDESHARE | GMEM_MOVEABLE,
    length);
  p = GlobalLock(handle);
  DT_MEMCPY(p, data, length);				/* copy data to global area 		*/
  GlobalUnlock(handle);
  PostMessage((HWND)ipc->partnerwnd,
    WM_DDE_EXECUTE, 
    (WORD)ipc->ourwnd,
    MAKELONG(0, handle));

  DT_Q(dtipc_dde_waitack,(ctl, ipc,		/* wait for ack 					*/
    &wparam, &lparam));

  if (HIWORD(lparam))
    GlobalFree(
      (HGLOBAL)(HIWORD(lparam)));

  if ((LOWORD(lparam) & 0x8000) == 0)	/* request denied? 					*/
    return dt_err(ctl, F,
      "request denied, code 0x%04x",
      LOWORD(lparam));

  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/
static
dt_rc_e
dtipc_dde_recv(						/* receive data on connection		*/
  dt_ctl_t *ctl,
  dtipc_t *ipc,
  void **data,
  long length)
{
  return DT_RC_GOOD;
}

/*..........................................................................*/
dt_rc_e
DTCONFIG_API1
dtipc_dde_init(						/* init structure					*/
  dt_ctl_t *ctl,
  dtipc_t *ipc,
  long recv_max,
  unsigned long flags)
{
  DT_MEMSET(ipc, 0, sizeof(*ipc));
  ipc->datafd.fd = -1;

  if (recv_max == 0)
    ipc->recv_max = DTIPC_RECV_MAX_DEFAULT;
  else
    ipc->recv_max = recv_max;

  ipc->flags = flags;

  ipc->rregister = dtipc_dde_register;
  ipc->unregister = dtipc_dde_unregister;
  ipc->wait = dtipc_dde_wait;
  ipc->isipc = dtipc_dde_isipc;
  ipc->find = dtipc_dde_find;
  ipc->unfind = dtipc_dde_unfind;
  ipc->done = dtipc_dde_done;

  return DT_RC_GOOD;
}
#endif





/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
  ! Copyright (C) 1998 Dtack Inc. All Rights Reserved
  ! To use this file, you must have signed a license agreement with Dtack Inc.
  ! Under no circumstances may you redistribute this file.
  ! This software is provided AS IS with no warranty expressed or implied.
  ! Dtack Inc. accepts no liability for use or misuse of this file.
  ! http://www.dtack.com  dtack@dtack.com  telephone +360.417.6700
  ! Dtack Inc, P.O. Box 3061, Port Angeles, WA USA 98362
  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */
