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
DT_RCSID("source/low $RCSfile: dbg2.c,v $ $Revision: 1.1 $");
#include <dtack/str.h>

/* ........................................................................ *
| NAME
| dt_dbg_trace() - output program location debug message
| 
| DESCRIPTION
| dt_dbg_trace() outputs a debug message of class :DT_DBG_MASK_TRACE:
| identifying a particular program location.
|_index traceback
|
| If ~ctl->dbg_mask~ indicates that debug messages of class
|_index DT_DBG_MASK_TRACE, :DT_DBG_MASK_TRACE: debug class
|_aside Only outputs if TRACE class enabled.
| :DT_DBG_MASK_TRACE: are enabled, then this function
| outputs a debug message formed by expanding the ~format~ string
| using the variable arguments like sprintf() would.
| ~*caller~, ~*sourcefile~ and ~sourceline~ are also included in the message.
|
| This function does nothing if ~ctl~ is NULL.
|
| After adding ~*caller~, ~*sourcefile~ and ~sourceline~
| to the message format,
|_aside dt_dbg_va() does all the work.
|_hyper dt_dbg() and dt_dbg_va(), dt_dbg_va()
| is called to do the work.
| The comments pertaining to restrictions on the complexity of
| the ~*format~ string discussed in the dt_dbg_va()
| function description apply here as well.
|
| ~*sourcefile~ normally is copying using the C preprocessor variable :__FILE__:
| and ~sourceline~ normally comes from :__LINE__:.
| 
| This function is used by the automated internal error-checking macros
|_aside Used heavily in library code.
| :DT_Q:, :DT_C:, and their friends.
| It provides these macros with a consistent interface
| for printing out the program location just before returning after
| called routine has reported a failure.
|
| RETURN VALUES
| None.
| 
| ERRORS
| None.
| 
| CAVEATS
| Nothing is ever done if ~ctl~ is NULL.
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
| dt_dbg() and dt_dbg_va()	output debug message
| dtstr_printf()			format variable arguments to string
|
| EXAMPLES
| For a complete and working example program, please see
|_hyper section, Example :dtenv1.c:
| 
| The following code snippet highlights this function:
| 
|:  dt_ctl_t ctl = {0};					// clear ctl structure
|:
|:  ctl.dbg_mask = DT_DBG_MASK_TRACE;	// enable TRACE class
|_aside Enable TRACE debug.
|:
|:  dt_dbg_trace(&ctl, "main",			// output debug message from main
|_Aside Do TRACE debug.
|:    __FILE__, __LINE__,				// from where in main
|:    "int %d, dbl %g, %s",				// the format string
|:    99, 1.234, "howdy again");
| 
| END
 * ........................................................................ */

void
DTCONFIG_API2
dt_dbg_trace(							/* debug program location  			*/
  dt_ctl_t *ctl,						/* environment control 				*/
  const char *caller,					/* caller's name 					*/
  const char *sourcefile,				/* caller's __FILE__ 				*/
  long sourceline,						/* caller's __LINE__ 				*/
  const char *format,					/* message format, like printf() 	*/
  ...)									/* format substitutions 			*/
{
  va_list ap;
  va_start(ap, format);
  dt_dbg_trace_va(ctl, caller, 			/* let dt_dbg_trace_va() do it all	*/
    sourcefile, sourceline,
    format, ap);
  va_end(ap);
}

/* ........................................................................ *
 * ........................................................................ */

void
DTCONFIG_API1
dt_dbg_trace_va(						/* debug program location  			*/
  dt_ctl_t *ctl,						/* environment control 				*/
  const char *caller,					/* caller's name 					*/
  const char *sourcefile,				/* caller's __FILE__ 				*/
  long sourceline,						/* caller's __LINE__ 				*/
  const char *format,					/* message format, like printf() 	*/
  va_list ap)							/* format substitutions 			*/
{
  const long class_bits =				/* our message class is fixed 		*/
    DT_DBG_MASK_TRACE;

  if (DT_DBG(ctl, class_bits))			/* this dbg will generate output?	*/
  {
	char dir[32];
    char file[32];
	const char *d1, *d2, *f1, *f2;
    char tmp[128];
	d1 = sourcefile;
	d2 = strchr(d1, ' ');
	DT_MEMSET(dir, 0, sizeof(dir));
	DT_MEMSET(file, 0, sizeof(file));
	if (d2 == NULL)
	{
	  d1 = strrchr(sourcefile, '/');
	  if (d1 == NULL)
	    d1 = sourcefile;
	  else
	    d1++;
	  strncpy(dir, d1, sizeof(dir));
	}
	else
	{
	  strncpy(dir, d1, 
        DT_MIN((unsigned int)(d2-d1), 
          sizeof(dir)));
	  for (f1 = d2;						/* find start of second token		*/
           *f1 != '\0' && 
		   *f1 == ' ';
          f1++);
	  if (*f1 == '\0')					/* no second token?					*/
	    f1 = NULL;
	  if (f1 != NULL)
	    f1 = strchr(f1, ' ');			/* find space after second token  	*/
	  if (f1 != NULL)
  	    while (*f1 != '\0' &&			/* find start of third token 		*/
			   *f1 == ' ') 
		  f1++;
	  if (f1 != NULL)
	  {
  	    for (f2 = f1;					/* find end of third token			*/
             *f2 != '\0' &&
             *f2 != ','  &&
             *f2 != ' ';
             f2++);
		file[0] = '/';
  	    strncpy(file+1, f1, 
          DT_MIN((unsigned int)(f2-f1), 
            sizeof(file)-1));
	  }
	}
	
    dtstr_printf(ctl, tmp, sizeof(tmp),	/* say where called from		    */
      "at %s%s@%ld %s",
      dir, file, sourceline,
      format);

	dt_dbg_va(ctl, caller, class_bits,	/* output the debug				    */
      tmp, ap);
  }
}

/* ........................................................................ *
| NAME
| dt_dbg_parse() - create debug mask from string
| 
| DESCRIPTION
| dt_dbg_parse() parses a string of debug classes and sets bits in a
| debug mask accordingly.
|
| ~*string~ is examined for a plus-sign separated list of debug classes.
| For each class found, the corresponding bit in ~*dbg_mask~ is set.
| If there is a parsing error,
|_aside Parsing error returned as message.
| a friendly error message is stored at ~*message~,
| never exceeding a maximum message length of ~lmax~
| including the trailing null character delimiter.
|
| ~*dbg_mask~ is set to 0 before adding any new bits.
|
| No message is returned if ~message~ is NULL or ~lmax~ is 0.
| If a message can be returned,
| a zero-length message is returned if ~*string~ is valid.
| If ~lmax~ is not long enough to hold the entire message,
| then a partial message is stored.
| 
| ~*string~ should contain a series of debug class names
| or their unambiguous abbreviations
| separated by plus (+) signs,
| such as in :trace+io+stats:.
| If ~string~ is NULL or points to a zero-length string,
| then ~*dbg_mask~ will be returned with no bits set (0).
|
| The valid class names and their typical uses are:
| #Class name#	#Typical use#
| "alimits"		machine or program limits
| "all"			amalgamation of all class bits
| "alt"			program-choosen alternative
| "arg"			messages about arguments and there parsing
| "assert"		assertion messages
| "async"		asynchronous events by user
| "bigio"		major file I/O activities
| "complex"		execution complexity indicators
| "cstack"		C compiler stack
| "img"			high level image operations
| "intermd"		intermediate process results
| "io"			all file I/O activities
| "ipc"			interprocess communication activity
| "lut"			lookup table operations
| "memstats"	memory usage summary
| "memtrace"	malloc(), realloc() and free()
| "memx"		external big memory operations
| "object"		operations or values of objects
| "page"		big memory paging
| "pixel"		operations or values of pixels
| "stats"		statistics
| "time"		execution timing
| "trace"		non-zero return code traceback
| "usr1"		user's use
| "usr2"		user's use
| "win"			windowing client/server operations
| 
| Examples of valid ~*string~ arguments are:
| #Value of string#		#Resulting dbg_mask#
| :NULL:				0
| :"":					0
| :"io":				:DT_DBG_MASK_IO:
| :"trace":				:DT_DBG_MASK_TRACE:
| :"trac":				:DT_DBG_MASK_TRACE:
| :"trac+mem+io":		:DT_DBG_MASK_IO | DT_DBG_MASK_TRACE | DT_DBG_MASK_IO:
|
| Examples of invalid ~*string~ arguments with approximate error messages are:
| #Value of string#		#Approximate error message#
| :"a":					Sorry, ambiguous class name "a".
| :"xxx":				Sorry, unrecognized class name "xxx".
|
| This function does not explicitly change the value of ~ctl->dbg_mask~.
| Of course, you can always pass ~&ctl->dbg_mask~ as ~*dbg_mask~.
|
| This function is used by dt_enter_parse() when it it sees a
| :-D: parameter on the command line
|
| RETURN VALUES
| If ~*string~ is valid, then mask bits are set in ~dbg_mask~ and
| ~message[0]~ is set to 0.
| Otherwise ~*dbg_mask~ is unchanged and an error message is stored
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
| debug mask, ~ctl->dbg_mask~.
| 
| SEE ALSO
| Please refer to the
|_hyper Overview, "Overview"
| and
|_hyper sections, The :dbg: Function Group
| 
| The following functions are also related:
| dt_dbg() and dt_dbg_va()	output debug message
| dt_enter_parse()			uses this function to parse command line parameter
|
| EXAMPLES
| For a complete and working example program, please see
|_hyper section, Example :dtenv1.c:
| 
| The following code snippet highlights this function:
|:
|:  dt_dbg_parse(&ctl, "page",			// parse the "page" string
|_aside Parse a debug string.
|:    &ctl.dbg_mask, NULL, 0);			// put straight into ctl
|:
|:  dt_dbg(&ctl, "main",				// output the debug
|_aside Enabled debug.
|:    DT_DBG_MASK_PAGE,					// this class is enabled
|:    "this message does appear");
| 
| END
 * ........................................................................ */

dt_rc_e
DTCONFIG_API1
dt_dbg_parse(							/* set dbg flags from dbg string	*/
  dt_ctl_t *ctl,						/* environment control 				*/
  const char *string,					/* string to parse 					*/
  long *dbg_mask,						/* returned debug mask 				*/
  char *message,						/* returned error message 			*/
  int lmax)								/* maximum length of message 		*/
{
  DT_F("dt_dbg_parse");
  long mask = 0;
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
          " debug class name \"%s\".",
           q));
        return DT_RC_GOOD;
	  }
	  DT_MEMCPY(t, q, n);					/* get a copy of the flag		    */
	  t[n] = '\0';
	  {
	    DT_Q(dtstr_choice,(ctl,			/* look up name in choice list	    */
          dt_dbg_mask_names, t, 
		    &choice));
	    if (choice == -1)
		{
		  if (message != NULL)
          DT_Q(dtstr_printf,(ctl, 
            message, lmax,
            "Sorry, unrecognized"
            " debug class name \"%s\".", t));
          return DT_RC_GOOD;
		}
        else
        if (choice < -1)
		{
		  if (message != NULL)
          DT_Q(dtstr_printf,(ctl, 
            message, lmax,
            "Sorry, debug class name \"%s\""
            " could match any of"
            " %d choices.",
            t, -choice));
          return DT_RC_GOOD;
		}
        else
          mask |= dt_dbg_mask_values[choice];
	  }
  	  q = p + 1;
  } while (*p);

  *dbg_mask = mask;						/* return the mask					*/
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
