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
| NAME
| Introduction - intro
| 
| DESCRIPTION
| As yet undocumented.
| 
| RETURN VALUES
| Function return value will be DT_RC_GOOD on success.  
| Any other return value indicates failure or noncompletion.
| When DT_RC_GOOD is not returned, any other return values may not be valid.
| 
| ERRORS
| As yet undocumented.
| 
| END
 *..........................................................................*/

#include <dtack/base.h>

DT_RCSID("source/inter $RCSfile: note.c,v $ $Revision: 1.1 $");

#include <dtack/note.h>
#include <dtack/mem.h>

/*..........................................................................*/
dt_rc_e
DTCONFIG_API1
dtnote_clear(							/* clear note						*/
  dt_ctl_t *ctl,
  dtnote_t *note)
{
  DT_F("dtnote_clear");

  if (note->cursor.ptr)					/* this is not a first request?		*/
    DT_Q(dtmem_unmap,(ctl, note->mem,	/* release old page without writing	*/
      note->cursor.offset));

  memset(&note->cursor, 0,
    sizeof(note->cursor));

  note->n = 0;
  note->partial = 0;
  note->full = 0;

  return DT_RC_GOOD;
}

/*..........................................................................*/
dt_rc_e
DTCONFIG_API1
dtnote_newpage(							/* go to next page for writing		*/
  dt_ctl_t *ctl,
  dtnote_t *note)
{
  DT_F("dtnote_newpage");
  if (note->cursor.ptr)					/* this is not the first page?		*/
  {
    memset(note->cursor.ptr, '\0',		/* clear to end with EOS marks		*/
      note->cursor.left);
    DT_Q(dtmem_put,(ctl, note->mem,		/* release page for writing		    */
      note->cursor.offset));
  }
  note->cursor.offset +=				/* ready for next offset		    */ 
    note->cursor.size;
  DT_Q(dtmem_map,(ctl, note->mem,		/* map next page				    */
    note->cursor.offset, 
    (void **)&note->cursor.ptr, 
    &note->cursor.size));
  note->cursor.left =					/* new page is empty			    */
    note->cursor.size;
  note->cursor.pos = 
    note->cursor.offset;
  return DT_RC_GOOD;
}

/*..........................................................................
| NAME
| dtnote_add - add line to note
| 
| DESCRIPTION
| Appends string ~s~ of length ~l~ to note and finishes current line.
| If ~l~ is -1, then strlen() is used.
|
| If ~s~ contains only the null character, 
| and the note currently ends in a null character,
| then a hash (#) character is added before the null character.
|
| Pairs of null characters in ~s~ are changed to ! followed by the
| null character.
| 
| Caller must be careful not to call this routine with a string
| that does not end with the null character.
| Likewise, caller must be careful not to give strings with
| null characters anywhere but at the end.
| 
| RETURN VALUES
| Function return value will be DT_RC_GOOD on success.  
| Any other return value indicates failure or noncompletion.
| When DT_RC_GOOD is not returned, any other return values may not be valid.
| 
| ERRORS
| As yet undocumented.
| 
| END
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtnote_add(
  dt_ctl_t *ctl,
  dtnote_t *note,
  const char *s,
  int l)
{
  DT_F("dtnote_add");
  unsigned int i;

  if (note->cursor.left <				/* not at start of page?		    */
      note->cursor.size &&
      note->cursor.ptr[					/* current partial ends with EOS?   */
        note->partial-1] == '\0' &&
      l == 1 && *s == '\0')				/* caller is adding null string? 	*/
    DT_Q(dtnote_append,(ctl, note,		/* deactivate the end-of-page		*/
      "#", 1));
      
  DT_Q(dtnote_append,(ctl, note,		/* append contents				    */
    s, l));

  if (note->full)						/* some message was skipped?	    */
    return DT_RC_GOOD;					/* skip append with no error	    */

  for (i=0; i<note->partial-1; i++)
  {
    if (note->cursor.ptr[i] == '\0' &&
        note->cursor.ptr[i+1] == '\0')
      note->cursor.ptr[i] = '!';
    
  }

  note->cursor.ptr += note->partial;	/* advance to new output position   */
  note->cursor.left -= note->partial;	/* mark we used some of it up	    */
  note->cursor.pos += note->partial;	/* keep track of total fullness	    */

  note->partial = 0;					/* partials completed			    */

  note->n++;							/* count entries				    */

  return DT_RC_GOOD;
}

