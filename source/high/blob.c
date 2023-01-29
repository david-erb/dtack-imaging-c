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
  19990813 DE check for null image
 *..........................................................................*/


#include <dtack/base.h>
DT_RCSID("source/high $RCSfile: blob.c,v $ $Revision: 1.1 $");
#include <dtack/stack.h>
#include <dtack/mem.h>

/*..........................................................................
 * mean, sum, area
 *..........................................................................*/

static
dt_rc_e
dtimage_blob_stats_1(					/* blob mean, sum, area 			*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtimage_t *image,						/* image of gray level values 		*/
  dtrun_t *run,							/* run from which labels were made 	*/
  dtrun_blob_t *blob,					/* blob representation 				*/
  dtstats_t *stats,					/* return statistics 				*/
  const int shift)						/* log2 pixel size					*/
{
  DT_F("dtimage_blob_stats_1");
  dtrun_id_t p;
  dtrun_elem_t *e;

  {
    double sum = 0.0;
	unsigned long area = 0;
    for (p = blob->phead;				/* point to first run in chain		*/
		 p != DTRUN_NOID;				/* until end of chain 				*/
		 p = e->p)						/* point to next run in chain	    */
	{
	  double s;
	  char *v;
  	  DTRUN_PEEK(DT_Q, ctl,				/* access run in blob chain 	 	*/
        run, p, &e);
	  DTIMAGE_GETROW(ctl, image, e->y,	/* access row 						*/
        (void **)&v);
	  dt1d_sum(image->ntype,			/* sum(v)							*/
        v+(e->x<<shift), e->l+1, &s);
	  DTIMAGE_UNMAPROW(ctl, image,		/* release row 						*/
        e->y);
	  area += e->l + 1;					/* accumulate area 					*/
	  sum += s;
	}

	stats->sum = sum;
	stats->area = area;
	stats->mean = sum / area;
    stats->have |= 
      DTSTATS_SUM |
	  DTSTATS_AREA |
	  DTSTATS_MEAN;
  } 

  return DT_RC_GOOD;
}

/*..........................................................................
 * mean, sum, area, min, max
 *..........................................................................*/

static
dt_rc_e
dtimage_blob_stats_2(
  dt_ctl_t *ctl,						/* environment control 				*/
  dtimage_t *image,						/* image of gray level values 		*/
  dtrun_t *run,							/* run from which labels were made 	*/
  dtrun_blob_t *blob,					/* blob representation 				*/
  dtstats_t *stats,						/* return statistics 				*/
  const int shift)						/* log2 pixel size					*/
{
  DT_F("dtimage_blob_stats_2");
  dtrun_id_t p;
  dtrun_elem_t *e;

  {
    double sum = 0.0;
	unsigned long area = 0;
	double minval = dt_ntype_maxval[
      image->ntype];
	double maxval = dt_ntype_minval[
      image->ntype];
	dt1d_i_t min, max;
    for (p = blob->phead;				/* point to first run in chain		*/
		 p != DTRUN_NOID;				/* until end of chain 				*/
		 p = e->p)						/* point to next run in chain	    */
	{
	  double s;
	  char *v;
	  double g;
  	  DTRUN_PEEK(DT_Q, ctl,				/* access run in blob chain 	 	*/
        run, p, &e);
	  DTIMAGE_GETROW(ctl, image, e->y,	/* access row 						*/
        (void **)&v);

	  dt1d_sum(image->ntype,			/* sum(v)							*/
        v+(e->x<<shift), e->l+1, &s);
	  area += e->l + 1;					/* accumulate area 					*/
	  sum += s;

	  dt1d_range(image->ntype,			/* find data range 					*/
        v+(e->x<<shift), e->l+1, 
        &min, &max);
      DTNTYPE_GET1(DT_Q, ctl,			/* fetch min pixel					*/
        v, e->x+min, image->ntype, &g);
	  if (g < minval) 
        minval = g;
      DTNTYPE_GET1(DT_Q, ctl,			/* fetch max pixel					*/
        v, e->x+max, image->ntype, &g);
	  if (g > maxval) 
        maxval = g;

	  DTIMAGE_UNMAPROW(ctl, image,		/* release row 						*/
        e->y);
	}

	stats->sum = sum;
	stats->area = area;
	stats->mean = sum / area;
    stats->have |= 
      DTSTATS_SUM |
	  DTSTATS_AREA |
	  DTSTATS_MEAN;

	stats->minval = minval;
	stats->maxval = maxval;
    stats->have |=
	  DTSTATS_MINVAL |
	  DTSTATS_MAXVAL;
  } 

  return DT_RC_GOOD;
}

/*..........................................................................
 * min, max
 *..........................................................................*/

