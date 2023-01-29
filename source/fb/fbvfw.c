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
  CHANGES
  19981127 DE added raw buffer concept
 *..........................................................................*/

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
| SOURCES
| Winnov: 2-composite 4-svideo 5-mxc
| 
| ERRORS
| As yet undocumented.
| 
| REV
| 1.5 19970525 fixed bug which turned 24-bit into 8 bit
| 
| END
 *..........................................................................*/

#include <dtack/base.h>
DT_RCSID("source/fb $RCSfile: fbvfw.c,v $ $Revision: 1.7 $");
#include <dtack/lut8.h>
#include <dtack/color.h>
#include <dtack/fb.h>
#include <dtack/os.h>
#include <dtack/str.h>
#include <dtack/prj.h>
#include <dtack/yield.h>
#include <dtack/errno.h>
#include <dtack/portio.h>

#include <dtack/msw.h>

#define DMASK 0

#define DESC "vfw capture card"

extern dtfb_init_f dtfb_vfw_init;

#if DTPRJ_VFW == DT_1 && !defined(__cplusplus)

#define CALLBACK_YIELD

typedef struct {
  dtportio_t portio;
  dtos_time_t switch_time;
  long switch_delay;					/* time at last source switch 		*/
  long rawsize;							/* allocated size of raw buffer 	*/
  dt_ctl_t *ctl;
  void *raw;							/* raw buffer 						*/
  char *buffer;
  char *spec;
  char *board_spec;
  BITMAPINFOHEADER *header;
  HWND hwnd;
  HANDLE grab;
  HANDLE done;
  int nwarnings;
  int winnov_open;
  int xe;
  int ye;
  int flag_nooverlay;					/* no overlay at all 				*/
  int flag_nosetsource;					/* not supposed to set source?		*/
  int flag_nosetquality;				/* not supposed to set quality?		*/
  int flag_nosetvcr;					/* not supposed to set to vcr?		*/
  int flag_noshow;						/* don't show overlay at start 		*/
  int flag_minimize;					/* minimize overlay at start 		*/
  int async;
  int shutdown;
  int flag_writeraw;
  int flag_setsize;
  int flag_srcdlg;
  int flag_fmtdlg;
  int flag_cmpdlg;
  int flag_dspdlg;
  int flag_mono;
  int flag_bmp24;
  int flag_hwnd;
  int had_error;
  int portio_initialized;
  dt_rc_e rc;
  char board_type[128];
  char capturing_died[256];
  char action[32];
} dtfb_vfw_priv_t;

#define USE_GLOBAL_PRIV
#ifdef USE_GLOBAL_PRIV
static dtfb_vfw_priv_t global_priv;
#endif

#define ACTION1(PRIV, S) 			\
  strcpy((PRIV)->action, (S));			\
  if ((PRIV)->hwnd != (HWND)0 &&		\
	  IsWindow((PRIV)->hwnd))			\
  {										\
    dt_dbg(ctl, F, DT_DBG_MASK_ALIMITS, \
      "doing %s IsWindow(%ld)=0x%08lx", \
      (S), (long)(PRIV)->hwnd,			\
      (long)IsWindow((PRIV)->hwnd));	\
  }

#define ACTION(PRIV, S) 				\
  ACTION1(PRIV, S, goto cleanup;)		\
  else									\
  {										\
    rc = dt_err(ctl, F, "%s failed," 	\
      " hwnd 0x%08lx (%ld) is no longer valid",		\
      (PRIV)->action, 					\
      (long)(PRIV)->hwnd,				\
      (long)(PRIV)->hwnd);				\
    goto cleanup;						\
  }

#define RESULT(PRIV) \
  if ((PRIV)->rc != DT_RC_GOOD) \
  { \
    rc = dt_err(ctl, F, "%s failed", \
      (PRIV)->action); \
    goto cleanup; \
  }  

#define MAXWARNINGS 5

#ifndef DTDEPEND
#  define NOCOMPMAN						/* dont include COMPMAN				*/
#  define NODRAWDIB						/* dont include DRAWDIB				*/
#  define NOAVIFMT						/* dont include AVI file format		*/
#  define NOMMREG						/* dont include MMREG				*/
#  define NOAVIFILE						/* dont include AVIFile interface	*/
#  define NOMCIWND						/* dont include AVIWnd class		*/
#  define NOMSACM						/* dont include ACM stuff			*/
#  include <vfw.h>
#endif

#if DTPRJ_WINNOV == DT_1
#include <wnvvfe.h>

