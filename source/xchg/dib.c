/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
  ! Copyright (C) 1998 Dtack Inc. All Rights Reserved
  ! To use this file, you must have signed a license agreement with Dtack Inc.
  ! Under no circumstances may you redistribute this file.
  ! This software is provided AS IS with no warranty expressed or implied.
  ! Dtack Inc. accepts no liability for use or misuse of this file.
  ! http://www.dtack.com  dtack@dtack.com  telephone +360.417.6700
  ! Dtack Inc, P.O. Box 3061, Port Angeles, WA USA 98362
  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */




 #include <dtack/base.h>
DT_RCSID("source/high $RCSfile: dib.c,v $ $Revision: 1.3 $");

#include <dtack/dib.h>
#include <dtack/os.h>
#include <dtack/mem.h>

static dtntype_b32_t longword = 0xdeadbeefUL;

#define IS_LITTLE_ENDIAN \
  (*(char *)(&longword) == (char)0xef)

#define SWAP(V, A, B) \
  { \
    char t = ((char *)&(V))[A]; \
    ((char *)&(V))[A] = ((char *)&(V))[B]; \
    ((char *)&(V))[B] = t; \
  }
#define FIX2(V) SWAP(V, 0, 1)
#define FIX4(V) SWAP(V, 0, 3) SWAP(V, 1, 2);

/*..........................................................................
| NAME
| dtdib_import() - import Microsoft Windows bitmap
| 
| SUMMARY
| Assume ~dib~ has been initialized.
| Change no part of ~dib~.
| Interpret the input sequentially, first the pallette then the image data.y
| Use the given function to get new bytes.
| Create a new image if the given structure is not initialized.
| 
| END
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtdib_import(
  dt_ctl_t *ctl,
  const dtdib_t *dib,
  dt_io_f *readfunc,
  void *arg,
  dtxy_t y0,							/* start loading at this row 		*/
  int reverse,
  dtxy_t xe,							/* allocate image with this size 	*/
  dtxy_t ye,
  dtimage_t *image,
  unsigned int pagesize,
  unsigned int npages)
{
  DT_F("dtdib_import");
  int created = 0;
  dt_ntype_e ntype;
  int f3 = 1;
  int ncolors;
  int rgbsize;
  dtdib_rgb_t *rgb = NULL;
  dtxy_t y;
  dt_rc_e rc;

  dt_dbg(ctl, F, DT_DBG_MASK_OBJECT,
    "bitmap: biSize %ld,"
    " biWidth %ld,"
    " biHeight %ld",
    dib->biSize, 
    dib->biWidth, 
    dib->biHeight);

  dt_dbg(ctl, F, DT_DBG_MASK_OBJECT,
    "bitmap: biBitCount %d,"
    " biSizeImage %ld"
    " biClrUsed %ld",
    dib->biBitCount, 
    dib->biSizeImage, 
    dib->biClrUsed);

  dt_dbg(ctl, F, DT_DBG_MASK_OBJECT,
    "starting at row %u,"
    " image %s",
    y0, image->xe?
      "already exists":
      "will be created");

										/* ................................ */
  if (dib->biSize != 40)				/* make sure first member is right	*/
  {
    rc = dt_err(ctl, F,
      "bitmap is corrupt"
      " (biSize %ld != 40)",
      dib->biSize);
	goto cleanup;
  }

  if (dib->biBitCount == 1)				/* check bit count 					*/
    ntype = DT_NTYPE_B01;
  else
  if (dib->biBitCount == 8)
    ntype = DT_NTYPE_B08;
  else
  if (dib->biBitCount == 24)
  {
    ntype = DT_NTYPE_B08;
	f3 = 3;
  }
  else
                                        // watchfrog #1
  if (dib->biBitCount == 32)
  {
    ntype = DT_NTYPE_B08;
	f3 = 4;
  }
  else
  {
    rc = dt_err(ctl, F,
      "Sorry, %d-bit images"
      " are not supported",
      dib->biBitCount);
	goto cleanup;
  }

  if (dib->biClrUsed > 256 &&			/* check color count 				*/
	  dib->biClrUsed != 0xffff)
  {
    rc = dt_err(ctl, F,
      "bitmap is corrupt"
      " (biClrUsed %ld out of range)",
      dib->biClrUsed);
	goto cleanup;
  }

										/* ................................ */
  if (dib->biClrUsed == 0xffff)			/* special case: no pallette		*/
  {										/* DTACK MADE THIS UP!			    */
    ncolors = 0;
  }
  else
  if (dib->biClrUsed != 0)				/* how many colors in pallette 		*/
  {
    ncolors = (int)dib->biClrUsed;
  }
  else
  if (dib->biBitCount == 1)
  {
    ncolors = 2;
  }
  else
  if (dib->biBitCount == 8)
  {
    ncolors = 256;
  }
  else
  if (dib->biBitCount == 24)
  {
    ncolors = 256;
  }
  else
  if (dib->biBitCount == 32)
  {
    ncolors = 256;
  }

  if (ncolors)							/* is there a pallette?				*/
  {
	rgbsize = sizeof(*rgb) * ncolors;	/* size of pallette 				*/
	DT_C(dtos_malloc2,(ctl,				/* space for the pallette 			*/
      (void **)&rgb, rgbsize,
      F, "pallette"));
  
	DT_C((readfunc),(ctl, arg,			/* fetch pallette					*/
      rgb, rgbsize));
  }

  if (image->xe == 0)					/* caller has not allocated image? 	*/
  {
	if (xe == 0)
	  xe = (dtxy_t)dib->biWidth * f3;
	if (ye == 0)
	  ye = (dtxy_t)dib->biHeight;
    DT_C(dtimage_create3,(ctl, image,	/* get space for the image		    */
      pagesize, npages,
      xe, ye,
      ntype, NULL));
	created = 1;
    dt_dbg(ctl, F, DT_DBG_MASK_OBJECT,
      "allocated new image %ux%u",     
      xe, ye);
  }

  if (dib->biWidth * f3 !=				/* check image width 				*/
	  (dtntype_b32_t)image->xe)
  {
	rc = dt_err(ctl, F,
      "bitmap width %ld"
      " not same as image %u",
      dib->biWidth, image->xe);
	goto cleanup;
  }

  if (dib->biHeight+y0 >				/* check image height 				*/
	  (dtntype_b32_t)image->ye)
  {
	rc = dt_err(ctl, F,
      "bitmap height %ld from y0 %u"
      " taller than image %u",
      dib->biHeight, y0, image->ye);
	goto cleanup;
  }

  if (ntype != image->ntype)			/* check image type 				*/
  {
	rc = dt_err(ctl, F,
      "bitmap ntype %d"
      " not same as image %d",
      ntype, image->ntype);
	goto cleanup;
  }

  ye = dib->biHeight;					/* height of the bitmap 			*/
  
  for (y=0; y<ye; y++)
  {
	dt_ntype_b08_t *p;
	char pad[DTDIB_PADMAX];
	dtxy_t row = reverse?				/* supposed to reverse rows? 		*/
      y0+ye-y-1: y0+y;
	DTIMAGE_GETROW2(DT_C, ctl, image,	/* access our image row 			*/
      row, (void **)&p);
	DT_C((readfunc),(ctl, arg,			/* fetch row from dib 				*/
      (void *)p, image->rowsize));
	if (DTDIB_PAD(image->rowsize))
  	  DT_C((readfunc),(ctl, arg, pad,	/* fetch pad						*/
        DTDIB_PAD(image->rowsize)));
                                        /* 24-bit?						    */
    if (f3 == 3) 			
	{
	  unsigned int x;
	  dt_ntype_b08_t b;
	  for (x=0; x<image->rowsize; x+=3)
	  {
		b = p[x];						/* switch BGR to RGB			    */
		p[x] = p[x+2];
		p[x+2] = b;
	  }
	}
	else
                                        /* 32-bit?			    */
                                        // watchfrog #1
    if (f3 == 4) 			
	{
	  unsigned int x;
	  dt_ntype_b08_t b;
	  for (x=0; x<image->rowsize; x+=4)
	  {
		b = p[x];						/* switch BGR to RGB			    */
		p[x] = p[x+2];
		p[x+2] = b;
	  }
	}
	DTIMAGE_PUTROW2(DT_C, ctl, image,	/* release our image row 			*/
      row);
  }

  dt_dbg(ctl, F, DT_DBG_MASK_OBJECT,
	"read rows %u to %u",
    y0, y);

cleanup:
  if (rgb != NULL)
    DT_I(dtos_free2,(ctl, rgb,			/* free pallette 					*/
      F, "pallette"));
  
  if (rc != DT_RC_GOOD && created)
    DT_I(dtimage_free,(ctl, image));

  return rc;
}

