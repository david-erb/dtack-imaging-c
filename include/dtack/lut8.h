/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
  ! Copyright (C) 1998 Dtack Inc. All Rights Reserved
  ! To use this file, you must have signed a license agreement with Dtack Inc.
  ! Under no circumstances may you redistribute this file.
  ! This software is provided AS IS with no warranty expressed or implied.
  ! Dtack Inc. accepts no liability for use or misuse of this file.
  ! http://www.dtack.com  dtack@dtack.com  telephone +360.417.6700
  ! Dtack Inc, P.O. Box 3061, Port Angeles, WA USA 98362
  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */




#define DTLUT8_NAME_MAXLEN (32)

#define DTLUT8_NSTANDARD (9)

#define DTLUT8_CMP(A, B) \
((A).r!=(B).r? ((A).r-(B).r) : ((A).g!=(B).g? ((A).g-(B).g) : ((A).b-(B).b)))

typedef dt_ntype_b08_t dtlut8_data_t;

typedef struct {
  dtlut8_data_t r, g, b, i;
} dtlut8_t;

#define DTLUT8_TRANSPARENT   (0)
#define DTLUT8_BLACK         (1)
#define DTLUT8_BLUE          (2)
#define DTLUT8_RED           (3)
#define DTLUT8_MAGENTA       (4)
#define DTLUT8_GREEN         (5)
#define DTLUT8_CYAN          (6)
#define DTLUT8_YELLOW        (7)
#define DTLUT8_WHITE         (255)

extern int *dtlut8_standard_index;

extern const char ** const dtlut8_lutnames;
extern const dtlut8_data_t ** const dtlut8_luts;

#include <dtack/lut8.p>





/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
  ! Copyright (C) 1998 Dtack Inc. All Rights Reserved
  ! To use this file, you must have signed a license agreement with Dtack Inc.
  ! Under no circumstances may you redistribute this file.
  ! This software is provided AS IS with no warranty expressed or implied.
  ! Dtack Inc. accepts no liability for use or misuse of this file.
  ! http://www.dtack.com  dtack@dtack.com  telephone +360.417.6700
  ! Dtack Inc, P.O. Box 3061, Port Angeles, WA USA 98362
  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */
