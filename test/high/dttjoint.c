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
DT_RCSID("test $RCSfile: dttjoint.c,v $ $Revision: 1.9 $");
#include <dtack/mem.h>

#define USAGE "[-Dnnnn]"

static const char *ops[] = {"all",
  "fast",
  NULL};

/*..........................................................................*/
static 
dt_rc_e
dttjoint_make(
  dt_ctl_t *ctl,
  dtimage_t *image1,
  dtimage_t *image2,
  dtxy_t ncols,
  dtxy_t nrows,
  dt_ntype_e ntype,
  void *data1,
  void *data2)
{
  DT_F("dttjoint_make");
  dtxy_t y;
  unsigned int rowsize = (unsigned int)
    (ncols * dt_ntype_sizeof[ntype]);

  DT_Q(dtimage_create,(ctl,				/* make image to hold given data    */
    image1, ncols, nrows, ntype));
 
  for (y=0; y<nrows; y++)				/* copy given data row by row		*/
  {
    void *p;
    DTIMAGE_MAPROW(ctl, image1, y, &p);
    DT_MEMCPY(p, data1, rowsize);
    DTIMAGE_PUTROW(ctl, image1, y);
    data1 = (char *)data1 + rowsize;
  }

  if (image2 != NULL)
  {
    DT_Q(dtimage_create,(ctl,			/* make image to hold given data    */
      image2, ncols, nrows, ntype));
 
    for (y=0; y<nrows; y++)				/* copy given data row by row		*/
    {
      void *p;
      DTIMAGE_MAPROW(ctl, image2, y, &p);
      DT_MEMCPY(p, data2, rowsize);
      DTIMAGE_PUTROW(ctl, image2, y);
      data2 = (char *)data2 + rowsize;
    }
  }

  return DT_RC_GOOD;
}
/*..........................................................................*/
static 
dt_rc_e
dttjoint_check_and_free(
  dt_ctl_t *ctl,
  dtimage_t *joint,
  dt_ntype_b32_t *joint_data,
  int m,
  char *f,
  char *id,
  int *assert)
{
  DT_F("dttjoint_check_and_free");
  dtimage_t joint_expect;
  long cmp;

  DT_Q(dttjoint_make,(ctl,				/* allocate expected joint image	*/
     &joint_expect, NULL, m, m,
     DT_NTYPE_B32, joint_data, NULL));

  DT_Q(dtimage_cmp,(ctl,				/* compare expected to actual	    */
    joint, &joint_expect, 
    0, &cmp, NULL, NULL));

  *assert = DT_RC_GOOD == dt_assert(
    ctl, cmp == 0,
    f, "%s", id);

  if (cmp != 0)							/* comparison failed?			    */
    DT_Q(dtimage_dbg_values,(ctl,
      joint, NULL, DT_DBG_MASK_PIXEL));

  DT_Q(dtimage_free,(ctl,				/* free expected image			    */
    &joint_expect));

  return DT_RC_GOOD;
}
/*..........................................................................*/
static
dt_rc_e
dttjoint1(
  dt_ctl_t *ctl,
  int *assert)
{
  DT_F("dttjoint1");
  dtimage_t input1, input2;
  dtimage_t joint;

#define NCOLS 4
#define NROWS 4
#define M (4)
  static dt_ntype_b08_t input1_data[NCOLS*NROWS] = {
    0,0,1,1,
    0,0,1,1,
    0,2,2,2,
    2,2,3,3
  };
  static dt_ntype_b08_t input2_data[NCOLS*NROWS] = {
    0,0,1,1,
    0,0,1,1,
    0,2,2,2,
    2,2,3,3
  };
  static dt_ntype_b32_t joint_data[M*M] = {
    5,0,0,0,
    0,4,0,0,
    0,0,5,0,
    0,0,0,2
  };

  DT_Q(dttjoint_make,(ctl,				/* make input images			    */
    &input1, &input2,
    NCOLS, NROWS, DT_NTYPE_B08,
    &input1_data[0],
    &input2_data[0]));

  DT_Q(dtimage_create,(ctl,				/* space for joint outputs		    */
    &joint, M, M, DT_NTYPE_B32));

  DT_Q(dtimage_joint,(ctl,				/* make the joint output		    */
    &input1, &input2, &joint));

  DT_Q(dttjoint_check_and_free,(		/* check and free the joint output	*/
    ctl, &joint, &joint_data[0], M, F,
    "diagonal", assert));

  DT_Q(dtimage_free,(ctl, &joint));
  DT_Q(dtimage_free,(ctl, &input1));
  DT_Q(dtimage_free,(ctl, &input2));

#undef NCOLS
#undef NROWS
#undef M
    
  return DT_RC_GOOD;
}
/*..........................................................................*/
static
dt_rc_e
dttjoint2(
  dt_ctl_t *ctl,
  int *assert)
{
  DT_F("dttjoint2");
  dtimage_t input1, input2;
  dtimage_t joint;

#define NCOLS 4
#define NROWS 4
#define M (4)
  static dt_ntype_b08_t input1_data[NCOLS*NROWS] = {
    1,0,1,1,
    0,0,2,1,
    0,2,2,2,
    2,2,3,3
  };
  static dt_ntype_b08_t input2_data[NCOLS*NROWS] = {
    0,0,3,1,
    0,1,1,2,
    1,2,2,2,
    1,2,3,1
  };
  static dt_ntype_b32_t joint_data[M*M] = {
    2,1,0,0,
    2,1,2,1,
    0,1,4,0,
    0,1,0,1
  };

  DT_Q(dttjoint_make,(ctl,				/* make input images			    */
    &input1, &input2,
    NCOLS, NROWS, DT_NTYPE_B08,
    &input1_data[0],
    &input2_data[0]));

  DT_Q(dtimage_create,(ctl,				/* space for joint outputs		    */
    &joint, M, M, DT_NTYPE_B32));

  DT_Q(dtimage_joint,(ctl,				/* make the joint output		    */
    &input1, &input2, &joint));

  DT_Q(dttjoint_check_and_free,(		/* check and free the joint output	*/
    ctl, &joint, &joint_data[0], M, F,
    "dispersed", assert));

  DT_Q(dtimage_free,(ctl, &joint));
  DT_Q(dtimage_free,(ctl, &input1));
  DT_Q(dtimage_free,(ctl, &input2));

#undef NCOLS
#undef NROWS
#undef M
    
  return DT_RC_GOOD;
}
/*..........................................................................*/
static
dt_rc_e
dttjoint3(
  dt_ctl_t *ctl,
  int *assert)
{
  DT_F("dttjoint3");
  dtimage_t input1, input2;
  dtimage_t joint;

#define NCOLS 16
#define NROWS 16
#define M (2)
  static dt_ntype_b08_t input1_data[NCOLS*NROWS] = {
    0x80, 0x01,
    0x00, 0x00,
    0xf0, 0x0f,
    0x00, 0x00,

    0x00, 0x00,
    0x00, 0x00,
    0x00, 0x00,
    0x01, 0x00,

    0x01, 0x00,
    0x00, 0x00,
    0x00, 0x00,
    0x00, 0x00,

    0x00, 0x00,
    0xff, 0xff,
    0x00, 0x00,
    0x00, 0x00
  };
  static dt_ntype_b08_t input2_data[NCOLS*NROWS] = {
    0x00, 0x00,
    0x00, 0x00,
    0xff, 0xff,
    0x00, 0x00,

    0x00, 0x00,
    0x00, 0x00,
    0x00, 0x80,
    0x01, 0x00,

    0x00, 0x00,
    0x80, 0x00,
    0x01, 0x00,
    0x00, 0x00,

    0x00, 0x00,
    0x00, 0x00,
    0x00, 0x00,
    0x00, 0x01
  };
  static dt_ntype_b32_t joint_data[NCOLS*NROWS] = {
    216, 19,
     12,  9
  };

  DT_Q(dttjoint_make,(ctl,				/* make input images			    */
    &input1, &input2,
    NCOLS, NROWS, DT_NTYPE_B01,
    &input1_data[0],
    &input2_data[0]));

  DT_Q(dtimage_create,(ctl,				/* space for joint outputs		    */
    &joint, M, M, DT_NTYPE_B32));

  DT_Q(dtimage_joint,(ctl,				/* make the joint output		    */
    &input1, &input2, &joint));

  DT_Q(dttjoint_check_and_free,(		/* check and free the joint output	*/
    ctl, &joint, &joint_data[0], M, F,
    "binary", assert));

  DT_Q(dtimage_free,(ctl, &joint));
  DT_Q(dtimage_free,(ctl, &input1));
  DT_Q(dtimage_free,(ctl, &input2));

#undef NCOLS
#undef NROWS
#undef M
    
  return DT_RC_GOOD;
}

/*..........................................................................*/

dt_rc_e
DTCONFIG_API1
dttjoint_arg_check(						/* check args and params            */
  dt_ctl_t *ctl,
  void *appstruct,
  dtparam_t *param,
  dt_enter_arg_t *arg,
  char *msg,
  const char **start)
{
  DT_F("dttjoint_arg_check");

  DT_Q(dtt_check_op,(ctl, param,        /* make sure op is understood       */
    ops, msg, start));

  return DT_RC_GOOD;
}

/*..........................................................................*/

DTT_TEST(dttjoint)
{
  DT_F("dttjoint");
  dt_ctl_t *ctl = &param->ctl;
  dt_enter_arg_t arg[1];
  int assert_fail = 0;
  dt_rc_e rc = DT_RC_GOOD;

  DT_GI(dt_heapmon_init,(ctl));         /* init memory checking             */

  DT_GI(dtparam_init,(param));          /* init param structure             */

  DT_GI(dtparam_set,(param, 0,          /* set default for params we want   */
    &param->op, NULL, NULL, "fast",
    &param->reps, NULL, NULL, "1",
    NULL));

  arg[0].desc = NULL;                   /* no arguments                     */
  
  DT_GI(dt_interactive_parse,(			/* parse command line interactively	*/
    argc, argv, F, USAGE, param,
    arg, dttjoint_arg_check, NULL));
                                        /* ................................ */
  while (rc == DT_RC_GOOD)
  {
    long reps;
    for (reps=0;
         reps < param->reps &&
         rc == DT_RC_GOOD;
         reps++)
    {
      if (!strcmp(param->op, "all") ||
          !strcmp(param->op, "fast"))
      {
		DT_GI(dttjoint1,(ctl, &assert_fail));

		DT_GI(dttjoint2,(ctl, &assert_fail));

		DT_GI(dttjoint3,(ctl, &assert_fail));
      }
	}

    DT_G(dt_interactive_after,(         /* make post display                */
      ctl, param, USAGE,
      arg, dttjoint_arg_check, NULL,
      rc));
  }

                                        /* ................................ */

  DT_I(dt_interactive_leave,(			/* clean up before leaving			*/
    param, rc));

  DT_I(dt_heapmon_uninit,(ctl,			/* check all memory is freed	    */
    &assert_fail));

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
