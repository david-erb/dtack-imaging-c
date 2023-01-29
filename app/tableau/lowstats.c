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
| main program
| 
| END
 *..........................................................................*/

#include <tableau.h>

DT_RCSID("app/tableau $RCSfile: lowstats.c,v $ $Revision: 1.8 $");

#define TS (8192)
#define TS_MASK (0x1fff)
typedef unsigned short tableau_lowstat_t;

/*..........................................................................
 *..........................................................................*/

static
dt_rc_e 
tableau_lowstats_prob(
  dt_ctl_t *ctl,
  dthist_t *hist,
  unsigned char *p,						/* output pointer 					*/
  int max,
  int *used)
{
  DT_F("tableau_lowstats_prob");
  dthist_bin_t i;
  dthist_freq_t sum = 0;
  double scale;
  tableau_lowstat_t *q =
    (tableau_lowstat_t *)p;
  int need = (hist->width + 1) *
    sizeof(*q);

  if (max < need)
    return dt_err(ctl, F,
      "buffer needs at least %d bytes,"
      " has only %d", 
      need, max);

  for (i=0; i<hist->width; i++)			/* get histogram sum 				*/
    sum += hist->freq[i];

  if (sum > 0)
    scale = (double)(TS-1) / sum;		/* scale to table size				*/
  else
    scale = 0.0;

  *q++ = hist->width;					/* encode histogram width 			*/

  sum = 0;
  for (i=0; i<hist->width; i++)			/* encode histogram bins 			*/
  {
	sum += hist->freq[i];
    *q++ = DT_MIN((TS-1), (int)
      (sum * scale + 0.5));
  }
  
  q[-1] = TS-1;							/* ensure P=1 						*/

  *used = need;							/* return how much we used 			*/
  return DT_RC_GOOD;
}

/*..........................................................................
 * encode low stats of image
 * assume p has enough space (returned by lowstats_size)
 *..........................................................................*/
  
dt_rc_e
DTCONFIG_API1
tableau_lowstats_encode(
  dtparam_t *param,
  dtimage_t *image,						/* image to encode					*/
  int dmax,
  int lmax,
  dthist_t *fhist,						/* work structures 					*/
  dthist_t *lhist,
  void *lowstats,						/* output pointer 					*/
  int max,
  int *used,
  dtscr_t *scr)
{
  DT_F("tableau_lowstats_encode");
  dt_ctl_t *ctl = &param->ctl;
  unsigned char *p = (unsigned char *)
    lowstats;
  int i;
  int needed;
  dt_rc_e rc = DT_RC_GOOD;
  
  DTCSTACK_HIGHWATER;

  if (image == NULL ||					/* need to release histograms? 		*/
	  fhist->width != dmax)
  {
	for (i=0; i<fhist->width; i++)
	  if (lhist[i].width)
	    DT_I(dthist_free,(ctl, 
          &lhist[i]));
	if (fhist->width)
	  DT_I(dthist_free,(ctl, fhist));
  }

  if (image == NULL)					/* supposed to quit? 				*/
    return rc;

  if (dmax == 0)						/* not supposed to do anything? 	*/
  {
	*used = 0;
	return rc;
  }

  if (fhist->width == 0)				/* structure not initialized yet? 	*/
  {
	DT_Q(dthist_alloc,(ctl, fhist,		/* space for frequency histogram 	*/
      1 + dmax + 1 + dmax + 1,
      -dmax-1, dmax+2));
	for (i=0; i<fhist->width; i++)
	  DT_Q(dthist_alloc,(ctl,			/* space for length histograms		*/
        &lhist[i], lmax, 0, lmax));
  }

  DT_Q(dthist_clear,(ctl, fhist));		/* clear all histograms 			*/
  for (i=0; i<fhist->width; i++)
    DT_Q(dthist_clear,(ctl,
      &lhist[i]));

  DT_Q(tableau_run_stats,(param,		/* build run statistics 			*/
    image, fhist, lhist));

  *used = 0;
  DT_Q(tableau_lowstats_prob,(ctl,		/* encode frequency histogram 		*/
    fhist, p+(*used), max, &needed));
  *used += needed;
  max -= needed;

  for (i=0; i<fhist->width; i++)
  {
	DT_Q(tableau_lowstats_prob,(ctl,	/* encode length histograms 		*/
      &lhist[i], 
      p+(*used), max, &needed));
	*used += needed;
	max -= needed;
  }

  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/


#define UMASK (0x1ffff)
#define UMAX (0x20000)
static tableau_lowstat_t *uniform = NULL;
#define U(u) uniform[u++ & UMASK]

dt_rc_e
DTCONFIG_API1
tableau_lowstats_init(
  dt_ctl_t *ctl)
{
  DT_F("tableau_lowstats_init");
  dt1d_ran_t ran;
  int i;

  if (uniform != NULL)
    dt_err(ctl, F,
      "already initialized");

  DT_Q(dtos_malloc2,(ctl,				/* space for random numbers 		*/
    (void **)&uniform,  
    UMAX * sizeof(*uniform), 
    F, "uniform table"));

  DT_Q_1D(dt1d_ran_uniform_init,(&ran,	/* init random number generator 	*/
    0));

  dt_dbg(ctl, F, DT_DBG_MASK_USR1,
    "generating random numbers");
  for (i=0; i<UMAX; i++)
  {
	double x;
    DT_Q_1D(dt1d_ran_uniform,(
      &ran, &x));
	uniform[i] = (tableau_lowstat_t)
      (TS * x) & TS_MASK;
  }

  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
tableau_lowstats_uninit(
  dt_ctl_t *ctl)
{
  DT_F("tableau_lowstats_uninit");
  dt_rc_e rc = DT_RC_GOOD;

  if (uniform != NULL)
    DT_I(dtos_free2,(ctl,
      uniform,
      F, "uniform table"));

  return rc;
}

/*..........................................................................
 *..........................................................................*/

static
dt_rc_e
DTCONFIG_API1
tableau_lowstats_rotate(
  dt_ctl_t *ctl,
  tableau_lowstat_t *p,
  tableau_lowstat_t g,
  tableau_lowstat_t *t)
{
  DT_F("tableau_lowstats_rotate");
  int i, k, w;

  w = *p++;								/* width 							*/
  k = 0;								/* table index 						*/
  for (i=0; i<w; i++)					/* for each histogram bin 			*/
  {
	int k0 = k;
	while (k<p[i] || k == (TS-1))		/* expand bin into table 			*/
      t[k++] = g;
	if (k0 < k)
    g++;
  }
   
  if (k != TS)
    return dt_err(ctl, F,
      "insanity check: w=%d, k=%d", 
      w, k);
  
  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
tableau_lowstats_decode(
  dtparam_t *param,
  dtimage_t *image,						/* image to decode into				*/
  void *lowstats,						/* input pointer 					*/
  int lowstats_size)					/* size of input 					*/
{
  DT_F("tableau_lowstats_decode");
#ifdef NONO
  dt_ctl_t *ctl = &param->ctl;
  tableau_lowstat_t *p = 
    (tableau_lowstat_t *)lowstats;
  int underflow, overflow;
  int i, w;
  int used;
  const dtxy_t xmax = image->xe;

  tableau_lowstat_t ftable[TS];
  tableau_lowstat_t ltable[TABLEAU_MAXDIFF][TS];

  dtxy_t x, y;
  int u = 0;
  
  DTCSTACK_HIGHWATER;

  if (lowstats_size == 0)
    return DT_RC_GOOD;

  if (uniform == NULL)
    DT_Q(tableau_lowstats_init,( ctl));
  
  w = *p;
  underflow = -w / 2 + 128;
  overflow = w / 2 + 128;
  dt_dbg(ctl, F, DT_DBG_MASK_USR1,
    "w=%d, underflow=%d, overflow=%d",
    w, underflow, overflow);
  DT_Q(tableau_lowstats_rotate,(ctl,
    p, underflow, ftable));

  p += w + 1;							/* skip the ftable 					*/

  for (i=0; i<w; i++)					/* for each ltable 					*/
  {
    DT_Q(tableau_lowstats_rotate,(ctl,	/* rotate the lut 					*/
      p, 0, ltable[i]));
	p += *p + 1;
  }

  used = sizeof(*p) * (p - 
    (tableau_lowstat_t *)lowstats);
  if (used != lowstats_size)
    return dt_err(ctl, F,
      "insanity check: decoded %d of %d",
      used, lowstats_size);

  for (y=0; y<image->ye; y++)
  {
	dt_ntype_b08_t * const p =
      image->row[y].b08;
	x = 0;								/* start of row 					*/
	while (x < xmax)
	{
	  int d, l;
	  d = ftable[U(u)];					/* the diff value 					*/
	  l = ltable[d-underflow][U(u)]+1;	/* the run length 					*/
	  if (l+x > xmax)					/* don't run off end of row 		*/
	    l = xmax - x;
	  if (d == overflow ||				/* one of the extremes? 			*/
		  d == underflow ||
		  d == 128)						/* diff is 0? 						*/
	    x += l;							/* save time 						*/
	  else
	  {
		d -= 128;
  	    while(l--)						/* add diff to pixels in run 		*/
	      p[x++] += d;
	  }
	}
  }
#endif

  return DT_RC_GOOD;
}
 
#ifdef NONO
/*..........................................................................
 *..........................................................................*/

static
void
_histprint(
  dt_ctl_t *ctl,
  dthist_t *hist)
{
  int w = hist->width;
  int i;
  for (i=0; i<w; i++)
  {
	double min, max;
    dthist_bin_to_value(NULL,
      hist, i, &min, &max);
    dt_say(ctl, "hist",
      "%4d. (%8g -> %8g) %8lu",
      i, min, max, hist->freq[i]);
  }
}

/*..........................................................................
 *..........................................................................*/

static
dt_rc_e
tableau_encode_hplot(
  dt_ctl_t *ctl,
  dthist_t *hist,
  const char *name)
{
  DT_F("tableau_encode_hplot");
  dtimage_t plot;
  dt_rc_e rc;

  plot.xe = 0;

  DT_C(dtimage_create,(ctl,				/* get space for histogram plot		*/
    &plot, hist->width, 64,
    DT_NTYPE_B08));

  DT_C(dtimage_constant,(ctl,			/* clear histogram plot				*/
    &plot, 0));

  DT_C(dtimage_hist_plot_horiz,(ctl,	/* make bar plot of histogram		*/
    &plot, hist,
    2, plot.ye-4, 255));

  DT_C(dtimage_export,(ctl,				/* output histogram plot			*/
    &plot, name, 
    DTIMAGE_NOPOS, 
    DTIMAGE_NOPOS));
cleanup:
  if (plot.xe)
    DT_I(dtimage_free,(ctl, &plot));
  return rc;
}

/*..........................................................................
 *..........................................................................*/

static
dt_rc_e 
tableau_encode_ucheck(
  dt_ctl_t *ctl,
  dthist_t *xhist,
  dthist_t *yhist,
  int *isuniform)
{
  DT_F("tableau_encode_ucheck");
  dt_stats_t xstats;
  dt_stats_t ystats;
  double xdf, xchisq, xprob;
  double ydf, ychisq, yprob;

										/* ................................ */
  memset(&xstats, 0, sizeof(xstats));
  xstats.want =
	  DT_STATS_AREA |
	  DT_STATS_SUM |
	  DT_STATS_MEAN |
	  DT_STATS_VARIANCE |
	  DT_STATS_STANDARD_DEVIATION;

  DT_Q(dthist_stats,(ctl, xhist,		/* compute stats for this histogram */
    &xstats));

  if (xstats.area < 300)
  {
	*isuniform = -1;
    return DT_RC_GOOD;
  }

  xstats.width = xhist->width;			/* number of bins				    */

  DT_Q(dthist_chisq1,(ctl, xhist,		/* compute chi-squared statistic 	*/
    dthist_bin_uniform, (void *)&xstats,
    &xdf, &xchisq, &xprob));
										/* ................................ */
  memset(&ystats, 0, sizeof(ystats));
  ystats.want =
	  DT_STATS_AREA |
	  DT_STATS_SUM |
	  DT_STATS_MEAN |
	  DT_STATS_VARIANCE |
	  DT_STATS_STANDARD_DEVIATION;

  DT_Q(dthist_stats,(ctl, yhist,		/* compute stats for this histogram */
    &ystats));

  ystats.width = yhist->width;			/* number of bins				    */

  DT_Q(dthist_chisq1,(ctl, yhist,		/* compute chi-squared statistic 	*/
    dthist_bin_uniform, (void *)&ystats,
    &ydf, &ychisq, &yprob));
										/* ................................ */

  *isuniform = 
    xprob >= 0.1 && 
    yprob >= 0.1;

  if (DT_DBG(ctl, DT_DBG_MASK_STATS))
  {
	  dt_dbg(ctl, F, DT_DBG_MASK_STATS,
        "%-20s = %ld",
        "area", xstats.area);
	  dt_dbg(ctl, F, DT_DBG_MASK_STATS,
        "%-20s = %gX %gY",
        "sum", xstats.sum, ystats.sum);
	  dt_dbg(ctl, F, DT_DBG_MASK_STATS,
        "%-20s = %gX %gY",
        "mean", xstats.mean, ystats.mean);
	  dt_dbg(ctl, F, DT_DBG_MASK_STATS,
        "%-20s = %gX %gY",
        "variance", xstats.variance, ystats.variance);
	  dt_dbg(ctl, F, DT_DBG_MASK_STATS,
        "%-20s = %gX %gY",
        "standard deviation", 
        xstats.standard_deviation, ystats.standard_deviation);
	  dt_dbg(ctl, F, DT_DBG_MASK_STATS,
        "%-20s = %gX %gY",
        "uniform df", xdf, ydf);
	  dt_dbg(ctl, F, DT_DBG_MASK_STATS,
        "%-20s = %gX %gY",
        "uniform chisq", xchisq, ychisq);
	  dt_dbg(ctl, F, DT_DBG_MASK_STATS,
        "%-20s = %6.4f %6.4f        %s distributed",
        "uniform prob", xprob, yprob,
        *isuniform? "UNIFORMLY": "NOT uniformly");
  }

  return DT_RC_GOOD;
}
#endif

#ifdef NONO
static
void
dummy()
{
	{
	  int i, k;
	  static int cutoff[] = {4, 8, 10, 12, 14, 16, 24, 32, 48, 64, 80, 96, 128, 0};
	  dt_ntype_b08_t table[256];
	  
	  for (k=0; cutoff[k]; i++)
	  {
		int k0 = cutoff[k];
		int k1 = cutoff[k+1];
		for (i=k0; i<k1; i++)
		table[128+i] = k0 + 8;
	  }
      table[0] = table[1];
	}

	{
	  for (k=0; cutoff[k]; i++)
	  {
		int k0 = cutoff[k];
		int k1 = cutoff[k+1];
		int v = 0;
		for (i=k0; i<k1; i++)
		  v += i;
		v /= (cutoff[k+1]-cutoff[k]);
		memset(table, 0, sizeof(table));
		for (i=k0; i<k1; i++)
		{
		  table[128+i+8] = v;
		  table[128-i-8] = -v;
		}
	  }
	}
    DT_C(tableau_fast_lookup,(ctl,		/* quantize diffs					*/
      diffs, 
      tableau_table_encode_diffs,
      diffs));
}
#endif

#ifdef NONO
static
void
dummy()
{
  dt_say(ctl, F,
    "histogram run frequencies"
    " by gray level");
  _histprint(ctl, &encode_arg->fhist);

  DT_Q(dtimage_export,(ctl, image,
    ":diffs", 
    DTIMAGE_NOPOS, 
    DTIMAGE_NOPOS));
  for (i=0; i<w; i++)
  {
	int isuniform;
    DT_Q(tableau_encode_ucheck,(ctl, 
      &encode_arg->xhist[i], 
      &encode_arg->yhist[i], 
      &isuniform));
	
	if (isuniform != -1)				/* significant? 					*/
	{
	  dtlut8_t lut8;
	  double min, max;
	  dthist_bin_to_value(NULL,
        &encode_arg->fhist, i, 
        &min, &max);

	  dt_say(ctl, F,
        "gray level %g -> %g X distribution",
        min, max);

	  DT_Q(tableau_encode_hplot,(ctl,
        &encode_arg->xhist[i], 
        ":X dist"));

	  DT_Q(tableau_encode_hplot,(ctl,
        &encode_arg->yhist[i], 
        ":Y dist"));

	  lut8.r = lut8.g = lut8.b = 255;
	  DT_Q(dtlut8_export,(ctl, &lut8,
        ":diffs", min+128, 1));
	  {
		char c;
		read(fileno(stdin), &c, 1);
	  }
	  lut8.r = lut8.g = lut8.b = min+128;
	  DT_Q(dtlut8_export,(ctl, &lut8,
        ":diffs", min+128, 1));
    }
  }
}
#endif





/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
  ! Copyright (C) 1998 Dtack Inc. All Rights Reserved
  ! To use this file, you must have signed a license agreement with Dtack Inc.
  ! Under no circumstances may you redistribute this file.
  ! This software is provided AS IS with no warranty expressed or implied.
  ! Dtack Inc. accepts no liability for use or misuse of this file.
  ! http://www.dtack.com  dtack@dtack.com  telephone +360.417.6700
  ! Dtack Inc, P.O. Box 3061, Port Angeles, WA USA 98362
  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */
