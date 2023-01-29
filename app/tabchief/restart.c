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
DT_RCSID("app/tabchief $RCSfile: server.c,v $ $Revision: 1.8 $");
#include <dtack/msw.h>

#define DT_RC_SHUTDOWN ((dt_rc_e)(0xdeadfeeb))

/* ------------------------------------------------------------------------
 * ------------------------------------------------------------------------ */

dt_rc_e
DTCONFIG_API1
tabchief_restart(
  dtparam_t *param,
  dtipc_t *ipc,
  const char *command,
  const char *arg)
{
  DT_F("tabchief_server_restart");
  dt_ctl_t *ctl = &param->ctl;
  const char *process = arg;
  char t[128];
  int win;

  dtstr_printf(ctl, t, sizeof(t),
    "%s", process);
  /*  TABCHIEF_TRIM(t); */

  tabchief_reply(param, ipc, "<p>");
  if (!strcmp(t, "computer"))
  {
	dt_rc_e rc;
    DT_G(tabchief_restart,(
      param, ipc, "restart", param->name));

    DT_G(tabchief_restart,(
      param, ipc, "restart", "tableau"));

	tabchief_reply(param, ipc, 
      "<p>Rebooting now, bye-bye!");

    return DT_RC_SHUTDOWN;
  }
  else
  if (!strcmp(t, param->name))	/* stopping tabstart? 				*/
  {
	DT_Q(tabchief_status_win,(
      param, t, &win));
	if (win != 0)						/* process looks stoppable? 		*/
	{
	  
	  int result;
#     ifdef DTCONFIG_IMPLIED_MSW32
	    DWORD pid = 0;
	    HANDLE handle;
	    GetWindowThreadProcessId(
          (HWND)win, &pid);
	    handle = OpenProcess(0, 0, pid);
        result = TerminateProcess(handle, 0);
	    CloseHandle(handle);
#	  else
	    int pid, handle;
        result = pid = handle = 0;
#     endif
      tabchief_reply(param, ipc, 
        "<p>%s asked %s (pid 0x%08lx, handle 0x%04x) to stop.",
        result? 
          "Successfully": 
          "Unsuccessfully",
        process, pid, handle);
	}
  }
  else									/* stopping tabchief or tableau? 	*/
  {
	
	DT_Q(tabchief_status_win,(
      param, t, &win));
	if (win != 0)						/* process looks stoppable? 		*/
	{
	  int result;
#     ifdef DTCONFIG_IMPLIED_MSW32
        result = PostMessage((HWND)win,
          DTMSW_MESSAGE_STOP, 0, 0);
#	  else
        result = 0;
#     endif
      tabchief_reply(param, ipc, 
        "<p>%s asked %s to stop.",
        result? 
          "Successfully": 
          "Unsuccessfully",
        t);
	}
	  
	DT_Q(tabchief_status_win,(			/* see if tabstart is alive 		*/
      param, param->name, &win));

	if (win != 0)						/* tabstart looks healthy? 			*/
      tabchief_reply(param, ipc, 
        "<p>%s will soon restart %s.",
        param->name, t);
	else								/* tabstart is dead? 				*/
	  tabchief_reply(param, ipc, 
        "<p>%s is dead"
        " so an automatic restart of %s"
        " is not possible.",
        param->name, t);
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
