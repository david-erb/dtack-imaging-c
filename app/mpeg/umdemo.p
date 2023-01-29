#ifdef __cplusplus
extern "C" {
#endif

/*----in arg.c:----*/

u_rc_e 
U_API
umdemo_init(
  dtparam_t *param,
  umdemo_t **umdemop);
u_rc_e 
U_API
umdemo_uninit(
  dtparam_t *param,
  umdemo_t *umdemo);

/*----in command.c:----*/

u_rc_e 
U_API
umdemo_command_index_from_string(
  u_ctl_t *ctl,
  const char *command,
  int *indx,
  char *response,
  int maxresponse);
u_rc_e 
U_API
umdemo_command_index_from_key(
  u_ctl_t *ctl,
  char key,
  int *indx,
  char *response,
  int maxresponse);
u_rc_e 
U_API
umdemo_command_string_from_key(
  u_ctl_t *ctl,
  char key,
  const char **command,
  char *response,
  int maxresponse);
u_rc_e 
U_API
umdemo_command_func_from_key(
  u_ctl_t *ctl,
  char key,
  umdemo_command_f **function,
  char *response,
  int maxresponse);
u_rc_e 
U_API
umdemo_command_prompt(
  u_ctl_t *ctl,
  char *prompt,
  int max);
u_rc_e 
U_API
umdemo_command(
  umdemo_t *umdemo,
  char *command);

/*----in control.c:----*/

u_rc_e 
U_API
umdemo_op_control_transact(
  dtparam_t *param,
  const char *command,
  char *reply,
  int maxreply);
u_rc_e 
U_API
umdemo_op_control_keybd(
  dtparam_t *param);
u_rc_e 
U_API
umdemo_op_control(
  dtparam_t *param);

/*----in feed.c:----*/

u_rc_e 
U_API
umdemo_feed(
  u_ctl_t *ctl,							/* environment control 				*/
  umdec_t *umdec,						/* umdec object 					*/
  void *arg,							/* caller's unchanged argument 		*/
  void *buffer,							/* buffer to put data in 			*/
  long want,							/* maximum number of bytes wanted 	*/
  long *got)							/* actual number of bytes returned 	*/;

/*----in feedfifo.c:----*/

u_rc_e 
U_API
umdemo_fifo_command(
  umdemo_t *umdemo);
u_rc_e 
U_API
umdemo_fifo_poll(
  umdemo_t *umdemo);

/*----in feedipc.c:----*/

u_rc_e 
U_API
umdemo_ipc_command(
  umdemo_t *umdemo);
u_rc_e 
U_API
umdemo_ipc_poll(
  umdemo_t *umdemo);

/*----in feedkey.c:----*/

u_rc_e 
U_API
umdemo_keybd_poll(
  umdemo_t *umdemo);

/*----in fifo.c:----*/

u_rc_e 
U_API
umdemo_op_fifo(
  dtparam_t *param);

/*----in file.c:----*/

u_rc_e 
U_API
umdemo_op_file(
  dtparam_t *param);

/*----in frame.c:----*/

u_rc_e
U_API
umdemo_frame(
  u_ctl_t *ctl,							/* environment control 				*/
  umdec_t *umdec,						/* umdec object 					*/
  void *arg,							/* caller's unchanged argument 		*/
  umdec_frametype_e frametype,			/* type of frame being given 		*/
  void **buffer,						/* buffers containing frame data	*/
  int xe,								/* frame width 						*/
  int ye)								/* frame height						*/;

/*----in keybd.c:----*/

u_rc_e 
U_API
umdemo_keybd_line(
  u_ctl_t *ctl,
  dtkeybd_t *keybd,
  char *line,
  int max);

/*----in server.c:----*/

u_rc_e 
U_API
umdemo_op_server(
  dtparam_t *param);

/*----in status.c:----*/

u_rc_e 
U_API
umdemo_status_string(
  umdemo_t *umdemo,
  char *string,
  int max);
u_rc_e 
U_API
umdemo_status_say(
  umdemo_t *umdemo);

/*----in stuff.c:----*/

u_rc_e 
U_API
umdemo_op_stuff(
  dtparam_t *param);

/*----in tell.c:----*/

u_rc_e
U_API
umdemo_op_tell(
  dtparam_t *param);
#ifdef __cplusplus
}
#endif
