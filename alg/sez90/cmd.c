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
| USAGE
| dt_sez90 - algorithm executive
| 
|.SYNOPSIS
|:dt_sez90 [parameters] input output [plot]
| 
|.SUMMARY
| Find global thresholds using peaks in histogram.
| Produce a multi-thresholded image and an annotated histogram plot.
| 
|.ARGUMENTS
| :input:		name of the input image
| :output:   	name of the optional output multi-thresholded image
| :plot:		name of the output histogram plot image
|
|.PARAMETERS
| :-n:			algorithm window size (default 15)
| :-dx:			algorithm closeness value (default 3)
| :-op:			operation, one of {normal test time} (default normal)
| :-foreground:	lightest multi-threshold output pixel color (default 255)
| :-background:	darkest multi-threshold output pixel color (default 1)
| :-x:			output x position of image data
| :-y:			output y position of image data
|dtinclude(params1.dx)
| 
|.DESCRIPTION
| First calculates the histogram of ~input~.
| Then uses the dt_sez90 algorithm to compute the histogram peaks.
| The N peak start positions are used to make N+1 bins.
| N+1 gray levels for the bins are chosen evenly over the 
| gray level range ~background~ to ~foreground~.
| Pixel gray levels in ~output~ are assigned the gray level of the bin
| corresponding to the pixel at the same position in ~input~.
|
| If an ~output~ image is produced,
| it will have the same dimensions as the input image and be 8 bits deep.
| The maximum value of its pixels will be ~foreground~.
| The minimum will be ~background~ if more than one peak is found.
| If there is a gap between the end of one peak and the start of the next,
| pixels in the gap are assigned to the peak on the left.
|
| The default ~background~ is 1, 
| ensuring no pixels in ~output~ get the value 0.
| This allows you to use the multi-thresholded image as
| a segmentation in which all pixels belong to objects
| and none to background.
|
| For a striking pseudo-color display using a Dtack viewer,
| choosing ~background~ 1 and ~foreground~ 7.
| These values are normally assigned bright colors on the display.
|
| If a ~plot~ image is produced,
| it will have dimensions 256 by 128 and be 8 bits deep.
| It will have gray background with white histogram bars.
| Superimposed on the histogram are green boxes with red bars
| indicating the peak extents and peak maxima.
| A blue waveform indicates the PDS.
|
| The name of an ~output~ image is optional.
| If ~output~ is not given, or is a single hyphen (-),
| then no output image is produced.
| The name of a ~plot~ image is also optional.
| If not given, no plot is produced.
| If neither ~output~ nor ~plot~ is produced,
| then the program only produced a printed summary of the peaks.
|
|.OPERATIONS 
| The :-op: parameter may take one of the following values:
| :normal:	perform algorithm on input to produce output
| :test:	perform implementation self-tests and display pass/fail result
| :time:	perform implementation benchmark and display execution times
|
|.PAGING
| The :test: and :time: operations may be exercised under varying paging 
| configurations.
| The minimum no-dialog configurations are:
| :-op test:		maxalloc 16384, pagesize 128, npages 1
| :-op time:		maxalloc 262144, pagesize 512, npages 1
|
|dtinclude(params2.dx)
|
| END
 *..........................................................................*/

#include <dtack/base.h>
DT_RCSID("alg/sez90 $RCSfile: cmd.c,v $ $Revision: 1.11 $");
#include <dtack/os.h>
#include <dtack/str.h>
#include <dtack/lut8.h>
#include <dt_sez90.h>
#include <dtack/mem.h>

#include <sez90.use>

# define OP_DEFAULT "normal"
# define OP_TEST    "test"
# define OP_TIME    "time"

static const char *ops[] = {
  OP_DEFAULT,
  OP_TEST,
  OP_TIME,
  NULL};

#define BACKGROUND_COLOR  128			/* gray for plot background		    */
#define BAR_COLOR  DTLUT8_WHITE			/* white for histogram bars		    */
#define BOX_COLOR  DTLUT8_GREEN			/* green for boxes around peaks	    */
#define MAX_COLOR  DTLUT8_RED			/* red for maxima of peaks		    */
#define PDS_COLOR  DTLUT8_BLUE			/* blue for pds waveform		    */
#define ZERO_COLOR DTLUT8_YELLOW		/* yellow for line at pds y=0.0		*/

/*..........................................................................
| NAME
| dt_sez90_cmd - command line interface to algorithm
| 
| DESCRIPTION
| As yet undocumented.
| 
| RETURN VALUES
| Program exit value will be DT_EXIT_GOOD on success.  
| Any other exit value indicates failure or noncompletion.
| 
| ERRORS
| As yet undocumented.
| 
| END
 *..........................................................................*/

dt_rc_e
dt_sez90_cmd(
  int argc,
  char *argv[],
  dtparam_t *param)
{
  DT_F("dt_sez90_cmd");
  dt_ctl_t *ctl = &param->ctl;
# define ARG_MAXLEN 128
  char input[ARG_MAXLEN];
  char output[ARG_MAXLEN];
  char plot[ARG_MAXLEN];
  dt_enter_arg_t arg[4];
  dt_rc_e rc = DT_RC_GOOD;

  DT_GI(dt_enter_arg,(ctl,				/* set up positionals			    */
    arg,
    "Input image",   NULL, input,  sizeof(input),
    "Output image",  NULL, output, sizeof(output),
    "Plot image",    NULL, plot,   sizeof(plot),
    NULL));

  DT_GI(dt_sez90_cmd_parse,(param,		/* parse args					    */
    argc, argv, arg));

  while (rc == DT_RC_GOOD)
  {
    if (!strcmp(param->op, OP_DEFAULT))
	{
	  DT_GI(dt_sez90_cmd2,(param,		/* import image, do the operation	*/
         input, output, plot));
	}
  
    if (!strcmp(param->op, OP_TEST))
	{
	  int assert_fail = 0;
	  DT_GI(dt_sez90_test_all,(			/* do the operation				    */
        param, &assert_fail));
  
      if (rc == DT_RC_GOOD &&			/* test worked?					    */
          assert_fail)					/* but some results wrong?		    */
        rc = DT_RC_BAD;
	}

    if (!strcmp(param->op, OP_TIME))
	{
	  int assert_fail = 0;
	  DT_GI(dt_sez90_time_all,(			/* do the operation				    */
        param, &assert_fail));
  
	  if (rc == DT_RC_GOOD &&			/* test worked?					    */
          assert_fail)					/* but some results wrong?		    */
	  rc = DT_RC_BAD;
	}

    DT_G(dt_interactive_after,(			/* make post display				*/
      ctl, param, usage, arg,
      dt_sez90_cmd_check, rc));
  } 

  if (rc == DT_RC_STOP)
    rc = DT_RC_GOOD;

  DT_I(dt_interactive_leave,(			/* clean up before leaving			*/
    param, rc));

  return rc;
}

/*..........................................................................*/

dt_rc_e
dt_sez90_plot(							/* make annotated histogram plot 	*/
  dtparam_t *param,
  dthist_t *hist,
  unsigned int ngray,
  const double *pds,
  const char *mark,
  char *plot_name)
{
  DT_F("dt_sez90_plot");
  dt_ctl_t *ctl = &param->ctl;
  dtimage_t plot;
  dt1d_i_t min, max;
  double pmin, pmax;
  dtxy_t t;
  dt_rc_e rc;

  plot.xe = 0;
  if (plot_name && *plot_name)			/* not null or empty name?		    */
  {
    DT_C(dtimage_create,(ctl,			/* space for plot image			    */
      &plot, ngray, 128,
      DT_NTYPE_B08));
    DT_C(dtimage_constant,(ctl,			/* clear plot image				    */
      &plot, 128.0));					/* to gray background			    */
    DT_C(dtimage_hist_plot_horiz,(ctl,	/* make bar plot of histogram		*/
      &plot, hist,
      2, plot.ye-4, BAR_COLOR));

    DT_C(dt1d_range_double,(			/* highest pds point for scaling 	*/
      pds, ngray, &min, &max));
    pmin = pds[min];
    pmax = pds[max];
    if (pmin > 0)						/* make zero coincide on graph	    */
      pmin = -pmax;
    else
    if (pmax < 0)
      pmax = -pmin;
    else
    if (pmax > -pmin)
      pmin = -pmax;
    else
      pmax = -pmin;

#   define Y(U) ((dtxy_t)(plot.ye-1-(plot.ye-2)*((U-pmin)/(pmax-pmin))))

	DT_C(dtimage_plot_line,(ctl,		/* draw 0 line					    */
      &plot,
      0, Y(0.0), ngray-1, Y(0.0),
      ZERO_COLOR));

    for (t=0; t<ngray; t++)				/* for all pds values			    */
	  DT_C(dtimage_plot_line,(ctl,
        &plot,
        t, Y(pds[t]),
        t, Y(pds[t-1]), PDS_COLOR));

    DT_C(dt_sez90_plot_boxes,(param,	/* draw boxes surrounding peaks	    */
      mark, ngray, &plot, BOX_COLOR));

    DT_C(dt_sez90_plot_maxes,(param,	/* draw peak max lines			    */
      mark, ngray, &plot, MAX_COLOR));

    DT_C(dtimage_export,(ctl,			/* output plot						*/
      &plot, plot_name,
      -1, -1));
  }

cleanup:

  if (plot.xe)
    DT_I(dtimage_free,(ctl, &plot));	/* free plot image					*/

  return rc;
}

/*..........................................................................*/

dt_rc_e
dt_sez90_thresh(						/* threshold image based on peaks 	*/
  dtparam_t *param,
  dtimage_t *input,						/* input gray level image 			*/
  const char *mark,						/* mark array from dtsez90 			*/
  unsigned int ngray,					/* length of mark array				*/
  dtimage_t *output,					/* pre-created output image 		*/
  double *thresh,						/* tmp space array, optionally NULL	*/
  double *values)						/* tmp space array, optionally NULL	*/
{
  DT_F("dt_sez90_output");
  dt_ctl_t *ctl = &param->ctl;
  int allocated_thresh = 0;
  int allocated_values = 0;
  unsigned int i;
  unsigned int n;
  double inc;
  dt_rc_e rc;

  n = 0;
  for (i=0; i<ngray; i++)
  if (mark[i] & DT_SEZ90_START &&		/* if this gray level is peak start	*/
      i > 0)							/* and peak doesn't start at 0	    */
    n++;								/* count number of thresholds 		*/

  if (thresh == NULL)
  {
    DT_C(dtos_malloc2,(ctl,				/* get space for thresh array 		*/
      (void **)&thresh, 
      n * sizeof(*thresh),
      F, "thresh array"));
	allocated_thresh = 1;
  }

  if (values == NULL)
  {
    DT_C(dtos_malloc2,(ctl,				/* get space for values 			*/
      (void **)&values, 
      (n + 1) * sizeof(*values),
      F, "values array"));
	allocated_values = 1;
  }

  n = 0;
  for (i=0; i<ngray; i++)
  if (mark[i] & DT_SEZ90_START &&		/* if this gray level is peak start	*/
      i > 0)							/* and peak doesn't start at 0	    */
    thresh[n++] = i;					/* remember start of the peak	    */

  inc =
	(param->foreground -				/* delta gray level per threshold   */
     param->background) / 
     (double)(n? n: 1);
  dt_dbg(ctl, F, DT_DBG_MASK_ANY,
    "n %u, inc %8.5f", n, inc);
  for (i=0; i<n; i++)					/* assign values equidistantly	    */
    values[i] = param->background +
      i * inc;
  values[i] = param->foreground;		/* in case of non-integrality	    */

  DT_C(dtimage_thresh_multi,(ctl,		/* multithreshold image				*/
    input, thresh, n, values,
    output));

cleanup:
  if (allocated_values)  
    DT_C(dtos_free2,(ctl,				/* free space for values array 		*/
      values, 
      F, "values array"));
  if (allocated_thresh)
    DT_C(dtos_free2,(ctl,				/* free space for thresh array 		*/
      thresh, 
      F, "thresh array"));
  return rc;
}

/*..........................................................................*/

dt_rc_e
dt_sez90_cmd2(
  dtparam_t *param,
  char *input_name,
  char *output_name,
  char *plot_name)
{
  DT_F("dt_sez90_cmd2");
  dt_ctl_t *ctl = &param->ctl;
  dtimage_t input;
  dthist_t hist;
  unsigned int width;
  double *freq = NULL;
  double *pds = NULL;
  char *mark = NULL;
  double *thresh = NULL;
  double *values = NULL;
  int start, maximum, end;
  dtxy_t i;
  dt_rc_e rc;

  input.xe = 0;
  DT_C(dtimage_import,(ctl,				/* get input image					*/
    &input, input_name));
										/* ................................ */

  hist.freq = NULL;
  if (input.xe == 1 &&					/* single-column image?				*/
	  input.ntype == DT_NTYPE_F2)		/* of type double?					*/
  {
	DT_Q(dthist_alloc,(ctl, &hist,		/* make histogram to hold data		*/
      input.ye, 0, input.ye));
    for (i=0; i<input.ye; i++)			/* copy out histogram data			*/
	  DTIMAGE_GETPIX1(DT_Q, ctl,
        &input, 0, i, &hist.freq[i]);	/* incidentally convert to ulong	*/
	output_name = NULL;					/* no possibility of thresholding	*/
  }
  else									/* normal image being histogrammed? */
  {
    DT_C(dtimage_hist_alloc,(ctl,		/* space for histogram				*/
      &input, &hist));
    DT_C(dtimage_hist,(ctl,				/* make histogram of input			*/
      &input, &hist));
  }

  width = hist.width;

  DT_C(dtos_malloc2,(ctl,				/* get space for freq array 		*/
    (void **)&freq, 
    width * sizeof(*freq),
    F, "freq array"));

  for (i=0; i<width; i++)				/* convert histogram to double 		*/
    freq[i] = hist.freq[i];

  DT_C(dtos_malloc2,(ctl,				/* get space for pds 				*/
    (void **)&pds, 
    (width + 1) * sizeof(*pds),
    F, "pds array"));
  
  DT_C(dtos_malloc2,(ctl,				/* get space for mark array 		*/
    (void **)&mark, 
    width * sizeof(*mark),
    F, "mark array"));

  DT_C(dt_sez90_pds,(					/* calculate peak detection signal	*/
    param, freq, width, pds));

  DT_C(dt_sez90_mark,(					/* mark peaks						*/
    param, freq, width, pds, mark));

  DT_C(dt_sez90_ends,(					/* get peaks at the ends			*/
    param, freq, width, mark));

  DT_C(dt_sez90_plot,(param,			/* make annotated histogram plot 	*/
	&hist, width, pds, mark,
    plot_name));


  if (output_name != NULL &&			/* output name is given?		    */
      strlen(output_name) > 0 &&
      strcmp(output_name, "-"))			/* and it's not just a hyphen?	   	*/
  {
    thresh = freq;						/* reuse freq space as thresh 		*/
    values = pds;						/* reuse pds space as values 		*/
	DT_C(dt_sez90_thresh,(param,		/* make multi-threshold display 	*/
      &input, mark, width,
      &input,							/* output to same image 			*/
      thresh, values));					/* reuse these for work space		*/
    DT_C(dtimage_export,(ctl,			/* display multithresholded image   */
      &input, output_name, -1, -1));
  }

  i = 1;
  end = 0;
  do {
    DT_C(dt_sez90_peak_next,(param,		/* find next peak				    */
      mark, width, end,
      &start, &maximum, &end));
    if (end < width)
      dt_say(ctl, F,
        values != NULL?
        "%2d. peak start %3ld"
        "  maximum %3ld  end %3ld"
        "  assigned gray %3d":
        "%2d. peak start %3ld"
        "  maximum %3ld  end %3ld",
        i, start, maximum, end,
        values != NULL?
        (int)values[i-1]: 0);
	 i++;
  } while (end < width);

cleanup:

  if (mark != NULL)  
    DT_C(dtos_free2,(ctl,				/* free space for mark array 		*/
      mark, 
      F, "mark array"));
  if (pds != NULL)  
    DT_C(dtos_free2,(ctl,				/* free space for pds array 		*/
      pds, 
      F, "pds array"));
  if (freq != NULL)  
    DT_C(dtos_free2,(ctl,				/* free space for freq array 		*/
      freq, 
      F, "freq array"));

  if (input.xe)
    DT_I(dtimage_free,(ctl, &input));	/* free input image 				*/

  return rc;
}

