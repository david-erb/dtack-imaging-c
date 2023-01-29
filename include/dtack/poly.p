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

/*----in poly.c:----*/


DX_PREFIX
dt_rc_e 
dtpoly_2printe(
  dt_ctl_t *ctl,						/* environment control				*/
  dtpoly_2_t *spoly,
  FILE *file,
  char *pf)
DX_SUFFIX

DX_PREFIX
dt_rc_e 
dtpoly_2printf(
  dt_ctl_t *ctl,						/* environment control				*/
  dtpoly_2_t *spoly,
  FILE *file,
  char *pf)
DX_SUFFIX

DX_PREFIX
dt_rc_e 
dtpoly_2ident(
  dt_ctl_t *ctl,						/* environment control				*/
  dtpoly_2_t *spoly)
DX_SUFFIX

DX_PREFIX
dt_rc_e 
dtpoly_2xform(
  dt_ctl_t *ctl,						/* environment control				*/
  dtpoly_2_t *spoly,
  dtpoint_2d_t *pi,
  int n,
  dtpoint_2d_t *po)
DX_SUFFIX

DX_PREFIX
dt_rc_e 
dtpoly_2mate(
  dt_ctl_t *ctl,						/* environment control				*/
  dtpoly_2_t *spoly,
  dtpoint_2d_t *p1,
  dtpoint_2d_t *p2)
DX_SUFFIX

/* ---------------------- */
#undef DT_DEFARGVAL
#undef DX_SUFFIX
#undef DX_PREFIX
#define DT_DEFARGVAL(A)
#ifdef __cplusplus
}
#endif
