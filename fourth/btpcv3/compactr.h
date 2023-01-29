/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
  ! Copyright (C) 1998 Dtack Inc. All Rights Reserved
  ! To use this file, you must have signed a license agreement with Dtack Inc.
  ! Under no circumstances may you redistribute this file.
  ! This software is provided AS IS with no warranty expressed or implied.
  ! Dtack Inc. accepts no liability for use or misuse of this file.
  ! http://www.dtack.com  dtack@dtack.com  telephone +360.417.6700
  ! Dtack Inc, P.O. Box 3061, Port Angeles, WA USA 98362
  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */




#define NUMCODERS (6)

typedef
  union
  {
	struct
	{
	  long f[256];
	  unsigned short enc[256];
	  unsigned char len[256];
	} enctables;
	struct 
	{
	  unsigned char dec[TABLESIZE];
	  unsigned char len[TABLESIZE];
	} dectables;
  } Compactr_shared_t;

class 
Compactr {
public:
  int constructor_failed;
  class cfpio *pfio;
  int longest;
  int longmask;
  unsigned char num_symbols_of_length[32];
  unsigned char ordered_symbols[256];
  
// The shared data area:
  
  Compactr_shared_t *shared;

// Next the stuff for encoding
	long *freq;
	unsigned short *encode;
	unsigned char *enclength;
// Then the stuff for decoding
	unsigned char *decode;
	unsigned char *declength;

	Compactr();
	~Compactr();
	void putbit(int val);
	int getbit();
	void reset();
	void attach(class cfpio *f);
	void inc(int symbol);
	void dec(int symbol);
	void buildtree(void);
	void writetree(void);
	void maketable_encode(void);
	void put(int inbyte);
	void readtree(void);
	void maketable_decode(void);
	int get();
};







/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
  ! Copyright (C) 1998 Dtack Inc. All Rights Reserved
  ! To use this file, you must have signed a license agreement with Dtack Inc.
  ! Under no circumstances may you redistribute this file.
  ! This software is provided AS IS with no warranty expressed or implied.
  ! Dtack Inc. accepts no liability for use or misuse of this file.
  ! http://www.dtack.com  dtack@dtack.com  telephone +360.417.6700
  ! Dtack Inc, P.O. Box 3061, Port Angeles, WA USA 98362
  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */
