/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
  ! Copyright (C) 1998 Dtack Inc. All Rights Reserved
  ! To use this file, you must have signed a license agreement with Dtack Inc.
  ! Under no circumstances may you redistribute this file.
  ! This software is provided AS IS with no warranty expressed or implied.
  ! Dtack Inc. accepts no liability for use or misuse of this file.
  ! http://www.dtack.com  dtack@dtack.com  telephone +360.417.6700
  ! Dtack Inc, P.O. Box 3061, Port Angeles, WA USA 98362
  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */


typedef enum {
  DTDB_TYPE_STRING = 1,
  DTDB_TYPE_LONG = 2,
  DTDB_TYPE_DOUBLE = 3,
  DTDB_TYPE_IMAGE = 4,
  DTDB_TYPE_UNKNOWN = 0
} dtdb_type_e;

typedef unsigned long dtdb_flags_t;
#define DTDB_FLAG_NOCOPY 0x0001

typedef struct dtdb_cell_t {
  const void *data;
  long l;
} dtdb_cell_t;

typedef struct dtdb_record_t {
  dtdb_cell_t cells[1];
} dtdb_record_t;

typedef struct dtdb_field_t {
  const char *name;
  dtdb_flags_t flags;
  dtdb_type_e type;						/* field datatype 					*/
  long i;								/* index of field in table 			*/
} dtdb_field_t;

typedef struct dtdb_table_t {
  const char *name;
  dtlist_t fields;
  dtlist_t records;
  long nfields;
  long nrecords;
  int fields_initialized;
  int records_initialized;
} dtdb_table_t;

#include <dtack/db.p>





/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
  ! Copyright (C) 1998 Dtack Inc. All Rights Reserved
  ! To use this file, you must have signed a license agreement with Dtack Inc.
  ! Under no circumstances may you redistribute this file.
  ! This software is provided AS IS with no warranty expressed or implied.
  ! Dtack Inc. accepts no liability for use or misuse of this file.
  ! http://www.dtack.com  dtack@dtack.com  telephone +360.417.6700
  ! Dtack Inc, P.O. Box 3061, Port Angeles, WA USA 98362
  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */
