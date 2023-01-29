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

/*----in qnx4.c:----*/


DX_PREFIX
void
DTCONFIG_API1
dtqnx4_out_string(
  dt_ctl_t *ctl,
  const char *string,
  int length)
DX_SUFFIX

DX_PREFIX
void
DTCONFIG_API1
dtqnx4_dbg_string(
  dt_ctl_t *ctl,
  const char *string,
  int length)
DX_SUFFIX

DX_PREFIX
void
DTCONFIG_API1
dtqnx4_err_string(
  dt_ctl_t *ctl,
  const char *string,
  int length)
DX_SUFFIX

DX_PREFIX
void
DTCONFIG_API1
dtqnx4_say_string(
  dt_ctl_t *ctl,
  const char *string,
  int length)
DX_SUFFIX

/* ---------------------- */
#undef DT_DEFARGVAL
#undef DX_SUFFIX
#undef DX_PREFIX
#define DT_DEFARGVAL(A)
#ifdef __cplusplus
}
#endif
