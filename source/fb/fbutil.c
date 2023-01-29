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
DT_RCSID("source/fb $RCSfile: fbutil.c,v $ $Revision: 1.7 $");

/*..........................................................................
 * copy BI8 from frame buffer to memory, reverse rows
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtfb_util_bi8(
  dt_ctl_t *ctl,
  unsigned char *p,
  unsigned char *d,
  const int xe,
  const int ye)
{
  int y;
  d += (ye-1) * xe;						/* point down to last row		    */
  for (y=0; y<ye; y++)
  {
	DT_MEMCPY(d, p, xe);
	d -= xe;
	p += xe;
  }
  return DT_RC_GOOD;
}

/*..........................................................................
 * copy BI24 BGR888 frame buffer to RGB888 memory, reverse rows
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtfb_util_bi24_rgb(
  dt_ctl_t *ctl,
  unsigned char *p,
  unsigned char *d,
  const int xe,
  const int ye)
{
  const int xe3 = xe * 3;
  int x, y;
  d += (ye-1) * xe3;					/* point down to last row		    */
  for (y=0; y<ye; y++)
  {
	for (x=0; x<xe3; x+=3)
	{
	  d[x+0] = p[x+2];					/* reverse BGR 						*/
	  d[x+1] = p[x+1];
	  d[x+2] = p[x+0];
	}
	d -= xe3;
	p += xe3;
  }
  return DT_RC_GOOD;
}

/*..........................................................................
 * copy BGR888 frame buffer to RGB888 memory
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtfb_util_bgr_rgb(
  dt_ctl_t *ctl,
  const unsigned char *p,
  unsigned char *d,
  const int xe,
  const int ye,
  int skip,
  int dup)
{
  const int xe3 = xe * 3;
  int x, y, i;
  for (y=0; y<ye; y++)
  {
	for (x=0; x<xe3; x+=3)
	{
	  d[x+0] = p[x+2];					/* reverse BGR 						*/
	  d[x+1] = p[x+1];
	  d[x+2] = p[x+0];
	}
	d += xe3;
	p += xe3;
	p += skip * xe3;					/* skip input lines 				*/
	for (i=0; i<dup; i++)				/* dup previous line in output 		*/
	{  
	  DT_MEMCPY(d, d-xe3, xe3);
	  d += xe3;
	}
  }
  return DT_RC_GOOD;
}

/*..........................................................................
 * copy BI16 RGB555 from frame buffer to RGB888 memory, reverse rows
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtfb_util_bi16_rgb(
  dt_ctl_t *ctl,
  unsigned char *p,
  unsigned char *d,
  const int xe,
  const int ye)
{
  const unsigned short *q =
    (unsigned short *)p;
  const int xe3 = xe * 3;
  int x, y;
  d += (ye-1) * xe3;					/* point down to last row		    */
  for (y=0; y<ye; y++)
  {
	for (x=0; x<xe3; x+=3)
	{									/* xRRR RRGG GGGB BBBB 				*/
	  const int v = *q++;
	  d[x+0] = (v >> 7) & 0xf8;			/* R								*/
	  d[x+1] = (v >> 2) & 0xf8;			/* G 								*/
	  d[x+2] = (v << 3);				/* B 								*/
	}
	d -= xe3;
  }
  return DT_RC_GOOD;
}

/*..........................................................................
 * copy BI24 BGR888 frame buffer to 8-bit green memory, reverse rows
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtfb_util_bi24_g(
  dt_ctl_t *ctl,
  unsigned char *p,
  unsigned char *d,
  const int xe,
  const int ye)
{
  const int xe3 = xe * 3;
  int x, y;
  d += (ye-1) * xe;						/* point down to last row		    */
  p++;									/* offset to green in triplet 		*/
  for (y=0; y<ye; y++)
  {
	for (x=0; x<xe3; x+=3)
      *d++ = p[x];
	d -= xe + xe;
	p += xe3;
  }
  return DT_RC_GOOD;
}

/*..........................................................................
 * copy BI16 RGB555 from frame buffer to 8-bit green memory, reverse rows
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtfb_util_bi16_g(
  dt_ctl_t *ctl,
  unsigned char *p,
  unsigned char *d,
  const int xe,
  const int ye)
{
  const unsigned short *q =
    (unsigned short *)p;
  int x, y;
  d += (ye-1) * xe;						/* point down to last row		    */
  for (y=0; y<ye; y++)
  {
	for (x=0; x<xe; x++)
	  d[x] = (*q++ >> 2) & 0xf8;		/* G in xRRR RRGG GGGB BBBB			*/
	d -= xe;
  }
  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtfb_util_grb0888_rgb888(
  dt_ctl_t *ctl,
  const unsigned char * const p,
  unsigned char * const d,
  const int xe,
  const int ye,
  const unsigned char *rlookup,
  const unsigned char *glookup,
  const unsigned char *blookup,
  int skip,
  int dup)
{
  int x, y;
  int xp = 0;
  int xd = 0;
  int i;
  for (y=0; y<ye; y+=dup+1)
  {
	for (x=0; x<xe; x++)
	{
	  d[xd+0] = rlookup[p[xp+2]];
	  d[xd+1] = glookup[p[xp+1]];
	  d[xd+2] = blookup[p[xp+0]];
	  xp += 4;
	  xd += 3;
	}
	xp += skip * xe * 4;				/* skip input lines 				*/
	for (i=0; i<dup; i++)				/* dup previous line in output 		*/
	{  
	  DT_MEMCPY(d+xd, d+xd-xe*3, xe*3);
	  xd += xe * 3;
	}
  }
  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtfb_util_grb0888_bi24(
  dt_ctl_t *ctl,
  const unsigned char *p,
  unsigned char * const d,
  const int xe,
  const int ye)
{
  int x, y;
  const int xe4 = xe * 4;
  int xd = 0;
  p += (ye-1) * xe4;					/* point down to last row		    */
  for (y=0; y<ye; y++)
  {
	int xp = 0;
	for (x=0; x<xe; x++)
	{
	  d[xd+0] = p[xp+3];
	  d[xd+1] = p[xp+1];
	  d[xd+2] = p[xp+2];
	  xp += 4;
	  xd += 3;
	}
	p -= xe4;
  }

  return DT_RC_GOOD;
}

/*..........................................................................
 * write raw bytes							
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtfb_util_write_raw(
  dt_ctl_t *ctl,
  void *data,
  long size,
  const char *filename)
{
  DT_F("dtfb_util_write_raw");
  dtfd_t _fd, *fd = &_fd;
  dt_rc_e rc;
  DT_Q(dtfd_open,(ctl, fd, filename, "w"));
  DT_G(dtfd_write_all,(ctl, fd, data, size));
  DT_I(dtfd_close,(ctl, fd));
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
