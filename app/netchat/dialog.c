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
| netchat_op_dialog - dialog
| 
|.DESCRIPTION
| Undocumented.
| 
| END
 *..........................................................................*/

#include <netchat.h>
DT_RCSID("app/netchat $RCSfile: dialog.c,v $ $Revision: 1.8 $");

#include <dtack/str.h>

/*..........................................................................
 *..........................................................................*/

dt_rc_e 
DTCONFIG_API1
netchat_dialog_update(
  netchat_t *netchat)
{
  DT_F("netchat_dialog_update");
  dt_ctl_t *ctl = netchat->ctl;

  if (!NETCHAT_HAVE_DIALOG(netchat))
    return DT_RC_GOOD;

  if (netchat->update)					/* screen needs redrawing? 			*/
  {
	DT_Q(dtstr_printchar,(ctl,			/* make new state string 			*/
      (char)netchat->state,
      netchat->state_contents,
      sizeof(netchat->state_contents)));
	netchat->state_field->changed = 1;
	netchat->message_field->changed = 1;
	netchat->partner_field->changed = 1;

	DT_Q(dtdialog_input,(ctl,			/* draw the screen					*/
      &netchat->dialog, 
      NULL, NULL));
	
	netchat->update = 0;
  }
  else									/* screen doesn't need redrawing? 	*/
  {
	DT_Q(dtdialog_twirl,(ctl,			/* just breathe 					*/
      &netchat->dialog, -1, -1, 
      DTDIALOG_TWIRL_FLAG_ADVANCE));
  }

  return DT_RC_GOOD;
}

/*..........................................................................
 * handle incoming request from ipc
 *..........................................................................*/

dt_rc_e 
DTCONFIG_API1
netchat_dialog_poll(
  netchat_t *netchat,
  char **request)
{
  DT_F("netchat_dialog_poll");
  dt_ctl_t *ctl = netchat->ctl;
  dt_key_t key;
  int is_key;
  char *p;

  if (!NETCHAT_HAVE_DIALOG(netchat))
    return DT_RC_GOOD;

  DT_Q(dtdialog_key,(ctl,				/* poll for keyboard input			*/
    &netchat->dialog, 
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
	    netchat->partner_field;
	  DT_Q(dtdialog_input,(ctl,			/* draw the screen					*/
        &netchat->dialog, 
        &field, &key));
	} while (key != DT_KEY_ENTER);		/* wait for enter key 				*/
	
	DT_Q(dtstr_printf,(ctl,				/* remember the partner name 		*/
      p+strlen("partner?"),
      strlen(p)-strlen("partner?"),
      "%s",
      netchat->partner_contents));
  }

  return DT_RC_GOOD;
}

/*..........................................................................*/

static
dt_rc_e
netchat_dialog_fields(
  netchat_t *netchat)
{
  DT_F("netchat_dialog_fields");
  dt_ctl_t *ctl = netchat->ctl;
  dtdialog_t *dialog = &netchat->dialog;
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
    (char *)netchat->ident, 0));

  row += 2;
  DT_Q(dtdialog_field,(ctl,				/* partner label 					*/
    dialog, 0,  row, NULL, 
    "Partner", 0));

  netchat->partner_field = 
    &dialog->field[dialog->nfields];
  DT_Q(dtdialog_field,(ctl,				/* partner field 					*/
    dialog, 12, row, NULL, 
    netchat->partner_contents,
    sizeof(netchat->partner_contents)));
  strcpy(netchat->partner_contents, 
   "");

  row += 2;
  DT_Q(dtdialog_field,(ctl,				/* message label 					*/
    dialog, 0, row, NULL, 
    "Message", 0));

  netchat->message_field = 
    &dialog->field[dialog->nfields];
  DT_Q(dtdialog_field,(ctl,				/* message field 					*/
    dialog, 12,  row, NULL, 
    netchat->message_contents,
    sizeof(netchat->message_contents)));
  strcpy(netchat->message_contents, 
   "");

  row += 2;
  DT_Q(dtdialog_field,(ctl,				/* state label 						*/
    dialog, 0, row, NULL, 
    "State", 0));

  netchat->state_field = 
    &dialog->field[dialog->nfields];
  DT_Q(dtdialog_field,(ctl,				/* state field 						*/
    dialog, 12,  row, NULL, 
    netchat->state_contents,
    sizeof(netchat->state_contents)));
  strcpy(netchat->state_contents, 
    "state");

  netchat->update = 1;

  return DT_RC_GOOD;
}

/*..........................................................................
 * initialize netchat structure
 *..........................................................................*/

dt_rc_e 
DTCONFIG_API1
netchat_dialog_init(
  netchat_t *netchat,
  dtparam_t *param)
{
  DT_F("netchat_dialog_init");
  dt_ctl_t *ctl = &param->ctl;
  dt_rc_e rc = DT_RC_GOOD;

  DT_C(dtdialog_init,(ctl,				/* initialize a dialog 				*/
    &netchat->dialog, NULL,
    param->dialog,
    "Netchat", 0L,
    param->keyboard));
  netchat->dialog_initialized = 1;

  DT_C(netchat_dialog_fields,(			/* make fields for display 			*/
    netchat));		

  DT_C(dtdialog_twirl,(ctl,				/* initialize twirl 				*/
    &netchat->dialog, -1, -1, 
    DTDIALOG_TWIRL_FLAG_START));

cleanup:

  return rc;
}

/*..........................................................................
 * uninitialize netchat structure
 *..........................................................................*/

dt_rc_e 
DTCONFIG_API1
netchat_dialog_uninit(
  netchat_t *netchat)
{
  DT_F("netchat_dialog_uninit");
  dt_ctl_t *ctl = netchat->ctl;
  dt_rc_e rc = DT_RC_GOOD;

  if (netchat->dialog_initialized)
  {
	DT_I(dtdialog_uninit,(ctl,
      &netchat->dialog));
	netchat->dialog_initialized = 0;
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
