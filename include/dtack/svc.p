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

/*----in svc.c:----*/


DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtsvc_register(							/* register name					*/
  dt_ctl_t *ctl,
  dtsvc_t *svc,
  const char *svcspec,
  unsigned long flags)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtsvc_unregister(						/* unregister structure				*/
  dt_ctl_t *ctl,
  dtsvc_t *svc)
DX_SUFFIX

/*----in svc_ipc.c:----*/


DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtsvc_ipc_getipc(
  dt_ctl_t *ctl,
  dtsvc_t *svc,
  void **vipc)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtsvc_ipc_register(						/* register service name			*/
  dt_ctl_t *ctl,
  dtsvc_t *svc,
  const char *name,
  unsigned long flags)
DX_SUFFIX

/*----in svc_dsk.c:----*/


DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtsvc_dsk_next(							/* return next window name			*/
  dt_ctl_t *ctl,
  dtsvc_t *svc,
  dtlist_elem_t **elem)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtsvc_dsk_register(						/* register service name			*/
  dt_ctl_t *ctl,
  dtsvc_t *svc,
  const char *name,
  unsigned long flags)
DX_SUFFIX

/* ---------------------- */
#undef DT_DEFARGVAL
#undef DX_SUFFIX
#undef DX_PREFIX
#define DT_DEFARGVAL(A)
#ifdef __cplusplus
}
#endif
