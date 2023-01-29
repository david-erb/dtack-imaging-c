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

/*----in rev.c:----*/


DX_PREFIX
dt_rc_e
DTCONFIG_API1
tabrobot_rev(
  dt_ctl_t *ctl,
  const char **rev_string,
  int *rev_major,
  int *rev_minor,
  long *rev_date)
DX_SUFFIX

/*----in log.c:----*/


DX_PREFIX
void
DTCONFIG_API1
tabrobot_dbg(							/* output debug message				*/
  dt_ctl_t *ctl,						/* environment control 				*/
  const char *caller,					/* caller's name 					*/
  long class_bits,						/* debug message class 				*/
  const char *format,					/* message format, like sprintf() 	*/
  va_list ap)							/* format substitutions 			*/
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
tabrobot_err(							/* handle errors					*/
  dt_ctl_t *ctl,						/* environment control 				*/
  const char *caller,					/* caller's name 					*/
  const char *format,					/* message format, like sprintf() 	*/
  va_list ap)							/* format substitutions 			*/
DX_SUFFIX

DX_PREFIX
void
DTCONFIG_API1
tabrobot_say(							/* output console message			*/
  dt_ctl_t *ctl,						/* environment control 				*/
  const char *caller,					/* caller's name 					*/
  const char *format,					/* message format, like sprintf() 	*/
  va_list ap)							/* format substitutions 			*/
DX_SUFFIX

/*----in preset.c:----*/


DX_PREFIX
dt_rc_e
DTCONFIG_API1
tabrobot_preset_set(
  dtparam_t *param,
  const char *preset,
  const char *desc)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
tabrobot_preset_get(
  dtparam_t *param,
  const char *preset,
  char *desc,
  int max)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
tabrobot_preset_load(
  dtparam_t *param,
  dtlist_t *list,
  long *mtime)
DX_SUFFIX

/*----in client.c:----*/


DX_PREFIX
dt_rc_e
DTCONFIG_API1
tabrobot_client_send(
  dtparam_t *param,
  dtipc_t *ipc,
  const char *command,
  long *turnaround)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
tabrobot_client(
  dtparam_t *param,
  const char *command)
DX_SUFFIX

/*----in roam.c:----*/


DX_PREFIX
dt_rc_e
DTCONFIG_API1
tabrobot_roam(
  dtparam_t *param)
DX_SUFFIX

/*----in gui_qnx.c:----*/


DX_PREFIX
dt_rc_e
DTCONFIG_API1
tabrobot_gui_qnx(
  dtparam_t *param)
DX_SUFFIX

/*----in server.c:----*/


DX_PREFIX
dt_rc_e
DTCONFIG_API1
tabrobot_server(
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