/*..........................................................................*/

dt_rc_e 
dt_sez90_cmd_check(						/* check params for this program	*/
  dt_ctl_t *ctl,
  dtparam_t *param,
  dt_enter_arg_t *arg,
  char *msg,
  const char **start)
{
  DT_F("dt_sez90_cmd_check");
  int op;
  int i;

  DT_Q(dtstr_choice,(ctl, ops,			/* make sure op is understood	    */
    param->op, &op));

  if (op == -1)
  {
    sprintf(msg,
      "Sorry, op \"%s\" is not understood", 
      param->op);
    *start = param->op_desc;			/* put cursor at this field		    */
    return DT_RC_GOOD;
  }

  if (op < -1)
  {
    sprintf(msg,
      "Sorry, op \"%s\" is ambiguous",
      param->op);
    *start = param->op_desc;			/* put cursor at this field		    */
    return DT_RC_GOOD;
  }

  if (param->n % 2 != 1)				/* n parameter must be odd		    */
  {
    sprintf(msg,
      "Sorry, n param %ld is not odd",
      param->n);
    *start = param->n_desc;				/* put cursor at this field		    */
    return DT_RC_GOOD;
  }

  if (strcmp(param->op, OP_TEST) &&		/* test and time have no args	    */
      strcmp(param->op, OP_TIME))
  for (i=0; i<2; i++)					/* check first two args for a value	*/
  {
    int l = strlen(arg[i].val);
    int j;

    for (j=0; j<l; j++)					/* search value for non-blank	    */
      if (arg[i].val[j] != ' ')
        break;
    if (j == l)
    {
      sprintf(msg, 
        "Please enter something for \"%s\"",
        arg[i].desc);
      *start = arg[i].val;				/* position cursor on this field    */
      return DT_RC_GOOD;
    }
  }

  return DT_RC_GOOD;
}

/*..........................................................................*/

dt_rc_e 
dt_sez90_cmd_parse(						/* parse params for this program	*/
  dtparam_t *param,
  int argc,
  char *argv[],
  dt_enter_arg_t *arg)
{
  DT_F("dt_sez90_cmd_parse");
  dt_ctl_t *ctl = &param->ctl;
  dt_rc_e rc = DT_RC_GOOD;

  DT_GI(dtparam_init,(param));			/* init param structure             */

  DT_GI(dtparam_set,(param,				/* set default for params we want	*/
    DTPARAM_FLAG_PRIMARY,
    &param->n,          "Window size", NULL, "15",
    &param->dx,         "Closeness",   NULL, "3",
    NULL));

  DT_GI(dtparam_set,(param, 
    DTPARAM_FLAG_SECONDARY,
    &param->op,         NULL,                    NULL, OP_DEFAULT,
    &param->background, "Darkest output pixel",  NULL, "1",
    &param->foreground, "Lightest output pixel", NULL, "255",
    &param->x,          NULL,                    NULL, NULL,
    &param->y,          NULL,                    NULL, NULL,
    NULL));

  DT_GI(dt_interactive_parse,(			/* parse command line interactively	*/
    argc, argv, "dtsez90", usage, param,
    arg, dt_sez90_cmd_check));

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
