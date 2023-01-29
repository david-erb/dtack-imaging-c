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
| :dttscr: - Test Graphical Display of Images
| 
|.SYNOPSIS
|:dttscr [parameters]
|_index Test Program,, :dttscr: graphical display of images
|
|.SUMMARY
| Test image display on native graphical system.
| Display test images briefly.
| Expect no client messages or user interaction.
|
|.PARAMETERS
| :-op: ~op~			operation, (default :all:)
| :-screen: ~scrspec~	screen to server to use, (default depends on system)
| :-reps: ~repetitions~	number of times to iterate, (default 1)
| :-w: ~width~			of largest test image, (default 192)
| :-h: ~height~			of largest test image, (default 192)
| :-dt: ~milliseconds~	delay while image is displayed, (default 1000)
|dtinclude(params1a.dx)
| 
|.DESCRIPTION
| :dttscr: tests the ability of the native graphical system to display images.
| The test program displays images only,
| no user or client program input is expected.
|
| The purpose of this test is to establish a confidence level
| that the Dtack Imaging Software libraries
| can successfully display images on your computer.
|
| If ~scrspec~ indicates a true graphical display,
|_aside Expect this display sequence.
| i.e. not :-screen dummy:,
| you will see three test images appear on the upper left
| part of your monitor, then disappear.
|
|dtinclude(screen1.dx)
| 
| If ~scrspec~ is :dummy:, the test will still be run,
| but no graphical display will appear.
|
| The value of ~repetitions~ controls how many times the program
| runs the test.
| The dtscr_create() and dtscr_destroy() calls
| are inside the repetitions loop.
| At high repetitions,
| you will gain confidence in the ability of the interface
| to the windowing system to correctly allocate and free resources.
| 
|.OPERATIONS 
| The :-op: parameter may take one of the following values.
| The default is :all:.
| :all:		do all tests, currently the only one is :test1:
| :test1:	simple 3-image display
|
|.EXAMPLES
| Test using the default native display for the system.
|
|:    dttscr
|
| Same test, but also show successful assertions.
| Sample output is shown.
|
|:    dttscr -D assert
|:    dttscr: dttscr_export:   output "dttscr1"
|:    dttscr: dttscr_export:   output "dttscr2"
|:    dttscr: dttscr_export:   output "dttscr3"
|:    dttscr: dttscr_import:   dttscr1 OK
|:    dttscr: dttscr_import:   dttscr2 OK
|:    dttscr: dttscr_import:   dttscr3 OK
|:    dttscr:   all memory freed
|
| Run test and pause for five seconds while images are visible on screen.
| 
|:    dttscr -dt 5000
| 
| Test everything and capture successful assertion messages
| and potential error messages in a file called :tmp.out:.
| This is important on DOS, where the graphics display can wipe
| out any text messages.
| 
|:    dttscr -D assert -errto tmp.out -dbgto tmp.out
| 
| Test everything, but don't display any windows.
| 
|:    dttscr -D assert -screen dummy
|
| Run until the fat lady sings.
|
|:    dttscr -reps 10000
|
|dtinclude(params2.dx)
|
| END
 *..........................................................................*/

#include <dtack/t.h>
DT_RCSID("test/view $RCSfile: dttscrx.c,v $ $Revision: 1.1 $");
#include <dtack/lut8.h>
#include <dtack/xchg.h>

#include <dtack/list.h>
#include <dtack/svc.h>
#include <dtack/scr.h>

#include <dtack/win_r.h>					/* needed inside dtwins.h only 		*/
#include <dtack/wins.h>						/* we are an svc-on-scr server 		*/

#include <dtack/dsk.h>						/* needed to delete after disk test */

#include <dtack/mem.h>						/* needed for image comparison 		*/

#include <dttscr.use>

static const char *ops[] = {
  "all",
  "test1",
  NULL};

#define PAGESIZE 0

#define NAME1 "dttscr1"
#define NAME2 "dttscr2"
#define NAME3 "dttscr3"

/*..........................................................................
 *..........................................................................*/
static
dt_rc_e
dttscr_gen(								/* generate image given function 	*/
  dt_ctl_t *ctl,
  dtimage_t *image,
  dtxy_t w,
  dtxy_t h,
  dtt_image_gen_f *gen,
  long flags)
{
  DT_F("dttscr_gen");
  dt_rc_e rc;

  DTCSTACK_HIGHWATER;

  image->xe = 0;
  DT_C(dtimage_create3,(ctl, image,		/* make new image 1				    */
    PAGESIZE, 1,
    w, h, DT_NTYPE_B08, NULL));

  DT_C((*gen),(ctl, image));			/* generate test image				*/

  return DT_RC_GOOD;

cleanup:
  if (image->xe)
    DT_I(dtimage_free,(ctl, image));

  return rc;
}

/*..........................................................................
| This function is used to keep code size down.
| Library code could do it but it is optimized and very large.
 *..........................................................................*/
static
dt_rc_e
dttscr_cmp(								/* compare 8-bit images 			*/
  dt_ctl_t *ctl,
  dtimage_t *image1,
  dtimage_t *image2,
  const char *f,
  const char *description,
  int *assert_fail)
{
  DT_F("dttscr_cmp");
  dtxy_t x, y;
  if (image1->xe != image2->xe ||
	  image1->ye != image2->ye)
  {
	*assert_fail = *assert_fail + 1;
	dt_assert(ctl, 0, f,
	  "%s image size %ux%u wrong"
      " (%ux%u)", description,
      image1->xe, image1->ye,
      image2->xe, image2->ye);
	return DT_RC_GOOD;
  }

  for (y=0; y<image1->ye; y++)
  { 
	dt_ntype_b08_t *p1, *p2;
	DTIMAGE_GETROW(ctl, image1, y, &p1);
	DTIMAGE_GETROW(ctl, image2, y, &p2);
	
    for (x=0; x<image1->xe; x++)
	  if (p1[x] != p2[x])
	  {
		*assert_fail = *assert_fail + 1;
		dt_assert(ctl, 0, f,
          "%s pixel at (%u,%u)"
          " was 0x%02x (0x%02x)",
          description,
          x, y, p1[x], p2[x]);
		x = image1->xe;					/* just report the first problem 	*/
		y = image1->ye;
	  }
	DTIMAGE_UNMAPROW(ctl, image1, y);
	DTIMAGE_UNMAPROW(ctl, image2, y);
  }

  if (y == image1->ye)
    dt_assert(ctl, 1, f, "%s OK",
      description);

  return DT_RC_GOOD;
}

/*..........................................................................
| Generate and export image to disk server.
 *..........................................................................*/
static
dt_rc_e
dttscr_export(
  dt_ctl_t *ctl,
  dtscr_t *scr,
  const char *windowname,
  dtxy_t x0,
  dtxy_t y0,
  dtxy_t w,
  dtxy_t h,
  dtt_image_gen_f *gen,
  long flags,
  long n,
  int *assert_fail)
{
  DT_F("dttscr_export");
  dtimage_t image;
  dtos_time_t mark1, mark2;
  long i;
  char id[64];
  dt_rc_e rc;

  DTCSTACK_HIGHWATER;
 
  image.xe = 0;

  DT_C(dttscr_gen,(ctl, &image,			/* generate test image 				*/
    w, h, gen, flags));

  DT_C(dtos_time,(ctl, &mark1));
  for (i=0; i<n; i++)
  {
    DT_C(dtxchg_scr_export_image,(ctl,	/* output the whole image		    */
      scr, windowname, 
      &image, x0, y0));
  }
  DT_C(dtos_time,(ctl, &mark2));
  
  dtstr_printf(ctl, id, sizeof(id),
    "export %ux%u image n=%ld",
    w, h, n);
  DT_C(dtt_time_report,(ctl,			/* report time					    */
    &mark1, &mark2, id));

  dt_assert(ctl, 1,
    F, "output \"%s\"",
    windowname);

cleanup:

  if (image.xe)
    DT_I(dtimage_free,(ctl, &image));	/* free test image 					*/

  return rc;
}

/*..........................................................................
 * no interaction
 *..........................................................................*/
static
dt_rc_e
dttscr_export3(
  dt_ctl_t *ctl,
  dtscr_t *scr,
  dtxy_t w,
  dtxy_t h,
  long n,
  int *assert_fail)
{
  DT_F("dttscr_test");
  dt_rc_e rc;
  long flags = 0;

  DTCSTACK_HIGHWATER;

  DT_C(dttscr_export,(ctl,				/* output images directly to screen	*/
    scr,	
    NAME1,
    w/2+w/4, 0,
    w, h,
    dtimage_gen_test1,
	flags,
	n,
    assert_fail));

  DT_C(dttscr_export,(ctl,	
    scr,	
    NAME2,
    w/4, 0,
    w/2, h/2,
    dtimage_gen_color256,
	flags,
	n,
    assert_fail));

  DT_C(dttscr_export,(ctl,	
    scr,	
    NAME3,
    0, 0,
    w/4, h/4,
    dtimage_gen_color256,
	flags,
	n,
    assert_fail));

cleanup:

  return rc;
}

/*..........................................................................
| Import image and compare to freshly generated image.  
 *..........................................................................*/
static
dt_rc_e
dttscr_import(
  dt_ctl_t *ctl,
  dtscr_t *scr,
  const char *windowname,
  dtxy_t w,
  dtxy_t h,
  dtt_image_gen_f *gen,
  long flags,
  long n,
  int *assert_fail)
{
  DT_F("dttscr_import");
  dtimage_t img1;
  dtimage_t img2;
  dtos_time_t mark1, mark2;
  long i;
  char id[64];
  dt_rc_e rc;

  DTCSTACK_HIGHWATER;
  
  img1.xe = 0;
  img2.xe = 0;
  
  DT_C(dttscr_gen,(ctl, &img1,			/* generate test image 				*/
    w, h, gen, flags));

  DT_C(dtos_time,(ctl, &mark1));
  for (i=0; i<n; i++)
  {
    DT_C(dtxchg_scr_import_image,(ctl,	/* input the whole image		    */
      scr, windowname, &img2,
      PAGESIZE, 1));
	if (i != n-1)
	{
      DT_C(dtimage_free,(ctl, &img2));
	  img2.xe = 0;
	}
  }
  DT_C(dtos_time,(ctl, &mark2));
  
  dtstr_printf(ctl, id, sizeof(id),
    "import %ux%u image n=%ld",
    w, h, n);
  DT_C(dtt_time_report,(ctl,			/* report time					    */
    &mark1, &mark2, id));

  DT_C(dttscr_cmp,(ctl,					/* compare two images				*/
    &img1, &img2, F, windowname,
    assert_fail));

cleanup:
  if (img2.xe)
    DT_I(dtimage_free,(ctl, &img2));

  if (img1.xe)
    DT_I(dtimage_free,(ctl, &img1));

  return rc;
}

/*..........................................................................
 * no interaction
 *..........................................................................*/
static
dt_rc_e
dttscr_import3(
  dt_ctl_t *ctl,
  dtscr_t *scr,
  dtxy_t w,
  dtxy_t h,
  long n,
  int *assert_fail)
{
  DT_F("dttscr_test");
  dt_rc_e rc;
  long flags = 0;

  DTCSTACK_HIGHWATER;

  DT_C(dttscr_import,(ctl,				/* output images with dsk ipc 		*/
    scr,	
    NAME1,
    w, h,
    dtimage_gen_test1,
	flags,
	n,
    assert_fail));

  DT_C(dttscr_import,(ctl,	
    scr,	
    NAME2,
    w/2, h/2,
    dtimage_gen_color256,
	flags,
	n,
    assert_fail));

  DT_C(dttscr_import,(ctl,	
    scr,	
    NAME3,
    w/4, h/4,
    dtimage_gen_color256,
	flags,
	n,
    assert_fail));

cleanup:

  return rc;
}

/*..........................................................................
 * no interaction, few windows
 *..........................................................................*/
static
dt_rc_e
dttscr_test1(
  dt_ctl_t *ctl,
  dtscr_t *scr,
  const char *tmpdir,
  dtxy_t w,
  dtxy_t h,
  long n,
  long dt,
  int *assert_fail)
{
  DT_F("dttscr_test1");
  dt_rc_e rc;

  DT_C(dttscr_export3,(ctl,				/* put 3 windows in the screen 		*/
    scr, w, h, n, assert_fail));

  DT_C(dttscr_import3,(ctl,				/* verify 3 windows in the screen	*/
    scr, w, h, n, assert_fail));

  if (dt == -1)							/* interact at end? 				*/
  {
	dt_say(ctl, (const char *)NULL,
      "hit ^D to close window,"
      " ^Q to terminate the application");
	while (1)
	{
	  dt_rc_e rc;
      rc = dtscr_wait_event(ctl,		/* wait for quit event				*/
        scr, (dtsvc_t *)NULL, 3);
	  if (rc != DT_RC_GOOD)
		break;

	  dt_dbg(ctl, F, DT_DBG_MASK_ASYNC,
        "event type %d bubbled up",
        scr->last_event.id);

	  if (scr->last_event.keys[0] ==	/* user hit quit key?			    */
		  DTSCR_KEY_QUIT)
		break;							/* must be to quit the program		*/
	}
  }
  else
  {
    DT_Q(dtos_delay,(ctl, dt));			/* pause at end						*/
  }

cleanup:

  return rc;
}

/*..........................................................................*/

dt_rc_e
DTCONFIG_API1
dttscr_arg_check(						/* check args and params            */
  dt_ctl_t *ctl,
  void *app,
  dtparam_t *param,
  dt_enter_arg_t *arg,
  char *msg,
  const char **start)
{
  DT_F("dttscr_arg_check");

  DT_Q(dtt_check_op,(ctl, param,        /* make sure op is understood       */
    ops, msg, start));

  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/

dt_rc_e 
dttscrx(
  const char *F,
  int argc,
  char *argv[],
  dtparam_t *param,
  int dummy)
{
  dt_ctl_t *ctl = &param->ctl;
  dt_enter_arg_t arg[1];
  dtscr_t scr;
  int have_screen = 0;
  dtxy_t w, h;
  long dt;
  int assert_fail = 0;
  dt_rc_e rc;

  DTCSTACK_HIGHWATER;

  DT_C(dt_heapmon_init,(ctl));			/* init memory checking			    */

  DT_C(dtparam_init,(param));			/* init param structure             */

  DT_C(dtparam_set,(param, 0,			/* set default for params we want   */
    &param->op, NULL, NULL, "all",
    &param->reps, NULL, NULL, "1",
    &param->screen, NULL, NULL, NULL,
    &param->w, NULL, NULL, "192",
    &param->h, NULL, NULL, "192",
    &param->n, "Timing loops", NULL, "192",
    &param->dt, "Delay between reps", NULL, "1000",
    NULL));

  arg[0].desc = NULL;                   /* no arguments                     */
  DT_C(dt_enter_parse,(					/* go parse params and args			*/
    argc, argv, F, usage, param,
    arg, dttscr_arg_check, NULL));

  w = (dtxy_t)param->w;					/* for short					    */
  h = (dtxy_t)param->h;
  dt = (long)param->dt;

  if (strcmp(param->op, "all") &&		/* special op?   					*/
	  dummy)							/* but dummy linked? 				*/
    rc = dt_err(ctl, F,
      "special op \"%s\" requested"
      " for dummy module", param->op);
  else
  {
    long reps;
    for (reps=0;
         reps < param->reps &&
         rc == DT_RC_GOOD;
         reps++)
    {
	  
	  DT_C(dtscr_create,(ctl, &scr,		/* open window system 				*/
        param->screen));
      have_screen = 1;
										/* ................................	*/
      if (!strcmp(param->op, "all") ||
          !strcmp(param->op, "test1"))
      {
        DT_C(dttscr_test1,(ctl, &scr,	
          param->output, w, h, 
          param->n, dt,
          &assert_fail));
	  }
										/* ................................ */
	  DT_C(dtscr_destroy,(ctl, &scr));	/* close window system 				*/
	  have_screen = 0;
	}
  }

                                        /* ................................ */
cleanup:
  if (have_screen)
  DT_I(dtscr_destroy,(ctl, &scr));

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
