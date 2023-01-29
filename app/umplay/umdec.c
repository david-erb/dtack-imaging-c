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

#include <umplay.h>
U_R("app/umplay $RCSfile: umdec.c,v $ $Revision: 1.8 $");

/* ------------------------------------------------------------------------
 * ------------------------------------------------------------------------ */

u_rc_e 
U_API
umplay_umdec_init(
  umplay_t *umplay,						/* already-initialized structure 	*/
  dtparam_t *param,						/* command-line parameters 			*/
  umdec_t *umdec)						/* umdec to be initialized 			*/
{
  U_F("umplay_umdec_init");
  u_ctl_t *ctl = &param->ctl;
  int umdec_initialized = 0;
  const char *string;
  long x0, y0, height;
  long foreground, background;
  const char *filename;
  u_rc_e rc;

  U_C(umdec_init,(ctl, umdec,			/* create the umdec object			*/
    param->variant, 0L));
  umdec_initialized = 1;
  umplay->control.mode =
	UMDEC_MODE_FORWARD;

										/* ................................ */
  DT_C(dtlist_default_s,(ctl,
    &umplay->config_list,
    UMPLAY_OVERLAY_TEXT_STRING,
    " ", &string));

  DT_C(dtlist_default_l,(ctl,
    &umplay->config_list,
    UMPLAY_OVERLAY_TEXT_X0,
    40, &x0));

  DT_C(dtlist_default_l,(ctl,
    &umplay->config_list,
    UMPLAY_OVERLAY_TEXT_Y0,
    420, &y0));

  DT_C(dtlist_default_l,(ctl,
    &umplay->config_list,
    UMPLAY_OVERLAY_TEXT_HEIGHT,
    32, &height));

  DT_C(dtlist_default_l,(ctl,
    &umplay->config_list,
    UMPLAY_OVERLAY_TEXT_FOREGROUND,
    0x00ffffff, &foreground));

  DT_C(dtlist_default_l,(ctl,
    &umplay->config_list,
    UMPLAY_OVERLAY_TEXT_BACKGROUND,
    0x00010101, &background));

  DT_C(umdec_overlay_text,(ctl,			/* place the text 					*/
    umdec, string,
    (int)x0, (int)y0, (int)height, 
    foreground, background));

										/* ................................ */
  DT_C(dtlist_default_s,(ctl,
    &umplay->config_list,
    UMPLAY_OVERLAY_IMAGE_VP_FILENAME,
    "", &filename));

  DT_C(dtlist_default_l,(ctl,
    &umplay->config_list,
    UMPLAY_OVERLAY_IMAGE_X0,
    40, &x0));

  DT_C(dtlist_default_l,(ctl,
    &umplay->config_list,
    UMPLAY_OVERLAY_IMAGE_Y0,
    40, &y0));

  if (filename != NULL &&
	  filename[0] != '\0')
    DT_C(umdec_overlay_image,(ctl,		/* place the image 					*/
      umdec, filename, 
      (int)x0, (int)y0));
										/* ................................ */
  
cleanup:
  if (rc != DT_RC_GOOD &&
	  umdec_initialized)
    U_I(umdec_uninit,(ctl, umdec));		/* clean up the umdec object 		*/

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
