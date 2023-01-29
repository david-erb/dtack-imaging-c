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
| TITLE
| The :stack: Function Group
|
|!The :stack: Function Group
|_index stack Function Group, :stack: Function Group
|_index Function Group,, :stack:
|_index big memory,, use of, stack
| The :stack: function group provides functions for
|_aside Memory that grows.
| sequential access to big memory.
| Paging between RAM and external memory, such as XMS or disk,
| happens automatically.
| The memory is grown automatically as well.
|
| The :stack: function group has the following operations:
| dtstack_init()		initialize structure and allocate resources
| dtstack_init2()		initialize structure and allocate resources
| dtstack_prealloc()	wrap stack structure around existing big memory
| dtstack_free()		free resources
| dtstack_set()			relocate stack position pointer
| dtstack_push()		increment stack position pointer
| dtstack_pop()			decrement stack position pointer
| dtstack_peek()		examine stack element non-sequentially
| dtstack_poke()		change stack element non-sequentially
| dtstack_read()		read file into stack
| dtstack_write()		write entire stack to file
| 
| END
 *..........................................................................*/

#include <dtack/base.h>
DT_RCSID("source/low $RCSfile: stack.c,v $ $Revision: 1.4 $");
#include <dtack/os.h>
#include <dtack/mem.h>
#include <dtack/stack.h>


#ifdef PRINT
#  define DTSTACK_PRINT(CTL, STACK, FUNC) dtstack_print(CTL, STACK, FUNC)
#else
#  define DTSTACK_PRINT(CTL, STACK, FUNC)
#endif

# define CEIL(A, B) (unsigned int) \
    ((long)((long)(A) / (long)(B)) * (long)(B))

# define LCEIL(A, B) \
    ((long)((long)(A) / (long)(B)) * (long)(B))

/*__________________________________________________________________________
 * init stack
 * we will only ever map 1 page at a time
 * attempts to allocate memory in units of around 16K
 * will allocate in units of initial_depth if that is less
 * actual stack limit may be higher than maximum depth because of 16K roundup
 *__________________________________________________________________________*/

dt_rc_e
DTCONFIG_API1
dtstack_init2(
  dt_ctl_t *ctl,
  dtstack_t *stack,
  unsigned int elsize,
  long initial_depth,
  long maximum_depth,
  unsigned int pagesize,
  unsigned int npages,
  char *F)
{
  long total;
  if (F == NULL)
    F = "dtstack_init2";

  DT_MEMSET(stack, 0, sizeof(*stack));		/* clear structure				    */
  
  if (ctl != NULL)
  {
    dtmemsource_t *memsource;
    DT_Q(dtmem_configure,(ctl,			/* make sure memory is configured	*/
      ctl->mem));
    memsource =							/* access list of memsources 		*/
      (dtmemsource_t *)ctl->memsource;
    stack->a_size_cur =					/* use first memsource as model 	*/
      memsource[0].pagesize;
  }
  else									/* running bare? 					*/
    stack->a_size_cur =					/* choose some good size 			*/
      DTMEMRAM_PAGESIZE_DEFAULT;
	  
  if (stack->a_size_cur == 0 ||			/* memsource may have no limit 		*/
	  stack->a_size_cur > 16384)		/* memsource may have high limit 	*/
  	stack->a_size_cur = 16384;
  stack->a_size_cur = CEIL(				/* size must be integral elsize 	*/
    stack->a_size_cur, elsize);

  if (initial_depth == 0)				/* caller is not giving depth?	    */
    initial_depth =						/* choose a good size for paging    */
      stack->a_size_cur / elsize;

  if (maximum_depth &&					/* caller is limiting depth? 		*/
	  initial_depth > maximum_depth)
    initial_depth = maximum_depth;

  stack->a_size_max =					/* never increase size				*/
    stack->a_size_cur;		

  total = initial_depth * elsize;
  stack->a_units_cur = (unsigned int)	/* units we need for initial depth 	*/
    (total / stack->a_size_cur);
  if ((long)stack->a_units_cur *		/* partial unit left over? 			*/
      (long)stack->a_size_cur < total)
    stack->a_units_cur++;				/* add an extra unit 				*/
  stack->a_units_max = 65535U;			/* units can go to the 16-bit sky 	*/
  stack->a_units_inc = 					/* realloc first by doubling size	*/
    stack->a_units_cur;

  stack->elsize = elsize;
  stack->offset_max = maximum_depth *
    elsize;

  DT_Q(dtmem_alloc,(ctl,				/* get first space for the stack    */
    &stack->memalloc,
    pagesize, npages,
    stack->a_size_cur,
    stack->a_units_cur,
    F, "stack"));

  stack->offset_cur =					/* size of stack in bytes		    */ 
    ((dtmemalloc_t *)
      stack->memalloc)->total;

  if (stack->offset_max)
  stack->offset_cur = DT_MIN(			/* arbitrary limit set by caller	*/
    stack->offset_cur,
    stack->offset_max);

  stack->datasize = ((dtmemalloc_t *)	/* how much we get contiguously	    */
    stack->memalloc)->pagesize;

  stack->flags |= DTSTACK_FLAG_FREE;	/* mark we can free this memory	    */

  DTSTACK_PRINT(ctl, stack, "init");

  return DT_RC_GOOD;
}

