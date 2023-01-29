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
| textchat_op_dialog - dialog
| 
|.DESCRIPTION
| Undocumented.
| 
| END
 *..........................................................................*/

#include <textchat.h>
DT_RCSID("app/textchat $RCSfile: dialog.c,v $ $Revision: 1.8 $");

#include <dtack/str.h>

/*..........................................................................
 *..........................................................................*/

dt_rc_e 
DTCONFIG_API1
textchat_dialog_update(
  textchat_t *textchat)
{
  DT_F("textchat_dialog_update");
  dt_ctl_t *ctl = textchat->ctl;

  if (!TEXTCHAT_HAVE_DIALOG(textchat))
    return DT_RC_GOOD;

  if (textchat->update)					/* screen needs redrawing? 			*/
  {
	DT_Q(dtstr_printchar,(ctl,			/* make new state string 			*/
      (char)textchat->state,
      textchat->state_contents,
      sizeof(textchat->state_contents)));
	textchat->state_field->changed = 1;
	textchat->message_field->changed = 1;
	textchat->partner_field->changed = 1;

	DT_Q(dtdialog_input,(ctl,			/* draw the screen					*/
      &textchat->dialog, 
      NULL, NULL));
	
	textchat->update = 0;
  }
  else									/* screen doesn't need redrawing? 	*/
  {
	DT_Q(dtdialog_twirl,(ctl,			/* just breathe 					*/
      &textchat->dialog, -1, -1, 
      DTDIALOG_TWIRL_FLAG_ADVANCE));
  }

  return DT_RC_GOOD;
}

/*..........................................................................
 * handle incoming request from ipc
 *..........................................................................*/

dt_rc_e 
DTCONFIG_API1
textchat_dialog_poll(
  textchat_t *textchat,
  char **request)
{
  DT_F("textchat_dialog_poll");
  dt_ctl_t *ctl = textchat->ctl;
  dt_key_t key;
  int is_key;
  char *p;

  if (!TEXTCHAT_HAVE_DIALOG(textchat))
    return DT_RC_GOOD;

  DT_Q(dtdialog_key,(ctl,				/* poll for keyboard input			*/
    &textchat->dialog, 
    &is_key, &key));

  if (!is_key)							/* no key hit? 						*/
    return DT_RC_GOOD;

  switch(key)
  {
#   define KEY(K, R) \
	  case DT_KEY_ ## K: \
        DT_Q(dtos_strdup,(ctl,\
        "GET /" R, request)); break;
	KEY(F1, "talk");
	KEY(F2, "listen");
	KEY(F4, "partner?xxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
	KEY(F5, "exit");
  }

  if (*request == NULL)					/* key ignored? 					*/
    return DT_RC_GOOD;

  p = strstr(*request, "partner?");

  if (p != NULL)						/* need partner name? 				*/
  {
	do {
	  dtdialog_field_t *field =
	    textchat->partner_field;
	  DT_Q(dtdialog_input,(ctl,			/* draw the screen					*/
        &textchat->dialog, 
        &field, &key));
	} while (key != DT_KEY_ENTER);		/* wait for enter key 				*/
	
	DT_Q(dtstr_printf,(ctl,				/* remember the partner name 		*/
      p+strlen("partner?"),
      strlen(p)-strlen("partner?"),
      "%s",
      textchat->partner_contents));
  }

  return DT_RC_GOOD;
}

/*..........................................................................*/

static
dt_rc_e
textchat_dialog_fields(
  textchat_t *textchat)
{
  DT_F("textchat_dialog_fields");
  dt_ctl_t *ctl = textchat->ctl;
  dtdialog_t *dialog = &textchat->dialog;
  int row = 1;

  DT_Q(dtdialog_field,(ctl,
    dialog, 0, row, NULL, 
    "F1=Talk   F2=Listen   F4=partner   F5=Exit", 0));
  
  row += 2;
  DT_Q(dtdialog_field,(ctl,				/* us label 						*/
    dialog, 0,  row, NULL, 
    "Us", 0));

  DT_Q(dtdialog_field,(ctl,				/* us field 						*/
    dialog, 12, row, NULL, 
    (char *)textchat->ident, 0));

  row += 2;
  DT_Q(dtdialog_field,(ctl,				/* partner label 					*/
    dialog, 0,  row, NULL, 
    "Partner", 0));

  textchat->partner_field = 
    &dialog->field[dialog->nfields];
  DT_Q(dtdialog_field,(ctl,				/* partner field 					*/
    dialog, 12, row, NULL, 
    textchat->partner_contents,
    sizeof(textchat->partner_contents)));
  strcpy(textchat->partner_contents, 
   "");

  row += 2;
  DT_Q(dtdialog_field,(ctl,				/* message label 					*/
    dialog, 0, row, NULL, 
    "Message", 0));

  textchat->message_field = 
    &dialog->field[dialog->nfields];
  DT_Q(dtdialog_field,(ctl,				/* message field 					*/
    dialog, 12,  row, NULL, 
    textchat->message_contents,
    sizeof(textchat->message_contents)));
  strcpy(textchat->message_contents, 
   "");

  row += 2;
  DT_Q(dtdialog_field,(ctl,				/* state label 						*/
    dialog, 0, row, NULL, 
    "State", 0));

  textchat->state_field = 
    &dialog->field[dialog->nfields];
  DT_Q(dtdialog_field,(ctl,				/* state field 						*/
    dialog, 12,  row, NULL, 
    textchat->state_contents,
    sizeof(textchat->state_contents)));
  strcpy(textchat->state_contents, 
    "state");

  textchat->update = 1;

  return DT_RC_GOOD;
}

/*..........................................................................
 * initialize textchat structure
 *..........................................................................*/

dt_rc_e 
DTCONFIG_API1
textchat_dialog_init(
  textchat_t *textchat,
  dtparam_t *param)
{
  DT_F("textchat_dialog_init");
  dt_ctl_t *ctl = &param->ctl;
  dt_rc_e rc = DT_RC_GOOD;

  DT_C(dtdialog_init,(ctl,				/* initialize a dialog 				*/
    &textchat->dialog, NULL,
    param->dialog,
    "Textchat", 0L,
    param->keyboard));
  textchat->dialog_initialized = 1;

  DT_C(textchat_dialog_fields,(			/* make fields for display 			*/
    textchat));		

  DT_C(dtdialog_twirl,(ctl,				/* initialize twirl 				*/
    &textchat->dialog, -1, -1, 
    DTDIALOG_TWIRL_FLAG_START));

cleanup:

  return rc;
}

/*..........................................................................
 * uninitialize textchat structure
 *..........................................................................*/

dt_rc_e 
DTCONFIG_API1
textchat_dialog_uninit(
  textchat_t *textchat)
{
  DT_F("textchat_dialog_uninit");
  dt_ctl_t *ctl = textchat->ctl;
  dt_rc_e rc = DT_RC_GOOD;

  if (textchat->dialog_initialized)
  {
	DT_I(dtdialog_uninit,(ctl,
      &textchat->dialog));
	textchat->dialog_initialized = 0;
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
