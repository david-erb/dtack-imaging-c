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
| Function return value will be DT_RC_GOOD on success.  
| Any other return value indicates failure or noncompletion.
| When DT_RC_GOOD is not returned, any other return values may not be valid.
| 
| ERRORS
| As yet undocumented.
| 
| END
 *..........................................................................*/

#ifndef DTT_MAIN						/* not being compiled with a main?	*/
#include <dtack/base.h>
#include <dtack/os.h>
#include <dtack/mem.h>
#include <dtack/stack.h>

DT_RCSID("source/low $RCSfile: stack_io.c,v $ $Revision: 1.1 $");
#endif


# define CEIL(A, B) (unsigned int) \
    ((long)((long)(A) / (long)(B)) * (long)(B))

/* ........................................................................ *
| NAME
| dtstack_write - write stack contents to file 
| 
| DESCRIPTION
| Only writes stack up to its current fullness.
| 
| RETURN VALUES
| Function return value will be DT_RC_GOOD on success.
| Any other return value indicates failure or noncompletion.
| When DT_RC_GOOD is not returned, any other return values may not be valid.
| 
| ERRORS
| As yet undocumented.
| 
| END
 * ........................................................................ */

dt_rc_e 
DTCONFIG_API1
dtstack_write(							/* write stack to file				*/
  dt_ctl_t *ctl,
  dtstack_t *stack,
  dtfd_t *fd)
{
  DT_F("dtstack_write");
  DT_Q(dtmem_write,(ctl,	
    stack->memalloc,
    0,									/* start writing at stack start	    */
    (long)DTSTACK_DEPTH(stack) *		/* write the current stack depth	*/
    (long)stack->elsize,
    fd));
  stack->repage = 1;					/* we have screwed up page mapping 	*/
  return DT_RC_GOOD;
}

/* ........................................................................ *
| NAME
| dtstack_read - read stack contents from file 
| 
| DESCRIPTION
| Expands stack up to ~maximum_depth~ or end of file or until stack overflow.
| Values from file are pushed onto stack on top of whatever is there.
| 
| RETURN VALUES
| Function return value will be DT_RC_GOOD on success.
| Any other return value indicates failure or noncompletion.
| When DT_RC_GOOD is not returned, any other return values may not be valid.
|
| BUGS
| Reads one el too far when stack overflows.
| 
| ERRORS
| As yet undocumented.
| 
| END
 * ........................................................................ */

dt_rc_e 
DTCONFIG_API1
dtstack_read(							/* read stack from file				*/
  dt_ctl_t *ctl,
  dtstack_t *stack,
  long maximum_depth,					/* depth limit for this read	    */
  dtfd_t *fd,
  dtstack_code_e *code,
  int *peof)
{
  DT_F("dtstack_read");
  void *elnew = NULL;
  long offset_max = maximum_depth *		/* caller's limit for this call 	*/
    (long)stack->elsize;
  long offset =							/* current stack byte position	    */
    (long)DTSTACK_DEPTH(stack) *
    (long)stack->elsize;
  char elnew1[64];						/* a small stack element			*/
  int eof = 0;
  dt_rc_e rc;

  if (offset_max == 0)
    offset_max = stack->offset_max;
  else
  if (stack->offset_max)
    offset_max = DT_MIN(offset_max,
      stack->offset_max);

  if (stack->elsize <= sizeof(elnew1))	/* can we use the dynamic one? 		*/
    elnew = elnew1;
  else
    DT_Q(dtos_malloc2,(ctl,				/* space for single stack element	*/
      &elnew, stack->elsize,
      F, "stack element"));

  *code = DTSTACK_CODE_OK;
  while(1)
  {
    long got;
    void *el;
    long sofar;
    if (offset_max &&					/* caller is setting a limit? 		*/
         offset >= offset_max)			/* and we are at that limit?	    */
      break;
    for (sofar = 0;
         sofar < (long)stack->elsize;
         sofar += got)
	{
      DT_C(dtfd_read,(ctl, fd,			/* read next el from file		    */
        elnew, stack->elsize, &got));
      if (got == 0)						/* not enough left in file?		    */
        break;
	}
    if (got == 0)						/* no new stack elements in file?   */
	{
	  eof = 1;
      break;
	}

    DT_C(dtstack_push,(ctl, stack,		/* push to stretch out the stack    */
      &el, code));
    if (*code != DTSTACK_CODE_OK)		/* stack would not be pushed?	    */
      break;

    DT_MEMCPY(el, elnew, stack->elsize);	/* copy the new el into the stack	*/
    offset += stack->elsize;

    DT_C(dtmem_read,(ctl,				/* read the memalloc from file	    */
      stack->memalloc,
      offset,
      offset_max? offset_max-offset: 0,
      fd,
      &got,
      &eof));
    offset += got;

	DT_Q(dtstack_set,(ctl, stack,		/* set new stack operating position */
      DTSTACK_SET_FLAG_POSITION,
      offset));

    if (eof)							/* no more data to read?		    */
      break;
  }
  if (peof)
    *peof = eof;

cleanup:

  if (elnew && elnew != elnew1)
    DT_I(dtos_free2,(ctl, elnew,
      F, "stack element"));

  return rc;
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
