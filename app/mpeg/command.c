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

#include <umdemo.h>
U_R("app/umdemo $RCSfile: feed.c,v $ $Revision: 1.8 $");

/*..........................................................................*/

static
u_rc_e 
umdemo_command_source(
  umdemo_t *umdemo,
  const char *command)
{
  U_F("umdemo_command_source");
  u_ctl_t *ctl = umdemo->ctl;
  char *filename = strchr(command, ' ');
  if (UMDEMO_HAS_SOURCE(umdemo))		/* we already have a data source?	*/
  {
    U_Q(dtstr_printf,(ctl,
      umdemo->response,
      sizeof(umdemo->response),
      "already have a data source: %s",
      umdemo->fd.filename));
  }
  else
  if (filename == NULL ||				/* no filename given? 				*/
	  filename[0] == '\0' ||
	  filename[0] == ' ' &&
	  filename[1] == '\0')
  {
    U_Q(dtstr_printf,(ctl,
      umdemo->response,
      sizeof(umdemo->response),
      "%s", 
      "no filename given"));
  }
  else
  {
	filename++;
	U_Q(dtfd_open,(ctl, &umdemo->fd,	/* open mpeg file for input 		*/
      filename, "r"));
	umdemo->frame_last = 0;				/* set to display first frame */
	umdemo->frame_pause = 1;
  }

  return U_RC_GOOD;
}

/*..........................................................................*/

static
u_rc_e 
umdemo_command_forward(
  umdemo_t *umdemo,
  const char *command)
{
  U_F("umdemo_command_forward");
  u_ctl_t *ctl = umdemo->ctl;
  if (!UMDEMO_HAS_SOURCE(umdemo))		/* we have no data source?			*/
  {
    U_Q(dtstr_printf,(ctl,
      umdemo->response,
      sizeof(umdemo->response),
      "%s",
      "no data source"));
  }
  else
  if (umdemo->frame_pause > 0)			/* we are paused? 					*/
  {
    umdemo->frame_pause = 0;			/* set to free running 				*/
    U_Q(dtstr_printf,(ctl,
      umdemo->response,
      sizeof(umdemo->response),
      "forward"));
  }
  else									/* we are already going forward? 	*/
  {
    U_Q(dtstr_printf,(ctl,
      umdemo->response,
      sizeof(umdemo->response),
      "%s",
      "already going forward"));
  }

  return U_RC_GOOD;
}

/*..........................................................................*/

static
u_rc_e 
umdemo_command_pause(
  umdemo_t *umdemo,
  const char *command)
{
  U_F("umdemo_command_pause");
  u_ctl_t *ctl = umdemo->ctl;
  if (!UMDEMO_HAS_SOURCE(umdemo))		/* we have no data source?			*/
  {
    U_Q(dtstr_printf,(ctl,
      umdemo->response,
      sizeof(umdemo->response),
      "%s",
      "no data source"));
  }
  else
  {
    umdemo->frame_pause =				/* set new pause point 				*/
      umdemo->frame_last+1;
    U_Q(dtstr_printf,(ctl,
      umdemo->response,
      sizeof(umdemo->response),
      "paused"));
  }
  return U_RC_GOOD;
}

/*..........................................................................*/

static
u_rc_e 
umdemo_command_quit(
  umdemo_t *umdemo,
  const char *command)
{
  U_F("umdemo_command_quit");
  u_ctl_t *ctl = umdemo->ctl;
  if (!UMDEMO_HAS_SOURCE(umdemo))		/* we have no data source?			*/
  {
    U_Q(dtstr_printf,(ctl,
      umdemo->response,
      sizeof(umdemo->response),
      "%s",
      "no data source"));
  }
  else
  {
	if (UMDEMO_HAS_FD(umdemo))			/* we have a file open?				*/
	{
	  U_Q(dtfd_close,(ctl,				/* close it 						*/
        &umdemo->fd));
	}
	umdemo->stopping = 1;
  }
  return U_RC_GOOD;
}

/*..........................................................................*/

static
u_rc_e 
umdemo_command_exit(
  umdemo_t *umdemo,
  const char *command)
{
  umdemo->exiting = 1;
  return U_RC_GOOD;
}

/*..........................................................................*/

static
u_rc_e 
umdemo_command_status(
  umdemo_t *umdemo,
  const char *command)
{
  U_F("umdemo_command_status");
  u_ctl_t *ctl = umdemo->ctl;
  char s[256];
  
  U_Q(umdemo_status_string,(umdemo,		/* compose a status string 			*/
    umdemo->response,
    sizeof(umdemo->response)));

  return U_RC_GOOD;
}

/*..........................................................................*/

static const char *commands[] = {
  UMDEMO_COMMAND_SOURCE,
  UMDEMO_COMMAND_FORWARD,
  UMDEMO_COMMAND_PAUSE,
  UMDEMO_COMMAND_QUIT,
  UMDEMO_COMMAND_EXIT,
  UMDEMO_COMMAND_STATUS,
  NULL
};

static dt_key_t keys[] = {
  UMDEMO_KEY_SOURCE,
  UMDEMO_KEY_FORWARD,
  UMDEMO_KEY_PAUSE,
  UMDEMO_KEY_QUIT,
  UMDEMO_KEY_EXIT,
  UMDEMO_KEY_STATUS,
  DT_KEY_IGNORE
};

static umdemo_command_f *command_func[] = {
  umdemo_command_source,
  umdemo_command_forward,
  umdemo_command_pause,
  umdemo_command_quit,
  umdemo_command_exit,
  umdemo_command_status,
  NULL
};

/*..........................................................................*/

u_rc_e 
U_API
umdemo_command_index_from_string(
  u_ctl_t *ctl,
  const char *command,
  int *indx,
  char *response,
  int maxresponse)
{
  U_F("umdemo_command_index_from_string");
  char chop[128];
  char *p;
  char message[80];

  U_Q(dtstr_printf,(ctl,				/* local copy of input command 		*/
    chop, sizeof(chop),
    "%s", command));
  p = strchr(chop, ' ');				/* terminate command at space 		*/
  if (p != NULL)
    *p++ = '\0';
  
  U_Q(dtstr_choice2,(ctl, commands,
    chop, indx,
    message, sizeof(message)));
  
  return U_RC_GOOD;
}

/*..........................................................................*/

u_rc_e 
U_API
umdemo_command_index_from_key(
  u_ctl_t *ctl,
  char key,
  int *indx,
  char *response,
  int maxresponse)
{
  U_F("umdemo_command_index_from_key");
  int i;
  for (i=0;								/* search key list 					*/
	   keys[i] != DT_KEY_IGNORE &&
	   keys[i] != key;
	   i++);
  
  if (keys[i] == DT_KEY_IGNORE)			/* key not in list? 				*/
  {
	char s[16];
	char c = key;
	dtstr_printable(ctl, &c, 1,
      s, sizeof(s));
	dtstr_printf(ctl,
      response,
      maxresponse,
      "Sorry, key %s is unassigned.",
      s);
  }
  else
  {
	*indx = i;
	response[0] = '\0';
  }

  return U_RC_GOOD;
}

/*..........................................................................*/

u_rc_e 
U_API
umdemo_command_string_from_key(
  u_ctl_t *ctl,
  char key,
  const char **command,
  char *response,
  int maxresponse)
{
  U_F("umdemo_command_string_from_key");
  int indx;
  U_Q(umdemo_command_index_from_key,(	/* get key's index 					*/
    ctl, key, &indx,
    response, maxresponse));
  if (response[0] == '\0')				/* key was good? 					*/
    *command = commands[indx];
  else
	*command = NULL;
  return U_RC_GOOD;
}

/*..........................................................................*/

u_rc_e 
U_API
umdemo_command_func_from_key(
  u_ctl_t *ctl,
  char key,
  umdemo_command_f **function,
  char *response,
  int maxresponse)
{
  U_F("umdemo_command_func_from_key");
  int indx;
  U_Q(umdemo_command_index_from_key,(	/* get key's index 					*/
    ctl, key, &indx,
    response, maxresponse));
  if (response[0] == '\0')				/* key was good? 					*/
	*function = command_func[indx];
  else
	*function = NULL;
  return U_RC_GOOD;
}

/*..........................................................................*/

u_rc_e 
U_API
umdemo_command_prompt(
  u_ctl_t *ctl,
  char *prompt,
  int max)
{
  U_F("umdemo_command_prompt");
  U_Q(dtstr_printf,(ctl,
    prompt, max,
    "%c=%s %c=%s %c=%s",
    UMDEMO_KEY_FORWARD,
    UMDEMO_COMMAND_FORWARD,
    UMDEMO_KEY_PAUSE,
    UMDEMO_COMMAND_PAUSE,
    UMDEMO_KEY_QUIT,
    UMDEMO_COMMAND_QUIT));
  return U_RC_GOOD;
}

/*..........................................................................*/

u_rc_e 
U_API
umdemo_command(
  umdemo_t *umdemo,
  char *command)
{
  U_F("umdemo_command");
  u_ctl_t *ctl = umdemo->ctl;
  int indx;
  u_rc_e rc = U_RC_GOOD;;

  u_dbg(ctl, F, DT_DBG_MASK_USR2,
    "got command \"%s\"",
    command);

  umdemo->response[0] = '\0';			/* clear response field 			*/
  U_Q(umdemo_command_index_from_string,(
    ctl, command, &indx,
    umdemo->response,
    sizeof(umdemo->response)));

  if (umdemo->response[0] == '\0')		/* command was good? 				*/
  {
	U_G((*command_func[indx]),(umdemo,	/* call the designated command 		*/
      command));
	if (umdemo->response[0] == '\0')	/* no response yet? 				*/
	{
	  dtstr_printf(ctl,					/* default response 				*/
        umdemo->response,
        sizeof(umdemo->response),
        "%s", "ok");
	}
	if (rc == U_RC_BAD)					/* don't let these errors kill us 	*/
	  rc = U_RC_GOOD;
  }

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
