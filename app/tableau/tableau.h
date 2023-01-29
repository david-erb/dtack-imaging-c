/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
  ! Copyright (C) 1998 Dtack Inc. All Rights Reserved
  ! To use this file, you must have signed a license agreement with Dtack Inc.
  ! Under no circumstances may you redistribute this file.
  ! This software is provided AS IS with no warranty expressed or implied.
  ! Dtack Inc. accepts no liability for use or misuse of this file.
  ! http://www.dtack.com  dtack@dtack.com  telephone +360.417.6700
  ! Dtack Inc, P.O. Box 3061, Port Angeles, WA USA 98362
  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */




#ifdef __cplusplus
extern "C" {
#endif

#include <dtack/base.h>
#include <dtack/ipc.h>
#include <dtack/ftp.h>
#include <dtack/os.h>
#include <dtack/str.h>

#include <dtack/lut8.h>
#include <dtack/list.h>
#include <dtack/svc.h>
#include <dtack/scr.h>

#include <dtack/dg.h>
  
#include <dtack/xchg.h>

#include <dtack/mem.h>
#include <dtack/stack.h>

#include <dtack/color.h>
#include <dtack/fb.h>
  
#include <dtack/rev.h>						/* app and library revisions 		*/

#define TABLEAU_VERSION (201)

#define TABLEAU_RECV_MAX (65536*4)

#define TABLEAU_FILENAME_MAX (128)

#define TABLEAU_SPONGE					/* how much the Internet can sponge */\
  (DT_CAST(long, param->psi))

#ifdef NONO
#define TABLEAU_PSEUDO_CELL_START (10)
#define TABLEAU_PSEUDO_CELL_COUNT (236)
#endif
#define TABLEAU_PSEUDO_CELL_START (32)
#define TABLEAU_PSEUDO_CELL_COUNT (192)

#define TABLEAU_ROLL_KEEP_ENC (DT_CAST(int, \
  param->dz))
#define TABLEAU_ROLL_KEEP_CUR (DT_CAST(int, \
  (param->dz - (int)param->dz) * 1000.0))

typedef dtntype_s4_t tableau_len_t;		/* 32-bit integer in structures!	*/
										/* ................................ */

#define TABLEAU_TITLE "tableau"
#define TABLEAU_TITLE_CURRENT TABLEAU_TITLE ": current"
#define TABLEAU_TITLE_DECODED TABLEAU_TITLE ": decoded"
#define TABLEAU_TITLE_LABELED TABLEAU_TITLE ": labeled"
#define TABLEAU_X0 10
#define TABLEAU_Y0 10
  
#define TABLEAU_DIALOG_MODE_START "start"
#define TABLEAU_DIALOG_MODE_STOP "stop"
  
										/* ................................ */
#define TABLEAU_CAM_MAX (32)			/* max number of cams 				*/

typedef struct {
  unsigned char *bitmap[TABLEAU_CAM_MAX];
  long last_access[TABLEAU_CAM_MAX];
  long next_access[TABLEAU_CAM_MAX];
  time_t midnight;
  time_t autotime;
  time_t when;
  int count;
  int autoday;
  int current_index;
  int initialized;
  char name[TABLEAU_CAM_MAX][64];
  char spigot[TABLEAU_CAM_MAX][64];
} tableau_cam_t;
										/* ................................ */
typedef struct {
  char command[16];
  char name[16];
  tableau_len_t version;
  tableau_len_t frame;
} tableau_command_t;
										/* ................................ */
typedef struct {
  tableau_len_t l;
  tableau_len_t n;
  tableau_len_t rc;
} tableau_ack_t;
										/* ................................ */
#define TABLEAU_HEADER_CAMNAME_MAX (16)
typedef struct {
  tableau_len_t l;						/* length of encoded data 			*/
  tableau_len_t n;						/* frame sequence number 			*/
  tableau_len_t ms;						/* millisecond at acquire 			*/
  tableau_len_t xe;						/* size of decoded image 			*/
  tableau_len_t ye;
  tableau_len_t org;
  char cam[TABLEAU_HEADER_CAMNAME_MAX];
  char codec[16];
} tableau_header_t;

										/* ................................ */
  
#define TABLEAU_HEADER_FIX(DTC, CTL, IPC, HEADER) \
{\
  (HEADER)->l   = DTIPC_FIX(DTC, CTL, IPC, (HEADER)->l); \
  (HEADER)->xe  = DTIPC_FIX(DTC, CTL, IPC, (HEADER)->xe); \
  (HEADER)->ye  = DTIPC_FIX(DTC, CTL, IPC, (HEADER)->ye); \
  (HEADER)->ms  = DTIPC_FIX(DTC, CTL, IPC, (HEADER)->ms); \
  (HEADER)->n   = DTIPC_FIX(DTC, CTL, IPC, (HEADER)->n); \
  (HEADER)->org = DTIPC_FIX(DTC, CTL, IPC, (HEADER)->org); \
}

										/* ................................ */

#define TABLEAU_CODEC_UNCOMPRESSED		"uncompressed"
#define TABLEAU_CODEC_BTPC 				"btpc"
#define TABLEAU_CODEC_JPEG 				"jpeg"
#define TABLEAU_CODEC_DIFFS				"diffs"
#define TABLEAU_CODEC_LOSSLESS			"lossless"
  
#define TABLEAU_CC_N  (3)				/* number of color components		*/
#define TABLEAU_CC_Y  (0)				/* color components 				*/
#define TABLEAU_CC_CB (1)
#define TABLEAU_CC_CR (2)

										/* ................................ */

#define TABLEAU_GET_ARGS \
  dtparam_t *param, \
  struct tableau_get_t *get, \
  tableau_header_t **header, \
  void **data, \
  long *l

#define TABLEAU_GET_FREE_ARGS \
  dtparam_t *param, \
  struct tableau_get_t *get

typedef enum {
  TABLEAU_GET_FTP     = 1000,
  TABLEAU_GET_IPC     = 1001,
  TABLEAU_GET_SCR     = 1002,
  TABLEAU_GET_ROLL    = 1003,
  TABLEAU_GET_FB      = 1004,
  TABLEAU_GET_FILE    = 1005,
  TABLEAU_GET_UNKNOWN = 1999
} tableau_get_e;

typedef struct tableau_get_t {
  tableau_get_e type;
  dtos_time_t start;
  int partner_stopped;
  union {
	struct {
	  dtipc_t *ipc;
	} ipc;
	struct {
	  dtfb_t fb;						/* fb we are grabbing from 			*/
      tableau_cam_t cam;
	  dt_color_org_t org_component;		/* delivery color from command line */
	  dt_color_org_t org;				/* component delivered individually	*/
	  dt_color_convert_t convert;		/* convert from fb to delivery 		*/
	  unsigned char *raw;				/* raw bytes sucked out of fb 		*/
	  unsigned long *accumulator;		/* frame averaging accumulator 		*/
	  unsigned char *component;			/* frame in delivery format 		*/
	  int cc;
      tableau_len_t frame;
	} fb;
	struct {
      tableau_len_t frame;
	} gen;
	struct {
      tableau_len_t frame;
	  const char *suffix;
	  int cc;
      char dir[128];
	} roll;
	struct {
      tableau_len_t frame;
      dtfd_t listfd;
      char listfn[128];
	} ser;
	struct {
      tableau_len_t frame;
      dtfd_t fd;
	  char name[TABLEAU_FILENAME_MAX];
	} file;
  } u;
#if defined(DTCONFIG_IMPLIED_MSW32) && defined(DTCONFIG_MICROSOFT)
  dt_rc_e (*function)(TABLEAU_GET_ARGS);
  dt_rc_e (*free)(TABLEAU_GET_FREE_ARGS);
#else
  dt_rc_e DTCONFIG_API1 (*function)(TABLEAU_GET_ARGS);
  dt_rc_e DTCONFIG_API1 (*free)(TABLEAU_GET_FREE_ARGS);
#endif
  int isserver;
  double shoveled_kb;
  long shoveled_frames;
} tableau_get_t;

typedef
dt_rc_e
DTCONFIG_API1
tableau_get_f(TABLEAU_GET_ARGS);
										/* ................................ */
										/* PUT	 							*/
#define TABLEAU_PUT_ARGS \
  dtparam_t *param, \
  struct tableau_put_t *put, \
  dtimage_t *image,						/* full image 						*/\
  tableau_header_t *header,				/* compressed header 				*/\
  void *data,							/* compressed data 					*/\
  int cc								/* color component 					*/

typedef enum {
  TABLEAU_PUT_FTP     = 2000,
  TABLEAU_PUT_IPC     = 2001,
  TABLEAU_PUT_SCR     = 2002,
  TABLEAU_PUT_ROLL    = 2003,
  TABLEAU_PUT_NULL    = 2004,
  TABLEAU_PUT_FILE    = 2005,
  TABLEAU_PUT_PUSH    = 2006,
  TABLEAU_PUT_UNKNOWN = 2999
} tableau_put_e;

typedef struct tableau_put_t {
  tableau_put_e type;
  dtos_time_t start;
  int partner_stopped;
  union {
	struct {
      dtftp_t ftp;
	  char format[128];					/* filename format 					*/
	  char name[TABLEAU_FILENAME_MAX];
      int output_header;
	} ftp;
	struct {
	  dtipc_t *ipc;
	  long sent;
	  long acked;
	} ipc;
	struct {
	  dtimage_t blue;					/* replicated 						*/
	  dtimage_t red;
	  dtimage_t resampled[3];
	  dt_color_org_t org_y_cb_cr;
	  dt_color_org_t org_r_g_b;
	  dt_color_org_t org_pseudo;
	  dt_color_convert_t convert_y_cb_cr_to_pseudo;
	  dt_color_convert_t convert_r_g_b_to_pseudo;
	  dtlut8_t *lut8;
	  unsigned char pseudo_remap[256];
	  dtimage_t inset;
	  dtscr_t scr;
	  int skiplut;
	  int swap;
      dtxy_t x0;
      dtxy_t y0;
	} scr;
	struct {
      char dir[128];
	  char name[TABLEAU_FILENAME_MAX];
	} roll;
	struct {
      tableau_len_t frame;
      dtfd_t fd;
	  char name[TABLEAU_FILENAME_MAX];
	} file;
	struct {
      dtipc_t *ipc;
	  int flag;
	} push;
  } u;
#if defined(DTCONFIG_IMPLIED_MSW32) && defined(DTCONFIG_MICROSOFT)
  dt_rc_e (*function)(TABLEAU_PUT_ARGS);
#else
  dt_rc_e DTCONFIG_API1 (*function)(TABLEAU_PUT_ARGS);
#endif
  int isserver;
  double shoveled_kb;
  long shoveled_frames;
} tableau_put_t;

typedef
dt_rc_e
DTCONFIG_API1
tableau_put_f(TABLEAU_PUT_ARGS);
										/* ................................ */
										/* COOK 							*/
typedef struct {
  const char *spec;						/* copy of the spec string 			*/
  dtimage_t lopass;
  dtimage_t kernel;
  dtimage_t resampled;
  dtimage_t trimmed;
  int initialized;
} tableau_cook_arg_t;

typedef
dt_rc_e
DTCONFIG_API1
tableau_cook_f(
  dtparam_t *param,
  tableau_cook_arg_t *cook_arg,			/* cooker args 						*/
  dtimage_t *raw,						/* image to cook					*/
  dt_color_org_t *org,					/* org of the raw image 			*/
  dtimage_t *cook);						/* the cooked image					*/

extern tableau_cook_f **tableau_cook_methods;

										/* ................................ */
										/* ENCODE						    */

#define TABLEAU_MAXDIFF (64)
typedef struct {
  dtscr_t scr;
  dtimage_t current;					/* current image					*/
  dtimage_t diffs;						/* diffs image						*/
  dtimage_t shrink;						/* shrunken image 					*/
  dtimage_t dilated;	
  dtimage_t eroded;	
  dtimage_t labeled;	
  dthist_t fhist;
  dthist_t lhist[TABLEAU_MAXDIFF];
  char lowstats[1024];
  int initialized;
  int quality;
  void *b;
  long max;
} tableau_encode_arg_t;

typedef
dt_rc_e
DTCONFIG_API1
tableau_encode_f(
  dtparam_t *param,
  tableau_encode_arg_t *encode_arg,		/* encoder args 					*/
  dtimage_t *image,						/* image to compress				*/
  dt_color_org_t *org,					/* incoming image color org 		*/
  tableau_header_t *header,				/* compression header 				*/
  void **data,							/* the encoded buffer 				*/
  long *l);								/* current length of buffer			*/

extern tableau_encode_f **tableau_encode_methods;

										/* ................................ */
										/* DECODE						    */
typedef struct {
  dtscr_t scr;
  dtimage_t current;
  dtimage_t shrink;
  dtimage_t diffs;
  int initialized;
  void *b;
  long max;
  long l;
} tableau_decode_arg_t;

typedef
dt_rc_e
DTCONFIG_API1
tableau_decode_f(
  dtparam_t *param,
  tableau_decode_arg_t *decode_arg,		/* decoder args 					*/
  tableau_header_t *header,				/* compression header 				*/
  void *data,							/* buffer to be decoded 			*/
  dtimage_t *image);					/* decoded image 					*/

extern tableau_decode_f **tableau_decode_methods;

										/* ................................ */
typedef struct {
  dtparam_t *param;
  dt_ctl_t *ctl;
  tableau_get_t *get;
  tableau_put_t *put;
  tableau_len_t last_frame;
  tableau_len_t last_size;
  double last_time;
  char last_cam[TABLEAU_HEADER_CAMNAME_MAX];
  char message[256];
  char last_message[256];
} tableau_t;
										/* ................................ */

extern const char **tableau_codec_choices;

extern dt_ntype_e *tableau_ntypes;

#include <tableau.p>

#ifdef __cplusplus
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
