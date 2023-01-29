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
  19980712 DE proper computation of standard deviation, based on variance
 *..........................................................................*/


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
DT_RCSID("source/high $RCSfile: stats.c,v $ $Revision: 1.1 $");
#include <dtack/mem.h>
#include <dtack/os.h>

/*..........................................................................*/
dt_rc_e 
DTCONFIG_API1
dtimage_variance(						/* variance of pixel values			*/
  dt_ctl_t *ctl,
  dtimage_t *image,
  double *variance,
  double *mean)
{
  DT_F("dtimage_variance");
  dtstats_t stats;
 
  stats.have = DTSTATS_NONE;			/* don't have any stats yet 		*/
  stats.want = DTSTATS_VARIANCE;		/* want variance 					*/

  DT_Q(dtimage_stats,(ctl, image,		/* compute stats 					*/
    &stats));

  if (DTSTATS_MISSING(&stats))			/* check stats 						*/
	return dt_err(ctl, F,
      "some stats not computed");

  if (variance != NULL)					/* caller wants variance? 			*/
	*variance = stats.variance;

  if (mean != NULL)						/* caller wants mean? 				*/
    *mean = stats.mean;

  return DT_RC_GOOD;
}

/*..........................................................................
| NAME
| dtimage_stats2() - two image statistics
| 
| SUMMARY
| Get some statistics between two images.
| Possible statistics are :DTSTATS_CMP: and :DTSTATS_NDIFFS:.
| For :DTSTATS_CMP:, returned sign of comparison is in ~stats->sign~,
| And location of first difference is in ~stats->x0~ and ~stats->y0~.
| For :DTSTATS_NDIFFS:, returned number of differences is in  ~stats->area~.
| Promote both image pixels to a common type for comparison.
| Ignore pixels of image if beyond the extent of any other.
| 
| END
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtimage_stats2(							/* statistics on images				*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtimage_t *input1,					/* first input image 				*/
  dtimage_t *input2,					/* second input image 				*/
  double constant,						/* constant							*/
  dtstats_t *stats)						/* output statistics 				*/
{
  DT_F("dtimage_stats2");
  dtxy_t xmax, ymax;
  dt_ntype_e *ntypes = NULL;			/* we can handle any ntype		    */
  dt_ntype_e ntype;
  void *in1 = NULL;
  void *in2 = NULL;
  unsigned long have = stats->have;
  unsigned long want = stats->want;
  dtxy_t y;
  dt_rc_e rc;

  rc = dtimage_check_1f(ctl,			/* check simple stuff				*/
    input1, "input1",
    ntypes, DTIMAGE_FLAG_PAGED, F);
  if (rc != DT_RC_GOOD)
    return rc;

  rc = dtimage_check_1f(ctl,			/* check simple stuff				*/
    input2, "input2",
    ntypes, DTIMAGE_FLAG_PAGED, F);
  if (rc != DT_RC_GOOD)
    return rc;

  xmax = DT_MIN(
    input1->xe, input2->xe);

  ymax = DT_MIN(
    input1->ye, input2->ye);

  DT_Q(dtntype_promote,(ctl,			/* find common ntype 				*/
    input1->ntype, input2->ntype,
    &ntype, NULL, 0));

  if (input1->ntype != ntype)
    DT_Q(dtos_malloc2,(ctl,				/* space for promoted row 			*/
      (void **)&in1,
      (long)(dt_ntype_sizeof[ntype] * xmax),
      F, "promoted in1 row"));

  if (input2->ntype != ntype)
    DT_Q(dtos_malloc2,(ctl,				/* space for promoted row 			*/
      (void **)&in2,
      (long)(dt_ntype_sizeof[ntype] * xmax),
      F, "promoted in2 row"));

  want &= ~have;						/* only want what we don't have 	*/
  if (want & DTSTATS_CMP)
  {
	stats->sign = 0;
    have |= DTSTATS_CMP;
  }
  if (want & DTSTATS_NLESS)
  {
	stats->nless = 0;
	stats->area = 0;
    have |= DTSTATS_NLESS | DTSTATS_AREA;
  }
  if (want & DTSTATS_NMORE)
  {
	stats->nmore = 0;
	stats->area = 0;
    have |= DTSTATS_NMORE | DTSTATS_AREA;
  }
  
  for (y=0; y<ymax && want; y++)		/* for each row 					*/
  {
	void *in1p, *in2p;

    DTIMAGE_GETROW2(DT_C, ctl,			/* access input row 				*/
      input1, y, &in1p);
	if (in1 != NULL)
      DT_C_1D(dt1d_cast,(				/* promote to intermediate type 	*/
        in1p, xmax, input1->ntype,
        in1, xmax, ntype));

    DTIMAGE_GETROW2(DT_C, ctl,			/* access input row 				*/
      input2, y, &in2p);
	if (in2 != NULL)
      DT_C_1D(dt1d_cast,(				/* promote to intermediate type 	*/
        in2p, xmax, input2->ntype,
        in2, xmax, ntype));

										/* ................................ */
	if (want & DTSTATS_CMP)				/* comparing images? 				*/
	{
	  long cmp;
	  DT_C_1D(dt1d_cmp,(ntype,			/* compare row 						*/
        in1? in1: in1p, xmax,
		in2? in2: in2p, xmax, 
        constant, &cmp));
	  if (cmp != 0)						/* images differed? 				*/
	  {
		stats->x0 = (dtxy_t)((cmp < 0? 
          -cmp: cmp) - 1);
		stats->y0 = y;
		stats->sign = cmp < 0? -1: 1;
		want &= ~DTSTATS_CMP;			/* only want first difference 		*/
	  }
	}
										/* ................................ */
	if (want & DTSTATS_NLESS ||			/* counting pixels which differ? 	*/
		want & DTSTATS_NMORE)
	{
	  long nless = 0;
	  long nmore = 0;
	  DT_C_1D(dt1d_diff,(ntype,			/* count differences on row 		*/
        in1? in1: in1p, xmax,
		in2? in2: in2p, xmax, 
        constant, &nless, &nmore));
	  stats->area += xmax;
	  stats->nless += nless;
	  stats->nmore += nmore;
	}
										/* ................................ */

    DTIMAGE_UNMAPROW2(DT_C, ctl, 
      input2, y);
    DTIMAGE_UNMAPROW2(DT_C, ctl, 
      input1, y);
  }

  stats->have = have;

cleanup:
  if (in2 != NULL)
    DT_I(dtos_free2,(ctl, in2,
       F, "promoted in2 row"));
  if (in1 != NULL)
    DT_I(dtos_free2,(ctl, in1,
       F, "promoted in1 row"));

  return rc;
}

/*..........................................................................
 * mean, sum, area
 *..........................................................................*/

static
dt_rc_e
dtimage_stats_moment1(					/* mean, sum, area 					*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtimage_t *image,						/* image of gray level values 		*/
  dtstats_t *stats)						/* return statistics 				*/
{
  DT_F("dtimage_stats_moment1");

  DT_Q(dtimage_sum,(ctl, image,			/* compute image sum 				*/
    &stats->sum));
  
  stats->area =							/* compute image area 				*/
    (double)image->xe *
    (double)image->ye;

  stats->have |=						/* mark what we have 				*/
    DTSTATS_SUM |
	DTSTATS_AREA;

  if (stats->area > 0.0)				/* there is some area? 				*/
  {
    stats->mean = stats->sum /			/* compute image mean 				*/
      stats->area;
    stats->have |=						/* mark what we have 				*/
	  DTSTATS_MEAN;
  }

  return DT_RC_GOOD;
}

/*..........................................................................
 * variance
 *..........................................................................*/

static
dt_rc_e
dtimage_stats_variance(					/* variance							*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtimage_t *image,						/* image of gray level values 		*/
  dtstats_t *stats)						/* return statistics 				*/
{
  DT_F("dtimage_stats_variance");
  double sumsq;

  DT_Q(dtimage_sumsq_scalar,(ctl,		/* sum (pixels minus mean) squared  */
    image, stats->mean, &sumsq));				

  if (stats->area > 1)
	 stats->variance = sumsq / 
       (stats->area - 1.0);
  else
	stats->variance = 0.0;

  stats->standard_deviation =			/* stddev is a freebie 				*/
    sqrt(stats->variance);
  
  stats->have |=
	DTSTATS_VARIANCE |
	DTSTATS_STANDARD_DEVIATION;

  return DT_RC_GOOD;
}

