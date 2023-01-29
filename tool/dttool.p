#ifdef __cplusplus
extern "C" {
#endif

/*----in parse.c:----*/

dt_rc_e
dttool_parse(
  dtparam_t *param,
  char *f,
  char *usage,
  int argc,
  char *argv[],
  int *n);
dt_rc_e
dttool_usage(
  dtparam_t *param,
  char *string);
dt_rc_e 
dttool_parse_op(						/* check ops for this program		*/
  dt_ctl_t *ctl,
  dtparam_t *param,
  const char **ops,
  char *msg,
  const char **start);
dt_rc_e 
dttool_parse_precision(					/* check precision for this program	*/
  dt_ctl_t *ctl,
  dtparam_t *param,
  const char **precisions,
  char *message,
  const char **start);
dt_rc_e 
dttool_parse_arg_string(				/* get string argument				*/
  dtparam_t *param,
  dt_enter_arg_t *arg,
  int k,
  const char *dflt,
  char *message,
  const char **start);
dt_rc_e 
dttool_parse_arg_long(					/* get argument as long				*/
  dtparam_t *param,
  dt_enter_arg_t *arg,
  int k,
  const char *dflt,
  long *longval,
  char *message,
  const char **start);
dt_rc_e 
dttool_parse_param_color(				/* parse color						*/
  dtparam_t *param,
  const char *dflt,
  double *color,
  char *message,
  const char **start);
#ifdef __cplusplus
}
#endif
