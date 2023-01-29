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
DT_RCSID("src/low $RCSfile: memio.c,v $ $Revision: 1.1 $");
#include <dtack/mem.h>

/* ........................................................................ *
| NAME
| dtmem_write() - write big memory to file
| 
| DESCRIPTION
| dtmem_write() writes all or part of a big memory block to an
| opened file.
| 
| ~*memalloc~ represents a big memory block created by dtmem_alloc().
| Data is written starting at ~offset~ in the big memory block.
| The total number of bytes written to the file is ~total~.
| If ~total~ is either 0 or when added to ~offset~
| would be greater than the current size of ~memalloc~,
| then all data starting at ~offset~ is written.
| ~*fd~ represents an opened file, as returned by :dtfd_open(filename, "w"):.
|_aside You provide the open file.
| ~*fd~ is not closed by this function.
|
| No file header is written by this function, just the beef.
| 
| This function uses dtmem_get() and dtmem_unmap().
| There must be a direct-access buffer available for binding.
|_aside Uses direct-access buffer.
| There will be no net link count increase or decrease
| caused by this function.
|
| RETURN VALUES
| The file pointed to by ~*fd~ will have bytes written to it.
| Bytes are cycled through direct-access buffers.
|dtinclude(return1.dx)
| 
| ERRORS
| Unsuccessful completion may be due to:
| -	no direct-access buffer available
| - error in dtmem_get()
| -	error writing file
| 
| CAVEATS
| Attempting to use a ~*memalloc~ structure
| which has not been created by dtmem_alloc()
| may lead to undefined or catastrophic results.
| 
| SEE ALSO
| Introduction		for more on the :mem: function group
| dtmem_read()		read big memory from file
| dtmem_get()		bind direct-access buffer
|
| EXAMPLE
| For complete, working example programs, please see the 
| :mem: Function Group Introduction.
|
|:dtfd_t fd;
|:dtmemalloc_t *memalloc;
|:
|:dtfd_open(NULL, &fd, "myfile", "w");	// open file for writing 
|_aside You open the file.
|:
|:dtmem_alloc(NULL, &memalloc, 			// big allocate 10000 bytes
|:  0, 0, 100, 100, NULL, NULL);
|:
|:dtmem_write(NULL, memalloc, 0,		// write entire big memory
|:  0, &fd);
| 
| END
 * ........................................................................ */

dt_rc_e 
DTCONFIG_API1
dtmem_write(							/* write big memory to file			*/
  dt_ctl_t *ctl,						/* environment control				*/
  void *memalloc,						/* big memory to operate on 		*/
  long offset,							/* offset of first byte to write 	*/
  long total,							/* total number of bytes to write 	*/
  dtfd_t *fd)							/* opened file to write to 			*/
{
  DT_F("dtmem_write");
#define MEMALLOC ((dtmemalloc_t *)memalloc)
  long offset2;
  unsigned int size;

  if (total == 0)						/* caller wants to write to end?	*/
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
    DT_Q(dtfd_write_all,(ctl, fd,		/* write page to file			    */
      p, size));
    DT_Q((MEMALLOC->unmap),(ctl,		/* release page 					*/
      MEMALLOC, offset));
    offset += size;
  }

  return DT_RC_GOOD;
# undef MEMALLOC
}

/* ........................................................................ *
| NAME
| dtmem_read() - read big memory tfrom file
| 
| DESCRIPTION
| dtmem_read() reads all or part of a big memory block t from an
| opened file.
| 
| ~*memalloc~ represents a big memory block created by dtmem_alloc().
| Data is read into the big memory block starting at ~offset~.
| The total number of bytes read from the file will not exceed ~total~.
| If ~total~ is either 0 or when added to ~offset~
| would be greater than the current size of ~memalloc~,
| then the rest of the big memory block starting at ~offset~
| may be stored to unless end-of-file is reached first.
| ~*fd~ represents an opened file, as returned by :dtfd_open(filename, "r"):.
|_aside You provide the open file.
| ~*fd~ is not closed by this function.
| The actual number of bytes read from the file is returned in ~*got~
| unless ~got~ is NULL.
| If ~eof~ is not NULL,
| then ~*eof~ is returned as 1
| if the end of the input file was encountered during reading,
| and 0 otherwise.
|
| No error message is returned if end-of-file is detected during reading.
| 
| This function uses dtmem_get() and dtmem_put().
| There must be a direct-access buffer available for binding.
|_aside Uses direct-access buffer.
| There will be no net link count increase or decrease
| caused by this function.
|
| RETURN VALUES
| The file pointed to by ~*fd~ will have bytes read from it.
| Bytes are cycled through direct-access buffers
| and stored in the big memory block.
|dtinclude(return1.dx)
| 
| ERRORS
| Unsuccessful completion may be due to:
| -	no direct-access buffer available
| - error in dtmem_get() or dtmem_put()
| -	error reading file
| 
| CAVEATS
| Attempting to use a ~*memalloc~ structure
| which has not been created by dtmem_alloc()
| may lead to undefined or catastrophic results.
|
| No error message is returned if end-of-file is detected during reading.
| 
| SEE ALSO
| Introduction		for more on the :mem: function group
| dtmem_write()		write big memory to file
| dtmem_put()		unbind and direct-access buffer
|
| EXAMPLE
| For complete, working example programs, please see the 
| :mem: Function Group Introduction.
|
|:dtfd_t fd;
|:dtmemalloc_t *memalloc;
|:int eof;
|:long got;
|:
|:dtfd_open(NULL, &fd, "myfile", "w");	// open file for writing 
|_aside You open the file.
|:
|:dtmem_alloc(NULL, &memalloc, 			// big allocate 10000 bytes
|:  0, 0, 100, 100, NULL, NULL);
|:
|:dtmem_read(NULL, memalloc, 0,			// read entire big memory
|:  0, &fd, &eof, &got, &eof);
| 
| END
 * ........................................................................ */

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
{
  DT_F("dtmem_read");
#define MEMALLOC ((dtmemalloc_t *)memalloc)
  long offset2;
  unsigned int size;
  long lgot;

  if (got != NULL)						/* caller wants this back?		    */
    *got = 0;

  if (total == 0)						/* caller wants to read to end?		*/
    total = MEMALLOC->total;

  offset2 = DT_MIN(offset+total,		/* don't read past end			    */
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
    DT_Q(dtfd_read,(ctl, fd,			/* read page from file			    */
      p, size, &lgot));
    DT_Q((MEMALLOC->put),(ctl,			/* save page 						*/
      MEMALLOC, offset));
    if (lgot == 0)						/* no more in file?				    */
      break;
    offset += lgot;
    if (got != NULL)					/* caller wants to know this?	    */
      *got += lgot;						/* keep track of total we have read */
  }

  if (eof != NULL)						/* caller wants to know if eof hit? */
    *eof = !lgot;

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
