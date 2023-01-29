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
 * - second order polynomial transformations
 *-------------------------------------------------------------------------*/

static double ident_h[] = {0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0};
static double ident_v[] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0};

/*-----------------------------------------------------------------------	
 * FUNCTION NAME
 *   dtpoly_2printe
 * PURPOSE
 *   print second order polynomial coefficients in floating notation
 * ARGUMENTS
 *   dtpoly_2_t *spoly;	coefficients to print
 *   FILE *file;				file to print to (may be stdout)
 *   char *prefix;				prefix to go on each line
 * RETURNED VALUES
 *   none
 * ERROR HANDLING
 *   no possible errors
 * PRINCIPAL EXTERNAL REFERENCES
 *   none
 * OUTLINE OF ALGORITHM
 *   prints second order polynomial coefficients to file with each line
 *   prefixed by given string with each element printed using %16e format
 * CAVEATS
 *-----------------------------------------------------------------------*/

dt_rc_e 
dtpoly_2printe(
  dt_ctl_t *ctl,						/* environment control				*/
  dtpoly_2_t *spoly,
  FILE *file,
  char *pf)
{
  fprintf(file, "%sxxy %16e %16e\n", pf, spoly->bh[0], spoly->bv[0]);
  fprintf(file, "%sxx  %16e %16e\n", pf, spoly->bh[1], spoly->bv[1]);
  fprintf(file, "%sxyy %16e %16e\n", pf, spoly->bh[2], spoly->bv[2]);
  fprintf(file, "%syy  %16e %16e\n", pf, spoly->bh[3], spoly->bv[3]);
  fprintf(file, "%sxy  %16e %16e\n", pf, spoly->bh[4], spoly->bv[4]);
  fprintf(file, "%sx   %16e %16e\n", pf, spoly->bh[5], spoly->bv[5]);
  fprintf(file, "%sy   %16e %16e\n", pf, spoly->bh[6], spoly->bv[6]);
  fprintf(file, "%s1   %16e %16e\n", pf, spoly->bh[7], spoly->bv[7]);
  return DT_RC_GOOD;
}

/*-----------------------------------------------------------------------	
 * FUNCTION NAME
 *   dtpoly_2printf
 * PURPOSE
 *   print second order polynomial coefficients in fixed notation
 * ARGUMENTS
 *   dtpoly_2_t *spoly;	coefficients to print
 *   FILE *file;				file to print to (may be stdout)
 *   char *prefix;				prefix to go on each line
 * RETURNED VALUES
 *   none
 * ERROR HANDLING
 *   no possible errors
 * PRINCIPAL EXTERNAL REFERENCES
 *   none
 * OUTLINE OF ALGORITHM
 *   prints second order polynomial coefficients to file with each line
 *   prefixed by given string with each element printed using %16.6f format
 * CAVEATS
 *-----------------------------------------------------------------------*/

dt_rc_e 
dtpoly_2printf(
  dt_ctl_t *ctl,						/* environment control				*/
  dtpoly_2_t *spoly,
  FILE *file,
  char *pf)
{
  fprintf(file, "%sxxy %16.6f %16.6f\n", pf, spoly->bh[0], spoly->bv[0]);
  fprintf(file, "%sxx  %16.6f %16.6f\n", pf, spoly->bh[1], spoly->bv[1]);
  fprintf(file, "%sxyy %16.6f %16.6f\n", pf, spoly->bh[2], spoly->bv[2]);
  fprintf(file, "%syy  %16.6f %16.6f\n", pf, spoly->bh[3], spoly->bv[3]);
  fprintf(file, "%sxy  %16.6f %16.6f\n", pf, spoly->bh[4], spoly->bv[4]);
  fprintf(file, "%sx   %16.6f %16.6f\n", pf, spoly->bh[5], spoly->bv[5]);
  fprintf(file, "%sy   %16.6f %16.6f\n", pf, spoly->bh[6], spoly->bv[6]);
  fprintf(file, "%s1   %16.6f %16.6f\n", pf, spoly->bh[7], spoly->bv[7]);
  return DT_RC_GOOD;
}

/*-----------------------------------------------------------------------	
 * FUNCTION NAME
 *   dtpoly_2ident
 * PURPOSE
 *   set second order polynomial coefficients to identity transformation
 * ARGUMENTS
 *   dtpoly_2_t *spoly;	coefficients to set to identity
 * RETURNED VALUES
 *   none
 * ERROR HANDLING
 *   no possible errors
 * PRINCIPAL EXTERNAL REFERENCES
 *   none
 * OUTLINE OF ALGORITHM
 *   Simply sets horizontal and vertical polynomial coefficients all to
 *   0.0 except the x and y terms, respectively, which are set to 1.
 * CAVEATS
 *-----------------------------------------------------------------------*/

dt_rc_e 
dtpoly_2ident(
  dt_ctl_t *ctl,						/* environment control				*/
  dtpoly_2_t *spoly)
{
  int i;
  for (i=0; i<8; i++)
  {
    spoly->bh[i] = ident_h[i];
    spoly->bv[i] = ident_v[i];
  }
  return DT_RC_GOOD;
}

/*-----------------------------------------------------------------------	
 * FUNCTION NAME
 *   dtpoly_2xform
 * PURPOSE
 *   transform list of points under second order polynomial transformation
 * ARGUMENTS
 *   dtpoly_2_t *spoly;	polynomial coefficients describing transform
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
 *   on entire list of points pi, performs matrix operations:
 *     po.x = ps * spolya
 *     po.y = ps * spolyb
 *   where
 *     ps is matrix built from the x and y members of the input points pi:
 *       [xxy xx xyy yy xy x y 1]
 *     spolya holds the horizontal transformation coefficients:
 *       transpose([a0 a1 a2 a3 a4 a5 a6 a7])
 *     spolyb holds the vertical transformation coefficients:
 *       transpose([b0 b1 b2 b3 b4 b5 b6 b7])
 *     po.x are the output points' x coordinates
 *     po.y are the output points' y coordinates
 *   internal calculations are done in double precision and rounded off
 *     to nearest integer before assignment to po
 *   po may be same as pi
 * CAVEATS
 *   Output array po must contain space enough to hold as many elements as
 *     are in input array pi.
 *-----------------------------------------------------------------------*/

dt_rc_e 
dtpoly_2xform(
  dt_ctl_t *ctl,						/* environment control				*/
  dtpoly_2_t *spoly,
  dtpoint_2d_t *pi,
  int n,
  dtpoint_2d_t *po)
{
  int i;
  double dx, dy;
  double dxdy, dx2, dy2;

  for (i=0; i<n; i++)				/* for all points					*/
  {
    dx = (double)pi[i].x;			/* for short						*/
    dy = (double)pi[i].y;
    dxdy = dx * dy;					/* precompute for speed				*/
    dx2 = dx * dx;
    dy2 = dy * dy;
    po[i].x =
        spoly->bh[0] * dxdy * dx +	/* calculate new x coordinate		*/
        spoly->bh[1] * dx2 +
        spoly->bh[2] * dxdy * dy +
        spoly->bh[3] * dy2 +
        spoly->bh[4] * dxdy +
        spoly->bh[5] * dx +
        spoly->bh[6] * dy +
        spoly->bh[7];
    po[i].y =
        spoly->bv[0] * dxdy * dx +		/* calculate new y coordinate		*/
        spoly->bv[1] * dx2 +
        spoly->bv[2] * dxdy * dy +
        spoly->bv[3] * dy2 +
        spoly->bv[4] * dxdy +
        spoly->bv[5] * dx +
        spoly->bv[6] * dy +
        spoly->bv[7];
  }
  return DT_RC_GOOD;
}
#undef PSECT

/*-----------------------------------------------------------------------	
 * FUNCTION NAME
 *   dtpoly_2mate
 * PURPOSE
 *   calculate second order polynomial coefficients by mating two node lists
 * ARGUMENTS
 *   dtpoly_2_t *spoly;	polynomial coefficients describing transform
 *   dtpoint_2d_t *p1;		first input list of nodes
 *   dtpoint_2d_t *p2;		input list of nodes
 * RETURNED VALUES
 *   output spoly structure contains coefficients describing transform
 * ERROR HANDLING
 *   no possible errors
 * PRINCIPAL EXTERNAL REFERENCES
 *   iv_mat_alloc
 *   iv_mat_ludec
 *   iv_mat_lusub
 * OUTLINE OF ALGORITHM
 *   Solve two sets of 8 simultaneous equations to get horizontal and
 *   vertical coefficients.
 * CAVEATS
 *   Input arrays p1 and p2 must contain exactly 8 points.
 *   Input array p1 must not contain points which are coincident.
 *   Input array p2 must not contain points which are coincident.
 *-----------------------------------------------------------------------*/

dt_rc_e 
dtpoly_2mate(
  dt_ctl_t *ctl,						/* environment control				*/
  dtpoly_2_t *spoly,
  dtpoint_2d_t *p1,
  dtpoint_2d_t *p2)
{
  DT_F("dtpoly_2mate");
  double aa[8*8];
  double *a[8];
  dt1d_i_t indices[8];
  double x, y;
  double d;
  int singular;
  int i, j;
  dt_rc_e rc = DT_RC_GOOD;

  for (i=0; i<8; i++)					/* load matrix and RHS's			*/
  {
    spoly->bh[i] = p1[i].x;				/* RHS for horizontal				*/
    spoly->bv[i] = p1[i].y;				/* RHS for vertical					*/
    x = p2[i].x;						/* for short 						*/
    y = p2[i].y;
    a[i] = &aa[i*8];
    a[i][0] = x * x * y;				/* load the matrix row 				*/
    a[i][1] = x * x;
    a[i][2] = x * y * y;
    a[i][3] = y * y;
    a[i][4] = x * y;
    a[i][5] = x;
    a[i][6] = y;
    a[i][7] = 1.0;
  }

  DT_C(dtmatrix_lu_decompose,(ctl,		/* LU decompose the matrix			*/
    (void **)a, 8, DTNTYPE_F2, 
    indices, &d, &singular));

  if (singular)
	DT_CLEANUP((ctl, F,
      "matrix is singular"));

  DT_C(dtmatrix_lu_substitute,(ctl,		/* solve for horizontal				*/
    (void **)a, 8, DTNTYPE_F2, 
    indices, spoly->bh));
  DT_C(dtmatrix_lu_substitute,(ctl,		/* solve for vertical				*/
    (void **)a, 8, DTNTYPE_F2, 
    indices, spoly->bv));

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
