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

#include <tabdrone.h>
DT_RCSID("app/tabdrone $RCSfile: dir.c,v $ $Revision: 1.8 $");
#include <dtack/msw.h>

#ifndef DTDEPEND
#ifndef DTCONFIG_MICROSOFT
#include <dirent.h>
#else
typedef struct {
  char *name;
  HANDLE handle;
} DIR;

typedef struct dirent {
  char *d_name;
};

										/* ................................ */
static
DIR *
opendir(
  const char *dirname)
{
  DIR *dir = malloc(sizeof(DIR));
  dir->name = malloc(strlen(dirname) + 3);
  strcpy(dir->name, dirname);
  strcat(dir->name, "/*");
  dir->handle = NULL;
  return dir;
}

										/* ................................ */
static
void
closedir(
  DIR *dir)
{
  if (dir->handle != NULL)
    FindClose(dir->handle);
  if (dir->name != NULL)
	free(dir->name);
  free(dir);
  dir->handle = NULL;
}

										/* ................................ */
static
void
rewinddir(
  DIR *dir)
{
  if (dir->handle != NULL)
    FindClose(dir->handle);
  dir->handle = NULL;
}

										/* ................................ */
static
struct dirent *
readdir(
  DIR *dir)
{
  static struct dirent d;
  static WIN32_FIND_DATA finddata;
  if (dir->handle == NULL)
  {
	dir->handle = FindFirstFile(
      dir->name, &finddata);
	if (dir->handle == INVALID_HANDLE_VALUE)
	{
	  dir->handle = NULL;
	  return NULL;
	}
  }
  else
  {
	BOOL ok = FindNextFile(
      dir->handle, &finddata);
	if (!ok)
	  return NULL;
  }
  d.d_name = finddata.cFileName;
  return &d;
}
#endif
#endif

/*..........................................................................
 * find last filename matching template
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
tabdrone_dir_scan(
  tabdrone_t *tabdrone,					/* application state 				*/
  const char *template,
  tabdrone_frame_t *pframe)
{
  DT_F("tabdrone_dir_scan");
  dt_ctl_t *ctl = tabdrone->ctl;
  char *dot = ".";
  char *slash = "/";
  char *dirname = NULL;
  const char *filename;
  DIR *dir = NULL;
  struct dirent *dirent;
  int matched = 0;
  int ignored = 0;
  dt_rc_e rc = DT_RC_GOOD;

  filename = strrchr(template, '/');	/* find slash in template 			*/
  
  if (filename == template)				/* only slash is at start? 			*/
  {
	dirname = slash;
	filename++;
  }
  else
  if (filename != NULL)					/* slash somewhere in the middle? 	*/
  {
    DT_C(dtos_strdup,(ctl, template,
      &dirname));
	*strrchr(dirname, '/') = '\0';
    filename = dirname+strlen(dirname)+1;
  }
  else									/* no slash? 						*/
  {
	dirname = dot;
    filename = template;
  }

  dir = opendir(dirname);
  if (dir == NULL)
  {
	rc = dt_err(ctl, F, 
      "cannot opendir \"%s\"", 
      dirname);
	goto cleanup;
  }
  rewinddir(dir);						/* go to start of directory 		*/

  *pframe = 0;
  while (1)
  {
	int n;
	tabdrone_frame_t frame;
    dirent = readdir(dir);				/* read next directory entry 		*/
    if (dirent == NULL)					/* end of directory? 				*/
      break;
	n = sscanf(dirent->d_name,			/* scan for frame number			*/
      filename, &frame);	
	if (n == 1)							/* filename matches template?		*/
	{
	  matched++;
	  *pframe = DT_MAX(frame, *pframe);	/* remember highest frame number 	*/
	}
	else
	{
	  dt_dbg(ctl, F, DT_DBG_MASK_OBJECT,
        "ignoring \"%s\"", dirent->d_name);
	  ignored++;
	}
  } 

  *pframe = *pframe - 1;

  dt_dbg(ctl, F, DT_DBG_MASK_OBJECT,
    "ignored %d entries, matched %d,"
    " highest frame is %ld",
    ignored, matched, *pframe);

cleanup:
  if (dir != NULL)
    closedir(dir);

  if (dirname != dot &&					/* we strdup'ed the dirname? 		*/
	  dirname != slash &&
	  dirname != NULL)
    DT_I(dtos_free2,(ctl, dirname,
      F, "template"));

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
