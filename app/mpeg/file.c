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

#include <umdemo.h>
U_R("app/umdemo $RCSfile: file.c,v $ $Revision: 1.8 $");

/* ------------------------------------------------------------------------
 * requires input and variant
 * ignores connect
 * ------------------------------------------------------------------------ */

u_rc_e 
U_API
umdemo_op_file(
  dtparam_t *param)
{
  U_F("umdemo_op_file");
  u_ctl_t *ctl = &param->ctl;
  umdemo_t *umdemo = NULL;
  dtos_time_t beg, end;
  umdec_t _umdec, *umdec = &_umdec;
  u_rc_e rc;
  
  U_C(umdemo_init,(param, &umdemo));	/* init callback umdemo structure	*/

  U_C(dtfd_open,(ctl, &umdemo->fd,		/* open mpeg file for input 		*/
    param->input, "r"));

  U_C(dtkeybd_init,(ctl,				/* get us a keyboard 				*/
    &umdemo->keybd, param->keyboard, 
    0));
  umdemo->keybd_initialized = 1;
  u_say(ctl, F, 
    "%c=forward %c=pause %c=quit",
    UMDEMO_KEY_FORWARD,
    UMDEMO_KEY_PAUSE,
    UMDEMO_KEY_QUIT);

  U_C(dtos_time,(ctl, &beg));
										/* ................................ */
  U_C(umdec_init,(ctl, umdec,			/* this starts playing immediately 	*/
    param->variant,
    umdemo_feed, (void *)umdemo,
    umdemo_frame, (void *)umdemo,
    0L));	
  U_C(umdec_uninit,(ctl, umdec));		/* clean up the umdec object 		*/
										/* ................................ */
  U_C(dtos_time,(ctl, &end));

  u_dbg(ctl, F, DT_DBG_MASK_TIME,
    "%d frames in %0.1f seconds,"
    " %0.2f frames/second",
    umdemo->frame_last,
    (end.wall-beg.wall),
    umdemo->frame_last /
    (end.wall-beg.wall));
  
cleanup:
  if (umdemo != NULL)
    U_I(umdemo_uninit,(param, umdemo));

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
