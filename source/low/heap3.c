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
DT_RCSID("source/low $RCSfile: heap3.c,v $ $Revision: 1.2 $");
#include <dtack/os.h>

extern
dt_rc_e
DTCONFIG_API1
dtos_heap_whowhy_dbg(					/* print out who/why heap table		*/
  dt_ctl_t *ctl,
  dtos_heap_table_t *table,
  int tablesize,
  const char *f,
  const char *desc);

#include <dtack/heap.h>
#include <dtack/str.h>

typedef struct {
  dt_ctl_t *ctl;
  char message[256];					/* first message received 			*/
  ERF old_erf;							/* old erf to restore at end 		*/
  long blocks_at_start;					/* blocks allocated at our start 	*/
  long bytes_at_start;
  long allocs_limit;					/* artificial block limit 			*/
  long bytes_limit;
} dtheap_t;

static dtheap_t *global_heap = NULL;

/*..........................................................................*/
#if DTPRJ_STRATOS_MEMCHECK >= 99		/* complete control of MemCheck?	*/

#if !defined (_MC_VERSION)
#error  Must use MemCheck V3.0 header!
#endif

/* These are the default settings used by MemCheck */

MCSETTINGS  _MCVAR  MC_DefaultSettings = { 

    /*
        "Flags" structure member, bit by bit:

        Bit Flag                What
        --- ---------------     -----------------------------------
        0   MCF_ACTIVE          MemCheck active?
        1   MCF_FAST_MODE       Fast mode?
        2   MCF_PROTECTED_MODE  Protected mode?
        3   MCF_FAR_NULL_CHECK  Check for far NULL ptr assigns *
        4   MCF_NEAR_NULL_CHECK Check for far NULL ptr assigns *
        5   MCF_STANDARD_STACK  Standard stack frame *
        6   MCF_AUTOINIT        Start up automatically *
        7   MCF_CLEAR_ON_FREE   Clear buffers when freed 
        8   MCF_DISK_ROCKET     Use DiskRocket options
        9   MCF_IDX_IN_MEMORY   Use memory only for Rocket indexes *
                                (only if DiskRocket linked)
        10  MCF_SOURCE_ONLY     Intercept in source code only 

        * = default option (see below)
    */
    /* "Flags" element: list only defaults here! */
    MCF_FAR_NULL_CHECK  |
    MCF_NEAR_NULL_CHECK |
    MCF_STANDARD_STACK  |
/*  MCF_AUTOINIT        |  */
    MCF_IDX_IN_MEMORY       /* default when DiskRocket linked */
    ,   /* end of "Flags" field */

    MAX_MEMORY,             /* max mem = 1000K */

    /* If MCE_FAR/NEAR_NULL_CHECK bitflags set ... */
    D_NULLCHECK_BYTES_NEAR, /* near null bytes */
    D_NULLCHECK_BYTES_FAR,  /* far  null bytes */

    D_CheckByteCt,          /* check bytes */
    D_AlignSize,            /* align size */

    ""                      /* default dir to current */

};  /* END of default MemCheck Settings struct */

#endif
/*..........................................................................*/
#if DTPRJ_STRATOS_MEMCHECK >= 4			/* complete control of MemCheck?	*/

void
DTCONFIG_API1
_MCCALLBACK
mc_get_settings(						/* settings hook, page 5-37 		*/
  MCSETTINGS *settings)
{
#ifdef NONO
    printf("mc_get_settings:"
      " MCF_ACTIVE=%s,"
      " MCF_AUTOINIT=%s\n",
      settings->Flags & MCF_ACTIVE?
        "yes": "no",
      settings->Flags & MCF_AUTOINIT?
        "yes": "no");
#endif

  settings->Flags |= MCF_ACTIVE |		/* always activate					*/
	MCF_CLEAR_ON_FREE;
  
  settings->Flags &= ~MCF_AUTOINIT;		/* never autoinit					*/
}

#endif

/*..........................................................................*/
static
void
_MCCALLBACK
neuter_erf(
  char *message)
{
}

/*..........................................................................*/
static
void
_MCCALLBACK
dtheap_erf(
  char *message)
{
  DT_F("dtheap_erf");
  if (global_heap != NULL &&
	  !global_heap->message[0])			/* this would be the first error? 	*/
  {
    strncpy(global_heap->message,		/* keep a copy of the message 		*/
      message,
      sizeof(global_heap->message)-1);
    global_heap->message[sizeof(
      global_heap->message)-1] = '\0';
  }
  dt_err(global_heap->ctl, F,			/* log a dtack error 				*/
    "%s", message);
}

/*..........................................................................*/

static
void
dbg_status(								/* debug MemCheck status			*/
  dt_ctl_t *ctl,
  const char *F)
{
#if DTPRJ_STRATOS_MEMCHECK
  dt_dbg(ctl, F, DT_DBG_MASK_MEMSTATS,
    "is_active %d,"
    " A/F/C blocks %lu/%lu/%lu,"
    " bytes %lu/%lu/%lu",
    mc_is_active(),
    mc_blocks_allocated(),
    mc_blocks_freed(),
    mc_blocks_allocated() -
    mc_blocks_freed(),
    mc_bytes_allocated(),
    mc_bytes_freed(),
    mc_bytes_allocated() -
    mc_bytes_freed());
#endif
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

  DT_Q(dtos_malloc2,(ctl,				/* get space for heap structure 	*/
    vheap, sizeof(dtheap_t),
    F, "heap structure"));

  heap = (dtheap_t *)(*vheap);

  DT_MEMSET(heap, 0, sizeof(*heap));		/* clear structure				    */

# if DTPRJ_STRATOS_MEMCHECK >= 4		/* complete control of MemCheck?	*/
  {
	dbg_status(ctl, F);

	if (mc_blocks_allocated())			/* some blocks allocated already? 	*/
	  return dt_err(ctl, F,
        "%lu blocks already allocated",
        mc_blocks_allocated());

    mc_startcheck(neuter_erf);			/* start it silent					*/
  }
# endif

# if DTPRJ_STRATOS_MEMCHECK >= 3		/* use MemCheck for error checking?	*/
  {
	dbg_status(ctl, F);

    heap->old_erf =						/* remember old error reporting 	*/
      mc_set_erf(dtheap_erf);			/* change current error reporting	*/
	heap->message[0] = '\0';			/* clear existing message 			*/
  }
# endif

  heap->ctl = ctl;
  heap->blocks_at_start =				/* remember what's going on now 	*/
    mc_blocks_allocated();
  heap->bytes_at_start =
    mc_bytes_allocated();

  global_heap = heap;					/* this is what callback uses 		*/

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
  
  if (heap == NULL)						/* heap monitoring not active? 		*/
    return DT_RC_GOOD;

# if DTPRJ_STRATOS_MEMCHECK >= 4		/* complete control of MemCheck?	*/
  {
    mc_set_erf(neuter_erf);				/* ignore leak messages				*/
    mc_endcheck();						/* kill it							*/
  }
# endif

# if DTPRJ_STRATOS_MEMCHECK >= 3		/* use MemCheck for error checking?	*/
  {
    if (mc_is_active())					/* is it still going? 				*/
  	  (void)mc_set_erf(
        heap->old_erf);					/* replace original error handling	*/
  }
# endif
  
  DT_I(dtos_free2,(ctl,					/* free space for heap structure	*/
    heap,
    F, "heap structure"));

  global_heap = NULL;

  return rc;
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
      mc_blocks_allocated() +
      allocs_more;

  if (bytes_more == -1)					/* shutting off limit checking?	    */
    heap->bytes_limit = 0;
  else
    heap->bytes_limit =
      mc_bytes_allocated() +
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
  return DT_RC_GOOD;
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
  *blocks =
    mc_blocks_allocated() -
    mc_blocks_freed();
  *bytes =
    mc_bytes_allocated() -
    mc_bytes_freed();
 
  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/

#ifdef NONO
#define N 10
#define M 64

static dtos_heap_table_t *global_table = NULL;
static char *global_who = NULL;
static int global_n = 0;
static
void
_MCCALLBACK
report(									/* produce heap report 				*/
  MEMRECP mem)
{
  if (mem->flags == REPORT_START ||
	  mem->flags == REPORT_END)
    return;
  if (global_n < N)
  {
	int n = global_n;					/* for short 						*/
	global_table[n].ptr = mem->ptr;
	global_table[n].size = mem->size;
	global_table[n].who =
      global_who + n * M;
	if (mem->ptr != global_table)
      global_table[n].why = NULL;
	else
      global_table[n].why = "this table!";
    dtstr_printf(NULL, 
      (char *)global_table[n].who,
      M, "%12s@%-4u%s",
      mem->file, mem->line,
      mem->flags & MRFLAG_EXACT_LOC?	/* this is the exact file and line? */
        "": "?");
  }
  global_n++;
  return;
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
  void *p;
  unsigned int n;
  dt_rc_e rc;

  n =
    N * sizeof(dtos_heap_table_t) +
    N * M;
  
  DT_Q(dtos_malloc2,(ctl,				/* space for heap table 			*/
    &p, n,
    F, "heap table"));

  global_table = (dtos_heap_table_t *)p;
  global_who = (char *)&global_table[N];

  global_n = 0;

  mc_report(report);					/* build table 						*/

  DT_G(dtos_heap_whowhy_dbg,(ctl,		/* print our own who/why table	    */
    global_table,
    DT_MIN(N, global_n),
	caller, "outstanding mallocs"));

  DT_I(dtos_free2,(ctl, p,				/* free table 						*/
    F, "heap table"));

  global_table = NULL;	
  global_who = NULL;
  global_n = 0;
  
  return rc;
}
#endif


/*..........................................................................
 *..........................................................................*/

static dt_ctl_t *global_ctl = NULL;
static const char *global_caller = NULL;

static
void
_MCCALLBACK
report(									/* produce heap report 				*/
  MEMRECP mem)
{
  dt_ctl_t *ctl = global_ctl;
  const char *caller = global_caller;

  if (mem->flags != REPORT_START &&		/* this is a real allocation? 		*/
	  mem->flags != REPORT_END)
  {
	void *ptr = mem->ptr;				/* for short 						*/
	unsigned long size = mem->size;
    char who[32];
    dtstr_printf(ctl, 
      who, sizeof(who),
      "%12s@%-4u%s",
      mem->file, mem->line,
      mem->flags & MRFLAG_EXACT_LOC?	/* this is the exact file and line? */
        "": "?");
	dt_dbg(ctl, caller, DT_DBG_MASK_ALWAYS,
      "  0x%08lx %6lu %s",
      DT_PTR_TO_ULONG(ptr),
	  size,
	  who? who: "unknown");
  }

  return;
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
  if (mc_blocks_allocated() ==			/* no outstanding blocks? 			*/
	  mc_blocks_freed())
    dt_dbg(ctl, caller,
      DT_DBG_MASK_ALWAYS,
      "no outstanding heap allocations");
  else									/* we have outstanding blocks? 		*/
  {
	dt_dbg(ctl, caller,
      DT_DBG_MASK_ALWAYS,
      "outstanding heap allocations:");
	dt_dbg(ctl, caller, 
      DT_DBG_MASK_ALWAYS,
      "     start     size file        @line");
	global_ctl = ctl;
	global_caller = caller;
	mc_report(report);					/* report individual blocks 		*/
	dt_dbg(ctl, caller,
      DT_DBG_MASK_ALWAYS,
	  "outstanding heap allocations"
      " total %lu in %lu blocks",
      mc_bytes_allocated()-
      mc_bytes_freed(),
      mc_blocks_allocated()-
      mc_blocks_freed());
  }
  
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
      mc_blocks_allocated()+1 >
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
      mc_bytes_allocated()+size >
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
  ERF erf =								/* remember handler for a minute 	*/
    mc_set_erf(neuter_erf);				/* don't report anything			*/
  DT_Q(dtos_heap_fill,(ctl,				/* fill the heap and return amount	*/
    chunk_size, maxalloc,
    nchunks));
  (void)mc_set_erf(erf);				/* replace error reporting 			*/
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
  ERF erf =								/* remember handler for a minute 	*/
    mc_set_erf(neuter_erf);				/* don't report anything			*/
  DT_Q(dtos_heap_fill_say,(ctl,			/* fill heap and say how much	    */
    chunk_size, caller));
  (void)mc_set_erf(erf);				/* replace error reporting 			*/
  return DT_RC_GOOD;
}


/*..........................................................................
 * this gets called from dtos_malloc2() before malloc
 *..........................................................................*/

#ifdef  mc_set_location
#undef  mc_set_location
#endif
#ifdef  _mc_set_location
#undef  _mc_set_location
#endif
#ifdef  mc_check_buffers
#undef  mc_check_buffers
#endif

#define mc_set_location()
#define _mc_set_location(FILE, LINE)
#define mc_check_buffers()

dt_rc_e 
DTCONFIG_API1
dtheap_check(							/* check heap integrity 			*/
  dt_ctl_t *ctl,						/* environment control 				*/
  void *vheap,							/* heap structure 					*/
  const char *caller)					/* caller ID 						*/
{
  DT_F("dtheap_check");
  dtheap_t *heap = (dtheap_t *)vheap;

# if DTPRJ_STRATOS_MEMCHECK >= 3		/* use MemCheck for error checking?	*/
  if (heap != NULL)						/* we are on stage yet? 			*/
  {
	mc_check_buffers();					/* check integrity of all buffers 	*/
    if (heap->message[0])				/* we have an outstanding message?	*/
    {
	  heap->message[0] = '\0';			/* clear existing message 			*/
      return DT_RC_BAD;
	}
  }
# endif

  DT_Q(dtos_heap_integrity,(ctl,		/* check normal heap integrity 		*/
    caller));

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
