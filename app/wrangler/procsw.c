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
#include <dtack/msw.h>

#include <dtack/ipc.h>
#include <dtack/list.h>
#include <dtack/str.h>
#include <dtack/os.h>
#include <dtprocs.h>
#include <dtack/errno.h>

#ifdef DTCONFIG_IMPLIED_MSW32

/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtprocs_init(							/* instantiate dtprocs object 		*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtprocs_t *procs,						/* dtprocs object 					*/
  const char *listname)
{
  DT_F("dtprocs_init");
  HANDLE lock;
  DWORD e;
  dt_rc_e rc;

  memset(procs, 0, sizeof(*procs));		/* clear process list 				*/
  procs->listname = listname;
  
  DT_C(dtlist_create,(ctl,				/* create list of processes			*/
    &procs->list,
    listname));
  procs->created = 1;
  
										/* ................................ */
  lock = CreateSemaphore(NULL,			/* get us a semaphore 				*/
    0, 1, listname);
  e = GetLastError();
  if (lock == NULL)						/* could not get semaphore?			*/
  {
	rc = dt_err(ctl, F,
	  "CreateSemaphore(NULL, "
      "0, 1, \"%s\")"
      DTERRNO_LASTF,
      listname,
      DTERRNO_LASTE(e));
	goto cleanup;
  }
										/* ................................ */

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
  char s[128];
  int sysrc;
  HANDLE lock = NULL;
  BOOLEAN ok;
  STARTUPINFO sinfo;
  BOOLEAN inherit_handles;
  PROCESS_INFORMATION pinfo;
  HANDLE indup;
  HANDLE outdup;
  DWORD result;
  DWORD e;
  char *elemname;
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

										/* ................................ */
  lock = OpenSemaphore(					/* access the semaphore				*/
    SEMAPHORE_ALL_ACCESS, FALSE,
    procs->listname);
  e = GetLastError();
  if (lock == NULL)						/* could not get semaphore?			*/
  {
	char s[256];
	e = GetLastError();
	DTERRNO_LASTS(e, s, sizeof(s));
	rc = dt_err(ctl, F,
	  "OpenSemaphore("
      "SEMAPHORE_ALL_ACCESS, FALSE, "
      "\"%s\")"
      DTERRNO_LASTFS,
      procs->listname,
      DTERRNO_LASTES(e, s));
	goto cleanup;
  }

  SetEnvironmentVariable("DTPROCS_LOCK", 
    procs->listname);

  sprintf(s, "0x%08lx", GetCurrentProcessId());
  SetEnvironmentVariable("DTPROCS_PPID", s);

  if (inhandle != -1)
  {
    sprintf(s, "0x%08lx", inhandle);
    SetEnvironmentVariable("DTPROCS_INHANDLE", s);
  }
  if (outhandle != -1)
  {
	sprintf(s, "0x%08lx", outhandle);
	SetEnvironmentVariable("DTPROCS_OUTHANDLE", s);
  }

  memset(&sinfo, 0, sizeof(sinfo));
  GetStartupInfo(&sinfo);
  if (inhandle != -1 ||					/* caller is giving a handle?		*/
	  outhandle != -1)
  {
    sinfo.dwFlags = STARTF_USESTDHANDLES;
    sinfo.hStdInput = inhandle;
    sinfo.hStdOutput = outhandle;
	inherit_handles = TRUE;
  }
  else
  {
	inherit_handles = FALSE;
  }
  
  ok = CreateProcess(					/* start new process 				*/
    proc->executable,
    proc->args,
    NULL,								/* process security 				*/
    NULL,								/* thread security 					*/
    inherit_handles,					/* inherit handles					*/
    CREATE_NEW_CONSOLE | 				/* flags 							*/
    CREATE_SUSPENDED,
    NULL,								/* environment 						*/
    NULL,								/* current directory 				*/
    &sinfo,
    &pinfo);
  if (!ok)
  {
	char s[256];
	e = GetLastError();
	DTERRNO_LASTS(e, s, sizeof(s));
	rc = dt_err(ctl, F,
      "CreateProcess("
      "\"%s\", "
      "... inherit_handles=%s, ...)"
      DTERRNO_LASTFS,
      proc->executable,
      inherit_handles? "TRUE": "FALSE",
      DTERRNO_LASTES(e, s));
	goto cleanup;
  }

  DT_I(dtprocs_proc_describe,(ctl,		/* make descriptive string 			*/
    procs, proc, s, sizeof(s)));
  
  dt_dbg(ctl, F, DT_DBG_MASK_COMMAND,	/* issue debug 						*/
    "started (%s, suspended) %s", 
    inherit_handles? 
      "inherit": "no inherit",
    s);

  result = ResumeThread(pinfo.hThread);
  if (result == 0xffffffff)
  {
	char s[256];
	e = GetLastError();
	DTERRNO_LASTS(e, s, sizeof(s));
	rc = dt_err(ctl, F,
      "ResumeThread("
      "0x%08lx)"
      DTERRNO_LASTFS,
      pinfo.hThread,
      DTERRNO_LASTES(e, s));
	goto cleanup;
  }
  
  dt_dbg(ctl, F, DT_DBG_MASK_COMMAND,	/* issue debug 						*/
    "resumed %s", s);

  result = WaitForSingleObject(			/* wait for child's signal			*/
    lock, 
    DTPROCS_TIMEOUT * 1000);
  if (result == WAIT_TIMEOUT)
  {
	rc = dt_err(ctl, F,
      "no ack within %d sec from %s",
      DTPROCS_TIMEOUT, s);
	goto cleanup;
  }

  CloseHandle(pinfo.hProcess);			/* don't need these anymore		    */
  CloseHandle(pinfo.hThread);

  proc->pid = pinfo.dwProcessId;		/* record child starting 			*/
  proc->running = 1;
  DT_C(dtos_strdup,(ctl, name,			/* make list element name 			*/
	&elemname));
  DT_C(dtlist_after,(ctl,				/* add child to list 				*/
    &procs->list, DTLIST_ELEM_TAIL,
    elemname, proc, NULL));
  if (pproc != NULL)					/* caller wants the proc structure? */
	*pproc = proc;

  dt_dbg(ctl, F, DT_DBG_MASK_COMMAND,	/* issue debug 						*/
    "notified by %s", s);

cleanup:
  if (lock != NULL)
	CloseHandle(lock);

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
  HANDLE handle = NULL;
  BOOLEAN ok;
  DWORD exitcode;
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
	handle = OpenProcess(				/* obtain handle to process 		*/
      PROCESS_QUERY_INFORMATION,
      FALSE, proc->pid);
	if (handle == NULL)
	{
	  char s[256];
	  e = GetLastError();
	  DTERRNO_LASTS(e, s, sizeof(s));
	  dt_dbg(ctl, F, DT_DBG_MASK_COMMAND,
        "OpenProcess("
        "PROCESS_QUERY_INFORMATION, "
        "FALSE, "
        "0x%08lx)"
        DTERRNO_LASTFS,
        proc->pid,
        DTERRNO_LASTES(e, s));
	  proc->running = 0;
	  continue;
	}

	ok = GetExitCodeProcess(handle,		/* get process's exit code 			*/
      &exitcode);

	if (ok == FALSE)
	{
	  char s[256];
	  e = GetLastError();
	  DTERRNO_LASTS(e, s, sizeof(s));
	  dt_dbg(ctl, F, DT_DBG_MASK_ANY,
        "GetExitCodeProcess("
        "0x%08lx, &exitcode)"
        DTERRNO_LASTFS,
        handle,
        DTERRNO_LASTES(e, s));
	  proc->running = 0;
	  continue;
	}

	CloseHandle(handle);
	handle = NULL;

	if (exitcode != STILL_ACTIVE)		/* child not running? 				*/
	{
	  proc->running = 0;					
	  continue;
	}
  }

cleanup:
  if (handle != NULL)
	CloseHandle(handle);
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
