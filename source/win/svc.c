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
| DESCRIPTION
| This is the win cover level.
|
| RETURN VALUES
| All functions Return DTOS_RANWELL on success.  
| Any other return value indicates failure.
| 
| ERRORS
| As yet undocumented.
| 
| END
 *..........................................................................*/

#include <dtack/base.h>
DT_RCSID("source/low $RCSfile: winc.c,v $ $Revision: 1.1 $");

#include <dtack/list.h>
#include <dtack/svc.h>
#include <dtack/os.h>
#include <dtack/str.h>

static const char *choices[] = {
  "ipc",
  "dsk",
  NULL
};

static dtsvc_register_f *register_methods[] = {
  dtsvc_ipc_register,
  dtsvc_dsk_register
};

/*..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtsvc_register(							/* register name					*/
  dt_ctl_t *ctl,
  dtsvc_t *svc,
  const char *svcspec,
  unsigned long flags)
{
  DT_F("dtsvc_register");
  char *server;
  char tmp[DTSVC_SERVER_MAXLEN];
  int choice;
  dtsvc_t tmpsvc;

  if (svcspec == NULL)					/* no window client type given? 	*/
    return dt_err(ctl, F,
      "NULL svcspec");
  else
  if (strlen(svcspec) == 0)				/* no window client type given? 	*/
    return dt_err(ctl, F,
      "zero-length svcspec");
  else
  if (strlen(svcspec) > sizeof(tmp)-1)
    return dt_err(ctl, F,
      "svcspec \"%s\" too long",
      svcspec);

  dt_dbg(ctl, F, DT_DBG_MASK_IPC,
    "svcspec is \"%s\"",
     svcspec? svcspec: "NULL");

  strcpy(tmp, svcspec);
  svcspec = tmp;
  server = strchr(svcspec,				/* get server part					*/
    DT_SEPARATOR_SPEC);

  if (server != NULL)					/* separate type from server	    */
  {
    if (server == svcspec)
      return dt_err(ctl, F,
        "zero-length server part"
        " in svcspec \"%s\"",
        svcspec);
    *server = '\0';
	server = server + 1;
  }
  else
    server = "";

  DT_Q(dtstr_choice,(ctl, choices,		/* look up win server type	    	*/
    svcspec, &choice));

  if (choice == -1)						/* unknown svcspec				    */
    return dt_err(ctl, F,
      "unknown svcspec \"%s\"",
      svcspec);
  else
  if (choice < -1)						/* ambiguous svcspec			    */
    return dt_err(ctl, F,
      "ambiguous svcspec \"%s\"",
      svcspec);

  DT_MEMSET(&tmpsvc, 0, sizeof(tmpsvc));	/* clear the structure 				*/

  strcpy(tmpsvc.server, server);		/* remember server name 			*/

  DT_Q((register_methods[choice]),(ctl,	/* call the register method			*/
    &tmpsvc, server, flags));

  *svc = tmpsvc;						/* copy whole structure to caller	*/

  return DT_RC_GOOD;
}

/*..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtsvc_unregister(						/* unregister structure				*/
  dt_ctl_t *ctl,
  dtsvc_t *svc)
{
  DT_F("dtsvc_unregister");
  dt_rc_e rc = DT_RC_GOOD;
  if (svc != NULL &&
	  svc->unregister != NULL)
    DT_I(svc->unregister,(ctl, svc));	/* call unregister method 			*/
  DT_MEMSET(svc, 0, sizeof(*svc));			/* clear structure for safety 		*/
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
