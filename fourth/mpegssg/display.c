/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
  ! Copyright (C) 1998 Dtack Inc. All Rights Reserved
  ! To use this file, you must have signed a license agreement with Dtack Inc.
  ! Under no circumstances may you redistribute this file.
  ! This software is provided AS IS with no warranty expressed or implied.
  ! Dtack Inc. accepts no liability for use or misuse of this file.
  ! http://www.dtack.com  dtack@dtack.com  telephone +360.417.6700
  ! Dtack Inc, P.O. Box 3061, Port Angeles, WA USA 98362
  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */




/* display.c, X11 interface                                                 */

/* Copyright (C) 1996, MPEG Software Simulation Group. All Rights Reserved. */

/*
 * Disclaimer of Warranty
 *
 * These software programs are available to the user without any license fee or
 * royalty on an "as is" basis.  The MPEG Software Simulation Group disclaims
 * any and all warranties, whether express, implied, or statuary, including any
 * implied warranties or merchantability or of fitness for a particular
 * purpose.  In no event shall the copyright-holder be liable for any
 * incidental, punitive, or consequential damages of any kind whatsoever
 * arising from the use of these programs.
 *
 * This disclaimer of warranty extends to the user of these programs and user's
 * customers, employees, agents, transferees, successors, and assigns.
 *
 * The MPEG Software Simulation Group does not represent or warrant that the
 * programs furnished hereunder are free of infringement of any third-party
 * patents.
 *
 * Commercial implementations of MPEG-1 and MPEG-2 video, including shareware,
 * are subject to royalty fees to patent holders.  Many of these patents are
 * general enough such that they are unavoidable regardless of implementation
 * design.
 *
 */

 /* the Xlib interface is closely modeled after
  * mpeg_play 2.0 by the Berkeley Plateau Research Group
  */


#include <dtack/base.h>
DT_RCSID("fourth/mpegssg $RCSfile: display.c,v $ $Revision: 1.7 $");
#include <umdec.h>
#include <dtack/lut8.h>
#include <dtack/os.h>

#include <ssg.h>

#include <config.h>
#include <global.h>

static unsigned char Pixel[256];

/* private prototypes */
static int Display_Image _ANSI_ARGS_((unsigned char *Dithered_Image));
static void Dither_Frame _ANSI_ARGS_((unsigned char *src[]));
static void Dither_Top_Field _ANSI_ARGS_((unsigned char *src[], unsigned char *dst));
static void Dither_Bottom_Field _ANSI_ARGS_((unsigned char *src[], unsigned char *dst));
static void Dither_Top_Field420 _ANSI_ARGS_((unsigned char *src[],
                                      unsigned char *dst));
static void Dither_Bottom_Field420 _ANSI_ARGS_((unsigned char *src[],
                                      unsigned char *dst));

/* local data */
static unsigned char *Dithered_Image, *Dithered_Image2;

static unsigned char Y_Table[256+16];
static unsigned char Cb_Table[128+16];
static unsigned char Cr_Table[128+16];

/* connect to server, create and map window,
 * allocate colors and (shared) memory
 */

int
Initialize_Display_Process(
  char *name)
{
  DT_F("ssg_Initialize_Display_Process");
  dt_ctl_t *ctl = ssg_ctl;
  dt_rc_e rc;
  int crv, cbu, cgu, cgv;
  int Y, Cb, Cr;
  int i;
  dtlut8_t lut[240-16];

  /* matrix coefficients */
  crv = Inverse_Table_6_9[matrix_coefficients][0];
  cbu = Inverse_Table_6_9[matrix_coefficients][1];
  cgu = Inverse_Table_6_9[matrix_coefficients][2];
  cgv = Inverse_Table_6_9[matrix_coefficients][3];

  /* allocate colors */

  /* color allocation:
   * i is the (internal) 8 bit color number, it consists of separate
   * bit fields for Y, U and V: i = (yyyyuuvv), we don't use yyyy=0000
   * and yyyy=1111, this leaves 32 colors for other applications
   *
   * the allocated colors correspond to the following Y, U and V values:
   * Y:   24, 40, 56, 72, 88, 104, 120, 136, 152, 168, 184, 200, 216, 232
   * U,V: -48, -16, 16, 48
   *
   * U and V values span only about half the color space; this gives
   * usually much better quality, although highly saturated colors can
   * not be displayed properly
   *
   * translation to R,G,B is implicitly done by the color look-up table
   */
  for (i=16; i<240; i++)
  {
	Pixel[i] = i;

    /* color space conversion */
    Y  = 16*((i>>4)&15) + 8;
    Cb = 32*((i>>2)&3)  - 48;
    Cr = 32*(i&3)       - 48;

    Y = 76309 * (Y - 16); /* (255/219)*65536 */

    lut[i-16].r = Clip[(Y + crv*Cr + 32768)>>16];
    lut[i-16].g = Clip[(Y - cgu*Cb - cgv*Cr + 32768)>>16];
    lut[i-16].b = Clip[(Y + cbu*Cb + 32786)>>16];
  }
  
  ssg_src = (void **)&lut;				/* global so callback finds them 	*/
  ssg_xe = 16;
  ssg_ye = 240 - 16;
  ssg_frametype = SSG_FRAMETYPE_LUT;
  DT_C((*ssg_umdec->frame_callback),(ctl, /* give lut to callback 			*/
    ssg_umdec,
    ssg_umdec->frame_arg,
    UMDEC_FRAMETYPE_FRAMEINC));

  DT_C(dtos_malloc2,(ctl,
    (void **)&Dithered_Image,
    Coded_Picture_Width*
    Coded_Picture_Height,
    F, "Dithered_Image"));

  if (!progressive_sequence)
    DT_C(dtos_malloc2,(ctl,
      (void **)&Dithered_Image2,
      Coded_Picture_Width*
      Coded_Picture_Height,
      F, "Dithered_Image2"));

cleanup:
  if (ssg_umdec->rc == DT_RC_GOOD)
    ssg_umdec->rc = rc;
  if (rc != DT_RC_GOOD)
    return 0;
  else
    return 1;
}

void Terminate_Display_Process()
{
  DT_F("ssg_Terminate_Display_Process");
  dt_ctl_t *ctl = ssg_ctl;
  dt_rc_e rc = DT_RC_GOOD;
  if (Dithered_Image2 != NULL)
  DT_I(dtos_free2,(ctl,
    Dithered_Image2,
    F, "Dithered_Image2"));
  if (Dithered_Image != NULL)
  DT_I(dtos_free2,(ctl,
    Dithered_Image,
    F, "Dithered_Image"));
  if (ssg_umdec->rc == DT_RC_GOOD)
    ssg_umdec->rc = rc;
}

static int Display_Image(
  unsigned char *Dithered_Image)
{
  DT_F("ssg_Display_Image");
  dt_ctl_t *ctl = ssg_ctl;
  dt_rc_e rc;
  ssg_src = (void **)&Dithered_Image;	/* global so callback finds them 	*/
  ssg_xe = Coded_Picture_Width;
  ssg_ye = Coded_Picture_Height;
  ssg_frametype = SSG_FRAMETYPE_PSEUDO;
  DT_C((*ssg_umdec->frame_callback),(ctl,
    ssg_umdec,
    ssg_umdec->frame_arg, 
    UMDEC_FRAMETYPE_FRAMEINC));
    
cleanup:
  if (rc == DT_RC_STOP)
    return 0;
  if (ssg_umdec->rc == DT_RC_GOOD)
    ssg_umdec->rc = rc;
  if (rc != DT_RC_GOOD)
    return 0;
  else
    return 1;
}

void Display_Second_Field()
{
  Display_Image(Dithered_Image2);
}

/* 4x4 ordered dither
 *
 * threshold pattern:
 *   0  8  2 10
 *  12  4 14  6
 *   3 11  1  9
 *  15  7 13  5
 */

void Initialize_Dither_Matrix()
{
  int i, v;

  for (i=-8; i<256+8; i++)
  {
    v = i>>4;
    if (v<1)
      v = 1;
    else if (v>14)
      v = 14;
    Y_Table[i+8] = v<<4;
  }

  for (i=0; i<128+16; i++)
  {
    v = (i-40)>>4;
    if (v<0)
      v = 0;
    else if (v>3)
      v = 3;
    Cb_Table[i] = v<<2;
    Cr_Table[i] = v;
  }
}

int dither(src)
unsigned char *src[];
{
  /* should this test only the display flag, not progressive_sequence ? --CF */
  /* CHANGE 95/05/13: progressive_sequence -> progressive_frame */

  if( progressive_frame || Display_Progressive_Flag)
    Dither_Frame(src);
  else
  {
    if ((picture_structure==FRAME_PICTURE && top_field_first) || picture_structure==BOTTOM_FIELD)
    {
      /* top field first */
      if (chroma_format==CHROMA420 && hiQdither)
      {
        Dither_Top_Field420(src,Dithered_Image);
        Dither_Bottom_Field420(src,Dithered_Image2);
      }
      else
      {
        Dither_Top_Field(src,Dithered_Image);
        Dither_Bottom_Field(src,Dithered_Image2);
      }
    }
    else
    {
      /* bottom field first */
      if (chroma_format==CHROMA420 && hiQdither)
      {
        Dither_Bottom_Field420(src,Dithered_Image);
        Dither_Top_Field420(src,Dithered_Image2);
      }
      else
      {
        Dither_Bottom_Field(src,Dithered_Image);
        Dither_Top_Field(src,Dithered_Image2);
      }
    }
  }

  return Display_Image(Dithered_Image);
}

static void Dither_Frame(src)
unsigned char *src[];
{
  int i,j;
  int y,u,v;
  unsigned char *py,*pu,*pv,*dst;

  py = src[0];
  pu = src[1];
  pv = src[2];
  dst = Dithered_Image;

  for (j=0; j<Coded_Picture_Height; j+=4)
  {
    /* line j + 0 */
    for (i=0; i<Coded_Picture_Width; i+=4)
    {
      y = *py++;
      u = *pu++ >> 1;
      v = *pv++ >> 1;
      *dst++ = Pixel[Y_Table[y]|Cb_Table[u]|Cr_Table[v]];
      y = *py++;
      if (chroma_format==CHROMA444)
      {
        u = *pu++ >> 1;
        v = *pv++ >> 1;
      }
      *dst++ = Pixel[Y_Table[y+8]|Cb_Table[u+8]|Cr_Table[v+8]];
      y = *py++;
      u = *pu++ >> 1;
      v = *pv++ >> 1;
      *dst++ = Pixel[Y_Table[y+2]|Cb_Table[u+2]|Cr_Table[v+2]];
      y = *py++;
      if (chroma_format==CHROMA444)
      {
        u = *pu++ >> 1;
        v = *pv++ >> 1;
      }
      *dst++ = Pixel[Y_Table[y+10]|Cb_Table[u+10]|Cr_Table[v+10]];
    }

    if (chroma_format==CHROMA420)
    {
      pu -= Chroma_Width;
      pv -= Chroma_Width;
    }

    /* line j + 1 */
    for (i=0; i<Coded_Picture_Width; i+=4)
    {
      y = *py++;
      u = *pu++ >> 1;
      v = *pv++ >> 1;
      *dst++ = Pixel[Y_Table[y+12]|Cb_Table[u+12]|Cr_Table[v+12]];
      y = *py++;
      if (chroma_format==CHROMA444)
      {
        u = *pu++ >> 1;
        v = *pv++ >> 1;
      }
      *dst++ = Pixel[Y_Table[y+4]|Cb_Table[u+4]|Cr_Table[v+4]];
      y = *py++;
      u = *pu++ >> 1;
      v = *pv++ >> 1;
      *dst++ = Pixel[Y_Table[y+14]|Cb_Table[u+14]|Cr_Table[v+14]];
      y = *py++;
      if (chroma_format==CHROMA444)
      {
        u = *pu++ >> 1;
        v = *pv++ >> 1;
      }
      *dst++ = Pixel[Y_Table[y+6]|Cb_Table[u+6]|Cr_Table[v+6]];
    }

    /* line j + 2 */
    for (i=0; i<Coded_Picture_Width; i+=4)
    {
      y = *py++;
      u = *pu++ >> 1;
      v = *pv++ >> 1;
      *dst++ = Pixel[Y_Table[y+3]|Cb_Table[u+3]|Cr_Table[v+3]];
      y = *py++;
      if (chroma_format==CHROMA444)
      {
        u = *pu++ >> 1;
        v = *pv++ >> 1;
      }
      *dst++ = Pixel[Y_Table[y+11]|Cb_Table[u+11]|Cr_Table[v+11]];
      y = *py++;
      u = *pu++ >> 1;
      v = *pv++ >> 1;
      *dst++ = Pixel[Y_Table[y+1]|Cb_Table[u+1]|Cr_Table[v+1]];
      y = *py++;
      if (chroma_format==CHROMA444)
      {
        u = *pu++ >> 1;
        v = *pv++ >> 1;
      }
      *dst++ = Pixel[Y_Table[y+9]|Cb_Table[u+9]|Cr_Table[v+9]];
    }

    if (chroma_format==CHROMA420)
    {
      pu -= Chroma_Width;
      pv -= Chroma_Width;
    }

    /* line j + 3 */
    for (i=0; i<Coded_Picture_Width; i+=4)
    {
      y = *py++;
      u = *pu++ >> 1;
      v = *pv++ >> 1;
      *dst++ = Pixel[Y_Table[y+15]|Cb_Table[u+15]|Cr_Table[v+15]];
      y = *py++;
      if (chroma_format==CHROMA444)
      {
        u = *pu++ >> 1;
        v = *pv++ >> 1;
      }
      *dst++ = Pixel[Y_Table[y+7]|Cb_Table[u+7]|Cr_Table[v+7]];
      y = *py++;
      u = *pu++ >> 1;
      v = *pv++ >> 1;
      *dst++ = Pixel[Y_Table[y+13]|Cb_Table[u+13]|Cr_Table[v+13]];
      y = *py++;
      if (chroma_format==CHROMA444)
      {
        u = *pu++ >> 1;
        v = *pv++ >> 1;
      }
      *dst++ = Pixel[Y_Table[y+5]|Cb_Table[u+5]|Cr_Table[v+5]];
    }
  }

}

static void Dither_Top_Field(src,dst)
unsigned char *src[];
unsigned char *dst;
{
  int i,j;
  int y,Y2,u,v;
  unsigned char *py,*Y2_ptr,*pu,*pv,*dst2;

  py = src[0];
  Y2_ptr = src[0] + (Coded_Picture_Width<<1);
  pu = src[1];
  pv = src[2];
  dst2 = dst + Coded_Picture_Width;

  for (j=0; j<Coded_Picture_Height; j+=4)
  {
    /* line j + 0, j + 1 */
    for (i=0; i<Coded_Picture_Width; i+=4)
    {
      y = *py++;
      Y2 = *Y2_ptr++;
      u = *pu++ >> 1;
      v = *pv++ >> 1;
      *dst++  = Pixel[Y_Table[y]|Cb_Table[u]|Cr_Table[v]];
      *dst2++ = Pixel[Y_Table[((y+Y2)>>1)+12]|Cb_Table[u+12]|Cr_Table[v+12]];

      y = *py++;
      Y2 = *Y2_ptr++;
      if (chroma_format==CHROMA444)
      {
        u = *pu++ >> 1;
        v = *pv++ >> 1;
      }
      *dst++  = Pixel[Y_Table[y+8]|Cb_Table[u+8]|Cr_Table[v+8]];
      *dst2++ = Pixel[Y_Table[((y+Y2)>>1)+4]|Cb_Table[u+4]|Cr_Table[v+4]];

      y = *py++;
      Y2 = *Y2_ptr++;
      u = *pu++ >> 1;
      v = *pv++ >> 1;
      *dst++  = Pixel[Y_Table[y+2]|Cb_Table[u+2]|Cr_Table[v+2]];
      *dst2++ = Pixel[Y_Table[((y+Y2)>>1)+14]|Cb_Table[u+14]|Cr_Table[v+14]];

      y = *py++;
      Y2 = *Y2_ptr++;
      if (chroma_format==CHROMA444)
      {
        u = *pu++ >> 1;
        v = *pv++ >> 1;
      }
      *dst++  = Pixel[Y_Table[y+10]|Cb_Table[u+10]|Cr_Table[v+10]];
      *dst2++ = Pixel[Y_Table[((y+Y2)>>1)+6]|Cb_Table[u+6]|Cr_Table[v+6]];
    }

    py += Coded_Picture_Width;

    if (j!=(Coded_Picture_Height-4))
      Y2_ptr += Coded_Picture_Width;
    else
      Y2_ptr -= Coded_Picture_Width;

    dst += Coded_Picture_Width;
    dst2 += Coded_Picture_Width;

    if (chroma_format==CHROMA420)
    {
      pu -= Chroma_Width;
      pv -= Chroma_Width;
    }
    else
    {
      pu += Chroma_Width;
      pv += Chroma_Width;
    }

    /* line j + 2, j + 3 */
    for (i=0; i<Coded_Picture_Width; i+=4)
    {
      y = *py++;
      Y2 = *Y2_ptr++;
      u = *pu++ >> 1;
      v = *pv++ >> 1;
      *dst++  = Pixel[Y_Table[y+3]|Cb_Table[u+3]|Cr_Table[v+3]];
      *dst2++ = Pixel[Y_Table[((y+Y2)>>1)+15]|Cb_Table[u+15]|Cr_Table[v+15]];

      y = *py++;
      Y2 = *Y2_ptr++;
      if (chroma_format==CHROMA444)
      {
        u = *pu++ >> 1;
        v = *pv++ >> 1;
      }
      *dst++  = Pixel[Y_Table[y+11]|Cb_Table[u+11]|Cr_Table[v+11]];
      *dst2++ = Pixel[Y_Table[((y+Y2)>>1)+7]|Cb_Table[u+7]|Cr_Table[v+7]];

      y = *py++;
      Y2 = *Y2_ptr++;
      u = *pu++ >> 1;
      v = *pv++ >> 1;
      *dst++  = Pixel[Y_Table[y+1]|Cb_Table[u+1]|Cr_Table[v+1]];
      *dst2++ = Pixel[Y_Table[((y+Y2)>>1)+13]|Cb_Table[u+13]|Cr_Table[v+13]];

      y = *py++;
      Y2 = *Y2_ptr++;
      if (chroma_format==CHROMA444)
      {
        u = *pu++ >> 1;
        v = *pv++ >> 1;
      }
      *dst++  = Pixel[Y_Table[y+9]|Cb_Table[u+9]|Cr_Table[v+9]];
      *dst2++ = Pixel[Y_Table[((y+Y2)>>1)+5]|Cb_Table[u+5]|Cr_Table[v+5]];
    }

    py += Coded_Picture_Width;
    Y2_ptr += Coded_Picture_Width;
    dst += Coded_Picture_Width;
    dst2 += Coded_Picture_Width;
    pu += Chroma_Width;
    pv += Chroma_Width;
  }
}

