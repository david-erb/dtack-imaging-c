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
U_R("app/umplay $RCSfile: decdata.c,v $ $Revision: 1.8 $");
#include <dtack/dg.h>

#ifndef DTDEPEND
#include <signal.h>
#endif

static void sigpipe(int signum) { }

#define BUFSIZE (4000)
static dtdg_server_request_f umplay_decdata_delay;

static const char *requests[] = {
  "delay",
  "exit",
  NULL
};
static dtdg_server_request_f *funcs[] = {
  umplay_decdata_delay,
  dtdg_server_exit,
  NULL
};
  
/*..........................................................................
 *..........................................................................*/

static
dt_rc_e
DTCONFIG_API1
umplay_decdata_delay(
  dt_ctl_t *ctl,
  dtdg_server_t *dgserver,
  void *arg,
  const char *command,
  char *rest,
  char *message,
  int message_max)
{
  DT_F("umplay_decdata_delay");
  umplay_t *umplay = (umplay_t *)arg;
  long milliseconds;
  int isnumber;

  if (dgserver != NULL)
    return dt_err(ctl, F,
      "no dgserver IPC spec given");

	DT_Q(dtstr_to_long2,(ctl, rest,
      &milliseconds, &isnumber));
	if (isnumber)
	{
	  umplay->delay = milliseconds;
	  DT_Q(dtdg_server_response,(ctl, dgserver,
        DTDG_RESPONSE_RC
        ": %d", DT_RC_GOOD));
	  DT_Q(dtdg_server_response,(ctl, dgserver,
        DTDG_RESPONSE_RESULT
        ": delaying %ld", milliseconds));
	}
	else
	{
	  DT_Q(dtdg_server_response,(ctl, dgserver,
        DTDG_RESPONSE_RC
        ": %d", DT_RC_BAD));
	  DT_Q(dtdg_server_response,(ctl, dgserver,
        DTDG_RESPONSE_RESULT
        ": \"%s\" is not a number", rest));
	}
 
  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/

static
dt_rc_e 
umplay_decdata_transact(
  umplay_t *umplay,
  dtdg_server_t *dgserver)
{
  DT_F("umplay_decdata_transact");
  dt_ctl_t *ctl = umplay->ctl;
  char *request = NULL;
  char message[256];
  dt_rc_e rc;

  if (dgserver == NULL)
    return dt_err(ctl, F,
      "no dgserver IPC spec given");

  DT_C(dtdg_server_get_request,(		/* receive the datagram request		*/
    ctl, dgserver, &request));

  if (request == NULL)					/* no request available? 			*/
    goto cleanup;

  DT_C(dtdg_server_header_plain,(ctl,	/* give html header 				*/
    dgserver, "umplay -op decdata"));

  DT_C(dtdg_server_request,(ctl,		/* generate datagram response		*/
    dgserver, request,
    (void *)umplay,
    requests, funcs,
    message, 
    sizeof(message)))

  if (message[0] != '\0')				/* error handling request?			*/
  {
    DT_C(dtdg_server_response,(ctl,		/* tack on end of request 			*/
      dgserver, 
      DTDG_RESPONSE_ERROR
      ": %s", message));
  }

  DT_C(dtdg_server_trailer_plain,(ctl,	/* give html trailer 				*/
    dgserver));

  DT_C(dtdg_server_done,(ctl,			/* finish response to the datagram	*/
    dgserver));

cleanup:
  if (request != NULL)
	DT_I(dtos_free2,(ctl, request,		/* free the datagram command 		*/
      F, "request"));
  return rc;
}

/*..........................................................................
 *..........................................................................*/

static
dt_rc_e 
umplay_decdata_check(
  umplay_t *umplay,
  dtdg_server_t *dgserver)
{
  DT_F("umplay_decdata_check");
  dt_ctl_t *ctl = umplay->ctl;
  int is_request;
  dt_rc_e rc;

  if (dgserver == NULL)
    return DT_RC_GOOD;

  while (1)								/* while commands are available 	*/
  {
	DT_C(dtdg_server_is_request,(ctl,	/* check state of command input 	*/
      dgserver, &is_request));
	if (!is_request)					/* there is work for us? 			*/
	  break;
	DT_C(umplay_decdata_transact,(		/* handle the command 				*/
      umplay, dgserver));
	if (umplay->delay != 0)				/* delay is called for? 			*/
	{
	  dt_dbg(ctl, F, DT_DBG_MASK_COMMAND,
        "delaying %ld", umplay->delay);
	  DT_C(dtos_delay,(ctl, 
        umplay->delay));
	  dt_dbg(ctl, F, DT_DBG_MASK_COMMAND,
        "done delaying %ld", umplay->delay);
	  umplay->delay = 0;
	}
  }

cleanup:
  return rc;
}

/* ------------------------------------------------------------------------
 * ------------------------------------------------------------------------ */

u_rc_e
U_API
umplay_op_decdata(
  dtparam_t *param)
{
  U_F("umplay_op_decdata");
  u_ctl_t *ctl = &param->ctl;
  umplay_t *umplay = NULL;
  dtdg_server_t _dgserver, *dgserver = &_dgserver;
  dtfd_t mpeg_file;
  void (*sigpipe_previous)(int) = NULL;
  dtfd_t data_fifo;
  void *buf = NULL;
  u_rc_e rc;

  mpeg_file.fd = -1;
  data_fifo.fd = -1;

  U_C(umplay_init,(param, &umplay));	/* init umplay structure			*/

  if (param->connect == NULL)
    dgserver = NULL;
  else
    DT_C(dtdg_server_init,(ctl, dgserver, /* become a datagram server 		*/
      param->connect));

  U_C(dtfd_open,(ctl, &mpeg_file,		/* open mpeg file for input 		*/
    param->input, "r"));
  
  {
	unsigned long decade;
    U_C(dtfd_read_all,(ctl,				/* check for nci file signature 	*/
      &mpeg_file,
      (void **)&decade, 
      sizeof(decade)));
	U_C(dtfd_seek,(ctl,					/* seek back to start of file 		*/
      &mpeg_file, 0,
      DTFD_WHENCE_SET, NULL));
	if (decade == 0xdecade00 ||			/* nci file signature found? 		*/
		decade == 0x00decade)
  	  U_C(dtfd_seek,(ctl,				/* seek test file signature			*/
        &mpeg_file, 400,
        DTFD_WHENCE_SET, NULL));
  }

  sigpipe_previous =					/* server exit may nuke our pipes 	*/
    signal(SIGPIPE, sigpipe);

  U_C(dtfd_open,(ctl, &data_fifo,		/* open data fifo					*/
    umplay->data_fifo_name, "w"));

  U_C(dtos_malloc2,(ctl, &buf,			/* allocate buffer 					*/
    BUFSIZE, F, "buf"));

  while(1)								/* data shoveling loop 				*/
  {
	long got, written;
	if (dgserver != NULL)
  	  DT_C(umplay_decdata_check,(	/* handle datagram commands			*/
        umplay, dgserver));

	U_C(dtfd_read,(ctl, &mpeg_file,		/* read data from mpeg file 		*/
      buf, BUFSIZE, &got));
	if (got == 0)						/* end of input file reached? 		*/
	  break;
	U_C(dtfd_write,(ctl,				/* write data into data fifo		*/
      &data_fifo, buf, got, &written));
	if (written < got)					/* go until fifo dries up 			*/
	  break;
  }
  
cleanup:
  if (buf != NULL)
    U_I(dtos_free2,(ctl, buf,
      F, "buf"));
  if (data_fifo.fd != -1)
    U_I(dtfd_close,(ctl, &data_fifo));

  if (sigpipe_previous != NULL)
    signal(SIGPIPE, sigpipe_previous);

  if (mpeg_file.fd != -1)
    U_I(dtfd_close,(ctl, &mpeg_file));

  if (dgserver != NULL)
    DT_I(dtdg_server_uninit,(ctl,		/* quit being a datagram server 	*/
      dgserver));

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
