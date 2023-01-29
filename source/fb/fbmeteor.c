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
| REV
| 1.5 19970525 fixed bug which turned 24-bit into 8 bit
| 1.6 19970902 switch video source
| 
| END
 *..........................................................................*/

#include <dtack/base.h>
DT_RCSID("source/fb $RCSfile: fbmeteor.c,v $ $Revision: 1.7 $");
#include <dtack/lut8.h>
#include <dtack/color.h>
#include <dtack/fb.h>
#include <dtack/os.h>
#include <dtack/str.h>
#include <dtack/prj.h>
#include <dtack/yield.h>
#include <dtack/errno.h>

#define DESC "meteor capture card"

extern dtfb_init_f dtfb_meteor_init;

#if DTPRJ_METEOR == DT_1

typedef struct {
  dtos_time_t switch_time;				/* time at last source switch 		*/
  double rgain, ggain, bgain;
  char *spec;
  char *mem;
  long switch_delay;					/* delay before grabbing video 		*/
  int mem_size;
  int fd;
  int acquiring;
  int flag_yuv411;
  int flag_bi24;
  int skip;
  int dup;
  unsigned char rlookup[256];
  unsigned char glookup[256];
  unsigned char blookup[256];
} priv_t;

static priv_t global_priv;

#define SVALUE(S) \
  value = (S); svalue = #S

#ifndef DTDEPEND
#include <sys/types.h>
#include <sys/mman.h>
#include <sys/fcntl.h>
#include <meteor_ioctl.h>
#endif

/*..........................................................................
 * query driver and print interesting things
 *..........................................................................*/

static
dt_rc_e
dtfb_meteor_report(
  dt_ctl_t *ctl,
  dtfb_t *fb,
  const char *prefix)
{
  DT_F("dtfb_meteor_report");
  priv_t *priv = (priv_t *)fb->priv;
  dt_rc_e rc = DT_RC_GOOD;
cleanup:
  return rc;
}

/*..........................................................................
 *..........................................................................*/

static
dt_rc_e
dtfb_meteor_control(
  dt_ctl_t *ctl,
  dtfb_t *fb,
  dtfb_control_e control)
{
  DT_F("dtfb_meteor_control");
  priv_t *priv = (priv_t *)fb->priv;
  int value;
  char *svalue;
  int sysrc;
  dt_rc_e rc = DT_RC_GOOD;

  DTCSTACK_HIGHWATER;

  switch (control)
  {
	case DTFB_CONTROL_RESET:
	break;
	case DTFB_CONTROL_SNAPSHOT:
	{
	  dt_dbg(ctl, F, DT_DBG_MASK_COMMAND,
        "SNAPSHOT");
	  SVALUE(METEOR_CAP_SINGLE);
      sysrc = ioctl(priv->fd,				/* capture single meteor frame 	*/
        METEORCAPTUR, &value);
	  if (sysrc < 0)
	  {
	    rc = dt_err(ctl, F,
          "ioctl(%d, METEORCAPTUR, %s (%d))"
          DTERRNO_FMT,
          priv->fd,
	      svalue, value,
          DTERRNO_ARG(errno));
	    goto cleanup;
	  }
	}
	break;
	case DTFB_CONTROL_FREEZE:
    if (priv->acquiring)
	{
	  dt_dbg(ctl, F, DT_DBG_MASK_COMMAND,
        "FREEZE");
	  SVALUE(METEOR_CAP_STOP_CONT);
      sysrc = ioctl(priv->fd,				/* stop continuous capture		*/
        METEORCAPTUR, &value);
	  if (sysrc < 0)
	  {
	    rc = dt_err(ctl, F,
          "ioctl(%d, METEORCAPTUR, %s (%d))"
          DTERRNO_FMT,
          priv->fd,
	      svalue, value,
          DTERRNO_ARG(errno));
	    goto cleanup;
	  }
	  priv->acquiring = 0;
	}
	break;
	case DTFB_CONTROL_ACQUIRE:
    if (!priv->acquiring)
	{
	  dt_dbg(ctl, F, DT_DBG_MASK_COMMAND,
        "ACQUIRE");
	  SVALUE(METEOR_CAP_CONTINOUS);
      sysrc = ioctl(priv->fd,				/* start continuous capture		*/
        METEORCAPTUR, &value);
	  if (sysrc < 0)
	  {
	    rc = dt_err(ctl, F,
          "ioctl(%d, METEORCAPTUR, %s (%d))"
          DTERRNO_FMT,
          priv->fd,
	      svalue, value,
          DTERRNO_ARG(errno));
	    goto cleanup;
	  }
	  priv->acquiring = 1;
	}
	break;
	default:
      rc = dt_err(ctl, F,
        "invalid control code 0x%04x",
        control);
  }
  
cleanup:
  return rc;
}

