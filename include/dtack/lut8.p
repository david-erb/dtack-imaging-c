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

/*----in lut8xchg.c:----*/


DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtlut8_export(							/* export lut to file or window		*/
  dt_ctl_t *ctl,
  const dtlut8_t *lut,
  const char *spec,
  int start,
  int count)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtlut8_import(							/* import lut to file or window		*/
  dt_ctl_t *ctl,
  dtlut8_t *lut,
  const char *spec,
  int start,
  int count)
DX_SUFFIX

/*----in lut8.c:----*/


DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtlut8_standard(
  dt_ctl_t *ctl,
  dtlut8_t *lut)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtlut8_name_to_index(
  dt_ctl_t *ctl,
  const char *name,
  int *idx)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtlut8_index_to_name(
  dt_ctl_t *ctl,
  int idx,
  char *name)
DX_SUFFIX

/*----in lut8_mat.c:----*/


DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtlut8_tag(								/* set tags in lut					*/
  dt_ctl_t *ctl,
  dtlut8_t *lut)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtlut8_sort(							/* sort color array					*/
  dt_ctl_t *ctl,
  dtlut8_t *lut)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtlut8_search(							/* search for color					*/
  dt_ctl_t *ctl,
  dtlut8_t *color,
  dtlut8_t *lut,
  int *idx1,
  int *idx2)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtlut8_closest(							/* search for color					*/
  dt_ctl_t *ctl,
  dtlut8_t *color,
  dtlut8_t *lut,
  int *idx)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtlut8_match(							/* match user to display colors		*/
  dt_ctl_t *ctl,
  dtlut8_t *user,
  dtlut8_t *display,
  unsigned char *user_to_display,
  unsigned char *display_to_user)
DX_SUFFIX

/* ---------------------- */
#undef DT_DEFARGVAL
#undef DX_SUFFIX
#undef DX_PREFIX
#define DT_DEFARGVAL(A)
#ifdef __cplusplus
}
#endif
