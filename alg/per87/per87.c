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

static char rcsid[] = "alg $RCSfile: per87.c,v $ $Revision: 1.3 $";

#include <dtack/base.h>
#include <dtack/os.h>
#include <dtack/1d.h>
#include <dt_per87.h>

#define DAMP_UCHAR_ZERO (1024)
#define SERIES_UCHAR_ZERO (256)

/*..........................................................................
 *..........................................................................*/

static
dt_rc_e
dt_per87_uchar_alloc(					/* alloc tables and temp space		*/
  dtparam_t *param,						/* input parameters				    */
  long max,
  unsigned char **gtemp,				/* temp input for one row/column	*/
  unsigned char **ttemp,				/* temp output for one row/column	*/
  long **gradient,						/* gradient vector to alloc only    */
  double **damp,						/* damp table to alloc and init	    */
  double **series)						/* series table to alloc and init   */
{
  static char *F = "dt_per87_uchar_alloc";
  dt_ctl_t *ctl = &param->ctl;
  double G;
  double b = param->b;					/* determines significant values    */
  double s = param->dz;					/* determines small change		    */
  double n = param->n;					/* series terms to use			    */
  long i;
  dt_rc_e rc;

  *gtemp = NULL;
  *ttemp = NULL;
  *gradient = NULL;
  *damp = NULL;
  *series = NULL;

  DT_G(dtos_malloc,(ctl, 				/* temp input vector				*/
    (void **)gtemp,
    max * sizeof(**gtemp)));

  if (rc == DT_RC_GOOD)
  DT_G(dtos_malloc,(ctl, 				/* temp output vector				*/
    (void **)ttemp,
    max * sizeof(**ttemp)));

  if (rc == DT_RC_GOOD)
  DT_G(dtos_malloc,(ctl, 				/* temp vector for gradient		 	*/
    (void **)gradient,
    max * sizeof(**gradient)));

  if (rc == DT_RC_GOOD)
  DT_G(dtos_malloc,(ctl,				/* space for damp table			    */
    (void **)damp,
    2 * DAMP_UCHAR_ZERO *
    sizeof(**damp)));

  if (rc == DT_RC_GOOD)
  DT_G(dtos_malloc,(ctl,				/* space for series table		    */
    (void **)series,
    2 * SERIES_UCHAR_ZERO *
    sizeof(**series)));

  if (rc != DT_RC_GOOD)					/* one of the allocations failed?   */
  {
    if (*gtemp != NULL)
      DT_G(dtos_free,(ctl, gtemp));
    if (*ttemp != NULL)
      DT_G(dtos_free,(ctl, ttemp));
    if (*gradient != NULL)
      DT_G(dtos_free,(ctl, gradient));
    if (*damp != NULL)
      DT_G(dtos_free,(ctl, damp));
    if (*series != NULL)
      DT_G(dtos_free,(ctl, series));
    return DT_RC_BAD;
  }

  *damp = *damp +						/* offset table to negative indices	*/
    DAMP_UCHAR_ZERO;
  *series = *series +
    SERIES_UCHAR_ZERO;

  G = 255.0;							/* max possible g					*/
  for (i = -(DAMP_UCHAR_ZERO);			/* compute damp table			    */
       i < DAMP_UCHAR_ZERO;
       i++)
  {
    double differential = i;			/* differential indexes table	    */
	double temp;
    if (i != 0)							/* only care about non-zero case    */
	{
      temp = fabs(differential);
      temp = temp / s;
      temp = log(temp);
	  temp = pow(temp, 2.0*b);
	  (*damp)[i] = exp(-temp);
	}
	else								/* we won't ever use this one	    */
	  (*damp)[i] = 0.0;
  }

  G = 255.0;							/* max possible input gray level    */
  for (i = -(SERIES_UCHAR_ZERO);		/* compute series table			    */
       i < SERIES_UCHAR_ZERO;
       i++)
  {
    double diff = i;					/* diff indexes table			    */
    (*series)[i] = 1.0 / n * diff *
      (pow(diff/G, n) - 1.0) /
          (diff/G - 1.0);
  }

  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/

static
dt_rc_e
dt_per87_uchar_free(					/* free tables and temp space		*/
  dtparam_t *param,						/* input parameters				    */
  unsigned char *gtemp,					/* temp input vector			    */
  unsigned char *ttemp,					/* temp output vector			    */
  long *gradient,						/* gradient vector to alloc only    */
  double *damp,							/* damp table to alloc and init	    */
  double *series)						/* series table to alloc and init   */
{
  static char *F = "dt_per87_uchar_free";
  dt_ctl_t *ctl = &param->ctl;
  DT_Q(dtos_free,(ctl, gtemp));
  DT_Q(dtos_free,(ctl, ttemp));
  DT_Q(dtos_free,(ctl, gradient));
  DT_Q(dtos_free,(ctl, damp-DAMP_UCHAR_ZERO));
  DT_Q(dtos_free,(ctl, series-SERIES_UCHAR_ZERO));
  return DT_RC_GOOD;
}


/*..........................................................................
 *..........................................................................*/

static
dt_rc_e
dt_per87_uchar_initial(					/* make initial threshold guess		*/
  dtparam_t *param,						/* input parameters				    */
  dt_ntype_b08_t g0,					/* initial pixel on this line	    */
  dt_ntype_b08_t *g,					/* input previous gray level vector */
  long n,								/* length of array				    */
  long *differential,					/* input previous differential		*/
  dt_ntype_b08_t *threshold,			/* input previous threshold array	*/
  dt_ntype_b08_t *t0)					/* output initial threshold guess   */
{
  static char *F = "dt_per87_uchar_initial";
  dt_ctl_t *ctl = &param->ctl;
  double var_g, var_delta_g;
  double d_squared;
  long d_squared_l;
  double w1, w2, w3, ws;
  double bias = param->offset;			/* bias to use in guess				*/
  long t0_l;
  long i;

  DT_Q(dt1d_variance_uchar,(ctl,		/* variance of brightness array	    */
    g, n, &var_g));

  DT_Q(dt1d_variance_long,(ctl,			/* variance of gradient array	    */
    differential, n, &var_delta_g));

  d_squared_l = 0;
  for (i=0; i<n; i++)					/* sum squared differences		    */
  {
    long d = g[i] - threshold[i];
    d_squared_l += d * d;
  }
  d_squared = (double)d_squared_l /
                  (double)n;

  ws = var_g + var_delta_g + d_squared;	/* weights denominator			    */

  w1 = var_g / ws;						/* weights						    */
  w2 = var_delta_g / ws;
  w3 = d_squared / ws;

  t0_l =								/* equation (22)				    */
    w1 * g0 +
    w2 * (double)threshold[n-1] +
    w3 * bias +
    0.5;								/* round to long				    */

  if (t0_l < 0)
    *t0 = 0;
  else
  if (t0_l > 255)
    *t0 = 255;
  else
    *t0 = t0_l;

  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/

static int row48;

static
dt_rc_e
dt_per87_uchar_1d(						/* segment 1d gradient array	    */
  dtparam_t *param,						/* input parameters				    */
  dt_ntype_b08_t *g,					/* input gray level vector		    */
  long xmax,							/* length of array				    */
  long *differential,					/* input differential				*/
  dt_ntype_b08_t t0,					/* input initial threshold guess    */
  double *damp,							/* damp table					    */
  double *series,						/* series table 				    */
  dt_ntype_b08_t *threshold)			/* output threshold array			*/
{
  dt_ctl_t *ctl = &param->ctl;
  int k;

  threshold[0] = t0;					/* initial threshold approximation	*/
  for (k=1; k<xmax; k++)				/* for each pixel in vector			*/
  {
	if (differential[k] != 0)			/* non-zero differential pixel?		*/
	{
      long diff;
      long t_l;
      long d = differential[k];

      if (d < -(DAMP_UCHAR_ZERO))		/* clip differential to index	    */
        d = -(DAMP_UCHAR_ZERO);
      else
      if (d > DAMP_UCHAR_ZERO-1)
        d = DAMP_UCHAR_ZERO-1;
     
      diff = (long)g[k] -
             (long)threshold[k-1];
      t_l = (double)threshold[k-1] +
        damp[d] * series[diff] +
        0.5;							/* round result to long			    */

      if (t_l < 0)						/* clip threshold to output		    */
        threshold[k] = 0;
      else
      if (t_l > 255)
        threshold[k] = 255;
      else
        threshold[k] = t_l;
#ifdef NONO
if (row48) printf("g[%3d] = %3d, damp[%4d] = %9.2e, series[%4d] = %9.2e, thresh %3d\n",
k, g[k], d, damp[d], diff, series[diff], t_l);
#endif
	}
	else								/* no differential change			*/
	  threshold[k] = threshold[k-1];	/* so no correction factor			*/
  }

  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/

dt_rc_e
dt_per87_convolve_uchar(				/* convolve and fill in edges		*/
  dtparam_t *param,
  dt_ntype_b08_t *data,
  long n,
  long *kernel,
  long kernel_size,	
  long *gradient)
{
  static char *F = "dt_per87_uchar";
  dt_ctl_t *ctl = &param->ctl;
  long first, last;
  long i;

  DT_Q(dt1d_convolve_uchar,(ctl,		/* do the convolution			    */
    data, n, kernel, kernel_size,
    gradient, n));

  first = kernel_size / 2;				/* first pixel after edge-effect	*/
  last = n - 1 -						/* last pixel before edge-effect 	*/
    (kernel_size-1 - kernel_size/2);
  for (i=0; i<first; i++)				/* fill in leading edge-effect	    */
    gradient[i] = gradient[first]; 
  for (i=last+1; i<n; i++)			/* fill in trailing edge-effect		*/
    gradient[i] = gradient[last];

  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/

dt_rc_e
dt_per87_uchar(							/* Perez&Gonzalez 1987				*/
  dtparam_t *param,						/* input parameters				    */
  dtimage_t *input,						/* input image					    */
  dtimage_t *output,					/* output binary image			    */
  long *kernel,							/* 1-d gradient operator		    */
  long kernel_size)						/* gradient vector length		    */
{
  static char *F = "dt_per87_uchar";
  dt_ctl_t *ctl = &param->ctl;
  double *damp, *series;
  long xmax, ymax;
  long x, y;
  dt_ntype_e ntypes[] =					/* datatypes we can handle			*/
    {DT_NTYPE_B08, 0};
  dt_ntype_b08_t *g = NULL;
  dt_ntype_b08_t *threshold = NULL;
  dt_ntype_b08_t *gtemp, *ttemp;
  long *gradient;
  dt_ntype_b08_t t0;
  dt_rc_e rc;

  rc = dtimage_check_2(ctl,				/* check simple stuff				*/
    input, "input",
    output, "output",
    ntypes, F, &xmax, &ymax);
  if (rc != DT_RC_GOOD)
    return rc;

  DT_Q(dt_per87_uchar_alloc,(param,		/* get initial arrays			    */
    DT_MAX(xmax, ymax),
    &gtemp, &ttemp,
    &gradient, &damp, &series));

										/* -------------------------------- */
  										/* to right along row			    */
  if (strcmp(param->op, "col"))		   	/* only if not doing cols only		*/
  {
	t0 = param->t;
	for (y=0; y<ymax; y++)				/* for all rows					    */
	{
	  if (y != 0)						/* not first row?				    */
	    dt_per87_uchar_initial(			/* make initial threshold guess	    */
          param, input->row[y].b08[0],
          g, xmax,
          gradient, threshold, &t0);			   
    
	  g = input->row[y].b08;			/* input directly from image		*/
	  threshold = output->row[y].b08;	/* output directly to threshold	    */

	  dt_per87_convolve_uchar(param,	/* form image gradient			    */
        g, xmax, kernel, kernel_size,
        gradient);
row48 = (y==48);
	  dt_per87_uchar_1d(param,			/* operate on single row			*/
        g, xmax, gradient, t0,
        damp, series, threshold);
	}
  }
										/* -------------------------------- */
										/* down along column			    */
  if (strcmp(param->op, "row"))			/* only if not doing rows only	    */
  {
	t0 = param->t;
	g = gtemp;
	threshold = ttemp;
	for (x=0; x<xmax; x++)				/* for all columns				    */
	{
	  if (x != 0)						/* not first row?				    */
	    dt_per87_uchar_initial(			/* make initial threshold guess	    */
          param, input->row[0].b08[x],
          g, ymax,
          gradient, threshold, &t0);			   
    
	  for (y=0; y<ymax; y++)			/* fetch data from new column	    */
        g[y] = input->row[y].b08[x];

	  dt_per87_convolve_uchar(param,	/* form image gradient			    */
        g, ymax, kernel, kernel_size,
        gradient);

	  dt_per87_uchar_1d(param,			/* operate on single row			*/
        g, ymax, gradient, t0,
        damp, series, threshold);
    
      if (!strcmp(param->op, "col"))	/* doing cols only?				    */
	    for (y=0; y<ymax; y++)			/* copy data to output row			*/
          output->row[y].b08[x] =
            threshold[y];
      else
      if (!strcmp(param->op, "min"))	/* choose min of row and col result	*/
	    for (y=0; y<ymax; y++)		
          output->row[y].b08[x] = 
            DT_MIN(threshold[y],
              output->row[y].b08[x]);
      else
      if (!strcmp(param->op, "max"))	/* choose max of row and col result	*/
	    for (y=0; y<ymax; y++)		
          output->row[y].b08[x] = 
            DT_MAX(threshold[y],
              output->row[y].b08[x]);
      else
      if (!strcmp(param->op, "ave"))	/* choose ave of row and col result	*/
	    for (y=0; y<ymax; y++)		
          output->row[y].b08[x] =
            (output->row[y].b08[x] +
             threshold[y]) / 2;
	}
  }
  
  DT_Q(dt_per87_uchar_free,(param, 		/* free temp space				    */
    gtemp, ttemp,
    gradient, damp, series));

  return rc;
}

/*..........................................................................
 *..........................................................................*/

static
dt_rc_e
dt_per87_double_initial(				/* make initial threshold guess		*/
  dtparam_t *param,						/* input parameters				    */
  double g0,							/* initial pixel on this line	    */
  double *g,							/* input previous gray level vector */
  long n,								/* length of array				    */
  double *differential,					/* input previous differential		*/
  double *threshold,					/* input previous threshold array	*/
  double *t0)							/* output initial threshold guess   */
{
  static char *F = "dt_per87_double_initial";
  dt_ctl_t *ctl = &param->ctl;
  double var_g, var_delta_g;
  double d_squared;
  double w1, w2, w3, ws;
  double bias = param->offset;			/* bias to use in guess				*/
  long i;

  DT_Q(dt1d_variance_double,(ctl,		/* variance of brightness array	    */
    g, n, &var_g));

  DT_Q(dt1d_variance_double,(ctl,		/* variance of gradient array	    */
    differential, n, &var_delta_g));

  d_squared = 0.0;
  for (i=0; i<n; i++)					/* sum squared differences		    */
  {
    double d = g[i] - threshold[i];
    d_squared += d * d;
  }
  d_squared /= (double)n;

  ws = var_g + var_delta_g + d_squared;	/* weights denominator			    */

  w1 = var_g / ws;						/* weights						    */
  w2 = var_delta_g / ws;
  w3 = d_squared / ws;

  *t0 =									/* equation (22)				    */
    w1 * g0 +
    w2 * threshold[n-1] +
    w3 * bias;

  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/

static
dt_rc_e
dt_per87_double_1d(						/* segment 1d gradient array	    */
  dtparam_t *param,						/* input parameters				    */
  double *g,							/* input gray level vector		    */
  long xmax,							/* length of array				    */
  double *differential,					/* input differential				*/
  double t0,							/* input initial threshold guess    */
  double *threshold)					/* output threshold array			*/
{
  dt_ctl_t *ctl = &param->ctl;
  double G;
  int k;
  double b = param->b;					/* determines significant values    */
  double s = param->dz;					/* determines small change		    */
  double n = param->n;					/* series terms to use			    */

  G = 255.0;							/* max possible g					*/
  threshold[0] = t0;					/* initial threshold approximation	*/
  for (k=1; k<xmax; k++)				/* for each pixel in vector			*/
  {
	if (differential[k] != 0)			/* non-zero differential pixel?		*/
	{
      double temp, damp, diff, series;
  	  temp = fabs(differential[k]);
      temp = temp / s;
	  temp = log(temp);
	  temp = pow(temp, 2.0*b);
	  damp = exp(-temp);
	  diff = g[k] - threshold[k-1];
	  series = 1.0 / n * diff *
        (pow(diff/G, n) - 1.0) /
              (diff/G - 1.0);
	  threshold[k] = threshold[k-1] +
        damp * series;
	}
	else								/* no differential change			*/
	  threshold[k] = threshold[k-1];	/* so no correction factor			*/
  }

  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/

dt_rc_e
dt_per87_double(						/* Perez&Gonzalez 1987				*/
  dtparam_t *param,						/* input parameters				    */
  dtimage_t *input,						/* input image					    */
  dtimage_t *output,					/* output binary image			    */
  double *kernel,						/* 1-d gradient operator		    */
  long kernel_size)						/* gradient vector length		    */
{
  static char *F = "dt_per87_double";
  dt_ctl_t *ctl = &param->ctl;
  long xmax, ymax;
  long x, y;
  dt_ntype_e ntypes[] =					/* datatypes we can handle			*/
    {DT_NTYPE_B08, 0};
  double *g, *gradient, *threshold;
  dt_rc_e rc;

  rc = dtimage_check_2(ctl,				/* check simple stuff				*/
    input, "input",
    output, "output",
    ntypes, F, &xmax, &ymax);
  if (rc != DT_RC_GOOD)
    return rc;

  DT_Q(dtos_malloc,(ctl, (void**)&g,	/* three temp rows of doubles		*/
    3 * xmax * sizeof(double)));

  gradient = &g[xmax];					/* one third of space for gradient	*/
  threshold = &gradient[xmax];			/* one third of space for threshold	*/

  for (y=0; y<ymax; y++)				/* for all rows					    */
  {
    long first, last;
    dt_ntype_b08_t *p;
    dt_ntype_b08_t *q;
    double t0;
    
    p = input->row[y].b08;
    q = output->row[y].b08;

	if (y == 0)							/* first row?					    */
      t0 = param->t;					/* first guess arbitrary		    */
    else
    {
	  DT_G(dt_per87_double_initial,		/* make initial threshold guess	    */
        (param, (double)p[0], g, xmax,
        gradient, threshold, &t0));			   
      if (rc != DT_RC_GOOD) break;
	}

	for (x=0; x<xmax; x++)				/* convert input row to double	    */
      g[x] = p[x];

    DT_G(dt1d_convolve_double,(ctl,		/* form image gradient			    */
      g, xmax,
      kernel, kernel_size,
      gradient, xmax));
    if (rc != DT_RC_GOOD) break;

    first = kernel_size / 2;			/* first pixel after edge-effect	*/
    last = xmax - 1 -					/* last pixel before edge-effect 	*/
      (kernel_size-1 - kernel_size/2);
	for (x=0; x<first; x++)				/* fill in leading edge-effect	    */
      gradient[x] = gradient[first]; 
    for (x=last+1; x<xmax; x++)			/* fill in trailing edge-effect		*/
      gradient[x] = gradient[last];

    DT_G(dt_per87_double_1d,(param,		/* operate on single row			*/
      g, xmax, gradient, t0,
      threshold));
    if (rc != DT_RC_GOOD) break;

    for (x=0; x<xmax; x++)				/* copy threshold to output		    */
    {
      long t_l = threshold[x] + 0.5;	/* round off threshold			    */
      if (t_l < 0)						/* clip threshold values to B08	    */
        q[x] = 0;
      else
      if (t_l > 255)
        q[x] = 255;
      else
        q[x] = t_l;
	}
  }
  
  DT_Q(dtos_free,(ctl, g));				/* free temp space				    */

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
