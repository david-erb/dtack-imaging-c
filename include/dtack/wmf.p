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

/*----in wmf.c:----*/


DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtwmf_placeable_endian(					/* fix windows metafile header		*/
  dt_ctl_t *ctl,
  dtwmf_placeable_t *placeable)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtwmf_import(
  dt_ctl_t *ctl,
  dt_io_f *readfunc,
  void *arg,
  dtxy_t xe,							/* allocate image with this size 	*/
  dtxy_t ye,
  dtimage_t *image,
  unsigned int pagesize,
  unsigned int npages)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtwmf_placeable_init(					/* create makefile placeable header	*/
  dt_ctl_t *ctl,
  dtwmf_placeable_t *placeable,
  dtimage_t *image)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtwmf_metahead_init(					/* init metafile header				*/
  dt_ctl_t *ctl,
  dtwmf_metahead_t *metahead,
  dtimage_t *image)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtwmf_export(
  dt_ctl_t *ctl,
  dt_io_f *writefunc,
  void *arg,
  dtimage_t *image,
  dtxy_t xpos,
  dtxy_t ypos)
DX_SUFFIX

DX_PREFIX
dt_rc_e 
DTCONFIG_API1
dtwmf_metaname(
  dt_ctl_t *ctl,
  int code,
  char *string,
  int lmax)
DX_SUFFIX

/* ---------------------- */
#undef DT_DEFARGVAL
#undef DX_SUFFIX
#undef DX_PREFIX
#define DT_DEFARGVAL(A)
#ifdef __cplusplus
}
#endif
