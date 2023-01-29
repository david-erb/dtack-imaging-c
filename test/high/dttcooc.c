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
DT_RCSID("test $RCSfile: dttcooc.c,v $ $Revision: 1.9 $");
#include <dtack/mem.h>

#define USAGE "[-Dnnnn]"

static const char *ops[] = {"all",
  "fast",
  NULL};

/*..........................................................................*/
static 
dt_rc_e
dttcooc_make(
  dt_ctl_t *ctl,
  dtimage_t *image,
  dtxy_t ncols,
  dtxy_t nrows,
  dt_ntype_e ntype,
  void *data)
{
  DT_F("dttcooc_make");
  dtxy_t y;
  unsigned int rowsize = (unsigned int)
    (ncols * dt_ntype_sizeof[ntype]);

  DT_Q(dtimage_create,(ctl,				/* make image to hold given data    */
    image, ncols, nrows, ntype));
 
  for (y=0; y<nrows; y++)				/* copy given data row by row		*/
  {
    void *p;
    DTIMAGE_MAPROW(ctl, image, y, &p);
    DT_MEMCPY(p, data, rowsize);
    DTIMAGE_PUTROW(ctl, image, y);
    data = (char *)data + rowsize;
  }

  return DT_RC_GOOD;
}
/*..........................................................................*/
static 
dt_rc_e
dttcooc_check_and_free(
  dt_ctl_t *ctl,
  dtimage_t *cooc,
  dt_ntype_b32_t *cooc_data,
  int m,
  char *f,
  char *id,
  int *assert)
{
  DT_F("dttcooc_check_and_free");
  dtimage_t cooc_expect;
  long cmp;

  DT_Q(dttcooc_make,(ctl,				/* allocate expected cooc image		*/
     &cooc_expect, m, m,
     DT_NTYPE_B32, cooc_data));

  DT_Q(dtimage_cmp,(ctl,				/* compare expected to actual	    */
    cooc, &cooc_expect, 
    0, &cmp, NULL, NULL));

  *assert = DT_RC_GOOD == dt_assert(
    ctl, cmp == 0,
    f, "%s", id);

  if (cmp != 0)							/* comparison failed?			    */
    DT_Q(dtimage_dbg_values,(ctl,
      cooc, NULL, DT_DBG_MASK_PIXEL));

  DT_Q(dtimage_free,(ctl,				/* free expected image			    */
    &cooc_expect));

  return DT_RC_GOOD;
}
/*..........................................................................*/
static
dt_rc_e
dttcooc1(
  dt_ctl_t *ctl,
  int *assert)
{
  DT_F("dttcooc1");
  dtimage_t input;
  dtimage_t cooc;

#define NCOLS 4
#define NROWS 4
#define M (4)
  static dt_ntype_b08_t input_data[NCOLS*NROWS] = {
    0,0,1,1,
    0,0,1,1,
    0,2,2,2,
    2,2,3,3
  };
  static dt_ntype_b32_t cooc_h[M*M] = {
    2,2,1,0,
    0,2,0,0,
    0,0,3,1,
    0,0,0,1
  };
  static dt_ntype_b32_t cooc_v[M*M] = {
    3,0,2,0,
    0,2,2,0,
    0,0,1,2,
    0,0,0,0
  };
  static dt_ntype_b32_t cooc_ld[M*M] = {
    1,1,3,0,
    0,1,1,0,
    0,0,0,2,
    0,0,0,0
  };
  static dt_ntype_b32_t cooc_rd[M*M] = {
    2,1,0,0,
    0,1,2,0,
    0,0,2,1,
    0,0,0,0
  };
  static dt_ntype_b32_t cooc_sum[M*M] = {
    16, 4, 6, 0,
     4,12, 5, 0,
     6, 5,12, 6,
     0, 0, 6, 2
  };

  DT_Q(dttcooc_make,(ctl, &input,		/* make input image				    */
    NCOLS, NROWS, DT_NTYPE_B08,
    &input_data[0]));

  DT_Q(dtimage_create,(ctl,				/* space for cooc outputs		    */
    &cooc, M, M, DT_NTYPE_B32));

  DT_Q(dtimage_cooc_set,(ctl,			/* (1,0)							*/
    &input, 1, 0, &cooc));
  DT_Q(dttcooc_check_and_free,(	
    ctl, &cooc, &cooc_h[0], M, F,
    "h", assert));
  DT_Q(dtimage_cooc_set,(ctl,			/* (-1,0)							*/
    &input, -1, 0, &cooc));
  DT_Q(dttcooc_check_and_free,(	
    ctl, &cooc, &cooc_h[0], M, F,
    "h neg", assert));

  DT_Q(dtimage_cooc_set,(ctl,			/* (0,1)							*/
    &input, 0, 1, &cooc));
  DT_Q(dttcooc_check_and_free,(	
    ctl, &cooc, &cooc_v[0], M, F,
    "v", assert));
  DT_Q(dtimage_cooc_set,(ctl,			/* (0,-1)							*/
    &input, 0, -1, &cooc));
  DT_Q(dttcooc_check_and_free,(	
    ctl, &cooc, &cooc_v[0], M, F,
    "v neg", assert));

  DT_Q(dtimage_cooc_set,(ctl,			/* (1,1)							*/
    &input, 1, 1, &cooc));
  DT_Q(dttcooc_check_and_free,(	
    ctl, &cooc, &cooc_ld[0], M, F,
    "ld", assert));
  DT_Q(dtimage_cooc_set,(ctl,			/* (-1,-1)							*/
    &input, -1, -1, &cooc));
  DT_Q(dttcooc_check_and_free,(	
    ctl, &cooc, &cooc_ld[0], M, F,
    "ld neg", assert));

  DT_Q(dtimage_cooc_set,(ctl,			/* (1,-1)							*/
    &input, 1, -1, &cooc));
  DT_Q(dttcooc_check_and_free,(	
    ctl, &cooc, &cooc_rd[0], M, F,
    "rd", assert));
  DT_Q(dtimage_cooc_set,(ctl,			/* (-1,1)							*/
    &input, -1, 1, &cooc));
  DT_Q(dttcooc_check_and_free,(	
    ctl, &cooc, &cooc_rd[0], M, F,
    "rd neg", assert));

  DT_Q(dtimage_constant,(ctl,			/* clear cooc output				*/
    &cooc, 0.0));
  DT_Q(dtimage_cooc_sum,(ctl,			/* (1,0)							*/
    &input, 1, 0, &cooc));
  DT_Q(dtimage_cooc_sum,(ctl,			/* (0,1)							*/
    &input, 0, 1, &cooc));
  DT_Q(dtimage_cooc_sum,(ctl,			/* (1,1)							*/
    &input, 1, 1, &cooc));
  DT_Q(dtimage_cooc_sum,(ctl,			/* (1,-1)							*/
    &input, 1, -1, &cooc));
  DT_Q(dtimage_cooc_mirror,(ctl,		/* do mirroring to lower triangle	*/
    &cooc));
  DT_Q(dttcooc_check_and_free,(	
    ctl, &cooc, &cooc_sum[0], M, F,
    "sum", assert));

  DT_Q(dtimage_free,(ctl, &cooc));
  DT_Q(dtimage_free,(ctl, &input));

#undef NCOLS
#undef NROWS
#undef M
    
  return DT_RC_GOOD;
}
/*..........................................................................*/
static
dt_rc_e
dttcooc2(
  dt_ctl_t *ctl,
  int *assert)
{
  DT_F("dttcooc2");
  dtimage_t input;
  dtimage_t cooc;

#define NCOLS 16
#define NROWS 16
#define M (2)
  static dt_ntype_b08_t input_data[NCOLS*NROWS] = {
    1,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,1,
    0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0,
    1,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0,
    0,0,0,0, 0,0,0,1, 0,0,0,0, 0,0,0,0,

    0,0,0,0, 0,0,0,1, 1,0,0,0, 0,0,0,0,
    0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0,
    0,0,0,0, 0,0,0,0, 1,0,0,0, 0,0,0,0,
    0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0,

    0,0,0,0, 0,0,0,0, 0,0,0,0, 1,0,0,0,
    0,0,0,0, 0,0,0,1, 0,0,0,0, 0,0,0,0,
    0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0,
    0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,1,

    0,0,0,0, 0,0,0,0, 0,0,0,1, 0,0,0,0,
    0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0,
    0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0,
    1,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,1
  };
  static dt_ntype_b32_t cooc_h[M*M] = {
    221, 18,
      0, 1
  };
  static dt_ntype_b32_t cooc_v[M*M] = {
    219, 20,
      0,  1
  };
  static dt_ntype_b32_t cooc_ld[M*M] = {
    208, 16,
      0,  1
  };
  static dt_ntype_b32_t cooc_rd[M*M] = {
    207, 18,
     0,  0
  };
  static dt_ntype_b32_t cooc_sum[M*M] = {
    1710, 72,
      72,  6
  };

  DT_Q(dttcooc_make,(ctl, &input,		/* make input image				    */
    NCOLS, NROWS, DT_NTYPE_B08,
    &input_data[0]));

  DT_Q(dtimage_create,(ctl,				/* space for cooc outputs		    */
    &cooc, M, M, DT_NTYPE_B32));

  DT_Q(dtimage_cooc_set,(ctl,			/* (1,0)							*/
    &input, 1, 0, &cooc));
  DT_Q(dttcooc_check_and_free,(	
    ctl, &cooc, &cooc_h[0], M, F,
    "h", assert));
  DT_Q(dtimage_cooc_set,(ctl,			/* (-1,0)							*/
    &input, -1, 0, &cooc));
  DT_Q(dttcooc_check_and_free,(	
    ctl, &cooc, &cooc_h[0], M, F,
    "h neg", assert));

  DT_Q(dtimage_cooc_set,(ctl,			/* (0,1)							*/
    &input, 0, 1, &cooc));
  DT_Q(dttcooc_check_and_free,(	
    ctl, &cooc, &cooc_v[0], M, F,
    "v", assert));
  DT_Q(dtimage_cooc_set,(ctl,			/* (0,-1)							*/
    &input, 0, -1, &cooc));
  DT_Q(dttcooc_check_and_free,(	
    ctl, &cooc, &cooc_v[0], M, F,
    "v neg", assert));

  DT_Q(dtimage_cooc_set,(ctl,			/* (1,1)							*/
    &input, 1, 1, &cooc));
  DT_Q(dttcooc_check_and_free,(	
    ctl, &cooc, &cooc_ld[0], M, F,
    "ld", assert));
  DT_Q(dtimage_cooc_set,(ctl,			/* (-1,-1)							*/
    &input, -1, -1, &cooc));
  DT_Q(dttcooc_check_and_free,(	
    ctl, &cooc, &cooc_ld[0], M, F,
    "ld neg", assert));

  DT_Q(dtimage_cooc_set,(ctl,			/* (1,-1)							*/
    &input, 1, -1, &cooc));
  DT_Q(dttcooc_check_and_free,(	
    ctl, &cooc, &cooc_rd[0], M, F,
    "rd", assert));
  DT_Q(dtimage_cooc_set,(ctl,			/* (-1,1)							*/
    &input, -1, 1, &cooc));
  DT_Q(dttcooc_check_and_free,(	
    ctl, &cooc, &cooc_rd[0], M, F,
    "rd neg", assert));

  DT_Q(dtimage_constant,(ctl,			/* clear cooc output				*/
    &cooc, 0.0));
  DT_Q(dtimage_cooc_sum,(ctl,			/* (1,0)							*/
    &input, 1, 0, &cooc));
  DT_Q(dtimage_cooc_sum,(ctl,			/* (0,1)							*/
    &input, 0, 1, &cooc));
  DT_Q(dtimage_cooc_sum,(ctl,			/* (1,1)							*/
    &input, 1, 1, &cooc));
  DT_Q(dtimage_cooc_sum,(ctl,			/* (1,-1)							*/
    &input, 1, -1, &cooc));
  DT_Q(dtimage_cooc_mirror,(ctl,		/* do mirroring to lower triangle	*/
    &cooc));
  DT_Q(dttcooc_check_and_free,(	
    ctl, &cooc, &cooc_sum[0], M, F,
    "sum", assert));

  DT_Q(dtimage_free,(ctl, &cooc));
  DT_Q(dtimage_free,(ctl, &input));

#undef NCOLS
#undef NROWS
#undef M
    
  return DT_RC_GOOD;
}
/*..........................................................................*/
static
dt_rc_e
dttcooc3(
  dt_ctl_t *ctl,
  int *assert)
{
  DT_F("dttcooc3");
  dtimage_t input;
  dtimage_t cooc;

#define NCOLS 16
#define NROWS 16
#define M (2)
  static dt_ntype_b01_t input_data[(NCOLS/8)*NROWS] = {
    0x80, 0x01,
    0x00, 0x00,
    0x80, 0x00,
    0x01, 0x00,

	0x01, 0x80,
    0x00, 0x00,
	0x00, 0x80,
	0x00, 0x00,

	0x00, 0x08,
	0x01, 0x00,
	0x00, 0x00,
	0x00, 0x01,

	0x00, 0x10,
	0x00, 0x00,
	0x00, 0x00,
	0x80, 0x01
  };
  static dt_ntype_b32_t cooc_h[M*M] = {
    221, 18,
      0, 1
  };
  static dt_ntype_b32_t cooc_v[M*M] = {
    219, 20,
      0,  1
  };
  static dt_ntype_b32_t cooc_ld[M*M] = {
    208, 16,
      0,  1
  };
  static dt_ntype_b32_t cooc_rd[M*M] = {
    207, 18,
     0,  0
  };
  static dt_ntype_b32_t cooc_sum[M*M] = {
    1710, 72,
      72,  6
  };

  DT_Q(dttcooc_make,(ctl, &input,		/* make input image				    */
    NCOLS, NROWS, DT_NTYPE_B01,
    &input_data[0]));

  DT_Q(dtimage_create,(ctl,				/* space for cooc outputs		    */
    &cooc, M, M, DT_NTYPE_B32));

  DT_Q(dtimage_cooc_set,(ctl,			/* (1,0)							*/
    &input, 1, 0, &cooc));
  DT_Q(dttcooc_check_and_free,(	
    ctl, &cooc, &cooc_h[0], M, F,
    "h", assert));
  DT_Q(dtimage_cooc_set,(ctl,			/* (-1,0)							*/
    &input, -1, 0, &cooc));
  DT_Q(dttcooc_check_and_free,(	
    ctl, &cooc, &cooc_h[0], M, F,
    "h neg", assert));

  DT_Q(dtimage_cooc_set,(ctl,			/* (0,1)							*/
    &input, 0, 1, &cooc));
  DT_Q(dttcooc_check_and_free,(	
    ctl, &cooc, &cooc_v[0], M, F,
    "v", assert));
  DT_Q(dtimage_cooc_set,(ctl,			/* (0,-1)							*/
    &input, 0, -1, &cooc));
  DT_Q(dttcooc_check_and_free,(	
    ctl, &cooc, &cooc_v[0], M, F,
    "v neg", assert));

  DT_Q(dtimage_cooc_set,(ctl,			/* (1,1)							*/
    &input, 1, 1, &cooc));
  DT_Q(dttcooc_check_and_free,(	
    ctl, &cooc, &cooc_ld[0], M, F,
    "ld", assert));
  DT_Q(dtimage_cooc_set,(ctl,			/* (-1,-1)							*/
    &input, -1, -1, &cooc));
  DT_Q(dttcooc_check_and_free,(	
    ctl, &cooc, &cooc_ld[0], M, F,
    "ld neg", assert));

  DT_Q(dtimage_cooc_set,(ctl,			/* (1,-1)							*/
    &input, 1, -1, &cooc));
  DT_Q(dttcooc_check_and_free,(	
    ctl, &cooc, &cooc_rd[0], M, F,
    "rd", assert));
  DT_Q(dtimage_cooc_set,(ctl,			/* (-1,1)							*/
    &input, -1, 1, &cooc));
  DT_Q(dttcooc_check_and_free,(	
    ctl, &cooc, &cooc_rd[0], M, F,
    "rd neg", assert));

  DT_Q(dtimage_constant,(ctl,			/* clear cooc output				*/
    &cooc, 0.0));
  DT_Q(dtimage_cooc_sum,(ctl,			/* (1,0)							*/
    &input, 1, 0, &cooc));
  DT_Q(dtimage_cooc_sum,(ctl,			/* (0,1)							*/
    &input, 0, 1, &cooc));
  DT_Q(dtimage_cooc_sum,(ctl,			/* (1,1)							*/
    &input, 1, 1, &cooc));
  DT_Q(dtimage_cooc_sum,(ctl,			/* (1,-1)							*/
    &input, 1, -1, &cooc));
  DT_Q(dtimage_cooc_mirror,(ctl,		/* do mirroring to lower triangle	*/
    &cooc));
  DT_Q(dttcooc_check_and_free,(	
    ctl, &cooc, &cooc_sum[0], M, F,
    "sum", assert));

  DT_Q(dtimage_free,(ctl, &cooc));
  DT_Q(dtimage_free,(ctl, &input));

#undef NCOLS
#undef NROWS
#undef M
    
  return DT_RC_GOOD;
}

/*..........................................................................*/

dt_rc_e
DTCONFIG_API1
dttcooc_arg_check(						/* check args and params            */
  dt_ctl_t *ctl,
  void *appstruct,
  dtparam_t *param,
  dt_enter_arg_t *arg,
  char *msg,
  const char **start)
{
  DT_F("dttcooc_arg_check");

  DT_Q(dtt_check_op,(ctl, param,        /* make sure op is understood       */
    ops, msg, start));

  return DT_RC_GOOD;
}

/*..........................................................................*/

DTT_TEST(dttcooc)
{
  DT_F("dttcooc");
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
    arg, dttcooc_arg_check, NULL));
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
		DT_GI(dttcooc1,(ctl, &assert_fail));

		DT_GI(dttcooc2,(ctl, &assert_fail));

		DT_GI(dttcooc3,(ctl, &assert_fail));
      }
	}

    DT_G(dt_interactive_after,(         /* make post display                */
      ctl, param, USAGE,
      arg, dttcooc_arg_check, NULL,
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
