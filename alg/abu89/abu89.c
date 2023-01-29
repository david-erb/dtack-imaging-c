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

static char rcsid[] = "lib $RCSfile: abu89.c,v $ $Revision: 1.3 $";

#include <dtack/base.h>

static long kernel_3x3_ave[9] = {		/* default averaging kernel		    */
  1,1,1,
  1,1,1,
  1,1,1
};

dt_rc_e dtimage_sum_cumulative(			/* make cumulative sum				*/
  dt_ctl_t *ctl,
  dtimage_t *input,
  dtimage_t *output);
dt_rc_e dtimage_sum_cumulative_rev(		/* make cumulative sum				*/
  dt_ctl_t *ctl,
  dtimage_t *input,
  dtimage_t *output);

#define DISPLAY(NAME) DT_GI(dt_abu89_display,(param, NAME, #NAME))

/*..........................................................................
 *..........................................................................*/

dt_rc_e
dt_abu89_display(						/* display image for debug			*/
  dtparam_t *param,
  dtimage_t *image,
  char *name)
{
  static char *F = "dt_abu89_display";
  dt_ctl_t *ctl = &param->ctl;
  dtimage_t display;
  char window[32];
  double min, max;
  double scale, offset;
  strcpy(window, ":");
  strcat(window, name);
  DT_Q(dtimage_range,(ctl, image,
    &min, &max));
  scale = 255.0 / (max - min);
  offset = -scale * min;
  dt_say(ctl, F, "%-10s min %e, max %e, scale %e, offset %e",
    name, min, max, scale, offset);
  DT_Q(dtimage_create,(ctl,
    &display, image->xe, image->ye,
    DT_NTYPE_B08));
  DT_Q(dtimage_linear_scalar,(ctl,
    image, scale, offset, &display));
  DT_Q(dtimage_export,(ctl, &display,
    window, -1, -1));
  DT_Q(dtimage_free,(ctl, &display));
  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/

dt_rc_e
dt_abu89_Hst(							/* compute Hst						*/
  dtparam_t *param,
  dtimage_t *nlogn,						/* n*log(n) of PMF				    */
  double Hst)
{
  static char *F = "dt_abu89_Hst";
  dt_ctl_t *ctl = &param->ctl;
  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/

dt_rc_e
dt_abu89(								/* compute factor					*/
  dtparam_t *param,
  dtimage_t *image,						/* input image					    */
  dtimage_t *average,					/* input neighborhood average	    */
  dtimage_t *joint,						/* output probability matrix	    */
  double *thresh)						/* output threshold					*/
{
  static char *F = "dt_abu89";
  dt_ctl_t *ctl = &param->ctl;
  dtimage_t local_image;
  dtimage_t local_convolve, *convolve;
  dtimage_t local_average;
  dtimage_t local_joint;
#define NTEMP 11
  dtimage_t temp[NTEMP];
  dtimage_t *p = &temp[0];
  dtimage_t *logp = &temp[1];
  dtimage_t *plogp = &temp[2];
  dtimage_t *Psta = &temp[3];
  dtimage_t *Pstb = &temp[4];
  dtimage_t *Hsta = &temp[5];
  dtimage_t *Hstb = &temp[6];
  dtimage_t *HA = &temp[7];
  dtimage_t *HB = &temp[8];
  dtimage_t *entropy = &temp[9];
  dtimage_t *Pstc = &temp[10];
  dtimage_t import;
  double min, max, pscale;
  long x, y;
  double hmm;
  double psimax;
  double pstcmax;
  int s, t;
  int i;
  int smax, tmax;
  dt_rc_e rc = DT_RC_GOOD;

  if (average == NULL)					/* caller is not giving averge?	    */
  {
    convolve = &local_convolve;
    convolve->xe = 0;
    DT_GI(dtimage_create,(ctl,			/* space for convolve image		    */
      convolve,
      image->xe, image->ye,
      image->ntype));

    DT_GI(dtimage_constant,(ctl,		/* clear convolve image			    */
      convolve, 0.0));

    DT_GI(dtimage_convolve_long,(ctl,	/* make convolve image				*/
      image, kernel_3x3_ave, 3, 3, 9,
      convolve));

    average = &local_average;
    average->xe = 0;
    DT_GI(dtimage_overlay,(ctl,			/* trim convolution edge effect	    */
      convolve, average,
      1, 1,
      convolve->xe-2, 
      convolve->ye-2));

    local_image.xe = 0;
    DT_GI(dtimage_overlay,(ctl,
      image, &local_image,
      1, 1,
      convolve->xe-2, 
      convolve->ye-2));
    image = &local_image;
  }
  else
    convolve = NULL;

  if (joint == NULL)					/* caller is not giving joint?	    */
  {
    joint = &local_joint;
    joint->xe = 0;
    DT_GI(dtimage_create,(ctl, joint,	/* space for joint matrix			*/
       256, 256, DT_NTYPE_B32));
  }
  DT_GI(dtimage_joint,(ctl,				/* make joint probability matrix    */
    image, average, joint));  

  for (i=0; i<NTEMP; i++)
  {
    temp[i].xe = 0;						/* temp space					    */
    DT_GI(dtimage_create,(ctl,
        &temp[i],
        256, 256, DT_NTYPE_F2));
  }

  {
    dtimage_t jt;
    DT_GI(dtimage_create,(ctl, &jt,
       256, 256, DT_NTYPE_B32));
    DT_GI(dtimage_sum_cumulative,(	
      ctl, joint, &jt));
    dt_say(ctl, F, "jt[255[255] %ld", jt.row[255].b32[255]);
  }

  DT_GI(dtimage_cast,(ctl, joint, p));	/* make it a double				    */

  DT_GI(dtimage_mult_scalar,(ctl, p,	/* normalize probabilities (eqn 5)	*/
    1.0/(double)(image->xe*image->ye),
    p));
  DISPLAY(p);

  DT_GI(dtimage_sum_cumulative,(		/* accumulate all Pst's (eqn 7)	    */
    ctl, p, Psta));
  DISPLAY(Psta);
  DT_GI(dtimage_sum_cumulative_rev,(	/* accumulate all Pst's (eqn 7)	    */
    ctl, p, Pstb));
  DISPLAY(Pstb);

  for (y=0; y<p->ye; y++)				/* clear out zeros before log	    */
  {
    dt_ntype_f2_t * const q =
      p->row[y].f2;
    for (x=0; x<p->xe; x++)
      if (q[x] == 0.0) q[x] = 1.0;
  }

  DT_GI(dtimage_log,(ctl,				/* -log(p)							*/
    p, -1.0, 0.0, logp));
  DISPLAY(logp);

  DT_GI(dtimage_mult_image,(ctl,		/* p * -log(p)					    */
    p, logp, 1.0, plogp));
  DISPLAY(plogp);

  DT_GI(dtimage_sum_cumulative,(		/* accumulate all Hst's (eqn 9)	    */
    ctl, plogp, Hsta));
  DISPLAY(Hsta);
  DT_GI(dtimage_sum_cumulative_rev,(	/* accumulate all Hst's (eqn 9)	    */
    ctl, plogp, Hstb));
  DISPLAY(Hstb);

  smax = tmax = 0;
  psimax = 0.0;
  for (s=0; s<255; s++)
  {
    for (t=0; t<255; t++)
	{
      double pst, ha, hb, psi;
      if (pst = Psta->row[s].f2[t])
	    ha = log(pst) + Hsta->row[s].f2[t] / pst;
	  else
	    ha = 0.0;

      if (pst = Pstb->row[s+1].f2[t+1])
	    hb = log(pst) + Hstb->row[s+1].f2[t+1] / pst;
	  else
	    hb = 0.0;

      psi = ha + hb;

      entropy->row[s].f2[t] = psi;
      HA->row[s].f2[t] = ha;
      HB->row[s].f2[t] = hb;

	  Pstc->row[s].f2[t] = 1.0 - Psta->row[s].f2[t] - Pstb->row[s+1].f2[t+1];

      if (psi > psimax)
	  {
        smax = s;
        tmax = t;
        psimax = psi;
        pstcmax = Pstc->row[s].f2[t];
	  }
    }
  }
  DISPLAY(HA);
  DISPLAY(HB);
  DISPLAY(Pstc);
  DISPLAY(entropy);

  dt_say(ctl, F, "maximum entropy %e at s %d and t %d, off-diagonal %e",
    psimax, smax, tmax, pstcmax);

  import.xe = 0;
  {
    char temp[80];
  DT_GI(dtimage_import,(ctl,
    &import, strcpy(temp, ":p")));
  DT_GI(dtimage_plot_line,(ctl,
    &import, tmax, 0, tmax, 255, 4));
  DT_GI(dtimage_plot_line,(ctl,
    &import, 0, smax, 255, smax, 4));
  DT_GI(dtimage_export,(ctl,
    &import, temp, -1, -1));
  }

  if (import.xe != 0)
    dtimage_free(ctl, &import);

  for (i=NTEMP-1; i>=0; i--)
  {
    if (temp[i].xe != 0)
      dtimage_free(ctl, &temp[i]);
  }

  if (joint == &local_joint &&
      joint->xe != 0)
    dtimage_free(ctl, joint);

  if (average == &local_average &&
      average->xe != 0)
    dtimage_free(ctl, average);

  if (image == &local_image &&
      image->xe != 0)
    dtimage_free(ctl, image);
   
  if (convolve == &local_convolve &&
      convolve->xe != 0)
    dtimage_free(ctl, convolve);

  *thresh = tmax + 1;
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
