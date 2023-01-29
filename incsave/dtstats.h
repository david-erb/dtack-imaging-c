/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
  ! Copyright (C) 1998 Dtack Inc. All Rights Reserved
  ! To use this file, you must have signed a license agreement with Dtack Inc.
  ! Under no circumstances may you redistribute this file.
  ! This software is provided AS IS with no warranty expressed or implied.
  ! Dtack Inc. accepts no liability for use or misuse of this file.
  ! http://www.dtack.com  dtack@dtack.com  telephone +360.417.6700
  ! Dtack Inc, P.O. Box 3061, Port Angeles, WA USA 98362
  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */




										/* ................................ */
										/* statistics 						*/
#define DTSTATS_NONE				 (0x00000000UL)
#define DTSTATS_ALL					 (0xffffffffUL)
#define DTSTATS_MEAN				 (0x00000001UL)
#define DTSTATS_MEDIAN				 (0x00000002UL)
#define DTSTATS_MODE				 (0x00000004UL)
#define DTSTATS_MAXVAL				 (0x00000008UL)
#define DTSTATS_MINVAL				 (0x00000010UL)
#define DTSTATS_SUM					 (0x00000020UL)
#define DTSTATS_SUMSQ				 (0x00000040UL)
#define DTSTATS_VARIANCE			 (0x00000080UL)
#define DTSTATS_STANDARD_DEVIATION	 (0x00000100UL)
#define DTSTATS_SKEWNESS			 (0x00000200UL)
#define DTSTATS_KURTOSIS			 (0x00000400UL)
#define DTSTATS_XMIN				 (0x00000800UL)
#define DTSTATS_YMIN				 (0x00001000UL)
#define DTSTATS_XMAX				 (0x00002000UL)
#define DTSTATS_YMAX				 (0x00004000UL)
#define DTSTATS_WIDTH				 (0x00008000UL)
#define DTSTATS_HEIGHT				 (0x00010000UL)
#define DTSTATS_AREA				 (0x00020000UL)
#define DTSTATS_XCENTROID			 (0x00040000UL)
#define DTSTATS_YCENTROID			 (0x00080000UL)
#define DTSTATS_CMP					 (0x00100000UL)
#define DTSTATS_NLESS				 (0x00200000UL)
#define DTSTATS_NMORE				 (0x00400000UL)
#define DTSTATS_MINX				 (0x00800000UL)
#define DTSTATS_MINY				 (0x01000000UL)
#define DTSTATS_MAXX				 (0x02000000UL)
#define DTSTATS_MAXY				 (0x04000000UL)

#define DTSTATS_MISSING(S)  			\
  (((S)->want != DTSTATS_ALL) && 		\
   (((S)->have^(S)->want)&(S)->want))


#define DTSTATS_CENTROID \
  (DTSTATS_XCENTROID | \
   DTSTATS_YCENTROID)

#define DTSTATS_SIZE \
  (DTSTATS_WIDTH | \
   DTSTATS_HEIGHT | \
   DTSTATS_AREA)

#define DTSTATS_BOUNDBOX \
  (DTSTATS_XMIN | \
   DTSTATS_YMIN | \
   DTSTATS_XMAX | \
   DTSTATS_YMAX)
	 
typedef unsigned long dtstats_flags_t;

typedef struct {
  dtstats_flags_t want;
  dtstats_flags_t have;
  double mean;
  double median;
  double mode;
  double maxval;
  double minval;
  double sum;
  double sumsq;
  double variance;
  double standard_deviation;
  double skewness;
  double kurtosis;
  double xcentroid;
  double ycentroid;
  unsigned long area;
  unsigned long nless;
  unsigned long nmore;
  int minx, miny;					/* location of minimum value 		*/
  int maxx, maxy;					/* location of maximum value 		*/
  int x0;
  int y0;
  int xmin;						/* bounding box 					*/
  int ymin;
  int xmax;
  int ymax;
  int width;
  int height;
  int sign;
} dtstats_t;





/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
  ! Copyright (C) 1998 Dtack Inc. All Rights Reserved
  ! To use this file, you must have signed a license agreement with Dtack Inc.
  ! Under no circumstances may you redistribute this file.
  ! This software is provided AS IS with no warranty expressed or implied.
  ! Dtack Inc. accepts no liability for use or misuse of this file.
  ! http://www.dtack.com  dtack@dtack.com  telephone +360.417.6700
  ! Dtack Inc, P.O. Box 3061, Port Angeles, WA USA 98362
  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */
