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
DT_RCSID("source/high $RCSfile: read_tif.c,v $ $Revision: 1.1 $");
#include <dtack/mem.h>

#if DTPRJ_TIFF != DT_1

dt_rc_e
DTCONFIG_API1
dtimage_read_tiff_file(	
  dt_ctl_t *ctl,
  dtimage_t *image,
  char *filename)
{
  DT_F("dtimage_read_tiff_file");
  
  return dt_err_notlinked(ctl, F,
    "tiff");
}

#else

#ifndef DTDEPEND
#include <tiffio.h>
#endif

/*..........................................................................
 *..........................................................................*/

static
dt_rc_e
dtimage_read_tiff_row(					/* get row from tif file			*/
  dt_ctl_t *ctl,
  dtimage_t *image,
  unsigned int rowsize,
  dtxy_t row,
  TIFF *tiff)
{
  DT_F("dtimage_read_tiff_row");
  dt_rc_e rc = DT_RC_GOOD;
  int tiffrc;
  void *p;

  DTIMAGE_MAPROW(ctl, image, row, &p);	/* map row into ram					*/
    
  ((char *)p)[rowsize-1] = 0;			/* possibly pad bit image columns 	*/
  
  tiffrc = TIFFReadScanline(tiff,		/* read line from tiff			    */
    p, row, 0);
  if (tiffrc < 0)
    return dt_err(ctl, F,
      "TIFFReadScanline(tiff, buf, y=%u, 0) gave rc %d",
      row, rc);

  DTIMAGE_PUTROW(ctl, image, row);		/* mark we changed the row			*/

  return DT_RC_GOOD;
}

/*..........................................................................
| if tiff image is a bit image and the number of columns is not evenly
| divisible by 8, then the created dtimage will be padded with 0's
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtimage_read_tiff_file(					/* get image from tif file			*/
  dt_ctl_t *ctl,
  dtimage_t *image,
  char *filename)
{
  DT_F("dtimage_read_tiff_file");
  TIFF *tiff;
  unsigned short bps;
  unsigned short spp;
  unsigned short photometric;
  int width, length;
  int tiffrc;
  unsigned int rowsize;
  dt_ntype_e ntype;
  dtxy_t y;
  dt_rc_e rc = DT_RC_GOOD;

  tiff = TIFFOpen(filename, "r");		/* open tiff file				    */
  if (tiff == NULL)
    return dt_err(ctl, F,
      "TIFFOpen(\"%s\", \"r\") failed",
      filename);

  if (rc == DT_RC_GOOD)
  {
    tiffrc = TIFFGetField(tiff,			/* bits per sample				    */
      TIFFTAG_BITSPERSAMPLE, &bps);
    if (tiffrc == 0)
      rc = dt_err(ctl, F, 
        "no bits/sample field");
  }
  if (rc == DT_RC_GOOD)
  {
    tiffrc = TIFFGetField(tiff,			/* samples per pixel			    */
      TIFFTAG_SAMPLESPERPIXEL, &spp);
    if (tiffrc == 0)
      rc = dt_err(ctl, F, 
        "no samples/pixel field");
  }
  if (rc == DT_RC_GOOD)
  {
    tiffrc = TIFFGetField(tiff,			/* photometric type				    */
      TIFFTAG_PHOTOMETRIC,
      &photometric);
    if (tiffrc == 0)
      rc = dt_err(ctl, F, 
        "no photometric field");
  }
  if (rc == DT_RC_GOOD)
  {
    tiffrc = TIFFGetField(tiff,			/* image width					    */
      TIFFTAG_IMAGEWIDTH, &width);
    if (tiffrc == 0)
      rc = dt_err(ctl, F, 
        "no image width field");
  }
  if (rc == DT_RC_GOOD)
  {
    tiffrc = TIFFGetField(tiff,			/* image height					    */
      TIFFTAG_IMAGELENGTH, &length);
    if (tiffrc == 0)
      rc = dt_err(ctl, F, 
        "no image length field");
  }

  if (rc == DT_RC_GOOD)
  if (bps == 1)
  {
    ntype = DT_NTYPE_B01;
    width = (width+7) & ~7;				/* increase to integral bytes 		*/
  }
  else
  if (bps == 8)
    ntype = DT_NTYPE_B08;
  else
    rc = dt_err(ctl, F,
      "unsupported bits/sample %d",
      bps);

  if (rc == DT_RC_GOOD)
  if (spp != 1)
    rc = dt_err(ctl, F,
      "unsupported samples/pixel %d",
      spp);
  
  rowsize = width *						/* size of one row in bytes		    */
    dt_ntype_sizeof[ntype];
  
  if (rc == DT_RC_GOOD)
  if (TIFFScanlineSize(tiff) !=			/* see if tiff thinks same rowsize  */
      (int)rowsize)
    rc = dt_err(ctl, F,
      "rowsize %ld but TIFFScanlineSize %d",
      rowsize,
      TIFFScanlineSize(tiff));

  if (rc != DT_RC_GOOD)
  {
	TIFFClose(tiff);
    return rc;
  }

  DT_Q(dtimage_create,(ctl, image,		/* create image					    */
    width, length, ntype));

  for (y=0; y<image->ye; y++)
  {
    DT_G(dtimage_read_tiff_row,(ctl,
      image, rowsize, y, tiff));
    if (rc != DT_RC_GOOD)				/* read failed?					    */
      break;
  } while(y < image->ye);

  if (rc != DT_RC_GOOD)					/* read failed?					    */
    DT_Q(dtimage_free,(ctl, image));	/* free image before error exit	    */

  TIFFClose(tiff);

  return rc;
}
#endif






/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
  ! Copyright (C) 1998 Dtack Inc. All Rights Reserved
  ! To use this file, you must have signed a license agreement with Dtack Inc.
  ! Under no circumstances may you redistribute this file.
  ! This software is provided AS IS with no warranty expressed or implied.
  ! Dtack Inc. accepts no liability for use or misuse of this file.
  ! http://www.dtack.com  dtack@dtack.com  telephone +360.417.6700
  ! Dtack Inc, P.O. Box 3061, Port Angeles, WA USA 98362
  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */
