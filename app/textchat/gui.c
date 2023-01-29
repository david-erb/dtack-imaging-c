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
| textchat_* - textchat internal routine
| 
|.DESCRIPTION
| Undocumented.
| 
| END
 *..........................................................................*/

#include <textchat.h>
DT_RCSID("app/textchat $RCSfile: run.c,v $ $Revision: 1.8 $");

/*..........................................................................
 *..........................................................................*/

dt_rc_e 
DTCONFIG_API1
textchat_op_gui(
  dtparam_t *param)
{
  DT_F("textchat_op_gui");
  dt_ctl_t *ctl = &param->ctl;
  textchat_t *textchat = NULL;
  dtdg_server_t _dgserver, *dgserver = &_dgserver;
  dt_rc_e rc;

  dgserver->initialized = 0;

  DT_C(textchat_init,(&textchat,		/* initialize dialog and stuff 		*/
    param));

  DT_C(dtdg_server_init,(ctl, dgserver,	/* become a datagram server 		*/
    textchat->dgserver_spec));

  DT_C(textchat_update,(textchat));		/* draw the initial screen			*/
  
  while (1)
  {
	DT_C(textchat_request_poll,(		/* poll for and handle input 		*/
      textchat,	dgserver));
	  
	DT_C(textchat_state,(textchat));	/* get current audio state 			*/

	DT_C(textchat_update,(textchat));	/* update the screen				*/

	DT_C(dtos_delay,(ctl, param->dt));	/* wait before polling again 		*/
  }

cleanup:

  DT_I(dtdg_server_uninit,(ctl,			/* quit being a datagram server 	*/
    dgserver));
  
  DT_I(textchat_uninit,(textchat));
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
