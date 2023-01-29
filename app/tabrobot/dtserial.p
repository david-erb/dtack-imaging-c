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

/*----in serial.c:----*/


DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtserial_open(
  dt_ctl_t *ctl,
  dtserial_t *serial,
  const char *spec)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtserial_close(
  dt_ctl_t *ctl,
  dtserial_t *serial)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtserial_write(
  dt_ctl_t *ctl,
  dtserial_t *serial,
  void *buf,
  long n)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtserial_setup(
  dt_ctl_t *ctl,
  dtserial_t *serial,
  const char *string)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtserial_open(
  dt_ctl_t *ctl,
  dtserial_t *serial,
  const char *spec)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtserial_close(
  dt_ctl_t *ctl,
  dtserial_t *serial)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtserial_write(
  dt_ctl_t *ctl,
  dtserial_t *serial,
  void *buf,
  long n)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtserial_setup(
  dt_ctl_t *ctl,
  dtserial_t *serial,
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
