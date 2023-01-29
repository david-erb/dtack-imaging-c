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
DT_RCSID("test $RCSfile: dttliner.c,v $ $Revision: 1.2 $");
#include <dtack/list.h>
#include <dtack/liner.h>

#define USAGE "[-Dnnnn] [-op n]"

static const char *ops[] = {
  "default", "all",
  NULL};

#define COMMENT "#*;"
#define SEPARATORS ":"

static const char *filename1 = "dttline1.dat";
static const char *expect1[] = {
  "line 1",
  "  line 2",
  "",
  "",
  "xxxxxxxxxxxxxxxxxxxxxxxx looooong line 3 xxxxxxxxxxxxxxxxxxx"
  "yyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyy"
  "zzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzz"
  "wwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwww"
  "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
  "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
  "cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc",
  NULL
};

static const char *filename2 = "dttline2.dat";
static const char *expect2[] = {
  "no LF at end of file",
  NULL
};

static const char *filename3 = "dttline3.dat";
static const char *expect3[] = {
  "CR but no LF at end of file",
  NULL
};

										/* ................................ */
static const char *filename4a = "dttlin4a.dat";
#define S4A ":"
static const char *filename4b = "dttlin4b.dat";
#define S4B "=-@"

static const char *expect4[] = {
  "line1",
  "line 2",
  "line3",
  "line 4",
  "line    5",
  "line\t6",
  NULL
};
static const char *value4[] = {
  "value",
  "the value",
  "the  value",
  "the\t\tvalue",
  "",
  "",
  NULL
};
										/* ................................ */

static const char *filename5 = "dttlin5.dat";
#define S5 " "

static const char *expect5[] = {
  "line1",
  "line\t2",
  "line3\t\tthe",
  "line4",
  "line",
  "line\t6",
  NULL
};
static const char *value5[] = {
  "value",
  "the value",
  "value",
  "the\t\tvalue",
  "5",
  "",
  NULL
};
										/* ................................ */
static const char *filename6 = "dttlin6.dat";
#define S6 "\t"

static const char *expect6[] = {
  "line1",
  "line 2",
  "line3    the",
  "line 4",
  "line    5",
  "line",
  NULL
};
static const char *value6[] = {
  "value",
  "the value",
  "value",
  "the\t\tvalue",
  "",
  "6",
  NULL
};
										/* ................................ */
static const char *filename7 = "dttlin7.dat";
#define S7 " \t"

static const char *expect7 = 
  "-param1 value1 -param2 \"value  2\" -param3 -param4 \"value4\"\t4";

/*..........................................................................*/
static
dt_rc_e
dttliner_test1(
  dtparam_t *param,
  const char *filename,
  const char *expect[],
  int *assert_fail)
{
  DT_F("dttliner_test1");
  dt_ctl_t *ctl = &param->ctl;
  char path[256];
  dtfd_t fd;
  char *line = NULL;
  int n = 0;
  dt_rc_e rc = DT_RC_GOOD;

  fd.fd = -1;
  
  DT_C(dtstr_printf,(ctl, path,
    sizeof(path), "%s/%s",
    param->datadir, filename));

  DT_C(dtfd_open,(ctl, &fd, path, "r"));

  do {
	DT_C(dtliner_fd_line,(ctl, &fd,		/* get next line 					*/
      &line));
	if (line == NULL &&
		expect[n] == NULL)
	  break;
	else
	if (line == NULL)					/* file exhausted? 					*/
	{
      *assert_fail += DT_RC_GOOD !=
	  dt_assert(ctl, 0, F,
        "%s: expected \"%s\""
        " but got EOF",
        filename, expect[n]);
	  break;
	}
	else
	if (expect[n] == NULL)				/* expected no more? 				*/
	{
      *assert_fail += DT_RC_GOOD !=
	  dt_assert(ctl, 0, F,
        "%s: expected EOF"
        " but got \"%s\"",
        filename, line);
	  break;
	}
	if (strcmp(line, expect[n]))		/* didn't get the expected? 		*/
	{
      *assert_fail += DT_RC_GOOD !=
	  dt_assert(ctl, 0, F,
        "%s: expected \"%s\""
        " but got \"%s\"",
        filename, expect[n], line);
	  break;
	}
	if (line != NULL)
      DT_I(dtos_free2,(ctl, line,
        F, "line"));
	line = NULL;
	n++;
  } while (expect[n-1] != NULL);
  
  if (expect != NULL)					/* made it to the end? 				*/
    dt_assert(ctl, 1, F,
      "%s: got all %d lines",
      filename, n);

cleanup:
  if (fd.fd != -1)
    DT_I(dtfd_close,(ctl, &fd));

  if (line != NULL)
    DT_I(dtos_free2,(ctl, line,
      F, "line"));

  return rc;
}

/*..........................................................................*/
static
dt_rc_e
dttliner_test2(
  dtparam_t *param,
  const char *filename,
  const char *separators,
  const char *expect[],
  const char *value[],
  int *assert_fail)
{
  DT_F("dttliner_test2");
  dt_ctl_t *ctl = &param->ctl;
  char path[256];
  dtfd_t fd;
  dtlist_t list;
  int list_created = 0;
  dtlist_elem_t *elem;
  int n = 0;
  dt_rc_e rc = DT_RC_GOOD;

  fd.fd = -1;

  DT_C(dtlist_create,(ctl, &list,		/* create the list 					*/
    "list"));
  list_created = 1;

  DT_C(dtstr_printf,(ctl, path,			/* form the filename 				*/
    sizeof(path), "%s/%s",
    param->datadir, filename));

  DT_C(dtfd_open,(ctl, &fd, path,		/* open the file 					*/
    "r"));

  DT_C(dtliner_fd_list,(ctl, &fd,		/* make list from file 				*/
    COMMENT, separators, &list));
  
  DT_C(dtfd_close,(ctl, &fd));			/* close the file 					*/

  elem = DTLIST_ELEM_HEAD;
  do {
	DT_C(dtlist_next,(ctl, &list, elem,	/* get next line 					*/ 
      &elem));
	if (elem == NULL &&					/* expected end of list? 			*/
		expect[n] == NULL)
	  break;
	else
	if (elem == NULL)					/* list now exhausted? 				*/
	{
      *assert_fail += DT_RC_GOOD !=
	  dt_assert(ctl, 0, F,
        "%s: expected \"%s\""
        " but got EOF",
        filename, expect[n]);
	  break;
	}
	else
	if (expect[n] == NULL)				/* expected no more? 				*/
	{
      *assert_fail += DT_RC_GOOD !=
	  dt_assert(ctl, 0, F,
        "%s: expected EOF"
        " but got \"%s\"",
        filename, elem->name);
	  break;
	}
	if (strcmp(elem->name, expect[n]))	/* didn't get the expected name?	*/
	{
      *assert_fail += DT_RC_GOOD !=
	  dt_assert(ctl, 0, F,
        "%s: expected name \"%s\""
        " but got \"%s\"",
        filename, expect[n], elem->name);
	  break;
	}
	if (strcmp((char *)elem->user, value[n]))	/* didn't get the expected value?	*/
	{
      *assert_fail += DT_RC_GOOD !=
	  dt_assert(ctl, 0, F,
        "%s: expected value \"%s\""
        " but got \"%s\"",
        filename, value[n], elem->user);
	  break;
	}
	n++;
  } while (expect[n-1] != NULL);
  
  if (expect != NULL)					/* made it to the end? 				*/
    dt_assert(ctl, 1, F,
      "%s: got all %d lines",
      filename, n);

cleanup:
  if (fd.fd != -1)
    DT_I(dtfd_close,(ctl, &fd));

  if (list_created)
  {
	DT_I(dtlist_free,(ctl, &list));
	DT_I(dtlist_destroy,(ctl, &list));
  }

  return rc;
}

/*..........................................................................*/
static
dt_rc_e
dttliner_test3(
  dtparam_t *param,
  const char *filename,
  const char *separators,
  const char *expect,
  int *assert_fail)
{
  DT_F("dttliner_test3");
  dt_ctl_t *ctl = &param->ctl;
  char path[256];
  dtfd_t fd;
  char *string = NULL;
  dt_rc_e rc = DT_RC_GOOD;

  fd.fd = -1;

  DT_C(dtstr_printf,(ctl, path,			/* form the filename 				*/
    sizeof(path), "%s/%s",
    param->datadir, filename));

  DT_C(dtfd_open,(ctl, &fd, path,		/* open the file 					*/
    "r"));

  DT_C(dtliner_fd_string,(ctl, &fd,		/* make string from file 			*/
    COMMENT, separators, &string));
  
  DT_C(dtfd_close,(ctl, &fd));			/* close the file 					*/
  
  if (string == NULL)
  {
    *assert_fail += DT_RC_GOOD !=
	dt_assert(ctl, 0, F,
      "%s: expected \"%s\""
      " but got NULL",
      filename, expect);
  }
  if (strcmp(string, expect))			/* didn't get the expected string?	*/
  {
	*assert_fail += DT_RC_GOOD !=
	dt_assert(ctl, 0, F,
      "%s: expected \"%s\"",
      filename, expect);
	dt_assert(ctl, 0, F,
      "%s: but got  \"%s\"",
      filename, string);
  }
  else
	dt_assert(ctl, 1, F,
      "%s: got expected string",
      filename);

cleanup:
  if (string != NULL)
	DT_I(dtos_free2,(ctl, string,
      F, string));

  if (fd.fd != -1)
    DT_I(dtfd_close,(ctl, &fd));

  return rc;
}

/*..........................................................................*/
static
dt_rc_e
dttliner_line_test(
  dtparam_t *param,
  int *assert_fail)
{
  DT_F("dttliner_line_test");
  dt_ctl_t *ctl = &param->ctl;
  
  DT_Q(dttliner_test1,(param, filename1, expect1, assert_fail));
  DT_Q(dttliner_test1,(param, filename2, expect2, assert_fail));
  DT_Q(dttliner_test1,(param, filename3, expect3, assert_fail));
  
  DT_Q(dttliner_test2,(param, filename4a, S4A, expect4, value4, assert_fail));
  DT_Q(dttliner_test2,(param, filename4b, S4B, expect4, value4, assert_fail));
  DT_Q(dttliner_test2,(param, filename5,  S5,  expect5, value5, assert_fail));
  DT_Q(dttliner_test2,(param, filename6,  S6,  expect6, value6, assert_fail));
  
  DT_Q(dttliner_test3,(param, filename7,  S7,  expect7, assert_fail));
  return DT_RC_GOOD;
}

/*..........................................................................*/
dt_rc_e
DTCONFIG_API1
dttliner_arg_check(						/* check args and params            */
  dt_ctl_t *ctl,
  void *app,
  dtparam_t *param,
  dt_enter_arg_t *arg,
  char *msg,
  const char **start)
{
  DT_F("dttliner_arg_check");

  DT_Q(dtt_check_op,(ctl, param,        /* make sure op is understood       */
    ops, msg, start));

  if (msg[0] == '\0')					/* no problems yet? 				*/
  DT_Q(dtt_check_datadir,(ctl, param,	/* make sure datadir exists       	*/
    msg, start));

  return DT_RC_GOOD;
}

/*..........................................................................*/

DTT_TEST(dttliner)
{
  DT_F("dttliner");
  dt_ctl_t *ctl = &param->ctl;
  dt_enter_arg_t arg[1];
  int assert_fail = 0;                  /* failed assertion count           */
  dt_rc_e rc = DT_RC_GOOD;

  DT_GI(dt_heapmon_init,(ctl));         /* init memory checking             */

  DT_GI(dtparam_init,(param));          /* init param structure             */

  DT_GI(dtparam_set,(param, 0,          /* set default for params we want   */
    &param->op, NULL, NULL, "default",
    &param->datadir, NULL, NULL, ".",
    &param->reps, NULL, NULL, "1",
    &param->max, "Timing size", NULL, "100000",
    &param->n, "Timing loops", NULL, "5",
    NULL));

  arg[0].desc = NULL;                   /* no arguments                     */
  
  DT_GI(dt_enter_parse,(				/* parse command line				*/
    argc, argv, F, USAGE, param,
    arg, dttliner_arg_check, NULL));
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
          !strcmp(param->op, "default"))
      {
        DT_GI(dttliner_line_test,(param,
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
