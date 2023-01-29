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
DT_RCSID("app/tableau $RCSfile: get_ser.c,v $ $Revision: 1.8 $");
#include <dtack/yield.h>

#define WAIT_INC (500)					/* frame poll increment 			*/

#define CR (0x0d)
#define LF (0x0a)

/*..........................................................................
 * get next filename from list file
 * return to start of file if line is "loop"
 *..........................................................................*/

static
dt_rc_e
tableau_get_ser_list(
  dtparam_t *param,
  tableau_get_t *get,
  char *filename,
  int max)
{
  DT_F("tableau_get_ser_list");
  dt_ctl_t *ctl = &param->ctl;
  int l;

loop:
  
  filename[0] = '\0';					/* no prefix for now 				*/
  DT_Q(dtfd_read_line,(ctl,
    &get->u.ser.listfd, 
    filename+strlen(filename),
    max-strlen(filename)));

  l = strlen(filename);
  if (filename[0] == '\0')				/* no more lines in file? 			*/
    return dt_err(ctl, F,
      "end of list file %s",
      get->u.ser.listfd.filename);
  else
  if (l >= max-1 &&
	  filename[l-1] != LF)				/* line we just got is unfinished? 	*/
    return dt_err(ctl, F,
      "line in file %s longer than %d",
      get->u.ser.listfd.filename, max);

  while (l > 0 &&						/* nuke the crlf's 					*/
		 filename[l-1] == LF ||
		 filename[l-1] == CR)
	filename[--l] = '\0';

  if (!strcmp(filename, "loop"))		/* line from file is "loop"? 		*/
  {
	DT_Q(dtfd_seek,(ctl,				/* go back to start of file			*/
      &get->u.ser.listfd, 0,	
      DTFD_WHENCE_SET, NULL));
	get->u.ser.frame = 0;				/* clear frame count			    */
	goto loop;
  }
  else
	get->u.ser.frame++;					/* increment frame count		    */

  return DT_RC_GOOD;
}

/*..........................................................................
 * wait for next available file and return it's name
 *..........................................................................*/

static
dt_rc_e
tableau_get_ser_wait(
  dtparam_t *param,
  tableau_get_t *get,
  char *filename,
  int max)
{
  DT_F("tableau_get_ser_wait");
  dt_ctl_t *ctl = &param->ctl;
  int frame = get->u.ser.frame + 2;		/* frame we need to see				*/
  int isreadable;
  int skipped;
  long wait_max = param->max > 0?		/* max wait in milliseconds 		*/
    param->max: 180000;
  long wait_inc = (param->max -			/* wait increment in milliseconds 	*/
    (long)param->max) * 1000;
  if (wait_inc == 0)					/* use default increment if 0 		*/
    wait_inc = WAIT_INC;
  if (wait_inc > wait_max)
	wait_inc = wait_max;

  while (1)
  {
	dtstr_printf(ctl, filename, max,	/* make filename from template 		*/
      get->u.ser.listfn, frame);
	DT_Q(dtos_isreadable,(ctl,			/* see if file exists 				*/
      filename, &isreadable));
	if (!isreadable)					/* this frame not readable yet?		*/
	  break;
	frame++;							/* advance to next file 			*/
  } 

  skipped = frame - get->u.ser.frame;
  if (skipped == 2)						/* new frame not here yet? 			*/
  {
	long waited = 0;
    dt_dbg(ctl, F, DT_DBG_MASK_OBJECT,
      "waiting for %s", filename);
	while (1)
	{
	  DT_Q(dtos_delay,(ctl, wait_inc));	/* sleep a short while				*/
	  DT_Q(dtos_isreadable,(ctl,		/* see if file exists now			*/
        filename, &isreadable));
	  if (isreadable)					/* got it? 							*/
	    break;
	  waited += wait_inc;
	  if (waited >= wait_max)
	    return dt_err(ctl, F,
          "waited too long"
          " for new frame %s", 
          filename);
	}
  }
  else
  {
	frame--;
	if (skipped > 3)
      dt_dbg(ctl, F, DT_DBG_MASK_OBJECT,
        "skipped %d frame%s",
        skipped-3, skipped==4? "": "s");
  }

  frame--;								/* back up to last complete file 	*/
  dtstr_printf(ctl, filename, max,		/* make filename from template 		*/
    get->u.ser.listfn, frame);

  get->u.ser.frame = frame;
  dt_dbg(ctl, F, DT_DBG_MASK_OBJECT,
    "doing %s", filename);

  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
tableau_get_ser(
  dtparam_t *param,
  tableau_get_t *get,
  tableau_header_t **header,
  void **data,
  long *l)
{
  DT_F("tableau_get_ser");
  dt_ctl_t *ctl = &param->ctl;
  char filename[128];
  dtfd_t fd;
  dt_rc_e rc;

  fd.fd = -1;

  if (get->u.ser.listfn[0])				/* we have a template? 				*/
	DT_C(tableau_get_ser_wait,(param,	/* wait for/get next available file	*/
      get, 
      filename, sizeof(filename)))
  else									/* we have no template? 			*/
	DT_C(tableau_get_ser_list,(param,	/* get next line from file 			*/
      get, 
      filename, sizeof(filename)));
  
  if (*header == NULL)					/* don't have an old header? 		*/
  DT_C(dtos_malloc2,(ctl,				/* allocate space for header 		*/
    (void **)header,
    sizeof(tableau_header_t),
    F, "header"));
  memset((*header), 0,
    sizeof(tableau_header_t));

  DT_C(dtfd_open,(ctl, &fd,				/* open encoded file to read		*/
    filename, "r"));	

  if (strstr(filename, ".u") != NULL)	/* a ".u" in filename?				*/
  {
    DT_C(dtfd_read_all,(ctl, &fd,		/* read compression header			*/
      *header, sizeof(**header)));
  }
  else									/* no ".u" in filename? 			*/
  {
	unsigned long size;

	(*header)->l = 0;					/* set from file size later 		*/
	(*header)->n = get->u.ser.frame;	/* frame sequence number 			*/
	(*header)->ms = 0;					/* milliseconds when captured 		*/
	(*header)->xe = 0;					/* jpeg will give us frame size		*/
	(*header)->ye = 0;
	if (strstr(filename, "luminance"))	/* "luminance" in the file's name?	*/
	  (*header)->org =					/* black and white organization		*/
         DT_COLOR_ORG_LUMINANCE;
	else
	  (*header)->org =					/* color organization 				*/
         DT_COLOR_ORG_RGB888;
	strcpy((*header)->cam, "ser");		/* cam name 						*/
	strcpy((*header)->codec,			/* decoder to use 					*/
      TABLEAU_CODEC_JPEG);

	DT_C(dtfd_seek,(ctl, &fd, 0,		/* get jpeg file size 				*/
      DTFD_WHENCE_END, &size));
    (*header)->l = size;
  
	DT_C(dtfd_seek,(ctl, &fd, 0,		/* return to start of file 			*/
      DTFD_WHENCE_SET, NULL));
  } 

  if (*l < (*header)->l)				/* given buffer is too small? 		*/
  {
	if (*l != 0)						/* we even have a given buffer? 	*/
	  DT_Q(dtos_free2,(ctl, *data,
        F, "ser file data"));
	*l = 0;
    DT_Q(dtos_malloc2,(ctl, data,		/* space for the data 				*/
      DT_CAST(unsigned int, 
        (*header)->l),
      F, "ser file data"));
	*l = (*header)->l;					/* remember how big it is 			*/
  }
	 
  DT_C(dtfd_read_all,(ctl, &fd,			/* read jpeg data from file			*/
    *data, (*header)->l));
  
cleanup:
  if (fd.fd != -1)
    DT_I(dtfd_close,(ctl, &fd));		/* close jpeg file					*/

  return rc;
}

/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
tableau_get_ser_free(
  dtparam_t *param,
  tableau_get_t *get)
{
  dt_ctl_t *ctl = &param->ctl;
  dt_rc_e rc = DT_RC_GOOD;
  DT_F("tableau_get_ser_3");
  if (get->u.ser.listfd.fd != -1)
    DT_I(dtfd_close,(ctl, 
       &get->u.ser.listfd));
  return rc;
}

/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
tableau_get_ser_setup(
  dtparam_t *param,
  tableau_get_t *get,
  const char *spec)
{
  DT_F("tableau_get_ser_setup");
  dt_ctl_t *ctl = &param->ctl;
  const char *filename;

  filename = strchr(spec, ',');
  if (filename == NULL ||
	  filename[1] == '\0')
    return dt_err(ctl, F,
      "no listfile given in \"%s\"",
      spec);
  else
    filename++;							/* skip the comma 					*/

  if (filename[0] == '/' ||
	  filename[1] == ':')
    dtstr_printf(ctl,					/* don't prepend the path			*/
      get->u.ser.listfn,
      sizeof(get->u.ser.listfn), 
      "%s",
      filename);
  else
    dtstr_printf(ctl,					/* prepend the path					*/
      get->u.ser.listfn,
      sizeof(get->u.ser.listfn), 
      "%s/%s",
      param->path, filename);

  get->u.ser.listfd.fd = -1;

  DT_Q(dtfd_open,(ctl,					/* open list file for reading 		*/
    &get->u.ser.listfd,
    get->u.ser.listfn, "r"));
  
  DT_Q(tableau_get_ser_list,(param,		/* get first line from file 		*/
    get,
    get->u.ser.listfn,
    sizeof(get->u.ser.listfn)));

  if (strchr(get->u.ser.listfn, '%'))	/* first filename has a % in it?	*/
  {
	int i;
    DT_Q(dtfd_close,(ctl,				/* all we need from this file 		*/
      &get->u.ser.listfd));
	DT_Q(tableau_dir_scan,(ctl,			/* scan for first frame				*/
      get->u.ser.listfn,
      &get->u.ser.frame));
	for (i=0; i<2; i++)					/* see if previous files exist 		*/
	{
	  char fn[128];
	  int isreadable;
	  dtstr_printf(ctl, fn, sizeof(fn),	/* make filename from template 		*/
        get->u.ser.listfn, 
        get->u.ser.frame-1);
	  DT_Q(dtos_isreadable,(ctl,		/* see if file exists 				*/
        fn, &isreadable));
	  if (!isreadable)					/* this frame not readable yet?		*/
	    break;
	  get->u.ser.frame--;
	}
  }
  else									/* first filename has no % in it? 	*/
  {
	get->u.ser.listfn[0] = '\0';		/* clear the template 				*/
	DT_Q(dtfd_seek,(ctl,				/* go back to start of file			*/
      &get->u.ser.listfd, 0,	
      DTFD_WHENCE_SET, NULL));
  }

  get->function = tableau_get_ser;
  get->free = tableau_get_ser_free;

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
