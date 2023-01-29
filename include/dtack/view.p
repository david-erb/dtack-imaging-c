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

/*----in view_ipc.c:----*/


DX_PREFIX
dt_rc_e
dtview_ipc(
  int argc,
  char *argv[],
  dtparam_t *param)
DX_SUFFIX

/*----in view1.c:----*/


DX_PREFIX
dt_rc_e
dtview_memory(
  dt_ctl_t *ctl,
  dtscr_t *scr)
DX_SUFFIX

DX_PREFIX
dt_rc_e
dtview_keys(
  dt_ctl_t *ctl,
  dtscr_t *scr,
  const char *output,
  const char *help_string)
DX_SUFFIX

DX_PREFIX
dt_rc_e
dtview_resize(
  dt_ctl_t *ctl,
  dtscr_t *scr)
DX_SUFFIX

DX_PREFIX
dt_rc_e
dtview_work(
  int argc,
  char *argv[],
  dtparam_t *param,
  dtsvc_t *svc,			
  dtview_loop_f *loop)
DX_SUFFIX

/*----in view2.c:----*/


DX_PREFIX
dt_rc_e
dtview_panel(
  dt_ctl_t *ctl,
  dtscr_t *scr,
  char *title,
  const char *string,
  int x0,
  int y0,
  int gravity)
DX_SUFFIX

DX_PREFIX
dt_rc_e
dtview_dsk(
  dt_ctl_t *ctl,
  dtscr_t *scr,
  const char *tmpdir)
DX_SUFFIX

DX_PREFIX
dt_rc_e
dtview_save(
  dt_ctl_t *ctl,
  dtscr_t *scr,
  const char *dirname)
DX_SUFFIX

/* ---------------------- */
#undef DT_DEFARGVAL
#undef DX_SUFFIX
#undef DX_PREFIX
#define DT_DEFARGVAL(A)
#ifdef __cplusplus
}
#endif
