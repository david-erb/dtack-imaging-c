#ifdef __cplusplus
extern "C" {
#endif
#ifdef DX_PREFIX
#  undef DX_PREFIX
#endif
#ifndef DT_PREFIX
#  define DX_PREFIX
#else
#  define DX_PREFIX DT_PREFIX
#endif
#ifdef DX_SUFFIX
#  undef DX_SUFFIX
#endif
#ifndef DT_SUFFIX
#  define DX_SUFFIX ;
#else
#  define DX_SUFFIX DT_SUFFIX
#endif
#ifdef DT_DEFARGVAL
#  undef DT_DEFARGVAL
#  define DT_DEFARGVAL(A) = (A)
#endif

/*----in threads.c:----*/


DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtthreads_init(							/* instantiate dtthreads object 	*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtthreads_t *threads,					/* dtthreads object 				*/
  const char *listname)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtthreads_uninit(						/* release dtthreads object 		*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtthreads_t *threads)					/* dtthreads object 				*/
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtthreads_poll(							/* poll state of children 			*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtthreads_t *threads)					/* threads object 					*/
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtthreads_reap(							/* reap first dead threads			*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtthreads_t *threads,					/* dtthreads object 				*/
  dtthread_t *rthread)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtthreads_thread_cancel(				/* issue cancel request				*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtthreads_t *threads,					/* dtthreads object 				*/
  dtthread_t *thread)					/* the thread to cancel 			*/
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtthreads_thread(						/* start threads					*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtthreads_t *threads,					/* dtthreads object 				*/
  const char *name,						/* identifier 						*/
  dtthread_work_f *work,				/* thread work function 			*/
  void *work_arg,						/* work function opaque arg 		*/
  dtthread_t **rthread)					/* return thread object 			*/
DX_SUFFIX

/* ---------------------- */
#undef DT_DEFARGVAL
#undef DX_SUFFIX
#undef DX_PREFIX
#define DT_DEFARGVAL(A)
#ifdef __cplusplus
}
#endif
