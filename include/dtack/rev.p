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

/*----in rev.c:----*/


DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtrev_base(
  dt_ctl_t *ctl,
  const char **rev_string,
  int *rev_major,
  int *rev_minor,
  long *rev_date)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtrev_say1(								/* say one rev						*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtrev_f *rev)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtrev_say(								/* say one rev						*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtrev_f **rev)
DX_SUFFIX

DX_PREFIX
void
DTCONFIG_API1
dtrev_dbg1(								/* dbg one rev						*/
  dt_ctl_t *ctl,						/* environment control 				*/
  unsigned long mask,
  dtrev_f *rev)
DX_SUFFIX

DX_PREFIX
void
DTCONFIG_API1
dtrev_dbg(								/* dbg list of revs					*/
  dt_ctl_t *ctl,						/* environment control 				*/
  unsigned long mask,
  dtrev_f **rev)
DX_SUFFIX

/* ---------------------- */
#undef DT_DEFARGVAL
#undef DX_SUFFIX
#undef DX_PREFIX
#define DT_DEFARGVAL(A)
#ifdef __cplusplus
}
#endif
