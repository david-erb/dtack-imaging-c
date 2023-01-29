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
DT_RCSID("test $RCSfile: dttfast.c,v $ $Revision: 1.7 $");
#include <dtack/keymap.h>
#include <dtack/keybd.h>

static char *USAGE = "usage: [fparams] chipset mode\n\
chipset is either -1 or a Fastgraph SVGA number, e.g.\n\
  -1 does not call fg_svgainit(), just uses VGA\n\
   0 is autodetect                  12 is Paradise PVGA1a\n\
   1 is autodetect VESA first       19 is Video7\n\
   2 is Ahead                    24-26 are Trident\n\
   6 is ATI 28800                37-38 are Cirrus\n\
mode is a Fastgraph 256-color video mode, one of:\n\
   0 search using fg_testmode()\n\
  19 320x200       25 640x480\n\
  22 320x240       26 800x600\n\
  24 640x400       27 1024x768\n\
Ways to run this program:\n\
  dttfastg                                    pause, default op\n\
  dttfastg -op [buffer|image|block|rowbyrow]  pause, different ops\n\
  dttsvgac -w 400 -h 400                      pause, bigger image\n\
  dttsvgac -D assert                          no pause\n\
  dttsvgac -D time -n 100                     no pause, more timing loops";

#define OP_DEFAULT "buffer"

/*..........................................................................*/

#if DTPRJ_FASTGRAPH == DT_1

#  if DTPRJ_FASTGRAPH_FAKE != DT_1
#    include <dtack/nofar.h>
#  endif

#  ifndef DTDEPEND
#    include <dtack/fastg.h>
#  endif

/*..........................................................................
 *..........................................................................*/

static unsigned int bufsize = 0;
static char *buf = NULL;

/*..........................................................................*/
dt_rc_e
g_open(
  dt_ctl_t *ctl,
  int chipset,
  int mode,
  int *oldmode,
  dtkeybd_t *keyboard,
  int nopause)
{
  DT_F("dttfastg_gopen");
  int detect = -1;

# ifdef FG32
    fg_initpm();						/* first Fastgraph call			    */
    fg_vb init();						/* init virtual buffers				*/
# endif

  if (chipset >= 0)						/* negative is for vga			    */
  {
    detect = fg_svgainit(chipset);		/* select svga type    				*/
    if ((chipset == 0 || 
         chipset == 1) &&
        detect == 0)
      return dt_err(ctl, F,
        "chipset/VESA autodetect failed");
  }

  if (mode == 0)						/* caller wants mode auto-select?	*/
  {
    static int modes[] = {27, 26, 25, 24, 22, 19, 0};
    int i;
    for (i=0; modes[i] != 0; i++)
    {
      if (fg_testmode(modes[i], 1))
        break;
      if (!nopause)
        dt_say(ctl, F,
          "chipset %d, detect %d, mode %d not available",
          chipset, detect, modes[i]);
    }
    if (modes[i] == 0)
      return dt_err(ctl, F,
        "no mode available");
    mode = modes[i];
  }

  *oldmode = fg_getmode();

  if (!nopause)
  {
    dt_say(ctl, F,
      "chipset %d, detect %d, new mode %d...hit any key",
      chipset, detect, mode);
    fg_waitkey();
  }

  fg_setmode(mode);						/* put us in the graphics mode		*/

  return DT_RC_GOOD;
}

/*..........................................................................*/
dt_rc_e
g_close(
  dt_ctl_t *ctl,
  int oldmode)
{
  fg_setmode(oldmode);
  return DT_RC_GOOD;
}

/*..........................................................................*/

static
void
putrowbyrow(							/* output one row at a time		    */
  char *data,
  int x0,
  int y0,
  int w,
  int h)
{
  int y;

  fg_move(x0, y0);
  fg_moverel(0, -h);					/* top of block					    */
  for (y=0; y<h; y++)					/* for each row in block		    */
  {
    fg_moverel(0, 1);					/* down one for next row		    */
    fg_putimage(data, w, 1);			/* display row						*/
    data += w;
  }
}

/*..........................................................................*/

static
void
getrowbyrow(							/* input one row at a time		    */
  char *data,
  int x0,
  int y0,
  int w,
  int h)
{
  int y;

  fg_move(x0, y0);
  fg_moverel(0, -h);					/* top of block					    */
  for (y=0; y<h; y++)					/* for each row in block		    */
  {
    fg_moverel(0, 1);					/* down one for next row		    */
    fg_getimage(data, w, 1);			/* input row						*/
    data += w;
  }
}

/*..........................................................................*/

static
void
putbuffer(								/* copy rows to buffer and output	*/
  char *data,
  int x0,
  int y0,
  int w,
  int h)
{
  int nrows = (int)(bufsize / w);		/* rows that fit in our buffer	    */
  int y;
  char *p, *pbot;

  pbot = buf + nrows * w;				/* point into bottom of buffer	    */

  fg_move(x0, y0);
  for (y=0; y<h; y+=nrows)				/* for each buf in block		    */
  {
    if (y+nrows > h)					/* partial buffer at end?		    */
	{
	  nrows = h - y;
      p = buf + nrows * w;				/* point into bottom of buffer	    */
	}
    else
      p = pbot;
    while (p > buf)						/* copy rows from top of data	    */
	{
      p -= w;
      DT_MEMCPY(p, data, w);
      data += w;
	}
    fg_moverel(0, nrows);				/* down next buf				    */
    fg_putimage(buf, w, nrows);			/* display row						*/
  }
}

/*..........................................................................*/

static
void
getbuffer(								/* copy rows via buffer				*/
  char *data,
  int x0,
  int y0,
  int w,
  int h)
{
  int nrows = (int)(bufsize / w);		/* rows that fit in our buffer	    */
  int y;
  char *p, *pbot;

  pbot = buf + nrows * w;				/* point into bottom of buffer	    */

  fg_move(x0, y0);
  for (y=0; y<h; y+=nrows)				/* for each buf in block		    */
  {
    if (y+nrows > h)					/* partial buffer at end?		    */
	{
	  nrows = h - y;
      p = buf + nrows * w;				/* point into bottom of buffer	    */
	}
    else
      p = pbot;
    fg_moverel(0, nrows);				/* down next buf				    */
    fg_getimage(buf, w, nrows);			/* input buf						*/
    while (p > buf)						/* copy rows to top of data	    */
	{
      p -= w;
      DT_MEMCPY(data, p, w);
      data += w;
	}
  }
}

/*..........................................................................*/

static
void
putblock(								/* output using fg_putblock		    */
  char *data,
  int x0,
  int y0,
  int w,
  int h)
{
  fg_putblock(data,
    x0-w+1, x0, y0-h+1, y0);
}

/*..........................................................................*/

static
void
getblock(								/* input using fg_getblock		    */
  char *data,
  int x0,
  int y0,
  int w,
  int h)
{
  fg_getblock(data,
    x0-w+1, x0, y0-h+1, y0);
}

/*..........................................................................*/

static
void
putimage(								/* output using fg_putimage		    */
  char *data,
  int x0,
  int y0,
  int w,
  int h)
{
  fg_move(x0, y0);
  fg_putimage(data,
    w, h);
}

/*..........................................................................*/

static
void
getimage(								/* input using fg_getimage		    */
  char *data,
  int x0,
  int y0,
  int w,
  int h)
{
  fg_move(x0, y0);
  fg_getimage(data,
    w, h);
}

/*..........................................................................*/

static
void
putvirt(								/* copy rows as virtual buffer		*/
  char *data,
  int x0,
  int y0,
  int w,
  int h)
{
  int handle = -1;
  int fgrc = -1;

  handle = vbdefine(data, w, h);		/* virtual buffer around our data 	*/
  if (handle < 0 || handle > 31)
  {
	rc = dt_err(ctl, F,
      "invalid handle %d returned"
      " by Fastgraph's vbdefine",
      handle);
	goto cleanup;
  }
  fgrc  = vbopen(handle);				/* make us the active virtual buffer */
  if (fgrc != 0)
  {
	rc = dt_err(ctl, F,
      "vbopen(%d) failed, rc %d",
      handle, fgrc);
	goto cleanup;
  }

  vbpaste(0, 0, w, h, x0, y0);			/* virtual buffer to active page */
										
cleanup:
  if (fgrc == 0)
    fg_vbclose();

  if (handle >= 0 && handle <= 31)
    fg_vbundef(handle);
}

/*..........................................................................*/

static
void
getvirt(								/* copy rows as virtual buffer		*/
  char *data,
  int x0,
  int y0,
  int w,
  int h)
{
  int handle = -1;
  int fgrc = -1;

  handle = vbdefine(data, w, h);		/* virtual buffer around our data 	*/
  if (handle < 0 || handle > 31)
  {
	rc = dt_err(ctl, F,
      "invalid handle %d returned"
      " by Fastgraph's vbdefine",
      handle);
	goto cleanup;
  }
  fgrc  = vbopen(handle);				/* make us the active virtual buffer */
  if (fgrc != 0)
  {
	rc = dt_err(ctl, F,
      "vbopen(%d) failed, rc %d",
      handle, fgrc);
	goto cleanup;
  }

  vbcut(0, 0, w, h, x0, y0);			/* active page to virtual buffer	*/
										
cleanup:
  if (fgrc == 0)
    fg_vbclose();

  if (handle >= 0 && handle <= 31)
    fg_vbundef(handle);
}

/*..........................................................................*/
#endif

#ifdef NONO
  for (i=0; i<256; i++)					/* make grayscale lut			    */
    lut[i].r = lut[i].g = lut[i].b =
    i >> 2;

  fg_setdacs(0, 256, (char *)lut);		/* set grayscale lut			    */
#endif


/*..........................................................................*/

DTT_MAIN(dttfastg)
{
  DT_F("dttfastg");
  dt_ctl_t *ctl = &param->ctl;
  int n;
  int assert_fail = 0;
#if DTPRJ_FASTGRAPH == DT_1
  char *chipset_string;
  char *mode_string;
  long tmplong;
  int chipset;
  int mode;
  int oldmode;
  int nopause;
  dtkeybd_t keyboard;
  void (*put)(char *, int, int, int, int);
  void (*get)(char *, int, int, int, int);
  dtos_time_t mark1, mark2;
  dtos_time_t mark3, mark4;
  long cmprc;
  char id[32];
  int have_keyboard = 0;
#endif
  dt_rc_e rc = DT_RC_GOOD;

  DT_GI(dt_heapmon_init,(ctl));			/* init memory checking			    */
										/* ................................ */
  DT_GI(dtparam_init,(param));			/* init param structure             */

  DTPARAM_DEFAULT(dbg, 0);				/* set default for params we want	*/
  DTPARAM_DEFAULT(mem, 0);
  DTPARAM_DEFAULT(op, OP_DEFAULT);
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

#if DTPRJ_FASTGRAPH == DT_1

  if (argc > 0)							/* chipset given?				    */
    chipset_string = argv[0];			/* use it						    */
  else									/* chipset not given?			    */
    chipset_string = "0";				/* default is autodetect		    */

  if (argc > 1)							/* mode given?					    */
    mode_string = argv[1];				/* use it						    */
  else									/* mode not given?				    */
    mode_string = "24";					/* default is 640x400			    */

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

  if (ctl->dbg_mask&DT_DBG_MASK_TIME ||	/* outputting timing?			    */
      ctl->dbg_mask&DT_DBG_MASK_ASSERT)	/* outputting assertions?		    */
    nopause = 1;						/* this means nopause	   		    */
  else
    nopause = 0;

  DT_GI(g_open,(ctl, chipset, mode,		/* open graphics				    */
    &oldmode, &keyboard, nopause));
										/* ................................ */
  if (rc == DT_RC_GOOD)
  {
    if (!strcmp(param->op, "image"))
    {
      put = putimage;
      get = getimage;
    }
    else
    if (!strcmp(param->op, "rowbyrow"))
    {
      put = putrowbyrow;
      get = getrowbyrow;
    }
    else
    if (!strcmp(param->op, "buffer"))
    {
      put = putbuffer;
      get = getbuffer;
    }
    else
    if (!strcmp(param->op, "block"))
    {
      put = putblock;
      get = getblock;
    }
    else
    if (!strcmp(param->op, "virtual"))
    {
      put = putvirtual;
      get = getvirtual;
    }
  
    bufsize = 8192;
    DT_GI(dtos_malloc2,(ctl,			/* space for buffer				    */
      (void **)&buf, bufsize,
      F, "buffer"));
  
    DT_GI(dtt_raw,(ctl, param,			/* do output, time it, and check it	*/
      (int)param->n,
      put, get,
      &mark1, &mark2, &mark3, &mark4,
      &cmprc));
  
    if (buf)
      DT_G(dtos_free2,(ctl, 
        buf, F, "buffer"));
  
    if (!nopause)
      fg_waitkey();
  
    DT_I(g_close,(ctl, oldmode));		/* close graphics				    */
  }

  sprintf(id, "fastgraph %s",
    param->op);
  DT_GI(dtt_raw_report,(ctl,
    id,
    (int)param->n,
    &mark1, &mark2, &mark3, &mark4,
    &cmprc, &assert_fail));

  if (have_keyboard)
  DT_I(dtkeybd_uninit,(ctl,				/* get rid of our keyboard			*/
    &keyboard));
										/* ................................ */
#else
  if (strcmp(param->op, OP_DEFAULT))	/* caller wants specific test?	    */
  {
    DT_GI(dt_assert_defined,(ctl, 0, F,
      "DTPRJ_FASTGRAPH", 
      "FASTGRAPH graphics"));
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
