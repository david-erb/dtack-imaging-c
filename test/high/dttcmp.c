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
DT_RCSID("test $RCSfile: dttcmp.c,v $ $Revision: 1.6 $");

#define USAGE "[-Dnnnn]"

static const char *ops[] = {"all",
  "default", "time", 
  NULL};

#define ND(D) (sizeof((D))/sizeof(*(D)))

/*..........................................................................*/
static 
dt_rc_e
dttcmp_make(							/* make image by tiling data 		*/
  dt_ctl_t *ctl,
  dtimage_t *image,
  dtxy_t ncols,
  dtxy_t nrows,
  dtntype_e ntype,
  double *data,
  dtxy_t ndata,
  dtxy_t xsplat,
  dtxy_t ysplat,
  double splat)			      
{
  DT_F("dttcmp_make");
  dtxy_t x, y;
  unsigned long k = 0;
  double *vector = NULL;
  void *p;
  dt_rc_e rc;

  image->xe = 0;
  DT_C(dtimage_create3,(ctl,			/* make image to hold given data    */
    image, 0, 1, ncols, nrows, 
    ntype, NULL));

  DT_C(dtos_malloc2,(ctl,				/* space for vector 				*/
    (void **)&vector, 
    ncols*sizeof(*vector),
    F, "vector"));

  for (y=0; y<nrows; y++)				/* copy given data row by row		*/
  {
    DTIMAGE_MAPROW2(DT_C, 
      ctl, image, y, &p);
    for (x=0; x<ncols; x++)				/* tile data into vector 			*/
      vector[x] = data[k++ % ndata];
    DT_C_1D(dt1d_cast,(
      vector, ncols, DTNTYPE_F2,
      p, ncols, ntype));
    DTIMAGE_PUTROW2(DT_C, 
      ctl, image, y);
  }

  if (xsplat != -1 &&
	  ysplat != -1)
  {
    DTIMAGE_GETROW2(DT_C,
      ctl, image, ysplat, &p);
    DTNTYPE_SET1(DT_C, ctl, p, 
      xsplat, ntype, splat); 
    DTIMAGE_PUTROW2(DT_C,
      ctl, image, ysplat);
  }

cleanup:
  if (vector != NULL)
    DT_I(dtos_free2,(ctl,
      vector, F, "vector"));

  if (rc != DT_RC_GOOD && image->xe)
    DT_I(dtimage_free,(ctl, image));

  return rc;
}

/*..........................................................................*/
static
dt_rc_e
dttcmp_cmp_one(
  dt_ctl_t *ctl,
  int input1_ncols,
  int input1_nrows,
  dtntype_e input1_ntype,
  double *input1_data,
  dtxy_t input1_ndata,
  int input2_ncols,
  int input2_nrows,
  dtntype_e input2_ntype,
  double *input2_data,
  dtxy_t input2_ndata,
  dtxy_t xsplat,
  dtxy_t ysplat,
  double splat,			      
  long expect_cmp,
  dtxy_t expect_xdiff,
  dtxy_t expect_ydiff,
  long loops,
  const char *id,
  int *assert)
{
  DT_F("dtcmp_cmp1");
  dtimage_t input1, input2;
  dtos_time_t mark1, mark2;
  long loop;
  long cmp;
  dtxy_t x, y;
  dt_rc_e rc;

  input1.xe = 0;
  input2.xe = 0;
  
  DT_C(dttcmp_make,(ctl,				/* load up source image 			*/
    &input1, 
    input1_ncols, input1_nrows,
    input1_ntype,
    input1_data, input1_ndata,
    xsplat, ysplat, splat));
  
  DT_C(dttcmp_make,(ctl,				/* load up source image 			*/
    &input2, 
    input2_ncols, input2_nrows,
    input2_ntype,
    input2_data, input2_ndata,
    -1, -1, 0));

  DT_C(dtos_time,(ctl, &mark1));
  for (loop=0; loop<loops; loop++)
  {
    DT_C(dtimage_cmp,(ctl,
      &input1, 
      &input2,
      0, &cmp, &x, &y));
  }
  DT_C(dtos_time,(ctl, &mark2));

  *assert = *assert +
    (DT_RC_GOOD == dt_assert(
     ctl, 
     cmp == expect_cmp &&
	 (cmp == 0 || x == expect_xdiff) &&
	 (cmp == 0 || y == expect_ydiff), 
     F, "%s cmp %ld at %u,%u (%ld at %u,%d)", 
     id, cmp, cmp? x: 0, cmp? y: 0, expect_cmp, expect_xdiff, expect_ydiff));

  DT_C(dtt_time_report,(ctl,			/* report time					    */
    &mark1, &mark2, id));

cleanup:
  if (input2.xe)
    DT_I(dtimage_free,(ctl, &input2));	/* free input2 image		    	*/
  if (input1.xe)
    DT_I(dtimage_free,(ctl, &input1));	/* free input1 image		    	*/
  return rc;
}

