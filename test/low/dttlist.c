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
#include <dtack/link2.h>

DT_RCSID("test $RCSfile: dttlist.c,v $ $Revision: 1.2 $");

#define USAGE "[-Dnnnn] [-op n]"

static const char *ops[] = {"all",
  "default", "all", "link2", "time",
  NULL};

typedef struct dttlist_link2_t {
  int data;
  struct {
    struct dttlist_link2_t *head;
    struct dttlist_link2_t *next;
    struct dttlist_link2_t *prev;
  } link2;
} dttlist_link2_t;

/*..........................................................................*/

static
dt_rc_e
dttlist_link2_print(
  dt_ctl_t *ctl,
  dttlist_link2_t *test,
  dttlist_link2_t *base,
  char *F,
  char *desc)
{
  dttlist_link2_t *t = test->link2.head;
  int i;
  for (i=0; t; i++)
  {
    char next[32];
    char prev[32];
    sprintf(next, "%ld",
      (long)(t->link2.next-base));
    sprintf(prev, "%ld",
      (long)(t->link2.prev-base));

    dt_dbg(ctl, F, DT_DBG_MASK_PIXEL,
      "%s%3d. %03d head %4ld, next %-4s, prev %-4s",
      desc? desc: "",
      i,
      t->data,
      (long)(t->link2.head-base),
      t->link2.next? next: "NULL",
      t->link2.prev? prev: "NULL");
     
    t = t->link2.next;
  }
  return DT_RC_GOOD;
}

/*..........................................................................*/

static
dt_rc_e
dttlist_link2_check(
  dt_ctl_t *ctl,
  dttlist_link2_t *test,
  dttlist_link2_t *base,
  int *data,
  char *desc,
  int *assert_fail)
{
  DT_F("dttlist_link2_check");
  dttlist_link2_t *head = test->link2.head;
  dttlist_link2_t *t = head;
  int i;
  int bad = 0;

  DT_Q(dttlist_link2_print,(ctl,
    head, base, F, desc));

  for (i=0; data[i] != -1; i++)
  {
    if (t == NULL)
    {
      *assert_fail += DT_RC_GOOD !=
        dt_assert(ctl, 0, F,
          "%-16s data[%d] end of list (%03d)",
          desc, i, data[i]);
      break;
    }
    else
	{
      if (t->data != data[i])			/* check proper data at this node	*/
      {
        *assert_fail += DT_RC_GOOD !=
          dt_assert(ctl, 0, F,
            "%-16s data[%d] %03d (%03d)",
            desc, i, t->data, data[i]);
        bad++;
        if (bad > 10) break;
      }
      if (t->link2.head != head)		/* check proper link at this node	*/
      {
        *assert_fail += DT_RC_GOOD !=
          dt_assert(ctl, 0, F,
            "%-16s head[%d] %03d (%03d)",
            desc, i,
            t->link2.head-base,
            head-base);
        bad++;
        if (bad > 10) break;
      }
	}
    t = t->link2.next;
  }

  if (!bad)								/* no bad asserts in loop above?    */
    dt_assert(ctl, 1, F,
      "%-16s data %d out of %d",
      desc, i, i);

  if (t != NULL)						/* not at end of list here?		    */
    *assert_fail += DT_RC_GOOD !=
      dt_assert(ctl, 0, F,
        "%-16s data[%d] %03d (end of list)",
          desc, i, t->data, data[i]);

  return DT_RC_GOOD;
}


/*..........................................................................*/
static
dt_rc_e
dttlist_link2(
  dt_ctl_t *ctl,
  int *assert_fail)
{
  DT_F("dttlist_link2");
# define N 8
  dttlist_link2_t test[N];
  int i;

  for (i=0; i<N; i++)
  {
    test[i].data = i * 111;
    if (i == 0)
      DTLINK2_ISOLATE(DT_Q, ctl, 
        &test[i], 
        dttlist_link2_t, link2)
    else
      DTLINK2_LINK1(DT_Q, ctl, 
        &test[i-1], &test[i], 
        dttlist_link2_t, link2);
  }

  {
    char *id = "initial list";
    int data[N+1] = {000, 111, 222, 333, 444, 555, 666, 777, -1};
    DT_Q(dttlist_link2_check,(ctl,	
      test, test, data, id, assert_fail));
  }
										/* ................................. */
  for (i=N/2; i<N; i++)
  {
    DTLINK2_UNLINK1(DT_Q, ctl,
      &test[i],
      dttlist_link2_t, link2);
    if (i == N/2)
      DTLINK2_ISOLATE(DT_Q, ctl, 
        &test[i], 
        dttlist_link2_t, link2)
    else
      DTLINK2_LINK1(DT_Q, ctl, 
        &test[i-1], &test[i], 
        dttlist_link2_t, link2);
  }

  {
    char *id = "first half";
    int data[N/2+1] = {000, 111, 222, 333, -1};
    DT_Q(dttlist_link2_check,(ctl,	
      test, test, data, id, assert_fail));
  }
  {
    char *id = "second half";
    int data[N/2+1] = {444, 555, 666, 777, -1};
    DT_Q(dttlist_link2_check,(ctl,	
      test+N/2, test, data, id, assert_fail));
  }
										/* ................................. */
  for (i=N/2-1; i>=0; i--)
  {
    DTLINK2_UNLINK1(DT_Q, ctl,
      &test[i],
      dttlist_link2_t, link2);
    if (i == N/2-1)
      DTLINK2_ISOLATE(DT_Q, ctl, 
        &test[i], 
        dttlist_link2_t, link2)
    else
      DTLINK2_LINK1(DT_Q, ctl, 
        &test[i+1], &test[i], 
        dttlist_link2_t, link2);
  }

  {
    char *id = "reversed";
    int data[N/2+1] = {333, 222, 111, 000, -1};
    DT_Q(dttlist_link2_check,(ctl,	
      test, test, data, id, assert_fail));
  }

  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/

static
dt_rc_e
dttlist_time(
  dt_ctl_t *ctl,
  long max,
  long loops,
  int *assert_fail)
{
#ifdef NONO
  DT_F("dttlist_time");
  dtos_time_t mark1, mark2;
  long loop;
  char f[64];
  DT_Q(dtos_time,(ctl, &mark1));
  for (loop=0; loop<loops; loop++)
  {
    DT_Q(dttlist_paged,(ctl,			/* no reallocations				    */
      NULL, max, 0x08, 
      assert_fail));
  }
  DT_Q(dtos_time,(ctl, &mark2));

  sprintf(f, "%ld type 8 (%2ld)",	
    max, loops);
  dtt_time_report(ctl,
    &mark1, &mark2, f);
#endif
										/* ................................ */
  return DT_RC_GOOD;    
}

/*..........................................................................*/

dt_rc_e
DTCONFIG_API1
dttlist_arg_check(						/* check args and params            */
  dt_ctl_t *ctl,
  void *app,
  dtparam_t *param,
  dt_enter_arg_t *arg,
  char *msg,
  const char **start)
{
  DT_F("dttlist_arg_check");

  DT_Q(dtt_check_op,(ctl, param,        /* make sure op is understood       */
    ops, msg, start));

  return DT_RC_GOOD;
}

/*..........................................................................*/

DTT_TEST(dttlist)
{
  DT_F("dttlist");
  dt_ctl_t *ctl = &param->ctl;
  dt_enter_arg_t arg[1];
  int assert_fail = 0;                  /* failed assertion count           */
  dt_rc_e rc = DT_RC_GOOD;

  DT_GI(dt_heapmon_init,(ctl));         /* init memory checking             */

  DT_GI(dtparam_init,(param));          /* init param structure             */

  DT_GI(dtparam_set,(param, 0,          /* set default for params we want   */
    &param->op, NULL, NULL, "default",
    &param->reps, NULL, NULL, "1",
    &param->max, "Timing size", NULL, "100000",
    &param->n, "Timing loops", NULL, "5",
    NULL));

  arg[0].desc = NULL;                   /* no arguments                     */
  
  DT_GI(dt_enter_parse,(				/* parse command line				*/
    argc, argv, F, USAGE, param,
    arg, dttlist_arg_check, NULL));
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
          !strcmp(param->op, "default") ||
          !strcmp(param->op, "link2"))
      {
        DT_GI(dttlist_link2,(ctl,
          &assert_fail));
      }
    
      if (!strcmp(param->op, "all") ||
          !strcmp(param->op, "time"))
      {
        DT_GI(dttlist_time,(ctl,
          (long)param->max, param->n,
          &assert_fail));
      }
    }
  }

                                        /* ................................ */

  DT_I(dt_leave,(param));				/* clean up before leaving          */

  DT_I(dt_heapmon_uninit,(ctl,          /* check all memory is freed        */
    &assert_fail));

  if (assert_fail)                      /* any failed assertions?           */
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
