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
DT_RCSID("source/inter $RCSfile: dialog.c,v $ $Revision: 1.2 $");
#include <dtack/os.h>
#include <dtack/str.h>
#include <dtack/dialog.h>

/*..........................................................................*/
dt_rc_e
DTCONFIG_API1
dtdialog_init2(							/* init structure					*/
  dt_ctl_t *ctl,
  dtdialog_t *dialog,
  dtdialog_t *existing,
  dtdialog_init_f *init,
  char *subspec,
  const char *title,
  unsigned long flags,
  const char *keyboard)
{
  DT_F("dtdialog_init2");
  unsigned int n;
  dt_rc_e rc;

  memset(dialog, 0, sizeof(*dialog));	/* clear the structure			    */

  DT_C(dtos_strdup,(ctl, title,			/* space for the title string	    */
    &dialog->title));

  DT_C(dtos_strdup,(ctl, subspec,		/* space for the subspec string	    */
    &dialog->subinit));

  n = (DTDIALOG_MAXFIELDS +
       DTDIALOG_NBUTTONS) *
    sizeof(dtdialog_field_t);

  DT_C(dtos_malloc2,(ctl,				/* space for the fields				*/
    (void **)&dialog->field, n,
    F, "fields"));
  memset(dialog->field, 0, n);			/* clear fields					    */

  dialog->butfield = dialog->field + 	/* button fields after other fields	*/
    DTDIALOG_MAXFIELDS;

  dialog->nfields = 0;
  dialog->maxfields = 
    DTDIALOG_MAXFIELDS;

  dialog->flags = flags;

  DT_C((init),(ctl,						/* do begin method					*/
    dialog, existing, subspec,
    keyboard));

  return DT_RC_GOOD;
  
cleanup:
  if (dialog->field)
    DT_I(dtos_free2,(ctl,
      dialog->field, F, "fields"));
  if (dialog->subinit)
    DT_I(dtos_free2,(ctl,
      dialog->subinit, F, "subinit"));
  if (dialog->title)
    DT_I(dtos_free2,(ctl,
      dialog->title, F, "title"));
  return rc;
}

/*..........................................................................
| NAME
| dtdialog_init() - initialize dialog structure
| 
| SUMMARY
| Call an implementation function indicated by the first part of ~spec~. 
| Support one of the specs "text" or "msw" (Microsoft Windows).
| Allocate system resources such as windows if ~existing~ is NULL.
| Otherwise share the resources already allocated for an existing dialog.
| 
| Consider everything in ~spec~ past the first comma to the
| implementation dependent end pass it unchanged to the
| implementation's initialization method.
| Allocate and prepare structure members common to all implementations.
| The dialog initially has no visible text or input fields.
| Have an error if ~spec~ is NULL, empty or otherwise invalid.
| 
| END
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtdialog_init(							/* initialize dialog structure		*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtdialog_t *dialog,					/* dialog object description 		*/
  dtdialog_t *existing,					/* existing dialog object 			*/
  const char *spec,						/* dialog specification string 		*/
  const char *title,					/* dialog screen title 				*/
  unsigned long flags,					/* dialog appearance flags 			*/
  const char *keyboard)					/* keyboard description string 		*/
{
  DT_F("dtdialog_init");
  char dialogtype[64];
  char *subspec;
  int choice;
  static const char *choices[] = {
    "text",
    "vba",
    NULL
  };
  static dtdialog_init_f *methods[] = {
    dtdialog_text_init,
    dtdialog_vba_init,
    NULL
  };

  if (spec == NULL)						/* caller not giving spec string?	*/
    spec = "";							/* choose compile-time default	    */

  if (strlen(spec) > 
      sizeof(dialogtype)-1)
    return dt_err(ctl, F,
      "\"%s\" too long", spec);

  strcpy(dialogtype, spec);				/* string we will parse				*/

  subspec = strchr(dialogtype,			/* get subspec part					*/
    DT_SEPARATOR_SPEC);

  if (subspec == NULL)					/* separate spec from subspec	    */
    subspec = "";
  else
    *subspec++ = '\0';

  DT_Q(dtstr_choice,(ctl, choices,		/* look up dialog name			    */
    dialogtype, &choice));
  
  if (choice == -1)						/* unknown name					    */
    return dt_err(ctl, F,
      "Sorry, dialog spec \"%s\""
      " is not understood",
      spec);
  else
  if (choice < -1)						/* ambiguous name				    */
    return dt_err(ctl, F,
      "Sorry, dialog spec \"%s\""
      " is ambiguous (%d possibilities)",
      spec, -choice);

  DT_Q(dtdialog_init2,(ctl,
    dialog, existing,
    methods[choice],
    subspec, title, flags, keyboard));

  return DT_RC_GOOD;
}

/*..........................................................................*/
dt_rc_e
DTCONFIG_API1
dtdialog_uninit(						/* uninit structure					*/
  dt_ctl_t *ctl,
  dtdialog_t *dialog)
{
  DT_F("dtdialog_uninit");

  dt_dbg_trace(ctl, F,
    DT_RCSID_STRING, __LINE__, 
    "dialog->uninit = 0x%08lx",
    dialog->uninit);

  if (dialog->uninit != NULL)
  DT_Q(dialog->uninit,(ctl, dialog));	/* do the method				    */

  DT_Q(dtos_free2,(ctl,
    dialog->field,
    F, "fields"));

  DT_Q(dtos_free,(ctl,
    dialog->title));

  DT_Q(dtos_free,(ctl,
    dialog->subinit));

  return DT_RC_GOOD;
}

/*..........................................................................*/
dt_rc_e
DTCONFIG_API1
dtdialog_field(							/* define field						*/
  dt_ctl_t *ctl,
  dtdialog_t *dialog,
  int x,
  int y,
  const char *name,
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
  field->name = name;
  field->lmax = lmax;
  field->x = x;
  field->y = y;

  dialog->nfields++;

  return DT_RC_GOOD;
}

/*..........................................................................*/
dt_rc_e
DTCONFIG_API1
dtdialog_button(						/* define button					*/
  dt_ctl_t *ctl,
  dtdialog_t *dialog,
  char *label,							/* button's label				    */
  int pos,								/* horizontal relative position	    */
  dt_key_t key)							/* key-equivalent to return		    */
{
  DT_F("dtdialog_button");

  if (dialog->button != NULL)
  DT_Q(dialog->button,(ctl, dialog,		/* do the method				    */
    label, pos, key));

  return DT_RC_GOOD;
}

/*..........................................................................*/
dt_rc_e
DTCONFIG_API1
dtdialog_input(							/* take input						*/
  dt_ctl_t *ctl,
  dtdialog_t *dialog,
  dtdialog_field_t **start,
  dt_key_t *key)
{
  DT_F("dtdialog_input");

  if (dialog->input != NULL)
  DT_Q(dialog->input,(ctl, dialog,		/* do the method				    */
    start, key));

  return DT_RC_GOOD;
}

/*..........................................................................*/
dt_rc_e
DTCONFIG_API1
dtdialog_hide(							/* hide screen						*/
  dt_ctl_t *ctl,
  dtdialog_t *dialog,
  int flags)
{
  DT_F("dtdialog_hide");

  if (dialog->hide)						/* has a method?				    */
  DT_Q(dialog->hide,(ctl, dialog,		/* do the method				    */
    flags));

  return DT_RC_GOOD;
}

/*..........................................................................*/
dt_rc_e
DTCONFIG_API1
dtdialog_twirl(							/* twirl screen						*/
  dt_ctl_t *ctl,
  dtdialog_t *dialog,
  int x, 
  int y,
  int flags)
{
  DT_F("dtdialog_twirl");

  if (dialog->twirl)					/* has a method?				    */
  DT_Q(dialog->twirl,(ctl, dialog,		/* do the method				    */
    x, y, flags));

  return DT_RC_GOOD;
}

