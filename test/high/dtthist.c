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
DT_RCSID("test $RCSfile: dtthist.c,v $ $Revision: 1.9 $");

#define USAGE "[-Dnnnn]"

static const char *ops[] = {
  "all",
  "fast",
  "time",
  NULL};

/*..........................................................................*/
dt_rc_e
DTCONFIG_API1
dtthist_print_hist_diff(
  dt_ctl_t *ctl,
  dtntype_b32_t *got,
  dtntype_b32_t *expected,
  unsigned int n,
  long diff)
{
  int bad = DT_CAST(int, (diff < 0? -diff: diff) - 1);
  unsigned int beg = bad < 5? 0: bad-5;
  unsigned int end = (bad+5) > (int)n? n: bad+5;
  unsigned int i;
  for (i=beg; i<end; i++)
    dt_dbg(ctl, NULL, DT_DBG_MASK_ALL,
      "  %s%3u. got %6d (expected %6d)",
      (long)i == bad? "->": "  ",
      i, got[i], expected[i]);
  return DT_RC_GOOD;
}

/*..........................................................................*/
dt_rc_e
DTCONFIG_API1
dtthist_print_minmax_diff(
  dt_ctl_t *ctl,
  double *mingot,
  double *maxgot,
  double *minexpected,
  double *maxexpected,
  unsigned int n,
  long mindiff,
  long maxdiff)
{
  long minbad = (mindiff < 0? -mindiff: mindiff) - 1;
  long maxbad = (maxdiff < 0? -maxdiff: maxdiff) - 1;
  unsigned int beg = 0;
  unsigned int end = n;
  unsigned int i;
  for (i=beg; i<end; i++)
    dt_dbg(ctl, NULL, DT_DBG_MASK_ALL,
      "  %s%3u. got %g to %g (expected %g to %g)",
      (long)i == minbad || (long)i == maxbad? "->": "  ",
      i, mingot[i], maxgot[i], 
      minexpected[i], maxexpected[i]);
  return DT_RC_GOOD;
}

/*..........................................................................*/
dt_rc_e
DTCONFIG_API1
dtthist_freq_check(
  dt_ctl_t *ctl,
  void *input_data,
  unsigned int input_ndata,
  dt_ntype_e input_ntype,
  dt_ntype_e process_ntype,
  dthist_freq2_t *expect_hist_data,
  unsigned int width,
  double min,
  double max,  
  const char *f,
  int *assert_fail)
{
  DT_F("dtthist_freq_check");
  void *process_data = NULL;
  dthist_t hist;
  long cmp;
  dt_rc_e rc;

  hist.freq = NULL;

  DT_C(dtos_malloc2,(ctl,				/* get space for process data 		*/
    (void **)&process_data,
    DT_CAST(unsigned int,
      input_ndata * 
      dt_ntype_sizeof[process_ntype]),
    F, "process_data"));
  DT_C_1D(dt1d_cast,(						/* convert input to process data 	*/
    input_data, 
    input_ndata, input_ntype,
    process_data, 
    input_ndata, process_ntype));
  DT_C(dthist_alloc,(ctl, &hist,		/* allocate histogram 				*/
    width, min, max));
  DT_C(dthist_clear,(ctl, &hist));		/* clear histogram 					*/
  DT_C(dthist_add_values,(ctl, &hist,	/* accumulate values into histogram */
    process_data, 
    0, input_ndata, process_ntype));
  DT_C_1D(dt1d_cmp,(					/* check histogram values 			*/
    DTNTYPE_B32,
    expect_hist_data, width,
    (dtntype_b32_t *)hist.freq, width, 0, &cmp));

  if (DT_DBG(ctl, DT_DBG_MASK_PIXEL)) 
	DT_Q(dtthist_print_hist_diff,(ctl,	/* print stuff out 					*/
      (dtntype_b32_t *)hist.freq, 
      expect_hist_data,
      width, cmp));
  
  if (DT_RC_GOOD != dt_assert(ctl, 
      cmp == 0, f,
      "%s (%g to %g) compare %ld (0)", 
      dt_ntype_string[process_ntype],
      min, max, cmp))
  {
    *assert_fail = *assert_fail + 1;
  }

cleanup:
  if (hist.freq != NULL)
    DT_I(dthist_free,(ctl, &hist));
  if (process_data != NULL)
    DT_I(dtos_free2,(ctl,
      process_data,
      F, "process_data"));
  return rc;
}
/*..........................................................................*/
dt_rc_e
DTCONFIG_API1
dtthist_freq(
  dt_ctl_t *ctl,
  int *assert_fail)
{
  DT_F("dtthist_freq");

#define ASIZE(A) (sizeof(A)/sizeof(*(A)))
#define CHECK(INPUT_NTYPE, PROCESS_NTYPE, MIN, MAX) \
    DT_Q(dtthist_freq_check,(ctl, \
      input_data, ASIZE(input_data), \
      INPUT_NTYPE, PROCESS_NTYPE, \
      hist_expect, ASIZE(hist_expect), \
	  MIN, MAX, \
      F, assert_fail));
				  
										/* ................................ */
  {
    static dt_ntype_b08_t input_data[] = {
      0, 1, 127, 128, 
      0, 255, 254, 255};
    static dthist_freq2_t hist_expect[256] = {0};
    hist_expect[0] = 2;
    hist_expect[1] = 1;
    hist_expect[127] = 1;
    hist_expect[128] = 1;
    hist_expect[254] = 1;
    hist_expect[255] = 2;
    CHECK(DT_NTYPE_B08, DT_NTYPE_B08, 0, 256);
    CHECK(DT_NTYPE_B08, DT_NTYPE_B16, 0, 256);
    CHECK(DT_NTYPE_B08, DT_NTYPE_B32, 0, 256);
    CHECK(DT_NTYPE_B08, DT_NTYPE_S2,  0, 256);
    CHECK(DT_NTYPE_B08, DT_NTYPE_F2,  0, 256);
  }
				  
										/* ................................ */
  {
    static dt_ntype_f2_t input_data[] = {
      0.0, 0.1, 0.2, 0.4, 
      0.0, 1.0, 0.2, 1.0};
    static dthist_freq2_t hist_expect[10] = {0};
    hist_expect[0] = 2;
    hist_expect[1] = 1;
    hist_expect[2] = 2;
    hist_expect[4] = 1;
    hist_expect[9] = 2;
    CHECK(DT_NTYPE_F2, DT_NTYPE_F2, 0, 1);
  }
										/* ................................ */
  return DT_RC_GOOD;
}

/*..........................................................................*/
dt_rc_e
DTCONFIG_API1
dtthist_bins_check(
  dt_ctl_t *ctl,
  unsigned int width,
  double min,
  double max,  
  dthist_bin_t *bins,
  double *expect_mins,
  double *expect_maxs,
  int nexpect,
  const char *f,
  int *assert_fail)
{
  DT_F("dtthist_bins_check");
  dthist_t hist;
# define NEXPECT (32)
  double mins[NEXPECT];
  double maxs[NEXPECT];
  int i;
  long mincmp, maxcmp;
  dt_rc_e rc;

  hist.freq = NULL;
  DT_C(dthist_alloc,(ctl, &hist,		/* allocate histogram 				*/
    width, min, max));

  nexpect = DT_MIN(nexpect, NEXPECT);
  for (i=0; i<nexpect; i++)				/* fetch range for bins 			*/
	DT_Q(dthist_bin_to_value,(ctl,
      &hist, bins[i],
      &mins[i], &maxs[i]));

  DT_C_1D(dt1d_cmp,(
    DTNTYPE_F2,
    expect_mins, nexpect,
    mins, nexpect, 0, &mincmp));
  DT_C_1D(dt1d_cmp,(
	DTNTYPE_F2,
    expect_maxs, nexpect,
    maxs, nexpect, 0, &maxcmp));

  if (DT_DBG(ctl, DT_DBG_MASK_PIXEL)) 
    DT_Q(dtthist_print_minmax_diff,(ctl,
      mins, maxs,
      expect_mins, expect_maxs,
      nexpect, mincmp, maxcmp));

  if (DT_RC_GOOD != dt_assert(ctl, 
      mincmp == 0 && maxcmp == 0, f,
      "%u (%g to %g) compare %ld to %ld (0-0)", 
      width,
      min, max, mincmp, maxcmp))
  {
    *assert_fail = *assert_fail + 1;
  }

cleanup:
  if (hist.freq != NULL)
    DT_I(dthist_free,(ctl, &hist));
  return rc;
}
/*..........................................................................*/
dt_rc_e
DTCONFIG_API1
dtthist_bins(
  dt_ctl_t *ctl,
  int *assert_fail)
{
  DT_F("dtthist_bins");
										/* ................................ */
  {
	static dthist_bin_t  bin[] = {0, 1, 254, 255};
	static double        min[] = {0, 1, 254, 255};
	static double        max[] = {1, 2, 255, 256};
	DT_Q(dtthist_bins_check,(ctl, 256, 0, 256, bin, min, max, ASIZE(bin), 
      F, assert_fail));
  }
										/* ................................ */
  {
	static dthist_bin_t  bin[] = { 0,  1, 5, 9, 10};
	static double        min[] = {-5, -4, 0, 4,  5};
	static double        max[] = {-4, -3, 1, 5,  6};
	DT_Q(dtthist_bins_check,(ctl, 10, -5, 5, bin, min, max, ASIZE(bin), 
      F, assert_fail));
  }

										/* ................................ */
  return DT_RC_GOOD;
}

/*..........................................................................*/
dt_rc_e
DTCONFIG_API1
dtthist_image_check(
  dt_ctl_t *ctl,
  dtimage_t *input,
  dt_ntype_e process_ntype,
  dthist_freq2_t *expect_hist_data,
  unsigned int width,
  double min,
  double max,  
  long n,
  const char *f,
  int *assert_fail)
{
  DT_F("dtthist_image_check");
  dtimage_t process;
  dthist_t hist;
  long cmp;
  long i;
  dtos_time_t mark1, mark2;
  char tmp[80];
  dt_rc_e rc;

  process.xe = 0;
  hist.freq = NULL;

  DT_C(dtimage_create,(ctl,				/* get space for process data 		*/
    &process, input->xe, input->ye,
    process_ntype));
  DT_C(dtimage_cast,(ctl,				/* convert input to process data 	*/
    input, &process));
  DT_C(dthist_alloc,(ctl, &hist,		/* allocate histogram 				*/
    width, min, max));
  DT_C(dthist_clear,(ctl, &hist));		/* clear histogram 					*/

  DT_C(dtos_time,(ctl, &mark1));
  for (i=0; i<n; i++)
  {
    DT_C(dtimage_hist,(ctl, &process,	/* accumulate values into histogram */
      &hist));
  }
  DT_C(dtos_time,(ctl, &mark2));
  DT_C(dtstr_printf,(ctl,
    tmp, sizeof(tmp),
    "%s %u (%g to %g) n=%ld",
    dt_ntype_string[process_ntype],
    width, min, max, n));
  if (DT_DBG(ctl, DT_DBG_MASK_TIME)) 
    DT_C(dtt_time_report,(ctl,			/* report time					    */
      &mark1, &mark2, tmp));

  DT_C_1D(dt1d_cmp,(					/* check histogram values 			*/
    DTNTYPE_B32,
    expect_hist_data, width,
    (dtntype_b32_t *)hist.freq, width, 0, &cmp));

  if (DT_DBG(ctl, DT_DBG_MASK_PIXEL))
  {
	DT_Q(dtthist_print_hist_diff,(ctl,	/* print stuff out 					*/
      (dtntype_b32_t *)hist.freq, expect_hist_data,
      width, cmp));
  }
  
  if (DT_RC_GOOD != dt_assert(ctl, 
      cmp == 0, f,
      "%s (%g to %g) compare %ld (0)", 
      dt_ntype_string[process_ntype],
      min, max, cmp))
  {
    *assert_fail = *assert_fail + 1;
  }

cleanup:
  if (hist.freq != NULL)
    DT_I(dthist_free,(ctl, &hist));
  if (process.xe != 0)
    DT_I(dtimage_free,(ctl,
      &process));
  return rc;
}
/*..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtthist_image(
  dt_ctl_t *ctl,
  dtxy_t w,
  dtxy_t h,
  long n,
  int *assert_fail)
{
  DT_F("dtthist_image");
  dtimage_t input;
  dt_rc_e rc;
  input.xe = 0;

#undef CHECK
#define CHECK(INPUT_NTYPE, PROCESS_NTYPE, MIN, MAX) \
    DT_C(dtthist_image_check,(ctl, \
      &input, PROCESS_NTYPE, \
      hist_expect, ASIZE(hist_expect), \
	  MIN, MAX, \
      n, F, assert_fail));
				  
										/* ................................ */
  {
    static dthist_freq2_t hist_expect[256] = {0};
	DT_C(dtimage_create,(ctl, 
      &input, w, h, DT_NTYPE_B08));
	DT_C(dtimage_constant,(ctl,
      &input, 3.0));
    hist_expect[3] = (unsigned long)input.xe * (unsigned long)input.ye;
    CHECK(DT_NTYPE_B08, DT_NTYPE_B08, 0, 256);
    CHECK(DT_NTYPE_B08, DT_NTYPE_B16, 0, 256);
    CHECK(DT_NTYPE_B08, DT_NTYPE_B32, 0, 256);
    CHECK(DT_NTYPE_B08, DT_NTYPE_S2,  0, 256);
    CHECK(DT_NTYPE_B08, DT_NTYPE_F2,  0, 256);
	DT_Q(dtimage_free,(ctl, &input));
	input.xe = 0;
  }
										/* ................................ */
  {
    static dthist_freq2_t hist_expect[2] = {0};
	DT_C(dtimage_create,(ctl, 
      &input, w, h, DT_NTYPE_F2));
	DT_C(dtimage_constant,(ctl,
      &input, -0.1));
    hist_expect[1] = (unsigned long)input.xe * (unsigned long)input.ye;
    CHECK(DT_NTYPE_B08, DT_NTYPE_B08,  -0.3, 0.0);
    CHECK(DT_NTYPE_B08, DT_NTYPE_F2,   -0.3, 0.0);
	DT_Q(dtimage_free,(ctl, &input));
	input.xe = 0;
  }

										/* ................................ */
cleanup:
  if (input.xe != 0)
    DT_C(dtimage_free,(ctl, &input));
  return rc;
}

/*..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtthist_arg_check(						/* check args and params            */
  dt_ctl_t *ctl,
  void *app,
  dtparam_t *param,
  dt_enter_arg_t *arg,
  char *msg,
  const char **start)
{
  DT_F("dtthist_arg_check");

  DT_Q(dtt_check_op,(ctl, param,        /* make sure op is understood       */
    ops, msg, start));

  return DT_RC_GOOD;
}

/*..........................................................................*/

DTT_TEST(dtthist)
{
  DT_F("dtthist");
  dt_ctl_t *ctl = &param->ctl;
  dt_enter_arg_t arg[1];
  int assert_fail = 0;
  dt_rc_e rc;

  DT_C(dt_heapmon_init,(ctl));			/* init memory checking             */

  DT_C(dtparam_init,(param));			/* init param structure             */

  DT_C(dtparam_set,(param, 0,			/* set default for params we want   */
    &param->op, NULL, NULL, "fast",
    &param->n, NULL, NULL, "5",
    &param->w, NULL, NULL, "512",
    &param->h, NULL, NULL, "512",
    &param->reps, NULL, NULL, "1",
    NULL));

  arg[0].desc = NULL;                   /* no arguments                     */
  DT_C(dt_enter_parse,(					/* go parse params and args			*/
    argc, argv, F, USAGE, param,
    arg, dtthist_arg_check, NULL));
                                        /* ................................ */
  {
    long reps;
    for (reps=0;
         reps < param->reps &&
         rc == DT_RC_GOOD;
         reps++)
    {
      if (!strcmp(param->op, "all") ||
          !strcmp(param->op, "fast"))
      {
		DT_C(dtthist_freq,(ctl, &assert_fail));
		DT_C(dtthist_bins,(ctl, &assert_fail));
		DT_C(dtthist_image,(ctl, 128, 128, 1, &assert_fail));
	  }
	  else
      if (!strcmp(param->op, "time"))
      {
		DT_C(dtthist_image,(ctl, 
          DT_CAST(dtxy_t, param->w),
          DT_CAST(dtxy_t, param->h),
          param->n, &assert_fail));
	  }
	}
  }
                                        /* ................................ */
cleanup:
  DT_I(dt_leave,(param));				/* clean up before leaving			*/

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
