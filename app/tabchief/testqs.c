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
DT_RCSID("app/tabchief $RCSfile: testqs.c,v $ $Revision: 1.8 $");

										/* ................................ */
static const char *strings0[] = {		/* very simple 						*/
"id=test",
"id=test&button-add=ADD&day=Monday&start=10%3A00&end=11%3A00&continuously=yes&period=&duration=",
"id=test&button-add=ADD&day=Tuesday&start=10%3A00&end=11%3A00&period=10&singleframe=yes&duration=",
"id=test&button-add=ADD&day=Wednesday&start=10%3A00&end=11%3A00&period=00%3A10&duration=0.10",
"id=test&button-add=ADD&day=Thursday&start=0&end=23%3A59&period=00%3A10&duration=00%3A00.10",
NULL 
};

static const char *results0[] = {
"Monday 10:00 11:00 00:00 00:00",
"Tuesday 10:00 11:00 00:10 00:00",
"Wednesday 10:00 11:00 00:10 00:00.10",
"Thursday 00:00 23:59 00:10 00:00.10",
"end of rules",
NULL
};
										/* ................................ */
static const char *strings1[] = {		/* lots of adding and deleting 		*/
"id=test",
"id=test&button-add=ADD&day=Sunday&start=&end=&period=&duration=",
"id=test&button-add=ADD&day=Sunday&start=3%3A00&end=4%3A00&continuously=yes&period=&duration=",
"id=test&button-add=ADD&day=Friday&start=03%3A00&end=04%3A00&continuously=yes&period=&duration=",
"id=test&button-add=ADD&day=Wednesday&start=03%3A00&end=04%3A00&continuously=yes&period=&duration=",
"id=test&day=Wednesday&start=03%3A00&end=04%3A00&continuously=yes&period=&duration=&button-01=DEL",
"id=test&button-add=ADD&day=Wednesday&start=03%3A00&end=04%3A00&continuously=yes&period=&duration=",
"id=test&button-add=ADD&day=Monday&start=03%3A00&end=04%3A00&continuously=yes&period=&duration=",
"id=test&day=Monday&start=03%3A00&end=04%3A00&continuously=yes&period=&duration=&button-01=DEL",
"id=test&day=Friday&start=03%3A00&end=04%3A00&continuously=yes&period=&duration=&button-01=DEL",
"id=test&button-add=ADD&day=Wednesday&start=03%3A00&end=04%3A00&continuously=yes&period=&duration=",
"id=test&button-add=ADD&day=Saturday&start=03%3A00&end=04%3A00&continuously=yes&period=&duration=",
"id=test&button-add=ADD&day=Tuesday&start=03%3A00&end=04%3A00&continuously=yes&period=&duration=",
"id=test&button-add=ADD&day=Saturday&start=03%3A00&end=04%3A00&continuously=yes&period=&duration=",
"id=test&button-add=ADD&day=Saturday&start=03%3A00&end=04%3A00&continuously=yes&period=&duration=",
"id=test&day=Saturday&start=03%3A00&end=04%3A00&continuously=yes&period=&duration=&button-05=DEL",
"id=test&day=Saturday&start=03%3A00&end=04%3A00&continuously=yes&period=&duration=&button-01=DEL",
"id=test&day=Saturday&start=03%3A00&end=04%3A00&continuously=yes&period=&duration=&button-03=DEL",
"id=test&button-add=ADD&day=Monday&start=03%3A00&end=04%3A00&continuously=yes&period=&duration=",
"id=test&button-add=ADD&day=Thursday&start=03%3A00&end=04%3A00&continuously=yes&period=&duration=",
"id=test&day=Thursday&start=03%3A00&end=04%3A00&continuously=yes&period=&duration=&button-01=DEL",
"id=test&button-add=ADD&day=Monday&start=03%3A00&end=04%3A00&continuously=yes&period=&duration=",
"id=test&day=Monday&start=03%3A00&end=04%3A00&continuously=yes&period=&duration=&button-02=DEL",
"id=test&button-add=ADD&day=Wednesday&start=03%3A00&end=04%3A00&continuously=yes&period=&duration=",
NULL
};

static const char *results1[] = {
"Sunday 03:00 04:00 00:00 00:00",
"Monday 03:00 04:00 00:00 00:00",
"Wednesday 03:00 04:00 00:00 00:00",
"Thursday 03:00 04:00 00:00 00:00",
"Friday 03:00 04:00 00:00 00:00",
"Saturday 03:00 04:00 00:00 00:00",
"end of rules",
NULL
};
										/* ................................ */
static const char *strings2[] = {		/* kooky url lines					*/
"id=test&",
"id=test&&",
"id=test",
"id=tes",
"",
"",
"",
"",
"",
"id=test",
"id=test&&",
"id=test&&&&",
"id=test&button-add=ADD&day=Sunday&start=&end=&period=&duration=",
"id=test&butxton-add=ADD&day=Sunday&start=&end=&period=&duration=",
"id=test&button-add=ADD&day=Suxnday&start=&end=&period=&duration=",
"id=test&button-add=ADD&day=Sunday&start=&end=",
"id=test&button-add=ADD&day=Sunday&start=&end=",
"id=test&button-add=ADD&day=Sunday&start=&end=333333333333&period=&duration=",
"id=test&button-add=ADD&day=Sunday&start=999999999999999999&end=333333333333&period=&duration=",
"id=test&button-add=ADD&day=Sunday&start=99999&end=333333333333&period=&duration=",
"id=test&button-add=ADD&day=Sunday&start=59&end=333333333333&period=&duration=",
"id=test&button-add=ADD&day=Sunday&start=59&end33&period=&duration=xxxxxxxxxxxxxxxxxxxxxxxxxxxxx",
"id=test&button-add=ADD&day=Sunday&start=59&end=33&period=&duration=xxxxxxxxxxxxxxxxxxxxxxxxxxxxx",
"id=test&button-add=ADD&day=Sunday&start=29&end=33&period=&duration=xxxxxxxxxxxxxxxxxxxxxxxxxxxxx",
"id=test&button-add=ADD&day=Sunday&start=29&end=33&period=&duration=%3A%",
"id=test&button-add=ADD&day=Sunday&start=29&end=33&period=&duration=3",
"id=test&button-add=ADD&day=Sunday&start=00%3A29&end=00%3A33&continuously=yes&period=&duration=00%3A03",
"id=test&button-add=ADD&day=Sunday&start=00%3A29&end=00%3A%333&continuously=yes&period=&duration=00%3A03",
NULL
};

static const char *results2[] = {
"Sunday 00:29 00:33 00:00 00:00",
"end of rules",
NULL
};

/*..........................................................................
 *..........................................................................*/
static
dt_rc_e
tabchief_test_qslocal_one(
  dtparam_t *param,
  const char *id,
  const char **strings,
  const char **results)
{
  DT_F("tabchief_test_qslocal");
  dt_ctl_t *ctl = &param->ctl;
  dtfd_t _fd, *fd = &_fd;
  char filename[80];
  int i;
  dt_rc_e rc;

  fd->fd = -1;

  DT_C(dsched_database,(param, id,		/* make database filename 			*/
    filename, sizeof(filename)));
  DT_C(dtfd_open,(ctl, fd,				/* open and clear database			*/
    filename, "w"));
  DT_C(dtfd_write_all,(ctl, fd,
    DSCHED_SENTINEL "\n",
    strlen(DSCHED_SENTINEL "\n")));
  DT_C(dtfd_close,(ctl, fd));

  for (i=0; strings[i]; i++)			/* feed scheduler the input strings */
  {
    DT_C(dsched,(param, NULL,
      "schedule", strings[i]));
  }

  DT_C(dtfd_open,(ctl, fd,				/* open database filename to read	*/
    filename, "r"));
  for (i=0; results[i]; i++)			/* loop through expected results 	*/
  {
	char line[80];
	DT_C(dtfd_read_line,(ctl, fd,
      line, sizeof(line)));
	if (line[strlen(line)-1] == '\n')
	  line[strlen(line)-1] = '\0';
	dt_assert(ctl,
      !strcmp(line, results[i]),
      F, "%d got \"%s\", expected \"%s\"",
      i, line, results[i]);
  }
cleanup:
  if (fd->fd != -1)
    DT_I(dtfd_delete,(ctl, fd));		/* close and delete database 		*/
  return rc;
}

/*..........................................................................
 * -input is local temporary filename
 * -config is remote directory where tabchief server gets config
 *..........................................................................*/
static
dt_rc_e
tabchief_test_qsremote_one(
  dtparam_t *param,
  const char *ipcspec,
  const char *id,
  const char **strings,
  const char **results)
{
  DT_F("tabchief_test_qsremote_one");
  dt_ctl_t *ctl = &param->ctl;
  dtfd_t _fd, *fd = &_fd;
  char filename[80];
  char s[256];
  dtparam_t tparam = *param;
  int i;
  dt_rc_e rc;

  fd->fd = -1;

  DT_C(dtfd_open,(ctl, fd,				/* open and clear cam file			*/
    param->input, "w"));
  DT_C(dtfd_write_all,(ctl, fd,
    DSCHED_SENTINEL "\n",
    strlen(DSCHED_SENTINEL "\n")));
  DT_C(dtfd_close,(ctl, fd));

  DT_C(dsched_database,(param, id,		/* make remote database filename	*/
    filename, sizeof(filename)));

  tparam.output = filename;
  tparam.output_flag |= DTPARAM_FLAG_VALUE;
  tparam.input_flag |= DTPARAM_FLAG_VALUE;

  DT_C(tabchief_client,(				/* copy empty file to server		*/
    &tparam, ipcspec, "copyto"));

  for (i=0; strings[i]; i++)			/* feed scheduler the input strings */
  {
	strcpy(s, "GET /schedule?");
	strcat(s, strings[i]);
	strcat(s, " HTTP/1.0");
	DT_C(tabchief_client,(		
      param, ipcspec, s));
  }

  tparam.input = filename;				/* reverse names for copy back		*/
  tparam.output = param->input;

  DT_C(tabchief_client,(				/* copy file from server 			*/
    &tparam, ipcspec, "copyfrom"));

  DT_C(dtfd_open,(ctl, fd,				/* open database filename to read	*/
    tparam.output, "r"));
  for (i=0; results[i]; i++)			/* loop through expected results 	*/
  {
	DT_C(dtfd_read_line,(ctl, fd,
      s, sizeof(s)));
	if (s[strlen(s)-1] == '\n')
	  s[strlen(s)-1] = '\0';
	dt_assert(ctl,
      !strcmp(s, results[i]),
      F, "%d got \"%s\", expected \"%s\"",
      i, s, results[i]);
  }
cleanup:
  if (fd->fd != -1)
    DT_I(dtfd_delete,(ctl, fd));		/* close and delete local tmp file	*/
  return rc;
}

/*..........................................................................
 *..........................................................................*/
dt_rc_e
DTCONFIG_API1
tabchief_test_qslocal(
  dtparam_t *param)
{
  DT_F("tabchief_test_qslocal");
  dt_ctl_t *ctl = &param->ctl;
  DT_Q(tabchief_test_qslocal_one,(param, "test", strings0, results0));
  DT_Q(tabchief_test_qslocal_one,(param, "test", strings1, results1));
  DT_Q(tabchief_test_qslocal_one,(param, "test", strings2, results2));
  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/
dt_rc_e
DTCONFIG_API1
tabchief_test_qsremote(
  dtparam_t *param,
  const char *ipcspec)
{
  DT_F("tabchief_test_qsremote");
  dt_ctl_t *ctl = &param->ctl;
  DT_Q(tabchief_test_qsremote_one,(param, ipcspec, "test", strings0, results0));
  DT_Q(tabchief_test_qsremote_one,(param, ipcspec, "test", strings1, results1));
  DT_Q(tabchief_test_qsremote_one,(param, ipcspec, "test", strings2, results2));
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
