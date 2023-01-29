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

/*----in hws.c:----*/


DX_PREFIX
dt_rc_e
DTCONFIG_API1
dthws_connect(
  dt_ctl_t *ctl,
  dthws_t *hws,
  char *init)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dthws_disconnect(
  dt_ctl_t *ctl,
  dthws_t *hws)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dthws_win_create(						/* make/display a new window		*/
  dt_ctl_t *ctl,
  dthws_t *hws,
  dthws_win_t *hws_win,
  const char *windowname,
  int x0,
  int y0,
  int xe,
  int ye,
  int bits)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dthws_wait_event(						/* hold window until user interacts	*/
  dt_ctl_t *ctl,
  dthws_t *hws,
  dtsvc_t *svc,
  double timeout,
  int flag,
  dt_event_t *event)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dthws_handle_event(						/* hold window until user interacts	*/
  dt_ctl_t *ctl,
  dthws_t *hws,
  void *buffer,
  int *handled,
  dt_event_t *event)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dthws_win_destroy(						/* get rid of window			    */
  dt_ctl_t *ctl,
  dthws_t *hws,
  dthws_win_t *win)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dthws_export_data(						/* write window's data				*/
  dt_ctl_t *ctl,
  dthws_t *hws,
  dthws_win_t *hws_win,
  void *data,
  int x0,
  int y0,
  int xe,
  int ye)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dthws_import_data(						/* get window's data				*/
  dt_ctl_t *ctl,
  dthws_t *hws,
  dthws_win_t *hws_win,
  void *data,
  int x0,
  int y0,
  int xe,
  int ye)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dthws_export_color(						/* write window's colormap			*/
  dt_ctl_t *ctl,
  dthws_t *hws,
  dthws_win_t *win,
  dtlut8_t *lut,
  int start,
  int count)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dthws_import_color(						/* get window's colormap			*/
  dt_ctl_t *ctl,
  dthws_t *hws,
  dthws_win_t *win,
  dtlut8_t *lut,
  int start,
  int count)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dthws_win_clear(						/* clear window						*/
  dt_ctl_t *ctl,
  dthws_t *hws,
  dthws_win_t *win,
  int background)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dthws_win_raise(						/* raise window						*/
  dt_ctl_t *ctl,
  dthws_t *hws,
  dthws_win_t *win)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dthws_win_lower(						/* lower window						*/
  dt_ctl_t *ctl,
  dthws_t *hws,
  dthws_win_t *win)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dthws_win_print(						/* print window						*/
  dt_ctl_t *ctl,
  dthws_t *hws,
  dthws_win_t *hws_win,
  const char *printer,					/* OS-specific printer string		*/
  double ppm,							/* pixels per millimeter 			*/
  int *submitted)						/* return true if submitted 		*/
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dthws_win_iconify(						/* iconify window					*/
  dt_ctl_t *ctl,
  dthws_t *hws,
  dthws_win_t *win)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dthws_win_move(							/* move window						*/
  dt_ctl_t *ctl,
  dthws_t *hws,
  dthws_win_t *win,
  int x,
  int y)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dthws_win_cursor(
  dt_ctl_t *ctl,
  dthws_t *hws,
  dthws_win_t *win,
  int *x,
  int *y,
  int op)
DX_SUFFIX

/*----in hwsx.c:----*/


DX_PREFIX
dt_rc_e
DTCONFIG_API1
dthws_x_connect(
  dt_ctl_t *ctl,
  dthws_t *hws,
  char *server)
DX_SUFFIX

/*----in hwsqw4.c:----*/


DX_PREFIX
dt_rc_e
DTCONFIG_API1
dthws_qw4_connect(
  dt_ctl_t *ctl,
  dthws_t *hws,
  char *spec)
DX_SUFFIX

/*----in hwsfgw.c:----*/

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dthws_fgw_connect(
  dt_ctl_t *ctl,
  dthws_t *hws,
  char *server)
DX_SUFFIX

/*----in hwsmsw.c:----*/

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dthws_msw_connect(
  dt_ctl_t *ctl,
  dthws_t *hws,
  char *server)
DX_SUFFIX

/* ---------------------- */
#undef DT_DEFARGVAL
#undef DX_SUFFIX
#undef DX_PREFIX
#define DT_DEFARGVAL(A)
#ifdef __cplusplus
}
#endif
