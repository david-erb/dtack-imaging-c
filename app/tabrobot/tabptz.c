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
| USAGE
| tabptz - client/server IP address mapper
| 
|.SYNOPSIS
|:tabptz [params] -connect host:port
|
|.SUMMARY
| Handle specialized PTZ socket commands.
| 
|.PARAMETERS
| :-connect:	connection spec
|dtinclude(params1.dx)
|
|.EXAMPLES
| Server:
|:  tabptz -connect 156.46.122.16:5790
| 
| Client:
|:  tabptz -connect 156.46.122.16:5300 -name myname
|
|dtinclude(params2.dx)
|
| END
 *..........................................................................*/

/*..........................................................................
  CHANGES
  19990519 DE poll for command/data
  19990517 DE explicitly non-console app
 *..........................................................................*/

#include <tabrobot.h>
DT_RCSID("tabrobot $RCSfile: tabptz.c,v $ $Revision: 1.8 $");

#undef DTPRJ_CONSOLE
#define DTPRJ_CONSOLE DT_0				/* make this a non-console app 		*/

#include <dtptz.h>
#include <dtack/licens.h>
#include <dtack/rev.h>

#include <dtack/main.h>

#include <tabptz.use>

#define dt_interactive_parse			/* don't use dialog 				*/\
  dt_enter_parse

#define CR 0x0d
#define LF 0x0a

/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
tabptz_rev(
  dt_ctl_t *ctl,
  const char **rev_string,
  int *rev_major,
  int *rev_minor,
  long *rev_date)
{
  if (rev_string != NULL)
    *rev_string = "tabptz app";
  if (rev_major != NULL)
    *rev_major = 1;
  if (rev_minor != NULL)
    *rev_minor = 9;
  if (rev_date != NULL)
    *rev_date = 20011008;
  return DT_RC_GOOD;
}

static
dtrev_f *revs[] = {
  tabptz_rev,
  dtptz_ipc_rev,
  dtptz_pela_rev,
  dtipc_rocket_rev,
  NULL
};

/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
tabptz_server(
  dtparam_t *param)
{
  DT_F("tabptz_server");
  dt_ctl_t *ctl = &param->ctl;
  dtptz_t _ptz, *ptz = &_ptz;
  dtipc_t _ipc, *ipc = &_ipc;
  char cmd[256];
  int i, n;
  char *p;
  dt_rc_e rc;

  ptz->initialized = 0;
  ipc->type = DTIPC_TYPE_NONE;

  DT_C(dtptz_init,(ctl, ptz,			/* open ptz device 					*/
    param->output, 0));
  
  DT_C(dtipc_register,(ctl, ipc,		/* register as a server 			*/
    param->connect, 0, 0));

  dt_dbg(ctl, F, DT_DBG_MASK_USR1,
    "ready for ptz commands");

  while(1)
  {
	int is_request;
	DT_C(dt_yield,(ctl, "waiting for connection"));

	DT_C(dtipc_poll,(ctl,				/* wait for incoming connection 	*/
      ipc,
      DTIPC_POLL_ACCEPT,
      param->omega, 
      "connection from client",
      &is_request));
	
	if (!is_request)					/* nobody requesting anything? 		*/
	  continue;

	DT_C(dtipc_wait,(ctl, ipc));		/* wait for client to connect 		*/

	while(1)
	{
	  int l;

	  DT_C(dt_yield,(ctl, 
        "waiting for command"));
	  
	  DT_C(dtipc_poll,(ctl,				/* wait for incoming command		*/
        ipc,
        DTIPC_POLL_READ,
        param->omega, 
        "command from client",
        &is_request));
	  if (!is_request)					/* no command yet? 					*/
		continue;

	  DT_C(dtipc_read_line2,(ctl, ipc,	/* read command from ipc 			*/
		param->epsilon, 
        cmd, sizeof(cmd)));

	  l = strlen(cmd);
	  while (l > 0 &&
			 cmd[l-1] == LF ||
			 cmd[l-1] == CR)
		cmd[--l] = '\0';				/* nuke the crlf's 					*/
	  
	  if (l == 0)
	  {
		dt_dbg(ctl, F, DT_DBG_MASK_COMMAND,
           "connection severed", cmd);
		break;
	  }

	  dt_dbg(ctl, F, DT_DBG_MASK_COMMAND,
         "%s", cmd);

	  DT_C(dtstr_tokenize,(ctl, cmd,	/* break up the line 				*/
        ';', &n));
	  p = cmd;
	  for (i=0; i<n; i++)				/* for each command on the line 	*/
	  {
		int isnumber;
		long value;
		DT_C(dtstr_to_long2,(ctl, p,
          &value, &isnumber));
		if (isnumber)					/* command is just a number? 		*/
		  DT_C(dtos_delay,(ctl,			/* delay milliseconds 				*/
            value))
  	    else							/* not a number? 					*/
		  DT_C(dtptz_command,(ctl, ptz,	/* send command to ptz 				*/
            p));
        p += strlen(p) + 1;
	  }
	}

	DT_C(dtipc_done,(ctl, ipc));		/* close data side 					*/
  }
  
cleanup:
  if (ipc->type != DTIPC_TYPE_NONE)
    DT_I(dtipc_unregister,(ctl, ipc));	/* release resources				*/

  if (ptz->initialized)
	DT_I(dtptz_uninit,(ctl, ptz));
  
  return rc;
}

/*..........................................................................*/

dt_rc_e 
DTCONFIG_API1
tabptz_arg_check(						/* check args and params			*/
  dt_ctl_t *ctl,
  void *app,
  dtparam_t *param,
  dt_enter_arg_t *arg,
  char *message,
  const char **start)
{
  DT_F("tabptz_arg_check");

  if (param->op != NULL &&				/* op is given? 					*/
	  !strcmp(param->op, "rev"))		/* user just wants revision? 		*/
	return DT_RC_GOOD;

  if (param->connect == NULL ||
	  strlen(param->connect) == 0)
  {
	dtstr_printf(ctl, 
      message, DT_ENTER_MSG_MAXLEN,
      "Please enter something"
      " for \"%s\" (-connect)",
      param->connect_desc);
    *start = param->connect_desc;
	return DT_RC_GOOD;
  }
  
  if (param->config == NULL ||
	  strlen(param->config) == 0)
  {
	dtstr_printf(ctl, 
      message, DT_ENTER_MSG_MAXLEN,
      "Please enter something"
      " for \"%s\"",
      param->config_desc);
    *start = param->config_desc;
	return DT_RC_GOOD;
  }
  
  if (param->output == NULL ||
	  strlen(param->output) == 0)
  {
	dtstr_printf(ctl, 
      message, DT_ENTER_MSG_MAXLEN,
      "Please enter something"
      " for \"%s\" (-output)",
      param->output_desc);
    *start = param->output_desc;
	return DT_RC_GOOD;
  }
  
  return DT_RC_GOOD;
}

/*..........................................................................
| NAME
| main program
| 
| END
 *..........................................................................*/
  
DTMAIN1(tabptz)
{
  char F[32];
  dt_ctl_t *ctl = &param->ctl;
  dt_enter_arg_t arg[1];
  dtlicense_t license;
  long rep;
  dt_rc_e rc = DT_RC_GOOD;

  DTMAIN_NAME(F);						/* get program name from argv[0]	*/

  license.initialized = 0;

  {
	int pid;
	ctl->progname = argv[0];			/* make sure our name is correct 	*/
	DT_Q(dtos_getpid,(ctl, &pid));
  }

  DT_GI(dt_heapmon_init,(ctl));			/* init memory checking			    */

  DT_GI(dtparam_init,(param));			/* init param structure             */

  DT_GI(dtparam_set,(param,
    DTPARAM_FLAG_PRIMARY |
    DTPARAM_FLAG_LENGTHY,
    &param->connect,					/* listen address 					*/
      "IPC spec", NULL, 
      "rocket,0:5791",
    &param->config, 
      "Config directory", NULL, 
      NULL,
    &param->output, 
      "Ptz spec", NULL, 
      "",
    &param->op, 
      "Operation", NULL, 
      "server",
    &param->omega, 
      "Poll time", NULL, 
      "1000",
    &param->epsilon, 
      "IPC timeout", NULL, 
      "5000",
    NULL));

  arg[0].desc = NULL;                   /* no positional arguments          */
 
  DT_C(dt_interactive_parse,(			/* parse command line interactively	*/
    argc, argv, F, usage, param,
    arg, tabptz_arg_check, NULL));
										/* ................................ */
  ctl->dbg = tabrobot_dbg;				/* special logging needs 			*/
  ctl->err = tabrobot_err;
  ctl->say = tabrobot_say;

  if (!strcmp(param->op, "rev") ||		/* op is to say rev?				*/
	  DT_DBG(ctl, DT_DBG_MASK_USR1))
  {
	DT_C(dtrev_say,(ctl, revs)); 
  }

  if (!strcmp(param->op, "server"))		/* op is to run normally?			*/
  {
    DT_C(dtlicense_request,(ctl,		/* request a tableau license 		*/
      &license, param->config, 
      ctl->progname));

	DT_C(tabptz_server,(param));
  }
										/* ................................ */
cleanup:
  DT_I(dtlicense_release,(ctl,
    &license));

  DT_I(dt_leave,(param));				/* clean up before leaving			*/
    
  ctl->dbg = NULL;
  ctl->err = NULL;
  ctl->say = NULL;

  {
	int check_fail = 0;
    DT_I(dt_heapmon_uninit,(ctl,		/* check all memory is freed        */
      &check_fail));
    if (rc == DT_RC_GOOD &&
		check_fail)						/* any failed assertions?           */
      rc = DT_RC_BAD;
  }

  dt_dbg(ctl, F, 0, NULL);				/* close files 						*/
  dt_say(ctl, F, NULL);
  dt_err(ctl, F, NULL);
  
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
