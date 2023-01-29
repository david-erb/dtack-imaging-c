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
| dttool find utility - 
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
#include <dtack/mem.h>

DT_RCSID("tool $RCSfile: find.c,v $ $Revision: 1.6 $");

#define USAGE "[params] input want"

/*..........................................................................*/

DTTOOL_MAIN(dttool_find)
{
  dt_ctl_t *ctl = &param->ctl;
  dtimage_t input;						/* image structure					*/
  int n;
  double want;
  dtxy_t x, y;
  dt_rc_e rc;
  
  input.xe = 0;

  DT_Q(dtparam_init,(param));			/* init param structure             */

  DT_C(dtparam_set,(param, 
    DTPARAM_FLAG_PRIMARY,
    &param->precision,  NULL, NULL, "b08",
    &param->min,  NULL, NULL, NULL,
    &param->max,  NULL, NULL, NULL,
    &param->x,  NULL, NULL, NULL,
    &param->y,  NULL, NULL, NULL,
    NULL));

  DT_C(dttool_parse,(param, F, USAGE,	/* go parse args					*/
    argc, argv, &n));

  argc -= n;
  argv += n;
  if (rc == DT_RC_GOOD)
  if (argc != 2)						/* check positional args			*/
    DT_C(dttool_usage,(param, USAGE));

  DT_C(dtimage_import,(ctl,				/* get input image					*/
    &input, argv[0]));

  DT_C(dtstr_to_double,(ctl,
    argv[1], &want));

  for (y=0; y<input.ye; y++)
    for (x=0; x<input.xe; x++)
	{
	  double v;
	  DTIMAGE_GETPIX1(DT_C, ctl,
        &input, x, y, &v);
	  if (v == want)
        dt_dbg(ctl, F, DT_DBG_MASK_ANY,
          "x= %5u  y=%5u  v= %g",
          x, y, v);
	}

cleanup:
  if (input.xe)							/* input image got allocated?	    */
    DT_I(dtimage_free,(ctl,				/* free input image					*/
      &input));

  DT_I(dtparam_free,(param));			/* free param space					*/

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
