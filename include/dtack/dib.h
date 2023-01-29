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
  19991217 DE changed some long's to 4-byte int's in favor of 64-bit CPU
 *..........................................................................*/




#ifdef DTCONFIG_IMPLIED_SEGMENTED
#  define DTCONFIG_HUGE _huge
#else
#  define DTCONFIG_HUGE 
#endif

typedef
dt_rc_e 
DTCONFIG_API1
dt_io_f(
  dt_ctl_t *ctl,
  void *arg,
  void *data,
  unsigned int size);

										/* ................................ */
typedef struct {		
	dtntype_b32_t     biSize;
	dtntype_b32_t     biWidth;
	dtntype_b32_t     biHeight;
	unsigned short    biPlanes;
	unsigned short    biBitCount;
	dtntype_b32_t     biCompression;
	dtntype_b32_t     biSizeImage;
	dtntype_b32_t     biXPelsPerMeter;
	dtntype_b32_t     biYPelsPerMeter;
	dtntype_b32_t     biClrUsed;
	dtntype_b32_t     biClrImportant;
} dtdib_t;

typedef struct {						/* pallette comes after dib 		*/
    unsigned char rgbBlue;
    unsigned char rgbGreen;
    unsigned char rgbRed;
    unsigned char rgbReserved;
} dtdib_rgb_t;

#define DTDIB_PADMAX (3)
#define DTDIB_PAD(S) ((4-((S)&3))&3)
#define DTDIB_PADDED(S) ((S)+DTDIB_PAD(S))

extern dtntype_b32_t dtdib_longword;

#include <dtack/dib.p>





/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
  ! Copyright (C) 1998 Dtack Inc. All Rights Reserved
  ! To use this file, you must have signed a license agreement with Dtack Inc.
  ! Under no circumstances may you redistribute this file.
  ! This software is provided AS IS with no warranty expressed or implied.
  ! Dtack Inc. accepts no liability for use or misuse of this file.
  ! http://www.dtack.com  dtack@dtack.com  telephone +360.417.6700
  ! Dtack Inc, P.O. Box 3061, Port Angeles, WA USA 98362
  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */
