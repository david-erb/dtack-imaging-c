/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
  ! Copyright (C) 1998 Dtack Inc. All Rights Reserved
  ! To use this file, you must have signed a license agreement with Dtack Inc.
  ! Under no circumstances may you redistribute this file.
  ! This software is provided AS IS with no warranty expressed or implied.
  ! Dtack Inc. accepts no liability for use or misuse of this file.
  ! http://www.dtack.com  dtack@dtack.com  telephone +360.417.6700
  ! Dtack Inc, P.O. Box 3061, Port Angeles, WA USA 98362
  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */




/* ........................................................................ *
| NAME
| function - purpose
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
 * ........................................................................ */

#include <dtack/base.h>
DT_RCSID("source/low $RCSfile: os_heap.c,v $ $Revision: 1.2 $");
#include <dtack/os.h>

/*..........................................................................
 *..........................................................................*/

dt_rc_e 
DTCONFIG_API1
dtos_heap_integrity(					/* check heap integrity 			*/
  dt_ctl_t *ctl,						/* environment control 				*/
  const char *caller)					/* caller ID 						*/
{
# if defined(DTCONFIG_DV0) || \
     defined(DTCONFIG_DW0) || \
     defined(DTCONFIG_QW0) || \
     defined(DTCONFIG_QW1)
  {
    int status = _heapchk();
    if (status != _HEAPOK &&
	    status != _HEAPEMPTY)
    {
	  char message[32];
  	  if (status == _HEAPBADBEGIN)
	    strcpy(message,  "_HEAPBADBEGIN");
  	  else
  	  if (status == _HEAPBADNODE)
	    strcpy(message, "_HEAPBADNODE");
 	  else
	    sprintf(message, "status %d", status);
	  return dt_err(ctl, caller, 
        "_heapchk() status %s",
        message);
	}
  }
# endif
  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/

dt_rc_e 
DTCONFIG_API1
dtos_heap_fill(							/* fill heap 						*/
  dt_ctl_t *ctl,						/* environment control 				*/
  unsigned long chunk_size,				/* size of chunk to fill with		*/
  unsigned long maxalloc,				/* maximum size to fill to 			*/
  unsigned long *nchunks)				/* returned chunks to fill heap		*/
{
  DT_F("dtos_heap_fill");
  unsigned long total = 0;
  typedef struct chunk_t {
    struct chunk_t *chunk;
  } chunk_t;
  chunk_t *thes;
  chunk_t *last = NULL;
  chunk_t *first;
  unsigned long max = 0UL;
  unsigned long min = 0xffffffffUL;
  dt_rc_e rc = DT_RC_GOOD;

  chunk_size = DT_MAX(chunk_size, sizeof(unsigned long));

  *nchunks = 0;
  while (total < maxalloc &&
         rc == DT_RC_GOOD)
  {
	thes = (chunk_t *)malloc(			/* grab a chunk 					*/
      DT_CAST(unsigned int,
        chunk_size));
	if (thes == NULL)
      break;
	if (DT_PTR_TO_ULONG(thes)+chunk_size >
        DTCONFIG_MEM_TOP)
	{
	  free(thes);
	  thes = NULL;
      rc = dt_err(ctl, F,
        "malloc(%ld) gave ptr"
        " 0x%08lx-0x%08lx above mem top 0x%08lx",
        chunk_size, 
        DT_PTR_TO_ULONG(thes),
        DT_PTR_TO_ULONG(thes) + chunk_size - 1,
        DTCONFIG_MEM_TOP);
	  break;
	}

    max = DT_MAX(max, DT_PTR_TO_ULONG(thes));
    min = DT_MIN(min, DT_PTR_TO_ULONG(thes));

    DT_MEMSET(thes, 0x11,					/* clear it to make sure		    */
      DT_CAST(unsigned int, 
        chunk_size));
    thes->chunk = last;					/* chain to last chunk			    */
	last = thes;
	total += chunk_size;
    if (*nchunks == 0)
  	  first = thes;
	*nchunks = *nchunks + 1;
  }

  dt_dbg(ctl, F, DT_DBG_MASK_MEMTRACE,
    "first chunk 0x%08lx, last 0x%08lx",
    DT_PTR_TO_ULONG(first),
    DT_PTR_TO_ULONG(last));

  dt_dbg(ctl, F, DT_DBG_MASK_MEMTRACE,
    "min chunk 0x%08lx, max 0x%08lx-0x%08lx", 
    min, max, max+chunk_size-1);

  while (last != NULL)
  {
	thes = last;
	last = last->chunk;
	free(thes);
  }

  return rc;
}

/*..........................................................................
 *..........................................................................*/

dt_rc_e 
DTCONFIG_API1
dtos_heap_fill_say(						/* fill heap, say results 			*/
  dt_ctl_t *ctl,						/* environment control 				*/
  unsigned long chunk_size,				/* chunk size to fill with 			*/
  const char *F)						/* caller ID 						*/
{
  unsigned long nchunks;
  DT_Q(dtos_heap_fill,(ctl,
    chunk_size,
    1000000L, &nchunks));				/* one megabyte maximum 			*/
  dt_say(ctl, F, "able to malloc %ld %ld-byte chunks (%ld total)",
    nchunks, chunk_size,
    nchunks * chunk_size);
  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtos_heap_option(						/* set heap option					*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtos_heap_option_e option)			/* heap option desired 				*/
{
  DT_F("dtos_heap_option");
  dt_rc_e rc = DT_RC_GOOD;

  switch(option)
  {
	case DTOS_HEAP_OPTION_NO_NEARHEAP:
#   if defined(DTCONFIG_DV0) || \
       defined(DTCONFIG_DW0) || \
       defined(DTCONFIG_QW0) || \
       defined(DTCONFIG_QW1)
	{
	    size_t before = _memavl();
		if (_nmalloc(1) == NULL)
		  rc = dt_err(ctl, F,
            "errno %d in _nmalloc(1)", errno);
		else
		if (_nheapmin() != 0)
		  rc = dt_err(ctl, F,
            "errno %d in _nheapmin()", errno);
		else
          dt_dbg(ctl, F, DT_DBG_MASK_MEMSTATS,
            "before _nheapmin(), _memavl() was %u, after %u",
            before, _memavl());
	}
#   endif	
	break;
	default:
	  rc = dt_err(ctl, F,
        "invalid option %d", option);
  }

  return rc;
}

/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtos_heap_table_init(
  dt_ctl_t *ctl,
  dtos_heap_table_t **table,
  int count)
{
  DT_F("dtos_heap_table_init");
  int n;

  n = count * sizeof(**table);			/* size of allocation table	    	*/
    
  DT_Q(dtos_malloc2,(ctl,				/* space for allocation table	    */
    (void **)table,
    n, F, "heap table"));

  DT_MEMSET(*table, 0, n);					/* clear allocation table			*/

  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtos_heap_table_reinit(
  dt_ctl_t *ctl,
  dtos_heap_table_t **table,
  int *count,
  int increment)
{
  DT_F("dtos_heap_table_init");
  long n1, n2;
  void *newtable;
  void *usage = ctl->heap;

  n1 = (long)(*count) *					/* hold size of allocation table   	*/
    (long)sizeof(**table);

  n2 = ((long)(*count) +				/* new size of allocation table	   	*/
    (long)increment) *
    (long)sizeof(**table);

  ctl->heap = NULL;						/* protect against recursion 		*/
  
  DT_Q(dtos_realloc3,(ctl,				/* try to increase table size 		*/
    *table, (void **)&newtable,
	n2, F, "new heap table"));

  ctl->heap = usage;

  if (newtable != NULL)					/* increase successful? 			*/
  {
	DT_MEMSET((char *)newtable+			/* clear out the new part 			*/
      (unsigned int)n1, 0,	
      (unsigned int)(n2-n1));
    *table = (dtos_heap_table_t *)newtable;
	*count = *count + increment;
  }
  else									/* increase failed? 				*/
  {
	return dt_err(ctl, F,
      "could not increase"
      " malloc table size from %d to %d",
      *count, *count+increment);
  }

  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/

#ifdef NONO

dt_rc_e
DTCONFIG_API1
dtos_heap_whowhy_dbg(					/* print out who/why heap table		*/
  dt_ctl_t *ctl,
  dtos_heap_table_t *table,
  int tablesize,
  const char *f,
  const char *desc)
{
  if (tablesize)
  {
	int n;
	int i;

	n = 0;
    for (i=0; i<tablesize; i++)			/* scan table 						*/
	{
      if (table[i].ptr &&
		  (table[i].who ||
		   table[i].why))
	  {
        if (n == 0)						/* outputting first line? 			*/
		{
          dt_dbg(ctl, f, DT_DBG_MASK_ALWAYS,
            "%s:", desc);
          dt_dbg(ctl, f, DT_DBG_MASK_ALWAYS,
            "     start     size file        @line: why");
	    }
        dt_dbg(ctl, f, DT_DBG_MASK_ALWAYS,
          "  0x%08lx %6ld %s%s%s",
          DT_PTR_TO_ULONG(table[i].ptr),
	  	  table[i].size,
		  table[i].who? table[i].who: "who unknown",
		  table[i].why? ": ":         "",
		  table[i].why? table[i].why: "");
		n++;
  	  }
	}
  }

  return DT_RC_GOOD;
}

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
