/*..........................................................................
| NAME
| :dtthreads: Function Group - threads list
|
| SUMMARY
| The datagram server object ~threads~ encapsulates the ability
| of a process to send a datagram request and get the reply.
|
| A companion object called ~dgserver~ provides the ability for a process
| to act as a server.
|
| Each instantiation of a client object represents a single datagram
| conversation.
| 
| From the client's point of view, the conversation goes like this:
| 1.	call dtthreads_init() with the server spec and the request string
| 2.	call dtthreads_get_header() to wait for incoming response header
| 3.	call dtthreads_get_line() to get incoming data lines
| 		(assuming the data is formatted as text)
| 6.	go back to dtthreads_uninit() to finish the conversation
|
| END
 *..........................................................................*/

#include <dtack/base.h>
DT_RCSID("source/low $RCSfile: threads.c,v $ $Revision: 1.8 $");

#include <dtack/ipc.h>
#include <dtack/list.h>
#include <dtack/str.h>
#include <dtack/os.h>
#include <dtack/thread.h>				/* thread object					*/
#include <dtack/threads.h>				/* threads manager 					*/
#include <dtack/errno.h>

/*..........................................................................
| NAME
| dtthreads_init() - initialize thread list object
|
| SUMMARY
| Initialize the datagram client object ~threads~.
| Expect ~spec~ to the of the form #scheme,carrier#.
| Have an error if #scheme# is not one of a predefined set.
| Have an error if there is no server at #carrier# or the server
| refuses to talk.
| 
| END
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtthreads_init(							/* instantiate dtthreads object 	*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtthreads_t *threads,					/* dtthreads object 				*/
  const char *listname)
{
  DT_F("dtthreads_init");
  dt_rc_e rc;

  memset(threads, 0, sizeof(*threads));	/* clear threads list 				*/
  
  DT_C(dtlist_create,(ctl,				/* create list of threads			*/
    &threads->list,
    listname));
  threads->created = 1;

cleanup:
  if (rc != DT_RC_GOOD)
  {
	if (threads->created)
	  DT_I(dtlist_destroy,(ctl,
        &threads->list));
	memset(threads, 0, sizeof(*threads));
  }
  return rc;
}

/*..........................................................................
| NAME
| dtthreads_uninit() - uninitialize thread list object
|
| SUMMARY
| Free all resources associated with thread list object.
| Running children will become orphans and/or zombies.
| 
| END
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtthreads_uninit(						/* release dtthreads object 		*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtthreads_t *threads)					/* dtthreads object 				*/
{
  DT_F("dtthreads_init");
  dt_rc_e rc = DT_RC_GOOD;
  if (threads->created)					/* threads list got created? 		*/
  {
	DT_I(dtlist_free,(ctl,				/* free all list entries 			*/
      &threads->list));
	DT_I(dtlist_destroy,(ctl,			/* destroy list itself 				*/
      &threads->list));
  }
  memset(threads, 0, sizeof(*threads));
  return rc;
}

/*..........................................................................
| NAME
| dtthreads_poll() - poll state of children
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
dtthreads_poll(							/* poll state of children 			*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtthreads_t *threads)					/* threads object 					*/
{
  DT_F("dtthreads_poll");
  dtlist_elem_t *elem;
  int e;
  dt_rc_e rc;

  elem = DTLIST_ELEM_HEAD;				/* start at top of list 			*/
  while (1)
  {
	dtthread_t *thread;
	int sysrc;
	int status;
	DT_C(dtlist_next,(ctl,				/* get next thread list entry 		*/
      &threads->list, elem, &elem));
	if (elem == NULL)
	  break;
	thread = (dtthread_t *)				/* address the thread entry 		*/
      elem->user;
	if (!strcmp(thread-> status,		/* we know this is dead already? 	*/
		  DTTHREAD_STATUS_DEAD))
	  continue;
										/* ................................ */
	if (!strcmp(thread->status,			/* thread was canceled? 			*/
		DTTHREAD_STATUS_CANCELED))
	{
	  DT_C(dtthread_reap,(ctl, 
        thread));
	  thread->status =					/* mark thread dead					*/
        DTTHREAD_STATUS_DEAD;
	}
  }

cleanup:
  return rc;
}

