
/*..........................................................................
  CHANGES
  20000411 DE fixed bug not processing last column or row
 *..........................................................................*/

/* depends on, for example:
#define NTYPE_T dt_ntype_b08_t
#define NTYPE_E DT_NTYPE_B08
#define NTYPE_M b08
*/

  dtxy_t x, y, c, r;
  const dtxy_t xe = input->xe;
  const dtxy_t ye = input->ye;
  const int kx2 = kx / 2;
  const int ky2 = ky / 2;
  const dtxy_t xe2 = xe - kx + 1;		/* columns actually processed 		*/
  const dtxy_t ye2 = ye - ky + 1;		/* rows actually processed 			*/
  const int n2 = (kx * ky) / 2;			/* the median point 				*/
  NTYPE_T a_dynamic[64];				/* maybe save a malloc 				*/
  NTYPE_T *a = NULL;
  unsigned int a_size = kx * ky *		/* size needed for sort array		*/
    sizeof(NTYPE_T);
  dt_rc_e rc = DT_RC_GOOD;

  if (a_size > sizeof(a_dynamic))
    DT_C(dtos_malloc2,(ctl, 			/* space for sort array elements 	*/
      (void **)&a, a_size,
      F, "sort array"))
	else
	  a = (NTYPE_T *)a_dynamic;

  for (y=0; y<ye2; y++)
  {
	NTYPE_T * const q =					/* row we are outputting 			*/
      output->row[y+ky2].NTYPE_M;
    for (x=0; x<xe2; x++)				/* for each output pixel 			*/
	{
	  int n = 0;
	  const dtxy_t r2 = y + ky;			/* end row of window 				*/
	  for (r=y; r<r2; r++)
	  {
		const NTYPE_T * const p = 
          input->row[r].NTYPE_M;
		const dtxy_t c2 = x + kx;
	    for (c=x; c<c2; c++)
          a[n++] = p[c];
	  }
	  dt1d_sort(NTYPE_E, a, n);
	  q[x+kx2] = a[n2];
	}
  }

cleanup:

  if (a != NULL && 
      a != a_dynamic)
    DT_I(dtos_free2,(ctl, a,
	  F, "sort array"));

  return rc;
