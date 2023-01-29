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

#include <tabgrab.h>
DT_RCSID("app/tabgrab $RCSfile: client.c,v $ $Revision: 1.8 $");

#define CR (0x0d)
#define LF (0x0a)

/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
tabgrab_client(
  dtparam_t *param,
  const char *command)
{
  DT_F("tabgrab_client");
  dt_ctl_t *ctl = &param->ctl;
  dtipc_t _ipc, *ipc = &_ipc;
  int ipc_initialized = 0;
  char crlf[3] = {CR, LF, '\0'};
  char reply[256];
  int l;
  dt_rc_e rc;

  DT_C(dtipc_find,(ctl, ipc,
    param->connect, 0, 0, 1000));
  ipc_initialized = 1;

  DT_C(dtipc_write_all,(ctl, ipc,
    command, strlen(command)));
  DT_C(dtipc_write_all,(ctl, ipc,
    crlf, strlen(crlf)));

  DT_C(dtipc_read_line,(ctl, ipc,
    reply, sizeof(reply)));

	l = strlen(reply);
	while (l > 0 &&						/* nuke the crlf's 					*/
		   reply[l-1] == LF ||
		   reply[l-1] == CR)
	  reply[--l] = '\0';

  dt_dbg(ctl, F, DT_DBG_MASK_ANY,
    "reply: %s", reply);
  
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
