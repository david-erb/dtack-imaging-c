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

#include <tabqueue.h>
DT_RCSID("app/tabqueue $RCSfile: client.c,v $ $Revision: 1.8 $");

#define CR (0x0d)
#define LF (0x0a)

/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
tabqueue_client_send(
  dtparam_t *param,
  dtipc_t *ipc,
  const char *command,
  long *turnaround)
{
  DT_F("tabqueue_client_send");
  dt_ctl_t *ctl = &param->ctl;
  char t[256];
  long i;
  char c;
  dtos_time_t beg, end;
  dt_rc_e rc;

  for (i=0; i<param->n; i++)
  {
    DT_C(dtos_time,(ctl, &beg));
    DT_C(dtstr_printf,(ctl,				/* add CRLF 						*/
      t, sizeof(t),
      "%s%c%c", command, CR, LF));
	DT_C(dtipc_write_all,(ctl, ipc,		/* send command itself 				*/
      t, strlen(t)));
	DT_C(dtipc_read_all,(ctl, ipc,		/* wait for ack 					*/
      &c, 1));
	DT_C(dtos_time,(ctl, &end));

	dt_dbg(ctl, F, 
      DT_DBG_MASK_REPLY |
      DT_DBG_MASK_TIME,
      "reply: 0x%02x (%g ms)", c,
      (end.wall-beg.wall)*1000.0);
  }
  
cleanup:
  return rc;
}

/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
tabqueue_client(
  dtparam_t *param,
  const char *command)
{
  DT_F("tabqueue_client");
  dt_ctl_t *ctl = &param->ctl;
  dtipc_t _ipc, *ipc = &_ipc;
  int ipc_initialized = 0;
  long turnaround;
  dt_rc_e rc;

  DT_C(dtipc_find,(ctl, ipc,			/* connect with the server 			*/
    param->connect, 0, 0, 1000));
  ipc_initialized = 1;

  DT_C(tabqueue_client_send,(param,		/* send the command 				*/
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
