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
DT_RCSID("test $RCSfile: dttarith.c,v $ $Revision: 1.6 $");

#define USAGE "[-Dnnnn]"

static const char *ops[] = {"all",
  "default", "time", 
  NULL};

#define ND(D) (sizeof((D))/sizeof(*(D)))

/*..........................................................................*/
static 
dt_rc_e
dttarith_make(							/* make image by tiling data 		*/
  dt_ctl_t *ctl,
  dtimage_t *image,
  dtxy_t ncols,
  dtxy_t nrows,
  dt_ntype_e ntype,
  double *data,
  dtxy_t ndata)
{
  DT_F("dttarith_make");
  dtxy_t x, y;
  unsigned long k = 0;
  double *vector;

  DT_Q(dtimage_create3,(ctl,			/* make image to hold given data    */
    image, 0, 1, ncols, nrows, 
    ntype, NULL));

  DT_Q(dtos_malloc2,(ctl,				/* space for vector 				*/
    (void **)&vector, 
    ncols*sizeof(*vector),
    F, "vector"));

  for (y=0; y<nrows; y++)				/* copy given data row by row		*/
  {
    void *p;
    DTIMAGE_MAPROW(ctl, image, y, &p);
    for (x=0; x<ncols; x++)				/* tile data into vector 			*/
      vector[x] = data[
        (int)(k++ % ndata)];
    DT_Q_1D(dt1d_cast,(
      vector, ncols, DT_NTYPE_F2,
      p, ncols, ntype));
    DTIMAGE_PUTROW(ctl, image, y);
  }
  
  DT_Q(dtos_free2,(ctl,
    vector, F, "vector"));

  return DT_RC_GOOD;
}

/*..........................................................................*/
static 
dt_rc_e
dttarith_check_image(					/* check image for expected data 	*/
  dt_ctl_t *ctl,
  dtimage_t *actual,
  dt_ntype_e ntype,
  double *data,
  dtxy_t ndata,
  const char *f,
  const char *id,
  int *assert)
{
  DT_F("dttarith_check_image");
  dtimage_t expect;
  long cmp;

  DT_Q(dttarith_make,(ctl,				/* allocate expected log image		*/
     &expect, 
     actual->xe, actual->ye,			/* same size and shape as actual    */
     ntype, data, ndata));

  DT_Q(dtimage_cmp,(ctl,				/* compare expected to actual	    */
    actual, &expect, 0,
    &cmp, NULL, NULL));

  *assert = *assert +
    (DT_RC_GOOD == dt_assert(
     ctl, cmp == 0,
     f, "%s cmp %ld (%ld)", 
     id, cmp, 0L));

  DT_Q(dtimage_free,(ctl, &expect));	/* free expected image			    */

  DT_Q(dtimage_free,(ctl, actual));		/* free actual image			    */

  return DT_RC_GOOD;
}

/*..........................................................................*/
static
dt_rc_e
dttarith_op(							/* do scalar op and check 			*/
  dt_ctl_t *ctl,
  dt_op_e op,
  double a,
  double b,
  double c,
  int ncols,
  int nrows,
  dt_ntype_e input1_ntype,
  double *input1_data,
  dtxy_t input1_ndata,
  dt_ntype_e input2_ntype,
  double *input2_data,
  dtxy_t input2_ndata,
  dt_ntype_e expect_ntype,
  double *expect_data,
  dtxy_t expect_ndata,
  long loops,
  const char *id,
  int *assert)
{
  DT_F("dttarith_op");
  dtimage_t input1, input2, actual;
  dtos_time_t mark1, mark2;
  long loop;
  dt_rc_e rc;

  input1.xe = 0;
  input2.xe = 0;
  actual.xe = 0;
  
  DT_C(dttarith_make,(ctl,				/* load up source image 			*/
    &input1, ncols, nrows, input1_ntype,
    input1_data, input1_ndata));
  
  DT_C(dttarith_make,(ctl,				/* load up source image 			*/
    &input2, ncols, nrows, input2_ntype,
    input2_data, input2_ndata));

  DT_C(dtimage_create3,(ctl,			/* space for actualed image			*/
    &actual, 0, 1, ncols, nrows, 
    expect_ntype, NULL));

  DT_C(dtos_time,(ctl, &mark1));
  for (loop=0; loop<loops; loop++)
  {
	if (op == DT_OP_ADD ||				/* one of the arithmetic ones? 		*/
		op == DT_OP_MUL ||
		op == DT_OP_DIV)
      DT_C(dtimage_arith,(ctl,
        &input1, a,
        &input2, b, c,
        op, 0, &actual))
	else								/* must be a logical one 			*/
      DT_C(dtimage_logical,(ctl, 
        &input1, (unsigned long)a,
        &input2, (unsigned long)b, 
        (unsigned long)c,
        op, &actual))
  }
  DT_C(dtos_time,(ctl, &mark2));

  DT_C(dtimage_dbg_values,(ctl,
    &actual, F, DT_DBG_MASK_PIXEL));
  DT_C(dttarith_check_image,(ctl,		/* check results and free images 	*/
    &actual, 
    expect_ntype, 
    expect_data, 
    expect_ndata,
    F, id, assert));

  DT_C(dtt_time_report,(ctl,			/* report time					    */
    &mark1, &mark2, id));

cleanup:
  if (input2.xe)
    DT_I(dtimage_free,(ctl, &input2));	/* free input2 image		    	*/
  if (input1.xe)
    DT_I(dtimage_free,(ctl, &input1));	/* free input1 image		    	*/
  if (actual.xe)
    DT_I(dtimage_free,(ctl, &actual));	/* free actual image			    */
  return rc;
}

/*..........................................................................*/
static
dt_rc_e
dttarith_file(
  dt_ctl_t *ctl,
  const char *testdata,
  dtxy_t ncols,
  dtxy_t nrows,
  long loops,
  int *assert)
{
  DT_F("dttarith_file");
  dtfd_t fd;
  int l = 0;
  double input1[8];
  dt_ntype_e input1_ntype = DT_NTYPE_B08;
  int input1_ndata;
  double input2[8];
  dt_ntype_e input2_ntype = DT_NTYPE_B08;
  int input2_ndata;
  double a, b, c;
  dt_rc_e rc;

  DT_Q(dtfd_open,(ctl, &fd,				/* open data file 					*/
    testdata, "r"));

  while(1)
  {
	char line[128];
	int n;
	double d[8];
	char cmd[32], type[32];
	int choice;
	dt_op_e op;
	double expect[8];
	dt_ntype_e expect_ntype;
	int expect_ndata;
	char op_char;
	char id[128];

	l++;
	if (fgets(line, sizeof(line),		/* read until end of file 			*/
          fd.file) == NULL)
	  break;

	cmd[0] = '\0';
	type[0] = '\0';
	
    n = sscanf(line,					/* read line from data file 		*/
      "%s %s"
      " %le %le %le %le "
      " %le %le %le %le",
      cmd, type, 
      &d[0], &d[1], &d[2], &d[3],
      &d[4], &d[5], &d[6], &d[7]);
	
	if (cmd[0] == '#')					/* skip comment 					*/
	  continue;
	if (n == 0 || cmd[0] == '\0')		/* skip blank line 					*/
	  continue;
	if (!strcmp(cmd, "exit"))			/* exit command? 					*/
	  break;
	
    if (n < 5)
	{
	  rc = dt_err(ctl, F,
        "not enough stuff on line %d",
        l);
	  goto cleanup;
	}
	
	DT_C(dtstr_choice,(ctl,				/* type should be ntype string 		*/
      dt_ntype_string, type, &choice));

	if (choice < 0)
	{
	  rc = dt_err(ctl, F,
        "invalid type \"%s\""
        " on line %d",
        type, l);
	  goto cleanup;
	}

    if (!strcmp(cmd, "scalar"))			/* this is a scalar line 			*/
	{
	  a = d[0];
	  b = d[1];
	  c = d[2];
	  continue;
	} 

	if (!strcmp(cmd, "data1"))			/* this is a data line? 			*/
	{
	  input1_ndata = n - 2;
	  input1_ntype = dt_ntype_alias[choice-1];
	  dt1d_cast(
        d, input1_ndata, 
          DT1D_NTYPE_F2,
        input1, input1_ndata, 
          DT1D_NTYPE_F2);
	  continue;
	}

	if (!strcmp(cmd, "data2"))			/* this is a data line? 			*/
	{
	  input2_ndata = n - 2;
	  input2_ntype = dt_ntype_alias[choice-1];
	  dt1d_cast(
        d, input2_ndata, 
          DT1D_NTYPE_F2,
        input2, input2_ndata, 
          DT1D_NTYPE_F2);
	  continue;
	}

	if (!strcmp(cmd, "add"))			/* this is an operation line? 		*/
	{
	  op = DT_OP_ADD;
	  op_char = '+';
	}
	else
	if (!strcmp(cmd, "mult"))
	{
	  op = DT_OP_MUL;
	  op_char = '*';
	}
	else
	if (!strcmp(cmd, "div"))
	{
	  op = DT_OP_DIV;
	  op_char = '/';
	}
	else
	if (!strcmp(cmd, "or"))
	{
	  op = DT_OP_OR;
	  op_char = '|';
	}
	else
	if (!strcmp(cmd, "and"))
	{
	  op = DT_OP_AND;
	  op_char = '&';
	}
	else
	if (!strcmp(cmd, "xor"))
	{
	  op = DT_OP_XOR;
	  op_char = '^';
	}
	else
	{
	  rc = dt_err(ctl, F,
        "invalid command \"%s\" on line %d",
        cmd, l);
	  goto cleanup;
	}

	expect_ndata = n - 2;
	expect_ntype = dt_ntype_alias[choice-1];
	dt1d_cast(
      d, expect_ndata, DT1D_NTYPE_F2,
      expect, expect_ndata, DT1D_NTYPE_F2);

	if (ctl->dbg_mask & 
		DT_DBG_MASK_TIME)
      sprintf(id, 
        "%ld L%03d %-4s %-3s%c%-3s->%-3s %dx%d", 
        loops, l, cmd,
        dt_ntype_string[input1_ntype], op_char,
        b? dt_ntype_string[input2_ntype]: "xxx",
	    dt_ntype_string[expect_ntype],
        ncols, nrows);
	else
      sprintf(id, 
        "L%03d %-4s %3s%c%-3s->%-3s",
        l, cmd, 
        dt_ntype_string[input1_ntype], op_char,
        b? dt_ntype_string[input2_ntype]: "xxx",
	    dt_ntype_string[expect_ntype]);
	
    DT_C(dttarith_op,(ctl,		/* make images, do operation 		*/
      op, a, b, c, ncols, nrows,
      input1_ntype, input1, input1_ndata,
      input2_ntype, input2, input2_ndata,
      expect_ntype, expect, expect_ndata,
      loops, id, assert));
  }

cleanup:
  DT_I(dtfd_close,(ctl, &fd));
  return rc;
}

/*..........................................................................*/

dt_rc_e 
DTCONFIG_API1
dttarith_arg_check(						/* check args and params			*/
  dt_ctl_t *ctl,
  void *app,
  dtparam_t *param,
  dt_enter_arg_t *arg,
  char *msg,
  const char **start)
{
  DT_F("dttarith_arg_check");

  DT_Q(dtt_check_op,(ctl, param,		/* make sure op is understood	    */
    ops, msg, start));

  return DT_RC_GOOD;
}

/*..........................................................................*/

DTT_MAIN(dttarith)
{
  DT_F("dttarith");
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
    arg, dttarith_arg_check, NULL));

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
        DT_C(dttarith_file,(ctl,
          testdata, 4, 4, 1,
          &assert_fail));
      }

      if (!strcmp(param->op, "all") ||
          !strcmp(param->op, "time"))
      {
        DT_C(dttarith_file,(ctl, 
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
