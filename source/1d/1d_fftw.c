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
  19980712 DE PRJ_FFTW_DEFINE removed in favor of $(MK_FFTW_DEFINE)
 *..........................................................................*/



#include <dtack/config.h>				/* platform-specific defines		*/
#include <dtack/ntype.h>
#include <dtack/stats.h>
#include <dtack/1d.h>

#define DT_1 1

#if DTPRJ_FFTW == DT_1

#ifndef DTDEPEND
#include <fftw.h>
#endif

/*..........................................................................
| NAME
| dt1d_fftw() - compute 1D FFT using FFTW package
| 
| SUMMARY
| Compute in-place 1D FFT of ~vector~.
| Expect that ~vector~ is of type :DTNTYPE_F2:.
| Treat ~n~ as the number of :DTNTYPE_F2: pairs.
| Expect the even/odd element pairs to correspond to each complex number's
| real/imaginary parts.
| Do forward FFT if ~forward~ is non-zero otherwise do reverse FFT.
| 
| END
 *..........................................................................*/

dt1d_rc_e
DT1D_API
dt1d_fftw(								/* compute 1D FFT					*/
  dt1d_ntype_f2_t *vector,
  dt1d_i_t n,
  int forward)
{
  fftw_plan plan = NULL;
  FFTW_COMPLEX *complex =
    (FFTW_COMPLEX *)vector;

  plan = fftw_create_plan(				/* create FFTW plan 				*/
    n,
    forward? 
      FFTW_FORWARD: FFTW_BACKWARD,
    FFTW_ESTIMATE | FFTW_IN_PLACE);
  if (plan == NULL)
	return DT1D_RC_BAD;

  fftw(plan, 1,							/* generate FFT 					*/
    complex, 1, 0,
    NULL, 0, 0);						/* ignored for in-place 			*/

cleanup:
  if (plan != NULL)
	fftw_destroy_plan(plan);

  return DT1D_RC_GOOD;
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
