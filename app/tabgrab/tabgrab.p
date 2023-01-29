#ifdef __cplusplus
extern "C" {
#endif

/*----in client.c:----*/

dt_rc_e
DTCONFIG_API1
tabgrab_client(
  dtparam_t *param,
  const char *command);

/*----in grab.c:----*/

dt_rc_e
DTCONFIG_API1
tabgrab_grab(
  tabgrab_t *tabgrab);

/*----in init.c:----*/

dt_rc_e
DTCONFIG_API1
tabgrab_uninit(
  tabgrab_t *tabgrab);
dt_rc_e
DTCONFIG_API1
tabgrab_init(
  tabgrab_t *tabgrab,
  dtparam_t *param);

/*----in server.c:----*/

dt_rc_e
DTCONFIG_API1
tabgrab_server(
  dtparam_t *param);
#ifdef __cplusplus
}
#endif
