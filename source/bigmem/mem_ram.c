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
| All functions Return DTOS_RANWELL on success.  
| Any other return value indicates failure.
| 
| ERRORS
| As yet undocumented.
| 
| END
 *..........................................................................*/

#include <dtack/base.h>
DT_RCSID("source/low $RCSfile: mem_ram.c,v $ $Revision: 1.3 $");
#include <dtack/mem.h>
#include <dtack/os.h>
#include <dtack/str.h>

#define DESC1 "allocation structure"
#define DESC2 "page structures"
#define DESC3 "page buffer"

/*..........................................................................
 * uninit does not free any allocations which may be outstanding
 * outstanding allocations must be freed before the source is uninitted!
 *..........................................................................*/

static
dt_rc_e
dtmemram_uninit(						/* uninit a "ram" memsource   	    */
  dt_ctl_t *ctl,
  dtmemsource_t *memsource)
{
  DT_F("dtmemram_uninit");
  dt_dbg(ctl, F, DT_DBG_MASK_PAGE,
    "\"%s\" being uninitialized", 
    memsource->name);
  DT_Q(dtmemsource_uninit,(ctl,			/* do common uninit for memsources	*/
    memsource));
  return DT_RC_GOOD;
}

/*..........................................................................
 * returns *pmemalloc == NULL with no error if allocation of pages fails
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtmemram_alloc(							/* allocate space on "ram" source	*/
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
  DT_F("dtmemram_alloc");
  dtmemalloc_t *memalloc = NULL;
  unsigned int req_pagesize = pagesize;
  unsigned int req_npages;
  unsigned int mpages;
  long total;
  dtmem_page_t *page;
  long flags;
  unsigned int i;
  dt_rc_e rc;

  npages = 0;							/* ignore page limits on ram		*/
  req_npages = npages;

  DT_Q(dtmemstd_page_compute,(ctl,		/* compute page allocation			*/
    memsource,
    pagesize, npages,
    size, units,
    f, desc,
    &pagesize, &npages, &mpages,
    &total, &page, &flags));

  if (total >							/* too much data to fit?		    */
      (long)pagesize * (long)npages)
  {
    DT_Q(dtos_free2,(ctl,				/* free page structures			    */
      page, F, DESC2));
    return dt_err(ctl, F,				/* complain						    */
      "size %u * units %u >"
      " pagesize %u * npages %u",
      size, units, pagesize, npages);
  }

  DT_G(dtos_malloc2,(ctl,				/* space for the memalloc structure	*/
    (void **)&memalloc,
    sizeof(dtmemalloc_t), F, DESC1));
  if (rc != DT_RC_GOOD)
  {
    DT_Q(dtos_free2,(ctl,				/* free page structures			    */
      page, F, DESC2));
    return rc;
  }
  DT_MEMSET(memalloc, 0,					/* zero out the memalloc structure  */
    sizeof(dtmemalloc_t));

  memalloc->memsource = memsource;		/* remember our source			    */
  memalloc->size      = size;			/* remember allocation parameters	*/
  memalloc->units     = units;
  memalloc->req_pagesize  = req_pagesize;
  memalloc->req_npages    = req_npages;
  memalloc->pagesize  = pagesize;
  memalloc->npages    = npages;
  memalloc->mpages    = mpages;
  memalloc->total     = total;			/* total in this allocation		    */
  memalloc->flags     = flags;
  memalloc->page      = page;
  memalloc->map       = dtmemstd_map;	/* set our methods				    */
  memalloc->unmap     = dtmemstd_unmap;
  memalloc->get       = dtmemstd_get;
  memalloc->put       = dtmemstd_put;
  memalloc->free      = dtmemstd_free;
  memalloc->remalloc   = dtmemram_remalloc;

  DT_C(dtstr_printf,(ctl,				/* default identifier			    */
    memalloc->mem.name,
    sizeof(memalloc->mem.name),
    "%s%05d",
    memsource->name, 
    memsource->nextalloc));

  dt_dbg(ctl, f, DT_DBG_MASK_PAGE,
    "malloc'ing %u units of %u (total %ld) for %s on \"%s\"", 
    units, size, total, desc,
    memalloc->mem.name);

  for (i=0; i<memalloc->npages; i++)
  {
    long page_beg;
    page_beg = (long)i *				/* offset of start of page		    */
      (long)memalloc->pagesize;
     
    if (page_beg+(long)memalloc->pagesize >	/* full buffer goes past total?	    */
        (long)memalloc->total)				/* happens in last page only	    */
      memalloc->page[i].datasize =		/* buffer only partially full	    */
        (unsigned int)
        (memalloc->total-page_beg); 
    else
      memalloc->page[i].datasize =		/* buffer completely full			*/
        memalloc->pagesize;

	DT_C(dtos_malloc3,(ctl,				/* try to get the space			    */
      (void **)&memalloc->page[i].buf,
      memalloc->page[i].datasize,
      F, DESC3));
    if (memalloc->page[i].buf == NULL)	/* ran out of space?			    */
	{
      *pmemalloc = NULL;				/* indicate condition to caller	    */
      goto cleanup;						/* free stuff allocated so far	    */
	}
      
	memalloc->page[i].flags |= 
      DTMEM_FLAG_ALLOCATED;
    memalloc->page[i].page = i;			/* page is now inhabited		    */
  }

  memalloc->mem.type =					/* remember our type			    */
    DTMEM_TYPE_MALLOC;		

  memalloc->memalloc_links = 1;			/* initiate link count 				*/

  *pmemalloc = (void *)memalloc;

  return DT_RC_GOOD;

cleanup:								/* died during buffer allocs	    */
  if (memalloc)
  {
    DT_I(dtmemstd_page_free,(ctl,		/* free page allocations		    */
      memalloc, f, desc));
    DT_I(dtos_free2,(ctl, memalloc,		/* free memalloc structure itself   */
      F, DESC1));
  }

  return rc;
}

/*..........................................................................
 *..........................................................................*/

dt_rc_e 
DTCONFIG_API1
dtmemram_remalloc(						/* grow/shrink space on ram source	*/
  dt_ctl_t *ctl,
  dtmemalloc_t *memalloc,
  unsigned int size,
  unsigned int units,
  int *gotspace,
  const char *f,
  const char *desc)
{
  DT_F("dtmemram_remalloc");
  unsigned int pagesize;
  unsigned int npages;
  unsigned int mpages;
  long total;
  dtmem_page_t *page;
  long flags;
  unsigned int i, n;
  dt_rc_e rc = DT_RC_GOOD;

  DT_Q(dtmemstd_page_compute,(ctl,		/* compute page allocation			*/
    memalloc->memsource,
    memalloc->req_pagesize, 
    memalloc->req_npages,
    size, units,
    f, desc,
    &pagesize, &npages, &mpages,
    &total, &page, &flags));

  if (total >							/* too much data to fit?		    */
      (long)pagesize * (long)npages)
  {
    dt_dbg(ctl, F, DT_DBG_MASK_PAGE,	/* complain						    */
      "size %u * units %u >"
      " pagesize %u * npages %u",
      size, units, pagesize, npages);
    goto cleanup;						/* free stuff allocated so far	    */
  }
  
  if (pagesize != memalloc->pagesize)	/* form factor is changing? 		*/
  {
    dt_dbg(ctl, F, DT_DBG_MASK_PAGE,	/* complain						    */
      "size %u * units %u needs"
      " pagesize %u"
      " but old pagesize is %u",
      size, units, pagesize,
      memalloc->pagesize);
    goto cleanup;						/* free stuff allocated so far	    */
  }

  for (i=memalloc->npages;				/* for pages being added		    */
       i<npages;
       i++)
  {
	long page_beg;
	page_beg = (long)i *				/* offset of start of page		    */
      (long)memalloc->pagesize;
     
	if (page_beg+
		  (long)memalloc->pagesize >	/* full buffer goes past total?	    */
        total)							/* happens in last page only	    */
      page[i].datasize =				/* buffer only partially full	    */
        (unsigned int)
        (total-page_beg); 
    else
      page[i].datasize =				/* buffer completely full			*/
        memalloc->pagesize;

    DT_C(dtos_malloc3,(ctl,				/* try to get the space			    */
      (void **)&page[i].buf,
      page[i].datasize,
      F, DESC3));
    if (page[i].buf == NULL)			/* ran out of space?			    */
       goto cleanup;					/* free stuff allocated so far	    */
      
    page[i].flags |= 
      DTMEM_FLAG_ALLOCATED;
    page[i].page = i;					/* page is now inhabited		    */
  }

  n = DT_MIN(memalloc->npages, npages);	/* number of reusable pages		    */
  i = n - 1;							/* index of page being resized	    */

  page[i].datasize = (unsigned int)
    DT_MIN(								/* size of page being resized		*/
      (long)pagesize,					/* either a whole page			    */
      total - (long)i*(long)pagesize);	/* or possibly a partial page	    */
      
  if (page[i].datasize == 0)			/* possibly an integral multiple    */
     page[i].datasize = pagesize;
  if (page[i].datasize ==				/* keeping entire last page?	    */
      memalloc->page[i].datasize)
  {
    page[i].buf =						/* no need to grow				    */
      memalloc->page[i].buf;
    dt_dbg(ctl, F, DT_DBG_MASK_PAGE,
      "leaving last page at %u",
      page[i].datasize);
  }
  else
  {
    DT_Q(dtos_realloc3,(ctl,			/* grow/shrink last page		    */
       memalloc->page[i].buf,
       (void **)&page[i].buf,
       page[i].datasize, f, desc));
    if (page[i].buf == NULL)
      goto cleanup;
    dt_dbg(ctl, F, DT_DBG_MASK_PAGE,
      "realloced last page from %u to %u",
      memalloc->page[i].datasize,
      page[i].datasize);
  }
  page[i].flags |= 
    DTMEM_FLAG_ALLOCATED;
  page[i].page = i;						/* page is now inhabited		    */

  for (i=0; i<n-1; i++)					/* copy pages we are keeping		*/
    page[i] = memalloc->page[i];

  for (i=n;								/* for all discarded pages			*/
       i<memalloc->npages;
       i++)
    DT_Q(dtos_free2,(ctl,				/* free them					    */
      memalloc->page[i].buf,
      F, DESC3));

  DT_Q(dtos_free2,(ctl,					/* free old page structures		    */
    memalloc->page, F, DESC2));

  memalloc->size      = size;			/* remember new allocation params	*/
  memalloc->units     = units;
  memalloc->total     = total;			/* total in this allocation		    */
  memalloc->pagesize  = pagesize;
  memalloc->npages    = npages;
  memalloc->mpages    = mpages;
  memalloc->page      = page;			/* new page structures			    */
  memalloc->flags     = flags;

  *gotspace = 1;

  return DT_RC_GOOD;

cleanup:								/* died during buffer allocs	    */

  if (page)
  {
    for (i=0; i<npages; i++)				/* free buffers that got allocated	*/
      if (page[npages-i-1].buf)
        DT_I(dtos_free2,(ctl,
          page[npages-i-1].buf,
          F, DESC3));

    DT_I(dtos_free2,(ctl,				/* free array of page structures    */
      page,
      F, DESC2));
  }

  *gotspace = 0;

  return rc;
}

/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtmemram_prealloc(						/* wrap structures around buffers	*/
  dt_ctl_t *ctl,
  dtmemsource_t *memsource,
  void **pmemalloc,
  unsigned int size,
  unsigned int units,
  void **pages,
  unsigned int pagesize,
  unsigned int npages,
  const char *f,
  const char *desc)
{
  DT_F("dtmemram_prealloc");
  dtmemalloc_t *memalloc;
  unsigned int i;
  long total;
  long given;
  long page_beg;
  dt_rc_e rc;

  if (pagesize == 0)					/* forgot to tell how big?	    	*/
    return dt_err(ctl, F,
      "invalid pagesize (%u)"
      " for caller-provided pages",
      pagesize);
  if (npages == 0)						/* forgot to tell how big?	    	*/
    return dt_err(ctl, F,
      "invalid npages (%u)"
      " for caller-provided pages",
      npages);

  pagesize = 							/* integral units per page		    */
    (pagesize/size) * size;

  if (pagesize == 0)
    return dt_err(ctl, F,
      "unit size %u too large"
      " for buffer size %u",
      size, pagesize);

  total = (long)size * (long)units;		/* total in this allocation		    */

  if ((long)pagesize > total)			/* everything fits in one page?	    */
    pagesize = (unsigned int)total;

  given = (long)pagesize *
          (long)npages;
  if (given < total)
    return dt_err(ctl, F,
      "%u units of %u > given %u pages of %u",
      units, size, npages, pagesize);

  DT_Q(dtos_malloc2,(ctl,				/* space for the memalloc structure	*/
    (void **)&memalloc,
    sizeof(dtmemalloc_t), F, DESC1));
  DT_MEMSET(memalloc, 0,					/* zero out the memalloc structure  */
    sizeof(dtmemalloc_t));

  DT_G(dtos_malloc2,(ctl,				/* space for the page structures	*/
    (void **)&memalloc->page,
    npages *
    sizeof(dtmem_page_t),
    F, DESC2));
  if (rc != DT_RC_GOOD)
  {
    DT_Q(dtos_free2,(ctl,
      memalloc, F, DESC1));
    return rc;
  }
  DT_MEMSET(memalloc->page, 0,				/* zero out the new structures	 	*/
    npages *
    sizeof(dtmem_page_t));

  memalloc->memsource = memsource;		/* remember our source			    */
  memalloc->size      = size;			/* remember allocation parameters	*/
  memalloc->units     = units;
  memalloc->pagesize  = pagesize;
  memalloc->npages    = npages;
  memalloc->mpages    = npages;
  memalloc->total     = total;			/* total in this allocation		    */
  memalloc->map       = dtmemstd_map;	/* set our methods				    */
  memalloc->unmap     = dtmemstd_unmap;
  memalloc->get       = dtmemstd_get;
  memalloc->put       = dtmemstd_put;
  memalloc->free      = dtmemstd_free;

  sprintf(memalloc->mem.name,			/* default identifier			    */
    "%s prealloc",
    memsource->name); 

  dt_dbg(ctl, f, DT_DBG_MASK_PAGE,
    "making pages for %u units of %u bytes (total %ld) for %s", 
    units, size, total, desc);

  for (i=0; i<npages; i++)
  {
	memalloc->page[i].buf = (char *)pages[i];
	memalloc->page[i].flags |= 
      DTMEM_FLAG_ALLOCATED;
    memalloc->page[i].page = i;			/* page is now inhabited		    */
    page_beg = (long)i *				/* offset of start of page		    */
      (long)memalloc->pagesize;
     
    if (page_beg+
		  (long)memalloc->pagesize >	/* full buffer goes past total?	    */
        (long)memalloc->total)			/* happens in last page only	    */
      memalloc->page[i].datasize =		/* buffer only partially full	    */
        (unsigned int)
        (memalloc->total-page_beg); 
    else
      memalloc->page[i].datasize =		/* buffer completely full			*/
        memalloc->pagesize;
  }

  if (npages > 1)						/* there is more than one page?		*/
    memalloc->flags |=					/* then it is called segmented   	*/
      DTMEMALLOC_FLAG_SEGMENTED;

  memalloc->mem.type =					/* remember our type			    */
    DTMEM_TYPE_PREALLOC;		
      
  memalloc->memalloc_links = 1;			/* initiate link count 				*/

  dt_dbg(ctl, f, DT_DBG_MASK_PAGE,
    "   used %u pages at pagesize %u (%s)", 
    npages, pagesize,
    memalloc->flags &
    DTMEMALLOC_FLAG_SEGMENTED?
    "segmented": "unsegmented");

  *pmemalloc = (void *)memalloc;

  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtmemram_init(							/* init a "ram" memsource		    */
  dt_ctl_t *ctl,
  dtmemsource_t *memsource,
  const char *name,
  long maxusage,
  unsigned int pagesize,
  unsigned int npages)
{
  DT_F("dtmemram_init");
  dt_dbg(ctl, F, DT_DBG_MASK_PAGE,
    "\"%s\" being initialized pagesize %u npages %u",
    name, pagesize, npages);

  DT_Q(dtmemsource_init,(ctl,			/* do common init for memsources	*/
    memsource, name,
    maxusage, pagesize, npages,
    DTMEMRAM_MAXUSAGE_DEFAULT,
    DTMEMRAM_PAGESIZE_DEFAULT,
    DTMEMRAM_NPAGES_DEFAULT));
  
  memsource->uninit = dtmemram_uninit;
  memsource->alloc  = dtmemram_alloc;	/* ram uses standard paging			*/
  memsource->open   = NULL;				/* ram doesn't need these	    	*/
  memsource->close  = NULL;
  memsource->write  = NULL;
  memsource->read   = NULL;
  
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
