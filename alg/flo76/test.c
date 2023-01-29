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

DT_RCSID("flo76 $RCSfile: test.c,v $ $Revision: 1.1 $");

#include <dt_flo76.h>
#include <dtack/mem.h>

#define USAGE "[-Dnnnn]"

/*..........................................................................*/

int
dt_flo76_test_all(
  dtparam_t *param,
  int *assert_fail)
{
  DT_F("dt_flo76_test");
  dt_ctl_t *ctl = &param->ctl;

  DT_Q(dt_flo76_test1,(param,			/* generated images				    */
    assert_fail));

  return DT_RC_GOOD;
}


/*..........................................................................*/

dt_rc_e
dt_flo76_test(
  dtparam_t *param,
  dtimage_t *source,
  char *id,
  int *assert_fail)
{
  DT_F("dt_flo76_test");
  dt_ctl_t *ctl = &param->ctl;
  dtimage_t fast = {0};
  dtimage_t easy = {0};
  double source_mean, easy_mean;
  long cmp;
  dt_rc_e rc = DT_RC_GOOD;

  DT_GI(dtimage_create,(ctl,			/* space for easy image		    	*/
    &easy, source->xe, source->ye,
    DT_NTYPE_B08));

  param->variant = "easy";
  DT_GI(dt_flo76,(param, source,		/* make easy image			    	*/
    &easy));

  DT_GI(dtimage_create,(ctl,			/* space for fast image		    	*/
    &fast, source->xe, source->ye,
    DT_NTYPE_B08));

  param->variant = "fast";
  DT_GI(dt_flo76,(param, source,		/* make fast image			    	*/
    &fast));

  DT_GI(dtimage_cmp,(ctl,				/* compare fast and easy		    */
    &fast, &easy, &cmp));

  if (rc == DT_RC_GOOD)
  *assert_fail += 
    DT_RC_GOOD != dt_assert(ctl,
      cmp == fast.xe, id,
      "fast/easy comparison %ld (%u)",
      cmp, fast.xe);


  DT_GI(dtimage_mean,(ctl, source,		/* get source image mean			*/
    &source_mean));

  DT_GI(dtimage_mean,(ctl, &fast,		/* get fast image mean			    */
    &easy_mean));

  if (rc == DT_RC_GOOD)
  *assert_fail += 
    DT_RC_GOOD != dt_assert(ctl,
      fabs(easy_mean-source_mean) < 1.0, id,
      "means %5.1f %5.1f diff %5.3f (<%e)",
      source_mean, easy_mean,
      fabs(easy_mean-source_mean), 1.0);

  DT_I(dtimage_free,(ctl, &easy));
  DT_I(dtimage_free,(ctl, &fast));

  return rc;
}

/*..........................................................................*/

dt_rc_e
dt_flo76_test1(
  dtparam_t *param,
  int *assert_fail)
{
  DT_F("dt_flo76_test1");
  dt_ctl_t *ctl = &param->ctl;
  dtimage_t source;
  dt_rc_e rc = DT_RC_GOOD;

  param->max = 255;						/* we will use 8-bit images		    */
  param->min = 0;

  DT_Q(dtimage_create,(ctl,				/* space for source image		    */
    &source, 100, 100,
    DT_NTYPE_B08));

  DT_GI(dtimage_gen_test1,(ctl,			/* make source image			    */
    &source));

  DT_GI(dt_flo76_test,(param,			/* run the test					    */
    &source, "gen_test1", 
    assert_fail));

  DT_I(dtimage_free,(ctl, &source));

  return rc;
}
/*..........................................................................*/





/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
  ! Copyright (C) 1998 Dtack Inc. All Rights Reserved
  ! To use this file, you must have signed a license agreement with Dtack Inc.
  ! Under no circumstances may you redistribute this file.
  ! This software is provided AS IS with no warranty expressed or implied.
  ! Dtack Inc. accepts no liability for use or misuse of this file.
  ! http://www.dtack.com  dtack@dtack.com  telephone +360.417.6700
  ! Dtack Inc, P.O. Box 3061, Port Angeles, WA USA 98362
  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */
