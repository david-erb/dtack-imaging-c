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
DT_RCSID("source/high $RCSfile: logical.c,v $ $Revision: 1.1 $");
#include <dtack/os.h>
#include <dtack/mem.h>

/*..........................................................................
| NAME
| dtimage_logical() - image logical operations
| 
| SUMMARY
| Implement :out[i] = in1[i] AND a OP in2[i] AND b OP c: for each pixel :i:.
| 
| Expect :OP: to be one of :DT_OP_OR:, :DT_OP_AND:, or :DT_OP_XOR:. 
| 
| Use only the integral part of any floating input numbers.
| Use C-like type conversion when assigning final result to output
| (except single-bit output which is considered boolean).
| Permit space sharing among the images.
| Ignore pixels of image if beyond the extent of any other.
| 
| END
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtimage_logical(						/* do logical op with each pixel	*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtimage_t *input1,					/* first input image 				*/
  unsigned long a,						/* first constant 					*/
  dtimage_t *input2,					/* second input image 				*/
  unsigned long b,						/* second constant 					*/
  unsigned long c,						/* final scalar constant 			*/
  dt_op_e op,							/* operation to perform 			*/
  dtimage_t *output)					/* output image						*/
{
  DT_F("dtimage_logical");
  dtxy_t xmax, ymax;
  dt_ntype_e *ntypes = NULL;			/* we can handle any ntype		    */
  dt_ntype_e ntype;
  void *in1 = NULL;
  void *in2 = NULL;
  void *out = NULL;
  dtxy_t y;
  dt_rc_e rc;

  rc = dtimage_check_1f(ctl,			/* check simple stuff				*/
    input1, "input1",
    ntypes, DTIMAGE_FLAG_PAGED, F);
  if (rc != DT_RC_GOOD)
    return rc;

  rc = dtimage_check_1f(ctl,			/* check simple stuff				*/
    input2, "input2",
    ntypes, DTIMAGE_FLAG_PAGED, F);
  if (rc != DT_RC_GOOD)
    return rc;

  rc = dtimage_check_1f(ctl,			/* check simple stuff				*/
    output, "output",
    ntypes, DTIMAGE_FLAG_PAGED, F);
  if (rc != DT_RC_GOOD)
    return rc;

  xmax = DT_MIN(DT_MIN(output->xe,
    input1->xe), input2->xe);

  ymax = DT_MIN(DT_MIN(output->ye,
    input1->ye), input2->ye);

  ntype = DT_NTYPE_B32;					/* use unsigned arithmetic			*/

  if (input1->ntype != ntype)
    DT_Q(dtos_malloc2,(ctl,				/* space for promoted row 			*/
      (void **)&in1,
      (long)(dt_ntype_sizeof[ntype] * xmax),
      F, "promoted in1 row"));

  if (input2->ntype != ntype)
    DT_Q(dtos_malloc2,(ctl,				/* space for promoted row 			*/
      (void **)&in2,
      (long)(dt_ntype_sizeof[ntype] * xmax),
      F, "promoted in2 row"));

  if (output->ntype != ntype)
    DT_Q(dtos_malloc2,(ctl,				/* space for promoted row 			*/
      (void **)&out,
      (long)(dt_ntype_sizeof[ntype] * xmax),
      F, "promoted out row"));

  for (y=0; y<ymax; y++)				/* for each row 					*/
  {
	void *in1p, *in2p, *outp;

    DTIMAGE_GETROW2(DT_C, ctl,			/* access input row 				*/
      input1, y, &in1p);
	if (in1 != NULL)
      DT_C_1D(dt1d_cast,(				/* promote to intermediate type 	*/
        in1p, xmax, input1->ntype,
        in1, xmax, ntype));

    DTIMAGE_GETROW2(DT_C, ctl,			/* access input row 				*/
      input2, y, &in2p);
	if (in2 != NULL)
      DT_C_1D(dt1d_cast,(				/* promote to intermediate type 	*/
        in2p, xmax, input2->ntype,
        in2, xmax, ntype));
	
    DTIMAGE_GETROW2(DT_C, ctl,			/* access output row 				*/
      output, y, &outp);

    switch(op)							/* do the operation 				*/
	{
	  case DT_OP_OR:
        DT_C_1D(dt1d_or,(ntype,
          in1? in1: in1p, xmax, a,
		  in2? in2: in2p, xmax, b, c,
          out? out: outp, xmax));
	  break;
	  case DT_OP_AND:
        DT_C_1D(dt1d_and,(ntype,
          in1? in1: in1p, xmax, a,
		  in2? in2: in2p, xmax, b, c,
          out? out: outp, xmax));
	  break;
	  case DT_OP_XOR:
        DT_C_1D(dt1d_xor,(ntype,
          in1? in1: in1p, xmax, a,
		  in2? in2: in2p, xmax, b, c,
          out? out: outp, xmax));
	  break;
	  default:
	    rc = dt_err(ctl, F,
          "bad op %d", op);
		goto cleanup;
	}

	if (out != NULL)
      DT_C_1D(dt1d_cast,(				/* promote to output type 			*/
        out, xmax, ntype,
        outp, xmax, output->ntype))
	
    DTIMAGE_PUTROW2(DT_C, ctl, 
      output, y);
    DTIMAGE_UNMAPROW2(DT_C, ctl, 
      input2, y);
    DTIMAGE_UNMAPROW2(DT_C, ctl, 
      input1, y);
  }

cleanup:
  if (out != NULL)
    DT_I(dtos_free2,(ctl, out,
       F, "promoted out row"));
  if (in2 != NULL)
    DT_I(dtos_free2,(ctl, in2,
       F, "promoted in2 row"));
  if (in1 != NULL)
    DT_I(dtos_free2,(ctl, in1,
       F, "promoted in1 row"));

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
