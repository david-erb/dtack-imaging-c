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
#include <dtack/mem.h>
#include <dtack/stack.h>
#include <dtack/prj.h>

DT_RCSID("test $RCSfile: dttstack.c,v $ $Revision: 1.1 $");

#define USAGE "[-Dnnnn] [-op n]"

static const char *ops[] = {"all",
  "default", "all", "small", "ram", "ems", "time",
  NULL};

typedef long el_t;
#define ELSIZE ((int)(sizeof(el_t)))

#define V(I) ((long)(I)*10L)
#define U(I) ((long)(I)*10L+1L)

/*..........................................................................*/

static
dt_rc_e
dttstack_check(
  dt_ctl_t *ctl,
  dtstack_t *stack,
  long nels,
  int do_overflow,
  int *assert_fail,
  char *desc)
{
  static char *F = "dttstack_check";
  int bad;
  long i;
  el_t *el;
  dtstack_code_e code;
  long expect;

  if (do_overflow)						/* supposed to do overflow check?	*/
  {
    DTSTACK_PUSH(DT_Q, ctl, stack,		/* push on the overflow			    */
      (void **)&el, &code);

    *assert_fail += DT_RC_GOOD !=
      dt_assert(ctl,
        code == DTSTACK_CODE_OVERFLOW, F,
        "%s overflow at nels=%ld (got code %d)",
            desc, nels, code);
  }

  code = DTSTACK_CODE_OK;
  bad = 0;
  for (i=nels-1; i>=0; i--)				/* check poking/peeking/popping	    */
  {
    DTSTACK_POKE(DT_Q, ctl, stack,
      i, (void **)&el);
    if (*el != V(i))
    {
      *assert_fail += DT_RC_GOOD !=
        dt_assert(ctl, 0, F,
          "%s poke %ld %ld (%ld)",
          desc, i, *el, V(i));
      bad++;
      if (bad > 10) break;
    }

    *el = U(i);							/* change value while we poke	    */

    DTSTACK_PEEK(DT_Q, ctl, stack,
      nels-i-1, (void **)&el);
    if (nels-i-1 < i)					/* crossover point?				  	*/
      expect = V(nels-i-1);
    else
      expect = U(nels-i-1);
    if (*el != expect)
    {
      *assert_fail += DT_RC_GOOD !=
        dt_assert(ctl, 0, F,
          "%s peek %ld %ld (%ld)",
          desc, nels-i-1, *
          el, expect);
      bad++;
      if (bad > 10) break;
    }

    DTSTACK_POP(DT_Q, ctl, stack,
      (void **)&el, &code);
    if (code != DTSTACK_CODE_OK)
      return dt_err(ctl, F,
        "%s bad pop %ld code %d",
        desc, i, code);
    if (*el != U(i))
    {
      *assert_fail += DT_RC_GOOD !=
        dt_assert(ctl, 0, F,
          "%s pop %ld %ld (%ld)",
          desc, i, *el, U(i));
      bad++;
      if (bad > 10) break;
    }
  }
  if (!bad)
    dt_assert(ctl, 1, F,
      "%s poke/peek/pop %ld out of %ld",
      desc, nels, nels);

  DTSTACK_POP(DT_Q, ctl, stack,			/* pop the underflow				*/
    (void **)&el, &code);

  *assert_fail += DT_RC_GOOD !=
    dt_assert(ctl,
      code == DTSTACK_CODE_UNDERFLOW, F,
      "%s underflow at %ld (got code %d)",
      desc, i, code);

  return DT_RC_GOOD;
}

/*..........................................................................*/

static
dt_rc_e
dttstack_exercise(
  dt_ctl_t *ctl,
  dtstack_t *stack,
  long nels,
  int do_overflow,
  const char *tmpdir,				  
  int *assert_fail,
  char *F,
  char *desc)
{
  long i;
  el_t *el;
  dtstack_code_e code;
  char desc2[64];
  char filename[80];
  dtfd_t fd;
  int eof;
  dt_rc_e rc;

  DT_MEMSET(&fd, 0, sizeof(fd));

  code = DTSTACK_CODE_OK;
  for (i=0; i<nels; i++)				/* push the stack				    */
  {
    DTSTACK_PUSH(DT_Q, ctl, stack,
      (void **)&el, &code);
    if (code != DTSTACK_CODE_OK)
      return dt_err(ctl, F,
        "%s bad push %d code %d",
        desc, i, code);
    *el = V(i);
  }

  DT_C(dtstr_printf,(ctl,
    filename, sizeof(filename),
    "%s/dttstack.tmp", tmpdir));

										/* ................................ */
  DT_C(dtfd_open,(ctl, &fd,
    filename, "w"));
  DT_C(dtstack_write,(ctl, stack,		/* write the stack				    */
    &fd));
  DT_C(dtfd_close,(ctl, &fd));

  DT_C(dtstr_printf,(ctl, desc2,
    sizeof(desc2), "%s after push",
    desc));
  DT_C(dttstack_check,(ctl, stack,
    nels, do_overflow,
    assert_fail, desc2));
										/* ................................ */
  DT_C(dtfd_open,(ctl, &fd,
    filename, "r"));
  DT_C(dtstack_read,(ctl, stack,		/* read back the whole stack	    */
    nels, &fd, &code, &eof));
  DT_C(dtfd_close,(ctl, &fd));
  DT_C(dtstr_printf,(ctl, desc2,
    sizeof(desc2), 
    "%s after limited full read",
    desc));
  *assert_fail += DT_RC_GOOD !=
    dt_assert(ctl, 
      code == DTSTACK_CODE_OK &&
      eof == 0, F,
      "%s code %d (%d), eof %d (%d)", 
      desc2, code, DTSTACK_CODE_OK,
      eof, 0);
  DT_C(dttstack_check,(ctl, stack,
    nels, do_overflow,
    assert_fail, desc2));
										/* ................................ */
  DT_C(dtfd_open,(ctl, &fd,
    filename, "r"));
  DT_C(dtstack_read,(ctl, stack,		/* read back the whole file		    */
    0, &fd, &code, &eof));
  DT_C(dtfd_close,(ctl, &fd));
  DT_C(dtstr_printf,(ctl, desc2,
    sizeof(desc2), 
    "%s after unlimited full read",
    desc));
  if (stack->offset_max)
    *assert_fail += DT_RC_GOOD !=
      dt_assert(ctl, 
        code == DTSTACK_CODE_OK &&
        eof == 0, F,
        "%s code %d (%d), eof %d (%d)", 
        desc2, code, DTSTACK_CODE_OK,
        eof, 0);
  else
    *assert_fail += DT_RC_GOOD !=
      dt_assert(ctl, 
        code == DTSTACK_CODE_OK &&
        eof == 1, F,
        "%s code %d (%d), eof %d (%d)", 
        desc2, code, DTSTACK_CODE_OK,
        eof, 1);
  DT_C(dttstack_check,(ctl, stack,
    nels, 0,
    assert_fail, desc2));
										/* ................................ */


cleanup:
  DT_I(dtstack_free,(ctl, stack, F));	/* free the stack				    */

  {
    int do_unlink = 0;
    if (fd.filename)
      do_unlink = 1;
    if (fd.file)
      DT_I(dtfd_close,(ctl, &fd));
    if (do_unlink)
      DT_I(dtos_unlink,(ctl, 
        filename));
  }

  return rc;
}

/*..........................................................................*/
static
dt_rc_e
dttstack_paged(
  dt_ctl_t *ctl,
  char *configure,
  long limit,
  int mask,
  const char *tmpdir,
  int *assert_fail)
{
  DT_F("dttstack_paged");
  void *memsource = NULL;
  void *memalloc = NULL;
  dtstack_t stack;
  char desc[64];
  int i;
  dt_rc_e rc;

  if (configure)						/* caller is configuring?		    */
  {
    memsource = ctl->memsource;
    ctl->memsource = NULL;
    DT_C(dtmem_configure,(ctl,			/* set up just the config we want	*/
      configure));
  }
  else
    configure = "default";

  if (mask & 0xf0)						/* preallocating?				    */
    DT_C(dtmem_alloc,(ctl,
      &memalloc, 0, 0,  
      (unsigned int)limit, ELSIZE,
      F, "prealloc"))

  for (i=0; i<8; i++)
  {
    int type = (0x80 >> i) & mask;
    if (type == 0)
      continue;
    
    DT_Q(dtstr_printf,(ctl, 
      desc, sizeof(desc), 
      "%s type 0x%02x",
      configure, type));

    if (type & 0x80)					/* preallocating?					*/
      DT_C(dtstack_prealloc,(ctl,		/* preallocate the stack		    */
        &stack, ELSIZE, 
        memalloc, 
        limit, F));

    if (type & 0x08)					/* giving initial and maximum?	    */
      DT_C(dtstack_init,(ctl,			/* init the stack				    */
        &stack, ELSIZE, 
        limit, 
        limit, F));

    if (type & 0x04)					/* giving initial but no maximum?   */
      DT_C(dtstack_init,(ctl,			/* init the stack				    */
        &stack, ELSIZE, 
        limit, 
        0, F));

    if (type & 0x02)					/* giving maximum but no initial?   */
      DT_C(dtstack_init,(ctl,			/* init the stack				    */
        &stack, ELSIZE, 
        limit > 10? 0: 1,				/* this causes reallocs			    */
        limit, F));

    if (type & 0x01)					/* not giving initial or maximum?   */
      DT_C(dtstack_init,(ctl,			/* init the stack				    */
        &stack, ELSIZE, 
        0,
        0, F));

    DT_C(dttstack_exercise,(ctl,		/* exercise the stack			    */
      &stack,						
      limit, type & 0xaa, tmpdir,
      assert_fail,
      F, desc));
  }

cleanup:
  if (memalloc)							/* preallocated?				    */
    DT_I(dtmem_free,(ctl,
      memalloc, F, "prealloc"));

  if (memsource)
  {
    DT_I(dtmem_unconfigure,(ctl));		/* release the config we used	    */
    ctl->memsource = memsource;
  }
  return rc;
}

/*..........................................................................*/
static
dt_rc_e
dttstack_small(
  dt_ctl_t *ctl,
  const char *tmpdir,
  int *assert_fail)
{
  DT_F("dttstack_ram");

  DT_Q(dttstack_paged,(ctl,	
      NULL, 10, 0x8f, tmpdir, assert_fail));

  return DT_RC_GOOD;
}

/*..........................................................................*/
static
dt_rc_e
dttstack_ram(
  dt_ctl_t *ctl,
  const char *tmpdir,
  int *assert_fail)
{
  DT_F("dttstack_ram");
  char configure[64];

  sprintf(configure, "ram,,%d,1",		/* all els in one page				*/
    ELSIZE*10);
  DT_Q(dttstack_paged,(ctl,	
      configure, 10, 0x8c, tmpdir, assert_fail));

  sprintf(configure, "ram,,%d,10",		/* one el per page					*/
    ELSIZE);
  DT_Q(dttstack_paged,(ctl,	
      configure, 10, 0x0c, tmpdir, assert_fail));

  sprintf(configure, "ram,,%d,5",		/* two els per page					*/
    ELSIZE*2);
  DT_Q(dttstack_paged,(ctl,	
      configure, 10, 0x0c, tmpdir, assert_fail));

  return DT_RC_GOOD;
}

