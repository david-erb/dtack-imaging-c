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
  20000807 DE experimental MSW32 delay using select() (commented out)
  19980716 DE extra delay time for all Sun platforms
 *..........................................................................*/


/*..........................................................................
| NAME
| Introduction - intro
| 
| DESCRIPTION
| As yet undocumented.
| 
| RETURN VALUES
| Function return value will be DT_RC_GOOD on success.  
| Any other return value indicates failure or noncompletion.
| When DT_RC_GOOD is not returned, any other return values may not be valid.
| 
| ERRORS
| As yet undocumented.
| 
| END
 *..........................................................................*/

#include <dtack/base.h>

DT_RCSID("source/low $RCSfile: os_delay.c,v $ $Revision: 1.1 $");

#include <dtack/os.h>

#include <dtack/msw.h>

#ifdef DTCONFIG_IMPLIED_MSW	
# include <winsock2.h>
#endif

/*..........................................................................*/
static
int
valid(long milliseconds)
{
  if (milliseconds < 0 ||
      milliseconds > 0x7fff)
    return 0;
  return 1;
}

#ifdef DTCONFIG_IMPLIED_QNX4
/*..........................................................................*/
#ifndef DTDEPEND
#include <time.h>
#endif
dt_rc_e
DTCONFIG_API1
dtos_delay(
  dt_ctl_t *ctl,
  long milliseconds)
{
  DT_F("dtos_delay");
  struct timespec ts;
  int sysrc;

  if (!milliseconds)					/* sleep 0 is a no-op			    */
    return DT_RC_GOOD;
  if (!valid(milliseconds))				/* check time for validity		    */
    return dt_err(ctl, F,
      "invalid delay time %ld milliseconds",
      milliseconds);

  ts.tv_sec = milliseconds / 1000L;
  ts.tv_nsec =
    (milliseconds % 1000L) * 1000000L;

  sysrc = nanosleep(&ts, NULL);

  if (sysrc == -1)
    return dt_err(ctl, F,
      "errno %d in nanosleep({%ld,%ld}, NULL)",
      errno,
      ts.tv_sec,
      ts.tv_nsec);


  return DT_RC_GOOD;
}

#else
#ifdef DTCONFIG_IMPLIED_UNIXLIKE
/*..........................................................................*/

#ifndef DTDEPEND
#include <sys/types.h>
#include <sys/time.h>
#include <signal.h>
#endif

dt_rc_e
DTCONFIG_API1
dtos_delay(
  dt_ctl_t *ctl,
  long milliseconds)
{
  DT_F("dtos_delay");
  struct timeval timeout;
  int sysrc;

  if (!milliseconds)					/* sleep 0 is a no-op			    */
    return DT_RC_GOOD;

  if (!valid(milliseconds))				/* check time for validity		    */
    return dt_err(ctl, F,
      "invalid delay time %ld milliseconds",
      milliseconds);

# if defined(DTCONFIG_IMPLIED_SUNOS) ||	/* on Sun, select returns too soon	*/\
     defined(DTCONFIG_IMPLIED_SOLARIS)
   milliseconds += 10;
#endif

  timeout.tv_sec = milliseconds / 1000;
  timeout.tv_usec = (milliseconds % 1000) * 1000;
  
  sysrc = select(0, NULL, NULL, NULL,
    &timeout);

  if (sysrc == -1 &&
	  errno != EINTR)
    return dt_err(ctl, F,
      "errno %d in select",
      errno);

  return DT_RC_GOOD;
}
#else
#ifdef DTCONFIG_IMPLIED_UNIXLIKE_NOSELECT
/*..........................................................................*/

#ifndef DTDEPEND
#include <sys/types.h>
#include <sys/time.h>
#include <signal.h>
#endif

#ifdef DTCONFIG_IMPLIED_SUNOS_OLD
extern int setitimer(int, struct itimerval *, struct itimerval *);
#endif

static void sigalrm_handler(int signo){}

