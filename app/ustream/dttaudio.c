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
#include <dtaudio.h>

DT_RCSID("test $RCSfile: dttaudio.c,v $ $Revision: 1.8 $");

static const char *ops[] = {"all",
  "play", 
  "record", 
  NULL};

typedef struct {						/* application structure			*/
  int op;
} app_t;

#define usage ""

#define PARAM_CHECK(WHICH, WANTED) \
{ \
  DT_Q(dttaudio_param,(param, \
     param->WHICH, \
     param->WHICH ## _desc, \
     param->WHICH ## _flag, \
     WANTED, message, DT_ENTER_MSG_MAXLEN)); \
   if (message[0]) \
   { \
	 *start = param->op_desc; \
	 return DT_RC_GOOD; \
   } \
 }

/*..........................................................................*/

static
dt_rc_e 
dttaudio_play(							/* play an input file 				*/
  dt_ctl_t *ctl,
  dtaudio_t *audio,
  const char *filename)
{
  DT_F("dttaudio_play");
  dtfd_t _fd, *fd = &_fd;
  void *buffer = NULL;
# define BUFSIZE (2000)
  long got;
  dt_rc_e rc = DT_RC_GOOD;

  fd->fd = -1;
  DT_C(dtfd_open,(ctl, fd,				/* open input file 					*/
    filename, "r"));
  DT_C(dtos_malloc2,(ctl,				/* allocate space for buffer 		*/
    &buffer, BUFSIZE,
    F, "buffer"));
  while(1)								/* until end of file 				*/
  {
	DT_C(dtfd_read,(ctl, fd,			/* read from file 					*/
      buffer, BUFSIZE, &got));
	if (got == 0)						/* end of file? 					*/
	  break;
	DT_C(dtaudio_write,(ctl, audio,		/* write to audio device 			*/
      buffer, got));
  }
cleanup:
  if (buffer != NULL)					/* buffer got allocated? 			*/
    DT_I(dtos_free2,(ctl, buffer,
      F, "buffer"));
  if (fd->fd != -1)						/* file got opened? 				*/
    DT_I(dtfd_close,(ctl, fd));

  return rc;
}

/*..........................................................................*/

static
dt_rc_e 
dttaudio_record(						/* record an output file			*/
  dt_ctl_t *ctl,
  dtaudio_t *audio,
  const char *filename,
  long limit)
{
  DT_F("dttaudio_record");
  dtfd_t _fd, *fd = &_fd;
  void *buffer = NULL;
# define BUFSIZE (2000)
  long total = 0;
  dt_rc_e rc = DT_RC_GOOD;

  fd->fd = -1;
  DT_C(dtfd_open,(ctl, fd,				/* open output file					*/
    filename, "w"));
  DT_C(dtos_malloc2,(ctl,				/* allocate space for buffer 		*/
    &buffer, BUFSIZE,
    F, "buffer"));
  while(1)								/* until end of file 				*/
  {
	DT_C(dtaudio_read,(ctl, audio,		/* read from audio device 			*/
      buffer, BUFSIZE));
	DT_C(dtfd_write_all,(ctl, fd,		/* write to file 					*/
      buffer, BUFSIZE));
	total += BUFSIZE;
	if (total >= limit)
	  break;
  }
cleanup:
  if (buffer != NULL)					/* buffer got allocated? 			*/
    DT_I(dtos_free2,(ctl, buffer,
      F, "buffer"));
  if (fd->fd != -1)						/* file got opened? 				*/
    DT_I(dtfd_close,(ctl, fd));

  return rc;
}

/*..........................................................................
 *..........................................................................*/

static
dt_rc_e 
dttaudio_param(
  dtparam_t *param,
  const char *value,
  const char *desc,
  dtparam_flag_t flags,
  int wanted,
  char *message,
  int mmax)
{
  DT_F("dttaudio_param");
  dt_ctl_t *ctl = &param->ctl;
  if (flags & DTPARAM_FLAG_VALUE)
  {
  	if (!wanted)
  	  DT_Q(dtstr_printf,(ctl, message, mmax,
        "%s is not allowed for the %s operation.",
        desc, param->op));
  }
  else
  	if (wanted)
  	  DT_Q(dtstr_printf,(ctl, message, mmax,
        "%s is required for the %s operation.",
        desc, param->op));

  return DT_RC_GOOD;
}

/*..........................................................................*/

static
dt_rc_e 
DTCONFIG_API1
dttaudio_arg_check(						/* check args and params			*/
  dt_ctl_t *ctl,
  void *vapp,
  dtparam_t *param,
  dt_enter_arg_t *arg,
  char *message,
  const char **start)
{
  DT_F("dttaudio_arg_check");
  app_t *app = (app_t *)vapp;

  if (param->op == NULL ||
	  strlen(param->op) == 0)
  {
	dtstr_printf(ctl, 
      message, DT_ENTER_MSG_MAXLEN,
      "Please enter something"
      " for \"%s\"",
      param->op_desc);
    *start = param->op_desc;
	return DT_RC_GOOD;
  }

  DT_Q(dtstr_choice2,(ctl, ops,
    param->op, &app->op,
    message, DT_ENTER_MSG_MAXLEN));
  if (message[0])
  {
    *start = param->op_desc;
	return DT_RC_GOOD;
  }

  if (!strcmp(ops[app->op], "play"))	/* be the ipc server				*/
  {
	PARAM_CHECK(input, 1);
	PARAM_CHECK(output, 0);
  }

  if (!strcmp(ops[app->op], "record"))	/* send keyboard commands to server	*/
  {
	PARAM_CHECK(input, 0);
	PARAM_CHECK(output, 1);
  }

  return DT_RC_GOOD;
}

/*..........................................................................
| NAME
| main program
| 
| END
 *..........................................................................*/

DTT_MAIN(dttaudio)
{
  DT_F("dttaudio");
  dt_ctl_t *ctl = &param->ctl;
  char spec[64];
  dt_enter_arg_t arg[2];
  app_t app;
  int assert_fail = 0;					/* failed assertion count		    */
  dtaudio_t audio;
  int audio_initialized = 0;
  dt_rc_e rc = DT_RC_GOOD;

  DT_C(dt_heapmon_init,(ctl));			/* init memory checking			    */

  DT_C(dtparam_init,(param));			/* init param structure             */

  DT_C(dtparam_set,(param, 0,			/* set default for params we want	*/
    &param->op, NULL, NULL, "play",
    &param->reps, NULL, NULL, "1",
    &param->input, NULL, NULL, "",
    &param->output, NULL, NULL, "",
    &param->n, "Bytes to record", NULL, "30000",
    &param->k, "Volume level", NULL, "-1",
    NULL));

  DT_C(dt_enter_arg,(ctl,				/* set up positionals			    */
    arg,
    "Audio spec", NULL,
      spec, sizeof(spec),
    NULL));
  
  DT_C(dt_enter_parse,(					/* parse command line interactively	*/
    argc, argv, F, usage, param,
    arg, dttaudio_arg_check, &app));
  
  while (rc == DT_RC_GOOD)
  {
	dtos_time_t mark1, mark2;
    long reps;
	
	DT_C(dtos_time,(ctl, &mark1));		/* get time at start 				*/

    for (reps=0; 
         reps < param->reps && 
         rc == DT_RC_GOOD;
         reps++)
	{
#     define ISOP(OP) (DT_ABBREV(OP, \
        param->op))

      if (ISOP("play"))
      {
		DT_C(dtaudio_init,(ctl, &audio, /* initialize audio device 			*/
          spec, DTAUDIO_MODE_WRITE, -1));
		audio_initialized = 1;
		if (param->k != -1)
  		  DT_C(dtaudio_set_volume,(ctl,	/* set playback volume 				*/
            &audio, (int)param->k));
		DT_C(dttaudio_play,(ctl,		/* play from disk file				*/
          &audio,
          param->input));
      }
	  else
      if (ISOP("record"))
      {
		DT_C(dtaudio_init,(ctl, &audio, /* initialize audio device 			*/
          spec, DTAUDIO_MODE_READ, -1));
		audio_initialized = 1;
		if (param->k != -1)
		  DT_C(dtaudio_set_volume,(ctl,	/* set recording volume 			*/
            &audio, (int)param->k));
		DT_C(dttaudio_record,(ctl,		/* record to disk file				*/
          &audio,
          param->output,
          param->n));
      }
	  else
        rc = dt_err(ctl, F,
          "no case for op \"%s\"",
          param->op);
	}

	DT_C(dtos_time,(ctl, &mark2));		/* get time at end 					*/

	if (DT_DBG(ctl, DT_DBG_MASK_TIME))
	  DT_G(dtos_time_report,(ctl,
        &mark1, &mark2, param->op));
  }
										/* ................................ */
cleanup:
  if (audio_initialized)
    DT_I(dtaudio_uninit,(ctl, &audio));	

  DT_I(dt_leave,(param));			/* clean up before leaving			*/

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
