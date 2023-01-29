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
| dt_flo76 command - Levine&Nazif 1985
| 
| DESCRIPTION
| As yet undocumented.
| 
| RETURN VALUES
| Program exit value will be DT_EXIT_GOOD on success.  
| Any other exit value indicates failure or noncompletion.
| 
| ERRORS
| As yet undocumented.
| 
| END
 *..........................................................................*/

#define LINE printf("%d\n", __LINE__);

#include <dtack/base.h>
DT_RCSID("alg/flo76 $RCSfile: cmd.c,v $ $Revision: 1.1 $");
#include <dtack/os.h>
#include <dtack/str.h>
#include <dtack/1d.h>
#include <dt_flo76.h>

static char *USAGE = "usage: [params] input_image output_image\n\
Implementation of algorithm by Floyd and Steinberg, 1976.\n\
Binarizes input image using error diffusion.\n\
Output image always has same dimensions as input image.\n\
Output image pixels take one of two values: \n\
  param.foreground\n\
  param.background";

static const char *ops[] = {
# define OP_DO 0
  "do",
# define OP_TEST 1
  "test",
# define OP_TIME 2
  "time",
  NULL};

/*..........................................................................*/

dt_rc_e
dt_flo76_cmd(
  int argc,
  char *argv[])
{
  DT_F("dt_flo76_cmd");
  dtparam_t param = {0};				/* parameter structure				*/
  dt_ctl_t *ctl = &param.ctl;
  int op;
  int n;
  dt_rc_e rc = DT_RC_GOOD;

  DT_Q(dt_flo76_cmd_parse,(&param,		/* parse args					    */
    argc, argv, &op, &n));

  argc -= n;
  argv += n;

  switch(op)
  {
    case OP_DO:
	{
      if (argc != 2)					/* check remaining args				*/
        rc = dt_interactive_usage(
          &param, USAGE);
      DT_GI(dt_flo76_cmd_import_and_do,	/* import images, do the operation	*/
        (&param, argv[0], argv[1]));
	}
    break;
    case OP_TEST:
	{
      int assert_fail = 0;
      if (argc != 0)					/* check remaining args				*/
        rc = dt_interactive_usage(
          &param, USAGE);
      DT_GI(dt_flo76_test_all,(&param,	/* do the operation				    */
        &assert_fail));

      if (rc == DT_RC_GOOD &&			/* test worked?					    */
          assert_fail)					/* but some results wrong?		    */
        rc = DT_RC_BAD;
	}
    break;

#ifdef NONO
    case OP_TIME:
	{
      int assert_fail = 0;
      if (argc != 0)					/* check remaining args				*/
        rc = dt_interactive_usage(
          &param, USAGE);
      DT_GI(dt_flo76_time_all,(&param,	/* do the operation				    */
        &assert_fail));

      if (rc == DT_RC_GOOD &&			/* test worked?					    */
          assert_fail)					/* but some results wrong?		    */
        rc = DT_RC_BAD;
	}
    break;
#endif
  }

  DT_Q(dtparam_free,(&param));			/* free param space					*/

  return rc;
}

/*..........................................................................*/

dt_rc_e
dt_flo76_cmd_import_and_do(
  dtparam_t *param,
  char *input_name,
  char *output_name)
{
  DT_F("dt_flo76_import_and_do");
  dt_ctl_t *ctl = &param->ctl;
  dtimage_t input = {0}, output = {0};	/* image structures					*/
  dt_rc_e rc = DT_RC_GOOD;

  DT_Q(dtimage_import,(ctl,				/* get input image					*/
    &input, input_name));

  param->max = DTPARAM_GET_VALUE(		/* default "max" param from image   */
    max, 
    dt_ntype_maxval[input.ntype]);
  param->min = DTPARAM_GET_VALUE(		/* default "min" param from image   */
    min, 
    dt_ntype_minval[input.ntype]);

  DT_Q(dtimage_create,(ctl,				/* get space for output image	    */
    &output, input.xe, input.ye,
    DT_NTYPE_B08));

  DT_Q(dt_flo76,(param, &input,			/* do the algorithm				    */
    &output));

  DT_Q(dtimage_export,(ctl,				/* put output image					*/
    &output, output_name,
    (dtxy_t)param->x,
    (dtxy_t)param->y));

  return rc;
}

/*..........................................................................*/

dt_rc_e 
dt_flo76_cmd_parse(						/* parse params for this program	*/
  dtparam_t *param,
  int argc,
  char *argv[],
  int *op,
  int *n)
{
  DT_F("dt_flo76_cmd_parse");
  dt_ctl_t *ctl = &param->ctl;
  dt_rc_e rc = DT_RC_GOOD;

  DT_G(dtparam_init,(param));			/* init param structure             */
                                        /* set default for params we want	*/
  DTPARAM_DEFAULT_P(param, dbg, 0);
  DTPARAM_DEFAULT_P(param, background, 0);
  DTPARAM_DEFAULT_P(param, foreground, 255);
  DTPARAM_DEFAULT_P(param, op,         "do");
  DTPARAM_DEFAULT_P(param, variant,    "easy");
  DTPARAM_DEFAULT_P(param, x,          DTIMAGE_NOPOS);
  DTPARAM_DEFAULT_P(param, y,          DTIMAGE_NOPOS);

  param->max_flag |= DTPARAM_FLAG_PARSE;
  param->min_flag |= DTPARAM_FLAG_PARSE;

  DT_GI(dt_interactive_parse,(			/* go parse args					*/
    argc, argv, "dt_flo76",
    USAGE, param, n));

  DT_GI(dtstr_choice,(ctl, ops,			/* make sure op is understood	    */
    param->op, op));

  if (rc == DT_RC_GOOD)
  if (*op == -1)
    DT_G(dt_err,(ctl, F,
      "unknown op \"%s\"", 
      param->op));
  if (*op < -1)
    DT_G(dt_err,(ctl, F,
      "ambiguous op \"%s\"",
      param->op));

  if (rc != DT_RC_GOOD)					/* failure during parsing?		    */
    DT_Q(dtparam_free,(param));			/* free param space					*/

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
