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

/*----in wins.c:----*/


DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtwins_init(
  dt_ctl_t *ctl,
  dtwins_t *wins,
  dtscr_t *scr)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtwins_uninit(
  dt_ctl_t *ctl,
  dtwins_t *wins)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtwins_dispatch(
  dt_ctl_t *ctl,
  dtwins_t *wins,
  dtsvc_t *svc)
DX_SUFFIX

/*----in wins_ipc.c:----*/


DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtwins_ipc_dispatch(
  dt_ctl_t *ctl,
  dtwins_t *wins,
  dtsvc_t *svc)
DX_SUFFIX

/*----in wins_dsk.c:----*/


DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtwins_dsk_dispatch(
  dt_ctl_t *ctl,
  dtwins_t *wins,
  dtsvc_t *svc)
DX_SUFFIX

/* ---------------------- */
#undef DT_DEFARGVAL
#undef DX_SUFFIX
#undef DX_PREFIX
#define DT_DEFARGVAL(A)
#ifdef __cplusplus
}
#endif
