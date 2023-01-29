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
| :dtistream: Function Group - input stream object
|
| SUMMARY
| The ~dtistream~ object gives a process the ability to act
| as a server for receiving streaming data.
| 
| Each instantiation of this object represents one streaming conversation.
|
| A companion object called ~dtostream~ provides the ability for a process
| to act as a data-providing client.
|
| In a streaming conversation, the data receiver process is the server.
| 
| From the server's point of view, the conversation goes like this:
| 1.	call dtistream_init() to instantiate the object
| 2.	optionally call dtistream_is_available() to poll for incoming packet
| 3.	repeatedly call dtistream_recv() to get incoming packets
| 4.	call dtistream_uninit() to release the object
| 
| END
 *..........................................................................*/

#include <dtack/base.h>
DT_RCSID("app/ustream $RCSfile: istream.c,v $ $Revision: 1.8 $");

#include <dtstream.h>
#include <dtistream.h>
#include <dtack/str.h>
#include <dtack/os.h>
  
static const char *choices[] = {
  "ipc",
  "file",
  "audio",
  NULL
};

static dtistream_init_f *methods[] = {
  dtistream_ipc_init,
  dtistream_file_init,
  dtistream_audio_init,
  NULL
};

/*..........................................................................
| NAME
| dtistream_init() - initialize streaming receiver
|
| SUMMARY
| Initialize the streaming server object ~istream~.
| Expect ~spec~ to the of the form #scheme,carrier#.
| Have an error if no client connects within ~timeout~ milliseconds.
| Have an error if #scheme# is not one of a predefined set.
| Have an error if there is already a server at #carrier#.
|
| This function does not block.
| 
| END
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtistream_init(							/* instantiate dtistream object 	*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtistream_t *istream,					/* this dtistream object 			*/
  const char *spec,						/* the server spec 					*/
  dtstream_id_t *id,					/* our ID 							*/
  dtstream_id_t *permit,				/* permitted client ID				*/
  long timeout)							/* maximum wait for client 			*/
{
  DT_F("dtistream_init");
  char *info;
  int choice;
  dt_rc_e rc;

  memset(istream, 0,					/* clear the structure 				*/
    sizeof(*istream));

  if (spec == NULL)
    spec = "";

  DT_Q(dtos_strdup,(ctl, spec,
    &istream->scheme));

  info = strchr(istream->scheme, ',');	/* info part after scheme	    	*/
  if (info)
    *info++ = '\0';
  else
    info = "";

  DT_C(dtstr_choice,(ctl, choices,		/* look up istream->scheme	    	*/
    istream->scheme, &choice));

  if (choice == -1)						/* unknown type?				    */
  {
    rc = dt_err(ctl, F,
      "unknown istream scheme \"%s\"",
      istream->scheme);
	goto cleanup;
  }
  else
  if (choice == -2)						/* ambiguous type?				    */
  {
    rc = dt_err(ctl, F,
      "ambiguous istream scheme \"%s\"",
      istream->scheme);
	goto cleanup;
  }

  DT_C((methods[choice]),(ctl,			/* give info to implementation		*/
    istream, info, id, permit,
    timeout))

  istream->initialized = 1;
cleanup:
  if (rc != DT_RC_GOOD)
  {
	if (istream->scheme != NULL)
      DT_I(dtos_free2,(ctl,
        istream->scheme,
        F, "scheme"));
	memset(istream, 0, sizeof(*istream));
  }
  return rc;
}

/*..........................................................................
| NAME
| dtistream_is_available() - indicate data available
|
| SUMMARY
| Return ~is_available~ as non-zero if a client has sent data.
| 
| END
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtistream_is_available(					/* check for client data pending 	*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtistream_t *istream,					/* this dtistream object 			*/
  int *is_available)					/* returned availability flag 		*/
{
  DT_F("dtistream_is_available");
  if (istream->is_available != NULL)
    DT_Q((*istream->is_available),(
      ctl, istream, is_available))
  else
    return dt_err(ctl, F,
      "function not implemented"
      " for istream scheme %s",
      istream->scheme);
  return DT_RC_GOOD;
}

/*..........................................................................
| NAME
| dtistream_recv() - receive incoming packet
|
| SUMMARY
| Allocate space for the incoming data at ~*packet~
| and place the data there.
| Place the length of this data at ~*packet_length~.
| Rely on the caller to free this space
| If no data is currently available, then block.
| 
| END
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtistream_recv(							/* get incoming request 			*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtistream_t *istream,					/* this dtistream object 			*/
  void **packet,						/* returned data packet 			*/
  long *packet_length)					/* returned packet length 			*/
{
  DT_F("dtistream_recv");
  if (istream->recv != NULL)
    DT_Q((*istream->recv),(
      ctl, istream, 
      packet, packet_length))
  else
    return dt_err(ctl, F,
      "function not implemented"
      " for istream scheme %s",
      istream->scheme);
  return DT_RC_GOOD;
}

/*..........................................................................
| NAME
| dtistream_uninit() - terminate conversation
|
| SUMMARY
| Terminate server actions.
| Accept no further data packets.
| Cause the data-provider partner to have an error the next time
| it tries to send.
| 
| END
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtistream_uninit(						/* release resources 				*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtistream_t *istream)					/* this dtistream object 			*/
{
  DT_F("dtistream_uninit");
  dt_rc_e rc = DT_RC_GOOD;
  if (istream->initialized)
  {
  	if (istream->uninit != NULL)
      DT_Q((*istream->uninit),(
        ctl, istream));
	if (istream->scheme != NULL)
	  DT_I(dtos_free2,(ctl,
        istream->scheme,
        F, "scheme"));
  }
  memset(istream, 0,					/* clear the data structure 		*/
    sizeof(*istream));
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
