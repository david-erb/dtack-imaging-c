/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
  ! Copyright (C) 1998 Dtack Inc. All Rights Reserved
  ! To use this file, you must have signed a license agreement with Dtack Inc.
  ! Under no circumstances may you redistribute this file.
  ! This software is provided AS IS with no warranty expressed or implied.
  ! Dtack Inc. accepts no liability for use or misuse of this file.
  ! http://www.dtack.com  dtack@dtack.com  telephone +360.417.6700
  ! Dtack Inc, P.O. Box 3061, Port Angeles, WA USA 98362
  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */




#include <dtack/base.h>
DT_RCSID("source/high $RCSfile: xchg_w.c,v $ $Revision: 1.1 $");

#include <dtack/lut8.h>
#include <dtack/xchg.h>

#include <dtack/os.h>
#include <dtack/str.h>
  
#define RETURN(ERR, MSG, SUB)			\
{										\
  if (err != NULL)						\
    *err = (ERR);						\
  if (message != NULL && lmax > 0) 		\
    dtstr_printf(ctl, message, lmax,	\
      (MSG), (SUB));					\
  return DT_RC_GOOD;					\
}

/*..........................................................................
| NAME
| dtxchg_parse() - parse external image spec
| 
| SUMMARY
| Return no error message if ~lmax~ is zero or ~message~ is NULL.
| Return no error number if ~err~ is NULL.
| Otherwise, return a non-empty error message and a non-zero error number 
| in the following situations:
| :DTXCHG_ERR_NOSPEC:		~spec~ is NULL
| :DTXCHG_ERR_EMPTYSPEC:	~spec~ has zero length
| :DTXCHG_ERR_NOSERVER:		the first character of ~spec~ is a colon and 
|							~ctl~ is NULL or ~ctl->win~ is NULL
| :DTXCHG_ERR_BADSUFFIX:	~spec~ has an unrecognized suffix
| :DTXCHG_ERR_LONGTYPE:		~spec~ has an overly long type part
| :DTXCHG_ERR_BADTYPE:		~spec~ has an unrecognized type part
| :DTXCHG_ERR_AMBTYPE:		~spec~ has an ambiguous type part
|
|.Syntax Name Tables
| Use this table to return ~which~ from a type string.
| type string		~which~ returned
| pgm				:DTXCHG_PGM:
| pbm				:DTXCHG_PBM:
| 
| Use this table to return ~which~ from a suffix.
| suffix			~which~ returned
| .pgm				:DTXCHG_PGM:
| .pbm				:DTXCHG_PBM:
| 
| END
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtxchg_parse(							/* parse external image spec		*/
  dt_ctl_t *ctl,						/* environment control 				*/
  const char *spec,						/* external image spec				*/
  dtxchg_e *which,						/* returned exchanger type 			*/
  char **server,						/* returned server part 			*/
  char **id,							/* returned image identifier 		*/
  dt_err_t *err,						/* returned error code 				*/
  char *message,						/* returned error message 			*/
  int lmax)								/* maximum length of error message 	*/
{
  DT_F("dtxchg_parse");
  const char *xchg_s = "";
  int xchg_l = 0;
  const char *server_s = "";
  int server_l = 0;
  const char *id_s;
  int id_l;
  const char *p;
  int choice;
  char tmp[33];
# define COLON ':'
# define SEP ','
# define EOS '\0'
# define DOT '.'

  if (message != NULL &&				/* in case nothing bad happens 		*/
	  lmax > 0)
    *message = '\0';
  if (err != NULL)
    *err = DT_ERR_NONE;

  if (spec == NULL)						/* NULL spec? 						*/
	RETURN(
      DTXCHG_ERR_NOSPEC,
      "Sorry, no file"
      " or window name was given.", 
      NULL);
  
  if (strlen(spec) == 0)				/* zero-length spec? 				*/
	RETURN(
      DTXCHG_ERR_EMPTYSPEC,
      "Sorry, a zero-length file"
      " or window name was given.",
      NULL);

  if (spec[0] == COLON)					/* spec begins with a colon?	    */
  {
	if (ctl == NULL ||
		ctl->win == NULL)
	{
	  dt_dbg(ctl, F, DT_DBG_MASK_ANY, "ctl=0x%08lx, ctl->win=\"%s\"",
			 ctl,
             (ctl!=NULL && ctl->win!=NULL)? ctl->win: "NULL");

	  RETURN(
        DTXCHG_ERR_NOSERVER,
        "Sorry, no window server has"
        " been specified to use with"
        " \"%s\".",					
        spec);
	}

    xchg_s = ctl->win;					/* get server from param 			*/
    p = strchr(xchg_s, SEP);
    if (p != NULL)						/* win param has commas?		   	*/
    {
      xchg_l = (int)(p - xchg_s);
      server_s = p + 1;					/* server is all after first comma	*/
      server_l = strlen(xchg_s) -
        xchg_l - 1;
    }
    else								/* win param has no commas?	    	*/
      xchg_l = strlen(xchg_s);			/* it's all in the xchanger type	*/
    id_s = spec + 1;
  }
  else									/* spec does not begin with colon?	*/
  if ((p=strchr(spec, SEP)) != NULL)	/* spec has commas in it?			*/
  {
    xchg_s = spec;
    xchg_l = (int)(p - xchg_s);
    server_s = p + 1;
    id_s = strrchr(server_s, SEP);
    if (id_s != NULL)
    {
      server_l = (int)(id_s-server_s);	/* server is between first and last	*/
      id_s++;							/* id is all after last comma	    */
    }
    else								/* only one comma in whole thing?	*/
      id_s = server_s;
  }
  else									/* no comma anywhere?				*/
  {
    char *suffix;
    id_s = spec;						/* it all goes to the id		    */
    suffix = strrchr(id_s, DOT);		/* check the suffix				    */
    if (suffix != NULL)					/* there is a suffix?			    */
	{
      suffix += 1;						/* skip the DOT					    */
      DT_Q(dtstr_choice,(ctl,			/* choose suffix					*/
        dtxchg_suffixes, suffix,
        &choice));
      if (choice < -1)					/* ambiguous?					    */
        RETURN(
          DTXCHG_ERR_BADSUFFIX,
          "Sorry, suffix on \"%s\""
          " is ambiguous.",
          spec)
      else
      if (choice >= 0)					/* known suffix?				    */
	  {
		xchg_s = dtxchg_choices			/* select exchanger string			*/
          [dtxchg_suffixes_which[choice]];
        xchg_l = strlen(xchg_s);
	  }
	}
  }
  id_l = strlen(id_s);

  if (xchg_l > (int)sizeof(tmp)-1)
    RETURN(
      DTXCHG_ERR_LONGTYPE,
      "Sorry, the type part of \"%s\""
      " is too long.",
      spec);
    
										/* ................................ */
  strncpy(tmp, xchg_s, xchg_l);
  tmp[xchg_l] = '\0';
  DT_Q(dtstr_choice2,(ctl,				/* choose exchanger				    */
    dtxchg_choices, tmp, &choice,
    message, lmax));

  if (choice < 0)						/* invalid choice?				    */
  {
    if (choice == -1)
      RETURN(
        DTXCHG_ERR_BADTYPE,
        "Sorry, type \"%s\""
        " is not one of the allowed"
        " choices.",
        tmp)
    else
      RETURN(
        DTXCHG_ERR_AMBTYPE,
        "Sorry, type \"%s\""
        " ambiguous.",
        tmp);
  }

  *which = (dtxchg_e)choice;			/* return choice index			    */

  if (server)
  {
    DT_Q(dtos_malloc2,(ctl,
      (void **)server, server_l+1,
      F, "server"));
    DT_MEMCPY(*server, server_s,	
      server_l);
    (*server)[server_l] = '\0';
  }

  if (id)
  {
    DT_Q(dtos_malloc2,(ctl,
      (void **)id, id_l+1,
      F, "id"));
    DT_MEMCPY(*id, id_s, id_l);
    (*id)[id_l] = '\0';
  }


  return DT_RC_GOOD;
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
