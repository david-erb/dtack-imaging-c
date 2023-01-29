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
DT_RCSID("source/low $RCSfile: matrix.c,v $ $Revision: 1.8 $");
#include <dtack/os.h>
#include <dtack/point.h>				/* xyz points 						*/
#include <dtack/poly.h>					/* polynomial transformation		*/
#include <dtack/matrix.h>				/* matrix operations 				*/

#define TINY 1.0e-20

/*---------------------------------------------------------------------------
 * function: dtmatrix_lu_decompose
 * purpose: matrix LU decomposition
 * - matrix decomposed in place
 * - from Numerical Recipies in C, changed to use matrix base 0
 *--------------------------------------------------------------------------*/

dt_rc_e 
dtmatrix_lu_decompose(
  dt_ctl_t *ctl,						/* environment control 				*/
  void **va,							/* matrix data 						*/
  int n,								/* matrix dimension 				*/
  dtntype_e ntype,						/* matrix data type 				*/
  dt1d_i_t *indices,
  double *d,
  int *singular)
{
  DT_F("dtmatrix_lu_decompose");
  dtntype_f2_t **a = (dtntype_f2_t **)va;
  int i, imax, j, k;
  double big, dum, sum, temp;
  dtntype_f2_t *vv = NULL;
  dtntype_e ntypes[] = {				/* only operate on F2 images 		*/
    DTNTYPE_F2,
    DTNTYPE_NULL
  };
  dt_rc_e rc = DT_RC_GOOD;

  if (ntype != DTNTYPE_F2)
	DT_CLEANUP((ctl, F,
      "ntype %d not supported",
      ntype));

  DT_C(dtos_malloc2,(ctl,
    (void **)&vv, n * sizeof(*vv),
    F, "vv"));
										/* ................................ */
   *d = 1.0;
   *singular = 0;
   for( i=1; i<=n; i++ )
   {
       big = 0.0;
       for( j=1; j<=n; j++ )
           if( (temp = fabs( a[i-1][j-1] )) > big )
			 big = temp;
       if( big == 0.0 )
       {
         *singular = 1;
         goto cleanup;
       }
       vv[i-1] = 1.0 / big;
   }
   for( j=1; j<=n; j++ )
   {
       for( i=1; i<j; i++ )
       {
           sum = a[i-1][j-1];
           for( k=1; k<i; k++ )
			 sum -= a[i-1][k-1] * a[k-1][j-1];
           a[i-1][j-1] = sum;
       }
       big = 0.0;
       for( i=j; i<=n; i++ )
       {
           sum = a[i-1][j-1];
           for( k=1; k<j; k++ )
			 sum -= a[i-1][k-1] * a[k-1][j-1];
           a[i-1][j-1] = sum;
           if(  (dum = vv[i-1] * fabs( sum ))  >=  big  )
           {
               big = dum;
               imax = i;
           }
       }
       if( j != imax )
       {
           for( k=1; k<i; k++ )
           {
               dum = a[imax-1][k-1];
               a[imax-1][k-1] = a[j-1][k-1];
               a[j-1][k-1] = dum;
           }
           *d = -(*d);
           vv[imax-1] = vv[j-1];
       }
       indices[j-1] = imax;
       if( a[j-1][j-1] == 0.0 )
		 a[j-1][j-1] = TINY;
       if( j != n )
       {
           dum = 1.0/( a[j-1][j-1] );
           for( i=j+1; i<=n; i++ )
			 a[i-1][j-1] *= dum;
       }
   }
										/* ................................ */
cleanup:
  if (vv)
	DT_I(dtos_free2,(ctl, vv, F, "vv"));
  return rc;
}

/*---------------------------------------------------------------------------
 * function: dtmatrix_lu_substitute
 * purpose: matrix LU decomposition backsubstitution given RHS
 * - RHS replaced by solution
 * - from Numerical Recipies in C, changed to use matrix base 0
 *--------------------------------------------------------------------------*/

dt_rc_e 
dtmatrix_lu_substitute(
  dt_ctl_t *ctl,						/* environment control 				*/
  void **va,							/* matrix data 						*/
  int n,								/* matrix dimension 				*/
  dtntype_e ntype,						/* matrix data type 				*/
  dt1d_i_t *indices,
  double *b)
{
  DT_F("dtmatrix_lu_substitute");
  dtntype_f2_t **a = (dtntype_f2_t **)va;
  int i, ii=0, ip, j;
  double sum;
  dt_rc_e rc = DT_RC_GOOD;

  if (ntype != DTNTYPE_F2)
	DT_CLEANUP((ctl, F,
      "ntype %d not supported",
      ntype));
   
   for( i=1; i<=n; i++ )
   {
       ip = indices[i-1];
       sum = b[ip-1];
       b[ip-1] = b[i-1];
       if( ii )
           for( j=ii; j<=i-1; j++ ) sum -= a[i-1][j-1] * b[j-1];
       else if( sum ) ii = i;
       b[i-1] = sum;
   }
   for( i=n; i>=1; i-- )
   {
       sum = b[i-1];
       for( j=i+1; j<=n; j++ ) sum -= a[i-1][j-1] * b[j-1];
       b[i-1] = sum/a[i-1][i-1];
   }
cleanup:
  return rc;
}
#undef PSECT


/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
  ! Copyright (C) 1998 Dtack Inc. All Rights Reserved
  ! To use this file, you must have signed a license agreement with Dtack Inc.
  ! Under no circumstances may you redistribute this file.
  ! This software is provided AS IS with no warranty expressed or implied.
  ! Dtack Inc. accepts no liability for use or misuse of this file.
  ! http://www.dtack.com  dtack@dtack.com  telephone +360.417.6700
  ! Dtack Inc, P.O. Box 3061, Port Angeles, WA USA 98362
  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */

