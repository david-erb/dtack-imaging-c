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

#include <dtack/base.h>
DT_RCSID("source/fb $RCSfile: fbvbmci.c,v $ $Revision: 1.7 $");
#include <dtack/lut8.h>
#include <dtack/color.h>
#include <dtack/fb.h>
#include <dtack/os.h>
#include <dtack/str.h>
#include <dtack/prj.h>
#include <dtack/mem.h>

#include <dtack/msw.h>

#define DESC "vbmci capture card"

extern dtfb_init_f dtfb_vbmci_init;

#if DTPRJ_VBMCI == DT_1

typedef struct {
  dtimage_t image;
  HWND hMCIVideo;
  HGLOBAL handle;
  char *buffer;
  char *spec;
  char *board_type;
  char *tmpname;
} dtfb_vbmci_priv_t;

#ifndef DTDEPEND
#  include <mmsystem.h>
#endif

/*..........................................................................
 * send query and get reply, if any
 *..........................................................................*/

static
dt_rc_e
dtfb_vbmci_sendget(
  dt_ctl_t *ctl,
  dtfb_t *fb,
  const char *F,
  const char *string,
  char *answer,
  int max)
{
  int mcirc;

  DTCSTACK_HIGHWATER;

  mcirc = mciSendString(string,			/* send string to driver 			*/
    answer, max, NULL);
  
  if (mcirc != 0)						/* send failed? 					*/
  {
	char errstring[128];
	if (!mciGetErrorString(mcirc,		/* get error string 				*/
          errstring, 
          sizeof(errstring)))
	  strcpy(errstring,					/* error unknown? 					*/
        "unknown error");
    dt_err(ctl, F,						
      "mciSendString rc %d (%s)",
      mcirc, errstring);
    return dt_err(ctl, F,				/* return dtack error 				*/
      "doing \"%s\"",
      string);
  }

  return DT_RC_GOOD;
}

/*..........................................................................
 * send command, don't expect reply
 *..........................................................................*/

static
dt_rc_e
dtfb_vbmci_send(
  dt_ctl_t *ctl,
  dtfb_t *fb,
  const char *F,
  const char *format,
  ...)				
{
  char string[256];
  va_list ap;

  DTCSTACK_HIGHWATER;

  va_start(ap, format);
  DT_Q(dtstr_printf_va,(ctl,			/* make complete string 			*/
    string, sizeof(string), 
    format, ap));
  va_end(ap);

  DT_Q(dtfb_vbmci_sendget,(ctl, fb, F,	/* send string to driver 			*/
    string, NULL, 0));

  return DT_RC_GOOD;
}

/*..........................................................................
 * query driver and print interesting things
 *..........................................................................*/

static
dt_rc_e
dtfb_vbmci_report(
  dt_ctl_t *ctl,
  dtfb_t *fb)
{
  DT_F("dtfb_vbmci_report");
  char string[256];
  int l = sizeof(string);

#define REPORT(S) \
  DT_Q(dtfb_vbmci_sendget,(ctl, fb, F, \
    S, string, l)); \
  dt_dbg(ctl, F, DT_DBG_MASK_ALIMITS, \
    "%26s: %s", S, string);

  REPORT("info vb product");
  REPORT("capability vb card type");
  REPORT("get vb card type");
  REPORT("get vb video");
  REPORT("get vb window");
  REPORT("get vb limit video");
  REPORT("get vb limit display");
  REPORT("where vb video");
  REPORT("where vb frame");
  REPORT("where vb source");
  REPORT("where vb destination");

  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/

static
dt_rc_e
dtfb_vbmci_control(
  dt_ctl_t *ctl,
  dtfb_t *fb,
  dtfb_control_e control)
{
  DT_F("dtfb_vbmci_control");

  DTCSTACK_HIGHWATER;

  switch (control)
  {
	case DTFB_CONTROL_RESET:
	break;
	case DTFB_CONTROL_SNAPSHOT:
      DT_Q(dtfb_vbmci_send,(ctl, fb, F,
        "unfreeze vb"));
      DT_Q(dtfb_vbmci_send,(ctl, fb, F,
        "freeze vb"));
	break;
	case DTFB_CONTROL_FREEZE:
      DT_Q(dtfb_vbmci_send,(ctl, fb, F,
        "freeze vb"));
	break;
	case DTFB_CONTROL_ACQUIRE:
      DT_Q(dtfb_vbmci_send,(ctl, fb, F,
        "unfreeze vb"));
	break;
	default:
      return dt_err(ctl, F,
        "invalid control code 0x%04x",
        control);
  }
  return DT_RC_GOOD;
}

/*..........................................................................
 * use the mode config string to change config file which has window size
 *..........................................................................*/

static
dt_rc_e
dtfb_vbmci_getblock_handle(
  dt_ctl_t *ctl,
  dtfb_t *fb,
  void *data,
  int x0,
  int y0,
  int xe,
  int ye)
{
  DT_F("dtfb_vbmci_getblock_handle");
  dtfb_vbmci_priv_t *priv =
    (dtfb_vbmci_priv_t *)fb->priv;
  LPVOID lphandle = NULL;
  dt_rc_e rc;

  DT_C(dtfb_vbmci_control,(ctl,			/* freeze 							*/
    fb, DTFB_CONTROL_FREEZE));

  lphandle = (LPVOID)GlobalLock(
    priv->handle);

  dt_dbg(ctl, F, DT_DBG_MASK_COMPLEX,
    "save vb memory buffer"
    " type dib 24-bit"
    " address %lu wait", 
    (unsigned long)(LPVOID)lphandle);

  DT_C(dtfb_vbmci_send,(ctl, fb, F,		/* extract memory 					*/
    "save vb memory buffer"
    " type dib 24-bit"
    " address %lu", 
    (unsigned long)(LPVOID)lphandle));

cleanup:
  if (lphandle != NULL)
    GlobalUnlock(priv->handle);

  return rc;
}

/*..........................................................................
 * use the mode config string to change config file which has window size
 *..........................................................................*/

static
dt_rc_e
dtfb_vbmci_getblock_dib(
  dt_ctl_t *ctl,
  dtfb_t *fb,
  void *data,
  int x0,
  int y0,
  int xe,
  int ye)
{
  DT_F("dtfb_vbmci_getblock");
  dtfb_vbmci_priv_t *priv =
    (dtfb_vbmci_priv_t *)fb->priv;
  
  dt_rc_e rc;

  DT_C(dtfb_vbmci_control,(ctl,			/* freeze 							*/
    fb, DTFB_CONTROL_FREEZE));

  dt_dbg(ctl, F, DT_DBG_MASK_COMPLEX,
    "save vb %s"
    " at 0 0 %u %u type dib 24-bit wait",
    priv->tmpname, fb->xe, fb->ye);

  DT_C(dtfb_vbmci_send,(ctl, fb, F,		/* write a bmp file					*/
    "save vb %s"
    " at 0 0 %u %u type dib 24-bit wait",
    priv->tmpname, fb->xe, fb->ye));

  DT_C(dtimage_read_bmp_file,(ctl,		/* read in the bmp file			    */
    &priv->image, priv->tmpname));

  DT_C(dtmem_getpatch,(ctl,				/* suck out the data			    */
    priv->image.memalloc,
    data, x0*3, y0, xe*3, ye));

#ifdef NONO
  dt_ntype_b08_t *d;
  dtxy_t x, y;
  x0 *= 3;
  xe *= 3;
  d = (dt_ntype_b08_t *)data;
  for (y=y0; y<y0+ye; y++)
  {
	const dt_ntype_b08_t * const p =
      &priv->image.row[y].b08[x0];
	for (x=0; x<xe; x+=3)
	{
	  d[x  ] = p[x+2];
	  d[x+1] = p[x+1];
	  d[x+2] = p[x  ];
	}
	d += x;
  }
#endif

cleanup:
  return rc;
}

/*..........................................................................
 *..........................................................................*/

static
dt_rc_e
dtfb_vbmci_uninit(
  dt_ctl_t *ctl,
  dtfb_t *fb)
{
  DT_F("dtfb_vbmci_uninit");
  dtfb_vbmci_priv_t *priv =
    (dtfb_vbmci_priv_t *)fb->priv;
  dt_rc_e rc = DT_RC_GOOD;

  DTCSTACK_HIGHWATER;

  DT_Q(dtfb_vbmci_send,(ctl, fb, F,
    "close vb"));
  
  if (priv != NULL &&
	  priv->image.xe != 0)
	DT_I(dtimage_free,(ctl, &priv->image));

  if (priv != NULL &&
	  priv->handle)
	GlobalFree(priv->handle);

  if (priv != NULL &&
	  priv->buffer != NULL)
    DT_I(dtos_free2,(ctl, priv->buffer,
      F, "buffer"));

  if (priv != NULL &&
	  priv->spec != NULL)
    DT_I(dtos_free2,(ctl, priv->spec,
      F, "spec"));

  if (priv != NULL)
    DT_I(dtos_free2,(ctl, priv,
      F, "priv"));

  return rc;
}

#endif

/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtfb_vbmci_init(
  dt_ctl_t *ctl,
  dtfb_t *fb,
  const char *spec)
{
  DT_F("dtfb_vbmci_init");
# if DTPRJ_VBMCI == DT_1
  dtfb_vbmci_priv_t *priv = NULL;
  int ntokens;
  char *p;
  long longval;
  dt_rc_e rc;

  DTCSTACK_HIGHWATER;

  if (spec == NULL)
  {
    rc = dt_err(ctl, F,
      "NULL vbmci spec");
    goto cleanup;
  }
  else
  if (strlen(spec) == 0)
  {
    rc = dt_err(ctl, F,
      "zero-length vbmci spec");
    goto cleanup;
  }

  DT_C(dtos_malloc2,(ctl,				/* space for private structure	    */
    (void **)&priv,
    sizeof(dtfb_vbmci_priv_t),
    F, "priv"));
  DT_MEMSET(priv, 0,						/* clear private structure 			*/
    sizeof(dtfb_vbmci_priv_t));

  DT_C(dtos_strdup,(ctl, spec,			/* get space for spec edit 			*/
    &priv->spec));

  DT_C(dtstr_tokenize,(ctl,				/* separate spec into tokens 		*/
    priv->spec,
    ',', &ntokens));
										/* ................................ */
  p = priv->spec;

  if (ntokens >= 1)
  {
    priv->board_type = p;
	p += strlen(p) + 1;
  }

  if (ntokens >= 2)
  {
    DT_C(dtstr_to_long,(ctl,
      p, &longval));
	fb->xe = longval;
	p += strlen(p) + 1;
  }
  else
    fb->xe = 352;

  if (ntokens >= 3)
  {
    DT_C(dtstr_to_long,(ctl,
      p, &longval));
	fb->ye = longval;
	p += strlen(p) + 1;
  }
  else
    fb->ye = 288;

  if (ntokens >= 4)
  {
    priv->tmpname = p;
  }
  else
    priv->tmpname = "d:\\dtvbmci.bmp";

										/* ................................ */
  DT_C(dtfb_vbmci_send,(ctl, fb, F,
    "open digitalvideo alias vb"
    " style overlapped wait"));

  DT_C(dtfb_vbmci_send,(ctl, fb, F,
    "window vb state show wait"));

  DT_C(dtfb_vbmci_send,(ctl, fb, F,
    "set vb card type to 3"));

  DT_C(dtfb_vbmci_send,(ctl, fb, F,
    "set vb memory buffer"
    " format to yuv wait"));

										/* ................................ */
  {
	char s[64], t[64];
	dtstr_printf(ctl, s, sizeof(s),
      "get vb alloc at 0 0 %u %u",
      fb->xe, fb->ye);
	dt_dbg(ctl, F, DT_DBG_MASK_ALIMITS, 
      "alloc request: <%s>", s);
	DT_C(dtfb_vbmci_sendget,(ctl, fb,	/* get optimum buffer allocation 	*/
      F, s, t, sizeof(t)));
	strcpy(s, t);
	dt_dbg(ctl, F, DT_DBG_MASK_ALIMITS,
      "alloc answer: <%s>", s);
	DT_C(dtstr_tokenize,(ctl,			/* divide string into tokens		*/
      t, ' ', &ntokens));
	if (ntokens != 4)					/* not a parsable return string? 	*/
	{
	  rc = dt_err(ctl, F,
        "get vb alloc returned"
        " invalid \"%s\" (ntokens=%d)", 
        s, ntokens);
	  goto cleanup;
	}
	p = t;
	p += strlen(p) + 1;
	p += strlen(p) + 1;
	DT_C(dtstr_to_long,(ctl,
      p, &longval));
	fb->xe = longval;
	p += strlen(p) + 1;
	DT_C(dtstr_to_long,(ctl,
      p, &longval));
	fb->ye = longval;
	dt_dbg(ctl, F, DT_DBG_MASK_ALIMITS,
      "acquire size changed to %ux%u",
      fb->xe, fb->ye);
  }

										/* ................................ */
  DT_C(dtfb_vbmci_send,(ctl, fb, F,		/* set acquire and capture regions 	*/
    "put vb source at 0 0 %u %u wait",
    fb->xe, fb->ye));

  DT_C(dtfb_vbmci_send,(ctl, fb, F,
    "put vb frame at 0 0 %u %u wait",
    fb->xe, fb->ye));

  DT_C(dtfb_vbmci_send,(ctl, fb, F,
    "put vb destination at 0 0 %u %u wait",
    fb->xe, fb->ye));
  
										/* ................................ */
  {
	char t[64];
	BOOLEAN winrc;
	DT_C(dtfb_vbmci_sendget,(ctl, fb,	/* get video window's handle		*/
      F, "status vb window handle",
      t, sizeof(t)));
	priv->hMCIVideo = (HWND)atol(t);
  
	winrc = MoveWindow(priv->hMCIVideo,	/* move video frame to corner 		*/
      0, 0, fb->xe, fb->ye, TRUE);
	if (winrc == FALSE)					/* move window failed? 				*/
	{
	  rc = dt_err(ctl, F,
	    "MoveWindow(0x%08lx, 0, 0"
        " %u, %u, TRUE) failed",
        priv->hMCIVideo,
        fb->xe, fb->ye);
	  goto cleanup;
	}
  }

  DT_C(dtfb_vbmci_report,(ctl, fb));	/* report what we have so far 		*/

  fb->org.org  = DT_COLOR_ORG_YCBCR;
  fb->org.org  = DT_COLOR_ORG_RGB888;
  fb->rowsize  = 3 * fb->xe;			/* three bytes per pixel 			*/
  
  fb->priv     = priv;
  fb->uninit   = dtfb_vbmci_uninit;
  fb->control  = dtfb_vbmci_control;
  fb->getblock = dtfb_vbmci_getblock_handle;
  fb->getblock = dtfb_vbmci_getblock_dib;
                
  DT_C(dtimage_create,(ctl, 
    &priv->image, fb->rowsize, fb->ye,
    DT_NTYPE_B08));

#ifdef NONO
  dt_dbg(ctl, F, DT_DBG_MASK_ALIMITS,
    "allocating global %ld",
	(long)fb->xe*(long)fb->ye * 6L);
  priv->handle = GlobalAlloc(
    GMEM_MOVEABLE | GMEM_SHARE, 
	(long)fb->xe*(long)fb->ye * 6L);
  if (!priv->handle) 
  {
	rc = dt_err(ctl, F,
      "GlobalAlloc failed");
	goto cleanup;
  }
#endif

#ifdef NONO
  DT_C(dtos_malloc2,(ctl, 
    &priv->buffer, fb->rowsize*fb->ye,
    F, "buffer"));
#endif

cleanup:
  if (rc != DT_RC_GOOD)
  {
	if (priv != NULL &&
		priv->image.xe != 0)
	  DT_I(dtimage_free,(ctl, &priv->image));

	if (priv != NULL &&
		priv->handle)
	  GlobalFree(priv->handle);
 
    if (priv != NULL &&
		priv->buffer != NULL)
      DT_I(dtos_free2,(ctl, priv->buffer,
        F, "buffer"));

    if (priv != NULL &&
		priv->spec != NULL)
      DT_I(dtos_free2,(ctl, priv->spec,
        F, "spec"));

	if (priv != NULL)
	  DT_I(dtos_free2,(ctl, priv,
        F, "priv"));
  }

  return rc;
# else
  return dt_err_notlinked(ctl, F, DESC);  
# endif
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
