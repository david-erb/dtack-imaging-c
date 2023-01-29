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

/*----in thread.c:----*/


DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtthread_describe(						/* make descriptive string			*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtthread_t *thread,
  char *string,
  int lmax)
DX_SUFFIX

/*----in threadu.c:----*/


DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtthread_reap(							/* reap cancelled thread			*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtthread_t *thread)					/* the thread to destroy 			*/
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtthread_destroy(						/* issue cancel request				*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtthread_t *thread)					/* the thread to destroy 			*/
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtthread_launch(						/* start threads					*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtthread_t *thread)					/* thread object ready to go 		*/
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtthread_reap(							/* reap cancelled thread			*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtthread_t *thread)					/* the thread to destroy 			*/
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtthread_destroy(						/* issue cancel request				*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtthread_t *thread)					/* the thread to destroy 			*/
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtthread_launch(						/* start threads					*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtthread_t *thread)					/* thread object ready to go 		*/
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtthread_rev(							/* deliver revision 				*/
  dt_ctl_t *ctl,						/* environment control 				*/
  const char **rev_string,				/* returned rev string 				*/
  int *rev_major,						/* returned rev major 				*/
  int *rev_minor,						/* returned rev minor 				*/
  long *rev_date)						/* returned rev date 				*/
DX_SUFFIX

/* ---------------------- */
#undef DT_DEFARGVAL
#undef DX_SUFFIX
#undef DX_PREFIX
#define DT_DEFARGVAL(A)
#ifdef __cplusplus
}
#endif
