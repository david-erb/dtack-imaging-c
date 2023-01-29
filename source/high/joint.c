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

DT_RCSID("source/high $RCSfile: joint.c,v $ $Revision: 1.1 $");
#include <dtack/mem.h>

static dt_ntype_e joint_ntypes[] = {
  DT_NTYPE_B32, DT_NTYPE_NULL};
/*..........................................................................*/
dt_rc_e
DTCONFIG_API1
dtimage_joint(
  dt_ctl_t *ctl,
  dtimage_t *input1,
  dtimage_t *input2,
  dtimage_t *joint)
{
  DT_F("dtimage_joint");
  static dt_ntype_e *ntypes = NULL;
  dtxy_t xmax, ymax;
  dt_rc_e rc;

  rc = dtimage_check_2f(ctl,			/* check input data type		    */
    input1, "input1",
    input2, "input2",
    ntypes, DTIMAGE_FLAG_PAGED, F,
    &xmax, &ymax);
  if (rc != DT_RC_GOOD)
    return rc;

  rc = dtimage_check_1f(ctl, joint,		/* check output data type		    */
    "output joint probability matrix",
    joint_ntypes, 
    DTIMAGE_FLAG_PAGED, F);
  if (rc != DT_RC_GOOD)
    return rc;

  DT_Q(dtimage_constant,(ctl,			/* clear output image			    */
    joint, 0.0));

  switch(input1->ntype)
  {
    case DT_NTYPE_B08:
      DT_Q(dtimage_joint_b08,(ctl,
        input1, input2, xmax, ymax,
        joint));
    break;
    default:
      DT_Q(dtimage_joint_any,(ctl,
        input1, input2, xmax, ymax,
        joint));
  }

  return DT_RC_GOOD;
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
