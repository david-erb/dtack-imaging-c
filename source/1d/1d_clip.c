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
 * clip
 *..........................................................................*/

#define BODY(V1_TYPE, V2_TYPE) \
  dt1d_i_t i, n; \
  const V1_TYPE vmin = (V1_TYPE)min; \
  const V1_TYPE vmax = (V1_TYPE)max; \
  n = DT1D_MIN(nv1, nv2); \
  for (i=0; i<n; i++) \
  { \
    const V1_TYPE d = v1[i]; \
    if (d < vmin) \
      v2[i] = (V2_TYPE)vmin; \
    else \
    if (d > vmax) \
      v2[i] = (V2_TYPE)vmax; \
    else \
      v2[i] = (V2_TYPE)d; \
  } \
  return DT1D_RC_GOOD;

/*..........................................................................
 * clip double to uchar
 *..........................................................................*/

dt1d_rc_e
DT1D_API
dt1d_clip_double_uchar(
  const double * const v1,
  const dt1d_i_t nv1,
  unsigned char * const v2,
  const dt1d_i_t nv2,
  double min,
  double max)
{
  BODY(double, unsigned char);
}

/*..........................................................................
 * clip double to ushort
 *..........................................................................*/

dt1d_rc_e
DT1D_API
dt1d_clip_double_ushort(
  const double * const v1,
  const dt1d_i_t nv1,
  unsigned short * const v2,
  const dt1d_i_t nv2,
  double min,
  double max)
{
  BODY(double, unsigned short);
}

/*..........................................................................
 * clip double to uint32
 *..........................................................................*/

dt1d_rc_e
DT1D_API
dt1d_clip_double_uint32(
  const double * const v1,
  const dt1d_i_t nv1,
  dtntype_b32_t * const v2,
  const dt1d_i_t nv2,
  double min,
  double max)
{
  BODY(double, dtntype_b32_t);
}
/*..........................................................................
 * clip double to char
 *..........................................................................*/

dt1d_rc_e
DT1D_API
dt1d_clip_double_char(
  const double * const v1,
  const dt1d_i_t nv1,
  signed char * const v2,
  const dt1d_i_t nv2,
  double min,
  double max)
{
  BODY(double, char);
}

/*..........................................................................
 * clip double to short
 *..........................................................................*/

dt1d_rc_e
DT1D_API
dt1d_clip_double_short(
  const double * const v1,
  const dt1d_i_t nv1,
  signed short * const v2,
  const dt1d_i_t nv2,
  double min,
  double max)
{
  BODY(double, short);
}

/*..........................................................................
 * clip double to sint32
 *..........................................................................*/

dt1d_rc_e
DT1D_API
dt1d_clip_double_sint32(
  const double * const v1,
  const dt1d_i_t nv1,
  dtntype_s4_t * const v2,
  const dt1d_i_t nv2,
  double min,
  double max)
{
  BODY(double, dtntype_s4_t);
}

/*..........................................................................
 * clip double to float
 *..........................................................................*/

dt1d_rc_e
DT1D_API
dt1d_clip_double_float(
  const double * const v1,
  const dt1d_i_t nv1,
  float * const v2,
  const dt1d_i_t nv2,
  double min,
  double max)
{
  BODY(double, float);
}

/*..........................................................................
 * clip uint32 to uchar
 *..........................................................................*/

dt1d_rc_e
DT1D_API
dt1d_clip_uint32_uchar(
  const dtntype_b32_t * const v1,
  const dt1d_i_t nv1,
  unsigned char * const v2,
  const dt1d_i_t nv2,
  double min,
  double max)
{
  BODY(dtntype_b32_t, unsigned char);
}

/*..........................................................................
 * clip double to double
 *..........................................................................*/

dt1d_rc_e
DT1D_API
dt1d_clip_double_double(
  const double * const v1,
  const dt1d_i_t nv1,
  double * const v2,
  const dt1d_i_t nv2,
  double min,
  double max)
{
  BODY(double, unsigned char);
}

/*..........................................................................
 * clip sint32 to uchar
 *..........................................................................*/

dt1d_rc_e
DT1D_API
dt1d_clip_sint32_uchar(
  const dtntype_s4_t * const v1,
  const dt1d_i_t nv1,
  unsigned char * const v2,
  const dt1d_i_t nv2,
  double min,
  double max)
{
  BODY(dtntype_s4_t, unsigned char);
}
#undef BODY

/*..........................................................................
 * clip in place
 *..........................................................................*/

#define BODY(TYPE) \
  dt1d_i_t i; \
  for (i=0; i<nv; i++) \
  { \
    const TYPE d = v[i]; \
    if (d < vmin) \
      v[i] = vmin; \
    else \
    if (d > vmax) \
      v[i] = vmax; \
    else \
      v[i] = d; \
  } \
  return DT1D_RC_GOOD;

/*..........................................................................
 * clip in place definitions
 *..........................................................................*/

#define DT1D_BODY(NTYPE, DTYPE, NAME) \
dt1d_rc_e \
DT1D_API \
dt1d_clip_ ## NAME( \
  DTYPE * v, \
  dt1d_i_t nv, \
  const DTYPE vmin, \
  const DTYPE vmax) \
{ \
  BODY(DTYPE) \
}
DT1D_ALL_BODIES							/* clip declarations 				*/
#undef DT1D_BODY

/*..........................................................................
 * clip in place dispatch
 *..........................................................................*/

dt1d_rc_e
DT1D_API
dt1d_clip(
  dt1d_ntype_e ntype,
  const void * const v,
  const dt1d_i_t nv,
  double vmin,
  double vmax)
{
#define DT1D_BODY(NTYPE, DTYPE, NAME) \
  case NTYPE: rc = dt1d_clip_ ## NAME((DTYPE *)v, nv, (DTYPE)vmin, (DTYPE)vmax); break;
  dt1d_rc_e rc;

  switch(ntype)
  {
    DT1D_ALL_BODIES
    default: rc = dt1d_err_ntype(ntype, dt1d_ntype_list);
  }
  return rc;
  
# undef DT1D_BODY
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
