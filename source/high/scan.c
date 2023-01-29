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
  CHANGES
  20010104 DE init p and blob members to keep purify happy
  19991217 DE changed some long's to 4-byte int's in favor of 64-bit CPU
 *..........................................................................*/


#include <dtack/base.h>
DT_RCSID("src/high $RCSfile: scan.c,v $ $Revision: 1.3 $");

#include <dtack/stack.h>
#include <dtack/mem.h>

#define S1 char
#define S2 short
#define S4 dtntype_s4_t
#define S8 double

#define CMP1(P, X, V) \
  ((size == 1 && ((S1 *)(P))[(X)] != V ## 1) || \
   (size == 2 && ((S2 *)(P))[(X)] != V ## 2) || \
   (size == 4 && ((S4 *)(P))[(X)] != V ## 4) || \
   (size == 8 && ((S8 *)(P))[(X)] != V ## 8))

#define CMP2(P, X, Q, U) \
  ((size == 1 && ((S1 *)(P))[(X)] != ((S1 *)(Q))[(U)]) || \
   (size == 2 && ((S2 *)(P))[(X)] != ((S2 *)(Q))[(U)]) || \
   (size == 4 && ((S4 *)(P))[(X)] != ((S4 *)(Q))[(U)]) || \
   (size == 8 && ((S8 *)(P))[(X)] != ((S8 *)(Q))[(U)]))

#define ADD                                                                   \
{                                     		                                  \
  DTSTACK_PUSH(DT_Q, ctl,				/* put run on the stack		    	*/\
    stack, (void **)&el,                   	                        	      \
    &code);                                                           		  \
  if (code != DTSTACK_CODE_OK)			/* stack still OK?				  	*/\
    goto done;                                                        		  \
  el->x = x0;                                                         		  \
  el->y = y;                                                          		  \
  el->l = x - x0 - 1;                                                 		  \
  el->blob = NULL;                      /* init just to keep purify happy   */\
  el->p = 0;                            /* init just to keep purify happy   */\
}

/*..........................................................................
| NAME
| dtimage_run_scan() - scan image for runs
| 
| SUMMARY
| Extract horizontal runs of pixels with identical values.
| Push runs onto a stack in the pre-initialized ~run~ structure.
| Grow the stack as necessary.
| Examine only pixels in the rectangular area specified.
| Ignore pixels with the ~invisible~ value if ~flag~ is less than 0.
| Ignore all except invisible pixels if it is greater than 0.
| Ignore no pixels if flag is 0.
| 
| END
 *..........................................................................*/

dt_rc_e 
DTCONFIG_API1
dtimage_run_scan(						/* scan image for runs 				*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtimage_t *input,						/* input image 						*/
  dtrun_t *run,							/* initialized run structure 		*/
  dtxy_t xmin,							/* upper left column of rectangle	*/
  dtxy_t ymin,							/* upper left row 					*/
  dtxy_t xmax,							/* lower right column 				*/
  dtxy_t ymax,							/* lower right row 					*/
  double invisible,						/* invisible pixel value 			*/
  int flag)								/* how to treat invisible pixel		*/
{
  DT_F("dtimage_run_scan");
  dtxy_t x0, x, y;
  dtstack_code_e code = DTSTACK_CODE_OK;
  dtrun_elem_t *el;
  dtstack_t *stack = 
    (dtstack_t *)run->stack;

  const int size = DT_CAST(int, 		/* size of pixel 					*/
    dt_ntype_sizeof[input->ntype]);
  dt_ntype_f2_t value;					/* big enough for largest pixel 	*/
  dt_rc_e rc;

  rc = dtimage_check_1f(ctl,			/* check simple stuff				*/
    input, "input",
    dtrun_ntype_list, 
    DTIMAGE_FLAG_PAGED, F);		
  if (rc != DT_RC_GOOD)
    return rc;

  DTNTYPE_SET1(DT_Q, ctl,				/* re-cast invisible value 			*/
    &value, 0, input->ntype, 
    invisible);
  
  {
    const S1 value1 =					/* background pixel value			*/
      size == 1? *((S1 *)&value): 0;
    const S2 value2 = 
      size == 2? *((S2 *)&value): 0;
    const S4 value4 = 
      size == 4? *((S4 *)&value): 0;
    const S8 value8 = 
      size == 8? *((S8 *)&value): 0;

  run->head = DTRUN_NOID;
  for (y=ymin; y<ymax; y++)				/* for all rows in image		    */
  {
    void *p;
    DTIMAGE_GETROW(ctl, input, y, &p);
    {
      x0 = xmin;
      if (flag < 0)						/* ................................ */
  	  {
        for (x=x0+1; x<xmax; x++)
		{
          if (CMP2(p, x, p, x0))		/* run starts here?				    */
          {
            if (CMP1(p, x0, value))		/* keep the run?				    */
              ADD;
            x0 = x;
  	      }
		}
        if (CMP1(p, x0, value))			/* keep the last run?			    */
          ADD;
      }
      else								/* ................................ */
      if (flag > 0)
  	  {
        for (x=x0+1; x<xmax; x++)
		{
          if (CMP2(p, x, p, x0))		/* run starts here?				    */
          {
            if (!CMP1(p, x0, value))	/* keep the run?				    */
              ADD;
            x0 = x;
  	      }
		}
        if (!CMP1(p, x0, value))		/* keep the last run?			    */
          ADD;
      }
      else								/* ................................ */
  	  {
        for (x=x0+1; x<xmax; x++)
		{
          if (CMP2(p, x, p, x0))		/* run starts here?				    */
          {
            ADD;						/* keep all the runs			    */
            x0 = x;
    	  }
		}
        ADD;							/* add the last run				    */
  	  } 
    }
    DTIMAGE_UNMAPROW(ctl, input, y);	/* no change to the input image	    */
  }
  }

  run->runs = DTSTACK_DEPTH(stack);

  dt_dbg(ctl, F, DT_DBG_MASK_COMPLEX,
    "%ld runs took %ld bytes of stack",
    run->runs,   
    run->runs * sizeof(*el));

done:
  if (code == DTSTACK_CODE_OVERFLOW)
    return dt_err(ctl, F, "run stack overflow at %ld runs",
      DTSTACK_DEPTH(stack));
  if (code == DTSTACK_CODE_UNDERFLOW)
    return dt_err(ctl, F, "run stack underflow at %ld runs",
      DTSTACK_DEPTH(stack));

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
