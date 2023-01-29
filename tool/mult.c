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
| dttoolmult utility - 
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

DT_RCSID("tool $RCSfile: mult.c,v $ $Revision: 1.6 $");

#define USAGE "[params] input scalar|image output"

static dt_rc_e parse(					/* parse params for this program	*/
  dtparam_t *param,
  int argc,
  char *argv[],
  int *n);

/*..........................................................................*/

DTTOOL_MAIN(dttool_mult)
{
  dtimage_t image1;						/* image structure					*/
  dtimage_t image2;
  
  dt_ctl_t *ctl = &param->ctl;
  double scalar;
  dt_rc_e rc;
  int n;

  DT_Q(parse,(param,					/* parse parameters				   	*/
     argc, argv, &n));

  argc -= n;
  argv += n;
  if (argc != 3 && argc != 4)			/* check positional args			*/
    DT_Q(dttool_usage,(param, USAGE));

  DT_Q(dtimage_import,(ctl,				/* get first input image			*/
    &image1, argv[0]));

  image2.xe = 0;
  if (argc == 3)					   	/* doing image * scalar -> image?	*/
  {
    DT_G(dtstr_to_double,(ctl,			/* get scalar						*/
      argv[1], &scalar));
    DT_GI(dtimage_arith,(ctl,			/* do scalar operation			    */
      &image1, scalar, 
      &image1, 0, 0,
      DT_OP_ADD, DT_ASSIGN_CAST,
      &image1));
  }
  else									/* doing image * image * scalar?	*/
  {
    DT_G(dtimage_import,(ctl,			/* get input2 as image				*/
      &image2, argv[1]));
    DT_GI(dtstr_to_double,(ctl,			/* get scalar						*/
      argv[2], &scalar));
    DT_GI(dtimage_arith,(ctl,			/* do image operation			    */
      &image1, scalar,
      &image2, 1, 0,
      DT_OP_MULT, DT_ASSIGN_CLIP,
      &image1));
  }

  DT_GI(dtimage_export,(ctl,			/* output resulting image			*/
    &image1, argv[argc-1], 
    DTIMAGE_NOPOS, DTIMAGE_NOPOS));

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
