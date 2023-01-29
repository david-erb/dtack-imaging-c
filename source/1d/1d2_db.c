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
| NAME
| Introduction - intro
| 
| DESCRIPTION
| As yet undocumented.
| 
| RETURN VALUES
| Function return value will be DT1D_RC_GOOD on success.  
| Any other return value indicates failure or noncompletion.
| When DT1D_RC_GOOD is not returned, any other return values may not be valid.
| 
| ERRORS
| As yet undocumented.
| 
| END
 *..........................................................................*/

#include <dtack/config.h>				/* platform-specific defines		*/
#include <dtack/ntype.h>
#include <dtack/stats.h>
#include <dtack/1d.h>

/*..........................................................................
 * double vector moments computation
 * based on Numerical Recipes Chapter 13
 *..........................................................................*/

dt1d_rc_e
DT1D_API
dt1d_moments_double(
  const double * const x,
  const dt1d_i_t n,
  double *mean,
  double *avedev,
  double *variance,
  double *skewness,
  double *kurtosis)
{
  double s = 0.0;
  double a = 0.0;
  double ss = 0.0;
  double sss = 0.0;
  double ssss = 0.0;
  int i;

  if (n < 2)
    return dt1d_err_length(n);

  for (i=0; i<n; i++)
    s += x[i];
  s /= (double)n;
										/* mean							    */
  for (i=0; i<n; i++)
  {
    const double d = s - x[i];			/* difference from mean			    */
    const double d2 = d * d;
    if (d < 0.0) 
      a -= d;
    else
      a += d;
    ss += d2;
    sss += d2 * d;
    ssss += d2 * d2;
  }

  *mean = s;
  *avedev = a / (double)n;
  ss /= (double)(n-1);
  if (ss != 0.0)
  {
    *variance = ss;
    *skewness = sss / (double)n / (ss * sqrt(ss));
    *kurtosis = ssss / (double)n / (ss * ss) - 3.0;
  }
  else
  {
    *variance = 0.0;
    *skewness = 0.0;
    *kurtosis = 0.0;
  }
  return DT1D_RC_GOOD;
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