#define WINNOV_C(FUNC, ARGS)			/* check rc and goto cleanup		*/\
{                                                                             \
  int winnov_rc;                                                              \
  DT_SETLOC                                                                   \
  ACTION(priv, #FUNC)                                                         \
  winnov_rc = FUNC ARGS;		                                              \
  if (winnov_rc != WNV_VFE_ERR_NO_ERROR)                                      \
  {                                                                           \
    rc = dt_err(ctl, F,                                                       \
      "%s returned winnov_rc %d",                                             \
      #FUNC, winnov_rc); 		                                              \
    goto cleanup;                                                             \
  }                                                                           \
  RESULT(priv);                                                               \
}

#define WINNOV_I(FUNC, ARGS)			/* do func, only set rc if bad		*/\
{                                                                             \
  int winnov_rc;                                                              \
  DT_SETLOC                                                                   \
  winnov_rc = FUNC ARGS;		                                              \
  if (winnov_rc != WNV_VFE_ERR_NO_ERROR)                                      \
  {                                                                           \
    dt_err(ctl, F,                                                            \
      "%s returned winnov_rc %d",                                             \
      #FUNC, winnov_rc);                                                      \
    if (rc == DT_RC_GOOD)                                                     \
      rc = DT_RC_BAD;                                                         \
  }                                                                           \
}
#endif

/*..........................................................................
 *..........................................................................*/

static
dt_rc_e
dtfb_vfw_messages(						/* handle window messages			*/
  dt_ctl_t *ctl,
  dtfb_vfw_priv_t *priv)
{
  DT_F("dtfb_vfw_messages");
  dtmsw_t *msw = (dtmsw_t *)ctl->msw;	/* reference special msw structure 	*/
  MSG msg;
  const char *msg_text;
  dt_rc_e rc = DT_RC_GOOD;

  if (priv == NULL)
    return rc;

  if (msw == NULL)
	return rc;
  
  while (PeekMessage(&msg,				/* check for messages 				*/
    priv->hwnd,
    0, 0, 0))
  {
	GetMessage(&msg, priv->hwnd, 0, 0);	/* remove message 					*/

    if (msg.message ==					/* got close? 						*/
		WM_CLOSE)
	{
      dt_dbg(ctl, F, DT_DBG_MASK_ASYNC,
        "WM_CLOSE received");
	  rc = DT_RC_STOP;
	}
	else
    if (msg.message ==					/* got system menu close? 			*/
		WM_SYSCOMMAND &&
		msg.wParam == SC_CLOSE)
	{
      dt_dbg(ctl, F, DT_DBG_MASK_ASYNC,
        "WM_SYSCOMMAND:SC_CLOSE received");
	  rc = DT_RC_STOP;
	}
	else
    if (msg.message ==					/* got windows quit? 				*/
		WM_QUIT)
	{
      rc = dt_err(ctl, F, 
        "WM_QUIT received");
	  rc = DT_RC_STOP;
	}
	else
    if (msg.message ==					/* got windows destroy? 			*/
		WM_DESTROY)
	{
      rc = dt_err(ctl, F, 
        "WM_DESTROY received");
	  rc = DT_RC_STOP;
	}

	DefWindowProc(priv->hwnd,
      msg.message, 
      msg.wParam, msg.lParam);

    dtmsw_message_string(ctl,			/* look up message string 			*/
      msg.message, &msg_text);

	dt_dbg(ctl, "vfw", DT_DBG_MASK_ASYNC,
      "0x%04lx, 0x%04x (%s), 0x%04x, 0x%08lx",
	  (long)priv->hwnd,
      msg.message, 
      msg_text, msg.wParam, msg.lParam);
  }

  return rc;
}

/*..........................................................................
 *..........................................................................*/

static
dt_rc_e
dtfb_vfw_wait(							/* handle window messages			*/
  dt_ctl_t *ctl,
  dtfb_vfw_priv_t *priv,
  HANDLE semaphore,
  long timeout,
  const char *why,
  char *message,
  int lmax)
{
  DT_F("dtfb_vfw_wait");
  long result;
  long i;
  dt_rc_e rc = DT_RC_GOOD;

dt_dbg(ctl, F, DMASK, "@%ld rc=%ld", __LINE__, rc);

#define INC (100L)
  for (i=0;
	   timeout==-1 || i<timeout;
	   i+=INC)
	{
	  result = WaitForSingleObject(		/* sleep a short while				*/
        semaphore, INC);
dt_dbg(ctl, F, DMASK, "@%ld rc=%ld", __LINE__, rc);
	  DT_C(dtfb_vfw_messages,(ctl,		/* check for messages on window	    */
        priv));
dt_dbg(ctl, F, DMASK, "@%ld rc=%ld", __LINE__, rc);
	  DT_C(dtmsw_yield,(ctl));			/* check for pid messages 			*/
dt_dbg(ctl, F, DMASK, "@%ld result=%ld rc=%ld", result, __LINE__, rc);
	  if (result != WAIT_TIMEOUT)
		break;
	}
	
  if (result == WAIT_TIMEOUT &&
	  message != NULL && lmax > 0)
  {
	DT_C(dtstr_printf,(ctl,
      message, lmax,
      "timeout waiting %ld ms for S%d"
      " (%s)",
      timeout, semaphore, why));
  }

cleanup:
dt_dbg(ctl, F, DMASK, "@%ld rc=%ld", __LINE__, rc);
  return rc;
}

/*..........................................................................
 *..........................................................................*/

static
LRESULT FAR PASCAL 
dtfb_vfw_error(							/* error callback 					*/
  HWND hwnd, 
  int n, 
  LPCSTR s)
{
  dtfb_vfw_priv_t *priv;
  dt_ctl_t *ctl;

#ifdef USE_GLOBAL_PRIV
  priv = &global_priv;

  if (priv->hwnd != hwnd)
  {
    char s[256];
	sprintf(s, "priv->hwnd=0x%08lx hwnd=0x%08lx",
      (long)priv->hwnd, (long)hwnd);
    MessageBox((HWND)NULL, s,
      "Video", MB_OK);
    priv->hwnd = (HWND)0;
	return (LRESULT)FALSE;
  }
#else
  priv = (dtfb_vfw_priv_t *)
    capGetUserData(hwnd);
#endif

  if (priv == NULL)						/* safety check 					*/
  {
    MessageBox((HWND)NULL, "NULL priv", 
      "Error", MB_OK);
	return (LRESULT)TRUE;
  }


  if (n == 0)
  {
    priv->rc = DT_RC_GOOD;
	return (LRESULT)TRUE;
  }

  priv->had_error = 1;

  ctl = priv->ctl;
  if (ctl == NULL)						/* safety check 					*/
  {
    MessageBox((HWND)NULL, "NULL ctl", 
      "Error", MB_OK);
	return (LRESULT)TRUE;
  }

  priv->rc = dt_err(ctl, 
    priv->action,
    "result %d: \"%s\"", n, s);

  return (LRESULT)TRUE;
}

/*..........................................................................
 *..........................................................................*/

static
LRESULT FAR PASCAL 
dtfb_vfw_video(							/* video callback 					*/
  HWND hwnd, 
  LPVIDEOHDR video)
{
  DT_F("dtfb_vfw_video");
  dtfb_vfw_priv_t *priv;
  dt_ctl_t *ctl;
  int size;
  dt_rc_e rc;

#ifdef USE_GLOBAL_PRIV
  priv = &global_priv;

  if (priv->hwnd != hwnd)
  {
    char s[256];
	sprintf(s, "priv->hwnd=0x%08lx hwnd=0x%08lx",
      (long)priv->hwnd, (long)hwnd);
    MessageBox((HWND)NULL, s,
      "Video", MB_OK);
    priv->hwnd = (HWND)0;
	return (LRESULT)FALSE;
  }
#else
  priv = (dtfb_vfw_priv_t *)
    capGetUserData(hwnd);
#endif

  if (priv == NULL)						/* safety check 					*/
  {
    MessageBox((HWND)NULL, "NULL priv", 
      "Video", MB_OK);
	return (LRESULT)FALSE;
  }

  ctl = priv->ctl;
  if (ctl == NULL)						/* safety check 					*/
  {
    MessageBox((HWND)NULL, "NULL ctl", 
      "Video", MB_OK);
	return (LRESULT)FALSE;
  }

  if (priv->had_error)					/* error function was called?		*/
  {
	sprintf(priv->capturing_died,
      "had previous error in callback A");
	goto cleanup;
  }

  if (!IsWindow(hwnd))					/* window has been closed? 			*/
  {
	sprintf(priv->capturing_died,
      "window is no longer valid in callback A");
	goto cleanup;
  }

  DT_C(dtfb_vfw_wait,(ctl, priv,
    priv->grab, -1, "permit to grab",
    priv->capturing_died,
    sizeof(priv->capturing_died)));
										/* ................................ */

  size = priv->xe * priv->ye *			/* expected buffer size 			*/
    priv->header->biBitCount / 8;

  if (video->dwBytesUsed == 0)
  {
	dt_dbg(ctl, F, DT_DBG_MASK_ASYNC,
      "warning captured size %ld"
      " but expected %dx%d=%ld",
      video->dwBytesUsed, 
      priv->xe, priv->ye, size);
	if (priv->nwarnings++ < MAXWARNINGS)
	  goto cleanup;
  }

  if (size != video->dwBytesUsed)		/* buffer size is unexpected?		*/
  {
	sprintf(priv->capturing_died,
      "captured size %ld"
      " but expected %dx%d=%ld",
      video->dwBytesUsed, 
      priv->xe, priv->ye, size);
	goto cleanup;
  }

  if (priv->rawsize <					/* raw buffer is too small? 		*/
	  video->dwBytesUsed)
  {
	if (priv->raw)						/* raw buffer is allocated at all? 	*/
	  DT_C(dtos_free2,(ctl, priv->raw,	/* release raw buffer 				*/
        F, "raw buffer"));
	priv->raw = NULL;
	priv->rawsize = 0;
	DT_C(dtos_malloc2,(ctl,				/* allocate space for raw buffer 	*/
      &priv->raw, video->dwBytesUsed,
      F, "raw"));
	priv->rawsize = video->dwBytesUsed;
  }

  if (priv->had_error)					/* error function was called?		*/
  {
	sprintf(priv->capturing_died,
      "had previous error in callback B");
	goto cleanup;
  }

  if (!IsWindow(hwnd))					/* window has been closed? 			*/
  {
	sprintf(priv->capturing_died,
      "window is no longer valid in callback B");
	goto cleanup;
  }

  rc = dtfb_vfw_messages(ctl, priv);	/* check for window messages 		*/
  if (rc == DT_RC_STOP)
  {
	sprintf(priv->capturing_died,
      "window message caused STOP in callback B");
	goto cleanup;
  }
  else
  if (rc != DT_RC_GOOD)
  {
	sprintf(priv->capturing_died,
      "window message caused ERROR in callback B");
	goto cleanup;
  }
										/* ................................ */
  {
  long e_code = 0;
  __try {
    MoveMemory(priv->raw,				/* copy raw data to raw buffer		*/
      video->lpData, 
      video->dwBytesUsed);
  }
  __except(e_code = _exception_code(),  /* save code if exception happens 	*/
		   EXCEPTION_EXECUTE_HANDLER)
  {
  }

  if (e_code != 0)						/* an exception happened? 			*/
  {
	sprintf(priv->capturing_died,
      "exception 0x%08lx"
      " reading %ld bytes at 0x%08lx",
      e_code, video->dwBytesUsed,
      video->lpData);
	goto cleanup;
  }
  }

cleanup:
  ReleaseSemaphore(priv->done, 1,		/* signal grab is done				*/
    NULL);

  if (priv->capturing_died[0])
  {
#if 0
	ACTION(priv, "capCaptureStop")
    capCaptureStop(priv->hwnd);	
	ACTION(priv, "capCaptureAbort")
    capCaptureAbort(priv->hwnd);		/* thread suicide				    */
    DestroyWindow(priv->hwnd);
#endif
    return (LRESULT)FALSE;
  }
  else
	return (LRESULT)TRUE;
}

#ifdef CALLBACK_YIELD

/*..........................................................................
 *..........................................................................*/

static
LRESULT FAR PASCAL 
dtfb_vfw_yield(							/* yield callback 					*/
  HWND hwnd)
{
  DT_F("dtfb_vfw_yield");
  dtfb_vfw_priv_t *priv;
  dt_ctl_t *ctl;
  LRESULT lresult;
  dt_rc_e rc;

#ifdef USE_GLOBAL_PRIV
  priv = &global_priv;

  if (priv->hwnd != hwnd)
  {
    char s[256];
	sprintf(s, "priv->hwnd=0x%08lx hwnd=0x%08lx",
      (long)priv->hwnd, (long)hwnd);
    MessageBox((HWND)NULL, s,
      "Video", MB_OK);
    priv->hwnd = (HWND)0;
	return (LRESULT)FALSE;
  }
#else
  priv = (dtfb_vfw_priv_t *)
    capGetUserData(hwnd);
#endif

  if (priv == NULL)						/* safety check 					*/
  {
    MessageBox((HWND)NULL, "NULL priv", 
      "Video", MB_OK);
	return (LRESULT)TRUE;
  }

#if 0
  MessageBox((HWND)NULL, "pause", 
   "Yield", MB_OK);
#endif

  ctl = priv->ctl;
  if (ctl == NULL)						/* safety check 					*/
  {
    MessageBox((HWND)NULL, "NULL ctl", 
      "Video", MB_OK);
	return (LRESULT)TRUE;
  }

  if (priv->had_error)					/* error function was called?		*/
  {
	sprintf(priv->capturing_died,
      "had previous error");
	goto cleanup;
  }

  if (!IsWindow(hwnd))					/* window has been closed? 			*/
  {
	sprintf(priv->capturing_died,
      "window is no longer valid");
	goto cleanup;
  }

  DT_C(dtfb_vfw_messages,(ctl, priv));	/* check for messages on window		*/

cleanup:
  priv->rc = rc;

  if (priv->capturing_died[0] ||
	  rc != DT_RC_GOOD)
  {
	ReleaseSemaphore(priv->done, 1,		/* signal grab is done				*/
      NULL);
    return (LRESULT)FALSE;
  }
  else
	return (LRESULT)TRUE;
}

/*..........................................................................
 *..........................................................................*/

static
LRESULT FAR PASCAL 
dtfb_vfw_capcontrol(						/* control callback 				*/
  HWND hwnd,
  int state)
{
  DT_F("dtfb_vfw_capcontrol");
  return dtfb_vfw_yield(hwnd);
}
#endif

/*..........................................................................
 * query driver and print interesting things
 *..........................................................................*/

static
dt_rc_e
dtfb_vfw_report(
  dt_ctl_t *ctl,
  dtfb_t *fb,
  const char *prefix)
{
  DT_F("dtfb_vfw_report");
  dtfb_vfw_priv_t *priv = fb->priv;
  CAPDRIVERCAPS caps;
  CAPSTATUS capstatus;
  CAPTUREPARMS captureparms;
  dt_rc_e rc = DT_RC_GOOD;

  ACTION(priv, "capDriverGetCaps")
  capDriverGetCaps(priv->hwnd, 
    &caps, sizeof(caps));
  RESULT(priv);
 

#define REPORT(S) \
  dt_dbg(ctl, NULL, DT_DBG_MASK_ALIMITS, \
    "%s  %s %s", prefix, #S, caps.S? "true": "false");
  REPORT(fHasOverlay);
  REPORT(fHasDlgVideoSource);
  REPORT(fHasDlgVideoFormat);
  REPORT(fHasDlgVideoDisplay);
  REPORT(fCaptureInitialized);
  REPORT(fDriverSuppliesPalettes);
#undef REPORT


#define REPORT(S) \
  dt_dbg(ctl, NULL, DT_DBG_MASK_ALIMITS, \
    "%s  %s %ld (0x%08lx)", prefix, #S, \
    (long)priv->header->S, \
    (long)priv->header->S);
  REPORT(biSize);
  REPORT(biCompression);
  REPORT(biWidth);
  REPORT(biHeight);
  REPORT(biPlanes);
  REPORT(biBitCount);
  REPORT(biSizeImage);
#undef REPORT

  ACTION(priv, "capGetStatus")
  capGetStatus(priv->hwnd, 
    &capstatus, sizeof(capstatus));
  RESULT(priv);

#define REPORT(S) \
  dt_dbg(ctl, NULL, DT_DBG_MASK_ALIMITS, \
    "%s  %s %ld (0x%08lx)", prefix, #S, \
    (long)capstatus.S, \
    (long)capstatus.S)
  REPORT(uiImageWidth);
  REPORT(uiImageHeight);
  REPORT(fLiveWindow);
  REPORT(fOverlayWindow);
  REPORT(dwCurrentVideoFrame);
  REPORT(dwCurrentTimeElapsedMS);
  REPORT(fCapturingNow);
  REPORT(dwReturn);
#undef REPORT

  ACTION(priv, "capCaptureGetSetup")
  capCaptureGetSetup(priv->hwnd, 
    &captureparms, sizeof(captureparms));
  RESULT(priv);

#define REPORT(S) \
  dt_dbg(ctl, NULL, DT_DBG_MASK_ALIMITS, \
    "%s  %s %ld (0x%08lx)", prefix, #S, \
    (long)captureparms.S, \
    (long)captureparms.S)
  REPORT(dwRequestMicroSecPerFrame);
  REPORT(fYield);
  REPORT(wNumVideoRequested);
  REPORT(fCaptureAudio);
  REPORT(fLimitEnabled);
  REPORT(wTimeLimit);
#undef REPORT
cleanup:
  return rc;
}

/*..........................................................................
 *..........................................................................*/

static
dt_rc_e
dtfb_vfw_control(
  dt_ctl_t *ctl,
  dtfb_t *fb,
  dtfb_control_e control)
{
  DT_F("dtfb_vfw_control");

  DTCSTACK_HIGHWATER;

  switch (control)
  {
	case DTFB_CONTROL_RESET:
	break;
	case DTFB_CONTROL_SNAPSHOT:
	break;
	case DTFB_CONTROL_FREEZE:
	break;
	case DTFB_CONTROL_ACQUIRE:
	break;
	default:
      return dt_err(ctl, F,
        "invalid control code 0x%04x",
        control);
  }
  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/

static
dt_rc_e
dtfb_vfw_getblock(
  dt_ctl_t *ctl,
  dtfb_t *fb,
  void *data,
  int x0,
  int y0,
  int xe,
  int ye)
{
  DT_F("dtfb_vfw_getblock");
  dtfb_vfw_priv_t *priv = fb->priv;
  long result;
  long e_code = 0;
  dt_rc_e rc;
  
  if (x0 != 0 || y0 != 0)				/* the whole pie only			    */
    return dt_err(ctl, F,
      "x0=%d or y0=%d is not 0",
      x0, y0);

  priv->ctl = ctl;
  priv->xe = fb->xe;
  priv->ye = fb->ye;

  if (priv->capturing_died[0])
  {
	rc = dt_err(ctl, F,
      "%s",
      priv->capturing_died);
	goto cleanup;
  }

  DT_C(dtfb_vfw_messages,(ctl, priv));	/* check for messages on window		*/
  DT_C(dtmsw_yield,(ctl));				/* check for pid messages 			*/


  while (1)
  {
	ReleaseSemaphore(priv->grab, 1,		/* ready to grab				    */ 
      NULL);

	if (priv->had_error)				/* error function was called?		*/
    {
	  rc = dt_err(ctl, F,
        "had previous error");
	  goto cleanup;
    }

    if (!IsWindow(priv->hwnd))			/* window has been closed? 			*/
    {
	  rc = dt_err(ctl, F,
        "window is no longer valid");
	  goto cleanup;
	}
  
	if (!priv->async)					/* synchronous?					    */
	{
	  __try 
      {
	    ACTION(priv, "capGrabFrameNoStop")
	    capGrabFrameNoStop(priv->hwnd);	/* grab one frame 					*/
	    RESULT(priv);
	  }
	  __except(e_code =					/* save code if exception happens 	*/
        _exception_code(),
   	      EXCEPTION_EXECUTE_HANDLER)
	  {
	  }

	  if (e_code != 0)					/* an exception happened? 			*/
	  {
		rc = dt_err(ctl, F,
          "exception 0x%08lx",
          e_code);
	    goto cleanup;
      }
	}

    DT_C(dtfb_vfw_wait,(ctl, priv,
      priv->done, 100, "grab to be done",
      priv->capturing_died,
      sizeof(priv->capturing_died)));

	if (priv->switch_delay)				/* need to delay after switching? 	*/
	{
	  dtos_time_t now;
	  double elapsed, delay;
	  DT_C(dtos_time,(ctl, &now));
	  elapsed = now.wall -				/* time elapsed since switch 		*/
        priv->switch_time.wall;
	  delay = (double)					/* total delay in milliseconds 		*/
        priv->switch_delay / 1000.0;
	  if (elapsed < delay)				/* need to wait longer? 			*/
	  {
	    long milliseconds = 0.5 +
          1000.0 * (delay - elapsed);
		dt_dbg(ctl, F, DT_DBG_MASK_ASYNC,
          "delaying %ld ms",
          milliseconds);
		DT_C(dtos_delay,(ctl, 
          milliseconds));
	  }
	  priv->switch_delay = 0;			/* delay no more 					*/
	}
	else								/* no need to delay? 				*/
	  break;							/* grab no more images 				*/
  }

  if (priv->capturing_died[0])
  {
	rc = dt_err(ctl, F,
      "%s",
      priv->capturing_died);
	goto cleanup;
  }
										/* ................................ */
  if (priv->header->biBitCount == 16)	/* 16 bit RGB? 						*/
  {
	if (priv->flag_mono)
	  DT_C(dtfb_util_bi16_g,(ctl,		/* load the buffer				    */
        priv->raw,
        data,
        priv->xe, priv->ye))
	else
      DT_C(dtfb_util_bi16_rgb,(ctl,		/* load the buffer				    */
        priv->raw,
        data,
        priv->xe, priv->ye));
  }
  else
  if (priv->header->biBitCount == 24)	/* 24 bit RGB? 						*/
  {
	if (priv->flag_mono)
	{
	  DT_C(dtfb_util_bi24_g,(ctl,		/* load the buffer with green	    */
        priv->raw,
        data,
        priv->xe, priv->ye));
	}
	else
	if (priv->flag_bmp24)				/* want bmp24-bit output? 			*/
	{
      DT_MEMCPY(data,					/* this is the native form 			*/
        priv->raw,
        priv->xe * 3 * priv->ye);
	}
	else
      DT_C(dtfb_util_bi24_rgb,(ctl,		/* load the buffer				    */
        priv->raw,
        data,
        priv->xe, priv->ye));
  }
  else									/* not 16 or 24, presume 8 bit		*/
  {
	DT_C(dtfb_util_bi8,(ctl,			/* load the buffer				    */
      priv->raw,
      data,
      priv->xe, priv->ye));
  }

  if (priv->flag_writeraw)				/* supposed to write raw bytes? 	*/
  {
    DT_C(dtfb_util_write_raw,(ctl,		/* write the buffer					*/
      priv->raw,
      priv->rawsize,
      "fbvfw.raw"));
    priv->flag_writeraw = 0;			/* only do it once 					*/
  }

cleanup:
  return rc;
}

/*..........................................................................
 *..........................................................................*/

static
dt_rc_e
dtfb_vfw_release(						/* release resources 				*/
  dt_ctl_t *ctl,
  dtfb_vfw_priv_t *priv)
{
  DT_F("dtfb_vfw_release");
  long result;
  dt_rc_e rc = DT_RC_GOOD;

dt_dbg(ctl, F, DMASK, "@%ld priv=0x%08lx", __LINE__, priv);
  if (priv == NULL)
    return rc;

dt_dbg(ctl, F, DMASK, "@%ld priv->raw=0x%08lx", __LINE__, priv->raw);
  if (priv->raw)						/* raw buffer is allocated at all? 	*/
	DT_I(dtos_free2,(ctl, priv->raw,	/* release raw buffer 				*/
      F, "raw buffer"));

  if (priv->async)
  {
    strcpy(priv->capturing_died,		/* we are going down 				*/
	  "closing");
    ReleaseSemaphore(priv->grab, 1,		/* signal death to capture thread	*/
      NULL);

    DT_I(dtfb_vfw_wait,(ctl, priv,
      priv->done, 100, "grab to die",
      priv->capturing_died,
      sizeof(priv->capturing_died)));

    DT_I(dtos_delay,(ctl, 100));		/* wait even longer				    */
  
	ACTION(priv, "capCaptureStop")
    capCaptureStop(priv->hwnd);	
    ACTION(priv, "capCaptureAbort")
    capCaptureAbort(priv->hwnd);
    DestroyWindow(priv->hwnd);
	DT_I(dtfb_vfw_messages,(ctl, 
      priv));
  }

cleanup:

dt_dbg(ctl, F, DMASK, "@%ld priv->header=0x%08lx", __LINE__, priv->header);
  if (priv->header != NULL)
    DT_I(dtos_free2,(ctl, 
      priv->header,
      F, "bitmapinfo"));

# if DTPRJ_WINNOV == DT_1
  if (priv->winnov_open)
    WINNOV_I(WnvVfe_Close,(priv->hwnd));
# endif

  {
  long e_code = 0;
  __try {
    if (priv->hwnd != NULL)
    {
	  ACTION1(priv, "capDriverDisconnect")
dt_dbg(ctl, F, DMASK, "@%ld priv->hwnd=0x%08lx", __LINE__, priv->hwnd);
      capDriverDisconnect(priv->hwnd);
    }
  }
  __except(e_code = _exception_code(),  /* save code if exception happens 	*/
		   EXCEPTION_EXECUTE_HANDLER)
  {
  }

  if (e_code != 0)						/* an exception happened? 			*/
  {
	rc = dt_err(ctl, F,
      "exception 0x%08lx"
      " doing capDriverDisconnect(0x%08lx)",
      e_code, priv->hwnd);
  }
  }
	
dt_dbg(ctl, F, DMASK, "@%ld priv->done=0x%08lx", __LINE__, priv->done);
  if (priv->done != (HANDLE)NULL)
  {
	CloseHandle(priv->done);
	priv->done = NULL;
  }

  if (priv->grab != (HANDLE)NULL)
  {
	CloseHandle(priv->grab);
	priv->grab = NULL;
  }

  if (priv->portio_initialized)
    DT_I(dtportio_uninit,(ctl,
      &priv->portio));

  if (priv->spec != NULL)
    DT_I(dtos_free2,(ctl, priv->spec,
      F, "spec"));

#ifdef USE_GLOBAL_PRIV
  DT_MEMSET(priv, 0, sizeof(*priv));
#else
  DT_I(dtos_free2,(ctl, priv,
    F, "priv"));
#endif

  return rc;
}

/*..........................................................................
 *..........................................................................*/

static
dt_rc_e
dtfb_vfw_winnov(						/* set winnov value					*/
  dt_ctl_t *ctl,
  dtfb_vfw_priv_t *priv,
  const char *which,
  char *what)
{
  DT_F("dtfb_vfw_winnov");
#if DTPRJ_WINNOV == DT_1
  int n;
  long v;
  dt_rc_e rc;

  DT_C(dtstr_tokenize,(ctl, what, ',',
    &n));
  DT_C(dtstr_to_long,(ctl, what, &v));	/* decode numeric value 			*/

  if (DT_ABBREV(which, "brightness"))	/* brightness being changed? 		*/
  {
    dt_dbg(ctl, F, DT_DBG_MASK_COMMAND,
      "%s to %d",
      which, v);
	WINNOV_C(WnvVfe_SetBrightness,(
      priv->hwnd, v));
  }
  else
  if (DT_ABBREV(which, "contrast"))		/* contrast being changed? 			*/
  {
    dt_dbg(ctl, F, DT_DBG_MASK_COMMAND,
      "%s to %d",
      which, v);
	WINNOV_C(WnvVfe_SetContrast,(
      priv->hwnd, v));
  }
  else
  if (DT_ABBREV(which, "hue"))			/* hue being changed? 				*/
  {
    dt_dbg(ctl, F, DT_DBG_MASK_COMMAND,
      "%s to %d",
      which, v);
	WINNOV_C(WnvVfe_SetHue,(
      priv->hwnd, v));
  }
  else
  if (DT_ABBREV(which, "pan"))			/* pan being changed? 				*/
  {
	long h = v;
	if (n > 1)
	{
	  DT_C(dtstr_to_long,(ctl, 			/* decode numeric value 			*/ 
        what + strlen(what) + 1,
        &v));
	}
	else
	  v = 0;
    dt_dbg(ctl, F, DT_DBG_MASK_COMMAND,
      "%s to %d,%d",
      which, h, v);
	WINNOV_C(WnvVfe_SetPan,(
      priv->hwnd, h, v));
  }
  else
  if (DT_ABBREV(which, "quality"))		/* quality being changed? 			*/
  {
    dt_dbg(ctl, F, DT_DBG_MASK_COMMAND,
      "%s to %d",
      which, v);
	WINNOV_C(WnvVfe_SetQuality,(
      priv->hwnd, v));
  }
  else
  if (DT_ABBREV(which, "saturation"))	/* saturation being changed? 		*/
  {
    dt_dbg(ctl, F, DT_DBG_MASK_COMMAND,
      "%s to %d",
      which, v);
	WINNOV_C(WnvVfe_SetSaturation,(
      priv->hwnd, v));
  }
  else
  if (DT_ABBREV(which, "source"))		/* source being changed? 			*/
  {
    dt_dbg(ctl, F, DT_DBG_MASK_COMMAND,
      "%s to %d",
      which, v);
	WINNOV_C(WnvVfe_SetSource,(
      priv->hwnd, v));
  }
  else
  if (DT_ABBREV(which, "zoom"))			/* zoom being changed? 				*/
  {
    dt_dbg(ctl, F, DT_DBG_MASK_COMMAND,
      "%s to %d",
      which, v);
	WINNOV_C(WnvVfe_SetZoom,(
      priv->hwnd, v));
  }

cleanup:
  return rc;
#else
  return dt_err_notlinked(ctl, F, "winnov hardware");
#endif
}

/*..........................................................................
 *..........................................................................*/

static
dt_rc_e
dtfb_vfw_source(						/* change signal source				*/
  dt_ctl_t *ctl,
  dtfb_t *fb,
  const char *source)
{
  DT_F("dtfb_vfw_source");
  dtfb_vfw_priv_t *priv = fb->priv;
  char *s = NULL;
  char *p;
  int n, i;
  dt_rc_e rc;

  if (source == NULL)
    goto cleanup;

  DT_C(dtos_strdup,(ctl, source, &s)); 
  DT_C(dtstr_tokenize,(ctl, s, '+',
    &n));

  p = s;
  for (i=0; i<n; i++)
  {
	if (DT_ABBREV(p, "brightness,") ||
		DT_ABBREV(p, "contrast,") ||
		DT_ABBREV(p, "hue,") ||
		DT_ABBREV(p, "pan,") ||
		DT_ABBREV(p, "quality,") ||
		DT_ABBREV(p, "saturation,") ||
		DT_ABBREV(p, "source,") ||
		DT_ABBREV(p, "zoom,"))
	{
	  if (DT_ABBREV(priv->board_type, "Winnov") ||
		  DT_ABBREV(priv->board_type, "Videum"))
	  {
		*strchr(p, ',') = '\0';
		DT_C(dtfb_vfw_winnov,(ctl,		/* set winnov value					*/
          priv, 
          p, p+strlen(p)+1));
	  }
	  else
	  {
		rc = dt_err(ctl, F, 
          "board_spec \"%s\" does not"
          " support %s", 
          priv->board_spec, p);
	  }
	}
	else
	if (DT_ABBREV(p, "vidmux,"))
	{
	  long port, value, delay;
	  DT_C(dtfb_vidmux_parse,(ctl,		/* parse the vidmux string 			*/
        strchr(p, ',')+1, 
        &port, &value, &delay));

	  if (!priv->portio_initialized)	/* ports not accessed yet? 			*/
	    DT_C(dtportio_init,(ctl,		/* access ports directly		    */
          &priv->portio, NULL));
	  priv->portio_initialized = 1;

	  DT_C(dtfb_vidmux_switch,(			/* switch the vidmux 				*/
        ctl,
        &priv->portio, port, value));
	  
	  DT_Q(dtos_time,(ctl,				/* current time at switch			*/
        &priv->switch_time));
	  priv->switch_delay = delay;		/* delay just before getblock		*/
	}
	else
	{
	  rc = dt_err(ctl, F,  
        "\"%s\" is not a valid command", 
        p);
	  goto cleanup;
	}
	p = p + strlen(p) + 1;
  }
cleanup:
  if (s != NULL)
    DT_I(dtos_free2,(ctl, s,
      F, "source string"));
  return rc;
}

/*..........................................................................
 *..........................................................................*/

static
dt_rc_e
dtfb_vfw_uninit(						/* release resources 				*/
  dt_ctl_t *ctl,
  dtfb_t *fb)
{
  DT_F("dtfb_vfw_uninit");
  dtfb_vfw_priv_t *priv = fb->priv;
  DT_Q(dtfb_vfw_release,(ctl, priv));
  return DT_RC_GOOD;
}

#endif

/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtfb_vfw_init(
  dt_ctl_t *ctl,
  dtfb_t *fb,
  const char *spec)
{
  DT_F("dtfb_vfw_init");
# if DTPRJ_VFW == DT_1 && !defined(__cplusplus)
  dtfb_vfw_priv_t *priv = NULL;
  long hwnd;
  DWORD e;
  int ntokens;
  char *p;
  long longval;
  LRESULT result;
  WINDOWPLACEMENT placement;
  BOOLEAN boolrc;
  int hsize;
  int i, j;
  int isnumber;
  CAPTUREPARMS captureparms;
  dt_rc_e rc;

  DTCSTACK_HIGHWATER;

  if (spec == NULL)
  {
    rc = dt_err(ctl, F,
      "NULL vfw spec");
    goto cleanup;
  }
  else
  if (strlen(spec) == 0)
  {
    rc = dt_err(ctl, F,
      "zero-length vfw spec");
    goto cleanup;
  }

#ifdef USE_GLOBAL_PRIV
  priv = &global_priv;
#else
  DT_C(dtos_malloc2,(ctl,				/* space for private structure	    */
    (void **)&priv,
    sizeof(dtfb_vfw_priv_t),
    F, "priv"));
#endif
  DT_MEMSET(priv, 0,						/* clear private structure 			*/
    sizeof(dtfb_vfw_priv_t));
  
  priv->ctl = ctl;

  DT_C(dtos_strdup,(ctl, spec,			/* get space for spec edit 			*/
    &priv->spec));

  DT_C(dtstr_tokenize,(ctl,				/* separate spec into tokens 		*/
    priv->spec,
    ',', &ntokens));
										/* ................................ */
  p = priv->spec;

  if (ntokens >= 1)
  {
    priv->board_spec = p;
	p += strlen(p) + 1;
  }

  if (ntokens >= 2)						/* width 							*/
  {
    DT_C(dtstr_to_long,(ctl,
      p, &longval));
	fb->xe = longval;
	p += strlen(p) + 1;
  }
  else
    fb->xe = 352;

  if (ntokens >= 3)						/* height 							*/
  {
    DT_C(dtstr_to_long,(ctl,
      p, &longval));
	fb->ye = longval;
	p += strlen(p) + 1;
  }
  else
    fb->ye = 288;

  if (ntokens >= 4)						/* flags+flags						*/
  {
    priv->flag_nooverlay = !!strstr(p, "nooverlay");
    priv->flag_nosetsource = !!strstr(p, "nosetsource");
    priv->flag_nosetquality = !!strstr(p, "nosetquality");
    priv->flag_nosetvcr = !!strstr(p, "nosetvcr");
    priv->flag_minimize = !!strstr(p, "minimize");
    priv->flag_noshow = !!strstr(p, "noshow");
    priv->async = !!strstr(p, "async");
    priv->shutdown = !!strstr(p, "shutdown");
    priv->flag_writeraw = !!strstr(p, "writeraw");
    priv->flag_setsize = !!strstr(p, "setsize");
    priv->flag_srcdlg = !!strstr(p, "srcdlg");
    priv->flag_fmtdlg = !!strstr(p, "fmtdlg");
    priv->flag_cmpdlg = !!strstr(p, "cmpdlg");
    priv->flag_dspdlg = !!strstr(p, "dspdlg");
    priv->flag_mono = !!strstr(p, "mono");
    priv->flag_bmp24 = !!strstr(p, "bmp24");
    priv->flag_hwnd = !!strstr(p, "hwnd=");
  }

										/* ................................ */
  priv->grab = CreateSemaphore(NULL,
    0, 1, NULL);
  if (priv->grab == (HANDLE)NULL)
  {
	rc = dt_err(ctl, F,
      "CreateSemaphore failed");
	goto cleanup;
  }
  priv->done = CreateSemaphore(NULL,
    0, 1, NULL);
  if (priv->done == (HANDLE)NULL)
  {
	rc = dt_err(ctl, F,
      "CreateSemaphore failed");
	goto cleanup;
  }
										/* ................................ */
  j = -1;
  dtstr_to_long2(ctl, 
    priv->board_spec, &longval,
    &isnumber);
  if (isnumber)
	j = (int)longval;
  else
  for (i=0; i<10; i++)
  {
	char name[80];
	char version[80];
		 
    if (capGetDriverDescription(i,		/* get ith capture driver name 		*/
          name, sizeof(name),
          version, sizeof(version)))
	{
	  dt_dbg(ctl, F, DT_DBG_MASK_ALIMITS,
        "%d. driver name=\"%s\""
        " version=\"%s\"",
        i, name, version);
	  if (DT_ABBREV(name, 
            priv->board_spec))
	  {
		if (j == -1)					/* no board selected yet? 			*/
		{
		  j = i;
		  dtstr_printf(ctl,
            priv->board_type,
            sizeof(priv->board_type),
            "%s", name);
		}
		else							/* have already selected a board? 	*/
		{
		  rc = dt_err(ctl, F,
			"board type \"%s\""
            " is ambiguous",
            priv->board_spec);
		  goto cleanup;
		}
	  }
	}
  }
  if (j == -1)
  {
	rc = dt_err(ctl, F,
      "board type \"%s\" not found",
      priv->board_spec);
	goto cleanup;
  }
										/* ................................ */
  if (!priv->flag_hwnd)					/* no special window to use? 		*/
  {
    priv->hwnd = capCreateCaptureWindow(	/* create window for capture 		*/
      (LPSTR)(ctl->progname != NULL? 
         ctl->progname: "Video Monitor"),
      WS_OVERLAPPED | 
      WS_CAPTION |
      WS_MINIMIZEBOX |
      WS_SYSMENU,
      0, 0, fb->xe, fb->ye,
      NULL, 0);
  }
  else									/* need to be a child window? 		*/
  {
	DT_C(dtstr_to_long,(ctl,
      strstr(p, "hwnd=")+5, &hwnd));
    priv->hwnd =						/* create window for capture 		*/
      capCreateCaptureWindow( 
        (LPSTR)(ctl->progname != NULL? 
           ctl->progname: "Video Monitor"),
        WS_CHILD,
        0, 0, fb->xe, fb->ye,
        (HWND)hwnd, 0);
  }
  
  if (priv->hwnd == NULL)				/* creation of capture window failed? */
  {
    e = GetLastError();
	rc = dt_err(ctl, F,
      "capCreateCaptureWindow(\"%s\","
      " FLAGS,"
      " 0, 0, %d, %d, 0x%08lx, 0)"
      " failed"
      DTERRNO_LASTF,
      (LPSTR)(ctl->progname != NULL? 
         ctl->progname: "Video Monitor"),
      fb->xe, fb->ye, hwnd,
      DTERRNO_LASTE(e));
	goto cleanup;
  }

  ACTION(priv, "capSetUserData")
  capSetUserData(priv->hwnd, priv);		/* attach user address to window 	*/

										/* ................................ */
  rc = DT_RC_GOOD;
  {
  char doing[128];
  long e_code = 0;
  doing[0] = '\0';
  __try {
	ACTION(priv, "capDriverConnect")
    dtstr_printf(ctl, 
      doing, sizeof(doing),
      "capDriverConnect(0x%08lx, %d)",
      (long)priv->hwnd, j);
    result = capDriverConnect(			/* connect capture driver to window */
      priv->hwnd, j);
dt_dbg(ctl, F, DMASK, "@%ld priv->rc=%ld", __LINE__, priv->rc);
	RESULT(priv);
dt_dbg(ctl, F, DMASK, "@%ld result=%ld", __LINE__, result);
	if (result != TRUE)
    {
	  char s[256];
      e = GetLastError();
      dtstr_printf(ctl, 
        doing, sizeof(doing),
        "GetLastError");
	  DTERRNO_LASTS(e, s, sizeof(s));
	  rc = dt_err(ctl, F,
        "capDriverConnect(0x%08lx, %d)"
        " failed"
        DTERRNO_LASTFS
        ", YOU MAY HAVE TO REBOOT YOUR COMPUTER",
        (long)priv->hwnd, j,
        DTERRNO_LASTES(e, s));
	  if (priv->shutdown)				/* supposed to shut down on this? 	*/
	    rc = DT_RC_SHUTDOWN;
	}

	dtstr_printf(ctl, 
      doing, sizeof(doing),
        "dtfb_vfw_wait(ctl, priv, 0x%08lx, 200, ...", priv->done);
dt_dbg(ctl, F, DMASK, "@%ld rc=%d", __LINE__, rc);
    rc = dtfb_vfw_wait(ctl, priv,
      priv->done, 250, "initial messages",
      NULL, 0);
dt_dbg(ctl, F, DMASK, "@%ld rc=%d (0x%08lx)", __LINE__, rc, (long)rc);
  }

  __except(e_code = _exception_code(),  /* save code if exception happens 	*/
		   EXCEPTION_EXECUTE_HANDLER)
  {
  }

  if (e_code != 0)						/* an exception happened? 			*/
  {
	rc = dt_err(ctl, F,
      "exception 0x%08lx"
      " doing %s",
      ", YOU MAY HAVE TO REBOOT YOUR COMPUTER",
      e_code, doing);
  }
  }

dt_dbg(ctl, F, DMASK, "@%ld rc=%d", __LINE__, rc);
  if (rc != DT_RC_GOOD)
    goto cleanup;

# if DTPRJ_WINNOV == DT_1
  if (DT_ABBREV(priv->board_type, "Winnov") ||
	  DT_ABBREV(priv->board_type, "Videum"))
  {
    WINNOV_C(WnvVfe_Open,(priv->hwnd));	/* this causes error callback	    */
    priv->winnov_open = 1;
	if (!priv->flag_nosetsource)		/* caller does not prohibit? 		*/
	  WINNOV_C(WnvVfe_SetSource,(		/* select composite input 			*/
        priv->hwnd, 2));
	if (!priv->flag_nosetquality)		/* caller does not prohibit? 		*/
	  WINNOV_C(WnvVfe_SetQuality,(		/* select high quality (low compr)	*/
        priv->hwnd, 100));
	if (!priv->flag_nosetvcr)			/* caller does not prohibit? 		*/
	  WINNOV_C(WnvVfe_SetVcr,(			/* select vcr (field) mode			*/
        priv->hwnd, TRUE));
	priv->flag_setsize = 1;				/* always OK for videum 			*/
	{
	  int i;
	  for (i=0; i<8; i++)
	  {
		char name[80];
		int winnov_rc;
		winnov_rc = WnvVfe_GetSourceName(priv->hwnd, i, name);
		if (winnov_rc != WNV_VFE_ERR_NO_ERROR)
		  strcpy(name, "nonexistent");
		dt_dbg(ctl, F, DT_DBG_MASK_ALIMITS,
		  "%d. \"%s\"", i, name);
	  }
	}
  }
# endif
  
  ACTION(priv, "capSetCallbackOnError")
  capSetCallbackOnError(priv->hwnd,		/* define the error callback 		*/
    (FARPROC)dtfb_vfw_error);
  RESULT(priv);

  ACTION(priv, "capOverlay")
  capOverlay(priv->hwnd, 
    priv->flag_nooverlay? FALSE: TRUE);
  RESULT(priv);

  DT_C(dt_yield,(ctl, "overlay enabled"));

  ACTION(priv, "capCaptureGetSetup")
  capCaptureGetSetup(priv->hwnd, 
    &captureparms, sizeof(captureparms));
  RESULT(priv);

  captureparms.fYield = priv->async?	/* asynchronous operation		    */
    TRUE: FALSE;
  captureparms.wNumVideoRequested = 1;	/* single video buffer			    */
  captureparms.fAbortLeftMouse = TRUE;

  ACTION(priv, "capCaptureSetSetup")
  capCaptureSetSetup(priv->hwnd, 
    &captureparms, sizeof(captureparms));
  RESULT(priv);

  if (priv->async)						/* running asynchronously?		    */
  {
    ACTION(priv, 
      "capSetCallbackOnVideoStream")
    capSetCallbackOnVideoStream(		/* define the video callback 		*/
      priv->hwnd,
      (FARPROC)dtfb_vfw_video);
    RESULT(priv);
  }
  else
  {
    ACTION(priv, "capSetCallbackOnFrame")
    capSetCallbackOnFrame(				/* define the frame callback 		*/
      priv->hwnd,
      (FARPROC)dtfb_vfw_video);
    RESULT(priv);
  }

# ifdef CALLBACK_YIELD
  ACTION(priv, "capSetCallbackOnYield")
  capSetCallbackOnYield(priv->hwnd,		/* define the yield callback 		*/
    (FARPROC)dtfb_vfw_yield);
  RESULT(priv);

  ACTION(priv, "capSetCallbackOnCapControl")
  capSetCallbackOnCapControl(priv->hwnd,
    (FARPROC)dtfb_vfw_capcontrol);
  RESULT(priv);
# endif

										/* ................................ */
  ACTION(priv, "capGetVideoFormatSize")
  hsize = capGetVideoFormatSize(
    priv->hwnd);
  RESULT(priv);

  DT_C(dtos_malloc2,(ctl, 
    (void **)&priv->header, hsize,
    F, "bitmapinfo"));

  ACTION(priv, "capGetVideoFormat 1")	/* initial format 					*/
  (void)capGetVideoFormat(priv->hwnd,
    priv->header, hsize);
  RESULT(priv);

  fb->priv = priv;
  DT_C(dtfb_vfw_report,(ctl, fb, "1"));	/* report initial status			*/

										/* ................................ */
  if (priv->flag_srcdlg)				/* supposed to dialog source? 		*/
  {
	ACTION(priv, "capDlgVideoSource")
	capDlgVideoSource(priv->hwnd);
	RESULT(priv);
  }

  if (priv->flag_fmtdlg)				/* supposed to dialog format? 		*/
  {
	ACTION(priv, "capDlgVideoFormat")
	capDlgVideoFormat(priv->hwnd);
	RESULT(priv);
  }

  if (priv->flag_cmpdlg)				/* supposed to dialog compression? 	*/
  {
	ACTION(priv, "capDlgVideoCompression")
	capDlgVideoCompression(priv->hwnd);
	RESULT(priv);
  }

  if (priv->flag_dspdlg)				/* supposed to dialog display?	 	*/
  {
	ACTION(priv, "capDlgVideoDisplay")
	capDlgVideoDisplay(priv->hwnd);
	RESULT(priv);
  }

  if (priv->flag_srcdlg ||				/* we had a dialog? 				*/
	  priv->flag_fmtdlg ||
	  priv->flag_cmpdlg ||
	  priv->flag_dspdlg)
  {
	ACTION(priv, "capGetVideoFormat 2")
	(void)capGetVideoFormat(priv->hwnd,
      priv->header, hsize);
	RESULT(priv);
	DT_C(dtfb_vfw_report,(ctl, fb,		/* report status after dialog		*/
      "2"));
  }
  
										/* ................................ */

  if (priv->flag_setsize)				/* supposed to set format? 			*/
  {
	priv->header->biWidth = fb->xe;
	priv->header->biHeight = fb->ye;

	ACTION(priv, "capSetVideoFormat")
	capSetVideoFormat(priv->hwnd,		/* set image size and format		*/
      priv->header, hsize);
	RESULT(priv);

	ACTION(priv, "capGetVideoFormat 3")
	(void)capGetVideoFormat(priv->hwnd,	/* get status after change 			*/
      priv->header, hsize);
	RESULT(priv);
	DT_C(dtfb_vfw_report,(ctl, fb,		/* report status after change		*/
      "3"));
  }
										/* ................................ */
										/* check for valid video format		*/

  if (priv->header->biCompression != BI_RGB)
  {
    DWORD b1 = priv->header->biCompression;
	char s1[32];
	DT_C(dtstr_printable,(ctl, 
      (char *)&b1, sizeof(b1),
      s1, sizeof(s1)));
	rc = dt_err(ctl, F,
      "invalid video format biCompression"
      " %s"
      " expecting R G B"
      "\r\n\r\nPLEASE RECONFIGURE YOUR VIDEO CARD TO RGB24, RGB TRUECOLOR or RGB MILLIONS",
      s1);
	goto cleanup;
  }

  if (priv->header->biPlanes != 1)
  {
	rc = dt_err(ctl, F,
      "invalid video format biPlanes"
      " %d"
      " expecting 1"
      "\r\n\r\nPLEASE RECONFIGURE YOUR VIDEO CARD TO RGB24, RGB TRUECOLOR or RGB MILLIONS",
      priv->header->biPlanes);
	goto cleanup;
  }

  if (priv->header->biBitCount != 8 &&
	  priv->header->biBitCount != 16 &&
	  priv->header->biBitCount != 24)
  {
	rc = dt_err(ctl, F,
      "invalid video format biBitCount"
      " %d"
      " expecting 8, 16 or 24"
      "\r\n\r\nPLEASE RECONFIGURE YOUR VIDEO CARD TO RGB24, RGB TRUECOLOR or RGB MILLIONS",
      priv->header->biBitCount);
	goto cleanup;
  }

  {
	char s[80];
	ACTION(priv, "capGetMCIDeviceName")
	capGetMCIDeviceName(priv->hwnd, s, sizeof(s));
	RESULT(priv);
  }
										/* ................................ */
  fb->xe = priv->header->biWidth;		/* always uses vfw's preferred size */
  fb->ye = priv->header->biHeight;

  placement.length = sizeof(placement);
  boolrc = GetWindowPlacement(
    priv->hwnd, &placement);
  if (boolrc == FALSE)
	DT_CLEANUP((ctl, F,
      "GetWindowPlacement() failed"));
  boolrc = MoveWindow(priv->hwnd,
    placement.rcNormalPosition.left,
    placement.rcNormalPosition.top,
    fb->xe, fb->ye, FALSE);
  if (boolrc == FALSE)
	DT_CLEANUP((ctl, F,
      "MoveWindow() failed"));

  if (!priv->flag_noshow)
  {
	ShowWindow(priv->hwnd,				/* SHOW THE WINDOW 					*/
	  priv->flag_minimize? 
	  SW_SHOWMINNOACTIVE: SW_SHOW);
  }
										/* ................................ */

  fb->xe = priv->header->biWidth;
  fb->ye = priv->header->biHeight;
  
  fb->uninit   = dtfb_vfw_uninit;
  fb->control  = dtfb_vfw_control;
  fb->getblock = dtfb_vfw_getblock;
  fb->source   = dtfb_vfw_source;

  if (priv->async)						/* running asynchronously?		    */
  {
    long parent = GetCurrentThreadId();
	int i;
	ACTION(priv, "capCaptureSequenceNoFile")
	capCaptureSequenceNoFile(priv->hwnd);
    if (GetCurrentThreadId() != parent)
	{
	  ACTION(priv, "capCaptureAbort")
	  capCaptureAbort(priv->hwnd);
	  ExitThread(0);					/* probably don't get here		    */
	}
	RESULT(priv);

	for (i=0; i<30; i++)
	{
	  DT_C(dt_yield,(ctl, "grabbing starting"));
	  DT_C(dtos_delay,(ctl, 100));
	}
	DT_C(dt_yield,(ctl, "grabbing started"));
  }

										/* ................................ */

  if (priv->header->biBitCount == 8)	/* camera is giving 8-bit? 			*/
	priv->flag_mono = 1;

  if (priv->flag_mono)					/* monochrome input? 				*/
  {
	fb->org.org = DT_COLOR_ORG_LUMINANCE;
	fb->rowsize = fb->xe;				/* one byte per pixel 				*/
  }
  else
  if (priv->flag_bmp24)					/* want bmp24 output?				*/
  {
	fb->org.org  = DT_COLOR_ORG_BMP24;
	fb->rowsize  = 3 * fb->xe;			/* three bytes per pixel 			*/
  }
  else									/* color input? 					*/
  {
	fb->org.org  = DT_COLOR_ORG_RGB888;
	fb->rowsize  = 3 * fb->xe;			/* three bytes per pixel 			*/
  }

cleanup:
  if (rc != DT_RC_GOOD)
  {
	DT_I(dtfb_vfw_release,(ctl, priv));
	DT_MEMSET(fb, 0, sizeof(*fb));
  }

  return rc;
# else
  return dt_err_notlinked(ctl, F, DESC);  
# endif
}

/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtfb_vfw_rev(							/* deliver revision 				*/
  dt_ctl_t *ctl,						/* environment control 				*/
  const char **rev_string,				/* returned rev string 				*/
  int *rev_major,						/* returned rev major 				*/
  int *rev_minor,						/* returned rev minor 				*/
  long *rev_date)						/* returned rev date 				*/
{
  if (rev_string != NULL)
    *rev_string = "dtfb_vfw lib";
  if (rev_major != NULL)
    *rev_major = 1;
  if (rev_minor != NULL)
    *rev_minor = 22;
  if (rev_date != NULL)
    *rev_date = 19981201;
  return DT_RC_GOOD;
}





/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
  ! Copyright (C) 1998 Dtack Inc. All Rights Reserved
  ! To use this file, you must have signed a license agreement with Dtack Inc.
  ! Under no circumstances may you redistribute this file.
  ! This software is provided AS IS with no warranty expressed or implied.
  ! Dtack Inc. accepts no liability for use or misuse of this file.
  ! http://www.dtack.com  dtack@dtack.com  telephone +360.417.6700
  ! Dtack Inc, P.O. Box 3061, Port Angeles, WA USA 98362
  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */
