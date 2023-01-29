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
DT_RCSID("source/high $RCSfile: sum.c,v $ $Revision: 1.1 $");
#include <dtack/mem.h>

/*..........................................................................*/
dt_rc_e 
DTCONFIG_API1
dtimage_sum(					/* sum of pixel values				*/
  dt_ctl_t *ctl,
  dtimage_t *image,
  double *psum)
{
  DT_F("dtimage_sum");
  const dt_ntype_e *ntypes =			/* should handle any 1d datatype  	*/
    dt1d_ntype_list;
  dtxy_t y;
  double sum, s;
  dt_rc_e rc;

  rc = dtimage_check_1f(ctl,			/* check simple stuff				*/
    image, "image",
    ntypes, DTIMAGE_FLAG_PAGED, F);
  if (rc != DT_RC_GOOD)
    return rc;

  sum = 0.0;
  for (y=0; y<image->ye; y++)			/* for each row					    */
  {
    void *p;
    DTIMAGE_GETROW(ctl, image, y, &p);
    DT_Q_1D(dt1d_sum,(					/* sum squared pixels on row	   	*/
      image->ntype, p,
      image->xe, &s));
    DTIMAGE_UNMAPROW(ctl, image, y);
    sum += s;
  }
  *psum = sum;

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
