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

/*..........................................................................
 *..........................................................................*/

static
void
add_scalar_b08_b08(						/* add image and scalar  		*/
  dt_ctl_t *ctl,
  dtimage_t *input,
  dtxy_t xmax,
  dtxy_t ymax,
  int scalar,
  dtimage_t *output)
{
  dtxy_t x;
  dtxy_t y;
  dt_ntype_b08_t *p;					/* coursing pointers				*/
  dt_ntype_b08_t *q;
  for (y=0; y<ymax; y++)				/* for all rows						*/
  {
	p = input->row[y].b08;				/* start of this row				*/
	q = output->row[y].b08;
	for (x=0; x<xmax; x++)				/* for all columns					*/
      q[x] = p[x] + scalar;
  }
}

/*..........................................................................
 *..........................................................................*/

static
void
add_scalar_b32_b08(						/* add image and scalar  		*/
  dt_ctl_t *ctl,
  dtimage_t *input,
  dtxy_t xmax,
  dtxy_t ymax,
  long scalar,
  dtimage_t *output)
{
  dtxy_t x;
  dtxy_t y;
  dt_ntype_b32_t *p;					/* coursing pointers				*/
  dt_ntype_b08_t *q;
  for (y=0; y<ymax; y++)				/* for all rows						*/
  {
	p = input->row[y].b32;				/* start of this row				*/
	q = output->row[y].b08;
	for (x=0; x<xmax; x++)				/* for all columns					*/
      q[x] = p[x] + scalar;
  }
}

/*..........................................................................
 *..........................................................................*/

static
void
add_scalar_b32_b32(						/* add image and scalar  		*/
  dt_ctl_t *ctl,
  dtimage_t *input,
  dtxy_t xmax,
  dtxy_t ymax,
  long scalar,
  dtimage_t *output)
{
  dtxy_t x;
  dtxy_t y;
  dt_ntype_b32_t *p;					/* coursing pointers				*/
  dt_ntype_b32_t *q;
  for (y=0; y<ymax; y++)				/* for all rows						*/
  {
	p = input->row[y].b32;				/* start of this row				*/
	q = output->row[y].b32;
	for (x=0; x<xmax; x++)				/* for all columns					*/
      q[x] = p[x] + scalar;
  }
}

/*..........................................................................
| NAME
| dtimage_add_scalar() - add offset to each pixel
| 
| SUMMARY
| Implement output = input + offset for each pixel.
| Truncate high order bits if the output precision is too low
| (except single-bit output which is considered boolean).
| Permit the input image to be the same as the output image.
| 
| END
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtimage_add_scalar(						/* add scalar to each pixel			*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtimage_t *input,						/* input image						*/
  double offset,						/* offset							*/
  dtimage_t *output)					/* output image						*/
{
  DT_F("dtimage_add_scalar");
  dtxy_t xmax, ymax;
  dt_ntype_e *ntypes = NULL;			/* we can handle any ntype		    */
  dt_rc_e rc;

  rc = dtimage_check_2(ctl,				/* check simple stuff				*/
    input, "input",
    output, "output",
    ntypes, F, &xmax, &ymax);
  if (rc != DT_RC_GOOD)
    return rc;

  switch(DT_NTYPE_PAIR(input->ntype,	/* branch based on ntype			*/
           output->ntype))
  {
	case DT_NTYPE_PAIR(DT_NTYPE_B08, DT_NTYPE_B08):
      add_scalar_b08_b08(ctl,
        input, xmax, ymax,
        (int)offset, output);
    break;
	case DT_NTYPE_PAIR(DT_NTYPE_B32, DT_NTYPE_B08):  
      add_scalar_b32_b08(ctl,
        input, xmax, ymax,
        (long)offset, output);
    break;
	case DT_NTYPE_PAIR(DT_NTYPE_B32, DT_NTYPE_B32):  
      add_scalar_b32_b32(ctl,
        input, xmax, ymax,
        (long)offset, output);
    break;
    default:
	{
      dtxy_t x, y;
      double d;
      for (y=0; y<ymax; y++)
        for (x=0; x<xmax; x++)
        {
		  DT_Q(dtimage_get_pixel,(ctl,	/* fetch source pixel			    */
            input, x, y, &d));
          d = d + offset;				/* do operation					    */
          DT_Q(dtimage_set_pixel,(ctl,	/* store output pixel			    */
            output, x, y, d));
		}
	}
  }

  return DT_RC_GOOD;
}

/*..........................................................................
| NAME
| dtimage_subabs_scalar() - absolute difference of each pixel and offset
| 
| SUMMARY
| Implement output = ABS(input - offset) for each pixel
| Truncate high order bits if the output precision is too low
| (except single-bit output which is considered boolean).
| Permit the input and output images to be the same.
| 
| END
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtimage_subabs_scalar(					/* subtract image and offset		*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtimage_t *input,						/* input image						*/
  unsigned long offset,					/* offset							*/
  dtimage_t *output)					/* output image						*/
{
  DT_F("dtimage_subabs_scalar");
  void *vpro = NULL;
  dtxy_t xe, ye;
  dtxy_t x, y;
  dt_ntype_e *ntypes = NULL;			/* we can handle any ntype		    */
  const int do_f2 =
    (DTNTYPE_ISFLOAT(input->ntype) ||
     DTNTYPE_ISFLOAT(output->ntype));
  const dt_ntype_s4_t s4offset = 
    DT_CAST(dt_ntype_s4_t, offset);
  const dt_ntype_f2_t f2offset = 
    DT_CAST(dt_ntype_f2_t, offset);
  void *pin = NULL;
  void *pout = NULL;
  dt_rc_e rc;

  rc = dtimage_check_2f(ctl,			/* check simple stuff				*/
    input, "input",
    output, "output",
    ntypes, DTIMAGE_FLAG_PAGED,
    F, &xe, &ye);
  if (rc != DT_RC_GOOD)
    return rc;

  DT_C(dtos_malloc2,(ctl,				/* space for one promoted row 		*/
    &vpro, xe *
      (do_f2? 
        sizeof(dt_ntype_f2_t):
        sizeof(dt_ntype_s4_t)),
    F, "pro row"));

  for (y=0; y<ye; y++)
  {
	DTIMAGE_GETROW2(DT_C, ctl, input, y, &pin);
	if (do_f2)
	{
	  dt_ntype_f2_t * const f2pro =
        (dt_ntype_f2_t *)vpro;
  	  DT_C(dt1d_cast,(					/* promote raw input row to double	*/
        pin, xe, input->ntype,
        vpro, xe, DT_NTYPE_F2));
	  for (x=0; x<xe; x++)				/* subtract scalar and take abs		*/
	    if ((f2pro[x] -= f2offset) < 0.0)
          f2pro[x] = -f2pro[x];
	}
	else
	{
	  dt_ntype_s4_t * const s4pro =
        (dt_ntype_s4_t *)vpro;
  	  DT_C(dt1d_cast,(					/* promote raw input row to long	*/
        pin, xe, input->ntype,
        vpro, xe, DT_NTYPE_S4));
	  for (x=0; x<xe; x++)				/* subtract scalar and take abs		*/
	    if ((s4pro[x] -= s4offset) < 0)
          s4pro[x] = -s4pro[x];
	}
	DTIMAGE_UNMAPROW2(DT_C, ctl, input, y);
	pin = NULL;

	DTIMAGE_GETROW2(DT_C, ctl, output, y, &pout);
	DT_C(dt1d_cast,(
      vpro, xe, do_f2? 
        DT_NTYPE_F2: 
        DT_NTYPE_S4,
      pout, xe, output->ntype));
	DTIMAGE_PUTROW2(DT_C, ctl, output, y);
	pout = NULL;
  }

cleanup:
  if (pin != NULL)						/* dangling map? 					*/
	DTIMAGE_UNMAPROW2(DT_I, ctl,
	  input, y);
  if (pout != NULL)						/* dangling map? 					*/
	DTIMAGE_UNMAPROW2(DT_I, ctl, 
      output, y);
  if (vpro != NULL)
    DT_I(dtos_free2,(ctl,
      vpro,
      F, "pro row"));
  return rc;
}

/*..........................................................................
 *..........................................................................*/

static
void
subabs_image_b08(						/* "subabs" image and image 	 	*/
  dt_ctl_t *ctl,
  dtimage_t *input1,
  dtimage_t *input2,
  dtxy_t xmax,
  dtxy_t ymax,
  dtimage_t *output)
{
  dtxy_t x;
  dtxy_t y;
  for (y=0; y<ymax; y++)				/* for all rows						*/
  {
	dt_ntype_b08_t *p1;					/* coursing pointers				*/
	dt_ntype_b08_t *p2;
	dt_ntype_b08_t *q;
	p1 = input1->row[y].b08;			/* start of this row				*/
	p2 = input2->row[y].b08;		
	q = output->row[y].b08;
	for (x=0; x<xmax; x++)				/* for all columns					*/
	{
      int d = (int)p1[x] - (int)p2[x];
      if (d >= 0)
	    q[x] = d;
      else
        q[x] = -d;
	}
  }
}

/*..........................................................................
| NAME
| dtimage_subabs_image() - absolute difference between pixels
| 
| SUMMARY
| Implement output = ABS(input1 - input2) for each pixel.
| Truncate high order bits if the output precision is too low
| (except single-bit output which is considered boolean).
| Permit the input image to be the same as the output image.
| Operate only on the intersection of the three image rectangles.
| 
| END
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtimage_subabs_image(					/* pixel subtraction absolute value	*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtimage_t *input1,					/* first input image				*/
  dtimage_t *input2,					/* second input image 				*/
  dtimage_t *output)					/* output image						*/
{
  DT_F("dtimage_subabs_image");
  dtxy_t xmax, ymax;
  dt_ntype_e ntypes[] =					/* datatypes we can handle			*/
    {DT_NTYPE_B08, DT_NTYPE_NULL};
  dt_rc_e rc;

  rc = dtimage_check_3(ctl,				/* check simple stuff				*/
    input1, "input1",
    input2, "input2",
    output, "output",
    ntypes, F, &xmax, &ymax);
  if (rc != DT_RC_GOOD)
    return rc;

  switch(input1->ntype)					/* branch based on ntype			*/
  {
	case DT_NTYPE_B08:  
      subabs_image_b08(ctl,
        input1, input2, xmax, ymax,
        output);
    break;
  }

  return DT_RC_GOOD;
}


/*..........................................................................
 *..........................................................................*/

static
void
mult_scalar_b08(						/* "mult" image and scalar  		*/
  dt_ctl_t *ctl,
  dtimage_t *input,
  const dtxy_t xmax,
  const dtxy_t ymax,
  const double scalar,
  dtimage_t *output)
{
  dtxy_t x;
  dtxy_t y;
  for (y=0; y<ymax; y++)				/* for all rows						*/
  {
	const dt_ntype_b08_t * const p =	/* coursing pointers				*/
	  input->row[y].b08;				/* start of this row				*/
	dt_ntype_b08_t * const q =
	  output->row[y].b08;
	for (x=0; x<xmax; x++)				/* for all columns					*/
      q[x] = (dt_ntype_b08_t)(
        (double)p[x] * scalar);
  }
}

/*..........................................................................
 *..........................................................................*/

static
void
mult_scalar_b32(						/* "mult" image and scalar  		*/
  dt_ctl_t *ctl,
  dtimage_t *input,
  const dtxy_t xmax,
  const dtxy_t ymax,
  const double scalar,
  dtimage_t *output)
{
  dtxy_t x;
  dtxy_t y;
  for (y=0; y<ymax; y++)				/* for all rows						*/
  {
	const dt_ntype_b32_t * const p =	/* coursing pointers				*/
	  input->row[y].b32;				/* start of this row				*/
	dt_ntype_b32_t * const q =
	  output->row[y].b32;
	for (x=0; x<xmax; x++)				/* for all columns					*/
      q[x] = (dt_ntype_b32_t)(
        (double)p[x] * scalar);
  }
}

/*..........................................................................
 *..........................................................................*/

static
void
mult_scalar_f2(							/* "mult" image and scalar  		*/
  dt_ctl_t *ctl,
  dtimage_t *input,
  const dtxy_t xmax,
  const dtxy_t ymax,
  const double scalar,
  dtimage_t *output)
{
  dtxy_t x;
  dtxy_t y;
  for (y=0; y<ymax; y++)				/* for all rows						*/
  {
	const dt_ntype_f2_t * const p =		/* coursing pointers				*/
	  input->row[y].f2;					/* start of this row				*/
	dt_ntype_f2_t * const q =
	  output->row[y].f2;
	for (x=0; x<xmax; x++)				/* for all columns					*/
      q[x] = p[x] * scalar;
  }
}

