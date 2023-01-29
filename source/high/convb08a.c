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
| Introduction - intro
| 
| DESCRIPTION
| As yet undocumented.
| 
| RETURN VALUES
| Function return value will be DT_RC_GOOD on success.  
| Any other return value indicates failure or noncompletion.
| When DT_RC_GOOD is not returned, any other return values may not be valid.
| 
| ERRORS
| As yet undocumented.
| 
| END
 *..........................................................................*/

#include <dtack/base.h>
#include <dtack/os.h>
#include <dtack/yield.h>
#include <dtack/mem.h>

DT_RCSID("base $RCSfile: convb08a.c,v $ $Revision: 1.1 $");

static long kernel_3x3_ave[] = {1,1,1, 1,1,1, 1,1,1};
static long kernel_3x3_laplace[] = {0,-1,0, -1,4,-1, 0,-1,0};

/*..........................................................................*/
dt_rc_e
DTCONFIG_API1
dtimage_convolve_long_b08(
  dt_ctl_t *ctl,
  dtimage_t *input,
  long *kernel,
  int kxsize,
  int kysize,
  long gain,
  dtimage_t *output,
  int *didit)
{
  DT_F("dtimage_convolve_long_b08");
  static dt_ntype_e input_ntypes[] = {
    DT_NTYPE_B08, DT_NTYPE_NULL};
  static dt_ntype_e output_ntypes[] = {
    DT_NTYPE_B08, DT_NTYPE_NULL};
  dtxy_t xmax, ymax;
  dtxy_t x1, y1;
  dtxy_t x2, y2;
  int kx1, ky1;
  int kx2, ky2;
  long *mul = NULL;
  long cmp;
  dt_rc_e rc;

  *didit = 1;

  rc = dtimage_check_1f(ctl,			/* check input data type		    */
    input, "input",
    input_ntypes,
    DTIMAGE_FLAG_PAGED, F);
  if (rc != DT_RC_GOOD)
    return rc;

  rc = dtimage_check_1f(ctl,			/* check output data type		    */
    output, "output",
    output_ntypes, 
    DTIMAGE_FLAG_PAGED, F);
  if (rc != DT_RC_GOOD)
    return rc;

  xmax = DT_MIN(input->xe, output->xe);
  ymax = DT_MIN(input->ye, output->ye);

  kx1 = -kxsize / 2;
  ky1 = -kysize / 2 ;
  kx2 = kxsize + kx1;
  ky2 = kysize + ky1;
  x1 = -kx1;
  y1 = -ky1;
  x2 = xmax - kx2 + 1;
  y2 = ymax - ky2 + 1;

  if (kxsize == 3 && kysize == 3)
  {
	DT_C_1D(dt1d_cmp,(DT1D_NTYPE_S4,	/* b08,b08 and 3x3 average kernel   */
      kernel, 9,
      kernel_3x3_ave, 9, 0, &cmp));
	if (cmp == 0)
	{
      DT_C(dtimage_convolve_long_b08_b08_3x3_ave,(ctl,
        input, x1, y1, x2, y2,
        kernel, kx1, ky1, kx2, ky2,
        gain, output));
	  goto cleanup;
	}

	DT_C_1D(dt1d_cmp,(DT1D_NTYPE_S4,	/* b08,b08 and 3x3 laplace			*/
      kernel, 9,
      kernel_3x3_laplace, 9, 0, &cmp));
	if (cmp == 0)
	{
	  DT_C(dtos_malloc2,(ctl,
        (void **)&mul, 
        256*sizeof(long),
        F, "mul buffer"));
	  DT_C(dtimage_convolve_long_b08_b08_3x3_laplace,
        (ctl,
         input, x1, y1, x2, y2,
         kernel, kx1, ky1, kx2, ky2,
         gain, mul, output));
	  goto cleanup;
	}
										/* b08,b08 and 3x3 unknown kernel	*/
	{
	  DT_C(dtos_malloc2,(ctl,
        (void **)&mul, 256*9*sizeof(long),
        F, "mul buffer"));
	  DT_C(dtimage_convolve_long_b08_b08_3x3_any,(ctl,
        input, x1, y1, x2, y2,
        kernel, kx1, ky1, kx2, ky2,
        gain, mul, output));
	  goto cleanup;
    }
  }

  *didit = 0;							/* couldn't do it				    */

cleanup:
  if (mul != NULL)
    DT_I(dtos_free2,(ctl, mul,
      F, "mul buffer"));

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
