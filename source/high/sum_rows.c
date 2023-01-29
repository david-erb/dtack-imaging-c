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
| Function return value will be DT_RC_GOOD on success.  
| Any other return value indicates failure or noncompletion.
| When DT_RC_GOOD is not returned, any other return values may not be valid.
| 
| ERRORS
| As yet undocumented.
| 
| END
 *..........................................................................*/

#include <dtack/base.h>
#include <dtack/os.h>
DT_RCSID("source/high $RCSfile: sum_rows.c,v $ $Revision: 1.1 $");


/*0,1,2,3,4,5,6,7,8,9,a,b,c,d,e,f         */

static int bitsum[256] = {				/* sum of bits in a byte		    */
  0,1,1,2,1,2,2,3,1,2,2,3,2,3,3,4,   /* 0 */
  1,2,2,3,2,3,3,4,2,3,3,4,3,4,4,5,   /* 1 */
  1,2,2,3,2,3,3,4,2,3,3,4,3,4,4,5,   /* 2 */
  2,3,3,4,3,4,4,5,3,4,4,5,4,5,5,6,   /* 3 */
  1,2,2,3,2,3,3,4,2,3,3,4,3,4,4,5,   /* 4 */
  2,3,3,4,3,4,4,5,3,4,4,5,4,5,5,6,   /* 5 */
  2,3,3,4,3,4,4,5,3,4,4,5,4,5,5,6,   /* 6 */
  3,4,4,5,4,5,5,6,4,5,5,6,5,6,6,7,   /* 7 */
  1,2,2,3,2,3,3,4,2,3,3,4,3,4,4,5,   /* 8 */
  2,3,3,4,3,4,4,5,3,4,4,5,4,5,5,6,   /* 9 */
  2,3,3,4,3,4,4,5,3,4,4,5,4,5,5,6,   /* a */
  3,4,4,5,4,5,5,6,4,5,5,6,5,6,6,7,   /* 7 */
  2,3,3,4,3,4,4,5,3,4,4,5,4,5,5,6,   /* c */
  3,4,4,5,4,5,5,6,4,5,5,6,5,6,6,7,   /* d */
  3,4,4,5,4,5,5,6,4,5,5,6,5,6,6,7,   /* e */
  4,5,5,6,5,6,6,7,5,6,6,7,6,7,7,8    /* f */
};

/*..........................................................................*/
static
void
sum_rows_b01(							/* sum bits along rows			    */
  dt_ctl_t *ctl,
  dtimage_t *image,
  long *sum,							/* cleared output sum array		    */
  dtxy_t n)								/* number of rows to sum		    */
{
  dtxy_t x, y;
  dt_ntype_b01_t *d;
  long s;
  dtxy_t xe = image->xe / 8;			/* address via pixels			    */

  for (y=0; y<n; y++)
  {
    d = image->row[y].b01;				/* start of the row				    */
    s = 0;
    for (x=0; x<xe; x++)
      s += bitsum[d[x]];
    sum[y] = s;
  }
}
/*..........................................................................*/
dt_rc_e 
DTCONFIG_API1
dtimage_sum_rows(						/* make sum of each row				*/
  dt_ctl_t *ctl,
  dtimage_t *image,
  double *sum,							/* returned sum vector			    */
  dtxy_t n)								/* max length of sum vector		    */
{
  DT_F("dtimage_sum_rows");
  dtxy_t x, y;
  dt_rc_e rc;

  rc = dtimage_check_1(ctl,				/* check simple stuff				*/
    image, "image",
    NULL, F);
  if (rc != DT_RC_GOOD)
    return rc;

  n = DT_MIN(n, image->ye);				/* limit to min size			    */

  switch(image->ntype)
  {
    case DT_NTYPE_B01:					/* bit image					    */
	{
      long *lsum = NULL;
      dtxy_t i;
      const char *temp =
        "temp sum array";
      DT_Q(dtos_malloc2,(ctl,			/* space for temp long sum array    */
        (void **)&lsum,
        n*sizeof(*lsum),
        F, temp));
      DT_MEMSET(lsum, '\0',				/* clear sum array				    */
        n * sizeof(*lsum));
      sum_rows_b01(ctl,
        image, lsum, n);
      for (i=0; i<n; i++)
        sum[i] = lsum[i];
      DT_Q(dtos_free2,(ctl,
        lsum, F, temp));
	}
    break;
										/* ................................ */
    default:
	{
      for (y=0; y<n; y++)
      {
        DT_Q_1D(dt1d_sum,(image->ntype,
          image->row[y].dvoid,
          image->xe,
          &sum[y]))
      }
	}
  }

  return DT_RC_GOOD;
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
