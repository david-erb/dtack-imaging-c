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

#define BODIES DT1D_INTEGER_BODIES

/*..........................................................................
 *..........................................................................*/

#define DT1D_BODY(NTYPE, DTYPE, NAME) \
static																		  \
dt1d_rc_e                                                                     \
and_ ## NAME(												         	      \
  const DTYPE * in1,														  \
  dt1d_i_t nin1,															  \
  const DTYPE a,															  \
  const DTYPE * in2,														  \
  dt1d_i_t nin2,															  \
  const DTYPE b,															  \
  const DTYPE c,															  \
  DTYPE * out,																  \
  dt1d_i_t nout)															  \
{																			  \
  const dt1d_i_t n = DT1D_MIN(nout,											  \
    DT1D_MIN(nin1, nin2));													  \
  dt1d_i_t i;																  \
  for (i=0; i<n; i++)														  \
    out[i] = (DTYPE)(														  \
      (in1[i] & a) &														  \
      (in2[i] & b) & c);													  \
  return DT1D_RC_GOOD;														  \
}

BODIES

#undef DT1D_BODY

/*..........................................................................
 *..........................................................................*/

#define DT1D_BODY(NTYPE, DTYPE, NAME) \
  case NTYPE: rc = and_ ## NAME( \
    (DTYPE *)in1, nin1, a, \
    (DTYPE *)in2, nin2, b, c, \
    (DTYPE *)out, nout); \
  break;

/*..........................................................................
 *..........................................................................*/

dt1d_rc_e
DT1D_API
dt1d_and(
  dt1d_ntype_e ntype,
  const void * in1,	
  dt1d_i_t nin1,	
  const dtntype_b32_t a,	
  const void * in2,	
  dt1d_i_t nin2,	
  const dtntype_b32_t b,	
  const dtntype_b32_t c,	
  void * out,		
  dt1d_i_t nout)	
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

/*..........................................................................
 *..........................................................................*/

#define DT1D_BODY(NTYPE, DTYPE, NAME) 										  \
static																		  \
dt1d_rc_e                                                                     \
or_ ## NAME(													              \
  const DTYPE * in1,														  \
  dt1d_i_t nin1,															  \
  const DTYPE a,															  \
  const DTYPE * in2,														  \
  dt1d_i_t nin2,															  \
  const DTYPE b,															  \
  const DTYPE c,															  \
  DTYPE * out,																  \
  dt1d_i_t nout)															  \
{																			  \
  const dt1d_i_t n = DT1D_MIN(nout,											  \
    DT1D_MIN(nin1, nin2));													  \
  dt1d_i_t i;																  \
  for (i=0; i<n; i++)														  \
    out[i] = (DTYPE)(														  \
      (in1[i] & a) |														  \
      (in2[i] & b) | c);													  \
  return DT1D_RC_GOOD;														  \
}

BODIES

#undef DT1D_BODY

/*..........................................................................
 *..........................................................................*/

#define DT1D_BODY(NTYPE, DTYPE, NAME) \
  case NTYPE: rc = or_ ## NAME( \
    (DTYPE *)in1, nin1, a, \
    (DTYPE *)in2, nin2, b, c, \
    (DTYPE *)out, nout); \
  break;

/*..........................................................................
| NAME
| dt1d_or() - vector logical or
| 
| SUMMARY
| Implement output = (input1 AND a) OR (input2 AND b) OR c for each element.
| Assume all vectors are of type ~ntype~.
| Convert the constants a, b, and c to ~ntype~ before operation.
| Have an error if ~ntype~ is not one of the integer data types.
| Ignore elements beyond the length of the shortest vector.
| Permit space sharing among all the vectors
| 
| END
 *..........................................................................*/

dt1d_rc_e
DT1D_API
dt1d_or(
  dt1d_ntype_e ntype,
  const void * in1,	
  dt1d_i_t nin1,	
  const dtntype_b32_t a,	
  const void * in2,	
  dt1d_i_t nin2,	
  const dtntype_b32_t b,	
  const dtntype_b32_t c,	
  void * out,		
  dt1d_i_t nout)	
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

/*..........................................................................
 *..........................................................................*/

#define DT1D_BODY(NTYPE, DTYPE, NAME) \
static																		  \
dt1d_rc_e                                                                     \
xor_ ## NAME(													              \
  const DTYPE * in1,														  \
  dt1d_i_t nin1,															  \
  const DTYPE a,															  \
  const DTYPE * in2,														  \
  dt1d_i_t nin2,															  \
  const DTYPE b,															  \
  const DTYPE c,															  \
  DTYPE * out,																  \
  dt1d_i_t nout)															  \
{																			  \
  const dt1d_i_t n = DT1D_MIN(nout,											  \
    DT1D_MIN(nin1, nin2));													  \
  dt1d_i_t i;																  \
  for (i=0; i<n; i++)														  \
    out[i] = (DTYPE)(														  \
      (in1[i] & a) ^														  \
      (in2[i] & b) ^ c);													  \
  return DT1D_RC_GOOD;														  \
}

BODIES

#undef DT1D_BODY

/*..........................................................................
 *..........................................................................*/

#define DT1D_BODY(NTYPE, DTYPE, NAME) \
  case NTYPE: rc = xor_ ## NAME( \
    (DTYPE *)in1, nin1, a, \
    (DTYPE *)in2, nin2, b, c, \
    (DTYPE *)out, nout); \
  break;

/*..........................................................................
 *..........................................................................*/

dt1d_rc_e
DT1D_API
dt1d_xor(
  dt1d_ntype_e ntype,
  const void * in1,	
  dt1d_i_t nin1,	
  const dtntype_b32_t a,	
  const void * in2,	
  dt1d_i_t nin2,	
  const dtntype_b32_t b,	
  const dtntype_b32_t c,	
  void * out,		
  dt1d_i_t nout)	
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
