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
  19991217 DE changed some example printf casts
 *..........................................................................*/



#include <dtack/base.h>
DT_RCSID("source/low $RCSfile: hist.c,v $ $Revision: 1.4 $");
#include <dtack/os.h>

/*..........................................................................
| NAME
| dthist_alloc() - allocate histogram
| 
| DESCRIPTION
|_index histogram,, allocation
| dthist_alloc() allocates space for a histogram and initializes
| the given structure.
|
| The ~hist~ structure is initialized to contain
| ~width~ bins representing data values to be mapped from
| the range ~minval~ up to ~maxval~.
|
| The histogram structure allocated by this function should be
|_Aside Resources need to be freed.
| ultimately freed by calling dthist_free().
| 
| Upon allocation, the histogram frequency bins in the array
|_aside You might want to zero the frequencies.
| ~hist->freq~ will have uninitialized values.
| Therefore, they will normally need to be cleared.
| dthist_clear() is a convenient means to do this.
|
| RETURN VALUES
| The ~*hist~ structure is initialized,
| unless an error condition is returned in which case ~*hist~ is unchanged.
|dtinclude(return1.dx)
| 
| ERRORS
| Unsuccessful completion may be due to:
| - invalid arguments
| - unable to allocate space for the histogram frequency array
| 
| CAVEATS
| The ~hist->freq~ array is not cleared to zero by this function.
| Use dthist_clear().
|
| When accumulating data into the histogram,
|_aside Binning data precision loss.
| downscaling of data values with concomitant precision loss
| will occur if the difference ~maxval-minval~ is greater than ~width~.
| 
| When accumulating data into the histogram,
|_Aside Bin clipping.
| data values falling outside the range of ~minval~ to ~maxval~
| will be assigned to the lowest and highest bins, respectively.
| 
| SEE ALSO
| Please refer to the
|_hyper section, The :hist: Function Group Introduction
| 

| The following functions are also related:
| dthist_free()						free space allocated by dthist_alloc()
| dthist_clear()					clear histogram frequency counts to 0
| dthist_add_values()				accumulate frequency counts for a data array
|
| EXAMPLES
| For a complete and working example program, please see the
|_hyper section, Example :dthist1.c:
| 
| The following code snippet allocates a histogram
|_aside No data precision loss.
| in which data values map directly into their respective bins.
| 
|:  dthist_t hist;
|:  dthist_alloc(NULL, &hist, 256,		// allocate the direct histogram
|:    0, 256);
|:  dthist_clear(NULL, &hist);			// clear frequency counts
| 
| The following snippet allocates histogram of 100 bins prepared to
|_aside Precision reduction by binning.
| handle data in the range -1000 up to 1000.
| Binning will occur by dividing the data values by 20 and offsetting.
| 
|:  dthist_t hist;
|:  dthist_alloc(NULL, &hist, 100,		// allocate the 100-bin histogram
|:    -1000.0, 1000.0);
|:  dthist_clear(NULL, &hist);			// clear frequency counts
|
| END
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dthist_alloc2(							/* allocate space for histogram 	*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dthist_t *hist,						/* histogram description structure 	*/
  dthist_bin_t width,					/* desired number of bins 			*/
  double minval,						/* data value at lowest bin 		*/
  double maxval,						/* data value above highest bin 	*/
  dtntype_e ntype)						/* histogram contents data type 	*/
{
  DT_F("dthist_alloc");
  static dt_ntype_e ntypes[] = {
    DT_NTYPE_B08,
    DT_NTYPE_B16,
    DT_NTYPE_B32,
    DT_NTYPE_S1,
    DT_NTYPE_S2,
    DT_NTYPE_S4,
    DT_NTYPE_F1,
    DT_NTYPE_F2,
    DT_NTYPE_NULL
  };
  int i;
  void *freq;

  if (width == 0)			
    return dt_err(ctl, F,
      "width %u is invalid", width);

  if (maxval <= minval)		
    return dt_err(ctl, F,
      "maxval %g is invalid"
      " for minval %g",
      maxval, minval);

  for (i=0; ntypes[i] != 0; i++)		/* scan list of valid ntypes		*/
    if (ntype == ntypes[i])
      break;  

  if (ntypes[i] == 0)
  {
	char s[256];
	dtntype_to_string(ctl, ntype,
      s, sizeof(s));
	return dt_err(ctl, F,
	  "ntype \"%s\" not supported", s);
  }

  DT_Q(dtos_malloc2,(ctl,				/* get space for frequency bins 	*/
    &freq,
    width * dt_ntype_sizeof[ntype],
    F, "hist->freq"));

  DT_MEMSET(hist, 0, sizeof(*hist));		/* clear histogram structure 		*/

  hist->freq = freq;
  hist->dscale = (double)width /		/* set up data scaling number 		*/
    (maxval - minval);
  hist->minval = minval;
  hist->maxval = maxval;
  hist->width = width;
  hist->ntype = ntype;

  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/
dt_rc_e
DTCONFIG_API1
dthist_alloc(							/* allocate space for histogram 	*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dthist_t *hist,						/* histogram description structure 	*/
  dthist_bin_t width,					/* desired number of bins 			*/
  double minval,						/* data value at lowest bin 		*/
  double maxval)						/* data value above highest bin 	*/
{
  DT_F("dthist_alloc");
  DT_Q(dthist_alloc2,(ctl, hist,
    width, minval, maxval, 
    DTHIST_FREQ2_NTYPE));				/* default ntype 					*/
  return DT_RC_GOOD;
}

/*..........................................................................
| NAME
| dthist_free() - free histogram
| 
| DESCRIPTION
| dthist_free() free space for a histogram which has been allocated
| by dthist_alloc().
|_index histogram,, freeing
|
| The histogram structure is freed.
|_aside Releases resources.
| The structure cleared itself is to 0.
|
| RETURN VALUES
| The ~hist~ structure is cleared to 0.
|dtinclude(return1.dx)
| 
| ERRORS
| Unsuccessful completion may be due to:
| - unable to free space for the histogram frequency array
| 
| CAVEATS
| Do not try to use this function on a ~hist~ structure which
|_aside Re-nitialize first.
| has not been initialized with dthist_alloc().
| 
| SEE ALSO
| Please refer to the
|_hyper section, The :hist: Function Group Introduction
| 
| The following functions are also related:
| dthist_alloc()		allocate space for a histogram
|
| EXAMPLES
| For a complete and working example program, please see the
|_hyper section, Example :dthist1.c:
| 
| The following example allocates and frees a histogram.
| 
|:  dthist_alloc(NULL, &hist, 256,		// allocate the histogram
|:    0, 256);
|:  dthist_free(NULL, &hist);			// free the histogram
|
| END
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dthist_free(							/* free space for histogram 		*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dthist_t *hist)						/* histogram description structure 	*/
{
  DT_F("dthist_free");
  if (hist->freq)
    DT_Q(dtos_free2,(ctl, hist->freq, 
      F, "hist->freq"));
  DT_MEMSET(hist, 0, sizeof(*hist));		/* clear histogram structure 		*/
  return DT_RC_GOOD;
}

/*..........................................................................
| NAME
| dthist_clear() - clear histogram counts
| 
| DESCRIPTION
| dthist_clear() clears histogram frequency counts to 0.
|_index histogram,, clearing
|_index zeroing,, histogram
|
| Each element of the frequency count array ~hist->freq~
| is cleared to 0.
| The histogram structure itself is not modified.
|
| This function should be called after the histogram
|_aside Clear before adding values.
| has been allocated and before data values are added to the histogram.
| It may be called multiple times before the histogram is freed.
| For example, once one histogram is complete,
| you could use dthist_clear() to clear the counts prior to
| reusing the histogram structure.
|
| RETURN VALUES
| All bins in the frequency array are cleared to 0.
|dtinclude(return1.dx)
| 
| ERRORS
|dtinclude(noerror.dx)
| 
| CAVEATS
| Do not try to use this function on a ~hist~ structure which
|_aside Initialize first.
| has not been initialized with dthist_alloc().
| 
| SEE ALSO
| Please refer to the
|_hyper section, The :hist: Function Group Introduction
| 
| The following functions are also related:
| dthist_alloc()		allocate space for a histogram
|
| EXAMPLES
| For a complete and working example program, please see the
|_hyper section, Example :dthist1.c:
| 
| The following code snippet allocates and clears a histogram.
| 
|:  dthist_alloc(NULL, &hist, 256,		// allocate the histogram
|:    0, 256);
|:  dthist_clear(NULL, &hist);			// clear the histogram
|
| END
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dthist_clear(							/* clear histogram counts 			*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dthist_t *hist)						/* histogram description structure 	*/
{
  DT_MEMSET(hist->freq, 0,
    hist->width * dt_ntype_sizeof[hist->ntype]);
  return DT_RC_GOOD;
}

/*..........................................................................
| NAME
| dthist_scale() - scale histogram counts
| 
| DESCRIPTION
| dthist_scale() scales the histogram frequency counts
| to fall within desired range.
|_index histogram,, scaling
|_index scaling,, histogram
|
| Each element of the frequency count array ~hist->freq~
| is multiplied by a constant scale factor.
| The scale factor is set to the largest value in the array divided
| by ~height~.
|
| This function linearly scales all bars of the histogram
|_aside Useful before plotting.
| such that they fall within a prescribed height.
| It is useful when preparing a histogram for plotting, for example.
|
| This function scales the values in the ~hist->freq~ array,
| not the number of bins or the data range they cover.
| The histogram structure itself is not modified.
| 
| If all bins contain the frequency count of 0,
| then no scaling takes place.
|
| RETURN VALUES
| All bin counts in the frequency array are scaled proportionately.
|dtinclude(return1.dx)
| 
| ERRORS
|dtinclude(noerror.dx)
| 
| CAVEATS
| Due to numeric round-off, the resulting highest value
| in ~hist->freq~ may not be exactly ~height~.
| 
| Do not try to use this function on a ~hist~ structure which
|_aside Initialize first.
| has not been initialized with dthist_alloc().
| 
| SEE ALSO
| Please refer to the
|_hyper section, The :hist: Function Group Introduction
| 
| The following functions are also related:
| dthist_alloc()		allocate space for a histogram
| dthist_clear()		clear histogram counts
|
| EXAMPLES
| For a complete and working example program, please see the
|_hyper section, Example :dthist2.c:
| 
| The following code snippet scales a histogram to a max frequency count
| of 128.
| 
|:  dthist_scale(NULL, &hist, 128);	   	// scale the histogram
|
| END
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dthist_scale(							/* scale histogram counts			*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dthist_t *hist,						/* histogram description structure 	*/
  double height)						/* desired max height of bars  		*/
{
  dt1d_i_t imax;
  double vmax;
  double scale;
  
  dt1d_range(hist->ntype,				/* find max frequency 				*/
    hist->freq, hist->width, 
    NULL, &imax);

  DTNTYPE_GET1(DT_Q, ctl,				/* fetch value from freq array 		*/
    hist->freq, imax, hist->ntype,
    &vmax);
 
  if (vmax != 0)
  {
    scale = height / vmax;				/* compute conversion scale			*/
  
    dt1d_mult(hist->ntype,				/* convert to desired scale		    */
      hist->freq, hist->width, 
      (long)scale,
      hist->freq, hist->width, 0, 0,
      hist->freq, hist->width);
  }

  return DT_RC_GOOD;
}

/*..........................................................................
| NAME
| dthist_bin_to_value() and macros - get floor and ceiling of bin
| 
| DESCRIPTION
| dthist_bin_to_value() returns the data value range 
| associated with the given bin.
| DTHIST_BIN_TO_MINVALUE() returns just the minimum value, and
| DTHIST_BIN_TO_MAXVALUE() returns just the maximum.
|_index DTHIST_BIN_TO_MINVALUE()
|_index DTHIST_BIN_TO_MAXVALUE()
|
| For the given ~bin~ in the histogram ~hist~,
|_aside Returns bin limits.
| return the ~*floor~ (minimum data value)
| and/or ~*ceiling~ (maximum data value).
| For the function form dthist_bin_to_value(),
| either or both of the arguments ~*floor~ or ~*ceiling~
| may be NULL, indicating you are not interested in that particular value.
| If any functions are called from within the macro forms,
| use the ~check~ macro to process the return code.
|
| During binning done on this histogram by dthist_add_values(),
| all data values between ~*floor~ and ~*ceiling~,
| but not including ~*ceiling~, are mapped to ~bin~.
| 
| Neither the function nor the macro forms 
| check to make sure that ~bin~ is
| greater than or equal to 0 and less than ~hist->width~.
| If ~bin~ is outside these limits,
| then data values between the resulting ~floor~ and ~ceiling~,
| when used in dthist_value_to_bin() or dthist_add_values(),
| will be mapped to the lowest or highest actual bin.
| 
| If you think of the histogram bin as a bar in a bar chart,
|_aside Bin as a bar.
| then the ~*floor~ is the x coordinate of the left side of the bar
| and the ~*ceiling~ is just past the right side of the bar.
| 
| The two macros are more efficient,
|_aside Macros quicker.
| especially if only one returned value is desired.
| The cost of more efficiency is increased object code size
| and possibly less error checking.
| For the macro forms, the returned ~*floor~ and ~*ceiling~
| can be any scalar type to which a :double: can be assigned.
| 
| This function will always return the ~*ceiling~ of ~bin~
|_Aside Bin adjacency.
| the same as the ~*floor~ of the next bin.
| If the histogram was allocated such that ~hist->dscale~ is 1.0,
| then ~*floor~ is returned as a non-fractional number and
| ~*ceiling~ is returned as ~floor+1~.
|
| If the macros DTHIST_BIN_TO_MINVALUE() or DTHIST_BIN_TO_MAXVALUE()
|_Aside Check macros.
|_index check macros
| need to call a function,
| the return code from the function will be processed with
| the ~check~ macro.
|dxinclude(check1.dx)
|
| This function is the dual of dthist_value_to_bin().
|_Aside Function's dual.
|
| RETURN VALUES
| Values are returned in the ~*floor~ and/or ~*ceiling~ arguments,
| assuming the pointers are not NULL.
|dtinclude(return1.dx)
| 
| ERRORS
|dtinclude(noerror.dx)
| 
| CAVEATS
| Neither the function nor the macro forms 
| check to make sure that ~bin~ is
| greater than or equal to 0 and less than ~hist->width~.
| 
| Do not try to use this function on a ~hist~ structure which
|_aside Initialize first.
| has not been initialized with dthist_alloc().
| 
| SEE ALSO
| Please refer to the
|_hyper section, The :hist: Function Group Introduction
| 
| The following functions are also related:
| dthist_alloc()			allocate space for a histogram
| dthist_value_to_bin()		the dual of this function
|
| EXAMPLES
| For a complete and working example program, please see the
|_hyper section, Example :dthist2.c:
| 
| The following code snippet loops through all histogram bins,
| printing the data value range of each bin.
|
|:  dthist_hist_t hist;
|:  dthist_bin_t bin;
|:  for (bin=0; bin<hist.width; bin++)	// loop for each bin
|:  {
|:    double floor, ceiling;
|:    dthist_bin_to_value(NULL, &hist,	// get bin data range
|_aside Bin's bounds.
|:      bin, &floor, &ceiling);
|:    printf("bin[%lu]"					// print bin information
|:      " range %g-%g count %ld",
|:      bin, floor, ceiling,
|:      hist->freq[bin]);
|:  }
| 
| The following snippet does the same thing but uses the macro forms.
|
|:  dthist_hist_t hist;
|:  dthist_bin_t bin;
|:  for (bin=0; bin<hist.width; bin++)	// loop for each bin
|:  {
|:    double floor, ceiling;
|:    DTHIST_BIN_TO_MINVALUE(DT_N, 		// get bin data minimum
|:      NULL, &hist, bin, &floor);
|:    DTHIST_BIN_TO_MAXVALUE(DT_N, 		// get bin data maximum
|:      NULL, &hist, bin, &ceiling);
|:    printf("bin[%u]"					// print bin information
|:      " range %g-%g count %ld",
|:      (unsigned int)bin, floor, ceiling,
|:      hist->freq[bin]);
|:  }
|
| END
 *..........................................................................*/

dt_rc_e 
DTCONFIG_API1
dthist_bin_to_value(					/* get bin floor and ceiling 		*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dthist_t *hist,						/* histogram description structure 	*/
  dthist_bin_t bin,						/* bin to query 					*/
  double *floor,						/* returned floor 					*/
  double *ceiling)						/* returned ceiling 				*/
{
  if (floor != NULL)
    DTHIST_BIN_TO_MINVALUE(DT_Q, ctl, hist, bin, floor);
  if (ceiling != NULL)
    DTHIST_BIN_TO_MAXVALUE(DT_Q, ctl, hist, bin, ceiling);
  return DT_RC_GOOD;
}

#ifdef NOCOMPILE
void
DTCONFIG_API1
DTHIST_BIN_TO_MINVALUE(					/* get bin floor   					*/
  dt_rc_check_f *check,					/* return code check function 		*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dthist_t *hist,						/* histogram description structure 	*/
  dthist_bin_t bin,						/* bin to query 					*/
  double *floor)						/* returned floor 					*/
{
}

void
DTCONFIG_API1
DTHIST_BIN_TO_MAXVALUE(					/* get bin ceiling 					*/
  dt_rc_check_f *check,					/* return code check function 		*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dthist_t *hist,						/* histogram description structure 	*/
  dthist_bin_t bin,						/* bin to query 					*/
  double *ceiling)		  				/* returned ceiling 				*/
{
}
#endif

/*..........................................................................
| NAME
| dthist_value_to_bin() and macros - get floor and ceiling of bin
| 
| DESCRIPTION
| dthist_value_to_bin() returns the bin to use for the given data value.
|_index histogram,, binning
|_index binning,, histogram
|_index DTHIST_VALUE_TO_BIN()
|
| For the given ~value~, return the corresponding ~*bin~ 
| in the histogram ~hist~.
| If any functions are called from within the macro form,
| use the ~check~ macro to process the return code.
|
| The ~*bin~ is computed according to the equation:
|_aside All numbers mapped to some bin.
| 
|:  (data - hist->minval) / hist->dscale.
| 
| The bin 0, i.e. the lowest bin,
| is returned for any ~value~ less than ~hist->minval~.
| The highest bin, ~hist->width-1~,
| is returned for any ~value~ greater than or equal to ~hist->maxval~.
| In this way, the sum of the histogram frequency counts
| will equal the number of data items processed.
| 
| If you are simply accumulating frequency counts in the bins,
| then you should use dthist_add_values() instead of this function.
| However, this function will be necessary to do any other operations
|_aside Direct operation on bin.
| requiring knowledge of the bin corresponding to a specific data value.
| 
| The macro form is more efficient.
|_aside Macro is quicker.
| The cost more efficiency is increased object code size
| and possibly less error checking.
| For the macro form, the returned ~*bin~
| can be any scalar type to which a :dthist_bin_t: can be assigned.
|
| If the macro DTHIST_VALUE_TO_BIN() needs to call a function,
|_Aside Check macros.
|_index check macros
| the return code from the function will be processed with
| the ~check~ macro.
|dxinclude(check1.dx)
|
| This function is the dual of dthist_bin_to_value().
|_Aside Function's dual.
|
| RETURN VALUES
| Value is returned in the ~*bin~ argument.
|dtinclude(return1.dx)
| 
| ERRORS
|dtinclude(noerror.dx)
| 
| CAVEATS
| Do not try to use this function on a ~hist~ structure which
|_aside Initialize first.
| has not been initialized with dthist_alloc().
| 
| SEE ALSO
| Please refer to the
|_hyper section, The :hist: Function Group Introduction
| 
| The following functions are also related:
| dthist_alloc()		allocate space for a histogram
| dthist_bin_to_value()	the dual of this function
|
| EXAMPLES
| For a complete and working example program, please see the
|_hyper section, Example :dthist1.c:
| 
| The following code snippet returns the bin for a data value.
|
|:  dthist_hist_t hist;
|:  dthist_bin_t bin;
|:  dthist_value_to_bin(NULL, &hist,	// return bin for value
|:    25.0, &bin);
| 
| The following snippet does the same thing with a macro.
|
|:  dthist_hist_t hist;
|:  dthist_bin_t bin;
|:  DTHIST_VALUE_TO_BIN(DT_N, NULL,		// return bin for value
|:    &hist, 25.0, &bin);
|
| END
 *..........................................................................*/


dt_rc_e 
DTCONFIG_API1
dthist_value_to_bin(					/* get floor and ceiling of bin 	*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dthist_t *hist,						/* histogram description structure 	*/
  double value,							/* value whose bin is desired 		*/
  dthist_bin_t *bin)					/* returned bin for value 			*/
{
  DTHIST_VALUE_TO_BIN(DT_Q, ctl, hist, value, bin);
  return DT_RC_GOOD;
}

#ifdef NOCOMPILE
void
DTCONFIG_API1
DTHIST_VALUE_TO_BIN(					/* macro form 						*/
  dt_rc_check_f *check,					/* return code check function 		*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dthist_t *hist,						/* histogram description structure 	*/
  double value,							/* value whose bin is desired 		*/
  dthist_bin_t *bin)					/* returned bin for value 			*/
{
}
#endif

/*..........................................................................
 * returns the expected value from a Gaussian distribution
 * the stats structure must contain the mean and variance
 *..........................................................................*/

dt_rc_e 
DTCONFIG_API1
dthist_bin_gaussian(
  dt_ctl_t *ctl,						/* environment control 				*/
  void *arg,
  double in,
  double *out)
{
  dtstats_t *stats =
    (dtstats_t *)arg;
  *out = stats->sum *
    exp(-((in - stats->mean) *
		  (in - stats->mean)) /
		(2.0 * stats->variance)) /
    sqrt(stats->variance * 2.0 * DT_PI);
  return DT_RC_GOOD;
}

/*..........................................................................
 * returns the expected out from a uniform distribution
 * the stats structure must contain the total area and width=number of bins
 *..........................................................................*/

dt_rc_e 
DTCONFIG_API1
dthist_bin_uniform(
  dt_ctl_t *ctl,						/* environment control 				*/
  void *arg,
  double in,
  double *out)
{
  dtstats_t *stats =
    (dtstats_t *)arg;
  *out = (double)stats->area / 
         (double)stats->width;
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
