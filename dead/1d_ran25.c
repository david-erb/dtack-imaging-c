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
 * http://www.taygeta.com/random.html
 *
 * haven't tried this one yet
 *..........................................................................*/

/* r250.h	prototypes for r250 random number generator,

		Kirkpatrick, S., and E. Stoll, 1981; "A Very Fast
		Shift-Register Sequence Random Number Generator",
		Journal of Computational Physics, V.40

		also:

		see W.L. Maier, DDJ May 1991


*/

#ifndef _R250_H_
#define _R250_H_ 1.2

#ifdef __cplusplus
extern "C" {
#endif

#ifdef NO_PROTO
void         r250_init();
unsigned int r250();
double      dr250();

#else
void         r250_init(int seed);
unsigned int r250( void );
double       dr250( void );
#endif

#ifdef __cplusplus
}
#endif

#endif

/*..........................................................................
 *..........................................................................*/

/* r250.c	the r250 uniform random number algorithm

		Kirkpatrick, S., and E. Stoll, 1981; "A Very Fast
		Shift-Register Sequence Random Number Generator",
		Journal of Computational Physics, V.40

		also:

		see W.L. Maier, DDJ May 1991



*/

static char rcsid[] = "@(#)r250.c	1.2 15:50:31 11/21/94   EFC";


/
#include <limits.h>

#include "r250.h"

/* set the following if you trust rand(), otherwise the minimal standard
   generator is used
*/
/* #define TRUST_RAND */


#ifndef TRUST_RAND
#include "randlcg.h"
#endif

/* defines to allow for 16 or 32 bit integers */
#define BITS 31


#if WORD_BIT == 32
#ifndef BITS
#define BITS	32
#endif
#else
#ifndef BITS
#define BITS    16
#endif
#endif

#if BITS == 31
#define MSB          0x40000000L
#define ALL_BITS     0x7fffffffL
#define HALF_RANGE   0x20000000L
#define STEP         7
#endif

#if BITS == 32
#define MSB          0x80000000L
#define ALL_BITS     0xffffffffL
#define HALF_RANGE   0x40000000L
#define STEP         7
#endif

#if BITS == 16
#define MSB         0x8000
#define ALL_BITS    0xffff
#define HALF_RANGE  0x4000
#define STEP        11
#endif

static unsigned int r250_buffer[ 250 ];
static int r250_index;

#ifdef NO_PROTO
void r250_init(sd)
int seed;
#else
void r250_init(int sd)
#endif
{
	int j, k;
	unsigned int mask, msb;

#ifdef TRUST_RAND        

#if BITS == 32 || BITS == 31       
	srand48( sd );
#else
	srand( sd );
#endif	


#else
	set_seed( sd );
#endif
	
	r250_index = 0;
	for (j = 0; j < 250; j++)      /* fill r250 buffer with BITS-1 bit values */
#ifdef TRUST_RAND
#if BITS == 32 || BITS == 31
		r250_buffer[j] = (unsigned int)lrand48();
#else
		r250_buffer[j] = rand();
#endif
#else
		r250_buffer[j] = randlcg();
#endif


	for (j = 0; j < 250; j++)	/* set some MSBs to 1 */
#ifdef TRUST_RAND
		if ( rand() > HALF_RANGE )
			r250_buffer[j] |= MSB;
#else
		if ( randlcg() > HALF_RANGE )
			r250_buffer[j] |= MSB;
#endif


	msb = MSB;	        /* turn on diagonal bit */
	mask = ALL_BITS;	/* turn off the leftmost bits */

	for (j = 0; j < BITS; j++)
	{
		k = STEP * j + 3;	/* select a word to operate on */
		r250_buffer[k] &= mask; /* turn off bits left of the diagonal */
		r250_buffer[k] |= msb;	/* turn on the diagonal bit */
		mask >>= 1;
		msb  >>= 1;
	}

}

unsigned int r250()		/* returns a random unsigned integer */
{
	register int	j;
	register unsigned int new_rand;

	if ( r250_index >= 147 )
		j = r250_index - 147;	/* wrap pointer around */
	else
		j = r250_index + 103;

	new_rand = r250_buffer[ r250_index ] ^ r250_buffer[ j ];
	r250_buffer[ r250_index ] = new_rand;

	if ( r250_index >= 249 )	/* increment pointer for next time */
		r250_index = 0;
	else
		r250_index++;

	return new_rand;

}


double dr250()		/* returns a random double in range 0..1 */
{
	register int	j;
	register unsigned int new_rand;

	if ( r250_index >= 147 )
		j = r250_index - 147;	/* wrap pointer around */
	else
		j = r250_index + 103;

	new_rand = r250_buffer[ r250_index ] ^ r250_buffer[ j ];
	r250_buffer[ r250_index ] = new_rand;

	if ( r250_index >= 249 )	/* increment pointer for next time */
		r250_index = 0;
	else
		r250_index++;

	return (double)new_rand / ALL_BITS;

}

#ifdef MAIN

/* test driver	prints out either NMR_RAND values or a histogram	*/

#include <stdio.h>

#define NMR_RAND	5000
#define MAX_BINS	500

#ifdef NO_PROTO
void main(argc, argv)
int argc;
char **argv;
#else
void main(int argc, char **argv)
#endif
{
	int j,k,nmr_bins,seed;
	int bins[MAX_BINS];
	double randm, bin_inc;
	double bin_limit[MAX_BINS];

	if ( argc != 3 )
	{
		printf("Usage -- %s nmr_bins seed\n", argv[0]);
		exit(1);
	}

	nmr_bins = atoi( argv[1] );
	if ( nmr_bins > MAX_BINS )
	{
		printf("ERROR -- maximum number of bins is %d\n", MAX_BINS);
		exit(1);
	}

	seed = atoi( argv[2] );

	r250_init( seed );

	if ( nmr_bins < 1 )	/* just print out the numbers */
	{
		for (j = 0; j < NMR_RAND; j++)
			printf("%f\n", dr250() );
		exit(0);
	}
	
	bin_inc = 1.0 / nmr_bins;
	for (j = 0; j < nmr_bins; j++)	/* initialize bins to zero */
	{
		bins[j] = 0;
		bin_limit[j] = (j + 1) * bin_inc;
	}

	bin_limit[nmr_bins-1] = 1.0e7;	/* make sure all others are in last bin */

	for (j = 0; j < NMR_RAND; j++)
	{
		randm = r250() / (double)ALL_BITS;
		for (k = 0; k < nmr_bins; k++)
			if ( randm < bin_limit[k] )
			{
				bins[k]++;
				break;
			}
	}


	for (j = 0; j < nmr_bins; j++)
		printf("%d\n", bins[j]);
	
}

#endif

/*..........................................................................
 *..........................................................................*/

/* randlcg.h	prototypes for the minimal standard random number generator,

   Linear Congruential Method, the "minimal standard generator"
   Park & Miller, 1988, Comm of the ACM, 31(10), pp. 1192-1201


  rcsid: @(#)randlcg.h	1.1 15:48:09 11/21/94   EFC

*/

#ifndef _RANDLCG_H_
#define _RANDLCG_H_ 1.1

#ifdef __cplusplus
extern "C" {
#endif

#ifdef NO_PROTO
long         set_seed();
long         get_seed();
dtntype_b32_t int randlcg();

#else
long         set_seed(long);
long         get_seed(long);
dtntype_b32_t int randlcg();

#endif

#ifdef __cplusplus
}
#endif

#endif

/*..........................................................................
 *..........................................................................*/

/* rndlcg            Linear Congruential Method, the "minimal standard generator"
                     Park & Miller, 1988, Comm of the ACM, 31(10), pp. 1192-1201

*/

static char rcsid[] = "@(#)randlcg.c	1.1 15:48:15 11/21/94   EFC";

#include <math.h>
#include <limits.h>

#define ALL_BITS     0xffffffff

static long int quotient  = LONG_MAX / 16807L;
static long int remainder = LONG_MAX % 16807L;

static long int seed_val = 1L;

long set_seed(long int sd)
{
        return seed_val = sd;
}

long get_seed()
{
        return seed_val;
}


