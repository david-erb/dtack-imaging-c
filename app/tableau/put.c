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
DT_RCSID("app/tableau $RCSfile: put.c,v $ $Revision: 1.8 $");

#define IPC ((put)->u.ipc.ipc)

/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
tableau_put_null(
  dtparam_t *param,
  tableau_put_t *put,
  dtimage_t *image,						/* full image 						*/
  tableau_header_t *header,				/* compressed header 				*/
  void *data,							/* compressed data 					*/
  int cc)								/* color component 					*/
{
  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
tableau_put_setup(
  dtparam_t *param,
  tableau_put_t *put,
  const char *spec)
{
  DT_F("tableau_put_setup");
  dt_ctl_t *ctl = &param->ctl;
  char *spec2 = NULL;
  dt_rc_e rc;
  
  if (spec == NULL ||
	  strlen(spec) == 0)
    return dt_err(ctl, F,
      "NULL or empty input name");

  DT_C(dtos_time,(ctl, &put->start));	/* remember starting time 			*/

#define IS(TYPE) (DT_ABBREV(spec, TYPE))
  
										/* ................................ */
  if (IS("ftp"))
  {
	put->type = TABLEAU_PUT_FTP;
	put->function = tableau_put_ftp;
    DT_C(tableau_put_ftp_setup,(param,
      put, spec));
  }
  else									/* ................................ */
  if (IS("socket") ||
	  IS("rocket") ||
	  IS("qnx4")   ||
	  IS("qbaby")  ||
      IS("http"))
  {
	put->type = TABLEAU_PUT_IPC;
	put->function = tableau_put_ipc;
    if (IS("http"))						/* supposed to do URL thing first? 	*/
	{
      DT_C(tableau_http_start,(			/* tell httpd to start us a server 	*/
         param, spec, &spec2));
	  spec = spec2;
	}
	DT_C(dtos_malloc2,(ctl,				/* space for ipc structure 			*/
      (void **)&IPC, sizeof(*IPC),
      F, "ipc"));
    DT_C(tableau_ipc_setup,(param,
      spec, "get", IPC));				/* tell server to get what we put	*/
  }
  else									/* ................................ */
  if (IS("hws"))
  {
	put->type = TABLEAU_PUT_SCR;
	put->function = tableau_put_scr;
    DT_C(tableau_put_scr_setup,(param,
      put, spec));
  }
  else									/* ................................ */
  if (IS("roll"))
  {
	char *name = strchr(spec, ',');
	int isdir;
	
	put->type = TABLEAU_PUT_ROLL;
	put->function = tableau_put_roll;

	if (name == NULL)
	  return dt_err(ctl, F,
        "no name given");
	name++;								/* skip the comma 					*/
	dtstr_printf(ctl,					/* make roll directory name 		*/
      put->u.roll.dir,
      sizeof(put->u.roll.dir), 
      "%s%s%s",
      param->path,
  	  strlen(param->path) == 0? "":
#ifdef DTCONFIG_IMPLIED_UNIXLIKE
        "/",
#else
        "\\",
#endif
      name);
#ifdef NONO
    DT_C(dtos_isdir,(ctl,				/* make sure directory exists 		*/
      put->u.roll.dir, &isdir));
	if (!isdir)
	{
 	  rc = dt_err(ctl, F,
        "\"%s\" is not a directory",
        put->u.roll.dir);
	  goto cleanup;
	}
#endif
  }
  else									/* ................................ */
  if (IS("null"))
  {
	put->function = tableau_put_null;
  }
  else									/* ................................ */
  if (IS("file"))
  {
	char *name = strchr(spec, ',');
	if (name == NULL)
	  return dt_err(ctl, F,
        "no name given");
	name++;								/* skip the comma 					*/
	put->type = TABLEAU_PUT_FILE;
	put->function = tableau_put_file;
    DT_C(tableau_put_file_setup,(param,
      put, name));
  }
  else									/* ................................ */
  {
	rc = dt_err(ctl, F,
	  "invalid output spec \"%s\"", 
      spec);
	goto cleanup;
  }

 cleanup:
  if (spec2 != NULL)
    DT_I(dtos_free2,(ctl, spec2,
      F, "spec from server"));
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
