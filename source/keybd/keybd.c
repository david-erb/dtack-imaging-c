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
| TITLE dtkeybd()
|dxinclude(liblink1.dx)
|
|!INTRODUCTION
| This Function Group gives you a way to get input the keyboard
| without worrying about the operating system details
| of keyboard management.
|
| This Function Group provides individual key values immediately
| without waiting for the ENTER key.
| Therefore, the keyboard device must return one raw byte for each key hit.
| Standard input is not acceptable because it provides no input
| until an entire line is entered.
| 
| The model for the display/keyboard combinations is
| that the display system is independent of the keyboard system.
| In some cases, however,
| the keyboard type must agree with the display type
| because they use a common mode or server.
| For example, in qnxterm term_load() mode,
| both the keyboard and the display must agree.
| 
| Currently, QNX applications may select between two possibilities:
| termios	used when the keyboard is used standalone and display output 
|			continues to be line-oriented.  Also used with curses.
| qnxterm	used when keyboard is used in conjunction with the qnxterm
|			display functions.
|
| Currently, Microsoft windows keyboard remapping is done
| entirely within the WNDPROC functions.
| It would probably be good to handle it in a manner similar
| to the qnxterm routines just described.
| 
|!SUMMARY
| Gets input from the keyboard.
| Hides operating system details of any specific keyboard.
| Has one function which waits until a key is hit.
| Has another function which polls and returns immediately.
|
| Supports a predefined set of keyboards which are:
| :"trmios":	standard Unix functions based on :struct trmios:
| :"qnxterm":	the QNX term_load() environment
| :"dos":		the DOS getch() and kbhit() functions.
| 
| Uses the :keymap: Function Group to map sequences of raw bytes into
| discrete integer key values.
|
|.Salient Concepts
| You select the keyboard type at runtime.
| 
| You can poll or wait for the keyboard.
| 
| You don't get back raw bytes.
| Instead you get back one of a predefined set of integers
| which represents the actual key pressed.
| 
|.Specification Syntax
| The ~spec~ argument to dtkeybd_init() shall have up to three comma-separated
| values in the form :spec1,spec2,spec3:.
| :spec1:	selects the implementation from the list above
| :spec2:	passed to the implementation init function
| :spec3:	selects the initial keyboard mapping
| 
| Here are some examples of valid ~spec~ strings:
| :trmios:				Unix with termcap and :TERM: environment variable
| :trmios,,ctlkeys:		Unix using only control+key mappings, ignore :TERM:
| :trmios,,myfile:		Unix using mappings specified in file :myfile:
| :qnxterm:				use QNX "term" interface, standard QNX mappings
| :qnxterm,,ctlkeys:	use QNX "term", but only map control+keys
| :dos:					use DOS getch() with control+key mappings
| :dos,,ctlkeys:		use DOS getch() with control+key mappings
| :dos,,myfile:			use DOS getch() with mappings from file :myfile:
|
| The format of :myfile: can be found in the :keymap: Function Group.
|
|.Runtime Environment
| Uses no environment variables, files, or signals directly.
| However, calls members of the :keymap: Function Group
| which may use the :TERM: environment variable or an external file.
|
|!Example dtkeybd1.c - Show basic keyboard operations
|:// Example Program dtkeybd1
|:// Illustrates basic keyboard operations
|:// Program takes one optional command line argument: the keyboard spec.
|:// First wait for a key, then poll every one second for a key.
|:// Prints out simple result like:
|://   dtkeybd1: hit any key (waiting)
|://   dtkeybd1: you hit F1
|://   dtkeybd1: hit any key (polling) .....
|://   dtkeybd1: you hit F2
|:// All error checking left out for clarity.
|:
|:#include <dtack/base.h>
|:#include <dtack/keymap.h>
|:#include <dtack/keybd.h>
|:#include <dtack/os.h>
|:
|:int main(int argc, char *argv[])
|:{
|:  dt_ctl_t _ctl, *ctl = &_ctl;
|:  dtkeybd_t keybd;					// the keyboard object descriptor
|:  dt_key_t key;
|:  char string[16];
|:
|:  DT_MEMSET(ctl, 0, sizeof(*ctl));
|://ctl->dbg_mask = DT_DBG_MASK_ASYNC;
|:  dtkeybd_init(ctl, &keybd,			// initialize keyboard
|:    argc>1? argv[1]: NULL, 0);		// give argument if there is one
|:	
|:  printf("dtkeybd1:"
|:    " hit any key (waiting)\n");
|:  dtkeybd_read(ctl, &keybd, &key);	// read key from keyboard
|:  dtkeymap_name_str(ctl, 				// make key value printable
|:    &keybd.keymap,			
|:    key, string, sizeof(string));		
|:  printf("dtkeybd1:" 					// print what we got
|:    " you hit %s\n",
|:    string);	
|:	
|:  printf("dtkeybd1:"
|:    " hit any key (polling) ");
|:  while(1)							// polling loop
|:  {
|:    int iskey;
|:    dtkeybd_scan(ctl, &keybd,			// poll keyboard
|:      &iskey, &key);
|:    if (iskey)						// a key was hit?
|:      break;
|:    printf(".");						// no key was hit?
|:    fflush(stdout);
|:    dtos_delay(ctl, 1000);
|:  }
|:  printf("\n");
|:
|:  dtkeymap_name_str(ctl, 				// make key value printable
|:    &keybd.keymap,			
|:    key, string, sizeof(string));		
|:  printf("dtkeybd1:" 					// print what we got
|:    " you hit %s\n",
|:    string);	
|:	
|:  dtkeybd_uninit(ctl, &keybd);		// release keyboard resources	
|:}
| 
| END
 *..........................................................................*/

#include <dtack/base.h>
DT_RCSID("source/keybd $RCSfile: keybd.c,v $ $Revision: 1.7 $");
#include <dtack/os.h>
#include <dtack/str.h>
#include <dtack/keymap.h>
#include <dtack/keybd.h>
#include <dtack/prj.h>

static const char *choices[] = {
  "trmios",
  "qnxterm",
  "dos",
  NULL
};

static dtkeybd_init_f *methods[] = {
  dtkeybd_unix_init,
  dtkeybd_qnxterm_init,
  dtkeybd_dos_init,
};

/*..........................................................................
| NAME
| dtkeybd_init() - initialize keyboard	
| 
| SUMMARY
| Initialize the ~keybd~ structure.
| 
| Expect ~spec~ to be of the form :spec1,spec2,spec3:.
| If ~spec~ is NULL or empty, 
| use the compiled-in value returned :dtprj_param_default("-keyboard"):.
| Use ~spec1~ to select one of the predefined implementations.
| Call implementation's init function giving it ~spec2~ as an argument.
| 
| Call dtkeymap_init() to create the initial keyboard mapping.
| Create an identity mapping if :DTKEYBD_FLAG_INIT_NOMAP: is set.
| Otherwise, give it ~spec3~ as an argument.
| If ~spec3~ is NULL or empty, choose a default mapping set based on ~spec1~.
| 
| Allocate and prepare structure members common to all implementations.
| Have an error if ~spec~ is invalid.
| Have an error if support for the selected implementation has not been linked.
|
| END
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtkeybd_init(							/* initialize keyboard 				*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtkeybd_t *keybd,						/* keyboard object descriptor 		*/
  const char *spec,						/* keyboard specification 			*/
  long flags)							/* control flags 					*/
{
  DT_F("dtkeybd_init");
  char *spec1 = NULL;
  char *spec2;
  char *spec3;
  int choice;
  dt_rc_e rc;

  if (spec == NULL ||					/* no keyboard type given? 			*/
	  strlen(spec) == 0)				/* empty keyboard type given? 		*/
	DT_C(dtprj_param_default,(ctl,		/* use the project default 			*/
      "-keyboard", &spec));

  DT_C(dtos_strdup,(ctl, spec,			/* local copy for changing 			*/
    &spec1));

  spec2 = strchr(spec1, ',');			/* spec2 part after spec1		    */
  if (spec2)
  {
    *spec2++ = '\0';
	spec3 = strchr(spec2, ',');			/* spec3 part after spec2		    */
    if (spec3)
      *spec3++ = '\0';
    else
      spec3 = "";
  }
  else									/* no commas at all? 				*/
  {
    spec2 = "";
    spec3 = "";
  }

  memset(keybd, 0, sizeof(*keybd));		/* clear structure				    */

  keybd->spec = spec1;					/* this is what gets freed 			*/
  keybd->spec1 = spec1;
  keybd->spec2 = spec2;
  keybd->spec3 = spec3;

  if (strlen(keybd->spec1) == 0)		/* empty keyboard type given? 		*/
	DT_C(dtprj_param_default,(ctl,		/* use the project default 			*/
      "-keyboard", &keybd->spec1));

  if (flags & DTKEYBD_FLAG_INIT_NOMAP)	/* ok for us to make initial map?	*/
    keybd->spec3 = "";
  else
  if (strlen(keybd->spec3) == 0)		/* no spec3? 						*/
  {
    if (!strcmp(keybd->spec1, "trmios"))
	  keybd->spec3 = "termcap";
    else
    if (!strcmp(keybd->spec1, "qnxterm"))
	  keybd->spec3 = "qnxterm";
    else
	  keybd->spec3 = "ctlkeys";
  }

  dt_dbg(ctl, F, DT_DBG_MASK_ASYNC,		/* debug how spec maps to spec[123]	*/
    "keyboard keybd->spec \"%s\""
    " (%s,%s,%s)",
    keybd->spec, keybd->spec1, 
    keybd->spec2, keybd->spec3); 

  DT_C(dtstr_choice,(ctl, choices,		/* look up keybd type			    */
    keybd->spec1, &choice));
  if (rc == DT_RC_GOOD)
  if (choice == -1)						/* unknown keyboard type given?		*/
    rc = dt_err(ctl, F,
      "Sorry, \"%s\""
	  " is an unknown keyboard spec",
      keybd->spec1);
  else
  if (choice < -1)						/* ambiguous keyboard type given?	*/
    rc = dt_err(ctl, F,
      "Sorry, \"%s\""
	  " is an ambiguous keyboard spec"
      " (%d matches)",
      keybd->spec1, -choice);

  if (rc != DT_RC_GOOD)
    goto cleanup;

  DT_C((methods[choice]),(ctl,			/* initialize the implementation	*/
    keybd, keybd->spec2, flags));
  
  keybd->maxbuffer = 
    sizeof(keybd->buffer);

  DT_C(dtkeymap_init,(ctl,				/* initialize keymaps 				*/
    &keybd->keymap, keybd->spec3, 0));

  return DT_RC_GOOD;

cleanup:
  DT_I(dtkeybd_uninit,(ctl, keybd));
  return rc;
}

/*..........................................................................
| NAME
| dtkeybd_uninit() - close keyboard	
| 
| SUMMARY
| Close the keyboard.
| Release all resources used by the keyboard object descriptor.
| 
| END
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtkeybd_uninit(							/* close keyboard 					*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtkeybd_t *keybd)						/* keyboard object descriptor 		*/
{
  DT_F("dtkeybd_uninit");
  dt_rc_e rc = DT_RC_GOOD;
  DT_I(dtkeymap_uninit,(ctl,			/* knock off the keymap 			*/
    &keybd->keymap));
  if (keybd->uninit)
    DT_I((keybd->uninit),(ctl,			/* let the method die gracefully 	*/
      keybd));
  if (keybd->spec)
    DT_I(dtos_free,(ctl,				/* free parse string			    */
      keybd->spec));
  memset(keybd, 0, sizeof(*keybd));		/* clear the structure 				*/
  return rc;
}


/*..........................................................................
| NAME
| dtkeybd_read() - wait for raw bytes and map to key
| 
| SUMMARY
| Wait for raw bytes to come from the physical keyboard.
| Do not return until there are enough raw bytes to match one key map.
| Return the key mapped, not the raw raw bytes.
| Save any unused raw bytes such that they are available to the next call
| to this function or to dtkeybd_scan().
| 
| END
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtkeybd_read(							/* read keyboard				    */
  dt_ctl_t *ctl,						/* environment control			    */
  dtkeybd_t *keybd,						/* keyboard object descriptor 		*/
  dt_key_t *key)						/* returned key value			    */
{
  DT_F("dtkeybd_read");
  dt_rc_e rc = DT_RC_GOOD;

  do {
	int n = keybd->n;

    if (keybd->scan)					/* we have a scan method? 			*/
      DT_Q((keybd->scan),(ctl, keybd));	/* use method to get characters 	*/

	if (keybd->n > n ||					/* scanned some new keys? 			*/
		keybd->n > 0 &&					/* or we already have some keys? 	*/
		!keybd->ispartial ||			/* and they're not a partial match?	*/
		keybd->read == NULL)			/* or we have no read method? 		*/
	{									/* don't wait 						*/
	}
	else
	  DT_Q((keybd->read),(ctl,			/* use method to wait on characters	*/
		keybd));
	
    if (keybd->n != 0)					/* anything in the buffer?	 		*/
      DT_Q(dtkeymap_match,(ctl,			/* match buffer to key maps		    */
        &keybd->keymap, 
        keybd->buffer, keybd->n, 
        &keybd->n, key, 
        &keybd->ispartial));

  } while(keybd->ispartial);			/* loop until complete match 		*/

  return rc;
}

