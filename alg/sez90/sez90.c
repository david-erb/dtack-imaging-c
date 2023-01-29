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
| TITLE
| Algorithm: dt_sez90
| 
|!Abstract
| This is an implementation of an automatic peak detection algorithm.
| The algorithm is due to Sezan (1990).
| It is described completely in [sez90].
| Input is a one-dimensional array,
| most commonly the histogram of an image.
| The implementation provides a description of each peak by its
| start point, end point, and maximum point.
| The peak start points are prime candidates for thresholds.
| Two input parameters control the sensitivity of the peak detection and
| the number of quantization levels.
| The implementation creates a list of peaks or
| extracts peaks one by one.
| Utility routines plot boxes and lines to illustrate peak extents.
| A simple main program exercises the routines.
|
|!Overview
|_figure half, toucan.ps, Source Image
|_figure half, toucan-multi.ps, Multithresholded Image
|_figure half, toucan-sez90.ps, Histogram with Thresholds
|
|.~What are the goals of the algorithm?~
| The algorithm locates the peaks in a histogram.
| The peaks give a set of global thresholds 
| which may be used for segmentation or data reduction. 
| The algorithm is computationally simple.
| It has adjustable sensitivity.
| It is designed to require a minimum of parameter input.
| 
|.~How would you use the algorithm?~
| First you make the one-dimensional array containing the peaks of interest.
| This will typically be just the histogram of your image.
| You then pass this array to the implementation.
| You get back a list of peaks.
| Each peak has a start, a maximum point, and an end.
| If you don't get the number of peaks you want, 
| you adjust the sensitivity parameters and call the implementation again.
| Finally, you use the list of peaks
| to globally theshold your image.
|
|.~What does the output look like?~
| [fig:toucan-multi.ps] is what you get if you use the peaks
| from [fig:toucan.ps] to select thresholds.
| 
|.~What should you be aware of?~
| It is a global thresholding approach.
| To give usable output,
| the algorithm depends on the input array having clear peaks.
| You will have to determine suitable parameter values 
| which work for your application.
| 
|.~What is the interface to the implementation?~
| The implementation has a run-ready main program and a C-callable library.
| The main program has a number of command-line options.
| It uses the library to do all its tasks.
| It will read an input image,
| make its histogram,
| find and print the peaks,
| plot boxes around the peaks, and
| produce a multithresholded output image.
| 
|.~What inputs do the C-callable library routines require?~
| You must provide a read-only array of double-precision numbers.
| You must choose a value for the sensitivy N which controls
| the size of the neighborhood in which peaks are considered distinct.
| You must also choose a closeness value which controls the merging of
| adjacent peaks.
| There are reasonable defaults for both input parameters.
| The plotting routines require a pre-allocated image to draw in.
| 
|.~What do the library routines output?~
| One routine outputs the Peak Detection Signal (PDS) which contains
| zero-crossings indicating peak starts.
| Another routine produces the next peak in the PDS given a starting point.
| A third is a convenience routine which uses the two above to
| give just a list of the peak starts, maxima, and ends in three arrays.
| Two other routines draw boxes around the peaks and highlight the
| peak maxima.
| 
|.~Does the implementation test itself?~
| Yes the implementation contains an extensive self-test suite.
| Successful execution of the test suite indicates the correctness
| of the implementation to a high degree of confidence.
| 
|.~Can you modify it?~
| The distribution comes with complete, well-commented source code.
| You are free to modify the code to suit your application.
| (Please be aware that Dtack's code, modified or not, is subject
| to the redistrubtion restrictions of the License Agreement.)
| 
|.~What documentation exists for it?~
| -	Overview which you are reading 
| -	Abstract
| -	Introduction
| -	Guide
| -	Reference
| 
|.~How do you install it?~
| Installation requires the Dtack Imaging Software base libraries.
| All installation is done in a single directory by a simple script.
| To use the software after installation, only the following are required:
| :dt_sez90.h:		header file
| :libdt_sez90.a:	library
| :dt_sez90:		main program executable 
|					(not required if you write your own program)
| (Actual names may vary slightly depending on operating system.)
|
|!Algorithm Review
| This section is a review of the algorithm presented in [sez90].
|
| The motivation of the article is to find an automated method 
| to reduce the number of gray levels needed to store an image.
| The approach is to first determine a small number of gray-level thresholds.
| Then, by mapping all gray levels between two adjacent thresholds
| to a single class, data reduction is achieved.
| The algorithm finds this small number of gray-level thresholds by
| locating peaks in the histogram.
| Ideal candidates for gray-level thresholds are gray levels
| falling in deep valleys between the peaks.
| 
| Although the application described in the article is oriented
| to image quantization,
| the technique also has application to global thresholding
| and multi-thresholding.
| The article references [sez89] as an example.
| In fact,
| the algorithm is applicable to the entire class of
| peak detection problems in which a one-dimensional input signal
| is bimodal or multimodal.
| 
| The stated goals of the algorithm are for it to:
| 1.	be computationally simple
| 2.	be able to detect subtle peaks
| 3.	have adjustable sensitivity
| 4.	have few input parameters
| 
| Other notable peak detection algorithms exist in the literature.
| A few of these are: 
| [ros83]	histogram concavity
| [ekl79]	difference operators applied in parallel
| [bou85]	Chebyshev polynomial curve fitting of the cumulative distribution
|			function
| 
| Central to the algorithm is the concept of the peak detection signal (pds).
| The peak detection signal is interesting because:
| negative-going zero crossings	indicate a peak start
| positive-going zero crossings	indicate a peak maximum
| local maxima					indicate a peak end
| 
| Theoretically,
| the pds is computed by convolving the histogram with a peak detection kernel.
| The peak detection kernel is itself the result of the convolution
| of a differencer and smoother.
| The differencer approximates a first-order differentiation.
| The smoother is added because real histograms are not ideally smooth.
| The smoother is specified in a general form.
| However, when using a simple averaging filter,
| the entire operation will algebraically reduce to 
| an equation which is very efficient computationally.
| 
| Calculating the pds requires first computing the cdf of the histogram.
| Then each point in the pds is simply the difference
| between the point in the cdf and the average of its neighbors.
| 
| There are only two input parameters to the algorithm.
| The first is N, which is the size of the neighborhood used
| in calculating the pds.
| A smaller value of N will tend to make the algorithm sensitive 
| to smaller, localized peaks.
| A larger value of N will tend to cause the grouping small peaks together into
| a wider, more global peaks.
| In practical applications,
| a value of N between 11 and 41 usually produces reasonable results.
| 
| The second input parameter is the closeness.
| If the end of one peak is closer to the start of the next peak
| than the value of the closeness parameter,
| then the two peaks are merged.
| A smaller value of closeness will produce more peaks
| which are close to one another.
| A larger value of closeness will result in the merging
| of nearby peaks with one another.
| In practice, setting the closeness to a value between 3 and 7
| is usually adequate.
| 
| The algorithm does not require a priori knowledge of the 
| number of peaks in the histogram.
| However, if the number of peaks is known beforehand,
| the algorithm may be applied iteratively.
| Feedback between successive iterations would be used to adjust
| the two input parameters until the desired number of peaks is returned.
| 
|!Software Summary
| The routines in this algorithm automatically
| locate, extract, plot, and print peaks and valleys
| in a one-dimensional array such as a histogram.
|
| Two input parameters control the number of peaks found.
| One adjusts sensitivity by defining a neighborhood width.
| The other controls the merging of peaks which are too close together.
| 
| You can run the algorithm immediately from a simple main program.
| It parses all the input arguments it needs from the command line.
| The main program takes an input image, 
| computes the histogram, then finds the valleys.
| It produces a plot of the histogram overlaid with color boxes
| around the peaks.
| It also produces a multi-thresholded output image from the original.
|
| :dt_sez90:			main program parses command line arguments
|						and calls the functions
| :dt_sez90_samp_ma:	sample program showing simple peak detection
| :dt_sez90_samp_en:	sample program showing detection of partial peaks
| :dt_sez90_samp_pm:	sample program showing intermediate pds calculation
| :dt_sez90_samp_pl:	sample program showing plotting
| dt_sez90()			produces an array in which peaks are marked.
|						Its only input is a one-dimensional array such
|						as a histogram.
| dt_sez90_ends()		locates any partial peaks at the very beginning
|						or very end of the histogram.
| dt_sez90_mark()		places identifying marks at the start, maximum, and
|						end points of each peak.
| dt_sez90_mark_dbg()	prints marks and their locations, 
|						primarily for use during debugging.
| dt_sez90_pds()		accomplishes the first step toward peak detection.
|						It calculates the Peak Detection Signal,
|						a one-dimensional array whose
|						zero crossings and local maxima indicate
|						peak locations.
| dt_sez90_peak_next()	extracts the next peak relative to a current
|						position in the ~mark~ array.
|						By repeated calling, you can examine
|						the peaks one at at time.
| dt_sez90_peak_prev()	extracts the previous peak relative to a current
|						position in the ~mark~ array.
|						By repeated calling, you can examine
|						the peaks one at at time.
| dt_sez90_plot_boxes()	prepares a visual display of peak locations
|						by drawing boxes encompassing them.
| dt_sez90_plot_maxes()	prepares a visual display of peak locations
|						by drawing vertical lines at their maxima.
|
|!Application Notes
| One powerful image segmentation technique begins by
| finding peaks in a histogram.
| Segmentation is accomplished easily by using the low points in the
| valleys between the peaks as global thresholds.
| For example, the image in [fig:toucan.ps]
| has white objects and gray objects
| on a black background.
| These three distinct areas are apparent has three peaks in the image's
| histogram shown in [fig:toucan.ps].
| Two global thresholds, say 150 and 200,
| partition the range of 256 gray levels.
| All pixels less than 150 would be considered background.
| Gray objects would be made of pixels between 150 and 200.
| All pixels above 200 would make up the white objects.
| These two thresholds correspond nicely with the valleys between
| the three peaks.
|
| It seems to be sensitive to tiny peaks.  
| This can be good or bad.
|
| It will often give a very wide peak, not wrong, just wider
| than it is visibly.
|
| Would like to have some way of measuring the significance of the peak.
| Suggest adding absolute value of pds inside peak?
| Maybe a different algorithm?
| 
|!Implementation Notes
| Wherever possible, the implementation faithfully follows
| the guidelines outlined in the original article.
| However, there are places where the implementator
| has had to make decisions based on other factors
| besides what may be found in the content of the body of the article.
|
|.Partial peaks
|_figure half, sez90-ends-hist.ps, Only One Complete Peak
|_figure half, sez90-ends-pds.ps, Peak Detection Signal
|_figure half, sez90-ends-ends.ps, End Peaks Added
|
| The article [sez90] defines a peak as starting at a negative
| crossover, having its maximum at a positive crossover,
| and ending at the maximum found before the next negative crossover.
| It does not discuss how to handle partial peaks which often occur
| at the start and end of the histogram.
| For example, [fig:sez90-ends-hist.ps] illustrates a hypothetical histogram.
| [fig:sez90-ends-pds.ps] is its peak detection signal.
| A box delineates the center peak, which is the only detected peak.
| The leftmost peak is undetected since it has no defined start point.
| This is because the peak detection signal does not deliver an 
| initial negative crossover.
| Likewise, the rightmost peak is undetected since its end point is unknown.
| This is because the peak detection signal does not deliver an 
| final negative crossover which would be the necessary bound on
| the search for the local maximum.
|
| You may call dt_end_ends() which will analyze the ends of
| the histogram and mark partial peaks as regular peaks.
| [fig:sez90-ends-ends.ps] shows delineation of all peaks
| after the those on the ends have been added by dt_sez90_ends().
|
| The rightmost bulge suffers in a similar way.
| There is no negative crossover to its right, 
| therefore we don't know if the top of the bulge is truly
| a local maximum and the end of the peak cannot be known for sure.
|
| Some applications will always select the leftmost bulge as a peak.
| Others will only want to know about peaks with well defined starts and ends.
| This implementation provides the routine dt_sez90_ends() for
| analyzing partial peaks at the ends.
| It essentially adds peaks at the ends if there are significant bulges there.
| Any peaks added are merged with their neighbors if necessary.
|
|.Zero Crossings
| In scanning the pds for zero crossings, 
| it often happens that a pds point ~n~ is above zero 
| while ~n+1~ is below zero.
| Even though the zero crossing happens between the two points,
| one or the other must be chosen as the zero crossing point.
| This implementation chooses between these two points
| by which is closer to zero.  
| If they are both the same distance from zero,
| then the one with the lower index is chosen. 
| 
| END
 *..........................................................................*/


#include <dtack/base.h>
DT_RCSID("alg/sez90 $RCSfile: sez90.c,v $ $Revision: 1.11 $");

#include <dtack/os.h>
#include <dt_sez90.h>

#define DBG dt_dbg(ctl, F, DT_DBG_MASK_OBJECT,
				   
static int global_first = 1;
static double global_input[256];

static
void
check(
  dt_ctl_t *ctl,
  const double *t1,
  double *t2,
  char *what)
{
  int i;
  int diff = 0;
  for (i=0; i<256; i++)
  {
    if (!global_first &&
        !diff && 
        t1[i] != t2[i])
	{
      dt_dbg(ctl, NULL, DT_DBG_MASK_ANY,
        "%s differed at %d",
        what, i);
	  diff = 1;
	}
    t2[i] = t1[i];
  }
  if (!global_first &&
	  !diff)
    dt_dbg(ctl, NULL, DT_DBG_MASK_ANY,
      "%s did not differ",
      what);
  global_first = 0;
}
/*..........................................................................*
 * find context-free zero crossing points in pds array
 * does not find local maxima
 * context-free means zero crossings are found individually, regardless
 *   of their positions or the positions of other zero crossings 
 * if the zero crossing happens between two points, the point closer
 *   to zero is designated as the zero crossing
 *..........................................................................*/
static
void
crossings(
  const double * const pds,
  const int m,
  char * const mark)
{
  int i, j;
  int this, last;

#define NIL 0
#define ZER 1
#define NEG 2
#define POS 3

  last = NIL;
  j = 0;
  for (i=0; i<m; i++)					/* scan input array				    */
  {
	if (pds[i] > 0.0)					/* determine sign				    */
      this = POS;
    else
    if (pds[i] < 0.0)
      this = NEG;
    else
      this = ZER;

	switch(4*last+this)					/* crossover case				    */
	{
	  case 4*NEG+POS:					/* complete crossover to positive? 	*/
        mark[i] |= DT_SEZ90_POS_CROSS;
	  break;
	  case 4*POS+NEG:					/* complete crossover to negative?  */
        mark[i] |= DT_SEZ90_NEG_CROSS;
	  break;

	  case 4*NEG+ZER:					/* hit the zero from negative?	    */
        j = i - 1;
	  break;
	  case 4*POS+ZER:					/* hit the zero from positive?	    */
		j = i - 1;
	  break;

	  case 4*ZER+NEG:					/* leaving zero for negative?	    */
        if (pds[j] > 0.0) 				/* it's really a crossover?		    */
          mark[(i+j)/2] |= 
            DT_SEZ90_NEG_CROSS;
	  break;

	  case 4*ZER+POS:					/* leaving zero for positive?	    */
        if (pds[j] < 0.0)				/* it's really a crossover?		    */
          mark[(i+j)/2] |= 
            DT_SEZ90_POS_CROSS;
      break;

	}

    last = this;
  }
}
/*..........................................................................*
 * find local maxima
 * finds a local maxima 
 *   1. between start and first neg cross
 *   2. between each pair of neg crosses
 *   3. between last neg cross and end
 *..........................................................................*/

static
void
local_maxima(
  const double * const pds,
  const int m,
  char * const mark)
{
  int i, k;
  i = 0;
  k = i;
  while(i < m)							/* scan entire pds				    */
  {
    i++;
    if (pds[i] > pds[k])				/* keep track of maximum we hit	    */
      k = i;
    if (mark[i] & DT_SEZ90_NEG_CROSS ||	/* until next neg cross			    */
        i == m-1)						/* or end of array				    */
	{
	  if (pds[k] > 0)
	    mark[k] |= DT_SEZ90_LOCAL_MAX;	/* mark the local max			    */
	  k = i;
	}
  }
}
/*..........................................................................*
 * find raw peak
 * returns complete raw peak triplet neg/pos/max
 * also returns next neg 
 * if there is no next neg, then max may not be true and full peak
 *   cannot be assumed
 *..........................................................................*/

static
void
raw_peak(
  char * const mark,
  const int n,
  int i,
  int * const s,
  int * const m,
  int * const e,
  int * const f)
{
  while (i<n && !(mark[i] & DT_SEZ90_NEG_CROSS)) i++;
  *s = i;
  while (i<n && !(mark[i] & DT_SEZ90_POS_CROSS)) i++;
  *m = i;
  while (i<n && !(mark[i] & DT_SEZ90_LOCAL_MAX)) i++;
  *e = i;
  if (i == *s) i++;
  while (i<n && !(mark[i] & DT_SEZ90_NEG_CROSS)) i++;
  *f = i;
}
/*..........................................................................*
 * merge two peaks if necessary
 * if s1-e0 > dx, then peaks are not merged and this routine does nothing
 * else interior END and START are removed
 * interior MAX which is the smaller of the two is removed
 * returns 1 if peaks merged, else 0
 *..........................................................................*/
static
int
merge(
  char * const mark,
  const double * const input,
  const int dx,
  const int s0,
  const int m0,
  const int e0,
  const int s1,
  const int m1,
  const int e1)
{
  if (s1 - e0 > dx)						/* no reason to merge?			    */
    return 0;
  mark[e0] &= ~DT_SEZ90_END;			/* get rid of interior end		    */
  mark[s1] &= ~DT_SEZ90_START;			/* get rid of interior start	    */
  if (input[m1] > input[m0])				/* which max is highest?		    */
    mark[m0] &= ~DT_SEZ90_MAXIMUM;		/* get rid of the lower one		    */
  else
    mark[m1] &= ~DT_SEZ90_MAXIMUM;
  return 1;
}

/*..........................................................................
| NAME
| dt_sez90() - peak detection
| 
| DESCRIPTION
| dt_sez90() produces an array in which peaks are marked.
| Its only input is a one-dimensional array such as a histogram.
|_index peak,, detection
|_index detection,, of peaks
|
| The given ~hist~ array of length ~n~ is scanned for peaks.
| Peaks are marked in the ~mark~ array, also of length ~n~.
| 
| This is a convenience function which combines the functions 
| dt_sez90_pds() and dt_sez90_mark().
| The pds array needed by these two functions is automatically
| allocated allocated and freed.
| You may use dt_sez90_pds() and dt_sez90_mark() separately
| if the pds array is of use to your program.
|
| Once the peaks have been marked, you may simply scan the ~mark~
| array to locate peaks for analysis.
| A useful function is  dt_sez90_peak_next(), which extracts the
| peaks from the ~mark~ array one at a time.
| You may also use dt_sez90_plot_boxes() to prepare a visual
| verification of the peak.
|
| Peaks are characterized by their starts, maxima, and ends.
| The positions of each of these are indicated by specific bits
| in the ~mark~ array.
| For example, if a peak is found to start at histogram position 23,
| then the value of ~mark[23]~ will have the :DT_SEZ90_START: bit set.
| Please refer to dt_sez90_mark() for a description of all possible bits.
| 
| CAVEATS
| The ~mark~ array is cleared to zero before peak detection begins.
|
| The ~mark~ array must be of length ~n~.  
| Elements of the array past the ~nth~ will be ignored.
| 
| PARAMETERS
| ~param->n~ is the neighborhood window size for averaging.  
| It must be odd, greater than 3 and less than (~m~-1)/2.
| Default is 31 if ~param->n_flag~ indicates that ~param->n~ does not
| have an assigned value.
| ~param->n~ is actually used in the call to dt_sez90_pds().
|_index param,, n, dt_sez90()
|
| ~param->dx~ is the closeness parameter.
|_index param,, dx, dt_sez90()
| If the distance between the end of one peak and the start of the next
| is ~param->dx~ or less, then the two peaks will be merged.
| A value of -1 indicates that no peaks should be merged.
| Default is -1 if ~param->dx_flag~ indicates that ~param->dx~ does not
| have an assigned value.
| ~param->n~ is actually used in the call to dt_sez90_mark().
|
| EXAMPLE samp_ma.c
|:// example program dt_sez90_samp_ma
|:// illustrates use of dt_sez90() and dt_sez90_next()
|:// only argument is input image name
|:#include <dtack/base.h>
|:#include <dt_sez90.h>
|:void main(int argc, char *argv[])
|:{
|:  dtparam_t param;
|:  dtimage_t image;
|:  dthist_t hist;
|:  double input[256];
|:  char mark[256];
|:  int i, s, m, e;
|:  dtparam_init(&param);				// init default parameters
|:  dtimage_import(&param.ctl, &image,	// read and allocate image
|:    argv[1]);
|:  dtimage_hist(&param.ctl, &image,	// get histogram
|:    hist, 256, 0);
|:  for (i=0; i<256; i++)				// convert frequency to double
|:    input[i] = hist.freq[i];
|:  dt_sez90(&param, hist, 256, mark);	// find peaks, set up mark array
|:  e = 0;								// start at beginning of array	
|:  do {
|:    dt_sez90_peak_next(&param,		// find next peak in mark array
|:      mark, 256, e, &s, &m, &e);
|:    printf("peak start %d, max %d, end %d\n",
|:      s, m ,e);
|:  } while (s < 256);
|:}
|
| RETURN VALUES
| The positions of peaks are marked by bits in the output ~mark~ array.
| The value of the function is either :DT_RC_GOOD: or :DT_RC_BAD:.
| Before returning :DT_RC_BAD:, dt_err() will have been called at least once.
| 
| ERRORS
| Unsuccessful completion may be due to:
| - unable to allocate space for the pds array
| - error in dt_sez90_pds() or dt_sez90_mark()
| 
| SEE ALSO
| dt_sez90_pds()		is used to calculate the pds
| dt_sez90_mark()		is used to mark peak positions, 
|						contains full description of bits in ~mark~ array
| dt_sez90_peak_next()	may be used to access peaks one at a time
|
| END
 *..........................................................................*/

dt_rc_e
dt_sez90(								/* calculate pds and mark peaks		*/
  dtparam_t *param,
  const double * const input,			/* input array to find peaks in	    */
  const int n,							/* length of input and output	    */
  char * const mark)					/* output array to put marks in	   	*/
{
  static char *F = "dt_sez90";
  dt_ctl_t *ctl = &param->ctl;
  double *pds = NULL;
  dt_rc_e rc = DT_RC_GOOD;
  
  DT_Q(dtos_malloc2,(ctl,				/*[Get space for pds.			   ]*/
    (void **)&pds, n*sizeof(*pds),
    F, "pds array"));
  
  DT_C(dt_sez90_pds,(param,				/*[Get pds from dt_sez90_pds().]   ]*/
    input, n, pds));
  
  DT_C(dt_sez90_mark,(param,			/*[Mark peaks with dt_sez90_mark().]*/
    input, n, pds, mark));
  
cleanup:
  if (pds != NULL)
    DT_I(dtos_free2,(ctl, pds,			/*[Free pds space.				   ]*/
      F, "pds_array"));  
  return rc;
}

/*..........................................................................
| NAME
| dt_sez90_hist() - peak detection from histogram
| 
| DESCRIPTION
| dt_sez90_hist() produces an array in which peaks are marked.
| Its only input is a histogram structure such as that computed by
| dtimage_hist().
|_index histogram peak,, detection
|_index detection,, of  histogram peaks
|
| The given ~hist~ structure is scanned for peaks.
| Peaks are marked in the ~mark~ array, which must be of length ~hist->width~.
| 
| This is a convenience function which converts the histogram frequency
| data to double precision then calls the function dt_sez90().
| Please refer to its documentation for more details. 
|
| RETURN VALUES
| The positions of peaks are marked by bits in the output ~mark~ array.
| The value of the function is either :DT_RC_GOOD: or :DT_RC_BAD:.
| Before returning :DT_RC_BAD:, dt_err() will have been called at least once.
| 
| ERRORS
| Unsuccessful completion may be due to:
| - unable to allocate space for the pds array
| - error in dt_sez90_pds() or dt_sez90_mark()
| 
| SEE ALSO
| dt_sez90()			does the actual work
| dt_sez90_peak_next()	may be used to access peaks one at a time
|
| END
 *..........................................................................*/

dt_rc_e
dt_sez90_hist(							/* calculate pds and mark peaks		*/
  dtparam_t *param,
  dthist_t *hist,						/* histogram to find peaks in	    */
  char * const mark)					/* output array to put marks in	   	*/
{
  static char *F = "dt_sez90_hist";
  dt_ctl_t *ctl = &param->ctl;
  double *input = NULL;
  int n = (int)hist->width;
  dt_rc_e rc = DT_RC_GOOD;

  DT_Q(dtos_malloc2,(ctl,				/*[Get space for input array.	   ]*/
    (void **)&input, n*sizeof(*input),
    F, "input array"));
  
  DT_C(dt_sez90,(param,					/*[Use dt_sez90() to do the work.  ]*/
    input, n, mark));
  
cleanup:
  if (input != NULL)
    DT_I(dtos_free2,(ctl, input,		/*[Free input space.			   ]*/
      F, "input array"));  
  return rc;
}

/*..........................................................................
| NAME
| dt_sez90_ends() - mark partial peaks at ends
| 
| DESCRIPTION
| dt_sez90_ends() locates any partial peaks at the very beginning
| or very end of the histogram.
|
| Using the provided ~input~ and ~mark~ arrays, each of length ~n~,
| partial peaks at the beginning or end of ~input~ are estimated and
| added to ~mark~.
|
| This is an optional routine.
| You may choose not to call it and simply ignore any partial
| peaks on the histogram ends.
| Or you may choose to handle partial peaks in your own way.
|
| If you choose to call this routine, 
| you would normally call it just after dt_sez90() or dt_sez90_mark().
|
| Once marked, partial peaks are treated no differently than any other peak by
| subsequent routines.
|
| The main peak finding routines dt_sez90() and dt_sez90_mark()
| deal only with complete peaks.
| A complete peak requires the following sequence in order from left to right:
| 1.	negative crossover (:DT_SEZ90_NEG_CROSS: mark)
| 2.	positive crossover (:DT_SEZ90_POS_CROSS: mark)
| 3.	local maximum (:DT_SEZ90_LOCAL_MAX: mark)
| 4.	negative crossover (:DT_SEZ90_NEG_CROSS: mark)
| However, peaks on either end of the histogram may not
| produce a sequence necessary for a complete peak.
|_index peak,, partial
|_index partial,, peak
|
| A partial peak on the left is indicated by the occurrence of a
| :DT_SEZ90_POS_CROSS: mark before the first :DT_SEZ90_NEG_CROSS: mark.
| A partial peak on the right is indicated by there being a
| :DT_SEZ90_POS_CROSS: mark with no following :DT_SEZ90_NEG_CROSS: mark
| before the end of the array.
|
| The start point of the left-hand partial peak is always estimated to be 0.
| The end point of the right-hand partial peak is estimated to be
| the last position in ~input~.
| The maximum of any partial peak is estimated as the highest ~input~ point
| inside the partial peak.
|
| Partial peaks are subject to merging with their neighbor
| based on the closeness value in ~param->dx~.
|
| On the left end, one of three things is done:
| 1.	nothing if no partial peaks are present
| 2.	a new peak is added if a partial peak is present and too far
|		from its neighbor to be merged 
| 3.	the existing first peak is widened if a partial peak is
|		present and sufficiently close to its neighbor to be merged
|
| One of these three things is also done on the right end.
|
| CAVEATS
| It is assumed that ~mark~ was calculcated from ~input~
| by one of the peak finding routines dt_sez90() or dt_sez90_mark().
|
| The technique implemented by this function is #not# described in [sez90].
| It may be considered an algorithm extension added by DTACK.
|
| Simply ignoring the end peaks may not be a good idea since often
| very significant peaks will be missed.
|
| This routine works very well for most applications which use
| peaks to estimate modes in the histogram.
| However, it should be remembered that partial peaks
| are estimations only.
| Care should be taken when making decisions
| based on the width or height of a partial peak.
|
| A partial peak is, by definition, incomplete.
| A partial peak on the left end has an unknown start point
| A partial peak on the right has an unknown end point.
| The unknown points are assumed to be somewhere outside the
| range of the data.
| Because the actual extent of a partial peak is unknown,
| its actual maximum is also in doubt.
| The maximum could very well be outside the range of the data.
| This routine can only calculate estimations of the unknown points.
|
| The methods of finding a peak maximum differs in the 
| partial versus the complete peak cases.
| The two methods differ subtly.
| 
| PARAMETERS
| ~param->dx~ is the closeness parameter.
| If the distance between the end of one peak and the start of the next
| is ~param->dx~ or less, then the two peaks will be merged.
| Partial peaks produced by this routine are subject to potential merging
| with their neighbor.
| A value of -1 indicates that no peaks should be merged.
| Default is -1 if ~param->dx_flag~ indicates that ~param->dx~ does not
| have an assigned value.
|_index param,, dx, dt_sez90_ends()
|_index closeness,, of partial peaks
|
| EXAMPLE samp_en.c
|:// example program dt_sez90_samp_en
|:// illustrates use of dt_sez90_ends()
|:// only argument is input image name
|:#include <dtack/base.h>
|:#include <dt_sez90.h>
|:void main(int argc, char *argv[])
|:{
|:  dtparam_t param;
|:  dtimage_t image;
|:  double hist[256];
|:  char mark[256];
|:  int i, before, after;
|:  dtparam_init(&param);				// init default parameters
|:  dtimage_import(&param.ctl, &image,	// read and allocate image
|:    argv[1]);
|:  dtimage_hist(&param.ctl, &image,	// get histogram
|:    hist, 256, 0);
|:  dt_sez90(&param, hist, 256, mark);	// find peaks, set up mark array
|:  before = 0;
|:  for (i=0; i<256; i++)				// count peaks before
|:    if (mark[i] & DT_SEZ90_START)
|:      before++;
|:  dt_sez90_ends(&param, hist, 256,	// mark peaks at ends
|:     mark);
|:  after = 0;
|:  for (i=0; i<256; i++)				// count peaks after
|:    if (mark[i] & DT_SEZ90_START)
|:      after++;
|:  printf("%d peaks before, %d peaks after\n",
|:    before, after);
|:}
| 
| RETURN VALUES
| The peaks are marked by the :DT_SEZ90_START:, :DT_SEZ90_MAXIMUM:, and
| :DT_SEZ90_END: bits being placed into the output ~mark~ array.
| The value of the function is either :DT_RC_GOOD: or :DT_RC_BAD:.
| Before returning :DT_RC_BAD:, dt_err() will have been called at least once.
| 
| ERRORS
| Unsuccessful completion may be due to:
| - error in dt_sez90_peak_next() or dt_sez90_peak_prev()
| 
| END
 *..........................................................................*/

