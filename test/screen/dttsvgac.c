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
| All functions Return DTOS_RANWELL on success.  
| Any other return value indicates failure.
| 
| ERRORS
| As yet undocumented.
| 
| END
 *..........................................................................*/

#include <dtack/t.h>
DT_RCSID("test $RCSfile: dttsvgac.c,v $ $Revision: 1.7 $");
#include <dtack/keybd.h>

static char *USAGE = "\
usage: dttsvgac [params] chipset mode\n\
chipsets are:\n\
  0   auto detect\n\
  2   ATI\n\
 16   VESA\n\
modes are:\n\
  0   320x200                     3   800x600\n\
  1   640x400 (default)           4   1024x768\n\
  2   640x480                     5   1280x1024\n\
Ways to run this program:\n\
  dttsvgac                          run auto\n\
  dttsvgac -w 400 -h 400            run auto, bigger image\n\
  dttsvgac -D time                  run auto, output timing\n\
  dttsvgac -op wait                 pause during run\n\
  dttsvgac -op wait -n 1            pause and fewer timing loops";

#define DESC "Svgacc graphics"

/*..........................................................................*/

#if DTPRJ_SVGACC == DT_1

#  if DTPRJ_SVGACC_FAKE == DT_1
#    include <dtack/nofar.h>
#  endif

#  include <dtack/svgacc.h>

RasterBlock *rasterblock = NULL;
int bufsize;

/*..........................................................................
 *..........................................................................*/

static
void waitkey(
  dt_ctl_t *ctl,
  dtkeybd_t *keyboard)
{
  dt_key_t key;
  dtkeybd_read(ctl,				/* wait for key						*/
    keyboard, &key); 
}

/*..........................................................................*/
dt_rc_e
g_open(
  dt_ctl_t *ctl,
  int chipset,
  int mode,
  int *oldmode,
  dtkeybd_t *keyboard,
  int ifpause)
{
  DT_F("dttsvgacc_gopen");
  int cpu;
  int detect;
  int newmode;
  int mem;

  cpu = whichcpu();						/* check cpu					    */
  if (cpu < 386)
    return dt_err(ctl, F,
      "unsupported cpu %d", cpu);

  *oldmode = videomodeget();

  detect = whichvga();					/* autodetect svga type			    */
  mem = whichmem();

  if (detect == UNKNOWN)
    return dt_err(ctl, F,
      "chipset autodetect gave UNKNOWN");

  switch(mode)							/* switch into requested mode	    */
  {
	case 0: res320(); break;
	case 1: res640l(); break;
	case 2: res640(); break;
	case 3: res800(); break;
	case 4: res1024(); break;
	case 5: res1280(); break;
  }

  newmode = videomodeget();				/* see which mode we got		    */
  videomodeset(*oldmode);				/* return to text mode			    */
  if (chipset == 0)						/* caller wants autodetect?		    */
    chipset = detect;

  if (ifpause)
  {
    dt_say(ctl, F,
      "sizeof(RasterBlock) %d",
      sizeof(RasterBlock));

    dt_say(ctl, F,
      "old mode %d, detected chipset %d, memory %dk",
      *oldmode, detect, mem);

    dt_say(ctl, F,
      "will set chipset %d, new hw mode to be %d",
      chipset, newmode);

    dt_say(ctl, F,
      "...hit any key");

    waitkey(ctl, keyboard);
  }

  setcard((VGAChipset)chipset, mem);	/* put in the setup we want		    */

  switch(mode)
  {
	case 0: res320(); break;
	case 1: res640l(); break;
	case 2: res640(); break;
	case 3: res800(); break;
	case 4: res1024(); break;
	case 5: res1280(); break;
  }

  return DT_RC_GOOD;
}

/*..........................................................................*/
dt_rc_e
g_close(
  dt_ctl_t *ctl,
  int oldmode)
{
  videomodeset(oldmode);				/* put us in the text mode			*/
  return DT_RC_GOOD;
}

/*..........................................................................*/

static
void
putblock(								/* output using blkput			    */
  char *data,
  int x0,
  int y0,
  int w,
  int h)
{
  int nrows = bufsize / w;				/* rows that fit in our buffer		*/
  int y;
  unsigned long rb =
    (unsigned long)rasterblock;
  RasterBlock *blk = (RasterBlock *)
    (rb + 16L - (rb & 15L));

  blk->width = w - 1;
  blk->height = nrows - 1;
      
  for (y=0; y<h; y+=nrows)				/* for each buf full				*/
  {
	if (y+nrows > h)					/* partial buffer at end?			*/
	{
	  nrows = h - y;
      blk->height = nrows - 1;
	}
    DT_MEMCPY(blk->data,
      data, w * nrows);
    data = (char *)data + w * nrows;

	blkput(SET, x0, y0+y,				/* display buf full					*/
      blk);
  }
}

/*..........................................................................*/

static
void
getblock(								/* input using blkget			    */
  char *data,
  int x0,
  int y0,
  int w,
  int h)
{
  int nrows = bufsize / w;				/* rows that fit in our buffer		*/
  int y;
  int ret;
  unsigned long rb =
    (unsigned long)rasterblock;
  RasterBlock *blk = (RasterBlock *)
    (rb + 16L - (rb & 15L));

  blk->width = w - 1;
  blk->height = nrows - 1;
      
  for (y=0; y<h; y+=nrows)				/* for each buf full				*/
  {
	if (y+nrows > h)					/* partial buffer at end?			*/
	{
	  nrows = h - y;
      blk->height = nrows - 1;
	}
	ret = blkget(x0, y0+y,				/* get buf full						*/
      x0+w-1, y0+y+nrows-1,
      blk);
    if (ret == 0)
	{
      printf(
        "blkget(%d, %d, %d, %d, ...) failed\n",
        x0, y0+y, x0+w-1, y0+y+nrows-1);
      break;
	}

    DT_MEMCPY(data, blk->data, w * nrows);
	data = (char *)data + w * nrows;
  }
}

/*..........................................................................*/
#endif

/*..........................................................................*/

DTT_MAIN(dttsvgacc)
{
  DT_F("dttsvgacc");
  dt_ctl_t *ctl = &param->ctl;
  int assert_fail = 0;
  int n;
#if DTPRJ_SVGACC == DT_1
  dtkeybd_t keyboard;
  char *chipset_string;
  char *mode_string;
  long tmplong;
  int chipset;
  int mode;
  int oldmode;
  dtos_time_t mark1, mark2;
  dtos_time_t mark3, mark4;
  long cmprc;
  int have_keyboard = 0;
#endif
  dt_rc_e rc = DT_RC_GOOD;

  DT_GI(dt_heapmon_init,(ctl));			/* init memory checking			    */
										/* ................................ */
  DT_GI(dtparam_init,(param));			/* init param structure             */

  DTPARAM_DEFAULT(dbg, 0);
  DTPARAM_DEFAULT(mem, 0);
  DTPARAM_DEFAULT(op, "auto");
  DTPARAM_DEFAULT(w, 256);
  DTPARAM_DEFAULT(h, 256);
  DTPARAM_DEFAULT(n, 30);

  DT_GI(dt_enter_param,(				/* go parse args					*/
    argc, argv, F, USAGE, param, &n));

  argc -= n;
  argv += n;

  if (argc > 2)
    return dt_usage_say(
      param, USAGE);

#if DTPRJ_SVGACC == DT_1

  if (argc > 0)							/* chipset given?				    */
    chipset_string = argv[0];			/* use it						    */
  else									/* chipset not given?			    */
    chipset_string = "0";				/* default is autodetect		    */

  if (argc > 1)							/* mode given?					    */
    mode_string = argv[1];				/* use it						    */
  else									/* mode not given?				    */
    mode_string = "1";					/* default is 640x400			    */

  DT_GI(dtstr_to_long,(ctl,				/* this better be a number		    */
    chipset_string, &tmplong));
  chipset = (int)tmplong;

  DT_GI(dtstr_to_long,(ctl,				/* this better be a number too	    */
    mode_string, &tmplong));
  mode = (int)tmplong;

  DT_GI(dtkeybd_init,(ctl,				/* make us a keyboard				*/
    &keyboard, 
    "dos", 0));
  if (rc == DT_RC_GOOD)
    have_keyboard = 1;

  DT_GI(g_open,(ctl, chipset, mode,		/* open graphics				    */
    &oldmode, &keyboard, 
    strcmp(param->op, "auto")));
										/* ................................ */
  bufsize = 8192;
  DT_GI(dtos_malloc2,(ctl, 
     (void **)&rasterblock,
    bufsize+16+
    sizeof(rasterblock->width)+
    sizeof(rasterblock->height),
    F, "rasterblock"));  

  DT_GI(dtt_raw,(ctl, param,			/* do output, time it, and check it	*/
    (int)param->n,
    putblock, getblock,
    &mark1, &mark2, &mark3, &mark4,
    &cmprc));

  if (rasterblock)
    DT_G(dtos_free2,(ctl, 
      rasterblock, F, "rasterblock"));

  if (strcmp(param->op, "auto"))
    waitkey(ctl, &keyboard);

  DT_I(g_close,(ctl, oldmode));			/* close graphics				    */

  DT_GI(dtt_raw_report,(ctl,
    "svgacc block", 
    (int)param->n,
    &mark1, &mark2, &mark3, &mark4,
    &cmprc, &assert_fail));

  if (have_keyboard)
  DT_I(dtkeybd_uninit,(ctl,				/* get rid of our keyboard			*/
    &keyboard));
										/* ................................ */
#else
  if (strcmp(param->op, "auto"))		/* caller wants specific test?	    */
  {
    DT_GI(dt_assert_defined,(ctl, 0, F,
      "DTPRJ_SVGACC", 
      "SVGACC graphics"));
    assert_fail++;
  }
#endif
										/* ................................ */

  DT_I(dt_leave,(param));				/* clean up before leaving			*/

  DT_I(dt_heapmon_uninit,(ctl,			/* check all memory is freed	    */
    &assert_fail));

  if (assert_fail)						/* any failed assertions?		    */
    return DT_RC_BAD;
  else
    return DT_RC_GOOD;
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
