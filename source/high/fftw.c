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
  CHANGES
  19991219 DE added yields
  19981015 DE check for segmented output
  19980712 DE PRJ_FFTW_DEFINE removed in favor of $(MK_FFTW_DEFINE)
 *..........................................................................*/


#include <dtack/base.h>
DT_RCSID("source/high $RCSfile: fftw.c,v $ $Revision: 1.1 $");

#if DTPRJ_FFTW == DT_1

#ifndef DTDEPEND
#include <fftw.h>
#endif

/*..........................................................................
| NAME
| dtimage_fftw_2d() - compute 2D FFT using FFTW package
| 
| SUMMARY
| Compute 2D FFT of real-valued ~input~ and return result as ~output~.
| Allocate space for ~output~ if it is not preallocated.
| Have an error if ~output~ is preallocated but does not have the 
| following attributes: non-paged, contiguous, and ntype :DTNTYPE_F2:.
| 
| END
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtimage_fftw_2d(						/* compute 2D FFT					*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtimage_t *input,						/* input image 						*/
  int isreal,							/* true if input is real valued		*/
  int forward,							/* true if forward transform 		*/
  dtimage_t *output)					/* returned frequency domain image	*/
{
  DT_F("dtimage_fftw_2d");
  fftwnd_plan plan = NULL;
  dt_ntype_e ntypes[] = {				/* we can handle this type			*/
      DT1D_NTYPE_F2,
      DT1D_NTYPE_NULL};
  int output_allocated = 0;
  FFTW_COMPLEX *complex;
  dt_rc_e rc;

  if (output->xe == 0)					/* output is empty? 				*/
  {
	DT_C(dtimage_create,(ctl,			/* allocate space for the output 	*/
      output, 
      input->xe*(isreal? 2: 1),
      input->ye,
      DTNTYPE_F2));
	output_allocated = 1;
  }

  rc = dtimage_check_1(ctl,				/* check simple stuff				*/
    output, "output",
    ntypes, F);
  if (rc != DT_RC_GOOD)
    return rc;

  if (!(input->flags & DTIMAGE_FLAG_CONTIG))
	DT_CLEANUP((ctl, F, "input is not contiguous"));
  if (!(output->flags & DTIMAGE_FLAG_CONTIG))
	DT_CLEANUP((ctl, F, "output is not contiguous"));
  if (output->flags & DTIMAGE_FLAG_SEGMENTED)
	DT_CLEANUP((ctl, F, "output is segmented"));

  complex = (FFTW_COMPLEX *)			/* direct access to space			*/
    output->row[0].dvoid;

  DT_C(dt_yield,(ctl, 
    "created FFTW plan"));

  plan = fftw2d_create_plan_specific(	/* create FFTW plan 				*/
    output->xe/2, output->ye,
    forward? 
  	  FFTW_FORWARD: FFTW_BACKWARD,
    FFTW_ESTIMATE | FFTW_IN_PLACE,
    complex, 1,							/* the array we will be using 		*/
    NULL, 0);							/* ignored for in-place 			*/
  if (plan == NULL)
  {
	rc = dt_err(ctl, F,
       "fftw2d_create_plan_specific()"
       " failed");
	goto cleanup;
  }

  if (isreal)							/* input is real valued? 			*/
  {
    DT_C(dtimage_constant,(ctl, output,	/* make sure imaginary is clear		*/
      0.0));
    DT_C(dtimage_cast2,(ctl,			/* set real parts of fftw input 	*/
      input, 0, 1,
      output, 0, 2));
  }
  else									/* input is complex? 				*/
  {
	DT_C(dtimage_cast,(ctl, input,		/* simply copy input to output 		*/
      output));
  }

  DT_C(dt_yield,(ctl, 
    "generating FFT"));
  
  fftwnd(plan, 1,						/* generate FFT 					*/
    complex, 1, 0,
    NULL, 0, 0);						/* ignored for in-place 			*/

cleanup:
  if (rc != DT_RC_GOOD &&				/* something went wrong? 			*/
	  output_allocated)					/* but we allocated the output? 	*/
	DT_I(dtimage_free,(ctl, output));	/* free the output 					*/
  
  if (plan != NULL)
	fftwnd_destroy_plan(plan);

  return rc;
}
#endif



/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
  ! Copyright (C) 1998 Dtack Inc. All Rights Reserved
  ! To use this file, you must have signed a license agreement with Dtack Inc.
  ! Under no circumstances may you redistribute this file.
  ! This software is provided AS IS with no warranty expressed or implied.
  ! Dtack Inc. accepts no liability for use or misuse of this file.
  ! http://www.dtack.com  dtack@dtack.com  telephone +360.417.6700
  ! Dtack Inc, P.O. Box 3061, Port Angeles, WA USA 98362
  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */
