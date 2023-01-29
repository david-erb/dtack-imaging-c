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
DT_RCSID("source/high $RCSfile: xchg_dsk.c,v $ $Revision: 1.1 $");

#include <dtack/os.h>
#include <dtack/lut8.h>
#include <dtack/xchg.h>
#include <dtack/list.h>						/* needed in dtdsk.h				*/
#include <dtack/dsk.h>
#include <dtack/dsk_c.h>
#include <dtack/mem.h>

/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtxchg_dsk_export_image(
  dt_ctl_t *ctl,
  const char *server,
  const char *id,
  dtimage_t *image,
  dtxy_t xpos,
  dtxy_t ypos)
{
  DT_F("dtxchg_dsk_export_image");
  dt_ntype_e ntypes[] =					/* datatypes we can handle			*/
    {DT_NTYPE_B01, DT_NTYPE_B08, 
     DT_NTYPE_NULL};
  dtxy_t y;
  dt_rc_e rc;

  rc = dtimage_check_1f(ctl,			/* check image validity			    */
    image, id, 
    ntypes, DTIMAGE_FLAG_PAGED, F);
  if (rc != DT_RC_GOOD)
    return rc;

  if (strlen(server) == 0)
    server = DTDSK_DEFAULT_SERVER;
  if (strlen(id) == 0)
    id = DTDSK_DEFAULT_ID;
  DT_Q(dtdsk_clnt_create_window,(ctl,
    server, id, xpos, ypos,
    image->xe, image->ye,
    dt_ntype_bits[image->ntype]));

  y = 0;	
  do {
    void *p;
    unsigned int datasize;
    unsigned int nrows;

    DTIMAGE_GETROWS(ctl, image, y,		/* map the next block               */
      &p, &datasize);
    nrows = datasize / image->rowsize;	/* how many rows in block           */
   
    DT_Q(dtdsk_clnt_export_data,(ctl,	/* go put data into the window	    */
      server, id, 0, y,
      image->rowsize, nrows, 
      dt_ntype_bits[image->ntype],
      p));

    DTIMAGE_UNMAPROW(ctl, image, y);	/* don't need this block any more   */
    y += nrows;
  } while(y < image->ye);

  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtxchg_dsk_import_image2(
  dt_ctl_t *ctl,
  const char *server,
  const char *id,
  dtimage_t *image,
  unsigned int pagesize,
  unsigned int npages)
{
  DT_F("dtxchg_dsk_import_image");
  dtdsk_win_t win;
  dt_ntype_e ntype;
  dtxy_t y;

  if (strlen(server) == 0)
    server = DTDSK_DEFAULT_SERVER;
  if (strlen(id) == 0)
    id = DTDSK_DEFAULT_ID;

  DT_Q(dtdsk_getwin,(ctl,				/* get info on existing window	    */
    server, id, &win));

  if (win.bits == 1)
    ntype = DT_NTYPE_B01;
  else
  if (win.bits == 8)
    ntype = DT_NTYPE_B08;
  else
    return dt_err(ctl, F,
      "cannot handle %d-bit image from \"%s/%s\"",
      win.bits, server, id);

  DT_Q(dtimage_create3,(ctl, image,		/* get space for image			    */
    pagesize, npages,
    win.xe, win.ye, ntype,
    NULL));

  DT_Q(dtimage_dbg_op,(ctl, image,		/* trace execution				    */
    id, F));

  y = 0;	
  do {
    void *p;
    unsigned int datasize;
    unsigned int nrows;

    DTIMAGE_MAPROWS(ctl, image, y,		/* map the next block               */
      &p, &datasize);
    nrows = datasize / image->rowsize;	/* how many rows in block           */
   
    DT_Q(dtdsk_clnt_import_data,(ctl,	/* go get data from the window	    */
      server, id, 0, y,
      image->rowsize, nrows,
      p));

    DTIMAGE_PUTROW(ctl, image, y);		/* mark block changed				*/
    y += nrows;
  } while(y < image->ye);

  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtxchg_dsk_import_image(
  dt_ctl_t *ctl,
  const char *server,
  const char *id,
  dtimage_t *image)
{
  DT_F("dtxchg_dsk_import_image");
  DT_Q(dtxchg_dsk_import_image2,(ctl,
    server, id, image, 0, 0));
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
