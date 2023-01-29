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

/*----in dialog.c:----*/


DX_PREFIX
dt_rc_e
DTCONFIG_API1
tableau_dialog(							/* hit the done url					*/
  dtparam_t *param,
  const char *mode,						/* what's happening 				*/
  const char *client,
  long seconds,
  long frames,
  long kbytes,
  const char *keyword)					/* keyword we need to go 			*/
DX_SUFFIX

/*----in annotate.c:----*/


DX_PREFIX
dt_rc_e
DTCONFIG_API1
tableau_annotate(
  dt_ctl_t *ctl,
  dtimage_t *image,
  int y0,
  dt_color_org_t *org,
  const char *string)
DX_SUFFIX

/*----in cam.c:----*/


DX_PREFIX
dt_rc_e
DTCONFIG_API1
tableau_cam_reload(
  dtparam_t *param,
  tableau_cam_t *cam)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
tableau_cam_release(
  dtparam_t *param,
  tableau_cam_t *cam)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
tableau_cam_init(
  dtparam_t *param,
  tableau_cam_t *cam)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
tableau_cam_uninit(
  dtparam_t *param,
  tableau_cam_t *cam)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
tableau_cam_now(
  dtparam_t *param,
  tableau_cam_t *cam,
  long *now)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
tableau_cam_next(
  dtparam_t *param,
  tableau_cam_t *cam)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
tableau_cam_wait(
  dtparam_t *param,
  tableau_cam_t *cam)
DX_SUFFIX

/*----in cook.c:----*/


DX_PREFIX
dt_rc_e
DTCONFIG_API1
tableau_cook_raw(
  dtparam_t *param,
  tableau_cook_arg_t *cook_arg,			/* cooker args						*/
  dtimage_t *raw,						/* raw image location 				*/
  dt_color_org_t *org,
  dtimage_t *cooked)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
tableau_cook_ge(
  dtparam_t *param,
  tableau_cook_arg_t *cook_arg,			/* cooker args						*/
  dtimage_t *raw,						/* raw image location 				*/
  dt_color_org_t *org,
  dtimage_t *cooked)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
tableau_cook_stamp_it(
  dt_ctl_t *ctl,
  dtimage_t *image,
  int y0,
  dt_color_org_t *org,
  const char *format)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
tableau_cook_stamp(
  dtparam_t *param,
  tableau_cook_arg_t *cook_arg,			/* cooker args						*/
  dtimage_t *raw,						/* raw image location 				*/
  dt_color_org_t *org,
  dtimage_t *cooked)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
tableau_cook_resize(
  dtparam_t *param,
  tableau_cook_arg_t *cook_arg,			/* cooker args						*/
  dtimage_t *raw,						/* raw image location 				*/
  dt_color_org_t *org,
  dtimage_t *cooked)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
tableau_cook_trim(
  dtparam_t *param,
  tableau_cook_arg_t *cook_arg,			/* cooker args						*/
  dtimage_t *raw,						/* raw image location 				*/
  dt_color_org_t *org,
  dtimage_t *cooked)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
tableau_cook_from_name(
  dtparam_t *param,
  const char *name,
  tableau_cook_f **cook)
DX_SUFFIX

/*----in decode.c:----*/


DX_PREFIX
dt_rc_e
DTCONFIG_API1
tableau_decode_uncompressed(
  dtparam_t *param,
  tableau_decode_arg_t *decode_arg,		/* decoder args 					*/
  tableau_header_t *header,				/* compression header 				*/
  void *data,							/* buffer to be decoded 			*/
  dtimage_t *image)						/* decoded image 					*/
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
tableau_decode_jpeg(
  dtparam_t *param,
  tableau_decode_arg_t *decode_arg,		/* decoder args 					*/
  tableau_header_t *header,				/* compression header 				*/
  void *b,								/* buffer to be decoded 			*/
  dtimage_t *image)						/* decoded image 					*/
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
tableau_decode_from_name(
  dtparam_t *param,
  const char *name,
  tableau_decode_f **decode)
DX_SUFFIX

/*----in demo0.c:----*/


DX_PREFIX
dt_rc_e
DTCONFIG_API1
tableau_demo(
  dt_ctl_t *ctl,
  dtimage_t *image,
  int y0,
  dt_color_org_t *org,
  const char *string)
DX_SUFFIX

/*----in demo1.c:----*/


DX_PREFIX
dt_rc_e
DTCONFIG_API1
tableau_demo(
  dt_ctl_t *ctl,
  dtimage_t *image,
  int y0,
  dt_color_org_t *org,
  const char *string)
