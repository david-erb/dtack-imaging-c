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
  19991217 DE bug not clearing partial arrays
 *..........................................................................*/



/* before including, define the following like:
   define KTYPE dt_ntype_s4_t
   define KTYPE_E DT_NTYPE_S4
   define KTYPE_LIST_T long_list_t
   define KTYPE_LIST_COMPARE long_list_compare
 */

  static dt_ntype_e *ntypes = NULL;		/* we can handle all ntypes 		*/
  dtxy_t xe, ye;
  int x, y;
  KTYPE **partial = NULL;
  KTYPE *promoted = NULL;
  KTYPE_LIST_T *list = NULL;
  int i, j, n;
  int ksize;
  dt_rc_e rc;

  rc = dtimage_check_2f(ctl,			/* tracer						    */
    input, "input",
    output, "output",
    ntypes, DTIMAGE_FLAG_PAGED, F,
    &xe, &ye);
  if (rc != DT_RC_GOOD)
    return rc;

  ksize = 0;
  for (i=0; i<kxsize * kysize; i++)		/* count non-zero kernel elements 	*/
    if (kernel[i] != (KTYPE)0)
      ksize++;

  DT_C(dtos_malloc2,(ctl,				/* space for the element list		*/
    (void **)&list,
    ksize * sizeof(*list),
    F, "kernel offsets list"));
  
  i = 0;
  j = 0;
  for (y=0; y<kysize; y++)				/* make kernel element struct list 	*/
    for (x=0; x<kxsize; x++)
	{
      if (kernel[j] != (KTYPE)0)		/* forget zero kernel elements 		*/
      {
        list[i].kvalue = kernel[j];
	    list[i].dx = kxsize - x - 1;
  	    list[i].dy = kysize - y - 1;
	    i++;
      }
      j++;
	}

  DT_C_1D(dt1d_sort_struct,(			/* sort list by kernel value 		*/
    (void *)list, sizeof(*list),
    ksize, KTYPE_LIST_COMPARE));

  if (DT_DBG(ctl, DT_DBG_MASK_OBJECT))
  for (i=0; i<ksize; i++)
    dt_dbg(ctl, F, DT_DBG_MASK_OBJECT,
      "%2d. %4ld (%2u,%2u)",  
      i, list[i].kvalue,
      list[i].dx, list[i].dy);

  DT_C(dtos_malloc2,(ctl,				/* space for the partial pointers 	*/
    (void **)&partial,
    kysize * sizeof(*partial),
    F, "partial pointers"));
  DT_MEMSET(partial, 0,					/* clear array of pointers 			*/
    kysize * sizeof(*partial));

  n = (xe+kxsize) * sizeof(KTYPE);
  for (i=0; i<kysize; i++)				/* space for the partial outputs 	*/
  {
    DT_C(dtos_malloc2,(ctl,
      (void **)&partial[i], n,
      F, "partial row"));
	DT_MEMSET(partial[i], 0, n);		/* clear the partial output			*/
  }

  DT_C(dtos_malloc2,(ctl,				/* space for the promoted row 		*/
    (void **)&promoted,
    xe * sizeof(KTYPE),
    F, "promoted row"));

  {										/* do the work 						*/
    const int kx1 = kxsize / 2;
    const int ky1 = kysize / 2;
    const unsigned int qoff =			/* x offset of first output pixel 	*/
      DT_CAST(unsigned int,
        kx1 * 
        dt_ntype_sizeof[output->ntype]);
    const unsigned int nq =				/* number of output pixels 			*/
      xe-(kxsize-1);

    for (y=0; y<ye; y++)				/* for each input row 				*/
    {
	  KTYPE * const p = partial[0];		/* this is the row that is complete */
      KTYPE klast;
	  KTYPE product = 0;
      void *in = NULL;
	  DTIMAGE_GETROW(ctl, input, y, &in);
      DT_Q_1D(dt1d_cast,(				/* promote input row 				*/
        in, xe, input->ntype, 
        promoted, xe, KTYPE_E));
	  DTIMAGE_UNMAPROW(ctl, input, y);
      for (x=0; x<xe; x++)				/* for each input column 			*/
	  {
        if (promoted[x] != 0)
		{
		  klast = 0;
		  for (i=0; i<ksize; i++)		/* for each kernel element 			*/
		  {
			const KTYPE_LIST_T *
              const listi = &list[i];
			if (listi->kvalue != klast)	/* need a new product? 				*/
			{
			  klast = listi->kvalue;
			  product = listi->kvalue *
                promoted[x];
			}
			partial[listi->dy][x+listi->dx] += product;
#ifdef NONO
dt_dbg(ctl, F, DT_DBG_MASK_ANY,
"partial[%d][%d] += %d (%d*%d) -> %d", 
listi->dy, x+listi->dx, product, 
listi->kvalue,
promoted[x],
partial[listi->dy][x+listi->dx]); 
#endif
#ifdef NONO
			partial[listi->dy][x+listi->dx] +=
			  listi->kvalue *
                promoted[x];
#endif
		  }
		}
	  }
	  if (y >= kysize-1)				/* have enough to output a row? 	*/
	  {
        char *q = NULL;
	    DTIMAGE_GETROW(ctl, output,
          y-(kysize-1-ky1), &q);
	    DT_Q_1D(dt1d_cast,(				/* convert answer to output type 	*/
          p+kxsize-1, nq, KTYPE_E,
	  	  q+qoff, nq, output->ntype));
	    DTIMAGE_PUTROW(ctl, output,
          y-(kysize-1-ky1));
	  }
	  for (i=1; i<kysize; i++)			/* bubble down the partial rows 	*/
        partial[i-1] = partial[i];
	  partial[i-1] = p;
	  DT_MEMSET(p, 0,					/* clear the partial row 			*/
        (xe + kxsize) * sizeof(*p));
      DT_YIELD_ROW(DT_C, "convolving",
        y, ye);
    }
  }

cleanup:
  if (promoted != NULL)					/* space for the promoted row		*/
    DT_I(dtos_free2,(ctl,
      promoted,
      F, "promoted row"));

  if (partial != NULL)
  for (i=0; i<kysize; i++)				/* space for the partial outputs 	*/
    if (partial[i] != NULL)
      DT_I(dtos_free2,(ctl,
        partial[i],
        F, "partial row"));

  if (partial != NULL)
    DT_I(dtos_free2,(ctl,				/* space for the partial pointers 	*/
      partial,
      F, "partial pointers"));

  if (list != NULL)
    DT_I(dtos_free2,(ctl,				/* space for the partial pointers 	*/
      list,
      F, "kernel offsets list"));





/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
  ! Copyright (C) 1998 Dtack Inc. All Rights Reserved
  ! To use this file, you must have signed a license agreement with Dtack Inc.
  ! Under no circumstances may you redistribute this file.
  ! This software is provided AS IS with no warranty expressed or implied.
  ! Dtack Inc. accepts no liability for use or misuse of this file.
  ! http://www.dtack.com  dtack@dtack.com  telephone +360.417.6700
  ! Dtack Inc, P.O. Box 3061, Port Angeles, WA USA 98362
  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */
