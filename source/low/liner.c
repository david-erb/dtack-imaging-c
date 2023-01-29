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
  20010104 DE fix error checking for crlf on end of line (purify)
  20000109 DE treat lines ending in backslash as having continuation
  19990723 DE new functions to keep blank and comment lines
 *..........................................................................*/


/*..........................................................................
| TITLE dtliner()
|dxinclude(liblink1.dx)
| 
|!SUMMARY
| Works together to read lines out of text files.
| Uses the :fd: Function Group.
| Writing and reading works for sockets as well as regular files.
|
| The :typedef struct dtliner_t: data structure used by all these functions
| contains no public members.
|
|.Salient Concepts
| You initialize the ~fd~ separately.
| You then use the liner functions.
| Under no circumstances should you change any members.
| 
| END
 *..........................................................................*/

#include <dtack/base.h>
DT_RCSID("source/low $RCSfile: fd.c,v $ $Revision: 1.2 $");
#include <dtack/os.h>
#include <dtack/list.h>
#include <dtack/liner.h>

#define CR 0x0d
#define LF 0x0a

/*..........................................................................
| NAME
| dtliner_line - get next line from file 
| 
| SUMMARY
| Read next line from file.
| Allocate enough space at ~*line~ to hold entire line.
| Rely on the caller to free this space with dtos_free().
| Remove the newline character from the end of the string.
| Removed CR characters only if immediately preceding a LF or EOF.
| If a backslash immediately precedes CR/LF or LF, add next line as continuation.
| If a backslash immediately precedes CR/EOF, ignore it.
| Return ~line~ set to NULL when the end of file is reached
| or an error happens.
| 
| END
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtliner_fd_line(						/* get next line from file			*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtfd_t *fd,							/* file object descriptor 			*/
  char **line)							/* return line contents 			*/
{
  DT_F("dtliner_fd_line");
  char *s = NULL;
  int max = 0;
  dt_rc_e rc = DT_RC_GOOD;

  while(1)
  {
	char *t = s;
	int l;
	max += 128;							/* initial size and size increment	*/
	DT_C(dtos_malloc2,(ctl,				/* get space for bigger string 		*/
      (void **)&s, max,
      F, "line"));
	if (t != NULL)						/* we have a partial previous line? */
	{
	  strcpy(s, t);						/* copy it over 					*/
	  DT_C(dtos_free2,(ctl, t,
        F, "old line"));
	}
	else
	  s[0] = '\0';
	
	l = strlen(s);
	DT_C(dtfd_read_line,(ctl, fd,		/* read until end of line 			*/
      s+l, max-l));
	if (l == (int)strlen(s))			/* end of file? 					*/
	{
	  if (l == 0)
	  {
	    DT_C(dtos_free2,(ctl, s,
          F, "empty line"));
	    s = NULL;
	  }
	  else								/* last line not complete with LF? 	*/
	  {
	    if (s[l-1] == CR)				/* trailing CR at end of file?		*/
	      s[--l] = '\0';				/* nuke carriage return				*/
	    if (s[l-1] == '\\')				/* trailing backslash at eof?		*/
		  s[--l] = '\0';				/* nuke the backslash 				*/
	  }
	  break;
		
	}
	l = strlen(s);
	if (l && s[l-1] == LF)				/* end of line? 					*/
	{
	  s[--l] = '\0';					/* nuke newline character 			*/
	  if (l && s[l-1] == CR)			/* carriage return?					*/
	    s[--l] = '\0';					/* nuke carriage return				*/
	  if (l && s[l-1] == '\\')			/* last character is backslash? 	*/
		s[--l] = '\0';					/* nuke the backslash 				*/
	  else								/* last character not backslash? 	*/
		break;							/* return line to user 				*/
	}

  } 
	
cleanup:
  if (rc != DT_RC_GOOD &&
	  s != NULL)
  {
    DT_I(dtos_free2,(ctl, s,
      F, "line"));
	s = NULL;
  }
  *line = s;
  return rc;
}

