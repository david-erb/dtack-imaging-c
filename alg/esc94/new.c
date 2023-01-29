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
| dt_esc94 command - Levine&Nazif 1985
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
DT_RCSID("alg/esc94 $RCSfile: new.c,v $ $Revision: 1.1 $");
#include <dtack/os.h>
#include <dtack/str.h>
#include <dtack/dialog.h>

#define ESCAPE (0x1b)
#define TAB ('\t')
#define ENTER ('\n')

/*..........................................................................*/
dt_rc_e
dtdialog_curses_uninit(					/* uninit structure					*/
  dt_ctl_t *ctl,
  dtdialog_t *dialog)
{
/*DT_F("dtdialog_curses_uninit");*/

  wmove(stdscr, LINES-1, 0);
  wrefresh(curscr);
  endwin();
  fflush(stdout);
  
  return DT_RC_GOOD;
}

/*..........................................................................*/
static
int
edit_field(dtdialog_field_t *field)		/* edit on one field			    */
{
  int key;
  int x;
  int i;
  int first = 1;

  wmove(stdscr, field->y, field->x);	/* move to start of field		    */
  x = 0;
  do {
    wrefresh(stdscr);
    key = wgetch(stdscr);
    if (key == erasechar() && x > 0)
	{
      x--;
      field->contents[x] = '\0';
      wmove(stdscr, field->y,			/* move back one char on screen		*/
        x+field->x);
      wdelch(stdscr);
	}
    else
    if (isprint(key) &&					/* non-edit key?				    */
        x < field->lmax-1)
	{
      if (first)						/* first key hit?				    */
	  {
        for (i=0;						/* erase input field			    */
             i<strlen(field->contents);
             i++)
          wdelch(stdscr);
        field->contents[0] = '\0';		/* clear input string			    */
        first = 0;
	  }
      winsch(stdscr, key);				/* put key into display			  	*/
      field->contents[x] = key;
      field->contents[x+1] = '\0';
      x++;
      wmove(stdscr, field->y, x+field->x);
	}
  } while(key != ENTER &&				/* edit until special key		    */
          key != ESCAPE &&
          key != TAB);
 
  wmove(stdscr, field->y, field->x);	/* redisplay contents highlighted   */
  wstandout(stdscr);
  waddstr(stdscr, field->contents);
  wstandend(stdscr);
  wrefresh(stdscr);

  return key;
}

/*..........................................................................*/
dt_rc_e
dtdialog_curses_input(					/* show fields and take input		*/
  dt_ctl_t *ctl,
  dtdialog_t *dialog,
  dt_key_t *key)
{
  DT_F("dtdialog_curses_input");
  int i;

  for (i=0; i<dialog->nfields; i++)		/* show all fields				    */
  {
    dtdialog_field_t *field =			/* for short					    */
      &dialog->field[i];
    wmove(stdscr, field->y, field->x);
    if (field->lmax)					/* it's an input field?			    */
      wstandout(stdscr);				/* display highlighted			    */
    waddstr(stdscr, field->contents);	/* display field contents		    */
    if (field->lmax)
      wstandend(stdscr);
  }

  i = 0;
  do {
    if (dialog->field[i].lmax)			/* it is an input field?		   	*/
      *key = edit_field(				/* edit the field				    */
        &dialog->field[i]);		
    i += 1;								/* advance to next field		    */
    if (i >= dialog->nfields)
      i = 0;
  } while(*key != ENTER &&				/* keep going until return is hit   */
          *key != ESCAPE);				/* or escape is hit				    */

  return DT_RC_GOOD;
}

/*..........................................................................*/
dt_rc_e
dtdialog_curses_init(					/* init structure					*/
  dt_ctl_t *ctl,
  dtdialog_t *dialog,
  char *title)
{
/*DT_F("dtdialog_curses_init");*/
  static char *help =
    "RETURN: done  --  ESCAPE: abort  --  TAB: next field";
 
  dialog->init = dtdialog_curses_init; 
  dialog->uninit = dtdialog_curses_uninit; 
  dialog->input = dtdialog_curses_input; 

  initscr();
  cbreak();
  noecho();
  mvwaddstr(stdscr, 0,					/* title at the top				    */
    (COLS-strlen(dialog->title))/2,
    dialog->title);
  mvwaddstr(stdscr, LINES-1,			/* help at the bottom			    */
    (COLS-strlen(help))/2, help);

  return DT_RC_GOOD;
}

/*..........................................................................*/
dt_rc_e
dtdialog_init(							/* init structure					*/
  dt_ctl_t *ctl,
  dtdialog_t *dialog,
  char *name,
  char *title)
{
  DT_F("dtdialog_init");
  int choice;
  
  static const char *choices[] = {
    "curses",
    NULL
  };

  static dtdialog_init_f *methods[] = {
    dtdialog_curses_init
  };

  DT_Q(dtstr_choice,(ctl, choices,		/* look up dialog name			    */
    name, &choice));
  
  if (choice == -1)						/* unknown name					    */
    return dt_err(ctl, F,
      "unknown dialog spec \"%s\"",
      name);
  else
  if (choice == -2)						/* ambiguous name				    */
    return dt_err(ctl, F,
      "ambiguous dialog spec \"%s\"",
      name);

  DT_Q(dtos_strdup,(ctl, title,			/* space for the title string	    */
    &dialog->title));

  DT_Q(dtos_malloc2,(ctl,				/* space for the fields				*/
    (void **)&dialog->field,
    DTDIALOG_MAXFIELDS * 
    sizeof(dtdialog_field_t),
    F, "fields"));

  dialog->nfields = 0;
  dialog->maxfields = 
    DTDIALOG_MAXFIELDS;

  DT_Q((methods[choice]),(ctl,			/* chop off the dialog spec			*/
    dialog, title))

  return DT_RC_GOOD;
}

/*..........................................................................*/
dt_rc_e
dtdialog_uninit(						/* uninit structure					*/
  dt_ctl_t *ctl,
  dtdialog_t *dialog)
{
  DT_F("dtdialog_uninit");

  DT_Q(dialog->uninit,(ctl, dialog));	/* do the method				    */

  DT_Q(dtos_free2,(ctl,
    dialog->field,
    F, "fields"));

  DT_Q(dtos_free,(ctl,
    dialog->title));

  return DT_RC_GOOD;
}

/*..........................................................................*/
dt_rc_e
dtdialog_field(							/* define field						*/
  dt_ctl_t *ctl,
  dtdialog_t *dialog,
  int x,
  int y,
  char *contents,
  int lmax)
{
  DT_F("dtdialog_field");
  dtdialog_field_t *field;

  if (dialog->nfields >=
      dialog->maxfields)
    return dt_err(ctl, F,
      "too many fields in dialog, max is %d",
      dialog->maxfields);

  field = &dialog->field[dialog->nfields];
  memset(field, 0, sizeof(*field));

  field->contents = contents;
  field->lmax = lmax;
  field->x = x;
  field->y = y;

  dialog->nfields++;

  return DT_RC_GOOD;
}

/*..........................................................................*/
dt_rc_e
dtdialog_input(							/* take input						*/
  dt_ctl_t *ctl,
  dtdialog_t *dialog,
  dt_key_t *key)
{
  DT_F("dtdialog_input");

  DT_Q(dialog->input,(ctl, dialog,		/* do the method				    */
    key));

  return DT_RC_GOOD;
}

/*..........................................................................*/
/*..........................................................................*/






/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
  ! Copyright (C) 1998 Dtack Inc. All Rights Reserved
  ! To use this file, you must have signed a license agreement with Dtack Inc.
  ! Under no circumstances may you redistribute this file.
  ! This software is provided AS IS with no warranty expressed or implied.
  ! Dtack Inc. accepts no liability for use or misuse of this file.
  ! http://www.dtack.com  dtack@dtack.com  telephone +360.417.6700
  ! Dtack Inc, P.O. Box 3061, Port Angeles, WA USA 98362
  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */
