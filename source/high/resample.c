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
  20000506 DE fixed bug when enlarging b08 by 2
  19981208 DE optimized
 *..........................................................................*/

/*..........................................................................
| NAME
| dturesample utility - 
| 
| DESCRIPTION
| As yet undocumented.
| 
| RETURN VALUES
| Program exit value will be DT_EXIT_GOOD on success.  
| Any other exit value indicates failure or noncompletion.
| 
| ERRORS
| As yet undocumented.
| 
| END
 *..........................................................................*/

#include <dtack/base.h>
DT_RCSID("source/high $RCSfile: resample.c,v $ $Revision: 1.2 $");
#include <dtack/os.h>
#include <dtack/mem.h>
#include <dtack/label.h>

/*..........................................................................*/

static
dt_rc_e 
dtimage_resample_nearest_b08_shrink2(	/* subsample 8-bit image by 2		*/
  dt_ctl_t *ctl,
  dtimage_t *input,
  dtxy_t xe,
  dtxy_t ye,
  dtimage_t *output)
{
  DT_F("dtimage_resample"
	   "_nearest_b08_shrink2");
  dtxy_t xo, xi, y;

  for (y=0; y<ye; y++)
  {
	const dt_ntype_b08_t *p1;
	dt_ntype_b08_t *q1;
	DTIMAGE_GETROW(ctl, input, y*2, &p1);
	DTIMAGE_MAPROW(ctl, output, y, &q1);
	{
	  const dt_ntype_b08_t * const p = p1;
	  dt_ntype_b08_t * const q = q1;
	  for (xi=0, xo=0; xo<xe; xi+=2, xo++)
	    q[xo] = p[xi];
	}
	DTIMAGE_PUTROW(ctl, output, y);
	DTIMAGE_UNMAPROW(ctl, input, y*2);
  }
  
  return DT_RC_GOOD;
}

/*..........................................................................*/

static
dt_rc_e 
dtimage_resample_nearest_b08_enlarge2(	/* oversample 8-bit image by 2		*/
  dt_ctl_t *ctl,
  dtimage_t *input,
  const dtxy_t xe,
  const dtxy_t ye,
  dtimage_t *output)
{
  DT_F("dtimage_resample"
	   "_nearest_b08_enlarge2");
  dtxy_t x, y;

  for (y=0; y<ye; y++)					/* traverse output image		    */
  {
	const dt_ntype_b08_t *p1;
	dt_ntype_b08_t *q1;
	dt_ntype_b08_t *q2;
	DTIMAGE_GETROW(ctl, input, y/2, &p1);
	DTIMAGE_MAPROW(ctl, output, y, &q1);
	if (y<ye-1)
	  DTIMAGE_MAPROW(ctl, output, y+1, &q2);
	{
	  const dt_ntype_b08_t * const p = p1;
	  dt_ntype_b08_t * const qc1 = q1;
	  dt_ntype_b08_t * const qc2 = q2;
	  dtxy_t x2 = 0;
	  if (y<ye-1)
      for (x=0; x<xe; x+=2)
	  {
		qc1[x] = qc1[x+1] =
		qc2[x] = qc2[x+1] = p[x2];
		x2++;
	  }
	  else
      for (x=0; x<xe; x+=2)
	  {
		qc1[x] = qc1[x+1] = p[x2];
		x2++;
	  }
	}
	if (y<ye-1)
	  DTIMAGE_PUTROW(ctl, output, y+1);
	DTIMAGE_PUTROW(ctl, output, y);
	DTIMAGE_UNMAPROW(ctl, input, y/2);
  }
  return DT_RC_GOOD;
}

/*..........................................................................*/

static
dt_rc_e 
dtimage_resample_nearest_any(			/* resample 8-bit image				*/
  dt_ctl_t *ctl,
  dtimage_t *input,
  double dx, 
  double dy,
  dtxy_t xe,
  dtxy_t ye,
  dtimage_t *output)
{
  DT_F("dtimage_resample"
	   "_nearest_any");
  dtxy_t x, y;
  if (input->ntype == DTNTYPE_B01)
	return dt_err(ctl, F,
      "bit images not supported");

  for (y=0; y<ye; y++)					/* traverse output image		    */
  {
	const void *p1;
	void *q1;
	DTIMAGE_GETROW(ctl, input, (dtxy_t)(y * dy), &p1);
	DTIMAGE_MAPROW(ctl, output, y, &q1);

#   define CASE(E, T)					/* enumeration->datatype case 		*/\
    case E:																	  \
	{																		  \
	  const T * const p = (T *)p1;											  \
	  T * const q = (T *)q1;												  \
	  const dtxy_t xe2 = xe;												  \
	  for (x=0; x<xe2; x++)													  \
		q[x] = p[(dtxy_t)(x * dx)];											  \
	}																		  \
    break;

	switch(input->ntype)
	{
	  DTNTYPE_CASES
	  default:
		return dt_err(ctl, F,
          "unrecognized ntype %d", 
          input->ntype);
	}

	DTIMAGE_PUTROW(ctl, output, y);
	DTIMAGE_UNMAPROW(ctl, input, (dtxy_t)(y * dy));
  }
  return DT_RC_GOOD;
}

/*..........................................................................*/

static
void
resample_nearest_b01_shrink2(			/* resample to output from input	*/
  dt_ctl_t *ctl,
  dtimage_t *input,
  dtimage_t *output)
{
  int i;
  dt_ntype_b01_t table[256];
  dtxy_t x, y;
  dtxy_t xe;
  dt_ntype_b01_t *in, *out;

  for (i=0; i<256; i++)					/* make table					    */
    table[i] =
      (((i & 0x80) != 0) << 3) |
      (((i & 0x20) != 0) << 2) |
      (((i & 0x08) != 0) << 1) |
      (((i & 0x02) != 0));

  xe = input->xe / 8;

  for (y=0; y<input->ye; y+=2)
  {
    in = input->row[y].b01;
    out = output->row[y>>1].b01;

    for (x=0; x<xe; x+=2)
	  out[x>>1] =
        (table[in[x]] << 4) |
        table[in[x+1]];
  }
}


/*..........................................................................*/

static
void
resample_nearest_b01_shrink4(			/* resample to output from input	*/
  dt_ctl_t *ctl,
  dtimage_t *input,
  dtimage_t *output)
{
  int i;
  dt_ntype_b01_t table[256];
  dtxy_t x, y;
  dtxy_t xe;
  dt_ntype_b01_t *in;
  dt_ntype_b01_t *out;

  for (i=0; i<256; i++)					/* make table					    */
    table[i] =
      (((i & 0xf0) != 0) << 1) |
       ((i & 0x0f) != 0);

  xe = input->xe / 8;

  for (y=0; y<input->ye; y+=4)
  {
    in = input->row[y].b01;
    out = output->row[y>>2].b01;

    for (x=0; x<xe; x+=4)
	  out[x>>2] =
        (table[in[x]]   << 6) |
        (table[in[x+1]] << 4) |
        (table[in[x+2]] << 2) |
         table[in[x+3]];
  }
}

/*..........................................................................*/

static
void
resample_max_b01_shrink2(				/* resample to output from input	*/
  dt_ctl_t *ctl,
  dtimage_t *input,
  dtimage_t *output)
{
  int i;
  dt_ntype_b01_t table[256];
  dtxy_t x, y;
  dtxy_t xe;
  dt_ntype_b01_t *in1, *in2, *out;

  for (i=0; i<256; i++)					/* make table					    */
    table[i] =
      (((i & 0xc0) != 0) << 3) |
      (((i & 0x30) != 0) << 2) |
      (((i & 0x0c) != 0) << 1) |
      (((i & 0x03) != 0));

  xe = input->xe / 8;

  for (y=0; y<input->ye; y+=2)
  {
    in1 = input->row[y].b01;
    in2 = input->row[y+1].b01;
    out = output->row[y>>1].b01;

    for (x=0; x<xe; x+=2)
	  out[x>>1] =
        ((table[in1[x]] << 4) |
          table[in1[x+1]])   |
        ((table[in2[x]] << 4) |
          table[in2[x+1]]);
  }
}

/*..........................................................................*/

static
void
resample_max_b01_shrink4(						/* resample to output from input	*/
  dt_ctl_t *ctl,
  dtimage_t *input,
  dtimage_t *output)
{
  int i;
  dt_ntype_b01_t table[256];
  dtxy_t x, y;
  dtxy_t xe;
  dt_ntype_b01_t *in1, *in2, *in3, *in4;
  dt_ntype_b01_t *out;

  for (i=0; i<256; i++)					/* make table					    */
    table[i] =
      (((i & 0xf0) != 0) << 1) |
       ((i & 0x0f) != 0);

  xe = input->xe / 8;

  for (y=0; y<input->ye; y+=4)
  {
    in1 = input->row[y].b01;
    in2 = input->row[y+1].b01;
    in3 = input->row[y+2].b01;
    in4 = input->row[y+3].b01;
    out = output->row[y>>2].b01;

    for (x=0; x<xe; x+=4)
	  out[x>>2] =
        (table[in1[x]]   << 6) |
        (table[in1[x+1]] << 4) |
        (table[in1[x+2]] << 2) |
        (table[in1[x+3]]     ) |
        (table[in2[x]]   << 6) |
        (table[in2[x+1]] << 4) |
        (table[in2[x+2]] << 2) |
        (table[in2[x+3]]     ) |
        (table[in3[x]]   << 6) |
        (table[in3[x+1]] << 4) |
        (table[in3[x+2]] << 2) |
        (table[in3[x+3]]     ) |
        (table[in4[x]]   << 6) |
        (table[in4[x+1]] << 4) |
        (table[in4[x+2]] << 2) |
        (table[in4[x+3]]);

  }
}

/*..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtimage_resample_bilinear(				/* resample to output from input	*/
  dt_ctl_t *ctl,
  dtimage_t *input,
  double dx,							/* stepsize over input image	    */
  double dy,
  dtimage_t *output)
{
  DT_F("dtimage_resample_bilinear");
  static dt_ntype_e *ntypes = NULL;		/* we can handle all ntypes 		*/
  dtxy_t input_xe;
  dtxy_t output_xe, output_ye;
  dtxy_t x, y;
  dtxy_t yupper = 0;
  dtxy_t ylower = 0;
  dt_ntype_f2_t *p = NULL;
  dt_ntype_f2_t *upper = NULL;
  dt_ntype_f2_t *lower = NULL;
  const double roundoff =  
    DTNTYPE_ISFLOAT(output->ntype)?
    0.0: 0.5;
  void *q;
  double tmp;
  dt_rc_e rc;

  rc = dtimage_check_2f(ctl,			/* tracer						    */
    input, "input",
    output, "output",
    ntypes, DTIMAGE_FLAG_PAGED, F,
    NULL, NULL);
  if (rc != DT_RC_GOOD)
    return rc;

  tmp = (input->xe-1) / dx;
  output_xe = DT_MIN(output->xe,		/* limit size of output image	    */
    DT_CAST(dtxy_t, tmp) + 1);
  tmp = (input->ye-1) / dy;
  output_ye = DT_MIN(output->ye, 
    DT_CAST(dtxy_t, tmp) + 1);

  input_xe = DT_MIN(input->xe,
    DT_CAST(dtxy_t, 
      (output_xe * dx + 2)));
  
  DT_C(dtos_malloc2,(ctl,				/* space for the upper row 			*/
    (void **)&upper,
    input_xe * sizeof(*upper),
    F, "upper row"));

  DT_C(dtos_malloc2,(ctl,				/* space for the lower row 			*/
    (void **)&lower,
    input_xe * sizeof(*lower),
    F, "lower row"));
  DT_MEMSET(lower, 0,					/* in 1:1 never gets loaded		    */
   input_xe * sizeof(*lower));

  DT_C(dtos_malloc2,(ctl,				/* space for the output row 		*/
    (void **)&p,
    output_xe * sizeof(*p),
    F, "output row"));
  
  for (y=0; y<output_ye; y++)			/* for each output row 				*/
  {
	double ysrc = (double)y * dy;
	if (y == 0 || 
        (dtxy_t)ysrc != yupper)			/* need new input row? 				*/
	{
      yupper = (dtxy_t)ysrc;
	  DTIMAGE_GETROW2(DT_C, ctl, 
        input, yupper, &q);
      DT_C_1D(dt1d_cast,(				/* promote upper input row 			*/
        q, input_xe, input->ntype, 
        upper, input_xe, DT_NTYPE_F2));
	  DTIMAGE_UNMAPROW2(DT_C, ctl,
        input, yupper);
	}
	  
	if (ysrc != (double)yupper &&		/* not smack on upper row?		    */
        (dtxy_t)ysrc+1 != ylower)		/* need new input row? 				*/
	{
      ylower = (dtxy_t)ysrc + 1;
  	  DTIMAGE_GETROW2(DT_C, ctl, 
        input, ylower, &q);
	  DT_C_1D(dt1d_cast,(				/* promote lower input row 			*/
        q, input_xe, input->ntype, 
        lower, input_xe, DT_NTYPE_F2));
	  DTIMAGE_UNMAPROW2(DT_C, ctl,
        input, ylower);
	}

	{
	  const double y2 = ysrc - yupper;
	  const double y1 = 1.0 - y2;
      for (x=0; x<output_xe; x++)		/* for each output column 			*/
	  {
		const double xsrc = 
          (double)x * dx;
		const dtxy_t left = (dtxy_t)xsrc;
		const double x2 = xsrc - left;
		const double x1 = 1.0 - x2;
		
        if (x2 != 0.0)					/* not smack on a column?		    */
	  	  p[x] = roundoff +
            upper[left]   * x1 * y1 +
            upper[left+1] * x2 * y1 +
		    lower[left]   * x1 * y2 +
            lower[left+1] * x2 * y2;
		else
	  	  p[x] = 
            upper[left]   * y1 +
		    lower[left]   * y2;
	  }
	}

    DTIMAGE_GETROW2(DT_C, ctl,
      output, y, &q);
	DT_Q_1D(dt1d_cast,(					/* convert answer to output type 	*/
      p, output_xe, DT_NTYPE_F2,
	  q, output_xe, output->ntype));
	DTIMAGE_PUTROW2(DT_C, ctl, 
      output, y);
  }

cleanup:

  if (p != NULL)
    DT_I(dtos_free2,(ctl, p,			/* free space for the output row 	*/
      F, "output row"));

  if (upper != NULL)
    DT_I(dtos_free2,(ctl, upper,		/* free space for the upper row 	*/
      F, "upper row"));

  if (lower != NULL)
    DT_I(dtos_free2,(ctl, lower,		/* free space for the lower row 	*/
      F, "lower row"));

  return rc;
}

/*..........................................................................
 * return size of output image based on input image size and algorithm
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtimage_resample_size(					/* return resampled image size		*/
  dt_ctl_t *ctl,
  dtimage_t *input,
  double dx,							/* stepsize over input image	    */
  double dy,
  const char *op,
  dtxy_t *xe,							/* returned image width 			*/
  dtxy_t *ye)							/* returned image height			*/
{
  DT_F("dtimage_resample_size");
  double tmp;

  if (!strcmp(op, "bilinear") ||		/* bilinear interpolation? 			*/
	  !strcmp(op, "nearest+1"))			/* or nearest-plus-1?				*/
  {
    tmp = (double)(input->xe-1) / dx;
    *xe = DT_CAST(dtxy_t, tmp) + 1;		/* width of output image		    */

    tmp = (double)(input->ye-1) / dy;
    *ye = DT_CAST(dtxy_t, tmp) + 1;		/* height of output image 			*/
  }
  else
  {
    tmp = (double)(input->xe) / dx;
    *xe = DT_CAST(dtxy_t, tmp);			/* width of output image		    */

    tmp = (double)(input->ye) / dy;
    *ye = DT_CAST(dtxy_t, tmp);			/* height of output image 			*/
  }

  dt_dbg(ctl, F, DT_DBG_MASK_COMPLEX,
    "%s (double)(%u-1) / %g = %0.18e -> %u",
    op, input->ye, dy, tmp, *ye);

  return DT_RC_GOOD;
}

/*..........................................................................
 * return scale which will produce given output size
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtimage_resample_scale(					/* return resampled image size		*/
  dt_ctl_t *ctl,
  dtimage_t *input,
  dtxy_t xe,							/* desired image width 				*/
  dtxy_t ye,							/* desired image height				*/
  const char *op,
  double *dx,							/* returned resample scale			*/
  double *dy)
{
  DT_F("dtimage_resample_scale");
  double tmp;

  if (!strcmp(op, "bilinear") ||		/* bilinear interpolation? 			*/
	  !strcmp(op, "nearest+1"))			/* or nearest-plus-1?				*/
  {
    *dx = ((double)input->xe-1.0) / 
               ((double)xe-1);
    *dy = ((double)input->ye-1.0) / 
               ((double)ye-1);
  }
  else
  {
	*dx = (double)(input->xe) / 
             (double)xe;
	*dy = (double)(input->ye) / 
             (double)ye;
  }

  return DT_RC_GOOD;
}

