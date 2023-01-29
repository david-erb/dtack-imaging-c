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
  19991109 don't care abount unacknowledged dead child
 *..........................................................................*/


/*..........................................................................
| NAME
| :dtprocs: Function Group - datagram client object
|
| SUMMARY
| The datagram server object ~procs~ encapsulates the ability
| of a process to send a datagram request and get the reply.
|
| A companion object called ~dgserver~ provides the ability for a process
| to act as a server.
|
| Each instantiation of a client object represents a single datagram
| conversation.
| 
| From the client's point of view, the conversation goes like this:
| 1.	call dtprocs_init() with the server spec and the request string
| 2.	call dtprocs_get_header() to wait for incoming response header
| 3.	call dtprocs_get_line() to get incoming data lines
| 		(assuming the data is formatted as text)
| 6.	go back to dtprocs_uninit() to finish the conversation
|
| END
 *..........................................................................*/

#include <dtack/base.h>
DT_RCSID("source/low $RCSfile: procs.c,v $ $Revision: 1.8 $");

#include <dtack/ipc.h>
#include <dtack/list.h>
#include <dtack/str.h>
#include <dtack/os.h>
#include <dtprocs.h>
#include <dtack/errno.h>

#ifdef DTCONFIG_IMPLIED_UNIXLIKE
#include <signal.h>
#include <sys/wait.h>

/*..........................................................................*/
static int signal_number = 0;
static
void signal_handler(int signo)
{
  signal_number = signo;
}

/*..........................................................................
| NAME
| dtprocs_init() - initialize proc list object
|
| SUMMARY
| Initialize the datagram client object ~procs~.
| Expect ~spec~ to the of the form #scheme,carrier#.
| Have an error if #scheme# is not one of a predefined set.
| Have an error if there is no server at #carrier# or the server
| refuses to talk.
| 
| END
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtprocs_init(							/* instantiate dtprocs object 		*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtprocs_t *procs,						/* dtprocs object 					*/
  const char *listname)
{
  DT_F("dtprocs_init");
  dt_rc_e rc;

  memset(procs, 0, sizeof(*procs));		/* clear process list 				*/
  
  DT_C(dtlist_create,(ctl,				/* create list of processes			*/
    &procs->list,
    listname));
  procs->created = 1;

cleanup:
  if (rc != DT_RC_GOOD)
  {
	if (procs->created)
	  DT_I(dtlist_destroy,(ctl,
        &procs->list));
	memset(procs, 0, sizeof(*procs));
  }
  return rc;
}

/*..........................................................................
| NAME
| dtprocs_uninit() - uninitialize proc list object
|
| SUMMARY
| Free all resources associated with proc list object.
| Running children will become orphans and/or zombies.
| 
| END
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtprocs_uninit(							/* release dtprocs object 			*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtprocs_t *procs)						/* dtprocs object 					*/
{
  DT_F("dtprocs_init");
  dt_rc_e rc = DT_RC_GOOD;
  if (procs->created)					/* procs list got created? 			*/
  {
	DT_I(dtlist_free,(ctl,				/* free all list entries 			*/
      &procs->list));
	DT_I(dtlist_destroy,(ctl,			/* destroy list itself 				*/
      &procs->list));
  }
  memset(procs, 0, sizeof(*procs));
  return rc;
}

/*..........................................................................
| NAME
| dtprocs_proc_describe() - make descriptive string
|
| SUMMARY
| Wait for next response line from server.
| Wait a maximum of ~milliseconds~.
| Return ~*is_ready~ non-zero if a line can be read.
| 
| END
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtprocs_proc_describe(					/* make descriptive string			*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtprocs_t *procs,						/* dtprocs object 					*/
  dtprocs_proc_t *proc,
  char *string,
  int lmax)
{
  DT_F("dtprocs_proc_describe");
  DT_Q(dtstr_printf,(ctl, string, lmax,
    "\"%s\" (%s)",
    proc->name, proc->executable));
  return DT_RC_GOOD;
}

/*..........................................................................
| NAME
| dtprocs_start() - start process
|
| SUMMARY
| Wait for next response line from server.
| Wait a maximum of ~milliseconds~.
| Return ~*is_ready~ non-zero if a line can be read.
| 
| END
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtprocs_start(							/* start process					*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtprocs_t *procs,						/* dtprocs object 					*/
  const char *name,
  const char *executable,
  const char *args,
  int inhandle,
  int outhandle,
  dtprocs_goodbye_f *goodbye,
  void *goodbye_arg,
  dtprocs_proc_t **pproc)
{
  DT_F("dtprocs_proc_start");
  dtprocs_proc_t *proc = NULL;
  sigset_t sigusr1, oset;
  struct sigaction act, oldact, oldaact;
  char s[128];
  int sysrc;
  int e;
  char *a = NULL;
  dt_rc_e rc = DT_RC_GOOD;

  DT_C(dtos_malloc2,(ctl,				/* space for proc entry 			*/
    (void **)&proc, sizeof(*proc),
    F, "proc structure"));
  memset(proc, 0, sizeof(*proc));
  
  DT_C(dtos_time,(ctl,					/* remember when child started 		*/
    &proc->start_time));
  DT_C(dtstr_printf,(ctl, 
    proc->name, sizeof(proc->name),
    "%s", name));
  DT_C(dtstr_printf,(ctl, 
    proc->executable, 
    sizeof(proc->executable),
    "%s", executable));
  DT_C(dtstr_printf,(ctl, 
    proc->args, sizeof(proc->args),
    "%s", args));
  
  sigprocmask(SIG_BLOCK,				/* query list of blocked signals	*/
    NULL, &sigusr1);
  sigaddset(&sigusr1, SIGUSR1);			/* block our signal too				*/
  sigprocmask(SIG_SETMASK,
    &sigusr1, &oset);
  sigdelset(&sigusr1, SIGUSR1);			/* unblock our signal at pause		*/
  act.sa_handler = signal_handler;
  act.sa_flags = 0;
  sigemptyset(&act.sa_mask);
  sigaction(SIGUSR1, &act, &oldact);	/* set up our signal handler	    */
  
  sysrc = fork();
  e = errno;
  if (sysrc == -1)
  {
	rc = dt_err(ctl, F,
      "fork()" 
      DTERRNO_FMT,
      DTERRNO_ARG(e));
	goto cleanup;
  }
  else
  if (sysrc != 0)						/* we are the parent?			    */
  {
	int old_alarm;
	char *elemname;

	DT_I(dtprocs_proc_describe,(ctl,	/* make descriptive string 			*/
      procs, proc, s, sizeof(s)));

	dt_dbg(ctl, F, DT_DBG_MASK_COMMAND,	/* issue debug 						*/
      "started %s", s);

	signal_number = 0;
	sigaction(SIGALRM, &act, &oldaact);	/* handle alarm too					*/
	old_alarm = alarm(DTPROCS_TIMEOUT);
	sigsuspend(&sigusr1);				/* sit around waiting for signal    */
	sigaction(SIGALRM,					/* replace original alarm handler	*/
      &oldaact, NULL);
	alarm(old_alarm);					/* resume old alarm timer 			*/
	sigprocmask(SIG_SETMASK,			/* replace original signal state	*/
      &oset, NULL);
	sigaction(SIGUSR1,					/* replace original handler    		*/
      &oldact, NULL);

	if (signal_number == SIGALRM)
	{
	  rc = dt_err(ctl, F,
        "no ack within %d sec from %s",
        DTPROCS_TIMEOUT, s);
	  goto cleanup;
	}

	if (signal_number != SIGUSR1)
	{
	  rc = dt_err(ctl, F,
        "unexpected signal %d",
        signal_number);
	  goto cleanup;
	}
	  
	proc->pid = sysrc;					/* record child starting 			*/
	proc->running = 1;
    DT_C(dtos_strdup,(ctl, name,		/* make list element name 			*/
	  &elemname));
	DT_C(dtlist_after,(ctl,				/* add child to list 				*/
      &procs->list, DTLIST_ELEM_TAIL,
      elemname, proc, NULL));
	if (pproc != NULL)					/* caller wants the proc structure? */
	  *pproc = proc;

	dt_dbg(ctl, F, DT_DBG_MASK_COMMAND,	/* issue debug 						*/
      "notified by %s", s);
  }
  else									/* we are the child? 				*/
  {
#   define MAXARGV (64+2)
	int argc;
	char *argv[MAXARGV];

	sigprocmask(SIG_SETMASK,			/* replace original signal state	*/
      &oset, NULL);
	sigaction(SIGUSR1,					/* replace original handler    		*/
      &oldact, NULL);

	if (goodbye != NULL)				/* caller has goodbye function? 	*/
	  goodbye(ctl, goodbye_arg);		/* run caller's goodbye function 	*/
	
	DT_C(dtos_strdup,(ctl, args, &a));	/* space to edit in 				*/
	DT_C(dtstr_argv,(ctl, a,			/* separate tokens 					*/
      argv+1, MAXARGV-2, &argc));
	if (argc > MAXARGV-2)				/* too many tokens? 				*/
	{
	  rc = dt_err(ctl, F,
        "more than %d tokens in arg",
        MAXARGV-2);
	  goto cleanup;
	}
	argv[0] = (char *)executable;		/* conform to execv convention 		*/
	argv[argc+1] = NULL;

	DT_I(dtprocs_proc_describe,(ctl,	/* make descriptive string 			*/
      procs, proc, s, sizeof(s)));
	dt_dbg(ctl, F, DT_DBG_MASK_COMMAND,	/* issue debug 						*/
      "child as %s", s);
	  
	sysrc = execv(executable, argv);	/* exec us 							*/
	e = errno;
	if (sysrc == -1)
	{
	  rc = dt_err(ctl, F,
        "exec(\"%s\", \"%s\" ...[%d])" 
        DTERRNO_FMT,
	    executable, 
        argv[1]? argv[1]: "NULL",
        argc+1,
        DTERRNO_ARG(e));
	  goto cleanup;
	}
  }

cleanup:
  if (a != NULL)
	DT_I(dtos_free2,(ctl, a,
      F, "args dup"));
  if (rc != DT_RC_GOOD)
  {
	if (proc != NULL)
	  DT_I(dtos_free2,(ctl, proc,
        F, "proc structure"));
  }
  return rc;
}

