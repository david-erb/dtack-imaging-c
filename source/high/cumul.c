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
DT_RCSID("source/high $RCSfile: cum.c,v $ $Revision: 1.1 $");
#include <dtack/mem.h>
#include <dtack/os.h>

/*..........................................................................
| NAME
| dtimage_cumulative() - compute cumulative sum
| 
| SUMMARY
| Implement :out[i] = SUM(in[0..i]): for each pixel :i:.
| 
| If the :DT_ASSIGN_REVERSE: flag is set,
| implement :out[i] = SUM(in[i..n-1]): instead.
| 
| Traverse the image along rows first.
| Do not use a serpentine path: traverse all rows in the same direction.
| Process each pixel result by taking absolute value and/or 
| clipping as specified
| by the bits :DT_ASSIGN_ABS:, and :DT_ASSIGN_CLIP:.
| Use C-like type conversion when assigning final result to output
| (except single-bit output which is considered boolean).
| Permit space sharing among all the images.
| Ignore pixels of image if beyond the extent of the other.
| 
| END
 *..........................................................................*/

dt_rc_e 
DTCONFIG_API1
dtimage_cumulative(						/* make cumulative sum				*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtimage_t *input,						/* input image 						*/
  dtimage_t *output,					/* output image 					*/
  unsigned long assign_flags)			/* operation control flags		    */
{
  DT_F("dtimage_cumulative");
  dtxy_t xmax, ymax;
  dtxy_t row;
  void *in = NULL;
  void *out = NULL;
  dt_ntype_e ntype;
  dt_rc_e rc;

  rc = dtimage_check_2(ctl,				/* check simple stuff				*/
    input, "input",
    output, "output",
    NULL, F, &xmax, &ymax);
  if (rc != DT_RC_GOOD)
    return rc;
  if (
	  DTNTYPE_ISFLOAT(input->ntype) ||	/* either input is floating? 		*/
	  DTNTYPE_ISFLOAT(output->ntype))	/* or output is floating? 			*/
    ntype = DT_NTYPE_F2;				/* do floating arithmetic 			*/
  else
  if (									
	DTNTYPE_ISSIGNED(input->ntype) ||	/* either input is signed? 			*/
	DTNTYPE_ISSIGNED(output->ntype))	/* or output is signed? 			*/
    ntype = DT_NTYPE_S4;				/* use signed arithmetic 			*/
  else								
    ntype = DT_NTYPE_B32;				/* use unsigned arithmetic 			*/

  DT_Q(dtos_malloc2,(ctl,				/* space for promoted row 			*/
    (void **)&in,
    (long)(dt_ntype_sizeof[ntype] * xmax),
    F, "promoted in row"));

  DT_Q(dtos_malloc2,(ctl,				/* space for promoted row 			*/
    (void **)&out,
    (long)(dt_ntype_sizeof[ntype] * xmax),
    F, "promoted out row"));

  for (row=0; row<ymax; row++)			/* for each row 					*/
  {
	void *p;
	dtxy_t y = 
	  assign_flags &
	  DT_ASSIGN_REVERSE? 
	  ymax-row-1: row;

    DTIMAGE_GETROW2(DT_C, ctl,			/* access input row 				*/
      input, y, &p);
    DT_C_1D(dt1d_cast,(					/* promote to intermediate type 	*/
      p, xmax, input->ntype,
      in, xmax, ntype));
    DTIMAGE_UNMAPROW2(DT_C, ctl, 
      input, y);

	if (assign_flags &
		DT_ASSIGN_REVERSE)
      DT_C_1D(dt1d_cumr,(ntype,			/* reverse sum this row 			*/
        in, xmax, out, xmax))
	else
      DT_C_1D(dt1d_cumf,(ntype,			/* forward sum this row				*/
        in, xmax, out, xmax))

    if (row > 0)
      DT_C_1D(dt1d_add,(ntype,				/* sum to previous rows				*/
        in, xmax, 1.0,
        out, xmax, 1.0, 0.0,
        out, xmax))
	else
	  DT_C_1D(dt1d_cast,(
        in, xmax, ntype,
        out, xmax, ntype));

	if (assign_flags & DT_ASSIGN_ABS)
	  DT_C_1D(dt1d_abs,(ntype,			/* absolute value if desired 		*/
        out, xmax));

	if (assign_flags & DT_ASSIGN_CLIP)
	  DT_C_1D(dt1d_clip,(ntype,			/* clip if desired 					*/
        out, xmax,
        dt_ntype_minval[output->ntype],
        dt_ntype_maxval[output->ntype]));

    DTIMAGE_GETROW2(DT_C, ctl,			/* access output row 				*/
      output, y, &p);
    DT_C_1D(dt1d_cast,(					/* promote to output type 			*/
      out, xmax, ntype,
      p, xmax, output->ntype))
    DTIMAGE_PUTROW2(DT_C, ctl, 
      output, y);
  }

cleanup:
  if (out != NULL)
    DT_Q(dtos_free2,(ctl, out,
       F, "promoted out row"));
  if (in != NULL)
    DT_Q(dtos_free2,(ctl, in,
       F, "promoted in row"));

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
