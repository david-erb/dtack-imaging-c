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
| This is an implementation of.
| The algorithm is due to.
| It is described completely in [xxx99].
| Input is.
| The implementation provides.
| A simple main program provides command-line execution of the routines.
| A main program has an option for testing the implementation.
|
|!Overview
|_figure half, toucan.ps, Source Image
|_figure half, toucan-multi.ps, Multithresholded Image
|_figure half, toucan-sez90.ps, Histogram with Thresholds
|
|.~What are the goals of the algorithm?~
| 
|.~How would you use the algorithm?~
|
|.~What does the output look like?~
| 
|.~What should you be aware of?~
| 
|.~What is the interface to the implementation?~
| The implementation has a run-ready main program and a C-callable library.
| The main program has a number of command-line options.
| It uses the library to do all its tasks.
| It will read an input image
| and produce.
| 
|.~What inputs do the C-callable library routines require?~
| You must provide.
| You must choose a value.
| There are reasonable defaults for both input parameters.
| 
|.~What do the library routines output?~
| 
|.~Does the implementation test itself?~
| Yes the implementation contains an extensive self-test suite.
| Successful execution of the test suite indicates the correctness
| of the implementation to a high degree of confidence.
| The test suite has a C-callable interface.
| You may also invoke it from the main program.
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
|			(not required if you write your own program)
| (Actual names may vary slightly depending on operating system.)
|
|!Algorithm Review
| This section is a review of the algorithm presented in [sez90].
| 
| The stated goals of the algorithm are for it to:
| 1.	be computationally simple
| 
| Other notable peak detection algorithms exist in the literature.
| A few of these are: 
| [ros83]	histogram concavity
| 
| There are only two input parameters to the algorithm.
| 
|!Implementation Interface
| The routines in this algorithm do this.
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
|!Implementation Notes
| this section describes assumptions and deviations of the
| implementation from the original article
|
|!Application Examples
| this section gives practical examples of the use of this algorithm
| usually used in conjunction with other programs
| described in prose and using other dt programs
| 
| END
 *..........................................................................*/


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
|_index param,, n, dt_sez90()
|_index param,, dx, dt_sez90()
| 
| PARAMETERS
| ~param->n~	the neighborhood window size for averaging.  
|	It must be odd, greater than 3 and less than (~m~-1)/2.
|	Default is 31 if ~param->n_flag~ indicates that ~param->n~ does not
|	have an assigned value.
|	~param->n~ actually used in the call to dt_sez90_pds().
| ~param->dx~	the closeness parameter.
|	If the distance between the end of one peak and the start of the next
|	is ~param->dx~ or less, then the two peaks will be merged.
|	A value of -1 indicates that no peaks should be merged.
|	Default is -1 if ~param->dx_flag~ indicates that ~param->dx~ does not
|	have an assigned value.
|dtinclude(paramctl.dx)
|dtinclude(params0.dx)
|
| RETURN VALUES
| The positions of peaks are marked by bits in the output ~mark~ array.
|dtinclude(return1.dx)
| 
| ERRORS
| Unsuccessful completion may be due to:
| - unable to allocate space for the pds array
| - error in dt_sez90_pds() or dt_sez90_mark()
| 
| CAVEATS
| The ~mark~ array is cleared to zero before peak detection begins.
|
| The ~mark~ array must be of length ~n~.  
| Elements of the array past the ~nth~ will be ignored.
| 
| SEE ALSO
| dt_sez90_pds()		used to calculate the pds
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
|:  double hist[256];
|:  char mark[256];
|:  int s, m, e;
|:  dtparam_init(&param);				// init default parameters
|:}
|
| END
 *..........................................................................*/





/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
  ! Copyright (C) 1998 Dtack Inc. All Rights Reserved
  ! To use this file, you must have signed a license agreement with Dtack Inc.
  ! Under no circumstances may you redistribute this file.
  ! This software is provided AS IS with no warranty expressed or implied.
  ! Dtack Inc. accepts no liability for use or misuse of this file.
  ! http://www.dtack.com  dtack@dtack.com  telephone +360.417.6700
  ! Dtack Inc, P.O. Box 3061, Port Angeles, WA USA 98362
  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */
