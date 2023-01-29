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

#define DESC "SVGACC library"

extern dtcanvas_init_f dtcanvas_svgacc_init;

#if DTPRJ_SVGACC == DT_1
const int dtcanvas_svgacc_dummy = 0;

static dtcanvas_uninit_f dtcanvas_svgacc_uninit;
static dtcanvas_open_f dtcanvas_svgacc_open;
static dtcanvas_input_f dtcanvas_svgacc_input;
static dtcanvas_close_f dtcanvas_svgacc_close;
static dtcanvas_putblock_f dtcanvas_svgacc_putblock;
static dtcanvas_getblock_f dtcanvas_svgacc_getblock;
static dtcanvas_putcolor_f dtcanvas_svgacc_putcolor;
static dtcanvas_getcolor_f dtcanvas_svgacc_getcolor;
static dtcanvas_clear_f dtcanvas_svgacc_clear;
static dtcanvas_cursor_f dtcanvas_svgacc_cursor;

#  include <dtack/nofar.h>
#  include <dtack/svgacc.h>

typedef enum {
  MOUSE_OFF = 0,
  MOUSE_ON
} mouse_e;

typedef struct {
  int mode;
  int oldmode;
  RasterBlock *rasterblock;
  unsigned int bufsize;
  dtkeybd_t keyboard;
  mouse_e mouse;
  int mousex, mousey;
  int click_state;
  dtos_time_t click_time;
  dtos_time_t unclick_time;
} dtcanvas_svgacc_priv_t;

/*..........................................................................
 *..........................................................................*/

static
dt_rc_e
dtcanvas_svgacc_uninit(
  dt_ctl_t *ctl,
  dtcanvas_t *canvas)
{
  DT_F("dtcanvas_svgacc_uninit");
  dtcanvas_svgacc_priv_t *priv =
    canvas->priv;
  dt_rc_e rc = DT_RC_GOOD;

  DTCSTACK_HIGHWATER;

  DT_I(dtkeybd_uninit,(ctl,				/* get rid of our keyboard			*/
    &priv->keyboard));

  DT_I(dtos_free2,(ctl, 
    priv->rasterblock,
    F, "priv buffer"));  
  DT_I(dtos_free2,(ctl, 
    canvas->priv,
    F, "priv"));  

  return rc;
}

/*..........................................................................
 *..........................................................................*/

static
dt_rc_e
dtcanvas_svgacc_open(
  dt_ctl_t *ctl,
  dtcanvas_t *canvas)
{
  DT_F("dtcanvas_svgacc_open");
  dtcanvas_svgacc_priv_t *priv =
    canvas->priv;
  int resrc = 0;

  DTCSTACK_HIGHWATER;

  switch(priv->mode)
  {
	case 0: resrc = res320(); break;
	case 1: resrc = res640l(); break;
	case 2: resrc = res640(); break;
	case 3: resrc = res800(); break;
	case 4: resrc = res1024(); break;
	case 5: resrc = res1280(); break;
  }

  if (resrc == 0)
    return dt_err(ctl, F,
      "unable to set resolution at mode %d",
      priv->mode);

  {
	int nbuts = whichmouse();			/* see if we have a mouse 			*/
	if (nbuts == 0)						/* no mouse or mouse driver? 		*/
	  priv->mouse = MOUSE_OFF;
	else
	{
      mouseenter();						/* enable the mouse driver 			*/
	  priv->mouse = MOUSE_ON;
	}
	
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
  mousestatus(x, y, &buts);
  *buttons = 0;
  if (buts & 0x01) *buttons |= 0x01;
  if (buts & 0x02) *buttons |= 0x04;	/* middle button is third bit 		*/
  if (buts & 0x04) *buttons |= 0x02;	/* right button is second bit 		*/
  return DT_RC_GOOD;
}
/*..........................................................................
 *..........................................................................*/

static
dt_rc_e
dtcanvas_svgacc_input(					/* stop and wait for input		    */
  dt_ctl_t *ctl,
  dtcanvas_t *canvas,
  dtsvc_t *svc,
  double timeout,
  int flag,
  dt_event_t *event)
{
  DT_F("dtcanvas_svgacc_input");
  dtcanvas_svgacc_priv_t *priv =
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
dtcanvas_svgacc_close(
  dt_ctl_t *ctl,
  dtcanvas_t *canvas)
{
  DT_F("dtcanvas_svgacc_close");
  dtcanvas_svgacc_priv_t *priv =
    canvas->priv;

  DTCSTACK_HIGHWATER;

  videomodeset(priv->oldmode);		/* put us in the text mode			*/
  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/

static
dt_rc_e
dtcanvas_svgacc_putblock(
  dt_ctl_t *ctl,
  dtcanvas_t *canvas,
  void *data,
  int x0,
  int y0,
  int xe,
  int ye)
{
  DT_F("dtcanvas_svgacc_putblock");
  dtcanvas_svgacc_priv_t *priv =
    canvas->priv;
  int w = xe;
  int y;
  int nrows;
  unsigned long rb =
    (unsigned long)priv->rasterblock;
  RasterBlock *rasterblock =			/* rasterblock has to be 16-aligned	*/
    (RasterBlock *)
      (rb + 16L - (rb & 15L));

  char *p;

  DTCSTACK_HIGHWATER;

  DT_Q(dtcanvas_clip,(ctl,				/* do clipping					    */
    canvas, data, x0, y0, xe, ye,
    &data, &x0, &y0, &w, &ye));

  if (xe < 0 || ye < 0)					/* picture entirely clipped out?    */
    return DT_RC_GOOD;

  nrows = (int)(priv->bufsize / w);		/* rows that fit in our buffer		*/

  rasterblock->width = w - 1;
  rasterblock->height = nrows - 1;
      
  for (y=0; y<ye; y+=nrows)				/* for each buf full				*/
  {
    int i;
	if (y+nrows > ye)					/* partial buffer at end?			*/
	{
	  nrows = ye - y;
      rasterblock->height = nrows - 1;
	}
    p = &rasterblock->data[0];
    for (i=0; i<nrows; i++)
	{
	  DT_MEMCPY(p, data, w);
	  p += w;
	  data = (char *)data + xe;
	}
	blkput(SET, x0, y0+y,				/* display buf full					*/
      rasterblock);
  }

  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/

static
dt_rc_e
dtcanvas_svgacc_getblock(
  dt_ctl_t *ctl,
  dtcanvas_t *canvas,
  void *data,
  int x0,
  int y0,
  int xe,
  int ye)
{
  DT_F("dtcanvas_svgacc_getblock");
  dtcanvas_svgacc_priv_t *priv =
    canvas->priv;
  int w = xe;
  int y;
  int nrows;
  unsigned long rb =
    (unsigned long)priv->rasterblock;
  RasterBlock *rasterblock =			/* rasterblock has to be 16-aligned	*/
    (RasterBlock *)
      (rb + 16L - (rb & 15L));
  char *p;
  int ret;

  DTCSTACK_HIGHWATER;

  DT_Q(dtcanvas_clip,(ctl,				/* do clipping					    */
    canvas, data, x0, y0, xe, ye,
    &data, &x0, &y0, &w, &ye));

  if (xe < 0 || ye < 0)					/* picture entirely clipped out?    */
    return DT_RC_GOOD;

  nrows = (int)(priv->bufsize / w);		/* rows that fit in our buffer		*/

  rasterblock->width = w - 1;
  rasterblock->height = nrows - 1;
      
  for (y=0; y<ye; y+=nrows)				/* for each buf full				*/
  {
    int i;
	if (y+nrows > ye)					/* partial buffer at end?			*/
	{
	  nrows = ye - y;
      rasterblock->height = nrows - 1;
	}
	ret = blkget(x0, y0+y,				/* get buf full						*/
      x0+w-1, y0+y+nrows-1,
      rasterblock);
    if (ret == 0)
      return dt_err(ctl, F,
        "blkget(%d, %d, %d, %d, rasterblock) failed",
        x0, y0+y,
        x0+w-1, y0+y+nrows-1);

    p = &rasterblock->data[0];
    for (i=0; i<nrows; i++)
	{
	  DT_MEMCPY(data, p, w);
	  p += w;
	  data = (char *)data + xe;
	}
  }

  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/

static
dt_rc_e
dtcanvas_svgacc_putcolor(
  dt_ctl_t *ctl,
  dtcanvas_t *canvas,
  dtlut8_t *lut,
  int start,
  int count)
{
  DT_F("dtcanvas_svgacc_putcolor");
  RGB *rgb = NULL;
  int i;
  dt_rc_e rc;

  DTCSTACK_HIGHWATER;

  DT_C(dtos_malloc2,(ctl,				/* get temporary space for palette 	*/
    (void **)&rgb, 3*count,
    F, "palette"));
  for (i=0; i<count; i++)				/* convert lut to palette 			*/
  {
	rgb[i].r = lut[i].r >> 2;
	rgb[i].g = lut[i].g >> 2;
	rgb[i].b = lut[i].b >> 2;
  }

  palset(rgb, start, start+count-1);	/* set VGA palette 					*/

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
dtcanvas_svgacc_getcolor(
  dt_ctl_t *ctl,
  dtcanvas_t *canvas,
  dtlut8_t *lut,
  int start,
  int count)
{
  DT_F("dtcanvas_svgacc_getcolor");
  DTCSTACK_HIGHWATER;
  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/

static
dt_rc_e
dtcanvas_svgacc_clear(
  dt_ctl_t *ctl,
  dtcanvas_t *canvas,
  int background)
{
  DT_F("dtcanvas_svgacc_clear");
  DTCSTACK_HIGHWATER;
  fillscreen(background);
  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/

static
dt_rc_e
dtcanvas_svgacc_cursor(
  dt_ctl_t *ctl,
  dtcanvas_t *canvas,
  int *x,
  int *y,
  int op)
{
  DT_F("dtcanvas_svgacc_cursor");
  dtcanvas_svgacc_priv_t *priv =
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
	  mouselocset(*x, *y);				/* move svgacc's mouse position 	*/
    }
  }
  
  return DT_RC_GOOD;
}

#endif


/*..........................................................................
| can,svgacc
| can,svgacc,ati
| can,svgacc,ati,800
| can,svgacc,ati,800.512
| can,svgacc,,800
| can,svgacc,,.512
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtcanvas_svgacc_init(
  dt_ctl_t *ctl,
  dtcanvas_t *canvas,
  char *driver)
{
  DT_F("dtcanvas_svgacc_init");
# if DTPRJ_SVGACC == DT_1
  char *reso;
  char *memory;
  dtvga_chipset_e chipset;
  VGAChipset method;
  int mode;
  int cpu;
  int xe;
  int ye;
  dtcanvas_svgacc_priv_t *priv;
  VGAChipset detect;
  dt_rc_e rc = DT_RC_GOOD;

  DTCSTACK_HIGHWATER;

  if (!dtprj_svgacc)					/* called but not implemented?	    */
    return dt_err_notlinked(ctl,
      F, DESC);  

  cpu = whichcpu();						/* check cpu					    */
  if (cpu < 386)
    return dt_err(ctl, F,
      "unsupported cpu %d", cpu);

  if (driver == NULL)					/* NULL means default			    */
    driver = "";

  DT_Q(dtos_strdup,(ctl, driver,
    &driver));

  memory = strchr(driver, '.');			/* find the memory spec			    */
  if (memory != NULL)
    *memory++ = '\0';					/* separate chipset and mem		  	*/
  else
    memory = "";

  reso = strchr(driver, ',');			/* reso part after driver		    */
  if (reso)
    *reso++ = '\0';
  else
    reso = "";

  DT_GI(dtvga_lookup_chipset,(ctl,		/* get number for chipset			*/
    driver, &chipset, NULL));

  DT_GI(dtvga_lookup_resolution,(ctl,	/* get resolution pair				*/
    reso, &xe, &ye, NULL));

  if (rc == DT_RC_GOOD)
  switch(chipset)						/* map to svgacc's number        	*/
  {
    case DTVGA_CHIPSET_AUTO: 		method = UNKNOWN; break;
    case DTVGA_CHIPSET_ACUMOS: 		method = ACUMOS; break;
    case DTVGA_CHIPSET_ATI: 		method = ATI; break;
    case DTVGA_CHIPSET_AHEAD: 		method = AHEADA; break;
    case DTVGA_CHIPSET_AHEAD_A: 	method = AHEADA; break;
    case DTVGA_CHIPSET_AHEAD_B: 	method = AHEADB; break;
    case DTVGA_CHIPSET_CT: 			method = CHIPSTECH; break;
    case DTVGA_CHIPSET_CIRRUS:		method = CIRRUS; break;
    case DTVGA_CHIPSET_CIRRUS_5400:	method = CIRRUS; break;
    case DTVGA_CHIPSET_CIRRUS_6400:	method = CIRRUS; break;
    case DTVGA_CHIPSET_EVEREX: 		method = EVEREX; break;
    case DTVGA_CHIPSET_GENOA: 		method = GENOA; break;
    case DTVGA_CHIPSET_NCR: 		method = NCR; break;
    case DTVGA_CHIPSET_NCR_77CX2:	method = NCR; break;
    case DTVGA_CHIPSET_OAK:			method = OAKTECH; break;
    case DTVGA_CHIPSET_OAK_OTI067:	method = OAKTECH; break;
    case DTVGA_CHIPSET_OAK_OTI077:	method = OAKTECH; break;
    case DTVGA_CHIPSET_OAK_OTI087:	method = OAKTECH; break;
    case DTVGA_CHIPSET_PARADISE: 	method = PARADISE; break;
    case DTVGA_CHIPSET_REALTEK: 	method = REALTEK; break;
    case DTVGA_CHIPSET_TRIDENT: 	method = TRIDENT; break;
    case DTVGA_CHIPSET_TSENG_ET3000:method = TSENG3; break;
    case DTVGA_CHIPSET_TSENG_ET4000:method = TSENG4; break;
    case DTVGA_CHIPSET_VESA: 		method = VESA; break;
    case DTVGA_CHIPSET_VIDEO7: 		method = VIDEO7; break;
    case DTVGA_CHIPSET_AVANCE: 		method = AVANCE; break;
    case DTVGA_CHIPSET_MXIC: 		method = MXIC; break;
    case DTVGA_CHIPSET_PRIMUS: 		method = PRIMUS; break;
    default:
      rc = dt_err(ctl, F,
	    "unsupported chipset \"%s\" (%d)",
	    driver, chipset);
  }

  if (rc == DT_RC_GOOD)
  if (xe == 320 && ye == 200)			/* map resolution to mode			*/
    mode = 0;
  else
  if (xe == 640 && ye == 400)
    mode = 1;
  else
  if (xe == 640 && ye == 480)
    mode = 2;
  else
  if (xe == 800 && ye == 600)
    mode = 3;
  else
  if (xe == 1024 && ye == 768)
    mode = 4;
  else
  if (xe == 1280 && ye == 1024)
    mode = 5;
  else
    rc = dt_err(ctl, F,
      "unsupported resolution \"%s\" (%d by %d)",
      reso, xe, ye);

  if (rc == DT_RC_GOOD)
  if (method == UNKNOWN)				/* caller wants to autodetect?	    */
  {
    detect = whichvga();				/* select svga type				    */
    if (detect == UNKNOWN)
      return dt_err(ctl, F,
        "chipset autodetect");
  }
  else									/* caller is specifying card?	    */
  {
    int mem;
    if (!strcmp(memory, ""))			/* supposed to guess the memory?	*/
	{
      if (xe == 320)					/* guess based on resolution 		*/
	    mem = 256;
	  else
      if (xe == 640 && ye == 400)
	    mem = 256;
	  else
      if (xe == 640)
	    mem = 512;
	  else
      if (xe == 800)
	    mem = 512;
	  else
      if (xe == 1024)
	    mem = 1024;
	  else
      if (xe == 1280)
	    mem = 2048;
	}
	else
    if (!strcmp(memory, "256"))			/* some memory type is given? 		*/
      mem = 256;
	else
    if (!strcmp(memory, "512"))
      mem = 512;
	else
    if (!strcmp(memory, "1024"))
      mem = 1024;
	else
    if (!strcmp(memory, "2048"))
      mem = 2048;
	else								/* can't recognize memory type? 	*/
	  return dt_err(ctl, F,
        "need %s,%s.N where N is"
        " 256, 512, 1024 or 2048",
        driver, reso);
    setcard(method, mem);				/* force card type				    */
  }

  DT_Q(dtos_free,(ctl, driver));		/* don't need this string anymore   */
  if (rc != DT_RC_GOOD)
    return rc;

  DT_Q(dtos_malloc2,(ctl,				/* space for private structure		*/
    (void **)&priv,
    sizeof(dtcanvas_svgacc_priv_t),
    F, "priv"));  

  DT_Q(dtkeybd_init,(ctl,				/* make us a keyboard				*/
    &priv->keyboard, 
    "dos", 0));

  if (DT_DBG(ctl, DT_DBG_MASK_WIN))
  {
    dt_key_t key;
    dt_dbg(ctl, F, DT_DBG_MASK_WIN,
      "using SVGACC chipset %d (hit any key)", 
      detect);
    DT_Q(dtkeybd_read,(ctl,				/* wait for key						*/
      &priv->keyboard, &key)); 
  }

  priv->bufsize = 8192;

  DT_Q(dtos_malloc2,(ctl, 
    (void **)&priv->rasterblock,
    priv->bufsize+
    16+									/* rasterblock must be 16-aligned	*/
    sizeof(priv->rasterblock->width)+
    sizeof(priv->rasterblock->height),
    F, "priv rasterblock"));  

  priv->oldmode = videomodeget();
  priv->mode = mode;

  canvas->priv  = priv;
  canvas->xe = xe;						/* remember canvas dimensions	    */
  canvas->ye = ye;
  canvas->init     = dtcanvas_svgacc_init;
  canvas->uninit   = dtcanvas_svgacc_uninit;
  canvas->open     = dtcanvas_svgacc_open;
  canvas->input    = dtcanvas_svgacc_input;
  canvas->close    = dtcanvas_svgacc_close;
  canvas->putblock = dtcanvas_svgacc_putblock;
  canvas->getblock = dtcanvas_svgacc_getblock;
  canvas->putcolor = dtcanvas_svgacc_putcolor;
  canvas->getcolor = dtcanvas_svgacc_getcolor;
  canvas->clear    = dtcanvas_svgacc_clear;
  canvas->cursor   = dtcanvas_svgacc_cursor;
  return DT_RC_GOOD;
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
