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
  19991217 DE changed some long's to 4-byte int's in favor of 64-bit CPU
 *..........................................................................*/

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
DT_RCSID("source/high $RCSfile: convolve.c,v $ $Revision: 1.1 $");
#include <dtack/os.h>
#include <dtack/mem.h>

/*..........................................................................*/
dt_rc_e
DTCONFIG_API1
dtimage_convolve(
  dt_ctl_t *ctl,
  dtimage_t *input,
  dtimage_t *kernel,
  dtimage_t *output)
{
  DT_F("dtimage_convolve");
  double linear_dynamic[25];			/* maybe save a malloc 				*/
  void *linear = NULL;
  dtxy_t y;
  dt_rc_e rc;

  if (input->ntype == DT_NTYPE_F1 ||	/* input is floating? 				*/
	  input->ntype == DT_NTYPE_F2 ||
	  kernel->ntype == DT_NTYPE_F1 ||	/* one of the floating kernels? 	*/
	  kernel->ntype == DT_NTYPE_F2 ||
	  output->ntype == DT_NTYPE_F1 ||	/* output is floating? 				*/
	  output->ntype == DT_NTYPE_F2)
  {
    double *q;
	unsigned int n =					/* size needed for linear kernel	*/
      kernel->xe * kernel->ye *
        sizeof(double);

	if (n > sizeof(linear_dynamic))
  	  DT_C(dtos_malloc2,(ctl, &linear,	/* space for linear kernel elements */
        n, F, "linear kernel"))
	else
	  linear = (void *)linear_dynamic;
	q = (double *)linear;				/* address space as double 			*/
	for (y=0; y<kernel->ye; y++)		/* go through all kernel rows 		*/
	{
  	  void *p;
 	  DTIMAGE_GETROW(ctl,				/* address kernel row 				*/
        kernel, y, &p);
	  DT_Q_1D(dt1d_cast,(				/* convert elements to double 		*/
        p, kernel->xe, kernel->ntype,
        q, kernel->xe, DT_NTYPE_F2));
  	  DTIMAGE_UNMAPROW(ctl, kernel, y);
	  q += kernel->xe;
	}
	DT_C(dtimage_convolve_double,(ctl,	/* do the convolution 				*/
      input, 
      (double *)linear,
      DT_CAST(int, kernel->xe),
      DT_CAST(int, kernel->ye),
	  output));
  }
  else									/* not a floating kernel? 			*/
  {
    dtntype_s4_t *q;
	unsigned int n =					/* size needed for linear kernel	*/
      kernel->xe * kernel->ye *
        sizeof(dtntype_s4_t);

	if (n > sizeof(linear_dynamic))
  	  DT_C(dtos_malloc2,(ctl, &linear,	/* space for linear kernel elements */
        n, F, "linear kernel"))
	else
	  linear = (void *)linear_dynamic;
	q = (dtntype_s4_t *)linear;			/* address space as dtntype_s4_t 	*/
	for (y=0; y<kernel->ye; y++)		/* go through all kernel rows 		*/
	{
  	  void *p;
 	  DTIMAGE_GETROW(ctl,				/* address kernel row 				*/
        kernel, y, &p);
	  DT_Q_1D(dt1d_cast,(				/* convert elements to dtntype_s4_t */
        p, kernel->xe, kernel->ntype,
        q, kernel->xe, DT_NTYPE_S4));
  	  DTIMAGE_UNMAPROW(ctl, kernel, y);
	  q += kernel->xe;
	}
	DT_C(dtimage_convolve_s4,(ctl,		/* do the convolution 				*/
      input, 
      (dtntype_s4_t *)linear,
      DT_CAST(int, kernel->xe),
      DT_CAST(int, kernel->ye),
	  output));
  }
  
cleanup:
  if (linear != NULL && 
      linear != linear_dynamic)
    DT_I(dtos_free2,(ctl, linear,
	  F, "linear kernel"));

  return rc;
}



/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtimage_boxcarr(
  dt_ctl_t *ctl,
  dtimage_t *input,
  int kernel_size,
  dtimage_t *work,
  dtimage_t *output)
{
  DT_F("dtimage_boxcarr");
  int workstride, outstride;
  dtxy_t xe = DT_MIN(work->xe, output->xe);
  dtxy_t ye = DT_MIN(input->ye, work->ye);
  dtxy_t x, y;

  for (y=0; y<ye; y++)
	dt1d_boxcarr(work->ntype,
	  input->row[y].f2, input->xe, 1,
	  kernel_size,
	  work->row[y].f2, work->xe, 1);

  workstride = 
	work->row[1].f2 -
	work->row[0].f2;
  outstride = 
	output->row[1].f2 -
	output->row[0].f2;

  for (x=0; x<xe; x++)
	dt1d_boxcarr(work->ntype,
	  work->row[0].f2+x, work->ye, workstride,
	  kernel_size,
	  output->row[0].f2+x, output->ye, outstride);

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
