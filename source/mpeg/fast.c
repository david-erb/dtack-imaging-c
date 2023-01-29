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
| ERRORS
| As yet undocumented.
| 
| END
 *..........................................................................*/

#include <dtack/base.h>
DT_RCSID("source/mpeg $RCSfile: fast.c,v $ $Revision: 1.8 $");
#include <umdec.h>

#include <dtack/lut8.h>
#include <dtack/list.h>
#include <dtack/svc.h>
#include <dtack/scr.h>
#include <dtack/color.h>
#include <umdecscr.h>

/*..........................................................................
 *..........................................................................*/

dt_rc_e 
DTCONFIG_API1
umdec_scr_fast_lookup(
  dt_ctl_t *ctl,
  dtimage_t *input,
  dt_ntype_b08_t *table,
  dtimage_t *output)
{
  const int n = input->xe * input->ye;
  dt_ntype_b08_t * const in = input->row[0].b08;
  dt_ntype_b08_t * const out = output->row[0].b08;
  const dt_ntype_b08_t * const t = table;
  int i;
  for (i=0; i<n; i++)
	out[i] = t[in[i]];
  return DT_RC_GOOD;
}

/*..........................................................................
 * fast pixel decimation
 * x and y are size of source which is assumed exactly 2x destination
 * works also in-place (s==d)
 * decimated result is left in upper left of image
 * image is assumed 2n x 2n
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
umdec_scr_fast_decimate(
  dt_ctl_t *ctl,
  unsigned char *s,
  dtxy_t xe,
  dtxy_t ye,
  unsigned char *d)
{
  dtxy_t x, y;

  for (y=0; y<ye/2; y++)
  {
    unsigned char * const p = d + y * xe / 2;
    unsigned char * const q = s + y * 2 * xe;
	for (x=0; x<(xe>>1); x++)
	  p[x] = q[x<<1];
  }
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
