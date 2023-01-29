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
| USAGE
| dttcast - test executive
| 
|.SYNOPSIS
|:dttcast [params]
|
|.SUMMARY
| Exercise and verify the dtimage_cast() and related functions.
|
|.PARAMETERS
| :-op:		operation to perform (default "default")
| :-reps:	number of times to repeat selected op or ops (default 1)
| :-n:		number of inner loops for timing (default 5)
|dtinclude(params1.dx)
|
|.OPERATIONS 
| The :-op: parameter may take one of the following values:
| :default:	do the :simple: and :image: ops 
| :simple:	tiny image (4x4 or smaller) with wide value range 
|			in each datatype converted to each other datatype
| :image:	medium sized image (128x128) with medium value range
|			in each datatype converted to each other datatype
| :time:	large image (256x256) with binary pattern
|			in each datatype converted to each other datatype and back
| 
|.PAGING
| The :image: and :time: operations may be exercised under varying paging
| configurations.
| The minimum no-dialog configurations are:
| :-op image:		maxalloc 16384, pagesize 1024, npages 1
| :-op time:		maxalloc 655360, pagesize 2048, npages 1
|
|dtinclude(params2.dx)
|
| END
 *..........................................................................*/

#include <dtack/t.h>
#include <dtack/mem.h>

DT_RCSID("test $RCSfile: dttcast.c,v $ $Revision: 1.8 $");

#include <dttcast.use>

static const char *ops[] = {"all",
  "default", "simple", "image", "time", 
  NULL};

/*..........................................................................*/
static 
dt_rc_e
dttcast_simple_one(
  dt_ctl_t *ctl,
  dtxy_t ncols,
  dtxy_t nrows,
  void *data1,
  dt_ntype_e ntype1,
  void *data2,
  dt_ntype_e ntype2,
  char *f,
  int *assert_fail)
{
  DT_F("dttcast_simple_one");
  dtimage_t image1, image2;
  char id[64];
  unsigned int rowsize1 =
    (unsigned int)(ncols * dt_ntype_sizeof[ntype1]);
  unsigned int rowsize2 =
    (unsigned int)(ncols * dt_ntype_sizeof[ntype2]);
  dtxy_t y;

  sprintf(id, "%s %-3s->%-3s", f,
    dt_ntype_string[ntype1],
    dt_ntype_string[ntype2]);

  DT_Q(dtimage_create,(ctl,				/* make image to hold given data    */
    &image1, ncols, nrows, ntype1));
 
  for (y=0; y<nrows; y++)				/* copy given data row by row		*/
  {
    void *p;
    DTIMAGE_MAPROW(ctl, &image1, y, &p);
    DT_MEMCPY(p, data1, rowsize1);
    DTIMAGE_PUTROW(ctl, &image1, y);
    data1 = (char *)data1 + rowsize1;
  }

  DT_Q(dtimage_create,(ctl,				/* make destination image			*/
    &image2, ncols, nrows, ntype2));

  DT_Q(dtimage_cast,(ctl,				/* do cast						    */
    &image1, &image2));

  assert_fail += (DT_RC_GOOD !=			/* check exact data correspondence  */
    dt_assert(ctl,
      !memcmp(image2.row[0].dvoid,
         data2, nrows * rowsize2),
      F, id));

  DT_Q(dtimage_free,(ctl, &image2));
  DT_Q(dtimage_free,(ctl, &image1));

  return DT_RC_GOOD;
}

/*..........................................................................*/
static
dt_rc_e
dttcast_simple(
  dt_ctl_t *ctl,
  int *assert_fail)
{
  DT_F("dttcast_simple");
  int i, j;

# define ALL_DATA \
  DATA(b08, dt_ntype_b08_t, DT_NTYPE_B08) \
  DATA(b16, dt_ntype_b16_t, DT_NTYPE_B16) \
  DATA(b32, dt_ntype_b32_t, DT_NTYPE_B32) \
  DATA(s2,  dt_ntype_s2_t,  DT_NTYPE_S2) \
  DATA(s4,  dt_ntype_s4_t,  DT_NTYPE_S4) \
  DATA(f1,  dt_ntype_f1_t,  DT_NTYPE_F1) \
  DATA(f2,  dt_ntype_f2_t,  DT_NTYPE_F2)

#define NCOLS 3
#define NROWS 3
#define DATA(NAME, TYPE, ENUM) 			/* data for any form but b01		*/\
  static TYPE NAME[NCOLS*NROWS] = { \
      1,   2,   3, \
    127, 128, 129, \
    253, 254, 255 \
  };

ALL_DATA								/* generate data for any form		*/

#undef DATA
#define DATA(NAME, TYPE, ENUM) 			/* list entry for each data			*/\
  {(void *)NAME, ENUM},

  static struct {						/* generate list entries			*/
    void *data;
    dt_ntype_e type;
  } list[] = {ALL_DATA {NULL, DTNTYPE_NULL}};

  for (i=0; list[i].data; i++)
  {
    for (j=0; list[j].data; j++)
    DT_Q(dttcast_simple_one,(ctl,	
      NCOLS, NROWS, 
      list[i].data, list[i].type,
      list[j].data, list[j].type,
      F, assert_fail));
  }
  return DT_RC_GOOD;

#undef DATA
#undef ALL_DATA
#undef NROWS
#undef NCOLS    
}


/*..........................................................................*/
static 
dt_rc_e
dttcast_image_one(
  dt_ctl_t *ctl,
  dtimage_t *src,
  dtimage_t *dst,
  char *f,
  int *assert_fail)
{
  DT_F("dttcast_image_one");
  dtimage_t fwd;
  char id[64];
  long cmprc;
  dt_rc_e rc;

  fwd.xe = 0;
  DT_C(dtimage_create,(ctl,				/* get space to cast forward into    */
    &fwd, dst->xe, dst->ye,
    dst->ntype));

  sprintf(id, "%-3s->%-3s",
    dt_ntype_string[src->ntype], 
    dt_ntype_string[dst->ntype]);
  DT_YIELD(DT_C, id);

  DT_C(dtimage_cast,(ctl,				/* cast one time				    */
    src, &fwd));

  DT_C(dtimage_cmp,(ctl,				/* compare results				    */
    &fwd, dst, 0, &cmprc, NULL, NULL));

  *assert_fail += DT_RC_GOOD !=
    dt_assert(ctl, cmprc == 0,
      F, "%s image comparison %ld (0)",
      id, cmprc);

cleanup:
  if (fwd.xe)
    DT_I(dtimage_free,(ctl, &fwd));

  return rc;
}

/*..........................................................................
 *..........................................................................*/
static
dt_rc_e
dttcast_image(
  dt_ctl_t *ctl,
  int *assert_fail)
{
  DT_F("dttcast_image");
  dtimage_t src;
  dtimage_t dst;
  dtxy_t i;
  dt_rc_e rc;

  src.xe = 0;
  dst.xe = 0;
#define NCOLS 128
#define NROWS 128
#define DATA(IMAGE, TYPE, FORE, BACK) \
  if (IMAGE.xe) DT_C(dtimage_free,(ctl, &IMAGE)); IMAGE.xe = 0; \
  DT_C(dtimage_create,(ctl, &IMAGE, NCOLS, NROWS, TYPE)); \
  DT_C(dtimage_constant,(ctl, &IMAGE, BACK)); \
  for (i=0; i<NROWS; i++) \
    DT_C(dtimage_set_pixel,(ctl, &IMAGE, i, i, FORE)) \

  {
    DATA(src, DT_NTYPE_B01, 1, 0.0);
    DATA(dst, DT_NTYPE_B01, 1, 0.0);
    DT_C(dttcast_image_one,(ctl, &src, &dst, F, assert_fail));
    DATA(dst, DT_NTYPE_B08, 1, 0.0);
    DT_C(dttcast_image_one,(ctl, &src, &dst, F, assert_fail));
    DATA(dst, DT_NTYPE_B16, 1, 0.0);
    DT_C(dttcast_image_one,(ctl, &src, &dst, F, assert_fail));
    DATA(dst, DT_NTYPE_B32, 1, 0.0);
    DT_C(dttcast_image_one,(ctl, &src, &dst, F, assert_fail));
    DATA(dst, DT_NTYPE_S2, 1, 0.0);
    DT_C(dttcast_image_one,(ctl, &src, &dst, F, assert_fail));
    DATA(dst, DT_NTYPE_S4, 1, 0.0);
    DT_C(dttcast_image_one,(ctl, &src, &dst, F, assert_fail));
    DATA(dst, DT_NTYPE_F1, 1, 0.0);
    DT_C(dttcast_image_one,(ctl, &src, &dst, F, assert_fail));
    DATA(dst, DT_NTYPE_F2, 1, 0.0);
    DT_C(dttcast_image_one,(ctl, &src, &dst, F, assert_fail));
  }

  {
    DATA(src, DT_NTYPE_B08, i+1, 0.0);
    DATA(dst, DT_NTYPE_B01, 1.0, 0.0);
    DT_C(dttcast_image_one,(ctl, &src, &dst, F, assert_fail));
    DATA(dst, DT_NTYPE_B08, i+1, 0.0);
    DT_C(dttcast_image_one,(ctl, &src, &dst, F, assert_fail));
    DATA(dst, DT_NTYPE_B16, i+1, 0.0);
    DT_C(dttcast_image_one,(ctl, &src, &dst, F, assert_fail));
    DATA(dst, DT_NTYPE_B32, i+1, 0.0);
    DT_C(dttcast_image_one,(ctl, &src, &dst, F, assert_fail));
    DATA(dst, DT_NTYPE_S2, i+1, 0.0);
    DT_C(dttcast_image_one,(ctl, &src, &dst, F, assert_fail));
    DATA(dst, DT_NTYPE_S4, i+1, 0.0);
    DT_C(dttcast_image_one,(ctl, &src, &dst, F, assert_fail));
    DATA(dst, DT_NTYPE_F1, i+1, 0.0);
    DT_C(dttcast_image_one,(ctl, &src, &dst, F, assert_fail));
    DATA(dst, DT_NTYPE_F2, i+1, 0.0);
    DT_C(dttcast_image_one,(ctl, &src, &dst, F, assert_fail));
  }

cleanup:
  if (dst.xe)
    DT_I(dtimage_free,(ctl, &dst));
  if (src.xe)
    DT_I(dtimage_free,(ctl, &src));
  return rc;

#undef DATA
#undef NROWS
#undef NCOLS    
}

/*..........................................................................*/
static 
dt_rc_e
dttcast_time_one(
  dt_ctl_t *ctl,
  dtimage_t *image,
  dt_ntype_e desttype,
  long n,
  char *f,
  int *assert_fail)
{
  DT_F("dttcast_time_one");
  dtos_time_t mark1, mark2;
  dtimage_t fwdimage;
  dtimage_t bckimage;
  char id[64];
  int i;
  long cmprc;
  dt_rc_e rc;

  fwdimage.xe = 0;
  DT_C(dtimage_create,(ctl,				/* get space to cast forward into    */
    &fwdimage, image->xe, image->ye,
    desttype));

  bckimage.xe = 0;
  DT_C(dtimage_create,(ctl,				/* get space to cast backward into   */
    &bckimage, image->xe, image->ye,
    image->ntype));

  sprintf(id, "%-3s->%-3s (%2ld)",
    dt_ntype_string[image->ntype], 
    dt_ntype_string[desttype], 
    n);

  DT_C(dtos_time,(ctl, &mark1));
  for (i=0; i<n; i++)
  {
    DT_YIELD_PERCENT(DT_C, id,			/* yield during running			    */
      (unsigned int)i, 
      (unsigned int)n);
    DT_C(dtimage_cast,(ctl,				/* cast forward one time		    */
      image, &fwdimage));
  }
  DT_C(dtos_time,(ctl, &mark2));
  DT_C(dtt_time_report,(ctl,			/* report time					    */
    &mark1, &mark2, id));

  sprintf(id, "%-3s->%-3s (%2ld)",
    dt_ntype_string[desttype], 
    dt_ntype_string[image->ntype], 
    n);
  DT_YIELD(DT_C, id);
  DT_C(dtos_time,(ctl, &mark1));
  for (i=0; i<n; i++)
  {
    DT_C(dtimage_cast,(ctl,				/* cast back one time			    */
      &fwdimage, &bckimage));
  }
  DT_C(dtos_time,(ctl, &mark2));
  DT_C(dtt_time_report,(ctl,			/* report time					    */
    &mark1, &mark2, id));

  DT_C(dtimage_cmp,(ctl,				/* compare results				    */
    image, &bckimage, 0,
    &cmprc, NULL, NULL));

  *assert_fail += DT_RC_GOOD !=
    dt_assert(ctl, cmprc == 0,
      F, "image comparison %ld (0)",
      cmprc);

cleanup:
  DT_I(dtimage_free,(ctl, &bckimage));
  DT_I(dtimage_free,(ctl, &fwdimage));

  return rc;
}

/*..........................................................................
 * 6: for timing
 *..........................................................................*/
static
dt_rc_e
dttcast_time(
  dt_ctl_t *ctl,
  long n,
  int *assert_fail)
{
  DT_F("dttcast_time");
  dtimage_t image;
  dtxy_t i;
  dt_ntype_e list[] = {
    DT_NTYPE_B01,
    DT_NTYPE_B08,
    DT_NTYPE_B16,
    DT_NTYPE_B32,
    DT_NTYPE_S1,
    DT_NTYPE_S2,
    DT_NTYPE_S4,
    DT_NTYPE_F1,
    DT_NTYPE_F2,
    DT_NTYPE_NULL};
  static int scale[] = {
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    5,
    5};
  dt_rc_e rc;

  image.xe = 0;
#define NCOLS 256
#define NROWS 256
#define DATA(TYPE) \
  if (image.xe) {DT_C(dtimage_free,(ctl, &image)); image.xe = 0;} \
  DT_C(dtimage_create,(ctl, &image, NCOLS, NROWS, TYPE)); \
  DT_C(dtimage_constant,(ctl, &image, 0.0)); \
  for (i=0; i<NROWS; i++) \
    DT_C(dtimage_set_pixel,(ctl, &image, i, i, 1.0)) \

  DATA(DT_NTYPE_B08);
  for (i=0; list[i]; i++)
    DT_C(dttcast_time_one,(ctl, &image, list[i], n/scale[i], F, assert_fail));

#ifdef NONO
  {
    DATA(DT_NTYPE_B16);
    DT_C(dttcast_time,(ctl, &image,
      F, assert_fail,
      MEAN, VARIANCE));
  }

  {
    DATA(DT_NTYPE_B32);
    DT_C(dttcast_time,(ctl, &image, 10,
      F, assert_fail,
      MEAN, VARIANCE));
  }

  {
    DATA(DT_NTYPE_S2);
    DT_C(dttcast_time,(ctl, &image, 10,
      F, assert_fail,
      MEAN, VARIANCE));
  }

  {
    DATA(DT_NTYPE_S4);
    DT_C(dttcast_time,(ctl, &image, 10,
      F, assert_fail,
      MEAN, VARIANCE));
  }

  {
    DATA(DT_NTYPE_F1);
    DT_C(dttcast_time,(ctl, &image, 5,
      F, assert_fail,
      MEAN, VARIANCE));
  }

  {
    DATA(DT_NTYPE_F2);
    DT_C(dttcast_time,(ctl, &image, 5,
      F, assert_fail,
      MEAN, VARIANCE));
  }
#endif

cleanup:
  if (image.xe)
    DT_I(dtimage_free,(ctl, &image));
    
  return rc;

#undef DATA
#undef NROWS
#undef NCOLS    
}
/*..........................................................................*/

dt_rc_e 
DTCONFIG_API1
dttcast_arg_check(						/* check args and params			*/
  dt_ctl_t *ctl,
  void *app,
  dtparam_t *param,
  dt_enter_arg_t *arg,
  char *msg,
  const char **start)
{
  DT_F("dttcast_arg_check");

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

DTT_MAIN(dttcast)
{
  DT_F("dttcast");
  dt_ctl_t *ctl = &param->ctl;
  dt_enter_arg_t arg[1];
  int assert_fail = 0;					/* failed assertion count		    */
  dt_rc_e rc = DT_RC_GOOD;

  DT_GI(dt_heapmon_init,(ctl));			/* init memory checking			    */

  DT_GI(dtparam_init,(param));			/* init param structure             */

  DT_GI(dtparam_set,(param, 0,			/* set default for params we want	*/
    &param->op, NULL, NULL, "default",
    &param->reps, NULL, NULL, "1",
    &param->n, "Timing loops", NULL, "5",
    NULL));

  arg[0].desc = NULL;					/* no arguments					    */
  
  DT_GI(dt_interactive_parse,(			/* parse command line interactively	*/
    argc, argv, F, usage, param,
    arg, dttcast_arg_check, NULL));

  while (rc == DT_RC_GOOD)
  {
    long reps;
    for (reps=0; 
         reps < param->reps && 
         rc == DT_RC_GOOD;
         reps++)
	{
      if (!strcmp(param->op, "all") ||
          !strcmp(param->op, "default") ||
          !strcmp(param->op, "simple"))
      {
        DT_YIELD(DT_GI, "Doing simple tests");
        DT_GI(dttcast_simple,(ctl, 
          &assert_fail));
      }

      if (!strcmp(param->op, "all") ||
          !strcmp(param->op, "default") ||
          !strcmp(param->op, "image"))
      {
        DT_GI(dttcast_image,(ctl,
          &assert_fail));
      }

      if (!strcmp(param->op, "all") ||
          !strcmp(param->op, "time"))
      {
        DT_GI(dttcast_time,(ctl, 
          param->n, &assert_fail));
      }
	}

    DT_G(dt_interactive_after,(         /* make post display                */
      ctl, param, usage,
      arg, dttcast_arg_check, NULL, rc));
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
