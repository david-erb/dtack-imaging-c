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
DT_RCSID("source/high $RCSfile: sum_cols.c,v $ $Revision: 1.1 $");

/*..........................................................................*/
static
void
sum_cols_b01(							/* sum bits down columns		    */
  dt_ctl_t *ctl,
  dtimage_t *image,
  long *sum,							/* cleared output sum array		    */
  dtxy_t n)								/* number of cols to sum		    */
{
  dt_ntype_b01_t *d;
  dtxy_t x, y;
  long *s;
  unsigned char pixel;

  n /= 8;								/* address via pixels			    */
  for (y=0; y<image->ye; y++)
  {
    d = image->row[y].b01;				/* first group of 8 on the row	    */
    s = sum;
    for (x=0; x<n; x++)					/* for each groupof 8 on the row    */
	{
      pixel = d[x];
      s[0] += (pixel & 0x80) != 0;		/* sum into appropriate cell	    */
      s[1] += (pixel & 0x40) != 0;
      s[2] += (pixel & 0x20) != 0;
      s[3] += (pixel & 0x10) != 0;
      s[4] += (pixel & 0x08) != 0;
      s[5] += (pixel & 0x04) != 0;
      s[6] += (pixel & 0x02) != 0;
      s[7] += (pixel & 0x01) != 0;
      s += 8;							/* next output group of 8 pixels    */
	}
  }
}
/*..........................................................................*/
dt_rc_e 
DTCONFIG_API1
dtimage_sum_cols(						/* make sum of columns				*/
  dt_ctl_t *ctl,
  dtimage_t *image,
  double *sum,							/* returned sum vector			    */
  dtxy_t n)								/* max length of sum vector		    */
{
  DT_F("dtimage_sum_cols");
  dtxy_t x, y, i;
  dtntype_e ntypes[] = {
	DTNTYPE_B01,
	DTNTYPE_B08,
	DTNTYPE_B16,
	DTNTYPE_B32,
	DTNTYPE_S1,
	DTNTYPE_S2,
	DTNTYPE_S4,
	DTNTYPE_F1,
	DTNTYPE_F2,
	DTNTYPE_NULL
  };
  long *lsum = NULL;
  const char *temp = "temp sum array";
  const dtxy_t ye = image->ye;
  dt_rc_e rc;

  rc = dtimage_check_1(ctl,				/* check simple stuff				*/
    image, "image",
    ntypes, F);
  if (rc != DT_RC_GOOD)
    return rc;

  n = DT_MIN(n, image->xe);				/* limit to min size			    */

  if (DTNTYPE_ISFLOAT(image->ntype))
	DT_MEMSET(sum, '\0',					/* clear sum array				    */
      n * sizeof(*sum));
  else
  {
    DT_Q(dtos_malloc2,(ctl,				/* space for temp long sum array    */
      (void **)&lsum, n*sizeof(*lsum),
      F, temp));
	DT_MEMSET(lsum, '\0',					/* clear sum array				    */
      n * sizeof(*lsum));
  }


#define LCASE(E, T) 					\
    case E:								\
	{									\
      for (y=0; y<ye; y++)				\
      {									\
		const T * const d = (T *)		\
          image->row[y].dvoid;			\
        for (x=0; x<n; x++)				\
          lsum[x] += d[x];				\
      }									\
	}									\
    break;

#define FCASE(E, T) 					\
    case E:								\
	{									\
      for (y=0; y<ye; y++)				\
      {									\
		const T * const d = (T *)		\
          image->row[y].dvoid;			\
        for (x=0; x<n; x++)				\
          sum[x] += d[x];				\
      }									\
	}									\
    break;

  switch(image->ntype)
  {
    case DT_NTYPE_B01:					/* bit image					    */
      sum_cols_b01(ctl,
        image, lsum, n);
    break;

	LCASE(DTNTYPE_B08, dtntype_b08_t)		\
	LCASE(DTNTYPE_B16, dtntype_b16_t)		\
	LCASE(DTNTYPE_B32, dtntype_b32_t)		\
    LCASE(DTNTYPE_S1,  dtntype_s1_t)		\
    LCASE(DTNTYPE_S2,  dtntype_s2_t)		\
    LCASE(DTNTYPE_S4,  dtntype_s4_t)		\
    FCASE(DTNTYPE_F1,  dtntype_f1_t)		\
    FCASE(DTNTYPE_F2,  dtntype_f2_t)
  }

  if (lsum != NULL)						/* had we allocated temp space?	    */
  {
    for (i=0; i<n; i++)
      sum[i] = lsum[i];
    DT_Q(dtos_free2,(ctl,
      lsum, F, temp));
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