/*..........................................................................
| NAME
| dtkeybd_scan() - poll for raw bytes and map to key
| 
| SUMMARY
| Poll for raw bytes on keyboard and place them in the buffer.
| Never wait for keys to be hit.
| If there are enough raw bytes in the buffer to match one key map,
| return the key mapped, not the raw raw bytes.
| If there is a key match, set ~iskey~ to 1 else to 0.
| Save any unused raw bytes such that they are available to the next call
| to this function or to dtkeybd_read().
| 
| END
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtkeybd_scan(							/* poll keyboard				    */
  dt_ctl_t *ctl,						/* environment control			    */
  dtkeybd_t *keybd,						/* keyboard descriptor object		*/
  int *iskey,							/* returned true if key available 	*/
  dt_key_t *key)						/* returned key value if available  */
{
  DT_F("dtkeybd_scan");
  dt_rc_e rc = DT_RC_GOOD;
  if (keybd->scan)
  {
    DT_Q((keybd->scan),(ctl, keybd));	/* use method to get characters 	*/

	if (keybd->n)						/* anything in the buffer?	 		*/
	{
      DT_Q(dtkeymap_match,(ctl,			/* match buffer to key maps		    */
        &keybd->keymap, 
        keybd->buffer, keybd->n, 
        &keybd->n, key, 
        &keybd->ispartial));
	  *iskey = keybd->ispartial? 0: 1;	/* got a complete key? 				*/
	}
	else
	  *iskey = 0;
  }
  return rc;
}

/*..........................................................................
| NAME
| dtkeybd_flush() - flush keyboard	
| 
| SUMMARY
| Repeatedly scan and discard raw bytes from the keyboard
| until there are no more available.
| Clear the internal buffer.
| 
| END
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtkeybd_flush(							/* flush keyboard input			    */
  dt_ctl_t *ctl,						/* environment control			    */
  dtkeybd_t *keybd)						/* keyboard descriptor object	    */
{
  DT_F("dtkeybd_flush");
  dt_rc_e rc = DT_RC_GOOD;
  if (keybd->scan)						/* we have a scan method?			*/
    DT_Q((keybd->scan),(ctl, keybd));	/* use method to get characters 	*/
  keybd->n = 0;							/* forget everything in the buffer 	*/
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
