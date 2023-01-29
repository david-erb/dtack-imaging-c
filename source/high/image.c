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
  19981219 DE changed error message
  19980924 DE added FLAG_OVERLAY
 *..........................................................................*/



#include <dtack/base.h>
#include <dtack/mem.h>
#include <dtack/os.h>

DT_RCSID("source/high $RCSfile: image.c,v $ $Revision: 1.1 $");

#define DESC_ROWPTRS "image row pointers"

/*..........................................................................
| NAME
| dtimage_dup() - duplicate image
| 
| SUMMARY
| Create a new image having its own space the same dimensions
| as the original image.
| Make the contents of the new image identical to the original.
| 
| END
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtimage_dup(
  dt_ctl_t *ctl,						/* environment control 				*/
  dtimage_t *image,						/* original image					*/
  dtimage_t *dup)						/* new image 						*/
{
  DT_F("dtimage_dup");
  dt_rc_e rc;

  DT_C(dtimage_create,(ctl, dup,		/* create space for dup image 		*/
    image->xe, image->ye,
    image->ntype));

  DT_C(dtimage_cast,(ctl, image,
    dup));
  
#ifdef NONO
  DT_C(dtmem_copy,(ctl,					/* copy byte for byte 				*/
    image->memalloc, dup->memalloc));
#endif

cleanup:
  if (rc != DT_RC_GOOD)
  DT_I(dtimage_free,(ctl, dup));
  return rc;
}

/*..........................................................................
| NAME
| dtimage_free() - free image space
| 
| SUMMARY
| Release memory allocated for image.
| Image may have been allocated by dtimage_create(), dtimage_dup(),
| dtimage_overlay(), or dtimage_import().
| Do not free the data structure itself, but make its contents invalid.
| 
| END
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtimage_free(							/* free image data space			*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtimage_t *image)						/* image to free					*/
{
  DT_F("dtimage_free");
  if (image->xe &&						/* this image is really allocated?	*/
      image->ye &&
      image->row &&
      image->memalloc)
  {
    if (image->flags &					/* we allocated the pixel space?    */
        DTIMAGE_FLAG_FREE)
    {
      DT_Q(dtmem_free,(ctl,				/* free it through memsource	    */
        image->memalloc,
        F, "image"));
    }
#ifdef NONO
	dtmemalloc_t *memalloc;
	memalloc = (dtmemalloc_t *)
      image->memalloc;
	if (memalloc->links < 1)			/* no one else has it linked? 		*/
#endif
      DT_Q(dtos_free2,(ctl, image->row,	/* free the row pointer space		*/
        F, DESC_ROWPTRS));
  }

  DT_MEMSET(image, 0, sizeof(*image));		/* clear the structure			    */

  return DT_RC_GOOD;
}

