/*..........................................................................
| NAME
| :dtthreads: Function Group - thread object
|
| END
 *..........................................................................*/

/*..........................................................................
  CHANGES
  20000507 DE added to the library
 *..........................................................................*/

#define DTTHREAD_STATUS_UNKNOWN ""
#define DTTHREAD_STATUS_DEAD "dead"
#define DTTHREAD_STATUS_LAUNCHING "launching"
#define DTTHREAD_STATUS_LAUNCHED "launched"
#define DTTHREAD_STATUS_CANCELED "canceled"

typedef struct dtthread_t {
  dt_ctl_t *ctl;
  dt_rc_e (*work)(struct dtthread_t *);
  void *work_arg;
  unsigned long pid;
  unsigned long pthread;
  char name[128];
  dtos_time_t start_time;
  dtos_time_t finish_time;
  const char *status;
  dt_rc_e rc;							/* return code 						*/
} dtthread_t;

typedef 
dt_rc_e 
dtthread_work_f(						/* prototype of work function 		*/
  dtthread_t *thread);

#include <dtack/thread.p>
