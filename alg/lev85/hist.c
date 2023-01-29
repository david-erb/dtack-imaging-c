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
DT_RCSID("alg/lev85 $RCSfile: hist.c,v $ $Revision: 1.9 $");
#include <dtlev85.h>

/*..........................................................................
| NAME
| dtlev85_hist() - contribute region to uniformity
| 
| DESCRIPTION
| dtlev85_hist() accumulates one region's contribution into
| the uniformity structure.
| The region is completely characterized by its histogram.
|_index dtlev85 structure,, contribution of a region to
|_index contribution,, of a region to dtlev85 structure
|_index region,, contribution to a dtlev85 structure
|_index histogram,, contribution to a dtlev85 structure
|_index histogram,, loss of precision
|
| The ~lev85~ structure is updated to include the variance
| of the region described by the histogram ~hist~.
|_aside Uniformity itself not computed here.
| The uniformity itself is not computed by this function.
| Instead it is deferred until dtlev85_compute() is called.
| 
| For the purposes of this function, the region to be contributed
|_aside You compute histogram of region.
| is completely described by its histogram ~hist~.
| This histogram may have been computed in any manner desired.
|
| This function is designed to be called once for each region in an area
|_aside Call this function repeatedly.
| before the actual uniformity of the area is computed.
| The steps to computing the uniformity of a multiple-region area are:
| 1.	initialize structure (dtlev85_init())
| 2.	compute histogram of first region
| 3.	accumulate region's contribution (dtlev85_hist())
| 4.	compute histogram of next region
| 5.	accumulate region's contribution (dtlev85_hist() again)
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
| If the histogramming operation has involved binning,
| the subsequent loss of precision may affect the accuracy
| of the uniformity.
| As a result, dtlev85_hist() is normally only used
|_aside Works best for 8-bit images.
| when the gray level image type is of low enough precision
| that the histogram bins correspond one-to-one with the
| data values.
| For example, an 8-bit image can be represented by a histogram bin
| array of only 256 values without loss of precision.
| However, a 32-bit image would require too much memory
| to give every possible pixel value its own histogram bin.
| Pixels from such an image would normally be linearly down-scaled
| to reduce the number of bins required.
| This down-scaling process is referred to as binning.
| It reduces the precision of the input image
| thereby possibly adversely affecting the accuracy of the uniformity.
| 
| The gray level used in uniformity computation is the floor (lowest value)
|_aside Possible loss of accuracy.
| of the histogram bin.
| For histograms whose scale value is 1.0 
| (i.e. no precision loss during binning)
| this is the exact value of the original gray level pixels.
| However, the value is slightly lower than the correct value
| when the binning scale value is not 1.0.
| For example, consider gray level pixels ranging from 0 up to 1000
| which have been binned into 100 bins.
| Then pixels in the range from 50.0 to but not including 60.0
| will have been binned into bin 5.
| This range of values has the theoretical average value of 55.0.
| However, in the uniformity computation the frequency count in the bin
| will be used as though all pixels had the value of exactly 50.0.
| 
| An alternative means of contributing to the uniformity of an area
|_aside An alternative method.
| is through the use of the function dtlev85_blob().
| This function does not suffer from the binning problem just described.
| Results are accurate for all data types.
| You make use use the connected components mechanism
| to construct a linked list of runs as the required input data structure.
| The linked list is passed to dtlev85_blob().
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
| RETURN VALUES
| The contents of the dtlev85 structure are updated to include
| the contribution of the region.
|dtinclude(return1.dx)
| 
| ERRORS
| Unsuccessful completion may be due to:
| - ~hist~ structure is corrupt
| 
| CAVEATS
| Attempting to use a dtlev85 structure without first initializing it
| may result in incorrect results.
|
| If ~hist~ has been allocated such that its number of bins
| is less than the integral value of its data range,
| then loss of precision in the resulting uniformity will occur.
| 
| SEE ALSO
| dtlev85_init()	initializes lev85 structure
| dtlev85_blob()	contribute to uniformity via direct blobs
| dtlev85_compute()	computes uniformity for the area
| dtlev85_combine()	combines two areas into a single area
|
| EXAMPLE
| For an example using dtlev85_hist(), please refer to the
|_hyper dtlev85_compute()
| section.
|
| END
 *..........................................................................*/

dt_rc_e
dtlev85_hist(							/* contribute region to uniformity	*/
  dtparam_t *param,						/* control parameter structure 		*/
  dtlev85_t *lev85,						/* algorithm housekeeping 			*/
  dthist_t *hist)						/* pre-computed histogram 			*/
{
  DT_F("dtlev85_hist");
  dt_ctl_t *ctl = param?				/* get environment from param 		*/
    &param->ctl: NULL;
  long Aj;
  double sum;
  double g;
  double fbar, varj;
  const dthist_bin_t w =
    hist->width;
  const dthist_freq_t * const h =
    hist->freq;
  dthist_bin_t bin;

  sum = 0.0;
  Aj = 0;
  for (bin=0; bin<w; bin++)
  if (h[bin] != 0)
  {
    Aj += h[bin];						/*[Get area and					   ]*/
    DTHIST_BIN_TO_MINVALUE(DT_Q, ctl,
      hist, bin, &g);
    sum += h[bin] * g;					/*[sum of gray levels in histogram.]*/
  }

  for (bin=0; bin<w-1; bin++)			/*+Scan for histogram minimum	   ]*/
    if (h[bin] != 0) break;
  DTHIST_BIN_TO_MINVALUE(DT_Q, ctl,
    hist, bin, &g);
  if (lev85->n == 0)				
    lev85->fmin = g;
  else
    lev85->fmin = DT_MIN(
      lev85->fmin, g);

  for (bin=w-1; bin>1; bin--)			/*[and maximum gray level.  	   ]*/
    if (h[bin] != 0) break;
  DTHIST_BIN_TO_MINVALUE(DT_Q, ctl,
    hist, bin, &g);
  if (lev85->n == 0)				
    lev85->fmax = g;
  else
    lev85->fmax = DT_MAX(
      lev85->fmax, g);

  fbar = sum / (double)Aj;				/*[Get mean gray level.			   ]*/

  varj = 0.0;
  for (bin=0; bin<w; bin++)
  if (h[bin] != 0)
  {
	DTHIST_BIN_TO_MINVALUE(DT_Q, ctl,
      hist, bin, &g);
    varj += (double)h[bin] *			/*+Sum variance numerator.		   ]*/
      ((double)g-fbar) *
      ((double)g-fbar);
  }

  dt_dbg(ctl, F, DT_DBG_MASK_OBJECT,	/*+Do some debug for this	 	   ]*/
    "Aj %6ld, fbar %5.1f, varj %8.2e",	/*[contribution.				   ]*/
    Aj, fbar, varj/Aj);

  lev85->n++;							/*+Accumulate count of regions,	   ]*/
  lev85->Aa += Aj;						/*[sum of total area, and	       ]*/
  lev85->Ua += varj;					/*[sum of variance numerators.	   ]*/
										/*[Aj in variance denominator of   ]*/
										/*[eqn (2) p.158 is cancelled by   ]*/
										/*[Aj weight in eqn (9). 		   ]*/
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
