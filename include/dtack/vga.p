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

/*----in vga.c:----*/


DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtvga_lookup_resolution(
  dt_ctl_t *ctl,
  char *string,
  int *xresolution,
  int *yresolution,
  const char ***resolutions)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtvga_lookup_chipset(
  dt_ctl_t *ctl,
  char *string,
  dtvga_chipset_e *chipset,
  const char ***chipsets)
DX_SUFFIX

/* ---------------------- */
#undef DT_DEFARGVAL
#undef DX_SUFFIX
#undef DX_PREFIX
#define DT_DEFARGVAL(A)
#ifdef __cplusplus
}
#endif
