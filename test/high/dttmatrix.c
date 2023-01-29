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
  19990130 DE added to the suite of tests
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
DT_RCSID("test $RCSfile: dttmatrix.c,v $ $Revision: 1.7 $");

#include <dtack/point.h>				/* xyz points 						*/
#include <dtack/points.h>				/* lists of xyz points 				*/
#include <dtack/poly.h>					/* polynomial transformation		*/
#include <dtack/matrix.h>				/* matrix operations 				*/

#define USAGE "[-Dnnnn]"

#define BIG (128)

static const char *ops[] = {"all", "fast", "time", NULL};

/*..........................................................................
 *..........................................................................*/
static 
dt_rc_e
dttmatrix_test_do(
  dt_ctl_t *ctl,						/* environment control 				*/
  dtimage_t *input,
  dtimage_t *expected,
  const char *f,
  const char *id,
  int *assert_fail)
{
  DT_F("dttmatrix_test_do");
  char t[32];
  dt1d_i_t *indices = NULL;
  double d;
  int singular;
  long cmp;
  dtxy_t x, y;
  dt_rc_e rc = DT_RC_GOOD;

  DT_C(dtos_malloc2,(ctl,
    (void **)&indices, 
    input->xe*sizeof(*indices),
    F, "indices"));
  DT_C(dtimage_lu_decompose,(ctl,		/* make the resample output		    */
    input, indices, &d, &singular));

  DT_C(dtimage_cmp,(ctl,				/* compare expected to actual	    */
    input, expected, 0.00001,
    &cmp, &x, &y));

  DT_C(dtntype_to_string,(ctl,
    input->ntype, t, sizeof(t)));
  *assert_fail +=
    DT_RC_GOOD != dt_assert(
      ctl, cmp == 0,
      f, "%s %s", id, t);

  if (DT_DBG(ctl, DT_DBG_MASK_PIXEL) &&
	  cmp != 0)
  {
	double vin, vout;
	DT_C(dtimage_get_pixel,(ctl, input, x, y, &vin));
	DT_C(dtimage_get_pixel,(ctl, expected, x, y, &vout));
    dt_dbg(ctl, F, DT_DBG_MASK_PIXEL,
      "image differs at [%d,%d] input=%g expected=%g delta=%g", 
      x, y, vin, vout, fabs(vin-vout));
  }

  if (cmp != 0 && input->xe < 20)		/* appears to be a problem? 		*/
  {
    DT_C(dtimage_dbg_values,(ctl,
      input, NULL, DT_DBG_MASK_PIXEL));
    DT_C(dtimage_dbg_values,(ctl,
      expected, NULL, DT_DBG_MASK_PIXEL));
  }

cleanup:
  if (indices)
	DT_I(dtos_free2,(ctl, indices,
      F, "indices"));
  return rc;
}

/*..........................................................................
 *..........................................................................*/
static 
dt_rc_e
dttmatrix_test_one(
  dt_ctl_t *ctl,						/* environment control 				*/
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
  DT_F("dttmatrix_test_one");
  dtimage_t input;
  dtimage_t expected;
  char t[32];
  unsigned long cmp;
  dt_rc_e rc = DT_RC_GOOD;

  input.xe = 0;
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

  DT_C(dttmatrix_test_do,(ctl,		/* do the test 						*/
    &input, &expected,
    f, id, assert_fail));
  
cleanup:
  DT_I(dtimage_free,(ctl, &expected));
  DT_I(dtimage_free,(ctl, &input));

  return rc;
}

/*..........................................................................
 * convolve very simple input image with a number of kernels
 *..........................................................................*/
