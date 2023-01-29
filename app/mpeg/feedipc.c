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
U_R("app/umdemo $RCSfile: feedipc.c,v $ $Revision: 1.8 $");
  
/*..........................................................................*/

u_rc_e 
U_API
umdemo_ipc_command(
  umdemo_t *umdemo)
{
  U_F("umdemo_ipc_command");
  u_ctl_t *ctl = umdemo->ctl;
  char s[256];
  u_rc_e rc = U_RC_GOOD;

  U_Q(dtipc_read_line,(ctl,				/* receive command from client 		*/
    &umdemo->command_ipc, s, sizeof(s)));

  if (s[strlen(s)-1] != '\n')			/* no newline at end? 				*/
  {
	dtstr_printf(ctl,					/* assume line was too long 		*/
      umdemo->response,
      sizeof(umdemo->response),
      "command too long");
  }
  else									/* full line received? 				*/
  {
	s[strlen(s)-1] = '\0';				/* nuke the newline 				*/
	U_G(umdemo_command,(umdemo, s));	/* handle the command 				*/
  }
  
  U_Q(dtipc_write,(ctl,					/* acknowledge command 				*/
    &umdemo->command_ipc, 
    umdemo->response,
    strlen(umdemo->response)));
  dt_dbg(ctl, F, DT_DBG_MASK_USR2,
    "replying %s", umdemo->response);
  
  U_Q(dtipc_write,(ctl,					/* terminate acknowledgment			*/
    &umdemo->command_ipc, "\n", 1)); 

  U_Q(dtipc_read_line,(ctl,				/* receive command-end from client	*/
    &umdemo->command_ipc, 
    s, sizeof(s)));

  return rc;
}

/*..........................................................................*/

u_rc_e 
U_API
umdemo_ipc_poll(
  umdemo_t *umdemo)
{
  U_F("umdemo_ipc_poll");
  u_ctl_t *ctl = umdemo->ctl;
  int ready;
  int accepted = 0;
  int rc = U_RC_GOOD;

  U_Q(dtipc_poll,(ctl,					/* check for incoming connection 	*/
    &umdemo->command_ipc,
    DTIPC_POLL_ACCEPT, 0,
    "connection from a client",
    &ready));

  if (!ready)							/* nobody talking to us?		    */
    return U_RC_GOOD;

  U_C(dtipc_wait,(ctl,					/* accept the connection		    */
    &umdemo->command_ipc));
  accepted = 1;

  U_C(umdemo_ipc_command,(umdemo));		/* read and handle the command	    */

cleanup:
  if (accepted)
    U_I(dtipc_done,(ctl,
      &umdemo->command_ipc));

  return U_RC_GOOD;						/* ignore return code 				*/
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
