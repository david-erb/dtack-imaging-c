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
| dttoollinear utility -
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

DT_RCSID("tool $RCSfile: linear.c,v $ $Revision: 1.5 $");

#define USAGE "[params] input1 a [input2 b] scalar output"

static dt_rc_e parse(					/* parse params for this program	*/
  dtparam_t *param,
  int argc,
  char *argv[],
  int *n);

/*..........................................................................*/

DTTOOL_MAIN(dttool_linear)
{
  dt_ctl_t *ctl = &param->ctl;
  dtimage_t image1;						/* image structure					*/
  dtimage_t image2;
  dtimage_t output;  
  double a;
  double b;
  double scalar;
  char *arg_output;
  dt_ntype_e ntype;
  dt_rc_e rc;
  int n;
  
  image1.xe = 0;
  image2.xe = 0;
  output.xe = 0;

  DT_C(parse,(param,					/* parse parameters				   	*/
     argc, argv, &n));

  argc -= n;
  argv += n;
  if (argc != 4 && argc != 6)			/* check positional args			*/
    DT_C(dttool_usage,(param, USAGE));

  DT_C(dtimage_import,(ctl,				/* get first input image			*/
    &image1, argv[0]));
  DT_C(dtstr_to_double,(ctl,			/* image1 multiplier				*/
    argv[1], &a));

  if (strcmp(param->precision, "same"))
  {
	char message[80];
    DT_C(dtntype_lookup,(ctl,			/* get desired output precision	    */
      param->precision, &ntype,
      message, sizeof(message)));
	if (message[0])
	{
      rc = dt_err(ctl, F, "%s", message);
	  goto cleanup;
	}
  }
  else
    ntype = image1.ntype;

  image2.xe = 0;
  if (argc == 4)						/* no second image?					*/
  {
    DT_C(dtstr_to_double,(ctl,			/* scalar offset					*/
      argv[2], &scalar));
    DT_C(dtimage_create,(ctl,			/* space for output image		    */
      &output, image1.xe, image1.ye,
      ntype));
    DT_C(dtimage_arith,(ctl,			/* do scalar operation			    */
      &image1, a, 
      &image1, 0, scalar, 
      DT_OP_ADD, DT_ASSIGN_CLIP,
      &output));
    arg_output = argv[3];
  }
  else									/* input2 is not a number?		    */
  {
    DT_G(dtimage_import,(ctl,			/* get input2 as image				*/
      &image2, argv[2]));
    DT_C(dtstr_to_double,(ctl,			/* image2 multiplier				*/
      argv[3], &b));
    DT_C(dtstr_to_double,(ctl,			/* scalar offset					*/
      argv[4], &scalar));
    DT_C(dtimage_create,(ctl,			/* space for output image		    */
      &output, image1.xe, image1.ye,
      ntype));
    DT_C(dtimage_arith,(ctl,			/* do image operation			    */
      &image1, a, 
      &image2, b, scalar,
      DT_OP_ADD, DT_ASSIGN_CLIP,
      &output));
    arg_output = argv[5];
  }

  DT_C(dtimage_export,(ctl,				/* output resulting image			*/
    &output, arg_output, 
    DTIMAGE_NOPOS, DTIMAGE_NOPOS));

cleanup:
  if (output.xe)						/* output image got allocated?	    */
    DT_I(dtimage_free,(ctl,				/* free output image				*/
      &output));

  if (image2.xe)						/* second image got allocated?	    */
    DT_I(dtimage_free,(ctl,				/* free second image				*/
      &image2));

  if (image1.xe)						/* first image got allocated?	    */
    DT_I(dtimage_free,(ctl,				/* free first image					*/
      &image1));

  DT_I(dtparam_free,(param));			/* free param space					*/

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
										

  DT_Q(dtparam_set,(param, 
    DTPARAM_FLAG_PRIMARY,
    &param->precision,  NULL, NULL, "same",
    NULL));

  DT_Q(dttool_parse,(param, F, USAGE,	/* go parse args					*/
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
