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
  19981231 DE use 1d to do the work and handle all types
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
DT_RCSID("source/high $RCSfile: thresh.c,v $ $Revision: 1.2 $");
#include <dtack/os.h>

/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtimage_thresh_scalar(					/* threshold image by scalar		*/
  dt_ctl_t *ctl,
  dtimage_t *input,						/* input image						*/
  double scalar,						/* threshold						*/
  double below,							/* value for pixels below thresh	*/
  double above,							/* value for pixels at or above		*/
  dtimage_t *output)					/* output image						*/
{
  DT_F("dtimage_thresh_scalar");
  dtxy_t xmax, ymax;
  dtxy_t y, ye;
  void *in, *out;
  dt_ntype_e ntypes[] = {				/* datatypes we can handle			*/
      DTNTYPE_B08,
      DTNTYPE_B16,
      DTNTYPE_B32,
      DTNTYPE_S1,
      DTNTYPE_S2,
      DTNTYPE_S4,
      DTNTYPE_F1,
      DTNTYPE_F2,
      DTNTYPE_NULL};
  dt_rc_e rc = DT_RC_GOOD;

  rc = dtimage_check_2(ctl,				/* check simple stuff				*/
    input, "input",
    output, "output",
    ntypes, F, &xmax, &ymax);
  if (rc != DT_RC_GOOD)
    return rc;

  ye = input->ye;
  for (y=0; y<ye; y++)
  {
    DTIMAGE_GETROW2(DT_C, ctl, input, y, &in);
    DTIMAGE_GETROW2(DT_C, ctl, output, y, &out);
    DT_C_1D(dt1d_thresh_scalar,(
      input->ntype, 
      in, input->xe,
      scalar,
      below, above,
      out, output->xe));
    DTIMAGE_PUTROW2(DT_C, ctl, out, y);
	out = NULL;
    DTIMAGE_UNMAPROW2(DT_C, ctl, in, y);
	in = NULL;
  }

cleanup:
  if (out)
    DTIMAGE_PUTROW2(DT_I, ctl, out, y);
  if (in)
    DTIMAGE_UNMAPROW2(DT_I, ctl, in, y);

  return rc;
}

/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtimage_thresh_image(					/* "thresh" two images				*/
  dt_ctl_t *ctl,
  dtimage_t *input1,					/* input images						*/
  dtimage_t *input2,
  double below,							/* value for pixels below thresh	*/
  double above,							/* value for pixels at or above		*/
  dtimage_t *output)					/* output image						*/
{
  DT_F("dtimage_thresh_image");
  dtxy_t y, ye;
  dtxy_t xmax, ymax;
  void *in1, *in2, *out;
  dt_ntype_e ntypes[] = {				/* datatypes we can handle			*/
      DTNTYPE_B08,
      DTNTYPE_B16,
      DTNTYPE_B32,
      DTNTYPE_S1,
      DTNTYPE_S2,
      DTNTYPE_S4,
      DTNTYPE_F1,
      DTNTYPE_F2,
      DTNTYPE_NULL};
  dt_rc_e rc = DT_RC_GOOD;

  rc = dtimage_check_3(ctl,				/* check simple stuff				*/
    input1, "input1",
    input2, "input2",
    output, "output",
    ntypes, F, &xmax, &ymax);
  if (rc != DT_RC_GOOD)
    return rc;

  ye = input1->ye;
  for (y=0; y<ye; y++)
  {
    DTIMAGE_GETROW2(DT_C, ctl, input1, y, &in1);
    DTIMAGE_GETROW2(DT_C, ctl, input2, y, &in2);
    DTIMAGE_GETROW2(DT_C, ctl, output, y, &out);
    DT_C_1D(dt1d_thresh_vector,(
      input1->ntype, 
      in1, input1->xe,
      in2, input2->xe,
      below, above,
      out, output->xe));
    DTIMAGE_PUTROW2(DT_C, ctl, out, y);
	out = NULL;
    DTIMAGE_UNMAPROW2(DT_C, ctl, in2, y);
	in2 = NULL;
    DTIMAGE_UNMAPROW2(DT_C, ctl, in1, y);
	in1 = NULL;
  }

cleanup:
  if (out)
    DTIMAGE_PUTROW2(DT_I, ctl, out, y);
  if (in2)
    DTIMAGE_UNMAPROW2(DT_I, ctl, in2, y);
  if (in1)
    DTIMAGE_UNMAPROW2(DT_I, ctl, in1, y);

  return rc;
}

/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtimage_thresh_multi(					/* threshold image by array			*/
  dt_ctl_t *ctl,
  dtimage_t *input,						/* input image						*/
  const double * const thresh,			/* threshold array					*/
  unsigned int n,
  const double * const values,			/* output array					    */
  dtimage_t *output)					/* output image						*/
{
  DT_F("dtimage_multithresh");
  dt_ntype_e ntypes[] =					/* datatypes we can handle			*/
    {DT_NTYPE_B08, DT_NTYPE_NULL};
  double *lut = NULL;
  unsigned int i;
  dt_rc_e rc;

  rc = dtimage_check_2(ctl,				/* check simple stuff				*/
    input, "input",
    output, "output",
    ntypes, F, NULL, NULL);
  if (rc != DT_RC_GOOD)
    return rc;

  switch(input->ntype)					/* branch based on ntype			*/
  {
	case DT_NTYPE_B08:  
	{
      const unsigned int maxval =
        (unsigned int)
          dt_ntype_maxval[DT_NTYPE_B08];
      unsigned int j = 0;
	  DT_C(dtos_malloc2,(ctl,			/* get space for lut 				*/
        (void **)&lut,
        (maxval + 1) * sizeof(*lut),
        F, "temporary lut"));
      for (i=0; i<n+1; i++)				/* make the lut					    */
	  {
        unsigned int t;
        if (i == n)						/* last "threshold" is maxval	    */
          t = maxval + 1;
        else
          t = (unsigned int)thresh[i];
        if (t > maxval)					/* clip to output maximum value 	*/
          t = maxval + 1;
		while (j < t)					/* fill values in between 			*/
          lut[j++] = values[i];
	  }
      DT_C(dtimage_lookup,(ctl,			/* transform via lookup			    */
        input, lut, 256, output));
	}
    break;
  }
  
cleanup:
  if (lut != NULL)
    DT_I(dtos_free2,(ctl,
      lut, F, "temporary lut"));

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
