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

#include <dtack/win_r.h>

#include <dtack/winc.h>

#include <dtack/os.h>
#include <dtack/str.h>

static const char *choices[] = {
  "ipc",
  "dsk",
  NULL
};

static dtwinc_init_f *methods[] = {
  dtwinc_ipc_init,
  dtwinc_dsk_init
};

/*..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtwinc_init(							/* init structure					*/
  dt_ctl_t *ctl,
  dtwinc_t *winc,
  const char *init)
{
  DT_F("dtwinc_init");
  char tmp[DTWINC_SERVER_MAXLEN];
  char *server;
  int choice;

  if (init == NULL)						/* no window client type given? 	*/
    return dt_err(ctl, F,
      "NULL win server type");
  else
  if (strlen(init) == 0)				/* no window client type given? 	*/
    return dt_err(ctl, F,
      "zero-length win server type");
  else
  if (strlen(init) > sizeof(tmp)-1)
    return dt_err(ctl, F,
      "winc name \"%s\" too long",
      init);

  strcpy(tmp, init);
  init = tmp;
  server = strchr(init,					/* get server part					*/
    DT_SEPARATOR_SPEC);

  if (server != NULL)					/* separate init from server	    */
  {
    if (server == init)
      return dt_err(ctl, F,
        "zero-length server name in \"%s\"",
        init);
    *server = '\0';
	server = server + 1;
  }
  else
    server = "";

  DT_Q(dtstr_choice,(ctl, choices,		/* look up win server type	    	*/
    init, &choice));

  if (choice == -1)						/* unknown name					    */
    return dt_err(ctl, F,
      "unknown win spec \"%s\"",
      init);
  else
  if (choice < -1)						/* ambiguous name				    */
    return dt_err(ctl, F,
      "ambiguous win spec \"%s\"",
      init);

  DT_Q(dtstr_printf,(ctl,				/* remember server name 			*/
    winc->server, 
    sizeof(winc->server),
    "%s", server));

  DT_Q((methods[choice]),(ctl,			/* chop off the win spec			*/
    winc));

  return DT_RC_GOOD;
}

/*..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtwinc_uninit(							/* init structure					*/
  dt_ctl_t *ctl,
  dtwinc_t *winc)
{
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
