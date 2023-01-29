/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
  ! Copyright (C) 1998 Dtack Inc. All Rights Reserved
  ! To use this file, you must have signed a license agreement with Dtack Inc.
  ! Under no circumstances may you redistribute this file.
  ! This software is provided AS IS with no warranty expressed or implied.
  ! Dtack Inc. accepts no liability for use or misuse of this file.
  ! http://www.dtack.com  dtack@dtack.com  telephone +360.417.6700
  ! Dtack Inc, P.O. Box 3061, Port Angeles, WA USA 98362
  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */




/* systems.c, systems-specific routines                                 */

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
DT_RCSID("fourth/mpegssg $RCSfile: display.c,v $ $Revision: 1.7 $");
#include <umdec.h>

#include <ssg.h>

#include <config.h>
#include <global.h>

#define UDEBUG(A) printf A
#undef  UDEBUG
#define UDEBUG(A)

/* initialize buffer, call once before first getbits or showbits */

/* parse system layer, ignore everything we don't need */
void Next_Packet()
{
  unsigned int code;
  int l;

  for(;;)
  {
    code = Get_Long();

    /* remove system layer byte stuffing */
    while ((code & 0xffffff00) != 0x100)
	{
      code = (code<<8) | Get_Byte();
	}

    switch(code)
    {
    case PACK_START_CODE:				/* pack header 0x000001ba */
	  UDEBUG(("packet code %08x == PACK_START_CODE\n",code));
      /* skip pack header (system_clock_reference and mux_rate) */
      ld->Rdptr += 8;
      break;
    case VIDEO_ELEMENTARY_STREAM:		/* 0x000001e0 						*/
	{
	  int length;
      length = Get_Word();				/* packet_length 					*/
	  UDEBUG(("packet code %08x == VIDEO_ELEMENTARY_STREAM length=%d\n",
        code, length));
      ld->Rdmax = ld->Rdptr + length;

      code = Get_Byte();

      if((code>>6)==0x02)
      {
        ld->Rdptr++;
        code=Get_Byte();  /* parse PES_header_data_length */
        ld->Rdptr+=code;    /* advance pointer by PES_header_data_length */
        return;
      }
      else if(code==0xff)
      {
        /* parse MPEG-1 packet header */
		int n = 1;
        while((code=Get_Byte())== 0xFF) n++;
      }
       
      /* stuffing bytes */
      if(code>=0x40)
      {
        if(code>=0x80)
        {
          fprintf(stderr,"Error in packet header\n");
          exit(1);
        }
        /* skip STD_buffer_scale */
        ld->Rdptr++;
        code = Get_Byte();
      }

      if(code>=0x30)
      {
        if(code>=0x40)
        {
          fprintf(stderr,"Error in packet header\n");
          exit(1);
        }
        /* skip presentation and decoding time stamps */
        ld->Rdptr += 9;
      }
      else if(code>=0x20)
      {
        /* skip presentation time stamps */
        ld->Rdptr += 4;
      }
      else if(code!=0x0f)
      {
        fprintf(stderr,"Error in packet header\n");
        exit(1);
      }
	  
      return;
	}
    case ISO_END_CODE: /* end */
	  UDEBUG(("packet code %08x == ISO_END_CODE\n",code));
	  longjmp(ssg_main_jump, 
        SSG_MAIN_JUMP_ISO_END);
	  
	case SEQUENCE_END_CODE:
	  UDEBUG(("packet code %08x == SEQUENCE_END_CODE\n",code));
	  longjmp(ssg_main_jump, 
        SSG_MAIN_JUMP_SEQUENCE_END);
    default:
      if(code>=SYSTEM_START_CODE)		/* 0x000001bb 						*/
      {
		UDEBUG(("packet code %08x >= SYSTEM_START_CODE 1bb\n",code));
        /* skip system headers and non-video packets*/
        code = Get_Word();
        ld->Rdptr += code;
      }
      else
      {
        fprintf(stderr,"Unexpected startcode %08x in system layer"
          " after %ld bytes fetched\n",code, ssg_read_total);
        exit(1);
      }
      break;
    }
  }
}



void Flush_Buffer32()
{
  int Incnt;

  ld->Bfr = 0;

  Incnt = ld->Incnt;
  Incnt -= 32;

  if (System_Stream_Flag && (ld->Rdptr >= ld->Rdmax-4))
  {
    while (Incnt <= 24)
    {
      if (ld->Rdptr >= ld->Rdmax)
        Next_Packet();
      ld->Bfr |= Get_Byte() << (24 - Incnt);
      Incnt += 8;
    }
  }
  else
  {
    while (Incnt <= 24)
    {
      if (ld->Rdptr >= ld->Rdbfr+BUFSIZE)
        Fill_Buffer();
      ld->Bfr |= *ld->Rdptr++ << (24 - Incnt);
      Incnt += 8;
    }
  }
  ld->Incnt = Incnt;

#ifdef VERIFY 
  ld->Bitcnt += 32;
#endif /* VERIFY */
}


unsigned int Get_Bits32()
{
  unsigned int l;

  l = Show_Bits(32);
  Flush_Buffer32();

  return l;
}


int Get_Long()
{
  int i;

  i = Get_Word();
  return (i<<16) | Get_Word();
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
