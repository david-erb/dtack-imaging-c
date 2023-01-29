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

typedef struct {
  dtntype_s4_t size;
  dtxy_t rows1;
  dtxy_t rows2;
  int ncolors;
  int ndibs;
  int recordsize;
} dtwmf_split_t;

										/* ................................ */
typedef struct { 
  dtntype_b32_t rdSize;
  unsigned short rdFunction;
  unsigned short rdParm[1];
} dtwmf_record_t;

										/* ................................ */
#define DTWMF_PLACEABLE_MAGIC1 (0xcdd7)
#define DTWMF_PLACEABLE_MAGIC2 (0x9ac6)

typedef struct {
  unsigned short magic1;
  unsigned short magic2;
  unsigned short hmf;
  struct {
    short left;
    short top;
    short right;
    short bottom;
  } bbox;
  unsigned short inch; 
  unsigned short reserved1;
  unsigned short reserved2;
  unsigned short checksum;
} dtwmf_placeable_t;

										/* ................................ */
typedef struct {
  unsigned short  mtType;
  unsigned short  mtHeaderSize;
  unsigned short  mtVersion;
  unsigned short  mtSize[2];
  unsigned short  mtNoObjects;
  unsigned short  mtMaxRecord[2];
  unsigned short  mtNoParameters;
} dtwmf_fileheader_t;

typedef struct {
  dtwmf_fileheader_t fileheader;
										/* +18 = 40 						*/
  struct { 
    unsigned short rdSize[2];
    unsigned short rdFunction;
  } savedc;
										/* +6 = 46 							*/
  struct { 
    unsigned short rdSize[2];
    unsigned short rdFunction;
    unsigned short rdParm[2];
  } setwindoworg;
										/* +10 = 56							*/
  struct { 
    unsigned short rdSize[2];
    unsigned short rdFunction;
    unsigned short rdParm[2];
  } setwindowext;
										/* +10 = 66 						*/
  struct { 
    unsigned short rdSize[2];
    unsigned short rdFunction;
    unsigned short rdParm[1];
  } setstretchbltmode;
										/* +8 = 74							*/
  struct { 
    unsigned short rdSize[2];
    unsigned short rdFunction;
    unsigned short rdParm[4];
  } intersectcliprect;

  struct {
    unsigned short rdSize[2];
    unsigned short rdFunction;
    unsigned short rdParm[1];
  } setpolyfillmode;

  struct {
    unsigned short rdSize[2];
    unsigned short rdFunction;
    unsigned short rdParm[1];
  } setbkmode2;
} dtwmf_metahead_t;

typedef struct {						/* stretch record 					*/
  unsigned short rdSize[2];
  unsigned short rdFunction;
  unsigned short rdParm[11];
} dtwmf_stretchdib_t;

typedef struct {	
  struct {
    unsigned short rdSize[2];
    unsigned short rdFunction;
    unsigned short rdParm[1];
  } setbkmode1;
				
  struct {
    unsigned short rdSize[2];
    unsigned short rdFunction;
    unsigned short rdParm[1];
  } restoredc;
				
  struct {								/* termination record after all 	*/
    unsigned short rdSize[2];
    unsigned short rdFunction;
  } terminate;
				
} dtwmf_metatail_t;

