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

#include <dtack/config.h>				/* platform-specific defines		*/
#include <dtack/ntype.h>
#include <dtack/stats.h>
#include <dtack/1d.h>

/*..........................................................................
 * mean, sum, area
 *..........................................................................*/

static
dt1d_rc_e
dt1d_stats_moment1(						/* mean, sum, area 					*/
  dt1d_ntype_e ntype,					/* type of elements 				*/
  void *in,								/* input array	 					*/
  dt1d_i_t nin,							/* number of elements 				*/
  dtstats_t *stats)						/* return statistics 				*/
{
  dt1d_sum(ntype, in, nin,				/* compute image sum 				*/
    &stats->sum);
  
  stats->area =	nin;					/* compute image area 				*/

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
  
  return DT1D_RC_GOOD;
}

/*..........................................................................
 * variance
 *..........................................................................*/

static
dt1d_rc_e
dt1d_stats_variance(					/* variance							*/
  dt1d_ntype_e ntype,					/* type of elements 				*/
  void *in,								/* input array	 					*/
  dt1d_i_t nin,							/* number of elements 				*/
  dtstats_t *stats)						/* return statistics 				*/
{
  double sumsq;

  dt1d_sumsq_scalar(					/* sum (pixels minus mean) squared  */
    ntype, in, nin, stats->mean,
    &sumsq);				

  if (stats->area > 1)
	 stats->variance = sumsq / 
       (stats->area - 1.0);
  else
	stats->variance = 0.0;

  stats->have |=
	DTSTATS_VARIANCE;

  return DT1D_RC_GOOD;
}

/*..........................................................................
 * range
 *..........................................................................*/

static
dt1d_rc_e
dt1d_stats_range(						/* range 							*/
  dt1d_ntype_e ntype,					/* type of elements 				*/
  void *in,								/* input array	 					*/
  dt1d_i_t nin,							/* number of elements 				*/
  dtstats_t *stats)						/* return statistics 				*/
{
  int y;
  dt1d_i_t imin;
  dt1d_i_t imax;
  double vmin = 0.0;
  double vmax = 0.0;
  dt1d_rc_e rc;

  rc = dt1d_range(						/* get extrema on row 				*/
    ntype, in, nin,
    &imin, &imax);
  if (rc != DT1D_RC_GOOD)
	return rc;

  DTNTYPE_GET1(dummy, dummy,
    in, imin, ntype, 
    &stats->minval);

  stats->minx = imin;
  stats->miny = 0;

  DTNTYPE_GET1(dummy, dummy,
    in, imax, ntype, 
    &stats->maxval);

  stats->maxx = imax;
  stats->maxy = 0;

  stats->have |=
	DTSTATS_MINX |
	DTSTATS_MINY |
	DTSTATS_MAXX |
	DTSTATS_MAXY |
	DTSTATS_MINVAL |
	DTSTATS_MAXVAL;

  return DT1D_RC_GOOD;
}

/*..........................................................................
| NAME
| dt1d_stats() - compute blob statistics
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

dt1d_rc_e
DT1D_API
dt1d_stats(								/* compute blob statistics 			*/
  dt1d_ntype_e ntype,					/* type of elements 				*/
  void *in,								/* input array	 					*/
  dt1d_i_t nin,							/* number of elements 				*/
  dtstats_t *stats)					/* return statistics 				*/
{
  unsigned long want = stats->want;
  dt1d_rc_e rc;

  if (want & DTSTATS_VARIANCE)			/* variance implies... 				*/
	want |= DTSTATS_MEAN |
	        DTSTATS_AREA;
										/* ................................ */
  want &= ~stats->have;
  if (want & DTSTATS_SUM      ||		/* want first moment stuff?			*/
      want & DTSTATS_AREA     ||
	  want & DTSTATS_MEAN)
  {
	rc = dt1d_stats_moment1(
      ntype, in, nin, stats);
	if (rc != DT1D_RC_GOOD)
	  return rc;
  } 
										/* ................................ */
  want &= ~stats->have;
  if (want & DTSTATS_VARIANCE)			/* want variance? 					*/
  {
	rc = dt1d_stats_variance(
      ntype, in, nin, stats);
	if (rc != DT1D_RC_GOOD)
	  return rc;
  } 
										/* ................................ */
  want &= ~stats->have;
  if (want & DTSTATS_MINVAL ||			/* want range?						*/
	  want & DTSTATS_MAXVAL)
  {
	rc = dt1d_stats_range(
      ntype, in, nin, stats);
	if (rc != DT1D_RC_GOOD)
	  return rc;
  }
										/* ................................ */

  return DT1D_RC_GOOD;
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
