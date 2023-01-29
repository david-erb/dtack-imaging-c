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

/*----in fb.c:----*/


DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtfb_init(
  dt_ctl_t *ctl,
  dtfb_t *fb,
  const char *spec)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtfb_uninit(
  dt_ctl_t *ctl,
  dtfb_t *fb)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtfb_control(
  dt_ctl_t *ctl,
  dtfb_t *fb,
  dtfb_control_e control)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtfb_putblock(
  dt_ctl_t *ctl,
  dtfb_t *fb,
  void *data,
  int x0,
  int y0,
  int xe,
  int ye)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtfb_getblock(
  dt_ctl_t *ctl,
  dtfb_t *fb,
  void *data,
  int x0,
  int y0,
  int xe,
  int ye)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtfb_source(
  dt_ctl_t *ctl,
  dtfb_t *fb,
  const char *source)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtfb_putcolor(
  dt_ctl_t *ctl,
  dtfb_t *fb,
  dtlut8_t *lut,
  int start,
  int count)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtfb_getcolor(
  dt_ctl_t *ctl,
  dtfb_t *fb,
  dtlut8_t *lut,
  int start,
  int count)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtfb_cursor(
  dt_ctl_t *ctl,
  dtfb_t *fb,
  int *x,
  int *y,
  int op)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtfb_clear(
  dt_ctl_t *ctl,
  dtfb_t *fb,
  int background)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtfb_clip(
  dt_ctl_t *ctl,
  dtfb_t *fb,
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

/*----in fbdir.c:----*/


DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtfb_dir_init(
  dt_ctl_t *ctl,
  dtfb_t *fb,
  const char *spec)
DX_SUFFIX

/*----in fbgen.c:----*/


DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtfb_gen_init(
  dt_ctl_t *ctl,
  dtfb_t *fb,
  const char *spec)
DX_SUFFIX

/*----in fbvbmci.c:----*/


DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtfb_vbmci_init(
  dt_ctl_t *ctl,
  dtfb_t *fb,
  const char *spec)
DX_SUFFIX

/*----in fbvmc.c:----*/


DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtfb_vmc_init(
  dt_ctl_t *ctl,
  dtfb_t *fb,
  const char *spec)
DX_SUFFIX

/*----in fbvfw.c:----*/


DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtfb_vfw_init(
  dt_ctl_t *ctl,
  dtfb_t *fb,
  const char *spec)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtfb_vfw_rev(							/* deliver revision 				*/
  dt_ctl_t *ctl,						/* environment control 				*/
  const char **rev_string,				/* returned rev string 				*/
  int *rev_major,						/* returned rev major 				*/
  int *rev_minor,						/* returned rev minor 				*/
  long *rev_date)						/* returned rev date 				*/
DX_SUFFIX

/*----in fbsnap.c:----*/


DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtfb_snap_init(
  dt_ctl_t *ctl,
  dtfb_t *fb,
  const char *spec)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtfb_snap_rev(							/* deliver revision 				*/
  dt_ctl_t *ctl,						/* environment control 				*/
  const char **rev_string,				/* returned rev string 				*/
  int *rev_major,						/* returned rev major 				*/
  int *rev_minor,						/* returned rev minor 				*/
  long *rev_date)						/* returned rev date 				*/
DX_SUFFIX

/*----in fbpsi.c:----*/


DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtfb_psi_init(
  dt_ctl_t *ctl,
  dtfb_t *fb,
  const char *spec)
DX_SUFFIX

/*----in fbcortex.c:----*/


DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtfb_cortex_init(
  dt_ctl_t *ctl,
  dtfb_t *fb,
  const char *spec)
DX_SUFFIX

/*----in fbmeteor.c:----*/


DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtfb_meteor_init(
  dt_ctl_t *ctl,
  dtfb_t *fb,
  const char *spec)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtfb_meteor_rev(						/* deliver revision 				*/
  dt_ctl_t *ctl,						/* environment control 				*/
  const char **rev_string,				/* returned rev string 				*/
  int *rev_major,						/* returned rev major 				*/
  int *rev_minor,						/* returned rev minor 				*/
  long *rev_date)						/* returned rev date 				*/
DX_SUFFIX

/*----in fbbt.c:----*/


DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtfb_bt_init(
  dt_ctl_t *ctl,
  dtfb_t *fb,
  const char *spec)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtfb_bt_rev(						/* deliver revision 				*/
  dt_ctl_t *ctl,						/* environment control 				*/
  const char **rev_string,				/* returned rev string 				*/
  int *rev_major,						/* returned rev major 				*/
  int *rev_minor,						/* returned rev minor 				*/
  long *rev_date)						/* returned rev date 				*/
DX_SUFFIX

/*----in fbutil.c:----*/


DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtfb_util_bi8(
  dt_ctl_t *ctl,
  unsigned char *p,
  unsigned char *d,
  const int xe,
  const int ye)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtfb_util_bi24_rgb(
  dt_ctl_t *ctl,
  unsigned char *p,
  unsigned char *d,
  const int xe,
  const int ye)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtfb_util_bgr_rgb(
  dt_ctl_t *ctl,
  const unsigned char *p,
  unsigned char *d,
  const int xe,
  const int ye,
  int skip,
  int dup)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtfb_util_bi16_rgb(
  dt_ctl_t *ctl,
  unsigned char *p,
  unsigned char *d,
  const int xe,
  const int ye)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtfb_util_bi24_g(
  dt_ctl_t *ctl,
  unsigned char *p,
  unsigned char *d,
  const int xe,
  const int ye)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtfb_util_bi16_g(
  dt_ctl_t *ctl,
  unsigned char *p,
  unsigned char *d,
  const int xe,
  const int ye)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtfb_util_grb0888_rgb888(
  dt_ctl_t *ctl,
  const unsigned char * const p,
  unsigned char * const d,
  const int xe,
  const int ye,
  const unsigned char *rlookup,
  const unsigned char *glookup,
  const unsigned char *blookup,
  int skip,
  int dup)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtfb_util_grb0888_bi24(
  dt_ctl_t *ctl,
  const unsigned char *p,
  unsigned char * const d,
  const int xe,
  const int ye)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtfb_util_write_raw(
  dt_ctl_t *ctl,
  void *data,
  long size,
  const char *filename)
DX_SUFFIX

/* ---------------------- */
#undef DT_DEFARGVAL
#undef DX_SUFFIX
#undef DX_PREFIX
#define DT_DEFARGVAL(A)
#ifdef __cplusplus
}
#endif