/*..........................................................................
| NAME
| dtliner_line_iscomment - return if line is a comment
| 
| SUMMARY
| Return ~iscomment~ as non-zero if ~line~ is a comment.
| Lines containing only white space are considered comments.
| Zero-length lines are also considered comments.
| 
| END
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtliner_line_iscomment(					/* see if line is a comment			*/
  dt_ctl_t *ctl,						/* environment control 				*/
  const char *line,						/* line to be examined 				*/
  const char *comment,					/* string of comment characters		*/
  int *iscomment)						/* returned comment indicator 		*/
{

  while(isspace(*line))					/* find first non-white character 	*/
    line++;

  if (*line == '\0')					/* end of line? 					*/
    *iscomment = 1;
  else
  if (comment == NULL ||				/* NULL or empty comment list?		*/
	  *comment == '\0')
	*iscomment = 0;						/* nothing is a comment 			*/
  else
  if (strchr(comment, *line))			/* found a comment character? 		*/
    *iscomment = 1;
  else
    *iscomment = 0;

  return DT_RC_GOOD;
}

/*..........................................................................
| NAME
| dtliner_line_split - split line into key and value
| 
| SUMMARY
| Ignore any line where the first non-white character is in ~comment~.
| Ignore any line containing only white space.
| Ignore each line with no key.
| Otherwise, parse into the triplets: key ~separator~ value.
| Trim all whitespace around each of the triplet items.
| Assign a zero-length value to each line with no ~separators~.
| Assign a zero-length value to all lines if ~separators~ is NULL or empty.
| Return ~*key~ as NULL when the line is ignored.
| Otherwise allocate space for both ~*key~ and ~*value~.
| 
| END
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtliner_line_split(						/* split line into key and value	*/
  dt_ctl_t *ctl,						/* environment control 				*/
  char *line,							/* the line to be parsed 			*/
  const char *comment,					/* list of comment characters 		*/
  const char *separators,				/* the key:value separator 			*/
  char **key,							/* returned key string 				*/
  char **value)							/* returned value string 			*/
{
  DT_F("dtliner_line_split");
  int iscomment;
  char *nbeg = line;
  char *nend;
  char *vbeg;
  char *vend = nbeg + strlen(nbeg);
  int l;
  dt_rc_e rc;

  *key = NULL;
  *value = NULL;

  if (separators == NULL)
    separators = "";

  DT_C(dtliner_line_iscomment,(ctl,		/* see if it is a comment 			*/
    line, comment, &iscomment));

  if (iscomment)						/* it's not a comment? 				*/
    return DT_RC_GOOD;
  
  while (isspace(*nbeg))				/* trim leading white space 		*/
    nbeg++;
  if (*nbeg == '\0')					/* empty line? 						*/
    return DT_RC_GOOD;

  for (nend = nbeg;						/* search for separator 			*/
	   *nend != '\0' &&
	   !strchr(separators, *nend);
	   nend++);

  if (*nend == '\0')					/* no separator? 					*/
  {
	nend--;
	while (isspace(*nend))				/* trim trailing white space 		*/
      nend--;
	vbeg = vend = "";
  }
  else
  {
	vbeg = nend + 1;
	nend--;
	while (isspace(*nend))				/* trim trailing white space 		*/
      nend--;
	while (isspace(*vbeg))				/* trim leading white space 		*/
      vbeg++;
	if (*vbeg == '\0')					/* only white after separator? 		*/
	  vbeg = vend = "";
	else
	{
	  vend--;
	  while (isspace(*vend))			/* trim trailing white space 		*/
        vend--;
	}
  }

  l = (int)(nend - nbeg + 1);
  DT_C(dtos_malloc2,(ctl,				/* space for the key 				*/
    (void **)key, l+1,
    F, "elem->name"));
  DT_MEMCPY(*key, nbeg, l);
  (*key)[l] = '\0';
	
  l = (int)(vend - vbeg + 1);
  DT_C(dtos_malloc2,(ctl,				/* space for the value				*/
    (void **)value, l+1,
    F, "elem->user"));
  DT_MEMCPY(*value, vbeg, l);
  (*value)[l] = '\0';
  
cleanup:
  if (rc != DT_RC_GOOD)
  {
	if (*value != NULL)
  	  DT_C(dtos_free2,(ctl, *value,
        F, "elem->user"));
	if (*key != NULL)
  	  DT_C(dtos_free2,(ctl, *key,
        F, "elem->name"));
  }
  return rc;
}

/*..........................................................................
| NAME
| dtliner_line_list - add line to list
| 
| SUMMARY
| Ignore any line where the first non-white character is in ~comment~.
| Ignore any line containing only white space.
| Otherwise, parse into the triplets: key ~separator~ value.
| Add an element to ~list~ with parsed key and value.
| Trim all whitespace around each of the triplet items.
| Assign a zero-length value to each line with no ~separator~.
| Build ~list~ such that dtlist_free() can be used to free all space.
| 
| END
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtliner_line_list(						/* make list from lines in file		*/
  dt_ctl_t *ctl,						/* environment control 				*/
  char *line,							/* the line to be parsed 			*/
  const char *comment,					/* list of comment characters 		*/
  const char *separators,				/* the key:value separators			*/
  dtlist_t *list)						/* list to add line to 				*/
{
  DT_F("dtliner_line_list");
  char *key = NULL;
  char *value = NULL;
  dt_rc_e rc;

  DT_C(dtliner_line_split,(ctl, line,	/* split line into key and value 	*/
    comment, separators,
    &key, &value));

  if (key != NULL)						/* line not being ignored? 			*/
    DT_C(dtlist_after,(ctl, list,		/* make new list element 			*/
      DTLIST_ELEM_TAIL,
      key, value, NULL));

cleanup:
  if (rc != DT_RC_GOOD)
  {
	if (value != NULL)
  	  DT_C(dtos_free2,(ctl, value,
        F, "elem->user"));
	if (key != NULL)
  	  DT_C(dtos_free2,(ctl, key,
        F, "elem->name"));
  }
  return rc;
}

/*..........................................................................
| NAME
| dtliner_fd_list - create list from lines in file
| 
| SUMMARY
| Read all lines from file.
| Remove the newline character from the end of the value.
| Ignore any line where the first non-white character is in ~comment~.
| Ignore any line containing only white space.
| Otherwise, parse into triplets: key ~separator~ value end
| assign an element of ~list~.
| Trim all whitespace around each of the triplets.
| Assign a zero-length value to each line with no ~separator~.
| Build ~list~ such that dtlist_free() can be used to free all space.
| 
| END
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtliner_fd_list(						/* make list from lines in file		*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtfd_t *fd,							/* file object descriptor 			*/
  const char *comment,					/* string of comment characters		*/
  const char *separators,				/* the key:value separator 			*/
  dtlist_t *list)						/* list to which lines are added 	*/
{
  DT_F("dtliner_fd_list");
  char *line = NULL;
  dt_rc_e rc;

  while (1)
  {
	DT_C(dtliner_fd_line,(ctl, fd,		/* get next line 					*/
	  &line));
	if (line == NULL)					/* end of file? 					*/
	  break;
	DT_C(dtliner_line_list,(ctl,		/* add it to the list 				*/
      line, comment, separators,  
      list));
	DT_C(dtos_free2,(ctl, line,			/* free line space 					*/
      F, "line"));
	line = NULL;
  }

cleanup:
  if (line != NULL)
	DT_I(dtos_free2,(ctl, line,
      F, "line"));
  return rc;
}

/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtliner_file_list(						/* make list from lines in file		*/
  dt_ctl_t *ctl,						/* environment control 				*/
  const char *filename,					/* name of file to read 			*/
  const char *comment,					/* string of comment characters		*/
  const char *separators,				/* the key:value separator 			*/
  dtlist_t *list)						/* list to which lines are added 	*/
{
  DT_F("dtliner_file_list");
  dtfd_t fd;
  dt_rc_e rc;

  fd.fd = -1;
  DT_C(dtfd_open,(ctl, &fd,				/* open the file 					*/
    filename, "r"));
  DT_C(dtliner_fd_list,(ctl,			/* read the file 					*/
    &fd, comment, separators, list));

cleanup:
  if (fd.fd != -1)						/* file was opened? 				*/
	DT_I(dtfd_close,(ctl, &fd));		/* close the file 					*/
  return rc;
}

/*..........................................................................
| NAME
| dtliner_line_list2 - add line to list
| 
| SUMMARY
| Mark special any line where the first non-white character is in ~comment~.
| Mark special any line containing only white space.
| A special line will have keyword equal to line and :NULL: value.
| Otherwise, parse into the triplets: key ~separator~ value.
| Add an element to ~list~ with parsed key and value.
| Trim all whitespace around each of the triplet items.
| Assign a zero-length value to each non-special line with no ~separator~.
| Build ~list~ such that dtlist_free() can be used to free all space.
| 
| END
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtliner_line_list2(						/* make list from lines in file		*/
  dt_ctl_t *ctl,						/* environment control 				*/
  char *line,							/* the line to be parsed 			*/
  const char *comment,					/* list of comment characters 		*/
  const char *separators,				/* the key:value separators			*/
  dtlist_t *list)						/* list to add line to 				*/
{
  DT_F("dtliner_line_list2");
  char *key = NULL;
  char *value = NULL;
  dt_rc_e rc;

  DT_C(dtliner_line_split,(ctl, line,	/* split line into key and value 	*/
    comment, separators,
    &key, &value));

  if (key == NULL)						/* special line? 					*/
  {
	DT_C(dtos_strdup,(ctl, line,		/* use entire line for key 			*/
      &key));
	value = NULL;
  }

  DT_C(dtlist_after,(ctl, list,			/* make new list element 			*/
    DTLIST_ELEM_TAIL,
    key, value, NULL));

cleanup:
  if (rc != DT_RC_GOOD)
  {
	if (value != NULL)
  	  DT_C(dtos_free2,(ctl, value,
        F, "elem->user"));
	if (key != NULL)
  	  DT_C(dtos_free2,(ctl, key,
        F, "elem->name"));
  }
  return rc;
}

/*..........................................................................
| NAME
| dtliner_fd_list2 - create list from lines in file
| 
| SUMMARY
| Read all lines from file.
| Parse lines using dtliner_line_list2().
| 
| END
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtliner_fd_list2(						/* make list from lines in file		*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtfd_t *fd,							/* file object descriptor 			*/
  const char *comment,					/* string of comment characters		*/
  const char *separators,				/* the key:value separator 			*/
  dtlist_t *list)						/* list to which lines are added 	*/
{
  DT_F("dtliner_fd_list2");
  char *line = NULL;
  dt_rc_e rc;

  while (1)
  {
	DT_C(dtliner_fd_line,(ctl, fd,		/* get next line 					*/
	  &line));
	if (line == NULL)					/* end of file? 					*/
	  break;
	DT_C(dtliner_line_list2,(ctl,		/* add it to the list 				*/
      line, comment, separators,  
      list));
	DT_C(dtos_free2,(ctl, line,			/* free line space 					*/
      F, "line"));
	line = NULL;
  }

cleanup:
  if (line != NULL)
	DT_I(dtos_free2,(ctl, line,
      F, "line"));
  return rc;
}

/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtliner_file_list2(						/* make list from lines in file		*/
  dt_ctl_t *ctl,						/* environment control 				*/
  const char *filename,					/* name of file to read 			*/
  const char *comment,					/* string of comment characters		*/
  const char *separators,				/* the key:value separator 			*/
  dtlist_t *list)						/* list to which lines are added 	*/
{
  DT_F("dtliner_file_list2");
  dtfd_t fd;
  dt_rc_e rc;

  fd.fd = -1;
  DT_C(dtfd_open,(ctl, &fd,				/* open the file 					*/
    filename, "r"));
  DT_C(dtliner_fd_list2,(ctl,			/* read the file 					*/
    &fd, comment, separators, list));

cleanup:
  if (fd.fd != -1)						/* file was opened? 				*/
	DT_I(dtfd_close,(ctl, &fd));		/* close the file 					*/
  return rc;
}

/*..........................................................................
| NAME
| dtliner_line_string - add line to string
| 
| SUMMARY
| Ignore any line where the first non-white character is in ~comment~.
| Ignore any line containing only white space.
| Otherwise, parse into the triplets: key ~separator~ value.
| Add the key and the value to the given string.
| Trim all whitespace around each of the triplet items.
| 
| END
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtliner_line_string(					/* make string from lines in file	*/
  dt_ctl_t *ctl,						/* environment control 				*/
  char *line,							/* the line to be parsed 			*/
  const char *comment,					/* list of comment characters 		*/
  const char *separators,				/* the key:value separators			*/
  char **string)						/* string to be updated 			*/
{
  DT_F("dtliner_line_string");
  char *key = NULL;
  char *value = NULL;
  dt_rc_e rc;

  DT_C(dtliner_line_split,(ctl, line,	/* split line into key and value 	*/
    comment, separators,
    &key, &value));

  if (key != NULL)						/* line not being ignored? 			*/
  {
	int sl, kl, vl, l, i;
	int need_quotes = 0;
	char *s;
	if (*string == NULL)				/* no starter string? 				*/
	  sl = 0;
	else
	  sl = strlen(*string);
	kl = strlen(key);
	vl = strlen(value);

	if (value[0] != '"')				/* first char of value not a quote? */
	for (i=0; i<vl; i++)				/* search for white space in value 	*/
	{
	  if (isspace(value[i]))
	  {
		need_quotes = 1;				/* use quotes if white space found 	*/
		break;	
	  }
	}
	  
	l = sl + !!sl + 
      kl +
      !!vl + vl + 
      2 * need_quotes + 1;

	DT_C(dtos_malloc2,(ctl,				/* space for new string 			*/
      (void **)&s, l,
      F, "string"));
	if (*string != NULL)				/* there was a starter string? 		*/
	{
	  strcpy(s, *string);
	  DT_C(dtos_free2,(ctl, *string,	/* free the starter string 			*/
        F, "old string"));
	}
	else
	  s[0] = '\0';
	*string = s;
	if (sl)
	  strcat(s, " ");
	strcat(s, key);
	if (vl)
	  strcat(s, " ");
	if (need_quotes)
	  strcat(s, "\"");
	strcat(s, value);
	if (need_quotes)
	  strcat(s, "\"");
  }

cleanup:
  if (value != NULL)
    DT_C(dtos_free2,(ctl, value,
      F, "elem->user"));
  if (key != NULL)
    DT_C(dtos_free2,(ctl, key,
      F, "elem->name"));
  
  return rc;
}

/*..........................................................................
| NAME
| dtliner_fd_string - create string from lines in file
| 
| SUMMARY
| Read all lines from file.
| Remove the newline character from the end of each line.
| Ignore any line where the first non-white character is in ~comment~.
| Ignore any line containing only white space.
| Otherwise, parse into triplets: key ~separator~ value.
| Append key and value to ~string~.
| Trim all whitespace around each of the triplets.
| Place quote characters around the value if it contains white space.
| Increase size of ~*string~ using dtos_malloc() as necessary.
| Create ~*string~ if it is given as NULL.
| 
| END
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtliner_fd_string(						/* make string from lines in file		*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtfd_t *fd,							/* file object descriptor 			*/
  const char *comment,					/* string of comment characters		*/
  const char *separators,				/* the key:value separator 			*/
  char **string)						/* string to be appended 			*/
{
  DT_F("dtliner_fd_string");
  char *line = NULL;
  dt_rc_e rc;

  while (1)
  {
	DT_C(dtliner_fd_line,(ctl, fd,		/* get next line 					*/
	  &line));
	if (line == NULL)					/* end of file? 					*/
	  break;
	DT_C(dtliner_line_string,(ctl,		/* add it to the string 			*/
      line, comment, separators,  
      string));
	DT_C(dtos_free2,(ctl, line,			/* free line space 					*/
      F, "line"));
	line = NULL;
  }

cleanup:
  if (line != NULL)
	DT_C(dtos_free2,(ctl, line,
      F, "line"));
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
