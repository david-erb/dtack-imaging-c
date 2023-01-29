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
| All functions Return DTOS_RANWELL on success.  
| Any other return value indicates failure.
| 
| ERRORS
| As yet undocumented.
| 
| END
 *..........................................................................*/

#include <dtack/t.h>
DT_RCSID("test $RCSfile: dttdia.c,v $ $Revision: 1.4 $");
#include <dtack/dialog.h>
#include <dtack/prj.h>

static char *USAGE = "usage: [params] [<dialog-spec>]\n"
DTDIALOG_HELP
"\n\
  If <dialog-spec> is omitted or is the empty string, then the\n\
  environment variable DT_DIALOG must contain the value.";

typedef struct {
  dt_key_t *keylist;
  int delay;
  int count;
  dtdialog_key_f *keyfunc;
} arg_t;

static dtdialog_key_f input_script;

# define NLINES 10
# define MAXLEN 10

static dt_key_t keylist1[] = {
DT_KEY_TAB,								/* switch to next field			    */
DT_KEY_UPARROW,							/* switch back to first field	    */
DT_KEY_DOWNARROW,						/* switch back to second field	    */
DT_KEY_TAB,								/* switch to next field			    */
DT_KEY_RIGHTARROW,						/* leave first char				    */
DT_KEY_ERASEEOL,						/* clear the field				    */
DT_KEY_ERASEEOL,						/* do nothing					    */
DT_KEY_LEFTARROW,						/* cover the first char again	    */
DT_KEY_ERASEEOL,						/* clear the field				    */

DT_KEY_TAB,								/* switch to next field			    */
DT_KEY_RIGHTARROW, DT_KEY_RIGHTARROW,	/* move over to '('					*/
DT_KEY_RIGHTARROW, DT_KEY_RIGHTARROW,
DT_KEY_RIGHTARROW, DT_KEY_RIGHTARROW,
DT_KEY_RIGHTARROW, DT_KEY_RIGHTARROW,
DT_KEY_RIGHTARROW, DT_KEY_RIGHTARROW,
'x',

DT_KEY_TAB,								/* switch back to first field	    */
DT_KEY_UPARROW,							/* switch back to last field	    */
DT_KEY_DOWNARROW,						/* switch back to first field	    */

DT_KEY_END, 
DT_KEY_DELETE, DT_KEY_DELETE,			/* already at end, do nothing		*/
DT_KEY_BACKSPACE, DT_KEY_BACKSPACE,		/* delete " me"						*/
DT_KEY_BACKSPACE, 
DT_KEY_LEFTARROW,						/* position on top of last char		*/
DT_KEY_DELETE,							/* delete the 'r' of "over"			*/
DT_KEY_DELETE, DT_KEY_DELETE,			/* already at end, do nothing		*/
DT_KEY_BACKSPACE, DT_KEY_BACKSPACE,		/* delete "ove"						*/
DT_KEY_BACKSPACE, 
DT_KEY_HOME, 
DT_KEY_DELETE, DT_KEY_DELETE,			/* delete the 'ty' of "type"		*/
DT_KEY_RIGHTARROW,						/* move over to 'x'					*/
DT_KEY_RIGHTARROW,
DT_KEY_RIGHTARROW,
DT_KEY_BACKSPACE, DT_KEY_BACKSPACE,		/* delete "pe "						*/
DT_KEY_BACKSPACE, 
DT_KEY_TAB,

'x', ' ', 'y', ' ', 'z', DT_KEY_ENTER,	/* simply complete overstrike here	*/

DT_KEY_TAB,								/* switch to next field			    */
DT_KEY_F2,
0
};

typedef struct {
  int old, new;
} transit_table_t;

