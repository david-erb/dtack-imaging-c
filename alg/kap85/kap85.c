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

static char rcsid[] = "lib $RCSfile: kap85.c,v $ $Revision: 1.1 $";

#include <dtack/base.h>
#include <dtack/os.h>
#include <dtack/1d.h>
#include <dt_kap85.h>

#define DISPLAY(NAME) DT_GI(dt_kap85_display,(param, NAME, maxval, #NAME))

/*..........................................................................*/
static dt_rc_e hist_plot(				/* make histogram plot				*/
  dt_ctl_t *ctl,
  dtimage_t *hplot,
  double *hist,
  long maxval,
  double scale,
  double offset,
  double foreground)
{
  static char *F = "hist_plot";
  const int ybot = hplot->ye - 2;		/* histogram plot limits			*/
  int ytop = 2;
  int x, y;

  maxval = DT_MIN(maxval, hplot->xe);
  for (x=0; x<maxval; x++)				/* for each bar					    */
  {
    const int ybar = ybot -				/* bar height						*/
      (int)(hist[x]*scale+offset);
	for (y=ybot; y>=ybar; y--)
	  DT_Q(dtimage_set_pixel,(ctl,
        hplot, x, y, foreground));
  }

  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/

dt_rc_e
dt_kap85_display(						/* display histogram for debug		*/
  dtparam_t *param,
  double *hist,
  int maxval,
  char *name)
{
  static char *F = "dt_kap85_display";
  dt_ctl_t *ctl = &param->ctl;
  dtimage_t display;
  char window[32];
  long min, max;
  double scale, offset;
  static double height = 128;
  const background = 0;
  const foreground = 255;

  strcpy(window, ":");
  strcat(window, name);

  DT_Q(dt1d_range_double,(ctl, hist, maxval,
    &min, &max));
  scale = (height-4.0) / (hist[max] - hist[min]);
  offset = -scale * hist[min];

  dt_say(ctl, F, "%-10s min %e, max %e, scale %e, offset %e",
    name, hist[min], hist[max], scale, offset);

  DT_Q(dtimage_create,(ctl,				/* image for histogram plot		    */
    &display, maxval, height,
    DT_NTYPE_B08));

  DT_Q(dtimage_constant,(ctl,			/* clear histogram plot				*/
    &display, background));

  DT_Q(hist_plot,(ctl,
    &display, hist, maxval, scale, offset,
    foreground));

  DT_Q(dtimage_export,(ctl, &display,
    window, -1, -1));

  DT_Q(dtimage_free,(ctl, &display));
  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/

dt_rc_e
dt_kap85_hist(							/* compute threshold on hist only	*/
  dtparam_t *param,
  double *hist,							/* input histogram				    */
  int maxval,							/* histogram width				    */
  double scale,							/* scale to apply to histogram	    */
  double *thresh)						/* output threshold					*/
{
  static char *F = "dt_kap85";
  dt_ctl_t *ctl = &param->ctl;
  double *Ps;
  double *Hs;
  double *plogp;
  double *psi;
  double P;
  double H;
  double Hn;
  double p1;
  int i;
  int max;
  dt_rc_e rc = DT_RC_GOOD;

  DT_Q(dtos_malloc2,(ctl,				/* space for buffers			    */
    (void **)&Ps,
    4*sizeof(double)*maxval,
    F, "computation buffers"));

  Hs = Ps + maxval;						/* partition buffers			    */
  psi = Hs + maxval;
  plogp = psi + maxval;

  P = 0.0;
  H = 0.0;
  for (i=0; i<maxval; i++)
  {
    const double p = hist[i] * scale;	/* frequency probability		    */
    Ps[i] = P += p;						/* probability distribution		    */
    plogp[i] = p * (p?log(p): 0.0);
    Hs[i] = H -= p * (p?log(p): 0.0);	/* entropy						    */
  }

  Hn = Hs[maxval-1];					/* total entropy of histogram	    */
  p1 = 1.0 - scale;						/* close enough to unity 			*/

  for (i=0; i<maxval; i++)
  {
    double HA, HB;

    if (Ps[i] > 0.0)					/* not at very start?			    */
      HA = log(Ps[i]) +
        Hs[i] / Ps[i];
    else
      HA = 0.0;

    if (Ps[i] <= p1)					/* not at very end?				    */
      HB = log(1.0 - Ps[i]) +
        (Hn - Hs[i]) / (1.0 - Ps[i]);
    else
      HB = 0.0;

	psi[i] = HA + HB;

    if (i == 0 || psi[i] > psi[max])
      max = i;
  }

  if (ctl->dbg_mask &&					/* want to display intermediate?	*/
      DT_DBG_MASK_INTERMD)
  {
    DISPLAY(hist);
    DISPLAY(plogp);
    DISPLAY(Ps);
    DISPLAY(Hs);
    DISPLAY(psi);

    dt_dbg(ctl, F, DT_DBG_MASK_INTERMD,
      "maximum entropy %e at s %d",
      psi[max], max);
  }

  DT_Q(dtos_free2,(ctl, Ps,
    F, "computation buffers"));

  *thresh = max + 1;

  return DT_RC_GOOD;
}


/*..........................................................................
 *..........................................................................*/

dt_rc_e
dt_kap85(								/* compute factor					*/
  dtparam_t *param,
  dtimage_t *image,						/* input image					    */
  double *thresh)						/* output threshold					*/
{
  static char *F = "dt_kap85";
  dt_ctl_t *ctl = &param->ctl;
  double *hist;
  const maxval = 256;
  dt_rc_e rc = DT_RC_GOOD;

  DT_GI(dtos_malloc2,(ctl,				/* space for histogram			    */
    (void **)&hist,
    sizeof(double)*maxval,
    F, "histogram buffer"));

  DT_GI(dtimage_hist,(ctl,				/* make histogram				    */
    image, hist, maxval, 0.0)); 
  
  DT_GI(dt_kap85_hist,(param,			/* compute threshold			    */
    hist, maxval, 
    1.0 /								/* scale to make frequencies into   */
    (double)(image->xe * image->ye),	/*    probabilities				    */
    thresh));

  DT_Q(dtos_free2,(ctl, hist,			/* free histogram				    */
    F, "histogram buffer"));

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
