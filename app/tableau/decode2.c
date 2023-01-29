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
| main program
| 
| END
 *..........................................................................*/

#include <tableau.h>

DT_RCSID("test/tableau $RCSfile: decode.c,v $ $Revision: 1.8 $");

#ifndef DTDEPEND
#include <../fourth/btpcv3/btpc.h>
#endif

/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
tableau_decode_uncompressed(
  dtparam_t *param,
  tableau_decode_arg_t *decode_arg,		/* decoder args 					*/
  tableau_header_t *header,				/* compression header 				*/
  void *data,							/* buffer to be decoded 			*/
  dtimage_t *image)						/* decoded image 					*/
{
  DT_F("tabenc_decode_uncompressed");
  dt_ctl_t *ctl = &param->ctl;

  if (data == NULL ||					/* cleaning up? 					*/
	  image->xe != header->xe ||
	  image->ye != header->ye)
  {
    if (image->xe != 0)					/* we have an image already? 		*/
	  DT_Q(dtimage_free,(ctl, image));	/* free the old image 				*/
  }

  if (data == NULL)						/* need to cleanup and quit? 		*/
  {
	return DT_RC_GOOD;
  }
	
  if (image->xe == 0)					/* need image?						*/
    DT_Q(dtimage_create,(ctl, image,	/* allocate new image 				*/
      header->xe, header->ye, 
      DT_NTYPE_B08));

  DT_Q(dtimage_check_1f,(ctl, image,	/* make sure image is not paged 	*/
    "decoded image", 
    tableau_ntypes, 0, F));

  memcpy(image->row[0].dvoid, data,		/* transfer uncompressed data 		*/
    header->xe * header->ye);

  return DT_RC_GOOD;
}

#ifdef NONO
/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
tableau_decode_uncompressed(
  dtparam_t *param,
  tableau_decode_arg_t *decode_arg,		/* decoder args 					*/
  tableau_header_t *header,				/* compression header 				*/
  void *data,							/* buffer to be decoded 			*/
  dtimage_t *image)						/* decoded image 					*/
{
  DT_F("tabenc_decode_uncompressed");
  dt_ctl_t *ctl = &param->ctl;
  dtmemalloc_t *memalloc = NULL;

  if (data == NULL ||					/* cleaning up? 					*/
	  image->xe == 0 ||
	  image->row[0].dvoid != data)		/* new data? 						*/
  {
    if (image->xe != 0)					/* we have an image already? 		*/
	{
      if (!(image->flags &				/* we allocated the pixel space?    */
			DTIMAGE_FLAG_FREE))
	    memalloc = image->memalloc;
	  DT_Q(dtimage_free,(ctl, image));	/* free the old image 				*/
	  if (memalloc != NULL)
  	    DT_Q(dtmem_free,(ctl,			/* free memalloc 					*/
          memalloc,
          F, "decoded image data"));
	}
  }

  if (data == NULL)						/* need to cleanup and quit? 		*/
  {
	return DT_RC_GOOD;
  }
	
  if (image->xe == 0)					/* new data? 						*/
  {
	DT_Q(dtmem_prealloc,(ctl,			/* wrap memalloc around buffer 		*/
      (void **)&memalloc, 
      header->xe, header->ye, &data,    
      header->xe * header->ye, 1,
      F, "raw uncompressed data"));
    DT_Q(dtimage_create3,(ctl, image,	/* wrap image around memalloc		*/
      0, 0,
      header->xe, header->ye, 
      DT_NTYPE_B08, memalloc));
  }

  DT_Q(dtimage_check_1f,(ctl, image,
    "decoded image", 
    tableau_ntypes, 0, F));

  return DT_RC_GOOD;
}
#endif
/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
tableau_decode_jpeg(
  dtparam_t *param,
  tableau_decode_arg_t *decode_arg,		/* decoder args 					*/
  tableau_header_t *header,				/* compression header 				*/
  void *b,								/* buffer to be decoded 			*/
  dtimage_t *image)						/* decoded image 					*/
{
  DT_F("tableau_decode_jpeg");
  dt_ctl_t *ctl = &param->ctl;
  dt_rc_e rc = DT_RC_GOOD;

  if (b == NULL)						/* need to cleanup and quit? 		*/
  {
	if (image->xe != 0)
      DT_I(dtimage_free,(ctl, image));
	return rc;
  }

  DT_Q(dtxchg_jpeg_import_memory,(		/* decode the buffer into image 	*/
    ctl, b, header->l, NULL, image));

  DT_Q(dtimage_check_1f,(ctl, image,
    "decoded image", 
    tableau_ntypes, 0, F));

  return DT_RC_GOOD;
}
/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
tableau_decode_btpc(
  dtparam_t *param,
  tableau_decode_arg_t *decode_arg,		/* decoder args 					*/
  tableau_header_t *header,				/* compression header 				*/
  void *b,								/* buffer to be decoded 			*/
  dtimage_t *image)						/* decoded image 					*/
{
  DT_F("tableau_decode_btpc");
  dt_ctl_t *ctl = &param->ctl;
  dt_rc_e rc = DT_RC_GOOD;

  if (b == NULL)						/* need to cleanup and quit? 		*/
  {
	if (image->xe != 0)
      DT_I(dtimage_free,(ctl, image));
	return rc;
  }

  DT_Q(tableau_btpc_decode_buffer,(		/* decode the buffer into image 	*/
    param,
    (char *)b, header->l, image));

  DT_Q(dtimage_check_1f,(ctl, image,
    "decoded image", 
    tableau_ntypes, 0, F));

  return DT_RC_GOOD;
}

/*..........................................................................
 * image MUST BE CURRENT on input
 *..........................................................................*/

#define COMPRESS_DIFFS tableau_encode_rlerb
#define UNCOMPRESS_DIFFS tableau_decode_rlerb

static
dt_rc_e
DTCONFIG_API1
tableau_decode_diffs(
  dtparam_t *param,
  tableau_decode_arg_t *decode_arg,		/* decoder args 					*/
  tableau_header_t *header,				/* compression header 				*/
  void *data,							/* buffer to be decoded 			*/
  dtimage_t *image)						/* decoded image 					*/
{
  DT_F("tableau_decode_diffs");
  dt_ctl_t *ctl = &param->ctl;
  dtimage_t *diffs = 
    &decode_arg->diffs;
  dtscr_t *scr = &decode_arg->scr;
  const char *mod;
  dt_rc_e rc = DT_RC_GOOD;

  if (data == NULL)						/* need to cleanup and quit? 		*/
  {
    DT_I(tableau_diffs_uncompress,(		/* cleanup decoder					*/
      param, decode_arg, NULL, NULL, 0,
      image));
	if (decode_arg->initialized)
	  DT_I(dtscr_destroy,(ctl, scr));
	return rc;
  }

  mod = strchr(header->codec, ',');		/* see if codec is modified			*/
  if (mod == NULL)
    mod = "";
  
  if (strstr(mod, ",init") != NULL)		/* first image in sequence?			*/
  {
    DT_Q(tableau_diffs_uncompress,(		/* uncompress buffer directly 		*/
      param,
      decode_arg, header, 
      data,
      header->l,
      diffs));
  }
  else									/* not first image in sequence? 	*/
  {
    DT_Q(tableau_zlib_uncompress,(ctl, 	/* uncompress the buffer			*/
      data,								/* where compressed data is 		*/
      header->l,						/* length of compressed data 		*/
      &decode_arg->b,					/* where to decompress into			*/
      &decode_arg->max,					/* current size of b				*/
      &decode_arg->l));					/* amount of data in b				*/

    DT_Q(tableau_diffs_uncompress,(		/* uncompress the zlib output 		*/
      param,
      decode_arg, header, 
      decode_arg->b,					/* decompression buffer 			*/
      decode_arg->l,					/* how much was uncompressed 		*/
      diffs));
  }

  DT_Q(dtimage_check_1f,(ctl, diffs,	/* make sure image is not paged 	*/
    "decoded diffs", 
    tableau_ntypes, 0, F));

  if (image->xe != diffs->xe ||			/* image size changing? 			*/
	  image->ye != diffs->ye)
  {
	if (image->xe != 0)
	  DT_Q(dtimage_free,(ctl, image));
    DT_Q(dtimage_create,(ctl, image,	/* space for image 					*/
      diffs->xe, diffs->ye, 
      DT_NTYPE_B08));
  }

  if (strstr(mod, ",init"))				/* first image in sequence? 		*/
  {
	DT_Q(dtimage_cast,(ctl, diffs,		/* assume this is a full image 		*/
      image));
  }
  else									/* assume incoming is a delta 		*/
  {
    DT_Q(tableau_fast_add,(ctl,			/* update current					*/
      image, diffs));
  }

  return rc;
}

/*..........................................................................
 * look up decode routine by name
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
tableau_decode_from_name(
  dtparam_t *param,
  const char *name,
  tableau_decode_f **decode)
{
  DT_F("tableau_decode_from_name");
  dt_ctl_t *ctl = &param->ctl;
  char s[32], *t;
  int choice;
 
  dtstr_printf(ctl, s, sizeof(s),		/* local copy of name			    */
    "%s", name);

  t = strchr(s, ',');					/* see if name has a comma 			*/
  if (t != NULL)
    *t = '\0';							/* chop off the comma			    */

  DT_Q(dtstr_choice,(ctl,				/* look up decode type		    	*/
    tableau_codec_choices, 
    s, &choice));

  if (choice == -1)						/* unknown type?				    */
    return dt_err(ctl, F,
      "unknown codec \"%s\"", name);
  else
  if (choice == -2)						/* ambiguous type?				    */
    return dt_err(ctl, F,
      "ambiguous codec \"%s\"", name);

  *decode =							
    tableau_decode_methods[choice];

  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/

static tableau_decode_f *decode_methods[] = {
  tableau_decode_uncompressed,
  tableau_decode_btpc,
  tableau_decode_jpeg,
  tableau_decode_diffs
};
tableau_decode_f **tableau_decode_methods = decode_methods;





/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
  ! Copyright (C) 1998 Dtack Inc. All Rights Reserved
  ! To use this file, you must have signed a license agreement with Dtack Inc.
  ! Under no circumstances may you redistribute this file.
  ! This software is provided AS IS with no warranty expressed or implied.
  ! Dtack Inc. accepts no liability for use or misuse of this file.
  ! http://www.dtack.com  dtack@dtack.com  telephone +360.417.6700
  ! Dtack Inc, P.O. Box 3061, Port Angeles, WA USA 98362
  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */
