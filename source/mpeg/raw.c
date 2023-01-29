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
DT_RCSID("source/mpeg $RCSfile: raw.c,v $ $Revision: 1.8 $");
#include <umdec.h>

#include <dtack/lut8.h>
#include <dtack/list.h>
#include <dtack/svc.h>
#include <dtack/scr.h>
#include <dtack/color.h>
#include <umdecscr.h>

#include <dtack/xchg.h>

#include <dtack/mem.h>
#include <dtack/os.h>

/*..........................................................................
 *..........................................................................*/

dt_rc_e 
DTCONFIG_API1
umdec_scr_raw(
  dt_ctl_t *ctl,
  umdec_scr_t *umdec_scr,
  void **buffer,
  long xe,
  long ye)
{
  DT_F("umdec_scr_raw");
  dt_rc_e rc;

  DT_C(dtscr_wait_event,(ctl,			/* poll for events					*/
    umdec_scr->scr, NULL, 0));
  
  if (umdec_scr->image.xe == 0)			/* image not created yet? 			*/
  {
    DT_C(dtimage_create3,(ctl,			/* allocate space for pseudo image	*/
	  &umdec_scr->image,
      0, 0, xe/2, ye/2, 
      DT_NTYPE_B08, NULL));
  }

  if (umdec_scr->tmp == NULL)
    DT_C(dtos_malloc2,(ctl, 
      &umdec_scr->tmp, (xe/2) * (ye/2),
      F, "tmp"));

  DT_C(umdec_scr_fast_decimate,(ctl,
    buffer[0], xe, ye, 
    umdec_scr->tmp));

  DT_C(dt_color_convert3,(ctl,			/* dither 							*/
    &umdec_scr->convert,
    umdec_scr->tmp,
    buffer[1],
    buffer[2],
    &umdec_scr->org_y_cb_cr,
    umdec_scr->image.xe, 
    umdec_scr->image.ye,
    umdec_scr->image.row[0].dvoid,
    umdec_scr->skiplut?					/* possibly generate new lut 		*/
      NULL: umdec_scr->lut,
    NULL,
    &umdec_scr->org_pseudo));

  DT_C(umdec_scr_fast_lookup,(ctl,		/* remap pseudo into 10..246 		*/
    &umdec_scr->image,
    umdec_scr->pseudo_remap,
    &umdec_scr->image));

  DT_C(dtxchg_scr_export_image,(ctl,		/* display frame					*/
    umdec_scr->scr, 
    UMDEC_SCR_WINDOW_NAME, 
    &umdec_scr->image,
    DTIMAGE_NOPOS, DTIMAGE_NOPOS));

  if (!umdec_scr->skiplut)				/* new lut was generated?			*/
  {
    DT_C(dtscr_export_color,(ctl,		/* output the lut 					*/
      umdec_scr->scr, 
      UMDEC_SCR_WINDOW_NAME, 
      umdec_scr->lut,
      UMDEC_SCR_PSEUDO_CELL_START,
      UMDEC_SCR_PSEUDO_CELL_COUNT));
	umdec_scr->skiplut = 1; 
  }

cleanup:
  DT_I(dtscr_wait_event,(ctl,			/* poll for events					*/
    umdec_scr->scr, NULL, 0));

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
