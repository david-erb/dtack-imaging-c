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
| tabchief - client/server IP address mapper
| 
|.SYNOPSIS
|:tabchief [params] -connect host:port -cache file -ident name
|
|.SUMMARY
| Stream load and show full-motion video from the Tabchief Internet server.
| 
|.PARAMETERS
| :-connect:	connection spec (required for either client or server)
| :-cache:		cache file (required for server)
| :-ident:		cient name (required for client)
| :-reps:		number of times to repeat (default 0 means forever)
| :-dt:			delay in seconds between repeats (default 30)
|dtinclude(params1.dx)
|
|.EXAMPLES
| Server:
|:  tabchief -connect 156.46.122.16:5300 -cache /var/tabchief.cache
| 
| Client:
|:  tabchief -connect 156.46.122.16:5300 -name myname
|
|.ADVANTAGES
| -	Does not disturb the existing DNS server.
| 
|.TESTS
| tabchief -op test -connect x:y -input /tmp/xxx -out /tmp/yyy -config /tmp
| -input must be a locally-writable filename (deleted when tests are done)
| -config must be the server's config directory (test.sch not deleted)
| -output must be a server-writable filename (not deleted when done)
|
|dtinclude(params2.dx)
|
| END
 *..........................................................................*/

/*..........................................................................
  CHANGES
  19990619 DE added rev
  19990619 DE no delay before reboot
  19990619 DE explicitly non-console app
 *..........................................................................*/

#include <tabchief.h>
DT_RCSID("tabchief $RCSfile: tabchief.c,v $ $Revision: 1.8 $");

#undef DTPRJ_CONSOLE
#define DTPRJ_CONSOLE DT_0				/* make this a non-console app 		*/

#include <dtack/main.h>
#include <dtack/rev.h>

#include <tabchief.use>

#define DT_RC_SHUTDOWN ((dt_rc_e)(0xdeadfeeb))

#define dt_interactive_parse			/* don't use dialog 				*/\
  dt_enter_parse

#define CR (0x0d)
#define LF (0x0a)

										/* ................................ */
static const char *ops[] = {
  "server",
  "mockup",
  "test",
  "testts",
  "testqslocal",
  "testqsremote",
  "testcopy",
  "copyto",
  "copyfrom",
  "run",
  NULL
};

/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
tabchief_rev(
  dt_ctl_t *ctl,
  const char **rev_string,
  int *rev_major,
  int *rev_minor,
  long *rev_date)
{
  if (rev_string != NULL)
    *rev_string = "tabchief app";
  if (rev_major != NULL)
    *rev_major = 3;
  if (rev_minor != NULL)
    *rev_minor = 2;
  if (rev_date != NULL)
    *rev_date = 19990619;
  return DT_RC_GOOD;
}

static
dtrev_f *revs[] = {
  tabchief_rev,
  dtipc_rocket_rev,
  NULL
};


/* ------------------------------------------------------------------------
 * ------------------------------------------------------------------------ */

static
int
cgi_split(								/* separate string into tokens 		*/
  char *p,								/* string changed in place 			*/
  char c)								/* separator character 				*/
{
  char *q = p;
  int i;
  int l = strlen(p) + 1;				/* q needs the EOS too 				*/
  int n = 0;
  for (i=0; i<l; i++)
  {
    if (p[i] == c)
	{
      *q++ = '\0';
	  n++;
	}
	else
    if (p[i] == '+')
	{
      *q++ = ' ';
	}
	else
    if (p[i] == '%' && p[i+1] != '%')
	{
	  char t[3];
	  t[0] = p[i+1];
	  t[1] = p[i+2];
	  t[2] = '\0';
      *q++ = (char)strtol(t, 0, 16);
	  i += 2;
	}
	else
    if (p[i] == '%' && p[i+1] == '%')
	{
      *q++ = '%';
	  i += 1;
	}
	else
	  *q++ = p[i];
  }
  return n + 1;
}

/*..........................................................................
 *..........................................................................*/

static
void
tabchief_stamp(							/* output debug message				*/
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
tabchief_dbg(								/* output debug message				*/
  dt_ctl_t *ctl,						/* environment control 				*/
  const char *caller,					/* caller's name 					*/
  long class_bits,						/* debug message class 				*/
  const char *format,					/* message format, like sprintf() 	*/
  va_list ap)							/* format substitutions 			*/
{
  char s[64];
  tabchief_stamp(ctl, caller, 
    s, sizeof(s));
  ctl->dbg = NULL;						/* don't come back here 			*/
  dt_dbg_va(ctl, s, class_bits,			/* let dt_dbg_va() do all the work 	*/
    format, ap);
  ctl->dbg = tabchief_dbg;				/* come back here next time 		*/
}

/* ........................................................................ *
 * ........................................................................ */

dt_rc_e
DTCONFIG_API1
tabchief_err(								/* handle errors					*/
  dt_ctl_t *ctl,						/* environment control 				*/
  const char *caller,					/* caller's name 					*/
  const char *format,					/* message format, like sprintf() 	*/
  va_list ap)							/* format substitutions 			*/
{
  tabchief_t *tabchief = 
    (tabchief_t *)ctl->user;
  dt_rc_e rc;
  char s[64];
  tabchief_stamp(ctl, caller, 
    s, sizeof(s));

  if (tabchief != NULL &&
	  !tabchief->response[0])			/* no error yet? 					*/
    dtstr_printf_va(ctl,				/* keep first error 				*/
      tabchief->response, 
      sizeof(tabchief->response),
      format, ap);

  ctl->err = NULL;						/* don't come back here 			*/
  rc = dt_err_va(ctl, s,				/* log the error 					*/
    format, ap);
  ctl->err = tabchief_err;				/* come back here next time 		*/
  return rc;
}

/*..........................................................................*
 *..........................................................................*/

void
DTCONFIG_API1
tabchief_say(								/* output console message			*/
  dt_ctl_t *ctl,						/* environment control 				*/
  const char *caller,					/* caller's name 					*/
  const char *format,					/* message format, like sprintf() 	*/
  va_list ap)							/* format substitutions 			*/
{
  char s[64];
  tabchief_stamp(ctl, caller, 
    s, sizeof(s));

  ctl->say = NULL;						/* don't come back here 			*/
  dt_say_va(ctl, s,
    format, ap);
  ctl->say = tabchief_say;				/* come back here next time 		*/
}

/*..........................................................................
 *..........................................................................*/
dt_rc_e
tabchief_test_time_to_seconds(
  dtparam_t *param)
{
  DT_F("tabchief_test_time_to_seconds");
  dt_ctl_t *ctl = &param->ctl;
  dsched_t dsched;
  int seconds;

  dsched_init(&dsched, param, NULL);	/* initialize structure 			*/
  
#define GOOD(STRING, SECONDS) \
  dsched_time_to_seconds(&dsched, STRING, &seconds); \
  dt_assert(ctl, seconds == SECONDS, \
          F, "\"%s\" gave %d seconds and expected %d", \
          STRING, seconds, SECONDS)

  GOOD("3", 180);
  GOOD("3.3", 183);
  GOOD("1:3", 3780);
  GOOD("1:3.3", 3783);

#undef GOOD

#define BAD(STRING) \
  dsched.error[0] = '\0'; \
  dsched_time_to_seconds(&dsched, STRING, &seconds); \
  dt_assert(ctl, dsched.error[0], \
          F, "\"%s\" gave \"%s\" and expected it", \
          STRING, dsched.error)

  BAD("3.");
  BAD(".3.");
  BAD("3:");
  BAD("3:3.");
  BAD("3.3:");
  BAD("3:.");
  BAD(":3.");
  BAD(":3");
  BAD(":.3");

#undef BAD

  dsched_uninit(&dsched);
  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/

static
dt_rc_e
tabchief_test(
  dtparam_t *param,
  const char *ipcspec)
{
  DT_F("tabchief_test");
  dt_ctl_t *ctl = &param->ctl;

# define T(X) (!strcmp(#X, param->op))
  if (T(test) || T(testts))
    DT_Q(tabchief_test_time_to_seconds,(param));
  if (T(test) || T(testqslocal))
  {
    DT_Q(tabchief_test_qslocal,(param));
  }
  if (T(test) || T(testqsremote))
  {
    DT_Q(tabchief_test_qsremote,(param, ipcspec));
  }
  if (T(test) || T(testcopy))
    DT_Q(tabchief_test_copy,(param, ipcspec));
# undef T
  
  return DT_RC_GOOD;
}

/*..........................................................................*/

dt_rc_e 
DTCONFIG_API1
tabchief_arg_check(						/* check args and params			*/
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
  
DTMAIN1(tabchief)
{
  char F[32];
  dt_ctl_t *ctl = &param->ctl;
  dt_enter_arg_t arg[1];
  char env[256];
  char *ipcspec = NULL;
  long rep;
  int op;
  dt_rc_e rc = DT_RC_GOOD;
										/* ................................ */
  if (argv[0] != NULL)					/* we have a program name? 			*/
  {
	char *t;
	int i, l;
	char *pslash, *pdot;
	t = strdup(argv[0]);
	l = strlen(t);						/* length of program name 			*/
	for (i=l-1; i>0; i--)				/* change backslashes to slashes 	*/
	  if (t[i] == '\\')
		t[i] = '/';
      else
		t[i] = tolower(t[i]);
	pslash = strrchr(t, '/');			/* find last slash					*/
	if (pslash != NULL)
	  pslash++;
	else
	  pslash = t;
	pdot = strrchr(pslash, '.');		/* find last dot 					*/
	if (pdot != NULL &&					/* there is a dot? 					*/
		!strcmp(pdot, ".exe"))			/* and suffix is .exe? 				*/
	  *pdot = '\0';						/* chop off the suffix 				*/
	l = DT_MIN(strlen(t), sizeof(F)-1);
	for (i=0; i<l; i++)
	  F[i] = tolower(pslash[i]);
	F[i] = '\0';
	free(t);
  }
  else									/* have no program name? 			*/
	strcpy(F, "tabchief");
										/* ................................ */

  {
	int pid;
	ctl->progname = argv[0];			/* get name from executable file	*/
	DT_Q(dtos_getpid,(ctl, &pid));
  }

  DT_GI(dt_heapmon_init,(ctl));			/* init memory checking			    */

										/* ................................ */

  DT_GI(dtparam_init,(param));			/* init param structure             */

  DT_GI(dtparam_set,(param,
    DTPARAM_FLAG_PRIMARY |
    DTPARAM_FLAG_LENGTHY,
    &param->connect,					/* listen address 					*/
      "IPC spec", NULL, 
      "0:5780",
    &param->config,						/* config directory 				*/
      "Config dir", NULL, 
      "",
    &param->ident,						/* reverb server 					*/
      "Reverb server", NULL, 
      "none",
    &param->input, 
      "Input file", NULL, 
      "",
    &param->output, 
      "Output file", NULL, 
      "",
    &param->name, 
      "Proc list", NULL, 
      "tabstart",
    NULL));

  DT_GI(dtparam_set,(param,
    DTPARAM_FLAG_SECONDARY |
    DTPARAM_FLAG_LENGTHY,
    &param->reps, 
      "Repeat", NULL, 
      "0",
    &param->dt, 
      "Repeat delay seconds", NULL, 
      "30",
    &param->max, 
      "Reverb timeout", NULL, 
      "10000",
    &param->omega, 
      "Auto reverb freq", NULL, 
      "60000",
    &param->n, 
      "Reverb fail tol", NULL, 
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
  
  DT_C(dt_interactive_parse,(			/* parse command line interactively	*/
    argc, argv, F, usage, param,
    arg, tabchief_arg_check, NULL));
  
										/* ................................ */
  ctl->dbg = tabchief_dbg;				/* special logging needs 			*/
  ctl->err = tabchief_err;
  ctl->say = tabchief_say;

  if (!strcmp(param->op, "rev") ||		/* op is to say rev?				*/
	  DT_DBG(ctl, DT_DBG_MASK_USR1))
  {
	DT_C(dtrev_say,(ctl, revs));
  }

  if (!strcmp(param->op, "rev"))		/* op is to say rev?				*/
	goto cleanup;

# define IPCTYPE "rocket"
# define L (strlen(IPCTYPE ","))
  DT_C(dtos_malloc,(ctl,
    (void **)&ipcspec,
    strlen(param->connect) + L + 1));
  strcpy(ipcspec, IPCTYPE ",");			/* prepend socket before ipc spec 	*/
  strcat(ipcspec, param->connect);

  {
	char message[80];
	DT_C(dtstr_choice2,(ctl, ops,
      param->op, &op, 
      message, sizeof(message)));
    if (message[0])
	{
	  rc = dt_err(ctl, F, "%s",
        message);
	  goto cleanup;
	}
  }

  for (rep=0; 
	   param->reps == 0 ||
	   rep<param->reps; 
	   rep++)
  {
	if (!strcmp(ops[op], "server") ||	/* op is server? 					*/
		!strcmp(ops[op], "mockup"))		/* op is a mockup server? 			*/
	{
	  DT_C(tabchief_server,(param, 
        ipcspec));
	}
	if (!strncmp(ops[op], "test", 4))	/* op is test? 						*/
	{
	  DT_C(tabchief_test,(param, 
        ipcspec));
	}
	if (!strcmp(ops[op], "copyto") ||	/* op is client? 					*/
		!strcmp(ops[op], "copyfrom") ||
		!strncmp(ops[op], "run", 3))
	{
	  DT_C(tabchief_client,(param, 
        ipcspec, ops[op]));
	}
	
	if (rep < param->reps-1 &&			/* don't wait the last time		    */
		param->dt != 0)
  	  DT_C(dtos_delay,(ctl, 
        param->dt*1000L));

	{
	  char s[64];
	  DT_C(dtstr_printf,(ctl,
         s, sizeof(s),
        "rep %ld", rep));
	  DT_C(dt_yield,(ctl, s));
	}
  }	
										/* ................................ */
cleanup:
  if (ipcspec != NULL)
    DT_I(dtos_free,(ctl, ipcspec));

  if (rc == DT_RC_SHUTDOWN)
    dt_say(ctl, F, "shutting down");

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

#ifdef DTCONFIG_IMPLIED_MSW32
  if (rc == DT_RC_SHUTDOWN)
  {
#if 0
	dtos_delay(NULL, 10000);
#endif
    ExitWindowsEx(EWX_FORCE|EWX_REBOOT, 0L);
  }
#endif
  
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
