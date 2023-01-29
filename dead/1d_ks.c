/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
  ! Copyright (C) 1998 Dtack Inc. All Rights Reserved
  ! To use this file, you must have signed a license agreement with Dtack Inc.
  ! Under no circumstances may you redistribute this file.
  ! This software is provided AS IS with no warranty expressed or implied.
  ! Dtack Inc. accepts no liability for use or misuse of this file.
  ! http://www.dtack.com  dtack@dtack.com  telephone +360.417.6700
  ! Dtack Inc, P.O. Box 3061, Port Angeles, WA USA 98362
  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */




This first section is from old dtt1d.c:

typedef double dt1d_ks_f(double *, dt1d_i_t, dt1d_i_t, void *);

  cdf_normal_t normal;

  DT_Q(dt1d_sort_double,(				/* do the sort					    */
    x, N));
  DT_Q(dt1d_ks_double,(					/* compute ks					    */
    ctl, x, N,
    cdf_uniform, NULL,
    &ksd, &ksp));

  DT_Q(dtt1d_double_check,(ctl,
    ksd, 5.68095889325e-03, F, "uniform Kolmogorov-Smirnov D"));
  DT_Q(dtt1d_double_check,(ctl,
    ksp, 9.68840000000e-01, F, "uniform Kolmogorof-Smirnov D significance"));



  DT_Q(dt1d_sort_double,(				/* do the sort					    */
    x, N));

  normal.mean = 0.0;					/* what we need in cdf_normal	    */
  normal.variance = 1.0;
  normal.sigma = sqrt(normal.variance);
  normal.scale1 = 1.0 /
    (normal.sigma * sqrt(2.0*DT_PI));
  normal.scale2 = 2.0 * normal.variance;

  DT_Q(dt1d_ks_double,(					/* compute ks					    */
    ctl, x, N,
    cdf_normal, &normal,
    &ksd, &ksp));
  DT_Q(dtt1d_double_check,(ctl,
    ksd, 9.99758677363e-01, F, "gauss Kolmogorov-Smirnov D"));
  DT_Q(dtt1d_double_check,(ctl,
    ksp, 1e-6, F, "gauss Kolmogorov-Smirnov D significance"));

/*..........................................................................
 * Numerical Recipes in C page 492
 * assumes data is sorted
 *..........................................................................*/

#define EPS1 1.0e-3
#define EPS2 1.0e-8

/*..........................................................................
 * return significance level
 * returns values between 0.0 (cdf's are completely different) and
 *   1.0 (cdf's are exactly the same)
 * small values of this function show that the cumulative distribution
 *   functions are significantly different
 * to be conservative, you wouldn't say they are different unless this
 *   function returned less than 0.01
 *..........................................................................*/

static 
double dt1d_ks_prob(
  dt_ctl_t *ctl,
  double alam)
{
  DT_F("dt1d_ks_prob");
  int j;
  double a2;
  double sum = 0.0;
  double term;
  double termbf = 0.0;
  long mask = DT_DBG_MASK_INTERMD;
  long debug = ctl->dbg_mask & mask;

  if (debug)
    dt_dbg(ctl, F, mask, "alam %1.11e", alam);

  if (alam < 0.05)
    return 0.999999;
  if (alam > 20.0)
    return 0.000001;

  a2 = -2.0 * alam * alam;
  for (j=1; j<=1000; j+=2)
  {
	term = 2.0 * (exp(a2 * j * j) - exp(a2 * (j+1)*(j+1)));
    sum += term;
    if (debug && j < 10)
      dt_dbg(ctl, F, mask,
        "  %d. term %1.11e sum %1.11e",
        j, term, sum);
    if (term <= EPS1 * termbf ||
        term < EPS2 * sum)
	  break;
    termbf = term;
  }
  if (debug && j > 10)
    dt_dbg(ctl, F, mask,
      "  %d. final term %1.11e sum %1.11e",
      j, term, sum);
  return (long)(sum * 1e6) / 1e6;
}

static
dt_rc_e 
dt1d_ks_one_double(
  dt_ctl_t *ctl,
  double *data,
  dt1d_i_t n,
  dt1d_ks_f *func,
  void *arg,
  double *d,
  double *prob)
{
  DT_F("dt1d_ks_one_double");
  int j;
  double fo=0.0;
  double fn, ff, en;
  double max;
  double t;
  long mask = DT_DBG_MASK_INTERMD;
  long debug = ctl->dbg_mask & mask;

  DTYIELD(DT_Q, ctl, DTT_YIELD_AT_UNIT, F);

  en = n;
  max = 0.0;
  for (j=1; j<=n; j++)
  {
	fn = (double)j / en;
    ff = func(data+1, n, j-1, arg);
#ifdef NONO
printf("%2d. fo %e fn %e ff %e\n", j, fo, fn, ff);
#endif

    t = fo - ff;
    if (t < 0.0) t = -t;
    if (t > max) max = t;

    t = fn - ff;
    if (t < 0.0) t = -t;
    if (t > max) max = t;

    if (debug && (j < 10 || max == t))
      dt_dbg(ctl, F, mask, "%6d. fn %e, ff %e, fn-ff %e",
        j, fn, ff, t);

    fo = fn;		
  }
  if (debug)
    dt_dbg(ctl, F, mask, "n %d, sqrt(n) %1.0f, D %1.6f",
      n, sqrt(en), max);

  *d = max;
  *prob = dt1d_ks_prob(ctl, sqrt(en) * max);
  return DT_RC_GOOD;
}

/*..........................................................................
 * make ks call and check result
 *..........................................................................*/


dt_rc_e
dt1d_ks_double(
  dt_ctl_t *ctl,
  double *v,
  dt1d_i_t nv,
  dt1d_ks_f *func,
  void *arg,
  double *d,
  double *p)
{
  DT_F("dt1d_ks_double");
  DT_Q(dt1d_ks_one_double,(ctl, v-1, nv, func, arg, d, p));
  return DT_RC_GOOD;
}

/*..........................................................................
 * value of uniform cdf at index n
 *..........................................................................*/

static double cdf_uniform(
  double *v,
  dt1d_i_t nv,
  dt1d_i_t n,
  void *arg)
{
  return v[n];
}

/*..........................................................................
 * value of normal cdf at index n
 *..........................................................................*/

typedef struct {
  double mean;
  double variance;
  double sigma;
  double scale1;
  double scale2;
} cdf_normal_t;

static double cdf_normal(
  double *v,
  dt1d_i_t nv,
  dt1d_i_t n,
  void *arg)
{
  cdf_normal_t *a = (cdf_normal_t *)arg;
  double x = v[n] - a->mean;
  return a->scale1 * exp(-x*x/a->scale2);
}

/*..........................................................................
 * make ks call and check result
 *..........................................................................*/

static
dt_rc_e
dtt1d_ks_double_check(
  dt_ctl_t *ctl,
  char *F,
  char *id,
  double *b,
  dt1d_i_t n,
  dt1d_ks_f func,
  void *arg,
  double dwant,
  double pwant)
{
  double dgot, pgot;
  char id2[80];

  DT_Q(dt1d_ks_double,(ctl,				/* compute ks					    */
    b, n, func, arg,
    &dgot, &pgot));

  sprintf(id2, "%s n=%ld D",
    id, (long)n);
  DT_Q(dtt1d_double_check,(ctl,			/* check d result				    */
    dgot, dwant, F, id2));

  sprintf(id2, "%s prob", id);
  DT_Q(dtt1d_double_check,(ctl,			/* check p result				    */
    pgot, pwant, F, id2));

  return DT_RC_GOOD;
}

/*..........................................................................
 * test ks routine(s)
 *..........................................................................*/

static 
dt_rc_e
dtt1d_ks_double(
  dt_ctl_t *ctl)
{
  DT_F("dtt1d_ks_double");
  dt1d_i_t n;
  double *x;
  dt1d_i_t i;
  char id[32];


#define N 100
  DT_Q(dtos_malloc2,(ctl, 
    (void **)&x, N * sizeof(*x),
    F, "buffer"));

  n = 10;
  sprintf(id, "linear %ld", (long)n);
  for (i=0; i<n; i++)
    x[i] = (double)i / (double)n;
  DT_Q(dtt1d_ks_double_check,(ctl,
    F, id, x, n, cdf_uniform, NULL,
    1.0 / (double)n, 9.99965e-01));

  n = N;
  sprintf(id, "linear %ld", (long)n);
  for (i=0; i<n; i++)
    x[i] = (double)i / (double)n;
  DT_Q(dtt1d_ks_double_check,(ctl,
    F, id, x, n, cdf_uniform, NULL,
    1.0 / (double)n, 9.99999e-01));

  dtos_free2(ctl, x, F, "buffer");
#undef N

										/* ................................	*/
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
