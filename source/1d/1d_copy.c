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
 * scalar set
 *..........................................................................*/

#define SET(TYPE) \
  const TYPE v = (TYPE)value; \
  dt1d_i_t i; \
  for (i=i1; i<i2; i++) \
    in[i] = v; \
  return DT1D_RC_GOOD;

/*..........................................................................
 * uchar scalar set
 *..........................................................................*/

dt1d_rc_e
DT1D_API
dt1d_set_uchar(
  unsigned char * const in,
  const dt1d_i_t i1,
  const dt1d_i_t i2,
  double value)
{
  SET(unsigned char);
}

/*..........................................................................
 * ushort scalar set
 *..........................................................................*/

dt1d_rc_e
DT1D_API
dt1d_set_ushort(
  unsigned short * const in,
  const dt1d_i_t i1,
  const dt1d_i_t i2,
  double value)
{
  SET(unsigned short);
}

/*..........................................................................
 * uint32 scalar set
 *..........................................................................*/

dt1d_rc_e
DT1D_API
dt1d_set_uint32(
  dtntype_b32_t * const in,
  const dt1d_i_t i1,
  const dt1d_i_t i2,
  double value)
{
  SET(dtntype_b32_t);
}

/*..........................................................................
 * char scalar set
 *..........................................................................*/

dt1d_rc_e
DT1D_API
dt1d_set_char(
  signed char * const in,
  const dt1d_i_t i1,
  const dt1d_i_t i2,
  double value)
{
  SET(signed char);
}

/*..........................................................................
 * short scalar set
 *..........................................................................*/

dt1d_rc_e
DT1D_API
dt1d_set_short(
  signed short * const in,
  const dt1d_i_t i1,
  const dt1d_i_t i2,
  double value)
{
  SET(signed short);
}

/*..........................................................................
 * sint32 scalar set
 *..........................................................................*/

dt1d_rc_e
DT1D_API
dt1d_set_sint32(
  dtntype_s4_t * const in,
  const dt1d_i_t i1,
  const dt1d_i_t i2,
  double value)
{
  SET(dtntype_s4_t);
}

/*..........................................................................
 * float scalar set
 *..........................................................................*/

dt1d_rc_e
DT1D_API
dt1d_set_float(
  float * const in,
  const dt1d_i_t i1,
  const dt1d_i_t i2,
  double value)
{
  SET(double);
}

/*..........................................................................
 * double scalar set 
 *..........................................................................*/

dt1d_rc_e
DT1D_API
dt1d_set_double(
  double * const in,
  const dt1d_i_t i1,
  const dt1d_i_t i2,
  double value)
{
  SET(double);
}

/*..........................................................................
 * any scalar set
 *..........................................................................*/

dt1d_rc_e
DT1D_API
dt1d_set(
  dt1d_ntype_e ntype,
  void * const in,
  const dt1d_i_t i1,
  const dt1d_i_t i2,
  double value)
{
# define CASE(NTYPE, FUNC, TYPE) \
  case NTYPE: rc = FUNC((TYPE *)in, i1, i2, value); break;
  dt1d_rc_e rc;

  switch(ntype)
  {
    CASE(DT1D_NTYPE_B08, dt1d_set_uchar,  unsigned char);
    CASE(DT1D_NTYPE_B16, dt1d_set_ushort, unsigned short);
    CASE(DT1D_NTYPE_B32, dt1d_set_uint32, dtntype_b32_t);
    CASE(DT1D_NTYPE_S1,  dt1d_set_char,   signed char);
    CASE(DT1D_NTYPE_S2,  dt1d_set_short,  short);
    CASE(DT1D_NTYPE_S4,  dt1d_set_sint32,   dtntype_s4_t);
    CASE(DT1D_NTYPE_F1,  dt1d_set_float,  float);
    CASE(DT1D_NTYPE_F2,  dt1d_set_double, double);
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
