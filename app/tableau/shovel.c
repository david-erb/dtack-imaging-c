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
| USAGE
| shovel
| 
| END
 *..........................................................................*/

#include <tableau.h>
DT_RCSID("app/tableau $RCSfile: shovel.c,v $ $Revision: 1.8 $");
#include <dtack/yield.h>

#define CR 0x0d
#define LF 0x0a

/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
tableau_shovel(
  dtparam_t *param,
  const char *dgserver_spec,
  tableau_get_t *get,
  const char *cook_spec,
  const char *encode_spec,
  tableau_put_t *put)
{
  DT_F("tableau_shovel");
  dt_ctl_t *ctl = &param->ctl;
  tableau_t _tableau, *tableau = &_tableau;
  dtdg_server_t _dgserver, *dgserver = &_dgserver;
  dtipc_t _cmdipc;
  dtipc_t *cmdipc = NULL;
  int cmdipc_connected = 0;
  long n = DT_CAST(long, param->n);
  long i;
  tableau_header_t *dheader = NULL;
  long dl = 0;
  void *ddata = NULL;
  tableau_header_t _eheader,
    *eheader = &_eheader;
  long el = 0;
  void *edata = NULL;

  tableau_decode_f *decode = NULL;
  tableau_cook_f *cook = NULL;
  tableau_encode_f *encode = NULL;

  void *mess = NULL;
  tableau_decode_arg_t *decode_arg;
  tableau_cook_arg_t *cook_arg;
  tableau_encode_arg_t *encode_arg;
  dtimage_t *input;
  dtimage_t *cooked;
  
  long start = 0;
  int cc;

  int mess_size;
  dt_color_org_t org;
  dt_rc_e rc;
  
  DTCSTACK_HIGHWATER;

  memset(tableau, 0, sizeof(*tableau));
  tableau->param = param;
  tableau->ctl = ctl;
  tableau->get = get;
  tableau->put = put;

  dgserver->initialized = 0;

  if (dgserver_spec != NULL)			/* we should be a server? 			*/
  {
	DT_C(dtipc_register,(ctl,
      &_cmdipc, dgserver_spec, 0, 0));
	cmdipc = &_cmdipc;
  }

  mess_size = TABLEAU_CC_N * (
        sizeof(*decode_arg)+
        sizeof(*cook_arg)+
        sizeof(*encode_arg)+
        sizeof(*input)+
        sizeof(*cooked));

  DT_C(dtos_malloc2,(ctl, &mess,
    mess_size, F, "mess"));

  memset(mess, 0, mess_size);
  decode_arg = (tableau_decode_arg_t *)mess;
  cook_arg = (tableau_cook_arg_t *)(decode_arg+TABLEAU_CC_N);
  encode_arg = (tableau_encode_arg_t *)(cook_arg+TABLEAU_CC_N);
  input = (dtimage_t *)(encode_arg+TABLEAU_CC_N);
  cooked = (dtimage_t *)(input+TABLEAU_CC_N);
  
  for (cc=0; cc<TABLEAU_CC_N; cc++)
  {
	encode_arg[cc].quality = 
      DT_CAST(int, param->t);
	cook_arg[cc].spec = cook_spec;
  }

  DT_C(tableau_cook_from_name,(			/* look up cook function 			*/
    param, cook_spec, &cook));

  DT_C(tableau_encode_from_name,(		/* look up encode function 			*/
    param, encode_spec, &encode));

  for (i=0; n==0 || i<n; i++)
  {
	dtos_time_t beg, end;

	DT_C(dt_yield,(ctl, "grabbing"));
	DT_C(dtos_time,(ctl, &beg));

	DT_C((get->function),(param,		/* fetch current image and switch	*/
      get, &dheader, &ddata, &dl));
	if (get->partner_stopped)
	  goto cleanup;

	switch(dheader->org)
	{
	  case DT_COLOR_ORG_LUMINANCE: 
	  case DT_COLOR_ORG_YCBCR: 
	  case DT_COLOR_ORG_RGB888: 
        cc = TABLEAU_CC_Y;
	  break;
	  case DT_COLOR_ORG_CB: 
        cc = TABLEAU_CC_CB;
	  break;
	  case DT_COLOR_ORG_CR: 
        cc = TABLEAU_CC_CR;
	  break;
	  default:
		rc = dt_err(ctl, F,
		  "unexpected dheader->org %d",
          dheader->org);
		goto cleanup;
    }

										/* ................................ */
	DT_C(dt_yield,(ctl, "decoding"));

    DT_C(tableau_decode_from_name,(		/* look up decoder function 		*/
      param, dheader->codec, &decode));
	
	if (encode != NULL ||				/* need an image to encode? 		*/
		put->function == tableau_put_scr) /* or need it to display? 			*/
	{
	  DT_C((decode),(param,				/* decode the input image			*/
        &decode_arg[cc], dheader, ddata,
        &input[cc]));
	}

	DT_C(dt_color_org_set,(ctl, &org,	/* make org structure 				*/
      DT_CAST(dt_color_org_e,
        dheader->org)));

										/* ................................ */
	DT_C(dt_yield,(ctl, "cooking"));

	if (encode != NULL)
	  DT_C((cook),(param,				/* cook the input					*/
        &cook_arg[cc],
        &input[cc], &org, &cooked[cc]));

	DT_C(tableau_demo,(ctl,				/* put demo notification in			*/
      &cooked[cc], 
      (cooked[cc].ye-16)/2,
      &org, 
      "Tableau Demo"));

										/* ................................ */
	DT_C(dt_yield,(ctl, "encoding"));

    *eheader = *dheader;				/* copy of what we got				*/
	if (encode == NULL)
	  edata = ddata;
	else
  	  DT_C((encode),(param,				/* encode the cooked image			*/
        &encode_arg[cc], &cooked[cc],
        &org,
        eheader, &edata, &el));
	
										/* ................................ */
	DT_C(dt_yield,(ctl, "size %ld",
      eheader->l));

	DT_C((put->function),(param,		/* output the cooked image			*/
      put, 
      encode==NULL? 
        &input[cc]: &cooked[cc],
      eheader, edata, cc));
	if (put->partner_stopped)
	  goto cleanup;
	if (start == 0)
	  start = eheader->ms;

										/* ................................ */
	if (put->type == TABLEAU_PUT_ROLL)
	  DT_C(dt_yield,(ctl, "file %s",
        put->u.roll.name))
	else
	  DT_C(dt_yield,(ctl, "done"));

	DT_C(dtos_time,(ctl, &end));

    dt_dbg(ctl, NULL, DT_DBG_MASK_USR2,
      "%6ld. %8s %6ld %4.1f sec",
      dheader->n,
      dheader->cam,
      eheader->l,
      end.wall-beg.wall);

	tableau->last_frame = dheader->n;
	tableau->last_size = eheader->l;
	tableau->last_time = end.wall-beg.wall;
	strcpy(tableau->last_cam, dheader->cam);

	get->shoveled_kb += dheader->l / 1000;		/* count bytes shoveled 			*/
	get->shoveled_frames++;
	put->shoveled_kb += eheader->l / 1000;
	put->shoveled_frames++;
	
	DT_C(dt_yield,(ctl, "time %0.1f",
      end.wall-beg.wall));

	if (cmdipc != NULL)					/* we are doing special ipc?		   */
	{
	  int isready;
	  if (!cmdipc_connected)
	  {
		DT_C(dtipc_poll,(ctl, cmdipc,	/* poll for connection 				*/
          DTIPC_POLL_ACCEPT, 0,
          "cmdipc client", &isready));
		if (isready)					/* client is connecting? 			*/
		{
		  DT_C(dtipc_wait,(ctl,			/* accept connection 				*/
            cmdipc));
		  cmdipc_connected = 1;
		}
	  }
	  while (cmdipc_connected)			/* a client is connected? 			*/
	  {
		char cmd[128];
		int l;
		char c[32];
		DT_C(dtipc_poll,(ctl, cmdipc,	/* poll for data 					*/
          DTIPC_POLL_READ, 0,
          "cmdipc read", &isready));
		if (!isready)					/* no data ready? 					*/
		  break;

		DT_G(dtipc_read_line,(ctl,		/* read line of data 				*/
          cmdipc, cmd, sizeof(cmd)));
		if (rc != DT_RC_GOOD ||			/* read failed? 					*/
			strlen(cmd) == 0)			/* or no data? 						*/
		{
		  DT_G(dtipc_done,(ctl,			/* close connection to client 		*/
            cmdipc));
		  cmdipc_connected = 0;
		  break;
		}

		l = strlen(cmd);
		while (l > 0 &&					/* nuke the crlf's 					*/	
			   cmd[l-1] == LF ||
			   cmd[l-1] == CR)
		  cmd[--l] = '\0';			
		
		dt_dbg(ctl, NULL, DT_DBG_MASK_COMMAND, 
          "cmd=\"%s\"", cmd);
		if (!strcmp(cmd, "quality-"))
		{
		  param->t -= 5;
		  param->t = DT_MAX(param->t, 5);
		}
		else
		if (!strcmp(cmd, "quality+"))
		{
		  param->t += 5;
		  param->t = DT_MIN(param->t, 95);
		}
		else
		if (!strcmp(cmd, "stop"))
		{
		  rc = DT_RC_STOP;
		  goto cleanup;
		}
		else
		if (!strncmp(cmd, "source,", 7))
		{
		  DT_G(dtfb_source,(ctl, 
            &get->u.fb.fb, cmd+7));
		}
		else
		  rc = dt_err(ctl, F,
            "command \"%s\" unrecognized",
            cmd);
		dtstr_printf(ctl, c, sizeof(c),
          "%d\r\n\r\n", rc);
		DT_G(dtipc_write_all,(ctl, cmdipc, c, strlen(c)));
		param->t = DT_MIN(param->t, 100);
		encode_arg[0].quality = param->t;

	  }
	}
	if (dgserver->initialized)			/* we are be a dgserver? 			*/
	{
	  DT_C(tableau_poll,(tableau,		/* poll for command 				*/
        dgserver, param->dt));
	  encode_arg[0].quality = param->t;
	}
	else
	  DT_C(dtos_delay,(ctl, param->dt));
  }

cleanup:
  if (cmdipc_connected)					/* a client is connected? 			*/
	DT_I(dtipc_done,(ctl, cmdipc));		/* close connection to client 		*/

  if (cmdipc != NULL)					/* we are doing special ipc?		   */
	DT_I(dtipc_unregister,(ctl,
      cmdipc));

  for (cc=0; cc<TABLEAU_CC_N; cc++)
  {
    dt_dbg(ctl, F, DT_DBG_MASK_COMPLEX, 
      "(encode) 0x%08lx", encode);
    if (encode != NULL)
    DT_I((encode),(param, &encode_arg[cc],
      NULL, NULL, eheader, &edata, &el));

    dt_dbg(ctl, F, DT_DBG_MASK_COMPLEX, 
      "(cook) 0x%08lx", cook);
    if (cook != NULL)
    DT_I((cook),(param, &cook_arg[cc],
      NULL, NULL, &cooked[cc]));

    dt_dbg(ctl, F, DT_DBG_MASK_COMPLEX, 
      "(decode) 0x%08lx", decode);
    if (decode != NULL)
    DT_I((decode),(param, &decode_arg[cc],
      NULL, NULL, &input[cc]));
  }
  
  if (mess != NULL)
    DT_I(dtos_free2,(ctl, mess,
      F, "mess"));

  if (dgserver->initialized)
  DT_I(dtdg_server_uninit,(ctl,			/* quit being a datagram server 	*/
    dgserver));

  DT_I(tableau_get_common_free,(		/* free common stuff 				*/
    param, get,
    &dheader, &ddata, &dl));

  return rc;
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
