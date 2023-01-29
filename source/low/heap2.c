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
DT_RCSID("source/low $RCSfile: heap2.c,v $ $Revision: 1.2 $");
#include <dtack/os.h>

#include <dtack/heap.h>

typedef struct {
  long current;
  long highwater;
  long mallocs;
  long frees;
  long malloced;
  long freed;
} dtheap_entry_t;

typedef struct {
  dtheap_entry_t total;
  dtheap_entry_t tiny;
  dtheap_entry_t small;
  dtheap_entry_t medium;
  dtheap_entry_t large;
  dtheap_entry_t giant;
  dtos_heap_table_t *table;
  int tablesize;
  long allocs_limit;
  long bytes_limit;
} dtheap_t;

#define DTHEAP_TABLESIZE_DEFAULT (500)

/*..........................................................................
 *..........................................................................*/

dt_rc_e 
DTCONFIG_API1
dtheap_check(							/* check heap integrity 			*/
  dt_ctl_t *ctl,						/* environment control 				*/
  void *vheap,							/* heap structure 					*/
  const char *caller)					/* caller ID 						*/
{
  const char *F = caller;
  DT_Q(dtos_heap_integrity,(ctl,
    caller));
  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/

dt_rc_e 
DTCONFIG_API1
dtheap_set_limit(						/* set artificial malloc limit	    */
  dt_ctl_t *ctl,
  void *vheap,
  long allocs_more,
  long bytes_more)
{
  DT_F("dtheap_limit");
  dtheap_t *heap = (dtheap_t *)vheap;

  if (heap == NULL)						/* this has to be initted		    */
    return dt_err(ctl, F,
      "heap is NULL");

  if (allocs_more == -1)				/* shutting off limit checking?	    */
    heap->allocs_limit = 0;
  else
    heap->allocs_limit =
      heap->total.mallocs +
      allocs_more;

  if (bytes_more == -1)					/* shutting off limit checking?	    */
    heap->bytes_limit = 0;
  else
    heap->bytes_limit =
      heap->total.malloced +
      bytes_more;

  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtheap_track_alloc(						/* track a malloc just done			*/
  dt_ctl_t *ctl,
  void *vheap,
  void *ptr,
  const long size,
  const char *who,
  const char *why)
{
  DT_F("dtheap_track_alloc");
  dtheap_t *heap = (dtheap_t *)vheap;
  int i;

#define UPDATE(M) \
  { \
    heap->M.mallocs++; \
    heap->M.current += size; \
    heap->M.malloced += size; \
    heap->M.highwater = DT_MAX( \
      heap->M.highwater, \
      heap->M.current); \
  }

  UPDATE(total)

  if (heap->tablesize)
  {
    for (i=0; i<heap->tablesize; i++)
      if (!heap->table[i].ptr)
	  {
        heap->table[i].ptr = ptr;
        heap->table[i].size = size;
        break;
	  }

    if (i == heap->tablesize)
	{
	  DT_Q(dtos_heap_table_reinit,(ctl,
		&heap->table,
	    &heap->tablesize,
        DTHEAP_TABLESIZE_DEFAULT));
	  heap->table[i].ptr = ptr;
	  heap->table[i].size = size;
	}

    if (size < 10)
      UPDATE(tiny)
    else
    if (size < 100)
      UPDATE(small)
    else
    if (size < 1000)
      UPDATE(medium)
    else
    if (size < 10000)
      UPDATE(large)
    else
      UPDATE(giant)
  }

#undef UPDATE

  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtheap_track_free(						/* track a heap free operation 		*/
  dt_ctl_t *ctl,						/* environment control 				*/
  void *vheap,							/* heap structure 					*/
  void *ptr)							/* block being freed 				*/
{
  DT_F("dtheap_track_free");
  dtheap_t *heap = (dtheap_t *)vheap;
  long size;
  int i;

  if (ptr == NULL)						/* can't free this!				    */
    return dt_err(ctl, F,
      "attempt to free NULL ptr");

  for (i=0;								/* search allocation table for ptr	*/
       i<heap->tablesize; 
       i++)
    if (heap->table[i].ptr == ptr)
    {
      size = heap->table[i].size;
      heap->table[i].ptr = NULL;
      heap->table[i].size = 0;
      break;
	}

  if (i == heap->tablesize)		/* did not find ptr in table?	    */
  {
	dt_rc_e rc = dt_err(ctl, F,
      "ptr 0x%08x not in table",
      DT_PTR_TO_ULONG(ptr));
	DT_I(dt_heapmon_report,(ctl,		/* spit out report				    */
      F));
	return rc;
  }

# define UPDATE(M) \
  { \
    heap->M.frees++; \
    heap->M.current -= size; \
    heap->M.freed += size; \
  }

  UPDATE(total);

  if (heap->tablesize)
  {
    if (size < 10)
      UPDATE(tiny)
    else
    if (size < 100)
      UPDATE(small)
    else
    if (size < 1000)
      UPDATE(medium)
    else
    if (size < 10000)
      UPDATE(large)
    else
      UPDATE(giant)
  }

#undef UPDATE

  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/

dt_rc_e 
DTCONFIG_API1
dtheap_init(							/* commence heap tracking 			*/
  dt_ctl_t *ctl,						/* environment control 				*/
  void **vheap,							/* heap structure 					*/
  int count)							/* initial table entry count 		*/
{
  DT_F("dtheap_init");
  dtheap_t *heap;

  if (count == 0)
    count = DTHEAP_TABLESIZE_DEFAULT;

  DT_Q(dtos_malloc2,(ctl,				/* get space for heap structure 	*/
    vheap, sizeof(dtheap_t),
    F, "heap structure"));

  heap = (dtheap_t *)(*vheap);

  DT_MEMSET(heap, 0, sizeof(*heap));		/* clear structure				    */

  heap->tablesize = count;

  DT_Q(dtos_heap_table_init,(ctl,
    &heap->table, count));

  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtheap_uninit(							/* quit heap tracking 				*/
  dt_ctl_t *ctl,						/* environment control 				*/
  void *vheap)							/* heap structure 					*/
{
  DT_F("dtheap_uninit");
  dtheap_t *heap = (dtheap_t *)vheap;
  dt_rc_e rc = DT_RC_GOOD;
  
  if (heap != NULL)
  {
    DT_I(dtos_free2,(ctl,				/* free space for allocation table	*/
      heap->table,
      F, "heap table"));

    DT_I(dtos_free2,(ctl,				/* free space for heap structure	*/
      heap,
      F, "heap structure"));
  }

  return rc;
}

/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtheap_outstanding(						/* return outstanding heap info 	*/
  dt_ctl_t *ctl,						/* environment control 				*/
  void *vheap,							/* heap structure 					*/
  long *blocks,							/* outstanding blocks 				*/
  long *bytes)							/* outstanding bytes 				*/
{
  dtheap_t *heap = (dtheap_t *)vheap;

  *blocks =
    heap->total.mallocs-
    heap->total.frees;
  *bytes =
    heap->total.current;
 
  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtheap_report(							/* produce heap report 				*/
  dt_ctl_t *ctl,						/* environment control 				*/
  void *vheap,							/* heap structure 					*/
  const char *caller)					/* caller ID						*/
{
  DT_F("dtheap_report");
  dtheap_t *heap = (dtheap_t *)vheap;
  char tmp[80];

#define PRINT(M) \
  sprintf(tmp, \
    "%-6s %8ld %8ld    %6ld %6ld   %8ld %8ld", \
    #M, \
    heap->M.current, \
    heap->M.highwater, \
    heap->M.malloced, \
    heap->M.freed, \
    heap->M.mallocs, \
    heap->M.frees); \
  dt_dbg(ctl, caller, DT_DBG_MASK_ANY, "%s", tmp);

  if (heap->tablesize)
  {
    dt_dbg(ctl, caller, DT_DBG_MASK_ANY,
      "                         -----bytes------  ------blocks------");
    dt_dbg(ctl, caller, DT_DBG_MASK_ANY,
      "up to       now  at most allocated  freed  allocated    freed");
	
    PRINT(total);
    PRINT(tiny);
    PRINT(small);
    PRINT(medium);
    PRINT(large);
    PRINT(giant);

	DT_Q(dtos_heap_table_dbg,(ctl,		/* print our own malloc table	    */
      heap->table,
      heap->tablesize,
	  caller, "outstanding mallocs"));

#ifdef NONO
	DT_Q(dtos_heap_whowhy_dbg,(ctl,		/* print our own who/why table	    */
      heap->table,
      heap->tablesize,
	  caller, "outstanding mallocs"));
#endif
  }
  else
  {
    dt_dbg(ctl, caller, DT_DBG_MASK_ANY,
      "%ld malloc'ed in %ld mallocs,"
      " %ld frees",
      heap->total.malloced,
      heap->total.mallocs,
      heap->total.frees);
    dt_dbg(ctl, caller, DT_DBG_MASK_ANY,
      "individual malloc pointers"
      " and sizes not tracked");
  }

  dtcstack_dbg(ctl, caller,				/* print stack status			    */
    DT_DBG_MASK_ANY);

  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/

dt_rc_e 
DTCONFIG_API1
dtheap_check_limit(
  dt_ctl_t *ctl,
  void *vheap,
  long size,
  const char *caller)
{
  dtheap_t *heap = (dtheap_t *)vheap;
  dtcstack_highwater(ctl, caller);

  if (heap->allocs_limit &&
      heap->total.mallocs+1 >
      heap->allocs_limit)
  {
    dt_dbg(ctl, caller, 
      DT_DBG_MASK_PAGE|
      DT_DBG_MASK_MEMTRACE,
      "allocs_limit %ld exceeded",
      heap->allocs_limit);
    return DT_RC_BAD;
  }

  if (heap->bytes_limit &&
      heap->total.malloced+size >
      heap->bytes_limit)
  {
    dt_dbg(ctl, caller, 
      DT_DBG_MASK_PAGE|
      DT_DBG_MASK_MEMTRACE,
      "bytes_limit %ld exceeded by request for %ld",
      heap->bytes_limit,
      size);
    return DT_RC_BAD;
  }
  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/

dt_rc_e 
DTCONFIG_API1
dtheap_avail(
  dt_ctl_t *ctl,
  void *vheap,
  unsigned long chunk_size,
  unsigned long maxalloc,
  unsigned long *nchunks)
{
  DT_F("dtheap_avail");
  DT_Q(dtos_heap_fill,(ctl,
    chunk_size, maxalloc,
    nchunks));
  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/

dt_rc_e 
DTCONFIG_API1
dtheap_avail_say(
  dt_ctl_t *ctl,
  void *vheap,
  unsigned long chunk_size,
  const char *caller)
{
  DT_F("dtheap_avail_say");
  DT_Q(dtos_heap_fill_say,(ctl,
    chunk_size, caller));
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
