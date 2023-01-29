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
DT_RCSID("source/high $RCSfile: hist2.c,v $ $Revision: 1.1 $");

/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtimage_hist_plot_horiz(				/* plot histogram 					*/
  dt_ctl_t *ctl,
  dtimage_t *image,						/* output image						*/
  dthist_t *hist,						/* input histogram					*/
  dtxy_t y0,							/* baseline of histogram 			*/
  dtxy_t ye,							/* height of tallest bar 			*/
  double color)
{
  DT_F("dtimage_hist_plot_horiz");
  dthist_freq2_t *freq =
    (dthist_freq2_t *)hist->freq;
  dtxy_t imin;							/* min data on plot				    */
  dtxy_t imax;							/* max data on plot				    */
  dthist_freq2_t bmin;
  dthist_freq2_t bmax;
  double scale;
  dtxy_t i, n;
  dt_rc_e rc;

  rc = dtimage_check_1f(ctl,			/* check simple stuff				*/
    image, "image",
    NULL, DTIMAGE_FLAG_PAGED, F);
  if (rc != DT_RC_GOOD)
    return rc;

  if (hist->ntype !=					/* not default ntype? 				*/
	  DTHIST_FREQ2_NTYPE)
  {
	char s[256];
	dtntype_to_string(ctl, hist->ntype,
      s, sizeof(s));
	return dt_err(ctl, F,
	  "ntype \"%s\" not supported", s);
  }

  n = DT_MIN(hist->width, 
    (unsigned long)image->xe);

  DT_Q_1D(dt1d_range,(hist->ntype,		/* get range of bar heights		    */
    hist->freq, n,
    &imin, &imax));

  bmin = 0;								/* do absolute plotting			    */
  bmax = freq[imax];

  scale = (double)(ye-1) /				/* scale data to fit on plot	    	*/
         (double)(bmax - bmin);

  for (i=0; i<n; i++)					/* for each bar					    */
  {
    dtxy_t bar = image->ye-1-(dtxy_t)(	/* bar height						*/
      (double)(freq[i]-bmin) *
       scale);
    DT_Q(dtimage_plot_line,(ctl,		/* draw vertical line			    */
      image, 
      i, image->ye - 1 - y0,
      i, bar - y0, color));
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
