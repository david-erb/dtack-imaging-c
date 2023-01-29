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
DT_RCSID("source/high $RCSfile: cast.c,v $ $Revision: 1.1 $");
#include <dtack/mem.h>

/*..........................................................................
| NAME
| dtimage_cast_row() - copy/convert row from one image to another
| 
| SUMMARY
| Copy pixels from row ~input_y~ of the ~input~ image to row
| ~output_y~ of the ~output~ image.
| Skip input pixels if ~input_dx~ is greater than 1.
| Skip output pixels if ~output_dx~ is greater than 1.
| Perform data type conversion if data types are not the same.
| Truncate without comment if row pixel widths are not the same.
| If ~input_dx~ and ~input_dy~ are omitted, 0, or 1 then
| source and destination images may be the same.
| Have an error if either of the given rows is outside its respective
| image extent.
| 
| END
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtimage_cast_row(						/* copy/convert one row of image	*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtimage_t *input,						/* source image						*/
  dtxy_t input_y,						/* source row 						*/
  dtimage_t *output,					/* destination image				*/
  dtxy_t output_y)						/* destination row					*/
{
  DT_F("dtimage_cast_row");
  dtxy_t n;
  void *in, *out;

  if (input_y >= input->ye)				/* check for row within range		*/
    return dtimage_err_outside(ctl,
      input, "input",
      0, input_y, F);

  if (output_y >= output->ye)
    return dtimage_err_outside(ctl,
      output, "output",
	  0, output_y, F);

  n = DT_MIN(input->xe, output->xe);	/* use shorter of two rows		    */
  
  DTIMAGE_GETROW(ctl, input,			/* bring rows into memory 			*/
    input_y, &in);
  DTIMAGE_GETROW(ctl, output, 
    output_y, &out);
  DT_Q_1D(dt1d_cast,(					/* convert the row 					*/
    in, n, input->ntype,
    out, n, output->ntype));
  DTIMAGE_PUTROW(ctl, output, input_y);	/* release rows from memory 		*/
  DTIMAGE_UNMAPROW(ctl, input, output_y);

  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtimage_cast_row2(						/* copy/convert one row of image	*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtimage_t *input,						/* source image						*/
  dtxy_t input_x0,						/* source offset 					*/
  dtxy_t input_dx,						/* source stride 					*/
  dtxy_t input_y,						/* source row 						*/
  dtimage_t *output,					/* destination image				*/
  dtxy_t output_x0,						/* destination offset				*/
  dtxy_t output_dx,						/* destination stride				*/
  dtxy_t output_y)						/* destination row					*/
{
  DT_F("dtimage_cast_row2");
  dtxy_t n;
  void *in, *out;

  if (input_y >= input->ye)				/* check for row within range		*/
    return dtimage_err_outside(ctl,
      input, "input",
      0, input_y, F);

  if (output_y >= output->ye)
    return dtimage_err_outside(ctl,
      output, "output",
	  0, output_y, F);
  
  DTIMAGE_GETROW(ctl, input,			/* bring rows into memory 			*/
    input_y, &in);
  DTIMAGE_GETROW(ctl, output, 
    output_y, &out);
  DT_Q_1D(dt1d_cast2,(					/* convert the row 					*/
    in, input_x0, input_dx, input->xe, 
    input->ntype,
    out, output_x0, output_dx, output->xe, 
    output->ntype));
  DTIMAGE_PUTROW(ctl, output, input_y);	/* release rows from memory 		*/
  DTIMAGE_UNMAPROW(ctl, input, output_y);

  return DT_RC_GOOD;
}

/*..........................................................................
| NAME
| dtimage_cast() - copy/convert one image to another
| 
| SUMMARY
| Copy ~input~ pixels to ~output~.
| Skip input pixels if ~input_dx~ is greater than 1.
| Skip output pixels if ~output_dx~ is greater than 1.
| Copy the pixel at the first column of the first row
| of the input image to the same position in the output image.
| Perform data type conversion if data types are not the same.
| Truncate without comment if image pixel sizes are not the same.
| Source and destination images may share the same memory
| only if the regions and data types cause the source bytes
| to be accessed before they become destination bytes.
| Have an error if either of the given rows is outside its respective
| image extent.
| 
| END
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtimage_cast(							/* copy/convert image to another 	*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtimage_t *input,						/* source image 					*/
  dtimage_t *output)					/* destination image 				*/
{
  DT_F("dtimage_cast");
  dt_ntype_e *ntypes = NULL;			/* we can handle any type		    */
  dtxy_t xe, ye;
  dtxy_t y;
  dt_rc_e rc;

  rc = dtimage_check_2f(ctl,			/* tracer						    */
    input, "input",
    output, "output",
    ntypes, DTIMAGE_FLAG_PAGED, F,		/* paged images are supported 		*/
    &xe, &ye);
  if (rc != DT_RC_GOOD)
    return rc;

  for (y=0; y<ye; y++)
    DT_Q(dtimage_cast_row,(ctl,
      input, y, output, y));
  
  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtimage_cast2(							/* copy/convert image to another 	*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtimage_t *input,						/* source image 					*/
  dtxy_t input_x0,						/* source offset 					*/
  dtxy_t input_dx,						/* source stride 					*/
  dtimage_t *output,					/* destination image 				*/
  dtxy_t output_x0,						/* destination offset				*/
  dtxy_t output_dx)						/* destination stride				*/
{
  DT_F("dtimage_cast2");
  dt_ntype_e ntypes[] = {				/* we can handle these types		*/
      DTNTYPE_B08,
      DTNTYPE_B16,
      DTNTYPE_B32,
      DTNTYPE_S1,
      DTNTYPE_S2,
      DTNTYPE_S4,
      DTNTYPE_F1,
      DTNTYPE_F2,
      DTNTYPE_NULL};
  dtxy_t y, ye;
  dt_rc_e rc;

  rc = dtimage_check_1f(ctl,			/* tracer						    */
    input, "input",
    ntypes, DTIMAGE_FLAG_PAGED, F);		/* paged images are supported 		*/
  if (rc != DT_RC_GOOD)
    return rc;

  rc = dtimage_check_1f(ctl,			/* tracer						    */
    output, "output",
    ntypes, DTIMAGE_FLAG_PAGED, F);		/* paged images are supported 		*/
  if (rc != DT_RC_GOOD)
    return rc;

  ye = DT_MIN(input->xe, output->ye);
  for (y=0; y<ye; y++)
    DT_Q(dtimage_cast_row2,(ctl,
      input, input_x0, input_dx, y, 
      output, output_x0, output_dx, y));
  
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











