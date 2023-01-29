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

extern dtfb_init_f dtfb_dir_init;

static dtfb_uninit_f dtfb_dir_uninit;
static dtfb_control_f dtfb_dir_control;
static dtfb_putblock_f dtfb_dir_putblock;
static dtfb_getblock_f dtfb_dir_getblock;
static dtfb_putcolor_f dtfb_dir_putcolor;
static dtfb_getcolor_f dtfb_dir_getcolor;
static dtfb_clear_f dtfb_dir_clear;
static dtfb_cursor_f dtfb_dir_cursor;

typedef char cwdir_pixel_t;

typedef struct {
  const char *pathpat;
  int current;
} dtfb_dir_priv_t;


/*..........................................................................
 *..........................................................................*/

static
dt_rc_e
dtfb_dir_control(
  dt_ctl_t *ctl,
  dtfb_t *fb,
  dtfb_control_e control)
{
  DT_F("dtfb_dir_control");
  dtfb_dir_priv_t *priv =
    (dtfb_dir_priv_t *)fb->priv;

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
dtfb_dir_putblock(
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
dtfb_dir_getblock(
  dt_ctl_t *ctl,
  dtfb_t *fb,
  void *data,
  int x0,
  int y0,
  int xe,
  int ye)
{
  DT_F("dtfb_dir_getblock");
  dtfb_dir_priv_t *priv =
    (dtfb_dir_priv_t *)fb->priv;
  int w = xe;
  dt_rc_e rc;

  DT_Q(dtfb_clip,(ctl,					/* do clipping					    */
    fb, data, x0, y0, xe, ye,
    &data, &x0, &y0, &w, &ye));

  if (xe < 0 || ye < 0)					/* picture entirely clipped out?    */
    return DT_RC_GOOD;

  if (xe == w && xe == fb->xe)			/* and reading full rows? 			*/
  {
	unsigned int n =					/* bytes to suck out 				*/
      fb->rowsize * (ye-y0);
	char fn[256];
	dtfd_t fd;
	dtstr_printf(ctl, fn, sizeof(fn),
      priv->pathpat, priv->current);
    DT_Q(dtfd_open,(ctl, &fd, 
      fn, "r"));
    DT_G(dtfd_read_all,(ctl, &fd,
      data, n));
    DT_I(dtfd_close,(ctl, &fd));
  }
  else
  {
	rc = dt_err(ctl, F,
      "mode not supported");
  }

  return rc;
  
}

/*..........................................................................
 *..........................................................................*/

static
dt_rc_e
dtfb_dir_putcolor(
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
dtfb_dir_getcolor(
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
dtfb_dir_clear(
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
dtfb_dir_cursor(
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
dtfb_dir_source(						/* change signal source				*/
  dt_ctl_t *ctl,
  dtfb_t *fb,
  const char *source)
{
  DT_F("dtfb_dir_source");
  dt_dbg(ctl, F, DT_DBG_MASK_ANY,
    "switching source to \"%s\"",
    source);
  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/

static
dt_rc_e
dtfb_dir_uninit(
  dt_ctl_t *ctl,
  dtfb_t *fb)
{
  DT_F("dtfb_dir_uninit");
  dtfb_dir_priv_t *priv =
    (dtfb_dir_priv_t *)fb->priv;
  dt_rc_e rc = DT_RC_GOOD;

  DTCSTACK_HIGHWATER;

  if (priv != NULL)
  {
	if (priv->pathpat != NULL)
      DT_I(dtos_free2,(ctl, 
        (char *)priv->pathpat,
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
dtfb_dir_init(
  dt_ctl_t *ctl,
  dtfb_t *fb,
  const char *spec)
{
  DT_F("dtfb_dir_init");
  dtfb_dir_priv_t *priv = NULL;
  char *pathpat = NULL;
  char *width_s, *height_s, *start_s;
  long width, height, start;
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

  DT_Q(dtos_strdup,(ctl, spec,			/* get space for spec edit 			*/
    &pathpat));

  width_s = strchr(pathpat, ',');
  if (width_s != NULL)
	*width_s++ = '\0';
  else
    width_s = "260";

  height_s = strchr(width_s, ',');
  if (height_s != NULL)
	*height_s++ = '\0';
  else
    height_s = "260";

  start_s = strchr(height_s, ',');
  if (start_s != NULL)
	*start_s++ = '\0';
  else
    start_s = "0";

  DT_C(dtstr_to_long,(ctl, width_s,
    &width));
  DT_C(dtstr_to_long,(ctl, height_s,
    &height));
  DT_C(dtstr_to_long,(ctl, start_s,
    &start));
  DT_C(dtos_malloc2,(ctl,				/* space for private structure	    */
    (void **)&priv,
    sizeof(dtfb_dir_priv_t),
    F, "priv"));
  DT_MEMSET(priv, 0,						/* clear private structure 			*/
    sizeof(dtfb_dir_priv_t));

  priv->pathpat = pathpat;
  priv->current = start-1;

  fb->xe       = width;
  fb->ye       = height;
  fb->rowsize  = (fb->xe * 2);
  fb->org.org  = DT_COLOR_ORG_YCBYCR;
  
  fb->priv     = priv;
  fb->uninit   = dtfb_dir_uninit;
  fb->control  = dtfb_dir_control;
  fb->putblock = dtfb_dir_putblock;
  fb->getblock = dtfb_dir_getblock;
  fb->putcolor = dtfb_dir_putcolor;
  fb->getcolor = dtfb_dir_getcolor;
  fb->cursor   = dtfb_dir_cursor;
  fb->clear    = dtfb_dir_clear;
  fb->source   = dtfb_dir_source;
  
cleanup:
  if (rc != DT_RC_GOOD)
  {
	if (priv != NULL)
	  DT_I(dtos_free2,(ctl, priv,
        F, "priv"));
  
    if (pathpat != NULL)
      DT_I(dtos_free2,(ctl, pathpat,
        F, "spec"));
  }

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
