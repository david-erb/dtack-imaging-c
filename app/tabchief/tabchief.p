#ifdef __cplusplus
extern "C" {
#endif

/*----in client.c:----*/

dt_rc_e
DTCONFIG_API1
tabchief_client(
  dtparam_t *param,
  const char *ipcspec,
  const char *command);

/*----in copy.c:----*/

dt_rc_e
DTCONFIG_API1
tabchief_server_copyto(
  dtparam_t *param,
  dtipc_t *ipc,
  const char *command,
  const char *arg);
dt_rc_e
DTCONFIG_API1
tabchief_client_copyto(
  dtparam_t *param,
  dtipc_t *ipc);
dt_rc_e
DTCONFIG_API1
tabchief_server_copyfrom(
  dtparam_t *param,
  dtipc_t *ipc,
  const char *command,
  const char *arg);
dt_rc_e
DTCONFIG_API1
tabchief_client_copyfrom(
  dtparam_t *param,
  dtipc_t *ipc);

/*----in dsched.c:----*/

dt_rc_e
DTCONFIG_API1
dsched(
  dtparam_t *param,
  dtipc_t *ipc,
  const char *command,
  const char *arg);

/*----in edit.c:----*/

dt_rc_e
DTCONFIG_API1
tabchief_server_editfile(
  dtparam_t *param,
  dtipc_t *ipc,
  const char *command,
  const char *arg);

/*----in file.c:----*/

dt_rc_e
DTCONFIG_API1
tabchief_server_catfile(
  dtparam_t *param,
  dtipc_t *ipc,
  const char *filename,
  unsigned long flags);
dt_rc_e
DTCONFIG_API1
tabchief_server_showfile(
  dtparam_t *param,
  dtipc_t *ipc,
  const char *command,
  const char *arg);
dt_rc_e
DTCONFIG_API1
tabchief_server_tailfile(
  dtparam_t *param,
  dtipc_t *ipc,
  const char *command,
  const char *arg);
dt_rc_e
DTCONFIG_API1
tabchief_server_html(
  dtparam_t *param,
  dtipc_t *ipc,
  const char *command,
  const char *arg);
dt_rc_e
DTCONFIG_API1
tabchief_server_gif(
  dtparam_t *param,
  dtipc_t *ipc,
  const char *command,
  const char *arg);

/*----in index.c:----*/

dt_rc_e
DTCONFIG_API1
tabchief_index(
  dtparam_t *param,
  dtipc_t *ipc,
  const char *command,
  const char *arg);

/*----in reply.c:----*/

dt_rc_e
DTCONFIG_API1
tabchief_reply(
  dtparam_t *param,
  dtipc_t *ipc,
  const char *format,
  ...);
dt_rc_e
DTCONFIG_API1
tabchief_reply_header(
  dtparam_t *param,
  dtipc_t *ipc,
  const char *title,
  const char *host);
dt_rc_e
DTCONFIG_API1
tabchief_reply_trailer(
  dtparam_t *param,
  dtipc_t *ipc);
dt_rc_e
DTCONFIG_API1
tabchief_reply_message(
  dtparam_t *param,
  dtipc_t *ipc,
  const char *message);

/*----in restart.c:----*/

dt_rc_e
DTCONFIG_API1
tabchief_restart(
  dtparam_t *param,
  dtipc_t *ipc,
  const char *command,
  const char *arg);

/*----in save.c:----*/

dt_rc_e
DTCONFIG_API1
tabchief_server_savefile(
  dtparam_t *param,
  dtipc_t *ipc,
  const char *command,
  const char *arg);

/*----in status.c:----*/

dt_rc_e
DTCONFIG_API1
tabchief_status_restart(
  dtparam_t *param,
  dtipc_t *ipc,
  const char *process);
dt_rc_e
DTCONFIG_API1
tabchief_status_win(
  dtparam_t *param,
  const char *process,
  int *win);
dt_rc_e
DTCONFIG_API1
tabchief_status_one(
  dtparam_t *param,
  dtipc_t *ipc,
  const char *process);

/*----in server.c:----*/

dt_rc_e
DTCONFIG_API1
tabchief_server_run(
  dtparam_t *param,
  dtipc_t *ipc,
  const char *command,
  const char *arg);
dt_rc_e
DTCONFIG_API1
tabchief_server(
  dtparam_t *param,
  const char *ipcspec);

/*----in split.c:----*/

int
DTCONFIG_API1
dsched_split(							/* separate string into tokens 		*/
  char *p,								/* string changed in place 			*/
  char c)								/* separator character 				*/;

/*----in testcopy.c:----*/

dt_rc_e
DTCONFIG_API1
tabchief_test_copy(
  dtparam_t *param,
  const char *ipcspec);

/*----in testqs.c:----*/

dt_rc_e
DTCONFIG_API1
tabchief_test_qslocal(
  dtparam_t *param);
dt_rc_e
DTCONFIG_API1
tabchief_test_qsremote(
  dtparam_t *param,
  const char *ipcspec);
#ifdef __cplusplus
}
#endif
