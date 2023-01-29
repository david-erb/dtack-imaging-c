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
  19991217 DE changed some long's to 4-byte int's in favor of 64-bit CPU
 *..........................................................................*/


#include <dtack/base.h>
DT_RCSID("src/high $RCSfile: label2.c,v $ $Revision: 1.7 $");
#include <dtack/mem.h>

#include <dtack/stack.h>
#include <dtack/link1.h>					/* single-linked list macros 		*/

#define NOID DTRUN_NOID					/* for short 						*/
#define SET  DTRUN_SET
#define GET  DTRUN_GET
#define ID   DTRUN_ID

#define S1 char
#define S2 short
#define S4 dtntype_s4_t
#define S8 double

#define CMP(P, X, Q, U) \
  ((size == 1 && ((S1 *)(P))[(X)] != ((S1 *)(Q))[(U)]) || \
   (size == 2 && ((S2 *)(P))[(X)] != ((S2 *)(Q))[(U)]) || \
   (size == 4 && ((S4 *)(P))[(X)] != ((S4 *)(Q))[(U)]) || \
   (size == 8 && ((S8 *)(P))[(X)] != ((S8 *)(Q))[(U)]))

/*..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtimage_run_label2(
  dt_ctl_t *ctl,
  dtrun_t *run,
  unsigned long flags,
  void *ivalue,
  void *jvalue,
  const int size,
  long ipos,
  dtrun_elem_t *iel,
  long njpos,
  long jpos1,
  unsigned int *fiv,
  dtrun_blob_t *active,
  dtrun_blob_t *available)
{
  DT_F("dtimage_run_label2");
  long jpos;
  const int elsize =
    sizeof(dtrun_elem_t);

      const dtrun_id_t iid = 
        ID(ipos);
      const dtxy_t i0 = iel->x;
      const dtxy_t il = iel->l;

      for (jpos=(*fiv);					/* for all runs on previous row	    */
           jpos<njpos;  
           jpos+=elsize)
      {
        dtrun_elem_t *jel;
        DTRUN_POKE(DT_Q, ctl, run,		/* the run on the row above			*/
          jpos+jpos1, (void **)&jel);

		if (!CMP(ivalue, i0,			/* only consider if same value	    */
                 jvalue, jel->x))
		{
          const dtxy_t j0 = jel->x;
		  const dtxy_t jl = jel->l;
		  const int d = i0 - j0;		/* distance between starts			*/
		  int rel;


#define IR 0x1
#define JR 0x2
#define IC 0x4
#define JC 0x8
#define COVER (JC|IC)
#define BEYOND (JR|JC)
		  if (flags & 
			  DTIMAGE_ADJ_FLAG_8)		/* 8-connected?						*/
		  {
 		    if (d > 0)					/* i is to right of j?				*/
		    {	
			  if (d > jl+1)   rel = IR;	/* i start past j end?				*/
			  else
			  if (d+il<jl-1)  rel = JC;	/* i completely covered by j?		*/
			  else            rel = IC;	/* end of i extends past end of j	*/
		    }
		    else						/* i is to left of j?				*/
		    {
  			  if (-d>il+1)    rel = JR;	/* j start past i end?				*/
			  else
			  if (-d+jl<il-1) rel = IC;	/* j completely covered by i?		*/
			  else            rel = JC;	/* end of j extends past end of i	*/
			}
   		  }
		  else							/* 4-connected?						*/
		  {
 		    if (d > 0)					/* i is to right of j?				*/
		    {	
			  if (d > jl)    rel = IR;	/* i start past j end?				*/
			  else
			  if (d+il < jl) rel = JC;	/* i completely covered by j?		*/
			  else           rel = IC;	/* end of i extends past end of j	*/
		    }
		    else						/* i is to left of j?				*/
		    {
  			  if (-d > il)    rel = JR;	/* j start past i end?				*/
			  else
			  if (-d+jl < il) rel = IC;	/* j completely covered by i?		*/
			  else            rel = JC;	/* end of j extends past end of i	*/
			}
   		  }
		  

		  if (rel & COVER)				/* two runs overlap?				*/
		  {
            dtrun_blob_t * 
              iblob = iel->blob;
            dtrun_blob_t *  
              jblob = jel->blob;
            if (iel->blob == NULL)		/* first time run in any blob?		*/
			{
			  iel->blob = jblob;		/* assign new run to the blob	  	*/
			  iel->p = jel->p;			/* link run into blob's chain		*/
              jblob->count++;			/* count runs in the blob		    */
			  jel->p = iid;
			}
			else						/* merge blobs					    */
			{
              if (iblob != jblob)		/* not already in the same blob?	*/
			  {						
                dtrun_id_t p;	
                dtrun_elem_t *el;

				if (jblob->count <		/* fewer runs in the jblob?		    */
                    iblob->count)
				{
				  dtrun_blob_t *t;
                  t = jblob;
                  jblob = iblob;		/* swap to unlink the smallest	    */
                  iblob = t;
				}

                p = iblob->phead;		/* head of iblob run chain		    */
                while(1)				/* traverse iblob run chain		    */
				{
                  if (p == iid)			/* we are passing the 'i' run?		*/
				    el = iel;
				  else
                    DTRUN_POKE(DT_Q,	/* access the run				    */
                      ctl, run, p, &el);
                  
				  el->blob = jblob;		/* point run at its new blob	    */
                  if (el->p == NOID)
                    break;
                  p = el->p;			/* next run in run chain		    */
				}

                el->p = jblob->phead;	/* join the run chains			    */
                jblob->phead =
                  iblob->phead;
				jblob->count +=			/* keep run count current		    */
                  iblob->count;

                DTLINK1_UNLINK1(DT_Q,	/* unlink i blob from its status    */
                  ctl, iblob, 
                  dtrun_blob_t,
                  status);
                DTLINK1_LINK1(DT_Q,		/* add it to the available list		    */
                  ctl, available, iblob,
                  dtrun_blob_t,
                  status);
			  }
			}
            if (jblob->status.head !=	/* j blob is not already active?    */
                active)
			{
              DTLINK1_UNLINK1(DT_Q,		/* unlink it from the inactive list	*/
                ctl, jblob, 
                dtrun_blob_t,
                status);
              DTLINK1_LINK1(DT_Q,		/* add it to the active list	    */
                ctl, active, jblob,
                dtrun_blob_t,
                status);
			}
		  }
		  if (rel & BEYOND)				/* j now beyond i					*/
            break;						/* quit looking for j's on this i	*/
#undef IR
#undef JR
#undef IC
#undef JC
#undef COVER
#undef BEYOND
		}
      }

  *fiv = (unsigned int)jpos;			/* remember where we left off 		*/

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
