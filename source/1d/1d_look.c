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
 * lookup every member of array in table
 *..........................................................................*/

#define LOOKUP_BODY \
  const dt1d_i_t n = \
    DT1D_MIN(nin, nout);				/* limit length to minimum		    */\
  dt1d_i_t i; \
  for (i=0; i<n; i++) \
    out[i] = table[(unsigned int)in[i]]; \
  return DT1D_RC_GOOD;

/*..........................................................................
 * uint32
 *..........................................................................*/

dt1d_rc_e
DT1D_API
dt1d_lookup_uint32(						/* lookup every member in table		*/
  dtntype_b32_t * const in,
  const dt1d_i_t nin,
  dtntype_b32_t * const out,
  const dt1d_i_t nout,
  const dtntype_b32_t * const table)
{
  LOOKUP_BODY;
}

/*..........................................................................
 * long
 *..........................................................................*/

dt1d_rc_e
DT1D_API
dt1d_lookup_sint32(						/* lookup every member in table		*/
  dtntype_s4_t * const in,
  const dt1d_i_t nin,
  dtntype_s4_t * const out,
  const dt1d_i_t nout,
  const dtntype_s4_t * const table)
{
  LOOKUP_BODY;
}

/*..........................................................................
 * uchar
 *..........................................................................*/

dt1d_rc_e
DT1D_API
dt1d_lookup_uchar(						/* lookup every member in table		*/
  unsigned char * const in,
  const dt1d_i_t nin,
  unsigned char * const out,
  const dt1d_i_t nout,
  const unsigned char * const table)
{
  LOOKUP_BODY;
}

/*..........................................................................
 * lookup every member of array in table
 *..........................................................................*/

#define LOOKUP_INPLACE_BODY \
  dt1d_i_t i; \
  for (i=0; i<nv; i++) \
    v[i] = table[(unsigned int)v[i]]; \
  return DT1D_RC_GOOD;

/*..........................................................................
 * uint32
 *..........................................................................*/

dt1d_rc_e
DT1D_API
dt1d_lookup_inplace_uint32(				/* lookup every member in table		*/
  dtntype_b32_t * const v,
  const dt1d_i_t nv,
  const dtntype_b32_t * const table)
{
  LOOKUP_INPLACE_BODY;
}

/*..........................................................................
 * long
 *..........................................................................*/

dt1d_rc_e
DT1D_API
dt1d_lookup_inplace_sint32(				/* lookup every member in table		*/
  dtntype_s4_t * const v,
  const dt1d_i_t nv,
  const dtntype_s4_t * const table)
{
  LOOKUP_INPLACE_BODY;
}

/*..........................................................................
 * uchar
 *..........................................................................*/

dt1d_rc_e
DT1D_API
dt1d_lookup_inplace_uchar(				/* lookup every member in table		*/
  unsigned char * const v,
  const dt1d_i_t nv,
  const unsigned char * const table)
{
  LOOKUP_INPLACE_BODY;
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
