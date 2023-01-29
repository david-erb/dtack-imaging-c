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
  20000204 DE added signature
  19990721 DE added internal buffering
  19990201 DE ifdef around declaration of dtfd_clear
  19980905 DE add explicit member isopen
 *..........................................................................*/


/*..........................................................................
| include $RCSfile: dtfd.h,v $ $Revision: 1.2 $
 *..........................................................................*/

#define DTFD_ISOPEN(CTL, FD) ((FD)->isopen)
#define DTFD_CLEAR(CTL, FD) (*(FD) = dtfd_clear)

typedef struct {
  char *b;								/* allocated buffer pointer 		*/
  char *p;								/* current position 				*/
  long m;								/* allocated size 					*/
  long n;								/* bytes available after p			*/
} dtfd_buffer_t;

#define DTFD_BUFFER_SIZE (4096)			/* short-read buffer size 			*/

#define DTFD_SIG ((dtntype_b32_t)(0xdeadfeeb))

typedef struct {
  dtntype_b32_t sig;
  void *gzfile;							/* may use zlib for file i/o 		*/
  FILE *file;
  char *filename;
  long pos;
  int fd;
  int isopen;
  dtfd_buffer_t b;
} dtfd_t;

#ifdef __cplusplus
extern "C" {
#endif
extern const dtfd_t dtfd_clear;			/* example of a cleared structure 	*/
#ifdef __cplusplus
}
#endif

typedef enum {
  DTFD_WHENCE_SET = SEEK_SET,
  DTFD_WHENCE_CUR = SEEK_CUR,
  DTFD_WHENCE_END = SEEK_END
} dtfd_whence_e;

typedef enum {
  DTFD_POLL_READ = 1001,
  DTFD_POLL_WRITE = 1002
} dtfd_poll_e;

#include <dtack/fd.p>





/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
  ! Copyright (C) 1998 Dtack Inc. All Rights Reserved
  ! To use this file, you must have signed a license agreement with Dtack Inc.
  ! Under no circumstances may you redistribute this file.
  ! This software is provided AS IS with no warranty expressed or implied.
  ! Dtack Inc. accepts no liability for use or misuse of this file.
  ! http://www.dtack.com  dtack@dtack.com  telephone +360.417.6700
  ! Dtack Inc, P.O. Box 3061, Port Angeles, WA USA 98362
  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */
