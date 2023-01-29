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
DT_RCSID("source/high $RCSfile: sumsqd.c,v $ $Revision: 1.1 $");
#include <dtack/mem.h>

/*..........................................................................*/
dt_rc_e 
DTCONFIG_API1
dtimage_sumsq_diffs(					/* sum of differences squared		*/
  dt_ctl_t *ctl,
  dtimage_t *image1,
  dtimage_t *image2,
  double *psum)
{
  DT_F("dtimage_sumsq_diffs");
  const dt_ntype_e *ntypes =			/* should handle any 1d datatype  	*/
    dt1d_ntype_list;
  dtxy_t xmax, ymax;
  dtxy_t y;
  double sum, s;
  dt_rc_e rc;

  rc = dtimage_check_2f(ctl,			/* check simple stuff				*/
    image1, "image1",
    image2, "image2",
    ntypes, DTIMAGE_FLAG_PAGED,
    F, &xmax, &ymax);
  if (rc != DT_RC_GOOD)
    return rc;

  sum = 0.0;
  for (y=0; y<ymax; y++)				/* for each row					    */
  {
    void *p1, *p2;
    DTIMAGE_GETROW(ctl, image1, y, &p1);
    DTIMAGE_GETROW(ctl, image2, y, &p2);
    DT_Q_1D(dt1d_sumsq_diffs,(			/* sum squared differences on row   */
      image1->ntype,
      p1, xmax,
      p2, xmax, &s));
    DTIMAGE_UNMAPROW(ctl, image2, y);
    DTIMAGE_UNMAPROW(ctl, image1, y);
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
