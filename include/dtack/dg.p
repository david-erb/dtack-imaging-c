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

/*----in line.c:----*/


DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtdg_get_line(							/* receive single response line		*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtdg_t *dg,							/* this dtdg object 				*/
  char **line)							/* returned line 					*/
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtdg_get_line2(							/* receive single response line		*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtdg_t *dg,							/* this dtdg object 				*/
  long timeout,
  long maxbytes,						/* max bytes to read total			*/
  char **line,							/* returned line 					*/
  long *got)							/* number of bytes read 			*/
DX_SUFFIX

/*----in header.c:----*/


DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtdg_header_get(						/* receive response header			*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtdg_t *dg,							/* this dtdg object 				*/
  long timeout)
DX_SUFFIX

DX_PREFIX
dt_rc_e 
DTCONFIG_API1
dtdg_header_search(						/* search field value for header	*/
  dt_ctl_t *ctl,
  dtdg_t *dg,
  const char *name,
  const char **value)
DX_SUFFIX

DX_PREFIX
dt_rc_e 
DTCONFIG_API1
dtdg_header_find(						/* find field value in header		*/
  dt_ctl_t *ctl,
  dtdg_t *dg,
  const char *name,
  const char **value)
DX_SUFFIX

DX_PREFIX
dt_rc_e 
DTCONFIG_API1
dtdg_header_since(						/* search field value for header	*/
  dt_ctl_t *ctl,
  dtdg_t *dg,
  const char *filename,
  int *modified)
DX_SUFFIX

/*----in dgserver.c:----*/


DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtdg_url_to_spec(						/* convert url to ipc spec			*/
  dt_ctl_t *ctl,						/* environment control 				*/
  const char *url,						/* given possible url 				*/
  char **spec)							/* converted spec					*/
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtdg_server_use(						/* instantiate dtdg_server object 	*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtdg_server_t *dgserver,				/* this dtdg_server object 			*/
  dtipc_t *ipc)							/* existing ipc connection 			*/
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtdg_server_init(						/* instantiate dtdg_server object 	*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtdg_server_t *dgserver,				/* this dtdg_server object 			*/
  const char *spec)						/* spec identifying this server 	*/
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtdg_server_is_request(					/* check for client request pending */
  dt_ctl_t *ctl,						/* environment control 				*/
  dtdg_server_t *dgserver,				/* this dtdg_server object 			*/
  int *is_request)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtdg_server_read_request2(				/* get incoming request 			*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtdg_server_t *dgserver,				/* this dtdg_server object 			*/
  long timeout,							/* timeout 							*/
  char **request)						/* the returned request string		*/
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtdg_server_read_request(				/* get incoming request 			*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtdg_server_t *dgserver,				/* this dtdg_server object 			*/
  char **request)						/* the returned request string		*/
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtdg_server_get_request(				/* get incoming request 			*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtdg_server_t *dgserver,				/* this dtdg_server object 			*/
  char **request)						/* the returned request string		*/
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtdg_server_wait(						/* wait for client request			*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtdg_server_t *dgserver,				/* this dtdg_server object 			*/
  long timeout,							/* maximum milliseconds to wait 	*/
  char **request)						/* returned request or NULL 		*/
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtdg_server_done(						/* finish response 					*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtdg_server_t *dgserver)				/* this dtdg_server object 			*/
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtdg_server_uninit(						/* release resources 				*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtdg_server_t *dgserver)				/* this dtdg_server object 			*/
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtdg_server_wait_line(					/* receive single post line		*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtdg_server_t *dgserver,				/* this dtdg_server object 			*/
  long timeout,							/* maximum milliseconds to wait 	*/
  int *is_available)					/* returned ready status 			*/
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtdg_server_get_post(					/* receive post body				*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtdg_server_t *dgserver,				/* this dtdg_server object 			*/
  long timeout,
  long maxlines,						/* maximum number of lines 			*/
  long *pgot)							/* number of lines gotten 			*/
DX_SUFFIX

DX_PREFIX
dt_rc_e 
DTCONFIG_API1
dtdg_server_post_search(				/* search field value for post		*/
  dt_ctl_t *ctl,
  dtdg_server_t *dgserver,
  const char *name,
  const char **value)
DX_SUFFIX

DX_PREFIX
dt_rc_e 
DTCONFIG_API1
dtdg_server_post_find(					/* find field value in post			*/
  dt_ctl_t *ctl,
  dtdg_server_t *dgserver,
  const char *name,
  const char **value)
DX_SUFFIX

/*----in request.c:----*/


DX_PREFIX
dt_rc_e 
DTCONFIG_API1
dtdg_server_request2(
  dt_ctl_t *ctl,
  dtdg_server_t *dgserver,
  char *request,
  char separator,
  void *arg,
  const char **requests,
  dtdg_server_request_f **funcs,
  char *message,
  int message_max)
DX_SUFFIX

DX_PREFIX
dt_rc_e 
DTCONFIG_API1
dtdg_server_request(
  dt_ctl_t *ctl,
  dtdg_server_t *dgserver,
  char *request,
  void *arg,
  const char **requests,
  dtdg_server_request_f **funcs,
  char *message,
  int message_max)
DX_SUFFIX

/*----in response.c:----*/


DX_PREFIX
dt_rc_e
DTCONFIG_API2
dtdg_server_response(					/* emit response line 				*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtdg_server_t *dgserver,				/* this dtdg_server object 			*/
  const char *format,
  ...)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtdg_server_response_va(				/* emit response line 				*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtdg_server_t *dgserver,				/* this dtdg_server object 			*/
  const char *format,
  va_list ap)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtdg_server_header_plain(
  dt_ctl_t *ctl,				   
  dtdg_server_t *dgserver,
  const char *title)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtdg_server_header_html(
  dt_ctl_t *ctl,				   
  dtdg_server_t *dgserver,
  const char *title)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtdg_server_trailer_plain(
  dt_ctl_t *ctl,				   
  dtdg_server_t *dgserver)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtdg_server_trailer_html(
  dt_ctl_t *ctl,				   
  dtdg_server_t *dgserver)
DX_SUFFIX

/*----in rev.c:----*/


DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtdg_rev(
  dt_ctl_t *ctl,
  const char **rev_string,
  int *rev_major,
  int *rev_minor,
  long *rev_date)
DX_SUFFIX

/*----in exit.c:----*/


DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtdg_server_exit(
  dt_ctl_t *ctl,
  dtdg_server_t *dgserver,
  void *arg,
  const char *command,
  char *rest,
  char *message,
  int message_max)
DX_SUFFIX

/*----in delay.c:----*/


DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtdg_server_delay(
  dt_ctl_t *ctl,
  dtdg_server_t *dgserver,
  void *arg,
  const char *command,
  char *rest,
  char *message,
  int message_max)
DX_SUFFIX

/*----in dgclient.c:----*/


DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtdg_client_init2(						/* instantiate dtdg_client object 	*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtdg_client_t *dgclient,				/* this dtdg_client object 			*/
  const char *spec,						/* spec identifying server 			*/
  const char *request,					/* the request string 				*/
  dtlist_t *request_header_list,		/* list of request header fields 	*/
  long timeout)							/* maximum time to wait for accept 	*/
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtdg_client_init(						/* instantiate dtdg_client object 	*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtdg_client_t *dgclient,				/* this dtdg_client object 			*/
  const char *spec,						/* spec identifying server 			*/
  const char *request,					/* the request string 				*/
  long timeout)							/* maximum time to wait for accept 	*/
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtdg_client_wait_line(					/* receive single response line		*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtdg_client_t *dgclient,				/* this dtdg_client object 			*/
  long timeout,							/* maximum milliseconds to wait 	*/
  int *is_available)					/* returned ready status 			*/
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtdg_client_wait_header(				/* wait for header to start			*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtdg_client_t *dgclient,				/* this dtdg_client object 			*/
  long timeout,							/* maximum milliseconds to wait 	*/
  int *is_available)					/* returned ready status 			*/
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtdg_client_get_response(				/* receive response body			*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtdg_client_t *dgclient,				/* this dtdg_client object 			*/
  long timeout,
  long max,								/* maximum number of lines 			*/
  long *pgot)							/* number of lines gotten 			*/
DX_SUFFIX

DX_PREFIX
dt_rc_e 
DTCONFIG_API1
dtdg_client_response_search(			/* search field value for response	*/
  dt_ctl_t *ctl,
  dtdg_client_t *dgclient,
  const char *name,
  const char **value)
DX_SUFFIX

DX_PREFIX
dt_rc_e 
DTCONFIG_API1
dtdg_client_response_find(				/* find field value in response		*/
  dt_ctl_t *ctl,
  dtdg_client_t *dgclient,
  const char *name,
  const char **value)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtdg_client_uninit(						/* release resources 				*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtdg_client_t *dgclient)				/* this dtdg_client object 			*/
DX_SUFFIX

/* ---------------------- */
#undef DT_DEFARGVAL
#undef DX_SUFFIX
#undef DX_PREFIX
#define DT_DEFARGVAL(A)
#ifdef __cplusplus
}
#endif
