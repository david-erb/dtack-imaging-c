/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
  ! Copyright (C) 1998 Dtack Inc. All Rights Reserved
  ! To use this file, you must have signed a license agreement with Dtack Inc.
  ! Under no circumstances may you redistribute this file.
  ! This software is provided AS IS with no warranty expressed or implied.
  ! Dtack Inc. accepts no liability for use or misuse of this file.
  ! http://www.dtack.com  dtack@dtack.com  telephone +360.417.6700
  ! Dtack Inc, P.O. Box 3061, Port Angeles, WA USA 98362
  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */





/* mpeg2dec.c, main(), initialization, option processing                    */

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

#include <dtack/base.h>
DT_RCSID("fourth/mpegssg $RCSfile: ssg.c,v $ $Revision: 1.7 $");
#include <umdec.h>

#define GLOBAL
#include <ssg.h>

#include <config.h>
#include <global.h>

/* private prototypes */
static int  video_sequence _ANSI_ARGS_((int *framenum));
static int Decode_Bitstream _ANSI_ARGS_((void));
static int  Headers _ANSI_ARGS_((void));
static int Initialize_Sequence _ANSI_ARGS_((void));
static void Initialize_Decoder _ANSI_ARGS_((void));
static void Deinitialize_Sequence _ANSI_ARGS_((void));
static void Clear_Options _ANSI_ARGS_((void));

#if OLD
static int  Get_Val _ANSI_ARGS_((char *argv[]));
#endif

/*..........................................................................*/

int 
ssg_y_cb_cr(
  void **src,
  int offset,
  int incr,
  int hsize,
  int height)
{
  DT_F("ssg_y_cb_cr");
  dt_ctl_t *ctl = ssg_ctl;
  dt_rc_e rc;
  ssg_src = src;						/* global so callback finds them 	*/
  ssg_xe = hsize;
  ssg_ye = height;
  ssg_frametype = SSG_FRAMETYPE_YCBCR_420;
  DT_G((*ssg_umdec->frame_callback),(ctl, 
    ssg_umdec,
    ssg_umdec->frame_arg,
    UMDEC_FRAMETYPE_FRAMEINC));
  if (rc == DT_RC_STOP)
    return 0;
  if (ssg_umdec->rc == DT_RC_GOOD)
    ssg_umdec->rc = rc;
  if (rc != DT_RC_GOOD)
    return 0;
  else
    return 1;
}

/*..........................................................................*/

int 
ssg_read(
  char *buffer,
  int want)
{
  DT_F("ssg_read");
  dt_ctl_t *ctl = ssg_ctl;
  dt_rc_e rc;
  long got = 0;
  DT_G((*ssg_umdec->feed_callback),(ctl,	/* dip into caller's feed function 	*/
    ssg_umdec,
    ssg_umdec->feed_arg,  
    buffer, want, &got));
  if (ssg_umdec->control.mode ==		/* mode is set to stop? 			*/
	  UMDEC_MODE_STOP)
    got = 0;							/* pretend like end of data 		*/
  ssg_read_total += got;

  if (got < want)						/* ran out of data? 				*/
  {
    longjmp(ssg_main_jump,				/* jump to ssg_main 				*/
      SSG_MAIN_JUMP_SSG_READ);
  }

  return (int)got;
}

/*..........................................................................*/

dt_rc_e 
DTCONFIG_API1
ssg_main(
  dt_ctl_t *ctl,
  umdec_t *mdec,
  int keep_picture_coding,
  const char *output_type)
{
  DT_F("ssg_main");
  int jumped;
  ssg_ctl = ctl;						/* place in global 					*/
  ssg_umdec = mdec;
  
  ssg_need_lep = 1;						/* need LEP to start 				*/
  ssg_read_total = 0;
  ssg_total_i = 0;
  ssg_total_p = 0;
  ssg_total_b = 0;
  ssg_total_d = 0;
  ssg_skipped_i = 0;
  ssg_skipped_p = 0;
  ssg_skipped_b = 0;
  ssg_skipped_d = 0;

  if (keep_picture_coding != 0)			/* caller is giving this? 			*/
    ssg_keep_picture_coding =			/* use it 							*/
      keep_picture_coding;
  else
    ssg_keep_picture_coding = 1;		/* use I frames only				*/
  
  Clear_Options();
  if (!strcmp(output_type, SSG_OUTPUT_DITHER))
    Output_Type = T_X11;
  else
    Output_Type = T_Y_CB_CR;

  Quiet_Flag = 0;
  Verbose_Flag = ALL_LAYERS;

  ld = &base; /* select base layer context */

  System_Stream_Flag = 1;

  jumped = setjmp(ssg_main_jump);		/* set jump point 					*/
  if (jumped == 0)						/* we didn't jump? 					*/
  {
	Initialize_Buffer();				/* do the work 						*/

	Initialize_Decoder();

	Decode_Bitstream();
  }

  dt_dbg(ctl, F, DT_DBG_MASK_COMPLEX,
	"read %d bytes", ssg_read_total);
  dt_dbg(ctl, F, DT_DBG_MASK_COMPLEX,
    "skipped %d out of %d I frames",
    ssg_skipped_i, ssg_total_i);
  dt_dbg(ctl, F, DT_DBG_MASK_COMPLEX,
    "skipped %d out of %d P frames",
    ssg_skipped_p, ssg_total_p);
  dt_dbg(ctl, F, DT_DBG_MASK_COMPLEX,
    "skipped %d out of %d B frames",
    ssg_skipped_b, ssg_total_b);
  dt_dbg(ctl, F, DT_DBG_MASK_COMPLEX,
    "skipped %d out of %d D frames",
    ssg_skipped_d, ssg_total_d);

  return ssg_umdec->rc;
}
/*..........................................................................*/

/* IMPLEMENTAION specific rouintes */
static void Initialize_Decoder()
{
  int i;

  /* Clip table */
  if (!(Clip=(unsigned char *)malloc(1024)))
    Error("Clip[] malloc failed\n");

  Clip += 384;

  for (i=-384; i<640; i++)
    Clip[i] = (i<0) ? 0 : ((i>255) ? 255 : i);

  /* IDCT */
  if (Reference_IDCT_Flag)
    Initialize_Reference_IDCT();
  else
    Initialize_Fast_IDCT();

}
/*..........................................................................*/


/* mostly IMPLEMENTAION specific rouintes */
static int Initialize_Sequence()
{
  int cc, size;
  static int Table_6_20[3] = {6,8,12};
  int rc = 1;

  /* check scalability mode of enhancement layer */
  if (Two_Streams && (enhan.scalable_mode!=SC_SNR) && (base.scalable_mode!=SC_DP))
    Error("unsupported scalability mode\n");

  /* force MPEG-1 parameters for proper decoder behavior */
  /* see ISO/IEC 13818-2 section D.9.14 */
  if (!base.MPEG2_Flag)
  {
    progressive_sequence = 1;
    progressive_frame = 1;
    picture_structure = FRAME_PICTURE;
    frame_pred_frame_dct = 1;
    chroma_format = CHROMA420;
    matrix_coefficients = 5;
  }

  /* round to nearest multiple of coded macroblocks */
  /* ISO/IEC 13818-2 section 6.3.3 sequence_header() */
  mb_width = (horizontal_size+15)/16;
  mb_height = (base.MPEG2_Flag && !progressive_sequence) ? 2*((vertical_size+31)/32)
                                        : (vertical_size+15)/16;

  Coded_Picture_Width = 16*mb_width;
  Coded_Picture_Height = 16*mb_height;

  /* ISO/IEC 13818-2 sections 6.1.1.8, 6.1.1.9, and 6.1.1.10 */
  Chroma_Width = (chroma_format==CHROMA444) ? Coded_Picture_Width
                                           : Coded_Picture_Width>>1;
  Chroma_Height = (chroma_format!=CHROMA420) ? Coded_Picture_Height
                                            : Coded_Picture_Height>>1;
  
  /* derived based on Table 6-20 in ISO/IEC 13818-2 section 6.3.17 */
  block_count = Table_6_20[chroma_format-1];

  for (cc=0; cc<3; cc++)
  {
    if (cc==0)
      size = Coded_Picture_Width*Coded_Picture_Height;
    else
      size = Chroma_Width*Chroma_Height;

    if (!(backward_reference_frame[cc] = (unsigned char *)malloc(size)))
      Error("backward_reference_frame[] malloc failed\n");

    if (!(forward_reference_frame[cc] = (unsigned char *)malloc(size)))
      Error("forward_reference_frame[] malloc failed\n");

    if (!(auxframe[cc] = (unsigned char *)malloc(size)))
      Error("auxframe[] malloc failed\n");

    if(Ersatz_Flag)
      if (!(substitute_frame[cc] = (unsigned char *)malloc(size)))
        Error("substitute_frame[] malloc failed\n");


    if (base.scalable_mode==SC_SPAT)
    {
      /* this assumes lower layer is 4:2:0 */
      if (!(llframe0[cc] = (unsigned char *)malloc((lower_layer_prediction_horizontal_size*lower_layer_prediction_vertical_size)/(cc?4:1))))
        Error("llframe0 malloc failed\n");
      if (!(llframe1[cc] = (unsigned char *)malloc((lower_layer_prediction_horizontal_size*lower_layer_prediction_vertical_size)/(cc?4:1))))
        Error("llframe1 malloc failed\n");
    }
  }

  /* SCALABILITY: Spatial */
  if (base.scalable_mode==SC_SPAT)
  {
    if (!(lltmp = (short *)malloc(lower_layer_prediction_horizontal_size*((lower_layer_prediction_vertical_size*vertical_subsampling_factor_n)/vertical_subsampling_factor_m)*sizeof(short))))
      Error("lltmp malloc failed\n");
  }

  if (Output_Type==T_X11)
  {
	rc = Initialize_Display_Process("");
	if (rc == 1)
      Initialize_Dither_Matrix();
  }

  return rc;
}
/*..........................................................................*/

void Error(text)
char *text;
{
  fprintf(stderr,text);
  exit(1);
}
/*..........................................................................*/

/* Trace_Flag output */
void Print_Bits(code,bits,len)
int code,bits,len;
{
  int i;
  for (i=0; i<len; i++)
    printf("%d",(code>>(bits-1-i))&1);
}

/*..........................................................................*/



static int Headers()
{
  int ret;

  ld = &base;
  

  /* return when end of sequence (0) or picture
     header has been parsed (1) */

  ret = Get_Hdr();


  if (Two_Streams)
  {
    ld = &enhan;
    if (Get_Hdr()!=ret && !Quiet_Flag)
      fprintf(stderr,"streams out of sync\n");
    ld = &base;
  }

  return ret;
}

/*..........................................................................*/


static int Decode_Bitstream()
{
  int ret;
  int Bitstream_Framenum;

  Bitstream_Framenum = 0;

  for(;;)
  {

#ifdef VERIFY
    Clear_Verify_Headers();
#endif /* VERIFY */

    ret = Headers();
    
    if(ret == 1)
      ret = video_sequence(&Bitstream_Framenum);

    if(ret != 1)
      return ret;
  }
}

/*..........................................................................*/

static void Deinitialize_Sequence()
{
  int i;

  /* clear flags */
  base.MPEG2_Flag=0;

  for(i=0;i<3;i++)
  {
    free(backward_reference_frame[i]);
    free(forward_reference_frame[i]);
    free(auxframe[i]);

    if (base.scalable_mode==SC_SPAT)
    {
     free(llframe0[i]);
     free(llframe1[i]);
    }
  }

  if (base.scalable_mode==SC_SPAT)
    free(lltmp);

  if (Output_Type==T_X11) 
    Terminate_Display_Process();
}

/*..........................................................................*/

static int video_sequence(Bitstream_Framenumber)
int *Bitstream_Framenumber;
{
  int Bitstream_Framenum;
  int Sequence_Framenum;
  int Return_Value;

  Bitstream_Framenum = *Bitstream_Framenumber;
  Sequence_Framenum=0;

  Return_Value = Initialize_Sequence();
  if (Return_Value != 1)
    return Return_Value;

  /* decode picture whose header has already been parsed in 
     Decode_Bitstream() */

  Return_Value = Decode_Picture(
    Bitstream_Framenum, Sequence_Framenum);
  if (Return_Value == 0)
    return Return_Value;

  /* update picture numbers */
  if (!Second_Field)
  {
    Bitstream_Framenum++;
    Sequence_Framenum++;
  }

  /* loop through the rest of the pictures in the sequence */
  while ((Return_Value=Headers()))
  {
    Return_Value = Decode_Picture(Bitstream_Framenum, Sequence_Framenum);
	if (Return_Value == 0)
      break;

    if (!Second_Field)
    {
      Bitstream_Framenum++;
      Sequence_Framenum++;
    }
  }

  /* put last frame */
  if (Sequence_Framenum!=0)
  {
    Output_Last_Frame_of_Sequence(Bitstream_Framenum);
  }

  Deinitialize_Sequence();

#ifdef VERIFY
    Clear_Verify_Headers();
#endif /* VERIFY */

  *Bitstream_Framenumber = Bitstream_Framenum;
  return(Return_Value);
}



static void Clear_Options()
{
  Verbose_Flag = SEQUENCE_LAYER;
  Output_Picture_Filename = " ";
  hiQdither  = 0;
  Frame_Store_Flag = 0;
  Spatial_Flag = 0;
  Lower_Layer_Picture_Filename = " ";
  Reference_IDCT_Flag = 0;
  Trace_Flag = 0;
  Quiet_Flag = 0;
  Ersatz_Flag = 0;
  Substitute_Picture_Filename  = " ";
  Two_Streams = 0;
  Enhancement_Layer_Bitstream_Filename = " ";
  Big_Picture_Flag = 0;
  Main_Bitstream_Flag = 0;
  Main_Bitstream_Filename = " ";
  Verify_Flag = 0;
  Stats_Flag  = 0;
  User_Data_Flag = 0;
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
