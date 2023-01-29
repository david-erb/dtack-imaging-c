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

/*----in parse.c:----*/


DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtxchg_parse(							/* parse external image spec		*/
  dt_ctl_t *ctl,						/* environment control 				*/
  const char *spec,						/* external image spec				*/
  dtxchg_e *which,						/* returned exchanger type 			*/
  char **server,						/* returned server part 			*/
  char **id,							/* returned image identifier 		*/
  dt_err_t *err,						/* returned error code 				*/
  char *message,						/* returned error message 			*/
  int lmax)								/* maximum length of error message 	*/
DX_SUFFIX

/*----in xchg_dtm.c:----*/


DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtxchg_dtm_export_image(
  dt_ctl_t *ctl,
  const char *server,
  const char *id,
  dtimage_t *image,
  dtxy_t xpos,
  dtxy_t ypos)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtxchg_dtm_export_image(
  dt_ctl_t *ctl,
  const char *server,
  const char *id,
  dtimage_t *image,
  dtxy_t xpos,
  dtxy_t ypos)
DX_SUFFIX

/*----in xchg_win.c:----*/


DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtxchg_win_export_image(
  dt_ctl_t *ctl,
  const char *server,
  const char *id,
  dtimage_t *image,
  int xpos,
  int ypos)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtxchg_win_import_image(
  dt_ctl_t *ctl,
  const char *server,
  const char *id,
  dtimage_t *image,
  unsigned int pagesize,
  unsigned int npages)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtxchg_win_export_lut8(
  dt_ctl_t *ctl,
  const char *server,
  const char *id,
  const dtlut8_t *lut,
  int start,
  int count)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtxchg_win_import_lut8(
  dt_ctl_t *ctl,
  const char *server,
  const char *id,
  dtlut8_t *lut,
  int start,
  int count)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtxchg_win_stop(
  dt_ctl_t *ctl,
  const char *server)
DX_SUFFIX

/*----in xchg_wxx.c:----*/


DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtxchg_wxx_export_image(
  dt_ctl_t *ctl,
  const char *server,
  const char *id,
  dtimage_t *image,
  int xpos,
  int ypos)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtxchg_wxx_import_image(
  dt_ctl_t *ctl,
  const char *server,
  const char *id,
  dtimage_t *image,
  unsigned int pagesize,
  unsigned int npages)
DX_SUFFIX

/*----in xchg_jpg.c:----*/


DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtxchg_jpeg_import_file(				/* get image from jpeg file			*/
  dt_ctl_t *ctl,
  char *filename,
  dtimage_t *image)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtxchg_jpeg_import_memory(				/* get image from jpeg memory		*/
  dt_ctl_t *ctl,
  void *memory,
  long memory_max,
  long *memory_used,
  dtimage_t *image)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtxchg_jpeg_export_file(				/* get image from jpeg file			*/
  dt_ctl_t *ctl,
  const char *quality_string,
  const char *filename,
  dtimage_t *image)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtxchg_jpeg_export_memory(				/* get image from jpeg memory		*/
  dt_ctl_t *ctl,
  const char *quality_string,
  void *memory,
  long memory_max,
  long *memory_used,
  dtimage_t *image)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtxchg_jpeg_import2(					/* get image from jpeg				*/
  dt_ctl_t *ctl,
  char *filename,
  unsigned long flags,
  void *memory,
  long memory_max,
  long *memory_used,
  dtimage_t *image)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtxchg_jpeg_import(						/* get image from jpeg				*/
  dt_ctl_t *ctl,
  char *filename,
  void *memory,
  long memory_max,
  long *memory_used,
  dtimage_t *image)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtxchg_jpeg_import_file(				/* get image from jpeg file			*/
  dt_ctl_t *ctl,
  char *filename,
  dtimage_t *image)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtxchg_jpeg_import_file2(				/* get image from jpeg file			*/
  dt_ctl_t *ctl,
  char *filename,
  unsigned long flags,
  dtimage_t *image)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtxchg_jpeg_import_memory(				/* get image from jpeg memory		*/
  dt_ctl_t *ctl,
  void *memory,
  long memory_max,
  long *memory_used,
  dtimage_t *image)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtxchg_jpeg_export(						/* get image from jpeg file			*/
  dt_ctl_t *ctl,
  const char *spec,
  const char *filename,
  void *memory,
  long memory_max,
  long *memory_used,
  dtimage_t *image)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtxchg_jpeg_export_file(				/* get image from jpeg file			*/
  dt_ctl_t *ctl,
  const char *quality_string,
  const char *filename,
  dtimage_t *image)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtxchg_jpeg_export_memory(				/* get image from jpeg memory		*/
  dt_ctl_t *ctl,
  const char *quality_string,
  void *memory,
  long memory_max,
  long *memory_used,
  dtimage_t *image)
DX_SUFFIX

/*----in xchg_scr.c:----*/


DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtxchg_scr_export_image(
  dt_ctl_t *ctl,
  void *vscr,
  const char *windowname,
  dtimage_t *image,
  int x0,
  int y0)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtxchg_scr_import_image(
  dt_ctl_t *ctl,
  void *vscr,
  const char *windowname,
  dtimage_t *image,
  unsigned int pagesize,
  unsigned int npages)
DX_SUFFIX

/*----in xchg_can.c:----*/


DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtxchg_canvas_export_image(
  dt_ctl_t *ctl,
  void *vcanvas,
  dtimage_t *image,
  int x0,
  int y0)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtxchg_canvas_import_image(
  dt_ctl_t *ctl,
  void *vcanvas,
  dtimage_t *image,
  int x0,
  int y0)
DX_SUFFIX

/* ---------------------- */
#undef DT_DEFARGVAL
#undef DX_SUFFIX
#undef DX_PREFIX
#define DT_DEFARGVAL(A)
#ifdef __cplusplus
}
#endif
