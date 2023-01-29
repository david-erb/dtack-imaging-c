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
DT_RCSID("test $RCSfile: dttmap.c,v $ $Revision: 1.6 $");
#include <dtack/keymap.h>

#define USAGE "[-Dnnnn]"

static const char *ops[] = {"all",
  "default",
  NULL};

/*..........................................................................*/
dt_rc_e
dttmap_match(
  dt_ctl_t *ctl,
  dtkeymap_t *keymap,
  const char *string,
  dt_key_t want,
  int *assert)
{
  DT_F("dttmap_match");
  int left;
  dt_key_t key;
  int partial;
  char s[64];
  char want_str[64];
  char key_str[64];

  strcpy(s, string);
  DT_Q(dtkeymap_match,(ctl,
    keymap, 
    s, strlen(string),
    &left, &key, &partial));

  DT_Q(dtkeymap_name_str,(ctl,
    keymap, want, 
    want_str, sizeof(want_str)));

  DT_Q(dtkeymap_name_str,(ctl,
    keymap, key, 
    key_str, sizeof(key_str)));

  assert += DT_RC_GOOD != 
  dt_assert(ctl,
    left == 0 && 
    key == want &&
    partial == 0, F,
    "%s -> %s (%s), left %d (0), partial %d (0)",
    string, want_str, key_str,
    left, partial);

  return DT_RC_GOOD;
}

/*..........................................................................*/
dt_rc_e
dttmap_test(
  dt_ctl_t *ctl,
  int *assert)
{
  DT_F("dttmap_test");
  dtkeymap_t keymap;
  int have_keymap = 0;
  typedef struct {
    dt_key_t key;
    char string[8];
  } map_t;
  int i;

  static map_t entry[4] = {
    {DT_KEY_F8,         {'x', 'y', 0}},
    {DT_KEY_F9,         {'1', 0}},
    {DT_KEY_F8,         {'y', 0}},
    {0}};

  dt_rc_e rc;

  DT_C(dtkeymap_init,(ctl,
    &keymap, "none", 0));
  have_keymap = 1;

  for (i=0; entry[i].key; i++)
    DT_C(dtkeymap_map,(ctl,
      &keymap, entry[i].key,
      entry[i].string, 0));

  for (i=0; entry[i].key; i++)
    DT_C(dttmap_match,(ctl,
      &keymap, entry[i].string, 
      entry[i].key, assert));

cleanup:
  if (have_keymap)
    DT_I(dtkeymap_uninit,(ctl, &keymap));
  return rc;
}

/*..........................................................................*/
dt_rc_e
dttmap_file(
  dt_ctl_t *ctl,
  const char *testdata,
  int *assert)
{
  DT_F("dttmap_file");
  dtkeymap_t keymap;
  int have_keymap = 0;
  dt_rc_e rc;

  DT_C(dtkeymap_init,(ctl,
    &keymap, testdata, 0));
  have_keymap = 1;

cleanup:
  if (have_keymap)
    DT_I(dtkeymap_uninit,(ctl, &keymap));
  return rc;
}

/*..........................................................................*/
static
dt_rc_e 
dttmap_arg_check(						/* check args and params			*/
  dt_ctl_t *ctl,
  void *app,
  dtparam_t *param,
  dt_enter_arg_t *arg,
  char *msg,
  const char **start)
{
  DT_F("dttmap_arg_check");

  DT_Q(dtt_check_op,(ctl, param,		/* make sure op is understood	    */
    ops, msg, start));

  return DT_RC_GOOD;
}

/*..........................................................................*/

DTT_TEST(dttmap)
{
  DT_F("dttmap");
  dt_ctl_t *ctl = &param->ctl;
  dt_enter_arg_t arg[1];
  char testdata[128];
  int assert_fail = 0;					/* failed assertion count		    */
  dt_rc_e rc;

  DT_C(dt_heapmon_init,(ctl));			/* init memory checking			    */

  DT_C(dtparam_init,(param));			/* init param structure             */

  DT_C(dtparam_set,(param, 0,			/* set default for params we want	*/
    &param->op, NULL, NULL, "default",
    &param->datadir, NULL, NULL, NULL,
    &param->input, NULL, NULL, NULL,
    &param->reps, NULL, NULL, "1",
    NULL));

  arg[0].desc = NULL;					/* no arguments					    */
  DT_C(dt_enter_parse,(					/* go parse params and args			*/
    argc, argv, F, USAGE, param,
    arg, dttmap_arg_check, NULL));

  DT_C(dtstr_printf,(ctl, testdata,
    sizeof(testdata), "%s/%s",
    param->datadir, 
    param->input? param->input:
    "keymap.dat"));
										/* ................................ */
  {
    long reps;
    for (reps=0; 
         reps < param->reps && 
         rc == DT_RC_GOOD;
         reps++)
	{
      if (!strcmp(param->op, "all") ||
          !strcmp(param->op, "default"))
      {
        DT_C(dttmap_test,(ctl,
          &assert_fail));
        DT_C(dttmap_file,(ctl,
          testdata,
          &assert_fail));
      }
	}
  }

										/* ................................ */
cleanup:
  DT_I(dt_leave,(param));				/* clean up before leaving			*/

  DT_I(dt_heapmon_uninit,(ctl,			/* check all memory is freed	    */
    &assert_fail));

  if (assert_fail)						/* any failed assertions?		    */
    return DT_RC_BAD;
  else
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
