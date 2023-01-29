/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
  ! Copyright (C) 1998 Dtack Inc. All Rights Reserved
  ! To use this file, you must have signed a license agreement with Dtack Inc.
  ! Under no circumstances may you redistribute this file.
  ! This software is provided AS IS with no warranty expressed or implied.
  ! Dtack Inc. accepts no liability for use or misuse of this file.
  ! http://www.dtack.com  dtack@dtack.com  telephone +360.417.6700
  ! Dtack Inc, P.O. Box 3061, Port Angeles, WA USA 98362
  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */




#include <dtack/base.h>
DT_RCSID("alg/lev85 $RCSfile: blob.c,v $ $Revision: 1.9 $");

#include <dtlev85.h>
#include <dtack/stack.h>
#include <dtack/mem.h>

/*..........................................................................
| NAME
| dtlev85_blob() - contribute blob to uniformity
| 
| DESCRIPTION
| dtlev85_blob() accumulates one region's contribution into
| the uniformity structure.
| The geometric locations of the region's pixels
| are given by a linked list of runs.
|_index dtlev85 structure,, contribution of a region to
|_index contribution,, of a region to dtlev85 structure
|_index region,, contribution to a dtlev85 structure
|_index blob,, contribution to a dtlev85 structure
|
| The ~lev85~ structure is updated to include the variance
| of the region described by the runs
| stored in ~run~ and linked by ~blob~.
| ~gray~ is the original gray level image
|_aside Uniformity itself not computed here.
| The uniformity itself is not computed by this function.
| Instead it is deferred until dtlev85_compute() is called.
| Any previously known statistics about ~blob~ are stored in ~stats~,
| and any statistics computed by this function are returned
| in the same structure.
|
| The ~run~ and ~blob~ structures are normally provided
|_aside Base Library does connect components.
| by the Dtack Base Library connected components mechanism.
| The starting point for this mechanism is usually dtimage_adj().
| It takes a labeled image and extracts
| regions whose pixels have the same value and are
| maximally 4- or 8-connected.
| For each such region, a callback is made giving both ~run~ and ~blob~.
| dtlev85_blob() could be called from such a callback.
|
| The ~blob~ structure provides pixel location information only.
| This function accesses the pixels in the ~gray~ image at these locations
| during uniformity computation.
| 
| The ~stats~ structure may be used to promote program efficiency.
|_Aside Avoid recomputation.
| It is often the case that an application
| must make several different computations on the same blob.
| If more than one computation requires a same statistic about the blob,
| it makes sense to compute that statistic only once.
| For example, the area of the blob is a statistic
| needed by the uniformity computation.
| But it may also be needed by some application-dependent computation
| such as a shape factor.
| If the uniformity is computed first,
| then the area is already known and stored in ~stats~.
| The shape factor algorithm need only access the number ~stats->area~
| instead of recomputing it.
| If they have not already been computed,
| the following members of ~stats~ are computed by this function:
| 
| #stats->have / stats->want#	#stats structure member#
| :DT_STATS_MINVAL:				~stats->minval~
| :DT_STATS_MAXVAL:				~stats->maxval~
| :DT_STATS_AREA:				~stats->area~
| :DT_STATS_MEAN:				~stats->mean~
| :DT_STATS_VARIANCE:			~stats->variance~
|
| This function is designed to be called once for each region in an area
|_aside Call this function repeatedly.
| before the actual uniformity of the area is computed.
| The steps to computing the uniformity of a multiple-region area are:
| 1.	initialize structure (dtlev85_init())
| 2.	using connectivity analysis, create the first blob structure
| 3.	accumulate blob's contribution (dtlev85_blob())
| 4.	create next blob structure
| 5.	accumulate region's contribution (dtlev85_blob() again)
| 6.	repeat steps 4 and 5 for each remaining region
| 7.	compute final uniformity (dtlev85_compute())
| 
| Being able to accumulate several regions before
| computing the final uniformity is important
| in many dynamic applications.
| Often the region set remains the same,
| but they are combined and re-combined
| in different ways as the search for the optimum areas progresses.
| 
| An alternative means of contributing to the uniformity of an area
|_aside An alternative method.
| is through the use of the function dtlev85_hist().
| Its advantages are that it may be somewhat simpler and faster
| if you have an easy means of calculating histograms for your regions.
| You do not have to deal with a blob structure.
| However, if your gray level image is of a data type
| which does not lend itself to binning the precision loss,
| then the blob approach may be the only acceptable approach.
|
| The two uniformity contribution functions, dtlev85_hist()
|_aside OK to mix methods.
| and dtlev85_blob(),
| each contribute exactly one region each time they are called.
| Since both functions use the ~lev85~ structure in the same way,
| calls to the functions may be intermixed.
| A call to dtlev85_compute() after all contributions have been made
| will compute the correct answer.
|
| Although you can compute the ~blob~ structure of your regions in any
|_aside Blob within connected component analysis.
| convenient way,
| the Dtack Imaging Software Base Library
| provides a convenient means to do it within the framework
| of connected components analysis.
| To use this mechanism, you write a callback routine which gets
| invoked with the blob structure defining one region.
| Within your callback routine, you may perform any computation you desire.
| This includes passing the blob structure to dtlev85_blob().
|_aside See the example program.
| For a working illustration of this exact mechanism,
| please refer to
|_hyper section, Example lev85d.c
|
| RETURN VALUES
| The contents of the dtlev85 structure are updated to include
| the contribution of the region.
|dtinclude(return1.dx)
| 
| ERRORS
| Unsuccessful completion may be due to:
| - error computing blob statistics
| 
| CAVEATS
| Attempting to use a dtlev85 structure without first initializing it
| may result in incorrect results.
|
| This function does not recompute ~stats~ members
| which are already marked in ~stats->have~.
| If the contents of ~stats~ are undefined,
| be sure to set ~stats->have~ to 0.
| 
| SEE ALSO
| dtlev85_init()	initializes lev85 structure
| dtlev85_hist()	contribute to uniformity via histogram
| dtlev85_compute()	computes uniformity for the area
| dtlev85_combine()	combines two areas into a single area
|
| EXAMPLE
| For a complete and working example program, please see the
|_hyper section, Example lev85d.c
| 
| The following code illustrates a simple function designed
| to be the callback for dtimage_adj_blob().
| 
|:dt_rc_e
|:mydeliver(	
|:  dt_ctl_t *ctl,
|:  dtrun_t *run,						// connected components housekeeping
|:  dtrun_blob_t *blob,
|:  void *arg)
|:{
|:  dtlev85_t lev85;
|:  dt_stats_t stats;
|:
|:  dtlev85_init(NULL, &lev85);			// init lev85 structure
|:  stats.have = 0;						// we know no stats yet
|:  dtlev85_blob(NULL,          		// simply add blob to it
|:    &lev85, &gray, run, blob,
|:    &stats);
|:  dtlev85_compute(NULL, &lev85);		// compute blob's uniformity		
|:  printf("dtlev85d:  blob"			// print out interesting stuff
|:    " label %6g, "
|:    " area %6ld, "
|:    " uniformity %0.5f\n",
|:	  blob->label, lev85.Aa,
|:    lev85.u);
|:  return DT_RC_GOOD;					// keep doing blobs				    
|:}
|
| END
 *..........................................................................*/

dt_rc_e
dtlev85_blob(	
  dtparam_t *param,
  dtlev85_t *lev85,
  dtimage_t *gray,
  dtrun_t *run,
  dtrun_blob_t *blob,
  dt_stats_t *stats)
{
  DT_F("dtlev85_blob");
  dt_ctl_t *ctl = param?				/* get environment from param 		*/
    &param->ctl: NULL;

  stats->want |= 						/*[Mark the stats we want to be    ]*/
    DT_STATS_MINVAL    |				/*[able to compute uniformity.	   ]*/
    DT_STATS_MAXVAL    |
    DT_STATS_AREA      |
    DT_STATS_MEAN      |
    DT_STATS_VARIANCE;

  DT_Q(dtimage_blob_stats,(ctl, gray,	/*[Compute those stats. 		   ]*/
    run, blob, stats));

  if (DT_DBG(ctl, DT_DBG_MASK_OBJECT))
    dt_dbg(ctl, F, DT_DBG_MASK_OBJECT,
      "Aj %6ld, fbar %g, varj %g",
      stats->area, stats->mean, 
      stats->variance);

  if (lev85->n == 0)					/*+If this is the first region in  ]*/
  {										/*[this area,					   ]*/
    lev85->fmin = stats->minval;		/*[start keeping absolute gray 	   ]*/
    lev85->fmax = stats->maxval;		/*[level min and max.			   ]*/
  }
  else									/*[If not the first region,        ]*/
  {
    lev85->fmin = DT_MIN(				/*[compare this region's gray level]*/
      stats->minval, lev85->fmin);		/*[min and max to the absolute	   ]*/
    lev85->fmax = DT_MAX(				/*[and keep the extremes.		   ]*/
      stats->maxval, lev85->fmax);
  }

										/*+In the lev85 structure,		   ]*/
  lev85->n++;							/*[keep count of regions,		   ]*/
  lev85->Aa += stats->area;				/*[sum of total area, and		   ]*/
  lev85->Ua += stats->variance *		/*[region contribution to		   ]*/
    stats->area;						/*[uniformity.					   ]*/
										/* Aj in variance denominator of	*/
										/*   eqn (2) p.158 is cancelled by 	*/
										/*   Aj weight in eqn (9). 			*/

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
