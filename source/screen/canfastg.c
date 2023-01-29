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
#include <dtack/lut8.h>
#include <dtack/keymap.h>
#include <dtack/keybd.h>
#include <dtack/os.h>
#include <dtack/list.h>
#include <dtack/svc.h>
#include <dtack/canvas.h>

#include <dtack/vga.h>
#include <dtack/prj.h>

#define DESC "Fastgraph library"

extern dtcanvas_init_f dtcanvas_fastgraph_init;

#if DTPRJ_FASTGRAPH == DT_1 && !defined(DTCONFIG_IMPLIED_MSW)
const int dtcanvas_fastgraph_dummy = 0;

static dtcanvas_uninit_f dtcanvas_fastgraph_uninit;
static dtcanvas_open_f dtcanvas_fastgraph_open;
static dtcanvas_input_f dtcanvas_fastgraph_input;
static dtcanvas_close_f dtcanvas_fastgraph_close;
static dtcanvas_putblock_f dtcanvas_fastgraph_putblock;
static dtcanvas_getblock_f dtcanvas_fastgraph_getblock;
static dtcanvas_putcolor_f dtcanvas_fastgraph_putcolor;
static dtcanvas_getcolor_f dtcanvas_fastgraph_getcolor;
static dtcanvas_clear_f dtcanvas_fastgraph_clear;
static dtcanvas_cursor_f dtcanvas_fastgraph_cursor;

#include <dtack/nofar.h>
#include <dtack/fastg.h>

#ifndef FG_CDECL						/* this wasn't in the header file? 	*/
#  define FASTGRAPH3					/* we must be using fastgraph3 		*/
#endif

typedef enum {
  MOUSE_OFF = 0,
  MOUSE_ON
} mouse_e;

typedef struct {
  int mode;
  int oldmode;
# ifdef FASTGRAPH3						/* compiling for fastgraph3?		*/
  char *buf;							/* lookup/clip/flip buffer		    */
  unsigned int bufsize;
# endif
  dtkeybd_t keyboard;
  mouse_e mouse;
  int mousex, mousey;
  int click_state;
  dtos_time_t click_time;
  dtos_time_t unclick_time;
} dtcanvas_fastgraph_priv_t;


/*..........................................................................
 *..........................................................................*/

static
dt_rc_e
dtcanvas_fastgraph_uninit(
  dt_ctl_t *ctl,
  dtcanvas_t *canvas)
{
  DT_F("dtcanvas_fastgraph_uninit");
  dtcanvas_fastgraph_priv_t *priv =
    canvas->priv;
  dt_rc_e rc = DT_RC_GOOD;

  DTCSTACK_HIGHWATER;

# ifdef FASTGRAPH3						/* compiling for fastgraph3?		*/

  DT_I(dtos_free2,(ctl,					/* release the internal buffer 		*/
    priv->buf,
    F, "priv buffer"));  

# endif

  DT_I(dtkeybd_uninit,(ctl,				/* get rid of our keyboard			*/
    &priv->keyboard));

  DT_I(dtos_free2,(ctl, 
    canvas->priv,
    F, "priv"));  
  return rc;
}

/*..........................................................................
 *..........................................................................*/

static
dt_rc_e
dtcanvas_fastgraph_open(
  dt_ctl_t *ctl,
  dtcanvas_t *canvas)
{
  DT_F("dtcanvas_fastgraph_open");
  dtcanvas_fastgraph_priv_t *priv =
    canvas->priv;

  DTCSTACK_HIGHWATER;

  fg_setmode(priv->mode);				/* put us in the graphics mode		*/
  
  {
	int status = fg_mouseini();			/* try to enable the mouse 			*/
    if (status < 0)						/* no mouse or mouse driver? 		*/
	  priv->mouse = MOUSE_OFF;
	else
	  priv->mouse = MOUSE_ON;
  }

  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/
static
dt_rc_e 
mousefunc(
  dt_ctl_t *ctl,
  dtcanvas_t *canvas,
  int *x,
  int *y,
  int *buttons)
{
  int buts;
  fg_mousepos(x, y, &buts);				/* poll mouse status 				*/
  *buttons = 0;
  if (buts & 0x01) *buttons |= 0x01;
  if (buts & 0x02) *buttons |= 0x02;	/* middle button is second bit 		*/
  if (buts & 0x04) *buttons |= 0x04;	/* right button is third bit 		*/
  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/

static
dt_rc_e
dtcanvas_fastgraph_input(				/* stop and wait for input		    */
  dt_ctl_t *ctl,
  dtcanvas_t *canvas,
  dtsvc_t *svc,
  double timeout,
  int flag,
  dt_event_t *event)
{
  DT_F("dtcanvas_fastgraph_input");
  dtcanvas_fastgraph_priv_t *priv =
    canvas->priv;

  DTCSTACK_HIGHWATER;

  DT_Q(dtcanvas_poll,(ctl,				/* this function does all the work 	*/
	canvas,
    event,
	timeout,
    flag,
    &priv->keyboard,
    mousefunc,
    &priv->mousex,
    &priv->mousey,
    &priv->unclick_time,
    &priv->click_state));

  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/

static
dt_rc_e
dtcanvas_fastgraph_close(
  dt_ctl_t *ctl,
  dtcanvas_t *canvas)
{
  DT_F("dtcanvas_fastgraph_close");
  dtcanvas_fastgraph_priv_t *priv =
    canvas->priv;

  DTCSTACK_HIGHWATER;

  if (priv->mouse != MOUSE_OFF)			/* mouse not already off? 			*/
  {
    fg_mousefin();						/* disable mouse 					*/
	priv->mouse = MOUSE_OFF;
  }
  
  fg_setmode(priv->oldmode);			/* put us in the original mode		*/

										/* don't use fg_reset() because		*/
										/*   it outputs stuff to stdout		*/
  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/

#ifdef FASTGRAPH3						/* compiling for fastgraph3?		*/

static
dt_rc_e
dtcanvas_fastgraph_putblock(			/* using fg_putbuffer not vbpaste	*/
  dt_ctl_t *ctl,
  dtcanvas_t *canvas,
  void *data,
  int x0,
  int y0,
  int xe,
  int ye)
{
  DT_F("dtcanvas_fastgraph_putblock");
  dtcanvas_fastgraph_priv_t *priv =
    canvas->priv;
  int w = xe;
  int y;
  int nrows;
  char *buf = priv->buf;
  char *pbot;
  char *p;

  DTCSTACK_HIGHWATER;

  DT_Q(dtcanvas_clip,(ctl,				/* do clipping					    */
    canvas, data, x0, y0, xe, ye,
    &data, &x0, &y0, &w, &ye));

  if (xe < 0 || ye < 0)					/* picture entirely clipped out?    */
    return DT_RC_GOOD;
  
  fg_move(x0, y0);						/* top of block						*/

  nrows = (int)(priv->bufsize / w);		/* rows that fit in our buffer		*/
      
  pbot = buf + nrows * w;				/* point into bottom of buffer		*/

  for (y=0; y<ye; y+=nrows)				/* for each buf full				*/
  {
	if (y+nrows > ye)					/* partial buffer at end?			*/
	{
	  nrows = ye - y;
	  p = buf + nrows * w;				/* point into bottom of buffer		*/
	}
	else
	  p = pbot;
	while (p > buf)						/* copy rows from top of data		*/
	{
	  p -= w;
	  DT_MEMCPY(p, data, w);
	  data = (char *)data + xe;
	}
	fg_moverel(0, nrows);				/* down next buf					*/
	fg_putimage(buf, w, nrows);			/* display buf full					*/
  }

  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/

static
dt_rc_e
dtcanvas_fastgraph_getblock(			/* using getbuffer not vbcut 		*/
  dt_ctl_t *ctl,
  dtcanvas_t *canvas,
  void *data,
  int x0,
  int y0,
  int xe,
  int ye)
{
  DT_F("dtcanvas_fastgraph_getblock");
  dtcanvas_fastgraph_priv_t *priv =
    canvas->priv;
  int w = xe;
  int y;
  int nrows;
  char *buf = priv->buf;
  char *pbot;
  char *p;

  DTCSTACK_HIGHWATER;

  DT_Q(dtcanvas_clip,(ctl,				/* do clipping					    */
    canvas, data, x0, y0, xe, ye,
    &data, &x0, &y0, &w, &ye));

  if (xe < 0 || ye < 0)					/* picture entirely clipped out?    */
    return DT_RC_GOOD;

  fg_move(x0, y0);						/* top of block						*/

  nrows = (int)(priv->bufsize / w);	/* rows that fit in our buffer		*/
      
  pbot = buf + nrows * w;				/* point into bottom of buffer		*/

  for (y=0; y<ye; y+=nrows)				/* for each buf full				*/
  {
	if (y+nrows > ye)					/* partial buffer at end?			*/
	{
	  nrows = ye - y;
	  p = buf + nrows * w;				/* point into bottom of buffer		*/
	}
	else
	  p = pbot;
	fg_moverel(0, nrows);				/* down next buf				  	*/
	fg_getimage(buf, w, nrows);			/* get buf							*/
	while (p > buf)						/* copy rows from top of data		*/
	{
	  p -= w;
	  DT_MEMCPY(data, p, w);
	  data = (char *)data + xe;
	}
  }

  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/

#else  									/* not  compiling for fastgraph3?	*/

static
dt_rc_e
dtcanvas_fastgraph_putblock(			/* using vbpaste not putbuffer 		*/
  dt_ctl_t *ctl,					
  dtcanvas_t *canvas,
  void *data,
  int x0,
  int y0,
  int xe,
  int ye)
{
  DT_F("dtcanvas_fastgraph_putblock");
  int w = xe;
  int handle = -1;
  int fgrc = -1;
  dt_rc_e rc = DT_RC_GOOD;

  dt_dbg(ctl, F, DT_DBG_MASK_PIXEL,
    "before clip x0=%d, y0=%d, xe=%d, ye=%d",
    x0, y0, xe, ye);

  DT_Q(dtcanvas_clip,(ctl,				/* do clipping					    */
    canvas, data, x0, y0, xe, ye,
    &data, &x0, &y0, &w, &ye));

  dt_dbg(ctl, F, DT_DBG_MASK_PIXEL,
    "after  clip x0=%d, y0=%d, xe=%d, ye=%d",
    x0, y0, xe, ye);

  if (xe < 0 || ye < 0)					/* picture entirely clipped out?    */
    return DT_RC_GOOD;
  
  handle = fg_vbdefine(data, xe, ye);	/* virtual buffer around our data 	*/
  if (handle < 0 || handle > 31)
  {
	rc = dt_err(ctl, F,
      "invalid handle %d returned"
      " by fg_vbdefine(data, %d, %d)",
      handle, xe, ye);
	goto cleanup;
  }
  fgrc  = fg_vbopen(handle);			/* make us the active virtual buffer */
  if (fgrc != 0)
  {
	rc = dt_err(ctl, F,
      "fg_vbopen(%d) failed, rc %d",
      handle, fgrc);
	goto cleanup;
  }

  fg_vbpaste(0, w-1, 0, ye-1,			/* virtual buffer to active page 	*/
    x0, y0+ye-1);

cleanup:
  if (fgrc == 0)
    fg_vbclose();

  if (handle >= 0 && handle <= 31)
    fg_vbundef(handle);

  return rc;
}

/*..........................................................................
 *..........................................................................*/

static
dt_rc_e
dtcanvas_fastgraph_getblock(			/* using vbcut not getbuffer 		*/
  dt_ctl_t *ctl,
  dtcanvas_t *canvas,
  void *data,
  int x0,
  int y0,
  int xe,
  int ye)
{
  DT_F("dtcanvas_fastgraph_getblock");
  int w;
  int handle = -1;
  int fgrc = -1;
  dt_rc_e rc = DT_RC_GOOD;

  dt_dbg(ctl, F, DT_DBG_MASK_PIXEL,
    "before clip x0=%d, y0=%d, xe=%d, ye=%d",
    x0, y0, xe, ye);

  DT_Q(dtcanvas_clip,(ctl,				/* do clipping					    */
    canvas, data, x0, y0, xe, ye,
    &data, &x0, &y0, &w, &ye));

  if (xe < 0 || ye < 0)					/* picture entirely clipped out?    */
    return DT_RC_GOOD;
  
  handle = fg_vbdefine(data, xe, ye);	/* virtual buffer around our data 	*/
  if (handle < 0 || handle > 31)
  {
	rc = dt_err(ctl, F,
      "invalid handle %d returned"
      " by fg_vbdefine(data, %d, %d)",
      handle, xe, ye);
	goto cleanup;
  }
  fgrc  = fg_vbopen(handle);			/* make us the active virtual buffer */
  if (fgrc != 0)
  {
	rc = dt_err(ctl, F,
      "fg_vbopen(%d) failed, rc %d",
      handle, fgrc);
	goto cleanup;
  }

  dt_dbg(ctl, F, DT_DBG_MASK_PIXEL,
    "fg_vbcut(%d, %d, %d, %d, 0, 0)",
    x0, x0+w-1, y0, y0+ye-1, 0, 0);

  fg_vbcut(x0, x0+w-1, y0, y0+ye-1,		/* active page to virtual buffer 	*/
    0, ye-1);

cleanup:
  if (fgrc == 0)
    fg_vbclose();

  if (handle >= 0 && handle <= 31)
    fg_vbundef(handle);

  return rc;
}

#endif 

/*..........................................................................
 *..........................................................................*/

static
dt_rc_e
dtcanvas_fastgraph_putcolor(
  dt_ctl_t *ctl,
  dtcanvas_t *canvas,
  dtlut8_t *lut,
  int start,
  int count)
{
  DT_F("dtcanvas_fastgraph_putcolor");
  struct {
    char r, g, b;
  } *rgb;
  int i;
  dt_rc_e rc;

  DTCSTACK_HIGHWATER;

  DT_C(dtos_malloc2,(ctl,				/* get temporary space for palette 	*/
    (void **)&rgb, 3*count,
    F, "palette"));

  for (i=0; i<count; i++)				/* convert lut to palette			*/
  {
    rgb[i].r = lut[i].r >> 2;
    rgb[i].g = lut[i].g >> 2;
    rgb[i].b = lut[i].b >> 2;
  }

  fg_setdacs(start, count,				/* set VGA palette					*/
    (char *)rgb);

cleanup:
  if (rgb != NULL)
    DT_I(dtos_free2,(ctl, rgb,
      F, "palette"));
  return rc;
}

/*..........................................................................
 *..........................................................................*/

static
dt_rc_e
dtcanvas_fastgraph_getcolor(
  dt_ctl_t *ctl,
  dtcanvas_t *canvas,
  dtlut8_t *lut,
  int start,
  int count)
{
  DT_F("dtcanvas_fastgraph_getcolor");

  DTCSTACK_HIGHWATER;
  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/

static
dt_rc_e
dtcanvas_fastgraph_clear(
  dt_ctl_t *ctl,
  dtcanvas_t *canvas,
  int background)
{
  DT_F("dtcanvas_fastgraph_clear");

  DTCSTACK_HIGHWATER;
  if (background == 0)
    fg_erase();
  else
  {
    fg_setcolor(background);
    fg_fillpage();
  }

  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/

static
dt_rc_e
dtcanvas_fastgraph_cursor(
  dt_ctl_t *ctl,
  dtcanvas_t *canvas,
  int *x,
  int *y,
  int op)
{
  DT_F("dtcanvas_fastgraph_cursor");
  dtcanvas_fastgraph_priv_t *priv =
    canvas->priv;

  DTCSTACK_HIGHWATER;

  if (op != DT_CURSOR_OP_QUERY &&
      op != DT_CURSOR_OP_OFF)
  {
    if (*x != priv->mousex ||			/* not moving to same position? 	*/
        *y != priv->mousey)
    {
	  priv->mousex = *x;				/* remember new position for motion */
	  priv->mousey = *y;				/*   comparison 					*/
	  fg_mousemov(*x, *y);				/* move fastgraph's mouse position 	*/
    }
  }
  
  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/

static
dt_rc_e
dtfastgraph_method(
  dt_ctl_t *ctl,
  const char *driver,
  dtvga_chipset_e chipset,
  int *method)
{
  DT_F("dtcanvas_fastgraph_method");

  switch(chipset)						/* map to fastgraph's number        */
  {
    case DTVGA_CHIPSET_AUTO:            *method =  0; break;
    case DTVGA_CHIPSET_ATI:             *method =  4; break;
    case DTVGA_CHIPSET_ATI_18800:       *method =  4; break;
    case DTVGA_CHIPSET_ATI_18800_1:     *method =  5; break;
    case DTVGA_CHIPSET_ATI_28800:       *method =  6; break;
    case DTVGA_CHIPSET_AHEAD:           *method =  2; break;
    case DTVGA_CHIPSET_AHEAD_A:         *method =  2; break;
    case DTVGA_CHIPSET_AHEAD_B:         *method =  3; break;
    case DTVGA_CHIPSET_CT:              *method =  7; break;
    case DTVGA_CHIPSET_CT_451:          *method =  7; break;
    case DTVGA_CHIPSET_CT_452:          *method =  8; break;
    case DTVGA_CHIPSET_CT_453:          *method =  9; break;
    case DTVGA_CHIPSET_CIRRUS: 		    *method = 20; break;
    case DTVGA_CHIPSET_CIRRUS_5400:     *method = 20; break;
    case DTVGA_CHIPSET_GENOA:           *method = 10; break;
    case DTVGA_CHIPSET_OAK:             *method = 11; break;
    case DTVGA_CHIPSET_OAK_OTI067:      *method = 11; break;
    case DTVGA_CHIPSET_PARADISE:        *method = 12; break;
    case DTVGA_CHIPSET_WD:              *method = 12; break;
    case DTVGA_CHIPSET_WD_C00:          *method = 13; break;
    case DTVGA_CHIPSET_WD_C01:          *method = 14; break;
    case DTVGA_CHIPSET_TRIDENT:         *method = 15; break;
    case DTVGA_CHIPSET_TRIDENT_8800: 	*method = 15; break;
    case DTVGA_CHIPSET_TRIDENT_8900: 	*method = 16; break;
    case DTVGA_CHIPSET_TRIDENT_9000: 	*method = 22; break;
    case DTVGA_CHIPSET_TSENG_ET3000: 	*method = 17; break;
    case DTVGA_CHIPSET_TSENG_ET4000: 	*method = 18; break;
    case DTVGA_CHIPSET_VESA:         	*method =  1; break;
    case DTVGA_CHIPSET_VIDEO7:       	*method = 19; break;
    case DTVGA_CHIPSET_S3:           	*method = 21; break;
    case DTVGA_CHIPSET_CIRRUS_6400:    	*method = 26; break;
    case DTVGA_CHIPSET_NCR_77CX2:      	*method = 22; break;
    case DTVGA_CHIPSET_OAK_OTI077:     	*method = 23; break;
    case DTVGA_CHIPSET_OAK_OTI087:     	*method = 24; break;
    case DTVGA_CHIPSET_VGA:           	*method = -1; break;
    case DTVGA_CHIPSET_VGAX:           	*method = -2; break;
    default:
      return dt_err(ctl, F,
	    "unsupported chipset \"%s\" (dtvga_chipset_e %d)",
	    driver, chipset);
  }
  return DT_RC_GOOD;
}


/*..........................................................................
 *..........................................................................*/

static
dt_rc_e
dtfastgraph_mode(
  dt_ctl_t *ctl,
  const char *reso,
  int xe,
  int ye,
  int *mode)
{
  DT_F("dtfastgraph_mode");
  if (xe == 320 && ye == 200)			/* map resolution to mode			*/
    *mode = 19;
  else
  if (xe == 320 && ye == 240)
    *mode = 22;
  else
  if (xe == 640 && ye == 400)
    *mode = 24;
  else
  if (xe == 640 && ye == 480)
    *mode = 25;
  else
  if (xe == 800 && ye == 600)
    *mode = 26;
  else
  if (xe == 1024 && ye == 768)
    *mode = 27;
  else
    return dt_err(ctl, F,
      "unsupported resolution \"%s\" (%d by %d)",
      reso, xe, ye);
  return DT_RC_GOOD;
}

#endif

/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtcanvas_fastgraph_init(
  dt_ctl_t *ctl,
  dtcanvas_t *canvas,
  char *driver)
{
  DT_F("dtcanvas_fastgraph_init");
# if DTPRJ_FASTGRAPH == DT_1 && !defined(DTCONFIG_IMPLIED_MSW)
  char *reso;
  dtvga_chipset_e chipset;
  int method, mode, detect;
  int have_keyboard = 0;
  int xe;
  int ye;
  dtcanvas_fastgraph_priv_t *priv = NULL;
  dt_rc_e rc;

  DTCSTACK_HIGHWATER;

  if (!dtprj_fastgraph)					/* called but not implemented?	    */
    return dt_err_notlinked(ctl,
      F, DESC);  

  if (driver == NULL)					/* NULL means default			    */
    driver = "";

  DT_Q(dtos_strdup,(ctl, driver,		/* get space for driver edit */
    &driver));

  reso = strchr(driver, ',');			/* reso part after driver		    */
  if (reso)
    *reso++ = '\0';
  else
    reso = "";

  DT_C(dtvga_lookup_chipset,(ctl,		/* get number for chipset			*/
    driver, &chipset, NULL));
  
  DT_C(dtfastgraph_method,(ctl,			/* get fastgraph method number 		*/
    driver, chipset, &method));

  DT_C(dtvga_lookup_resolution,(ctl,    /* get resolution pair				*/
    reso, &xe, &ye, NULL));

  DT_C(dtfastgraph_mode,(ctl,			/* get fastgraph mode number 		*/
    reso, xe, ye, &mode));

# ifdef FG32
    fg_initpm();						/* first Fastgraph call			    */
# endif

  if ((!*reso ||						/* resolution not specified?	    */
       mode >= 24) &&					/* or we know we want SVGA?	    	*/
      method >= 0)						/* and it's a SVGA method?		    */
  {
    detect = fg_svgainit(method);		/* select svga type    				*/
    if (method == 0 && detect == 0)
	{
      rc = dt_err(ctl, F,
        "chipset/VESA autodetect failed");
	  goto cleanup;
	}
    else
    if (method == 1 && detect == 0)
	{
      rc = dt_err(ctl, F,
        "VESA/chipset autodetect failed");
	  goto cleanup;
	}
  }
  else									/* not going to use SVGA		    */
    detect = -1;

  if (!*reso)							/* resolution not specified?	    */
  {
    if (chipset == DTVGA_CHIPSET_VGA)	/* chipset implies resolution?	    */
    {
      xe = 320;
      ye = 200;
      mode = 19;
    }
    else
    if (chipset == DTVGA_CHIPSET_VGAX)	/* chipset implies resolution?	    */
    {
      xe = 320;
      ye = 240;
      mode = 22;
    }
    else								/* must search for mode				*/
	{
      static int modes[] =				/* search sequence				  	*/
        {27, 26, 25, 24, 22, 19, 0};
      static int xmaxes[] =				/* matching resolutions			    */
	    {1024, 800, 640, 640, 320, 320};
      static int ymaxes[] = 
	    { 768, 600, 480, 400, 240, 200};
      int i;
      for (i=0; modes[i] != 0; i++)
      {
        if (fg_testmode(modes[i], 1))
          break;
        dt_dbg(ctl, NULL, DT_DBG_MASK_WIN,
          "chipset %d, detect %d, mode %d not available",
          chipset, detect, modes[i]);
      }
      if (modes[i] == 0)
	  {
        rc = dt_err(ctl, F,
          "chipset %d, detect %d, no mode available",
          chipset, detect);
		goto cleanup;
	  }
      xe = xmaxes[i];
      ye = ymaxes[i];
      mode = modes[i];
    }
  }

  DT_C(dtos_malloc2,(ctl,				/* space for private structure	    */
    (void **)&priv,
    sizeof(dtcanvas_fastgraph_priv_t),
    F, "priv"));  

  DT_C(dtkeybd_init,(ctl,				/* make us a keyboard				*/
    &priv->keyboard, 
    "dos", 0));
  have_keyboard = 1;
  
  if (DT_DBG(ctl, DT_DBG_MASK_WIN))
  {
    dt_key_t key;
    dt_dbg(ctl, NULL, DT_DBG_MASK_WIN,
      "Fastgraph chipset %d mode %d (hit any key)",
      detect, mode);
    DT_C(dtkeybd_read,(ctl,				/* wait for key						*/
      &priv->keyboard, &key)); 
  }

# ifdef FASTGRAPH3						/* compiling for fastgraph3?		*/

  priv->bufsize = 8192;					/* we need a private buffer 		*/

  DT_C(dtos_malloc2,(ctl, 
    (void **)&priv->buf,
    priv->bufsize,
    F, "priv buf"));  

# else									/* not compiling for fastgraph3? 		*/

    fg_vbinit();						/* init virtual buffers 			*/

# endif

  priv->oldmode = fg_getmode();
  priv->mode = mode;

  canvas->xe       = xe;				/* remember canvas dimensions	    */
  canvas->ye       = ye;
  canvas->priv     = priv;
  canvas->init     = dtcanvas_fastgraph_init;
  canvas->uninit   = dtcanvas_fastgraph_uninit;
  canvas->open     = dtcanvas_fastgraph_open;
  canvas->input    = dtcanvas_fastgraph_input;
  canvas->close    = dtcanvas_fastgraph_close;
  canvas->putblock = dtcanvas_fastgraph_putblock;
  canvas->getblock = dtcanvas_fastgraph_getblock;
  canvas->putcolor = dtcanvas_fastgraph_putcolor;
  canvas->getcolor = dtcanvas_fastgraph_getcolor;
  canvas->cursor   = dtcanvas_fastgraph_cursor;
  canvas->clear    = dtcanvas_fastgraph_clear;
  
cleanup:
  if (rc != DT_RC_GOOD)
  {
	if (priv != NULL)
	{
	  if (have_keyboard)
	    DT_I(dtkeybd_uninit,(ctl,		/* get rid of our keyboard			*/
          &priv->keyboard));
#     ifdef FASTGRAPH3					/* compiling for fastgraph3?		*/
	  if (priv->buf != NULL)
	    DT_I(dtos_free2,(ctl, priv->buf,
          F, "priv buf"));
#     endif
	  DT_I(dtos_free2,(ctl, priv,
        F, "priv"));
	}
  }
  
  if (driver != NULL)
    DT_I(dtos_free2,(ctl, driver,
      F, "driver"));

  return rc;
# else
  return dt_err_notlinked(ctl, F, DESC);  
# endif
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
