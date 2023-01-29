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
DT_RCSID("source/high $RCSfile: arith.c,v $ $Revision: 1.1 $");
#include <dtack/os.h>
#include <dtack/mem.h>
#include <dtack/str.h>

/*..........................................................................
| NAME
| dtimage_arith() - image arithmetic operations
| 
| SUMMARY
| Implement :out[i] = (in1[i] * a) OP (in2[i] * b) OP c: for each pixel :i:.
| 
| Expect :OP: to be one of :DT_OP_ADD:, :DT_OP_MUL:, or :DT_OP_DIV:.
| 
| Perform integer arithmetic if all image types are integer
| and all constants are whole numbers.
| Process result by taking absolute value and/or clipping as specified
| by the bits :DT_ASSIGN_ABS:, and :DT_ASSIGN_CLIP:.
| Use C-like type conversion when assigning final result to output
| (except single-bit output which is considered boolean).
| Permit memory sharing among images.
| Ignore pixels of image if beyond the extent of any other.
| 
| END
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtimage_arith(							/* do scalar op with each pixel		*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtimage_t *input1,					/* first input image 				*/
  double a,								/* first image factor 				*/
  dtimage_t *input2,					/* second input image 				*/
  double b,								/* second image factor 				*/
  double c,								/* offset 							*/
  dt_op_e op,							/* operation to perform 			*/
  unsigned long assign_flags,			/* final assignment flags 			*/
  dtimage_t *output)					/* output image						*/
{
  DT_F("dtimage_arith");
  dtxy_t xmax, ymax;
  dt_ntype_e *ntypes = NULL;			/* we can handle any ntype		    */
  dt_ntype_e ntype;
  void *in1 = NULL;
  void *in2 = NULL;
  void *out = NULL;
  dtxy_t y;
  dt_rc_e rc;

  rc = dtimage_check_1f(ctl,			/* check simple stuff				*/
    input1, NULL,
    ntypes, DTIMAGE_FLAG_PAGED, F);
  if (rc != DT_RC_GOOD)
    return rc;

  rc = dtimage_check_1f(ctl,			/* check simple stuff				*/
    input2, NULL,
    ntypes, DTIMAGE_FLAG_PAGED, F);
  if (rc != DT_RC_GOOD)
    return rc;

  rc = dtimage_check_1f(ctl,			/* check simple stuff				*/
    output, NULL,
    ntypes, DTIMAGE_FLAG_PAGED, F);
  if (rc != DT_RC_GOOD)
    return rc;

  if (DT_DBG(ctl, DT_DBG_MASK_IMG))
  {
	char s1[32], s2[32], s3[32], s4[32];
	DT_Q(dtimage_to_string,(ctl,
      input1, NULL, s1, sizeof(s1)));
	DT_Q(dtimage_to_string,(ctl,
      input1, NULL, s2, sizeof(s2)));
	DT_Q(dtimage_to_string,(ctl,
      input1, NULL, s3, sizeof(s3)));
	DT_Q(dtstr_print_op,(ctl, op,
      s4, sizeof(s4)));
	dt_dbg(ctl, F, DT_DBG_MASK_IMG,
      "%s * %g %s %s * %g %s %g -> %s",
      s1, a, s4, s2, b, s4, c, s3);
  }

  xmax = DT_MIN(DT_MIN(output->xe,
    input1->xe), input2->xe);

  ymax = DT_MIN(DT_MIN(output->ye,
    input1->ye), input2->ye);

  if (
	DTNTYPE_ISFLOAT(input1->ntype) ||	/* either input is floating? 		*/
    DTNTYPE_ISFLOAT(input2->ntype) ||	
	DTNTYPE_ISFLOAT(output->ntype) ||	/* or output is floating? 			*/
	ceil(a) != a ||
	ceil(b) != b ||
	ceil(c) != c)
    ntype = DT_NTYPE_F2;				/* do floating arithmetic 			*/
  else
  if (									
	DTNTYPE_ISSIGNED(input1->ntype) ||	/* either input is signed? 			*/
    DTNTYPE_ISSIGNED(input2->ntype) ||	
	DTNTYPE_ISSIGNED(output->ntype) ||	/* or output is signed? 			*/
	a < 0.0 ||
	b < 0.0 ||
	c < 0.0)
    ntype = DT_NTYPE_S4;				/* use signed arithmetic 			*/
  else								
    ntype = DT_NTYPE_B32;				/* use unsigned arithmetic 			*/

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

#if 0
dt_dbg(ctl, F, DT_DBG_MASK_ANY, 
"a=%g b=%g c=%g *in1=0x%08lx *in2=0x%08lx *out=0x%08lx", a, b, c,
*(dtntype_b32_t *)(in1? in1: in1p),
*(dtntype_b32_t *)(in2? in2: in2p),
*(dtntype_b32_t *)(out? out: outp));
#endif

    switch(op)							/* do the operation 				*/
	{
	  case DT_OP_ADD:
        DT_C_1D(dt1d_add,(ntype,
          in1? in1: in1p, xmax, a,
		  in2? in2: in2p, xmax, b, c,
          out? out: outp, xmax));
	  break;
	  case DT_OP_SUB:
        DT_C_1D(dt1d_sub,(ntype,
          in1? in1: in1p, xmax, a,
		  in2? in2: in2p, xmax, b, c,
          out? out: outp, xmax));
	  break;
	  case DT_OP_MUL:
        DT_C_1D(dt1d_mult,(ntype,
          in1? in1: in1p, xmax, a,
		  in2? in2: in2p, xmax, b, c,
          out? out: outp, xmax));
	  break;
	  case DT_OP_DIV:
        DT_C_1D(dt1d_div,(ntype,
          in1? in1: in1p, xmax, a,
		  in2? in2: in2p, xmax, b, c,
          out? out: outp, xmax));
	  break;
	  default:
	    rc = dt_err(ctl, F,
          "bad op %d", op);
		goto cleanup;
	}

	if (assign_flags & DT_ASSIGN_ABS)
	  DT_C_1D(dt1d_abs,(ntype,			/* absolute value if desired 		*/
        out? out: outp, xmax));

	if (assign_flags & DT_ASSIGN_CLIP)
	  DT_C_1D(dt1d_clip,(ntype,			/* clip if desired 					*/
        out? out: outp, xmax,
        dt_ntype_minval[output->ntype],
        dt_ntype_maxval[output->ntype]));

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
