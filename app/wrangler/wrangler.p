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

/*----in rev.c:----*/


DX_PREFIX
dt_rc_e
DTCONFIG_API1
wrangler_rev(
  dt_ctl_t *ctl,
  const char **rev_string,
  int *rev_major,
  int *rev_minor,
  long *rev_date)
DX_SUFFIX

/*----in verify.c:----*/


DX_PREFIX
dt_rc_e
DTCONFIG_API1
wrangler_verify(
  wrangler_t *wrangler,
  dtipc_t *ipc)
DX_SUFFIX

/*----in init.c:----*/


DX_PREFIX
dt_rc_e
DTCONFIG_API1
wrangler_uninit(
  wrangler_t *wrangler)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
wrangler_init(
  wrangler_t *wrangler,
  dtparam_t *param)
DX_SUFFIX

/*----in server.c:----*/


DX_PREFIX
dt_rc_e 
DTCONFIG_API1
wrangler_goodbye(
  dt_ctl_t *ctl,
  void *goodbye_arg)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
wrangler_cmd_start(
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
wrangler_cmd_cgi(						/* start cgi program 				*/
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
wrangler_cmd_docs(						/* serve file to browser			*/
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
wrangler_cmd_state(						/* reply our state to browser 		*/
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
wrangler_cmd_exit(						/* exit this server					*/
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
wrangler_server_transact(
  wrangler_t *wrangler,
  dtdg_server_t *dgserver)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
wrangler_server(
  dtparam_t *param)
DX_SUFFIX
#undef DX_SUFFIX
#undef DX_PREFIX
#ifdef __cplusplus
}
#endif