/*..........................................................................*/
static
dt_rc_e
dttstack_ems(
  dt_ctl_t *ctl,
  const char *tmpdir,
  int *assert_fail)
{
  DT_F("dttstack_ems");
  char configure[64];
#ifdef NONO
  sprintf(configure, "ems,,%d,1",		/* all els in one page				*/
    ELSIZE*10);
  DT_Q(dttstack_paged,(ctl,	
      configure, 10, 0x8f, tmpdir, assert_fail));
#endif
  sprintf(configure, "ems,,%d,1",		/* one el per page					*/
    ELSIZE);
  DT_Q(dttstack_paged,(ctl,	
      configure, 10, 0x0f, tmpdir, assert_fail));

  sprintf(configure, "ems,,%d,1",		/* two els per page					*/
    ELSIZE*2);
  DT_Q(dttstack_paged,(ctl,	
      configure, 10, 0x0f, tmpdir, assert_fail));

  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/

static
dt_rc_e
dttstack_time(
  dt_ctl_t *ctl,
  long max,
  long loops,
  const char *tmpdir,			   
  int *assert_fail)
{
  DT_F("dttstack_time");
  dtos_time_t mark1, mark2;
  long loop;
  char f[64];
#ifdef NONO
  DT_Q(dtos_time,(ctl, &mark1));
  for (loop=0; loop<loops; loop++)
  {
    DT_Q(dttstack_paged,(ctl,			/* no reallocations				    */
      NULL, max, 0x08, 
      tmpdir, assert_fail));
  }
  DT_Q(dtos_time,(ctl, &mark2));

  sprintf(f, "%ld type 8 (%2ld)",	
    max, loops);
  dtt_time_report(ctl,
    &mark1, &mark2, f);
#endif
										/* ................................ */
  DT_Q(dtos_time,(ctl, &mark1));
  for (loop=0; loop<loops; loop++)
  {
    DT_Q(dttstack_paged,(ctl,			/* realloc up to given max		    */
      NULL, max, 0x02, 
      tmpdir, assert_fail));
  }
  DT_Q(dtos_time,(ctl, &mark2));

  sprintf(f, "%ld type 2 (%2ld)",	
    max, loops);
  dtt_time_report(ctl,
    &mark1, &mark2, f);
										/* ................................ */

  return DT_RC_GOOD;    
}

/*..........................................................................*/
dt_rc_e
DTCONFIG_API1
dttstack_arg_check(						/* check args and params            */
  dt_ctl_t *ctl,
  void *app,
  dtparam_t *param,
  dt_enter_arg_t *arg,
  char *msg,
  const char **start)
{
  DT_F("dttstack_arg_check");

  DT_Q(dtt_check_op,(ctl, param,        /* make sure op is understood       */
    ops, msg, start));

  return DT_RC_GOOD;
}

/*..........................................................................*/

DTT_TEST(dttstack)
{
  DT_F("dttstack");
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
    &param->output, "Tmp dir", NULL, "/tmp",
    NULL));

  arg[0].desc = NULL;                   /* no arguments                     */
  
  DT_GI(dt_enter_parse,(				/* parse command line				*/
    argc, argv, F, USAGE, param,
    arg, dttstack_arg_check, NULL));
                                        /* ................................ */
  {
    long reps;
    for (reps=0;
         reps < param->reps &&
         rc == DT_RC_GOOD;
         reps++)
    {

      if (!strcmp(param->op, "all") ||
          !strcmp(param->op, "default") ||
          !strcmp(param->op, "small"))
      {
        DT_GI(dttstack_small,(ctl,
          param->output, &assert_fail));
      }

      if (!strcmp(param->op, "all") ||
          !strcmp(param->op, "default") ||
          !strcmp(param->op, "ram"))
      {
        DT_GI(dttstack_ram,(ctl,
          param->output, &assert_fail));
      }

      if (((DT_ABBREV("all", param->op) ||
            DT_ABBREV("default", param->op)) &&
           dtprj_emsif) ||
          DT_ABBREV("ems", param->op))
      {
        DT_GI(dttstack_ems,(ctl,
          param->output, &assert_fail));
	  }
    
      if (!strcmp(param->op, "all") ||
          !strcmp(param->op, "time"))
      {
        DT_GI(dttstack_time,(ctl,
          (long)param->max, param->n,
          param->output, &assert_fail));
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
