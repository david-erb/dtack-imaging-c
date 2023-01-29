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

/*----in canvas.c:----*/


DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtcanvas_init(
  dt_ctl_t *ctl,
  dtcanvas_t *canvas,
  char *server)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtcanvas_uninit(
  dt_ctl_t *ctl,
  dtcanvas_t *canvas)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtcanvas_open(
  dt_ctl_t *ctl,
  dtcanvas_t *canvas)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtcanvas_input(
  dt_ctl_t *ctl,
  dtcanvas_t *canvas,
  dtsvc_t *svc,
  double timeout,
  int flag,
  dt_event_t *event)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtcanvas_close(
  dt_ctl_t *ctl,
  dtcanvas_t *canvas)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtcanvas_putblock(
  dt_ctl_t *ctl,
  dtcanvas_t *canvas,
  void *data,
  int x0,
  int y0,
  int xe,
  int ye)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtcanvas_getblock(
  dt_ctl_t *ctl,
  dtcanvas_t *canvas,
  void *data,
  int x0,
  int y0,
  int xe,
  int ye)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtcanvas_putcolor(
  dt_ctl_t *ctl,
  dtcanvas_t *canvas,
  dtlut8_t *lut,
  int start,
  int count)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtcanvas_getcolor(
  dt_ctl_t *ctl,
  dtcanvas_t *canvas,
  dtlut8_t *lut,
  int start,
  int count)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtcanvas_cursor(
  dt_ctl_t *ctl,
  dtcanvas_t *canvas,
  int *x,
  int *y,
  int op)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtcanvas_clear(
  dt_ctl_t *ctl,
  dtcanvas_t *canvas,
  int background)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtcanvas_clip(
  dt_ctl_t *ctl,
  dtcanvas_t *canvas,
  void *data,
  int x0,
  int y0,
  int xe,
  int ye,
  void **cdata,
  int *cx0,
  int *cy0,
  int *cxe,
  int *cye)
DX_SUFFIX

/*----in canpoll.c:----*/


DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtcanvas_poll(
  dt_ctl_t *ctl,
  dtcanvas_t *canvas,
  dt_event_t *event,
  double timeout,
  int flag,
  dtkeybd_t *keybd,
  dtcanvas_mouse_f *mousefunc,
  int *mousex,
  int *mousey,
  dtos_time_t *unclick_time,
  int *click_state)
DX_SUFFIX

/*----in candum.c:----*/


DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtcanvas_dummy_init(
  dt_ctl_t *ctl,
  dtcanvas_t *canvas,
  char *init)
DX_SUFFIX

/*----in canfastg.c:----*/


DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtcanvas_fastgraph_init(
  dt_ctl_t *ctl,
  dtcanvas_t *canvas,
  char *driver)
DX_SUFFIX

/*----in cansvgac.c:----*/


DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtcanvas_svgacc_init(
  dt_ctl_t *ctl,
  dtcanvas_t *canvas,
  char *driver)
DX_SUFFIX

/*----in canhws.c:----*/


DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtcanvas_hws_init(
  dt_ctl_t *ctl,
  dtcanvas_t *canvas,
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
