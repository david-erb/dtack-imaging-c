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
 * this random number generation code came from 
 * Dwyer, J. "Quick and Portable Random Number Generators",
 *   C/C++ User's Journal 13:6 p.33 June 1995
 *
 * Needs work: some of the assertions fail when running on Unix.
 *..........................................................................*/

#include <dtack/ntype.h>
#include <dtack/stats.h>
#include <dtack/1d.h>

/* Listing 2
	rand_por[t].c
	a portable and reasonably fast random number generator
		multiplicative random number generator
	see
	   L'Ecuyer - Comm. of the ACM, Oct. 1990, vol. 33.
	   Numerical Recipes in C, 2nd edition, pp.  278-86
	   L'Ecuyer and Cote, ACM Transactions on Mathematical Software,
		 March 1991
	   Russian peasant algorithm -- Knuth, vol. II, pp.  442-43
	Copyright (c) 1994, 1995 by Gerald P. Dwyer, Jr.     */

#include	<time.h>
#include	<stdlib.h>
#include	<limits.h>
#include	<assert.h>

#define	TESTING

#define	TRUE (-1)
#define	FALSE 0

long init_rand_port(long seed) ;
long get_init_rand_port(void) ;
long genr_rand_port(long init_rand) ;
long rand_port(void) ;
double rand_rect_port(void) ;

#define  MOD    2147483647L     /* modulus for generator */
#define  MULT        41358L     /* multiplier            */
                                /* modulus = mult*quotient + remainder */
#define  Q           51924L     /* int(modulus / multiplier) */
#define  R           10855L     /* remainder                 */
#define  MAX_VALUE    (MOD-1)

#define  EXP_VAL 1285562981L    /* value for 10,000th draw */

#define  IMPOSSIBLE_RAND (-1)
#define  STARTUP_RANDS   16     /* throw away this number of
						  initial random numbers */

static long rand_num = IMPOSSIBLE_RAND ;

/* initialize random number generator with seed */
long init_rand_port(long seed)
{
	extern long rand_num ;
	int i ;

	if (seed < 1 || seed > MAX_VALUE)		/* if seed out of range */
		seed = get_init_rand_port() ;			/* get seed */

	rand_num = seed ;
	for (i = 0; i < STARTUP_RANDS; i++)	/* and throw away */
		rand_num = genr_rand_port(rand_num) ;	/* some initial ones */

	return seed ;
}


/* get a long initial seed for generator
	assumes that rand returns a short integer */
long get_init_rand_port(void)
{
	long seed ;

	srand((unsigned int)time(NULL)) ;    /* initialize system generator */
	do {
		seed  = ((long)rand())*rand() ;
		seed += ((long)rand())*rand() ;
	} while (seed > MAX_VALUE) ;

	assert (seed > 0) ;

	return seed ;
}


/* generate the next value in sequence from generator
	uses approximate factoring
	residue = (a * x) mod modulus
			= a*x - [(a*x)/modulus]*modulus
	where
		[(a*x)/modulus] = integer less than or equal to (a*x)/modulus
	approximate factoring avoids overflow associated with a*x and
		uses equivalence of above with
	residue = a * (x - q * k) - r * k + (k-k1) * modulus
	where
		modulus = a * q + r
		q  = [modulus/a]
		k  = [x/q]		(= [ax/aq])
		k1 = [a*x/modulus]
	assumes
		a, m > 0
		0 < init_rand < modulus
		a * a <= modulus
		[a*x/a*q]-[a*x/modulus] <= 1
			(for only one addition of modulus below) */
long genr_rand_port(long init_rand)
{
	long k, residue ;

	k = init_rand / Q ;
	residue = MULT * (init_rand - Q * k) - R * k ;
	if (residue < 0)
		residue += MOD ;

	assert(residue >= 1 && residue <= MAX_VALUE) ;

	return residue ;
}


/* get a random number */
long rand_port(void)
{
	extern long rand_num ;

				/* if not initialized, do it now */
	if (rand_num == IMPOSSIBLE_RAND) {
		rand_num = 1 ;
		init_rand_port(rand_num) ;
	}

	rand_num = genr_rand_port(rand_num) ;

	return rand_num ;
}


/* generates a value on (0,1) with mean of .5
	range of values is [1/(MAX_VALUE+1), MAX_VALUE/(MAX_VALUE+1)]
	to get [0,1], use (double)(rand_port()-1)/(double)(MAX_VALUE-1) */
double rand_rect_port(void)
{
	return (double)rand_port()/(double)(MAX_VALUE+1) ;
}


/* ........................................................................
 * ........................................................................	*/

dt1d_rc_e
DT1D_API
dt1d_ran_uniform(
  dt1d_ran_t *ran,
  double *x)
{
  *x = rand_rect_port();
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
  init_rand_port(seed);
}

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
