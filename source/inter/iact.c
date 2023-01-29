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

DT_RCSID("source/inter $RCSfile: iact.c,v $ $Revision: 1.2 $");

#include <dtack/dialog.h>
#include <dtack/cons.h>
#include <dtack/note.h>
#include <dtack/alpha.h>
#include <dtack/iact.h>

#include <dtack/os.h>
#include <dtack/mem.h>

/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtiact_buttons(
  dt_ctl_t *ctl,
  dtdialog_t *dialog,
  char *status,
  dtiact_but_t *waitbut,
  dtiact_but_t *runbut)
{
  DT_F("dtiact_buttons");
  dtiact_but_t *but;
  int i;

  if (!strcmp(status,
      DTIACT_STATUS_WAITING))
    but = waitbut;
  else
  if (!strcmp(status,
      DTIACT_STATUS_RUNNING))
    but = runbut;
  else
    but = NULL;

  if (but)
  {
    status[0] = toupper(status[0]);
    for (i=0; i<DTDIALOG_NBUTTONS; i++)
	{
      DT_Q(dtdialog_button,(ctl,
        dialog, 
        but[i].label, 
        i,
        but[i].key));
	}
  }
  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtiact_twirl(
  dt_ctl_t *ctl,
  dtdialog_t *dialog,
  char *status)
{
  DT_F("dtiact_twirl");
  int flags;;

  if (!strcmp(status,					/* going into waiting state?	    */
      DTIACT_STATUS_WAITING))
    flags =								/* shut off twirling			    */
      DTDIALOG_TWIRL_FLAG_STOP;	
  else
  if (!strcmp(status,					/* going into running state?	    */
      DTIACT_STATUS_RUNNING))
    flags =
      DTDIALOG_TWIRL_FLAG_START;		/* start twirling					*/
  else									/* any other state				    */
    flags = 							/* do a twirl					    */
      DTDIALOG_TWIRL_FLAG_ADVANCE;

  DT_Q(dtdialog_twirl,(ctl,				/* twirl						  	*/
    dialog, -1, -1, flags));

  return DT_RC_GOOD;
}


/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtiact_cons_opfunc(
  dt_ctl_t *ctl,
  dtcons_op_e op,
  void *arg1,
  void *arg2)
{
  DT_F("dtiact_cons_opfunc");
  dtalpha_t *alpha = (dtalpha_t *)arg1;
  dt_key_t key = 0;
  
  static dtiact_but_t					/* a two-member structure 			*/
    runbut[DTDIALOG_NBUTTONS] = {
      DTIACT_BUT_HALT,					/* initialize both members 			*/
      DTIACT_BUT_HELP,
      DTIACT_BUT_SWAP,
      DTIACT_BUT_EXIT,
      DTIACT_BUT_TOP,
      DTIACT_BUT_BOT
  };
  
  static dtiact_but_t 					/* a two-member structure 			*/
    waitbut[DTDIALOG_NBUTTONS] = {
      DTIACT_BUT_RUN,					/* initialize both members 			*/
      DTIACT_BUT_HELP,
      DTIACT_BUT_SWAP,
      DTIACT_BUT_EXIT,
      DTIACT_BUT_TOP,
      DTIACT_BUT_BOT
  };

  switch(op)
  {
    case DTCONS_OP_ENTER:				/* ................................	*/
      DT_Q(dtdialog_hide,(ctl,			/* un-hide the dialog			    */
        &alpha->dialog,
        DTDIALOG_HIDE_FLAG_OFF));
      DT_Q(dtalpha_update,(ctl,			/* send an update to the alpha	    */
        alpha, &key));
      if (arg2)
        *(dt_key_t *)arg2 = key;
    break;
    case DTCONS_OP_LEAVE:				/* ................................	*/
      DT_Q(dtdialog_hide,(ctl,			/* hide the dialog				    */
        &alpha->dialog,
        DTDIALOG_HIDE_FLAG_CLEAR));
    break;
    case DTCONS_OP_STATUS:				/* ................................	*/
      DT_Q(dtalpha_status,(ctl,
        alpha, (char *)arg2));
    break;
    case DTCONS_OP_UPDATE:				/* ................................	*/
      DT_Q(dtiact_twirl,(ctl,			/* do twirl for this status			*/
        &alpha->dialog,
        alpha->status));
      DT_Q(dtiact_buttons,(ctl,			/* set up buttons for this status	*/
        &alpha->dialog,
        alpha->status, 
        waitbut, runbut));
      DT_Q(dtalpha_update,(ctl,			/* update the alpha				    */
        alpha, &key));
      if (arg2)
        *(dt_key_t *)arg2 = key;
    break;
    case DTCONS_OP_INPUT:				/* ................................	*/
      DT_Q(dtalpha_input,(ctl,
        alpha, &key));
      if (arg2)
        *(dt_key_t *)arg2 = key;
    break;
    case DTCONS_OP_CLOSE:				/* ................................	*/
	{
      dtdialog_t dialog = alpha->dialog;
      dtnote_t note = alpha->note;

      dt_dbg_trace(ctl, F,
        DT_RCSID_STRING, __LINE__, 
        "&alpha->dialog = 0x%08lx",
        &alpha->dialog);

      DT_Q(dtiact_buttons,(ctl,			/* set up buttons for this status	*/
        &alpha->dialog,
        alpha->status, 
        waitbut, runbut));


	  if (ctl->cons != NULL)			/* we are doing console sharing? 	*/
	  {
		dtcons_t *cons = 
		  (dtcons_t *)ctl->cons;

		dt_dbg_trace(ctl, F,
          DT_RCSID_STRING, __LINE__, 
          "resetting pointers from console,"
          " dbg_string 0x%08lx, dbgto 0x%08lx",
          cons->dbg_string, cons->dbgto);
		
		ctl->err_string = cons->err_string;
		ctl->dbg_string = cons->dbg_string;
		ctl->say_string = cons->say_string;
		ctl->errto = cons->errto;
		ctl->dbgto = cons->dbgto;
		ctl->sayto = cons->sayto;
	  }
	  else
	  {
		dt_dbg_trace(ctl, F,
          DT_RCSID_STRING, __LINE__, 
          "resetting pointers to NULL");

		ctl->err_string = NULL;
		ctl->dbg_string = NULL;
		ctl->say_string = NULL;
		ctl->errto = NULL;
		ctl->dbgto = NULL;
		ctl->sayto = NULL;
	  }

      dt_dbg_trace(ctl, F,
        DT_RCSID_STRING, __LINE__, 
        "pointers are now reset");

      DT_Q(dtalpha_finish,(ctl, alpha));
      DT_Q(dtdialog_uninit,(ctl, &dialog));
      DT_Q(dtmem_free,(ctl, note.mem,
        F, "alpha lines"));
      DT_Q(dtos_free2,(ctl, alpha,
        F, "alpha structure"));

	}
    break;
  }
  return DT_RC_GOOD;
}


/*..........................................................................*/

static
void
out_string_alpha(
  dt_ctl_t *ctl,
  dtalpha_t *alpha,
  const char *string,
  int length)
{
  DT_F("dt_interact_out_string");
  const char *q = string;
  dt_err_string_f *err_string = ctl->err_string;
  dt_dbg_string_f *dbg_string = ctl->dbg_string;
  dt_say_string_f *say_string = ctl->say_string;
  void *errto = ctl->errto;
  void *dbgto = ctl->dbgto;
  void *sayto = ctl->sayto;
  dt_rc_e rc = DT_RC_GOOD;

  if (ctl->cons != NULL)				/* we are doing console sharing? 	*/
  {
	dtcons_t *cons = 
      (dtcons_t *)ctl->cons;
	ctl->err_string = cons->err_string;	/* grab environment console saved 	*/
	ctl->dbg_string = cons->dbg_string;
	ctl->say_string = cons->say_string;
	ctl->errto = cons->errto;
	ctl->dbgto = cons->dbgto;
	ctl->sayto = cons->sayto;
  }
  else									/* console not available? 			*/
  {
	ctl->err_string = NULL;
	ctl->dbg_string = NULL;
	ctl->say_string = NULL;
	ctl->errto = NULL;
	ctl->dbgto = NULL;
	ctl->sayto = NULL;
  }

  if (length < 0)
    length = strlen(q);

  do {
    int l;
    for (l=0;							/* search for newline or end of q   */
         l < length && q[l] != '\n';
         l++);

    DT_C(dtnote_append,(ctl,			/* append message contents to note	*/
      &alpha->note, q, l));

    if (l < length)						/* newline in message?			    */
      DT_C(dtnote_add,(ctl,				/* make new line in note		    */
        &alpha->note, "", 1));

    q += l + 1;							/* point past line we just did	    */
    length -= l + 1;					/* how much of q is left		    */
  } while(length > 0);					/* until q exhausted			    */

cleanup:
  if (rc == DT_RC_GOOD ||				/* compiler wants us to use rc	    */
      rc != DT_RC_GOOD)					/* just how smart is it?		    */
  {
    ctl->err_string = err_string;
    ctl->dbg_string = dbg_string;
    ctl->say_string = say_string;
    ctl->errto = errto;
    ctl->dbgto = dbgto;
    ctl->sayto = sayto;
  }
}

/*..........................................................................*/
void 
DTCONFIG_API1
dtiact_out_string_err( 
dt_ctl_t *ctl, 
const char *string, 
int length)
{ out_string_alpha(ctl, (dtalpha_t *)ctl->errto, string, length); }

/*..........................................................................*/
void 
DTCONFIG_API1
dtiact_out_string_dbg( 
dt_ctl_t *ctl, 
const char *string, 
int length)
{ out_string_alpha(ctl, (dtalpha_t *)ctl->dbgto, string, length); }

/*..........................................................................*/
void 
DTCONFIG_API1
dtiact_out_string_say( 
dt_ctl_t *ctl, 
const char *string, 
int length)
{ out_string_alpha(ctl, (dtalpha_t *)ctl->sayto, string, length); }

/*..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtiact_newalpha(
  dt_ctl_t *ctl,
  dtalpha_t **palpha,
  char *tag)
{
  DT_F("dtiact_newalpha");
  dtalpha_t *alpha = NULL;
  dtnote_t note;
  dtdialog_t dialog;
  dtdialog_t *ctl_dialog =
    (dtdialog_t *)ctl->dialog;
  int have_dialog = 0;
  dt_rc_e rc = DT_RC_GOOD;

  memset(&note, 0, sizeof(note));

  DT_C(dtos_malloc2,(ctl,				/* space for the alpha structure	*/
    (void **)&alpha,
    sizeof(*alpha),
    F, "alpha structure"));

  DT_C(dtmem_alloc,(ctl,				/* space for the alpha lines	    */
    &note.mem, 
    0, 0, 4096, 4,
    F, "alpha lines"));

  DT_C(dtcons_add,(ctl,					/* register it with cons manager	*/
    tag,
    dtiact_cons_opfunc, 
    alpha));

  DT_C(dtdialog_init2,(ctl,				/* make a dialog				    */
    &dialog,
    ctl_dialog, 
    ctl_dialog->init,
    ctl_dialog->subinit,
    tag, 
    DTDIALOG_FLAG_VSMALL,
    NULL));
  have_dialog = 1;
   
  DT_C(dtalpha_begin,(ctl,				/* make an alpha				    */
    alpha, &dialog, &note));
 
  *palpha = alpha;

  return DT_RC_GOOD;

cleanup:
  if (have_dialog)
    DT_I(dtdialog_uninit,(ctl,
      &dialog));

  if (note.mem)
    DT_I(dtmem_free,(ctl, note.mem,
      F, "alpha lines"));
    
  if (alpha)
    DT_I(dtos_free2,(ctl, alpha,
      F, "alpha structure"));

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
