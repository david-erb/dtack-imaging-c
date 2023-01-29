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
DT_RCSID("app/tabchief $RCSfile: status.c,v $ $Revision: 1.8 $");
#include <dtack/msw.h>

#define LINK(URL1, URL2, WORDS) \
  tabchief_reply(param, ipc, \
    "<a href=\"/%s%s\">%s</a>", \
    URL1, URL2, WORDS)
    
/* ------------------------------------------------------------------------
 * ------------------------------------------------------------------------ */

dt_rc_e
DTCONFIG_API1
tabchief_status_restart(
  dtparam_t *param,
  dtipc_t *ipc,
  const char *process)
{
  DT_F("tabchief_status_restart");
  dt_ctl_t *ctl = &param->ctl;
  int winstart, win;
  char t[128];

  DT_Q(tabchief_status_win,(param, param->name, &winstart));

  dtstr_printf(ctl, t, sizeof(t),
    "%s", process);
  TABCHIEF_TRIM(t);

  DT_Q(tabchief_status_win,(param, process, &win));
  
  if (win != 0)
  {
	char s[64];
	tabchief_reply(param, ipc,
      "%s is running (0x%04x).",
      t, win);
	if (winstart > 0)
	  dtstr_printf(ctl, s, sizeof(t), "Restart %s", t);
	else
	  dtstr_printf(ctl, s, sizeof(t), "Stop %s", t);
	LINK("restart?", process, s);
  }
  else
	tabchief_reply(param, ipc,
      "%s (%s) is not running.", process, t);

  tabchief_reply(param, ipc, "<br>");
  return DT_RC_GOOD;
}

/* ------------------------------------------------------------------------
 * ------------------------------------------------------------------------ */

dt_rc_e
DTCONFIG_API1
tabchief_status_win(
  dtparam_t *param,
  const char *process,
  int *win)
{
  DT_F("tabchief_status_win");
  dt_ctl_t *ctl = &param->ctl;
# ifdef DTCONFIG_IMPLIED_MSW
  *win = (int)FindWindow(process, process);
  dt_dbg(ctl, F, DT_DBG_MASK_PIXEL, "process=\"%s\", *win=0x%04x", process, *win); 
# else
  *win = 0;
# endif
  return DT_RC_GOOD;
}

/* ------------------------------------------------------------------------
 * ------------------------------------------------------------------------ */

dt_rc_e
DTCONFIG_API1
tabchief_status_one(
  dtparam_t *param,
  dtipc_t *ipc,
  const char *process)
{
  DT_F("tabchief_status_one");
  dt_ctl_t *ctl = &param->ctl;
  char t[128];
  int win;

  dtstr_printf(ctl, t, sizeof(t),
    "%s", process);
  /*  TABCHIEF_TRIM(t); */

  DT_Q(tabchief_status_win,(param, t, &win));

  if (win != 0)
    tabchief_reply(param, ipc,
      "%s is running (0x%04x).",
      t, win);
  else
   tabchief_reply(param, ipc,
    "%s is not running.", t);
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
