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
 * ------------------------------------------------------------------------ */

u_rc_e 
U_API
umdemo_op_control_transact(
  dtparam_t *param,
  const char *command,
  char *reply,
  int maxreply)
{
  U_F("umdemo_op_control_transact");
  u_ctl_t *ctl = &param->ctl;
  dtipc_t _ipc, *ipc = &_ipc;
  int connected = 0;
  u_rc_e rc;

  U_C(dtipc_find,(ctl, ipc,				/* contact server 					*/
    param->connect, 0, 0));
  connected = 1;
  U_C(dtipc_write,(ctl, ipc,			/* write command 					*/
    command, strlen(command)));
  U_C(dtipc_write,(ctl, ipc,			/* command termination 				*/
    "\n", 1));	
  U_C(dtipc_read_line,(ctl, ipc,		/* read response 					*/
    reply, maxreply));
  reply[strlen(reply)-1] = '\0';		/* nuke the newline 				*/
  U_C(dtipc_write,(ctl, ipc,			/* transaction termination			*/
    "\n", 1));	

cleanup:
  if (connected)
    U_I(dtipc_unfind,(ctl, ipc));		/* finish transaction 				*/

  return rc;
}

/* ------------------------------------------------------------------------
 * ------------------------------------------------------------------------ */

u_rc_e 
U_API
umdemo_op_control_keybd(
  dtparam_t *param)
{
  U_F("umdemo_op_control_keybd");
  u_ctl_t *ctl = &param->ctl;
  dtkeybd_t keybd;
  int keybd_initialized = 0;
  char prompt[128];
  const char *string;
  char command[256];
  u_rc_e rc;

  U_C(dtkeybd_init,(ctl,				/* get us a keyboard 				*/
    &keybd, param->keyboard, 0));
  keybd_initialized = 1;

  U_C(umdemo_command_prompt,(ctl,		/* make prompt 						*/
    prompt, sizeof(prompt)-10));
  u_say(ctl, F, "%s %c=%s .=end",		/* emit prompt 						*/
    prompt,
    UMDEMO_KEY_SOURCE,
    UMDEMO_COMMAND_SOURCE);
  
  while(1)
  {
	dt_key_t key;
	U_C(dtkeybd_read,(ctl, &keybd,		/* wait for key to be hit 			*/
      &key));
	if (key == '.')						/* quit session? 					*/
	  break;
	U_C(umdemo_command_string_from_key,(	/* look up command from key 		*/
      ctl, key, &string,
      prompt, sizeof(prompt)));

	if (key == UMDEMO_KEY_SOURCE)		/* special case: get filename 		*/
	{
	  dtstr_printf(ctl,
        command, sizeof(command),
        "enter filename: ");
	  u_say_string(ctl,
        command, strlen(command));
	  dtstr_printf(ctl,
        command, sizeof(command),
        "%s ", string);
	  U_Q(umdemo_keybd_line,(ctl,		/* get filename from console 		*/
        &keybd,
        command+strlen(command),
        sizeof(command)-strlen(command)));
	  string = command;
	}

	if (string != NULL)					/* good command? 					*/
  	  U_C(umdemo_op_control_transact,(	/* send command 					*/
        param, string,
        prompt, sizeof(prompt)));
	
	u_say(ctl, F, "%s", prompt);		/* emit response					*/
  }
  
cleanup:
  if (keybd_initialized)
    U_I(dtkeybd_uninit,(ctl, &keybd));

  return rc;
}

/* ------------------------------------------------------------------------
 * requires input and variant
 * ignores connect
 * if input == "key", use keyboard interactively
 * else if input is a known command, send the command
 * else try to open input as a readable file
 * ------------------------------------------------------------------------ */

u_rc_e 
U_API
umdemo_op_control(
  dtparam_t *param)
{
  U_F("umdemo_op_control");
  u_ctl_t *ctl = &param->ctl;
  const char *command = param->input;
  char reply[256];
  int indx;

  U_Q(umdemo_command_index_from_string,(
    ctl, command, &indx,
    reply, sizeof(reply)));
  
  if (!strcmp(command, "key"))			/* supposed to use keyboard? 		*/
  {
	U_Q(umdemo_op_control_keybd,(
      param));
  }
  else
  if (indx >= 0)						/* this is a recognized command? 	*/
  {
	char response[256];
	U_Q(umdemo_op_control_transact,(	/* send command 					*/
      param, command,
      response, sizeof(response)));
	u_dbg(ctl, F, DT_DBG_MASK_USR1,
      "%s", response);
  }
  else									/* presume it's a filename 			*/
  {
	int isreadable;
	U_Q(dtos_isreadable,(ctl,
      command, &isreadable));
	if (!isreadable)
      return u_err(ctl, F, "Sorry,"
		" \"%s\" is neither a command"
        " nor a readable file.",
        param->input);
	return u_err(ctl, F,
      "not implemented");
  }

  return U_RC_GOOD;
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
