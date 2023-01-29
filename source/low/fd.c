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
  20000304 DE change backslashes in filenames
  20000204 DE added signature
  20000104 DE properly initialize dtfd_clear for egcs
  19990810 DE use strlen if write length is -1
  19990721 DE added buffering scheme to speed up short reads
  19990718 DE check mask before debugging (for efficiency only)
  19981123 DE fixed bug in debug
  19980923 DE added rz to input modes
  19980905 DE add explicit member isopen
  19980716 DE remove sunpro1 compiler warnings
 *..........................................................................*/


/*..........................................................................
| TITLE dtfd()
|dxinclude(liblink1.dx)
| 
|!SUMMARY
| Works together to open, read, write, and close files.
| Provides a thin layer above the native operating system file interface.
| Writing and reading works for sockets as well as regular files.
|
| The :typedef struct dtfd_t: data structure used by all these functions
| contains three read-only public members:
| :fd.fd:		the native OS file descriptor
| :fd.filename:	the filename at dtfd_open()
| :fd.file:		the native OS file pointer, NULL if none
|
|.Salient Concepts
| You initialize the ~fd~ object descriptor with dtfd_open() or dtfd_fd().
| You then pass ~fd~ to all other functions.
| 
| You may examine several members of the ~fd~ object descriptor.
| Under no circumstances should you change any members.
| 
| END
 *..........................................................................*/

#include <dtack/base.h>
DT_RCSID("source/low $RCSfile: fd.c,v $ $Revision: 1.2 $");
#include <dtack/os.h>
#include <dtack/errno.h>

#include <dtack/msw.h>

const dtfd_t dtfd_clear = {			/* example of a cleared structure 	*/
  0, NULL, NULL, NULL, 0, 0, 0, 
  {NULL, NULL, 0, 0}};

#ifndef DTDEPEND
# if DTPRJ_ZLIB == DT_1
#  include <zlib.h>
#ifdef __cplusplus
extern "C" {
#endif
   int gztransparent(gzFile file);
#ifdef __cplusplus
}
#endif
#endif
#endif

#ifdef DTCONFIG_MICROSOFT
#define READ _read
#define WRITE _write
#define LSEEK _lseek
#define CLOSE _close
#define FILENO _fileno
#else
#define READ read
#define WRITE write
#define LSEEK lseek
#define WRITE write
#define CLOSE close
#define FILENO fileno
#endif

/*..........................................................................
| NAME
| dtfd_open - open a file
| 
| SUMMARY
| As yet undocumented.
| 
| END
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtfd_open(								/* open a file						*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtfd_t *fd,							/* file object descriptor 			*/
  const char *filename,					/* name of file to open 			*/
  const char *mode)						/* file open mode string 			*/
{
  DT_F("dtfd_open");
  int possibly_compressed = 0;
#if 0
  char filename[8192];
#endif
  void *gzfile = NULL;
  FILE *file = NULL;
  char *fn;
  int i;
  int opened = 0;

#if 0
  if (strlen(filename0) >				/* check for name too long 			*/
	  sizeof(filename))
  {
	strncpy(filename, filename0, 60);
	filename[60] = '\0';
	return dt_err(ctl, F,
      "filename \"%s\"... too long",
      filename);
  }

  for (i=0; filename0[i]; i++)			/* copy to temp space 				*/
	filename[i] =
	  filename0[i] == '\\'?				/* change backslashes to slashes 	*/
        '/': filename0[i];
  filename[i] = '\0';
#endif

  if (!strcmp(mode, "rz"))				/* reading possible gzipped file? 	*/
  {
	possibly_compressed = 1;
	mode = DTCONFIG_FOPEN_RBINARY;
  }
  else
  if (!strcmp(mode, "r"))
	mode = DTCONFIG_FOPEN_RBINARY;
  else
  if (!strcmp(mode, "w"))
	mode = DTCONFIG_FOPEN_WBINARY;
  else
  if (!strcmp(mode, "a"))
	mode = DTCONFIG_FOPEN_ABINARY;
  else
  if (!strcmp(mode, "rw"))
	mode = DTCONFIG_FOPEN_RWBINARY;
  else
  if (!strcmp(mode, "wr"))
	mode = DTCONFIG_FOPEN_WRBINARY;
  else
  if (!strcmp(mode, "rt"))
	mode = DTCONFIG_FOPEN_RTEXT;
  else
  if (!strcmp(mode, "wt"))
	mode = DTCONFIG_FOPEN_WTEXT;
  else
  if (!strcmp(mode, "at"))
	mode = DTCONFIG_FOPEN_ATEXT;
  else
  if (!strcmp(mode, "rwt"))
	mode = DTCONFIG_FOPEN_RWTEXT;
  else
  if (!strcmp(mode, "wrt"))
	mode = DTCONFIG_FOPEN_WRTEXT;
  else
    return dt_err(ctl, F,
      "unrecognized mode \"%s\" for \"%s\"",
      mode, filename);

  if (!strcmp(filename, "stdin"))
  {
	file = stdin;
	opened = 1;
  }
  else
  if (!strcmp(filename, "stdout"))
  {
	file = stdout;
	opened = 1;
  }
# if DTPRJ_ZLIB == DT_1					/* using gz library? 				*/
  else
  if (possibly_compressed &&			/* could be a .gz file? 			*/
	  !strcmp(mode,						/* open for binary reading? 		*/
        DTCONFIG_FOPEN_RBINARY))
  {
	gzfile = gzopen(filename, mode);	/* use gz lib to open 				*/
	if (gzfile == NULL)					/* open failed?						*/
      return dt_err(ctl, F,
        "gzopen(\"%s\", \"%s\")"
        DTERRNO_FMT,
        filename, mode,
        DTERRNO_ARG(errno));
	if (gztransparent(gzfile))			/* gz file is transparent? 			*/
	{
	  gzclose(gzfile);					/* don't use zlib on it 			*/
	  gzfile = NULL;
	}
	else
	  opened = 1;
  }
# endif

  if (!opened)
  {
	file = fopen(filename, mode);
	if (file == NULL)					/* open failed?						*/
      return dt_err(ctl, F,
        "fopen(\"%s\", \"%s\")"
        DTERRNO_FMT,
        filename, mode,
        DTERRNO_ARG(errno));
  }

  DT_MEMSET(fd, '\0', sizeof(*fd));		/* clear structure memory			*/

  DT_Q(dtos_strdup,(ctl,				/* space for the filename			*/
    filename, &fn));

  fd->b.m = DTFD_BUFFER_SIZE;			/* nominal buffer size 				*/
  DT_Q(dtos_malloc2,(ctl,				/* get space for internal buffer 	*/
    (void **)&fd->b.b, fd->b.m, 
    F, "buffer"));
  
  fd->sig = DTFD_SIG;					/* mark with signature 				*/
  fd->gzfile = gzfile;					/* save stuff in structure 			*/
  fd->file = file;						/* save stuff in structure 			*/
  if (file != NULL)
	fd->fd = FILENO(file);
  else
	fd->fd = -1;
  fd->filename = fn;
  fd->isopen = 1;						/* mark the structure open 			*/
 
# if DTPRJ_ZLIB == DT_1					/* using gz library? 				*/
  if (fd->gzfile != NULL)
  dt_dbg(ctl, F, 
    DT_DBG_MASK_IO|DT_DBG_MASK_BIGIO,
    "gzopened \"%s\" mode \"%s\"",
    fd->filename, mode);
  else
# endif
  dt_dbg(ctl, F, 
    DT_DBG_MASK_IO|DT_DBG_MASK_BIGIO,
    "opened \"%s\" mode \"%s\" on fd %d",
    fd->filename, mode, fd->fd);

  return DT_RC_GOOD;
}

/*..........................................................................
| NAME
| dtfd_fd() - wrap object around existing fd
| 
| SUMMARY
| As yet undocumented.
| 
| END
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtfd_fd(								/* wrap object around existing fd	*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtfd_t *fd,							/* file object descriptor 			*/
  const char *filename,					/* filename associated with fd 		*/
  int sysfd)							/* the system file descriptor 		*/
{
  DT_F("dtfd_fd");
  char *fn;

  DT_MEMSET(fd, '\0', sizeof(*fd));		/* clear structure memory			*/

  DT_Q(dtos_strdup,(ctl,				/* space for the filename			*/
    filename, &fn));

  fd->b.m = DTFD_BUFFER_SIZE;			/* nominal buffer size 				*/
  DT_Q(dtos_malloc2,(ctl,				/* get space for internal buffer 	*/
    (void **)&fd->b.b, fd->b.m, 
    F, "buffer"));

  fd->sig = DTFD_SIG;					/* mark with signature 				*/
  fd->file = NULL;
  fd->fd = sysfd;
  fd->filename = fn;
  fd->isopen = 1;						/* mark the structure open 			*/
 
  dt_dbg(ctl, F, 
    DT_DBG_MASK_IO|DT_DBG_MASK_BIGIO,
    "system fd %d assigned to \"%s\"",
    fd->fd, fd->filename);

  return DT_RC_GOOD;
}

/*..........................................................................
| NAME
| dtfd_close() - close and keep file
| 
| SUMMARY
| As yet undocumented.
| 
| END
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtfd_close(								/* close end keep file				*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtfd_t *fd)							/* file object descriptor 			*/
{
  DT_F("dtfd_close");

  if (!DTFD_ISOPEN(ctl, fd))			/* structure is not open? 			*/
	return DT_RC_GOOD;

# if DTPRJ_ZLIB == DT_1					/* using gz library? 				*/
  if (fd->filename != NULL &&
	  fd->gzfile != NULL)
  {
    dt_dbg(ctl, F, DT_DBG_MASK_IO|DT_DBG_MASK_BIGIO,
      "gzclosing \"%s\"",
      fd->filename);
	gzclose(fd->gzfile);
  }
  else
# endif
  if (fd->filename &&
	  fd->fd != -1)
  {
    dt_dbg(ctl, F, DT_DBG_MASK_IO|DT_DBG_MASK_BIGIO,
      "closing \"%s\" on fd %d (%s)",
      fd->filename, fd->fd,
      fd->file? "file": "fd");
	if (fd->file == NULL)
      CLOSE(fd->fd);
    else
    if (fd->file != stdin &&			/* it's a file needing closing?		*/
        fd->file != stdout)
      fclose(fd->file);
  }
  
  if (fd->b.b)							/* we have internal buffer base?	*/
    DT_Q(dtos_free2,(ctl,				/* free it 							*/
      fd->b.b, F, "buffer"));

  if (fd->filename)						/* we have filename space? 			*/
    DT_Q(dtos_free2,(ctl,				/* free it 							*/
      fd->filename, F, fd->filename));
  
  DT_MEMSET(fd, '\0', sizeof(*fd));		/* clear structure memory			*/
  fd->fd = -1;
  return DT_RC_GOOD;
}

/*..........................................................................
| NAME
| dtfd_delete() - close and delete file
| 
| SUMMARY
| As yet undocumented.
| 
| END
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtfd_delete(							/* close and delete file			*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtfd_t *fd)							/* file object descriptor 			*/
{
  DT_F("dtfd_delete");
  dt_rc_e rc = DT_RC_GOOD;
# if DTPRJ_ZLIB == DT_1					/* using gz library? 				*/
  if (fd->filename != NULL &&
	  fd->gzfile == NULL)
  {
    dt_dbg(ctl, F, 
      DT_DBG_MASK_IO|DT_DBG_MASK_BIGIO,
      "closing/deleting gzopened \"%s\"",
      fd->filename);
	gzclose(fd->gzfile);				/* let gz close the file 			*/
	DT_G(dtos_unlink,(ctl,				/* then delete the file				*/
      fd->filename));
  }
  else
#endif
  if (fd->file == NULL)
  {
    rc = dt_err(ctl, F,
      "cannot delete \"%s\""
      " instantiated by dtfd_fd",
      fd->filename);
  }
  else
  {
    dt_dbg(ctl, F, 
      DT_DBG_MASK_IO|DT_DBG_MASK_BIGIO,
      "closing/deleting \"%s\" on fd %d",
      fd->filename, fd->fd);
    if (fd->file != stdin &&
        fd->file != stdout)
    {
      fclose(fd->file);					/* close the file 					*/
      DT_G(dtos_unlink,(ctl,			/* then delete the file				*/
        fd->filename));
    }
  }
  DT_I(dtos_free,(ctl, fd->filename));
  DT_MEMSET(fd, '\0', sizeof(fd));		/* clear structure memory			*/
  return rc;
}

/*..........................................................................
| NAME
| dtfd_read() - copy to user buffer from internal buffer
| 
| SUMMARY
| Copy contents of internal buffer to provided ~buffer~.
| Return number of bytes read in ~*got~.
| 
| END
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtfd_read_buffer(						/* copy buffer from input buffer	*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtfd_t *fd,							/* file object descriptor 			*/
  void *buf,							/* buffer to put data into			*/
  long want,							/* maximum number of bytes wanted 	*/
  long *got)							/* returned number of bytes gotten 	*/
{
  DT_F("dtfd_read_buffer");
  long mask = DT_DBG_MASK_IO |
    (want > 1? DT_DBG_MASK_BIGIO: 0);

  *got = DT_MIN(fd->b.n, want);			/* how much we need from buffer 	*/

  if (*got)								/* anything being returned? 		*/
  {
	DT_MEMCPY(buf, fd->b.p, *got);		/* copy stuff out of buffer 		*/
	fd->b.p += *got;					/* adjust buffer 					*/
	fd->b.n -= *got;
	if (DT_DBG(ctl, mask))
	dt_dbg(ctl, F, mask,
      "from \"%s\" buffer using %ld,"
      " %ld now left in buffer pos=%ld", 
      fd->filename, *got, fd->b.n, fd->pos);
  }

  return DT_RC_GOOD;
}

/*..........................................................................
| NAME
| dtfd_read() - read buffer from file
| 
| SUMMARY
| Reads data from file.
| Only makes one try.
| Return number of bytes read in ~*got~.
| Zero bytes returned in ~*got~ probably means end of file.
| Not an error when ~*got~ is returned less than ~want~.
| 
| END
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtfd_read(								/* read buffer from file			*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtfd_t *fd,							/* file object descriptor 			*/
  void *buf1,							/* buffer to put data into			*/
  long want1,							/* maximum number of bytes wanted 	*/
  long *got1)							/* returned number of bytes gotten 	*/
{
  DT_F("dtfd_read");
  long mask = DT_DBG_MASK_BIGIO;
  void *buf2;
  long want2;
  long got2;
  int sysrc;

  if (want1 > DTCONFIG_READ_MAX)
    want1 = DTCONFIG_READ_MAX;

  if (fd->b.n)							/* internal buffer not empty? 		*/
  {
    DT_Q(dtfd_read_buffer,(ctl, fd,		/* get stuff from internal buffer	*/
      buf1, want1, got1));
	fd->pos += *got1;					/* update current position 			*/
	return DT_RC_GOOD;
  }

  if (fd->b.m > want1)					/* short read? 						*/
  {
	want2 = fd->b.m;
	buf2 = fd->b.b;
  }
  else
  {
	want2 = want1;
	buf2 = buf1;
  }

# if DTPRJ_ZLIB == DT_1					/* using gz library? 				*/
  if (fd->gzfile != NULL)				/* did gzopen? 						*/
	sysrc = gzread(fd->gzfile, buf2, 
      (unsigned)want2);
  else
# endif
  if (fd->file == NULL)					/* did not do fopen?			    */
    sysrc = READ(fd->fd, buf2,			/* do the read raw					*/
      (size_t)want2);	
  else
    sysrc = fread(buf2, 1,				/* do the read buffered				*/
      (size_t)want2, fd->file);	

  if (sysrc == -1)						/* read failed?						*/
# if DTPRJ_ZLIB == DT_1
  if (fd->gzfile != NULL)				/* did gzopen? 						*/
  {
	int gzerrno;
	const char *gzerrmsg = gzerror(
      fd->gzfile, &gzerrno);
	if (gzerrno == Z_ERRNO)				/* system error? 					*/
	  return dt_err(ctl, F,
        "gzread(\"%s\", buf, %ld)"
        DTERRNO_FMT,
        fd->filename, want2,
        DTERRNO_ARG(errno));
	else								/* gz lib error? 					*/
	  return dt_err(ctl, F,
        "gzread(\"%s\", buf, %ld) %s",
        fd->filename, want2,
        gzerrmsg);
  }
  else
# endif
  if (fd->file == NULL)					/* did not do fopen?			    */
    return dt_err(ctl, F,
      "read(\"%s\" fd=%d, buf, %ld)"
      DTERRNO_FMT,
      fd->filename, fd->fd, want2,
      DTERRNO_ARG(errno));
  else
    return dt_err(ctl, F,
      "fread(buf, 1, %ld, \"%s\" fd=%d)"
      DTERRNO_FMT,
      want2, fd->filename, fd->fd,
      DTERRNO_ARG(errno));
  
  if (sysrc < 0)						/* return bytes we got				*/
    got2 = 0x10000L + sysrc;
  else
    got2 = sysrc;

  if (DT_DBG(ctl, mask))
  dt_dbg(ctl, F, mask,
    "from \"%s\" read %ld of %ld"
    " into %s buffer", 
    fd->filename, got2, want2,
    buf2 == fd->b.b? "internal": "user");

  if (buf2 == fd->b.b)					/* we read into internal buffer? 	*/
  {
	fd->b.n = got2;
	fd->b.p = fd->b.b;
    DT_Q(dtfd_read_buffer,(ctl, fd,		/* get stuff from internal buffer	*/
      buf1, want1, got1));
  }
  else									/* we read into users's buffer? 	*/
  {
	*got1 = got2;
  }

  fd->pos += *got1;						/* update current position 			*/
  
  return DT_RC_GOOD;
}

/*..........................................................................
| NAME
| dtfd_read_all() - fill buffer from file
| 
| SUMMARY
| Reads data from file.
| Will continue to read until all ~want~ bytes have been read.
| Returns an error if end of file is reached before buffer is full.
| 
| END
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtfd_read_all(							/* fill buffer from file			*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtfd_t *fd,							/* file object descriptor 			*/
  void *buf,							/* buffer to return bytes in 		*/
  long want)							/* maximum number of bytes wanted 	*/
{
  DT_F("dtfd_read_all");
  long have;
  long got;
  long mask = DT_DBG_MASK_IO |
    (want > 1? DT_DBG_MASK_BIGIO: 0);

  for (have=0; have<want; have+=got)
  {
	DT_Q(dtfd_read,(ctl, fd,			/* do the read						*/
      (char *)buf+(size_t)have, 
      DT_MIN(want-have, 
        DTCONFIG_READ_MAX),
      &got));
	if (DT_DBG(ctl, mask))
	dt_dbg(ctl, F, mask,
      "from \"%s\" now have read %ld of %ld (%ld%%)",
      fd->filename, got+have, want,
      (100L * (long)(got+have)) /
      (want? want: 1));
    if (got == 0)						/* end-of-file?					    */
      break;
  }

  if (have < want)						/* didn't get all we wanted?		*/
    return dt_err(ctl, F,
      "only read %ld out of %ld from \"%s\"",
      have, want, fd->filename);

  return DT_RC_GOOD;
}

/*..........................................................................
| NAME
| dtfd_read_line() - read file up to new line
| 
| SUMMARY
| Read data from file.
| Continue to read until ~max-1~ bytes have been read,
| or until a newline character is reached.
| If ~max-1~ bytes are reached before the newline is found,
| leave remaining bytes on line to be read by next call.
| If reached, the newline character is returned in the buffer.
| Always place an end-of-string character in the returned buffer.
| 
| Have an error if end of file is reached before buffer is full.
| 
| END
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtfd_read_line(							/* read file up to new line			*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtfd_t *fd,							/* file object descriptor 			*/
  void *buf,							/* buffer to return bytes in 		*/
  long max)								/* maximum size of buffer 			*/
{
  DT_F("dtfd_read_line");
  long have;
  long got;
  long mask = DT_DBG_MASK_BIGIO;
  char *b = (char *)buf;
  char c;
  int n = 0;

  if (max <= 0)
    return dt_err(ctl, F,
      "invalid buffer size %d", max);

  for (have=0; have<max-1; have++)
  {
	DT_Q(dtfd_read,(ctl, fd, &c, 1,		/* do the read						*/
      &got));
    if (got == 0)						/* end-of-file?					    */
      break;
	b[n++] = c;							/* stuff character into buffer 		*/
	if (c == '\n')						/* quit looping upon newline 		*/
	  break;
  }

  b[n] = '\0';							/* end string always 				*/
  
  if (DT_DBG(ctl, mask))
  if (got > 0 && c != '\n')
    dt_dbg(ctl, F, mask,
	  "line longer than %d", max-1);
  else
    dt_dbg(ctl, F, mask,
	  "line length %d", n);

  return DT_RC_GOOD;
}

/*..........................................................................
| NAME
| dtfd_skip() - skip bytes by reading
| 
| SUMMARY
| Reads and discards data from file.
| Will continue to read until all ~want~ bytes have been read.
| Returns an error if end of file is reached before buffer is full.
| 
| END
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtfd_skip(								/* skip bytes by reading			*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtfd_t *fd,							/* file object descriptor 			*/
  long want)							/* number of bytes wanted 			*/
{
  DT_F("dtfd_skip");
  long max;
  long have;
  long got;
  void *buf = NULL;
  long mask = DT_DBG_MASK_IO |
    (want > 1? DT_DBG_MASK_BIGIO: 0);
  dt_rc_e rc;

  max = DT_MIN(want, 8192);
  DT_C(dtos_malloc2,(ctl,
    &buf, max, F, "buf"));
  
  for (have=0; have<want; have+=got)
  {
	DT_C(dtfd_read,(ctl, fd, buf,		/* do the read						*/
      DT_MIN(want-have, max),
      &got));
	if (DT_DBG(ctl, mask))
	dt_dbg(ctl, F, mask,
      "from \"%s\" now have skipped %ld of %ld (%ld%%)",
      fd->filename, got+have, want,
      (100L * (long)(got+have)) /
      (want? want: 1));
    if (got == 0)						/* end-of-file?					    */
      break;
  }

  if (have < want)						/* didn't get all we wanted?		*/
    return dt_err(ctl, F,
      "only skipped %ld out of %ld from \"%s\"",
      have, want, fd->filename);

cleanup:
  if (buf != NULL)
	DT_I(dtos_free2,(ctl, buf, F, "buf"));
  
  return rc;
}

/*..........................................................................
| NAME
| dtfd_write() - write buffer to file
| 
| SUMMARY
| Writes data to file.
| Only makes one try.
| Returns actual number of bytes successfully written.
| Does not return an error if not all ~want~ bytes can be written.
| 
| END
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtfd_write(								/* write buffer to file				*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtfd_t *fd,							/* file object descriptor 			*/
  const void *buf,						/* buffer holding data 				*/
  long want,							/* number of bytes to write 		*/
  long *got)							/* returned number of bytes written */
{
  DT_F("dtfd_write");
  long mask = DT_DBG_MASK_IO |
    (want > 1? DT_DBG_MASK_BIGIO: 0);
  int sysrc;

  if (buf == NULL ||					/* caller wants to flush?			*/
	  want == 0)
  {
	if (fd->file == NULL)				/* did not do fopen?			    */
	{
										/* no way to flush 					*/
	}
	else								/* came through fopen?			    */
	{
	  if (DT_DBG(ctl, mask))
      dt_dbg(ctl, F, mask,
        "flushing \"%s\"",
        fd->filename);
	  sysrc = fflush(fd->file);
	  if (sysrc == -1)					/* flush failed?					*/
		return dt_err(ctl, F,
          "fflush(\"%s\" fd=%d)"
          DTERRNO_FMT,
          fd->filename, fd->fd,
          DTERRNO_ARG(errno));
	}
	if (got != NULL)
	  *got = 0;
  }

  if (want > DTCONFIG_WRITE_MAX)
    return dt_err(ctl, F, 
      "%ld is too many bytes to write at once",
      want);

  if (fd->file == NULL)					/* did not do fopen?			    */
    sysrc = WRITE(fd->fd, buf,			/* write raw					    */
      (size_t)want);
  else									/* came through fopen?			    */
    sysrc = fwrite(buf, 1,				/* write buffered					*/
      (size_t)want, fd->file);

  if (sysrc == -1)						/* write failed?					*/
  if (fd->file == NULL)
    return dt_err(ctl, F,
      "write(\"%s\" fd=%d, buf=0x%08lx, %ld)"
      DTERRNO_FMT,
      fd->filename, fd->fd, buf, want,
      DTERRNO_ARG(errno));
  else
    return dt_err(ctl, F,
      "fwrite(buf=0x%08lx, 1, %ld, \"%s\" fd=%d)"
      DTERRNO_FMT,
      buf, want, fd->filename, fd->fd,
      DTERRNO_ARG(errno));

  if (got != NULL)
  if (sysrc < 0)						/* return bytes we got				*/
    *got = 0x10000L + sysrc;
  else
    *got = sysrc;

  if (DT_DBG(ctl, mask))
  if (DT_DBG(ctl, mask))
    dt_dbg(ctl, F, mask,
      "on \"%s\" wrote buf=0x%08lx %ld of %ld", 
      fd->filename, buf, (long)sysrc, want);

  return DT_RC_GOOD;
}

/*..........................................................................
| NAME
| dtfd_write_all() - write data to file
| 
| SUMMARY
| Writes data to file.
| Will continue to write until all ~want~ bytes have been written.
| Use :strlen(buf): if ~want~ is -1.
| Returns an error if a system error is found before all bytes are written.
| 
| END
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtfd_write_all(							/* write all data to file			*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtfd_t *fd,							/* file object descriptor 			*/
  const void *buf,						/* buffer holding data 				*/
  long want)							/* number of bytes to write 		*/
{
  DT_F("dtfd_write_all");
  long have;
  long got;
  long mask = DT_DBG_MASK_IO |
    (want > 1? DT_DBG_MASK_BIGIO: 0);

  if (want == -1)						/* use string's length? 			*/
	want = strlen((const char *)buf);

  if (buf == NULL ||					/* caller wants to flush?			*/
	  want == 0)
  {
	DT_Q(dtfd_write,(ctl, fd,			/* do the flush						*/
      buf, want, NULL));
	return DT_RC_GOOD;
  }

  for (have=0; have<want; have+=got)
  {
	DT_Q(dtfd_write,(ctl, fd,			/* do the write						*/
      (char *)buf+(size_t)have, 
      DT_MIN(want-have, 
        DTCONFIG_WRITE_MAX),
      &got));
    if (DT_DBG(ctl, mask))
	  dt_dbg(ctl, F, mask,
        "on \"%s\" now have written %ld of %ld (%ld%%)",
        fd->filename, got+have, want,
        (100L * (long)(got+have)) /
        (want? want: 1));
    if (got == 0)						/* end-of-file?					    */
      break;
  }

  if (have < want)						/* didn't get all we wanted?		*/
    return dt_err(ctl, F,
      "only wrote %ld out of %ld on \"%s\"",
      have, want, fd->filename);

  return DT_RC_GOOD;
}

/*..........................................................................
| NAME
| dtfd_seek() - set file position for read or write
| 
| SUMMARY
| As yet undocumented.
| 
| END
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtfd_seek(								/* seek to position					*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtfd_t *fd,							/* file object descriptor 			*/
  long offset,							/* offset byte number 				*/
  dtfd_whence_e whence,					/* type of seek 					*/
  long *position)						/* returned absolute position 		*/
{
  DT_F("dtfd_seek");
  long mask = DT_DBG_MASK_IO |
    DT_DBG_MASK_BIGIO;
  long sysrc;

  if (fd->file == stdout)
    return dt_err(ctl, F, 
      "cannot seek on \"%s\"", 
      fd->filename);

  if (fd->gzfile != NULL ||				/* this is a gz file? 				*/
	  fd->file == stdin)				/* or stdin? 						*/
  {
	long skip;
	if (whence == DTFD_WHENCE_END)
      return dt_err(ctl, F, 
        "cannot seek from end on file \"%s\"", 
        fd->filename);
	else
	if (whence == DTFD_WHENCE_SET)
	{
	  skip = offset - fd->pos;
	  if (skip < 0)
        return dt_err(ctl, F, 
          "cannot seek backwards"
          " from %ld to %ld on file \"%s\"", 
          fd->pos, offset,
          fd->filename);
	}
	else
	if (whence == DTFD_WHENCE_CUR)
	{
	  skip = offset;
	  if (skip < 0)
        return dt_err(ctl, F, 
          "cannot seek backwards"
          " by %ld on file \"%s\"", 
          skip, fd->filename);
	}
	else
	  return dt_err(ctl, F,
		"invalid whence=%d on file \"%s\"", 
        whence, fd->filename);

	DT_Q(dtfd_skip,(ctl, fd, skip));	/* skip by reading 					*/

	if (position != NULL)
	  *position = fd->pos;				/* update from current position 	*/

	return DT_RC_GOOD;
  }

  if (whence == DTFD_WHENCE_CUR)		/* relative offset? 				*/
  {
	offset += fd->pos;					/* convert to absolute 				*/
	whence = DTFD_WHENCE_SET;
  }

  if (fd->file == NULL)					/* did not do fopen?			    */
  {
    sysrc = LSEEK(fd->fd, 
      offset, whence);
  }
  else
  {
    sysrc = fseek(fd->file, 
      offset, whence);
	if (sysrc != -1)
	  sysrc = ftell(fd->file);
  }

  if (sysrc == -1)
    return dt_err(ctl, F, 
      "in %cseek(\"%s\", %ld, %d)"
      DTERRNO_FMT, 
      fd->file == NULL? 'l': 'f',
      fd->filename, offset, whence,
      DTERRNO_ARG(errno));

  fd->pos = sysrc;

  if (position != NULL)
    *position = sysrc;					/* position file is left at		    */

  fd->b.n = 0;							/* clear internal input buffer 		*/
  fd->b.p = fd->b.b;

  if (DT_DBG(ctl, mask))
  dt_dbg(ctl, F, mask,
    "in \"%s\" seek %ld whence %d leave pos at %ld",
    fd->filename, offset, whence, sysrc);

  return DT_RC_GOOD;
}

/*..........................................................................
| NAME
| dtfd_size() - return uncompressed file size
| 
| SUMMARY
| As yet undocumented.
| 
| END
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtfd_size(								/* return uncompressed file size	*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtfd_t *fd,							/* file object descriptor 			*/
  long *size)							/* returned uncompressed size		*/
{
  DT_F("dtfd_size");
  long mask = DT_DBG_MASK_IO |
    DT_DBG_MASK_BIGIO;
  long sysrc;

  if (fd->file == stdin ||
	  fd->file == stdout)
    return dt_err(ctl, F, 
      "cannot determine size of \"%s\"", 
      fd->filename);

  if (fd->gzfile != NULL)				/* this is a gz file?				*/
  {
	FILE *file;
	file = fopen(fd->filename, "r");	/* open the gz file directly 		*/
	if (file == NULL)
	  return dt_err(ctl, F,
        "fopen(\"%s\", \"r\")"
        DTERRNO_FMT,
        fd->filename,
        DTERRNO_ARG(errno));
	fseek(file, -4, SEEK_END);			/* seek to final 4 bytes 			*/
	*size =								/* get uncompressed size 			*/
      (fgetc(file)      ) |
      (fgetc(file) <<  8) |
      (fgetc(file) << 16) |
      (fgetc(file) << 24);
	fclose(file);						/* close direct file 				*/
  }
  else
  {
	long pos;
	DT_Q(dtfd_seek,(ctl, fd, 0,			/* get current position 			*/
      DTFD_WHENCE_CUR, &pos));
	DT_Q(dtfd_seek,(ctl, fd, 0,			/* seek to end to get size 			*/
      DTFD_WHENCE_END, size));
	DT_Q(dtfd_seek,(ctl, fd, pos,		/* return to current position 		*/
      DTFD_WHENCE_SET, NULL));
  }

  if (DT_DBG(ctl, mask))
  dt_dbg(ctl, F, mask,
    "in \"%s\" file size is %ld",
    fd->filename, *size);

  return DT_RC_GOOD;
}

#ifndef DTCONFIG_IMPLIED_MSW
#include <sys/time.h>
/*..........................................................................
 * timeout of -1 means wait forever
 * doesn't work on WIN32!!!!
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtfd_poll(								/* wait for fd to be active			*/
  dt_ctl_t *ctl,
  dtfd_t *fd,
  dtfd_poll_e what,
  long milliseconds,
  const char *waiting_for,
  int *isready)
{
  DT_F("dtfd_poll");
  fd_set fds;
  int in = (what == DTFD_POLL_READ);
  int first = -1;
  int ready = -1;

  if (fd->gzfile != NULL)
    return dt_err(ctl, F, 
      "cannot poll on gzfile \"%s\"", 
      fd->filename);

  do {
	struct timeval timeout;
	int sysrc;

	if (first == 1 && 
		DT_DBG(ctl, DT_DBG_MASK_BIGIO))
	{	
      dt_dbg(ctl, F, DT_DBG_MASK_BIGIO,
        "waiting for %s on fd %d",
        waiting_for, fd->fd);
	  first = 0;
	}

	if (first != -1)
	{
	  DT_Q(dt_yield,(ctl, waiting_for));
	}

	if (milliseconds == 0)				/* no waiting?						*/
	{
	  timeout.tv_sec = 0;				/* loop poll time	 				*/
	  timeout.tv_usec = 0;
	}
	else
	if (milliseconds == -1)				/* waiting forever? 				*/
	{
	  timeout.tv_sec = 1;				/* loop poll time one second		*/
	  timeout.tv_usec = 0;
	}
	else
	if (milliseconds < 1000)			/* very short wait?					*/
	{
	  timeout.tv_sec = 
        milliseconds/1000;
	  timeout.tv_usec = 
        (milliseconds%1000)*1000;
	}
	else								/* waiting longer than one sec?	    */
	{
	  timeout.tv_sec = 1;				/* loop poll time one second		*/
	  timeout.tv_usec = 0;
	}

	FD_ZERO(&fds);
	FD_SET(fd->fd, &fds);				/* wait for activity				*/
	sysrc = select(fd->fd+1,
      in? &fds: (fd_set *)NULL, 
      in? (fd_set *)NULL: &fds, 
      NULL, &timeout);
	if (sysrc == -1 &&
		errno == EINTR)
	{	
	  return dt_err(ctl, F,
	    "%s got unexpected signal",
        fd->filename);
	}
	if (sysrc == -1)					/* other failure in select()?	    */
      return dt_err(ctl, F,
        "%s select(%d, fds,"
        " NULL, NULL, (%ld,%ld))"
        " waiting for %s"
        " gave errno %d",
        fd->filename,
        fd->fd+1,
        timeout.tv_sec, timeout.tv_usec,
        waiting_for,
        errno);

	ready = FD_ISSET(fd->fd, &fds);

	if (first == -1)
	  first = 1;

	if (milliseconds == 0)				/* this is a poll?				    */
	  break;

	if (milliseconds != -1)				/* not waiting forever?			    */
	{
	  milliseconds -= 1000;
	  if (milliseconds <= 0)			/* waited long enough?			    */
	    break;
	}

  } while(!ready);						/* input not yet available? 		*/

  if (isready != NULL)					/* caller wants to know? 			*/
    *isready = ready;

  return DT_RC_GOOD;
}
#endif



/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtfd_rev(								/* deliver revision 				*/
  dt_ctl_t *ctl,						/* environment control 				*/
  const char **rev_string,				/* returned rev string 				*/
  int *rev_major,						/* returned rev major 				*/
  int *rev_minor,						/* returned rev minor 				*/
  long *rev_date)						/* returned rev date 				*/
{
  if (rev_string != NULL)
    *rev_string = "dtfd lib";
  if (rev_major != NULL)
    *rev_major = 3;
  if (rev_minor != NULL)
    *rev_minor = 6;
  if (rev_date != NULL)
    *rev_date = 19990722;
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
