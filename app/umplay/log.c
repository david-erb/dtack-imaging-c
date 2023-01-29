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
| Introduction - intro
| 
| ERRORS
| As yet undocumented.
| 
| END
 *..........................................................................*/


#include <umplay.h>
U_R("app/umplay $RCSfile: log.c,v $ $Revision: 1.8 $");

/*..........................................................................
 *..........................................................................*/

u_rc_e
U_API
umplay_log_setup(						/* set up file logging				*/
  u_ctl_t *ctl)							/* environment control 				*/
{
  if (ctl->errto == NULL)				/* default error handling? 			*/
    ctl->err_string = umplay_log_string;
  if (ctl->dbgto == NULL)
    ctl->dbg_string = umplay_log_string;
  return U_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/

void
U_API
umplay_log_string(						/* output message to log file		*/
  u_ctl_t *ctl,							/* environment control 				*/
  const char *string,
  int length)
{
  umplay_t *umplay = 
    (umplay_t *)ctl->user;
  int l;
  int i;

  if (umplay == NULL)					/* structure not initialized yet? 	*/
    return;
  
  l = strlen(umplay->logline);
  
  if (length == -1)						/* caller giving -1 length? 		*/
    length = strlen(string);

  for (i=0; i<length; i++)				/* tack string on current line 		*/
    if (l < UMPLAY_LOGLINE_MAX-2)
	  umplay->logline[l++] =
        string[i];
    else
    if (l == UMPLAY_LOGLINE_MAX-2)
      umplay->logline[l++] = '\n';
  
  umplay->logline[l] = '\0';

  if (string[length-1] == '\n')			/* line is finished?				*/
  {
    time_t clock = time(NULL);			/* current time 					*/
    struct tm *t = localtime(&clock);	/* format into fields 				*/
	char filename[128];
	FILE *file;
	int n = 0;
    strftime(filename,					/* express as filename				*/
      sizeof(filename),
      UMPLAY_LOGFILE ".%y%m%d", t);
	file = fopen(filename,				/* open log for append 				*/
      DTCONFIG_FOPEN_ATEXT);
	if (file != NULL)					/* cannot open file? 				*/
	{
	  n = fprintf(file, "%s",			/* emit the line 					*/
        umplay->logline);
	  if (umplay->logline[l-1] != '\n')
	    fprintf(file, "\n");
      fclose(file);
	}
	if (n < strlen(umplay->logline))	/* write to log file failed? 		*/
	  printf("!%s",  umplay->logline);
	umplay->logline[0] = '\0';			/* clear the line 					*/
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
