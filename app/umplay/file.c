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
U_R("app/umplay $RCSfile: file.c,v $ $Revision: 1.8 $");

/* ------------------------------------------------------------------------
 * requires input and variant
 * ignores connect
 * ------------------------------------------------------------------------ */

u_rc_e 
U_API
umplay_op_file(
  dtparam_t *param)
{
  U_F("umplay_op_file");
  u_ctl_t *ctl = &param->ctl;
  umplay_t *umplay = NULL;
  dtos_time_t beg, end;
  umdec_t _umdec, *umdec = &_umdec;
  int umdec_initialized = 0;
  u_rc_e rc;
  
  U_C(umplay_init,(param, &umplay));	/* init callback umplay structure	*/

  U_C(dtfd_open,(ctl, &umplay->fd,		/* open mpeg file for input 		*/
    param->input, "r"));
  
  {
	unsigned long decade;
    U_C(dtfd_read_all,(ctl,				/* check out nci file signature 	*/
      &umplay->fd,
      (void **)&decade, 
      sizeof(decade)));
	U_C(dtfd_seek,(ctl,					/* seek back to start of file 		*/
      &umplay->fd, 0,
      DTFD_WHENCE_SET, NULL));
	if (decade == 0xdecade00 ||			/* nci file signature found? 		*/
		decade == 0x00decade)
  	  U_C(dtfd_seek,(ctl,				/* seek past file signature			*/
        &umplay->fd, 400,
        DTFD_WHENCE_SET, NULL));
  }

  if (strcmp(param->keyboard, "none"))
  {
	U_C(dtkeybd_init,(ctl,				/* get us a keyboard 				*/
      &umplay->keybd, param->keyboard, 
      0));
    umplay->keybd_initialized = 1;
	u_say(ctl, F, 
      "%c=forward %c=pause %c=quit",
      UMPLAY_KEY_FORWARD,
      UMPLAY_KEY_PAUSE,
      UMPLAY_KEY_QUIT);
  }

  U_C(dtos_time,(ctl, &beg));
										/* ................................ */
  U_C(umplay_umdec_init,(umplay, param,	/* initialize the umdec object 		*/
    umdec));
  umdec_initialized = 1;
  
  umplay->control.mode =				/* define initial mode 				*/
    UMDEC_MODE_FORWARD;

  U_C(umdec_go,(ctl, umdec,				/* this starts playing immediately 	*/
    umplay_feed, (void *)umplay,
    umplay_frame, (void *)umplay,
    &umplay->control));
										/* ................................ */
  U_C(dtos_time,(ctl, &end));

  u_dbg(ctl, F, DT_DBG_MASK_TIME,
    "%d frames in %0.1f seconds,"
    " %0.2f frames/second",
    umplay->frame,
    (end.wall-beg.wall),
    umplay->frame /
    (end.wall-beg.wall));
  
cleanup:
  if (umdec_initialized)				/* umdec object got initialized? 	*/
    U_I(umdec_uninit,(ctl, umdec));		/* clean up the umdec object 		*/
  if (umplay != NULL)
    U_I(umplay_uninit,(param, umplay));

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
