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

/*----in winc.c:----*/


DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtwinc_init(							/* init structure					*/
  dt_ctl_t *ctl,
  dtwinc_t *winc,
  const char *init)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtwinc_uninit(							/* init structure					*/
  dt_ctl_t *ctl,
  dtwinc_t *winc)
DX_SUFFIX

/*----in winc_ipc.c:----*/


DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtwinc_ipc_init(
  dt_ctl_t *ctl,
  dtwinc_t *winc)
DX_SUFFIX

/*----in winc_dsk.c:----*/


DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtwinc_dsk_init(
  dt_ctl_t *ctl,
  dtwinc_t *winc)
DX_SUFFIX

/* ---------------------- */
#undef DT_DEFARGVAL
#undef DX_SUFFIX
#undef DX_PREFIX
#define DT_DEFARGVAL(A)
#ifdef __cplusplus
}
#endif
