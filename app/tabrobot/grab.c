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
DT_RCSID("app/tableau $RCSfile: get_fb.c,v $ $Revision: 1.8 $");

/*..........................................................................
 *..........................................................................*/

static
dt_rc_e
DTCONFIG_API1
tableau_get_fb(
  tabgrab_t *tabgrab)
{
  DT_F("tableau_get_fb");
  dt_ctl_t *ctl = &tabgrab->ctl;
  tableau_header_t _header, *header = &_header;
  unsigned long n = fb->rowsize * fb->ye;
  dt_rc_e rc = DT_RC_GOOD;

  memset(header, 0,						/* clear header 					*/
    sizeof(tableau_header_t));
  header->l = n;
  header->n = frame;
  header->org = DT_COLOR_ORG_RGB888;	/* return org type we deliver	    */
  strcpy(header->codec,					/* deliver as uncompressed data 	*/
    TABLEAU_CODEC_UNCOMPRESSED);

  DT_C(dtfb_control,(ctl, fb,			/* start acquiring again			*/
    DTFB_CONTROL_ACQUIRE));

  DT_C(dtos_malloc2,(ctl, &buffer,		/* get space for buffer 			*/
    n, F, "buffer"));

  DT_Q(dtfb_getblock,(ctl, fb,			/* suck bytes out 					*/
    buffer, 0, 0, fb->xe, fb->ye));
  
  DT_C(dtfd_seek,(ctl, fd,				/* go back to start of file			*/
    0, DTFD_WHENCE_SET, NULL));

  DT_C(dtfd_write_all,(ctl, fd,			/* write compression header to file	*/
      header, sizeof(*header)));

  DT_C(dtfd_write_all,(ctl, fd,			/* write data to file				*/
    buffer, header->l));

  DT_C(dtfd_write_all,(ctl, fd,			/* flush the file					*/
    NULL, 0));

cleanup:
  if (buffer != NULL)
	DT_I(dtos_free2,(ctl, buffer,		/* free buffer 						*/
      F, "buffer"));
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