/*..........................................................................
 *..........................................................................*/

static
dt_rc_e
dtfb_meteor_getblock(
  dt_ctl_t *ctl,
  dtfb_t *fb,
  void *data,
  int x0,
  int y0,
  int xe,
  int ye)
{
  DT_F("dtfb_meteor_getblock");
  priv_t *priv = (priv_t *)fb->priv;
  dt_rc_e rc = DT_RC_GOOD;
  
  if (x0 != 0 || y0 != 0)				/* the whole pie only			    */
    return dt_err(ctl, F,
      "x0=%d or y0=%d is not 0",
      x0, y0);

  DT_Q(dtos_delay,(ctl, 
    priv->switch_delay));

  if (priv->flag_yuv411)
  {
    DT_MEMCPY(data, priv->mem,				/* copy bytes from meteor's buffer 	*/
      priv->mem_size);
  }
  else
  if (priv->flag_bi24)
  {
    DT_C(dtfb_util_grb0888_bi24,(ctl,
      (const unsigned char *)priv->mem, 
      (unsigned char *)data, xe, ye));
  }
  else
  {
    DT_C(dtfb_util_grb0888_rgb888,(ctl,
      (const unsigned char *)priv->mem, 
      (unsigned char *)data, xe, ye, 
      priv->rlookup,
	  priv->glookup, 
      priv->blookup,
      priv->skip,
      priv->dup));
  }

cleanup:
  return rc;
}

/*..........................................................................
 *..........................................................................*/

static
dt_rc_e
dtfb_meteor_release(						/* release resources 				*/
  dt_ctl_t *ctl,
  priv_t *priv)
{
  DT_F("dtfb_meteor_release");
  long result;
  dt_rc_e rc = DT_RC_GOOD;

  if (priv == NULL)
    return rc;

  if (priv->mem != NULL)
    munmap(priv->mem, priv->mem_size);
  
  if (priv->fd > 0)
	close(priv->fd);

  if (priv->spec != NULL)
    DT_I(dtos_free2,(ctl, priv->spec,
      F, "spec"));

  DT_I(dtos_free2,(ctl, priv,
    F, "priv"));

  return rc;
}

/*..........................................................................
 *..........................................................................*/

static
dt_rc_e
dtfb_meteor_source(						/* change signal source				*/
  dt_ctl_t *ctl,
  dtfb_t *fb,
  const char *source)
{
  DT_F("dtfb_meteor_source");
  priv_t *priv = (priv_t *)fb->priv;
  char *s = NULL;
  int i, n;
  char *p;
  long v;
  char c;
  int sysrc;
  dt_rc_e rc = DT_RC_GOOD;

  if (source == NULL)
    goto cleanup;

  DT_C(dtos_strdup,(ctl, source, &s));	/* a place to edit 					*/
  DT_C(dtstr_tokenize,(ctl, s, '+',		/* multiple commands together 		*/
    &n));

  p = s;
  for (i=0; i<n; i++)
  {
										/* ................................ */
	if (DT_ABBREV(p, "source,"))
	{
	  p = strchr(p, ',') + 1;
	  DT_C(dtstr_to_long,(ctl, p, &v));
	  switch(v)
	  {
	    case 0: v = METEOR_INPUT_DEV0; break;
	    case 1: v = METEOR_INPUT_DEV1; break;
	    case 2: v = METEOR_INPUT_DEV2; break;
	    case 3: v = METEOR_INPUT_DEV3; break;
	    case 4: v = METEOR_INPUT_DEV_SVIDEO; break;
	    default:
		{
		  rc = dt_err(ctl, F,
            "invalid source %ld", v);
		  goto cleanup;
	    }
	  }
	  DT_Q(dtos_time,(ctl,				/* current time at switch			*/
        &priv->switch_time));
	  priv->switch_delay = 0;			/* delay just before getblock		*/
	  dt_dbg(ctl, F, DT_DBG_MASK_COMMAND,
        "switch to video source 0x%04x,"
        " delay will be %d ms",
        v, priv->switch_delay);
	  sysrc = ioctl(priv->fd, 
        METEORSINPUT, &v);
	  if (sysrc != 0)
	  {
		rc = dt_err(ctl, F,
          "ioctl(%d, METEORSINPUT, &(%ld))"
          DTERRNO_FMT,
          priv->fd,  v,
          DTERRNO_ARG(errno));
		goto cleanup;
	  }
	}
										/* ................................ */
	else
	if (DT_ABBREV(p, "chcv,"))
	{
	  p = strchr(p, ',') + 1;
	  DT_C(dtstr_to_long,(ctl, p, &v));
	  c = (signed char)v;
	  sysrc = ioctl(priv->fd, 
        METEORSCHCV, &c);
	  if (sysrc != 0)
	  {
		rc = dt_err(ctl, F,
          "ioctl(%d, METEORSCHCV, &(%d))"
          DTERRNO_FMT,
          priv->fd, c,
          DTERRNO_ARG(errno));
		goto cleanup;
	  }
										/* ................................ */
	  c = 0;
	  sysrc = ioctl(priv->fd,			/* confirm value					*/
        METEORGCHCV, &c);
	  if (sysrc < 0)
	  {
		rc = dt_err(ctl, F,
          "ioctl(%d, METEORGCHCV, &c)"
          DTERRNO_FMT,
          priv->fd,
          DTERRNO_ARG(errno));
    	goto cleanup;
	  }
	  dt_dbg(ctl, F, DT_DBG_MASK_COMMAND,
        "METEORGCHCV=%d", c);
	}
										/* ................................ */
	else
	if (DT_ABBREV(p, "hue,"))
	{
	  p = strchr(p, ',') + 1;
	  DT_C(dtstr_to_long,(ctl, p, &v));
	  c = (signed char)v;
	  sysrc = ioctl(priv->fd, 
        METEORSHUE, &c);
	  if (sysrc != 0)
	  {
		rc = dt_err(ctl, F,
          "ioctl(%d, METEORSHUE, &(%d))"
          DTERRNO_FMT,
          priv->fd, c,
          DTERRNO_ARG(errno));
		goto cleanup;
	  }
	}
										/* ................................ */
	else
	if (DT_ABBREV(p, "rgain,"))
	{
	  p = strchr(p, ',') + 1;
	  DT_C(dtstr_to_double,(ctl, p, 
        &priv->rgain));
	}
										/* ................................ */
	else
	if (DT_ABBREV(p, "ggain,"))
	{
	  p = strchr(p, ',') + 1;
	  DT_C(dtstr_to_double,(ctl, p, 
        &priv->ggain));
	}
										/* ................................ */
	else
	if (DT_ABBREV(p, "bgain,"))
	{
	  p = strchr(p, ',') + 1;
	  DT_C(dtstr_to_double,(ctl, p, 
        &priv->bgain));
	}
	else
	{
	  rc = dt_err(ctl, F,
        "invalid meteor command \"%s\"",
        p);
	  goto cleanup;
	}
	p = p + strlen(p) + 1;
  }
  
  for (i=0; i<256; i++)
  {
	priv->rlookup[i] = (unsigned char)DT_MAX(0.0, DT_MIN(255.0, (priv->rgain * i)));
	priv->glookup[i] = (unsigned char)DT_MAX(0.0, DT_MIN(255.0, (priv->ggain * i)));
	priv->blookup[i] = (unsigned char)DT_MAX(0.0, DT_MIN(255.0, (priv->bgain * i)));
  }
  
cleanup:
  if (s != NULL)
    DT_I(dtos_free2,(ctl, s,
      F, "source string"));
  return rc;
}

/*..........................................................................
 *..........................................................................*/

static
dt_rc_e
dtfb_meteor_uninit(						/* release resources 				*/
  dt_ctl_t *ctl,
  dtfb_t *fb)
{
  DT_F("dtfb_meteor_uninit");
  priv_t *priv = (priv_t *)fb->priv;
  DT_Q(dtfb_meteor_release,(ctl, priv));
  return DT_RC_GOOD;
}

#endif

/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtfb_meteor_init(
  dt_ctl_t *ctl,
  dtfb_t *fb,
  const char *spec)
{
  DT_F("dtfb_meteor_init");
# if DTPRJ_METEOR == DT_1
  priv_t *priv = NULL;
  int ntokens;
  char *p;
  char *dev = "";
  long longval;
  int value;
  char *svalue;
  signed char c;
  struct meteor_geomet geo;
  int sysrc;
  dt_rc_e rc;

  DTCSTACK_HIGHWATER;

  if (spec == NULL)
  {
    rc = dt_err(ctl, F,
      "NULL meteor spec");
    goto cleanup;
  }
  else
  if (strlen(spec) == 0)
  {
    rc = dt_err(ctl, F,
      "zero-length meteor spec");
    goto cleanup;
  }

  DT_C(dtos_malloc2,(ctl,				/* space for private structure	    */
    (void **)&priv,
    sizeof(priv_t),
    F, "priv"));
  DT_MEMSET(priv, 0, sizeof(priv_t));		/* clear private structure 			*/

  DT_C(dtos_strdup,(ctl, spec,			/* get space for spec edit 			*/
    &priv->spec));

  DT_C(dtstr_tokenize,(ctl,				/* separate spec into tokens 		*/
    priv->spec,
    ',', &ntokens));
										/* ................................ */
  p = priv->spec;

  if (ntokens >= 1)
  {
    dev = p;
	p += strlen(p) + 1;
  }

  if (ntokens >= 2)						/* width 							*/
  {
    DT_C(dtstr_to_long,(ctl,
      p, &longval));
	fb->xe = longval;
	p += strlen(p) + 1;
  }
  else
    fb->xe = 352;

  if (ntokens >= 3)						/* height 							*/
  {
    DT_C(dtstr_to_long,(ctl,
      p, &longval));
	fb->ye = longval;
	p += strlen(p) + 1;
  }
  else
    fb->ye = 288;

  if (ntokens >= 4)						/* flags+flags						*/
  {
    priv->flag_yuv411 = !!strstr(p, "yuv411");
    priv->flag_bi24 = !!strstr(p, "bi24");
    priv->skip = !!strstr(p, "skip");
    priv->dup = !!strstr(p, "dup");
  }
										/* ................................ */
  priv->fd = open(dev, O_RDONLY);
  if (priv->fd < 0)
  {
	rc = dt_err(ctl, F,
      "open(\"%s\", O_RDONLY)"
      DTERRNO_FMT,
      dev, DTERRNO_ARG(errno));
	goto cleanup;
  }
										/* ................................ */
				 
  geo.columns = fb->xe;
  geo.rows = fb->ye;
  geo.rows *= (priv->skip + 1);
  geo.rows /= (priv->dup + 1);
  geo.frames = 1;
  if (priv->flag_yuv411)
  {
    SVALUE(METEOR_GEO_YUV_PLANAR);
  }
  else
  {
    SVALUE(METEOR_GEO_RGB24);
  }
  geo.oformat = value;
  sysrc = ioctl(priv->fd, 
    METEORSETGEO, &geo);
  if (sysrc < 0)
  {
	rc = dt_err(ctl, F,
      "ioctl(%d, METEORSETGEO, {%d,%d,%d,%s (%d)})"
      DTERRNO_FMT,
      priv->fd,
	  geo.columns, geo.rows,
      geo.frames, svalue, value,
      DTERRNO_ARG(errno));
	goto cleanup;
  }
										/* ................................ */
  SVALUE(METEOR_FMT_NTSC);
  sysrc = ioctl(priv->fd, 
    METEORSFMT, &value);
  if (sysrc < 0)
  {
	rc = dt_err(ctl, F,
      "ioctl(%d, METEORSFMT, %s (%d))"
      DTERRNO_FMT,
      priv->fd,
	  svalue, value,
      DTERRNO_ARG(errno));
	goto cleanup;
  }
										/* ................................ */
  SVALUE(METEOR_INPUT_DEV0);
  sysrc = ioctl(priv->fd, 
    METEORSINPUT, &value);
  if (sysrc < 0)
  {
	rc = dt_err(ctl, F,
      "ioctl(%d, METEORSINPUT, %s (%d))"
      DTERRNO_FMT,
      priv->fd,
	  svalue, value,
      DTERRNO_ARG(errno));
	goto cleanup;
  }
										/* ................................ */
  sysrc = ioctl(priv->fd, 
    METEORGCHCV, &c);
  if (sysrc < 0)
  {
	rc = dt_err(ctl, F,
      "ioctl(%d, METEORGCHCV, &c)"
      DTERRNO_FMT,
      priv->fd,
      DTERRNO_ARG(errno));
	goto cleanup;
  }
  dt_dbg(ctl, F, DT_DBG_MASK_ALIMITS,
    "METEORGCHCV=%d", c);
										/* ................................ */
  sysrc = ioctl(priv->fd, 
    METEORGHUE, &c);
  if (sysrc < 0)
  {
	rc = dt_err(ctl, F,
      "ioctl(%d, METEORHUE, &c)"
      DTERRNO_FMT,
      priv->fd,
      DTERRNO_ARG(errno));
	goto cleanup;
  }
  dt_dbg(ctl, F, DT_DBG_MASK_ALIMITS,
    "METEORGHUE=%d", c);
										/* ................................ */
  if (priv->flag_yuv411)
	priv->mem_size = (geo.columns * geo.rows) * 3 / 2;
  else
	priv->mem_size = geo.columns * geo.rows * 4;
  
  dt_dbg(ctl, F, DT_DBG_MASK_ALIMITS,
    "geo.columns=%d geo.rows=%d mem_size=%d",
    geo.columns, geo.rows, priv->mem_size);
  
  priv->mem = (char *)mmap(
    (caddr_t)0, priv->mem_size, 
    PROT_READ, MAP_FILE|MAP_PRIVATE,
    priv->fd, (off_t)0);
  if (priv->mem == (char *)(-1))
  {
	rc = dt_err(ctl, F,
      "mmap(0, %d, PROT_READ, MAP_FILE|MAP_PRIVATE, %d, 0)"
      DTERRNO_FMT,
      priv->mem_size,
      priv->fd,
      DTERRNO_ARG(errno));
	goto cleanup;
  }
										/* ................................ */
  fb->priv = priv;
  DT_C(dtfb_meteor_report,(ctl, fb,		/* report what we have so far 		*/ 
    "1"));
										/* ................................ */
  
  fb->uninit   = dtfb_meteor_uninit;
  fb->control  = dtfb_meteor_control;
  fb->getblock = dtfb_meteor_getblock;
  fb->source   = dtfb_meteor_source;
  
  if (priv->flag_yuv411)
  {
    fb->org.org  = DT_COLOR_ORG_Y_CB_CR_411;
    fb->rowsize  = (3 * fb->xe) / 2;	/* 1.5 bytes per pixel 				*/
  }
  else
  {
	fb->org.org  = DT_COLOR_ORG_RGB888;
	fb->rowsize  = 3 * fb->xe;			/* three bytes per pixel 			*/
  }

  DT_C(dtfb_meteor_source,(ctl, fb, "rgain,1.0+ggain,1.0+bgain,1.0"));
  
cleanup:
  if (rc != DT_RC_GOOD)
  {
	DT_I(dtfb_meteor_release,(ctl, priv));
	DT_MEMSET(fb, 0, sizeof(*fb));
  }

  return rc;
# else
  return dt_err_notlinked(ctl, F, DESC);  
# endif
}

/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtfb_meteor_rev(						/* deliver revision 				*/
  dt_ctl_t *ctl,						/* environment control 				*/
  const char **rev_string,				/* returned rev string 				*/
  int *rev_major,						/* returned rev major 				*/
  int *rev_minor,						/* returned rev minor 				*/
  long *rev_date)						/* returned rev date 				*/
{
  if (rev_string != NULL)
    *rev_string = "dtfb_meteor lib";
  if (rev_major != NULL)
    *rev_major = 1;
  if (rev_minor != NULL)
    *rev_minor = 6;
  if (rev_date != NULL)
    *rev_date = 19970902;
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
