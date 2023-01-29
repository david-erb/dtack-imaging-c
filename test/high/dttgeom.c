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
DT_RCSID("test $RCSfile: dttgeom.c,v $ $Revision: 1.9 $");
#include <dtack/mem.h>

#define USAGE "[-Dnnnn]"

static const char *ops[] = {"all",
  "fast",
  NULL};

typedef enum {
  GEOM_OP_UNKNOWN = 0,
  GEOM_OP_TRANSPOSE = 1,
  GEOM_OP_ROTATE90,
  GEOM_OP_ROTATE270N,
  GEOM_OP_ROTATE180,
  GEOM_OP_ROTATE180N,
  GEOM_OP_ROTATE270,
  GEOM_OP_ROTATE90N,
  GEOM_OP_ROTATE450N,
  GEOM_OP_BIGUGLY = 1000
} op_e;

static const char *op_s[] = {
  "unknown",
  "transpose",
  "rotate 90",
  "rotate -270",
  "rotate 180",
  "rotate -180",
  "rotate 270",
  "rotate -90",
  "rotate -450",
  NULL
};

#define DTYPES																  \
  DTYPE(B08, b08)															  \
  DTYPE(B16, b16)															  \
  DTYPE(S4, s4)																  \
  DTYPE(F2, f2)	

/*..........................................................................*/
static 
dt_rc_e
dttgeom_make(
  dt_ctl_t *ctl,
  dtimage_t *image,
  dtxy_t ncols,
  dtxy_t nrows,
  dt_ntype_e ntype,
  void *data)
{
  DT_F("dttgeom_make");
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
dttgeom_test1_run(
  dt_ctl_t *ctl,
  void *source_data,
  dtxy_t source_xe,				  
  dtxy_t source_ye,
  dtntype_e source_ntype,
  op_e op,	  
  void *expect_data,
  dtxy_t expect_xe,				  
  dtxy_t expect_ye,
  dtntype_e expect_ntype,
  int *assert)
{
  DT_F("dttgeom_test1_run");
  dtimage_t source_image;
  dtimage_t actual_image;
  dtimage_t expect_image;
  dtxy_t x, y;
  char id[256];
  long cmp;
  dt_rc_e rc = DT_RC_GOOD;
  
  source_image.xe = 0;
  actual_image.xe = 0;
  expect_image.xe = 0;
  
  DT_C(dttgeom_make,(ctl,				/* create and populate source 		*/
    &source_image,
    source_xe,
	source_ye,
    source_ntype,
    source_data));

  switch(op)							/* run the desired test 			*/
  {
    case GEOM_OP_TRANSPOSE:
	  DT_C(dtimage_transpose,(ctl,
		&source_image, &actual_image));
    break;
    case GEOM_OP_ROTATE90:
	  DT_C(dtimage_rotate,(ctl,
        &source_image, 90.0,
        &actual_image));
	break;
    case GEOM_OP_ROTATE270N:
	  DT_C(dtimage_rotate,(ctl,
        &source_image, -270.0,
        &actual_image));
	break;
    case GEOM_OP_ROTATE180:
	  DT_C(dtimage_rotate,(ctl,
        &source_image, 180.0,
        &actual_image));
	break;
    case GEOM_OP_ROTATE180N:
	  DT_C(dtimage_rotate,(ctl,
        &source_image, -180.0,
        &actual_image));
	break;
    case GEOM_OP_ROTATE270:
	  DT_C(dtimage_rotate,(ctl,
        &source_image, 270.0,
        &actual_image));
	break;
    case GEOM_OP_ROTATE90N:
	  DT_C(dtimage_rotate,(ctl,
        &source_image, -90.0,
        &actual_image));
	break;
    case GEOM_OP_ROTATE450N:
	  DT_C(dtimage_rotate,(ctl,
        &source_image, -450.0,
        &actual_image));
	break;
  }
  
  DT_C(dttgeom_make,(ctl,				/* create and populate expected 	*/
    &expect_image,
    expect_xe,
	expect_ye,
    expect_ntype,
    expect_data));

  DT_C(dtimage_cmp,(ctl,				/* compare expected to actual	    */
    &actual_image, &expect_image, 
    0, &cmp, &x, &y));

  DT_C(dtstr_printf,(ctl, id, sizeof(id),
    "%s %s", 
    dt_ntype_string[source_ntype],
    op_s[op]));
  
  if (cmp == 0)
    dt_assert(
      ctl, cmp == 0,
      NULL, "%s", id);
  else
    *assert = DT_RC_GOOD == dt_assert(
      ctl, cmp == 0,
     NULL, "%s (first wrong [%d,%d])", id,
     x, y);

  if (cmp != 0)							/* comparison failed?			    */
    DT_Q(dtimage_dbg_values,(ctl,
      &actual_image, op_s[op],
      DT_DBG_MASK_PIXEL));


cleanup:
  if (expect_image.xe)
	DT_I(dtimage_free,(ctl,
      &expect_image));
  if (actual_image.xe)
	DT_I(dtimage_free,(ctl,
      &actual_image));
  if (source_image.xe)
	DT_I(dtimage_free,(ctl,
      &source_image));

  return rc;
}
/*..........................................................................*/
static
dt_rc_e
dttgeom_test1(
  dt_ctl_t *ctl,
  int *assert)
{
  DT_F("dttgeom_test1");

#define NCOLS 4
#define NROWS 5

										/* ................................ */
#define DTYPE(N, T)															  \
  static dt_ntype_ ## T ## _t 												  \
  source_ ## T[NCOLS*NROWS] = {												  \
     0,  1,  2,  3,															  \
     4,  5,  6,  7,															  \
     8,  9, 10, 11,															  \
    12, 13, 14, 15,															  \
    16, 17, 18, 19															  \
  };
DTYPES
#undef DTYPE
										/* ................................ */
{
#define DTYPE(N, T)															  \
  static dt_ntype_ ## T ## _t 												  \
  transposed_ ## T[NCOLS*NROWS] = {											  \
     0,  4,  8, 12, 16,														  \
     1,  5,  9, 13, 17,														  \
     2,  6, 10, 14, 18,														  \
     3,  7, 11, 15, 19														  \
  };
DTYPES
#undef DTYPE

#define DTYPE(N, T)															  \
  DT_Q(dttgeom_test1_run,(ctl, 												  \
    (void *)source_ ## T,													  \
    NCOLS, NROWS, DT_NTYPE_ ## N,											  \
    GEOM_OP_TRANSPOSE,														  \
    (void *)transposed_ ## T,												  \
    NROWS, NCOLS, DT_NTYPE_ ## N, assert));
DTYPES
#undef DTYPE
}
										/* ................................ */
{
#define DTYPE(N, T)															  \
  static dt_ntype_ ## T ## _t 												  \
  rotated90_ ## T[NCOLS*NROWS] = {											  \
     16, 12,  8,  4,  0,													  \
     17, 13,  9,  5,  1,													  \
     18, 14, 10,  6,  2,													  \
     19, 15, 11,  7,  3														  \
  };
DTYPES
#undef DTYPE

#define DTYPE(N, T)															  \
  DT_Q(dttgeom_test1_run,(ctl, 												  \
    (void *)source_ ## T,													  \
    NCOLS, NROWS, DT_NTYPE_ ## N,											  \
    GEOM_OP_ROTATE90,														  \
    (void *)rotated90_ ## T,												  \
    NROWS, NCOLS, DT_NTYPE_ ## N, assert));
DTYPES
#undef DTYPE

#define DTYPE(N, T)															  \
  DT_Q(dttgeom_test1_run,(ctl, 												  \
    (void *)source_ ## T,													  \
    NCOLS, NROWS, DT_NTYPE_ ## N,											  \
    GEOM_OP_ROTATE270N,														  \
    (void *)rotated90_ ## T,												  \
    NROWS, NCOLS, DT_NTYPE_ ## N, assert));
DTYPES
#undef DTYPE
}
										/* ................................ */
{
#define DTYPE(N, T)															  \
  static dt_ntype_ ## T ## _t 												  \
  rotated180_ ## T[NCOLS*NROWS] = {											  \
     19, 18, 17, 16,														  \
     15, 14, 13, 12,														  \
     11, 10,  9,  8,														  \
      7,  6,  5,  4,														  \
      3,  2,  1,  0															  \
  };
DTYPES
#undef DTYPE

#define DTYPE(N, T)															  \
  DT_Q(dttgeom_test1_run,(ctl, 												  \
    (void *)source_ ## T,													  \
    NCOLS, NROWS, DT_NTYPE_ ## N,											  \
    GEOM_OP_ROTATE180,														  \
    (void *)rotated180_ ## T,												  \
    NCOLS, NROWS, DT_NTYPE_ ## N, assert));
DTYPES
#undef DTYPE

#define DTYPE(N, T)															  \
  DT_Q(dttgeom_test1_run,(ctl, 												  \
    (void *)source_ ## T,													  \
    NCOLS, NROWS, DT_NTYPE_ ## N,											  \
    GEOM_OP_ROTATE180N,														  \
    (void *)rotated180_ ## T,												  \
    NCOLS, NROWS, DT_NTYPE_ ## N, assert));
DTYPES
#undef DTYPE
}
										/* ................................ */
{
#define DTYPE(N, T)															  \
  static dt_ntype_ ## T ## _t 												  \
  rotated270_ ## T[NCOLS*NROWS] = {											  \
     3,  7, 11, 15, 19,														  \
     2,  6, 10, 14, 18,														  \
     1,  5,  9, 13, 17,														  \
     0,  4,  8, 12, 16														  \
  };
DTYPES
#undef DTYPE

#define DTYPE(N, T)															  \
  DT_Q(dttgeom_test1_run,(ctl, 												  \
    (void *)source_ ## T,													  \
    NCOLS, NROWS, DT_NTYPE_ ## N,											  \
    GEOM_OP_ROTATE270,														  \
    (void *)rotated270_ ## T,												  \
    NROWS, NCOLS, DT_NTYPE_ ## N, assert));
DTYPES
#undef DTYPE

#define DTYPE(N, T)															  \
  DT_Q(dttgeom_test1_run,(ctl, 												  \
    (void *)source_ ## T,													  \
    NCOLS, NROWS, DT_NTYPE_ ## N,											  \
    GEOM_OP_ROTATE90N,														  \
    (void *)rotated270_ ## T,												  \
    NROWS, NCOLS, DT_NTYPE_ ## N, assert));
DTYPES
#undef DTYPE

#define DTYPE(N, T)															  \
  DT_Q(dttgeom_test1_run,(ctl, 												  \
    (void *)source_ ## T,													  \
    NCOLS, NROWS, DT_NTYPE_ ## N,											  \
    GEOM_OP_ROTATE450N,														  \
    (void *)rotated270_ ## T,												  \
    NROWS, NCOLS, DT_NTYPE_ ## N, assert));
DTYPES
#undef DTYPE
}

#undef NCOLS
#undef NROWS
    
  return DT_RC_GOOD;
}

/*..........................................................................*/

dt_rc_e
DTCONFIG_API1
dttgeom_arg_check(						/* check args and params            */
  dt_ctl_t *ctl,
  void *appstruct,
  dtparam_t *param,
  dt_enter_arg_t *arg,
  char *msg,
  const char **start)
{
  DT_F("dttgeom_arg_check");

  DT_Q(dtt_check_op,(ctl, param,        /* make sure op is understood       */
    ops, msg, start));

  return DT_RC_GOOD;
}

/*..........................................................................*/

DTT_MAIN(dttgeom)
{
  DT_F("dttgeom");
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
    arg, dttgeom_arg_check, NULL));
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
		DT_GI(dttgeom_test1,(ctl, &assert_fail));
      }
	}

    DT_G(dt_interactive_after,(         /* make post display                */
      ctl, param, USAGE,
      arg, dttgeom_arg_check, NULL,
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