/*..........................................................................
| NAME
| dtimage_overlay() - create a new image on top of an existing one
| 
| SUMMARY
| Create in image data structure which references all or part
| of the image memory of another image.
| Arrange the pointers such that the two images share pixel memory.
| Flag the overlayed image structure such that the pixel memory is not freed
| when it is passed to dtimage_free().
| Mark the overlayed image as having non-contiguous rows if it
| does not start in column 0 or have the same width.
| (Images with non-contiguous rows suffer performance penalties
| in some operations.)
| Have an error if any part of the overlay image lies outside
| the boundaries of the underlying image.
| 
| END
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtimage_overlay(						/* overlay new image on old memory	*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtimage_t *image,						/* underlying image					*/
  dtimage_t *overlay,					/* image structure to initialize 	*/
  dtxy_t x0,							/* starting column 					*/
  dtxy_t y0,							/* starting row 					*/
  dtxy_t xe,							/* overlayed image width 			*/
  dtxy_t ye)							/* overlayed image height 			*/
{
  DT_F("dtimage_overlay");
  dtxy_t y;
  dtxy_t b0;
  dt_rc_e rc;

  rc = dtimage_check_1f(ctl,			/* check simple stuff				*/
    image, "image",
    NULL, DTIMAGE_FLAG_PAGED, F);
  if (rc != DT_RC_GOOD)
    return rc;

  if (image->ntype == DT_NTYPE_B01 &&
      (xe & 0x07) != 0)
    return dt_err(ctl, F,
      "bit image width %ld not multiple of 8",
      xe);

  if (xe == 0 || ye == 0)
    return dt_err(ctl, F,
      "overlay size [%u %u] is invalid",
      xe, ye);

  if (x0+xe > image->xe)
    return dt_err(ctl, F,
      "overlay x0 %ld plus xe %ld > base xe %ld",
      x0, xe, image->xe);

  if (y0+ye > image->ye)
    return dt_err(ctl, F,
      "overlay y0 %ld plus ye %ld > base ye %ld",
      y0, ye, image->ye);

  DT_MEMSET(overlay, 0, sizeof(*overlay));	/* clear image structure 			*/
  
  DT_Q(dtos_malloc2,(ctl,				/* space for the row pointers	    */
    (void **)&overlay->row,
    ye * sizeof(*overlay->row),
    F, DESC_ROWPTRS));

  b0 = (dtxy_t)(x0 *					/* byte offset					    */
    dt_ntype_sizeof[image->ntype]);

  for (y=0; y<ye; y++)					/* assign new row pointers		    */
    overlay->row[y].dchar =
      image->row[y0+y].dchar + b0;

  overlay->xe = xe;
  overlay->ye = ye;
  overlay->ntype = image->ntype;
  overlay->rowsize = (unsigned int)		/* size of one row in bytes		    */
    (xe * 
     dt_ntype_sizeof[image->ntype]);
  
  overlay->flags = 0;
  if (image->flags &					/* original image was paged?	    */
      DTIMAGE_FLAG_PAGED)
    overlay->flags |=					/* then so is this one			    */
      DTIMAGE_FLAG_PAGED;

  if (image->flags &					/* original image was segmented?    */
      DTIMAGE_FLAG_SEGMENTED)
    overlay->flags |=					/* then so is this one			    */
      DTIMAGE_FLAG_SEGMENTED;

  if ((image->flags &					/* original image was contiguous?	*/
       DTIMAGE_FLAG_CONTIG) &&
      x0 == 0 &&						/* and overlay covers all width?	*/
      xe == image->xe)
    overlay->flags |=					/* then this one is contiguous		*/
      DTIMAGE_FLAG_CONTIG;

  overlay->flags |=						/* mark it overlayed 				*/
    DTIMAGE_FLAG_OVERLAY;
  
  dt_dbg(ctl, F, DT_DBG_MASK_IMG,
    "overlay is %spaged, %ssegmented, and %scontiguous",
    overlay->flags & DTIMAGE_FLAG_PAGED? "": "not ",
    overlay->flags & DTIMAGE_FLAG_SEGMENTED? "": "not ",
    overlay->flags & DTIMAGE_FLAG_CONTIG? "": "not ");

  overlay->memalloc = image->memalloc;	/* this image shares memory			*/
  return DT_RC_GOOD;
}

