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
| Introduction - intro
| 
| DESCRIPTION
| This class of export function packs the arguments together
| into an RPC-like stream and uses dtwinc functions to send them.
| 
| END
 *..........................................................................*/

#include <dtack/base.h>
DT_RCSID("source/high $RCSfile: xchg_wxx.c,v $ $Revision: 1.1 $");

#include <dtack/lut8.h>
#include <dtack/xchg.h>
#include <dtack/mem.h>
#include <dtack/str.h>
#include <dtack/dib.h>
#include <dtack/wmf.h>
#include <dtack/os.h>
#include <dtack/msw.h>

#define DESC "Microsoft Windows"

#ifdef DTCONFIG_IMPLIED_MSW32
#define SetMetaFileBits(H) (H)
#define GetMetaFileBits(H) (H)
#endif

#ifndef DTCONFIG_IMPLIED_MSW
#define HANDLE int
#endif

typedef struct {
  char DTCONFIG_HUGE *buffer;
  long current;
  long maxsize;
  HANDLE handle;
} arg_t;

/*..........................................................................
 *..........................................................................*/

#ifndef DTCONFIG_IMPLIED_MSW

static long global_size = 0;
static char *global_pointer = NULL;

#define GMEM_MOVEABLE       0x0002
#define GMEM_SHARE          0x2000

										/* ................................ */
static
HANDLE GlobalAlloc(unsigned int flags, 
  unsigned long size)
{
  HANDLE handle = 1111;
  char filename[128];
  sprintf(filename, "/tmp/%d.mem", handle);
  unlink(filename);
  global_size = size;
  global_pointer = malloc(global_size);
  printf("GlobalAlloc: unlinked %s, size now %ld\n", filename, global_size);
  return handle;
}
										/* ................................ */
static
char *GlobalLock(HANDLE handle)			/* get pointer to global memory 	*/
{
  char filename[128];
  FILE *file;
  sprintf(filename, "/tmp/%d.mem", handle);
  file = fopen(filename, "r");
  if (file != NULL)
  {
    if (global_pointer != NULL)
      free(global_pointer);
	global_pointer = malloc(1000000);	/* enough for a reasonable file 	*/
#ifdef NONO
    fread(global_pointer, 1,			/* blow off placeable header 		*/
      sizeof(dtwmf_placeable_t), file);
#endif
    global_size = fread(global_pointer,
      1, 1000000, file);
	global_pointer = realloc(
      global_pointer, global_size);
	fclose(file);
    printf("GlobalLock: read %s, size now %ld\n", filename, global_size);
  }
  else
    printf("GlobalLock: no such file %s, size stays %ld\n", filename, global_size);
  return global_pointer;
}
										/* ................................ */
static
int GlobalUnlock(HANDLE handle)			/* release global memory 			*/
{
  char filename[128];
  FILE *file;
  sprintf(filename, "/tmp/%d.mem", handle);
  file = fopen(filename, "w");
  if (file != NULL)
  {
	dtwmf_placeable_t placeable;
    global_size = fwrite(global_pointer, 1, global_size, file);
	fclose(file);
    printf("GlobalUnlock: wrote %s, size %ld\n", filename, global_size);
  }
  return 0;
}
										/* ................................ */
static
long GlobalSize(HANDLE handle)
{
  return global_size;
}
										/* ................................ */
static
HANDLE GlobalFree(HANDLE handle)
{
  free(global_pointer);
  printf("GlobalFree: freed %ld\n", global_size);
  global_size = 0;
  global_pointer = NULL;
  return 0;
}
										/* ................................ */
static
HANDLE GetMetaFileBits(HANDLE handle)	/* get global memory handle 		*/
{
  return handle;
}
										/* ................................ */
static
HANDLE SetMetaFileBits(HANDLE handle)	/* restore metafile 				*/
{
  return handle;
}
#endif

/*..........................................................................
 * spec is of the form handle+xe+ye
 *..........................................................................*/

static
dt_rc_e
dtxchg_wxx_check(
  dt_ctl_t *ctl,
  const char *spec,
  HANDLE *phandle,
  dtxy_t *pxe,
  dtxy_t *pye)
{
  DT_F("dtxchg_wxx_check");
  const char *s;
  char *end;
  long longval;
  HANDLE handle;
  dtxy_t xe, ye;
  dt_rc_e rc;

  s = spec;
										/* ................................ */
  longval = strtol(s, &end, 0);			/* try the conversion			    */

  if (end == s ||						/* make sure handle is a number 	*/
      *end != '+' ||					/* followed by a plus 				*/
	  longval < 0L ||					/* make sure handle is reasonable 	*/
	  longval > 65535L)
  {
    rc = dt_err(ctl, F,
      "\"%s\" has a bad handle",
      spec);
	goto cleanup;
  }
  handle = (HANDLE)longval;

										/* ................................ */
  s = end + 1;
  longval = strtol(s, &end, 0);			/* try the conversion			    */

  if (end == s ||						/* make sure handle is a number 	*/
      *end != '+' ||					/* followed by a plus 				*/
	  longval < 0L ||					/* make sure value is reasonable 	*/
	  longval > 65535L)
  {
    rc = dt_err(ctl, F,
      "\"%s\" has a bad xe value",
      spec);
	goto cleanup;
  }
  xe = (dtxy_t)longval;
										/* ................................ */
  s = end + 1;
  longval = strtol(s, &end, 0);			/* try the conversion			    */

  if (end == s ||						/* make sure handle is a number 	*/
      *end != '\0' ||					/* followed by end of string		*/
	  longval < 0L ||					/* make sure value is reasonable 	*/
	  longval > 65535L)
  {
    rc = dt_err(ctl, F,
      "\"%s\" has a bad ye value",
      spec);
	goto cleanup;
  }
  ye = (dtxy_t)longval;
										/* ................................ */

  if (phandle != NULL)
    *phandle = handle;
  if (pxe != NULL)
    *pxe = xe;
  if (pye != NULL)
    *pye = ye;
cleanup:
  return rc;
}

/*..........................................................................
 *..........................................................................*/
static
void
memcpy_f2h(
  char DTCONFIG_HUGE *h,
  unsigned long k,
  char *f,
  unsigned int n)
{
  unsigned int m = (unsigned int)		/* amount in first segment 			*/
    (65536UL - (k & 0x0000ffffUL));
  if (m > n)							/* not bleeding into next segment?	*/
    m = n;
  memcpy((void DTCONFIG_FAR *)(h+k),	/* copy first part 					*/
    f, m);
  if (m < n)							/* bleeding into next segment? 		*/
    memcpy(
      (void DTCONFIG_FAR *)(h+k+m),		/* copy second part 				*/
      f+m, n-m);
}

/*..........................................................................
 *..........................................................................*/

static
void
memcpy_h2f(
  char *f,
  char DTCONFIG_HUGE *h,				/* huge memory source 				*/
  unsigned long k,						/* huge offset 						*/
  unsigned int n)
{
  unsigned int m = (unsigned int)		/* amount in first segment 			*/
    (65536UL - (k & 0x0000ffffUL));
  if (m > n)							/* not bleeding into next segment?	*/
    m = n;
  memcpy(f,								/* copy first part 					*/
	(void DTCONFIG_FAR *)(h+k), m);
  if (m < n)							/* bleeding into next segment? 		*/
    memcpy(f+m,
      (void DTCONFIG_FAR *)(h+k+m),		/* copy second part 				*/
      n-m);
}

/*..........................................................................
 * called import to get data
 *..........................................................................*/

static
DTCONFIG_API1
dt_rc_e
writefunc(
  dt_ctl_t *ctl,
  void *arg,
  void *data,
  unsigned int size)
{
  DT_F("dtxchg_wxx_export_writefunc");
  arg_t *a = (arg_t *)arg;

  if (a->current + size > a->maxsize)	/* stay inside buffer 				*/
    return dt_err(ctl, F,
      "attempt to access"
      " global memory block %d"
      " past its size %ld",
      a->handle, a->maxsize);

  memcpy_f2h(a->buffer, a->current,		/* copy tp global memory 			*/
    data, size);
  
  a->current += size;					/* increment position 				*/

  return DT_RC_GOOD;
}

/*..........................................................................
 * Export image to in-memory metafile.
 * Do not put a placeable header in the memory.
 * Expect mfspec to be of the form handle:xe:ye.
 * Check but ignore handle, xe and ye.
 * Always allocate a new global handle and make a metafile in it.
 * Always emit a feedback type 3 entry indicating the metafile handle.
 *..........................................................................*/

static
dt_rc_e
export_wmf_handle(
  dt_ctl_t *ctl,
  const char *mfspec,
  const char *id,
  dtimage_t *image,
  int xpos,
  int ypos)
{
  DT_F("dtxchg_wxx_export_wmf_handle");
  dtwmf_metahead_t metahead;
  arg_t a;
  HANDLE handle = 0;
  HANDLE wmfhandle = (HANDLE)NULL;
  dt_rc_e rc;

  DT_MEMSET(&a, 0, sizeof(a));

  rc = dtimage_check_1f(ctl,			/* check image validity			    */
    image, id, NULL,
    DTIMAGE_FLAG_PAGED, F);
  if (rc != DT_RC_GOOD)
    return rc;

  DT_C(dtwmf_metahead_init,(ctl,		/* init metafile header buffer 		*/
    &metahead, image));

  dt_dbg(ctl, F, DT_DBG_MASK_OBJECT,
    "<%s> mtSize %ld", id,
    *(unsigned long *)
    metahead.fileheader.mtSize);

  DT_C(dtxchg_wxx_check,(ctl,			/* see if we are given a handle 	*/
    mfspec, &handle, NULL, NULL));
  handle = (HANDLE)NULL;				/* for now, ignore handle			*/
  a.handle = handle;

  if (a.handle != (HANDLE)NULL)			/* we are given a handle? 			*/
  {
	long size;
	size = GlobalSize(a.handle);
	if (size == 0)						/* but handle does not give size? 	*/
	{
	  rc = dt_err(ctl, F,
        "GlobalSize("
        " %d) gave 0", a.handle);
  	  goto cleanup;
	}
	if (size <							/* check the size 					*/
        *(unsigned long *)
        metahead.fileheader.mtSize * 2L)
	{
	  rc = dt_err(ctl, F,
        "GlobalSize("
        " %d) = %ld not big enough,"
        " need %ld", a.handle, size,
        *(unsigned long *)
        metahead.fileheader.mtSize * 2L);
  	  goto cleanup;
	}
  }
  else									/* not given a handle? 				*/
  {
    a.handle = GlobalAlloc(				/* allocate space for metafile 		*/
      GMEM_SHARE | GMEM_MOVEABLE,
      *(unsigned long *)
      metahead.fileheader.mtSize * 2L);
    if (a.handle == (HANDLE)NULL)
    {
	  rc = dt_err(ctl, F,
        "GlobalAlloc("
        "GMEM_SHARE | GMEM_MOVEABLE,"
        " %ld) failed",
        *(unsigned long *)
        metahead.fileheader.mtSize * 2L);
  	  goto cleanup;
    }
  }

  a.maxsize = GlobalSize(a.handle);

  dt_dbg(ctl, F, DT_DBG_MASK_OBJECT,
    "<%s> a.handle %d, GlobalSize %ld", id,
    a.handle, a.maxsize);

  a.buffer = GlobalLock(a.handle);		/* get pointer to space 			*/
  if (a.buffer == NULL)
  {
	rc = dt_err(ctl, F,
      "GlobalLock(%d) failed",
      a.handle);
	goto cleanup;
  }

  dt_dbg(ctl, F, DT_DBG_MASK_OBJECT,
    "<%s> a.buffer 0x%08lx", 
    id, a.buffer);

  a.current = 0;
  DT_C(dtwmf_export,(ctl,				/* export the data 					*/
    writefunc, &a, image,
    xpos, ypos));

  GlobalUnlock(a.handle);				/* release lock 					*/
  a.buffer = NULL;

  wmfhandle = SetMetaFileBits(			/* turn memory into metafile 		*/
    a.handle);
  if (wmfhandle == (HANDLE)NULL)
  {
	rc = dt_err(ctl, F,
      "SetMetaFileBits(%d) failed",
      a.handle);
	goto cleanup;
  }

  a.handle = (HANDLE)NULL;

  dt_dbg(ctl, F, DT_DBG_MASK_OBJECT,
    "<%s> will return hMF %d", 
    id, wmfhandle);
  
cleanup:
  if (a.buffer != NULL)					/* we have a memory lock? 			*/
    GlobalUnlock(a.handle);
  if (rc != DT_RC_GOOD &&
	  wmfhandle != (HANDLE)NULL)		/* and we have metafile handle? 	*/
    a.handle = GetMetaFileBits(			/* turn metafile back to memory 	*/
      wmfhandle);
  if (rc != DT_RC_GOOD &&
	  a.handle != (HANDLE)NULL &&		/* we have a memory handle? 		*/
      handle == (HANDLE)NULL)			/* which we were not given?		    */
    GlobalFree(a.handle);
  if (rc == DT_RC_GOOD)
  {
    char s[80];
    dtstr_printf(ctl, s, sizeof(s),	
      "DT IMAGE OUTPUT");
    DT_Q(dtmsw_feedback,(ctl, 1, s));
    dtstr_printf(ctl, s, sizeof(s),	
	  "%d", wmfhandle);
    DT_Q(dtmsw_feedback,(ctl, 1, s));
    dtstr_printf(ctl, s, sizeof(s),	
	  "%s", id);
    DT_Q(dtmsw_feedback,(ctl, 1, s));
    dtstr_printf(ctl, s, sizeof(s),	
	  "%u", image->xe);
    DT_Q(dtmsw_feedback,(ctl, 1, s));
    dtstr_printf(ctl, s, sizeof(s),	
	  "%u", image->ye);
    DT_Q(dtmsw_feedback,(ctl, 1, s));
  }
  return rc;
}

/*..........................................................................
 * write a temporary file with a wmf image in it
 * tell front end to read this file
 * also give the image size to the front end to save it endless trouble
 *..........................................................................*/

static
dt_rc_e
export_file(
  dt_ctl_t *ctl,
  const char *tmpdir,
  const char *id,
  dtimage_t *image,
  unsigned int pagesize,
  unsigned int npages)
{
  DT_F("dtxchg_wxx_export_file");
  char *type;
  char s[80];
  int pid;

  if (strncmp(tmpdir, "wmf,", 4))
  {
    type = "wmf";
    tmpdir += 4;
  }
  else
  if (strncmp(tmpdir, "bmp,", 4))
  {
    type = "bmp";
    tmpdir += 4;
  }
  else
    return dt_err(ctl, F,
      "Sorry, \"%s\" does not indicate"
      " with a known type",
      tmpdir);
  
  dtstr_printf(ctl, s, sizeof(s),		/* 1. the feedback command 			*/
	"export");
  DT_Q(dtmsw_feedback,(ctl, 1, s));

  dtstr_printf(ctl, s, sizeof(s),		/* 2. name of image window we want 	*/
	"%s", id);
  DT_Q(dtmsw_feedback,(ctl, 1, s));

  dtstr_printf(ctl, s, sizeof(s),		/* 3. image width					*/
	"%d", image->xe);
  DT_Q(dtmsw_feedback,(ctl, 1, s));

  dtstr_printf(ctl, s, sizeof(s),		/* 4. image height					*/
	"%d", image->ye);
  DT_Q(dtmsw_feedback,(ctl, 1, s));

  dtstr_printf(ctl, s, sizeof(s),		/* 5. type of file we want			*/
	"%s", type);
  DT_Q(dtmsw_feedback,(ctl, 1, s));

  DT_Q(dtos_getpid,(ctl, &pid));
  dtstr_printf(ctl, s, sizeof(s),		/* 6. name of tmp file				*/
	"%s/%08x.%s", tmpdir, pid, type);
  DT_Q(dtmsw_feedback,(ctl, 1, s));

  if (!strcmp(type, "wmf"))
    DT_Q(dtimage_write_wmf_file,(ctl,	/* write the temporary wmf file		*/
      image, s, 0, 0, 0))
  else
  if (!strcmp(type, "bmp"))
    DT_Q(dtimage_write_bmp_file,(ctl,	/* write the temporary bmp file		*/
      image, s, 0, 0, 0))

  DT_Q(dtmsw_kick,(ctl,					/* kick front end 					*/
    DTMSW_KICK_FEEDBACK));

  DT_Q(dtmsw_pause,(ctl));				/* wait for confirmation 			*/

  DT_Q(dtos_unlink,(ctl, s));			/* remove the file 					*/

  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtxchg_wxx_export_image(
  dt_ctl_t *ctl,
  const char *server,
  const char *id,
  dtimage_t *image,
  int xpos,
  int ypos)
{
  DT_F("dtxchg_wxx_export_image");

  if (!strncmp(server, "file,", 5))
    DT_Q(export_file,(ctl,
      server+5, id, image,
      xpos, ypos))
  else
    return dt_err(ctl, F,
      "Sorry, \"%s\" is not a known"
      " method",
      server);

  return DT_RC_GOOD;
}

/*..........................................................................
 * called by dtdib_import to get data
 * rgb is right after the dib and data is right after the rgb
 *..........................................................................*/

static
DTCONFIG_API1
dt_rc_e
readfunc(
  dt_ctl_t *ctl,
  void *arg,
  void *data,
  unsigned int size)
{
  DT_F("dtwmf_readfunc");
  arg_t *a = (arg_t *)arg;

  if (a->current + size > a->maxsize)	/* stay inside buffer 				*/
    return dt_err(ctl, F,
      "attempt to access"
      " global memory block %d"
      " past its size %ld",
      a->handle, a->maxsize);

  memcpy_h2f(data,						/* copy from global memory 			*/
    a->buffer, a->current, size);
  
  a->current += size;					/* increment position 				*/

  return DT_RC_GOOD;
}

/*..........................................................................
 * id is an image name possibly including spaces
 *..........................................................................*/

static
dt_rc_e
import_dib_image(
  dt_ctl_t *ctl,
  const char *memhandle,
  const char *id,
  dtimage_t *image,
  unsigned int pagesize,
  unsigned int npages)
{
  DT_F("dtxchg_wxx_import_dib_image");
  char *end;
  long longval;
  arg_t a;
  dtdib_t *dib;
  dt_rc_e rc;

  DT_MEMSET(&a, 0, sizeof(a));
  
  longval = strtol(memhandle, &end, 0);	/* try the conversion			    */

  if (end == memhandle ||				/* make sure handle is a number 	*/
      *end != '\0' ||					/* followed by a comma 				*/
	  longval <= 0L ||					/* make sure handle is reasonable 	*/
	  longval > 65535L)
  {
    rc = dt_err(ctl, F,
      "\"%s\" is not a valid"
      " handle",
      memhandle);
	goto cleanup;
  }

  a.handle = (HANDLE)longval;

  a.buffer = GlobalLock(a.handle);		/* get pointer to global memory 	*/
  if (a.buffer == NULL)
  {
    rc = dt_err(ctl, F,
      "GlobalLock(\"%d\") failed",
      a.handle);
	goto cleanup;
  }

  a.maxsize = GlobalSize(a.handle);

  dt_dbg(ctl, F, DT_DBG_MASK_OBJECT,
    "memory: handle %d, size %ld",
    a.handle, a.maxsize);

  dib = (dtdib_t *)a.buffer;			/* dib header at start of memory 	*/
  a.current = sizeof(*dib);				/* pallette is next 				*/
  
  DT_C(dtdib_import,(ctl, dib,		 	/* import dib from memory 			*/
    readfunc, (void *)&a,
	0,									/* load from row 0 					*/
    0,									/* don't reverse rows 				*/
    0, 0,								/* allocate image for dib 			*/
    image, pagesize, npages));

cleanup:
  if (a.buffer != NULL)
    GlobalUnlock(a.handle);				/* release global memory 			*/

  return rc;
}

/*..........................................................................
 * mfspec is of the form handle+xe+ye
 * the caller must give x and y to save us from having to scan the entire
 *   metafile to find its extent
 * the mf at handle should not have the Adobe placeable header in it
 * id is an image name possibly including spaces
 *..........................................................................*/

static
dt_rc_e
import_wmf_handle(
  dt_ctl_t *ctl,
  const char *mfspec,
  const char *id,
  dtimage_t *image,
  unsigned int pagesize,
  unsigned int npages)
{
  DT_F("dtxchg_wxx_import_wmf_handle");
  HANDLE hmf;
  dtxy_t xe, ye;
  arg_t a;
  dt_rc_e rc;

  DT_MEMSET(&a, 0, sizeof(a));

  DT_C(dtxchg_wxx_check,(ctl,			/* see if we are given a handle 	*/
    mfspec, &hmf, &xe, &ye));
  if (hmf == (HANDLE)NULL ||
	  xe == 0 ||
	  ye == 0)
  {
    rc = dt_err(ctl, F,
      "\"%s\" is invalid for importing",
      mfspec);
	goto cleanup;
  }
										/* ................................ */

  a.handle = GetMetaFileBits(hmf);		/* get global memory handle 		*/
  if (a.handle == (HANDLE)NULL)
  {
    rc = dt_err(ctl, F,
      "GetMetaFileBits(\"%d\") failed",
      hmf);
	goto cleanup;
  }

  a.buffer = GlobalLock(a.handle);		/* get pointer to global memory 	*/
  if (a.buffer == NULL)
  {
    rc = dt_err(ctl, F,
      "GlobalLock(\"%d\") failed",
      a.handle);
	goto cleanup;
  }

  a.maxsize = GlobalSize(a.handle);

  dt_dbg(ctl, F, DT_DBG_MASK_OBJECT,
    "memory: hmf %d, handle %d, size %ld",
    hmf, a.handle, a.maxsize);

  a.current = 0;
  DT_C(dtwmf_import,(ctl,				/* import image from memory 		*/
    readfunc, (void *)&a,
    xe, ye,								/* allocate image with this size 	*/
    image, pagesize, npages));

cleanup:
  if (a.buffer != NULL)
  {
    dt_dbg(ctl, F, DT_DBG_MASK_OBJECT,
      "a.buffer 0x%08lx,"
      " a.current %ld,"
      " a.maxsize %ld",
      a.buffer, a.current, a.maxsize);

    GlobalUnlock(a.handle);				/* release global memory 			*/

    dt_dbg(ctl, F, DT_DBG_MASK_OBJECT,
      "returning hmf %d,", hmf);
  }

  if (a.handle != (HANDLE)NULL)
  {
    HANDLE new_hmf = 
      SetMetaFileBits(a.handle);		/* restore metafile 				*/
	
	if (new_hmf != hmf)					/* system gives different handle? 	*/
	{
      char s[80];
      dtstr_printf(ctl, s, sizeof(s),	
	    "DT IMAGE OUTPUT");
      DT_Q(dtmsw_feedback,(ctl, 1, s));
      dtstr_printf(ctl, s, sizeof(s),	
	    "%d", new_hmf);
      DT_Q(dtmsw_feedback,(ctl, 1, s));
      dtstr_printf(ctl, s, sizeof(s),	
	    "%s", id);
      DT_Q(dtmsw_feedback,(ctl, 1, s));
      dtstr_printf(ctl, s, sizeof(s),	
	    "%u", image->xe);
      DT_Q(dtmsw_feedback,(ctl, 1, s));
      dtstr_printf(ctl, s, sizeof(s),	
	    "%u", image->ye);
      DT_Q(dtmsw_feedback,(ctl, 1, s));
	}
  }

  return rc;
}

/*..........................................................................
 * the file has the Adobe placeable header in it
 * we deduce the image size from the extents specified in this header
 * id is an image window name possibly including spaces
 *..........................................................................*/

static
dt_rc_e
import_file(
  dt_ctl_t *ctl,
  const char *tmpdir,
  const char *id,
  dtimage_t *image,
  unsigned int pagesize,
  unsigned int npages)
{
  DT_F("dtxchg_wxx_import_file");
  char *type;
  char s[80];
  int pid;

  if (strncmp(tmpdir, "wmf,", 4))
  {
    type = "wmf";
    tmpdir += 4;
  }
  else
  if (strncmp(tmpdir, "bmp,", 4))
  {
    type = "bmp";
    tmpdir += 4;
  }
  else
    return dt_err(ctl, F,
      "Sorry, \"%s\" does not indicate"
      " with a known type",
      tmpdir);
  
  dtstr_printf(ctl, s, sizeof(s),		/* 1. the feedback command 			*/
	"import");
  DT_Q(dtmsw_feedback,(ctl, 1, s));

  dtstr_printf(ctl, s, sizeof(s),		/* 2. name of image window we want 	*/
	"%s", id);
  DT_Q(dtmsw_feedback,(ctl, 1, s));

  dtstr_printf(ctl, s, sizeof(s),		/* 3. type of file we want			*/
	"%s", type);
  DT_Q(dtmsw_feedback,(ctl, 1, s));

  DT_Q(dtos_getpid,(ctl, &pid));
  dtstr_printf(ctl, s, sizeof(s),		/* 4. name of tmp file for image	*/
	"%s/%08x.%s", tmpdir, pid, type);
  DT_Q(dtmsw_feedback,(ctl, 1, s));

  DT_Q(dtmsw_kick,(ctl,					/* kick front end 					*/
    DTMSW_KICK_FEEDBACK));

  DT_Q(dtmsw_pause,(ctl));				/* wait for confirmation 			*/

  if (!strcmp(type, "wmf"))
    DT_Q(dtimage_read_wmf_file,(ctl,	/* read the file as wmf				*/
      image, s))
  else
  if (!strcmp(type, "bmp"))
    DT_Q(dtimage_read_bmp_file,(ctl,	/* read the file as bmp				*/
      image, s))

  DT_Q(dtos_unlink,(ctl, s));			/* remove the file 					*/

  return DT_RC_GOOD;
}

/*..........................................................................
 * server is either "wmf,handle" or "dib,handle"
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtxchg_wxx_import_image(
  dt_ctl_t *ctl,
  const char *server,
  const char *id,
  dtimage_t *image,
  unsigned int pagesize,
  unsigned int npages)
{
  DT_F("dtxchg_wxx_import_image");

  if (!strncmp(server, "file,", 5))
    DT_Q(import_file,(ctl,
      server+5, id, image,
      pagesize, npages))
  else
    return dt_err(ctl, F,
      "Sorry, \"%s\" is not a known"
      " method",
      server);

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
