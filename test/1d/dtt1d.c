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
  19991217 DE changed expected cast for "in stride and out stride, changed type"
  19990130 DE put ifdef before FFT test
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

#include <dtack/t.h>
#include <malloc.h>

DT_RCSID("test $RCSfile: dtt1d.c,v $ $Revision: 1.8 $");

#define USAGE "[-Dnnnn]"

#define L(V) (dt1d_i_t)(sizeof(V) / sizeof(*V))

#define EDGE (0.0)
#define EPS_EXACT (1e-10)

static const char *ops[] = {"all",
  "default", "time", "uniform", "gauss", "sorti", "sortr", "sort", 
  NULL};

/*..........................................................................
 * test fft
 *..........................................................................*/

static 
dt_rc_e
dtt1d_fft(
  dt_ctl_t *ctl)
{
  DT_F("dtt1d_fft");
  long cmp;

  DTYIELD(DT_Q, ctl, DTT_YIELD_AT_UNIT, F);

										/* ................................	*/
  {
    static char *id = "fft and inverse";
    static double in[] = {
  	  1.0, 0.0, 2.0, 0.0, 3.0, 0.0, 4.0, 0.0,
  	  5.0, 0.0, 6.0, 0.0, 7.0, 0.0, 8.0, 0.0,
  	  8.0, 0.0, 7.0, 0.0, 6.0, 0.0, 5.0, 0.0,
  	  4.0, 0.0, 3.0, 0.0, 2.0, 0.0, 1.0, 0.0};
    static double expect[L(in)];
	dt1d_i_t i;
	dt1d_i_t l;
	dt1d_rc_e rc;

	l = L(in)/2;
    for (i=0; i<l; i++)
	{
	  expect[i*2] = in[i*2] * l;
	  expect[i*2+1] = in[i*2+1] * l;
	}
	
#   if DTPRJ_FFTW != DT_1				/* no fft available? 				*/
    rc = dt1d_fft_complex(in, l, 1);	/* forward fft 						*/
    dt_assert(ctl, rc == DT1D_RC_BAD, F,
      "no FFT available", id);
    return DT_RC_GOOD;
#   endif

    DT_Q_1D(dt1d_fft_complex,(in, 		/* forward fft 						*/
      l, 1));

    DT_Q_1D(dt1d_fft_complex,(in, 		/* reverse fft 						*/
      l, 0));

  if (DT_DBG(ctl, DT_DBG_MASK_PIXEL))	/* caller wants values printed?	    */
    for (i=0; i<l; i++)
      dt_dbg(ctl, F, DT_DBG_MASK_PIXEL,
        "%2ld. %g %g",
        i, in[i*2], in[i*2+1]);
 
    DT_Q_1D(dt1d_cmp,(DT1D_NTYPE_F2,		/* compare results				    */
      in, l*2, expect, L(expect), 
      EPS_EXACT, &cmp));

    dt_assert(ctl, cmp == 0, F,
      "%s (cmp %ld)", id, cmp);
  }
										/* ................................	*/
  {
    static char *id = "fft of sine";
    static double in[64];
    static double expect1[L(in)];
    static double expect2[L(in)];
	dt1d_i_t i;
	dt1d_i_t l;

	l = L(in)/2;
    for (i=0; i<l; i++)
	{
      in[i*2] = cos((4.0*DT_PI)*(double)i/(double)l);
      in[i*2+1] = 0.0;
	  if (i == 2 || i == l-2)
		expect1[i*2] = l/2;				/* the spikes 						*/
	  else
		expect1[i*2] = 0.0;
      expect1[i*2+1] = 0.0;
      expect2[i*2] = in[i*2] * (double)l;
      expect2[i*2+1] = 0.0;
	}

    DT_Q_1D(dt1d_fft_complex,(in, 		/* forward fft 						*/
      l, 1));

    if (DT_DBG(ctl, DT_DBG_MASK_PIXEL))	/* caller wants values printed?	    */
    for (i=0; i<l; i++)
      dt_dbg(ctl, F, DT_DBG_MASK_PIXEL,
        "%2ld. expected %g %g and got %16g %16g",
        i, 
        expect1[i*2], expect1[i*2+1],
        in[i*2], in[i*2+1]);

    DT_Q_1D(dt1d_cmp,(DT1D_NTYPE_F2,	/* compare forward results			*/
      in, l*2, expect1, L(expect1), 
      EPS_EXACT, &cmp));

    dt_assert(ctl, cmp == 0, F,
      "%s (cmp %ld)", id, cmp);

    DT_Q_1D(dt1d_fft_complex,(in, 		/* reverse fft 						*/
      l, 0));

    DT_Q_1D(dt1d_cmp,(DT1D_NTYPE_F2,	/* compare reverse results			*/
      in, l*2, expect2, L(expect2), 
      EPS_EXACT, &cmp));

    dt_assert(ctl, cmp == 0, F,
      "%s (cmp %ld)", id, cmp);
  }
										/* ................................	*/

  return DT_RC_GOOD;
}

/*..........................................................................
 * test strided copy/casting
 *..........................................................................*/

static 
dt_rc_e
dtt1d_cast2(
  dt_ctl_t *ctl)
{
  DT_F("dtt1d_cast2");
  long cmp;

  DTYIELD(DT_Q, ctl, DTT_YIELD_AT_UNIT, F);

										/* ................................	*/
  {
    static char *id = "in stride no offset";
    static double in[] =
  	  {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    static double expect[] =
      {0, 2, 4, 6, 8};
    static double out[5];

	DT_MEMSET(out, 0, sizeof(out));
    DT_Q_1D(dt1d_cast2,(
      in, 0, 2, L(in), DTNTYPE_F2,
      out, 0, 1, L(in), DTNTYPE_F2));
 
    DT_Q_1D(dt1d_cmp,(DT1D_NTYPE_F2,		/* compare results				    */
      out, L(out), expect, L(expect), 
      0, &cmp));

    dt_assert(ctl, cmp == 0, F,
      "%s (cmp %ld)", id, cmp);
  }

										/* ................................	*/
  {
    static char *id = "in stride and in offset";
    static double in[] =
  	  {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    static double expect[] =
	  {1, 3, 5, 7, 9};
    static double out[5];
 
	DT_MEMSET(out, 0, sizeof(out));
    DT_Q_1D(dt1d_cast2,(
      in, 1, 2, L(in), DTNTYPE_F2,
      out, 0, 1, L(in), DTNTYPE_F2));
 
    DT_Q_1D(dt1d_cmp,(DT1D_NTYPE_F2,		/* compare results				    */
      out, L(out), expect, L(expect), 
      0, &cmp));

    dt_assert(ctl, cmp == 0, F,
      "%s (cmp %ld)", id, cmp);
  }

										/* ................................	*/
  {
    static char *id = "in stride and out offset";
    static double in[] =
  	  {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    static double expect[] =
	  {-1, 1, 3, 5, 7, 9};
    static double out[6];

	DT_MEMSET(out, 0, sizeof(out));
	out[0] = expect[0];
    DT_Q_1D(dt1d_cast2,(
      in, 1, 2, L(in), DTNTYPE_F2,
      out, 1, 1, L(in), DTNTYPE_F2));
 
    DT_Q_1D(dt1d_cmp,(DT1D_NTYPE_F2,		/* compare results				    */
      out, L(out), expect, L(expect), 
      0, &cmp));

    dt_assert(ctl, cmp == 0, F,
      "%s (cmp %ld)", id, cmp);
  }

										/* ................................	*/
  {
    static char *id = "in stride and out stride";
    static double in[] =
  	  {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    static double expect[] =
	  {-1, 1, -2, 5, -3, 9};
    static double out[6];

	DT_MEMSET(out, 0, sizeof(out));
	out[0] = expect[0];
	out[2] = expect[2];
	out[4] = expect[4];
    DT_Q_1D(dt1d_cast2,(
      in, 1, 4, L(in), DTNTYPE_F2,
      out, 1, 2, L(out), DTNTYPE_F2));
 
    DT_Q_1D(dt1d_cmp,(DT1D_NTYPE_F2,		/* compare results				    */
      out, L(out), expect, L(expect), 
      0, &cmp));

    dt_assert(ctl, cmp == 0, F,
      "%s (cmp %ld)", id, cmp);
  }

										/* ................................	*/
  {
    static char *id = "in stride and out stride, changed type";
    static double in[] =
  	  {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    static dt1d_ntype_s4_t expect[] =
	  {-1, 1, -2, 5, -3, 9};
    static dt1d_ntype_s4_t out[6];
	int i;

	DT_MEMSET(out, 0, sizeof(out));
	out[0] = expect[0];
	out[2] = expect[2];
	out[4] = expect[4];
    DT_Q_1D(dt1d_cast2,(
      in, 1, 4, L(in), DTNTYPE_F2,
      out, 1, 2, L(out), DTNTYPE_S4));

	for (i=0; i<L(out); i++)
    DT_Q_1D(dt1d_cmp,(DT1D_NTYPE_S4,	/* compare results				    */
      out, L(out), expect, L(expect), 
      0, &cmp));

    dt_assert(ctl, cmp == 0, F,
      "%s (cmp %ld)", id, cmp);
  }
										/* ................................	*/

  return DT_RC_GOOD;
}

/*..........................................................................
 * make check double result
 *..........................................................................*/

static
dt_rc_e
dtt1d_double_check(
  dt_ctl_t *ctl,
  double got,
  double want,
  double eps,				   
  char *F,
  char *id)
{
  double normalize;
  double delta;
  normalize = fabs(want);				/* normalize result magnitude	    */
  if (normalize == 0.0)
    normalize = 1.0;
  delta = fabs(want-got) / normalize;	/* significant digit difference	    */
  dt_assert(ctl, delta < eps,
    F, "%s got %0.11e want %0.11e (delta %0.11e)",
    id, got, want, delta);
  return DT_RC_GOOD;
}

/*..........................................................................
 * get space for double vector
 *..........................................................................*/
static 
dt_rc_e
dtt1d_get_double_vector(
  dt_ctl_t *ctl,
  long nmax,
  double **x,
  dt1d_i_t *n,
  char *F)
{
  *n = (dt1d_i_t)(						/* max we could give			    */
    DTCONFIG_MALLOC_MAX / sizeof(**x));
  
  if (*n > nmax)						/* caller wants less?		   		*/
    *n = (dt1d_i_t)nmax;

  DT_Q(dtos_malloc2,(ctl,				/* get space for the vector			*/
    (void **)x, *n * sizeof(**x),
    F, "double vector"));

  return DT_RC_GOOD;
}

/*..........................................................................
 * check result from random call
 *..........................................................................*/

static
dt_rc_e
dtt1d_random_double_check(
  dt_ctl_t *ctl,
  double *b,
  double *x,
  dt1d_i_t n,
  char *F,
  char *id)
{
  dt1d_i_t i;
  long cmp;

  if (DT_DBG(ctl, DT_DBG_MASK_PIXEL))	/* caller wants values printed?	    */
  {
    dt1d_i_t m = DT_MIN(n, 64);			/* limit printing					*/
    for (i=0; i<m; i++)
      dt_dbg(ctl, F, DT_DBG_MASK_PIXEL,
        "%2ld. %1.12e",
        i, x[i]);
  }

  DT_Q_1D(dt1d_cmp,(DT1D_NTYPE_F2,		/* compare results				    */
    b, n, x, n, 1e-10, &cmp));
  if (cmp)								/* results wrong?				    */
  {
    int wrong = abs((int)cmp) - 1;
    dt_assert(ctl, 0, F,
      "%s: index %ld wrong, expected %1.12e but got %1.12e (delta %e)",
      id, wrong, b[wrong], x[wrong], x[wrong]-b[wrong]);
  }
  else									/* results right?				    */
    dt_assert(ctl, 1, F, "%s", id);

  return DT_RC_GOOD;
}

/*..........................................................................
 * test random routine(s)
 *..........................................................................*/

#define N (32)

static 
dt_rc_e
dtt1d_random1(
  dt_ctl_t *ctl)
{
  DT_F("dtt1d_random1");
  dt1d_ran_t ran;
  double *x;
  int i;

  DTYIELD(DT_Q, ctl, DTT_YIELD_AT_UNIT, F);

  DT_Q(dtos_malloc2,(ctl, (void **)&x,	/* space to generate into		    */
    N * sizeof(*x), F, "buffer"));
										/* ................................	*/
  for (i=0; i<2; i++)
  {
	char *id = "random uniform double sequence 1";
    static double b[N] = {
	  1.708280361063e-01,
	  7.499019804850e-01,
	  9.637165562357e-02,
	  8.704652270271e-01,
	  5.773035067951e-01,
	  7.857992588397e-01,
	  6.921941534586e-01,
	  3.687662699204e-01,
	  8.739040768618e-01,
	  7.450950984501e-01,
	  4.460459090931e-01,
	  3.537282030934e-01,
	  7.325196320025e-01,
	  2.602220010829e-01,
	  3.942937749239e-01,
	  7.767899512256e-01,
	  8.450351375803e-01,
	  5.757882004828e-01,
	  7.155385951687e-01,
	  8.300424607388e-02,
	  4.558251286598e-01,
	  1.099468141806e-01,
	  5.452280238166e-01,
	  3.906865706487e-01,
	  5.685854232144e-01,
	  9.590664494884e-01,
	  8.677190964591e-01,
	  1.631895102524e-01,
	  2.755089268683e-01,
	  2.603610948721e-01,
	  9.240947418692e-01,
	  4.359226371027e-01
	};

    DT_Q_1D(dt1d_ran_uniform_init,(		/* init random numbers			    */
      &ran, 0));
    DT_Q_1D(dt1d_random_uniform_double,(	/* compute random numbers		    */
      &ran, x, N));
    DT_Q(dtt1d_random_double_check,(ctl,
      b, x, N, F, id));
  }
										/* ................................	*/
  for (i=0; i<2; i++)
  {
	char *id = "random gauss double sequence 1";
    static double b[N] = {
	  5.277897166355e-01,
	  -6.952068855584e-01,
	  1.397556267585e+00,
	  3.780135780144e-01,
	  -9.862627149679e-01,
	  1.444399450325e+00,
	  3.667456362208e-01,
	  5.594876822156e-01,
	  -2.025626578206e+00,
	  -7.471764404025e-01,
	  -9.119781337851e-01,
	  8.843714645203e-01,
	  1.351550208584e+00,
	  -5.161577214694e-01,
	  2.529020761506e-01,
	  1.151367918530e+00,
	  -4.464391781120e-01,
	  2.307574414668e-01,
	  -9.775244971706e-01,
	  -1.107080276853e-01,
	  -2.218739778144e+00,
	  9.179953105867e-01,
	  5.394497496351e-01,
	  8.059484508808e-02,
	  -7.006245779912e-02,
	  7.649198722077e-02,
	  -9.464695468452e-01,
	  -8.866421925284e-01,
	  -1.170128614573e-01,
	  7.744472779668e-01,
	  -3.814789874259e-01,
	  2.965313746208e-01
	};

    DT_Q_1D(dt1d_ran_gauss_init,(			/* init random numbers			    */
      &ran, 0));
    DT_Q_1D(dt1d_random_gauss_double,(		/* compute random numbers		    */
      &ran, x, N));
    DT_Q(dtt1d_random_double_check,(ctl,
      b, x, N, F, id));
  }
										/* ................................	*/
  DT_Q(dtos_free2,(ctl, x,
    F, "buffer"));


  return DT_RC_GOOD;
}
#undef N

/*..........................................................................
 * test uniform random population
 *..........................................................................*/


static 
dt_rc_e
dtt1d_random2(
  dt_ctl_t *ctl)
{
  DT_F("dtt1d_random2");
  dt1d_ran_t ran;
  double *x;
  double sum;
  double variance;

  DTYIELD(DT_Q, ctl, DTT_YIELD_AT_UNIT, F);

#define N 7500							/* keep it under 64K			    */
  DT_Q(dtos_malloc2,(ctl, (void **)&x,	/* space to generate into		    */
    N * sizeof(*x), F, "buffer"));

										/* ................................ */
  DT_Q_1D(dt1d_ran_uniform_init,(		/* init random numbers			    */
    &ran, 0));
  DT_Q_1D(dt1d_random_uniform_double,(	/* compute random numbers		    */
    &ran, x, N));

  DT_Q_1D(dt1d_sum,(DTNTYPE_F2,			/* compute the sum					*/
    x, N, &sum));	

  DT_Q(dtt1d_double_check,(ctl,
    sum/N, 5.0e-01, 2.0e-2, 
    F, "uniform mean"));
										/* ................................ */
  DT_Q_1D(dt1d_ran_gauss_init,(			/* init random numbers			    */
    &ran, 0));
  DT_Q_1D(dt1d_random_gauss_double,(	/* compute random numbers		    */
    &ran, x, N));

  DT_Q_1D(dt1d_sum,(DTNTYPE_F2,			/* compute the sum					*/
    x, N, &sum));	
  DT_Q(dtt1d_double_check,(ctl,
    sum/N, 0.0, 2.0e-2, 
    F, "gauss mean"));

  DT_Q_1D(dt1d_variance,(DTNTYPE_F2,	/* compute the variance				*/
    x, N, &variance));	
  DT_Q(dtt1d_double_check,(ctl,
    variance, 1.0e+00, 2.0e-2, 
    F, "gauss variance"));
										/* ................................ */

  DT_Q(dtos_free2,(ctl, x,
    F, "buffer"));
#undef N


  return DT_RC_GOOD;
}
 
/*..........................................................................
 * test uniform random timing
 *..........................................................................*/

static 
dt_rc_e
dtt1d_random_uniform_double_timing(
  dt_ctl_t *ctl,
  long nmax)
{
  DT_F("dtt1d_random_uniform_double_timing");
  dt1d_ran_t ran;
  double *x;
  dt1d_i_t n;
  dtos_time_t mark1, mark2;
  double sum;
  char id[32];

  DTYIELD(DT_Q, ctl, DTT_YIELD_AT_UNIT, F);

  DT_Q(dtt1d_get_double_vector,(ctl,	/* get space for double vector	    */
    nmax, &x, &n, F));

  DT_Q(dtos_time,(ctl, &mark1));

  DT_Q_1D(dt1d_ran_uniform_init,(			/* init random numbers			    */
    &ran, 0));
  DT_Q_1D(dt1d_random_uniform_double,(		/* compute random numbers		    */
    &ran, x, n));

  DT_Q(dtos_time,(ctl, &mark2));

  sprintf(id,
    "random uniform (%6u)", 
    (unsigned int)n);
  DT_Q(dtt_time_report,(ctl,			/* report time					    */
    &mark1, &mark2, id));

  DT_Q_1D(dt1d_sum,(DT1D_NTYPE_F2,
    x, nmax, &sum));
  DT_Q(dtt1d_double_check,(ctl,
    sum/(double)nmax, 
    5.0e-01, 1.0e-3, F, id));

  dtos_free2(ctl, x, F,					/* free vector					    */
    "double vector");

  return DT_RC_GOOD;
}
#undef N
 
/*..........................................................................
 * test gauss random timing
 *..........................................................................*/

static 
dt_rc_e
dtt1d_random_gauss_double_timing(
  dt_ctl_t *ctl,
  long nmax)
{
  DT_F("dtt1d_random_gauss_double_timing");
  dt1d_ran_t ran;
  double *x;
  dt1d_i_t n;
  dtos_time_t mark1, mark2;
  double sum;
  char id[32];

  DTYIELD(DT_Q, ctl, DTT_YIELD_AT_UNIT, F);

  DT_Q(dtt1d_get_double_vector,(ctl,	/* get space for double vector	    */
    nmax, &x, &n, F));

  DT_Q(dtos_time,(ctl, &mark1));

  DT_Q_1D(dt1d_ran_gauss_init,(			/* init random numbers			    */
    &ran, 0));
  DT_Q_1D(dt1d_random_gauss_double,(		/* compute random numbers		    */
    &ran, x, n));

  DT_Q(dtos_time,(ctl, &mark2));

  sprintf(id,
    "random gauss (%6u)", 
    (unsigned int)n);
  DT_Q(dtt_time_report,(ctl,			/* report time					    */
    &mark1, &mark2, id));

  DT_Q_1D(dt1d_sum,(DT1D_NTYPE_F2,
    x, nmax, &sum));
  DT_Q(dtt1d_double_check,(ctl,
    sum/(double)nmax, 
    0.0, 2.0e-2, F, id));

  dtos_free2(ctl, x, F,					/* free vector					    */
    "double vector");

  return DT_RC_GOOD;
}
#undef N

/*..........................................................................
 * make variance call and check result
 *..........................................................................*/

static
dt_rc_e
dtt1d_variance_double_check(
  dt_ctl_t *ctl,
  char *F,
  char *id,
  double *b,
  dt1d_i_t n,
  double want)
{
  double got;
  DT_Q_1D(dt1d_variance,(DTNTYPE_F2,	/* compute variance				    */
    b, n, &got));
  DT_Q(dtt1d_double_check,(ctl,			/* check result					    */
    got, want, EPS_EXACT, F, id));
  return DT_RC_GOOD;
}

/*..........................................................................
 * test variance routine(s)
 *..........................................................................*/

static 
dt_rc_e
dtt1d_variance(
  dt_ctl_t *ctl)
{
  DT_F("dtt1d_variance");

										/* ................................	*/
  {
	char *id = "variance of zero";
    static double b[8] =
      {0, 0, 0, 0,
       0, 0, 0, 0};
    DT_Q(dtt1d_variance_double_check,(ctl, F, id, b, L(b),
      0.0));
  }
										/* ................................	*/
  {
	char *id = "variance of small positive numbers";
    static double b[8] =
      {11, 27, 127, 27,
       1, 107, 37, 7};
    DT_Q(dtt1d_variance_double_check,(ctl, F, id, b, L(b),
      2.25485714286e+03));
    DT_Q(dtt1d_variance_double_check,(ctl, F, id, b, L(b)-1,
      2.38380952381e+03));
  }
										/* ................................	*/

  {
	char *id = "variance of small negative numbers";
    static double b[8] =
      {-18, -1, -128, -12,
       -11, -28, -38, -3};
    DT_Q(dtt1d_variance_double_check,(ctl, F, id, b, L(b),
      1.72441071429e+03));
  }
										/* ................................	*/
  {
	char *id = "variance of mixed numbers";
    static double b[8] =
      {-18333, 1, 12800, -12,
       -11, -28, 38, -3};
    DT_Q(dtt1d_variance_double_check,(ctl, F, id, b, L(b),
      7.08705505714e+07));
  }
										/* ................................	*/
  {					
	char *id = "variance of large positive numbers";
    static double b[8] =
      {183432.0, 10000.0, 12876.0, 183431.0,
       1001.0, 28.0, 3842.0, 332.0};
    DT_Q(dtt1d_variance_double_check,(ctl, F, id, b, L(b),
      6.86835683050e+09));
  }
										/* ................................	*/
  {
	char *id = "variance of mixed numbers";
    static double b[8] =
      {-18.33, 1.1, 128.7, 128.8,
       -28.99, -28.9, 38.66, -3.22};
    DT_Q(dtt1d_variance_double_check,(ctl, F, id, b, L(b),
      4.39146127857e+03));
  }
										/* ................................	*/
  return DT_RC_GOOD;
}

/*..........................................................................
 * test double-precision sort routine
 *..........................................................................*/

static 
dt_rc_e
dtt1d_sort_double(
  dt_ctl_t *ctl)
{
  DT_F("dtt1d_sort_double");
  long cmp;

  DTYIELD(DT_Q, ctl, DTT_YIELD_AT_UNIT, F);

										/* ................................	*/
  {
    static char *id = "short sort";
    static double in[] =
      {9, 3, 1, 2};
    static double out[] =
      {1, 2, 3, 9};
 
    DT_Q_1D(dt1d_sort,(DTNTYPE_F2,		/* do the sort					    */
      in, L(in)));

    if (DT_DBG(ctl, DT_DBG_MASK_PIXEL))	/* caller wants values printed?	    */
    {
      dt1d_i_t m = DT_MIN(L(in), 64);	/* limit printing					*/
      int i;
      for (i=0; i<m; i++)
	    dt_dbg(ctl, F, DT_DBG_MASK_PIXEL,
		  "%s %2ld. %1.12e",
          id, i, in[i]);
    }
 
    DT_Q_1D(dt1d_cmp,(DT1D_NTYPE_F2,		/* compare results				    */
      in, L(in), out, L(out), 
      0, &cmp));

    dt_assert(ctl, cmp == 0, F,
      "%s (cmp %ld)", id, cmp);
  }
  
  {
    static char *id = "medium sort";
    static double in[] =
      {4, -1, 3, -1, 2, -1, 1, 0, 1, 3, 2, 0, -2};
    static double out[] =
      {-2, -1, -1, -1, 0, 0, 1, 1, 2, 2, 3, 3, 4};
 
    DT_Q_1D(dt1d_sort,(DTNTYPE_F2,		/* do the sort					    */
      in, L(in)));

    if (DT_DBG(ctl, DT_DBG_MASK_PIXEL))	/* caller wants values printed?	    */
    {
      dt1d_i_t m = DT_MIN(L(in), 64);	/* limit printing					*/
      int i;
      for (i=0; i<m; i++)
	    dt_dbg(ctl, F, DT_DBG_MASK_PIXEL,
          "%s %2ld. %1.12e",
          id, i, in[i]);
    }
 
    DT_Q_1D(dt1d_cmp,(DT1D_NTYPE_F2,		/* compare results				    */
      in, L(in), out, L(out), 
      0, &cmp));

    dt_assert(ctl, cmp == 0, F,
      "%s (cmp %ld)", id, cmp);
  }
  
  {
    static char *id = "sorted sort";
    static double in[16] =
      {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
    static double out[16] =
      {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
 
    DT_Q_1D(dt1d_sort,(DTNTYPE_F2,		/* do the sort					    */
      in, L(in)));
 
    DT_Q_1D(dt1d_cmp,(DT1D_NTYPE_F2,	/* compare results				    */
      in, L(in), out, L(out), 
      0, &cmp));

    dt_assert(ctl, cmp == 0, F,
      "%s (cmp %ld)", id, cmp);
  }
  
  {
    static char *id = "unsorted sort";
    static double in[16] =
      {15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0};
    static double out[16] =
      {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
 
    DT_Q_1D(dt1d_sort,(DTNTYPE_F2,		/* do the sort					    */
      in, L(in)));
 
    DT_Q_1D(dt1d_cmp,(DT1D_NTYPE_F2,	/* compare results				    */
      in, L(in), out, L(out),
      0, &cmp));

    dt_assert(ctl, cmp == 0, F,
      "%s (cmp %ld)", id, cmp);
  }
  
  {
    static char *id = "constant sort";
    static double in[25] =
      {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1};
    static double out[25] =
      {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1};
 
    DT_Q_1D(dt1d_sort,(DTNTYPE_F2,		/* do the sort					    */
      in, L(in)));
 
    DT_Q_1D(dt1d_cmp,(DT1D_NTYPE_F2,		/* compare results				    */
      in, L(in), out, L(out),
      0, &cmp));

    dt_assert(ctl, cmp == 0, F,
      "%s (cmp %ld)", id, cmp);
  }
										/* ................................	*/

  return DT_RC_GOOD;
}

/*..........................................................................
 * comparison function for structure sorting
 *..........................................................................*/
static dt1d_compare_f compare;
static
int
compare(
  void *s1, 
  void *s2, 
  unsigned int size)
{
  double d1 = *(double *)s1;
  double d2 = *(double *)s2;
  if (d1 < d2)
	return -1;
  else
  if (d1 > d2)
	return 1;
  else
	return 0;
}

/*..........................................................................
 * test structure sort routine
 *..........................................................................*/

static 
dt_rc_e
dtt1d_sort_struct(
  dt_ctl_t *ctl)
{
  DT_F("dtt1d_sort_struct");
  long cmp;

  DTYIELD(DT_Q, ctl, DTT_YIELD_AT_UNIT, F);

										/* ................................	*/
  {
    static char *id = "short sort";
    static double in[] =
      {9, 3, 1, 2};
    static double out[] =
      {1, 2, 3, 9};
 
    DT_Q_1D(dt1d_sort_struct,(			/* do the sort					    */
      in, sizeof(*in), L(in), 
      compare));

    if (DT_DBG(ctl, DT_DBG_MASK_PIXEL))	/* caller wants values printed?	    */
    {
      dt1d_i_t m = DT_MIN(L(in), 64);	/* limit printing					*/
      int i;
      for (i=0; i<m; i++)
	    dt_dbg(ctl, F, DT_DBG_MASK_PIXEL,
		  "%s %2ld. %1.12e",
          id, i, in[i]);
    }
 
    DT_Q_1D(dt1d_cmp,(DT1D_NTYPE_F2,		/* compare results				    */
      in, L(in), out, L(out), 
      0, &cmp));

    dt_assert(ctl, cmp == 0, F,
      "%s (cmp %ld)", id, cmp);
  }
  
  {
    static char *id = "medium sort";
    static double in[] =
      {4, -1, 3, -1, 2, -1, 1, 0, 1, 3, 2, 0, -2};
    static double out[] =
      {-2, -1, -1, -1, 0, 0, 1, 1, 2, 2, 3, 3, 4};
 
    DT_Q_1D(dt1d_sort_struct,(			/* do the sort					    */
      in, sizeof(*in), L(in), 
      compare));

    if (DT_DBG(ctl, DT_DBG_MASK_PIXEL))	/* caller wants values printed?	    */
    {
      dt1d_i_t m = DT_MIN(L(in), 64);	/* limit printing					*/
      int i;
      for (i=0; i<m; i++)
	    dt_dbg(ctl, F, DT_DBG_MASK_PIXEL,
          "%s %2ld. %1.12e",
          id, i, in[i]);
    }
 
    DT_Q_1D(dt1d_cmp,(DT1D_NTYPE_F2,		/* compare results				    */
      in, L(in), out, L(out), 
      0, &cmp));

    dt_assert(ctl, cmp == 0, F,
      "%s (cmp %ld)", id, cmp);
  }
  
  {
    static char *id = "sorted sort";
    static double in[16] =
      {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
    static double out[16] =
      {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
 
    DT_Q_1D(dt1d_sort_struct,(			/* do the sort					    */
      in, sizeof(*in), L(in), 
      compare));
 
    DT_Q_1D(dt1d_cmp,(DT1D_NTYPE_F2,	/* compare results				    */
      in, L(in), out, L(out), 
      0, &cmp));

    dt_assert(ctl, cmp == 0, F,
      "%s (cmp %ld)", id, cmp);
  }
  
  {
    static char *id = "unsorted sort";
    static double in[16] =
      {15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0};
    static double out[16] =
      {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
 
    DT_Q_1D(dt1d_sort_struct,(			/* do the sort					    */
      in, sizeof(*in), L(in), 
      compare));
 
    DT_Q_1D(dt1d_cmp,(DT1D_NTYPE_F2,	/* compare results				    */
      in, L(in), out, L(out),
      0, &cmp));

    dt_assert(ctl, cmp == 0, F,
      "%s (cmp %ld)", id, cmp);
  }
  
  {
    static char *id = "constant sort";
    static double in[25] =
      {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1};
    static double out[25] =
      {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1};
 
    DT_Q_1D(dt1d_sort_struct,(			/* do the sort					    */
      in, sizeof(*in), L(in), 
      compare));
 
    DT_Q_1D(dt1d_cmp,(DT1D_NTYPE_F2,		/* compare results				    */
      in, L(in), out, L(out),
      0, &cmp));

    dt_assert(ctl, cmp == 0, F,
      "%s (cmp %ld)", id, cmp);
  }
										/* ................................	*/

  return DT_RC_GOOD;
}

/*..........................................................................
 * check sort output
 *..........................................................................*/
static 
dt_rc_e
dtt1d_sort_double_check(
  dt_ctl_t *ctl,
  double *x,
  dt1d_i_t n,
  char *F)
{
  dt1d_i_t i;

  for (i=1; i<n; i++)
  {
    if (x[i] < x[i-1])
	{
      dt_assert(ctl, 0, F,
        "sort[%u]=%e greater than sort[%u]=%e", 
        (unsigned int)(i-1), x[i-1],
        (unsigned int)i, x[i]);
      break;
	}
  }

  if (i == n)
    dt_assert(ctl, 1, F,
      "vector size %u is properly sorted", 
      (unsigned int)n);

  return DT_RC_GOOD;
}

/*..........................................................................
 * time sort routine on inverted data
 *..........................................................................*/

static 
dt_rc_e
dtt1d_sort_double_invert_timing(
  dt_ctl_t *ctl,
  long nmax)
{
  DT_F("dtt1d_sort_double_invert_timing");
  double *x;
  dt1d_i_t n, i;
  dtos_time_t mark1, mark2;
  char id[32];

  DTYIELD(DT_Q, ctl, DTT_YIELD_AT_UNIT, F);

  DT_Q(dtt1d_get_double_vector,(ctl,	/* get space for double vector	    */
    nmax, &x, &n, F));

  for (i=0; i<n; i++)					/* init inverse					    */
    x[i] = n - i - 1;

  DT_Q(dtos_time,(ctl, &mark1));

  DT_Q_1D(dt1d_sort,(DTNTYPE_F2,		/* do the sort					    */
    x, n));

  DT_Q(dtos_time,(ctl, &mark2));

  sprintf(id,
    "inverted sort (%6u)", 
    (unsigned int)n);
  DT_Q(dtt_time_report,(ctl,			/* report time					    */
    &mark1, &mark2, id));

  DT_Q(dtt1d_sort_double_check,(		/* check the sort results		    */
    ctl, x, n, F));

  dtos_free2(ctl, x, F,					/* free vector					    */
    "double vector");

  return DT_RC_GOOD;
}

/*..........................................................................
 * time sort routine on random data
 *..........................................................................*/
static 
dt_rc_e
dtt1d_sort_double_random_timing(
  dt_ctl_t *ctl,
  long nmax)
{
  DT_F("dtt1d_sort_double_random_timing");
  dt1d_ran_t ran;
  double *x;
  dt1d_i_t n;
  dtos_time_t mark1, mark2;
  char id[32];

  DTYIELD(DT_Q, ctl, DTT_YIELD_AT_UNIT, F);

  DT_Q(dtt1d_get_double_vector,(ctl,	/* get space for double vector	    */
    nmax, &x, &n, F));

  DT_Q_1D(dt1d_ran_uniform_init,(		/* init random numbers			    */
    &ran, 0));
  DT_Q_1D(dt1d_random_uniform_double,(	/* compute random numbers		    */
    &ran, x, n));

  DT_Q(dtos_time,(ctl, &mark1));

  DT_Q_1D(dt1d_sort,(DTNTYPE_F2,		/* do the sort					    */
    x, n));

  DT_Q(dtos_time,(ctl, &mark2));

  sprintf(id,
    "random sort (%6u)", 
    (unsigned int)n);
  DT_Q(dtt_time_report,(ctl,			/* report time					    */
    &mark1, &mark2, id));

  DT_Q(dtt1d_sort_double_check,(		/* check the sort results		    */
    ctl, x, n, F));

  dtos_free2(ctl, x, F,					/* free vector					    */
    "double vector");

  return DT_RC_GOOD;
}

/*..........................................................................*/

static
dt_rc_e
dtt1d_convolve_double_check(
  dt_ctl_t *ctl,
  double *in,
  dt1d_i_t nin,
  double *kernel,
  dt1d_i_t nkernel,
  double *out,
  dt1d_i_t nout,
  char *F,
  char *id)
{
  double *actual;
  dt1d_i_t i;
  long cmp;

  DT_Q(dtos_malloc,(ctl,				/* space for output vector		    */
    (void **)&actual,
    nout * sizeof(*actual)));

  for (i=0; i<nout; i++)				/* set edge-effect default values   */
    actual[i] = EDGE;

  DT_Q_1D(dt1d_convolve,(				/* do the convolution			    */
    DTNTYPE_F2, in, nin, 
    kernel, nkernel,
    actual, nout));
 
  DT_Q_1D(dt1d_cmp,(DT1D_NTYPE_F2,		/* compare results				    */
    actual, nout,
    out, nout, 0, &cmp));

  dt_assert(ctl, cmp == 0, F, "%s (cmp %ld)", id, cmp);

  DT_Q(dtos_free,(ctl, actual));		/* free temp space				    */

  return DT_RC_GOOD;
}


/*..........................................................................
 * test double-precision convolution routine
 *..........................................................................*/

static 
dt_rc_e
dtt1d_convolve_double(
  dt_ctl_t *ctl)
{
  DT_F("dtt1d_convolve_double");

  DTYIELD(DT_Q, ctl, DTT_YIELD_AT_UNIT, F);

										/* ................................	*/
  {
    static char *id = "convolution of 3-vector with 3-kernel";
    static double in[] =
      {1.0, 1.0, 2.0};
    static double kernel[] =
      {-1.0, 2.0, -1.0};
    static double out[] =
       {EDGE,
        -1.0,
        EDGE};
 
    DT_Q(dtt1d_convolve_double_check,(ctl,
      in, L(in), kernel, L(kernel), out, L(out), F, id));
  }
										/* ................................	*/
  {
    static char *id = "convolution of solid with smoothing kernel"; 
    static double in[] =
      {1.0, 1.0, 1.0, 1.0,
       1.0, 1.0, 1.0, 1.0};
    static double kernel[] =
      {1.0, 1.0, 1.0};
    static double out[] =
      {EDGE,
       3.0, 3.0, 3.0,
       3.0, 3.0, 3.0,
       EDGE};
 
    DT_Q(dtt1d_convolve_double_check,(ctl,
      in, L(in), kernel, L(kernel), out, L(out), F, id));
  }
										/* ................................	*/
  {
    static char *id = "convolution of solid with gradient kernel";
    static double in[] =
      {1.0, 1.0, 1.0, 1.0,
       1.0, 1.0, 1.0, 1.0};
    static double kernel[] =
      {-1.0, 2.0, -1.0};
    static double out[] =
      {EDGE,
       0.0, 0.0, 0.0,
       0.0, 0.0, 0.0,
       EDGE};
 
    DT_Q(dtt1d_convolve_double_check,(ctl,
      in, L(in), kernel, L(kernel), out, L(out), F, id));
  }
										/* ................................	*/
  {
    static char *id = "convolution of pulse with centered gradient kernel"; 
    static double in[] =
      {1.0, 1.0, 1.0, 8.0,
       1.0, 1.0, 1.0, 1.0};
    static double kernel[] =
      {-1.0, 2.0, -1.0};
    static double out[] =
      {EDGE,
       0.0, -7.0, 14.0,
       -7.0, 0.0, 0.0,
       EDGE};
 
    DT_Q(dtt1d_convolve_double_check,(ctl,
      in, L(in), kernel, L(kernel), out, L(out), F, id));
  }
										/* ................................	*/
  {
    static char *id = "convolution of pulse with uncentered gradient kernel"; 
    static double in[] =
      {1.0, 1.0, 1.0, 8.0,
       1.0, 1.0, 1.0, 1.0};
    static double kernel[] =
      {-1.0, 0.0, 1.0};
    static double out[] =
      {EDGE,
       0.0, 7.0, 0.0,
       -7.0, 0.0, 0.0,
       EDGE};
 
    DT_Q(dtt1d_convolve_double_check,(ctl,
      in, L(in), kernel, L(kernel), out, L(out), F, id));
  }

  return DT_RC_GOOD;
}


/*..........................................................................*/
dt_rc_e 
DTCONFIG_API1
dtt1d_arg_check(						/* check args and params			*/
  dt_ctl_t *ctl,
  void *app,
  dtparam_t *param,
  dt_enter_arg_t *arg,
  char *msg,
  const char **start)
{
  DT_F("dtt1d_arg_check");

  DT_Q(dtt_check_op,(ctl, param,		/* make sure op is understood	    */
    ops, msg, start));

  return DT_RC_GOOD;
}

/*..........................................................................*/

DTT_MAIN(dtt1d)
{
  DT_F("dtt1d");
  dt_ctl_t *ctl = &param->ctl;
  dt_enter_arg_t arg[1];
  int assert_fail = 0;					/* failed assertion count		    */
  dt_rc_e rc = DT_RC_GOOD;

  DT_GI(dt_heapmon_init,(ctl));			/* init memory checking			    */

  DT_GI(dtparam_init,(param));			/* init param structure             */

  DT_GI(dtparam_set,(param, 0,			/* set default for params we want	*/
    &param->op, NULL, NULL, "default",
    &param->reps, NULL, NULL, "1",
    &param->n, "Sort size", NULL, "7500",
    NULL));

  arg[0].desc = NULL;					/* no arguments					    */
  
  DT_GI(dt_enter_parse,(				/* parse command line interactively	*/
    argc, argv, F, USAGE, param,
    arg, dtt1d_arg_check, NULL));
										/* ................................ */

  {
    long reps;
    for (reps=0; 
         reps < param->reps && 
         rc == DT_RC_GOOD;
         reps++)
	{
      if (!strcmp(param->op, "all") ||
          !strcmp(param->op, "default"))
      {
        DT_Q(dtt1d_cast2,(ctl));	
        DT_Q(dtt1d_random1,(ctl));	
        DT_Q(dtt1d_random2,(ctl));	
        DT_Q(dtt1d_sort_double,(ctl));
        DT_Q(dtt1d_sort_struct,(ctl));
        DT_Q(dtt1d_variance,(ctl));		
        DT_Q(dtt1d_convolve_double,(ctl));
        DT_Q(dtt1d_fft,(ctl));	
      }
    
      if (!strcmp(param->op, "all") ||
          !strcmp(param->op, "time") ||
          !strcmp(param->op, "uniform"))
      {
        DT_Q(dtt1d_random_uniform_double_timing,(ctl, param->n));
      }
    
      if (!strcmp(param->op, "all") ||
          !strcmp(param->op, "time") ||
          !strcmp(param->op, "gauss"))
      {
        DT_Q(dtt1d_random_gauss_double_timing,(ctl, param->n));
      }
    
      if (!strcmp(param->op, "all") ||
          !strcmp(param->op, "time") ||
          !strcmp(param->op, "sort") ||
          !strcmp(param->op, "sorti"))
      {
        DT_Q(dtt1d_sort_double_invert_timing,(ctl, param->n));
      }
    
      if (!strcmp(param->op, "all") ||
          !strcmp(param->op, "time") ||
          !strcmp(param->op, "sort") ||
          !strcmp(param->op, "sortr"))
      {
        DT_Q(dtt1d_sort_double_random_timing,(ctl, param->n));
      }
	}
  }

										/* ................................ */

  DT_I(dt_leave,(param));				/* clean up before leaving			*/

  DT_I(dt_heapmon_uninit,(ctl,			/* check all memory is freed	    */
    &assert_fail));

  if (assert_fail)						/* any failed assertions?		    */
    return DT_RC_BAD;
  else
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
