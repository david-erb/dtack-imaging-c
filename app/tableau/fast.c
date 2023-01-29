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
| main program
| 
| END
 *..........................................................................*/

#include <tableau.h>

DT_RCSID("app/tableau $RCSfile: fast.c,v $ $Revision: 1.8 $");

/*..........................................................................
 * fast pixel decimation
 * x and y are size of source which is assumed exactly 2x destination
 * works also in-place (s==d)
 * decimated result is left in upper left of image
 * image is assumed 2n x 2n
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
tableau_fast_decimate(
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

/*..........................................................................
 * fast in-place pixel duplication
 * source is assumed in upper left of image
 * image is assumed 2n x 2n
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
tableau_fast_pixdup(
  dt_ctl_t *ctl,
  dtimage_t *image)
{
  const dtxy_t ye = image->ye;
  const dtxy_t xe = image->xe;
  dtxy_t x, y;

  for (y=ye-1; y>=0; y-=2)
  {
    dt_ntype_b08_t * const p   = image->row[y/2].b08;
	dt_ntype_b08_t * const qc1 = image->row[y].b08;
	dt_ntype_b08_t * const qc2 = image->row[y-1].b08;
	for (x=xe-1; x>=0; x-=2)
	{
	  qc1[x] = qc1[x-1] =
	  qc2[x] = qc2[x-1] = p[x>>1];
	}
  }
  return DT_RC_GOOD;
}

/*..........................................................................
 * fast in-place pixel decimation
 * decimated result is left in upper left of image
 * image is assumed 2n x 2n
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
tableau_fast_pixdec(
  dt_ctl_t *ctl,
  dtimage_t *image)
{
  const dtxy_t ye = image->ye;
  const dtxy_t xe = image->xe;
  dtxy_t x, y;

  for (y=0; y<ye/2; y++)
  {
    dt_ntype_b08_t * const p = image->row[y].b08;
	dt_ntype_b08_t * const q = image->row[y*2].b08;
	for (x=0; x<(xe>>1); x++)
	  p[x] = q[x<<1];
  }
  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
tableau_fast_pixrep(
  dt_ctl_t *ctl,
  dtimage_t *input,
  int factor,
  dtimage_t *output)
{
  const dtxy_t ye = input->ye;
  const dtxy_t xe = input->xe;
  dtxy_t x, y;

  for (y=0; y<ye; y++)					/* traverse output image		    */
  {
    const dt_ntype_b08_t * const p =
      input->row[y].b08;
	dt_ntype_b08_t * const qc1 = 
      output->row[y*2].b08;
	dt_ntype_b08_t * const qc2 =
      output->row[y*2+1].b08;
	dtxy_t x2 = 0;
	for (x=0; x<xe; x++)
	{
	  qc1[x2] = qc1[x2+1] =
	  qc2[x2] = qc2[x2+1] = p[x];
	  x2 += 2;
	}
  }
  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/

dt_rc_e 
DTCONFIG_API1
tableau_fast_lookup(
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
 *..........................................................................*/

dt_rc_e 
DTCONFIG_API1
tableau_fast_separate(
  dt_ctl_t *ctl,
  void *input,
  int xe,
  int ye,
  void *output1,
  void *output2,
  void *output3)
{
  const int n = xe * ye;
  dt_ntype_b08_t *in = (dt_ntype_b08_t *)input;
  dt_ntype_b08_t *out1 = (dt_ntype_b08_t *)output1;
  dt_ntype_b08_t *out2 = (dt_ntype_b08_t *)output2;
  dt_ntype_b08_t *out3 = (dt_ntype_b08_t *)output3;
  int i;
  for (i=0; i<n; i++)
  {
	*out1++ = *in++;
	*out2++ = *in++;
	*out3++ = *in++;
  }
  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/

dt_rc_e 
DTCONFIG_API1
tableau_fast_clear(
  dt_ctl_t *ctl,
  dtimage_t *image)
{
  const dtxy_t xe = image-> xe;
  const dtxy_t ye = image->ye;
  dtxy_t y;

  for (y=0; y<ye; y++)
    memset(image->row[y].b08, 0, xe);

  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/

dt_rc_e 
DTCONFIG_API1
tableau_fast_diff(
  dt_ctl_t *ctl,
  dtimage_t *input1,
  dtimage_t *input2,
  int eps,
  dtimage_t *output)
{
  const int n = DT_MIN(DT_MIN(input1->xe, input2->xe), output->xe);
  dtxy_t ymax = DT_MIN(DT_MIN(input1->ye, input2->ye), output->ye);
  dtxy_t y;

  for (y=0; y<ymax; y++)
  {
	dt_ntype_b08_t * const in1 = input1->row[y].b08;
	dt_ntype_b08_t * const in2 = input2->row[y].b08;
	dt_ntype_b08_t * const out = output->row[y].b08;
#   define SHIFT (1)
	const int e = eps >> SHIFT;
	int i;
	for (i=0; i<n; i++)
	{
	  const int d = ((int)in1[i] - (int)in2[i]) >> SHIFT;
	  if (d < -128)
	    out[i] = 0;
	  else
	  if (d > 127)
	    out[i] = 255;
	  else
	  if (d < e && d > -e)
	    out[i] = 128;
	  else
	    out[i] = d + 128;
	}
  }

  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/

dt_rc_e 
DTCONFIG_API1
tableau_fast_add(
  dt_ctl_t *ctl,
  dtimage_t *input1,
  dtimage_t *input2)
{
  const dtxy_t xe = input1->xe;
  const dtxy_t ye = input1->ye;
  dtxy_t x, y;

  for (y=0; y<ye; y++)					/* traverse output image		    */
  {
    dt_ntype_b08_t * const p =
      input1->row[y].b08;
	dt_ntype_b08_t * const q = 
      input2->row[y].b08;
	for (x=0; x<xe; x++)
	{
	  const int s = (int)p[x] + (((int)q[x] - 128) << SHIFT);
	  if (s > 255)
	    p[x] = 255;
	  else
	  if (s < 0)
	    p[x] = 0;
	  else
	    p[x] = s;
	}
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
