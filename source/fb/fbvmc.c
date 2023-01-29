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
DT_RCSID("source/fb $RCSfile: fbvmc.c,v $ $Revision: 1.7 $");
#include <dtack/lut8.h>
#include <dtack/color.h>
#include <dtack/fb.h>
#include <dtack/os.h>
#include <dtack/str.h>
#include <dtack/prj.h>
#include <dtack/phys.h>

#define DESC "vmc2 capture card"

extern dtfb_init_f dtfb_vmc_init;

#if DTPRJ_VMC == DT_1
const int dtfb_vmc_dummy = 0;

static dtfb_uninit_f dtfb_vmc_uninit;
static dtfb_control_f dtfb_vmc_control;
static dtfb_putblock_f dtfb_vmc_putblock;
static dtfb_getblock_f dtfb_vmc_getblock;
static dtfb_putcolor_f dtfb_vmc_putcolor;
static dtfb_getcolor_f dtfb_vmc_getcolor;
static dtfb_clear_f dtfb_vmc_clear;
static dtfb_cursor_f dtfb_vmc_cursor;

typedef struct {
  char *ioaddr_s;
  char *memaddr_s;
  char *mode_s;
  long pagesize;
  long memaddr;
  dtphys_t phys;
  int ioaddr;
  int ab_sel;
  int lo_res;
  int ncols;
  int nrows;
} dtfb_vmc_priv_t;

#ifdef DTCONFIG_IMPLIED_QNX432
#include <sys/inline.h>
#else
void outb(int port, int val);
int inb(int port);
#endif

static struct {
  unsigned char control0;
  unsigned char control1;
  unsigned char status;
} shadow;

/*--------------------------------------------------------------------------*
 *--------------------------------------------------------------------------*/
#define F "vmc_out1"

void myoutb(int port, unsigned char val);
#pragma aux myoutb = "out  dx,al"\
        parm [edx] [eax] modify exact nomemory [];

static
dt_rc_e 
vmc_out(								/* output to vmc ioaddr			*/
  dt_ctl_t *ctl,			 
  dtfb_vmc_priv_t *vmc,
  int reg,
  unsigned char val)
{
  if (vmc->ioaddr)
  {
    myoutb(vmc->ioaddr, reg);
    myoutb(vmc->ioaddr+1, val);
    if (DT_DBG(ctl, DT_DBG_MASK_PIXEL))
      dt_dbg(ctl, F, DT_DBG_MASK_PIXEL,
        "out %02x 0x%02x (%d)",
        reg, val, val);
  }
  return DT_RC_GOOD;
}
#undef F

/*--------------------------------------------------------------------------*
 *--------------------------------------------------------------------------*/
#define F "vmc_out2"

static
dt_rc_e 
vmc_out2(								/* output to vmc ioaddr			*/
  dt_ctl_t *ctl,			 
  dtfb_vmc_priv_t *vmc,
  int reg,
  unsigned int val)
{
  if (vmc->ioaddr)
  {
    myoutb(vmc->ioaddr, reg);
    myoutb(vmc->ioaddr+1, val & 0xff);
    myoutb(vmc->ioaddr, reg+1);
    myoutb(vmc->ioaddr+1, (val >> 8) & 0xff);
    if (DT_DBG(ctl, DT_DBG_MASK_PIXEL))
      dt_dbg(ctl, F, DT_DBG_MASK_PIXEL,
        "out %02x 0x%04x (%d)",
        reg, val, val);
  }
  return DT_RC_GOOD;
}
#undef F

/*--------------------------------------------------------------------------*
 *--------------------------------------------------------------------------*/
#define F "vmc_out3"

static
dt_rc_e 
vmc_out3(								/* output to vmc ioaddr			*/
  dt_ctl_t *ctl,			 
  dtfb_vmc_priv_t *vmc,
  int reg,
  unsigned long val)
{
  if (vmc->ioaddr)
  {
    myoutb(vmc->ioaddr, reg);
    myoutb(vmc->ioaddr+1, val & 0xff);
    myoutb(vmc->ioaddr, reg+1);
    myoutb(vmc->ioaddr+1, (val >> 8) & 0xff);
    myoutb(vmc->ioaddr, reg+3);
    myoutb(vmc->ioaddr+1, (val >> 16) & 0xff);
    if (DT_DBG(ctl, DT_DBG_MASK_PIXEL))
      dt_dbg(ctl, F, DT_DBG_MASK_PIXEL,
        "out %02x 0x%08x (%d)",
        reg, val, val);
  }
  return DT_RC_GOOD;
}
#undef F

/*--------------------------------------------------------------------------*
 *--------------------------------------------------------------------------*/
#define F "vmc_in"

static
dt_rc_e 
vmc_in(							/* input from vmc port			*/
  dt_ctl_t *ctl,			
  dtfb_vmc_priv_t *vmc,
  int reg,
  unsigned char *val)
{
  if (vmc->ioaddr)
  {
    myoutb(vmc->ioaddr, reg);
    *val = inb(vmc->ioaddr+1);
  }
  else
    *val = 0;
  return DT_RC_GOOD;
}
#undef F


/*--------------------------------------------------------------------------*
 *--------------------------------------------------------------------------*/
#define F "vmc_open"

static
dt_rc_e 
vmc_open(							/* init data structure				*/
  dt_ctl_t *ctl,			  
  dtfb_vmc_priv_t *vmc)
{
  unsigned char val;

#ifdef NONO
  DT_Q(vmc_out,(ctl, vmc, 0xff, 0x01));	/* global enable registers 			*/
  
  DT_Q(vmc_out,(ctl, vmc, 0xff, 0x02));	/* global enable frame buffer		*/
  DT_Q(vmc_in,(ctl, vmc, 0x00, &val));	/* check signature 					*/
  
  if (val != (vmc->ioaddr & 0xff))
      return dt_err(ctl, F, 
        "no vmc found at ioaddr 0x%04x"
        " (verify=0x%02x)",
        vmc->ioaddr, val);
#endif

  return DT_RC_GOOD;
}
#undef F

/*--------------------------------------------------------------------------*
 * wait for start of vertical blank after odd field
 *--------------------------------------------------------------------------*/
#define F "vmc_waitvb"

#define FIELD 0x08
#define BLANK 0x04

static
dt_rc_e 
vmc_waitvb(								/* wait for vertical blank			*/
  dt_ctl_t *ctl,				
  dtfb_vmc_priv_t *vmc,
  unsigned char *pval)
{
  unsigned char val;
  if (vmc->ioaddr)
  {
	again:
	do									/* wait out current vb				*/
	{
      DT_Q(vmc_in,(ctl, vmc, 
        0x09, &val));
	  printf("%02x ", val);
	}
	while (val & BLANK);

	printf("\n");

    if (DT_DBG(ctl, DT_DBG_MASK_PIXEL))
    dt_dbg(ctl, F, DT_DBG_MASK_PIXEL,
      "end of vb 0x%02x (%d)"
      " %s, %s vb",
      val, val,
      val & FIELD? "odd": "even",
      val & BLANK? "in": "not in");
										
	do									/* wait for start of vb				*/
	  DT_Q(vmc_in,(ctl, vmc, 
        0x09, &val))
	while (!(val & BLANK));

    if (DT_DBG(ctl, DT_DBG_MASK_PIXEL))
    dt_dbg(ctl, F, DT_DBG_MASK_PIXEL,
      "beg of vb 0x%02x (%3d)"
      " %s, %s vb",
      val, val,
      val & FIELD? "odd": "even",
      val & BLANK? "in": "not in");

	if (val & FIELD)					/* odd field is coming next?		*/
	  goto again;
  }

  if (pval != NULL)						/* caller wants final status? 		*/
    *pval = val;

  return DT_RC_GOOD;
}
#undef F

/*--------------------------------------------------------------------------*
 * freeze at end of odd field
 * makes sure something is aquired if requested
 *--------------------------------------------------------------------------*/
#define F "vmc_freeze"

static
dt_rc_e 
vmc_freeze(								/* freeze and view memory		  	*/
  dt_ctl_t *ctl,				
  dtfb_vmc_priv_t *vmc)
{
  unsigned char val;

  DT_Q(dtos_delay,(ctl, 1000));

#ifdef NONO
  do									/* wait until acquiring starts		*/
  {
    DT_Q(vmc_in,(ctl, vmc, 
      0x20, &val))
	printf("%02x ", val);
  }
  while (!(val & 0x01));
#endif

  printf("\n");

  DT_Q(vmc_waitvb,(ctl,					/* wait for end of odd field		*/
    vmc, &val));

  DT_Q(vmc_out,(ctl,					/* quit acquiring					*/
    vmc, 0x20, 0x20)); 

  return DT_RC_GOOD;
}

#undef F

/*--------------------------------------------------------------------------*
 *--------------------------------------------------------------------------*/
#define F "vmc_acquire"

static
dt_rc_e 
vmc_acquire(							/* start acquiring					*/
  dt_ctl_t *ctl,				
  dtfb_vmc_priv_t *vmc)
{
  unsigned char val;

  DT_Q(vmc_out,(ctl,					/* start acquiring				    */
    vmc, 0x20, 0x21));

  return DT_RC_GOOD;
}

#undef F

/*--------------------------------------------------------------------------*
 *--------------------------------------------------------------------------*/
#define F "vmc_snapshot"

static
dt_rc_e 
vmc_snapshot(							/* freeze one new frame				*/
  dt_ctl_t *ctl,				  
  dtfb_vmc_priv_t *vmc)
{
  DT_Q(vmc_acquire,(ctl, vmc));			/* start acquiring 					*/

  DT_Q(vmc_freeze,(ctl, vmc));			/* quit acquiring					*/

  return DT_RC_GOOD;
}

#undef F

/*--------------------------------------------------------------------------*
 *--------------------------------------------------------------------------*/
#define F "vmc_reset"

static
dt_rc_e 
vmc_reset(						/* reset hardware to initial state		*/
  dt_ctl_t *ctl,				
  dtfb_vmc_priv_t *vmc,
  int xe,
  int ye)
{
  DT_Q(vmc_out,(ctl, vmc, 0xff, 0x01));	/* global enable registers 			*/
  
  DT_Q(vmc_out,(ctl, vmc, 0xff, 0x03));	/* enable host frame buffer access	*/

  DT_Q(vmc_out,(ctl, vmc, 0x01, 0x00));	/* disable vram write mask			*/
#ifdef NONO
#endif

  DT_Q(vmc_out,(ctl, vmc, 0x82, 0x80));	/* enable page mode					*/
  DT_Q(vmc_out,(ctl, vmc, 0x06, 0x10));	/* map vram to isa megabyte 0		*/
  DT_Q(vmc_out,(ctl, vmc, 0x81,			/* offset into megabyte 0			*/
    0xa0 | (vmc->memaddr >> 15) & 0x1f));

#ifdef NONO
  DT_Q(vmc_out,(ctl, vmc, 0x07, 0xff));	/* enable all bitplane capture		*/
  DT_Q(vmc_out,(ctl, vmc, 0x08, 0xff));	/* enable all bitplane capture		*/
  
  DT_Q(vmc_out,(ctl, vmc, 0x09, 0x00));	/* disable interrupts				*/
  
  DT_Q(vmc_out,(ctl, vmc, 0x20, 0x20));	/* freeze							*/
  
  DT_Q(vmc_out,(ctl, vmc, 0x21, 0x90));	/* rgb, field inverted				*/

  DT_Q(vmc_out2,(ctl, vmc, 0x22, 16));	/* x start							*/

  DT_Q(vmc_out2,(ctl, vmc, 0x24, 0));	/* y start							*/

  DT_Q(vmc_out2,(ctl, vmc, 0x26, xe));	/* x end							*/

  DT_Q(vmc_out2,(ctl, vmc, 0x28, ye));	/* y end							*/

  DT_Q(vmc_out3,(ctl, vmc, 0x2a, 0));	/* vram capture address				*/

  DT_Q(vmc_out,(ctl, vmc, 0x2d, 64));	/* normal x scaling, capture all 	*/
  DT_Q(vmc_out,(ctl, vmc, 0x2e, 64));	/* normal y scaling, capture all 	*/
  
  DT_Q(vmc_out,(ctl, vmc, 0x30, 16));	/* input video start line			*/
  
  DT_Q(vmc_out,(ctl, vmc, 0x38, 0x00));	/* scaling control					*/
#endif
  return DT_RC_GOOD;
}

#undef F

