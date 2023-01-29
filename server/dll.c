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
#include <dtack/str.h>
#include <dtack/main.h>

DT_RCSID("server $RCSfile: dll.c,v $ $Revision: 1.12 $");

/*..........................................................................
 *..........................................................................*/

static
dt_rc_e 
DTCONFIG_API1
dtstr_find_white(						/* find next whitespace character 	*/
  dt_ctl_t *ctl,						/* environment control 				*/
  char *string,							/* string to search 				*/
  char **white)							/* returned pointer to next white 	*/
{
  while(*string != '\0' &&
		*string != '\t' &&
		*string != '\n' &&
		*string != ' ')
    string++;
  *white = string;
  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/

static
dt_rc_e 
DTCONFIG_API1
dtstr_find_nonwhite(					/* find next non-whitespace char	*/
  dt_ctl_t *ctl,						/* environment control 				*/
  char *string,							/* string to search 				*/
  char **nonwhite)						/* returned pointer to non-white 	*/
{
  while(*string != '\0' &&
		(*string == '\t' ||
		 *string == '\n' ||
		 *string == ' '))
    string++;
  *nonwhite = string;
  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/

dt_rc_e 
DTCONFIG_API1
dtstr_find_quote(						/* find next quote character 		*/
  dt_ctl_t *ctl,						/* environment control 				*/
  char *string,							/* string to search 				*/
  char **quote)							/* returned pointer to next quote 	*/
{
  while(*string != '\0' &&
		*string != '"')
    string++;
  *quote = string;
  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/

static
dt_rc_e 
DTCONFIG_API1
dtstr_argv(								/* make argv from string			*/
  dt_ctl_t *ctl,						/* environment control 				*/
  char *string,							/* string to be parsed 				*/
  char **argv,							/* returned array of token pointers	*/
  int maxargv,							/* maximum tokens to return 		*/
  int *argc)							/* returned number of tokens found 	*/
{
  char *s = string;
  char *e = s + strlen(s);
  int n = 0;
  do {
    char *t;
    dtstr_find_nonwhite(ctl, s, &s);	/* find non-white				    */
    if (s == e)							/* end of string?				    */
      break;

    if (*s == '"')						/* token starts with a quote?	    */
      dtstr_find_quote(ctl, ++s, &t);	/* find close quote					*/
    else
      dtstr_find_white(ctl, s, &t);		/* find white				    	*/

    if (n < maxargv)					/* we can save this token?		    */
	{
      argv[n++] = s;					/* save where token starts		    */
      *t = '\0';						/* end the token				    */
	}

    s = t + 1;							/* skip the terminator			    */
  } while (s < e);

  *argc = n;
  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/

static
void
DTCONFIG_API2
dt_dbg(									/* output debug message				*/
  dt_ctl_t *ctl,						/* environment control 				*/
  const char *caller,					/* caller's name 					*/
  const long class_bits,				/* debug message class 				*/
  const char *format,					/* message format, like sprintf() 	*/
  ...)									/* format substitutions 			*/
{
}

/* ........................................................................ *
 * ........................................................................ */

static
dt_rc_e
DTCONFIG_API2
dt_err(									/* handle errors					*/
  dt_ctl_t *ctl,						/* environment control 				*/
  const char *caller,					/* caller's name 					*/
  const char *format,					/* message format, like sprintf() 	*/
  ...)									/* format substitutions 			*/
{
  return DT_RC_BAD;
}

/* ........................................................................ *
 * ........................................................................ */

DTMAIN1(dtdll)
{
  DT_F("dtdll");
  char *libhandle = argv[1];			/* first tokens are required 		*/
  char *function = argv[2];
  char *end;
  long longval;
  HANDLE handle;
  dtmain_f *funcptr;
  dt_rc_e rc;
  
  longval = strtol(libhandle, &end, 0);	/* try the conversion			    */

  if (end == libhandle ||				/* make sure handle is a number 	*/
      *end != '\0' ||					/* followed by nothing				*/
	  longval <= 0L ||					/* make sure handle is reasonable 	*/
	  longval > 65535L)
  {
	char tmp[256];
	sprintf(tmp,
      "\"%s\" is not a valid"
      " handle", 
      libhandle);
	MessageBox((HWND)NULL, 
      tmp, F, MB_OK);
	rc = DT_RC_BAD;
	goto cleanup;
  }

  handle = (HANDLE)longval;				/* handle to the library 			*/

  funcptr = (dtmain_f *)GetProcAddress(	/* address the DLL function 		*/
    handle, function);
    
  if (funcptr == NULL)
  {
	char tmp[256];
	sprintf(tmp,
      "GetProcAddress(%d, \"%s\")"
      " failed",
      handle, function);
	MessageBox((HWND)NULL, 
      tmp, F, MB_OK);
	rc = DT_RC_BAD;
	goto cleanup;
  }

  argc -= 2;							/* ignore first two arguments 		*/
  argv += 2;
  argv[0] = function;
  rc = (*funcptr)(argc, argv, param);	/* call the DLL function 			*/
  
cleanup:
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
