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
  19991009 DE enable dos key mappings in console mode
  19990501 DE support MSW console mode
 *..........................................................................*/


#include <dtack/base.h>
#include <dtack/prj.h>

/*..........................................................................*/

const int dtprj_console =
#if DTPRJ_CONSOLE == DT_1
  1;
#else
  0;
#endif

const int dtprj_fastgraph =
#if DTPRJ_FASTGRAPH_FAKE == DT_1
  0;
#else
#  if DTPRJ_FASTGRAPH == DT_1
    1;
#  else
    0;
#  endif
#endif

const int dtprj_fastvirt =
#if DTPRJ_FASTVIRT_FAKE == DT_1
  0;
#else
#  if DTPRJ_FASTVIRT == DT_1
    1;
#  else
    0;
#  endif
#endif

const int dtprj_svgacc =
#if DTPRJ_SVGACC_FAKE == DT_1
  0;
#else
#  if DTPRJ_SVGACC == DT_1
    1;
#  else
    0;
#  endif
#endif

const int dtprj_cortex =
#if DTPRJ_CORTEX == DT_1
  1;
#else
  0;
#endif

const int dtprj_vmc =
#if DTPRJ_VMC == DT_1
  1;
#else
  0;
#endif

const int dtprj_x =
#if DTPRJ_X == DT_1
  1;
#else
  0;
#endif

const int dtprj_fgw =
#if DTPRJ_FGW == DT_1
  1;
#else
  0;
#endif

const int dtprj_ncsa_dtm =
#if DTPRJ_NCSA_DTM == DT_1
  1;
#else
  0;
#endif

const int dtprj_qnx4_ipc =
#if DTPRJ_QNX4_IPC == DT_1
  1;
#else
  0;
#endif

const int dtprj_socket =
#if DTPRJ_SOCKET == DT_1
  1;
#else
  0;
#endif

const int dtprj_dde_ipc =
#if DTPRJ_DDE_IPC == DT_1
  1;
#else
  0;
#endif

const int dtprj_emsif =
#if DTPRJ_EMSIF == DT_1
  1;
#else
  0;
#endif


const int dtprj_xmsif =
#if DTPRJ_XMSIF == DT_1
  1;
#else
  0;
#endif


const int dtprj_xmslb =
#if DTPRJ_XMSLB == DT_1
  1;
#else
  0;
#endif


const int dtprj_txms =
#if DTPRJ_TXMS == DT_1
  1;
#else
  0;
#endif


const int dtprj_memdisk =
#if DTPRJ_MEMDISK == DT_1
  1;
#else
  0;
#endif


const int dtprj_qw4 =
#if DTPRJ_QW4 == DT_1
  1;
#else
  0;
#endif


const int dtprj_gws =
#if DTPRJ_GWS == DT_1
  1;
#else
  0;
#endif


const int dtprj_tiff =
#if DTPRJ_TIFF == DT_1
  1;
#else
  0;
#endif


const int dtprj_curses =
#if DTPRJ_CURSES == DT_1
  1;
#else
  0;
#endif


const int dtprj_termcap =
#if DTPRJ_TERMCAP == DT_1
  1;
#else
  0;
#endif

const int dtprj_termios =
#if DTPRJ_TERMIOS == DT_1
  1;
#else
  0;
#endif

const int dtprj_int10 =
#if DTPRJ_INT10 == DT_1
  1;
#else
  0;
#endif

const int dtprj_qnxterm =
#if DTPRJ_QNXTERM == DT_1
  1;
#else
  0;
#endif

const int dtprj_noscreen =
#if DTPRJ_NOSCREEN == DT_1
  1;
#else
  0;
#endif

const int dtprj_nointer =
#if DTPRJ_NOINTER == DT_1
  1;
#else
  0;
#endif

const int dtprj_nowin =
#if DTPRJ_NOWIN == DT_1
  1;
#else
  0;
#endif

										/* ................................ */
const int dtprj_stratos_memcheck =
#ifdef DTPRJ_STRATOS_MEMCHECK
  DTPRJ_STRATOS_MEMCHECK;
#else
  0;
#endif

										/* ................................ */
										/* defaults for command line 		*/
										/* parameters which have no 		*/
										/* environment variable set 		*/
dtprj_param_defaults_t 
dtprj_param_defaults[] = {
										  
										/* ................................ */
#if     defined(DTCONFIG_IMPLIED_DOS)
                                          {"-datadir", ".\\data"},
#else										
#if     defined(DTCONFIG_IMPLIED_MSW)
                                          {"-datadir", ".\\data"},
#else
                                          {"-datadir", "./data"},
#endif
#endif

										/* ................................ */
#if     defined(DTCONFIG_IMPLIED_DOS)
                                          {"-dialog", "text,int10"},
#else
#if     defined(DTCONFIG_IMPLIED_MSW)
                                          {"-dialog", ""},
#else
#if     defined(DTCONFIG_IMPLIED_QNX4)
                                          {"-dialog", "text,qnx"},
#else
#if     defined(DTCONFIG_IMPLIED_UNIXLIKE_AND_BRAVE)
                                          {"-dialog", "text,curses"},
#else
#if     defined(DTCONFIG_IMPLIED_UNIXLIKE)
                                          {"-dialog", "none"},
#endif
#endif
#endif
#endif
#endif
										/* ................................ */
#if     defined(DTCONFIG_IMPLIED_DOS)
                                          {"-keyboard", "dos"},
#else
#if     defined(DTCONFIG_IMPLIED_MSW)
										/* in windows, handle keyboard input
										   with getch/kbhit, and map ff and 
										   e0 sequences to arrow and F keys	*/
#       if DTPRJ_CONSOLE == DT_1
                                          {"-keyboard", "dos,,dos"},
#       else
                                          {"-keyboard", ""},
#       endif
#else
#if     defined(DTCONFIG_IMPLIED_QNX4)
                                          {"-keyboard", "qnxterm"},
#else
#if     defined(DTCONFIG_IMPLIED_UNIXLIKE)
                                          {"-keyboard", "trmios"},
#endif
#endif
#endif
#endif

										/* ................................ */
#if     defined(DTCONFIG_IMPLIED_DOS16)
                                          {"-mem", "xmsif"},
#else
                                          {"-mem", "ram"},
#endif

										/* ................................ */
#if     defined(DTPRJ_FASTVIRT)
                                          {"-screen", "canvas,fastvirt"},
#else
#if     defined(DTPRJ_FASTGRAPH)
                                          {"-screen", "canvas,fastgraph"},
#else
#if     defined(DTPRJ_SVGACC)
                                          {"-screen", "canvas,svgacc"},
#else										
#if     defined(DTCONFIG_IMPLIED_MSW)
                                          {"-screen", "hws,msw"},
#else
#if     defined(DTCONFIG_IMPLIED_QNX4)
                                          {"-screen", "hws,qw4"},
#else
#if     defined(DTPRJ_X)
                                          {"-screen", "hws,x"},
#endif
#endif
#endif
#endif
#endif
#endif

										/* ................................ */
#if     defined(DTCONFIG_IMPLIED_DOS)
                                        {"-win", "dsk,.\\tmp\\dtview"},
#else										
#if     defined(DTCONFIG_IMPLIED_MSW)
                                        {"-win", "wxx,file,bmp,.\\tmp"},
#else										
#if     defined(DTCONFIG_IMPLIED_QNX4)
                                        {"-win", "win,ipc,qnx4," 
                                                 DTPRJ_QNX4_IPC_NAME},
#else										
#if     DTPRJ_SOCKET == DT_1
                                        {"-win", "win,ipc,socket," 
                                                 DTPRJ_SOCKET_HOST ":"
                                                 DTPRJ_SOCKET_PORT},
#endif
#endif
#endif
#endif
										/* ................................ */
#if     defined(DTCONFIG_IMPLIED_DOS)
                                        {"-output", ".\\tmp"},
#else										
#if     defined(DTCONFIG_IMPLIED_MSW)
                                        {"-output", ".\\tmp"},
#else										
                                        {"-output", "./tmp"},
#endif
#endif

										/* ................................ */
#if     defined(DTCONFIG_IMPLIED_MSW)
                                        {"-yield", "normal"},
#else
                                        {"-yield", ""},
#endif
                                        {0, 0}
};






/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
  ! Copyright (C) 1998 Dtack Inc. All Rights Reserved
  ! To use this file, you must have signed a license agreement with Dtack Inc.
  ! Under no circumstances may you redistribute this file.
  ! This software is provided AS IS with no warranty expressed or implied.
  ! Dtack Inc. accepts no liability for use or misuse of this file.
  ! http://www.dtack.com  dtack@dtack.com  telephone +360.417.6700
  ! Dtack Inc, P.O. Box 3061, Port Angeles, WA USA 98362
  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */
