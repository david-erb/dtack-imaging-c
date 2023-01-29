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
DT_RCSID("src/low $RCSfile: DT_MEMSET.c,v $ $Revision: 1.2 $");
#include <dtack/mem.h>

/* ........................................................................ *
| NAME
| dtmem_clear() - clear big memory contents to 0
| 
| DESCRIPTION
| dtmem_clear() clears entire big memory contents to 0.
| 
| ~*memalloc~ represents a big memory block created by dtmem_alloc().
| The big memory ~*memalloc~ contents are cleared to 0.
|
| This function is equivalent to calling dtmem_set() with value 0.
| To set just part of a big memory to a byte constant,
| use dtmem_set2().
|
| This function uses dtmem_map() and dtmem_put().
| There must be a direct-access buffer available for binding.
|_aside Uses direct-access buffer.
| There will be no net link count increase or decrease
| caused by this function.
|
| RETURN VALUES
| Bytes of value 0 are cycled through the direct-access buffers
| on their way out to ~*memalloc~.
|dtinclude(return1.dx)
| 
| ERRORS
| Unsuccessful completion may be due to:
| -	no direct-access buffer available
| - error in dtmem_map() or dtmem_put()
| 
| CAVEATS
| Attempting to use a ~*memalloc~ structure
| which has not been created by dtmem_alloc()
| may lead to undefined or catastrophic results.
| 
| SEE ALSO
| Introduction		for more on the :mem: function group
| dtmem_map()		bind direct-access buffer without copying
| dtmem_put()		unbind direct-access buffer
| dtmem_set()		set all big memory contents to byte constant
| dtmem_set2()		set some big memory contents to byte constant
|
| EXAMPLE
| For complete, working example programs, please see the 
| :mem: Function Group Introduction.
| 
|:dtmemalloc_t *memalloc;
|:
|:dtmem_alloc(NULL, &memalloc, 			// big allocate 10000 bytes
|:  0, 0, 100, 100, NULL, NULL);
|:
|:dtmem_clear(NULL, memalloc);			// clear big memory
| 
| END
 * ........................................................................ */

dt_rc_e 
DTCONFIG_API1
dtmem_clear(							/* clear big memory to zero			*/
  dt_ctl_t *ctl,
  void *memalloc)
{
  DT_F("dtmem_clear");
#define MEMALLOC ((dtmemalloc_t *)memalloc)
  long offset;
  unsigned int size;

  for (offset=0;
       offset < MEMALLOC->total;
       offset += size)
  {
    void *p;
    DT_Q((MEMALLOC->map),(ctl,			/* bind direct-access buffer 		*/
      MEMALLOC, offset, 
      &p, &size));
    DT_MEMSET(p, 0, size);					/* clear direct-access buffer 		*/
    DT_Q((MEMALLOC->put),(ctl,			/* unbind direct-access buffer 		*/
      MEMALLOC, offset));
  }

# undef MEMALLOC
  return DT_RC_GOOD;
}

/* ........................................................................ *
| NAME
| dtmem_set() - set big memory contents to byte constant
| 
| DESCRIPTION
| dtmem_set() sets entire big memory contents to a byte constant.
| 
| ~*memalloc~ represents a big memory block created by dtmem_alloc().
| The big memory ~*memalloc~ contents are set to the byte constant ~value~.
|
| When ~value~ is 0, this function is equivalent to calling dtmem_clear().
| To set just part of a big memory to a byte constant, use dtmem_set2().
|
| This function uses dtmem_map() and dtmem_put().
| There must be a direct-access buffer available for binding.
|_aside Uses direct-access buffer.
| There will be no net link count increase or decrease
| caused by this function.
|
| RETURN VALUES
| Bytes of value ~value~ are cycled through the direct-access buffers
| on their way out to ~*memalloc~.
|dtinclude(return1.dx)
| 
| ERRORS
| Unsuccessful completion may be due to:
| -	no direct-access buffer available
| - error in dtmem_map() or dtmem_put()
| 
| CAVEATS
| Attempting to use a ~*memalloc~ structure
| which has not been created by dtmem_alloc()
| may lead to undefined or catastrophic results.
| 
| SEE ALSO
| Introduction		for more on the :mem: function group
| dtmem_map()		bind direct-access buffer without copying
| dtmem_put()		unbind direct-access buffer
| dtmem_clear()		clear big memory contents to 0
|
| EXAMPLE
| For complete, working example programs, please see the 
| :mem: Function Group Introduction.
| 
|:dtmemalloc_t *memalloc;
|:
|:dtmem_alloc(NULL, &memalloc, 			// big allocate 10000 bytes
|:  0, 0, 100, 100, NULL, NULL);
|:
|:dtmem_clear(NULL, memalloc);			// clear big memory
| 
| END
 * ........................................................................ */

dt_rc_e 
DTCONFIG_API1
dtmem_set(								/* set all big memory to constant	*/
  dt_ctl_t *ctl,						/* environment control 				*/
  void *memalloc,						/* big memory to operate on 		*/
  const char value)						/* value to set big memory to 		*/
{
  DT_F("dtmem_set");
#define MEMALLOC ((dtmemalloc_t *)memalloc)
  long offset;
  unsigned int size;

  for (offset=0;
       offset < MEMALLOC->total;
       offset += size)
  {
    void *p;
    DT_Q((MEMALLOC->map),(ctl,
      MEMALLOC, offset, 
      &p, &size));
    DT_MEMSET(p, value, size);
    DT_Q((MEMALLOC->put),(ctl,
      MEMALLOC, offset));
  }

# undef MEMALLOC
  return DT_RC_GOOD;
}

/* ........................................................................ *
| NAME
| dtmem_set2() - set some big memory contents to byte constant
| 
| DESCRIPTION
| dtmem_set2() sets some or all big memory contents to a byte constant.
| 
| ~*memalloc~ represents a big memory block created by dtmem_alloc().
| The big memory ~*memalloc~ contents
| starting at ~offset~ and going for ~total~ bytes
| are set to the byte constant ~value~.
|
| If ~total~ is 0, then all bytes starting at ~offset~
| and continuing until the end of the big memory are set.
| If ~total~ when added to ~offset~ would exceed the big memory
| total allocation, no error or warning is issued
| and no bytes beyond the end of the big memory are set.
|
| When ~value~, ~offset~ and ~total~ are all 0,
| this function is equivalent to calling dtmem_clear().
| To most efficiently set all of a big memory to a byte constant, 
| use dtmem_set2().
|
| This function uses dtmem_get() and dtmem_put().
| There must be a direct-access buffer available for binding.
|_aside Uses direct-access buffer.
| There will be no net link count increase or decrease
| caused by this function.
|
| RETURN VALUES
| Bytes of value ~value~ are cycled through the direct-access buffers
| on their way out to ~*memalloc~.
|dtinclude(return1.dx)
| 
| ERRORS
| Unsuccessful completion may be due to:
| -	no direct-access buffer available
| - error in dtmem_map() or dtmem_put()
| 
| CAVEATS
| Attempting to use a ~*memalloc~ structure
| which has not been created by dtmem_alloc()
| may lead to undefined or catastrophic results.
| 
| SEE ALSO
| Introduction		for more on the :mem: function group
| dtmem_get()		bind direct-access buffer
| dtmem_put()		unbind direct-access buffer
| dtmem_clear()		clear big memory contents to 0
|
| EXAMPLE
| For complete, working example programs, please see the 
| :mem: Function Group Introduction.
| 
|:dtmemalloc_t *memalloc;
|:
|:dtmem_alloc(NULL, &memalloc, 			// big allocate 10000 bytes
|:  0, 0, 100, 100, NULL, NULL);
|:
|:dtmem_clear(NULL, memalloc);			// clear big memory
| 
| END
 * ........................................................................ */

dt_rc_e 
DTCONFIG_API1
dtmem_set2(								/* set some big memory to constant	*/
  dt_ctl_t *ctl,						/* environment control 				*/
  void *memalloc,						/* big memory to operate on 		*/
  long offset,							/* start point 						*/
  long total,							/* number of bytes to set 			*/
  const char value)						/* value to set big memory to 		*/
{
  DT_F("dtmem_set2");
# define MEMALLOC ((dtmemalloc_t *)memalloc)
  long offset2;
  unsigned int size;

  if (total == 0)						/* caller wants to fill to the end?	*/
    total = MEMALLOC->total;

  offset2 = DT_MIN(offset+total,		/* don't copy past end			    */
    MEMALLOC->total);

  while (offset < offset2)
  {
    void *p;
    DT_Q((MEMALLOC->get),(ctl,			/* get the source memory		    */
      MEMALLOC, offset, 
      &p, &size));
    if (offset+(long)size > offset2)	/* this page would go too far?	    */
      size = (unsigned int)
        (offset2 - offset);
    DT_MEMSET(p, value, size);
    DT_Q((MEMALLOC->put),(ctl,
      MEMALLOC, offset));
    offset += size;
  }
# undef MEMALLOC
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
