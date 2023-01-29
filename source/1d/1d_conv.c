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
  19980716 DE remove sunpro1 compiler warnings
 *..........................................................................*/


/*..........................................................................
| NAME
| Introduction - intro
| 
| DESCRIPTION
| As yet undocumented.
| 
| RETURN VALUES
| Function return value will be DT1D_RC_GOOD on success.  
| Any other return value indicates failure or noncompletion.
| When DT1D_RC_GOOD is not returned, any other return values may not be valid.
| 
| ERRORS
| As yet undocumented.
| 
| END
 *..........................................................................*/

#include <dtack/config.h>				/* platform-specific defines		*/
#include <dtack/ntype.h>
#include <dtack/stats.h>
#include <dtack/1d.h>

/*..........................................................................
 * uchar vector convolution
 *..........................................................................*/

#define BODY(TYPE, FUNC, ACCTYPE)											  \
static																		  \
dt1d_rc_e																	  \
FUNC(																		  \
  TYPE *in,																	  \
  dt1d_i_t nin,																  \
  TYPE *kernel,																  \
  dt1d_i_t kernel_size,														  \
  TYPE *out,																  \
  dt1d_i_t nout)															  \
{																			  \
  int db, de;																  \
  int kb, ke;																  \
  dt1d_i_t i, n;															  \
  n = DT1D_MIN(nin, nout+kernel_size);	/* limit length to minimum		    */\
  db = kernel_size / 2;					/* skip left edge effect		    */\
  de = n - (kernel_size - db - 1);		/* skip right edge effect		    */\
  kb = -db;								/* new kernel bounds			    */\
  ke = kernel_size + kb;													  \
  kernel -= kb;							/* zero-center kernel vector	    */\
  memset((void *)out, 0, -kb*sizeof(TYPE));	/* clear left edge effect			*/\
  out -= kb;							/* skip left edge effect 			*/\
  for (i=db; i<de; i++)					/* loop over vector				    */\
  {																			  \
    ACCTYPE a = 0.0;					/* accumulator					    */\
    TYPE *d = in + i;														  \
	int j;																	  \
	for (j=kb; j<ke; j++)				/* loop over kernel size		    */\
      a += d[j] * kernel[j];												  \
	*out++ = a;																  \
  }   																		  \
  memset((void *)out, 0, (ke-1)*sizeof(TYPE)); /* clear right edge effect			*/\
  return DT1D_RC_GOOD;														  \
}

    BODY(dt1d_ntype_b08_t, convolve_uchar,  dtntype_b32_t)
    BODY(dt1d_ntype_b16_t, convolve_ushort, dtntype_b32_t)
    BODY(dt1d_ntype_b32_t, convolve_uint32, dtntype_b32_t)
    BODY(dt1d_ntype_s1_t,  convolve_char,   dtntype_s4_t)
    BODY(dt1d_ntype_s2_t,  convolve_short,  dtntype_s4_t)
    BODY(dt1d_ntype_s4_t,  convolve_sint32, dtntype_s4_t)
    BODY(dt1d_ntype_f1_t,  convolve_float,  double)
    BODY(dt1d_ntype_f2_t,  convolve_double, double)

/*..........................................................................
 * any scalar sum
 *..........................................................................*/

dt1d_rc_e
DT1D_API
dt1d_convolve(							/* vector division					*/
  dt1d_ntype_e ntype,					/* vector element type 				*/
  void * in1,							/* first input vector 				*/
  dt1d_i_t nin1,						/* length of first input vector 	*/
  void * in2,							/* second input vector 				*/
  dt1d_i_t nin2,						/* length of second input vector 	*/
  void * out,							/* output vector 					*/
  dt1d_i_t nout)						/* length of output vector 			*/
{
# define CASE(NTYPE, FUNC, TYPE) \
  case NTYPE: rc = FUNC(				\
    (TYPE *)in1, nin1, 					\
    (TYPE *)in2, nin2, 					\
    (TYPE *)out, nout); 				\
  break;

  dt1d_rc_e rc;
  switch(ntype)
  {
    CASE(DT1D_NTYPE_B08, convolve_uchar,  unsigned char);
    CASE(DT1D_NTYPE_B16, convolve_ushort, unsigned short);
    CASE(DT1D_NTYPE_B32, convolve_uint32, dtntype_b32_t);
    CASE(DT1D_NTYPE_S1,  convolve_char,   signed char);
    CASE(DT1D_NTYPE_S2,  convolve_short,  short);
    CASE(DT1D_NTYPE_S4,  convolve_sint32, dtntype_s4_t);
    CASE(DT1D_NTYPE_F1,  convolve_float,  float);
    CASE(DT1D_NTYPE_F2,  convolve_double, double);
    default: rc = dt1d_err_ntype(ntype, dt1d_ntype_list);
  }
  return rc;
# undef CASE
}


/*..........................................................................
 * vector convolution
 *..........................................................................*/

#define BODY(TYPE, FUNC, ACCTYPE)											  \
static						 \
dt1d_rc_e					 \
FUNC(			             \
  TYPE *in,					 \
  dt1d_i_t nin,				 \
  dt1d_i_t instride,         \
  dt1d_i_t kernel_size,		 \
  TYPE *out,				 \
  dt1d_i_t nout,			 \
  dt1d_i_t outstride)        \
{						     \
  int i, n; \
  int kernel_instride = kernel_size * instride; \
  ACCTYPE a = 0; \
  n = kernel_size / 2; \
  for (i=0; i<n; i++) \
  { \
    a += *in; \
	in += instride; \
	*out = 0; \
	out += outstride; \
  } \
 \
  for ( ; i<kernel_size; i++) \
  { \
    a += *in; \
	in += instride; \
  } \
 \
  n = DT1D_MIN(nin, nout); \
  for ( ; i<n; i++) \
  { \
    *out = (a += *in - *(in-kernel_instride)); \
	in += instride; \
	out += outstride; \
  } \
  for ( ; i<nout; i++) \
  { \
	*out = 0; \
	out += outstride; \
  } \
  return DT1D_RC_GOOD;		 \
}
 
    BODY(dt1d_ntype_b08_t, boxcarr_uchar,  dtntype_b32_t)
    BODY(dt1d_ntype_b16_t, boxcarr_ushort, dtntype_b32_t)
    BODY(dt1d_ntype_b32_t, boxcarr_uint32, dtntype_b32_t)
    BODY(dt1d_ntype_s1_t,  boxcarr_char,   dtntype_s4_t)
    BODY(dt1d_ntype_s2_t,  boxcarr_short,  dtntype_s4_t)
    BODY(dt1d_ntype_s4_t,  boxcarr_sint32, dtntype_s4_t)
    BODY(dt1d_ntype_f1_t,  boxcarr_float,  double)
    BODY(dt1d_ntype_f2_t,  boxcarr_double, double)

#undef BODY

/*..........................................................................
 * any scalar sum
 *..........................................................................*/

dt1d_rc_e
DT1D_API
dt1d_boxcarr(							/* vector division					*/
  dt1d_ntype_e ntype,					/* vector element type 				*/
  void *in,
  dt1d_i_t nin,
  dt1d_i_t instride, 
  dt1d_i_t kernel_size,
  void *out,
  dt1d_i_t nout,	
  dt1d_i_t outstride)
{
# define CASE(NTYPE, FUNC, TYPE) \
  case NTYPE: rc = FUNC(				\
    (TYPE *)in, nin, instride,			\
    kernel_size,                        \
    (TYPE *)out, nout, outstride);		\
  break;

  dt1d_rc_e rc;
  switch(ntype)
  {
    CASE(DT1D_NTYPE_B08, boxcarr_uchar,  unsigned char);
    CASE(DT1D_NTYPE_B16, boxcarr_ushort, unsigned short);
    CASE(DT1D_NTYPE_B32, boxcarr_uint32, dtntype_b32_t);
    CASE(DT1D_NTYPE_S1,  boxcarr_char,   signed char);
    CASE(DT1D_NTYPE_S2,  boxcarr_short,  short);
    CASE(DT1D_NTYPE_S4,  boxcarr_sint32, dtntype_s4_t);
    CASE(DT1D_NTYPE_F1,  boxcarr_float,  float);
    CASE(DT1D_NTYPE_F2,  boxcarr_double, double);
    default: rc = dt1d_err_ntype(ntype, dt1d_ntype_list);
  }
  return rc;
# undef CASE
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
