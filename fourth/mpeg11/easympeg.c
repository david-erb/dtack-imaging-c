/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
  ! Copyright (C) 1998 Dtack Inc. All Rights Reserved
  ! To use this file, you must have signed a license agreement with Dtack Inc.
  ! Under no circumstances may you redistribute this file.
  ! This software is provided AS IS with no warranty expressed or implied.
  ! Dtack Inc. accepts no liability for use or misuse of this file.
  ! http://www.dtack.com  dtack@dtack.com  telephone +360.417.6700
  ! Dtack Inc, P.O. Box 3061, Port Angeles, WA USA 98362
  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */




/*
 * The World's Easiest MPEG Player -- a demonstration of the MPEG Library,
 * written for Silicon Graphics machines.  Masochists may port to X
 * if desired.
 *
 * By Greg Ward, 94/8/15
 */

#include <stdlib.h>
#include <errno.h>
#include <gl.h>
#define BOOLEAN_TYPE_EXISTS
#include "mpeg.h"


long ImageChecksum (ImageDesc *img, char *pixels)
{
   int   i;
   long  sum = 0;

   for (i = 0; i < img->Size; i++)
   {
      sum += pixels [img->Size];
   }
   return (sum);
}


int main (int argc, char *argv[])
{
   FILE       *mpeg;
   ImageDesc   img;
   char       *pixels;
   Boolean     moreframes = TRUE;

   if (argc != 2) 
   {
      fprintf (stderr, "Usage: %s mpegfile\n", argv[0]);
      exit (1);
   }

   mpeg = fopen (argv[1], "r");
   if (!mpeg)
   {
      perror (argv[1]);
      exit (1);
   }
      
   if (!OpenMPEG (mpeg, &img))
   {
      fprintf (stderr, "OpenMPEG on %s failed\n", argv[1]);
      exit (1);
   }

   pixels = (char *) malloc (img.Size * sizeof(char));

   foreground ();
   prefsize (img.Width, img.Height);
   winopen ("Easy MPEG");
   RGBmode ();
   gconfig ();
   clear ();
   
   while (1)
   {
      while (moreframes)
      {
	 moreframes = GetMPEGFrame (pixels);
/*	 printf ("checksum: %d\n", ImageChecksum (&img, pixels));      */
	 lrectwrite (0, 0, img.Width-1, img.Height-1, (unsigned long *) pixels);
      }

      RewindMPEG (mpeg, &img);
      moreframes = TRUE;
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
