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
DT_RCSID("source/inter $RCSfile: diatext.c,v $ $Revision: 1.2 $");
#include <dtack/os.h>
#include <dtack/dialog.h>
#include <dtack/str.h>
#include <dtack/cons.h>
#include <dtack/keymap.h>
#include <dtack/keybd.h>

#include <dtack/text.h>

typedef struct {
  dttext_t text;
  char topline[256];
  int itwirl;
} dtdialog_text_priv_t;

static dtdialog_uninit_f dtdialog_text_uninit;
static dtdialog_button_f dtdialog_text_button;
static dtdialog_input_f dtdialog_text_input;
static dtdialog_key_f dtdialog_text_key;
static dtdialog_hide_f dtdialog_text_hide;
static dtdialog_twirl_f dtdialog_text_twirl;

#define DTDIALOG_TEXT_NTWIRL (4)

#define Y(IY) (IY+1)					/* offset for title				    */
#define FILLER '-'

/*..........................................................................*/
static
dt_rc_e
dtdialog_text_title(					/* output title						*/
  dt_ctl_t *ctl,
  dtdialog_t *dialog)
{
  DT_F("dtdialog_text_title");
  dtdialog_text_priv_t *priv =
    (dtdialog_text_priv_t *)dialog->priv;
  char *old_topline = priv->topline;
  char new_topline[sizeof(priv->topline)];
  int m = 0;
  int m1;
  int i, l;
  const char *tag, *tag1 = NULL;
  int iscurr;
  const int lmax = DT_MIN((unsigned int)dialog->xe,
    sizeof(priv->topline));
  dt_rc_e rc = DT_RC_GOOD;

  DT_Q(dtstr_printf,(ctl,
    new_topline, lmax,
    "Program: %s --", 
    (ctl && ctl->progname)?
      ctl->progname: ""));

  m = strlen(new_topline);

  i = 0;
  do {
	DT_Q(dtcons_tag,(ctl, i++,			/* get ith cons tag					*/
      &tag, &iscurr));
	if (tag == NULL)
	  break;
	l = strlen(tag);
	DT_Q(dtstr_printf,(ctl,
      new_topline+m, lmax-m,
      "- %s -", tag));
	if (iscurr)
	{
	  tag1 = tag;
	  m1 = m + 2;
	}
	m += l + 4;
  } while(tag);

  memset(new_topline+m, '-', lmax-m);
  new_topline[lmax-1] = '\0';

  if (strcmp(new_topline, old_topline))	/* topline has changed?				*/
  {
    DT_GI(dttext_out,(ctl,				/* topline at the top				*/
      &priv->text,
      new_topline,
      0, 0,
      DTTEXT_FLAG_NORMAL));
	if (tag1 != NULL)
      DT_GI(dttext_out,(ctl,			/* topline at the top				*/
        &priv->text,
        tag1,
        m1, 0,
        DTTEXT_FLAG_HIGHLIGHT));
    memcpy(old_topline, new_topline, lmax);
  }

  return DT_RC_GOOD;
}

/*..........................................................................*/
dt_rc_e
DTCONFIG_API1
dtdialog_text_init(						/* init structure					*/
  dt_ctl_t *ctl,
  dtdialog_t *dialog,
  dtdialog_t *existing,
  char *init,
  const char *keyboard)
{
  DT_F("dtdialog_text_init");
  dtdialog_text_priv_t *priv = NULL;
  dt_rc_e rc = DT_RC_GOOD;

  if (!existing)
  {
    DT_C(dtos_malloc2,(ctl,				/* space for priv structure			*/
      (void **)&priv,
      sizeof(*priv),
      F, "priv"));
	memset(priv, 0, sizeof(*priv));

	if (keyboard)
  	  DT_C(dtos_strdup,(ctl, keyboard,
        (char **)&priv->text.keyboard_name));

    DT_C(dttext_beg,(ctl,				/* do the "beg" method			    */
      &priv->text, init));
	dialog->keyboard = (void *)			/* so upper levels can print maps 	*/
      priv->text.keyboard;
 
    dialog->init = dtdialog_text_init; 
    dialog->uninit = dtdialog_text_uninit; 
    dialog->button = dtdialog_text_button; 
    dialog->input = dtdialog_text_input; 
    dialog->hide = dtdialog_text_hide; 
    dialog->twirl = dtdialog_text_twirl; 
    dialog->key = dtdialog_text_key; 

    dialog->existing = 0;
  }
  else
  {
    priv = (dtdialog_text_priv_t *)existing->priv;

    dialog->init = existing->init; 
    dialog->uninit = existing->uninit; 
    dialog->button = existing->button; 
    dialog->input = existing->input; 
    dialog->hide = existing->hide; 
    dialog->twirl = existing->twirl; 
    dialog->key = existing->key;
    dialog->keyboard = existing->keyboard;

    dialog->existing = 1;
  }

  dialog->priv = priv;

  dialog->xe = priv->text.xe;			/* usable area back from method	    */
  dialog->ye = priv->text.ye -			/* actual size of screen			*/
    (dialog->title? 1: 0) -				/* set aside one for possible title	*/
    1;									/* set aside one for buttons	    */

  return DT_RC_GOOD;
cleanup:
  if (priv->text.keyboard_name)
    DT_I(dtos_free2,(ctl, 
      (void *)priv->text.keyboard_name,
      F, "priv->keyboard_name"));
  if (priv)
    DT_I(dtos_free2,(ctl, priv,
      F, "priv"));
  return rc;
}

/*..........................................................................*/
static
dt_rc_e
dtdialog_text_uninit(					/* uninit structure					*/
  dt_ctl_t *ctl,
  dtdialog_t *dialog)
{
  DT_F("dtdialog_text_uninit");
  dt_rc_e rc = DT_RC_GOOD;
  dtdialog_text_priv_t *priv =
    (dtdialog_text_priv_t *)dialog->priv;

  if (!dialog->existing)
  {
    DT_I(dttext_end,(ctl,
      &priv->text));
	
    if (priv->text.keyboard_name)
      DT_I(dtos_free2,(ctl, 
        (void *)priv->text.keyboard_name,
        F, "priv->keyboard_name"));

    DT_I(dtos_free2,(ctl, priv,			/* free our priv structure	    */
      F, "priv"));  
  }

  return rc;
}

