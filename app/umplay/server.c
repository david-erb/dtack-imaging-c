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
U_R("app/umplay $RCSfile: server.c,v $ $Revision: 1.8 $");

/* ------------------------------------------------------------------------
 * requires connect and variant
 * ignores input
 * ------------------------------------------------------------------------ */

u_rc_e 
U_API
umplay_op_server(
  dtparam_t *param)
{
  U_F("umplay_op_server");
  u_ctl_t *ctl = &param->ctl;
  umdec_t _umdec, *umdec = &_umdec;
  int umdec_initialized = 0;
  umplay_t *umplay = NULL;
  u_rc_e rc;
  
  U_C(umplay_init,(param, &umplay));	/* init umplay structure			*/

  U_C(umplay_log_setup,(ctl));			/* set up file logging 				*/
  
  U_C(umplay_umdec_init,(umplay, param,	/* initialize the umdec object 		*/
    umdec));
  umdec_initialized = 1;

  U_C(dtipc_register,(ctl,				/* establish listen address			*/
    &umplay->command_ipc,
    param->connect, 0, 0));

  while(1)
  {
	umplay->control.mode =				/* prepare initial state 			*/
	  UMDEC_MODE_FORWARD;
	U_C(umdec_go,(ctl, umdec,			/* this starts playing immediately 	*/
      umplay_feed, (void *)umplay,
      umplay_frame, (void *)umplay,
      &umplay->control));
	if (umplay->exiting)				/* exit command issued to us? 		*/
	  goto cleanup;
  }
  
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