/*..........................................................................
 * resample fast if we can
 * allocate output image if not already allocated
 * leave image allocated even if returning cando=0
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtimage_resample_fast(					/* resample to output from input	*/
  dt_ctl_t *ctl,
  dtimage_t *input,
  double dx,							/* stepsize over input image	    */
  double dy,
  const char *op,
  dtimage_t *output,
  int *cando)							/* return if we did it fast 		*/
{
  DT_F("dtimage_resample_fast");
  dtxy_t xe, ye;
  dtxy_t x, y;

  DT_Q(dtimage_resample_size,(ctl,		/* determine resampled image size 	*/
    input, dx, dy, op, &xe, &ye));

  if (!strcmp(op, "nearest+1"))			/* op is nearest-plus-1?			*/
	op = "nearest";						/* process like nearest 			*/

  if (output->xe == 0)					/* output image not allocated? 		*/
  {
    DT_Q(dtimage_create,(ctl, output,	/* space for output image		    */
      xe, ye, input->ntype));
  }
  else									/* output image is allocated? 		*/
  {
	xe = DT_MIN(xe, output->xe);		/* don't overstep output image 		*/
	ye = DT_MIN(ye, output->ye);
  }

  *cando = 1;							/* presume we can do it fast 		*/
										/* ................................ */
  if (!strcmp(op, "nearest") &&			/* ignore neighborhood?			    */
      input->ntype == DT_NTYPE_B01 &&	/* and input is bit image?			*/
      output->ntype == DT_NTYPE_B01 &&	/* and output is bit image?			*/
      dx == 2.0 && dy == 2.0)			/* and subsample by 2?			    */
    resample_nearest_b01_shrink2(ctl,
      input, output);
  else
										/* ................................ */
  if (!strcmp(op, "nearest") &&			/* ignore neighborhood?			    */
      input->ntype == DT_NTYPE_B01 &&	/* and input is bit image?			*/
      output->ntype == DT_NTYPE_B01 &&	/* and output is bit image?			*/
      dx == 4.0 && dy == 4.0)			/* and subsample by 4?			    */
    resample_nearest_b01_shrink4(ctl,
      input, output);
  else
										/* ................................ */
  if (!strcmp(op, "max") &&				/* max of neighborhood?			    */
      input->ntype == DT_NTYPE_B01 &&	/* and input is bit image?			*/
      output->ntype == DT_NTYPE_B01 &&	/* and output is bit image?			*/
      dx == 2.0 && dy == 2.0)			/* and subsample by 2?			    */
    resample_max_b01_shrink2(ctl,
      input, output);
  else
										/* ................................ */
  if (!strcmp(op, "max") &&				/* max of neighborhood?			    */
      input->ntype == DT_NTYPE_B01 &&	/* and input is bit image?			*/
      output->ntype == DT_NTYPE_B01 &&	/* and output is bit image?			*/
      dx == 4.0 && dy == 4.0)			/* and subsample by 4?			    */
    resample_max_b01_shrink4(ctl,
      input, output);
  else
										/* ................................ */
  if (!strcmp(op, "nearest") &&			/* nearest neighbor?			    */
      input->ntype == DT_NTYPE_B08 &&	/* and input is byte image?			*/
      output->ntype == DT_NTYPE_B08 &&	/* and output is byte image?		*/
      dx == 2.0 && dy == 2.0)			/* and subsample by 2?			    */
  {
    DT_Q(dtimage_resample_nearest_b08_shrink2,(ctl,
      input, xe, ye, output))
  }
  else
										/* ................................ */
  if (!strcmp(op, "nearest") &&			/* nearest neighbor?			    */
      input->ntype == DT_NTYPE_B08 &&	/* and input is byte image?			*/
      output->ntype == DT_NTYPE_B08 &&	/* and output is byte image?		*/
      dx == 0.5 && dy == 0.5)			/* and enlarge by 2?			    */
  {
    DT_Q(dtimage_resample_nearest_b08_enlarge2,(ctl,
      input, xe, ye, output))
  }
  else
										/* ................................ */
  if (!strcmp(op, "nearest")&			/* nearest neighbor?			    */
      input->ntype != DT_NTYPE_B01 &&	/* and input is not a bit image?	*/
      output->ntype != DT_NTYPE_B01 &&	/* and output is not a bit image?	*/
	  input->ntype == output->ntype)	/* and input and output same types? */
  {
    DT_Q(dtimage_resample_nearest_any,(ctl,
      input, dx, dy, xe, ye, output))
  }
  else
										/* ................................ */
	*cando = 0;							/* we can't do it fast 				*/

  return DT_RC_GOOD;
}


/*..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtimage_resample(						/* resample to output from input	*/
  dt_ctl_t *ctl,
  dtimage_t *input,
  double dx,							/* stepsize over input image	    */
  double dy,
  const char *op,
  dtimage_t *output)
{
  DT_F("dtimage_resample");
  dtxy_t xe, ye;
  dtxy_t x, y;
  int cando;
  dt_rc_e rc;

  rc = dtimage_check_1f(ctl,			/* tracer						    */
    input, "input",
    dt1d_ntype_list,
    DTIMAGE_FLAG_PAGED, F);
  if (rc != DT_RC_GOOD)
    return rc;

  if (output->xe)
  rc = dtimage_check_1f(ctl,			/* tracer						    */
    output, "output",
    dt1d_ntype_list,
    DTIMAGE_FLAG_PAGED, F);
  if (rc != DT_RC_GOOD)
    return rc;

  DT_Q(dtimage_resample_fast,(ctl,		/* try to do it fast 				*/
    input, dx, dy, op, output,
    &cando));

  if (cando)							/* we did it fast? 					*/
	return DT_RC_GOOD;

  DT_Q(dtimage_resample_size,(ctl,		/* determine resampled image size 	*/
    input, dx, dy, op, &xe, &ye));

  if (output->xe == 0)					/* output image not allocated? 		*/
  {
    DT_Q(dtimage_create,(ctl, output,	/* space for output image		    */
      xe, ye, input->ntype));
  }
  else									/* output image is allocated? 		*/
  {
	xe = DT_MIN(xe, output->xe);		/* don't overstep output image 		*/
	ye = DT_MIN(ye, output->ye);
  }

  if (!strcmp(op, "nearest+1"))			/* op is nearest-plus-1?			*/
	op = "nearest";						/* process like nearest 			*/

  if (!strcmp(op, "nearest"))			/* nearest neighbor?			    */
  {
    for (y=0; y<ye; y++)				/* traverse output image		    */
    {
      for (x=0; x<xe; x++)
      {
	    double pixel;
	    DT_Q(dtimage_get_pixel,(ctl,	/* fetch						    */
          input,
          (dtxy_t)((double)x * dx),
          (dtxy_t)((double)y * dy),
          &pixel));
        DT_Q(dtimage_set_pixel,(ctl,	/* store					  	    */
          output, x, y, pixel));
	  }
	}
  }
  else
  if (!strcmp(op, "max"))				/* max of neighborhood?			    */
  {
    for (y=0; y<ye; y++)				/* traverse output image		    */
    {
      for (x=0; x<xe; x++)
      {
	    double pixel, max;
        dtxy_t x0 = (dtxy_t)((double)(x+0) * dx);
        dtxy_t x1 = (dtxy_t)((double)(x+1) * dx);
        dtxy_t y0 = (dtxy_t)((double)(y+0) * dy);
        dtxy_t y1 = (dtxy_t)((double)(y+1) * dy);
        dtxy_t i, j;
        DT_Q(dtimage_get_pixel,(		/* fetch first					    */
          ctl, input, x0, y0,
          &max));
        for (j=y0; j<y1; j++)
		{
          for (i=x0; i<x1; i++)
		  {
  	        DT_Q(dtimage_get_pixel,(	/* fetch others in resampled area	*/
              ctl, input, i, j,
              &pixel));
            max = DT_MAX(max, pixel);
		  }
		}
        DT_Q(dtimage_set_pixel,(ctl,	/* store max in resampled area 	    */
          output, x, y, max));
	  }
	}
  }
  else
  if (!strcmp(op, "bilinear"))			/* bilinear interpolation?			*/
  {
    DT_Q(dtimage_resample_bilinear,(
	  ctl, input, dx, dy, output));
  }
  else
	return dt_err(ctl, F,
      DT_LABEL_ADD(DT_LABEL_INVALID,
        "invalid resample algorithm")
	  " \"%s\"", op? op: "NULL");

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
