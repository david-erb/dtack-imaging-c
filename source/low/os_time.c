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
  19990819 DE add dtos_time_str1()
  19980716 DE remove sunpro0 compiler errors
 *..........................................................................*/


#include <dtack/base.h>
DT_RCSID("source/low $RCSfile: os_time.c,v $ $Revision: 1.2 $");
#include <dtack/os.h>

#include <dtack/msw.h>

#include <dtack/errno.h>

#if defined(DTCONFIG_IMPLIED_SUNOS)

extern int sys_nerr;
extern char *sys_errlist[];

#if !defined(DTCONFIG_SP0) && \
    !defined(DTCONFIG_SP1)
const									/* SunPro doesn't like this const	*/
#endif
char *strerror(int e)
{
  if (e >= 0 && e < sys_nerr)
    return sys_errlist[e];
  else
    return "unknown errno";
}

#endif

/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtos_time_delay(						/* delay							*/
  dt_ctl_t *ctl,
  dtos_time_t *since,					/* event time 						*/
  long milli)							/* total delay needed since event 	*/
{
  DT_F("dtos_time_delay");
  dtos_time_t now;
  double elapsed, delay;

  if (milli == 0)						/* no delay necessary? 				*/
	return DT_RC_GOOD;

  DT_Q(dtos_time,(ctl, &now));
  elapsed = now.wall - since->wall;		/* time elapsed since event 		*/
	
  delay = (double)milli / 1000.0;		/* total delay in milliseconds 		*/

  if (elapsed < delay)					/* need to wait longer? 			*/
  {
	long milliseconds = 0.5 +
	  1000.0 * (delay - elapsed);
	DT_Q(dtos_delay,(ctl,				/* actually delay 					*/
      milliseconds));
  }
  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtos_time_str1(							/* print date/time as string		*/
  dt_ctl_t *ctl,
  long date,							/* date as yyyymmdd 				*/
  long time,							/* time as hhmmss 					*/
  char *string,							/* returned string 					*/
  int max)								/* max length of string 			*/
{
  DT_F("dtos_time_str1");
  struct tm t1;
  time_t tt;
  DT_MEMSET(&t1, 0, sizeof(t1));

  t1.tm_year = date / 10000;
  t1.tm_mon = (date - t1.tm_year * 10000) / 100 - 1;
  t1.tm_mday = date % 100;
  t1.tm_hour = time / 10000;
  t1.tm_min = (time - t1.tm_hour * 10000) / 100;
  t1.tm_sec = time % 100;
  t1.tm_year -= 1900;

  tt = mktime(&t1);						/* get tm_wday and tm_yday set 		*/
  t1 = *localtime(&tt);					/* reload all fields 				*/

  strftime(string, max,
    "%A %b %d, %Y at %H:%M.%S",
    &t1);

  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtos_time_long(							/* return current date and time		*/
  dt_ctl_t *ctl,
  long *ldate,							/* date as yyyymmdd 				*/
  long *ltime)							/* time as hhmmss 					*/
{
  DT_F("dtos_time_long");
  time_t tt;
  struct tm t1;

  tt = time(NULL);
  t1 = *localtime(&tt);

  *ldate = 
    (t1.tm_year+1900) * 10000 +
    (t1.tm_mon+1) * 100 +
    t1.tm_mday;

  *ltime =
    t1.tm_hour * 10000 +
    t1.tm_min * 100 +
    t1.tm_sec;

  return DT_RC_GOOD;
}

/*..........................................................................
| NAME
| Introduction - os_time.c
|
| DESCRIPTION
| Time-getting function.
| Various operating system flavors are in this file.
| 
| END
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtos_time_report(
  dt_ctl_t *ctl,
  dtos_time_t *mark1,
  dtos_time_t *mark2,
  const char *f)
{
  DT_F("dtos_time_report");
  char cpu[32];
  char compiler[32];
  char date_stamp[7];

  strcpy(cpu, "cpu");
  strcpy(compiler, "compiler");

  DT_Q(dtos_time_stamp,(ctl,			/* make date stamp string			*/
    date_stamp, NULL));

  dt_dbg(ctl, NULL, DT_DBG_MASK_TIME,
    "| %s | %-24s | %-8s | %-16s | %-8s |%6.2f |%6.2f |%6.2f",
    date_stamp,
    f, DTCONFIG, compiler, cpu,
    mark2->user - mark1->user,
    mark2->sys - mark1->sys,
    mark2->wall - mark1->wall);
  return DT_RC_GOOD;
}

/*..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtos_time_stamp(						/* make date/time stamp strings		*/
  dt_ctl_t *ctl,
  char *date_stamp,
  char *time_stamp)
{
  time_t t;
  struct tm *tm_buf;
  t = time(NULL);
  tm_buf = localtime(&t);

  if (date_stamp != NULL)
  sprintf(date_stamp, "%02d%02d%02d",
    DT_MIN(99, DT_MAX(0, tm_buf->tm_year)),
    DT_MIN(11, DT_MAX(0, tm_buf->tm_mon))+1,
    DT_MIN(31, DT_MAX(0, tm_buf->tm_mday)));

  if (time_stamp != NULL)
  sprintf(time_stamp, "%02d%02d%02d",
    DT_MIN(23, DT_MAX(0, tm_buf->tm_hour)),
    DT_MIN(59, DT_MAX(0, tm_buf->tm_min))+1,
    DT_MIN(59, DT_MAX(0, tm_buf->tm_sec)));

  return DT_RC_GOOD;
}

#if defined(DTCONFIG_IMPLIED_MSW)
/*..........................................................................*/
dt_rc_e
DTCONFIG_API1
dtos_time(								/* store current time			    */
  dt_ctl_t *ctl,
  dtos_time_t *mark)
{
  BOOL boolrc;
  LARGE_INTEGER frequency, counter;

  mark->user =
	mark->sys =
	mark->wall = 
	  (double)GetTickCount() / 1000.0;		/* already in milliseconds, neat 	*/

  boolrc = QueryPerformanceFrequency(&frequency);
  if (boolrc)
	boolrc = QueryPerformanceCounter(&counter);
  if (boolrc)
  {
#if 0
	LONGLONG qwhole, qpartial;
	LARGE_INTEGER whole, partial; 

	qwhole = counter.QuadPart / frequency.QuadPart;	
	qpartial = counter.QuadPart % frequency.QuadPart;

	whole.QuadPart = qwhole;
	partial.QuadPart = qpartial;
#endif
	memcpy(&mark->sys, &frequency, sizeof(frequency));
	memcpy(&mark->user, &counter, sizeof(counter));
  }

  return DT_RC_GOOD;
}

#else

#if defined(DTCONFIG_IMPLIED_DOS)
/*..........................................................................*/
dt_rc_e
DTCONFIG_API1
dtos_time(								/* store current time			    */
  dt_ctl_t *ctl,
  dtos_time_t *mark)
{
  struct dostime_t dostime;
  _dos_gettime(&dostime);
  mark->user   =
  mark->sys =
  mark->wall   = 
    dostime.hour * 3600.0 +
    dostime.minute * 60.0 +
    dostime.second +
    dostime.hsecond / 100.0;

  return DT_RC_GOOD;
}

#else
#if defined(DTCONFIG_OSTIME_GETRUSAGE)	/* includes Linux, FreeBSD, SunOS	*/
/*..........................................................................*/

#ifndef DTDEPEND
#  include <sys/time.h>
#  if defined(DTCONFIG_IMPLIED_SOLARIS_2_4)
#    include <sys/rusage.h>
#  else
#    include <sys/resource.h>
#  endif
#endif

#define ZERO (756152101)				/* around 12:00 noon 12/17/93	    */

#if defined(DTCONFIG_IMPLIED_SUNOS) &&	/* not in this guy's header files!	*/\
    defined(DTCONFIG_IMPLIED_GNU)
int gettimeofday(struct timeval *, struct timezone *);
int getrusage(int, struct rusage *);
#endif 			

/*..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtos_time(								/* store current time			    */
  dt_ctl_t *ctl,
  dtos_time_t *mark)
{
  DT_F("dtos_time");
  int rc, e;
  struct timeval tp;
  struct rusage ru;

  rc = getrusage(RUSAGE_SELF, &ru);
  if (rc == -1)
  {
	int e = errno;
	return dt_err(ctl, F,
      "getrusage(RUSAGE_SELF, &ru)"
      DTERRNO_FMT,
      DTERRNO_ARG(e));
  }

  rc = gettimeofday(&tp, NULL);	
  if (rc == -1)
  {
	int e = errno;
	return dt_err(ctl, F,
      "gettimeofday(&tp, NULL)"
      DTERRNO_FMT,
      DTERRNO_ARG(e));
  }

  mark->user = ru.ru_utime.tv_sec + ru.ru_utime.tv_usec * 1e-6;
  mark->sys  = ru.ru_stime.tv_sec + ru.ru_stime.tv_usec * 1e-6;
  mark->wall = (tp.tv_sec - ZERO) + tp.tv_usec * 1e-6;

  return DT_RC_GOOD;
}

#else
#if defined(DTCONFIG_OSTIME_PIOCUSAGE)
/*..........................................................................*/

#ifndef DTDEPEND
#  include <sys/time.h>
#  include <sys/resource.h>
#  include <sys/procfs.h>
#endif

/*..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtos_time(								/* store current time			    */
  dt_ctl_t *ctl,
  dtos_time_t *mark)
{
  DT_F("dtos_time");
  char proc[32];
  int fd = -1;
  int sysrc;
  prusage_t pru;
  struct timeval tp;
  dt_rc_e rc = DT_RC_GOOD;

  sprintf(proc,"/proc/%ld", getpid());	/* our process space 				*/
  fd = open(proc, O_RDONLY);
  if (fd == -1)
  {
	int e = errno;
	rc = dt_err(ctl, F,
      "open(\"%s\", O_RDONLY)"
      DTERRNO_FMT,
      proc, DTERRNO_ARG(e));
	goto cleanup;
  }
	  
  sysrc = ioctl(fd, PIOCUSAGE, &pru);	/* fetch out prusage structure */
  if (sysrc == -1)
  {
	int e = errno;
	rc = dt_err(ctl, F,
      "ioctl(\"%s\" (fd=%d),"
      " PIOCUSAGE, ...)"
      DTERRNO_FMT,
      proc, fd, DTERRNO_ARG(e));
	goto cleanup;
  }

  mark->user = pru.pr_utime.tv_sec + pru.pr_utime.tv_nsec * 1e-9;
  mark->sys  = pru.pr_stime.tv_sec + pru.pr_stime.tv_nsec * 1e-9;
  mark->wall = pru.pr_rtime.tv_sec + pru.pr_rtime.tv_nsec * 1e-9;

cleanup:
  if (fd != -1)
	close(fd);
  
  return rc;
}

#else
#if defined(DTCONFIG_OSTIME_TIMES)

/*..........................................................................*/

#ifndef DTDEPEND
#  include <sys/times.h>
#  include <limits.h>
#endif

#define ZERO (756152101)				/* around 12:00 noon 12/17/93	    */

/*..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtos_time(								/* store current time			    */
  dt_ctl_t *ctl,
  dtos_time_t *mark)
{
  DT_F("dtos_time");
  clock_t wall, e;
  struct tms tmsbuffer;
  double tick;

  wall = times(&tmsbuffer);
  if (wall == (clock_t)(-1))
    return dt_err(ctl, F, "times() errno %d (%s)",
      e, strerror(e));

  tick = sysconf(_SC_CLK_TCK);
  if (tick == 0.0)
    tick = 1.0;

  mark->user   = (double)tmsbuffer.tms_utime / tick;
  mark->sys    = (double)tmsbuffer.tms_stime / tick;
  mark->wall   = (double)wall / tick;

  return DT_RC_GOOD;
}

#else
  no defined way to get system times
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