/*..........................................................................*/
dt_rc_e
DTCONFIG_API1
dtnote_append(
  dt_ctl_t *ctl,
  dtnote_t *note,
  const char *s,
  int l)
{
  DT_F("dtnote_append");
  dtmemalloc_t *memalloc = 
    (dtmemalloc_t *)note->mem;
  unsigned int m = note->partial;
  unsigned int n = l;

  if (note->full)						/* some message was skipped?	    */
    return DT_RC_GOOD;					/* skip append with no error	    */

  if (l == -1)							/* caller not giving a length?	    */
    l = strlen(s) + 1;					/* figure it out				    */

  if ((long)(m+n+note->cursor.pos) >=	/* string would overfill space?		*/
        memalloc->total ||
      m+n >= memalloc->pagesize)		/* string would overfill any page?	*/
  {
    note->full = 1;						/* mark note as being full		    */
    return DT_RC_GOOD;					/* skip append with no error	    */
  }

  if (m+n > note->cursor.left)			/* string would overfill page?		*/
  {
    char tmp[256];
    m = DT_MIN(m, sizeof(tmp));			/* truncate a long partial		    */
    memcpy(tmp, note->cursor.ptr, m);	/* copy partial from old page 		*/
    DT_Q(dtnote_newpage,(ctl, note));	/* finish page and get another	    */
    memcpy(note->cursor.ptr, tmp, m);	/* copy partial into new page	    */
  }

  if (m+n > note->cursor.left)			/* string overfill new page?		*/
  {
    note->full = 1;						/* mark note as being full		    */
    return DT_RC_GOOD;					/* skip append with no error	    */
  }
  else
    memcpy(note->cursor.ptr+m, s, n);	/* append string into page		    */

  note->partial = m+n;

  return DT_RC_GOOD;
}

/*..........................................................................*/
dt_rc_e
DTCONFIG_API1
dtnote_setpos(
  dt_ctl_t *ctl,
  dtnote_t *note,
  dtnote_cursor_t *cursor,
  long pos)
{
  DT_F("dtnote_setpos");
  dtmemalloc_t *memalloc = 
    (dtmemalloc_t *)note->mem;
  unsigned int pagepos;

  if (pos < 0)							/* trying to go before start?	    */
    pos = 0;
  if (pos > note->cursor.pos)			/* trying to go past end?		    */
    pos = note->cursor.pos;

  if (cursor->ptr)						/* this is not a first request?		*/
    DT_Q(dtmem_unmap,(ctl, note->mem,	/* release old page without writing	*/
      cursor->offset));

  pagepos = (unsigned int)				/* position within page			    */
    (pos % memalloc->pagesize);

  cursor->offset = pos - pagepos;		/* start of requested page		    */
  DT_Q(dtmem_get,(ctl, note->mem,		/* get next page				    */
    cursor->offset, 
    (void **)&cursor->ptr,
    &cursor->size));

  cursor->ptr += pagepos;				/* point to requested position	    */
  cursor->left = cursor->size -			/* new page is partially unread		*/
    pagepos;
  cursor->pos = pos;					/* remember the position		    */

  return DT_RC_GOOD;
}

