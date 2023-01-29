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
| mj - mj main program
| 
|.SYNOPSIS
|:mj [params] -connect host:port
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
|:  mj -connect 156.46.122.16:5790
| 
| Client:
|:  mj -connect 156.46.122.16:5300 -name myname
|
|dtinclude(params2.dx)
|
| END
 *..........................................................................*/

#include <mj.h>
DT_RCSID("mj $RCSfile: mj.c,v $ $Revision: 1.8 $");

#include <dtack/main.h>
#include <dtack/errno.h>

#include <mj.use>

#include <winsock.h>

#define dt_interactive_parse			/* don't use dialog 				*/\
  dt_enter_parse

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
dt_rc_e
dtprocs_notify(
  dt_ctl_t *ctl,
  void *procs)
{
  DT_F("dtprocs_notify");
#ifdef DTCONFIG_IMPLIED_UNIXLIKE
  long signal_number;
  int sysrc;
  int e;

  DT_Q(dtstr_to_long,(ctl, parent, &signal_number));
  sysrc = kill(getppid(), signal_number);
  e = errno;
  if (sysrc != 0)
	return dt_err(ctl, F,
      "kill(%d, %d)"
      DTERRNO_FMT,
      getppid(), signal_number,
      DTERRNO_ARG(e));
  return DT_RC_GOOD;
#endif
										/* ................................ */
#ifdef DTCONFIG_IMPLIED_MSW32
  HANDLE lock = NULL;
  char s[256];
  dt_rc_e rc = DT_RC_GOOD;

										/* ................................ */
  *s = '\0';
  GetEnvironmentVariable(
    "DTPROCS_LOCK", s, sizeof(s));
  dt_dbg(ctl, F, DT_DBG_MASK_COMMAND,
    "DTPROCS_LOCK=\"%s\"", s);

  if (s != NULL &&
	  *s != '\0')
  {
	lock = OpenSemaphore(				/* access the semaphore				*/
      SEMAPHORE_ALL_ACCESS, FALSE,
      s);
	if (lock == NULL)					/* could not get semaphore?			*/
	{
	  DWORD e = GetLastError();
	  char s[256];
	  DTERRNO_LASTS(e, s, sizeof(s));
	  rc = dt_err(ctl, F,
	    "OpenSemaphore("
        "SEMAPHORE_ALL_ACCESS, FALSE, "
        "\"%s\")"
        DTERRNO_LASTFS,
        s,
        DTERRNO_LASTES(e, s));
	  goto cleanup;
	}
  
	ReleaseSemaphore(lock, 1,			/* release access					*/
	  NULL);
  }
  
cleanup:
  if (lock != NULL)
	CloseHandle(lock);
  return rc;
#endif
}

/*..........................................................................
 *..........................................................................*/

static
dt_rc_e
dtprocs_getio(
  dt_ctl_t *ctl,
  void *procs,
  long *inhandle,
  long *outhandle)
{
  DT_F("dtprocs_notify");
										/* ................................ */
#ifdef DTCONFIG_IMPLIED_MSW32
  HANDLE prochandle = NULL;
  HANDLE duphandle;
  char s[256];
  dt_rc_e rc = DT_RC_GOOD;

										/* ................................ */
  *s = '\0';
  GetEnvironmentVariable(
    "DTPROCS_PPID", s, sizeof(s));
  dt_dbg(ctl, F, DT_DBG_MASK_COMMAND,
    "DTPROCS_PPID=\"%s\"", s);

  if (*s != '\0')						/* environment variable is set? 	*/
  {
	unsigned long v;
    char *e;
    v = strtoul(s, &e, 0);
	prochandle = OpenProcess(			/* access parent process 			*/
      PROCESS_ALL_ACCESS,
      TRUE, v);
	if (prochandle == NULL)
    {
	  char s[256];
	  DWORD e = GetLastError();
	  DTERRNO_LASTS(e, s, sizeof(s));
	  rc = dt_err(ctl, F,
        "OpenProcess("
        "..., 0x%08lx)"
        DTERRNO_LASTFS,
        v,
        DTERRNO_LASTES(e, s));
	  goto cleanup;
	}
  }
										/* ................................ */
  *s = '\0';
  GetEnvironmentVariable(
    "DTPROCS_INHANDLE", s, sizeof(s));
  dt_dbg(ctl, F, DT_DBG_MASK_COMMAND,
    "DTPROCS_INHANDLE=\"%s\"", s);

  if (inhandle != NULL &&				/* caller once input handle? 		*/
	  *s != '\0')						/* environment variable is set? 	*/
  {
	long v;
    BOOL ok;
	DT_C(dtstr_to_long,(ctl,
      s, &v));

    ok = DuplicateHandle(
      prochandle,						/* source process 					*/
      v,								/* handle to duplicate 				*/
      GetCurrentProcess(),				/* source process 					*/
      &duphandle,						/* duplicated handle 				*/
      0,								/* flags 							*/
      TRUE,								/* new handle is inheritable		*/
      DUPLICATE_SAME_ACCESS);			/* use same access flags 			*/
	if (!ok)
    {
	  char s[256];
	  DWORD e = GetLastError();
	  DTERRNO_LASTS(e, s, sizeof(s));
	  rc = dt_err(ctl, F,
        "DuplicateHandle("
        "0x%08lx, 0x%08lx...)"
        DTERRNO_LASTFS,
        prochandle,
        v,
        DTERRNO_LASTES(e, s));
	  goto cleanup;
	}

	*inhandle = (long)duphandle;

	dt_dbg(ctl, F, DT_DBG_MASK_COMMAND,
      "handle dup 0x%08lx->0x%08lx",
      v, *inhandle);
  }
  
										/* ................................ */
  *s = '\0';
  GetEnvironmentVariable(
    "DTPROCS_OUTHANDLE", s, sizeof(s));
  dt_dbg(ctl, F, DT_DBG_MASK_COMMAND,
    "DTPROCS_OUTHANDLE=\"%s\"", s);

  if (outhandle != NULL &&				/* caller wants output handle? 		*/
	  *s != '\0')						/* environment variable is set? 	*/
  {
	long v;
    BOOL ok;
	DT_C(dtstr_to_long,(ctl,
      s, &v));

    ok = DuplicateHandle(
      prochandle,						/* source process 					*/
      v,								/* handle to duplicate 				*/
      GetCurrentProcess(),				/* target process 					*/
      &duphandle,						/* duplicated handle 				*/
      0,								/* flags 							*/
      TRUE,								/* new handle is inheritable		*/
      DUPLICATE_SAME_ACCESS);			/* use same access flags 			*/
	if (!ok)
    {
	  char s[256];
	  DWORD e = GetLastError();
	  DTERRNO_LASTS(e, s, sizeof(s));
	  rc = dt_err(ctl, F,
        "DuplicateHandle("
        "0x%08lx, 0x%08lx...)"
        DTERRNO_LASTFS,
        prochandle,
        v,
        DTERRNO_LASTES(e, s));
	  goto cleanup;
	}

	*outhandle = (long)duphandle;
	
	dt_dbg(ctl, F, DT_DBG_MASK_COMMAND,
      "handle dup 0x%08lx->0x%08lx",
      v, *outhandle);
  }
  
cleanup:
  if (prochandle != NULL)
	CloseHandle(prochandle);

  return rc;
#endif
}

/*..........................................................................
 *..........................................................................*/

static
void
mj_stamp(								/* output debug message				*/
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
mj_dbg(							/* output debug message				*/
  dt_ctl_t *ctl,						/* environment control 				*/
  const char *caller,					/* caller's name 					*/
  long class_bits,						/* debug message class 				*/
  const char *format,					/* message format, like sprintf() 	*/
  va_list ap)							/* format substitutions 			*/
{
  char s[64];
  mj_stamp(ctl, caller, 
    s, sizeof(s));
  ctl->dbg = NULL;						/* don't come back here 			*/
  dt_dbg_va(ctl, s, class_bits,			/* let dt_dbg_va() do all the work 	*/
    format, ap);
  ctl->dbg = mj_dbg;				/* come back here next time 		*/
}

/* ........................................................................ *
 * ........................................................................ */

dt_rc_e
DTCONFIG_API1
mj_err(							/* handle errors					*/
  dt_ctl_t *ctl,						/* environment control 				*/
  const char *caller,					/* caller's name 					*/
  const char *format,					/* message format, like sprintf() 	*/
  va_list ap)							/* format substitutions 			*/
{
  dt_rc_e rc;
  char s[64];
  mj_stamp(ctl, caller, 
    s, sizeof(s));

  ctl->err = NULL;						/* don't come back here 			*/
  rc = dt_err_va(ctl, s,				/* log the error 					*/
    format, ap);
  ctl->err = mj_err;				/* come back here next time 		*/
  return rc;
}

/*..........................................................................*
 *..........................................................................*/

void
DTCONFIG_API1
mj_say(							/* output console message			*/
  dt_ctl_t *ctl,						/* environment control 				*/
  const char *caller,					/* caller's name 					*/
  const char *format,					/* message format, like sprintf() 	*/
  va_list ap)							/* format substitutions 			*/
{
  char s[64];
  mj_stamp(ctl, caller, 
    s, sizeof(s));

  ctl->say = NULL;						/* don't come back here 			*/
  dt_say_va(ctl, s,
    format, ap);
  ctl->say = mj_say;				/* come back here next time 		*/
}

/*..........................................................................*/

dt_rc_e 
DTCONFIG_API1
mj_arg_check(						/* check args and params			*/
  dt_ctl_t *ctl,
  void *app,
  dtparam_t *param,
  dt_enter_arg_t *arg,
  char *message,
  const char **start)
{
  if (message != NULL)
    *message = '\0';
  return DT_RC_GOOD;
}

/*..........................................................................
| NAME
| main program
| 
| END
 *..........................................................................*/
  
DTMAIN1(mj)
{
  DT_F("mj");
  dt_ctl_t *ctl = &param->ctl;
  dt_enter_arg_t arg[1];
  long rep;
  int op;
  long inhandle, outhandle;
  dt_rc_e rc = DT_RC_GOOD;

  {
	int pid;
	ctl->progname = argv[0];			/* make sure our name is correct 	*/
	DT_Q(dtos_getpid,(ctl, &pid));
  }

  DT_GI(dt_heapmon_init,(ctl));			/* init memory checking			    */

  if (argc == 1 &&
	  getenv("QUERY_STRING") != NULL)
  {
	char *q = strdup(
      getenv("QUERY_STRING"));
    char **argv2;
	int i, n;
	
	n = cgi_split(q, '&');
	argv2 = (char **)malloc((n+2) *
      sizeof(*argv2));
	argv2[0] = argv[0];
	for (i=0; i<n; i++)
	{
	  argv2[i+1] = q;
	  q += strlen(q) + 1;
	}
	argv2[i+1] = NULL;
	argc = i + 1;
	argv = argv2;
  }

  DT_GI(dtparam_init,(param));			/* init param structure             */

  DT_GI(dtparam_set,(param,
    DTPARAM_FLAG_PRIMARY |
    DTPARAM_FLAG_LENGTHY,
    &param->input, 
      "filter graph", NULL, 
      "",
    &param->variant, 
      "variant", NULL, 
      "jpegpush",
    &param->dt, 
      "Poll time", NULL, 
      "-1",
    NULL));

  DT_GI(dtparam_set,(param,
    DTPARAM_FLAG_SECONDARY |
    DTPARAM_FLAG_LENGTHY,
    &param->op, 
      "Operation", NULL, 
      "test",
    &param->reps, 
      "Repetitions", NULL, 
      "0",
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
    argc, &argv[0], "", usage, param,
    arg, mj_arg_check, NULL));
										/* ................................ */
  ctl->dbg = mj_dbg;					/* special logging needs 			*/
  ctl->err = mj_err;
  ctl->say = mj_say;

  {
	WORD request = MAKEWORD(1, 1);
	WSADATA w;
	memset(&w, 0, sizeof(w));
	WSAStartup(request, &w);
  }

  DT_C(dtprocs_getio,(ctl, NULL,		/* get io handles					*/
    &inhandle, &outhandle));
  DT_C(dtprocs_notify,(ctl, NULL));		/* notify parent 					*/

#ifdef NONO
  {
	  unsigned long got;
#define HEADER "Content-type: text/plain\r\n\r\nhello\r\n"
	if (SOCKET_ERROR == send(outhandle, HEADER, strlen(HEADER), 0))
	{
	  DWORD e = WSAGetLastError();
	  char s[256];
	  DTERRNO_LASTS(e, s, sizeof(s));
	  rc = dt_err(ctl, F,
	    "send(0x%08lx, ...) e=%d"
        DTERRNO_LASTFS,
        outhandle, e,
        DTERRNO_LASTES(e, s));
	  goto cleanup;
	}
  }

  dt_dbg(ctl, F, DT_DBG_MASK_ANY, "send(0x%08lx) worked!", outhandle);
#endif

  ctl->user2 = (void *)inhandle;		/* for passing to cgiout filter 	*/
  ctl->user3 = (void *)outhandle;
  ctl->user4 = (void *)param->variant;
  
  for (rep=0; 
	   param->reps == 0 ||
	   rep<param->reps; 
	   rep++)
  {
	if (DT_ABBREV(param->op, "test"))	/* op is to test?					*/
	{
	  
#ifdef DTCONFIG_IMPLIED_MSW32
  HANDLE handle;
  unsigned long got;
  char t[256];
  handle = GetStdHandle(STD_OUTPUT_HANDLE);
# define HEADER "Content-type: text/plain\r\n\r\n"
  WriteFile(handle, HEADER, strlen(HEADER), &got, NULL);
# define OUTMSG "mj: test: this is stdout\r\n"
  WriteFile(handle, OUTMSG, strlen(OUTMSG), &got, NULL);
  sprintf(t, "argc = %d\r\n", argc);
  WriteFile(handle, t, strlen(t), &got, NULL);
  sprintf(t, "argv[0] = \"%s\"\r\n", argv[0]);
  WriteFile(handle, t, strlen(t), &got, NULL);
  sprintf(t, "GetCommandLine() = \"%s\"\r\n", GetCommandLine());
  WriteFile(handle, t, strlen(t), &got, NULL);
# define ERRMSG "mj: test: this is stderr\r\n"
  handle = GetStdHandle(STD_ERROR_HANDLE);
  WriteFile(handle, ERRMSG, strlen(ERRMSG), &got, NULL);
#endif
	  if (param->reps == 0)
	    param->reps = 1;
	}
	else
	if (DT_ABBREV(param->op, "play"))	/* op is to play?					*/
	{
      dt_dbg(ctl, F, DT_DBG_MASK_USR1,
        "playing");
	  DT_C(mj_play,(param));
	  if (param->reps == 0)
	    param->reps = 1;
	}
	else
	{
#ifdef NONO
	  DT_C(mj_client,(param,
        param->op));
	  if (param->reps == 0)
	    param->reps = 1;
#endif
	}
  }	
										/* ................................ */
cleanup:
  DT_I(dt_leave,(param));				/* clean up before leaving			*/

  dt_dbg(ctl, F, DT_DBG_MASK_USR1,
    "leaving");
  
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
