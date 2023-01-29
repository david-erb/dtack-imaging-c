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
DT_RCSID("app/tableau $RCSfile: put_ftp.c,v $ $Revision: 1.8 $");

#define IPC (&(put)->u.ipc.ipc)
#define SCR (&(put)->u.scr.scr)

/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
tableau_put_ftp_setup(
  dtparam_t *param,
  tableau_put_t *put,
  const char *spec)
{
  DT_F("tableau_put_ftp");
  dt_ctl_t *ctl = &param->ctl;
  dtftp_t *ftp = &put->u.ftp.ftp;
  char *spec2 = NULL;
  char *directory;
  char *format;
  int isdir, isreadable;
  char line[256];
  char *filename = line;
  int ntokens;
# define NEED 5
  int i;
  dtfd_t fd;
  char *p[NEED];
  int ftp_initialized = 0;
  dt_rc_e rc;

  fd.fd = -1;

  DT_C(dtos_strdup,(ctl, spec,			/* local copy of spec 				*/
    &spec2));
  directory = strchr(spec2, ',');		/* directory starts after comma 	*/
  if (directory == NULL)
  {
	rc = dt_err(ctl, F,
      "no directory found"
      " in spec \"%s\"",
      spec);
	goto cleanup;
  }
  directory++;							/* skip the comma 					*/
  format = strrchr(directory, '/');
  if (format == NULL)
  {
	rc = dt_err(ctl, F,
      "no filename"
      " in spec \"%s\"",
      spec);
	goto cleanup;
  }
  *format++ = '\0';						/* skip the slash					*/

  if (param->config == NULL)
  {
	rc = dt_err(ctl, F,
      "no config directory given"
      " (need -config)");
	goto cleanup;
  }
  
  DT_C(dtos_isdir,(ctl, param->config,	/* make sure directory exists 		*/
    &isdir));
  if (!isdir)
  {
	rc = dt_err(ctl, F,
      "Sorry, %s is not a directory.",
      param->config);
	goto cleanup;
  }
  
  dtstr_printf(ctl, 
    filename, sizeof(line),
    "%s/%s", param->config, 
    "ftp.cfg");

  DT_Q(dtos_isreadable,(ctl,
    filename, &isreadable));
  if (!isreadable)
  {
	rc = dt_err(ctl, F,
      "Sorry, %s is not a readable file.",
      filename);
	goto cleanup;
  }

  DT_C(dtfd_open,(ctl, &fd,				/* open config file to read 		*/
    filename, "r"));

  strcpy(line, "rocket,");
  DT_C(dtfd_read_line,(ctl, &fd,		/* read config file 				*/
    line+strlen(line), 
    sizeof(line)-strlen(line)));
  DT_C(dtstr_tokenize,(ctl, line, ' ',	/* carve up the config line 		*/
    &ntokens));
  if (ntokens != NEED)					/* minimal check 					*/
  {
	rc = dt_err(ctl, F,
      "config file has %d tokens"
      ", but need exactly %d",
      ntokens, NEED);
	goto cleanup;
  }

  p[0] = line;
  for (i=1; i<NEED; i++)				/* separate the tokens 				*/
    p[i] = p[i-1] + strlen(p[i-1]) + 1;
  
  if (p[NEED-1][0] != ';')				/* check last token 				*/
  {
	rc = dt_err(ctl, F,
      "last token on line"
      " does not begin with ';'");
	goto cleanup;
  }
  
  DT_C(dtftp_init,(ctl, ftp,			/* get ready for ftp 				*/
    p[0], p[1], p[2], p[3]));
  ftp_initialized = 1;

  DT_C(dtftp_connect,(ctl, ftp));		/* connect to ftp server 			*/

  DT_C(dtftp_chdir,(ctl, ftp,			/* set the destination directory 	*/
    directory));

  if (strstr(directory, ".u") != NULL)	/* a ".u" in directory name?		*/
    put->u.ftp.output_header = 1;		/* include our header 				*/
  
  strcpy(put->u.ftp.format, format);	/* remember the filename format 	*/

cleanup:
  if (rc != DT_RC_GOOD &&
	  ftp_initialized)
  {
	DT_I(dtftp_uninit,(ctl, ftp));		/* close ftp server connection 		*/
  }
  if (fd.fd != -1)
    DT_I(dtfd_close,(ctl, &fd));		/* close config file 				*/
  if (spec2  != NULL)
    DT_I(dtos_free2,(ctl, spec2,
      F, spec2));
  return rc;
}

/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
tableau_put_ftp(
  dtparam_t *param,
  tableau_put_t *put,
  dtimage_t *image,						/* full image 						*/
  tableau_header_t *header,				/* compressed header 				*/
  void *data,							/* compressed data 					*/
  int cc)
{
  DT_F("tableau_put_ftp");
  dt_ctl_t *ctl = &param->ctl;
  dtftp_t *ftp = &put->u.ftp.ftp;
  tableau_len_t frame;
  char fn[TABLEAU_FILENAME_MAX];
  
  if (data == NULL)						/* supposed to clean up? 			*/
  {
	DT_Q(dtftp_uninit,(ctl, ftp)); 
	return DT_RC_GOOD;
  }
  
  DT_Q(tableau_roll_init,(ctl,			/* init roll file 					*/
    param->config, header->cam, NULL));

  DT_Q(tableau_roll_get,(ctl,			/* current frame number				*/
    param->config, header->cam,
    -1, &frame));

  frame++;

  dtstr_printf(ctl, fn, sizeof(fn),		/* make output filename 			*/
    put->u.ftp.format,
    header->cam,
    frame);

  dt_dbg(ctl, F, DT_DBG_MASK_WIN,
    "sending %s", fn);

  DT_Q(dtftp_put_start,(ctl, ftp, fn));	/* start the file putting process 	*/
  if (put->u.ftp.output_header)
  {
    DT_Q(dtftp_put_data,(ctl, ftp,		/* output the header 				*/
      header, sizeof(*header)));
  }
  DT_Q(dtftp_put_data,(ctl, ftp,		/* output the data 					*/
    data, header->l));

  DT_Q(dtftp_put_finish,(ctl, ftp));	/* finish the outputting			*/

  DT_Q(dtstr_printf,(ctl,				/* remember the name 				*/
    put->u.ftp.name,
	sizeof(put->u.ftp.name),
    "%s", fn));

  DT_Q(tableau_roll_add,(ctl,			/* add new frame to roll 			*/
    param->config,
    header->cam,
    frame));

  if (frame >= 
	  TABLEAU_ROLL_KEEP_ENC)
  {
    dtstr_printf(ctl, fn, sizeof(fn),	/* make stale filename				*/
      put->u.ftp.format,
      header->cam,
      frame -
      TABLEAU_ROLL_KEEP_ENC);
	if (strcmp(put->u.ftp.name, fn))	/* stale filename is different? 	*/
	  DT_Q(dtftp_delete,(ctl, ftp,		/* delete the stale file			*/
        fn));
  }

  return DT_RC_GOOD;
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
