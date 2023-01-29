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
  20000630 DE 01-30
  20000128 DE 01-26
  20000110 DE 01-23
  20000106 DE 01-22
  19991217 DE introduction of config2.h
  19990810 DE 01-20
  19990809 DE changed dtxy_t to dtstats_xy_t
  19990718 DE 01-19
  19990705 DE 01-18
  19990207 DE 01-17
  19990124 DE 01-16
  19981016 DE 01-15
  19981015 DE added DT_CLEANUP
  19980716 DE added DT_MEMCPY, DT_MEMSET, DT_FREE
 *..........................................................................*/


/*..........................................................................
| include $RCSfile: dt.h,v $ $Revision: 1.23 $
 *..........................................................................*/

#define DT_H_INCLUDED					/* remember we included it		    */

#include <dtack/config.h>				/* platform-specific defines		*/
#include <dtack/ntype.h>				/* scalar data types 				*/
#include <dtack/stats.h>				/* stats structure 					*/
#include <dtack/1d.h>

#include <dtack/config2.h>				/* platform-specific includes		*/

#define DT_COPYRIGHT ""
#define DT_VERSION "02.00.03"
#define DT_VERSION_MAJOR "02"
#define DT_VERSION_MINOR "00"
#define DT_VERSION_YEAR  "2003"
#define DT_VERSION_MONTH "11"
#define DT_VERSION_DAY   "21"

#define DT_RCSID(RCSID) 
#define DT_RCSID_STRING ""

#ifdef DTCONFIG_WANT_STATIC_F
#	define DT_F(NAME) static char *F = NAME
#else
#	define DT_F(NAME) char *F = NULL
#endif

#ifdef __cplusplus
#  define DT_CLIB(FUNC) ::FUNC
#else
#  define DT_CLIB(FUNC) FUNC
#endif

#define DT_MEMSET(D, C, N) ((void)(DT_CLIB(memset)((void *)(D), C, N)))
#define DT_MEMCPY(D, S, N) ((void)(DT_CLIB(memcpy)((void *)(D), (void *)(S), N)))
#define DT_FREE(P) ((void)(DT_CLIB(free)((DTCONFIG_FREE_ARGTYPE *)(P))))

#define DT_KEYSTRING_MAXLEN (16)
#define DT_WINDOWNAME_MAXLEN (32)

#define DT_CTL_MEMSOURCES (6)

typedef enum {							/* return codes						*/
  DT_RC_BAD = -1,
  DT_RC_GOOD = 0,
  DT_RC_STOP = 1,
  DT_RC_EXIT = 2,
  DT_RC_SHUTDOWN = 0xdead
} dt_rc_e;

										/* function arguments for functions */
										/* which dt_ctl_t has in it 		*/
#define DT_ERR_ARGS \
  struct dt_ctl_t *ctl, \
  const char *caller, \
  const char *format, \
  va_list substitutions
#define DT_SAY_ARGS \
  struct dt_ctl_t *ctl, \
  const char *caller, \
  const char *format, \
  va_list substitutions
#define DT_DBG_ARGS \
  struct dt_ctl_t *ctl, \
  const char *caller, \
  long class_bits, \
  const char *format, \
  va_list substitutions

#define DT_ERR_STRING_ARGS \
  struct dt_ctl_t * ctl, \
  const char *string, \
  int length
#define DT_SAY_STRING_ARGS \
  struct dt_ctl_t * ctl, \
  const char *string, \
  int length
#define DT_DBG_STRING_ARGS \
  struct dt_ctl_t * ctl, \
  const char *string, \
  int length

#define DT_YIELD_ARGS \
  struct dt_ctl_t *ctl, \
  const char *formst, \
  va_list substitutions

										/* ................................ */
typedef struct {
  unsigned long flags;					/* enabling flags				    */
  unsigned long mem;					/* tripwire for dtmem			    */
} dt_tripwire_t;
#define DT_TRIPWIRE_MEM (0x01)			/* enabling flag values			    */
										/* ................................ */

typedef struct dt_ctl_t {				/* program control structure		*/
  dt_rc_e (DTCONFIG_API1DEC *yield)(DT_YIELD_ARGS); 
  dt_rc_e (DTCONFIG_API1DEC *err)(DT_ERR_ARGS);	
  void    (DTCONFIG_API1DEC *say)(DT_SAY_ARGS);		
  void    (DTCONFIG_API1DEC *dbg)(DT_DBG_ARGS);		
  void    (DTCONFIG_API1DEC *err_string)(DT_ERR_STRING_ARGS);
  void    (DTCONFIG_API1DEC *say_string)(DT_SAY_STRING_ARGS);
  void    (DTCONFIG_API1DEC *dbg_string)(DT_DBG_STRING_ARGS);
  void *dbgto;
  void *errto;
  void *sayto;
  const char *progname;					/* current program name				*/
  const char *win;						/* default windowing server name 	*/
  const char *mem;						/* memory source specification 		*/
  void *memsource;						/* big memory source table 			*/
  void *dialog;							/* main dialog structure 			*/
  void *cons;							/* virtual console structures 		*/
  void *heap;							/* heap usage structure 			*/
  void *cstack;							/* stack usage structure 			*/
  void *user;							/* for end user use					*/
  void *user2;							/* for end user use					*/
  void *user3;							/* for end user use					*/
  void *user4;							/* for end user use					*/
  long dbg_mask;						/* current debug mask				*/
  unsigned long yield_flags;
  unsigned long misc_flags;
  dt_tripwire_t tripwire;
  void *feedbackwin;					/* feedback channel 				*/
  int feedbackonly;						/* exit immediately after feedback?	*/
  void *msw;
} dt_ctl_t;


#define DT_MISC_FLAG_QNX4_TERM			/* qnx4 is in TERM mode 			*/\
  (0x00000001L)

typedef dt_rc_e DTCONFIG_API1 dt_err_f(DT_ERR_ARGS);
typedef void    DTCONFIG_API1 dt_say_f(DT_SAY_ARGS);
typedef void    DTCONFIG_API1 dt_dbg_f(DT_DBG_ARGS);
typedef void    DTCONFIG_API1 dt_err_string_f(DT_ERR_STRING_ARGS);
typedef void    DTCONFIG_API1 dt_say_string_f(DT_SAY_STRING_ARGS);
typedef void    DTCONFIG_API1 dt_dbg_string_f(DT_DBG_STRING_ARGS);

										/* ................................ */
#define DT_ERR_LENGTH (80)				/* typical message length 			*/
typedef int dt_err_t;
#define DT_ERR_NONE (0)					/* no error							*/
										/* error bases 						*/
#define DT_ERR_XCHG (10000)				/* for the dtxchg family 			*/
										/* ................................ */

#define DT_NTYPE_NULL    DTNTYPE_NULL 
#define DT_NTYPE_UNKNOWN DTNTYPE_UNKNOWN 
#define DT_NTYPE_B01     DTNTYPE_B01 
#define DT_NTYPE_B08     DTNTYPE_B08 
#define DT_NTYPE_B16     DTNTYPE_B16 
#define DT_NTYPE_B32     DTNTYPE_B32 
#define DT_NTYPE_S1      DTNTYPE_S1  
#define DT_NTYPE_S2      DTNTYPE_S2  
#define DT_NTYPE_S4      DTNTYPE_S4  
#define DT_NTYPE_F1      DTNTYPE_F1  
#define DT_NTYPE_F2      DTNTYPE_F2  

typedef dtntype_e dt_ntype_e;

typedef dtntype_b01_t dt_ntype_b01_t;
typedef dtntype_b08_t dt_ntype_b08_t;
typedef dtntype_b16_t dt_ntype_b16_t;
typedef dtntype_b32_t dt_ntype_b32_t;
typedef dtntype_s1_t  dt_ntype_s1_t;
typedef dtntype_s2_t  dt_ntype_s2_t;
typedef dtntype_s4_t  dt_ntype_s4_t;
typedef dtntype_f1_t  dt_ntype_f1_t;
typedef dtntype_f2_t  dt_ntype_f2_t;

#define DT_NTYPE_F1_MINVAL (-1e38)
#define DT_NTYPE_F1_MAXVAL (1e38)
#define DT_NTYPE_F2_MINVAL (-1e300)
#define DT_NTYPE_F2_MAXVAL (1e300)

typedef void *dt_ntype_def;				/* dummy for typedef arguments 		*/

#define DT_NTYPE_PAIR(A, B)				/* combine two ntypes into one case	*/\
  ((A)*100+(B))

extern 
const dt_ntype_e *dt_ntype_list;		/* list of all valid ntypes			*/
extern 
const double *dt_ntype_sizeof;			/* array turning ntype_e to sizeof	*/
extern 
const int    *dt_ntype_bits;			/* array turning ntype_e to bits   	*/
extern 
const char  **dt_ntype_string;			/* array turning ntype_e to string	*/
extern 
const double *dt_ntype_minval;			/* array turning ntype_e to minval  */
extern 
const double *dt_ntype_maxval;			/* array turning ntype_e to maxval  */
extern 
const void  **dt_ntype_maxval_ptr;		/* array of native maxval pointers	*/
extern 
const int dt_ntype_count;				/* count of ntypes				    */
extern 
const dt_ntype_e *dt_ntype_alias;

										/* ................................ */
typedef unsigned short dti_small_t;		/* when coords are stored in arrays	*/
typedef unsigned long  dti_large_t;
typedef unsigned int   dti_t;
#define DTI_SMALL_MIN ((dti_small_t)(0)))
#define DTI_SMALL_MAX ((dti_small_t)(0xffffffffL)))
#define DTI_LARGE_MIN ((dti_large_t)(0)))
#define DTI_LARGE_MAX ((dti_large_t)(0xffffffffL)))
#define DTI_MIN       ((dti_t)(0)))
#define DTI_MAX       ((dti_t)(0xffffffffL)))
										/* ................................ */
typedef short dtxy_small_t;				/* when coords are stored in arrays	*/
#define DTXY_SMALL_MIN ((dtxy_small_t)(0)))
#define DTXY_SMALL_MAX ((dtxy_small_t)(0xffff)))

typedef long  dtxy_large_t;
#define DTXY_LARGE_MIN ((dtxy_large_t)(0)))
#define DTXY_LARGE_MAX ((dtxy_large_t)(0xffffffffL)))

typedef dtstats_xy_t dtxy_t;

#ifdef DTCONFIG_16BIT
#define DTXY_MIN       ((dtxy_t)(0)))
#define DTXY_MAX       ((dtxy_t)(0xffff)))
#else
#define DTXY_MIN       ((dtxy_t)(0)))
#define DTXY_MAX       ((dtxy_t)(0xffffffff)))
#endif

#ifdef DTCONFIG_IMPLIED_DOS16
# define DT_PTR_TO_ULONG(P) \
    ((unsigned long)(FP_SEG((void *)(P))) * 16UL + \
     (unsigned long)(FP_OFF((void *)(P))))
#else
# define DT_PTR_TO_ULONG(P) \
    ((unsigned long)(P))
#endif

										/* ................................ */
#define DT_MIN(A, B) \
  (((A)<(B))?(A):(B))
#define DT_MAX(A, B) \
  (((A)>(B))?(A):(B))
#define DT_ABBREV(S, T) \
  (!strncmp(S, T, strlen(T)))


										/* ................................ */
#define DT_SEPARATOR_LIST ('+')
#define DT_SEPARATOR_SPEC (',')
#define DT_SEPARATOR_ENUM ('=')

										/* ................................ */
#define DT_DBG(CTL, MASK) 													  \
  (((MASK) & DT_DBG_MASK_ANY) ||											  \
   ((MASK) == DT_DBG_MASK_ALL) ||											  \
   ((CTL) != NULL && 														  \
    ((CTL)->dbg_mask & DT_DBG_MASK_ANY || 									  \
     (CTL)->dbg_mask & (MASK))))
#define DT_DBG_MASK_ALWAYS   (0x40000000L)	/* value to print anything		*/
#define DT_DBG_MASK_ALL      (0x3fffffffL)	/* all debug masks				*/
#define DT_DBG_MASK_STATS    (0x00000001L)	/* statistics					*/
#define DT_DBG_MASK_IMG      (0x00000002L)	/* image operations				*/
#define DT_DBG_MASK_TIME     (0x00000004L)	/* execution times				*/
#define DT_DBG_MASK_COMPLEX  (0x00000008L)	/* execution complexity 		*/
#define DT_DBG_MASK_IPC      (0x00000010L)	/* interprocess communication	*/
#define DT_DBG_MASK_ARG      (0x00000020L)	/* arguments					*/
#define DT_DBG_MASK_ASSERT   (0x00000040L)	/* assertions					*/
#define DT_DBG_MASK_MEMTRACE (0x00000080L)	/* any memory allocation/free	*/
#define DT_DBG_MASK_ASYNC    (0x00000100L)	/* asynchronous events			*/
#define DT_DBG_MASK_BIGIO    (0x00000200L)	/* big blocks of I/O only		*/
#define DT_DBG_MASK_PAGE     (0x00000400L)	/* memory paging				*/
#define DT_DBG_MASK_WIN      (0x00000800L)	/* window interface				*/
#define DT_DBG_MASK_ALIMITS  (0x00001000L)	/* print limits					*/
#define DT_DBG_MASK_OBJECT   (0x00004000L)	/* object values				*/
#define DT_DBG_MASK_INTERMD  (0x00004000L)	/* intermediate values			*/
#define DT_DBG_MASK_PIXEL    (0x00008000L)	/* pixel values					*/
#define DT_DBG_MASK_IO       (0x00010000L)	/* all I/O						*/
#define DT_DBG_MASK_MEMSTATS (0x00020000L)	/* memory summary				*/
#define DT_DBG_MASK_ALT      (0x00040000L)	/* program-choosen alternative	*/
#define DT_DBG_MASK_CSTACK   (0x00080000L)	/* c compiler stack				*/
#define DT_DBG_MASK_MEMX     (0x00100000L)	/* extended memory operations	*/
#define DT_DBG_MASK_LUT      (0x00200000L)	/* lut operations				*/
#define DT_DBG_MASK_COMMAND  (0x00400000L)	/* commands						*/
#define DT_DBG_MASK_REPLY    (0x00800000L)	/* replies to commands			*/
#define DT_DBG_MASK_SCHEDULE (0x01000000L)	/* scheduling					*/
#define DT_DBG_MASK_CONFIG	 (0x02000000L)  /* config						*/
#define DT_DBG_MASK_TRACE    (0x08000000L)	/* execution trace				*/
#define DT_DBG_MASK_USR1     (0x10000000L)	/* user's use					*/
#define DT_DBG_MASK_USR2     (0x20000000L)	/* user's use					*/

#define DT_DBG_MASK_ANY      DT_DBG_MASK_ALWAYS

extern const long *dt_dbg_mask_values;
extern const char **dt_dbg_mask_names;
extern const char **dt_dbg_mask_descs;
										/* ................................ */
#define DT_MARK         (0x01)			/* marks						    */
#define DT_MARK_MAXIMUM	(0x02)
#define DT_MARK_MINIMUM	(0x04)
#define DT_MARK_START   (0x08)
#define DT_MARK_END	    (0x10)
#define DT_MARK_DEF1    (0x20)
#define DT_MARK_DEF2    (0x40)
#define DT_MARK_DEF3    (0x80)


										/* ................................ */
typedef int dt_key_t;
#define DT_KEY_IGNORE     (0x100)
#define DT_KEY_UPARROW    (0x101)
#define DT_KEY_DOWNARROW  (0x102)
#define DT_KEY_LEFTARROW  (0x103)
#define DT_KEY_RIGHTARROW (0x104)
#define DT_KEY_PAGEDOWN   (0x105)
#define DT_KEY_PAGEUP     (0x106)
#define DT_KEY_F1         (0x107)
#define DT_KEY_F2         (0x108)
#define DT_KEY_F3         (0x109)
#define DT_KEY_F4         (0x10a)
#define DT_KEY_F5         (0x10b)
#define DT_KEY_F6         (0x10c)
#define DT_KEY_F7         (0x10d)
#define DT_KEY_F8         (0x10e)
#define DT_KEY_F9         (0x10f)
#define DT_KEY_F10        (0x110)
#define DT_KEY_ENTER      (0x111)
#define DT_KEY_SPACE      (0x112)
#define DT_KEY_BACKSPACE  (0x113)
#define DT_KEY_DELETE     (0x114)
#define DT_KEY_INSERT     (0x115)
#define DT_KEY_TAB        (0x116)
#define DT_KEY_HOME       (0x117)
#define DT_KEY_END        (0x118)
#define DT_KEY_ERASEEOL   (0x119)
#define DT_KEY_BACKTAB    (0x11a)
#define DT_KEY_STOP       (0x11b)
#define DT_KEY_TIMER      (0x17f)		/* timer tick looks like attn key   */

#define DT_KEY_JUMP_MIN   (0x180)		/* jump-to-cons					    */
#define DT_KEY_JUMP_MAX   (0x18f)

#define DT_KEY_ESCAPE     (0x01b)
#define DT_KEY_FLAG_NOECHO (0)			/* don't echo keys					*/


typedef enum {
  DT_EVENT_UNKNOWN = 0,
  DT_EVENT_NOTHING,
  DT_EVENT_POLL,
  DT_EVENT_IPC,
  DT_EVENT_MOUSEMOVE,
  DT_EVENT_MOUSEPRESS,
  DT_EVENT_MOUSERELEASE,
  DT_EVENT_KEYBOARD,
  DT_EVENT_MOVED,
  DT_EVENT_RESIZED,
  DT_EVENT_EXPOSED,
  DT_EVENT_COVERED,
  DT_EVENT_ICONIZED,
  DT_EVENT_LUTCHANGED,					/* base lookup table changed		*/
  DT_EVENT_DESTROYED,					/* window destroyed 				*/
  DT_EVENT_KILLED,						/* connection with server killed 	*/
  DT_EVENT_MOUSEDOUBLEPRESS
} dt_event_e;

typedef struct {
  dt_event_e type;
  long id;
  dt_key_t keys[DT_KEYSTRING_MAXLEN];
  int nkeys;
  int mousex;
  int mousey;
  int left;
  int middle;
  int right;
  int x0, y0, xe, ye;
} dt_event_t;

										/* ................................ */
#define DT_CURSOR_OP_OFF (0)	
#define DT_CURSOR_OP_NORMAL (1)
#define DT_CURSOR_OP_DRAG (2)
#define DT_CURSOR_OP_QUERY (3)
										/* ................................ */

typedef enum {							/* operations					    */
  DT_OP_MIN,
  DT_OP_MAX,
  DT_OP_AVE,
  DT_OP_AND,
  DT_OP_OR,
  DT_OP_XOR,
  DT_OP_INVERT,
  DT_OP_ADD,
  DT_OP_SUB,
  DT_OP_SUBFROM,
  DT_OP_SUBABS,
  DT_OP_MUL,
  DT_OP_DIV,
  DT_OP_ROW,
  DT_OP_COL,
  DT_OP_LUT,
  DT_OP_CLEAR,
  DT_OP_SET,
  DT_OP_ABS,
  DT_OP_CLIP,
  DT_OP_CAST,
  DT_OP_UNKNOWN = 1000
} dt_op_e;

#define DT_OP_MULT DT_OP_MUL

										/* ................................ */
										/* conversion before assignment		*/
#define DT_ASSIGN_CAST    (0x0001)
#define DT_ASSIGN_ABS     (0x0002)
#define DT_ASSIGN_CLIP    (0x0004)
#define DT_ASSIGN_REVERSE (0x0008)

										/* ................................ */

typedef dt_rc_e DTCONFIG_API1 \
  dt_yield_f(DT_YIELD_ARGS);

#define DT_YIELD_FLAG_OFF       (0x00000000L)
#define DT_YIELD_FLAG_LOW       (0x00010000L)
#define DT_YIELD_FLAG_LESS      (0x00040000L)
#define DT_YIELD_FLAG_NORMAL    (0x00080000L)
#define DT_YIELD_FLAG_HIGH      (0x00200000L)
#define DT_YIELD_FLAG_HIGHER    (0x00400000L)
#define DT_YIELD_FLAG_HIGHEST   (0x00800000L)
#define DT_YIELD_FLAG_DOT       (0x10000000L)
#define DT_YIELD_FLAG_TWIRL     (0x20000000L)
#define DT_YIELD_FLAG_MESSAGE   (0x40000000L)
#define DT_YIELD_FLAG_CHARMASK	(0x000000ffL)
#define DT_YIELD_FLAG_CHARSHIFT (0)

#define DT_YIELD(CHECK, STATUS) \
  {if (ctl && \
       ctl->yield && \
       ctl->yield_flags && \
       !(ctl->yield_flags & DT_YIELD_FLAG_OFF) && \
       ctl->cons) \
   { \
     CHECK(dt_yield,(ctl, STATUS)); \
   } \
  }

