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
DT_RCSID("source/xchg $RCSfile: export.c,v $ $Revision: 1.1 $");

#include <dtack/os.h>
#include <dtack/ipc.h>
#include <dtack/pnm.h>
#include <dtack/lut8.h>

#include <dtack/xchg.h>

extern
dt_rc_e
DTCONFIG_API1
dtimage_write_rad_file(					/* put image to rad file			*/
  dt_ctl_t *ctl,
  dtimage_t *image,
  const char *name,
  const char *format);

/*..........................................................................
| NAME
| dtimage_export() - convert and output image
| 
| SUMMARY
| Convert the given ~image~ to the format indicated by ~spec~.
| Output the converted image to the destination indicated by ~spec~.
| If image positioning is supported by the destination,
| position the output image at column ~xpos~ and row ~ypos~.
| 
| Details of the syntax of ~spec~ are given in the
| introduction section of this document.
|
| Have an error if ~spec~ specifies an unknown destination.
| Have an error if the specified output format is not compatible
| with the image's size or type.
| 
| END
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtimage_export(							/* convert and output image			*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtimage_t *image,						/* source image to be converted 	*/
  const char *spec,						/* destination specification 		*/
  dtxy_t xpos,							/* output column 					*/
  dtxy_t ypos)							/* output row 						*/
{
  DT_F("dtimage_export");
  dt_ntype_e *ntypes = NULL;			/* datatypes we can handle			*/
  dtxchg_e which;
  char *server;
  char *id;
  long flags = 0;
  dt_rc_e rc;
    
  rc = dtimage_check_1f(ctl,			/* check image validity			    */
    image, "image", ntypes, 
    DTIMAGE_FLAG_PAGED, F);
  if (rc != DT_RC_GOOD)
    return rc;

  {
	char message[256];
	DT_Q(dtxchg_parse,(ctl, spec,		/* figure out which exchanger	    */
      &which, &server, &id,
      NULL, message, sizeof(message)));
	if (message[0] != '\0')
      return dt_err(ctl, F, "%s", 
        message);
  }

  if (xpos != DTIMAGE_NOPOS)
    flags |= DT_HAS_XPOS;
  if (ypos != DTIMAGE_NOPOS)
    flags |= DT_HAS_YPOS;
  if (strcmp(id, "stdout"))
     flags |=
      DT_DO_START_PAGE |
      DT_DO_END_PAGE;

  switch(which)
  { 
    case DTXCHG_RAWFILE:				/* ................................	*/
    {
	  dtfd_t fd;
      DT_C(dtfd_open,(ctl,				/* open file for writing			*/
        &fd, id, "w"));
      DT_G(dtimage_write_raw_fd,(ctl,	/* write image to opened fd	  		*/
        image, 0, &fd));
      DT_I(dtfd_close,(ctl, &fd));		/* close opened fd					*/
	  if (rc != DT_RC_GOOD)
	    goto cleanup;
	} break;
    case DTXCHG_ASCIIFILE:				/* ................................	*/
    {
 	  dtfd_t fd;
      DT_C(dtfd_open,(ctl,				/* open file for writing			*/
        &fd, id, "w"));
      DT_G(dtimage_write_ascii_fd,(ctl,	/* write image to opened fd	  		*/
        image, &fd));
      DT_I(dtfd_close,(ctl, &fd));		/* close opened fd					*/
	  if (rc != DT_RC_GOOD)
	    goto cleanup;
	} break;
    case DTXCHG_LJ600:					/* ................................	*/
    {
      DT_C(dtimage_write_lj_file,(		/* write lj600 file					*/
        ctl, image, id, 
        600, xpos, ypos, flags));
	} break;
    case DTXCHG_LJ300:					/* ................................	*/
    {
      DT_C(dtimage_write_lj_file,(		/* write lj300 file					*/
        ctl, image, id, 
        300, xpos, ypos, flags));
	} break;
    case DTXCHG_WMF:					/* ................................	*/
    {
      DT_C(dtimage_write_wmf_file,(		/* write wmf file					*/
        ctl, image, id, 
        xpos, ypos, flags));
	} break;
    case DTXCHG_BMP:					/* ................................	*/
    {
      DT_C(dtimage_write_bmp_file,(		/* write bmp file					*/
        ctl, image, id, 
        xpos, ypos, flags));
	} break;
    case DTXCHG_PNM:					/* ................................	*/
    case DTXCHG_PGM:					/* ................................	*/
    {
      DT_C(dtimage_write_pnm_file,		/* write pgm or pbm file			*/
        (ctl, image, id));
	} break;
    case DTXCHG_PBM:					/* ................................	*/
    {
      if (image->ntype != DT_NTYPE_B01)
        rc = dt_err(ctl, F,
          "cannot write %d-bit image to PBM file",
          dt_ntype_bits[image->ntype]);
      else
        DT_C(dtimage_write_pnm_file,
          (ctl, image, id));
	} break;
    case DTXCHG_PGM_ASCII:				/* ................................	*/
    case DTXCHG_PNM_ASCII:				/* ................................	*/
    {
	  DT_C(dtimage_write_pnm_ascii_file,	
        (ctl, image, id));
	} break;
    case DTXCHG_PBM_ASCII:				/* ................................	*/
    {
      if (image->ntype != DT_NTYPE_B01)
        rc = dt_err(ctl, F,
          "cannot write %d-bit image"
          " to ASCII PBM file",
          dt_ntype_bits[image->ntype]);
      else
        DT_C(dtimage_write_pnm_ascii_file,
          (ctl, image, id));
	} break;
    case DTXCHG_JPG:					/* ................................	*/
    {
      DT_C(dtxchg_jpeg_export_file,		/* output to jpeg file				*/
        (ctl, server, id, image));
	} break;
    case DTXCHG_WIN:					/* ................................	*/
    {
      DT_C(dtxchg_win_export_image,		/* output to window server			*/
        (ctl, server, id, image,
         xpos, ypos));
	} break;
    case DTXCHG_DTM:					/* ................................	*/
    {
      DT_C(dtxchg_dtm_export_image,		/* output using dtm				    */
        (ctl, server, id, image,
         xpos, ypos));
	} break;
    case DTXCHG_WXX:					/* ................................	*/
    {
      DT_C(dtxchg_wxx_export_image,		/* output as to wxx server			*/
        (ctl, server, id, image,
         xpos, ypos));
	} break;
    default:
	{
      if (which < DTXCHG_END_OF_LIST)	/* some known one we can't handle?	*/
	  {
        rc = dt_err(ctl, F,
          "cannot write %s file \"%s\"",
          dtxchg_choices[which], spec);
	  }
	  else								/* totally unknown "which"		    */
	  {
        rc = dt_err(ctl, F,
          "invalid xchg choice %d for \"%s\"",
          which, spec);
	  }
	}
  }
 
cleanup:
  DT_I(dtos_free2,(ctl, 
    server, F, "server"));
  DT_I(dtos_free2,(ctl, 
    id, F, "id"))

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
