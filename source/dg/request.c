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
  19990814 DE added custom separator
 *..........................................................................*/



/*..........................................................................
| NAME
| Introduction - intro
| 
| ERRORS
| As yet undocumented.
| 
| END
 *..........................................................................*/

#include <dtack/base.h>
DT_RCSID("source/dg $RCSfile: request.c,v $ $Revision: 1.8 $");

#include <dtack/ipc.h>
#include <dtack/list.h>
#include <dtack/dg.h>
#include <dtack/str.h>

/*..........................................................................
 *..........................................................................*/

dt_rc_e 
DTCONFIG_API1
dtdg_server_request2(
  dt_ctl_t *ctl,
  dtdg_server_t *dgserver,
  char *request,
  char separator,
  void *arg,
  const char **requests,
  dtdg_server_request_f **funcs,
  char *message,
  int message_max)
{
  DT_F("dtdg_server_request2");
  int n;
  int choice;
  char *path;
  char *rest;
 
  dt_dbg(ctl, F, DT_DBG_MASK_COMMAND,	/* debug the request 				*/
    "%s", request);

  dtstr_tokenize(ctl, request, ' ', 	/* split input into tokens 			*/
    &n);

  if (n == 0)							/* no input tokens? 				*/
  {
    DT_Q(dtstr_printf,(ctl,
      message, message_max,
	  "Sorry, no command received."));
	return DT_RC_GOOD;
  }

  if (strcmp(request, "GET"))			/* command is not GET? 				*/
  {
    DT_Q(dtstr_printf,(ctl,
      message, message_max,
	  "Sorry,"
      " expected GET but got \"%s\".", 
      request));
	return DT_RC_GOOD;
  }

  if (n == 1)							/* no path after GET? 				*/
  {	
    DT_Q(dtstr_printf,(ctl,
      message, message_max,
	  "Sorry, nothing received"
      " after GET."));
	return DT_RC_GOOD;
  }

  path = request+strlen(request)+1;		/* skip the GET 					*/
  rest = strchr(path+1, separator);		/* find the command separator 		*/
  if (rest == NULL)						/* no separator?					*/
    rest = "";							/* whole thing is the command 		*/
  else									/* there is a question mark? 		*/
    *rest++ = '\0';						/* split line at question mark		*/

  DT_Q(dtstr_choice2,(ctl,				/* look up function 				*/
    requests, path+1, &choice,
    message, message_max));

  if (choice >= 0)						/* valid choice? 					*/
  {
	dt_rc_e rc;
	DT_G((funcs[choice]),(ctl,			/* call appropriate function 		*/
      dgserver, arg,
      path+1, rest, 
      message, message_max));
	if (rc == DT_RC_STOP ||				/* stop code?						*/
		rc == DT_RC_SHUTDOWN)
	  return rc;
  }

  return DT_RC_GOOD;
}


/*..........................................................................
 *..........................................................................*/

dt_rc_e 
DTCONFIG_API1
dtdg_server_request(
  dt_ctl_t *ctl,
  dtdg_server_t *dgserver,
  char *request,
  void *arg,
  const char **requests,
  dtdg_server_request_f **funcs,
  char *message,
  int message_max)
{
  DT_F("dtdg_server_request");
  DT_Q(dtdg_server_request2,(
    ctl,
	dgserver,
	request,
    '?',
	arg,
	requests,
	funcs,
	message,
	message_max));
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



