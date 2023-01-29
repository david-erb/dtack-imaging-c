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
| Function return value will be DT_RC_GOOD on success.  
| Any other return value indicates failure or noncompletion.
| When DT_RC_GOOD is not returned, any other return values may not be valid.
| 
| ERRORS
| As yet undocumented.
| 
| END
 *..........................................................................*/

#include <dtack/base.h>
#include <dtack/lut8.h>

DT_RCSID("source/high $RCSfile: gen.c,v $ $Revision: 1.1 $");
#include <dtack/mem.h>
#include <dtack/os.h>

/*..........................................................................
 * ramp image with border and x across middle
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtimage_gen_test1(
  dt_ctl_t *ctl,
  dtimage_t *image)
{
  DT_F("dtimage_gen_test1");
  dtxy_t xe = image->xe;
  dtxy_t ye = image->ye;
  dtxy_t x, y;
  dt_ntype_b08_t *p;
  int i;

  DTIMAGE_MAPROW(ctl, image, 0, &p);
  for (x=0; x<xe; x++)					/* top row						    */
    p[x] = xe-x-1;
  DTIMAGE_PUTROW(ctl, image, 0);

  for (y=1; y<ye-1; y++)				/* gray scale background		    */
  {
    DTIMAGE_MAPROW(ctl, image, y, &p);
    for (x=0; x<xe; x++)
      p[x] = x;

    p[y] = ye-y-1;						/* X across middle				    */
    p[xe-y-1] = y;
    p[0] = ye-y-1;						/* left column					    */
    p[xe-1] = ye-y-1;					/* right column					    */
    DTIMAGE_PUTROW(ctl, image, y);
  }

  DTIMAGE_MAPROW(ctl, image, ye-1, &p);
  for (x=0; x<xe; x++)					/* bottom row					    */
    p[x] = xe-x-1;
  DTIMAGE_PUTROW(ctl, image, ye-1);

  for (i=0; i<DTLUT8_NSTANDARD; i++)	/* slap in the colors				*/
  {
    char name[DTLUT8_NAME_MAXLEN+8];
    sprintf(name, "%3d. ", dtlut8_standard_index[i]);
    DT_Q(dtlut8_index_to_name,(ctl,
      i, name+strlen(name)));
    DT_Q(dtimage_plot_text_8x16,(ctl,
      image, name, 48, 8+i*16, dtlut8_standard_index[i]));
  }
  return DT_RC_GOOD;
}

/*..........................................................................
 * ramp image with border and x across middle
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtimage_gen_test2(
  dt_ctl_t *ctl,
  dtimage_t *image)
{
  dtxy_t xe = image->xe;
  dtxy_t ye = image->ye;
  dtxy_t x, y;

  for (y=0; y<ye; y++)					/* gray scale background		    */
    for (x=0; x<xe; x++)
      image->row[y].b08[x] = x;
  for (x=0; x<xe; x++)
  {
    image->row[0].b08[x] = xe-x-1;
    image->row[ye-1].b08[x] = xe-x-1;
  }
  for (y=0; y<ye; y++)
  {
    image->row[y].b08[y] = ye-y-1;
    image->row[y].b08[xe-y-1] = y;
    image->row[y].b08[0] = ye-y-1;
    image->row[y].b08[xe-1] = ye-y-1;
  }

  return DT_RC_GOOD;
}

/*..........................................................................
 * ramp image with border and x across middle
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtimage_gen_test3(
  dt_ctl_t *ctl,
  dtimage_t *image)
{
  dtxy_t xe = image->xe;
  dtxy_t ye = image->ye;
  dtxy_t x, y;

  for (y=0; y<ye; y++)					/* gray scale background		    */
    for (x=0; x<xe; x++)
      image->row[y].b08[x] = x;
  for (x=0; x<xe; x++)
  {
    image->row[0].b08[x] = xe-x-1;
    image->row[ye-1].b08[x] = xe-x-1;
  }
  for (y=0; y<ye; y++)
  {
    image->row[y].b08[y] = ye-y-1;
    image->row[y].b08[xe-y-1] = y;
    image->row[y].b08[0] = ye-y-1;
    image->row[y].b08[xe-1] = ye-y-1;
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
