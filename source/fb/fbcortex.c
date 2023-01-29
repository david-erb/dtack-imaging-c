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
DT_RCSID("source/fb $RCSfile: fbcortex.c,v $ $Revision: 1.7 $");
#include <dtack/lut8.h>
#include <dtack/color.h>
#include <dtack/fb.h>
#include <dtack/os.h>
#include <dtack/str.h>
#include <dtack/prj.h>
#include <dtack/phys.h>

#define DESC "cortex framebuffer"

extern dtfb_init_f dtfb_cortex_init;

#if DTPRJ_CORTEX == DT_1
const int dtfb_cortex_dummy = 0;

static dtfb_uninit_f dtfb_cortex_uninit;
static dtfb_control_f dtfb_cortex_control;
static dtfb_putblock_f dtfb_cortex_putblock;
static dtfb_getblock_f dtfb_cortex_getblock;
static dtfb_putcolor_f dtfb_cortex_putcolor;
static dtfb_getcolor_f dtfb_cortex_getcolor;
static dtfb_clear_f dtfb_cortex_clear;
static dtfb_cursor_f dtfb_cortex_cursor;

typedef char cwcortex_pixel_t;

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
} dtfb_cortex_priv_t;

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
 * cortex interface
 *--------------------------------------------------------------------------*/

#define RAM_ENABLE	0x20
#define AB_SEL		0x10
#define LUT_LOAD_EN	0x04
#define BLANK		0x08
#define DISPLAY_RAM	0x10

#define ACQUIRE		0x08
#define LO_RES 		0x20

#define FIELD       0x04
#define VERT_BLANK	0x08
#define ACQUIRING_ACK	0x20
#define ADC_OVERFLOW	0x01

/*--------------------------------------------------------------------------*
 *--------------------------------------------------------------------------*/
#define F "cwcortex_out"

void myoutb(int port, unsigned char val);
#pragma aux myoutb = "out  dx,al"\
        parm [edx] [eax] modify exact nomemory [];

static
dt_rc_e 
cwcortex_out(							/* output to cortex ioaddr			*/
  dt_ctl_t *ctl,			 
  dtfb_cortex_priv_t *cortex,
  int reg,
  unsigned char val)
{
  if (reg == 0)							/* remember value in global		    */
    shadow.control0 = val;
  else
  if (reg == 1)
    shadow.control1 = val;

  if (cortex->ioaddr)
  {
    reg += cortex->ioaddr;
    myoutb(reg, val);
    if (DT_DBG(ctl, DT_DBG_MASK_PIXEL))
      dt_dbg(ctl, F, DT_DBG_MASK_PIXEL,
        "out %03x 0x%04x (%3d)",
        reg, val, val);
  }
  return DT_RC_GOOD;
}
#undef F

/*--------------------------------------------------------------------------*
 *--------------------------------------------------------------------------*/
#define F "cwcortex_in"

static
dt_rc_e 
cwcortex_in(							/* input from cortex port			*/
  dt_ctl_t *ctl,			
  dtfb_cortex_priv_t *cortex,
  int reg,
  unsigned char *val)
{
  if (cortex->ioaddr)
    *val = inb(cortex->ioaddr + reg); 
  else
    *val = 0;
  shadow.status = *val;					/* stick it in the global		    */
  return DT_RC_GOOD;
}
#undef F


/*--------------------------------------------------------------------------*
 *--------------------------------------------------------------------------*/
#define F "cwcortex_open"

static
dt_rc_e 
cwcortex_open(							/* init data structure				*/
  dt_ctl_t *ctl,			  
  dtfb_cortex_priv_t *cortex)
{
  unsigned char val;
  int bits;

  DT_MEMSET(&shadow, 0, sizeof(shadow));	/* clear shadow registers 			*/
  
  if (cortex->ioaddr)
  {
    DT_Q(cwcortex_in,(ctl, cortex, 0, &val));
    if (val == 0xff)
      return dt_err(ctl, F, 
        "no cortex found at ioaddr 0x%04x",
        cortex->ioaddr);
  }

  cortex->ab_sel =
    ((cortex->memaddr == 0xe0000L || 
	  cortex->memaddr == 0xb0000L)? 
      AB_SEL: 0);


  cortex->lo_res = 
    (!strcmp(cortex->mode_s, "lo")? 
      LO_RES: 0);

  shadow.control0 = cortex->lo_res;		/* in case reset is not done 		*/
  shadow.control1 = cortex->ab_sel;

  return DT_RC_GOOD;
}
#undef F

/*--------------------------------------------------------------------------*
 * wait for start of vertical blank after odd field
 *--------------------------------------------------------------------------*/
#define F "cwcortex_waitvb"

static
dt_rc_e 
cwcortex_waitvb(						/* wait for vertical blank			*/
  dt_ctl_t *ctl,				
  dtfb_cortex_priv_t *cortex,
  unsigned char *pval)
{
  unsigned char val;
  if (cortex->ioaddr)
  {
	again:
	do									/* wait out current vb				*/
      DT_Q(cwcortex_in,(ctl, cortex, 
        0, &val))
	while (val & VERT_BLANK);

    if (DT_DBG(ctl, DT_DBG_MASK_PIXEL))
    dt_dbg(ctl, F, DT_DBG_MASK_PIXEL,
      "end of vb 0x%02x (%3d)"
      " %s, %s vb, %s",
      val, val,
      val & FIELD? "odd": "even",
      val & VERT_BLANK? "in": "not in",
      val & ACQUIRING_ACK?
        "acquiring": "not acquiring",
      val & ADC_OVERFLOW?
        "overflow": "no overflow");
										
	do									/* wait for start of vb				*/
	  DT_Q(cwcortex_in,(ctl, cortex, 
        0, &val))
	while (!(val & VERT_BLANK));

    if (DT_DBG(ctl, DT_DBG_MASK_PIXEL))
    dt_dbg(ctl, F, DT_DBG_MASK_PIXEL,
      "beg of vb 0x%02x (%3d)"
      " %s, %s vb, %s",
      val, val,
      val & FIELD? "odd": "even",
      val & VERT_BLANK? "in": "not in",
      val & ACQUIRING_ACK?
        "acquiring": "not acquiring",
      val & ADC_OVERFLOW?
        "overflow": "no overflow");

	if (val & FIELD)					/* odd field is coming next?		*/
	  goto again;
  }

  if (pval != NULL)						/* caller wants final status? 		*/
    *pval = val;

  return DT_RC_GOOD;
}
#undef F

/*--------------------------------------------------------------------------*
 *--------------------------------------------------------------------------*/
#define F "cwcortex_lut_linear"

static
dt_rc_e 
cwcortex_lut_linear(					/* load linear lut into hardware	*/
  dt_ctl_t *ctl,					
  dtfb_cortex_priv_t *cortex)
{
  register int i;
  DT_Q(cwcortex_waitvb,(ctl, cortex,  NULL));
  DT_Q(cwcortex_out,(ctl, cortex, 0, 0));
  DT_Q(cwcortex_out,(ctl, cortex, 1, BLANK | LUT_LOAD_EN));
  for (i = 0;i<256;i++)
  {
    DT_Q(cwcortex_out,(ctl, cortex, 2,(char)i));
    DT_Q(cwcortex_out,(ctl, cortex, 3,(char)i));
  }
  return DT_RC_GOOD;
}
#undef F

/*--------------------------------------------------------------------------*
 * freeze at end of odd field
 * makes sure something is aquired if requested
 *--------------------------------------------------------------------------*/
#define F "cwcortex_freeze"

static
dt_rc_e 
cwcortex_freeze(						/* freeze and view memory		  	*/
  dt_ctl_t *ctl,				
  dtfb_cortex_priv_t *cortex)
{
  unsigned char val;

  if (!(shadow.control0 & ACQUIRE))		/* acquiring not requested? 		*/
    return DT_RC_GOOD;

  do									/* wait until acquiring starts		*/
    DT_Q(cwcortex_in,(ctl, cortex, 
      0, &val))
  while (!(val & ACQUIRING_ACK));

  DT_Q(cwcortex_waitvb,(ctl,			/* wait for end of odd field		*/
    cortex, &val));

  DT_Q(cwcortex_out,(ctl,				/* quit acquiring					*/
    cortex, 0, 
    shadow.control0 & ~ACQUIRE));

  return DT_RC_GOOD;
}

