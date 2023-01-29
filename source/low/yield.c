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
  CHANGES
  19991219 DE send dt_yield as caller to dt_say
 *..........................................................................*/


#include <dtack/base.h>
DT_RCSID("source/low $RCSfile: yield.c,v $ $Revision: 1.1 $");
#include <dtack/str.h>
#include <dtack/cons.h>
#include <dtack/msw.h>

static const char *flag_names[] = {
  "off",
  "low",
  "normal",
  "high",
  "dot",
  "twirl",
  "message",
  "silent",
  NULL
};

#define DT_YIELD_FLAG_SILENT (0x08000000L)

static const long flag_values[] = {
  DT_YIELD_FLAG_OFF,	
  DT_YIELD_FLAG_LOW,	
  DT_YIELD_FLAG_NORMAL,
  DT_YIELD_FLAG_HIGH,	
  DT_YIELD_FLAG_DOT,	
  DT_YIELD_FLAG_TWIRL,	
  DT_YIELD_FLAG_MESSAGE,
  DT_YIELD_FLAG_SILENT
};

const long *dt_yield_flag_values = flag_values;
const char **dt_yield_flag_names = flag_names;

/* ........................................................................ *
 * ........................................................................ */

dt_rc_e
DTCONFIG_API1
dt_yield_status(						/* yield timeslice 					*/
  dt_ctl_t *ctl,						/* environment control 				*/
  const char *status)					/* format of yield message 			*/
{
  DT_F("dt_yield_status");
  dtcons_t *cons = (dtcons_t *)ctl->cons;
  dt_say_string_f *say_string =
    ctl->say_string?
      ctl->say_string:
      dt_say_string;
  char t[64];
  dt_rc_e rc = DT_RC_GOOD;
										/* ................................ */
  if (cons != NULL &&					/* we have interactive consoles? 	*/
	  cons->update)
  {
	dt_key_t key = 0;

    DT_G(cons->update,(ctl, status,		/* update all consoles 				*/
      &key));
    if (key != 0 ||						/* console saw a key it? 			*/
		rc != DT_RC_GOOD)				/* human told console to quit? 		*/
      dt_dbg(ctl, F, DT_DBG_MASK_ASYNC,
        "key 0x%04x rc %d",
        key, rc);
  }
										/* ................................ */
  else									/* no interactive consoles? 		*/
  if (ctl->yield_flags &				/* we are supposed to do message? 	*/
	  DT_YIELD_FLAG_MESSAGE)
  {
	dt_say(ctl, "dt_yield", status);	/* output message to console 		*/
  }
										/* ................................ */
  else									/* not outputting a message?		*/
  if (ctl->yield_flags &				/* we are supposed to be twirling?	*/
	  DT_YIELD_FLAG_TWIRL)
  {
#   define NTWIRL (4)
    static char twirl[NTWIRL] =
      {'|', '/', '-', '\\'};
    int i =								/* get current character index		*/
	  ((unsigned)(ctl->yield_flags &		
        DT_YIELD_FLAG_CHARMASK) + 1) %	/* and increment 					*/
      NTWIRL;
	ctl->yield_flags &=					/* clear old character index 		*/
      ~DT_YIELD_FLAG_CHARMASK;
	ctl->yield_flags |= i;				/* set new character index 			*/
	t[1] = 0x08;						/* backspace						*/
	t[0] = twirl[i];					/* current twirl character 			*/
    say_string(ctl, t, 2);
  }
  else									/* not outputting message or twirl?	*/
  if (ctl->yield_flags &				/* we are supposed to output a dot?	*/
	  DT_YIELD_FLAG_DOT)
  {
	t[0] = '.';
    say_string(ctl, t, 1);
  }
  else
  if (dtconfig_implied_msw)				/* we are running windows? 			*/
  {
    if (ctl->yield_flags &				/* supposed to be silent?		    */
		DT_YIELD_FLAG_SILENT ||
		status == NULL ||				/* no status? 						*/
	    *status == '\0')
	{
      DT_Q(dtmsw_yield,(ctl))			/* just yield 						*/
	}
    else
	{
	  DT_Q(dtmsw_status,(ctl, status));	/* send status to window 			*/
	}
  }

  return rc;
}

/* ........................................................................ *
| NAME
| dt_yield() - yield timeslice
| 
| DESCRIPTION
|_index timeslice,, yielding
| dt_yield() voluntarily yields the running program's timeslice.
|
| If ~*ctl~ indicates that this program is executing under
|_aside Message used as interactive feedback.
| a Dtack Imaging Software interactive dialog,
| then ~format~ is expanded using the variable argument list like sprintf()
| All open consoles are updated with the resulting message.
| If there are no open consoles,
| the message or just a twirling baton or a single dot
| may be printed on the console.
| The operating system's native timeslice yield function, if any,
| is called.
|
| The message created by expanding ~*format~ and the variable argument list
| will normally be some sort of status message.
| Good examples would be something like "percent complete",
| or "number of lines processed".
| This message provides important feedback to the human operator
| in an interactive environment.
|
| The message is output to all open interactive consoles.
| Interactive consoles are set up by the Dtack Imaging Software
| described in
|_hyper document, dtdoclist(dtinter, title)
| 
| If there are no open interactive consoles,
| then depending on bits in the yield control word,
| one of the following is output to the standard console
| through the dt_say() mechanism.
| The dt_say() mechanism bypasses stdout so you will always see it
| on your console even if stdout is redirected to a file.
| 
| :DT_YIELD_FLAG_MESSAGE:	the formatted message is output
| :DT_YIELD_FLAG_TWIRL:		a twirling baton is output using the characters
| 							hyphen, slash, vertical bar, and backslash,
| 							in rotating sequence with backspaces
| 							between them
| :DT_YIELD_FLAG_DOT:		just a dot is output
| none of the above			nothing output
| 
| This function does not examine the yield frequency hints 
| in ~ctl->yield_flags~.
| This function should only be called when yielding is known to be wanted.
| It is the caller's responsibility to check the yield control word
| and determine the desirability of yielding at any particular moment.
|
| This is the default timeslice yielding function.
|_aside This function called only indirectly.
| It is not called directly from within the Dtack Imaging Software libraries.
| Instead, the library code always refers to ~ctl->yield~.
| In order for this function to be called from the libraries,
| you must assign it address to the function pointer like:
| 
|:  ctl->yield = dt_yield;
| 
| You are free to write your own yield function and place its
| address at ~ctl->yield~ instead.
|
| RETURN VALUES
| The value of the function is always one of three different values:
| :DT_RC_GOOD:	function executed successfully
| :DT_RC_STOP:	stop requested by human operator
| :DT_RC_BAD:	function did not execuate successfully
| Before returning :DT_RC_BAD:, dt_err() will have been called at least once.
| 
| ERRORS
| Unsuccessful completion may be due to:
| - error in the console updating routines, if any
| - error in the operating system timeslice yielding routine, if any
| 
| CAVEATS
| This function not called directly from libraries.
| To be called, its address placed in ~ctl->yield~.
| 
| It is best to keep the ~format~ string fairly simple.
|_Aside Keep ~*format~ simple.
| The number of bytes which can be formatted in one call is limited.
| The arguments in the variable list must match their respective
| format specifiers in the ~*format~ string.
| 
| SEE ALSO
| Please refer to the
|_hyper section, The :yield: Function Group
| 
| The following function is also related:
| dt_yield_parse()		create yield control word from string
|
| EXAMPLES
| For a complete and working example program, please see
|_hyper section, Example :dtenv5.c:
| 
| The following code snippet highlights this function:
|:
|:  dtbpe_t bpe;
|:  dtbpe_init(NULL, &bpe, 0);			// allocate bpe resources
|:  dtbpe_uninit(NULL, &bpe);			// free bpe resources
| 
| END
 * ........................................................................ */

dt_rc_e
DTCONFIG_API2
dt_yield(								/* yield timeslice 					*/
  dt_ctl_t *ctl,						/* environment control 				*/
  const char *format,					/* format of yield message 			*/
  ...)
{
  DT_F("dt_yield");
  va_list ap;
  dt_rc_e rc;
  va_start(ap, format);
  DT_C(dt_yield_va,(ctl, format, ap));
cleanup:
  va_end(ap);
  return rc;
}

/* ........................................................................ *
 * ........................................................................ */

dt_rc_e
DTCONFIG_API1
dt_yield_va(							/* yield timeslice 					*/
  dt_ctl_t *ctl,						/* environment control 				*/
  const char *format,					/* format of yield message 			*/
  va_list ap)
{
  DT_F("dt_yield_va");
  if (ctl != NULL &&					/* ctl indicates a yield function? 	*/
	  ctl->yield != NULL &&
	  ctl->yield != dt_yield_va)
  {
    DT_Q((ctl->yield),(ctl,				/* invoke caller's yield function 	*/
      format, ap));
  }
  else									/* no custom yield function? 		*/
  {
	char string[128];
	DT_Q(dtstr_printf_va,(ctl,
      string, sizeof(string),
      format, ap));
    DT_Q(dt_yield_status,(ctl,			/* call standard function 			*/
      string));
  }

  return DT_RC_GOOD;
}

/* ........................................................................ *
| NAME
| dt_yield_parse() - create yield control word from string
|_index yield control word,, from string
| 
| DESCRIPTION
| dt_yield_parse() parses a string and sets bits a
| yield control word accordingly.
|
| ~*string~ is examined and if a valid yield control word
| representation is found, it sets a bit in ~*yield_flags~.
| If ~*string~ is not valid,
|_aside Parsing error returned as message.
| a friendly error message is stored at ~*message~,
| never exceeding a maximum message length of ~lmax~
| including the trailing null character delimiter.
|
| ~*yield_flags~ is set to 0 before adding any new bits.
|
| No message is returned if ~message~ is NULL or ~lmax~ is 0.
| If a message can be returned,
| a zero-length message is returned if ~*string~ is valid.
| If ~lmax~ is not long enough to hold the entire message,
| then a partial message is stored.
| 
| ~*string~ may contain one or more of the following values.
|_aside Yield control word values.
| The C function strstr() is used to extract the values,
| so it is only necessary that the following values be
| anywhere in the string.
| 
| "off"		never yield
| "low"		call yield function at 0%, 50% and 100%
| "normal"	call yield function at 0% and every 20% up to 100%
| "high"	call yield function at 0% and every 5% up to 100%
| "dot"		output dot to console when yielding
| "twirl"	output twirling baton to console when yielding
| "message"	output formatted message to console when yielding
| 
| If ~string~ is NULL or points to a zero-length string,
| then ~*yield_flags~ will be returned with no bits set (0).
|
| This function does not generally change the value of ~ctl->yield_flags~.
| Of course, you can always pass the address of
| ~ctl->yield_flags~ as the ~*yield_flags~ function argument.
|
| This function is used by dt_enter_parse() when it it sees a
| :-yield: parameter on the command line
|
| RETURN VALUES
| If ~*string~ is valid, then mask bits are set in ~*yield_flags~ and
| ~message[0]~ is set to 0.
| Otherwise ~*yield_flags~ is unchanged and an error message is stored
|_aside Parsing error doesn't give bad return code.
| in ~*message~.
|dxinclude(return0.dx)
| 
| ERRORS
|dxinclude(noerror.dx)
| 
| CAVEATS
| Errors parsing ~*string~ do not cause this function to return
| a bad return code.
| You must check the length of the returned ~message~ string,
| which will be greater than 0 if there were parsing errors.
|
| This function does not change the program environment's
| yield control word, ~ctl->yield_flags~.
| 
| SEE ALSO
| Please refer to the
|_hyper section, The :yield: Function Group
| 
| The following functions are also related:
| dt_yield()		default yield function
| dt_enter_parse()	uses this function to parse command line parameter
|
| EXAMPLES
| For a complete and working example program, please see
|_hyper section, Example :dtenv5.c:
| 
| The following code snippet highlights this function:
|:
|:  dtbpe_t bpe;
|:  dtbpe_init(NULL, &bpe, 0);			// allocate bpe resources
|:  dtbpe_uninit(NULL, &bpe);			// free bpe resources
| 
| END
 * ........................................................................ */

dt_rc_e
DTCONFIG_API1
dt_yield_parse(							/* set yield flags from string		*/
  dt_ctl_t *ctl,						/* environment control 				*/
  const char *string,					/* string to parse 					*/
  unsigned long *yield_flags,			/* returned yield flags				*/
  char *message,						/* returned error message 			*/
  int lmax)								/* maximum length of message 		*/
{
  DT_F("dt_yield_parse");
  long flags = 0;
  const char *p, *q;
  char t[32];
  int choice;
  q = string;
  if (q != NULL &&						/* not NULL input string? 			*/
	  *q != '\0')						/* not empty input string? 			*/
  do {
	  unsigned int n;
	  p = strchr(q, DT_SEPARATOR_LIST);	/* find plus sign separator		  	*/
	  if (!p)							/* no more plus signs?			    */
	    p = q + strlen(q);				/* end of string is separator	    */
      n = (unsigned int)(p-q);
	  if (n >= sizeof(t)-1)				/* too long to be a valid flag?	    */
	  {
		if (message != NULL)
        DT_Q(dtstr_printf,(ctl, 
          message, lmax,
          "Sorry, unrecognized"
          " yield hint \"%s\".",
           q));
        return DT_RC_GOOD;
	  }
	  DT_MEMCPY(t, q, n);					/* get a copy of the flag		    */
	  t[n] = '\0';
	  {
	    DT_Q(dtstr_choice,(ctl,			/* look up name in choice list	    */
          dt_yield_flag_names, t, 
		    &choice));
	    if (choice == -1)
		{
		  if (message != NULL)
          DT_Q(dtstr_printf,(ctl, 
            message, lmax,
            "Sorry, unrecognized"
            " yield hint \"%s\".", t));
          return DT_RC_GOOD;
		}
        else
        if (choice < -1)
		{
		  if (message != NULL)
          DT_Q(dtstr_printf,(ctl, 
            message, lmax,
            "Sorry, yield hint \"%s\""
            " could match any of"
            " %d choices.",
            t, -choice));
          return DT_RC_GOOD;
		}
        else
          flags |= dt_yield_flag_values[choice];
	  }
  	  q = p + 1;
  } while (*p);

  *yield_flags = flags;					/* return the flags					*/
  if (message != NULL &&
	  lmax > 0)
    *message = '\0';					/* return no error message 			*/
  
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
