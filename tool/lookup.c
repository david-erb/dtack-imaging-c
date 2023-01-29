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
| dttool_lookup utility - tell win server to stop
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

DT_RCSID("tool $RCSfile: lookup.c,v $ $Revision: 1.9 $");

#include <dtack/lut8.h>

#define USAGE "[params], -win param specifies win server"
static const char *usage = USAGE;

/*..........................................................................*/

dt_rc_e 
DTCONFIG_API1
dttool_lookup_cmd_check(				/* check params for this program	*/
  dt_ctl_t *ctl,
  void *appstruct,
  dtparam_t *param,
  dt_enter_arg_t *arg,
  char *msg,
  const char **start)
{
  DT_F("dttool_lookup_cmd_check");
  int lindex;

  DT_Q(dtstr_choice,(ctl,				/* make sure op is understood	    */
    dtlut8_lutnames,
    arg[1].val, &lindex));

  if (lindex == -1)
  {
    sprintf(msg,
      "Sorry, lut \"%s\""
      " is not one of the known names", 
      arg[1].val);
    *start = arg[1].desc;				/* put cursor at this field		    */
    return DT_RC_GOOD;
  }

  if (lindex < -1)
  {
    sprintf(msg,
      "Sorry, lut \"%s\" is ambiguous",
      arg[1].val);
    *start = arg[1].desc;				/* put cursor at this field		    */
    return DT_RC_GOOD;
  }

  return DT_RC_GOOD;
}

/*..........................................................................*/

DTTOOL_MAIN(dttool_lookup)
{
  dt_ctl_t *ctl = &param->ctl;
# define ARG_MAXLEN 128
  char input_name[ARG_MAXLEN];
  char lut_name[ARG_MAXLEN];
  char output_name[ARG_MAXLEN];
  dt_enter_arg_t arg[4];
  dtimage_t image;
  dt_rc_e rc = DT_RC_GOOD;
  image.xe = 0;

  DT_GI(dtparam_init,(param));			/* init param structure             */

  DT_GI(dtparam_set,(param, 
    DTPARAM_FLAG_PRIMARY,
    NULL));

  DT_GI(dt_enter_arg,(ctl,				/* set up positionals			    */
    arg,
    "Input image", NULL, 
      input_name, sizeof(input_name),
    "Lut name", NULL, 
      lut_name, sizeof(lut_name),
    "Output image", NULL, 
      output_name, sizeof(output_name),
    NULL));
  
  DT_GI(dt_interactive_parse,(			/* parse command line interactively	*/
    argc, argv, F, usage, param,
    arg, dttool_lookup_cmd_check, NULL));
										/* ................................ */
  while (rc == DT_RC_GOOD)
  {
	int lindex;

	DT_GI(dtstr_choice,(ctl,			/* get the lut by its name 			*/
	  dtlut8_lutnames,
      lut_name, &lindex));

	DT_GI(dtimage_import,(ctl,			/* get image from file or window	*/
      &image, input_name));
	DT_GI(dtimage_lookup_b08,(ctl,		/* do lookup in place 				*/
      &image, dtlut8_luts[lindex],
      256, &image));

    DT_GI(dtimage_export,(ctl,			/* output resulting image			*/
      &image, output_name, 
      DTIMAGE_NOPOS, DTIMAGE_NOPOS));

	DT_GI(dtimage_free,(ctl, &image));
	image.xe = 0;
	
    DT_G(dt_interactive_after,(			/* make post display				*/
      ctl, param, usage, arg,
      dttool_lookup_cmd_check, NULL, rc));
  } 

  if (image.xe)
	DT_I(dtimage_free,(ctl, &image));

  if (rc == DT_RC_STOP)
    rc = DT_RC_GOOD;

  DT_I(dt_interactive_leave,(			/* clean up before leaving			*/
    param, rc));

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
