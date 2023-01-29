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
DT_RCSID("source/high $RCSfile: cooc.c,v $ $Revision: 1.1 $");
#include <dtack/mem.h>

static dt_ntype_e input_ntypes[] = {
  DT_NTYPE_B01, DT_NTYPE_B08, DT_NTYPE_NULL};
 
static dt_ntype_e cooc_ntypes[] = {
  DT_NTYPE_B32, DT_NTYPE_NULL};

/*..........................................................................*/
dt_rc_e
DTCONFIG_API1
dtimage_cooc_sum(
  dt_ctl_t *ctl,
  dtimage_t *input,
  int dx,
  int dy,
  dtimage_t *cooc)
{
  DT_F("dtimage_cooc_sum");
  dtxy_t x1, y1;
  dtxy_t x2, y2;
  dt_rc_e rc;

  rc = dtimage_check_1f(ctl, input,		/* check input data type		    */
    "input", 
    input_ntypes, 
    DTIMAGE_FLAG_PAGED, F);
  if (rc != DT_RC_GOOD)
    return rc;
  rc = dtimage_check_1f(ctl, cooc,		/* check output data type		    */
    "output co-occurence matrix",
    cooc_ntypes, 
    DTIMAGE_FLAG_PAGED, F);
  if (rc != DT_RC_GOOD)
    return rc;

  if (dx < 0)							/* x scan limits				    */
  {
	x1 = -dx;
    x2 = 0;
  }
  else
  {
    x1 = 0;
    x2 = dx;
  }

  if (dy < 0)							/* y scan limits				    */
  {
	y1 = -dy;
    y2 = 0;
  }
  else
  {
    y1 = 0;
    y2 = dy;
  }

  switch(input->ntype)
  {
    case DT_NTYPE_B08:
      DT_Q(dtimage_cooc_b08,(ctl,
        input,
        x1, y1, x2, y2,
        input->xe-abs(dx),
        input->ye-abs(dy),
        cooc));
    break;
    default:
      DT_Q(dtimage_cooc_any,(ctl,
        input,
        x1, y1, x2, y2,
        input->xe-abs(dx),
        input->ye-abs(dy),
        cooc));
  }

  return DT_RC_GOOD;
}
/*..........................................................................*/
dt_rc_e
DTCONFIG_API1
dtimage_cooc_set(
  dt_ctl_t *ctl,
  dtimage_t *input,
  int dx,
  int dy,
  dtimage_t *cooc)
{
  DT_F("dtimage_cooc_set");

  DT_Q(dtimage_constant,(ctl,			/* clear output image to start	    */
    cooc, 0.0));

  DT_Q(dtimage_cooc_sum,(ctl,
    input, dx, dy, cooc));

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
