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

#include <tabgrab.h>
DT_RCSID("app/tabgrab $RCSfile: server.c,v $ $Revision: 1.8 $");

#define CR (0x0d)
#define LF (0x0a)

/*..........................................................................
 *..........................................................................*/

static
dt_rc_e
tabgrab_server_transact(
  tabgrab_t *tabgrab,
  dtipc_t *ipc)
{
  DT_F("tabgrab_server_transact");
  dt_ctl_t *ctl = tabgrab->ctl;
  int isready;
  char cmd[128];
  char ack[128];
  int l;
  char *p;
  long frame = 0;
  dt_rc_e rc;

	DT_C(dtipc_poll,(ctl, ipc,			/* wait for incoming data 			*/
      DTIPC_POLL_READ, 
      tabgrab->param->dt,
      "ipc read", &isready));
	if (!isready)						/* no data ready? 					*/
	  goto cleanup;

	DT_C(dtipc_read_line,(ctl,			/* get request line 				*/
      ipc, cmd, sizeof(cmd)));

	l = strlen(cmd);
	while (l > 0 &&						/* nuke the crlf's 					*/
		   cmd[l-1] == LF ||
		   cmd[l-1] == CR)
	  cmd[--l] = '\0';
		
	if (l == 0)
	{
	  dt_dbg(ctl, NULL, 
        DT_DBG_MASK_COMMAND, 
        "client closed her end");
	  goto cleanup;
	}

	if (DT_ABBREV(cmd, "grab,"))
	{
	  p = strchr(cmd, ',');				/* see if there is a comma 			*/
	  if (p != NULL)					/* there is a comma? 				*/
	    DT_C(dtstr_to_long,(ctl, p+1,	/* see if a number follows			*/
          &frame));

	  dt_dbg(ctl, NULL, 
        DT_DBG_MASK_COMMAND, 
        "\"%s\", current frame is %ld",
        cmd, tabgrab->frame);

	  tabgrab->nrequests++;
	  if (frame >= tabgrab->frame)		/* don't already have this frame?	*/
	    DT_C(tabgrab_grab,(tabgrab))	/* grab next frame 					*/
	  else
		tabgrab->nalready++;
	  dtstr_printf(ctl,					/* build ack line 					*/
        ack, sizeof(ack),			
        "%s,%ld%c%c",
        tabgrab->filename, 
        tabgrab->frame, CR, LF);
	}
	else
	if (DT_ABBREV(cmd, "brightness") ||
		DT_ABBREV(cmd, "contrast") ||
		DT_ABBREV(cmd, "hue") ||
		DT_ABBREV(cmd, "pan") ||
		DT_ABBREV(cmd, "quality") ||
		DT_ABBREV(cmd, "saturation") ||
		DT_ABBREV(cmd, "source") ||
		DT_ABBREV(cmd, "zoom") ||
		DT_ABBREV(cmd, "chcv") ||		/* for meteor 						*/
		DT_ABBREV(cmd, "rgain") ||
		DT_ABBREV(cmd, "ggain") ||
		DT_ABBREV(cmd, "bgain"))		
	{
	  dtstr_printf(ctl,					/* build ack line 					*/
        ack, sizeof(ack),			
        "ok%c%c",
        CR, LF);
	  DT_C(dtfb_source,(ctl,			/* let fb implementation do it 		*/
        &tabgrab->fb, cmd));
	}
	else
	if (DT_ABBREV(cmd, "stop"))
	{
	  dtstr_printf(ctl,					/* build ack line 					*/
        ack, sizeof(ack),			
        "ok%c%c",
        CR, LF);
	  rc = DT_RC_STOP;
	}
	else
	if (DT_ABBREV(cmd, "stats"))
	{
	  dtos_time_t now;
	  double elapsed;
	  DT_C(dtos_time,(ctl, &now));
	  elapsed = now.wall -
		tabgrab->beg.wall;
	  dtstr_printf(ctl,					/* build response line 				*/
        ack, sizeof(ack),			
        "%ld frames in %0.0f seconds is %0.2f fps and %0.4f sec/frame%c%c",
        tabgrab->frame,
        elapsed,
  	    elapsed? tabgrab->frame/elapsed: 0.0,
  	    tabgrab->frame? elapsed/tabgrab->frame: 0.0,
        CR, LF);
	}
	else
	  rc = dt_err(ctl, F,
        "unrecognized command \"%s\"",
        cmd);
  
cleanup:
	if (rc != DT_RC_GOOD &&
		rc != DT_RC_STOP)
	  dtstr_printf(ctl,					/* build nack line 					*/
        ack, sizeof(ack),			
        "%s%c%c",
        "error", 0, CR, LF);

	DT_I(dtipc_write_all,(ctl, ipc,		/* return ack/nack line 			*/
      ack, strlen(ack)));

  return rc;
}

/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
tabgrab_server(
  dtparam_t *param)
{
  DT_F("tabgrab_server");
  dt_ctl_t *ctl = &param->ctl;
  tabgrab_t _tabgrab, *tabgrab = &_tabgrab;
  dtipc_t _ipc, *ipc = &_ipc;
  dt_rc_e rc;

  ipc->type = DTIPC_TYPE_NONE;

  DT_C(tabgrab_init,(tabgrab, param));	/* init tabgrab structure 			*/
  
  DT_C(dtipc_register,(ctl, ipc,		/* register as a server 			*/
    param->connect, 0, 0));

  dt_dbg(ctl, F, DT_DBG_MASK_USR1,
    "ready for action");

  while(1)
  {
	int isready;

	DT_C(tabgrab_grab,(tabgrab));		/* grab another frame for fun		*/

	DT_C(dtipc_poll,(ctl, ipc,			/* wait for incoming connection		*/
      DTIPC_POLL_ACCEPT, 
      tabgrab->param->omega,
      "client connect", &isready));
	if (isready)						/* client is connecting? 			*/
	{
      DT_C(dtipc_wait,(ctl, ipc));		/* wait for connect					*/
	  
	  DT_G(tabgrab_server_transact,(	/* transact							*/
        tabgrab, ipc));
	  DT_I(dtipc_done,(ctl, ipc));		/* server finishes transaction	    */
	  if (rc == DT_RC_STOP)
	    goto cleanup;
	}
	else
	  DT_C(dt_yield,(ctl,				/* check for async messages 		*/
        "polling for connections"));
  }

cleanup:
  if (ipc->type != DTIPC_TYPE_NONE)
    DT_I(dtipc_unregister,(ctl, ipc));	/* release resources				*/

  dt_dbg(ctl, F, DT_DBG_MASK_TIME,
    "%ld frames grabbed", 
    tabgrab->frame);
  dt_dbg(ctl, F, DT_DBG_MASK_TIME,
    "%ld TCP requests",
    tabgrab->nrequests);
  dt_dbg(ctl, F, DT_DBG_MASK_TIME,
    "%ld frames already there",
    tabgrab->nalready);
 
  DT_I(tabgrab_uninit,(tabgrab));		/* free tabgrab resources 			*/
  
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
