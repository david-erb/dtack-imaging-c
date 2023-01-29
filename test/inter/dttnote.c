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

DT_RCSID("test $RCSfile: dttnote.c,v $ $Revision: 1.2 $");

#include <dtack/note.h>

#include <dtack/mem.h>

#define USAGE "[-Dnnnn]"

static const char *ops[] = {"all",
  "fast",
  NULL};

/*..........................................................................*/

dt_rc_e
dttnote_simple(
  dt_ctl_t *ctl,
  char *id,
  int size,
  int units,
  int *assert_fail)
{
  DT_F("dttnote_simple");
  dtnote_t note;
  dtnote_cursor_t cursor;
  char *s;
  void *mem;

# define CHECK(S1, S2) \
  assert_fail += DT_RC_GOOD != \
    dt_assert(ctl, \
      !strcmp(S1?S1:"null", S2?S2:"null"), \
      F, "%s: \"%s\" (\"%s\")", id, \
      S1?S1:"null", S2?S2:"null");

  DT_Q(dtmem_alloc,(ctl, &mem,
    0, 0, size, units, F, "test"));

  DT_MEMSET(&note, 0, sizeof(note));
  note.mem = mem;
  DT_Q(dtnote_add,(ctl, &note, "a", -1));
  DT_Q(dtnote_add,(ctl, &note, "b", -1));
  DT_MEMSET(&cursor, 0, sizeof(cursor));
  DT_Q(dtnote_next,(ctl, &note, &cursor, &s));
  CHECK(s, "a");
  DT_Q(dtnote_next,(ctl, &note, &cursor, &s));
  CHECK(s, "b");

  DT_Q(dtnote_next,(ctl, &note, &cursor, &s));
  CHECK(s, NULL);
  DT_Q(dtnote_next,(ctl, &note, &cursor, &s));
  CHECK(s, NULL);

  DT_Q(dtnote_add,(ctl, &note, "c", -1));
  DT_Q(dtnote_next,(ctl, &note, &cursor, &s));
  CHECK(s, "c");

  DT_Q(dtnote_next,(ctl, &note, &cursor, &s));
  CHECK(s, NULL);
  DT_Q(dtnote_next,(ctl, &note, &cursor, &s));
  CHECK(s, NULL);

  DT_Q(dtnote_prev,(ctl, &note, &cursor, &s));
  CHECK(s, "c");

  DT_Q(dtnote_next,(ctl, &note, &cursor, &s));
  CHECK(s, "c");

  DT_Q(dtnote_prev,(ctl, &note, &cursor, &s));
  CHECK(s, "c");

  DT_Q(dtnote_prev,(ctl, &note, &cursor, &s));
  CHECK(s, "b");

  DT_Q(dtnote_prev,(ctl, &note, &cursor, &s));
  CHECK(s, "a");

  DT_Q(dtnote_next,(ctl, &note, &cursor, &s));
  CHECK(s, "a");

  DT_Q(dtnote_prev,(ctl, &note, &cursor, &s));
  CHECK(s, "a");

  DT_Q(dtnote_prev,(ctl, &note, &cursor, &s));
  CHECK(s, NULL);

  DT_Q(dtnote_setpos,(ctl, &note, &cursor, 4));
  DT_Q(dtnote_next,(ctl, &note, &cursor, &s));
  CHECK(s, "c");

  DT_Q(dtmem_free,(ctl, mem, F, "test"));

  return DT_RC_GOOD;
}

/*..........................................................................*/

dt_rc_e
dttnote_check(
  dt_ctl_t *ctl,
  char *F,
  char *id,
  dtnote_t *note,
  int n,
  int *assert_fail)
{
  dtnote_cursor_t cursor;
  char s1[10];
  char *s2;
  long p1 = -1;
  long p2 = -1;
  int i;

# define SET(S1) \
  sprintf(s1, "line %03d ", i);

  DT_MEMSET(&cursor, 0, sizeof(cursor));

  for (i=0; i<n; i++)					/* fetch back all the strings	    */
  {
    if (i == 1)
      p1 = cursor.pos;
    if (i == n-2)
      p2 = cursor.pos;

    DT_Q(dtnote_next,(ctl, note,
      &cursor, &s2));
    SET(s1);
    CHECK(s2, s1);
  }

  DT_Q(dtnote_next,(ctl, note,			/* should get NULL now 				*/
    &cursor, &s2));
  CHECK(s2, NULL);

  DT_Q(dtnote_next,(ctl, note,			/* should get NULL again			*/
    &cursor, &s2));
  CHECK(s2, NULL);

  for (i=n-1; i>=0; i--)				/* fetch back in reverse		    */
  {
    DT_Q(dtnote_prev,(ctl, note,
      &cursor, &s2));
    SET(s1);
    CHECK(s2, s1);
  }

  DT_Q(dtnote_prev,(ctl, note,			/* should get NULL now 				*/
    &cursor, &s2));
  CHECK(s2, NULL);

  DT_Q(dtnote_prev,(ctl, note,			/* should get NULL again			*/
    &cursor, &s2));
  CHECK(s2, NULL);

  if (n)
  {
    DT_Q(dtnote_setpos,(ctl, note,		/* position at start			    */
      &cursor, 0));
    DT_Q(dtnote_next,(ctl, note,
      &cursor, &s2));
    i = 0;
    SET(s1);
    CHECK(s2, s1);

    DT_Q(dtnote_setpos,(ctl, note,		/* position at last				    */
      &cursor, 0x7fffffffL));
    DT_Q(dtnote_prev,(ctl, note,
      &cursor, &s2));
    i = n - 1;
    SET(s1);
    CHECK(s2, s1);
  }

  if (p1 >= 0)
  {
    DT_Q(dtnote_setpos,(ctl, note,		/* position at second			    */
      &cursor, p1));
    DT_Q(dtnote_next,(ctl, note,
      &cursor, &s2));
    i = 1;
    SET(s1);
    CHECK(s2, s1);
  }

  if (p2 >= 0)
  {
    DT_Q(dtnote_setpos,(ctl, note,		/* position at next to last		    */
      &cursor, p2));
    DT_Q(dtnote_next,(ctl, note,
      &cursor, &s2));
    i = n - 2;
    SET(s1);
    CHECK(s2, s1);
  }


  return DT_RC_GOOD;
}

/*..........................................................................*/

dt_rc_e
dttnote_longer(
  dt_ctl_t *ctl,
  char *id,
  int pagesize,
  int npages,
  int *assert_fail)
{
  DT_F("dttnote_longer");
  dtnote_t note;
  char s1[10];
  dtmemsource_t memsource;
  dtmemalloc_t *memalloc;
  int i, n;

# define SET(S1) \
  sprintf(s1, "line %03d ", i);

  DT_Q(dtmemram_init,(ctl,				/* make a memsource for our use	    */
    &memsource, "test", 0,
    pagesize, 2));

  DT_Q((memsource.alloc),(ctl,			/* allocate with the interface	    */
    &memsource,
    (void **)&memalloc,
    0, 0,								/* should use dtmem_alloc2		    */
    pagesize, npages,
    F, "test"));

  DT_MEMSET(&note, 0, sizeof(note));
  note.mem = memalloc;

  n = pagesize / sizeof(s1);			/* how many strings in a page	    */
  n *= npages;							/* strings in all the pages		    */

  for (i=0; i<n+2; i++)					/* overfill						    */
  {
    SET(s1);
    DT_Q(dtnote_add,(ctl, &note,
      s1, sizeof(s1)));
  }

  DT_Q(dttnote_check,(ctl, F, id,		/* check readback				    */
    &note, n, assert_fail));

  DT_Q((memalloc->free),(ctl,			/* deallocate with the interface	*/
    memalloc,
    F, "test"));

  DT_Q(dtmemsource_uninit,(ctl,			/* get rid of the memsource		    */
    &memsource));

  return DT_RC_GOOD;
}

/*..........................................................................*/

dt_rc_e
dttnote_append(
  dt_ctl_t *ctl,
  char *id,
  int pagesize,
  int npages,
  int *assert_fail)
{
  DT_F("dttnote_append");
  dtnote_t note;
  char s1[10];
  dtmemsource_t memsource;
  dtmemalloc_t *memalloc;
  int i, n;

# define SET(S1) \
  sprintf(s1, "line %03d ", i);

  DT_Q(dtmemram_init,(ctl,				/* make a memsource for our use	    */
    &memsource, "test", 0,
    pagesize, 2));

  DT_Q((memsource.alloc),(ctl,			/* allocate with the interface	    */
    &memsource,
    (void **)&memalloc,
    0, 0,								/* should use dtmem_alloc2		    */
    pagesize, npages,
    F, "test"));

  DT_MEMSET(&note, 0, sizeof(note));
  note.mem = memalloc;

  n = pagesize / sizeof(s1);			/* how many strings in a page	    */
  n *= npages;							/* strings in all the pages		    */

  for (i=0; i<n+2; i++)					/* overfill						    */
  {
    SET(s1);
    DT_Q(dtnote_append,(ctl, &note,
      s1, sizeof(s1)));
    DT_Q(dtnote_add,(ctl, &note,
      NULL, 0));
  }

  DT_Q(dttnote_check,(ctl, F, id,		/* check readback				    */
    &note, n, assert_fail));

  DT_Q((memalloc->free),(ctl,			/* deallocate with the interface	*/
    memalloc,
    F, "test"));

  DT_Q(dtmemsource_uninit,(ctl,			/* get rid of the memsource		    */
    &memsource));

  return DT_RC_GOOD;
}

/*..........................................................................*/
static
dt_rc_e
dttnote_arg_check(						/* check args and params            */
  dt_ctl_t *ctl,
  void *app,
  dtparam_t *param,
  dt_enter_arg_t *arg,
  char *msg,
  const char **start)
{
  DT_F("dttnote_arg_check");

  DT_Q(dtt_check_op,(ctl, param,        /* make sure op is understood       */
    ops, msg, start));

  return DT_RC_GOOD;
}

/*..........................................................................*/

DTT_TEST(dttnote)
{
  DT_F("dttnote");
  dt_ctl_t *ctl = &param->ctl;
  dt_enter_arg_t arg[1];
  int assert_fail = 0;
  dt_rc_e rc = DT_RC_GOOD;

  DT_GI(dt_heapmon_init,(ctl));         /* init memory checking             */

  DT_GI(dtparam_init,(param));          /* init param structure             */

  DT_GI(dtparam_set,(param, 0,          /* set default for params we want   */
    &param->op, NULL, NULL, "fast",
    &param->reps, NULL, NULL, "1",
    NULL));

  arg[0].desc = NULL;                   /* no arguments                     */
  
  DT_GI(dt_interactive_parse,(			/* parse command line interactively	*/
    argc, argv, F, USAGE, param,
    arg, dttnote_arg_check, NULL));
                                        /* ................................ */
  while (rc == DT_RC_GOOD)
  {
    long reps;
    for (reps=0;
         reps < param->reps &&
         rc == DT_RC_GOOD;
         reps++)
    {
      if (!strcmp(param->op, "all") ||
          !strcmp(param->op, "fast"))
      {
		DT_GI(dttnote_simple,(ctl, "10,1", 10, 1,
          &assert_fail));

        DT_GI(dttnote_longer,(ctl, " 9,2",  9, 2,
          &assert_fail));
        DT_GI(dttnote_longer,(ctl, "11,2", 11, 2,
          &assert_fail));
        DT_GI(dttnote_longer,(ctl, "29,2", 29, 2,
          &assert_fail));

        DT_GI(dttnote_append,(ctl, "11,2", 11, 2,
          &assert_fail));
        DT_GI(dttnote_append,(ctl, "29,2", 29, 2,
          &assert_fail));
      }
	}

    DT_G(dt_interactive_after,(         /* make post display                */
      ctl, param, USAGE,
      arg, dttnote_arg_check, NULL, rc));
  }

                                        /* ................................ */

  DT_I(dt_interactive_leave,(			/* clean up before leaving			*/
    param, rc));

  DT_I(dt_heapmon_uninit,(ctl,			/* check all memory is freed	    */
    &assert_fail));

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
