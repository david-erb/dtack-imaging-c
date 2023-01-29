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
U_R("app/umplay $RCSfile: feedipc.c,v $ $Revision: 1.8 $");
  
/*..........................................................................*/

u_rc_e 
U_API
umplay_ipc_command(
  umplay_t *umplay)
{
  U_F("umplay_ipc_command");
  u_ctl_t *ctl = umplay->ctl;
  char s[256];
  u_rc_e rc = U_RC_GOOD;

  U_Q(dtipc_read_line,(ctl,				/* receive command from client 		*/
    &umplay->command_ipc, s, sizeof(s)));

  if (s[strlen(s)-1] != '\n')			/* no newline at end? 				*/
  {
	dtstr_printf(ctl,					/* assume line was too long 		*/
      umplay->response,
      sizeof(umplay->response),
      "command too long");
  }
  else									/* full line received? 				*/
  {
	s[strlen(s)-1] = '\0';				/* nuke the newline 				*/
	U_G(umplay_command,(umplay, s));	/* handle the command 				*/
  }
  
  U_Q(dtipc_write_all,(ctl,				/* acknowledge command 				*/
    &umplay->command_ipc, 
    umplay->response,
    strlen(umplay->response)));
  dt_dbg(ctl, F, DT_DBG_MASK_USR2,
    "replying %s", umplay->response);
  
  U_Q(dtipc_write_all,(ctl,				/* terminate acknowledgment			*/
    &umplay->command_ipc, "\n", 1)); 

  U_Q(dtipc_read_line,(ctl,				/* receive command-end from client	*/
    &umplay->command_ipc, 
    s, sizeof(s)));

  return rc;
}

/*..........................................................................*/

u_rc_e 
U_API
umplay_ipc_poll(
  umplay_t *umplay)
{
  U_F("umplay_ipc_poll");
  u_ctl_t *ctl = umplay->ctl;
  int ready;
  int accepted = 0;
  int rc = U_RC_GOOD;

  U_Q(dtipc_poll,(ctl,					/* check for incoming connection 	*/
    &umplay->command_ipc,
    DTIPC_POLL_ACCEPT, 0,
    "connection from a client",
    &ready));

  if (!ready)							/* nobody talking to us?		    */
    return U_RC_GOOD;

  U_C(dtipc_wait,(ctl,					/* accept the connection		    */
    &umplay->command_ipc));
  accepted = 1;

  U_C(umplay_ipc_command,(umplay));		/* read and handle the command	    */

cleanup:
  if (accepted)
    U_I(dtipc_done,(ctl,
      &umplay->command_ipc));

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
