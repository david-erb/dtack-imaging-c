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

#include <tabchief.h>
DT_RCSID("app/tabchief $RCSfile: client.c,v $ $Revision: 1.8 $");

/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
tabchief_client(
  dtparam_t *param,
  const char *ipcspec,
  const char *command)
{
  DT_F("tabchief_client");
  dt_ctl_t *ctl = &param->ctl;
  dtipc_t _ipc, *ipc = &_ipc;
  dt_rc_e rc;

  ipc->type = DTIPC_TYPE_NONE;

  DT_C(dtipc_find,(ctl, ipc,			/* contact the server				*/
    ipcspec, 0, 0, -1));

  if (!strcmp(command, "copyto"))
    DT_C(tabchief_client_copyto,(param, ipc))
  else
  if (!strcmp(command, "copyfrom"))
    DT_C(tabchief_client_copyfrom,(param, ipc))
  else
  {
	long got;
	DT_C(dtipc_write_all,(ctl, ipc,		/* preface with leading slash		*/
      "GET /", 5));
	DT_C(dtipc_write_all,(ctl, ipc,		/* send the command raw 			*/
      command, strlen(command)));
	DT_C(dtipc_write_all,(ctl, ipc,		/* separate from args				*/
      "?", 1));
	DT_C(dtipc_write_all,(ctl, ipc,		/* send the command raw 			*/
      param->input, strlen(param->input)));
    DT_C(dtipc_write_all,(ctl, ipc,		/* terminate with newline 			*/
      "\n\n", 2));
	do {								/* politely listen to reply		 	*/
	  char s[256];
	  int i;
	  DT_C(dtipc_read_some,(ctl, ipc,	
        s, sizeof(s), &got));
	  for (i=0; i<got; i++)
	    putchar(s[i]);
	} while (got > 0);
  }

cleanup:
  if (ipc->type != DTIPC_TYPE_NONE)
    DT_I(dtipc_unfind,(ctl, ipc));		/* release resources				*/

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
