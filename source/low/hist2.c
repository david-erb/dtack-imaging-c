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
DT_RCSID("source/low $RCSfile: hist2.c,v $ $Revision: 1.4 $");
#include <dtack/os.h>

/*..........................................................................
| NAME
| dthist_add_values() - accumulate frequency counts
| 
| DESCRIPTION
| dthist_add_values() accumulates frequencies for given array of values.
|_index histogram,, binning
|_index binning,, histogram
|_index histogram,, accumulating
|_index accumulating,, histogram
|_index histogram,, adding values to
|
| For each element of the array ~values~
| starting with ~beg~ and ending before ~end~,
| add 1 to the frequency counts of each corresponding bin in histogram ~hist~.
| are processed.
| Array values are assumed of type ~ntype~.
|
| This function does histogram binning.
| For each element of ~*values~,
| the bin to the used for incrementing is
|_aside Bin equation.
| computed according to the equation:
| 
|:  dthist_bin_t bin = (data - hist->minval) * hist->dscale.
| 
| The bin 0, i.e. the lowest bin,
|_aside Tails tucked in.
| is incremented for any ~value~ element less than ~hist->minval~.
| The highest bin, ~hist->width-1~,
| is incremented for any ~value~ element
| greater than or equal to ~hist->maxval~.
| In this way, the sum of the histogram frequency counts
| will equal the number of data items processed.
|
| Histograms with ~hist->minval~ of 0 and ~hist->dscale~ of 1.0
|_aside Extra efficiency.
| are processed more efficiently when ~ntype~ is one of the unsigned
| integral types: :DT_NTYPE_B08:, :DT_NTYPE_B16:, :DT_NTYPE_B32:.
| Special consideration is given to the case where
| ~hist->minval~ is 0, ~hist->maxval~ is 256 or greater, 
| ~hist->dscale~ is 1.0, and
| the input data is of ~ntype~ :DT_NTYPE_B08:.
| This is the case where the histogram has 256 bins and the data
| is :unsigned char:.
| 
| If you need to perform other operations on the bins
|_aside Access to bin.
| besides simply incrementing them,
| use the dthist_value_to_bin() function to determine the bin
| which corresponds to a specific data value.
|
| The array of ~values~ is assumed one-dimensional with a stride of one,
|_aside Data array in regular memory.
| and entirely mapped.
| This means that all elements are located in sequential memory
| locations and no elements are skipped between ~beg~ and ~end~.
| No special instructions are issued to
| bring parts of the array into main memory from virtual memory.
|
| If ~ntype~ is not one of the allowed types,
|_aside No error on invalid data type.
| this function will silently treat all data ~values~ as though
| they had the value 0.
| Only one bin will be incremented:
| the one accumulating ~values~ of 0.
| Allowed values for ~ntype~ are:
| 
| #ntype#							#C type#
| :DT_NTYPE_B01: alias :DT_BIT:		single bit
| :DT_NTYPE_B08: alias :DT_UCHAR:	unsigned char
| :DT_NTYPE_B16: alias :DT_USHORT:	unsigned short
| :DT_NTYPE_B32: alias :DT_ULONG:	unsigned long
| :DT_NTYPE_S1: alias :DT_CHAR:		char
| :DT_NTYPE_S2: alias :DT_SHORT:	short
| :DT_NTYPE_S4: alias :DT_LONG:		long
| :DT_NTYPE_F1: alias :DT_FLOAT:	float
| :DT_NTYPE_F2: alias :DT_DOUBLE:	double
| 
| RETURN VALUES
| Elements of ~hist->freq~ are incremented according to input values.
|dtinclude(return1.dx)
| 
| ERRORS
|dtinclude(noerror.dx)
| 
| CAVEATS
| The function produces no error if ~ntype~ is invalid.
| 
| Do not try to use this function on a ~hist~ structure which
|_aside Be sure to initialize first.
| has not been initialized with dthist_alloc().
| 
| SEE ALSO
| Please refer to the
|_hyper section, The :hist: Function Group Introduction
| 
| The following functions are also related:
| dthist_alloc()		allocate space for a histogram
| dthist_clear()		clear frequency counts
| DTHIST_VALUE_TO_BIN()	return bin for given data value
|
| EXAMPLES
| For a complete and working example program, please see the
|_hyper section, Example :dthist2.c:
| 
| The following code snippet highlights this function:
| 
|:  dthist_add_values(NULL, &hist,		// add data to histogram
|:    data, 0, 1000, 					// there are 1000 data items
|:    DT_NTYPE_F2);						// data is double precision
|
| END
 *..........................................................................*/

dt_rc_e 
DTCONFIG_API1
dthist_add_values_fast(					/* accumulate frequencies fast		*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dthist_t *hist,						/* histogram description structure 	*/
  void *values,							/* array of input data values 		*/
  unsigned int beg,						/* start index						*/
  unsigned int end,						/* end index						*/
  dt_ntype_e ntype,						/* type of input data values 		*/
  int *cando)							/* return true if we did it 		*/
{
  DT_F("dthist_add_values_fast");

#define DIRECT1(TYPE) \
{ \
  dthist_freq2_t * const f = \
    (dthist_freq2_t *)hist->freq; \
  const TYPE * const p = \
    (TYPE *)values; \
  for (i=beg; i<end; i++) \
    f[p[i]]++; \
}

#define DIRECT2(TYPE) \
{ \
  dthist_freq2_t * const f = \
    (dthist_freq2_t *)hist->freq; \
  const TYPE * const p = \
    (TYPE *)values; \
  for (i=beg; i<end; i++) \
    if (p[i] <= w) \
      f[(unsigned int)p[i]]++; \
    else \
      f[w]++; \
}

  const dthist_bin_t w = hist->width-1;
  unsigned int i;

  *cando = 1;							/* presume we can do it fast 		*/

  if (ntype == DT_NTYPE_B08 &&			/* direct mapping to bins? 			*/
      hist->ntype ==					/* and default ntype? 				*/
	    DTHIST_FREQ2_NTYPE &&
      hist->dscale == 1.0 &&		
	  hist->minval == 0.0 &&
	  hist->maxval >= 256.0)
    DIRECT1(dt_ntype_b08_t)
  else									
  if (ntype == DT_NTYPE_B08 &&			/* direct mapping with clipping?	*/
      hist->ntype ==					/* and default ntype? 				*/
	    DTHIST_FREQ2_NTYPE &&
      hist->dscale == 1.0 &&		
	  hist->minval == 0.0)
    DIRECT2(dt_ntype_b08_t)
  else									
  if (ntype == DT_NTYPE_B16 &&
      hist->ntype ==					/* and default ntype? 				*/
	    DTHIST_FREQ2_NTYPE &&
      hist->dscale == 1.0 &&		
	  hist->minval == 0.0)
    DIRECT2(dt_ntype_b16_t)
  else								
  if (ntype == DT_NTYPE_B32 &&
      hist->ntype ==					/* and default ntype? 				*/
	    DTHIST_FREQ2_NTYPE &&
      hist->dscale == 1.0 &&		
	  hist->minval == 0.0)
    DIRECT2(dt_ntype_b32_t)
  else									/* not directly mapped? 			*/
    *cando = 0;							/* then it's not fast 				*/

  return DT_RC_GOOD;
}
/*..........................................................................
 *..........................................................................*/

dt_rc_e 
DTCONFIG_API1
dthist_add_values(						/* accumulate frequencies 			*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dthist_t *hist,						/* histogram description structure 	*/
  void *values,							/* array of input data values 		*/
  unsigned int beg,						/* start index						*/
  unsigned int end,						/* end index						*/
  dt_ntype_e ntype)						/* type of input data values 		*/
{
  DT_F("dthist_add_values");
  int cando;
  const dthist_bin_t w = hist->width-1;
  unsigned int i;
  
  DT_Q(dthist_add_values_fast,(ctl,		/* try to do it fast 				*/
    hist, values, beg, end, ntype,
    &cando));

  if (cando)							/* we did it fast? 					*/
	return DT_RC_GOOD;					/* we can quit now 					*/

  if (hist->ntype ==					/* default ntype? 					*/
	  DTHIST_FREQ2_NTYPE)
  {
    double value = 0;
    dthist_bin_t bin;
	dthist_freq2_t * const f = 
      (dthist_freq2_t *)hist->freq; 
    for (i=beg; i<end; i++)
    {
	  DTNTYPE_GET1(DT_Q, ctl,			/* fetch value from input array 	*/
        values, i, ntype, &value);
	  DTHIST_VALUE_TO_BIN(DT_Q, ctl,	/* find the associated bin 			*/
        hist, value, &bin);
	  f[bin]++;							/* increase frequency count for bin */
    }
  }
  else									/* not default ntype?				*/
  {
	char s[256];
	dtntype_to_string(ctl, ntype,
      s, sizeof(s));
	return dt_err(ctl, F,
	  "ntype \"%s\" not supported", s);
  }

  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/

dt_rc_e 
DTCONFIG_API1
dthist_stats(							/* calculate statistics				*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dthist_t *hist,						/* histogram description structure 	*/
  dtstats_t *stats)
{
  DT_F("dthist_stats");
  dthist_freq2_t * const freq =
    (dthist_freq2_t *)hist->freq;
  const dthist_bin_t w = hist->width;
  dthist_bin_t bin;
  long area;
  double sum, mean, variance;
  double standard_deviation;
  dt_rc_e rc = DT_RC_GOOD;

  if (hist->ntype !=					/* not default ntype? 				*/
	  DTHIST_FREQ2_NTYPE)
  {
	char s[256];
	dtntype_to_string(ctl, hist->ntype,
      s, sizeof(s));
	return dt_err(ctl, F,
	  "ntype \"%s\" not supported", s);
  }

  sum = 0.0;
  area = 0;

  for (bin=0; bin<w;  bin++)			/* first pass 						*/
  {
	double min, max, d;
	DT_C(dthist_bin_to_value,(ctl,
      hist, bin, &min, &max));
	d = (max + min) / 2.0;
	sum += d * freq[bin];
	area += freq[bin];
  }

  mean = sum / (area? area: 1.0);
  variance = 0.0;
  for (bin=0; bin<w; bin++)				/* second pass 						*/
  {
	double min, max, d;
	DT_C(dthist_bin_to_value,(ctl,
      hist, bin, &min, &max));
	d = (max + min) / 2.0;
	variance += freq[bin] *				/* squared difference from mean 	*/
      (d - mean) * (d - mean);
  }

  variance /= (area? area: 1.0);
  standard_deviation =  
    variance >= 0.0?
      sqrt(variance): 0;

  if (stats->want & DTSTATS_AREA)
  {
	stats->area = area;
	stats->have |= DTSTATS_AREA;
  }

  if (stats->want & DTSTATS_SUM)
  {
	stats->sum = sum;
	stats->have |= DTSTATS_SUM;
  }

  if (stats->want & DTSTATS_MEAN)
  {
	stats->mean = mean;
	stats->have |= DTSTATS_MEAN;
  }

  if (stats->want & DTSTATS_VARIANCE)
  {
	stats->variance = variance;
	stats->have |= DTSTATS_VARIANCE;
  }

  if (stats->want & DTSTATS_STANDARD_DEVIATION)
  {
	stats->standard_deviation = standard_deviation;
	stats->have |= DTSTATS_STANDARD_DEVIATION;
  }

cleanup:
  return rc;
}

/*..........................................................................
 *..........................................................................*/

dt_rc_e 
DTCONFIG_API1
dthist_gen(								/* generate histogram				*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dthist_t *hist,						/* histogram description structure 	*/
  dt_double_f *double_func,
  void *arg)
{
  DT_F("dthist_chisq1");
  dthist_freq2_t * const freq =
    (dthist_freq2_t *)hist->freq;
  const dthist_bin_t w = hist->width;
  dthist_bin_t i;

  if (hist->ntype !=					/* not default ntype? 				*/
	  DTHIST_FREQ2_NTYPE)
  {
	char s[256];
	dtntype_to_string(ctl, hist->ntype,
      s, sizeof(s));
	return dt_err(ctl, F,
	  "ntype \"%s\" not supported", s);
  }
  
  for (i=0; i<w; i++)
  {
	double value;
	DT_Q((*double_func),(ctl, arg,
      i, &value));
	freq[i] = (dthist_freq2_t)
      (value + 0.5);
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
