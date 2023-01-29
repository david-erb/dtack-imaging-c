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

/*----in trmios.c:----*/


DX_PREFIX
dt_rc_e
DTCONFIG_API1
dttrmios_keyboard_init(					/* set up keyboard 					*/
  dt_ctl_t *ctl,
  dttrmios_keyboard_t *priv,
  const char *spec,
  long flags)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dttrmios_keyboard_uninit(				/* tear down keyboard				*/
  dt_ctl_t *ctl,
  dttrmios_keyboard_t *priv)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dttrmios_keyboard_scan(					/* get key from keyboard, no wait	*/
  dt_ctl_t *ctl,
  dttrmios_keyboard_t *priv,
  char *buffer,
  int max,
  int *got)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dttrmios_keyboard_read(					/* get key from keyboard			*/
  dt_ctl_t *ctl,
  dttrmios_keyboard_t *priv,
  char *buffer,
  int max,
  int *got)
DX_SUFFIX

/* ---------------------- */
#undef DT_DEFARGVAL
#undef DX_SUFFIX
#undef DX_PREFIX
#define DT_DEFARGVAL(A)
#ifdef __cplusplus
}
#endif
