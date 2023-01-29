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

/*----in phys.c:----*/


DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtphys_open(							/* open a file						*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtphys_t *phys,						/* phys object descriptor 			*/
  long physaddr,
  long size)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtphys_close(							/* close and release				*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtphys_t *phys)						/* phys object descriptor 			*/
DX_SUFFIX

/* ---------------------- */
#undef DT_DEFARGVAL
#undef DX_SUFFIX
#undef DX_PREFIX
#define DT_DEFARGVAL(A)
#ifdef __cplusplus
}
#endif