/*--------------------------------------------------------------------------*
 *--------------------------------------------------------------------------*/
#define F "vmc_report1"

static
dt_rc_e 
vmc_report(								/* print current hardware state		*/
  dt_ctl_t *ctl,				
  dtfb_vmc_priv_t *vmc,
  int reg,
  const char *comment)
{
  unsigned char val;
  DT_Q(vmc_in,(ctl, vmc, reg, &val));
  dt_dbg(ctl, NULL, DT_DBG_MASK_OBJECT,
    "%2x = 0x%02x %s", reg, val, comment);
  return DT_RC_GOOD;
}
#undef F

/*--------------------------------------------------------------------------*
 *--------------------------------------------------------------------------*/
#define F "vmc_report2"

static
dt_rc_e 
vmc_report2(								/* print current hardware state		*/
  dt_ctl_t *ctl,				
  dtfb_vmc_priv_t *vmc,
  int reg,
  const char *comment)
{
  unsigned char val1, val2;
  DT_Q(vmc_in,(ctl, vmc, reg, &val1));
  DT_Q(vmc_in,(ctl, vmc, reg+1, &val2));
  dt_dbg(ctl, NULL, DT_DBG_MASK_OBJECT,
    "%2x = 0x%04lx %s", reg, 
    val2*0x100L + val1, comment);
  return DT_RC_GOOD;
}
#undef F

/*--------------------------------------------------------------------------*
 *--------------------------------------------------------------------------*/
#define F "vmc_report3"

static
dt_rc_e 
vmc_report3(								/* print current hardware state		*/
  dt_ctl_t *ctl,				
  dtfb_vmc_priv_t *vmc,
  int reg,
  const char *comment)
{
  unsigned char val1, val2, val3;
  DT_Q(vmc_in,(ctl, vmc, reg, &val1));
  DT_Q(vmc_in,(ctl, vmc, reg+1, &val2));
  DT_Q(vmc_in,(ctl, vmc, reg+2, &val3));
  dt_dbg(ctl, NULL, DT_DBG_MASK_OBJECT,
    "%2x = 0x%08lx %s", reg, 
    val3*0x1000L + val2*0x100L + val1,
    comment);
  return DT_RC_GOOD;
}
#undef F

/*--------------------------------------------------------------------------*
 *--------------------------------------------------------------------------*/
#define F "vmc_report"

static
dt_rc_e 
vmc_report_all(								/* print current hardware state		*/
  dt_ctl_t *ctl,				
  dtfb_vmc_priv_t *vmc)
{
  DT_Q(vmc_report,(ctl, vmc, 0xff, "global enable register"));

  DT_Q(vmc_report,(ctl, vmc, 0x00, "io address signature"));
  DT_Q(vmc_report,(ctl, vmc, 0x01, "disable vram write mask"));
  
  DT_Q(vmc_report,(ctl, vmc, 0x82, "enable page mode"));
  DT_Q(vmc_report,(ctl, vmc, 0x06, "map vram to isa megabyte"));
  DT_Q(vmc_report,(ctl, vmc, 0x81, "memory page base address"));

  DT_Q(vmc_report,(ctl, vmc, 0x07, "enable all bitplane capture"));
  DT_Q(vmc_report,(ctl, vmc, 0x08, "enable all bitplane capture"));
  
  DT_Q(vmc_report,(ctl, vmc, 0x09, "disable interrupts"));
  
  DT_Q(vmc_report,(ctl, vmc, 0x20, "freeze"));
  
  DT_Q(vmc_report,(ctl, vmc, 0x21, "rgb"));

  DT_Q(vmc_report2,(ctl, vmc, 0x22, "x start"));

  DT_Q(vmc_report2,(ctl, vmc, 0x24, "y start"));

  DT_Q(vmc_report2,(ctl, vmc, 0x26, "x end"));

  DT_Q(vmc_report2,(ctl, vmc, 0x28, "y end"));

  DT_Q(vmc_report3,(ctl, vmc, 0x2a, "vram capture address"));

  DT_Q(vmc_report,(ctl, vmc, 0x2d, "normal x scaling, capture all"));
  DT_Q(vmc_report,(ctl, vmc, 0x2e, "normal y scaling, capture all"));
  
  DT_Q(vmc_report,(ctl, vmc, 0x30, "input video start line"));
  
  DT_Q(vmc_report,(ctl, vmc, 0x38, "scaling control"));

  return DT_RC_GOOD;
}