dt_rc_e
DTCONFIG_API1
dtos_delay(
  dt_ctl_t *ctl,
  long milliseconds)
{
  DT_F("dtos_delay");
  struct itimerval value;
  struct sigaction act, oact;
  sigset_t sigalrm, oset;
  int sigwasblocked = 0;
  int sysrc;

  if (!milliseconds)					/* sleep 0 is a no-op			    */
    return DT_RC_GOOD;

#ifdef DTCONFIG_FG1						/* FreeBSD doesn't sleep enough 	*/
#  define EXTRA 10
#else
#  define EXTRA 0
#endif

  if (!valid(milliseconds+EXTRA))		/* check time for validity		    */
    return dt_err(ctl, F,
      "invalid delay time %ld milliseconds",
      milliseconds);

  milliseconds += EXTRA;

  value.it_interval.tv_sec = 0;
  value.it_interval.tv_usec = 0;
  value.it_value.tv_sec = milliseconds / 1000L;
  value.it_value.tv_usec = (milliseconds % 1000L) * 1000L;

  sigemptyset(&sigalrm);				/* make empty signal set		    */
  sigaddset(&sigalrm, SIGALRM);			/* put just our signal in the set   */
  sigprocmask(SIG_BLOCK,				/* block our signal				    */
    &sigalrm, &oset);
  if (sigismember(&oset, SIGALRM))		/* signal was already blocked?	    */
    sigwasblocked = 1;					/* remember for cleanup			    */

  act.sa_handler = sigalrm_handler;
  act.sa_flags = 0;
  sigemptyset(&act.sa_mask);
  sigaction(SIGALRM, &act, &oact);		/* set up our signal handler	    */

  sysrc = setitimer(ITIMER_REAL,		/* set the timer				    */
    &value, NULL);

  if (sysrc == 0)						/* timer got set OK?			    */
  {
    sigdelset(&oset, SIGALRM);			/* our signal unblocked on suspend	*/
    sigsuspend(&oset);					/* sit around waiting for timer		*/
  }

  if (!sigwasblocked)					/* signal was not blocked before?   */
    sigprocmask(SIG_UNBLOCK,			/* unblock our signal			    */
      &sigalrm, &oset);

  sigaction(SIGALRM, &oact, NULL);		/* restore old signal handler	    */

  if (sysrc != 0)
    return dt_err(ctl, F,
      "errno %d in setitimer(ITIMER_REAL, {{0,0},{%ld,%ld}}, NULL)",
      errno,   
      value.it_value.tv_sec,
      value.it_value.tv_usec);

  return DT_RC_GOOD;
}
#else
#ifdef DTCONFIG_IMPLIED_MSW
/*..........................................................................*/
dt_rc_e
DTCONFIG_API1
dtos_delay(
  dt_ctl_t *ctl,
  long milliseconds)
{
  DT_F("dtos_delay");
  dtos_time_t mark1, mark2;
  if (!milliseconds)					/* sleep 0 is a no-op			    */
    return DT_RC_GOOD;
  if (!valid(milliseconds))				/* check time for validity		    */
    return dt_err(ctl, F,
      "invalid delay time %ld milliseconds",
      milliseconds);

#ifdef DTCONFIG_IMPLIED_MSW32_EXPERIMENTAL
  {
    struct timeval timeout;

    timeout.tv_sec = milliseconds / 1000;
    timeout.tv_usec = (milliseconds % 1000) * 1000;
  
    select(0, NULL, NULL, NULL,
      &timeout);
  }
#endif

#ifdef DTCONFIG_IMPLIED_MSW32
  {
    OSVERSIONINFO v;
    v.dwOSVersionInfoSize =				/* initialize the size member 		*/
	  sizeof(OSVERSIONINFO);
	if (GetVersionEx(&v) == FALSE)		/* get os version 					*/
	  return dt_err(ctl, F,
        "GetVersionEx() failed,"
        " GetLastError=%ld (0x%08lx)",
        GetLastError(),
        GetLastError());
    if (v.dwPlatformId !=				/* true 32-bit os? 					*/
		VER_PLATFORM_WIN32s)
	{
	  DT_Q(dtmsw_yield,(ctl));			/* yield with PeekMessage			*/
	  Sleep(milliseconds);				/* easy win32 call 					*/
	  return DT_RC_GOOD;
	}
  }
#endif

  DT_Q(dtos_time,(ctl, &mark1));
  do {
    DT_Q(dtmsw_yield,(ctl));			/* yield with PeekMessage			*/
	DT_Q(dtos_time,(ctl, &mark2));		/* get time at end of delay 		*/
    milliseconds -= DT_CAST(long,		/* subtract actual elapsed time 	*/
      1000.0*(mark2.wall-mark1.wall));
    mark1 = mark2;
  } while(milliseconds > 0.0);
  return DT_RC_GOOD;
}
#else
#if defined(DTCONFIG_MICROSOFT)			/* should be MICROSOFT_DOS 			*/
/*..........................................................................*/
#ifndef DTDEPEND
#  include <time.h>
#  include <sys/timeb.h>
#endif
dt_rc_e
DTCONFIG_API1
dtos_delay(
  dt_ctl_t *ctl,
  long milliseconds)
{
  DT_F("dtos_delay");
  struct _timeb time1, time2;
  long elapsed;
  if (!milliseconds)					/* sleep 0 is a no-op			    */
    return DT_RC_GOOD;
  if (!valid(milliseconds))				/* check time for validity		    */
    return dt_err(ctl, F,
      "invalid delay time %ld milliseconds",
      milliseconds);

  _ftime(&time1);						/* starting time				    */
  do {
    _ftime(&time2);						/* new time						    */
    elapsed =							/* elapsed milliseconds			    */
      (long)(1000.0 * difftime(
         time2.time, time1.time)) +
      (long)time2.millitm - 
      (long)time1.millitm;
  } while(elapsed < milliseconds);		/* busy wait					    */

  return DT_RC_GOOD;
}
#else
#if defined(DTCONFIG_IMPLIED_DOS)
/*..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtos_delay(
  dt_ctl_t *ctl,
  long milliseconds)
{
  DT_F("dtos_delay");
  dtos_time_t mark1, mark2;
  if (!milliseconds)					/* sleep 0 is a no-op			    */
    return DT_RC_GOOD;
  if (!valid(milliseconds))				/* check time for validity		    */
    return dt_err(ctl, F,
      "invalid delay time %ld milliseconds",
      milliseconds);
  DT_Q(dtos_time,(ctl, &mark1));
  do {
    delay((int)(milliseconds));
	DT_Q(dtos_time,(ctl, &mark2));		/* get time at end of delay 		*/
    milliseconds -= DT_CAST(long,		/* subtract actual elapsed time 	*/
      1000.0*(mark2.wall-mark1.wall));
    mark1 = mark2;
  } while(milliseconds > 0.0);
  return DT_RC_GOOD;
}


#else
/*..........................................................................*/
dt_rc_e
DTCONFIG_API1
dtos_delay(
  dt_ctl_t *ctl,
  long milliseconds)
{
  DT_F("dtos_delay");
  if (!milliseconds)					/* sleep 0 is a no-op			    */
    return DT_RC_GOOD;
  if (!valid(milliseconds))				/* check time for validity		    */
    return dt_err(ctl, F,
      "invalid delay time %ld milliseconds",
      milliseconds);
  sleep((int)(milliseconds/1000)+1);	/* do it using sleep			    */
  return DT_RC_GOOD;
}
#endif
#endif
#endif
#endif
#endif
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
