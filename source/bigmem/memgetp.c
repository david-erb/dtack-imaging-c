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
DT_RCSID("src/low $RCSfile: memget.c,v $ $Revision: 1.2 $");
#include <dtack/mem.h>

/* ........................................................................ *
| NAME
| dtmem_getpatch() - copy two-dimensional data from big memory
| 
| DESCRIPTION
| dtmem_getpatch() copies a rectangular region from a big memory block
| into a contiguous series of rows in a two-dimensional array.
| 
| ~*memalloc~ represents a big memory block created by dtmem_alloc().
| ~data~ points to a two-dimensional output array of row width ~xe~ bytes
| and height ~ye~ rows.
| The bytes are copied from the big memory starting at byte ~x0~ of unit ~y0~.
|
| For the purposes of this function,
|_Aside Big memory acts two dimensional.
| the big memory block should be considered as two-dimensional.
| The ~size~ argument given to dtmem_alloc() is the "width".
| The ~units~ argument defines the number of "rows".
| [fig:mempatch] illustrates this concept.
|
|_figure picture, mempatch, Big Memory Patch
|
| This function can be considered to copy a small rectangular subwindow
| (the patch) from an area inside a large rectangular window (the big
| memory block).
|
| The rectangular regions are clipped.
|_aside Data is silently clipped.
| Only data in the intersection between the two regions is copied.
| Some data is always clipped if ~x0~ or ~y0~ is less than 0.
| It is possible that no data is transferred,
| for example if ~x0~ is greater than ~size~
| and ~y0~ is greater than ~units~.
| No error or warning is issued if some or all data is clipped.
| The parts of the two-dimensional array ~*data~
| which are clipped out are not changed by this function.
| 
| The destination bytes are copied to ~*data~ in column order.
| This means that the first ~xe~ bytes comprise the first row.
| The second ~xe~ bytes follow immediately after the first row
|_aside Destination data is contiguous.
| in linear memory.
| The total size of the destination memory is ~xe~ times ~ye~ bytes.
|
| This function uses dtmem_get() and dtmem_put().
| There must be a direct-access buffer available for binding.
|_aside Uses direct-access buffer.
| There will be no net link count increase or decrease
| caused by this function.
|
| This function achieves its maximum efficiency when the following
|_aside Congruent copy is fastest.
| two conditions hold:
| 1.	~x0~ is 0
| 2.	~xe~ equals ~size~
|
| RETURN VALUES
| If the rectangular regions overlap,
| bytes are copied from the big memory block ~*memalloc~
| to the two-dimensional array ~*data.
| Bytes are cycled through direct-access buffers.
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
| Only data in the intersection of rectangular regions is copied.
| This could lead to partial or total clipping.
| Clipped parts of the  ~*data~ array remain unchanged.
| 
| SEE ALSO
| Introduction		for more on the :mem: function group
| dtmem_get()		bind direct-access buffer
| dtmem_put()		unbind direct-access buffer
| dtmem_putpatch()	copy rectangular patch to big memory
|
| EXAMPLE
| For complete, working example programs, please see the 
| :mem: Function Group Introduction.
| 
|:dtmemalloc_t *memalloc;
|:void *data;
|:
|:dtos_malloc(NULL, &data, 20*20);		// get some ram
|:
|:dtmem_alloc(NULL, &memalloc,			// big allocate 10000 bytes
|:  0, 0, 100, 100, NULL, NULL);
|:
|:dtmem_getpatch(NULL, memalloc,		// get 20x20 patch from big memory
|:  data, 40, 40, 20, 20);				// 40, 40 is in the middle
| 
| END
 * ........................................................................ */

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
{
  DT_F("dtmem_getpatch");
#define MEMALLOC ((dtmemalloc_t *)memalloc)
  unsigned int y;
  char *cdata = (char *)data;
  long offset;
  unsigned int size;
  char *p;
  unsigned int units;
  unsigned int page;
  unsigned int i = 0;

  DT_Q(dtmem_clip,(ctl,	MEMALLOC,		/* clip rectangles				    */
    data, x0, y0, xe, ye,
    &data, &x0, &y0, &xe, &ye));

  if (xe < 0 || ye < 0)					/* completely clipped out?		    */
    return DT_RC_GOOD;
  
  {
    const unsigned int 
      w = (unsigned int)xe;
    const unsigned int
      ux0 = (unsigned int)x0;			/* for short					    */
    const unsigned int
      uy0 = (unsigned int)y0;
    const unsigned int
      uxe = (unsigned int)xe;
    const unsigned int
      uye = (unsigned int)ye;

  for (y=0; y<uye; y+=i)				/* for whole patch					 */
  {
    offset = (long)(y+uy0) *			/* offset where row starts			*/
      (long)MEMALLOC->size;
    DTMEMSTD_GET(ctl, MEMALLOC, offset,	/* get the memory					*/
      (void **)&p, &size, page);
    units = size / MEMALLOC->size;		/* units we can access				*/
    units = DT_MIN(units, uye-y);		/* limit at end of patch		    */
	p += ux0;							/* offset into each unit			*/
    if (ux0 == 0 &&						/* patch starts at left edge?		*/
		uxe == w &&						/* patch width matches data? 		*/
        MEMALLOC->size == w)			/* memory width matches data? 		*/
	{
	  const unsigned int n = uxe*units;
	  unsigned int j;
	  for (j=0; j<n; j++)				/* copy whole thing at once 		*/
	    cdata[j] = p[j];
      cdata += n;						/* advance output pointer			*/
      i = units;						/* how many units we did 			*/
#     ifdef DTCONFIG_TRIPWIRE			/* supposed to do tripwiring?	    */
	  {
        if (DTMEM_TRIPWIRE)
          ctl->tripwire.mem |= 
            DTMEM_TRIPWIRE_GETPATCH_MATCH;
	  }
#     endif
	}
    else
    for (i=0; i<units; i++)				/* for each unit (row) we mapped	*/
    {
	  unsigned int j;
	  for (j=0; j<uxe; j++)				/* copy a row						*/
	    cdata[j] = p[j];
      cdata += w;						/* advance output pointer			*/
      p += MEMALLOC->size;				/* advance input pointer			*/
    }
    DTMEMSTD_UNMAP(ctl, MEMALLOC, 
      offset, page);
  }

# ifdef DTCONFIG_TRIPWIRE				/* supposed to do tripwiring?	    */
  {
    if (DTMEM_TRIPWIRE && i == uye)		/* all done in one pass?		    */
      ctl->tripwire.mem |= 
        DTMEM_TRIPWIRE_GETPATCH_ONEPASS;
  }
# endif
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
