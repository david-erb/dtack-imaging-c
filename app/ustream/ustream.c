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
| ustream - streaming transfer
| 
|.SYNOPSIS
|:ustream -op [server | command] [params]
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

#include <ustream.h>
DT_RCSID("app/ustream $RCSfile: ustream.c,v $ $Revision: 1.8 $");
#include <dtack/str.h>
#include <dtack/main.h>
#include <dtack/rev.h>

#include <ustream.use>

static const char *ops[] = {			/* values for -op parameter 		*/
  "server",
  "rev",
  NULL
};

ustream_op_f ustream_op_rev;

static ustream_op_f *op_func[] = {		/* functions for -op parameter 		*/
  ustream_op_server,
  ustream_op_rev,
  NULL
};

typedef struct {						/* application structure			*/
  int op;
} ustream_app_t;

static
dtrev_f *revs[] = {
  ustream_rev,
  ustate_rev,
  dtdg_rev,
  NULL
};

#define dt_interactive_parse			/* don't use dialog 				*/\
  dt_enter_parse

#define PARAM_CHECK(WHICH, WANTED) \
{ \
  DT_Q(ustream_param,(param, \
     param->WHICH, \
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
ustream_op_rev(
  dtparam_t *param)
{
  DT_F("ustream_op_rev");
  dt_ctl_t *ctl = &param->ctl;
  DT_Q(dtrev_say,(ctl, revs)); 
  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/

static
dt_rc_e 
ustream_param(
  dtparam_t *param,
  const char *value,
  const char *desc,
  dtparam_flag_t flags,
  int wanted,
  char *message,
  int mmax)
{
  DT_F("ustream_param");
  dt_ctl_t *ctl = &param->ctl;
  if (flags & DTPARAM_FLAG_VALUE)
  {
  	if (!wanted)
  	  DT_Q(dtstr_printf,(ctl, message, mmax,
        "%s is not allowed for the %s operation.",
        desc, param->op));
  }
  else
  	if (wanted)
  	  DT_Q(dtstr_printf,(ctl, message, mmax,
        "%s is required for the %s operation.",
        desc, param->op));

  return DT_RC_GOOD;
}

/*..........................................................................*/

dt_rc_e 
DTCONFIG_API1
ustream_arg_check(						/* check args and params			*/
  dt_ctl_t *ctl,
  void *vapp,							/* app structure 					*/
  dtparam_t *param,
  dt_enter_arg_t *arg,
  char *message,
  const char **start)
{
  const char *F = NULL;
  ustream_app_t *app = (ustream_app_t *)vapp;

  DT_Q(dtstr_choice2,(ctl, ops,
    param->op, &app->op,
    message, DT_ENTER_MSG_MAXLEN));
  if (message[0])
  {
    *start = param->op_desc;
	return DT_RC_GOOD;
  }

  if (!strcmp(ops[app->op], "server"))	/* be the ipc server				*/
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
  
DTMAIN1(ustream)
{
  DT_F("ustream");
  dt_ctl_t *ctl = &param->ctl;
  dt_enter_arg_t arg[1];
  ustream_app_t ustream_app;
  long rep;
  dt_rc_e rc = DT_RC_GOOD;

  DT_C(dt_heapmon_init,(ctl));			/* init memory checking			    */

  DT_C(dtparam_init,(param));			/* init param structure             */

  DT_C(dtparam_set,(param,
    DTPARAM_FLAG_PRIMARY |
    DTPARAM_FLAG_LENGTHY,
    &param->op,							/* program operation				*/
      "Operation", NULL, 
      "server",
    &param->connect,					/* ipc spec							*/
      "Ipc spec", NULL, 
      NULL,
    &param->ident,						/* ident string						*/
      "Server ident", NULL, 
      NULL,
    &param->path,						/* istream preference				*/
      "Istream preference", NULL, 
      NULL,
    &param->config,						/* config file						*/
      "Config file", NULL, 
      NULL,
    NULL));

  DT_C(dtparam_set,(param,
    DTPARAM_FLAG_SECONDARY |
    DTPARAM_FLAG_LENGTHY,
    &param->reps, 
      "Repeat", NULL, 
      "1",
    &param->dt, 
      "Poll period ms", NULL, 
      "100",
    NULL));

  arg[0].desc = NULL;                   /* no positional arguments          */

  DT_C(dt_enter_parse,(					/* parse command line interactively	*/
    argc, argv, F, usage, param,
    arg, ustream_arg_check, &ustream_app));

  ctl->dbg = ustream_dbg;				/* special logging needs 			*/
  ctl->err = ustream_err;
  ctl->say = ustream_say;

										/* ................................ */
  for (rep=0; 
	   param->reps == 0 ||
	   rep<param->reps; 
	   rep++)
  {
	DT_C((*op_func[ustream_app.op]),
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
