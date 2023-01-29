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
| TITLE tabgrab
| tabgrab program - run demo
| 
| USAGE
| tabgrab @configfile.arg  (use -help for more info)
| 
|.SYNOPSIS
|:tabgrab @configfile  (use -help for more info)
|
|.SUMMARY
| Grab frames and stuff them into file or shared memory.
| All args come from config file, please see the example.
| 
| END
 *..........................................................................*/



#include <tabgrab.h>
DT_RCSID("tabgrab $RCSfile: tabgrab.c,v $ $Revision: 1.8 $");
#undef DTPRJ_CONSOLE
#define DTPRJ_CONSOLE DT_0				/* make this a non-console app 		*/

#include <dtack/main.h>
#include <dtack/rev.h>

#include <tabgrab.use>

#define dt_interactive_parse			/* don't use dialog 				*/\
  dt_enter_parse

#define APPNAME_TRIM(S)                                                       \
  {                                                                           \
    char *p;                                                                  \
	p = strrchr((S), '.');				/* chop off filetype				*/\
	if (p != NULL)                                                            \
	  *p = '\0';                                                              \
	p = strrchr((S), '/');				/* trim off path before '/'			*/\
	if (p != NULL)                                                            \
	  memmove((char *)(S), p+1,                                               \
        strlen(p));                                                           \
	p = strrchr((S), '\\');				/* trim off path before '\'			*/\
	if (p != NULL)                                                            \
	  memmove((char *)(S), p+1,                                               \
        strlen(p));                                                           \
  } 

/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
tabgrab_rev(
  dt_ctl_t *ctl,
  const char **rev_string,
  int *rev_major,
  int *rev_minor,
  long *rev_date)
{
  if (rev_string != NULL)
    *rev_string = "tabgrab app";
  if (rev_major != NULL)
    *rev_major = 1;
  if (rev_minor != NULL)
    *rev_minor = 1;
  if (rev_date != NULL)
    *rev_date = 19980507;
  return DT_RC_GOOD;
}

static
dtrev_f *revs[] = {
  tabgrab_rev,
  dtfb_vfw_rev,
  dtfb_meteor_rev,
  dtfb_bt_rev,
  NULL
};

/*..........................................................................
 *..........................................................................*/

static
void
tabgrab_stamp(							/* output debug message				*/
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

void
DTCONFIG_API1
tabgrab_dbg(							/* output debug message				*/
  dt_ctl_t *ctl,						/* environment control 				*/
  const char *caller,					/* caller's name 					*/
  long class_bits,						/* debug message class 				*/
  const char *format,					/* message format, like sprintf() 	*/
  va_list ap)							/* format substitutions 			*/
{
  char s[64];
  tabgrab_stamp(ctl, caller, 
    s, sizeof(s));
  ctl->dbg = NULL;						/* don't come back here 			*/
  dt_dbg_va(ctl, s, class_bits,			/* let dt_dbg_va() do all the work 	*/
    format, ap);
  ctl->dbg = tabgrab_dbg;				/* come back here next time 		*/
}

/* ........................................................................ *
 * ........................................................................ */

dt_rc_e
DTCONFIG_API1
tabgrab_err(							/* handle errors					*/
  dt_ctl_t *ctl,						/* environment control 				*/
  const char *caller,					/* caller's name 					*/
  const char *format,					/* message format, like sprintf() 	*/
  va_list ap)							/* format substitutions 			*/
{
  dt_rc_e rc;
  char s[64];
  tabgrab_stamp(ctl, caller, 
    s, sizeof(s));

  ctl->err = NULL;						/* don't come back here 			*/
  rc = dt_err_va(ctl, s,				/* log the error 					*/
    format, ap);
  ctl->err = tabgrab_err;				/* come back here next time 		*/
  return rc;
}

/*..........................................................................*
 *..........................................................................*/

void
DTCONFIG_API1
tabgrab_say(							/* output console message			*/
  dt_ctl_t *ctl,						/* environment control 				*/
  const char *caller,					/* caller's name 					*/
  const char *format,					/* message format, like sprintf() 	*/
  va_list ap)							/* format substitutions 			*/
{
  char s[64];
  tabgrab_stamp(ctl, caller, 
    s, sizeof(s));

  ctl->say = NULL;						/* don't come back here 			*/
  dt_say_va(ctl, s,
    format, ap);
  ctl->say = tabgrab_say;				/* come back here next time 		*/
}

/*..........................................................................*/

dt_rc_e 
DTCONFIG_API1
tabgrab_arg_check(						/* check args and params			*/
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
  
DTMAIN1(tabgrab)
{
  char F[32];
  dt_ctl_t *ctl = &param->ctl;
  dt_enter_arg_t arg[1];
  char *appname = NULL;
  long rep;
  int op;
  dt_rc_e rc = DT_RC_GOOD;

  DTMAIN_NAME(F);						/* get program name from argv[0]	*/
  
  {
	int pid;
	ctl->progname = argv[0];			/* make sure our name is correct 	*/
	DT_Q(dtos_getpid,(ctl, &pid));
  }

  DT_C(dt_heapmon_init,(ctl));			/* init memory checking			    */

  DT_C(dtparam_init,(param));			/* init param structure             */

  DT_C(dtparam_set,(param,
    DTPARAM_FLAG_PRIMARY |
    DTPARAM_FLAG_LENGTHY,
    &param->connect,					/* listen address 					*/
      "IPC spec", NULL, 
      "rocket,0:5790",
    &param->input, 
      "Fb spec", NULL, 
      "",
    &param->output, 
      "Output file", NULL, 
      "",
    &param->dt, 
      "Command timeout", NULL, 
      "-1",
    &param->omega, 
      "Auto grab frequency", NULL, 
      "500",
    NULL));

  DT_C(dtparam_set,(param,
    DTPARAM_FLAG_SECONDARY |
    DTPARAM_FLAG_LENGTHY,
    &param->reps, 
      "Repeat", NULL, 
      "0",
    &param->op, 
      "Operation", NULL, 
      "server",
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

  DT_C(dtos_strdup,(ctl, argv[0], 
    &appname));
  APPNAME_TRIM(appname);
  
  DT_C(dt_interactive_parse,(			/* parse command line interactively	*/
    argc, argv, appname, usage, param,
    arg, tabgrab_arg_check, NULL));
										/* ................................ */
  ctl->dbg = tabgrab_dbg;				/* special logging needs 			*/
  ctl->err = tabgrab_err;
  ctl->say = tabgrab_say;

  for (rep=0; 
	   param->reps == 0 ||
	   rep<param->reps; 
	   rep++)
  {
	if (DT_ABBREV(param->op, "rev"))	/* op is to say rev and quit?		*/
	{
	  DT_C(dtrev_say,(ctl, revs));
	  param->reps = 1;
	}
	else
	if (DT_ABBREV(param->op, "server"))	/* op is server? 					*/
	{
	  DT_C(tabgrab_server,(param));
	}
	else
	{
	  DT_C(tabgrab_client,(param,
        param->op));
	  if (param->reps == 0)
		param->reps = 1;
	}
  }	
										/* ................................ */
cleanup:
  if (appname != NULL)
	DT_I(dtos_free2,(ctl, appname,
      F, "appname"));

  ctl->progname = NULL;

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
