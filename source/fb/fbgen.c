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
DT_RCSID("source/fb $RCSfile: fbgen.c,v $ $Revision: 1.7 $");
#include <dtack/lut8.h>
#include <dtack/color.h>
#include <dtack/fb.h>
#include <dtack/os.h>
#include <dtack/str.h>
#include <dtack/mem.h>

extern dtfb_init_f dtfb_gen_init;

static dtfb_uninit_f dtfb_gen_uninit;
static dtfb_control_f dtfb_gen_control;
static dtfb_putblock_f dtfb_gen_putblock;
static dtfb_getblock_f dtfb_gen_getblock;
static dtfb_putcolor_f dtfb_gen_putcolor;
static dtfb_getcolor_f dtfb_gen_getcolor;
static dtfb_clear_f dtfb_gen_clear;
static dtfb_cursor_f dtfb_gen_cursor;

typedef char cwdir_pixel_t;

typedef struct {
  dtimage_t image1;
  dtimage_t image3;
  double radius;
  double background;
  double foreground;
  long count;
  char *spec;
  char *pattern;
  int current;
  int cx, cy;
  int dx, dy;
} priv_t;


/*..........................................................................
 *..........................................................................*/

static
dt_rc_e
dtfb_gen_control(
  dt_ctl_t *ctl,
  dtfb_t *fb,
  dtfb_control_e control)
{
  DT_F("dtfb_gen_control");
  priv_t *priv =
    (priv_t *)fb->priv;

  DTCSTACK_HIGHWATER;

  switch (control)
  {
	case DTFB_CONTROL_RESET:
	break;
	case DTFB_CONTROL_SNAPSHOT:
      priv->current++;
	break;
	case DTFB_CONTROL_FREEZE:
	break;
	case DTFB_CONTROL_ACQUIRE:
      priv->current++;
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
dtfb_gen_putblock(
  dt_ctl_t *ctl,
  dtfb_t *fb,
  void *data,
  int x0,
  int y0,
  int xe,
  int ye)
{
  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/

static
dt_rc_e
dtfb_gen_getblock_sweep(
  dt_ctl_t *ctl,
  dtfb_t *fb,
  void *data,
  int x0,
  int y0,
  int xe,
  int ye)
{
  DT_F("dtfb_gen_getblock_sweep");
  priv_t *priv = (priv_t *)fb->priv;
  dtos_time_t now;
  double second, degrees, radians;

  DT_Q(dtimage_plot_line,(ctl,			/* erase old sweep 					*/
    &priv->image1,
    priv->cx, priv->cy,
    priv->dx, priv->dy,
    priv->background))

  DT_Q(dtos_time,(ctl, &now));

  second = (int)now.wall % 60;
  degrees = second * 6.0;
  radians = DT_PI * degrees / 180.0;

  priv->dx = (int)(priv->cx +
    priv->radius * sin(radians));
  priv->dy = (int)(priv->cy +
    priv->radius * cos(radians));
  
  DT_Q(dtimage_plot_line,(ctl,			/* draw new sweep					*/
    &priv->image1,
    priv->cx, priv->cy,
    priv->dx, priv->dy,
    priv->foreground))

  DT_Q(dtimage_resample,(ctl,
    &priv->image1, 1.0/3.0, 1.0,
    "nearest", &priv->image3));

  DT_Q(dtmem_getpatch,(ctl,				/* suck data of of image */
    priv->image3.memalloc,
    data, x0, y0, xe*3, ye));
  
  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/

static
dt_rc_e
dtfb_gen_getblock_count(
  dt_ctl_t *ctl,
  dtfb_t *fb,
  void *data,
  int x0,
  int y0,
  int xe,
  int ye)
{
  DT_F("dtfb_gen_getblock_count");
  priv_t *priv = (priv_t *)fb->priv;
  dtimage_t _inset, *inset = &_inset;
  char s[10];

  DT_Q(dtimage_constant,(ctl, &priv->image1, 128.0));

  DT_Q(dtstr_printf,(ctl, s, sizeof(s), "%ld", priv->count));
  priv->count++;

  priv->dx = DT_MAX(priv->dx, (int)strlen(s)*8);
  priv->dx = DT_MIN(priv->dx, (int)fb->xe);
  priv->dy = DT_MAX(priv->dy, 16);
  priv->dy = DT_MIN(priv->dy, fb->ye);

  priv->cx = DT_MAX(priv->cx, 0);  
  priv->cy = DT_MAX(priv->cy, 0);  
  if (priv->cx+priv->dx > priv->image1.xe)
    priv->cx = priv->image1.xe-priv->dx;
  if (priv->cy+priv->dy > priv->image1.ye)
    priv->cy = priv->image1.ye-priv->dy;
  DT_Q(dtimage_overlay,(ctl, &priv->image1, inset, 
    priv->cx, priv->cy, priv->dx, priv->dy));
  DT_Q(dtimage_constant,(ctl, inset, 0.0));

  DT_Q(dtimage_plot_text_8x16,(ctl, &priv->image1, s,
    priv->cx+(priv->dx-8*strlen(s))/2, 
    priv->cy+(priv->dy-16)/2, 
    255.0));

  DT_Q(dtimage_resample,(ctl,
    &priv->image1, 1.0/3.0, 1.0,
    "nearest", &priv->image3));

  DT_Q(dtmem_getpatch,(ctl,				/* suck data of of image */
    priv->image3.memalloc,
    data, x0, y0, xe*2, ye));

  DT_Q(dtimage_free,(ctl, inset));  
  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/

static
dt_rc_e
dtfb_gen_getblock(
  dt_ctl_t *ctl,
  dtfb_t *fb,
  void *data,
  int x0,
  int y0,
  int xe,
  int ye)
{
  DT_F("dtfb_gen_getblock");
  priv_t *priv = (priv_t *)fb->priv;

  if (!strcmp(priv->pattern, "sweep"))
    DT_Q(dtfb_gen_getblock_sweep,(ctl, fb, data, x0, y0, xe, ye))
  else
    DT_Q(dtfb_gen_getblock_count,(ctl, fb, data, x0, y0, xe, ye));
  
  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/

static
dt_rc_e
dtfb_gen_putcolor(
  dt_ctl_t *ctl,
  dtfb_t *fb,
  dtlut8_t *lut,
  int start,
  int count)
{
  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/

static
dt_rc_e
dtfb_gen_getcolor(
  dt_ctl_t *ctl,
  dtfb_t *fb,
  dtlut8_t *lut,
  int start,
  int count)
{
  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/

static
dt_rc_e
dtfb_gen_clear(
  dt_ctl_t *ctl,
  dtfb_t *fb,
  int background)
{
  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/

static
dt_rc_e
dtfb_gen_cursor(
  dt_ctl_t *ctl,
  dtfb_t *fb,
  int *x,
  int *y,
  int op)
{
  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/

static
dt_rc_e
dtfb_source_change(
  dt_ctl_t *ctl,
  priv_t *priv,
  const char *source)
{
  DT_F("dtfb_source_change");
  if (!strcmp(source, "<x"))
    priv->cx -= 5;
  if (!strcmp(source, ">x"))
    priv->cx += 5;
  if (!strcmp(source, "<y"))
    priv->cy -= 5;
  if (!strcmp(source, ">y"))
    priv->cy += 5;
  if (!strcmp(source, "<z"))
  {
	priv->dx -= 5; 
	priv->dy -= 5;
  }
  if (!strcmp(source, ">z"))
  {
    priv->dx += 5;
	priv->dy += 5;
  }
  dt_dbg(ctl, F, DT_DBG_MASK_OBJECT,
	"change \"%s\" pos=%d,%d size=%d,%d",
    source, 
    priv->cx, priv->cy,
    priv->dx, priv->dy);
  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/

static
dt_rc_e
dtfb_source_vidmux(						/* switch vidmux					*/
  dt_ctl_t *ctl,
  const char *source,
  int *port,
  int *value)
{
  DT_F("dtfb_source_vidmux");
  char *s = NULL;
  int n;
  long v;
  dt_rc_e rc;

  if (source == NULL)
    goto cleanup;

  dt_dbg(ctl, F, DT_DBG_MASK_ANY,
    "vidmux at 0x%03x switch to %d",
    port, value);

  DT_C(dtos_strdup,(ctl, source, &s)); 
  DT_C(dtstr_tokenize,(ctl, s, ',',
    &n));

  if (n != 2)
  {
	rc = dt_err(ctl, F,
      "\"%s\" is not valid for vidmux",
      source);
	goto cleanup;
  }
		
  DT_C(dtstr_to_long,(ctl, s, 
    &v));
  *port = (int)v;
  DT_C(dtstr_to_long,(ctl, s+strlen(s)+1,
    &v));
  *value = (int)v;

cleanup:
  if ( s != NULL)
    DT_I(dtos_free2,(ctl, s,
      F, "source string"));
  return rc;
}

/*..........................................................................
 *..........................................................................*/

static
dt_rc_e
dtfb_gen_source(						/* change signal source				*/
  dt_ctl_t *ctl,
  dtfb_t *fb,
  const char *source)
{
  DT_F("dtfb_gen_source");
  priv_t *priv = (priv_t *)fb->priv;
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
	if (DT_ABBREV(p, "vidmux,"))
	{
	  int port, value;
	  DT_C(dtfb_source_vidmux,(ctl,
        strchr(p, ',')+1, 
        &port, &value));
	}
	else
	if (DT_ABBREV(p, "change,"))
	{
	  DT_C(dtfb_source_change,(ctl, priv,
        strchr(p, ',')+1));
	}
	else
	{
	  rc = dt_err(ctl, F,  
        "\"%s\" is not a valid video source", 
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
dtfb_gen_uninit(
  dt_ctl_t *ctl,
  dtfb_t *fb)
{
  DT_F("dtfb_gen_uninit");
  priv_t *priv = (priv_t *)fb->priv;
  dt_rc_e rc = DT_RC_GOOD;

  DTCSTACK_HIGHWATER;

  if (priv != NULL)
  {
	if (priv->image3.xe != 0)
	  DT_I(dtimage_free,(ctl,
        &priv->image3));
	if (priv->image1.xe != 0)
	  DT_I(dtimage_free,(ctl,
        &priv->image1));

	if (priv->spec != NULL)
      DT_I(dtos_free2,(ctl, 
        priv->spec,
        F, "spec"));

    DT_I(dtos_free2,(ctl,				/* free private space 				*/
      fb->priv,
      F, "priv"));
  }

  return rc;
}

/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtfb_gen_init(
  dt_ctl_t *ctl,
  dtfb_t *fb,
  const char *spec)
{
  DT_F("dtfb_gen_init");
  priv_t *priv = NULL;
  int ntokens;
  char *p;
  long longval;
  dt_rc_e rc;

  DTCSTACK_HIGHWATER;

  if (spec == NULL)
  {
    rc = dt_err(ctl, F,
      "NULL dir spec");
    goto cleanup;
  }
  else
  if (strlen(spec) == 0)
  {
    rc = dt_err(ctl, F,
      "zero-length dir spec");
    goto cleanup;
  }

  DT_C(dtos_malloc2,(ctl,				/* space for private structure	    */
    (void **)&priv, sizeof(priv_t),
    F, "priv"));
  DT_MEMSET(priv, 0,						/* clear private structure 			*/
    sizeof(priv_t));

  DT_C(dtos_strdup,(ctl, spec,			/* get space for spec edit 			*/
    &priv->spec));

  DT_C(dtstr_tokenize,(ctl,				/* separate spec into tokens 		*/
    priv->spec,
    ',', &ntokens));
										/* ................................ */
  p = priv->spec;

  if (ntokens >= 1)
  {
    priv->pattern = p;
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
  }
  else
    fb->ye = 288;

  if (ntokens >= 4)						/* flags+flags						*/
  {
  }

  priv->background = 0;
  priv->foreground = 255;
  priv->radius = 0.3 * 
    DT_MIN(fb->xe, fb->ye);
  priv->dx = 64;
  priv->dy = 64;
  priv->cx = (int)((fb->xe-priv->dx) / 2.0);
  priv->cy = (int)((fb->ye-priv->dy) / 2.0);

  DT_C(dtimage_create,(ctl,				/* create image 					*/
    &priv->image1, fb->xe, fb->ye,
    DT_NTYPE_B08));
  DT_C(dtimage_create,(ctl,				/* create image 					*/
    &priv->image3, fb->xe*3, fb->ye,
    DT_NTYPE_B08));
  DT_C(dtimage_constant,(ctl,			/* clear image 						*/
    &priv->image3, priv->background));

  fb->org.org = DT_COLOR_ORG_RGB888;
  fb->xe *= 3;
  fb->rowsize = fb->xe;	
  
  fb->priv     = priv;
  fb->uninit   = dtfb_gen_uninit;
  fb->control  = dtfb_gen_control;
  fb->putblock = dtfb_gen_putblock;
  fb->getblock = dtfb_gen_getblock;
  fb->putcolor = dtfb_gen_putcolor;
  fb->getcolor = dtfb_gen_getcolor;
  fb->cursor   = dtfb_gen_cursor;
  fb->clear    = dtfb_gen_clear;
  fb->source   = dtfb_gen_source;
  
cleanup:
  if (rc != DT_RC_GOOD)
    DT_I(dtfb_gen_uninit,(ctl, fb));

  return rc;
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
