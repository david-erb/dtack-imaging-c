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
| dt_interactive_dialog - interactive dialog
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

#include <dtack/base.h>
DT_RCSID("source/inter $RCSfile: interdia.c,v $ $Revision: 1.2 $");
#include <dtack/str.h>
#include <dtack/os.h>

#include <dtack/cons.h>
#include <dtack/dialog.h>
#include <dtack/note.h>
#include <dtack/alpha.h>
#include <dtack/iact.h>
									
#define LMAX (128)						/* space reserved for input field 	*/

static dtcons_opfunc_f dt_interdialog_cons_opfunc;

										/* ................................ */

static dt_enter_newfield_f newfield_callback;

#define MSG_X0 (0)
#define MSG_Y0 (0)
#define COLWIDTH (38)
#define GUTTER (4)

#define STATUS_X0 (xxx->dialog->xe-sizeof(xxx->status)-2)
#define STATUS_Y0 (xxx->dialog->ye-1)

#define EMPTY_FIELD_CONTENTS ""

#define CALLBACK_PARAM_MAX (64)
#define CALLBACK_ARGV_MAX \
  (2*CALLBACK_PARAM_MAX+2)

typedef struct {						/* arg from param_callback		    */
  dtdialog_t *dialog;
  int i;
  dtparam_flag_t flag;
  char *abbrev[CALLBACK_PARAM_MAX];
  char *desc[CALLBACK_PARAM_MAX];
  char *input[CALLBACK_PARAM_MAX];
  char *argv[CALLBACK_ARGV_MAX];
  int argc;
} callback_arg_t;

typedef struct {
  dtparam_t *param;
  dtdialog_t *dialog;
  dtdialog_field_t *field;
  char status[24];
  char msg[DT_ENTER_MSG_MAXLEN];
  callback_arg_t callback_arg;
  dtdialog_field_t *msg_field;
  dtdialog_field_t *status_field;
} xxx_t;

/*..........................................................................*/
static
dt_rc_e
dt_interactive_make_argv(
  dt_ctl_t *ctl,
  callback_arg_t *callback_arg)
{
  DT_F("dt_interactive_make_argv");
  int argc = 0;
  char **argv = callback_arg->argv;
  int i;

  for (i=0; i<callback_arg->i; i++)
  {
	char *val;
	char *abbrev;
	char *end;

	val = callback_arg->input[i];		/* for short						*/
	abbrev = callback_arg->abbrev[i];

    while(*val && *val == ' ')			/* trim leading blanks			    */
      val++;

	end = val + strlen(val);
    while (end > val)					/* trim trailing blanks			    */
    {
      end--;
      if (*end != ' ')
        break;
      *end = '\0';
    }

    if (argc+2 >= CALLBACK_ARGV_MAX)	/* filled our argv array already? 	*/
      return dt_err(ctl, F,
        "to many params (max %d)",
        CALLBACK_PARAM_MAX);
    argv[argc++] = abbrev;				/* add "-abbrev" to argv array	    */
    argv[argc++] = val;					/* add param's value to argv array	*/
  }
  
  argv[argc] = NULL;					/* NULL sentinel 					*/
  callback_arg->argc = argc;
  
  return DT_RC_GOOD;
}

/*..........................................................................*/
static
dt_rc_e
dt_interactive_find_desc(
  dt_ctl_t *ctl,
  callback_arg_t *callback_arg,
  const char *desc,
  const char **start)
{
  int i;

  for (i=0; i<callback_arg->i; i++)
  {
    if (callback_arg->desc[i] == desc)	/* this param is a match?		    */
	{
      *start = callback_arg->input[i];	/* mark it for the start		    */
	  break;
	}
  }

  return DT_RC_GOOD;
}

/*..........................................................................*/
static
dt_rc_e
dt_interactive_find_abbrev(
  dt_ctl_t *ctl,
  callback_arg_t *callback_arg,
  const char *abbrev,
  const char **start)
{
  int i;

  for (i=0; i<callback_arg->i; i++)
  {
    if (!strcmp(abbrev,					/* this param is a match?		    */
	  	  callback_arg->abbrev[i]))
	{
      *start = callback_arg->input[i];	/* mark it for the start		    */
	  break;
	}
  }

  return DT_RC_GOOD;
}

/*..........................................................................*/
static
dt_rc_e
dt_interactive_free_field(
  dt_ctl_t *ctl,
  callback_arg_t *callback_arg)
{
  DT_F("dt_interactive_free_field");
  int i;

  for (i=0; i<callback_arg->i; i++)
  {
    DT_Q(dtos_free2,(ctl,
      callback_arg->input[i],			/* free the input string	    	*/
      F, "field value"));
  }

  return DT_RC_GOOD;
}

/*..........................................................................
 * CLOSE is issued only at the end of the program by dt_interactive_leave()
 *..........................................................................*/

static
dt_rc_e
DTCONFIG_API1
dt_interdialog_cons_opfunc(
  dt_ctl_t *ctl,
  dtcons_op_e op,
  void *arg1,
  void *arg2)
{
  DT_F("dt_interdialog_cons_opfunc");
  xxx_t *xxx = (xxx_t *)arg1;
  dtparam_t *param = xxx->param;
  dtdialog_t *dialog = xxx->dialog;
  dtdialog_field_t *field = xxx->field;
  dt_key_t key = 0;
  int iskey;

  static dtiact_but_t runbut[DTDIALOG_NBUTTONS] = {
    DTIACT_BUT_HALT,
    DTIACT_BUT_HELP,
    DTIACT_BUT_SWAP,
    DTIACT_BUT_EXIT
  };

  static dtiact_but_t waitbut[DTDIALOG_NBUTTONS] = {
    DTIACT_BUT_RUN,
    DTIACT_BUT_HELP,
    DTIACT_BUT_SWAP,
    DTIACT_BUT_EXIT
};

  switch(op)
  {
    case DTCONS_OP_ENTER:				/* ................................	*/
      DT_Q(dtdialog_hide,(ctl,          /* un-hide the dialog               */
        dialog,
        DTDIALOG_HIDE_FLAG_OFF));
    case DTCONS_OP_UPDATE:				/* ................................	*/
      DT_Q(dtiact_twirl,(ctl,			/* do twirl for this status			*/
        xxx->dialog,
        xxx->status));
      DT_Q(dtiact_buttons,(ctl,			/* set up buttons for this status	*/
        xxx->dialog, xxx->status,
        waitbut, runbut));
      DT_Q(dtdialog_input,(ctl,			/* display but don't wait		    */
        dialog, NULL, NULL));
      DT_Q(dtdialog_key,(ctl,			/* check for keyboard input		    */
        dialog, &iskey, &key));
	  if (iskey)
        dt_dbg(ctl, F, DT_DBG_MASK_ASYNC,
          "key 0x%04x, arg2=0x%08lx",
          key, arg2);
      if (arg2)
        if (iskey)
          *(dt_key_t *)arg2 = key;
        else
          *(dt_key_t *)arg2 = 0;
    break;
    case DTCONS_OP_LEAVE:				/* ................................	*/
      DT_Q(dtdialog_hide,(ctl,
        dialog,
        DTDIALOG_HIDE_FLAG_CLEAR));
    break;
    case DTCONS_OP_STATUS:				/* ................................	*/
	{
      char *status = (char *)arg2;
      unsigned int l = strlen(status);
      l = DT_MIN(l, sizeof(xxx->status)-1);
      memcpy(xxx->status, status, l);
      xxx->status[l] = '\0';
      xxx->status_field->changed = 0;   /* mark contents of field new       */
    }
    break;
    case DTCONS_OP_INPUT:				/* ................................	*/
      DT_Q(dtiact_buttons,(ctl,			/* set up buttons for this status	*/
        xxx->dialog, xxx->status,
        waitbut, runbut));
      DT_Q(dtdialog_input,(ctl,			/* display then wait for key		*/
        dialog, &field, &key));
      xxx->field = field;
      if (arg2)
        *(dt_key_t *)arg2 = key;

    break;
    case DTCONS_OP_CLOSE:				/* ................................	*/
	{
      DT_Q(dt_interactive_free_field,(	/* free fields for params		    */
        ctl, &xxx->callback_arg));
      DT_Q(dtos_free2,(ctl, xxx,
        F, "xxx structure"));
	}
    break;
  }
  return DT_RC_GOOD;
}

/*..........................................................................*/

static
dt_rc_e 
dt_interactive_interact(				/* have a dialog around params/args	*/
  dt_ctl_t *ctl,
  dtparam_t *param,
  const char *usage,
  const char *start,
  dt_enter_arg_t *arg,
  dt_enter_check_f *check,
  void *appstruct,
  dt_rc_e xrc)
{
  DT_F("dt_interactive_interact");
  xxx_t *xxx;
  int i;
  dtdialog_field_t *field = NULL;
  long dbg_mask;
  int nused;
  dt_rc_e rc = DT_RC_GOOD;

  if (xrc == DT_RC_EXIT)				/* an exit here means really exit   */
    return xrc;

  if (ctl->cons == NULL ||				/* no console handling in effect?   */
      ctl->dialog == NULL)				/* no main dialog defined?		    */
    return DT_RC_STOP;					/* can't do much in this routine    */

  if (xrc == DT_RC_STOP)				/* there was a stop code?			*/
    dt_dbg(ctl, NULL, DT_DBG_MASK_ANY,
      "program terminated by keyboard"
      " or mouse event");
  else
  if (xrc != DT_RC_GOOD)				/* there was a bad code?			*/
    dt_dbg(ctl, NULL, DT_DBG_MASK_ANY,
      "program stopped due to error");

  if (xrc != DT_RC_GOOD)				/* there was a non-good stop code?	*/
  {
    dtcons_opfunc_f *opfunc = NULL;
    void *arg;
    DT_Q(dtcons_search,(ctl,			/* see if we have an error display	*/
      DTIACT_CONS_ERR,
      &opfunc, &arg));
    if (opfunc)							/* we have an error display?	    */
      DT_Q(dtcons_goto,(ctl,			/* go to error display			    */
        opfunc, arg));
  }

  {
    dt_key_t key;
    DT_Q(dtcons_update,(ctl,			/* update display status		    */
      DTIACT_STATUS_WAITING,
      &key));
	if (rc == DT_RC_EXIT)				/* an exit here means really exit   */
      return rc;
  }

  DT_Q(dtcons_find,(ctl,				/* get our working structure	    */
    DTIACT_CONS_BASE,
    NULL, (void **)&xxx));

  do {
    if (xxx->msg[0])					/* message remains for display?		*/
      xxx->msg_field->changed = 0;		/* make sure msg field gets updated	*/

    DT_GI(dt_interactive_find_desc,(	/* see if start refers to a param	*/
      ctl, &xxx->callback_arg,
      start, &start));

    for (i=0;							/* find corresponding field			*/ 
         i<xxx->dialog->nfields;
         i++)
      if (xxx->dialog->field[i].contents ==
          start)
      {
        field = &xxx->dialog->field[i];
        break;
      }

    xxx->field = field;					/* start field					    */

    do {
	  dt_key_t key;
      DT_GI(dtcons_input,(ctl, &key));	/* do the dialog				    */
      dt_dbg(ctl, F, DT_DBG_MASK_ASYNC,
        "key 0x%04x rc %d",
        key, rc);
      if (rc == DT_RC_STOP)				/* stop means: stop editing and run	*/
        break;
      if (rc != DT_RC_GOOD)				/* got EXIT or something bad		*/
        return rc;
      if (key == DTCONS_KEY_HELP)		/* help key?						*/
        DT_Q(dt_interactive_help,(ctl,	/* display usage on help screen	    */
          NULL, usage));
	} while (rc == DT_RC_GOOD);

    rc = DT_RC_GOOD;

    DT_GI(dt_interactive_make_argv,(	/* make argc/argv from values		*/
      ctl, &xxx->callback_arg));

#   ifdef NONO
	{
	  int i;
	  for (i=0; 
		   i<xxx->callback_arg.argc;
		   i++)
	    dt_dbg(ctl, F,
          DT_DBG_MASK_ALWAYS, "%2d. %s",
          i,
		  xxx->callback_arg.argv[i]?
            xxx->callback_arg.argv[i]:
            "NULL");
	}
#   endif

    if (xxx->msg[0])					/* there used to be a message?		*/
    {
      xxx->msg[0] = '\0';				/* clear the message		    	*/
      xxx->msg_field->changed = 0;		/* make sure msg field gets cleared	*/
    }

    DT_GI(dtparam_parse,(param,			/* parse the argc/argv of params	*/
      xxx->callback_arg.argc,
      xxx->callback_arg.argv, 
      &nused,
      xxx->msg,
      sizeof(xxx->msg)));

    if (xxx->msg[0])					/* a message from general parsing?	*/
	{
      DT_GI(
        dt_interactive_find_abbrev,(	/* find start for abbreviation		*/
        ctl, &xxx->callback_arg,
        xxx->callback_arg.argv[			/* keyword abbreviation that failed */
		  nused-1],
        &start));						/* start input field 				*/
	}

    if (!xxx->msg[0])					/* no parse error yet?				*/
	{
      DT_GI(dt_dbg_parse,(				/* set dbg mask from dbg param		*/
        ctl, param->dbg, &dbg_mask,
        xxx->msg, sizeof(xxx->msg)));
      if (xxx->msg[0])					/* a message from dbg parsing?		*/
        start = param->dbg_desc;		/* field where focus should go	    */
	  else
	    ctl->dbg_mask |= dbg_mask;		/* put mask in environment 			*/
	}
	
    if (!xxx->msg[0])					/* no parse error yet?				*/
	{
	  unsigned long yield_flags;
	  
      DT_GI(dt_yield_parse,				/* set yield mode from param		*/
        (ctl, param->yield,
		 &yield_flags,
         xxx->msg, sizeof(xxx->msg)));
      if (xxx->msg[0])					/* message from yield parsing?		*/
        start = param->yield_desc;		/* field where focus should go	    */
	  else
	  {
		ctl->yield_flags = yield_flags;	/* put flags in the environment 	*/
		if (ctl->yield_flags !=			/* yield has been turned on? 		*/
			DT_YIELD_FLAG_OFF &&
			ctl->yield == NULL)			/* don't have yield function yet? 	*/
          ctl->yield = dt_yield_va;		/* start using standard function 	*/
	  }
	}

    if (!xxx->msg[0])					/* no parse error yet?				*/
	{
	  DT_GI(check,(ctl, appstruct,		/* check everything else for error	*/
		param, arg,
        xxx->msg, &start))
	}

    if (rc == DT_RC_GOOD &&
        xxx->msg[0] != '\0')			/* some message?				    */
	{
      DT_Q(dtcons_goto,(ctl,			/* go to param display			    */
        dt_interdialog_cons_opfunc, 
        xxx));
	}

	if (ctl->win == NULL ||				/* we don't have a win server yet? 	*/
		strcmp(param->win, ctl->win))	/* win server changed on dialog? 	*/
	{
	  if (ctl->win != NULL)
	    DT_Q(dtos_free2,(ctl,			/* get rid of old server name 		*/
          (char *)ctl->win,
          F, "default windowing server"));
	  ctl->win = NULL;
	  DT_Q(dtos_malloc2,(ctl,			/* get space for server name 		*/
        (void **)&ctl->win,
        strlen(param->win)+1,
        F, "default windowing server"));
	  strcpy((char *)ctl->win,
        param->win);
	}

  } while(rc == DT_RC_GOOD &&			/* until no error					*/
          xxx->msg[0] != '\0');			/* and no message					*/

  for (i=0; i<xxx->callback_arg.argc; i++)
    dt_dbg(ctl, F, DT_DBG_MASK_ARG,
      "param \"%s\"",
      xxx->callback_arg.argv[i]);

  for (i=0; arg[i].desc; i++)
    dt_dbg(ctl, F, DT_DBG_MASK_ARG,
     "arg \":%s\": \"%s\"",
     arg[i].desc, arg[i].val);

  if (rc == DT_RC_GOOD)
  {
	dt_key_t key;
    DT_G(dtcons_update,(ctl,			/* update display status		    */
      DTIACT_STATUS_RUNNING, 
      &key));
    if (rc == DT_RC_EXIT)				/* an exit here means really exit   */
      return rc;
    else
	if (rc != DT_RC_BAD)
	{
      rc = DT_RC_GOOD;
	}
  }

  return rc;;
}

/*..........................................................................*/

dt_rc_e 
DTCONFIG_API1
dt_interactive_after(					/* have a dialog around params/args	*/
  dt_ctl_t *ctl,
  dtparam_t *param,
  const char *usage,
  dt_enter_arg_t *arg,
  dt_enter_check_f *check,
  void *appstruct,
  dt_rc_e xrc)
{
  DT_F("dt_interactive_after");
  DT_Q(dt_interactive_interact,(ctl,
    param, usage, NULL,
    arg, check, appstruct, xrc));
  return DT_RC_GOOD;
}

/* ........................................................................ *
| NAME
| dt_interactive_dialog - have a dialog around params/args
| 
| DESCRIPTION
| This is the main routine called by interactive applications.
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
 * ........................................................................ */

dt_rc_e 
DTCONFIG_API1
dt_interactive_dialog(					/* have a dialog around params/args	*/
  dt_ctl_t *ctl,
  dtparam_t *param,
  int argc,
  char *argv[],
  const char *usage,
  dt_enter_arg_t *arg,
  dt_enter_check_f *check,
  void *appstruct)
{
  DT_F("dt_interactive_dialog");
  xxx_t *xxx;
  char msg[DT_ENTER_MSG_MAXLEN];
  int desc_max = 0;
  int i, n;
  const char *start = NULL;

  DTCSTACK_HIGHWATER;

  DT_Q(dt_enter_param,					/* parse what's on command line	    */
    (argc, argv, ctl->progname, usage,
     param, &n));

  argc -= n;
  argv += n;

  for (i=0; arg[i].desc; i++)			/* for each expected positional	    */
  {
    desc_max = DT_MAX(
	  (unsigned int)desc_max,			/* find the longest description	    */
      strlen(arg[i].desc));
    if (i < argc)						/* this one is on command line?		*/
      strcpy(arg[i].val, argv[i]);		/* fill in value from command line	*/
    else								/* this one is not on command line?	*/
      arg[i].val[0] = '\0';
  }

  if (i < argc)							/* more args than positionals?		*/
  if (i == 0)
  {
    dt_err(ctl, NULL,
      "Sorry, no arguments are"
      " allowed on command line.");
	return DT_RC_STOP;
  }
  else
  {
    dt_err(ctl, NULL,
      "Sorry, expected only %d"
      " arguments on command line.", i);
	return DT_RC_STOP;
  }

  msg[0] = '\0';

  if (check != NULL)					/* caller has a check function? 	*/
    DT_Q(check,(ctl, appstruct,			/* check everything for input error	*/
      param, arg,
      msg, &start));

  if (!ctl->cons)						/* not ready for console sharing?	*/
  {
    if (!msg[0])						/* no errors?					    */
      return DT_RC_GOOD;				/* no need for a dialog			    */
    else
	{
	  dt_err(ctl, NULL, "%s", msg);		/* spit out message as error		*/
	  return dt_usage_say(				/* give usage as line-based output	*/
        param, usage);
	}
  }

  DT_Q(dtos_malloc2,(ctl,
    (void **)&xxx, sizeof(*xxx),
    F, "xxx structure"));
  memset(xxx, 0, sizeof(*xxx));
  xxx->param = param;
  xxx->dialog = 
    (dtdialog_t *)ctl->dialog;			/* use the main dialog			    */

  DT_Q(dtcons_add,(ctl,					/* register dialog for interaction	*/
    DTIACT_CONS_BASE,
    dt_interdialog_cons_opfunc, xxx));

  xxx->msg_field =						/* field where message will live	*/
    &xxx->dialog->field[
      xxx->dialog->nfields];
  DT_Q(dtdialog_field,(ctl,				/* field for error message			*/
    xxx->dialog, MSG_X0, MSG_Y0, 
    DTIACT_FIELDNAME_ERROR, 
    xxx->msg,
    DTDIALOG_FIELD_HIGHLIGHT |
    DTDIALOG_FIELD_BEEP));

  strncpy(xxx->msg, msg,
    sizeof(xxx->msg));

  xxx->status_field = 					/* field where status will live		*/
    &xxx->dialog->field[
      xxx->dialog->nfields];
  DT_Q(dtdialog_field,(ctl,				/* make status display field	    */
    xxx->dialog,
    STATUS_X0, STATUS_Y0,
    DTIACT_FIELDNAME_STATUS, 
    xxx->status, 
    DTDIALOG_FIELD_NORMAL));
  strcpy(xxx->status, ""); 

  xxx->callback_arg.dialog = xxx->dialog;

  DT_Q(dt_enter_fields,(param, arg,		/* create interactive fields		*/
    newfield_callback, 
    (void *)&xxx->callback_arg));

  DT_Q(dtcons_goto,(ctl,				/* go to the dialog					*/
    dt_interdialog_cons_opfunc, xxx));

  DT_Q(dt_interactive_interact,(ctl,	/* interact with dialog			    */
    param, usage, start,
    arg, check, appstruct, DT_RC_GOOD));

  return DT_RC_GOOD;
}

/*..........................................................................*/
static
dt_rc_e
DTCONFIG_API1
newfield_callback(
  dt_ctl_t *ctl,
  void *arg,
  char *abbrev,
  char *desc,
  char *val,
  int x,
  int y,
  int labelwidth,						/* label width					    */
  int inputwidth)						/* input width					    */
{
  DT_F("dt_interactive_newfield_callback");
  int justify;
  callback_arg_t *callback_arg =
    (callback_arg_t *)arg;
  dtdialog_t *dialog =					/* for short					    */
    callback_arg->dialog;
  char *s;

  if (desc == NULL)
    desc = "";

  dt_dbg(ctl, F, DT_DBG_MASK_USR1,
    "\"%s\", labelwidth=%d, inputwidth=%d", desc, 
    labelwidth, inputwidth);

  justify = labelwidth - strlen(desc);	/* right-justify the label		    */
  if (justify < 0)
    justify = 0;

  DT_Q(dtdialog_field,(ctl,				/* field for the param description	*/
    dialog, x+justify, y,
    abbrev, desc,
    DTDIALOG_FIELD_NORMAL));

  if (val == NULL)
    val = EMPTY_FIELD_CONTENTS;
  if (strncmp(abbrev, "arg", 3))		/* this is a param? 				*/
  {
    DT_Q(dtos_malloc2,(ctl,				/* space for the input value	    */
      (void **)&s, LMAX,
       F, "field value"));
  
	dtstr_printf(ctl, s, LMAX,
      "%s", val);

    callback_arg->input[				/* remember the input contents	    */
      callback_arg->i] = s;

    callback_arg->abbrev[
      callback_arg->i] = abbrev;
    callback_arg->desc[
      callback_arg->i] = desc;
    callback_arg->i++;
  }
  else									/* this is an argument? 			*/
    s = val;

  DT_Q(dtdialog_field,(ctl,				/* make a field for the input	    */
    dialog, x+labelwidth+1, y, 
    abbrev, s, inputwidth));

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
