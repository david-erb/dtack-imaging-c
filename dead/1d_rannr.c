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
 * this random number generator came from Numerical Recipes
 * here is the structure in dt1d.h:
   typedef struct {
     long ix1, ix2, ix3;
     double r[98];
   } dt1d_ran_uniform_t;

 * first 8 numbers from uniform with seed=0
    static double b[N] = {
      2.742720152221e-01,
      5.690608566897e-01,
      3.253029373176e-01,
      5.608891969317e-02,
      7.483756503745e-01,
      4.135940964463e-01,
      5.743546164501e-01,
      5.509031460886e-02};
 
 * first 8 numbers from Gaussian with seed=0
    static double b[N] = {
	  5.069124659059e-01,
	  -1.656862293236e+00,
	  -4.034191831599e-01,
	  -1.587618544666e-01,
	  -5.267551675908e-01,
	  1.514169193975e+00,
	  -6.329789398979e-01,
	  1.057853039448e-01};
 *..........................................................................*/

#include <dtack/ntype.h>
#include <dtack/stats.h>
#include <dtack/1d.h>

/* ........................................................................
 * ran1
 * Returns a uniform random deviate between 0.0 and 1.0.
 * Set idum to any negative value to initialize or reinitialize the
 *   sequence.
 * Numerical Recipes in C page 210
 * ........................................................................	*/

#define M1 259200L
#define IA1 7141L
#define IC1 54773L
#define RM1 (1.0/M1)

#define M2 134456L
#define IA2 8121L
#define IC2 28411L
#define RM2 (1.0/M2)

#define M3 243000L
#define IA3 4561L
#define IC3 51349L

/* ........................................................................
 * ........................................................................	*/

dt1d_rc_e
DT1D_API
dt1d_ran_uniform(
  dt1d_ran_t *ran,
  double *x)
{
  dt1d_ran_uniform_t *r;
  int j;

  r = &ran->uniform;					/* deference control structure	    */
  r->ix1 = (IA1 * r->ix1 + IC1) % M1;	/* generate next number			    */
  r->ix2 = (IA2 * r->ix2 + IC2) % M2;
  r->ix3 = (IA3 * r->ix3 + IC3) % M3;
  j = 1 + (int)((97 * r->ix3) / M3);	/* integer between 1 and 97		    */
  if (j > 97 || j < 1)
    return DT1D_RC_BAD;
  *x = r->r[j];							/* return table entry			    */
  r->r[j] = (r->ix1+r->ix2*RM2) * RM1;	/* refill table entry			    */
  return DT1D_RC_GOOD;
}

/*..........................................................................
 * double vector uniform random number generation
 *..........................................................................*/

dt1d_rc_e
DT1D_API
dt1d_ran_uniform_init(
  dt1d_ran_t *ran,
  long seed)
{
  dt1d_ran_uniform_t *r;
  int j;
  long ix1, ix2, ix3;

  r = &ran->uniform;					/* deference control structure	    */
  ix1 = (IC1 - seed) % M1;
  ix1 = (IA1 * ix1 + IC1) % M1;
  ix2 = ix1 % M2;
  ix1 = (IA1 * ix1 + IC1) % M1;
  ix3 = ix1 % M3;
  for (j=1; j<=97; j++)
  {
    ix1 = (IA1 * ix1 + IC1) % M1;
    ix2 = (IA2 * ix2 + IC2) % M2;
    r->r[j] = (ix1 + ix2 * RM2) * RM1;
  }
  r->ix1 = ix1;
  r->ix2 = ix2;
  r->ix3 = ix3;
  return DT1D_RC_GOOD;
}

#ifdef NONO
/* ........................................................................
 * gasdev
 * Returns a normally distributed deviate with zero mean and unit variance
 * using ran1(idum) as the source of uniform deviates.
 * Numerical Recipes in C page 216-217
 * ........................................................................	*/

static
double gasdev(
  int *idum)
{
  static int iset = 0;
  static double gset;
  double fac, r, v1, v2;
  if (iset == 0)
  {
	do {
      v1 = 2.0 * ran1(idum) - 1.0;
      v2 = 2.0 * ran1(idum) - 1.0;
      r = v1 * v1 + v2 * v2;
	} while (r >= 1.0);
    fac = sqrt(-2.0 * log(r) / r);
    gset = v1 * fac;
    iset = 1;
    return v2 * fac;
  }
  else
  {
    iset = 0;
    return gset;
  }
}
#endif

/* ........................................................................
 * gauss random number generation
 * ........................................................................	*/

dt1d_rc_e
DT1D_API
dt1d_ran_gauss(
  dt1d_ran_t *ran,
  double *x)
{
  double fac, radius, v1, v2;

  if (ran->gauss.iset == 0)
  {
	do {
      dt1d_ran_uniform(ran, &v1);
      dt1d_ran_uniform(ran, &v2);
      v1 = 2.0 * v1 - 1.0;
      v2 = 2.0 * v2 - 1.0;
      radius = v1 * v1 + v2 * v2;
	} while (radius >= 1.0);
    fac = sqrt(-2.0 * log(radius) / radius);
    ran->gauss.gset = v1 * fac;
    ran->gauss.iset = 1;
    *x = v2 * fac;
  }
  else
  {
    ran->gauss.iset = 0;
    *x = ran->gauss.gset;
  }

  return DT1D_RC_GOOD;
}

/*..........................................................................
 * double vector gauss random number initialization
 *..........................................................................*/

dt1d_rc_e
DT1D_API
dt1d_ran_gauss_init(
  dt1d_ran_t *ran,
  long seed)
{
  dt1d_ran_uniform_init(ran, seed);		/* re-init the uniform generator	*/
  ran->gauss.iset = 0;					/* clear our own flip-flop		    */
  return DT1D_RC_GOOD;
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
