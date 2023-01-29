/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
  ! Copyright (C) 1998 Dtack Inc. All Rights Reserved
  ! To use this file, you must have signed a license agreement with Dtack Inc.
  ! Under no circumstances may you redistribute this file.
  ! This software is provided AS IS with no warranty expressed or implied.
  ! Dtack Inc. accepts no liability for use or misuse of this file.
  ! http://www.dtack.com  dtack@dtack.com  telephone +360.417.6700
  ! Dtack Inc, P.O. Box 3061, Port Angeles, WA USA 98362
  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */



#include <dtack/base.h>
DT_RCSID("source/high $RCSfile: fft.c,v $ $Revision: 1.1 $");

/*..........................................................................
| NAME
| dtimage_fft_*() - compute 2D FFT
| 
| SUMMARY
| Compute 2D FFT of ~input~ and return result as ~output~.
| Allocate space for ~output~ if it is not preallocated.
| Have an error if ~output~ is preallocated but does not have the 
| following attributes: non-paged, contiguous, and ntype :DTNTYPE_F2:.
| In the double case, presume the input image is real valued. 
| In the complex case, presume the input image contains
| real parts in the even pixels and imaginary in the odd.
| Compute the forward FFT transform if ~forward~ is non-zero,
| otherwise compute backward transform.
| 
| END
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtimage_fft_double(						/* compute double-precision 2D FFT	*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtimage_t *input,						/* input image 						*/
  dtimage_t *output)					/* returned frequency domain image	*/
{
  DT_F("dtimage_fft_double");
#if DTPRJ_FFTW == DT_1
  DT_Q(dtimage_fftw_2d,(ctl,
    input, 1, 1, output));
  return DT_RC_GOOD;
#else
  return dt_err_notlinked(ctl, F,
    "FFTW");
#endif
}

/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtimage_fft_complex(					/* compute double-precision 2D FFT	*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtimage_t *input,						/* input image 						*/
  int forward,							/* true if forward transform 		*/
  dtimage_t *output)					/* returned frequency domain image	*/
{
  DT_F("dtimage_fft_double");
#if DTPRJ_FFTW == DT_1
  DT_Q(dtimage_fftw_2d,(ctl,
    input, 0, forward, output));
  return DT_RC_GOOD;
#else
  return dt_err_notlinked(ctl, F,
    "FFTW");
#endif
}

/*..........................................................................
 * return magnitude in even pixels and -PI<=angle<=PI in odd pixels
 *..........................................................................*/

static
dt_rc_e
complex_polar(							/* convert complex to polar			*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtimage_t *input,						/* input image 						*/
  int want_magnitude,
  int want_angle)
{
  DT_F("dtimage_fft_complex_polar");
  dtxy_t x, y;
  const dtxy_t xe = input->xe;
  const dtxy_t ye = input->ye;
  dt_rc_e rc;
  DT_C(dt_yield,(ctl, 
    "converting complex fft to polar"));
  for (y=0; y<ye; y++)
  {
	double *p = (double *)input->row[y].dvoid;
	for (x=0; x<xe; x+=2)
	{
	  double r = p[x];
	  double i = p[x+1];
	  if (want_magnitude)
		p[x] = sqrt(r * r + i * i);
	  if (want_angle)
		p[x+1] = atan2(i, r);
	}
  }
cleanup:
  return rc;
}

/*..........................................................................
| NAME
| dtimage_fft_polar() - return fft in polar form
| 
| SUMMARY
| Compute 2D FFT of ~input~ and return result in either single ~polar~ image or
| as separate ~magnitude~ and ~angle~ images.
| If ~polar~ is being returned, the magnitudes are stored in the even
| pixels and the angles in the odd pixels.
| Allocate space for any output image which is initially empty.
| 
| END
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtimage_fft_polar(						/* return fft in polar form			*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtimage_t *input,						/* input image 						*/
  dtimage_t *polar)						/* returned polar image 			*/
{
  DT_F("dtimage_fft_polar");
  dt_ntype_e *ntypes = NULL;			/* we can handle any type		    */
  dtimage_t output;
  dt_rc_e rc;

  rc = dtimage_check_1(ctl,				/* check simple stuff				*/
    input, "input",
    ntypes, F);
  if (rc != DT_RC_GOOD)
    return rc;

  output.xe = 0;
  DT_C(dtimage_fft_double,(ctl,			/* create output and compute fft 	*/
    input, &output));

  DT_C(complex_polar,(ctl,				/* convert complex to polar 		*/
    &output, 1, 1));
  
  if (polar->xe != 0)					/* polar image is pre-allocated? 	*/
	DT_C(dtimage_cast,(ctl, &output,	/* copy output to it 				*/
      polar))
  else									/* polar image not pre-allocated? 	*/
  {
	*polar = output;					/* use new space 					*/
	output.xe = 0;
  }
cleanup:
  if (output.xe != 0)
	DT_I(dtimage_free,(ctl, &output));

  return rc;
}

/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtimage_fft_polar2(						/* return fft in polar form			*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtimage_t *input,						/* input image 						*/
  dtimage_t *magnitude,					/* returned magnitude image 		*/
  dtimage_t *angle)						/* returned angle image 			*/
{
  DT_F("dtimage_fft_polar2");
  dt_ntype_e *ntypes = NULL;			/* we can handle any type		    */
  dtimage_t polar;
  int magnitude_allocated = 0;
  int angle_allocated = 0;
  dt_rc_e rc;

  rc = dtimage_check_1(ctl,				/* check simple stuff				*/
    input, "input",
    ntypes, F);
  if (rc != DT_RC_GOOD)
    return rc;

  polar.xe = 0;
  DT_C(dtimage_fft_polar,(ctl, input,	/* compute fft as polar 			*/
    &polar));
  
  if (magnitude->xe == 0)				/* magnitude needs to be allocated? */
  {
	DT_C(dtimage_create,(ctl, magnitude,
      polar.xe/2, polar.ye, DTNTYPE_F2));
	magnitude_allocated = 1;
  }
  DT_C(dtimage_cast2,(ctl,				/* get magnitude from even pixels 	*/
    &polar, 0, 2,
    magnitude, 0, 1));
  
  if (angle->xe == 0)					/* angle needs to be allocated 		*/
  {
	DT_C(dtimage_create,(ctl, angle,
      polar.xe/2, polar.ye, DTNTYPE_F2));
	angle_allocated = 1;
  }
  DT_C(dtimage_cast2,(ctl,				/* get angle from odd pixels 		*/
    &polar, 1, 2,
    angle, 0, 1));

cleanup:
  if (rc != DT_RC_GOOD &&
	  angle_allocated)
	DT_I(dtimage_free,(ctl, angle));
  if (rc != DT_RC_GOOD &&
	  magnitude_allocated)
	DT_I(dtimage_free,(ctl, magnitude));
  if (polar.xe != 0)
	DT_I(dtimage_free,(ctl, &polar));

  return rc;
}

/*..........................................................................
| NAME
| dtimage_fft_polar() - return FFT magnitude
| 
| SUMMARY
| Compute 2D FFT of ~input~ and return ~magnitude~ image.
| Magnitude is computed as sqrt(r*r+i*i) on the complex FFT output.
| Allocate space for the output image if it is initially empty.
| 
| END
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtimage_fft_magnitude(					/* return FFT magnitude				*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtimage_t *input,						/* input image 						*/
  dtimage_t *magnitude)					/* returned magnitude image			*/
{
  DT_F("dtimage_fft_magnitude");
  dt_ntype_e *ntypes = NULL;			/* we can handle any type		    */
  dtimage_t output;
  int magnitude_allocated = 0;
  dt_rc_e rc;

  rc = dtimage_check_1(ctl,				/* check simple stuff				*/
    input, "input",
    ntypes, F);
  if (rc != DT_RC_GOOD)
    return rc;

  output.xe = 0;
  DT_C(dtimage_fft_double,(ctl,			/* create output and compute fft 	*/
    input, &output));

  DT_C(complex_polar,(ctl,				/* compute complex magnitude		*/
    &output, 1, 0));
  
  if (magnitude->xe == 0)				/* magnitude not pre-allocated? 	*/
  {
	DT_C(dtimage_create,(ctl,			/* allocate new space 				*/
      magnitude,
      output.xe/2, output.ye, 
      DTNTYPE_F2));
	magnitude_allocated = 1;
  }

  DT_C(dtimage_cast2,(ctl,				/* get magnitude from even pixels 	*/
    &output, 0, 2,
    magnitude, 0, 1));

cleanup:
  if (rc != DT_RC_GOOD &&
	  magnitude_allocated)
  {
	DT_I(dtimage_free,(ctl, magnitude));
  }
  if (output.xe != 0)
  {
	DT_I(dtimage_free,(ctl, &output));
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
















