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
 * make -f qw1.mak SRC=/usr/erb/prj/dt OUT=/usr/erb/prj/dtqw1
 *..........................................................................*/
#include <dtack/t.h>

DT_RCSID("test $RCSfile: dttftp.c,v $ $Revision: 1.8 $");

#include <dtack/ipc.h>
#include <dtack/ftp.h>

static const char *ops[] = {
  "default", 
  "single",
  "multiple",
  NULL};

#define usage ""

/*..........................................................................*/

dt_rc_e 
DTCONFIG_API1
dttftp_test_single(
  dtparam_t *param,
  const char *control_spec,
  const char *data_spec,
  const char *user,
  const char *password)
{
  DT_F("dttftp_test_single");
  dt_ctl_t *ctl = &param->ctl;
  dtftp_t ftp;
  dt1d_ran_t ran;
  double *buffer = NULL;
  dtos_time_t mark1, mark2;
  long i;
  dt_rc_e rc;
  
  DT_C(dtos_malloc2,(ctl,				/* get space for buffer 			*/ 
    (void **)&buffer,
    DT_CAST(long, param->max),  
    F, "buffer"));

  DT_Q_1D(dt1d_ran_uniform_init,(		/* init random numbers			    */
    &ran, 0));
  DT_Q_1D(dt1d_random_uniform_double,(	/* compute random numbers		    */
    &ran, buffer,
    (long)param->max/sizeof(*buffer)));

  DT_C(dtftp_init,(ctl, &ftp,
    control_spec, data_spec,
    user, password));

  DT_C(dtftp_connect,(ctl, &ftp));
  DT_C(dtftp_chdir,(ctl, &ftp,			/* set the destination directory 	*/
    param->output));
  
  DT_C(dtos_time,(ctl, &mark1));		/* get time at start				*/
  for (i=0; i<param->n; i++)
  {
    DT_C(dtftp_put_start,(ctl, &ftp,	/* start the file putting process 	*/
      "tmp.ftp"));
    DT_C(dtftp_put_data,(ctl, &ftp,  
      buffer,
      DT_CAST(long, param->max)));
  }
  DT_C(dtftp_put_finish,(ctl, &ftp));
  DT_C(dtos_time,(ctl, &mark2));		/* get time at end 					*/
  dt_dbg(ctl, F, DT_DBG_MASK_USR1,
    "%g bytes/second",
    param->max/(mark2.wall-mark1.wall));

  DT_C(dtftp_disconnect,(ctl, &ftp));
cleanup:
  DT_I(dtftp_uninit,(ctl, &ftp));

  if (buffer != NULL)
    DT_I(dtos_free2,(ctl, buffer,
      F, "buffer"));
  
  return rc;
}

/*..........................................................................*/

dt_rc_e 
DTCONFIG_API1
dttftp_test_multiple(
  dtparam_t *param,
  const char *control_spec,
  const char *data_spec,
  const char *user,
  const char *password)
{
  DT_F("dttftp_test_multiple");
  dt_ctl_t *ctl = &param->ctl;
# define MAX_FTP (10)
  dtftp_t ftp[MAX_FTP];
  void *buffer = NULL;
  long l;
  int n = 0;
  int i;
  dt_rc_e rc;
  
  DT_C(dtos_malloc2,(ctl, &buffer,		/* get space for buffer 			*/
    DT_CAST(long, param->max),  
    F, "buffer"));

  while (strlen(data_spec))				/* initialize all connections 		*/
  {
	char *p = strchr(data_spec, '+');
	long l;
	char t[32];
	if (p == NULL)
	  l = strlen(data_spec);
	else
	  l = p - data_spec + 1;
	if ((unsigned long)l > sizeof(t))
	{
	  rc = dt_err(ctl, F,
        "element \"%s\" length %ld"
        " longer than max %d",
        data_spec, l, sizeof(t));
	}
	DT_MEMCPY(t, data_spec, (int)l);
	if (t[(int)l-1] == '+')
      t[(int)l-1] = '\0';
	else
	  t[(int)l] = '\0';

	if (n > MAX_FTP)
	{
	  rc = dt_err(ctl, F,
        "too many FTP connections,"
        " max is %d", MAX_FTP);
	}
    DT_C(dtftp_init,(ctl, &ftp[n++],	/* initialize ftp structure 		*/
      control_spec, t,
      user, password));
	data_spec += l;
  }

  for (i=0; i<n; i++)					/* open all connections 			*/
  {
	dt_dbg(ctl, F, DT_DBG_MASK_TIME,
      "connecting %d...", i);
	DT_C(dtftp_connect,(ctl, &ftp[i]));	/* log in 							*/
	DT_C(dtftp_chdir,(ctl, &ftp[i],		/* set the destination directory 	*/
      param->output));
  }

  for (i=0; i<param->n; i++)
  {
	int j = i % n;
    DT_C(dtftp_put_start,(ctl, &ftp[j],	/* start the file putting process 	*/
      "tmp.ftp"));
    DT_C(dtftp_put_data,(ctl, &ftp[j],  /* send the data 					*/
      buffer,
      DT_CAST(long, param->max)));
  }

  for (i=0; i<n; i++)					/* close all connections 			*/
  {
    DT_C(dtftp_put_finish,(ctl, 
      &ftp[i]));
  
    DT_C(dtftp_disconnect,(ctl, 
      &ftp[i]));
  }
  
cleanup:
  for (i=0; i<n; i++)
    DT_I(dtftp_uninit,(ctl, &ftp[i]));

  if (buffer != NULL)
    DT_I(dtos_free2,(ctl, buffer,
      F, "buffer"));
  
  return rc;
}

/*..........................................................................*/

dt_rc_e 
DTCONFIG_API1
dttftp_arg_check(						/* check args and params			*/
  dt_ctl_t *ctl,
  void *app,
  dtparam_t *param,
  dt_enter_arg_t *arg,
  char *msg,
  const char **start)
{
  DT_F("dttftp_arg_check");

  DT_Q(dtt_check_op,(ctl, param,		/* make sure op is understood	    */
    ops, msg, start));

  return DT_RC_GOOD;
}

/*..........................................................................
| NAME
| main program
| 
| END
 *..........................................................................*/

DTT_MAIN(dttftp)
{
  DT_F("dttftp");
  dt_ctl_t *ctl = &param->ctl;
  char control_spec[64];
  char data_spec[64];
  char user[64];
  char password[64];
  dt_enter_arg_t arg[5];
  int assert_fail = 0;					/* failed assertion count		    */
  dt_rc_e rc = DT_RC_GOOD;

  DT_GI(dt_heapmon_init,(ctl));			/* init memory checking			    */

  DT_GI(dtparam_init,(param));			/* init param structure             */

  DT_GI(dtparam_set,(param, 0,			/* set default for params we want	*/
    &param->op, NULL, NULL, "default",
    &param->reps, NULL, NULL, "1",
    &param->n, "times to send", NULL, "10",
    &param->max, "file size", NULL, "10000",
    &param->output, "remote directory", NULL, "/tmp",
    NULL));

  DT_GI(dt_enter_arg,(ctl,				/* set up positionals			    */
    arg,
    "Control spec", NULL,
      control_spec, 
      sizeof(control_spec),
    "Data spec", NULL,
      data_spec, 
      sizeof(data_spec),
    "User", NULL,
      user, 
      sizeof(user),
    "Password", NULL,
      password, 
      sizeof(password),
     NULL));
  
  DT_GI(dt_interactive_parse,(			/* parse command line interactively	*/
    argc, argv, F, usage, param,
    arg, dttftp_arg_check, NULL));

  while (rc == DT_RC_GOOD)
  {
	dtos_time_t mark1, mark2;
    long reps;
	
	DT_GI(dtos_time,(ctl, &mark1));		/* get time at start 				*/

    for (reps=0; 
         reps < param->reps && 
         rc == DT_RC_GOOD;
         reps++)
	{
#     define ISOP(OP) (DT_ABBREV(OP, \
        param->op))

	  if (ISOP("single") || ISOP("default"))
      DT_GI(dttftp_test_single,(param,
        control_spec, data_spec,
        user, password));

	  if (ISOP("multiple"))
      DT_GI(dttftp_test_multiple,(param,
        control_spec, data_spec,
        user, password));
	}

	DT_GI(dtos_time,(ctl, &mark2));		/* get time at end 					*/

	if (DT_DBG(ctl, DT_DBG_MASK_TIME))
	{
	  char id[32];
	  dtstr_printf(ctl, id, sizeof(id),
        "%s %s", param->op, "total");
	  DT_GI(dtt_time_report,(ctl,
        &mark1, &mark2, id));
	}

    DT_G(dt_interactive_after,(         /* make post display                */
      ctl, param, usage,
      arg, dttftp_arg_check, NULL, rc));
  }
										/* ................................ */

  DT_I(dt_interactive_leave,(			/* clean up before leaving			*/
    param, rc));

  DT_I(dt_heapmon_uninit,(ctl,			/* check all memory is freed	    */
    &assert_fail));

  if (rc == DT_RC_STOP)
    rc = DT_RC_GOOD;

  if (assert_fail)						/* any failed assertions?		    */
    return DT_RC_BAD;
  else
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
