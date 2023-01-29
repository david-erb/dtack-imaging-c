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
diff_ ## NAME(												                  \
  const DTYPE * in1,														  \
  dt1d_i_t nin1,															  \
  const DTYPE * in2,														  \
  dt1d_i_t nin2,															  \
  DTYPE epsilon,		    												  \
  long *nless,																  \
  long *nmore)																  \
{																			  \
  const dt1d_i_t n =                                                          \
    DT1D_MIN(nin1, nin2);				/* limit length to minimum		    */\
  dt1d_i_t i;                                                                 \
  long less = 0;                                                              \
  long more = 0;                                                              \
  if (epsilon == 0)                                                           \
  {                                                                           \
    for (i=0; i<n; i++)                                                       \
      if (in1[i] < in2[i])                                                    \
        less++;                                                               \
      else																	  \
      if (in1[i] > in2[i])                                                    \
        more++;                                                               \
  }                                                                           \
  else                                                                        \
  {                                                                           \
    for (i=0; i<n; i++)                                                       \
      if (in1[i] < in2[i] &&                                                  \
          in2[i] - in1[i] > epsilon)										  \
        less++;                                                               \
      else																	  \
      if (in1[i] > in2[i] &&                                                  \
          in1[i] - in2[i] > epsilon)										  \
        more++;                                                               \
  }                                                                           \
  *nless = less;                                                              \
  *nmore = more;                                                              \
  return DT1D_RC_GOOD;                                                        \
}

BODIES

#undef DT1D_BODY

/*..........................................................................
 *..........................................................................*/

#define DT1D_BODY(NTYPE, DTYPE, NAME) \
  case NTYPE: rc = diff_ ## NAME( \
    (DTYPE *)in1, nin1, \
    (DTYPE *)in2, nin2, (DTYPE)epsilon, nless, nmore); \
  break;

/*..........................................................................
| NAME
| dt1d_diff() - count vector differences
| 
| SUMMARY
| Compare each element of the vectors.
| Return the number of elements which differ by more than ~epsilon~.
| 
| All elements in the longer vector beyond the length of the shorter vector
| are ignored.
| 
| Assume all vectors are of type ~ntype~.
| Have an error if ~ntype~ is not one of the allowed data types.
| Permit space sharing among all the vectors
| 
| END
 *..........................................................................*/

dt1d_rc_e
DT1D_API
dt1d_diff(								/* vector difference count			*/
  dt1d_ntype_e ntype,					/* vector element type 				*/
  const void * in1,						/* first input vector 				*/
  dt1d_i_t nin1,						/* length of first input vector 	*/
  const void * in2,						/* second input vector 				*/
  dt1d_i_t nin2,						/* length of second input vector 	*/
  const double epsilon,					/* maximum allowable difference		*/
  long *nless,							/* returned count in1 less than in2	*/
  long *nmore)							/* returned count in1 more than in2 */
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
