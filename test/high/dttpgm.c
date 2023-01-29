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
#include <dtack/pnm.h>

DT_RCSID("test $RCSfile: dttpgm.c,v $ $Revision: 1.6 $");

static const char *ops[] = {"all",
  "default",
  NULL};

#define USAGE "[-Dnnnn]"

#define FILENAME1 "dttpgm1.dat"
#define FILENAME2 "dttpgm2.dat"

/*..........................................................................*/

#define CHECK(COND) printf("%s: %c %s %s\n", NAME, COND?' ':'*', id, #COND);

static
dt_rc_e
read_good(
  dt_ctl_t *ctl,
  char *id,								/* id of this test					*/
  dtfd_t *fd,							/* input							*/
  int ptype,
  unsigned long xe,
  unsigned long ye,
  dt_ntype_e ntype,
  char next,
  int *assert_fail)
{
  char *F = id;
  dtpnm_t pnm;
  char pnext;

  DT_Q(dtpnm_read_header,(ctl,			/* read and parse header			*/
    &pnm, fd));						

  DT_Q(dtfd_read_all,(ctl,				/* next char to be parsed			*/
    fd, &pnext, 1));			

  *assert_fail += DT_RC_GOOD != 
  dtt_check(ctl, 
    ptype == pnm.ptype &&
    xe == pnm.xe &&
    ye == pnm.ye,
    "%s: ptype %d (%d) xe %lu (%lu), ye %lu (%lu)", 
     id, 
     pnm.ptype, ptype,
     pnm.xe, xe,
     pnm.ye, ye);

  *assert_fail += DT_RC_GOOD != 
  dtt_check(ctl, 
    ntype == pnm.ntype &&
    pnext == next,
    "%s: ntype %d (%d), next 0x%02x (0x%02x)",
    id,
    pnm.ntype, ntype,
    pnext, next);

  return DT_RC_GOOD;
}

/*..........................................................................*/

dt_rc_e
DTCONFIG_API1
dttpgm_arg_check(						/* check args and params            */
  dt_ctl_t *ctl,
  void *app,
  dtparam_t *param,
  dt_enter_arg_t *arg,
  char *msg,
  const char **start)
{
  DT_F("dttpgm_arg_check");

  DT_Q(dtt_check_op,(ctl, param,        /* make sure op is understood       */
    ops, msg, start));

  if (msg[0] == '\0')					/* no problems yet? 				*/
  DT_Q(dtt_check_datadir,(ctl, param,	/* make sure datadir exists       	*/
    msg, start));

  return DT_RC_GOOD;
}


/*..........................................................................*/

DTT_TEST(dttpgm)
{
  DT_F("dttpgm");
  dt_ctl_t *ctl = &param->ctl;
  dt_enter_arg_t arg[1];
  dtfd_t fd;
  int assert_fail = 0;
  char *filename = NULL;
  int *afp = &assert_fail;
  dt_rc_e rc;

  DT_C(dt_heapmon_init,(ctl));			/* init memory checking             */

  DT_C(dtparam_init,(param));			/* init param structure             */

  DT_C(dtparam_set,(param, 0,			/* set default for params we want   */
    &param->op, NULL, NULL, "default",
    &param->datadir, NULL, NULL, NULL,
    &param->reps, NULL, NULL, "1",
    NULL));

  arg[0].desc = NULL;                   /* no arguments                     */
  DT_C(dt_enter_parse,(					/* go parse params and args			*/
    argc, argv, F, USAGE, param,
    arg, dttpgm_arg_check, NULL));
                                        /* ................................ */

  DT_C(dtos_malloc2,(ctl, 
    (void **)&filename,
	strlen(param->datadir)+
    DT_MAX(strlen(FILENAME1), 
           strlen(FILENAME2))+2,
    F, "data filename"));

  {
    long reps;
    for (reps=0;
         reps < param->reps &&
         rc == DT_RC_GOOD;
         reps++)
    {
	  strcpy(filename, param->datadir);
	  strcat(filename, "/");
	  strcat(filename, FILENAME1);
      DT_C(dtfd_open,(ctl, &fd, filename, "r"));
	  DT_C(read_good,(ctl, "f1a", &fd, 5, 100L, 200L, DT_NTYPE_B08, '\n', afp));
	  DT_C(dtfd_close,(ctl, &fd));
	  
	  strcpy(filename, param->datadir);
	  strcat(filename, "/");
	  strcat(filename, FILENAME2);
	  DT_C(dtfd_open,(ctl, &fd, filename, "r"));
	  DT_C(read_good,(ctl, "f2a", &fd, 5, 65540L, 65540L, DT_NTYPE_B08, 'a', afp));
	  DT_C(read_good,(ctl, "f2b", &fd, 5,     1L,     1L, DT_NTYPE_B08, 'b', afp));
	  DT_C(read_good,(ctl, "f2c", &fd, 5,     2L,   222L, DT_NTYPE_B08, 'c', afp));
	  DT_C(read_good,(ctl, "f2d", &fd, 2,     3L,   444L, DT_NTYPE_B08, 'd', afp));
	  DT_C(dtfd_close,(ctl, &fd));
	}
  }

                                        /* ................................ */
cleanup:
  if (filename)
  DT_I(dtos_free2,(ctl, filename,
    F, "data filename"));

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
