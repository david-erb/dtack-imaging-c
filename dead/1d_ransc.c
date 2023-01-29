/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
  ! Copyright (C) 1998 Dtack Inc. All Rights Reserved
  ! To use this file, you must have signed a license agreement with Dtack Inc.
  ! Under no circumstances may you redistribute this file.
  ! This software is provided AS IS with no warranty expressed or implied.
  ! Dtack Inc. accepts no liability for use or misuse of this file.
  ! http://www.dtack.com  dtack@dtack.com  telephone +360.417.6700
  ! Dtack Inc, P.O. Box 3061, Port Angeles, WA USA 98362
  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */




/*..........................................................................
 * this random number generation code came from 
 * Dr.Dobbs Journal Feb 1992?
 *
 * seems to give one random bit at a time?
 *..........................................................................*/

_PSEUDO-RANDOM SEQUENCE GENERATOR FOR 32-BIT CPUs_
by Bruce Schneier


[LISTING ONE]


int RANDOM ()  {
    static dtntype_b32_t register; /*Register must be unsigned so right 
                                     shift works properly.*/
    /*Register should be initialized with some random value.*/
    register = ((((register >> 31) /*Shift the 32nd bit to the first bit*/
               ^ (register >> 6)   /*XOR it with the seventh bit*/
               ^ (register >> 4)   /*XOR it with the fifth bit*/
               ^ (register >> 2)   /*XOR it with the third bit*/
               ^ (register >> 1)   /*XOR it with the second bit*/
               ^ register)         /*and XOR it with the first bit.*/
               & 0x0000001)        /*Strip all the other bits off and*/
               <<31)               /*move it back to the 32nd bit.*/
               | (register >> 1);  /*Or with the register shifted right.*/
    return register & 0x00000001;  /*Return the first bit.*/
}




[LISTING TWO]


int VERYRANDOM ()  {
    static dtntype_b32_t regA, regB, regC;
    /*regA, regB, and regC should be initialized with some random value.*/
    regA = ((((regA>>31)^(regA>>6)^(regA>>4)^(regA>>2)^(regA<<1)^regA)
           & 0x00000001)<<31) | (regA>>1);
    regB = ((((regB>>30)^(regB>>2)) & 0x00000001)<<30) | (regB>>1);
    regC = ((((regC>>28)^(regC>>1)) & 0x00000001)<<28) | (regC>>1);
    /*regB is a 31-bit LFSR.  regC is a 29-bit LFSR.*/
    /*Both feedback sequences are chosen to be maximum length.*/
    return ((regA & regB) | (!regA & regC)) & 0x00000001;
    /*Above is equivalant to:  if A then return B else return C.*/
     /* Variants:  return ((regA & regB) | (regA & regC) | (regB & regC)) & 
       0x00000001; Above variant returns the majority of A, B, and C.
       return (regA ^ regB ^ regC) & 0x00000001;
       Above variant returns the XOR of A, B, and C.  */  
}







/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
  ! Copyright (C) 1998 Dtack Inc. All Rights Reserved
  ! To use this file, you must have signed a license agreement with Dtack Inc.
  ! Under no circumstances may you redistribute this file.
  ! This software is provided AS IS with no warranty expressed or implied.
  ! Dtack Inc. accepts no liability for use or misuse of this file.
  ! http://www.dtack.com  dtack@dtack.com  telephone +360.417.6700
  ! Dtack Inc, P.O. Box 3061, Port Angeles, WA USA 98362
  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */
