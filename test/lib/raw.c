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
| dtt_check - check condition
| 
| DESCRIPTION
| As yet undocumented.
| 
| RETURN VALUES
| As yet undocumented.
| 
| ERRORS
| As yet undocumented.
| 
| END
 *..........................................................................*/

#include <dtack/t.h>

DT_RCSID("test $RCSfile: raw.c,v $ $Revision: 1.4 $");
#include <dtack/mem.h>

/*..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtt_raw_export(
  dt_ctl_t *ctl,
  dtimage_t *image,
  void (*put)(char *, int, int, int, int))
{
  DT_F("dtt_raw_export");
  dtxy_t y;

  y = 0;
  while (y<image->ye)
  {
    void *p;
    unsigned int datasize;
    unsigned int nrows;

    DTIMAGE_GETROWS(ctl, image, y,		/* map the next block               */
      &p, &datasize);
    nrows = datasize / image->rowsize;	/* how many rows in block           */

    put((char *)p,						/* display next block of image		*/
      0, y,
      (int)image->rowsize,
      (int)nrows);

    DTIMAGE_UNMAPROW(ctl, image, y);	/* don't need this block any more   */
    y += nrows;
  }
  
  return DT_RC_GOOD;
}

/*..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtt_raw_import(
  dt_ctl_t *ctl,
  dtimage_t *image,
  void (*get)(char *, int, int, int, int))
{
  DT_F("dtt_raw_import");
  dtxy_t y;

  y = 0;
  while (y<image->ye)
  {
    void *p;
    unsigned int datasize;
    unsigned int nrows;

    DTIMAGE_MAPROWS(ctl, image, y,		/* map the next block               */
      &p, &datasize);
    nrows = datasize / image->rowsize;	/* how many rows in block           */

    get((char *)p,						/* retrieve next block of image		*/
      0, y,
      (int)image->rowsize,
      (int)nrows);

    DTIMAGE_PUTROW(ctl, image, y);		/* mark the block as changed		*/
    y += nrows;
  }
  
  return DT_RC_GOOD;
}

/*..........................................................................*/
dt_rc_e
DTCONFIG_API1
dtt_raw_report(
  dt_ctl_t *ctl,
  char *id,
  int loops,
  dtos_time_t *mark1,
  dtos_time_t *mark2,
  dtos_time_t *mark3,
  dtos_time_t *mark4,
  long *cmprc,
  int *assert_fail)
{
  DT_F("dtt_raw_report");
  char tmp[64];

  sprintf(tmp, "%s put (%d)", id, loops);
  DT_Q(dtos_time_report,(ctl,
    mark1, mark2, tmp))

  sprintf(tmp, "%s get (%d)", id, loops);
  DT_Q(dtos_time_report,(ctl,
    mark3, mark4, tmp))

  *assert_fail += DT_RC_GOOD !=
    dt_assert(ctl, *cmprc == -1,
      id, "image comparison");

  return DT_RC_GOOD;
}
/*..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtt_raw(
  dt_ctl_t *ctl,
  dtparam_t *param,
  int loops,
  void (*put)(char *, int, int, int, int),
  void (*get)(char *, int, int, int, int),
  dtos_time_t *mark1,
  dtos_time_t *mark2,
  dtos_time_t *mark3,
  dtos_time_t *mark4,
  long *cmprc)
{
  DT_F("dtt_raw");
  dtimage_t img1;
  dtimage_t img2;
  int loop;
  dt_rc_e rc;
										/* ................................ */
  img1.xe = 0;
  img2.xe = 0;

  DT_C(dtimage_create,(ctl,				/* make an image					*/
    &img1, 
    (dtxy_t)param->w,
    (dtxy_t)param->h, 
    DT_NTYPE_B08));

  DT_C(dtimage_create,(ctl,				/* make second image				*/
    &img2, 
    (dtxy_t)param->w,
    (dtxy_t)param->h, 
    DT_NTYPE_B08));

  DT_C(dtimage_gen_test1,(ctl,			/* generate a test pattern			*/
    &img1));

  DT_C(dtos_time,(ctl, mark1));
  for (loop=0; loop<loops; loop++)
    DT_C(dtt_raw_export,(ctl,			/* export the image				    */
      &img1, put));
  DT_C(dtos_time,(ctl, mark2));

  DT_C(dtos_time,(ctl, mark3));
  for (loop=0; loop<loops; loop++)
    DT_C(dtt_raw_import,(ctl,			/* import the image				    */
      &img2, get));
  DT_C(dtos_time,(ctl, mark4));

  DT_C(dtimage_cmp,(ctl,				/* compare two images				*/
    &img1, &img2, 0,
    cmprc, NULL, NULL));

  if (*cmprc != 0)
    *cmprc = -1;
cleanup:

  if (img2.xe)
  DT_Q(dtimage_free,(ctl, &img2));		/* free image space				    */
  if (img1.xe)
  DT_Q(dtimage_free,(ctl, &img1));		/* free image space				    */

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