/*..........................................................................
| NAME
| dtimage_mult_scalar() - product of each pixel and constant factor
| 
| SUMMARY
| Implement output = input * factor for each pixel.
| Truncate high order bits if the output precision is too low
| (except single-bit output which is considered boolean).
| Permit the input image to be the same as the output image.
| 
| END
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtimage_mult_scalar(					/* "mult" pixels by scalar			*/
  dt_ctl_t *ctl,						/* environment control */
  dtimage_t *input,						/* input image						*/
  double factor,						/* constant 						*/
  dtimage_t *output)					/* output image						*/
{
  DT_F("dtimage_mult_scalar");
  dtxy_t xmax, ymax;
  dt_rc_e rc;

  rc = dtimage_check_2(ctl,				/* check simple stuff				*/
    input, "input",
    output, "output",
    NULL, F, &xmax, &ymax);
  if (rc != DT_RC_GOOD)
    return rc;

  switch(DT_NTYPE_PAIR(input->ntype, output->ntype))
  {
	case DT_NTYPE_PAIR(DT_NTYPE_B08, DT_NTYPE_B08):  
      mult_scalar_b08(ctl,
        input, xmax, ymax,
        factor, output);
    return DT_RC_GOOD;
	case DT_NTYPE_PAIR(DT_NTYPE_B32, DT_NTYPE_B32):  
      mult_scalar_b32(ctl,
        input, xmax, ymax,
        factor, output);
    return DT_RC_GOOD;
	case DT_NTYPE_PAIR(DT_NTYPE_F2, DT_NTYPE_F2):  
      mult_scalar_f2(ctl,
        input, xmax, ymax,
        factor, output);
    return DT_RC_GOOD;
    default:
    {									/* we have to do it the hard way    */
      dtxy_t x, y;
      double d;
      for (y=0; y<ymax; y++)
	  {
	    for (x=0; x<xmax; x++)
		{	
		  DT_Q(dtimage_get_pixel,(ctl,	/* fetch source pixel			    */
            input, x, y, &d));
		  DT_Q(dtimage_set_pixel,(ctl,	/* store output pixel			    */
            output, x, y, d*factor));
		}
	  }
	}
  }


  return DT_RC_GOOD;
}


/*..........................................................................
 *..........................................................................*/

static
void
mult_image_b08_b08(						/* "mult" image and image 	 	*/
  dt_ctl_t *ctl,
  dtimage_t *input1,
  dtimage_t *input2,
  const double scalar,
  const dtxy_t xmax,
  const dtxy_t ymax,
  dtimage_t *output)
{
  dtxy_t x;
  dtxy_t y;
  double d;
  for (y=0; y<ymax; y++)				/* for all rows						*/
  {
    const dt_ntype_b08_t * const p1 =	/* coursing pointers				*/
	  input1->row[y].b08;	
    const dt_ntype_b08_t * const p2 =
	  input2->row[y].b08;		
    dt_ntype_b08_t * const q =
	  output->row[y].b08;
    if (scalar == 1.0)
	for (x=0; x<xmax; x++)				/* for all columns					*/
      q[x] = p1[x] * p2[x];
    else
	for (x=0; x<xmax; x++)				/* for all columns					*/
	{
      d = 
        (double)p1[x] * 
        (double)p2[x] * 
        scalar;
      q[x] = (dt_ntype_b08_t)d;
	}
  }
}

/*..........................................................................
 *..........................................................................*/

static
void
mult_image_b32_b32(						/* "mult" image and image 	 	*/
  dt_ctl_t *ctl,
  dtimage_t *input1,
  dtimage_t *input2,
  const double scalar,
  const dtxy_t xmax,
  const dtxy_t ymax,
  dtimage_t *output)
{
  dtxy_t x;
  dtxy_t y;
  double d;
  for (y=0; y<ymax; y++)				/* for all rows						*/
  {
    const dt_ntype_b32_t * const p1 =	/* coursing pointers				*/
	  input1->row[y].b32;	
    const dt_ntype_b32_t * const p2 =
	  input2->row[y].b32;		
    dt_ntype_b32_t * const q =
	  output->row[y].b32;
    if (scalar == 1.0)
	for (x=0; x<xmax; x++)				/* for all columns					*/
      q[x] = p1[x] * p2[x];
    else
	for (x=0; x<xmax; x++)				/* for all columns					*/
	{
      d = 
        (double)p1[x] * 
        (double)p2[x] * 
        scalar;
      q[x] = (dt_ntype_b32_t)d;
	}
  }
}

/*..........................................................................
 *..........................................................................*/

static
void
mult_image_f2_f2(						/* "mult" image and image 	 	*/
  dt_ctl_t *ctl,
  dtimage_t *input1,
  dtimage_t *input2,
  const double scalar,
  const dtxy_t xmax,
  const dtxy_t ymax,
  dtimage_t *output)
{
  dtxy_t x;
  dtxy_t y;
  for (y=0; y<ymax; y++)				/* for all rows						*/
  {
    const dt_ntype_f2_t * const p1 =	/* coursing pointers				*/
	  input1->row[y].f2;	
    const dt_ntype_f2_t * const p2 =
	  input2->row[y].f2;		
    dt_ntype_f2_t * const q =
	  output->row[y].f2;
    if (scalar == 1.0)
	for (x=0; x<xmax; x++)				/* for all columns					*/
      q[x] = p1[x] * p2[x];
    else
	for (x=0; x<xmax; x++)				/* for all columns					*/
      q[x] = p1[x] * p2[x] * scalar;
  }
}

/*..........................................................................
| NAME
| dtimage_mult_image() - multiply pixels of two images
| 
| SUMMARY
| Implement output = input1 * input2 * factor for each pixel.
| Truncate high order bits if the output precision is too low
| (except single-bit output which is considered boolean).
| Permit either or both of the input images to be the same as the
| output image.
| Operate only on the intersection of the three image rectangles.
| 
| END
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtimage_mult_image(						/* "mult" two images				*/
  dt_ctl_t *ctl,						/* Environment control 				*/
  dtimage_t *input1,					/* first input image				*/
  dtimage_t *input2,					/* second input image 				*/
  double factor,						/* constant 						*/
  dtimage_t *output)					/* output image						*/
{
  DT_F("dtimage_mult_image");
  dtxy_t xmax, ymax;
  dt_rc_e rc;

  rc = dtimage_check_3(ctl,				/* check simple stuff				*/
    input1, "input1",
    input2, "input2",
    output, "output",
    NULL, F, &xmax, &ymax);
  if (rc != DT_RC_GOOD)
    return rc;

  if (input1->ntype == input2->ntype)
  switch(DT_NTYPE_PAIR(input1->ntype, output->ntype))
  {
	case DT_NTYPE_PAIR(DT_NTYPE_B08, DT_NTYPE_B08):  
      mult_image_b08_b08(ctl,
        input1, input2, factor,
        xmax, ymax, output);
    return DT_RC_GOOD;
	case DT_NTYPE_PAIR(DT_NTYPE_B32, DT_NTYPE_B32):  
      mult_image_b32_b32(ctl,
        input1, input2, factor,
        xmax, ymax, output);
    return DT_RC_GOOD;
	case DT_NTYPE_PAIR(DT_NTYPE_F2, DT_NTYPE_F2):  
      mult_image_f2_f2(ctl,
        input1, input2, factor,
        xmax, ymax, output);
    return DT_RC_GOOD;
  }

  {										/* we have to do it the hard way    */
    dtxy_t x, y;
    double d1, d2;
    for (y=0; y<ymax; y++)
	{
	  for (x=0; x<xmax; x++)
	  {	
		DT_Q(dtimage_get_pixel,(ctl,	/* fetch source pixel			    */
          input1, x, y, &d1));
		DT_Q(dtimage_get_pixel,(ctl,	/* fetch other source pixel		    */
          input2, x, y, &d2));
        DT_Q(dtimage_set_pixel,(ctl,	/* store output pixel			    */
          output, x, y, d1*d2*factor));
	  }
	}
  }


  return DT_RC_GOOD;
}

/*..........................................................................
| NAME
| dtimage_linear_scalar() - apply constant linear transform to each pixel
| 
| SUMMARY
| Implement output = input * factor + offset for each pixel.
| Keep the result within the representable range
| of the output image data type by setting the result
| to the minimum or maximum of that range if necessary.
| Permit the input image to be the same as the output image.
| Operate only on the intersection of the two image rectangles.
| 
| END
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtimage_linear_scalar(					/* pixel linear transform			*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtimage_t *input,						/* input image						*/
  double factor,						/* constant multiplication factor	*/
  double offset,						/* constant offset					*/
  dtimage_t *output)					/* output image						*/
{
  DT_F("dtimage_linear_scalar");
  dtxy_t xe, ye;
  dt_ntype_f2_t *promoted;
  double vmin, vmax;
  dtxy_t x, y;
  dt_rc_e rc;

  rc = dtimage_check_1f(ctl,			/* check simple stuff				*/
    input, "input",
    dt_ntype_list,
    DTIMAGE_FLAG_PAGED, F);
  if (rc != DT_RC_GOOD)
    return rc;

  rc = dtimage_check_1f(ctl,			/* check simple stuff				*/
    output, "output",
    dt_ntype_list, 
    DTIMAGE_FLAG_PAGED, F);
  if (rc != DT_RC_GOOD)
    return rc;
  
  xe = DT_MIN(input->xe, output->xe);
  ye = DT_MIN(input->ye, output->ye);

  DT_C(dtos_malloc2,(ctl,				/* space for one promoted row 		*/
    (void **)&promoted,
    xe * sizeof(*promoted),
    F, "promoted row"));

  vmin = dt_ntype_minval[output->ntype];
  vmax = dt_ntype_maxval[output->ntype];
  for (y=0; y<ye; y++)
  {
	void *p;
	DTIMAGE_GETROW(ctl, input, y, &p);
	DT_Q(dt1d_cast,(					/* promote raw input row to double	*/
      p, xe, input->ntype,
      promoted, xe, DT_NTYPE_F2));
	DTIMAGE_UNMAPROW(ctl, input, y);

	for (x=0; x<xe; x++)				/* do linear operation on row	 	*/
	{
	  const double v = 
        promoted[x] * factor + scalar;
      promoted[x] = DT_MAX(vmin,
        DT_MIN(vmax, v));
	}

	DTIMAGE_GETROW(ctl, output, y, &p);
	DT_Q(dt1d_cast,( 
      promoted, xe, DT_NTYPE_F2,
      p, xe, output->ntype));
	DTIMAGE_PUTROW(ctl, output, y);
  }

cleanup:
  DT_I(dtos_free2,(ctl,
    promoted,
    F, "promoted row"));
  return rc;
}

/*..........................................................................
 *..........................................................................*/

static
void
linear_image_b08(						/* "linear" image and image 	 	*/
  dt_ctl_t *ctl,
  dtimage_t *input1,
  double a,
  dtimage_t *input2,
  double b,
  double scalar,
  dtxy_t xmax,
  dtxy_t ymax,
  dtimage_t *output)
{
  dtxy_t x;
  dtxy_t y;
  for (y=0; y<ymax; y++)				/* for all rows						*/
  {
	dt_ntype_b08_t *p1;					/* coursing pointers				*/
	dt_ntype_b08_t *p2;
	dt_ntype_b08_t *q;
	p1 = input1->row[y].b08;			/* start of this row				*/
	p2 = input2->row[y].b08;		
	q = output->row[y].b08;
	for (x=0; x<xmax; x++)				/* for all columns					*/
	{
      double d =
        (double)p1[x] * a +
        (double)p2[x] * b +
        scalar;
      if (d < 0.0)						/* clip to base data type limits    */
        q[x] = 0;
      else
      if (d > 255.0)
        q[x] = 255;
      else
        q[x] = (dt_ntype_b08_t)d;
	}
  }
}

/*..........................................................................
| NAME
| dtimage_linear_image() - linear transform of two images
| 
| SUMMARY
| Implement output = input1 * factor1 + input2 * factor2 + offset
| for each pixel.
| Keep the result within the representable range
| of the output image data type by setting the result
| to the minimum or maximum of that range if necessary.
| Permit either or both of the input images to be the same as the output 
| image.
| Operate only on the intersection of the three image rectangles.
| 
| END
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtimage_linear_image(					/* linear transform two images		*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtimage_t *input1,					/* first input image				*/
  double factor1,						/* factor for first image 			*/
  dtimage_t *input2,					/* second input image 				*/
  double factor2,						/* factor for second image 			*/
  double offset,						/* additive offset 					*/
  dtimage_t *output)					/* output image						*/
{
  DT_F("dtimage_linear_image");
  dtxy_t xmax, ymax;
  dt_ntype_e ntypes[] =					/* datatypes we can handle			*/
    {DT_NTYPE_B08, DT_NTYPE_NULL};
  dt_rc_e rc;

  rc = dtimage_check_3(ctl,				/* check simple stuff				*/
    input1, "input1",
    input2, "input2",
    output, "output",
    ntypes, F, &xmax, &ymax);
  if (rc != DT_RC_GOOD)
    return rc;

  switch(input1->ntype)					/* branch based on ntype			*/
  {
	case DT_NTYPE_B08:  
      linear_image_b08(ctl,
        input1, factor1, 
        input2, factor2, offset,
        xmax, ymax, output);
    break;
  }

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
