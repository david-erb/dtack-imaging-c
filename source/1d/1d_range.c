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
| END
 *..........................................................................*/

#include <dtack/config.h>				/* platform-specific defines		*/
#include <dtack/ntype.h>
#include <dtack/stats.h>
#include <dtack/1d.h>

#define BODY(TYPE)                                                            \
  dt1d_i_t i, imin, imax;                                                     \
  TYPE vmin, vmax;                                                            \
  imin = imax = 0;                                                            \
  vmin = vmax = v[0];                                                         \
  for (i=1; i<nv; i++)					/* scan data					    */\
  {                                                                           \
    const TYPE d = v[i];				/* fetch data element			    */\
    if (d < vmin)                                                             \
      vmin = d, imin = i;                                                     \
    else                                                                      \
    if (d > vmax)                                                             \
      vmax = d, imax = i;                                                     \
  }                                                                           \
  if (vimin != NULL) *vimin = imin;		/* caller wants return?			    */\
  if (vimax != NULL) *vimax = imax;                                           \
  return DT1D_RC_GOOD;

/*..........................................................................
 * unsigned char vector min/max finder
 * returns index to min and max
 *..........................................................................*/

dt1d_rc_e
DT1D_API
dt1d_range_uchar(
  const unsigned char * const v,
  const dt1d_i_t nv,
  dt1d_i_t *vimin,
  dt1d_i_t *vimax)
{
  BODY(unsigned char);
}

/*..........................................................................
 * unsigned short vector min/max finder
 * returns index to min and max
 *..........................................................................*/

dt1d_rc_e
DT1D_API
dt1d_range_ushort(
  const unsigned short * const v,
  const dt1d_i_t nv,
  dt1d_i_t *vimin,
  dt1d_i_t *vimax)
{
  BODY(unsigned short);
}

/*..........................................................................
 * uint32 vector min/max finder
 * returns index to min and max
 *..........................................................................*/

dt1d_rc_e
DT1D_API
dt1d_range_uint32(
  const dtntype_b32_t * const v,
  const dt1d_i_t nv,
  dt1d_i_t *vimin,
  dt1d_i_t *vimax)
{
  BODY(dtntype_b32_t);
}

/*..........................................................................
 * char vector min/max finder
 * returns index to min and max
 *..........................................................................*/

dt1d_rc_e
DT1D_API
dt1d_range_char(
  const signed char * const v,
  const dt1d_i_t nv,
  dt1d_i_t *vimin,
  dt1d_i_t *vimax)
{
  BODY(signed char);
}

/*..........................................................................
 * short vector min/max finder
 * returns index to min and max
 *..........................................................................*/

dt1d_rc_e
DT1D_API
dt1d_range_short(
  const short * const v,
  const dt1d_i_t nv,
  dt1d_i_t *vimin,
  dt1d_i_t *vimax)
{
  BODY(short);
}

/*..........................................................................
 * long vector min/max finder
 * returns index to min and max
 *..........................................................................*/

dt1d_rc_e
DT1D_API
dt1d_range_sint32(
  const dtntype_s4_t * const v,
  const dt1d_i_t nv,
  dt1d_i_t *vimin,
  dt1d_i_t *vimax)
{
  BODY(dtntype_s4_t);
}

/*..........................................................................
 * float vector min/max finder
 * returns index to min and max
 *..........................................................................*/

dt1d_rc_e
DT1D_API
dt1d_range_float(
  const float * const v,
  const dt1d_i_t nv,
  dt1d_i_t *vimin,
  dt1d_i_t *vimax)
{
  BODY(float);
}

/*..........................................................................
 * double vector min/max finder
 * returns index to min and max
 *..........................................................................*/

dt1d_rc_e
DT1D_API
dt1d_range_double(
  const double * const v,
  const dt1d_i_t nv,
  dt1d_i_t *vimin,
  dt1d_i_t *vimax)
{
  BODY(double);
}

/*..........................................................................
 * the int routine is called direct by dtrun_rows_max() for example
 * returns index to min and max
 *..........................................................................*/

dt1d_rc_e
DT1D_API
dt1d_range_int(
  const int * const v,
  const dt1d_i_t nv,
  dt1d_i_t *vimin,
  dt1d_i_t *vimax)
{
  BODY(int);
}

/*..........................................................................
 * any range
 *..........................................................................*/

dt1d_rc_e
DT1D_API
dt1d_range(
  dt1d_ntype_e ntype,
  const void * const v,
  const dt1d_i_t nv,
  dt1d_i_t *vimin,
  dt1d_i_t *vimax)
{
# define CASE(NTYPE, FUNC, TYPE) \
  case NTYPE: rc = FUNC((TYPE *)v, nv, vimin, vimax); break;
  dt1d_rc_e rc;

  switch(ntype)
  {
    CASE(DT1D_NTYPE_B08, dt1d_range_uchar,  unsigned char);
    CASE(DT1D_NTYPE_B16, dt1d_range_ushort, unsigned short);
    CASE(DT1D_NTYPE_B32, dt1d_range_uint32, dtntype_b32_t);
    CASE(DT1D_NTYPE_S1,  dt1d_range_char,   signed char);
    CASE(DT1D_NTYPE_S2,  dt1d_range_short,  short);
    CASE(DT1D_NTYPE_S4,  dt1d_range_sint32, dtntype_s4_t);
    CASE(DT1D_NTYPE_F1,  dt1d_range_float,  float);
    CASE(DT1D_NTYPE_F2,  dt1d_range_double, double);
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
