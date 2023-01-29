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

/*----in f.c:----*/


DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtf_init(								/* initialize structure				*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtf_t *f)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtf_uninit(								/* uninit structure					*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtf_t *f)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtf_set(								/* set param to value				*/
  const char *keyword,
  const char *value)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtf_get(								/* get param's value				*/
  const char *keyword,
  const char **value)
DX_SUFFIX

DX_PREFIX
FILE
DTCONFIG_API1
*dtf_open(								/* fake fopen						*/
  const char *filename,
  const char *mode)
DX_SUFFIX

DX_PREFIX
int
DTCONFIG_API1
dtf_read(								/* fake fread						*/
  void *buf,
  unsigned int size,
  unsigned int n,
  FILE *file)
DX_SUFFIX

DX_PREFIX
int
DTCONFIG_API1
dtf_close(								/* fake fclose						*/
  FILE *file)
DX_SUFFIX

DX_PREFIX
int
DTCONFIG_API1
dtf_error(								/* fake ferror						*/
  FILE *file)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtf_rev(								/* deliver revision 				*/
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
#ifdef __cplusplus
}
#endif
