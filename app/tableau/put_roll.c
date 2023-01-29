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
DT_RCSID("app/tableau $RCSfile: put_roll.c,v $ $Revision: 1.8 $");

/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
tableau_put_roll(
  dtparam_t *param,
  tableau_put_t *put,
  dtimage_t *image,						/* full image 						*/
  tableau_header_t *header,				/* compressed header 				*/
  void *data,							/* compressed data 					*/
  int cc)								/* color component 					*/
{
  DT_F("tableau_put_roll");
  dt_ctl_t *ctl = &param->ctl;
  long frame;
  dt_color_org_t org;
  char suffix[16];
  char fn[TABLEAU_FILENAME_MAX];
  dtfd_t fd;
  tableau_header_t lossless;
  tableau_encode_arg_t lossless_arg;
  void *b = NULL;
  long l;
  dt_rc_e rc;

  if (data == NULL)						/* supposed to clean up? 			*/
  {
	return DT_RC_GOOD;
  }

  fd.fd = -1;

  DT_C(dt_color_org_set,(ctl, &org,		/* make org structure 				*/
    DT_CAST(dt_color_org_e,
      header->org)));
  suffix[0] = '.';
  DT_C(dt_color_org_string,(ctl, &org,	/* make org string name 			*/
    suffix+1, sizeof(suffix)-1));
  
  dtstr_printf(ctl, fn, sizeof(fn),		/* make lossless template 			*/
    "%s/%s.%%08ld%s.%s", 
    put->u.roll.dir, 
    header->cam,
    suffix,
    "enc");
  
  DT_C(tableau_roll_init,(ctl,			/* init roll file 					*/
    put->u.roll.dir, header->cam,
    fn));

  {
	tableau_len_t v;

    DT_C(tableau_roll_get,(ctl,			/* current frame number				*/
      put->u.roll.dir, header->cam,
      -1, &v));

    frame = v + 1;
  } 

  dtstr_printf(ctl, fn, sizeof(fn),		/* make encoded filename 			*/
    "%s/%s.%08ld%s.%s", 
    put->u.roll.dir, 
    header->cam,
    frame, suffix,
    "enc");

  DT_C(dtfd_open,(ctl, &fd, fn, "w"));	/* open file to write 				*/

  if (strstr(fn, ".u") != NULL)			/* a ".u" in filename?				*/
  {
    DT_C(dtfd_write_all,(ctl, &fd,		/* write compression header to file	*/
      header, sizeof(*header)));
  }

  DT_C(dtfd_write_all,(ctl, &fd,		/* write data to file				*/
    data, header->l));
  DT_C(dtfd_close,(ctl, &fd));			/* close file 						*/
  fd.fd = -1;

  DT_C(dtstr_printf,(ctl,				/* remember the name 				*/
    put->u.roll.name,
	sizeof(put->u.roll.name),
    "%s", fn));
										/* ................................ */
  if (TABLEAU_ROLL_KEEP_CUR != 0)		/* we are saving current files? 	*/
  {
	lossless = *header;					/* copy this 						*/
	memset(&lossless_arg, 0,
      sizeof(lossless_arg));
	l = 0;

	DT_C(tableau_encode_uncompressed,(	/* do quickest lossless compression	*/
      param,
      &lossless_arg, image,
      &org,
      &lossless, &b, &l));

	dtstr_printf(ctl, fn, sizeof(fn),	/* make lossless filename 			*/
      "%s/%s.%08ld%s.%s", 
 	  put->u.roll.dir, 
      header->cam,
      frame, suffix,
      "cur");

    DT_C(dtfd_open,(ctl, &fd, fn,		/* open file to write 				*/
      "w"));
    DT_C(dtfd_write_all,(ctl, &fd,		/* write lossless header to file	*/
      &lossless, sizeof(lossless)));
    DT_C(dtfd_write_all,(ctl, &fd,		/* write data to file				*/
      b, lossless.l));
    DT_C(dtfd_close,(ctl, &fd));		/* close file 						*/
    fd.fd = -1;
  }

										/* ................................ */
  if (org.org !=
	  DT_COLOR_ORG_CB &&
	  org.org !=
	  DT_COLOR_ORG_CR)
    DT_C(tableau_roll_add,(ctl,			/* add new frame to roll 			*/
      put->u.roll.dir, 
      header->cam,
      frame));

  if (frame >= 
	  TABLEAU_ROLL_KEEP_ENC)
  {
	int is;
    dtstr_printf(ctl,					/* stale enc filename				*/
      fn, sizeof(fn),
      "%s/%s.%08ld%s.%s",
      put->u.roll.dir, 
      header->cam,
      frame -
      TABLEAU_ROLL_KEEP_ENC, suffix,
      "enc");
	dtos_isreadable(ctl, fn, &is);
	if (is)
	  dtos_unlink(ctl, fn);
  }

  if (frame >= 
	  TABLEAU_ROLL_KEEP_CUR)
  {
	int is;
	dtstr_printf(ctl,					/* stale cur filename				*/
      fn, sizeof(fn),
      "%s/%s.%08ld%s.%s",
      put->u.roll.dir, 
      header->cam,
      frame -
      TABLEAU_ROLL_KEEP_CUR, suffix,
      "cur");
	dtos_isreadable(ctl, fn, &is);
	if (is)
	  dtos_unlink(ctl, fn);
  }

cleanup:
  if (fd.fd != -1)
    DT_I(dtfd_close,(ctl, &fd));		/* close file 						*/

  if (b != NULL)
  DT_I(tableau_encode_uncompressed,(	/* clean up lossless compression	*/
    param,
    &lossless_arg, NULL, NULL,
    &lossless, &b, &l));

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
