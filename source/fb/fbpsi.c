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
DT_RCSID("source/fb $RCSfile: fbpsi.c,v $ $Revision: 1.7 $");
#include <dtack/lut8.h>
#include <dtack/color.h>
#include <dtack/fb.h>
#include <dtack/os.h>
#include <dtack/str.h>
#include <dtack/prj.h>
#include <dtack/phys.h>

#define DESC "psi2 capture card"

extern dtfb_init_f dtfb_psi_init;

#if DTPRJ_VMC == DT_1

#include <signal.h>

const int dtfb_psi_dummy = 0;

static dtfb_uninit_f dtfb_psi_uninit;
static dtfb_control_f dtfb_psi_control;
static dtfb_putblock_f dtfb_psi_putblock;
static dtfb_getblock_f dtfb_psi_getblock;
static dtfb_putcolor_f dtfb_psi_putcolor;
static dtfb_getcolor_f dtfb_psi_getcolor;
static dtfb_clear_f dtfb_psi_clear;
static dtfb_cursor_f dtfb_psi_cursor;

typedef struct {
  char *ioaddr_s;
  char *memaddr_s;
  char *mode_s;
  long pagesize;
  long memaddr;
  dtphys_t phys;
  int ioaddr;
  pid_t pid;
  pid_t proxy;
  int irqnum;
  int irqid;
  int irqcount;							/* running irq count 				*/
  int kicked;
  int kickme;
  int alarm;
  int xe;
  int ye;
} dtfb_psi_priv_t;

#ifdef DTCONFIG_IMPLIED_QNX432
#  include "psi.h"
#  include <sys/kernel.h>
#  include <sys/name.h>
#  include <sys/inline.h>
   void myoutb(int port, unsigned char val);
#  pragma aux myoutb = "out  dx,al"\
        parm [edx] [eax] modify exact nomemory [];
#else
  void outb(int port, int val);
  int inb(int port);
#endif

/*--------------------------------------------------------------------------*
 *--------------------------------------------------------------------------*/
#define F "psi_out"

static
dt_rc_e 
psi_out(								/* output to psi ioaddr			*/
  dt_ctl_t *ctl,			 
  dtfb_psi_priv_t *psi,
  int reg,
  unsigned char val)
{
  if (psi->ioaddr)
  {
    myoutb(psi->ioaddr, reg);
    myoutb(psi->ioaddr+1, val);
    if (DT_DBG(ctl, DT_DBG_MASK_PIXEL))
      dt_dbg(ctl, F, DT_DBG_MASK_PIXEL,
        "out %02x 0x%02x (%d)",
        reg, val, val);
  }
  return DT_RC_GOOD;
}
#undef F

/*--------------------------------------------------------------------------*
 *--------------------------------------------------------------------------*/
#define F "psi_in"

static
dt_rc_e 
psi_in(									/* input from psi port			*/
  dt_ctl_t *ctl,			
  dtfb_psi_priv_t *psi,
  int reg,
  unsigned char *val)
{
  if (psi->ioaddr)
  {
    myoutb(psi->ioaddr, reg);
    *val = inb(psi->ioaddr+1);
  }
  else
    *val = 0;
  return DT_RC_GOOD;
}
#undef F


/*--------------------------------------------------------------------------*
 *--------------------------------------------------------------------------*/
#define F "psi_send"

static
dt_rc_e 
psi_send(
  dt_ctl_t *ctl,			  
  dtfb_psi_priv_t *priv,
  VMC2_MSG *message)
{
  int sysrc;
  sysrc = Send(priv->pid, message,  message, 
    sizeof(*message), 
    sizeof(*message));

  if (sysrc == -1)
    return dt_err(ctl, F,  
      "errno %d in Send",
      errno);

  if (message->nResultCode != 1)
    return dt_err(ctl, F,
      "vmc driver gave bad result code %d",
      message->nResultCode);

  return DT_RC_GOOD;
}
#undef F

/*--------------------------------------------------------------------------*
 *--------------------------------------------------------------------------*/

#pragma off (check_stack);

int global_proxy = 0;
long global_count = 0;

dtfb_psi_priv_t *global_priv = NULL;

static
pid_t far
psi_handler(void)
{
  pid_t proxy = 0;
  if (global_priv != NULL)
  {
	int t;
    myoutb(global_priv->ioaddr, 0x09);	/* acknowledge interrupt 			*/
    t = inb(global_priv->ioaddr+1);
	global_priv->irqcount++;

	if (global_priv->kickme)
	{
	  global_priv->kicked++;
	  proxy = global_priv->proxy;
	}
  }
  return proxy;
}

#pragma on (check_stack);

/*--------------------------------------------------------------------------*
 *--------------------------------------------------------------------------*/
#define F "psi_open"

static
dt_rc_e 
psi_open(				
  dt_ctl_t *ctl,			  
  dtfb_psi_priv_t *priv)
{
  VMC2_MSG message;
  
  priv->pid = qnx_name_locate(0, 
    VMC2_DRIVER_NAME, 
    sizeof(VMC2_MSG), NULL);
  if (priv->pid == (pid_t **)(-1))
	return dt_err(ctl, F,
      "could not locate driver"
      " registered name \"%s\"",
      VMC2_DRIVER_NAME);

  DT_MEMSET(&message, 0, sizeof(message));	
  message.nMsgType = _MTMoveVideo;
  message.nDataValue = VMC2_READ_VALUE;

#ifdef NONO
  message.stArea.nWidth = priv->xe;
  message.stArea.nHeight = priv->ye;
#endif

  DT_Q(psi_send,(ctl, priv, &message));

  priv->xe = message.stArea.nWidth;
  priv->ye = message.stArea.nHeight;

  if (priv->xe & 1 ||
	  priv->ye & 1)
    return dt_err(ctl, F,
      "bad size %dx%d",
      priv->xe, priv->ye);

  priv->proxy = qnx_proxy_attach(		/* get a proxy						*/
    0, 0, 0, 0);
  if (priv->proxy == (pid_t)(-1))
    return dt_err(ctl, F, 
      "qnx_proxy_attach(0, 0, 0, 0)"
      " errno %d (%s)",
      errno, strerror(errno));

  priv->irqid = qnx_hint_attach(		/* attach interrupt handler			*/
    priv->irqnum,	
    psi_handler, 
    FP_SEG(&global_priv));
  if (priv->irqid == -1)
    return dt_err(ctl, F, 
      "qnx_hint_attach(%d)"
      " errno %d (%s)",
      priv->irqnum, 
      errno, strerror(errno));

  return DT_RC_GOOD;
}
#undef F

/*--------------------------------------------------------------------------*
 *--------------------------------------------------------------------------*/
#define F "psi_reset"

static
dt_rc_e 
psi_reset(								/* reset hardware to initial state	*/
  dt_ctl_t *ctl,				
  dtfb_psi_priv_t *priv,
  dt_color_org_t *org)
{
  VMC2_MSG message;
  unsigned char val;

  myoutb(priv->ioaddr, 0x09);			/* disable interrupts				*/
  myoutb(priv->ioaddr+1, 0x00);

  DT_MEMSET(&message, 0, sizeof(message));	
  message.nMsgType = _MTLoadConfig;
  strcpy(message.szPath, priv->mode_s);
  DT_Q(psi_send,(ctl, priv, &message));

  myoutb(priv->ioaddr, 0x21);			/* get current capture control 		*/
  val = inb(priv->ioaddr+1);

  if (org->org == DT_COLOR_ORG_RGB565 ||
	  org->org == DT_COLOR_ORG_RGB555)
	val |= 0x10;						/* turn on the rgb bit 				*/
  else
    val &= ~0x10;						/* turn off the rgb bit 			*/

  myoutb(priv->ioaddr, 0x21);
  myoutb(priv->ioaddr+1, val);

  return DT_RC_GOOD;
}

#undef F

/*--------------------------------------------------------------------------*
 * freeze at end of odd field
 * makes sure something is aquired if requested
 *--------------------------------------------------------------------------*/
#define F "psi_freeze"

static void alarm_handler(int signum) { global_priv->alarm = 1; }

static
dt_rc_e 
psi_freeze(								/* freeze and view memory		  	*/
  dt_ctl_t *ctl,				
  dtfb_psi_priv_t *priv)
{
  int sysrc;
  int status20;
  int received = 0;
# define MINR (2)
  const int dbg = DT_DBG_MASK_OBJECT;
  void (*old_alarm_handler)(int);
  u_int old_alarm;
  
  if (DT_DBG(ctl, dbg))
    dt_dbg(ctl, F, dbg,
      "waiting for interrupt kicked=%d, irqcount=%d",
      priv->kicked, priv->irqcount);

  old_alarm = alarm(0);					/* cancel existing alarm 			*/
  old_alarm_handler = signal(SIGALRM,	/* set alarm signal handler 		*/
    alarm_handler);

  myoutb(priv->ioaddr, 0x20);			/* status now						*/
  status20 = inb(priv->ioaddr+1);
  while (status20 & 1 ||				/* still acquiring? 				*/
		 received < MINR ||				/* or some kicks are needed? 		*/
		 priv->kicked > received)		/* or some kicks are pending? 		*/
  {
	priv->alarm = 0;
	alarm(1);							/* one second timeout 				*/
	sysrc = Receive(priv->proxy,		/* wait for the kick				*/
      0, NULL);
	alarm(0);							/* cancel alarm timeout 			*/
	if (sysrc == (pid_t)(-1))
	{
	  int status09;
      dt_err(ctl, F,
        "Receive(proxy) errno %d (%s)",
        errno, strerror(errno));

	  myoutb(priv->ioaddr, 0x09);	
	  status09 = inb(priv->ioaddr+1);
      dt_err(ctl, F,
        "status20=0x%02x, status09=0x%02x"
        " received=%d, kicked=%d, kickme=%d"
        " irqcount=%d",
        status20, status09, received, 
        priv->kicked, priv->kickme,
        priv->irqcount);
	  if (errno != EINTR)
	    return DT_RC_BAD;
	  else
	    continue;
	}
	
	received++;

	myoutb(priv->ioaddr, 0x20);			/* status now						*/
	status20 = inb(priv->ioaddr+1);

	if (received >= MINR &&				/* need no more kicks? 				*/
		!(status20 & 1))				/* acquisition is finished? 		*/
	  priv->kickme = 0;					/* quit kicking 					*/
  }

  signal(SIGALRM, old_alarm_handler);	/* replace old signal handler 		*/
  alarm(old_alarm);						/* reinstate old alarm timeout 		*/

  priv->kicked = 0;

  if (DT_DBG(ctl, dbg))
    dt_dbg(ctl, F, dbg,
      "got interrupt"
      " status =0x%02x,"
      " received=%d, priv->kicked=%d,"
      " priv->irqcount=%d",
      status20, received, 
      priv->kicked, priv->irqcount);

  return DT_RC_GOOD;
}

#undef F

/*--------------------------------------------------------------------------*
 *--------------------------------------------------------------------------*/
#define F "psi_acquire"

static
dt_rc_e 
psi_acquire(							/* start acquiring					*/
  dt_ctl_t *ctl,				
  dtfb_psi_priv_t *priv)
{
#ifdef NONO
  VMC2_MSG message;
  DT_MEMSET(&message, 0, sizeof(message));	
  message.nMsgType = _MTLiveVideo;
  DT_Q(psi_send,(ctl, priv, &message));
#endif

  myoutb(priv->ioaddr, 0x09);			/* clear pending interrupts			*/
  inb(priv->ioaddr+1);

  myoutb(priv->ioaddr, 0x09);			/* enable interrupts				*/
  myoutb(priv->ioaddr+1, 0x02);

  priv->kickme = 1;						/* start kicking 					*/

  myoutb(priv->ioaddr, 0x20);			/* acquire single frame 			*/
  myoutb(priv->ioaddr+1, 0x23);

  return DT_RC_GOOD;
}

#undef F

/*--------------------------------------------------------------------------*
 *--------------------------------------------------------------------------*/
#define F "psi_snapshot"

static
dt_rc_e 
psi_snapshot(							/* freeze one new frame				*/
  dt_ctl_t *ctl,				  
  dtfb_psi_priv_t *priv)
{
  DT_Q(psi_acquire,(ctl, priv));		/* start acquiring 					*/

  DT_Q(psi_freeze,(ctl, priv));			/* quit acquiring					*/

  return DT_RC_GOOD;
}

#undef F

/*--------------------------------------------------------------------------*
 *--------------------------------------------------------------------------*/
#define F "psi_regdump1"

static
dt_rc_e 
psi_regdump(							/* print current hardware state		*/
  dt_ctl_t *ctl,				
  dtfb_psi_priv_t *psi,
  int reg,
  const char *comment)
{
  unsigned char val;
  DT_Q(psi_in,(ctl, psi, reg, &val));
  dt_dbg(ctl, NULL, DT_DBG_MASK_OBJECT,
    "%2x = 0x%02x %s", reg, val, comment);
  return DT_RC_GOOD;
}
#undef F

/*--------------------------------------------------------------------------*
 *--------------------------------------------------------------------------*/
#define F "psi_regdump2"

static
dt_rc_e 
psi_regdump2(							/* print current hardware state		*/
  dt_ctl_t *ctl,				
  dtfb_psi_priv_t *psi,
  int reg,
  const char *comment)
{
  unsigned char val1, val2;
  DT_Q(psi_in,(ctl, psi, reg, &val1));
  DT_Q(psi_in,(ctl, psi, reg+1, &val2));
  dt_dbg(ctl, NULL, DT_DBG_MASK_OBJECT,
    "%2x = 0x%04lx %s", reg, 
    val2*0x100L + val1, comment);
  return DT_RC_GOOD;
}
#undef F

/*--------------------------------------------------------------------------*
 *--------------------------------------------------------------------------*/
#define F "psi_regdump3"

static
dt_rc_e 
psi_regdump3(							/* print current hardware state		*/
  dt_ctl_t *ctl,				
  dtfb_psi_priv_t *psi,
  int reg,
  const char *comment)
{
  unsigned char val1, val2, val3;
  DT_Q(psi_in,(ctl, psi, reg, &val1));
  DT_Q(psi_in,(ctl, psi, reg+1, &val2));
  DT_Q(psi_in,(ctl, psi, reg+2, &val3));
  dt_dbg(ctl, NULL, DT_DBG_MASK_OBJECT,
    "%2x = 0x%08lx %s", reg, 
    val3*0x1000L + val2*0x100L + val1,
    comment);
  return DT_RC_GOOD;
}
#undef F

/*--------------------------------------------------------------------------*
 *--------------------------------------------------------------------------*/
#define F "psi_regdump"

static
dt_rc_e 
psi_regdump_all(						/* print current hardware state		*/
  dt_ctl_t *ctl,				
  dtfb_psi_priv_t *psi)
{
  DT_Q(psi_regdump,(ctl, psi, 0xff, "global enable register"));

  DT_Q(psi_regdump,(ctl, psi, 0x00, "io address signature"));
  DT_Q(psi_regdump,(ctl, psi, 0x01, "disable vram write mask"));
  
  DT_Q(psi_regdump,(ctl, psi, 0x82, "enable page mode"));
  DT_Q(psi_regdump,(ctl, psi, 0x06, "map vram to isa megabyte"));
  DT_Q(psi_regdump,(ctl, psi, 0x81, "memory page base address"));

  DT_Q(psi_regdump,(ctl, psi, 0x07, "enable all bitplane capture"));
  DT_Q(psi_regdump,(ctl, psi, 0x08, "enable all bitplane capture"));
  
  DT_Q(psi_regdump,(ctl, psi, 0x09, "status"));
  
  DT_Q(psi_regdump,(ctl, psi, 0x20, "freeze"));
  
  DT_Q(psi_regdump,(ctl, psi, 0x21, "capture control"));

  DT_Q(psi_regdump2,(ctl, psi, 0x22, "x start"));

  DT_Q(psi_regdump2,(ctl, psi, 0x24, "y start"));

  DT_Q(psi_regdump2,(ctl, psi, 0x26, "x end"));

  DT_Q(psi_regdump2,(ctl, psi, 0x28, "y end"));

  DT_Q(psi_regdump3,(ctl, psi, 0x2a, "vram capture address"));

  DT_Q(psi_regdump,(ctl, psi, 0x2d, "normal x scaling"));
  DT_Q(psi_regdump,(ctl, psi, 0x2e, "normal y scaling"));
  
  DT_Q(psi_regdump,(ctl, psi, 0x30, "input video start line"));
  
  DT_Q(psi_regdump,(ctl, psi, 0x38, "scaling control"));

  DT_Q(psi_regdump,(ctl, psi, 0x80, "extended control"));
  DT_Q(psi_regdump,(ctl, psi, 0x88, "extended interrupt"));

  DT_Q(psi_regdump,(ctl, psi, 0x8a, "enhanced horizontal scaling"));
  DT_Q(psi_regdump,(ctl, psi, 0x8b, "misc 1"));
  DT_Q(psi_regdump,(ctl, psi, 0x8c, "enhanced vertical scaling"));
  DT_Q(psi_regdump,(ctl, psi, 0x8d, "misc 2"));

  return DT_RC_GOOD;
}

#undef F

/*--------------------------------------------------------------------------*
 *--------------------------------------------------------------------------*/
#define F "psi_query"

static
dt_rc_e 
psi_query(
  dt_ctl_t *ctl,
  dtfb_psi_priv_t *priv,
  int query, 
  int *value)
{
  VMC2_MSG message;
  DT_MEMSET(&message, 0, sizeof(message));	
  message.nMsgType = query;
  message.nDataValue = VMC2_READ_VALUE;
  DT_Q(psi_send,(ctl, priv, &message));
  *value = message.nDataValue;
  return DT_RC_GOOD;
}
#undef F

/*--------------------------------------------------------------------------*
 *--------------------------------------------------------------------------*/
#define F "psi_report1"

static
dt_rc_e 
psi_report(								/* print current hardware state		*/
  dt_ctl_t *ctl,				
  dtfb_psi_priv_t *psi,
  int value,
  const char *comment)
{
  dt_dbg(ctl, NULL, DT_DBG_MASK_OBJECT,
    "%-20s = %4d", comment, value);
  return DT_RC_GOOD;
}
#undef F

/*--------------------------------------------------------------------------*
 *--------------------------------------------------------------------------*/
#define F "psi_report"

static
dt_rc_e 
psi_report_all(								/* print current hardware state		*/
  dt_ctl_t *ctl,				
  dtfb_psi_priv_t *priv)
{
  VMC2_MSG message;
  int value;

  DT_Q(psi_query,(ctl, priv, _MTPanOff, &value));
  DT_Q(psi_report,(ctl, priv, value, "_MTPanOff"));

  DT_Q(psi_query,(ctl, priv, _MTShiftXOff, &value));
  DT_Q(psi_report,(ctl, priv, value, "_MTShiftXOff"));

  DT_Q(psi_query,(ctl, priv, _MTShiftYOff, &value));
  DT_Q(psi_report,(ctl, priv, value, "_MTShiftYOff"));

  DT_Q(psi_query,(ctl, priv, _MTChannel,  &value));
  DT_Q(psi_report,(ctl, priv, value, "_MTChannel"));

  DT_MEMSET(&message, 0, sizeof(message));	
  message.nMsgType = _MTMoveVideo;
  message.nDataValue = VMC2_READ_VALUE;
  DT_Q(psi_send,(ctl, priv, &message));

  DT_Q(psi_report,(ctl, priv, message.stArea.nX, "stArea.nX"));
  DT_Q(psi_report,(ctl, priv, message.stArea.nY, "stArea.nY"));
  DT_Q(psi_report,(ctl, priv, message.stArea.nWidth, "stArea.nWidth"));
  DT_Q(psi_report,(ctl, priv, message.stArea.nHeight, "stArea.nHeight"));


  return DT_RC_GOOD;
}

#undef F

/*..........................................................................
 *..........................................................................*/

static
dt_rc_e
dtfb_psi_control(
  dt_ctl_t *ctl,
  dtfb_t *fb,
  dtfb_control_e control)
{
  DT_F("dtfb_psi_control");
  dtfb_psi_priv_t *priv = fb->priv;

  DTCSTACK_HIGHWATER;

  switch (control)
  {
	case DTFB_CONTROL_RESET:
      DT_Q(psi_reset,(ctl, priv, &fb->org));
      DT_Q(psi_report_all,(ctl, priv));
      DT_Q(psi_regdump_all,(ctl, priv));
	break;
	case DTFB_CONTROL_SNAPSHOT:
      DT_Q(psi_snapshot,(ctl, priv));
	break;
	case DTFB_CONTROL_FREEZE:
      DT_Q(psi_freeze,(ctl, priv));
	break;
	case DTFB_CONTROL_ACQUIRE:
      DT_Q(psi_acquire,(ctl, priv));
	break;
	default:
      return dt_err(ctl, F,
        "invalid control code 0x%04x",
        control);
  }
  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/

static
dt_rc_e
dtfb_psi_putblock(
  dt_ctl_t *ctl,
  dtfb_t *fb,
  void *data,
  int x0,
  int y0,
  int xe,
  int ye)
{
  DT_F("dtfb_psi_putblock");
  return DT_RC_GOOD;
}

/*..........................................................................
 * use the mode config string to change config file which has window size
 *..........................................................................*/

static
dt_rc_e
dtfb_psi_getblock(
  dt_ctl_t *ctl,
  dtfb_t *fb,
  void *data,
  int x0,
  int y0,
  int xe,
  int ye)
{
  DT_F("dtfb_psi_getblock");
  dtfb_psi_priv_t *priv =
    fb->priv;
  int w = xe;
  int y;
  short *p;
  char *d;
  unsigned char val;
  int last;

  DTCSTACK_HIGHWATER;

  DT_Q(psi_freeze,(ctl,					/* freeze next even field 			*/
    priv));

  DT_Q(dtfb_clip,(ctl,					/* do clipping					    */
    fb, data, x0, y0, xe, ye,
    &data, &x0, &y0, &w, &ye));

  if (xe < 0 || ye < 0)					/* picture entirely clipped out?    */
    return DT_RC_GOOD;

  DT_Q(psi_in,(ctl, priv, 0x82, &val));

  val &= 0xe0;

  d = (char *)data;

# define XE (1024)						/* line width is actually 2048	    */

  last = -1;
  for (y=y0; y<y0+ye; y++)				/* for each row in input data 		*/
  {
	int x;
	int page = y / 16;
	if (page != last)
	{
	  myoutb(priv->ioaddr, 0x82);		/* map vram page to physical 32K	*/
	  myoutb(priv->ioaddr+1, val|page);
	  last = page;
	  p = (short *)priv->phys.virtaddr + 
        (int)(y%16) * XE + x0;
	}

	if (fb->org.org ==					/* full image of Y only			    */
		DT_COLOR_ORG_LUMINANCE)
	{
  	  for (x=0; x<w; x++)				/* copy data from frame buffer 		*/
        d[x] = p[x];
	  d += w;
	}

#ifdef NONO
	if (fb->org.org &					/* full image of either Cr or Cb    */
		DT_COLOR_ORG_CR2 ||
		fb->org.org &
		DT_COLOR_ORG_CB2)
	{
	  const short * const q = p +
	    !(flags & DT_COLOR_ORG_CB2);
  	  for (x=0; x<w; x+=2)				/* copy data from frame buffer 		*/
        d[x] = d[x+1] = q[x] >> 8;
	  d += w;
	}
#endif

	if (fb->org.org ==					/* double-wide image YCRYCB			*/
		DT_COLOR_ORG_YCBYCR ||
		fb->org.org ==					/* double-wide image YUYV			*/
		DT_COLOR_ORG_YUYV)
	{
	  DT_MEMCPY(d, p, w * 2);				/* copy data from frame buffer 		*/
	  d += w * 2;
	}

	if (fb->org.org ==					/* double-wide image RGB 5:6:5		*/
		DT_COLOR_ORG_RGB565)
	{
	  DT_MEMCPY(d, p, w * 2);				/* copy data from frame buffer 		*/
	  d += w * 2;
	}

	p += XE;
  }

  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/

static
dt_rc_e
dtfb_psi_putcolor(
  dt_ctl_t *ctl,
  dtfb_t *fb,
  dtlut8_t *lut,
  int start,
  int count)
{
  DT_F("dtfb_psi_putcolor");
  struct {
    char r, g, b;
  } *rgb;
  int i;
  dt_rc_e rc;

  DTCSTACK_HIGHWATER;

  DT_C(dtos_malloc2,(ctl,				/* get temporary space for palette 	*/
    (void **)&rgb, 3*count,
    F, "palette"));

  for (i=0; i<count; i++)				/* convert lut to palette			*/
  {
    rgb[i].r = lut[i].r >> 2;
    rgb[i].g = lut[i].g >> 2;
    rgb[i].b = lut[i].b >> 2;
  }

cleanup:
  if (rgb != NULL)
    DT_I(dtos_free2,(ctl, rgb,
      F, "palette"));
  return rc;
}

/*..........................................................................
 *..........................................................................*/

static
dt_rc_e
dtfb_psi_getcolor(
  dt_ctl_t *ctl,
  dtfb_t *fb,
  dtlut8_t *lut,
  int start,
  int count)
{
  DT_F("dtfb_psi_getcolor");

  DTCSTACK_HIGHWATER;

  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/

static
dt_rc_e
dtfb_psi_clear(
  dt_ctl_t *ctl,
  dtfb_t *fb,
  int background)
{
  DT_F("dtfb_psi_clear");
  char b[512];
  int y;
  
  DTCSTACK_HIGHWATER;

  DT_MEMSET(b, background, fb->xe);		/* fill line buffer with background */
  
  for (y=0; y<fb->ye; y++)				/* propagate to all rows 			*/
	DT_Q(dtfb_psi_putblock,(ctl,
      fb, b, 0, y, fb->xe, 1));

  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/

static
dt_rc_e
dtfb_psi_cursor(
  dt_ctl_t *ctl,
  dtfb_t *fb,
  int *x,
  int *y,
  int op)
{
  DT_F("dtfb_psi_cursor");
  dtfb_psi_priv_t *priv =
    fb->priv;

  DTCSTACK_HIGHWATER;
  
  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/

static
dt_rc_e
dtfb_psi_uninit(
  dt_ctl_t *ctl,
  dtfb_t *fb)
{
  DT_F("dtfb_psi_uninit");
  dtfb_psi_priv_t *priv =
    fb->priv;
  dt_rc_e rc = DT_RC_GOOD;

  DTCSTACK_HIGHWATER;

  if (priv != NULL)
  {
	global_priv = NULL;

    if (priv->phys.physaddr != 0)		/* we accessed physical memory? 	*/
      DT_I(dtphys_close,(ctl,			/* release it 						*/
        &priv->phys));

	myoutb(priv->ioaddr, 0x09);			/* disable interrupts				*/
	myoutb(priv->ioaddr+1, 0x00);

	if (priv->irqid != 0)
      qnx_hint_detach(priv->irqid);
	if (priv->proxy != 0)
      qnx_proxy_detach(priv->proxy);

	if (priv->ioaddr_s != NULL)
      DT_I(dtos_free2,(ctl, 
        priv->ioaddr_s,
        F, "spec"));

    DT_I(dtos_free2,(ctl,				/* free private space 				*/
      fb->priv,
      F, "priv"));

  }

  return rc;
}

#endif

/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtfb_psi_init(
  dt_ctl_t *ctl,
  dtfb_t *fb,
  const char *spec)
{
  DT_F("dtfb_psi_init");
# if DTPRJ_VMC == DT_1
  char *ioaddr_s;
  char *memaddr_s;
  char *mode_s;
  long ioaddr;
  long memaddr;
  char *p;
  dtfb_psi_priv_t *priv = NULL;
  dt_rc_e rc;

  DTCSTACK_HIGHWATER;

  if (!dtprj_vmc)						/* called but not implemented?	    */
    return dt_err_notlinked(ctl,
      F, DESC);  

  if (spec == NULL)
  {
    rc = dt_err(ctl, F,
      "NULL psi spec");
    goto cleanup;
  }
  else
  if (strlen(spec) == 0)
  {
    rc = dt_err(ctl, F,
      "zero-length psi spec");
    goto cleanup;
  }

  DT_Q(dtos_strdup,(ctl, spec,			/* get space for spec edit 			*/
    &ioaddr_s));

  memaddr_s = strchr(ioaddr_s, ',');	/* where the memaddr string starts 	*/
  if (memaddr_s)
    *memaddr_s++ = '\0';
  else
    memaddr_s = "";

  mode_s = strchr(memaddr_s, ',');		/* where the mode string starts 	*/
  if (mode_s)
    *mode_s++ = '\0';
  else
    mode_s = "";

  DT_C(dtstr_to_long,(ctl,
    ioaddr_s, &ioaddr));

  DT_C(dtstr_to_long,(ctl,
    memaddr_s, &memaddr));

  DT_C(dtos_malloc2,(ctl,				/* space for private structure	    */
    (void **)&priv,
    sizeof(dtfb_psi_priv_t),
    F, "priv"));
  DT_MEMSET(priv, 0,						/* clear private structure 			*/
    sizeof(dtfb_psi_priv_t));

  priv->ioaddr_s = ioaddr_s;
  priv->memaddr_s = memaddr_s;
  priv->mode_s = mode_s;
  priv->ioaddr = (int)ioaddr;
  priv->memaddr = memaddr;
  priv->irqnum = 12;

  global_priv = priv;

  priv->pagesize = 0x8000;

  DT_C(dtphys_open,(ctl,				/* access physical memory 			*/
    &priv->phys,
    memaddr, priv->pagesize));

  priv->xe = 480;
  priv->ye = 360;

  DT_C(psi_open,(ctl, priv));

  fb->xe       = priv->xe;				/* remember fb dimensions	    	*/
  fb->ye       = priv->ye;
  fb->org.org  = DT_COLOR_ORG_YCBYCR;
  fb->rowsize  = 2 * priv->xe;			/* two bytes per pixel 				*/
  
  fb->priv     = priv;
  fb->uninit   = dtfb_psi_uninit;
  fb->control  = dtfb_psi_control;
  fb->putblock = dtfb_psi_putblock;
  fb->getblock = dtfb_psi_getblock;
  fb->putcolor = dtfb_psi_putcolor;
  fb->getcolor = dtfb_psi_getcolor;
  fb->cursor   = dtfb_psi_cursor;
  fb->clear    = dtfb_psi_clear;
  
cleanup:
  if (rc != DT_RC_GOOD)
  {
	if (priv != NULL &&
		priv->phys.virtaddr != NULL)	/* we accessed physical memory? 	*/
	  DT_I(dtphys_close,(ctl,			/* release it 						*/
        &priv->phys));

	if (priv != NULL)
	  DT_I(dtos_free2,(ctl, priv,
        F, "priv"));
  
    if (ioaddr_s != NULL)
      DT_I(dtos_free2,(ctl, ioaddr_s,
        F, "spec"));
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
