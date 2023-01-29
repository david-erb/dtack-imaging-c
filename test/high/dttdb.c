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
  19990804 DE added to the suite of tests
 *..........................................................................*/

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
DT_RCSID("test $RCSfile: dttdb.c,v $ $Revision: 1.7 $");

#include <dtack/list.h>
#include <dtack/db.h>					/* database routines				*/

#define USAGE "[-Dnnnn]"

static const char *ops[] = {"all", "fast", "time", NULL};

/*..........................................................................
 *..........................................................................*/
static
dt_rc_e
dttdb_table1(
  dt_ctl_t *ctl,
  int *assert_fail)
{
  DT_F("dttdb_table1");
  dtdb_table_t table;
  dtdb_field_t *field;
  int i;
  long recordi;
  long fieldi;
  double d1, d2;
  dt_rc_e rc;

  DT_C(dtdb_table_init,(ctl,
    &table, "test1", ""));

  DT_C(dtdb_table_addfield,(ctl,
    &table, "field#1",
    DTDB_TYPE_UNKNOWN, 
    DTDB_FLAG_NOCOPY));

  DT_C(dtdb_table_addfield,(ctl,
    &table, "field#2",
    DTDB_TYPE_UNKNOWN, 0));

  DT_C(dtdb_table_addfield,(ctl,
    &table, "field#3",
    DTDB_TYPE_UNKNOWN, 0));

  *assert_fail +=
    DT_RC_GOOD != dt_assert(
      ctl, table.nfields == 3,
      F, "table.nfields=%ld",
      table.nfields);

										/* ................................ */
  DT_C(dtdb_table_findfield,(ctl,
    &table, "field#2", &field));

  *assert_fail +=
    DT_RC_GOOD != dt_assert(
      ctl, !strcmp(field->name, "field#2"),
      F, "field#2.name=%s",
      field->name);

										/* ................................ */
  DT_C(dtdb_table_searchfield,(ctl,
    &table, "field#X", &field));

  *assert_fail +=
    DT_RC_GOOD != dt_assert(
      ctl, field == NULL,
      F, "field#X.name=NULL");

										/* ................................ */
  DT_C(dtdb_table_getfield,(ctl,
    &table, 2, &field));

  *assert_fail +=
    DT_RC_GOOD != dt_assert(
      ctl, !strcmp(field->name, "field#3"),
      F, "field#3.name=%s",
      field->name);

										/* ................................ */
  DT_C(dtdb_table_addrecord,(ctl,
    &table, &recordi));

  *assert_fail +=
    DT_RC_GOOD != dt_assert(
      ctl, recordi == 0,
      F, "recordi=0",
      recordi);

  DT_C(dtdb_table_addrecord,(ctl,
    &table, &recordi));

  *assert_fail +=
    DT_RC_GOOD != dt_assert(
      ctl, recordi == 1,
      F, "recordi=1",
      recordi);

  *assert_fail +=
    DT_RC_GOOD != dt_assert(
      ctl, table.nrecords == 2,
      F, "table.nrecords=2",
      table.nrecords);

										/* ................................ */
  d1 = 1.23;
  DT_C(dtdb_table_setcell1,(ctl,
    &table, recordi, "field#1", 
    &d1, sizeof(d1)));

  d2 = 4.56;
  DT_C(dtdb_table_setcell1,(ctl,
    &table, recordi, "field#2", 
    &d2, sizeof(d2)));

  d2 = 7.89;
  DT_C(dtdb_table_setcell1,(ctl,
    &table, recordi, "field#2", 
    &d2, sizeof(d2)));

  DT_C(dtdb_table_getcell1,(ctl,
    &table, recordi, "field#2", 
    &d2, sizeof(d2), NULL));

  *assert_fail +=
    DT_RC_GOOD != dt_assert(
      ctl, 7.89 == d2,
      F, "7.89=%g",
      d2);

  d1 = 3.21;
  DT_C(dtdb_table_getcell1,(ctl,
    &table, recordi, "field#1", 
    &d1, sizeof(d1), NULL));

  *assert_fail +=
    DT_RC_GOOD != dt_assert(
      ctl, 3.21 == d1,
      F, "3.21=%g",
      d1);

										/* ................................ */
  d1 = 7.89;
  DT_C(dtdb_table_searchcell1,(ctl,
    &table, "field#1", 
    &d1, sizeof(d1), &recordi));

  *assert_fail +=
    DT_RC_GOOD != dt_assert(
      ctl, recordi = 1,
      F, "recordi=%d",
      recordi);
										/* ................................ */
  DT_C(dtdb_table_html_fd,(ctl,
    &table, NULL,
    NULL, NULL, NULL));

cleanup:
  DT_I(dtdb_table_uninit,(ctl,
    &table));
    
  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/
static
dt_rc_e
dttdb_table2_compare(
  dt_ctl_t *ctl,
  dtdb_table_t *table,
  const char *id,
  long *indices,
  int n,
  int *assert_fail)
{
  DT_F("dttdb_table2_compare");
  int i;
  dt_rc_e rc = DT_RC_GOOD;

  for (i=0; i<n; i++)
  {
	long j;
	DT_C(dtdb_table_getcell1,(ctl,
      table, i, "index", 
      &j, sizeof(j), NULL));

	*assert_fail +=
      DT_RC_GOOD != dt_assert(
        ctl, j == indices[i],
        F, "%s indices[%d]=%d index=%ld",
        id, i, indices[i], j);
  }

cleanup:
    
  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/
static
dt_rc_e
dttdb_table2(
  dt_ctl_t *ctl,
  int *assert_fail)
{
  DT_F("dttdb_table2");
  dtdb_table_t table;
  dtdb_field_t *field;
  int i;
#define N 5
  const char *s[N] = {
    "a",
    "b",
    "aa",
    NULL,
    "aa"
  };
  double d[N] = {
    1,
    2,
    0,
    -1,
    1,
  };
  long l[N] = {
    0,
    2,
    1,
    -1,
    1,
  };

  dt_rc_e rc;

  DT_C(dtdb_table_init,(ctl,
    &table, "test1", ""));

  DT_C(dtdb_table_addfield,(ctl,
    &table, "index",
    DTDB_TYPE_LONG, 0));

  DT_C(dtdb_table_addfield,(ctl,
    &table, "field#1",
    DTDB_TYPE_STRING, 0));

  DT_C(dtdb_table_addfield,(ctl,
    &table, "field#2",
    DTDB_TYPE_LONG, 0));

  DT_C(dtdb_table_addfield,(ctl,
    &table, "field#3",
    DTDB_TYPE_DOUBLE, 0));

										/* ................................ */
  
  for (i=0; i<N; i++)
  {
	long recordi;
	DT_C(dtdb_table_addrecord,(ctl,
      &table, &recordi));
	DT_C(dtdb_table_setcell1,(ctl,
      &table, i, "index",  &i, -1));
	if (s[i])
    DT_C(dtdb_table_setcell1,(ctl,
      &table, recordi, "field#1",
      s[i], -1));
	DT_C(dtdb_table_setcell1,(ctl,
      &table, recordi, "field#2",
      &l[i], -1));
	DT_C(dtdb_table_setcell1,(ctl,
       &table, recordi, "field#3",
      &d[i], -1));
  }
										/* ................................ */
  {
	const char *id = "descending string sort";
	long indices[N] = {1,2,4,0,3};
	DT_C(dtdb_table_sortcell1,(ctl,
      &table, "field#1", 0));

	DT_C(dttdb_table2_compare,(ctl,
      &table, id, indices, N,
      assert_fail));
  }

  {
	const char *id = "ascending string sort";
	long indices[N] = {3,0,4,2,1};
	DT_C(dtdb_table_sortcell1,(ctl,	
      &table, "field#1", 1));

	DT_C(dttdb_table2_compare,(ctl,
      &table, id, indices, N,
      assert_fail));
  }

  {
	const char *id = "ascending double sort";
	long indices[N] = {3,2,4,0,1};
	DT_C(dtdb_table_sortcell1,(ctl,	
      &table, "field#3", 1));

	DT_C(dttdb_table2_compare,(ctl,
      &table, id, indices, N,
      assert_fail));
  }

  {
	const char *id = "descending double sort";
	long indices[N] = {1,4,0,2,3};
	DT_C(dtdb_table_sortcell1,(ctl,	
      &table, "field#3", 0));

	DT_C(dttdb_table2_compare,(ctl,
      &table, id, indices, N,
      assert_fail));
  }

  DT_C(dtdb_table_html_fd,(ctl,
    &table, 
    NULL, NULL, NULL, 
    NULL));

cleanup:
  DT_I(dtdb_table_uninit,(ctl,
    &table));
    
  return DT_RC_GOOD;
}

/*..........................................................................*/
dt_rc_e
DTCONFIG_API1
dttdb_arg_check(						/* check args and params            */
  dt_ctl_t *ctl,
  void *app,
  dtparam_t *param,
  dt_enter_arg_t *arg,
  char *msg,
  const char **start)
{
  DT_F("dttdb_arg_check");

  DT_Q(dtt_check_op,(ctl, param,        /* make sure op is understood       */
    ops, msg, start));

  return DT_RC_GOOD;
}

/*..........................................................................*/

DTT_MAIN(dttdb)
{
  DT_F("dttdb");
  dt_ctl_t *ctl = &param->ctl;
  int assert_fail = 0;					/* failed assertion count		    */
  dt_enter_arg_t arg[1];
  dt_rc_e rc = DT_RC_GOOD;

  DT_C(dt_heapmon_init,(ctl));			/* init memory checking			    */

  DT_C(dtparam_init,(param));			/* init param structure             */

  DT_C(dtparam_set,(param, 0,			/* set default for params we want   */
    &param->op, NULL, NULL, "fast",
    NULL));

  arg[0].desc = NULL;					/* no arguments 					*/
  
  DT_C(dt_interactive_parse,(			/* parse command line interactively	*/
    argc, argv, F, USAGE, param,
    arg, dttdb_arg_check, NULL));

  if (!strcmp(param->op, "all") ||
	  !strcmp(param->op, "fast"))
  {
	DT_C(dttdb_table1,(ctl, &assert_fail));
	DT_C(dttdb_table2,(ctl, &assert_fail));
  }

cleanup:
  DT_I(dt_interactive_leave,(
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
