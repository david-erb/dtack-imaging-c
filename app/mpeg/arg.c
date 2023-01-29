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
U_R("app/umdemo $RCSfile: umdemo.c,v $ $Revision: 1.8 $");

/* ------------------------------------------------------------------------
 * ------------------------------------------------------------------------ */

u_rc_e 
U_API
umdemo_init(
  dtparam_t *param,
  umdemo_t **umdemop)
{
  U_F("umdemo_setup");
  u_ctl_t *ctl = &param->ctl;
  umdemo_t *umdemo = NULL;
  int i;
  u_rc_e rc;
  
  U_C(dtos_malloc2,(ctl,				/* space for big umdemo structure 	*/
    (void **)&umdemo, sizeof(*umdemo),
    F, "umdemo"));
  *umdemop = umdemo;

  memset(umdemo, 0, sizeof(*umdemo));	/* clear big umdemo structure 		*/

  ctl->user = (void *)umdemo;			/* start special message handling 	*/
  umdemo->fd.fd = -1;					/* mark stuff uninitialized 		*/
  umdemo->command_fifo.fd = -1;			
  umdemo->data_fifo.fd = -1;			
  umdemo->param = param;
  umdemo->ctl = ctl;
  umdemo->frame_max = param->n;
  
cleanup:
  if (rc != U_RC_GOOD)
  {
    U_I(umdemo_uninit,(param, umdemo));	
	*umdemop = NULL;
  }

  return rc;
}

/* ------------------------------------------------------------------------
 * ------------------------------------------------------------------------ */

u_rc_e 
U_API
umdemo_uninit(
  dtparam_t *param,
  umdemo_t *umdemo)
{
  U_F("umdemo_uninit");
  u_ctl_t *ctl = &param->ctl;
  u_rc_e rc = U_RC_GOOD;

  ctl->user = NULL;						/* cancel special message handling 	*/

  if (umdemo != NULL)
  {
    if (UMDEMO_HAS_KEYBD(umdemo))
      U_I(dtkeybd_uninit,(ctl, &umdemo->keybd));

    if (UMDEMO_HAS_FD(umdemo))
      U_I(dtfd_close,(ctl, &umdemo->fd));

    if (UMDEMO_HAS_REPLY_FIFO(umdemo))
      U_I(dtfd_close,(ctl, &umdemo->reply_fifo));

    if (UMDEMO_HAS_COMMAND_FIFO(umdemo))
      U_I(dtfd_close,(ctl, &umdemo->command_fifo));

    if (UMDEMO_HAS_DATA_FIFO(umdemo))
      U_I(dtfd_close,(ctl, &umdemo->data_fifo));

	if (UMDEMO_HAS_DATA_IPC(umdemo))		/* we have a data pipe open? 		*/
	{
	  U_I(dtipc_unfind,(ctl, 
        &umdemo->data_ipc));
	}

	if (UMDEMO_HAS_COMMAND_IPC(umdemo))		/* we have a command pipe open? 	*/
	{
	  U_I(dtipc_unregister,(ctl, 
        &umdemo->command_ipc));
	}

    U_I(dtos_free2,(ctl, umdemo,
      F, "umdemo"));
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
