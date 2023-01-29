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
U_R("app/umplay $RCSfile: feed.c,v $ $Revision: 1.8 $");

/*..........................................................................*/

static
u_rc_e 
umplay_command_source(
  umplay_t *umplay,
  const char *command)
{
  U_F("umplay_command_source");
  u_ctl_t *ctl = umplay->ctl;
  char *filename = strchr(command, ' ');
  if (UMPLAY_HAS_SOURCE(umplay))		/* we already have a data source?	*/
  {
    U_Q(dtstr_printf,(ctl,
      umplay->response,
      sizeof(umplay->response),
      "already have a data source: %s",
      umplay->fd.filename));
  }
  else
  if (filename == NULL ||				/* no filename given? 				*/
	  filename[0] == '\0' ||
	  filename[0] == ' ' &&
	  filename[1] == '\0')
  {
    U_Q(dtstr_printf,(ctl,
      umplay->response,
      sizeof(umplay->response),
      "%s", 
      "no filename given"));
  }
  else
  {
	filename++;
	U_Q(dtfd_open,(ctl, &umplay->fd,	/* open mpeg file for input 		*/
      filename, "r"));
  }

  return U_RC_GOOD;
}

/*..........................................................................*/

static
u_rc_e 
umplay_command_forward(
  umplay_t *umplay,
  const char *command)
{
  U_F("umplay_command_forward");
  u_ctl_t *ctl = umplay->ctl;
  umdec_mode_e mode;
  
  if (!strcmp(command,					/* this is fast forward? 			*/
	     UMPLAY_COMMAND_FFORWARD))
	mode = UMDEC_MODE_FFORWARD;
  else
  if (!strcmp(command,					/* this is slow forward? 			*/
	    UMPLAY_COMMAND_SFORWARD))
    mode = UMDEC_MODE_SFORWARD;
  else									/* must be normal forward 			*/
	mode = UMDEC_MODE_FORWARD;
 
  if (!UMPLAY_HAS_SOURCE(umplay))		/* we have no data source?			*/
  {
    U_Q(dtstr_printf,(ctl,
      umplay->response,
      sizeof(umplay->response),
      "%s",
      "no data source"));
  }
  else
  if (mode != umplay->control.mode)		/* or in a different mode? 			*/
  {
	umplay->control.mode = mode;		/* set hardware mode later 			*/
    U_Q(dtstr_printf,(ctl,
      umplay->response,
      sizeof(umplay->response),
      "ok %s", command));
  }
  else									/* we are already going forward? 	*/
  {
    U_Q(dtstr_printf,(ctl,
      umplay->response,
      sizeof(umplay->response),
      "already going %s", command));
  }

  return U_RC_GOOD;
}

/*..........................................................................*/

static
u_rc_e 
umplay_command_1forward(
  umplay_t *umplay,
  const char *command)
{
  U_F("umplay_command_1forward");
  u_ctl_t *ctl = umplay->ctl;
  umdec_mode_e mode;

  if (!UMPLAY_HAS_SOURCE(umplay))		/* we have no data source?			*/
  {
    U_Q(dtstr_printf,(ctl,
      umplay->response,
      sizeof(umplay->response),
      "%s",
      "no data source"));
    U_Q(dtstr_printf,(ctl,
      umplay->response,
      sizeof(umplay->response),
      "ok %s", command));
  }

  umplay->control.mode =				/* set hardware mode later 			*/
    UMDEC_MODE_1FORWARD;

  return U_RC_GOOD;
}

/*..........................................................................*/

static
u_rc_e 
umplay_command_pause(
  umplay_t *umplay,
  const char *command)
{
  U_F("umplay_command_pause");
  u_ctl_t *ctl = umplay->ctl;
  if (!UMPLAY_HAS_SOURCE(umplay))		/* we have no data source?			*/
  {
    U_Q(dtstr_printf,(ctl,
      umplay->response,
      sizeof(umplay->response),
      "%s",
      "no data source"));
  }
  else
  {
	umplay->control.mode =				/* set hardware mode later 			*/
      UMDEC_MODE_PAUSE;
    U_Q(dtstr_printf,(ctl,
      umplay->response,
      sizeof(umplay->response),
      "ok paused"));
  }
  return U_RC_GOOD;
}

/*..........................................................................*/

static
u_rc_e 
umplay_command_quit(
  umplay_t *umplay,
  const char *command)
{
  U_F("umplay_command_quit");
  u_ctl_t *ctl = umplay->ctl;
  if (!UMPLAY_HAS_SOURCE(umplay))		/* we have no data source?			*/
  {
    U_Q(dtstr_printf,(ctl,
      umplay->response,
      sizeof(umplay->response),
      "%s",
      "no data source"));
  }
  else
  {
	if (UMPLAY_HAS_FD(umplay))			/* we have a file open?				*/
	{
	  U_Q(dtfd_close,(ctl,				/* close it 						*/
        &umplay->fd));
	}
	umplay->control.mode =
	  UMDEC_MODE_STOP;
  }
  return U_RC_GOOD;
}

/*..........................................................................*/

static
u_rc_e 
umplay_command_exit(
  umplay_t *umplay,
  const char *command)
{
  U_F("umplay_command_exit");
  u_ctl_t *ctl = umplay->ctl;
  if (UMPLAY_HAS_FD(umplay))			/* we have a file open?				*/
  {
	U_Q(dtfd_close,(ctl,				/* close it 						*/
      &umplay->fd));
  }
  
  umplay->control.mode =
	UMDEC_MODE_STOP;

  umplay->exiting = 1;
  return U_RC_GOOD;
}

/*..........................................................................*/

static
u_rc_e 
umplay_command_status(
  umplay_t *umplay,
  const char *command)
{
  U_F("umplay_command_status");
  u_ctl_t *ctl = umplay->ctl;
  char s[256];
  
  U_Q(umplay_status_string,(umplay,		/* compose a status string 			*/
    umplay->response,
    sizeof(umplay->response)));

  return U_RC_GOOD;
}

/*..........................................................................*/

static const char *commands[] = {
  UMPLAY_COMMAND_SOURCE,
  UMPLAY_COMMAND_FORWARD,
  UMPLAY_COMMAND_FFORWARD,
  UMPLAY_COMMAND_SFORWARD,
  UMPLAY_COMMAND_1FORWARD,
  UMPLAY_COMMAND_PAUSE,
  UMPLAY_COMMAND_QUIT,
  UMPLAY_COMMAND_EXIT,
  UMPLAY_COMMAND_STATUS,
  NULL
};

static dt_key_t keys[] = {
  UMPLAY_KEY_SOURCE,
  UMPLAY_KEY_FORWARD,
  UMPLAY_KEY_FFORWARD,
  UMPLAY_KEY_SFORWARD,
  UMPLAY_KEY_1FORWARD,
  UMPLAY_KEY_PAUSE,
  UMPLAY_KEY_QUIT,
  UMPLAY_KEY_EXIT,
  UMPLAY_KEY_STATUS,
  DT_KEY_IGNORE
};

static umplay_command_f *command_func[] = {
  umplay_command_source,
  umplay_command_forward,
  umplay_command_forward,
  umplay_command_forward,
  umplay_command_1forward,
  umplay_command_pause,
  umplay_command_quit,
  umplay_command_exit,
  umplay_command_status,
  NULL
};

/*..........................................................................*/

u_rc_e 
U_API
umplay_command_index_from_string(
  u_ctl_t *ctl,
  const char *command,
  int *indx,
  char *response,
  int maxresponse)
{
  U_F("umplay_command_index_from_string");
  char chop[128];
  char *p;

  U_Q(dtstr_printf,(ctl,				/* local copy of input command 		*/
    chop, sizeof(chop),
    "%s", command));
  p = strchr(chop, ' ');				/* terminate command at space 		*/
  if (p != NULL)
    *p++ = '\0';
  
  U_Q(dtstr_choice2,(ctl, commands,
    chop, indx,
    response, maxresponse));
  
  return U_RC_GOOD;
}

/*..........................................................................*/

u_rc_e 
U_API
umplay_command_index_from_key(
  u_ctl_t *ctl,
  char key,
  int *indx,
  char *response,
  int maxresponse)
{
  U_F("umplay_command_index_from_key");
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
umplay_command_string_from_key(
  u_ctl_t *ctl,
  char key,
  const char **command,
  char *response,
  int maxresponse)
{
  U_F("umplay_command_string_from_key");
  int indx;
  U_Q(umplay_command_index_from_key,(	/* get key's index 					*/
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
umplay_command_func_from_key(
  u_ctl_t *ctl,
  char key,
  umplay_command_f **function,
  char *response,
  int maxresponse)
{
  U_F("umplay_command_func_from_key");
  int indx;
  U_Q(umplay_command_index_from_key,(	/* get key's index 					*/
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
umplay_command_prompt(
  u_ctl_t *ctl,
  char *prompt,
  int max)
{
  U_F("umplay_command_prompt");
  U_Q(dtstr_printf,(ctl,
    prompt, max,
    "%c=%s %c=%s %c=%s",
    UMPLAY_KEY_FORWARD,
    UMPLAY_COMMAND_FORWARD,
    UMPLAY_KEY_PAUSE,
    UMPLAY_COMMAND_PAUSE,
    UMPLAY_KEY_QUIT,
    UMPLAY_COMMAND_QUIT));
  return U_RC_GOOD;
}

/*..........................................................................*/

u_rc_e 
U_API
umplay_command(
  umplay_t *umplay,
  char *command)
{
  U_F("umplay_command");
  u_ctl_t *ctl = umplay->ctl;
  int indx;
  u_rc_e rc = U_RC_GOOD;;

  u_dbg(ctl, F, DT_DBG_MASK_USR2,
    "interpreting command \"%s\"",
    command);

  umplay->response[0] = '\0';			/* clear response field 			*/
  U_Q(umplay_command_index_from_string,(
    ctl, command, &indx,
    umplay->response,
    sizeof(umplay->response)));

  if (umplay->response[0] == '\0')		/* command was good? 				*/
  {
	U_G((*command_func[indx]),(umplay,	/* call the designated command 		*/
      command));
	if (umplay->response[0] == '\0')	/* no response yet? 				*/
	{
	  dtstr_printf(ctl,					/* default response 				*/
        umplay->response,
        sizeof(umplay->response),
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