static
dt_rc_e
dttmatrix_test_all(
  dt_ctl_t *ctl,						/* environment control 				*/
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
  DT_F("dttmatrix_test_all");
  int i;
  dt_rc_e rc;

  for (i=0; dt_ntype_list[i] != DTNTYPE_NULL; i++)
  {
	if (dt_ntype_list[i] == DTNTYPE_B01)
	  continue;
	if (dt_ntype_list[i] != DTNTYPE_F2)
	  continue;
	DT_C(dttmatrix_test_one,(ctl, 
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
 *..........................................................................*/
static
dt_rc_e
dttmatrix_small(
  dt_ctl_t *ctl,
  int *assert_fail)
{
  DT_F("dttmatrix_small");
  int i;
  dt_rc_e rc;

#define NCOLS 8
#define NROWS 8
  static dt_ntype_f2_t input_data[NCOLS*NROWS] = {
	100, 100, 100, 100,	100, 100, 100, 100,
	100, 100, 100, 100,	100, 100, 100, 100,
	100, 100, 100, 100,	100, 100, 100, 100,
	100, 100, 100, 100,	100, 100, 100, 100,
	100, 100, 100, 100,	100, 100, 100, 100,
	100, 100, 100, 100,	100, 100, 100, 100,
	100, 100, 100, 100,	100, 100, 100, 100,
	100, 100, 100, 100,	100, 100, 100, 100
  };
  static dt_ntype_f2_t expected_data[NCOLS*NROWS] = {
    1.0e+02, 1.0e+02, 1.0e+02, 1.0e+02, 1.0e+02, 1.0e+02, 1.0e+02, 1.0e+02,
    1.0e+00, 1.0e-20, 0.0e+00, 0.0e+00, 0.0e+00, 0.0e+00, 0.0e+00, 0.0e+00,
    1.0e+00, 0.0e+00, 1.0e-20, 0.0e+00, 0.0e+00, 0.0e+00, 0.0e+00, 0.0e+00,
    1.0e+00, 0.0e+00, 0.0e+00, 1.0e-20, 0.0e+00, 0.0e+00, 0.0e+00, 0.0e+00,
    1.0e+00, 0.0e+00, 0.0e+00, 0.0e+00, 1.0e-20, 0.0e+00, 0.0e+00, 0.0e+00,
    1.0e+00, 0.0e+00, 0.0e+00, 0.0e+00, 0.0e+00, 1.0e-20, 0.0e+00, 0.0e+00,
    1.0e+00, 0.0e+00, 0.0e+00, 0.0e+00, 0.0e+00, 0.0e+00, 1.0e-20, 0.0e+00,
    1.0e+00, 0.0e+00, 0.0e+00, 0.0e+00, 0.0e+00, 0.0e+00, 0.0e+00, 1.0e-20
  };

  DT_C(dttmatrix_test_all,(ctl, 
    NCOLS, NROWS, 
    DTNTYPE_F2, input_data,
    NCOLS, NROWS, 
    DTNTYPE_F2, expected_data,
    F, "small", assert_fail));

cleanup:

#undef NCOLS
#undef NROWS
    
  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/
static
dt_rc_e
dttmatrix_poly_one(
  dt_ctl_t *ctl,
  dtpoint_2d_t *corners1,
  dtpoint_2d_t *corners2,
  dtpoint_2d_t *points,
  dtpoint_2d_t *expected,
  int n,
  const char *id,
  int *assert_fail)
{
  DT_F("dttmatrix_poly_one");
# define N 8
  dtpoint_2d_t c1[N];
  dtpoint_2d_t c2[N];
  dtpoint_2d_t *p2 = NULL;
  dt1d_i_t i;
  dtpoly_2_t poly;
  int cmp;
  dt1d_i_t k;
#define EPSILON (1e-10)
  dt_rc_e rc = DT_RC_GOOD;

  c1[0].x = corners1[0].x;				/* source space 					*/
  c1[0].y = corners1[0].y;
  c1[2].x = corners1[1].x;
  c1[2].y = corners1[1].y;
  c1[4].x = corners1[2].x;
  c1[4].y = corners1[2].y;
  c1[6].x = corners1[3].x;
  c1[6].y = corners1[3].y;
			
  c2[0].x = corners2[0].x;				/* destination space 				*/
  c2[0].y = corners2[0].y;
  c2[2].x = corners2[1].x;
  c2[2].y = corners2[1].y;
  c2[4].x = corners2[2].x;
  c2[4].y = corners2[2].y;
  c2[6].x = corners2[3].x;
  c2[6].y = corners2[3].y;

  for (i=1; i<N; i+=2)
  {
	c1[i].x = (c1[i-1].x + c1[(i+1)%N].x) / 2.0;
	c1[i].y = (c1[i-1].y + c1[(i+1)%N].y) / 2.0;
	c2[i].x = (c2[i-1].x + c2[(i+1)%N].x) / 2.0;
	c2[i].y = (c2[i-1].y + c2[(i+1)%N].y) / 2.0;
  }

  DT_C(dtos_malloc2,(ctl,
    (void **)&p2, n * sizeof(*p2),
    F, "p2"));
  DT_C(dtpoly_2ident,(ctl, &poly));
  DT_C(dtpoly_2mate,(ctl, &poly, c2, c1));

  if (DT_DBG(ctl, DT_DBG_MASK_PIXEL))
	DT_C(dtpoly_2printf,(ctl, &poly, stdout, ""));
  
  DT_C(dtpoly_2xform,(ctl, &poly, points, n, p2));

  DT_C(dtpoints_2dcompare,(ctl, p2, expected, n, EPSILON, &cmp, &k));
  if (cmp)
    *assert_fail +=
      DT_RC_GOOD != dt_assert(
        ctl, 0,
        F, "%s index %d got [%g %g] but expected [%g %g] epsilon=%g",
        id, k, p2[k].x, p2[k].y, expected[k].x, expected[k].y, EPSILON);
  else
	dt_assert(ctl, 1, F, "%s all points correct", id);

cleanup:

  if (p2)
	DT_I(dtos_free2,(ctl, p2, F, "p2"));

#undef N
    
  return rc;
}

/*..........................................................................
 *..........................................................................*/
static
dt_rc_e
dttmatrix_poly(
  dt_ctl_t *ctl,
  int *assert_fail)
{
  DT_F("dttmatrix_poly");
# define M 5
  dtpoint_2d_t points[] = {{0, 0}, {0, 1}, {1, 1}, {2, 2}, {2, 0}};
  dt_rc_e rc = DT_RC_GOOD;

										/* ................................ */
  {
	dtpoint_2d_t corners1[] = {{0, 0}, {2, 0}, {2, 2}, {0, 2}};
	dtpoint_2d_t corners2[] = {{0, 0}, {2, 0}, {2, 2}, {0, 2}};
	dtpoint_2d_t expected[] = {{0, 0}, {0, 1}, {1, 1}, {2, 2}, {2, 0}};

	DT_C(dttmatrix_poly_one,(ctl,
      corners1, corners2,
      points, expected,
      sizeof(points)/sizeof(*points),
      "1x", assert_fail));
  }
										/* ................................ */
  {
	dtpoint_2d_t corners1[] = {{0, 0}, {2, 0}, {2, 2}, {0, 2}};
	dtpoint_2d_t corners2[] = {{0, 0}, {4, 0}, {4, 4}, {0, 4}};
	dtpoint_2d_t expected[] = {{0, 0}, {0, 2}, {2, 2}, {4, 4}, {4, 0}};

	DT_C(dttmatrix_poly_one,(ctl,
      corners1, corners2,
      points, expected,
      sizeof(points)/sizeof(*points),
      "2x", assert_fail));
  }
										/* ................................ */
  {
	dtpoint_2d_t corners1[] = {{0, 0}, {2, 0}, {2, 2}, {0, 2}};
	dtpoint_2d_t corners2[] = {{0, 0}, {0, 2}, {2, 2}, {2, 0}};
	dtpoint_2d_t expected[] = {{0, 0}, {1, 0}, {1, 1}, {2, 2}, {0, 2}};

	DT_C(dttmatrix_poly_one,(ctl,
      corners1, corners2,
      points, expected,
      sizeof(points)/sizeof(*points),
      "transpose", assert_fail));
  }
										/* ................................ */
  {
	dtpoint_2d_t corners1[] = {{0, 0}, {2, 0}, {2, 2}, {0, 2}};
	dtpoint_2d_t corners2[] = {{0, 0}, {-2, 0}, {-2, 2}, {0, 2}};
	dtpoint_2d_t expected[] = {{0, 0}, {0, 1}, {-1, 1}, {-2, 2}, {-2, 0}};

	DT_C(dttmatrix_poly_one,(ctl,
      corners1, corners2,
      points, expected,
      sizeof(points)/sizeof(*points),
      "mirror x", assert_fail));
  }

cleanup:
    
  return rc;
}

/*..........................................................................
 *..........................................................................*/
static
dt_rc_e
dttmatrix_large_one(
  dt_ctl_t *ctl,
  long max,								/* image size 						*/
  long n,								/* timing loops 					*/
  dtntype_e ntype,						/* desired data type 				*/
  const char *f,
  const char *id,
  int *assert_fail)
{
  DT_F("dttmatrix_large");
  dtimage_t input;
  dtimage_t expected;
  dt1d_i_t *indices = NULL;
  double d;
  int singular;
#define NCOLS (max)
#define NROWS (max)
  dtos_time_t mark1, mark2;
  long i;
  char t[32];
  char s[128];
  dt_rc_e rc = DT_RC_GOOD;

  input.xe = 0;
  
  DT_C(dtimage_create,(ctl,				/* create input image 				*/
    &input, NCOLS, NROWS, 
    ntype));
  
  DT_C(dtimage_constant,(ctl,			/* populate input image 			*/
    &input, 0.0));

  DT_C(dtos_malloc2,(ctl,
    (void **)&indices, input.xe,
    F, "indices"));

  DT_C(dtos_time,(ctl, &mark1));
  for (i=0; i<n; i++)
  {
	DT_C(dtimage_lu_decompose,(ctl,
      &input, indices, &d, &singular));
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
  if (indices)
	DT_I(dtos_free2,(ctl, indices,
      F, "indices"));
  DT_I(dtimage_free,(ctl, &input));

  return rc;

#undef NCOLS
#undef NROWS
}

/*..........................................................................
 *..........................................................................*/
static
dt_rc_e
dttmatrix_large(
  dt_ctl_t *ctl,
  long max,								/* image size 						*/
  long n,								/* timing loops 					*/
  int *assert_fail)
{
  DT_F("dttmatrix_large");
  dt_rc_e rc = DT_RC_GOOD;

  DT_C(dttmatrix_large_one,(ctl,
    max, n, DTNTYPE_F2, 
    F, "large", assert_fail));
  
cleanup:

  return rc;

#undef NCOLS
#undef NROWS
}


/*..........................................................................*/
dt_rc_e
DTCONFIG_API1
dttmatrix_arg_check(						/* check args and params            */
  dt_ctl_t *ctl,
  void *app,
  dtparam_t *param,
  dt_enter_arg_t *arg,
  char *msg,
  const char **start)
{
  DT_F("dttmatrix_arg_check");

  DT_Q(dtt_check_op,(ctl, param,        /* make sure op is understood       */
    ops, msg, start));

  return DT_RC_GOOD;
}

/*..........................................................................*/

DTT_TEST(dttmatrix)
{
  DT_F("dttmatrix");
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
    arg, dttmatrix_arg_check, NULL));

  if (!strcmp(param->op, "all") ||
	  !strcmp(param->op, "fast"))
  {
	DT_C(dttmatrix_small,(ctl, &assert_fail));
	DT_C(dttmatrix_poly,(ctl, &assert_fail));
  }

  if (!strcmp(param->op, "time"))
  {
	DT_C(dttmatrix_large,(ctl,
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
