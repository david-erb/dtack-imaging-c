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
  20000404 DE added new ntypes
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
DT_RCSID("source/high $RCSfile: median.c,v $ $Revision: 1.1 $");
#include <dtack/os.h>
#include <dtack/mem.h>


/*..........................................................................
| NAME
| dtimage_median() - median filter
| 
| SUMMARY
| Create median filter.
| 
| END
 *..........................................................................*/

static
dt_rc_e
dtimage_median_b08(
  dt_ctl_t *ctl,
  dtimage_t *input,
  const int kx,
  const int ky,
  dtimage_t *output)
{
  DT_F("dtimage_median_b08");
#define NTYPE_T dt_ntype_b08_t
#define NTYPE_E DT_NTYPE_B08
#define NTYPE_M b08
#include <median.i>
#undef NTYPE_M
#undef NTYPE_E
#undef NTYPE_T
}

/*..........................................................................
 *..........................................................................*/

static
dt_rc_e
dtimage_median_b16(
  dt_ctl_t *ctl,
  dtimage_t *input,
  const int kx,
  const int ky,
  dtimage_t *output)
{
  DT_F("dtimage_median_b16");
#define NTYPE_T dt_ntype_b16_t
#define NTYPE_E DT_NTYPE_B16
#define NTYPE_M b16
#include <median.i>
#undef NTYPE_M
#undef NTYPE_E
#undef NTYPE_T
}

/*..........................................................................
 *..........................................................................*/

static
dt_rc_e
dtimage_median_b32(
  dt_ctl_t *ctl,
  dtimage_t *input,
  const int kx,
  const int ky,
  dtimage_t *output)
{
  DT_F("dtimage_median_b32");
#define NTYPE_T dt_ntype_b32_t
#define NTYPE_E DT_NTYPE_B32
#define NTYPE_M b32
#include <median.i>
#undef NTYPE_M
#undef NTYPE_E
#undef NTYPE_T
}

/*..........................................................................
 *..........................................................................*/

static
dt_rc_e
dtimage_median_s1(
  dt_ctl_t *ctl,
  dtimage_t *input,
  const int kx,
  const int ky,
  dtimage_t *output)
{
  DT_F("dtimage_median_s1");
#define NTYPE_T dt_ntype_s1_t
#define NTYPE_E DT_NTYPE_S1
#define NTYPE_M s1
#include <median.i>
#undef NTYPE_M
#undef NTYPE_E
#undef NTYPE_T
}

/*..........................................................................
 *..........................................................................*/

static
dt_rc_e
dtimage_median_s2(
  dt_ctl_t *ctl,
  dtimage_t *input,
  const int kx,
  const int ky,
  dtimage_t *output)
{
  DT_F("dtimage_median_s2");
#define NTYPE_T dt_ntype_s2_t
#define NTYPE_E DT_NTYPE_S2
#define NTYPE_M s2
#include <median.i>
#undef NTYPE_M
#undef NTYPE_E
#undef NTYPE_T
}

/*..........................................................................
 *..........................................................................*/

static
dt_rc_e
dtimage_median_s4(
  dt_ctl_t *ctl,
  dtimage_t *input,
  const int kx,
  const int ky,
  dtimage_t *output)
{
  DT_F("dtimage_median_s4");
#define NTYPE_T dt_ntype_s4_t
#define NTYPE_E DT_NTYPE_S4
#define NTYPE_M s4
#include <median.i>
#undef NTYPE_M
#undef NTYPE_E
#undef NTYPE_T
}

/*..........................................................................
 *..........................................................................*/

static
dt_rc_e
dtimage_median_f1(
  dt_ctl_t *ctl,
  dtimage_t *input,
  const int kx,
  const int ky,
  dtimage_t *output)
{
  DT_F("dtimage_median_f1");
#define NTYPE_T dt_ntype_f1_t
#define NTYPE_E DT_NTYPE_F1
#define NTYPE_M f1
#include <median.i>
#undef NTYPE_M
#undef NTYPE_E
#undef NTYPE_T
}

/*..........................................................................
 *..........................................................................*/

static
dt_rc_e
dtimage_median_f2(
  dt_ctl_t *ctl,
  dtimage_t *input,
  const int kx,
  const int ky,
  dtimage_t *output)
{
  DT_F("dtimage_median_f2");
#define NTYPE_T dt_ntype_f2_t
#define NTYPE_E DT_NTYPE_F2
#define NTYPE_M f2
#include <median.i>
#undef NTYPE_M
#undef NTYPE_E
#undef NTYPE_T
}

/*..........................................................................
| NAME
| dtimage_median() - median filter
| 
| SUMMARY
| Create median filter.
| 
| END
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtimage_median(
  dt_ctl_t *ctl,
  dtimage_t *input,
  const int kx,
  const int ky,
  dtimage_t *output)
{
  DT_F("dtimage_median");
  static dt_ntype_e ntypes[] = {
    DTNTYPE_B08, 
    DTNTYPE_B16, 
    DTNTYPE_B32, 
    DTNTYPE_S1, 
    DTNTYPE_S2, 
    DTNTYPE_S4, 
    DTNTYPE_F1, 
    DTNTYPE_F2, 
    DT_NTYPE_NULL};
  dt_rc_e rc;

  rc = dtimage_check_2f(ctl,			/* check image data types		    */
    input, "input",
    output, "output",
    ntypes, 0, F, NULL, NULL);			/* cannot handle paged images 		*/
  if (rc != DT_RC_GOOD)
    return rc;

  if (input->ntype == DTNTYPE_B08)
	DT_C(dtimage_median_b08,(ctl, input, kx, ky, output))
  else
  if (input->ntype == DTNTYPE_B16)
	DT_C(dtimage_median_b16,(ctl, input, kx, ky, output))
  else
  if (input->ntype == DTNTYPE_B32)
	DT_C(dtimage_median_b32,(ctl, input, kx, ky, output))
  else
  if (input->ntype == DTNTYPE_S1)
	DT_C(dtimage_median_s1,(ctl, input, kx, ky, output))
  else
  if (input->ntype == DTNTYPE_S2)
	DT_C(dtimage_median_s2,(ctl, input, kx, ky, output))
  else
  if (input->ntype == DTNTYPE_S4)
	DT_C(dtimage_median_s4,(ctl, input, kx, ky, output))
  else
  if (input->ntype == DTNTYPE_F1)
	DT_C(dtimage_median_f1,(ctl, input, kx, ky, output))
  else
  if (input->ntype == DTNTYPE_F2)
	DT_C(dtimage_median_f2,(ctl, input, kx, ky, output))
  else
    rc = dt_err(ctl, F, "invalid input->ntype %d", input->ntype);
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