static
dt_rc_e
dtimage_blob_stats_3(
  dt_ctl_t *ctl,						/* environment control 				*/
  dtimage_t *image,						/* image of gray level values 		*/
  dtrun_t *run,							/* run from which labels were made 	*/
  dtrun_blob_t *blob,					/* blob representation 				*/
  dtstats_t *stats,					/* return statistics 				*/
  const int shift)						/* log2 pixel size					*/
{
  DT_F("dtimage_blob_stats_3");
  dtrun_id_t p;
  dtrun_elem_t *e;

  {
	double minval = dt_ntype_maxval[
      image->ntype];
	double maxval = dt_ntype_minval[
      image->ntype];
	dt1d_i_t min, max;
    for (p = blob->phead;				/* point to first run in chain		*/
		 p != DTRUN_NOID;				/* until end of chain 				*/
		 p = e->p)						/* point to next run in chain	    */
	{
	  char *v;
	  double g;
  	  DTRUN_PEEK(DT_Q, ctl,				/* access run in blob chain 	 	*/
        run, p, &e);
	  DTIMAGE_GETROW(ctl, image, e->y,	/* access row 						*/
        (void **)&v);

	  dt1d_range(image->ntype,			/* find data range 					*/
        v+(e->x<<shift), e->l+1, 
        &min, &max);
      DTNTYPE_GET1(DT_Q, ctl,			/* fetch min pixel					*/
        v, e->x+min, image->ntype, &g);
	  if (g < minval) 
        minval = g;
      DTNTYPE_GET1(DT_Q, ctl,			/* fetch max pixel					*/
        v, e->x+max, image->ntype, &g);
	  if (g > maxval) 
        maxval = g;

	  DTIMAGE_UNMAPROW(ctl, image,		/* release row 						*/
        e->y);
	}

	stats->minval = minval;
	stats->maxval = maxval;
    stats->have |=
	  DTSTATS_MINVAL |
	  DTSTATS_MAXVAL;
  } 

  return DT_RC_GOOD;
}

/*..........................................................................
| NAME
| dtimage_blob_stats() - compute blob statistics
| 
| SUMMARY
| Compute statistics on given blob.
| Compute only the statistics indicated by bits in the ~stats->want~ member.
| Attempt to be as efficient as possible:
| assume that statistics in ~stats->have~ do not need to be recomputed.
| Set bits in ~stats->have~ for each requested statistic,
| as well as any statistics computed for free.
| 
| END
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtimage_blob_stats(						/* compute blob statistics 			*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtimage_t *image,						/* image of gray level values 		*/
  dtrun_t *run,							/* run from which labels were made 	*/
  dtrun_blob_t *blob,					/* blob representation 				*/
  dtstats_t *stats)						/* return statistics 				*/
{
  DT_F("dtimage_blob_stats");
  dtrun_id_t p;
  dtrun_elem_t *e;
  unsigned long have = stats->have;
  unsigned long want = stats->want;
  dt_rc_e rc = DT_RC_GOOD;

  if (!image)
	DT_CLEANUP((ctl, F,
      "image supplied for blob statistics is NULL"));
  {
  const double dsize =
    dt_ntype_sizeof[image->ntype];
  
  const int shift =						/* pixel size shift 				*/
    (dsize == 1.0? 0:
    (dsize == 2.0? 1:
    (dsize == 4.0? 2:
    (dsize == 8.0? 3: -1))));
  
  if (shift < 0)
    return dt_err(ctl, F,
      "cannot handle %g-bit data",
      dsize);

										/* ................................ */
  want &= ~have;
  if ((want & DTSTATS_SUM       ||		/* first moment stuff? 				*/
       want & DTSTATS_AREA      ||
	   want & DTSTATS_MEAN)     &&
	  (want & DTSTATS_MINVAL    ||		/* plus min and max? 				*/
	   want & DTSTATS_MAXVAL))
  {
	DT_C(dtimage_blob_stats_2,(ctl,
      image, run, blob, stats, 
      shift));
    have = stats->have;
  } 

										/* ................................ */
  want &= ~have;
  if (want & DTSTATS_SUM      ||		/* first moment stuff only 			*/
      want & DTSTATS_AREA     ||
	  want & DTSTATS_MEAN     ||
	  want & DTSTATS_VARIANCE)
  {
	DT_C(dtimage_blob_stats_1,(ctl,
      image, run, blob, stats, shift));
    have = stats->have;
  } 
										/* ................................ */
  want &= ~have;
  if (want & DTSTATS_MINVAL ||			/* just min and max 				*/
	  want & DTSTATS_MAXVAL)
  {
	DT_C(dtimage_blob_stats_3,(ctl,
      image, run, blob, stats, shift));
    have = stats->have;
  }
										/* ................................ */
  want &= ~have;
  if (want & DTSTATS_VARIANCE)
  {
    double sum = 0.0;
    for (p = blob->phead;				/* point to first run in chain		*/
		 p != DTRUN_NOID;				/* until end of chain 				*/
		 p = e->p)						/* point to next run in chain	    */
	{
	  char *v;
	  double s;
  	  DTRUN_PEEK(DT_C, ctl,				/* access run in blob chain 	 	*/
        run, p, &e);
	  DTIMAGE_GETROW(ctl, image, e->y,	/* access row 						*/
        (void **)&v);
	  dt1d_sumsq_scalar(image->ntype,	/* sum((v-mean) * (v-mean)) 		*/
        v+(e->x<<shift), e->l+1, 
        stats->mean, &s);
	  DTIMAGE_UNMAPROW(ctl, image,		/* release row 						*/
        e->y);
	  sum += s;
	}
	if (stats->area <= 1)
	  stats->variance = 0;
	else
	  stats->variance = sum / (stats->area-1);
    have |= DTSTATS_VARIANCE;
  } 
										/* ................................ */
  want &= ~have;
  if (want & DTSTATS_BOUNDBOX ||
      want & DTSTATS_SIZE)
  {
	dtxy_t xmin = image->xe;
	dtxy_t ymin = image->ye;
	dtxy_t xmax = 0;
	dtxy_t ymax = 0;
	unsigned long area = 0;
    for (p = blob->phead;				/* point to first run in chain		*/
		 p != DTRUN_NOID;				/* until end of chain 				*/
		 p = e->p)						/* point to next run in chain	    */
	{
  	  DTRUN_PEEK(DT_C, ctl,				/* access run in blob chain 	 	*/
        run, p, &e);
	  xmin = DT_MIN(xmin, e->x); 
	  ymin = DT_MIN(ymin, e->y);
	  xmax = DT_MAX(xmax, e->x+e->l); 
	  ymax = DT_MAX(ymax, e->y);
	  area += e->l + 1;
	}
    have |= DTSTATS_BOUNDBOX ||
			DTSTATS_AREA;

	stats->xmin = xmin;					/* bounding box 					*/
	stats->xmax = xmax;
	stats->ymin = ymin;
	stats->ymax = ymax;

	stats->area = area;					/* size 							*/
	stats->width = xmax - xmin + 1;
	stats->height = ymax - ymin + 1;
  } 
										/* ................................ */
  want &= ~have;
  if (want & DTSTATS_CENTROID)
  {
	unsigned long area = 0;
	unsigned long xsum = 0;
	unsigned long ysum = 0;
    for (p = blob->phead;				/* point to first run in chain		*/
		 p != DTRUN_NOID;				/* until end of chain 				*/
		 p = e->p)						/* point to next run in chain	    */
	{
	  dtxy_t x;
  	  DTRUN_PEEK(DT_C, ctl,				/* access run in blob chain 	 	*/
        run, p, &e);
      for (x=e->x; x<=e->x+e->l; x++)	/* process all pixels in run	   	*/
	    xsum += x;
      ysum += e->y * (e->l + 1);
	  area += e->l + 1;
	}
    have |= DTSTATS_CENTROID;

	stats->xcentroid = (double)xsum /	/* centroid 						*/
	  area;
	stats->ycentroid = (double)ysum /
	  area;

	stats->area = area;
  } 
										/* ................................ */

  stats->have = have;
  }

cleanup:
  return rc;
}

/*..........................................................................
| NAME
| dtimage_blob_hist() - compute blob histogram
| 
| SUMMARY
| Accumulate histogram of given blob.
| Presume that the histogram structure is already allocated by dthist_alloc().
| Do not clear the histogram before accumulating values from the blob.
| 
| END
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtimage_blob_hist(						/* accumulate blob histogram		*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtimage_t *image,						/* image of gray level values 		*/
  dtrun_t *run,							/* run from which labels were made 	*/
  dtrun_blob_t *blob,					/* blob representation 				*/
  dthist_t *hist)						/* return histogram					*/
{
  DT_F("dtimage_blob_hist");
  dtrun_id_t p;
  dtrun_elem_t *e;

  for (p = blob->phead;					/* point to first run in chain		*/
	   p != DTRUN_NOID;					/* until end of chain 				*/
	   p = e->p)						/* point to next run in chain	    */
  {
	void *v;
	DTRUN_PEEK(DT_Q, ctl,				/* access run in blob chain 	 	*/
      run, p, &e);
	DTIMAGE_GETROW(ctl, image, e->y,	/* access row 						*/
      &v);
	DT_Q(dthist_add_values,(ctl,		/* add values to histogram 			*/
      hist, v, e->x, e->x+e->l+1, 
      image->ntype));
    DTIMAGE_UNMAPROW(ctl, image, e->y);	/* release row 						*/
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
