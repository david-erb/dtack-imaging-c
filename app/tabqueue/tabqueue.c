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
| tabqueue - client/server IP address mapper
| 
|.SYNOPSIS
|:tabqueue [params] -connect host:port
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
|:  tabqueue -connect 156.46.122.16:5790
| 
| Client:
|:  tabqueue -connect 156.46.122.16:5300 -name myname
|
|dtinclude(params2.dx)
|
| END
 *..........................................................................*/

#include <tabqueue.h>
DT_RCSID("tabqueue $RCSfile: tabqueue.c,v $ $Revision: 1.8 $");

#include <dtack/main.h>

#include <tabqueue.use>

#define dt_interactive_parse			/* don't use dialog 				*/\
  dt_enter_parse

/*..........................................................................
 *..........................................................................*/

static
void
tabqueue_stamp(							/* output debug message				*/
  dt_ctl_t *ctl,						/* environment control 				*/
  const char *caller,					/* caller's name 					*/
  char *s,
  int lmax)
{
  int pid;
  time_t clock = time(NULL);			/* current time 					*/
  struct tm *t = localtime(&clock);		/* format into fields 				*/
  strftime(s, lmax,						/* express as string 				*/
    "%Y%m%d.%H%M.%S", t);

  dtos_getpid(ctl, &pid);

  dtstr_printf(ctl,						/* tack on the caller name and pid	*/
    s+strlen(s), lmax-strlen(s),
    " [%5d]%s%s", pid, 
    caller? " ": "",
    caller? caller: "");
}

/*..........................................................................
 *..........................................................................*/

static
void
DTCONFIG_API1
tabqueue_dbg(							/* output debug message				*/
  dt_ctl_t *ctl,						/* environment control 				*/
  const char *caller,					/* caller's name 					*/
  long class_bits,						/* debug message class 				*/
  const char *format,					/* message format, like sprintf() 	*/
  va_list ap)							/* format substitutions 			*/
{
  char s[64];
  tabqueue_stamp(ctl, caller, 
    s, sizeof(s));
  ctl->dbg = NULL;						/* don't come back here 			*/
  dt_dbg_va(ctl, s, class_bits,			/* let dt_dbg_va() do all the work 	*/
    format, ap);
  ctl->dbg = tabqueue_dbg;				/* come back here next time 		*/
}

/* ........................................................................ *
 * ........................................................................ */

static
dt_rc_e
DTCONFIG_API1
tabqueue_err(							/* handle errors					*/
  dt_ctl_t *ctl,						/* environment control 				*/
  const char *caller,					/* caller's name 					*/
  const char *format,					/* message format, like sprintf() 	*/
  va_list ap)							/* format substitutions 			*/
{
  dt_rc_e rc;
  char s[64];
  tabqueue_stamp(ctl, caller, 
    s, sizeof(s));

  ctl->err = NULL;						/* don't come back here 			*/
  rc = dt_err_va(ctl, s,				/* log the error 					*/
    format, ap);
  ctl->err = tabqueue_err;				/* come back here next time 		*/
  return rc;
}

/*..........................................................................*
 *..........................................................................*/

static
void
DTCONFIG_API1
tabqueue_say(							/* output console message			*/
  dt_ctl_t *ctl,						/* environment control 				*/
  const char *caller,					/* caller's name 					*/
  const char *format,					/* message format, like sprintf() 	*/
  va_list ap)							/* format substitutions 			*/
{
  char s[64];
  tabqueue_stamp(ctl, caller, 
    s, sizeof(s));

  ctl->say = NULL;						/* don't come back here 			*/
  dt_say_va(ctl, s,
    format, ap);
  ctl->say = tabqueue_say;				/* come back here next time 		*/
}

/*..........................................................................*/

static
dt_rc_e 
DTCONFIG_API1
tabqueue_arg_check(						/* check args and params			*/
  dt_ctl_t *ctl,
  void *app,
  dtparam_t *param,
  dt_enter_arg_t *arg,
  char *message,
  const char **start)
{

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
  
  return DT_RC_GOOD;
}

/*..........................................................................
| NAME
| main program
| 
| END
 *..........................................................................*/
  
DTMAIN1(tabqueue)
{
  DT_F("tabqueue");
  dt_ctl_t *ctl = &param->ctl;
  dt_enter_arg_t arg[1];
  long rep;
  dt_rc_e rc = DT_RC_GOOD;

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
    NULL));

  DT_GI(dtparam_set,(param,
    DTPARAM_FLAG_SECONDARY |
    DTPARAM_FLAG_LENGTHY,
    &param->reps, 
      "Repeat op", NULL, 
      "0",
    &param->op, 
      "Operation", NULL, 
      "server",
    &param->n, 
      "Repeat send", NULL, 
      "1",
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
    argc, argv, "", usage, param,
    arg, tabqueue_arg_check, NULL));
										/* ................................ */
  ctl->dbg = tabqueue_dbg;				/* special logging needs 			*/
  ctl->err = tabqueue_err;
  ctl->say = tabqueue_say;

  for (rep=0; 
	   param->reps == 0 ||
	   rep<param->reps; 
	   rep++)
  {
	if (DT_ABBREV(param->op, "server"))	/* op is server? 					*/
	{
	  DT_C(tabqueue_server,(param));
	}
	else
	{
	  DT_C(tabqueue_client,(param,
        param->op));
	  if (param->reps == 0)
		param->reps = 1;
	}
  }	
										/* ................................ */
cleanup:
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
