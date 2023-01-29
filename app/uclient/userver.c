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
| userver - streaming transfer
| 
|.SYNOPSIS
|:userver -op [server | command] [params]
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
DT_RCSID("app/userver $RCSfile: userver.c,v $ $Revision: 1.8 $");
#include <dtack/main.h>
#include <dtack/ipc.h>
#include <dtack/list.h>
#include <dtack/str.h>
#include <dtack/os.h>

#include <userver.use>

typedef struct {						/* application structure			*/
  int op;
} userver_app_t;

#define CR 0x0d
#define LF 0x0a

#define PARAM_CHECK(WHICH, WANTED) \
{ \
  DT_Q(userver_param,(param, \
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
userver_transact(
  dtparam_t *param)
{
  DT_F("userver_transact");
  dt_ctl_t *ctl = &param->ctl;
  dtipc_t _ipc, *ipc = &_ipc;
  long mask = DT_DBG_MASK_REPLY;
  dt_rc_e rc;

  DT_C(dtipc_register,(ctl, ipc,
    param->connect, 0, 0));
  DT_C(dtipc_wait,(ctl, ipc));			/* wait for connect					*/

  while(1)
  {
	char line[256];
	int l;

    DT_C(dtipc_read_line,(ctl, ipc,		/* read the request line 			*/
      line, sizeof(line)));

	l = strlen(line);

	if (l == 0)							/* server has ended conversation? 	*/
	  break;

	while (l > 0 &&
		   line[l-1] == LF ||
		   line[l-1] == CR)
	  line[--l] = '\0';					/* nuke the crlf's 					*/

	dt_dbg(ctl, F, mask, "%s", line);
  }

cleanup:
  DT_I(dtipc_done,(ctl, ipc));			/* server finishes transaction	    */
  DT_I(dtipc_unregister,(ctl, ipc));

  return rc;
}

/*..........................................................................
 *..........................................................................*/

static
dt_rc_e 
userver_param(
  dtparam_t *param,
  const char *value,
  const char *abbrev,
  const char *desc,
  dtparam_flag_t flags,
  int wanted,
  char *message,
  int mmax)
{
  DT_F("userver_param");
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
userver_arg_check(						/* check args and params			*/
  dt_ctl_t *ctl,
  void *vapp,							/* app structure 					*/
  dtparam_t *param,
  dt_enter_arg_t *arg,
  char *message,
  const char **start)
{
  const char *F = NULL;
  userver_app_t *app = (userver_app_t *)vapp;

  PARAM_CHECK(connect, 1);

  return DT_RC_GOOD;
}

/*..........................................................................
| NAME
| main program
| 
| END
 *..........................................................................*/
  
DTMAIN1(userver)
{
  DT_F("userver");
  dt_ctl_t *ctl = &param->ctl;
  dt_enter_arg_t arg[1];
  userver_app_t userver_app;
  long rep;
  dt_rc_e rc = DT_RC_GOOD;

  DT_C(dt_heapmon_init,(ctl));			/* init memory checking			    */

  DT_C(dtparam_init,(param));			/* init param structure             */

  DT_C(dtparam_set,(param,
    DTPARAM_FLAG_PRIMARY |
    DTPARAM_FLAG_LENGTHY,
    &param->connect,					/* ipc spec							*/
      "Ipc spec", NULL, 
      NULL,
    NULL));

  DT_C(dtparam_set,(param,
    DTPARAM_FLAG_SECONDARY,
    &param->dt,							/* reply timeout					*/
      "Reply timeout", NULL, 
      "2000",
    &param->reps,						/* repeat loops						*/
      NULL, NULL, 
      "1",
    NULL));

  arg[0].desc = NULL;                   /* no positional arguments          */

  DT_C(dt_enter_parse,(					/* parse command line				*/
    argc, argv, F, usage, param,
    arg, userver_arg_check, 
    &userver_app));
										/* ................................ */
  for (rep=0; rep<param->reps; rep++)
  {
	DT_C(userver_transact,(param));
  }
  
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
