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

#include <dtack/base.h>
DT_RCSID("src/high $RCSfile: run.c,v $ $Revision: 1.7 $");
#include <dtack/mem.h>
#include <dtack/os.h>

#include <dtack/stack.h>
#include <dtack/link1.h>

#define NOID DTRUN_NOID
#define SET  DTRUN_SET
#define GET  DTRUN_GET
#define ID   DTRUN_ID

extern
dt_rc_e
DTCONFIG_API1
dtrun_guts(								/* ***** 							*/
  dt_ctl_t *ctl,						/* not in dtrun.p on purpose		*/
  dtrun_t *run,							/* ***** 							*/
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

/*..........................................................................*/

dt_rc_e 
DTCONFIG_API1
dtrun_init(								/* allocate run structure		    */
  dt_ctl_t *ctl,
  dtrun_t *run)
{
  DT_F("dtrun_alloc_struct");

  DT_Q(dtos_malloc2,(ctl, 
    &run->stack,
    sizeof(dtstack_t),
    F, "stack structure"));

  DT_Q(dtstack_init,(ctl, 
    (dtstack_t *)run->stack,
    sizeof(dtrun_elem_t),
    0,									/* default initial depth			*/
    0,									/* default max depth			    */
    F));
  
  return DT_RC_GOOD;
}

/*..........................................................................*/

dt_rc_e 
DTCONFIG_API1
dtrun_uninit(							/* free run structure			    */
  dt_ctl_t *ctl,
  dtrun_t *run)
{
  DT_F("dtrun_free_struct");
  DT_Q(dtstack_free,(ctl,
    (dtstack_t *)run->stack, F));
  DT_Q(dtos_free2,(ctl, 
    run->stack, F, "stack structure"));
  return DT_RC_GOOD;
}

/*..........................................................................*/
dt_rc_e
DTCONFIG_API1
dtrun_deliver_list(						/* blob list delivery routine 		*/
  dt_ctl_t *ctl,
  dtrun_t *run,
  dtrun_blob_t *list,
  dtrun_deliver_f *deliver_callback,
  void *arg)
{
  DT_F("dtrun_deliver_list");
  dtrun_blob_t *blob;

  for (blob=list->status.next;			/* pass through all given blobs		*/
       blob; 
       blob=blob->status.next)
  {
    if (deliver_callback != NULL)		/* there is a callback routine?		*/
  	  DT_Q(deliver_callback,(ctl,		/* give it this blob			    */
        run, blob, arg));
    run->blobs++;						/* fyi only						    */
  }

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
