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
 * abs in place
 *..........................................................................*/

#define BODY(DTYPE) \
  dt1d_i_t i; \
  for (i=0; i<nv; i++) \
    if (v[i] < 0) \
      v[i] = (DTYPE)(-v[i]); \
  return DT1D_RC_GOOD;

/*..........................................................................
 * abs in place definitions
 *..........................................................................*/

#define DT1D_BODY(NTYPE, DTYPE, NAME) \
extern  \
dt1d_rc_e \
DT1D_API \
dt1d_abs_ ## NAME( \
  DTYPE * v, \
  dt1d_i_t nv) \
{ \
  BODY(DTYPE) \
}
DT1D_SIGNED_BODIES						/* abs definitions					*/
#undef DT1D_BODY

/*..........................................................................
 * abs in place dispatch
 *..........................................................................*/

dt1d_rc_e
DT1D_API
dt1d_abs(
  dt1d_ntype_e ntype,
  const void * const v,
  const dt1d_i_t nv)
{
  dt1d_rc_e rc;

  switch(ntype)
  {
#   define DT1D_BODY(NTYPE, DTYPE, NAME) \
      case NTYPE: rc = dt1d_abs_ ## NAME((DTYPE *)v, nv); break;
    DT1D_SIGNED_BODIES
#   undef DT1D_BODY

#   define DT1D_BODY(NTYPE, DTYPE, NAME) \
      case NTYPE: rc = DT1D_RC_GOOD; break;
    DT1D_UNSIGNED_BODIES
#   undef DT1D_BODY

    default: rc = dt1d_err_ntype(ntype, dt1d_ntype_list);
  }
  return rc;
  
}

#undef BODY





/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
  ! Copyright (C) 1998 Dtack Inc. All Rights Reserved
  ! To use this file, you must have signed a license agreement with Dtack Inc.
  ! Under no circumstances may you redistribute this file.
  ! This software is provided AS IS with no warranty expressed or implied.
  ! Dtack Inc. accepts no liability for use or misuse of this file.
  ! http://www.dtack.com  dtack@dtack.com  telephone +360.417.6700
  ! Dtack Inc, P.O. Box 3061, Port Angeles, WA USA 98362
  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */
