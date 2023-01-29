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
DT_RCSID("source/low $RCSfile: dbg.c,v $ $Revision: 1.1 $");
#include <dtack/str.h>
#include <dtack/cstack.h>

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

/* ------------------------------------------------------------------------	*/
static
void
___log_va(
  const char *f,
  const char *format,
  va_list ap)
{
  FILE *file = stderr; 					/* fopen("/tmp/tmp.out", "a");		*/
  if (file)
  {
    char s[64];
    time_t clock = time(NULL);			/* current time 					*/
    struct tm *t = localtime(&clock);	/* format into fields 				*/
    strftime(s, sizeof(s),				/* express as string 				*/
      "%Y%m%d.%H%M.%S", t);

    fprintf(file, "%s [%5d] ", s, getpid());
    fprintf(file, "%s: ", f);
    vfprintf(file, format, ap);
    fprintf(file, "\n");
    if (file != stderr &&
		file != stdout)
	  fclose(file);
  }
}

/* ------------------------------------------------------------------------	*/
static
void
___log(
  const char *f,
  const char *format,
  ...)
{
  va_list ap;
  va_start(ap, format);
  ___log_va(f, format, ap);
  va_end(ap);
}
/* ------------------------------------------------------------------------	*/
// ----------------------------------------------------------------------------
//

										/* crypt functions */
#include <crypt.i>

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
#ifdef NONO
	if (fd != FILENO(stdout) &&
		fd != FILENO(stderr))
      CLOSE(fd);
#endif
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
dt_dbg_string(							/* no prefix or substitutes			*/
  struct dt_ctl_t *ctl,
  const char *string,
  int length)
{
  DT_F("dt_dbg_string");
  void *dbgto;

  if (ctl != NULL &&					/* we have ctl? 					*/
	  ctl->dbg_string != NULL &&		/* override function given?		    */
	  ctl->dbg_string != dt_dbg_string)	/* avoid recursion 					*/
  {
    (ctl->dbg_string)(ctl,				/* call override function		    */
      string, length);
	return;
  }

  if (ctl != NULL &&					/* we have ctl? 					*/
	  ctl->dbgto != NULL)				/* output handle given?			    */
    dbgto = ctl->dbgto;					/* use it						    */
  else									/* no ctl or handle is not given?	*/
    dbgto = NULL;						/* use default						*/

  if (string == NULL)					/* this is a close message?		    */
  {
	if (ctl->dbgto == ctl->errto)		/* we are sharing the file? 		*/
      ctl->errto = NULL;
	if (ctl->dbgto == ctl->sayto)		/* we are sharing the file? 		*/
      ctl->sayto = NULL;
    ctl->dbgto = NULL;
  }


  if (dbgto != NULL)					/* output to file?					*/
  {
    fileout((int)(long)dbgto, string, 
      length);
  }
  else									/* output not to file?			    */
  {
#   if defined(DTCONFIG_IMPLIED_DOS)	/* in DOS? 							*/
	{
      fileout(FILENO(stdout), 
        string, length);
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
	}
#   else
#   ifdef DTCONFIG_IMPLIED_QNX4
      dtqnx4_dbg_string(ctl,			/* special function handles it	    */
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
| dt_dbg() and dt_dbg_va() - output debug message
| 
| DESCRIPTION
| dt_dbg() and dt_dbg_va() output a debug message
| if its class matches the current debug mask
| stored in the runtime environment.
|_index debug message handling
|
| If ~ctl~ is NULL, then dt_dbg() and dt_dbg_va() do nothing
| unless ~ctl->dbg_mask~ is :DT_DBG_MASK_ALWAYS:,
| in which the message is filtered, formatted 
| and output in the default manner.
| This involves comparing the bits in ~class_bits~ to ~ctl->dbg_mask~,
| then making a message by using ~*format~ and the variable argument
| list just like sprintf() would.
| 
| dt_dbg() does nothing but make the :va_list: structure and give it
|_Aside Functions essentially equivalent.
| to dt_dbg_va() which makes all the decisions and does all the work.
| Except for their arguments, these two functions are functionally equivalent.
|
| This section describes the default method for
| debug message filtering, formatting, and output as done by dt_dbg_va().
| The default method is used whenever ~ctl->dbg~ is NULL.
| The next subsection describes how you can provide your own functions
| to customize the default functionality.
|
| In the default method,
| before starting to format or print the message,
|_aside Message has class.
| ~class_bits~ is logically ANDed with ~ctl->dbg_mask~.
| If the result is 0, then nothing is done.
| However, if any bits in ~class_bits~ match bits in ~ctl->dbg_mask~,
|_aside Mask controls amount of debug.
| outputs a debug message formed by formatting the ~format~ string
| and the variable arguments like printf() would.
| The ~caller~ string is included in the message.
|
| ~class_bits~ may have zero or more bits turned on.
| No bits on means the message error is not printed unless
| ~ctl->dbg_mask~ is :DT_DBG_MASK_ALWAYS:.
| If multiple bits are on, this signifies that the accompanying
| message applies to more than one debug class.
| 
| Each bit position in ~class_bits~ is assigned a
| particular debug message class.
| There is maximum of 32 debug message classes.
| The table below lists the predefined debug message classes
| along with the symbolic names from the file :dtack/base.h:.
|_index classes,, debug
|_index debug,, classes
| 
| #Debug message class#		#Typical use#
| :DT_DBG_MASK_ALIMITS:		machine or program limits
| :DT_DBG_MASK_ALL:			amalgamation of all class bits
| :DT_DBG_MASK_ALT:			program-choosen alternative
| :DT_DBG_MASK_ALWAYS:		always output
| :DT_DBG_MASK_ARG:			messages about arguments and their parsing
| :DT_DBG_MASK_ASSERT:		assertion messages
| :DT_DBG_MASK_ASYNC:		asynchronous interactive events
| :DT_DBG_MASK_BIGIO:		major file I/O activities
| :DT_DBG_MASK_COMPLEX:		execution complexity indicators
| :DT_DBG_MASK_CSTACK:		C compiler stack
| :DT_DBG_MASK_IMG:			high level image operations
| :DT_DBG_MASK_INTERMD:		intermediate process results
| :DT_DBG_MASK_IO:			all file I/O activities
| :DT_DBG_MASK_IPC:			interprocess communication activity
| :DT_DBG_MASK_LUT:			lookup table operations
| :DT_DBG_MASK_MEMSTATS:	memory usage summary
| :DT_DBG_MASK_MEMTRACE:	malloc(), realloc() and free()
| :DT_DBG_MASK_MEMX:		external big memory operations
| :DT_DBG_MASK_OBJECT:		operations or values of objects
| :DT_DBG_MASK_PAGE:		big memory paging
| :DT_DBG_MASK_PIXEL:		operations or values of pixels
| :DT_DBG_MASK_STATS:		statistics
| :DT_DBG_MASK_TIME:		execution timing
| :DT_DBG_MASK_TRACE:		non-zero return code traceback
| :DT_DBG_MASK_USR1:		user's use
| :DT_DBG_MASK_USR2:		user's use
| :DT_DBG_MASK_WIN:			windowing client/server operations
|
| In applications where speed is critical,
|_aside Avoid function call overhead.
| it may be detrimental for your function to call dt_dbg() only to have
| it determine that the debug output would be filtered out anyway.
| In these cases, a calling program may use the :DT_DBG: macro
| to determine whether a certain class of debug message is enabled.
| This macro takes two arguments: a :dt_ctl_t: structure pointer
| and a ~class_bits~ number.
| This macro evaluates to 1 if the current debug mask
| together with the given ~class_bits~ would produce a debug message.
| If ~ctl~ is NULL, this macro evaluates to 0.
| The example code snippet below illustrates this macro.
|
| Both ~*caller~ and ~ctl->progname~ are integrated into the message,
|_Aside Makes a prettier message.
| if they are not NULL or zero-length.
|
|dxinclude(strprt1.dx)
|
|.CUSTOMIZING 
| The :dbg: Function Group offers two levels of customization flexibility.
| Both depend on the ~*ctl~ structure.
| In the first level,
| you can provide your own function to completely replace all debug handling.
| In the second level, you can let dt_dbg() do the message
| filtering and formatting,
| and write your own routine to take over just the actual output 
| of strings.
| 
| The first thing dt_dbg() does is to check the value of ~ctl->dbg~.
|_aside Complete customization.
| If not NULL, this value is assumed a pointer to a custom function.
| The custom function's prototype is exactly the same as that of dt_dbg_va().
| The custom function pointed to is called and no further action is taken.
| The custom function is responsible for implementing any filtering
| based on ~class_bits~.
| It is also responsible for all formatting and output of the message.
| This mechanism allows you to supplant the standard debug message handling
| with a routine of your own.
|
| If ~ctl->dbg~ is NULL, dt_dbg_va() does its default filtering
| and formatting.
| When the time comes to output a string,
| it looks at ~ctl->dbg_string~.
| If not NULL, this is assumed to point to a custom function.
|_Aside Customized output only.
| This function may be called multiple times for each line of debug output.
| The end of a line is marked by a newline character in the string.
|
| The prototype for ~ctl->dbg_string~ in :dtack/base.h: is:
| 
|:  typedef
|:  void 
|:  dt_dbg_string_f(					// debug string output 
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
| Nothing at all is done if ~ctl~ is NULL.
| 
| If ~ctl->dbg~ is not NULL, it must point to a function
| declared as type :dt_dbg_f:.
|
| If ~ctl->dbg_string~ is not NULL, it must point to a function
| declared as type :dt_dbg_string_f:.
| 
|dxinclude(strprt3.dx)
| 
| SEE ALSO
| Please refer to the
|_hyper Overview, "Overview"
| and
|_hyper sections, The :dbg: Function Group
| 
| The following functions are also related:
| dt_enter_parse()	parse command line, including debug mask
| dtstr_printf()	format variable arguments to string
|
| EXAMPLES
| For a complete and working example program, please see
|_hyper section, Example :dtenv1.c:
| 
| The following code snippet highlights this function:
| 
|:  dt_dbg(&ctl, "myname",				// output debug message
|_aside Simple debug.
|:    DT_DBG_MASK_ALWAYS,				// don't ever filter this one out
|:    "int %d, long 0x%08lx, %s",		// the format string
|:    3, (void *)0x12345678L, "howdy");
|
| The following message would only be output if the
| :DT_DBG_MASK_PAGE: bit were on in ~ctl.dbg_mask~.
| 
|:  dt_dbg(&ctl, "myname",				// output debug message
|_aside Filtered debug.
|:    DT_DBG_MASK_PAGE,					// PAGE class may be filtered out
|:    "this message may not appear");
| 
| END
 * ........................................................................ */

void
DTCONFIG_API2
dt_dbg(									/* output debug message				*/
  dt_ctl_t *ctl,						/* environment control 				*/
  const char *caller,					/* caller's name 					*/
  const long class_bits,				/* debug message class 				*/
  const char *format,					/* message format, like sprintf() 	*/
  ...)									/* format substitutions 			*/
{
  va_list ap;
  va_start(ap, format);
  dt_dbg_va(ctl, caller, class_bits,	/* let dt_dbg_va() do all the work 	*/
    format, ap);
  va_end(ap);
}

/*..........................................................................
 *..........................................................................*/

void
DTCONFIG_API1
dt_dbg_va(								/* output debug message, va_list	*/
  dt_ctl_t *ctl,						/* environment control 				*/
  const char *caller,					/* caller's name 					*/
  const long class_bits,				/* debug message class 				*/
  const char *format,					/* message format, like vsprintf() 	*/
  va_list ap)							/* format substitutions 			*/
{


  dtcstack_mark2(ctl, caller);			/* track compiler stack usage 		*/
  
  if (ctl != NULL &&					/* there is a ctl structure?		*/
      ctl->dbg != NULL &&				/* there is a dbg function?			*/
      ctl->dbg != (dt_dbg_f *)dt_dbg)	/* the dbg function is not us?		*/
	(*ctl->dbg)(ctl, caller,			/* call the override function		*/
      class_bits, format, ap);
  else									/* we will do the default action	*/
  if (format == NULL)					/* this is a close-down message?	*/
    dt_dbg_string(ctl, NULL, 0);
  else
  {
    if (class_bits == DT_DBG_MASK_ANY ||
		class_bits == DT_DBG_MASK_ALWAYS ||
        (ctl && (class_bits & ctl->dbg_mask)))
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

      dt_dbg_string(ctl, tmp, -1);		/* send the message prefix		    */

      dtstr_printf_va(ctl, tmp,			/* make the message proper		    */
        sizeof(tmp), format, ap);
      dt_dbg_string(ctl, tmp, -1);		/* send the message proper		    */

      tmp[0] = '\n';
      dt_dbg_string(ctl, tmp, 1);		/* finish with a newline		    */

#     if DTPRJ_STRATOS_MEMCHECK >= 1	/* using MemCheck?					*/
      {
        void *p = malloc(2);			/* fix MemCheck 3.0 link error	    */
        if (p) free(p);
      }
#     endif
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
