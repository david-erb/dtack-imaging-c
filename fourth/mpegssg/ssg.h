/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
  ! Copyright (C) 1998 Dtack Inc. All Rights Reserved
  ! To use this file, you must have signed a license agreement with Dtack Inc.
  ! Under no circumstances may you redistribute this file.
  ! This software is provided AS IS with no warranty expressed or implied.
  ! Dtack Inc. accepts no liability for use or misuse of this file.
  ! http://www.dtack.com  dtack@dtack.com  telephone +360.417.6700
  ! Dtack Inc, P.O. Box 3061, Port Angeles, WA USA 98362
  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */





#ifndef DTDEPEND
#include <setjmp.h>
#endif

extern
dt_rc_e
DTCONFIG_API1
ssg_main(
  dt_ctl_t *ctl,
  umdec_t *mdec,
  int keep_picture_coding,
  const char *output_type);
										/* ................................ */
										/* stuff after this is internal 	*/
#ifdef GLOBAL 
#define SSG_GLOBAL 
dt_ctl_t *ssg_ctl = NULL;
umdec_t *ssg_umdec = NULL;
#else
#define SSG_GLOBAL extern
extern dt_ctl_t *ssg_ctl;
extern umdec_t *ssg_umdec;
#endif

SSG_GLOBAL jmp_buf ssg_main_jump;
SSG_GLOBAL int ssg_keep_picture_coding;
SSG_GLOBAL int ssg_need_lep;
SSG_GLOBAL long ssg_read_total;
SSG_GLOBAL int ssg_total_i;
SSG_GLOBAL int ssg_total_p;
SSG_GLOBAL int ssg_total_b;
SSG_GLOBAL int ssg_total_d;
SSG_GLOBAL int ssg_skipped_i;
SSG_GLOBAL int ssg_skipped_p;
SSG_GLOBAL int ssg_skipped_b;
SSG_GLOBAL int ssg_skipped_d;

#define SSG_MAIN_JUMP_SSG_READ (1)
#define SSG_MAIN_JUMP_ISO_END  (2)
#define SSG_MAIN_JUMP_SEQUENCE_END  (3)

SSG_GLOBAL void **ssg_src;
SSG_GLOBAL int ssg_xe;
SSG_GLOBAL int ssg_ye;
SSG_GLOBAL int ssg_frametype;

#define SSG_FRAMETYPE_YCBCR_420 1
#define SSG_FRAMETYPE_LUT       2
#define SSG_FRAMETYPE_PSEUDO    3

#define SSG_OUTPUT_RAW "raw"
#define SSG_OUTPUT_DITHER "dither"





/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
  ! Copyright (C) 1998 Dtack Inc. All Rights Reserved
  ! To use this file, you must have signed a license agreement with Dtack Inc.
  ! Under no circumstances may you redistribute this file.
  ! This software is provided AS IS with no warranty expressed or implied.
  ! Dtack Inc. accepts no liability for use or misuse of this file.
  ! http://www.dtack.com  dtack@dtack.com  telephone +360.417.6700
  ! Dtack Inc, P.O. Box 3061, Port Angeles, WA USA 98362
  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */
