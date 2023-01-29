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

#include <tabchief.h>
DT_RCSID("app/tabchief $RCSfile: edit.c,v $ $Revision: 1.8 $");

/* ------------------------------------------------------------------------
 * ------------------------------------------------------------------------ */

dt_rc_e
DTCONFIG_API1
tabchief_server_editfile(
  dtparam_t *param,
  dtipc_t *ipc,
  const char *command,
  const char *arg)
{
  DT_F("tabchief_server_editfile");
  dt_ctl_t *ctl = &param->ctl;
  int isreadable;
  dt_rc_e rc;

  DT_C(dtos_isreadable,(ctl, arg,
    &isreadable));
  if (!isreadable)
  {
	rc = dt_err(ctl, F,
      "Sorry, \"%s\" is not a readable file.",
      arg);
	goto cleanup;
  }

  tabchief_reply(param, ipc, 
    "<form action=\"/save\" method=GET>"); 

  tabchief_reply(param, ipc, "Save to file: "
    "<input size=48 name=\"file\""
    " value=\"%s\" type=\"text\"><br>\n",
    arg);

  tabchief_reply(param, ipc, 
    "<input type=\"submit\""
    " name=\"save\""
    " value=\"SAVE\">");
  
  tabchief_reply(param, ipc, 
    "<input type=\"submit\""
    " name=\"quit\""
    " value=\"QUIT\"><br>");
  
  tabchief_reply(param, ipc,
    "<textarea rows=20 cols=80 name=\"text\">");

  DT_C(tabchief_server_catfile,(param,
    ipc, arg, TABCHIEF_FLAGS_RAW));

cleanup:

  tabchief_reply(param, ipc, "");

  tabchief_reply(param, ipc,
    "</textarea>");

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
