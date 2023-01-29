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
| To speed things up, a check is first made for pages mapped linearly.  
| This always occurs in the ram memsource.
| It can also occur in a paged under the following conditions:
| -	total memory access is less than pagesize times npages
| -	memory is accessed in page order
| 
| RETURN VALUES
| All functions Return DTOS_RANWELL on success.  
| Any other return value indicates failure.
| 
| ERRORS
| As yet undocumented.
| 
| END
 *..........................................................................*/

#include <dtack/base.h>
DT_RCSID("source/low $RCSfile: mem_std.c,v $ $Revision: 1.3 $");

#include <dtack/mem.h>
#include <dtack/os.h>
#include <dtack/str.h>


#define DESC1 "allocation structure"
#define DESC2 "page structures"
#define DESC3 "mapped array"

#ifdef NONO
#define GIVE_PTR(I)						/* link page and return it's info   */\
  memalloc->page[I].links++;			/* up link count on page		    */\
  memalloc->page[I].age =				/* give the page an age			    */\
    memalloc->age++;                                                          \
  if (ramptr != NULL)					/* caller wants pointer back?		*/\
    *ramptr = memalloc->page[I].buf +	/* give caller a pointer to use	    */\
      DTMEM_OFFSET(offset);                                                   \
  if (datasize != NULL)					/* caller wants size back?			*/\
    *datasize =                                                               \
      memalloc->page[I].datasize -		/* amount in this buffer		    */\
      DTMEM_OFFSET(offset);				/* less offset into page		    */

#define LINEAR(PAGE)					/* quick check for linear mapping	*/\
  ((page < npages) &&					/* page could be linear?			*/\
   (memalloc->page[PAGE].page==PAGE) &&	/* page is mapped linearly?			*/\
   (memalloc->page[PAGE].flags &		/* and it has been allocated?	  	*/\
    DTMEM_FLAG_ALLOCATED))
#endif

/*..........................................................................
 * caller may specify pagesize or 0 for memsource or compiled default
 * caller may specify npages limit or 0 to use memsource or compiled limit
 * returns *pmemalloc == NULL with no error if open fails to give memory
 *..........................................................................*/

dt_rc_e 
DTCONFIG_API1
dtmemstd_page_compute(					/* compute page allocation			*/
  dt_ctl_t *ctl,
  dtmemsource_t *memsource,
  unsigned int pagesize,
  unsigned int npages,
  unsigned int size,
  unsigned int units,
  const char *f,
  const char *desc,
  unsigned int *ppagesize,
  unsigned int *pnpages,
  unsigned int *pmpages,
  long *ptotal,
  dtmem_page_t **ppage,
  long *pflags)  
{
  DT_F("dtmemstd_page_compute");
  long total;
  unsigned int mpages;
  dtmem_page_t *page;
  long flags;

  if (pagesize == 0)					/* caller is not specifying?	    */
  {
    pagesize = memsource->pagesize;		/* get source's per-page size limit */
    if (pagesize == 0)					/* memsource doesn't care?		    */
      pagesize = (unsigned int)			/* get system's per-page size limit	*/
        DTCONFIG_MALLOC_MAX / 4U;		/* but keep it reasonable		    */
  }

  if (pagesize < size)
    return dt_err(ctl, F,
      "unit size %u > page size %u",
      size, pagesize);

  pagesize = 							/* integral units per page		    */
    (pagesize/size) * size;

  total = (long)size * (long)units;		/* total in this allocation		    */

  if ((long)pagesize > total)			/* everything fits in one page?	    */
    pagesize = (unsigned int)total;

  mpages = (unsigned int)				/* total pages we would need	    */
    ((total+pagesize-1) / pagesize);

  if (npages == 0 ||					/* caller is not specifying?	    */
      npages > mpages)					/* caller is specifying overkill?	*/
    npages = mpages;

  if (memsource->npages > 0 &&			/* memsource limits pages?		    */
      npages > memsource->npages)
    npages = memsource->npages;

  DT_Q(dtos_malloc2,(ctl,				/* space for the page structures	*/
    (void **)&page,
    npages *
    sizeof(dtmem_page_t),
    F, DESC2));

  DT_MEMSET(page, 0,					/* zero out the new structures	 	*/
    npages *
    sizeof(dtmem_page_t));

  flags = 0;
  if (total >							/* too much to fit all at once?  	*/
      (long)pagesize * (long)npages)
    flags |= DTMEMALLOC_FLAG_PAGED;		/* then it is called paged		    */
  else									/* we are completely mapped?		*/
  if (npages > 1)						/* but there is more than one page?	*/
    flags |= DTMEMALLOC_FLAG_SEGMENTED;	/* then it is called segmented   	*/

  flags |= DTMEMALLOC_FLAG_FREE;		/* free pages when done			    */

  *ppagesize = pagesize;
  *pnpages = npages;
  *pmpages = mpages;
  *ptotal = total;
  *pflags = flags;
  *ppage = page;
      
  dt_dbg(ctl, f, DT_DBG_MASK_PAGE,
    "will malloc as needed pagesize %u up to %u pages (%s)", 
    pagesize, npages, 
    flags & DTMEMALLOC_FLAG_PAGED?
    "paged": "all mapped");

  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/

dt_rc_e 
DTCONFIG_API1
dtmemstd_page_free(						/* free page space					*/
  dt_ctl_t *ctl,
  dtmemalloc_t *memalloc,
  const char *f,
  const char *desc)
{
  DT_F("dtmemstd_page_free");
  dt_rc_e rc = DT_RC_GOOD;

  if (memalloc &&
      memalloc->page)
  {
    if (memalloc->flags &				/* free pages when done?		    */
        DTMEMALLOC_FLAG_FREE)
    {
      int npages = memalloc->npages;
      int i;
      dt_dbg(ctl, f, DT_DBG_MASK_PAGE,
        "freeing %u pages on \"%s\" for %s", 
        npages, 
        memalloc->mem.name?
        memalloc->mem.name: "NULL",
        desc);

      for (i=0; i<npages; i++)			/* for each page					*/
	    if (memalloc->page[i].flags &	/* page is allocated?			    */
            DTMEM_FLAG_ALLOCATED)
  	    DT_I(dtos_free2,(ctl,
          memalloc->page[i].buf,
          F, "page buffer"));
	}

    DT_I(dtos_free2,(ctl,				/* free page structures too		    */
      memalloc->page, F, DESC2));
  }

  return rc;
}

/*..........................................................................
 * caller may specify pagesize or 0 for memsource or compiled default
 * caller may specify npages limit or 0 to use memsource or compiled limit
 * returns *pmemalloc == NULL with no error if open fails to give memory
 *..........................................................................*/

dt_rc_e 
DTCONFIG_API1
dtmemstd_alloc(							/* allocate space on std source		*/
  dt_ctl_t *ctl,
  dtmemsource_t *memsource,
  void **pmemalloc,
  unsigned int pagesize,
  unsigned int npages,
  unsigned int size,
  unsigned int units,
  const char *f,
  const char *desc)
{
  DT_F("dtmemstd_alloc");
  dtmemalloc_t *memalloc = NULL;
  unsigned int *mapped = NULL;
  unsigned int req_pagesize = pagesize;
  unsigned int req_npages = npages;
  unsigned int mpages;
  long total;
  dtmem_page_t *page = NULL;
  long flags;
  unsigned int i;
  dt_rc_e rc;

  DT_C(dtmemstd_page_compute,(ctl,		/* compute page allocation			*/
    memsource,
    pagesize, npages,
    size, units,
    f, desc,
    &pagesize, &npages, &mpages,
    &total, &page, &flags));

  DT_C(dtos_malloc2,(ctl,				/* space for the memalloc structure	*/
    (void **)&memalloc,
    sizeof(dtmemalloc_t),
    F, DESC1));
  DT_MEMSET(memalloc, 0,					/* zero out the memalloc structure  */
    sizeof(dtmemalloc_t));

  DT_C(dtos_malloc2,(ctl,				/* space for page mapping			*/
    (void **)&mapped,
    sizeof(*mapped) * mpages,
    F, DESC3));
  for (i=0; i<mpages; i++)
    mapped[i] = DTMEM_NOTMAPPED;

  memalloc->memsource = memsource;		/* remember our source			    */
  memalloc->size      = size;			/* remember allocation parameters	*/
  memalloc->units     = units;
  memalloc->req_pagesize  = req_pagesize;
  memalloc->req_npages    = req_npages;
  memalloc->pagesize  = pagesize;
  memalloc->npages    = npages;
  memalloc->mpages    = mpages;
  memalloc->mapped    = mapped;
  memalloc->total     = total;			/* total in this allocation		    */
  memalloc->flags     = flags;
  memalloc->page      = page;
  memalloc->map       = dtmemstd_map;	/* set our methods				    */
  memalloc->unmap     = dtmemstd_unmap;
  memalloc->get       = dtmemstd_get;
  memalloc->put       = dtmemstd_put;
  memalloc->free      = dtmemstd_free;
  memalloc->remalloc  = dtmemstd_remalloc;

  DT_C(dtstr_printf,(ctl,				/* default identifier			    */
    memalloc->mem.name,
    sizeof(memalloc->mem.name),
    "%s seq %d",
    memsource->name, 
    memsource->nextalloc));

  dt_dbg(ctl, f, DT_DBG_MASK_PAGE,
    "size %u units %u (total %ld) on \"%s\" for %s", 
    size, units, total,
    memalloc->mem.name, desc);

  if (memalloc->memsource->open)		/* there is an open call?		    */
  {
    DT_C((*memalloc->memsource->open),(	/* open the memory source			*/
      ctl, &memalloc->mem,
      memsource->name,
      memsource->nextalloc,
      memalloc->total));
	if (!memalloc->mem.gotspace)		/* didn't have enough space?		*/
	{
	  dt_dbg(ctl, f, DT_DBG_MASK_PAGE,
        "unable to complete allocation"); 
      *pmemalloc = NULL;				/* indicate failure to caller	    */
      goto cleanup;
    }
	memalloc->mem.total =				/* size for lower layer's benefit 	*/
      memalloc->total;
  }

  memalloc->memalloc_links = 1;			/* initiate link count 				*/

  *pmemalloc = (void *)memalloc;

  return DT_RC_GOOD;

cleanup:								/* died during buffer allocs	    */
  if (mapped)
    DT_I(dtos_free2,(ctl, mapped,		/* free mapped array				*/
      F, DESC3));
  if (memalloc)
    DT_I(dtos_free2,(ctl, memalloc,		/* free memalloc structure itself   */
      F, DESC1));
  if (page)
    DT_I(dtos_free2,(ctl, page,			/* free page structures too		    */
      F, DESC2));
  return rc;
}

/*..........................................................................
 *..........................................................................*/

dt_rc_e 
DTCONFIG_API1
dtmemstd_flush(							/* flush all unwritten pages		*/
  dt_ctl_t *ctl,
  dtmemalloc_t *memalloc)
{
  DT_F("dtmemstd_flush");
  unsigned int i;
  for (i=0; i<memalloc->npages; i++)
  {
    if (memalloc->page[i].flags &		/* page marked for writing?  		*/
        DTMEM_FLAG_MARKED)
    {									/* write the buffer				    */
      if (memalloc->memsource->write)
      DT_Q(memalloc->memsource->write,(
        ctl, &memalloc->mem, 
        (long)memalloc->page[i].page *
        (long)memalloc->pagesize,
        memalloc->page[i].buf,
        memalloc->page[i].datasize));
      memalloc->page[i].flags &=		/* no longer needs to be written	*/
        ~DTMEM_FLAG_MARKED;
	}
  }
  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/

dt_rc_e 
DTCONFIG_API1
dtmemstd_remalloc(						/* grow/shrink space on std source	*/
  dt_ctl_t *ctl,
  dtmemalloc_t *memalloc,
  unsigned int size,
  unsigned int units,
  int *gotspace,
  const char *f,
  const char *desc)
{
  DT_F("dtmemstd_remalloc");
  unsigned int pagesize;
  unsigned int npages;
  unsigned int mpages;
  long total;
  dtmem_page_t *page = NULL;
  unsigned int *mapped = NULL;
  long flags;
  unsigned int i;
  dt_rc_e rc;
  
  DT_Q(dtmemstd_flush,(ctl,				/* flush all unwritten pages	    */
    memalloc));

  memalloc->mem.gotspace = 0;
  if (memalloc->memsource->resize)		/* memsource can resize?		    */
  DT_Q((*memalloc->memsource->resize),(	/* do it						    */
    ctl, &memalloc->mem,
    (long)size * (long)units));

  *gotspace = memalloc->mem.gotspace;
  if (!memalloc->mem.gotspace)			/* couldn't get the space? 			*/
     return DT_RC_GOOD;

  DT_Q(dtmemstd_page_compute,(ctl,		/* compute page allocation			*/
    memalloc->memsource,
    memalloc->req_pagesize, 
    memalloc->req_npages,
    size, units,
    f, desc,
    &pagesize, &npages, &mpages,
    &total, &page, &flags));

  if (pagesize == memalloc->pagesize &&	/* no change in form factor?	    */
      npages   == memalloc->npages)
  {
    DT_Q(dtos_free2,(ctl,				/* leave pages the way they were    */
      page, F, DESC2));
	page = NULL;
  }
  else
  {
    DT_Q(dtmemstd_page_free,(ctl,		/* free page allocations		    */
      memalloc, f, desc));
    memalloc->page = page;				/* use newly computed pages		    */
	page = NULL;
  }

  if (mpages != memalloc->mpages)		/* total number of pages changes? 	*/
  {
    DT_C(dtos_malloc2,(ctl,				/* space for page mapping			*/
      (void **)&mapped,
      sizeof(*mapped) * mpages,
      F, "new " DESC3));
    DT_C(dtos_free2,(ctl,				/* free mapped array				*/
      memalloc->mapped, F,
      "old " DESC3));
	memalloc->mapped = mapped;
  }

  for (i=0; i<mpages; i++)				/* nobody is mapped 				*/
    memalloc->mapped[i] = 
      DTMEM_NOTMAPPED;

  memalloc->size      = size;			/* remember new allocation params	*/
  memalloc->units     = units;
  memalloc->pagesize  = pagesize;
  memalloc->npages    = npages;
  memalloc->mpages    = mpages;
  memalloc->total     = total;			/* total in this allocation		    */
  memalloc->flags     = flags;

  memalloc->mem.total =					/* size for lower layer's benefit 	*/
    memalloc->total;

  return DT_RC_GOOD;

cleanup:								/* died during buffer allocs	    */
  if (mapped)
    DT_I(dtos_free2,(ctl, mapped,		/* free mapped array				*/
      F, DESC3));
  if (page)
    DT_I(dtos_free2,(ctl, page,			/* free page structures too		    */
      F, DESC2));
  return rc;
}

/*..........................................................................
 *..........................................................................*/

dt_rc_e 
DTCONFIG_API1
dtmemstd_free(							/* free space						*/
  dt_ctl_t *ctl,
  dtmemalloc_t *memalloc,
  const char *f,
  const char *desc)
{
  DT_F("dtmemstd_free");
  dt_rc_e rc = DT_RC_GOOD;

  if (memalloc->memsource != NULL &&
	  memalloc->memsource->close)		/* close the memory source instance	*/
  DT_I((*memalloc->memsource->close),(
    ctl, &memalloc->mem));

  if (memalloc->mapped)
    DT_I(dtos_free2,(ctl,				/* free mapped array				*/
      memalloc->mapped, F, DESC3));

  DT_I(dtmemstd_page_free,(ctl,			/* free page allocations		    */
    memalloc, f, desc));

  DT_I(dtos_free2,(ctl, memalloc,
    F, DESC1));
  
  return rc;
}

/*..........................................................................
 *..........................................................................*/

dt_rc_e 
DTCONFIG_API1
dtmemstd_map(							/* map space						*/
  dt_ctl_t *ctl,
  dtmemalloc_t *memalloc,
  long offset,		
  void **ramptr,
  unsigned int *datasize)
{
  DT_F("dtmemstd_map");
  unsigned int npages = memalloc->npages;
  unsigned int i;
  int unallocated = -1;
  int unlinked = -1;
  unsigned int page;
  long page_beg;

  if (DTMEM_ISMAPPED(ctl, memalloc,		/* quick check to see if mapped 	*/
        offset, &page))
  {
    DTMEM_LINK(ctl, memalloc, offset, page, ramptr, datasize);
	return DT_RC_GOOD;
  }

  if (offset < 0 ||
      offset >= memalloc->total)
    return dt_err(ctl, F,
      "offset %ld beyond allocated memory %ld",
      offset, memalloc->total);

  for (i=0; i<npages; i++)				/* search for a page to use		    */
  {
	if (unallocated == -1 &&			/* take first one encountered	    */
        !(memalloc->page[i].flags &		/* remember an unallocated page    	*/
          DTMEM_FLAG_ALLOCATED))
      unallocated = i;

    if (memalloc->page[i].links == 0)	/* page is not currently linked?	*/
	{
      if (unlinked == -1 ||				/* first unlinked one encountered?  */
          memalloc->page[i].age <		/* this unlinked page is older? 	*/
          memalloc->page[unlinked].age)
        unlinked = i;
	}
  }

  if (i == npages)						/* our page not in memory?		    */
  {
	if (unallocated == -1)				/* no free pages we can allocate?	*/
	{
      if (unlinked == -1)				/* no unlinked pages we can reuse?  */
        return dt_err(ctl, F, 
          "%d pages already mapped"
          " and linked on \"%s\"",
          i, memalloc->mem.name);
	  else								/* we found an unlinked page	    */
	  {
        i = unlinked;
        dt_dbg(ctl, F, DT_DBG_MASK_PAGE,
          "displacing page %u"
          " for page %u on \"%s\"",
          memalloc->page[i].page, page,
          memalloc->mem.name);
		if (memalloc->mapped != NULL)	/* we are bothering to map? 		*/
          memalloc->mapped[				/* old page is not mapped anymore 	*/
            memalloc->page[i].page] = 
              DTMEM_NOTMAPPED;

        if (memalloc->page[i].flags &	/* page marked for writing?  		*/
            DTMEM_FLAG_MARKED)
		{								/* write the buffer				    */
          if (memalloc->memsource->write)
          DT_Q((*memalloc->memsource->write),(
            ctl, &memalloc->mem, 
            (long)memalloc->page[i].page *
            (long)memalloc->pagesize,
            memalloc->page[i].buf,
            memalloc->page[i].datasize));

          memalloc->page[i].flags &=	/* no longer needs to be written	*/
            ~DTMEM_FLAG_MARKED;
		}
	  }
	}
	else								/* we have an unallocated page	    */
	{
	  dt_rc_e rc;
	  i = unallocated;
	  dt_dbg(ctl, F, DT_DBG_MASK_PAGE,
        "malloc'ing %u bytes for page %u on \"%s\"",
        memalloc->pagesize, page,
        memalloc->mem.name);
	  DT_G(dtos_malloc2,(ctl,			/* get ram space for this page	    */
        (void **)&memalloc->page[i].buf, 
        memalloc->pagesize,
        F, "page buffer"));
	  if (rc != DT_RC_GOOD)				/* failed to get memory for page? 	*/
	  {
#       ifdef NONO						/* os_mem does this now 			*/
		if (DT_DBG(ctl, 
             DT_DBG_MASK_MEMSTATS))
          DT_I(dt_heapmon_report,(ctl,	/* spit out report				    */
            F));
#       endif
		return rc;
	  }

	  memalloc->page[i].flags |= 
        DTMEM_FLAG_ALLOCATED;
	}

	if (memalloc->mapped != NULL)		/* we are bothering to map? 		*/
	  memalloc->mapped[page] = i;		/* cross index 						*/

    memalloc->page[i].page = page;		/* page is now inhabited		    */
    page_beg = (long)page *				/* offset of start of page		    */
      (long)memalloc->pagesize;
     
    if (page_beg + (long)memalloc->pagesize >	/* full buffer goes past total?	    */
        (long)memalloc->total)			/* happens in last page only	    */
      memalloc->page[i].datasize =		/* buffer only partially full	    */
        (unsigned int)
        (memalloc->total-page_beg); 
    else
      memalloc->page[i].datasize =		/* buffer completely full			*/
        memalloc->pagesize;
  }
#ifdef NONO
  else									/* our page is already in memory?	*/
    page_beg = (long)page *				/* offset of start of page		    */
      (long)memalloc->pagesize;
#endif

  DTMEM_LINK(ctl, memalloc, offset,		/* link page and return its info   	*/
    page, ramptr, datasize);

  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/

dt_rc_e 
DTCONFIG_API1
dtmemstd_unmap(							/* unmap space						*/
  dt_ctl_t *ctl,
  dtmemalloc_t *memalloc,
  long offset)	
{
  DT_F("dtmemstd_unmap");
  unsigned int page;

  DTMEMSTD_UNMAP(ctl, memalloc, offset, page);

  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/

dt_rc_e 
DTCONFIG_API1
dtmemstd_get(							/* get space						*/
  dt_ctl_t *ctl,
  dtmemalloc_t *memalloc,
  long offset,
  void **ramptr,
  unsigned int *datasize)
{
  DT_F("dtmemstd_get");
  unsigned int i;
  unsigned int page;

  if (offset < 0 ||
      offset >= memalloc->total)
    return dt_err(ctl, F,
      "offset %ld beyond allocated memory %ld",
      offset, memalloc->total);

  if (DTMEM_ISMAPPED(ctl, memalloc,		/* quick check to see if mapped 	*/
        offset, &page))
  {
    DTMEM_LINK(ctl, memalloc, offset,	/* link the page and get the info 	*/
      page, ramptr, datasize);
	return DT_RC_GOOD;
  }

  DT_Q(dtmemstd_map,(ctl,				/* map disk space to ram		    */
    memalloc, offset, 
    ramptr, datasize));

  if (memalloc->mapped != NULL) 
  {
	i = memalloc->mapped[page];			/* get the index 					*/

	if (i == DTMEM_NOTMAPPED)			/* page still not in ram?		    */
      return dt_err(ctl, F, 
        "page at %04d:%04d still not mapped", 
        page, DTMEM_OFFSET(ctl, memalloc, offset));
  }
  else
    i = page;

  if (memalloc->memsource->read)
  {
    dt_rc_e rc;
    DT_G((*memalloc->memsource->read),(	/* read buffer					    */
      ctl, &memalloc->mem, 
      (long)memalloc->page[i].page *
      (long)memalloc->pagesize, 
      memalloc->page[i].buf, 
      memalloc->page[i].datasize));

    if (rc != DT_RC_GOOD)				/* read failed?					    */
    {
      memalloc->page[i].links--;		/* reduce link count on page		*/
      return rc;
    }
  }

  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/

dt_rc_e 
DTCONFIG_API1
dtmemstd_put(							/* put space						*/
  dt_ctl_t *ctl,
  dtmemalloc_t *memalloc,
  long offset)
{
  DT_F("dtmemstd_put");
  unsigned int page;

  DTMEMSTD_PUT(ctl, memalloc, offset, page);

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
