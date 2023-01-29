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
| main program
| 
| END
 *..........................................................................*/

#include <tableau.h>
DT_RCSID("app/tableau $RCSfile: get.c,v $ $Revision: 1.8 $");

#define IPC (&(get)->u.ipc.ipc)
#define FB  (&(get)->u.fb.fb)

/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
tableau_ipc_setup(
  dtparam_t *param,
  const char *spec,
  const char *server_command,
  dtipc_t *ipc)
{
  DT_F("tableau_ipc_setup");
  dt_ctl_t *ctl = &param->ctl;
  tableau_command_t command;			/* introductory command to server	*/
  char s[128];
  char *name;
  tableau_ack_t *ack = NULL;
  dt_rc_e rc;
  
  if (spec == NULL ||
	  strlen(spec) == 0)
    return dt_err(ctl, F,
      "NULL or empty input name");

  dtstr_printf(ctl, s, sizeof(s),		/* place to edit spec 				*/
    "%s", spec);
  name = strchr(s, '+');				/* see if spec has a plus sign 		*/
  if (name != NULL)						/* has a plus sign? 				*/
	*name++ = '\0';
  else									/* has no plus sign 				*/
    name = "";

  DT_C(dtipc_find,(ctl, ipc,			/* find the server 					*/
    s, TABLEAU_RECV_MAX, 0, 
    param->epsilon));
  
  memset(&command, 0, 
    sizeof(command));

  dtstr_printf(ctl, command.command,	/* tell server what to do 			*/
    sizeof(command.command),
    "%s", server_command);
  
  dtstr_printf(ctl, command.name,		/* tell server what to use 			*/
    sizeof(command.name),
    "%s", name);
  
  command.frame = DT_CAST(				/* tell server what frame 			*/
    tableau_len_t, param->offset);

  command.version = TABLEAU_VERSION;	/* tell server our version 			*/
  
  DT_C(dtipc_send,(ctl, ipc,			/* send introductory command 		*/
    &command, sizeof(command)));
  
  DT_C(dtipc_recv,(ctl, ipc,			/* wait for ack						*/
    (void **)&ack, sizeof(*ack)));

  ack->rc = 
    DTIPC_FIX(DT_C, ctl, ipc, ack->rc);

  if (ack->rc == DT_RC_BAD)				/* explicit return code 			*/
    rc = dt_err(ctl, F,
      "server error code");
  else
  if (ack->rc == DT_RC_STOP)			/* explicit return code 			*/
    rc = dt_err(ctl, F,
      "server stop code");
  else
  if (ack->rc != DT_RC_GOOD)			/* but not good? 					*/
  {
    rc = dt_err(ctl, F,
      "server version is %04ld,"
      " but our version is %04ld",
      ack->rc, TABLEAU_VERSION);
  }

cleanup:
  if (ack != NULL)
    DT_I(dtos_free2,(ctl, ack,
      F, "ack"));
  
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
