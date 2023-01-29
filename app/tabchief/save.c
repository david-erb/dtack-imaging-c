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
DT_RCSID("app/tabchief $RCSfile: save.c,v $ $Revision: 1.8 $");

/* ------------------------------------------------------------------------
 * ------------------------------------------------------------------------ */

dt_rc_e
DTCONFIG_API1
tabchief_server_savefile(
  dtparam_t *param,
  dtipc_t *ipc,
  const char *command,
  const char *arg)
{
  DT_F("tabchief_server_savefile");
  dt_ctl_t *ctl = &param->ctl;
  tabchief_t *tabchief = 
    (tabchief_t *)ctl->user;
  dtfd_t _fd, *fd = &_fd;
  char *query_string;
  char *p;
  char *save = "";
  char *quit = "";
  char *text = "";
  char *file = "";
  int i, n;
  dt_rc_e rc;

  fd->fd = -1;

  DT_C(dtos_strdup,(ctl, arg,
    &query_string));

  p = query_string? query_string: "";
  n = dsched_split(p, '&');				/* parse CGI input into tokens 		*/
  for (i=0; i<n; i++)					/* for each token 					*/
  {
	DSCHED_GET(save);
	DSCHED_GET(quit);
	DSCHED_GET(text);
	DSCHED_GET(file);
    p = p + strlen(p) + 1;				/* advance to next token 			*/
  }

  if (strlen(quit))
  {
	DT_C(dtstr_printf,(ctl,
      tabchief->response,
      sizeof(tabchief->response),
      "Quit button hit, changes not saved!"));
  }
  else
  if (strlen(file) == 0)
  {
	DT_C(dtstr_printf,(ctl,
      tabchief->response,
      sizeof(tabchief->response),
      "No filename, changes not saved!"));
  }
  else
  if (strcmp(save, "SAVE"))
  {
	DT_C(dtstr_printf,(ctl,
      tabchief->response,
      sizeof(tabchief->response),
      "Save button not hit, changes not saved!"));
  }
  else
  {
	DT_C(dtfd_open,(ctl, fd,			/* open local file for writing		*/
      file, "w"));
	DT_C(dtfd_write_all,(ctl, fd,		/* write the string 				*/
      text, strlen(text)));
	DT_C(dtstr_printf,(ctl,
      tabchief->response,
      sizeof(tabchief->response),
      "Changes saved to %s!", 
      file));
  }

cleanup:
  if (fd->fd != -1)
    DT_I(dtfd_close,(ctl, fd));

  if (query_string != NULL)
    DT_I(dtos_free,(ctl, query_string));

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
