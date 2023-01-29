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
| dtimage_cooc() - cooccurrence matrix definition
| 
| SUMMARY
| Make coocurrent matrix.
| 
| END
 *..........................................................................*/

#include <dtack/base.h>
DT_RCSID("source/high $RCSfile: cooc2.c,v $ $Revision: 1.1 $");
#include <dtack/mem.h>
 
static dt_ntype_e cooc_ntypes[] = {
  DT_NTYPE_B32, DT_NTYPE_NULL};

/*..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtimage_cooc_b08(
  dt_ctl_t *ctl,
  dtimage_t *input,
  dtxy_t x1,
  dtxy_t y1,
  dtxy_t x2,
  dtxy_t y2,
  dtxy_t xe,
  dtxy_t ye,
  dtimage_t *cooc)
{
  DT_F("dtimage_cooc_b08");
  dtxy_t x, y;
  dt_ntype_b08_t *p1, *p2;
  for (y=0; y<ye; y++)					/* for all input rows			    */
  {
    DTIMAGE_GETROW(ctl, input, y+y1,
      &p1);
    DTIMAGE_GETROW(ctl, input, y+y2,
      &p2);
    p1 += x1;
    p2 += x2;
    for (x=0; x<xe; x++)
	{
	  dt_ntype_b32_t *q;
	  dtxy_t row, col;
      if (p1[x] > p2[x])				/* sum only into upper triangle	    */
	  {
		row = p2[x];
		col = p1[x];
	  }
	  else
	  {
		row = p1[x];
		col = p2[x];
	  }

      DTIMAGE_GETROW(ctl, cooc, row, &q);
      q[col]++;
      DTIMAGE_PUTROW(ctl, cooc, row);
	}
    DTIMAGE_UNMAPROW(ctl, input, y+y2);
    DTIMAGE_UNMAPROW(ctl, input, y+y1);
  }
  return DT_RC_GOOD;
}
/*..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtimage_cooc_any(
  dt_ctl_t *ctl,
  dtimage_t *input,
  dtxy_t x1,
  dtxy_t y1,
  dtxy_t x2,
  dtxy_t y2,
  dtxy_t xe,
  dtxy_t ye,
  dtimage_t *cooc)
{
  DT_F("dtimage_cooc_any");
  dtxy_t x, y;
  double v1, v2;
  for (y=0; y<ye; y++)					/* for all input rows			    */
  {
    for (x=0; x<xe; x++)				/* for all input pixels			    */
	{
	  dt_ntype_b32_t *q;
	  dtxy_t row, col;

      DT_Q(dtimage_get_pixel,(ctl,		/* fetch joint pixel				*/
        input, x1+x, y1+y, &v1));
      DT_Q(dtimage_get_pixel,(ctl,		/* fetch pixel for comparison	    */
        input, x2+x, y2+y, &v2));

      if (v1 > v2)						/* sum only into upper triangle	    */
	  {
		row = (dtxy_t)v2;
		col = (dtxy_t)v1;
	  }
	  else
	  {
		row = (dtxy_t)v1;
		col = (dtxy_t)v2;
	  }

      DTIMAGE_GETROW(ctl, cooc, row, &q);
      q[col]++;
      DTIMAGE_PUTROW(ctl, cooc, row);
	}
  }
  return DT_RC_GOOD;
}

/*..........................................................................*/
dt_rc_e
DTCONFIG_API1
dtimage_cooc_mirror(
  dt_ctl_t *ctl,
  dtimage_t *cooc)
{
  DT_F("dtimage_cooc_mirror");
  dtxy_t x, y;
  dt_rc_e rc;

  rc = dtimage_check_1f(ctl, cooc,		/* check output data type		    */
    "output co-occurence matrix",
    cooc_ntypes, 
    DTIMAGE_FLAG_PAGED, F);
  if (rc != DT_RC_GOOD)
    return rc;

  for (y=0; y<cooc->ye; y++)			/* do lower triangle			    */
  {
    dt_ntype_b32_t *p;
    DTIMAGE_GETROW(ctl, cooc, y, &p);
    for (x=y+1; x<cooc->xe; x++)
	{
      dt_ntype_b32_t *q;
      DTIMAGE_GETROW(ctl, cooc, x, &q);
      q[y] = p[x];
      DTIMAGE_PUTROW(ctl, cooc, x);
	}
    DTIMAGE_UNMAPROW(ctl, cooc, y);
  }

  for (y=0; y<cooc->ye; y++)			/* do diagonal					    */
  {
    dt_ntype_b32_t *p;
    DTIMAGE_GETROW(ctl, cooc, y, &p);
    p[y] *= 2;
    DTIMAGE_PUTROW(ctl, cooc, y);
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
