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
 * related functions: dtmatrix_2d
 * - 2d geometric transformations
 * - matrix elements are assigned:
 *      0 1 2
 *      3 4 5
 *      6 7 8
 * <changes by> David Erb 5/6/91
 *   - added matinvert
 *-------------------------------------------------------------------------*/
 
/*-----------------------------------------------------------------------	
 * FUNCTION NAME
 *   dtmatrix_2dident
 * PURPOSE
 *   generate identity transform matrix
 * ARGUMENTS
 *   dtmatrix_2d_t *mat;			matrix to be set to identity
 * RETURNED VALUES
 *   elements argument "mat" set to identity transform
 * ERROR HANDLING
 *   no possible errors
 * PRINCIPAL EXTERNAL REFERENCES
 *   none
 * OUTLINE OF ALGORITHM
 *   set all elements of mat to:
 *      1  0  0
 *      0  1  0
 *      0  0  1
 * CAVEATS
 *-----------------------------------------------------------------------*/

dt_rc_e 
dtmatrix_2dident(
  dt_ctl_t *ctl,						/* environment control 				*/
  dtmatrix_2d_t *mat)
{
  int i;
  for (i=0; i<9; i++)
    mat->coeff[i] = 0.0;
  mat->coeff[0] = mat->coeff[4] = mat->coeff[8] = 1.0;

  return DT_RC_GOOD;
}

/*-----------------------------------------------------------------------	
 * FUNCTION NAME
 *   dtmatrix_2dxform
 * PURPOSE
 *   transform list of points under matrix
 * ARGUMENTS
 *   dtmatrix_2d_t *mat;		matrix describing transformation
 *   dtpoint_2d_t *pi;		list of input points
 *   int n;						number of points in input list
 *   dtpoint_2d_t *po;		list of output points
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
 *       [x y 1]
 *     mat is transformation matrix:
 *       a  b  c
 *       d  e  d
 *       g  h  f 
 *     po is matrix:
 *       [x y 0]
 *   internal calculations are done in double precision and rounded off
 *     to nearest integer before assignment to po
 *   po may be same as pi
 * CAVEATS
 *-----------------------------------------------------------------------*/

dt_rc_e 
dtmatrix_2dxform(
  dt_ctl_t *ctl,						/* environment control 				*/
  dtmatrix_2d_t *mat,
  dtpoint_2d_t *pi,
  int n,
  dtpoint_2d_t *po)
{
  int i;
  double x, y;
  for (i=0; i<n; i++)					/* for all points in list			*/
  {
    x = mat->coeff[0] * pi[i].x +
        mat->coeff[3] * pi[i].y +
        mat->coeff[6];
    y = mat->coeff[1] * pi[i].x +
        mat->coeff[4] * pi[i].y +
        mat->coeff[7];
    po[i].x = (int)(x + 0.5);			/* round off						*/
    po[i].y = (int)(y + 0.5);
  }

  return DT_RC_GOOD;
}

/*-----------------------------------------------------------------------	
 * FUNCTION NAME
 *   dtmatrix_2dtranslate
 * PURPOSE
 *   apply x and y translation to matrix
 * ARGUMENTS
 *   dtmatrix_2d_t *mati;		matrix describing initial transformation
 *   dtmatrix_2d_t *mato;		matrix describing new transformation
 *   double dx;					x shift
 *   double dy;					y shift
 * RETURNED VALUES
 *   output matrix describing combination of input matrix with translation
 * ERROR HANDLING
 *   no possible errors
 * PRINCIPAL EXTERNAL REFERENCES
 *   none
 * OUTLINE OF ALGORITHM
 *   assembles translation matrix matn:
 *      1  0  0
 *      0  1  0
 *     dx dy  1
 *   then performs matrix multiplication:
 *     mato = matn * mati
 *   mato may be same as mati
 * CAVEATS
 *-----------------------------------------------------------------------*/

dt_rc_e 
dtmatrix_2dtranslate(
  dt_ctl_t *ctl,						/* environment control 				*/
  dtmatrix_2d_t *mati,
  dtmatrix_2d_t *mato,
  double dx, 
  double dy)
{
  int i;
  dtmatrix_2d_t matn;
  dtmatrix_2dident(ctl, &matn);			/* initialize the translate matrix	*/
  matn.coeff[6] = dx;					/* set up the translation			*/
  matn.coeff[7] = dy;
  dtmatrix_2dcombine(ctl,				/* combine matrices					*/
    mati, &matn, mato);

  return DT_RC_GOOD;
}

/*-----------------------------------------------------------------------	
 * FUNCTION NAME
 *   dtmatrix_2drotate
 * PURPOSE
 *   apply rotation to matrix
 * ARGUMENTS
 *   dtmatrix_2d_t *mati;		matrix describing initial transformation
 *   dtmatrix_2d_t *mato;		matrix describing new transformation
 *   double angle;				rotation angle in radians
 * RETURNED VALUES
 *   output matrix describing combination of input matrix with rotation
 * ERROR HANDLING
 *   no possible errors
 * PRINCIPAL EXTERNAL REFERENCES
 *   none
 * OUTLINE OF ALGORITHM
 *   assembles rotation matrix matn:
 *     cos sin  0
 *    -sin cos  0
 *       0   0  1
 *   then performs matrix multiplication:
 *     mato = matn * mati
 *   mato may be same as mati
 * CAVEATS
 *-----------------------------------------------------------------------*/

dt_rc_e 
dtmatrix_2drotate(
  dt_ctl_t *ctl,						/* environment control 				*/
  dtmatrix_2d_t *mati,
  dtmatrix_2d_t *mato,
  double angle)
{
  int i;
  dtmatrix_2d_t matn;
  dtmatrix_2dident(ctl, &matn);			/* initialize the rotate matrix	*/
  matn.coeff[0] =						/* set up the rotation			*/
  matn.coeff[4] = cos(angle);
  matn.coeff[1] = sin(angle);
  matn.coeff[3] = -matn.coeff[1];
  dtmatrix_2dcombine(ctl,				/* combine matrices					*/
    mati, &matn, mato);

  return DT_RC_GOOD;
}

/*-----------------------------------------------------------------------	
 * FUNCTION NAME
 *   dtmatrix_2dscale
 * PURPOSE
 *   apply x and y scales to matrix
 * ARGUMENTS
 * dtmatrix_2d_t *mati;		matrix describing initial transformation
 * dtmatrix_2d_t *mato;		matrix describing new transformation
 * double sx;					x scale factor
 * double sy;					y scale factor
 * RETURNED VALUES
 *   output matrix describing combination of input matrix with scaling
 * ERROR HANDLING
 *   no possible errors
 * PRINCIPAL EXTERNAL REFERENCES
 *   none
 * OUTLINE OF ALGORITHM
 *   assembles scale matrix matn:
 *     sx   0   0
 *      0  sy   0
 *      0   0   1
 *   then performs matrix multiplication:
 *     mato = matn * mati
 *   mato may be same as mati
 * CAVEATS
 *-----------------------------------------------------------------------*/

dt_rc_e 
dtmatrix_2dscale(
  dt_ctl_t *ctl,						/* environment control 				*/
  dtmatrix_2d_t *mati,
  dtmatrix_2d_t *mato,
  double sx, 
  double sy)
{
  int i;
  dtmatrix_2d_t matn;
  dtmatrix_2dident(ctl, &matn);			/* initialize the rotate matrix	*/
  matn.coeff[0] = sx;					/* set up the scale				*/
  matn.coeff[4] = sy;
  dtmatrix_2dcombine(ctl,				/* combine matrices					*/
    mati, &matn, mato);

  return DT_RC_GOOD;
}

/*-----------------------------------------------------------------------	
 * FUNCTION NAME
 *   dtmatrix_2dcombine
 * PURPOSE
 *   combine two matrices
 * ARGUMENTS
 *   dtmatrix_2d_t *mati;		matrix describing first transformation
 *   dtmatrix_2d_t *matn;		matrix describing second transformation
 *   dtmatrix_2d_t *mato;		matrix describing combined transformation
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
dtmatrix_2dcombine(
  dt_ctl_t *ctl,						/* environment control 				*/
  dtmatrix_2d_t *mati,
  dtmatrix_2d_t *mato,
  dtmatrix_2d_t *matn)
{
  int i;
  double x, y;
  dtmatrix_2d_t matt;
										/*- - - - - - - - - - - - - - - - - */
  matt.coeff[0] =
    mati->coeff[0] * matn->coeff[0] +
    mati->coeff[1] * matn->coeff[3] +
    mati->coeff[2] * matn->coeff[6];
  matt.coeff[1] =
    mati->coeff[0] * matn->coeff[1] +
    mati->coeff[1] * matn->coeff[4] +
    mati->coeff[2] * matn->coeff[7];
  matt.coeff[2] =
    mati->coeff[0] * matn->coeff[2] +
    mati->coeff[1] * matn->coeff[5] +
    mati->coeff[2] * matn->coeff[8];
										/*- - - - - - - - - - - - - - - - - */
  matt.coeff[3] =
    mati->coeff[3] * matn->coeff[0] +
    mati->coeff[4] * matn->coeff[3] +
    mati->coeff[5] * matn->coeff[6];
  matt.coeff[4] =
    mati->coeff[3] * matn->coeff[1] +
    mati->coeff[4] * matn->coeff[4] +
    mati->coeff[5] * matn->coeff[7];
  matt.coeff[5] =
    mati->coeff[3] * matn->coeff[2] +
    mati->coeff[4] * matn->coeff[5] +
    mati->coeff[5] * matn->coeff[8];
										/*- - - - - - - - - - - - - - - - - */
  matt.coeff[6] =
    mati->coeff[6] * matn->coeff[0] +
    mati->coeff[7] * matn->coeff[3] +
    mati->coeff[8] * matn->coeff[6];
  matt.coeff[7] =
    mati->coeff[6] * matn->coeff[1] +
    mati->coeff[7] * matn->coeff[4] +
    mati->coeff[8] * matn->coeff[7];
  matt.coeff[8] =
    mati->coeff[6] * matn->coeff[2] +
    mati->coeff[7] * matn->coeff[5] +
    mati->coeff[8] * matn->coeff[8];

  for (i=0; i<9; i++)					/* copy combined matrix to output	*/
    mato->coeff[i] = matt.coeff[i];

  return DT_RC_GOOD;
}

/*-----------------------------------------------------------------------	
 * FUNCTION NAME
 *   dtmatrix_2dprintf
 * PURPOSE
 *   print matrix in fixed notation
 * ARGUMENTS
 *   dtmatrix_2d_t *mat;		matrix to print
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
dtmatrix_2dprintf(
  dt_ctl_t *ctl,						/* environment control 				*/
  dtmatrix_2d_t *mat,
  FILE *file,
  char *prefix)
{
  fprintf(file, "%s%14.6f %14.6f %14.6f\n",
    prefix, mat->coeff[0], mat->coeff[1], mat->coeff[2]);
  fprintf(file, "%s%14.6f %14.6f %14.6f\n",
    prefix, mat->coeff[3], mat->coeff[4], mat->coeff[5]);
  fprintf(file, "%s%14.6f %14.6f %14.6f\n",
    prefix, mat->coeff[6], mat->coeff[7], mat->coeff[8]);

  return DT_RC_GOOD;
}

/*-----------------------------------------------------------------------	
 * FUNCTION NAME
 *   dtmatrix_2dpoly2
 * PURPOSE
 *   create second order polynomial transformation from matrix transformation
 * ARGUMENTS
 *   dtmatrix_2d_t *mat;		input matrix to change to polynomial
 *   iv_geom2d_spoly_t *spoly;	output second order polynomial
 * RETURNED VALUES
 *   coefficients in output polynomial set up according to input matrix
 * ERROR HANDLING
 *   no possible errors
 * PRINCIPAL EXTERNAL REFERENCES
 *   none
 * OUTLINE OF ALGORITHM
 *   Elements of input matrix are simply copied to correct positions in
 *   second order polynomial coefficient arrays.
 *   After copy, an spoly operation using the output polynomial transform
 *   perform the same transformation as a mat operation using the input
 *   matrix transformation.
 *   Use this function when the source points are known and you want to find
 *   the dest points.
 *   Since matrix transformations are order 1 only by definition, all
 *   coefficients in the output polynomial transformationa above order 1
 *   will be set to 0.
 * CAVEATS
 *-----------------------------------------------------------------------*/

dt_rc_e 
dtmatrix_2dpoly2(
  dt_ctl_t *ctl,						/* environment control 				*/
  dtmatrix_2d_t *mat,
  dtpoly_2_t *spoly)
{
  int i;

  for (i=0; i<5; i++)
    spoly->bh[i] = spoly->bv[i] = 0.0;	/* no coeffients above order 1		*/

  spoly->bh[5] = mat->coeff[0];			/* h part of polynomial transform	*/
  spoly->bh[6] = mat->coeff[3];
  spoly->bh[7] = mat->coeff[6];

  spoly->bv[5] = mat->coeff[1];			/* v part of polynomial transform	*/
  spoly->bv[6] = mat->coeff[4];
  spoly->bv[7] = mat->coeff[7];

  return DT_RC_GOOD;
}

/*-----------------------------------------------------------------------	
 * FUNCTION NAME
 *   dtmatrix_2dspolyi
 * PURPOSE
 *   create second order polynomial transformation from inverse of
 *   matrix transformation
 * ARGUMENTS
 *   dtmatrix_2d_t *mat;		input matrix to change to polynomial
 *   dtpoly_2_t *spoly;			output second order polynomial
 * RETURNED VALUES
 *   coefficients in output polynomial set up according to input matrix
 * ERROR HANDLING
 *   no possible errors
 * PRINCIPAL EXTERNAL REFERENCES
 *   none
 * OUTLINE OF ALGORITHM
 *   Matrix is LU decomposed then the back substituted twice with identity
 *   columns to create second order polynomial transformation.
 *   After substitution, an spoly operation using the output polynomial
 *   transform performs the same transformation as a mat operation using the
 *   inverse of the input matrix transformation.
 *   Use this function when the dest points are known and you want to find
 *   the source points.
 *   Since matrix transformations are order 1 only by definition, all
 *   coefficients in the output polynomial transformationa above order 1
 *   will be set to 0.
 * CAVEATS
 *-----------------------------------------------------------------------*/

dt_rc_e 
dtmatrix_2dspolyi(
  dt_ctl_t *ctl,						/* environment control 				*/
  dtmatrix_2d_t *mat,
  dtpoly_2_t *spoly)
{
  DT_F("dtmatrix_2dspolyi");
  int i, j;
  double aa[3*3];
  double *a[3];
  double d;
  int singular;
  int k[3];
  dt_rc_e rc = DT_RC_GOOD;

  for (i=0; i<8; i++)
    spoly->bh[i] = spoly->bv[i] = 0.0;	/* no coeffients above order 1		*/

  a[0] = &aa[0];
  a[0][0] = mat->coeff[0];				/* matrix into a different form		*/
  a[0][1] = mat->coeff[1];
  a[0][2] = mat->coeff[2];
  a[1] = &aa[3];
  a[1][0] = mat->coeff[3];
  a[1][1] = mat->coeff[4];
  a[1][2] = mat->coeff[5];
  a[2] = &aa[6];
  a[2][0] = mat->coeff[6];
  a[2][1] = mat->coeff[7];
  a[2][2] = mat->coeff[8];

  DT_C(dtmatrix_lu_decompose,(ctl,		/* LU decompose						*/
    (void **)a, 3, DTNTYPE_F2, 
    k, &d, &singular));

  if (singular)
	DT_CLEANUP((ctl, F,
      "matrix is singular"));

  spoly->bh[5] = 1.0;					/* unit vector for solution			*/
  DT_C(dtmatrix_lu_substitute,(ctl, 	/* solve for horizontal				*/
    (void **)a, 3, DTNTYPE_F2, 
    k, &spoly->bh[5]));

  spoly->bv[6] = 1.0;					/* unit vector for solution			*/
  DT_C(dtmatrix_lu_substitute,(ctl, 	/* solve for vertical				*/
    (void **)a, 3, DTNTYPE_F2, 
    k, &spoly->bv[5]));

cleanup:
  return rc;
}

/*-----------------------------------------------------------------------	
 * FUNCTION NAME
 *   dtmatrix_2dinvert
 * PURPOSE
 *   invert 3x3 matrix
 * ARGUMENTS
 *   dtmatrix_2d_t *mati;		input matrix
 *   dtmatrix_2d_t *mato;		output matrix, may be same as mati
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
dtmatrix_2dinvert(
  dt_ctl_t *ctl,						/* environment control 				*/
  dtmatrix_2d_t *mati,
  dtmatrix_2d_t *mato)
{
  DT_F("dtmatrix_2dinvert");
  int i, j;
  double aa[3*3];
  double *a[3];
  double d;
  int singular;
  int k[3];
  double u[3];
  dt_rc_e rc = DT_RC_GOOD;

  a[0] = &aa[0];
  a[0][0] = mati->coeff[0];				/* matrix into a different form		*/
  a[0][1] = mati->coeff[1];
  a[0][2] = mati->coeff[2];
  a[1] = &aa[3];
  a[1][0] = mati->coeff[3];
  a[1][1] = mati->coeff[4];
  a[1][2] = mati->coeff[5];
  a[2] = &aa[6];
  a[2][0] = mati->coeff[6];
  a[2][1] = mati->coeff[7];
  a[2][2] = mati->coeff[8];

  dtmatrix_2dident(ctl, mato);			/* clear output matrix				*/

  DT_C(dtmatrix_lu_decompose,(ctl,		/* LU decompose						*/
    (void **)a, 3, DTNTYPE_F2, 
    k, &d, &singular));

  if (singular)
	DT_CLEANUP((ctl, F,
      "matrix is singular"));

  u[0] = 1.0;							/* unit vector for solution			*/
  u[1] = 0.0;
  u[2] = 0.0;
  DT_C(dtmatrix_lu_substitute,(ctl, 	/* solve for horizontal				*/
    (void **)a, 3, DTNTYPE_F2, k, u));
  mato->coeff[0] = u[0];				/* put answer into output matrix	*/
  mato->coeff[3] = u[1];
  mato->coeff[6] = u[2];

  u[0] = 0.0;							/* unit vector for solution			*/
  u[1] = 1.0;
  u[2] = 0.0;
  DT_C(dtmatrix_lu_substitute,(ctl, 	/* solve for vertical				*/
    (void **)a, 3, DTNTYPE_F2, k, u));
  mato->coeff[1] = u[0];				/* put answer into output matrix	*/
  mato->coeff[4] = u[1];
  mato->coeff[7] = u[2];

cleanup:
  return rc;
}

/*-----------------------------------------------------------------------	
 * FUNCTION NAME
 *   dtmatrix_2dmate
 * PURPOSE
 *   calculate second order polynomial coefficients by mating two node lists
 * ARGUMENTS
 *   dtmatrix_2d_t *mat;	    matrix coefficients describing transform
 *   dtpoint_2d_t *p1;		first input list of nodes
 *   dtpoint_2d_t *p2;		input list of nodes
 * RETURNED VALUES
 *   output mat structure contains coefficients describing transform
 * ERROR HANDLING
 *   no possible errors
 * PRINCIPAL EXTERNAL REFERENCES
 *   iv_mat_alloc
 *   iv_mat_ludec
 *   iv_mat_lusub
 * OUTLINE OF ALGORITHM
 *   Solve two sets of 3 simultaneous equations to get horizontal and
 *   vertical coefficients.
 * CAVEATS
 *   Input arrays p1 and p2 must contain exactly 3 points.
 *   Input array p1 must not contain points which are coincident.
 *   Input array p2 must not contain points which are coincident.
 *-----------------------------------------------------------------------*/

dt_rc_e
dtmatrix_2dmate(
  dt_ctl_t *ctl,						/* environment control 				*/
  dtmatrix_2d_t *mat,
  dtpoint_2d_t *p1,
  dtpoint_2d_t *p2)
{
  DT_F("dtmatrix_2dmate");
  double aa[3*3];
  double *a[3];
  int indx[3];
  double bh[3], bv[3];
  double x, y;
  double d;
  int singular;
  int i;
  dt_rc_e rc = DT_RC_GOOD;

  for (i=0; i<3; i++)					/* load matrix and RHS's			*/
  {
    bh[i] = p1[i].x;					/* RHS for horizontal				*/
    bv[i] = p1[i].y;					/* RHS for vertical					*/
    x = p2[i].x;
    y = p2[i].y;
    a[i] = &aa[3*i];
    a[i][0] = x;
    a[i][1] = y;
    a[i][2] = 1.0;
  }


  DT_C(dtmatrix_lu_decompose,(ctl,		/* LU decompose						*/
    (void **)a, 3, DTNTYPE_F2, 
    indx, &d, &singular));

  if (singular)
	DT_CLEANUP((ctl, F,
      "matrix is singular"));

  DT_C(dtmatrix_lu_substitute,(ctl, 	/* solve for horizontal				*/
    (void **)a, 3, DTNTYPE_F2, 
    indx, bh));
  DT_C(dtmatrix_lu_substitute,(ctl, 	/* solve for vertical				*/
    (void **)a, 3, DTNTYPE_F2, 
    indx, bv));

  mat->coeff[0] = bh[0];				/* put coefficients into matrix		*/
  mat->coeff[1] = bv[0];
  mat->coeff[2] = 0.0;
  mat->coeff[3] = bh[1];
  mat->coeff[4] = bv[1];
  mat->coeff[5] = 0.0;
  mat->coeff[6] = bh[2];
  mat->coeff[7] = bv[2];
  mat->coeff[8] = 1.0;

cleanup:
  return rc;
}

/*-----------------------------------------------------------------------	
 * FUNCTION NAME
 *   dtmatrix_2dfrom_poly2
 * PURPOSE
 *   create a matrix transformation from a second order polynomial transform
 * ARGUMENTS
 *   dtmatrix_2d_t *mat;		input matrix to change to polynomial
 *   dtpoly_2_t *poly;			output second order polynomial
 * RETURNED VALUES
 *   coefficients in output matrix set up according to input spoly
 * ERROR HANDLING
 *   no possible errors
 * PRINCIPAL EXTERNAL REFERENCES
 *   none
 * OUTLINE OF ALGORITHM
 *   Elements of input spoly are simply copied to correct positions in
 *   output matrix.
 *   After copy, a mat operation using the output matrix should
 *   perform the same transformation as an spoly operation using the input
 *   second order polynomial.
 *   Use this function when the source points are known and you want to find
 *   the dest points.
 * CAVEATS
 *-----------------------------------------------------------------------*/

dt_rc_e 
dtmatrix_2dfrom_poly2(
  struct dtmatrix_2d_t *mat,
  dtpoly_2_t *spoly)
{
  int i;

   mat->coeff[0] = spoly->bh[5];		/* h part of polynomial transform	*/
   mat->coeff[3] = spoly->bh[6];
   mat->coeff[6] = spoly->bh[7];

   mat->coeff[1] = spoly->bv[5];		/* v part of polynomial transform	*/
   mat->coeff[4] = spoly->bv[6];
   mat->coeff[7] = spoly->bv[7];
   
   mat->coeff[2] = 0.0;
   mat->coeff[5] = 0.0;
   mat->coeff[8] = 1.0;
   return DT_RC_GOOD;
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

