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
| Function return value will be DT_RC_GOOD on success.  
| Any other return value indicates failure or noncompletion.
| When DT_RC_GOOD is not returned, any other return values may not be valid.
| 
| ERRORS
| As yet undocumented.
| 
| END
 *..........................................................................*/

#include <dtack/t.h>
DT_RCSID("test $RCSfile: dttshm.c,v $ $Revision: 1.2 $");
#include <dtack/mem.h>
#include <dtack/shm.h>
#include <dtack/font.h>
#include <dtack/color.h>
#include <dtack/tableau.h>

#define USAGE "[-Dnnnn] [-op n]"

static const char *ops[] = {
  "generate", 
  "sleep", 
  NULL};


#define APP_MAX_SHM (16)

typedef struct {
  dt_ctl_t *ctl;
  dtshm_t shm[APP_MAX_SHM];
  void *memalloc[APP_MAX_SHM];
} app_t;

/*..........................................................................*/
static
dt_rc_e
dttshm_sleep(
  dtparam_t *param,
  int *assert_fail)
{
  DT_F("dttshm_sleep");
  dt_ctl_t *ctl = &param->ctl;
  long i, j;
  dtos_time_t beg, end;
  double elapsed;
  dt_rc_e rc = DT_RC_GOOD;

										/* ................................ */
  DT_C(dtos_time,(ctl, &beg));
  j = 5000 / (int)param->dt;
  for (i=0; i<j; i++)
    DT_C(dtos_delay,(ctl, param->dt));
  DT_C(dtos_time,(ctl, &end));
  elapsed = end.wall-beg.wall;			/* elapsed time 					*/
  dt_dbg(ctl, F, DT_DBG_MASK_TIME,
    "%ld naps, %0.2f naps/second (should be %0.2f)",
	i, elapsed? (i/elapsed): 0.0, 1000.0/param->dt);

cleanup:
  return rc;
}

/*..........................................................................*/
static
dt_rc_e
dttshm_uninit(
  app_t *app)
{
  DT_F("dttshm_uninit");
  dt_ctl_t *ctl = app->ctl;
  int i;
  dt_rc_e rc = DT_RC_GOOD;

  for (i=APP_MAX_SHM-1; i>=0; i--)
  {
    if (app->memalloc[i])
      DT_I(dtmem_free,(ctl, app->memalloc[i], F, "shm wrap"));
	DT_I(dtshm_destroy,(ctl, &app->shm[i]));
  }
  
  memset(app, 0, sizeof(*app));

  return rc;
}

/*..........................................................................*/
static
dt_rc_e
dttshm_init(
  app_t *app,
  dtparam_t *param)
{
  DT_F("dttshm_init");
  dt_ctl_t *ctl = &param->ctl;
  char t[256];
  long length = param->w * param->h + sizeof(tableau_header_t);
  int i;
  dt_rc_e rc = DT_RC_GOOD;

  memset(app, 0, sizeof(*app));

  app->ctl = &param->ctl;

  t[0] = '0';
  t[1] = 'x';
  for (i=0; 1; i++)
  {
	long shmid;
    tableau_header_t *h;
    void *b;
	DT_C(dtstr_extract,(ctl, param->output, ",", i, t+2, sizeof(t)-2));
	if (!t[2])
	  break;
	if (i >= APP_MAX_SHM)
	  DT_CLEANUP((ctl, F, "APP_MAX_SHM %d exceeded",
        APP_MAX_SHM));
	DT_C(dtstr_to_long,(ctl, t, &shmid));
	DT_C(dtshm_create,(ctl, 
      &app->shm[i], shmid, length));
										/* ................................ */
	h = (tableau_header_t *)
      app->shm[i].address;
	memset(h, 0, sizeof(*h));
    h->l = param->w * param->h;			/* length of encoded data 			*/
    h->xe = param->w;					/* size of decoded image 			*/
    h->ye = param->h;
    h->org = DT_COLOR_ORG_LUMINANCE;
	DT_C(dtstr_printf,(ctl,
      h->cam, sizeof(h->cam),
      "cam%ld", shmid));
										/* ................................ */
	b = (void *)(h+1);
	memset(b, 0xcc, h->l);
	DT_C(dtmem_prealloc,(ctl,
      &app->memalloc[i],				/* structure to describe block		*/
      length,							/* unit size						*/
      1,								/* number of units 					*/
      &b,								/* RAM block given 					*/
      length,							/* given RAM page size 				*/
      1,								/* number of RAM pages given		*/
      F,								/* caller name 						*/
      "shm wrap"));						/* what allocation is for 			*/
  }

  if (!i)
    DT_CLEANUP((ctl, F,
	  "no -output given"));

cleanup:
  if (rc != DT_RC_GOOD)
    DT_I(dttshm_uninit,(app));
  return rc;
}

/*..........................................................................*/
static
dt_rc_e
dttshm_generate(
  dtparam_t *param,
  int *assert_fail)
{
  DT_F("dttshm_test");
  dt_ctl_t *ctl = &param->ctl;
  app_t app;
  long k = 0;
  long i, j;
  dtos_time_t beg, end, now, then;
  double elapsed;
  dt_rc_e rc = DT_RC_GOOD;

  memset(&then, 0, sizeof(then));
  
  DT_C(dttshm_init,(&app, param));

  DT_C(dtos_time,(ctl, &beg));
  for (i=0;
	   param->n == -1 ||
	   i < param->n;
	   i++)
  {
	for (j=0; j<APP_MAX_SHM; j++)
	if (app.memalloc[j])
	{
	  tableau_header_t *h;
      char t[256];

	  h = (tableau_header_t *)
		app.shm[j].address;
	  h->n = k;

	  DT_C(dtstr_printf,(ctl,
        t, sizeof(t),
        "%010ld", k++));
	  DT_C(dtfont_8x16_string,(ctl,		/* draw text in in memory			*/
        t, app.memalloc[j],
        DTNTYPE_B08, 0, 0, 0x00));
	  DT_C(dtos_delay,(ctl, 
        (long)param->dt));
	}
	DT_C(dtos_time,(ctl, &now));
	if (now.wall - then.wall > 0.25)
	{	
	  DT_C(dt_yield,(ctl, "%010ld", k));
	  then = now;
	}
  }
  DT_C(dtos_time,(ctl, &end));
  elapsed = end.wall-beg.wall;			/* elapsed time 					*/
  dt_dbg(ctl, F, DT_DBG_MASK_TIME,
    "%ld frames, %0.2f frames/second",
	k, elapsed? (k/elapsed): 0.0);

cleanup:
  DT_I(dttshm_uninit,(&app));
  return rc;
}

/*..........................................................................*/
dt_rc_e
DTCONFIG_API1
dttshm_arg_check(						/* check args and params            */
  dt_ctl_t *ctl,
  void *app,
  dtparam_t *param,
  dt_enter_arg_t *arg,
  char *msg,
  const char **start)
{
  DT_F("dttshm_arg_check");

  DT_Q(dtt_check_op,(ctl, param,        /* make sure op is understood       */
    ops, msg, start));

  return DT_RC_GOOD;
}

/*..........................................................................*/

DTMAIN1(dttshm)
{
  DT_F("dttshm");
  dt_ctl_t *ctl = &param->ctl;
  dt_enter_arg_t arg[1];
  int assert_fail = 0;                  /* failed assertion count           */
  dt_rc_e rc = DT_RC_GOOD;

  DT_C(dt_heapmon_init,(ctl));         /* init memory checking             */

  DT_C(dtparam_init,(param));          /* init param structure             */

  DT_C(dtparam_set,(param, 0,          /* set default for params we want   */
    &param->op, NULL, NULL, "default",
    &param->output, "SHM list", NULL, "5700,5701",
    &param->n, "Loops", NULL, "120",
    &param->dt, "Delay", NULL, "15",
    NULL));

  arg[0].desc = NULL;                   /* no arguments                     */
  
  DT_C(dt_enter_parse,(					/* parse command line				*/
    argc, argv, F, USAGE, param,
    arg, dttshm_arg_check, NULL));
                                        /* ................................ */

  if (!strcmp(param->op, "generate"))
  {
	DT_C(dttshm_generate,(param,
      &assert_fail));
  }

  if (!strcmp(param->op, "sleep"))
  {
	DT_C(dttshm_sleep,(param,
      &assert_fail));
  }

                                        /* ................................ */
cleanup:
  DT_I(dt_leave,(param));				/* clean up before leaving          */

  DT_I(dt_heapmon_uninit,(ctl,          /* check all memory is freed        */
    &assert_fail));

  if (assert_fail)                      /* any failed assertions?           */
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
