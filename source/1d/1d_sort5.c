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
  19990809 DE added sort func with arg
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

#define A(I) ((void *)((char *)a + size * (I)))

/*..........................................................................
 * http://www.ctc.dcu.ie/ctcweb/courses/algorithms/course/heapsort.html
 *..........................................................................*/

static	
void 	
downheap(		
  void *a,		
  int size,
  dt1d_i_t n,	
  dt1d_i_t k,
  dt1d_compare_f *compare,
  void *temp)
{				
  dt1d_i_t i;	
  memcpy(temp, A(k), size);
  while (k<=n/2)		
  {						
	i = 2 * (k+1) - 1;	
    if (((i+1)<n) && (compare(A(i), A(i+1), size)<0))
	  i++;
	if ((i>=n) || (compare(temp, A(i), size)>=0))
	  break;	
    memcpy(A(k), A(i), size);			
	k = i;					
  }							
  memcpy(A(k), temp, size);			
}				
			
/*..........................................................................
 *..........................................................................*/
							
static						
dt1d_rc_e					
sort(						
  void *a,	
  int size,				
  dt1d_i_t n,
  dt1d_compare_f *compare,
  void *temp)
{							
  dt1d_i_t i;				
  for (i=(n/2-1); i>=0; i--)
	downheap(a, size, n, i, compare, temp);
  while (n > 0)					
  {		
    memcpy(temp, A(0), size);
	memcpy(A(0), A(n-1), size);
    memcpy(A(n-1), temp, size);
	downheap(a, size, --n, 0, compare, temp);
  }									
  return DT1D_RC_GOOD;	
}	

/*..........................................................................
 * struct heap sort
 *..........................................................................*/

dt1d_rc_e
DT1D_API
dt1d_sort_struct(
  void *v,
  unsigned int size,
  dt1d_i_t nv,
  dt1d_compare_f *compare)
{
  char temp[256];
  if (size > sizeof(temp))
    return DT1D_RC_BAD;
  if (nv > 1)
    sort(v, size, nv, compare, temp);
  return DT1D_RC_GOOD;
}


/*..........................................................................
 *..........................................................................*/

static	
void 	
downheap2(		
  void *a,		
  int size,
  dt1d_i_t n,	
  dt1d_i_t k,
  dt1d_compare2_f *compare,
  void *arg,							/* passthrough to compare func 		*/
  int ascending,
  void *temp)
{				
  dt1d_i_t i;	
  memcpy(temp, A(k), size);
  while (k<=n/2)		
  {						
	i = 2 * (k+1) - 1;	
    if (((i+1)<n) && (ascending*compare(arg, A(i), A(i+1), size)<0))
	  i++;
	if ((i>=n) || (ascending*compare(arg, temp, A(i), size)>=0))
	  break;	
    memcpy(A(k), A(i), size);			
	k = i;					
  }							
  memcpy(A(k), temp, size);			
}				
			
/*..........................................................................
 *..........................................................................*/
							
static						
dt1d_rc_e					
sort2(						
  void *a,	
  int size,				
  dt1d_i_t n,
  dt1d_compare2_f *compare,
  void *arg,							/* passthrough to compare func 		*/
  int ascending,
  void *temp)
{							
  dt1d_i_t i;				
  for (i=(n/2-1); i>=0; i--)
	downheap2(a, size, n, i, compare, arg, ascending, temp);
  while (n > 0)					
  {		
    memcpy(temp, A(0), size);
	memcpy(A(0), A(n-1), size);
    memcpy(A(n-1), temp, size);
	downheap2(a, size, --n, 0, compare, arg, ascending, temp);
  }									
  return DT1D_RC_GOOD;	
}	

/*..........................................................................
 * struct heap sort
 *..........................................................................*/

dt1d_rc_e
DT1D_API
dt1d_sort_struct2(
  void *v,
  unsigned int size,
  dt1d_i_t nv,
  dt1d_compare2_f *compare,
  void *arg,							/* passthrough to compare func 		*/
  int ascending)
{
  char temp[2048];
  if (size > sizeof(temp))
    return DT1D_RC_BAD;
  if (ascending <= 0)
	ascending = -1;
  else
	ascending = 1;

  if (nv > 1)
    sort2(v, size, nv, compare, arg, ascending, temp);
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
