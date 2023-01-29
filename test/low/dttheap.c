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

#include <dtack/t.h>
DT_RCSID("test $RCSfile: dttheap.c,v $ $Revision: 1.1 $");
#include <dtack/heap.h>

#define USAGE "[-Dnnnn]"

static const char *ops[] = {
  "all",
  "fast",
  NULL};

/*..........................................................................*/

static
dt_rc_e
dttheap_test(
  dt_ctl_t *ctl,
  int *assert_fail)
{
  DT_F("dttheap_test");
  
#define BUFSIZE 10
  char *buf1;
  char *buf2;
  long blocks, bytes;

  DT_Q(dt_heapmon_init,(ctl));			/* init memory checking             */

  DT_Q(dtos_malloc2,(ctl,				/* allocate a small piece			*/
    (void **)&buf1, BUFSIZE,
    F, "buf1"));

  DT_Q(dtheap_outstanding,(ctl,			/* check outstanding memory 		*/
    ctl->heap,
    &blocks, &bytes));
  
  *assert_fail = *assert_fail + (
    DT_RC_GOOD != dt_assert(ctl, 
      blocks == 1 && 
      bytes == BUFSIZE,
      F,
      "blocks %ld (%d),"
      " bytes %ld (%d)", 
      blocks, 1, bytes, BUFSIZE));

  DT_Q(dtos_realloc3,(ctl, buf1,		/* allocate a bigger piece			*/
    (void **)&buf2, BUFSIZE*2,
    F, "realloc buf1"));

  DT_Q(dtheap_outstanding,(ctl,			/* check outstanding memory 		*/
    ctl->heap,
    &blocks, &bytes));
  
  *assert_fail = *assert_fail + (
    DT_RC_GOOD != dt_assert(ctl, 
      buf2 != NULL &&
      blocks == 1 && 
      bytes == BUFSIZE * 2,
      F,
      "blocks %ld (%d),"
      " bytes %ld (%d)", 
      blocks, 1, bytes, BUFSIZE*2));

  DT_Q(dtos_malloc2,(ctl,				/* allocate  another small piece	*/
    (void **)&buf1, BUFSIZE*3,
    F, "bigger buf1"));

  DT_Q(dtheap_outstanding,(ctl,			/* check outstanding memory 		*/
    ctl->heap,
    &blocks, &bytes));
  
  *assert_fail = *assert_fail + (
    DT_RC_GOOD != dt_assert(ctl, 
      blocks == 2 && 
      bytes == BUFSIZE * 5,
      F,
      "blocks %ld (%d),"
      " bytes %ld (%d)", 
      blocks, 1, bytes, BUFSIZE*5));

  if (DT_DBG(ctl, DT_DBG_MASK_ASSERT))	/* debug level high enough? 		*/
    DT_Q(dt_heapmon_report,(ctl, "A"));	/* make a heap report 				*/

  DT_Q(dtos_free2,(ctl, buf1,
    F, "bigger buf1"));

  if (buf2 != NULL)
  DT_Q(dtos_free2,(ctl, buf2,
    F, "realloc buf1"));

  DT_Q(dt_heapmon_uninit,(ctl,			/* check all memory is freed	    */
    assert_fail));
  
  return DT_RC_GOOD;
}


/*..........................................................................*/
dt_rc_e
DTCONFIG_API1
dttheap_arg_check(						/* check args and params            */
  dt_ctl_t *ctl,
  void *app,
  dtparam_t *param,
  dt_enter_arg_t *arg,
  char *msg,
  const char **start)
{
  DT_F("dttheap_arg_check");

  DT_Q(dtt_check_op,(ctl, param,        /* make sure op is understood       */
    ops, msg, start));

  return DT_RC_GOOD;
}

/*..........................................................................*/

DTT_TEST(dttheap)
{
  DT_F("dttheap");
  dt_ctl_t *ctl = &param->ctl;
  dt_enter_arg_t arg[1];
  int assert_fail = 0;
  dt_rc_e rc = DT_RC_GOOD;

  DT_GI(dtparam_init,(param));          /* init param heapucture             */

  DT_GI(dtparam_set,(param, 0,          /* set default for params we want   */
    &param->op, NULL, NULL, "fast",
    &param->reps, NULL, NULL, "1",
    NULL));

  arg[0].desc = NULL;                   /* no arguments                     */
  
  DT_GI(dt_enter_parse,(				/* parse command line interactively	*/
    argc, argv, F, USAGE, param,
    arg, dttheap_arg_check, NULL));
                                        /* ................................ */
  {
    long reps;
    for (reps=0;
         reps < param->reps &&
         rc == DT_RC_GOOD;
         reps++)
    {
	  DTT_YIELD_REPS(DT_G, ctl,			/* yield at start of repetition 	*/
        param, reps);
	  
      if (!strcmp(param->op, "all") ||
          !strcmp(param->op, "fast"))
      {
		DT_GI(dttheap_test,(ctl, &assert_fail));
      }
	}
  }

                                        /* ................................ */
  DT_I(dt_leave,(param));				/* clean up before leaving			*/

  if (assert_fail)						/* any failed assertions?		    */
    return DT_RC_BAD;
  else
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
