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
| dtimage_plot - x
| 
| DESCRIPTION
| As yet undocumented.
| 
| RETURN VALUES
| Program exit value will be DT_EXIT_GOOD on success.  
| Any other exit value indicates failure or noncompletion.
| 
| ERRORS
| As yet undocumented.
| 
| END
 *..........................................................................*/

#include <dtack/base.h>
DT_RCSID("source/high $RCSfile: plot2.c,v $ $Revision: 1.1 $");

/*..........................................................................*/
dt_rc_e 
DTCONFIG_API1
dtimage_plot_xbars_sint32(				/* bar plot, bars at each x			*/
  dt_ctl_t *ctl,
  dtimage_t *image,						/* image to draw bars in		    */
  dtntype_s4_t *bars,					/* array of bars				    */
  dtxy_t n,								/* number of bars				    */
  dtxy_t barzero,						/* where baseline of bar goes	    */
  dtxy_t barsize,						/* size of tallest bar			    */
  double color)
{
  DT_F("dtimage_plot_xbars_long");
  dtxy_t imin;							/* min data on plot				    */
  dtxy_t imax;							/* max data on plot				    */
  long bmin;
  long bmax;
  double scale;
  dtxy_t i;

  n = DT_MIN(n, image->xe);

  DT_Q_1D(dt1d_range,(DTNTYPE_B32,		/* get range of bar heights		    */
    bars, n, &imin, &imax));

  bmin = 0;								/* do absolute plotting			    */
  bmax = bars[imax];

  scale = (double)(barsize-1) /			/* scale data to fit on plot	    */
         (double)(bmax - bmin);

  for (i=0; i<n; i++)					/* for each bar					    */
  {
    dtxy_t bar = image->ye-1-(dtxy_t)(	/* bar height						*/
      (double)(bars[i]-bmin) * scale);
    DT_Q(dtimage_plot_line,(ctl,		/* draw vertical line			    */
      image, 
      i, image->ye - 1 - barzero,
      i, bar - barzero, color));
  }

  return DT_RC_GOOD;
}

/*..........................................................................*/
dt_rc_e 
DTCONFIG_API1
dtimage_plot_ybars_sint32(				/* bar plot, bars at each y			*/
  dt_ctl_t *ctl,
  dtimage_t *image,						/* image to draw bars in		    */
  dtntype_s4_t *bars,					/* array of bars				    */
  dtxy_t n,								/* number of bars				    */
  dtxy_t barzero,						/* where baseline of bar goes	    */
  dtxy_t barsize,						/* size of tallest bar			    */
  double color)
{
  DT_F("dtimage_plot_ybars_long");
  dtxy_t imin;							/* min data on plot				    */
  dtxy_t imax;							/* max data on plot				    */
  long bmin;
  long bmax;
  double scale;
  dtxy_t i;

  n = DT_MIN(n, image->ye);

  DT_Q_1D(dt1d_range,(DTNTYPE_B32,		/* get range of bar heights		    */
    bars, n, &imin, &imax));

  bmin = 0;								/* do absolute plotting			    */
  bmax = bars[imax];

  scale = (double)(barsize-1) /			/* scale data to fit on plot	    */
         (double)(bmax - bmin);

  for (i=0; i<n; i++)					/* for each bar					    */
  {
    dtxy_t bar = (dtxy_t)(				/* bar width						*/
      (double)(bars[i]-bmin) * scale);
    DT_Q(dtimage_plot_line,(ctl,		/* draw horizontal line			    */
      image, barzero, i,
      bar+barzero, i, color));
  }

  return DT_RC_GOOD;
}

/*..........................................................................*/
dt_rc_e 
DTCONFIG_API1
dtimage_plot_xbars_double(				/* bar plot, bars at each x			*/
  dt_ctl_t *ctl,
  dtimage_t *image,						/* image to draw bars in		    */
  double *bars,							/* array of bars				    */
  dtxy_t n,								/* number of bars				    */
  dtxy_t barzero,						/* where baseline of bar goes	    */
  dtxy_t barsize,						/* size of tallest bar			    */
  double color)
{
  DT_F("dtimage_plot_xbars_double");
  dtxy_t imin;							/* min data on plot				    */
  dtxy_t imax;							/* max data on plot				    */
  double bmin;
  double bmax;
  double scale;
  dtxy_t i;

  n = DT_MIN(n, image->xe);

  DT_Q_1D(dt1d_range,(DTNTYPE_F2,		/* get range of bar heights		    */
    bars, n, &imin, &imax));

  bmin = 0.0;							/* do absolute plotting			    */
  bmax = bars[imax];

  scale = (double)(barsize-1) /			/* scale data to fit on plot	    */
            (bmax - bmin);

  for (i=0; i<n; i++)					/* for each bar					    */
  {
    dtxy_t bar = image->ye-1-(dtxy_t)(	/* bar height						*/
      (bars[i]-bmin) * scale);
    DT_Q(dtimage_plot_line,(ctl,		/* draw vertical line			    */
      image, 
      i, image->ye - 1 - barzero,
      i, bar - barzero, color));
  }

  return DT_RC_GOOD;
}

/*..........................................................................*/
dt_rc_e 
DTCONFIG_API1
dtimage_plot_ybars_double(				/* bar plot, bars at each y			*/
  dt_ctl_t *ctl,
  dtimage_t *image,						/* image to draw bars in		    */
  double *bars,							/* array of bars				    */
  dtxy_t n,								/* number of bars				    */
  dtxy_t barzero,						/* where baseline of bar goes	    */
  dtxy_t barsize,						/* size of tallest bar			    */
  double color)
{
  DT_F("dtimage_plot_ybars_double");
  dtxy_t imin;							/* min data on plot				    */
  dtxy_t imax;							/* max data on plot				    */
  double bmin;
  double bmax;
  double scale;
  dtxy_t i;

  n = DT_MIN(n, image->ye);

  DT_Q_1D(dt1d_range,(DTNTYPE_F2,		/* get range of bar heights		    */
    bars, n, &imin, &imax));

  bmin = 0.0;							/* do absolute plotting			    */
  bmax = bars[imax];

  scale = (double)(barsize-1) /			/* scale data to fit on plot	    */
            (bmax - bmin);

  for (i=0; i<n; i++)					/* for each bar					    */
  {
    dtxy_t bar = (dtxy_t)(				/* bar width						*/
      (bars[i]-bmin) * scale);
    DT_Q(dtimage_plot_line,(ctl,		/* draw horizontal line			    */
      image, barzero, i,
      bar+barzero, i, color));
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
