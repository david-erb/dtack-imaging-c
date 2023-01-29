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
DT_RCSID("test $RCSfile: assert.c,v $ $Revision: 1.1 $");
#include <dtack/os.h>
#include <dtack/str.h>

/*..........................................................................
 * dt_assert_defined - complain that support is not defined
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dt_assert_defined(
  dt_ctl_t *ctl,
  int isdefined,
  char *F,
  char *symbol,
  char *desc)
{
  return dt_assert(ctl, isdefined, F,
    "%s%s defined:%s compiled support for %s",
    symbol, 
    isdefined? " is": "",
    isdefined? " not": " no",
    desc);
}

/* ........................................................................ *
| NAME
| dt_assert() and dt_assert_va() - output debug message
| 
| DESCRIPTION
| dt_assert() and dt_assert_va() output a debug message
| based on the truth of a given condition.
|
| If ~condition~ is non-zero,
| a debug message of class :DT_DBG_MASK_ASSERT: is issued,
| otherwise
| a debug message of class :DT_DBG_MASK_ALWAYS: is issued
|_index DT_DBG_MASK_ASSERT,:DT_DBG_MASK_ASSERT:, condition truth
|_index DT_DBG_MASK_ALWAYS,:DT_DBG_MASK_ALWAYS:, condition false
| The message is issued through dt_dbg(),
| giving it the ~caller~, ~format~, and all the variable arguments.
| The message is formatted by expanding ~*format~
| and the variable arguments like printf() would.
| The ~caller~ string is included in the message.
| 
| dt_assert() does nothing but make the :va_list: structure and give it
|_Aside Functions essentially equivalent.
| to dt_assert_va() which makes all the decisions and does all the work.
| In terms of functionality, these two functions are functionally equivalent.
|
| If ~ctl~ is NULL, then dt_assert() and dt_assert_va() 
| will not print successful assertions, only failed ones.
|
| If your program uses the dt_enter_parse() function,
|_aside Mask can be specified on command line.
| then ~ctl->dbg_mask~ is automatically set up
| according to the value of the :-D: command line parameter.
| Setting :-D assert: on the command line
| enables the :DT_DBG_MASK_ASSERT: debug messages.
| Please see the
|_hyper dt_assert_parse()
| function description.
| 
| This function is used heavily in the Dtack Imaging Software test programs.
| The test programs want silence when there are no problems,
| but always want to see failed assertions.
| From time to time, it may be interesting for the human operator
| to see the successful assertions as well.
| This is accomplished by setting :-D assert: on the command line.
|
| Both ~*caller~ and ~ctl->progname~ are integrated into the message,
|_Aside Makes a prettier message.
| if they are not NULL or zero-length.
|
|dxinclude(strprt2.dx)
|
| RETURN VALUES
| A formatted error message is output through dt_dbg().
| This function returns :DT_RC_GOOD: if the assertion passed,
| otherwise it returns :DT_RC_BAD:.
| However, unlike most Dtack Imaging Software functions,
| dt_err() is #NOT# called by this function before returning :DT_RC_BAD:
| 
| ERRORS
| None.
| 
| CAVEATS
| dt_err() is #NOT# called by this function before returning :DT_RC_BAD:
| 
| Successful assertions are never printed if ~ctl~ is NULL.
| 
| If you don't want dt_dbg() output,
| then don't call this function since it may use :DT_DBG_MASK_ALWAYS:.
| 
| It is best to keep the ~*format~ fairly simple.
| The number of bytes which can be formatted in one call is limited.
| The arguments in the variable list must match their respective
| format specifiers in the ~*format~.
| 
| SEE ALSO
| Please refer to the
|_hyper section, Overview
| 
| The following functions are also related:
| dt_dbg()	issue a debug message
|
| EXAMPLES
| For a complete and working example program, please see
|_hyper section, Example :dtenv1.c:
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
dt_assert(
  dt_ctl_t *ctl,						/* environment control 				*/
  int condition,						/* assertion truth condition 		*/
  const char *caller,					/* caller's name 					*/
  const char *format,					/* message format, like printf() 	*/
  ...)									/* format substitutions 			*/
{
  dt_rc_e rc;
  va_list ap;
  va_start(ap, format);
  rc = dt_assert_va(ctl, condition,		/* let va do the work 				*/
    caller, format, ap);
  va_end(ap);
  return rc;
}

/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dt_assert_va(
  dt_ctl_t *ctl,						/* environment control 				*/
  int condition,						/* assertion truth condition 		*/
  const char *caller,					/* caller's name 					*/
  const char *format,					/* message format, like printf() 	*/
  va_list ap)							/* format substitutions 			*/
{
  DT_F("dt_assert_va");
  char string[256];

  DT_Q(dtstr_printf_va,(ctl, string,	/* format the caller's string 		*/
    sizeof(string), format, ap));

  if (condition)						/* check assertion true?		    */
  {
    dt_dbg(ctl, caller,
      DT_DBG_MASK_ASSERT, 
      "  %s", string);
    return DT_RC_GOOD;
  }
  else
  {
#ifdef NONO
    dt_ctl_t myctl;
	if (ctl)
	  myctl = *ctl;
	else
 	  DT_MEMSET(&myctl, 0, sizeof(myctl));
    myctl.dbg_string =					/* goes out through error string    */
      (dt_dbg_string_f *)
      (myctl.err_string?				/* caller has a custom routine? 	*/
       myctl.err_string:				/* use custom routine 				*/
       dt_err_string);					/* otherwise use default routine 	*/
#endif
    dt_dbg(ctl,  caller,				/* goes out formatted like debug    */
      DT_DBG_MASK_ALWAYS,
      "* %s", string);
    return DT_RC_BAD;
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
