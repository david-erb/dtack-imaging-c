typedef struct dtmatrix_3d_t {			/* 3x3 matrix xform desc			*/
  double coeff[16];
} dtmatrix_3d_t;

typedef enum {							/* axis types						*/
  DTMATRIX_3D_AXIS_X,
  DTMATRIX_3D_AXIS_Y,
  DTMATRIX_3D_AXIS_Z
} dtmatrix_3d_axis_t;

typedef struct dtmatrix_2d_t {			/* 3x3 matrix xform desc			*/
  double coeff[9];
} dtmatrix_2d_t;

#include <dtack/matrix.p>










