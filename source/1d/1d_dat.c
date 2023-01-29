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
| NAME
| Introduction - intro
| 
| DESCRIPTION
| As yet undocumented.
| 
| RETURN VALUES
| Function return value will be DT1D_RC_GOOD on success.  
| Any other return value indicates failure or noncompletion.
| When DT1D_RC_GOOD is not returned, any other return values may not be valid.
| 
| ERRORS
| As yet undocumented.
| 
| END
 *..........................................................................*/


#include <dtack/config.h>				/* platform-specific defines		*/
#include <dtack/ntype.h>
#include <dtack/stats.h>
#include <dtack/1d.h>

static unsigned int square_table[512] = {
65535u, 65025u, 64516u, 64009u, 63504u, 63001u, 62500u, 62001u, 
61504u, 61009u, 60516u, 60025u, 59536u, 59049u, 58564u, 58081u, 
57600u, 57121u, 56644u, 56169u, 55696u, 55225u, 54756u, 54289u, 
53824u, 53361u, 52900u, 52441u, 51984u, 51529u, 51076u, 50625u, 
50176u, 49729u, 49284u, 48841u, 48400u, 47961u, 47524u, 47089u, 
46656u, 46225u, 45796u, 45369u, 44944u, 44521u, 44100u, 43681u, 
43264u, 42849u, 42436u, 42025u, 41616u, 41209u, 40804u, 40401u, 
40000u, 39601u, 39204u, 38809u, 38416u, 38025u, 37636u, 37249u, 
36864u, 36481u, 36100u, 35721u, 35344u, 34969u, 34596u, 34225u, 
33856u, 33489u, 33124u, 32761u, 32400u, 32041u, 31684u, 31329u, 
30976u, 30625u, 30276u, 29929u, 29584u, 29241u, 28900u, 28561u, 
28224u, 27889u, 27556u, 27225u, 26896u, 26569u, 26244u, 25921u, 
25600u, 25281u, 24964u, 24649u, 24336u, 24025u, 23716u, 23409u, 
23104u, 22801u, 22500u, 22201u, 21904u, 21609u, 21316u, 21025u, 
20736u, 20449u, 20164u, 19881u, 19600u, 19321u, 19044u, 18769u, 
18496u, 18225u, 17956u, 17689u, 17424u, 17161u, 16900u, 16641u, 
16384u, 16129u, 15876u, 15625u, 15376u, 15129u, 14884u, 14641u, 
14400u, 14161u, 13924u, 13689u, 13456u, 13225u, 12996u, 12769u, 
12544u, 12321u, 12100u, 11881u, 11664u, 11449u, 11236u, 11025u, 
10816u, 10609u, 10404u, 10201u, 10000u,  9801u,  9604u,  9409u, 
 9216u,  9025u,  8836u,  8649u,  8464u,  8281u,  8100u,  7921u, 
 7744u,  7569u,  7396u,  7225u,  7056u,  6889u,  6724u,  6561u, 
 6400u,  6241u,  6084u,  5929u,  5776u,  5625u,  5476u,  5329u, 
 5184u,  5041u,  4900u,  4761u,  4624u,  4489u,  4356u,  4225u, 
 4096u,  3969u,  3844u,  3721u,  3600u,  3481u,  3364u,  3249u, 
 3136u,  3025u,  2916u,  2809u,  2704u,  2601u,  2500u,  2401u, 
 2304u,  2209u,  2116u,  2025u,  1936u,  1849u,  1764u,  1681u, 
 1600u,  1521u,  1444u,  1369u,  1296u,  1225u,  1156u,  1089u, 
 1024u,   961u,   900u,   841u,   784u,   729u,   676u,   625u, 
  576u,   529u,   484u,   441u,   400u,   361u,   324u,   289u, 
  256u,   225u,   196u,   169u,   144u,   121u,   100u,    81u, 
   64u,    49u,    36u,    25u,    16u,     9u,     4u,     1u, 
    0u,     1u,     4u,     9u,    16u,    25u,    36u,    49u, 
   64u,    81u,   100u,   121u,   144u,   169u,   196u,   225u, 
  256u,   289u,   324u,   361u,   400u,   441u,   484u,   529u, 
  576u,   625u,   676u,   729u,   784u,   841u,   900u,   961u, 
 1024u,  1089u,  1156u,  1225u,  1296u,  1369u,  1444u,  1521u, 
 1600u,  1681u,  1764u,  1849u,  1936u,  2025u,  2116u,  2209u, 
 2304u,  2401u,  2500u,  2601u,  2704u,  2809u,  2916u,  3025u, 
 3136u,  3249u,  3364u,  3481u,  3600u,  3721u,  3844u,  3969u, 
 4096u,  4225u,  4356u,  4489u,  4624u,  4761u,  4900u,  5041u, 
 5184u,  5329u,  5476u,  5625u,  5776u,  5929u,  6084u,  6241u, 
 6400u,  6561u,  6724u,  6889u,  7056u,  7225u,  7396u,  7569u, 
 7744u,  7921u,  8100u,  8281u,  8464u,  8649u,  8836u,  9025u, 
 9216u,  9409u,  9604u,  9801u, 10000u, 10201u, 10404u, 10609u, 
10816u, 11025u, 11236u, 11449u, 11664u, 11881u, 12100u, 12321u, 
12544u, 12769u, 12996u, 13225u, 13456u, 13689u, 13924u, 14161u, 
14400u, 14641u, 14884u, 15129u, 15376u, 15625u, 15876u, 16129u, 
16384u, 16641u, 16900u, 17161u, 17424u, 17689u, 17956u, 18225u, 
18496u, 18769u, 19044u, 19321u, 19600u, 19881u, 20164u, 20449u, 
20736u, 21025u, 21316u, 21609u, 21904u, 22201u, 22500u, 22801u, 
23104u, 23409u, 23716u, 24025u, 24336u, 24649u, 24964u, 25281u, 
25600u, 25921u, 26244u, 26569u, 26896u, 27225u, 27556u, 27889u, 
28224u, 28561u, 28900u, 29241u, 29584u, 29929u, 30276u, 30625u, 
30976u, 31329u, 31684u, 32041u, 32400u, 32761u, 33124u, 33489u, 
33856u, 34225u, 34596u, 34969u, 35344u, 35721u, 36100u, 36481u, 
36864u, 37249u, 37636u, 38025u, 38416u, 38809u, 39204u, 39601u, 
40000u, 40401u, 40804u, 41209u, 41616u, 42025u, 42436u, 42849u, 
43264u, 43681u, 44100u, 44521u, 44944u, 45369u, 45796u, 46225u, 
46656u, 47089u, 47524u, 47961u, 48400u, 48841u, 49284u, 49729u, 
50176u, 50625u, 51076u, 51529u, 51984u, 52441u, 52900u, 53361u, 
53824u, 54289u, 54756u, 55225u, 55696u, 56169u, 56644u, 57121u, 
57600u, 58081u, 58564u, 59049u, 59536u, 60025u, 60516u, 61009u, 
61504u, 62001u, 62500u, 63001u, 63504u, 64009u, 64516u, 65025u};


unsigned int *dt1d_square_table = square_table + 256;






/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
  ! Copyright (C) 1998 Dtack Inc. All Rights Reserved
  ! To use this file, you must have signed a license agreement with Dtack Inc.
  ! Under no circumstances may you redistribute this file.
  ! This software is provided AS IS with no warranty expressed or implied.
  ! Dtack Inc. accepts no liability for use or misuse of this file.
  ! http://www.dtack.com  dtack@dtack.com  telephone +360.417.6700
  ! Dtack Inc, P.O. Box 3061, Port Angeles, WA USA 98362
  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */
