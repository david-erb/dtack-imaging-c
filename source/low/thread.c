/*..........................................................................
| NAME
| :dtthread: Function Group - thread object
|
| END
 *..........................................................................*/

/*..........................................................................
  CHANGES
  20000507 DE added to the library
 *..........................................................................*/

#include <dtack/base.h>
DT_RCSID("source/low $RCSfile: thread.c,v $ $Revision: 1.8 $");

#include <dtack/str.h>
#include <dtack/os.h>
#include <dtack/thread.h>				/* thread object					*/

/*..........................................................................
| NAME
| dtthread_describe() - make descriptive string
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
dtthread_describe(						/* make descriptive string			*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtthread_t *thread,
  char *string,
  int lmax)
{
  DT_F("dtthread_describe");
  if (!strcmp(thread->status, DTTHREAD_STATUS_LAUNCHING) ||
	  !strcmp(thread->status, DTTHREAD_STATUS_LAUNCHED) ||
	  !strcmp(thread->status, DTTHREAD_STATUS_CANCELED))
  {
	DT_Q(dtstr_printf,(ctl, string, lmax,
      (thread->pid>0x7ffff)?
	    "[%08lx.0x%08lx] %s \"%s\"":
        "[%5d.0x%08lx] %s \"%s\"",
      thread->pid, 
      thread->pthread, 
      thread->status,
      thread->name));
  }
  else
  {
	DT_Q(dtstr_printf,(ctl, string, lmax,
      (thread->pid>0x7ffff)?
	    "[%08lx.0x%08lx] %s \"%s\" (rc=%d)":
        "[%5d.0x%08lx] %s \"%s\" (rc=%d)",
      thread->pid, 
      thread->pthread, 
      thread->status,
      thread->name,
      thread->rc));
  }
  return DT_RC_GOOD;
}
