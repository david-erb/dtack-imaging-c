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

#include <ustream.h>
DT_RCSID("app/ustream $RCSfile: command.c,v $ $Revision: 1.8 $");
#include <dtack/str.h>

static const char *choices[] = {
  "ident",
  "exit",
  "status",
  "stream",
  NULL
};
static ustream_command_f *command[] = {
  ustream_cmd_ident,
  ustream_cmd_exit,
  ustream_cmd_status,
  ustream_cmd_stream,
  NULL
};

/*..........................................................................
 *..........................................................................*/

dt_rc_e 
ustream_command_request(
  ustream_t *ustream,
  dtdgserver_t *dgserver,
  char *request)
{
  DT_F("ustream_command_request");
  dt_ctl_t *ctl = ustream->ctl;
  int n;
  int choice;
  char *path;
  char *arg;
  char message[256];
  dt_rc_e rc;
 
  DT_C(ustream_response_header,(		/* give html header 				*/
    ustream, dgserver,
    ustream->ident));

  dt_dbg(ctl, F, DT_DBG_MASK_COMMAND,	/* debug the request 				*/
    "%s", request);

  dtstr_tokenize(ctl, request, ' ', 	/* split input into tokens 			*/
    &n);

  if (n == 0)							/* no input tokens? 				*/
  {
    DT_C(ustream_response_error,(
      ustream, dgserver,
	  "Sorry, no command received."));
	goto cleanup;
  }

  if (strcmp(request, "GET"))			/* command is not GET? 				*/
  {
    DT_C(ustream_response_error,(
      ustream, dgserver,
	  "Sorry,"
      " expected GET but got \"%s\".", 
      request));
	goto cleanup;
  }

  if (n == 1)							/* no path after GET? 				*/
  {	
    DT_C(ustream_response_error,(
      ustream, dgserver,
	  "Sorry, nothing received"
      " after GET."));
  	goto cleanup;
  }

  path = request+strlen(request)+1;		/* skip the GET 					*/
  dtstr_tokenize(ctl, path, '?', &n);	/* split off args after the '?'		*/
  if (n == 1)							/* no question mark?				*/
    arg = "";							/* whole thing is the command 		*/
  else									/* there is a question mark? 		*/
    arg = path+strlen(path)+1;			/* arg is everything after 			*/

  DT_C(dtstr_choice2,(ctl,				/* look up function 				*/
    choices, path+1, &choice,
    message, sizeof(message)));
  if (choice >= 0)						/* valid choice? 					*/
  {
	DT_G((command[choice]),(			/* call appropriate function 		*/
      ustream, dgserver, path+1, arg));
	if (rc != DT_RC_STOP &&
		rc != DT_RC_SHUTDOWN)
	  rc = DT_RC_GOOD;
  }
  else									/* not a valid choice 				*/
  {
    DT_C(ustream_response_error,(
      ustream, dgserver,
      "%s", message));
	goto cleanup;
  }

  if (ustream->message[0])				/* got some error message? 			*/
    DT_I(ustream_response_error,(
      ustream, dgserver, 
      "%s", ustream->message));

cleanup:
  DT_I(ustream_response,(				/* give a blank line just for fun 	*/
    ustream, dgserver, ""));

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
