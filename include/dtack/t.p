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

/*----in check.c:----*/


DX_PREFIX
dt_rc_e
DTCONFIG_API2
dtt_check(
  dt_ctl_t *ctl,
  int cond,
  char *format,
  ...)
DX_SUFFIX

DX_PREFIX
dt_rc_e 
DTCONFIG_API1
dtt_check_op(							/* check ops for this program		*/
  dt_ctl_t *ctl,
  dtparam_t *param,
  const char **ops,
  char *msg,
  const char **start)
DX_SUFFIX

DX_PREFIX
dt_rc_e 
DTCONFIG_API1
dtt_check_op2(							/* check ops for this program		*/
  dt_ctl_t *ctl,
  dtparam_t *param,
  const char **ops,
  int *op,
  char *msg,
  const char **start)
DX_SUFFIX

DX_PREFIX
dt_rc_e 
DTCONFIG_API1
dtt_check_datadir(						/* make sure -datadir is directory	*/
  dt_ctl_t *ctl,
  dtparam_t *param,
  char *msg,
  const char **start)
DX_SUFFIX

DX_PREFIX
dt_rc_e 
dtt_check_arg(							/* get argument						*/
  dtparam_t *param,
  dt_enter_arg_t *arg,
  int k,
  void *extra,
  char *message,
  const char **start)
DX_SUFFIX

/*----in gen256.c:----*/


DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtt_gen_color256(					/* generate 16x16 squares 			*/
  dt_ctl_t *ctl,
  dtimage_t *image)
DX_SUFFIX

/*----in util1.c:----*/


DX_PREFIX
dt_rc_e
DTCONFIG_API1
dttutil_read_runadj(
  dt_ctl_t *ctl,
  FILE *file,
  char *desc,
  int *ncols,
  int *nrows,
  int *nruns,
  int *nblobs,
  dtntype_s4_t *expect_labels,
  dtntype_s4_t *expect_areas,
  dtntype_s4_t *expect_sums,
  dt_ntype_b08_t *data,
  unsigned long *flags,
  int *good)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtt_image_synthetic(					/* gen image and call function		*/
  dtparam_t *param,
  const char *select,
  dt_ntype_e ntype,					
  dtt_image_f *do_image,				/* function to call				    */
  void *arg)							/* caller's arg to pass to function	*/
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtt_image_list(							/* read/gen images and do them		*/
  dtparam_t *param,
  const char **list,
  const char *select,
  dt_ntype_e ntype,
  dtt_image_f *do_image,				/* function to call				    */
  void *arg)							/* caller's arg to pass to function	*/
DX_SUFFIX

/*----in time.c:----*/


DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtt_time_report(
  dt_ctl_t *ctl,
  dtos_time_t *mark1,
  dtos_time_t *mark2,
  const char *f)
DX_SUFFIX

/*----in raw.c:----*/


DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtt_raw_export(
  dt_ctl_t *ctl,
  dtimage_t *image,
  void (*put)(char *, int, int, int, int))
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtt_raw_import(
  dt_ctl_t *ctl,
  dtimage_t *image,
  void (*get)(char *, int, int, int, int))
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtt_raw_report(
  dt_ctl_t *ctl,
  char *id,
  int loops,
  dtos_time_t *mark1,
  dtos_time_t *mark2,
  dtos_time_t *mark3,
  dtos_time_t *mark4,
  long *cmprc,
  int *assert_fail)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtt_raw(
  dt_ctl_t *ctl,
  dtparam_t *param,
  int loops,
  void (*put)(char *, int, int, int, int),
  void (*get)(char *, int, int, int, int),
  dtos_time_t *mark1,
  dtos_time_t *mark2,
  dtos_time_t *mark3,
  dtos_time_t *mark4,
  long *cmprc)
DX_SUFFIX

/* ---------------------- */
#undef DT_DEFARGVAL
#undef DX_SUFFIX
#undef DX_PREFIX
#define DT_DEFARGVAL(A)
#ifdef __cplusplus
}
#endif
