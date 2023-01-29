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

/*----in dsk_c.c:----*/


DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtdsk_getwin(							/* get info on existing window	    */
  dt_ctl_t *ctl,
  const char *server,
  const char *id,
  dtdsk_win_t *win)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtdsk_clnt_create_window(
  dt_ctl_t *ctl,
  const char *server,
  const char *id,
  int x0,
  int y0,
  int xe,
  int ye,
  int bits)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtdsk_clnt_export_data(
  dt_ctl_t *ctl,
  const char *server,
  const char *id,
  int x0,
  int y0,
  int xe,
  int ye,
  int bits,
  const void *data)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtdsk_clnt_import_data(
  dt_ctl_t *ctl,
  const char *server,
  const char *id,
  int x0,
  int y0,
  int xe,
  int ye,
  void *data)
DX_SUFFIX

/* ---------------------- */
#undef DT_DEFARGVAL
#undef DX_SUFFIX
#undef DX_PREFIX
#define DT_DEFARGVAL(A)
#ifdef __cplusplus
}
#endif
