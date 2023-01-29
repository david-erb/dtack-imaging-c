/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
  ! Copyright (C) 1998 Dtack Inc. All Rights Reserved
  ! To use this file, you must have signed a license agreement with Dtack Inc.
  ! Under no circumstances may you redistribute this file.
  ! This software is provided AS IS with no warranty expressed or implied.
  ! Dtack Inc. accepts no liability for use or misuse of this file.
  ! http://www.dtack.com  dtack@dtack.com  telephone +360.417.6700
  ! Dtack Inc, P.O. Box 3061, Port Angeles, WA USA 98362
  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */




/* ----------------------------- MNI Header -----------------------------------
@NAME       : mpeg.h
@INPUT      : 
@OUTPUT     : 
@RETURNS    : 
@DESCRIPTION: Types and function prototypes needed for applications to
              use the Berkely MPEG decoding engine via the MNI front
              end.
@METHOD     : 
@GLOBALS    : Types defined:
                 ImageDesc  - structure giving height, width, etc.
                 DitherEnum - the different dither types supported by
                              the decoding engine
@CALLS      : 
@CREATED    : Greg Ward, 94/6/16.
@MODIFIED   : Greg Ward, 94/9/12 (based on John Cristy's fixes): made
              more amenable to use with other libraries that also
	      happen to define TRUE, FALSE, [Bb]oolean, and added
	      PROTO macro
---------------------------------------------------------------------------- */

#ifndef __MPEG_H
#define __MPEG_H

/* An attempt at a portable and integrable boolean type... */

#if (!defined(TRUE) || !defined(FALSE))
# define TRUE 1
# define FALSE 0
#endif

#if (!defined (BOOLEAN_TYPE_EXISTS))
typedef unsigned int Boolean;
#endif

typedef struct
{
   int	Height,			/* in pixels */
        Width,
        Depth,			/* image depth (bits) */
        PixelSize,              /* bits actually stored per pixel */
        Size,			/* bytes for whole image */
        BitmapPad;              /* "quantum" of a scanline -- each scanline */
				/* starts on an even interval of this */
                                /* many bits */
} ImageDesc;

typedef enum 
{
   HYBRID_DITHER,
   HYBRID2_DITHER,
   FS4_DITHER,
   FS2_DITHER,
   FS2FAST_DITHER,
   Twox2_DITHER,
   GRAY_DITHER,
   FULL_COLOR_DITHER,
   NO_DITHER,
   ORDERED_DITHER,
   MONO_DITHER,
   MONO_THRESHOLD,
   ORDERED2_DITHER,
   MBORDERED_DITHER
} DitherEnum;


typedef enum
{
   MPEG_DITHER,
   MPEG_QUIET,
   MPEG_LUM_RANGE,
   MPEG_CR_RANGE,
   MPEG_CB_RANGE
} MPEGOptionEnum;

/* Kludge so we can compile under ANSI or K&R */

#undef PROTO
#if __STDC__
#define PROTO(formal_parameters) formal_parameters
#else
#define const
#define PROTO(formal_parameters) ()
#endif

/* Function prototypes (all are defined in wrapper.c) */

extern Boolean OpenMPEG PROTO((FILE *MPEGfile, ImageDesc *ImgInfo));
void   CloseMPEG PROTO((void));
void   RewindMPEG PROTO((FILE *MPEGfile, ImageDesc *Image));
void   SetMPEGOption PROTO((MPEGOptionEnum Option, int value));
extern Boolean GetMPEGFrame PROTO((char *Frame));

#endif   /* __MPEG_H */





/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
  ! Copyright (C) 1998 Dtack Inc. All Rights Reserved
  ! To use this file, you must have signed a license agreement with Dtack Inc.
  ! Under no circumstances may you redistribute this file.
  ! This software is provided AS IS with no warranty expressed or implied.
  ! Dtack Inc. accepts no liability for use or misuse of this file.
  ! http://www.dtack.com  dtack@dtack.com  telephone +360.417.6700
  ! Dtack Inc, P.O. Box 3061, Port Angeles, WA USA 98362
  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */
