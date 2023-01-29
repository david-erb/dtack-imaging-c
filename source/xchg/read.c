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
  19980923 DE added rz to automatically scan for gzipped input files
 *..........................................................................*/



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
DT_RCSID("source/high $RCSfile: read.c,v $ $Revision: 1.1 $");

#include <dtack/mem.h>
#include <dtack/pnm.h>
#include <dtack/dib.h>
#include <dtack/bmp.h>
#include <dtack/wmf.h>

extern
dt_rc_e
DTCONFIG_API1
dtimage_read_rad_fd(					/* get image data from opened fd	*/
  dt_ctl_t *ctl,
  dtimage_t *image,
  dtfd_t *fd);

/*..........................................................................
 *..........................................................................*/

static
dt_rc_e
dtimage_read_pnm_ascii_fd(				/* get ascii image from open fd		*/
  dt_ctl_t *ctl,
  dtimage_t *image,
  dtpnm_t *pnm,
  dtfd_t *fd)
{
  DT_F("dtimage_pnm_ascii_fd");
  dtxy_t y, x;
  double val;

  for (y=0; y<image->ye; y++)
  {
    for (x=0; x<image->xe; x++)			/* for each pixel in the row	    */
	{
      DT_Q(dtpnm_read_double,(ctl, pnm,	/* fetch the next pixel			    */
        fd, "pixel", &val));
      if (val >							/* check value range			    */
          dt_ntype_maxval[pnm->ntype])	
         return dt_err(ctl, F,
            "pixel value %e greater than maxgray %e",
            val, 
            dt_ntype_maxval[pnm->ntype]);	
      DT_Q(dtimage_set_pixel,(ctl,		/* put new pixel into image		    */
        image, x, y, val));
    }
  } 

  return DT_RC_GOOD;
}

/*..........................................................................
| You must read the entire image,
| reading a patch out of an image is not support at this time.
| Image data type and dimensions must match file data type and connections.
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtimage_read_raw_fd(					/* get raw image data from open fd	*/
  dt_ctl_t *ctl,
  dtimage_t *image,
  unsigned int rowsize,
  dtfd_t *fd)
{
  DT_F("dtimage_read_raw_fd");
  dtxy_t y;
  y = 0;								/* start of first block			    */
  do {
    void *p;
    unsigned int datasize;
	if (image->flags &					/* rows are contiguous? 			*/
		DTIMAGE_FLAG_CONTIG)
      DTIMAGE_GETROWS(ctl, image, y,	/* map the next block               */
        &p, &datasize)
    else
	{
      DTIMAGE_GETROW(ctl, image, y,		/* map just one row					*/
        &p);
	  datasize = image->rowsize;
	}
    DT_Q(dtfd_read_all,(ctl, fd,		/* read this memory block or row  	*/
      p, datasize));
    DTIMAGE_PUTROW(ctl, image, y);		/* don't need this block any more   */
    y += datasize / image->rowsize;		/* how many rows in block           */
  } while(y < image->ye);
  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/

static
dt_rc_e
dtimage_read_pnm_fd(					/* get image from opened pnm fd		*/
  dt_ctl_t *ctl,
  dtimage_t *image,
  dtfd_t *fd)
{
  DT_F("dtimage_read_pnm_fd");
  dtimage_t input;
  dtpnm_t pnm;
  dt_rc_e rc;

  input.xe = 0;

  DT_C(dtpnm_read_header,(ctl,			/* read header from opened fd	  	*/
    &pnm, fd));

  if (pnm.ntype == DT_NTYPE_B01)		/* binary input image? 				*/
    pnm.xe = (pnm.xe + 7) & ~0x0007;	/* row has exact number of bytes 	*/

  DT_C(dtimage_create,(ctl, &input,		/* create image					    */
    pnm.xe, pnm.ye, pnm.ntype));

  DT_C(dtimage_dbg_op,(ctl, &input,		/* trace execution				    */
    fd->filename, F));

  if (pnm.ptype == 4 ||					/* one of the raw types?			*/
      pnm.ptype == 5)
  {
    DT_C(dtimage_read_raw_fd,(ctl,		/* read raw data					*/
      &input, input.rowsize, fd));
  }
  else									/* not raw type?				    */
  if (pnm.ptype == 2)					/* one of the ascii types?			*/
    DT_C(dtimage_read_pnm_ascii_fd,(	/* read the image very slowly	    */
      ctl, &input, &pnm, fd))
  else
  {
    rc = dt_err(ctl, F,
      "magic number P%d not supported",
      pnm.ptype);
	goto cleanup;
  }

cleanup:
  if (rc != DT_RC_GOOD && input.xe)		/* read failed?					    */
    DT_I(dtimage_free,(ctl, &input))	/* free image before error exit	    */
  else
    *image = input;

  return rc;
}

/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtimage_read_pnm_file(					/* get image from pnm file			*/
  dt_ctl_t *ctl,
  dtimage_t *image,
  char *name)
{
  DT_F("dtimage_read_pnm_file");
  dtfd_t fd;
  dt_rc_e rc;

  DT_Q(dtfd_open,(ctl,					/* open file for reading			*/
    &fd, name, "rz"));

  DT_G(dtimage_read_pnm_fd,(ctl,		/* read image from opened fd	  	*/
	image, &fd));

  DT_Q(dtfd_close,(ctl, &fd));			/* close opened fd					*/

  return rc;
}

/*..........................................................................
 * called by dtdib_import to get data
 * rgb is right after the dib and data is right after the rgb
 *..........................................................................*/

typedef struct {
  dtfd_t *fd;
  long total;
} myarg_t;

dt_rc_e
DTCONFIG_API1
dtimage_wmfbmp_readfunc(
  dt_ctl_t *ctl,
  void *arg,
  void *data,
  unsigned int size)
{
  DT_F("dtimage_wmfbmp_readfunc");
  myarg_t *myarg = (myarg_t *)arg;
  DT_Q(dtfd_read_all,(ctl, myarg->fd,
    data, size));
  myarg->total += size;

# ifdef NONO
  dt_dbg(ctl, F, DT_DBG_MASK_ANY,
    "just read %ld, total now %ld",
    size, myarg->total);
# endif

  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtimage_read_wmf_fd(					/* get image from opened pnm fd		*/
  dt_ctl_t *ctl,
  dtimage_t *image,
  dtfd_t *fd)
{
  DT_F("dtimage_read_wmf_fd");
  dtwmf_placeable_t placeable;
  myarg_t myarg;
  dt_rc_e rc;
  
  DT_C(dtfd_read_all,(ctl, fd,			/* read the placeable header		*/
    (void *)&placeable, 
    sizeof(placeable)));
  
  DT_C(dtwmf_placeable_endian,(ctl,		/* convert endian-ness				*/
    &placeable));

  if (placeable.magic1 !=				/* check the magic number 			*/
	  DTWMF_PLACEABLE_MAGIC1 ||
	  placeable.magic2 !=
	  DTWMF_PLACEABLE_MAGIC2)
  {
	  dt_err(ctl, F, 
        "metafile has bad"
        " Adobe placeable"
        " magic number %04x%04x"
        " (expected %04x%04x)",
        placeable.magic1,
        placeable.magic2,
        DTWMF_PLACEABLE_MAGIC1,
        DTWMF_PLACEABLE_MAGIC2);
	  goto cleanup;
  }

  myarg.fd = fd;
  myarg.total = 0;

  DT_C(dtwmf_import,(ctl,				/* import image from file 			*/
    dtimage_wmfbmp_readfunc, (void *)&myarg,
    placeable.bbox.right -				/* guess the image size				*/
    placeable.bbox.left,
    placeable.bbox.bottom -
    placeable.bbox.top,
    image, 0, 0));

cleanup:

  return rc;
}

/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtimage_read_wmf_file(					/* get image from pnm file			*/
  dt_ctl_t *ctl,
  dtimage_t *image,
  char *name)
{
  DT_F("dtimage_read_wmf_file");
  dtfd_t fd;
  dt_rc_e rc;

  DT_Q(dtfd_open,(ctl,					/* open file for reading			*/
    &fd, name, "rz"));

  DT_G(dtimage_read_wmf_fd,(ctl,		/* read image from opened fd	  	*/
	image, &fd));

  DT_Q(dtfd_close,(ctl, &fd));			/* close opened fd					*/

  return rc;
}

/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtimage_read_bmp_fd(					/* get image from opened pnm fd		*/
  dt_ctl_t *ctl,
  dtimage_t *image,
  dtfd_t *fd)
{
  DT_F("dtimage_read_bmp_fd");
  myarg_t myarg;
  dt_rc_e rc;

  myarg.fd = fd;
  myarg.total = 0;

  DT_C(dtbmp_import,(ctl,				/* import image from file 			*/
    dtimage_wmfbmp_readfunc,
    (void *)&myarg,
    image, 0, 0));

cleanup:

  return rc;
}

/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtimage_read_bmp_file(					/* get image from pnm file			*/
  dt_ctl_t *ctl,
  dtimage_t *image,
  char *name)
{
  DT_F("dtimage_read_bmp_file");
  dtfd_t fd;
  dt_rc_e rc;

  DT_Q(dtfd_open,(ctl,					/* open file for reading			*/
    &fd, name, "rz"));

  DT_G(dtimage_read_bmp_fd,(ctl,		/* read image from opened fd	  	*/
	image, &fd));

  DT_Q(dtfd_close,(ctl, &fd));			/* close opened fd					*/

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