/*__________________________________________________________________________
 * init stack
 * we will only ever map 1 page at a time
 * attempts to allocate memory in units of around 16K
 * will allocate in units of initial_depth if that is less
 * actual stack limit may be higher than maximum depth because of 16K roundup
 *__________________________________________________________________________*/

dt_rc_e
DTCONFIG_API1
dtstack_init(
  dt_ctl_t *ctl,
  dtstack_t *stack,
  unsigned int elsize,
  long initial_depth,
  long maximum_depth,
  char *F)
{
  if (F == NULL)
    F = "dtstack_init";
  DT_Q(dtstack_init2,(ctl,
    stack, elsize,
    initial_depth, maximum_depth, 
    0, 0, F));
  return DT_RC_GOOD;
}

/*__________________________________________________________________________
 * make stack on preallocated space
 * memalloc->size must be integral multiple of elsize
 *__________________________________________________________________________*/

dt_rc_e
DTCONFIG_API1
dtstack_prealloc(
  dt_ctl_t *ctl,
  dtstack_t *stack,
  unsigned int elsize,
  void *vmemalloc,
  long maximum_depth,
  char *F)
{
  DT_MEMSET(stack, 0, sizeof(*stack));		/* clear structure				    */

  stack->memalloc = vmemalloc;

  stack->a_units_cur = ((dtmemalloc_t *)
    stack->memalloc)->units;
  stack->a_size_cur = ((dtmemalloc_t *)
    stack->memalloc)->size;

  stack->a_units_inc =					/* increase by initial size		    */
    stack->a_units_cur;
  stack->a_size_inc = 
    stack->a_size_cur;
  stack->a_units_max = CEIL(65535u, 
    stack->a_units_inc);
  stack->a_size_max = CEIL(65535u,
    stack->a_size_inc);

  stack->elsize = elsize;
  stack->offset_max = maximum_depth *
    elsize;

  stack->offset_cur =					/* size of stack in bytes		    */ 
    ((dtmemalloc_t *)
      stack->memalloc)->total;

  if (stack->offset_max)
  stack->offset_cur = DT_MIN(			/* arbitrary limit set by caller	*/
    stack->offset_cur,
    stack->offset_max);

  stack->datasize = ((dtmemalloc_t *)	/* how much we get contiguously	    */
    stack->memalloc)->pagesize;

  DTSTACK_PRINT(ctl, stack, "prealloc");

  return DT_RC_GOOD;
}

/* ........................................................................ */

dt_rc_e
DTCONFIG_API1
dtstack_push(
  dt_ctl_t *ctl,
  dtstack_t *stack,
  void **el,
  dtstack_code_e *code)
{
  DT_F("dtstack_push");
  int repage = 0;
  long offset = stack->offset +			/* absolute byte position		    */
     (stack->el - stack->elmin);

  if (stack->offset_max &&				/* stack has a limit?			    */
	  offset >= stack->offset_max)		/* and we have reached that?	    */
  {
	if (code)
      *code = DTSTACK_CODE_OVERFLOW; 	/* overflow							*/
	return DT_RC_GOOD;
  }	

  if (offset == stack->offset_cur)		/* stack is full?				    */
  {
    if (stack->a_units_cur ==			/* can't increase units?		    */
        stack->a_units_max)
      if (stack->a_size_cur ==			/* can't increase size?			    */
          stack->a_size_max)
	  {
		if (code)
		  *code =						/* overflow							*/
            DTSTACK_CODE_OVERFLOW;
    	return DT_RC_GOOD;
	  }
      else								/* can increase size?			    */
	  {
        stack->a_size_cur =
		  (unsigned int)
          DT_MIN(
            (long)stack->a_size_cur +
            (long)stack->a_size_inc,
            (long)stack->a_size_max);
        stack->a_size_inc *= 2;			/* exponential size increase	    */
	  }
    else								/* can increase units?			    */
	{
      stack->a_units_cur =
		  (unsigned int)
        DT_MIN(
          (long)stack->a_units_cur +
          (long)stack->a_units_inc,
          (long)stack->a_units_max);
      stack->a_units_inc *= 2;			/* exponential units increase	    */
	  stack->a_units_inc = DT_MIN(		/* plateau at about 128K per malloc	*/
        stack->a_units_inc, 8);
	}
       
    DT_Q(dtmem_realloc,(ctl,			/* reallocate at the new size	    */
      stack->memalloc, 
      0, 0,								/* use originally specified sizes 	*/
      stack->a_size_cur,
      stack->a_units_cur,
      F, "stack realloc"));

    stack->datasize = ((dtmemalloc_t *)	/* how much we get contiguously	    */
      stack->memalloc)->pagesize;
    stack->offset = LCEIL(offset,		/* offset is aligned at datasize    */
      stack->datasize);

    stack->offset_cur =					/* size of stack in bytes		    */ 
      ((dtmemalloc_t *)
        stack->memalloc)->total;

    if (stack->offset_max)
    stack->offset_cur = DT_MIN(			/* arbitrary limit set by caller	*/
      stack->offset_cur,
      stack->offset_max);

    repage = 1;
  }
  else									/* stack is not full?			    */
  if (stack->elmax == NULL)
    repage = 2;
  else									/* not first time in?			    */
  if (stack->el == stack->elmax)		/* page is full?					*/
  {
    stack->offset += stack->datasize;	/* offset that starts next page		*/
    repage = 3;
  }
  else
  if (stack->repage)   					/* we were told to repage?			*/
    repage = 4;

  if (repage)							/* we repaged?					    */
  {
    unsigned int datasize;
    DT_Q(dtmem_get,(ctl,				/* access the new page				*/
      stack->memalloc, stack->offset,
      (void **)&stack->elmin, 
      &datasize));
    DT_Q(dtmem_put,(ctl,				/* mark it for saving				*/
      stack->memalloc, stack->offset));

    if (stack->offset_max)				/* there is a caller limit?		    */
    if (stack->offset+(long)datasize >		/* they gave us too much?		    */
        stack->offset_max)
      datasize = (unsigned int)
        (stack->offset_max -
         stack->offset);

    stack->el = stack->elmin +			/* point into mem mapped at offset	*/
      (unsigned int)
      (offset - stack->offset);
    stack->elmax = stack->elmin +
      datasize;
  }

#ifdef PRINT
  if (repage == 1)
    DTSTACK_PRINT(ctl, stack, "incr");
  else
  if (repage == 3)
    DTSTACK_PRINT(ctl, stack, "page");
#endif

  DTSTACK_PUSHEL(stack, el);			/* return mapped pointer		    */

  stack->repage = 0;   					/* no longer need repaging			*/

  return DT_RC_GOOD;
}
  

/* ........................................................................ */

dt_rc_e
DTCONFIG_API1
dtstack_pop(
  dt_ctl_t *ctl,
  dtstack_t *stack,
  void **el,
  dtstack_code_e *code)
{
  DT_F("dtstack_pop");

  if (stack->el == stack->elmin ||		/* at bottom of page?				*/
      stack->repage)   					/* we were told to repage?			*/
  {
    if (stack->offset == 0 &&			/* at bottom of extent?				*/
        stack->el == stack->elmin)		/* at bottom of page?				*/
	{
      if (code)
        *code = DTSTACK_CODE_UNDERFLOW; /* underflow						*/
      return DT_RC_GOOD;
	}	 
	else								/* still in same of extent?		    */
	{
      unsigned int offset;				/* current page offset			    */
      if (stack->el == stack->elmin)	/* we are repaging because of pop?	*/
	  {
        stack->offset -=				/* offset start of previous page	*/
          stack->datasize;
        offset = stack->datasize;
	  }
      else								/* we were told to repage?			*/
        offset =
          (unsigned int)(stack->el -
            stack->elmin);
      DT_Q(dtmem_get,(ctl, 
        stack->memalloc,				/* get previous page				*/
        stack->offset,
        (void **)&stack->elmin,
        NULL));
      DT_Q(dtmem_put,(ctl,				/* release when required		  	*/
        stack->memalloc, 
        stack->offset));
      stack->el = stack->elmin +
        offset;
      stack->elmax = stack->elmin +
        stack->datasize;
	}
  }

  DTSTACK_POPEL(stack, el);				/* return mapped pointer		    */

  stack->repage = 0;   					/* no longer need repaging			*/

  return DT_RC_GOOD;
}

/* ........................................................................ 
 * return pointer to el with given el position in stack
 * pointer is only guaranteed good until next push/pop/peek/poke call
 * ........................................................................ */

dt_rc_e
DTCONFIG_API1
dtstack_peek(
  dt_ctl_t *ctl,
  dtstack_t *stack,
  long pos,								/* stack position id			    */
  void **el)
{
  DT_F("dtstack_peek");
  pos *= stack->elsize;					/* change to byte position			*/

  DT_Q(dtmem_get,(ctl,					/* copy page into ram			    */
    stack->memalloc,
    pos, el, NULL));
    
  DT_Q(dtmem_unmap,(ctl,				/* free this page on next demand    */
    stack->memalloc, pos));

  stack->repage = 1;  					/* force a repage at push or pop    */

  return DT_RC_GOOD;
}

/* ........................................................................ 
 * return pointer to el with given el position in stack
 * page already marked as changed by this routine
 * pointer is only guaranteed good until next push/pop/peek/poke call
 * ........................................................................ */

dt_rc_e
DTCONFIG_API1
dtstack_poke(
  dt_ctl_t *ctl,
  dtstack_t *stack,
  long pos,								/* stack position id			    */
  void **el)
{
  DT_F("dtstack_poke");
  pos *= stack->elsize;					/* change to byte position			*/

  DT_Q(dtmem_get,(ctl,					/* copy page into ram			    */
    stack->memalloc,
    pos, el, NULL));
    
  DT_Q(dtmem_put,(ctl,					/* free this page on next demand    */
    stack->memalloc, pos));

  stack->repage = 1;   					/* force a repage at push or pop    */

  return DT_RC_GOOD;
}

/* ........................................................................ 
 * return pointer to el with given el position in stack
 * pointer is only guaranteed good until next push/pop/peek/poke call
 * ........................................................................ */

dt_rc_e
DTCONFIG_API1
dtstack_peek1(
  dt_ctl_t *ctl,
  dtstack_t *stack,
  long pos,								/* stack position id			    */
  void **el)
{
  DT_F("dtstack_peek1");

  DT_Q(dtmem_get,(ctl,					/* copy page into ram			    */
    stack->memalloc,
    pos, el, NULL));
    
  DT_Q(dtmem_unmap,(ctl,				/* free this page on next demand    */
    stack->memalloc, pos));

  stack->repage = 1;  					/* force a repage at push or pop    */

  return DT_RC_GOOD;
}

/* ........................................................................ 
 * return pointer to el with given el position in stack
 * page already marked as changed by this routine
 * pointer is only guaranteed good until next push/pop/peek/poke call
 * ........................................................................ */

dt_rc_e
DTCONFIG_API1
dtstack_poke1(
  dt_ctl_t *ctl,
  dtstack_t *stack,
  long pos,								/* stack position id			    */
  void **el)
{
  DT_F("dtstack_poke1");

  DT_Q(dtmem_get,(ctl,					/* copy page into ram			    */
    stack->memalloc,
    pos, el, NULL));
    
  DT_Q(dtmem_put,(ctl,					/* free this page on next demand    */
    stack->memalloc, pos));

  stack->repage = 1;   					/* force a repage at push or pop    */

  return DT_RC_GOOD;
}

/*__________________________________________________________________________
 * free stack space
 *__________________________________________________________________________*/

dt_rc_e
DTCONFIG_API1
dtstack_free(
  dt_ctl_t *ctl,
  dtstack_t *stack,
  char *F)
{
  if (stack->memalloc &&				/* space is allocated?				*/
	  stack->flags &					/* and we are supposed to free it?	*/
	  DTSTACK_FLAG_FREE)
    DT_Q(dtmem_free,(ctl,				/* free it						    */
      stack->memalloc, F, "stack"));
  DT_MEMSET(stack, 0, sizeof(*stack));		/* clear structure				    */
  return DT_RC_GOOD;
}

/*__________________________________________________________________________
 * OVERFLOW: set new position at which stack will overflow
 * POSITION: set new stack offset, from where next push or pop will happen
 *__________________________________________________________________________*/

dt_rc_e
DTCONFIG_API1
dtstack_set(
  dt_ctl_t *ctl,
  dtstack_t *stack,
  dtstack_set_flag_e flag,
  long value)
{
  DT_F("dtstack_set");

  if (flag ==							/* setting overflow point? 			*/
	  DTSTACK_SET_FLAG_OVERFLOW)
  {
	stack->offset_max = value;
  }
  else
  if (flag ==							/* setting push/pop point? 			*/
	  DTSTACK_SET_FLAG_POSITION)
  {
	long offset = value;
    stack->offset = LCEIL(offset,		/* offset is aligned at datasize    */
      stack->datasize);

    if (stack->offset ==				/* stack is full?				    */
        stack->offset_cur)
	{
      stack->el =						/* force repage in next push or pop	*/
      stack->elmin =
      stack->elmax = NULL;
	}
	else
    {					
      unsigned int datasize;
      DT_Q(dtmem_get,(ctl,				/* access the new page				*/
        stack->memalloc, stack->offset,
        (void **)&stack->elmin, 
        &datasize));
      DT_Q(dtmem_put,(ctl,				/* mark it for saving				*/
        stack->memalloc,
        stack->offset));

      if (stack->offset_max)			/* there is a caller limit?		    */
      if (stack->offset+(long)datasize >	/* they gave us too much?		    */
          stack->offset_max)
        datasize = (unsigned int)
          (stack->offset_max -
           stack->offset);

      stack->el = stack->elmin +		/* point into mem mapped at offset	*/
        (unsigned int)
        (offset - stack->offset);
      stack->elmax = stack->elmin +
        datasize;
	}
  }
  else
	return dt_err(ctl, F,
	  "invalid flag argument %d",
      flag);

  return DT_RC_GOOD;
}

/*__________________________________________________________________________
 *__________________________________________________________________________*/

dt_rc_e
DTCONFIG_API1
dtstack_print(
  dt_ctl_t *ctl,
  dtstack_t *stack,
  char *F)
{
  dt_dbg(ctl, F, DT_DBG_MASK_PIXEL, 
    "offset %ld, offset_cur %ld, offset_max %ld",
    stack->offset,
    stack->offset_cur,
    stack->offset_max);
  dt_dbg(ctl, F, DT_DBG_MASK_PIXEL, 
    "units_cur %u, units_inc %u, units_max %u",
    stack->a_units_cur,
    stack->a_units_inc,
    stack->a_units_max);
  dt_dbg(ctl, F, DT_DBG_MASK_PIXEL, 
    "size_cur %u, size_inc %u, size_max %u",
    stack->a_size_cur,
    stack->a_size_inc,
    stack->a_size_max);
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
