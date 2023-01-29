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
| tabrobot - client/server IP address mapper
| 
|.SYNOPSIS
|:tabrobot [params] -connect host:port
|
|.SUMMARY
| Handle specialized PTZ socket commands.
| 
|.PARAMETERS
| :-connect:	connection spec (required for either client or server)
|dtinclude(params1.dx)
|
|.EXAMPLES
| Server:
|:  tabrobot -connect 156.46.122.16:5790
| 
| Client:
|:  tabrobot -connect 156.46.122.16:5300 -name myname
|
|dtinclude(params2.dx)
|
| END
 *..........................................................................*/

/*..........................................................................
  CHANGES
  20000720 DE add delay scale factor param->b
  20000223 DE added param->epsilon
  19990517 DE explicitly non-console app
 *..........................................................................*/

#include <tabrobot.h>
DT_RCSID("tabrobot $RCSfile: tabrobot.c,v $ $Revision: 1.8 $");

#undef DTPRJ_CONSOLE
#define DTPRJ_CONSOLE DT_0				/* make this a non-console app 		*/

#include <dtack/main.h>
#include <dtack/rev.h>
#include <dtptz.h>
#include <dtack/licens.h>

#include <tabrobot.use>

#define dt_interactive_parse			/* don't use dialog 				*/\
  dt_enter_parse

/*..........................................................................
 *..........................................................................*/

static
dtrev_f *revs[] = {
  tabrobot_rev,
  dtptz_ipc_rev,
  dtipc_rocket_rev,
  NULL
};

/*..........................................................................*/

dt_rc_e 
DTCONFIG_API1
tabrobot_arg_check(						/* check args and params			*/
  dt_ctl_t *ctl,
  void *app,
  dtparam_t *param,
  dt_enter_arg_t *arg,
  char *message,
  const char **start)
{
  DT_F("tabrobot_arg_check");

  if (param->op != NULL &&				/* op is given? 					*/
	  !strcmp(param->op, "rev"))		/* user just wants revision? 		*/
	return DT_RC_GOOD;

  if (param->connect == NULL ||
	  strlen(param->connect) == 0)
  {
	dtstr_printf(ctl, 
      message, DT_ENTER_MSG_MAXLEN,
      "Please enter something"
      " for \"%s\"",
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
  
  return DT_RC_GOOD;
}

/*..........................................................................
| NAME
| main program
| 
| END
 *..........................................................................*/
  
DTMAIN1(tabrobot)
{
  char F[32];
  dt_ctl_t *ctl = &param->ctl;
  dt_enter_arg_t arg[1];
  dtlicense_t license;
  long rep;
  dt_rc_e rc = DT_RC_GOOD;

  license.initialized = 0;

  DTMAIN_NAME(F);						/* get program name from argv[0]	*/
  
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
      "rocket,0:5790",
    &param->output, 
      "Ptz spec", NULL, 
      "",
    &param->config, 
      "Config directory", NULL, 
      NULL,
    &param->precision, 
      "Presets file", NULL, 
      NULL,
    &param->max, 
      "Max connect ms", NULL, 
      "180000",
    &param->min, 
      "Grace period ms", NULL, 
      "10000",
    &param->omega, 
      "Notify time ms", NULL, 
      "5000",
    &param->datadir, 
      "roam stopfile", NULL, 
      "roam.suspend",
    NULL));

  DT_GI(dtparam_set,(param,
    DTPARAM_FLAG_SECONDARY |
    DTPARAM_FLAG_LENGTHY,
    &param->reps, 
      "Repeat op", NULL, 
      "1",
    &param->op, 
      "Operation", NULL, 
      "server",
    &param->epsilon, 
      "IPC timeout", NULL, 
      "2000",
    &param->b, 
      "movement time scale", NULL, 
      "0.05",
    NULL));

  DT_Q(dtparam_set,(param,				/* hide params we always parse for	*/
    DTPARAM_FLAG_QUATERNARY,
    &param->dialog,   NULL, NULL, NULL,
    &param->keyboard, NULL, NULL, NULL,
    &param->dbgto,    NULL, NULL, NULL,
    &param->errto,    NULL, NULL, NULL,
    &param->sayto,    NULL, NULL, NULL,
    &param->mem,      NULL, NULL, NULL,
    &param->dbg,      NULL, NULL, NULL,
    &param->yield,    NULL, NULL, NULL,
    &param->win,      NULL, NULL, NULL,
    &param->help,     NULL, NULL, NULL,
    &param->HELP,     NULL, NULL, NULL,
    NULL));

  arg[0].desc = NULL;                   /* no positional arguments          */
 
  DT_C(dt_interactive_parse,(			/* parse command line interactively	*/
    argc, argv, F, usage, param,
    arg, tabrobot_arg_check, NULL));

										/* ................................ */
  ctl->dbg = tabrobot_dbg;				/* special logging needs 			*/
  ctl->err = tabrobot_err;
  ctl->say = tabrobot_say;

  if (!strcmp(param->op, "rev") ||		/* op is to say rev?				*/
	  DT_DBG(ctl, DT_DBG_MASK_USR1))
  {
	DT_C(dtrev_say,(ctl, revs));
  }

  if (!strcmp(param->op, "rev"))		/* op is to say rev?				*/
	goto cleanup;

  DT_C(dtlicense_request,(ctl,			/* request a tableau license 		*/
    &license, param->config, 
    ctl->progname));
  for (rep=0; 
	   param->reps == 0 ||
		 rep<param->reps; 
	   rep++)
  {
	if (DT_ABBREV(param->op, "server")) /* op is server? 					*/
	{
	  DT_C(tabrobot_server,(param));
	}
	else
	if (DT_ABBREV(param->op, "gui"))	/* op is gui? 						*/
	{
	  DT_C(tabrobot_gui_qnx,(param));
	}
	else
	{
	  DT_G(tabrobot_client,(param,		/* issue command to server 			*/
        param->op));
	  if (rep != param->reps-1)			/* not last rep? 					*/
		DT_C(dtos_delay,(ctl,			
          param->omega));				/* delay between reps 				*/
	}
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
