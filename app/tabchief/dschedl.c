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
DT_RCSID("app/tabchief $RCSfile: load.c,v $ $Revision: 1.8 $");
#include <dtack/liner.h>

#ifdef DTCONFIG_IMPLIED_UNIXLIKE
#define MODE_READ "r"
#define MODE_WRITE "w"
#endif 

#ifdef DTCONFIG_IMPLIED_MSW
#define MODE_READ "rb"
#define MODE_WRITE "wb"
#endif 

/* ------------------------------------------------------------------------
 * ------------------------------------------------------------------------ */
dt_rc_e
DTCONFIG_API1
dsched_database(
  dtparam_t *param,
  const char *id,
  char *filename,
  int max)
{
  dtstr_printf(NULL,
    filename, max,
    "%s/%s.sch", param->config, id);

  return DT_RC_GOOD;
}

/* ------------------------------------------------------------------------
 * WARNING WARNING WARNING WARNING: CODE DUPLICATED
 * ------------------------------------------------------------------------ */

static
void
dsched_error(
  dsched_t *dsched,
  const char *format,
  ...)
{
  va_list arglist;
  va_start(arglist, format);
  if (strlen(dsched->error) == 0)
  vsprintf(dsched->error, format,		/* format error message 			*/
    arglist);	
  va_end(arglist);
}

/* ------------------------------------------------------------------------
 * ------------------------------------------------------------------------ */

void
DTCONFIG_API1
dsched_init(		
  dsched_t *dsched,
  dtparam_t *param,
  dtipc_t *ipc)
{
  dt_ctl_t *ctl;
  memset(dsched, 0, sizeof(*dsched));
  dsched->param = param;
  dsched->ipc = ipc;
  ctl = &dsched->param->ctl;
  dtos_strdup(ctl, "", &dsched->added.day);
  dtos_strdup(ctl, "", &dsched->added.start);
  dtos_strdup(ctl, "", &dsched->added.end);
  dtos_strdup(ctl, "", &dsched->added.period);
  dtos_strdup(ctl, "", &dsched->added.duration);
}

/* ------------------------------------------------------------------------
 * ------------------------------------------------------------------------ */

void
DTCONFIG_API1
dsched_uninit(		
  dsched_t *dsched)
{
  int i;
  dsched_rule_free(dsched, &dsched->added);
  for (i=0; i<DSCHED_RULES; i++)
    dsched_rule_free(dsched, &dsched->rule[i]);
  memset(dsched, 0, sizeof(*dsched));
}
/* ------------------------------------------------------------------------
 * ------------------------------------------------------------------------ */

void
DTCONFIG_API1
dsched_bitset(							/* turn on bits						*/
  dsched_t *dsched,
  int start,
  int end,
  unsigned char *bitmap)
{
  int i;

  if (DSCHED_ISERROR(dsched))
    return;

  for (i=start; i<end; i++)
  {
    bitmap[i>>3] |= (unsigned char)(0x80 >> (i&7));
  }
}

/* ------------------------------------------------------------------------
 * if bitmap argument is NULL, just parse
 * ------------------------------------------------------------------------ */

void
DTCONFIG_API1
dsched_bitmap(							/* convert rules to 86400-bit map	*/
  dsched_t *dsched,
  int wantday,
  unsigned char *bitmap)
{
  int i;

  memset(bitmap, 0, 60*60*24);			/* clear bitmap 					*/

  for (i=0; i<DSCHED_RULES; i++)		/* for each possible rule 			*/
  if (dsched->rule[i].day != NULL)		/* rule is active? 					*/
  {
	int ok;
	int dayi;
	int start, end;
	int period, duration;
	
	dsched_check(dsched,
      dsched->rule[i].day,
      dsched->rule[i].start,
      dsched->rule[i].end,
      dsched->rule[i].period,
      dsched->rule[i].duration,
      &ok);

	if (!ok)
	  return;

    for (dayi=0; dayi<7; dayi++)		/* find day for rule				*/
	  if (!strcmp(dsched->rule[i].day,
		  dsched_days[dayi]))
	    break;
   
	if (wantday != dayi)
	  continue;

	dsched_time_to_seconds(dsched,
      dsched->rule[i].start, &start);
	dsched_time_to_seconds(dsched,
      dsched->rule[i].end, &end);
	dsched_time_to_seconds(dsched,
      dsched->rule[i].period, &period);
	dsched_time_to_seconds(dsched,
      dsched->rule[i].duration, &duration);

	if (period == 0)					/* continuous? 						*/
	{
	  dsched_bitset(dsched,
        start, end, bitmap);
	}
	else								/* timed snapshots?					*/
	{
	  int j;

	  if (duration == 0)				/* single frame? 					*/
	    duration = 1;					/* same as one second 				*/

	  for (j=start; j<end; j+=period)
	    dsched_bitset(dsched,
          j, j+duration, bitmap);
	}	
  }
}

/* ------------------------------------------------------------------------
 * ------------------------------------------------------------------------ */

void
DTCONFIG_API1
dsched_natural_time(					/* time in natural language			*/
  dsched_t *dsched,
  int seconds,
  char *t)
{
  int hours = seconds / (60*60);
  int minutes = (seconds - hours * 60 * 60) / 60;
  seconds = seconds % 60;
  sprintf(t, "%02d:%02d", hours, minutes);
  if (seconds)
    sprintf(t+strlen(t), ".%02d", seconds);
}

/* ------------------------------------------------------------------------
 * ------------------------------------------------------------------------ */

char *
dsched_natural_expand(					/* time in natural language			*/
  dsched_t *dsched,
  const char *string,
  char *t)
{
  int seconds;
  if (string[0] == '\0')
  {
    t[0] = '\0';
  }
  else
  {
	dsched_time_to_seconds(dsched,		/* first convert to seconds 		*/
      string, &seconds);
	if (seconds == -1)
      strcpy(t, string);
	else
	  dsched_natural_time(dsched,		/* then make natural time 			*/
        seconds, t);
  }
  return t;
}

/* ------------------------------------------------------------------------
 * ------------------------------------------------------------------------ */

void
DTCONFIG_API1
dsched_natural(							/* rule in natural language			*/
  dsched_t *dsched,
  int rule,
  char *s,
  int max)
{
  int period, duration;

  sprintf(s, "%s at ",
    dsched->rule[rule].day);

  dsched_natural_expand(dsched, 
    dsched->rule[rule].start, 
    s+strlen(s));
  strcat(s, " until ");
  dsched_natural_expand(dsched, 
    dsched->rule[rule].end, 
    s+strlen(s));

  dsched_time_to_seconds(dsched,		/* convert period to seconds		*/
    dsched->rule[rule].period, 
	&period);
  if (period == -1)
  {
	strcat(s, "error");
	return;
  }
  
  dsched_time_to_seconds(dsched,		/* convert duration to seconds		*/
    dsched->rule[rule].duration, 
	&duration);
  if (duration == -1)
  {
	strcat(s, "error");
	return;
  }
  
  if (period == 0)
  {
	strcat(s, " continuously");
  }
  else
  {
	strcat(s, " every ");
    dsched_natural_expand(dsched, 
      dsched->rule[rule].period, 
      s+strlen(s));
	if (duration == 0)
	{
	  strcat(s, ", single frame only");
	}
	else
	{
	  strcat(s, " lingering for ");
	  dsched_natural_expand(dsched,  
        dsched->rule[rule].duration, 
        s+strlen(s));
	}
  }
}

/* ------------------------------------------------------------------------
 * ------------------------------------------------------------------------ */

FILE *
dsched_open(
  dsched_t *dsched,
  const char *filename,
  const char *mode)
{
  FILE *file = fopen(filename, mode);
  if (file == NULL)
    dsched_error(dsched,
      "unable to open file \"%s\" mode \"%s\"",
      filename, mode);
  return file;
}

/* ------------------------------------------------------------------------
 * ------------------------------------------------------------------------ */

void
DTCONFIG_API1
dsched_time_to_seconds(					/* convert HH:MM.SS to seconds  	*/
  dsched_t *dsched,
  const char *string,
  int *seconds)
{
  char s[128];
  int hour = 0;
  int minute = 0;
  int second = 0;
  int z;
  int n[4];

  *seconds = -1;						/* in case of conversion error 		*/

  dtstr_printf(NULL, s, sizeof(s)-4,
    "%s", string);
  strcat(s, "XXX0");

  memset(n, 0, sizeof(n));
  if ((n[0] = sscanf(s, "%d:%d.%dXXX%d", &hour, &minute, &second, &z)) != 4 &&
	  (n[1] = sscanf(s, "%d:%dXXX%d", &hour, &minute, &z))             != 3 &&
	  (n[2] = sscanf(s, "%d.%dXXX%d", &minute, &second, &z))           != 3 &&
	  (n[3] = sscanf(s, "%dXXX%d", &minute, &z))                       != 2)
  {
    dsched_error(dsched, 
      "Sorry, \"%s\" is not a valid time.",
      string);
	return;
  }

  if (n[2])
    hour = 0;
  if (n[3])
  {
	hour = 0;
	second = 0;
  }

  if (hour < 0 || hour > 23)
  {
    dsched_error(dsched, 
      "Sorry, \"%s\" has an invalid hour value.",
      string);
	return;
  }

  if (minute < 0 || minute > 59)
  {
    dsched_error(dsched, 
      "Sorry, \"%s\" has an invalid minute value.",
      string);
	return;
  }

  if (second < 0 || second > 59)
  {
    dsched_error(dsched, 
      "Sorry, \"%s\" has an invalid second value.",
      string);
	return;
  }
  
  *seconds = (hour * 60 + minute) * 60 + second;
}

/* ------------------------------------------------------------------------
 * if bitmap argument is NULL, just parse
 * ------------------------------------------------------------------------ */

void
DTCONFIG_API1
dsched_check(							/* check rule						*/
  dsched_t *dsched,
  const char *day,
  const char *start,
  const char *end,
  const char *period,
  const char *duration,
  int *ok)
{
  int dayi;
  int start_seconds;
  int end_seconds;
  int period_seconds;
  int duration_seconds;

  *ok = 0;

  for (dayi=0; dayi<7; dayi++)			/* find day for rule				*/
    if (!strcmp(day, dsched_days[dayi]))
	  break;
   
  if (dayi == 7)						/* not a valid day? 				*/
  {
	dsched_error(dsched,
      "Sorry, \"%s\" is not"
      " a day of the week.",
      day);
	return;
  }

  dsched_time_to_seconds(dsched,		/* convert start to seconds 		*/
    start, &start_seconds);
  if (start_seconds == -1)
  {
	strcpy(dsched->error+
      strlen(dsched->error)-1,
      " to start.");
    return;
  }
  dsched_time_to_seconds(dsched,		/* convert end to seconds 			*/
    end, &end_seconds);
  if (end_seconds == -1)
  {
	strcpy(dsched->error+
      strlen(dsched->error)-1, 
      " to end.");
    return;
  }
  if (start_seconds > end_seconds)
    dsched_error(dsched,
      "Sorry, \"%s\" is"
      " after \"%s\".",
      start, end);

  dsched_time_to_seconds(dsched,		/* convert period to seconds		*/
    period, &period_seconds);
  if (period_seconds == -1)
  {
	strcpy(dsched->error+
      strlen(dsched->error)-1, 
      " for period.");
    return;
  }

  dsched_time_to_seconds(dsched,		/* convert duration to seconds		*/
    duration, &duration_seconds);
  if (duration_seconds == -1)
  {
	strcpy(dsched->error+
      strlen(dsched->error)-1, 
      " to linger.");
    return;
  }
  
  *ok = 1;
}

/* ------------------------------------------------------------------------
 * ------------------------------------------------------------------------ */

void
DTCONFIG_API1
dsched_rule_free(						/* delete rule from structure 		*/
  dsched_t *dsched,
  dsched_rule_t *rule)
{
  DT_F("dsched_rule_free");
  dt_ctl_t *ctl = &dsched->param->ctl;
#define FREE(MEMBER) \
  if (rule->MEMBER != NULL) \
    dtos_free2(ctl, (char *)rule->MEMBER, F, #MEMBER); \
  rule->MEMBER = NULL;
  FREE(day);
  FREE(start);
  FREE(end);
  FREE(period);
  FREE(duration);
#undef FREE
}
  
/* ------------------------------------------------------------------------
 * ------------------------------------------------------------------------ */

void
DTCONFIG_API1
dsched_add(								/* add rule to structure 			*/
  dsched_t *dsched,
  const char *day,
  const char *start,
  const char *end,
  const char *period,
  const char *duration)
{
  dt_ctl_t *ctl = &dsched->param->ctl;

  int ok;
  int n;
  dsched_rule_free(dsched, &dsched->added);
  dtos_strdup(ctl, day, &dsched->added.day);
  dtos_strdup(ctl, start, &dsched->added.start);
  dtos_strdup(ctl, end, &dsched->added.end);
  dtos_strdup(ctl, period, &dsched->added.period);
  dtos_strdup(ctl, duration, &dsched->added.duration);

  dsched_check(dsched, day, 
    start, end, period, duration, &ok);
  if (!ok)
  {
    return;
  }

  n = dsched->nrules++;
  dtos_strdup(ctl, day, &dsched->rule[n].day);
  dtos_strdup(ctl, start, &dsched->rule[n].start);
  dtos_strdup(ctl, end, &dsched->rule[n].end);
  dtos_strdup(ctl, period, &dsched->rule[n].period);
  dtos_strdup(ctl, duration, &dsched->rule[n].duration);

  strcpy(dsched->change, "added: ");
  dsched_natural(dsched, n,				/* get change 						*/
    dsched->change+
      strlen(dsched->change), 
    sizeof(dsched->change)-
      strlen(dsched->change));
}

/* ------------------------------------------------------------------------
 * ------------------------------------------------------------------------ */

void
DTCONFIG_API1
dsched_load(							/* load structure from database 	*/
  dsched_t *dsched,
  const char *database)
{
  FILE *file;
  char line[256];
  int n;
  if (DSCHED_ISERROR(dsched))
    return;
  file = dsched_open(dsched, 
    database, MODE_READ);
  if (file == NULL)
  {
    return;
  }
  memset(line, 0, sizeof(line));
  while (fgets(line, 
           sizeof(line), file))
  {
	int l = strlen(line);

	while (l &&
	  (line[l-1] == 0x0a ||				/* nuke CR's and LF's			    */
	   line[l-1] == 0x0d))
	  line[--l] = '\0';

	if (!strncmp(line,					/* stop reading rules at sentinel	*/
		  DSCHED_SENTINEL,
          strlen(line)))
	  break;
    dtstr_tokenize(NULL, line, ' ', &n);
	if (n != 5)
	  dsched_error(dsched, "Sorry, database file \"%s\" is mangled.",
        database);
	else
	{
	  char *day = line;
	  char *start = day + strlen(day) + 1;
	  char *end = start + strlen(start) + 1;
	  char *period = end + strlen(end) + 1;
	  char *duration = period + strlen(period) + 1;
	  dsched_add(dsched, day,
        start, end, period, duration);
	}
	memset(line, 0, sizeof(line));
  }
  fclose(file);
}

/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dsched_camlist(
  dt_ctl_t *ctl,
  const char *config,
  dtlist_t *list)
{
  DT_F("tabchief_camlist");
  char filename[256];
  int isdir, isreadable;
  dtfd_t _fd, *fd = &_fd;
  dtlist_elem_t *elem;
  dt_rc_e rc;

  fd->fd = -1;
										/* ................................ */
  if (config == NULL)
  {
	rc = dt_err(ctl, F,
      "Sorry, no config directory given.");
	goto cleanup;
  }

  DT_C(dtos_isdir,(ctl, config,			/* make sure directory exists 		*/
    &isdir));
  if (!isdir)
  {
	rc = dt_err(ctl, F,
      "Sorry, %s is not a directory.",
      config);
	goto cleanup;
  }
										/* ................................ */
  dtstr_printf(ctl, 
    filename, sizeof(filename),
    "%s/cam.lst", config);

  DT_Q(dtos_isreadable,(ctl,			/* make sure file exists 			*/
    filename, &isreadable));
  if (!isreadable)
  {
	rc = dt_err(ctl, F,
      "Sorry, %s is not a readable file.",
      filename);
	goto cleanup;
  }

										/* ................................ */
  DT_C(dtfd_open,(ctl, fd,
    filename, "r"));
  DT_C(dtliner_fd_list,(ctl, fd,
    "#;*", " \t", list));

  elem = DTLIST_ELEM_HEAD;
  while(1)
  {
	char * spigot;
	char *user;
	dsched_cam_t *cam;
	DT_C(dtlist_next,(ctl, list,		/* get next list element 			*/
      elem, &elem));
	if (elem == NULL)					/* end of list? 					*/
	  break;
	spigot = strchr(elem->name, ',');	/* find comma in camera name 		*/
	if (spigot != NULL)					/* camera name has a comma? 		*/
	  *spigot++ = '\0';					/* and camera name after comma		*/
	else
	  spigot = "";

	user = (char *)elem->user;			/* description string 				*/

	DT_C(dtos_malloc2,(ctl,				/* space for cam structure 			*/
      (void **)&cam, 
      sizeof(*cam)+strlen(user),
      F, "elem->user cam structure"));

	elem->user = (void *)cam;
	cam->spigot = spigot;				/* fill in cam structure 			*/
	strcpy(cam->description, user);

	DT_C(dtos_free2,(ctl, user,
      F, "old elem->user"));
  }
cleanup:
  if (fd->fd != -1)
    DT_I(dtfd_close,(ctl, fd));
	
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
