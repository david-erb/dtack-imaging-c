/*..........................................................................
| NAME
| :dtthread: Function Group - thread object for Win32
|
| END
 *..........................................................................*/

/*..........................................................................
  CHANGES
  20000507 DE added to the library
 *..........................................................................*/

#include <dtack/base.h>
DT_RCSID("source/low $RCSfile: thread.c,v $ $Revision: 1.8 $");

#include <dtack/ipc.h>
#include <dtack/list.h>
#include <dtack/str.h>
#include <dtack/os.h>
#include <dtack/thread.h>				/* thread object					*/
#include <dtack/errno.h>
#include <dtack/msw.h>


#ifdef DTCONFIG_IMPLIED_MSW32

#include <process.h>

/*..........................................................................
 *..........................................................................*/

void 
__cdecl
thread_start(
  void *opaque)
{
  DT_F("dtthread_start");
  dtthread_t *thread = (dtthread_t *)opaque;
  dt_rc_e rc;
  dt_ctl_t *ctl = thread->ctl;


  thread->status =						/* set our launched state 			*/
	DTTHREAD_STATUS_LAUNCHED;

  rc = (thread->work)(thread);			/* do the the thread's work 		*/
  
  DT_I(dtos_time,(ctl,					/* remember when thread finished 	*/
    &thread->finish_time));

  thread->status =
	DTTHREAD_STATUS_CANCELED;
  
  thread->rc = rc;
}

/*..........................................................................
| NAME
| dtthread_destroy() - destroy thread
|
| SUMMARY
| Destroy ~thread~ thread.
| 
| END
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtthread_reap(							/* reap cancelled thread			*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtthread_t *thread)					/* the thread to destroy 			*/
{
  DT_F("dtthread_destroy");
  dt_rc_e rc = DT_RC_GOOD;

cleanup:
  return rc;
}

/*..........................................................................
| NAME
| dtthread_destroy() - destroy thread
|
| SUMMARY
| Destroy ~thread~.
| 
| END
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtthread_destroy(						/* issue cancel request				*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtthread_t *thread)					/* the thread to destroy			*/
{
  DT_F("dtthread_destroy");
  BOOL sysrc;
  int e;
  dt_rc_e rc = DT_RC_GOOD;

  sysrc = TerminateThread((HANDLE)thread->pthread, -1);
  if (sysrc)
  {
	char s[256];
	DWORD e = GetLastError();
	DTERRNO_LASTS(e, s, sizeof(s));
	rc = dt_err(ctl, F,
      "TerminateThread()" 
      DTERRNO_LASTFS,
      DTERRNO_LASTES(e, s));
	goto cleanup;
  }
cleanup:
  return rc;
}

/*..........................................................................
| NAME
| dtthread_launch() - launch new thread
|
| SUMMARY
| Thread this threads.
| Add subthreads to list of subthreadses.
| 
| END
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtthread_launch(						/* launch thread					*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtthread_t *thread)					/* thread object ready to go 		*/
{
  DT_F("dtthread_launch");
  unsigned long pthread;
  dt_rc_e rc = DT_RC_GOOD;

  if (sizeof(pthread) !=  
	  sizeof(thread->pthread))
	return dt_err(ctl, F,
      "thread size %d !="
	  " unsigned long size %d",
      sizeof(thread),
	  sizeof(thread->pthread));

  pthread = _beginthread(				/* create OS thread 				*/
    thread_start, 0,
    (void *)thread);
  if (pthread == (unsigned long)(-1))
  {
	int e = errno;
	rc = dt_err(ctl, F,
      "_begin_thread()" 
      DTERRNO_FMT,
      DTERRNO_ARG(e));
	goto cleanup;
  }

  thread->pthread =						/* record thread id 				*/
	(unsigned long)pthread;

cleanup:
  if (rc != DT_RC_GOOD)
  {
	if (thread != NULL)
	  DT_I(dtos_free2,(ctl, thread,
        F, "thread structure"));
  }
  return rc;
}

/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtthread_rev(							/* deliver revision 				*/
  dt_ctl_t *ctl,						/* environment control 				*/
  const char **rev_string,				/* returned rev string 				*/
  int *rev_major,						/* returned rev major 				*/
  int *rev_minor,						/* returned rev minor 				*/
  long *rev_date)						/* returned rev date 				*/
{
  if (rev_string != NULL)
    *rev_string = "dtack win32 threads lib";
  if (rev_major != NULL)
    *rev_major = 2;
  if (rev_minor != NULL)
    *rev_minor = 1;
  if (rev_date != NULL)
    *rev_date = 20001011;
  return DT_RC_GOOD;
}

#endif									/* end if WIN32						*/
