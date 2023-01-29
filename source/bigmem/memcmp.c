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
DT_RCSID("src/low $RCSfile: memcmp.c,v $ $Revision: 1.1 $");
#include <dtack/mem.h>

/* ........................................................................ *
| NAME
| dtmem_cmp() - compare big memory to big memory
| 
| DESCRIPTION
| dtmem_cmp() compares one big memory block's contents to another.
| 
| ~*smemalloc~ and ~*dmemalloc~ represent big memory blocks
| created by dtmem_alloc().
| The big memory ~*smemalloc~ contents
| are compared to the big memory ~*dmemalloc~.
| If ~limit~ is not 0, then only ~limit~ bytes are compared.
| If the memories are not different,
| then ~*cmp~ is returned 0,
| otherwise it contains information 
| about the point where the memories differed.
|
| The memories may be considered as having no difference
| if ~*cmp~ is returned 0.
| If ~limit~ is 0,
| then ~*cmp~ is returned 0 if and only if
| the two memories have the same total allocation
| and no bytes in them differ.
| 
| The other case is where ~limit~ is not 0.
| In this case, no more than ~limit~ bytes are compared.
| ~*cmp~ is returned as 0 if no bytes differ
| both memories are at least as long as ~limit~.
| However, if both memories are shorter than ~limit~,
| but both have the same length and don't differ,
| then ~*cmp~ is also returned 0.
| 
| If not 0, then the value of ~*cmp~ is 1 plus the smaller of
| the offset of the first byte which differed,
| and the size of the smaller of the two total allocations.
| In other words, if some byte differs before the end of either
| memory is reached, then the byte's offset plus 1 is returned.
| If no bytes differ, but one memory is shorter than the other,
| then the length of the shorter memory plus 1 is returned.
|
| This function uses dtmem_get() and dtmem_unmap().
| There must be a direct-access buffer available for binding.
|_aside Uses direct-access buffer.
| There will be no net link count increase or decrease
| caused by this function.
|
| RETURN VALUES
| Bytes are cycled through the direct-access buffers
| of ~*smemalloc~ and ~*dmemalloc~.
|dtinclude(return1.dx)
| 
| ERRORS
| Unsuccessful completion may be due to:
| -	no direct-access buffer available
| - error in dtmem_get() or dtmem_unmap()
| 
| CAVEATS
| Attempting to use a ~*memalloc~ structure
| which has not been created by dtmem_alloc()
| may lead to undefined or catastrophic results.
| 
| SEE ALSO
| Introduction		for more on the :mem: function group
| dtmem_get()		bind direct-access buffer
| dtmem_unmap()		unbind direct-access buffer without updating
|
| EXAMPLE
| For complete, working example programs, please see the 
| :mem: Function Group Introduction.
| 
|:dtmemalloc_t *source;
|:dtmemalloc_t *destination;
|:long cmp;
|:
|:dtmem_alloc(NULL, &source, 			// big allocate 10000 bytes
|:  0, 0, 100, 100, NULL, NULL);
|:
|:dtmem_alloc(NULL, &destination, 		// big allocate 40000 bytes
|:  0, 0, 200, 200, NULL, NULL);
|:
|:dtmem_cmp(NULL, source,				// copy 10000 bytes only
|:  destination, 10000, &cmp);
| 
| END
 * ........................................................................ */

dt_rc_e 
DTCONFIG_API1
dtmem_cmp(								/* compare big memories 			*/
  dt_ctl_t *ctl,						/* environment control 				*/
  void *smemalloc,						/* first memory to compare 			*/
  void *dmemalloc,						/* second memory to compare 		*/
  long limit,							/* how many bytes to compare 		*/
  long *cmp)							/* result of comparison 			*/
{
  DT_F("dtmem_cmp");
# define SMEMALLOC ((dtmemalloc_t *)smemalloc)
# define DMEMALLOC ((dtmemalloc_t *)dmemalloc)
  long soffset = 0;
  long doffset = 0;
  long smapoff = -1;
  long dmapoff = -1;
  unsigned int sdatasize = 0;
  unsigned int ddatasize = 0;
  unsigned int size;
  char *sp;
  char *dp;
  long ilimit = limit;					/* remember original limit 			*/
  unsigned int i;

  if (ilimit == 0)						/* caller wants everything compared */
    ilimit = DT_MAX(
      SMEMALLOC->total, 
      DMEMALLOC->total);

  limit = DT_MIN(ilimit,
    SMEMALLOC->total);
  limit = DT_MIN(limit,
    DMEMALLOC->total);

  do {
    if (sdatasize == 0)
	{
      if (smapoff >= 0)					/* source has been mapped?		    */
        DT_Q((SMEMALLOC->unmap),(ctl,	/* release source memory			*/
          SMEMALLOC, smapoff));
      DT_Q((SMEMALLOC->get),(ctl,		/* get source memory				*/
        SMEMALLOC, soffset, 
        (void **)&sp, &sdatasize));
	  if (soffset+(long)sdatasize > limit)
        sdatasize = (unsigned)
          (limit - soffset);
      smapoff = soffset;
	}
    if (ddatasize == 0)
	{
      if (dmapoff >= 0)					/* destination has been mapped?	    */
        DT_Q((DMEMALLOC->unmap),(ctl,	/* release destination memory		*/
          DMEMALLOC, dmapoff));
      DT_Q((DMEMALLOC->get),(ctl,		/* get destination memory			*/
        DMEMALLOC, doffset, 
        (void **)&dp, &ddatasize));
	  if (doffset+(long)ddatasize > limit)
        ddatasize = (unsigned)
          (limit - doffset);
      dmapoff = doffset;
	}
    size = DT_MIN(sdatasize, ddatasize);
    for (i=0; i<size; i++)				/* compare memory which is mapped	*/
      if (sp[i] != dp[i])				/* found a difference?			    */
        break;
    if (i < size)						/* found a difference?			    */
      break;							/* quit main loop too			    */
    soffset += size;
    doffset += size;
    sp += size;
    dp += size;
    sdatasize -= size;
    ddatasize -= size;
  } while(soffset < limit && 
          doffset < limit);

  if (i < size)							/* some byte differed?			    */
    *cmp = soffset + i + 1;
  else									/* no bytes differed?			    */
  if (SMEMALLOC->total ==				/* sizes are the same?			    */
      DMEMALLOC->total)
    *cmp = 0;							/* memories are the same		    */
  else									/* no bytes differed?			    */
  if (SMEMALLOC->total >= ilimit &&		/* we hit our limit first 			*/
      DMEMALLOC->total >= ilimit)
    *cmp = 0;							/* memories are the same		    */
  else									/* sizes are not the same		    */
    *cmp = limit + 1;

  if (smapoff >= 0)						/* source has been mapped?		    */
    DT_Q((SMEMALLOC->unmap),(ctl,		/* release source memory			*/
      SMEMALLOC, smapoff));
  if (dmapoff >= 0)						/* destination has been mapped?	    */
    DT_Q((DMEMALLOC->unmap),(ctl,		/* release destination memory		*/
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
