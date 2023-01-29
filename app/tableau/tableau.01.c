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
#include <dtack/main.h>
#include <dtack/errno.h>

#include <reverb.h>

#include <tableau.use>

#define ACCEPT_TIMEOUT (30000)

#define dt_interactive_parse			/* don't use dialog 				*/\
  dt_enter_parse
#define dt_interactive_leave			/* don't use dialog 				*/\
  dt_leave
#define LEAVE_ARGS param

static
dtrev_f *revs[] = {
  tableau_rev,
  dtfb_vfw_rev,
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

/*..........................................................................
 *..........................................................................*/

static
dt_rc_e
tableau_setup(
  dtparam_t *param,
  dtipc_t *ipc,
  const char *getspec,
  tableau_get_t *get,
  const char *putspec,
  tableau_put_t *put)
{
  DT_F("tableau_setup");
  dt_ctl_t *ctl = &param->ctl;
  tableau_command_t *command = NULL;
  char *s = NULL;
  tableau_ack_t ack;
  dt_rc_e rc;
  
  memset(&ack, 0, sizeof(ack));
  
  DTCSTACK_HIGHWATER;

  DT_C(dtipc_recv,(ctl, ipc,			/* first receive from client	    */
    (void **)&command,
    sizeof(*command)));

  command->version = DTIPC_FIX(DT_C,
    ctl, ipc, command->version);

  param->offset = DTIPC_FIX(DT_C, ctl,
    ipc, command->frame);

  dt_dbg(ctl, F, DT_DBG_MASK_COMMAND,
    "command: %s %s version=%04ld frame=%ld",
    command->command,
    command->name,
    command->version, 
    command->frame);

  if (command->version !=				/* check sender's version 			*/
	  TABLEAU_VERSION)
  {
	ack.rc = (tableau_len_t)			/* special ack tells sender problem */
      TABLEAU_VERSION;
	rc = dt_err(ctl, F,
	  "sender version %04ld"
      " does not match our version %04ld",
      command->version,
	  TABLEAU_VERSION);
	goto cleanup;
  }

# define IS(CMD) \
    (!strcmp(CMD, command->command))
										/* ................................ */
  if (IS("get"))						/* client is sending data			*/
  {
	get->u.ipc.ipc = ipc;				/* server gets data from this ipc 	*/
	get->function = tableau_get_ipc;
	get->free = tableau_get_ipc_free;
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
	put->u.ipc.ipc = ipc;				/* server puts data from this ipc 	*/
	put->function = tableau_put_ipc;
	put->isserver = 1;
    dt_dbg(ctl, F, DT_DBG_MASK_COMMAND,
      "get from \"%s\", put to ipc",
      s);
  }
  else									/* ................................ */
  {
	rc = dt_err(ctl, F,
      "unknown command");
  }

cleanup:
  {
	if (ack.rc == 0)					/* rc not already set? 				*/
	  ack.rc = (tableau_len_t)rc;
	DT_I(dtipc_send,(ctl, ipc,			/* ack the command 					*/
      &ack, sizeof(ack)));
  }

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
tableau_reverb(
  dtparam_t *param,
  char *host,
  int max)
{
  DT_F("tableau_reverb");
  dt_ctl_t *ctl = &param->ctl;
  char *reverb = NULL;
  dt_rc_e rc = DT_RC_GOOD;

  if (param->ident != NULL &&			/* supposed to use reverb server?	*/
	  strlen(param->ident) != 0 &&
	  strcmp(param->ident, "none"))
  {
	char *ident;

#   define P "rocket,"
#   define L (strlen(P))
	DT_C(dtos_malloc,(ctl,
      (void **)&reverb,
      strlen(param->ident) + L + 1));
	strcpy(reverb, P);					/* prepend socket before ipc spec 	*/
	strcat(reverb, param->ident);
  
	ident = strchr(reverb, '+');
	if (ident == NULL)					/* reverb name not given? 			*/
	{
	  (void)dt_err(ctl, F,
        "no ident given in reverb"
        " %s", reverb);
	}
	else								/* reverb name given 				*/
	{
	  *ident++ = '\0';
	  DT_G(reverb_client,(ctl,			/* contact reverb server 			*/
        reverb, ident,
        DTIPC_FLAG_TRY_10 |
        DTIPC_FLAG_TRY_DELAY_10,
        host, max));
	}
  }
  else
    host[0] = '\0';
  
cleanup:
  if (reverb != NULL)
    DT_I(dtos_free2,(ctl, reverb,
      F, "reverb"));

  return rc;
}

/*..........................................................................
 *..........................................................................*/

dt_rc_e
tableau_server_transact(
  dtparam_t *param,
  dtipc_t *ipc,
  const char *getspec,
  const char *putspec)
{
  DT_F("tableau_server");
  dt_ctl_t *ctl = &param->ctl;
  tableau_get_t _get, *get = &_get;
  tableau_put_t _put, *put = &_put;
  dt_rc_e rc;
   
  memset(get, 0, sizeof(*get));
  memset(put, 0, sizeof(*put));

  DTCSTACK_HIGHWATER;

  DT_C(tableau_setup,(param,			/* set up get and put				*/
    ipc,
    getspec, get,
    putspec, put));

  DT_C(tableau_shovel,(param,			/* shovel bits 						*/
    param->cache,						/* dgserver 						*/
    get,  
    param->op, 
    param->variant, 
    put));

cleanup:
  if (get->shoveled != put->shoveled)
    dt_dbg(ctl, F, DT_DBG_MASK_USR1,
      "server to %s shoveled"
      " %ld bytes in and %ld bytes out",
      ipc->client_spec, 
      get->shoveled, put->shoveled);
  else
  if (get->shoveled != 0)
    dt_dbg(ctl, F, DT_DBG_MASK_USR1,
      "server to %s shoveled"
      " %ld bytes",
      ipc->client_spec, get->shoveled); 
    
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
  dt_rc_e rc;
 
  fd.fd = -1;

  DTCSTACK_HIGHWATER;

  memset(ipc, 0, sizeof(*ipc));
  p = strchr(ipcspec, ':');				/* find colon in listen spec 		*/
  if (p == NULL ||						/* no colon? 						*/
	  strcmp(p, ":0"))					/* or port other than 0? 			*/
  {
	dt_dbg(ctl, F, DT_DBG_MASK_USR1,
      "momma server"
      " beginning operation");
	DT_C(dtipc_register,(ctl, ipc,		/* register as forking momma server	*/
      ipcspec, 0,
      DTIPC_FLAG_FORK_AFTER_WAIT));
	ipc_registered = 1;
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
    param, ipc, getspec, putspec));

cleanup:
  if (ipc_accepted)
    DT_I(dtipc_done,(ctl, ipc));		/* server finishes transaction	    */
  if (ipc_registered)
    DT_I(dtipc_unregister,(ctl, ipc));	/* server closes connection			*/

  return rc;
}

/*..........................................................................
 *..........................................................................*/

dt_rc_e
tableau_exclusive(
  dtparam_t *param,
  const char *ipcspec,
  const char *getspec,
  const char *putspec)
{
  DT_F("tableau_server");
  dt_ctl_t *ctl = &param->ctl;
  dtipc_t _ipc, *ipc = &_ipc;
  int ipc_registered = 0;
  int ipc_accepted = 0;
  dt_rc_e rc;

  DTCSTACK_HIGHWATER;

  memset(ipc, 0, sizeof(*ipc));
  
  dt_dbg(ctl, F, DT_DBG_MASK_USR1,
    "exclusive server"
    " beginning operation");
  DT_C(dtipc_register,(ctl, ipc,		/* register as non-forking server	*/
    ipcspec, 0,
    DTIPC_FLAG_FORK_AFTER_WAIT));
  ipc_registered = 1;

  while(1)								/* loop handling transactions 		*/
  {
	DT_C(dtipc_wait,(ctl, ipc));		/* wait for client connection		*/
	ipc_accepted = 1;

    dt_dbg(ctl, F, DT_DBG_MASK_USR1,
      "exclusively serving %s",
      ipc->client_spec);

    DT_G(tableau_server_transact,(		/* shovel							*/
      param, ipc, getspec, putspec));
	if (rc == DT_RC_BAD)				/* ignore shovel error 				*/
	  rc = DT_RC_GOOD;
	if (rc != DT_RC_GOOD)
	  goto cleanup;

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
  long dbg_mask;
  char host[64];
  dt_rc_e rc;
  
  memset(get, 0, sizeof(*get));
  memset(put, 0, sizeof(*put));

										/* ................................ */
  dbg_mask = ctl->dbg_mask;				/* save mask 						*/
  ctl->dbg_mask |= DT_DBG_MASK_USR1;	/* make sure we get this message 	*/
  DT_G(tableau_reverb,(param, host,		/* contact reverb server 			*/
    sizeof(host)));
  if (rc != DT_RC_GOOD)
    dt_say(ctl, F,
      "WARNING: failed to register"
      " with reverb server");
  ctl->dbg_mask = dbg_mask;				/* restore mask 					*/
										/* ................................ */

  DT_C(tableau_get_setup,(param,		/* set up source 					*/
    get, getspec));

  DT_C(tableau_put_setup,(param,		/* set up sink 						*/
    put, putspec));

  DT_C(tableau_shovel,(param,			/* shovel bits 						*/
    param->cache,						/* dgserver 						*/
    get, 
    param->op,
    param->variant,
    put));

cleanup:
  if (get->shoveled != put->shoveled)
    dt_dbg(ctl, F, DT_DBG_MASK_USR1,
      "client shoveled"
      " %ld bytes in and %ld bytes out",
      get->shoveled, put->shoveled);
  else
  if (get->shoveled != 0)
    dt_dbg(ctl, F, DT_DBG_MASK_USR1,
      "client shoveled"
      " %ld bytes",
      get->shoveled); 

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

static
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
  
DTMAIN2(tableau2)
{
  DT_F("tableau");
  dt_ctl_t *ctl = &param->ctl;
  dt_enter_arg_t arg[1];
  char *ipcspec = NULL;
  dt_rc_e rc;

  {
	int pid;
	ctl->progname = argv[0];			/* make sure our name is correct 	*/
	DT_Q(dtos_getpid,(ctl, &pid));
  }

  DT_Q(dt_heapmon_init,(ctl));			/* init memory checking			    */

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
    &param->max, "Frame wait ms",  NULL, "180000",
    &param->omega, "Ack wait ms",  NULL, "0",
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
      "Reverb", NULL, 
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
    arg, tableau_arg_check, NULL));
  
										/* ................................ */
  ctl->dbg = tableau_dbg;				/* special logging needs 			*/
  ctl->err = tableau_err;
  ctl->say = tableau_say;

										/* ................................ */

										/* ................................ */
  if (!strncmp(param->op, "test", 4))	/* op is test? 						*/
  {
	DT_C(tableau_test,(param)); 
  }
  else
  if (!strcmp(param->op, "rev"))		/* op is to say rev?				*/
  {
	DT_C(dtrev_say,(ctl, revs)); 
  }
  else
  if (param->connect != NULL)			/* op is server?					*/
  {
	char *c;
	DT_C(dtos_strdup,(ctl, 
      param->connect, &ipcspec));
	c = ipcspec + strlen(ipcspec) - 1;
	if (*c == '!')						/* last char of connect is '!'? 	*/
	{
	  *c = '\0';
	  DT_C(tableau_exclusive,(param,	/* run exclusive server 			*/
        ipcspec,
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