/*..........................................................................*/
static
dt_rc_e
dttcmp_cmp_all(
  dt_ctl_t *ctl,
  int input1_ncols,
  int input1_nrows,
  double *input1_data,
  dtxy_t input1_ndata,
  int input2_ncols,
  int input2_nrows,
  double *input2_data,
  dtxy_t input2_ndata,
  dtxy_t xsplat,
  dtxy_t ysplat,
  double splat,			      
  long expect_cmp,
  dtxy_t expect_xdiff,
  dtxy_t expect_ydiff,
  const char *id,
  long loops,
  int *assert)
{
  DT_F("dttcmp_cmp_all");
  int i;
  int j;

  for (i=0; dt_ntype_list[i] != DTNTYPE_NULL; i++)
  {
    for (j=0; dt_ntype_list[j] != DTNTYPE_NULL; j++)
	{
	  char id2[64];
	  dtstr_printf(ctl, id2, sizeof(id2),
        "%8s %-3s-%3s", id,
        dt_ntype_string[dt_ntype_list[i]],
        dt_ntype_string[dt_ntype_list[j]]);
	  DT_Q(dttcmp_cmp_one,(ctl,
        input1_ncols, input1_nrows,
        dt_ntype_list[i],
        input1_data, input1_ndata,
        input2_ncols, input2_nrows,
        dt_ntype_list[j],
        input2_data, input2_ndata,
		xsplat, ysplat, splat,
        expect_cmp, expect_xdiff, expect_ydiff,
        loops, id2, assert));
	}
  }
  return DT_RC_GOOD;
}

/*..........................................................................*/
static
dt_rc_e
dttcmp_cmp(
  dt_ctl_t *ctl,
  const char *testdata,
  dtxy_t ncols,
  dtxy_t nrows,
  long loops,
  int *assert)
{
  DT_F("dttcmp_cmp");
#define N (5)
  double i1[N] = {0, 1, 1, 0, 1};

										/* ................................ */
  DT_Q(dttcmp_cmp_all,(ctl,
    ncols, nrows,  
    i1, N,
    ncols, nrows,  
    i1, N,
    -1, -1, 0,
    0, 0, 0, 
    "same", 
    loops, assert));

  DT_Q(dttcmp_cmp_all,(ctl,
    ncols+N*8, nrows,  
    i1, N,
    ncols, nrows,  
    i1, N,
    -1, -1, 0,
    0, 0, 0, 
    "same, wider1", 
    loops, assert));

  DT_Q(dttcmp_cmp_all,(ctl,
    ncols, nrows+1,  
    i1, N,
    ncols, nrows,  
    i1, N,
    -1, -1, 0,
    0, 0, 0, 
    "same, higher1", 
    loops, assert));

  DT_Q(dttcmp_cmp_all,(ctl,
    ncols, nrows,  
    i1, N,
    ncols+8*N, nrows,  
    i1, N,
    -1, -1, 0,
    0, 0, 0, 
    "same, wider2", 
    loops, assert));

  DT_Q(dttcmp_cmp_all,(ctl,
    ncols, nrows,  
    i1, N,
    ncols, nrows+1,  
    i1, N,
    -1, -1, 0,
    0, 0, 0, 
    "same, higher2", 
    loops, assert));
										/* ................................ */
  DT_Q(dttcmp_cmp_all,(ctl,
    ncols, nrows,  
    i1, N,
    ncols, nrows,  
    i1, N,
    6, 4, 6,							/* splat location and value 		*/
    1, 6, 4, 
    "diff", 
    loops, assert));

  DT_Q(dttcmp_cmp_all,(ctl,
    ncols, nrows,  
    i1, N,
    ncols+N*8, nrows,  
    i1, N,
    6, 4, 6,							/* splat location and value 		*/
    1, 6, 4, 
    "diff, wider1", 
    loops, assert));

  return DT_RC_GOOD;
}

/*..........................................................................*/

dt_rc_e 
DTCONFIG_API1
dttcmp_arg_check(						/* check args and params			*/
  dt_ctl_t *ctl,
  void *app,
  dtparam_t *param,
  dt_enter_arg_t *arg,
  char *msg,
  const char **start)
{
  DT_F("dttcmp_arg_check");

  DT_Q(dtt_check_op,(ctl, param,		/* make sure op is understood	    */
    ops, msg, start));

  return DT_RC_GOOD;
}

/*..........................................................................*/

DTT_MAIN(dttcmp)
{
  DT_F("dttcmp");
  dt_ctl_t *ctl = &param->ctl;
  dt_enter_arg_t arg[1];
  char testdata[128];
  int assert_fail = 0;					/* failed assertion count		    */
  dt_rc_e rc;

  DT_C(dt_heapmon_init,(ctl));			/* init memory checking			    */

  DT_C(dtparam_init,(param));			/* init param structure             */

  DT_C(dtparam_set,(param, 0,			/* set default for params we want	*/
    &param->op, NULL, NULL, "default",
    &param->datadir, NULL, NULL, NULL,
    &param->input, NULL, NULL, NULL,
    &param->reps, NULL, NULL, "1",
    &param->n, "Timing loops", NULL, "5",
    &param->w, "Timing width", NULL, "128",
    &param->h, "Timing height", NULL, "120",
    NULL));

  arg[0].desc = NULL;					/* no arguments					    */
  DT_C(dt_enter_parse,(					/* go parse params and args			*/
    argc, argv, F, USAGE, param,
    arg, dttcmp_arg_check, NULL));

  DT_C(dtstr_printf,(ctl, testdata,
    sizeof(testdata), "%s/%s",
    param->datadir, 
    param->input? param->input:
    "arith.dat"));
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
        DT_C(dttcmp_cmp,(ctl,
          testdata, 8, 8, 1,
          &assert_fail));
      }

      if (!strcmp(param->op, "all") ||
          !strcmp(param->op, "time"))
      {
        DT_C(dttcmp_cmp,(ctl, 
          testdata,
          (dtxy_t)param->w,				/* test image size 					*/
		  (dtxy_t)param->h, 
          param->n,						/* loop count 						*/
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
