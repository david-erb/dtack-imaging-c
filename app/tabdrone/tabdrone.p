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

/*----in clip.c:----*/


DX_PREFIX
dt_rc_e
DTCONFIG_API1
tabdrone_clip(							/* pump clip input to output		*/
  tabdrone_t *tabdrone,					/* application state 				*/
  dtipc_t *ipc)
DX_SUFFIX

/*----in live.c:----*/


DX_PREFIX
dt_rc_e
DTCONFIG_API1
tabdrone_audio_check(
  tabdrone_t *tabdrone,
  dtipc_t *ipc)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
tabdrone_live(							/* pump live input to output		*/
  tabdrone_t *tabdrone,					/* application state 				*/
  dtipc_t *ipc)
DX_SUFFIX

/*----in rev.c:----*/


DX_PREFIX
dt_rc_e
DTCONFIG_API1
tabdrone_rev(
  dt_ctl_t *ctl,
  const char **rev_string,
  int *rev_major,
  int *rev_minor,
  long *rev_date)
DX_SUFFIX

/*----in dir.c:----*/


DX_PREFIX
dt_rc_e
DTCONFIG_API1
tabdrone_dir_scan(
  tabdrone_t *tabdrone,					/* application state 				*/
  const char *template,
  tabdrone_frame_t *pframe)
DX_SUFFIX

/*----in start.c:----*/


DX_PREFIX
dt_rc_e
DTCONFIG_API1
tabdrone_start(							/* hit the start url					*/
  tabdrone_t *tabdrone)					/* application state 				*/
DX_SUFFIX

/*----in limit.c:----*/


DX_PREFIX
dt_rc_e
DTCONFIG_API1
tabdrone_limit(							/* limit frames						*/
  tabdrone_t *tabdrone)					/* application state 				*/
DX_SUFFIX

/*----in done.c:----*/


DX_PREFIX
dt_rc_e
DTCONFIG_API1
tabdrone_done(							/* hit the done url					*/
  tabdrone_t *tabdrone)					/* application state 				*/
DX_SUFFIX

/*----in init.c:----*/


DX_PREFIX
dt_rc_e
DTCONFIG_API1
tabdrone_init(							/* init tabdrone structure			*/
  tabdrone_t *tabdrone,					/* application state 				*/
  dtparam_t *param)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
tabdrone_uninit(						/* uninit tabdrone structure		*/
  tabdrone_t *tabdrone)					/* application state 				*/
DX_SUFFIX

/*----in server.c:----*/


DX_PREFIX
dt_rc_e
DTCONFIG_API1
tabdrone_server(
  dtparam_t *param)
DX_SUFFIX

/*----in feedback.c:----*/


DX_PREFIX
dt_rc_e
DTCONFIG_API1
tabdrone_feedback(						/* check for feedback from java		*/
  tabdrone_t *tabdrone,					/* application state 				*/
  dtipc_t *ipc)
DX_SUFFIX

/*----in compress.c:----*/


DX_PREFIX
dt_rc_e
DTCONFIG_API1
tabdrone_compress(
  dtparam_t *param,
  void *data,
  long ndata,
  long xe,
  long ye,
  long quality,
  void **buffer,
  long *nbuffer,
  long *obuffer)
DX_SUFFIX

/*----in pumpe.c:----*/


DX_PREFIX
dt_rc_e
DTCONFIG_API1
tabdrone_pump(							/* pump input to output 			*/
  tabdrone_t *tabdrone,					/* application state 				*/
  dtipc_t *ipc)
DX_SUFFIX

/*----in output.c:----*/


DX_PREFIX
dt_rc_e
DTCONFIG_API1
tabdrone_output(
  tabdrone_t *tabdrone,					/* application state 				*/
  const void *buffer,
  long nbuffer,
  const char *suffix,
  dtipc_t *ipc)
DX_SUFFIX

/*----in grab.c:----*/


DX_PREFIX
dt_rc_e
DTCONFIG_API1
tabdrone_grab(
  dtparam_t *param,
  void **data,
  long *ndata,
  long *xe,
  long *ye)
DX_SUFFIX

/*----in verify.c:----*/


DX_PREFIX
dt_rc_e
DTCONFIG_API1
tabdrone_verify(
  dtparam_t *param,
  dtipc_t *ipc)
DX_SUFFIX

/* ---------------------- */
#undef DT_DEFARGVAL
#undef DX_SUFFIX
#undef DX_PREFIX
#define DT_DEFARGVAL(A)
#ifdef __cplusplus
}
#endif
