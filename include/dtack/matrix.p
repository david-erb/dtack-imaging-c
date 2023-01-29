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

/*----in matrix3d.c:----*/


DX_PREFIX
dt_rc_e 
dtmatrix_3dident(
  dt_ctl_t *ctl,						/* environment control 				*/
  dtmatrix_3d_t *mat)
DX_SUFFIX

DX_PREFIX
dt_rc_e 
dtmatrix_3dcopy(
  dt_ctl_t *ctl,						/* environment control 				*/
  dtmatrix_3d_t *mati,
  dtmatrix_3d_t *mato)
DX_SUFFIX

DX_PREFIX
dt_rc_e 
dtmatrix_3dxform(
  dt_ctl_t *ctl,						/* environment control 				*/
  dtmatrix_3d_t *mat,
  dtpoint_3d_t *pi,
  int n,
  int persp,
  dtpoint_3d_t *po)
DX_SUFFIX

DX_PREFIX
dt_rc_e 
dtmatrix_3dtranslate(
  dt_ctl_t *ctl,						/* environment control 				*/
  dtmatrix_3d_t *mati,
  dtmatrix_3d_t *mato,
  double dx,
  double dy,
  double dz)
DX_SUFFIX

DX_PREFIX
dt_rc_e 
dtmatrix_3drotate(
  dt_ctl_t *ctl,						/* environment control 				*/
  dtmatrix_3d_t *mati,
  dtmatrix_3d_t *mato,
  dtmatrix_3d_axis_t axis,
  double angle)
DX_SUFFIX

DX_PREFIX
dt_rc_e 
dtmatrix_3dscale(
  dt_ctl_t *ctl,						/* environment control 				*/
  dtmatrix_3d_t *mati,
  dtmatrix_3d_t *mato,
  double sx,
  double sy,
  double sz)
DX_SUFFIX

DX_PREFIX
dt_rc_e 
dtmatrix_3dcombine(
  dt_ctl_t *ctl,						/* environment control 				*/
  dtmatrix_3d_t *mati,
  dtmatrix_3d_t *mato,
  dtmatrix_3d_t *matn)
DX_SUFFIX

DX_PREFIX
dt_rc_e 
dtmatrix_3dprinte(
  dt_ctl_t *ctl,						/* environment control 				*/
  dtmatrix_3d_t *mat,
  FILE *file,
  char *prefix)
DX_SUFFIX

DX_PREFIX
dt_rc_e 
dtmatrix_3dprintf(
  dt_ctl_t *ctl,						/* environment control 				*/
  dtmatrix_3d_t *mat,
  FILE *file,
  char *prefix)
DX_SUFFIX

DX_PREFIX
dt_rc_e 
dtmatrix_3dinvert(
  dt_ctl_t *ctl,						/* environment control 				*/
  dtmatrix_3d_t *mati,
  dtmatrix_3d_t *mato)
DX_SUFFIX

DX_PREFIX
dt_rc_e 
dtmatrix_3deye(
  dt_ctl_t *ctl,						/* environment control 				*/
  dtmatrix_3d_t *mati,
  dtmatrix_3d_t *mato,
  double distance,
  double bearing,
  double azimuth)
DX_SUFFIX

DX_PREFIX
dt_rc_e 
dtmatrix_3dpersp(
  dt_ctl_t *ctl,						/* environment control 				*/
  dtmatrix_3d_t *mati,
  dtmatrix_3d_t *mato,
  double s,
  double d,
  double f)
DX_SUFFIX

DX_PREFIX
dt_rc_e 
dtmatrix_3dview(
  dt_ctl_t *ctl,						/* environment control 				*/
  dtmatrix_3d_t *mati,
  dtmatrix_3d_t *mato,
  double xmin,
  double xmax,
  double ymin,
  double ymax,
  int vcx,
  int vcy,
  int vsx,
  int vsy)
DX_SUFFIX

DX_PREFIX
dt_rc_e 
dtmatrix_3dmate(
  dt_ctl_t *ctl,						/* environment control 				*/
  dtmatrix_3d_t *mat,
  dtpoint_3d_t *p1,
  dtpoint_3d_t *p2)
DX_SUFFIX

/*----in matrix2d.c:----*/


DX_PREFIX
dt_rc_e 
dtmatrix_2dident(
  dt_ctl_t *ctl,						/* environment control 				*/
  dtmatrix_2d_t *mat)
DX_SUFFIX

DX_PREFIX
dt_rc_e 
dtmatrix_2dxform(
  dt_ctl_t *ctl,						/* environment control 				*/
  dtmatrix_2d_t *mat,
  dtpoint_2d_t *pi,
  int n,
  dtpoint_2d_t *po)
DX_SUFFIX

DX_PREFIX
dt_rc_e 
dtmatrix_2dtranslate(
  dt_ctl_t *ctl,						/* environment control 				*/
  dtmatrix_2d_t *mati,
  dtmatrix_2d_t *mato,
  double dx, 
  double dy)
DX_SUFFIX

DX_PREFIX
dt_rc_e 
dtmatrix_2drotate(
  dt_ctl_t *ctl,						/* environment control 				*/
  dtmatrix_2d_t *mati,
  dtmatrix_2d_t *mato,
  double angle)
DX_SUFFIX

DX_PREFIX
dt_rc_e 
dtmatrix_2dscale(
  dt_ctl_t *ctl,						/* environment control 				*/
  dtmatrix_2d_t *mati,
  dtmatrix_2d_t *mato,
  double sx, 
  double sy)
DX_SUFFIX

DX_PREFIX
dt_rc_e 
dtmatrix_2dcombine(
  dt_ctl_t *ctl,						/* environment control 				*/
  dtmatrix_2d_t *mati,
  dtmatrix_2d_t *mato,
  dtmatrix_2d_t *matn)
DX_SUFFIX

DX_PREFIX
dt_rc_e 
dtmatrix_2dprintf(
  dt_ctl_t *ctl,						/* environment control 				*/
  dtmatrix_2d_t *mat,
  FILE *file,
  char *prefix)
DX_SUFFIX

DX_PREFIX
dt_rc_e 
dtmatrix_2dpoly2(
  dt_ctl_t *ctl,						/* environment control 				*/
  dtmatrix_2d_t *mat,
  dtpoly_2_t *spoly)
DX_SUFFIX

DX_PREFIX
dt_rc_e 
dtmatrix_2dspolyi(
  dt_ctl_t *ctl,						/* environment control 				*/
  dtmatrix_2d_t *mat,
  dtpoly_2_t *spoly)
DX_SUFFIX

DX_PREFIX
dt_rc_e 
dtmatrix_2dinvert(
  dt_ctl_t *ctl,						/* environment control 				*/
  dtmatrix_2d_t *mati,
  dtmatrix_2d_t *mato)
DX_SUFFIX

DX_PREFIX
dt_rc_e
dtmatrix_2dmate(
  dt_ctl_t *ctl,						/* environment control 				*/
  dtmatrix_2d_t *mat,
  dtpoint_2d_t *p1,
  dtpoint_2d_t *p2)
DX_SUFFIX

DX_PREFIX
dt_rc_e 
dtmatrix_2dfrom_poly2(
  struct dtmatrix_2d_t *mat,
  dtpoly_2_t *spoly)
DX_SUFFIX

/*----in matrix.c:----*/


DX_PREFIX
dt_rc_e 
dtmatrix_lu_decompose(
  dt_ctl_t *ctl,						/* environment control 				*/
  void **va,							/* matrix data 						*/
  int n,								/* matrix dimension 				*/
  dtntype_e ntype,						/* matrix data type 				*/
  dt1d_i_t *indices,
  double *d,
  int *singular)
DX_SUFFIX

DX_PREFIX
dt_rc_e 
dtmatrix_lu_substitute(
  dt_ctl_t *ctl,						/* environment control 				*/
  void **va,							/* matrix data 						*/
  int n,								/* matrix dimension 				*/
  dtntype_e ntype,						/* matrix data type 				*/
  dt1d_i_t *indices,
  double *b)
DX_SUFFIX

/* ---------------------- */
#undef DT_DEFARGVAL
#undef DX_SUFFIX
#undef DX_PREFIX
#define DT_DEFARGVAL(A)
#ifdef __cplusplus
}
#endif