#undef F

/*--------------------------------------------------------------------------*
 *--------------------------------------------------------------------------*/
#define F "cwcortex_acquire"

static
dt_rc_e 
cwcortex_acquire(						/* start acquiring					*/
  dt_ctl_t *ctl,				
  dtfb_cortex_priv_t *cortex)
{
  unsigned char val;

  DT_Q(cwcortex_out,(ctl,				/* why???						    */
    cortex, 1, 
    shadow.control1 & 
    ~(cortex->ab_sel | RAM_ENABLE)));

  DT_Q(cwcortex_out,(ctl,				/* start acquiring				    */
    cortex, 0, 
    shadow.control0 | ACQUIRE));

  return DT_RC_GOOD;
}

#undef F

/*--------------------------------------------------------------------------*
 *--------------------------------------------------------------------------*/
#define F "cwcortex_snapshot"

static
dt_rc_e 
cwcortex_snapshot(						/* freeze one new frame				*/
  dt_ctl_t *ctl,				  
  dtfb_cortex_priv_t *cortex)
{
  DT_Q(cwcortex_acquire,(ctl, cortex));	/* start acquiring 					*/

  DT_Q(cwcortex_freeze,(ctl, cortex));	/* quit acquiring					*/

  return DT_RC_GOOD;
}

#undef F

/*--------------------------------------------------------------------------*
 *--------------------------------------------------------------------------*/
#define F "cwcortex_reset"

static
dt_rc_e 
cwcortex_reset(						/* reset hardware to initial state		*/
  dt_ctl_t *ctl,				
  dtfb_cortex_priv_t *cortex)
{
  DT_Q(cwcortex_out,(ctl, cortex, 0, 0));
  DT_Q(cwcortex_out,(ctl, cortex, 1, 0));

  DT_Q(cwcortex_lut_linear,(ctl, cortex));

  DT_Q(cwcortex_out,(ctl, cortex, 0, cortex->lo_res));
  DT_Q(cwcortex_out,(ctl, cortex, 1, cortex->ab_sel));

  return DT_RC_GOOD;
}

#undef F

/*..........................................................................
 *..........................................................................*/

static
dt_rc_e
dtfb_cortex_control(
  dt_ctl_t *ctl,
  dtfb_t *fb,
  dtfb_control_e control)
{
  DT_F("dtfb_cortex_control");
  dtfb_cortex_priv_t *priv =
    fb->priv;

  DTCSTACK_HIGHWATER;

  switch (control)
  {
	case DTFB_CONTROL_RESET:
      DT_Q(cwcortex_reset,(ctl, priv));
	break;
	case DTFB_CONTROL_SNAPSHOT:
      DT_Q(cwcortex_snapshot,(ctl, priv));
	break;
	case DTFB_CONTROL_FREEZE:
      DT_Q(cwcortex_freeze,(ctl, priv));
	break;
	case DTFB_CONTROL_ACQUIRE:
      DT_Q(cwcortex_acquire,(ctl, priv));
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
dtfb_cortex_putblock(
  dt_ctl_t *ctl,
  dtfb_t *fb,
  void *data,
  int x0,
  int y0,
  int xe,
  int ye)
{
  DT_F("dtfb_cortex_putblock");
  dtfb_cortex_priv_t *priv =
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
      DT_Q(cwcortex_out,(ctl,			/* select page						*/
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
dtfb_cortex_getblock(
  dt_ctl_t *ctl,
  dtfb_t *fb,
  void *data,
  int x0,
  int y0,
  int xe,
  int ye)
{
  DT_F("dtfb_cortex_getblock");
  dtfb_cortex_priv_t *priv =
    fb->priv;
  int w = xe;
  int y;
  char *p;
  char *d;
  int last;

  DTCSTACK_HIGHWATER;

  DT_Q(cwcortex_freeze,(ctl,			/* freeze next even field 			*/
    priv));
  DT_Q(cwcortex_out,(ctl, priv, 1,		/* enable memory 					*/
    shadow.control1 |
    priv->ab_sel | RAM_ENABLE));

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
	DT_Q(cwcortex_out,(ctl,				/* select lowres page 0				*/
      priv, 0, 
      shadow.control0 & ~0x3));
	p = (char *)priv->phys.virtaddr +	/* start of the input area			*/
      y0 * fb->xe;	
	DT_MEMCPY(d, p, n);
  }
  else
  if (!strcmp(priv->mode_s, "lo"))		/* partial rows in low resolution?	*/
  {
	int i;
	DT_Q(cwcortex_out,(ctl,				/* select lowres page 0				*/
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
     	DT_Q(cwcortex_out,(ctl,			/* select lowres page 0				*/
          priv, 0, 
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
     	DT_Q(cwcortex_out,(ctl,			/* select lowres page 0				*/
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

  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/

static
dt_rc_e
dtfb_cortex_putcolor(
  dt_ctl_t *ctl,
  dtfb_t *fb,
  dtlut8_t *lut,
  int start,
  int count)
{
  DT_F("dtfb_cortex_putcolor");
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
dtfb_cortex_getcolor(
  dt_ctl_t *ctl,
  dtfb_t *fb,
  dtlut8_t *lut,
  int start,
  int count)
{
  DT_F("dtfb_cortex_getcolor");

  DTCSTACK_HIGHWATER;

  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/

static
dt_rc_e
dtfb_cortex_clear(
  dt_ctl_t *ctl,
  dtfb_t *fb,
  int background)
{
  DT_F("dtfb_cortex_clear");
  char b[512];
  int y;
  
  DTCSTACK_HIGHWATER;

  DT_MEMSET(b, background, fb->xe);		/* fill line buffer with background */
  
  for (y=0; y<fb->ye; y++)				/* propagate to all rows 			*/
	DT_Q(dtfb_cortex_putblock,(ctl,
      fb, b, 0, y, fb->xe, 1));

  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/

static
dt_rc_e
dtfb_cortex_cursor(
  dt_ctl_t *ctl,
  dtfb_t *fb,
  int *x,
  int *y,
  int op)
{
  DT_F("dtfb_cortex_cursor");
  dtfb_cortex_priv_t *priv =
    fb->priv;

  DTCSTACK_HIGHWATER;
  
  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/

static
dt_rc_e
dtfb_cortex_uninit(
  dt_ctl_t *ctl,
  dtfb_t *fb)
{
  DT_F("dtfb_cortex_uninit");
  dtfb_cortex_priv_t *priv =
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
dtfb_cortex_init(
  dt_ctl_t *ctl,
  dtfb_t *fb,
  const char *spec)
{
  DT_F("dtfb_cortex_init");
# if DTPRJ_CORTEX == DT_1
  char *ioaddr_s;
  char *memaddr_s;
  char *mode_s;
  long ioaddr;
  long memaddr;
  char *p;
  dtfb_cortex_priv_t *priv = NULL;
  dt_rc_e rc;

  DTCSTACK_HIGHWATER;

  if (!dtprj_cortex)					/* called but not implemented?	    */
    return dt_err_notlinked(ctl,
      F, DESC);  

  if (spec == NULL)
  {
    rc = dt_err(ctl, F,
      "NULL cortex spec");
    goto cleanup;
  }
  else
  if (strlen(spec) == 0)
  {
    rc = dt_err(ctl, F,
      "zero-length cortex spec");
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
    sizeof(dtfb_cortex_priv_t),
    F, "priv"));
  DT_MEMSET(priv, 0,						/* clear private structure 			*/
    sizeof(dtfb_cortex_priv_t));

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

  DT_C(cwcortex_open,(ctl, priv));

  fb->xe       = priv->ncols;			/* remember fb dimensions	    	*/
  fb->ye       = priv->nrows;
  fb->rowsize  = priv->ncols;
  fb->org.org  = DT_COLOR_ORG_LUMINANCE;

  fb->priv     = priv;
  fb->uninit   = dtfb_cortex_uninit;
  fb->control  = dtfb_cortex_control;
  fb->putblock = dtfb_cortex_putblock;
  fb->getblock = dtfb_cortex_getblock;
  fb->putcolor = dtfb_cortex_putcolor;
  fb->getcolor = dtfb_cortex_getcolor;
  fb->cursor   = dtfb_cortex_cursor;
  fb->clear    = dtfb_cortex_clear;
  
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