/*..........................................................................
| NAME
| dtimage_create3() - create and allocate image
| 
| SUMMARY
| Create a new image description in the given data structure.
| Allocate space for the pixels, but not for the data structure itself.
| Have an error if ~ntype~ is not one of the known types.
| Allow caller to specify big memory form factor,
| but choose reasonable defaults for ~pagesize~ when it is 0
| and for ~npages~ when it is 0.
| Allow the caller to specify a pre-allocated big memory block.
| In this case, flag the data structure such that the pixel memory 
| is not freed by dtimage_free().
| If ~pre_memalloc~ is NULL, have an error if no big memory source has at least
| ~width~ * ~height * pixel_size bytes, where pixel_size depends on the ntype.
| In all cases, allocate the row pointer tables in malloc() memory.
| 
| END
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtimage_create3(						/* create and allocate image		*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtimage_t *image,						/* image to create					*/
  unsigned int pagesize,				/* big memory page size 			*/
  unsigned int npages,					/* big memory access buffers needed */
  long width,							/* image width in columns			*/
  long height,							/* image height in rows 			*/
  dt_ntype_e ntype,						/* image pixel data type 			*/
  void *pre_memalloc)					/* pre-allocated data, if any	    */
{
  DT_F("dtimage_create3");
  dtmemalloc_t *memalloc;
  unsigned int rowsize;
  dtxy_t xe, ye, y;
  int paging_allowed;
  dt_rc_e rc;

#if DTPRJ_NOPAGING == DT_1				/* paging not allowed? 				*/
  paging_allowed = 0;
# else
  paging_allowed = 1;
# endif

  if (ntype == DT_NTYPE_B01 &&
      (width & 0x07) != 0)
    return dt_err(ctl, F,
      "bit image width %ld not multiple of 8",
      width);

  if (width > DTIMAGE_SEG_MAXSIZE)
    return dt_err(ctl, F,
      "image size (%lu,%lu) exceeds maximum width %lu",
      width, height, DTIMAGE_SEG_MAXSIZE);

  if (height*sizeof(*image->row) >
      DTCONFIG_MALLOC_MAX)
    return dt_err(ctl, F,
      "image size (%lu,%lu) exceeds maximum height %lu",
      width, height, DTIMAGE_SEG_MAXSIZE);

  rowsize = (unsigned int)(width *		/* size of one row in bytes		    */
    dt_ntype_sizeof[ntype]);
  xe = (dtxy_t)width;
  ye = (dtxy_t)height;

  image->row = NULL;		
  image->memalloc = NULL;		
  image->flags = 0;

  DT_Q(dtos_malloc2,(ctl,				/* space for the row pointers	    */
    (void **)&image->row,
    height * sizeof(*image->row),
    F, DESC_ROWPTRS));
			
  if (pre_memalloc != NULL)				/* caller is giving pixel space?    */
  {
    image->memalloc = pre_memalloc;
    image->flags &= ~DTIMAGE_FLAG_FREE;	/* don't free this space		    */
    paging_allowed = 0;					/* can't page preallocated space    */
  }
  else									/* we have to allocate the space?   */
  {
    DT_C(dtmem_alloc,(ctl,				/* allocate space				    */
      &image->memalloc,
      pagesize, npages,
      rowsize, ye, F, "image"));
    image->flags |= DTIMAGE_FLAG_FREE;	/* free this space when done	    */
  }

  memalloc = (dtmemalloc_t *)			/* overcome the opaqueness		    */
      image->memalloc;		

  if (memalloc->flags &					/* image will not fit in pages?		*/
      DTMEMALLOC_FLAG_PAGED)
  {
    if (!paging_allowed)				/* are we allowed to page?		    */
    {
	  rc = dt_err(ctl, F,
        "image size (%u,%u) would require paging on \"%s\"",
        xe, ye,
        memalloc->mem.name);
      goto cleanup;
	}
    else								/* paging is allowed?			    */
	{
      long vlong = 0;
      image->row[0].vlong = vlong;		/* compute offsets only			    */
      for (y=1; y<ye; y++)
        image->row[y].vlong = 
          (vlong += rowsize);
      image->flags |=					/* mark image as paged			    */
        DTIMAGE_FLAG_PAGED;
	}
  }
  else									/* entire image fits in memory?	    */
  {
    long vlong = 0;
    for (y=0; y<ye; y++)				/* for each row					    */
	{
	  DT_Q(memalloc->map,(ctl,			/* make sure the row is mapped 		*/
        (struct dtmemalloc_t *)
          image->memalloc, vlong,
        &image->row[y].dvoid,
        NULL));
      vlong += rowsize;					/* next row offset				    */
    }
  }

  if (memalloc->flags &					/* memory is segmented?				*/
      DTMEMALLOC_FLAG_SEGMENTED)
    image->flags |=						/* mark image as segmented		    */
      DTIMAGE_FLAG_SEGMENTED;


  image->flags |= DTIMAGE_FLAG_CONTIG;	/* rows are stored contiguously		*/
  image->rowsize = rowsize;
  image->xe = (dtxy_t)width;			/* set up image members				*/
  image->ye = (dtxy_t)height;
  image->ntype = ntype;

  DT_Q(dtimage_dbg_op,(ctl, image,		/* trace execution				    */
    NULL, F));
  
  return DT_RC_GOOD;

cleanup:
  if (image->row)
    dtos_free2(ctl, image->row,			/* free space for the row pointers	*/
    F, DESC_ROWPTRS);
  if (image->memalloc)					/* free image data space		    */
    dtmem_free(ctl, image->memalloc,
      F, "image");
  return rc;
}

/*..........................................................................
| NAME
| dtimage_create2() - create and allocate image
| 
| SUMMARY
| Call dtimage_create3() with ~pagesize~ and ~npages~ both set to 0.
| Allows no control over big memory form factor.
| 
| END
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtimage_create2(						/* create and allocate image		*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtimage_t *image,						/* image to create					*/
  long width,							/* image width in columns			*/
  long height,							/* image height in rows 			*/
  dt_ntype_e ntype,						/* image pixel data type 			*/
  void *memalloc)						/* pre-allocated data, if any	    */
{
  DT_F("dtimage_create2");
  DT_Q(dtimage_create3,(ctl, image,
    0, 0, width, height, ntype, 
    memalloc));
  return DT_RC_GOOD;
}

/*..........................................................................
| NAME
| dtimage_create() - create and allocate image
| 
| SUMMARY
| Call dtimage_create3() with ~pagesize~ and ~npages~ both set to 0,
| and ~pre_memalloc~ set to NULL.
| Always allocates new pixel space.
| 
| END
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtimage_create(							/* create and allocate image		*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtimage_t *image,						/* image to create					*/
  long width,							/* image width in columns			*/
  long height,							/* image height in rows 			*/
  dt_ntype_e ntype)						/* image pixel data type 			*/
{
  DT_F("dtimage_create");
  DT_Q(dtimage_create3,(ctl, image,
    0, 0, width, height, ntype, NULL));
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
