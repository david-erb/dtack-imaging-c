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
umdemo_op_fifo(
  dtparam_t *param)
{
  U_F("umdemo_op_fifo");
  u_ctl_t *ctl = &param->ctl;
  umdemo_t *umdemo = NULL;
  umdec_t _umdec, *umdec = &_umdec;
  umdemo_deccmd_t deccmd;
  u_rc_e rc;
  
  U_C(umdemo_init,(param, &umdemo));	/* init callback umdemo structure	*/

  dt_dbg(ctl, F, DT_DBG_MASK_USR1,
    "waiting for client to open"
    " data fifo %s",
    param->input);
  U_C(dtfd_open,(ctl,					/* open data fifo 					*/
    &umdemo->data_fifo,
    param->input, "r"));

  dt_dbg(ctl, F, DT_DBG_MASK_USR1,
    "waiting for client to open"
    " command fifo %s",
    param->connect);
  U_C(dtfd_open,(ctl,					/* open command fifo 				*/
    &umdemo->command_fifo,
    param->connect, "r"));

  dt_dbg(ctl, F, DT_DBG_MASK_USR1,
    "waiting for client to open"
    " reply fifo %s",
    param->path);
  U_C(dtfd_open,(ctl,					/* open reply fifo 					*/
    &umdemo->reply_fifo,
    param->path, "w"));
  
  dt_dbg(ctl, F, DT_DBG_MASK_USR1,
    "waiting for client to send"
    " init command on command fifo %s"
    " (%d bytes)", 
    umdemo->command_fifo.filename,
    sizeof(deccmd));

  U_C(dtfd_read_all,(ctl,				/* read initial command 			*/
    &umdemo->command_fifo,
    (void **)&deccmd,
    sizeof(deccmd)));

  if (deccmd.cmd !=
	  DECCMD_INITPARAM)
  {
	rc = dt_err(ctl, F,
      "init command 0 expected"
      " but got command %d",
      (int)deccmd.cmd);
	goto cleanup;
  }

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
