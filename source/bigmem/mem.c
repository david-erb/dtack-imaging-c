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
DT_RCSID("src/low $RCSfile: mem.c,v $ $Revision: 1.2 $");
#include <dtack/mem.h>
#include <dtack/os.h>

/* ........................................................................ *
| NAME
| dtmem_alloc() - allocate a big memory block
| 
| DESCRIPTION
| dtmem_alloc() allocates a new big memory block.
| 
| A big memory block is created from one of the memory sources
| in the table ~ctl->memsource~ and
| its representation stored at ~**pmemalloc~.
| The big memory block will have the form factor
| ~npages~ pages each of size ~pagesize~ bytes.
| Its total allocation will be ~units~ units of ~size~ bytes.
| ~f~ should point to the string name of the caller, or be NULL.
| ~desc~ should be a short description of the
| intended use of the big memory block, or be NULL.
| The last two arguments are used in debug messages.
| 
| The set of memory sources stored in the table
| at ~ctl->memsource~ is scanned in order of appearance
| to find the first one which can satisfy the request.
| This table is created by dtmem_configure() from a string argument.
| The format of the string is described in the function description for
| dtmem_configure().
| In the case that either ~*ctl~ is NULL or ~ctl->memsource~ is NULL,
| then then environment variable :DT_MEM: is examined.
| If this environment variable is not set or is empty,
| then the only memory source which will be used is "ram".
|
| ~pmemalloc~ is to be considered as an opaque handle to the big memory block.
| In actuality it is a structure pointer,
| however no members of the structure are public.
| You must pass the value ~pmemalloc~
| to any other members of the :mem: Function Group
| who wish to operate on the big memory block.
|
| The total allocation of the new memory block is ~size~ times ~units~.
| 
| The values of ~size~ and ~units~ must not be 0.
| The upper limits for these values depend on the types of memory sources
| available.
| It is an error
| if there is no memory source available which can accommodate
| the requested ~size~ and ~units~ values.
| 
| Using a value of 0 for ~pagesize~ means that the default pagesize
| of the memory source should be used.
| Likewise,
| using a value of 0 for ~npages~ means that the default number of pages
| of the memory source should be used.
|
|.Macro Implementation
| A macro implementation of this function call is available.
| The macro implementation bypasses the external big memory options.
| Instead, it simply does a malloc() for the requested amount
| plus an extra 8-byte overhead in which to record the size and some flags.
| The overhead is placed at the start of the requested memory.
|
|dtinclude(nomem1.dx)
|
| RETURN VALUES
| The ~*pmemalloc~ structure is initialized to contain
| the description of a big memory block.
| The contents of the big memory block itself are not initialized
| and remain undefined.
| If an error occurs, the contents of ~*pmemalloc~ undefined.
|dtinclude(return1.dx)
| 
| ERRORS
| Unsuccessful completion may be due to:
| -	unable to allocate small RAM bookkeeping structures
| -	unable to allocate external space for the new memory block
| 
| CAVEATS
| Attempting to use an invalid ~*memalloc~ structure
| in a subsequent :mem: Function Group call
| may lead to undefined or catastrophic results.
|
| This routine allocates resources which must be freed with dtmem_free().
| Not calling dtmem_free() will leave resources allocated.
| RAM resources may be freed when the program exits,
| but XMS resources will not be freed until the computer is rebooted.
| Disk resources are never automatically reclaimed.
| 
| SEE ALSO
| Introduction		for more on the :mem: function group
| dtmem_free()		free big memory allocation
|
| EXAMPLE
| For complete, working example programs, please see the 
| :mem: Function Group Introduction.
| 
|:  dtmemalloc_t *memalloc;
|:
|:  dtmem_alloc(NULL, &memalloc, 		// allocate 10000 bytes
|:    0, 0, 100, 100, NULL, NULL);
|:
|:  dtmem_free(NULL, memalloc, 			// free the allocation
|:    NULL, NULL);
| 
| END
 * ........................................................................ */

dt_rc_e 
DTCONFIG_API1
dtmem_alloc(							/* allocate a big memory block		*/
  dt_ctl_t *ctl,						/* environment control 				*/
  void **pmemalloc,						/* structure to describe block		*/
  unsigned int pagesize,				/* RAM page size 					*/
  unsigned int npages,					/* number of RAM pages to maintain	*/
  unsigned int size,					/* unit size						*/
  unsigned int units,					/* number of units 					*/
  const char *f,						/* caller name 						*/
  const char *desc)						/* what allocation is for 			*/
{
  DT_F("dtmem_alloc");
  dtmemsource_t memsource_space;
  dtmemsource_t *memsource;
  long total;
  int i, n;

  if (f == NULL)
    f = F;
  if (desc == NULL)
    desc = "unknown";

  if (ctl != NULL)						/* we have a control structure? 	*/
  {
	if (ctl->memsource == NULL)			/* not already configured? 			*/
      DT_Q(dtmem_configure,(ctl,		/* configure memory interfaces	    */
	    ctl->mem));
    memsource =							/* list of available interfaces	    */
      (dtmemsource_t *)ctl->memsource;
	n = DT_CTL_MEMSOURCES;
  }
  else									/* no control structure? 			*/
  {
	memsource = &memsource_space;
    DT_Q(dtmemram_init,(ctl,			/* init the temporary memory source	*/
      memsource, "ram", 0, 0, 0));
    n = 1;								/* only one to choose from 			*/
  }

  total = (long)size * (long)units;		/* total bytes we need			    */

  for (i=0; i<n; i++)					/* scan list of available sources	*/
  if (memsource[i].alloc)				/* this one is not available	    */
  {
    long pagebuffers = 0;
	if (memsource[i].alloc ==			/* this is a ram memsource? 		*/
        dtmemram_alloc)
	  pagebuffers =
  	    (long)memsource[i].pagesize *	/* ram to be used by page buffers 	*/
        (long)memsource[i].npages +
        (long)memsource[i].npages *		/* ram to be used by page structs 	*/
        (long)sizeof(dtmem_page_t);
	
    if (memsource[i].maxusage &&		/* source has a cap?			    */
        total+memsource[i].usage >		/* and we would exceed it?		    */
        memsource[i].maxusage -
        pagebuffers)					/* account for ram buffers			*/
	{
	  dt_dbg(ctl, f, DT_DBG_MASK_PAGE,
        "\"%s\" has %ld left of %ld"
        " need %ld,"
        " trying next memsource",
        memsource[i].name, 
        memsource[i].maxusage -
        memsource[i].usage,
        memsource[i].maxusage,
        total+pagebuffers);
      continue;							/* try another source			    */
	}

    DT_Q((memsource[i].alloc),(ctl,		/* allocate with the interface	    */
      &memsource[i],
      pmemalloc, 
      pagesize, npages,    
      size, units,
      f, desc));
    if (*pmemalloc == NULL)				/* not enough memory here?		    */
	{
	  dt_dbg(ctl, f, DT_DBG_MASK_PAGE,
        "\"%s\" has less space than %ld,"
        " trying next memsource",
        memsource[i].name, total);
      continue;
	}

    memsource[i].nextalloc++;			/* sequence number					*/
    memsource[i].nallocs++;				/* keep count of open allocs	    */
    memsource[i].usage += 				/* keep track of how much used	    */
      (long)size * (long)units;
    break;
  }

  if (i == n)							/* no resource with enough space?   */
    return dt_err(ctl, f,
      "no memsource has %ld bytes"
      " available for %s",
      total, desc);
  else
  if (DT_DBG(ctl, DT_DBG_MASK_PAGE))
  {
    char tmp[32];
	sprintf(tmp, "%ld", 
      memsource[i].maxusage);
    dt_dbg(ctl, f, DT_DBG_MASK_PAGE,
      "selected \"%s\","
      " usage now %ld, limit %s",
      memsource[i].name, 
	  memsource[i].usage,
	  memsource[i].maxusage? 
        tmp: "none");
  }

  if (ctl == NULL)						/* no control structure? 			*/
    ((dtmemalloc_t *)(*pmemalloc))->	/* no place to keep memsource space */
      memsource = NULL;					
  
  return DT_RC_GOOD;
}

/* ........................................................................ *
| NAME
| dtmem_realloc() - change big memory block size
| 
| DESCRIPTION
| dtmem_realloc() changes the size of a big memory block.
| Contents of the big memory block are unchanged up to the minimum of the
| new and old sizes are unchanged.
| 
| ~*memalloc~ represents a big memory block created by dtmem_alloc().
| The big memory block is changed to have the form factor
| ~npages~ pages each of size ~pagesize~ bytes.
| Its total allocation is changed to be ~units~ units of ~size~ bytes.
| the contents of the old memory block are transferred to the new one.
| Transfer stops at the minimum of the old and new total allocation.
| ~f~ should point to the string name of the caller, or be NULL.
| ~desc~ should be a short description of the
| intended use of the big memory block, or be NULL.
| The last two arguments are used in debug messages.
|
| The total allocation of the new memory block is ~size~ times ~units~.
| If the new total allocation is greater than that of the existing
| big memory block,
| then the contents of the new block past the 
| existing total allocation are undefined.
| If the new total allocation is less than the old,
| then the contents of the old block are silently truncated.
| 
| Using a value of 0 for ~size~ means that the unit size
| of the existing big memory block should not be changed.
| Likewise,
| using a value of 0 for ~units~ means that the number of units
| used by the existing big memory block should not be changed.
| It does not make sense to use 0 for both ~size~ and ~units~
| since this would mean the total allocation remains the same,
| defeating the purpose of the function.
| 
| Using a value of 0 for ~pagesize~ means that the pagesize
| of the existing big memory block should not be changed.
| Likewise,
| using a value of 0 for ~npages~ means that the number of pages
| used by the existing big memory block should not be changed.
|
| The form factor of a big memory block is specified by its
| ~pagesize~ and ~npages~ values.
| If three specific conditions are met,
| then the block is left resident
| on the old memory source and simply increased in size.
| These conditions are:
| 
| 1.	the memory source of the existing block supports native reallocation
| 1.	the memory source of the existing block would not be overfilled
|		by a requested increase in size
| 2.	the requested form factor is not different from the existing block
|
| If any of these conditions is not met, 
| then an entirely new memory block is allocated.
| The contents are copied and the old block freed.
|
| If ~*memalloc~ was returned by a dtmem_prealloc() call,
| then an error will result.
|
|.Macro Implementation
| A macro implementation of this function call is available.
| The macro implementation bypasses the external big memory options.
| Instead, it simply does a realloc() for the requested amount
| plus an extra 8-byte overhead in which to record the size and some flags.
| The overhead is placed at the start of the requested memory.
|
|dtinclude(nomem1.dx)
|
| RETURN VALUES
| The ~*memalloc~ structure is changed to represent the new size and/or
| form factor.
| Contents of the old memory block are copied to the new space.
| If an error occurs, the value and contents of ~*memalloc~
| are left in such a state that it still represents a valid
| big memory block, however the desired size change may not
| have been accomplished.
|dtinclude(return1.dx)
| 
| ERRORS
| Unsuccessful completion may be due to:
| -	~*memalloc~ was created by dtmem_prealloc()
| - ~*memalloc~ has more than one link
| -	unable to allocate space for the new memory block
| -	error accessing old or new memory block during contents transfer
| 
| CAVEATS
| Attempting to use a ~*memalloc~ structure which 
| does not represent a valid big memory block may lead to undefined
| or catastrophic results.
|
| This routine allocates resources which must be freed with dtmem_free().
| Not calling dtmem_free() will leave resources allocated.
| RAM resources may be freed when the program exits,
| but XMS resources will not be freed until the computer is rebooted.
| Disk resources are never automatically reclaimed.
| 
| SEE ALSO
| Introduction		for more on the :mem: function group
| dtmem_alloc()		initial big memory allocation
|
| EXAMPLE
| For complete, working example programs, please see the 
| :mem: Function Group Introduction.
| 
|:  dtmemalloc_t *memalloc;
|:
|:  dtmem_alloc(NULL, &memalloc, 		// allocate 10000 bytes
|:    0, 0, 100, 100, NULL, NULL);
|:
|:  dtmem_realloc(NULL, memalloc, 		// reallocate 20000 bytes
|:    0, 0, 100, 200, NULL, NULL);
|:
|:  dtmem_free(NULL, memalloc, 			// free the allocation
|:    NULL, NULL);
| 
| END
 * ........................................................................ */

dt_rc_e 
DTCONFIG_API1
dtmem_realloc(							/* change big memory block size		*/
  dt_ctl_t *ctl,						/* environment control 				*/
  void *memalloc,						/* big memory to change size of 	*/
  unsigned int pagesize,				/* new page size 					*/
  unsigned int npages,					/* new RAM pages to maintain 		*/
  unsigned int size,					/* new unit size					*/
  unsigned int units,					/* new number of units 				*/
  const char *f,						/* caller name 						*/
  const char *desc)						/* what allocation is for 			*/
{
  DT_F("dtmem_realloc");
  dtmemalloc_t *smemalloc =
    (dtmemalloc_t *)memalloc;
  dtmemsource_t memsource_space;
  dtmemsource_t *memsource;
  long old_total;
  long new_total;
  dtmemalloc_t *dmemalloc = NULL;
  int gotspace = 0;
  dt_rc_e rc;

  if (f == NULL)
    f = F;
  if (desc == NULL)
    desc = "unknown";

  if (smemalloc->memalloc_links > 1)	/* multiple links to this memalloc? */
  {
	return dt_err(ctl, f,
      "can't realloc \"%s\","
      " memalloc has %d links",
      smemalloc->mem.name?
      smemalloc->mem.name: "NULL",
	  smemalloc->memalloc_links);
  }
  
  if (ctl != NULL &&					/* we have a control structure? 	*/
	  ctl->memsource != NULL &&
	  smemalloc->memsource != NULL)
  {
    memsource = smemalloc->memsource;	/* memory source already being used	*/
  }
  else									/* no control structure? 			*/
  {
	memsource = &memsource_space;
    DT_Q(dtmemram_init,(ctl,			/* init the temporary memory source */
      memsource, "ram", 0, 0, 0));
    smemalloc->memsource = memsource;
  }

  if (pagesize == 0)					/* caller not specifying page size? */
    pagesize = smemalloc->req_pagesize;	/* use originally requested size 	*/
  if (npages == 0)
    npages = smemalloc->req_npages;
  if (size == 0)
    size = smemalloc->size;
  if (units == 0)
    units = smemalloc->units;

  old_total = smemalloc->total;
  new_total = (long)size * (long)units;


  if (pagesize ==						/* not changing form factor?	    */
      smemalloc->req_pagesize &&
      npages   == 
      smemalloc->req_npages &&
      smemalloc->remalloc)				/* and there is a remalloc method?	*/
  {
    if (memsource->maxusage == 0 ||		/* source has no cap?			    */
        new_total-old_total+			/* or we would not exceed it?	    */
          memsource->usage <=
          memsource->maxusage)
	{
      DT_Q(smemalloc->remalloc,(ctl,		/* try to remalloc				    */
        smemalloc, size, units,
        &gotspace, f, desc));
	  
	  if (gotspace)						/* remalloc worked?				   	*/
        dt_dbg(ctl, f, DT_DBG_MASK_PAGE,
          "exhausted \"%s\", retrying"
          " remalloc %ld for %s",
          memsource->name, 
          new_total, desc);
	}
  }
  else
  if (smemalloc->remalloc)				/* there is a remalloc method?		*/
    dt_dbg(ctl, f, DT_DBG_MASK_PAGE,
      "form factor changed"
      " from %ux%u to %ux%u"
      " on \"%s\", retrying"
      " remalloc %ld for %s",
 	  smemalloc->req_pagesize,
	  smemalloc->req_npages,
	  pagesize,
	  npages,
      memsource->name, 
      new_total, desc);
  else
    dt_dbg(ctl, f, DT_DBG_MASK_PAGE,
      "\"%s\" does not support"
      " native remalloc, retrying"
      " realloc %ld for %s",
      memsource->name, 
      new_total, desc);

  if (gotspace)							/* we tried it and it worked?	   	*/
  {
    memsource->usage += 				/* keep track of how much used	    */
      new_total - old_total;

    if (DT_DBG(ctl, DT_DBG_MASK_PAGE))
    {
      char tmp[32];
      sprintf(tmp, "%ld", 
        memsource->maxusage);
      dt_dbg(ctl, f, DT_DBG_MASK_PAGE,
        "realloced some on \"%s\", usage now %ld, limit %s",
        memsource->name, 
	    memsource->usage,
	    memsource->maxusage? tmp: "none");
    }
  }
  else									/* realloc failed?				    */
  {
	long flags =
      smemalloc->flags;
    DT_Q(dtmem_alloc,(ctl,				/* allocate new memory			    */
      (void **)&dmemalloc,	
      pagesize, npages,
      size, units,
      f, desc));
    DT_C(dtmem_copy,(ctl,				/* copy old memory to new		    */
      (void *)smemalloc,
      (void *)dmemalloc));
	{
	  dtmemalloc_t tmp = *smemalloc;	/* swap the guts 					*/
	  *smemalloc = *dmemalloc;
	  *dmemalloc = tmp;
	}
    DT_Q(dtmem_free,(ctl,				/* free old memory				    */
      (void *)dmemalloc,	
      f, desc));
	smemalloc->flags = flags;			/* remember old flags though 		*/
  }

  if (ctl == NULL)						/* no control structure? 			*/
    smemalloc->memsource = NULL;		/* no place to keep memsource space */

  return DT_RC_GOOD;

cleanup:
  if (dmemalloc)
    DT_I(dtmem_free,(ctl, (void *)dmemalloc,
      f, desc));
  return rc;
}

/* ........................................................................ *
| NAME
| dtmem_prealloc() - wrap big memory around malloc() memory
| 
| DESCRIPTION
| dtmem_prealloc() wraps a big memory block mechanism around of an existing
| RAM allocation which is given as an argument.
| 
| A big memory block is created from the list of ~npages~
| RAM blocks pointed to by ~*pages~.
| All the RAM blocks are assumed to be of length ~*pagesize~.
| The memory block created is represented by a structure stored at ~**pmemalloc~.
| Its total allocation will be recorded as ~units~ units of ~size~ bytes.
| ~f~ should point to the string name of the caller, or be NULL.
| ~desc~ should be a short description of the
| intended use of the big memory block, or be NULL.
| The last two arguments are used in debug messages.
|
| A big memory block created with this function
| may not be used in a dtmem_realloc() call.
| 
| When this big memory block is freed by dtmem_free(),
| the RAM blocks stored at ~*pages~ is not freed.
| Since you obtained the blocks from somewhere,
| it is your responsibility to release them.
|
| The values of ~size~ and ~units~ must not be 0.
| The upper limits for these values depend on the given RAM size,
| ~npages~ times ~pagesize~.
| It is an error it the total allocation ~units~ times ~size~
| is greater than ~npages~ times ~pagesize~,
| with ~pagesize~ first adjusted down to an integral multiple of ~size~.
| If the total allocation is less than the given RAM size,
| then the excess RAM is silently ignored.
| 
| The set of memory sources stored in the table
| at ~ctl->memsource~ is not used by this function.
| dtmem_configure() need not have been called before.
| It is not called by this function either.
| 
| ~pmemalloc~ is to be considered as an opaque handle to the big memory block.
| In actuality it is a structure pointer,
| however no members of the structure are public.
| You must pass the value ~pmemalloc~
| to any other members of the :mem: Function Group
| who wish to operate on the big memory block.
|
|.Macro Implementation
| A macro implementation of this function call is available.
| The macro implementation bypasses the external big memory options.
| in the macro implementation,
| it is an error if ~npages~ is not equal to 1,
| or if the ~*pages~ are not contiguous in RAM.
| It is also an error if fitting ~size~ times ~units~ bytes
| inside ~pagesize~ times ~npages~ does not leave room for
| extra 8-byte overhead in which to record the size and some flags.
| (~pagesize~ is first adjusted down to an integral multiple of ~size~.)
| The overhead is placed at the start of the requested memory.
|
|dtinclude(nomem1.dx)
|
| RETURN VALUES
| The ~*pmemalloc~ structure is initialized to contain
| the description of a big memory block.
| The contents of the big memory block itself are not initialized
| and remain undefined.
| If an error occurs, the contents of ~*pmemalloc~ undefined.
|dtinclude(return1.dx)
| 
| ERRORS
| Unsuccessful completion may be due to:
| -	unable to allocate small RAM bookkeeping structures
| 
| CAVEATS
| Attempting to use an invalid ~*memalloc~ structure
| in a subsequent :mem: Function Group call
| may lead to undefined or catastrophic results.
|
| This routine allocates resources which must be freed with dtmem_free().
| The RAM blocks stored at ~*pages~ are not freed by dtmem_free().
| 
| SEE ALSO
| Introduction		for more on the :mem: function group
| dtmem_alloc()		create external big memory allocation
| dtmem_free()		free big memory allocation
|
| EXAMPLE
| For complete, working example programs, please see the 
| :mem: Function Group Introduction.
| 
|:  void *pages = malloc(10000);		// preallocate 10000 bytes
|:
|:  dtmem_alloc(NULL, &memalloc, 		// wrap big memory around it
|:    10000, 1, pages,
|:    100, 100, NULL, NULL);
|:
|:  dtmem_free(NULL, memalloc, 			// free the allocation
|:    NULL, NULL);
| 
| END
 * ........................................................................ */

dt_rc_e 
DTCONFIG_API1
dtmem_prealloc(							/* wrap big memory around RAM		*/
  dt_ctl_t *ctl,						/* environment control 				*/
  void **pmemalloc,						/* structure to describe block		*/
  unsigned int size,					/* unit size						*/
  unsigned int units,					/* number of units 					*/
  void **pages,							/* RAM block given 					*/
  unsigned int pagesize,				/* given RAM page size 				*/
  unsigned int npages,					/* number of RAM pages given		*/
  const char *f,						/* caller name 						*/
  const char *desc)						/* what allocation is for 			*/
{
  DT_F("dtmem_prealloc");
  dtmemsource_t *memsource;

  if (f == NULL)
    f = F;
  if (desc == NULL)
    desc = "unknown";

  {
	dtmemsource_t memsource_space;
	memsource = &memsource_space;
    DT_Q(dtmemram_init,(ctl,			/* init the memory source		    */
      memsource, "ram", 0, 0, 0));
	DT_Q(dtmemram_prealloc,(ctl,		/* allocate with the interface	    */
      memsource,
      pmemalloc, size, units,
      pages, pagesize, npages,
      f, desc));
    ((dtmemalloc_t *)(*pmemalloc))->	/* no place to keep memsource space */
      memsource = NULL;					
  }

  return DT_RC_GOOD;
}

/* ........................................................................ *
| NAME
| dtmem_link() - link to big memory allocation
| 
| DESCRIPTION
| dtmem_link() links to a big memory allocation.
| 
| The big memory block represented by ~*vmemalloc~ 
| has its link count increased.
| Big memory blocks with link counts greater than one will not be freed.
|
| END
 * ........................................................................ */

dt_rc_e 
DTCONFIG_API1
dtmem_link(								/* free allocated memory			*/
  dt_ctl_t *ctl,						/* environment control 				*/
  void *vmemalloc)						/* big memory block description 	*/
{
  DT_F("dtmem_link");
  dtmemalloc_t *memalloc = 
    (dtmemalloc_t *)vmemalloc;

  memalloc->memalloc_links++;

  dt_dbg(ctl, F, DT_DBG_MASK_PAGE,
    "\"%s\" link count increased to %d", 
    memalloc->mem.name?
    memalloc->mem.name: "NULL",
	memalloc->memalloc_links);

  return DT_RC_GOOD;
}

/* ........................................................................ *
| NAME
| dtmem_free() - free big memory allocation
| 
| DESCRIPTION
| dtmem_free() releases a big memory allocation.
| 
| The big memory block represented by ~*vmemalloc~ is freed,
|_Aside Frees resources.
| along with any small bookkeeping structures which supported it.
| ~f~ should point to the string name of the caller, or be NULL.
| ~desc~ should be a short description of the
| intended use of the big memory block, or be NULL.
| The last two arguments are used in debug messages.
|
| If the big memory block had been created by dtmem_prealloc(),
|_aside Does not free preallocated blocks.
| then the RAM blocks themselves are not freed.
| Since you obtained the RAM blocks from somewhere,
| it is your responsibility to release them.
| The big memory bookkeeping structures are, however, released.
|
| If this was the last big memory block, then dtmem_unconfigure()
|_Aside Big memory deconfigured.
| is called to dismantle the big memory sources.
| If you call dtmem_alloc() after this, dtmem_configure() will be called
| automatically to reinstate the big memory sources.
|
|.Macro Implementation
| A macro implementation of this function call is available.
| The macro implementation simply uses free() to release the memory block.
|
|dtinclude(nomem1.dx)
|
| RETURN VALUES
| The ~*vmemalloc~ structure and all its contents are freed.
|dtinclude(return1.dx)
| 
| ERRORS
| Unsuccessful completion may be due to:
| -	unable to release external memory resources
| -	unable to free small RAM bookkeeping structures
| 
| CAVEATS
| Attempting to use a freed ~*memalloc~ structure
| in a subsequent :mem: Function Group call
| may lead to undefined or catastrophic results.
|
| This routine frees essential resources.
| Not calling it after dtmem_alloc(),
| dtmem_realloc() or dtmem_prealloc() will leave resources allocated.
| RAM resources may be freed when the program exits,
| but XMS resources will not be freed until the computer is rebooted.
| Disk resources are never automatically reclaimed.
| 
| SEE ALSO
| Introduction		for more on the :mem: function group
| dtmem_alloc()		create external big memory allocation
|
| EXAMPLE
| For complete, working example programs, please see the 
| :mem: Function Group Introduction.
| 
|:  dtmemalloc_t *memalloc;
|:
|:  dtmem_alloc(NULL, &memalloc, 		// allocate 10000 bytes
|:    0, 0, 100, 100, NULL, NULL);
|:
|:  dtmem_free(NULL, memalloc, 			// free the allocation
|:    NULL, NULL);
| 
| END
 * ........................................................................ */

dt_rc_e 
DTCONFIG_API1
dtmem_free(								/* free allocated memory			*/
  dt_ctl_t *ctl,						/* environment control 				*/
  void *vmemalloc,						/* big memory block description 	*/
  const char *f,						/* caller name 						*/
  const char *desc)						/* what allocation was for 			*/
{
  DT_F("dtmem_free");
  dtmemalloc_t *memalloc = 
    (dtmemalloc_t *)vmemalloc;

  if (f == NULL)
    f = F;
  if (desc == NULL)
    desc = "unknown";

  memalloc->memalloc_links--;			/* reduce link count 				*/
  dt_dbg(ctl, f, DT_DBG_MASK_PAGE,
    "\"%s\" link count reduced to %d", 
    memalloc->mem.name?
    memalloc->mem.name: "NULL",
	memalloc->memalloc_links);
  if (memalloc->memalloc_links > 0)		/* still more links? 				*/
	return DT_RC_GOOD;
  
  if (memalloc->memsource != NULL)
  {
    memalloc->memsource->nallocs--;		/* keep count of open allocs	    */
    memalloc->memsource->usage -=		/* keep track of how much used	    */
      (long)memalloc->size * 
      (long)memalloc->units;
  }

  DT_Q((memalloc->free),(ctl,			/* release memory block			    */
    memalloc, f, desc));

  if (ctl != NULL &&					/* we have environment control? 	*/
	  ctl->memsource != NULL)			/* and we are configured? 			*/
  {
	dtmemsource_t *memsource =
	  (dtmemsource_t *)ctl->memsource;
	int i;
	int n = 0;
	for (i=0; i<DT_CTL_MEMSOURCES; i++)
	  if (memsource[i].alloc != NULL)	/* this one is available?	    	*/
        n += memsource[i].nallocs;		/* keep count of open allocs	    */
	if (n == 0)							/* no outstanding big memory? 		*/
	{
	  DT_Q(dtmem_unconfigure,(ctl));	/* dismantle big memories 			*/
  	  ctl->memsource = NULL;			/* force configure next time 		*/
	}
  }

  return DT_RC_GOOD;
}

/* ........................................................................ *
| NAME
| dtmem_map() - map piece of big memory to direct-access buffer
| 
| DESCRIPTION
| dtmem_map() binds a piece of big memory to a direct-access buffer
| without copying the contents into the buffer.
| 
| ~*memalloc~ represents a big memory block created by dtmem_alloc().
| The piece of big memory starting at ~offset~ 
| is bound to one of the block's direct-access buffers.
| The pointer into the direct-access buffer corresponding to ~offset~
| is returned in ~*ramptr~, and the number of bytes from this point
| to the end of the direct-access buffer is returned in ~*datasize~.
| 
| [fig:memmap1] shows the mapping of a piece of
| a big memory block into an internal direct access buffer.
| 
|_figure picture, memmap1, Mapping a Direct-Access Buffer
| 
| It is an error if there are no available unbound direct-access buffers.
| The maximum number of direct-access buffers available is specified 
| at the time the big memory block is allocated.
| This is the ~npages~ argument to dtmem_alloc().
|
| When you are done with the direct-access buffer,
| you should unbind it by using either dtmem_put() or dtmem_unmap().
| Use the first of these two if you have modified the direct-access buffer
| and the second if you have not.
|
| Each time a direct-access buffer is bound,
| the link count for that buffer is incremented.
| Each time it is unbound, the link count is decremented.
| A direct-access buffer is not available for rebinding a different piece
| of big memory unless its link count is 0.
| 
| The binding operation in this function does not copy the
| contents of the big memory into the direct-access buffer.
| For this reason, this function is slightly faster than dtmem_get()
| which does do the copy.
| However, the entire contents of the direct-access buffer is left
| undefined by this function.
| You should only use this function if don't expect
| the big memory to contain any particular value,
| but plan only to store values into it.
| In this case, you would always unbind the direct-access buffer
| with dtmem_put(), which will save your stored values back to external memory.
|
| In general, you don't know ahead of time which direct-access
| buffer you will be using for a given offset.
| If there is only one for the big memory block,
| then of course you always use the same one.
| However, if there are more than one direct-access buffer,
| the algorithm may use any of them.
| It is recommended that you don't assume any particular buffer,
| but always use the ~*ramptr~ returned by this function.
|
| The direct-access buffer is guaranteed to have a size 
| which is an integral multiple of the the unit size which was specified
| at the time the big memory block is allocated.
| This is the ~size~ argument to dtmem_alloc().
| However, the value returned in ~datasize~ by this function
| may be any non-zero value.
| This is because the offset you specify need not be related to the unit size.
| For example, if you have allocated 1 unit of size 100,
| then you request offset 99,
| ~*datasize~ will be returned as 1.
| The entire unit of 100 bytes will be bound into the direct-access buffer,
| but the pointer returned will be to the 99th byte in the buffer,
| leaving only one byte after it.
|
|.Macro Implementation
| A macro implementation of this function call is available.
| The macro implementation simply returns in ~*ramptr~ 
| the pointer to the memory plus ~offset~,
| skipping the extra 8-byte overhead.
| The size which has been recorded in the overhead,
| less the offset, is returned in ~*datasize~.
|
|dtinclude(nomem1.dx)
|
| RETURN VALUES
| The ~*ramptr~ pointer is set to point into a direct-access buffer.
| The number of bytes in the direct-access buffer after
| the pointer is returned in ~*datasize~.
| The contents of the direct-access buffer are not copied from the
| external big memory and remain undefined.
|dtinclude(return1.dx)
| 
| ERRORS
| Unsuccessful completion may be due to:
| -	no available unbound direct-access buffers
| -	offset less than 0 or greater then total allocation
| -	problem writing previously marked direct-access buffer while unbinding it
|	in preparation for rebinding it for the new request
| 
| CAVEATS
| Attempting to use a ~*memalloc~ structure
| which has not been created by dtmem_alloc()
| may lead to undefined or catastrophic results.
|
| Don't try to use more than ~*datasize~ bytes after ~*ramptr~
| or any bytes before it.
|
| Don't forget to unbind the direct-access buffer when you are done with it.
| 
| SEE ALSO
| Introduction		for more on the :mem: function group
| dtmem_get()		bind with copying
| dtmem_put()		unbind with copying
| dtmem_unmap()		unbind
|
| EXAMPLE
| For complete, working example programs, please see the 
| :mem: Function Group Introduction.
| 
|:dtmemalloc_t *memalloc;
|:long offset;
|:unsigned int datasize;
|:
|:dtmem_alloc(NULL, &memalloc,	 		// allocate 10000 bytes
|:  0, 0, 100, 100, NULL, NULL);
|:
|:for (offset = 0;						// loop over whole big memory
|:     offset < 100*100;
|:     offset += datasize)
|:{
|:  char *ramptr;
|:
|:  dtmem_map(NULL, memalloc, offset,	// bind next offset
|:    (void **)&ramptr, &datasize);
|:
|:  DT_MEMSET(ramptr, 0xff, datasize);		// set buffer to all 1's
|:
|:  dtmem_put(NULL, memalloc, offset);	// unbind offset, mark to write
|:}
| 
| END
 * ........................................................................ */

dt_rc_e 
DTCONFIG_API1
dtmem_map(								/* map big memory for direct-access	*/
  dt_ctl_t *ctl,						/* environment control 				*/
  void *memalloc,						/* big memory to operate on			*/
  long offset,							/* desired offset 					*/
  void **ramptr,						/* returned direct-access pointer 	*/
  unsigned int *datasize)				/* bytes available after pointer 	*/
{
  DT_F("dtmem_map");

  DT_Q(((dtmemalloc_t *)memalloc)->map,(ctl,
    (dtmemalloc_t *)memalloc, offset,
    ramptr, datasize));

  return DT_RC_GOOD;
}

/* ........................................................................ *
| NAME
| dtmem_get() - copy piece of big memory to direct-access buffer
| 
| DESCRIPTION
| dtmem_get() binds a piece of big memory to a direct-access buffer
| and copies its contents into the buffer.
| 
| ~*memalloc~ represents a big memory block created by dtmem_alloc().
| The piece of big memory starting at ~offset~ 
| is bound to one of the block's direct-access buffers.
| The contents of the piece of a memory are copied into the direct-access buffer.
| The pointer into the direct-access buffer corresponding to ~offset~
| is returned in ~*ramptr~, and the number of bytes from this point
| to the end of the direct-access buffer is returned in ~*datasize~.
| 
| [fig:memget1] shows the getting of a piece of
| a big memory block into an internal direct access buffer.
| 
|_figure picture, memget1, Getting a Direct-Access Buffer
|
| This function tries to be as smart as possible when deciding
| when and when not to copy big memory contents to the direct-access buffers.
| For example, it will never copy big memory contents to
| a direct-access buffer which is already bound to that offset and size.
| Also, it will not write a direct-access buffer back to external memory
| until that direct-access buffer is needed for binding to a new
| piece of big memory.
| 
| It is an error if there are no available unbound direct-access buffers.
| The maximum number of direct-access buffers available is specified 
| at the time the big memory block is allocated.
| This is the ~npages~ argument to dtmem_alloc().
|
| When you are done with the direct-access buffer,
| you should unbind it by using either dtmem_put() or dtmem_unmap().
| Use the first of these two if you have modified the direct-access buffer
| and the second if you have not.
|
| Each time a direct-access buffer is bound,
| the link count for that buffer is incremented.
| Each time it is unbound, the link count is decremented.
| A direct-access buffer is not available for rebinding a different piece
| of big memory unless its link count is 0.
| 
| The binding operation in this function copies the
| contents of the big memory into the direct-access buffer.
| For this reason, this function is slightly slower than dtmem_put()
| which does not do the copy.
| However, you must use this function if you want to examine
| the contents of big memory, or change some but not all of it.
|
| In general, you don't know ahead of time which direct-access
| buffer you will be using for a given offset.
| If there is only one for the big memory block,
| then of course you always use the same one.
| However, if there are more than one direct-access buffer,
| the algorithm may use any of them.
| It is recommended that you don't assume any particular buffer,
| but always use the ~*ramptr~ returned by this function.
|
| The direct-access buffer is guaranteed to have a size 
| which is an integral multiple of the the unit size which was specified
| at the time the big memory block is allocated.
| This is the ~size~ argument to dtmem_alloc().
| However, the value returned in ~datasize~ by this function
| may be any non-zero value.
| This is because the offset you specify need not be related to the unit size.
| For example, if you have allocated 1 unit of size 100,
| then you request offset 99,
| ~*datasize~ will be returned as 1.
| The entire unit of 100 bytes will be bound into the direct-access buffer,
| but the pointer returned will be to the 99th byte in the buffer,
| leaving only one byte after it.
|
|.Macro Implementation
| A macro implementation of this function call is available.
| The macro implementation simply returns in ~*ramptr~ 
| the pointer to the memory plus ~offset~,
| skipping the extra 8-byte overhead.
| The size which has been recorded in the overhead,
| less the offset, is returned in ~*datasize~.
| The macro never copies memory contents.
|
|dtinclude(nomem1.dx)
|
| RETURN VALUES
| The ~*ramptr~ pointer is set to point into a direct-access buffer.
| The number of bytes in the direct-access buffer after
| the pointer is returned in ~*datasize~.
| The contents of the direct-access buffer are copied from the
| external big memory into the direct-access buffer.
|dtinclude(return1.dx)
| 
| ERRORS
| Unsuccessful completion may be due to:
| -	no available unbound direct-access buffers
| -	offset less than 0 or greater then total allocation
| -	problem writing previously marked direct-access buffer while unbinding it
|	in preparation for rebinding it for the new request
| -	problem copying big memory contents into direct-access buffer
| 
| CAVEATS
| Attempting to use a ~*memalloc~ structure
| which has not been created by dtmem_alloc()
| may lead to undefined or catastrophic results.
|
| Don't try to use more than ~*datasize~ bytes after ~*ramptr~
| or any bytes before it.
|
| Don't forget to unbind the direct-access buffer when you are done with it.
| 
| SEE ALSO
| Introduction		for more on the :mem: function group
| dtmem_map()		bind without copying
| dtmem_put()		unbind and copy to external memory
| dtmem_unmap()		unbind without copying to external memory
|
| EXAMPLE
| For complete, working example programs, please see the 
| :mem: Function Group Introduction.
| 
|:dtmemalloc_t *memalloc;
|:long offset;
|:unsigned int datasize;
|:long sum = 0;
|:
|:dtmem_alloc(NULL, &memalloc,	 		// allocate 10000 bytes
|:  0, 0, 100, 100, NULL, NULL);
|:
|:for (offset = 0;						// loop over whole big memory
|:     offset < 100*100;
|:     offset += datasize)
|:{
|:  char *ramptr;
|:  unsigned int i;
|:
|:  dtmem_get(NULL, memalloc, offset,	// get next offset
|:    (void **)&ramptr, &datasize);
|:
|:  for (i=0; i<datasize; i++)			// sum all data
|:    sum += ramptr[i];
|:
|:  dtmem_unmap(NULL, memalloc, 		// unbind offset, mark unchanged
|:     offset);
|:}
| 
| END
 * ........................................................................ */

dt_rc_e 
DTCONFIG_API1
dtmem_get(								/* get big memory for direct-access	*/
  dt_ctl_t *ctl,						/* environment control 				*/
  void *memalloc,						/* big memory to operate on			*/
  long offset,							/* desired offset 					*/
  void **ramptr,						/* returned direct-access pointer 	*/
  unsigned int *datasize)				/* bytes available after pointer 	*/
{
  DT_F("dtmem_get");

  DT_Q(((dtmemalloc_t *)memalloc)->get,(ctl,
    (dtmemalloc_t *)memalloc, offset,
    ramptr, datasize));

  return DT_RC_GOOD;
}

/* ........................................................................ *
| NAME
| dtmem_unmap() - unbind direct-access buffer without writing
| 
| DESCRIPTION
| dtmem_unmap() unbinds a piece of big memory from a direct-access buffer
| without writing its contents to the external big memory.
| 
| ~*memalloc~ represents a big memory block created by dtmem_alloc().
| The link count of the direct-access buffer containing ~offset~
| is decremented.
| When the link count reaches zero,
| then the direct-access buffer will be available for rebinding.
| The direct-access buffer is not marked as changed,
| therefore the contents of the buffer will not be written
| into the external big memory when the buffer is rebound.
|
| If you have changed the direct-access buffer's contents,
| you should not use this function to unbind it.
| Instead, you should use dtmem_put().
| 
| It is an error if none of the big memory block's direct-access buffers
| contain ~offset~.
|
|.Macro Implementation
| A macro implementation of this function call is available.
| The macro implementation is a no-op.
|
|dtinclude(nomem1.dx)
|
| RETURN VALUES
| The link count of a direct-access buffer maintained in ~*memalloc~
| is decremented.
|dtinclude(return1.dx)
| 
| ERRORS
| Unsuccessful completion may be due to:
| -	no direct-access buffer corresponds to ~offset~
| - link count of the direct-access buffer is already zero
| 
| CAVEATS
| Attempting to use a ~*memalloc~ structure
| which has not been created by dtmem_alloc()
| may lead to undefined or catastrophic results.
| 
| SEE ALSO
| Introduction		for more on the :mem: function group
| dtmem_map()		bind without copying
| dtmem_get()		bind and copy from external memory
| dtmem_put()		unbind and update external memory
|
| EXAMPLE
| For complete, working example programs, please see the 
| :mem: Function Group Introduction.
| 
|:dtmemalloc_t *memalloc;
|:long offset;
|:unsigned int datasize;
|:long sum = 0;
|:
|:dtmem_alloc(NULL, &memalloc,	 		// allocate 10000 bytes
|:  0, 0, 100, 100, NULL, NULL);
|:
|:for (offset = 0;						// loop over whole big memory
|:     offset < 100*100;
|:     offset += datasize)
|:{
|:  char *ramptr;
|:  unsigned int i;
|:
|:  dtmem_get(NULL, memalloc, offset,	// get next offset
|:    (void **)&ramptr, &datasize);
|:
|:  for (i=0; i<datasize; i++)			// sum all data
|:    sum += ramptr[i];
|:
|:  dtmem_unmap(NULL, memalloc, 		// unbind offset, mark unchanged
|:     offset);
|:}
| 
| END
 * ........................................................................ */

dt_rc_e 
DTCONFIG_API1
dtmem_unmap(							/* unmap piece without writing		*/
  dt_ctl_t *ctl,						/* environment control 				*/
  void *memalloc,						/* big memory to operate on 		*/
  long offset)							/* offset to unbind 				*/
{
  DT_F("dtmem_unmap");

  DT_Q(((dtmemalloc_t *)memalloc)->unmap,(ctl,
    (dtmemalloc_t *)memalloc, offset));

  return DT_RC_GOOD;
}

/* ........................................................................ *
| NAME
| dtmem_put() - unbind direct-access buffer and update external memory
| 
| DESCRIPTION
| dtmem_put() unbinds a piece of big memory from a direct-access buffer
| and writes its contents to the external big memory.
| 
| ~*memalloc~ represents a big memory block created by dtmem_alloc().
| The link count of the direct-access buffer containing ~offset~ is decremented.
| When the link count reaches zero,
| then the direct-access buffer will be available for rebinding.
| The direct-access buffer is marked as changed,
| therefore the contents of the buffer will be written
| into the external big memory when the buffer is rebound.
|
| If you are sure you have not changed the direct-access buffer's contents,
| you may optionally call dtmem_unmap().
| It may be slightly more efficient since
| it does not require that the big memory block the written to external
| memory when rebound.
|
| It you are not sure if the direct-access buffer contents have changed
| or not, then this function is safer than dtmem_unmap().
| It won't hurt if the memory is written back even if it is unchanged.
| 
| It is an error if none of the big memory block's direct-access buffers
| contain ~offset~.
|
|.Macro Implementation
| A macro implementation of this function call is available.
| The macro implementation is a no-op.
|
|dtinclude(nomem1.dx)
|
| RETURN VALUES
| The link count of a direct-access buffer maintained in ~*memalloc~
| is decremented.
| The same direct-access buffer is marked for writing to external memory
| when rebound.
|dtinclude(return1.dx)
| 
| ERRORS
| Unsuccessful completion may be due to:
| -	no direct-access buffer corresponds to ~offset~
| - link count of the direct-access buffer is already zero
| 
| CAVEATS
| Attempting to use a ~*memalloc~ structure
| which has not been created by dtmem_alloc()
| may lead to undefined or catastrophic results.
| 
| SEE ALSO
| Introduction		for more on the :mem: function group
| dtmem_map()		bind without copying
| dtmem_get()		bind and copy from external memory
| dtmem_unmap()		unbind without updating external memory
|
| EXAMPLE
| For complete, working example programs, please see the 
| :mem: Function Group Introduction.
| 
|:dtmemalloc_t *memalloc;
|:long offset;
|:unsigned int datasize;
|:long sum = 0;
|:
|:dtmem_alloc(NULL, &memalloc,	 		// allocate 10000 bytes
|:  0, 0, 100, 100, NULL, NULL);
|:
|:for (offset = 0;						// loop over whole big memory
|:     offset < 100*100;
|:     offset += datasize)
|:{
|:  char *ramptr;
|:  unsigned int i;
|:
|:  dtmem_get(NULL, memalloc, offset,	// get next offset
|:    (void **)&ramptr, &datasize);
|:
|:  for (i=0; i<datasize; i+=2)			// zero every other byte
|:    ramptr[i] = 0;
|:
|:  dtmem_put(NULL, memalloc, 			// unbind offset, mark as changed
|:     offset);
|:}
| 
| END
 * ........................................................................ */

dt_rc_e 
DTCONFIG_API1
dtmem_put(								/* unbind piece and update memory	*/
  dt_ctl_t *ctl,						/* environment control 				*/
  void *memalloc,						/* big memory to operate on 		*/
  long offset)							/* offset to unbind 				*/
{
  DT_F("dtmem_put");

  DT_Q(((dtmemalloc_t *)memalloc)->put,(ctl,
    (dtmemalloc_t *)memalloc, offset));

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
