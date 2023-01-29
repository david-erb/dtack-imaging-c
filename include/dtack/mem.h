/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
  ! Copyright (C) 1998 Dtack Inc. All Rights Reserved
  ! To use this file, you must have signed a license agreement with Dtack Inc.
  ! Under no circumstances may you redistribute this file.
  ! This software is provided AS IS with no warranty expressed or implied.
  ! Dtack Inc. accepts no liability for use or misuse of this file.
  ! http://www.dtack.com  dtack@dtack.com  telephone +360.417.6700
  ! Dtack Inc, P.O. Box 3061, Port Angeles, WA USA 98362
  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */





#define DTMEM_INIT_ARGS     dt_ctl_t *, dtmemsource_t *, \
                              const char *, long, unsigned int, unsigned int
#define DTMEM_UNINIT_ARGS   dt_ctl_t *, struct dtmemsource_t *
#define DTMEM_ALLOC_ARGS    dt_ctl_t *, struct dtmemsource_t *, \
                              void **, \
                              unsigned int pagesize, unsigned int npages, \
                              unsigned int size, unsigned int units, \
                              const char *, const char *
#define DTMEM_REMALLOC_ARGS  dt_ctl_t *, \
                              struct dtmemalloc_t *, \
                              unsigned int size, unsigned int units, \
                              int *gotspace, const char *, const char *
#define DTMEM_MAP_ARGS      dt_ctl_t *, struct dtmemalloc_t *, \
                              long, void **, unsigned int *
#define DTMEM_UNMAP_ARGS    dt_ctl_t *, struct dtmemalloc_t *, long
#define DTMEM_GET_ARGS      dt_ctl_t *, struct dtmemalloc_t *, \
                              long, void **, unsigned int *
#define DTMEM_PUT_ARGS      dt_ctl_t *, struct dtmemalloc_t *, long
#define DTMEM_FREE_ARGS     dt_ctl_t *, struct dtmemalloc_t *, const char *, const char *
#define DTMEM_OPEN_ARGS     dt_ctl_t *, dtmem_t *, const char *, int, long
#define DTMEM_RESIZE_ARGS   dt_ctl_t *, dtmem_t *, long
#define DTMEM_CLOSE_ARGS    dt_ctl_t *, dtmem_t *
#define DTMEM_WRITE_ARGS    dt_ctl_t *, dtmem_t *, long, void *, unsigned int
#define DTMEM_READ_ARGS     dt_ctl_t *, dtmem_t *, long, void *, unsigned int

#define DTMEMRAM_MAXUSAGE_DEFAULT 0		/* use all of it					*/
#define DTMEMRAM_PAGESIZE_DEFAULT 		/* high limit to page size			*/\
  ((unsigned int) \
     (DTCONFIG_MALLOC_MAX/4U))
#define DTMEMRAM_NPAGES_DEFAULT (0)		/* no limit to number of pages		*/

#define DTMEMDISK_MAXUSAGE_DEFAULT 0	/* use all of it					*/
#define DTMEMDISK_PAGESIZE_DEFAULT		/* limit to page size				*/\
  (16384)
#define DTMEMDISK_NPAGES_DEFAULT (2)	/* default pages we can map at once	*/

typedef struct {						/* priv for disks			    	*/
  dtfd_t fd;
  long total;
} dtmemdisk_t;

#define DTMEMEMS_MAXUSAGE_DEFAULT 0		/* use all of it					*/
#define DTMEMEMS_PAGESIZE_DEFAULT		/* limit to page size				*/\
  (16384)
#define DTMEMEMS_NPAGES_DEFAULT (2)		/* default pages we can map at once	*/

#define DTMEMXMS_MAXUSAGE_DEFAULT 0		/* use all of it					*/
#define DTMEMXMS_PAGESIZE_DEFAULT		/* limit to page size				*/\
  (16384)
#define DTMEMXMS_NPAGES_DEFAULT (2)		/* default pages we can map at once	*/

typedef struct {						/* priv for EMS				    */
  int handle;
} dtmemems_t;

typedef struct {						/* priv for XMSIF				    */
  int handle;
} dtmemxmsif_t;

typedef struct {						/* priv for XMSLB				    */
  unsigned int handle;					/* really XMShandle type 			*/
} dtmemxmslb_t;

typedef struct {						/* priv for TXMS				    */
  unsigned int handle;
} dtmemtxms_t;

typedef enum {
  DTMEM_TYPE_MALLOC,
  DTMEM_TYPE_PREALLOC,
  DTMEM_TYPE_DISK,
  DTMEM_TYPE_EMS,
  DTMEM_TYPE_XMSIF,
  DTMEM_TYPE_XMSLB,
  DTMEM_TYPE_TXMS,
  DTMEM_TYPE_UNKNOWN
} dtmem_type_e;

typedef struct {						/* all privs lumped			    */
  dtmem_type_e type;
  char name[128];
  int gotspace;
  long total;
  union {
    dtmemdisk_t disk;
    dtmemems_t ems;
    dtmemxmsif_t xmsif;
    dtmemxmslb_t xmslb;
    dtmemtxms_t txms;
  } u;
} dtmem_t;

typedef struct dtmemsource_t
{
  dt_rc_e (*uninit)(DTMEM_UNINIT_ARGS);	
  dt_rc_e (DTCONFIG_API1DEC *alloc)(DTMEM_ALLOC_ARGS);
  dt_rc_e (*open)(DTMEM_OPEN_ARGS);
  dt_rc_e (*resize)(DTMEM_RESIZE_ARGS);
  dt_rc_e (*close)(DTMEM_CLOSE_ARGS);
  dt_rc_e (*write)(DTMEM_WRITE_ARGS);
  dt_rc_e (*read)(DTMEM_READ_ARGS);
  char name[128];
  unsigned int npages;
  int nallocs;
  int nextalloc;
  long usage;
  long maxusage;
  unsigned long faults;
  unsigned int pagesize;
} dtmemsource_t;

#define DTMEM_FLAG_ALLOCATED (0x01)	/* page has been allocated		    */
#define DTMEM_FLAG_MARKED    (0x02)	/* page has been marked for write   */

typedef struct {
  char *buf;
  unsigned long age;					/* sequential age of the page	    */
  unsigned long flags;
  unsigned int page;					/* page number					    */
  unsigned int links;
  unsigned int datasize;
} dtmem_page_t;

#define DTMEM_PAGE(CTL, MEMALLOC, OFFSET)	/* page number from offset		*/\
  ((unsigned int)((long)(OFFSET)/ \
   ((dtmemalloc_t *)(MEMALLOC))->pagesize))

#define DTMEM_OFFSET(CTL, MEMALLOC, OFFSET)	/* compute offset into page		*/\
  ((unsigned int)((long)(OFFSET) % \
   ((dtmemalloc_t *)(MEMALLOC))->pagesize))

#define DTMEM_ISMAPPED(CTL, MEMALLOC, OFFSET, PAGE)							  \
  (((long)(OFFSET) >= 0 && 													  \
    (long)(OFFSET) < ((dtmemalloc_t *)(MEMALLOC))->total) && 				  \
   ((*(PAGE) = DTMEM_PAGE(CTL, (MEMALLOC), OFFSET)) <						  \
     ((dtmemalloc_t *)(MEMALLOC))->mpages) &&								  \
   ((((dtmemalloc_t *)(MEMALLOC))->mapped == NULL) ||                         \
   (((dtmemalloc_t *)(MEMALLOC))->mapped[*(PAGE)] != DTMEM_NOTMAPPED)))

#define DTMEM_LINK(CTL, MEMALLOC,		/* link page and return it's info   */\
  OFFSET, PAGE,																  \
  RAMPTR, DATASIZE)															  \
{																			  \
  dtmem_page_t *dtmem_tmp_page;			/* structure for a mapped page 		*/\
  const unsigned int dtmem_tmp_offset =	/* offset into page 				*/\
    DTMEM_OFFSET(CTL, MEMALLOC,	    										  \
      OFFSET);	      														  \
  if (((dtmemalloc_t *)(MEMALLOC))->mapped != NULL)							  \
    dtmem_tmp_page =														  \
      &((dtmemalloc_t *)(MEMALLOC))->page[									  \
        ((dtmemalloc_t *)(MEMALLOC))->										  \
          mapped[(PAGE)]];													  \
  else																		  \
    dtmem_tmp_page =														  \
      &((dtmemalloc_t *)(MEMALLOC))->page[(PAGE)];							  \
  dtmem_tmp_page->links++;				/* up link count on page			*/\
  dtmem_tmp_page->age++;				/* give the page an age			    */\
  if ((RAMPTR) != NULL)					/* caller wants pointer back?		*/\
    *(char **)(RAMPTR) =				/* give caller a pointer to use	    */\
       dtmem_tmp_page->buf +												  \
       dtmem_tmp_offset;													  \
  if ((DATASIZE) != NULL)				/* caller wants size back?			*/\
    *(DATASIZE) =															  \
       dtmem_tmp_page->datasize -		/* amount in this buffer		    */\
       dtmem_tmp_offset;				/* less offset into page		    */\
}
										/* ................................ */
#define DTMEMSTD_MAP(CTL,                                                     \
          MEMALLOC, OFFSET,                                                   \
          RAMPTR, DATASIZE, PAGE)                                             \
  DTMEMSTD_MAP2(DT_Q, CTL,                                                    \
    MEMALLOC, OFFSET,                                                         \
    RAMPTR, DATASIZE, PAGE)

										/* ................................ */
#define DTMEMSTD_MAP2(CHECK, CTL,                                             \
          MEMALLOC, OFFSET,                                                   \
          RAMPTR, DATASIZE, PAGE)                                             \
{                                                                        	  \
  if (DTMEM_ISMAPPED(CTL,				/* desired page is mapped?			*/\
        MEMALLOC, OFFSET, &(PAGE)))		                                   	  \
    DTMEM_LINK(CTL,						/* simply link and get info 		*/\
      MEMALLOC, OFFSET,                                                  	  \
      PAGE, RAMPTR, DATASIZE)                                            	  \
  else									/* desired page is not mapped?		*/\
    CHECK(																  	  \
      (MEMALLOC)->map,(CTL,	                								  \
        MEMALLOC, OFFSET,    									  			  \
        (void **)(RAMPTR), DATASIZE))        								  \
}

										/* ................................ */
#define DTMEMSTD_GET(CTL,                                                     \
          MEMALLOC, OFFSET,                                                   \
          RAMPTR, DATASIZE, PAGE)                                             \
  DTMEMSTD_GET2(DT_Q, CTL,                                                    \
    MEMALLOC, OFFSET,                                                         \
    RAMPTR, DATASIZE, PAGE)

										/* ................................ */
#define DTMEMSTD_GET2(CHECK, CTL,                                             \
          MEMALLOC, OFFSET,                                                   \
          RAMPTR, DATASIZE, PAGE)                                             \
{                                                                        	  \
  if (DTMEM_ISMAPPED(CTL,				/* desired page is mapped?			*/\
        MEMALLOC, OFFSET, &(PAGE)))		                                   	  \
    DTMEM_LINK(CTL,						/* simply link and get info 		*/\
      MEMALLOC, OFFSET,                                                  	  \
      PAGE, RAMPTR, DATASIZE)                                            	  \
  else									/* desired page is not mapped?		*/\
    CHECK(																  	  \
      (MEMALLOC)->get,(CTL,	                								  \
        MEMALLOC, OFFSET,    									  			  \
        (void **)(RAMPTR), DATASIZE))        								  \
}
										/* ................................ */

#define DTMEMSTD_UNMAP2(CHECK, CTL, MEMALLOC, OFFSET, PAGE)					  \
{																			  \
  if (!DTMEM_ISMAPPED(CTL, MEMALLOC, OFFSET, &(PAGE)))						  \
    CHECK(dt_err,(CTL, F, 													  \
      "page at %04d:%04d not mapped", 										  \
       (PAGE), DTMEM_OFFSET(CTL, MEMALLOC, OFFSET)));						  \
  if (((dtmemalloc_t *)(MEMALLOC))->mapped != NULL)							  \
    page = ((dtmemalloc_t *)(MEMALLOC))->mapped[(PAGE)];					  \
  if (((dtmemalloc_t *)(MEMALLOC))->page[(PAGE)].links == 0)                  \
    CHECK(dt_err,(CTL, F, 													  \
      "page at %04d:%04d not linked", 										  \
       (PAGE), DTMEM_OFFSET(CTL, MEMALLOC, OFFSET)));						  \
  ((dtmemalloc_t *)(MEMALLOC))->page[(PAGE)].links--;						  \
}
#define DTMEMSTD_PUT2(CHECK, CTL, MEMALLOC, OFFSET, PAGE)					  \
{																			  \
  DTMEMSTD_UNMAP2(CHECK, CTL, MEMALLOC, OFFSET, PAGE)						  \
  ((dtmemalloc_t *)(MEMALLOC))->page[(PAGE)].flags |=						  \
    DTMEM_FLAG_MARKED;														  \
}

#define DTMEMSTD_UNMAP(CTL, MEMALLOC, OFFSET, PAGE)							  \
        DTMEMSTD_UNMAP2(DT_Q, CTL, MEMALLOC, OFFSET, PAGE)
#define DTMEMSTD_PUT(CTL, MEMALLOC, OFFSET, PAGE)							  \
        DTMEMSTD_PUT2(DT_Q, CTL, MEMALLOC, OFFSET, PAGE)

#define DTMEMALLOC_FLAG_FREE      (0x01)  /* free pages when done		  	*/
#define DTMEMALLOC_FLAG_PAGED     (0x02)  /* allocation requires paging		*/
#define DTMEMALLOC_FLAG_SEGMENTED (0x04)  /* not paged, but multiple segs	*/

#define DTMEM_NOTMAPPED					/* value of memalloc->mapped 		*/\
  ((unsigned int)(-1))

typedef struct dtmemalloc_t {
  dtmem_t mem;
  dtmemsource_t *memsource;				/* back to memsource hosting us	    */
  dt_rc_e (DTCONFIG_API1DEC *remalloc)(DTMEM_REMALLOC_ARGS);
  dt_rc_e (DTCONFIG_API1DEC *map)(DTMEM_MAP_ARGS);
  dt_rc_e (DTCONFIG_API1DEC *unmap)(DTMEM_UNMAP_ARGS);
  dt_rc_e (DTCONFIG_API1DEC *get)(DTMEM_GET_ARGS);
  dt_rc_e (DTCONFIG_API1DEC *put)(DTMEM_PUT_ARGS);
  dt_rc_e (DTCONFIG_API1DEC *free)(DTMEM_FREE_ARGS);
  dtmem_page_t *page;
  long total;							/* total in this allocation		    */
  unsigned long age;					/* current sequential age		    */
  unsigned int *mapped;					/* one for each possible page 		*/
  long flags;
  long memalloc_links;					/* number of links to this memalloc */
  unsigned int size;					/* remember allocation parameters   */
  unsigned int units;
  unsigned int req_npages;				/* originally requested values	    */
  unsigned int req_pagesize;
  unsigned int pagesize;				/* actually used values			    */
  unsigned int npages;
  unsigned int mpages;
} dtmemalloc_t;

#ifdef DTCONFIG_TRIPWIRE				/* supposed to do tripwiring?	    */
#  define DTMEM_TRIPWIRE 				/* condition: is tripwire enabled?	*/\
     (ctl != NULL &&\
      ctl->tripwire.flags & DT_TRIPWIRE_MEM)
#  define DTMEM_TRIPWIRE_GETPATCH_MATCH   0x01
#  define DTMEM_TRIPWIRE_GETPATCH_ONEPASS 0x02
#  define DTMEM_TRIPWIRE_PUTPATCH_MATCH   0x04
#  define DTMEM_TRIPWIRE_PUTPATCH_ONEPASS 0x08
#endif
#include <dtack/mem.p>





/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
  ! Copyright (C) 1998 Dtack Inc. All Rights Reserved
  ! To use this file, you must have signed a license agreement with Dtack Inc.
  ! Under no circumstances may you redistribute this file.
  ! This software is provided AS IS with no warranty expressed or implied.
  ! Dtack Inc. accepts no liability for use or misuse of this file.
  ! http://www.dtack.com  dtack@dtack.com  telephone +360.417.6700
  ! Dtack Inc, P.O. Box 3061, Port Angeles, WA USA 98362
  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */
