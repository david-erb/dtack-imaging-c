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
| umsg - msg queue send
| 
|.SYNOPSIS
|:umsg -connect pid -input "command"
|
|.SUMMARY
| Send command on message queue.
| 
|.PARAMETERS
| :-connect:	pid of recipient
| :-input:		command to send
|dtinclude(params1.dx)
|
|.DESCRIPTION
| This program sends a command to a recipient process using a message queue.
| The recipient must have created a message queue and be monitoring it.
| 
| END
 *..........................................................................*/

#include <dtack/base.h>
DT_RCSID("app/umsg $RCSfile: umsg.c,v $ $Revision: 1.8 $");
#include <dtack/main.h>
#include <dtack/msg.h>
#include <dtack/str.h>
#include <dtack/os.h>
#include <dtack/rev.h>

#include <umsg.use>

static
dtrev_f *revs[] = {
  dtmsg_rev,
  NULL
};

typedef struct {						/* application structure			*/
  int op;
} umsg_app_t;

#define PARAM_CHECK(WHICH, WANTED) \
{ \
  DT_Q(umsg_param,(param, \
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
umsg_transact(
  dtparam_t *param)
{
  DT_F("umsg_transact");
  dt_ctl_t *ctl = &param->ctl;
  long pid;
  dtmsg_t msg;
  dt_rc_e rc;

  DT_MEMSET(&msg, 0, sizeof(msg));
  DT_C(dtstr_to_long,(ctl, 
    param->connect, &pid));
  DT_C(dtmsg_access,(ctl, &msg, pid));

  DT_C(dtmsg_post,(ctl, &msg,
    param->input, 
    strlen(param->input)+1));
  
cleanup:
  DT_I(dtmsg_release,(ctl, &msg));

  return rc;
}

/*..........................................................................
 *..........................................................................*/

static
dt_rc_e 
umsg_param(
  dtparam_t *param,
  const char *value,
  const char *abbrev,
  const char *desc,
  dtparam_flag_t flags,
  int wanted,
  char *message,
  int mmax)
{
  DT_F("umsg_param");
  dt_ctl_t *ctl = &param->ctl;
  if (flags & DTPARAM_FLAG_VALUE ||
	  flags & DTPARAM_FLAG_DEFAULT)
  {
  	if (!wanted)
  	  DT_Q(dtstr_printf,(ctl, message, mmax,
        "%s (%s) is not allowed.",
        desc, abbrev));
  }
  else
  	if (wanted)
  	  DT_Q(dtstr_printf,(ctl, message, mmax,
        "%s (%s) is required.",
        desc, abbrev));

  return DT_RC_GOOD;
}

/*..........................................................................*/

dt_rc_e 
DTCONFIG_API1
umsg_arg_check(						/* check args and params			*/
  dt_ctl_t *ctl,
  void *vapp,							/* app structure 					*/
  dtparam_t *param,
  dt_enter_arg_t *arg,
  char *message,
  const char **start)
{
  const char *F = NULL;
  umsg_app_t *app = (umsg_app_t *)vapp;
  if (param->op &&
	  strcmp(param->op, "rev"))
  {
    PARAM_CHECK(input, 1);
    PARAM_CHECK(connect, 1);
  }

  return DT_RC_GOOD;
}

/*..........................................................................
| NAME
| main program
| 
| END
 *..........................................................................*/
  
DTMAIN1(umsg)
{
  DT_F("umsg");
  dt_ctl_t *ctl = &param->ctl;
  dt_enter_arg_t arg[1];
  umsg_app_t umsg_app;
  long rep;
  dtos_time_t mark1, mark2;
  dt_rc_e rc = DT_RC_GOOD;

  DT_C(dt_heapmon_init,(ctl));			/* init memory checking			    */

  DT_C(dtparam_init,(param));			/* init param structure             */

  DT_C(dtparam_set,(param,
    DTPARAM_FLAG_PRIMARY |
    DTPARAM_FLAG_LENGTHY,
    &param->connect,					/* ipc spec							*/
      "Msg queue id", NULL, 
      NULL,
    &param->input,						/* input command					*/
      "Command", NULL, 
      NULL,
    NULL));

  DT_C(dtparam_set,(param,
    DTPARAM_FLAG_SECONDARY,
    &param->op,							/* special op						*/
      NULL, NULL, 
      "default",
    &param->reps,						/* repeat loops						*/
      NULL, NULL, 
      "1",
    NULL));

  arg[0].desc = NULL;                   /* no positional arguments          */

  DT_C(dt_enter_parse,(					/* parse command line				*/
    argc, argv, F, usage, param,
    arg, umsg_arg_check, 
    &umsg_app));
										/* ................................ */
  if (!strcmp("rev", param->op))		/* op is to say rev?				*/
  {
	DT_C(dtrev_say,(ctl, revs));
	goto cleanup;
  }

  dtrev_dbg(ctl, DT_DBG_MASK_USR1, revs);

  DT_C(dtos_time,(ctl, &mark1));
  for (rep=0; rep<param->reps; rep++)
  {
	DT_C(umsg_transact,(param));
  }
  DT_C(dtos_time,(ctl, &mark2));
  
  dtos_time_report(ctl,
    &mark1, &mark2, param->input);
  
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

