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
 * scalar sum of squares
 *..........................................................................*/

#define SUMSQ(ACCUMULATOR_TYPE, SQUARE) \
  ACCUMULATOR_TYPE s; \
  dt1d_i_t i; \
  s = 0; \
  for (i=0; i<nin; i++) \
    s += SQUARE; \
  *sum = s; \
  return DT1D_RC_GOOD;

/*..........................................................................
 * uchar scalar sum of squares
 *..........................................................................*/

dt1d_rc_e
DT1D_API
dt1d_sumsq_uchar(
  const unsigned char * const in,
  const dt1d_i_t nin,
  double *sum)
{
  SUMSQ(unsigned long, dt1d_square_table[in[i]]);
}

/*..........................................................................
 * ushort scalar sum of squares
 *..........................................................................*/

dt1d_rc_e
DT1D_API
dt1d_sumsq_ushort(
  const unsigned short * const in,
  const dt1d_i_t nin,
  double *sum)
{
  SUMSQ(unsigned long, (dtntype_b32_t)in[i] * (dtntype_b32_t)in[i]);
}

/*..........................................................................
 * uint32 scalar sum of squares
 *..........................................................................*/

dt1d_rc_e
DT1D_API
dt1d_sumsq_uint32(
  const dtntype_b32_t * const in,
  const dt1d_i_t nin,
  double *sum)
{
  SUMSQ(unsigned long, in[i]*in[i]);
}

/*..........................................................................
 * char scalar sum of squares
 *..........................................................................*/

dt1d_rc_e
DT1D_API
dt1d_sumsq_char(
  const signed char * const in,
  const dt1d_i_t nin,
  double *sum)
{
  SUMSQ(long, dt1d_square_table[in[i]]);
}

/*..........................................................................
 * short scalar sum of squares
 *..........................................................................*/

dt1d_rc_e
DT1D_API
dt1d_sumsq_short(
  const signed short * const in,
  const dt1d_i_t nin,
  double *sum)
{
  SUMSQ(long, (dtntype_s4_t)in[i] * (dtntype_s4_t)in[i]);
}

/*..........................................................................
 * long scalar sum of squares
 *..........................................................................*/

dt1d_rc_e
DT1D_API
dt1d_sumsq_sint32(
  const dtntype_s4_t * const in,
  const dt1d_i_t nin,
  double *sum)
{
  SUMSQ(long, in[i]*in[i]);
}

/*..........................................................................
 * float scalar sum of squares
 *..........................................................................*/

dt1d_rc_e
DT1D_API
dt1d_sumsq_float(
  const float * const in,
  const dt1d_i_t nin,
  double *sum)
{
  SUMSQ(double, in[i]*in[i]);
}

/*..........................................................................
 * double scalar sum of squares
 *..........................................................................*/

dt1d_rc_e
DT1D_API
dt1d_sumsq_double(
  const double * const in,
  const dt1d_i_t nin,
  double *sum)
{
  SUMSQ(double, in[i]*in[i]);
}

/*..........................................................................
 * any scalar sum of squares
 *..........................................................................*/

dt1d_rc_e
DT1D_API
dt1d_sumsq(
  dt1d_ntype_e ntype,
  const void * const in,
  const dt1d_i_t nin,
  double *sum)
{
# define CASE(NTYPE, FUNC, TYPE) \
  case NTYPE: rc = FUNC((TYPE *)in, nin, sum); break;
  dt1d_rc_e rc;

  switch(ntype)
  {
    CASE(DT1D_NTYPE_B08, dt1d_sumsq_uchar,  unsigned char);
    CASE(DT1D_NTYPE_B16, dt1d_sumsq_ushort, unsigned short);
    CASE(DT1D_NTYPE_B32, dt1d_sumsq_uint32,  dtntype_b32_t);
    CASE(DT1D_NTYPE_S1, dt1d_sumsq_char,    signed char);
    CASE(DT1D_NTYPE_S2, dt1d_sumsq_short,   short);
    CASE(DT1D_NTYPE_S4, dt1d_sumsq_sint32,    dtntype_s4_t);
    CASE(DT1D_NTYPE_F1, dt1d_sumsq_float,   float);
    CASE(DT1D_NTYPE_F2, dt1d_sumsq_double,  double);
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
