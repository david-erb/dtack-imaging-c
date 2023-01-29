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
DT_RCSID("alg/lev85 $RCSfile: combine.c,v $ $Revision: 1.9 $");
#include <dtlev85.h>

/*..........................................................................
| NAME
| dtlev85_combine() - combine uniformity of two areas
| 
| DESCRIPTION
| dtlev85_combine() combines the structures describing the uniformity
| of two areas into a single area.
|_index dtlev85 structures,, combining
|_index combine,, two dtlev85 structures
|_index areas,, combine two
|
| The ~a2~ structure is changed such that it represents
| the accumulation of all regions which have contributed to
| both ~a0~ and ~a1~.
| 
| ~a2~ may point to the same structure as either ~a1~ or ~a0~.
| This function does not require that the ~a2~ structure has been 
| initialized by dtlev85_init().
| However, if it is subsequently to be used in a call to
| dtlev85_contribute(), then pre-initialization is necessary.
| 
| This function is very cheap compared to a full connectivity
|_aside This function is cheap.
| analysis on a segmented image.
| 
| In a dynamic environment, the set of regions composing an area
| will be continually changing.
|_aside Use when regions are continually grouped and regrouped.
| The well-known split/merge technique is such an environment.
| This function is designed to be used in such a dynamic environment.
| Consider the naive case in which each area consists of a single region.
| As part of the region-finding process, the intrinsic uniformity
| of each region is accumulated.
| This expensive process need not be repeated, even as the regions 
| are merged in various ways in the search for the "best" segmentation.
| Instead, the much cheaper dtlev85_combine() is used.
|
| The following steps illustrate one approach:
| 1.	initialize a set of lev85 structures,
|		one for each region (dtlev85_init())
| 2.	for each region, do connected components analysis
|		and call dtlev85_blob(),
|		(or, alternatively, compute the region's histogram and call
|		dtlev85_hist())
|		giving the lev85 structure associated with that region
| 3.	merge regions into areas as desired,
|		calling dtlev85_combine() for each region in the area
| 4.	call dtlev85_compute() to compute the uniformity of the areas
| 5.	repeat steps 3 and 4 until the desired segmentation goodness
|		criteria have been reached
| 
| After step 2, there is no need for further expensive connectivity analysis
| unless the regions themselves change.
| 
| PARAMETERS
|dtinclude(ctlonly.dx)
|
| RETURN VALUES
| The contents of ~a2~ structure are set to reflect
| the combined contribution of the other two areas.
|dtinclude(return1.dx)
| 
| ERRORS
|dtinclude(noerror.dx)
| 
| CAVEATS
| Attempting to use a ~lev85~ structure without first initializing it
| may result in incorrect results.
|
| SEE ALSO
| dtlev85_init()			initializes lev85 structure
| dtlev85_hist()	contributes a region's histogram to the area
| dtlev85_blob()	contributes a region's pixels directly to the area
| dtlev85_compute()			computes uniformity for the area
|
| EXAMPLE
| This example code fragment illustrates dtlev85_combine()
| used in conjunction with dtlev85_hist().
| It may also be used with dtlev85_blob().
| 
|:  dtlev85_t lev[4];
|:  ...
|:  dtlev85_init(NULL, lev, 4,			// initialize four lev85 structures
|:    &gray);
|:
|:  [compute first region's histogram]
|:  dtlev85_contribute(NULL, &lev[1]);	// contribute first region
|:
|:  [compute second region's histogram]
|:  dtlev85_contribute(NULL,  &lev[2]);	// contribute second region
|:
|:  [compute third region's histogram]
|:  dtlev85_contribute(NULL, &lev[3]);	// contribute third region
|:
|:  dtlev85_combine(&lev[1], &lev[2],	// combine first two regions
|:    &lev[0]);
|:  dtlev85_combine(&lev[0], &lev[3],	// combine third region with first two
|:    &lev[0]);
|:  dtlev85_compute(NULL, &lev[0]);		// compute final uniformity
|:  printf("uniformity %e\n",			// print the uniformity
|:    lev[0].u);	
|
| END
 *..........................................................................*/

dt_rc_e
dtlev85_combine(						/* combine uniformity of two areas	*/
  dtparam_t *param,						/* control parameter structure 		*/
  dtlev85_t *lev85_in1,					/* first area 						*/
  dtlev85_t *lev85_in2,					/* second area 						*/
  dtlev85_t *lev85_out)					/* output combined area 			*/
{
  lev85_out->Aa = lev85_in1->Aa + lev85_in2->Aa;
  lev85_out->Ua = lev85_in1->Ua + lev85_in2->Ua;
  if (lev85_in1->n > 0 &&
	  lev85_in2->n > 0)
  {
    lev85_out->fmin = DT_MIN(lev85_in1->fmin, lev85_in2->fmin);
    lev85_out->fmax = DT_MAX(lev85_in1->fmax, lev85_in2->fmax);
  }
  else
  if (lev85_in1->n > 0)
  {
	lev85_out->fmin = lev85_in1->fmin;
	lev85_out->fmax = lev85_in1->fmax;
  }
  else
  {
	lev85_out->fmin = lev85_in2->fmin;
	lev85_out->fmax = lev85_in2->fmax;
  }
  lev85_out->n  = lev85_in1->n  + lev85_in2->n;
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