#undef F

/*..........................................................................
 *..........................................................................*/

static
dt_rc_e
dtfb_vmc_control(
  dt_ctl_t *ctl,
  dtfb_t *fb,
  dtfb_control_e control)
{
  DT_F("dtfb_vmc_control");
  dtfb_vmc_priv_t *priv =
    fb->priv;

  DTCSTACK_HIGHWATER;

  switch (control)
  {
	case DTFB_CONTROL_RESET:
      DT_Q(vmc_reset,(ctl, priv, fb->xe, fb->ye));
      DT_Q(vmc_report_all,(ctl, priv));
	break;
	case DTFB_CONTROL_SNAPSHOT:
      DT_Q(vmc_snapshot,(ctl, priv));
	break;
	case DTFB_CONTROL_FREEZE:
      DT_Q(vmc_freeze,(ctl, priv));
	break;
	case DTFB_CONTROL_ACQUIRE:
      DT_Q(vmc_acquire,(ctl, priv));
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
dtfb_vmc_putblock(
  dt_ctl_t *ctl,
  dtfb_t *fb,
  void *data,
  int x0,
  int y0,
  int xe,
  int ye)
{
  DT_F("dtfb_vmc_putblock");
  dtfb_vmc_priv_t *priv =
    fb->priv;
  int w = xe;
  int x, y;
  char *p;
  int last;

  DTCSTACK_HIGHWATER;

  DT_Q(dtfb_clip,(ctl,					/* do clipping					    */
    fb, data, x0, y0, xe, ye,
    &data, &x0, &y0, &w, &ye));

  if (xe < 0 || ye < 0)					/* picture entirely clipped out?    */
    return DT_RC_GOOD;

  last = -1;
  for (y=y0; y<y0+ye; y++)				/* for each row in input data 		*/
  {
	int page = y / 128;
    if (page != last)
	{
      DT_Q(vmc_out,(ctl,			/* select page						*/
        priv, 0, page));
	  p = (char *)priv->phys.virtaddr + 
        (int)(y%128) * fb->xe + x0;		/* start of the output area			*/
	  last = page;
      printf("%s: page %d, v 0x%08lx, d 0x%08lx, %3d, %3d, %3d, (w=%3d), %3d\n",
        F, page, (unsigned long)p, (unsigned long)data, x0, y0, xe, w, ye);
	}
    for (x=0; x<w; x++)					/* copy data to frame buffer 		*/
      p[x] = ((char *)data)[x];
	data = (char *)data + xe;
    p += fb->xe;
  }

  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/

static
dt_rc_e
dtfb_vmc_getblock(
  dt_ctl_t *ctl,
  dtfb_t *fb,
  void *data,
  int x0,
  int y0,
  int xe,
  int ye)
{
  DT_F("dtfb_vmc_getblock");
  dtfb_vmc_priv_t *priv =
    fb->priv;
  int w = xe;
  int y;
  char *p;
  char *d;
  int last;

  DTCSTACK_HIGHWATER;

  DT_Q(vmc_freeze,(ctl,					/* freeze next even field 			*/
    priv));

  DT_Q(dtfb_clip,(ctl,					/* do clipping					    */
    fb, data, x0, y0, xe, ye,
    &data, &x0, &y0, &w, &ye));

  if (xe < 0 || ye < 0)					/* picture entirely clipped out?    */
    return DT_RC_GOOD;

  d = (char *)data;

  if (!strcmp(priv->mode_s, "lo") &&	/* in low resolution mode? 			*/
	  xe == w && xe == fb->xe)			/* and reading full rows? 			*/
  {
	unsigned int n = xe * (ye-y0);		/* bytes to suck out 				*/
	p = (char *)priv->phys.virtaddr +	/* start of the input area			*/
      y0 * fb->xe;	
	DT_MEMCPY(d, p, n);
  }
#ifdef NONO
  else
  if (!strcmp(priv->mode_s, "lo"))		/* partial rows in low resolution?	*/
  {
	int i;
	DT_Q(vmc_out,(ctl,				/* select lowres page 0				*/
      priv, 0, 
      shadow.control0 & ~0x3));
	p = (char *)priv->phys.virtaddr +	/* start of the input area			*/
      y0 * fb->xe;	
    for (y=0; y<ye; y++)				/* for each row in input data 		*/
    {
      for (i=0; i<w; i++)				/* copy data from frame buffer 		*/
        *d++ = p[i];
      p += fb->xe;
	}
  }
  else
  if (xe == w && xe == fb->xe)			/* and reading full rows? 			*/
  {
	int dy;
    last = -1;
    for (y=y0; y<y0+ye; y++)			/* for each row in input data 		*/
    {
      int i;
  	  int page = y / 128;
      if (page != last)
	  {
     	DT_Q(vmc_out,(ctl, priv, 0,		/* select lowres page 0				*/
          (shadow.control0 & ~0x3) | 
          page));
	    p = (char *)priv->phys.virtaddr + 
          (int)(y%128) * fb->xe + x0;	/* start of the input area			*/
	    last = page;
	  }
	  dy = DT_MIN(ye,					/* how many rows can we do? 		*/
        (page+1) * 128) - y;
	  DT_MEMCPY(d, p, fb->xe * dy);		/* copy as much as possible		    */
	  d += fb->xe * dy;
      p += fb->xe * dy;
	}
  }
  else									/* partial rows or skipping rows    */
  {
    last = -1;
    for (y=y0; y<y0+ye; y++)			/* for each row in input data 		*/
    {
      int i;
  	  int page = y / 128;
      if (page != last)
	  {
     	DT_Q(vmc_out,(ctl,			/* select lowres page 0				*/
          priv, 0, 
          (shadow.control0 & ~0x3) | 
          page));
	    p = (char *)priv->phys.virtaddr + 
          (int)(y%128) * fb->xe + x0;	/* start of the input area			*/
	    last = page;
	  }
      for (i=0; i<w; i++)				/* copy data from frame buffer 		*/
        *d++ = p[i];
      p += fb->xe;
	}
  }
#endif

  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/

static
dt_rc_e
dtfb_vmc_putcolor(
  dt_ctl_t *ctl,
  dtfb_t *fb,
  dtlut8_t *lut,
  int start,
  int count)
{
  DT_F("dtfb_vmc_putcolor");
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
dtfb_vmc_getcolor(
  dt_ctl_t *ctl,
  dtfb_t *fb,
  dtlut8_t *lut,
  int start,
  int count)
{
  DT_F("dtfb_vmc_getcolor");

  DTCSTACK_HIGHWATER;

  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/

static
dt_rc_e
dtfb_vmc_clear(
  dt_ctl_t *ctl,
  dtfb_t *fb,
  int background)
{
  DT_F("dtfb_vmc_clear");
  char b[512];
  int y;
  
  DTCSTACK_HIGHWATER;

  DT_MEMSET(b, background, fb->xe);		/* fill line buffer with background */
  
  for (y=0; y<fb->ye; y++)				/* propagate to all rows 			*/
	DT_Q(dtfb_vmc_putblock,(ctl,
      fb, b, 0, y, fb->xe, 1));

  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/

static
dt_rc_e
dtfb_vmc_cursor(
  dt_ctl_t *ctl,
  dtfb_t *fb,
  int *x,
  int *y,
  int op)
{
  DT_F("dtfb_vmc_cursor");
  dtfb_vmc_priv_t *priv =
    fb->priv;

  DTCSTACK_HIGHWATER;
  
  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/

static
dt_rc_e
dtfb_vmc_uninit(
  dt_ctl_t *ctl,
  dtfb_t *fb)
{
  DT_F("dtfb_vmc_uninit");
  dtfb_vmc_priv_t *priv =
    fb->priv;
  dt_rc_e rc = DT_RC_GOOD;

  DTCSTACK_HIGHWATER;

  if (priv != NULL)
  {
    if (priv->phys.physaddr != 0)		/* we accessed physical memory? 	*/
      DT_I(dtphys_close,(ctl,			/* release it 						*/
        &priv->phys));

	if (priv->ioaddr_s != NULL)
      DT_I(dtos_free2,(ctl, 
        priv->ioaddr_s,
        F, "spec"));

    DT_I(dtos_free2,(ctl,				/* free private space 				*/
      fb->priv,
      F, "priv"));
  }

  return rc;
}

#endif

/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtfb_vmc_init(
  dt_ctl_t *ctl,
  dtfb_t *fb,
  const char *spec)
{
  DT_F("dtfb_vmc_init");
# if DTPRJ_VMC == DT_1
  char *ioaddr_s;
  char *memaddr_s;
  char *mode_s;
  long ioaddr;
  long memaddr;
  char *p;
  dtfb_vmc_priv_t *priv = NULL;
  dt_rc_e rc;

  DTCSTACK_HIGHWATER;

  if (!dtprj_vmc)					/* called but not implemented?	    */
    return dt_err_notlinked(ctl,
      F, DESC);  

  if (spec == NULL)
  {
    rc = dt_err(ctl, F,
      "NULL vmc spec");
    goto cleanup;
  }
  else
  if (strlen(spec) == 0)
  {
    rc = dt_err(ctl, F,
      "zero-length vmc spec");
    goto cleanup;
  }

  DT_Q(dtos_strdup,(ctl, spec,			/* get space for spec edit 			*/
    &ioaddr_s));

  memaddr_s = strchr(ioaddr_s, ',');	/* where the memaddr string starts 	*/
  if (memaddr_s)
    *memaddr_s++ = '\0';
  else
    memaddr_s = "";

  mode_s = strchr(memaddr_s, ',');		/* where the mode string starts 	*/
  if (mode_s)
    *mode_s++ = '\0';
  else
    mode_s = "";

  DT_C(dtstr_to_long,(ctl,
    ioaddr_s, &ioaddr));

  DT_C(dtstr_to_long,(ctl,
    memaddr_s, &memaddr));

  DT_C(dtos_malloc2,(ctl,				/* space for private structure	    */
    (void **)&priv,
    sizeof(dtfb_vmc_priv_t),
    F, "priv"));
  DT_MEMSET(priv, 0,					/* clear private structure 			*/
    sizeof(dtfb_vmc_priv_t));

  priv->ioaddr_s = ioaddr_s;
  priv->memaddr_s = memaddr_s;
  priv->mode_s = mode_s;
  priv->ioaddr = (int)ioaddr;
  priv->memaddr = memaddr;
  if (!strcmp(mode_s, "lo"))			/* low resolution mode? 			*/
  {
	priv->ncols = 256;
	priv->nrows = 240;
  }
  else
  if (!strcmp(mode_s, "hi"))			/* high resolution mode? 			*/
  {
	priv->ncols = 512;
	priv->nrows = 480;
  }
  else									/* unknown resolution mode? 		*/
  {
	rc = dt_err(ctl, F,
      "invalid mode \"%s\"",
	  mode_s);
	goto cleanup;
  }
  priv->pagesize = 0x10000;

  DT_C(dtphys_open,(ctl,				/* access physical memory 			*/
    &priv->phys,
    memaddr, priv->pagesize));

  DT_C(vmc_open,(ctl, priv));

  fb->xe       = priv->ncols;			/* remember fb dimensions	    	*/
  fb->ye       = priv->nrows;
  fb->rowsize  = priv->ncols;
  fb->org.org  = DT_COLOR_ORG_LUMINANCE;

  fb->priv     = priv;
  fb->uninit   = dtfb_vmc_uninit;
  fb->control  = dtfb_vmc_control;
  fb->putblock = dtfb_vmc_putblock;
  fb->getblock = dtfb_vmc_getblock;
  fb->putcolor = dtfb_vmc_putcolor;
  fb->getcolor = dtfb_vmc_getcolor;
  fb->cursor   = dtfb_vmc_cursor;
  fb->clear    = dtfb_vmc_clear;
  
cleanup:
  if (rc != DT_RC_GOOD)
  {
	if (priv != NULL &&
		priv->phys.virtaddr != NULL)	/* we accessed physical memory? 	*/
	  DT_I(dtphys_close,(ctl,			/* release it 						*/
        &priv->phys));

	if (priv != NULL)
	  DT_I(dtos_free2,(ctl, priv,
        F, "priv"));
  
    if (ioaddr_s != NULL)
      DT_I(dtos_free2,(ctl, ioaddr_s,
        F, "spec"));
  }

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
