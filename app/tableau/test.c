/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
  ! Copyright (C) 1998 Dtack Inc. All Rights Reserved
  ! To use this file, you must have signed a license agreement with Dtack Inc.
  ! Under no circumstances may you redistribute this file.
  ! This software is provided AS IS with no warranty expressed or implied.
  ! Dtack Inc. accepts no liability for use or misuse of this file.
  ! http://www.dtack.com  dtack@dtack.com  telephone +360.417.6700
  ! Dtack Inc, P.O. Box 3061, Port Angeles, WA USA 98362
  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */





#include <tableau.h>
DT_RCSID("tableau $RCSfile: test.c,v $ $Revision: 1.8 $");

#ifndef DTDEPEND						/* no makefile dependency 			*/
#  include <dsched.h>
#endif

										/* ................................ */
static const char *results0[] = {
  "Sunday 00:00.09 00:00.10 00:00.01 00:00",
  "end of rules",
  NULL
};

#define SEQ0 (1)
#define TOT0 (2)
static const int seq0[SEQ0] = {
  7
};

										/* ................................ */
static const char *results1[] = {
  "Sunday 00:00.09 00:00.10 00:00 00:00",
  "Friday 00:00.09 00:00.10 00:00 00:00",
  "end of rules",
  NULL
};

#define SEQ1 (6)
#define TOT1 (6)
static const int seq1[SEQ1] = {
  6, 7, 6,
  7, 6, 6
};
				
										/* ................................ */
static const char *results2[] = {
  "Sunday 09:00 10:00 00:10 00:00",
  "end of rules",
  NULL
};

#define SEQ2 (18)
#define TOT2 (18)
static const int seq2[SEQ2] = {
  6, 7, 5, 5, 5, 5, 5, 5, 6, 
  7, 6, 5, 5, 5, 5, 5, 5, 6, 
};
										/* ................................ */
static const char *results3[] = {		/* once an hour all week			*/
  "Sunday 00:00 23:59.59 01:00 00:00",
  "Monday 00:00 23:59.59 01:00 00:00",
  "Tuesday 00:00 23:59.59 01:00 00:00",
  "Wednesday 00:00 23:59.59 01:00 00:00",
  "Thursday 00:00 23:59.59 01:00 00:00",
  "Friday 00:00 23:59.59 01:00 00:00",
  "Saturday 00:00 23:59.59 01:00 00:00",
  "end of rules",
  NULL
};

#define SEQ3 (1)
#define TOT3 (24*7*2)
static const int seq3[SEQ3] = {
  4
};
										/* ................................ */
static const char *results4[] = {		/* one camera continuously			*/
  "Sunday 00:00 23:59.59 00:00 00:00",
  "Monday 00:00 23:59.59 00:00 00:00",
  "end of rules",
  NULL
};

#define SEQ4 (1)
#define TOT4 (60*60*24*2*2-8)
static const int seq4[SEQ4] = {
  3
};
										/* ................................ */
static const char *results5[] = {		/* second camera simultaneously		*/
  "Sunday 00:00 23:59.59 00:00 00:00",
  "Monday 00:00 23:59.59 00:00 00:00",
  "end of rules",
  NULL
};

#define SEQ5 (2)
#define TOT5 (60*60*24*2*2-8)
static const int seq5[SEQ5] = {
  2, 3
};
										/* ................................ */
static const char *results6[] = {		/* every 10 minutes for 10 seconds	*/
  "Sunday 00:00 23:59.59 00:10 00:00.10",
  "Monday 00:00 23:59.59 00:10 00:00.10",
  "Tuesday 00:00 23:59.59 00:10 00:00.10",
  "Wednesday 00:00 23:59.59 00:10 00:00.10",
  "Thursday 00:00 23:59.59 00:10 00:00.10",
  "Friday 00:00 23:59.59 00:10 00:00.10",
  "Saturday 00:00 23:59.59 00:10 00:00.10",
  "end of rules",
  NULL
};

#define SEQ6 (1)
#define TOT6 (10*6*24*7*2-26)
static const int seq6[SEQ6] = {
  1
};

										/* ................................ */
/*..........................................................................
 *..........................................................................*/

static
dt_rc_e
tableau_test_schedule_remove(
  dtparam_t *param,
  const char *id)
{
  DT_F("tableau_test_schedule_remove");
  dt_ctl_t *ctl = &param->ctl;
  char filename[128];
  int isreadable;

  DT_Q(dsched_database,(param, id,		/* make database filename 			*/
    filename, sizeof(filename)));

  DT_Q(dtos_isreadable,(ctl, filename,
    &isreadable));
  if (isreadable)
    DT_Q(dtos_unlink,(ctl, filename));

  return DT_RC_GOOD;
}
/*..........................................................................
 *..........................................................................*/

static
dt_rc_e
tableau_test_schedule_write(
  dtparam_t *param,
  const char *id,
  const char **lines)
{
  DT_F("tableau_test_schedule_write");
  dt_ctl_t *ctl = &param->ctl;
  char filename[128];
  dtfd_t _fd, *fd = &_fd;
  int i;
  dt_rc_e rc;

  DT_C(dsched_database,(param, id,		/* make database filename 			*/
    filename, sizeof(filename)));

  DT_C(dtfd_open,(ctl, fd,
    filename, "w"));

  for (i=0; lines[i]; i++)				/* for all database lines 			*/
  {
    DT_C(dtfd_write_all,(ctl, fd,		/* write database line 				*/
      lines[i], strlen(lines[i])));
    DT_C(dtfd_write_all,(ctl, fd,
      "\n", 1));
  }

cleanup:
  if (fd->fd != -1)
    DT_I(dtfd_close,(ctl, fd));

  return rc;
}

/*..........................................................................
 *..........................................................................*/

static
dt_rc_e
tableau_test_schedule_one(
  dtparam_t *param,
  const char *id,
  const char **lines,
  const int *seq,
  int seqmax,
  int seqtot)
{
  DT_F("tableau_test_schedule");
  dt_ctl_t *ctl = &param->ctl;
  tableau_cam_t _cam, *cam = &_cam;
  time_t clock = time(NULL);			/* current time 					*/
  struct tm *t = localtime(&clock);		/* format into fields 				*/
  struct tm q;
  int cam_index;
  int unexpected = 0;
  int n;
  dt_rc_e rc;

  DT_C(tableau_test_schedule_write,(	/* create dummy database 			*/
    param, id, lines));

  DT_C(tableau_cam_init,(param, cam));	/* get camera config 				*/
  
  q = *t;
  q.tm_sec = q.tm_min = q.tm_hour = 0;	/* midnight							*/
  q.tm_mday = 1;						/* Dec 1, 1996 						*/
  q.tm_mon = 11;
  q.tm_year = 96;
  cam->autotime = mktime(&q);

  DT_C(tableau_cam_reload,(param,		/* re-get camera config 			*/
    cam));

  n = 0;
  while(1)
  {
#ifdef NONO
    DT_C(tableau_cam_next,(param,
      cam, &cam_index));
#endif
	if (cam_index == -1)
	  break;
	if (seq != NULL)
	if (n < seqtot)
	{
	  if (cam_index != seq[n%seqmax] &&
		  unexpected < 10)
	  {
  	    dt_assert(ctl, 0, F,
          "%s %3d. cam index expected %d and got %d",
          id, n, seq[n%seqmax], cam_index);
		unexpected++;
	  }
	}
	else
	if (n == seqtot)
	{
  	  dt_assert(ctl, 0, F,
        "%s %3d. unexpected cam index %d",
        id, n, cam_index);
	  unexpected++;
	  if (unexpected >= 10)
  	    break;
	}
	n++;
  }

cleanup:
  if (unexpected < 10)
  dt_assert(ctl, n == seqtot, F,
    "%s expected %d shots and got %d",
    id, seqtot, n);

  if (cam->initialized)
    DT_I(tableau_cam_uninit,(param,		/* release camera resources 		*/
      cam));

  return rc;
}

/*..........................................................................
 *..........................................................................*/

static
dt_rc_e
tableau_test_schedule(
  dtparam_t *param)
{
  DT_F("tableau_test_schedule");
  dt_ctl_t *ctl = &param->ctl;

  DT_Q(tableau_test_schedule_remove,(param, "test00"));
  DT_Q(tableau_test_schedule_remove,(param, "test01"));
  DT_Q(tableau_test_schedule_remove,(param, "test02"));
  DT_Q(tableau_test_schedule_remove,(param, "test03"));
  DT_Q(tableau_test_schedule_remove,(param, "test04"));
  DT_Q(tableau_test_schedule_remove,(param, "test05"));
  DT_Q(tableau_test_schedule_remove,(param, "test06"));
  DT_Q(tableau_test_schedule_remove,(param, "test07"));

  DT_Q(tableau_test_schedule_one,(
    param, "test07", results0, seq0, SEQ0, TOT0));
  
  DT_Q(tableau_test_schedule_one,(
    param, "test06", results1, seq1, SEQ1, TOT1));
  
  DT_Q(tableau_test_schedule_one,(
    param, "test05", results2, seq2, SEQ2, TOT2));

  DT_Q(tableau_test_schedule_remove,(param, "test05"));
  DT_Q(tableau_test_schedule_remove,(param, "test06"));
  DT_Q(tableau_test_schedule_remove,(param, "test07"));

  DT_Q(tableau_test_schedule_one,(
    param, "test04", results3, seq3, SEQ3, TOT3));

  DT_Q(tableau_test_schedule_remove,(param, "test04"));
  DT_Q(tableau_test_schedule_one,(
    param, "test03", results4, seq4, SEQ4, TOT4));

  DT_Q(tableau_test_schedule_one,(
    param, "test02", results5, seq5, SEQ5, TOT5));

  DT_Q(tableau_test_schedule_remove,(param, "test04"));
  DT_Q(tableau_test_schedule_remove,(param, "test03"));
  DT_Q(tableau_test_schedule_remove,(param, "test02"));

  DT_Q(tableau_test_schedule_one,(
    param, "test01", results6, seq6, SEQ6, TOT6));

  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
tableau_test(
  dtparam_t *param)
{
  DT_F("tableau_test");
  dt_ctl_t *ctl = &param->ctl;

# define T(X) (!strcmp(#X, param->op))
  if (T(test) || T(testcam))
    DT_Q(tableau_test_schedule,(param));
# undef T
  
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
