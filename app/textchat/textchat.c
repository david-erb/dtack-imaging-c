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
| textchat - streaming transfer
| 
|.SYNOPSIS
|:textchat -op [server | command] [params]
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

#include <textchat.h>
DT_RCSID("app/textchat $RCSfile: textchat.c,v $ $Revision: 1.8 $");
#include <dtack/str.h>
#include <dtack/main.h>

#include <textchat.use>

static const char *ops[] = {			/* values for -op parameter 		*/
  "gui",
  NULL
};

static textchat_op_f *op_func[] = {		/* functions for -op parameter 		*/
  textchat_op_gui,
  NULL
};

typedef struct {						/* application structure			*/
  int op;
} textchat_app_t;

#define dt_interactive_parse			/* don't use dialog 				*/\
  dt_enter_parse

#define PARAM_CHECK(WHICH, WANTED) \
{ \
  DT_Q(textchat_param,(param, \
     param->WHICH, \
     #WHICH, \
     param->WHICH ## _desc, \
     param->WHICH ## _flag, \
     WANTED, message, DT_ENTER_MSG_MAXLEN)); \
   if (message[0]) \
   { \
	 *start = param->op_desc; \
	 return DT_RC_GOOD; \
   } \
 }

/*..........................................................................
 *..........................................................................*/

static
dt_rc_e 
textchat_param(
  dtparam_t *param,
  const char *value,
  const char *abbrev,
  const char *desc,
  dtparam_flag_t flags,
  int wanted,
  char *message,
  int mmax)
{
  DT_F("textchat_param");
  dt_ctl_t *ctl = &param->ctl;
  if (flags & DTPARAM_FLAG_VALUE ||
	  flags & DTPARAM_FLAG_DEFAULT)
  {
  	if (!wanted)
  	  DT_Q(dtstr_printf,(ctl, message, mmax,
        "%s (-%s) is not allowed.",
        desc, abbrev));
  }
  else
  	if (wanted)
  	  DT_Q(dtstr_printf,(ctl, message, mmax,
        "%s (-%s) is required.",
        desc, abbrev));

  return DT_RC_GOOD;
}

/*..........................................................................*/

dt_rc_e 
DTCONFIG_API1
textchat_arg_check(						/* check args and params			*/
  dt_ctl_t *ctl,
  void *vapp,							/* app structure 					*/
  dtparam_t *param,
  dt_enter_arg_t *arg,
  char *message,
  const char **start)
{
  const char *F = NULL;
  textchat_app_t *app = (textchat_app_t *)vapp;

  DT_Q(dtstr_choice2,(ctl, ops,
    param->op, &app->op,
    message, DT_ENTER_MSG_MAXLEN));
  if (message[0])
  {
    *start = param->op_desc;
	return DT_RC_GOOD;
  }

  if (!strcmp(ops[app->op], "dialog"))	/* act normal						*/
  {
	PARAM_CHECK(ident, 1);
  }

  return DT_RC_GOOD;
}

/*..........................................................................
| NAME
| main program
| 
| END
 *..........................................................................*/
  
DTMAIN1(textchat)
{
  DT_F("textchat");
  dt_ctl_t *ctl = &param->ctl;
  dt_enter_arg_t arg[1];
  textchat_app_t textchat_app;
  long rep;
  dt_rc_e rc = DT_RC_GOOD;

  DT_C(dt_heapmon_init,(ctl));			/* init memory checking			    */

  DT_C(dtparam_init,(param));			/* init param structure             */

  DT_C(dtparam_set,(param,
    DTPARAM_FLAG_PRIMARY |
    DTPARAM_FLAG_LENGTHY,
    &param->op,							/* program operation				*/
      "Operation", NULL, 
      NULL,
    &param->connect,					/* ipc spec							*/
      "Ipc spec", NULL, 
      NULL,
    &param->ident,						/* audio ustream ident				*/
      "Audio ident", NULL, 
      NULL,
    &param->config,						/* config file						*/
      "Config file", NULL, 
      NULL,
    &param->dialog,						/* dialog spec						*/
      "Data dialog", NULL, 
      "text,curses",
    &param->keyboard,					/* keyboard spec					*/
      "Data keyboard", NULL, 
      "",
    NULL));

  DT_C(dtparam_set,(param,
    DTPARAM_FLAG_SECONDARY |
    DTPARAM_FLAG_LENGTHY,
    &param->reps, 
      "Repeat", NULL, 
      "1",
    &param->dt, 
      "Poll period ms", NULL, 
      "200",
    &param->x, 
      "Dialog column", NULL, 
      "0",
    &param->y, 
      "Dialog row", NULL, 
      "0",
    NULL));

  arg[0].desc = NULL;                   /* no positional arguments          */

  DT_C(dt_enter_parse,(					/* parse command line interactively	*/
    argc, argv, F, usage, param,
    arg, textchat_arg_check, 
    &textchat_app));

  ctl->dbg = textchat_dbg;				/* special logging needs 			*/
  ctl->err = textchat_err;
  ctl->say = textchat_say;

										/* ................................ */
  for (rep=0; 
	   param->reps == 0 ||
	   rep<param->reps; 
	   rep++)
  {
	DT_C((*op_func[textchat_app.op]),
      (param));
  }	
										/* ................................ */
cleanup:
  DT_I(dt_leave,(param));				/* clean up before leaving			*/
    
  ctl->dbg_string = NULL;
  ctl->err_string = NULL;
  ctl->say_string = NULL;
  ctl->dbg = NULL;
  ctl->err = NULL;
  ctl->say = NULL;

  if (rc == DT_RC_STOP)					/* we were told to stop?			*/
    rc = DT_RC_GOOD;
  
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
