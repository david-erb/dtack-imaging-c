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
| dt_ - dt
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
DT_RCSID("source/inter $RCSfile: diavba.c,v $ $Revision: 1.2 $");
#include <dtack/dialog.h>

static dtdialog_input_f dtdialog_vba_input;

#define EMIT printf(
#define TIME ); printf("%c\n", 0x0d);
#define DX(XX) ((XX))
#define DY(YY) ((YY))
#define AX(XX) DX(XX)
#define AY(YY) DY(YY)

/*..........................................................................*/
dt_rc_e
DTCONFIG_API1
dtdialog_vba_init(						/* init structure					*/
  dt_ctl_t *ctl,
  dtdialog_t *dialog,
  dtdialog_t *existing,
  char *init,
  const char *keyboard)
{
  memset(dialog, 0, sizeof(dialog));

  if (!existing)
  {
	dialog->init = dtdialog_vba_init;
	dialog->input = dtdialog_vba_input;
 
    dialog->existing = 0;
  }
  else
  {
    dialog->uninit = existing->uninit; 
    dialog->button = existing->button; 
    dialog->input = existing->input; 
    dialog->hide = existing->hide; 
    dialog->twirl = existing->twirl; 
    dialog->key = existing->key;
    dialog->keyboard = existing->keyboard;

    dialog->existing = 1;
  }

  dialog->xe = 80;
  dialog->ye = 25;

  return DT_RC_GOOD;
}

/*..........................................................................*/
static
dt_rc_e
dtdialog_vba_input(						/* show fields in VB.FMR format		*/
  dt_ctl_t *ctl,
  dtdialog_t *dialog,
  dtdialog_field_t **start,
  dt_key_t *key)
{
  
#define stop
#ifdef stop
  DT_F("dtdialog_vba_input");
  dt_dbg(ctl, F, DT_DBG_MASK_ASYNC,
    "returning STOP");
  return DT_RC_STOP;
#else
  return DT_RC_GOOD;
#endif
}

#ifdef NONO
/*..........................................................................*/
static
dt_rc_e
dtdialog_vba_input(						/* show fields in VB.FMR format		*/
  dt_ctl_t *ctl,
  dtdialog_t *dialog,
  dtdialog_field_t **start,
  dt_key_t *key)
{
  int i, nparam, narg;

  EMIT "VERSION 2.00"                 TIME
  EMIT "Begin Form %s", ctl->progname TIME
  EMIT "  ClientHeight    =   7410"   TIME
  EMIT "  ClientLeft      =   1050"   TIME
  EMIT "  ClientTop       =   1425"   TIME
  EMIT "  ClientWidth     =   9270"   TIME
  EMIT "  Height          =   7815"   TIME
  EMIT "  Left            =    990"   TIME
  EMIT "  ScaleHeight     =   7410"   TIME
  EMIT "  ScaleWidth      =   9270"   TIME
  EMIT "  Top             =   1080"   TIME
  EMIT "  Width           =   9390"   TIME
  EMIT "' ------------------------------------------" TIME

  nparam = 0;
  narg = 0;
  for (i=0; i<dialog->nfields; i++)		/* field definitions				*/
  {
    dtdialog_field_t *field =			/* for short					    */
      &dialog->field[i];

    if (field->lmax > 0)				/* it's an input field?			    */
	{
	  const char *type;
	  int n;
	  if (field->name[0] == '-')		/* it's a param input field?		*/
	  {
		n = nparam++;
		type = "param";
	  }
	  else
	  if (!strncmp(field->name, "arg", 3))	/* it's an arg input field?		    */
	  {
		n = narg++;
		type = "arg";
	  }
	  else
	    continue;
      EMIT "  Begin TextBox %s_text", type                 TIME
      EMIT "    Text    = \"%s\"", field->contents         TIME
      EMIT "    Width   = %d", 32                          TIME
      EMIT "    Height  = %d", DY(1)                       TIME
      EMIT "    Left    = %d", AX(field->x+2)              TIME
      EMIT "    Top     = %d", AY(field->y)                TIME
      EMIT "    Index   = %d", n                           TIME
      EMIT "  End" TIME
	}
	else								/* it's not an input field		    */
	{
	  const char *type;
	  int n;
	  if (field->name[0] == '-')
	  {
		n = nparam;
		type = "param";
	  }
	  else
	  if (!strcmp(field->name, "arg"))	/* it's an arg output field?	    */
	  {
		n = narg;
		type = "arg";
	  }
	  else
	    continue;
      EMIT "  Begin Label %s_label", type                  TIME
      EMIT "    Caption = \"%s\"", field->contents         TIME
      EMIT "    Width   = %d", DX(strlen(field->contents)) TIME
      EMIT "    Height  = %d", DY(1)                       TIME
      EMIT "    Left    = %d", AX(field->x)                TIME
      EMIT "    Top     = %d", AY(field->y)                TIME
      EMIT "    Index   = %d", n                           TIME
      EMIT "  End" TIME
	}
  }

  EMIT "End" TIME						/* end of form part				    */

  EMIT "' ------------------------------------------" TIME
  EMIT "' Declarations" TIME
  EMIT "Const narg   = %d", narg   TIME
  EMIT "Const nparam = %d", nparam TIME
  EMIT "Static abbrev(%d) as String", nparam TIME

  EMIT "' ------------------------------------------" TIME
  EMIT "Sub assign_abbrevs ()" TIME
  nparam = 0;
  for (i=0; i<dialog->nfields; i++)		/* parameter field constants	    */
  {
    dtdialog_field_t *field =			/* for short					    */
      &dialog->field[i];
    if (field->name[0] == '-' &&
		field->lmax > 0)				/* it's an input param field?	    */
	{
	  EMIT "  abbrev(%d) = \"%s\"", 
        nparam++, field->name TIME
	}
  }
  EMIT "End Sub" TIME

  return DT_RC_STOP;
}
#endif






/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
  ! Copyright (C) 1998 Dtack Inc. All Rights Reserved
  ! To use this file, you must have signed a license agreement with Dtack Inc.
  ! Under no circumstances may you redistribute this file.
  ! This software is provided AS IS with no warranty expressed or implied.
  ! Dtack Inc. accepts no liability for use or misuse of this file.
  ! http://www.dtack.com  dtack@dtack.com  telephone +360.417.6700
  ! Dtack Inc, P.O. Box 3061, Port Angeles, WA USA 98362
  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */
