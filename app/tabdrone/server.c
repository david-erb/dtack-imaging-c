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

#include <tabdrone.h>
DT_RCSID("app/tabdrone $RCSfile: server.c,v $ $Revision: 1.8 $");

/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
tabdrone_server(
  dtparam_t *param)
{
  DT_F("tabdrone_server");
  dt_ctl_t *ctl = &param->ctl;
  dtipc_t _ipc, *ipc = &_ipc;
  tabdrone_t _tabdrone;
  tabdrone_t *tabdrone = &_tabdrone;
  int major, minor;
  long date;
  int isready;
  int connected = 0;
  dt_rc_e rc;

  ipc->type = DTIPC_TYPE_NONE;

  DT_C(tabdrone_init,(tabdrone,			/* init application structure 		*/
    param));
  
  DT_C(dtipc_register,(ctl, ipc,		/* register as a server 			*/
    param->connect, 0, 0));

  DT_C(tabdrone_rev,(ctl, NULL,
    &major, &minor, &date));

  dt_dbg(ctl, NULL, DT_DBG_MASK_USR1,	/* announce readiness 				*/
    "%02d-%02d (%ld) %s %s",
    major, minor, date,
    param->ident, param->name);

  DT_C(dtipc_poll,(ctl, ipc,			/* wait for incoming connection		*/
    DTIPC_POLL_ACCEPT, 
    param->dt,
    "client connect", &isready));
  if (!isready)							/* client is connecting? 			*/
  {
	rc = dt_err(ctl, F,
      "client did not connect"
      " in time");
	goto cleanup;
  }

  DT_C(dtipc_wait,(ctl, ipc));			/* accept connection				*/
  connected = 1;

#if 0
  {
	int isready;
      DT_C(dtipc_poll,(ctl, ipc,		/* poll for incoming data 			*/
        DTIPC_POLL_READ, 
        -1,
        "ipc read", &isready));
	  if (!isready)						/* no data ready? 					*/
	    goto cleanup;
  }
#endif

  DT_C(tabdrone_verify,(param, ipc));	/* verify client identity 			*/

  DT_C(tabdrone_start,(tabdrone));		/* hit the start url 				*/
    
  DT_C(tabdrone_pump,(tabdrone, ipc));	/* pump input to output				*/

cleanup:
  if (connected)
    DT_I(dtipc_done,(ctl, ipc));		/* server finishes transaction	    */

  if (ipc->type != DTIPC_TYPE_NONE)
    DT_I(dtipc_unregister,(ctl, ipc));	/* release resources				*/

  DT_I(tabdrone_done,(tabdrone));		/* hit the done url 				*/
  
  DT_I(tabdrone_uninit,(tabdrone));		/* uninit application structure		*/

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
