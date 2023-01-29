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
DT_RCSID("source/low $RCSfile: lut8_mat.c,v $ $Revision: 1.1 $");
#include <dtack/lut8.h>

/*..........................................................................
 * makes a lut to transform image to be like default colormap
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtlut8_tag(								/* set tags in lut					*/
  dt_ctl_t *ctl,
  dtlut8_t *lut)
{
  int i;
  for (i=0; i<256; i++)
    lut[i].i = (dtlut8_data_t)i;
  return DT_RC_GOOD;
}

/*..........................................................................
 * compare two color elements
 *..........................................................................*/

static
int
compare(								/* compare two color elements		*/
  void *e1,
  void *e2,
  unsigned int size)
{
  return DTLUT8_CMP(
    *((dtlut8_t *)e1),
    *((dtlut8_t *)e2));
}

/*..........................................................................
 * sort color array
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtlut8_sort(							/* sort color array					*/
  dt_ctl_t *ctl,
  dtlut8_t *lut)
{
  DT_F("dtlut8_sort");
  dt1d_rc_e rc;
  rc = dt1d_sort_struct(		
    lut, sizeof(*lut), 256,
    compare);
  if (rc != DT1D_RC_GOOD)
    return dt_err(ctl, F,
      "dt1d_sort_struct failed");      
  return DT_RC_GOOD;
}

/*..........................................................................
 * search sorted color array for given combined value
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtlut8_search(							/* search for color					*/
  dt_ctl_t *ctl,
  dtlut8_t *color,
  dtlut8_t *lut,
  int *idx1,
  int *idx2)
{
  int n, i, j;
  int match = 0;

  if (!DTLUT8_CMP(lut[0], *color))		/* binary search doesn't find 0	    */
  {
    i = 0;
    match = 1;
  }
  else
  {
	n = 128;							/* binary search				    */
	i = 128;
	do {
      const int sign = 
        DTLUT8_CMP(lut[i], *color);
	  n /= 2;
	  if (sign < 0)						/* search upper half?			    */
        i += n;
	  else								/* search lower half?			    */
	  if (sign > 0)
        i -= n;
	  else								/* got a hit?					    */
	  {
		match = 1;
		break;
	  }
	} while (n);						/* until narrowed down to 0		    */
  }

  if (match)							/* got a hit?					    */
  {
    for (j=i; j > 0; j--)				/* search left for duplicates	    */
      if (DTLUT8_CMP(lut[j-1], *color))
        break;
    *idx1 = j;
    for (j=i; j < 255; j++)				/* search right for duplicates	    */
      if (DTLUT8_CMP(lut[j+1], *color))
        break;
    *idx2 = j;
  }
  else
  {
    *idx1 = -1;
    *idx2 = -2;
  }
  return DT_RC_GOOD;
}

/*..........................................................................
 * search for Euclidean-closest color to given one
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtlut8_closest(							/* search for color					*/
  dt_ctl_t *ctl,
  dtlut8_t *color,
  dtlut8_t *lut,
  int *idx)
{
  DT_F("dtlut8_closest");
  int i;
  unsigned long d1, d2;

  d1 = 0;
  for (i=0; i<256; i++)
  {
    d2 = (unsigned long)(lut[i].r-color->r) *
         (unsigned long)(lut[i].r-color->r) + 
         (unsigned long)(lut[i].g-color->g) *
         (unsigned long)(lut[i].g-color->g) +
         (unsigned long)(lut[i].b-color->b) *
         (unsigned long)(lut[i].b-color->b);
    if (i == 0 || d2 < d1)
	{
      *idx = i;
      d1 = d2;
	}
  }

# define TOOFAR (3*20*20*20)
  
  dt_dbg(ctl, F, DT_DBG_MASK_LUT,
    "closest to (%3d,%3d,%3d) is %3d (%3d,%3d,%3d) d=%ld%s",
    color->r,
    color->g,
    color->b,
    *idx, 
    lut[*idx].r,
    lut[*idx].g,
    lut[*idx].b,
    d1,
    d1 > TOOFAR? " (too far)": "");

  if (d1 > TOOFAR)
    *idx = -1;

  return DT_RC_GOOD;
}

/*..........................................................................
 * makes a lut to transform image to be like default colormap
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtlut8_match(							/* match user to display colors		*/
  dt_ctl_t *ctl,
  dtlut8_t *user,
  dtlut8_t *display,
  unsigned char *user_to_display,
  unsigned char *display_to_user)
{
  DT_F("dtlut8_match");
  unsigned char *uw;
  unsigned char *wu;
  int u, w, n;
  int idx, idx1, idx2;
  char dlut_used[256];
  char ulut_matched[256];
  int i;

  uw = user_to_display;					/* for short					    */
  wu = display_to_user;

  for (i=0; i<256; i++)
    dt_dbg(ctl, F, DT_DBG_MASK_PIXEL,
      "%3d. user (%3d,%3d,%3d,%3d)   display (%3d,%3d,%3d,%3d)",
      i,
      user[i].r, user[i].g, user[i].b, user[i].i,
      display[i].r, display[i].g, display[i].b, display[i].i);
      
  DT_MEMSET(dlut_used, 0,
    sizeof(dlut_used));
  DT_MEMSET(ulut_matched, 0,
    sizeof(ulut_matched));

  n = 0;
  for (u=0; u<256; u++)					/* for each user lut entry		    */
  {
    DT_Q(dtlut8_search,(ctl,			/* search display lut for match	    */
      &user[u], display,
      &idx1, &idx2));					/* get range of same-colored cells	*/

    for (i=idx1; i<=idx2; i++)			/* check list of duplicates		    */
	{
      idx = display[i].i;
      if (!dlut_used[idx])				/* until we find an unused one	    */
        break;
	}

    if (i <= idx2)						/* found an unused display index?   */
	{
	  dt_dbg(ctl, F, DT_DBG_MASK_PIXEL,
		"user color %3d matches %3d (%3d,%3d,%3d)",
        u, idx,
        display[i].r, 
        display[i].g,
        display[i].b);
      uw[u] = (unsigned char)idx;		/* map user to window			    */
      ulut_matched[u] = 1;
      wu[idx] = (unsigned char)u;		/* map window to user			    */
      dlut_used[idx] = 1;
      n++;
	}
  }
  dt_dbg(ctl, F, DT_DBG_MASK_LUT,
	"%d user colors matched display colors",
    n);
  w = -1;
  for (u=0; u<256; u++)					/* rescan user colors				*/
  {
    if (!ulut_matched[u])				/* user color matched no display?   */
	{
	  while (dlut_used[++w]);			/* find an unmatched window color  	*/
      idx = display[w].i;
	  dt_dbg(ctl, F, DT_DBG_MASK_PIXEL,
		"user color %3d replaces %3d (%3d,%3d,%3d)",
        u, 
        display[idx].i,
        display[idx].r, 
        display[idx].g,
        display[idx].b);
	  uw[u] = (unsigned char)w;
	  wu[w] = (unsigned char)u;
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
