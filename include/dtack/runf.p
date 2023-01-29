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

/*----in runf.c:----*/


DX_PREFIX
dt_rc_e 
DTCONFIG_API1
dtrunf_alloc_struct(				/* allocate run structure		    */
  dt_ctl_t *ctl,
  dtimage_t *image,
  dtrunf_t *run)
DX_SUFFIX

DX_PREFIX
dt_rc_e 
DTCONFIG_API1
dtrunf_free_struct(				/* free run structure			    */
  dt_ctl_t *ctl,
  dtimage_t *image,
  dtrunf_t *run)
DX_SUFFIX

DX_PREFIX
dt_rc_e 
DTCONFIG_API1
dtrunf_free_runs(					/* free run arrays				    */
  dt_ctl_t *ctl,
  dtimage_t *image,
  dtrunf_t *run)
DX_SUFFIX

DX_PREFIX
dt_rc_e 
DTCONFIG_API1
dtrunf_free_all(					/* free run structure and arrays    */
  dt_ctl_t *ctl,
  dtimage_t *image,
  dtrunf_t *run)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtrunf_scan(						/* scan image for all runs			*/
  dt_ctl_t *ctl,
  dtimage_t *image,
  dtrunf_t *run,
  const double invisible,
  const int flag)
DX_SUFFIX

DX_PREFIX
dt_rc_e 
DTCONFIG_API1
dtrunf_label(							/* label all runs					*/
  dt_ctl_t *ctl,
  dtimage_t *image,
  unsigned long flags,
  dtrunf_t *run)
DX_SUFFIX

/* ---------------------- */
#undef DT_DEFARGVAL
#undef DX_SUFFIX
#undef DX_PREFIX
#define DT_DEFARGVAL(A)
#ifdef __cplusplus
}
#endif
