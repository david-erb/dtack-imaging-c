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
#include <dtack/str.h>
#include <dtack/main.h>

#define DTTOOL_NAME_MAXLEN (128)		/* length of name input field 		*/
#define DTTOOL_NUMBER_MAXLEN (32)		/* length of numeric input field	*/

#define DTTOOL_PARSE_FLAG_NEED   (0x00000001UL)
#define DTTOOL_PARSE_FLAG_STRING (0x00000002UL)
#define DTTOOL_PARSE_FLAG_IMPORT (0x00000004UL)
#define DTTOOL_PARSE_FLAG_EXPORT (0x00000008UL)
#define DTTOOL_PARSE_FLAG_LONG   (0x00000010UL)

#define DTTOOL_PARSE_FLAG_NEEDSTRING \
 (DTTOOL_PARSE_FLAG_NEED | DTTOOL_PARSE_FLAG_STRING)
#define DTTOOL_PARSE_FLAG_NEEDIMPORT \
 (DTTOOL_PARSE_FLAG_NEED | DTTOOL_PARSE_FLAG_IMPORT)
#define DTTOOL_PARSE_FLAG_NEEDEXPORT \
 (DTTOOL_PARSE_FLAG_NEED | DTTOOL_PARSE_FLAG_EXPORT)
#define DTTOOL_PARSE_FLAG_NEEDLONG \
 (DTTOOL_PARSE_FLAG_NEED | DTTOOL_PARSE_FLAG_LONG)

#define DTTOOL_MAIN(PROG)                                                    \
  DT_F(# PROG);                                                              \
  DTMAIN2(PROG)

#include <dttool.p>





/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
  ! Copyright (C) 1998 Dtack Inc. All Rights Reserved
  ! To use this file, you must have signed a license agreement with Dtack Inc.
  ! Under no circumstances may you redistribute this file.
  ! This software is provided AS IS with no warranty expressed or implied.
  ! Dtack Inc. accepts no liability for use or misuse of this file.
  ! http://www.dtack.com  dtack@dtack.com  telephone +360.417.6700
  ! Dtack Inc, P.O. Box 3061, Port Angeles, WA USA 98362
  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */
