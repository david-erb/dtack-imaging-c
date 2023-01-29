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
  CHANGES
  19981212 DE removed unrechable return statements
  19981208 DE optimized
 *..........................................................................*/

/*..........................................................................
| NAME
| Introduction - intro
| 
| DESCRIPTION
| As yet undocumented.
| 
| RETURN VALUES
| Function return value will be DT_RC_GOOD on success.  
| Any other return value indicates failure or noncompletion.
| When DT_RC_GOOD is not returned, any other return values may not be valid.
| 
| ERRORS
| As yet undocumented.
| 
| END
 *..........................................................................*/

#include <dtack/base.h>
DT_RCSID("source/high $RCSfile: morph.c,v $ $Revision: 1.1 $");
#include <dtack/mem.h>
#include <dtack/os.h>

/*..........................................................................*/
static
dt_rc_e
dtimage_dilate3x3_s4(
  dt_ctl_t *ctl,
  dtimage_t *input,
  dtimage_t *output)
{
  DT_F("dtimage_dilate3x3_s4");
  
#define KOP     <
#define KTYPE   dt_ntype_s4_t
#define KTYPE_E DT_NTYPE_S4

#include <morph3x3.i>

#undef KOP    
#undef KTYPE  
#undef KTYPE_E
}

/*..........................................................................*/
static
dt_rc_e
dtimage_dilate3x3_b08(
  dt_ctl_t *ctl,
  dtimage_t *input,
  dtimage_t *output)
{
  DT_F("dtimage_dilate3x3_b08");
  
#define KOP     <
#define KTYPE   dt_ntype_b08_t
#define KTYPE_E DT_NTYPE_B08

#include <morph3x3.i>

#undef KOP    
#undef KTYPE  
#undef KTYPE_E
}

/*..........................................................................*/
static
dt_rc_e
dtimage_dilate3x3_b16(
  dt_ctl_t *ctl,
  dtimage_t *input,
  dtimage_t *output)
{
  DT_F("dtimage_dilate3x3_b16");
  
#define KOP     <
#define KTYPE   dt_ntype_b16_t
#define KTYPE_E DT_NTYPE_B16

#include <morph3x3.i>

#undef KOP    
#undef KTYPE  
#undef KTYPE_E
}

/*..........................................................................*/
static
dt_rc_e
dtimage_dilate3x3_b32(
  dt_ctl_t *ctl,
  dtimage_t *input,
  dtimage_t *output)
{
  DT_F("dtimage_dilate3x3_b32");
  
#define KOP     <
#define KTYPE   dt_ntype_b32_t
#define KTYPE_E DT_NTYPE_B32

#include <morph3x3.i>

#undef KOP    
#undef KTYPE  
#undef KTYPE_E
}

/*..........................................................................*/
static
dt_rc_e
dtimage_dilate3x3_f2(
  dt_ctl_t *ctl,
  dtimage_t *input,
  dtimage_t *output)
{
  DT_F("dtimage_dilate3x3_f2");
  
#define KOP     <
#define KTYPE   dt_ntype_f2_t
#define KTYPE_E DT_NTYPE_F2

#include <morph3x3.i>

#undef KOP    
#undef KTYPE  
#undef KTYPE_E
}

/*..........................................................................*/
dt_rc_e
DTCONFIG_API1
dtimage_dilate3x3(
  dt_ctl_t *ctl,
  dtimage_t *input,
  dtimage_t *output)
{
  DT_F("dtimage_dilate3x3");
  dt_rc_e rc = DT_RC_GOOD;

  if (DTNTYPE_ISFLOAT(input->ntype))
	DT_C(dtimage_dilate3x3_f2,(ctl, input, output))
  else
  if (DTNTYPE_ISSIGNED(input->ntype))
	DT_C(dtimage_dilate3x3_s4,(ctl, input, output))
  else
  if (input->ntype == DTNTYPE_B08)
	DT_C(dtimage_dilate3x3_b08,(ctl, input, output))
  else
  if (input->ntype == DTNTYPE_B16)
	DT_C(dtimage_dilate3x3_b16,(ctl, input, output))
  else
	DT_C(dtimage_dilate3x3_b32,(ctl, input, output))

cleanup:
  return rc;
}


/*..........................................................................*/
/*..........................................................................*/
/*..........................................................................*/
/*..........................................................................*/
/*..........................................................................*/
static
dt_rc_e
dtimage_erode3x3_s4(
  dt_ctl_t *ctl,
  dtimage_t *input,
  dtimage_t *output)
{
  DT_F("dtimage_erode3x3_s4");
  
#define KOP     >
#define KTYPE   dt_ntype_s4_t
#define KTYPE_E DT_NTYPE_S4

#include <morph3x3.i>

#undef KOP    
#undef KTYPE  
#undef KTYPE_E
}

/*..........................................................................*/
static
dt_rc_e
dtimage_erode3x3_b08(
  dt_ctl_t *ctl,
  dtimage_t *input,
  dtimage_t *output)
{
  DT_F("dtimage_erode3x3_b08");
  
#define KOP     >
#define KTYPE   dt_ntype_b08_t
#define KTYPE_E DT_NTYPE_B08

#include <morph3x3.i>

#undef KOP    
#undef KTYPE  
#undef KTYPE_E
}

/*..........................................................................*/
static
dt_rc_e
dtimage_erode3x3_b16(
  dt_ctl_t *ctl,
  dtimage_t *input,
  dtimage_t *output)
{
  DT_F("dtimage_erode3x3_b16");
  
#define KOP     >
#define KTYPE   dt_ntype_b16_t
#define KTYPE_E DT_NTYPE_B16

#include <morph3x3.i>

#undef KOP    
#undef KTYPE  
#undef KTYPE_E
}

/*..........................................................................*/
static
dt_rc_e
dtimage_erode3x3_b32(
  dt_ctl_t *ctl,
  dtimage_t *input,
  dtimage_t *output)
{
  DT_F("dtimage_erode3x3_b32");
  
#define KOP     >
#define KTYPE   dt_ntype_b32_t
#define KTYPE_E DT_NTYPE_B32

#include <morph3x3.i>

#undef KOP    
#undef KTYPE  
#undef KTYPE_E
}

/*..........................................................................*/
static
dt_rc_e
dtimage_erode3x3_f2(
  dt_ctl_t *ctl,
  dtimage_t *input,
  dtimage_t *output)
{
  DT_F("dtimage_erode3x3_f2");
  
#define KOP     >
#define KTYPE   dt_ntype_f2_t
#define KTYPE_E DT_NTYPE_F2

#include <morph3x3.i>

#undef KOP    
#undef KTYPE  
#undef KTYPE_E
}

/*..........................................................................*/
dt_rc_e
DTCONFIG_API1
dtimage_erode3x3(
  dt_ctl_t *ctl,
  dtimage_t *input,
  dtimage_t *output)
{
  DT_F("dtimage_erode3x3");
  dt_rc_e rc = DT_RC_GOOD;

  if (DTNTYPE_ISFLOAT(input->ntype))
	DT_C(dtimage_erode3x3_f2,(ctl, input, output))
  else
  if (DTNTYPE_ISSIGNED(input->ntype))
	DT_C(dtimage_erode3x3_s4,(ctl, input, output))
  else
  if (input->ntype == DTNTYPE_B08)
	DT_C(dtimage_erode3x3_b08,(ctl, input, output))
  else
  if (input->ntype == DTNTYPE_B16)
	DT_C(dtimage_erode3x3_b16,(ctl, input, output))
  else
	DT_C(dtimage_erode3x3_b32,(ctl, input, output))

cleanup:
  return rc;
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
