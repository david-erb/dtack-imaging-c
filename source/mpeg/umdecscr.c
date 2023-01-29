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
| ERRORS
| As yet undocumented.
| 
| END
 *..........................................................................*/

#include <dtack/base.h>
DT_RCSID("source/mpeg $RCSfile: umdecscr.c,v $ $Revision: 1.8 $");
#include <umdec.h>

#include <dtack/lut8.h>
#include <dtack/list.h>
#include <dtack/svc.h>
#include <dtack/scr.h>
#include <dtack/color.h>
#include <umdecscr.h>

#include <dtack/mem.h>
#include <dtack/os.h>

#ifndef DTDEPEND
#  include <ssg.h>						/* this shouldn't really be here 	*/
#endif

/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
umdec_scr_create(
  dt_ctl_t *ctl,							/* environment control 				*/
  umdec_scr_t *umdec_scr,	
  const char *scr_spec)
{
  DT_F("umdec_scr_create");
  int i;
  
  memset(umdec_scr, 0, sizeof(*umdec_scr));

  DT_Q(dtos_malloc2,(ctl, 
    (void **)&umdec_scr->scr,
    sizeof(*umdec_scr->scr),
	F, "scr structure"));

  DT_Q(dtscr_create,(ctl,
    umdec_scr->scr, scr_spec));

										/* ................................ */
  DT_Q(dt_color_org_set,(ctl,			/* set up color org structures		*/
    &umdec_scr->org_y_cb_cr,
    DT_COLOR_ORG_Y_CB_CR));

  DT_Q(dt_color_org_set,(ctl,
    &umdec_scr->org_pseudo,
    DT_COLOR_ORG_MAP));

  DT_Q(dt_color_convert_init,(ctl,		/* prepare for conversion 			*/
    &umdec_scr->convert,
    &umdec_scr->org_y_cb_cr,
    &umdec_scr->org_pseudo));
  umdec_scr->convert.ncells = 
    UMDEC_SCR_PSEUDO_CELL_COUNT;
  umdec_scr->color_initialized = 1;
	
  for (i=0;
	   i<UMDEC_SCR_PSEUDO_CELL_COUNT; 
	   i++)
	umdec_scr->pseudo_remap[i] =
      UMDEC_SCR_PSEUDO_CELL_START + i;
										/* ................................ */
  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
umdec_scr_destroy(
  dt_ctl_t *ctl,							/* environment control 				*/
  umdec_scr_t *umdec_scr)
{
  DT_F("umdec_scr_destroy");
  dt_rc_e rc = DT_RC_GOOD;
  if (umdec_scr != NULL)
  {
    if (umdec_scr->color_initialized)
	  DT_I(dt_color_convert_uninit,(	/* clean up after conversion		*/
        ctl,
        &umdec_scr->convert,
        &umdec_scr->org_y_cb_cr,
        &umdec_scr->org_pseudo));

    if (umdec_scr->tmp != NULL)
      DT_I(dtos_free2,(ctl, umdec_scr->tmp,
        F, "tmp"));

    if (umdec_scr->image.xe)
      DT_I(dtimage_free,(ctl, &umdec_scr->image));
  
	if (umdec_scr->scr != NULL)
	  DT_I(dtscr_destroy,(ctl,
        umdec_scr->scr));

	DT_I(dtos_free2,(ctl, 
      umdec_scr->scr,
  	  F, "scr structure"));
	memset(umdec_scr, 0, sizeof(*umdec_scr));
  }
  return DT_RC_GOOD;
}

/*..........................................................................
 * this is a umdec frame callback
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
umdec_scr_callback(
  dt_ctl_t *ctl,							/* environment control 				*/
  umdec_t *umdec,						/* umdec object 					*/
  void *arg,							/* caller's unchanged argument 		*/
  int frametype,						/* type of frame being given 		*/
  void **buffer,						/* buffers containing frame data	*/
  int xe,								/* frame width 						*/
  int ye)								/* frame height						*/
{
  DT_F("umdec_scr_callback");
  umdec_scr_t *umdec_scr = 
    (umdec_scr_t *)arg;
  dt_rc_e rc;

  if (frametype ==
	  SSG_FRAMETYPE_YCBCR_420)
  {
	DT_Q(umdec_scr_raw,(ctl, umdec_scr,	/* render frame to screen 			*/
      buffer, xe, ye));
  }
  else
  {
	return dt_err(ctl, F,
      "cannot handle SSG_FRAMETYPE=%d",
      frametype);
  }
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
