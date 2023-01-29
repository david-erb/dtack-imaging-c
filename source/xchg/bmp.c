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
DT_RCSID("source/high $RCSfile: wmf.c,v $ $Revision: 1.3 $");

#include <dtack/dib.h>
#include <dtack/bmp.h>
#include <dtack/str.h>
#include <dtack/os.h>
#include <dtack/mem.h>

static unsigned long longword = 0xdeadbeefUL;

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
| dtbmp_endian() - fix bmp header members endianness
| 
| SUMMARY
| Switch endianness of all bmp header members if on a big-endian machine.
| 
| END
 *..........................................................................*/

static
dt_rc_e
dtbmp_endian(							/* fix windows dib header			*/
  dt_ctl_t *ctl,
  dtbmp_filehead_t *bmp)
{
  if (IS_LITTLE_ENDIAN)					/* already little endian? 			*/
    return DT_RC_GOOD;
  FIX2(bmp->bfType);
  FIX4(bmp->bfSize);
  FIX2(bmp->bfReserved1);
  FIX2(bmp->bfReserved2);
  FIX4(bmp->bfOffBits);
  return DT_RC_GOOD;
}

/*..........................................................................
| NAME
| dtbmp_split() - calculate dib split parameters
| 
| SUMMARY
| Calculate number of dibs needed.
| 
| END
 *..........................................................................*/
static
dt_rc_e
dtbmp_split(
  dt_ctl_t *ctl,
  dtbmp_split_t *split,
  dtimage_t *image)
{
  split->ncolors = 256;

  split->recordsize =
    sizeof(dtdib_t) +					/* dib header 						*/
    sizeof(dtdib_rgb_t) *				/* pallette 						*/
      split->ncolors;

  split->size = 
    (long)image->ye * (long)			/* total data size for image 		*/
    DTDIB_PADDED(image->rowsize);
  
  split->ndibs = 1;						/* number of dibs to write 			*/
  
  split->rows1 = 0;						/* number of rows in first dib	    */
  
  split->rows2 = image->ye;				/* number of rows in last dib 		*/

  return DT_RC_GOOD;
}

/*..........................................................................
| NAME
| dtbmp_import() - import Microsoft Windows bmp file
| 
| SUMMARY
| Read image from bmp file.
| Allocate image with given size unless size is 0.
| Interpret the input sequentially.
| Use the given function to get new bytes.
| Create the image and populate it.
| 
| END
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtbmp_import(
  dt_ctl_t *ctl,
  dt_io_f *readfunc,
  void *arg,
  dtimage_t *image,
  unsigned int pagesize,
  unsigned int npages)
{
  DT_F("dtbmp_import");
  dtbmp_filehead_t filehead;
  long size;
  long offbits;
  dtdib_t dib;
  unsigned long mask = DT_DBG_MASK_OBJECT;
  dt_rc_e rc;

  DT_C((readfunc),(ctl, arg,			/* read the file header 			*/
    &filehead, sizeof(filehead)));
  DT_C(dtbmp_endian,(ctl, &filehead));	/* fix endian-ness 					*/
  DT_MEMCPY(&size, &filehead.bfSize,		/* extract long word 				*/
    sizeof(size));
  DT_MEMCPY(&offbits, &filehead.bfOffBits,
    sizeof(offbits));
  dt_dbg(ctl, F, mask,
    "header: bfType 0x%04x,"
    " bfSize %ld"
    " bfOffBits %ld",
    filehead.bfType,
	size, offbits);

  if (filehead.bfType != DTBMP_MAGIC)	/* check bmp file header type 		*/
  {
	rc = dt_err(ctl, F,
      "bmp file is corrupt"
      " (bfType 0x%04x, expected 0x%04x)",
      filehead.bfType, DTBMP_MAGIC);
	goto cleanup;
  }

  DT_C((readfunc),(ctl, arg,			/* get dib header					*/
    &dib, sizeof(dib)));
  DT_C(dtdib_endian,(ctl, &dib));		/* fix endian-ness 					*/

  if (offbits ==						/* no space for a colormap?		    */
	  sizeof(filehead) + sizeof(dib))
    dib.biClrUsed = 0xffff;

  DT_C(dtdib_import,(ctl,				/* import image from dib 			*/
    &dib,
    readfunc, arg,
    0,									/* start reading at this row 		*/
    1,									/* reverse rows						*/
    0,									/* allocate image as necessary		*/
    0,
    image, pagesize, npages));

cleanup:

  return rc;
}

/*..........................................................................
| NAME
| dtbmp_filehead_init() - initialize Microsoft Windows bmp file header
| 
| SUMMARY
| Fill all members of header.
| Also initialize the bmp file records contained in the header.
| For size number, account for the number of dibs we will be writing.
| 
| END
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtbmp_filehead_init(					/* init bmp file header				*/
  dt_ctl_t *ctl,
  dtbmp_filehead_t *filehead,
  dtimage_t *image)
{
  DT_F("dtbmp_filehead_init");
  dtbmp_split_t split;
  long offbits;
  long bfsize;

  DT_Q(dtbmp_split,(ctl, &split,
    image));

  offbits = 
    sizeof(dtbmp_filehead_t) +			/* size of bmp file header 			*/
	split.ndibs * split.recordsize;		/* size of the dib records 			*/

  bfsize = offbits +						/* where the data starts			*/
	split.size;							/* size of all the data 			*/

  DT_MEMSET(filehead, 0, 
    sizeof(*filehead));

  filehead->bfType = DTBMP_MAGIC;
  bfsize /= 4;
  DT_MEMCPY(&filehead->bfSize, &bfsize,
    sizeof(bfsize));
  DT_MEMCPY(&filehead->bfOffBits, &offbits,
    sizeof(offbits));
  
  return DT_RC_GOOD;
}

/*..........................................................................
| NAME
| dtbmp_export() - export Microsoft Windows bmp file
| 
| SUMMARY
| Do not write the Adobe placeable header.
| Use the given function to output bytes.
| 
| END
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtbmp_export(
  dt_ctl_t *ctl,
  dt_io_f *writefunc,
  void *arg,
  dtimage_t *image,
  dtxy_t xpos,
  dtxy_t ypos)
{
  DT_F("dtbmp_export");
  dtbmp_filehead_t filehead;
  long k = 0;
  dtbmp_split_t split;
  int rgbsize;
  dtdib_rgb_t *rgb = NULL;
  char pad[DTDIB_PADMAX];
  int i, j;
  dtxy_t y;
  dtdib_t dib;
  dt_rc_e rc;

  rc = dtimage_check_1f(ctl,			/* check image validity			    */
    image, "bmp file image", NULL,
    DTIMAGE_FLAG_PAGED, F);
  if (rc != DT_RC_GOOD)
    return rc;

										/* ................................ */
  DT_C(dtbmp_filehead_init,(ctl,		/* init bmp file header buffer 		*/
	&filehead, image));						
  DT_C(dtbmp_endian,(ctl, &filehead));	/* fix endian-ness					*/
  
  DT_C((*writefunc),(ctl, arg,			/* output bmp file header			*/
    &filehead, sizeof(filehead)));

  k += sizeof(filehead);
  dt_dbg(ctl, F, DT_DBG_MASK_OBJECT,
    "k %ld after filehead", k);
  
  DT_C(dtbmp_split,(ctl,
    &split, image));

  dt_dbg(ctl, F, DT_DBG_MASK_OBJECT,
    "split size %ld, rows1 %u, rows2 %u",
    split.size, split.rows1, split.rows2);
  dt_dbg(ctl, F, DT_DBG_MASK_OBJECT,
	"split ncolors %d, ndibs %d, recordsize %d",
    split.ncolors, split.ndibs,
    split.recordsize);

										/* ................................ */
  rgbsize = sizeof(*rgb) *				/* size of pallette 				*/
    split.ncolors;
  DT_C(dtos_malloc2,(ctl,				/* space for the pallette 			*/
    (void **)&rgb, rgbsize,
    F, "pallette"));
  
  DT_C(dtdib_rgb_gray,(ctl, rgb,		/* initialize the pallette 			*/
    image));
										/* ................................ */

  y = 0;
  for (i=0; i<split.ndibs; i++)			/* for each split dib 				*/
  {
    int nrows = 
      i < split.ndibs-1?
        split.rows1: split.rows2;
								
    DT_C(dtdib_init,(ctl, &dib,			/* initialize dib header 			*/
      nrows, image));
    DT_C(dtdib_endian,(ctl, &dib));		/* fix endian-ness					*/
    
    DT_C((*writefunc),(ctl, arg,		/* output dib header				*/
      &dib, sizeof(dib)));
    k += sizeof(dib);
  
    DT_C((*writefunc),(ctl, arg,		/* output pallette					*/
      rgb, rgbsize));
    k += rgbsize;

	DT_MEMSET(pad, 0, sizeof(pad));
    for (j=nrows-1; j>=0; j--)			/* upside-down mapping within dib	*/
#ifdef NONO
    for (j=0; j<nrows; j++)				/* normal mapping within dib		*/
#endif
    {
	  void *p;
	  DTIMAGE_GETROW2(DT_C, ctl, image,
        y+j, &p);
      DT_C((*writefunc),(ctl, arg,		/* output raw row					*/
        p, image->rowsize));
      k += image->rowsize;
  	  if (DTDIB_PAD(image->rowsize))
  	  {
  	    DT_C((*writefunc),(ctl, arg,	/* output padding					*/
          pad,
          DTDIB_PAD(image->rowsize)));
		k += DTDIB_PAD(image->rowsize);
  	  }
	  DTIMAGE_PUTROW2(DT_C, ctl, image, 
        y+j);
	}
    y += nrows;
  }
	
  dt_dbg(ctl, F, DT_DBG_MASK_OBJECT,
    "k %ld after bits", k);

cleanup:

  if (rgb != NULL)
    DT_I(dtos_free2,(ctl, rgb,
      F, "pallette"));
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
