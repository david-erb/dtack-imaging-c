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
| USAGE
| dttool - image subcommand dispatch
| 
|.SYNOPSIS
|:dttool subcommand [parameters] [arguments]
| 
|.SUMMARY
| Call ~subcommand~, giving it ~parameters~ and ~arguments~.
| The expected and allowed values for these differ by subcommand.
| 
|.ARGUMENTS
| ~subcommand~	name of the subcommand to be executed
| 
|.DESCRIPTION
| Call the appropriate function indicated by ~subcommand~.
| Unambiguous abbreviation is allowed.
| Allowed subcommands are:
| 
|:image transformation					neighborhood operators
|:--------------------                  ----------------------
|:copy          copy file or window		convolve      convolve image
|:paste         paste image on another	morph         morphology
|:resample      change image size		median        median filter
|:gen           generate image
|:
|:annotation							measurement
|:----------                            -----------
|:text          draw text in image		stats         image statistics
|:box           overlay box on image	snr           signal-to-noise ratio
|:enclose       enclose blobs in boxes	
|:profile       plot profile			miscellaneous
|:sum           plot row or column sum	-------------
|:run           plot row or column run	color         color space conversion
|:hist          plot histogram			cooc          co-occurrence matrix
|:										winstop       tell win server to stop
|:point operators						find          find pixel value in image
|:---------------						lut           query/set lut
|:thresh        threshold				
|:multithresh   multiple threshold
|:slice         gray level slice
|:logical       logical and/or/xor
|:mult          multiply
|:subabs        subtract and abs value
|:linear        linear combination
|:regray        remap dynamic range
|:lookup        lookup combination
|  
|.RETURN VALUES
|dtinclude(return2.dx)
| 
|.ERRORS
| - ~subcommand~ not found or ambiguous
| -	error executing ~subcommand~
| 
| END
 *..........................................................................*/

#include <dttool.h>
DT_RCSID("tool $RCSfile: main.c,v $ $Revision: 1.6 $");
#include <dtack/os.h>
#include <dttool.use>

#define ACTIONS \
  ACTION(copy) \
  ACTION(paste) \
  ACTION(resample) \
  ACTION(gen) \
  ACTION(text) \
  ACTION(box) \
  ACTION(enclose) \
  ACTION(profile) \
  ACTION(sum) \
  ACTION(run) \
  ACTION(hist) \
  ACTION(convolve) \
  ACTION(morph) \
  ACTION(fft) \
  ACTION(median) \
  ACTION(stats) \
  ACTION(snr) \
  ACTION(thresh) \
  ACTION(multithresh) \
  ACTION(slice) \
  ACTION(logical) \
  ACTION(mult) \
  ACTION(subabs) \
  ACTION(linear) \
  ACTION(nonlin) \
  ACTION(regray) \
  ACTION(lookup) \
  ACTION(color) \
  ACTION(cooc) \
  ACTION(winstop) \
  ACTION(find) \
  ACTION(lut)

#define ACTION(A) extern dtmain_f dttool_ ## A ## _cmd;
ACTIONS
#undef ACTION

#define ACTION(A) dttool_ ## A ## _cmd,
static dtmain_f *funcs[] = {
  ACTIONS
  NULL
};
#undef ACTION

#define ACTION(A) #A,
static const char *names[] = {
  ACTIONS
  NULL
};
#undef ACTION


/*..........................................................................*/

DTMAIN1(dttool_main)
{
  DT_F("dttool");
  dt_ctl_t *ctl = &param->ctl;
  int n;
  int choice;
  char message[80];
  dt_rc_e rc = DT_RC_GOOD;

  DT_GI(dt_heapmon_init,(ctl));			/* init memory checking			    */
  
  DT_GI(dtparam_init,(param));			/* init param structure             */

  DT_GI(dt_enter_param,(				/* parse params on command line	    */
    argc, argv, F, usage,
    param, &n));

  argc -= n;							/* skip the ones we used 			*/
  argv += n;

  if (argc == 0)
  {
    DT_GI(dtstr_printf,(ctl,
      message, sizeof(message),
      "Please give a subcommand."
      "  Try -help or -HELP."));
  }
  else
  {
    DT_GI(dtstr_choice2,(ctl, names,	/* find program name 				*/
      argv[0], &choice, 
      message, sizeof(message)));
  }

  if (rc == DT_RC_GOOD &&
	  message[0] != '\0')				/* program name failure? 			*/
    rc = dt_err(ctl, F, 
      "%s", message);
  
  DT_GI((*funcs[choice]),(argc, argv,	/* call the program 				*/
    param));

  if (rc == DT_RC_STOP)
    rc = DT_RC_GOOD;

  DT_I(dt_leave,(param));				/* clean up before leaving			*/

  DT_I(dtparam_free,(param));			/* free param space					*/

  {
	int check_fail = 0;
    DT_I(dt_heapmon_uninit,(ctl,		/* check all memory is freed        */
      &check_fail));
    if (check_fail)						/* any failed assertions?           */
      rc = DT_RC_BAD;
    else
      rc = DT_RC_GOOD;
  }

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
