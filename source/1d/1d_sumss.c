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
 * scalar sum of squared differences
 *..........................................................................*/

#define SUMSQ_DIFFS \
  const double cscalar = scalar; \
  dt1d_i_t i; \
  double s = 0; \
  for (i=0; i<nv; i++) \
  { \
    const double diff = (double)v[i] - cscalar; \
    s += diff * diff; \
  } \
  *sum = s; \
  return DT1D_RC_GOOD;

#define SUMSQ_DIFFS_SINT32 \
  dt1d_i_t i; \
  long s = 0; \
  for (i=0; i<nv; i++) \
  { \
    const long diff = (long)v[i] - lscalar; \
    s += diff * diff; \
  } \
  *sum = s; \
  return DT1D_RC_GOOD;

/*..........................................................................
 * uchar scalar sum of squared differences
 *..........................................................................*/

dt1d_rc_e
DT1D_API
dt1d_sumsq_scalar_uchar(
  const unsigned char * const v,
  const dt1d_i_t nv,
  double scalar,
  double *sum)
{
  const long lscalar = (long)scalar;
  if (lscalar == scalar &&				/* don't need floating point? 		*/
	  lscalar < 0x01000000L)
  {
    SUMSQ_DIFFS_SINT32
  }
  else
  {
    SUMSQ_DIFFS
  }
}

/*..........................................................................
 * ushort scalar sum of squared differences
 *..........................................................................*/

dt1d_rc_e
DT1D_API
dt1d_sumsq_scalar_ushort(
  const unsigned short * const v,
  const dt1d_i_t nv,
  double scalar,
  double *sum)
{
  SUMSQ_DIFFS
}

/*..........................................................................
 * uint32 scalar sum of squared differences
 *..........................................................................*/

dt1d_rc_e
DT1D_API
dt1d_sumsq_scalar_uint32(
  const dtntype_b32_t * const v,
  const dt1d_i_t nv,
  double scalar,
  double *sum)
{
  SUMSQ_DIFFS
}

/*..........................................................................
 * char scalar sum of squared differences
 *..........................................................................*/

dt1d_rc_e
DT1D_API
dt1d_sumsq_scalar_char(
  const signed char * const v,
  const dt1d_i_t nv,
  double scalar,
  double *sum)
{
  SUMSQ_DIFFS
}

/*..........................................................................
 * short scalar sum of squared differences
 *..........................................................................*/

dt1d_rc_e
DT1D_API
dt1d_sumsq_scalar_short(
  const signed short * const v,
  const dt1d_i_t nv,
  double scalar,
  double *sum)
{
  SUMSQ_DIFFS
}

/*..........................................................................
 * long scalar sum of squared differences
 *..........................................................................*/

dt1d_rc_e
DT1D_API
dt1d_sumsq_scalar_sint32(
  const dtntype_s4_t * const v,
  const dt1d_i_t nv,
  double scalar,
  double *sum)
{
  const long lscalar = (long)scalar;
  
  SUMSQ_DIFFS
}

/*..........................................................................
 * float scalar sum of squared differences
 *..........................................................................*/

dt1d_rc_e
DT1D_API
dt1d_sumsq_scalar_float(
  const float * const v,
  const dt1d_i_t nv,
  double scalar,
  double *sum)
{
  SUMSQ_DIFFS
}

/*..........................................................................
 * double scalar sum of squared differences
 *..........................................................................*/

dt1d_rc_e
DT1D_API
dt1d_sumsq_scalar_double(
  const double * const v,
  const dt1d_i_t nv,
  double scalar,
  double *sum)
{
  SUMSQ_DIFFS
}

/*..........................................................................
 * any scalar sum of squared diffs
 *..........................................................................*/

dt1d_rc_e
DT1D_API
dt1d_sumsq_scalar(
  dt1d_ntype_e ntype,
  const void * const v,
  const dt1d_i_t nv,
  double scalar,
  double *sum)
{
# define CASE(NTYPE, FUNC, TYPE) \
  case NTYPE: rc = FUNC((TYPE *)v, nv, scalar, sum); break;
  dt1d_rc_e rc;

  switch(ntype)
  {
    CASE(DT1D_NTYPE_B08, dt1d_sumsq_scalar_uchar,  unsigned char);
    CASE(DT1D_NTYPE_B16, dt1d_sumsq_scalar_ushort, unsigned short);
    CASE(DT1D_NTYPE_B32, dt1d_sumsq_scalar_uint32,  dtntype_b32_t);
    CASE(DT1D_NTYPE_S1, dt1d_sumsq_scalar_char,    signed char);
    CASE(DT1D_NTYPE_S2, dt1d_sumsq_scalar_short,   short);
    CASE(DT1D_NTYPE_S4, dt1d_sumsq_scalar_sint32,    dtntype_s4_t);
    CASE(DT1D_NTYPE_F1, dt1d_sumsq_scalar_float,   float);
    CASE(DT1D_NTYPE_F2, dt1d_sumsq_scalar_double,  double);
    default: rc = dt1d_err_ntype(ntype, dt1d_ntype_list);
  }
  return rc;
# undef CASE
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