/*..........................................................................
| NAME
| dtprocs_poll() - poll state of children
|
| SUMMARY
| Wait for next response line from server.
| If no response line is currently available, then block.
| Allocate space at ~*line~ and place the line there.
| Trim off a trailing CRLF.
| Rely on the caller to free this space.
|
| Return ~*line~ as NULL if the server has terminated the conversation.
| 
| Have an error if the response line does not contain a CRLF.
| 
| END
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtprocs_poll(							/* poll state of children 			*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtprocs_t *procs)						/* dtprocs object 					*/
{
  DT_F("dtprocs_poll");
  dtlist_elem_t *elem;
  int e;
  dt_rc_e rc;

  elem = DTLIST_ELEM_HEAD;				/* start at top of list 			*/
  while (1)
  {
	dtprocs_proc_t *proc;
	int sysrc;
	int status;
	DT_C(dtlist_next,(ctl,				/* get next proc list entry 		*/
      &procs->list, elem, &elem));
	if (elem == NULL)
	  break;
	proc = (dtprocs_proc_t *)			/* address the proc entry 			*/
      elem->user;
	if (!proc->running)					/* we know this is dead already? 	*/
	  continue;
	sysrc = waitpid(proc->pid,			/* check proc status by pid 		*/
      &status, WNOHANG);
	e = errno;
    if (e == ECHILD)					/* no such child?				    */
	  sysrc = -proc->pid;
	if (sysrc == -1)					/* waitpid call failed? 			*/
	{
	  rc = dt_err(ctl, F,
        "waitpid(%d, &status, WNOHANG)" 
        DTERRNO_FMT,
        proc->pid,
        DTERRNO_ARG(e));
	  goto cleanup;
	}
	if (sysrc == 0)						/* child still running? 			*/
	  continue;
	dt_dbg(ctl, F, DT_DBG_MASK_COMMAND,
      "waitpid(%d) returned %d",
      proc->pid, sysrc);
	proc->running = 0;					/* child not running 				*/
  }

cleanup:
  return rc;
}

/*..........................................................................
| NAME
| dtprocs_reap() - reap first dead process
|
| SUMMARY
| Scan list of children.
| Return a dtproc_proc_t structure describing the first dead one.
| Remove the dead one from the list.
| If no dead children, then return ~*proc~ NULL.
| The caller must free ~*proc~.
| 
| END
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtprocs_reap(							/* reap first dead process			*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtprocs_t *procs,						/* dtprocs object 					*/
  dtprocs_proc_t **proc)
{
  DT_F("dtprocs_reap");
  dtlist_elem_t *elem = NULL;
  dt_rc_e rc;

  DT_C(dtprocs_poll,(ctl, procs));		/* check list status				*/

  elem = DTLIST_ELEM_HEAD;				/* start at top of list 			*/
  while (1)
  {
	DT_C(dtlist_next,(ctl,				/* get next proc list entry 		*/
      &procs->list, elem, &elem));
	if (elem == NULL)
	  break;
	*proc = (dtprocs_proc_t *)			/* address the proc entry 			*/
      elem->user;
	if (!(*proc)->running)				/* this one is dead?				*/
	  break;
  }

cleanup:
  if (elem == NULL)						/* nobody to reap? 					*/
	*proc = NULL;
  else
  if (rc == DT_RC_GOOD)
  {
	char s[128];
	DT_I(dtos_free2,(ctl, 
      (char *)elem->name,
      F, "elem->name"));
	DT_I(dtlist_delete,(ctl,
      &procs->list, elem));
	DT_I(dtprocs_proc_describe,(ctl,	/* make descriptive string 			*/
      procs, *proc, s, sizeof(s)));
	dt_dbg(ctl, F, DT_DBG_MASK_COMMAND,	/* issue debug 						*/
      "reaped %s", s);
  }

  return rc;
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
