#ifdef __cplusplus
extern "C" {
#endif
#ifdef DX_PREFIX
#  undef DX_PREFIX
#endif
#ifndef DT_PREFIX
#  define DX_PREFIX
#else
#  define DX_PREFIX DT_PREFIX
#endif
#ifdef DX_SUFFIX
#  undef DX_SUFFIX
#endif
#ifndef DT_SUFFIX
#  define DX_SUFFIX ;
#else
#  define DX_SUFFIX DT_SUFFIX
#endif
#ifdef DT_DEFARGVAL
#  undef DT_DEFARGVAL
#  define DT_DEFARGVAL(A) = (A)
#endif

/*----in memsourc.c:----*/


DX_PREFIX
dt_rc_e 
DTCONFIG_API1
dtmemsource_init(						/* common init for all memsources    */
  dt_ctl_t *ctl,
  dtmemsource_t *memsource,
  const char *name,
  long maxusage,
  unsigned int pagesize,
  unsigned int npages,
  long maxusage_default,
  unsigned int pagesize_default,
  unsigned int npages_default)
DX_SUFFIX

DX_PREFIX
dt_rc_e 
DTCONFIG_API1
dtmemsource_uninit(						/* common uninit for all memsources	*/
  dt_ctl_t *ctl,
  dtmemsource_t *memsource)
DX_SUFFIX

DX_PREFIX
dt_rc_e 
DTCONFIG_API1
dtmemsource_report(		
  dt_ctl_t *ctl,
  dtmemsource_t *memsource,
  const char *f)
DX_SUFFIX

DX_PREFIX
dt_rc_e 
DTCONFIG_API1
dtmemsource_dbg(		
  dt_ctl_t *ctl,
  dtmemsource_t *memsource,
  const char *f,
  const char *desc)
DX_SUFFIX

DX_PREFIX
dt_rc_e 
DTCONFIG_API1
dtmemsource_string(		
  dt_ctl_t *ctl,
  dtmemsource_t *memsource,
  char *string,
  int max)
DX_SUFFIX

/*----in mem.c:----*/


DX_PREFIX
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
DX_SUFFIX

DX_PREFIX
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
DX_SUFFIX

DX_PREFIX
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
DX_SUFFIX

DX_PREFIX
dt_rc_e 
DTCONFIG_API1
dtmem_link(								/* free allocated memory			*/
  dt_ctl_t *ctl,						/* environment control 				*/
  void *vmemalloc)						/* big memory block description 	*/
DX_SUFFIX

DX_PREFIX
dt_rc_e 
DTCONFIG_API1
dtmem_free(								/* free allocated memory			*/
  dt_ctl_t *ctl,						/* environment control 				*/
  void *vmemalloc,						/* big memory block description 	*/
  const char *f,						/* caller name 						*/
  const char *desc)						/* what allocation was for 			*/
DX_SUFFIX

DX_PREFIX
dt_rc_e 
DTCONFIG_API1
dtmem_map(								/* map big memory for direct-access	*/
  dt_ctl_t *ctl,						/* environment control 				*/
  void *memalloc,						/* big memory to operate on			*/
  long offset,							/* desired offset 					*/
  void **ramptr,						/* returned direct-access pointer 	*/
  unsigned int *datasize)				/* bytes available after pointer 	*/
DX_SUFFIX

DX_PREFIX
dt_rc_e 
DTCONFIG_API1
dtmem_get(								/* get big memory for direct-access	*/
  dt_ctl_t *ctl,						/* environment control 				*/
  void *memalloc,						/* big memory to operate on			*/
  long offset,							/* desired offset 					*/
  void **ramptr,						/* returned direct-access pointer 	*/
  unsigned int *datasize)				/* bytes available after pointer 	*/
DX_SUFFIX

DX_PREFIX
dt_rc_e 
DTCONFIG_API1
dtmem_unmap(							/* unmap piece without writing		*/
  dt_ctl_t *ctl,						/* environment control 				*/
  void *memalloc,						/* big memory to operate on 		*/
  long offset)							/* offset to unbind 				*/
DX_SUFFIX

DX_PREFIX
dt_rc_e 
DTCONFIG_API1
dtmem_put(								/* unbind piece and update memory	*/
  dt_ctl_t *ctl,						/* environment control 				*/
  void *memalloc,						/* big memory to operate on 		*/
  long offset)							/* offset to unbind 				*/
DX_SUFFIX

/*----in memclip.c:----*/


DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtmem_clip(
  dt_ctl_t *ctl,
  void *vmemalloc,
  void *data,
  long x0,
  long y0,
  long xe,
  long ye,
  void **cdata,
  long *cx0,
  long *cy0,
  long *cxe,
  long *cye)
DX_SUFFIX

/*----in memputp.c:----*/


DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtmem_putpatch(							/* copy 2D data array to big memory	*/
  dt_ctl_t *ctl,						/* environment control 				*/
  void *memalloc,						/* big memory to operate on 		*/
  void *data,							/* source data 						*/
  long x0,								/* destination byte column 			*/
  long y0,								/* destination unit					*/
  long xe,								/* patch width in bytes				*/
  long ye)								/* patch number of rows 			*/
DX_SUFFIX

/*----in memgetp.c:----*/


DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtmem_getpatch(							/* copy big memory to 2D data array	*/
  dt_ctl_t *ctl,						/* environment control 				*/
  void *memalloc,						/* big memory to operate on 		*/
  void *data,							/* destination data					*/
  long x0,								/* destination byte column 			*/
  long y0,								/* destination unit					*/
  long xe,								/* patch width in bytes				*/
  long ye)								/* patch number of rows 			*/
DX_SUFFIX

/*----in memconf.c:----*/


DX_PREFIX
dt_rc_e 
DTCONFIG_API1
dtmem_configure(						/* configure big memory sources		*/
  dt_ctl_t *ctl,						/* environment control 				*/
  const char *config)					/* memory source specification 		*/
DX_SUFFIX

DX_PREFIX
dt_rc_e 
DTCONFIG_API1
dtmem_unconfigure(						/* unconfigure mem interfaces		*/
  dt_ctl_t *ctl)						/* environment control 				*/
DX_SUFFIX

/*----in memparse.c:----*/


DX_PREFIX
dt_rc_e 
DTCONFIG_API1
dtmem_parse(							/* parse memory configuration		*/
  dt_ctl_t *ctl,
  const char *config,
  char *name,
  int maxlen,
  long *pmaxusage,
  unsigned int *ppagesize,
  unsigned int *pnpages,
  const char **end)
DX_SUFFIX

/*----in memcopy.c:----*/


DX_PREFIX
dt_rc_e 
DTCONFIG_API1
dtmem_copy(								/* copy one big memory to another 	*/
  dt_ctl_t *ctl,						/* environment control 				*/
  void *smemalloc,						/* source big memory 				*/
  void *dmemalloc)						/* destination big memory 			*/
DX_SUFFIX

/*----in memio.c:----*/


DX_PREFIX
dt_rc_e 
DTCONFIG_API1
dtmem_write(							/* write big memory to file			*/
  dt_ctl_t *ctl,						/* environment control				*/
  void *memalloc,						/* big memory to operate on 		*/
  long offset,							/* offset of first byte to write 	*/
  long total,							/* total number of bytes to write 	*/
  dtfd_t *fd)							/* opened file to write to 			*/
DX_SUFFIX

DX_PREFIX
dt_rc_e 
DTCONFIG_API1
dtmem_read(								/* read big memory from file		*/
  dt_ctl_t *ctl,						/* environment control 				*/
  void *memalloc,						/* big memory to operate on 		*/
  long offset,							/* offset into memory to start at   */
  long total,							/* total number of bytes to read    */
  dtfd_t *fd,							/* file opened for reading 			*/
  long *got,							/* actual bytes read from file	    */
  int *eof)								/* return true if end-of-file found	*/
DX_SUFFIX

/*----in memcmp.c:----*/


DX_PREFIX
dt_rc_e 
DTCONFIG_API1
dtmem_cmp(								/* compare big memories 			*/
  dt_ctl_t *ctl,						/* environment control 				*/
  void *smemalloc,						/* first memory to compare 			*/
  void *dmemalloc,						/* second memory to compare 		*/
  long limit,							/* how many bytes to compare 		*/
  long *cmp)							/* result of comparison 			*/
DX_SUFFIX

/*----in memset.c:----*/


DX_PREFIX
dt_rc_e 
DTCONFIG_API1
dtmem_clear(							/* clear big memory to zero			*/
  dt_ctl_t *ctl,
  void *memalloc)
DX_SUFFIX

DX_PREFIX
dt_rc_e 
DTCONFIG_API1
dtmem_set(								/* set all big memory to constant	*/
  dt_ctl_t *ctl,						/* environment control 				*/
  void *memalloc,						/* big memory to operate on 		*/
  const char value)						/* value to set big memory to 		*/
DX_SUFFIX

DX_PREFIX
dt_rc_e 
DTCONFIG_API1
dtmem_set2(								/* set some big memory to constant	*/
  dt_ctl_t *ctl,						/* environment control 				*/
  void *memalloc,						/* big memory to operate on 		*/
  long offset,							/* start point 						*/
  long total,							/* number of bytes to set 			*/
  const char value)						/* value to set big memory to 		*/
DX_SUFFIX

/*----in mem_std.c:----*/


DX_PREFIX
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
DX_SUFFIX

DX_PREFIX
dt_rc_e 
DTCONFIG_API1
dtmemstd_page_free(						/* free page space					*/
  dt_ctl_t *ctl,
  dtmemalloc_t *memalloc,
  const char *f,
  const char *desc)
DX_SUFFIX

DX_PREFIX
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
DX_SUFFIX

DX_PREFIX
dt_rc_e 
DTCONFIG_API1
dtmemstd_flush(							/* flush all unwritten pages		*/
  dt_ctl_t *ctl,
  dtmemalloc_t *memalloc)
DX_SUFFIX

DX_PREFIX
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
DX_SUFFIX

DX_PREFIX
dt_rc_e 
DTCONFIG_API1
dtmemstd_free(							/* free space						*/
  dt_ctl_t *ctl,
  dtmemalloc_t *memalloc,
  const char *f,
  const char *desc)
DX_SUFFIX

DX_PREFIX
dt_rc_e 
DTCONFIG_API1
dtmemstd_map(							/* map space						*/
  dt_ctl_t *ctl,
  dtmemalloc_t *memalloc,
  long offset,		
  void **ramptr,
  unsigned int *datasize)
DX_SUFFIX

DX_PREFIX
dt_rc_e 
DTCONFIG_API1
dtmemstd_unmap(							/* unmap space						*/
  dt_ctl_t *ctl,
  dtmemalloc_t *memalloc,
  long offset)
DX_SUFFIX

DX_PREFIX
dt_rc_e 
DTCONFIG_API1
dtmemstd_get(							/* get space						*/
  dt_ctl_t *ctl,
  dtmemalloc_t *memalloc,
  long offset,
  void **ramptr,
  unsigned int *datasize)
DX_SUFFIX

DX_PREFIX
dt_rc_e 
DTCONFIG_API1
dtmemstd_put(							/* put space						*/
  dt_ctl_t *ctl,
  dtmemalloc_t *memalloc,
  long offset)
DX_SUFFIX

/*----in mem_ram.c:----*/


DX_PREFIX
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
DX_SUFFIX

DX_PREFIX
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
DX_SUFFIX

DX_PREFIX
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
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtmemram_init(							/* init a "ram" memsource		    */
  dt_ctl_t *ctl,
  dtmemsource_t *memsource,
  const char *name,
  long maxusage,
  unsigned int pagesize,
  unsigned int npages)
DX_SUFFIX

/*----in mem_disk.c:----*/


DX_PREFIX
dt_rc_e 
DTCONFIG_API1
dtmemdisk_init(							/* init a "disk" memsource		    */
  dt_ctl_t *ctl,
  dtmemsource_t *memsource,
  const char *name,
  long maxusage,
  unsigned int pagesize,
  unsigned int npages)
DX_SUFFIX

DX_PREFIX
dt_rc_e 
DTCONFIG_API1
dtmemdisk_init(							/* init a "disk" memsource		    */
  dt_ctl_t *ctl,
  dtmemsource_t *memsource,
  const char *name,
  long maxusage,
  unsigned int pagesize,
  unsigned int npages)
DX_SUFFIX

/*----in mem_ems.c:----*/


DX_PREFIX
dt_rc_e 
DTCONFIG_API1
dtmemems_init(							/* init a "ems" memsource		    */
  dt_ctl_t *ctl,
  dtmemsource_t *memsource,
  const char *name,
  long maxusage,
  unsigned int pagesize,
  unsigned int npages)
DX_SUFFIX

DX_PREFIX
dt_rc_e 
DTCONFIG_API1
dtmemems_init(							/* init a "ems" memsource		    */
  dt_ctl_t *ctl,
  dtmemsource_t *memsource,
  const char *name,
  long maxusage,
  unsigned int pagesize,
  unsigned int npages)
DX_SUFFIX

/*----in memxmsif.c:----*/


DX_PREFIX
dt_rc_e 
DTCONFIG_API1
dtmemxmsif_init(						/* init an "xms" memsource		    */
  dt_ctl_t *ctl,
  dtmemsource_t *memsource,
  const char *name,
  long maxusage,
  unsigned int pagesize,
  unsigned int npages)
DX_SUFFIX

DX_PREFIX
dt_rc_e 
DTCONFIG_API1
dtmemxmsif_init(						/* init an "xms" memsource		    */
  dt_ctl_t *ctl,
  dtmemsource_t *memsource,
  const char *name,
  long maxusage,
  unsigned int pagesize,
  unsigned int npages)
DX_SUFFIX

/*----in memxmslb.c:----*/


DX_PREFIX
dt_rc_e 
DTCONFIG_API1
dtmemxmslb_init(						/* init a "xms" memsource		    */
  dt_ctl_t *ctl,
  dtmemsource_t *memsource,
  const char *name,
  long maxusage,
  unsigned int pagesize,
  unsigned int npages)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtmemxmslb_init(						/* init a "xms" memsource		    */
  dt_ctl_t *ctl,
  dtmemsource_t *memsource,
  const char *name,
  long maxusage,
  unsigned int pagesize,
  unsigned int npages)
DX_SUFFIX

/*----in memtxms.c:----*/


DX_PREFIX
dt_rc_e 
DTCONFIG_API1
dtmemtxms_init(							/* init a "xms" memsource		    */
  dt_ctl_t *ctl,
  dtmemsource_t *memsource,
  const char *name,
  long maxusage,
  unsigned int pagesize,
  unsigned int npages)
DX_SUFFIX

DX_PREFIX
dt_rc_e 
DTCONFIG_API1
dtmemtxms_init(							/* init a "xms" memsource		    */
  dt_ctl_t *ctl,
  dtmemsource_t *memsource,
  const char *name,
  long maxusage,
  unsigned int pagesize,
  unsigned int npages)
DX_SUFFIX

/* ---------------------- */
#undef DT_DEFARGVAL
#undef DX_SUFFIX
#undef DX_PREFIX
#define DT_DEFARGVAL(A)
#ifdef __cplusplus
}
#endif
