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
DT_RCSID("source/low $RCSfile: memconf.c,v $ $Revision: 1.1 $");
#include <dtack/mem.h>
#include <dtack/os.h>
#include <dtack/prj.h>

/* ........................................................................ *
| NAME
| dtmem_configure() - configure big memory sources
| 
| DESCRIPTION
| dtmem_configure() initializes an internal table describing
| available big memory sources which will later be used by dtmem_alloc().
|
| The string ~config~ is parsed into its components.
| A small internal table is allocated and its address placed
| at ~ctl->memsource~.
| No big memory allocation is made by this function.
|
| The ~ctl~ structure is used as a vehicle to carry the table 
| to later calls to dtmem_alloc().
|_aside ~ctl~ structure carries the table.
| In order for dtmem_alloc() to be able to access the table,
| the same ~ctl~ structure used here must also be given as the first
| argument to dtmem_alloc().
| If ~ctl~ is itself NULL,
| then there is no place to store the configuration table,
| and this routine does nothing.
|
| When using the :mem: Function Group along with certain other
| Dtack Imaging Software Function Groups,
| you do not need to call this function yourself.
| This function is normally called for you by dt_enter_parse()
| or dt_interactive_enter().
|_index dt_enter(),, memory configuration
|_index dt_interactive_enter(),, memory configuration
| These two functions get the value of ~config~ either from the :-mem:
| command line argument or from the :DT_MEM: environment variable.  
| 
| If ~ctl->memsource~ is not NULL,
|_Aside Only parses string on first call.
| then the memory sources are deemed already configured
| and this routine does nothing.
|
| Only if ~ctl~ is not NULL, and ~ctl->memsource~ is NULL,
| does this function parse the ~config~ string.
| It treats a NULL ~config~ as though it were the string "ram".
|
| This function never looks at the :DT_MEM: environment variable.
| It relies entirely on the ~config~ string given as the argument.
| 
| The format of ~config~ is:
|
|:    spec1+spec2+spec3+spec4+spec5+spec6
|
| Only :spec1: is required.
| All :spec: after the first are optional.
| There should be no spaces or tab characters in the string.
| They are separated by plus (+) signs.
| There may be up to six of them.
| (The six is a compile-time constant.)
| Starting with :spec1: at each new request,
| dtmem_alloc() searches for the first :spec:
| which can satisfy the request.
|
| The format of a :spec: is:
|
|:    name,limit,pagesize,npages
| 
| where:
| :name:		either has a percent (%) sign in it or is one of:
|				:ram:, :xmsif:, :xmslb:, :txms:
| :limit:		maximum number of bytes to use of this memory source
|				before considering it full and trying the next one
| :pagesize:	size of the direct-access buffers
| :npages:		number of direct-access buffers
|
| Only :name: is required.
| If you want the default for a value besides :name:,
| leave it blank, but keep its separating comma.
| See the examples below.
| 
| The various allowed values of :name: mean:
| :ram:			use only malloc() to get memory for the big memory block
| :xmsif:		use shareware library XMSIF version 1.5
|				by James W. Birdsall dated 05/16/93
| :xmslb:		use freeware XMSLB library version 1.7
|				by Michael Graff
| :txms:		use shareware TXMS library version 1.0
|				by TechniLib of Jackson, MS
| :X%dY:		use disk files whose names are made by using sprintf()
|				in which :%d: is replaced by a number,
|				and :X: and :Y: can be zero or 
|				more valid filename characters
|
| The :xms: (Extended Memory) memory source types 
| are available on real-mode DOS only.
| The disk memory source is available on all systems.
| 
| Each memory source can have an assigned :limit:.
| No allocation will be made from a memory source
| if the sum of all previous allocations plus the new one
| would exceed :limit:.
| Note that :limit: generally bears no relation to the actual
| amount actually available in the memory source.
| If an allocation from a memory source fails because there isn't
| enough memory there, even if :limit: has not been reached,
| then dtmem_alloc() quietly tries the next memory source.
|
| The :limit: value should be a small integer,
| optionally followed by one of the four letters k, K, m or M.
| The letters indicate the number is in units of kilobytes or megabytes.
| To use all available bytes in the memory source,
| simply set :limit: to 0, or leave it empty.
| If something follows :limit: in the :spec:,
| be sure to place a comma following it even if it is empty.
| 
| The two values :pagesize: and :npages: define the default and maximum
| form factor of the big memory.
| Remember that big memory is implemented in a piecewise direct fashion.
| This means that you have direct access to the big memory via a normal
| pointer, but only to a piece of it at a time.
| The piece you are accessing must be bound to a direct-access buffer so you
| can use a pointer to it directly.
| 
| :pagesize: defines how big each RAM buffer is,
| and :npages: defines how many RAM buffers there are.
| Basically, a bigger :pagesize: will increase performance
| by decreasing the number of copy operations between the RAM
| buffer and whatever external memory is being used.
| However, making :pagesize: too large will use up valuable RAM,
| defeating the whole purpose of big memory.
|
| The :pagesize:, whether given explicitly or defaulted,
| places a maximum on the memory unit size.
| No memory unit may cross a page boundary.
| Please refer to the dtmem_alloc() function description for more
| information on memory form factor.
| 
| If :pagesize: is not given, or is 0,
| then an efficient value is chosen according to the memory source type.
| If :pagesize: is given and is not 0,
| then it is considered a maximum which the
| ~pagesize~ argument to dtmem_alloc() may not exceed.
|
| The following table lists compile-time defaults for each memory source type:
| #Memory Source#	:pagesize:
| :ram:				one fourth of MALLOC_MAX (16K for real-mode DOS, basically
|					unlimited for 32-bit virtual memory systems)
| :xms:				16K
| disk				16K
| 
| :npages: defines how many RAM buffers of size :pagesize:
| are allocated to provide access to the big memory.
| If you make only sequential passes through the data,
| then you will only need one page.
| However, if it is necessary for your program to access
| different parts of your dataset at the same time,
| then you must have enough buffers to meet the number
| of simultaneous accesses.
|
| If :npages: is not given, or is 0, then a value of 2 is used.
| If :npages: is given and is not 0, then the number
| becomes an upper limit which the ~npages~ argument to
| dtmem_alloc() may not exceed.
|
| The following table lists compile-time defaults for each memory source type:
| #Memory Source#	:npages:
| :ram:				unlimited
| :xms:				2
| disk				2
|
| RETURN VALUES
| Unless already set to something, the ~ctl->memsource~ pointer is assigned
| the address of a dynamically allocated table.
| This table is initialized from the results of parsing ~config~.
|dtinclude(return1.dx)
| 
| ERRORS
| Unsuccessful completion may be due to:
| - unable to allocate space for the internal table
| - invalid ~config~ format
| 
| CAVEATS
| This routine only does work if ~ctl~ is not NULL and
| ~ctl->memsource~ is NULL.
|
| This routine uses malloc() for to get a small table which should be freed
| with dtmem_unconfigure().
| 
| SEE ALSO
| Introduction			for more on the :mem: Function Group
| dtmem_unconfigure()	free space allocated by dtmem_configure()
|
| EXAMPLES
| The following are examples of individual memory :spec: strings:
| 
| :ram:			use ram, no total limit, page size one-fourth of MALLOC_MAX,
|				no limit to the number of pages
| :ram,,4k:		use ram, no total limit, page size 4096 bytes,
|				use as many pages as needed
| :ram,,8k,4:	use ram, no total limit, page size 8192 bytes,
|				use up to 4 pages, allocations of more than 32K bytes always fail
| :ram,1m:		use ram, allocations fail which cause total to go beyond
| 				1 megabyte
| :%d,2m,32k,1:	use disk, files are created with names of the form "1", "2"
|				and so on in the current directory,
| 				allocations fail which cause total to go beyond
|				2 megabytes, use a page size of 32k, but keep one page only
| 
| One or more of the
| individual :spec: strings may be combined with plus (+) signs
| to make a complete memory source specification string.
|dtinclude(memexam1.dx)
|
| Use the default memory source specification string, which is "ram":
| 
|:  dt_ctl_t ctl = {0};
|:  dtmem_configure(&ctl, NULL);  		// use default memory source
|
| Use a hard-coated memory source specification string:
| 
|:  dt_ctl_t ctl = {0};
|:  dtmem_configure(&ctl, "ram+%d");	// configure two memory sources
|
| Use a memory source specification string from the command line:
| 
|:  dt_ctl_t ctl = {0};
|:  dtmem_configure(&ctl, argv[1]);		// get sources from command line
|
| Use a memory source specification string from an environment variable:
| 
|:  dt_ctl_t ctl = {0};
|:  dtmem_configure(&ctl, 				// get sources from environment
|:    getenv("DT_MEM"));
| 
| END
 * ........................................................................ */

dt_rc_e 
DTCONFIG_API1
dtmem_configure(						/* configure big memory sources		*/
  dt_ctl_t *ctl,						/* environment control 				*/
  const char *config)					/* memory source specification 		*/
{
  DT_F("dtmem_configure");
  int i;

  DTCSTACK_HIGHWATER;
  
  if (ctl != NULL &&
	  ctl->memsource == NULL)			/* memsources not yet configured?	*/
  {
    dtmemsource_t *memsource =
      (dtmemsource_t *)ctl->memsource;
    unsigned int n;
    char name[128];
	long maxusage;
    unsigned int pagesize, npages;
    DT_Q(dtos_malloc2,(ctl, 			/* space for all the memsources		*/
      (void **)&memsource,
      n = DT_CTL_MEMSOURCES *
      sizeof(*memsource),
      F, "memsource structures"));
    DT_MEMSET(memsource, 0, n);

    if (config == NULL ||				/* caller is not giving the config?	*/
        strlen(config) == 0)			/* or config string is zero length?	*/
      config = "ram";					/* just use all ram				    */

    dt_dbg(ctl, F, DT_DBG_MASK_PAGE,
      "parsing \"%s\"", config);

    for (i=0; *config; i++)
	{
      if (i == DT_CTL_MEMSOURCES)
        return dt_err(ctl, F,
          "too many entries in mem config, limit is %d",
          DT_CTL_MEMSOURCES);

      DT_Q(dtmem_parse,(ctl, config,	/* parse one config entry		    */
        name, sizeof(name), &maxusage, 
        &pagesize, &npages, &config));
    
      if (strchr(name, '%'))			/* name has a percent in it?	    */
        DT_Q(dtmemdisk_init,(ctl,		/* init the disk source		    	*/
          &memsource[i],
          name, maxusage, 
          pagesize, npages))
      else
      if (!strcmp(name, "ram"))
        DT_Q(dtmemram_init,(ctl,		/* init the memory source		    */
          &memsource[i],
          name, maxusage, 
          pagesize, npages))
      else
      if (!strcmp(name, "ems"))			/* config string name is "ems"?		*/
        if (dtprj_emsif)				/* and we have ems compiled in?	    */
          DT_Q(dtmemems_init,(ctl,		/* init the ems source		    	*/
            &memsource[i],
            name, maxusage, 
            pagesize, npages))
        else							/* no ems compiled in?			    */
          dt_dbg(ctl, 
            F, DT_DBG_MASK_PAGE,
            "ignoring ems in mem config");
      else
      if (!strcmp(name, "xmsif"))		/* config string name is "xmsif"?   */
        if (dtprj_xmsif)				/* and we have xmsif compiled in?   */
          DT_Q(dtmemxmsif_init,(ctl,	/* init the xmsif source	    	*/
            &memsource[i],
            name, maxusage, 
            pagesize, npages))
        else							/* no xms compiled in?			    */
          dt_dbg(ctl, 
            F, DT_DBG_MASK_PAGE,
            "ignoring xmsif in mem config");
      else
      if (!strcmp(name, "xmslb"))		/* config string name is "xmslb"?   */
        if (dtprj_xmslb)				/* and we have xmslb compiled in?	*/
          DT_Q(dtmemxmslb_init,(ctl,	/* init the xmslb source	    	*/
            &memsource[i],
            name, maxusage, 
            pagesize, npages))
        else							/* no xmslb compiled in?		    */
          dt_dbg(ctl, 
            F, DT_DBG_MASK_PAGE,
            "ignoring xmslb in mem config");
      else
      if (!strcmp(name, "txms"))		/* config string name is "txms"?   	*/
        if (dtprj_txms)					/* and we have txms compiled in?	*/
          DT_Q(dtmemtxms_init,(ctl,		/* init the txms source	    		*/
            &memsource[i],
            name, maxusage, 
            pagesize, npages))
        else							/* no txms compiled in?		    	*/
          dt_dbg(ctl, 
            F, DT_DBG_MASK_PAGE,
            "ignoring txms in mem config");
      else
	  {
        dt_rc_e rc = dt_err(ctl, F,
          "invalid memsource name \"%s\"",
          name);
        DT_Q(dtos_free2,(ctl,			/* free space we allocated			*/
          memsource,
          F, "memsource structures"));
        return rc;
	  }
	}

    ctl->memsource = memsource;
  }

  return DT_RC_GOOD;
}


/* ........................................................................ *
| NAME
| dtmem_unconfigure() - unconfigure big memory sources
| 
| DESCRIPTION
| dtmem_configure() frees resources allocated by dtmem_computer().
|
| In particular, the small table describing memory sources
| is freed.
| 
| This function does not affect any outstanding big memory allocations.
| However, new allocations and any reallocations attempted after
| this function will operate as though dtmem_configure()
| had never been called.
|
| This function frees the small internal table whose address is
| at ~ctl->memsource~.
|
| The ~ctl~ structure is used as a vehicle to carry the table 
| from dtmem_configure() to other members of the :mem: Function Group.
|_aside ~ctl~ structure carries the table.
| In order for dtmem_unconfigure() to be able to free the table,
| the same ~ctl~ structure used here must have been the one used
| in the dtmem_configure() call.
| If ~ctl~ is itself NULL,
| or if ~ctl->memsource~ is NULL,
| then this routine does nothing.
|
| When using the :mem: Function Group along with certain other
| Dtack Imaging Software Function Groups,
| you do not need to call this function yourself.
| This function is normally called for you by dt_leave()
| or dt_interactive_leave().
|_index dt_leave(),, memory configuration
|_index dt_interactive_leave(),, memory configuration
|
| RETURN VALUES
| If set to something, then ~ctl->memsource~ table is freed.
|dtinclude(return1.dx)
| 
| ERRORS
| Unsuccessful completion may be due to:
| - unable to free space for the internal table
| 
| CAVEATS
| This routine only does work if ~ctl~ is not NULL and
| ~ctl->memsource~ is not NULL.
| with dtmem_unconfigure().
| 
| SEE ALSO
| Introduction			for more on the :mem: Function Group
| dtmem_configure()		configure memory sources
|
| EXAMPLES
|:  dt_ctl_t ctl = {0};
|:  dtmem_configure(&ctl, NULL);  		// use default memory source
|:  dtmem_unconfigure(&ctl);  			// free configuration table
| 
| END
 * ........................................................................ */

dt_rc_e 
DTCONFIG_API1
dtmem_unconfigure(						/* unconfigure mem interfaces		*/
  dt_ctl_t *ctl)						/* environment control 				*/
{
  DT_F("dtmem_unconfigure");
  dtmemsource_t *memsource = 
    (dtmemsource_t *)(ctl?
      ctl->memsource:
      NULL);

  if (memsource)						/* memsources are initialized?	    */
  {
	int i;
    for (i=0; i<DT_CTL_MEMSOURCES; i++)
    {
      if (memsource[i].name[0] == '\0')	/* this memsource not initialized?  */
	    continue;						/* skip it						    */
      DT_Q((memsource[i].uninit),(ctl,	/* uninit the source			    */
        &memsource[i]));
    }

    DT_Q(dtos_free2,(ctl, memsource,	/* free array of memsource structs  */
      F, "memsource structures"));

    ctl->memsource = NULL;
  }

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
