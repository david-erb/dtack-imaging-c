/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
  ! Copyright (C) 1998 Dtack Inc. All Rights Reserved
  ! To use this file, you must have signed a license agreement with Dtack Inc.
  ! Under no circumstances may you redistribute this file.
  ! This software is provided AS IS with no warranty expressed or implied.
  ! Dtack Inc. accepts no liability for use or misuse of this file.
  ! http://www.dtack.com  dtack@dtack.com  telephone +360.417.6700
  ! Dtack Inc, P.O. Box 3061, Port Angeles, WA USA 98362
  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */




#define BTPC_SIG3 "btpc 3."				/* file header signature 			*/

typedef enum {
  BTPC_RC_GOOD = 0,
  BTPC_RC_BADSIG,
  BTPC_RC_BADHEAD,
  BTPC_RC_BADLEVELS,
  BTPC_RC_BADCODERS,
  BTPC_RC_BADVERSION,
  BTPC_RC_BADCOMPONENTS,
  BTPC_RC_BADSIZE,
  BTPC_RC_BADQUALITY,
  BTPC_RC_BADMALLOC
} btpc_rc_e;

#ifdef __cplusplus
extern "C" {
#endif

int
DTCONFIG_API1
btpc_decode_issig(
  char *b,								/* buffer containing header			*/
  int length);							/* buffer length					*/

btpc_rc_e
DTCONFIG_API1
btpc_decode_header(
  char *b,								/* buffer containing header			*/
  int length,							/* buffer length					*/
  int *cols, 
  int *rows, 
  int *maxval, 
  int *pnmtype, 
  int *numlevels, 
  char *coltype, 
  char *uvfilter, 
  int *spacing,
  int *got);

btpc_rc_e
DTCONFIG_API1
btpc_decode(
  char *b,								/* buffer containing data only		*/
  int length,							/* buffer length					*/
  int xe, 
  int ye, 
  int maxval, 
  int pnmtype, 
  int numlevels, 
  char coltype, 
  char uvfilter, 
  int *spacing,
  unsigned char ***rows);

btpc_rc_e
DTCONFIG_API1
btpc_encode(
  unsigned char ***rows,
  int xe, 
  int ye, 
  int maxval, 
  int pnmtype, 
  int quality,
  char *b,		
  int length,
  int *used);

#ifdef __cplusplus
}
#endif





/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
  ! Copyright (C) 1998 Dtack Inc. All Rights Reserved
  ! To use this file, you must have signed a license agreement with Dtack Inc.
  ! Under no circumstances may you redistribute this file.
  ! This software is provided AS IS with no warranty expressed or implied.
  ! Dtack Inc. accepts no liability for use or misuse of this file.
  ! http://www.dtack.com  dtack@dtack.com  telephone +360.417.6700
  ! Dtack Inc, P.O. Box 3061, Port Angeles, WA USA 98362
  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */
