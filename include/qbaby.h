/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
  ! Copyright (C) 1998 Dtack Inc. All Rights Reserved
  ! To use this file, you must have signed a license agreement with Dtack Inc.
  ! Under no circumstances may you redistribute this file.
  ! This software is provided AS IS with no warranty expressed or implied.
  ! Dtack Inc. accepts no liability for use or misuse of this file.
  ! http://www.dtack.com  dtack@dtack.com  telephone +360.417.6700
  ! Dtack Inc, P.O. Box 3061, Port Angeles, WA USA 98362
  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */




#ifdef DTCONFIG_IMPLIED_QNX4

#define QBABY_BUFFER_MAX (16384)		/* size of internal qbaby buffers 	*/
#define QBABY_DEATH_SLEEP (1000)		/* milliseconds to poll for deaths 	*/

typedef enum {							/* request codes 					*/
  QBABY_REQUEST_REGISTER     = 1000,
  QBABY_REQUEST_WAIT         = 1001,
  QBABY_REQUEST_FIND         = 1002,
  QBABY_REQUEST_FILL         = 1003,
  QBABY_REQUEST_DRAIN        = 1004,
  QBABY_REQUEST_DONE         = 1005,
  QBABY_REQUEST_UNFIND       = 1006,
  QBABY_REQUEST_UNREGISTER   = 1007
} qbaby_request_e;
#define QBABY_REQUEST_MIN QBABY_REQUEST_REGISTER
#define QBABY_REQUEST_MAX QBABY_REQUEST_UNREGISTER
#define QBABY_REQUEST_TO_STRING(R) \
  (((R)->request < QBABY_REQUEST_MIN || \
    (R)->request > QBABY_REQUEST_MAX)? \
   "UNKNOWN": \
   qbaby_request_string[(R)->request-QBABY_REQUEST_MIN])
const char *qbaby_request_string[] = {
  "REGISTER",
  "WAIT",
  "FIND",
  "FILL",
  "DRAIN",
  "DONE",
  "UNFIND",
  "UNREGISTER"
};

typedef enum {							/* reply codes 						*/
  QBABY_REPLY_OK             = 2000,
  QBABY_REPLY_BADCMD         = 2001,
  QBABY_REPLY_LISTFULL       = 2002,
  QBABY_REPLY_EXISTS         = 2003,
  QBABY_REPLY_NOTFOUND       = 2004,
  QBABY_REPLY_NOTCONNECTED   = 2005,
  QBABY_REPLY_REJECTED       = 2006,
  QBABY_REPLY_READERROR      = 2007,
  QBABY_REPLY_ATTACHERROR    = 2008,
  QBABY_REPLY_TIMEOUT        = 2009,
  QBABY_REPLY_CLOSING        = 2010,
  QBABY_REPLY_SHUTDOWN       = 2011
} qbaby_reply_e;
#define QBABY_REPLY_MIN QBABY_REPLY_OK
#define QBABY_REPLY_MAX QBABY_REPLY_SHUTDOWN
#define QBABY_REPLY_TO_STRING(R) \
  (((R)->reply < QBABY_REPLY_MIN || \
    (R)->reply > QBABY_REPLY_MAX)? \
   "UNKNOWN": \
   qbaby_reply_string[(R)->reply-QBABY_REPLY_MIN])
const char *qbaby_reply_string[] = {
  "OK",
  "BADCMD",
  "LISTFULL",
  "EXISTS",
  "NOTFOUND",
  "NOTCONNECTED",
  "REJECTED",
  "READERROR",
  "ATTACHERROR",
  "TIMEOUT",
  "CLOSING",
  "SHUTDOWN"
};

typedef struct {						/* request header 					*/
  qbaby_request_e request;
  nid_t nid;
  pid_t pid;
  long serial;
  long timeout;
  int size;
  char name[QBABY_NAME_MAX];
} qbaby_request_t;

typedef struct {						/* reply header 					*/
  qbaby_reply_e reply;
  int size;
} qbaby_reply_t;

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
