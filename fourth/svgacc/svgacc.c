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
| NAME
| Introduction - intro
|
| DESCRIPTION
| As yet undocumented.
| 
| RETURN VALUES
| All functions Return DTOS_RANWELL on success.  
| Any other return value indicates failure.
| 
| ERRORS
| As yet undocumented.
| 
| END
 *..........................................................................*/

#include <dtack/config.h>

#if DTPRJ_SVGACC_FAKE == DT_1
#  include <dtack/nofar.h>
#  ifndef DTDEPEND
#    include <svgacc.h>
#  endif

#  define EMPTY_INT {return 0;}
#  define EMPTY_VOID {}

int  blkget (int x1, int y1, int x2, int y2, RasterBlock *gfxblk) EMPTY_INT
void blkput (PixelMode mode, int x, int y, RasterBlock *gfxblk)
  EMPTY_VOID

void bytecopy (void *src, void *dst, unsigned long numbytes) EMPTY_VOID

void drwbox (PixelMode mode, int colr, int x1, int y1, int x2, int y2)
  EMPTY_VOID
void drwline (PixelMode mode, int colr, int x1, int y1, int x2, int y2)
  EMPTY_VOID

void fillpage (int colr) EMPTY_VOID
void fillscreen (int colr) EMPTY_VOID
void fillview (int colr) EMPTY_VOID

void mousebutpress (int reqbut, int *xloc, int *yloc, int *num, int *mbuts)
  EMPTY_VOID
void mousebutrelease (int reqbut, int *xloc, int *yloc, int *num, int *mbuts)
  EMPTY_VOID
void mousecursordefault (void) EMPTY_VOID
void mousecursorset (MouseCursor *mousecursor) EMPTY_VOID
void mouseenter (void) EMPTY_VOID
void mouseexit (void) EMPTY_VOID
void mousehide (void) EMPTY_VOID
void mouseinfo (int *drvmajorver, int *drvminorver, int *mousetype, int *irqnumber) EMPTY_VOID
void mouselocset (int xloc, int yloc) EMPTY_VOID
void mouserangeset (int x1, int y1, int x2, int y2) EMPTY_VOID
void mouserestorestate (byte *mousebuf) EMPTY_VOID
void mousesavestate (byte *mousebuf) EMPTY_VOID
void mousesensset (int xsens, int ysens, int dblspdthresh) EMPTY_VOID
void mouseshow (void) EMPTY_VOID
void mousestatus (int *x, int *y, int *mbuts) EMPTY_VOID
int  mousestoragesize (void) EMPTY_INT

void palget (RGB *pal, int firstcolr, int lastcolr) EMPTY_VOID
void palset (RGB *pal, int firstcolr, int lastcolr) EMPTY_VOID

int  res320 (void) EMPTY_INT
int  res640 (void) EMPTY_INT
int  res640l (void) EMPTY_INT
int  res800 (void) EMPTY_INT
int  res1024 (void) EMPTY_INT
int  res1280 (void) EMPTY_INT
int  restext (void) EMPTY_INT

void setcard (VGAChipset chip, int mem) EMPTY_VOID

int  videomodeget (void) EMPTY_INT
void videomodeset (int videomode) EMPTY_VOID

int  whichcpu (void) EMPTY_INT
int  whichjoystick (void) EMPTY_INT
int  whichmem (void) EMPTY_INT
int  whichmouse (void) EMPTY_INT
VGAChipset whichvga (void) EMPTY_INT
int  whichxms (unsigned int *xmskbytesavail, unsigned int *xmshandlesavail) EMPTY_INT

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
