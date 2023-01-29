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
  19991217 DE split out stuff into config2.h
  19981015 DE raised MALLOC_MAX to bigger number
  19980716 DE added DTCONFIG_FREE_ARGTYPE
  19980716 DE corrected wrong IMPLIED for SP1
 *..........................................................................*/


/*
 * DTCONFIG_XXX is defined on the compiler command line
 * the "platform" is supposed to be a very general concept of the
 * compile-time environment
 */

#define DT_1 1							/* for #define value comparison	    */

#define DTCONFIG_FREE_ARGTYPE void		/* assumed true unless changed		*/

#define DTCONFIG_MALLOC_MAX_BIG (2048L*2048L*8L*2L*4L)

/*..........................................................................*/ 
#ifdef DTCONFIG_SG1
#define DTCONFIG "SG1"
#define DTCONFIG_IMPLIED_UNIXLIKE
#define DTCONFIG_IMPLIED_SUNOS
#define DTCONFIG_IMPLIED_GNU
#define DTCONFIG_32BIT
#define DTCONFIG_MALLOC_MAX (DTCONFIG_MALLOC_MAX_BIG)
#define DTCONFIG_READ_MAX DTCONFIG_MALLOC_MAX
#define DTCONFIG_WRITE_MAX DTCONFIG_MALLOC_MAX
#define DTCONFIG_WANT_STATIC_RCSID
#define DTCONFIG_WANT_STATIC_F
#define DTCONFIG_OSTIME_GETRUSAGE		/* use getrusage() in dtos_time()	*/

#define __USE_FIXED_PROTOTYPES__		/* some versions of gcc need it???	*/
#endif

/*..........................................................................*/ 
#ifdef DTCONFIG_SG5
#define DTCONFIG "SG5"
#define DTCONFIG_IMPLIED_UNIXLIKE
#define DTCONFIG_IMPLIED_SOLARIS_2_5
#define DTCONFIG_IMPLIED_SOLARIS
#define DTCONFIG_IMPLIED_GNU
#define DTCONFIG_32BIT
#define DTCONFIG_MALLOC_MAX (DTCONFIG_MALLOC_MAX_BIG)
#define DTCONFIG_READ_MAX DTCONFIG_MALLOC_MAX
#define DTCONFIG_WRITE_MAX DTCONFIG_MALLOC_MAX
#define DTCONFIG_WANT_STATIC_RCSID
#define DTCONFIG_WANT_STATIC_F
#define DTCONFIG_OSTIME_PIOCUSAGE		/* use proc in dtos_time()			*/
#endif

/*..........................................................................*/ 
/* si2 is: Silicon Graphics with native IRIX compiler						*/
#ifdef DTCONFIG_SI2
#define DTCONFIG "SI2"
#define DTCONFIG_IMPLIED_UNIXLIKE
#define DTCONFIG_IMPLIED_IRIX
#define DTCONFIG_32BIT
#define DTCONFIG_MALLOC_MAX (DTCONFIG_MALLOC_MAX_BIG)
#define DTCONFIG_READ_MAX DTCONFIG_MALLOC_MAX
#define DTCONFIG_WRITE_MAX DTCONFIG_MALLOC_MAX
#define DTCONFIG_WANT_STATIC_RCSID
#define DTCONFIG_WANT_STATIC_F
#define DTCONFIG_OSTIME_GETRUSAGE		/* use getrusage() in dtos_time()	*/
#define DTCONFIG_OSTIME_PIOCUSAGE		/* use proc in dtos_time()			*/
#endif

/*..........................................................................*/ 
/* sp0 is: SunOS 4.1.x with SUNPRO 3.0.1 compiler							*/
#ifdef DTCONFIG_SP0
#define DTCONFIG "SP0"
#define DTCONFIG_IMPLIED_UNIXLIKE
#define DTCONFIG_IMPLIED_SUNOS
#define DTCONFIG_32BIT
#define DTCONFIG_MALLOC_MAX (DTCONFIG_MALLOC_MAX_BIG)
#define DTCONFIG_READ_MAX DTCONFIG_MALLOC_MAX
#define DTCONFIG_WRITE_MAX DTCONFIG_MALLOC_MAX
#define DTCONFIG_WANT_STATIC_RCSID
#define DTCONFIG_WANT_STATIC_F
#define DTCONFIG_OSTIME_GETRUSAGE		/* use getrusage() in dtos_time()	*/

#ifdef __cplusplus
#  undef  DTCONFIG_FREE_ARGTYPE 
#  define DTCONFIG_FREE_ARGTYPE char
#endif
#endif

/*..........................................................................*/ 
/* sp1 is: Solaris 2.5.x with SUNPRO 4.2 compiler							*/
#ifdef DTCONFIG_SP1
#define DTCONFIG "SP1"
#define DTCONFIG_IMPLIED_UNIXLIKE
#define DTCONFIG_IMPLIED_SOLARIS
#define DTCONFIG_32BIT
#define DTCONFIG_MALLOC_MAX (DTCONFIG_MALLOC_MAX_BIG)
#define DTCONFIG_READ_MAX DTCONFIG_MALLOC_MAX
#define DTCONFIG_WRITE_MAX DTCONFIG_MALLOC_MAX
#define DTCONFIG_WANT_STATIC_RCSID
#define DTCONFIG_WANT_STATIC_F
#define DTCONFIG_OSTIME_PIOCUSAGE		/* use proc in dtos_time()			*/
#endif

/*..........................................................................*/ 
#ifdef DTCONFIG_FG1
#define DTCONFIG "FG1"
#define DTCONFIG_IMPLIED_UNIXLIKE
#define DTCONFIG_IMPLIED_GNU
#define DTCONFIG_32BIT
#define DTCONFIG_MALLOC_MAX (DTCONFIG_MALLOC_MAX_BIG)
#define DTCONFIG_READ_MAX DTCONFIG_MALLOC_MAX
#define DTCONFIG_WRITE_MAX DTCONFIG_MALLOC_MAX
#define DTCONFIG_WANT_STATIC_RCSID
#define DTCONFIG_WANT_STATIC_F
#define DTCONFIG_OSTIME_GETRUSAGE		/* use getrusage() in dtos_time()	*/
#endif

/*..........................................................................*/ 
#ifdef DTCONFIG_LG1
#define DTCONFIG "LG1"
#define DTCONFIG_IMPLIED_UNIXLIKE
#define DTCONFIG_IMPLIED_GNU
#define DTCONFIG_IMPLIED_LINUX
#define DTCONFIG_32BIT
#define DTCONFIG_MALLOC_MAX (DTCONFIG_MALLOC_MAX_BIG)
#define DTCONFIG_READ_MAX DTCONFIG_MALLOC_MAX
#define DTCONFIG_WRITE_MAX DTCONFIG_MALLOC_MAX
#define DTCONFIG_WANT_STATIC_RCSID
#define DTCONFIG_WANT_STATIC_F
#define DTCONFIG_OSTIME_GETRUSAGE		/* use getrusage() in dtos_time()	*/
#endif

/*..........................................................................*/ 
#ifdef DTCONFIG_LG6						/* 64-bit Linux 2.0.30 				*/
#define DTCONFIG "LG6"
#define DTCONFIG_IMPLIED_UNIXLIKE
#define DTCONFIG_IMPLIED_GNU
#define DTCONFIG_IMPLIED_LINUX
#define DTCONFIG_64BIT
#define DTCONFIG_MALLOC_MAX (DTCONFIG_MALLOC_MAX_BIG)
#define DTCONFIG_READ_MAX DTCONFIG_MALLOC_MAX
#define DTCONFIG_WRITE_MAX DTCONFIG_MALLOC_MAX
#define DTCONFIG_WANT_STATIC_RCSID
#define DTCONFIG_WANT_STATIC_F
#define DTCONFIG_OSTIME_GETRUSAGE		/* use getrusage() in dtos_time()	*/
#endif


/*..........................................................................*/ 
#ifdef DTCONFIG_QW0
#  define DTCONFIG "QW0"
#  define DTCONFIG_WATCOM_QNX
#  define DTCONFIG_IMPLIED_QNX416
#endif

/*..........................................................................*/ 
#ifdef DTCONFIG_QW1
#  define DTCONFIG "QW1"
#  define DTCONFIG_WATCOM_QNX
#  define DTCONFIG_IMPLIED_QNX432
#endif

/*..........................................................................*/ 
#ifdef DTCONFIG_QW6
#  define DTCONFIG "QW6"
#  define DTCONFIG_WATCOM_QNX
#  define DTCONFIG_IMPLIED_QNX432
#endif

/*..........................................................................*/ 
#ifdef DTCONFIG_DW0
#define DTCONFIG "DW0"
#define DTCONFIG_WATCOM_DOS				/* this is a DOS Watcom compiler    */
#define DTCONFIG_IMPLIED_DOS16			/* this is going to run on DOS	    */
#endif

/*..........................................................................*/ 
#ifdef DTCONFIG_DW1
#define DTCONFIG "DW1"
#define DTCONFIG_WATCOM_DOS				/* this is a DOS Watcom compiler    */
#define DTCONFIG_IMPLIED_DOS32			/* this is going to run on DOS	    */
#endif

/*..........................................................................*/ 
#ifdef DTCONFIG_DT0
#define DTCONFIG "DT0"
#define DTCONFIG_BORLAND				/* this is a Borland compiler	    */
#define DTCONFIG_IMPLIED_DOS16			/* this is going to run on DOS	    */
#endif

/*..........................................................................*/ 
#ifdef DTCONFIG_WW0
#define DTCONFIG "WW0"
#define DTCONFIG_WATCOM_DOS				/* this is a DOS Watcom compiler    */
#define DTCONFIG_IMPLIED_MSW16			/* this is going to run on Windows  */
#endif

/*..........................................................................*/ 
#ifdef DTCONFIG_DB0
#define DTCONFIG "DB0"
#define DTCONFIG_BORLAND				/* this is a Borland compiler	    */
#define DTCONFIG_IMPLIED_DOS16			/* this is going to run on DOS	    */
#endif

/*..........................................................................*/ 
#ifdef DTCONFIG_WB0
#  define DTCONFIG "WB0"
#  define DTCONFIG_BORLAND				/* this is a Borland compiler	    */
#  define DTCONFIG_IMPLIED_MSW16		/* this is going to run on Windows	*/
#  define _RTLDLL   					/* use BC30RTL.DLL				    */
#endif

/*..........................................................................*/ 
#ifdef DTCONFIG_WB3
#  define DTCONFIG "WB3"
#  define DTCONFIG_BORLAND				/* this is a Borland compiler	    */
#  define DTCONFIG_IMPLIED_MSW32		/* this is going to run on Windows	*/
#  define _RTLDLL  						/* use runtime C dll				*/
#endif

/*..........................................................................*/ 
#ifdef DTCONFIG_DV0
#define DTCONFIG "DV0"
#define DTCONFIG_MICROSOFT				/* this is a Microsoft compiler	    */
#define DTCONFIG_IMPLIED_DOS16			/* this is going to run on DOS	    */
#endif

/*..........................................................................*/ 
#ifdef DTCONFIG_WV0
#  define DTCONFIG "WV0"
#  define DTCONFIG_MICROSOFT			/* this is a Microsoft compiler	    */
#  define DTCONFIG_IMPLIED_MSW16		/* this is going to run on Windows	*/
#endif

/*..........................................................................*/ 
#ifdef DTCONFIG_WV4
#  define DTCONFIG "WV4"
#  define DTCONFIG_MICROSOFT			/* this is a Microsoft compiler	    */
#  define DTCONFIG_IMPLIED_MSW32		/* this is going to run on Windows	*/
#endif

/*..........................................................................*/ 
#ifdef DTCONFIG_WV5
#  define DTCONFIG "WV5"
#  define DTCONFIG_MICROSOFT			/* this is a Microsoft compiler	    */
#  define DTCONFIG_IMPLIED_MSW32		/* this is going to run on Windows	*/
#endif

/*..........................................................................*/ 
#ifndef DTCONFIG
* error: unknown os! *
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
