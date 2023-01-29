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

/*----in run.c:----*/


DX_PREFIX
dt_rc_e 
DTCONFIG_API1
dtrun_init(								/* allocate run structure		    */
  dt_ctl_t *ctl,
  dtrun_t *run)
DX_SUFFIX

DX_PREFIX
dt_rc_e 
DTCONFIG_API1
dtrun_uninit(							/* free run structure			    */
  dt_ctl_t *ctl,
  dtrun_t *run)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtrun_deliver_list(						/* blob list delivery routine 		*/
  dt_ctl_t *ctl,
  dtrun_t *run,
  dtrun_blob_t *list,
  dtrun_deliver_f *deliver_callback,
  void *arg)
DX_SUFFIX

/* ---------------------- */
#undef DT_DEFARGVAL
#undef DX_SUFFIX
#undef DX_PREFIX
#define DT_DEFARGVAL(A)
#ifdef __cplusplus
}
#endif