/*..........................................................................*/
static
dt_rc_e
dtdialog_text_edit_field(				/* edit on one field			    */
  dt_ctl_t *ctl,
  dtdialog_t *dialog,
  dtdialog_field_t *field,
  dt_key_t *pkey)
{
  DT_F("dtdialog_text_edit_field");
  dtdialog_text_priv_t *priv =
    (dtdialog_text_priv_t *)dialog->priv;
  dttext_t *text = &priv->text;
  dt_key_t key;
  int x;
  int i, l, lastl;
  int first = 1;
  int field_x = field->x + 2;			/* offset for the colon indicator	*/
  char keystring[2];
  const int lmax = DT_MIN(				/* limit field end to end of screen	*/
    field->lmax-1,
    dialog->xe-1-field_x);
  int quit = 0;

  keystring[1] = '\0';

  DT_Q(dttext_out,(ctl,					/* highlight input field		    */
    &priv->text,
    field->contents, 
    field_x, Y(field->y),
    DTTEXT_FLAG_HIGHLIGHT));

  DT_Q(dttext_out,(ctl,					/* bracket before input field	    */
    &priv->text,
    "[", 
    field_x-1, Y(field->y),
    DTTEXT_FLAG_NORMAL));
  DT_Q(dttext_out,(ctl,					/* bracket after input field	    */
    &priv->text,
    "]", 
    field_x+lmax, Y(field->y),
    DTTEXT_FLAG_NORMAL));

  x = 0;
  l = strlen(field->contents);			/* current length of contents	    */

  do {
    DT_Q(dttext_out,(ctl, text,			/* move cursor						*/
      NULL,
      field_x+x, Y(field->y),
      DTTEXT_FLAG_CURSOR));
    DT_Q(dialog->key,(ctl,				/* get keyboard input				*/
      dialog, NULL, &key))

    if (key == DT_KEY_ENTER ||			/* special key we don't want?		*/
        key == DT_KEY_ESCAPE ||
        key == DT_KEY_TAB ||
        key == DT_KEY_F1 ||
        key == DT_KEY_F2 ||
        key == DT_KEY_F3 ||
        key == DT_KEY_F4 ||
        key == DT_KEY_F5 ||
        key == DT_KEY_F6 ||
        key == DT_KEY_F7 ||
        key == DT_KEY_F8 ||
        key == DT_KEY_F9 ||
        key == DT_KEY_PAGEUP ||
        key == DT_KEY_PAGEDOWN ||
        key == DT_KEY_UPARROW ||
        key == DT_KEY_DOWNARROW)
      quit = 1;
    else
    if (first)							/* first key hit on this field?	    */
	{
      if (key != DT_KEY_RIGHTARROW &&	/* not arrow movement?			    */
          key != DT_KEY_LEFTARROW &&
          key != DT_KEY_HOME &&
          key != DT_KEY_END &&
          key != DT_KEY_ERASEEOL)
        field->contents[0] = '\0';		/* clear input string			    */
      first = 0;
	}

    if (!quit)
    if (key==DT_KEY_BACKSPACE && x>0)	/* backspace?					    */
	{
      x--;
      for (i=x; i<l; i++)				/* remove character from string	    */
        field->contents[i] =
          field->contents[i+1];
	}
    else
    if (key==DT_KEY_DELETE && x<l)
	{
      for (i=x; i<l; i++)				/* remove character from string	    */
        field->contents[i] =
          field->contents[i+1];
	}
    else
    if (key==DT_KEY_LEFTARROW && x==0)	/* left arrow when at left edge?    */
      quit = 1;
    else
    if (key==DT_KEY_LEFTARROW && x>0)	/* left arrow in middle?		    */
      x--;
    else
    if (key==DT_KEY_RIGHTARROW && x<l)
      x++;
    else
    if (key==DT_KEY_HOME)
      x = 0;
    else
    if (key==DT_KEY_END)
      x = l;
    else
    if (key==DT_KEY_ERASEEOL)
      field->contents[x] = '\0';
    else
    if (isprint(key) &&					/* non-edit key?				    */
        x < field->lmax-1)
	{
      for (i=l; i>x; i--)				/* make room for new char		    */
        field->contents[i] =
          field->contents[i-1];
      field->contents[l+1] = '\0';		/* string is now longer			    */
      field->contents[x] = key;			/* add new char					    */
      x++;
	}

    lastl = l;							/* old length					    */
    l = strlen(field->contents);		/* new length					    */
    for (i=l; i<lastl; i++)
      field->contents[i] = ' ';
    field->contents[i] = '\0';

    DT_Q(dttext_out,(ctl, text,			/* un-highlight current string		*/
      field->contents,
      field_x, Y(field->y),
      DTTEXT_FLAG_NORMAL));

    field->contents[l] = '\0';			/* set string to new length		    */

    DT_Q(dttext_out,(ctl, text,			/* place cursor						*/
      NULL, field_x+x, Y(field->y),
      DTTEXT_FLAG_CURSOR));
  } while(!quit);

  DT_Q(dttext_out,(ctl,					/* un-bracket before input field    */
    &priv->text,
    " ", 
    field_x-1, Y(field->y),
    DTTEXT_FLAG_NORMAL));
  DT_Q(dttext_out,(ctl,					/* un-bracket after input field	   */
    &priv->text,
    " ", 
    field_x+lmax, Y(field->y),
    DTTEXT_FLAG_NORMAL));

  *pkey = key;
  return DT_RC_GOOD;
}

/*..........................................................................*/
static
dt_rc_e
dtdialog_text_blank(					/* blank a field					*/
  dt_ctl_t *ctl,
  dtdialog_t *dialog,
  int x,
  int y,
  int l)
{
  DT_F("dtdialog_text_blank");
  dtdialog_text_priv_t *priv =
    (dtdialog_text_priv_t *)dialog->priv;
# define B (64)
  char b[B+1];
  int n, i;

  if (l <= 0)							/* want/need no filling?		    */
    return DT_RC_GOOD;

  n = l / B;
  memset(b, ' ', B);
  b[B] = '\0';
  for (i=0; i<n; i++)					/* clear full parts				    */
  {
    DT_Q(dttext_out,(ctl,
      &priv->text, b,
      x, y, 
      DTTEXT_FLAG_NORMAL));
    x += B;
  }

  l = l % B;							/* partial clearing left over	    */
  b[l] = '\0';
  if (l)
    DT_Q(dttext_out,(ctl,
      &priv->text, b,
      x, y, 
      DTTEXT_FLAG_NORMAL));

  return DT_RC_GOOD;
}

/*..........................................................................*/
static
dt_rc_e
dtdialog_text_button(					/* define button					*/
  dt_ctl_t *ctl,
  dtdialog_t *dialog,
  char *label,							/* button's label				    */
  int pos,								/* horizontal relative position	    */
  dt_key_t key)							/* key-equivalent to return		    */
{
  DT_F("dtdialog_text_button");
  dtdialog_field_t *butfield =			/* for short					    */
    &dialog->butfield[pos];
  char tmp[DTDIALOG_BUTTON_MAXLEN];

  if (pos >= DTDIALOG_NBUTTONS)
    return dt_err(ctl, F,
      "invalid button position %d (max %d)",
      pos, DTDIALOG_NBUTTONS);

  if (label)							/* this button being turned on?		*/
  {
    if (key == DT_KEY_ENTER)
      DT_Q(dtstr_printf,(ctl,			/* make actual button contents		*/
        tmp, sizeof(tmp), 
        "-ENTER:%s---------",
        label))
    else
    if (key == DT_KEY_ESCAPE)
      DT_Q(dtstr_printf,(ctl,			/* make actual button contents		*/
        tmp, sizeof(tmp), 
        "--ESC:%s----------",
        label))
    else
      DT_Q(dtstr_printf,(ctl,			/* make actual button contents		*/
        tmp, sizeof(tmp), 
        "---F%d:%s---------",
        key-DT_KEY_F1+1, label))
  }
  else									/* button turned off?				*/
  {
    memset(tmp, '-',
      DTDIALOG_BUTTON_MAXLEN-1);
    tmp[DTDIALOG_BUTTON_MAXLEN-1] = '\0';
  }
  
  butfield->contents = 
    dialog->butlabel +
    (pos * DTDIALOG_BUTTON_MAXLEN);

  if (strcmp(butfield->contents, tmp))	/* button contents are different?	*/
  {
    strcpy(butfield->contents, tmp);	/* new contents						*/
    butfield->changed = 0;				/* mark changed						*/
  }

  butfield->y = dialog->ye + 1;
  butfield->x = pos * (DTDIALOG_BUTTON_MAXLEN - 1);
  butfield->lmax = DTDIALOG_BUTTON_MAXLEN - 1;

  return DT_RC_GOOD;
}

/*..........................................................................*/
static
dt_rc_e
dtdialog_text_input(					/* show fields and take input		*/
  dt_ctl_t *ctl,
  dtdialog_t *dialog,
  dtdialog_field_t **start,
  dt_key_t *key)
{
  DT_F("dtdialog_text_input");
  dtdialog_text_priv_t *priv =
    (dtdialog_text_priv_t *)dialog->priv;
  int i;
  dtdialog_field_t *curfield;
  int quit = 0;
  int nout = 0;

  if (dialog->hidden)					/* this dialog is hidden?			*/
    return DT_RC_GOOD;

  DT_Q(dtdialog_text_title,(ctl,		/* output title line			    */
    dialog));

  for (i=0; i<dialog->nfields; i++)		/* show all fields				    */
  {
    dtdialog_field_t *field =			/* for short					    */
      &dialog->field[i];
    int flag = DTTEXT_FLAG_NORMAL;
    int x;
	int lmax;

    if (field->lmax > 0)				/* it's an input field?			    */
	{
      DT_Q(dttext_out,(ctl,
        &priv->text,					/* indicate is in an input field	*/
        ": ", field->x, Y(field->y),
        DTTEXT_FLAG_NORMAL));
      x = field->x + 2;
	  lmax = field->lmax;
	}
	else								/* it's not an input field		    */
	{
      if (!field->changed)				/* field has new contents?		    */
	  {
        x = field->x;					/* where to output it			    */
		lmax = field->l;
        field->changed = 1;				/* mark it no longer new		    */
        nout++;							/* count for debug				    */
	  }
	  else								/* field has old contents?		    */
        x = -1;							/* don't output it				    */
      if (field->lmax & 
          DTDIALOG_FIELD_HIGHLIGHT &
          ~DTDIALOG_FIELD_MASK)
        flag = DTTEXT_FLAG_HIGHLIGHT;
	}

    lmax = DT_MIN(lmax-1,				/* limit field end to end of screen	*/
      dialog->xe-1-field->x);

    if (x >= 0)							/* supposed to do it?			    */
	{
      int l = strlen(field->contents);

      DT_Q(dttext_out,(ctl,				/* display field contents		    */
        &priv->text, 
        field->contents, 
        x, Y(field->y), flag));
	  
      DT_Q(dtdialog_text_blank,(ctl,	/* clear previous garbage		    */
        dialog, x+l, Y(field->y),
        lmax-l));

      field->l = l;
	}
  }

  for (i=0; i<DTDIALOG_NBUTTONS; i++)	/* show all buttons				    */
  {
    dtdialog_field_t *butfield =		/* for short					    */
      &dialog->butfield[i];
    if (!butfield->changed &&			/* button has new contents?		    */
         butfield->contents)			/* button has any contents			*/
    {
      butfield->changed = 1;			/* mark it no longer new		    */
      nout++;							/* count for debug				    */
      DT_Q(dttext_out,(ctl,				/* display button contents		    */
        &priv->text, 
        butfield->contents, 
        butfield->x, butfield->y,
        DTTEXT_FLAG_NORMAL));
	}
  }

#ifdef NONO
  {
    char out[32];
    sprintf(out, "%d ", nout);
    DT_Q(dttext_out,(ctl,				/* say how many fields we output	*/
      &priv->text, 
      out, 0, 0, 
      DTTEXT_FLAG_NORMAL));
  }
#endif

  if (start)							/* caller has given start field		*/
    curfield = *start;
  else
    curfield = NULL;

  if (curfield == NULL)					/* caller has not specified start?	*/
    DT_Q(dtdialog_transit,(ctl,			/* find first input field		    */
      dialog, DT_KEY_HOME,
      NULL, &curfield));

  if (key == NULL)						/* caller does not want input?	    */
  {
    DT_Q(dttext_out,(ctl, 				/* move cursor causes update		*/
      &priv->text,
      NULL,
      curfield->x,
      curfield->y,
      DTTEXT_FLAG_CURSOR));
    return DT_RC_GOOD;
  }

  do {
    DT_Q(dtdialog_text_edit_field,(		/* edit the field				    */
      ctl, dialog, 
      curfield, key))		

#ifdef NONO
  {
    char out[32];
    sprintf(out, "curfield \"%s\"", curfield->contents);
    DT_Q(dttext_out,(ctl,
      &priv->text, 
      out, 0, 0, 
      DTTEXT_FLAG_NORMAL));
  }
#endif

    if (*key == DT_KEY_UPARROW ||		/* field-transit attention key?	    */
        *key == DT_KEY_DOWNARROW ||
        *key == DT_KEY_LEFTARROW ||
        *key == DT_KEY_TAB)
	{
      dtdialog_field_t *newfield;
      DT_Q(dtdialog_transit,(ctl,		/* find next input field		    */
        dialog, *key,
        curfield, &newfield));
      if (curfield == newfield)			/* only one input field?		    */
        quit = 1;						/* any attention key returns	    */
      else
        curfield = newfield;
	}
    else								/* non field-transit attention key?	*/
      quit = 1;
  } while(!quit);						/* keep going until attention		*/

  if (start)							/* caller wants field back			*/
    *start = curfield;

  return DT_RC_GOOD;
}

/*..........................................................................*/
static
dt_rc_e
dtdialog_text_hide(						/* hide screen from further input	*/
  dt_ctl_t *ctl,
  dtdialog_t *dialog,
  int flags)
{
  DT_F("dtdialog_text_hide");
  dtdialog_text_priv_t *priv =
    (dtdialog_text_priv_t *)dialog->priv;
  int i;

  if (flags & DTDIALOG_HIDE_FLAG_CLEAR)	/* supposed to clear when hiding?	*/
    DT_Q(dttext_clear,(ctl,
      &priv->text));

  if (flags & DTDIALOG_HIDE_FLAG_OFF)	/* really turning off hiding?		*/
  {
    dialog->hidden = 0;					/* simply mark unhidden			    */
    for (i=0; i<dialog->nfields; i++)	/* also mark all fields for redraw	*/
	  dialog->field[i].changed = 0;
    for (i=0; i<DTDIALOG_NBUTTONS; i++)	/* also mark all buttons for redraw	*/
	  dialog->butfield[i].changed = 0;
  }
  else
  {
    dialog->hidden = 1;					/* simply mark hidden			    */
    priv->topline[0] = '\0';			/* make top line get regenerated	*/
  }

  return DT_RC_GOOD;
}

/*..........................................................................*/
static
dt_rc_e
dtdialog_text_twirl(					/* display twirl					*/
  dt_ctl_t *ctl,
  dtdialog_t *dialog,
  int x,
  int y,
  int flags)
{
  DT_F("dtdialog_text_twirl");
  dtdialog_text_priv_t *priv =
    (dtdialog_text_priv_t *)dialog->priv;
  static char *twirl[DTDIALOG_TEXT_NTWIRL+1] =
    {"|", "/", "-", "\\", " "};

  if (dialog->hidden)					/* this dialog is hidden?			*/
    return DT_RC_GOOD;

  if (x == -1) x = dialog->xe - 2;		/* caller wants default location?	*/
  if (y == -1) y = dialog->ye - 1;

  if (flags &							/* stop twirling?					*/
      DTDIALOG_TWIRL_FLAG_STOP)
    priv->itwirl = 
      DTDIALOG_TEXT_NTWIRL;
  else
  if (flags &							/* start twirling?					*/
      DTDIALOG_TWIRL_FLAG_START)
    priv->itwirl = 0;
  else
  if (flags &							/* advance twirling?				*/
      DTDIALOG_TWIRL_FLAG_ADVANCE &&
      priv->itwirl <					/* twirling not off?			    */
      DTDIALOG_TEXT_NTWIRL)
    priv->itwirl = 
      (priv->itwirl+1) %
        DTDIALOG_TEXT_NTWIRL;

  DT_Q(dttext_out,(ctl,					/* display twirl contents		    */
    &priv->text, 
    twirl[priv->itwirl],
    dialog->xe - 2,
    dialog->ye - 1,
    DTTEXT_FLAG_NORMAL));

  return DT_RC_GOOD;
}

/*..........................................................................*/
static
dt_rc_e
dtdialog_text_key(					   
  dt_ctl_t *ctl,
  dtdialog_t *dialog,
  int *iskey,
  dt_key_t *key)
{
  DT_F("dtdialog_text_input");
  dtdialog_text_priv_t *priv =
    (dtdialog_text_priv_t *)dialog->priv;

  DT_Q(dttext_key,(ctl,					/* wait for keyboard input			*/
    &priv->text, iskey, key)); 
   
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