/*..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtdialog_key(					   
  dt_ctl_t *ctl,
  dtdialog_t *dialog,
  int *iskey,
  dt_key_t *key)
{
  DT_F("dtdialog_key");

  if (dialog->key != NULL)				/* we have a key method? 			*/
  {
    DT_Q(dialog->key,(ctl,
      dialog, iskey, key))
  }
  else									/* no key method? 					*/
  {
	if (iskey != NULL)					/* caller is polling? 				*/
	  *iskey = 0;						/* tell her no key was hit 			*/

  }

  return DT_RC_GOOD;
}

/*..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtdialog_help(					   
  dt_ctl_t *ctl,
  dtdialog_t *dialog,
  const char *filename,
  const char *topic,
  int flags)
{
  DT_F("dtdialog_help");

  if (dialog->help)						/* has a method?				    */
  DT_Q(dialog->help,(ctl, dialog,		/* do the method				    */
    filename, topic, flags));

  return DT_RC_GOOD;
}


/*..........................................................................*/
static 
int 
next(
  int i, 
  int dir, 
  int n)
{
  if (i == 0 && dir == -1)
    return n-1;
  if (i == n-1 && dir == 1)
    return 0;
  return i + dir;
}

/*..........................................................................*/
dt_rc_e
DTCONFIG_API1
dtdialog_transit(
  dt_ctl_t *ctl,
  dtdialog_t *dialog,
  dt_key_t key,
  dtdialog_field_t *oldfield,
  dtdialog_field_t **newfield)
{
  DT_F("dtdialog_transit");
  int n = 0;
  int i, j, k;
  int oldi = -1;
  int oldrow, oldcol;
  long v[DTDIALOG_MAXFIELDS];

# define COMBINE(I) \
  (((long)(dialog->field[(I)].y) << 20) | \
   ((long)(dialog->field[(I)].x) << 10) | (long)(I))
# define ROW(I) ((int)(v[(I)] >> 20))
# define COL(I) ((int)((v[(I)] >> 10) & 0x03ff))
# define ID(I)  ((int)(v[(I)] & 0x03ff))
# define NEXT(I, DIR) (next(I, DIR, n))

  for (i=0; i<dialog->nfields; i++)		/* search fields					*/
  {
    if (dialog->field[i].lmax > 0)		/* is an input field?				*/
	{
      v[n++] = COMBINE(i);
      if (oldfield == NULL)				/* caller is not giving start?	    */
        oldfield = &dialog->field[i];	/* start with current one			*/
	}
    
    if (oldfield == &dialog->field[i])	/* is the given field?				*/
      oldi = i;
  }

  if (oldi == -1)
    return dt_err(ctl, F,
      "input field not found");

  DT_Q(dt1d_sort_long,(v, n));			/* sort by y, then x, then i		*/

  for (i=0; i<n; i++)					/* find index of start field		*/
    if (ID(i) == oldi)
      break;

  oldrow = ROW(i);
  oldcol = COL(i);

  if (key == DT_KEY_DOWNARROW)
  {
    for (j=0; j<n; j++)
    {
      i = NEXT(i, 1);
      if (ROW(i) != oldrow)
        break;
    }
    for (j=0; j<n; j++)
    {
      k = NEXT(i, 1);
      if (ROW(k) != ROW(i) ||			/* hit a different row?				*/
          COL(k) > oldcol)				/* or went past desired field?		*/
        break;  
      i = k;
    }
  }
  else
  if (key == DT_KEY_UPARROW)
  {
    for (j=0; j<n; j++)
    {
      i = NEXT(i, -1);
      if (ROW(i) != oldrow)
        break;
    }
    for (j=0; j<n; j++)
    {
      k = NEXT(i, -1);
      if (ROW(k) != ROW(i) ||			/* hit a different row?				*/
          COL(k) < oldcol)				/* or went past desired field?		*/
        break;  
      i = k;
    }
  }
  else
  if (key == DT_KEY_TAB ||
      key == DT_KEY_ENTER)
    i = NEXT(i, 1);
  else
  if (key == DT_KEY_LEFTARROW)
    i = NEXT(i, -1);
  else
  if (key == DT_KEY_HOME)				/* HOME gives uppermost leftmost	*/
    i = 0;

  *newfield = &dialog->field[ID(i)];

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
