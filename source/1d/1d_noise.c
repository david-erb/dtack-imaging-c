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
 * double vector uniform random number generation
 *..........................................................................*/

dt1d_rc_e
DT1D_API
dt1d_random_uniform_double(
  dt1d_ran_t *ran,
  double *v,
  dt1d_i_t nv)
{
  dt1d_i_t i;
  dt1d_rc_e rc = DT1D_RC_GOOD;
  for (i=0; i<nv && rc == DT1D_RC_GOOD; i++)
    rc = dt1d_ran_uniform(ran, &v[i]);
  return rc;
}

/*..........................................................................
 * double vector gaussian random number generation
 *..........................................................................*/

dt1d_rc_e
DT1D_API
dt1d_random_gauss_double(
  dt1d_ran_t *ran,
  double *v,
  dt1d_i_t nv)
{
  dt1d_i_t i;
  dt1d_rc_e rc = DT1D_RC_GOOD;
  for (i=0; i<nv && rc == DT1D_RC_GOOD; i++)
    rc = dt1d_ran_gauss(ran, &v[i]);
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
