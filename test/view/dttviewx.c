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
| :dttview: - Test Image Viewer Interaction
| 
|.SYNOPSIS
|:dttview [parameters]
|:dttviewf [parameters]
|:dttviews [parameters]
|_index dttview, :dttview:, Test Program
|_index Test Program,, :dttview: image viewer interaction
|_index image viewer interaction,, testing
|
|.SUMMARY
| Test image viewer interaction routines.
| Display test images.
| Simulate user keyboard input to exercise viewer capabilities.
| Expect no client messages.
| The lettered variants, :dttviewf: and :dttviews:
| are differently linked versions of the program.
|
|.PARAMETERS
| :-op: ~op~			operation, (default :all:)
| :-screen: ~scrspec~	screen to server to use, (default depends on system)
| :-output: ~directory~	directory for :dsk: files, (default :/tmp:)
| :-reps: ~repetitions~	number of times to iterate, (default 1)
| :-w: ~width~			of largest test image, (default 192)
| :-h: ~height~			of largest test image, (default 192)
| :-dt: ~milliseconds~	delay between simulated keystrokes, (default 200)
|dtinclude(params1a.dx)
| 
|.DESCRIPTION
| :dttview: tests user interaction with the image viewer functions.
| The test program simulates user input only,
| no real user input is expected.
|
| The purpose of this test is to establish a confidence level
| that the Dtack Imaging Software libraries
| can successfully handle user interaction in the manipulation,
| such as window deletion, motion, and saving,
| of displayed images on your computer.
|
| This program is linked in three different ways.
|_aside Same program, just linked with different libraries.
| The reason this is that including graphical support for more than
| one package at a time on DOS makes the programs to large.
| :dttview:		linked with all graphical support accept Fastgraph and SVGACC
| :dttviewf:	linked with Fastgraph support only
| :dttviews:	linked with SVGACC support only
|
| if ~scrspec~ indicates a true graphical display,
|_aside Expect this display sequence.
| i.e. not :-screen dummy:,
| you will see the following display sequence:
| 1.	three test images appear on the upper left, then disappear
| 2.	three test images appear again, the middle one disappears,
|		the other two dance around, then disappear
| 3.	15 regularly-spaced images pop up, then disappear
| Each step is followed by an "Info" window with a message about saving windows.
| Each step may be run individually using the :-op: parameter (see below).
|
|dtinclude(screen1.dx)
| 
| If ~scrspec~ is :dummy:, the tests will still be run,
| but no graphical display will appear.
| 
| Only :dsk: images are used as input because this program
| is not supposed to test client/server communications.
| A different program, :dttwin: does that.
|_index dttwin, :dttwin:, Test Program
|
| The :-output: ~directory~ is used to store the :dsk: files.
|_aside Temporary :dsk: files cleaned up.
| Any temporary files are removed from ~directory~ if the program
| executes to completion.
|
|.OPERATIONS 
| The :-op: parameter may take one of the following values.
| The default is :all:.
| :all:		do all three tests
| :test1:	simple 3-image input and saving
| :test2:	test simulated keyboard input, delete an image, move and save the
|			others
| :test3:	simple 15-image input and saving
|
|.EXAMPLES
| Test all operations, using the default native display for the system.
|
|:    dttview
|
| Test simulated user interaction only.
|
|:    dttview -op test2
|
| Same test, but also show successful assertions.
| Sample output is shown.
|
|:    dttview -op test2 -D assert
|:    dttview: dttview_export:   output dsk,/tmp "dttview1"
|:    dttview: dttview_export:   output dsk,/tmp "dttview2"
|:    dttview: dttview_export:   output dsk,/tmp "dttview3"
|:    dttview: dttview_import:   dttview1 OK
|:    dttview: dttview_import:   dttview3 OK
|:    dttview: dttview_test2:   window "dttview1" x0 134 (134), y0 10 (10)
|:    dttview: dttview_test2:   window "dttview2" successfully removed
|:    dttview: dttview_test2:   window "dttview3" x0 50 (50), y0 50 (50)
|:    dttview:   all memory freed
|
| Test everything and capture successful assertion messages
| and potential error messages in a file called :tmp.out:.
| This is important on DOS, where the graphics display can wipe
| out any text messages.
| 
|:    dttview -D assert -errto tmp.out -dbgto tmp.out
| 
| Test everything, but don't display any windows.
| 
|:    dttview -D assert -screen dummy
|
| Run until the cows come home.
|
|:    dttview -reps 10000
|
|dtinclude(params2.dx)
|
| END
 *..........................................................................*/

#include <dtack/t.h>
DT_RCSID("test/view $RCSfile: dttviewx.c,v $ $Revision: 1.1 $");
#include <dtack/lut8.h>
#include <dtack/xchg.h>

#include <dtack/list.h>
#include <dtack/svc.h>
#include <dtack/scr.h>

#include <dtack/win_r.h>					/* needed inside dtwins.h only 		*/
#include <dtack/wins.h>						/* we are an svc-on-scr server 		*/

#include <dtack/view.h>

#include <dtack/dsk.h>						/* needed to delete after disk test */

#include <dtack/mem.h>						/* needed for image comparison 		*/

#include <dttview.use>

static const char *ops[] = {
  "all",
  "test1",
  "test2",
  "test3",
  NULL};

#define PAGESIZE (1280)					/* as small as possible for DOS	    */

#define NAME1 "dttview1"
#define NAME2 "dttview2"
#define NAME3 "dttview3"

typedef dt_rc_e DTCONFIG_API1 image_gen_f(dt_ctl_t *, dtimage_t *);

static long global_dt = 0;
static dtscr_wait_event_f *global_wait_event = NULL;

/*..........................................................................
 *..........................................................................*/

dt_rc_e
dttview_gen(							/* generate image given function 	*/
  dt_ctl_t *ctl,
  dtimage_t *image,
  dtxy_t w,
  dtxy_t h,
  image_gen_f *gen,
  long flags)
{
  DT_F("dttview_gen");
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

dt_rc_e
dttview_cmp(							/* compare 8-bit images 			*/
  dt_ctl_t *ctl,
  dtimage_t *image1,
  dtimage_t *image2,
  const char *f,
  const char *description,
  int *assert_fail)
{
  DT_F("dttview_cmp");
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

dt_rc_e
dttview_export(
  dt_ctl_t *ctl,
  const char *tmpdir,
  const char *windowname,
  dtxy_t x0,
  dtxy_t y0,
  dtxy_t w,
  dtxy_t h,
  image_gen_f *gen,
  long flags,
  int *assert_fail)
{
  DT_F("dttview_export");
  dtimage_t image;
  char *winspec = NULL;
  dt_rc_e rc;

  DTCSTACK_HIGHWATER;
 
  image.xe = 0;

  DT_C(dttview_gen,(ctl, &image,		/* generate test image 				*/
    w, h, gen, flags));

  DT_C(dtos_strdup3,(ctl,				/* make up the win,dsk leader		*/
    "dsk", ",", tmpdir, &winspec));

  DT_C(dtxchg_win_export_image,(ctl,	/* output the whole image		    */
    winspec, windowname, 
    &image, x0, y0));

  dt_assert(ctl, 1,
    F, "output %s \"%s\"",
    winspec, windowname);

cleanup:
  if (winspec != NULL)
    DT_I(dtos_free2,(ctl, winspec,
      F, "winspec"));

  if (image.xe)
    DT_I(dtimage_free,(ctl, &image));	/* free test image 					*/

  return rc;
}

/*..........................................................................
| Import image and compare to freshly generated image.  
 *..........................................................................*/

dt_rc_e
dttview_import(
  dt_ctl_t *ctl,
  const char *tmpdir,
  const char *windowname,
  dtxy_t w,
  dtxy_t h,
  image_gen_f *gen,
  long flags,
  int *assert_fail)
{
  DT_F("dttview_import");
  dtimage_t img1;
  dtimage_t img2;
  char *winspec = NULL;
  dt_rc_e rc;

  DTCSTACK_HIGHWATER;
  
  img1.xe = 0;
  img2.xe = 0;
  
  DT_C(dttview_gen,(ctl, &img1,			/* generate test image 				*/
    w, h, gen, flags));

  DT_C(dtos_strdup3,(ctl,				/* make up the win,dsk leader		*/
    "dsk", ",", tmpdir, &winspec));

  DT_C(dtxchg_win_import_image,(ctl,	/* input the whole image		    */
    winspec, windowname, &img2,
    PAGESIZE, 1));

  DT_C(dttview_cmp,(ctl,				/* compare two images				*/
    &img1, &img2, F, windowname,
     assert_fail));

cleanup:
  if (img2.xe)
    DT_I(dtimage_free,(ctl, &img2));

  if (winspec != NULL)
    DT_I(dtos_free2,(ctl, winspec,
      F, "winspec"));

  if (img1.xe)
    DT_I(dtimage_free,(ctl, &img1));

  return rc;
}

/*..........................................................................
 * no interaction
 *..........................................................................*/

dt_rc_e
dttview_export3(
  dt_ctl_t *ctl,
  dtscr_t *scr,
  const char *tmpdir,
  dtxy_t w,
  dtxy_t h,
  int *assert_fail)
{
  DT_F("dttview_test");
  dt_rc_e rc;
  long flags = 0;

  DTCSTACK_HIGHWATER;

  DT_C(dttview_export,(ctl,				/* output images with dsk ipc 		*/
    tmpdir,	
    NAME1,
    w/2+w/4, 0,
    w, h,
    dtt_gen_color256,
	flags,
    assert_fail));

  DT_C(dttview_export,(ctl,	
    tmpdir,	
    NAME2,
    w/4, 0,
    w/2, h/2,
    dtt_gen_color256, 
	flags,
    assert_fail));

  DT_C(dttview_export,(ctl,	
    tmpdir,	
    NAME3,
    0, 0,
    w/4, h/4,
    dtt_gen_color256, 
	flags,
    assert_fail));

  DT_C(dtview_dsk,(ctl, scr, tmpdir));	/* put dsk ipc windows on screen 	*/

cleanup:

  return rc;
}

/*..........................................................................
 * delete one window by interaction
 *..........................................................................*/

dt_rc_e
dttview_check(
  dt_ctl_t *ctl,
  const char *tmpdir,
  const char *windowname,
  int exists,
  dtxy_t x0,
  dtxy_t y0,
  const char *testid,
  int *assert_fail)
{
  DT_F("dttview_test");
  dtlist_t list;
  dtlist_elem_t *elem;
  dt_rc_e rc;
  DT_Q(dtdsk_getlist,(ctl, tmpdir,		/* get dsk window list 				*/
    &list));
  DT_C(dtlist_search,(ctl, &list,		/* search for this one 				*/
    windowname, &elem));
  if (exists)							/* is it supposed to exist? 		*/
  {
	if (elem)
	{
	  dtdsk_win_t *win =
	    (dtdsk_win_t *)elem->user;
      dt_assert(ctl,  
        x0 == win->x0 && y0 == win->y0,
        testid, 
        "window \"%s\" x0 %u (%u), y0 %u (%u)",
        windowname, win->x0, x0, win->y0, y0);
	}
	else
      dt_assert(ctl, 0,
        testid, 
        "window \"%s\" should exist but doesn't",
        windowname);
  }
  else									/* supposed to be gone? 			*/
  {
	if (elem)							/* it is gone? 						*/
      dt_assert(ctl, 0,
        testid, 
        "window \"%s\" shouldn't exist"
        " but does",
        windowname);
	else								/* still hanging around 			*/
      dt_assert(ctl, 1,
        testid, 
        "window \"%s\" successfully removed",
        windowname);
  }
cleanup:
  DT_I(dtlist_free,(ctl, &list));		/* free all list elements 			*/
  DT_I(dtlist_destroy,(ctl, &list));	/* free list structure itself 		*/
  return rc;
}

/*..........................................................................
 * no interaction, few windows
 *..........................................................................*/

dt_rc_e
dttview_test1(
  dt_ctl_t *ctl,
  dtscr_t *scr,
  const char *tmpdir,
  dtxy_t w,
  dtxy_t h,
  int *assert_fail)
{
  DT_F("dttview_test1");
  dtlut8_t lut[1];
  dt_rc_e rc;
  long flags = 0;

  DT_C(dttview_export3,(ctl,			/* put 3 windows in the screen 		*/
    scr, tmpdir, w, h, assert_fail));

  DT_C(dtdsk_delete,(ctl, tmpdir,		/* remove all traces of dsk 		*/
	NULL));
  
  lut[0].r = 255;
  lut[0].g = 255;
  lut[0].b = 0;
  DT_C(dtscr_export_color,(ctl, scr,	/* just change one color 			*/
    NAME3, lut, 15, 1));

  DT_C(dtview_save,(ctl, scr, tmpdir));	/* save screen windows to dsk 		*/

  {
    DT_C(dttview_import,(ctl,
      tmpdir,	
      NAME1, w, h,
      dtt_gen_color256, 
	  flags,
      assert_fail));

    DT_C(dttview_import,(ctl,	
      tmpdir,	
      NAME2, w/2, h/2,
      dtt_gen_color256, 
	  flags,
      assert_fail));

    DT_C(dttview_import,(ctl,	
      tmpdir,	
      NAME3, w/4, h/4,
      dtt_gen_color256, 
	  flags,
      assert_fail));
  }

  DT_C(dttview_check,(ctl, tmpdir,		/* make sure NAME1 is there		    */
    NAME1, 1, w/2+w/4, 0,
    F, assert_fail));

  DT_C(dttview_check,(ctl, tmpdir,		/* make sure NAME2 is there		    */
    NAME2, 1, w/4, 0,
    F, assert_fail));

  DT_C(dttview_check,(ctl, tmpdir,		/* make sure NAME3 is there		    */
    NAME3, 1, 0, 0,
    F, assert_fail));

  DT_Q(dtos_delay,(ctl, global_dt));	/* pause at end						*/

cleanup:
  DT_I(dtdsk_delete,(ctl, tmpdir,		/* remove all traces of dsk 		*/
	NULL));

  return rc;
}

/*..........................................................................
 *..........................................................................*/

static int count2 = 0;

static
dt_rc_e
dttview_wait_event2(
  dt_ctl_t *ctl,
  dtscr_t *scr,
  dtsvc_t *svc,
  double timeout,
  int flag,
  dt_event_t *event)
{
  DT_F("dttview_wait_event2");
# define LATCH_KEY (' ')
  struct {
    dt_key_t key;
    const char *name;
  } script[] = 
  {
	{DT_KEY_DELETE,      NAME2},

	{DT_KEY_TAB,         NAME3},
	{LATCH_KEY,          NAME1},
	{DT_KEY_DOWNARROW,   NAME1},	
	{DT_KEY_LEFTARROW,   NAME1},
	{LATCH_KEY,          NAME1},

	{DT_KEY_TAB,         NAME1},
	{LATCH_KEY,          NAME3},
	{DT_KEY_RIGHTARROW,  NAME3},
	{DT_KEY_RIGHTARROW,  NAME3},
	{DT_KEY_RIGHTARROW,  NAME3},
	{DT_KEY_RIGHTARROW,  NAME3},
	{DT_KEY_RIGHTARROW,  NAME3},
	{LATCH_KEY,          NAME3},

	{DT_KEY_TAB,         NAME3},
	{LATCH_KEY,          NAME1},
	{DT_KEY_LEFTARROW,   NAME1},
	{DT_KEY_RIGHTARROW,  NAME1},
	{LATCH_KEY,          NAME1},

	{DT_KEY_TAB,         NAME1},
	{LATCH_KEY,          NAME3},
	{DT_KEY_DOWNARROW,   NAME3},
	{DT_KEY_DOWNARROW,   NAME3},
	{DT_KEY_DOWNARROW,   NAME3},
	{DT_KEY_DOWNARROW,   NAME3},
	{DT_KEY_DOWNARROW,   NAME3},
	{LATCH_KEY,          NAME3},

	{DT_KEY_TAB,         NAME1},

	{DTVIEW_SAVE_KEY_CHAR,  NULL},
	{DTSCR_KEY_QUIT,        NULL}
  }; 

  DT_MEMSET(event, 0, sizeof(*event));

  DT_Q(dtos_delay,(ctl, global_dt));	/* slow things down a bit 			*/

  if (global_wait_event)
  {
    DT_Q(global_wait_event,(ctl,		/* poll for real events 			*/
      scr, svc, -1, 0, event));
	if (event->type != 
		DT_EVENT_NOTHING)
	  return DT_RC_GOOD;
  }

  event->type = DT_EVENT_KEYBOARD;
  event->mousex = scr->cursorx;
  event->mousey = scr->cursory;
  event->keys[0] = DTSCR_KEY_QUIT;
  event->nkeys = 1;

  event->keys[0] = script[count2].key;
  if (script[count2].name != NULL)		/* event happened in some window? 	*/
  {
	dtlist_elem_t *elem;
	dtscr_win_t *win;
    DT_Q(dtlist_find,(ctl,				/* find window in list by name		*/
      &scr->windowlist, 
      script[count2].name,
      &elem));
    win = (dtscr_win_t *)elem->user;	/* access window structure  		*/
    event->id = win->id;				/* extract window id 				*/
  }

  count2++;

  return DT_RC_GOOD;
}

/*..........................................................................
 * delete one window by interaction
 *..........................................................................*/

dt_rc_e
dttview_test2(
  dt_ctl_t *ctl,
  dtscr_t *scr,
  const char *tmpdir,
  dtxy_t w,
  dtxy_t h,
  int *assert_fail)
{
  DT_F("dttview_test2");
  dt_rc_e rc;
  long flags = 0;

  DT_C(dttview_export3,(ctl,			/* put 3 windows in the screen 		*/
    scr, tmpdir, w, h, assert_fail));

  global_wait_event = scr->wait_event;	/* real function we can poll with 	*/
  
  scr->wait_event =						/* substitute our fake input 		*/
    dttview_wait_event2;

  DT_C(dtscr_state_keystring_action,	/* return to look on save character	*/
    (ctl, scr, &scr->states_default,
     DTVIEW_SAVE_KEY,
     DTSCR_ACTION_QUIT));

  while (1)								/* loop like dtview does			*/
  {
    DT_Q(dtscr_wait_event,(ctl,			/* wait for quit event				*/
      scr, NULL, 3));
    if (scr->last_event.keys[0] == 		/* user hit save key?			    */
        DTVIEW_SAVE_KEY[0])
	{
      DT_Q(dtview_save,(ctl, scr,		/* display save window			    */
        tmpdir));
	}
    else								/* quit for some other reason?		*/
    if (scr->last_event.keys[0] == 		/* user hit quit key?			    */
        DTSCR_KEY_QUIT)
      break;							/* must be to quit the program		*/
    else			
    if (scr->last_event.type ==			/* it was an ipc event?			    */
        DT_EVENT_IPC)
	{
	  rc = dt_err(ctl, F,
        "did not expect IPC event");
	  goto cleanup;
	}
  }

  {
    DT_C(dttview_import,(ctl,
      tmpdir,	
      NAME1, w, h,
      dtt_gen_color256, 
	  flags,
      assert_fail));

    DT_C(dttview_import,(ctl,	
      tmpdir,	
      NAME3, w/4, h/4,
      dtt_gen_color256, 
	  flags,
      assert_fail));
  }

  DT_C(dttview_check,(ctl, tmpdir,		/* make sure NAME1 is moved		    */
    NAME1, 1, w/2+w/4-10, 0+10,
    F, assert_fail));

  DT_C(dttview_check,(ctl, tmpdir,		/* make sure NAME2 is gone		    */
    NAME2, 0, 0, 0,
    F, assert_fail));

  DT_C(dttview_check,(ctl, tmpdir,		/* make sure NAME3 is moved		    */
    NAME3, 1, 0+50, 0+50,
    F, assert_fail));

cleanup:
  DT_I(dtdsk_delete,(ctl, tmpdir,		/* remove all traces of dsk 		*/
	NULL));

  return rc;
}

/*..........................................................................
 *..........................................................................*/

dt_rc_e
dttview_test3_export(
  dt_ctl_t *ctl,
  const char *tmpdir,
  const char *windowname,
  dtxy_t x0,
  dtxy_t y0,
  dtxy_t xe,
  dtxy_t ye)
{
  DT_F("dttview_test3_export");
  dtimage_t image;
  char *winspec = NULL;
  dt_rc_e rc;
#define BACKGROUND 64
#define FOREGROUND 192

  DTCSTACK_HIGHWATER;

  image.xe = 0;
  DT_C(dtimage_create3,(ctl, &image,	/* make new image 1				    */
    PAGESIZE, 1,
    xe, ye, DT_NTYPE_B08, NULL));

  DT_C(dtimage_constant,(ctl, 			/* generate test image				*/
    &image, BACKGROUND));
  
  DT_C(dtimage_plot_text_8x16,(ctl,		/* center name inside it 			*/
    &image, windowname, 
    (xe - strlen(windowname)*8) / 2,
    (ye - 16) / 2,
    FOREGROUND));

  DT_C(dtos_strdup3,(ctl,				/* make up the win,dsk leader		*/
    "dsk", ",", tmpdir, &winspec));

  DT_C(dtxchg_win_export_image,(ctl,	/* output the whole image		    */
    winspec, windowname, 
    &image, x0, y0));

  dt_assert(ctl, 1,
    F, "output %s \"%s\"",
    winspec, windowname);

cleanup:
  if (winspec != NULL)
    DT_I(dtos_free2,(ctl, winspec,
      F, "winspec"));

  if (image.xe)
    DT_I(dtimage_free,(ctl, &image));

  return rc;

#undef FOREGROUND
#undef BACKGROUND
}

/*..........................................................................
 * no interaction, lots of windows
 *..........................................................................*/

dt_rc_e
dttview_test3(
  dt_ctl_t *ctl,
  dtscr_t *scr,
  const char *tmpdir,
  dtxy_t w,
  dtxy_t h,
  int *assert_fail)
{
  DT_F("dttview_test3");
#define NX 5
#define NY 3
#define XB 16
#define YB 32
  dtxy_t x0, y0;
  dtxy_t xe = (scr->xe - (NX * XB)) / NX;
  dtxy_t ye = (scr->ye - (NY * YB)) / NY;
  int k;
  dt_rc_e rc;

  k = 0;
  for (y0=YB/2; y0+ye<scr->ye; y0+=ye)
    for (x0=XB/2; x0+xe<scr->xe; x0+=xe)
	{
	  char name[32];
	  sprintf(name, "%d", k++);
      DT_C(dttview_test3_export,(ctl,	/* put window in dsk ipc			*/
        tmpdir,	
        name,
        x0, y0,
        xe-XB, ye-YB));
	}

  DT_C(dtview_dsk,(ctl, scr, tmpdir));	/* put dsk ipc windows on screen 	*/

  DT_C(dtdsk_delete,(ctl, tmpdir,		/* remove all traces of dsk 		*/
	NULL));

  DT_C(dtview_save,(ctl, scr, tmpdir));	/* save screen windows to dsk 		*/

  DT_C(dtscr_refresh,(ctl, scr));

  k = 0;
  for (y0=YB/2; y0+ye<scr->ye; y0+=ye)
    for (x0=XB/2; x0+xe<scr->xe; x0+=xe)
	{
	  char name[32];
	  sprintf(name, "%d", k++);
	  DT_C(dttview_check,(ctl, tmpdir,	/* make sure window is there	    */
        name, 1, x0, y0,
        F, assert_fail));
	}

cleanup:
  DT_I(dtdsk_delete,(ctl, tmpdir,		/* remove all traces of dsk 		*/
	NULL));

  return rc;
}

/*..........................................................................*/
dt_rc_e
DTCONFIG_API1
dttview_arg_check(						/* check args and params            */
  dt_ctl_t *ctl,
  void *app,
  dtparam_t *param,
  dt_enter_arg_t *arg,
  char *msg,
  const char **start)
{
  DT_F("dttview_arg_check");

  DT_Q(dtt_check_op,(ctl, param,        /* make sure op is understood       */
    ops, msg, start));

  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/

dt_rc_e 
dttviewx(
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
  int assert_fail = 0;
  dt_rc_e rc;

  DT_C(dtos_heap_option,(ctl,			/* maximize memory usage		    */
    DTOS_HEAP_OPTION_NO_NEARHEAP));

  DT_C(dt_heapmon_init,(ctl));			/* init memory checking			    */

  DT_C(dtparam_init,(param));			/* init param structure             */

  DT_C(dtparam_set,(param, 0,			/* set default for params we want   */
    &param->op, NULL, NULL, "all",
    &param->reps, NULL, NULL, "1",
    &param->screen, NULL, NULL, NULL,
    &param->output, NULL, NULL, "/tmp",
    &param->w, NULL, NULL, "192",
    &param->h, NULL, NULL, "192",
    &param->dt, NULL, NULL, "200",
    NULL));

  arg[0].desc = NULL;                   /* no arguments                     */
  DT_C(dt_enter_parse,(					/* go parse params and args			*/
    argc, argv, F, usage, param,
    arg, dttview_arg_check, NULL));

  DTCSTACK_HIGHWATER;

  DT_C(dtscr_create,(ctl, &scr,			/* open display system 				*/
    param->screen));
  have_screen = 1;

  w = (dtxy_t)param->w;					/* for short					    */
  h = (dtxy_t)param->h;
  global_dt = (long)param->dt;

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
										/* ................................	*/
      if (!strcmp(param->op, "all") ||
          !strcmp(param->op, "test1"))
      {
        DT_C(dttview_test1,(ctl, &scr,	/* test dtview_dsk and dtview_save	*/
          param->output, w, h,
          &assert_fail));
	  }
      if (!strcmp(param->op, "all") ||
          !strcmp(param->op, "test2"))
      {
        DT_C(dttview_test2,(ctl, &scr,	/* run fake interaction tests		*/
          param->output, w, h,
          &assert_fail));
      }
      if (!strcmp(param->op, "all") ||
          !strcmp(param->op, "test3"))
      {
        DT_C(dttview_test3,(ctl, &scr,	/* the many-window test				*/
          param->output, w, h,
          &assert_fail));
      }
										/* ................................ */
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