#define DTWMF_SETBKCOLOR              0x0201
#define DTWMF_SETBKMODE               0x0102
#define DTWMF_SETMAPMODE              0x0103
#define DTWMF_SETROP2                 0x0104
#define DTWMF_SETRELABS               0x0105
#define DTWMF_SETPOLYFILLMODE         0x0106
#define DTWMF_SETSTRETCHBLTMODE       0x0107
#define DTWMF_SETTEXTCHAREXTRA        0x0108
#define DTWMF_SETTEXTCOLOR            0x0209
#define DTWMF_SETTEXTJUSTIFICATION    0x020A
#define DTWMF_SETWINDOWORG            0x020B
#define DTWMF_SETWINDOWEXT            0x020C
#define DTWMF_SETVIEWPORTORG          0x020D
#define DTWMF_SETVIEWPORTEXT          0x020E
#define DTWMF_OFFSETWINDOWORG         0x020F
#define DTWMF_SCALEWINDOWEXT          0x0410
#define DTWMF_OFFSETVIEWPORTORG       0x0211
#define DTWMF_SCALEVIEWPORTEXT        0x0412
#define DTWMF_LINETO                  0x0213
#define DTWMF_MOVETO                  0x0214
#define DTWMF_EXCLUDECLIPRECT         0x0415
#define DTWMF_INTERSECTCLIPRECT       0x0416
#define DTWMF_ARC                     0x0817
#define DTWMF_ELLIPSE                 0x0418
#define DTWMF_FLOODFILL               0x0419
#define DTWMF_PIE                     0x081A
#define DTWMF_RECTANGLE               0x041B
#define DTWMF_ROUNDRECT               0x061C
#define DTWMF_PATBLT                  0x061D
#define DTWMF_SAVEDC                  0x001E
#define DTWMF_SETPIXEL                0x041F
#define DTWMF_OFFSETCLIPRGN           0x0220
#define DTWMF_TEXTOUT                 0x0521
#define DTWMF_BITBLT                  0x0922
#define DTWMF_STRETCHBLT              0x0B23
#define DTWMF_POLYGON                 0x0324
#define DTWMF_POLYLINE                0x0325
#define DTWMF_ESCAPE                  0x0626
#define DTWMF_RESTOREDC               0x0127
#define DTWMF_FILLREGION              0x0228
#define DTWMF_FRAMEREGION             0x0429
#define DTWMF_INVERTREGION            0x012A
#define DTWMF_PAINTREGION             0x012B
#define DTWMF_SELECTCLIPREGION        0x012C
#define DTWMF_SELECTOBJECT            0x012D
#define DTWMF_SETTEXTALIGN            0x012E
#define DTWMF_DRAWTEXT                0x062F

#define DTWMF_CHORD                   0x0830
#define DTWMF_SETMAPPERFLAGS          0x0231
#define DTWMF_EXTTEXTOUT              0x0a32
#define DTWMF_SETDIBTODEV             0x0d33
#define DTWMF_SELECTPALETTE           0x0234
#define DTWMF_REALIZEPALETTE          0x0035
#define DTWMF_ANIMATEPALETTE          0x0436
#define DTWMF_SETPALENTRIES           0x0037
#define DTWMF_POLYPOLYGON             0x0538
#define DTWMF_RESIZEPALETTE           0x0139

#define DTWMF_DIBBITBLT               0x0940
#define DTWMF_DIBSTRETCHBLT           0x0b41
#define DTWMF_DIBCREATEPATTERNBRUSH   0x0142
#define DTWMF_STRETCHDIB              0x0f43

#define DTWMF_EXTFLOODFILL            0x0548

#define DTWMF_RESETDC                 0x014C
#define DTWMF_STARTDOC                0x014D
#define DTWMF_STARTPAGE               0x004F
#define DTWMF_ENDPAGE                 0x0050
#define DTWMF_ABORTDOC                0x0052
#define DTWMF_ENDDOC                  0x005E

#define DTWMF_DELETEOBJECT            0x01f0

#define DTWMF_CREATEPALETTE           0x00f7
#define DTWMF_CREATEBRUSH             0x00F8
#define DTWMF_CREATEPATTERNBRUSH      0x01F9
#define DTWMF_CREATEPENINDIRECT       0x02FA
#define DTWMF_CREATEFONTINDIRECT      0x02FB
#define DTWMF_CREATEBRUSHINDIRECT     0x02FC
#define DTWMF_CREATEBITMAPINDIRECT    0x02FD
#define DTWMF_CREATEBITMAP            0x06FE
#define DTWMF_CREATEREGION            0x06FF
#define DTWMF_TERMINATE               0

#define DTWMF_COLORONCOLOR    (3)
#define DTWMF_SRCCOPY         (0x00CC0020L)

#include <dtack/wmf.p>





/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
  ! Copyright (C) 1998 Dtack Inc. All Rights Reserved
  ! To use this file, you must have signed a license agreement with Dtack Inc.
  ! Under no circumstances may you redistribute this file.
  ! This software is provided AS IS with no warranty expressed or implied.
  ! Dtack Inc. accepts no liability for use or misuse of this file.
  ! http://www.dtack.com  dtack@dtack.com  telephone +360.417.6700
  ! Dtack Inc, P.O. Box 3061, Port Angeles, WA USA 98362
  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */
