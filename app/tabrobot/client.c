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
  CHANGES
  20000223 DE added timeouts
 *..........................................................................*/

/*..........................................................................
| NAME
| main program
| 
| END
 *..........................................................................*/

#include <tabrobot.h>
DT_RCSID("app/tabrobot $RCSfile: client.c,v $ $Revision: 1.8 $");

#define CR (0x0d)
#define LF (0x0a)

/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
tabrobot_client_send(
  dtparam_t *param,
  dtipc_t *ipc,
  const char *command,
  long *turnaround)
{
  DT_F("tabrobot_client_send");
  dt_ctl_t *ctl = &param->ctl;
  char t[256];
  long i, l;
  int isready;
  dtos_time_t beg, end;
  dt_rc_e rc;

  {
	dt_dbg(ctl, F, DT_DBG_MASK_COMMAND, 
      "%s", command);
    DT_C(dtstr_printf,(ctl,				/* add CRLF 						*/
      t, sizeof(t),
      "%s%c%c", command, CR, LF));
    DT_C(dtipc_poll,(ctl, ipc,
      DTIPC_POLL_WRITE, param->epsilon,
      "write", &isready));
	if (!isready)
	  DT_CLEANUP((ctl, F,
        "timeout %g waiting to write",
        param->epsilon));
    DT_C(dtos_time,(ctl, &beg));
	DT_C(dtipc_write_all,(ctl, ipc,		/* send command itself 				*/
      t, strlen(t)));

	DT_C(dtipc_read_line2,(ctl, ipc,	/* wait for ack 					*/
      param->epsilon, t, sizeof(t)));
	DT_C(dtos_time,(ctl, &end));

	l = strlen(t);
	while (l > 0 &&						/* nuke the crlf's 					*/
		   t[l-1] == LF ||
		   t[l-1] == CR)
	  t[--l] = '\0';
		
	if (l == 0)
	{
	  dt_dbg(ctl, NULL, 
        DT_DBG_MASK_COMMAND, 
        "empty response from server");
	  goto cleanup;
	}
	
	dt_dbg(ctl, F, 
      DT_DBG_MASK_REPLY |
      DT_DBG_MASK_TIME,
      "reply: %s (%g ms)", t,
      (end.wall-beg.wall)*1000.0);

	while(strlen(t) > 2)
	{
	  t[0] = '\0';
	  DT_C(dtipc_read_line2,(ctl, ipc,	/* wait for ack 					*/
        param->epsilon, t, sizeof(t)));
	  l = strlen(t);
	  while (l > 0 &&					/* nuke the crlf's 					*/
			 t[l-1] == LF ||
			 t[l-1] == CR)
		t[--l] = '\0';
	  dt_dbg(ctl, F, 
        DT_DBG_MASK_REPLY,
        "reply: %s", t);
	}
  }
  
cleanup:
  return rc;
}

/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
tabrobot_client(
  dtparam_t *param,
  const char *command)
{
  DT_F("tabrobot_client");
  dt_ctl_t *ctl = &param->ctl;
  dtipc_t _ipc, *ipc = &_ipc;
  int ipc_initialized = 0;
  long turnaround;
  dt_rc_e rc;

  DT_C(dtipc_find,(ctl, ipc,			/* connect to server 				*/
    param->connect, 0, 0, param->epsilon));
  ipc_initialized = 1;

  DT_C(tabrobot_client_send,(param,		/* send client command 				*/
    ipc, command, &turnaround));
  
cleanup:
  if (ipc_initialized)
	DT_I(dtipc_unfind,(ctl, ipc));
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
