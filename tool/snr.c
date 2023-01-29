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
| dttoolor utility -
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

DT_RCSID("tool $RCSfile: snr.c,v $ $Revision: 1.4 $");

#define USAGE "[params] signal_image noise_image"

static dt_rc_e parse(					/* parse params for this program	*/
  dtparam_t *param,
  int argc,
  char *argv[],
  int *n);

/*..........................................................................*/

DTTOOL_MAIN(dttool_snr)
{
  dtimage_t signal;						/* image structure					*/
  dtimage_t noise;
  
  dt_ctl_t *ctl = &param->ctl;
  double sumsq_signal;
  double sumsq_diffs;
  dt_rc_e rc = DT_RC_GOOD;
  int n;

  DT_Q(parse,(param, argc, argv, &n));	/* parse parameters				   	*/
   

  argc -= n;
  argv += n;
  if (rc == DT_RC_GOOD)
  if (argc != 2)						/* check positional args			*/
    DT_G(dttool_usage,(param, USAGE));

  signal.xe = 0;
  DT_GI(dtimage_import,(ctl,			/* get first input image			*/
    &signal, argv[0]));

  noise.xe = 0;
  DT_GI(dtimage_import,(ctl,			/* get second input image			*/
    &noise, argv[1]));
 
  DT_GI(dtimage_sumsq,(ctl,
    &signal, &sumsq_signal));
  DT_GI(dtimage_sumsq_diffs,(ctl,
    &signal, &noise, &sumsq_diffs));

  if (rc == DT_RC_GOOD)
  {
    dt_say(ctl, F, "SNR: %e dB",
      10.0 *
      log(sumsq_signal / sumsq_diffs));
  }

  if (noise.xe)							/* second image got allocated?	    */
    dtimage_free(ctl, &noise);			/* free second image				*/

  if (signal.xe)						/* first image got allocated?	    */
    dtimage_free(ctl, &signal);			/* free first image					*/

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
