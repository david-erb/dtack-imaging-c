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
  19981231 DE snappy.h not inside ifdef
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
| ERRORS
| As yet undocumented.
| 
| REV
| 1.5 19970525 fixed bug which turned 24-bit into 8 bit
| 
| END
 *..........................................................................*/

#include <dtack/base.h>
DT_RCSID("source/fb $RCSfile: fbsnap.c,v $ $Revision: 1.7 $");
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

#define DESC "Snappy"

extern dtfb_init_f dtfb_snap_init;

#if DTPRJ_SNAP == DT_1 && !defined(__cplusplus)

#ifndef DTDEPEND
#include "snappy.h"
#endif

typedef struct {
  SNAPPTR snappy;
  HGLOBAL dib;
} dtfb_snap_priv_t;

/*..........................................................................
 *..........................................................................*/

static
dt_rc_e
dtfb_snap_control(
  dt_ctl_t *ctl,
  dtfb_t *fb,
  dtfb_control_e control)
{
  DT_F("dtfb_snap_control");

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
dtfb_snap_getblock(
  dt_ctl_t *ctl,
  dtfb_t *fb,
  void *data,
  int x0,
  int y0,
  int xe,
  int ye)
{
  DT_F("dtfb_snap_getblock");
  dtfb_snap_priv_t *priv = fb->priv;
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

  DT_C(dtfb_snap_messages,(ctl, priv));	/* check for messages on window		*/
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

    DT_C(dtfb_snap_wait,(ctl, priv,
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
      "fbsnap.raw"));
    priv->flag_writeraw = 0;			/* only do it once 					*/
  }

cleanup:
  return rc;
}

/*..........................................................................
 *..........................................................................*/

static
dt_rc_e
dtfb_snap_source(						/* change signal source				*/
  dt_ctl_t *ctl,
  dtfb_t *fb,
  const char *source)
{
  DT_F("dtfb_snap_source");
  dtfb_snap_priv_t *priv = fb->priv;
  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/

static
dt_rc_e
dtfb_snap_uninit(						/* release resources 				*/
  dt_ctl_t *ctl,
  dtfb_t *fb)
{
  DT_F("dtfb_snap_uninit");
  dtfb_snap_priv_t *priv = fb->priv;
  if (priv->snappy)
	SnapFree(priv->snappy);
  if (priv->dib)
	SnapFreeDib(priv->dib);
  DT_I(dtos_free2,(ctl, priv,
    F, "priv"));
  DT_MEMSET(fb, 0, sizeof(*fb));
  return DT_RC_GOOD;
}

#endif

/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtfb_snap_init(
  dt_ctl_t *ctl,
  dtfb_t *fb,
  const char *spec)
{
  DT_F("dtfb_snap_init");
# if DTPRJ_SNAP == DT_1 && !defined(__cplusplus)
  dtfb_snap_priv_t *priv = NULL;
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
      "NULL snap spec");
    goto cleanup;
  }
  else
  if (strlen(spec) == 0)
  {
    rc = dt_err(ctl, F,
      "zero-length snap spec");
    goto cleanup;
  }

  DT_C(dtos_malloc2,(ctl,				/* space for private structure	    */
    (void **)&priv,
    sizeof(dtfb_snap_priv_t),
    F, "priv"));
  DT_MEMSET(priv, 0,					/* clear private structure 			*/
    sizeof(dtfb_snap_priv_t));
										/* ................................ */
  priv->snappy = SnapAlloc();
  version = SnapGetDLLVersion();
  dtstr_printf(ctl, 
    priv->version, 
    sizeof(priv->version),
    "%d.%d.%d",
    HIBYTE(HIWORD(version)),
    LOBYTE(HIWORD(version)),
    LOWORD(version));
  priv->dib = SnapScanAndProcess(
    priv->snappy, 0);
										/* ................................ */

  fb->xe = priv->header->biWidth;
  fb->ye = priv->header->biHeight;
  
  fb->uninit   = dtfb_snap_uninit;
  fb->control  = dtfb_snap_control;
  fb->getblock = dtfb_snap_getblock;
  fb->source   = dtfb_snap_source;

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
	DT_I(dtfb_snap_release,(ctl, priv));
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
dtfb_snap_rev(							/* deliver revision 				*/
  dt_ctl_t *ctl,						/* environment control 				*/
  const char **rev_string,				/* returned rev string 				*/
  int *rev_major,						/* returned rev major 				*/
  int *rev_minor,						/* returned rev minor 				*/
  long *rev_date)						/* returned rev date 				*/
{
  if (rev_string != NULL)
    *rev_string = "dtfb_snap lib";
  if (rev_major != NULL)
    *rev_major = 1;
  if (rev_minor != NULL)
    *rev_minor = 1;
  if (rev_date != NULL)
    *rev_date = 19981213;
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
