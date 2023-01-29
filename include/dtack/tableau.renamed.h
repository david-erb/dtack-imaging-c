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
| CHANGES
| 2000-12-21 DE added allocated_size, revision, and extra fields to master structure
 *..........................................................................*/

typedef dtntype_s4_t tableau_len_t;

#define TABLEAU_HEADER_CAMNAME_MAX (16)
typedef struct {
  tableau_len_t l;						/* length of encoded data 			*/
  tableau_len_t n;						/* frame sequence number 			*/
  tableau_len_t ms;						/* millisecond at acquire 			*/
  tableau_len_t xe;						/* size of decoded image 			*/
  tableau_len_t ye;
  tableau_len_t org;
  char cam[TABLEAU_HEADER_CAMNAME_MAX];
  char codec[16];
} tableau_header_t;

#define TABLEAU_CODEC_UNCOMPRESSED		"uncompressed"

#define TABLEAU_MASTER_FLAG_REPAINT (0x00000001)
#define TABLEAU_MASTER_FLAG_NOPAINT (0x00000002)
#define TABLEAU_MASTER_FLAG_WRITING (0x00000004)
#define TABLEAU_MASTER_FLAG_CANCELLED (0x00000008)
#define TABLEAU_MASTER_FLAG_TERMINATE (0x00000010)
#define TABLEAU_MASTER_FLAG_HFLIP (0x00000020)
#define TABLEAU_MASTER_FLAG_VFLIP (0x00000040)

#define TABLEAU_MASTER_REVISION_0201 (0x00020001)
#define TABLEAU_MASTER_REVISION_0202 (0x00020002)
#define TABLEAU_MASTER_REVISION_CURRENT TABLEAU_MASTER_REVISION_0202

typedef struct {
  tableau_len_t allocated_size;			/* total size of the structure		*/
  tableau_len_t revision;				/* structure revision code 		    */
  tableau_len_t extra;					/* extra bytes per sample			*/
  tableau_len_t allocated;				/* total samples allocated			*/
  tableau_len_t total;					/* total samples loaded				*/
  tableau_len_t current;				/* current sample number 			*/
  tableau_len_t rate;					/* frame advancement rate			*/
  tableau_len_t armed;					/* armed and rolling grabbing		*/
  tableau_len_t countdown;				/* countdown til done grabbing		*/
  tableau_len_t flags;
  tableau_len_t expansion[100];
  tableau_header_t header;
} tableau_master_t;




/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
  ! Copyright (C) 1998 Dtack Inc. All Rights Reserved
  ! To use this file, you must have signed a license agreement with Dtack Inc.
  ! Under no circumstances may you redistribute this file.
  ! This software is provided AS IS with no warranty expressed or implied.
  ! Dtack Inc. accepts no liability for use or misuse of this file.
  ! http://www.dtack.com  dtack@dtack.com  telephone +360.417.6700
  ! Dtack Inc, P.O. Box 3061, Port Angeles, WA USA 98362
  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */
