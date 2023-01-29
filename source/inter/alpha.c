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
| Function return value will be DT_RC_GOOD on success.  
| Any other return value indicates failure or noncompletion.
| When DT_RC_GOOD is not returned, any other return values may not be valid.
| 
| ERRORS
| As yet undocumented.
| 
| END
 *..........................................................................*/

#include <dtack/base.h>

DT_RCSID("source/inter $RCSfile: alpha.c,v $ $Revision: 1.2 $");
#include <dtack/dialog.h>

#include <dtack/os.h>
#include <dtack/str.h>

#include <dtack/cons.h>
#include <dtack/note.h>
#include <dtack/alpha.h>

#define S(I) (alpha->s+(alpha->w+1)*I)

/*..........................................................................*/
dt_rc_e
DTCONFIG_API1
dtalpha_begin(
  dt_ctl_t *ctl,
  dtalpha_t *alpha,
  dtdialog_t *dialog,
  dtnote_t *note)
{
  DT_F("dtalpha_begin");
  int i;
  dt_rc_e rc = DT_RC_GOOD;

  memset(alpha, 0, sizeof(*alpha));		/* start with clean structure	    */

  alpha->xe = dialog->xe;
  alpha->ye = dialog->ye;
  alpha->w = alpha->xe;					/* for possible attrib chars   	    */
  alpha->h = alpha->ye - 1 - 1;			/* msg/at top, input/status bot		*/
  alpha->pos = 0;
  alpha->top = 0;
  alpha->note = *note;
  alpha->dialog = *dialog;				/* copy dialog structure in alpha   */
  dialog = &alpha->dialog;				/* use internal one from now on	    */

  DT_C(dtos_malloc2,(ctl,				/* space for display lines		    */
    (void **)&alpha->s,
    alpha->h*(alpha->w+1),
    F, "display buffer"));

  DT_C(dtos_malloc2,(ctl,				/* space for field pointers		    */
    (void **)&alpha->field,
    alpha->h*sizeof(*alpha->field),
    F, "field pointers"));

  DT_C(dtdialog_field,(ctl,				/* make command field label		    */
    dialog, 0, alpha->ye-1,
    DTALPHA_FIELDNAME_COMMAND,
    "command", 
    DTDIALOG_FIELD_NORMAL));

  alpha->input_field = &dialog->field[
    dialog->nfields];
  DT_C(dtdialog_field,(ctl,				/* make command input field		    */
    dialog, 7, alpha->ye-1,
    DTALPHA_FIELDNAME_INPUT, 
    alpha->input,
    sizeof(alpha->input)));
  alpha->input[0] = '\0';

  alpha->status_field = &dialog->field[
    dialog->nfields];
  DT_C(dtdialog_field,(ctl,				/* make status display field	    */
    dialog,
    alpha->xe-sizeof(alpha->status)-2,
    alpha->ye-1,
    DTALPHA_FIELDNAME_STATUS,
    alpha->status, DTDIALOG_FIELD_NORMAL));
  alpha->status[0] = '\0';

  alpha->at_field = &dialog->field[
    dialog->nfields];
  DT_C(dtdialog_field,(ctl,				/* make at display field	   		*/
    dialog,
    alpha->xe-sizeof(alpha->at)-2,
    0,
    DTALPHA_FIELDNAME_AT,
    alpha->at, DTDIALOG_FIELD_NORMAL));
  alpha->at[0] = '\0';

  alpha->msg_field = &dialog->field[
    dialog->nfields];
  DT_C(dtdialog_field,(ctl,				/* make msg display field	   		*/
    dialog,
    0, 0,
    DTALPHA_FIELDNAME_ERROR,
    alpha->msg,
    DTDIALOG_FIELD_HIGHLIGHT |
    DTDIALOG_FIELD_BEEP));
  alpha->msg[0] = '\0';

  for (i=0; i<alpha->h; i++)			/* for each display line		    */
  {
    alpha->field[i] = &dialog->field[
      dialog->nfields];
    DT_C(dtdialog_field,(ctl,			/* define line as field contents    */
      dialog,
      0, 1+i, 
      DTALPHA_FIELDNAME_LINE,
      S(i), 
      DTDIALOG_FIELD_NORMAL |
      DTDIALOG_FIELD_FIXED));
  }

  DT_C(dtalpha_refield,(ctl,			/* compute fields on dialog	    	*/
    alpha, 0));

  DT_Q(dtdialog_input,(ctl,				/* refresh screen with no waiting	*/
    dialog, NULL, NULL));

  return DT_RC_GOOD;

cleanup:
  DT_I(dtalpha_finish,(ctl, alpha));

  return rc;
}


/*..........................................................................*/

static
dt_rc_e
dtalpha_at(
  dt_ctl_t *ctl,
  dtalpha_t *alpha)
{
  DT_F("dtalpha_at");
  if (alpha->painted &&					/* alpha already painted?		    */
      alpha->at_bot == alpha->bot &&	/* and no change to this field?	    */
      alpha->at_n   == alpha->note.n)
    return DT_RC_GOOD;

  alpha->at_bot = alpha->bot;
  alpha->at_n   = alpha->note.n;

  DT_Q(dtstr_printf,(ctl,
    alpha->at, sizeof(alpha->at),
    "%d/%d %d%%%s",
    alpha->bot, alpha->note.n,
    100 * alpha->bot /
    (alpha->note.n + !alpha->note.n),
    alpha->note.full? " FULL": ""));
  alpha->at_field->changed = 0;
  return DT_RC_GOOD;
}

/*..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtalpha_refield(
  dt_ctl_t *ctl,
  dtalpha_t *alpha,
  int delta)
{
  DT_F("dtalpha_refield");
  int newt;
  int i, i0;
  char *s;

  newt = alpha->top + delta;

  if (newt + alpha->h > alpha->note.n)
    newt = alpha->note.n - alpha->h;

  if (newt < 0)
    newt = 0;

  delta = newt - alpha->top;

  if (alpha->painted &&					/* initial draw is done?		    */
      delta == 0)						/* no movement has occured?		    */
  {
    if (alpha->note.n == alpha->bot)	/* no new lines added? 				*/
      return DT_RC_GOOD;
    if (alpha->note.n >					/* or new lines are not visible?	*/
        alpha->top+alpha->h &&
        alpha->bot ==					/* and screen completely full?		*/
        alpha->top+alpha->h)
	{
      DT_Q(dtalpha_at,(ctl, alpha));	/* update "at" field			   	*/
      return DT_RC_GOOD;
	}
  }

  DT_Q(dtnote_setpos,(ctl,				/* go back to old top line		    */
    &alpha->note, &alpha->cursor,
    alpha->pos));

  if (delta < 0)						/* squirm backwards through notes   */
  {
    int d = -delta;
    for (i=0; i<d; i++)
	{
      DT_Q(dtnote_prev,(ctl,
        &alpha->note, &alpha->cursor,
        &s));
      if (s == NULL)
        return dt_err(ctl, F,
          "going off top of note alpha->top %d, delta %d",
          alpha->top, delta);
	}
  }
  else									/* squirm forwards through notes    */
  {
    for (i=0; i<delta; i++)
	{
      DT_Q(dtnote_next,(ctl,
        &alpha->note, &alpha->cursor,
        &s));
      if (s == NULL)
        return dt_err(ctl, F,
          "going off bottom of note alpha->top %d, delta %d",
          alpha->top, delta);
	}
  }

  alpha->pos = alpha->cursor.pos;		/* remember pos of new top line	    */
  if (alpha->top == newt)				/* top line not changing?		    */
  {
    i0 = alpha->bot - alpha->top;		/* output starting from old bot	    */
  }
  else									/* top line is changed?			    */
  {
    alpha->top = newt;					/* remember new top				    */
    i0 = 0;								/* need to output all lines		    */
  }

  for (i=0; i<i0; i++)					/* squirm down to first output	    */
  {
    DT_Q(dtnote_next,(ctl,
      &alpha->note, &alpha->cursor,
      &s));
    if (s == NULL)
      return dt_err(ctl, F,
        "i %d, i0 %d, top %d, delta %d",
        i, i0, alpha->top, delta);
  }

  for (i=i0; i<alpha->h; i++)
  {
    char *t;
    int j;
    j = i + alpha->top;
    if (j >= alpha->note.n)				/* no more lines in note?		    */
	  s = "~";
	else
      DT_Q(dtnote_next,(ctl,			/* get line from note			    */
        &alpha->note, &alpha->cursor,
        &s));

    t = S(i);							/* line's place in big string	    */
    sprintf(t, "%03d ", j);
    strncat(t, s,						/* put line in dialog			    */
      alpha->w-strlen(t));
    alpha->field[i]->changed = 0;		/* mark field for changed		    */
  }

  alpha->bot = DT_MIN(alpha->note.n,	/* bottom line on display		    */
    alpha->top+alpha->h);

  if (alpha->bot < alpha->note.n)		/* time to scroll lock?				*/
    alpha->scroll_lock = 1;
  else
    alpha->scroll_lock = 0;
    
  alpha->painted = 1;					/* initial draw is done			    */

  DT_Q(dtalpha_at,(ctl, alpha));		/* update "at" field			   	*/

  return DT_RC_GOOD;
}

/*..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtalpha_load(
  dt_ctl_t *ctl,
  dtalpha_t *alpha,
  char *filename)
{
  DT_F("dtalpha_load");
  dtfd_t fd;
  dt_rc_e rc = DT_RC_GOOD;
  char b[256];
  long got;

  DT_Q(dtfd_open,(ctl, &fd,				/* open text file to read		    */
    filename, "rt"));

  b[0] = '\0';
  do {
    int l = strlen(b);
    char *e, *s;
    DT_C(dtfd_read,(ctl, &fd, b+l,		/* read next chunk from file  	    */
      sizeof(b)-1-l, &got));
    l += (int)got;						/* current length of buffer		    */
    b[l] = '\0';
    s = b;
    do {
      e = strchr(s, '\n');
      if (e)
	  {
        int n = (int)(e-s);
        if (n == 0)						/* blank line?					    */
  		  DT_C(dtnote_add,(ctl,			/* add blank char to alpha		    */
            &alpha->note, " ", 2))
        else							/* not a blank line?			    */
		{
          s[n] = '\0';					/* nuke the newline				   y */
  		  DT_C(dtnote_add,(ctl,			/* add chars to alpha			    */
            &alpha->note, s, n+1));
		}
        s = e + 1;
	  }
    } while (e);
    if (got == 0 ||						/* last line in file?			    */
        b == s)							/* very long line?				    */
	{
      if (*s)
	  {
  	    DT_C(dtnote_add,(ctl,			/* add line to alpha			    */
          &alpha->note, s, l+1));
	  }
      b[0] = '\0';						/* next input goes on its own line	*/
	}
    else								/* partial line left in buffer?	    */
	{
      int n = strlen(s);
      int i;
      for (i=0; i<=n; i++)				/* shuffle down the partial line	*/
        b[i] = s[i];
	}
  } while(got);

cleanup:
  DT_Q(dtfd_close,(ctl, &fd));

  return rc;
}

/*..........................................................................*/

static
dt_rc_e
dtalpha_write(
  dt_ctl_t *ctl,
  dtalpha_t *alpha,
  dtfd_t *fd)
{
  DT_F("dtalpha_write");
  long pos;
  int i;
  dt_rc_e rc = DT_RC_GOOD;

  pos = alpha->cursor.pos;				/* remember pos of display line	    */
  DT_Q(dtnote_setpos,(ctl,				/* go to top of alpha				*/
    &alpha->note, &alpha->cursor,
    0));

  for (i=0; i<alpha->note.n; i++)		/* for all lines in alpha's note    */
  {
    char *s;
    DT_C(dtnote_next,(ctl,				/* get line from note			    */
      &alpha->note, &alpha->cursor,
      &s));
    if (s == NULL)						/* don't expect this			    */
	{
      rc = dt_err(ctl, F,
        "going off bottom of note at i=%d",
        i);
      break;
	}
    DT_C(dtfd_write_all,(ctl, fd,		/* put line into output file	    */
      s, strlen(s)));
    DT_C(dtfd_write_all,(ctl, fd,
      "\n", 1));
  }

cleanup:
  DT_I(dtnote_setpos,(ctl,				/* return to top old position		*/
    &alpha->note, &alpha->cursor,
    pos));

  return rc;
}

/*..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtalpha_save(
  dt_ctl_t *ctl,
  dtalpha_t *alpha,
  char *filename)
{
  DT_F("dtalpha_save");
  dtfd_t fd;
  dt_rc_e rc;

  DT_Q(dtfd_open,(ctl, &fd,				/* open text file to write		    */
    filename, "wt"));				

  DT_G(dtalpha_write,(ctl, alpha,		/* write to open output file	    */
    &fd));

  DT_Q(dtfd_close,(ctl, &fd));

  return rc;
}

/*..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtalpha_append(
  dt_ctl_t *ctl,
  dtalpha_t *alpha,
  char *filename)
{
  DT_F("dtalpha_append");
  dtfd_t fd;
  dt_rc_e rc;

  DT_Q(dtfd_open,(ctl, &fd,				/* open text file to append		    */
    filename, "at"));				

  DT_G(dtalpha_write,(ctl, alpha,		/* write to open output file	    */
    &fd));

  DT_Q(dtfd_close,(ctl, &fd));

  return rc;
}

/*..........................................................................*/
static
dt_rc_e
dtalpha_process_cmd(
  dt_ctl_t *ctl,
  dtalpha_t *alpha,
  dt_key_t *key)
{
  DT_F("dtalpha_process_cmd");
  int cmd;

  static const char *cmds[] = {
    "nop",
    "up",
    "down",
    "pgup",
    "pgdown",
    "help",
    "swap",
    "top",
    "bottom",
    "f5",
    "f6",
    "f7",
    "f8",
    "clear",
    "run",
    "exit",
    "timer",
    "save",
    "append",
    "load",
    NULL,
  };
#define MAXARGV (10)
  char argline[sizeof(alpha->input)];
  char *argv[MAXARGV];
  int argc;

  if (*key != DT_KEY_ENTER)				/* only process command at ENTER    */
    return DT_RC_GOOD;

  strcpy(argline, alpha->input);		/* copy of input for parsing	    */

  DT_Q(dtstr_argv,(ctl, argline,		/* parse line into tokens		    */
    argv, MAXARGV, &argc));

  if (argc == 0)						/* no tokens on line?			    */
    return DT_RC_GOOD;

  DT_Q(dtstr_choice,(ctl, cmds,			/* look up command in choice list	*/
    argv[0], &cmd));

# define ARGS(MIN, MAX) \
  if (argc-1 < MIN || \
      argc-1 > MAX) \
  { \
    DT_Q(dtstr_printf,(ctl, \
      alpha->msg, sizeof(alpha->msg), \
      "Sorry, %s tokens on command \"%s\"", \
      (argc-1<MIN)? "not enough": "too many", \
      alpha->input)); \
    alpha->msg_field->changed = 0; \
    break; \
  }

  switch(cmd)							/* index int "cmds" array		    */
  {
    case 0:  ARGS(0, 0); *key = 0; break;
    case 1:  ARGS(0, 0); *key = DT_KEY_UPARROW; break;
    case 2:  ARGS(0, 0); *key = DT_KEY_DOWNARROW; break;
    case 3:  ARGS(0, 0); *key = DT_KEY_PAGEUP; break; 
    case 4:  ARGS(0, 0); *key = DT_KEY_PAGEDOWN; break; 
    case 5:  ARGS(0, 0); *key = DTCONS_KEY_HELP; break;
    case 6:  ARGS(0, 0); *key = DTCONS_KEY_SWAP; break;
    case 7:  ARGS(0, 0); *key = DTCONS_KEY_TOP; break;
    case 8:  ARGS(0, 0); *key = DTCONS_KEY_BOT; break;
    case 9:  ARGS(0, 0); *key = DT_KEY_F5; break;
    case 10: ARGS(0, 0); *key = DT_KEY_F6; break;
    case 11: ARGS(0, 0); *key = DT_KEY_F7; break;
    case 12: ARGS(0, 0); *key = DT_KEY_F8; break;
    case 13: ARGS(0, 0); *key = DT_KEY_F9; break;
    case 14: ARGS(0, 0); *key = DTCONS_KEY_RUNSTOP; break;
    case 15: ARGS(0, 0); *key = DTCONS_KEY_EXIT; break;
    case 16: ARGS(0, 0); *key = DT_KEY_TIMER; break;

    case 17: ARGS(1, 1); 
      DT_Q(dtalpha_save,(ctl, alpha, argv[1])); 
      DT_Q(dtstr_printf,(ctl,
        alpha->msg, sizeof(alpha->msg),
        "File \"%s\" successfully saved",
        argv[1]));
      alpha->msg_field->changed = 0;
    break;

    case 18: ARGS(1, 1); 
      DT_Q(dtalpha_append,(ctl, alpha, argv[1])); 
      DT_Q(dtstr_printf,(ctl,
        alpha->msg, sizeof(alpha->msg),
        "File \"%s\" successfully appended",
        argv[1]));
      alpha->msg_field->changed = 0;
    break;

    case 19: ARGS(1, 1); 
      DT_Q(dtalpha_load,(ctl, alpha, argv[1])); 
      DT_Q(dtstr_printf,(ctl,
        alpha->msg, sizeof(alpha->msg),
        "File \"%s\" successfully loaded",
        argv[1]));
      alpha->msg_field->changed = 0;
    break;

    case -1: *key = 0;
      DT_Q(dtstr_printf,(ctl,
        alpha->msg, sizeof(alpha->msg),
        "Sorry, didn't understand command \"%s\"",
        alpha->input));
      alpha->msg_field->changed = 0;
    break;
    case -2: *key = 0;
      DT_Q(dtstr_printf,(ctl,
        alpha->msg, sizeof(alpha->msg),
        "Sorry, ambiguous command \"%s\"",
        alpha->input));
      alpha->msg_field->changed = 0;
    break;
    default: *key = 0;
  }

  return DT_RC_GOOD;
}

/*..........................................................................*/
static
dt_rc_e
dtalpha_process_key(
  dt_ctl_t *ctl,
  dtalpha_t *alpha,
  dt_key_t key,
  int *delta,
  int *quit)
{
  DT_F("dtalpha_process_key");
  char *cmd;
  cmd = "";
  *delta = 0;

  if (key >= DT_KEY_JUMP_MIN &&
      key <= DT_KEY_JUMP_MAX)
  {
    *quit = 1; 
    cmd = " ";
  }
  else
  switch(key)
  {
    case DT_KEY_UPARROW:     *delta = -1;            cmd = "up"; break;
    case DT_KEY_DOWNARROW:   *delta = 1;             cmd = "down"; break;
    case DT_KEY_PAGEDOWN:    *delta = alpha->h-1;    cmd = "pgdown"; break;
    case DT_KEY_PAGEUP:      *delta = -(alpha->h-1); cmd = "pgup"; break; 
                          
    case DTCONS_KEY_HELP:    *quit  = 1;             cmd = "help"; break;
    case DTCONS_KEY_RUNSTOP: *quit  = 1;             cmd = "done"; break;
    case DTCONS_KEY_EXIT:    *quit  = 1;             cmd = "exit"; break;
    case DTCONS_KEY_SWAP:    *quit  = 1;             cmd = " ";     break;
    case DTCONS_KEY_TOP:                             cmd = "top";
      alpha->scroll_lock = 1;
      alpha->top = 0;
      alpha->bot = 0;
      alpha->pos = 0;
      alpha->painted = 0;
    break;
    case DTCONS_KEY_BOT:                             cmd = "bot";
      *delta = alpha->note.n;
    break;
    case DT_KEY_F9:                                  cmd = "clear";
      DT_Q(dtnote_clear,(ctl,
        &alpha->note));
      alpha->top = 0;
      alpha->bot = 0;
      alpha->pos = 0;
      alpha->painted = 0;
    break;
    case DT_KEY_TIMER:     *quit  = 1;               cmd = "timer"; break;
  }

  if (*cmd)
  {
    strcpy(alpha->input, cmd);
    alpha->input_field->changed = 0;

    if (alpha->msg[0] != '\0')			/* we have existing message?	    */
    {
      alpha->msg[0] = '\0';				/* clear message				    */
      alpha->msg_field->changed = 0;
    }
  }

  return DT_RC_GOOD;
}

/*..........................................................................
 *
 *..........................................................................*/
dt_rc_e
DTCONFIG_API1
dtalpha_status(
  dt_ctl_t *ctl,
  dtalpha_t *alpha,
  char *status)
{
  unsigned int l = strlen(status);
  l = DT_MIN(l, 
    sizeof(alpha->status)-1);
  if (memcmp(alpha->status, status, l)) /* field contents have changed?    */
  {
    memcpy(alpha->status, status, l);
    alpha->status[l] = '\0';
    alpha->status_field->changed = 0;	/* mark contents of field new	    */
  }
  return DT_RC_GOOD;
}

/*..........................................................................
 * key is returned as 0 if no key was hit
 *..........................................................................*/
dt_rc_e
DTCONFIG_API1
dtalpha_update(
  dt_ctl_t *ctl,
  dtalpha_t *alpha,
  dt_key_t *key)
{
  DT_F("dtalpha_update");
  int iskey;
  int delta = 0;
  int quit = 0;

  do {
    if (delta == 0 &&
        !alpha->scroll_lock &&
        alpha->note.n > alpha->h)		/* more than a screenfull?			*/
      delta = alpha->note.n - 			/* autoscroll						*/
        alpha->bot;

    DT_Q(dtalpha_refield,(ctl,			/* recompute fields on dialog	    */
      alpha, delta));

    DT_Q(dtdialog_input,(ctl,			/* refresh screen with no waiting	*/
      &alpha->dialog, NULL, NULL));

    DT_Q(dtdialog_key,(ctl,				/* check for keyboard input		    */
      &alpha->dialog, &iskey, key));

    if (iskey)							/* key was hit?					    */
      DT_Q(dtalpha_process_key,(ctl,	/* process any key hit				*/
        alpha, *key, &delta, &quit))
    else								/* no key was hit?				    */
    if (key)							/* but caller wants key status?	    */
      *key = 0;

  } while(iskey && !quit);				/* process until no key or quit key	*/

  return DT_RC_GOOD;
}

/*..........................................................................*/
dt_rc_e
DTCONFIG_API1
dtalpha_input(							/* wait for keyboard input		    */
  dt_ctl_t *ctl,
  dtalpha_t *alpha,
  dt_key_t *key)
{
  DT_F("dtalpha_input");
  int delta = 0;
  int quit = 0;

  do {
    DT_Q(dtalpha_refield,(ctl,			/* recompute fields on dialog	    */
      alpha, delta));

    DT_Q(dtdialog_input,(ctl,			/* display and wait for input		*/
      &alpha->dialog, NULL, key));

    DT_Q(dtalpha_process_cmd,(ctl,		/* process the command input		*/
      alpha, key));

    DT_Q(dtalpha_process_key,(ctl,		/* process any key hit				*/
      alpha, *key, &delta, &quit));

  } while(!quit);

  return DT_RC_GOOD;
}

/*..........................................................................*/
dt_rc_e
DTCONFIG_API1
dtalpha_finish(
  dt_ctl_t *ctl,
  dtalpha_t *alpha)
{
  DT_F("dtalpha_finish");
  dt_rc_e rc = DT_RC_GOOD;

  dt_dbg_trace(ctl, F,
    DT_RCSID_STRING, __LINE__, 
    "alpha = 0x%08lx",
    alpha);

  if (alpha->field)						/* field pointers allocated?	    */
    DT_I(dtos_free2,(ctl,				/* free them					    */
      alpha->field,
      F, "field pointers"));

  if (alpha->s)							/* text buffer allocated?		    */
    DT_I(dtos_free2,(ctl,				/* free it						    */
      alpha->s,
      F, "display buffer"));

  memset(alpha, 0, sizeof(*alpha));		/* extra security				    */

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
