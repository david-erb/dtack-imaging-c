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

/*----in dsk.c:----*/


DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtdsk_makename(							/* make filename depending on type  */
  dt_ctl_t *ctl,
  const char *dirname,
  int type,
  const char *base,
  char *filename,
  unsigned int max)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtdsk_getlist(
  dt_ctl_t *ctl,
  const char *dirname,
  dtlist_t *list)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtdsk_putlist(
  dt_ctl_t *ctl,
  const char *dirname,
  dtlist_t *list)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtdsk_delete(
  dt_ctl_t *ctl,
  const char *dirname,
  const char *name)
DX_SUFFIX

/* ---------------------- */
#undef DT_DEFARGVAL
#undef DX_SUFFIX
#undef DX_PREFIX
#define DT_DEFARGVAL(A)
#ifdef __cplusplus
}
#endif
