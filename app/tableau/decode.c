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
  char s[128], *t;
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
  tableau_decode_jpeg
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
