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

#define BODIES DT1D_ALL_BODIES

/*..........................................................................
 * vector sum computation
 *..........................................................................*/

#define DT1D_BODY(NTYPE, DTYPE, NAME) 										  \
static																		  \
dt1d_rc_e                                                                     \
f_ ## NAME(													                  \
  const DTYPE * const x,													  \
  const dt1d_i_t n,															  \
  double *sum)																  \
{																			  \
  double s = 0.0;															  \
  dt1d_i_t i;																  \
  for (i=0; i<n; i++)														  \
    s += x[i];																  \
  *sum = s;																	  \
  return DT1D_RC_GOOD;														  \
}

BODIES

#undef DT1D_BODY

/*..........................................................................
 *..........................................................................*/

#define DT1D_BODY(NTYPE, DTYPE, NAME) \
  case NTYPE: rc = f_ ## NAME( \
    (DTYPE *)in, nin, sum); \
  break;

/*..........................................................................
 * vector sum computation
 *..........................................................................*/

dt1d_rc_e
DT1D_API
dt1d_sum(								/* vector sum					*/
  dt1d_ntype_e ntype,					/* vector element type 				*/
  const void * in,						/* input vector 					*/
  dt1d_i_t nin,							/* length of input vector 			*/
  double *sum)							/* returned sum				*/
{
  dt1d_rc_e rc;

  switch(ntype)
  {
    BODIES
    default: rc = dt1d_err_ntype(ntype, dt1d_ntype_list);
  }
  return rc;
}
#undef DT1D_BODY




/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
  ! Copyright (C) 1998 Dtack Inc. All Rights Reserved
  ! To use this file, you must have signed a license agreement with Dtack Inc.
  ! Under no circumstances may you redistribute this file.
  ! This software is provided AS IS with no warranty expressed or implied.
  ! Dtack Inc. accepts no liability for use or misuse of this file.
  ! http://www.dtack.com  dtack@dtack.com  telephone +360.417.6700
  ! Dtack Inc, P.O. Box 3061, Port Angeles, WA USA 98362
  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */
