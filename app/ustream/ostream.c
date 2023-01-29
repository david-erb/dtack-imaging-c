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
| :dtostream: Function Group - output stream object
|
| SUMMARY
| The ~dtostream~ object gives a process the ability to act
| as a client for sending streaming data.
| 
| Each instantiation of this object represents one streaming conversation.
|
| A companion object called ~dtistream~ provides the ability for a process
| to act as a data-receiving server.
|
| In a streaming conversation, the data sending process is the client.
| 
| From the client's point of view, the conversation goes like this:
| 1.	call dtostream_init() to instantiate the object
| 3.	repeatedly call dtostream_send() to send outgoing packets
| 4.	call dtostream_uninit() to release the object
| 
| END
 *..........................................................................*/

#include <dtack/base.h>
DT_RCSID("app/ustream $RCSfile: ostream.c,v $ $Revision: 1.8 $");

#include <dtstream.h>
#include <dtostream.h>
#include <dtack/str.h>
#include <dtack/os.h>
    
static const char *choices[] = {
  "ipc",
  "file",
  "audio",
  NULL
};

static dtostream_init_f *methods[] = {
  dtostream_ipc_init,
  dtostream_file_init,
  dtostream_audio_init,
  NULL
};

/*..........................................................................
| NAME
| dtostream_init() - initialize streaming client object
|
| SUMMARY
| Initialize the streaming client object ~ostream~.
| Expect ~spec~ to the of the form #scheme,carrier#.
| Have an error if connection cannot be made within ~timeout~ milliseconds.
| Have an error if #scheme# is not one of a predefined set.
| Have an error if there is no server at #carrier# or the server
| refuses to talk.
| 
| END
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtostream_init(							/* instantiate dtostream object 	*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtostream_t *ostream,					/* this dtostream object 			*/
  const char *spec,						/* the server spec 					*/
  dtstream_id_t *id,					/* our ID 							*/
  dtstream_id_t *permit,				/* permitted server ID				*/
  long timeout)							/* time to wait to contact server 	*/
{
  DT_F("dtostream_init");
  char *info;
  int choice;
  dt_rc_e rc;

  memset(ostream, 0,					/* clear the structure 				*/
    sizeof(*ostream));

  if (spec == NULL)
    spec = "";

  DT_Q(dtos_strdup,(ctl, spec,
    &ostream->scheme));

  info = strchr(ostream->scheme, ',');	/* info part after scheme	    	*/
  if (info)
    *info++ = '\0';
  else
    info = "";

  DT_C(dtstr_choice,(ctl, choices,		/* look up ostream->scheme	    	*/
    ostream->scheme, &choice));

  if (choice == -1)						/* unknown type?				    */
  {
    rc = dt_err(ctl, F,
      "unknown ostream scheme \"%s\"",
      ostream->scheme);
	goto cleanup;
  }
  else
  if (choice == -2)						/* ambiguous type?				    */
  {
    rc = dt_err(ctl, F,
      "ambiguous ostream scheme \"%s\"",
      ostream->scheme);
	goto cleanup;
  }

  DT_C((methods[choice]),(ctl,			/* give info to implementation		*/
    ostream, info, id, permit,
    timeout))

  ostream->initialized = 1;
cleanup:
  if (rc != DT_RC_GOOD)
  {
	if (ostream->scheme != NULL)
      DT_I(dtos_free2,(ctl,
        ostream->scheme,
        F, "scheme"));
	memset(ostream, 0, sizeof(*ostream));
  }
  return rc;
}

/*..........................................................................
| NAME
| dtostream_send() - send data packet
|
| SUMMARY
| Send ~packet_length~ bytes of data at ~packet~.
| If the communications pipe is full, then block.
| Have an error if the server has terminated the conversation.
| 
| END
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtostream_send(							/* send data packet					*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtostream_t *ostream,					/* this dtostream object 			*/
  void *packet,							/* data packet to send 				*/
  long packet_length)					/* data length to send 				*/
{
  DT_F("dtostream_send");
  if (ostream->send != NULL)
    DT_Q((*ostream->send),(
      ctl, ostream, 
      packet, packet_length))
  else
    return dt_err(ctl, F,
      "function not implemented"
      " for ostream scheme %s",
      ostream->scheme);
  return DT_RC_GOOD;
}

/*..........................................................................
| NAME
| dtostream_uninit() - quit conversation
|
| SUMMARY
| Terminate conversation.
| Will send no further data packets.
| Cause partner istream to notice end of conversation
| the next time it tries to send data.
| 
| END
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtostream_uninit(						/* release resources 				*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtostream_t *ostream)					/* this dtostream object 			*/
{
  DT_F("dtostream_uninit");
  dt_rc_e rc = DT_RC_GOOD;
  if (ostream->initialized)
  {
  	if (ostream->uninit != NULL)
      DT_Q((*ostream->uninit),(
        ctl, ostream));
	if (ostream->scheme != NULL)
	  DT_I(dtos_free2,(ctl,
        ostream->scheme,
        F, "scheme"));
  }
  memset(ostream, 0,					/* clear the data structure 		*/
    sizeof(*ostream));
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
