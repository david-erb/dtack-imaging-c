/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
  ! Copyright (C) 1998 Dtack Inc. All Rights Reserved
  ! To use this file, you must have signed a license agreement with Dtack Inc.
  ! Under no circumstances may you redistribute this file.
  ! This software is provided AS IS with no warranty expressed or implied.
  ! Dtack Inc. accepts no liability for use or misuse of this file.
  ! http://www.dtack.com  dtack@dtack.com  telephone +360.417.6700
  ! Dtack Inc, P.O. Box 3061, Port Angeles, WA USA 98362
  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */




#ifdef __cplusplus
extern "C" {
#endif

extern int 
DTCONFIG_API1
xv310a_slow_quant  
  (unsigned char* rgb888, 
   int w,
   int h, 
   unsigned char *pseudo, 
   unsigned char *rlut,
   unsigned char *glut,
   unsigned char *blut,
   int colors_wanted);

extern int 
DTCONFIG_API1
xv310a_slow_quant3  
  (unsigned char* c0,
   unsigned char* c1,
   unsigned char* c2,
   int w,
   int h, 
   unsigned char *pseudo, 
   unsigned char *rlut,
   unsigned char *glut,
   unsigned char *blut,
   int colors_wanted);

extern int 
DTCONFIG_API1
xv310a_slow_map3  
  (unsigned char* c0,
   unsigned char* c1,
   unsigned char* c2,
   int w,
   int h, 
   unsigned char *pseudo, 
   unsigned char *rlut,
   unsigned char *glut,
   unsigned char *blut,
   int colors_wanted);

#ifdef __cplusplus
}
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
