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
  19981208 DE added to the library
 *..........................................................................*/

/* before including, define the following like:
   define KOP <
   define KTYPE dt_ntype_s4_t
   define KTYPE_E DT_NTYPE_S4
 */

  dtxy_t xe, ye;
  KTYPE *promoted = NULL;
  KTYPE *p[3];
  KTYPE *q, *tmp;
  void *v;
  int voff;
  dtxy_t x, y;
  dt_rc_e rc;

  rc = dtimage_check_1f(ctl,			/* tracer						    */
    input, "input",
    dt1d_ntype_list,
    DTIMAGE_FLAG_PAGED, F);
  if (rc != DT_RC_GOOD)
    return rc;

  rc = dtimage_check_1f(ctl,			/* tracer						    */
    output, "output",
    dt1d_ntype_list,
    DTIMAGE_FLAG_PAGED, F);
  if (rc != DT_RC_GOOD)
    return rc;

  xe = DT_MIN(input->xe, output->xe);
  ye = DT_MIN(input->ye, output->ye);

  voff = DT_CAST(int,					/* skip dead pixels on left output 	*/
    dt_ntype_sizeof[output->ntype]);

  DT_C(dtos_malloc2,(ctl,				/* space for the promoted rows 		*/
    (void **)&promoted,
    4 * xe * sizeof(KTYPE),
    F, "promoted rows"));

  p[0] = promoted + 0 * xe;				/* distribute the promoted rows 	*/
  p[1] = promoted + 1 * xe;
  p[2] = promoted + 2 * xe;
  q    = promoted + 3 * xe;

  DTIMAGE_GETROW2(DT_C, ctl,			/* prime the input pipe 			*/
    input, 0, &v);
  dt1d_cast(v, xe, input->ntype,
    p[0], xe, KTYPE_E);
  DTIMAGE_GETROW2(DT_C, ctl,
    input, 1, &v);
  dt1d_cast(v, xe, input->ntype,
    p[1], xe, KTYPE_E);
  for (y=1; y<ye-1; y++)				/* loop for input rows 				*/
  {
    DTIMAGE_GETROW2(DT_C, ctl,			/* get next input row 				*/
      input, y+1, &v);
	dt1d_cast(v, xe, input->ntype,		/* promote 							*/
      p[2], xe, KTYPE_E);
	{
  	  const KTYPE * const p0 = p[0];
	  const KTYPE * const p1 = p[1];
	  const KTYPE * const p2 = p[2];
      for (x=1; x<xe-1; x++)			/* for all input columns 			*/
	  {
        KTYPE              m = p0[x-1];
        if (m KOP p0[x])   m = p0[x];
        if (m KOP p0[x+1]) m = p0[x+1];
        if (m KOP p1[x-1]) m = p1[x-1];
        if (m KOP p1[x])   m = p1[x];
        if (m KOP p1[x+1]) m = p1[x+1];
        if (m KOP p2[x-1]) m = p2[x-1];
        if (m KOP p2[x])   m = p2[x];
        if (m KOP p2[x+1]) m = p2[x+1];
	    q[x-1] = m;
	  }
	}
	DTIMAGE_GETROW2(DT_C, ctl,			/* prepare to output 				*/
      output, y, &v);
	dt1d_cast(q, xe-2, KTYPE_E,			/* demote output 					*/
      (char *)v+voff, xe-2,
      output->ntype);
	DTIMAGE_PUTROW2(DT_C, ctl,			/* copy to output 					*/
      input, y);
	DTIMAGE_UNMAPROW2(DT_C, ctl,		/* release oldest row 				*/
      input, y-1);
    tmp = p[0];
    p[0] = p[1];						/* shuffle rows down 				*/
    p[1] = p[2];
    p[2] = tmp;
  }
  DTIMAGE_UNMAPROW2(DT_C, ctl,			/* release last rows 				*/
    input, y-1);
  DTIMAGE_UNMAPROW2(DT_C, ctl,
    input, y);
  
cleanup:
  if (promoted != NULL)					/* space for the promoted rows		*/
    DT_I(dtos_free2,(ctl,
      promoted,
      F, "promoted rows"));

  return rc;



/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
  ! Copyright (C) 1998 Dtack Inc. All Rights Reserved
  ! To use this file, you must have signed a license agreement with Dtack Inc.
  ! Under no circumstances may you redistribute this file.
  ! This software is provided AS IS with no warranty expressed or implied.
  ! Dtack Inc. accepts no liability for use or misuse of this file.
  ! http://www.dtack.com  dtack@dtack.com  telephone +360.417.6700
  ! Dtack Inc, P.O. Box 3061, Port Angeles, WA USA 98362
  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */
