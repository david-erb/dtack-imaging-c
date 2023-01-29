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
  CHANGES
  20000203 DE created
 *..........................................................................*/


/*..........................................................................
| include $RCSfile: cipher.h,v $ $Revision: 1.2 $
 *..........................................................................*/


#define DTCIPHER_PASSWORD_MAX (512)		/* max expected password 			*/
#define DTCIPHER_KEYSIZE_MAX (512)		/* max expected key size 			*/
#define DTCIPHER_BLOCKSIZE_MAX (512)	/* max expected block size 			*/

typedef struct {
  char password1[DTCIPHER_PASSWORD_MAX];
  char password2[DTCIPHER_PASSWORD_MAX];
  char key[DTCIPHER_KEYSIZE_MAX];		/* generated key					*/
  char iv[DTCIPHER_KEYSIZE_MAX];		/* random vector 					*/
  int mode;
  int algorithm;
  char *name;
  int keysize;
  int blocksize;
  int initialized;
} dtcipher_t;

#define DTCIPHER_SIG "dtcipher"

typedef struct {
  char sig[10];
  dtntype_b32_t rev;
  dtntype_b32_t algorithm;
  dtntype_b32_t mode;
  char pad;
} dtcipher_header_t;

#include <dtack/cipher.p>


/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
  ! Copyright (C) 1998 Dtack Inc. All Rights Reserved
  ! To use this file, you must have signed a license agreement with Dtack Inc.
  ! Under no circumstances may you redistribute this file.
  ! This software is provided AS IS with no warranty expressed or implied.
  ! Dtack Inc. accepts no liability for use or misuse of this file.
  ! http://www.dtack.com  dtack@dtack.com  telephone +360.417.6700
  ! Dtack Inc, P.O. Box 3061, Port Angeles, WA USA 98362
  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */
