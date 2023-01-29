/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
  ! Copyright (C) 1998 Dtack Inc. All Rights Reserved
  ! To use this file, you must have signed a license agreement with Dtack Inc.
  ! Under no circumstances may you redistribute this file.
  ! This software is provided AS IS with no warranty expressed or implied.
  ! Dtack Inc. accepts no liability for use or misuse of this file.
  ! http://www.dtack.com  dtack@dtack.com  telephone +360.417.6700
  ! Dtack Inc, P.O. Box 3061, Port Angeles, WA USA 98362
  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */




#include <dtack/config.h>				/* platform-specific defines		*/
#include <dtack/ntype.h>
#include <dtack/stats.h>
#include <dtack/1d.h>

#define BODIES DT1D_BIG_BODIES

/*..........................................................................
 *..........................................................................*/

#define DT1D_BODY(NTYPE, DTYPE, NAME) \
dt1d_rc_e                                                                     \
DT1D_API                                                                      \
dt1d_cumf_ ## NAME(												              \
  const DTYPE * vin,														  \
  dt1d_i_t nin,																  \
  DTYPE * vout,																  \
  dt1d_i_t nout)															  \
{																			  \
  const dt1d_i_t n = DT1D_MIN(nin, nout);									  \
  dt1d_i_t i;																  \
  DTYPE s; 																	  \
  if (n <= 0) return DT1D_RC_GOOD; 											  \
  s = vout[0] = vin[0]; 													  \
  for (i=1; i<n; i++) 														  \
    vout[i] = (s += vin[i]); 												  \
  return DT1D_RC_GOOD;														  \
}

BODIES

#undef DT1D_BODY

/*..........................................................................
 *..........................................................................*/

#define DT1D_BODY(NTYPE, DTYPE, NAME) \
  case NTYPE: rc = dt1d_cumf_ ## NAME( \
    (DTYPE *)vin, nin, \
    (DTYPE *)vout, nout); \
  break;

/*..........................................................................
| NAME
| dt1d_cumf() - vector forward cumulative sum
| 
| SUMMARY
| Implement output[i] = SUM(input[0:i]) for each element.
| Elements in the longer vector are ignored.
| Assume all vectors are of type ~ntype~.
| Have an error if ~ntype~ is not one of the allowed "big" data types.
| Permit space sharing among all the vectors
| 
| END
 *..........................................................................*/

dt1d_rc_e
DT1D_API
dt1d_cumf(
  dt1d_ntype_e ntype,
  const void * vin,	
  dt1d_i_t nin,		
  void * vout,		
  dt1d_i_t nout)															  \
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
