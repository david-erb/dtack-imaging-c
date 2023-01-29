/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
  ! Copyright (C) 1998 Dtack Inc. All Rights Reserved
  ! To use this file, you must have signed a license agreement with Dtack Inc.
  ! Under no circumstances may you redistribute this file.
  ! This software is provided AS IS with no warranty expressed or implied.
  ! Dtack Inc. accepts no liability for use or misuse of this file.
  ! http://www.dtack.com  dtack@dtack.com  telephone +360.417.6700
  ! Dtack Inc, P.O. Box 3061, Port Angeles, WA USA 98362
  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */




#include <dtack/base.h>

DT_RCSID("source/inter $RCSfile: interact.c,v $ $Revision: 1.2 $");

#include <dtack/str.h>
#include <dtack/os.h>
#include <dtack/cons.h>
#include <dtack/dialog.h>
#include <dtack/note.h>
#include <dtack/alpha.h>
#include <dtack/iact.h>

#define VALUE_OR_DEFAULT \
  (DTPARAM_FLAG_VALUE | \
   DTPARAM_FLAG_DEFAULT)

extern const char *dt_err_prefix;		/* back door into dt_err's output	*/

/*..........................................................................*/
dt_rc_e 
DTCONFIG_API1
dt_interactive_parse(					/* parse params from console args	*/
  int argc,
  char *argv[],
  const char *progname,
  const char *usage,
  dtparam_t *param,
  dt_enter_arg_t *arg,
  dt_enter_check_f *check,
  void *appstruct)
{
  DT_F("dt_interactive_parse");
  dt_ctl_t *ctl = &param->ctl;

  if (ctl->feedbackonly)
  {
	DT_Q(dt_enter_parse,(
      argc, argv, progname,
      usage, param, arg, 
      check, appstruct));
  }
  else
  {
    DT_Q(dt_interactive_ready,(ctl,		/* ready for interaction           	*/
      progname, argc, argv));

    DT_Q(dt_interactive_dialog,(ctl,	/* go have a dialog					*/
      param, argc, argv, usage,
      arg, check, appstruct));
  }

  return DT_RC_GOOD;
}

/*..........................................................................*/
dt_rc_e 
DTCONFIG_API1
dt_interactive_leave(					/* clean up before leaving 			*/
  dtparam_t *param,
  dt_rc_e xrc)
{
  DT_F("dt_interactive_leave");
  dt_ctl_t *ctl = &param->ctl;
  dt_rc_e rc = DT_RC_GOOD;

  DT_I(dtcons_uninit,(ctl));			/* close cons					    */

  dt_dbg_trace(ctl, F,
    DT_RCSID_STRING, __LINE__, 
    "ctl->dialog = 0x%08lx",
    ctl->dialog);

  if (ctl->dialog)						/* we have a master dialog open?    */
  {
    DT_I(dtdialog_uninit,(ctl,			/* finish the template dialog	    */
      (dtdialog_t *)ctl->dialog));
    DT_I(dtos_free2,(ctl,				/* free the template dialog			*/
      ctl->dialog,
      F, "template dialog"));
    ctl->dialog = NULL;
  }

  DT_I(dt_leave,(param));				/* do low level epilogue 			*/

  return rc;
}

/*..........................................................................
| NAME
| dt_interactive_enter - enter interactive mode
| 
| DESCRIPTION
| First function called by interactive programs.
|
| Interactive programs will use the following start-up sequence: 
| 
|:  dt_heapmon_init(ctl)
|:  dt_interactive_enter(ctl, progname, argc, argv)
|:  dtparam_init(param)
|:  dtparam_set(param, flag, ...)
|:  dt_enter_arg(ctl, arg, ...)
|:  dt_interactive_dialog(ctl, param, argc, argv, usage, arg, check)
| 
| Interactive programs do the following:
| dt_interactive_enter()	get space for tmp param, call dt_enter2(),
|							open dialog, set -errto and -dbgto to dialog
| dt_interactive_dialog()	call dt_enter_param(), take input, call
|							dt_dbg_parse(), dt_yield_parse()
|
|.Standard Params
| This function scans the command line for several standard parameters.
| If these parameters are not given on the command line,
| the compile-time defaults are used.
| Whether achieved via command line or compile-time default,
| the values are used to establish the program's operating environment.
| Once established, this operating environment can normally not
| be changed during the life of the program.
|
| The standard parameters are:
| :-D:			debug level
| :-mem:		memory source specifications
| :-dialog:		interactive dialog type
| :-errto:		where error messages go
| :-dbgto:		where debug messages go
| :-sayto:		where interactive informational messages go
|
|.Help
| The help filename may be specified by ~param->help~.
| If ~param->help~ is the value "none", then the help facility will
| be disabled.
| If ~param->help~ does not contain any slash characters,
| then the directories specified in the environment variable DT_HELP
| will be searched for the file.
| If ~param->help~ does not contain any dot characters after the
| last slash,
| then a system-dependent suffix will be added.
| 
| If ~param->help~ is not set,
| then the value of ~progname~ will be used to form the help file.
| The directories specified in the environment variable DT_HELP
| will be searched for the file name ~progname~ 
| with a system-dependent suffix added.
| For example, if the environment variable DT_HELP is
| set to "/usr/local/lib/dt/dth",
| and ~progname~ is "dttos",
| then the help file would be "/usr/local/lib/dt/doc/dttos.dth"
| for most UNIX-like systems.
| It would be "/usr/local/lib/dt/doc/dttos.hlp"
| for Microsoft Windows systems.
| 
| If the environment variable DT_HELP is not set,
| then any help files must be in the current directory.
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

dt_rc_e
DTCONFIG_API1
dt_interactive_ready(
  dt_ctl_t *ctl,
  const char *progname,
  int argc,
  char *argv[])
{
  DT_F("dt_interactive_ready");
  dtparam_t *param = NULL;
  const char *dialog_server = NULL;
  dtdialog_t *dialog = NULL;
  int have_dialog = 0;
  dt_rc_e rc;

  DTCSTACK_HIGHWATER;

  ctl->progname = progname;	

  DT_C(dtos_malloc2,(ctl,				/* get space for temporary param 	*/
    (void **)&param, sizeof(*param),
    F, "temporary param structure"));

  memset(param, 0, sizeof(*param));
  
  DT_C(dtparam_init,(param));			/* init temporary param space 		*/
  
  param->ctl = *ctl;					/* use whatever ctl we have now 	*/
  
  DT_C(dtparam_setflags,(param,			/* enable all params for parsing  	*/
    DTPARAM_FLAG_PARSE));

  DT_C(dt_enter_param,(					/* parse command line				*/
    argc, argv, 
    progname,
    NULL,								/* no usage here yet 				*/
    param, NULL));

  *ctl = param->ctl;					/* keep whatever new ctl we got 	*/

  if (param->help_flag &				/* help param has a value?		    */
      VALUE_OR_DEFAULT ||
      param->HELP_flag &				/* HELP param has a value?		    */
      VALUE_OR_DEFAULT)
    dialog_server = NULL;				/* inhibit dialog				    */
  else
  if (param->dialog_flag &				/* dialog param has a value?		*/
      VALUE_OR_DEFAULT)
    dialog_server = param->dialog;
										/* ................................	*/
  if (dialog_server &&					/* we have a dialog server?			*/
      strcmp(dialog_server, "none"))	/* and it's not none?				*/
  {
    DT_C(dtos_malloc2,(ctl,				/* space for the template dialog	*/
      (void **)&dialog,
      sizeof(*dialog),
      F, "template dialog"));
    DT_C(dtdialog_init,(ctl,			/* make the template dialog		    */
      dialog, NULL, dialog_server,
      DTIACT_CONS_BASE,
      DTDIALOG_FLAG_VMEDIUM,
      param->keyboard));
	have_dialog = 1;
    ctl->dialog = dialog;
    DT_C(dtcons_init,(ctl));			/* get ready for console sharing	*/
  }

										/* ................................	*/
  if (param->errto_flag &				/* errto param has a value?			*/
      VALUE_OR_DEFAULT)
  {
										/* we already took care of this		*/
  }
  else									/* errto not specified?			    */
  if (ctl->dialog)						/* we are doing dialogs?			*/
  {
    dtalpha_t *alpha;
    char tag[64];

    strcpy(tag, DTIACT_CONS_OUTPUT);
    strcat(tag, DTIACT_CONS_ERR);
    if (!(param->dbgto_flag &			/* debug doesn't have it's own?		*/
          VALUE_OR_DEFAULT))
      DT_C(dtstr_printf,(ctl,
        tag+strlen(tag),
        sizeof(tag)-strlen(tag),
        "%s", DTIACT_CONS_DBG));
    if (!(param->sayto_flag &			/* info doesn't have it's own?		*/
          VALUE_OR_DEFAULT))
      DT_C(dtstr_printf,(ctl,
        tag+strlen(tag),
        sizeof(tag)-strlen(tag),
        "%s", DTIACT_CONS_SAY));

    DT_C(dtiact_newalpha,(ctl,			/* alpha for the output				*/
      &alpha, tag));

    ctl->err_string = dtiact_out_string_err;
    ctl->errto = alpha;
  }
										/* ................................	*/
  if (param->dbgto_flag &				/* dbgto param has a value?			*/
      VALUE_OR_DEFAULT)
  {
										/* we already took care of this		*/
  }
  else									/* dbgto not specified?				*/
  if (ctl->err_string ==				/* and error is a dialog already?	*/
      dtiact_out_string_err)
  {
    ctl->dbgto = ctl->errto;			/* dbg follows err					*/
    ctl->dbg_string = ctl->err_string;
  }
  else									/* dbg gets its own dialog?			*/
  if (ctl->dialog)						/* we are doing dialogs?			*/
  {
    dtalpha_t *alpha;
    char tag[64];

    strcpy(tag, DTIACT_CONS_OUTPUT);
    strcat(tag, DTIACT_CONS_DBG);
    if (!(param->sayto_flag &			/* info doesn't have it's own?		*/
          VALUE_OR_DEFAULT))
      DT_C(dtstr_printf,(ctl,
        tag+strlen(tag),
        sizeof(tag)-strlen(tag),
        "%s", DTIACT_CONS_SAY));

    DT_C(dtiact_newalpha,(ctl,
      &alpha, tag));
    ctl->dbg_string = dtiact_out_string_dbg;
    ctl->dbgto = alpha;
  }
										/* ................................	*/
  if (param->sayto_flag &				/* sayto param has a value?			*/
      VALUE_OR_DEFAULT)
  {
										/* we already took care of this		*/
  }
  else									/* sayto not specified?				*/
  if (ctl->err_string ==				/* error is a dialog already?		*/
      dtiact_out_string_err)
  {
    ctl->sayto = ctl->errto;			/* say follows err					*/
    ctl->say_string = ctl->err_string;
  }
  else
  if (ctl->dbg_string ==				/* dbg is a dialog already?			*/
      dtiact_out_string_dbg)
  {
    ctl->sayto = ctl->dbgto;			/* say follows dbg					*/
    ctl->say_string = ctl->dbg_string;
  }
  else									/* say gets its own dialog?			*/
  if (ctl->dialog)						/* we are doing dialogs?			*/
  {
    dtalpha_t *alpha;
    char tag[64];

    strcpy(tag, DTIACT_CONS_OUTPUT);
    strcat(tag, DTIACT_CONS_SAY);

    DT_C(dtiact_newalpha,(ctl,
      &alpha, tag));
    ctl->say_string = dtiact_out_string_say;
    ctl->sayto = alpha;
  }

  DT_C(dtparam_free,(param));
  DT_C(dtos_free2,(ctl, param,
    F, "temporary param structure"));
  return DT_RC_GOOD;
  
cleanup:
  if (param != NULL)
  {
    DT_I(dtparam_free,(param));
    DT_I(dtos_free2,(ctl, param,
      F, "temporary param structure"));
  }
  
  if (dialog)							/* dialog got allocated? 			*/
  {
	if (have_dialog)					/* dialog got initialized? 			*/
	  DT_I(dtdialog_uninit,(ctl,
        dialog));
    DT_I(dtos_free2,(ctl,				/* free the template dialog			*/
      dialog,
      F, "template dialog"));
  }
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
