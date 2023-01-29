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
  CHANGES
  19991217 DE changed some long's to 4-byte int's in favor of 64-bit CPU
 *..........................................................................*/



#include <dtack/base.h>
DT_RCSID("source/high $RCSfile: wmf.c,v $ $Revision: 1.3 $");

#include <dtack/dib.h>
#include <dtack/wmf.h>
#include <dtack/str.h>
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

#define SET2(A, V) {unsigned short v = (V); DT_MEMCPY(&(A), &v, sizeof(v));}
#define SET4(A, V) {dtntype_b32_t  v = (V); DT_MEMCPY(&(A), &v, sizeof(v));}

/*..........................................................................
| NAME
| dtwmf_placeable_endian() - fix placeable header members endianness
| 
| SUMMARY
| Switch endianness of all placeable header members if on a big-endian machine.
| 
| END
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtwmf_placeable_endian(					/* fix windows metafile header		*/
  dt_ctl_t *ctl,
  dtwmf_placeable_t *placeable)
{
  if (IS_LITTLE_ENDIAN)					/* already little endian? 			*/
    return DT_RC_GOOD;

  FIX2(placeable->magic1);
  FIX2(placeable->magic2);
  FIX2(placeable->hmf);
  FIX2(placeable->bbox.left);
  FIX2(placeable->bbox.top);
  FIX2(placeable->bbox.right);
  FIX2(placeable->bbox.bottom);
  FIX2(placeable->inch);
  FIX2(placeable->reserved1);
  FIX2(placeable->reserved2);
  FIX2(placeable->checksum);

  return DT_RC_GOOD;
}

/*..........................................................................
| NAME
| dtwmf_fileheader_endian() - fix metafile header members endianness
| 
| SUMMARY
| Switch endianness of all metafile header members if on a big-endian machine.
| just does the file header part, not the first few metafile records.
| 
| END
 *..........................................................................*/

static
dt_rc_e
dtwmf_fileheader_endian(				/* fix windows metafile header		*/
  dt_ctl_t *ctl,
  dtwmf_fileheader_t *fileheader)
{
  if (IS_LITTLE_ENDIAN)					/* already little endian? 			*/
    return DT_RC_GOOD;
  FIX2(fileheader->mtType);
  FIX2(fileheader->mtHeaderSize);
  FIX2(fileheader->mtVersion);
  FIX4(fileheader->mtSize);
  FIX2(fileheader->mtNoObjects);
  FIX4(fileheader->mtMaxRecord);
  FIX2(fileheader->mtNoParameters);
  return DT_RC_GOOD;
}

/*..........................................................................
| NAME
| dtwmf_metahead_endian() - fix metafile header members endianness
| 
| SUMMARY
| Switch endianness of all metafile header members if on a big-endian machine.
| Does the file header and the standard records we write.
| 
| END
 *..........................................................................*/

static
dt_rc_e
dtwmf_metahead_endian(					/* fix windows metafile header		*/
  dt_ctl_t *ctl,
  dtwmf_metahead_t *metahead)
{
  if (IS_LITTLE_ENDIAN)					/* already little endian? 			*/
    return DT_RC_GOOD;

  dtwmf_fileheader_endian(ctl,			/* do the file header 				*/
    &metahead->fileheader);

  FIX4(metahead->savedc.rdSize);
  FIX2(metahead->savedc.rdFunction);

  FIX4(metahead->setwindoworg.rdSize);
  FIX2(metahead->setwindoworg.rdFunction);
  FIX2(metahead->setwindoworg.rdParm[0]);
  FIX2(metahead->setwindoworg.rdParm[1]);
  
  FIX4(metahead->setwindowext.rdSize);
  FIX2(metahead->setwindowext.rdFunction);
  FIX2(metahead->setwindowext.rdParm[0]);
  FIX2(metahead->setwindowext.rdParm[1]);
  
  FIX4(metahead->setstretchbltmode.rdSize);
  FIX2(metahead->setstretchbltmode.rdFunction);
  FIX2(metahead->setstretchbltmode.rdParm[0]);
  
  FIX4(metahead->intersectcliprect.rdSize);
  FIX2(metahead->intersectcliprect.rdFunction);
  FIX2(metahead->intersectcliprect.rdParm[0]);
  FIX2(metahead->intersectcliprect.rdParm[1]);
  FIX2(metahead->intersectcliprect.rdParm[2]);
  FIX2(metahead->intersectcliprect.rdParm[3]);

  FIX4(metahead->setpolyfillmode.rdSize);
  FIX2(metahead->setpolyfillmode.rdFunction);
  FIX2(metahead->setpolyfillmode.rdParm[0]);
    
  FIX4(metahead->setbkmode2.rdSize);
  FIX2(metahead->setbkmode2.rdFunction);
  FIX2(metahead->setbkmode2.rdParm[0]);
    
  return DT_RC_GOOD;
}

/*..........................................................................
| NAME
| dtwmf_metatail_endian() - fix metafile header members endianness
| 
| SUMMARY
| Switch endianness of all metafile header members if on a big-endian machine.
| Does the file header and the standard records we write.
| 
| END
 *..........................................................................*/

static
dt_rc_e
dtwmf_metatail_endian(					/* fix windows metafile header		*/
  dt_ctl_t *ctl,
  dtwmf_metatail_t *metatail)
{
  if (IS_LITTLE_ENDIAN)					/* already little endian? 			*/
    return DT_RC_GOOD;
    
  FIX4(metatail->setbkmode1.rdSize);
  FIX2(metatail->setbkmode1.rdFunction);
  FIX2(metatail->setbkmode1.rdParm[0]);
    
  FIX4(metatail->restoredc.rdSize);
  FIX2(metatail->restoredc.rdFunction);
  FIX2(metatail->restoredc.rdParm[0]);
    
  FIX4(metatail->terminate.rdSize);
  FIX2(metatail->terminate.rdFunction);
    
  return DT_RC_GOOD;
}

/*..........................................................................
| NAME
| dtwmf_stretchdib_endian() - fix stretchdib record members endianness
| 
| SUMMARY
| Switch endianness of all stretchdib record members if on a big-endian machine.
| 
| END
 *..........................................................................*/

static
dt_rc_e
dtwmf_stretchdib_endian(					/* fix windows metafile header		*/
  dt_ctl_t *ctl,
  dtwmf_stretchdib_t *stretchdib)
{
  int i;
  if (IS_LITTLE_ENDIAN)					/* already little endian? 			*/
    return DT_RC_GOOD;

  FIX4(stretchdib->rdSize);
  FIX2(stretchdib->rdFunction);
  FIX4(stretchdib->rdParm[0]);
  for (i=2; i<11; i++)
    FIX2(stretchdib->rdParm[i]);
  return DT_RC_GOOD;
}

/*..........................................................................
| NAME
| dtwmf_split() - calculate dib split parameters
| 
| SUMMARY
| Calculated number of dibs needed.
| Keep metafile record size under arbitrary 32000.
| 
| END
 *..........................................................................*/
static
dt_rc_e
dtwmf_split(
  dt_ctl_t *ctl,
  dtwmf_split_t *split,
  dtimage_t *image)
{
  long max;
  
  split->ncolors = 256;

  split->recordsize =
    sizeof(dtwmf_stretchdib_t) +		/* record size 						*/
    sizeof(dtdib_t) +					/* dib header 						*/
    sizeof(dtdib_rgb_t) *				/* pallette 						*/
      split->ncolors;

  max = 32000L - split->recordsize;		/* max space left for image data 	*/

  split->size = 
    (long)image->ye * (long)			/* total data size for image 		*/
    DTDIB_PADDED(image->rowsize);
  
  split->ndibs = 1 + (int)				/* number of dibs to write 			*/
    (split->size / max);
  
  split->rows1 = (dtxy_t)(max /			/* number of rows in first dib 		*/
    DTDIB_PADDED(image->rowsize));
  if (split->rows1 > image->ye)
    split->rows1 = image->ye;
  
  split->rows2 = image->ye -			/* number of rows in last dib 		*/
    (split->ndibs-1) * split->rows1;

  return DT_RC_GOOD;
}

/*..........................................................................
| NAME
| dtwmf_import() - import Microsoft Windows metafile
| 
| SUMMARY
| Read image from metafile.
| Allocate image with given size unless size is 0.
| Interpret the input sequentially.
| Use the given function to get new bytes.
| Create the image and populate it.
| 
| END
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtwmf_import(
  dt_ctl_t *ctl,
  dt_io_f *readfunc,
  void *arg,
  dtxy_t xe,							/* allocate image with this size 	*/
  dtxy_t ye,
  dtimage_t *image,
  unsigned int pagesize,
  unsigned int npages)
{
  DT_F("dtwmf_import");
  dtwmf_fileheader_t fileheader;
  dtntype_b32_t size;
# define PARMS (16)						/* maximum parms we want to save 	*/
  unsigned short parm[PARMS+1];
  int quit;
  dtntype_b32_t mask = DT_DBG_MASK_OBJECT;
  dt_rc_e rc;

  DT_C((readfunc),(ctl, arg,			/* read the file header 			*/
    &fileheader, sizeof(fileheader)));
  DT_C(dtwmf_fileheader_endian,(ctl,	/* fix endian-ness 					*/
    &fileheader));

  DT_MEMCPY(&size, &fileheader.mtSize,		/* extract long word 				*/
    sizeof(size));

  dt_dbg(ctl, F, mask,
    "header: mtType %u,"
    " mtHeaderSize %u,"
    " mtVersion 0x%04x,"
    " mtSize %ld",
    fileheader.mtType,
    fileheader.mtHeaderSize,
    fileheader.mtVersion,
	size,
    fileheader.mtNoObjects);

  if (fileheader.mtType != 1 &&		/* check metafile header type 		*/
	  fileheader.mtType != 2)
  {
	rc = dt_err(ctl, F,
      "metafile is corrupt"
      " (mtType %d, expected 1 or 2)",
      fileheader.mtType);
	goto cleanup;
  }

  if (fileheader.mtVersion != 0x0300)	/* check metafile header version 	*/
  {
	rc = dt_err(ctl, F,
      "metafile is corrupt"
      " (mtVersion 0x%04x, expected 0x0300)",
      fileheader.mtVersion);
	goto cleanup;
  }
  
  quit = 0;
  while (quit == 0)						/* loop for each input record 		*/
  {
	dtwmf_record_t record;
    long n, i;
	char s[40];
	char name[40];

    DT_C((readfunc),(ctl, arg,			/* read the record description 		*/
      &record, sizeof(record)-2));
    if (!IS_LITTLE_ENDIAN)				/* not already little endian? 		*/
	{
      FIX4(record.rdSize);				/* fix endian-ness 					*/
      FIX2(record.rdFunction);
	}

    n = record.rdSize - 3;				/* number of params 				*/
	
	if (n < 0)
	{
	  dt_err(ctl, F, 
        "metafile is corrupt,"
        " bad rdSize %ld", 
        record.rdSize);
	  goto cleanup;
	}

    if (record.rdFunction ==			/* this is an image? 				*/
		DTWMF_STRETCHDIB)
	  n = 11;							/* 11 params before the dib 		*/

	s[0] = '\0';
	s[1] = '\0';
	for (i=0; i<n; i++)
	{
	  int k = (int)(i<PARMS? i: PARMS);
	  DT_C((readfunc),(ctl, arg,		/* get param input 					*/
        &parm[k], sizeof(*parm)));
	  if (!IS_LITTLE_ENDIAN)			/* not already little endian? 		*/
	  {
        FIX2(parm[k]);					/* fix endian-ness 					*/
	  }
	  if (DT_DBG(ctl, mask) &&
		  i < 12)
  	    dtstr_printf(ctl,				/* make printable string of parms 	*/
          s+strlen(s), 
          sizeof(s)-strlen(s),
          " %d", parm[k]);
	}

	if (DT_DBG(ctl, mask))
	{
  	  DT_C(dtwmf_metaname,(ctl,			/* make string name for function 	*/
        record.rdFunction,
        name, sizeof(name)));

      dt_dbg(ctl, F, mask,
        "record: rdSize %ld,"
        " %s (%s)",
        record.rdSize, 
        name, s+1);
	}

    if (record.rdFunction ==			/* termination record?				*/
		DTWMF_TERMINATE)
	  quit = 1;
	else
	if (record.rdFunction == 0xffff)	/* invalid record? 					*/
	{
	  dt_err(ctl, F, 
        "metafile is corrupt,"
        " bad function code %d", 
        record.rdFunction);
	  goto cleanup;
	}
	else
    if (record.rdFunction ==			/* this is our image? 				*/
		DTWMF_STRETCHDIB)
	{
	  dtdib_t dib;
	  DT_C((readfunc),(ctl, arg,		/* get dib header					*/
        &dib, sizeof(dib)));
	  DT_C(dtdib_endian,(ctl, &dib));	/* fix endianness 					*/
	  DT_C(dtdib_import,(ctl,			/* import image from dib 			*/
        &dib,
        readfunc, arg,
        parm[9],						/* start reading at this row 		*/
        1,								/* reverse rows 					*/
        xe, ye,							/* use this size to allocate image 	*/
        image, pagesize, npages));
	}
  } 

cleanup:

  return rc;
}

/*..........................................................................
| NAME
| dtwmf_placeable_init() - initialize Microsoft Windows metafile header
| 
| SUMMARY
| Fill in members of structure presuming we are writing an image metafile.
| 
| END
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtwmf_placeable_init(					/* create makefile placeable header	*/
  dt_ctl_t *ctl,
  dtwmf_placeable_t *placeable,
  dtimage_t *image)
{
  DT_MEMSET(placeable, 0, 
    sizeof(*placeable));
  placeable->magic1 =           DTWMF_PLACEABLE_MAGIC1;
  placeable->magic2 =           DTWMF_PLACEABLE_MAGIC2; 
  placeable->hmf =              0;	
  placeable->bbox.left =        0;
  placeable->bbox.top =         0;
  placeable->bbox.right =       image->xe;
  placeable->bbox.bottom =      image->ye;
  placeable->inch =             0x005f;
  placeable->reserved1 =        0;
  placeable->reserved2 =        0;
  placeable->checksum =         0;
  return DT_RC_GOOD;
}

/*..........................................................................
| NAME
| dtwmf_metahead_init() - initialize Microsoft Windows metafile header
| 
| SUMMARY
| Fill all members of header.
| Also initialize the metafile records contained in the header.
| For size number, account for the number of dibs we will be writing.
| All members are initialized in standard byte order (little-endian).
| 
| END
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtwmf_metahead_init(					/* init metafile header				*/
  dt_ctl_t *ctl,
  dtwmf_metahead_t *metahead,
  dtimage_t *image)
{
  DT_F("dtwmf_metahead_init");
  dtwmf_split_t split;
  long mtsize;
  long maxrecord;

  DT_Q(dtwmf_split,(ctl, &split,
    image));

  mtsize = 
    sizeof(dtwmf_metahead_t) +			/* size of metafile header 			*/
	split.ndibs * split.recordsize +	/* size of the dib records 			*/
	split.size +						/* size of all the data 			*/
	sizeof(dtwmf_metatail_t);			/* size of trailing records 		*/
  
  maxrecord = split.recordsize +		/* size of the stretch record 		*/
    split.rows1 *						/* number of rows in first dib 		*/
    DTDIB_PADDED(image->rowsize);		/* size of each row 				*/

  DT_MEMSET(metahead, 0, 
    sizeof(*metahead));

  SET2(metahead->fileheader.mtType        , (1));
  SET2(metahead->fileheader.mtHeaderSize  , (sizeof(dtwmf_fileheader_t) / 2));
  SET2(metahead->fileheader.mtVersion     , (0x0300));
  SET4(metahead->fileheader.mtSize        , (mtsize / 2));
  SET2(metahead->fileheader.mtNoObjects   , (0));
  SET4(metahead->fileheader.mtMaxRecord   , (maxrecord / 2));
  SET2(metahead->fileheader.mtNoParameters, (0));

  SET2(metahead->savedc.rdFunction       , (DTWMF_SAVEDC));
  SET4(metahead->savedc.rdSize           , (3));

  SET2(metahead->setwindoworg.rdFunction , (DTWMF_SETWINDOWORG));

  SET4(metahead->setwindoworg.rdSize     , (5));
  SET2(metahead->setwindoworg.rdParm[0]  , (0));
  SET2(metahead->setwindoworg.rdParm[1]  , (0));

  SET2(metahead->setwindowext.rdFunction , (DTWMF_SETWINDOWEXT));
  SET4(metahead->setwindowext.rdSize     , (5));
  SET2(metahead->setwindowext.rdParm[0]  , (image->ye-1));
  SET2(metahead->setwindowext.rdParm[1]  , (image->xe-1));

  SET2(metahead->setstretchbltmode.rdFunction , (DTWMF_SETSTRETCHBLTMODE));
  SET4(metahead->setstretchbltmode.rdSize     , (4));
  SET2(metahead->setstretchbltmode.rdParm[0]  , (DTWMF_COLORONCOLOR));

  SET2(metahead->intersectcliprect.rdFunction , (DTWMF_INTERSECTCLIPRECT));
  SET4(metahead->intersectcliprect.rdSize     , (7));
  SET2(metahead->intersectcliprect.rdParm[0]  , (image->ye-1));
  SET2(metahead->intersectcliprect.rdParm[1]  , (image->xe-1));
  SET2(metahead->intersectcliprect.rdParm[2]  , (0));
  SET2(metahead->intersectcliprect.rdParm[3]  , (0));

  SET2(metahead->setpolyfillmode.rdFunction , (DTWMF_SETPOLYFILLMODE));
  SET4(metahead->setpolyfillmode.rdSize     , (4));
  SET2(metahead->setpolyfillmode.rdParm[0]  , (1));

  SET2(metahead->setbkmode2.rdFunction , (DTWMF_SETBKMODE));
  SET4(metahead->setbkmode2.rdSize     , (4));
  SET2(metahead->setbkmode2.rdParm[0]  , (2));

  return DT_RC_GOOD;
}

/*..........................................................................
| NAME
| dtwmf_metatail_init() - initialize Microsoft Windows metafile tail records
| 
| SUMMARY
| Fill all members of tail records.
| All members are initialized in standard byte order (little-endian).
| 
| END
 *..........................................................................*/

static
dt_rc_e
dtwmf_metatail_init(					/* create windows metafile header	*/
  dt_ctl_t *ctl,
  dtwmf_metatail_t *metatail,
  dtimage_t *image)
{
  DT_MEMSET(metatail, 0, 
    sizeof(*metatail));

  SET2(metatail->setbkmode1.rdFunction , (DTWMF_SETBKMODE));
  SET4(metatail->setbkmode1.rdSize     , (4));
  SET2(metatail->setbkmode1.rdParm[0]  , (1));

  SET2(metatail->restoredc.rdFunction , (DTWMF_RESTOREDC));
  SET4(metatail->restoredc.rdSize     , (4));
  SET2(metatail->restoredc.rdParm[0]  , (0xffff));

  SET2(metatail->terminate.rdFunction , (0));
  SET4(metatail->terminate.rdSize     , (3));

  return DT_RC_GOOD;
}

/*..........................................................................
| NAME
| dtwmf_stretchdib_init() - initialize metafile stretchdib record
| 
| SUMMARY
| Fill all members of record.
| Presume we are writing only ~ye~ rows starting at ~y0~.
| All members are initialized in standard byte order (little-endian).
| 
| END
 *..........................................................................*/

static
dt_rc_e
dtwmf_stretchdib_init(					/* create stretchdib record			*/
  dt_ctl_t *ctl,
  dtwmf_stretchdib_t *stretchdib,
  dtxy_t y0,
  dtxy_t ye,
  dtimage_t *image)
{
  DT_F("dtwmf_stretchdib_init");
  dtwmf_split_t split;
  long dibsize;

  DT_Q(dtwmf_split,(ctl, &split,
    image));

  if (ye == 0)							/* caller wants to write it all? 	*/
    ye = image->ye;

  dibsize = split.recordsize +			/* size of record header 			*/
    (long)ye * (long)					/* size of data of block 			*/
    DTDIB_PADDED(image->rowsize);

  DT_MEMSET(stretchdib, 0, 
    sizeof(*stretchdib));

  SET2(stretchdib->rdFunction , (DTWMF_STRETCHDIB));
  SET4(stretchdib->rdSize     , (dibsize / 2));
  SET4(stretchdib->rdParm[0]  , (DTWMF_SRCCOPY));
  SET2(stretchdib->rdParm[2]  , (0));           /* flags         */
  SET2(stretchdib->rdParm[3]  , (ye));          /* src height    */
  SET2(stretchdib->rdParm[4]  , (image->xe));   /* src width     */
  SET2(stretchdib->rdParm[5]  , (0));           /* src y         */
  SET2(stretchdib->rdParm[6]  , (0));          	/* src x         */
  SET2(stretchdib->rdParm[7]  , (ye));          /* dest height   */
  SET2(stretchdib->rdParm[8]  , (image->xe));   /* dest width    */
  SET2(stretchdib->rdParm[9]  , (y0));          /* dest y        */
  SET2(stretchdib->rdParm[10] , (0));           /* dest x        */
  
  return DT_RC_GOOD;
}

/*..........................................................................
| NAME
| dtwmf_export() - export Microsoft Windows metafile
| 
| SUMMARY
| Do not write the Adobe placeable header.
| Use the given function to output bytes.
| 
| END
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtwmf_export(
  dt_ctl_t *ctl,
  dt_io_f *writefunc,
  void *arg,
  dtimage_t *image,
  dtxy_t xpos,
  dtxy_t ypos)
{
  DT_F("dtwmf_export");
  dtwmf_metahead_t metahead;
  long k = 0;
  dtwmf_split_t split;
  int rgbsize;
  dtdib_rgb_t *rgb = NULL;
  char pad[DTDIB_PADMAX];
  int i, j;
  dtxy_t y;
  dtwmf_stretchdib_t stretchdib;
  dtdib_t dib;
  dtwmf_metatail_t metatail;
  dt_rc_e rc;

  rc = dtimage_check_1f(ctl,			/* check image validity			    */
    image, "metafile image", NULL,
    DTIMAGE_FLAG_PAGED, F);
  if (rc != DT_RC_GOOD)
    return rc;
  
  DT_C(dtwmf_split,(ctl,
    &split, image));

  dt_dbg(ctl, F, DT_DBG_MASK_OBJECT,
    "split size %ld, rows1 %u, rows2 %u",
    split.size, split.rows1, split.rows2);
  dt_dbg(ctl, F, DT_DBG_MASK_OBJECT,
	"split ncolors %d, ndibs %d, recordsize %d",
    split.ncolors, split.ndibs,
    split.recordsize);


										/* ................................ */
  DT_C(dtwmf_metahead_init,(ctl,		/* init metafile header buffer 		*/
	&metahead, image));						
  
  DT_C(dtwmf_metahead_endian,(ctl,		/* fix endian-ness					*/
	&metahead));

  DT_C((*writefunc),(ctl, arg,			/* output metafile header			*/
    &metahead, sizeof(metahead)));

  k += sizeof(metahead);
  dt_dbg(ctl, F, DT_DBG_MASK_OBJECT,
    "k %ld after metahead", k);

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

    DT_C(dtwmf_stretchdib_init,(ctl,	/* initialize stretchdib record		*/
      &stretchdib, y, nrows, image));
	
    DT_C(dtwmf_stretchdib_endian,(ctl,	/* fix endian-ness					*/
	  &stretchdib));
    
    DT_C((*writefunc),(ctl, arg,		/* output stretchdib record			*/
      &stretchdib, 
      sizeof(stretchdib)));
    k += sizeof(stretchdib);
	
    dt_dbg(ctl, F, DT_DBG_MASK_OBJECT,
      "k %ld at stretch y %u-%u", 
      k, y, y+nrows);
								
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
    "k %ld before metatail", k);

  DT_C(dtwmf_metatail_init,(ctl,		/* init metafile tail buffer 		*/
	&metatail, image));
    
  DT_C(dtwmf_metatail_endian,(ctl,		/* fix endian-ness					*/
	&metatail));

  DT_C((*writefunc),(ctl, arg,			/* output tail						*/
    &metatail, sizeof(metatail)));
  k += sizeof(metatail);
	
  dt_dbg(ctl, F, DT_DBG_MASK_OBJECT,
    "k %ld after metatail", k);

cleanup:

  if (rgb != NULL)
    DT_I(dtos_free2,(ctl, rgb,
      F, "pallette"));
  return rc;
}

/*..........................................................................
 * name for metafile element
 *..........................................................................*/

dt_rc_e 
DTCONFIG_API1
dtwmf_metaname(
  dt_ctl_t *ctl,
  int code,
  char *string,
  int lmax)
{
  DT_F("dtwmf_metaname");
  char *s;
  static char unknown[32];
  switch (code)
  {
    case DTWMF_SETBKCOLOR: s = "SETBKCOLOR"; break;
    case DTWMF_SETBKMODE: s = "SETBKMODE"; break;
    case DTWMF_SETMAPMODE: s = "SETMAPMODE"; break;
    case DTWMF_SETROP2: s = "SETROP2"; break;
    case DTWMF_SETRELABS: s = "SETRELABS"; break;
    case DTWMF_SETPOLYFILLMODE: s = "SETPOLYFILLMODE"; break;
    case DTWMF_SETSTRETCHBLTMODE: s = "SETSTRETCHBLTMODE"; break;
    case DTWMF_SETTEXTCHAREXTRA: s = "SETTEXTCHAREXTRA"; break;
    case DTWMF_SETTEXTCOLOR: s = "SETTEXTCOLOR"; break;
    case DTWMF_SETTEXTJUSTIFICATION: s = "SETTEXTJUSTIFICATION"; break;
    case DTWMF_SETWINDOWORG: s = "SETWINDOWORG"; break;
    case DTWMF_SETWINDOWEXT: s = "SETWINDOWEXT"; break;
    case DTWMF_SETVIEWPORTORG: s = "SETVIEWPORTORG"; break;
    case DTWMF_SETVIEWPORTEXT: s = "SETVIEWPORTEXT"; break;
    case DTWMF_OFFSETWINDOWORG: s = "OFFSETWINDOWORG"; break;
    case DTWMF_SCALEWINDOWEXT: s = "SCALEWINDOWEXT"; break;
    case DTWMF_OFFSETVIEWPORTORG: s = "OFFSETVIEWPORTORG"; break;
    case DTWMF_SCALEVIEWPORTEXT: s = "SCALEVIEWPORTEXT"; break;
    case DTWMF_LINETO: s = "LINETO"; break;
    case DTWMF_MOVETO: s = "MOVETO"; break;
    case DTWMF_EXCLUDECLIPRECT: s = "EXCLUDECLIPRECT"; break;
    case DTWMF_INTERSECTCLIPRECT: s = "INTERSECTCLIPRECT"; break;
    case DTWMF_ARC: s = "ARC"; break;
    case DTWMF_ELLIPSE: s = "ELLIPSE"; break;
    case DTWMF_FLOODFILL: s = "FLOODFILL"; break;
    case DTWMF_PIE: s = "PIE"; break;
    case DTWMF_RECTANGLE: s = "RECTANGLE"; break;
    case DTWMF_ROUNDRECT: s = "ROUNDRECT"; break;
    case DTWMF_PATBLT: s = "PATBLT"; break;
    case DTWMF_SAVEDC: s = "SAVEDC"; break;
    case DTWMF_SETPIXEL: s = "SETPIXEL"; break;
    case DTWMF_OFFSETCLIPRGN: s = "OFFSETCLIPRGN"; break;
    case DTWMF_TEXTOUT: s = "TEXTOUT"; break;
    case DTWMF_BITBLT: s = "BITBLT"; break;
    case DTWMF_STRETCHBLT: s = "STRETCHBLT"; break;
    case DTWMF_POLYGON: s = "POLYGON"; break;
    case DTWMF_POLYLINE: s = "POLYLINE"; break;
    case DTWMF_ESCAPE: s = "ESCAPE"; break;
    case DTWMF_RESTOREDC: s = "RESTOREDC"; break;
    case DTWMF_FILLREGION: s = "FILLREGION"; break;
    case DTWMF_FRAMEREGION: s = "FRAMEREGION"; break;
    case DTWMF_INVERTREGION: s = "INVERTREGION"; break;
    case DTWMF_PAINTREGION: s = "PAINTREGION"; break;
    case DTWMF_SELECTCLIPREGION: s = "SELECTCLIPREGION"; break;
    case DTWMF_SELECTOBJECT: s = "SELECTOBJECT"; break;
    case DTWMF_SETTEXTALIGN: s = "SETTEXTALIGN"; break;
    case DTWMF_DRAWTEXT: s = "DRAWTEXT"; break;
    case DTWMF_CHORD: s = "CHORD"; break;
    case DTWMF_SETMAPPERFLAGS: s = "SETMAPPERFLAGS"; break;
    case DTWMF_EXTTEXTOUT: s = "EXTTEXTOUT"; break;
    case DTWMF_SETDIBTODEV: s = "SETDIBTODEV"; break;
    case DTWMF_SELECTPALETTE: s = "SELECTPALETTE"; break;
    case DTWMF_REALIZEPALETTE: s = "REALIZEPALETTE"; break;
    case DTWMF_ANIMATEPALETTE: s = "ANIMATEPALETTE"; break;
    case DTWMF_SETPALENTRIES: s = "SETPALENTRIES"; break;
    case DTWMF_POLYPOLYGON: s = "POLYPOLYGON"; break;
    case DTWMF_RESIZEPALETTE: s = "RESIZEPALETTE"; break;
    case DTWMF_DIBBITBLT: s = "DIBBITBLT"; break;
    case DTWMF_DIBSTRETCHBLT: s = "DIBSTRETCHBLT"; break;
    case DTWMF_DIBCREATEPATTERNBRUSH: s = "DIBCREATEPATTERNBRUSH"; break;
    case DTWMF_STRETCHDIB: s = "STRETCHDIB"; break;
    case DTWMF_EXTFLOODFILL: s = "EXTFLOODFILL"; break;
    case DTWMF_RESETDC: s = "RESETDC"; break;
    case DTWMF_STARTDOC: s = "STARTDOC"; break;
    case DTWMF_STARTPAGE: s = "STARTPAGE"; break;
    case DTWMF_ENDPAGE: s = "ENDPAGE"; break;
    case DTWMF_ABORTDOC: s = "ABORTDOC"; break;
    case DTWMF_ENDDOC: s = "ENDDOC"; break;
    case DTWMF_DELETEOBJECT: s = "DELETEOBJECT"; break;
    case DTWMF_CREATEPALETTE: s = "CREATEPALETTE"; break;
    case DTWMF_CREATEBRUSH: s = "CREATEBRUSH"; break;
    case DTWMF_CREATEPATTERNBRUSH: s = "CREATEPATTERNBRUSH"; break;
    case DTWMF_CREATEPENINDIRECT: s = "CREATEPENINDIRECT"; break;
    case DTWMF_CREATEFONTINDIRECT: s = "CREATEFONTINDIRECT"; break;
    case DTWMF_CREATEBRUSHINDIRECT: s = "CREATEBRUSHINDIRECT"; break;
    case DTWMF_CREATEBITMAPINDIRECT: s = "CREATEBITMAPINDIRECT"; break;
    case DTWMF_CREATEBITMAP: s = "CREATEBITMAP"; break;
    case DTWMF_CREATEREGION: s = "CREATEREGION"; break;
    case DTWMF_TERMINATE: s = "TERMINATE"; break;
    default:
      sprintf(unknown, "meta 0x%04x", code);
	  s = unknown;
  }
  DT_Q(dtstr_printf,(ctl, string, lmax,
    "%s", s));
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
