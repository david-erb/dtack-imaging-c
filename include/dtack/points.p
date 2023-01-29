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

/*----in points.c:----*/


DX_PREFIX
dt_rc_e 
dtpoints_2dprintf(
  dt_ctl_t *ctl,						/* environment control 				*/
  dtpoint_2d_t *points,					/* points array 					*/
  dt1d_i_t n,							/* number of points in list 		*/
  FILE *file,							/* file to print to 				*/
  const char *format)
DX_SUFFIX

DX_PREFIX
dt_rc_e 
dtpoints_2dcompare(
  dt_ctl_t *ctl,						/* environment control 				*/
  dtpoint_2d_t *points1,				/* points array 					*/
  dtpoint_2d_t *points2,				/* points array 					*/
  dt1d_i_t n,							/* number of points in list 		*/
  double epsilon,						/* maximum allowable difference 	*/
  int *cmp,								/* returned -1, 0 or 1 				*/
  dt1d_i_t *k)							/* index of first difference 		*/
DX_SUFFIX

/* ---------------------- */
#undef DT_DEFARGVAL
#undef DX_SUFFIX
#undef DX_PREFIX
#define DT_DEFARGVAL(A)
#ifdef __cplusplus
}
#endif
