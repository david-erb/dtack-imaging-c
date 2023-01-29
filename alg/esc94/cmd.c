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
| TITLE
| Algorithm: dt_esc94
| 
|!Usage
| Command line usage is:
|:dt_esc94 [params] input_image output_image
| 
| where params may be zero or more of:
| :-k:			algorithm constant k (default 3.0)
| :-lambda:		algorithm constant lambda (default 100.0)
| :-max:		???
| :-min:		???
| :-t:			???
| :-op:			operation to perform (default is "default")
| :-variant:	algorithm variant to use (default "fast")
| :-foreground:	color of "lighted" output pixels (default 255)
| :-background:	color of "darkened" output pixels (default 0)
| :-x:			output x position of image data
| :-y:			output y position of image data
|
| dtinclude(params1.dx)
| 
| The :-op: parameter may take one of the following values:
| :default:	perform algorithm on input to produce output
| :test:	perform implementation self-tests and display pass/fail result
| :time:	perform implementation benchmark and display execution times
|
| The :-variant: parameter may take one of the following values:
| :fast:	use fastest alternative
| :easy:	use most understandable alternative
| 
| The :test: and :time: ops may be exercised under varying memory
| conditions.
| The minimum conditions are:
| :-op test:		maxalloc 16384, pagesize 128, npages 1
| :-op time:		maxalloc 262144, pagesize 512, npages 1
|
| dtinclude(params2.dx)
|
| END
 *..........................................................................*/

#define DT_INMAIN

#include <dtack/base.h>
DT_RCSID("alg/esc94 $RCSfile: cmd.c,v $ $Revision: 1.2 $");
#include <dt_esc94.h>

#include <dtack/os.h>
#include <dtack/str.h>

static char *USAGE = "usage: [params] input_image output_image\n\
Implementation of algorithm by Eschbach, 1994.\n\
Binarizes input image using error diffusion.\n\
Output image always has same dimensions as input image.\n\
Output image pixels take one of two values: \n\
  param.foreground\n\
  param.background";

static const char *ops[] = {
# define OP_DEFAULT 0
  "default",
# define OP_TEST 1
  "test",
# define OP_TIME 2
  "time",
  NULL};

/*..........................................................................*/

dt_rc_e
dtesc94_cmd(
  int argc,
  char *argv[],
  dtparam_t *param)
{
  DT_F("dt_esc94_cmd");
  dt_ctl_t *ctl = &param->ctl;
# define ARG_MAXLEN 128
  char input[ARG_MAXLEN];
  char output[ARG_MAXLEN];
  dt_enter_arg_t arg[3];
  int op;
  dt_rc_e rc = DT_RC_GOOD;

#define INPUT_DESC "Source image"
#define INPUT_HELP NULL
#define OUTPUT_DESC "Destination image"
#define OUTPUT_HELP NULL

  DT_GI(dt_enter_arg,(ctl, arg,			/* set up positionals			    */
    INPUT_DESC, INPUT_HELP, 
    input, sizeof(input),
    OUTPUT_DESC, OUTPUT_HELP,
    output, sizeof(output),
    NULL));

  DT_GI(dt_esc94_cmd_parse,(param,		/* parse args					    */
    argc, argv, arg, &op));

  while (rc == DT_RC_GOOD)
  {
    switch(op)
    {
      case OP_DEFAULT:					/* ................................ */
        DT_GI(dt_esc94_cmd_default,		/* import, do algorithm, export		*/
          (param,
           input,
           output));
      break;
      case OP_TEST:						/* ................................ */
  	  {
        int assert_fail = 0;
        DT_GI(dt_esc94_test_all,(		/* do the operation				    */
          param, &assert_fail));
        if (rc == DT_RC_GOOD &&			/* test worked?					    */
            assert_fail)				/* but some results wrong?		    */
          rc = DT_RC_BAD;
  	  }
      break;
      case OP_TIME:						/* ................................ */
  	  {
        int assert_fail = 0;
        DT_GI(dt_esc94_time_all,(		/* do the operation				    */
          param, &assert_fail));
        if (rc == DT_RC_GOOD &&			/* test worked?					    */
            assert_fail)				/* but some results wrong?		    */
          rc = DT_RC_BAD;
  	  }
      break;
    }

    DT_G(dt_interactive_after,(			/* make post display				*/
      ctl, param, USAGE, arg,
      dt_esc94_cmd_check, NULL, rc));
  } 

  if (rc == DT_RC_STOP)
    rc = DT_RC_GOOD;

  DT_I(dt_interactive_leave,(			/* clean up before leaving			*/
    param, rc));

  DT_I(dtparam_free,(param));			/* free param space				    */

  return rc;
}

/*..........................................................................*/

dt_rc_e
dt_esc94_cmd_default(
  dtparam_t *param,
  char *input_name,
  char *output_name)
{
  DT_F("dt_esc94_cmd_default");
  dt_ctl_t *ctl = &param->ctl;
  dtimage_t input;						/* image structures					*/
  dtimage_t output;
  dtimage_t cropped;
  const char *precision =
    DTPARAM_GET_VALUE(precision, 
      "same");
  dtxy_t x0 = DT_CAST(dtxy_t,			/* image exported x position 		*/
    DTPARAM_GET_VALUE(x,				/* from -x arg 						*/
      DTIMAGE_NOPOS));					/* or use window system default 	*/
  dtxy_t y0 = DT_CAST(dtxy_t,			/* image exported y position 		*/
    DTPARAM_GET_VALUE(y,				/* from -y arg 						*/
      DTIMAGE_NOPOS));					/* or use window system default 	*/
  dt_ntype_e ntype;
  dt_rc_e rc;

  input.xe = 0;
  output.xe = 0;
  cropped.xe = 0;

  DT_C(dtimage_import,(ctl,				/* get input image					*/
    &input, input_name));

  if (strcmp(precision, "same"))
  {
	char message[80];
    DT_C(dt_ntype_lookup,(ctl,			/* get desired output precision	    */
      param->precision, &ntype,
      message, sizeof(message)));
    if (message[0])
	{
      rc = dt_err(ctl, F, "%s", message);
	  goto cleanup;
	}
  }
  else
    ntype = input.ntype;

  param->max = DTPARAM_GET_VALUE(		/* default "max" param from image   */
    max, 
    dt_ntype_maxval[input.ntype]);
  param->min = DTPARAM_GET_VALUE(		/* default "min" param from image   */
    min, 
    dt_ntype_minval[input.ntype]);
  param->t = DTPARAM_GET_VALUE(			/* default "thresh" param			*/
    t, 
    (param->max+param->min)/2.0);

  DT_C(dtimage_create,(ctl,				/* get space for output image	    */
    &output, input.xe, input.ye,
    DT_NTYPE_B08));

  DT_C(dt_esc94,(param, &input,			/* do the algorithm				    */
    &output));

  DT_C(dtimage_free,(ctl, &input));		/* free input image			    	*/
  input.xe = 0;

  if (ntype != output.ntype)			/* precision change specified?	    */
  {
	dtxy_t xe = ntype == DT_NTYPE_B01?
      output.xe & ~0x007: output.xe;
    DT_C(dtimage_create,(ctl,			/* make space for cropped image	    */
      &cropped, xe, output.ye, ntype));
    DT_C(dtimage_cast,(ctl, &output,	/* change precision				    */
      &cropped));
    DT_C(dtimage_free,(ctl, &output));	/* free output image				*/
	output = cropped;					/* used cropped as output 			*/
	cropped.xe = 0;						/* don't free this again 			*/
  }

  DT_C(dtimage_export,(ctl,				/* put output image					*/
    &output, output_name, x0, y0));
  
cleanup:
  if (cropped.xe)
    DT_I(dtimage_free,(ctl, &cropped));
  if (output.xe)
    DT_I(dtimage_free,(ctl, &output));
  if (input.xe)
    DT_I(dtimage_free,(ctl, &input));

  return rc;
}

/*..........................................................................*/

dt_rc_e 
dt_esc94_cmd_check(						/* check params for this program	*/
  dt_ctl_t *ctl,
  void *app,
  dtparam_t *param,
  dt_enter_arg_t *arg,
  char *message,
  const char **start)
{
  DT_F("dt_esc94_cmd_check");
  int op;
  int i;
  
  if (param->precision != NULL)
  {
	dt_ntype_e ntype;
    DT_Q(dt_ntype_lookup,(ctl,			/* get desired output precision	    */
      param->precision, &ntype,
      message, DT_ENTER_MSG_MAXLEN));
	if (message[0] != '\0')				/* bad precision? 					*/
	  *start = param->precision;
	return DT_RC_GOOD;
  }

  DT_Q(dtstr_choice,(ctl, ops,			/* make sure op is understood	    */
    param->op, &op));

  if (op == -1)
  {
    DT_Q(dtstr_printf,(ctl,
      message, DT_ENTER_MSG_MAXLEN,
      "Sorry, op \"%s\" is not understood", 
      param->op));
    *start = param->op_desc;			/* put cursor at this field		    */
    return DT_RC_GOOD;
  }

  if (op < -1)
  {
    DT_Q(dtstr_printf,(ctl,
      message, DT_ENTER_MSG_MAXLEN,
      "Sorry, op \"%s\" is ambiguous",
      param->op));
    *start = param->op_desc;			/* put cursor at this field		    */
    return DT_RC_GOOD;
  }

  if (op != OP_TEST &&					/* test and time have no args	    */
      op != OP_TIME)
  for (i=0; arg[i].desc; i++)			/* check all args for a value	    */
  {
    int l = strlen(arg[i].val);
    int j;

    for (j=0; j<l; j++)					/* search value for non-blank	    */
      if (arg[i].val[j] != ' ')
        break;
    if (j == l)
    {
      DT_Q(dtstr_printf,(ctl,
        message, DT_ENTER_MSG_MAXLEN,
        "Please enter something for \"%s\"",
        arg[i].desc));
      *start = arg[i].val;				/* position cursor on this field    */
      return DT_RC_GOOD;
    }
  }

  return DT_RC_GOOD;
}

/*..........................................................................*/

dt_rc_e 
dt_esc94_cmd_parse(						/* parse params for this program	*/
  dtparam_t *param,
  int argc,
  char *argv[],
  dt_enter_arg_t *arg,
  int *op)
{
  DT_F("dt_esc94_cmd_parse");
  dt_ctl_t *ctl = &param->ctl;
  dt_rc_e rc = DT_RC_GOOD;

  DT_GI(dtparam_init,(param));			/* init param structure             */


  DT_GI(dtparam_set,(param,				/* set default for params we want	*/
    DTPARAM_FLAG_PRIMARY,
    &param->op,         NULL, NULL, "default",
    &param->variant,    NULL, NULL, "fast",
    &param->k,          NULL, NULL, "3.0",
    &param->lambda,     NULL, NULL, "100.0",
    &param->min,        NULL, NULL, NULL,
    &param->max,        NULL, NULL, NULL,
    &param->t,          NULL, NULL, NULL,
    NULL));

  DT_GI(dtparam_set,(param, 
    DTPARAM_FLAG_SECONDARY,
    &param->background, NULL, NULL, "0",
    &param->foreground, NULL, NULL, "255",
    &param->x,          NULL, NULL, NULL,
    &param->y,          NULL, NULL, NULL,
    &param->precision,  NULL, NULL, NULL,
    NULL));

  DT_GI(dtparam_set,(param, 
    DTPARAM_FLAG_TERTIARY,
    &param->datadir, NULL, NULL, "./data",
    NULL));

  DT_GI(dt_interactive_parse,(			/* parse command line interactively	*/
    argc, argv, "dtesc94", USAGE, param,
    arg, dt_esc94_cmd_check, NULL));

  DT_Q(dtstr_choice,(ctl, ops,			/* make sure op is understood	    */
    param->op, op));

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
