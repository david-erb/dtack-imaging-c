/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
  ! Copyright (C) 1998 Dtack Inc. All Rights Reserved
  ! To use this file, you must have signed a license agreement with Dtack Inc.
  ! Under no circumstances may you redistribute this file.
  ! This software is provided AS IS with no warranty expressed or implied.
  ! Dtack Inc. accepts no liability for use or misuse of this file.
  ! http://www.dtack.com  dtack@dtack.com  telephone +360.417.6700
  ! Dtack Inc, P.O. Box 3061, Port Angeles, WA USA 98362
  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */

/*..........................................................................
| TITLE
| new
|
| END
 *..........................................................................*/

/*..........................................................................
  CHANGES
  19990130 DE added to the library
 *..........................................................................*/

#include <dtack/base.h>
DT_RCSID("source/low $RCSfile: matrix2d.c,v $ $Revision: 1.8 $");
#include <dtack/point.h>				/* xyz points 						*/
#include <dtack/poly.h>					/* polynomial transformation		*/
#include <dtack/matrix.h>				/* matrix operations 				*/

/*-------------------------------------------------------------------------
 * related functions: dtmatrix_3d
 * - 3d geometric transformations
 * - matrix elements are assigned:
 *      0  1  2  3
 *      4  5  6  7
 *      8  9 10 11
 *     12 13 14 15 
 *-------------------------------------------------------------------------*/
 
#define A11 0							/* matrix elements					*/
#define A12 1
#define A13 2
#define A14 3
#define A21 4
#define A22 5
#define A23 6
#define A24 7
#define A31 8
#define A32 9
#define A33 10
#define A34 11
#define A41 12
#define A42 13
#define A43 14
#define A44 15

/*-----------------------------------------------------------------------	
 * FUNCTION NAME
 *   dtmatrix_3dident
 * PURPOSE
 *   generate identity transform matrix
 * ARGUMENTS
 *   dtmatrix_3d_t *mat;			matrix to be set to identity
 * RETURNED VALUES
 *   elements argument "mat" set to identity transform
 * ERROR HANDLING
 *   no possible errors
 * PRINCIPAL EXTERNAL REFERENCES
 *   none
 * OUTLINE OF ALGORITHM
 *   set all elements of mat to:
 *      1  0  0  0
 *      0  1  0  0
 *      0  0  1  0
 *      0  0  0  1
 * CAVEATS
 *-----------------------------------------------------------------------*/

dt_rc_e 
dtmatrix_3dident(
  dt_ctl_t *ctl,						/* environment control 				*/
  dtmatrix_3d_t *mat)
{
  int i;
  for (i=0; i<16; i++)
    mat->coeff[i] = 0.0;
  mat->coeff[A11] =
  mat->coeff[A22] =
  mat->coeff[A33] =
  mat->coeff[A44]= 1.0;
  return DT_RC_GOOD;
}

/*-----------------------------------------------------------------------	
 * FUNCTION NAME
 *   dtmatrix_3dcopy
 * PURPOSE
 *   copy contents of matrix
 * ARGUMENTS
 *   dtmatrix_3d_t *mati;			source matrix
 *   dtmatrix_3d_t *mato;			destination matrix
 * RETURNED VALUES
 *   elements argument "mato" set to be the same as mati
 * ERROR HANDLING
 *   no possible errors
 * PRINCIPAL EXTERNAL REFERENCES
 *   none
 * OUTLINE OF ALGORITHM
 * CAVEATS
 *-----------------------------------------------------------------------*/

dt_rc_e 
dtmatrix_3dcopy(
  dt_ctl_t *ctl,						/* environment control 				*/
  dtmatrix_3d_t *mati,
  dtmatrix_3d_t *mato)
{
  int i;
  for (i=0; i<16; i++)
    mato->coeff[i] = mati->coeff[i];
  return DT_RC_GOOD;
}

/*-----------------------------------------------------------------------	
 * FUNCTION NAME
 *   dtmatrix_3dxform
 * PURPOSE
 *   transform list of points under matrix
 * ARGUMENTS
 *   dtmatrix_3d_t *mat;		matrix describing transformation
 *   dtpoint_3d_t *pi;		list of input points
 *   int n;						number of points in input list
 *   dtpoint_3d_t *po;		list of output points
 * RETURNED VALUES
 *   output array contains transformed points
 * ERROR HANDLING
 *   no possible errors
 * PRINCIPAL EXTERNAL REFERENCES
 *   none
 * OUTLINE OF ALGORITHM
 *   on entire list of points pi, performs matrix operation:
 *      po = pi * mat
 *   where
 *     pi is matrix:
 *       [x y z 1]
 *     mat is transformation matrix:
 *       a  b  c  d
 *       e  f  g  h
 *       i  j  k  l
 *       m  n  o  p
 *     po is matrix:
 *       [x y z w] (w dropped and lost after transform)
 *   po may be same as pi
 *   Copies tag field from input to output without change.
 * CAVEATS
 *-----------------------------------------------------------------------*/

dt_rc_e 
dtmatrix_3dxform(
  dt_ctl_t *ctl,						/* environment control 				*/
  dtmatrix_3d_t *mat,
  dtpoint_3d_t *pi,
  int n,
  int persp,
  dtpoint_3d_t *po)
{
  int i;
  double x, y, z, w;
  for (i=0; i<n; i++)					/* for all points in list			*/
  {
    x = mat->coeff[A11] * pi[i].x +
        mat->coeff[A21] * pi[i].y +
        mat->coeff[A31] * pi[i].z +
        mat->coeff[A41] * pi[i].w;
    y = mat->coeff[A12] * pi[i].x +
        mat->coeff[A22] * pi[i].y +
        mat->coeff[A32] * pi[i].z +
        mat->coeff[A42] * pi[i].w;
    z = mat->coeff[A13] * pi[i].x +
        mat->coeff[A23] * pi[i].y +
        mat->coeff[A33] * pi[i].z +
        mat->coeff[A43] * pi[i].w;
    w = mat->coeff[A14] * pi[i].x +
        mat->coeff[A24] * pi[i].y +
        mat->coeff[A34] * pi[i].z +
        mat->coeff[A44] * pi[i].w;

    if (persp)							/* wants perspective division?		*/
    {
      po[i].x = x / w;
      po[i].y = y / w;
      po[i].z = z / w;
      po[i].w = 1.0;
    }
    else								/* doesn't want perspective?		*/
    {
      po[i].x = x;
      po[i].y = y;
      po[i].z = z;
      po[i].w = w;
    }

    po[i].t = pi[i].t;
  }
  return DT_RC_GOOD;
}

/*-----------------------------------------------------------------------	
 * FUNCTION NAME
 *   dtmatrix_3dtranslate
 * PURPOSE
 *   apply x, y and z translation to matrix
 * ARGUMENTS
 *   dtmatrix_3d_t *mati;		matrix describing initial transformation
 *   dtmatrix_3d_t *mato;		matrix describing new transformation
 *   double dx;					x shift
 *   double dy;					y shift
 *   double dz;					z shift
 * RETURNED VALUES
 *   output matrix describing combination of input matrix with translation
 * ERROR HANDLING
 *   no possible errors
 * PRINCIPAL EXTERNAL REFERENCES
 *   none
 * OUTLINE OF ALGORITHM
 *   assembles translation matrix matn:
 *      1  0  0  0
 *      0  1  0  0
 *      0  0  1  0
 *     dx dy dz  1
 *   then performs matrix multiplication:
 *     mato = matn * mati
 *   mato may be same as mati
 * CAVEATS
 *-----------------------------------------------------------------------*/

dt_rc_e 
dtmatrix_3dtranslate(
  dt_ctl_t *ctl,						/* environment control 				*/
  dtmatrix_3d_t *mati,
  dtmatrix_3d_t *mato,
  double dx,
  double dy,
  double dz)
{
  DT_F("dtmatrix_3dtranslate");
  int i;
  dtmatrix_3d_t matn;
  dtmatrix_3dident(ctl, &matn);		/* initialize the translate matrix	*/
  matn.coeff[A41] = dx;					/* set up the translation			*/
  matn.coeff[A42] = dy;
  matn.coeff[A43] = dz;
  DT_Q(dtmatrix_3dcombine,(ctl,		/* combine matrices					*/
    mati, &matn, mato));
  return DT_RC_GOOD;
}

/*-----------------------------------------------------------------------	
 * FUNCTION NAME
 *   dtmatrix_3drotate
 * PURPOSE
 *   apply rotation to matrix
 * ARGUMENTS
 *   dtmatrix_3d_t *mati;		matrix describing initial transformation
 *   dtmatrix_3d_t *mato;		matrix describing new transformation
 *   dtmatrix_3d_axis_t axis;		axis about which rotation is done
 *   double angle;				rotation angle in radians
 * RETURNED VALUES
 *   output matrix describing combination of input matrix with rotation
 * ERROR HANDLING
 *   no possible errors
 * PRINCIPAL EXTERNAL REFERENCES
 *   none
 * OUTLINE OF ALGORITHM
 *   for axis x, assembles rotation matrix matn:
 *       1    0   0  0
 *       0  cos sin  0
 *       0 -sin cos  0
 *       0   0    0  1
 *   for axis y, assembles rotation matrix matn:
 *     cos   0 sin  0
 *       0   1   0  0
 *    -sin   0 cos  0
 *       0   0   0  1
 *   for axis z, assembles rotation matrix matn:
 *     cos sin   0  0
 *    -sin cos   0  0
 *       0   0   1  0
 *       0   0   0  1
 *   then performs matrix multiplication:
 *     mato = matn * mati
 *   mato may be same as mati
 * CAVEATS
 *-----------------------------------------------------------------------*/

dt_rc_e 
dtmatrix_3drotate(
  dt_ctl_t *ctl,						/* environment control 				*/
  dtmatrix_3d_t *mati,
  dtmatrix_3d_t *mato,
  dtmatrix_3d_axis_t axis,
  double angle)
{
  DT_F("dtmatrix_3drotate");
  int i;
  dtmatrix_3d_t matn;
  dtmatrix_3dident(ctl, &matn);		/* initialize the rotate matrix	*/
  switch (axis)							/* set up the rotation			*/
  {
    case DTMATRIX_3D_AXIS_X:
      matn.coeff[A22] =
      matn.coeff[A33] = cos(angle);
      matn.coeff[A23] = sin(angle);
      matn.coeff[A32] = -matn.coeff[A23];
    break;
    case DTMATRIX_3D_AXIS_Y:
      matn.coeff[A11] =
      matn.coeff[A33] = cos(angle);
      matn.coeff[A13] = sin(angle);
      matn.coeff[A31] = -matn.coeff[A13];
    break;
    case DTMATRIX_3D_AXIS_Z:
      matn.coeff[A11] =
      matn.coeff[A22] = cos(angle);
      matn.coeff[A12] = sin(angle);
      matn.coeff[A21] = -matn.coeff[A12];
    break;
  }
  DT_Q(dtmatrix_3dcombine,(ctl,		/* combine matrices					*/
    mati, &matn, mato));
  return DT_RC_GOOD;
}

/*-----------------------------------------------------------------------	
 * FUNCTION NAME
 *   dtmatrix_3dscale
 * PURPOSE
 *   apply x, y and z scales to matrix
 * ARGUMENTS
 * dtmatrix_3d_t *mati;		matrix describing initial transformation
 * dtmatrix_3d_t *mato;		matrix describing new transformation
 * double sx;					x scale factor
 * double sy;					y scale factor
 * double sz;					z scale factor
 * RETURNED VALUES
 *   output matrix describing combination of input matrix with scaling
 * ERROR HANDLING
 *   no possible errors
 * PRINCIPAL EXTERNAL REFERENCES
 *   none
 * OUTLINE OF ALGORITHM
 *   assembles scale matrix matn:
 *     sx   0   0  0
 *      0  sy   0  0
 *      0   0  sz  0
 *      0   0   0  1
 *   then performs matrix multiplication:
 *     mato = matn * mati
 *   mato may be same as mati
 * CAVEATS
 *-----------------------------------------------------------------------*/

dt_rc_e 
dtmatrix_3dscale(
  dt_ctl_t *ctl,						/* environment control 				*/
  dtmatrix_3d_t *mati,
  dtmatrix_3d_t *mato,
  double sx,
  double sy,
  double sz)
{
  DT_F("dtmatrix_3dscale");
  int i;
  dtmatrix_3d_t matn;
  dtmatrix_3dident(ctl, &matn);		/* initialize the rotate matrix		*/
  matn.coeff[A11] = sx;					/* set up the scale					*/
  matn.coeff[A22] = sy;
  matn.coeff[A33] = sz;
  DT_Q(dtmatrix_3dcombine,(ctl,		/* combine matrices					*/
    mati, &matn, mato));
  return DT_RC_GOOD;
}

/*-----------------------------------------------------------------------	
 * FUNCTION NAME
 *   dtmatrix_3dcombine
 * PURPOSE
 *   combine (multiply) two 4x4 matrices
 * ARGUMENTS
 *   dtmatrix_3d_t *mati;		matrix describing first transformation
 *   dtmatrix_3d_t *matn;		matrix describing second transformation
 *   dtmatrix_3d_t *mato;		matrix describing combined transformation
 * RETURNED VALUES
 *   output matrix containing combined transformation
 * ERROR HANDLING
 *   no possible errors
 * PRINCIPAL EXTERNAL REFERENCES
 *   none
 * OUTLINE OF ALGORITHM
 *   performs matrix multiplication:
 *     mato = matn * mati
 *   mato may be same as mati and/or matn
 * CAVEATS
 *-----------------------------------------------------------------------*/

dt_rc_e 
dtmatrix_3dcombine(
  dt_ctl_t *ctl,						/* environment control 				*/
  dtmatrix_3d_t *mati,
  dtmatrix_3d_t *mato,
  dtmatrix_3d_t *matn)
{
  int i;
  double x, y;
  dtmatrix_3d_t matt;
										/*- - - - - - - - - - - - - - - - - */
  matt.coeff[A11] =
    mati->coeff[A11] * matn->coeff[A11] +
    mati->coeff[A12] * matn->coeff[A21] +
    mati->coeff[A13] * matn->coeff[A31] +
    mati->coeff[A14] * matn->coeff[A41];
  matt.coeff[A12] =
    mati->coeff[A11] * matn->coeff[A12] +
    mati->coeff[A12] * matn->coeff[A22] +
    mati->coeff[A13] * matn->coeff[A32] +
    mati->coeff[A14] * matn->coeff[A42];
  matt.coeff[A13] =
    mati->coeff[A11] * matn->coeff[A13] +
    mati->coeff[A12] * matn->coeff[A23] +
    mati->coeff[A13] * matn->coeff[A33] +
    mati->coeff[A14] * matn->coeff[A43];
  matt.coeff[A14] =
    mati->coeff[A11] * matn->coeff[A14] +
    mati->coeff[A12] * matn->coeff[A24] +
    mati->coeff[A13] * matn->coeff[A34] +
    mati->coeff[A14] * matn->coeff[A44];
										/*- - - - - - - - - - - - - - - - - */
  matt.coeff[A21] =
    mati->coeff[A21] * matn->coeff[A11] +
    mati->coeff[A22] * matn->coeff[A21] +
    mati->coeff[A23] * matn->coeff[A31] +
    mati->coeff[A24] * matn->coeff[A41];
  matt.coeff[A22] =
    mati->coeff[A21] * matn->coeff[A12] +
    mati->coeff[A22] * matn->coeff[A22] +
    mati->coeff[A23] * matn->coeff[A32] +
    mati->coeff[A24] * matn->coeff[A42];
  matt.coeff[A23] =
    mati->coeff[A21] * matn->coeff[A13] +
    mati->coeff[A22] * matn->coeff[A23] +
    mati->coeff[A23] * matn->coeff[A33] +
    mati->coeff[A24] * matn->coeff[A43];
  matt.coeff[A24] =
    mati->coeff[A21] * matn->coeff[A14] +
    mati->coeff[A22] * matn->coeff[A24] +
    mati->coeff[A23] * matn->coeff[A34] +
    mati->coeff[A24] * matn->coeff[A44];
										/*- - - - - - - - - - - - - - - - - */
  matt.coeff[A31] =
    mati->coeff[A31] * matn->coeff[A11] +
    mati->coeff[A32] * matn->coeff[A21] +
    mati->coeff[A33] * matn->coeff[A31] +
    mati->coeff[A34] * matn->coeff[A41];
  matt.coeff[A32] =
    mati->coeff[A31] * matn->coeff[A12] +
    mati->coeff[A32] * matn->coeff[A22] +
    mati->coeff[A33] * matn->coeff[A32] +
    mati->coeff[A34] * matn->coeff[A42];
  matt.coeff[A33] =
    mati->coeff[A31] * matn->coeff[A13] +
    mati->coeff[A32] * matn->coeff[A23] +
    mati->coeff[A33] * matn->coeff[A33] +
    mati->coeff[A34] * matn->coeff[A43];
  matt.coeff[A34] =
    mati->coeff[A31] * matn->coeff[A14] +
    mati->coeff[A32] * matn->coeff[A24] +
    mati->coeff[A33] * matn->coeff[A34] +
    mati->coeff[A34] * matn->coeff[A44];
										/*- - - - - - - - - - - - - - - - - */
  matt.coeff[A41] =
    mati->coeff[A41] * matn->coeff[A11] +
    mati->coeff[A42] * matn->coeff[A21] +
    mati->coeff[A43] * matn->coeff[A31] +
    mati->coeff[A44] * matn->coeff[A41];
  matt.coeff[A42] =
    mati->coeff[A41] * matn->coeff[A12] +
    mati->coeff[A42] * matn->coeff[A22] +
    mati->coeff[A43] * matn->coeff[A32] +
    mati->coeff[A44] * matn->coeff[A42];
  matt.coeff[A43] =
    mati->coeff[A41] * matn->coeff[A13] +
    mati->coeff[A42] * matn->coeff[A23] +
    mati->coeff[A43] * matn->coeff[A33] +
    mati->coeff[A44] * matn->coeff[A43];
  matt.coeff[A44] =
    mati->coeff[A41] * matn->coeff[A14] +
    mati->coeff[A42] * matn->coeff[A24] +
    mati->coeff[A43] * matn->coeff[A34] +
    mati->coeff[A44] * matn->coeff[A44];

  for (i=0; i<16; i++)					/* copy combined matrix to output	*/
    mato->coeff[i] = matt.coeff[i];

  return DT_RC_GOOD;
}

/*-----------------------------------------------------------------------	
 * FUNCTION NAME
 *   dtmatrix_3dprinte
 * PURPOSE
 *   print matrix in floating notation
 * ARGUMENTS
 *   dtmatrix_3d_t *mat;		matrix to print
 *   FILE *file;				file to print to (may be stdout)
 *   char *prefix;				prefix to go on each line
 * RETURNED VALUES
 *   none
 * ERROR HANDLING
 *   no possible errors
 * PRINCIPAL EXTERNAL REFERENCES
 *   none
 * OUTLINE OF ALGORITHM
 *   prints matrix to file with each line prefixed by given string with
 *   each element printed using %14.6e format
 * CAVEATS
 *-----------------------------------------------------------------------*/

dt_rc_e 
dtmatrix_3dprinte(
  dt_ctl_t *ctl,						/* environment control 				*/
  dtmatrix_3d_t *mat,
  FILE *file,
  char *prefix)
{
  fprintf(file, "%s%14.6e %14.6e %14.6e %14.6e\n",
    prefix, mat->coeff[A11], mat->coeff[A12], mat->coeff[A13], mat->coeff[A14]);
  fprintf(file, "%s%14.6e %14.6e %14.6e %14.6e\n",
    prefix, mat->coeff[A21], mat->coeff[A22], mat->coeff[A23], mat->coeff[A24]);
  fprintf(file, "%s%14.6e %14.6e %14.6e %14.6e\n",
    prefix, mat->coeff[A31], mat->coeff[A32], mat->coeff[A33], mat->coeff[A34]);
  fprintf(file, "%s%14.6e %14.6e %14.6e %14.6e\n",
    prefix, mat->coeff[A41], mat->coeff[A42], mat->coeff[A43], mat->coeff[A44]);
  return DT_RC_GOOD;
}

/*-----------------------------------------------------------------------	
 * FUNCTION NAME
 *   dtmatrix_3dprintf
 * PURPOSE
 *   print matrix in fixed notation
 * ARGUMENTS
 *   dtmatrix_3d_t *mat;		matrix to print
 *   FILE *file;				file to print to (may be stdout)
 *   char *prefix;				prefix to go on each line
 * RETURNED VALUES
 *   none
 * ERROR HANDLING
 *   no possible errors
 * PRINCIPAL EXTERNAL REFERENCES
 *   none
 * OUTLINE OF ALGORITHM
 *   prints matrix to file with each line prefixed by given string with
 *   each element printed using %14.6f format
 * CAVEATS
 *-----------------------------------------------------------------------*/

dt_rc_e 
dtmatrix_3dprintf(
  dt_ctl_t *ctl,						/* environment control 				*/
  dtmatrix_3d_t *mat,
  FILE *file,
  char *prefix)
{
  fprintf(file, "%s%14.6f %14.6f %14.6f %14.6f\n",
    prefix, mat->coeff[A11], mat->coeff[A12], mat->coeff[A13], mat->coeff[A14]);
  fprintf(file, "%s%14.6f %14.6f %14.6f %14.6f\n",
    prefix, mat->coeff[A21], mat->coeff[A22], mat->coeff[A23], mat->coeff[A24]);
  fprintf(file, "%s%14.6f %14.6f %14.6f %14.6f\n",
    prefix, mat->coeff[A31], mat->coeff[A32], mat->coeff[A33], mat->coeff[A34]);
  fprintf(file, "%s%14.6f %14.6f %14.6f %14.6f\n",
    prefix, mat->coeff[A41], mat->coeff[A42], mat->coeff[A43], mat->coeff[A44]);
  return DT_RC_GOOD;
}

/*-----------------------------------------------------------------------	
 * FUNCTION NAME
 *   dtmatrix_3dinvert
 * PURPOSE
 *   invert 4x4 matrix
 * ARGUMENTS
 *   dtmatrix_3d_t *mati;		input matrix
 *   dtmatrix_3d_t *mato;		output matrix, may be same as mati
 * RETURNED VALUES
 *   output matrix has inverted input matrix
 * ERROR HANDLING
 *   no possible errors
 * PRINCIPAL EXTERNAL REFERENCES
 *   none
 * OUTLINE OF ALGORITHM
 *   Matrix is LU decomposed then the back substituted twice with identity
 *   columns to create second order polynomial transformation.
 * CAVEATS
 *-----------------------------------------------------------------------*/

dt_rc_e 
dtmatrix_3dinvert(
  dt_ctl_t *ctl,						/* environment control 				*/
  dtmatrix_3d_t *mati,
  dtmatrix_3d_t *mato)
{
  DT_F("dtmatrix_3dinvert");
#define N 4
  int i, j;
  double aa[N*N];
  double *a[N];
  double d;
  int singular;
  int k[N];
  double u[N];
  dt_rc_e  rc = DT_RC_GOOD;

  a[0] = &aa[0];
  a[0][0] = mati->coeff[A11];			/* matrix into a different form		*/
  a[0][1] = mati->coeff[A12];
  a[0][2] = mati->coeff[A13];
  a[0][3] = mati->coeff[A14];

  a[1] = &aa[4];
  a[1][0] = mati->coeff[A21];
  a[1][1] = mati->coeff[A22];
  a[1][2] = mati->coeff[A23];
  a[1][3] = mati->coeff[A24];

  a[2] = &aa[8];
  a[2][0] = mati->coeff[A31];
  a[2][1] = mati->coeff[A32];
  a[2][2] = mati->coeff[A33];
  a[2][3] = mati->coeff[A34];

  a[3] = &aa[12];
  a[3][0] = mati->coeff[A41];
  a[3][1] = mati->coeff[A42];
  a[3][2] = mati->coeff[A43];
  a[3][3] = mati->coeff[A44];

  dtmatrix_3dident(ctl, mato);		/* clear output matrix				*/

  DT_C(dtmatrix_lu_decompose,(ctl,		/* LU decompose						*/
    (void **)a, N, DTNTYPE_F2, k, &d,
    &singular));

  if (singular)
	DT_CLEANUP((ctl, F,
      "matrix is singular"));

  u[0] = 1.0;							/* unit vector for solution			*/
  u[1] = 0.0;
  u[2] = 0.0;
  u[3] = 0.0;
  DT_C(dtmatrix_lu_substitute,(ctl,		/* solve							*/
    (void **)a, N, DTNTYPE_F2, k, u));
  mato->coeff[A11] = u[0];				/* put answer into output matrix	*/
  mato->coeff[A21] = u[1];
  mato->coeff[A31] = u[2];
  mato->coeff[A41] = u[3];

  u[0] = 0.0;							/* unit vector for solution			*/
  u[1] = 1.0;
  u[2] = 0.0;
  u[3] = 0.0;
  DT_C(dtmatrix_lu_substitute,(ctl,		/* solve							*/
    (void **)a, N, DTNTYPE_F2, k, u));
  mato->coeff[A12] = u[0];				/* put answer into output matrix	*/
  mato->coeff[A22] = u[1];
  mato->coeff[A32] = u[2];
  mato->coeff[A42] = u[3];

  u[0] = 0.0;							/* unit vector for solution			*/
  u[1] = 0.0;
  u[2] = 1.0;
  u[3] = 0.0;
  DT_C(dtmatrix_lu_substitute,(ctl,		/* solve							*/
    (void **)a, N, DTNTYPE_F2, k, u));
  mato->coeff[A13] = u[0];				/* put answer into output matrix	*/
  mato->coeff[A23] = u[1];
  mato->coeff[A33] = u[2];
  mato->coeff[A43] = u[3];
cleanup:
  return rc;
}

/*-----------------------------------------------------------------------	
 * FUNCTION NAME
 *   dtmatrix_3deye
 * PURPOSE
 *   compute a matrix to transform 0.0 to an eye coordinate system
 * ARGUMENTS
 *   dtmatrix_3d_t *mati;		starting transform
 *   dtmatrix_3d_t *mato;		returned transform
 *   double distance;
 *   double bearing;			0 to 360 degrees, 0 is north facing
 *   double azimuth;			0 to 90 degrees above horizon, 0 is horizontal
 * RETURNED VALUES
 *   output matrix
 * ERROR HANDLING
 *   no possible errors
 * PRINCIPAL EXTERNAL REFERENCES
 *   none
 * OUTLINE OF ALGORITHM
 *   rotate around Z axis by bearing, rotate around X axis by azimuth, then
 *   shift by eye distance
 * CAVEATS
 *-----------------------------------------------------------------------*/

dt_rc_e 
dtmatrix_3deye(
  dt_ctl_t *ctl,						/* environment control 				*/
  dtmatrix_3d_t *mati,
  dtmatrix_3d_t *mato,
  double distance,
  double bearing,
  double azimuth)
{
  DT_F("dtmatrix_3deye");

  if (bearing != 0.0)
    dtmatrix_3drotate(ctl, mati, mato,		/* rotate image plane about z		*/
      DTMATRIX_3D_AXIS_Z,
      bearing);
  else
    dtmatrix_3dcopy(ctl, mati, mato);
  if (azimuth != 0.0)
    dtmatrix_3drotate(ctl, mato, mato, /* rotate image plane to azimuth	*/
      DTMATRIX_3D_AXIS_X,
      azimuth);
  if (distance != 0.0)
    dtmatrix_3dtranslate(ctl, mato, mato,	/* get eyeball distance				*/
      0.0, 0.0, distance);
#ifdef NONO
  dtmatrix_3dscale(ctl, mato, mato,	/* scale for viewing transform		*/
    0.75*distance,
    0.75*distance,
    1.0);
#endif
  return DT_RC_GOOD;
}

/*-----------------------------------------------------------------------	
 * FUNCTION NAME
 *   dtmatrix_3dpersp
 * PURPOSE
 *   compute a matrix to do linear part of perspective transformation
 * ARGUMENTS
 *   dtmatrix_3d_t *mati;		starting transform
 *   dtmatrix_3d_t *mato;		returned transform
 *   double s;
 *   double d;
 *   double f;
 * RETURNED VALUES
 *   output matrix
 * ERROR HANDLING
 *   no possible errors
 * PRINCIPAL EXTERNAL REFERENCES
 *   none
 * OUTLINE OF ALGORITHM
 * CAVEATS
 *-----------------------------------------------------------------------*/

dt_rc_e 
dtmatrix_3dpersp(
  dt_ctl_t *ctl,						/* environment control 				*/
  dtmatrix_3d_t *mati,
  dtmatrix_3d_t *mato,
  double s,
  double d,
  double f)
{
  DT_F("dtmatrix_3dpersp");
  dtmatrix_3d_t matn;
  dtmatrix_3dident(ctl, &matn);		/* initialize the translate matrix	*/
  matn.coeff[A33] = s / (d*(1.0-d/f));
  matn.coeff[A34] = s / d;
  matn.coeff[A43] = -s / (1.0-d/f);
  matn.coeff[A44] = 0.0;
  DT_Q(dtmatrix_3dcombine,(ctl,		/* combine matrices					*/
    mati, &matn, mato));
  return DT_RC_GOOD;
}
#undef PSECT

/*-----------------------------------------------------------------------	
 * FUNCTION NAME
 *   dtmatrix_3dview
 * PURPOSE
 *   compute a matrix to scale and shift data to center in viewport
 * ARGUMENTS
 *   dtmatrix_3d_t *mati;		starting transform
 *   dtmatrix_3d_t *mato;		returned transform
 *   double xmin, xmax;			data range to map to raster
 *   double ymin, ymax;
 *   int vcx, vcy;				raster center
 *   int vsx, vsy;				raster size
 * RETURNED VALUES
 *   output matrix
 * ERROR HANDLING
 *   no possible errors
 * PRINCIPAL EXTERNAL REFERENCES
 *   none
 * OUTLINE OF ALGORITHM
 *   Assumes raster 0,0 is upper left.
 *   Scale computed as ratio of given extremes to screen size.
 *   Scales about data center, translates data center to screen center.
 *   Scale does not change aspect ratio of data.
 *   Does not affect Z component.
 * CAVEATS
 *-----------------------------------------------------------------------*/

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
{
  double xscale, yscale, scale;
  dtmatrix_3dtranslate(ctl, mati, mato,	/* translate to data center			*/
    -(xmax+xmin)/2.0,
    -(ymax+ymin)/2.0,
    0.0);
  xscale = (double)vsx / (xmax-xmin);	/* calculate scaling factor			*/
  yscale = (double)vsy / (ymax-ymin);
  scale = DT_MIN(xscale, yscale);		/* choose most restrictive scale	*/
  dtmatrix_3dscale(ctl, mato, mato,		/* scale to new size				*/
    scale, scale, 1.0);
  dtmatrix_3dtranslate(ctl, mato, mato,	/* translate to screen center		*/
    (double)vcx,
    (double)vcy,
    0.0);
  return DT_RC_GOOD;
}


/*-----------------------------------------------------------------------	
 * FUNCTION NAME
 *   dtmatrix_3dmate
 * PURPOSE
 *   calculate second order polynomial coefficients by mating two node lists
 * ARGUMENTS
 *   dtmatrix_3d_t *mat;	    matrix coefficients describing transform
 *   dtpoint_3d_t *p1;		first input list of nodes
 *   dtpoint_3d_t *p2;		input list of nodes
 * RETURNED VALUES
 *   output mat structure contains coefficients describing transform
 * ERROR HANDLING
 *   no possible errors
 * PRINCIPAL EXTERNAL REFERENCES
 *   iv_mat_alloc
 *   iv_mat_ludec
 *   iv_mat_lusub
 * OUTLINE OF ALGORITHM
 *   Solve two sets of 4 simultaneous equations to get horizontal and
 *   vertical coefficients.
 * CAVEATS
 *   Input arrays p1 and p2 must contain exactly 4 points.
 *   Input array p1 must not contain points which are coincident.
 *   Input array p2 must not contain points which are coincident.
 *-----------------------------------------------------------------------*/

dt_rc_e 
dtmatrix_3dmate(
  dt_ctl_t *ctl,						/* environment control 				*/
  dtmatrix_3d_t *mat,
  dtpoint_3d_t *p1,
  dtpoint_3d_t *p2)
{
  DT_F("dtmatrix_3dmate");
  double aa[N*N];
  double *a[N];
  int indx[4];
  double bx[4], by[4], bz[4];
  double d;
  int singular;
  int i;
  dt_rc_e rc = DT_RC_GOOD;

  for (i=0; i<4; i++)					/* load matrix and RHS's			*/
  {
    bx[i] = p1[i].x;					/* RHS for horizontal				*/
    by[i] = p1[i].y;					/* RHS for vertical					*/
    bz[i] = p1[i].z;					/* RHS for third dimension			*/
    a[i] = &aa[i*N];
    a[i][0] = p2[i].x;
    a[i][1] = p2[i].y;
    a[i][2] = p2[i].z;
    a[i][3] = 1.0;
  }

  DT_C(dtmatrix_lu_decompose,(ctl,		/* LU decompose the matrix			*/
    (void **)a, 4, DTNTYPE_F2, indx, &d,
    &singular));

  if (singular)
	DT_CLEANUP((ctl, F,
      "matrix is singular"));

  DT_C(dtmatrix_lu_substitute,(ctl,		/* solve for horizontal				*/
    (void **)a, 4, DTNTYPE_F2, indx, bx));
  DT_C(dtmatrix_lu_substitute,(ctl,		/* solve for vertical				*/
    (void **)a, 4, DTNTYPE_F2, indx, by));
  DT_C(dtmatrix_lu_substitute,(ctl,		/* solve for third dimension		*/
    (void **)a, 4, DTNTYPE_F2, indx, bz));

  mat->coeff[A11] = bx[0];				/* put coefficients into matrix		*/
  mat->coeff[A12] = by[0];
  mat->coeff[A13] = bz[0];
  mat->coeff[A14] = 0.0;

  mat->coeff[A21] = bx[1];
  mat->coeff[A22] = by[1];
  mat->coeff[A23] = bz[1];
  mat->coeff[A24] = 0.0;

  mat->coeff[A31] = bx[2];
  mat->coeff[A32] = by[2];
  mat->coeff[A33] = bz[2];
  mat->coeff[A34] = 0.0;

  mat->coeff[A41] = bx[3];
  mat->coeff[A42] = by[3];
  mat->coeff[A43] = bz[3];
  mat->coeff[A44] = 1.0;
  
cleanup:
  return rc;
}

/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
  ! Copyright (C) 1998 Dtack Inc. All Rights Reserved
  ! To use this file, you must have signed a license agreement with Dtack Inc.
  ! Under no circumstances may you redistribute this file.
  ! This software is provided AS IS with no warranty expressed or implied.
  ! Dtack Inc. accepts no liability for use or misuse of this file.
  ! http://www.dtack.com  dtack@dtack.com  telephone +360.417.6700
  ! Dtack Inc, P.O. Box 3061, Port Angeles, WA USA 98362
  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */
