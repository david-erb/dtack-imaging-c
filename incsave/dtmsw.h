/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
  ! Copyright (C) 1998 Dtack Inc. All Rights Reserved
  ! To use this file, you must have signed a license agreement with Dtack Inc.
  ! Under no circumstances may you redistribute this file.
  ! This software is provided AS IS with no warranty expressed or implied.
  ! Dtack Inc. accepts no liability for use or misuse of this file.
  ! http://www.dtack.com  dtack@dtack.com  telephone +360.417.6700
  ! Dtack Inc, P.O. Box 3061, Port Angeles, WA USA 98362
  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */




#define DTMSW_KEY_FEEDBACK (112)
#define DTMSW_KEYFLAGS_FEEDBACK (0L)

#define DTMSW_KEY_LAUNCHED (113)
#define DTMSW_KEYFLAGS_LAUNCHED (0L)

#define DTMSW_KEY_STATUS (114)
#define DTMSW_KEYFLAGS_STATUS (0L)

#define DTMSW_KEY_FINISH (115)
#define DTMSW_KEYFLAGS_FINISH (0L)

#define DTMSW_KEY_OUTPUT (116)
#define DTMSW_KEYFLAGS_OUTPUT (0L)

#define DTMSW_MESSAGE_MIN     (0x2000)
#define DTMSW_MESSAGE_PAUSE   (DTMSW_MESSAGE_MIN+0)
#define DTMSW_MESSAGE_RESUME  (DTMSW_MESSAGE_MIN+1)
#define DTMSW_MESSAGE_STOP    (DTMSW_MESSAGE_MIN+2)
#define DTMSW_MESSAGE_DESTROY (DTMSW_MESSAGE_MIN+3)
#define DTMSW_MESSAGE_MAX     (DTMSW_MESSAGE_MIN+3)

typedef enum {
  DTMSW_KICK_FEEDBACK,
  DTMSW_KICK_LAUNCHED,
  DTMSW_KICK_STATUS,
  DTMSW_KICK_FINISH
} dtmsw_kick_e;

#define DTMSW_DBG(N) long N

typedef struct {
      DTMSW_DBG(n_outstring);
      DTMSW_DBG(n_peekmessage);
      DTMSW_DBG(n_getmessage);
      DTMSW_DBG(n_yield);
      DTMSW_DBG(n_kick);
      DTMSW_DBG(n_status);
      DTMSW_DBG(n_feedback);

  void *statuswin;
  void *outwin;
  long outwin_maxbytes;					/* max bytes to write into output 	*/
  long outwin_maxlines;					/* max lines to write into output 	*/
  long outwin_curlines;					/* current line count 				*/
  long outwin_curbytes;					/* current byte count 				*/
  long outwin_kicklines;				/* how many lines till we kick 		*/
  char *outbuf;
  int outbufn;
  int outbufm;
  int hinstance;
  int hprevinstance;
  int pidwin;							/* invisible window we own		    */
  long console;
} dtmsw_t;

#undef DTMSW_DBG

#ifdef DTCONFIG_IMPLIED_MSW
# ifndef DTDEPEND
#   define STRICT
#   define WIN32_LEAN_AND_MEAN
#   include <windows.h> 
# endif
#else
#ifndef WINAPI
#define WINAPI
#endif
#ifndef _export
#define _export
#endif
typedef struct {int dummy;} dtmsw_dummy_t;
#ifndef HWND
#define HWND dtmsw_dummy_t *
#endif
#ifndef UINT
#define UINT dtmsw_dummy_t *
#endif
#ifndef LONG
#define LONG dtmsw_dummy_t *
#endif
#endif

#include <dtack/msw.p>






/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
  ! Copyright (C) 1998 Dtack Inc. All Rights Reserved
  ! To use this file, you must have signed a license agreement with Dtack Inc.
  ! Under no circumstances may you redistribute this file.
  ! This software is provided AS IS with no warranty expressed or implied.
  ! Dtack Inc. accepts no liability for use or misuse of this file.
  ! http://www.dtack.com  dtack@dtack.com  telephone +360.417.6700
  ! Dtack Inc, P.O. Box 3061, Port Angeles, WA USA 98362
  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */
