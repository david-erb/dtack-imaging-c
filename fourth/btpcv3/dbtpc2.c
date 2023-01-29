/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
  ! Copyright (C) 1998 Dtack Inc. All Rights Reserved
  ! To use this file, you must have signed a license agreement with Dtack Inc.
  ! Under no circumstances may you redistribute this file.
  ! This software is provided AS IS with no warranty expressed or implied.
  ! Dtack Inc. accepts no liability for use or misuse of this file.
  ! http://www.dtack.com  dtack@dtack.com  telephone +360.417.6700
  ! Dtack Inc, P.O. Box 3061, Port Angeles, WA USA 98362
  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */




//
// Eight+one-band binary tree predictive decoder
//
// Version 3.
//
// Copyright (c) John Robinson. 1994, 1995.
//

#include <dtack/base.h>
#include <cfpio.h>
#include <compactr.h>
#include <btpc.h>

unsigned char *initclip(const int& topval);
void ycc_rgb(unsigned char ***components, const int& rows, const int& cols,
const unsigned char *clip, const char& expand);
void btpc_dec_plane(unsigned char **out,const int& XMAX, const int& YMAX,
const int& fromlevel, const int& tolevel, const int *spacing,
Compactr *codein, const int& clipval);

/*..........................................................................
 *..........................................................................*/

int writepnm(
  const char *pgmname, 
  unsigned char ***pbuf, 
  const int& rows, 
  const int& cols, 
  const int& maxval, 
  const int& pnmtype)
// Write picture to "P5" pgm or "P6" ppm format
{
  FILE *out = fopen(pgmname, "w");
  if (!out)
  {
    fprintf(stderr,"Cannot open %s\n", pgmname);
    exit (0);
  }
  if (pnmtype == 1)
  {	// PGM
    fprintf(out,"P5\n%d %d\n%d\n", cols, rows, maxval);
    for (int i = 0; i < rows; i++)
      fwrite(pbuf[0][i],1,cols,out);
  }
  else
  { // PPM
    fprintf(out,"P6\n%d %d\n%d\n", cols, rows, maxval);
    unsigned char *temp = new unsigned char[3*cols];
    for (int i = 0; i < rows; i++)
    {
      unsigned char *ptemp = temp;
      const unsigned char *pg = pbuf[0][i];
      const unsigned char *pr = pbuf[1][i];
      const unsigned char *pb = pbuf[2][i];
      for (int j = 0; j < cols; j++)
      {
	*ptemp++ = *pr++;
	*ptemp++ = *pg++;
	*ptemp++ = *pb++;
      }
      fwrite(temp,1,cols*3,out);
    }
    delete [] temp;
  }
  fclose(out);
  return 0;
}


/*..........................................................................
 *..........................................................................*/
int main(
  int argc, 
  char *argv[])
{
  if (argc != 3)
  {
    fprintf(stderr, "Usage: dbtpc code_file pgm_outpic\n");
    return -1;
  }

  // Use old stdlib for input because it seems to be more efficient
  FILE *cfp = fopen(argv[1],"r");
  if (!cfp) 
  {
    fprintf(stderr, "Can't open %s\n", argv[1]);
    return -1;
  }

  char *b;
  b = (char *)malloc(65536);

  int length;
  length = fread(b, 1, 65536, cfp); 	// read entire file
  
  if (length >= 65536)
  {
    fprintf(stderr, 
      "Too many bytes in %s\n", 
      argv[1]);
    return -1;
  }

  btpc_rc_e rc;
  int xe, ye;
  int maxval, pnmtype, numlevels;
  char coltype, uvfilter;
  int spacing[65];
  int got;

  rc = btpc_decode_header(		// scan the header
    b, length,	
    &xe, 
    &ye, 
    &maxval,
    &pnmtype,
    &numlevels,
    &coltype,
    &uvfilter,
    spacing,
    &got);

  if (rc != BTPC_RC_GOOD)
  {
    fprintf(stderr, 
      "Decode error %d in %s\n", 
      rc, argv[1]);
    return -1;
  }

  // Allocate space for decoded picture

  int xe2 = ((xe+15)>>4)<<4;
  int ye2 = ((ye+15)>>4)<<4;

  unsigned char **components[3];
  unsigned char **out;
  for (int compnum = 0; compnum < pnmtype; compnum++)
  {
    components[compnum] = out = new unsigned char*[ye2];
    int i;
    for (i = 0; i < ye2; i++)
    {
      out[i] = new unsigned char[xe2];
      memset(out[i],0, xe2);
    }
  }

  rc = btpc_decode(
    b+got, length-got,
    xe, 
    ye, 
    maxval,
    pnmtype,
    numlevels,
    coltype,
    uvfilter,
    spacing,
    components);
  if (rc != BTPC_RC_GOOD)
  {
    fprintf(stderr, 
      "Decode error %d in %s\n", 
      rc, argv[1]);
    return -1;
  }

  writepnm(argv[2], 
    components, 
    ye, xe,				// watch it, watch it!
    maxval, pnmtype);

  for (compnum = 0; compnum < pnmtype; compnum++)
  {
    out = components[compnum];
    int i;
    for (i = 0; i < ye; i++) 
      delete [] out[i];
    delete [] out;
  }
  return 1;
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
