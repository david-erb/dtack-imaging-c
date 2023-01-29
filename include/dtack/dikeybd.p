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

/*----in dikeybd.c:----*/

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtdikeybd_waitable(
  dt_ctl_t *ctl,
  dtdikeybd_t *dikeybd)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtdikeybd_scanq(
  dt_ctl_t *ctl,
  dtdikeybd_t *dikeybd,
  char *key)							/* key which is down or 0 if none */
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtdikeybd_wait(
  dt_ctl_t *ctl,
  dtdikeybd_t *dikeybd,
  long timeout,
  char *key)							/* key which is down or 0 if timeout */
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtdikeybd_isdown(
  dt_ctl_t *ctl,
  dtdikeybd_t *dikeybd,
  int vkey,
  int *isdown)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtdikeybd_dbgdown(
  dt_ctl_t *ctl,
  dtdikeybd_t *dikeybd,
  const char *f)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtdikeybd_uninit(
  dt_ctl_t *ctl,
  dtdikeybd_t *dikeybd)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtdikeybd_init(
  dt_ctl_t *ctl,
  dtdikeybd_t *dikeybd)
DX_SUFFIX

DX_PREFIX
void
DTCONFIG_API1
dtdikeybd_translate_error(
  dt_ctl_t *ctl,
  dtdikeybd_t *dikeybd,
  long hr,
  char *message,
  int max)
DX_SUFFIX

DX_PREFIX
   dt_rc_e
DTCONFIG_API1
dtdikeybd_rev(								/* deliver revision 				*/
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