static void Dither_Bottom_Field(src,dst)
unsigned char *src[];
unsigned char *dst;
{
  int i,j;
  int y,Y2,u,v;
  unsigned char *py,*Y2_ptr,*pu,*pv,*dst2;

  py = src[0] + Coded_Picture_Width;
  Y2_ptr = py;
  pu = src[1] + Chroma_Width;
  pv = src[2] + Chroma_Width;
  dst2 = dst + Coded_Picture_Width;

  for (j=0; j<Coded_Picture_Height; j+=4)
  {
    /* line j + 0, j + 1 */
    for (i=0; i<Coded_Picture_Width; i+=4)
    {
      y = *py++;
      Y2 = *Y2_ptr++;
      u = *pu++ >> 1;
      v = *pv++ >> 1;
      *dst++  = Pixel[Y_Table[((y+Y2)>>1)]|Cb_Table[u]|Cr_Table[v]];
      *dst2++ = Pixel[Y_Table[Y2+12]|Cb_Table[u+12]|Cr_Table[v+12]];

      y = *py++;
      Y2 = *Y2_ptr++;
      if (chroma_format==CHROMA444)
      {
        u = *pu++ >> 1;
        v = *pv++ >> 1;
      }
      *dst++  = Pixel[Y_Table[((y+Y2)>>1)+8]|Cb_Table[u+8]|Cr_Table[v+8]];
      *dst2++ = Pixel[Y_Table[Y2+4]|Cb_Table[u+4]|Cr_Table[v+4]];

      y = *py++;
      Y2 = *Y2_ptr++;
      u = *pu++ >> 1;
      v = *pv++ >> 1;
      *dst++  = Pixel[Y_Table[((y+Y2)>>1)+2]|Cb_Table[u+2]|Cr_Table[v+2]];
      *dst2++ = Pixel[Y_Table[Y2+14]|Cb_Table[u+14]|Cr_Table[v+14]];

      y = *py++;
      Y2 = *Y2_ptr++;
      if (chroma_format==CHROMA444)
      {
        u = *pu++ >> 1;
        v = *pv++ >> 1;
      }
      *dst++  = Pixel[Y_Table[((y+Y2)>>1)+10]|Cb_Table[u+10]|Cr_Table[v+10]];
      *dst2++ = Pixel[Y_Table[Y2+6]|Cb_Table[u+6]|Cr_Table[v+6]];
    }

    if (j==0)
      py -= Coded_Picture_Width;
    else
      py += Coded_Picture_Width;

    Y2_ptr += Coded_Picture_Width;
    dst += Coded_Picture_Width;
    dst2 += Coded_Picture_Width;

    if (chroma_format==CHROMA420)
    {
      pu -= Chroma_Width;
      pv -= Chroma_Width;
    }
    else
    {
      pu += Chroma_Width;
      pv += Chroma_Width;
    }

    /* line j + 2. j + 3 */
    for (i=0; i<Coded_Picture_Width; i+=4)
    {
      y = *py++;
      Y2 = *Y2_ptr++;
      u = *pu++ >> 1;
      v = *pv++ >> 1;
      *dst++  = Pixel[Y_Table[((y+Y2)>>1)+3]|Cb_Table[u+3]|Cr_Table[v+3]];
      *dst2++ = Pixel[Y_Table[Y2+15]|Cb_Table[u+15]|Cr_Table[v+15]];

      y = *py++;
      Y2 = *Y2_ptr++;
      if (chroma_format==CHROMA444)
      {
        u = *pu++ >> 1;
        v = *pv++ >> 1;
      }
      *dst++  = Pixel[Y_Table[((y+Y2)>>1)+11]|Cb_Table[u+11]|Cr_Table[v+11]];
      *dst2++ = Pixel[Y_Table[Y2+7]|Cb_Table[u+7]|Cr_Table[v+7]];

      y = *py++;
      Y2 = *Y2_ptr++;
      u = *pu++ >> 1;
      v = *pv++ >> 1;
      *dst++  = Pixel[Y_Table[((y+Y2)>>1)+1]|Cb_Table[u+1]|Cr_Table[v+1]];
      *dst2++ = Pixel[Y_Table[Y2+13]|Cb_Table[u+13]|Cr_Table[v+13]];

      y = *py++;
      Y2 = *Y2_ptr++;
      if (chroma_format==CHROMA444)
      {
        u = *pu++ >> 1;
        v = *pv++ >> 1;
      }
      *dst++  = Pixel[Y_Table[((y+Y2)>>1)+9]|Cb_Table[u+9]|Cr_Table[v+9]];
      *dst2++ = Pixel[Y_Table[Y2+5]|Cb_Table[u+5]|Cr_Table[v+5]];
    }

    py += Coded_Picture_Width;
    Y2_ptr += Coded_Picture_Width;
    dst += Coded_Picture_Width;
    dst2 += Coded_Picture_Width;
    pu += Chroma_Width;
    pv += Chroma_Width;
  }
}

static void Dither_Top_Field420(src,dst)
unsigned char *src[];
unsigned char *dst;
{
  int i,j;
  int Y1,Cb1,Cr1,Y2,Cb2,Cr2;
  unsigned char *Y1_ptr,*Cb1_ptr,*Cr1_ptr,*Y2_ptr,*Cb2_ptr,*Cr2_ptr,*dst2;

  Y1_ptr = src[0];
  Cb1_ptr = src[1];
  Cr1_ptr = src[2];

  Y2_ptr = Y1_ptr + (Coded_Picture_Width<<1);
  Cb2_ptr = Cb1_ptr + (Chroma_Width<<1);
  Cr2_ptr = Cr1_ptr + (Chroma_Width<<1);

  dst2 = dst + Coded_Picture_Width;

  for (j=0; j<Coded_Picture_Height; j+=4)
  {
    /* line j + 0, j + 1 */
    for (i=0; i<Coded_Picture_Width; i+=4)
    {
      Y1 = *Y1_ptr++;
      Y2 = *Y2_ptr++;
      Cb1 = *Cb1_ptr++ >> 1;
      Cr1 = *Cr1_ptr++ >> 1;
      Cb2 = *Cb2_ptr++ >> 1;
      Cr2 = *Cr2_ptr++ >> 1;
      *dst++  = Pixel[Y_Table[((3*Y1+Y2)>>2)]|Cb_Table[Cb1]|Cr_Table[Cr1]];
      *dst2++ = Pixel[Y_Table[((Y1+3*Y2)>>2)+12]|Cb_Table[((3*Cb1+Cb2)>>2)+12]
                                             |Cr_Table[((3*Cr1+Cr2)>>2)+12]];

      Y1 = *Y1_ptr++;
      Y2 = *Y2_ptr++;
      *dst++  = Pixel[Y_Table[((3*Y1+Y2)>>2)+8]|Cb_Table[Cb1+8]|Cr_Table[Cr1+8]];
      *dst2++ = Pixel[Y_Table[((Y1+3*Y2)>>2)+4]|Cb_Table[((3*Cb1+Cb2)>>2)+4]
                                            |Cr_Table[((3*Cr1+Cr2)>>2)+4]];

      Y1 = *Y1_ptr++;
      Y2 = *Y2_ptr++;
      Cb1 = *Cb1_ptr++ >> 1;
      Cr1 = *Cr1_ptr++ >> 1;
      Cb2 = *Cb2_ptr++ >> 1;
      Cr2 = *Cr2_ptr++ >> 1;
      *dst++  = Pixel[Y_Table[((3*Y1+Y2)>>2)+2]|Cb_Table[Cb1+2]|Cr_Table[Cr1+2]];
      *dst2++ = Pixel[Y_Table[((Y1+3*Y2)>>2)+14]|Cb_Table[((3*Cb1+Cb2)>>2)+14]
                                             |Cr_Table[((3*Cr1+Cr2)>>2)+14]];

      Y1 = *Y1_ptr++;
      Y2 = *Y2_ptr++;
      *dst++  = Pixel[Y_Table[((3*Y1+Y2)>>2)+10]|Cb_Table[Cb1+10]|Cr_Table[Cr1+10]];
      *dst2++ = Pixel[Y_Table[((Y1+3*Y2)>>2)+6]|Cb_Table[((3*Cb1+Cb2)>>2)+6]
                                            |Cr_Table[((3*Cr1+Cr2)>>2)+6]];
    }

    Y1_ptr += Coded_Picture_Width;

    if (j!=(Coded_Picture_Height-4))
      Y2_ptr += Coded_Picture_Width;
    else
      Y2_ptr -= Coded_Picture_Width;

    Cb1_ptr -= Chroma_Width;
    Cr1_ptr -= Chroma_Width;
    Cb2_ptr -= Chroma_Width;
    Cr2_ptr -= Chroma_Width;

    dst  += Coded_Picture_Width;
    dst2 += Coded_Picture_Width;

    /* line j + 2, j + 3 */
    for (i=0; i<Coded_Picture_Width; i+=4)
    {
      Y1 = *Y1_ptr++;
      Y2 = *Y2_ptr++;
      Cb1 = *Cb1_ptr++ >> 1;
      Cr1 = *Cr1_ptr++ >> 1;
      Cb2 = *Cb2_ptr++ >> 1;
      Cr2 = *Cr2_ptr++ >> 1;
      *dst++  = Pixel[Y_Table[((3*Y1+Y2)>>2)+3]|Cb_Table[((Cb1+Cb2)>>1)+3]
                                            |Cr_Table[((Cr1+Cr2)>>1)+3]];
      *dst2++ = Pixel[Y_Table[((Y1+3*Y2)>>2)+15]|Cb_Table[((Cb1+3*Cb2)>>2)+15]
                                             |Cr_Table[((Cr1+3*Cr2)>>2)+15]];

      Y1 = *Y1_ptr++;
      Y2 = *Y2_ptr++;
      *dst++  = Pixel[Y_Table[((3*Y1+Y2)>>2)+11]|Cb_Table[((Cb1+Cb2)>>1)+11]
                                             |Cr_Table[((Cr1+Cr2)>>1)+11]];
      *dst2++ = Pixel[Y_Table[((Y1+3*Y2)>>2)+7]|Cb_Table[((Cb1+3*Cb2)>>2)+7]
                                            |Cr_Table[((Cr1+3*Cr2)>>2)+7]];

      Y1 = *Y1_ptr++;
      Y2 = *Y2_ptr++;
      Cb1 = *Cb1_ptr++ >> 1;
      Cr1 = *Cr1_ptr++ >> 1;
      Cb2 = *Cb2_ptr++ >> 1;
      Cr2 = *Cr2_ptr++ >> 1;
      *dst++  = Pixel[Y_Table[((3*Y1+Y2)>>2)+1]|Cb_Table[((Cb1+Cb2)>>1)+1]
                                            |Cr_Table[((Cr1+Cr2)>>1)+1]];
      *dst2++ = Pixel[Y_Table[((Y1+3*Y2)>>2)+13]|Cb_Table[((Cb1+3*Cb2)>>2)+13]
                                             |Cr_Table[((Cr1+3*Cr2)>>2)+13]];

      Y1 = *Y1_ptr++;
      Y2 = *Y2_ptr++;
      *dst++  = Pixel[Y_Table[((3*Y1+Y2)>>2)+9]|Cb_Table[((Cb1+Cb2)>>1)+9]
                                            |Cr_Table[((Cr1+Cr2)>>1)+9]];
      *dst2++ = Pixel[Y_Table[((Y1+3*Y2)>>2)+5]|Cb_Table[((Cb1+3*Cb2)>>2)+5]
                                            |Cr_Table[((Cr1+3*Cr2)>>2)+5]];
    }

    Y1_ptr += Coded_Picture_Width;
    Y2_ptr += Coded_Picture_Width;
    Cb1_ptr += Chroma_Width;
    Cr1_ptr += Chroma_Width;
    if (j!=(Coded_Picture_Height-8))
    {
      Cb2_ptr += Chroma_Width;
      Cr2_ptr += Chroma_Width;
    }
    else
    {
      Cb2_ptr -= Chroma_Width;
      Cr2_ptr -= Chroma_Width;
    }
    dst += Coded_Picture_Width;
    dst2+= Coded_Picture_Width;
  }
}

static void Dither_Bottom_Field420(src,dst)
unsigned char *src[];
unsigned char *dst;
{
  int i,j;
  int Y1,Cb1,Cr1,Y2,Cb2,Cr2;
  unsigned char *Y1_ptr,*Cb1_ptr,*Cr1_ptr,*Y2_ptr,*Cb2_ptr,*Cr2_ptr,*dst2;

  Y2_ptr = Y1_ptr = src[0] + Coded_Picture_Width;
  Cb2_ptr = Cb1_ptr = src[1] + Chroma_Width;
  Cr2_ptr = Cr1_ptr = src[2] + Chroma_Width;

  dst2 = dst;

  for (j=0; j<Coded_Picture_Height; j+=4)
  {
    /* line j + 0, j + 1 */
    for (i=0; i<Coded_Picture_Width; i+=4)
    {
      Y1 = *Y1_ptr++;
      Y2 = *Y2_ptr++;
      Cb1 = *Cb1_ptr++ >> 1;
      Cr1 = *Cr1_ptr++ >> 1;
      Cb2 = *Cb2_ptr++ >> 1;
      Cr2 = *Cr2_ptr++ >> 1;
      *dst++  = Pixel[Y_Table[((3*Y1+Y2)>>2)+15]|Cb_Table[((3*Cb1+Cb2)>>2)+15]
                                             |Cr_Table[((3*Cr1+Cr2)>>2)+15]];
      *dst2++ = Pixel[Y_Table[((Y1+3*Y2)>>2)]|Cb_Table[((Cb1+Cb2)>>1)]
                                          |Cr_Table[((Cr1+Cr2)>>1)]];

      Y1 = *Y1_ptr++;
      Y2 = *Y2_ptr++;
      *dst++  = Pixel[Y_Table[((3*Y1+Y2)>>2)+7]|Cb_Table[((3*Cb1+Cb2)>>2)+7]
                                            |Cr_Table[((3*Cr1+Cr2)>>2)+7]];
      *dst2++ = Pixel[Y_Table[((Y1+3*Y2)>>2)+8]|Cb_Table[((Cb1+Cb2)>>1)+8]
                                            |Cr_Table[((Cr1+Cr2)>>1)+8]];

      Y1 = *Y1_ptr++;
      Y2 = *Y2_ptr++;
      Cb1 = *Cb1_ptr++ >> 1;
      Cr1 = *Cr1_ptr++ >> 1;
      Cb2 = *Cb2_ptr++ >> 1;
      Cr2 = *Cr2_ptr++ >> 1;
      *dst++  = Pixel[Y_Table[((3*Y1+Y2)>>2)+13]|Cb_Table[((3*Cb1+Cb2)>>2)+13]
                                             |Cr_Table[((3*Cr1+Cr2)>>2)+13]];
      *dst2++ = Pixel[Y_Table[((Y1+3*Y2)>>2)+2]|Cb_Table[((Cb1+Cb2)>>1)+2]
                                            |Cr_Table[((Cr1+Cr2)>>1)+2]];

      Y1 = *Y1_ptr++;
      Y2 = *Y2_ptr++;
      *dst++  = Pixel[Y_Table[((3*Y1+Y2)>>2)+5]|Cb_Table[((3*Cb1+Cb2)>>2)+5]
                                            |Cr_Table[((3*Cr1+Cr2)>>2)+5]];
      *dst2++ = Pixel[Y_Table[((Y1+3*Y2)>>2)+10]|Cb_Table[((Cb1+Cb2)>>1)+10]
                                             |Cr_Table[((Cr1+Cr2)>>1)+10]];
    }

    if (j!=0)
      Y1_ptr += Coded_Picture_Width;
    else
      Y1_ptr -= Coded_Picture_Width;

    Y2_ptr += Coded_Picture_Width;

    Cb1_ptr -= Chroma_Width;
    Cr1_ptr -= Chroma_Width;
    Cb2_ptr -= Chroma_Width;
    Cr2_ptr -= Chroma_Width;

    if (j!=0)
      dst  += Coded_Picture_Width;

    dst2 += Coded_Picture_Width;

    /* line j + 2, j + 3 */
    for (i=0; i<Coded_Picture_Width; i+=4)
    {
      Y1 = *Y1_ptr++;
      Y2 = *Y2_ptr++;
      Cb1 = *Cb1_ptr++ >> 1;
      Cr1 = *Cr1_ptr++ >> 1;
      Cb2 = *Cb2_ptr++ >> 1;
      Cr2 = *Cr2_ptr++ >> 1;
      *dst++  = Pixel[Y_Table[((3*Y1+Y2)>>2)+12]|Cb_Table[((Cb1+3*Cb2)>>2)+12]
                                             |Cr_Table[((Cr1+3*Cr2)>>2)+12]];
      *dst2++ = Pixel[Y_Table[((Y1+3*Y2)>>2)+3]|Cb_Table[Cb2+3]
                                            |Cr_Table[Cr2+3]];

      Y1 = *Y1_ptr++;
      Y2 = *Y2_ptr++;
      *dst++  = Pixel[Y_Table[((3*Y1+Y2)>>2)+4]|Cb_Table[((Cb1+3*Cb2)>>2)+4]
                                            |Cr_Table[((Cr1+3*Cr2)>>2)+4]];
      *dst2++ = Pixel[Y_Table[((Y1+3*Y2)>>2)+11]|Cb_Table[Cb2+11]
                                             |Cr_Table[Cr2+11]];

      Y1 = *Y1_ptr++;
      Y2 = *Y2_ptr++;
      Cb1 = *Cb1_ptr++ >> 1;
      Cr1 = *Cr1_ptr++ >> 1;
      Cb2 = *Cb2_ptr++ >> 1;
      Cr2 = *Cr2_ptr++ >> 1;
      *dst++  = Pixel[Y_Table[((3*Y1+Y2)>>2)+14]|Cb_Table[((Cb1+3*Cb2)>>2)+14]
                                             |Cr_Table[((Cr1+3*Cr2)>>2)+14]];
      *dst2++ = Pixel[Y_Table[((Y1+3*Y2)>>2)+1]|Cb_Table[Cb2+1]
                                            |Cr_Table[Cr2+1]];

      Y1 = *Y1_ptr++;
      Y2 = *Y2_ptr++;
      *dst++  = Pixel[Y_Table[((3*Y1+Y2)>>2)+6]|Cb_Table[((Cb1+3*Cb2)>>2)+6]
                                            |Cr_Table[((Cr1+3*Cr2)>>2)+6]];
      *dst2++ = Pixel[Y_Table[((Y1+3*Y2)>>2)+9]|Cb_Table[Cb2+9]
                                            |Cr_Table[Cr2+9]];
    }

    Y1_ptr += Coded_Picture_Width;
    Y2_ptr += Coded_Picture_Width;

    if (j!=0)
    {
      Cb1_ptr += Chroma_Width;
      Cr1_ptr += Chroma_Width;
    }
    else
    {
      Cb1_ptr -= Chroma_Width;
      Cr1_ptr -= Chroma_Width;
    }

    Cb2_ptr += Chroma_Width;
    Cr2_ptr += Chroma_Width;

    dst += Coded_Picture_Width;
    dst2+= Coded_Picture_Width;
  }

  Y2_ptr -= (Coded_Picture_Width<<1);
  Cb2_ptr -= (Chroma_Width<<1);
  Cr2_ptr -= (Chroma_Width<<1);

  /* dither last line */
  for (i=0; i<Coded_Picture_Width; i+=4)
  {
    Y1 = *Y1_ptr++;
    Y2 = *Y2_ptr++;
    Cb1 = *Cb1_ptr++ >> 1;
    Cr1 = *Cr1_ptr++ >> 1;
    Cb2 = *Cb2_ptr++ >> 1;
    Cr2 = *Cr2_ptr++ >> 1;
    *dst++  = Pixel[Y_Table[((3*Y1+Y2)>>2)+15]|Cb_Table[((3*Cb1+Cb2)>>2)+15]
                                           |Cr_Table[((3*Cr1+Cr2)>>2)+15]];

    Y1 = *Y1_ptr++;
    Y2 = *Y2_ptr++;
    *dst++  = Pixel[Y_Table[((3*Y1+Y2)>>2)+7]|Cb_Table[((3*Cb1+Cb2)>>2)+7]
                                          |Cr_Table[((3*Cr1+Cr2)>>2)+7]];

    Y1 = *Y1_ptr++;
    Y2 = *Y2_ptr++;
    Cb1 = *Cb1_ptr++ >> 1;
    Cr1 = *Cr1_ptr++ >> 1;
    Cb2 = *Cb2_ptr++ >> 1;
    Cr2 = *Cr2_ptr++ >> 1;
    *dst++  = Pixel[Y_Table[((3*Y1+Y2)>>2)+13]|Cb_Table[((3*Cb1+Cb2)>>2)+13]
                                           |Cr_Table[((3*Cr1+Cr2)>>2)+13]];

    Y1 = *Y1_ptr++;
    Y2 = *Y2_ptr++;
    *dst++  = Pixel[Y_Table[((3*Y1+Y2)>>2)+5]|Cb_Table[((3*Cb1+Cb2)>>2)+5]
                                          |Cr_Table[((3*Cr1+Cr2)>>2)+5]];
    }

}





/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
  ! Copyright (C) 1998 Dtack Inc. All Rights Reserved
  ! To use this file, you must have signed a license agreement with Dtack Inc.
  ! Under no circumstances may you redistribute this file.
  ! This software is provided AS IS with no warranty expressed or implied.
  ! Dtack Inc. accepts no liability for use or misuse of this file.
  ! http://www.dtack.com  dtack@dtack.com  telephone +360.417.6700
  ! Dtack Inc, P.O. Box 3061, Port Angeles, WA USA 98362
  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */
