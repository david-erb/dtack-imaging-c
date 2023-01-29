/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
  ! Copyright (C) 1998 Dtack Inc. All Rights Reserved
  ! To use this file, you must have signed a license agreement with Dtack Inc.
  ! Under no circumstances may you redistribute this file.
  ! This software is provided AS IS with no warranty expressed or implied.
  ! Dtack Inc. accepts no liability for use or misuse of this file.
  ! http://www.dtack.com  dtack@dtack.com  telephone +360.417.6700
  ! Dtack Inc, P.O. Box 3061, Port Angeles, WA USA 98362
  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */




#define ent sayood_ent
#define image_size sayood_image_size
#define readimage sayood_readimage
#define encuqi sayood_encuqi
#define decuqi sayood_decuqi
#define stuffit sayood_stuffit
#define readnbits sayood_readnbits
#define uquan_dec sayood_uquan_dec
#define uquan_enc sayood_uquan_enc
#define nuquan_dec sayood_nuquan_dec
#define nuquan_enc sayood_nuquan_enc
#define rangen sayood_rangen
#define vqencode sayood_vqencode
#define unstuff sayood_unstuff
#define readau sayood_readau
#define get_file_size sayood_get_file_size
#define predictor sayood_predictor

#define getim sayood_getim
#define storeim sayood_storeim
#define sort sayood_sort
#define huff sayood_huff
#define diff sayood_diff
#define getcode sayood_getcode
#define value sayood_value
#define images sayood_images
#define files sayood_files

#define create_list sayood_list
#define create_code sayood_code

#define make_list sayood_list
#define write_code sayood_code

										/* ................................ */
float ent(float [], int );
int image_size(char [], int *, int *);
void readimage(char [], unsigned char **, int, int);
int encuqi(int, int *, int);
int decuqi(int, int *);
void stuffit(int , int , FILE *, int );
int readnbits(int , FILE *);
int uquan_enc(float ,  float , int , float );
float uquan_dec(int,  float, float );
int nuquan_enc(float ,  float [], int);
float nuquan_dec(int,  float [] );
extern float rangen(int);
int vqencode( int *, int **,  int, int,float *);
/* vqencode(input,codebook,codebook_size,dimension,&distortion)
returns the index in the codebook of the closest codeword */
void unstuff(int , FILE *, int *, int *);
int readau(char filename[], short aufile[]);
int get_file_size(char [], int *);
float predictor(int ,int ,float [],float [], float *);


void getim(char *fname, unsigned char *array, int size);
void storeim(char *fname, unsigned char *array, int size);
void sort(float *prob, int *loc,int num);
void huff(float prob[], int loc[], int num, unsigned int *code, char *length);
void diff(int *diffs, unsigned char *image, int rows, int cols,int num, unsigned char *dimage);
void getcode(FILE *fp, int num, unsigned int *code, char *length);
void value(int *values, unsigned char *image, int size,int num);
void images(int rows, int cols, int *code,char *length, unsigned char *file, unsigned char *dimage);
int files(int size,int *code,char *length,unsigned char *file);

/* define the structure node */

        struct node {
                float pro; /* probabilities */
                int l; /* location of probability before sorted */
                unsigned int code; /* code */
                struct node *left; /* pointer for binary tree */
                struct node *right; /* pointer for binary tree */
                struct node *forward;
                struct node *back;
                struct node *parent; /* pointer to parent */
                int check;
        };

/* define subroutines and pointers to nodes */

typedef struct node NODE;
typedef struct NODE *BTREE;
BTREE create_list(float prob[], int loc[], int num);
void create_code(NODE *root, int lgth, unsigned int *code, char *length);

BTREE make_list (int num);
void write_code(NODE *root, int lgth, unsigned int *code, char *length, int *loc);

   


#define     myabs(x) ((x)<0? -(x): (x))





/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
  ! Copyright (C) 1998 Dtack Inc. All Rights Reserved
  ! To use this file, you must have signed a license agreement with Dtack Inc.
  ! Under no circumstances may you redistribute this file.
  ! This software is provided AS IS with no warranty expressed or implied.
  ! Dtack Inc. accepts no liability for use or misuse of this file.
  ! http://www.dtack.com  dtack@dtack.com  telephone +360.417.6700
  ! Dtack Inc, P.O. Box 3061, Port Angeles, WA USA 98362
  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */
