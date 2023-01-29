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
DT_RCSID("source/inter $RCSfile: text.c,v $ $Revision: 1.1 $");
#include <dtack/os.h>
#include <dtack/str.h>
#include <dtack/keymap.h>
#include <dtack/keybd.h>
#include <dtack/text.h>

/*..........................................................................
| NAME
| dttext_beg() - begin text operations
| 
| SUMMARY
| Call an implementation function indicated by the first part of ~spec~. 
| Support one of the specs "curses", "termcap", "int10", or "qnxterm".
| Consider everything in ~spec~ past the first comma to the
| implementation dependent end pass it unchanged to the
| implementation's initialization method.
| Prepare structure members common to all implementations.
| Have an error if ~spec~ is NULL, empty or otherwise invalid.
| 
| END
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dttext_beg(								/* begin text operations			*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dttext_t *text,						/* returned text object description	*/
  const char *spec)						/* specification string 			*/
{
  DT_F("dttext_beg");
  char texttype[64];
  char *subspec;
  int choice;
  static const char *choices[] = {
    "curses",
    "termcap",
    "int10",
    "qnxterm",
    NULL
  };

  static dttext_beg_f *methods[] = {
    dttext_curses_beg,
    dttext_tcap_beg,
    dttext_int10_beg,
    dttext_qnxterm_beg,
    NULL
  };

  if (spec == NULL)						/* no specification given? 			*/
    return dt_err(ctl, F,
      "NULL dialog name");
  else
  if (strlen(spec) == 0)				/* empty specification given? 		*/
    return dt_err(ctl, F,
      "zero-length dialog name");

  if (strlen(spec) >					/* specification too long? 			*/
	  sizeof(texttype)-1)
    return dt_err(ctl, F,
      "\"%s\" too long", spec);

  strcpy(texttype, spec);				/* string we will parse				*/

  subspec = strchr(texttype,			/* get subspec part					*/
    DT_SEPARATOR_SPEC);

  if (subspec == NULL)					/* separate spec from subspec	    */
    subspec = "";
  else
    *subspec++ = '\0';

  DT_Q(dtstr_choice,(ctl, choices,		/* check out text choices         	*/
    texttype, &choice));
  if (choice == -1)						/* unknown choice?					*/
    return dt_err(ctl, F,
      "unknown text type \"%s\"",
      texttype);
  else
  if (choice < -1)						/* ambiguous choice?				*/
    return dt_err(ctl, F,
      "ambiguous text type \"%s\"",
      texttype);

  DT_Q((methods[choice]),(ctl,			/* begin text subinit				*/
    text, subspec));

  return DT_RC_GOOD;
}

/*..........................................................................
| NAME
| dttext_end() - end text operations
| 
| SUMMARY
| Call implementation "end" method.
| Free structure members common to all implementations.
| 
| END
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dttext_end(								/* end text operations				*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dttext_t *text)						/* text object description 			*/
{
  DT_F("dttext_end");

  if (text->end)						/* object has an end method? 		*/
    DT_Q(text->end,(ctl, text));		/* call the object's end method 	*/
  
  return DT_RC_GOOD;
}
/*..........................................................................
| NAME
| dttext_out() - output text at position
| 
| SUMMARY
| Move the text output position to the given coordinates
| and place the text characters there.
| Leave the text output position after this operation undefined.
| Use bits in ~flags~ to control the text appearance,
| for example :DTTEXT_FLAG_NORMAL: and :DTTEXT_FLAG_HIGHLIGHT:.
| 
| END
 *..........................................................................*/


dt_rc_e
DTCONFIG_API1
dttext_out(								/* output text at position			*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dttext_t *text,						/* text object description 			*/
  const char *string,					/* string to be output 				*/
  int x,								/* output column position 			*/
  int y,								/* output row position 				*/
  unsigned long flags)					/* text appearance control flags 	*/
{
  DT_F("dttext_out");

  DT_Q(text->out,(ctl, text, string,
    x, y, flags));
  
  return DT_RC_GOOD;
}
/*..........................................................................
| NAME
| dttext_clear() - clear screen
| 
| SUMMARY
| Clear the text screen.
| Leave characters fields visible.
| 
| END
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dttext_clear(							/* clear screen						*/
  dt_ctl_t *ctl,						/* environment control				*/
  dttext_t *text)						/* text object description 			*/
{
  DT_F("dttext_clear");

  if (text->clear)						/* has a method?				    */
  DT_Q(text->clear,(ctl, text));		/* do the method				    */
  
  return DT_RC_GOOD;
}
/*..........................................................................
| NAME
| dttext_key() - take keyboard input
| 
| SUMMARY
| Take input from the text screen.
| If ~iskey~ is NULL, wait indefinitely for a key to be hit.
| Otherwise, poll the keyboard and return true if a key has been hit.
| Return the value of the key in ~key~.
| Multi-character keys are mapped into single :dt_key_t: values.
| 
| END
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dttext_key(								/* take keyboard input				*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dttext_t *text,						/* text object description 			*/
  int *iskey,							/* returned true if key hit 		*/
  dt_key_t *key)						/* returned key value 				*/
{
  DT_F("dttext_key");

  DT_Q(text->key,(ctl, text,			/* call implementation's method 	*/
    iskey, key));

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
