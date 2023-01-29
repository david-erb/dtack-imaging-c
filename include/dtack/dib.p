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

/*----in dib.c:----*/


DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtdib_import(
  dt_ctl_t *ctl,
  const dtdib_t *dib,
  dt_io_f *readfunc,
  void *arg,
  dtxy_t y0,							/* start loading at this row 		*/
  int reverse,
  dtxy_t xe,							/* allocate image with this size 	*/
  dtxy_t ye,
  dtimage_t *image,
  unsigned int pagesize,
  unsigned int npages)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtdib_init(								/* create windows dib header		*/
  dt_ctl_t *ctl,
  dtdib_t *dib,
  dtxy_t ye,
  dtimage_t *image)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtdib_endian(							/* fix windows dib header			*/
  dt_ctl_t *ctl,
  dtdib_t *dib)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtdib_rgb_gray(
  dt_ctl_t *ctl,
  dtdib_rgb_t *rgb,
  dtimage_t *image)
DX_SUFFIX

/* ---------------------- */
#undef DT_DEFARGVAL
#undef DX_SUFFIX
#undef DX_PREFIX
#define DT_DEFARGVAL(A)
#ifdef __cplusplus
}
#endif
