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

/*----in eventw.c:----*/


DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtevent_create(							/* create new event					*/
  dt_ctl_t *ctl,						/* environment control				*/
  dtevent_t *event,						/* returned event structure 		*/
  long key)								/* system-wide event id 			*/
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtevent_destroy(						/* release event object 			*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtevent_t *event)						/* event object 					*/
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtevent_access(							/* access new event					*/
  dt_ctl_t *ctl,						/* environment control				*/
  dtevent_t *event,						/* returned event structure 		*/
  long key)								/* system-wide event id 			*/
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtevent_release(						/* release event object 			*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtevent_t *event)						/* event object 					*/
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtevent_set(							/* set event object 				*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtevent_t *event)						/* event object 					*/
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtevent_reset(							/* reset event object 				*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtevent_t *event)						/* event object 					*/
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtevent_pulse(							/* pulse event object 				*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtevent_t *event)						/* event object 					*/
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtevent_wait(							/* wait event object 				*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtevent_t *event,						/* event object 					*/
  long timeout,							/* maximum wait in milliseconds 	*/
  int *set)								/* return true if set 				*/
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtevent_rev(						/* deliver revision 				*/
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
