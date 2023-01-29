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
| ustate - streaming transfer
| 
|.SYNOPSIS
|:ustate -op [server | command] [params]
|
|.SUMMARY
| Generic streaming client/server.
| 
|.PARAMETERS
| :-op:			operation
| :-connect:	network address to adopt or seek
| :-input:		input
| :-output:		data fifo
| :-reps:		number of times to repeat (default 1)
|dtinclude(params1.dx)
|
|.DESCRIPTION
| The program does one operation per invocation.
| The operations are:
| 
| END
 *..........................................................................*/

#include <dtack/base.h>
DT_RCSID("app/ustate $RCSfile: ustate.c,v $ $Revision: 1.8 $");
#include <dtack/main.h>
#include <dtack/list.h>
#include <ustate.h>
#include <dtack/os.h>
#include <dtack/str.h>

#include <ustate.use>

typedef struct {
  const char *ident;
  const char *program;
  const char *attribute;
  const char *deflt;
  int op;
} app_t;

static const char *ops[] = {
  "search",
  NULL
};

/* ------------------------------------------------------------------------
 * ------------------------------------------------------------------------ */

dt_rc_e 
DTCONFIG_API1
ustate_op_search(
  dtparam_t *param,
  app_t *app)
{
  DT_F("ustate_transact");
  dt_ctl_t *ctl = &param->ctl;
  ustate_t ustate;
  const char *value;
  dt_rc_e rc;
  
  DT_C(ustate_init_config,(ctl, 
    &ustate, param->config));
  DT_C(ustate_search,(ctl, &ustate, 
    app->ident, 
    app->program, 
    app->attribute,
    app->deflt, 
    &value));
  printf("%s\n", value);
cleanup:
  DT_I(ustate_uninit,(ctl, & ustate));
  return rc;
}

/*..........................................................................*/

dt_rc_e 
DTCONFIG_API1
ustate_arg_check(						/* check args and params			*/
  dt_ctl_t *ctl,
  void *vapp,							/* app structure 					*/
  dtparam_t *param,
  dt_enter_arg_t *arg,
  char *message,
  const char **start)
{
  const char *F = NULL;
  app_t *app = (app_t *)vapp;
  int n, i;
  if (!(param->config_flag & 
		DTPARAM_FLAG_VALUE))
  {
	dtstr_printf(ctl, 
      message, DT_ENTER_MSG_MAXLEN,
      "Sorry, the -config param is required.");
	return DT_RC_GOOD;
  }

  DT_Q(dtstr_choice2,(ctl, ops,
    param->op, &app->op,
    message, DT_ENTER_MSG_MAXLEN));
  if (message[0] != '\0')
    return DT_RC_GOOD;

  if (!strcmp(ops[app->op], "search"))
  {
    n = 4;
	arg[0].desc = "ident";
	arg[1].desc = "program";
	arg[2].desc = "attribute";
	arg[3].desc = "defaultx";
  }
  else
    n = 0;

  for (i=0; i<n; i++)
  {
	if (arg[i].val[0] == '\0')			/* don't have anything?				*/
	{
	  dtstr_printf(ctl, 
        message, DT_ENTER_MSG_MAXLEN,
        "Please enter something"
        " for \"%s\". (%d)",
        arg[i].desc, i);
	  return DT_RC_GOOD;
	}
  }

  if (arg[i].desc != NULL)
  {
	dtstr_printf(ctl, 
      message, DT_ENTER_MSG_MAXLEN,
      "Sorry, too many positional arguments.");
	return DT_RC_GOOD;
  }
  
  if (!strcmp(ops[app->op], "search"))
  {
	app->ident = arg[0].val;
	app->program = arg[1].val;
	app->attribute = arg[2].val;
	app->deflt = arg[3].val;
  }

  
  return DT_RC_GOOD;
}

/*..........................................................................
| NAME
| main program
| 
| END
 *..........................................................................*/
  
DTMAIN1(ustate)
{
  DT_F("ustate");
  dt_ctl_t *ctl = &param->ctl;
  dt_enter_arg_t arg[5];
# define ARG_MAXLEN 128
  char arg0[ARG_MAXLEN];
  char arg1[ARG_MAXLEN];
  char arg2[ARG_MAXLEN];
  char arg3[ARG_MAXLEN];
  app_t app;
  long rep;
  dt_rc_e rc = DT_RC_GOOD;

  DT_C(dt_heapmon_init,(ctl));			/* init memory checking			    */

  DT_C(dtparam_init,(param));			/* init param structure             */

  DT_C(dtparam_set,(param,
    DTPARAM_FLAG_PRIMARY |
    DTPARAM_FLAG_LENGTHY,
    &param->config,						/* config filename					*/
      "Config filename", NULL, 
      NULL,
    &param->op,							/* operation						*/
      "Operation", NULL, 
      "search",
    NULL));

  DT_GI(dt_enter_arg,(ctl,				/* set up positionals			    */
    arg,
    "Arg0", NULL, 
      arg0, sizeof(arg0),
    "Arg1", NULL, 
      arg1, sizeof(arg1),
    "Arg2", NULL, 
      arg2, sizeof(arg2),
    "Arg3", NULL, 
      arg3, sizeof(arg3),
    NULL));

  DT_C(dt_enter_parse,(					/* parse command line				*/
    argc, argv, F, usage, param,
    arg, ustate_arg_check, 
    &app));
										/* ................................ */
  DT_C(ustate_op_search,(param, &app));
										/* ................................ */
cleanup:
  DT_I(dt_leave,(param));				/* clean up before leaving			*/
  
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
