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
DT_RCSID("src/high $RCSfile: run_len.c,v $ $Revision: 1.1 $");

#include <dtack/os.h>
#include <dtack/mem.h>

#ifdef NONO
/*0,1,2,3,4,5,6,7,8,9,a,b,c,d,e,f         */

static int bitrun[256] = {				/* run of bits in a byte		    */
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
#endif

/*..........................................................................*/
dt_rc_e 
DTCONFIG_API1
dtrun_col(								/* find runs in xth column			*/
  dt_ctl_t *ctl,
  dtimage_t *image,
  const dtxy_t x,
  dtrun_len_t *run,						/* returned run vector			    */
  dtxy_t n)								/* max length of run vector		    */
{
  DT_F("dtrun_col");
  int incr, t;
  dtrun_len_t count;
  dtxy_t i, y;
#ifdef DTCONFIG_16BIT
  const dtrun_len_t max = 0x7fff;		/* dtrun_len_t limits maximum count */
#else
  const dtrun_len_t max = 0x7fffffffL;
#endif
  double value;

  DT_Q(dtimage_get_pixel,(ctl,			/* get value at top of column	    */
    image, x, 0, &value));
  incr = value? 1: -1;					/* initial count polarity		    */
  count = incr;
  i = 1;
    
  for (y=1; y<image->ye; y++)			/* for each row in image		    */
  {
    DTIMAGE_GETPIX1(DT_Q, ctl,			/* get next value down column		*/
      image, x, y, &value);
    t = value? 1: -1;;
    if (t != incr)						/* state transition?			    */
	{
      if (i == n-1)						/* last possible transition?	    */
        break;
      run[i++] = count;					/* record run count				    */
      incr = t;
      count = incr;
    }
    else
	{
	  if (count == max ||
		  count == -max)
	    return dt_err(ctl, F,
          "run too long, max is %d",
          max);
      count += incr;
	}
  }


  run[i] = count;						/* last run's count				    */
  run[0] = i;							/* number of runs we had		    */

  return DT_RC_GOOD;
}

/*..........................................................................*/
dt_rc_e 
DTCONFIG_API1
dtrun_row(								/* find runs in yth row				*/
  dt_ctl_t *ctl,
  dtimage_t *image,
  const dtxy_t y,
  dtrun_len_t *run,						/* returned run vector			    */
  dtxy_t n)								/* max length of run vector		    */
{
  DT_F("dtrun_row");
  int incr, t;
  dtrun_len_t count;
  dtxy_t i, x;
#ifdef DTCONFIG_16BIT
  const dtrun_len_t max = 0x7fff;		/* dtrun_len_t limits maximum count */
#else
  const dtrun_len_t max = 0x7fffffffL;
#endif
  double value;

  DT_Q(dtimage_get_pixel,(ctl,			/* get value at left of row			*/
    image, 0, y, &value));
  incr = value? 1: -1;					/* initial count polarity		    */
  count = incr;
  i = 1;
    
  for (x=1; x<image->xe; x++)			/* for each column in image		    */
  {
    DTIMAGE_GETPIX1(DT_Q, ctl,			/* get next value along row			*/
      image, x, y, &value);
    t = value? 1: -1;;
    if (t != incr)						/* state transition?			    */
	{
      if (i == n-1)						/* last possible transition?	    */
        break;
      run[i++] = count;					/* record run count				    */
      incr = t;
      count = incr;
    }
    else
	{
	  if (count == max ||
		  count == -max)
	    return dt_err(ctl, F,
          "run too long, max is %d",
          max);
      count += incr;
	}
  }

  run[i] = count;						/* last run's count				    */
  run[0] = i;							/* number of runs we had		    */

  return DT_RC_GOOD;
}


/*..........................................................................*/
dt_rc_e 
DTCONFIG_API1
dtrun_cols_max(							/* find max run in each column		*/
  dt_ctl_t *ctl,
  dtimage_t *image,
  dtrun_len_t *maxrun,					/* returned run vector			    */
  dtxy_t n)								/* max length of run vector		    */
{
  DT_F("dtrun_cols");
  dtrun_len_t *run;
  dtxy_t m;
  dtxy_t x;
  dt1d_i_t imin, imax;
  static char *temp = "temp run array";
  dt_rc_e rc = DT_RC_GOOD;

  m = image->ye + 1;					/* space for temp run array		    */
  DT_Q(dtos_malloc2,(ctl,
    (void **)&run, 
    m * sizeof(*run),
    F, temp));

  n = DT_MIN(n, image->xe);				/* limit output to given space	    */

  for (x=0; x<n; x++)
  {
	DT_C(dtrun_col,(ctl,				/* find runs in column			    */
      image, x, run, m));
    DT_C_1D(dt1d_range,(				/* find max run for this column	    */
      DTRUN_LEN_NTYPE,
      run+1, run[0], &imin, &imax));
    maxrun[x] = run[imax+1];
    if (maxrun[x] < 0)					/* there are no "on" runs?		    */
      maxrun[x] = 0;
  }

cleanup:
  DT_I(dtos_free2,(ctl, run, F, temp));	/* free temp run array			    */

  return rc;
}

/*..........................................................................*/
dt_rc_e 
DTCONFIG_API1
dtrun_rows_max(							/* find max run in each row			*/
  dt_ctl_t *ctl,
  dtimage_t *image,
  dtrun_len_t *maxrun,					/* returned run vector			    */
  dtxy_t n)								/* max length of run vector		    */
{
  DT_F("dtrun_rows");
  dtrun_len_t *run;
  dtxy_t m;
  dtxy_t y;
  dt1d_i_t imin, imax;
  static char *temp = "temp run array";
  dt_rc_e rc = DT_RC_GOOD;

  m = image->xe + 1;					/* space for temp run array		    */
  DT_Q(dtos_malloc2,(ctl,
    (void **)&run, 
    m * sizeof(*run),
    F, temp));

  n = DT_MIN(n, image->ye);				/* limit output to given space	    */

  for (y=0; y<n; y++)
  {
	DT_C(dtrun_row,(ctl,				/* find runs in row				    */
      image, y, run, m));
    DT_C_1D(dt1d_range,(				/* find max run for this row	    */
      DTRUN_LEN_NTYPE,
      run+1, run[0], &imin, &imax));
    maxrun[y] = run[imax+1];
    if (maxrun[y] < 0)					/* there are no "on" runs?		    */
      maxrun[y] = 0;
  }

cleanup:
  DT_I(dtos_free2,(ctl, run, F, temp));	/* free temp run array			    */

  return rc;
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