/*..........................................................................*/
static
dt_rc_e
input_script(
  dt_ctl_t *ctl,
  dtdialog_t *dialog,
  int *iskey,
  dt_key_t *key)
{
  DT_F("dttdia_input_script");
  arg_t *arg = dialog->key_arg;

  if (iskey)							/* caller is polling?				*/
  {
    DT_Q(arg->keyfunc,(ctl, dialog,		/* do a real poll				    */
      iskey, key));
  }
  else									/* caller wants to wait?		    */
  {
    if (arg->keylist[arg->count])		/* more keys in list?			    */
	{
      int jskey;
      DT_Q(dtos_delay,(ctl,				/* slow things down a bit		  	*/
        arg->delay));
      DT_Q(arg->keyfunc,(ctl, dialog,	/* do a real poll				    */
        &jskey, key));
      if (!jskey)						/* no real key from the poll?  		*/
        *key = arg->keylist[
          arg->count++];
	}
	else								/* list exhausted?					*/
	{
      DT_Q(arg->keyfunc,(ctl, dialog,	/* do a real wait				    */
        NULL, key));
	}
  }

  return DT_RC_GOOD;
}

/*..........................................................................*/

static
dt_rc_e
dttdia_transit_check(
  dt_ctl_t *ctl,
  dtdialog_t *dialog,
  dt_key_t key,
  transit_table_t *table,
  int *assert_fail)
{
  DT_F("dttdia_transit_check");
  int i;
  for (i=0; table[i].old != -1; i++)
  {
    int old = table[i].old;
    int new = table[i].new;
    dtdialog_field_t *newfield;
    DT_Q(dtdialog_transit,(ctl, dialog,
      key,
      &dialog->field[old],
      &newfield));
    *assert_fail += DT_RC_GOOD !=
      dt_assert(ctl,
        newfield == &dialog->field[new],
        F, "key 0x%04x: field %2d,%2d -> %2d,%2d (%2d,%2d)",
        key,
        dialog->field[old].x,
        dialog->field[old].y,
        newfield->x, newfield->y,
        dialog->field[new].x,
        dialog->field[new].y);
  }
  return DT_RC_GOOD;
}

/*..........................................................................*/

static
dt_rc_e
dttdia_transit(
  dt_ctl_t *ctl,
  dtdialog_t *dialog1,
  dtdialog_t *dialog2,
  int *assert_fail)
{
  DT_F("dttdia");
  dt_rc_e rc = DT_RC_GOOD;
  char label[10*9];
  char input[2];
  dt_key_t key;
  int i, j;
  static transit_table_t uparrow_table[] = {
    { 2, 14},
    { 4, 16},
    { 6, 18},
    { 8,  2},
    {10,  4},
    {12,  6},
    {14,  8},
    {16, 10},
    {18, 12},
    {-1, -1},
  };
  static transit_table_t downarrow_table[] = {
    { 2,  8},
    { 4, 10},
    { 6, 12},
    { 8, 14},
    {10, 16},
    {12, 18},
    {14,  2},
    {16,  4},
    {18,  6},
    {-1, -1},
  };
  static transit_table_t tab_table[] = {
    { 2,  4},
    { 4,  6},
    { 6,  8},
    { 8, 10},
    {10, 12},
    {12, 14},
    {14, 16},
    {16, 18},
    {18,  2},
    {-1, -1},
  };
  static transit_table_t leftarrow_table[] = {
    { 2, 18},
    { 4,  2},
    { 6,  4},
    { 8,  6},
    {10,  8},
    {12, 10},
    {14, 12},
    {16, 14},
    {18, 16},
    {-1, -1},
  };

										/* ................................ */
  DT_GI(dtdialog_field,(ctl,
    dialog1, 0, dialog1->ye-1, NULL,
    "test", 0));

  for (i=0; i<3; i++)
  {
    for (j=0; j<3; j++)
    {
      sprintf(label+(i*3+j)*10, "%02dx%02d:%02d", j, i, dialog1->nfields+1);
      DT_GI(dtdialog_field,(ctl,
        dialog1, j*20,   i*2+2, NULL, label+(i*3+j)*10, 0));
      DT_GI(dtdialog_field,(ctl,
        dialog1, j*20+13, i*2+2, NULL, input,
        sizeof(input)));
    }
  }

  input[0] = '\0';

  DT_GI(dtdialog_input,(ctl,		
    dialog1, NULL, &key));

  DT_GI(dttdia_transit_check,(ctl,		/* check uparrow transits			*/
    dialog1, DT_KEY_UPARROW,
    uparrow_table, assert_fail));

  DT_GI(dttdia_transit_check,(ctl,		/* check downarrow transits			*/
    dialog1, DT_KEY_DOWNARROW,
    downarrow_table, assert_fail));

  DT_GI(dttdia_transit_check,(ctl,		/* check tab transits				*/
    dialog1, DT_KEY_TAB,
    tab_table, assert_fail));

  DT_GI(dttdia_transit_check,(ctl,		/* check leftarrow transits			*/
    dialog1, DT_KEY_LEFTARROW,
    leftarrow_table, assert_fail));

  DT_GI(dtdialog_uninit,(ctl,			/* free dialog structure		    */
    dialog1));

  if (dialog2)
  DT_GI(dtdialog_uninit,(ctl,			/* free dialog structure		    */
    dialog2));

  return DT_RC_GOOD;
}

/*..........................................................................*/

static
dt_rc_e
dttdia_input(
  dt_ctl_t *ctl,
  dtdialog_t *dialog1,
  dtdialog_t *dialog2,
  char *line,
  dt_key_t *key)
{
  DT_F("dttdia_input");
  dtdialog_t *dialog = dialog1;
  dtdialog_field_t *curfield = NULL;
  dtdialog_field_t *curfield1;
  dtdialog_field_t *curfield2 = NULL;
  int i;
  int pos = 0;
  dt_rc_e rc = DT_RC_GOOD;

  do {

    DT_GI(dtdialog_input,(ctl,			/* take input from current dialog	*/
      dialog, &curfield, key));

    if (*key == DT_KEY_F4)				/* swap key?						*/
	{
	  DT_GI(dtdialog_hide,(ctl,			/* hide current dialog			    */
        dialog, 
        DTDIALOG_HIDE_FLAG_CLEAR));
      if (dialog == dialog1 &&			/* swap current dialog			    */
          dialog2)
      {
        curfield1 = curfield;
        curfield = curfield2;
        dialog = dialog2;
      }
      else
      {
        curfield2 = curfield;
        curfield = curfield1;
	    dialog = dialog1;
      }

      DT_Q(dtdialog_hide,(ctl,			/* un-hide the new current dialog   */
        dialog,
        DTDIALOG_HIDE_FLAG_OFF));
	}

    if (dialog == dialog2)				/* on scrolling dialog?				*/
	{
      if (*key == DT_KEY_PAGEDOWN)
        pos += NLINES-1;
      else
      if (*key == DT_KEY_PAGEUP)
        pos -= NLINES-1;
      else
      if (*key == DT_KEY_UPARROW)
        pos -= 1;
      else
      if (*key == DT_KEY_DOWNARROW)
        pos += 1;

      if (pos < 0) pos = 0;
      if (pos > 90) pos = 90;

      for (i=0; i<NLINES; i++)			/* update display fields		    */
        sprintf(line+i*MAXLEN,
          "line %03d", i+pos);

      for (i=0; i<dialog->nfields; i++)	/* mark all fields changed		    */
        dialog->field[i].changed = 0;
	}
  } while (*key != DT_KEY_F2 &&
           *key != DT_KEY_F3);

  return DT_RC_GOOD;
}

/*..........................................................................*/

static
dt_rc_e
dttdia_make_typeover(
  dt_ctl_t *ctl,
  dtdialog_t *dialog,
  char *input0,
  char *input1,
  char *input2,
  char *input3,
  int lmax)
{
  DT_F("dttdia_make_typeover");
  dt_rc_e rc = DT_RC_GOOD;

  DT_GI(dtdialog_field,(ctl,
    dialog, 0, 3, NULL, "Field 0", 0));

  DT_GI(dtdialog_field,(ctl,
    dialog, 12, 3, NULL, input0,
    lmax));
  strcpy(input0, "type xyz over me");

  DT_GI(dtdialog_field,(ctl,
    dialog, 40, 3, NULL, "Field 1", 0));

  DT_GI(dtdialog_field,(ctl,
    dialog, 52, 3, NULL, input1,
    lmax));
  strcpy(input1, "type x y z over me");

  DT_GI(dtdialog_field,(ctl,
    dialog, 0, 5, NULL, "Field 2", 0));
  DT_GI(dtdialog_field,(ctl,
    dialog, 12, 5, NULL, input2,
    lmax));
  strcpy(input2, "clear me");

  DT_GI(dtdialog_field,(ctl,
    dialog, 40, 5, NULL, "Field 2", 0));
  DT_GI(dtdialog_field,(ctl,
    dialog, 52, 5, NULL, input3,
    lmax));
  strcpy(input3, "insert x () inside");

  return rc;
}

/*..........................................................................*/

static
dt_rc_e
dttdia_check_typeover(
  dt_ctl_t *ctl,
  char *input0,
  char *input1,
  char *input2,
  char *input3,
  int *assert_fail)
{
  DT_F("dttdia_check_typeover");
  dt_rc_e rc = DT_RC_GOOD;

  *assert_fail += DT_RC_GOOD !=
    dt_assert(ctl,
      !strcmp(input0, "xyz "),
      F, "input0 \"%s\" (\"%s\")",
      "xyz ", input0); 

  *assert_fail += DT_RC_GOOD !=
    dt_assert(ctl,
      !strcmp(input1, "x y z"),
      F, "input1 \"%s\" (\"%s\")",
      "x y z", input1);

  *assert_fail += DT_RC_GOOD !=
    dt_assert(ctl,
      !strcmp(input2, ""),
      F, "input2 \"%s\" (\"%s\")",
      "", input2);

  *assert_fail += DT_RC_GOOD !=
    dt_assert(ctl,
      !strcmp(input3, "insert x (x) inside"),
      F, "input3 \"%s\" (\"%s\")",
      "insert x (x) inside", input3);

  return rc;
}

/*..........................................................................*/

static
dt_rc_e
dttdia_make_browse(
  dt_ctl_t *ctl,
  dtdialog_t *dialog,
  char *input,
  int lmax,
  char *line)
{
  DT_F("dttdia_make_browse");
  dt_rc_e rc = DT_RC_GOOD;
# define NLINES 10
# define MAXLEN 10
  int i;

  DT_GI(dtdialog_field,(ctl,
    dialog, 0, NLINES, NULL, "command", 0));
  DT_GI(dtdialog_field,(ctl,
    dialog, 9, NLINES, NULL, input, lmax));
  input[0] = '\0';

  for (i=0; i<NLINES; i++)
    DT_GI(dtdialog_field,(ctl,
      dialog, 0, i, NULL, line+i*MAXLEN, 0));

  return rc;
}

/*..........................................................................*/

static
dt_rc_e
dttdia_double(
  dt_ctl_t *ctl,
  dtdialog_t *dialog1,
  dtdialog_t *dialog2,
  int *assert_fail)
{
  DT_F("dttdia_double");
  dt_rc_e rc = DT_RC_GOOD;
# define LMAX 24
  char input0[LMAX];
  char input1[LMAX];
  char input2[LMAX];
  char input3[LMAX];
  char line[NLINES * MAXLEN];
  char input[32];
  dt_key_t key;
										/* ................................ */
  DT_GI(dttdia_make_typeover,(ctl,
    dialog1, input0, input1,
    input2, input3, LMAX));

  if (dialog2)
  DT_GI(dttdia_make_browse,(ctl,
    dialog2, input, sizeof(input),
    line));

  DT_GI(dttdia_input,(ctl,				/* take input from dialogs		    */
    dialog1, dialog2, line, &key));

  DT_GI(dtdialog_uninit,(ctl,			/* free dialog structure		    */
    dialog1));

  if (dialog2)
  DT_GI(dtdialog_uninit,(ctl,			/* free dialog structure		    */
    dialog2));

  DT_GI(dttdia_check_typeover,(ctl,
    input0, input1,
    input2, input3, assert_fail));

  return DT_RC_GOOD;
}

/*..........................................................................*/

DTT_TEST(dttdia)
{
  DT_F("dttdia");
  dt_ctl_t *ctl = &param->ctl;
  char server[64];
  int assert_fail = 0;
  dt_rc_e rc = DT_RC_GOOD;
  int n;
  dtdialog_t dialog1, dialog2;

  DT_GI(dt_heapmon_init,(ctl));			/* init memory checking			    */

  DT_GI(dtparam_init,(param));			/* init param structure             */

  DTPARAM_DEFAULT(dbg, 0);				/* set default for params we want	*/
  DTPARAM_DEFAULT(op, "fast");

  DT_GI(dt_enter_param,(				/* go parse args					*/
    argc, argv, F, USAGE, param, &n));

  argc -= n;
  argv += n;

  server[0] = '\0';
  if (argc == 1)
    DT_Q(dtstr_printf,(ctl, server,
      sizeof(server), "%s", argv[0]))
  else
  if (argc != 0)
    return dt_usage_say(
      param, USAGE);

  if (strlen(server) == 0)				/* no server argument?				*/
  {
	if (param->dialog_flag &			/* there is a dialog param? 		*/
		(DTPARAM_FLAG_VALUE |			/* on command line? 				*/
		 DTPARAM_FLAG_DEFAULT) &&		/* or just defaulted? 				*/
		strcmp(param->dialog, "none"))	/* and it's not "none" 				*/
	  DT_Q(dtstr_printf,(ctl, server,	/* use the dialog param 			*/
        sizeof(server), "%s", 
		param->dialog))
	else								/* dialog unknown thus far?			*/
	{
	  dtprj_param_defaults_t *dflts =	/* for short 						*/
	    dtprj_param_defaults;
	  int i;
	  for (i=0; dflts[i].abbrev; i++)	/* search project defaults table 	*/
	  {
	    if (!strcmp("-dialog",
      	      dflts[i].abbrev))
		{
		  DT_Q(dtstr_printf,(ctl, server,
            sizeof(server), "%s", 
		    dflts[i].dflt));
		  break;
		}
	  }
	}
  }

										/* ................................ */
  DT_GI(dtdialog_init,(ctl,				/* init the first dialog		    */
    &dialog1, NULL, server, 
    "Dttdia1 Title",
    DTDIALOG_FLAG_NONE,
    param->keyboard));				   

  if (strcmp(param->op, "transit"))		/* transit doesn't get second one   */
  DT_GI(dtdialog_init,(ctl,				/* init the second dialog		    */
    &dialog2, &dialog1, server, 
    "Dttdia2 Title",
    DTDIALOG_FLAG_NONE,
    param->keyboard));				   
										/* ................................ */
  if (!strcmp(param->op, "all") ||
      !strcmp(param->op, "wait"))
  {
    DT_GI(dttdia_double,(ctl,
      &dialog1, &dialog2,
      &assert_fail));
  }
										/* ................................ */
  if (!strcmp(param->op, "all") ||
      !strcmp(param->op, "auto") ||
      !strcmp(param->op, "slow"))
  {
    arg_t arg = {keylist1, 400, 0};
    arg.keyfunc = dialog1.key;
    dialog1.key = input_script;
    dialog1.key_arg = &arg;
    DT_GI(dttdia_double,(ctl,
      &dialog1, &dialog2,
      &assert_fail));
  }
										/* ................................ */
  if (!strcmp(param->op, "all") ||
      !strcmp(param->op, "auto") ||
      !strcmp(param->op, "fast"))
  {
    arg_t arg = {keylist1, 10, 0};
    arg.keyfunc = dialog1.key;
    dialog1.key = input_script;
    dialog1.key_arg = &arg;
    DT_GI(dttdia_double,(ctl,
      &dialog1, &dialog2,
      &assert_fail));
  }
										/* ................................ */
  if (!strcmp(param->op, "all") ||
      !strcmp(param->op, "transit"))
  {
    DT_GI(dttdia_transit,(ctl,
      &dialog1, NULL,
      &assert_fail));
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
