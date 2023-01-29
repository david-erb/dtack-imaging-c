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

static char rcsid[] = "test $RCSfile: test.c,v $ $Revision: 1.1 $";

#include <dtack/base.h>
#include <dt_abu89.h>

#define USAGE "[-Dnnnn]"

/*..........................................................................*/

int
dt_abu89_test(
  int argc,
  char *argv[])
{
  static char *F = "dt_abu89_test";
  dtparam_t param_block;
  dtparam_t *param = &param_block;		/* parameter structure				*/
  dt_ctl_t *ctl = &param->ctl;
  dt_rc_e rc;
  int n;
  int assert;
  int nassert = 0;						/* failed assertion count		    */

  DT_Q(dtparam_init,(param));			/* init param structure             */

										/* set default for params we want	*/
  DTPARAM_DEFAULT(dbg, 0);

  DT_Q(dt_interactive_parse,(			/* go parse args					*/
    argc, argv, F, USAGE, param, &n));

  argc -= n;
  argv += n;

  if (argc != 0)
    return dt_interactive_usage(
      param, USAGE);

  DT_G(dt_abu89_test1,(ctl, &assert));	/* test routine(s)			  	  	*/
  nassert += !assert;

  DT_G(dt_abu89_test2,(ctl, &assert));	
  nassert += !assert;

  DT_G(dt_abu89_test3,(ctl, &assert));	
  nassert += !assert;

  DT_G(dt_abu89_test4,(ctl, &assert));	
  nassert += !assert;

  if (nassert)							/* any failed assertions?		    */
    return DT_RC_BAD;
  else
    return DT_RC_GOOD;
    
}

/*..........................................................................*/
static 
dt_rc_e
dt_abu89_test_make(
  dt_ctl_t *ctl,
  long ncols,
  long nrows,
  dt_ntype_e ntype,
  dtimage_t *segmented,
  void *segmented_data,
  dtimage_t *gray,
  void *gray_data)
{
  static char *F = "dt_abu89_test_make";
  void *segs[2];
  long segsizes[2];

  segs[1] = NULL;
  segsizes[0] = ncols * nrows *
    dt_ntype_sizeof[ntype];
  segsizes[1] = 0;

  segs[0] = segmented_data;
  DT_Q(dtimage_create2,(ctl,			/* make segmented image			   */
    segmented, ncols, nrows, ntype,
    segs, segsizes));

  segs[0] = gray_data;
  DT_Q(dtimage_create2,(ctl,			/* make gray image			    	*/
    gray, ncols, nrows, ntype,
    segs, segsizes));

  return DT_RC_GOOD;
}
/*..........................................................................*/
static 
dt_rc_e
dt_abu89_test_check_and_free(
  dt_ctl_t *ctl,
  dtimage_t *segmented,
  dtimage_t *gray,
  char *f,
  double expect_uniformity,
  double actual_uniformity,
  long expect_nregions,
  long actual_nregions,
  int *assert)
{
  static char *F = "dt_abu89_test_check_and_free";

  DT_Q(dtimage_free,(ctl,				/* free images					    */
    gray));
  DT_Q(dtimage_free,(ctl,				/* free images					    */
    segmented));

  *assert = DT_RC_GOOD == dt_assert(ctl,
    expect_uniformity == actual_uniformity &&
    expect_nregions == actual_nregions,
    f, "uniformity %.5f == %.5f, nregions %ld == %ld",
    expect_uniformity, actual_uniformity,
    expect_nregions, actual_nregions);

  return DT_RC_GOOD;
}
/*..........................................................................*/

dt_rc_e
dt_abu89_test1(
  dt_ctl_t *ctl,
  int *assert)
{
  static char *F = "dt_abu89_test1";
  dtparam_t param;
  dtimage_t segmented;
  dtimage_t gray;
  double uniformity;
  long nregions;

#define NCOLS 16
#define NROWS 16
  static dt_ntype_b08_t segmented_data[NCOLS*NROWS] = {
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,
    0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,
    0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,
    0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,
    0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,
    0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,
    0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
  };
  static dt_ntype_b08_t gray_data[NCOLS*NROWS] = {
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,
    0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,
    0,2,2,2,2,0,0,0,0,0,0,0,0,0,0,0,
    0,2,2,2,2,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,2,2,1,1,0,0,0,0,0,0,0,
    0,0,0,0,0,2,2,1,1,0,0,0,0,0,0,0,
    0,0,0,0,0,2,2,1,1,0,0,0,0,0,0,0,
    0,0,0,0,0,2,2,1,1,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
  };

  *assert = 0;							/* fail if any errors			    */

  param.ctl = *ctl;						/* load up param structure		    */

  DT_Q(dt_abu89_test_make,(ctl,			/* make images					    */
    NCOLS, NROWS, DT_NTYPE_B08,
    &segmented, &segmented_data[0],
    &gray, &gray_data[0]));

  DT_Q(dt_abu89,(&param,				/* do algorithm					    */
    &segmented, 0.0,
    &gray, &uniformity, &nregions));

  DT_Q(dt_abu89_test_check_and_free,(	/* check and free				    */
    ctl, &segmented, &gray,
    F, uniformity, 0.5,
    nregions, 2, assert));

#undef NCOLS
#undef NROWS
    
  return DT_RC_GOOD;
}
/*..........................................................................*/

dt_rc_e
dt_abu89_test2(
  dt_ctl_t *ctl,
  int *assert)
{
  static char *F = "dt_abu89_test2";
  dtparam_t param;
  dtimage_t segmented;
  dtimage_t gray;
  double uniformity;
  long nregions;

#define NCOLS 16
#define NROWS 16
  static dt_ntype_b08_t segmented_data[NCOLS*NROWS] = {
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,1,2,3,4,0,0,0,0,0,0,0,0,0,0,0,
    0,1,2,3,4,0,0,0,0,0,0,0,0,0,0,0,
    0,1,2,3,4,0,0,0,0,0,0,0,0,0,0,0,
    0,1,2,3,4,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,
    0,0,0,0,0,2,2,2,2,0,0,0,0,0,0,0,
    0,0,0,0,0,3,3,3,3,0,0,0,0,0,0,0,
    0,0,0,0,0,4,4,4,4,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
  };
  static dt_ntype_b08_t gray_data[NCOLS*NROWS] = {
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,
    0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,
    0,2,2,2,2,0,0,0,0,0,0,0,0,0,0,0,
    0,2,2,2,2,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,2,2,1,1,0,0,0,0,0,0,0,
    0,0,0,0,0,2,2,1,1,0,0,0,0,0,0,0,
    0,0,0,0,0,2,2,1,1,0,0,0,0,0,0,0,
    0,0,0,0,0,2,2,1,1,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
  };

  *assert = 0;							/* fail if any errors			    */

  param.ctl = *ctl;						/* load up param structure		    */

  DT_Q(dt_abu89_test_make,(ctl,			/* make images					    */
    NCOLS, NROWS, DT_NTYPE_B08,
    &segmented, &segmented_data[0],
    &gray, &gray_data[0]));

  DT_Q(dt_abu89,(&param,				/* do algorithm					    */
    &segmented, 0.0,
    &gray, &uniformity, &nregions));

  DT_Q(dt_abu89_test_check_and_free,(	/* check and free				    */
    ctl, &segmented, &gray,
    F, uniformity, 0.5,
    nregions, 8, assert));

#undef NCOLS
#undef NROWS
    
  return DT_RC_GOOD;
}
/*..........................................................................*/

dt_rc_e
dt_abu89_test3(
  dt_ctl_t *ctl,
  int *assert)
{
  static char *F = "dt_abu89_test3";
  dtparam_t param;
  dtimage_t segmented;
  dtimage_t gray;
  double uniformity;
  long nregions;

#define NCOLS 16
#define NROWS 16
  static dt_ntype_b08_t segmented_data[NCOLS*NROWS] = {
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
  };
  static dt_ntype_b08_t gray_data[NCOLS*NROWS] = {
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,
    0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,
    0,2,2,2,2,0,0,0,0,0,0,0,0,0,0,0,
    0,2,2,2,2,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,2,2,1,1,0,0,0,0,0,0,0,
    0,0,0,0,0,2,2,1,1,0,0,0,0,0,0,0,
    0,0,0,0,0,2,2,1,1,0,0,0,0,0,0,0,
    0,0,0,0,0,2,2,1,1,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
  };

  *assert = 0;							/* fail if any errors			    */

  param.ctl = *ctl;						/* load up param structure		    */

  DT_Q(dt_abu89_test_make,(ctl,			/* make images					    */
    NCOLS, NROWS, DT_NTYPE_B08,
    &segmented, &segmented_data[0],
    &gray, &gray_data[0]));

  DT_Q(dt_abu89,(&param,				/* do algorithm					    */
    &segmented, 0.0,
    &gray, &uniformity, &nregions));

  DT_Q(dt_abu89_test_check_and_free,(	/* check and free				    */
    ctl, &segmented, &gray,
    F, uniformity, 0.0,
    nregions, 0, assert));

#undef NCOLS
#undef NROWS
    
  return DT_RC_GOOD;
}
/*..........................................................................*/

dt_rc_e
dt_abu89_test4(
  dt_ctl_t *ctl,
  int *assert)
{
  static char *F = "dt_abu89_test4";
  dtparam_t param;
  dtimage_t segmented;
  dtimage_t gray;
  double uniformity;
  long nregions;

#define NCOLS 16
#define NROWS 16
  static dt_ntype_b08_t segmented_data[NCOLS*NROWS] = {
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
  };
  static dt_ntype_b08_t gray_data[NCOLS*NROWS] = {
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,
    0,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,
    0,8,8,8,8,8,8,0,0,0,0,0,0,0,0,0,
    0,8,8,8,8,8,8,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,8,8,8,4,4,0,0,0,0,0,0,0,
    0,0,0,0,8,8,8,4,4,0,0,0,0,0,0,0,
    0,0,0,0,8,8,8,4,4,0,0,0,0,0,0,0,
    0,0,0,0,8,8,8,4,4,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
  };

  *assert = 0;							/* fail if any errors			    */

  param.ctl = *ctl;						/* load up param structure		    */

  DT_Q(dt_abu89_test_make,(ctl,			/* make images					    */
    NCOLS, NROWS, DT_NTYPE_B08,
    &segmented, &segmented_data[0],
    &gray, &gray_data[0]));

  DT_Q(dt_abu89,(&param,				/* do algorithm					    */
    &segmented, 1.0,
    &gray, &uniformity, &nregions));

  DT_Q(dt_abu89_test_check_and_free,(	/* check and free				    */
    ctl, &segmented, &gray,
    F, uniformity, 0.8125,
    nregions, 1, assert));

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
