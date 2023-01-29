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
  CHANGES
  19990619 DE explicitly non-console app
  19990619 DE say rev when usr1
  19990126 DE change recv to recvt
 *..........................................................................*/


#define DTPARAM_FLAG_ALLOC (0x0010)


/*..........................................................................
| USAGE
| tableau - tableau streaming video client
| 
|.SYNOPSIS
|:tableau [params]
|
|.SUMMARY
| Stream load and show full-motion video from the Tableau Internet server.
| 
|.PARAMETERS
| :-input:		movie name (required)
| :-dx:			display x zoom (default 1)
| :-dy:			display y zoom (default 1)
| :-background:	color table update frequency in frames (default 0)
|dtinclude(params1.dx)
|
|.DISPLAY
| For X Window, the program will use the :DISPLAY: environment variable.
|
|.EXAMPLES
| tableau -in demise -dx 1.5 -dy 1.5 -D usr1+usr2
| 
|dtinclude(params2.dx)
|
| END
 *..........................................................................*/


#include <tableau.h>
DT_RCSID("tableau $RCSfile: tableau.c,v $ $Revision: 1.8 $");

#undef DTPRJ_CONSOLE
#define DTPRJ_CONSOLE DT_0				/* make this a non-console app 		*/

#include <dtack/main.h>
#include <dtack/errno.h>

#include <dtack/licens.h>

#include <tableau.use>

#define DTIPC_FLAG_NOQUEUE 0x0400

#define ACCEPT_TIMEOUT (30000)

#define dt_interactive_parse			/* don't use dialog 				*/\
  dt_enter_parse
#define dt_interactive_leave			/* don't use dialog 				*/\
  dt_leave
#define LEAVE_ARGS param

#define CR (0x0d)
#define LF (0x0a)

static
dtrev_f *revs[] = {
  tableau_rev,
  dtfb_vfw_rev,
  dtfb_meteor_rev,
  dtfb_bt_rev,
  NULL
};

#ifdef DTCONFIG_IMPLIED_UNIXLIKE
#  define USE_SIGNAL_JUMP
#endif

#ifdef USE_SIGNAL_JUMP
#ifndef DTDEPEND						/* no makefile dependency 			*/
#  include <signal.h>
#  include <setjmp.h>
#endif

/*..........................................................................*/
static jmp_buf jump;
static
void sigint()
{
  longjmp(jump, SIGINT); 
}
#endif


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
tableau_command(						/* receive initial command			*/
  dtparam_t *param,
  dtipc_t *ipc,
  tableau_command_t **command)
{
  DT_F("tableau_command");
  dt_ctl_t *ctl = &param->ctl;
  tableau_ack_t ack;
  dt_rc_e rc;

  memset(&ack, 0, sizeof(ack));
  
  DTCSTACK_HIGHWATER;

  DT_C(dtipc_recvt,(ctl, ipc,			/* first receive from client	    */
    (void **)command,
    sizeof(**command),
    param->epsilon));

  (*command)->version = DTIPC_FIX(DT_C,
    ctl, ipc, (*command)->version);

  param->offset = DTIPC_FIX(DT_C, ctl,
    ipc, (*command)->frame);

  dt_dbg(ctl, F, DT_DBG_MASK_COMMAND,
    "command: %s %s version=%04ld frame=%ld",
    (*command)->command,
    (*command)->name,
    (*command)->version, 
    (*command)->frame);

  if ((*command)->version !=			/* check sender's version 			*/
	  TABLEAU_VERSION)
  {
	ack.rc = (tableau_len_t)			/* special ack tells sender problem */
      TABLEAU_VERSION;
	rc = dt_err(ctl, F,
	  "sender version %04ld"
      " does not match our version %04ld",
      (*command)->version,
	  TABLEAU_VERSION);
	goto cleanup;
  }

cleanup:
  {
	if (ack.rc == 0)					/* rc not already set? 				*/
	  ack.rc = (tableau_len_t)rc;
	DT_I(dtipc_send,(ctl, ipc,			/* ack the command 					*/
      &ack, sizeof(ack)));
  }
  
  return rc;
}

/*..........................................................................
 *..........................................................................*/

static
dt_rc_e
tableau_request(						/* receive http GET request			*/
  dtparam_t *param,
  dtipc_t *ipc,
  tableau_command_t **command)
{
  DT_F("tableau_request");
  dt_ctl_t *ctl = &param->ctl;
  char *request = NULL;
# define REQUEST_MAX (1024)
  int l;
  char *p;
  dt_rc_e rc;
  
  DTCSTACK_HIGHWATER;
  
  DT_C(dtos_malloc2,(ctl,				/* space for command block 			*/
    (void **)&request, REQUEST_MAX,
    F, "request buffer"));

  DT_C(dtipc_read_line,(ctl, ipc,
    request, REQUEST_MAX));

  l = strlen(request);
  if (l >= REQUEST_MAX-1 &&
	  request[l-1] != LF)				/* line we just got is unfinished? 	*/
    return dt_err(ctl, F,
      "http request longer than %d",
      REQUEST_MAX);

  while (l > 0 &&						/* nuke the crlf's 					*/
		 request[l-1] == LF ||
		 request[l-1] == CR)
	request[--l] = '\0';

  if (strncmp(request, "GET /", 5))		/* check for valid http request		*/
	return dt_err(ctl, F,
      "http request not GET");

  p = strchr(request+4, ' ');			/* look for space in request 		*/
  if (p != NULL)						/* terminate request at space 		*/
	*p = '\0';
  p = request + 5;
  
  DT_C(dtos_malloc2,(ctl,				/* space for command block 			*/
    (void **)command,
    sizeof(**command),
    F, "command buffer"));
  memset((void *)(*command), 0,
    sizeof(**command));

  strcpy((*command)->command, "put");	/* http server always puts 			*/
  dtstr_printf(ctl, (*command)->name,	/* get name from request line 		*/
    sizeof((*command)->name), "%s", p);
  (*command)->version =					/* presume versions match 			*/
    TABLEAU_VERSION;
  (*command)->frame = 0;

  dt_dbg(ctl, F, DT_DBG_MASK_COMMAND,
    "command: %s %s version=%04ld frame=%ld",
    (*command)->command,
    (*command)->name,
    (*command)->version, 
    (*command)->frame);

cleanup:
  if (request != NULL)
	DT_I(dtos_free2,(ctl, request,
      F, "request buffer"));
  return rc;
}

/*..........................................................................
 *..........................................................................*/

static
dt_rc_e
tableau_setup(
  dtparam_t *param,
  dtipc_t *ipc,
  int is_http,
  const char *getspec,
  tableau_get_t *get,
  const char *putspec,
  tableau_put_t *put)
{
  DT_F("tableau_setup");
  dt_ctl_t *ctl = &param->ctl;
  tableau_command_t *command = NULL;
  char *s = NULL;
  dt_rc_e rc;

  if (is_http)							/* client sending HTTP request?		*/
  {
	DT_C(tableau_request,(param,
      ipc, &command));
  }
  else			
  {
	DT_C(tableau_command,(param,
      ipc, &command));
  }

# define IS(CMD) \
    (!strcmp(CMD, command->command))
										/* ................................ */
  if (IS("get"))						/* client is sending data			*/
  {
	get->u.ipc.ipc = ipc;				/* server gets data from this ipc 	*/
	get->function = tableau_get_ipc;
	get->free = tableau_get_ipc_free;
	get->type = TABLEAU_GET_IPC;
	get->isserver = 1;
	DT_C(dtos_strdup3,(ctl,
      putspec,
      strlen(command->name)? ",": "",  
      command->name, &s));
	DT_C(tableau_put_setup,(param,		/* where server will put data 		*/
      put, s));
    dt_dbg(ctl, F, DT_DBG_MASK_COMMAND,
      "get from ipc, put to \"%s\"",
      s);
	if (param->ident &&					/* param->ident is set? 			*/
		(param->ident_flag &
		 DTPARAM_FLAG_ALLOC))
	  DT_C(dtos_free2,(ctl,				/* release old param->ident 		*/
        (char *)param->ident, 
        F, param->ident));
	DT_C(dtos_strdup,(ctl,				/* set new value for param->ident 	*/
      command->name,
      (char **)&param->ident));
	param->ident_flag |=				/* thus shall it be released later 	*/
      DTPARAM_FLAG_ALLOC;
  }
  else									/* ................................ */
  if (IS("put"))						/* client wants data				*/
  {
	DT_C(dtos_strdup3,(ctl,
      getspec,
      strlen(command->name)? ",": "",  
      command->name, &s));
	DT_C(tableau_get_setup,(param,		/* where server gets data from 		*/
      get, s));
	if (is_http)						/* client sending HTTP request?		*/
	{
	  put->type = TABLEAU_PUT_PUSH;
	  put->u.push.ipc = ipc;			/* server puts data to this ipc 	*/
	  put->function = tableau_put_push;
	  put->isserver = 1;
      dt_dbg(ctl, F, DT_DBG_MASK_COMMAND,
        "get from \"%s\", put to push",
        s);
	}
	else
	{
	  put->type = TABLEAU_PUT_IPC;
	  put->u.ipc.ipc = ipc;				/* server puts data to this ipc 	*/
	  put->function = tableau_put_ipc;
	  put->isserver = 1;
      dt_dbg(ctl, F, DT_DBG_MASK_COMMAND,
        "get from \"%s\", put to ipc",
        s);
	}
  }
  else									/* ................................ */
  {
	rc = dt_err(ctl, F,
      "unknown command");
  }

cleanup:
  if (s != NULL)
    DT_I(dtos_free2,(ctl, s,
      F, "strdup3"));

  if (command != NULL)
    DT_I(dtos_free2,(ctl, command,
      F, "command"));

  return rc;
}

/*..........................................................................
 *..........................................................................*/

static
dt_rc_e
tableau_server_transact(
  dtparam_t *param,
  dtipc_t *ipc,
  int is_http,
  const char *getspec,
  const char *putspec,
  int *partner_stopped)
{
  DT_F("tableau_transact");
  dt_ctl_t *ctl = &param->ctl;
  dtos_time_t beg, end;
  tableau_get_t _get, *get = &_get;
  tableau_put_t _put, *put = &_put;
  dt_rc_e rc;
   
  memset(get, 0, sizeof(*get));
  memset(put, 0, sizeof(*put));
  
  *partner_stopped = 0;

  DTCSTACK_HIGHWATER;

  DT_C(tableau_setup,(param,			/* set up get and put				*/
    ipc, is_http,
    getspec, get,
    putspec, put));

  DT_C(tableau_dialog,(param,			/* have VIDSTART dialog				*/
    "start", ipc->client_spec,
    0, 0, 0,
    ctl->progname));

  DT_C(dtos_time,(ctl, &beg));
  DT_C(tableau_shovel,(param,			/* shovel bits 						*/
    param->cache,						/* dgserver 						*/
    get,  
    param->op, 
    param->variant, 
    put));

  *partner_stopped = 
    put->partner_stopped ||
    get->partner_stopped;

cleanup:
  DT_I(dtos_time,(ctl, &end));
  
  DT_I(tableau_dialog,(param,			/* have VIDEND dialog				*/
    "stop", ipc->client_spec,
    (long)(end.wall-beg.wall),
    put->shoveled_frames,
    (long)put->shoveled_kb,
    NULL));

  if (get->shoveled_kb != put->shoveled_kb)
    dt_dbg(ctl, F, DT_DBG_MASK_USR1,
      "server to %s shoveled"
      " %g kbytes in and %g kbytes out",
      ipc->client_spec, 
      get->shoveled_kb, put->shoveled_kb);
  else
  if (get->shoveled_kb != 0)
    dt_dbg(ctl, F, DT_DBG_MASK_USR1,
      "server to %s shoveled"
      " %g kbytes",
      ipc->client_spec, get->shoveled_kb); 
    
  if (put->function != NULL)
  {
    dt_dbg(ctl, F, DT_DBG_MASK_COMPLEX, 
      "(put->function) 0x%08lx", 
      put->function);
    DT_I((put->function),(param, put,
      NULL, NULL, NULL, 0));
  }

  if (get->free != NULL)
  {
    dt_dbg(ctl, F, DT_DBG_MASK_COMPLEX, 
      "(get->free) 0x%08lx", get->free);
    DT_I((get->free),(param, get));
  }

  return rc;
}

/*..........................................................................
 *..........................................................................*/

static
dt_rc_e
tableau_momma(
  dtparam_t *param,
  const char *ipcspec,
  const char *getspec,
  const char *putspec)
{
  DT_F("tableau_momma");
  dt_ctl_t *ctl = &param->ctl;
  char *p;
  dtipc_t _ipc, *ipc = &_ipc;
  int ipc_registered = 0;
  int ipc_accepted = 0;
  dtfd_t fd;
  int partner_stopped;
  dt_rc_e rc;
 
  fd.fd = -1;

  DTCSTACK_HIGHWATER;

  memset(ipc, 0, sizeof(*ipc));
  p = strchr(ipcspec, ':');				/* find colon in listen spec 		*/
  if (p == NULL ||						/* no colon? 						*/
	  strcmp(p, ":0"))					/* or port other than 0? 			*/
  {
	DT_C(dtipc_register,(ctl, ipc,		/* register as forking momma server	*/
      ipcspec, 0,
      DTIPC_FLAG_FORK_AFTER_WAIT));
	ipc_registered = 1;
	dt_dbg(ctl, F, DT_DBG_MASK_USR1,
      "momma server"
      " beginning operation at %s",
      ipc->server_spec);
	DT_C(dtipc_wait,(ctl, ipc));		/* wait for client connection		*/
	ipc_accepted = 1;

    dt_dbg(ctl, F, DT_DBG_MASK_USR1,
      "child forked to serve %s",
      ipc->client_spec);
  }
  else									/* listen spec ends in :0? 			*/
  {
	int is_ready;

	DT_C(dtipc_register,(ctl, ipc,		/* register as non-forking server	*/
      ipcspec, 0,
      DTIPC_FLAG_SELF_BACKGROUND)); 
	ipc_registered = 1;
    if (!(ipc->flags &					/* this is the parent returning?	*/
		  DTIPC_FLAG_SELF_BACKGROUND))
	  goto cleanup;						/* having procreated, parent's done */
	
	printf("server_spec: %s\n",			/* stdout for CGI support			*/
      ipc->server_spec);
	
	fclose(stdout);						/* close CGI socket 				*/
	fclose(stderr);						/* close CGI socket 				*/

	dt_dbg(ctl, F, DT_DBG_MASK_IPC,
      "child closed stdout and stderr");
#ifdef NONO
	sysrc = dup(2);						/* recreate an fd 1 for leaks 		*/
	if (sysrc != 1)
	{
	  rc = dt_err(ctl, F,
        "dup(2) = %d"
        DTERRNO_FMT,
        sysrc,
        DTERRNO_ARG(errno));
	  goto cleanup;
	}
#endif

#   ifdef DTCONFIG_FG1
	{									/* some sort of chroot bug disturbs	*/
	  char fn[32];						/* the first file written			*/
	  long pid = getpid();
	  sprintf(fn, "/tmp/pid.%ld", pid);
	  DT_C(dtfd_open,(ctl, &fd,
        fn, "w"));
	  DT_C(dtfd_write_all,(ctl, &fd,
        &pid, sizeof(pid)));
	  DT_C(dtfd_close,(ctl, &fd));		/* close and delete the bad file 	*/
	  DT_C(dtos_unlink,(ctl, fn));
	}
#   endif

	DT_C(dtipc_poll,(ctl,				/* wait for a client to connect		*/
      ipc,
      DTIPC_POLL_ACCEPT, 
      ACCEPT_TIMEOUT,
      "connection from client",
      &is_ready));
	if (!is_ready)
    {
	  rc = dt_err(ctl, F,
        "client"
        " did not connect to %s"
        " within %ld ms",
        ipc->server_spec,
        ACCEPT_TIMEOUT);
	  goto cleanup;
    }

	DT_C(dtipc_wait,(ctl, ipc));		/* accept client connection			*/
	ipc_accepted = 1;

	dt_dbg(ctl, F, DT_DBG_MASK_USR1,
      "dedicated server %s"
      " beginning for client %s",
      ipc->server_spec,
      ipc->client_spec);
  }
										/* ................................ */
# if defined(USE_SIGNAL_JUMP)
  {
	int jumpcode;
	struct sigaction act;

	jumpcode = setjmp(jump);			/* set jump point 					*/
	if (jumpcode != 0)					/* we jumped? 						*/
	{
	  rc = dt_err(ctl, F,
        "SIGINT caught");
	  goto cleanup;
	}

	sigemptyset(&act.sa_mask);
	sigaddset(&act.sa_mask, SIGINT);
	sigprocmask(SIG_UNBLOCK,
      &act.sa_mask, NULL);

	act.sa_handler = sigint;
	act.sa_flags = 0;
	sigaction(SIGINT, &act, NULL);		/* set up our signal handler	    */
  }
# endif
										/* ................................ */

  DT_C(tableau_server_transact,(		/* shovel							*/
    param, ipc, 0,
    getspec, putspec,
    &partner_stopped));

cleanup:
  if (ipc_accepted)
    DT_I(dtipc_done,(ctl, ipc));		/* server finishes transaction	    */
  if (ipc_registered)
    DT_I(dtipc_unregister,(ctl, ipc));	/* server closes connection			*/

  return rc;
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
  const char *env;
  long signal_number;
  int sysrc;
  int e;

  env = getenv("DTPROCS_LOCK");
  if (env != NULL && *env != '\0')
  {
    DT_Q(dtstr_to_long,(ctl, env, &signal_number));
    sysrc = kill(getppid(), signal_number);
    e = errno;
    if (sysrc != 0)
	  return dt_err(ctl, F,
        "kill(%d, %d)"
        DTERRNO_FMT,
        getppid(), signal_number,
        DTERRNO_ARG(e));
  }
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
tableau_exclusive(
  dtparam_t *param,
  const char *ipcspec,
  int is_http,
  const char *getspec,
  const char *putspec)
{
  DT_F("tableau_server");
  dt_ctl_t *ctl = &param->ctl;
  dtipc_t _ipc, *ipc = &_ipc;
  int ipc_registered = 0;
  int ipc_accepted = 0;
  int partner_stopped;
  dt_rc_e rc;

  DTCSTACK_HIGHWATER;

  memset(ipc, 0, sizeof(*ipc));
  
  dt_dbg(ctl, F, DT_DBG_MASK_USR1,
    "exclusive %s server"
    " beginning operation",
    is_http? "http": "ipc");
  DT_C(dtipc_register,(ctl, ipc,		/* register as non-forking server	*/
    ipcspec, 0, 0));
  ipc_registered = 1;

  if (param->parent != NULL &&			/* parent wants to be notified? 	*/
	  param->parent[0] != '\0')
	DT_C(dtprocs_notify,(ctl, NULL));	/* notify parent 					*/
  
  while(1)								/* loop handling transactions 		*/
  {
	DT_C(dtipc_wait,(ctl, ipc));		/* wait for client connection		*/
	ipc_accepted = 1;

    dt_dbg(ctl, F, DT_DBG_MASK_USR1,
      "exclusively serving %s (%s)",
      ipc->client_spec,
      is_http? "http": "ipc");

    DT_G(tableau_server_transact,(		/* shovel							*/
      param, ipc, is_http,
      getspec, putspec, 
      &partner_stopped));
	if (partner_stopped)
	  rc = DT_RC_GOOD;
	if (rc == DT_RC_BAD)				/* ignore shovel error 				*/
	  rc = DT_RC_GOOD;
	if (rc != DT_RC_GOOD)
	  goto cleanup;
	if (is_http)						/* http server? 					*/
	  goto cleanup;						/* serve single client and die 		*/

	ipc_accepted = 0;
    DT_C(dtipc_done,(ctl, ipc));		/* server finishes transaction	    */
  }

cleanup:
  if (ipc_accepted)
    DT_I(dtipc_done,(ctl, ipc));		/* server finishes transaction	    */
  if (ipc_registered)
    DT_I(dtipc_unregister,(ctl, ipc));	/* server closes connection			*/

  return rc;
}

/*..........................................................................
 *..........................................................................*/

static
dt_rc_e
tableau_client(
  dtparam_t *param,
  const char *getspec,
  const char *putspec)
{
  DT_F("tableau_client");
  dt_ctl_t *ctl = &param->ctl;
  tableau_get_t _get, *get = &_get;
  tableau_put_t _put, *put = &_put;
  dtos_time_t beg, end;
  long dbg_mask;
  char host[64];
  dt_rc_e rc;
  
  memset(get, 0, sizeof(*get));
  memset(put, 0, sizeof(*put));

										/* ................................ */

  DT_C(tableau_get_setup,(param,		/* set up source 					*/
    get, getspec));

  DT_C(tableau_put_setup,(param,		/* set up sink 						*/
    put, putspec));


  DT_C(tableau_dialog,(param,			/* have VIDSTART dialog 			*/
    "VSTART", "",
    0, 0, 0,
    ctl->progname));

  DT_C(dtos_time,(ctl, &beg));
  DT_C(tableau_shovel,(param,			/* shovel bits 						*/
    param->cache,						/* dgserver 						*/
    get, 
    param->op,
    param->variant,
    put));

cleanup:
  DT_I(dtos_time,(ctl, &end));
  
  DT_I(tableau_dialog,(param,			/* have VIDSTOP dialog				*/
    "VSTOP", "",
    (long)(end.wall-beg.wall),
    put->shoveled_frames,
    (long)put->shoveled_kb,
    NULL));

  if (get->shoveled_kb != put->shoveled_kb)
    dt_dbg(ctl, F, DT_DBG_MASK_USR1,
      "client shoveled"
      " %g kbytes in and %g kbytes out",
      get->shoveled_kb, put->shoveled_kb);
  else
  if (get->shoveled_kb != 0)
    dt_dbg(ctl, F, DT_DBG_MASK_USR1,
      "client shoveled"
      " %g kbytes",
      get->shoveled_kb); 

  if (put->function != NULL)
  {
    dt_dbg(ctl, F, DT_DBG_MASK_COMPLEX, 
      "(put->function) 0x%08lx", put->function);
    DT_I((put->function),(param, put,
      NULL, NULL, NULL, 0));
  }

  if (get->free != NULL)
  {
    dt_dbg(ctl, F, DT_DBG_MASK_COMPLEX, 
      "(get->free) 0x%08lx", get->free);
    DT_I((get->free),(param, get));
  }

  return rc;
}

/*..........................................................................*/

dt_rc_e 
DTCONFIG_API1
tableau_arg_check(						/* check args and params			*/
  dt_ctl_t *ctl,
  void *app,
  dtparam_t *param,
  dt_enter_arg_t *arg,
  char *message,
  const char **start)
{
  if (param->input == NULL ||
	  strlen(param->input) == 0)
  {
	if (param->input == NULL ||
		strlen(param->input) == 0)
	  dtstr_printf(ctl, 
        message, DT_ENTER_MSG_MAXLEN,
        "Please enter something"
        " for \"%s\"",
        param->input_desc);
    *start = param->input_desc;			/* set focus at this field 			*/
  }

  if (param->screen == NULL ||
	  strlen(param->screen) == 0)
  {
	if (param->screen == NULL ||
		strlen(param->screen) == 0)
	  dtstr_printf(ctl, 
        message, DT_ENTER_MSG_MAXLEN,
        "Please enter something"
        " for \"%s\"",
        param->screen_desc);
    *start = param->screen_desc;			/* set focus at this field 			*/
  }
  
  return DT_RC_GOOD;
}

/*..........................................................................
| NAME
| main program
| 
| END
 *..........................................................................*/

#ifdef __cplusplus
extern "C" {
#endif
  
DTMAIN2(tableau)
{
  char F[32];
  dt_ctl_t *ctl = &param->ctl;
  dt_enter_arg_t arg[1];
  char *ipcspec = NULL;
  dtlicense_t license;
  dt_rc_e rc;

  license.initialized = 0;

  DTMAIN_NAME(F);						/* get program name from argv[0]	*/

  {
	int pid;
	ctl->progname = argv[0];			/* make sure our name is correct 	*/
	DT_Q(dtos_getpid,(ctl, &pid));
  }

  DT_Q(dt_heapmon_init,(ctl));			/* init memory checking			    */

										/* ................................ */
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

										/* ................................ */
  DT_Q(dtparam_init,(param));			/* init param structure             */

  DT_Q(dtparam_set,(param,
    DTPARAM_FLAG_PRIMARY |
    DTPARAM_FLAG_LENGTHY,
    &param->input,			
      "Grab from", NULL, 
      "vfw,Creative,320,240",
    &param->screen, 
      "Send to", NULL, 
      "rocket,156.46.122.16:5710+be.x",
    NULL));

  DT_Q(dtparam_set,(param,
    DTPARAM_FLAG_SECONDARY |
    DTPARAM_FLAG_LENGTHY,
    &param->op, 
      "Time stamp", NULL, 
      "raw",
    &param->config, 
      "Config dir", NULL, 
      NULL,
    NULL));

  DT_Q(dtparam_set,(param,	
    DTPARAM_FLAG_TERTIARY,
    &param->t, "Quality",          NULL, "30",
    &param->n, "Frame count",      NULL, "0",
    &param->max, "File wait ms",  NULL, "180000",
    &param->epsilon, "Ipc wait ms",  NULL, "-1",
    &param->dt, "Frame delay ms",  NULL, "0",
    &param->dz, "Frame fifo size", NULL, "100",
    NULL));

  DT_Q(dtparam_set,(param,
    DTPARAM_FLAG_QUATERNARY |
    DTPARAM_FLAG_LENGTHY,
    &param->connect, 
      "Listen port", NULL, 
      NULL,
    &param->ident, 
      "Ident", NULL, 
      NULL,
    &param->cache, 
      "Http port", NULL, 
      NULL,
    &param->path, 
      "Roll file path", NULL, 
      "/img/tableau",
    &param->variant, 
      "Encoding scheme", NULL, 
      "jpeg",
    &param->dialog, 
      "Start/done url", NULL, 
      "",
    NULL));

  DT_Q(dtparam_set,(param,	
    DTPARAM_FLAG_QUATERNARY,
    &param->dx, "Display x zoom",   NULL, "1",
    &param->dy, "Display y zoom",   NULL, "1",
    &param->background, "Recompute colors", NULL, "0",
    NULL));

  DT_Q(dtparam_set,(param,	
    DTPARAM_FLAG_QUATERNARY,
    &param->color, "Color to encode", NULL, "rgb888",
    &param->sigma, "Frame averaging", NULL, "1",
    &param->psi, "Internet sponge", NULL, "100000",
    &param->offset, "First frame", NULL, "0",
    &param->parent, "Parent process", NULL, "0",
    NULL));

  DT_Q(dtparam_set,(param,				/* hide params we always parse for	*/
    DTPARAM_FLAG_QUATERNARY,
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
    arg, tableau_arg_check, NULL));
  
										/* ................................ */
  ctl->dbg = tableau_dbg;				/* special logging needs 			*/
  ctl->err = tableau_err;
  ctl->say = tableau_say;

  if (!strcmp(param->op, "rev") ||		/* op is to say rev?				*/
	  DT_DBG(ctl, DT_DBG_MASK_USR1))
  {
	DT_C(dtrev_say,(ctl, revs));
  }

  if (!strcmp(param->op, "rev"))		/* op is to say rev?				*/
	goto cleanup;
										/* ................................ */
  if (!strncmp(param->op, "test", 4))	/* op is test? 						*/
  {
	DT_C(tableau_test,(param)); 
  }
  else
  if (param->connect != NULL)			/* op is server?					*/
  {
	char *c;
	DT_C(dtlicense_request,(ctl,		/* request a tableau license 		*/
      &license, param->config, 
      ctl->progname));
	DT_C(dtos_strdup,(ctl, 
      param->connect, &ipcspec));
	c = ipcspec + strlen(ipcspec) - 1;
	if (*c == '!')						/* last char of connect is '!'? 	*/
	{
	  *c = '\0';
	  DT_C(tableau_exclusive,(param,	/* run exclusive ipc server 	   	*/
        ipcspec, 0,
        param->input,
        param->screen));
	}
	else
	if (*c == '^')						/* last char of connect is '^'? 	*/
	{
	  *c = '\0';
	  DT_C(tableau_exclusive,(param,	/* run exclusive http server 		*/
        ipcspec, 1,
        param->input,
        param->screen));
	}
	else
	  DT_C(tableau_momma,(param,		/* run forking momma server 		*/
        ipcspec,
        param->input,
        param->screen));
  }
  else
  {										/* op is client? 					*/
	DT_C(dtlicense_request,(ctl,		/* request a tableau license 		*/
      &license, param->config, 
      ctl->progname));

	dt_dbg(ctl, F, DT_DBG_MASK_USR1,
      "client beginning operation");

    DT_C(tableau_client,(param,			/* start the client 				*/
      param->input,
      param->screen));

	dt_dbg(ctl, F, DT_DBG_MASK_USR1,
      "client ceasing operation");
  }
										/* ................................ */
cleanup:
  DT_I(dtlicense_release,(ctl,
    &license));
  
  if (ipcspec != NULL)
	DT_I(dtos_free2,(ctl, ipcspec,
      F, "ipcspec"));
  if (rc == DT_RC_SHUTDOWN)
  dt_dbg(ctl, F, DT_DBG_MASK_USR1,
    "shutting down computer");

  DT_I(dt_interactive_leave,(			/* clean up before leaving			*/
    LEAVE_ARGS));

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
     ExitWindowsEx(EWX_FORCE|EWX_REBOOT, 0L);
#endif

  return rc;
}


#ifdef __cplusplus
}
#endif






/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
  ! Copyright (C) 1998 Dtack Inc. All Rights Reserved
  ! To use this file, you must have signed a license agreement with Dtack Inc.
  ! Under no circumstances may you redistribute this file.
  ! This software is provided AS IS with no warranty expressed or implied.
  ! Dtack Inc. accepts no liability for use or misuse of this file.
  ! http://www.dtack.com  dtack@dtack.com  telephone +360.417.6700
  ! Dtack Inc, P.O. Box 3061, Port Angeles, WA USA 98362
  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */
