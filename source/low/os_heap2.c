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

#define DT1D_Q(FUNC, ARGS)				/* check rc and return				*/\
{                                                                             \
  dt1d_rc_e rc;                                                               \
  DT_SETLOC                                                                   \
  rc = FUNC ARGS;		                                                      \
  if (rc != DT1D_RC_GOOD) 		                                              \
  {                                                                           \
    dt_dbg_trace(ctl, F,                                                      \
      DT_RCSID_STRING, __LINE__,                                              \
      "%s returned rc %d", #FUNC, rc);                                        \
    return (dt_rc_e)rc;                                                       \
  }                                                                           \
}

#include <dtack/base.h>
DT_RCSID("source/low $RCSfile: os_heap2.c,v $ $Revision: 1.2 $");
#include <dtack/os.h>

/*..........................................................................
 * compare two table members by ptr
 *..........................................................................*/

static
int
ptr_compare(
  void *e1,
  void *e2,
  unsigned int size)
{
  const unsigned long k1 = DT_PTR_TO_ULONG(((dtos_heap_table_t *)e1)->ptr);
  const unsigned long k2 = DT_PTR_TO_ULONG(((dtos_heap_table_t *)e2)->ptr);
  if (k1 == k2)
    return 0;
  else
  if (k1 > k2)
    return 1;
  else
    return -1;
}

/*..........................................................................
 * We never call this from MemCheck (heap3.c) so leave it out to save
 * space, especially sort.
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtos_heap_table_dbg(					/* print out a heap table 			*/
  dt_ctl_t *ctl,
  dtos_heap_table_t *table,
  int tablesize,
  const char *f,
  const char *desc)
{
  DT_F("dtos_heap_table_dbg");
  int i, n;
  unsigned long total = 0;

  n = 0;
  if (tablesize)
  {

    DT1D_Q(dt1d_sort_struct,(			/* sort table by ptr value 			*/
      (void *)table, 
      sizeof(*table),
      tablesize, 
      ptr_compare));

    for (i=0; i<tablesize; i++)			/* scan table 						*/
	{
      if (table[i].ptr)					/* table entry occupied? 			*/
	  {
        if (n == 0)
		{
          dt_dbg(ctl, f, DT_DBG_MASK_ALWAYS,
            "%s:", desc);
          dt_dbg(ctl, f, DT_DBG_MASK_ALWAYS,
            "     start  - inclusive     size   gap after");
	    }
        dt_dbg(ctl, f, DT_DBG_MASK_ALWAYS,
          "  0x%08lx-0x%08lx %8lu %10lu",
          DT_PTR_TO_ULONG(table[i].ptr),
          DT_PTR_TO_ULONG(table[i].ptr) +
	  	  table[i].size - 1,
	  	  table[i].size,
          (i == tablesize-1?
             DTCONFIG_MEM_TOP: 
             DT_PTR_TO_ULONG(table[i+1].ptr)) - 
          (DT_PTR_TO_ULONG(table[i].ptr)+table[i].size));
		total += table[i].size;
        n++; 
  	  }
	}

    if (n == 0)
      dt_dbg(ctl, f, DT_DBG_MASK_ALWAYS,
        "no %s", desc);
	else
	  dt_dbg(ctl, f, DT_DBG_MASK_ALWAYS,
		"%s total %lu in %d chunks",
        desc, total, n);
  }

  return DT_RC_GOOD;
}

/*..........................................................................*/
#ifdef NONO
  dtos_heap_walk(ctl, 2, caller);		/* print all heap entries		    */

  dtos_heap_walk(ctl, 1, caller);		/* print used heap entries		    */

  dtos_heap_walk(ctl, 0, caller);		/* print free heap entries		    */
#endif
/*..........................................................................*/

dt_rc_e 
DTCONFIG_API1
dtos_heap_walk(							/* debug while walking heap 		*/
  dt_ctl_t *ctl,						/* environment control 				*/
  int type,								/* type of walk to take 			*/
  const char *caller)					/* caller ID 						*/
{
#if defined(DTCONFIG_DV0) || \
    defined(DTCONFIG_DW0) || \
    defined(DTCONFIG_QW0) || \
    defined(DTCONFIG_QW1)
  DT_F("dtos_heap_walk");
	
# define N (100)
  dtos_heap_table_t *table = NULL;
	
  _HEAPINFO info;
  int status;
  int want;
  const char *desc;
  int n;
  dt_rc_e rc;

  if (type == 0)
  {
    want = _FREEENTRY;
    desc = "free heap entries";
  }
  else
  if (type == 1)
  {
    want = _USEDENTRY;
    desc = "used heap entries";
  }
  else
  {
    desc = "all heap entries";
  }

  DT_C(dtos_malloc2,(ctl,				/* space for table to sort 			*/
    (void **)&table, 
    N * sizeof(*table),
    F, "table"));

  info._pentry = NULL;					/* start searching heap 			*/
  while (1)
  {
    status = _heapwalk(&info);
    if (status != _HEAPOK)
      break;
    if (type == 2 ||
        info._useflag == want)
	{
	  if (n < N)
	  {
	    table[n].ptr = (void *)info._pentry;
	    table[n].size = info._size;
	  }
      n++;
	}
  }
	
  if (status == _HEAPEND)				/* heap integrity is intact? 		*/
  {
    if (n > N)							/* our table overflowed? 			*/
      dt_dbg(ctl, caller,
        DT_DBG_MASK_ALWAYS,
        "%d is too many %s,"
        " only printing %d",
        n, desc, N);
	DT_C(dtos_heap_table_dbg,(ctl,		/* print out the table 				*/
      table, DT_MIN(N, n), 
      caller, desc));
  }
  else									/* heap has problems? 				*/
  {
	char message[32];
	if (status == _HEAPBADBEGIN)
	  strcpy(message,  "_HEAPBADBEGIN");
	else
	if (status == _HEAPBADNODE)
	  strcpy(message, "_HEAPBADNODE");
	else
	if (status == _HEAPEMPTY)
	  strcpy(message, "_HEAPEMPTY");
	else
	if (status == _HEAPBADPTR)
	  strcpy(message, "_HEAPBADPTR");
	else
	  sprintf(message, "status %d", status);
	rc = dt_err(ctl, caller, "_heapchk() status %s",
      message);
  }

cleanup:
  if (table != NULL)
    DT_I(dtos_free2,(ctl, table,
      F, "table"));
  return rc;
#else									/* os doesn't provide heap walking 	*/
  return DT_RC_GOOD;
#endif
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
