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

#include <dtack/ntype.h>
#include <dtack/stats.h>
#include <dtack/1d.h>

#define COMPARE(A, B) (*compare)(A, B, size)
#define COPY(A, B) \
 { \
   char * const a = (char *)(A); \
   const char * const b = (char *)(B); \
   const unsigned int n = size; \
   unsigned int i; \
   for (i=0; i<n; i++) \
     a[i] = b[i]; \
 }

#define RA(I) ((void *)((char *)ra + size * (I)))

/*..........................................................................
 * Numerical Recipes in C page 247
 * must not be called with n <= 1
 *..........................................................................*/

/*..........................................................................
 *..........................................................................*/

static
void
sort_struct(
  void *ra,
  void *rra,
  unsigned int size,
  dt1d_i_t n,
  dt1d_compare_f *compare)
{
  int l, j, ir, i;
  l = (n >> 1) + 1;
  ir = n;
  for (;;)
  {
	if (l > 1)							/* still in hiring phase		    */
	{
      COPY(rra, RA(--l));
	}
	else								/* in retirement and promotion	    */
	{
	  COPY(rra, RA(ir));				/* clear a space at end of array    */
	  COPY(RA(ir), RA(1));				/* retire top of heap into it	    */
	  if (--ir == 1)					/* done with last promotion		    */
	  {
		COPY(RA(1), rra);				/* the least competent worker	    */
		return;
	  }
	}
	i = l;								/* sift rra down to proper level	*/
	j = l << 1;
	while (j <= ir)
	{
	  if (j < ir &&						/* compare to better underling	    */
         COMPARE(RA(j), RA(j+1)) < 0)
        ++j;
	  if (COMPARE(rra, RA(j)) < 0)		/* demote rra					    */
	  {
		COPY(RA(i), RA(j));
		j += (i=j);
	  }
	  else
	    j = ir + 1;						/* set j to terminate sift-down	    */
	}
	COPY(RA(i), rra);					/* put rra into its slot		    */
  }
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
  char rra[32];
  if (size > sizeof(rra))
    return DT1D_RC_BAD;
  if (nv > 1)
    sort_struct((char *)v-size, rra, size, nv, compare);
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
