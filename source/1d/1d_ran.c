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
| END
 *..........................................................................*/


/*..........................................................................
 * the uniform random number generation code came from 
 * FreeBSD 2.1 source code (Linux's is similar).
 *..........................................................................*/

#include <dtack/config.h>				/* platform-specific defines		*/
#include <dtack/ntype.h>
#include <dtack/stats.h>
#include <dtack/1d.h>

#define	RAND48_SEED_0	(0x330e)
#define	RAND48_MULT_0	(0xe66d)
#define	RAND48_MULT_1	(0xdeec)
#define	RAND48_MULT_2	(0x0005)
#define	RAND48_ADD	(0x000b)

static
const
unsigned short _rand48_mult[3] = {
	RAND48_MULT_0,
	RAND48_MULT_1,
	RAND48_MULT_2
};

typedef struct {
  unsigned short seed[3];
} rand48_t;

/* ........................................................................
 * Copyright (c) 1993 Martin Birgmeier
 * All rights reserved.
 *
 * You may redistribute unmodified or modified versions of this source
 * code provided that the above copyright notice and this and the
 * following conditions are retained.
 *
 * This software is provided ``as is'', and comes with no warranties
 * of any kind. I shall in no event be liable for anything that happens
 * to anyone/anything when using this software.
 * ........................................................................	*/

static
void
_dorand48(unsigned short xseed[3])
{
	dtntype_b32_t accu;
	unsigned short temp[2];

	accu = (dtntype_b32_t) _rand48_mult[0] * (dtntype_b32_t) xseed[0] +
	 (dtntype_b32_t)RAND48_ADD;
	temp[0] = (unsigned short) accu;	/* lower 16 bits */
	accu >>= sizeof(unsigned short) * 8;
	accu += (dtntype_b32_t) _rand48_mult[0] * (dtntype_b32_t) xseed[1] +
	 (dtntype_b32_t) _rand48_mult[1] * (dtntype_b32_t) xseed[0];
	temp[1] = (unsigned short) accu;	/* middle 16 bits */
	accu >>= sizeof(unsigned short) * 8;
	accu += _rand48_mult[0] * xseed[2] + _rand48_mult[1] * xseed[1] + _rand48_mult[2] * xseed[0];
	xseed[0] = temp[0];
	xseed[1] = temp[1];
	xseed[2] = (unsigned short) accu;
}

/* ........................................................................
 * ........................................................................	*/

dt1d_rc_e
DT1D_API
dt1d_ran_uniform(
  dt1d_ran_t *ran,
  double *x)
{
  rand48_t *rand48 = (rand48_t *)&ran->uniform.priv;
  _dorand48(rand48->seed);
  *x = ldexp((double) rand48->seed[0], -48) +
       ldexp((double) rand48->seed[1], -32) +
	   ldexp((double) rand48->seed[2], -16);
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
  rand48_t *rand48 = (rand48_t *)&ran->uniform.priv;
  rand48->seed[0] = RAND48_SEED_0;
  rand48->seed[1] = (unsigned short) seed;
  rand48->seed[2] = (unsigned short) (seed >> 16);
  return DT1D_RC_GOOD;
}

/* ........................................................................
 * gauss random number generation
 * Box-Mueller http://www.taygeta.com/random/gaussian.html
 * boxmuller.c           Implements the Polar form of the Box-Muller
                         Transformation

                      (c) Copyright 1994, Everett F. Carter Jr.
                          Permission is granted by the author to use
                          this software for any application provided this
            +              copyright notice is preserved.
 * this is 0 mean and 1.0 SD
 * to change mean or SD, do M + this * SD
 * ........................................................................	*/

dt1d_rc_e
DT1D_API
dt1d_ran_gauss(
  dt1d_ran_t *ran,
  double *x)
{
  double w, v1, v2;

  if (ran->gauss.second == 0)
  {
	do {
      dt1d_ran_uniform(ran, &v1);
      dt1d_ran_uniform(ran, &v2);
      v1 = 2.0 * v1 - 1.0;
      v2 = 2.0 * v2 - 1.0;
      w = v1 * v1 + v2 * v2;
	} while (w >= 1.0);
    w = sqrt(-2.0 * log(w) / w);
    ran->gauss.next = v1 * w;
    ran->gauss.second = 1;
    *x = v2 * w;
  }
  else
  {
    ran->gauss.second = 0;
    *x = ran->gauss.next;
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
  ran->gauss.second = 0;				
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
