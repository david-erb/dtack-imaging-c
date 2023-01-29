/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
  ! Copyright (C) 1998 Dtack Inc. All Rights Reserved
  ! To use this file, you must have signed a license agreement with Dtack Inc.
  ! Under no circumstances may you redistribute this file.
  ! This software is provided AS IS with no warranty expressed or implied.
  ! Dtack Inc. accepts no liability for use or misuse of this file.
  ! http://www.dtack.com  dtack@dtack.com  telephone +360.417.6700
  ! Dtack Inc, P.O. Box 3061, Port Angeles, WA USA 98362
  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */




#include <dtack/config.h>				/* platform-specific defines		*/
#include <dtack/ntype.h>
#include <dtack/stats.h>
#include <dtack/1d.h>

#define BODIES DT1D_ALL_BODIES

/*..........................................................................
 *..........................................................................*/

#define DT1D_BODY(NTYPE, DTYPE, NAME) \
static																		  \
dt1d_rc_e                                                                     \
add_ ## NAME(													              \
  const DTYPE * in1,														  \
  dt1d_i_t nin1,															  \
  const DTYPE a,															  \
  const DTYPE * in2,														  \
  dt1d_i_t nin2,															  \
  const DTYPE b,															  \
  const DTYPE c,															  \
  DTYPE * out,																  \
  dt1d_i_t nout)															  \
{																			  \
  const dt1d_i_t n = DT1D_MIN(nout,											  \
    DT1D_MIN(nin1, nin2));													  \
  dt1d_i_t i;																  \
  if (b == 0)																  \
    for (i=0; i<n; i++)														  \
      out[i] = (DTYPE)((in1[i] * a) + c);									  \
  else                                                                        \
    for (i=0; i<n; i++) 													  \
      out[i] = (DTYPE)(                                                       \
         (in1[i] * a) +  												      \
         (in2[i] * b) + c);		    								          \
  return DT1D_RC_GOOD;														  \
}

BODIES

#undef DT1D_BODY

/*..........................................................................
 *..........................................................................*/

#define DT1D_BODY(NTYPE, DTYPE, NAME) \
  case NTYPE: rc = add_ ## NAME( \
    (DTYPE *)in1, nin1, (DTYPE)a, \
    (DTYPE *)in2, nin2, (DTYPE)b, (DTYPE)c, \
    (DTYPE *)out, nout); \
  break;

/*..........................................................................
| NAME
| dt1d_add() - vector add
| 
| SUMMARY
| Implement :out[i] = (in1[i] * a) + (in2[i] * b) + c: for each element.
| Assume all vectors are of type ~ntype~.
| Convert the constants :a:, :b:, and :c: to ~ntype~ before operation.
| Have an error if ~ntype~ is not one of the allowed data types.
| Ignore elements beyond the length of the shortest vector.
| Permit space sharing among all the vectors
| 
| END
 *..........................................................................*/

dt1d_rc_e
DT1D_API
dt1d_add(								/* vector add 						*/
  dt1d_ntype_e ntype,					/* vector element type 				*/
  const void * in1,						/* first input vector 				*/
  dt1d_i_t nin1,						/* length of first input vector 	*/
  const double a,						/* first factor 					*/
  const void * in2,						/* second input vector 				*/
  dt1d_i_t nin2,						/* length of second input vector 	*/
  const double b,						/* second factor 					*/
  const double c,						/* constant offset 					*/
  void * out,							/* output vector 					*/
  dt1d_i_t nout)						/* length of output vector 			*/
{
  dt1d_rc_e rc;

  switch(ntype)
  {
    BODIES
    default: rc = dt1d_err_ntype(ntype, dt1d_ntype_list);
  }
  return rc;
}
#undef DT1D_BODY

/*..........................................................................
 *..........................................................................*/

#define DT1D_BODY(NTYPE, DTYPE, NAME) \
static																		  \
dt1d_rc_e                                                                     \
sub_ ## NAME(												              \
  const DTYPE * in1,														  \
  dt1d_i_t nin1,															  \
  const DTYPE a,															  \
  const DTYPE * in2,														  \
  dt1d_i_t nin2,															  \
  const DTYPE b,															  \
  const DTYPE c,															  \
  DTYPE * out,																  \
  dt1d_i_t nout)															  \
{																			  \
  const dt1d_i_t n = DT1D_MIN(nout,											  \
    DT1D_MIN(nin1, nin2));													  \
  dt1d_i_t i;																  \
  if (b == 0)																  \
    for (i=0; i<n; i++)														  \
      out[i] = (DTYPE)((in1[i] * a) - c);									  \
  else                                                                        \
    for (i=0; i<n; i++) 													  \
      out[i] = (DTYPE)(                                                       \
         (in1[i] * a) -  												      \
         (in2[i] * b) - c);		    								          \
  return DT1D_RC_GOOD;														  \
}

BODIES

#undef DT1D_BODY

/*..........................................................................
 *..........................................................................*/

#define DT1D_BODY(NTYPE, DTYPE, NAME) \
  case NTYPE: rc = sub_ ## NAME( \
    (DTYPE *)in1, nin1, (DTYPE)a, \
    (DTYPE *)in2, nin2, (DTYPE)b, (DTYPE)c, \
    (DTYPE *)out, nout); \
  break;

/*..........................................................................
| NAME
| dt1d_sub() - vector subtract
| 
| SUMMARY
| Implement :out[i] = (in1[i] * a) - (in2[i] * b) - c: for each element.
| Assume all vectors are of type ~ntype~.
| Convert the constants :a:, :b:, and :c: to ~ntype~ before operation.
| Have an error if ~ntype~ is not one of the allowed data types.
| Ignore elements beyond the length of the shortest vector.
| Permit space sharing among all the vectors
| 
| END
 *..........................................................................*/

dt1d_rc_e
DT1D_API
dt1d_sub(								/* vector subtract					*/
  dt1d_ntype_e ntype,					/* vector element type 				*/
  const void * in1,						/* first input vector 				*/
  dt1d_i_t nin1,						/* length of first input vector 	*/
  const double a,						/* first factor 					*/
  const void * in2,						/* second input vector 				*/
  dt1d_i_t nin2,						/* length of second input vector 	*/
  const double b,						/* second factor 					*/
  const double c,						/* constant offset 					*/
  void * out,							/* output vector 					*/
  dt1d_i_t nout)						/* length of output vector 			*/
{
  dt1d_rc_e rc;

  switch(ntype)
  {
    BODIES
    default: rc = dt1d_err_ntype(ntype, dt1d_ntype_list);
  }
  return rc;
}
#undef DT1D_BODY


/*..........................................................................
 *..........................................................................*/

#define DT1D_BODY(NTYPE, DTYPE, NAME) \
static																		  \
dt1d_rc_e                                                                     \
mult_ ## NAME(													              \
  const DTYPE * in1,														  \
  dt1d_i_t nin1,															  \
  const DTYPE a,															  \
  const DTYPE * in2,														  \
  dt1d_i_t nin2,															  \
  const DTYPE b,															  \
  const DTYPE c,															  \
  DTYPE * out,																  \
  dt1d_i_t nout)															  \
{																			  \
  const dt1d_i_t n = DT1D_MIN(nout,											  \
    DT1D_MIN(nin1, nin2));													  \
  dt1d_i_t i;																  \
  if (b == 0)																  \
    for (i=0; i<n; i++)														  \
      out[i] = (DTYPE)((in1[i] * a) * c);									  \
  else                                                                        \
    for (i=0; i<n; i++) 													  \
      out[i] = (DTYPE)(                                                       \
         (in1[i] * a) *  												      \
         (in2[i] * b) * c);		    								          \
  return DT1D_RC_GOOD;														  \
}

BODIES

#undef DT1D_BODY

/*..........................................................................
 *..........................................................................*/

#define DT1D_BODY(NTYPE, DTYPE, NAME) \
  case NTYPE: rc = mult_ ## NAME( \
    (DTYPE *)in1, nin1, (DTYPE)a, \
    (DTYPE *)in2, nin2, (DTYPE)b, (DTYPE)c, \
    (DTYPE *)out, nout); \
  break;

/*..........................................................................
| NAME
| dt1d_mult() - vector multiplication
| 
| SUMMARY
| Implement :out[i] = (in1[i] * a) * (in2[i] * b) * c: for each element.
| Assume all vectors are of type ~ntype~.
| Convert the constants :a:, :b:, and :c: to ~ntype~ before operation.
| Have an error if ~ntype~ is not one of the allowed data types.
| Ignore elements beyond the length of the shortest vector.
| Permit space sharing among all the vectors
| 
| END
 *..........................................................................*/

dt1d_rc_e
DT1D_API
dt1d_mult(								/* vector multiplication 			*/
  dt1d_ntype_e ntype,					/* vector element type 				*/
  const void * in1,						/* first input vector 				*/
  dt1d_i_t nin1,						/* length of first input vector 	*/
  const double a,						/* first factor 					*/
  const void * in2,						/* second input vector 				*/
  dt1d_i_t nin2,						/* length of second input vector 	*/
  const double b,						/* second factor 					*/
  const double c,						/* constant factor 					*/
  void * out,							/* output vector 					*/
  dt1d_i_t nout)						/* length of output vector 			*/
{
  dt1d_rc_e rc;

  switch(ntype)
  {
    BODIES
    default: rc = dt1d_err_ntype(ntype, dt1d_ntype_list);
  }
  return rc;
}
#undef DT1D_BODY


/*..........................................................................
 *..........................................................................*/

#define DT1D_BODY(NTYPE, DTYPE, NAME) \
static																		  \
dt1d_rc_e                                                                     \
div_ ## NAME(													              \
  const DTYPE * in1,														  \
  dt1d_i_t nin1,															  \
  const DTYPE a,															  \
  const DTYPE * in2,														  \
  dt1d_i_t nin2,															  \
  const DTYPE b,															  \
  const DTYPE c,															  \
  DTYPE * out,																  \
  dt1d_i_t nout)															  \
{																			  \
   const dt1d_i_t n = DT1D_MIN(nout,										  \
    DT1D_MIN(nin1, nin2));													  \
  dt1d_i_t i;																  \
  if (in2 != NULL && b != 0)                                                  \
  for (i=0; i<n; i++)														  \
    out[i] = (DTYPE)(														  \
      (in1[i] * a) /														  \
      (in2[i] * b) / c); 													  \
  else                                                                        \
  for (i=0; i<n; i++)														  \
    out[i] = (DTYPE)(														  \
      (in1[i] * a) / c);													  \
  return DT1D_RC_GOOD;														  \
}

BODIES

#undef DT1D_BODY

/*..........................................................................
 *..........................................................................*/

#define DT1D_BODY(NTYPE, DTYPE, NAME) \
  case NTYPE: rc = div_ ## NAME( \
    (DTYPE *)in1, nin1, (DTYPE)a, \
    (DTYPE *)in2, nin2, (DTYPE)b, (DTYPE)c, \
    (DTYPE *)out, nout); \
  break;

/*..........................................................................
| NAME
| dt1d_div() - vector division
| 
| SUMMARY
| Implement :out[i] = (in1[i] * a) / (in2[i] * b) / c: for each element.
| Assume all vectors are of type ~ntype~.
| Convert the constants :a:, :b:, and :c: to ~ntype~ before operation.
| Have an error if ~ntype~ is not one of the allowed data types.
| Permit space sharing among all the vectors
| 
| END
 *..........................................................................*/

dt1d_rc_e
DT1D_API
dt1d_div(								/* vector division					*/
  dt1d_ntype_e ntype,					/* vector element type 				*/
  const void * in1,						/* first input vector 				*/
  dt1d_i_t nin1,						/* length of first input vector 	*/
  const double a,						/* first factor 					*/
  const void * in2,						/* second input vector 				*/
  dt1d_i_t nin2,						/* length of second input vector 	*/
  const double b,						/* second factor 					*/
  const double c,						/* constant divisor					*/
  void * out,							/* output vector 					*/
  dt1d_i_t nout)						/* length of output vector 			*/
{
  dt1d_rc_e rc;

  switch(ntype)
  {
    BODIES
    default: rc = dt1d_err_ntype(ntype, dt1d_ntype_list);
  }
  return rc;
}
#undef DT1D_BODY





/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
  ! Copyright (C) 1998 Dtack Inc. All Rights Reserved
  ! To use this file, you must have signed a license agreement with Dtack Inc.
  ! Under no circumstances may you redistribute this file.
  ! This software is provided AS IS with no warranty expressed or implied.
  ! Dtack Inc. accepts no liability for use or misuse of this file.
  ! http://www.dtack.com  dtack@dtack.com  telephone +360.417.6700
  ! Dtack Inc, P.O. Box 3061, Port Angeles, WA USA 98362
  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */
