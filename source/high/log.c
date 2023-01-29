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
DT_RCSID("source/high $RCSfile: log.c,v $ $Revision: 1.1 $");
#include <dtack/os.h>
#include <dtack/mem.h>

#define TMAX (8192)						/* cache size					    */

#if defined(DTCONFIG_WV4) || \
    defined(DTCONFIG_WV5)
#define MATHDECL __cdecl
#else
#define MATHDECL
#endif
typedef double MATHDECL func_f(double);	/* function pointer				    */

/*..........................................................................
 *..........................................................................*/

static
void
do_b08_b08(								/* do b08 image			 		  	*/
  dt_ctl_t *ctl,
  func_f *func,
  dtimage_t *input,
  const dtxy_t xmax,
  const dtxy_t ymax,
  const double a,
  const double scalar,
  const double vmin,
  const double vmax,
  dtimage_t *output)
{
  dtxy_t x;
  dtxy_t y;
  dt_ntype_b08_t *p;					/* coursing pointers				*/
  dt_ntype_b08_t *q;
  const dt_ntype_b08_t pmin =			/* precast						    */
    (dt_ntype_b08_t)vmin;			
  const dt_ntype_b08_t pmax = 
    (dt_ntype_b08_t)vmax;
  dt_ntype_b08_t t[256];
  double d;
  int i;

  t[0] = 0;
  for (i=1; i<256; i++)					/* make cache table			    	*/
  {
    d = func((double)i) * a +
      scalar;
    if (d < vmin)						/* clip to base data type limits    */
      t[i] = pmin;
    else
    if (d > vmax)
      t[i] = pmax;
    else
      t[i] = (dt_ntype_b08_t)d;
  }
  
  for (y=0; y<ymax; y++)				/* for all rows						*/
  {
	p = input->row[y].b08;				/* start of this row				*/
	q = output->row[y].b08;
	for (x=0; x<xmax; x++)				/* for all columns					*/
      q[x] = t[p[x]];
  }
}

/*..........................................................................
 *..........................................................................*/

static
void
do_b32_b08(								/* do b32 image to b08				*/
  dt_ctl_t *ctl,
  func_f *func,
  dtimage_t *input,
  const dtxy_t xmax,
  const dtxy_t ymax,
  const double a,
  const double scalar,
  const double vmin,
  const double vmax,
  dt_ntype_b08_t * const t,
  const long tmax,
  dtimage_t *output)
{
  dtxy_t x;
  dtxy_t y;
  dt_ntype_b32_t *p;					/* coursing pointers				*/
  dt_ntype_b08_t *q;
  const dt_ntype_b08_t pmin =			/* precast						    */ 
    (dt_ntype_b08_t)vmin;	
  const dt_ntype_b08_t pmax = 
    (dt_ntype_b08_t)vmax;
  double d;
  long i;
  int j;

  for (y=0; y<ymax; y++)				/* for all rows						*/
  {
	p = input->row[y].b32;				/* start of this row				*/
	q = output->row[y].b08;
	for (x=0; x<xmax; x++)				/* for all columns					*/
	{
      i = p[x];
	  j = (int)i;
      if (i < tmax && t[j] != 255)		/* can we look it up?			    */
        q[x] = t[j];
	  else	
	  {
        d = func((double)i) *
          a + scalar;
        if (d < vmin)					/* clip to base data type limits    */
          q[x] = pmin;
        else
        if (d > vmax)
          q[x] = pmax;
        else
          q[x] = (dt_ntype_b08_t)d;
        if (i < tmax)					/* we can reuse it?				    */
          t[j] = q[x];					/* use this value later			    */
	  }
	}
  }
}

/*..........................................................................
 *..........................................................................*/

static
void
do_b32_b32(								/* do b32 image to b32				*/
  dt_ctl_t *ctl,
  func_f *func,
  dtimage_t *input,
  const dtxy_t xmax,
  const dtxy_t ymax,
  const double a,
  const double scalar,
  const double vmin,
  const double vmax,
  dt_ntype_b32_t * const t,
  const long tmax,
  dtimage_t *output)
{
  dtxy_t x;
  dtxy_t y;
  dt_ntype_b32_t *p;					/* coursing pointers				*/
  dt_ntype_b32_t *q;
  const dt_ntype_b32_t pmin =			/* precast						    */ 
    (dt_ntype_b32_t)vmin;	
  const dt_ntype_b32_t pmax = 
    (dt_ntype_b32_t)vmax;
  double d;
  long i;
  int j;

  for (y=0; y<ymax; y++)				/* for all rows						*/
  {
	p = input->row[y].b32;				/* start of this row				*/
	q = output->row[y].b32;
	for (x=0; x<xmax; x++)				/* for all columns					*/
	{
      i = p[x];
      j = (int)i;
      if (i < tmax &&					/* can we look it up?			    */
          t[j] != 0xffffffffL)
        q[x] = t[j];
	  else	
	  {
        d = func((double)i) *
          a + scalar;
        if (d < vmin)					/* clip to base data type limits    */
          q[x] = pmin;
        else
        if (d > vmax)
          q[x] = pmax;
        else
          q[x] = (dt_ntype_b32_t)d;
        if (i < tmax)					/* we can reuse it?				    */
          t[j] = q[x];					/* use this value later			    */
	  }
	}
  }
}

/*..........................................................................
 *..........................................................................*/

static
void
do_f1_f1(								/* do f1 image			 		  	*/
  dt_ctl_t *ctl,
  func_f *func,
  dtimage_t *input,
  const dtxy_t xmax,
  const dtxy_t ymax,
  const double a,
  const double scalar,
  dtimage_t *output)
{
  dtxy_t x;
  dtxy_t y;
  
  for (y=0; y<ymax; y++)				/* for all rows						*/
  {
    const dt_ntype_f1_t * const p =		/* coursing pointers				*/
	  input->row[y].f1;					/* start of this row				*/
    dt_ntype_f1_t * const q =
	  output->row[y].f1;
	for (x=0; x<xmax; x++)				/* for all columns					*/
      q[x] = func(p[x]) * a + scalar;
  }
}

/*..........................................................................
 *..........................................................................*/

static
void
do_f2_f2(								/* do f2 image			 		  	*/
  dt_ctl_t *ctl,
  func_f *func,
  dtimage_t *input,
  const dtxy_t xmax,
  const dtxy_t ymax,
  const double a,
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
      q[x] = func(p[x]) * a + scalar;
  }
}

/*..........................................................................
 *..........................................................................*/

static
dt_rc_e
do_func(								/* do function to image				*/
  dt_ctl_t *ctl,
  char *F,
  func_f *func,
  dtimage_t *input,						/* input image						*/
  double a,								/* multiplication factor		    */
  double scalar,						/* scalar offset					*/
  dtimage_t *output)					/* output image						*/
{
  dtxy_t xmax, ymax;
  dt_ntype_e *ntypes = NULL;			/* we can handle any ntype		    */
  double vmin;
  double vmax;
  dt_rc_e rc;

  rc = dtimage_check_2(ctl,				/* check simple stuff				*/
    input, "input",
    output, "output",
    ntypes, F, &xmax, &ymax);
  if (rc != DT_RC_GOOD)
    return rc;

  vmin = dt_ntype_minval[output->ntype];
  vmax = dt_ntype_maxval[output->ntype];

  switch(DT_NTYPE_PAIR(input->ntype,	/* branch based on ntype			*/
           output->ntype))
  {
	case DT_NTYPE_PAIR(DT_NTYPE_B08, DT_NTYPE_B08):
      do_b08_b08(ctl, func,
        input, xmax, ymax,
        a, scalar,
        vmin, vmax,
        output);
    break;
    case DT_NTYPE_PAIR(DT_NTYPE_B32, DT_NTYPE_B08):
	{
      dt_ntype_b08_t *t08;
      DT_Q(dtos_malloc2,(ctl,			/* space for cache table		    */
        (void **)&t08,
        TMAX*sizeof(*t08),
        F, "cache"));
      DT_MEMSET(t08, 255,					/* set table to unlikely value		*/
        TMAX*sizeof(*t08));
      do_b32_b08(ctl, func,				/* do the log					    */
        input, xmax, ymax,
        a, scalar,
        vmin, vmax, t08, TMAX,
        output);
      DT_Q(dtos_free2,(ctl, t08,
        F, "cache"));
	}
    break;
    case DT_NTYPE_PAIR(DT_NTYPE_B32, DT_NTYPE_B32):
	{
      dt_ntype_b32_t *t32;
      DT_Q(dtos_malloc2,(ctl,			/* space for cache table		    */
        (void **)&t32,
        TMAX*sizeof(*t32),
        F, "cache"));
      DT_MEMSET(t32, 255,					/* set table to unlikely value		*/
        TMAX*sizeof(*t32));
      do_b32_b32(ctl, func,				/* do the log					    */
        input, xmax, ymax,
        a, scalar,
        vmin, vmax, t32, TMAX,
        output);
      DT_Q(dtos_free2,(ctl, t32,
        F, "cache"));
	}
    break;
    case DT_NTYPE_PAIR(DT_NTYPE_F2, DT_NTYPE_F2):
	{
      do_f2_f2(ctl, func,				/* do the log					    */
        input, xmax, ymax,
        a, scalar,
        output);
	}
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
          d = func(d) * a + scalar;		/* do function					    */
          if (d < vmin)
            d = vmin;
          else
          if (d > vmax)
            d = vmax;
          DT_Q(dtimage_set_pixel,(ctl,	/* store output pixel			    */
            output, x, y, d));
		}
	}
  }

  return DT_RC_GOOD;
}

/*..........................................................................
| NAME
| dtimage_log10() - compute log base 10 of each pixel
| 
| SUMMARY
| Implement output = LOG10(input) * factor + offset for each pixel.
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
dtimage_log10(							/* log 10 image						*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtimage_t *input,						/* input image						*/
  double factor,						/* multiplication factor		    */
  double offset,						/* scalar offset					*/
  dtimage_t *output)					/* output image						*/
{
  DT_F("dtimage_log10");
  return do_func(ctl, F, log10,
    input, factor, offset, output);
}

/*..........................................................................
| NAME
| dtimage_log() - compute natural logarithm of each pixel
| 
| SUMMARY
| Implement output = LOG(input) * factor + offset for each pixel.
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
dtimage_log(							/* log image						*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtimage_t *input,						/* input image						*/
  double factor,						/* multiplication factor		    */
  double offset,						/* scalar offset					*/
  dtimage_t *output)					/* output image						*/
{
  DT_F("dtimage_log");
  return do_func(ctl, F, log,
    input, factor, offset, output);
}

/*..........................................................................
| NAME
| dtimage_sin() - compute sine of each pixel
| 
| SUMMARY
| Implement output = SIN(input) * factor + offset for each pixel.
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
dtimage_sin(							/* sine of each pixel				*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtimage_t *input,						/* input image						*/
  double factor,						/* multiplication factor		    */
  double offset,						/* scalar offset					*/
  dtimage_t *output)					/* output image						*/
{
  DT_F("dtimage_sin");
  return do_func(ctl, F, sin,
    input, factor, offset, output);
}

/*..........................................................................
| NAME
| dtimage_cos() - compute cosine of each pixel
| 
| SUMMARY
| Implement output = COS(input) * factor + offset for each pixel.
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
dtimage_cos(							/* cosine of each pixel				*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtimage_t *input,						/* input image						*/
  double factor,						/* multiplication factor		    */
  double offset,						/* scalar offset					*/
  dtimage_t *output)					/* output image						*/
{
  DT_F("dtimage_cos");
  return do_func(ctl, F, cos,
    input, factor, offset, output);
}


/*..........................................................................
| NAME
| dtimage_sqrt() - compute square root of each pixel
| 
| SUMMARY
| Implement output = SQRT(input) * factor + offset for each pixel.
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
dtimage_sqrt(							/* square root of each pixel		*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtimage_t *input,						/* input image						*/
  double factor,						/* multiplication factor		    */
  double offset,						/* scalar offset					*/
  dtimage_t *output)					/* output image						*/
{
  DT_F("dtimage_sqrt");
  return do_func(ctl, F, sqrt,
    input, factor, offset, output);
}


/*..........................................................................
| NAME
| dtimage_abs() - compute absolute value of each pixel
| 
| SUMMARY
| Implement output = ABS(input) for each pixel.
| Permit the input image to be the same as the output image.
| Operate only on the intersection of the two image rectangles.
| 
| END
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtimage_abs(							/* absolute value of each pixel		*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtimage_t *input,						/* input image						*/
  dtimage_t *output)					/* output image						*/
{
  DT_F("dtimage_abs");
  void *ip = NULL;
  void *tp = NULL;
  void *op = NULL;
  dtxy_t xe = DT_MIN(input->xe, output->xe);
  dtxy_t ye = DT_MIN(input->ye, output->ye);
  dtxy_t x, y;
  unsigned int n;
  dt_rc_e rc;

  n = dt_ntype_sizeof[input->ntype] * xe;

  y = 0;

  DT_C(dtos_malloc2,(ctl,				/* space for promoted row 			*/
    (void **)&tp, n,
    F, "promoted row"));

  for (y=0; y<ye; y++)
  {
    DTIMAGE_GETROW2(DT_C, ctl,			/* access input row 				*/
      input, y, &ip);
	DT_MEMCPY(tp, ip, n);					/* make tmp copy 					*/
	DT_C_1D(dt1d_abs,(input->ntype,		/* do absolute value 				*/
	  tp, xe));
	ip = NULL;
    DTIMAGE_UNMAPROW2(DT_C, ctl,		/* release input row 				*/
      input, y);
	
										/* ................................ */
    DTIMAGE_MAPROW2(DT_C, ctl,			/* access output row 				*/
      output, y, &op);
 
	DT_C_1D(dt1d_cast,(					/* copy to output					*/
      tp, xe, input->ntype,
      op, xe, output->ntype));

	op = NULL;
    DTIMAGE_PUTROW2(DT_C, ctl,			/* release output row 				*/
      output, y);
  }
cleanup:
  if (op != NULL)
    DTIMAGE_PUTROW2(DT_I, ctl, 
      output, y);
  if (ip != NULL)
    DTIMAGE_UNMAPROW2(DT_I, ctl, 
      input, y);
  if (tp != NULL)
    DT_I(dtos_free2,(ctl, tp, 
      F, "tmp row"));
	
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
