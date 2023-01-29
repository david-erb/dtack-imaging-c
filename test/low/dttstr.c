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

DT_RCSID("test $RCSfile: dttstr.c,v $ $Revision: 1.1 $");

#define USAGE "[-Dnnnn]"

static const char *ops[] = {"all",
  "fast",
  NULL};

/*..........................................................................*/

static
dt_rc_e
dttstr_test(
  dt_ctl_t *ctl,
  int *assert_fail)
{
  DT_F("dttstr_test");
  char tmp[256];
  void *p;

# define CHECK(S1, S2) \
  *assert_fail += DT_RC_GOOD != \
    dt_assert(ctl, \
      !strcmp(S1, S2), \
      F, "\"%s\" (\"%s\")", \
      S1, S2);

# define CHECK2(S1, S2, S3) \
  *assert_fail += DT_RC_GOOD != \
    dt_assert(ctl, \
      !strcmp(S1, S2) || !strcmp(S1, S3), \
      F, "\"%s\" (\"%s\" or \"%s\")", \
      S1, S2, S3);

  DT_Q(dtstr_printf,(ctl, tmp, 5, "%s", "longer than 5"));    
  CHECK(tmp, "long");

  DT_Q(dtstr_printf,(ctl, tmp, 4, "%dx%d", 3, 50));    
  CHECK(tmp, "3x5");

  DT_Q(dtstr_printf,(ctl, tmp, 4, "%d", 1234));    
  CHECK(tmp, "123");

  p = (void *)(0x12345678L);
  DT_Q(dtstr_printf,(ctl, tmp, 11, "0x%08lx", p));    
  CHECK(tmp, "0x12345678");

  DT_Q(dtstr_printf,(ctl, tmp, 3, "0x%08lx", p));    
  CHECK(tmp, "0x");

  DT_Q(dtstr_printf,(ctl, tmp, 8, "%-7d", 1234));    
  CHECK(tmp, "1234   ");

  DT_Q(dtstr_printf,(ctl, tmp, 20, "%junk"));    
  CHECK(tmp, "%junk");

  DT_Q(dtstr_printf,(ctl, tmp, 20, "%%d%d%%d", 3));    
  CHECK(tmp, "%d3%d");

  DT_Q(dtstr_printf,(ctl, tmp, 14, "%8.2e", 3.4567));    
  CHECK2(tmp, "3.46e+00", "3.46e+000");

  DT_Q(dtstr_printf,(ctl, tmp, 8, "%-7s", "abcd"));    
  CHECK(tmp, "   abcd");
  DT_Q(dtstr_printf,(ctl, tmp, 18, "%-17s", "abcd"));    
  CHECK(tmp, "             abcd");
  DT_Q(dtstr_printf,(ctl, tmp, 17, "%-17s", "abcd"));    
  CHECK(tmp, "             abc");

  DT_Q(dtstr_printf,(ctl, tmp, 9, "%-7sx", "abcd"));    
  CHECK(tmp, "   abcdx");
  DT_Q(dtstr_printf,(ctl, tmp, 19, "%-17sx", "abcd"));    
  CHECK(tmp, "             abcdx");
  DT_Q(dtstr_printf,(ctl, tmp, 19, "%-17sxx", "abcd"));    
  CHECK(tmp, "             abcdx");

  DT_Q(dtstr_printf,(ctl, tmp, 8, "%7s", "abcd"));    
  CHECK(tmp, "abcd   ");
  DT_Q(dtstr_printf,(ctl, tmp, 18, "%17s", "abcd"));    
  CHECK(tmp, "abcd             ");
  DT_Q(dtstr_printf,(ctl, tmp, 17, "%17s", "abcd"));    
  CHECK(tmp, "abcd            ");

  DT_Q(dtstr_printf,(ctl, tmp, 9, "%7sx", "abcd"));    
  CHECK(tmp, "abcd   x");
  DT_Q(dtstr_printf,(ctl, tmp, 19, "%17sx", "abcd"));    
  CHECK(tmp, "abcd             x");
  DT_Q(dtstr_printf,(ctl, tmp, 19, "%17sxx", "abcd"));    
  CHECK(tmp, "abcd             x");

  return DT_RC_GOOD;
}


/*..........................................................................*/
dt_rc_e
DTCONFIG_API1
dttstr_arg_check(						/* check args and params            */
  dt_ctl_t *ctl,
  void *app,
  dtparam_t *param,
  dt_enter_arg_t *arg,
  char *msg,
  const char **start)
{
  DT_F("dttstr_arg_check");

  DT_Q(dtt_check_op,(ctl, param,        /* make sure op is understood       */
    ops, msg, start));

  return DT_RC_GOOD;
}

/*..........................................................................*/

DTT_TEST(dttstr)
{
  DT_F("dttstr");
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
  
  DT_GI(dt_enter_parse,(				/* parse interactively				*/
    argc, argv, F, USAGE, param,
    arg, dttstr_arg_check, NULL));
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
		DT_GI(dttstr_test,(ctl, &assert_fail));
      }
	}
  }

                                        /* ................................ */

  DT_I(dt_leave,(param));				/* clean up before leaving			*/

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
