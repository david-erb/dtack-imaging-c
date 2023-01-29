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
DT_RCSID("source/high $RCSfile: write.c,v $ $Revision: 1.3 $");

#include <dtack/pnm.h>
#include <dtack/stack.h>
#include <dtack/mem.h>
#include <dtack/str.h>

#define ESC 0x1b

/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtimage_write_raw_fd(					/* write raw image data on open fd	*/
  dt_ctl_t *ctl,
  dtimage_t *image,
  unsigned int reserved,
  dtfd_t *fd)
{
  DT_F("dtimage_write_raw_fd");
  dtxy_t y;
  y = 0;								/* start of first block			    */

  dt_dbg(ctl, F, DT_DBG_MASK_IMG,
    "image rows %s contiguous",
    image->flags & DTIMAGE_FLAG_CONTIG?
    "are": "are not");

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
    DTIMAGE_UNMAPROW(ctl, image, y);	/* don't need this block any more   */
    DT_Q(dtfd_write_all,(ctl, fd,		/* write this memory block or row  	*/
      p, datasize));
    y += datasize / image->rowsize;		/* how many rows in block           */
  } while(y < image->ye);
  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtimage_write_ascii_fd(					/* write ascii image data on fd		*/
  dt_ctl_t *ctl,
  dtimage_t *image,
  dtfd_t *fd)
{
  DT_F("dtimage_write_ascii_fd");
  dtxy_t x;
  dtxy_t y;
  int l = 0;
  const int size = DT_CAST(int,
    dt_ntype_sizeof[image->ntype]);
  char line[72];
  for (y=0; y<image->ye; y++)
  {
    char *p;
    DTIMAGE_GETROW(ctl, image, y, &p);
	for (x=0; x<image->xe; x++)
	{
  	  char s[64];
	  DT_Q(dtstr_print_ntype,(ctl,		/* format the pixel into ascii	    */
	    p, image->ntype,
        s, sizeof(s)));
      if (l+strlen(s) > sizeof(line)-2)	/* line full?					    */
	  {
	    strcat(line, "\n");
	    DT_Q(dtfd_write_all,(ctl, fd,	/* write line					    */
          line, l+1));
        l = 0;
	  }

      strcpy(line+l, s);				/* add pixel to growing line	    */
      l += strlen(s);

      if (x == image->xe-1)				/* end of row?					    */
	  {
	    strcat(line, "\n");
	    DT_Q(dtfd_write_all,(ctl, fd,	/* write line					    */
          line, l+1));
        l = 0;
	  }
      p += size;
	}
    DTIMAGE_UNMAPROW(ctl, image, y);	/* don't need this block any more   */
  }

  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtimage_write_lj_fd(					/* put image to opened lj fd		*/
  dt_ctl_t *ctl,
  dtimage_t *image,
  dtfd_t *fd,
  int resolution,						/* resoluton or 0 to leave as is    */
  dtxy_t xpos,							/* in units of resolution or -1	    */
  dtxy_t ypos,
  long flags)
{
  DT_F("dtimage_write_lj_fd");
  dt_ntype_e ntypes[] =					/* datatypes we can handle			*/
    {DT_NTYPE_B01, DT_NTYPE_NULL};
  char buf[64];
  dtxy_t y;
  dt_rc_e rc;

  rc = dtimage_check_1f(ctl,			/* check image validity			    */
    image, fd->filename, ntypes,
    DTIMAGE_FLAG_PAGED, F);
  if (rc != DT_RC_GOOD)
    return rc;

  if (flags & DT_DO_START_PAGE)
  {
    sprintf(buf, "%cE", ESC);			/* new page and reset				*/
    DT_Q(dtfd_write_all,(ctl, fd,
      buf, strlen(buf)));
  }

  if (resolution >= 0)					/* caller wants resolution set?	    */
  {
    sprintf(buf, "%c&u%dD%c*t%dR",		/* unit of movement and resolution 	*/
      ESC, resolution,
      ESC, resolution);
    DT_Q(dtfd_write_all,(ctl, fd,	
      buf, strlen(buf)));
  }

  if (flags & DT_HAS_XPOS)				/* x position specified?			*/
  {
    sprintf(buf, "%c*p%ldX", ESC, 
      (long)xpos);
    DT_Q(dtfd_write_all,(ctl, fd,	
      buf, strlen(buf)));
  }

  if (flags & DT_HAS_YPOS)				/* y position specified?			*/
  {
    sprintf(buf, "%c*p%ldY", ESC, 
      (long)ypos);
    DT_Q(dtfd_write_all,(ctl, fd,	
      buf, strlen(buf)));
  }

  sprintf(buf, "%c*r1A", ESC);			/* start raster graphics here	    */
  DT_Q(dtfd_write_all,(ctl, fd,
    buf, strlen(buf)));

  sprintf(buf, "%c*b%uW",				/* make per-row header			    */
    ESC, image->rowsize);

  for (y=0; y<image->ye; y++)			/* output rows one at a time	    */
  {
    void *p;
    DT_Q(dtfd_write_all,(ctl, fd,		/* write row header					*/
      buf, strlen(buf)));
    DTIMAGE_GETROW(ctl, image, y, &p);
    DT_Q(dtfd_write_all,(ctl, fd,		/* write row data					*/
      p, image->rowsize));
    DTIMAGE_UNMAPROW(ctl, image, y);
  }

  sprintf(buf, "%c*rB", ESC);			/* end raster graphics			    */
  DT_Q(dtfd_write_all,(ctl, fd,	
    buf, strlen(buf)));

  if (flags & DT_DO_END_PAGE)
  {
    sprintf(buf, "%cE", ESC);			/* new page						    */
    DT_Q(dtfd_write_all,(ctl, fd,		
      buf, strlen(buf)));
  }

  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtimage_write_lj_file(					/* put image to laserjet file		*/
  dt_ctl_t *ctl,
  dtimage_t *image,
  char *name,
  int resolution,
  dtxy_t x,
  dtxy_t y,
  long flags)
{
  DT_F("dtimage_write_lj_file");
  dtfd_t fd;
  dt_rc_e rc;

  DT_Q(dtfd_open,(ctl,					/* open file for writing			*/
    &fd, name, "w"));

  DT_G(dtimage_write_lj_fd,(ctl,		/* write image to opened fd	  		*/
    image, &fd, resolution,
    x, y, flags));						

  DT_Q(dtfd_close,(ctl, &fd));			/* close opened fd					*/

  return rc;
}

/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtimage_write_pgm_fd(					/* put image to opened pgm fd		*/
  dt_ctl_t *ctl,
  dtimage_t *image,
  dtfd_t *fd)
{
  DT_F("dtimage_write_pgm_fd");
  dtpnm_t pnm;
  dt_rc_e rc;

  rc = dtimage_check_1f(ctl,			/* check image validity			    */
    image, fd->filename, NULL,
    DTIMAGE_FLAG_PAGED, F);
  if (rc != DT_RC_GOOD)
    return rc;
  
  DT_MEMSET(&pnm, 0, sizeof(pnm));			/* clear pnm structure 				*/

  pnm.magic = DTPNM_MAGIC_PGM_RAW;		/* load fields of pnm structure 	*/
  pnm.xe = image->xe;
  pnm.ye = image->ye;
  pnm.ntype = image->ntype;

  DT_Q(dtpnm_write_header,(ctl,			/* write header to opened fd	  	*/
    &pnm, fd));

  DT_Q(dtimage_write_raw_fd,(ctl,		/* write raw data					*/
    image, 0, fd));

  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtimage_write_pgm_file(					/* put image to pgm file			*/
  dt_ctl_t *ctl,
  dtimage_t *image,
  char *name)
{
  DT_F("dtimage_write_pgm_file");
  dtfd_t fd;
  dt_rc_e rc;

  DT_Q(dtfd_open,(ctl,					/* open file for writing			*/
    &fd, name, "w"));

  DT_G(dtimage_write_pgm_fd,(ctl,		/* write image to opened fd	  		*/
    image, &fd));						

  DT_I(dtfd_close,(ctl, &fd));			/* close opened fd					*/

  return rc;
}

/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtimage_write_pgm_ascii_fd(				/* put image to opened pgm fd		*/
  dt_ctl_t *ctl,
  dtimage_t *image,
  dtfd_t *fd)
{
  DT_F("dtimage_write_pgm_ascii_fd");
  dtpnm_t pnm;
  dt_rc_e rc;

  rc = dtimage_check_1f(ctl,			/* check image validity			    */
    image, fd->filename, NULL,
    DTIMAGE_FLAG_PAGED, F);
  if (rc != DT_RC_GOOD)
    return rc;
  
  DT_MEMSET(&pnm, 0, sizeof(pnm));

  pnm.magic = DTPNM_MAGIC_PGM_ASCII;
  pnm.xe = image->xe;
  pnm.ye = image->ye;
  pnm.ntype = image->ntype;

  DT_Q(dtpnm_write_header,(ctl,			/* write header to opened fd	  	*/
    &pnm, fd));

  DT_Q(dtimage_write_ascii_fd,(ctl,		/* write ascii data					*/
    image, fd));

  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtimage_write_pgm_ascii_file(			/* put image to pgm file			*/
  dt_ctl_t *ctl,
  dtimage_t *image,
  char *name)
{
  DT_F("dtimage_write_pgm_ascii_file");
  dtfd_t fd;
  dt_rc_e rc;

  DT_Q(dtfd_open,(ctl,					/* open file for writing			*/
    &fd, name, "w"));

  DT_G(dtimage_write_pgm_ascii_fd,(ctl,	/* write image to opened fd	  		*/
    image, &fd));						

  DT_Q(dtfd_close,(ctl, &fd));			/* close opened fd					*/

  return rc;
}

/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtimage_write_pbm_fd(					/* put image to opened pbm fd		*/
  dt_ctl_t *ctl,
  dtimage_t *image,
  dtfd_t *fd)
{
  DT_F("dtimage_write_pbm_fd");
  dt_ntype_e ntypes[] =					/* datatypes we can handle			*/
    {DT_NTYPE_B01, DT_NTYPE_NULL};
  dtpnm_t pnm;
  dt_rc_e rc;

  rc = dtimage_check_1f(ctl,			/* check image validity			    */
    image, fd->filename, ntypes,
    DTIMAGE_FLAG_PAGED, F);
  if (rc != DT_RC_GOOD)
    return rc;

  pnm.magic = DTPNM_MAGIC_PBM_RAW;
  pnm.xe = image->xe;
  pnm.ye = image->ye;
  pnm.ntype = image->ntype;

  DT_Q(dtpnm_write_header,(ctl,			/* write header to opened fd	  	*/
    &pnm, fd));

  DT_Q(dtimage_write_raw_fd,(ctl,		/* write raw data					*/
    image, 0, fd));

  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtimage_write_pbm_ascii_fd(				/* put image to opened pbm fd		*/
  dt_ctl_t *ctl,
  dtimage_t *image,
  dtfd_t *fd)
{
  DT_F("dtimage_write_pbm_ascii_fd");
  dt_ntype_e ntypes[] =					/* datatypes we can handle			*/
    {DT_NTYPE_B01, DT_NTYPE_NULL};
  dtpnm_t pnm;
  dt_rc_e rc;

  rc = dtimage_check_1f(ctl,			/* check image validity			    */
    image, fd->filename, ntypes,
    DTIMAGE_FLAG_PAGED, F);
  if (rc != DT_RC_GOOD)
    return rc;

  pnm.magic = DTPNM_MAGIC_PBM_ASCII;
  pnm.xe = image->xe;
  pnm.ye = image->ye;
  pnm.ntype = image->ntype;

  DT_Q(dtpnm_write_header,(ctl,			/* write header to opened fd	  	*/
    &pnm, fd));

  DT_Q(dtimage_write_raw_fd,(ctl,		/* write raw data					*/
    image, 0, fd));

  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtimage_write_pbm_file(					/* put image to pbm file			*/
  dt_ctl_t *ctl,
  dtimage_t *image,
  char *name)
{
  DT_F("dtimage_write_pbm_file");
  dtfd_t fd;
  dt_rc_e rc;

  DT_Q(dtfd_open,(ctl,					/* open file for writing			*/
    &fd, name, "w"));

  DT_G(dtimage_write_pbm_fd,(ctl,		/* write image to opened fd	  		*/
    image, &fd));						

  DT_Q(dtfd_close,(ctl, &fd));			/* close opened fd					*/

  return rc;
}

/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtimage_write_pbm_ascii_file(			/* put image to pbm file			*/
  dt_ctl_t *ctl,
  dtimage_t *image,
  char *name)
{
  DT_F("dtimage_write_pbm_ascii_file");
  dtfd_t fd;
  dt_rc_e rc;

  DT_Q(dtfd_open,(ctl,					/* open file for writing			*/
    &fd, name, "w"));

  DT_G(dtimage_write_pbm_ascii_fd,(ctl,	/* write image to opened fd	  		*/
    image, &fd));						

  DT_Q(dtfd_close,(ctl, &fd));			/* close opened fd					*/

  return rc;
}


/*..........................................................................
 * choose pgm or pbm to write based on image ntype
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtimage_write_pnm_fd(					/* put image to opened pnm fd		*/
  dt_ctl_t *ctl,
  dtimage_t *image,
  dtfd_t *fd)
{
  DT_F("dtimage_write_pnm_fd");

  if (image->ntype == DT_NTYPE_B01)
    DT_Q(dtimage_write_pbm_fd,(ctl,
      image, fd))
  else
    DT_Q(dtimage_write_pgm_fd,(ctl,
      image, fd))

  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtimage_write_pnm_file(					/* put image to pnm file			*/
  dt_ctl_t *ctl,
  dtimage_t *image,
  const char *name)
{
  DT_F("dtimage_write_pnm_file");
  dtfd_t fd;
  dt_rc_e rc;

  DT_Q(dtfd_open,(ctl,					/* open file for writing			*/
    &fd, name, "w"));

  DT_G(dtimage_write_pnm_fd,(ctl,		/* write image to opened fd	  		*/
    image, &fd));						

  DT_Q(dtfd_close,(ctl, &fd));			/* close opened fd					*/

  return rc;
}

/*..........................................................................
 * choose pgm or pbm to write based on image ntype
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtimage_write_pnm_ascii_fd(				/* put image to opened pnm fd		*/
  dt_ctl_t *ctl,
  dtimage_t *image,
  dtfd_t *fd)
{
  DT_F("dtimage_write_pnm_ascii_fd");

  if (image->ntype == DT_NTYPE_B01)
    DT_Q(dtimage_write_pbm_ascii_fd,(ctl,
      image, fd))
  else
    DT_Q(dtimage_write_pgm_ascii_fd,(ctl,
      image, fd))

  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtimage_write_pnm_ascii_file(			/* put image to pnm file			*/
  dt_ctl_t *ctl,
  dtimage_t *image,
  const char *name)
{
  DT_F("dtimage_write_pnm_ascii_file");
  dtfd_t fd;
  dt_rc_e rc;

  DT_Q(dtfd_open,(ctl,					/* open file for writing			*/
    &fd, name, "w"));

  DT_G(dtimage_write_pnm_ascii_fd,(ctl,	/* write image to opened fd	  		*/
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
