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

#define SUMSQ_DIFFS(ACCUMULATOR_TYPE, DIFF_TYPE, SQUARE) \
  ACCUMULATOR_TYPE s; \
  const dt1d_i_t n = DT1D_MIN(nv1, nv2); \
  dt1d_i_t i; \
  s = 0; \
  for (i=0; i<n; i++) \
  { \
    const DIFF_TYPE diff = (DIFF_TYPE)v1[i] - (DIFF_TYPE)v2[i]; \
    s += SQUARE; \
  } \
  *sum = s; \
  return DT1D_RC_GOOD;

/*..........................................................................
 * uchar scalar sum of squared differences
 *..........................................................................*/

dt1d_rc_e
DT1D_API
dt1d_sumsq_diffs_uchar(
  const unsigned char * const v1,
  const dt1d_i_t nv1,
  const unsigned char * const v2,
  const dt1d_i_t nv2,
  double *sum)
{
  SUMSQ_DIFFS(unsigned long, int, dt1d_square_table[diff]);
}

/*..........................................................................
 * ushort scalar sum of squared differences
 *..........................................................................*/

dt1d_rc_e
DT1D_API
dt1d_sumsq_diffs_ushort(
  const unsigned short * const v1,
  const dt1d_i_t nv1,
  const unsigned short * const v2,
  const dt1d_i_t nv2,
  double *sum)
{
  SUMSQ_DIFFS(unsigned long, long, diff*diff);
}

/*..........................................................................
 * uint32 scalar sum of squared differences
 *..........................................................................*/

dt1d_rc_e
DT1D_API
dt1d_sumsq_diffs_uint32(
  const dtntype_b32_t * const v1,
  const dt1d_i_t nv1,
  const dtntype_b32_t * const v2,
  const dt1d_i_t nv2,
  double *sum)
{
  SUMSQ_DIFFS(unsigned long, long, diff*diff);
}

/*..........................................................................
 * char scalar sum of squared differences
 *..........................................................................*/

dt1d_rc_e
DT1D_API
dt1d_sumsq_diffs_char(
  const signed char * const v1,
  const dt1d_i_t nv1,
  const signed char * const v2,
  const dt1d_i_t nv2,
  double *sum)
{
  SUMSQ_DIFFS(long, int, dt1d_square_table[diff]);
}

/*..........................................................................
 * short scalar sum of squared differences
 *..........................................................................*/

dt1d_rc_e
DT1D_API
dt1d_sumsq_diffs_short(
  const signed short * const v1,
  const dt1d_i_t nv1,
  const signed short * const v2,
  const dt1d_i_t nv2,
  double *sum)
{
  SUMSQ_DIFFS(long, long, diff*diff);
}

/*..........................................................................
 * long scalar sum of squared differences
 *..........................................................................*/

dt1d_rc_e
DT1D_API
dt1d_sumsq_diffs_sint32(
  const dtntype_s4_t * const v1,
  const dt1d_i_t nv1,
  const dtntype_s4_t * const v2,
  const dt1d_i_t nv2,
  double *sum)
{
  SUMSQ_DIFFS(long, long, diff*diff);
}

/*..........................................................................
 * float scalar sum of squared differences
 *..........................................................................*/

dt1d_rc_e
DT1D_API
dt1d_sumsq_diffs_float(
  const float * const v1,
  const dt1d_i_t nv1,
 const  float * const v2,
  const dt1d_i_t nv2,
  double *sum)
{
  SUMSQ_DIFFS(double, double, diff*diff);
}

/*..........................................................................
 * double scalar sum of squared differences
 *..........................................................................*/

dt1d_rc_e
DT1D_API
dt1d_sumsq_diffs_double(
  const double * const v1,
  const dt1d_i_t nv1,
  const double * const v2,
  const dt1d_i_t nv2,
  double *sum)
{
  SUMSQ_DIFFS(double, double, diff*diff);
}

/*..........................................................................
 * any scalar sum of squared diffs
 *..........................................................................*/

dt1d_rc_e
DT1D_API
dt1d_sumsq_diffs(
  dt1d_ntype_e ntype,
  const void * const v1,
  const dt1d_i_t nv1,
  const void * const v2,
  const dt1d_i_t nv2,
  double *sum)
{
# define CASE(NTYPE, FUNC, TYPE) \
  case NTYPE: rc = FUNC((TYPE *)v1, nv1, (TYPE *)v2, nv2, sum); break;
  dt1d_rc_e rc;

  switch(ntype)
  {
    CASE(DT1D_NTYPE_B08, dt1d_sumsq_diffs_uchar,  unsigned char);
    CASE(DT1D_NTYPE_B16, dt1d_sumsq_diffs_ushort, unsigned short);
    CASE(DT1D_NTYPE_B32, dt1d_sumsq_diffs_uint32,  dtntype_b32_t);
    CASE(DT1D_NTYPE_S1, dt1d_sumsq_diffs_char,    signed char);
    CASE(DT1D_NTYPE_S2, dt1d_sumsq_diffs_short,   short);
    CASE(DT1D_NTYPE_S4, dt1d_sumsq_diffs_sint32,    dtntype_s4_t);
    CASE(DT1D_NTYPE_F1, dt1d_sumsq_diffs_float,   float);
    CASE(DT1D_NTYPE_F2, dt1d_sumsq_diffs_double,  double);
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
