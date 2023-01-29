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

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtftp_init(								/* allocate structure resources 	*/
  dt_ctl_t *ctl,
  dtftp_t *ftp,
  const char *control_spec,
  const char *data_spec,
  const char *user,
  const char *password)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtftp_uninit(							/* release structure resources 		*/
  dt_ctl_t *ctl,
  dtftp_t *ftp)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtftp_register(
  dt_ctl_t *ctl,
  dtftp_t *ftp)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtftp_connect(
  dt_ctl_t *ctl,
  dtftp_t *ftp)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtftp_chdir(							/* change remote directory 			*/
  dt_ctl_t *ctl,
  dtftp_t *ftp,
  const char *dirname)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtftp_delete(							/* delete remote file 				*/
  dt_ctl_t *ctl,
  dtftp_t *ftp,
  const char *filename)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtftp_mkdir(							/* make remote directory			*/
  dt_ctl_t *ctl,
  dtftp_t *ftp,
  const char *dirname)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtftp_put_start(						/* start data transfer 				*/
  dt_ctl_t *ctl,
  dtftp_t *ftp,
  const char *filename)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtftp_put_data(							/* send some data 					*/
  dt_ctl_t *ctl,
  dtftp_t *ftp,
  const void *data,
  long length)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtftp_put_finish(						/* wait for transfer to complete	*/
  dt_ctl_t *ctl,
  dtftp_t *ftp)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtftp_disconnect(
  dt_ctl_t *ctl,
  dtftp_t *ftp)
DX_SUFFIX

/* ---------------------- */
#undef DT_DEFARGVAL
#undef DX_SUFFIX
#undef DX_PREFIX
#define DT_DEFARGVAL(A)
#ifdef __cplusplus
}
#endif
