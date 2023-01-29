
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
  19990124 DE added some debug
  19981212 DE changed cmp type to avoid compilation error
  19981208 DE added to the suite of tests
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

#include <dtack/t.h>
DT_RCSID("test $RCSfile: dttresample.c,v $ $Revision: 1.7 $");
#include <dtack/mem.h>

#define USAGE "[-Dnnnn]"

#define DILATE 0
#define ERODE 1

#define BIG (512)

static const char *ops[] = {"all", "fast", "time", NULL};

/*..........................................................................
 *..........................................................................*/
static 
dt_rc_e
dttresample_test_do(
  dt_ctl_t *ctl,						/* environment control 				*/
  double dx,							/* the resample scale 				*/
  double dy,
  const char *op,
  dtimage_t *input,
  dtimage_t *output,
  dtimage_t *expected,
  const char *f,
  const char *id,
  int *assert_fail)
{
  DT_F("dttresample_test_do");
  char t[32];
  long cmp;
  dtxy_t x, y;
  dt_rc_e rc = DT_RC_GOOD;

  output->xe = 0;						/* image not allocated yet 			*/
  DT_Q(dtimage_resample,(ctl,			/* make the resample output		    */
    input, .2, .2, op,
    output));

  DT_C(dtimage_cmp,(ctl,				/* compare expected to actual	    */
    output, expected, 0.00001,
    &cmp, &x, &y));

  DT_C(dtntype_to_string,(ctl,
    input->ntype, t, sizeof(t)));
  *assert_fail +=
    DT_RC_GOOD != dt_assert(
      ctl, cmp == 0,
      f, "%s %s %s", id, op, t);

  if (DT_DBG(ctl, DT_DBG_MASK_PIXEL) &&
	  cmp != 0)
  {
	double vin, vout;
	DT_C(dtimage_get_pixel,(ctl, input, x, y, &vin));
	DT_C(dtimage_get_pixel,(ctl, output, x, y, &vout));
    dt_dbg(ctl, F, DT_DBG_MASK_PIXEL,
      "image differs at [%d,%d] input=%g output=%g delta=%g", 
      x, y, vin, vout, fabs(vin-vout));
  }

  if (cmp != 0 && input->xe < 20)		/* appears to be a problem? 		*/
  {
    DT_C(dtimage_dbg_values,(ctl,
      input, NULL, DT_DBG_MASK_PIXEL));
    DT_C(dtimage_dbg_values,(ctl,
      output, NULL, DT_DBG_MASK_PIXEL));
  }

cleanup:

  return rc;
}

/*..........................................................................
 *..........................................................................*/
static 
dt_rc_e
dttresample_test_one(
  dt_ctl_t *ctl,						/* environment control 				*/
  double dx,							/* the resample scale 				*/
  double dy,
  const char *op,						/* the resample operation 				*/
  dtxy_t incols,						/* image size 						*/
  dtxy_t inrows,
  dt_ntype_e input_ntype,				/* input data array type 			*/
  void *input_data,						/* input data array 				*/
  dt_ntype_e operation_ntype,			/* operation pixel type 			*/
  dtxy_t encols,						/* expected image size				*/
  dtxy_t enrows,
  dt_ntype_e expected_ntype,			/* expected data array type 		*/
  void *expected_data,					/* expected data array 				*/
  const char *f,
  const char *id,
  int *assert_fail)
{
  DT_F("dttresample_test_one");
  dtimage_t input;
  dtimage_t output;
  dtimage_t expected;
  char t[32];
  unsigned long cmp;
  dt_rc_e rc = DT_RC_GOOD;

  input.xe = 0;
  output.xe = 0;
  expected.xe = 0;
  
  DT_C(dtimage_create,(ctl,				/* create input image 				*/
    &input, incols, inrows, 
    operation_ntype));
  
  DT_C(dtimage_assign_array,(ctl,		/* populate input image 			*/
    &input, incols, inrows,
    input_ntype, input_data));
  
  DT_C(dtimage_create,(ctl,				/* create expected image 			*/
    &expected, encols, enrows, 
    operation_ntype));
  
  DT_C(dtimage_assign_array,(ctl,		/* populate expected image 			*/
    &expected, encols, enrows,
    expected_ntype, expected_data));

  DT_C(dttresample_test_do,(ctl,		/* do the test 						*/
    dx, dy, op, 
    &input, &output, &expected,
    f, id, assert_fail));
  
cleanup:
  DT_I(dtimage_free,(ctl, &expected));
  DT_I(dtimage_free,(ctl, &output));
  DT_I(dtimage_free,(ctl, &input));

  return rc;
}

/*..........................................................................
 * convolve very simple input image with a number of kernels
 *..........................................................................*/
static
dt_rc_e
dttresample_test_all(
  dt_ctl_t *ctl,						/* environment control 				*/
  double dx,							/* the resample scale 				*/
  double dy,
  const char *op,						/* the resample operation 				*/
  dtxy_t incols,						/* image size 						*/
  dtxy_t inrows,
  dt_ntype_e input_ntype,				/* input data array type 			*/
  void *input_data,						/* input data array 				*/
  dtxy_t encols,						/* expected image size				*/
  dtxy_t enrows,
  dt_ntype_e expected_ntype,			/* expected data array type 		*/
  void *expected_data,					/* expected data array 				*/
  const char *f,
  const char *id,
  int *assert_fail)
{
  DT_F("dttresample_test_all");
  int i;
  dt_rc_e rc;

  for (i=0; dt_ntype_list[i] != DTNTYPE_NULL; i++)
  {
	if (dt_ntype_list[i] == DTNTYPE_B01)
	  continue;
	DT_C(dttresample_test_one,(ctl, 
      dx, dy, op,
      incols, inrows, 
      input_ntype, input_data,
      dt_ntype_list[i],
      encols, enrows, 
      expected_ntype, expected_data,
      f, id, assert_fail));
  }

cleanup:

#undef NCOLS
#undef NROWS
    
  return DT_RC_GOOD;
}

/*..........................................................................
 * convolve very simple input image with a number of kernels
 *..........................................................................*/
static
dt_rc_e
dttresample_small(
  dt_ctl_t *ctl,
  int *assert_fail)
{
  DT_F("dttresample_small");
  int i;
  dt_rc_e rc;

#define NCOLS 4
#define NROWS 4
#define M (16)
  static dt_ntype_b08_t input_data[NCOLS*NROWS] = {
	100, 100, 100, 100,
	100, 100, 100, 100,
	100, 100, 100, 100,
	100, 100, 100, 100
  };
  static dt_ntype_b08_t expected_data[M*M] = {
	100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,
	100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,
	100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,
	100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,
	100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,
	100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,
	100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,
	100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,
	100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,
	100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,
	100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,
	100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,
	100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,
	100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,
	100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,
	100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100
  };

  DT_C(dttresample_test_all,(ctl, 
    0.2, 0.2,
    "bilinear",
    NCOLS, NROWS, 
    DTNTYPE_B08, input_data,
    M, M,
    DTNTYPE_B08, expected_data,
    F, "bilinear", assert_fail));

  DT_C(dttresample_test_all,(ctl, 
    0.2, 0.2,
    "nearest",
    NCOLS, NROWS, 
    DTNTYPE_B08, input_data,
    M, M,
    DTNTYPE_B08, expected_data,
    F, "nearest", assert_fail));

cleanup:

#undef NCOLS
#undef NROWS
    
  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/
static
dt_rc_e
dttresample_large_one(
  dt_ctl_t *ctl,
  double dx,
  double dy,
  const char *op,
  long max,								/* image size 						*/
  long n,								/* timing loops 					*/
  dtntype_e ntype,						/* desired data type 				*/
  const char *f,
  const char *id,
  int *assert_fail)
{
  DT_F("dttresample_large");
  dtimage_t input;
  dtimage_t output;
  dtimage_t expected;
#define NCOLS (max)
#define NROWS (max)
  dtos_time_t mark1, mark2;
  long i;
  char t[32];
  char s[128];
  dt_rc_e rc = DT_RC_GOOD;

  input.xe = 0;
  output.xe = 0;
  
  DT_C(dtimage_create,(ctl,				/* create input image 				*/
    &input, NCOLS, NROWS, 
    ntype));
  
  DT_C(dtimage_constant,(ctl,			/* populate input image 			*/
    &input, 0.0));

  DT_C(dtos_time,(ctl, &mark1));
  for (i=0; i<n; i++)
  {
    DT_C(dtimage_resample,(ctl,
      &input, dx, dy, op, &output))
  }
  DT_C(dtos_time,(ctl, &mark2));

  DT_C(dtntype_to_string,(ctl,
    input.ntype, t, sizeof(t)));
  DT_C(dtstr_printf,(ctl,
    s, sizeof(s), 
    "%s %s [%gx%g] %3s [%dx%d] (%ld loops)", 
    id, op, dx, dy, t, input.xe, input.ye, n));
  DT_C(dtt_time_report,(ctl,			/* report time					    */
    &mark1, &mark2, s));
  
cleanup:
  DT_I(dtimage_free,(ctl, &output));
  DT_I(dtimage_free,(ctl, &input));

  return rc;

#undef NCOLS
#undef NROWS
}

/*..........................................................................
 *..........................................................................*/
static
dt_rc_e
dttresample_large(
  dt_ctl_t *ctl,
  double dx,
  double dy,
  long max,								/* image size 						*/
  long n,								/* timing loops 					*/
  int *assert_fail)
{
  DT_F("dttresample_large");
  int i;
  dt_rc_e rc = DT_RC_GOOD;

  for (i=0; i<2; i++)
  {
	DT_C(dttresample_large_one,(ctl,
      dx, dy, "nearest",
      max, n, DTNTYPE_B08, 
      F, "nearest", assert_fail));

	DT_C(dttresample_large_one,(ctl,
      dx, dy, "nearest",
      max, n, DTNTYPE_B16, 
      F, "nearest", assert_fail));

	DT_C(dttresample_large_one,(ctl,
      dx, dy, "bilinear",
      max, n, DTNTYPE_B08, 
      F, "bilinear", assert_fail));

	DT_C(dttresample_large_one,(ctl,
      dx, dy, "bilinear",
      max, n, DTNTYPE_B16, 
      F, "bilinear", assert_fail));

	dx = 1.0 / dx;
	dy = 1.0 / dy;
  }
  
cleanup:

  return rc;

#undef NCOLS
#undef NROWS
}


/*..........................................................................*/
dt_rc_e
DTCONFIG_API1
dttresample_arg_check(						/* check args and params            */
  dt_ctl_t *ctl,
  void *app,
  dtparam_t *param,
  dt_enter_arg_t *arg,
  char *msg,
  const char **start)
{
  DT_F("dttresample_arg_check");

  DT_Q(dtt_check_op,(ctl, param,        /* make sure op is understood       */
    ops, msg, start));

  return DT_RC_GOOD;
}

/*..........................................................................*/

DTT_TEST(dttresample)
{
  DT_F("dttresample");
  dt_ctl_t *ctl = &param->ctl;
  int assert_fail = 0;					/* failed assertion count		    */
  dt_enter_arg_t arg[1];
  dt_rc_e rc = DT_RC_GOOD;

  DT_C(dt_heapmon_init,(ctl));			/* init memory checking			    */

  DT_C(dtparam_init,(param));			/* init param structure             */

  DT_C(dtparam_set,(param, 0,			/* set default for params we want   */
    &param->op, NULL, NULL, "fast",
    &param->reps, NULL, NULL, "1",
    &param->n, "Timing loops", NULL, "4",
    &param->max, "Image size", NULL, "2048",
    &param->dx, "X scale", NULL, "0.2",
    &param->dy, "Y scale", NULL, "0.2",
    NULL));

  arg[0].desc = NULL;					/* no arguments 					*/
  
  DT_C(dt_interactive_parse,(			/* parse command line interactively	*/
    argc, argv, F, USAGE, param,
    arg, dttresample_arg_check, NULL));

  if (!strcmp(param->op, "all") ||
	  !strcmp(param->op, "fast"))
  {
	DT_C(dttresample_small,(ctl, &assert_fail));
  }

  if (!strcmp(param->op, "time"))
  {
	DT_C(dttresample_large,(ctl,
      param->dx,
      param->dy,
      (long)param->max,
      param->n,
      &assert_fail));
  }

cleanup:
  DT_I(dt_interactive_leave,(
    param, rc));
  
  DT_I(dt_heapmon_uninit,(ctl,			/* check all memory is freed	    */
    &assert_fail));

  if (assert_fail)						/* any failed assertions?		    */
    return DT_RC_BAD;
  else
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
