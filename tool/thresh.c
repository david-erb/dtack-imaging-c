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
| dttoolthresh utility - simple constant bilevel threshold
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

DT_RCSID("tool $RCSfile: thresh.c,v $ $Revision: 1.5 $");

#define USAGE "[params] source scalar|image dest"

/*..........................................................................*/

DTTOOL_MAIN(dttool_thresh)
{
  dt_ctl_t *ctl = &param->ctl;
  dtimage_t image1;						/* image structure					*/
  dtimage_t image2;
  char *arg_input1;
  char *arg_input2;
  char *arg_output;
  double scalar;
  int isnumber;
  dt_rc_e rc;
  int n;
										/* -------------------------------- */
  DT_G(dtparam_init,(param));			/* init param structure             */
  DTPARAM_DEFAULT_P(param, dbg, 0);	/* set default for params we want	*/
  DTPARAM_DEFAULT_P(param,
    foreground, 255);	
  DTPARAM_DEFAULT_P(param,
    background, 0);	
  DT_GI(dt_enter_param,(					/* go parse args					*/
    argc, argv, F, USAGE, param, &n));

  argc -= n;
  argv += n;
  if (rc == DT_RC_GOOD)
  if (argc != 3)						/* check positional args			*/
    DT_Q(dt_usage_say,(
      param, USAGE));

  arg_input1 = argv[0];					/* get positionals				    */
  arg_input2 = argv[1];
  arg_output = argv[2];

  DT_GI(dtimage_import,(ctl,			/* get first input image			*/
    &image1, arg_input1));

  DT_GI(dtstr_to_double2,(ctl,			/* try to convert input2 to number	*/
    arg_input2, &scalar, &isnumber));

  image2.xe = 0;
  if (isnumber)							/* input2 is a number?			    */
  {
    DT_GI(dtimage_thresh_scalar,(ctl,	/* do scalar operation			    */
      &image1, scalar,
      param->background,
      param->foreground,
      &image1));
  }
  else									/* input2 is not a number?		    */
  {
    DT_GI(dtimage_import,(ctl,			/* get input2 as image				*/
      &image2, arg_input2));
    DT_GI(dtimage_thresh_image,(ctl,	/* do image operation			    */
      &image1, &image2,
      param->background,
      param->foreground,
      &image1));
  }

  DT_GI(dtimage_export,(ctl,			/* output resulting image			*/
    &image1, arg_output, 
    DTIMAGE_NOPOS, DTIMAGE_NOPOS));

  if (image2.xe)						/* second image got allocated?	    */
  DT_Q(dtimage_free,(ctl,				/* free second image				*/
    &image2));

  if (image1.xe)
  DT_Q(dtimage_free,(ctl,				/* free first image					*/
    &image1));

  DT_Q(dtparam_free,(param));			/* free param space					*/

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
