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

DT_RCSID("source/high $RCSfile: joint2.c,v $ $Revision: 1.1 $");
#include <dtack/mem.h>

/*..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtimage_joint_b08(
  dt_ctl_t *ctl,
  dtimage_t *input1,
  dtimage_t *input2,
  dtxy_t xmax,
  dtxy_t ymax,
  dtimage_t *joint)
{
  DT_F("dtimage_joint_b08");
  dtxy_t x, y;
  dt_ntype_b08_t *p1, *p2;
  for (y=0; y<ymax; y++)				/* for all input rows			    */
  {
    DTIMAGE_GETROW(ctl, input1, y, &p1);
    DTIMAGE_GETROW(ctl, input2, y, &p2);
    for (x=0; x<xmax; x++)				/* for all input pixels			    */
	{
      dt_ntype_b32_t *q;
      DTIMAGE_GETROW(ctl, joint, p2[x],
        &q);
      q[p1[x]]++;						/* sum output matrix			    */
      DTIMAGE_PUTROW(ctl, joint, p2[x]);
	}
    DTIMAGE_UNMAPROW(ctl, input1, y);
    DTIMAGE_UNMAPROW(ctl, input2, y);
  }
  return DT_RC_GOOD;
}
/*..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtimage_joint_any(
  dt_ctl_t *ctl,
  dtimage_t *input1,
  dtimage_t *input2,
  dtxy_t xmax,
  dtxy_t ymax,
  dtimage_t *joint)
{
  DT_F("dtimage_joint_any");
  dtxy_t x, y;
  double v1, v2;
  for (y=0; y<ymax; y++)				/* for all input rows			    */
  {
    for (x=0; x<xmax; x++)				/* for all input pixels			    */
	{
      dt_ntype_b32_t *q;
      DT_Q(dtimage_get_pixel,(ctl,		/* fetch base pixel				    */
        input1, x, y, &v1));
      DT_Q(dtimage_get_pixel,(ctl,		/* fetch pixel for comparison	    */
        input2, x, y, &v2));
      DTIMAGE_GETROW(ctl, joint,
        (dtxy_t)v2, &q);
      q[(dtxy_t)v1]++;					/* sum output matrix			    */
      DTIMAGE_PUTROW(ctl, joint,
        (dtxy_t)v2);
	}
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
