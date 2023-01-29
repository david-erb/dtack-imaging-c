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
  19991217 DE changed some long's to 4-byte int's in favor of 64-bit CPU
 *..........................................................................*/


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
DT_RCSID("source/low $RCSfile: runf.c,v $ $Revision: 1.9 $");
#include <dtack/runf.h>
#include <dtack/os.h>

#define I_RIGHT 0x1
#define J_RIGHT 0x2
#define I_COVER 0x4
#define J_COVER 0x8
#define COVER (J_COVER|I_COVER)
#define BEYOND (J_RIGHT|J_COVER)

/*..........................................................................*/

dt_rc_e 
DTCONFIG_API1
dtrunf_alloc_struct(				/* allocate run structure		    */
  dt_ctl_t *ctl,
  dtimage_t *image,
  dtrunf_t *run)
{
  DT_F("dtrunf_alloc_struct");
  unsigned int n;

  n = image->ye * sizeof(*run->row);
  DT_Q(dtos_malloc2,(ctl,
    (void **)&run->row, n,
    F, "row pointers"));
  DT_MEMSET(run->row, 0, n);

  n = image->ye * sizeof(*run->n);
  DT_Q(dtos_malloc2,(ctl,
    (void **)&run->n, n,
    F, "row run counts"));
  DT_MEMSET(run->n, 0, n);

  return DT_RC_GOOD;
}

/*..........................................................................*/

dt_rc_e 
DTCONFIG_API1
dtrunf_free_struct(				/* free run structure			    */
  dt_ctl_t *ctl,
  dtimage_t *image,
  dtrunf_t *run)
{
  DT_F("dtrunf_free_struct");
  dt_rc_e rc = DT_RC_GOOD;

  if (run->n != NULL)
  DT_I(dtos_free2,(ctl,
    run->n,
    F, "row run counts"));

  if (run->row != NULL)
  DT_I(dtos_free2,(ctl,
    run->row,
    F, "row pointers"));

  return rc;
}

/*..........................................................................*/

dt_rc_e 
DTCONFIG_API1
dtrunf_free_runs(					/* free run arrays				    */
  dt_ctl_t *ctl,
  dtimage_t *image,
  dtrunf_t *run)
{
  DT_F("dtrunf_free_runs");
  dtxy_t y;

  for (y=0; y<image->ye; y++)
    if (y == 0 || run->row[y] != run->row[y-1]+run->n[y-1])
	  if (run->row[y] != NULL)
        DT_Q(dtos_free2,(ctl, run->row[y], F, "run segment"));

  return DT_RC_GOOD;
}

/*..........................................................................*/

dt_rc_e 
DTCONFIG_API1
dtrunf_free_all(					/* free run structure and arrays    */
  dt_ctl_t *ctl,
  dtimage_t *image,
  dtrunf_t *run)
{
  DT_F("dtrunf_free_all");

  DT_Q(dtrunf_free_runs,(ctl, image, run));
  DT_Q(dtrunf_free_struct,(ctl, image, run));

  return DT_RC_GOOD;
}


/*..........................................................................*/

#define S1 char
#define S2 short
#define S4 dtntype_s4_t
#define S8 double

#define CMP1(P, X, V) \
  ((size == 1 && ((S1 *)(P))[(X)] != V ## 1) || \
   (size == 2 && ((S2 *)(P))[(X)] != V ## 2) || \
   (size == 4 && ((S4 *)(P))[(X)] != V ## 4) || \
   (size == 8 && ((S8 *)(P))[(X)] != V ## 8))

#define CMP2(P, X, Q, U) \
  ((size == 1 && ((S1 *)(P))[(X)] != ((S1 *)(Q))[(U)]) || \
   (size == 2 && ((S2 *)(P))[(X)] != ((S2 *)(Q))[(U)]) || \
   (size == 4 && ((S4 *)(P))[(X)] != ((S4 *)(Q))[(U)]) || \
   (size == 8 && ((S8 *)(P))[(X)] != ((S8 *)(Q))[(U)]))

#define ADD \
          r[k].x = x0; \
          r[k].y = y; \
          r[k].l = x - x0 - 1; \
          n++;							/* up run count for this row	    */\
          k++;

/*..........................................................................*/
dt_rc_e
DTCONFIG_API1
dtrunf_scan(						/* scan image for all runs			*/
  dt_ctl_t *ctl,
  dtimage_t *image,
  dtrunf_t *run,
  const double invisible,
  const int flag)
{
  DT_F("dtrunf_scan");
  dtxy_t x0, x, y;
  unsigned int n;
  dtrunf_elem_t *runs;
  unsigned int k;
  long kmax;
  const int size = DT_CAST(int, 		/* size of pixel 					*/
    dt_ntype_sizeof[image->ntype]);
  dt_ntype_f2_t value;					/* big enough for largest pixel 	*/
  dt_rc_e rc;

  rc = dtimage_check_1f(ctl,			/* check simple stuff				*/
    image, "image",
    dtrun_ntype_list, 0, F);
  if (rc != DT_RC_GOOD)
    return rc;

  DTNTYPE_SET1(DT_Q, ctl,				/* re-cast invisible value 			*/
    &value, 0, image->ntype, 
    invisible);
  
  {
    const S1 value1 =					/* background pixel value			*/
      size == 1? *((S1 *)&value): 0;
    const S2 value2 = 
      size == 2? *((S2 *)&value): 0;
    const S4 value4 = 
      size == 4? *((S4 *)&value): 0;
    const S8 value8 = 
      size == 8? *((S8 *)&value): 0;

  run->runs = 0;						/* clear run count (fyi only)	    */
  run->head = NULL;
  kmax = DTIMAGE_SEG_MAXSIZE /			/* runs in each segment				*/
           sizeof(*runs);
  run->segsize = kmax * sizeof(*runs);	/* size of each segment			    */
  if (run->segsize > 8192)
    run->segsize = 8192;
  kmax = run->segsize / sizeof(*runs);	/* revised max runs per segment	    */
  DT_Q(dtos_malloc2,(ctl,				/* get space for first run segment	*/
    (void **)&runs,
    run->segsize,
    F, "run segment"));
  run->head = runs;
  run->segments = 1;					/* clear segment count (fyi only)	*/
  k = 0;
  for (y=0; y<image->ye; y++)			/* for all rows in image		    */
  {
    const dt_ntype_b08_t * const p =
      image->row[y].b08;
    dtrunf_elem_t * const r =			/* short for segment pointer	    */
      runs;
    run->row[y] = &r[k];
    n = 0;								/* clear run count for row		    */
    x0 = 0;
	
										/* ................................ */
	if (flag < 0)
	{
      for (x=1; x<image->xe; x++)
	  {
		if (CMP2(p, x, p, x0))			/* run starts here?				    */
        {
		  if (CMP1(p, x0, value))		/* keep the run?				    */
	  	  {
            if ((long)k == kmax)		/* segment filled up?			    */
              break;
		    ADD;
		  }
          x0 = x;
		}
	  }
	}
										/* ................................ */
	else
	if (flag > 0)
	{
      for (x=1; x<image->xe; x++)
	  {
		if (CMP2(p, x, p, x0))			/* run starts here?				    */
        {
		  if (!CMP1(p, x0, value))		/* keep the run?				    */
	  	  {
            if ((long)k == kmax)		/* segment filled up?			    */
              break;
		    ADD;
		  }
          x0 = x;
		}
	  }
	}
										/* ................................ */
	else
	{
      for (x=1; x<image->xe; x++)
	  {
		if (CMP2(p, x, p, x0))			/* run starts here?				    */
		{								/* keep all runs 					*/
          if ((long)k == kmax)			/* segment filled up?			    */
            break;
		  ADD;
          x0 = x;
		}
	  }
	}
										/* ................................ */

    if ((long)k == kmax)				/* filled up the memory block?		*/
	{
	  DT_Q(dtos_malloc2,(ctl,			/* get space for run segment	    */
        (void **)&runs,
        run->segsize,
        F, "run segment"));
      run->segments++;					/* how many segments used (fyi)	    */
      y--;								/* rescan the row				    */
      k = 0;
	}
    else								/* finished row					    */
	{
      if (flag == 0 ||					/* keep the last run?			    */
          (flag < 0 && 
           CMP1(p, x0, value)) ||
          (flag > 0 && 
           !CMP1(p, x0, value)))
	  {
		ADD;
	  }
      run->n[y] = n;					/* runs on row					  	*/
      run->runs += n;					/* total runs in image (fyi)	    */
    }
  }
  }

  dt_dbg(ctl, F, DT_DBG_MASK_COMPLEX,
    "%ld runs took %ld segments (%ldK)",
    run->runs, run->segments,
    (run->segments * run->segsize) / 1000L);

  return DT_RC_GOOD;
}

/*..........................................................................*/

static
dt_rc_e
dtrunf_reassign(						/* reassign after joins				*/
  dt_ctl_t *ctl,
  dtimage_t *image,
  dtrunf_t *run)
{
  dtrunf_elem_t *head, *p, *q, *r, *h;
  unsigned long blobs = 0;
  unsigned long reassigns = 0;

  p = head = run->head;
  r = NULL;
  while (p != NULL)						/* for each run					    */
  {
    dtrunf_elem_t * const
	  s = p;
    dtrunf_elem_t * const
      sh = s->h;
    if (s == s->h)						/* this is a true header?		    */
	{
      blobs++;
	  if (head == NULL)					/* first true header encountered?   */
	    head = s;
      do {								/* find end of chain			    */
		r = p;							/* chain gets put in main chain	    */
		p = p->p;
	  } while (p != NULL &&
			   p->h == sh);
	}
	else
	{
      dtrunf_elem_t * const
	    shh = sh->h;
      for (h=shh; h!=h->h; h=h->h);		/* find true header				    */
  	  do {								/* find end of the chain		    */
	    p->h = h;						/* update to use true header   	    */
	    q = p;							/* remember last run on chain	    */
	    p = p->p;						/* next run on chain			    */
	  } while (p != NULL &&				/* until we hit end of chain	    */
			   p->h->h == shh &&		/* or until we hit another head	 	*/
			   p != p->h);				/* we might hit a true head here    */
	  q->p = h->p;						/* link chain into true chain	    */
	  h->p = s;
	  if (head == s)					/* we just subordinated the head?   */
	    head = NULL;
	  if (r != NULL)					/* main chain has something?	    */
	    r->p = p;						/* unlink chain from main chain	    */
	  reassigns++;						/* count reassigns (fyi only)	    */
	}
  }

  run->head = head;						/* head may have been reassigned	*/
  run->blobs = blobs;
  run->reassigns = reassigns;

  return DT_RC_GOOD;;
}

#ifdef NONO
static long number(dtrunf_t *run, dtrunf_elem_t *p)
{
  if (p == NULL)
    return -1;
  else
    return p - run->row[0];
}

dt_dbg(ctl, F, DT_DBG_MASK_OBJECT,
"head changed from %ld to %ld",
-1, number(run, s));

dt_dbg(ctl, F, DT_DBG_MASK_OBJECT,
"unlink %3ld (%3ld %3ld) %3ld, link between %3ld %3ld",
number(run, r), number(run, s), number(run, q),
number(run, p), number(run, h), number(run, h->p));

dt_dbg(ctl, F, DT_DBG_MASK_OBJECT,
"head changed from %ld to %ld",
number(run, head), -1);
#endif

/*..........................................................................*/
dt_rc_e 
DTCONFIG_API1
dtrunf_label(							/* label all runs					*/
  dt_ctl_t *ctl,
  dtimage_t *image,
  unsigned long flags,
  dtrunf_t *run)
{
  DT_F("dtrunf_label");
  dtrunf_elem_t *head;
  dtrunf_elem_t *ih, *jh;
  dt_ntype_f2_t *promoted = NULL;
  unsigned int joins = 0;
  const dtxy_t xe = image->xe;
  dtxy_t y;
  unsigned int i;
# define FMAX (256)
  unsigned int f[FMAX];
  unsigned int zero = 0;
  dt_rc_e rc;

  rc = dtimage_check_1f(ctl,			/* check simple stuff				*/
    image, "image",
    dtrun_ntype_list, 0, F);
  if (rc != DT_RC_GOOD)
    return rc;

  if (run->segments == 0)				/* no runs at all?				    */
  {
    run->blobs = 0;						/* then of course no blobs		    */
    return DT_RC_GOOD;
  }

  DT_C(dtos_malloc2,(ctl,				/* space for the promoted row 		*/
    (void **)&promoted,
    2 * xe * sizeof(*promoted),
    F, "promoted row"));

  head = NULL;
  for (i=0; i<run->n[0]; i++)			/* label first row of runs		    */
  {
    dtrunf_elem_t * const p =
      &run->row[0][i];
    if (i == 0)
      head = p;
    else
      run->row[0][i-1].p = p;			/* chain blobs					    */
	p->h = p;							/* each run is its own header	    */
  }
  if (i > 0)
    run->row[0][i-1].p = NULL;			/* tail of chain				    */

  dt1d_cast(							/* promote first row 				*/
    image->row[0].dvoid, xe, 
    image->ntype,
	promoted, xe, DT_NTYPE_F2);

  for (y=1; y<image->ye; y++)			/* for all rows of runs		    	*/
  {
    dtrunf_elem_t * const				/* this row's runs				    */
	  iruns = run->row[y];
    const unsigned int ni = run->n[y];
    dtrunf_elem_t * const				/* last row's runs				    */
	  jruns = run->row[y-1];
    const unsigned int nj = run->n[y-1];
    const dt_ntype_f2_t * const			/* this row's pixel values		    */
	  ivalue = promoted + (y&1) * xe;
    const dt_ntype_f2_t * const			/* last row's pixel values		    */
	  jvalue = promoted + !(y&1) * xe;

	dt1d_cast(							/* promote this row 				*/
      image->row[y].dvoid, xe, 
      image->ntype,
	  (dt_ntype_f2_t *)ivalue, xe, 
      DT_NTYPE_F2);

    DT_MEMSET(f, 0, sizeof(f));			/* first run on row above to check	*/
    for (i=0; i<ni; i++)				/* for all runs on this row		    */
    {
	  dtrunf_elem_t * const
        irun = &iruns[i];
      const dtxy_t i0 = irun->x;
      const dtxy_t il = irun->l;
      const dt_ntype_f2_t				/* pixel value of this run		    */
	    iv = ivalue[i0];
      unsigned int *fiv = 
        (int)iv == iv &&
	    iv >= 0 && iv < FMAX?
        &f[(unsigned int)iv]: &zero;
      unsigned int j;

      irun->h = NULL;					/* initially no label on run	    */

      for (j=(*fiv); j<nj; j++)			/* for all runs on previous row	    */
      {
  	    dtrunf_elem_t * const
          jrun = &jruns[j];
		const dtxy_t j0 = jrun->x;

		if (iv == jvalue[j0])			/* only consider if same value	    */
		{
		  const dtxy_t jl = jrun->l;
		  const int d = i0 - j0;		/* distance between starts			*/
		  int rel;

		  if (d > 0)					/* i is to right of j?				*/
		  {	
			if (d > jl)					/* i start past j end?				*/
              rel = I_RIGHT;
			else
			if (d+il < jl)				/* i completely covered by j?		*/
               rel = J_COVER;
			else						/* end of i extends past end of j	*/
		      rel = I_COVER;
		  }
		  else							/* i is to left of j?				*/
		  {
			const dtxy_t md = -d;
			if (md > il)				/* j start past i end?				*/
              rel = J_RIGHT;
			else
			if (md+jl < il)				/* j completely covered by i?		*/
  		      rel = I_COVER;
			else						/* end of j extends past end of i	*/
		      rel = J_COVER;
		  }

		  if (rel & COVER)				/* two runs overlap?				*/
		  {
			if (irun->h == NULL)		/* first time run in any run?		*/
			{
              irun->h = jrun->h;		/* assign label for this run		*/
			  irun->p = jrun->p;		/* link run into chain			    */
			  jrun->p = irun;
			}
			else				
			if (irun->h->h !=			/* this run in some other label?	*/
				jrun->h->h)
			{
              for (jh=jrun->h->h;		/* find true header of j		    */
                   jh!=jh->h;
                   jh=jh->h);
              for (ih=irun->h->h;		/* find true header of i		    */
                   ih!=ih->h;
                   ih=ih->h);
			  joins++;					/* count joins (fyi only)		    */
              jrun->h->h =				/* save search time later		    */
              irun->h->h =
              jh->h = ih;				/* join labels					    */
			}
		  }
		  if (rel & BEYOND)				/* j now beyond i					*/
            break;						/* quit looking for j's on this i	*/
		}
      }
      if (irun->h == NULL)				/* run never assigned to label?		*/
	  {
		irun->p = head;					/* link at head of chain		    */
        head = irun->h = irun;			/* it is its own header			    */
	  }
      if (fiv != &zero)
        *fiv = j;
    }
  }

  run->head = head;						/* what we know so far			 	*/
  run->joins = joins;

  DT_C(dtrunf_reassign,(ctl,			/* reassign after joins			    */
    image, run));

  dt_dbg(ctl, F, DT_DBG_MASK_COMPLEX,
    "%ld blobs took %ld joins and %ld reassigns",
    run->blobs, run->joins, run->reassigns);
  
cleanup:
  if (promoted != NULL)					/* space for the promoted row		*/
    DT_I(dtos_free2,(ctl,
      promoted,
      F, "promoted row"));

  return rc;
}

#ifdef NONO
dt_dbg(ctl, "", DT_DBG_MASK_PIXEL,
"joining jrun %3d (h=%3d, hh=%3d) to irun %3d (h=%3d, hh=%3d) th=%3d",
jrun-&run->row[0][0], jrun->h-&run->row[0][0], jrun->h->h-&run->row[0][0], 
irun-&run->row[0][0], irun->h-&run->row[0][0], irun->h->h-&run->row[0][0],
h-&run->row[0][0]);

int n = p - &run->row[0][0];
run->head = head;
if (p == head)
dttutil_offset_labels(ctl, image, run, run->runs, NULL, F, "");

dt_dbg(ctl, F, DT_DBG_MASK_PIXEL,
"true head %d left in place", n);

dt_dbg(ctl, F, DT_DBG_MASK_PIXEL, 
"head %d put in place after true %d",n, h-&run->row[0][0]);

#endif





/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
  ! Copyright (C) 1998 Dtack Inc. All Rights Reserved
  ! To use this file, you must have signed a license agreement with Dtack Inc.
  ! Under no circumstances may you redistribute this file.
  ! This software is provided AS IS with no warranty expressed or implied.
  ! Dtack Inc. accepts no liability for use or misuse of this file.
  ! http://www.dtack.com  dtack@dtack.com  telephone +360.417.6700
  ! Dtack Inc, P.O. Box 3061, Port Angeles, WA USA 98362
  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */
