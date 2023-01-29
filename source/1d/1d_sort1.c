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
 * http://www.ctc.dcu.ie/ctcweb/courses/algorithms/course/heapsort.html
 * | 980604 |  inverted sort (1000000) |  |   |      cpu |  4.49 |  0.01 |  4.57
 * | 980604 |    random sort (1000000) |  |   |      cpu |  6.40 |  0.02 |  6.62
 *..........................................................................*/

#define DT1D_BODY(NTYPE, DTYPE, NAME) 										  \
static																		  \
void 																		  \
downheap_ ## NAME(															  \
  DTYPE *a,																	  \
  dt1d_i_t n,																  \
  dt1d_i_t k)																  \
{																			  \
  dt1d_i_t i;																  \
  const DTYPE v = a[k];														  \
  while (k<=n/2)															  \
  {																			  \
	i = 2 * (k+1) - 1;														  \
    if (((i+1)<n) && (a[i]<a[i+1]))											  \
	  i++;																	  \
	if ((i>=n) || (v>=a[i]))												  \
	  break;																  \
	a[k] = a[i];															  \
	k = i;																	  \
  }																			  \
  a[k] = v;																	  \
}																			  \
																			  \
static																		  \
dt1d_rc_e																	  \
sort_ ## NAME(																  \
  DTYPE *a,																	  \
  dt1d_i_t n)																  \
{																			  \
  dt1d_i_t i;																  \
  for (i=(n/2-1); i>=0; i--)												  \
	downheap_ ## NAME(a, n, i);												  \
  while (n > 0)																  \
  {																			  \
	const DTYPE temp = a[0];												  \
	a[0] = a[n-1];															  \
	a[n-1] = temp;															  \
	downheap_ ## NAME(a, --n, 0);											  \
  }																			  \
  return DT1D_RC_GOOD;														  \
}	
	
DT1D_ALL_BODIES	

#undef DT1D_BODY

/*..........................................................................
 *..........................................................................*/

#define DT1D_BODY(NTYPE, DTYPE, NAME) \
  case NTYPE: if (nv > 1) rc = sort_ ## NAME((DTYPE *)v, nv); \
  break;

/*..........................................................................
| NAME
| dt1d_sort() - sort numerical data in place
| 
| SUMMARY
| Sort numerical data in place.
| 
| END
 *..........................................................................*/

dt1d_rc_e
DT1D_API
dt1d_sort(								/* vector in-place numerical sort	*/
  dt1d_ntype_e ntype,					/* vector element type 				*/
  void *v,
  dt1d_i_t nv)
{
  dt1d_rc_e rc = DT1D_RC_GOOD;

  switch(ntype)
  {
    DT1D_ALL_BODIES
    default: rc = dt1d_err_ntype(ntype, dt1d_ntype_list);
  }

  return rc;
}
#undef DT1D_BODY

/*..........................................................................
 *..........................................................................*/

#define DT1D_BODY(NTYPE, DTYPE, NAME) 										  \
static																		  \
void 																		  \
downheapp_ ## NAME(															  \
  DTYPE **a,																  \
  dt1d_i_t n,																  \
  dt1d_i_t k)																  \
{																			  \
  dt1d_i_t i;																  \
  DTYPE * const v = a[k];													  \
  while (k<=n/2)															  \
  {																			  \
	i = 2 * (k+1) - 1;														  \
    if (((i+1)<n) && (*a[i] < *a[i+1]))										  \
	  i++;																	  \
	if ((i>=n) || (*v >= *a[i]))											  \
	  break;																  \
	a[k] = a[i];															  \
	k = i;																	  \
  }																			  \
  a[k] = v;																	  \
}																			  \
																			  \
static																		  \
dt1d_rc_e																	  \
sortp_ ## NAME(																  \
  DTYPE **a,																  \
  dt1d_i_t n)																  \
{																			  \
  dt1d_i_t i;																  \
  for (i=(n/2-1); i>=0; i--)												  \
	downheapp_ ## NAME(a, n, i);											  \
  while (n > 0)																  \
  {																			  \
	DTYPE * const v = a[0];													  \
	a[0] = a[n-1];															  \
	a[n-1] = v;																  \
	downheapp_ ## NAME(a, --n, 0);											  \
  }																			  \
  return DT1D_RC_GOOD;														  \
}	
	
DT1D_ALL_BODIES	

#undef DT1D_BODY

/*..........................................................................
 *..........................................................................*/

#define DT1D_BODY(NTYPE, DTYPE, NAME) \
  case NTYPE: if (nv > 1) rc = sortp_ ## NAME((DTYPE **)v, nv); \
  break;

/*..........................................................................
| NAME
| dt1d_sort() - sort numerical data indices.
| 
| SUMMARY
| Sort numerical data indices.
| Data values are pointed to by list of pointers ~v~.
| Only the pointer list is modified.
| 
| END
 *..........................................................................*/

dt1d_rc_e
DT1D_API
dt1d_sortp(								/* vector index sort				*/
  dt1d_ntype_e ntype,					/* vector element type 				*/
  void **v,								/* index list 						*/
  dt1d_i_t nv)
{
  dt1d_rc_e rc = DT1D_RC_GOOD;

  switch(ntype)
  {
    DT1D_ALL_BODIES
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
