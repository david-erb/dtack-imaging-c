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

/*----in msgu.c:----*/


DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtmsg_create(							/* create new message				*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtmsg_t *msg,							/* returned message structure 		*/
  int key)								/* system-wide message id 			*/
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtmsg_destroy(							/* release msg object 				*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtmsg_t *msg)							/* msg object 						*/
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtmsg_access(							/* access new message				*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtmsg_t *msg,							/* returned message structure 		*/
  int key)								/* system-wide message id 			*/
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtmsg_release(							/* release msg object 				*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtmsg_t *msg)							/* msg object 						*/
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtmsg_post(								/* post msg object 					*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtmsg_t *msg,							/* msg object 						*/
  const void *data,
  long length)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtmsg_wait(								/* wait msg object 					*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtmsg_t *msg,							/* msg object 						*/
  long max,
  void *data,
  long *length)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtmsg_get_count(						/* get current message count		*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtmsg_t *msg,							/* returned message structure 	*/
  int *count)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtmsg_exists(							/* access new message				*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtmsg_t *msg,							/* returned message structure 		*/
  int key,								/* system-wide message id 			*/
  int *exists)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtmsg_flush(							/* flush all messages				*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtmsg_t *msg)							/* returned message structure 		*/
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtmsg_rev(								/* deliver revision 				*/
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
