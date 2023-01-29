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
| netchat - interactive audio control
| 
|.SYNOPSIS
|:netchat -op [server | rev] [params]
|
|.SUMMARY
| Run an interactive netchat gui dialog.
| 
|.PARAMETERS
| :-op:			operation, either "server" or "rev"
| :-connect:	network address to adopt for remote control
| :-ident:		audio ident, usually "AUD$NODE"
| :-config:		ustate config file, usually "/CONFIG/ustate.cfg"
| :-dialog:		dialog type, usually "graphics"
| :-dt:			poll period in milliseconds
| :-x:			left edge of dialog on screen
| :-y:			top edge of dialog on screen
|dtinclude(params1.dx)
|
|.DESCRIPTION
| The program throws up an interactive dialog.
| It polls the ustream with the same ident to show audio status.
| 
| END
 *..........................................................................*/

#include <netchat.h>
DT_RCSID("app/netchat $RCSfile: netchat.c,v $ $Revision: 1.8 $");
#include <dtack/str.h>
#include <dtack/main.h>
#include <dtack/rev.h>

#include <netchat.use>

static const char *ops[] = {			/* values for -op parameter 		*/
  "gui",
  "rev",
  NULL
};

netchat_op_f netchat_op_rev;

static netchat_op_f *op_func[] = {		/* functions for -op parameter 		*/
  netchat_op_gui,
  netchat_op_rev,
  NULL
};

typedef struct {						/* application structure			*/
  int op;
} netchat_app_t;

static
dtrev_f *revs[] = {
  netchat_rev,
  ustate_rev,
  dtdg_rev,
  NULL
};

#define dt_interactive_parse			/* don't use dialog 				*/\
  dt_enter_parse

#define PARAM_CHECK(WHICH, WANTED) \
{ \
  DT_Q(netchat_param,(param, \
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

/* ------------------------------------------------------------------------
 * ------------------------------------------------------------------------ */

dt_rc_e 
DTCONFIG_API1
netchat_op_rev(
  dtparam_t *param)
{
  DT_F("netchat_op_rev");
  dt_ctl_t *ctl = &param->ctl;
  DT_Q(dtrev_say,(ctl, revs)); 
  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/

static
dt_rc_e 
netchat_param(
  dtparam_t *param,
  const char *value,
  const char *abbrev,
  const char *desc,
  dtparam_flag_t flags,
  int wanted,
  char *message,
  int mmax)
{
  DT_F("netchat_param");
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
netchat_arg_check(						/* check args and params			*/
  dt_ctl_t *ctl,
  void *vapp,							/* app structure 					*/
  dtparam_t *param,
  dt_enter_arg_t *arg,
  char *message,
  const char **start)
{
  const char *F = NULL;
  netchat_app_t *app = (netchat_app_t *)vapp;

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
  
DTMAIN1(netchat)
{
  DT_F("netchat");
  dt_ctl_t *ctl = &param->ctl;
  dt_enter_arg_t arg[1];
  netchat_app_t netchat_app;
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
    &param->ident,						/* audio netchat ident				*/
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
    arg, netchat_arg_check, 
    &netchat_app));

  ctl->dbg = netchat_dbg;				/* special logging needs 			*/
  ctl->err = netchat_err;
  ctl->say = netchat_say;

										/* ................................ */
  for (rep=0; 
	   param->reps == 0 ||
	   rep<param->reps; 
	   rep++)
  {
	DT_C((*op_func[netchat_app.op]),
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