dtntype_b32_t int randlcg()       /* returns a random unsigned integer */
{
        if ( seed_val <= quotient )
                seed_val = (seed_val * 16807L) % LONG_MAX;
        else
        {
                long int high_part = seed_val / quotient;
                long int low_part  = seed_val % quotient;

                long int test = 16807L * low_part - remainder * high_part;

                if ( test > 0 )
                        seed_val = test;
                else
                        seed_val = test + LONG_MAX;

        }

        return seed_val;
}






/*..........................................................................
 *..........................................................................*/

/* quasi.h		Quasi-Random Number generator, an object oriented
                        implementation in C.

		returns an n-dimensional vector of values in 0.0..1.0
		maximum n is currently hardwired to 52

			See W.H. Press and S.A. Teukolsky, 1989, Quasi- (that is,
			Sub-) Random Numbers, Computers in Physics V3, No. 6,
			(Nov/Dec 1989), pp. 76-79


                rcsid: @(#)quasi.h	1.5 10:15:46 4/18/94   EFC

*/

#ifndef QUASI_RANDOM_H_
#define QUASI_RANDOM_H_ 1.5


typedef struct       /* data structure to maintain internal information */
{                    /* this allows multiple independent generators to exist in
                        a single application (each one would have its own
			QRStruct) */

      int err_flag;             /* err_flag == 0 if all is well */
      int dim;
      dtntype_b32_t int index;
      dtntype_b32_t int *ix;
} QRStruct;

#ifdef __cplusplus
extern "C" {
#endif

/* initialized the data structure for this dimension */
int QuasiRandomInitialize(QRStruct* qr, int dimension);

/* release internal storage for this QRStruct */
void QuasiRandomRelease(QRStruct* qr);

/* get an n-dimensional quasi-random number */
void QuasiRandomNumber(QRStruct* qr, float* x);


#ifdef __cplusplus
}
#endif

#endif






/*..........................................................................
 *..........................................................................*/

/* quasi.c		Implementation of the Quasi-Random Number generator
			currently hardwired to no more than 52 dimensions

			See W.H. Press and S.A. Teukolsky, 1989, Quasi- (that is,
			Sub-) Random Numbers, Computers in Physics V3, No. 6,
			(Nov/Dec 1989), pp. 76-79


*/

static char rcsid[] = "@(#)quasi.c	1.5 10:15:59 4/18/94   EFC";

#include <stdio.h>

#include "quasi.h"

/* the primitive polynomial coefficients for up to degree 8  */
static int ip[] = { 0, 1, 1, 2, 1, 4, 2, 4, 7, 11, 13, 14,
			  1, 13, 16, 19, 22, 25,
			  1, 4, 7, 8, 14, 19, 21, 28, 31, 32, 37, 41, 42,
			  50, 55, 56, 59, 62,
			  14, 21, 22, 38, 47, 49, 50, 52, 56, 67, 70, 84,
			  97, 103, 115, 122 };
static int mdeg[] = { 1, 2, 3, 3, 4, 4, 5, 5, 5, 5, 5, 5,
			    6, 6, 6, 6, 6, 6,
			    7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
			    8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8
			     };

static int            maxdim = sizeof(mdeg) / sizeof(int);

static int            maxbit = 30;	    /* must be no more than
					       number of bits in uint32 - 1 */

static dtntype_b32_t  int*  iv = NULL;
static double               factor = 1.0;
static int instances = 0;

#define INDEX(k,j)	[(k) + (j-1) * maxdim]


static void QuasiInit()	               /* initialize the direction numbers */
{                                      /* only done once */
	int j, k, l, ipp, niv;
	dtntype_b32_t int mval, limit;
	dtntype_b32_t int i;
        
        iv = (dtntype_b32_t int *)malloc( (niv = maxdim * maxbit)
					 * sizeof( dtntype_b32_t int) );

	for (k = 0; k < niv; k++)
        	iv[k] = 0;

        for (k = 0; k < maxdim; k++)
        	iv[k] = 1;

	mval = 4;
	ipp = 1;

        limit = (1L << (maxbit-1) );

        for (k = maxdim, j = 0; k < niv-1; k += 2)
        {
		iv[k] = ipp;
                if (++j == maxdim)
                {
		        if ( mval < limit )
			          mval *= 2;
                        ipp += 2;
                        j = 0;
                }

		if ( ipp > mval )
                	ipp = 1;
                        
                iv[k+1] = ipp;
                if (++j == maxdim)
                {
		        if ( mval < limit )
			          mval *= 2;
                        ipp += 2;
                        j = 0;
                }
		else
                {
                	ipp += 2;
                        if ( ipp > mval )
                        	ipp = 1;
                }
                

        }

	for (k = 0; k < maxdim; k++)
        {
        	/* normalize the set iv values */
        	for (j = 1; j <= mdeg[k]; j++)
                	iv INDEX(k,j) *= (1L << (maxbit - j));

		/* calcululate the rest of the iv values */
		for (j = mdeg[k] + 1; j <= maxbit; j++)
                {
                	ipp = ip[k];
                        
                        /* calculate Gray code of iv */
                        i = iv INDEX(k, j - mdeg[k]);
                        i ^= i / (1L << mdeg[k]);
                        
                        for (l = mdeg[k] - 1; l >= 1; l--)
                        {
                        	if ( ipp & 1 )
                                	i ^= iv INDEX(k, j-l);
                                ipp /= 2;
                        }

                        iv INDEX(k,j) = i;

        	}
	}

        factor = 1.0 / (1L << maxbit);

                        
}

int QuasiRandomInitialize(QRStruct* qr, int dimension)
{
        int k;

        qr->dim = dimension;
	qr->index = 0;

	if ( qr->dim > maxdim )		/* if dimension is too large   */
        {				/* truncate and set error flag */
        	qr->dim = maxdim;
		qr->err_flag = -1;
        }
        else
        	qr->err_flag = 0;
                                
	qr->ix = (dtntype_b32_t int *)malloc(qr->dim * sizeof( dtntype_b32_t int) );

	for (k = 0; k < qr->dim; k++)
        	qr->ix[k] = 0L;

                
	if ( instances++ == 0 )
		QuasiInit();

	return qr->err_flag;
}

void QuasiRandomRelease(QRStruct* qr)
{
	if ( --instances == 0 )
	    free( iv );

	free( qr->ix );

	qr->err_flag = -9;
}

void QuasiRandomNumber(QRStruct* qr, float* x)
{
	int i, j, k;
	dtntype_b32_t int im = qr->index++;

        /* find rightmost zero bit  */
        for (j = 0; j < maxbit; j++, im >>= 1)
        	if ( (im & 1L) == 0 )
                		break;

	i = j * maxdim;

        for (k = 0; k < qr->dim; k++)
        {
        	qr->ix[k] ^= iv[i + k];		/* integer values  */
                x[k]   = (float) ( factor * (double)qr->ix[k] );
        }

}


#ifdef MAIN

/* Demonstration test code, writes quasirandom numbers to standard out


   To compile with the test driver:

   cc -DMAIN -o quasi quasi.c -lm



   usage:

      quasi [points [dimension]]


*/

int main(int argc, char** argv)
{
    int i, k;
    float *x;
    QRStruct qr;
    int dim = 0;
    int points = 0;

    if ( argc > 1 )
          points = atoi( argv[1] );

    if ( argc > 2 )
          dim = atoi( argv[2] );

    if ( points < 1 )
           points = 100;    /* default to 100 points */

    if ( dim < 1 )
           dim = 2;         /* default to 2 dimensional points */


    x = (float *)malloc( dim * sizeof( float ) );

    QuasiRandomInitialize( &qr, dim );

    for (i = 0; i < points; i++)
    {
              QuasiRandomNumber( &qr, x );
              for (k = 0; k < dim; k++)
                       printf("%f  ", x[k]);
	      putchar( '\n' );
    }


    QuasiRandomRelease( &qr );

    free( x );

}

#endif








/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
  ! Copyright (C) 1998 Dtack Inc. All Rights Reserved
  ! To use this file, you must have signed a license agreement with Dtack Inc.
  ! Under no circumstances may you redistribute this file.
  ! This software is provided AS IS with no warranty expressed or implied.
  ! Dtack Inc. accepts no liability for use or misuse of this file.
  ! http://www.dtack.com  dtack@dtack.com  telephone +360.417.6700
  ! Dtack Inc, P.O. Box 3061, Port Angeles, WA USA 98362
  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */
