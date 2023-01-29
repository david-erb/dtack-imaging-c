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
// Helper routines for encoding, plus main encoding function
//
// Note: I have not made any attempt to optimize the encoding. The place to start
// would be the Huffman statistics gathering phase, which could be integrated
// with the formation of the binary tree.
//
// Copyright (C) John Robinson, 1994, 1995.
//
#define INCEDGE 1
#define ENCODER 1
#ifndef DTDEPEND
#include <stdio.h>
#include <string.h>
#endif

#include <cfpio.h>

#include <compactr.h>

#include <internal.p>

static int Asq_encode_table[513] = { 0};
static int *sq_encode_table;
static int Asq_decode_table[513] = { 0};
static int *sq_decode_table;
static int sq_range;
static int Adi_encode_table[513] = { 0};
static int *di_encode_table;
static int Adi_decode_table[513] = { 0};
static int *di_decode_table;
static int di_range;

static
void prepare_encoder(const int& sq_quant_spacing, const int& di_quant_spacing)
// Sets up tables of representative levels to use in the current bands.
{
    int i;
    // First do the square band
    sq_range = 255/sq_quant_spacing + 1;
    sq_encode_table = &Asq_encode_table[256];
    for (i = 1; i < 256; i++) {
	sq_encode_table[i] = i/sq_quant_spacing;
	sq_encode_table[-i] = -i/sq_quant_spacing;
	}
    sq_decode_table = &Asq_decode_table[256];
    int rep_level = sq_quant_spacing + (sq_quant_spacing-1)/2;
    sq_decode_table[0] = 0;
    for (i = 1; i <= sq_range; i++, rep_level += sq_quant_spacing) {
	sq_decode_table[i] = rep_level;
	sq_decode_table[-i] = -rep_level;
    	}
    // Then the diamond band
    di_range = 255/di_quant_spacing + 1;
    di_encode_table = &Adi_encode_table[256];
    for (i = 1; i < 256; i++) {
	di_encode_table[i] = i/di_quant_spacing;
	di_encode_table[-i] = -i/di_quant_spacing;
	}
    di_decode_table = &Adi_decode_table[256];
    rep_level = di_quant_spacing + (di_quant_spacing-1)/2;
    di_decode_table[0] = 0;
    for (i = 1; i <= di_range; i++, rep_level += di_quant_spacing) {
	di_decode_table[i] = rep_level;
	di_decode_table[-i] = -rep_level;
    	}
    }
static unsigned char **pyr;
static unsigned char **ridge;

int
DTCONFIG_API1
makescratch(
  const int& xmax, 
  const int& ymax)
// Make space for scratch images. Assume we don't run out of space.
	{
	pyr = new unsigned char *[ymax];
	ridge = new unsigned char *[ymax];
	for (int i = 0; i < ymax; i++)
		{
		pyr[i] = new unsigned char[xmax];
		ridge[i] = new unsigned char[xmax];
		}
	return 0;
	}
int 
DTCONFIG_API1
deletescratch(
  const int& ymax)
	{
	for (int i = 0; i < ymax; i++) {
	    delete [] pyr[i];
	    delete [] ridge[i];
	    }
	delete [] pyr;
	delete [] ridge;
	return 0;
	}

void 
DTCONFIG_API1
btpc_enc_level(
  unsigned char **in,
  unsigned char **pyr,
  unsigned char **ridge, 
  const int& XMAX, 
  const int& YMAX, 
  const int& level, 
  const int& sq_spacing, 
  const int& di_spacing, 
  const int& maxval)
{
    register int error;
    int code;
    register int prepel;
    int X, Y;
    register int ridgepel;
    register int p,q,r,s, t, u, v;	
    // Surround pels
    register int estimate;
    register int elow = 0;
	register int ehigh = 0;
    prepare_encoder(sq_spacing, di_spacing);

    const int sq_ridgethresh = 2*sq_spacing + 7;
    const int di_ridgethresh = 2*di_spacing + 7;
    const int XINTERVAL = 1 << level;
    const int YINTERVAL = 1 << level;
    const int XHINT = XINTERVAL >> 1;
    const int YHINT = YINTERVAL >> 1;
    const int XQINT = XHINT >> 1;
    const int YQINT = YHINT >> 1;
    //printf("level = %d\n",level);
    for (Y = YHINT; Y < YMAX; Y += YINTERVAL)
    {
	//printf("Y = %d\n",Y);
	int yplus = Y+YHINT;
	int yqplus = Y+YQINT;
	int yqminus = Y-YQINT;
	int yminus = Y-YHINT;
	int y2minus = Y-YINTERVAL;
	int y3minus = y2minus-YHINT;
	unsigned char *py = in[Y];
	unsigned char *pyplus = in[yplus];
	unsigned char *pyqplus = in[yqplus];
	unsigned char *pyqminus = in[yqminus];
	unsigned char *pyminus = in[yminus];
	unsigned char *py2minus = in[y2minus];
	unsigned char *py3minus = in[y3minus];
	register int signtemp;
	register int st2;
	// Set up q and r and v for first row
	v = q = pyminus[0];
	if (yplus == YMAX)
	    r = q;
	else
	    r = pyplus[0];
	for (X = XHINT; X < XMAX; ) {
	    int xplus = X+XHINT;
	    int xminus = X-XHINT;
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
	    if (!yminus)
		t = p;
	    else
		t = py2minus[X];
	    prepel = py[X];
	    ridgepel = 0;
	    register int fastpred = 3;
#define SQUARE 1
#include <macro.c>
#undef SQUARE
	    py[X] = prepel;
	    pyr[Y][X] = code;
	    ridge[Y][X] = ridgepel;
	    v = prepel;
	    prepel = pyminus[X];
	    ridgepel = 0;
#define DIAMOND_LEFT 1
#include <macro.c>
#undef DIAMOND_LEFT
	    pyminus[X] = prepel;
	    pyr[yminus][X] = code;
	    ridge[yminus][X] = ridgepel;
	    prepel = py[xminus];
	    ridgepel = 0;
#define DIAMOND_RIGHT 1
#include <macro.c>
#undef DIAMOND_RIGHT
	    py[xminus] = prepel;
	    pyr[Y][xminus] = code;
	    ridge[Y][xminus] = ridgepel;
	    X += XINTERVAL;
	    }
	}
    }

inline int count_next_char(unsigned char& outval, unsigned char& scratchval, 
	Compactr *codeout, int zerotree_offset)
	{
	int retval = 0;
	if (!scratchval)
		;
	else if (scratchval == 1)
		{
		if (!outval) {
			(codeout+zerotree_offset)->inc(131);
			retval = 2;
			}
		else {
			(codeout+zerotree_offset)->inc(130);
			(codeout+1)->inc(outval);
			}
		}
	else
		{
		// output simple codeword
		codeout += zerotree_offset;
		if ((outval >= 130)&&(outval <= 132))
		// got to escape the word
			codeout->inc(132);
		else
			codeout->inc(outval);
		}
	return retval;
	}
inline int put_next_char(unsigned char& outval, unsigned char& scratchval, 
	Compactr *codeout, int zerotree_offset)
	{
	int retval = 0;
	if (!scratchval)
		;
	else if (scratchval == 1)
		{
		if (!outval) {
			(codeout+zerotree_offset)->put(131);
			retval = 2;
			}
		else {
			(codeout+zerotree_offset)->put(130);
			(codeout+1)->put(outval);
			}
		}
	else
		{
		codeout += zerotree_offset;
		if (outval == 130) {
		// got to escape the word
			codeout->put(132);
			codeout->putbit(0);
			}
		else if (outval == 131) {
			codeout->put(132);
			codeout->putbit(1);
			codeout->putbit(0);
			}
		else if (outval == 132) {
		// got to escape the word
			codeout->put(132);
			codeout->putbit(1);
			codeout->putbit(1);
			}
		else
		// output simple codeword
			codeout->put(outval);
		}
	return retval;
	}

//
// bincode works from the finest band upwards, chunking zeros up the binary
// tree until a non-zero entry is reached, which is marked as a leaf. It
// then works down the tree, Huffman coding each band.
//
static
int bincode(unsigned char **pyr, unsigned char **ridge, 
	const int& inrows, const int& incols, 
	Compactr *codeout, unsigned char **scratch, int tolevel)
        {
	int x, y;
	int pel;
	int rpel;
	// Flags on scratch image interpreted as follows:
	// 0: Beyond end of a tree - don't code
	// 1: Tree terminator
	// 2: Ordinary
	const int rows = inrows;
	const int cols = incols;
	for (y = 0; y < rows; y++)
		memset(scratch[y],2,cols);
	// First blank out highest pass
	int yodd = 0;
	for (y = 0; y < rows; y++, yodd ^= 1)
	    {
	    for (x = (yodd? 0:1); x < cols; x += 2)
		{
		if (tolevel) {
			scratch[y][x] = 0;
			continue;
			}
		pel = pyr[y][x];
		if (pel == 0)
			scratch[y][x] = 1;
		}
	    }
	// Now first mpass
	for (y = 1; y < rows; y += 2)
	for (x = 1; x < cols; x += 2)
		{
		if (tolevel){
			scratch[y][x] = 1;
			pyr[y][x] = 0;
			continue;
			}
		if (scratch[y-1][x] == 1)
		    {
		    if (scratch[y][x-1] == 1)
			{
			scratch[y][x] = 1;
			scratch[y-1][x] = 0;
			scratch[y][x-1] = 0;
			}
		    }
		    
		}
	// Now remainder of bands
	for (int i = 2; i <= 8; i *= 2)
		{
		int twoi = i<<1;
		int halfi = i>>1;
		yodd = 0;
		for (y = 0; y < rows; y += i, yodd ^= 1)
		    {
		    int lchild = yodd? halfi : -halfi;
		    for (x = (yodd? 0: i); x < cols; x += twoi)
			  {
			  if ((scratch[y+halfi][x+lchild] == 1)&&
				(pyr[y+halfi][x+lchild] == 0))
			    {
			    if ((scratch[y+halfi][x+lchild+i] == 1)&&
				(pyr[y+halfi][x+lchild+i] == 0))
				{
				scratch[y][x]  = 1;
			  	scratch[y+halfi][x+lchild] = 0;
			   	scratch[y+halfi][x+lchild+i] = 0;
				}
			    }
			  }
		    }
		for (y = i; y < rows; y += twoi)
		for (x = i; x < cols; x += twoi)
			{
			  if((scratch[y-i][x] == 1)&&(pyr[y-i][x] == 0))
			    {
			    if((scratch[y][x-i] == 1)&&(pyr[y][x-i] == 0))
				{
				scratch[y][x] = 1;
			  	scratch[y-i][x] = 0;
			    	scratch[y][x-i] = 0;
				}
			    }
			}
		}
	// Finally, change all the terminators in the highest band back
	// to zeros
	yodd = 0;
	for (y = 0; y < rows; y++, yodd ^= 1)
	    {
	    for (x = (yodd? 0:1); x < cols; x += 2)
		{
		if (scratch[y][x] == 1)
			scratch[y][x] = 2;
		}
	    }
// For now, just going to do everything twice to form Huffman trees
	// DPCM code the low band
	int predpel;
	for (y = 0; y < rows; y += 16)
	for (x = 0; x < cols; x += 16)
		{
		predpel =  (x ? pyr[y][x-16] : (y ? pyr[y-16][x] : 0));
		int temp = pyr[y][x] - predpel;
		// Ensure wrapping round
		if (temp < -128) temp += 256;
		if (temp > 127) temp -= 256;
		codeout[0].inc(scratch[y][x] = (unsigned char) temp);
		}
	codeout[0].buildtree();
	codeout[0].writetree();
	for (y = 0; y < rows; y += 16)
	for (x = 0; x < cols; x += 16)
		codeout[0].put((unsigned char) scratch[y][x]);
	codeout[0].reset();
	// Now rest of levels
	int sq_zerotree = 0;
	int di_zerotree = 0;
	for (i = 4; i > 1; i--)
		{
		// printf("Level = %d\n", i);
		int hoffset = 1<<(i-1);
		int offset = 1<<i;
		for (y = hoffset; y < rows; y += offset)
		    {
		    int xodd = 0;
		    for (x = hoffset; x < cols; x += offset, xodd ^= 1)
			{
			if (!scratch[y][x])
				continue;
			sq_zerotree = count_next_char(pyr[y][x], scratch[y][x], 
				&codeout[0], xodd ? sq_zerotree : 0);
			di_zerotree = count_next_char(pyr[y-hoffset][x],
				scratch[y-hoffset][x], &codeout[3], 0);
			count_next_char(pyr[y][x-hoffset],scratch[y][x-hoffset],
				&codeout[3], di_zerotree);
			}
		    }
		for (int j = 0; j < NUMCODERS; j++) {
			codeout[j].buildtree();
			codeout[j].writetree();
			}
		for (y = hoffset; y < rows; y += offset)
		    {
		    // printf("y = %d\n",y);
		    int xodd = 0;
		    for (x = hoffset; x < cols; x += offset, xodd ^= 1)
			{
			sq_zerotree = put_next_char(pyr[y][x], scratch[y][x], 
				&codeout[0], xodd ? sq_zerotree : 0);
			if ((rpel = ridge[y][x]) != 0)
				codeout[0].putbit(rpel - 2);
			di_zerotree = put_next_char(pyr[y-hoffset][x],
				scratch[y-hoffset][x], &codeout[3], 0);
			if ((rpel = ridge[y-hoffset][x]) != 0)
				codeout[0].putbit(rpel - 2);
			put_next_char(pyr[y][x-hoffset],scratch[y][x-hoffset],
				&codeout[3], di_zerotree);
			if ((rpel = ridge[y][x-hoffset]) != 0)
				codeout[0].putbit(rpel - 2);
			}
		    }
		for (j = 0; j < NUMCODERS; j++) 
			codeout[j].reset();
		}
	if (tolevel)
		return 0;
	// Now deal with last level - For the finest band only, the leaf codeword
	// on the left sibling means that both siblings are zero.
	int hoffset = 1;
	int offset = 2;
	for (y = hoffset; y < rows; y += offset)
	    {
	    int xodd = 0;
	    for (x = hoffset; x < cols; x += offset, xodd ^= 1)
		{
		sq_zerotree = count_next_char(pyr[y][x], scratch[y][x], 
			&codeout[0], xodd ? sq_zerotree : 0);
		count_next_char(pyr[y-hoffset][x],
			scratch[y-hoffset][x], &codeout[3], 0);
		if (pyr[y-hoffset][x])
		    count_next_char(pyr[y][x-hoffset],scratch[y][x-hoffset],
			&codeout[3], 0);
		else
		    count_next_char(pyr[y][x-hoffset],scratch[y][x-hoffset],
			&codeout[4], 0);
		}
	    }
	for (int j = 0; j < NUMCODERS; j++) {
		codeout[j].buildtree();
		codeout[j].writetree();
		}
	for (y = hoffset; y < rows; y += offset)
	    {
	    int xodd = 0;
	    for (x = hoffset; x < cols; x += offset, xodd ^= 1)
		{
		sq_zerotree = put_next_char(pyr[y][x], scratch[y][x], 
			&codeout[0], xodd ? sq_zerotree : 0);
		if ((rpel = ridge[y][x]) != 0)
			codeout[0].putbit(rpel - 2);
		put_next_char(pyr[y-hoffset][x],
			scratch[y-hoffset][x], &codeout[3], 0);
		if ((rpel = ridge[y-hoffset][x]) != 0)
			codeout[0].putbit(rpel - 2);
		if (pyr[y-hoffset][x])
		    put_next_char(pyr[y][x-hoffset],scratch[y][x-hoffset],
			&codeout[3], 0);
		else
		    put_next_char(pyr[y][x-hoffset],scratch[y][x-hoffset],
			&codeout[4], 0);
		if ((rpel = ridge[y][x-hoffset]) != 0)
			codeout[0].putbit(rpel - 2);
		}
	    }
	for (j = 0; j < NUMCODERS; j++) 
		codeout[j].reset();
	return 0;
	}

void 
DTCONFIG_API1
btpc_enc_plane(
  unsigned char **in, 
  const int& xmax, 
  const int& ymax,
  const int& fromlevel, 
  const int& tolevel, 
  const int *spacing, 
  Compactr *codeout, 
  const int& clipval) 
{
	// First put lowband into pyr
	for (int y = 0; y < ymax; y += 16)
	for (int x = 0; x < xmax; x += 16)
		pyr[y][x] = in[y][x];
	// Do bands in pairs
	int band = 1;
	for (int level = fromlevel; level > tolevel; level--, band += 2)
	{
		btpc_enc_level(in,pyr,ridge,xmax,ymax,level,spacing[band],
			spacing[band+1], clipval);
	}
	bincode(pyr, ridge, ymax, xmax, codeout, in, tolevel);
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
