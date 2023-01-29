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
 * rev history
 * 19970228 1.2 add back2back setting on spec
 * 19970302 1.3 fixed bug causing mottled background in text
 * 19970302 1.3 allow very black text background
 * 19970307 1.4 retry rtdPlay in vp_set_control
 *..........................................................................*/

#include <dtack/base.h>							/* NCI fundamental declarations 	*/
#include <umdec.h>						/* public to the world 				*/

#include <dtack/os.h>						/* utility OS functions 			*/
#include <dtack/str.h>						/* utility string functions 		*/

#include <dtack/list.h>
#include <dtack/liner.h>

#include <dtack/errno.h>

DT_RCSID("source/mpeg $RCSfile: umdecvp,v $ $Revision: 1.7 $");

#define DESC "Optibase VideoPlex"

#if DTPRJ_MPEG_VIDEOPLEX == DT_1

#ifndef DTDEPEND
#  ifdef DTCONFIG_IMPLIED_QNX4
#    include <sys/name.h>				/* QNX specific 					*/
#  endif

#  include <globedef.h>					/* from videoplex sdk 				*/
#  include <struct.h>
#  include <videoplx.h>
#  include <osd.h>
#  define ushort unsigned short
#  include <osd_int.h>
#  include <nci_opti.h>
#endif

typedef struct {
  char *spec;
  char *settings;
  char *overlay_image_filename;
  char *overlay_text_string;
  int did_initsystem;
  int did_initboard;
  int board;
  int flag_back2back;
  int name_attach;
  double last_pts;
  umdec_mode_e mode;
  int overlay_initialized;
  int overlay_image_x0;
  int overlay_image_y0;
  int overlay_text_x0;
  int overlay_text_y0;
  int overlay_text_height;
  int overlay_text_foreground;
  int overlay_text_background;
} umdec_vp_priv_t;

#define NAME_ATTACH "videoplex"			/* name to attach 					*/

#define VP_ERROR(FUNC) 					\
{										\
  rc = dt_err(ctl, F,					\
    "%s returned %d", FUNC, sr);		\
  goto cleanup;							\
}

static unsigned long gray_pallette[16]=
{
  0xc2c2e0e0,
  0xc2cce0e0,
  0xc2dce0e0,
  0xc2ece0e0,
  0xccc2e0e0,
  0xcccce0e0,
  0xccdce0e0,
  0xccece0e0,
  0xdcc2e0e0,
  0xdccce0e0,
  0xdcdce0e0,
  0xdcece0e0,
  0xecc2e0e0,
  0xeccce0e0,
  0xecdce0e0,
  0xecece0e0
};

static unsigned long color_pallette[16]=
{
     0xC0C0c0c0,					/*/  [0] --- (CLEAR) 				*/
     0xD8D8E5F1,					/*/  [1] --- (VIOLET) 				*/
     0xEAEAC9E3,					/*/  [2] --- (YELLOW) 				*/
     0xF1F1DEE6,					/*/  [3] --- (PINK)   				*/
     0xC7C7EEDD,					/*/  [4] --- (NAVY)   				*/
     0xE3E3E6E9,					/*/  [5] --- (ORCHID) 				*/
     0xEAEAE9C4,					/*/  [6] --- (CYAN)   				*/
     0xCACAFCDB,					/*/  [7] --- (BLUE)   				*/
     0xEFEFC7E8,					/*/  [8] --- (GOLD)   				*/
     0xD8D8D4E7,					/*/  [9] --- (ORANGE) 				*/
     0xE7E7DBC6,					/*/  [A] --- (GREEN)  				*/
     0xE1E1E8CA,					/*/  [B] --- (TURQOISE) 			*/
     0xD4D4D6FC,					/*/  [C] --- (RED)	  				*/
     0xC4C4E0E0,					/*/  [D] --- (BLACK) 				*/
     0xFAFAE0E0,					/*/  [E] --- (WHITE) 				*/
     0xF0F0E4DC						/*/  [F] --- (GRAY)  				*/
};

#  define QUAD4(V1, V2, V3, V4) \
    ((((unsigned char)(V1) & 0xc0)   ) | \
     (((unsigned char)(V2) & 0xc0)>>2) | \
     (((unsigned char)(V3) & 0xc0)>>4) | \
     (((unsigned char)(V4) & 0xc0)>>6))
#  define QUAD1(V) QUAD4(V, V, V, V)
#  define QUADP(V) QUAD4((V)[0], (V)[1], (V)[2], (V)[3])

#define  OVERLAY_IMAGE_CONSTANT_FORMAT "%d,%d,0x%lx"
#define  OVERLAY_IMAGE_CONSTANT_N (3)

/*..........................................................................
 *..........................................................................*/

static
dt_rc_e
umdec_vp_overlay_release(				/* release overlay stuff			*/
  dt_ctl_t *ctl,						/* environment control 				*/
  umdec_vp_priv_t *priv,
  int release_image,
  int release_text)
{
  DT_F("umdec_vp_overlay_release");
  dt_rc_e rc = DT_RC_GOOD;

  if (release_image >= 0 &&
	  priv->overlay_image_filename != NULL)
  {
    DT_I(dtos_free2,(ctl,
      priv->overlay_image_filename,
      F, "overlay image filename"));
	priv->overlay_image_filename = NULL;
  }

  if (release_text >= 0 &&
	  priv->overlay_text_string != NULL)
  {
    DT_I(dtos_free2,(ctl,
      priv->overlay_text_string,
      F, "overlay text string"));
	priv->overlay_text_string = NULL;
  }
  return rc;
}

/*..........................................................................
 *..........................................................................*/

static
dt_rc_e
umdec_vp_release(						/* release resources 				*/
  dt_ctl_t *ctl,
  umdec_vp_priv_t *priv)
{
  DT_F("umdec_vp_release");
  dt_rc_e rc = DT_RC_GOOD;

  if (priv != NULL)
  {
    nci_opti_set_ctl(ctl);

	DT_I(umdec_vp_overlay_release,(		/* release overlay stuff 			*/
      ctl, priv, 0, 0));
	if (priv->did_initboard)
      rtdFreeBoard(priv->board);
	if (priv->did_initsystem)
      rtdFreeSystem();
  
#   ifdef DTCONFIG_IMPLIED_QNX4
    if (priv->name_attach != 0 &&
	    priv->name_attach != -1)
      qnx_name_detach(					/* unregister our name				*/
        getnid(), priv->name_attach);
#   endif

	if (priv->spec != NULL)
      DT_I(dtos_free2,(ctl, priv->spec,
        F, "priv->spec"));
    DT_I(dtos_free2,(ctl, priv,
      F, "priv"));
  }
  return rc;
}

/*..........................................................................
 * Scan file for line containing string.
 *..........................................................................*/

static
dt_rc_e
umdec_vp_pci_scan(						/* scan pci config file for string	*/
  dt_ctl_t *ctl,
  dtfd_t *fd,							/* opened file 						*/
  const char **string,					/* strings to search for 			*/
  char **line)							/* returned line needs to be freed 	*/
{
  DT_F("umdec_vp_pci_scan");

  while(1)
  {
	const char **s = string;
	DT_Q(dtliner_fd_line,(ctl, fd,		/* read next line from file			*/
      line));
	if (*line == NULL)					/* end of file? 					*/
	  return dt_err(ctl, F,
        "could not find \"%s\""
        " in %s",
        *string, fd->filename);
	for (s=string; *s!=NULL; s++)
	  if (strstr(*line, *string))		/* does line contain string? 		*/
  	    break;
	if (*s != NULL)						/* found a string? 					*/
	  break;
	DT_Q(dtos_free2,(ctl, *line,		/* free the lines we don't keep 	*/
      F, "unwanted line"));
  }
	  
  return DT_RC_GOOD;
}

/*..........................................................................
 * Read videoplex resources out of /CONFIG/show_pci2.out.
 * Stuff them in global variables read by the nci port of the videoplex sdk.
 * The vendor code is hard-coded here for now, but it could be gotten
 * from a global variable in the nci port.
 *..........................................................................*/

static
dt_rc_e
umdec_vp_pci(							/* pci resources 					*/
  dt_ctl_t *ctl,
  umdec_vp_priv_t *priv)
{
  DT_F("umdec_vp_pci");
  dtfd_t _fd, *fd = &_fd;
  char *line = NULL;
  char *p;
  int n;
# define CONFIG "/CONFIG/show_pci2.out"
  char *vendors[] = {					/* vendor code for this card 		*/
    "ID=1255h",
	NULL
  };
  char *addresses[] = {	
    "Base Address",						/* line containing the addresses 	*/
	"Vendor",							/* need address before next vendor	*/
	NULL
  };
  dt_rc_e rc = DT_RC_GOOD;

  nci_opti_set_ctl(ctl);

  fd->fd = -1;
  DT_C(dtfd_open,(ctl, fd,				/* open config file 				*/
    CONFIG, "r"));	
  
  DT_C(umdec_vp_pci_scan,(ctl, fd,		/* scan for our vendor code 		*/
    vendors, &line));
  DT_C(dtos_free2,(ctl, line,			/* don't need this line				*/
    F, "vendor line"));
  line = NULL;

  DT_C(umdec_vp_pci_scan,(ctl, fd,		/* scan for the base address line	*/
    addresses, &line));
  
  p = strstr(line, "MEM");				/* find interesting part of line	*/
  if (p == NULL)						/* didn't get address line? 		*/
  {
	rc = dt_err(ctl, F,
      "MEM not found in \"%s\"", line);
	goto cleanup;
  }
									
  n = sscanf(p,							/* parse the address line 			*/
    "MEM@%lxh,32bit length %ld "
    "IO@%lxh length %ld bytes "
    "IO@%lxh length %ld",
    &nci_opti_mem1_addr,
    &nci_opti_mem1_length,
    &nci_opti_io_addr,
    &nci_opti_io_length,
    &nci_opti_mem2_addr,
    &nci_opti_mem2_length);
  
  if (n != 6)							/* didn't get all values?			*/
  n = sscanf(p,							/* try it the old way				*/
    "MEM@%lxh,32bit length %ld "
    "IO@%lxh length %ld bytes "
    "MEM@%lxh,32bit length %ld",
    &nci_opti_mem1_addr,
    &nci_opti_mem1_length,
    &nci_opti_io_addr,
    &nci_opti_io_length,
    &nci_opti_mem2_addr,
    &nci_opti_mem2_length);
  
  if (n != 6)							/* didn't get all values?			*/
  {
	rc = dt_err(ctl, F,
      "found only %d values out of 6"
      " in \"%s\"",
      n, p);
	goto cleanup;
  }

  nci_opti_irq_num = 0;					/* we don't use interrupts */
  nci_opti_irq_options = 0;

  dt_dbg(ctl, F, DT_DBG_MASK_CONFIG,
    "mem1=0x%08lx/%ld io=0x%04lx/%ld mem2=0x%08lx/%ld",
    nci_opti_mem1_addr,
    nci_opti_mem1_length,
    (unsigned long)nci_opti_io_addr,
    nci_opti_io_length,
    (unsigned long)nci_opti_mem2_addr,
    nci_opti_mem2_length);

cleanup:
  if (line != NULL)					
    DT_I(dtos_free2,(ctl, line,			/* free last line 					*/
      F, "last line"));
  if (fd->fd != -1)
    DT_I(dtfd_close,(ctl, fd));			/* close config file 				*/
  return rc;
}

/*..........................................................................
 *..........................................................................*/

static
dt_rc_e
umdec_vp_uninit(						/* close object						*/
  dt_ctl_t *ctl,
  umdec_t *umdec)
{
  DT_F("umdec_vp_uninit");
  umdec_vp_priv_t *priv =
    umdec->priv;
  DT_Q(umdec_vp_release,(ctl, priv));
  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/

static
dt_rc_e
umdec_vp_status(
  dt_ctl_t *ctl,
  umdec_t *umdec)
{
  DT_F("umdec_vp_status");
  umdec_vp_priv_t *priv =
    umdec->priv;
  short sr;
  long v;
  char s[128];
  dt_rc_e rc = DT_RC_GOOD;

  nci_opti_set_ctl(ctl);

  s[0] = '\0';							/* clear output string 				*/

# define PARAM(P, A) \
  sr = rtdGetParams(priv->board, P, &v); \
  if (sr < 0) \
    VP_ERROR("rtdGetParams(" #P ")"); \
  dtstr_printf(ctl, \
    s+strlen(s), sizeof(s)-strlen(s), \
    "%s=%ld%s", #P, v, A);

  PARAM(DATA_SIZE_READ, ", ");
  PARAM(USE_PTS, ", ");
  PARAM(USE_FILE_INFO, "");
  
/*printf("%s\n", s);*/
  
# undef PARAM
  
cleanup:
  return rc;
}

/*..........................................................................
 *..........................................................................*/

static
const
char *
mode_string(
  umdec_mode_e mode)
{
# define S(MODE) \
  if (mode == (MODE)) \
    return #MODE

  S(UMDEC_MODE_STARTING);
  S(UMDEC_MODE_FORWARD);
  S(UMDEC_MODE_FFORWARD);
  S(UMDEC_MODE_SFORWARD);
  S(UMDEC_MODE_1FORWARD);
  S(UMDEC_MODE_1FORWARDED);
  S(UMDEC_MODE_PAUSE);
  S(UMDEC_MODE_STOP);

  return "UMDEC_UNKNOWN";
# undef S
}
/*..........................................................................
 *..........................................................................*/

static
dt_rc_e
umdec_vp_set_control(					/* set operation parameters 		*/
  dt_ctl_t *ctl,						/* environment control 				*/
  umdec_t *umdec,						/* decoder object structure 		*/
  umdec_control_t *control)				/* control parameters 				*/
{
  DT_F("umdec_vp_set_control");
  umdec_vp_priv_t *priv =
    umdec->priv;
  short sr;
  dt_rc_e rc = DT_RC_GOOD;

  if (control->mode != priv->mode)		/* not already in the mood?			*/
  {
	int speed = 0;
	int play;

	if (control->mode ==
		UMDEC_MODE_FORWARD)
	  speed = NORMAL_SPEED;
	else
	if (control->mode ==
		UMDEC_MODE_FFORWARD)
	  speed = FAST_FWD;
	else
	if (control->mode ==
		UMDEC_MODE_SFORWARD)
	  speed = SLOW_SPEED_4;

	if (priv->mode == UMDEC_MODE_STARTING ||
		priv->mode == UMDEC_MODE_FORWARD ||
		priv->mode == UMDEC_MODE_FFORWARD ||
		priv->mode == UMDEC_MODE_SFORWARD)
	  play = 0;
	else 
	  play = 1;

	if (control->mode ==				/* this is a play mode? 			*/
		UMDEC_MODE_FORWARD ||
		control->mode ==
		UMDEC_MODE_FFORWARD ||
		control->mode ==
		UMDEC_MODE_SFORWARD)
	{
	  dt_dbg(ctl, F, DT_DBG_MASK_COMMAND,
        "mode=%s->%s"
        " speed=%d play=%d",
        mode_string(priv->mode), 
        mode_string(control->mode),
        speed, play);
	  if (play)
	  {
	    sr = rtdPlay(priv->board,		/* start play 						*/
          CUR_FRAME,
          LAST_FRAME,
          NORMAL_SPEED);
	    if (sr < 0)
          VP_ERROR("rtdPlay("
            "CURRENT_FRAME,"
            " LAST_FRAME,"
            " NORMAL_SPEED)");
	  }

	  rtdSetParams(priv->board,
        PLAY_SPEED, speed);
	}
	else					
	if (control->mode ==				/* this is a single step? 			*/
		UMDEC_MODE_1FORWARD)
	{
	  dt_dbg(ctl, F, DT_DBG_MASK_COMMAND,
        "mode=%s->%s",
        mode_string(priv->mode), 
        mode_string(control->mode));
	  sr = rtdSingle(priv->board);
	  if (sr < 0)
        VP_ERROR("rtdSingle()");
	  control->mode =
  	    UMDEC_MODE_1FORWARDED;
	}
	else					
	if (control->mode ==				/* this is pause mode? 				*/
		UMDEC_MODE_PAUSE)
	{
	  dt_dbg(ctl, F, DT_DBG_MASK_COMMAND,
        "mode=%s->%s",
        mode_string(priv->mode), 
        mode_string(control->mode));
	  sr = rtdPause(priv->board);
	  if (sr < 0)
        VP_ERROR("rtdPause()");
	}
	else					
	if (control->mode ==				/* this is stop mode? 				*/
		UMDEC_MODE_STOP)
	{
	  dt_dbg(ctl, F, DT_DBG_MASK_COMMAND,
        "mode=%s->%s",
        mode_string(priv->mode), 
        mode_string(control->mode));
	  sr = rtdPause(priv->board);		/* this doesn't clear the display 	*/
	  if (sr < 0)
        VP_ERROR("rtdPause()");
	  sr = rtdPlay(priv->board,			/* this clears the display			*/
        FIRST_FRAME,			
        LAST_FRAME,
        NORMAL_SPEED);
	  if (sr < 0)
        VP_ERROR("after PAUSE at STOP"
          " rtdPlay(FIRST_FRAME,"
          " LAST_FRAME, NORMAL_SPEED)");
	}
  }

  priv->mode = control->mode;			/* remember actual mode 			*/
cleanup:
  return rc;
}

/*..........................................................................
 *..........................................................................*/

static
dt_rc_e
umdec_vp_get_control(					/* get operation parameters 		*/
  dt_ctl_t *ctl,						/* environment control 				*/
  umdec_t *umdec,						/* decoder object structure 		*/
  umdec_control_t *control)				/* returned control parameters		*/
{
  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/

static
dt_rc_e
umdec_vp_get_status(					/* get operation parameters 		*/
  dt_ctl_t *ctl,							/* environment control 				*/
  umdec_t *umdec,						/* decoder object structure 		*/
  umdec_status_t *status)				/* returned status					*/
{
  DT_F("umdec_vp_get_status");
										/* status.frame and status.pts		*/
										/* are already set during shovel 	*/
  return DT_RC_GOOD;
}


/*..........................................................................
 *..........................................................................*/

static
dt_rc_e
umdec_vp_shovel(
  dt_ctl_t *ctl,
  umdec_t *umdec)
{
  DT_F("umdec_vp_shovel");
  umdec_vp_priv_t *priv =
    umdec->priv;
  const long want = 8192;
  void *buffer = NULL;
  long got;
  short sr;
  double frame, pts;
  static nprint = 0;
  dt_rc_e rc = DT_RC_GOOD;

  nci_opti_set_ctl(ctl);

  DT_C(dtos_malloc2,(ctl, 
    &buffer, want,
    F, "buffer"));

  got = 0;
  while (1)
  {
	if (got == 0)						/* previous buffer flushed? 		*/
	{
	  DT_C((*umdec->feed_callback),(	/* get data 						*/
        ctl, umdec, umdec->feed_arg,
        buffer, want, &got));
	  if (got == 0)						/* end of data stream? 				*/
        break;
	}
	else								/* previous buffer pending? 		*/
	{
	  DT_C((*umdec->feed_callback),(	/* just poll for commands			*/
        ctl, umdec, umdec->feed_arg,
        buffer, 0, NULL));
	}
	if (umdec->control.mode ==			/* callback says to stop?		    */
		UMDEC_MODE_STOP)
  	  goto cleanup;
	sr = rtdPlayBuf(priv->board,		/* give data to videoplex fifo		*/
      buffer, got);
	if (sr < 0)							/* something bad happened? 			*/
      VP_ERROR("rtdPlayBuf")
	else							
	if (sr != 3)						/* it all fit? 						*/
	  got = 0;							/* mark buffer as flushed 			*/
	else								/* videoplex fifo too full? 		*/
	  DT_C(dtos_delay,(ctl, 10));		/* sit around awhile 				*/

	sr = rtdGetParams(priv->board,		/* get current pts 					*/
      NCI_PARAM_PTS,
      (LPLONG)&pts);
	if (sr < 0)			
      VP_ERROR("rtdGetParams("
        "NCI_PARAM_PTS)");

	if (umdec->status.pts == 0.0)		/* first PTS? 						*/
	  umdec->status.pts = pts;

	frame = umdec->status.frame +		/* compute frame from PTS 			*/
      (pts - umdec->status.pts) /
      90000.0;
	umdec->status.pts = pts;

	while(umdec->status.frame < frame)	/* give each frame inc to caller 	*/
	{
      DT_C((*umdec->frame_callback),(	
        ctl, umdec, umdec->frame_arg, 
        UMDEC_FRAMETYPE_FRAMEINC));
	  umdec->status.frame++;
	  if (umdec->control.mode ==
		  UMDEC_MODE_STOP)
	    goto cleanup;
	}
  }

cleanup:
  if (buffer != NULL)
    DT_I(dtos_free2,(ctl, buffer,
      F, "buffer"));

  return rc;
}

/*..........................................................................
 * b08 image is expected to be grayscale 0=black 255=white
 * only two MSBits used to select pallette entry
 *..........................................................................*/

static
dt_rc_e
umdec_vp_overlay_init_constant(			/* set up block from b08 image		*/
  dt_ctl_t *ctl,
  umdec_vp_priv_t *priv,
  int block,
  int xe, 
  int ye,
  unsigned char background,				/* block background before image 	*/
  int x0,								/* where block goes on screen 		*/
  int y0)
{
  DT_F("umdec_vp_overlay_init_constant");
  short sr;
  int size;
  unsigned char *data = NULL;
  OSD_BLK_HEADER obh;
  dt_rc_e rc = DT_RC_GOOD;

  xe = (xe + 15) & ~15;					/* width multiple of 16 			*/
  ye = (ye + 1) & ~1;					/* height multiple of 2 			*/

  obh.xdim= xe;
  obh.ydim= ye;

  size = xe / 4 * ye;					/* 4 bits per pixel pair			*/

  DT_C(dtos_malloc2,(ctl,				/* space for interleaved data 		*/
    (void **)&data, size,
    F, "interleaved data"));

  memset(data, background, size); /* clear block to background color 	*/

  dt_dbg(ctl, F, DT_DBG_MASK_CONFIG,
    "block at %dx%d, size %dx%d background 0x%02x",
    x0, y0, xe, ye, background);

  obh.bbuf[0] = (ulong *)data;
  obh.bbuf[1] = (ulong *)(data + size / 2);

  obh.odd_pal_included = 0;
  memcpy(obh.evn_palette, color_pallette, sizeof(color_pallette));
  memset(obh.odd_palette, 0, sizeof(color_pallette));

  sr = osd_BlockLoad(priv->board,
    block, obh, x0, y0);
  if (sr != 0)
    VP_ERROR("osd_BlockLoad");

cleanup:
  if (data != NULL)
    DT_I(dtos_free2,(ctl, data,
      F, "interleaved data"));
  
  return rc;
}

/*..........................................................................
 * b08 image is expected to be grayscale 0=black 255=white
 * only two MSBits used to select pallette entry
 *..........................................................................*/

static
dt_rc_e
umdec_vp_overlay_init_b08(				/* set up block from b08 image		*/
  dt_ctl_t *ctl,
  umdec_vp_priv_t *priv,
  int block,
  unsigned char background,				/* block background before image 	*/
  dtimage_t *image,
  int x0,								/* where block goes on screen 		*/
  int y0)
{
  DT_F("umdec_vp_overlay_init_b08");
  short sr;
  int xe;
  int ye;
  int size;
  dtimage_t large;
  unsigned char *data = NULL;
  unsigned char *p;
  int y;
  OSD_BLK_HEADER obh;
  dt_rc_e rc = DT_RC_GOOD;

  large.xe = 0;

  xe = (image->xe + 15) & ~15;			/* width multiple of 16 			*/
  ye = (image->ye + 1) & ~1;			/* height multiple of 2 			*/

  dt_dbg(ctl, F, DT_DBG_MASK_CONFIG,
    "image=%dx%d, block=%dx%d, background=0x%02x",
    image->xe, image->ye, xe, ye, background);

  obh.xdim= xe;
  obh.ydim= ye;

  DT_C(dtimage_create,(ctl, &large,		/* create proper sized image 		*/
    xe, ye, image->ntype));
  DT_C(dtimage_constant,(ctl, &large,	/* fill with clear background 		*/
    0.0));
  DT_C(dtimage_cast,(ctl, image,		/* copy smaller image in 			*/
    &large));

  size = xe / 4 * ye;					/* 4 bits per pixel pair			*/

  DT_C(dtos_malloc2,(ctl,				/* space for interleaved data 		*/
    (void **)&data, size,
    F, "interleaved data"));

  memset(data, QUAD1(background), size); /* clear block to background color 	*/

  obh.bbuf[0] = (ulong *)data;
  obh.bbuf[1] = (ulong *)(data + size / 2);

  p = data;
  for (y=0; y<large.ye; y+=2)			/* copy even rows 					*/
  {
	int x;
	unsigned char *q = large.row[y].b08;
	for (x=0; x<large.xe; x+=4)
	{
	  *p = QUADP(q+x);
#ifdef NONO
	  if ((q[x+0] || q[x+1]) && !(*p & 0xf0)) *p |= 0x10;
	  if ((q[x+2] || q[x+3]) && !(*p & 0x0f)) *p |= 0x01;
#endif
	  p++;
	}
  }

  p = data + size / 2;
  for (y=1; y<large.ye; y+=2)			/* copy odd rows 					*/
  {
	int x;
	unsigned char *q = large.row[y].b08;
	for (x=0; x<large.xe; x+=4)
	{
	  *p = QUADP(q+x);
#ifdef NONO
	  if ((q[x+0] || q[x+1]) && !(*p & 0xf0)) *p |= 0x10;
	  if ((q[x+2] || q[x+3]) && !(*p & 0x0f)) *p |= 0x01;
#endif
	  p++;
	}
  }

  obh.odd_pal_included = 0;
  memcpy(obh.evn_palette, gray_pallette, sizeof(gray_pallette));
  memset(obh.odd_palette, 0, sizeof(gray_pallette));

  sr = osd_BlockLoad(priv->board,
    block, obh, x0, y0);
  if (sr != 0)
    VP_ERROR("osd_BlockLoad");

cleanup:
  if (data != NULL)
    DT_I(dtos_free2,(ctl, data,
      F, "interleaved data"));
  if (large.xe != 0)
    DT_I(dtimage_free,(ctl, &large));
  
  return rc;
}

/*..........................................................................
 *..........................................................................*/

static
dt_rc_e
umdec_vp_overlay_init_text(				/* set up text overlay				*/
  dt_ctl_t *ctl,
  umdec_vp_priv_t *priv,
  int block)
{
  DT_F("umdec_vp_overlay_init_text");
  dtimage_t small, large;
  double scale;
  dt_rc_e rc = DT_RC_GOOD;

  small.xe = 0;
  large.xe = 0;

  dt_dbg(ctl, F, DT_DBG_MASK_CONFIG,
    "overlay string %s at %dx%d colors 0x%08lx/0x%08lx", 
    priv->overlay_text_string,
    priv->overlay_text_x0,
    priv->overlay_text_y0,
    priv->overlay_text_foreground,
    priv->overlay_text_background);
  
  DT_C(dtimage_create,(ctl, &small,		/* create the image 				*/
    8*strlen(priv->overlay_text_string),
    16,
    DT_NTYPE_B08));
  DT_C(dtimage_constant,(ctl,			/* set to background 				*/
    &small,  
    priv->overlay_text_background & 
      0xff));
  DT_C(dtimage_plot_text_8x16,(ctl,		/* draw the text 					*/
    &small, 
    priv->overlay_text_string,  
    0, 0,
    priv->overlay_text_foreground & 
      0xff));

	scale = 16.0 / (double)				/* extension factor 				*/
	  priv->overlay_text_height;

#   ifndef NONO
	DT_C(dtimage_resample,(ctl, &small, /* resize the image					*/
      scale, scale, 
      "bilinear", &large));
#   else
	large=small;
#   endif

  DT_C(umdec_vp_overlay_init_b08,(		/* set up block from b08 image		*/
    ctl, priv, block,
    priv->overlay_text_background,
    &large,
    priv->overlay_text_x0,
    priv->overlay_text_y0));

cleanup:
  if (large.xe != 0 &&
	  large.row != small.row)
    DT_I(dtimage_free,(ctl, &large));
  if (small.xe != 0)
    DT_I(dtimage_free,(ctl, &small));
  return rc;
}

/*..........................................................................
 *..........................................................................*/

static
dt_rc_e
umdec_vp_overlay_init_image(			/* set up image overlay				*/
  dt_ctl_t *ctl,
  umdec_vp_priv_t *priv,
  int block)
{
  DT_F("umdec_vp_overlay_init_image");
  int isreadable;
  int n;
  long background;
  int xe, ye;
  dt_rc_e rc = DT_RC_GOOD;

  DT_C(dtos_isreadable,(ctl,
    priv->overlay_image_filename,
    &isreadable));
  n = sscanf(priv->overlay_image_filename,
    OVERLAY_IMAGE_CONSTANT_FORMAT,
    &xe, &ye, &background);
  if (isreadable)						/* it's a file? 					*/
  {
	short overlay_image_x0 =
      priv->overlay_image_x0;
	short overlay_image_y0 =
      priv->overlay_image_y0;
	short sr;
    dt_dbg(ctl, F, DT_DBG_MASK_CONFIG,
      "overlay image %s at %dx%d",
      priv->overlay_image_filename,
      priv->overlay_image_x0,
      priv->overlay_image_y0);

	sr = rtdOsdFileLoad(priv->board,	/* load the image file 				*/
      priv->overlay_image_filename, 
      block,
      &overlay_image_x0,
      &overlay_image_y0);
	if (sr != 0)
      VP_ERROR("rtdOsdFileLoad");
  }
  else
  if (n == OVERLAY_IMAGE_CONSTANT_N)	/* it's the special constant form?	*/
  {
	DT_C(umdec_vp_overlay_init_constant,(
      ctl, priv, block,
      xe, ye,
      background,
      priv->overlay_image_x0,
      priv->overlay_image_y0));
  }
  else
  {
    rc = dt_err(ctl, F,
      "invalid filename \"%s\" (scan n=%d)",
      priv->overlay_image_filename, n);
	goto cleanup;
  }

cleanup:
  return rc;
}

/*..........................................................................
 *..........................................................................*/

static
dt_rc_e
umdec_vp_overlay_init(					/* set up overlay					*/
  dt_ctl_t *ctl,
  umdec_vp_priv_t *priv)
{
  DT_F("umdec_vp_overlay_init");
  short sr;
  dt_rc_e rc = DT_RC_GOOD;

  if (priv->overlay_initialized)		/* already initialized? 			*/
    return DT_RC_GOOD;
										/* ................................ */
  sr = osd_Init(priv->board);			/* init on-screen display 			*/
  if (sr != 0)
    VP_ERROR("osd_Init");
										/* ................................ */
  if (priv->overlay_text_string &&		/* a text string is defined?		*/
	  priv->overlay_image_filename)		/* and image filename defined?		*/
  {
	if (priv->overlay_image_y0 <		/* image is higher on screen? 		*/
		priv->overlay_text_y0)
	{
      DT_C(umdec_vp_overlay_init_image,(ctl, priv, 0));
      DT_C(umdec_vp_overlay_init_text,(ctl, priv, 1));
	}
	else
	{
      DT_C(umdec_vp_overlay_init_text,(ctl, priv, 0));
      DT_C(umdec_vp_overlay_init_image,(ctl, priv, 1));
	}
  }
  else
  if (priv->overlay_text_string)		/* a text string is defined?		*/
  {
	DT_C(umdec_vp_overlay_init_text,(ctl, priv, 0));
  }
  else
  if (priv->overlay_image_filename)		/* an image filename is defined?	*/
  {
	DT_C(umdec_vp_overlay_init_image,(ctl, priv, 0));
  }
cleanup:
  return rc;
}

/*..........................................................................
 *..........................................................................*/

static
dt_rc_e
umdec_vp_go(							/* start playing					*/
  dt_ctl_t *ctl,
  umdec_t *umdec,						/* umdec object 					*/
  umdec_control_t *control)				/* initial mode 					*/
{
  DT_F("umdec_vp_go");
  umdec_vp_priv_t *priv =
    umdec->priv;
  short sr;
  dt_rc_e rc;

  DT_C(umdec_vp_overlay_init,(ctl,		/* init overlay activity 			*/
    priv));
  
  sr = rtdPlay(priv->board,				/* start play 						*/
    FIRST_FRAME,						/* ignore hardware buffer contents	*/
    LAST_FRAME,
    NORMAL_SPEED);
  if (sr < 0)
    VP_ERROR("rtdPlay");

  priv->mode = UMDEC_MODE_STARTING;

  DT_C(umdec_vp_set_control,(ctl,		/* set initial speed 				*/
    umdec, control));

  sr = osd_Mode(priv->board, 1);		/* enable on-screen display 		*/
  if (sr != 0)
    VP_ERROR("osd_Mode");

  DT_C(umdec_vp_shovel,(ctl,			/* shovel it ourselves 				*/
    umdec));

cleanup:
  return rc;
}


/*..........................................................................
 *..........................................................................*/

static
dt_rc_e
umdec_vp_overlay_image(					/* load image into overlay			*/
  dt_ctl_t *ctl,						/* environment control 				*/
  umdec_t *umdec,						/* decoder object structure 		*/
  const char *filename,					/* filename containing image 		*/
  int x0,								/* column 							*/
  int y0)								/* row 								*/
{
  DT_F("umdec_vp_overlay_image");
  umdec_vp_priv_t *priv =
    umdec->priv;
  int isreadable;
  int n;
  int xe, ye;
  long background;

  DT_Q(dtos_isreadable,(ctl,			/* check if file is there 			*/
    filename, &isreadable));
  n = sscanf(filename,
    OVERLAY_IMAGE_CONSTANT_FORMAT,
    &xe, &ye, &background);

  if (!isreadable &&
	  n != OVERLAY_IMAGE_CONSTANT_N)
    return dt_err(ctl, F,
      "file %s is not readable",
      filename);

  DT_Q(umdec_vp_overlay_release,(ctl,	/* release previous overlay stuff 	*/
    priv, 0, -1));

  DT_Q(dtos_strdup,(ctl, filename,		/* remember the name 				*/
    &priv->overlay_image_filename));
  priv->overlay_image_x0 = x0;			/* remember the position 			*/
  priv->overlay_image_y0 = y0;

  priv->overlay_initialized = 0;		/* need to re-initialize overlay	*/

  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/

static
dt_rc_e
umdec_vp_overlay_text(					/* load text into overlay			*/
  dt_ctl_t *ctl,						/* environment control 				*/
  umdec_t *umdec,						/* decoder object structure 		*/
  const char *string,					/* text to display 					*/
  int x0,								/* column 							*/
  int y0,								/* row 								*/
  int height,							/* text height in pixels 			*/
  long foreground,						/* foreground color RGB 			*/
  long background)						/* background color RGB 			*/
{
  DT_F("umdec_vp_overlay_text");
  umdec_vp_priv_t *priv =
    umdec->priv;
  char *s;
  int n, i;
  DT_Q(umdec_vp_overlay_release,(ctl,	/* release previous overlay stuff 	*/
    priv, -1, 0));
  DT_Q(dtos_strdup,(ctl, string, &s));	/* remember the string				*/
  n = strlen(s);
  for (i=0; i<n; i++)					/* change hats to spaces			*/
    if (s[i] == '^')
	  s[i] = ' ';
  priv->overlay_text_string = s;
  priv->overlay_text_x0 = x0;			/* remember the position 			*/
  priv->overlay_text_y0 = y0;
  priv->overlay_text_height = height;
  priv->overlay_text_foreground = foreground;
  priv->overlay_text_background = background;

  priv->overlay_initialized = 0;		/* need to re-initialize overlay	*/
  
  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/

static
dt_rc_e
umdec_vp_overlay_clear(					/* clear overlay					*/
  dt_ctl_t *ctl,						/* environment control 				*/
  umdec_t *umdec)						/* decoder object structure 		*/
{
  DT_F("umdec_vp_overlay_clear");
  umdec_vp_priv_t *priv =
    umdec->priv;
  short sr;
  dt_rc_e rc = DT_RC_GOOD;

  DT_Q(umdec_vp_overlay_release,(ctl,	/* release previous overlay stuff 	*/
    priv, 0, 0));

  priv->overlay_initialized = 0;		/* need to re-initialize overlay	*/

  sr = osd_Init(priv->board);			/* init on-screen display 			*/
  if (sr != 0)
    VP_ERROR("osd_Init");

cleanup:  
  return rc;
}

#endif

/*..........................................................................
| NAME
| umdec_ssg_init() - initialize ssg decoder object
|
| SUMMARY
| Initialize the decoder object ~mdec~.
| 
| END
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
umdec_vp_init(
  dt_ctl_t *ctl,
  umdec_t *umdec,
  const char *spec)
{
  DT_F("umdec_vp_init");
# if DTPRJ_MPEG_VIDEOPLEX == DT_1
  umdec_vp_priv_t *priv = NULL;
  int ntokens;
  char *p;
  short sr;
  dt_rc_e rc;

  nci_opti_set_ctl(ctl);

  if (spec == NULL)
  {
    rc = dt_err(ctl, F,
      "NULL umdecvp spec");
    goto cleanup;
  }

  DT_C(dtos_malloc2,(ctl,				/* space for private structure	    */
    (void **)&priv,
    sizeof(umdec_vp_priv_t),
    F, "priv"));
  memset(priv, 0,						/* clear private structure 			*/
    sizeof(umdec_vp_priv_t));

  DT_C(dtos_strdup,(ctl, spec,			/* get space for spec edit 			*/
    &priv->spec));

  DT_C(dtstr_tokenize,(ctl,				/* separate spec into tokens 		*/
    priv->spec,
    ',', &ntokens));

  p = priv->spec;

  if (ntokens >= 1)
	priv->settings = p;
  else
    priv->settings = "";

# ifdef DTCONFIG_IMPLIED_QNX4
  priv->name_attach = qnx_name_attach(	/* register our name locally 		*/
    getnid(), NAME_ATTACH);
  if (priv->name_attach == -1)
  {
	rc = dt_err(ctl, F,
      "qnx_name_attach(%d, \"%s\")"
      DTERRNO_FMT,
      getnid(), NAME_ATTACH,
      DTERRNO_ARG(errno));
    goto cleanup;
  }
# endif

  umdec->priv          = priv;
  umdec->uninit        = umdec_vp_uninit;
  umdec->go            = umdec_vp_go;
  umdec->set_control   = umdec_vp_set_control;
  umdec->get_control   = umdec_vp_get_control;
  umdec->get_status    = umdec_vp_get_status;
  umdec->overlay_image = umdec_vp_overlay_image;
  umdec->overlay_text  = umdec_vp_overlay_text;
  umdec->overlay_clear = umdec_vp_overlay_clear;
  
  DT_C(umdec_vp_pci,(ctl, priv));

  rtdSetParams(SYS_PARAM,				/* go into PTS mode (required!)		*/
    USE_PTS, 0);

  sr = rtdInitSystem(0,					/* background mode, 0=polling mode 	*/
    NULL, NULL);
  if (sr < 0)
    VP_ERROR("rtdInitSystem");
  if (sr != 1)
  {
	rc = dt_err(ctl, F,
      "rtdInitSystem says there are"
      " %d VideoPlex boards",
      sr);
	goto cleanup;
  }
  priv->did_initsystem = 1;

  sr = rtdInitBoard(priv->board,		/* p 2-12 							*/
    NON_DMA_MODE,						/* non-DMA mode 					*/
    NTSC_M,								/* or PAL_M or NTSC_443				*/
    SYSTEM_FILE,						/* or VIDEO or AUDIO or TRANSPORT 	*/
    STAND_ALONE);						/* or genlock 						*/
  if (sr != 0)
    VP_ERROR("rtdInitBoard");
  priv->did_initboard = 1;

  priv->flag_back2back = !!strstr(		/* caller wants back2back mode? 	*/
    priv->settings, "back2back");

  dt_dbg(ctl, F, DT_DBG_MASK_CONFIG,
    "turning %s SEAMLESS_BACK2BACK",
    priv->flag_back2back? "on": "off");
  
  rtdSetParams(priv->board,
    SEAMLESS_BACK2BACK, 
    priv->flag_back2back);
										/* ................................ */

  DT_C(umdec_vp_status,(ctl, umdec));

cleanup:
  if (rc != DT_RC_GOOD)
	DT_I(umdec_vp_release,(ctl, priv));

  return rc;
# else
  return dt_err_notlinked(ctl, F, DESC);  
# endif
}

/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
umdec_vp_rev(							/* deliver revision 				*/
  dt_ctl_t *ctl,						/* environment control 				*/
  const char **rev_string,				/* returned rev string 				*/
  int *rev_major,						/* returned rev major 				*/
  int *rev_minor,						/* returned rev minor 				*/
  long *rev_date)						/* returned rev date 				*/
{
  if (rev_string != NULL)
    *rev_string = "umdec_vp lib";
  if (rev_major != NULL)
    *rev_major = 1;
  if (rev_minor != NULL)
    *rev_minor = 4;
  if (rev_date != NULL)
    *rev_date = 19970307;
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
