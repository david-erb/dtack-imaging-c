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
DT_RCSID("src/low $RCSfile: memcopy.c,v $ $Revision: 1.1 $");
#include <dtack/mem.h>

/* ........................................................................ *
| NAME
| dtmem_copy() - copy big memory to big memory
| 
| DESCRIPTION
| dtmem_copy() copies one big memory block's contents to another.
| 
| ~*smemalloc~ and ~*dmemalloc~ represent big memory blocks
| created by dtmem_alloc().
| The entire source big memory ~*smemalloc~ contents
| are copied to the destination big memory ~*dmemalloc~.
| 
| If the source total allocation is less than the destination
| total allocation,
| the destination beyond the size of the source stays unchanged.
| If the source is larger than the destination,
| bytes in the source beyond the size of the destination are silently
| ignored.
|
| This function uses dtmem_get() and dtmem_put().
| There must be a direct-access buffer available for binding.
|_aside Uses direct-access buffer.
| There will be no net link count increase or decrease
| caused by this function.
|
| RETURN VALUES
| Bytes are cycled through direct-access buffers
| from ~*smemalloc~ to ~*dmemalloc~.
|dtinclude(return1.dx)
| 
| ERRORS
| Unsuccessful completion may be due to:
| -	no direct-access buffer available
| - error in dtmem_get() or dtmem_put()
| 
| CAVEATS
| Attempting to use a ~*memalloc~ structure
| which has not been created by dtmem_alloc()
| may lead to undefined or catastrophic results.
|
| No error or warning message is issued if the big memory
| total allocations are not the same period
| 
| SEE ALSO
| Introduction		for more on the :mem: function group
| dtmem_get()		bind direct-access buffer
| dtmem_put()		unbind direct-access buffer
|
| EXAMPLE
|:dtmemalloc_t *source;
|:dtmemalloc_t *destination;
|:
|:dtmem_alloc(NULL, &source, 			// big allocate 10000 bytes
|:  0, 0, 100, 100, NULL, NULL);
|:
|:dtmem_alloc(NULL, &destination, 		// big allocate 40000 bytes
|:  0, 0, 200, 200, NULL, NULL);
|:
|:dtmem_copy(NULL, source,				// copy 10000 bytes only
|:  destination);
| 
| END
 * ........................................................................ */

dt_rc_e 
DTCONFIG_API1
dtmem_copy(								/* copy one big memory to another 	*/
  dt_ctl_t *ctl,						/* environment control 				*/
  void *smemalloc,						/* source big memory 				*/
  void *dmemalloc)						/* destination big memory 			*/
{
  DT_F("dtmem_copy");
# define SMEMALLOC ((dtmemalloc_t *)smemalloc)
# define DMEMALLOC ((dtmemalloc_t *)dmemalloc)
  long soffset = 0;
  long doffset = 0;
  long smapoff = -1;
  long dmapoff = -1;
  unsigned int sdatasize = 0;
  unsigned int ddatasize = 0;
  unsigned int size;
  void *sp;
  void *dp;
  long limit = DT_MIN(
    SMEMALLOC->total, DMEMALLOC->total);

  do {
    if (sdatasize == 0)
	{
      if (smapoff >= 0)					/* source has been mapped?		    */
        DT_Q((SMEMALLOC->unmap),(ctl,	/* release source memory			*/
          SMEMALLOC, smapoff));
      DT_Q((SMEMALLOC->get),(ctl,		/* get source memory				*/
        SMEMALLOC, soffset, 
        &sp, &sdatasize));
      smapoff = soffset;
	}
    if (ddatasize == 0)
	{
      if (dmapoff >= 0)					/* destination has been mapped?	    */
        DT_Q((DMEMALLOC->put),(ctl,		/* write destination memory			*/
          DMEMALLOC, dmapoff));
      DT_Q((DMEMALLOC->get),(ctl,		/* map destination memory			*/
        DMEMALLOC, doffset, 
        &dp, &ddatasize));
      dmapoff = doffset;
	}
    size = DT_MIN(sdatasize, ddatasize);
    DT_MEMCPY(dp, sp, size);
    soffset += size;
    doffset += size;
    sp = (void *)((char *)sp + size);
    dp = (void *)((char *)dp + size);
    sdatasize -= size;
    ddatasize -= size;
  } while(soffset < limit && 
          doffset < limit);

  if (smapoff >= 0)						/* source has been mapped?		    */
    DT_Q((SMEMALLOC->unmap),(ctl,		/* release source memory			*/
      SMEMALLOC, smapoff));
  if (dmapoff >= 0)						/* destination has been mapped?	    */
    DT_Q((DMEMALLOC->put),(ctl,			/* write destination memory			*/
      DMEMALLOC, dmapoff));

# undef DMEMALLOC
# undef SMEMALLOC

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
