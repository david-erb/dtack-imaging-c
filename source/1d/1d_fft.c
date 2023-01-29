/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
  ! Copyright (C) 1998 Dtack Inc. All Rights Reserved
  ! To use this file, you must have signed a license agreement with Dtack Inc.
  ! Under no circumstances may you redistribute this file.
  ! This software is provided AS IS with no warranty expressed or implied.
  ! Dtack Inc. accepts no liability for use or misuse of this file.
  ! http://www.dtack.com  dtack@dtack.com  telephone +360.417.6700
  ! Dtack Inc, P.O. Box 3061, Port Angeles, WA USA 98362
  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */




#include <dtack/config.h>				/* platform-specific defines		*/
#include <dtack/ntype.h>
#include <dtack/stats.h>
#include <dtack/1d.h>

#define DT_1 1

/*..........................................................................
| NAME
| dt1d_fft_complex() - compute 1D FFT on complex data
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
dt1d_fft_complex(						/* compute 1D FFT					*/
  dt1d_ntype_f2_t *vector,
  dt1d_i_t n,
  int forward)
{
#if DTPRJ_FFTW == DT_1
  return dt1d_fftw(vector, n, forward);
#else
  return DT1D_RC_BAD;
#endif
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
