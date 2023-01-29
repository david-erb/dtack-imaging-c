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

/*----in shmu.c:----*/


DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtshm_create(							/* create new shared memory			*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtshm_t *shm,							/* returned shared memory structure	*/
  int id,								/* system-wide shared memory id 	*/
  long length)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtshm_destroy(							/* release shm object 				*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtshm_t *shm)							/* shm object 						*/
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtshm_access(							/* access existing shared memory	*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtshm_t *shm,							/* returned shared memory structure	*/
  int id)								/* system-wide shared memory id 	*/
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtshm_release(							/* release shm object 				*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtshm_t *shm)							/* shm object 						*/
DX_SUFFIX

/* ---------------------- */
#undef DT_DEFARGVAL
#undef DX_SUFFIX
#undef DX_PREFIX
#define DT_DEFARGVAL(A)
#ifdef __cplusplus
}
#endif
