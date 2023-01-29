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

DT_RCSID("app/tableau $RCSfile: encode.c,v $ $Revision: 1.8 $");

#include <mangle.h>

/*..........................................................................
 * encode uncompressed
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
tableau_encode_uncompressed(
  dtparam_t *param,
  tableau_encode_arg_t *encode_arg,		/* encoder args 					*/
  dtimage_t *image,						/* image to encode					*/
  dt_color_org_t *org,					/* incoming image color org 		*/
  tableau_header_t *header,				/* compression header 				*/
  void **data,							/* the encoded buffer 				*/
  long *l)								/* current size of encoded buffer 	*/
{
  DT_F("tableau_encode_uncompressed");
  dt_ctl_t *ctl = &param->ctl;

  if (image == NULL)					/* need to cleanup and quit? 		*/
	return DT_RC_GOOD;

  DT_Q(dtimage_check_1f,(ctl, image,	/* make sure image is not paged 	*/
    "image to encode", 
    tableau_ntypes, 0, F));

  strcpy(header->codec,
    TABLEAU_CODEC_UNCOMPRESSED);
  header->l =							/* space it takes up 				*/
    DT_CAST(long, image->rowsize) * 
    DT_CAST(long, image->ye);	
  header->xe = image->xe;
  header->ye = image->ye;

  *data = image->row[0].dvoid;			/* presuming image is not paged 	*/

  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
tableau_encode_jpeg(
  dtparam_t *param,
  tableau_encode_arg_t *encode_arg,		/* encoder args 					*/
  dtimage_t *image,						/* image to encode					*/
  dt_color_org_t *org,					/* incoming image color org 		*/
  tableau_header_t *header,				/* compression header 				*/
  void **data,							/* the encoded buffer 				*/
  long *l)								/* current size of encoded buffer 	*/
{
  DT_F("tableau_encode_jpeg");
  dt_ctl_t *ctl = &param->ctl;
  long n;
  long used;
  dt_rc_e rc;

  if (image == NULL)					/* need to cleanup and quit? 		*/
  {
	if (*l != 0 && *data != NULL)
	  DT_Q(dtos_free2,(ctl, *data,
        F, "uncompressed data"));

    *l = 0;
    *data = NULL;

	return DT_RC_GOOD;
  }

  DT_C(dtimage_check_1f,(ctl, image,	/* make sure image is not paged 	*/
    "image to encode", 
    tableau_ntypes, 0, F));

  n = (long)image->xe *					/* upper bound on buffer size 		*/
      (long)image->ye;
  if (*l < n)							/* don't have enough space? 		*/
  {
	if (*l != 0)						/* have any space? 					*/
	  DT_C(dtos_free2,(ctl, *data,
        F, "old data space"));
	*l = 0;
    *data = NULL;

    DT_C(dtos_malloc2,(ctl, data, n,	/* space to hold buffer 			*/
      F, "encoded buffer"));

    *l = n;
  }

  {
	char s[32];
	DT_C(dt_color_org_string,(ctl, org,
      s, sizeof(s)));
	dtstr_printf(ctl, 
      s+strlen(s), 
      sizeof(s)-strlen(s),
      "+%d", encode_arg->quality);

    DT_C(dtxchg_jpeg_export_memory,(ctl,
      s, *data, n, &used, image));
  }

#ifdef NONO
  DT_C(dtxchg_jpeg_import_memory,(		/* decode the buffer into image 	*/
    ctl, *data, used, NULL, image));
#endif

  strcpy(header->codec,
    TABLEAU_CODEC_JPEG);
  header->l = used;						/* space it takes up 				*/
  header->xe = image->xe;				/* size of the image we shrunk 		*/
  header->ye = image->ye;

#ifdef NONO
  if (!strcmp(ctl->progname, "percap") &&
	  used > MANGLE_MAX * 2)
  {
	int n = used / 2;
	int i;
	char *p = (char *)(*data);
	if (n > MANGLE_MAX)
	  n = MANGLE_MAX;
	for (i=0; i<n; i++)
	{
	  char t = p[i];
	  p[i] = p[MANGLE_MAX+mangle_random[i]];
	  p[MANGLE_MAX+mangle_random[i]] = t;
	}
  }
#endif

cleanup:
  return rc;
}

/*..........................................................................
 * look up encode routine by name
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
tableau_encode_from_name(
  dtparam_t *param,
  const char *name,
  tableau_encode_f **encode)
{
  DT_F("tableau_encode_from_name");
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

  *encode =							
    tableau_encode_methods[choice];

  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/

static tableau_encode_f *encode_methods[] = {
  tableau_encode_uncompressed,
  tableau_encode_jpeg,
  NULL									/* passthrough 						*/
};
tableau_encode_f **tableau_encode_methods = encode_methods;





/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
  ! Copyright (C) 1998 Dtack Inc. All Rights Reserved
  ! To use this file, you must have signed a license agreement with Dtack Inc.
  ! Under no circumstances may you redistribute this file.
  ! This software is provided AS IS with no warranty expressed or implied.
  ! Dtack Inc. accepts no liability for use or misuse of this file.
  ! http://www.dtack.com  dtack@dtack.com  telephone +360.417.6700
  ! Dtack Inc, P.O. Box 3061, Port Angeles, WA USA 98362
  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */
