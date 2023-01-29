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
 * this file contains functions which cannot live in a Microsoft Windows dll
 *..........................................................................*/

#include <dtack/base.h>
#include <dtack/str.h>

/*..........................................................................
 *..........................................................................*/

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

/* ........................................................................ *
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

dt_rc_e 
DTCONFIG_API2
dt_enter_arg(							/* set up argument structure 		*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dt_enter_arg_t *arg,					/* argument structure to set up 	*/
  ...)
{
  dt_rc_e rc;
  va_list ap;
  va_start(ap, arg);					/* start variable arg procesing		*/
  rc = dt_enter_arg_va(ctl, arg, ap);
  va_end(ap);
  return rc;
}

/* ........................................................................ *
 * ........................................................................ */

dt_rc_e 
DTCONFIG_API2
dt_enter_arg2(							/* set up argument structure 		*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dt_enter_arg_t *arg,					/* argument structure to set up 	*/
  ...)
{
  dt_rc_e rc;
  va_list ap;
  va_start(ap, arg);					/* start variable arg procesing		*/
  rc = dt_enter_arg2_va(ctl, arg, ap);
  va_end(ap);
  return rc;
}

/* ........................................................................ *
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

/*..........................................................................*
 *..........................................................................*/

dt_rc_e
DTCONFIG_API2
dtparam_set(							/* mark keywords for parsing 		*/
  dtparam_t *param,						/* ~param~ structure to mark 		*/
  dtparam_flag_t flags,					/* flags for all keywords 			*/
  ...)
{
  dt_rc_e rc;
  va_list ap;
  va_start(ap, flags);
  rc = dtparam_set_va(param, flags, ap);
  va_end(ap);
  return rc;
}

/*..........................................................................*
 *..........................................................................*/

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

dt_rc_e
DTCONFIG_API2
dtstr_printf(							/* printf-like replacement			*/
  dt_ctl_t *ctl,						/* environment control 				*/
  char *string,							/* output string 					*/
  int max,								/* maximum output length 			*/
  const char *format,					/* format string with %-signs 		*/
  ...)									/* variable argument list 			*/
{
  dt_rc_e rc;
  va_list ap;
  va_start(ap, format);
  rc = dtstr_printf_va(ctl,
    string, max, format, ap);
  va_end(ap);
  return rc;
}

/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API2
dt_yield(								/* yield timeslice 					*/
  dt_ctl_t *ctl,						/* environment control 				*/
  const char *format,					/* format of yield message 			*/
  ...)
{
  dt_rc_e rc;
  va_list ap;
  va_start(ap, format);
  rc = dt_yield_va(ctl, format, ap);	/* let va do the work 				*/
  va_end(ap);
  return rc;
}

/* ........................................................................ *
 * ........................................................................ */





/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
  ! Copyright (C) 1998 Dtack Inc. All Rights Reserved
  ! To use this file, you must have signed a license agreement with Dtack Inc.
  ! Under no circumstances may you redistribute this file.
  ! This software is provided AS IS with no warranty expressed or implied.
  ! Dtack Inc. accepts no liability for use or misuse of this file.
  ! http://www.dtack.com  dtack@dtack.com  telephone +360.417.6700
  ! Dtack Inc, P.O. Box 3061, Port Angeles, WA USA 98362
  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */
