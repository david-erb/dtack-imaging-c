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
| DESCRIPTION
| As yet undocumented.
| 
| RETURN VALUES
| All functions Return DTOS_RANWELL on success.  
| Any other return value indicates failure.
| 
| ERRORS
| As yet undocumented.
| 
| END
 *..........................................................................*/

#include <dtack/t.h>
DT_RCSID("test $RCSfile: dttxchg.c,v $ $Revision: 1.1 $");
#include <dtack/lut8.h>
#include <dtack/xchg.h>
#include <dtack/prj.h>

/*..........................................................................
 * run "dtwin_which" the old way
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dttxchg_which_ok(
  dt_ctl_t *ctl,
  const char *spec,
  dtxchg_e *which,
  char **server,
  char **id,
  const char *F)
{
  dt_err_t err;
  char message[DT_ERR_LENGTH];
  dt_rc_e rc;
  DT_Q(dtxchg_parse,(ctl,				/* parse to get the id				*/
    spec, which,
    server, id, 
    &err, message, sizeof(message)));
  if (err != DT_ERR_NONE)
	return dt_err(ctl, F,
      "%s", message);
  return DT_RC_GOOD;
}
  
/*..........................................................................
 * run "dtwin_which" tests
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dttxchg_which(
  dt_ctl_t *ctl,
  int *assert_fail)
{
  DT_F("dttxchg_which");
  char *name, *server, *id;
  const char *win = ctl->win;
  dtxchg_e which;
# define W(COND) \
  { \
    char t[32]; \
    sprintf(t, "\"%-20s", name); t[strlen(name)+1] = '"'; t[20] = '\0'; \
    *assert_fail += DT_RC_GOOD == \
    dt_assert(ctl, COND, t, \
      "\"%s\" \"%s\" \"%s\"", \
      dtxchg_choices[which], \
      server, \
      id); \
  }
# define WFREE \
  DT_Q(dtos_free2,(ctl, server, F, "server")); \
  DT_Q(dtos_free2,(ctl, id, F, "id"))

  name = "bare_name";
  DT_Q(dttxchg_which_ok,(ctl, name, &which, &server, &id, F));
  W(!strcmp(dtxchg_choices[which], "pnm") &&
    !strcmp(server, "") &&
    !strcmp(id, "bare_name"));
  WFREE;

  name = "/img/tst/tmp.x";
  DT_Q(dttxchg_which_ok,(ctl, name, &which, &server, &id, F));
  W(!strcmp(dtxchg_choices[which], "pnm") &&
    !strcmp(server, "") &&
    !strcmp(id, "/img/tst/tmp.x"));
  WFREE;

  name = "pnm,/aaa,/bbb/xx.yy";
  DT_Q(dttxchg_which_ok,(ctl, name, &which, &server, &id, F));
  W(!strcmp(dtxchg_choices[which], "pnm") &&
    !strcmp(server, "/aaa") &&
    !strcmp(id, "/bbb/xx.yy"));
  WFREE;

  name = "win,,abcd";
  DT_Q(dttxchg_which_ok,(ctl, name, &which, &server, &id, F));
  W(!strcmp(dtxchg_choices[which], "win") &&
    !strcmp(server, "") &&
    !strcmp(id, "abcd"));
  WFREE;

										/* ................................ */
  name = "win,ipc,socket,a";
  DT_Q(dttxchg_which_ok,(ctl, name,
    &which, &server, &id, F));
  W(!strcmp(dtxchg_choices[which], "win") &&
    !strcmp(server, "ipc,socket") &&
    !strcmp(id, "a"));
  WFREE;

  name = "win,ipc,socket,";				/* win you don't want to give port */
  DT_Q(dttxchg_which_ok,(ctl, name,
    &which, &server, &id, F));
  W(!strcmp(dtxchg_choices[which], "win") &&
    !strcmp(server, "ipc,socket") &&
    !strcmp(id, ""));
  WFREE;

  name = "win,ipc,socket";				/* this would be invalid 			*/
  DT_Q(dttxchg_which_ok,(ctl, name,
    &which, &server, &id, F));
  W(!strcmp(dtxchg_choices[which], "win") &&
    !strcmp(server, "ipc") &&
    !strcmp(id, "socket"));
  WFREE;

  name = "win,ipc,socket,a";
  DT_Q(dttxchg_which_ok,(ctl, name,
    &which, &server, &id, F));
  W(!strcmp(dtxchg_choices[which], "win") &&
    !strcmp(server, "ipc,socket") &&
    !strcmp(id, "a"));
  WFREE;
										/* ................................ */
  name = "win,/tmp,abcd";
  DT_Q(dttxchg_which_ok,(ctl, name, &which, &server, &id, F));
  W(!strcmp(dtxchg_choices[which], "win") &&
    !strcmp(server, "/tmp") &&
    !strcmp(id, "abcd"));
  WFREE;

  name = "win,abcd";
  DT_Q(dttxchg_which_ok,(ctl, name, &which, &server, &id, F));
  W(!strcmp(dtxchg_choices[which], "win") &&
    !strcmp(server, "") &&
    !strcmp(id, "abcd"));
  WFREE;

  name = "win,x,y,z,abcd";
  DT_Q(dttxchg_which_ok,(ctl, name, &which, &server, &id, F));
  W(!strcmp(dtxchg_choices[which], "win") &&
    !strcmp(server, "x,y,z") &&
    !strcmp(id, "abcd"));
  WFREE;

  name = "dtm,:5709,my title";
  DT_Q(dttxchg_which_ok,(ctl, name, &which, &server, &id, F));
  W(!strcmp(dtxchg_choices[which], "dtm") &&
    !strcmp(server, ":5709") &&
    !strcmp(id, "my title"));
  WFREE;

  name = ":abcd1";
  ctl->win = "win";
  DT_Q(dttxchg_which_ok,(ctl, name, &which, &server, &id, F));
  W(!strcmp(dtxchg_choices[which], "win") &&
    !strcmp(server, "") &&
    !strcmp(id, "abcd1"));
  WFREE;

  name = ":abcd2";
  ctl->win = "win,x,y,z";
  DT_Q(dttxchg_which_ok,(ctl, name, &which, &server, &id, F));
  W(!strcmp(dtxchg_choices[which], "win") &&
    !strcmp(server, "x,y,z") &&
    !strcmp(id, "abcd2"));
  WFREE;

  name = ":";
  ctl->win = "win,ipc,qnx4,dtwin";
  DT_Q(dttxchg_which_ok,(ctl, name, &which, &server, &id, F));
  W(!strcmp(dtxchg_choices[which], "win") &&
    !strcmp(server, "ipc,qnx4,dtwin") &&
    !strcmp(id, ""));
  WFREE;

  name = ":a";
  ctl->win = "win,ipc,socket,berr:5701";
  DT_Q(dttxchg_which_ok,(ctl, name, &which, &server, &id, F));
  W(!strcmp(dtxchg_choices[which], "win") &&
    !strcmp(server, "ipc,socket,berr:5701") &&
    !strcmp(id, "a"));
  WFREE;

  ctl->win = win;						/* replace original value 			*/

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
