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

/*----in msw.c:----*/


DX_PREFIX
dt_rc_e 
DTCONFIG_API1
dtmsw_message_string(					/* convert message to string		*/
  dt_ctl_t *ctl,
  int message,
  const char **string)
DX_SUFFIX

DX_PREFIX
dt_rc_e
dtmsw_pidwin(							/* make invisible pid window		*/
  dt_ctl_t *ctl,
  int *pidwin)
DX_SUFFIX

DX_PREFIX
void
DTCONFIG_API1
dtmsw_dbg_string(
  dt_ctl_t *ctl,
  const char *string,
  int length)
DX_SUFFIX

DX_PREFIX
void
DTCONFIG_API1
dtmsw_err_string(
  dt_ctl_t *ctl,
  const char *string,
  int length)
DX_SUFFIX

DX_PREFIX
void
DTCONFIG_API1
dtmsw_say_string(
  dt_ctl_t *ctl,
  const char *string,
  int length)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtmsw_pause(
  dt_ctl_t *ctl)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtmsw_yield(
  dt_ctl_t *ctl)
DX_SUFFIX

DX_PREFIX
dt_rc_e 
DTCONFIG_API1
dtmsw_kick(
  dt_ctl_t *ctl,
  dtmsw_kick_e kick)
DX_SUFFIX

DX_PREFIX
dt_rc_e 
DTCONFIG_API1
dtmsw_status(
  dt_ctl_t *ctl,
  const char *string)
DX_SUFFIX

DX_PREFIX
dt_rc_e 
DTCONFIG_API1
dtmsw_feedback(							/* output feedback line 			*/
  dt_ctl_t *ctl,
  unsigned long flags,
  const char *string)
DX_SUFFIX

/* ---------------------- */
#undef DT_DEFARGVAL
#undef DX_SUFFIX
#undef DX_PREFIX
#define DT_DEFARGVAL(A)
#ifdef __cplusplus
}
#endif
