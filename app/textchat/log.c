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
| NAME
| textchat_* - textchat internal routine
| 
|.DESCRIPTION
| Undocumented.
| 
| END
 *..........................................................................*/

#include <textchat.h>
DT_RCSID("app/textchat $RCSfile: log.c,v $ $Revision: 1.8 $");
#include <dtack/str.h>

/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1		
textchat_log_setup(						/* set up file logging				*/
  textchat_t *textchat)
{
  DT_F("textchat_log_setup");
  dt_ctl_t *ctl = textchat->ctl;

  DT_Q(dtstr_printf,(ctl,
    textchat->logbase,
    sizeof(textchat->logbase),
    TEXTCHAT_LOGBASE, textchat->ident));
  if (ctl->errto == NULL)				/* default error handling? 			*/
    ctl->err_string = 
      textchat_log_string;
  if (ctl->dbgto == NULL)
    ctl->dbg_string = 
      textchat_log_string;
  ctl->user2 = (void *)textchat;
  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1		
textchat_log_quit(						/* quit file logging				*/
  textchat_t *textchat)
{
  DT_F("textchat_log_setup");
  dt_ctl_t *ctl = textchat->ctl;

  ctl->err_string = NULL;
  ctl->dbg_string = NULL;
  ctl->user2 = NULL;

  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/

void
DTCONFIG_API1		
textchat_log_string(					/* output message to log file		*/
  dt_ctl_t *ctl,						/* environment control 				*/
  const char *string,
  int length)
{
  textchat_t *textchat = 
    (textchat_t *)ctl->user2;
  int m;
  int i;

  if (textchat == NULL)					/* structure not initialized yet? 	*/
    return;
  
  m = strlen(textchat->logline);
  
  if (length == -1)						/* caller giving -1 length? 		*/
    length = strlen(string);

  for (i=0; i<length; i++)				/* tack string on current line 		*/
    if (m < TEXTCHAT_LOGLINE_MAX-2)
	  textchat->logline[m++] =
        string[i];
    else
    if (m == TEXTCHAT_LOGLINE_MAX-2)
      textchat->logline[m++] = '\n';
  
  textchat->logline[m] = '\0';

  if (string[length-1] == '\n')			/* line is finished?				*/
  {
	int d = strrchr(textchat->logbase,	/* position at final '.' 			*/
      '.') - textchat->logbase;
    time_t clock = time(NULL);			/* current time 					*/
    struct tm *t = localtime(&clock);	/* format into fields 				*/
	FILE *file;
	int n = 0;
    strftime(							/* express as filename				*/
      textchat->logbase+d,
      TEXTCHAT_LOGBASE_MAX-d,
      ".%y%m%d", t);
	file = fopen(textchat->logbase,		/* open log for append 				*/
      DTCONFIG_FOPEN_ATEXT);
	if (file != NULL)					/* cannot open file? 				*/
	{
	  n = fprintf(file, "%s",			/* emit the line 					*/
        textchat->logline);
	  if (textchat->logline[m-1] != '\n')
	    fprintf(file, "\n");
      fclose(file);
	}
	if (n < strlen(textchat->logline))	/* write to log file failed? 		*/
	  printf("!%s", textchat->logline);
	textchat->logline[0] = '\0';			/* clear the line 					*/
  }
}

/*..........................................................................
 *..........................................................................*/

static
void
textchat_stamp(							/* output debug message				*/
  dt_ctl_t *ctl,						/* environment control 				*/
  const char *caller,					/* caller's name 					*/
  char *s,
  int lmax)
{
  int pid;
  time_t clock = time(NULL);			/* current time 					*/
  struct tm *t = localtime(&clock);		/* format into fields 				*/
  strftime(s, lmax,						/* express as string 				*/
    "%Y%m%d.%H%M.%S", t);

  dtos_getpid(ctl, &pid);

  dtstr_printf(ctl,						/* tack on the caller name and pid	*/
    s+strlen(s), lmax-strlen(s),
    " [%5d]%s%s", pid, 
    caller? " ": "",
    caller? caller: "");
}

/*..........................................................................
 *..........................................................................*/

void
DTCONFIG_API1		
textchat_dbg(							/* output debug message				*/
  dt_ctl_t *ctl,						/* environment control 				*/
  const char *caller,					/* caller's name 					*/
  long class_bits,						/* debug message class 				*/
  const char *format,					/* message format, like sprintf() 	*/
  va_list ap)							/* format substitutions 			*/
{
  char s[64];
  ctl->dbg = NULL;						/* don't come back here 			*/
  textchat_stamp(ctl, caller, 
    s, sizeof(s));
  dt_dbg_va(ctl, s, class_bits,			/* let dt_dbg_va() do all the work 	*/
    format, ap);
  ctl->dbg = textchat_dbg;				/* come back here next time 		*/
}

/* ........................................................................ *
 * ........................................................................ */

dt_rc_e
DTCONFIG_API1		
textchat_err(							/* handle errors					*/
  dt_ctl_t *ctl,						/* environment control 				*/
  const char *caller,					/* caller's name 					*/
  const char *format,					/* message format, like sprintf() 	*/
  va_list ap)							/* format substitutions 			*/
{
  textchat_t *textchat = 
    (textchat_t *)ctl->user;
  dt_rc_e rc;
  char s[64];
  ctl->err = NULL;						/* don't come back here 			*/
  textchat_stamp(ctl, caller, 
    s, sizeof(s));
  if (textchat != NULL)					/* handling command for client? 	*/
  {
	if (!textchat->message[0])			/* no error yet? 					*/
	{
	  dtstr_printf_va(ctl,				/* keep first error 				*/
        textchat->message,
        sizeof(textchat->message),
        format, ap);
	  dt_dbg(ctl, caller,				/* log the error 				*/
        DT_DBG_MASK_USR1,
        "%s", textchat->message);
	}
	else								/* not first error? 				*/
	{
	  dt_dbg_va(ctl, caller,			/* log the error 				*/
        DT_DBG_MASK_USR1,
        format, ap);
	}
  }
  else									/* not first error? 				*/
    rc = dt_err_va(ctl, s,				/* log the error 					*/
      format, ap);

  ctl->err = textchat_err;				/* come back here next time 		*/
  return rc;
}

/*..........................................................................*
 *..........................................................................*/

void
DTCONFIG_API1		
textchat_say(							/* output console message			*/
  dt_ctl_t *ctl,						/* environment control 				*/
  const char *caller,					/* caller's name 					*/
  const char *format,					/* message format, like sprintf() 	*/
  va_list ap)							/* format substitutions 			*/
{
  char s[64];
  ctl->say = NULL;						/* don't come back here 			*/
  textchat_stamp(ctl, caller, 
    s, sizeof(s));

  dt_say_va(ctl, s,
    format, ap);
  ctl->say = textchat_say;				/* come back here next time 		*/
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
