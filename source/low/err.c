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


#include <dtack/base.h>
DT_RCSID("source/low $RCSfile: err.c,v $ $Revision: 1.1 $");
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

const char *dt_err_prefix = "error";


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
	{
	  dt_dbg_cryptout(fd, string, length);
	}
  }
}

/*..........................................................................
 *..........................................................................*/

void
DTCONFIG_API1
dt_err_string(							/* no prefix or substitutes			*/
  dt_ctl_t *ctl,
  const char *string,
  int length)
{
  DT_F("dt_err_string");
  void *errto;

  if (ctl != NULL &&					/* we have ctl? 					*/
	  ctl->err_string != NULL &&		/* override function given?		    */
	  ctl->err_string != dt_err_string)	/* avoid recursion 					*/
  {
    (ctl->err_string)(ctl,				/* call override function		    */
      string, length);
	return;
  }

  if (ctl != NULL &&					/* we have ctl? 					*/
	  ctl->errto != NULL)				/* output handle given?			    */
    errto = ctl->errto;					/* use it						    */
  else									/* no ctl or handle is not given?	*/
    errto = NULL;						/* use default						*/

  if (string == NULL)					/* this is a close message?		    */
  {
	if (ctl->errto == ctl->dbgto)		/* we are sharing the file? 		*/
      ctl->dbgto = NULL;
	if (ctl->errto == ctl->sayto)		/* we are sharing the file? 		*/
      ctl->sayto = NULL;
    ctl->errto = NULL;
  }

  if (errto != NULL)					/* output to file?					*/
    fileout((int)(long)errto, string, 
      length);
  else									/* output not to file?			    */
  {
#   if defined(DTCONFIG_IMPLIED_DOS)	/* in DOS? 							*/
      fileout(FILENO(stdout), 
        string, length);
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
	}
#   else
#   ifdef DTCONFIG_IMPLIED_QNX4
      dtqnx4_err_string(ctl,			/* special function handles it	    */
        string, length);
#   else
#   ifdef DTCONFIG_IMPLIED_UNIXLIKE
      fileout(FILENO(stderr), 
        string, length);
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
| dt_err() and dt_err_va() - output error message
| 
| DESCRIPTION
| dt_err() and dt_err_va() output an error message.
|_index error message handling
|
| An error message is formed by formatting the ~format~ string
|_aside Formats like sprintf().
| and the variable arguments like sprintf() would.
| The ~caller~ string is included in the message.
| The error message is output by a default or custom routine.
| The function always returns :DT_ERR_BAD:.
| 
| dt_err() does nothing but make the :va_list: structure and give it
|_Aside Functions essentially equivalent.
| to dt_err_va() which makes all the decisions and does all the work.
| Except for their arguments, these two functions are functionally equivalent.
|
| This function is called from many places within the Dtack Imaging Software
| libraries.
| It is only called at the point of original error.
|_aside Original error.
| Original errors come from operating system calls that fail,
| such as malloc() or open().
| Receiving a :DT_RC_BAD: return code from a call to
| a Dtack Imaging Software library function
| is not considered an original error.
| It a be presumed by all functions
| It is universally assumed that if a Dtack Imaging Software
| function returns :DT_RC_BAD:,
| then dt_err() will already have been called at least once.
|
| This section describes the default method for
| error message formatting and output as done by dt_err_va().
| The next subsection describes how you can provide your own functions
| to customize the default functionality.
|
| Both ~*caller~ and ~ctl->progname~ are integrated into the message,
|_Aside Makes a prettier message.
| if they are not NULL or zero-length.
|
|dxinclude(strprt1.dx)
|
|.CUSTOMIZING 
| The :err: Function Group offers two levels of customization flexibility.
| Both depend on the ~*ctl~ structure.
| In the first level,
| you can provide your own function to completely replace all error
| handling.
| In the second level, you can let dt_err() do the message formatting,
| and write your own routine to simply take over the actual output 
| of strings.
| 
| The first thing dt_err() does is to check the value of ~ctl->err~.
|_aside Complete customization.
| If not NULL, this value is assumed a pointer to a custom function.
| The custom function's prototype is exactly the same as that of dt_err_va().
| The custom function pointed to is called and no further action is taken.
| It is responsible for all formatting and output of the message.
| This mechanism allows you to supplant the standard error message handling
| with a routine of your own.
|
| In a similar manner, ~ctl->err_string~, if not NULL,
| also is assumed to point to a custom function.
| This custom function is called by dt_err_va() during formatting.
|_Aside Customized output only.
| This function may be called multiple times for each line of error output.
| The end of a line is marked by a newline character in the string.
| If you wish to have dt_err_va() perform its default formatting
| but want to take over the actual string output yourself,
| then set ~ctl->err_string~ to point to your function.
|
| The prototype for ~ctl->err_string~ is:
| 
|:  typedef
|:  void 
|:  dt_err_string_f(					// error string output 
|:    dt_ctl_t *ctl,					// environment control
|:    const char *string,				// string to output
|:    int length);						// length of string
|
| If the value of ~length~ is -1, then :strlen(string): should be implied.
|
| RETURN VALUES
| If a custom routine is called,
| returns whenever the custom routine returns.
| Otherwise, default is to always return :DT_RC_BAD:.
| 
| ERRORS
| Produces no errors of its own,
| just handles errors given to it by the caller.
| 
| CAVEATS
| If ~ctl->err~ is not NULL, it must point to a function
| declared as type :dt_err_f:.
|
| If ~ctl->err_string~ is not NULL, it must point to a function
| declared as type :dt_err_string_f:.
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
|_hyper sections, The :err: Function Group
|
| EXAMPLES
| For a complete and working example program, please see
|_hyper section, Example :dtenv4.c:
| 
| The following code snippet highlights this function:
| 
|:
|:  dt_err(ctl, "myprog",				// generate an original error
|_aside Generate error.
|:    "the truffles are out of syrup");
| 
| END
 * ........................................................................ */

dt_rc_e
DTCONFIG_API2
dt_err(									/* handle errors					*/
  dt_ctl_t *ctl,						/* environment control 				*/
  const char *caller,					/* caller's name 					*/
  const char *format,					/* message format, like sprintf() 	*/
  ...)									/* format substitutions 			*/
{
  dt_rc_e rc;
  va_list ap;
  va_start(ap, format);
  rc = dt_err_va(ctl, caller,
    format, ap);
  va_end(ap);
  return rc;
}

/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dt_err_va(								/* handle errors					*/
  dt_ctl_t *ctl,						/* environment control 				*/
  const char *caller,					/* caller's name 					*/
  const char *format,					/* message format, like vsprintf() 	*/
  va_list ap)							/* format substitutions 			*/
{
  if (ctl != NULL &&					/* there is a ctl structure?		*/
      ctl->err != NULL &&				/* there is a err function?			*/
      ctl->err != dt_err_va)			/* the err function is not us?		*/
	(*ctl->err)(ctl, caller,			/* call the override function		*/
       format, ap);
  else									/* we will do the default action	*/
  if (format == NULL)					/* this is a close-down message?	*/
    dt_err_string(ctl, NULL, 0);
  else
  {
	const char *progname;
	char tmp[512];
	if (ctl &&
		ctl->progname != NULL)			/* progname is set?				    */
	  progname = ctl->progname;			/* use it						    */
	else
      progname = "";

	if (caller != NULL &&				/* caller is given?				    */
        strlen(caller) != 0 &&
        strcmp(progname, caller))		/* caller is not same as progname?	*/
	  dtstr_printf(ctl, tmp,
        sizeof(tmp), "%s%s%s: %s: ",
        progname, 
		progname[0]? ": ": "",
        caller,
        dt_err_prefix);
	else								/* no known caller?				    */
      dtstr_printf(ctl, tmp,
        sizeof(tmp), "%s: %s: ",
        progname, dt_err_prefix);

    dt_err_string(ctl, tmp, -1);		/* send the message prefix		    */

    dtstr_printf_va(ctl, tmp,			/* make the message proper		    */
      sizeof(tmp), format, ap);

    dt_err_string(ctl, tmp, -1);		/* send the message proper		    */

    tmp[0] = '\n';
    dt_err_string(ctl, tmp, 1);			/* finish with a newline		    */

#   if DTPRJ_STRATOS_MEMCHECK >= 1		/* using MemCheck?					*/
    {
      void *p = malloc(2);				/* fix MemCheck 3.0 link error	    */
      if (p) free(p);
    }
#   endif
  }

  if (ctl == NULL)						/* there is no ctl structure?		*/
    exit(1);

  return DT_RC_BAD;
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
