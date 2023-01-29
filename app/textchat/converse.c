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
DT_RCSID("app/textchat $RCSfile: converse.c,v $ $Revision: 1.8 $");
#include <dtack/str.h>

/*..........................................................................
 * begin a conversation by telling both parties to start
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
textchat_converse(
  textchat_t *textchat,
  const char *talker_ident,
  const char *listener_ident)
{
  DT_F("textchat_converse");
  dt_ctl_t *ctl = textchat->ctl;
  dtdg_client_t _dgclient, *dgclient = &_dgclient;
  char request[256];
  const char *istream_spec;
  const char *audio_spec;
  dt_rc_e rc = DT_RC_GOOD;

  dgclient->initialized = 0;
										/* ................................ */
  DT_Q(ustate_find,(ctl,				/* get listener's audio spec		*/
    &textchat->ustate,
    listener_ident, "netchat",
    "audio_spec", 
    NULL,
    &audio_spec));

  DT_C(dtstr_printf,(ctl,				/* listener streaming request 		*/
    request, sizeof(request),
    "GET /stream?ipc&%s&%s",
    audio_spec,
    talker_ident));						/* tell listener to expect talker 	*/

  DT_C(textchat_ask,(textchat,			/* start listener streaming			*/
    listener_ident, request,
    dgclient));

  DT_C(dtdg_client_response_find,(		/* find listener's streaming spec	*/
    ctl, dgclient,
    DTDG_RESPONSE_ISTREAM_SPEC,
    &istream_spec));

  DT_C(dtdg_client_uninit,(ctl, 
    dgclient));
										/* ................................ */
  DT_Q(ustate_find,(ctl,				/* get talker's audio spec			*/
    &textchat->ustate,
    talker_ident, "netchat",
    "audio_spec", 
    NULL,
    &audio_spec));

  DT_C(dtstr_printf,(ctl,				/* talker streaming request 		*/
    request, sizeof(request),
    "GET /stream?%s&%s&%s",
    audio_spec,
    istream_spec,
    listener_ident));					/* tell talker to expect listener 	*/

  DT_C(textchat_ask,(textchat,			/* start talker streaming			*/
    talker_ident, request,
    dgclient));
										/* ................................ */

cleanup:
  if (dgclient->initialized)
    DT_I(dtdg_client_uninit,(ctl, dgclient));
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
