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
#include <dtack/os.h>
#include <dtack/str.h>

#undef DTPRJ_CONSOLE
#define DTPRJ_CONSOLE DT_0				/* make this a non-console app 		*/

#include <dtack/main.h>
#include <dtack/yield.h>

#define DTT_TESTDATA_DIR "./testdata"	/* if no environment variable		*/

										/* ................................ */
#ifndef DTT_MAIN
#define DTT_MAIN(NAME) DTMAIN1(NAME)
#endif

#ifndef DTT_TEST
#define DTT_TEST(NAME) DTMAIN2(NAME)
#endif
										/* ................................ */
#define DTT_IFCALL(NAME)													 \
  if (!strcmp(argv[0], #NAME))												 \
  {																			 \
    extern dtmain_f dtt ## NAME ## _cmd;									 \
    DT_G(dtt ## NAME ## _cmd,(  											 \
      argc, argv, param));	    											 \
  }																			 \
  else

typedef
dt_rc_e
DTCONFIG_API1
dtt_image_f(
  dt_ctl_t *ctl,
  dtimage_t *image,
  long nblobs,
  void *arg,
  const char *desc,
  long loops);

typedef 
dt_rc_e 
DTCONFIG_API1 
dtt_image_gen_f(
  dt_ctl_t *, 
  dtimage_t *);

										/* ................................ */

#define DTT_YIELD_AT_UNIT (DT_YIELD_FLAG_NORMAL|DT_YIELD_FLAG_LOW)
#define DTT_YIELD_AT_REPS (DT_YIELD_FLAG_NORMAL|DT_YIELD_FLAG_LOW)

#define DTT_YIELD_REPS(CHECK, CTL, PARAM, REPS)								  \
if (DTYIELD_WILL((CTL), DTT_YIELD_AT_REPS))									  \
{																			  \
  char s[32];																  \
  dtstr_printf(ctl, 														  \
    s, sizeof(s),															  \
    "beginning rep %ld of %ld",												  \
    (REPS)+1, (PARAM)->reps);												  \
  DTYIELD_DOIT(CHECK, ctl, s);												  \
}

										/* ................................ */
extern const char **dtt_blob_list;

										/* ................................ */
#define DTT_CHECK_FLAG_NEED   (0x00000001UL)
#define DTT_CHECK_FLAG_STRING (0x00000002UL)
#define DTT_CHECK_FLAG_IMPORT (0x00000004UL)
#define DTT_CHECK_FLAG_EXPORT (0x00000008UL)

#define DTT_CHECK_FLAG_NEEDSTRING \
 (DTT_CHECK_FLAG_NEED | DTT_CHECK_FLAG_STRING)
#define DTT_CHECK_FLAG_NEEDIMPORT \
 (DTT_CHECK_FLAG_NEED | DTT_CHECK_FLAG_IMPORT)
#define DTT_CHECK_FLAG_NEEDEXPORT \
 (DTT_CHECK_FLAG_NEED | DTT_CHECK_FLAG_EXPORT)

#include <dtack/t.p>





/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
  ! Copyright (C) 1998 Dtack Inc. All Rights Reserved
  ! To use this file, you must have signed a license agreement with Dtack Inc.
  ! Under no circumstances may you redistribute this file.
  ! This software is provided AS IS with no warranty expressed or implied.
  ! Dtack Inc. accepts no liability for use or misuse of this file.
  ! http://www.dtack.com  dtack@dtack.com  telephone +360.417.6700
  ! Dtack Inc, P.O. Box 3061, Port Angeles, WA USA 98362
  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */
