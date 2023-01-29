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

/*----in ptz.c:----*/


DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtptz_init(								/* initialize ptz object 			*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtptz_t *ptz,							/* ptz object returned				*/
  const char *spec,						/* ptz specification 				*/
  unsigned long flags)					/* process control flags 			*/
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtptz_uninit(
  dt_ctl_t *ctl,
  dtptz_t *ptz)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtptz_stop(
  dt_ctl_t *ctl,
  dtptz_t *ptz)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtptz_command(							/* load text into overlay			*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtptz_t *ptz,							/* ptz object structure 			*/
  const char *cmd)						/* cmd to display 					*/
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtptz_pela_init(
  dt_ctl_t *ctl,
  dtptz_t *ptz,
  const char *spec)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtptz_pela_rev(							/* deliver revision 				*/
  dt_ctl_t *ctl,						/* environment control 				*/
  const char **rev_string,				/* returned rev string 				*/
  int *rev_major,						/* returned rev major 				*/
  int *rev_minor,						/* returned rev minor 				*/
  long *rev_date)						/* returned rev date 				*/
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtptz_ipc_init(
  dt_ctl_t *ctl,
  dtptz_t *ptz,
  const char *spec)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtptz_ipc_rev(							/* deliver revision 				*/
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
