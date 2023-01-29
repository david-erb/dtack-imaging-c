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
| dts_parse - do argument parsing for samples
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
DT_RCSID("source/inter $RCSfile: interhlp.c,v $ $Revision: 1.1 $");


#include <dtack/cons.h>
#include <dtack/dialog.h>
#include <dtack/note.h>
#include <dtack/alpha.h>
#include <dtack/iact.h>

#include <dtack/os.h>
#include <dtack/str.h>
#include <dtack/keymap.h>
#include <dtack/keybd.h>

static dt_key_t keylist[] = {
  DT_KEY_STOP,
  DT_KEY_F1,
  DT_KEY_F2,
  DT_KEY_F3,
  DT_KEY_F4,
  DT_KEY_F5,
  DT_KEY_LEFTARROW,
  DT_KEY_RIGHTARROW,
  DT_KEY_UPARROW,
  DT_KEY_DOWNARROW,
  DT_KEY_PAGEUP,
  DT_KEY_PAGEDOWN,
  DT_KEY_HOME,
  DT_KEY_END,
  DT_KEY_BACKSPACE,
  DT_KEY_DELETE,
  DT_KEY_ENTER,
  DT_KEY_IGNORE};

/*..........................................................................*/
static
dt_rc_e
dt_interactive_help_keymap(
  dt_ctl_t *ctl,
  dtalpha_t *alpha,
  dt_key_t key)
{
  DT_F("dt_interactive_help_keymap");
  dtkeybd_t *keyboard = (dtkeybd_t *)	/* get keyboard pointer 			*/
    alpha->dialog.keyboard;

  if (keyboard)							/* this is a mapped keyboard?		*/
  {
    char name[16];
    char maps[80];
    char line[80];
    DT_Q(dtkeymap_name_str,(ctl,		/* name for the key's function 		*/
      &keyboard->keymap, key, 
      name, sizeof(name)));
  
    DT_Q(dtkeymap_maps_str,(ctl,		/* keystrokes which map function	*/
      &keyboard->keymap, key, 
      maps, sizeof(maps)));

    strcpy(line, "  ");
    strcat(line, name);
    strcat(line, "            ");
	
    DT_Q(dtstr_printf,(ctl, line+14,	/* format the line as it appears 	*/
      sizeof(line)-14, "%s",
      maps));
  
    DT_Q(dtnote_append,(ctl,			/* add help to alpha line by line   */
      &alpha->note, line, 
      strlen(line)));
  
    DT_Q(dtnote_add,(ctl,				/* complete the line in the alpha 	*/
  	  &alpha->note, "", 1));
  }
	   
  return DT_RC_GOOD;
}
/*..........................................................................*/
static
dt_rc_e
dt_interactive_help_keymaps(
  dt_ctl_t *ctl,
  dtalpha_t *alpha)
{
  DT_F("dt_interactive_help_keymaps");
  dtkeybd_t *keyboard = (dtkeybd_t *)	/* get keyboard pointer 			*/
    alpha->dialog.keyboard;

  if (keyboard)							/* this is a mapped keyboard?		*/
  {
    int i;
    DT_Q(dtnote_add,(ctl, &alpha->note,
      "These are the keyboard mappings:",
      -1));

    for (i=0;
  	   keylist[i] != DT_KEY_IGNORE;
  	   i++)
      DT_Q(dt_interactive_help_keymap,(ctl,
        alpha,
        keylist[i]));
  }

  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/

static
dt_rc_e
dt_interactive_help_fn(
  dt_ctl_t *ctl,
  const char *prefix,
  const char *basename,
  const char *suffix,
  char *filename,
  int lmax)
{
  DT_F("dt_interactive_help_fn");

  DT_Q(dtstr_printf,(ctl,				/* make filename				    */
    filename, lmax,
    "%s%s%s",
    prefix, basename, suffix));

  return DT_RC_GOOD;
}

/*..........................................................................
| NAME
| dt_interactive_help - activate help
| 
| DESCRIPTION
| If ~basename~ is not NULL, then this function uses it to form
| a complete filename.  
| The contents of the file are used to populate the alpha.
| If ~topic~ is not NULL in this case,
| then the alpha is pre-positioned at the indicated topic.
|
| If ~basename~ is NULL, then ~topic~ is presumed to contain the help text.
| The entire value of the string then populates the alpha.
| No parsing or editing of the string occurs.
| The alpha is initially position at the top. 
|
| Both ~basename~ and ~topic~ may not be NULL.
| 
| Most callers will use the value of ~param->help~ as ~basename~.
| They may want to fall back to using ~ctl->progname~ if
| ~param->help~ is not specified.
| 
| Specify the base part of the help filename as ~basename~.
| If ~basename~ has no slashes in it,
| then the list of directories in the environment variable :DT_HELP:
| is searched for the filename.
| If ~basename~ has slashes in it,
| then it is assumed to be a complete path and no searching is performed.
| 
| A system-dependent suffix will be added if ~basename~ has no suffix.
| If ~basename~ has slashes in it,
| then the suffix is set off by the last dot after the last slash.
| If ~basename~ has no slashes,
| then the suffix is set off by the last dot in the string.
| 
| This function must not be called until dt_interactive_enter
| has successfully returned.
|
| If the main dialog has a help facility, then it is used.
| Otherwise a help alpha is created to serve as the help facility.
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
dt_interactive_help(
  dt_ctl_t *ctl,
  const char *basename,
  const char *topic)
{
  DT_F("dt_interactive_help");
  char filename[256];
  dtcons_opfunc_f *opfunc = NULL;
  dtalpha_t *alpha;
  dt_rc_e rc;

  DT_Q(dtcons_search,(ctl,				/* see if we have a help display	*/
    DTIACT_CONS_HELP,
    &opfunc, (void **)&alpha));
  if (opfunc == NULL)					/* we don't have a help display?   	*/
  {
    DT_Q(dtiact_newalpha,(				/* make alpha for the output		*/
      ctl, &alpha, 
      DTIACT_CONS_HELP));
    opfunc = dtiact_cons_opfunc;

	DT_Q(dt_interactive_help_keymaps,(	/* print out key maps 				*/
      ctl, alpha));

    if (basename != NULL)				/* caller is indicating a file?	    */
	{
      DT_Q(dt_interactive_help_fn,(		/* put together a filename		    */
        ctl, "alpha/", basename, ".alp", 
        filename, sizeof(filename)));
      DT_G(dtalpha_load,(ctl, alpha,	/* read contents of help file	    */
        filename));
      if (rc != DT_RC_GOOD)				/* could not read help file?	    */
      {
        char *s;
        s = "unable to open help file: \"";
        DT_Q(dtnote_append,(ctl,		/* put a one-liner in help alpha 	*/
          &alpha->note, s, strlen(s))); 

        DT_Q(dtnote_append,(ctl,
          &alpha->note, filename, 
          strlen(filename)));

        DT_Q(dtnote_add,(ctl,
          &alpha->note, "\"", 2));

        s = "after locating the file, try using the \"load\" command on it";
        DT_Q(dtnote_add,(ctl,	
          &alpha->note, s, -1));
	  }
	}
	else								/* caller not indicating a file?    */
	{
      const char *s, *e;
      int l;
      s = topic;						/* topic contains help text		    */
      do {
        e = strchr(s, '\n');			/* locate next newline in text 		*/
        if (e)							/* there is another newline?		*/
          l = (int)(e - s);				/* length of line without newline 	*/
        else							/* no more newlines? 				*/
          l = strlen(s);				/* length of rest of text 			*/
        if (l)							/* this is a non-empty line? 		*/
          DT_Q(dtnote_append,(ctl,		/* add help to alpha line by line   */
            &alpha->note, s, l))
		else
          DT_Q(dtnote_append,(ctl,
            &alpha->note, " ", 1));
        DT_Q(dtnote_add,(ctl,			/* complete the line in the alpha 	*/
          &alpha->note, "", 1));
        s = e + 1;
	  } while(e && *s);
	}
  }
  
  DT_Q(dtnote_add,(ctl, &alpha->note,
    " ", -1));
  DT_Q(dtnote_add,(ctl, &alpha->note,
    "END OF HELP", -1));

  DT_Q(dtcons_goto,(ctl,				/* go to the help display		    */
    opfunc, alpha))

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
