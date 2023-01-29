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
  CHANGES
  19991217 DE data/dttxchg3.tiff to data/dttxchg3.tif 
 *..........................................................................*/


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
#include <dtack/lut8.h>
#include <dtack/xchg.h>
#include <dtack/mem.h>
#include <dtack/prj.h>
#include <dtack/list.h>
#include <dtack/dsk.h>

DT_RCSID("test $RCSfile: dttxchg.c,v $ $Revision: 1.1 $");

static char *USAGE = "usage: [params]\n\
value of -op may be one of:\n\
  fast   implies which, pgm, wmf, bmp, and dsk\n\
  which  string parsing\n\
  pgm    write/read pgm files\n\
  wmf    write/read Microsoft Windows metafiles\n\
  bmp    write/read Microsoft Windows metafiles\n\
  wxx    exchange data with VB front end\n\
  dsk    windows-on-disk output, input, server\n\
If -op is omitted then \"fast\" is used.";

static const char *ops[] = {"all",
  "fast",
  "which",
  "dsk",
  "pgm", "wmf", "bmp", "wxx", "tiff", 
  "lj3", "lj6", 
  NULL};

typedef struct {
  int op;
} app_t;

#define ISOP(S) (!strcmp(ops[a->op], (S)))

#define TMPDIR_REPLACE "%s"

#define PGM_NAME1 "pgm,," TMPDIR_REPLACE "/dttxchg1.pg-"
#define PGM_NAME2 TMPDIR_REPLACE "/dttxchg2.pgm"
#define PGM_NAME3 TMPDIR_REPLACE "/dttxchg3.pgm"
#define PGM_NAME4 "PGM,,send_max=1000,recv_max=1000:dttxchg3"

#define WMF_NAME1 "wmf,," TMPDIR_REPLACE "/dttxchg1.wm-"
#define WMF_NAME2 TMPDIR_REPLACE "/dttxchg2.wmf"
#define WMF_NAME3 TMPDIR_REPLACE "/dttxchg3.wmf"

#define BMP_NAME1 "bmp,," TMPDIR_REPLACE "/dttxchg1.bm-"
#define BMP_NAME2 TMPDIR_REPLACE "/dttxchg2.bmp"
#define BMP_NAME3 TMPDIR_REPLACE "/dttxchg3.bmp"

#define WXX_NAME1 "wxx,file,bmp," TMPDIR_REPLACE ",first image"
#define WXX_NAME2 "wxx,file,bmp," TMPDIR_REPLACE ",second image"
#define WXX_NAME3 "wxx,file,bmp," TMPDIR_REPLACE ",third image"

#define TIFF_NAME1 "tiff,,%s/dttxchg1.ti-"
#define TIFF_NAME2 "%s/dttxchg2.tif"
#define TIFF_NAME3 "%s/dttxchg3.tif"

#define LJ300_NAME1 "lj3,," TMPDIR_REPLACE "/dttxchg1.l3-"
#define LJ300_NAME2 TMPDIR_REPLACE "/dttxchg2.lj3"
#define LJ300_NAME3 TMPDIR_REPLACE "/dttxchg3.lj300"
#define LJ300_NAME4 "LJ3,,send_max=1000,recv_max=1000:dttxchg3"

#define LJ600_NAME1 "lj6,," TMPDIR_REPLACE "/dttxchg1.l6-"
#define LJ600_NAME2 TMPDIR_REPLACE "/dttxchg2.lj6"
#define LJ600_NAME3 TMPDIR_REPLACE "/dttxchg3.lj600"
#define LJ600_NAME4 "LJ6,,send_max=1000,recv_max=1000:dttxchg3"

#define DSK_LIST TMPDIR_REPLACE
#define DSK_BASE1 "dttxchg1"
#define DSK_BASE2 "dttxchg3"
#define DSK_BASE3 "dttxchg2"
#define DSK_NAME1 "win,dsk," DSK_LIST "," DSK_BASE1
#define DSK_NAME2 "win,dsk," DSK_LIST "," DSK_BASE2
#define DSK_NAME3 "win,dsk," DSK_LIST "," DSK_BASE3
#define DSK_NAME4 "DSK,,send_max=1000,recv_max=1000:dttxchg3"

#define FLAG_CAN_READ 1
#define FLAG_CAN_WRITE 2
#define FLAG_CAN_DELETE 4
#define FLAG_BITIMAGE 8
#define FLAG_NONE 0

extern
dt_rc_e
DTCONFIG_API1							/* not in any header file 			*/
dttxchg_which(
  dt_ctl_t *ctl,
  int *assert_fail);

/*..........................................................................
 *..........................................................................*/

static
dt_rc_e
dttxchg_gen(
  dt_ctl_t *ctl,
  dtimage_t *image,
  dtxy_t w,
  dtxy_t h,
  dtt_image_gen_f *gen,
  long flags)
{
  DT_F("dttxchg_gen");
  dtimage_t bit;
  dt_rc_e rc;
 
  bit.xe = 0;
  image->xe = 0;
  DT_Q(dtimage_create,(ctl, image,		/* make new image 1				    */
    w, h, DT_NTYPE_B08));

  DT_C((*gen),(ctl, image));			/* generate test image				*/

  if (flags & FLAG_BITIMAGE)			/* supposed to make a bit image? 	*/
  {
    DT_C(dtimage_create,(ctl, &bit,		/* make space for bit image	    */
      w, h, DT_NTYPE_B01));
    DT_C(dtimage_cast,(ctl, image,		/* change precision			    	*/
      &bit));
    DT_C(dtimage_free,(ctl, image));	/* free original image				*/
    *image = bit;
    bit.xe = 0;
  }
  return DT_RC_GOOD;

cleanup:
  if (bit.xe)
    DT_I(dtimage_free,(ctl, &bit));
  if (image->xe)
    DT_I(dtimage_free,(ctl, image));

  return rc;
}

/*..........................................................................
 *..........................................................................*/

static
dt_rc_e
dttxchg_export(
  dt_ctl_t *ctl,
  const char *tmpdir,
  const char *name,
  dtxy_t x0,
  dtxy_t y0,
  dtxy_t w,
  dtxy_t h,
  dtt_image_gen_f *gen,
  long flags,
  int *assert_fail)
{
  DT_F("dttxchg_export");
  dtimage_t image;
  char *filename = NULL;
  dt_rc_e rc;
 
  DT_C(dttxchg_gen,(ctl, &image,		/* generate test image 				*/
    w, h, gen, flags));

  DT_C(dtos_malloc2,(ctl,				/* add tmpdir to name				*/
    (void **)&filename, 
    strlen(tmpdir)+strlen(name),
    F, "filename"));
  sprintf(filename, name, tmpdir);

  DT_C(dtimage_export,(ctl,				/* output the whole image		    */
    &image, filename, x0, y0));
 
  dt_assert(ctl, 1,
    F, "output \"%s\"", filename);

  DT_C(dtimage_free,(ctl, &image));

cleanup:
  if (filename != NULL)
    DT_I(dtos_free2,(ctl, filename,
      F, "filename"));
  return rc;
}

/*..........................................................................
 *..........................................................................*/

static
dt_rc_e
dttxchg_import(
  dt_ctl_t *ctl,
  const char *tmpdir,
  const char *name,
  dtxy_t w,
  dtxy_t h,
  dtt_image_gen_f *gen,
  long flags,
  int *assert_fail)
{
  DT_F("dttxchg_import");
  dtimage_t img1;
  dtimage_t img2;
  char *filename = NULL;
  long cmprc;
  dtxy_t x, y;
  dt_rc_e rc;
  
  DT_Q(dttxchg_gen,(ctl, &img1,			/* generate test image 				*/
    w, h, gen, flags));

  DT_C(dtos_malloc2,(ctl,				/* add tmpdir to name				*/
    (void **)&filename, 
    strlen(tmpdir)+strlen(name),
    F, "filename"));
  sprintf(filename, name, tmpdir);

  img2.xe = 0;
  DT_C(dtimage_import,(ctl,				/* input the whole image		    */
    &img2, filename));

  DT_C(dtimage_cmp,(ctl,				/* compare two images				*/
    &img1, &img2, 0,
    &cmprc, &x, &y));

  if (cmprc == 0)
    dt_assert(ctl, 1,
      F, "image comparison \"%s\" OK",
      filename);
  else
  {
    *assert_fail += 1;
    dt_assert(ctl, 0,
      F, "image comparison \"%s\""
      " %ld at (%u,%u)",
      filename, cmprc, x, y);
  }

cleanup:
  if (filename != NULL)
    DT_I(dtos_free2,(ctl, filename,
      F, "filename"));
  if (img2.xe)
    DT_I(dtimage_free,(ctl, &img2));
  if (img1.xe)
    DT_I(dtimage_free,(ctl, &img1));

  return rc;
}

/*..........................................................................
 *..........................................................................*/

static
dt_rc_e
dttxchg_delete(
  dt_ctl_t *ctl,
  const char *tmpdir,
  const char *name)
{
  DT_F("dttxchg_delete");
  char *filename = NULL;
  dt_rc_e rc;

  DT_C(dtos_malloc2,(ctl,				/* add tmpdir to name				*/
    (void **)&filename, 
    strlen(tmpdir)+strlen(name),
    F, "filename"));
  sprintf(filename, name, tmpdir);

  DT_C(dtos_unlink,(ctl, 
    strstr(filename, tmpdir)));

cleanup:
  if (filename != NULL)
    DT_I(dtos_free2,(ctl, filename,
      F, "filename"));
  return rc;
}

/*..........................................................................
 *..........................................................................*/

static
dt_rc_e
dttxchg_test(
  dt_ctl_t *ctl,
  const char *tmpdir,
  const char *name1,
  const char *name2,
  const char *name3,
  dtxy_t w,
  dtxy_t h,
  long flags,
  int *assert_fail)
{
  DT_F("dttxchg_test");

  if (flags & FLAG_CAN_WRITE)			/* this server can give images?	   	*/
  {
    DT_Q(dttxchg_export,(ctl, tmpdir,	
      name1,
      w/2+w/4, 0,
      w, h,
      dtimage_gen_test1,
	  flags,
      assert_fail));

    DT_Q(dttxchg_export,(ctl, tmpdir,	
      name2,
      w/4, 0,
      w/2, h/2,
      dtimage_gen_color256, 
	  flags,
      assert_fail));

    DT_Q(dttxchg_export,(ctl, tmpdir,	
      name3,
      0, 0,
      w/4, h/4,
      dtimage_gen_color256, 
	  flags,
      assert_fail));
  }

  if (flags & FLAG_CAN_READ)			/* this server can give images?	   	*/
  {
    DT_Q(dttxchg_import,(ctl, tmpdir,	
      name1, w, h,
      dtimage_gen_test1, 
	  flags,
      assert_fail));

    DT_Q(dttxchg_import,(ctl, tmpdir,	
      name2, w/2, h/2,
      dtimage_gen_color256, 
	  flags,
      assert_fail));

    DT_Q(dttxchg_import,(ctl, tmpdir,	
      name3, w/4, h/4,
      dtimage_gen_color256, 
	  flags,
      assert_fail));
  }

  if (flags & FLAG_CAN_DELETE)			/* we should delete tmp files?		*/
  {
    DT_Q(dttxchg_delete,(ctl, tmpdir,	
      name1));

    DT_Q(dttxchg_delete,(ctl, tmpdir,	
      name2));

    DT_Q(dttxchg_delete,(ctl, tmpdir,	
      name3));
  }

  return DT_RC_GOOD;
}

/*..........................................................................*/

dt_rc_e
DTCONFIG_API1
dttxchg_arg_check(						/* check args and params            */
  dt_ctl_t *ctl,
  void *app,
  dtparam_t *param,
  dt_enter_arg_t *arg,
  char *msg,
  const char **start)
{
  DT_F("dttxchg_arg_check");
  app_t *a = (app_t *)app;

  DT_Q(dtt_check_op2,(ctl, param,		/* make sure op is understood       */
    ops, &a->op, msg, start));

  if (msg[0] == '\0')					/* no problems yet? 				*/
  DT_Q(dtt_check_datadir,(ctl, param,	/* make sure datadir exists       	*/
    msg, start));

  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/

DTT_MAIN(dttxchg)
{
  DT_F("dttxchg");
  dt_ctl_t *ctl = &param->ctl;
  dt_enter_arg_t arg[1];
  app_t app, *a = &app;
  dtxy_t w, h;
#define TMPDIR param->output
  int assert_fail = 0;
  dt_rc_e rc;

  DTCSTACK_HIGHWATER;

  DT_C(dt_heapmon_init,(ctl));			/* init memory checking			    */

  DT_C(dtparam_init,(param));			/* init param structure             */

  DT_C(dtparam_set,(param, 0,          /* set default for params we want   */
    &param->op, NULL, NULL, "fast",
    &param->datadir, NULL, NULL, NULL,
    &param->output, NULL, NULL, NULL,
    &param->reps, NULL, NULL, "1",
    &param->w, NULL, NULL, "192",
    &param->h, NULL, NULL, "192",
    NULL));

  arg[0].desc = NULL;                   /* no arguments                     */
  DT_C(dt_enter_parse,(					/* go parse params and args			*/
    argc, argv, F, USAGE, param,
    arg, dttxchg_arg_check, a));

  w = (dtxy_t)param->w;					/* for short					    */
  h = (dtxy_t)param->h;

  {
    long reps;
    for (reps=0;
         reps < param->reps &&
         rc == DT_RC_GOOD;
         reps++)
    {
										/* ................................	*/
	  if (ISOP("all") ||
          ISOP("fast") ||
          ISOP("which"))
	  {
        DT_C(dttxchg_which,(ctl,		/* run dtxchg_which tests		    */
          &assert_fail));
      }

										/* ................................	*/
      if (ISOP("all") ||
          ISOP("fast") ||
          ISOP("dsk"))
      {
        DT_C(dttxchg_test,(ctl,			/* run simple tests to dsk server	*/
          TMPDIR,
          DSK_NAME1,
          DSK_NAME2,
          DSK_NAME3,
          w, h, 
          FLAG_CAN_WRITE |
          FLAG_CAN_READ,
          &assert_fail));
  	    DT_C(dtdsk_delete,(ctl,			/* remove all traces of dsk 		*/
		  TMPDIR, NULL));
      }
										/* ................................	*/
      if (ISOP("all") ||
          ISOP("fast") ||
          ISOP("pgm"))
      {
        DT_C(dttxchg_test,(ctl,	
          TMPDIR,
          PGM_NAME1,
          PGM_NAME2,
          PGM_NAME3,
          w, h,
          FLAG_CAN_WRITE | 
          FLAG_CAN_READ |
          FLAG_CAN_DELETE,
          &assert_fail));
      }
										/* ................................	*/
      if (ISOP("all") ||
          ISOP("fast") ||
          ISOP("wmf"))
      {
        DT_C(dttxchg_test,(ctl,	
          TMPDIR,
          WMF_NAME1,
          WMF_NAME2,
          WMF_NAME3,
          w, h, 
          FLAG_CAN_WRITE | 
          FLAG_CAN_READ,
          &assert_fail));
      }
										/* ................................	*/
      if (ISOP("all") ||
          ISOP("fast") ||
          ISOP("bmp"))
      {
        DT_C(dttxchg_test,(ctl,	
          TMPDIR,
          BMP_NAME1,
          BMP_NAME2,
          BMP_NAME3,
          w, h, 
          FLAG_CAN_WRITE | 
          FLAG_CAN_READ,
          &assert_fail));
      }
										/* ................................	*/
      if (ISOP("all") ||
          ISOP("wxx"))
      {
        DT_C(dttxchg_test,(ctl,	
          TMPDIR,
          WXX_NAME1,
          WXX_NAME2,
          WXX_NAME3,
          w, h, 
          FLAG_CAN_WRITE | 
          FLAG_CAN_READ,
          &assert_fail));
      }
										/* ................................	*/
      if (ISOP("all") ||
          ISOP("fast") ||
          ISOP("tiff"))
      {
		char tiff_name1[128];
		char tiff_name2[128];
		char tiff_name3[128];
		DT_Q(dtstr_printf,(ctl,
          tiff_name1,
  		  sizeof(tiff_name1),
          TIFF_NAME1, param->datadir));
		DT_Q(dtstr_printf,(ctl,
          tiff_name2,
  		  sizeof(tiff_name2),
          TIFF_NAME2, param->datadir));
		DT_Q(dtstr_printf,(ctl,
          tiff_name3,
  		  sizeof(tiff_name3),
          TIFF_NAME3, param->datadir));
    	if (dtprj_tiff)					/* we can do this? 					*/
          DT_C(dttxchg_test,(ctl,	
            TMPDIR,
            tiff_name1,
            tiff_name2,
            tiff_name3,
            w, h, FLAG_CAN_READ,
            &assert_fail))
    	else
    	if (ISOP("tiff"))				/* we specifically wanted this? 	*/
          assert_fail += DT_RC_GOOD !=
          dt_assert(ctl, 0, F,
            "not linked with tiff");
      }
										/* ................................	*/
      if (ISOP("all") ||
          ISOP("fast") ||
          ISOP("lj3"))
      {
        DT_C(dttxchg_test,(ctl,	
          TMPDIR,
          LJ300_NAME1,
          LJ300_NAME2,
          LJ300_NAME3,
          w, h, FLAG_CAN_WRITE|FLAG_BITIMAGE|FLAG_CAN_DELETE,
          &assert_fail));
      }
										/* ................................	*/
      if (ISOP("all") ||
          ISOP("fast") ||
          ISOP("lj6"))
      {
        DT_C(dttxchg_test,(ctl,	
          TMPDIR,
          LJ600_NAME1,
          LJ600_NAME2,
          LJ600_NAME3,
          w, h, FLAG_CAN_WRITE|FLAG_BITIMAGE|FLAG_CAN_DELETE,
          &assert_fail));
      }
	}
  }

                                        /* ................................ */
cleanup:
  DT_I(dt_leave,(param));				/* clean up before leaving          */

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
