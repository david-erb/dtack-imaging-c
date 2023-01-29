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
DT_RCSID("source/fb $RCSfile: fb.c,v $ $Revision: 1.7 $");
#include <dtack/os.h>
#include <dtack/str.h>
#include <dtack/lut8.h>
#include <dtack/color.h>
#include <dtack/fb.h>

static const char *choices[] = {
  "cortex",
  /*  "vmc", */
  /*  "psi", */
  "vbmci",
  "dir",
  "vfw",
  "gen",
  "meteor",
  "bt",
  NULL
};

extern dtfb_init_f dtfb_cortex_init;

static dtfb_init_f *methods[] = {
  dtfb_cortex_init,
  /*  dtfb_vmc_init, */
  /*  dtfb_psi_init, */
  dtfb_vbmci_init, 
  dtfb_dir_init,
  dtfb_vfw_init,
  dtfb_gen_init,
  dtfb_meteor_init,
  dtfb_bt_init
};

/*..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtfb_init(
  dt_ctl_t *ctl,
  dtfb_t *fb,
  const char *spec)
{
  DT_F("dtfb_init");
  char *type;
  char *info;
  int choice;
  dt_rc_e rc;

  if (spec == NULL)
    spec = "";

  DT_Q(dtos_strdup,(ctl, spec,
    &type));

  info = strchr(type, ',');				/* info part after type		    	*/
  if (info)
    *info++ = '\0';
  else
    info = "";

  DT_C(dtstr_choice,(ctl, choices,		/* look up fb type			    	*/
    type, &choice));

  if (choice == -1)						/* unknown type?				    */
  {
    rc = dt_err(ctl, F,
      "unknown fb spec \"%s\"",
      type);
	goto cleanup;
  }
  else
  if (choice == -2)						/* ambiguous type?				    */
  {
    rc = dt_err(ctl, F,
      "ambiguous fb spec \"%s\"",
      type);
	goto cleanup;
  }

  DT_MEMSET(fb, 0, sizeof(*fb));			/* clear structure				    */

  DT_C((methods[choice]),(ctl,			/* give info to implementation		*/
    fb, info))

  fb->type = type;
  fb->info = info;

  return DT_RC_GOOD;
  
cleanup:
  DT_I(dtos_free2,(ctl, type,
    F, "type"));

  return rc;
}

/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtfb_uninit(
  dt_ctl_t *ctl,
  dtfb_t *fb)
{
  DT_F("dtfb_uninit");
  dt_rc_e rc = DT_RC_GOOD;
  if (fb->uninit)
    DT_Q((fb->uninit),(ctl, 
      fb));
  if (fb->type)
    DT_Q(dtos_free,(ctl,				/* free parse string			    */
      fb->type));
  DT_MEMSET(fb, 0, sizeof(*fb));
  return rc;
}

/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtfb_control(
  dt_ctl_t *ctl,
  dtfb_t *fb,
  dtfb_control_e control)
{
  DT_F("dtfb_control");
  dt_rc_e rc = DT_RC_GOOD;
  if (fb->control)
  DT_Q((fb->control),(ctl, fb, control));
  return rc;
}

/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtfb_putblock(
  dt_ctl_t *ctl,
  dtfb_t *fb,
  void *data,
  int x0,
  int y0,
  int xe,
  int ye)
{
  DT_F("dtfb_putblock");
  dt_rc_e rc = DT_RC_GOOD;
  if (fb->putblock)
  DT_Q((fb->putblock),(ctl, fb, data, x0, y0, xe, ye));
  return rc;
}

/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtfb_getblock(
  dt_ctl_t *ctl,
  dtfb_t *fb,
  void *data,
  int x0,
  int y0,
  int xe,
  int ye)
{
  DT_F("dtfb_getblock");
  dt_rc_e rc = DT_RC_GOOD;
  if (fb->getblock)
  DT_Q((fb->getblock),(ctl, fb, data, x0, y0, xe, ye));
  return rc;
}

/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtfb_source(
  dt_ctl_t *ctl,
  dtfb_t *fb,
  const char *source)
{
  DT_F("dtfb_getblock");
  dt_rc_e rc = DT_RC_GOOD;
  if (fb->source)
  DT_Q((fb->source),(ctl, fb, source));
  return rc;
}

/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtfb_putcolor(
  dt_ctl_t *ctl,
  dtfb_t *fb,
  dtlut8_t *lut,
  int start,
  int count)
{
  DT_F("dtfb_putcolor");
  dt_rc_e rc = DT_RC_GOOD;
  if (fb->putcolor)
  DT_Q((fb->putcolor),(ctl, fb, lut, start, count));
  return rc;
}

/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtfb_getcolor(
  dt_ctl_t *ctl,
  dtfb_t *fb,
  dtlut8_t *lut,
  int start,
  int count)
{
  DT_F("dtfb_getcolor");
  dt_rc_e rc = DT_RC_GOOD;
  if (fb->getcolor)
  DT_Q((fb->getcolor),(ctl, fb, lut, start, count));
  return rc;
}

/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtfb_cursor(
  dt_ctl_t *ctl,
  dtfb_t *fb,
  int *x,
  int *y,
  int op)
{
  DT_F("dtfb_cursor");
  if (fb->cursor)
  DT_Q((fb->cursor),(ctl, fb, x, y, op));
  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtfb_clear(
  dt_ctl_t *ctl,
  dtfb_t *fb,
  int background)
{
  DT_F("dtfb_clear");
  dt_rc_e rc = DT_RC_GOOD;
  if (fb->clear)
  DT_Q((fb->clear),(ctl, fb, background));
  return rc;
}

/*..........................................................................
 * returns overlap rectangle 
 * destination (0,0,fb->xe,fb->ye)
 * source (x0,y0,xe,ye)
 * overlap (cx0,cy0,cxe,cye)
 * if either cxe or cye is less then there is no overlap
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtfb_clip(
  dt_ctl_t *ctl,
  dtfb_t *fb,
  void *data,
  int x0,
  int y0,
  int xe,
  int ye,
  void **cdata,
  int *cx0,
  int *cy0,
  int *cxe,
  int *cye)
{
  dt_rc_e rc = DT_RC_GOOD;

  if (y0 < 0)							/* clip top of image?			    */
  {
    data = (char *)data - y0 * xe;
    ye += y0;
    y0 = 0;
  }

  if (y0+ye > fb->ye)					/* clip bottom of image?		    */
    ye = fb->ye - y0;

  if (x0 < 0)							/* clip left of image?			    */
  {
	data = (char *)data - x0;
	xe += x0;
	x0 = 0;
  }

  if (x0+xe > fb->xe)					/* clip right of image?			    */
    xe = fb->xe - x0;

  *cdata = data;
  *cx0 = x0;
  *cy0 = y0;
  *cxe = xe;
  *cye = ye;

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
