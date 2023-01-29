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
  CHANGES
  20000223 DE added timeouts
 *..........................................................................*/

/*..........................................................................
| NAME
| main program
| 
| END
 *..........................................................................*/

#include <tabrobot.h>
DT_RCSID("app/tabrobot $RCSfile: roam.c,v $ $Revision: 1.8 $");

/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
tabrobot_roam(
  dtparam_t *param)
{
  DT_F("tabrobot_client");
  dt_ctl_t *ctl = &param->ctl;
  dtlist_t _list, *list = &_list;
  dtlist_elem_t *elem;
  long mtime = 0;
  int n;
  long i;
  int suspend;
  dt_rc_e rc;

  list->name = NULL;

  DT_C(dtlist_create,(ctl, list,		/* create list to read into 		*/
    "presets"));

  for (i=0; i<param->n; i++)
  {
	DT_C(dtos_isreadable,(ctl,			/* check for suspend file 			*/
      param->datadir, &suspend));
	if (suspend)						/* suspend?							*/
	{
	  int t = 10;
	  dt_dbg(ctl, F, DT_DBG_MASK_COMMAND,
        "suspend, delaying %d seconds", t);
	  DT_C(dtos_delay,(ctl, t*1000));	/* delay some seconds				*/
	  DT_C(dt_yield,(ctl, "suspend"));
	  continue;
	}
	DT_C(tabrobot_preset_load,(param,	/* load presets list 				*/
      list, &mtime));
	elem = DTLIST_ELEM_HEAD;
	n = 0;
    while(1)							/* scan presets list 				*/
	{
	  const char *s, *p;
	  int m, t, d;
	  long preset;
	  int isnumeric;
	  char command[32];
	  DT_C(dtlist_next,(ctl, list,		/* get next preset 					*/
        elem, &elem));
	  if (!elem)						/* end of presets list? 			*/
		break;
	  DT_C(dtstr_to_long2,(ctl,
        elem->name, &preset, 
        &isnumeric));
	  if (!isnumeric)					/* preset name not numeric? 		*/
		break;
	  s = (const char *)elem->user;		/* get preset description 			*/
	  p = strstr(s, "dwell=");			/* find dwell in preset description */
	  if (!p)							/* no dwell? 						*/
		continue;						/* skip this preset 				*/
	  t = 0;
	  m = sscanf(p, "dwell=%d", &t);	/* get dwell time 					*/
	  if (!m)							/* no dwell time? 					*/
		continue;						/* skip this preset 				*/
      DT_C(dtstr_printf,(ctl,			/* form goto command 				*/
        command, sizeof(command),
        "ptz,G%ld", preset));

      DT_C(tabrobot_client,(param,		/* send client command 				*/
        command));

	  dt_dbg(ctl, F, DT_DBG_MASK_COMMAND,
        "delaying %d seconds", t);

	  for (d=0; d<t; d++)
	  {
		DT_C(dtos_delay,(ctl, 1000));	/* delay 							*/
	    DT_C(dt_yield,(ctl, s));
	  }
	  n++;

	  DT_C(dtos_isreadable,(ctl,		/* check for suspend file 			*/
        param->datadir, &suspend));
	  if (suspend)						/* suspend?							*/
		break;
	}
	if (!n && !suspend)					/* no presets with dwell? 			*/
	{
	  int t = 10;
	  dt_dbg(ctl, F, DT_DBG_MASK_COMMAND,
        "no presets, delaying %d seconds", t);
	  DT_C(dtos_delay,(ctl, t*1000));	/* delay 10 seconds					*/
	  DT_C(dt_yield,(ctl, "no presets"));
	}
  }

  dt_dbg(ctl, F, DT_DBG_MASK_USR1,
    "voluntary quit after %ld",
    param->n);
  
cleanup:
  if (list->name)
  {
	DT_I(dtlist_free,(ctl, list));
	DT_I(dtlist_destroy,(ctl, list));
  }
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
