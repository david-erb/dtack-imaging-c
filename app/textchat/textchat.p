#ifdef __cplusplus
extern "C" {
#endif

/*----in ask.c:----*/

dt_rc_e 
DTCONFIG_API1
textchat_ask(							/* ask audio server something		*/
  textchat_t *textchat,
  const char *ident,					/* audio client ident 				*/
  const char *request,					/* the given request string 		*/
  dtdg_client_t *dgclient)				/* client object holding response 	*/;

/*----in cmd.c:----*/

dt_rc_e
DTCONFIG_API1
textchat_cmd_ident(
  dt_ctl_t *ctl,
  dtdg_server_t *dgserver,
  void *arg,
  const char *command,
  char *rest,
  char *message,
  int message_max);
dt_rc_e
DTCONFIG_API1
textchat_cmd_state(
  dt_ctl_t *ctl,
  dtdg_server_t *dgserver,
  void *arg,
  const char *command,
  char *rest,
  char *message,
  int message_max);
dt_rc_e
DTCONFIG_API1
textchat_cmd_talk(
  dt_ctl_t *ctl,
  dtdg_server_t *dgserver,
  void *arg,
  const char *command,
  char *rest,
  char *message,
  int message_max);
dt_rc_e
DTCONFIG_API1
textchat_cmd_listen(
  dt_ctl_t *ctl,
  dtdg_server_t *dgserver,
  void *arg,
  const char *command,
  char *rest,
  char *message,
  int message_max);
dt_rc_e
DTCONFIG_API1
textchat_cmd_idle(
  dt_ctl_t *ctl,
  dtdg_server_t *dgserver,
  void *arg,
  const char *command,
  char *rest,
  char *message,
  int message_max);
dt_rc_e
DTCONFIG_API1
textchat_cmd_privacy(
  dt_ctl_t *ctl,
  dtdg_server_t *dgserver,
  void *arg,
  const char *command,
  char *rest,
  char *message,
  int message_max);
dt_rc_e
DTCONFIG_API1
textchat_cmd_partner(
  dt_ctl_t *ctl,
  dtdg_server_t *dgserver,
  void *arg,
  const char *command,
  char *rest,
  char *message,
  int message_max);

/*----in converse.c:----*/

dt_rc_e
DTCONFIG_API1
textchat_converse(
  textchat_t *textchat,
  const char *talker_ident,
  const char *listener_ident);

/*----in dialog.c:----*/

dt_rc_e 
DTCONFIG_API1
textchat_dialog_update(
  textchat_t *textchat);
dt_rc_e 
DTCONFIG_API1
textchat_dialog_poll(
  textchat_t *textchat,
  char **request);
dt_rc_e 
DTCONFIG_API1
textchat_dialog_init(
  textchat_t *textchat,
  dtparam_t *param);
dt_rc_e 
DTCONFIG_API1
textchat_dialog_uninit(
  textchat_t *textchat);

/*----in graphics.c:----*/

dt_rc_e 
DTCONFIG_API1
textchat_graphics_breathe(
  textchat_t *textchat);
dt_rc_e 
DTCONFIG_API1
textchat_graphics_update(
  textchat_t *textchat);
dt_rc_e 
DTCONFIG_API1
textchat_graphics_poll(
  textchat_t *textchat,
  char **request);
dt_rc_e 
DTCONFIG_API1
textchat_graphics_init(
  textchat_t *textchat,
  dtparam_t *param);
dt_rc_e 
DTCONFIG_API1
textchat_graphics_uninit(
  textchat_t *textchat);

/*----in gui.c:----*/

dt_rc_e 
DTCONFIG_API1
textchat_op_gui(
  dtparam_t *param);

/*----in ident.c:----*/

dt_rc_e 
DTCONFIG_API1
textchat_ident_to_spec(					/* convert ident to dg ipc spec		*/
  textchat_t *textchat,
  const char *ident,					/* audio client ident 				*/
  char *spec,							/* returned spec 					*/
  int spec_max)							/* max length of spec 				*/;

/*----in init.c:----*/

dt_rc_e 
DTCONFIG_API1
textchat_init(
  textchat_t **ptextchat,
  dtparam_t *param);
dt_rc_e 
DTCONFIG_API1
textchat_uninit(
  textchat_t *textchat);

/*----in log.c:----*/

dt_rc_e
DTCONFIG_API1		
textchat_log_setup(						/* set up file logging				*/
  textchat_t *textchat);
dt_rc_e
DTCONFIG_API1		
textchat_log_quit(						/* quit file logging				*/
  textchat_t *textchat);
void
DTCONFIG_API1		
textchat_log_string(					/* output message to log file		*/
  dt_ctl_t *ctl,						/* environment control 				*/
  const char *string,
  int length);
void
DTCONFIG_API1		
textchat_dbg(							/* output debug message				*/
  dt_ctl_t *ctl,						/* environment control 				*/
  const char *caller,					/* caller's name 					*/
  long class_bits,						/* debug message class 				*/
  const char *format,					/* message format, like sprintf() 	*/
  va_list ap)							/* format substitutions 			*/;
dt_rc_e
DTCONFIG_API1		
textchat_err(							/* handle errors					*/
  dt_ctl_t *ctl,						/* environment control 				*/
  const char *caller,					/* caller's name 					*/
  const char *format,					/* message format, like sprintf() 	*/
  va_list ap)							/* format substitutions 			*/;
void
DTCONFIG_API1		
textchat_say(							/* output console message			*/
  dt_ctl_t *ctl,						/* environment control 				*/
  const char *caller,					/* caller's name 					*/
  const char *format,					/* message format, like sprintf() 	*/
  va_list ap)							/* format substitutions 			*/;

/*----in request.c:----*/

dt_rc_e 
DTCONFIG_API1
textchat_request_handle(
  textchat_t *textchat,
  dtdg_server_t *dgserver,
  char *request);
dt_rc_e 
DTCONFIG_API1
textchat_request_poll(
  textchat_t *textchat,
  dtdg_server_t *dgserver);

/*----in state.c:----*/

dt_rc_e 
DTCONFIG_API1
textchat_state(							/* get current audio state 			*/
  textchat_t *textchat);

/*----in update.c:----*/

dt_rc_e 
DTCONFIG_API1
textchat_update(
  textchat_t *textchat);
#ifdef __cplusplus
}
#endif