DX_SUFFIX

/*----in dir.c:----*/


DX_PREFIX
dt_rc_e
DTCONFIG_API1
tableau_dir_scan(
  dt_ctl_t *ctl,
  const char *template,
  tableau_len_t *pframe)
DX_SUFFIX

/*----in encode.c:----*/


DX_PREFIX
dt_rc_e
DTCONFIG_API1
tableau_encode_uncompressed(
  dtparam_t *param,
  tableau_encode_arg_t *encode_arg,		/* encoder args 					*/
  dtimage_t *image,						/* image to encode					*/
  dt_color_org_t *org,					/* incoming image color org 		*/
  tableau_header_t *header,				/* compression header 				*/
  void **data,							/* the encoded buffer 				*/
  long *l)								/* current size of encoded buffer 	*/
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
tableau_encode_jpeg(
  dtparam_t *param,
  tableau_encode_arg_t *encode_arg,		/* encoder args 					*/
  dtimage_t *image,						/* image to encode					*/
  dt_color_org_t *org,					/* incoming image color org 		*/
  tableau_header_t *header,				/* compression header 				*/
  void **data,							/* the encoded buffer 				*/
  long *l)								/* current size of encoded buffer 	*/
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
tableau_encode_from_name(
  dtparam_t *param,
  const char *name,
  tableau_encode_f **encode)
DX_SUFFIX

/*----in fast.c:----*/


DX_PREFIX
dt_rc_e
DTCONFIG_API1
tableau_fast_decimate(
  dt_ctl_t *ctl,
  unsigned char *s,
  dtxy_t xe,
  dtxy_t ye,
  unsigned char *d)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
tableau_fast_pixdup(
  dt_ctl_t *ctl,
  dtimage_t *image)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
tableau_fast_pixdec(
  dt_ctl_t *ctl,
  dtimage_t *image)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
tableau_fast_pixrep(
  dt_ctl_t *ctl,
  dtimage_t *input,
  int factor,
  dtimage_t *output)
DX_SUFFIX

DX_PREFIX
dt_rc_e 
DTCONFIG_API1
tableau_fast_lookup(
  dt_ctl_t *ctl,
  dtimage_t *input,
  dt_ntype_b08_t *table,
  dtimage_t *output)
DX_SUFFIX

DX_PREFIX
dt_rc_e 
DTCONFIG_API1
tableau_fast_separate(
  dt_ctl_t *ctl,
  void *input,
  int xe,
  int ye,
  void *output1,
  void *output2,
  void *output3)
DX_SUFFIX

DX_PREFIX
dt_rc_e 
DTCONFIG_API1
tableau_fast_clear(
  dt_ctl_t *ctl,
  dtimage_t *image)
DX_SUFFIX

DX_PREFIX
dt_rc_e 
DTCONFIG_API1
tableau_fast_diff(
  dt_ctl_t *ctl,
  dtimage_t *input1,
  dtimage_t *input2,
  int eps,
  dtimage_t *output)
DX_SUFFIX

DX_PREFIX
dt_rc_e 
DTCONFIG_API1
tableau_fast_add(
  dt_ctl_t *ctl,
  dtimage_t *input1,
  dtimage_t *input2)
DX_SUFFIX

/*----in get.c:----*/


DX_PREFIX
dt_rc_e
DTCONFIG_API1
tableau_get_common_free(
  dtparam_t *param,
  tableau_get_t *get,
  tableau_header_t **header,
  void **data,
  long *l)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
tableau_get_ipc_free(
  dtparam_t *param,
  tableau_get_t *get)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
tableau_get_ipc(
  dtparam_t *param,
  tableau_get_t *get,
  tableau_header_t **header,
  void **data,
  long *l)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
tableau_get_setup(
  dtparam_t *param,
  tableau_get_t *get,
  const char *spec)
DX_SUFFIX

/*----in get_fb.c:----*/


DX_PREFIX
dt_rc_e
DTCONFIG_API1
tableau_get_fb(
  dtparam_t *param,
  tableau_get_t *get,
  tableau_header_t **header,
  void **data,
  long *l)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
tableau_get_fb_free(
  dtparam_t *param,
  tableau_get_t *get)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
tableau_get_fb_setup(
  dtparam_t *param,
  tableau_get_t *get,
  const char *spec)
DX_SUFFIX

/*----in get_roll.c:----*/