dt_rc_e
dt_sez90_ends(							/* mark peaks at ends of given pds	*/
  dtparam_t *param,					  
  const double * const input,			/* input histogram				    */
  const int n,					/* length of histogram and mark	    */
  char * const mark)					/* array where peaks are marked	    */
{
  static char *F = "dt_sez90_mark";
  dt_ctl_t *ctl = &param->ctl;
  int param_dx = (int)(
    DTPARAM_GET_VALUE(					/* get "dx" param				    */
      dx, DT_SEZ90_DEFAULT_CLOSENESS));

  int s, s0, m, m0, e, e0;
  int i;

  s0 = 0;								/* any partial to left starts at 0  */
  for (i=0; i<n; i++)
  {
    if (mark[i] & DT_SEZ90_NEG_CROSS)	/* we are on a peak's downslope?    */
	{
	  i = n;
	  break;
	}
    else
    if (mark[i] & DT_SEZ90_POS_CROSS)	/* we are on a peak's upslope?	    */
	{
      m0 = i;							/* we can use this max			    */
      while(i<n && !(mark[i] &			/* find where peak ends			    */
              DT_SEZ90_LOCAL_MAX)) 
        i++;
      e0 = i;
	  break;
	}
	else
    if (mark[i] & DT_SEZ90_LOCAL_MAX)	/* we are on a peak's downslope?    */
	{
      m0 = 0;							/* max somewhere to left		    */
      e0 = i;							/* peak's end known for sure	    */
	  break;
	}
  }

  if (i < n)							/* we have a partial peak?		    */
  {
    mark[s0] |= DT_SEZ90_START;
    mark[m0] |= DT_SEZ90_MAXIMUM;
    mark[e0] |= DT_SEZ90_END;
    DT_Q(dt_sez90_peak_next,(param,		/* find first real peak			    */
      mark, n, e0, &s, &m, &e));
    if (s < n)							/* there is a following real peak?	*/
	{
	  int merged =
      merge(mark, input, param_dx,		/* possibly merge peaks			    */
        s0, m0, e0, s, m, e);
	  DBG "partial peak (%d,%d,%d) %s with (%d,%d,%d)",
        s0, m0, e0,
        merged? "merged": "not merged",
        s, m, e);
	}
	else
	  DBG "no peak following leading partial peak (%d,%d,%d)",
        s0, m0, e0);
  }
  else
    DBG "no leading partial peak");
										/* ................................ */
  e0 = n-1;								/* any partial to right ends at n-1 */
  for (i=n-1; i>=0; i--)				/* ignore junk to right of margin   */
  {
    if (mark[i] & DT_SEZ90_NEG_CROSS)	/* we are on a peak's downslope?    */
	{
      s0 = i;							/* we can use this start		    */
      m0 = n-1;							/* max was somewhere to right	    */
	  break;
	}
    else
    if (mark[i] & DT_SEZ90_POS_CROSS)	/* we are on a peak's upslope?	    */
	{
      m0 = i;							/* we can use this max			    */
      while(i>=0 && !(mark[i] &			/* find where peak starts		    */
              DT_SEZ90_NEG_CROSS)) 
        i--;
      s0 = i;
	  break;
	}
  }

  if (s0 >= 0)							/* we have a partial peak?		    */
  {
    mark[s0] |= DT_SEZ90_START;
    mark[m0] |= DT_SEZ90_MAXIMUM;
    mark[e0] |= DT_SEZ90_END;
    DT_Q(dt_sez90_peak_prev,(param,		/* find last real peak			    */
      mark, n, s0, &s, &m, &e));
    if (s >= 0)							/* there is a preceeding peak?	    */
    {
      int merged =
      merge(mark, input, param_dx,		/* merge peaks					    */
        s, m, e, s0, m0, e0);
	  DBG "partial peak (%d,%d,%d) %s with (%d,%d,%d)",
        s, m, e,
        merged? "merged": "not merged",
        s0, m0, e0);
    }
	else
	  DBG "no peak preceeding final partial peak (%d,%d,%d)",
        s0, m0, e0);
  }

  return DT_RC_GOOD;
}

/*..........................................................................
| NAME
| dt_sez90_mark() - mark peaks indicated by pds array
| 
| DESCRIPTION
| dt_sez90_mark() places identifying marks at the start, maximum, and
| end points of each peak.
|
| Peak locations are computed using the provided ~input~ and ~pds~ arrays,
| each of length ~n~.
| The locations are marked by setting bits in the output ~mark~ array,
| also of length ~n~.
| The ~mark~ array is cleared to zeros first.
|
| The ~pds~ array must have been calculated by dt_sez90_pds() 
| prior to calling dt_sez90_mark().
| You may wish to use dt_sez90() which combines dt_sez90_pds() and
| dt_sez90_mark() into one function.
|
| If you wish to mark partial peaks which may occur at the
| beginning and end of the histogram,
| you may call dt_sez90_ends() after dt_sez90_mark().
|
| Once all peaks have been marked you may use dt_sez90_next()
| to examine them one at a time.
| You may also use dt_sez90_plot_boxes() to prepare a visual
| display of the peak locations.
|
| The ~mark~ array elements will have zero or more of the following bits set:
| :DT_SEZ90_START:		start of peak 
| :DT_SEZ90_MAXIMUM:	estimated peak maximum of peak 
| :DT_SEZ90_END:		end of peak 
| :DT_SEZ90_NEG_CROSS:	site of negative crossover
| :DT_SEZ90_LOCAL_MAX:	site of local maximum
| :DT_SEZ90_POS_CROSS:	site of positive crossover
|_index peak,, marked
|_index mark,, peak
|
| Normally you will use the :DT_SEZ90_START:, :DT_SEZ90_MAXIMUM:
| and :DT_SEZ90_END: marks and ignore the others.
| This is exactly what dt_sez90_peak_next() and dt_sez90_peak_prev() do.
|
| A given element of the ~mark~ array may have more than one type
| of bit set in it.  
| For example, :DT_SEZ90_START: marks will normally occur coincident with
| :DT_SEZ90_NEG_CROSS: marks.  
| However, the converse is not necessarily true since there may be
| peak merging.
|
| The :DT_SEZ90_NEG_CROSS: and :DT_SEZ90_POS_CROSS: marks are
| considered raw marks.
| They are context-free marks which are solely
| based on the raw input signal.  
| These marks may be specific to this algorithm.
|
| The others are "cooked" by looking at the raw marks and their
| relations to one another.
| For example, a single peak marked by a :DT_SEZ90_START: and a
| :DT_SEZ90_END: may encompass
| several :DT_SEZ90_NEG_CROSS: marks if peak merging has occurred.
|
| The values of the bits :DT_SEZ90_START:, :DT_SEZ90_MAXIMUM: and
| :DT_SEZ90_END: are the same values used by other peak finding
| algorithms.  
| Specifically, the values are equated to the Dtack Imaging Software
| generic mark values:
| :DT_SEZ90_START:		equated to :DT_MARK_START:
| :DT_SEZ90_MAXIMUM:	equated to :DT_MARK_MAXIMUM:
| :DT_SEZ90_END:		equated to :DT_MARK_END:
| This means if you write an analysis function using these values,
| then the analysis function will work on marks produced
| any peak finding algorithm.
|_index mark,, generic
|
| Only complete peaks are marked with :DT_SEZ90_START:,
| :DT_SEZ90_MAXIMUM: and :DT_SEZ90_END:.
| This means that for every :DT_SEZ90_START: marked, there will be exactly
| one corresponding :DT_SEZ90_END: with exactly one :DT_SEZ90_MAXIMUM:
| between them.
|
| It is up to you to handle partial peaks which can occur at
| the start and/or end of the histogram.
| You may choose to call dt_sez90_ends() to handle them, 
| or you may handle them yourself.  
| You can detect partial peaks by observing a :DT_SEZ90_POS_CROSS: mark before
| a :DT_SEZ90_NEG_CROSS: mark at the beginning of the histogram.
| Likewise, a :DT_SEZ90_NEG_CROSS: mark with no subsequent
| :DT_SEZ90_POS_CROSS: mark
| before the end of the histogram also indicates a partial
| peak at the end.
|
| CAVEATS
| It is assumed that the ~pds~ array has been calculated by dt_sez90_pds().
|
| The ~mark~ array is cleared at the beginning of dt_sez90_mark().
|
| Only complete peaks are marked by dt_sez90_mark().
| Call dt_sez90_ends() to pick up additional partial peaks
| which my occur at the beginning and end of the histogram.
|
| PARAMETERS
| This function uses ~param->dx~ as the closeness parameter.
| If the distance between the end of one peak and the start of the next
| is ~param->dx~ or less, then the two peaks will be merged.
| A value of -1 for ~param->dx~ indicates that no peaks should be merged.
| Default assigned by dtparam_init() is -1.
|_index param,, dx, dt_sez90_mark()
|
| EXAMPLE samp_pm.c
|:// example program dt_sez90_samp_pm
|:// shows how to use dt_sez90_pds() with dt_sez90_mark()
|:// only argument is input image name
|:#include <dtack/base.h>
|:#include <dt_sez90.h>
|:void main(int argc, char *argv[])
|:{
|:  dtparam_t param;
|:  dtimage_t image;
|:  double hist[256];
|:  double pds[256];
|:  char mark[256];
|:  dtparam_init(&param);				// init default parameters
|:  dtimage_import(&param.ctl, &image,	// read image
|:    argv[1]);
|:  dtimage_hist(&param.ctl, &image,	// get histogram
|:    hist, 256, 0);
|:  dt_sez90_pds(&param, hist, 256,		// compute pds
|:    pds);
|:  dt_sez90_mark(&param, hist, 256,   	// mark peaks using pds
|:    pds, mark);
|:  dt_sez90_mark_dbg(&param, hist,    	// print peak locations
|:    256, mark,
|:    NULL, DT_DBG_MASK_ANY);
|:}
| 
| RETURN VALUES
| The peaks are marked by the :DT_SEZ90_START:, :DT_SEZ90_MAXIMUM:, and
| :DT_SEZ90_END: bits in the output ~mark~ array.
| The value of the function is either :DT_RC_GOOD: or :DT_RC_BAD:.
| Before returning :DT_RC_BAD:, dt_err() will have been called at least once.
| 
| ERRORS
| The current implementation will not return an error.
|
| SEE ALSO
| dt_sez90()		combines dt_sez90_pds() and dt_sez90_mark()
| dt_sez90_pds()	compute ~pds~ which is input to dt_sez90_mark()
| dt_sez90_ends()	mark additional partial peaks at ends
| 
| END
 *..........................................................................*/

dt_rc_e
dt_sez90_mark(							/* mark peaks given pds				*/
  dtparam_t *param,
  const double * const input,
  const int n,
  const double * const pds,
  char * const mark)
{
  int param_dx = (int)( 
    DTPARAM_GET_VALUE(					/* get "dx" param				    */
      dx, DT_SEZ90_DEFAULT_CLOSENESS));
  int s, s0, m, m0, e, e0, f;

  memset(mark, 0, n*sizeof(*mark));		/* clear mark array				    */

  crossings(pds, (int)n, mark);			/* find all zero crossings		    */
  local_maxima(pds, (int)n, mark);		/* find local maxima			    */

  raw_peak(mark, n, 0,					/* find first peak				    */
    &s0, &m0, &e0, &f);

  while(f < n)							/* while there is a next peak		*/
  {
    raw_peak(mark, n, f,				/* find next peak				    */
      &s, &m, &e, &f);
    if (f == n ||						/* there are no more peaks?		    */
        s - e0 > param_dx)				/* or new peak too far to merge?   	*/
	{
      mark[s0] |= DT_SEZ90_START;		/* cast this peak into concrete	    */
      mark[m0] |= DT_SEZ90_MAXIMUM;
      mark[e0] |= DT_SEZ90_END;
      s0 = s;
      m0 = m;
	}
    else								/* we are supposed to merge?	    */
	  if (input[m] > input[m0])
        m0 = m;
    e0 = e;
  }

  return DT_RC_GOOD;
}

/*..........................................................................
| NAME
| dt_sez90_mark_dbg() - print marks
| 
| DESCRIPTION
| dt_sez90_mark_dbg() prints marks and their locations, 
| primarily for use during debugging.
|
| Print peaks marked in the ~mark~ array with their
| corresponding ~input~ values. 
| Both arrays are of length ~n~.
| Output is enabled or disabled with ~mask~.
| The string ~caller~ is included in the output message unless it is :NULL:.
|_index peak,, printing locations
|_index mark,, printing locations
| 
| All output is produced through dt_dbg(), 
| which is one of the three routines used by all dt routines
| for any kind of human message. 
| The other two are dt_say() and dt_err().
|
| Only the first element of a group of contiguous zeros in ~mark~
| are printed.
| A trailing elipsis (...) is printed when it is about to skip subsequent
| contiguous zeros.
|
| Whether anything is printed by dt_sez90_mark_dbg() or not
| depends on the value of ~mask~.
|_index mask,, debug, for printing peaks
|_index debug,, mask, for printing peaks
| Output is only produced if either one of two conditions is true:
| 1.	the value of ~mask~ is DT_DBG_MASK_ANY
| 2.	the logical "and" of ~mask~ with
|		~param->ctl.dbg_mask~ yields non-zero.
|
| Using a ~mask~ value is the easiest since printing is produced
| regardless of the value of ~param->ctl.dbg_mask~.
|
| Many algorithm and base library routines call dt_dbg() at various
| points in their operation.
| These will produce output depending on the value of the current 
| ~param->ctl.dbg_mask~.
| For example, all memory allocation routines print a message
| when ~param->ctl.dbg_mask~ has the :DT_DBG_MASK_MEM: bit set.
| If you want dt_sez90_mark_dbb() to produce output only for selected masks,
| then you will want to coordinate the ~mask~ you use
| with those being used by the other algorithm and base library routines.
|
| You may set the value of ~param->ctl->dbg_mask~ at any time during
| your program.
| Alternatively, many main programs take advantage of the function
| dt_interactive_parse() which sets
| it according the value of the "-D" flag on the command line.
|_index mask,, debug, setting on command line
|_index debug,, mask, setting on command line
| For example, "-Dmem" would set the :DT_DBG_MASK_MEM: bit
| in ~param->ctl.dbg_mask~.
| 
| Please refer to the documentation for dt_dbg() in [dt-lo-ref].
| 
| PARAMETERS
| Bits in ~param->ctl.dbg_mask~ control whether anything is output.
| No parameter members of ~param~ are used by this function.
| 
| EXAMPLE
| Please refer to dt_sez90_mark() for an example using this function.
| 
| RETURN VALUES
| None.
| 
| ERRORS
| No possible errors.
|
| SEE ALSO
| dt_dbg()	main debug routine
| 
| END
 *..........................................................................*/

void
dt_sez90_mark_dbg(						/* debug mark array				    */
  dtparam_t *param,
  const double * const input,
  const int n,
  const char * const mark,				/* peaks previously marked		    */
  char *caller,
  long mask)
{
  dt_ctl_t *ctl = &param->ctl;
  int i;
  int first = 0;
  for (i=0; i<n; i++)
  {
    if (i > 0 && i < n-1 &&
        !mark[i-1] && 
        !mark[i] &&
        !mark[i+1])
	{
	  if (first)
	    dt_dbg(ctl, caller, mask, "%4d. %8.3f ...", i, input[i]);
      first = 0;
	}
	else
	{
	  char s[64];
	  s[0] = '\0';
	  if (mark[i] & DT_SEZ90_NEG_CROSS) strcat(s, "neg ");
	  if (mark[i] & DT_SEZ90_POS_CROSS) strcat(s, "pos ");
	  if (mark[i] & DT_SEZ90_LOCAL_MAX) strcat(s, "max ");
	  if (mark[i] & DT_SEZ90_START) strcat(s, "START ");
	  if (mark[i] & DT_SEZ90_MAXIMUM) strcat(s, "MAX ");
	  if (mark[i] & DT_SEZ90_END) strcat(s, "END ");
	  dt_dbg(ctl, caller, mask, "%4d. %8.3f %s", i, input[i], s);
      first = 1;
	}
  }
}

/*..........................................................................
| NAME
| dt_sez90_pds() - compute peak detection signal
| 
| DESCRIPTION
| dt_sez90_pds() accomplishes the first step toward peak detection.
| It calculates the Peak Detection Signal, a one-dimensional array
| whose zero crossings and local maxima indicate peak locations.
|
| Using the provided ~input~ array of ~n~ elements,
| compute the output ~pds~ array, also of ~n~ elements.
|
| This is the first function called when doing peak detection.
| After this function you should call dt_sez90_mark() to actually
| mark the peak locations in yet another array.
| If you are not actually interested in the ~pds~ array,
| you may combine the functions of dt_sez90_pds() and dt_sez90_mark(),
| by simply calling dt_sez90().
| It will save you from having to allocate and free the ~pds~ array.
|
| The ~pds~ array elements are computed according to equation 5 in [sez90].
| The cdf (cumulative density function) of the input ~input~ is done first.
| Each ~pds~ element is then set to the difference between the corresponding
| cdf element and the average of the cdf element's ~param->n~ neighbors.
|_index peak detection signal,, computation
|_index pds,, computation
| 
| The margins of the ~pds~ array are computed in the same way, except
| neighbors which would fall outside the array
| boundaries do not contribute to the average.
| The margins are the ~param->n~/2-1 elements at either end.
| 
| CAVEATS
| You must be very careful in how you use the values in the margins
| of the ~pds~.  
| Values in the margins are convolved with less than a full ~param->n~
| neighborhood.
| Therefore, the margins suffer from convolution edge effects,
| and are probably not reliable.
|
| EXAMPLE
| Please refer to dt_sez90_mark() for an example using this function.
|
| PARAMETERS
| ~param->n~ is the neighborhood window size for averaging.  
| It must be odd, greater than 3 and less than (~m~-1)/2.
| Default is 31 if ~param->n_flag~ indicates that ~param->n~ does not
| have an assigned value.
|_index param,, n, dt_sez90_pds()
|
| Note that the other major parameter of this algorithm,
| ~param->dx~, the closeness value, is not used by this routine.
| 
| RETURN VALUES
| The first ~n~ elements of output ~pds~ array are set to computed values.
| The value of the function is either :DT_RC_GOOD: or :DT_RC_BAD:.
| Before returning :DT_RC_BAD:, dt_err() will have been called at least once.
| 
| ERRORS
| Unsuccessful completion may be due to:
| - invalid ~param->n~
| - failure to allocate space for cdf array
| - failure to compute cdf using dt1d_cumulative_double()
|
| SEE ALSO
| dt_sez90()			combines dt_sez90_pds() and dt_sez90_mark()
| dt_sez90_mark()		called after this function
| 
| END
 *..........................................................................*/

dt_rc_e
dt_sez90_pds(
  dtparam_t *param,
  const double *input,
  int n,
  double *pds)
{
  static char *F = "dt_sez90_pds";
  dt_ctl_t *ctl = &param->ctl;
  int param_n = 
    (int)DTPARAM_GET_VALUE(				/* get "n" param				    */
      n, DT_SEZ90_DEFAULT_N);
  const int width =						/* inclusive width of window		*/
    (int)param_n - 1;
  const int margin =					/* margin is half of width			*/
    width / 2;						
  double *cdf, *avg;
  int i, i1, i2;
  dt_rc_e rc = DT_RC_GOOD;

  if (param_n % 2 == 0 ||				/*[Verify suitability of param.	   ]*/
      param_n < 3 ||
      param_n >= (n-1)/2)
    return dt_err(ctl, F,
      "param->n %d is invalid: it must be odd and >=3 and <%ld",
      param_n, n, (n-1)/2);

  DT_GI(dtos_malloc2,(ctl,				/*[Allocate space for a cdf array  ]*/
    (void **)&cdf, n*sizeof(*cdf),		/*[of ~n~ elements.				   ]*/
    F, "temp cdf array"));
  DT_GI(dtos_malloc2,(ctl,				/*[Allocate space for an avg array ]*/
    (void **)&avg, n*sizeof(*avg),		/*[of ~n~ elements.				   ]*/
    F, "temp avg array"));

check(ctl,input, global_input, "input");
  
  DT_GI(dt1d_cumulative_double,(		/*[Compute the cdf array.		   ]*/
    input, n, cdf, n));
  DT_GI(dt1d_cumulative_double,(		/*[Compute the neighborhood sums.  ]*/
    cdf, n, avg, n));

  if (rc == DT_RC_GOOD)
  {
	double tmp;
    for (i=0; i<n; i++)
    {
      if (i <= margin)
	  {
		i2 = 2 * i;
		pds[i] = cdf[i] - avg[i2] / (double)(i2+1);
i1 = -1;
tmp = avg[i2];
	  }
	  else
	  if (i >= n-margin)
	  {
        i1 = n - 1 - 2 * (n - 1 - i) - 1;
        i2 = n - 1;
		pds[i] = cdf[i] - (avg[i2]-avg[i1]) / (double)(i2-i1);
tmp = avg[i2] - avg[i1];
	  }
	  else
	  {
	    i1 = i - margin - 1;
	    i2 = i + margin;
		pds[i] = cdf[i] - (avg[i2]-avg[i1]) / (double)(i2-i1);
tmp = avg[i2] - avg[i1];
	  }
if (i < margin + 5 || i >= n - margin - 5) 
dt_dbg(ctl, F, DT_DBG_MASK_PIXEL,
"%3ld %3ld %3ld (%10.2f / %3ld) avg %e  cdf %e  pds %e",
i1, i, i2, tmp, i2-i1, tmp/(double)(i2-i1), cdf[i], pds[i]);
    }
  }

										/*+Finally,						   ]*/
  dtos_free2(ctl, avg,					/*[free space for avg array and	   ]*/
    F, "temp avg array");
  dtos_free2(ctl, cdf,					/*[free space for cdf array.	   ]*/
    F, "temp cdf array");

  return rc;
}

/*..........................................................................
| NAME
| dt_sez90_peak_next() - find next peak in mark array
| 
| DESCRIPTION
| dt_sez90_peak_next() extracts the next peak relative to a current
| position in the ~mark~ array.
| By repeated calling, you can examine the peaks one at at time.
|
| These convenience functions extract the next or previous
| peak in the ~mark~ array given initial position ~i~.
| A peak is characterized by its ~start~, ~maximum~, and ~end~
| which are indices in the ~mark~ array.
| The ~mark~ array is assumed of size ~n~.
|_index peak,, find next
|_index find,, next peak
|_index next,, find peak
| 
| If there are no more peaks, then all of ~start~, ~maximum~, and ~end~
| will be set to ~n~ for dt_sez90_peak_next(),
| or to -1 for dt_sez90_peak_prev().
|
| Any of ~start~, ~maximum~, and ~end~ may be NULL, 
| indicating that no value is to be returned for that argument.
|
| This function allows you to deal with one peak at a time.
| 
| The ~mark~ array should have been calculated by dt_sez90() or
| dt_sez90_mark(). 
| If you plan to use dt_sez90_ends(), then it should be called
| before this function as well.
|
| If these routines have done their job, then this routine will 
| either return a complete peak or no peak.  
| A complete peak has a valid ~start~, ~maximum~, and ~end~.
| There are no partial peaks.
| In other words, when all of ~start~, ~maximum~, and ~end~
| are greater than -1 and less than ~n~, then you have a peak.
| Otherwise all three values should be equal to ~n~ for dt_sez90_peak_next()
| or to -1 for dt_sez90_peak_prev().
|
| PARAMETERS
| No parameter members of ~param~ are used by this function.
| 
| RETURN VALUES
| The arguments ~start~, ~maximum~, and ~end~ will be set to new values
| if their values are not NULL.
| The value of the function is either :DT_RC_GOOD: or :DT_RC_BAD:.
| Before returning :DT_RC_BAD:, dt_err() will have been called at least once.
| 
| ERRORS
| The current implementation will not return an error.
| 
| EXAMPLE
| Please refer to dt_sez90() for an example using this function.
| 
| END
 *..........................................................................*/

dt_rc_e
dt_sez90_peak_next(
  dtparam_t *param,
  const char * const mark,
  const int n,
  int i,
  int * const start,
  int * const maximum,
  int * const end)
{
  while (i<n && !(mark[i] & DT_SEZ90_START)) i++;
  if (start != NULL) *start = i;
  while (i<n && !(mark[i] & DT_SEZ90_MAXIMUM)) i++;
  if (maximum != NULL) *maximum = i;
  while (i<n && !(mark[i] & DT_SEZ90_END)) i++;
  if (end != NULL) *end = i;
  return DT_RC_GOOD;
}

/*..........................................................................
| NAME
| dt_sez90_prev() - find previous peak in mark array
| 
| DESCRIPTION
| dt_sez90_peak_prev() extracts the previous peak relative to a current
| position in the ~mark~ array.
| By repeated calling, you can examine the peaks one at at time.
|
| Convenience function to extract the next or previous
| peak in the ~mark~ array given initial position ~i~.
| A peak is characterized by its ~start~, ~maximum~, and ~end~
| which are indexes in the ~mark~ array.
| The ~mark~ array is assumed of size ~n~.
|_index peak,, find previous
|_index find,, previous peak
|_index previous,, find peak
| 
| Please refer to dt_sez90_next for a complete description.
|
| PARAMETERS
| No parameter members of ~param~ are used by this function.
| 
| RETURN VALUES
| The arguments ~start~, ~maximum~, and ~end~ will be set to new values
| if their values are not NULL.
| STDRET.
| 
| ERRORS
| NOERR.
| 
| END
 *..........................................................................*/

dt_rc_e
dt_sez90_peak_prev(
  dtparam_t *param,
  const char * const mark,
  const int n,
  int i,
  int * const start,
  int * const maximum,
  int * const end)
{
  while (i>=0 && !(mark[i] & DT_SEZ90_END)) i--;
  if (end != NULL) *end = i;
  while (i>=0 && !(mark[i] & DT_SEZ90_MAXIMUM)) i--;
  if (maximum != NULL) *maximum = i;
  while (i>=0 && !(mark[i] & DT_SEZ90_START)) i--;
  if (start != NULL) *start = i;
  return DT_RC_GOOD;
}

/*..........................................................................
| NAME
| dt_sez90_plot_boxes() - plot boxes around peaks
| 
| DESCRIPTION
| dt_sez90_plot_boxes() prepares a visual display of peak locations
| by drawing boxes encompassing them.
|
| For each peak in the array ~mark~ of length ~n~,
| a rectangle of intensity ~color~ is plotted in the output image ~plot~. 
|_index peak,, plotting box around
|_index box,, plotting around peak 
|_index plotting,, box around peak
| 
| To calculate the peaks, you first use dt_sez90() or dt_sez90_mark().
|
| Before plotting the boxes, you may first want to plot the histogram
| in ~plot~ using dtimage_plot_xbars_double().
| Then the boxes will appear to overlay on top of the histogram,
| giving an effective visual display.
| You should use two different colors for the histogram and the boxes.
|
| A companion function, dt_sez90_plot_maxes() will draw lines
| at peak maxima.
|
| After plotting the boxes, you will usually call dtimage_export()
| to display the plot or to produce a file for printing.
|
| The element ~mark[0]~ will be aligned with column 0 of ~plot~.
| Only boxes which fit completely within ~plot~ are plotted.
| The box will extend the full height of ~plot~ except for a
| 1-pixel-high margin at its top and bottom.
| 
| CAVEATS
| It is assumed that ~mark~ has been computed
| by either dt_sez90() or dt_sez90_mark() and optionally modified
| by dt_sez90_ends().  
|
| It is also assumed that ~plot~ has been allocated
| and is at least ~n~ pixels wide.  
| Boxes which extend beyond the width of ~plot~ are not plotted.
|
| The output image is not cleared before plotting the boxes.
|
| EXAMPLE samp_pl.c
|:// example program dt_sez90_samp_pl
|:// demonstrates how to plot boxes around peaks and lines at their maxima
|:// first argument is input image name, second argument is output plot name
|:#include <dtack/base.h>
|:#include <dt_sez90.h>
|:void main(int argc, char *argv[])
|:{
|:  dtparam_t param;
|:  dtimage_t image;
|:  double input[256];
|:  char mark[256];
|:  dtimage_t plot;
|:  dtparam_init(&param);				// init default parameters
|:  dtimage_import(&param.ctl, &image,	// read and allocate image
|:    argv[1]);
|:  dtimage_hist(&param.ctl, &image,	// get histogram
|:    hist, 256, 0);
|:  dt_sez90(&param, hist, 256, mark);	// find peaks, set up mark array
|:
|:  dtimage_create(&param.ctl, &plot,	// create plot image
|:    256, 128, DT_NTYPE_B08);			// width 256, height 128
|:  dtimage_constant(&param.ctl, &plot,	// clear plot image to color 0
|:    0);
|:  dtimage_plot_xbars_double(			// plot histogram in color 1
|:    &param.ctl, &plot, hist, 256,
|:    0, 128, 1);
|:  dt_sez90_plot_boxes(&param,			// plot boxes in color 2
|:    mark, 256, &plot, 2);
|:  dt_sez90_plot_maxes(&param,		   	// plot peak maxima in color 3
|:    mark, 256, &plot, 3);
|:  dtimage_export(&param.ctl, &plot,	// output plot to display or file
|:    argv[2], -1, -1);
|:}
|
| RETURN VALUES
| The contents of the image memory in ~plot~ is changed by plotted pixels.
| The value of the function is either :DT_RC_GOOD: or :DT_RC_BAD:.
| Before returning :DT_RC_BAD:, dt_err() will have been called at least once.
| 
| ERRORS
| Unsuccessful completion may be due to:
| - error in dt_sez90_peak_next()
| - error in dtimage_plot_line()
| 
| SEE ALSO
| dt_sez90()					is used to mark peak positions
| dt_sez90_mark()				is also used to mark peak positions
| dt_sez90_plot_maxes()			plots peak maxes instead of peak boxes
| dtimage_plot_xbars_double()	is used to plot a histogram
| dtimage_constant()			clears an image before plotting
|
| END
 *..........................................................................*/

dt_rc_e
dt_sez90_plot_boxes(					/* plot boxes around peaks			*/
  dtparam_t *param,
  const char * const mark,				/* peaks previously marked		    */
  const int n,
  dtimage_t *plot,						/* image to plot in				    */
  double color)
{
  static char *F = "dt_sez90_plot_boxes";
  dt_ctl_t *ctl = &param->ctl;
  int s, e, m;
  dt_rc_e rc = DT_RC_GOOD;

  e = 0;
  do {
	DT_Q(dt_sez90_peak_next,(param,		/* find next peak				    */
      mark, n, e, &s, &m, &e));

	if (e < n &&						/* this peak has a known end?		*/
        e < plot->xe)					/* and whole box fits in plot?	    */
	{
	  DT_Q(dtimage_plot_line,(ctl,		/* draw left side					*/
        plot,
        s, 1, s, plot->ye-2,
        color));
	  DT_Q(dtimage_plot_line,(ctl,		/* draw right side					*/
        plot,
        e, 1, e, plot->ye-2,
        color));
  	  DT_Q(dtimage_plot_line,(ctl,		/* draw top							*/
        plot,
        s, plot->ye-2, e, plot->ye-2,
        color));	
	  DT_Q(dtimage_plot_line,(ctl,		/* draw bottom						*/
        plot,
        e, 1, s, 1, 
        color));
    }
  } while (rc == DT_RC_GOOD && e < n);

  return DT_RC_GOOD;
}

/*..........................................................................
| NAME
| dt_sez90_plot_maxes() - plot lines at peak maxima
| 
| DESCRIPTION
| dt_sez90_plot_maxes() prepares a visual display of peak locations
| by drawing vertical lines at their maxima.
|
| For each peak in the array ~mark~ of length ~n~,
| a vertical line of intensity ~color~ is plotted in the output image ~plot~. 
|_index peak,, plotting line at maximum
|_index line,, plotting at peak maximum
|_index plotting,, line at peak maximum
| 
| To calculate the peaks, you first use dt_sez90() or dt_sez90_mark().
|
| Before plotting the maxima, you may first want to plot the histogram
| in ~plot~ using dtimage_plot_xbars_double().
| Then the lines will appear to overlay on top of the histogram,
| giving an effective visual display.
| You should use two different colors for the histogram and the 
| lines at the locations of the peak maxima.
|
| A companion function, dt_sez90_plot_boxes() will draw boxes
| around the peaks.
|
| After plotting the maxima, you will usually call dtimage_export()
| to display the plot or to produce a file for printing.
|
| The element ~mark[0]~ will be aligned with column 0 of ~plot~.
| Only lines at x locations within the width of ~plot~ are plotted.
| The line will extend the full height of ~plot~ with no
| margin at its top and bottom.
| 
| CAVEATS
| It is assumed that ~mark~ has been computed
| by either dt_sez90() or dt_sez90_mark() and optionally modified
| by dt_sez90_ends().  
|
| It is also assumed that ~plot~ has been allocated
| and is at least ~n~ pixels wide.  
| Lines which extend beyond the width of ~plot~ are not plotted.
|
| The output image is not cleared before plotting the lines.
|
| EXAMPLE
| Please refer to dt_sez90_plot_boxes() for an example using this function.
|
| RETURN VALUES
| The contents of the image memory in ~plot~ is changed by plotted pixels.
| The value of the function is either :DT_RC_GOOD: or :DT_RC_BAD:.
| Before returning :DT_RC_BAD:, dt_err() will have been called at least once.
| 
| ERRORS
| Unsuccessful completion may be due to:
| - error in dt_sez90_peak_next()
| - error in dtimage_plot_line()
| 
| SEE ALSO
| dt_sez90()					is used to mark peak positions
| dt_sez90_mark()				is also used to mark peak positions
| dt_sez90_plot_maxes()			plots peak maxes instead of peak boxes
| dtimage_plot_xbars_double()	is used to plot a histogram
| dtimage_constant()			clears an image before plotting
|
| END
 *..........................................................................*/

dt_rc_e
dt_sez90_plot_maxes(					/* plot peak maximums				*/
  dtparam_t *param,
  const char * const mark,				/* peaks previously marked			*/
  const int n,
  dtimage_t *plot,						/* image to plot in				    */
  double color)
{
  static char *F = "dt_sez90_plot_maxes";
  dt_ctl_t *ctl = &param->ctl;
  int s, e, m;
  dt_rc_e rc = DT_RC_GOOD;

  e = 0;
  do {
	DT_Q(dt_sez90_peak_next,(param,		/* find next peak				    */
      mark, n, e, &s, &m, &e));

	if (e < n)							/* this peak has a known end?		*/
	{
	  DT_Q(dtimage_plot_line,(ctl,		/* draw vertical line at max		*/
        plot,
        m, 1, m, plot->ye-2,
        color));
    }
  } while (rc == DT_RC_GOOD && e < n);

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