/*..........................................................................*/
dt_rc_e
DTCONFIG_API1
dtnote_nextpage(
  dt_ctl_t *ctl,
  dtnote_t *note,
  dtnote_cursor_t *cursor)
{
  DT_F("dtnote_nextpage");
  if (cursor->ptr)						/* this is not a first request?		*/
  {
    DT_Q(dtmem_unmap,(ctl, note->mem,	/* release page without writing	    */
      cursor->offset));
    cursor->offset += cursor->size;		/* ready for next offset		    */
  }
  DT_Q(dtmem_get,(ctl, note->mem,		/* get next page				    */
    cursor->offset, 
    (void **)&cursor->ptr,
    &cursor->size));
  cursor->left = cursor->size;			/* new page is completely unread	*/
  cursor->pos = cursor->offset;

  return DT_RC_GOOD;
}

/*..........................................................................
 * page string\0string\0string\0\0\0\0\0 end of page
 * page string\0string\0string\0end\0
 * no string crosses a page boundary
 * two adjacent EOS means next would not fit on page, go to next page
 *..........................................................................*/
dt_rc_e
DTCONFIG_API1
dtnote_next(
  dt_ctl_t *ctl,
  dtnote_t *note,
  dtnote_cursor_t *cursor,
  char **s)
{
  DT_F("dtnote_next");

  if (cursor->pos < note->cursor.pos)	/* have more input?				    */
  {
    int l;
    if (cursor->left == 0 ||			/* end of page?					    */
        cursor->ptr == NULL ||			/* or first call?				    */
        *cursor->ptr == '\0')			/* or hit a second EOS char?	    */
      DT_Q(dtnote_nextpage,(ctl,		/* map next page				    */
        note, cursor));

    *s = cursor->ptr;					/* give string back to caller	    */

    l = strlen(cursor->ptr) + 1;		/* point past current string	    */
    cursor->ptr += l;					/* update cursor position		    */
    cursor->pos += l;
    cursor->left -= l;
  }
  else
    *s = NULL;

  return DT_RC_GOOD;
}

/*..........................................................................*/
dt_rc_e
DTCONFIG_API1
dtnote_prevpage(
  dt_ctl_t *ctl,
  dtnote_t *note,
  dtnote_cursor_t *cursor)
{
  DT_F("dtnote_prevpage");
  dtmemalloc_t *memalloc = 
    (dtmemalloc_t *)note->mem;
  if (cursor->offset)					/* not already at first page?		*/
  {
    DT_Q(dtmem_unmap,(ctl, note->mem,	/* release page without writing	    */
      cursor->offset));
  }
  cursor->offset -= memalloc->pagesize;	/* ready for previous offset	    */
  DT_Q(dtmem_get,(ctl, note->mem,		/* get previous page			    */
    cursor->offset, 
    (void **)&cursor->ptr,
    &cursor->size));
  cursor->left = 0;						/* position at very end of new page	*/
  cursor->pos = cursor->offset +
    cursor->size;
  cursor->ptr += cursor->size;

  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/
dt_rc_e
DTCONFIG_API1
dtnote_prev(
  dt_ctl_t *ctl,
  dtnote_t *note,
  dtnote_cursor_t *cursor,
  char **s)
{
  DT_F("dtnote_prev");
  int i;

  if (cursor->pos)						/* have more input?				    */
  {
    if (cursor->pos == cursor->offset)	/* at start of page?			    */
      DT_Q(dtnote_prevpage,(ctl,		/* map previous page			    */
        note, cursor));

    i = 0;
    while(cursor->pos+i >				/* skip EOS's of preceding string    */
          cursor->offset)
      if (cursor->ptr[--i] != '\0')
        break;

    while(cursor->pos+i >				/* find EOS of preceding string	    */
          cursor->offset)
	{
      if (cursor->ptr[i-1] == '\0')
        break;
      i--;
	}

    cursor->ptr += i;
    cursor->pos += i;
    cursor->left -= i;

    *s = cursor->ptr;					/* give string back to caller	    */
  }
  else
    *s = NULL;

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