DX_PREFIX
dt_rc_e
DTCONFIG_API1
tableau_get_roll(
  dtparam_t *param,
  tableau_get_t *get,
  tableau_header_t **header,
  void **data,
  long *l)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
tableau_get_roll_free(
  dtparam_t *param,
  tableau_get_t *get)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
tableau_get_roll_setup(
  dtparam_t *param,
  tableau_get_t *get,
  const char *spec)
DX_SUFFIX

/*----in get_ser.c:----*/


DX_PREFIX
dt_rc_e
DTCONFIG_API1
tableau_get_ser(
  dtparam_t *param,
  tableau_get_t *get,
  tableau_header_t **header,
  void **data,
  long *l)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
tableau_get_ser_free(
  dtparam_t *param,
  tableau_get_t *get)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
tableau_get_ser_setup(
  dtparam_t *param,
  tableau_get_t *get,
  const char *spec)
DX_SUFFIX

/*----in get_file.c:----*/


DX_PREFIX
dt_rc_e
DTCONFIG_API1
tableau_get_file_shm(
  dtparam_t *param,
  tableau_get_t *get,
  tableau_header_t **header,
  void **data,
  long *l)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
tableau_get_file_normal(
  dtparam_t *param,
  tableau_get_t *get,
  tableau_header_t **header,
  void **data,
  long *l)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
tableau_get_file_free(
  dtparam_t *param,
  tableau_get_t *get)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
tableau_get_file_setup(
  dtparam_t *param,
  tableau_get_t *get,
  const char *spec)
DX_SUFFIX

/*----in http.c:----*/


DX_PREFIX
dt_rc_e
DTCONFIG_API1
tableau_http_start(
  dtparam_t *param,
  const char *url,
  char **spec)
DX_SUFFIX

/*----in ipc.c:----*/


DX_PREFIX
dt_rc_e
DTCONFIG_API1
tableau_ipc_setup(
  dtparam_t *param,
  const char *spec,
  const char *server_command,
  dtipc_t *ipc)
DX_SUFFIX

/*----in log.c:----*/


DX_PREFIX
void
DTCONFIG_API1		
tableau_dbg(							/* output debug message				*/
  dt_ctl_t *ctl,						/* environment control 				*/
  const char *caller,					/* caller's name 					*/
  long class_bits,						/* debug message class 				*/
  const char *format,					/* message format, like sprintf() 	*/
  va_list ap)							/* format substitutions 			*/
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1		
tableau_err(							/* handle errors					*/
  dt_ctl_t *ctl,						/* environment control 				*/
  const char *caller,					/* caller's name 					*/
  const char *format,					/* message format, like sprintf() 	*/
  va_list ap)							/* format substitutions 			*/
DX_SUFFIX

DX_PREFIX
void
DTCONFIG_API1		
tableau_say(							/* output console message			*/
  dt_ctl_t *ctl,						/* environment control 				*/
  const char *caller,					/* caller's name 					*/
  const char *format,					/* message format, like sprintf() 	*/
  va_list ap)							/* format substitutions 			*/
DX_SUFFIX

/*----in poll.c:----*/


DX_PREFIX
dt_rc_e
DTCONFIG_API1
tableau_poll_ident(
  dt_ctl_t *ctl,
  dtdg_server_t *dgserver,
  void *arg,
  const char *command,
  char *rest,
  char *message,
  int message_max)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
tableau_poll_state(
  dt_ctl_t *ctl,
  dtdg_server_t *dgserver,
  void *arg,
  const char *command,
  char *rest,
  char *message,
  int message_max)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
tableau_poll_quality(
  dt_ctl_t *ctl,
  dtdg_server_t *dgserver,
  void *arg,
  const char *command,
  char *rest,
  char *message,
  int message_max)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
tableau_poll_source(
  dt_ctl_t *ctl,
  dtdg_server_t *dgserver,
  void *arg,
  const char *command,
  char *rest,
  char *message,
  int message_max)
DX_SUFFIX

DX_PREFIX
dt_rc_e 
DTCONFIG_API1
tableau_poll(
  tableau_t *tableau,
  dtdg_server_t *dgserver,
  long delay)
DX_SUFFIX

/*----in put.c:----*/


DX_PREFIX
dt_rc_e
DTCONFIG_API1
tableau_put_null(
  dtparam_t *param,
  tableau_put_t *put,
  dtimage_t *image,						/* full image 						*/
  tableau_header_t *header,				/* compressed header 				*/
  void *data,							/* compressed data 					*/
  int cc)								/* color component 					*/
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
tableau_put_setup(
  dtparam_t *param,
  tableau_put_t *put,
  const char *spec)
DX_SUFFIX

/*----in put_scr.c:----*/


DX_PREFIX
dt_rc_e
DTCONFIG_API1
tableau_put_scr_setup(
  dtparam_t *param,
  tableau_put_t *put,
  const char *spec)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
tableau_put_scr(
  dtparam_t *param,
  tableau_put_t *put,
  dtimage_t *image,						/* full image 						*/
  tableau_header_t *header,				/* compressed header 				*/
  void *data,							/* compressed data 					*/
  int cc)
DX_SUFFIX

/*----in put_roll.c:----*/


DX_PREFIX
dt_rc_e
DTCONFIG_API1
tableau_put_roll(
  dtparam_t *param,
  tableau_put_t *put,
  dtimage_t *image,						/* full image 						*/
  tableau_header_t *header,				/* compressed header 				*/
  void *data,							/* compressed data 					*/
  int cc)								/* color component 					*/
DX_SUFFIX

/*----in put_ftp.c:----*/


DX_PREFIX
dt_rc_e
DTCONFIG_API1
tableau_put_ftp_setup(
  dtparam_t *param,
  tableau_put_t *put,
  const char *spec)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
tableau_put_ftp(
  dtparam_t *param,
  tableau_put_t *put,
  dtimage_t *image,						/* full image 						*/
  tableau_header_t *header,				/* compressed header 				*/
  void *data,							/* compressed data 					*/
  int cc)
DX_SUFFIX

/*----in put_ipc.c:----*/


DX_PREFIX
dt_rc_e
DTCONFIG_API1
tableau_put_ipc(
  dtparam_t *param,
  tableau_put_t *put,
  dtimage_t *image,						/* full image 						*/
  tableau_header_t *header,				/* compressed header 				*/
  void *data,							/* compressed data 					*/
  int cc)								/* color component 					*/
DX_SUFFIX

/*----in put_file.c:----*/


DX_PREFIX
dt_rc_e
DTCONFIG_API1
tableau_put_file_setup(
  dtparam_t *param,
  tableau_put_t *put,
  const char *spec)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
tableau_put_file(
  dtparam_t *param,
  tableau_put_t *put,
  dtimage_t *image,						/* full image 						*/
  tableau_header_t *header,				/* compressed header 				*/
  void *data,							/* compressed data 					*/
  int cc)								/* color component 					*/
DX_SUFFIX

/*----in put_push.c:----*/


DX_PREFIX
dt_rc_e
DTCONFIG_API1
tableau_put_push(
  dtparam_t *param,
  tableau_put_t *put,
  dtimage_t *image,						/* full image 						*/
  tableau_header_t *header,				/* compressed header 				*/
  void *data,							/* compressed data 					*/
  int cc)								/* color component 					*/
DX_SUFFIX

/*----in rev.c:----*/


DX_PREFIX
dt_rc_e
DTCONFIG_API1
tableau_rev(
  dt_ctl_t *ctl,
  const char **rev_string,
  int *rev_major,
  int *rev_minor,
  long *rev_date)
DX_SUFFIX

/*----in roll.c:----*/


DX_PREFIX
dt_rc_e
DTCONFIG_API1
tableau_roll_name(
  dt_ctl_t *ctl,
  const char *path,
  const char *camname,
  char *s,
  int m)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
tableau_roll_add(
  dt_ctl_t *ctl,
  const char *path,
  const char *camname,
  tableau_len_t frame)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
tableau_roll_get(
  dt_ctl_t *ctl,
  const char *path,
  const char *camname,
  tableau_len_t frame,					/* frame wanted 					*/
  tableau_len_t *newest)				/* newest available frame 			*/
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
tableau_roll_init(
  dt_ctl_t *ctl,
  const char *path,
  const char *camname,
  const char *template)
DX_SUFFIX

/*----in shovel.c:----*/


DX_PREFIX
dt_rc_e
DTCONFIG_API1
tableau_shovel(
  dtparam_t *param,
  const char *dgserver_spec,
  tableau_get_t *get,
  const char *cook_spec,
  const char *encode_spec,
  tableau_put_t *put)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
tableau_test(
  dtparam_t *param)
DX_SUFFIX

/* ---------------------- */
#undef DT_DEFARGVAL
#undef DX_SUFFIX
#undef DX_PREFIX
#define DT_DEFARGVAL(A)
#ifdef __cplusplus
}
#endif
