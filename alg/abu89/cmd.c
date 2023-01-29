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
| dt_abu89 command - Levine&Nazif 1985
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

static char rcsid[] = "util $RCSfile: cmd.c,v $ $Revision: 1.1 $";

#include <dtack/base.h>
#include <dt_abu89.h>

static dt_rc_e local_parse(				/* parse params for this program	*/
  dtparam_t *param,
  int argc,
  char *argv[],
  int *n);

#define USAGE "[params] image [dest]"

/*..........................................................................*/

static char *F = "dt_abu89_cmd";

int
dt_abu89_cmd(
  int argc,
  char *argv[])
{
  dtimage_t image;
  dtparam_t param;						/* parameter structure				*/
  dt_ctl_t *ctl = &param.ctl;
  int n;
  double thresh;
  dt_rc_e rc = DT_RC_GOOD;

  DT_Q(local_parse,(&param,				/* parse parameters				   	*/
    argc, argv, &n));

  argc -= n;
  argv += n;

  if (rc == DT_RC_GOOD)  
  if (argc < 1 || argc > 2)				/* check remaining args				*/
    return dt_interactive_usage(
      &param, USAGE);

  image.xe = 0;
  DT_GI(dtimage_import,(ctl,			/* get input image					*/
    &image, argv[0]));

  DT_GI(dt_abu89,(&param,				/* compute threshold				*/
    &image,  NULL, NULL, &thresh));

  if (rc == DT_RC_GOOD)  
  dt_say(ctl, F,						/* print threshold				    */
    "thresh %0.1f", thresh);

  if (rc == DT_RC_GOOD)  
  if (argc >= 2)
  {
    DT_GI(dtimage_thresh_scalar,(ctl,
      &image, thresh, 0, 255, &image));

    DT_GI(dtimage_export,(ctl,
      &image, argv[1], -1, -1));
  }

  if (image.xe != 0)
    dtimage_free(ctl, &image);

  DT_Q(dtparam_free,(&param));			/* free param space					*/

  return DT_RC_GOOD;
}

/*..........................................................................*/

static dt_rc_e local_parse(				/* parse params for this program	*/
  dtparam_t *param,
  int argc,
  char *argv[],
  int *n)
{
  dt_ctl_t *ctl = &param->ctl;
  dt_rc_e rc;

  DT_Q(dtparam_init,(param));			/* init param structure             */

										/* set default for params we want	*/
  DTPARAM_DEFAULT(dbg, 0);

  DT_Q(dt_interactive_parse,(			/* go parse args					*/
    argc, argv,
    F, USAGE, param, n));

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
