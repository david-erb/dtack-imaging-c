/*..........................................................................
| NAME
| :dtthreads: Function Group - datagram client object
|
| END
 *..........................................................................*/

/*..........................................................................
  CHANGES
  20001003 DE remove pthread support for Solaris
  20000507 DE added to the library
 *..........................................................................*/

#include <dtack/base.h>
DT_RCSID("source/low $RCSfile: threads.c,v $ $Revision: 1.8 $");

#include <dtack/ipc.h>
#include <dtack/list.h>
#include <dtack/str.h>
#include <dtack/os.h>
#include <dtack/thread.h>				/* thread object					*/
#include <dtack/errno.h>

#define DESC "pthread support"

#if defined(DTCONFIG_IMPLIED_SOLARIS)	/* pthread problem with Solaris 	*/

/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtthread_reap(							/* reap cancelled thread			*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtthread_t *thread)					/* the thread to destroy 			*/
{
  DT_F("dtthread_reap");
  return dt_err_notlinked(ctl, F, DESC);
}

/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtthread_destroy(						/* issue cancel request				*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtthread_t *thread)					/* the thread to destroy 			*/
{
  DT_F("dtthread_destroy");
  return dt_err_notlinked(ctl, F, DESC);
}

/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtthread_launch(						/* start threads					*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtthread_t *thread)					/* thread object ready to go 		*/
{
  DT_F("dtthread_launch");
  return dt_err_notlinked(ctl, F, DESC);
}

/*..........................................................................
 *..........................................................................*/
/*..........................................................................
 *..........................................................................*/
#else

#ifdef DTCONFIG_IMPLIED_UNIXLIKE
#include <pthread.h>


/*..........................................................................
 * this is the thread's callback
 *..........................................................................*/

static
void 
thread_cancel(
  void *opaque)
{
  DT_F("dtthread_cancel");
  dtthread_t *thread = (dtthread_t *)opaque;
  dt_ctl_t *ctl = thread->ctl;
  thread->status = DTTHREAD_STATUS_CANCELED;
}

/*..........................................................................
 *..........................................................................*/

static
void *
thread_start(
  void *opaque)
{
  DT_F("dtthread_start");
  dtthread_t *thread = (dtthread_t *)opaque;
  dt_rc_e rc;
  dt_ctl_t *ctl = thread->ctl;

  thread->status =						/* set our launched state 			*/
	DTTHREAD_STATUS_LAUNCHED;

  pthread_cleanup_push(thread_cancel, opaque);
  
  rc = (thread->work)(thread);			/* call the work function 			*/
  
  DT_I(dtos_time,(ctl,					/* remember when thread finished 	*/
    &thread->finish_time));
  thread->rc = rc;
  pthread_cleanup_pop(1); 

  return NULL;
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
  DT_F("dtthread_reap");
  pthread_t pthread;
  void *vrc;
  int sysrc;
  int e;
  dt_rc_e rc = DT_RC_GOOD;

  pthread = (pthread_t)thread->pthread;
  sysrc = pthread_join(					/* get thread return code			*/
    pthread, &vrc);
  e = errno;
  if (sysrc == -1)						/* join call failed?				*/
  {
	rc = dt_err(ctl, F,
      "pthread_join(0x%08lx, &vrc)" 
      DTERRNO_FMT,
      thread->pthread,
      DTERRNO_ARG(e));
	goto cleanup;
  }

cleanup:
  return rc;
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
dtthread_destroy(						/* issue cancel request				*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtthread_t *thread)					/* the thread to destroy 			*/
{
  DT_F("dtthread_destroy");
  pthread_t pthread;
  int sysrc;
  int e;
  dt_rc_e rc = DT_RC_GOOD;

  pthread = (pthread_t)thread->pthread;
  sysrc = pthread_cancel(pthread);
  e = errno;
  if (sysrc == -1)
  {
	rc = dt_err(ctl, F,
      "pthread_cancel()" 
      DTERRNO_FMT,
      DTERRNO_ARG(e));
	goto cleanup;
  }

cleanup:
  return rc;
}

/*..........................................................................
| NAME
| dtthread() - make new thread
|
| SUMMARY
| Thread this threads.
| Add subthreads to list of subthreadses.
| 
| END
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtthread_launch(						/* start threads					*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtthread_t *thread)					/* thread object ready to go 		*/
{
  DT_F("dtthread_launch");
  pthread_t pthread;
  int sysrc;
  int e;
  dt_rc_e rc = DT_RC_GOOD;

  sysrc = pthread_create(&pthread,		/* create OS thread 				*/
    NULL, thread_start, 
    (void *)thread);
  e = errno;
  if (sysrc == -1)
  {
	rc = dt_err(ctl, F,
      "pthread_create()" 
      DTERRNO_FMT,
      DTERRNO_ARG(e));
	goto cleanup;
  }

  thread->pthread =						/* record thread id 				*/
	(unsigned long)pthread;

cleanup:
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
    *rev_string = "dtack unix threads lib";
  if (rev_major != NULL)
    *rev_major = 2;
  if (rev_minor != NULL)
    *rev_minor = 1;
  if (rev_date != NULL)
    *rev_date = 20001011;
  return DT_RC_GOOD;
}


#endif									/* end if UNIX						*/
#endif									/* end if not Solaris				*/
