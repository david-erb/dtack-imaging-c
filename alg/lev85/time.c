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

DT_RCSID("time $RCSfile: time.c,v $ $Revision: 1.1 $");

#include <dtlev85.h>
#include <dtack/os.h>

#define USAGE "[-Dnnnn]"

/*..........................................................................*/

int
dtlev85_time_all(
  dtparam_t *param,
  int *assert_fail)
{
  DT_F("dtlev85_time");
  dt_ctl_t *ctl = &param->ctl;

  DT_Q(dtlev85_time1,(param, 
    assert_fail));

  return DT_RC_GOOD;
}

/*..........................................................................*/
static 
dt_rc_e
dtlev85_time_check_and_free(
  dt_ctl_t *ctl,
  dtimage_t *segmented,
  dtimage_t *gray,
  char *id,
  double expect_uniformity,
  double actual_uniformity,
  long expect_nregions,
  long actual_nregions,
  dtos_time_t *mark1,
  dtos_time_t *mark2,
  int *assert_fail)
{
  static char *F = "dtlev85_time_check_and_free";
  dt_rc_e rc;

  DT_Q(dtimage_free,(ctl,				/* free images					    */
    gray));
  DT_Q(dtimage_free,(ctl,				/* free images					    */
    segmented));

  rc = dt_assert(ctl,
    expect_uniformity == actual_uniformity &&
    expect_nregions == actual_nregions,
    id, "uniformity %.5f (%.5f), nregions %ld (%ld)",
    actual_uniformity, expect_uniformity,
    actual_nregions, expect_nregions);

  if (rc == DT_RC_GOOD)
    DT_Q(dtos_time_report,(ctl,
      mark1, mark2, id))
  else
    *assert_fail += 1;

  return DT_RC_GOOD;
}
/*..........................................................................*/

dt_rc_e
dtlev85_time1(
  dtparam_t *param,
  int *assert_fail)
{
  static char *F = "dtlev85_time1";
  dt_ctl_t *ctl = &param->ctl;
  dtimage_t segmented;
  dtimage_t gray;
  dtos_time_t mark1, mark2;
  dtlev85_t lev85;
  char id[32];

#define NCOLS 256
#define NROWS 256

  DT_Q(dtimage_create,(ctl,				/* space for grayscale image	    */
    &gray, NCOLS, NROWS,
    DT_NTYPE_B08));

  DT_Q(dtimage_gen_rect_tree,(ctl,		/* gen test pattern				    */
    &gray, 1.0, 255.0, 999));

  DT_Q(dtimage_create,(ctl,				/* space for segmented image	    */
    &segmented, NCOLS, NROWS,
    DT_NTYPE_B08));

  DT_Q(dtimage_cast,(ctl,				/* same data						*/
    &gray, &segmented));

  DT_Q(dtos_time,(ctl, &mark1));
  DT_Q(dtlev85_total,(param,			/* do algorithm					    */
    &gray, &segmented, 0.0,
    &lev85));
  DT_Q(dtos_time,(ctl, &mark2));

  sprintf(id, "%s %s", F,
    param->connect? 
    param->connect: "x");

  DT_Q(dtlev85_time_check_and_free,(	/* check and free				    */
    ctl, &segmented, &gray,
    id, lev85.u, 1.0,
    lev85.n, 2, 
    &mark1, &mark2, assert_fail));

#undef NCOLS
#undef NROWS
    
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
