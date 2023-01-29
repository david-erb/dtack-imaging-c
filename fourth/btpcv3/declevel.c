/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
  ! Copyright (C) 1998 Dtack Inc. All Rights Reserved
  ! To use this file, you must have signed a license agreement with Dtack Inc.
  ! Under no circumstances may you redistribute this file.
  ! This software is provided AS IS with no warranty expressed or implied.
  ! Dtack Inc. accepts no liability for use or misuse of this file.
  ! http://www.dtack.com  dtack@dtack.com  telephone +360.417.6700
  ! Dtack Inc, P.O. Box 3061, Port Angeles, WA USA 98362
  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */




#include <btpcv3.h>

//
// Binary tree predictive coding
// Helper routines for decoding, plus main decoding function
//
// Copyright (C) John Robinson, 1994, 1995.
//

#ifndef DTDEPEND
#include <stdio.h>
#endif

#define INCEDGE 1

#include <cfpio.h>
#include <compactr.h>

#include <internal.p>

// Following table is used to clip
static unsigned char Aclip[512] = { 0};
static unsigned char *clip;

unsigned char *
DTCONFIG_API1
initclip(const int& maxval) 
{
    clip = &Aclip[128];
    for (int i = -128; i < 0; i++)
	clip[i] = 0;
    for (i = 0; i < maxval; i++)
	clip[i] = i;
    for (; i < 512-128; i++)
	clip[i] = maxval;
    return clip;
}

static int Asq_decode_table[513] = { 0};
static int *sq_decode_table;
static int sq_range;
static int sq_out_of_range[256];
static int Adi_decode_table[513] = { 0};
static int *di_decode_table;
static int di_range;
static int di_out_of_range[256];

static
int prepare_decoder(const int& sq_quant_spacing, const int& di_quant_spacing)
// Sets up tables of representative levels to use in the current bands.
{
    int i;
    // First do the square band
    sq_decode_table = &Asq_decode_table[256];
    int rep_level = sq_quant_spacing + (sq_quant_spacing-1)/2;
    if (rep_level != sq_decode_table[1]){	// Already loaded
	sq_range = 255/sq_quant_spacing + 1;
	sq_decode_table[0] = 0;
	for (i = 1; i <= sq_range; i++, rep_level += sq_quant_spacing) {
	    sq_decode_table[i] = rep_level;
	    sq_decode_table[-i] = -rep_level;
	}
	for (i = 0; i < 256; i++)
	    {
	    sq_out_of_range[i] = (255-i)/sq_quant_spacing;
	    }
    }
    // Then the diamond band
    di_decode_table = &Adi_decode_table[256];
    rep_level = di_quant_spacing + (di_quant_spacing-1)/2;
    if (rep_level == di_decode_table[1])	// Already loaded
    	return 0;
    di_range = 255/di_quant_spacing + 1;
    di_decode_table[0] = 0;
    for (i = 1; i <= di_range; i++, rep_level += di_quant_spacing) {
	di_decode_table[i] = rep_level;
	di_decode_table[-i] = -rep_level;
    }
    for (i = 0; i < 256; i++)
	{
	di_out_of_range[i] = (255-i)/di_quant_spacing;
	}
    return 0;
}

static
void btpc_dec_level(unsigned char **out, const int& XMAX, const int& YMAX, 
const int& level, const int& sq_spacing, const int& di_spacing,
Compactr *codein)
{
    // printf("level %d\n", level); fflush(stdout);
    for (int ii = 0; ii < NUMCODERS; ii++)
	codein[ii].readtree();
    register int error;
    int X, Y;
    register int p,q,r,s, t, u, v;	
    // Surround pels
    register int estimate;
    register int elow = 0;
	register int ehigh = 0;
    int scratch;

    prepare_decoder(sq_spacing, di_spacing);
    const int sq_ridgethresh = 2*sq_spacing + 7;
    const int di_ridgethresh = 2*di_spacing + 7;
    const int XINTERVAL = 1 << level;
    const int YINTERVAL = 1 << level;
    const int XHINT = XINTERVAL >> 1;
    const int YHINT = YINTERVAL >> 1;
    const int XQINT = XHINT >> 1;
    const int YQINT = YHINT >> 1;
    for (Y = YHINT; Y < YMAX; Y += YINTERVAL)
    {
	// printf("Y = %d\n",Y); fflush(stdout);
	unsigned char *py = out[Y];
	int yplus = Y+YHINT;
	int yqplus = Y+YQINT;
	int yqminus = Y-YQINT;
	int yminus = Y-YHINT;
	int y2minus = Y-YINTERVAL;
	int y3minus = y2minus-YHINT;
	unsigned char *pyplus = out[yplus];
	unsigned char *pyqplus = out[yqplus];
	unsigned char *pyqminus = out[yqminus];
	unsigned char *pyminus = out[yminus];
	unsigned char *py2minus = out[y2minus];
	unsigned char *py3minus = out[y3minus];
	register int signtemp;
	register int st2;
	// register int ridgepel;
	int sq_zerotree = 0;
	int di_zerotree = 0;
	// Set up q and r and v for first row
	v = q = pyminus[0];
	if (yplus == YMAX)
	    r = q;
	else
	    r = pyplus[0];
        int nextscratch = -1;
	int xplus = XINTERVAL;
	int xminus = 0;
	for (X = XHINT; X < XMAX; X+=XINTERVAL, xplus+=XINTERVAL, xminus+=XINTERVAL) 
	    {
	    p = q;
	    s = r;
	    u = v;
	    if (xplus != XMAX) {
		q = pyminus[xplus];
		if (yplus == YMAX)
		    r = q;
		else
		    r = pyplus[xplus];
	    }
	    // Otherwise q and r are already correct
	register int fastpred = 3;
#define SQUARE 1
#include <macro.c>
#undef SQUARE
#define SQUARE_PROP 1
#include <macro.c>
#undef SQUARE_PROP
	    if (!yminus)
		t = p;
	    else
		t = py2minus[X];
#define DIAMOND_LEFT 1
#include <macro.c>
#undef DIAMOND_LEFT
#define DIAMOND_PROP_LEFT 1
#include <macro.c>
#undef DIAMOND_PROP_LEFT
#define DIAMOND_RIGHT 1
#include <macro.c>
#undef DIAMOND_RIGHT
#define DIAMOND_PROP_RIGHT 1
#include <macro.c>
#undef DIAMOND_PROP_RIGHT
	}
    }
}

void 
DTCONFIG_API1
btpc_dec_plane(
  unsigned char **out,
  const int& xmax, 
  const int& ymax,
  const int& fromlevel, 
  const int& tolevel, 
  const int *spacing,
  Compactr *codein, 
  const int& clipval) 
{
	int X, Y;
	// First get dpcm lowpass
	int predpel;
	int inc = 1 << fromlevel;
	codein[0].readtree();
	for (Y = 0; Y < ymax; Y += inc)
	for (X = 0; X < xmax; X += inc) {
		predpel =  (X ? out[Y][X-inc] : (Y ? out[Y-inc][X] : 0));
		int temp = codein[0].get() + predpel;
		if (temp < 0) temp += 256;
		if (temp > 255) temp -= 256;
		out[Y][X] = temp;
		}
	// Now do bands in pairs
	initclip(clipval);
	int band = 1;
	for (int level = fromlevel; level > tolevel; level--, band += 2) {
		btpc_dec_level(out,xmax,ymax,level,spacing[band],
			spacing[band+1], codein);
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
