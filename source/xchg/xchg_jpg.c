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
  20000505 DE added luminance flag to decompression
  19981005 DE malloc_max redefined
  19980716 DE remove sunpro1 compiler warnings
 *..........................................................................*/



#include <dtack/base.h>
DT_RCSID("source/xchg $RCSfile: xchg_jpg.c,v $ $Revision: 1.1 $");
#include <dtack/lut8.h>
#include <dtack/xchg.h>
#include <dtack/os.h>
#include <dtack/str.h>

#if DTPRJ_IJG != DT_1
/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtxchg_jpeg_import_file(				/* get image from jpeg file			*/
  dt_ctl_t *ctl,
  char *filename,
  dtimage_t *image)
{
  DT_F("dtxchg_jpeg_import_file");
  return dt_err_notlinked(ctl, F,
    "jpeg");
}

/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtxchg_jpeg_import_memory(				/* get image from jpeg memory		*/
  dt_ctl_t *ctl,
  void *memory,
  long memory_max,
  long *memory_used,
  dtimage_t *image)
{
  DT_F("dtxchg_jpeg_import_memory");
  return dt_err_notlinked(ctl, F,
    "jpeg");
}

/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtxchg_jpeg_export_file(				/* get image from jpeg file			*/
  dt_ctl_t *ctl,
  const char *quality_string,
  const char *filename,
  dtimage_t *image)
{
  DT_F("dtxchg_jpeg_export_file");
  return dt_err_notlinked(ctl, F,
    "jpeg");
}

/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtxchg_jpeg_export_memory(				/* get image from jpeg memory		*/
  dt_ctl_t *ctl,
  const char *quality_string,
  void *memory,
  long memory_max,
  long *memory_used,
  dtimage_t *image)
{
  DT_F("dtxchg_jpeg_export_memory");
  return dt_err_notlinked(ctl, F,
    "jpeg");
}

#else

#ifndef DTDEPEND
#ifdef DTCONFIG_MALLOC_MAX
#undef DTCONFIG_MALLOC_MAX
#endif
#include <jinclude.h>
#include <jpeglib.h>
#include <jerror.h>
#include <jversion.h>
#include <setjmp.h>
#endif
#include <dtack/color.h>

typedef struct {
  struct jpeg_error_mgr pub;
  dt_ctl_t *ctl;
  jmp_buf setjmp_buffer;				/* for return to caller 			*/
} err_t;

typedef struct {
  struct jpeg_source_mgr pub;			/* public fields */
  dt_ctl_t *ctl;
  dtfd_t fd;
  JOCTET *buffer;						/* file buffer 						*/
  void *memory;		
  long memory_max;
  long memory_used;
  int error_flag;						/* error flag 						*/
  boolean start_of_file;				/* have we gotten any data yet? 	*/
} src_t;

typedef struct {
  struct jpeg_destination_mgr pub;		/* public fields */
  dt_ctl_t *ctl;
  dtfd_t fd;
  JOCTET *buffer;						/* file buffer 						*/
  void *memory;		
  long memory_max;
  long memory_used;
  int error_flag;						/* error flag 						*/
  char overflow[256];
} dst_t;

#define INPUT_BUF_SIZE  4096	/* choose an efficiently fread'able size */

#define OUTPUT_BUF_SIZE  4096	/* choose an efficiently fwrite'able size */


/*
 * jerror.c
 *
 * Copyright (C) 1991-1996, Thomas G. Lane.
 * This file is part of the Independent JPEG Group's software.
 * For conditions of distribution and use, see the accompanying README file.
 *
 * This file contains simple error-reporting and trace-message routines.
 * These are suitable for Unix-like systems and others where writing to
 * stderr is the right thing to do.  Many applications will want to replace
 * some or all of these routines.
 *
 * These routines are used by both the compression and decompression code.
 */

/* this is not a core library module, so it doesn't define JPEG_INTERNALS */

#ifndef EXIT_FAILURE		/* define exit() codes if not provided */
#define EXIT_FAILURE  1
#endif


/*..........................................................................
 * Create the message string table.
 * We do this from the master message list in jerror.h by re-reading
 * jerror.h with a suitable definition for macro JMESSAGE.
 * The message table is made an external symbol just in case any applications
 * want to refer to it directly.
 *..........................................................................*/

#ifdef NEED_SHORT_EXTERNAL_NAMES
#define jpeg_std_message_table	jMsgTable
#endif

#define JMESSAGE(code,string)	string ,

const char * const jpeg_std_message_table[] = {
#include "jerror.h"
  NULL
};

/*..........................................................................
 * Error exit handler: must not return to caller.
 *
 * Applications may override this if they want to get control back after
 * an error.  Typically one would longjmp somewhere instead of exiting.
 * The setjmp buffer can be made a private field within an expanded error
 * handler object.  Note that the info needed to generate an error message
 * is stored in the error object, so you can generate the message now or
 * later, at your convenience.
 * You should make sure that the JPEG object is cleaned up (with jpeg_abort
 * or jpeg_destroy) at some point.
 *..........................................................................*/


static void DT_JPEG_METH
error_exit (j_common_ptr cinfo)
{
  DT_F("dtimage_xchg_jpeg_error");
  err_t *err = (err_t *)cinfo->err;
  dt_ctl_t *ctl = err->ctl;
  char buffer[JMSG_LENGTH_MAX];

  (*cinfo->err->format_message) (cinfo, buffer);

  dt_err(ctl, F, "%s", buffer);

  longjmp(err->setjmp_buffer, 1);
}

/*..........................................................................
 * Actual output of an error or trace message.
 * Applications may override this method to send JPEG messages somewhere
 * other than stderr.
 *..........................................................................*/


static void DT_JPEG_METH
output_message (j_common_ptr cinfo)
{
  DT_F("dtimage_xchg_jpeg_error");
  err_t *err = (err_t *)cinfo->err;
  dt_ctl_t *ctl = err->ctl;
  char buffer[JMSG_LENGTH_MAX];

  /* Create the message */
  (*cinfo->err->format_message) (cinfo, buffer);

  dt_dbg(ctl, F, DT_DBG_MASK_ANY,
    "%s", buffer);
}

/*..........................................................................
 * Decide whether to emit a trace or warning message.
 * msg_level is one of:
 *   -1: recoverable corrupt-data warning, may want to abort.
 *    0: important advisory messages (always display to user).
 *    1: first level of tracing detail.
 *    2,3,...: successively more detailed tracing messages.
 * An application might override this method if it wanted to abort on warnings
 * or change the policy about which messages to display.
 *..........................................................................*/


static void DT_JPEG_METH
emit_message (j_common_ptr cinfo, int msg_level)
{
  struct jpeg_error_mgr * err = cinfo->err;

  if (msg_level < 0) {
    /* It's a warning message.  Since corrupt files may generate many warnings,
     * the policy implemented here is to show only the first warning,
     * unless trace_level >= 3.
     */
    if (err->num_warnings == 0 || err->trace_level >= 3)
      (*err->output_message) (cinfo);
    /* Always count warnings in num_warnings. */
    err->num_warnings++;
  } else {
    /* It's a trace message.  Show it if trace_level >= msg_level. */
    if (err->trace_level >= msg_level)
      (*err->output_message) (cinfo);
  }
}

/*..........................................................................
 * Format a message string for the most recent JPEG error or message.
 * The message is stored into buffer, which should be at least JMSG_LENGTH_MAX
 * characters.  Note that no '\n' character is added to the string.
 * Few applications should need to override this method.
 *..........................................................................*/



static void DT_JPEG_METH
format_message (j_common_ptr cinfo, char * buffer)
{
  struct jpeg_error_mgr * err = cinfo->err;
  int msg_code = err->msg_code;
  const char * msgtext = NULL;
  const char * msgptr;
  char ch;
  boolean isstring;

  /* Look up message string in proper table */
  if (msg_code > 0 && msg_code <= err->last_jpeg_message) {
    msgtext = err->jpeg_message_table[msg_code];
  } else if (err->addon_message_table != NULL &&
	     msg_code >= err->first_addon_message &&
	     msg_code <= err->last_addon_message) {
    msgtext = err->addon_message_table[msg_code - err->first_addon_message];
  }

  /* Defend against bogus message number */
  if (msgtext == NULL) {
    err->msg_parm.i[0] = msg_code;
    msgtext = err->jpeg_message_table[0];
  }

  /* Check for string parameter, as indicated by %s in the message text */
  isstring = FALSE;
  msgptr = msgtext;
  while ((ch = *msgptr++) != '\0') {
    if (ch == '%') {
      if (*msgptr == 's') isstring = TRUE;
      break;
    }
  }

  /* Format the message into the passed buffer */
  if (isstring)
    sprintf(buffer, msgtext, err->msg_parm.s);
  else
    sprintf(buffer, msgtext,
	    err->msg_parm.i[0], err->msg_parm.i[1],
	    err->msg_parm.i[2], err->msg_parm.i[3],
	    err->msg_parm.i[4], err->msg_parm.i[5],
	    err->msg_parm.i[6], err->msg_parm.i[7]);
}


/*..........................................................................
 * Reset error state variables at start of a new image.
 * This is called during compression startup to reset trace/error
 * processing to default state, without losing any application-specific
 * method pointers.  An application might possibly want to override
 * this method if it has additional error processing state.
 *..........................................................................*/



static void DT_JPEG_METH
reset_error_mgr (j_common_ptr cinfo)
{
  cinfo->err->num_warnings = 0;
  /* trace_level is not reset since it is an application-supplied parameter */
  cinfo->err->msg_code = 0;	/* may be useful as a flag for "no error" */
}


/*..........................................................................
 * Fill in the standard error-handling methods in a jpeg_error_mgr object.
 * Typical call is:
 *	struct jpeg_compress_struct cinfo;
 *	struct jpeg_error_mgr err;
 *
 *	cinfo.err = jpeg_std_error(&err);
 * after which the application may override some of the methods.
 *..........................................................................*/


static void DT_JPEG_METH
init_err(struct jpeg_error_mgr * err)
{
  err->error_exit = error_exit;
  err->emit_message = emit_message;
  err->output_message = output_message;
  err->format_message = format_message;
  err->reset_error_mgr = reset_error_mgr;

  err->trace_level = 0;		/* default = no tracing */
  err->num_warnings = 0;	/* no warnings emitted yet */
  err->msg_code = 0;		/* may be useful as a flag for "no error" */

  /* Initialize message table pointers */
  err->jpeg_message_table = jpeg_std_message_table;
  err->last_jpeg_message = (int) JMSG_LASTMSGCODE - 1;

  err->addon_message_table = NULL;
  err->first_addon_message = 0;	/* for safety */
  err->last_addon_message = 0;
}

/*..........................................................................
 * Initialize source --- called by jpeg_read_header
 * before any data is actually read.
 *..........................................................................*/


static void DT_JPEG_METH
init_source (j_decompress_ptr cinfo)
{
  src_t *src = (src_t *)cinfo->src;

  /* We reset the empty-input-file flag for each image,
   * but we don't clear the input buffer.
   * This is correct behavior for reading a series of images from one source.
   */
  src->start_of_file = TRUE;
}

/*..........................................................................
 * Fill the input buffer --- called whenever buffer is emptied.
 *
 * In typical applications, this should read fresh data into the buffer
 * (ignoring the current state of next_input_byte & bytes_in_buffer),
 * reset the pointer & count to the start of the buffer, and return TRUE
 * indicating that the buffer has been reloaded.  It is not necessary to
 * fill the buffer entirely, only to obtain at least one more byte.
 *
 * There is no such thing as an EOF return.  If the end of the file has been
 * reached, the routine has a choice of ERREXIT() or inserting fake data into
 * the buffer.  In most cases, generating a warning message and inserting a
 * fake EOI marker is the best course of action --- this will allow the
 * decompressor to output however much of the image is there.  However,
 * the resulting error message is misleading if the real problem is an empty
 * input file, so we handle that case specially.
 *
 * In applications that need to be able to suspend compression due to input
 * not being available yet, a FALSE return indicates that no more data can be
 * obtained right now, but more may be forthcoming later.  In this situation,
 * the decompressor will return to its caller (with an indication of the
 * number of scanlines it has read, if any).  The application should resume
 * decompression after it has loaded more data into the input buffer.  Note
 * that there are substantial restrictions on the use of suspension --- see
 * the documentation.
 *
 * When suspending, the decompressor will back up to a convenient restart point
 * (typically the start of the current MCU). next_input_byte & bytes_in_buffer
 * indicate where the restart point will be if the current call returns FALSE.
 * Data beyond this point must be rescanned after resumption, so move it to
 * the front of the buffer rather than discarding it.
 *..........................................................................*/

static boolean DT_JPEG_METH
fill_input_buffer (j_decompress_ptr cinfo)
{
  DT_F("dtxchg_jpeg_fill_input_buffer");
  src_t *src = (src_t *)cinfo->src;
  dt_ctl_t *ctl = src->ctl;
  long nbytes;
  dt_rc_e rc;

  if (src->memory == NULL)				/* input is coming from fd? 		*/
  {
	nbytes = 0;
    DT_G(dtfd_read,(ctl, &src->fd,
      src->buffer, INPUT_BUF_SIZE,
       &nbytes));
	if (rc == DT_RC_GOOD &&				/* no file error? 					*/
		nbytes == 0 &&					/* however, end of file? 			*/
		src->error_flag == 0)			/* haven't given error before?		*/
      dt_err(ctl, F,
        "unexpected end of file"
        " in \"%s\"",
        src->fd.filename);

	if (nbytes == 0)					/* got no data? 					*/
	{
	  src->error_flag = 1;
	  src->buffer[0] = (JOCTET) 0xFF;
	  src->buffer[1] = (JOCTET) JPEG_EOI;
	  nbytes = 2;
	}
	else
	{
	  src->pub.next_input_byte = src->buffer;
	  src->pub.bytes_in_buffer = nbytes;
	  src->start_of_file = FALSE;
	}
  }
  else									/* input is coming from memory? 	*/
  {
	if (src->start_of_file == FALSE)	/* already used up the buffer? 		*/
	{
	  dt_err(ctl, F,
        "unexpected end of buffer"
        " after %ld bytes",
        src->memory_max);
	  src->error_flag = 1;
	  src->buffer[0] = (JOCTET) 0xFF;
	  src->buffer[1] = (JOCTET) JPEG_EOI;
	  nbytes = 2;
	}
	else
	{
	  src->pub.next_input_byte = 
        (const unsigned char *)src->memory;
	  src->pub.bytes_in_buffer =		/* give it all 						*/
        src->memory_max;
	  src->start_of_file = FALSE;
	}
  }

  return TRUE;
}

/*..........................................................................
 * Skip data --- used to skip over a potentially large amount of
 * uninteresting data (such as an APPn marker).
 *
 * Writers of suspendable-input applications must note that skip_input_data
 * is not granted the right to give a suspension return.  If the skip extends
 * beyond the data currently in the buffer, the buffer can be marked empty so
 * that the next read will cause a fill_input_buffer call that can suspend.
 * Arranging for additional bytes to be discarded before reloading the input
 * buffer is the application writer's problem.
 *..........................................................................*/


static void DT_JPEG_METH
skip_input_data (j_decompress_ptr cinfo, long num_bytes)
{
  src_t *src = (src_t *)cinfo->src;

  /* Just a dumb implementation for now.  Could use fseek() except
   * it doesn't work on pipes.  Not clear that being smart is worth
   * any trouble anyway --- large skips are infrequent.
   */
  if (num_bytes > 0) {
    while (num_bytes > (long) src->pub.bytes_in_buffer) {
      num_bytes -= (long) src->pub.bytes_in_buffer;
      (void) fill_input_buffer(cinfo);
      /* note we assume that fill_input_buffer will never return FALSE,
       * so suspension need not be handled.
       */
    }
    src->pub.next_input_byte += (size_t) num_bytes;
    src->pub.bytes_in_buffer -= (size_t) num_bytes;
  }
}

/*..........................................................................
 * An additional method that can be provided by data source modules is the
 * resync_to_restart method for error recovery in the presence of RST markers.
 * For the moment, this source module just uses the default resync method
 * provided by the JPEG library.  That method assumes that no backtracking
 * is possible.
 *..........................................................................*/


/*..........................................................................
 * Terminate source --- called by jpeg_finish_decompress
 * after all data has been read.  Often a no-op.
 *
 * NB: *not* called by jpeg_abort or jpeg_destroy; surrounding
 * application must deal with any cleanup that should happen even
 * for error exit.
 *..........................................................................*/


static void DT_JPEG_METH
term_source (j_decompress_ptr cinfo)
{
  /* no work necessary here */
}

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

dt_rc_e
DTCONFIG_API1
dtxchg_jpeg_import2(					/* get image from jpeg				*/
  dt_ctl_t *ctl,
  char *filename,
  unsigned long flags,
  void *memory,
  long memory_max,
  long *memory_used,
  dtimage_t *image)
{
  DT_F("dtxchg_jpeg_import");
  struct jpeg_decompress_struct cinfo;
  err_t err;
  src_t src;
  dtxy_t y, n;
  dt_rc_e rc = DT_RC_GOOD;

  DT_MEMSET(&cinfo, 0, sizeof(cinfo));
  DT_MEMSET(&err, 0, sizeof(err));
  DT_MEMSET(&src, 0, sizeof(src));
  src.fd.fd = -1;

  init_err(&err.pub);					/* init error handling methods		*/
  err.ctl = ctl;
  cinfo.err = (struct jpeg_error_mgr *)&err;

  jpeg_create_decompress(&cinfo);		/* init jpeg decompression object 	*/

  DT_MEMSET(&src, 0, sizeof(src));
  
  if (memory == NULL)					/* input comes from file? 			*/
  {
    DT_C(dtos_malloc2,(ctl,				/* get input buffer 				*/
      (void **)&src.buffer,
      INPUT_BUF_SIZE,
      F, "buffer"));
    DT_C(dtfd_open,(ctl, &src.fd,		/* open input file 					*/
      filename, "r"));
  }
  else									/* input comes from memory?			*/
  {
	src.memory = memory;
	src.memory_max = memory_max;
	src.memory_used = 0;
  }

  src.ctl = ctl;
  src.pub.init_source = init_source;
  src.pub.fill_input_buffer = fill_input_buffer;
  src.pub.skip_input_data = skip_input_data;
  src.pub.resync_to_restart = jpeg_resync_to_restart;
  src.pub.term_source = term_source;
  src.pub.bytes_in_buffer = 0;
  src.pub.next_input_byte = NULL;

  cinfo.src = (struct jpeg_source_mgr *)&src;

  jpeg_read_header(&cinfo, TRUE);

  if (flags & DTXCHG_FLAG_LUMINANCE)	/* caller wants luminance only? 	*/
    cinfo.out_color_space =				/* colorspace of produced image 	*/
      JCS_GRAYSCALE;

  jpeg_start_decompress(&cinfo);

  if (image->xe == 0)					/* image not allocated yet?			*/
  {
    DT_C(dtimage_create,(ctl, image,
      cinfo.output_width *
      cinfo.output_components,
      cinfo.output_height,
      DT_NTYPE_B08));
  }
  else									/* image is already allocated? 		*/
  if ((unsigned)image->xe !=			/* allocated image is right size? 	*/
      cinfo.output_width *
      cinfo.output_components ||
	  (unsigned)image->ye !=
      cinfo.output_height)
  {
	rc = dt_err(ctl, F,
      "given image size %ux%u"
      " does not agree with"
      " jpeg output size %ux%ux%u",
      image->xe,
	  image->ye,
      cinfo.output_width,
      cinfo.output_components,
      cinfo.output_height);
	goto cleanup;
  }

  for (y=0; y<image->ye; y+=n)
  {
    n = jpeg_read_scanlines(&cinfo,		/* read entire image 				*/
      (JSAMPARRAY)&image->row[y].dvoid,
      image->ye - y);
  }

  jpeg_finish_decompress(&cinfo);
  jpeg_destroy_decompress(&cinfo);

  if (memory_used != NULL)				/* caller wants this? 				*/
    *memory_used = src.memory_used;

cleanup:
  if (rc != DT_RC_GOOD &&				/* routine failed? 					*/
	  image->xe != 0)					/* but image got allocated? 		*/
    DT_I(dtimage_free,(ctl, image));
  if (src.fd.fd != -1)					/* file got opened? 				*/
    DT_I(dtfd_close,(ctl, &src.fd));
  if (src.buffer != NULL)				/* buffer got allocated? 			*/
    DT_I(dtos_free2,(ctl, src.buffer,
      F, "buffer"));
  return rc;
}

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

dt_rc_e
DTCONFIG_API1
dtxchg_jpeg_import(						/* get image from jpeg				*/
  dt_ctl_t *ctl,
  char *filename,
  void *memory,
  long memory_max,
  long *memory_used,
  dtimage_t *image)
{
  return dtxchg_jpeg_import2(ctl,
    filename, 0L,
    memory, memory_max, memory_used,
    image);
}

/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtxchg_jpeg_import_file(				/* get image from jpeg file			*/
  dt_ctl_t *ctl,
  char *filename,
  dtimage_t *image)
{
  DT_F("dtxchg_jpeg_import_file");
  DT_Q(dtxchg_jpeg_import,( ctl,
    filename,
    NULL, 0, NULL, 
    image));
  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtxchg_jpeg_import_file2(				/* get image from jpeg file			*/
  dt_ctl_t *ctl,
  char *filename,
  unsigned long flags,
  dtimage_t *image)
{
  DT_F("dtxchg_jpeg_import_file");
  DT_Q(dtxchg_jpeg_import2,( ctl,
    filename, flags,
    NULL, 0, NULL, 
    image));
  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtxchg_jpeg_import_memory(				/* get image from jpeg memory		*/
  dt_ctl_t *ctl,
  void *memory,
  long memory_max,
  long *memory_used,
  dtimage_t *image)
{
  DT_F("dtxchg_jpeg_import_memory");
  DT_Q(dtxchg_jpeg_import,( ctl,
    NULL,
    memory, memory_max, memory_used,
    image));
  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/
/*
 * Initialize destination --- called by jpeg_start_compress
 * before any data is actually written.
 */

static void DT_JPEG_METH
init_destination (j_compress_ptr cinfo)
{
  dst_t *dst = (dst_t *)cinfo->dest;

  if (dst->memory == NULL)				/* output goes to file? 			*/
  {
	dst->pub.next_output_byte = dst->buffer;
	dst->pub.free_in_buffer = OUTPUT_BUF_SIZE;
  }
  else									/* output goes to memory 			*/
  {
	dst->pub.next_output_byte = (unsigned char *)dst->memory;
	dst->pub.free_in_buffer = dst->memory_max;
  }
}



/*..........................................................................
 *..........................................................................*/
/*
 * Empty the output buffer --- called whenever buffer fills up.
 *
 * In typical applications, this should write the entire output buffer
 * (ignoring the current state of next_output_byte & free_in_buffer),
 * reset the pointer & count to the start of the buffer, and return TRUE
 * indicating that the buffer has been dumped.
 *
 * In applications that need to be able to suspend compression due to output
 * overrun, a FALSE return indicates that the buffer cannot be emptied now.
 * In this situation, the compressor will return to its caller (possibly with
 * an indication that it has not accepted all the supplied scanlines).  The
 * application should resume compression after it has made more room in the
 * output buffer.  Note that there are substantial restrictions on the use of
 * suspension --- see the documentation.
 *
 * When suspending, the compressor will back up to a convenient restart point
 * (typically the start of the current MCU). next_output_byte & free_in_buffer
 * indicate where the restart point will be if the current call returns FALSE.
 * Data beyond this point will be regenerated after resumption, so do not
 * write it out when emptying the buffer externally.
 */

static boolean DT_JPEG_METH
empty_output_buffer (j_compress_ptr cinfo)
{
  DT_F("dtxchg_jpeg_empty_output_buffer");
  dst_t *dst = (dst_t *)cinfo->dest;
  dt_ctl_t *ctl = dst->ctl;
  dt_rc_e rc;

  if (dst->memory == NULL)				/* output is going to fd? 			*/
  {
    DT_G(dtfd_write_all,(ctl, &dst->fd,
      dst->buffer, OUTPUT_BUF_SIZE));
	dst->pub.next_output_byte = dst->buffer;
	dst->pub.free_in_buffer = OUTPUT_BUF_SIZE;
  }
  else									/* input is coming from memory? 	*/
  {
	if (dst->error_flag == 0)
	  dt_err(ctl, F,
        "attempt to write past end of buffer"
        " after %ld bytes",
        dst->memory_max);
	dst->error_flag = 1;
	dst->memory_used = dst->memory_max;
	dst->pub.next_output_byte = (unsigned char *)dst->overflow;
	dst->pub.free_in_buffer = sizeof(dst->overflow);
  }

  return TRUE;
}



/*..........................................................................
 *..........................................................................*/
/*
 * Terminate destination --- called by jpeg_finish_compress
 * after all data has been written.  Usually needs to flush buffer.
 *
 * NB: *not* called by jpeg_abort or jpeg_destroy; surrounding
 * application must deal with any cleanup that should happen even
 * for error exit.
 */

static void DT_JPEG_METH
term_destination (j_compress_ptr cinfo)
{
  DT_F("dtxchg_jpeg_term_destination");
  dst_t *dst = (dst_t *)cinfo->dest;
  dt_ctl_t *ctl = dst->ctl;
  dt_rc_e rc;

  if (dst->memory == NULL)				/* output is going to fd? 			*/
  {
	size_t n = OUTPUT_BUF_SIZE - 
      dst->pub.free_in_buffer;
	if (n > 0) 
      DT_G(dtfd_write_all,(ctl, &dst->fd,
        dst->buffer, n));
  }
  else									/* input is coming from memory? 	*/
  {
	if (dst->memory_used == 0)			/* we did not overflow? 			*/
	  dst->memory_used =
        dst->memory_max - 
        dst->pub.free_in_buffer;
  }

}

/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtxchg_jpeg_export(						/* get image from jpeg file			*/
  dt_ctl_t *ctl,
  const char *spec,
  const char *filename,
  void *memory,
  long memory_max,
  long *memory_used,
  dtimage_t *image)
{
  DT_F("dtxchg_jpeg_export");
  char spec2[80];
  char *quality_spec;
  char *org_spec;
  long quality;
  dt_color_org_t org;
  struct jpeg_compress_struct cinfo;
  err_t err;
  dst_t dst;
  int y, n;
  dt_rc_e rc = DT_RC_GOOD;

  if (spec == NULL ||
	  spec[0] == '\0')
    spec = "luminance+60";

  dtstr_printf(ctl, spec2, sizeof(spec2),
    "%s", spec);

  quality_spec = strchr(spec2, DT_SEPARATOR_LIST);
  if (quality_spec != NULL)
    *quality_spec++ = '\0';
  if (quality_spec == NULL ||
	  quality_spec[0] == '\0')
    quality_spec = "60";

  org_spec = spec2;
  if (org_spec[0] == '\0')
    org_spec = "luminance";

  DT_Q(dtstr_to_long,(ctl,
    quality_spec, &quality));

  dt_dbg(ctl, F, DT_DBG_MASK_COMPLEX,
    "spec=\"%s\", org_spec=\"%s\", quality \"%s\"=%ld",
    spec,
    org_spec,
    quality_spec,
    quality);

  DT_MEMSET(&cinfo, 0, sizeof(cinfo));
  DT_MEMSET(&err, 0, sizeof(err));
  DT_MEMSET(&dst, 0, sizeof(dst));
  dst.fd.fd = -1;

  init_err(&err.pub);					/* init error handling methods		*/
  err.ctl = ctl;
  cinfo.err = (struct jpeg_error_mgr *)&err;

  jpeg_create_compress(&cinfo);			/* init jpeg compression object 	*/

  if (memory == NULL)					/* output goes to file? 			*/
  {
	DT_C(dtos_malloc2,(ctl,				/* get input buffer 				*/
      (void **)&dst.buffer,
      INPUT_BUF_SIZE,
      F, "jpeg export buffer"));
	
    DT_C(dtfd_open,(ctl, &dst.fd,		/* open output file					*/
      filename, "w"));
  }
  else									/* output goes to memory 			*/
  {
	dst.memory = memory;
	dst.memory_max = memory_max;
  }

  dst.ctl = ctl;
  dst.pub.init_destination = init_destination;
  dst.pub.empty_output_buffer = empty_output_buffer;
  dst.pub.term_destination = term_destination;

  cinfo.dest = 
    (struct jpeg_destination_mgr *)&dst;

  
										/* ................................ */

  {
	char message[DT_ERR_LENGTH];
	DT_Q(dt_color_org_parse,(ctl,		/* color form we are given			*/
      &org, org_spec,
      message, sizeof(message)));
	if (message[0] != '\0')
	{
      rc = dt_err(ctl, F,
        "%s", message);
	  goto cleanup;
	}
  }

  if (org.org == 
	  DT_COLOR_ORG_LUMINANCE)
  {
	cinfo.image_width = image->xe;
	cinfo.image_height = image->ye;
	cinfo.input_components = 1;			/* of color components per pixel 	*/
    cinfo.in_color_space =				/* colorspace of input image */
      JCS_GRAYSCALE;
  }
  else
  if (org.org == 
	  DT_COLOR_ORG_YCBCR)
  {
	if (image->xe % 3 != 0)
	{
	  rc = dt_err(ctl, F,
        "YCBCR component image width %u"
        " not divisible by 3",
        image->xe);
	  goto cleanup;
	}
	cinfo.image_width = image->xe / 3;
	cinfo.image_height = image->ye;
	cinfo.input_components = 3;			/* of color components per pixel 	*/
    cinfo.in_color_space =				/* colorspace of input image */
      JCS_YCbCr;
  }
  else
  if (org.org == 
	  DT_COLOR_ORG_RGB888)
  {
	if (image->xe % 3 != 0)
	{
	  rc = dt_err(ctl, F,
        "RGB component image width %u"
        " not divisible by 3",
        image->xe);
	  goto cleanup;
	}
	cinfo.image_width = image->xe / 3;
	cinfo.image_height = image->ye;
	cinfo.input_components = 3;			/* of color components per pixel 	*/
    cinfo.in_color_space =				/* colorspace of input image */
      JCS_RGB;
  }
  else
  {
	char org_s[32];
    DT_C(dt_color_org_string,(ctl,		/* make org string name 			*/
      &org, org_s, sizeof(org_s)));
	rc = dt_err(ctl, F,
      "cannot make jpeg out of"
      " %s images",
      org_s);
	goto cleanup;
  }
   
  jpeg_set_defaults(&cinfo);
  jpeg_set_quality(&cinfo, quality, 
     TRUE);								/* limit to baseline-JPEG values 	*/

  jpeg_start_compress(&cinfo, TRUE);

  for (y=0; y<image->ye; y+=n)
  {
    n = jpeg_write_scanlines(&cinfo,	/* write as much as possible		*/
      (JSAMPARRAY)&image->row[0].dvoid,
      image->ye - y);
  }

  jpeg_finish_compress(&cinfo);
  jpeg_destroy_compress(&cinfo);

  if (dst.error_flag != 0)				/* errors happened? 				*/
  {
	rc = dt_err(ctl, F,
      "errors during compression");
	goto cleanup;
  }
    
  if (memory_used != NULL)				/* caller wants this? 				*/
    *memory_used = dst.memory_used;

cleanup:
  if (dst.fd.fd != -1)
    DT_I(dtfd_close,(ctl, &dst.fd));
  if (dst.buffer != NULL)
    DT_I(dtos_free2,(ctl, dst.buffer,
      F, "jpeg export buffer"));
  return rc;
}

/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtxchg_jpeg_export_file(				/* get image from jpeg file			*/
  dt_ctl_t *ctl,
  const char *quality_string,
  const char *filename,
  dtimage_t *image)
{
  DT_F("dtxchg_jpeg_export_file");
  DT_Q(dtxchg_jpeg_export,( ctl,
    quality_string, filename,
    NULL, 0, NULL, 
    image));
  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtxchg_jpeg_export_memory(				/* get image from jpeg memory		*/
  dt_ctl_t *ctl,
  const char *quality_string,
  void *memory,
  long memory_max,
  long *memory_used,
  dtimage_t *image)
{
  DT_F("dtxchg_jpeg_export_memory");
  DT_Q(dtxchg_jpeg_export,( ctl,
    quality_string, NULL,
    memory, memory_max, memory_used,
    image));
  return DT_RC_GOOD;
}
#endif






/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
  ! Copyright (C) 1998 Dtack Inc. All Rights Reserved
  ! To use this file, you must have signed a license agreement with Dtack Inc.
  ! Under no circumstances may you redistribute this file.
  ! This software is provided AS IS with no warranty expressed or implied.
  ! Dtack Inc. accepts no liability for use or misuse of this file.
  ! http://www.dtack.com  dtack@dtack.com  telephone +360.417.6700
  ! Dtack Inc, P.O. Box 3061, Port Angeles, WA USA 98362
  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */
