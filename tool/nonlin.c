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
| dttool_nonlin utility -
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

#include <dttool.h>

DT_RCSID("tool $RCSfile: nonlin.c,v $ $Revision: 1.9 $");

#define USAGE "[params] input op scalar|image output"

static dt_rc_e parse(					/* parse params for this program	*/
  dtparam_t *param,
  int argc,
  char *argv[],
  int *n);

/*..........................................................................*/

DTTOOL_MAIN(dttool_nonlin)
{
  dt_ctl_t *ctl = &param->ctl;
  char *arg_input;
  char *arg_op;
  char *arg_output;
  dtimage_t input;
  dtimage_t output;	
  double scale = 1.0;
  double offset = 0.0;
  dt_rc_e rc = DT_RC_GOOD;
  int n;

  input.xe = 0;
  output.xe = 0;

  DT_C(parse,(param,					/* parse parameters				   	*/
     argc, argv, &n));

  argc -= n;
  argv += n;
  if (argc != 3)						/* check positional args			*/
    DT_C(dttool_usage,(param, USAGE));

  arg_input = argv[0];
  arg_op = argv[1];	
  arg_output = argv[2];

  DT_C(dtimage_import,(ctl,				/* get input image					*/
    &input, arg_input));

  DT_C(dtimage_create,(ctl, &output,	/* space for output image 			*/
    input.xe, input.ye, DTNTYPE_F2));

# define CASE1(NAME)					\
  if (!strcmp(arg_op, #NAME))			\
      DT_C(dtimage_ ## NAME,(ctl,		\
        &input, 1.0, 0.0, &output));

# define CASE2(NAME)					\
  if (!strcmp(arg_op, #NAME))			\
      DT_C(dtimage_ ## NAME,(ctl,		\
        &input, &output));

  CASE1(log10)
  CASE1(log)
  CASE1(sqrt)
  CASE1(cos)
  CASE1(sin)
  CASE2(abs)

  DT_C(dtimage_export,(ctl,				/* output resulting image			*/
    &output, arg_output, 
    DTIMAGE_NOPOS, DTIMAGE_NOPOS));

cleanup:
  if (output.xe)				
    dtimage_free(ctl, &output);
  if (input.xe)					
    dtimage_free(ctl, &input);	

  DT_Q(dtparam_free,(param));			/* free param space					*/

  return rc;
}

/*..........................................................................*/

static dt_rc_e parse(					/* parse params for this program	*/
  dtparam_t *param,
  int argc,
  char *argv[],
  int *n)
{
  dt_ctl_t *ctl = &param->ctl;

  DT_Q(dtparam_init,(param));			/* init param structure             */
										
										/* set default for params we want	*/
  DTPARAM_DEFAULT(dbg, 0);

  DT_Q(dttool_parse,(param, F, USAGE,		/* go parse args					*/
    argc, argv, n));

  return DT_RC_GOOD;
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
