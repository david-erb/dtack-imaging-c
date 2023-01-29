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
  19990124 DE workaround for Visual C++ optimizer bug
 *..........................................................................*/




#include <dtack/config.h>				/* platform-specific defines		*/
#include <dtack/ntype.h>
#include <dtack/stats.h>
#include <dtack/1d.h>

#define BODIES DT1D_ALL_BODIES

										/* force the variable X into 		*/
										/* an arg list to get around a 		*/
										/* a  VC++ 6.0 optimizer bugcommon */
static const int one = 1;
static const int zero = 0;
static void nothing(double x) { }
#define NOTHING(X) if (one == zero) nothing(X)

/*..........................................................................
 *..........................................................................*/

#define DT1D_BODY(NTYPE, DTYPE, NAME) 										  \
static																		  \
dt1d_rc_e                                                                     \
cmp_ ## NAME(												                  \
  const DTYPE * in1,														  \
  dt1d_i_t nin1,															  \
  const DTYPE * in2,														  \
  dt1d_i_t nin2,															  \
  DTYPE epsilon,		    												  \
  long *cmp)																  \
{																			  \
  const dt1d_i_t n =                                                          \
    DT1D_MIN(nin1, nin2);				/* limit length to minimum		    */\
  dt1d_i_t i;                                                                 \
  *cmp = 0;                                                                   \
  if (epsilon == 0)                                                           \
  {                                                                           \
    for (i=0; i<n; i++)                                                       \
      if (in1[i] < in2[i])			    /* in1 less than in2?		    	*/\
      {                                                                       \
        *cmp = -(long)(i + 1);			/* return negative index		    */\
        break;                                                                \
	  }                                                                       \
      else                                                                    \
      if (in1[i] > in2[i])				/* in1 greater than in2?			*/\
      {                                                                       \
        *cmp = i + 1;					/* return positive index			*/\
        break;                                                                \
	  }                                                                       \
  }                                                                           \
  else                                                                        \
  {                                                                           \
    for (i=0; i<n; i++)                                                       \
    {                                                                         \
      double delta = in2[i] - in1[i];                                         \
      if (delta > epsilon)				/* in1 less than in2?			    */\
      {                                                                       \
        NOTHING(delta);														  \
        *cmp = -(long)(i + 1);			/* return negative index		    */\
        break;                                                                \
	  }                                                                       \
      else                                                                    \
      if (delta < -epsilon)				/* in1 greater than in2?			*/\
      {                                                                       \
        NOTHING(delta);														  \
        *cmp = i + 1;					/* return positive index			*/\
        break;                                                                \
	  }                                                                       \
	}                                                                         \
  }                                                                           \
  return DT1D_RC_GOOD;                                                        \
}

BODIES

#undef DT1D_BODY

/*..........................................................................
 *..........................................................................*/

#define DT1D_BODY(NTYPE, DTYPE, NAME) \
  case NTYPE: rc = cmp_ ## NAME( \
    (DTYPE *)in1, nin1, \
    (DTYPE *)in2, nin2, (DTYPE)epsilon, cmp); \
  break;

/*..........................................................................
| NAME
| dt1d_cmp() - vector compare
| 
| SUMMARY
| Compare each element of the vectors.
| Return 0 if no elements differ by more than ~epsilon~.
| Otherwise return the location of the difference.
| A positive number indicates the first vector element is greater,
| and negative number indicates it is less.
| The absolute value of the number is one greater than the index
| of the first differing element.
| For example, ~*cmp~ of 1 means the first element differed.
| 
| Restrict the comparison to the number of elements in the shorter
| of the two vectors.
| 
| Assume all vectors are of type ~ntype~.
| Have an error if ~ntype~ is not one of the allowed data types.
| Permit space sharing among all the vectors
|
| DESCRIPTION
| The philosophy behind ignoring excess elements is that
| the caller can easily tell if the lengths are different
| and take action if this is an error.
| The logic to determine if a returned difference is due to
| a vector length difference alone is much more complex
| and less intuitive.
| 
| END
 *..........................................................................*/

dt1d_rc_e
DT1D_API
dt1d_cmp(								/* vector compare					*/
  dt1d_ntype_e ntype,					/* vector element type 				*/
  const void * in1,						/* first input vector 				*/
  dt1d_i_t nin1,						/* length of first input vector 	*/
  const void * in2,						/* second input vector 				*/
  dt1d_i_t nin2,						/* length of second input vector 	*/
  const double epsilon,					/* maximum allowable difference		*/
  long *cmp)							/* returned difference location 	*/
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
