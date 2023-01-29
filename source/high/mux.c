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

#include <dtack/base.h>
DT_RCSID("source/high $RCSfile: lookup.c,v $ $Revision: 1.1 $");
#include <dtack/mem.h>
#include <dtack/os.h>

/*..........................................................................
 *..........................................................................*/

static
dt_rc_e 
dtimage_band_put(						/* copy band into banded image		*/
  dt_ctl_t *ctl,
  dtimage_t *base,
  dtimage_t *band)
{
  DT_F("dtimage_band_put");
  dtxy_t x;
  dtxy_t y;
  for (y=0; y<ymax; y++)				/* for all rows						*/
  {
	void *p1, *q1;
	DTIMAGE_GETROW2(DT_Q, ctl,
      base, y, &basev);
	DTIMAGE_MAPROW2(DT_Q, ctl,
      band, y, &bandv);
	DT_C_1D(dt1d_
	{
  	  dt_ntype_b08_t * const p =		/* coursing pointers				*/
	    (dt_ntype_b08_t *)p1;
	  dt_ntype_b08_t * const q =
	    (dt_ntype_b08_t *)q1;
	  for (x=0; x<xmax; x++)			/* for all columns					*/
        q[x] = lut[p[x]];				/* do lookup					    */
	}
	DTIMAGE_UNMAPROW2(DT_Q, ctl,
      input, y);
	DTIMAGE_PUTROW2(DT_Q, ctl,
      output, y);
  }
  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtimage_lookup(							/* transform image by lut			*/
  dt_ctl_t *ctl,
  dtimage_t *input,						/* input image						*/
  double *lut,							/* transformation array				*/
  dtxy_t n,								/* number of transformation values	*/
  dtimage_t *output)					/* output image						*/
{
  DT_F("dtimage_lookup");
  dtxy_t xmax, ymax;
  dtxy_t i;
  dt_ntype_e ntypes[] =					/* datatypes we can handle			*/
    {DT_NTYPE_B08, DT_NTYPE_NULL};
  dt_rc_e rc;

  rc = dtimage_check_2f(ctl,			/* check simple stuff				*/
    input, "input",
    output, "output",
    ntypes, DTIMAGE_FLAG_PAGED,
    F, &xmax, &ymax);
  if (rc != DT_RC_GOOD)
    return rc;

  switch(input->ntype)					/* branch based on ntype			*/
  {
	case DT_NTYPE_B08:  
	{
      dt_ntype_b08_t l[256];
	  DT_MEMSET(l, '\0', sizeof(l));		/* clear the lut				    */
      for (i=0; i<n; i++)				/* shorten the lut					*/
        l[i] = DT_CAST(dt_ntype_b08_t, 
          lut[i]);
      DT_Q(dtimage_lookup_bb,(ctl,		/* transform via lookup			    */
        input, xmax, ymax,
        l, output));
	}
    break;
  }

  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtimage_lookup_b08(						/* transform image by lut			*/
  dt_ctl_t *ctl,
  dtimage_t *input,						/* input image						*/
  const dt_ntype_b08_t *lut,			/* transformation array				*/
  dtxy_t n,								/* number of transformation values	*/
  dtimage_t *output)					/* output image						*/
{
  DT_F("dtimage_lookup_b08");
  dtxy_t xmax, ymax;
  dt_ntype_e ntypes[] =					/* datatypes we can handle			*/
    {DT_NTYPE_B08, DT_NTYPE_NULL};
  dt_rc_e rc;

  rc = dtimage_check_2f(ctl,			/* check simple stuff				*/
    input, "input",
    output, "output",
    ntypes, DTIMAGE_FLAG_PAGED,
    F, &xmax, &ymax);
  if (rc != DT_RC_GOOD)
    return rc;

  switch(input->ntype)					/* branch based on ntype			*/
  {
	case DT_NTYPE_B08:  
	{
      dt_ntype_b08_t l[256];
	  DT_MEMCPY(l, lut, n);
	  DT_MEMSET(l+n, 0, 256-n);
      DT_Q(dtimage_lookup_bb,(ctl,		/* transform via lookup			    */
        input, xmax, ymax,
        l, output));
	}
    break;
  }

  return DT_RC_GOOD;
}

/*..........................................................................
 * added 5/13/98
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtimage_lookup_b16(						/* transform image by lut			*/
  dt_ctl_t *ctl,
  dtimage_t *input,						/* input image						*/
  const dt_ntype_b16_t *lut,			/* transformation array				*/
  dtxy_t n,								/* number of transformation values	*/
  dtimage_t *output)					/* output image						*/
{
  DT_F("dtimage_lookup_b16");
  dtxy_t xmax, ymax;
  dt_ntype_e ntypes[] =					/* datatypes we can handle			*/
    {DT_NTYPE_B16, DT_NTYPE_NULL};
  dtxy_t x;
  dtxy_t y;
  dt_rc_e rc;

  rc = dtimage_check_2f(ctl,			/* check simple stuff				*/
    input, "input",
    output, "output",
    ntypes, DTIMAGE_FLAG_PAGED,
    F, &xmax, &ymax);
  if (rc != DT_RC_GOOD)
    return rc;

  for (y=0; y<ymax; y++)				/* for all rows						*/
  {
	void *p1, *q1;
	DTIMAGE_GETROW2(DT_Q, ctl,
      input, y, &p1);
	DTIMAGE_MAPROW2(DT_Q, ctl,
      output, y, &q1);
	{
  	  dt_ntype_b16_t * const p =		/* coursing pointers				*/
	    (dt_ntype_b16_t *)p1;
	  dt_ntype_b16_t * const q =
	    (dt_ntype_b16_t *)q1;
	  for (x=0; x<xmax; x++)			/* for all columns					*/
        q[x] = lut[p[x]<n?				/* do lookup					    */
                 p[x]: p[n-1]];
	}
	DTIMAGE_UNMAPROW2(DT_Q, ctl,
      input, y);
	DTIMAGE_PUTROW2(DT_Q, ctl,
      output, y);
  }
  return DT_RC_GOOD;
}

/*..........................................................................
 * 98/06/09 added by FF
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtimage_lookup_s2(						/* transform image by lut			*/
  dt_ctl_t *ctl,
  dtimage_t *input,						/* input image						*/
  const dt_ntype_s2_t *lut,				/* transformation array				*/
  dtxy_t minval,						/* lowest transformation value		*/
  dtxy_t n,								/* number of transformation values	*/
  dtimage_t *output)					/* output image						*/
{
  DT_F("dtimage_lookup_s2");
  dtxy_t xmax, ymax;
  dt_ntype_e ntypes[] =					/* datatypes we can handle			*/
    {DT_NTYPE_S2, DT_NTYPE_NULL};
  dtxy_t x;
  dtxy_t y;
  dt_rc_e rc;

  rc = dtimage_check_2f(ctl,			/* check simple stuff				*/
    input, "input",
    output, "output",
    ntypes, DTIMAGE_FLAG_PAGED,
    F, &xmax, &ymax);
  if (rc != DT_RC_GOOD)
    return rc;

  for (y=0; y<ymax; y++)				/* for all rows						*/
  {
	void *p1, *q1;
	dtxy_t i;
	DTIMAGE_GETROW2(DT_Q, ctl,
      input, y, &p1);
	DTIMAGE_MAPROW2(DT_Q, ctl,
      output, y, &q1);
	{
  	  dt_ntype_s2_t * const p =			/* coursing pointers				*/
	    (dt_ntype_s2_t *)p1;
	  dt_ntype_s2_t * const q =
	    (dt_ntype_s2_t *)q1;
	  for (x=0; x<xmax; x++)			/* for all columns					*/
	  {
	    i = p[x]-minval;				/* map to lut index					*/
	    i = (i < 0) ? 0 : i;			/* check for underflow				*/
	    i = (i >= n) ? n-1 : i;			/* check for overflow				*/
	    q[x] = lut[i];					/* do lookup					    */
	  }
	}
	DTIMAGE_UNMAPROW2(DT_Q, ctl,
      input, y);
	DTIMAGE_PUTROW2(DT_Q, ctl,
      output, y);
  }
  return DT_RC_GOOD;
}

/*..........................................................................
 * do it slow
 *..........................................................................*/
dt_rc_e 
DTCONFIG_API1
dtimage_lookup_hist_slow(				/* lookup image through histogram	*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtimage_t *input,						/* input image 						*/
  dthist_t *hist,						/* hist structure containing lut 	*/
  dtimage_t *output)					/* output image 					*/
{
  DT_F("dtimage_lookup_slow");
  dtxy_t xe = DT_MIN(input->xe, output->xe);
  dtxy_t ye = DT_MIN(input->ye, output->ye);
  dtxy_t x;
  dtxy_t y;
  dthist_bin_t bin;
#define PROMOTED_NTYPE_E DTNTYPE_F2
#define PROMOTED_NTYPE_T dtntype_f2_t
  PROMOTED_NTYPE_T *promoted = NULL;
  void *p;
  dt_rc_e rc;

  dt_dbg(ctl, F, DT_DBG_MASK_IMG,
    "input->ntype=%d "
    "hist->ntype=%d "
    "output->ntype=%d",
    input->ntype, hist->ntype, output->ntype);

  DT_C(dtos_malloc2,(ctl,				/* space for promoted row 			*/
    (void **)&promoted,
    sizeof(*promoted) * xe,
    F, "promoted row"));

  for (y=0; y<ye; y++)					/* for all rows						*/
  {
	DTIMAGE_GETROW2(DT_C, ctl,			/* access input row 				*/
      input, y, &p);
	DT_C_1D(dt1d_cast,(					/* promote input row 				*/
      p, xe, input->ntype,
      promoted, xe, PROMOTED_NTYPE_E));
	DTIMAGE_UNMAPROW2(DT_C, ctl,		/* release input row 				*/
      input, y);

#define LOOP(NTYPE, TYPE) 				\
    if (hist->ntype == NTYPE)			\
	{									\
	  TYPE *freq = (TYPE *)hist->freq;	\
	  for (x=0; x<xe; x++)				\
	  {									\
        DTHIST_VALUE_TO_BIN(DT_C, ctl,	/* get bin from input value			*/\
          hist, promoted[x], &bin);		\
		promoted[x] = freq[bin];		/* get value of bin 				*/\
      }									\
	}									\
    else
	  
	LOOP(DTNTYPE_B08, dtntype_b08_t)
	LOOP(DTNTYPE_B16, dtntype_b08_t)
	LOOP(DTNTYPE_B32, dtntype_b08_t)
	LOOP(DTNTYPE_S1, dtntype_s1_t)
	LOOP(DTNTYPE_S2, dtntype_s2_t)
	LOOP(DTNTYPE_S4, dtntype_s4_t)
	LOOP(DTNTYPE_F1, dtntype_f1_t)
	LOOP(DTNTYPE_F2, dtntype_f2_t)
    {
	  char s[256];
	  dtntype_to_string(ctl, hist->ntype,
        s, sizeof(s));
  	  rc = dt_err(ctl, F,
	    "ntype \"%s\" not supported", s);
	  goto cleanup;
    }

	DTIMAGE_MAPROW2(DT_C, ctl,			/* access output row 				*/
      output, y, &p);
	DT_C_1D(dt1d_cast,(					/* un-promote result to output 		*/
      promoted, xe, PROMOTED_NTYPE_E,
      p, xe, output->ntype));
	DTIMAGE_PUTROW2(DT_C, ctl,			/* release output row 				*/
      output, y);
  }
cleanup:
  if (promoted != NULL)
	DT_I(dtos_free2,(ctl, promoted,
      F, "promoted row"));
	
  return DT_RC_GOOD;
}

/*..........................................................................
 * do it fast if we can
 * 98/06/09 modified by FF
 * - Added case of S2 type inputs.
 *..........................................................................*/
dt_rc_e 
DTCONFIG_API1
dtimage_lookup_hist_fast(				/* lookup image through histogram	*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtimage_t *input,						/* input image 						*/
  dthist_t *hist,						/* hist structure containing lut 	*/
  int *cando,							/* return if we could do it			*/
  dtimage_t *output)					/* output image 					*/
{
  DT_F("dtimage_lookup_hist_fast");
  dt_rc_e rc;

  dt_dbg(ctl, F, DT_DBG_MASK_IMG,
    "input->ntype=%d "
    "hist->ntype=%d "
    "output->ntype=%d",
    input->ntype, hist->ntype, output->ntype);

  if (input->ntype == DTNTYPE_B08 &&	/* special case 					*/
	  hist->ntype == DTNTYPE_B08 &&
	  output->ntype == DTNTYPE_B08 &&
	  hist->minval == 0 &&				/* linearly-scaled histogram? 		*/
	  hist->maxval == hist->width)
  {
	DT_C(dtimage_lookup_b08,(ctl,
      input, 
      (const dt_ntype_b08_t *)hist->freq, 
      hist->width,
      output));
	*cando = 1;
  }
  else
  if (input->ntype == DTNTYPE_B16 &&	/* special case * added 5/13/98		*/
	  hist->ntype == DTNTYPE_B16 &&
	  output->ntype == DTNTYPE_B16 &&
	  hist->minval == 0 &&				/* linearly-scaled histogram? 		*/
	  hist->maxval == hist->width)
  {
	DT_C(dtimage_lookup_b16,(ctl,
      input, 
      (const dt_ntype_b16_t *)hist->freq, 
      hist->width,
      output));
	*cando = 1;
  }
  else
  if (input->ntype == DTNTYPE_S2 &&		/* special case						*/
	  hist->ntype == DTNTYPE_S2 &&
	  output->ntype == DTNTYPE_S2 &&
	  (hist->maxval-hist->minval) ==	/* bin size == 1 ?					*/
	    hist->width)
  {
	DT_C(dtimage_lookup_s2,(ctl,
      input, 
      (const dt_ntype_s2_t *)hist->freq, 
      hist->minval,
      hist->width,
      output));
	*cando = 1;
  }
  else
	*cando = 0;
  
cleanup:
	
  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/
dt_rc_e 
DTCONFIG_API1
dtimage_lookup_hist(					/* lookup image through histogram	*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtimage_t *input,						/* input image 						*/
  dthist_t *hist,						/* hist structure containing lut 	*/
  dtimage_t *output)					/* output image 					*/
{
  DT_F("dtimage_lookup_hist");
  int cando;

  DT_Q(dtimage_lookup_hist_fast,(ctl,
    input, hist, &cando, output));

  if (!cando)
  DT_Q(dtimage_lookup_hist_slow,(ctl,
    input, hist, output));
	
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
