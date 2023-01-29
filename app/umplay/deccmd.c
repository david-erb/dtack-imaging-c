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
U_R("app/umplay $RCSfile: deccmd.c,v $ $Revision: 1.8 $");
#include <dtack/dg.h>

#ifndef DTDEPEND
#include <signal.h>
#endif

#define COMMAND(CMD, VALUE, SPEED)		\
{										\
  umplay_deccmd_t deccmd;				\
  memset(&deccmd, 0, sizeof(deccmd));	/* clear command structure 			*/\
  deccmd.cmd = DECCMD_ ## CMD;			/* prepare command					*/\
  deccmd.u.playSpeed = (SPEED);			/* always give play speed			*/\
  if (deccmd.cmd != DECCMD_NOP)			\
    dt_dbg(ctl, F, DT_DBG_MASK_COMMAND,	\
      "stuffing %s (%d) %s %s",			\
      #CMD, DECCMD_ ## CMD, 			\
     strlen(VALUE)? VALUE: "",			\
     strlen(VALUE)? #SPEED: "");		\
  else									\
    nop_count++;						\
  U_C(dtfd_write_all,(ctl,				/* write command into fifo 			*/\
    &command_fifo, 						\
    (void **)&deccmd, sizeof(deccmd)));	\
  fflush(command_fifo.file);			\
}

static void sigpipe(int signum) { }

/*..........................................................................
 *..........................................................................*/

static
dt_rc_e 
umplay_deccmd_request(					/* make request of decdata server	*/
  umplay_t *umplay,
  const char *spec,						/* spec of decdata process 			*/
  const char *request)					/* the given request string 		*/
{
  DT_F("umplay_deccmd_request");
  dt_ctl_t *ctl = umplay->ctl;
  dtdg_client_t _dgclient, *dgclient = &_dgclient;
  int is_available;
  const char *error;
  dt_rc_e rc;

  if (spec == NULL)
    return dt_err(ctl, F,
      "no IPC spec given");

  DT_C(dtdg_client_init,(ctl,			/* send request to server 			*/
    dgclient, spec,
    request, 1000));

  DT_C(dtdg_client_wait_header,(ctl,	/* wait for header to appear		*/
    dgclient, 2000,
    &is_available));
  if (!is_available)
  {
	rc = dt_err(ctl, F,
      "decdata server %s not responding",
      spec);
	goto cleanup;
  }

  DT_C(dtdg_client_get_header,(ctl,		/* pick off the header 				*/
    dgclient));

  DT_C(dtdg_client_get_response,(ctl,	/* pick off the response			*/
    dgclient, 2000));

  DT_C(dtdg_client_response_search,(	/* see if response has error		*/
    ctl, dgclient, DTDG_RESPONSE_ERROR,
    &error));
  
  if (error != NULL)					/* there is a server error? 		*/
  {
	rc = dt_err(ctl, F,
      DTDG_RESPONSE_ERROR ": %s",
      error);
	goto cleanup;
  }

cleanup:
  DT_I(dtdg_client_uninit,(ctl,
    dgclient));
  return rc;
}

/* ------------------------------------------------------------------------
 * ------------------------------------------------------------------------ */

u_rc_e
U_API
umplay_op_deccmd(
  dtparam_t *param)
{
  U_F("umplay_op_deccmd");
  u_ctl_t *ctl = &param->ctl;
  umplay_t *umplay = NULL;
  void (*sigpipe_previous)(int) = NULL;
  dtfd_t command_fifo;
  dtfd_t reply_fifo;
  dtlist_elem_t *elem;
  long limit = 0;
  long nop_count = 0;
  u_rc_e rc;
  
  U_C(umplay_init,(param, &umplay));	/* init umplay structure			*/

  command_fifo.fd = -1;
  reply_fifo.fd = -1;

  sigpipe_previous =					/* server exit may nuke our pipes 	*/
    signal(SIGPIPE, sigpipe);

  U_C(dtfd_open,(ctl, &command_fifo,	/* open command fifo				*/
    umplay->command_fifo_name, "w"));

  U_C(dtfd_open,(ctl, &reply_fifo,		/* open reply fifo					*/
    umplay->reply_fifo_name, "r"));
  
  COMMAND(INITPARAM, "speed", 0);		/* send init command 				*/

  elem = DTLIST_ELEM_HEAD;				/* start reading at head of list 	*/
  while(1)
  {
	int isnumber;
	long waited = 0;
	
	while (waited < limit)
	{
	  DT_C(dtos_delay,(ctl, 80));		/* wait poll period 				*/
	  COMMAND(NOP, "", 0)				/* send nop command 				*/
	  waited += 80;
	} 

	DT_C(dtlist_next,(ctl,				/* get next command entry 			*/
      &umplay->config_list,
      elem, &elem));
	if (elem == NULL)					/* command list exhausted? 			*/
	  break;
	DT_C(dtstr_to_long2,(ctl,			/* expect data to be a number 		*/
      (char *)elem->user, &limit,
      &isnumber));
	if (!isnumber)
	{
	  rc = dt_err(ctl, F,
          "invalid number \"%s\" in %s",
          elem->user, umplay->config_filename);
	  goto cleanup;
	}
  
#   define IS_CMD(CMD) \
      (!strcmp(elem->name, CMD))
  
    if (IS_CMD("forward"))
	  COMMAND(PLAY, "speed", 0)			/* send play command 				*/
	else
	if (IS_CMD("fforward"))
	  COMMAND(PLAY, "speed", 2)			/* send fast forward command		*/
	else
	if (IS_CMD("sforward"))
	  COMMAND(PLAY, "speed", -2)		/* send slow motion command 		*/
	else
	if (IS_CMD("1forward"))
	  COMMAND(STEPFRAME, "", 0)			/* send single-step command			*/
	else
	if (IS_CMD("nop"))
	  COMMAND(NOP, "", 0)				/* send nop command					*/
	else
	if (IS_CMD("starve"))
	{
	  char t[80];
	  DT_C(dtstr_printf,(ctl, 
        t, sizeof(t),
        "GET /delay?%ld", 
        limit));
	  dt_dbg(ctl, F, DT_DBG_MASK_COMMAND,
        "starving duration %d",	
        limit);
	  DT_C(umplay_deccmd_request,(		/* tell decdata to starve			*/
        umplay, param->connect, t));
	  limit = 0;
	}
	else
	if (IS_CMD("delay"))
	{
	  dt_dbg(ctl, F, DT_DBG_MASK_COMMAND,
        "delaying duration %d",	
        limit);
	  U_C(dtos_delay,(ctl,				/* wait for a bit 					*/
        limit));
	  dt_dbg(ctl, F, DT_DBG_MASK_COMMAND,
        "done delaying duration %d",	
        limit);
	  limit = 0;
	}
	else
	if (IS_CMD("pause"))
	  COMMAND(PAUSE, "", 0)				/* send pause command 				*/
	else
	if (IS_CMD("stop"))
	{
	  DT_C(umplay_deccmd_request,(		/* tell decdata to stop				*/
        umplay, param->connect,
        "GET /exit"));
	  break;
	}
	else
	if (IS_CMD("exit"))
	{
	  COMMAND(EXIT, "", 0);				/* tell server to stop				*/
	  break;
	}
	else
	{
	  rc = dt_err(ctl, F,
        "invalid command \"%s\" in %s",
        elem->name, 
        umplay->config_filename);
	  goto cleanup;
	}
  }

  dt_dbg(ctl, F, DT_DBG_MASK_COMMAND,
    "sent %ld NOPs",
    nop_count);

cleanup:
  if (reply_fifo.fd != -1)
    U_I(dtfd_close,(ctl, &reply_fifo));
  if (command_fifo.fd != -1)
    U_I(dtfd_close,(ctl, &command_fifo));

  if (sigpipe_previous != NULL)
    signal(SIGPIPE, sigpipe_previous);

  if (umplay != NULL)
    U_I(umplay_uninit,(param, umplay));
  
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
