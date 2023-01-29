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
  CHANGES
  20000306 DE added support for bare drive identifier on NT
  20000129 DE ifdef struct stat stuff
  19990814 DE added new functions
  19990114 DE changed debugging messages
 *..........................................................................*/




/*..........................................................................
| NAME
| Introduction - intro
| 
| DESCRIPTION
| As yet undocumented.
| 
| RETURN VALUES
| Function return value will be DT_RC_GOOD on success.  
| Any other return value indicates failure or noncompletion.
| When DT_RC_GOOD is not returned, any other return values may not be valid.
| 
| ERRORS
| As yet undocumented.
| 
| END
 *..........................................................................*/

#include <dtack/base.h>
DT_RCSID("source/low $RCSfile: os_futil.c,v $ $Revision: 1.1 $");
#include <dtack/errno.h>
#include <dtack/msw.h>
#include <dtack/os.h>

#ifndef DTDEPEND
#  if defined(DTCONFIG_WATCOM_DOS)
#    include <direct.h>
#  else
#  if !defined(DTCONFIG_MICROSOFT)
#    include <dirent.h>
#  endif
#  endif

#  ifdef DTCONFIG_IMPLIED_QNX4
#    include <unix.h>
#  endif

#  ifdef DTCONFIG_IMPLIED_SUNOS_OLD
     char *mktemp(char *);
#  endif
#endif

#ifdef DTCONFIG_MICROSOFT
#define FILENO _fileno
#define UNLINK _unlink
#define ISATTY _isatty
#define STAT _stat
#define STRUCT_STAT struct _stat
#else
#define FILENO fileno
#define UNLINK unlink
#define ISATTY isatty
#define STAT stat
#define STRUCT_STAT struct stat
#endif

/*..........................................................................*/
dt_rc_e
DTCONFIG_API1
dtos_tmpnam(
  dt_ctl_t *ctl,
  char *string,
  int l)
{
  DT_F("dtos_tmpnam");
  char buffer[L_tmpnam+1];
  char *b;
  b = tmpnam(buffer);					/* make a temporary name 			*/
  if (b == NULL)
    return dt_err(ctl, F,
      "errno %d in tmpnam()",
	  errno);  

  if ((int)strlen(buffer)+1 > l)		/* name is longer than space? 		*/
    return dt_err(ctl, F,
      "tmp file name length %d"
      " longer than given size %d",
      strlen(buffer)+1, l);

  strcpy(string, buffer);				/* copy back to caller's space		*/
  return DT_RC_GOOD;
}


/*..........................................................................*/
/*..........................................................................*/
#if defined (DTCONFIG_IMPLIED_UNIXLIKE) || \
    defined (DTCONFIG_IMPLIED_DOS)   || \
    defined (DTCONFIG_IMPLIED_MSW)
#define UNLINK_IMPLEMENTED 1

dt_rc_e
DTCONFIG_API1
dtos_unlink(							/* delete file by given name	    */
  dt_ctl_t *ctl,
  char *filename)
{
  DT_F("dtos_unlink");
  int sysrc;

  sysrc = UNLINK(filename);				/* do the system call			    */

  if (sysrc == -1)						/* system call failed?			    */
    return dt_err(ctl, F,
      "unlink(\"%s\") errno %d",
      filename, errno);  

  dt_dbg(ctl, F, 
    DT_DBG_MASK_IO,
    "unlinked \"%s\"", filename);

  return DT_RC_GOOD;
}
#endif
/*..........................................................................*/
#ifndef UNLINK_IMPLEMENTED
dt_rc_e
DTCONFIG_API1
dtos_unlink(
  dt_ctl_t *ctl,
  char *filename)
{
  DT_F("dtos_unlink");
  return dt_err(ctl, F,
    "function not implemented");
}
#endif

/*..........................................................................
 *..........................................................................*/

#if defined(DTCONFIG_IMPLIED_MSW32)
dt_rc_e
DTCONFIG_API1
dtos_isreadable(						/* see if filename is readable		*/
  dt_ctl_t *ctl,
  const char *filename,
  int *isreadable)
{
  DT_F("dtos_isreadable");

  if (filename != NULL)
  {
	HANDLE handle;
	handle = CreateFile(filename,		/* open file for reading			*/
      GENERIC_READ,
      0, NULL, OPEN_EXISTING, 0, NULL);
	if (handle == INVALID_HANDLE_VALUE)	/* open failed? 					*/
	{
	  if (DT_DBG(ctl, DT_DBG_MASK_OBJECT) &&
		  DT_DBG(ctl, DT_DBG_MASK_PIXEL))
      if (DT_DBG(ctl, DT_DBG_MASK_OBJECT))
	  {
	    int e = GetLastError();
		char s[128];
		DTERRNO_LASTS(e, s, sizeof(s));
	    dt_dbg(ctl, F, DT_DBG_MASK_OBJECT,
          "CreateFile(\"%s\", ..., OPEN_EXISTING, ...)"
          DTERRNO_LASTFS,
          filename, DTERRNO_LASTES(e, s));
      }
      *isreadable = 0;
    }
	else
	{
	  *isreadable = 1;
	  CloseHandle(handle);
	}
  }
  else
    *isreadable = 0;

  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/
#else  									/* not win32 						*/

dt_rc_e
DTCONFIG_API1
dtos_isreadable(						/* see if filename is readable		*/
  dt_ctl_t *ctl,
  const char *filename,
  int *isreadable)
{
  DT_F("dtos_isreadable");
  FILE *file;

  if (filename != NULL)
  {
	file = fopen(filename,				/* just try to open it			    */
      DTCONFIG_FOPEN_RBINARY);
  
	if (file == NULL)					/* open failed?						*/
	{
	  int e;
	  e = errno;
	  if (DT_DBG(ctl, DT_DBG_MASK_OBJECT) &&
		  DT_DBG(ctl, DT_DBG_MASK_PIXEL))
	  dt_dbg(ctl, F, DT_DBG_MASK_OBJECT,
        "fopen(\"%s\", \"%s\")"
        DTERRNO_FMT,
        filename, DTCONFIG_FOPEN_RBINARY,
        DTERRNO_ARG(e));
      *isreadable = 0;
    }
	else
	{
	  *isreadable = 1;
	  fclose(file);
	}
  }
  else
    *isreadable = 0;

  return DT_RC_GOOD;
}
#endif

/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtos_isdir(								/* see if cifname is a directory	*/
  dt_ctl_t *ctl,
  const char *dirname,
  int *isdir)
{
  DT_F("dtos_isdir");
  char dirname2[4];
  if (dirname == NULL)
  {
	*isdir = 0;
	return DT_RC_GOOD;
  }

  {
#if defined(DTCONFIG_IMPLIED_MSW32)
  DWORD attr = 0;
  UINT type = 0;

  if (strlen(dirname) == 2 &&			/* bare drive letter? 				*/
	  toupper(dirname[0]) >= 'A' &&
	  toupper(dirname[0]) <= 'Z' &&
	  dirname[1] == ':')
  {
	strcpy(dirname2, dirname);
	strcat(dirname2, "\\");
	dirname = dirname2;
	type = GetDriveType(dirname);
	*isdir =
	  type != DRIVE_UNKNOWN &&
	  type != DRIVE_NO_ROOT_DIR;
  }
  else
  {
    attr = GetFileAttributes(dirname);
										/* 2005-05-24 use bit comparison */
    if (attr & FILE_ATTRIBUTE_DIRECTORY)
      *isdir = 1;
    else
      *isdir = 0;
  }
#else
#if defined(DTCONFIG_MICROSOFT)
  struct _stat stat;
  char tmp[4];
  int sysrc;

  if (dirname[1] == ':' &&				/* looks like a drive letter? 		*/
	  (dirname[0] >= 'A' &&
	   dirname[0] <= 'Z' ||
	   dirname[0] >= 'a' &&
	   dirname[0] <= 'z'))
  {
	if (strlen(dirname) == 2)			/* bare drive and its colon? 		*/
	{
	  strcpy(tmp, dirname);
	  strcat(tmp, "/");
	  dirname = tmp;
	}
  }

  sysrc = _stat(dirname, &stat);
  *isdir = (sysrc == 0) && (stat.st_mode & _S_IFDIR);
#else
  struct stat statbuf;
  int sysrc = stat(dirname, &statbuf);
  *isdir = (sysrc == 0) && (statbuf.st_mode & S_IFDIR);
#endif
#endif
  }
  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtos_isatty_file(						/* see if file is a file or not		*/
  dt_ctl_t *ctl,
  FILE *file,
  int *is)
{
  *is = ISATTY(FILENO(file));
  return DT_RC_GOOD;
}


/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtos_isatty_fd(							/* see if fd is a file or not		*/
  dt_ctl_t *ctl,
  int fd,
  int *is)
{
  *is = ISATTY(fd);
  return DT_RC_GOOD;
}

/*..........................................................................
 * return 0 if error finding file 
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtos_futil_mtime(						/* return file's last mod time		*/
  dt_ctl_t *ctl,
  const char *filename,
  long *mtime)
{
  STRUCT_STAT statbuf;
  int sysrc;
  sysrc = STAT(filename, &statbuf);		/* get stat from filename 			*/
  if (sysrc >= 0)						/* stat worked?						*/
	*mtime = statbuf.st_mtime;
  else
	*mtime = 0;

  return DT_RC_GOOD;
}


/*..........................................................................
 * return 0 if error finding file 
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtos_futil_size(						/* return file's size in bytes		*/
  dt_ctl_t *ctl,
  const char *filename,
  long *size)
{
  STRUCT_STAT statbuf;
  int sysrc;
  sysrc = STAT(filename, &statbuf);		/* get stat from filename 			*/
  if (sysrc >= 0)						/* stat worked?						*/
	*size = statbuf.st_size;
  else
	*size = 0;

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
