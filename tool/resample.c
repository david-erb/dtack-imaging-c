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
| dttoolresample utility - 
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

DT_RCSID("tool $RCSfile: resample.c,v $ $Revision: 1.7 $");

#define USAGE "[params] input dx dy output"

/*..........................................................................*/

DTTOOL_MAIN(dttool_resample)
{
  dtimage_t input, output;				/* image structures					*/
  
  dt_ctl_t *ctl = &param->ctl;
  char *arg_input;
  char *arg_output;
  char *arg_dx, *arg_dy;
  double dx, dy;
  int n;
  dt_rc_e rc = DT_RC_GOOD;

  DT_Q(dtparam_init,(param));			/* init param structure             */

  DTPARAM_DEFAULT_P(param, dbg, 0);	/* set default for params we want	*/
  DTPARAM_DEFAULT_P(param, op,
    "nearest");

  DT_GI(dt_enter_param,(					/* go parse args					*/
    argc, argv, F, USAGE, param, &n));

  argc -= n;
  argv += n;
  if (rc == DT_RC_GOOD)
  if (argc != 4)						/* check positional args			*/
    DT_GI(dt_usage_say,(
      param, USAGE));

  arg_input = argv[0];					/* get positionals				    */
  arg_dx = argv[1];
  arg_dy = argv[2];
  arg_output = argv[3];

  DT_GI(dtstr_to_double,(ctl, arg_dx,	/* convert numeric positionals	    */
    &dx));
  DT_GI(dtstr_to_double,(ctl, arg_dy,
    &dy));

  input.xe = 0;
  DT_GI(dtimage_import,(ctl,			/* get input image					*/
    &input, arg_input));

  output.xe = 0;
  DT_GI(dtimage_resample,(ctl,			/* resample output from input		*/
    &input, dx, dy,
    param->op, &output));

  DT_GI(dtimage_export,(ctl,			/* output resulting image			*/
    &output, arg_output, DTIMAGE_NOPOS, DTIMAGE_NOPOS));

  if (output.xe != 0)
  DT_G(dtimage_free,(ctl,				/* free image						*/
    &output));
  if (input.xe != 0)
  DT_G(dtimage_free,(ctl,			   
    &input));

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
