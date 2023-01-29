/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
  ! Copyright (C) 1998 Dtack Inc. All Rights Reserved
  ! To use this file, you must have signed a license agreement with Dtack Inc.
  ! Under no circumstances may you redistribute this file.
  ! This software is provided AS IS with no warranty expressed or implied.
  ! Dtack Inc. accepts no liability for use or misuse of this file.
  ! http://www.dtack.com  dtack@dtack.com  telephone +360.417.6700
  ! Dtack Inc, P.O. Box 3061, Port Angeles, WA USA 98362
  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */




#include <dtack/base.h>

#define DT_DBG_MASK_CONFIG (0x02000000L) /* config							*/

DT_RCSID("source/low $RCSfile: dbg_dat.c,v $ $Revision: 1.1 $");

static const char *mask_names[] = {
  "any",
  "all",
  "stats",
  "img",
  "time",
  "complex",
  "ipc",
  "arg",
  "assert",
  "memtrace",
  "async",
  "bigio",
  "page",
  "win",
  "alimits",
  "object",
  "intermd",
  "pixel",
  "io",
  "memstats",
  "alt",
  "trace",
  "usr1",
  "usr2",
  "cstack",
  "memx",
  "lut",
  "command",
  "reply",
  "schedule",
  "config",
  NULL
};

static const char *mask_descs[] = {
  "all debug masks",
  "any debug mask",
  "statistics",
  "image operations",
  "execution times",
  "execution complexity indicators",
  "interprocess communication",
  "arguments",
  "assertions",
  "any memory allocation/freeing",
  "asynchronous events",
  "big blocks of I/O only",
  "memory paging",
  "windowing operations",
  "print limits",
  "object values",
  "intermediate values",
  "pixel values",
  "all I/O",
  "memory summary",
  "program-chosen alternative",
  "execution trace",
  "user's use",
  "user's use",
  "compiler stack monitoring",
  "extended memory operations",
  "lut operations",
  "commands to servers",
  "replies to commands",
  "scheduled operations",
  "config verification",
  NULL
};

static const long mask_values[] = {
  DT_DBG_MASK_ANY,						/* value of mask to print anything	*/
  DT_DBG_MASK_ALL,						/* all debug masks					*/
  DT_DBG_MASK_STATS,					/* statistics						*/
  DT_DBG_MASK_IMG,						/* image operations					*/
  DT_DBG_MASK_TIME,						/* execution times					*/
  DT_DBG_MASK_COMPLEX,					/* execution complexity indicators	*/
  DT_DBG_MASK_IPC,						/* interprocess communication		*/
  DT_DBG_MASK_ARG,						/* arguments						*/
  DT_DBG_MASK_ASSERT,					/* assertions						*/
  DT_DBG_MASK_MEMTRACE,					/* any memory allocation/freeing   	*/
  DT_DBG_MASK_ASYNC,					/* asynchronous events				*/
  DT_DBG_MASK_BIGIO,					/* big blocks of I/O only			*/
  DT_DBG_MASK_PAGE,						/* memory paging					*/
  DT_DBG_MASK_WIN,						/* windowing operations				*/
  DT_DBG_MASK_ALIMITS,					/* print limits						*/
  DT_DBG_MASK_OBJECT,					/* object values					*/
  DT_DBG_MASK_INTERMD,					/* intermediate values				*/
  DT_DBG_MASK_PIXEL,					/* pixel values						*/
  DT_DBG_MASK_IO,						/* all I/O							*/
  DT_DBG_MASK_MEMSTATS,					/* memory summary					*/
  DT_DBG_MASK_ALT,	 					/* program-chosen alternative		*/
  DT_DBG_MASK_TRACE,					/* execution trace					*/
  DT_DBG_MASK_USR1,						/* user's use						*/
  DT_DBG_MASK_USR2,						/* user's use						*/
  DT_DBG_MASK_CSTACK,					/* c compiler stack					*/
  DT_DBG_MASK_MEMX,						/* extended memory operations		*/
  DT_DBG_MASK_LUT,						/* lut operations					*/
  DT_DBG_MASK_COMMAND,					/* commands to servers				*/
  DT_DBG_MASK_REPLY,					/* replies to commands				*/
  DT_DBG_MASK_SCHEDULE,					/* scheduled operations				*/
  DT_DBG_MASK_CONFIG,					/* config verification				*/
  0
};

const long *dt_dbg_mask_values = mask_values;
const char **dt_dbg_mask_names = mask_names;
const char **dt_dbg_mask_descs = mask_descs;









/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
  ! Copyright (C) 1998 Dtack Inc. All Rights Reserved
  ! To use this file, you must have signed a license agreement with Dtack Inc.
  ! Under no circumstances may you redistribute this file.
  ! This software is provided AS IS with no warranty expressed or implied.
  ! Dtack Inc. accepts no liability for use or misuse of this file.
  ! http://www.dtack.com  dtack@dtack.com  telephone +360.417.6700
  ! Dtack Inc, P.O. Box 3061, Port Angeles, WA USA 98362
  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */
