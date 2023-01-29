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
DT_RCSID("app/tabchief $RCSfile: index.c,v $ $Revision: 1.8 $");

dtlist_t tabchief_proclist;

#define TRIM TABCHIEF_TRIM

/* ------------------------------------------------------------------------
 * ------------------------------------------------------------------------ */

dt_rc_e
DTCONFIG_API1
tabchief_index(
  dtparam_t *param,
  dtipc_t *ipc,
  const char *command,
  const char *arg)
{
  DT_F("tabchief_index");
  dt_ctl_t *ctl = &param->ctl;
  dtlist_t _list, *list = &_list;
  int list_created = 0;
  char t1[256];
  char t2[256];
  dtlist_elem_t *elem;
  dt_rc_e rc;

#define LINK(URL1, URL2, URL3, WORDS) \
  tabchief_reply(param, ipc, \
    "<a href=\"/%s%s%s\">%s</a><br>", \
    URL1, URL2, URL3, WORDS);
    
  tabchief_reply(param, ipc, "<p>");
  LINK("", "", "doc/all.htm",  
    "<b>Help</b>");

  tabchief_reply(param, ipc, "<p><b>Parameters</b><br>");

  LINK("edit?", param->config, "/ftp.cfg",  
    " Edit ftp parameters");

  elem = DTLIST_ELEM_HEAD;
  while (elem != NULL)
  {
	char title[64], *argfile;
	char t[128];
	DT_C(dtlist_next,(ctl,				/* fetch proc elem 					*/
      &tabchief_proclist,
      elem, &elem));
	if (elem == NULL)
	  break;
	argfile = strchr(
      (char *)elem->user, '@');
	if (argfile == NULL)
	  continue;
	argfile++;							/* skip the '@' 					*/

	dtstr_printf(ctl, t, sizeof(t),
	  "%s", elem->name);
	TRIM(t);
	dtstr_printf(ctl, 
      title, sizeof(title), 
      "Edit %s parameters", t);
	
	LINK("edit?", "", argfile, title);
  }


  tabchief_reply(param, ipc, "<p><b>Schedules</b><br>");
  
  DT_C(dtlist_create,(ctl, list,
    "cam list"));
  list_created = 1;

  DT_C(dsched_camlist,(ctl,
    param->config, list));

  elem = DTLIST_ELEM_HEAD;
  while (elem != NULL)
  {
	dsched_cam_t *cam;
	DT_C(dtlist_next,(ctl, list,		/* fetch cam elem 					*/
      elem, &elem));
	if (elem == NULL)
	  break;
	cam = (dsched_cam_t *)elem->user;	/* cam description 					*/
	if (cam->description[0])			/* cam has a description? 			*/
      tabchief_reply(param, ipc, 
        "<a href=\"/schedule?id=%s\">"
        "Schedule for %s (spigot %s):"
        " %s</a><br>\n",
        elem->name, elem->name,
        cam->spigot,
        cam->description);
	else
      tabchief_reply(param, ipc, 
        "<a href=\"/schedule?id=%s\">"
        "Schedule for %s (spigot %s)"
        "</a><br>\n",
        elem->name, elem->name,
        cam->spigot);
  }

  tabchief_reply(param, ipc, "<p><b>Logs</b><br>\n");
  dtstr_printf(ctl, t1, sizeof(t1), "/../logs/%s.log", param->name);
  dtstr_printf(ctl, t2, sizeof(t2), "%s log", param->name);
  t2[0] = toupper(t2[0]);
  LINK("tail?", param->config, t1, t2);

  elem = DTLIST_ELEM_HEAD;
  while (elem != NULL)
  {
	char log[64], title[64];
	char t[128];
	DT_C(dtlist_next,(ctl,				/* fetch proc elem 					*/
      &tabchief_proclist,
      elem, &elem));
	if (elem == NULL)
	  break;
	dtstr_printf(ctl, t, sizeof(t),
	  "%s", elem->name);
	TRIM(t);
	dtstr_printf(ctl, log, sizeof(log), 
      "/../logs/%s.log", t);
	dtstr_printf(ctl, 
      title, sizeof(title), "%s log", 
      t);
    LINK("tail?", param->config, log, title);
  }

  tabchief_reply(param, ipc, "<p><b>Status</b><br>\n");
  DT_Q(tabchief_status_one,(param, ipc, param->name));
  tabchief_reply(param, ipc, "<br>");
  elem = DTLIST_ELEM_HEAD;
  while (elem != NULL)
  {
	DT_C(dtlist_next,(ctl,				/* fetch proc elem 					*/
      &tabchief_proclist,
      elem, &elem));
	if (elem == NULL)
	  break;
    DT_Q(tabchief_status_restart,(param, ipc, elem->name));
  }

  tabchief_reply(param, ipc, "<p>");
  LINK("restart?computer", "", "", "Restart Computer");

  tabchief_reply(param, ipc, "</body>"); /* do our own trailer 				*/
  tabchief_reply(param, ipc, "</html>");
  tabchief_reply(param, ipc, "");

cleanup:
  if (list_created)
  {
    dtlist_free(ctl, list);				/* free all list elements 			*/
    dtlist_destroy(ctl, list);			/* destroy list itself 				*/
  }

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
