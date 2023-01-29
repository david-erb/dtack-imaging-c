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
DT_RCSID("src/high $RCSfile: label1.c,v $ $Revision: 1.7 $");
#include <dtack/mem.h>
#include <dtack/os.h>

#include <dtack/stack.h>					/* big memory stack access 			*/
#include <dtack/link1.h>					/* singly-linked list macros 		*/

#define NOID DTRUN_NOID
#define SET  DTRUN_SET
#define GET  DTRUN_GET
#define ID   DTRUN_ID

										/* this function is separate 		*/
										/* because it was to complex 		*/
										/* to be optimized by vc++1.5 		*/
										/* do not put this in dtimage.p 	*/
extern
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
  dtrun_blob_t *available);

/*..........................................................................
| NAME
| dtimage_run_label() - label runs by blob membership
| 
| SUMMARY
| Traverse the stack of runs in the ~run~ structure.
| Assume 4-connectivity unless the :DTIMAGE_ADJ_FLAG_8: bit is set.
| As each blob is found, pass the blob and the unchanged ~arg~
| to the ~deliver~ function.
| 
| END
 *..........................................................................*/

dt_rc_e 
DTCONFIG_API1
dtimage_run_label(						/* label runs by blob membership	*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtimage_t *image,						/* input image from which runs came */
  dtrun_t *run,							/* initialized run structure 		*/
  unsigned long flags,					/* process control 					*/
  dtrun_deliver_f *deliver,				/* function to call for each blob 	*/
  void *arg)							/* user argument to pass to deliver */
{
  DT_F("dtimage_run_label");
  long ipos;
  long nipos, njpos;
  long ipos1, jpos1;
  void *ivalue = NULL;					/* this row's pixel values		    */
  void *jvalue = NULL;					/* last row's pixel values		    */
  dtxy_t iy = 0;
  dtxy_t jy = 0;
  const int elsize =
    sizeof(dtrun_elem_t);
  const long maxpos = run->runs *
    elsize;
  const int maxblobs = image->xe*2 + 1;	/* max simultaneous active blobs 	*/
  dtrun_blob_t *blobs = NULL;
  dtrun_blob_t available;
  dtrun_blob_t tmp_blob[2];
  dtrun_blob_t *active = &tmp_blob[0];
  dtrun_blob_t *inactive = &tmp_blob[1];
  const int size = DT_CAST(int,
    dt_ntype_sizeof[image->ntype]);
# define FMAX (256)
  unsigned int *f = NULL;
  unsigned int zero = 0;
  dt_rc_e rc;

  rc = dtimage_check_1f(ctl,			/* check simple stuff				*/
    image, "image",
    dtrun_ntype_list, 
    DTIMAGE_FLAG_PAGED, F);		
  if (rc != DT_RC_GOOD)
    return rc;

  DT_C(dtos_malloc2,(ctl,				/* get space for blob list 			*/
    (void **)&blobs, 
    maxblobs*sizeof(*blobs),
    F, "active blob list"));

  DT_C(dtos_malloc2,(ctl,				/* get space for blob list 			*/
    (void **)&f, 
    FMAX*sizeof(*f),
    F, "up check start"));
  
  DTLINK1_ISOLATE(DT_C, ctl,			/* make an empty available list		*/
    &available, 
    dtrun_blob_t, status);

  {
    int i;
    for (i=0; i<maxblobs; i++)			/* blobs from array into available 	*/
      DTLINK1_LINK1(DT_C, ctl, 
        &available, &blobs[i], 
        dtrun_blob_t, status);
  }

  DTLINK1_ISOLATE(DT_C, ctl,			/* make an empty inactive list 		*/
    inactive,
    dtrun_blob_t, status)
  DTLINK1_ISOLATE(DT_C, ctl,			/* make an empty active list 		*/
    active,
    dtrun_blob_t, status)

  run->blobs = 0;						/* fyi only						    */
  nipos = 0;							/* runs on this row				    */
  ipos1 = 0;
  njpos = 0;							/* keep the compiler happy		    */
  jpos1 = 0;
  for (ipos=0; 
       ipos<maxpos; 
       ipos+=elsize)
  {
    dtrun_elem_t ielspace;
	dtrun_elem_t * const iel = &ielspace;
	double iv;

	{
  	  dtrun_elem_t *tmp;
      DTRUN_POKE(DT_Q, ctl,				/* the current run				    */
        run, ipos, (void **)&tmp);
      *iel = *tmp;						/* copy run element of stack 		*/
	}

    iel->blob = NULL;
    iel->p = NOID;
    if (ipos == 0 || iel->y != iy)		/* just hit a new row?			    */
	{
      if (jvalue)
        DTIMAGE_UNMAPROW(ctl,			/* release previous row			    */ 
          image, jy);

      if (inactive->status.next)		/* there are blobs to deliver?	    */
	  {
        DT_C(dtrun_deliver_list,(ctl,	/* deliver all blobs we can		    */
          run, inactive,
          deliver, 
          arg));
        DTLINK1_RELINK(DT_C, ctl,		/* make delivered blobs available	*/
          &available, 
          inactive->status.next,
          dtrun_blob_t, status)
        DTLINK1_ISOLATE(DT_C, ctl,		/* make inactive list empty		    */
          inactive,
          dtrun_blob_t, status)
	  }

	  {
        dtrun_blob_t *tmp;
        tmp = inactive;					/* swap active for inactive		    */
        inactive = active;
        active = tmp;
	  }

      if (ipos && iel->y - iy == 1)		/* row is adjacent to last one?	    */
	  {
        njpos = nipos;					/* runs on previous row			    */
        jpos1 = ipos1;					/* where last row started		    */
	  }
	  else								/* rows not adjacent?			    */
        njpos = 0;						/* act like no runs on previous row	*/
	  
	  jvalue = ivalue;
	  jy = iy;							/* remember mapped row at j		    */

      DTIMAGE_GETROW(ctl, image,		/* point to gray pixels in image    */
        iel->y, (void **)&ivalue);

      nipos = elsize;					/* start counting runs on this row	*/
      ipos1 = ipos;						/* where this row started		    */
      iy = iel->y;						/* remember previous row		    */

      DT_MEMSET(f, 0, FMAX*sizeof(*f));	/* first run on row above to check	*/
	}
    else
      nipos += elsize;					/* count runs on this row		    */
	
    DTNTYPE_GET1(DT_C, ctl,			/* label value of this run 			*/
      ivalue, iel->x, image->ntype,
      &iv);

	if (njpos)							/* any runs on row above?		    */
    {
      unsigned int *fiv = 
        (int)iv == iv &&
	    iv >= 0 && iv < FMAX?
        &f[(unsigned int)iv]: &zero;
	  DT_C(dtimage_run_label2,(ctl, run,
        flags, 
        ivalue, jvalue, size,
        ipos, iel,
        njpos, jpos1, fiv,
        active, &available));
      if (fiv == &zero)
        *fiv = 0;
    }

	if (iel->blob == NULL)				/* run never assigned to blob?		*/
	{
      dtrun_blob_t * const blob =
        available.status.next;
      if (blob)							/* available list is not empty?		*/
        available.status.next =			/* unlink top available blob	    */
          blob->status.next;
      else								/* available list is empty?			*/
        return dt_err(ctl, F,
          "too many active blobs (%d)",
          maxblobs);
      DTLINK1_LINK1(DT_C, ctl,			/* add blob to active list		    */
        active, blob,
        dtrun_blob_t, status)

	  blob->phead =	ID(ipos);			/* we are the first one in the blob	*/
	  blob->label = iv;					/* remember the label 				*/
	  blob->count = 0;					/* count of runs in the blob	    */
	  iel->p = NOID;					/* blob chain just has us			*/
      iel->blob = blob;					/* point back to our blob		    */
	}
	{
  	  dtrun_elem_t *tmp;
      DTRUN_POKE(DT_Q, ctl,				/* the current run				    */
        run, ipos, (void **)&tmp);
      *tmp = *iel;						/* copy run element back to stack	*/
	}
  }

  if (jvalue)
  {
    DTIMAGE_UNMAPROW(ctl, image, jy);	/* release previous row			    */ 
    jvalue = NULL;
  }

  if (rc == DT_RC_GOOD)
  {
    DT_C(dtrun_deliver_list,(ctl,		/* deliver all inactive blobs	    */
      run, inactive, deliver, arg));
    DT_C(dtrun_deliver_list,(ctl,		/* deliver all remaining blobs	    */
      run, active, deliver, arg));
  }

cleanup:
  if (jvalue)
    DTIMAGE_UNMAPROW2(DT_I, ctl,		/* release previous row			    */ 
      image, jy);

  if (ivalue)
    DTIMAGE_UNMAPROW2(DT_I, ctl,		/* release last row				    */ 
      image, iy);

  if (f)
    DT_Q(dtos_free2,(ctl, f,			/* free space for start check array	*/
      F, "up run check"));

  if (blobs)
    DT_Q(dtos_free2,(ctl, blobs,		/* free space for blob list 		*/
      F, "active blob list"));

  if (rc == DT_RC_STOP)
    rc = DT_RC_GOOD;

  if (rc != DT_RC_BAD)
  dt_dbg(ctl, F, DT_DBG_MASK_COMPLEX,
    "%lu blobs",
    run->blobs);

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
