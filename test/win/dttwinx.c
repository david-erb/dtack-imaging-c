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
| :dttwin: - Test Client/Server Windowing
| 
|.SYNOPSIS
|:dttwin [parameters]
|_index dttwin, :dttwin:, Test Program
|_index Test Program,, :dttwin: client/server windowing
|_index client/server windowing,, testing
|
|.SUMMARY
| Test client communication with windowing server.
| Fork server (if :-win ipc:), run client, run server (if :-win dsk:).
| Expect no screen interaction with user.
|
|.PARAMETERS
| :-op: ~op~			operation, (default :both:)
| :-win: ~winspec~		client/server communications method, (default all)
| :-screen: ~scrspec~	screen to server to use, (default depends on system)
| :-output: ~directory~	directory for :dsk: files, (default :/tmp:)
| :-reps: ~repetitions~	number of times to iterate, (default 1)
| :-w: ~width~			of largest test image, (default 192)
| :-h: ~height~			of largest test image, (default 192)
|dtinclude(params1a.dx)
| 
|.DESCRIPTION
| :dttwin: tests the communication between application clients
| and windowing servers.
|
| The purpose of this test is to establish a confidence level
| that the Dtack Imaging Software libraries
| can successfully perform client/server communications,
| where the server can display images on your computer.
|
| The client generates three test images of different sizes
| and sends them to the server.
| Then it asks the server for those three images back
| and compares the data to the original values.
| 
| The client does not know that a special test server is running.
| You may run the client part alone using :-op client:
|_aside Use this to test the Dtack Viewer.
| in the presence of :dtview:, the Dtack Viewer.
| This will then test interaction with the viewer.
| Note that this test program sends a command to the viewer
| which causes the viewer to exit.
|
| The client uses the :dtxchg_win: Function Family directly.
|_aside Client bypasses normal image output.
| dtimage_export() and dtimage_import() are not called.
| This reduces the size of the executable by more than 40K on DOS
| because special file formats and file compression are not needed.
| 
| The server part of :dttwin: simply receives the requests and
| dispatches them to the appropriate routine in the :dtscr: Function Family.
| When the server part has a ~scrspec~ which indicates a real display
| system, then the three windows sent by the client are displayed
| in the upper left area of your monitor.
| 
| The server part covers a large part of the server functionality of
| :dtview:, the Dtack Viewer.
|_index dtview, :dtview:, testing
| However, it does not cover real or simulated interactive input.
| Please refer to the test program :dttview:.
|_index dttview, :dttview:, Test Program
| 
| If ~winspec~ is not given, then the program tests all
|_aside All available communications tested.
| available client/server
| communications means defined for the current system.
| Currently, these possibilities include:
| :ipc,socket:	TCP/IP sockets
| :dsk:			client writes disk files, server reads them
|_index dsk, :dsk: client/server communications, testing
|
|dtinclude(screen1.dx)
| 
| The :-output: ~directory~ is used only when ~winspec~
|_aside Temporary :dsk: files cleaned up.
| indicates the :dsk: method of client/server communication.
| Any temporary files are removed from ~directory~ if the program
| executes to completion.
|
|.OPERATIONS 
| The :-op: parameter may take one of the following values.
| The default is :both:.
| If ~winspec~ indicates true interprocess communication, these values mean:
| :both:	fork server and run client simultaneously
| :client:	do just client, expect server already up and blocking
| :server:	do just server, block waiting for client
| 
| If ~winspec~ indicates :dsk:, these values mean:
| :both:	run client first then server, delete files when done
| :client:	do just client, leave files in :-output: ~directory~
| :server:	do just server, expect files, delete when done
|
|.EXAMPLES
| Test all available client/server communications for this system,
| using the default native display for the system.
|
|:    dttwin
|
| Same test, but also show successful assertions.
| Sample output from a system supporting sockets is shown.
|
|:    dttwin -D assert
|:    dttwin: dttwin_export:   output svcspec ipc,socket,5711, "first dttwin"
|:    dttwin: dttwin_export:   output svcspec ipc,socket,5711, "second dttwin"
|:    dttwin: dttwin_export:   output svcspec ipc,socket,5711, "third dttwin"
|:    dttwin: dttwin_import:   ipc,socket,5711,first dttwin OK
|:    dttwin: dttwin_import:   ipc,socket,5711,second dttwin OK
|:    dttwin: dttwin_import:   ipc,socket,5711,third dttwin OK
|:    dttwin: dttwin_screen:   window first dttwin OK
|:    dttwin: dttwin_screen:   window second dttwin OK
|:    dttwin: dttwin_screen:   window third dttwin OK
|:    dttwin:   all memory freed
|:    dttwin: dttwin_export:   output svcspec dsk,/tmp, "first dttwin"
|:    dttwin: dttwin_export:   output svcspec dsk,/tmp, "second dttwin"
|:    dttwin: dttwin_export:   output svcspec dsk,/tmp, "third dttwin"
|:    dttwin: dttwin_import:   dsk,/tmp,first dttwin OK
|:    dttwin: dttwin_import:   dsk,/tmp,second dttwin OK
|:    dttwin: dttwin_import:   dsk,/tmp,third dttwin OK
|:    dttwin: dttwin_screen:   window first dttwin OK
|:    dttwin: dttwin_screen:   window second dttwin OK
|:    dttwin: dttwin_screen:   window third dttwin OK
|:    dttwin:   all memory freed
|
| Same test, but capture all messages in a file called :tmp.out:.
| This is important on DOS, where the graphics display can wipe
| out any text messages.
| 
|:    dttwin -D assert -errto tmp.out -dbgto tmp.out
| 
| Same test, but don't display any windows.
| 
|:    dttwin -D assert -screen dummy
| 
| Only test :dsk: client/server communications, using current directory
| to hold temporary files.
| Display nothing, print nothing.
| 
|:    dttwin -win dsk,. -screen dummy
| 
| On a Unix-like system, run the server with some debug enabled
| in the background.  Then run the client.
| 
|:    dttwin -op server -D ipc+win &
|:    dttwin -op client
| 
| On any system, including DOS, run just the :dsk: client,
| leaving temporary files in the mytmp directory.
|
|:    mkdir mytmp
|:    dttwin -win dsk,mytmp -op client
|:    ls -l mytmp    (on DOS use: dir mytmp)
|:    -rw-r--r--  1 erb  110    128 Nov 12 05:32 DTDSK.LST
|:    -rw-r--r--  1 erb  110  36864 Nov 12 05:32 DTDSK000.DAT
|:    -rw-r--r--  1 erb  110   9216 Nov 12 05:32 DTDSK001.DAT
|:    -rw-r--r--  1 erb  110   2304 Nov 12 05:32 DTDSK002.DAT
|
| On any system, including DOS, run just the :dsk: server,
| expecting temporary files in the mytmp directory
| as from the previous example.
| The temporary files are removed by the test.
| 
|:    dttwin -win dsk,mytmp -op server -screen dummy
|
| Run all night.
|
|:    dttwin -reps 10000
| 
|dtinclude(params2.dx)
|
| END
 *..........................................................................*/

#include <dtack/t.h>
DT_RCSID("test/win $RCSfile: dttwinx.c,v $ $Revision: 1.1 $");
#include <dtack/lut8.h>
#include <dtack/xchg.h>

#include <dtack/list.h>
#include <dtack/svc.h>
#include <dtack/scr.h>

#include <dtack/win_r.h>					/* needed inside dtwins.h only 		*/
#include <dtack/wins.h>						/* we are an svc-on-scr server 		*/

#include <dtack/prj.h>						/* needed to decide what to be done */

#include <dtack/dsk.h>						/* needed to delete after disk test */

#include <dtack/mem.h>						/* needed for image comparison 		*/

#include <dttwin.use>

static const char *ops[] = {
  "both",
  "client",
  "server",
  NULL};

#define QNX4_IPC   "ipc,qnx4"
#define SOCKET_IPC "ipc,socket"
#define DSK_IPC    "dsk"

#define NAME1 "first dttwin"
#define NAME2 "second dttwin"
#define NAME3 "third dttwin"

#define FLAG_CAN_READ 1
#define FLAG_CAN_WRITE 2
#define FLAG_DO_SERVER 8
#define FLAG_DO_CLIENT 16
#define FLAG_DO_BACKGROUND 32
#define FLAG_NONE 0

typedef dt_rc_e DTCONFIG_API1 gen_f(dt_ctl_t *, dtimage_t *);

#ifdef DTCONFIG_IMPLIED_UNIXLIKE

#ifdef DTCONFIG_IMPLIED_QNX4
#  include <time.h>						/* for localtime() call 			*/
#endif

#define VARIANT(N) \
  (!strcmp(param->variant, #N))

#define CAPTURE_MAX (256)				/* max size of error capture 		*/

/*..........................................................................
 *..........................................................................*/

static
void
dttwin_stamp(							/* output debug message				*/
  dt_ctl_t *ctl,						/* environment control 				*/
  const char *caller,					/* caller's name 					*/
  char *s,
  int lmax)
{
  int pid;
  time_t clock = time(NULL);			/* current time 					*/
  struct tm *t = localtime(&clock);		/* format into fields 				*/
  strftime(s, lmax,						/* express as string 				*/
    "%Y%m%d.%H%M.%S", t);

  dtos_getpid(NULL, &pid);

  dtstr_printf(ctl,						/* tack on the caller name and pid	*/
    s+strlen(s), lmax-strlen(s),
    " [%5d]%s%s", pid, 
    caller? " ": "",
    caller? caller: "");
}

/*..........................................................................
 *..........................................................................*/

static
void
DTCONFIG_API2
dttwin_dbg(								/* output debug message				*/
  dt_ctl_t *ctl,						/* environment control 				*/
  const char *caller,					/* caller's name 					*/
  const long class_bits,				/* debug message class 				*/
  const char *format,					/* message format, like sprintf() 	*/
  va_list ap)							/* format substitutions 			*/
{
  char s[64];
  ctl->dbg = NULL;						/* don't come back here 			*/
  dttwin_stamp(NULL, caller, 
    s, sizeof(s));
  dt_dbg_va(ctl, s, class_bits,			/* let dt_dbg_va() do all the work 	*/
    format, ap);
  ctl->dbg = dttwin_dbg;				/* come back here next time 		*/
}

/* ........................................................................ *
 * ........................................................................ */

static
dt_rc_e
DTCONFIG_API2
dttwin_err(								/* handle errors					*/
  dt_ctl_t *ctl,						/* environment control 				*/
  const char *caller,					/* caller's name 					*/
  const char *format,					/* message format, like sprintf() 	*/
  va_list ap)							/* format substitutions 			*/
{
  dt_rc_e rc;
  char s[64];
  ctl->err = NULL;						/* don't come back here 			*/
  dttwin_stamp(NULL, caller, 
    s, sizeof(s));
  rc = dt_err_va(ctl, s,
    format, ap);
  ctl->err = dttwin_err;				/* come back here next time 		*/
  return rc;
}

/* ........................................................................ *
 * ........................................................................ */

static
dt_rc_e
DTCONFIG_API2
dttwin_err_capture(						/* handle errors					*/
  dt_ctl_t *ctl,						/* environment control 				*/
  const char *caller,					/* caller's name 					*/
  const char *format,					/* message format, like sprintf() 	*/
  va_list ap)							/* format substitutions 			*/
{
  dt_rc_e rc = DT_RC_BAD;
  char *s = (char *)ctl->user;
  dtstr_printf_va(NULL,					/* capture the error message 		*/
    (char *)ctl->user, CAPTURE_MAX,
    format, ap);
  return rc;
}

/*..........................................................................*
 *..........................................................................*/

static
void
DTCONFIG_API2
dttwin_say(							/* output console message			*/
  dt_ctl_t *ctl,						/* environment control 				*/
  const char *caller,					/* caller's name 					*/
  const char *format,					/* message format, like sprintf() 	*/
  va_list ap)							/* format substitutions 			*/
{
  char s[64];
  ctl->say = NULL;						/* don't come back here 			*/
  dttwin_stamp(NULL, caller, 
    s, sizeof(s));
  dt_say_va(ctl, caller,				/* let dt_say_va() do all the work 	*/
    format, ap);
  ctl->say = dttwin_say;				/* come back here next time 		*/
}

/*..........................................................................*
 *..........................................................................*/
#else
#define dttwin_dbg NULL
#define dttwin_err NULL
#define dttwin_say NULL
#endif

/*..........................................................................
 *..........................................................................*/

static
dt_rc_e
dttwin_gen(
  dt_ctl_t *ctl,
  dtimage_t *image,
  dtxy_t w,
  dtxy_t h,
  gen_f *gen,
  long flags)
{
  DT_F("dttwin_gen");
  dt_rc_e rc;
 
  image->xe = 0;
  DT_C(dtimage_create3,(ctl, image,		/* make new image 1				    */
    0, 1,								/* only need one page at a time 	*/
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
dttwin_cmp(								/* compare 8-bit images 			*/
  dt_ctl_t *ctl,
  dtimage_t *image1,
  dtimage_t *image2,
  const char *f,
  const char *description,
  int *assert_fail)
{
  DT_F("dttwin_cmp");
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
 *..........................................................................*/

static
dt_rc_e
dttwin_export(
  dt_ctl_t *ctl,
  const char *svcspec,
  const char *name,
  dtxy_t x0,
  dtxy_t y0,
  dtxy_t w,
  dtxy_t h,
  gen_f *gen,
  long flags,
  int *assert_fail)
{
  DT_F("dttwin_export");
  dtimage_t image;
  dt_rc_e rc;

  image.xe = 0;

  DT_C(dttwin_gen,(ctl, &image,			/* generate test image 				*/
    w, h, gen, flags));

  DT_C(dtxchg_win_export_image,(ctl,	/* output the whole image		    */
    svcspec, name, &image, x0, y0));
 
  dt_assert(ctl, 1, F,
    "output svcspec %s, \"%s\"",
    svcspec, name);

cleanup:
  if (image.xe != 0)
    DT_I(dtimage_free,(ctl, &image));

  return rc;
}

/*..........................................................................
 *..........................................................................*/

static
dt_rc_e
dttwin_import(
  dt_ctl_t *ctl,
  const char *svcspec,
  const char *name,
  dtxy_t w,
  dtxy_t h,
  gen_f *gen,
  long flags,
  int *assert_fail)
{
  DT_F("dttwin_import");
  dtimage_t img1;
  dtimage_t img2;
  char *description = NULL;
  dt_rc_e rc;
  
  img1.xe = 0;
  img2.xe = 0;
  
  DT_C(dttwin_gen,(ctl, &img1,			/* generate test image 				*/
    w, h, gen, flags));

  DT_C(dtxchg_win_import_image,(ctl,	/* input the whole image		    */
    svcspec, name, &img2,
    0, 1));								/* only need one page at a time 	*/

  DT_C(dtos_strdup3,(ctl, 
    svcspec, ",", name, 
    &description));
  
  DT_C(dttwin_cmp,(ctl,					/* compare two images				*/
    &img1, &img2, F, description,
    assert_fail));

cleanup:
  if (description != NULL)
    DT_I(dtos_free2,(ctl, description, 
      F, "description"));
  if (img2.xe)
    DT_I(dtimage_free,(ctl, &img2));
  if (img1.xe)
    DT_I(dtimage_free,(ctl, &img1));

  return rc;
}

/*..........................................................................
| Make sure the specified test image is in the screen.
 *..........................................................................*/

static
dt_rc_e
dttwin_screen(
  dt_ctl_t *ctl,
  dtscr_t *scr,
  const char *windowname,
  dtxy_t w,
  dtxy_t h,
  gen_f *gen,
  long flags,
  int *assert_fail)
{
  DT_F("dttwin_screen");
  dtimage_t img1;
  dtimage_t img2;
  char *description = NULL;
  dt_rc_e rc;
  
  img1.xe = 0;
  img2.xe = 0;
  
  DT_C(dttwin_gen,(ctl, &img1,			/* generate test image 				*/
    w, h, gen, flags));

  DT_C(dtxchg_scr_import_image,(ctl,	/* input the whole image		    */
    scr, windowname, &img2,
    0, 1));								/* only need one page at a time 	*/

  DT_C(dtos_strdup3,(ctl, 
    "window", " ", windowname, 
    &description));
  
  DT_C(dttwin_cmp,(ctl,					/* compare two images				*/
    &img1, &img2, F, description,
    assert_fail));

cleanup:
  if (description != NULL)
    DT_I(dtos_free2,(ctl, description, 
      F, "description"));
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
dttwin_client_test(
  dt_ctl_t *ctl,
  const char *svcspec,
  const char *name1,
  const char *name2,
  const char *name3,
  dtxy_t w,
  dtxy_t h,
  long flags,
  int *assert_fail)
{
  DT_F("dttwin_client_test");

  if (flags & FLAG_CAN_WRITE)			/* this server can give images?	   	*/
  {
    DT_Q(dttwin_export,(ctl, svcspec,	
      name1,
      w/2+w/4, 0,
      w, h,
      dtt_gen_color256,
	  flags,
      assert_fail));

    DT_Q(dttwin_export,(ctl, svcspec,	
      name2,
      w/4, 0,
      w/2, h/2,
      dtt_gen_color256, 
	  flags,
      assert_fail));

    DT_Q(dttwin_export,(ctl, svcspec,	
      name3,
      0, 0,
      w/4, h/4,
      dtt_gen_color256, 
	  flags,
      assert_fail));
  }

  if (flags & FLAG_CAN_READ)			/* this server can give images?	   	*/
  {
    DT_Q(dttwin_import,(ctl, svcspec,	
      name1, w, h,
      dtt_gen_color256, 
	  flags,
      assert_fail));

    DT_Q(dttwin_import,(ctl, svcspec,	
      name2, w/2, h/2,
      dtt_gen_color256, 
	  flags,
      assert_fail));

    DT_Q(dttwin_import,(ctl, svcspec,	
      name3, w/4, h/4,
      dtt_gen_color256, 
	  flags,
      assert_fail));
  }

  return DT_RC_GOOD;
}

/*..........................................................................
 * run ipc server
 *..........................................................................*/

static
dt_rc_e
dttwin_server_test(
  dt_ctl_t *ctl,
  const char *svcspec,
  const char *scrspec,
  const char *name1,
  const char *name2,
  const char *name3,
  dtxy_t w,
  dtxy_t h,
  long flags,
  int *isserver,
  int *assert_fail)
{
  DT_F("dttwin_server_test");
  dtsvc_t svc;
  dtscr_t scr;
  int have_scr = 0;
  dtwins_t wins;
  int have_wins = 0;
  dt_rc_e rc;

  svc.priv = NULL;

  if (flags & FLAG_DO_BACKGROUND)
    flags = DTSVC_FLAG_SELF_BACKGROUND;
  else
    flags = 0;

  dt_dbg(ctl, F, DT_DBG_MASK_IPC,
    "svcspec is \"%s\"", svcspec);

  DT_C(dtsvc_register,(ctl,				/* register public name			    */
    &svc, svcspec, flags));
  if (flags != 0 &&						/* were supposed to go background? 	*/
      !(svc.flags &						/* the foreground is returning? 	*/
      DTSVC_FLAG_SELF_BACKGROUND))
  {
    *isserver = 0;
	goto cleanup;
  }

  *isserver = 1;						/* we are the server 				*/

  DT_C(dtscr_create,(ctl, &scr,			/* open window system				*/
    scrspec));
  have_scr = 1;

  DT_C(dtwins_init,(ctl, &wins, &scr));	/* prepare to serve 				*/
  have_wins = 1;

  while (rc == DT_RC_GOOD)
  {
	DT_C(svc.wait,(ctl, &svc));			/* wait for svc work 				*/
	DT_C(dtwins_dispatch,(ctl,			/* handle the work 					*/
	  &wins, &svc));
  }

cleanup:

  if (rc == DT_RC_STOP)					/* just got a stop from client? 	*/
  {
	rc = DT_RC_GOOD;					/* don't get excited 				*/

	DT_GI(dttwin_screen,(ctl, &scr,		/* check windows in scr			    */
      name1, w, h,
      dtt_gen_color256, 
	  flags,
      assert_fail));

    DT_GI(dttwin_screen,(ctl, &scr,
      name2, w/2, h/2,
      dtt_gen_color256, 
  	  flags,
      assert_fail));

    DT_GI(dttwin_screen,(ctl, &scr,
      name3, w/4, h/4,
      dtt_gen_color256, 
      flags,
      assert_fail));
  }
    
  if (have_wins)
    DT_I(dtwins_uninit,(ctl, &wins));	/* quit serving						*/

  if (have_scr)
    DT_I(dtscr_destroy,(ctl, &scr));	/* close window system				*/

  if (svc.priv != NULL)
    DT_I(dtsvc_unregister,(ctl, &svc));	/* remove name from system		    */

  return rc;
}
				  

/*..........................................................................
 *..........................................................................*/

static
dt_rc_e
dttwin_test(
  dt_ctl_t *ctl,
  const char *svcspec,
  const char *scrspec,
  char *name1,
  char *name2,
  char *name3,
  dtxy_t w,
  dtxy_t h,
  long flags,
  int *assert_fail)
{
  DT_F("dttwin_test");
  int isserver = 0;

  if (!strncmp(svcspec, "dsk,", 4))		/* dsk interaction requested? 		*/
  {
	const char *dirname = svcspec+4;

    flags &= ~FLAG_DO_BACKGROUND;		/* server runs after client 		*/

    if (flags & FLAG_DO_CLIENT)
      DT_Q(dttwin_client_test,(ctl,		/* do client things 				*/
        svcspec,
        name1, name2, name3,
        w, h, 
        flags, assert_fail));

    if (flags & FLAG_DO_SERVER)
	{
      DT_Q(dttwin_server_test,(ctl,		/* run up a server 					*/
        svcspec, scrspec,
        name1, name2, name3,
        w, h, 
    	flags, &isserver,
        assert_fail));
	  DT_Q(dtdsk_delete,(ctl,			/* delete all the temporary files 	*/
        dirname, NULL));
	}
  }
  else									/* not dsk? 						*/
  {
    if (flags & FLAG_DO_SERVER)
      DT_Q(dttwin_server_test,(ctl,		/* run up a server 					*/
	    svcspec, scrspec,
        name1, name2, name3,
        w, h, 
	    flags, &isserver,
        assert_fail));
    if (isserver)						/* forked server is returning? 		*/
	  return DT_RC_STOP;
    else								/* we are the client?				*/
    {
	  DT_Q(dttwin_client_test,(ctl,		/* do client things 				*/
        svcspec,
	    name1, name2, name3,
  	    w, h, flags,
	    assert_fail));
	  DT_Q(dtxchg_win_stop,(ctl,		/* tell server to stop 				*/
	    svcspec));
	  DT_Q(dtos_delay,(ctl, 500));		/* wait til server is good and gone	*/
	}
  }

  return DT_RC_GOOD;
}

/*..........................................................................*/

dt_rc_e
DTCONFIG_API1
dttwin_arg_check(						/* check args and params            */
  dt_ctl_t *ctl,
  void *app,
  dtparam_t *param,
  dt_enter_arg_t *arg,
  char *msg,
  const char **start)
{
  DT_F("dttwin_arg_check");

  DT_Q(dtt_check_op,(ctl, param,        /* make sure op is understood       */
    ops, msg, start));

  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/


dt_rc_e 
dttwinx(
  const char *F,
  int argc,
  char *argv[],
  dtparam_t *param,
  int dummy)
{
  dt_ctl_t *ctl = &param->ctl;
  dt_enter_arg_t arg[1];
  dtxy_t w, h;
  int flags;
  char *dsk_ipc = NULL;
  int assert_fail = 0;
  dt_rc_e rc;

  DTCSTACK_HIGHWATER;

  DT_C(dt_heapmon_init,(ctl));			/* init memory checking			    */

  DT_C(dtparam_init,(param));			/* init param structure             */

  DT_C(dtparam_set,(param, 0,          /* set default for params we want   */
    &param->op, NULL, NULL, "both",
    &param->screen, NULL, NULL, NULL,
    &param->output, NULL, NULL, NULL,
    &param->reps, NULL, NULL, "1",
    &param->w, NULL, NULL, "192",
    &param->h, NULL, NULL, "192",
    NULL));

  arg[0].desc = NULL;                   /* no arguments                     */
  DT_C(dt_enter_parse,(					/* go parse params and args			*/
    argc, argv, F, usage, param,
    arg, dttwin_arg_check, NULL));

  ctl->dbg = dttwin_dbg;
  ctl->err = dttwin_err;
  ctl->say = dttwin_say;

  w = (dtxy_t)param->w;					/* for short					    */
  h = (dtxy_t)param->h;

  if (DT_ABBREV("server", param->op))
	flags = FLAG_DO_SERVER;
  else
  if (DT_ABBREV("client", param->op))
	flags = FLAG_DO_CLIENT;
  else
    flags = FLAG_DO_SERVER |
            FLAG_DO_BACKGROUND |
            FLAG_DO_CLIENT;

  DT_C(dtos_strdup3,(ctl,				/* make dsk name 					*/
    DSK_IPC, ",", param->output,
    &dsk_ipc));

  if (strcmp(param->op, "both") &&		/* special op?   					*/
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
	  if (param->win_flag &				/* -win has a run-time value? 		*/
		  DTPARAM_FLAG_VALUE)
	  {
          DT_C(dttwin_test,(ctl,	
            param->win,
            param->screen,
            NAME1, NAME2, NAME3,
            w, h, 
            flags |
            FLAG_CAN_WRITE |
            FLAG_CAN_READ,
            &assert_fail))
	  }
	  else								/* -win has no run-time value? 		*/
      {
    	if (dtprj_qnx4_ipc)				/* we can do QNX4-style ipc?		*/
          DT_C(dttwin_test,(ctl,	
            QNX4_IPC ","
            DTPRJ_QNX4_IPC_NAME2,
            param->screen,
            NAME1, NAME2, NAME3,
            w, h, 
            flags |
            FLAG_CAN_WRITE |
            FLAG_CAN_READ,
            &assert_fail))

    	if (dtprj_socket)				/* we can do sockets? 				*/
          DT_C(dttwin_test,(ctl,
            SOCKET_IPC ","
            DTPRJ_SOCKET_HOST2 ":"
            DTPRJ_SOCKET_PORT2,
            param->screen,
            NAME1, NAME2, NAME3,
            w, h,
            flags |
            FLAG_CAN_WRITE |
            FLAG_CAN_READ,
            &assert_fail))

        DT_C(dttwin_test,(ctl,			/* everyone can do dsk 				*/
          dsk_ipc,
          param->screen,
          NAME1, NAME2, NAME3,
          w, h,
          flags |
          FLAG_CAN_WRITE |
          FLAG_CAN_READ,
          &assert_fail))
      }
	}									/* end reps loop					*/
  }

                                        /* ................................ */
cleanup:
  if (dsk_ipc)
    DT_I(dtos_free2,(ctl, dsk_ipc,
      F, "dsk ipc name"));

  DT_I(dt_leave,(param));				/* clean up before leaving          */

  ctl->dbg = NULL;
  ctl->err = NULL;
  ctl->say = NULL;

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