/*..........................................................................
| NAME
| dtthreads_reap() - reap first dead threads
|
| SUMMARY
| Scan list of children.
| Return a dtthread_thread_t structure describing the first dead one.
| Remove the dead one from the list.
| If no dead children, then return ~*thread~ NULL.
| The caller must free ~*thread~.
| 
| END
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtthreads_reap(							/* reap first dead threads			*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtthreads_t *threads,					/* dtthreads object 				*/
  dtthread_t *rthread)
{
  DT_F("dtthreads_reap");
  dtlist_elem_t *elem = NULL;
  dtthread_t *thread;
  dt_rc_e rc;

  DT_C(dtthreads_poll,(ctl, threads));	/* check list status				*/

  elem = DTLIST_ELEM_HEAD;				/* start at top of list 			*/
  while (1)
  {
	DT_C(dtlist_next,(ctl,				/* get next thread list entry 		*/
      &threads->list, elem, &elem));
	if (elem == NULL)
	  break;
	thread = (dtthread_t *)				/* address the thread entry 		*/
      elem->user;
	if (!strcmp(thread->status,			/* this one is dead?				*/
		  DTTHREAD_STATUS_DEAD))
	  break;
  }

cleanup:
  if (rc == DT_RC_GOOD && !elem)
	memset(rthread, 0, sizeof(*rthread));
  
  if (rc == DT_RC_GOOD && elem)
  {
	*rthread = *thread;					/* copy reaped thread to caller 	*/
	DT_I(dtos_free2,(ctl, 
      (char *)thread,
      F, "thread structure"));
	DT_I(dtos_free2,(ctl, 
      (char *)elem->name,
      F, elem->name));
	DT_I(dtlist_delete,(ctl,
      &threads->list, elem));

	if (DT_DBG(ctl,
          DT_DBG_MASK_COMMAND))
	{
	  char s[128];
	  DT_I(dtthread_describe,(ctl,		/* make descriptive string 			*/
        rthread, s, sizeof(s)));
	  dt_dbg(ctl, F,					/* issue debug 						*/
 	    DT_DBG_MASK_COMMAND,
        "reaped %s", s);
	}
  }

  return rc;
}

/*..........................................................................
| NAME
| dtthreads_thread_cancel() - cancel thread
|
| SUMMARY
| Cancel ~thread~ thread.
| 
| END
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtthreads_thread_cancel(				/* issue cancel request				*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtthreads_t *threads,					/* dtthreads object 				*/
  dtthread_t *thread)					/* the thread to cancel 			*/
{
  DT_F("dtthreads_thread_cancel");
  dt_rc_e rc = DT_RC_GOOD;

  if (strcmp(thread->status,
	    DTTHREAD_STATUS_LAUNCHED))
	DT_CLEANUP((ctl, F,
      "thread state %s cannot be cancelled", thread->status));
  if (thread->pthread == 0)
	DT_CLEANUP((ctl, F,
      "thread thread 0x%08lx cannot be cancelled",
      thread->pthread));

  DT_C(dtthread_destroy,(ctl, thread));

cleanup:
  return rc;
}

/*..........................................................................
| NAME
| dtthreads_thread() - make new thread
|
| SUMMARY
| Thread this threads.
| Add subthreads to list of subthreadses.
| 
| END
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtthreads_thread(						/* start threads					*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtthreads_t *threads,					/* dtthreads object 				*/
  const char *name,						/* identifier 						*/
  dtthread_work_f *work,				/* thread work function 			*/
  void *work_arg,						/* work function opaque arg 		*/
  dtthread_t **rthread)					/* return thread object 			*/
{
  DT_F("dtthreads_thread");
  dtthread_t *thread = NULL;
  char *elemname;
  int pid;
  dt_rc_e rc = DT_RC_GOOD;

  DT_C(dtos_malloc2,(ctl,				/* space for thread entry 			*/
    (void **)&thread, sizeof(*thread),
    F, "thread structure"));
  memset(thread, 0, sizeof(*thread));

  DT_C(dtos_time,(ctl,					/* remember when thread started 	*/
    &thread->start_time));

  if (!name)							/* caller wants default name? 		*/
	DT_C(dtstr_printf,(ctl, 
      thread->name, sizeof(thread->name),
      "%s.%08ld", 
      threads->list.name,
      threads->nextid))
  else
	DT_C(dtstr_printf,(ctl, 
      thread->name, sizeof(thread->name),
      "%s", name))

  threads->nextid++;					/* bump up next id number 			*/
  
  thread->ctl = ctl;
  DT_C(dtos_getpid,(ctl, &pid));
  thread->pid = pid;
  thread->work = work;
  thread->work_arg = work_arg;

  thread->status =
	DTTHREAD_STATUS_LAUNCHING;

  DT_C(dtthread_launch,(ctl, thread));	/* create OS thread 				*/

  if (DT_DBG(ctl,
	    DT_DBG_MASK_COMMAND))
  {
	char s[128];
	DT_C(dtthread_describe,(ctl,		/* make descriptive string 			*/
      thread, s, sizeof(s)));

	dt_dbg(ctl, F,						/* issue debug 						*/
      DT_DBG_MASK_COMMAND,
      "started %s", s);
  }
	  
  DT_C(dtos_strdup,(ctl, thread->name,	/* make list element name 			*/
    &elemname));

  DT_C(dtlist_after,(ctl,				/* add thread to list 				*/
    &threads->list, DTLIST_ELEM_TAIL,
    elemname, thread, NULL));
  if (rthread)							/* caller wants thread structure? 	*/
	*rthread = thread;

cleanup:
  if (rc != DT_RC_GOOD)
  {
	if (thread != NULL)
	  DT_I(dtos_free2,(ctl, thread,
        F, "thread structure"));
  }
  return rc;
}
