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
#include <dtack/mem.h>

DT_RCSID("test $RCSfile: dttlog.c,v $ $Revision: 1.6 $");

#define USAGE "[-Dnnnn]"

static const char *ops[] = {"all",
  "default", "time", 
  NULL};

/*..........................................................................*/
static 
dt_rc_e
dttlog_make(
  dt_ctl_t *ctl,
  dtimage_t *image,
  dtxy_t ncols,
  dtxy_t nrows,
  dt_ntype_e ntype,
  void *data)
{
  DT_F("dttlog_make");
  dtxy_t y;

  DT_Q(dtimage_create,(ctl,				/* make image to hold given data    */
    image, ncols, nrows, ntype));
 
  for (y=0; y<nrows; y++)				/* copy given data row by row		*/
  {
    void *p;
    DTIMAGE_MAPROW(ctl, image, y, &p);
    DT_MEMCPY(p, data, image->rowsize);
    DTIMAGE_PUTROW(ctl, image, y);
    data = (char *)data + image->rowsize;
  }

  return DT_RC_GOOD;
}
/*..........................................................................*/
static 
dt_rc_e
dttlog_check_and_free(
  dt_ctl_t *ctl,
  dtimage_t *log,
  void *log_data,
  char *f,
  char *id,
  int *assert)
{
  DT_F("dttlog_check_and_free");
  dtimage_t log_expect;
  long cmp;

  DT_Q(dttlog_make,(ctl,				/* allocate expected log image		*/
     &log_expect, 
     log->xe, log->ye,					/* same size and shape as actual    */
     log->ntype,
     log_data));

  DT_Q(dtimage_cmp,(ctl,				/* compare expected to actual	    */
    log, &log_expect, 
    0, &cmp, NULL, NULL));

  *assert = *assert +
    (DT_RC_GOOD == dt_assert(
     ctl, cmp == 0,
     f, "%s", id));

  DT_Q(dtimage_dbg_values,(ctl,
    log, NULL, DT_DBG_MASK_PIXEL));

  DT_Q(dtimage_free,(ctl,				/* free expected image			    */
    &log_expect));

  return DT_RC_GOOD;
}
/*..........................................................................*/
static
dt_rc_e
dttlog1a(
  dt_ctl_t *ctl,
  int *assert)
{
  DT_F("dttlog1a");
  dtimage_t input;
  dtimage_t log;

#define NCOLS 3
#define NROWS 3
  static dt_ntype_b08_t input_data[NCOLS*NROWS] = {
      1,   2,   3,
    127, 128, 129,
    253, 254, 255
  };
  static dt_ntype_b08_t log10_data1[NCOLS*NROWS] = {
      0,   0,   0,
      2,   2,   2,
      2,   2,   2
  };
  static dt_ntype_b08_t log10_data2[NCOLS*NROWS] = {
        1,    31,    48,
      211,   211,   212,
      241,   241,   241
  };
  static dt_ntype_b08_t log_data1[NCOLS*NROWS] = {
      0,   0,   1,
      4,   4,   4,
      5,   5,   5
  };
  static dt_ntype_b08_t log_data2[NCOLS*NROWS] = {
        1,    70,   110,
      255,   255,   255,
      255,   255,   255
  };

  DT_Q(dttlog_make,(ctl, &input,		/* make input image				    */
    NCOLS, NROWS, DT_NTYPE_B08,
    &input_data[0]));

  DT_Q(dtimage_create,(ctl,				/* space for log output			 	*/
    &log, NCOLS, NROWS,
    DT_NTYPE_B08));

  DT_Q(dtimage_log10,(ctl,				
    &input, 1.0, 0.0, &log));
  DT_Q(dttlog_check_and_free,(
    ctl, &log, &log10_data1[0], F,
                                          "log10 8-into-8 unscaled",
    assert));

  DT_Q(dtimage_log10,(ctl,				
    &input, 100.0, 1.0, &log));
  DT_Q(dttlog_check_and_free,(
    ctl, &log, &log10_data2[0], F,
                                          "log10 8-into-8 scaled",
    assert));

  DT_Q(dtimage_log,(ctl,				
    &input, 1.0, 0.0, &log));
  DT_Q(dttlog_check_and_free,(
    ctl, &log, &log_data1[0], F,
                                          "log 8-into-8 unscaled",
    assert));

  DT_Q(dtimage_log,(ctl,				
    &input, 100.0, 1.0, &log));
  DT_Q(dttlog_check_and_free,(
    ctl, &log, &log_data2[0], F,
                                          "log 8-into-8 scaled",
    assert));

  DT_Q(dtimage_free,(ctl, &log));

  DT_Q(dtimage_free,(ctl, &input));

#undef NCOLS
#undef NROWS
    
  return DT_RC_GOOD;
}
/*..........................................................................*/
static
dt_rc_e
dttlog1b(
  dt_ctl_t *ctl,
  int *assert)
{
  DT_F("dttlog1b");
  dtimage_t input;
  dtimage_t log;

#define NCOLS 5
#define NROWS 5
  static dt_ntype_b32_t input_data[NCOLS*NROWS] = {
       1L,     2L,     3L,     4L,     5L,
     127L,   128L,   129L,   130L,   131L,
     253L,   254L,   255L,   256L,   257L,
   32765L, 32766L, 32767L, 32768L, 32769L,
   65533L, 65534L, 65535L, 65536L, 80000L
  };
  static dt_ntype_b08_t log10_data1[NCOLS*NROWS] = {
      0,   0,   0,   0,   0, 
      2,   2,   2,   2,   2, 
      2,   2,   2,   2,   2, 
      4,   4,   4,   4,   4, 
      4,   4,   4,   4,   4, 
  };
  static dt_ntype_b08_t log10_data2[NCOLS*NROWS] = {
        0,    3,    4,    6,    6,
       21,   21,   21,   21,   21,
       24,   24,   24,   24,   24,
       45,   45,   45,   45,   45,
       48,   48,   48,   48,   49
  };
  static dt_ntype_b08_t log_data1[NCOLS*NROWS] = {
      0,   0,   1,   1,   1,
      4,   4,   4,   4,   4,
      5,   5,   5,   5,   5,
     10,  10,  10,  10,  10,
     11,  11,  11,  11,  11
  };
  static dt_ntype_b08_t log_data2[NCOLS*NROWS] = {
        0,    6,   10,   13,   16,
       48,   48,   48,   48,   48,
       55,   55,   55,   55,   55,
      103,  103,  103,  103,  103, 
      110,  110,  110,  110,  112
  };

  DT_Q(dttlog_make,(ctl, &input,		/* make input image				    */
    NCOLS, NROWS, DT_NTYPE_B32,
    &input_data[0]));

  DT_Q(dtimage_create,(ctl,				/* space for log output			 	*/
    &log, NCOLS, NROWS,
    DT_NTYPE_B08));

  DT_Q(dtimage_log10,(ctl,				
    &input, 1.0, 0.0, &log));
  DT_Q(dttlog_check_and_free,(
    ctl, &log, &log10_data1[0], F,
                                          "log10 32-into-8 unscaled",
    assert));

  DT_Q(dtimage_log10,(ctl,				
    &input, 10.0, 0.0, &log));
  DT_Q(dttlog_check_and_free,(
    ctl, &log, &log10_data2[0], F,
                                          "log10 32-into-8 scaled",
    assert));

  DT_Q(dtimage_log,(ctl,				
    &input, 1.0, 0.0, &log));
  DT_Q(dttlog_check_and_free,(
    ctl, &log, &log_data1[0], F,
                                          "log 32-into-8 unscaled",
    assert));

  DT_Q(dtimage_log,(ctl,				
    &input, 10.0, 0.0, &log));
  DT_Q(dttlog_check_and_free,(
    ctl, &log, &log_data2[0], F,
                                          "log 32-into-8 scaled",
    assert));

  DT_Q(dtimage_free,(ctl, &log));

  DT_Q(dtimage_free,(ctl, &input));

#undef NCOLS
#undef NROWS
    
  return DT_RC_GOOD;
}
/*..........................................................................*/
static
dt_rc_e
dttlog2(
  dt_ctl_t *ctl,
  int n,
  dtxy_t ncols,
  dtxy_t nrows,
  int *assert)
{
  DT_F("dttlog2");
  dtimage_t input;
  dtimage_t log;
  dtimage_t overlay;
  int i;
  dtos_time_t mark1, mark2;
  char id[64];
  const double foreground = 101;
  dt_rc_e rc;

#define CCOLS 3
#define CROWS 3
  static dt_ntype_b08_t log10_data1[CCOLS*CROWS] = {
       2,     3,     3,
       3,     3,     3,
       3,     3,     3
  };

  input.xe = 0;							/* set to unallocated			    */
  log.xe = 0;
  overlay.xe = 0;

  DT_C(dtimage_create,(ctl,				/* space for input image			*/
    &input, ncols, nrows,
    DT_NTYPE_B32));

  DT_C(dtimage_constant,(ctl,			/* set input image to constant	    */
    &input, 8191.0));
  DTIMAGE_SETPIX1(DT_C, ctl, &input,	/* except for a few values		    */
    0, 0, foreground);
  DTIMAGE_SETPIX1(DT_C, ctl, &input,
    ncols/2, nrows/2, foreground);
  DTIMAGE_SETPIX1(DT_C, ctl, &input,
    ncols-3, nrows-3, foreground);

  DT_C(dtimage_create,(ctl,				/* space for log output			 	*/
    &log, ncols, nrows,
    DT_NTYPE_B08));

  DT_C(dtos_time,(ctl, &mark1));
  for (i=0; i<n; i++)
    DT_C(dtimage_log10,(ctl,			/* do the big log10				    */
      &input, 1.0, 0.0, &log));
  DT_C(dtos_time,(ctl, &mark2));
  sprintf(id, "log10 unscaled (%d@%ux%u)",
    n, ncols, nrows);
  DT_C(dtt_time_report,(ctl,			/* report time					    */
    &mark1, &mark2, id));

  DT_C(dtimage_overlay,(ctl, &log,
    &overlay, 0, 0, CCOLS, CROWS));
  DT_C(dttlog_check_and_free,(
    ctl, &overlay, &log10_data1[0], F,
                                          "log10 32-into-8 unscaled (top)",
    assert));
  DT_C(dtimage_free,(ctl, &overlay));

  DT_C(dtimage_overlay,(ctl, &log,
    &overlay, ncols/2, nrows/2,
    CCOLS, CROWS));
  DT_C(dttlog_check_and_free,(
    ctl, &overlay, &log10_data1[0], F,
                                          "log10 32-into-8 unscaled (middle)",
    assert));
  DT_C(dtimage_free,(ctl, &overlay));

  DT_C(dtimage_overlay,(ctl, &log,
    &overlay, ncols-3, nrows-3,
    CCOLS, CROWS));
  DT_C(dttlog_check_and_free,(
    ctl, &overlay, &log10_data1[0], F,
                                          "log10 32-into-8 unscaled (bottom)",
    assert));

cleanup:
  if (overlay.xe)
  DT_I(dtimage_free,(ctl, &overlay));

  if (log.xe)
  DT_I(dtimage_free,(ctl, &log));

  if (input.xe)
  DT_I(dtimage_free,(ctl, &input));

#undef CCOLS
#undef CROWS
    
  return rc;
}
/*..........................................................................*/
static
dt_rc_e
dttlog3(
  dt_ctl_t *ctl,
  int n,
  dtxy_t ncols,
  dtxy_t nrows,
  int *assert)
{
  DT_F("dttlog3");
  dtimage_t input;
  dtimage_t log;
  dtimage_t overlay;
  int i;
  dtos_time_t mark1, mark2;
  char id[64];
  dt_rc_e rc;

#define CCOLS 3
#define CROWS 3
  static dt_ntype_b08_t log10_data1[CCOLS*CROWS] = {
       4,     4,     4,
       4,     4,     4,
       4,     4,     4
  };

  input.xe = 0;							/* set to unallocated			    */
  log.xe = 0;
  overlay.xe = 0;

  DT_C(dtimage_create,(ctl,				/* space for input image			*/
    &input, ncols, nrows,
    DT_NTYPE_B32));

  DT_C(dtimage_constant,(ctl,			/* set input image to constant	    */
    &input, 10000.0));

  DT_C(dtimage_create,(ctl,				/* space for log output			 	*/
    &log, ncols, nrows,
    DT_NTYPE_B08));

  DT_C(dtos_time,(ctl, &mark1));
  for (i=0; i<n; i++)
    DT_C(dtimage_log10,(ctl,			/* do the big log10				    */
      &input, 1.0, 0.0, &log));
  DT_C(dtos_time,(ctl, &mark2));
  sprintf(id, "log10 scaled (%d@%ux%u)",
    n, ncols, nrows);
  DT_C(dtt_time_report,(ctl,			/* report time					    */
    &mark1, &mark2, id));

  DT_C(dtimage_overlay,(ctl, &log,
    &overlay, 0, 0, CCOLS, CROWS));
  DT_C(dttlog_check_and_free,(
    ctl, &overlay, &log10_data1[0], F,
                                          "log10 32-into-8 unscaled (top)",
    assert));
  DT_C(dtimage_free,(ctl, &overlay));

  DT_C(dtimage_overlay,(ctl, &log,
    &overlay, ncols/2, nrows/2,
    CCOLS, CROWS));
  DT_C(dttlog_check_and_free,(
    ctl, &overlay, &log10_data1[0], F,
                                          "log10 32-into-8 unscaled (middle)",
    assert));
  DT_C(dtimage_free,(ctl, &overlay));

  DT_C(dtimage_overlay,(ctl, &log,
    &overlay, ncols-3, nrows-3,
    CCOLS, CROWS));
  DT_C(dttlog_check_and_free,(
    ctl, &overlay, &log10_data1[0], F,
                                          "log10 32-into-8 unscaled (bottom)",
    assert));

cleanup:
  if (overlay.xe)
  DT_I(dtimage_free,(ctl, &overlay));

  if (log.xe)
  DT_I(dtimage_free,(ctl, &log));

  if (input.xe)
  DT_I(dtimage_free,(ctl, &input));

#undef CCOLS
#undef CROWS
    
  return rc;
}
/*..........................................................................*/
static
dt_rc_e
dttlog4(
  dt_ctl_t *ctl,
  int n,
  dtxy_t ncols,
  dtxy_t nrows,
  int *assert)
{
  DT_F("dttlog4");
  dtimage_t input;
  dtimage_t log;
  dtimage_t overlay;
  int i;
  dtos_time_t mark1, mark2;
  char id[64];
  const double foreground = 101;
  dt_rc_e rc;

#define CCOLS 3
#define CROWS 3
  static dt_ntype_b08_t log_data1[CCOLS*CROWS] = {
       4,     9,     9,
       9,     9,     9,
       9,     9,     9
  };

  input.xe = 0;							/* set to unallocated			    */
  log.xe = 0;
  overlay.xe = 0;

  DT_C(dtimage_create,(ctl,				/* space for input image			*/
    &input, ncols, nrows,
    DT_NTYPE_B32));

  DT_C(dtimage_constant,(ctl,			/* set input image to constant	    */
    &input, 8191.0));
  DTIMAGE_SETPIX1(DT_C, ctl, &input,	/* except for a few values		    */
    0, 0, foreground);
  DTIMAGE_SETPIX1(DT_C, ctl, &input,
    ncols/2, nrows/2, foreground);
  DTIMAGE_SETPIX1(DT_C, ctl, &input,
    ncols-3, nrows-3, foreground);

  DT_C(dtimage_create,(ctl,				/* space for log output			 	*/
    &log, ncols, nrows,
    DT_NTYPE_B08));


  DT_C(dtos_time,(ctl, &mark1));
  for (i=0; i<n; i++)
    DT_C(dtimage_log,(ctl,				/* do the big log				    */
      &input, 1.0, 0.0, &log));
  DT_C(dtos_time,(ctl, &mark2));
  sprintf(id, "log unscaled (%d@%ux%u)",
    n, ncols, nrows);
  DT_C(dtt_time_report,(ctl,			/* report time					    */
    &mark1, &mark2, id));

  DT_C(dtimage_overlay,(ctl, &log,
    &overlay, 0, 0, CCOLS, CROWS));
  DT_C(dttlog_check_and_free,(
    ctl, &overlay, &log_data1[0], F,
                                          "log 32-into-8 unscaled (top)",
    assert));
  DT_C(dtimage_free,(ctl, &overlay));

  DT_C(dtimage_overlay,(ctl, &log,
    &overlay, ncols/2, nrows/2,
    CCOLS, CROWS));
  DT_C(dttlog_check_and_free,(
    ctl, &overlay, &log_data1[0], F,
                                          "log 32-into-8 unscaled (middle)",
    assert));
  DT_C(dtimage_free,(ctl, &overlay));

  DT_C(dtimage_overlay,(ctl, &log,
    &overlay, ncols-3, nrows-3,
    CCOLS, CROWS));
  DT_C(dttlog_check_and_free,(
    ctl, &overlay, &log_data1[0], F,
                                          "log 32-into-8 unscaled (bottom)",
    assert));

cleanup:
  if (overlay.xe)
  DT_I(dtimage_free,(ctl, &overlay));

  if (log.xe)
  DT_I(dtimage_free,(ctl, &log));

  if (input.xe)
  DT_I(dtimage_free,(ctl, &input));

#undef CCOLS
#undef CROWS
    
  return rc;
}
/*..........................................................................*/
static
dt_rc_e
dttlog5(
  dt_ctl_t *ctl,
  int n,
  dtxy_t ncols,
  dtxy_t nrows,
  int *assert)
{
  DT_F("dttlog5");
  dtimage_t input;
  dtimage_t log;
  dtimage_t overlay;
  int i;
  dtos_time_t mark1, mark2;
  char id[64];
  dt_rc_e rc;

#define CCOLS 3
#define CROWS 3
  static dt_ntype_b08_t log_data1[CCOLS*CROWS] = {
       9,     9,     9,
       9,     9,     9,
       9,     9,     9
  };

  input.xe = 0;							/* set to unallocated			    */
  log.xe = 0;
  overlay.xe = 0;

  DT_C(dtimage_create,(ctl,				/* space for input image			*/
    &input, ncols, nrows,
    DT_NTYPE_B32));

  DT_C(dtimage_constant,(ctl,			/* set input image to constant	    */
    &input, 10000.0));

  DT_C(dtimage_create,(ctl,				/* space for log output			 	*/
    &log, ncols, nrows,
    DT_NTYPE_B08));


  DT_C(dtos_time,(ctl, &mark1));
  for (i=0; i<n; i++)
    DT_C(dtimage_log,(ctl,				/* do the big log				    */
      &input, 1.0, 0.0, &log));
  DT_C(dtos_time,(ctl, &mark2));

  sprintf(id, "log scaled (%d@%ux%u)",
    n, ncols, nrows);
  DT_C(dtt_time_report,(ctl,			/* report time					    */
    &mark1, &mark2, id));

  DT_C(dtimage_overlay,(ctl, &log,
    &overlay, 0, 0, CCOLS, CROWS));
  DT_C(dttlog_check_and_free,(
    ctl, &overlay, &log_data1[0], F,
                                          "log 32-into-8 unscaled (top)",
    assert));
  DT_C(dtimage_free,(ctl, &overlay));

  DT_C(dtimage_overlay,(ctl, &log,
    &overlay, ncols/2, nrows/2,
    CCOLS, CROWS));
  DT_C(dttlog_check_and_free,(
    ctl, &overlay, &log_data1[0], F,
                                          "log 32-into-8 unscaled (middle)",
    assert));
  DT_C(dtimage_free,(ctl, &overlay));

  DT_C(dtimage_overlay,(ctl, &log,
    &overlay, ncols-3, nrows-3,
    CCOLS, CROWS));
  DT_C(dttlog_check_and_free,(
    ctl, &overlay, &log_data1[0], F,
                                          "log 32-into-8 unscaled (bottom)",
    assert));

cleanup:
  if (overlay.xe)
  DT_I(dtimage_free,(ctl, &overlay));

  if (log.xe)
  DT_I(dtimage_free,(ctl, &log));

  if (input.xe)
  DT_I(dtimage_free,(ctl, &input));

#undef CCOLS
#undef CROWS
    
  return rc;
}

/*..........................................................................*/

dt_rc_e 
DTCONFIG_API1
dttlog_arg_check(						/* check args and params			*/
  dt_ctl_t *ctl,
  void *app,
  dtparam_t *param,
  dt_enter_arg_t *arg,
  char *msg,
  const char **start)
{
  DT_F("dttlog_arg_check");

  DT_Q(dtt_check_op,(ctl, param,		/* make sure op is understood	    */
    ops, msg, start));

  return DT_RC_GOOD;
}

/*..........................................................................*/

DTT_TEST(dttlog)
{
  DT_F("dttlog");
  dt_ctl_t *ctl = &param->ctl;
  dt_enter_arg_t arg[1];
  int assert_fail = 0;					/* failed assertion count		    */
  dt_rc_e rc;

  DT_C(dt_heapmon_init,(ctl));			/* init memory checking			    */

  DT_C(dtparam_init,(param));			/* init param structure             */

  DT_C(dtparam_set,(param, 0,			/* set default for params we want	*/
    &param->op, NULL, NULL, "default",
    &param->reps, NULL, NULL, "1",
    &param->n, "Timing loops", NULL, "5",
    &param->w, "Timing width", NULL, "128",
    &param->h, "Timing height", NULL, "120",
    NULL));

  arg[0].desc = NULL;					/* no arguments					    */
  DT_C(dt_enter_parse,(					/* go parse params and args			*/
    argc, argv, F, USAGE, param,
    arg, dttlog_arg_check, NULL));
										/* ................................ */
  {
    long reps;
    for (reps=0; 
         reps < param->reps && 
         rc == DT_RC_GOOD;
         reps++)
	{
      if (!strcmp(param->op, "all") ||
          !strcmp(param->op, "default"))
      {
        DT_C(dttlog1a,(ctl, &assert_fail));
        DT_C(dttlog1b,(ctl, &assert_fail));
      }

      if (!strcmp(param->op, "all") ||
          !strcmp(param->op, "time"))
      {
        DT_C(dttlog2,(ctl, 
          (int)param->n,
          (dtxy_t)param->w,
          (dtxy_t)param->h,
          &assert_fail));
        DT_C(dttlog3,(ctl,
          (int)param->n,
          (dtxy_t)param->w,
          (dtxy_t)param->h,
          &assert_fail));
        DT_C(dttlog4,(ctl,
          (int)param->n,
          (dtxy_t)param->w,
          (dtxy_t)param->h,
          &assert_fail));
        DT_C(dttlog5,(ctl,
          (int)param->n,
          (dtxy_t)param->w,
          (dtxy_t)param->h,
          &assert_fail));
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
