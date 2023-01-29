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
DT_RCSID("app/tabchief $RCSfile: dsched.c,v $ $Revision: 1.8 $");

#ifdef DSCHED_WINDOWS
#  include <windows.h>
#endif

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
static
void
dsched_del(								/* delete rule from structure 		*/
  dsched_t *dsched,
  int rule)
{
  if (dsched->rule[rule].day == NULL)	/* rule is inactive?				*/
    return;

  strcpy(dsched->change, "deleted: ");
  dsched_natural(dsched, rule,			/* get change 						*/
    dsched->change+
      strlen(dsched->change), 
    sizeof(dsched->change)-
      strlen(dsched->change));
  dsched_rule_free(dsched, &dsched->rule[rule]);
}

#ifdef NONO

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

static
void
dsched_minutes_to_seconds(					/* convert NNN to seconds  		*/
  dsched_t *dsched,
  const char *string,
  int *seconds)
{
  char *e;
  double d;

  if (DSCHED_ISERROR(dsched))
    return;

  d = strtod(string, &e);
  if (*e != '\0')
    dsched_error(dsched, 
      "Sorry, \"%s\" is not a valid number.",
      string);

  *seconds = (int)(d * 60.0);
}
#endif

/* ------------------------------------------------------------------------
 * ------------------------------------------------------------------------ */
static
void
dsched_save(							/* save structure to database 		*/
  dsched_t *dsched,
  const char *database)
{
  FILE *datfile;
  int day;
  int i;
  char t[64];
  if (DSCHED_ISERROR(dsched))
    return;
  datfile = dsched_open(dsched, 
    database, MODE_WRITE);
  if (DSCHED_ISERROR(dsched))
    return;

  for (day=0; day<7; day++)				/* give rules in weekday order 		*/
  for (i=0; i<DSCHED_RULES; i++)
  if (dsched->rule[i].day != NULL &&	/* rule is active? 					*/
	  !strcmp(dsched->rule[i].day,		/* rule is for this day?			*/
         dsched_days[day]))
  {
	fprintf(datfile, "%s ", dsched->rule[i].day); 
	fprintf(datfile, "%s ",
      dsched_natural_expand(dsched, dsched->rule[i].start, t));
	fprintf(datfile, "%s ",
      dsched_natural_expand(dsched, dsched->rule[i].end, t));
	fprintf(datfile, "%s ",
      dsched_natural_expand(dsched, dsched->rule[i].period, t));
	fprintf(datfile, "%s\n",
      dsched_natural_expand(dsched, dsched->rule[i].duration, t));
  }
  
#ifdef NONO
  fprintf(datfile, "bitmap\n");
  for (day=0; day<7; day++)				/* give bitmaps						*/
  {
	for (i=0; 
		 i<sizeof(dsched->bitmap[day]); 
		 i++)
  	  fprintf(datfile, "%02x",
        dsched->bitmap[day][i]);
    fprintf(datfile, "\n");			  
  }
#endif

  fprintf(datfile, "%s\n", DSCHED_SENTINEL);

  fclose(datfile);
}

/* ------------------------------------------------------------------------
 * ------------------------------------------------------------------------ */
static
void
dsched_html(							/* emit html 						*/
  dsched_t *dsched)
{
/*DT_F("dsched_html");*/
  dtparam_t *param = dsched->param;
  dt_ctl_t *ctl = &dsched->param->ctl;
  dtipc_t *ipc = dsched->ipc;
  int i, n;
  int day;
  char t[64];
  int period = -1;
  int duration = -1;

  tabchief_reply(param, ipc, 
    "Express all times as hours:minutes.seconds\n<p>\n");

  tabchief_reply(param, ipc, 
    "<form action=\"/%s\" method=GET>\n", 
    dsched->form);

  tabchief_reply(param, ipc, "for camera"
    "<input size=16 name=\"id\""
    " value=\"%s\" type=\"text\"><br>\n",
    dsched->id);

  tabchief_reply(param, ipc, 
    "<input type=\"submit\""
    " name=\"button-add\""
    " value=\"ADD\">\n");

  tabchief_reply(param, ipc, 
    "<select name=\"day\""
    " size=1>\n");

  for (i=0; i<7; i++)
    tabchief_reply(param, ipc, 
      "<option value=\"%s\"%s>%s\n",
      dsched_days[i], 
      !strcmp(dsched_days[i], dsched->added.day)? 
        " SELECTED": "",
      dsched_days[i]);

  tabchief_reply(param, ipc, "</select>\n");

  tabchief_reply(param, ipc, "at"
    "<input size=8 name=\"start\""
    " value=\"%s\" type=\"text\">\n",
    dsched_natural_expand(dsched, 
      dsched->added.start, t));

  tabchief_reply(param, ipc, "until"
    "<input size=8 name=\"end\""
    " value=\"%s\" type=\"text\"> give as 24-hour clock\n<br>\n",
    dsched_natural_expand(dsched, 
      dsched->added.end, t));
										/* ................................ */
  if (dsched->added.period[0])
  dsched_time_to_seconds(dsched,		/* convert period to seconds		*/
    dsched->added.period, 
	&period);

  tabchief_reply(param, ipc, 
    "<input type=\"checkbox\""
    " name=\"continuously\""
    " value=\"yes\"%s>continuously or every\n",
    period == 0? " CHECKED": "");

  tabchief_reply(param, ipc, 
    "<input size=8 name=\"period\""
    " value=\"%s\" type=\"text\">\n<br>\n",
    period == 0? "": 
      dsched_natural_expand(dsched, 
        dsched->added.period, t));
										/* ................................ */
  if (dsched->added.duration[0])
  dsched_time_to_seconds(dsched,		/* convert duration to seconds		*/
    dsched->added.duration, 
	&duration);
  
  tabchief_reply(param, ipc, 
    "<input type=\"checkbox\""
    " name=\"singleframe\""
    " value=\"yes\"%s>single frame or lingering for\n",
    period != 0 && duration == 0?
      " CHECKED": "");

  tabchief_reply(param, ipc, 
    "<input size=8 name=\"duration\""
    " value=\"%s\" type=\"text\">\n",
    period == 0 || duration == 0?
      "": dsched_natural_expand(dsched, 
        dsched->added.duration, t));
  
										/* ................................ */
  if (dsched->error[0])
  {
    tabchief_reply(param, ipc, 
      "<p><b><font size=+2>"
      "%s"
      "</font></b><p>\n",
      dsched->error);
	dt_dbg(ctl, NULL, DT_DBG_MASK_USR1,
      "%s", dsched->error);
  }
  else
  if (dsched->change[0])
  {
    tabchief_reply(param, ipc, 
      "<p><b><font size=+2>"
      "%s"
      "</font></b><p>\n",
      dsched->change);
	dt_dbg(ctl, NULL, DT_DBG_MASK_USR1,
      "%s", dsched->change);
  }
										/* ................................ */
  n = 0;
  for (day=0; day<7; day++)				/* give rules in weekday order 		*/
  for (i=0; i<DSCHED_RULES; i++)
  if (dsched->rule[i].day != NULL &&	/* rule is active? 					*/
	  !strcmp(dsched->rule[i].day,		/* rule is for this day?			*/
         dsched_days[day]))
  {
	char natural[128];
	if (n == 0)
	  tabchief_reply(param, ipc, 
        "<p><hr><p>Current schedule:\n");
	dsched_natural(dsched, i,
      natural, sizeof(natural));
	tabchief_reply(param, ipc, 
      "<br><input type=\"submit\""
      " name=\"button-%02d\""
      " value=\"DEL\">%s\n",
      n, natural);
	n++;
  }
  if (n == 0)
    tabchief_reply(param, ipc, 
      "<p><hr><p>No current operating schedule.<p>\n");
  
  tabchief_reply(param, ipc, 
    "</form>\n");
  
}

/* ------------------------------------------------------------------------
 * ------------------------------------------------------------------------ */
dt_rc_e
DTCONFIG_API1
dsched(
  dtparam_t *param,
  dtipc_t *ipc,
  const char *command,
  const char *arg)
{
  DT_F("dsched");
  dt_ctl_t *ctl = &param->ctl;
  char database[128];
  char *query_string;
  char *p;
  char *id = "";
  char *button = "nop=NOP";
  char *day = "";
  char *start = "";
  char *end = "";
  char *period = "";
  char *continuously = "";
  char *duration = "";
  char *singleframe = "";
  dsched_t dsched;
  int i, n;
  dt_rc_e rc;

  DT_C(dtos_strdup,(ctl, arg,
    &query_string));

  dt_dbg(ctl, F, DT_DBG_MASK_ARG,
    "%s", query_string);

  dsched_init(&dsched, param, ipc);		/* initialize structure 			*/

  p = query_string? query_string: "";
  n = dsched_split(p, '&');				/* parse CGI input into tokens 		*/
  for (i=0; i<n; i++)					/* for each token 					*/
  {
	DSCHED_GET(id);
	DSCHED_GET(button);
	DSCHED_GET(day);
	DSCHED_GET(start);
	DSCHED_GET(end);
	DSCHED_GET(continuously);
	DSCHED_GET(period);
	DSCHED_GET(singleframe);
	DSCHED_GET(duration);
    p = p + strlen(p) + 1;				/* advance to next token 			*/
  }

  if (!strcmp(continuously, "yes"))
  {
    period = "0";
    duration = "0";
  }
  if (!strcmp(singleframe, "yes"))
    duration = "0";

  dsched_database(param, id,			/* make database filename 			*/
    database, sizeof(database));

  dsched.id = id;
  dsched.form = command;

  dsched_load(&dsched, database);		/* load database 					*/

  if (DSCHED_ACTION(button) ==			/* action is to add? 				*/
	  DSCHED_ACTION_ADD)
    dsched_add(&dsched, day,			/* add new rule 					*/
      start, end, period, duration);
  else		
  if (DSCHED_ACTION(button) ==			/* action is no-op? 				*/
	  DSCHED_ACTION_NOP)
    dsched.change[0] = '\0';
  else		
  if (DSCHED_ACTION(button) !=			/* action is not no-op? 			*/
	  DSCHED_ACTION_NOP)
    dsched_del(&dsched,					/* delete the rule 					*/
      DSCHED_ACTION(button));
  
  dsched_save(&dsched, database);		/* save database 					*/

  dsched_html(&dsched);					/* emit html 						*/

cleanup:
  dsched_uninit(&dsched);

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
