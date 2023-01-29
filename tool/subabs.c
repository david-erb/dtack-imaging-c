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
| dttoolsubabs utility - 
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

DT_RCSID("tool $RCSfile: subabs.c,v $ $Revision: 1.7 $");

#define USAGE "[params] input scalar|image output"

static dt_rc_e parse(					/* parse params for this program	*/
  dtparam_t *param,
  int argc,
  char *argv[],
  int *n);

/*..........................................................................*/

DTTOOL_MAIN(dttool_subabs)
{
  dtimage_t image1;						/* image structure					*/
  dtimage_t image2;
  
  dt_ctl_t *ctl = &param->ctl;
  char *arg_input1;
  char *arg_input2;
  char *arg_output;
  double scalar;
  int isnumber;
  dt_rc_e rc;
  int n;

  DT_Q(parse,(param,					/* parse parameters				   	*/
     argc, argv, &n));

  argc -= n;
  argv += n;
  if (argc != 3)						/* check positional args			*/
    DT_Q(dttool_usage,(param, USAGE));

  arg_input1 = argv[0];					/* get positionals				    */
  arg_input2 = argv[1];
  arg_output = argv[2];

  DT_Q(dtimage_import,(ctl,			/* get first input image			*/
    &image1, arg_input1));

  dtstr_to_double2(ctl,					/* try to convert input2 to number	*/
    arg_input2, &scalar, &isnumber);

  image2.xe = 0;
  if (isnumber)							/* input2 is a number?			    */
  {
    DT_G(dtimage_arith,(ctl,			/* do scalar operation			    */
      &image1, 1, 
      &image1, 0, -scalar, 
      DT_OP_ADD, 
      DT_ASSIGN_ABS|DT_ASSIGN_CLIP,
      &image1));
  }
  else									/* input2 is not a number?		    */
  {
    DT_G(dtimage_import,(ctl,			/* get input2 as image				*/
      &image2, arg_input2));
    DT_GI(dtimage_arith,(ctl,			/* do scalar operation			    */
      &image1, 1, 
      &image2, -1, 0,
      DT_OP_ADD, 
      DT_ASSIGN_ABS|DT_ASSIGN_CLIP,
      &image1));
  }

  DT_GI(dtimage_export,(ctl,			/* output resulting image			*/
    &image1, arg_output, 
    (dtxy_t)param->x,
    (dtxy_t)param->y));

  if (image2.xe)						/* second image got allocated?	    */
    DT_I(dtimage_free,(ctl,				/* free second image				*/
      &image2));

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
										
										/* set default for params we want	*/
  DTPARAM_DEFAULT(dbg, 0);
  DTPARAM_DEFAULT(x, DTIMAGE_NOPOS);
  DTPARAM_DEFAULT(y, DTIMAGE_NOPOS);

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
