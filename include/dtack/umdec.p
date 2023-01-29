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

/*----in umdeclib.c:----*/


DX_PREFIX
dt_rc_e
DTCONFIG_API1
umdec_init(								/* initialize decoder object 		*/
  dt_ctl_t *ctl,							/* environment control 				*/
  umdec_t *umdec,						/* decoder object returned			*/
  const char *spec,						/* decoder specification 			*/
  unsigned long flags)					/* process control flags 			*/
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
umdec_uninit(
  dt_ctl_t *ctl,
  umdec_t *umdec)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
umdec_go(
  dt_ctl_t *ctl,
  umdec_t *umdec,
  umdec_feed_callback_f *feed,			/* callback for food 				*/
  void *feed_arg,						/* feed callback argument 			*/
  umdec_frame_callback_f *frame,		/* callback for frame 				*/
  void *frame_arg,						/* frame callback argument 			*/
  umdec_control_t *control)				/* initial mode 					*/
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
umdec_set_control(						/* set operation parameters 		*/
  dt_ctl_t *ctl,							/* environment control 				*/
  umdec_t *mdec,						/* decoder object structure 		*/
  umdec_control_t *control)				/* control parameters 				*/
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
umdec_get_control(						/* get operation parameters 		*/
  dt_ctl_t *ctl,							/* environment control 				*/
  umdec_t *mdec,						/* decoder object structure 		*/
  umdec_control_t *control)				/* returned control parameters		*/
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
umdec_get_status(						/* get operation parameters 		*/
  dt_ctl_t *ctl,						/* environment control 				*/
  umdec_t *mdec,						/* decoder object structure 		*/
  umdec_status_t *status)				/* returned status					*/
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
umdec_overlay_image(					/* load image into overlay			*/
  dt_ctl_t *ctl,						/* environment control 				*/
  umdec_t *mdec,						/* decoder object structure 		*/
  const char *filename,					/* filename containing image 		*/
  int x,								/* column 							*/
  int y)								/* row 								*/
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
umdec_overlay_text(						/* load text into overlay			*/
  dt_ctl_t *ctl,						/* environment control 				*/
  umdec_t *mdec,						/* decoder object structure 		*/
  const char *string,					/* text to display 					*/
  int x,								/* column 							*/
  int y,								/* row 								*/
  int height,							/* text height in pixels 			*/
  long foreground,						/* foreground color RGB 			*/
  long background)						/* background color RGB 			*/
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
umdec_overlay_clear(					/* clear overlay					*/
  dt_ctl_t *ctl,						/* environment control 				*/
  umdec_t *mdec)						/* decoder object structure 		*/
DX_SUFFIX

/*----in umdecssg.c:----*/


DX_PREFIX
dt_rc_e
DTCONFIG_API1
umdec_ssg_frame_callback(
  dt_ctl_t *ctl,							/* environment control 				*/
  umdec_t *umdec,						/* umdec object 					*/
  void *arg,							/* caller's unchanged argument 		*/
  umdec_frametype_e frametype)			/* type of frame being given 		*/
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
umdec_ssg_init(
  dt_ctl_t *ctl,
  umdec_t *umdec,
  const char *spec)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
umdec_ssg_rev(							/* deliver revision 				*/
  dt_ctl_t *ctl,						/* environment control 				*/
  const char **rev_string,				/* returned rev string 				*/
  int *rev_major,						/* returned rev major 				*/
  int *rev_minor,						/* returned rev minor 				*/
  long *rev_date)						/* returned rev date 				*/
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
umdec_vp_init(
  dt_ctl_t *ctl,
  umdec_t *umdec,
  const char *spec)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
umdec_vp_rev(							/* deliver revision 				*/
  dt_ctl_t *ctl,						/* environment control 				*/
  const char **rev_string,				/* returned rev string 				*/
  int *rev_major,						/* returned rev major 				*/
  int *rev_minor,						/* returned rev minor 				*/
  long *rev_date)						/* returned rev date 				*/
DX_SUFFIX

/* ---------------------- */
#undef DT_DEFARGVAL
#undef DX_SUFFIX
#undef DX_PREFIX
#define DT_DEFARGVAL(A)
#ifdef __cplusplus
}
#endif
