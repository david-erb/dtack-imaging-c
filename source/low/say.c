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
  19990501 DE modified support MSW console mode
  19990124 DE support MSW console mode
 *..........................................................................*/



#if (defined(DTCONFIG_DB0) || \
     defined(DTCONFIG_DT0) || \
     defined(DTCONFIG_DV0) || \
     defined(DTCONFIG_DW0) || \
     defined(DTCONFIG_DW1) || \
     defined(DTCONFIG_WV5) && \
    (DTPRJ_CONSOLE == DT_1))
#  ifndef DTDEPEND
#    include <conio.h>
#  endif
#endif

#include <dtack/base.h>

DT_RCSID("source/low $RCSfile: say.c,v $ $Revision: 1.1 $");
#include <dtack/str.h>

#ifdef DTCONFIG_IMPLIED_QNX4
#include <dtack/ipc.h>
#include <dtack/qnx4.h>		
#endif

#include <dtack/msw.h>

#ifdef DTCONFIG_MICROSOFT
#define WRITE _write
#define CLOSE _close
#define FILENO _fileno
#else
#define WRITE write
#define CLOSE close
#define FILENO fileno
#endif


#define DTCONFIG_TTY "/dev/tty"

/*..........................................................................
 *..........................................................................*/

static
void
fileout(		
  int fd,
  const char *string,
  int length)
{
  if (string == NULL)					/* this is a close message?		    */
  {
	if (fd != FILENO(stdout) &&
		fd != FILENO(stderr))
      CLOSE(fd);
  }
  else									/* not a close message? 			*/
  {
    if (length == -1)					/* caller has not given length?		*/
      length = strlen(string);
	if (fd == FILENO(stdout))
	{
	  fwrite(string, 1, length, 
         stdout);
	  fflush(stdout);
	}
	else
	if (fd == FILENO(stderr))
	{
	  fwrite(string, 1, length, 
         stderr);
	  fflush(stderr);
	}
	else
	  WRITE(fd, string, length);
  }
}

/*..........................................................................
 *..........................................................................*/

void
DTCONFIG_API1
dt_say_string(							/* no prefix or substitutes			*/
  dt_ctl_t *ctl,
  const char *string,
  int length)
{
  void *sayto;

  if (ctl != NULL &&					/* we have ctl? 					*/
	  ctl->say_string != NULL &&		/* ovsayide function given?		    */
	  ctl->say_string != dt_say_string)	/* avoid recursion 					*/
  {
    (ctl->say_string)(ctl,				/* call ovsayide function		    */
      string, length);
	return;
  }

  if (ctl != NULL &&					/* we have ctl? 					*/
	  ctl->sayto != NULL)				/* output handle given?			    */
    sayto = ctl->sayto;					/* use it						    */
  else									/* no ctl or handle is not given?	*/
    sayto = NULL;						/* use default						*/


  if (ctl != NULL &&					/* we have ctl? 					*/
	  string == NULL)					/* this is a close message?		    */
  {
	if (ctl->sayto == ctl->dbgto)		/* we are sharing the file? 		*/
      ctl->dbgto = NULL;
	if (ctl->sayto == ctl->errto)		/* we are sharing the file? 		*/
      ctl->errto = NULL;
    ctl->sayto = NULL;
  }

  if (sayto != NULL)					/* output to file?					*/
    fileout((int)(long)sayto, string, 
      length);
  else									/* output not to file?			    */
  {
#   if defined(DTCONFIG_IMPLIED_DOS)	/* in DOS? 							*/
	{
	  char t[2];
	  int i;

	  if (string == NULL)
        return;
	  if (length == -1)					/* caller has not given length?		*/
        length = strlen(string);

	  t[1] = '\0';
      for (i=0; i<length; i++)
      {
		if (string[i] == '\n')
		{
		  t[0] = '\r';
		  cputs(t);
		}
		t[0] = string[i];
		cputs(t);
	  }
	}
#   else
#   ifdef DTCONFIG_IMPLIED_MSW
	{
	  if (ctl && ctl->msw &&			/* separate windows console window? */
		  ((dtmsw_t *)ctl->msw)->outwin)
	  {
        dtmsw_dbg_string(ctl,			/* special function handles it	    */
          string, length);
	  }
	  else								/* no windows console?				*/
	  {
        fileout(FILENO(stdout),			/* use standard output 				*/
          string, length);
	  }
#if 0
	  else								/* no windows console?				*/
	  {
		char t[2];
		int i;

		if (string == NULL)
		  return;
		if (length == -1)				/* caller has not given length?		*/
		  length = strlen(string);

		t[1] = '\0';
		for (i=0; i<length; i++)
        {
		  if (string[i] == '\n')
		  {
			t[0] = '\r';
			cputs(t);
		  }
		  t[0] = string[i];
		  cputs(t);
		}
	  }
#endif
	}
#   else
#   ifdef DTCONFIG_IMPLIED_QNX4
      dtqnx4_say_string(ctl,			/* special function handles it	    */
        string, length);
#   else
#   ifdef DTCONFIG_IMPLIED_UNIXLIKE
#     ifdef DTCONFIG_TTY
	  {
  	    sayto = (void *)(long)open(		/* open console					    */
          DTCONFIG_TTY, O_WRONLY);
        if ((int)(long)sayto != -1)		/* console opened?				    */
	    {
          fileout((int)(long)sayto,		/* output this message			    */
            string, length);
		  if (ctl != NULL)
            ctl->sayto = sayto;			/* don't open next time				*/
		  else
		    CLOSE((int)(long)sayto);
		}
	  }
#     else
        fileout(FILENO(stderr), 
          string, length);
#     endif
#   else
      no known implied os
#   endif
#   endif
#   endif
#   endif
  }
}

/* ........................................................................ *
| NAME
| dt_say() and dt_say_va() - output console message
| 
| DESCRIPTION
| dt_say() and dt_say_va() output an console message.
|_index console message handling
|
| An console message is formed by formatting the ~format~ string
|_aside Formats like sprintf().
| and the variable arguments like sprintf() would.
| The ~caller~ string is included in the message.
| The console message is output by a default or custom routine.
| 
| dt_say() does nothing but make the :va_list: structure and give it
|_Aside Functions essentially equivalent.
| to dt_say_va() which makes all the decisions and does all the work.
| Except for their arguments, these two functions are functionally equivalent.
|
| This section describes the default method for
| console message formatting and output as done by dt_say_va().
| The next subsection describes how you can provide your own functions
| to customize the default functionality.
|
| The default function always outputs to the console or window
|_aside Does not output to :stdout:.
| where the program was run from.
| It does not output to :stdout: or :stderr:.
|
| Both ~*caller~ and ~ctl->progname~ are integrated into the message,
|_Aside Makes a prettier message.
| if they are not NULL or zero-length.
|
|dxinclude(strprt1.dx)
|
|.CUSTOMIZING 
| The :say: Function Group offers two levels of customization flexibility.
| Both depend on the ~*ctl~ structure.
| In the first level,
| you can provide your own function to completely replace all console
| handling.
| In the second level, you can let dt_say() do the message formatting,
| and write your own routine to simply take over the actual output 
| of strings.
| 
| The first thing dt_say() does is to check the value of ~ctl->say~.
|_aside Complete customization.
| If not NULL, this value is assumed a pointer to a custom function.
| The custom function's prototype is exactly the same as that of dt_say_va().
| The custom function pointed to is called and no further action is taken.
| It is responsible for all formatting and output of the message.
| This mechanism allows you to supplant the standard console message handling
| with a routine of your own.
|
| In a similar manner, ~ctl->say_string~, if not NULL,
| also is assumed to point to a custom function.
| This custom function is called by dt_dbg_va() during formatting.
|_Aside Customized output only.
| This function may be called multiple times for each line of console output.
| The end of a line is marked by a newline character in the string.
| If you wish to have dt_say_va() perform its default formatting
| but want to take over the actual string output yourself,
| then set ~ctl->say_string~ to point to your function.
|
| The prototype for ~ctl->say_string~ is:
| 
|:  typedef
|:  void 
|:  dt_say_string_f(					// console string output 
|:    dt_ctl_t *ctl,					// environment control
|:    const char *string,				// string to output
|:    int length);						// length of string
|
| If the value of ~length~ is -1, then :strlen(string): is implied.
|
| RETURN VALUES
| None.
| 
| ERRORS
| None.
| 
| CAVEATS
| If some condition exists which prevents the console message
| from being output,
| no attempt is made to output it to :stdout:.
| The message is simply lost.
| 
| If ~ctl->say~ is not NULL, it must point to a function
| declared as type :dt_say_f:.
|
| If ~ctl->say_string~ is not NULL, it must point to a function
| declared as type :dt_say_string_f:.
| 
| It is best to keep the ~*format~ string fairly simple.
| The number of bytes which can be formatted in one call is limited.
| The arguments in the variable list must match their respective
| format specifiers in the ~*format~ string.
| 
| SEE ALSO
| Please refer to the
|_hyper Overview, "Overview"
| and
|_hyper sections, The :say: Function Group
|
| EXAMPLES
|:
|:  dt_say(ctl, "myprog",				// output console message
|_aside Output console message.
|:    "the truffles are out of syrup");
| 
| END
 * ........................................................................ */

void
DTCONFIG_API2
dt_say(									/* output console message			*/
  dt_ctl_t *ctl,						/* environment control 				*/
  const char *caller,					/* caller's name 					*/
  const char *format,					/* message format, like sprintf() 	*/
  ...)									/* format substitutions 			*/
{
  va_list ap;
  va_start(ap, format);
  dt_say_va(ctl, caller,				/* let dt_say_va() do all the work 	*/
    format, ap);
  va_end(ap);
}

/*..........................................................................
 *..........................................................................*/

void
DTCONFIG_API1
dt_say_va(								/* output console message, va_list	*/
  dt_ctl_t *ctl,						/* environment control 				*/
  const char *caller,					/* caller's name 					*/
  const char *format,					/* message format, like vsprintf() 	*/
  va_list ap)							/* format substitutions 			*/
{
  if (ctl != NULL &&
      ctl->say != NULL &&
      ctl->say != dt_say_va)
	(*ctl->say)(ctl, caller,			/* call the override function		*/
       format, ap);
  else									/* we will do the default action	*/
  if (format == NULL)					/* this is a close-down message?	*/
    dt_say_string(ctl, NULL, 0);
  else
  {
	{
  	  const char *progname;
      char tmp[512];
	  if (ctl &&
          ctl->progname != NULL)		/* progname is set?				    */
        progname = ctl->progname;		/* use it						    */
	  else
        progname = "";

	  if (caller != NULL &&				/* caller is given?				    */
          strlen(caller) != 0 &&
          strcmp(progname, caller))		/* caller is not same as progname?	*/
        dtstr_printf(ctl, tmp,
          sizeof(tmp), "%s%s%s: ",
          progname, 
          progname[0]? ": ": "", 
          caller);
	  else								/* no known caller?				    */
        dtstr_printf(ctl, tmp,
          sizeof(tmp), "%s: ",
          progname);

      dt_say_string(ctl, tmp, -1);		/* send the message prefix		    */

      dtstr_printf_va(ctl, tmp,			/* make the message proper		    */
        sizeof(tmp), format, ap);
	  
      dt_say_string(ctl, tmp, -1);		/* send the message proper		    */

      tmp[0] = '\n';
      dt_say_string(ctl, tmp, 1);		/* finish with a newline		    */
    }
  }
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
