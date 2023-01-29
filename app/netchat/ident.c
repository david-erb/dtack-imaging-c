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
| netchat_* - netchat internal routine
| 
|.DESCRIPTION
| Undocumented.
| 
| END
 *..........................................................................*/

#include <netchat.h>
DT_RCSID("app/netchat $RCSfile: ident.c,v $ $Revision: 1.8 $");
#include <dtack/str.h>
#include <dtack/label.h>

/*..........................................................................
 * get audio server's ipc spec given audio ident
 *..........................................................................*/

dt_rc_e 
DTCONFIG_API1
netchat_ident_to_spec(					/* convert ident to dg ipc spec		*/
  netchat_t *netchat,
  const char *ident,					/* audio client ident 				*/
  char *spec,							/* returned spec 					*/
  int spec_max)							/* max length of spec 				*/
{
  DT_F("netchat_ident_to_spec");
  dt_ctl_t *ctl = netchat->ctl;
  const char *t;
  dt_rc_e rc;

  DT_G(ustate_find,(ctl,				/* get ustream's dgserver address	*/
    &netchat->ustate,
    ident, "ustream",
    "dgserver_spec", 
    NULL,
    &t));
  if (rc != DT_RC_GOOD)
  {
	if (DT_LABEL(DT_LABEL_NOTFOUND,
		  netchat->message))
	  DT_Q(dtstr_printf,(ctl,
        netchat->message,
        sizeof(netchat->message),
        NETCHAT_M_NOSUCHPARTNER,
        ident));
	return rc;
  }

  DT_Q(dtstr_printf,(ctl,
    spec, spec_max, "%s", t));
  
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