/*..........................................................................
| NAME
| dtdib_init() - initialize Microsoft Windows DIB header
| 
| SUMMARY
| Fill all members of header.
| All members are left in native host byte order.
| Presume we are writing only ~ye~ rows unless ~ye~ is zero.
| 
| END
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtdib_init(								/* create windows dib header		*/
  dt_ctl_t *ctl,
  dtdib_t *dib,
  dtxy_t ye,
  dtimage_t *image)
{
  if (ye == 0)							/* caller wants to write it all? 	*/
    ye = image->ye;

  dib->biSize =          sizeof(*dib);
  dib->biWidth =         image->xe;
  dib->biHeight =        ye;
  dib->biPlanes =        1;
  dib->biBitCount =      8;
  dib->biCompression =   0;
  dib->biSizeImage =     0;
  dib->biXPelsPerMeter = 0;
  dib->biYPelsPerMeter = 0;
  dib->biClrUsed =       0;
  dib->biClrImportant =  0;
  
  return DT_RC_GOOD;
}

/*..........................................................................
| NAME
| dtdib_endian() - fix dib header members endianness
| 
| SUMMARY
| Switch endianness of all dib header members if on a big-endian machine.
| 
| END
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtdib_endian(							/* fix windows dib header			*/
  dt_ctl_t *ctl,
  dtdib_t *dib)
{
  if (IS_LITTLE_ENDIAN)					/* already little endian? 			*/
    return DT_RC_GOOD;
  FIX4(dib->biSize);
  FIX4(dib->biWidth);
  FIX4(dib->biHeight);
  FIX2(dib->biPlanes);
  FIX2(dib->biBitCount);
  FIX4(dib->biCompression);
  FIX4(dib->biSizeImage);
  FIX4(dib->biXPelsPerMeter);
  FIX4(dib->biYPelsPerMeter);
  FIX4(dib->biClrUsed);
  FIX4(dib->biClrImportant);
  return DT_RC_GOOD;
}

/*..........................................................................
| NAME
| dtdib_rgb_gray() - initialize Microsoft Windows pallette
| 
| SUMMARY
| Fill all members of pallette with gray level transform.
| 
| END
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtdib_rgb_gray(
  dt_ctl_t *ctl,
  dtdib_rgb_t *rgb,
  dtimage_t *image)
{
  int i;
  for (i=0; i<256; i++)
  {
    rgb[i].rgbRed = i;
    rgb[i].rgbGreen = i;
    rgb[i].rgbBlue = i;
    rgb[i].rgbReserved = 0;
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
