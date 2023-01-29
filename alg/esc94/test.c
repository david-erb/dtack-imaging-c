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
#include <dtack/os.h>

DT_RCSID("esc94 $RCSfile: test.c,v $ $Revision: 1.2 $");

#include <dt_esc94.h>
#include <dtack/mem.h>

#define USAGE "[-Dnnnn]"


/*..........................................................................*/

dt_rc_e
dt_esc94_test_all(
  dtparam_t *param,
  int *assert_fail)
{
  DT_F("dt_esc94_test");
  dt_ctl_t *ctl = &param->ctl;
  dt_rc_e rc = DT_RC_GOOD;

  DT_GI(dt_heapmon_init,(ctl));			/* init memory checking			    */

  DT_GI(dt_esc94_test1,(param,			/* generated images				    */
    assert_fail));

  DT_I(dt_heapmon_uninit,(ctl,			/* check all memory is freed	    */
    assert_fail));

  return rc;
}


/*..........................................................................*/
static
dt_rc_e
dt_esc94_test_cmp_exact(
  dtparam_t *param,
  dtimage_t *img1,
  dtimage_t *img2,
  const char *id,
  const char *id1,
  const char *id2,
  int *assert_fail)
{
  DT_F("dt_esc94_test_cmp_exact");
  dt_ctl_t *ctl = &param->ctl;
  long ndiffs;
  dt_rc_e rc = DT_RC_GOOD;

#ifdef NONO
  DT_GI(dtimage_cmp_sum,(ctl,			/* compare img1 and img2		    */
    img1, img2, &ndiffs));
#endif

  if (rc == DT_RC_GOOD)
  *assert_fail += 
    DT_RC_GOOD != dt_assert(ctl,
      ndiffs == 0, id,
      "%s/%s ndiffs %ld (0)",
      id1, id2, ndiffs);

  return rc;
}

/*..........................................................................*/
static
dt_rc_e
dt_esc94_test_cmp_inexact(
  dtparam_t *param,
  dtimage_t *img1,
  dtimage_t *img2,
  char *id,
  char *id1,
  char *id2,
  double epsilon,
  int *assert_fail)
{
  DT_F("dt_esc94_test_cmp");
  dt_ctl_t *ctl = &param->ctl;
  double img1_mean, img2_mean;
  dt_rc_e rc = DT_RC_GOOD;

  DT_GI(dtimage_mean,(ctl, img1,		/* get img1 image mean				*/
    &img1_mean));

  DT_GI(dtimage_mean,(ctl, img2,		/* get img2 image mean			    */
    &img2_mean));

  if (rc == DT_RC_GOOD)
  *assert_fail += 
    DT_RC_GOOD != dt_assert(ctl,
      fabs(img2_mean-img1_mean) < epsilon, id,
      "%s %5.1f, %s %5.1f, diff %5.3f (<%e)",
      id1, img1_mean,
      id2, img2_mean,
      fabs(img2_mean-img1_mean), epsilon);

  return rc;
}

/*..........................................................................*/
static
dt_rc_e
dt_esc94_test_known(
  dtparam_t *param,
  dtimage_t *source,
  dtimage_t *known,
  char *id,
  char *op,
  int *assert_fail)
{
  DT_F("dt_esc94_test");
  dt_ctl_t *ctl = &param->ctl;
  dtimage_t result;
  char subid[32];
  dt_rc_e rc = DT_RC_GOOD;

  sprintf(subid, "%s known", op);
  result.xe = 0;
  DT_GI(dtimage_create,(ctl,			/* space for result image	    	*/
    &result, source->xe, source->ye,
    DT_NTYPE_B08));

  DT_GI(dt_esc94,(param, source,		/* make easy result image		    */
    &result));
  DT_GI(dt_esc94_test_cmp_exact,(param,	/* compare images					*/
    &result, known, id,
    subid, param->variant, 
    assert_fail));

  DT_I(dtimage_free,(ctl, &result));

  return rc;
}

/*..........................................................................*/
static
dt_rc_e
dt_esc94_test_unknown(
  dtparam_t *param,
  dtimage_t *source,
  char *id,
  int *assert_fail)
{
  DT_F("dt_esc94_test");
  dt_ctl_t *ctl = &param->ctl;
  dtimage_t result1;
  dtimage_t result2;
  dt_rc_e rc = DT_RC_GOOD;

  param->t = 0.0;						/* give consistent results		    */
  param->k = 1.0;
  param->lambda = 0.0;

  result1.xe = 0;						/* clear structures				    */
  result2.xe = 0;

  DT_GI(dtimage_create,(ctl,			/* space for result1 image	    	*/
    &result1, source->xe, source->ye,
    DT_NTYPE_B08));

  DT_GI(dtimage_create,(ctl,			/* space for result2 image	    	*/
    &result2, source->xe, source->ye,
    DT_NTYPE_B08));
										/* ................................ */
  param->variant = "easy";
  DT_GI(dt_esc94,(param, source,		/* make easy std image		    	*/
    &result1));
  DT_GI(dt_esc94_test_cmp_inexact,(		/* compare to source				*/
    param,
    source, &result1, id,
    "source", "easy-std",
    1.5, assert_fail));

										/* ................................ */
  param->variant = "fast";

  DT_GI(dt_esc94,(param, source,		/* make fast std image		    	*/
    &result2));
  DT_GI(dt_esc94_test_cmp_inexact,(		/* compare to source				*/
    param,
    source, &result2, id,
    "source", "fast-std",
    1.5, assert_fail));

  DT_GI(dt_esc94_test_cmp_inexact,(		/* compare fast to easy				*/
    param,
    &result2, &result1, id,
    "fast", "easy", 
    0.2, assert_fail));
										/* ................................ */

  DT_I(dtimage_free,(ctl, &result2));
  DT_I(dtimage_free,(ctl, &result1));

  return rc;
}

/*..........................................................................*/

dt_rc_e
dt_esc94_test1(
  dtparam_t *param,
  int *assert_fail)
{
  DT_F("dt_esc94_test1");
  dt_ctl_t *ctl = &param->ctl;
  dtimage_t source;
  dtimage_t known1;
  dtimage_t known8;
  char *filename = NULL;
  dt_rc_e rc = DT_RC_GOOD;

  DT_Q(dtos_malloc2,(ctl,				/* space for the filename		    */
    (void **)&filename,
    strlen(param->datadir)+32,
    F, "filename"));
										/* ................................ */

  source.xe = 0;
  known1.xe = 0;
  known8.xe = 0;

  param->t = 0.0;						/* give consistent results		    */
  param->k = 1.0;
  param->lambda = 0.0;

  param->max = 255;						/* we will use 8-bit images		    */
  param->min = 0;

  DT_GI(dtimage_create,(ctl,			/* space for source image		    */
    &source, 100, 100,
    DT_NTYPE_B08));

  DT_GI(dtimage_gen_test1,(ctl,			/* make source image			    */
    &source));

  DT_GI(dt_esc94_test_unknown,(param,	/* run the test					    */
    &source, "gen_test1", 
    assert_fail));

  DT_I(dtimage_free,(ctl, &source));


										/* ................................ */
# define DICE64 "dice64"
  sprintf(filename, "%s/%s.pgm",
    param->datadir, DICE64);
  source.xe = 0;
  DT_GI(dtimage_import,(ctl,			/* read test image from disk 		*/
    &source, filename));
										/* ................................ */
# define DICE64E "dice64e"
  sprintf(filename, "%s/%s.pbm",
    param->datadir, DICE64E);
  known1.xe = 0;
  DT_GI(dtimage_import,(ctl,			/* get known 1-bit image	    	*/
    &known1, filename));
  known8.xe = 0;
  DT_GI(dtimage_create,(ctl,			/* space for known 8-bit image	   	*/
    &known8,
    known1.xe, known1.ye,
    DT_NTYPE_B08));
  DT_GI(dtimage_cast,(ctl,				/* convert 1-bit to 8-bit		    */
    &known1, &known8));

  param->variant = "easy";
  DT_GI(dt_esc94_test_known,(param,		/* process it with k and cells		*/
    &source, &known8, 
    DICE64E, "cell", assert_fail));

  DT_I(dtimage_free,(ctl, &known8));
  DT_I(dtimage_free,(ctl, &known1));

										/* ................................ */
# define DICE64F "dice64f"
  sprintf(filename, "%s/%s.pbm",
    param->datadir, DICE64F);
  known1.xe = 0;
  DT_GI(dtimage_import,(ctl,			/* get known 1-bit image	    	*/
    &known1, filename));
  known8.xe = 0;
  DT_GI(dtimage_create,(ctl,			/* space for known 8-bit image	   	*/
    &known8,
    known1.xe, known1.ye,
    DT_NTYPE_B08));
  DT_GI(dtimage_cast,(ctl,				/* convert 1-bit to 8-bit		    */
    &known1, &known8));

  param->variant = "fast";
  DT_GI(dt_esc94_test_known,(param,		/* process it with k and cells		*/
    &source, &known8, 
    DICE64F, "CELL", assert_fail));

  DT_I(dtimage_free,(ctl, &known8));
  DT_I(dtimage_free,(ctl, &known1));

										/* ................................ */
  DT_I(dtimage_free,(ctl, &source));
  DT_I(dtos_free2,(ctl, filename,
    F, "filename"));

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
