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
U_R("app/umdemo $RCSfile: server.c,v $ $Revision: 1.8 $");

/* ------------------------------------------------------------------------
 * requires connect and variant
 * ignores input
 * ------------------------------------------------------------------------ */

u_rc_e 
U_API
umdemo_op_server(
  dtparam_t *param)
{
  U_F("umdemo_op_server");
  u_ctl_t *ctl = &param->ctl;
  umdemo_t *umdemo = NULL;
  umdec_t _umdec, *umdec = &_umdec;
  u_rc_e rc;
  
  U_C(umdemo_init,(param, &umdemo));	/* init callback umdemo structure	*/

  U_C(dtipc_register,(ctl,				/* establish listen address			*/
    &umdemo->command_ipc,
    param->connect, 0, 0));

  while(1)
  {
	umdemo->pausing = 0;
	umdemo->stopping = 0;
    U_C(umdec_init,(ctl, umdec,			/* this starts playing immediately 	*/
      param->variant,
      umdemo_feed, (void *)umdemo,
      umdemo_frame, (void *)umdemo,
      0L));	
    U_C(umdec_uninit,(ctl, umdec));		/* clean up the umdec object 		*/
	if (umdemo->exiting)
	  goto cleanup;
  }
  
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
