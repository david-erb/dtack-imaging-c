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

/*----in semu.c:----*/


DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtsem_create(							/* create new semaphore				*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtsem_t *sem,							/* returned semaphore structure 	*/
  int key)								/* system-wide semaphore id 		*/
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtsem_destroy(							/* release sem object 				*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtsem_t *sem)							/* sem object 						*/
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtsem_post(								/* post sem object 					*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtsem_t *sem,							/* sem object 						*/
  int count)							/* post count 						*/
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtsem_wait(								/* wait sem object 					*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtsem_t *sem,							/* sem object 						*/
  int count)							/* wait count 						*/
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtsem_get_count(						/* get current semaphore count		*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtsem_t *sem,							/* returned semaphore structure 	*/
  int *count)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtsem_get_waiters(						/* get current semaphore waiters		*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtsem_t *sem,							/* returned semaphore structure 	*/
  int *waiters)
DX_SUFFIX

/* ---------------------- */
#undef DT_DEFARGVAL
#undef DX_SUFFIX
#undef DX_PREFIX
#define DT_DEFARGVAL(A)
#ifdef __cplusplus
}
#endif