/*..........................................................................
 * range
 *..........................................................................*/

static
dt_rc_e
dtimage_stats_range(					/* range 							*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtimage_t *image,						/* image of gray level values 		*/
  dtstats_t *stats)					/* return statistics 				*/
{
  DT_F("dtimage_stats_range");
  dtxy_t y;
  dt_rc_e rc;

  rc = dtimage_check_1f(ctl,			/* check simple stuff				*/
    image, "image",
    dt1d_ntype_list, 
    DTIMAGE_FLAG_PAGED, F);
  if (rc != DT_RC_GOOD)
    return rc;

  for (y=0; y<image->ye; y++)
  {
	dtxy_t imin, imax;
    void *p;
    double v;
	DTIMAGE_GETROW(ctl, image, y, &p);	/* map row into memory 				*/

    DT_Q_1D(dt1d_range,(				/* get extrema on row 				*/
      image->ntype, p, image->xe,
      &imin, &imax));

	DTIMAGE_GETPIX1(DT_Q, ctl, image,	/* check smallest pixel				*/
      imin, y, &v);
	if (y == 0 || v < stats->minval)
	{
      stats->minx = imin;
      stats->miny = y;
	  stats->minval = v;
	}
	DTIMAGE_GETPIX1(DT_Q, ctl, image,	/* check largest pixel 				*/
      imax, y, &v);
	if (y == 0 || v > stats->maxval)
	{
      stats->maxx = imax;
      stats->maxy = y;
	  stats->maxval = v;
	}
	DTIMAGE_UNMAPROW(ctl, image, y);	/* release row 						*/
  }

  stats->have |=
	DTSTATS_MINX |
	DTSTATS_MINY |
	DTSTATS_MAXX |
	DTSTATS_MAXY |
	DTSTATS_MINVAL |
	DTSTATS_MAXVAL;

  return DT_RC_GOOD;
}

/*..........................................................................
| NAME
| dtimage_stats() - compute blob statistics
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
dtimage_stats(							/* compute blob statistics 			*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtimage_t *image,						/* image to compute stats on		*/
  dtstats_t *stats)					/* return statistics 				*/
{
  DT_F("dtimage_stats");
  unsigned long want = stats->want;

  if (want & DTSTATS_STANDARD_DEVIATION)
	want |= DTSTATS_VARIANCE;

  if (want & DTSTATS_VARIANCE)			/* variance implies... 				*/
	want |= DTSTATS_MEAN |
	        DTSTATS_AREA;
										/* ................................ */
  want &= ~stats->have;
  if (want & DTSTATS_SUM      ||		/* want first moment stuff?			*/
      want & DTSTATS_AREA     ||
	  want & DTSTATS_MEAN)
  {
	DT_Q(dtimage_stats_moment1,(ctl,
      image, stats));
  } 
										/* ................................ */
  want &= ~stats->have;
  if (want & DTSTATS_VARIANCE)			/* want variance? 					*/
  {
	DT_Q(dtimage_stats_variance,(ctl,
      image, stats));
  } 
										/* ................................ */
  want &= ~stats->have;
  if (want & DTSTATS_MINVAL ||			/* want range?						*/
	  want & DTSTATS_MAXVAL)
  {
	DT_Q(dtimage_stats_range,(ctl,
      image, stats));
  }
										/* ................................ */

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
