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
DT_RCSID("source/xchg $RCSfile: import.c,v $ $Revision: 1.2 $");
#include <dtack/os.h>
#include <dtack/ipc.h>
#include <dtack/lut8.h>

#include <dtack/xchg.h>

#include <dtack/pnm.h>
#include <dtack/mem.h>

/*..........................................................................
| NAME
| dtimage_import() - input image
| 
| SUMMARY
| Use ~spec~ to identify the source and format of the image.
| Determine the image size from the source.
| Allocate space for ~image~ of the appropriate size.
| Interpret the input data and fill the image with it.
| 
| Details of the syntax of ~spec~ are given in the
| introduction section of this document.
|
| Have an error if ~spec~ specifies an unknown source.
| 
| END
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtimage_import(							/* import image						*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtimage_t *image,						/* image to be allocated and input 	*/
  const char *spec)						/* source specification 			*/
{
  DT_F("dtimage_import");
  dtxchg_e which;
  char *server;
  char *id;
  dt_rc_e rc;

  DT_MEMSET(image, 0, sizeof(*image));		/* cancel contents of structure 	*/
  
  {
	char message[256];
	DT_Q(dtxchg_parse,(ctl, spec,		/* figure out which exchanger	    */
      &which, &server, &id,
      NULL, message, sizeof(message)));
	if (message[0] != '\0')
      return dt_err(ctl, F, "%s", 
        message);
  }

  switch(which)
  { 
    case DTXCHG_TIFF:					/* ................................	*/
    {
      DT_C(dtimage_read_tiff_file,		/* read tiff file					*/
        (ctl, image, id));
	  break;
	}
    case DTXCHG_WMF:					/* ................................	*/
    {
      DT_C(dtimage_read_wmf_file,(		/* read wmf file					*/
        ctl, image, id));
	} break;
    case DTXCHG_BMP:					/* ................................	*/
    {
      DT_C(dtimage_read_bmp_file,(		/* read bmp file					*/
        ctl, image, id));
	} break;
    case DTXCHG_PNM:					/* ................................	*/
    case DTXCHG_PGM:
    case DTXCHG_PBM:
    case DTXCHG_PNM_ASCII:
    case DTXCHG_PGM_ASCII:
    case DTXCHG_PBM_ASCII:
    {
      DT_C(dtimage_read_pnm_file,		/* read pnm file					*/
        (ctl, image, id));
	} break;
    case DTXCHG_JPG:					/* ................................	*/
    {
      DT_C(dtxchg_jpeg_import_file,		/* input from jpeg file				*/
        (ctl, id, image));
	} break;
    case DTXCHG_WIN:					/* ................................	*/
    {
      DT_C(dtxchg_win_import_image,		/* input from window server			*/
        (ctl, server, id, image, 
         0, 0));
	} break;
    case DTXCHG_WXX:					/* ................................	*/
    {
      DT_C(dtxchg_wxx_import_image,		/* input from wxx server			*/
        (ctl, server, id, image, 
         0, 0));
	} break;
    default:
	{
      if (which < DTXCHG_END_OF_LIST)	/* some known one we can't handle?	*/
	  {
        rc = dt_err(ctl, F,
          "Sorry, cannot import \"%s\""
          " since type \"%s\""
          " is not currently supported.",
          spec, dtxchg_choices[which]);
	  }
	  else								/* totally unknown "which"		    */
	  {
        rc = dt_err(ctl, F,
          "Sorry, an internal problem"
          " gave invalid xchg choice %d"
          " for \"%s\"",
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