#ifndef DT_YIELD_ROW_MODULUS
#define DT_YIELD_ROW_MODULUS (100)
#endif
#define DT_YIELD_ROW(CHECK, STATUS, Y, YE) \
  {if (ctl && \
       ctl->yield && \
       ctl->yield_flags && \
       !(ctl->yield_flags & DT_YIELD_FLAG_OFF) && \
       ctl->cons && \
       ((Y) % DT_YIELD_ROW_MODULUS == 0 || \
        (Y) == (YE)-1)) \
    { \
      CHECK(dt_yield,(ctl, "%s row %u/%u", \
        STATUS, (Y), (YE))); \
    } \
  }

#ifndef DT_YIELD_PERCENT_MODULUS
#define DT_YIELD_PERCENT_MODULUS (10)
#endif
#define DT_YIELD_PERCENT(CHECK, STATUS, Y, YE) \
  {if (ctl && \
       ctl->yield && \
       ctl->yield_flags && \
       !(ctl->yield_flags & DT_YIELD_FLAG_OFF) && \
       ctl->cons && \
       (((Y) == (YE-1)) || \
        (((100*(unsigned long)(Y)) / (unsigned long)(YE)) % \
         DT_YIELD_PERCENT_MODULUS == 0))) \
    { \
      CHECK(dt_yield,(ctl, "%s %u%%", \
        STATUS, ((100*(unsigned long)(Y)) / (unsigned long)(YE)))); \
    } \
  }

										/* ................................ */
#define DT_CAST(TYPE, VAL) ((TYPE)(VAL))
										/* ................................ */
#define DT_QUOTE(A) #A
										/* ................................ */
typedef void dt_rc_check_f(void);		/* dummy for DT_Q and friends 		*/
						
#define DT_CLEANUP(A)					/* give error and go to cleanup 	*/\
{										\
  rc = dt_err A;						\
  goto cleanup;							\
}

#define DT_Q(FUNC, ARGS)				/* check rc and return				*/\
{                                                                             \
  dt_rc_e rc;                                                                 \
  DT_SETLOC                                                                   \
  rc = FUNC ARGS;		                                                      \
  if (rc != DT_RC_GOOD) 		                                              \
  {                                                                           \
    dt_dbg_trace(ctl, F,                                                      \
      DT_RCSID_STRING, __LINE__,                                              \
      "rc=%d", rc);                                        \
    return rc;			                                                      \
  }                                                                           \
}

#define DT_G(FUNC, ARGS)				/* do func, always set rc			*/\
{                                                                             \
  DT_SETLOC                                                                   \
  rc = FUNC ARGS;				                                              \
  if (rc != DT_RC_GOOD) 		                                              \
    dt_dbg_trace(ctl, F,                                                      \
      DT_RCSID_STRING, __LINE__,                                              \
      "rc=%d", rc);                                        \
}

#define DT_C(FUNC, ARGS)				/* check rc and goto cleanup		*/\
{                                                                             \
  DT_SETLOC                                                                   \
  rc = FUNC ARGS;				                                              \
  if (rc != DT_RC_GOOD) 		                                              \
  {                                                                           \
    dt_dbg_trace(ctl, F,                                                      \
      DT_RCSID_STRING, __LINE__,                                              \
      "rc=%d", rc); 		                                  \
    goto cleanup;                                                             \
  }                                                                           \
}

#define DT_GI(FUNC, ARGS)               /* check rc before and after		*/\
{                                                                             \
  DT_SETLOC                                                                   \
  if (rc == DT_RC_GOOD)                                                       \
  { DT_G(FUNC, ARGS) } }

#define DT_I(FUNC, ARGS)				/* do func, only set rc if bad		*/\
{                                                                             \
  dt_rc_e trc;          		                                              \
  DT_SETLOC                                                                   \
  trc = FUNC ARGS;		                                                      \
  if (trc != DT_RC_GOOD)    		                                          \
  {                                                                           \
    dt_dbg_trace(ctl, F,                                                      \
      DT_RCSID_STRING, __LINE__,                                              \
      "rc=%d", trc);                                       \
    if (rc == DT_RC_GOOD)                                                     \
      rc = trc; 	                                                          \
  }                                                                           \
}

#define DT_N(FUNC, ARGS) 	            /* ignore rc						*/\
{                                                                             \
  DT_SETLOC                                                                   \
  FUNC ARGS;                                                                  \
}

										/* ................................ */
#define DT_Q_1D(FUNC, ARGS)				/* check rc and goto cleanup		*/\
{                                                                             \
  dt_rc_e rc = (dt_rc_e)FUNC ARGS;	                                          \
  if (rc != DT_RC_GOOD) 		                                              \
  {                                                                           \
    rc = dt_err(ctl,                                                   \
      "failed, rc=%d", rc); 	         	                                  \
    dt_dbg_trace(ctl, F,                                                      \
      DT_RCSID_STRING, __LINE__,                                              \
      "rc=%d", rc); 		                                  \
    return rc;                                                                \
  }                                                                           \
}

										/* ................................ */
#define DT_C_1D(FUNC, ARGS)				/* check rc and goto cleanup		*/\
{                                                                             \
  rc = (dt_rc_e)FUNC ARGS;	                                                  \
  if (rc != DT_RC_GOOD) 		                                              \
  {                                                                           \
    rc = dt_err(ctl,                                                   \
      "failed, rc=%d", rc); 	         	                                  \
    dt_dbg_trace(ctl, F,                                                      \
      DT_RCSID_STRING, __LINE__,                                              \
      "rc=%d", rc); 		                                  \
    goto cleanup;                                                             \
  }                                                                           \
}
										/* ................................ */

#ifndef DT_SETLOC						/* not already defined? 			*/
#  if DTPRJ_STRATOS_MEMCHECK >= 1		/* using MemCheck?					*/
#    define DT_SETLOC mc_set_location();
#  else
#    define DT_SETLOC
#  endif
#endif

										/* ................................ */
#define	DT_PI	3.14159265358979323846
#define	DT_E	2.7182818284590452354
										/* ................................ */
										/* misc flags					    */
#define DT_DO_START_PAGE (0x0002L)		/* start new printed page	    	*/
#define DT_DO_END_PAGE 	 (0x0004L)		/* end current printed page			*/
#define DT_HAS_XPOS      (0x0008L)		/* image has x position				*/
#define DT_HAS_YPOS      (0x0010L)		/* image has y position				*/
										/* ................................ */

typedef
dt_rc_e
DTCONFIG_API1
dt_double_f(							/* function returning double		*/
  dt_ctl_t *ctl,
  void *arg,
  double in,
  double *out);

										/* ................................ */
#include <dtack/param.h>					/* basic data types					*/
#include <dtack/cstack.h>					/* compiler stack checking 			*/
#include <dtack/hist.h>						/* histogram 						*/
#include <dtack/fd.h>						/* file I/O 						*/
#include <dtack/run.h>						/* connected components 			*/
#include <dtack/image.h>

										/* ................................ */


typedef struct {
  char *desc;
  char *help;
  char *val;
  int val_max;
  int reserved;
  unsigned long flags;
} dt_enter_arg_t;

typedef dt_rc_e DTCONFIG_API1
dt_enter_check_f(						/* check args/params for this app	*/
  dt_ctl_t *ctl,
  void *appspace,
  dtparam_t *param,
  dt_enter_arg_t *arg,
  char *message,
  const char **start);

#define DT_ENTER_MSG_MAXLEN (80)		/* size of message to check funcs 	*/

typedef
dt_rc_e
DTCONFIG_API1
dt_enter_newfield_f(					/* function called for each field	*/
  dt_ctl_t *ctl,
  void *arg,
  char *abbrev,
  char *desc,
  char *val,
  int x,
  int y,
  int l1,
  int l2);

										/* ................................ */

#include <dtack/base.p>							/* function prototypes				*/





/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
  ! Copyright (C) 1998 Dtack Inc. All Rights Reserved
  ! To use this file, you must have signed a license agreement with Dtack Inc.
  ! Under no circumstances may you redistribute this file.
  ! This software is provided AS IS with no warranty expressed or implied.
  ! Dtack Inc. accepts no liability for use or misuse of this file.
  ! http://www.dtack.com  dtack@dtack.com  telephone +360.417.6700
  ! Dtack Inc, P.O. Box 3061, Port Angeles, WA USA 98362
  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */
