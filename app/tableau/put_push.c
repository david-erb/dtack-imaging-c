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
DT_RCSID("app/tableau $RCSFILE: put_push.c,v $ $Revision: 1.8 $");

#define BOUNDARY "The-Data-Boundary"	/* the server push separator 		*/

# define EMIT(S) 						\
  DT_C(dtipc_write_all,(ctl, ipc, 		\
    (S), strlen(S)));

/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
tableau_put_push(
  dtparam_t *param,
  tableau_put_t *put,
  dtimage_t *image,						/* full image 						*/
  tableau_header_t *header,				/* compressed header 				*/
  void *data,							/* compressed data 					*/
  int cc)								/* color component 					*/
{
  DT_F("tableau_put_push");
  dt_ctl_t *ctl = &param->ctl;
  dtipc_t *ipc = put->u.push.ipc;		/* for short 						*/
  char s[16];
  dt_rc_e rc = DT_RC_GOOD;

  if (data == NULL)						/* supposed to clean up? 			*/
  {
	return rc;
  }

  if (put->u.push.flag == 0)			/* haven't emitted header yet? 		*/
  {
    EMIT("HTTP/1.0 200\r\n");
    EMIT(
      "Content-type: multipart/x-mixed-replace;boundary=");
	EMIT(BOUNDARY);
    EMIT("\r\n");
    EMIT("\r\n");
	put->u.push.flag = 1;
  }

  EMIT("--");
  EMIT(BOUNDARY);
  EMIT("\r\n");

  EMIT("Content-type: image/jpeg\r\n");
  EMIT("Content-length: ");
  dtstr_printf(ctl, s, sizeof(s),
    "%ld", header->l);
  EMIT(s);
  EMIT("\r\n");
  EMIT("\r\n");

  DT_C(dtipc_write_all,(ctl, ipc,		/* write data to push				*/
    data, header->l));

  EMIT("\r\n");
  EMIT("--");
  EMIT(BOUNDARY);
  EMIT("\r\n");

cleanup:

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
