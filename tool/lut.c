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
| dttool_lut utility - tell win server to stop
| 
| DESCRIPTION
| As yet undocumented.
| 
| RETURN VALUES
| Program exit value will be DT_EXIT_GOOD on success.  
| Any other exit value indicates failure or noncompletion.
| 
| ERRORS
| As yet undocumented.
| 
| END
 *..........................................................................*/

#include <dttool.h>

DT_RCSID("tool $RCSfile: lut.c,v $ $Revision: 1.9 $");

#include <dtack/lut8.h>
#include <dtack/os.h>

#define USAGE \
  "[-op get,set,read] [other params] windowname [args]\n" \
  "  args are: index r g b  OR  filename start-index"

static const char *usage = USAGE;

# define OP_GET     "get"
# define OP_SET1    "set1"
# define OP_READ    "read"
# define OP_DEFAULT OP_GET

static const char *ops[] = {
  OP_GET,
  OP_SET1,
  OP_READ,
  NULL};

/*..........................................................................*/

dt_rc_e 
DTCONFIG_API1
dttool_lut_cmd_check(					/* check params for this program	*/
  dt_ctl_t *ctl,
  void *appstruct,
  dtparam_t *param,
  dt_enter_arg_t *arg,
  char *msg,
  const char **start)
{
  DT_F("dttool_lut_cmd_check");
  int op;
  int i;
  int n;

  DT_Q(dtstr_choice,(ctl, ops,			/* make sure op is understood	    */
    param->op, &op));

  if (op == -1)
  {
    sprintf(msg,
      "Sorry, op \"%s\" is not understood", 
      param->op);
    *start = param->op_desc;			/* put cursor at this field		    */
    return DT_RC_GOOD;
  }

  if (op < -1)
  {
    sprintf(msg,
      "Sorry, op \"%s\" is ambiguous",
      param->op);
    *start = param->op_desc;			/* put cursor at this field		    */
    return DT_RC_GOOD;
  }

  if (DT_ABBREV(OP_GET, ops[op]))
    n = 2;
  else
  if (DT_ABBREV(OP_SET1, ops[op]))
    n = 4;
  else
  if (DT_ABBREV(OP_READ, ops[op]))
    n = 2;
  for (i=0; i<n; i++)
  {
    int l = strlen(arg[i].val);
    int j;

    for (j=0; j<l; j++)					/* search value for non-blank	    */
      if (arg[i].val[j] != ' ')
        break;
    if (j == l)
    {
      sprintf(msg, 
        "Please enter something for \"%s\"",
        arg[i].desc);
      *start = arg[i].val;				/* position cursor on this field    */
      return DT_RC_GOOD;
    }
  }

  if (DT_ABBREV(OP_READ, ops[op]))
    n = 1;

  for (i=1; i<n; i++)
  {
	int isnumber;
	long number;
    DT_Q(dtstr_to_long2,(ctl, arg[i].val,
      &number, &isnumber));

    if (!isnumber)
    {
      sprintf(msg,
        "Sorry, \"%s\" is not a number", 
        arg[i].val);
      *start = arg[i].desc;			/* put cursor at this field		    */
	  return DT_RC_GOOD;
	}
  }

  return DT_RC_GOOD;
}
/*..........................................................................*/
dt_rc_e 
dttool_lut_get(
  dt_ctl_t *ctl,			  
  const char * windowname,
  long start,
  long count)
{
  DT_F("dttool_lut_get");
  dtlut8_t lut[256];

  DT_Q(dtlut8_import,(ctl,
    lut, windowname,
    (int)start, (int)count));

  return DT_RC_GOOD;
}
/*..........................................................................*/
dt_rc_e 
dttool_lut_set1(
  dt_ctl_t *ctl,			  
  const char *windowname,
  long index,
  long r,
  long g,
  long b)
{
  DT_F("dttool_lut_set1");
  dtlut8_t lut[1];

  lut[0].r = (unsigned char)r;
  lut[0].g = (unsigned char)g;
  lut[0].b = (unsigned char)b;
  DT_Q(dtlut8_export,(ctl,
    lut, windowname, (int)index, 1));

  return DT_RC_GOOD;
}
/*..........................................................................*/
dt_rc_e 
dttool_lut_set(
  dt_ctl_t *ctl,			  
  const char *windowname,
  dtlut8_t *lut,
  int start,
  int count)
{
  DT_F("dttool_lut_set");
  DT_Q(dtlut8_export,(ctl,
    lut, windowname, start, count));
  return DT_RC_GOOD;
}
/*..........................................................................*/
dt_rc_e 
dttool_lut_read(
  dt_ctl_t *ctl,			  
  const char *windowname,
  const char *lutname,
  long start)
{
  DT_F("dttool_lut_read");
  dtimage_t lutimage;
  static dt_ntype_e ntypes[] =
    {DT_NTYPE_B08, DT_NTYPE_NULL};
  dt_rc_e rc;

  lutimage.xe = 0;
  
  DT_C(dtimage_import,(ctl, &lutimage,
    lutname));

  rc = dtimage_check_1f(ctl,			/* tracer						    */
    &lutimage, "lutimage",
    ntypes, 0, F);

  if (rc == DT_RC_GOOD)
  if (lutimage.xe > 256)
    rc = dt_err(ctl, F,
      "\"%s\" width %u"
      " too wide to be a lut",
      lutname, lutimage.xe);
  else
  if (lutimage.ye != 3)					/* image must have 3 rows to be lut	*/
    rc = dt_err(ctl, F,
      "\"%s\" row count %u"
      " not 3 as expected for a lut",
      lutname, lutimage.ye);
  else
  {
	dtlut8_t lut[256];
	int i;
	DT_MEMSET(lut, 0, sizeof(lut));
	for (i=0; i<lutimage.xe; i++)
	{
	  lut[i].r = lutimage.row[0].b08[i];
	  lut[i].g = lutimage.row[1].b08[i];
	  lut[i].b = lutimage.row[2].b08[i];
	}
	DT_C(dttool_lut_set,(ctl, 
      windowname, lut,
      DT_CAST(int, start),
      DT_CAST(int, 
        DT_MIN(256-start, 
          lutimage.xe))));
  }
  
cleanup:
  if (lutimage.xe)
    DT_I(dtimage_free,(ctl, &lutimage));

  return rc;
}

/*..........................................................................*/

DTTOOL_MAIN(dttool_lut)
{
  dt_ctl_t *ctl = &param->ctl;
# define ARG_MAXLEN 48
  char windowname[ARG_MAXLEN];
  char index[ARG_MAXLEN];
  char r[ARG_MAXLEN];
  char g[ARG_MAXLEN];
  char b[ARG_MAXLEN];
  dt_enter_arg_t arg[6];
  dt_rc_e rc = DT_RC_GOOD;

  DT_GI(dtparam_init,(param));			/* init param structure             */

  DT_GI(dtparam_set,(param, 
    DTPARAM_FLAG_PRIMARY,
    &param->op,         NULL,                    NULL, OP_DEFAULT,
    &param->n,          NULL,                    NULL, "16",
    NULL));

  DT_GI(dt_enter_arg,(ctl,				/* set up positionals			    */
    arg,
    "Window Name", NULL, 
      windowname, sizeof(windowname),
    "Index", NULL, 
      index, sizeof(index),
    "R", NULL, 
      r, sizeof(r),
    "G", NULL, 
      g, sizeof(g),
    "B", NULL, 
      b, sizeof(b),
    NULL));

  DT_GI(dt_interactive_parse,(			/* parse command line interactively	*/
    argc, argv, F, usage, param,
    arg, dttool_lut_cmd_check, NULL));

										/* ................................ */
  while (rc == DT_RC_GOOD)
  {
	int op;
	long lindex;

	DT_Q(dtstr_choice,(ctl, ops,
      param->op, &op));

	if (DT_ABBREV(OP_GET, ops[op]))
	{
      DT_Q(dtstr_to_long,(ctl, index,
        &lindex));
	  DT_G(dttool_lut_get,(ctl,
        windowname, lindex, param->n));
	}
	else
	if (DT_ABBREV(OP_SET1, ops[op]))
	{
	  long lr, lg, lb;
      DT_Q(dtstr_to_long,(ctl, index,
        &lindex));
	  DT_Q(dtstr_to_long,(ctl, r, &lr));
	  DT_Q(dtstr_to_long,(ctl, g, &lg));
	  DT_Q(dtstr_to_long,(ctl, b, &lb));
	  DT_G(dttool_lut_set1,(ctl,
        windowname, lindex, lr, lg, lb));
	}
	else
	if (DT_ABBREV(OP_READ, ops[op]))
	{
      DT_Q(dtstr_to_long,(ctl, r,
        &lindex));
	  DT_G(dttool_lut_read,(ctl,
        windowname, index, lindex));
	}
	
    DT_G(dt_interactive_after,(			/* make post display				*/
      ctl, param, usage, arg,
      dttool_lut_cmd_check, NULL, rc));
  } 

  if (rc == DT_RC_STOP)
    rc = DT_RC_GOOD;

  DT_I(dt_interactive_leave,(			/* clean up before leaving			*/
    param, rc));

  DT_Q(dtparam_free,(param));			/* free param space					*/

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
