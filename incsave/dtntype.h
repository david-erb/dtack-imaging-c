/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
  ! Copyright (C) 1998 Dtack Inc. All Rights Reserved
  ! To use this file, you must have signed a license agreement with Dtack Inc.
  ! Under no circumstances may you redistribute this file.
  ! This software is provided AS IS with no warranty expressed or implied.
  ! Dtack Inc. accepts no liability for use or misuse of this file.
  ! http://www.dtack.com  dtack@dtack.com  telephone +360.417.6700
  ! Dtack Inc, P.O. Box 3061, Port Angeles, WA USA 98362
  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */





typedef enum {
  DTNTYPE_NULL = 0,
  DTNTYPE_UNKNOWN = 1000,
  DTNTYPE_B01 = 1,					/* 1-bit binary						*/
  DTNTYPE_B08 = 2,					/* 8-bit unsigned binary			*/
  DTNTYPE_B16 = 3,					/* 16-bit unsigned binary			*/
  DTNTYPE_B32 = 4,					/* 32-bit unsigned binary			*/
  DTNTYPE_S1  = 5,					/* 8-bit signed integer			    */
  DTNTYPE_S2  = 6,					/* 16-bit signed integer		    */
  DTNTYPE_S4  = 7,					/* 32-bit signed integer		    */
  DTNTYPE_F1  = 8,					/* single precision float		    */
  DTNTYPE_F2  = 9					/* double precision float		    */
} dtntype_e;

#if defined(DTCONFIG_64BIT)				/* 64-bit architecture? 			*/
typedef unsigned int dtntype_b32_t;		/* 32-bit integer 					*/
typedef int dtntype_s4_t;
#else  									/* not 64-bit architecture? 		*/
typedef unsigned long dtntype_b32_t;	/* 32-bit integer 					*/
typedef long dtntype_s4_t;
#endif

typedef unsigned char  dtntype_b01_t; /* data types						*/
typedef unsigned char  dtntype_b08_t;
typedef unsigned short dtntype_b16_t;
typedef signed   char  dtntype_s1_t;
typedef signed   short dtntype_s2_t;
typedef float          dtntype_f1_t;
typedef double         dtntype_f2_t;

#define DTNTYPE_CASES								\
  CASE(DTNTYPE_B01,dtntype_b01_t)					\
  CASE(DTNTYPE_B08,dtntype_b08_t)					\
  CASE(DTNTYPE_B16,dtntype_b16_t)					\
  CASE(DTNTYPE_B32,dtntype_b32_t)					\
  CASE(DTNTYPE_S1,dtntype_s1_t)						\
  CASE(DTNTYPE_S2,dtntype_s2_t)						\
  CASE(DTNTYPE_S4,dtntype_s4_t)						\
  CASE(DTNTYPE_F1,dtntype_f1_t)						\
  CASE(DTNTYPE_F2,dtntype_f2_t)


#define DTNTYPE_ISFLOAT(NTYPE) \
  ((NTYPE)==DTNTYPE_F1 || \
   (NTYPE)==DTNTYPE_F2)

#define DTNTYPE_ISSIGNED(NTYPE) \
  ((NTYPE)==DTNTYPE_S1 || \
   (NTYPE)==DTNTYPE_S2 || \
   (NTYPE)==DTNTYPE_S4 || \
   (NTYPE)==DTNTYPE_F1 || \
   (NTYPE)==DTNTYPE_F2)
							
#define DTNTYPE_GET1(CHECK, CTL, 	                                          \
          P, I, NTYPE, VALUE)				 	 							  \
  {																			  \
    switch(NTYPE)															  \
    {																		  \
	  case DTNTYPE_B01:													  \
	     *(VALUE) = !!(((dtntype_b01_t *)(P))[(I)>>3]&(0x80>>((I)&7))); 	  \
  	  break;																  \
	  case DTNTYPE_B08:													  \
	    *(VALUE) = ((dtntype_b08_t *)(P))[(I)];						  	  \
  	  break;																  \
	  case DTNTYPE_B16:													  \
	    *(VALUE) = ((dtntype_b16_t *)(P))[(I)];						  	  \
  	  break;																  \
	  case DTNTYPE_B32:													  \
	    *(VALUE) = ((dtntype_b32_t *)(P))[(I)];						  	  \
	  break;																  \
	  case DTNTYPE_S1:														  \
	    *(VALUE) = ((dtntype_s1_t *)(P))[(I)];						  	   	  \
	  break;																  \
	  case DTNTYPE_S2:														  \
	    *(VALUE) = ((dtntype_s2_t *)(P))[(I)];								  \
	  break;																  \
	  case DTNTYPE_S4:														  \
	    *(VALUE) = ((dtntype_s4_t *)(P))[(I)];								  \
	  break;																  \
	  case DTNTYPE_F1:														  \
	    *(VALUE) = ((dtntype_f1_t *)(P))[(I)];								  \
	  break;																  \
	  case DTNTYPE_F2:														  \
	    *(VALUE) = ((dtntype_f2_t *)(P))[(I)];								  \
	  break;																  \
    }																		  \
  }

										/* ................................ */
#define DTNTYPE_SET1(CHECK, CTL, 	                                          \
          P, I, NTYPE, VALUE)				 	 							  \
  {																			  \
    switch(NTYPE)															  \
    {																		  \
	  case DTNTYPE_B01:													  \
	  {																		  \
        if ((VALUE))														  \
  	      ((dtntype_b01_t *)(P))[(I)>>3] |= 0x0080 >> ((I)&7);				  \
        else																  \
    	  ((dtntype_b01_t *)(P))[(I)>>3] &= 0xff7f >> ((I)&7);				  \
	  }																		  \
	  break;																  \
	  case DTNTYPE_B08:													  \
	    ((dtntype_b08_t *)(P))[(I)] = (dtntype_b08_t)(VALUE);				  \
	  break;																  \
	  case DTNTYPE_B16:													  \
	    ((dtntype_b16_t *)(P))[(I)] = (dtntype_b16_t)(VALUE);				  \
	  break;																  \
	  case DTNTYPE_B32:													  \
	    ((dtntype_b32_t *)(P))[(I)] = (dtntype_b32_t)(VALUE);				  \
	  break;																  \
	  case DTNTYPE_S1:														  \
	    ((dtntype_s1_t *)(P))[(I)] = (dtntype_s1_t)(VALUE);				  \
	  break;																  \
	  case DTNTYPE_S2:														  \
	    ((dtntype_s2_t *)(P))[(I)] = (dtntype_s2_t)(VALUE);				  \
	  break;																  \
	  case DTNTYPE_S4:														  \
	    ((dtntype_s4_t *)(P))[(I)] = (dtntype_s4_t)(VALUE);				  \
	  break;																  \
	  case DTNTYPE_F1:														  \
	    ((dtntype_f1_t *)(P))[(I)] = (dtntype_f1_t)(VALUE);				  \
	  break;																  \
      case DTNTYPE_F2:														  \
	    ((dtntype_f2_t *)(P))[(I)] = (dtntype_f2_t)(VALUE);				  \
  	  break;																  \
    }																		  \
  }

										/* ................................ */
#define DTNTYPE_SETS(CHECK, CTL,       	                                  \
          P, I1, I2, NTYPE, VALUE)			 	 							  \
  {																			  \
    unsigned int dt_tmp_i;	    											  \
    const unsigned int dt_tmp_i2 = DT_CAST(unsigned int, (I2));               \
    switch(NTYPE)															  \
    {																		  \
	  case DTNTYPE_B01:													  \
        if ((VALUE))														  \
          for (dt_tmp_i=(I1); dt_tmp_i<dt_tmp_i2; dt_tmp_i++)                 \
  	        ((dtntype_b01_t *)(P))[dt_tmp_i>>3] |= 0x0080>>(dt_tmp_i&7);     \
          else																  \
          for (dt_tmp_i=(I1); dt_tmp_i<dt_tmp_i2; dt_tmp_i++)                 \
    	    ((dtntype_b01_t *)(P))[dt_tmp_i>>3] &= 0xff7f>>(dt_tmp_i&7);     \
	  break;																  \
	  case DTNTYPE_B08:													  \
        for (dt_tmp_i=(I1); dt_tmp_i<dt_tmp_i2; dt_tmp_i++)                   \
	      ((dtntype_b08_t *)(P))[dt_tmp_i] = (dtntype_b08_t)(VALUE);   	  \
	  break;																  \
	  case DTNTYPE_B16:													  \
        for (dt_tmp_i=(I1); dt_tmp_i<dt_tmp_i2; dt_tmp_i++)                   \
	      ((dtntype_b16_t *)(P))[dt_tmp_i] = (dtntype_b16_t)(VALUE);	  	  \
	  break;																  \
	  case DTNTYPE_B32:													  \
        for (dt_tmp_i=(I1); dt_tmp_i<dt_tmp_i2; dt_tmp_i++)                   \
	      ((dtntype_b32_t *)(P))[dt_tmp_i] = (dtntype_b32_t)(VALUE);	  	  \
	  break;																  \
	  case DTNTYPE_S1:														  \
        for (dt_tmp_i=(I1); dt_tmp_i<dt_tmp_i2; dt_tmp_i++)                   \
	      ((dtntype_s1_t *)(P))[dt_tmp_i] = (dtntype_s1_t)(VALUE);	  	  \
	  break;																  \
	  case DTNTYPE_S2:														  \
        for (dt_tmp_i=(I1); dt_tmp_i<dt_tmp_i2; dt_tmp_i++)                   \
	      ((dtntype_s2_t *)(P))[dt_tmp_i] = (dtntype_s2_t)(VALUE);	  	  \
	  break;																  \
	  case DTNTYPE_S4:														  \
        for (dt_tmp_i=(I1); dt_tmp_i<dt_tmp_i2; dt_tmp_i++)                   \
	      ((dtntype_s4_t *)(P))[dt_tmp_i] = (dtntype_s4_t)(VALUE);	  	  \
	  break;																  \
	  case DTNTYPE_F1:														  \
        for (dt_tmp_i=(I1); dt_tmp_i<dt_tmp_i2; dt_tmp_i++)                   \
	      ((dtntype_f1_t *)(P))[dt_tmp_i] = (dtntype_f1_t)(VALUE);	      \
	  break;																  \
      case DTNTYPE_F2:														  \
        for (dt_tmp_i=(I1); dt_tmp_i<dt_tmp_i2; dt_tmp_i++)                   \
	      ((dtntype_f2_t *)(P))[dt_tmp_i] = (dtntype_f2_t)(VALUE);	 	  \
  	  break;																  \
    }																		  \
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
