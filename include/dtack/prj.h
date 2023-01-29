/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
  ! Copyright (C) 1998 Dtack Inc. All Rights Reserved
  ! To use this file, you must have signed a license agreement with Dtack Inc.
  ! Under no circumstances may you redistribute this file.
  ! This software is provided AS IS with no warranty expressed or implied.
  ! Dtack Inc. accepts no liability for use or misuse of this file.
  ! http://www.dtack.com  dtack@dtack.com  telephone +360.417.6700
  ! Dtack Inc, P.O. Box 3061, Port Angeles, WA USA 98362
  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */





/*..........................................................................*/
#if DTPRJ_NOPAGING == DT_1				/* paging not allowed? 				*/
										/* disable all paging types too 	*/
#  ifdef DTPRJ_EMS
#  undef DTPRJ_EMS			
#  endif
#  ifdef DTPRJ_XMSIF
#  undef DTPRJ_XMSIF
#  endif
#  ifdef DTPRJ_XMSLB
#  undef DTPRJ_XMSLB
#  endif
#  ifdef DTPRJ_TXMS
#  undef DTPRJ_TXMS
#  endif
#  ifdef DTPRJ_MEMDISK
#  undef DTPRJ_MEMDISK
#  endif
#else                                   /* paging not disallowed?			*/
#  define DTPRJ_MEMDISK DT_1			/* if we can page, we can do disk 	*/
#endif
/*..........................................................................*/

extern const int dtprj_fastgraph;
extern const int dtprj_svgacc;
extern const int dtprj_cortex;
extern const int dtprj_vmc;
extern const int dtprj_x;
extern const int dtprj_fgw;
extern const int dtprj_ncsa_dtm;
extern const int dtprj_qnx4_ipc;
extern const int dtprj_socket;
extern const int dtprj_emsif;
extern const int dtprj_xmsif;
extern const int dtprj_xmslb;
extern const int dtprj_txms;
extern const int dtprj_memdisk;
extern const int dtprj_qw4;
extern const int dtprj_gws;
extern const int dtprj_tiff;
extern const int dtprj_curses;
extern const int dtprj_termcap;
extern const int dtprj_termios;
extern const int dtprj_qnxterm;
extern const int dtprj_noscreen;
extern const int dtprj_nointer;
extern const int dtprj_nowin;

extern const int dtprj_stratos_memcheck;

typedef const struct {
  const char *abbrev;
  const char *dflt;
} dtprj_param_defaults_t;

extern dtprj_param_defaults_t			/* table of compile-time defaults 	*/
  dtprj_param_defaults[];

										/* DTPRJ_SOCKET_PORT:  				*/
										/* Default port number used by 		*/
										/* clients and servers. 			*/
#ifndef DTPRJ_SOCKET_HOST
#define DTPRJ_SOCKET_HOST "localhost"
#endif
#ifndef DTPRJ_SOCKET_PORT
#define DTPRJ_SOCKET_PORT "5710"
#endif
										/* DTPRJ_SOCKET_PORT2:  			*/
										/* Used by test programs which want	*/
										/* to run without interfering with 	*/
										/* a legitimate server.				*/
#ifndef DTPRJ_SOCKET_HOST2
#define DTPRJ_SOCKET_HOST2 "localhost"
#endif
#ifndef DTPRJ_SOCKET_PORT2
#define DTPRJ_SOCKET_PORT2 "5711"
#endif

										/* DTPRJ_QNX4_IPC_NAME: 			*/
										/* Default registered name used by	*/
										/* clients and servers. 			*/
#ifndef DTPRJ_QNX4_IPC_NAME
#define DTPRJ_QNX4_IPC_NAME "/dtack/ipc"
#endif
										/* DTPRJ_QNX4_IPC_NAME2:  			*/
										/* Used by test programs which want	*/
										/* to run without interfering with 	*/
										/* a legitimate server.				*/
#ifndef DTPRJ_QNX4_IPC_NAME2
#define DTPRJ_QNX4_IPC_NAME2 "/dtack/ipc2"
#endif


#include <dtack/prj.p>





/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
  ! Copyright (C) 1998 Dtack Inc. All Rights Reserved
  ! To use this file, you must have signed a license agreement with Dtack Inc.
  ! Under no circumstances may you redistribute this file.
  ! This software is provided AS IS with no warranty expressed or implied.
  ! Dtack Inc. accepts no liability for use or misuse of this file.
  ! http://www.dtack.com  dtack@dtack.com  telephone +360.417.6700
  ! Dtack Inc, P.O. Box 3061, Port Angeles, WA USA 98362
  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */
