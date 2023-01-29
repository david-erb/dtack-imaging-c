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

#include <dtack/t.h>
DT_RCSID("test $RCSfile: dttstat.c,v $ $Revision: 1.9 $");

#include <dtack/mem.h>

#define USAGE "[-Dnnnn] [-op all|fast|1|2|3|4|5|big|6|time"

static const char *ops[] = {"all",
  "fast", "big", "time", 
  "1", "2", "3", "4", "5", "6",
  NULL};

/*..........................................................................
 * check double result
 *..........................................................................*/

static
dt_rc_e
dttstat_double_check(
  dt_ctl_t *ctl,
  double got,
  double want,
  char *F,
  char *id,
  int *assert_fail)
{
  double normalize;
  double delta;
  normalize = fabs(want);				/* normalize result magnitude	    */
  if (normalize == 0.0)
    normalize = 1.0;
  delta = fabs(want-got) / normalize;	/* significant digit difference	    */
  *assert_fail += (DT_RC_GOOD !=
  dt_assert(ctl, delta < 1e-5,
    F, "%s got %e want %e (delta %1.1e)",
    id, got, want, got-want));
  return DT_RC_GOOD;
}

/*..........................................................................*/
static 
dt_rc_e
dttstat_simple(
  dt_ctl_t *ctl,
  dtxy_t ncols,
  dtxy_t nrows,
  dt_ntype_e ntype,
  void *data,
  char *f,
  int *assert_fail,
  double expect_sum,
  double expect_sumsq,
  double expect_mean,
  double expect_variance)
{
  DT_F("dttstat_simple");
  dtimage_t image;
  double sum, sumsq, mean, variance;
  char id[64];
  dtxy_t y;
  unsigned int rowsize = (unsigned int)
    (ncols * dt_ntype_sizeof[ntype]);

  sprintf(id, "%s %-3s", f,
    dt_ntype_string[ntype]);

  DT_Q(dtimage_create,(ctl,				/* make image to hold given data    */
    &image, ncols, nrows, ntype));
 
  for (y=0; y<nrows; y++)				/* copy given data row by row		*/
  {
    void *p;
    DTIMAGE_MAPROW(ctl, &image, y, &p);
    DT_MEMCPY(p, data, rowsize);
    DTIMAGE_PUTROW(ctl, &image, y);
    data = (char *)data + rowsize;
  }

  DT_Q(dtimage_sum,(ctl,				/* do sum						    */
    &image, &sum));
  dttstat_double_check(ctl,
    sum, expect_sum,
    id, "sum         ", assert_fail);

  DT_Q(dtimage_sumsq,(ctl,				/* do sum of squares			    */
    &image, &sumsq));
  dttstat_double_check(ctl,
    sumsq, expect_sumsq,
    id, "sumsq       ", assert_fail);

  DT_Q(dtimage_mean,(ctl,				/* do mean						    */
    &image, &mean));
  dttstat_double_check(ctl,
    mean, expect_mean,
    id, "mean(1)     ", assert_fail);

  DT_Q(dtimage_variance,(ctl,			/* do variance and mean			    */
    &image, &variance, &mean));
  dttstat_double_check(ctl,
    variance, expect_variance,
    id, "variance    ", assert_fail);
  dttstat_double_check(ctl,
    mean, expect_mean,
    id, "mean(2)     ", assert_fail);

  DT_Q(dtimage_free,(ctl, &image));

  return DT_RC_GOOD;
}

/*..........................................................................*/
static 
dt_rc_e
dttstat_dual(
  dt_ctl_t *ctl,
  dtxy_t ncols,
  dtxy_t nrows,
  dt_ntype_e ntype,
  void *data1,
  void *data2,
  char *f,
  int *assert_fail,
  double expect_sumsq_diffs)
{
  DT_F("dttstat_dual");
  dtimage_t image1, image2;
  double sumsq_diffs;
  char id[64];
  dtxy_t y;
  unsigned int rowsize = (unsigned int) 
    (ncols * dt_ntype_sizeof[ntype]);

  sprintf(id, "%s %-3s", f,
    dt_ntype_string[ntype]);

  DT_Q(dtimage_create,(ctl,				/* make image to hold given data    */
    &image1, ncols, nrows, ntype));
 
  for (y=0; y<nrows; y++)				/* copy given data row by row		*/
  {
    void *p;
    DTIMAGE_MAPROW(ctl, &image1, y, &p);
    DT_MEMCPY(p, data1, rowsize);
    DTIMAGE_PUTROW(ctl, &image1, y);
    data1 = (char *)data1 + rowsize;
  }

  DT_Q(dtimage_create,(ctl,				/* make image to hold given data    */
    &image2, ncols, nrows, ntype));
 
  for (y=0; y<nrows; y++)				/* copy given data row by row		*/
  {
    void *p;
    DTIMAGE_MAPROW(ctl, &image2, y, &p);
    DT_MEMCPY(p, data2, rowsize);
    DTIMAGE_PUTROW(ctl, &image2, y);
    data2 = (char *)data2 + rowsize;
  }

  DT_Q(dtimage_sumsq_diffs,(ctl,		/* do sum of squared diffs		    */
    &image1, &image2, &sumsq_diffs));
  dttstat_double_check(ctl,
    sumsq_diffs, expect_sumsq_diffs,
    id, "sumsq diffs ", assert_fail);

  DT_Q(dtimage_free,(ctl, &image1));
  DT_Q(dtimage_free,(ctl, &image2));

  return DT_RC_GOOD;
}

/*..........................................................................*/
static 
dt_rc_e
dttstat_image(
  dt_ctl_t *ctl,
  dtimage_t *image,
  char *f,
  int *assert_fail,
  double expect_sum,
  double expect_sumsq,
  double expect_mean,
  double expect_variance)
{
  DT_F("dttstat_image");
  double sum, sumsq, mean, variance;
  char id[64];

  sprintf(id, "%s %-3s", f,
    dt_ntype_string[image->ntype]);

  DT_Q(dtimage_sum,(ctl,				/* do sum						    */
    image, &sum));
  dttstat_double_check(ctl,
    sum, expect_sum,
    id, "sum         ", assert_fail);

  DT_Q(dtimage_sumsq,(ctl,				/* do sum of squares			    */
    image, &sumsq));
  dttstat_double_check(ctl,
    sumsq, expect_sumsq,
    id, "sumsq       ", assert_fail);

  DT_Q(dtimage_mean,(ctl,				/* do mean						    */
    image, &mean));
  dttstat_double_check(ctl,
    mean, expect_mean,
    id, "mean(1)     ", assert_fail);

  DT_Q(dtimage_variance,(ctl,			/* do variance and mean			    */
    image, &variance, &mean));
  dttstat_double_check(ctl,
    variance, expect_variance,
    id, "variance    ", assert_fail);
  dttstat_double_check(ctl,
    mean, expect_mean,
    id, "mean(2)     ", assert_fail);

  DT_Q(dtimage_free,(ctl, image));

  return DT_RC_GOOD;
}

/*..........................................................................*/
static 
dt_rc_e
dttstat_time(
  dt_ctl_t *ctl,
  dtimage_t *image,
  int reps,
  char *f,
  int *assert_fail,
  double expect_mean,
  double expect_variance)
{
  DT_F("dttstat_time");
  dtos_time_t mark1, mark2;
  double variance, mean;
  char id[64];
  int i;

  sprintf(id, "%s %-3s (%2d reps)", f,
    dt_ntype_string[image->ntype], reps);

  DT_Q(dtos_time,(ctl, &mark1));
  for (i=0; i<reps; i++)
  {
    DT_Q(dtimage_variance,(ctl,			/* do variance and mean			    */
      image, &variance, &mean));
  }
  DT_Q(dtos_time,(ctl, &mark2));

  dttstat_double_check(ctl,
    variance, expect_variance,
    id, "variance    ", assert_fail);
  dttstat_double_check(ctl,
    mean, expect_mean,
    id, "mean        ", assert_fail);

  DT_Q(dtt_time_report,(ctl,			/* report time					    */
    &mark1, &mark2, id));

  DT_Q(dtimage_free,(ctl, image));

  return DT_RC_GOOD;
}

/*..........................................................................*/
static
dt_rc_e
dttstat1(
  dt_ctl_t *ctl,
  int *assert_fail)
{
  DT_F("dttstat1");

#define NCOLS 3
#define NROWS 3
#define DATA(TYPE) \
  static TYPE data[NCOLS*NROWS] = { \
      1,   2,   3, \
    127, 128, 129, \
    253, 254, 255 \
  };
const double SUM = 1152.0;
const double SUMSQ = 242718.0;
const double MEAN = 128.0;
const double VARIANCE = 1.190775000000e+04;

  {
    DATA(dt_ntype_b08_t);
    DT_Q(dttstat_simple,(ctl,	
      NCOLS, NROWS, DT_NTYPE_B08,
      data, F, assert_fail,
      SUM, SUMSQ, MEAN, VARIANCE));
  }

  {
    DATA(dt_ntype_b16_t);
    DT_Q(dttstat_simple,(ctl,	
      NCOLS, NROWS, DT_NTYPE_B16,
      data, F, assert_fail,
      SUM, SUMSQ, MEAN, VARIANCE));
  }

  {
    DATA(dt_ntype_b32_t);
    DT_Q(dttstat_simple,(ctl,	
      NCOLS, NROWS, DT_NTYPE_B32,
      data, F, assert_fail,
      SUM, SUMSQ, MEAN, VARIANCE));
  }

  {
    DATA(dt_ntype_s2_t);
    DT_Q(dttstat_simple,(ctl,	
      NCOLS, NROWS, DT_NTYPE_S2,
      data, F, assert_fail,
      SUM, SUMSQ, MEAN, VARIANCE));
  }

  {
    DATA(dt_ntype_s4_t);
    DT_Q(dttstat_simple,(ctl,	
      NCOLS, NROWS, DT_NTYPE_S4,
      data, F, assert_fail,
      SUM, SUMSQ, MEAN, VARIANCE));
  }

  {
    DATA(dt_ntype_f1_t);
    DT_Q(dttstat_simple,(ctl,	
      NCOLS, NROWS, DT_NTYPE_F1,
      data, F, assert_fail,
      SUM, SUMSQ, MEAN, VARIANCE));
  }

  {
    DATA(dt_ntype_f2_t);
    DT_Q(dttstat_simple,(ctl,	
      NCOLS, NROWS, DT_NTYPE_F2,
      data, F, assert_fail,
      SUM, SUMSQ, MEAN, VARIANCE));
  }
    
  return DT_RC_GOOD;

#undef DATA
#undef NROWS
#undef NCOLS    
}

/*..........................................................................*/
static
dt_rc_e
dttstat2(
  dt_ctl_t *ctl,
  int *assert_fail)
{
  DT_F("dttstat2");

#define NCOLS 3
#define NROWS 3
#define DATA(TYPE) \
  static TYPE data[NCOLS*NROWS] = { \
   -128,  -2,   0, \
      1,   2,   3, \
    125, 126, 127 \
  };
const double SUM = 254.0;
const double SUMSQ = 64032.0;
const double MEAN = 2.822222222222e+01;
const double VARIANCE = 7.107944444444e+03;

  {
    DATA(dt_ntype_s1_t);
    DT_Q(dttstat_simple,(ctl,	
      NCOLS, NROWS, DT_NTYPE_S1,
      data, F, assert_fail,
      SUM, SUMSQ, MEAN, VARIANCE));
  }

  {
    DATA(dt_ntype_s2_t);
    DT_Q(dttstat_simple,(ctl,	
      NCOLS, NROWS, DT_NTYPE_S2,
      data, F, assert_fail,
      SUM, SUMSQ, MEAN, VARIANCE));
  }

  {
    DATA(dt_ntype_s4_t);
    DT_Q(dttstat_simple,(ctl,	
      NCOLS, NROWS, DT_NTYPE_S4,
      data, F, assert_fail,
      SUM, SUMSQ, MEAN, VARIANCE));
  }

  {
    DATA(dt_ntype_f1_t);
    DT_Q(dttstat_simple,(ctl,	
      NCOLS, NROWS, DT_NTYPE_F1,
      data, F, assert_fail,
      SUM, SUMSQ, MEAN, VARIANCE));
  }

  {
    DATA(dt_ntype_f2_t);
    DT_Q(dttstat_simple,(ctl,	
      NCOLS, NROWS, DT_NTYPE_F2,
      data, F, assert_fail,
      SUM, SUMSQ, MEAN, VARIANCE));
  }
    
  return DT_RC_GOOD;

#undef DATA
#undef NROWS
#undef NCOLS    
}

/*..........................................................................*/
static
dt_rc_e
dttstat3(
  dt_ctl_t *ctl,
  int *assert_fail)
{
  DT_F("dttstat3");

#define NCOLS 3
#define NROWS 3
#define DATA(TYPE) \
  static TYPE data1[NCOLS*NROWS] = { \
      0,   1,   2, \
    125, 126, 127, \
    253, 254, 255 \
  }; \
  static TYPE data2[NCOLS*NROWS] = { \
    255, 254, 253, \
    127, 126, 125, \
      3,   2,   1 \
  };

const double SUMSQ_DIFFS = 382563.0;

  {
    DATA(dt_ntype_b08_t);
    DT_Q(dttstat_dual,(ctl,	
      NCOLS, NROWS, DT_NTYPE_B08,
      data1, data2, F, assert_fail,
      SUMSQ_DIFFS));
  }

  {
    DATA(dt_ntype_b16_t);
    DT_Q(dttstat_dual,(ctl,	
      NCOLS, NROWS, DT_NTYPE_B16,
      data1, data2, F, assert_fail,
      SUMSQ_DIFFS));
  }

  {
    DATA(dt_ntype_b32_t);
    DT_Q(dttstat_dual,(ctl,	
      NCOLS, NROWS, DT_NTYPE_B32,
      data1, data2, F, assert_fail,
      SUMSQ_DIFFS));
  }

  {
    DATA(dt_ntype_s2_t);
    DT_Q(dttstat_dual,(ctl,	
      NCOLS, NROWS, DT_NTYPE_S2,
      data1, data2, F, assert_fail,
      SUMSQ_DIFFS));
  }

  {
    DATA(dt_ntype_s4_t);
    DT_Q(dttstat_dual,(ctl,	
      NCOLS, NROWS, DT_NTYPE_S4,
      data1, data2, F, assert_fail,
      SUMSQ_DIFFS));
  }

  {
    DATA(dt_ntype_f1_t);
    DT_Q(dttstat_dual,(ctl,	
      NCOLS, NROWS, DT_NTYPE_F1,
      data1, data2, F, assert_fail,
      SUMSQ_DIFFS));
  }

  {
    DATA(dt_ntype_f2_t);
    DT_Q(dttstat_dual,(ctl,	
      NCOLS, NROWS, DT_NTYPE_F2,
      data1, data2, F, assert_fail,
      SUMSQ_DIFFS));
  }
    
  return DT_RC_GOOD;

#undef DATA
#undef NROWS
#undef NCOLS    
}

/*..........................................................................*/
static
dt_rc_e
dttstat4(
  dt_ctl_t *ctl,
  int *assert_fail)
{
  DT_F("dttstat4");

#define NCOLS 3
#define NROWS 3
#define DATA(TYPE) \
  static TYPE data1[NCOLS*NROWS] = { \
   -128,-127,-126, \
      0,   1,   2, \
    125, 126, 127 \
  }; \
  static TYPE data2[NCOLS*NROWS] = { \
    127, 126, 125, \
      2,   1,   0, \
   -126,-127,-128 \
  };

const double SUMSQ_DIFFS = 384078.0;

  {
    DATA(dt_ntype_s1_t);
    DT_Q(dttstat_dual,(ctl,	
      NCOLS, NROWS, DT_NTYPE_S1,
      data1, data2, F, assert_fail,
      SUMSQ_DIFFS));
  }

  {
    DATA(dt_ntype_s2_t);
    DT_Q(dttstat_dual,(ctl,	
      NCOLS, NROWS, DT_NTYPE_S2,
      data1, data2, F, assert_fail,
      SUMSQ_DIFFS));
  }

  {
    DATA(dt_ntype_s4_t);
    DT_Q(dttstat_dual,(ctl,	
      NCOLS, NROWS, DT_NTYPE_S4,
      data1, data2, F, assert_fail,
      SUMSQ_DIFFS));
  }

  {
    DATA(dt_ntype_f1_t);
    DT_Q(dttstat_dual,(ctl,	
      NCOLS, NROWS, DT_NTYPE_F1,
      data1, data2, F, assert_fail,
      SUMSQ_DIFFS));
  }

  {
    DATA(dt_ntype_f2_t);
    DT_Q(dttstat_dual,(ctl,	
      NCOLS, NROWS, DT_NTYPE_F2,
      data1, data2, F, assert_fail,
      SUMSQ_DIFFS));
  }
    
  return DT_RC_GOOD;

#undef DATA
#undef NROWS
#undef NCOLS    
}

/*..........................................................................
 * 5: multi-page allocated images
 *..........................................................................*/
static
dt_rc_e
dttstat5(
  dt_ctl_t *ctl,
  int *assert_fail)
{
  DT_F("dttstat5");
  dtimage_t image;
  dtxy_t i;

#define NCOLS 100
#define NROWS 100
#define DATA(TYPE) \
  DT_Q(dtimage_create,(ctl, &image, NCOLS, NROWS, TYPE)); \
  DT_Q(dtimage_constant,(ctl, &image, 1.0)); \
  for (i=0; i<NROWS; i++) \
    DT_Q(dtimage_set_pixel,(ctl, &image, i, i, 2.0)) \

const double SUM = 10100.0;
const double SUMSQ = 10300.0;
const double MEAN = 1.01;
const double VARIANCE = 9.900990e-3;

  {
    DATA(DT_NTYPE_B08);
    DT_Q(dttstat_image,(ctl, &image,
      F, assert_fail,
      SUM, SUMSQ, MEAN, VARIANCE));
  }

  {
    DATA(DT_NTYPE_B16);
    DT_Q(dttstat_image,(ctl, &image,
      F, assert_fail,
      SUM, SUMSQ, MEAN, VARIANCE));
  }

  {
    DATA(DT_NTYPE_B32);
    DT_Q(dttstat_image,(ctl, &image,
      F, assert_fail,
      SUM, SUMSQ, MEAN, VARIANCE));
  }

  {
    DATA(DT_NTYPE_S2);
    DT_Q(dttstat_image,(ctl, &image,
      F, assert_fail,
      SUM, SUMSQ, MEAN, VARIANCE));
  }

  {
    DATA(DT_NTYPE_S4);
    DT_Q(dttstat_image,(ctl, &image,
      F, assert_fail,
      SUM, SUMSQ, MEAN, VARIANCE));
  }

  {
    DATA(DT_NTYPE_F1);
    DT_Q(dttstat_image,(ctl, &image,
      F, assert_fail,
      SUM, SUMSQ, MEAN, VARIANCE));
  }

  {
    DATA(DT_NTYPE_F2);
    DT_Q(dttstat_image,(ctl, &image,
      F, assert_fail,
      SUM, SUMSQ, MEAN, VARIANCE));
  }
    
  return DT_RC_GOOD;

#undef DATA
#undef NROWS
#undef NCOLS    
}

/*..........................................................................
 * 6: for timing
 *..........................................................................*/
static
dt_rc_e
dttstat6(
  dt_ctl_t *ctl,
  int *assert_fail)
{
  DT_F("dttstat6");
  dtimage_t image;
  dtxy_t i;

#define NCOLS 500
#define NROWS 500
#define DATA(TYPE) \
  DT_Q(dtimage_create,(ctl, &image, NCOLS, NROWS, TYPE)); \
  DT_Q(dtimage_constant,(ctl, &image, 1.0)); \
  for (i=0; i<NROWS; i++) \
    DT_Q(dtimage_set_pixel,(ctl, &image, i, i, 2.0)) \

const double VARIANCE = 1.996008e-03; 
const double MEAN = 1.002000e+00;

  {
    DATA(DT_NTYPE_B08);
    DT_Q(dttstat_time,(ctl, &image, 10,
      F, assert_fail,
      MEAN, VARIANCE));
  }

  {
    DATA(DT_NTYPE_B16);
    DT_Q(dttstat_time,(ctl, &image, 10,
      F, assert_fail,
      MEAN, VARIANCE));
  }

  {
    DATA(DT_NTYPE_B32);
    DT_Q(dttstat_time,(ctl, &image, 10,
      F, assert_fail,
      MEAN, VARIANCE));
  }

  {
    DATA(DT_NTYPE_S2);
    DT_Q(dttstat_time,(ctl, &image, 10,
      F, assert_fail,
      MEAN, VARIANCE));
  }

  {
    DATA(DT_NTYPE_S4);
    DT_Q(dttstat_time,(ctl, &image, 10,
      F, assert_fail,
      MEAN, VARIANCE));
  }

  {
    DATA(DT_NTYPE_F1);
    DT_Q(dttstat_time,(ctl, &image, 5,
      F, assert_fail,
      MEAN, VARIANCE));
  }

  {
    DATA(DT_NTYPE_F2);
    DT_Q(dttstat_time,(ctl, &image, 5,
      F, assert_fail,
      MEAN, VARIANCE));
  }
    
  return DT_RC_GOOD;

#undef DATA
#undef NROWS
#undef NCOLS    
}

/*..........................................................................*/

dt_rc_e
DTCONFIG_API1
dttstat_arg_check(						/* check args and params            */
  dt_ctl_t *ctl,
  void *appstruct,
  dtparam_t *param,
  dt_enter_arg_t *arg,
  char *msg,
  const char **start)
{
  DT_F("dttstat_arg_check");

  DT_Q(dtt_check_op,(ctl, param,        /* make sure op is understood       */
    ops, msg, start));

  return DT_RC_GOOD;
}

/*..........................................................................*/

DTT_TEST(dttstat)
{
  DT_F("dttstat");
  dt_ctl_t *ctl = &param->ctl;
  dt_enter_arg_t arg[1];
  int assert_fail = 0;
  dt_rc_e rc = DT_RC_GOOD;

  DT_GI(dt_heapmon_init,(ctl));         /* init memory checking             */

  DT_GI(dtparam_init,(param));          /* init param structure             */

  DT_GI(dtparam_set,(param, 0,          /* set default for params we want   */
    &param->op, NULL, NULL, "fast",
    &param->reps, NULL, NULL, "1",
    NULL));

  arg[0].desc = NULL;                   /* no arguments                     */
  
  DT_GI(dt_interactive_parse,(			/* parse command line interactively	*/
    argc, argv, F, USAGE, param,
    arg, dttstat_arg_check, NULL));
                                        /* ................................ */
  while (rc == DT_RC_GOOD)
  {
    long reps;
    for (reps=0;
         reps < param->reps &&
         rc == DT_RC_GOOD;
         reps++)
    {
        if (!strcmp(param->op, "all") ||
            !strcmp(param->op, "fast") ||
            !strcmp(param->op, "1"))
        {
          DT_GI(dttstat1,(ctl, &assert_fail));
        }
        if (!strcmp(param->op, "all") ||
            !strcmp(param->op, "fast") ||
            !strcmp(param->op, "2"))
        {
          DT_GI(dttstat2,(ctl, &assert_fail));
        }
        if (!strcmp(param->op, "all") ||
            !strcmp(param->op, "fast") ||
            !strcmp(param->op, "3"))
        {
          DT_GI(dttstat3,(ctl, &assert_fail));
        }
        if (!strcmp(param->op, "all") ||
            !strcmp(param->op, "fast") ||
            !strcmp(param->op, "4"))
        {
          DT_GI(dttstat4,(ctl, &assert_fail));
        }
        if (!strcmp(param->op, "all") ||
            !strcmp(param->op, "fast") ||
            !strcmp(param->op, "big") ||
            !strcmp(param->op, "5"))
        {
          DT_GI(dttstat5,(ctl, &assert_fail));
        }
        if (!strcmp(param->op, "all") ||
            !strcmp(param->op, "time") ||
            !strcmp(param->op, "6"))
        {
          DT_GI(dttstat6,(ctl, &assert_fail));
        }
	}

    DT_G(dt_interactive_after,(         /* make post display                */
      ctl, param, USAGE,
      arg, dttstat_arg_check, NULL,
      rc));
  }

                                        /* ................................ */

  DT_I(dt_interactive_leave,(			/* clean up before leaving			*/
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
