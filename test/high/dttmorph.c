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
  20000411 DE test edges with marker value
  20000404 DE added median
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
DT_RCSID("test $RCSfile: dttmorph.c,v $ $Revision: 1.7 $");
#include <dtack/mem.h>

#define USAGE "[-Dnnnn]"

#define DILATE 0
#define ERODE 1
#define MEDIAN 2

#define BIG (512)

static const char *ops[] = {"all", "fast", "time", NULL};

/*..........................................................................
 *..........................................................................*/
static 
dt_rc_e
dttmorph_test3x3_do(
  dt_ctl_t *ctl,						/* environment control 				*/
  int op,								/* the morph operation 				*/
  dtimage_t *input,
  dtimage_t *output,
  dtimage_t *expected,
  const char *f,
  const char *id,
  int *assert_fail)
{
  DT_F("dttmorph_test3x3_do");
  char t[32];
  long cmp;
  dt_rc_e rc = DT_RC_GOOD;

  DT_C(dtimage_constant,(ctl,			/* clear output image 				*/
    output, 9.0));						/* with marker value				*/
  
  if (op == DILATE)
	DT_C(dtimage_dilate3x3,(ctl,		/* perform dilation 				*/
      input, output))
  else									/* or 								*/
  if (op == ERODE)
	DT_C(dtimage_erode3x3,(ctl,			/* perform erosion 					*/
      input, output))
  else									/* or 								*/
  if (op == MEDIAN)
	DT_C(dtimage_median,(ctl,			/* perform median					*/
      input, 3, 3, output))

  DT_C(dtimage_cmp,(ctl,				/* compare expected to actual	    */
    output, expected, 0,
    &cmp, NULL, NULL));

  DT_C(dtntype_to_string,(ctl,
    input->ntype, t, sizeof(t)));
  *assert_fail +=
    DT_RC_GOOD != dt_assert(
      ctl, cmp == 0,
      f, "%s %s", id, t);

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
dttmorph_test3x3_one(
  dt_ctl_t *ctl,						/* environment control 				*/
  int op,								/* the morph operation 				*/
  dtxy_t ncols,							/* image size 						*/
  dtxy_t nrows,
  dt_ntype_e input_ntype,				/* input data array type 			*/
  void *input_data,						/* input data array 				*/
  dt_ntype_e operation_ntype,			/* operation pixel type 			*/
  dt_ntype_e expected_ntype,			/* expected data array type 		*/
  void *expected_data,					/* expected data array 				*/
  const char *f,
  const char *id,
  int *assert_fail)
{
  DT_F("dttmorph_test3x3_one");
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
    &input, ncols, nrows, 
    operation_ntype));
  
  DT_C(dtimage_assign_array,(ctl,		/* populate input image 			*/
    &input, ncols, nrows,
    input_ntype, input_data));
  
  DT_C(dtimage_create,(ctl,				/* create output image 				*/
    &output, ncols, nrows, 
    operation_ntype));
  
  DT_C(dtimage_create,(ctl,				/* create expected image 			*/
    &expected, ncols, nrows, 
    operation_ntype));
  
  DT_C(dtimage_assign_array,(ctl,		/* populate expected image 			*/
    &expected, ncols, nrows,
    expected_ntype, expected_data));

  DT_C(dttmorph_test3x3_do,(ctl,		/* do the test 						*/
    op, &input, &output, &expected,
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
dttmorph_test3x3_all(
  dt_ctl_t *ctl,						/* environment control 				*/
  int op,								/* the morph operation 				*/
  dtxy_t ncols,							/* image size 						*/
  dtxy_t nrows,
  dt_ntype_e input_ntype,				/* input data array type 			*/
  void *input_data,						/* input data array 				*/
  dt_ntype_e expected_ntype,			/* expected data array type 		*/
  void *expected_data,					/* expected data array 				*/
  const char *f,
  const char *id,
  int *assert_fail)
{
  DT_F("dttmorph_test3x3_all");
  int i;
  dt_rc_e rc;

  for (i=0; dt_ntype_list[i] != DTNTYPE_NULL; i++)
  {
	if (dt_ntype_list[i] == DTNTYPE_B01)
	  continue;
	DT_C(dttmorph_test3x3_one,(ctl, op,
      ncols, nrows, 
      input_ntype, input_data,
      dt_ntype_list[i],
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
dttmorph_3x3_small(
  dt_ctl_t *ctl,
  int *assert_fail)
{
  DT_F("dttmorph_3x3_small");
  int i;
  dt_rc_e rc;

#define NCOLS 7
#define NROWS 7
  static dt_ntype_b08_t input_data[NCOLS*NROWS] = {
    0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,
    0,0,1,1,1,0,0,
    0,0,1,1,1,0,0,
    0,0,1,1,1,0,0,
    0,0,0,0,0,0,0,
    0,0,0,0,0,0,0
  };
  static dt_ntype_b08_t erode_data[NCOLS*NROWS] = {
    9,9,9,9,9,9,9,
    9,0,0,0,0,0,9,
    9,0,0,0,0,0,9,
    9,0,0,1,0,0,9,
    9,0,0,0,0,0,9,
    9,0,0,0,0,0,9,
    9,9,9,9,9,9,9
  };
  static dt_ntype_b08_t dilate_data[NCOLS*NROWS] = {
    9,9,9,9,9,9,9,
    9,1,1,1,1,1,9,
    9,1,1,1,1,1,9,
    9,1,1,1,1,1,9,
    9,1,1,1,1,1,9,
    9,1,1,1,1,1,9,
    9,9,9,9,9,9,9
  };
  static dt_ntype_b08_t median_input_data[NCOLS*NROWS] = {
    0,0,0,0,0,0,0,
    0,1,2,3,0,0,0,
    0,4,5,6,0,0,0,
    0,7,8,9.0,0,0,
    0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,
    0,0,0,0,0,0,0
  };
  static dt_ntype_b08_t median_output_data[NCOLS*NROWS] = {
    9,9,9,9,9,9,9,
    9,0,2,0,0,0,9,
    9,2,5,3,0,0,9,
    9,0,5,0,0,0,9,
    9,0,0,0,0,0,9,
    9,0,0,0,0,0,9,
    9,9,9,9,9,9,9
  };

  DT_C(dttmorph_test3x3_all,(ctl, ERODE,
    NCOLS, NROWS, 
    DTNTYPE_B08, input_data,
    DTNTYPE_B08, erode_data,
    F, "erode", assert_fail));

  DT_C(dttmorph_test3x3_all,(ctl, DILATE,
    NCOLS, NROWS, 
    DTNTYPE_B08, input_data,
    DTNTYPE_B08, dilate_data,
    F, "dilate", assert_fail));

  DT_C(dttmorph_test3x3_all,(ctl, MEDIAN,
    NCOLS, NROWS, 
    DTNTYPE_B08, median_input_data,
    DTNTYPE_B08, median_output_data,
    F, "median", assert_fail));

cleanup:

#undef NCOLS
#undef NROWS
    
  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/
static
dt_rc_e
dttmorph_3x3_large_one(
  dt_ctl_t *ctl,
  int op,
  long max,								/* image size 						*/
  long n,								/* timing loops 					*/
  dtntype_e ntype,						/* desired data type 				*/
  const char *f,
  const char *id,
  int *assert_fail)
{
  DT_F("dttmorph_3x3_large");
  dtimage_t input;
  dtimage_t output;
  dtimage_t expected;
#define NCOLS (max)
#define NROWS (max)
  dtos_time_t mark1, mark2;
  dtxy_t x, y;
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
  i = 0;
  for (y=0; y<NROWS; y+=5)
    for (x=0; x<NCOLS; x+=5)
      DT_C(dtimage_set_pixel,(ctl,
        &input, x, y, i++));
  
  DT_C(dtimage_create,(ctl,				/* create output image 				*/
    &output, NCOLS, NROWS, 
    ntype));

  DT_C(dtos_time,(ctl, &mark1));
  for (i=0; i<n; i++)
  {
	if (op == DILATE)
      DT_C(dtimage_dilate3x3,(ctl,
        &input, &output))
    else
	if (op == ERODE)
      DT_C(dtimage_erode3x3,(ctl,
        &input, &output))
    else
	if (op == MEDIAN)
      DT_C(dtimage_median,(ctl,
        &input, 3, 3, &output))
  }
  DT_C(dtos_time,(ctl, &mark2));

  DT_C(dtntype_to_string,(ctl,
    input.ntype, t, sizeof(t)));
  DT_C(dtstr_printf,(ctl,
    s, sizeof(s), 
    "%s %3s [%dx%d] (%ld loops)", 
    id, t, input.xe, input.ye, n));
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
dttmorph_3x3_large(
  dt_ctl_t *ctl,
  long max,								/* image size 						*/
  long n,								/* timing loops 					*/
  int *assert_fail)
{
  DT_F("dttmorph_3x3_large");
  dt_rc_e rc = DT_RC_GOOD;

  DT_C(dttmorph_3x3_large_one,(ctl,
    DILATE,
    max, n, DTNTYPE_B08, 
    F, "dilate3x3", assert_fail));

  DT_C(dttmorph_3x3_large_one,(ctl,
    DILATE,
    max, n, DTNTYPE_B16, 
    F, "dilate3x3", assert_fail));

  DT_C(dttmorph_3x3_large_one,(ctl,
    ERODE,
    max, n, DTNTYPE_B08, 
    F, "erode3x3", assert_fail));

  DT_C(dttmorph_3x3_large_one,(ctl,
    ERODE,
    max, n, DTNTYPE_B16, 
    F, "erode3x3", assert_fail));

  DT_C(dttmorph_3x3_large_one,(ctl,
    MEDIAN,
    max, n, DTNTYPE_B08, 
    F, "median3x3", assert_fail));

  DT_C(dttmorph_3x3_large_one,(ctl,
    MEDIAN,
    max, n, DTNTYPE_B16, 
    F, "median3x3", assert_fail));
  
#if 0
  DT_C(dttmorph_3x3_large_one,(ctl,		/* do it on an F2 imagecommon 		*/
    DILATE,
    max, n, DTNTYPE_F2, 
    F, "dilate3x3", assert_fail));
#endif
  
cleanup:

  return rc;

#undef NCOLS
#undef NROWS
}


/*..........................................................................*/
dt_rc_e
DTCONFIG_API1
dttmorph_arg_check(						/* check args and params            */
  dt_ctl_t *ctl,
  void *app,
  dtparam_t *param,
  dt_enter_arg_t *arg,
  char *msg,
  const char **start)
{
  DT_F("dttmorph_arg_check");

  DT_Q(dtt_check_op,(ctl, param,        /* make sure op is understood       */
    ops, msg, start));

  return DT_RC_GOOD;
}

/*..........................................................................*/

DTT_MAIN(dttmorph)
{
  DT_F("dttmorph");
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
    NULL));

  arg[0].desc = NULL;					/* no arguments 					*/
  
  DT_C(dt_interactive_parse,(			/* parse command line interactively	*/
    argc, argv, F, USAGE, param,
    arg, dttmorph_arg_check, NULL));

  if (!strcmp(param->op, "all") ||
	  !strcmp(param->op, "fast"))
  {
	DT_C(dttmorph_3x3_small,(ctl, &assert_fail));
  }

  if (!strcmp(param->op, "time"))
  {
	DT_C(dttmorph_3x3_large,(ctl,
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
