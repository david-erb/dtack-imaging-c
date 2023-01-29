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

/*----in alpha.c:----*/


DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtalpha_begin(
  dt_ctl_t *ctl,
  dtalpha_t *alpha,
  dtdialog_t *dialog,
  dtnote_t *note)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtalpha_refield(
  dt_ctl_t *ctl,
  dtalpha_t *alpha,
  int delta)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtalpha_load(
  dt_ctl_t *ctl,
  dtalpha_t *alpha,
  char *filename)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtalpha_save(
  dt_ctl_t *ctl,
  dtalpha_t *alpha,
  char *filename)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtalpha_append(
  dt_ctl_t *ctl,
  dtalpha_t *alpha,
  char *filename)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtalpha_status(
  dt_ctl_t *ctl,
  dtalpha_t *alpha,
  char *status)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtalpha_update(
  dt_ctl_t *ctl,
  dtalpha_t *alpha,
  dt_key_t *key)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtalpha_input(							/* wait for keyboard input		    */
  dt_ctl_t *ctl,
  dtalpha_t *alpha,
  dt_key_t *key)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtalpha_finish(
  dt_ctl_t *ctl,
  dtalpha_t *alpha)
DX_SUFFIX

/* ---------------------- */
#undef DT_DEFARGVAL
#undef DX_SUFFIX
#undef DX_PREFIX
#define DT_DEFARGVAL(A)
#ifdef __cplusplus
}
#endif
