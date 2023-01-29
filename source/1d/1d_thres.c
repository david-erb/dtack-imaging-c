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

#define DT1D_BODY(NTYPE, DTYPE, NAME) 										  \
static																		  \
dt1d_rc_e                                                                     \
thresh_scalar_ ## NAME(											              \
  DTYPE * const in,															  \
  dt1d_i_t nin,																  \
  const DTYPE scalar,														  \
  const DTYPE below,					/* value for pixels below thresh	*/\
  const DTYPE above,					/* value for pixels at or above		*/\
  DTYPE * const out,					/* output vector					*/\
  dt1d_i_t nout)						/* length of output vector 			*/\
{																			  \
  const dt1d_i_t n =                                                          \
    DT1D_MIN(nin, nout);				/* limit length to minimum		    */\
  dt1d_i_t i;                                                                 \
  for (i=0; i<n; i++)                                                         \
    if (in[i] < scalar)			    /* in less than scalar?	    	*/\
      out[i] = below;														  \
    else																	  \
      out[i] = above;														  \
  return DT1D_RC_GOOD;                                                        \
}

BODIES

#undef DT1D_BODY

/*..........................................................................
 *..........................................................................*/

#define DT1D_BODY(NTYPE, DTYPE, NAME) 	\
  case NTYPE: rc = 						\
    thresh_scalar_ ## NAME( 			\
      (DTYPE *)in, nin, 				\
      (DTYPE)scalar,					\
      (DTYPE)below,						\
      (DTYPE)above,						\
      (DTYPE *)out, nout);				\
  break;

/*..........................................................................
| NAME
| dt1d_thresh() - scalar threshold
| 
| SUMMARY
| Compare each element of ~in~ to the threshold ~scalar~.
| Set the corresponding output element to ~below~ if the input is less.
| Otherwise set the output element to ~above~.
| 
| Restrict the comparison to the number of elements in the shorter
| of the two vectors.
| 
| Assume both vectors are of type ~ntype~.
| Have an error if ~ntype~ is not one of the allowed data types.
| Permit space sharing among the vectors
| 
| END
 *..........................................................................*/

dt1d_rc_e
DT1D_API
dt1d_thresh_scalar(						/* scalar threshold					*/
  dt1d_ntype_e ntype,					/* vector element type 				*/
  void *in,								/* first input vector 				*/
  dt1d_i_t nin,							/* length of first input vector 	*/
  double scalar,						/* scalar threshold					*/
  double below,							/* value for pixels below thresh	*/
  double above,							/* value for pixels at or above		*/
  void *out,							/* output vector 					*/
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

#define DT1D_BODY(NTYPE, DTYPE, NAME) 										  \
static																		  \
dt1d_rc_e                                                                     \
thresh_vector_ ## NAME(											              \
  DTYPE * const in1,														  \
  dt1d_i_t nin1,															  \
  DTYPE * const in2,														  \
  dt1d_i_t nin2,															  \
  const DTYPE below,					/* value for pixels below thresh	*/\
  const DTYPE above,					/* value for pixels at or above		*/\
  DTYPE * const out,					/* output vector					*/\
  dt1d_i_t nout)						/* length of output vector 			*/\
{																			  \
  const dt1d_i_t n =                                                          \
    DT1D_MIN(nout, 															  \
      DT1D_MIN(nin1, nin2));			/* limit length to minimum		    */\
  dt1d_i_t i;                                                                 \
  for (i=0; i<n; i++)                                                         \
    if (in1[i] < in2[i])			    /* in1 less than in2?		    	*/\
      out[i] = below;														  \
    else																	  \
      out[i] = above;														  \
  return DT1D_RC_GOOD;                                                        \
}

BODIES

#undef DT1D_BODY

/*..........................................................................
 *..........................................................................*/

#define DT1D_BODY(NTYPE, DTYPE, NAME) 	\
  case NTYPE: rc = 						\
    thresh_vector_ ## NAME( 			\
      (DTYPE *)in1, nin1, 				\
      (DTYPE *)in2, nin2, 				\
      (DTYPE)below,						\
      (DTYPE)above,						\
      (DTYPE *)out, nout);				\
  break;

/*..........................................................................
| NAME
| dt1d_thresh() - vector threshold
| 
| SUMMARY
| Compare each element of the vectors ~in1~ and ~in2~..
| Set the corresponding output element to ~below~ if the first is less.
| Otherwise set the output element to ~above~.
| 
| Restrict the comparison to the number of elements in the shorter
| of the three vectors.
| 
| Assume all vectors are of type ~ntype~.
| Have an error if ~ntype~ is not one of the allowed data types.
| Permit space sharing among all the vectors
| 
| END
 *..........................................................................*/

dt1d_rc_e
DT1D_API
dt1d_thresh_vector(						/* vector threshold					*/
  dt1d_ntype_e ntype,					/* vector element type 				*/
  void *in1,							/* first input vector 				*/
  dt1d_i_t nin1,						/* length of first input vector 	*/
  void *in2,							/* second input vector 				*/
  dt1d_i_t nin2,						/* length of second input vector 	*/
  double below,							/* value for pixels below thresh	*/
  double above,							/* value for pixels at or above		*/
  const void *out,						/* output vector 					*/
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
