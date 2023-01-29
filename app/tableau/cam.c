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
| USAGE
| shovel
| 
| END
 *..........................................................................*/

#include <tableau.h>
DT_RCSID("app/tableau $RCSfile: cam.c,v $ $Revision: 1.8 $");

#ifndef DTDEPEND						/* no makefile dependency 			*/
#  include <dsched.h>
#endif

#define BIT(BITMAP, I) ((BITMAP)[(I)>>3] & (0x80>>((I)&7)))
#define CLR(BITMAP, I) ((BITMAP)[(I)>>3] = (BITMAP)[(I)>>3] & ~(0x80>>((I)&7)))

/*..........................................................................
 *..........................................................................*/
static
time_t
cam_time(
  tableau_cam_t *cam)
{
  if (cam->autotime == 0)				/* want real time? 					*/
    return time(NULL);					/* give real time 					*/
  else									/* want fake time? 					*/
    return cam->autotime++;				/* auto advance one second 			*/
}

/*..........................................................................
 * load/reload single cam rules and make bitmap for this day only
 * create all structures as necessary
 *..........................................................................*/

static
dt_rc_e
tableau_cam_load_one(
  dtparam_t *param,
  tableau_cam_t *cam,
  int cam_index,
  int day)
{
  DT_F("tableau_cam_load_one");
  dt_ctl_t *ctl = &param->ctl;
  unsigned char *bitmap = cam->bitmap[cam_index];
  char filename[128];
  int isreadable;
  dsched_t _dsched, *dsched = &_dsched;
  int have_dsched = 0;
  dt_rc_e rc;

  DT_C(dsched_database,(param,			/* make database filename 			*/
    cam->name[cam_index],
    filename, sizeof(filename)));

  DT_C(dtos_isreadable,(ctl, filename,
    &isreadable));
  if (!isreadable)
  {
	rc = dt_err(ctl, F,
      "Sorry, \"%s\" is not a"
      " readable cam schedule file.",
      filename);
    goto cleanup;
  }

  if (bitmap == NULL)
    DT_C(dtos_malloc2,(ctl,				/* get space for day's bitmap 		*/
      (void **)&bitmap, 60*60*24,
      F, "bitmap"));
  
  dt_dbg(ctl, F, DT_DBG_MASK_WIN,
    "loading %s for %s",
    cam->name[cam_index],
    dsched_days[day]);

  dsched_init(dsched, param, NULL);		/* init database 					*/
  have_dsched = 1;

  dsched_load(dsched, filename);		/* load database 					*/

  if (DSCHED_ISERROR(dsched))			/* check for loading errors 		*/
  {
	rc = dt_err(ctl, F,
      "dsched error: %s",
      dsched->error);
	goto cleanup;
  }

  dsched_bitmap(dsched, day, bitmap);	/* load bitmap 						*/
  if (DSCHED_ISERROR(dsched))
  {
	rc = dt_err(ctl, F,
      "bitmap error: %s",
      dsched->error);
	goto cleanup;
  }

  cam->bitmap[cam_index] = bitmap;		/* remember bitmap 					*/
  cam->next_access[cam_index] = 0;
  cam->last_access[cam_index] = 0;

cleanup:
  if (rc != DT_RC_GOOD)
  {
	if (bitmap != NULL)
	{
	  DT_I(dtos_free2,(ctl, bitmap,
        F, "bitmap"));
	  cam->bitmap[cam_index] = NULL;
	}
  }

  if (have_dsched)
    dsched_uninit(dsched);
  return rc;
}

/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
tableau_cam_reload(
  dtparam_t *param,
  tableau_cam_t *cam)
{
  DT_F("tableau_cam_reload");
  dt_ctl_t *ctl = &param->ctl;
  time_t clock;
  struct tm *t, q;
  int day;
  int i;

  clock = cam_time(cam);				/* current time 					*/
  t = localtime(&clock);				/* format into fields 				*/
  day = cam->autotime? cam->autoday%7:	/* this day 						*/
    t->tm_wday;
  q = *t;
  q.tm_sec = q.tm_min = q.tm_hour = 0;	/* time at midnight					*/
  q.tm_wday = day;						/* possibly fake day 				*/
  cam->midnight = mktime(&q);			/* seconds at midnight 				*/
  if (cam->midnight == -1)
	return dt_err(ctl, F,
      "mktime failed");

  for (i=0; i<cam->count; i++)		/* for each possible camera 		*/
	DT_Q(tableau_cam_load_one,(param,	/* load information for this day 	*/
      cam, i, day));

  return DT_RC_GOOD;
}

/*..........................................................................
 * release all cam structures and bitmaps
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
tableau_cam_release(
  dtparam_t *param,
  tableau_cam_t *cam)
{
  DT_F("tableau_cam_release");
  dt_ctl_t *ctl = &param->ctl;
  int i;
  dt_rc_e rc = DT_RC_GOOD;

  for (i=0; i<cam->count; i++)		/* for each possible camera */
  {
	unsigned char *bitmap = cam->bitmap[i];
	if (bitmap != NULL)
	{
	  DT_I(dtos_free2,(ctl, bitmap,
        F, "bitmap"));
	  cam->bitmap[i] = NULL;
	}
  }

  return rc;
}

/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
tableau_cam_init(
  dtparam_t *param,
  tableau_cam_t *cam)
{
  DT_F("tableau_cam_init");
  dt_ctl_t *ctl = &param->ctl;
  dtlist_t _list, *list = &_list;
  int list_created = 0;
  dtlist_elem_t *elem;
  dt_rc_e rc;

  DT_C(dtlist_create,(ctl, list,		/* create list for cams 			*/
    "cam list"));
  list_created = 1;

  DT_C(dsched_camlist,(ctl,				/* read the list of cams 			*/
    param->config, list));

  elem = DTLIST_ELEM_HEAD;
  cam->count = 0;
  while (elem != NULL)					/* swerve through list of cams 		*/
  {
	DT_C(dtlist_next,(ctl, list,		/* fetch cam elem 					*/
      elem, &elem));
	if (elem == NULL)
	  break;
	if (cam->count >= TABLEAU_CAM_MAX)
	{
	  rc = dt_err(ctl, F,
        "too many cams in list,"
        " max is %d", 
        TABLEAU_CAM_MAX);
	  goto cleanup;
	}
	dtstr_printf(ctl,
      cam->name[cam->count],
      sizeof(cam->name[cam->count]),
      "%s", elem->name);
	dtstr_printf(ctl,
      cam->spigot[cam->count],
      sizeof(cam->spigot[cam->count]),
      "%s", ((dsched_cam_t *)elem->user)->spigot);
	cam->count++;
  }
  
  DT_C(tableau_cam_reload,(param, 		/* load all cam schedules 			*/
    cam));
  
  cam->initialized = 1;

cleanup:
  if (rc != DT_RC_GOOD)
  {
	DT_I(tableau_cam_release,(param,	/* release what we have so far 		*/
      cam));
	memset(cam, 0, sizeof(*cam));		/* clear out the structure 			*/
  }
  if (list_created)
  {
    DT_I(dtlist_free,(ctl, list));		/* free all list elements 			*/
    DT_I(dtlist_destroy,(ctl, list));	/* destroy list itself				*/
  }

  return rc;
}

/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
tableau_cam_uninit(
  dtparam_t *param,
  tableau_cam_t *cam)
{
  DT_F("tableau_cam_uninit");
  dt_ctl_t *ctl = &param->ctl;
  dt_rc_e rc = DT_RC_GOOD;
  DT_I(tableau_cam_release,(param, cam));
  memset(cam, 0, sizeof(*cam));			/* clear out the structure 			*/
  return rc;
}

/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
tableau_cam_now(
  dtparam_t *param,
  tableau_cam_t *cam,
  long *now)
{
  DT_F("tableau_cam_now");
  dt_ctl_t *ctl = &param->ctl;

  *now = cam_time(cam)-cam->midnight;	/* seconds since midnight 			*/

  if (*now >= 60*60*24)					/* day has changed? 				*/
  {
	cam->when = 0;						/* sleep no longer 					*/
    if (cam->autotime)					/* doing fake time?					*/
	{
	  if (cam->autoday == 13)			/* after two fake weeks? 			*/
	  {
		*now = -1;						/* signal end of test 				*/
		return DT_RC_GOOD;
	  }
	  cam->autoday++;
	}
	DT_Q(tableau_cam_reload,(param,		/* load information for new day 	*/
      cam));

	*now = cam_time(cam)-cam->midnight;	/* seconds since new midnight 		*/
  }
  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
tableau_cam_next(
  dtparam_t *param,
  tableau_cam_t *cam)
{
  DT_F("tableau_cam_next");
  dt_ctl_t *ctl = &param->ctl;
  long now;
  int i, j, k;

  DT_Q(tableau_cam_now,(param, cam, 	/* seconds since midnight 			*/
    &now));
										/* ................................ */
  for (i=0; i<cam->count; i++)			/* scan list of cameras 			*/
  {
	j = (cam->current_index+i+1) %		/* round-robin fashion 				*/
      cam->count;
	if (cam->bitmap[j] != NULL)			/* camera is active? 				*/
	{
	  for (k=cam->last_access[j];		/* check for bits on 				*/
		   k<=now; 
		   k++)
	    if (BIT(cam->bitmap[j], k))
          break;
	  if (k <= now)						/* some bit was on? 				*/
        break;
	}
  }

										/* ................................ */
  if (i < cam->count)					/* some camera is ready?			*/
  {
	cam->when = now;					/* no waiting 						*/
	cam->current_index = j;
	cam->last_access[j] = now;
  }										/* ................................ */
  else									/* no cam is ready? 				*/
  {					
	cam->when = 60*60*24;
	cam->current_index = -1;
	for (i=0; i<cam->count; i++)		/* scan list of cameras 			*/
	if (cam->bitmap[i] != NULL)			/* camera is active? 				*/
	{
	  k = cam->next_access[i];			/* next access to this cam 			*/
	  if (k < now)						/* don't know next access?			*/
	  {
		for (k=now; k<60*60*24; k++)	/* find next access 				*/
	      if (BIT(cam->bitmap[i], k))
            break;
		cam->next_access[i] = k;
	  }

	  if (k < cam->when)				/* this cam fires today? 			*/
	  {
		cam->when = k;
		cam->current_index = i;
	  }
	}
  }

  j = cam->current_index;
  k = cam->when;

  if (j != -1 &&
	  !BIT(cam->bitmap[j], k+1))		/* we are off next second?			*/
  {
	CLR(cam->bitmap[j], k);				/* no more shots this second 		*/
	cam->next_access[j] = 0;
  }

  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
tableau_cam_wait(
  dtparam_t *param,
  tableau_cam_t *cam)
{
  DT_F("tableau_cam_wait");
  dt_ctl_t *ctl = &param->ctl;
  long now;
  long delay;

  if (cam->when <= 0)					/* no delay? 						*/
    return DT_RC_GOOD;

  if (DT_DBG(ctl, DT_DBG_MASK_WIN))
  {
	DT_Q(tableau_cam_now,(param, cam, 	/* seconds since midnight 			*/
      &now));

	delay = cam->when - now;			/* seconds to delay 				*/

	dt_dbg(ctl, F, DT_DBG_MASK_WIN,
      "no cam ready, delaying %ld seconds",
      delay);
  }

  if (cam->autotime == 0)				/* want real time? 					*/
    while(1)
	{
	  DT_Q(tableau_cam_now,(param, cam,	/* seconds since midnight 			*/
        &now));
	  delay = cam->when - now;			/* seconds to delay 				*/
	  if (delay <= 0)
	    break;
	  delay = DT_MIN(delay, 1);
	  DT_Q(dt_yield,(ctl, 
        "cam schedule wait"));
      dtos_delay(ctl, delay * 1000);	/* really delay 					*/
	}
  else									/* fake time? 						*/
    cam->autotime = cam->when;			/* auto advance time 				*/

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
