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
  20000305 DE use SCHEDULE debug level for file times
  19990815 DE release header lines before getting new ones
 *..........................................................................*/


#include <dtack/base.h>
DT_RCSID("source/dg $RCSfile: header.c,v $ $Revision: 1.8 $");

#include <dtack/ipc.h>
#include <dtack/list.h>
#include <dtack/dg.h>
#include <dtack/liner.h>
#include <dtack/str.h>
#include <dtack/os.h>

/*..........................................................................
| NAME
| dtdg_header_get() - receive response header
|
| SUMMARY
| Read all request or response header lines.
| The header is separated from the data by an empty line.
| The header fields are simply stored within the object for later
| parsing and extraction.
| 
| END
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtdg_header_get(						/* receive response header			*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtdg_t *dg,							/* this dtdg object 				*/
  long timeout)
{
  DT_F("dtdg_get_header");
  char *line = NULL;
  int i;
  dt_rc_e rc;

  DT_C(dtlist_free,(ctl,				/* free existing header lines		*/
    &dg->header_list));
  
  while (1)
  {
	DT_C(dtdg_get_line2,(ctl,			/* get next line from partner 		*/
      dg, timeout, 0, &line, NULL));
	
	if (line == NULL)					/* partner has ended conversation? 	*/
	{
	  rc = dt_err(ctl, F, 
        "partner ended conversation"
        " before response header complete");
	  goto cleanup;
	}

	if (strlen(line) == 0)				/* also end on blank line 			*/
	  break;

	for (i=0;							/* convert keyword to lower case 	*/
		 line[i] != '\0' &&
		 line[i] != ':';
		 i++)
	  line[i] = tolower(line[i]);

	DT_C(dtliner_line_list,(ctl,		/* add it to the list 				*/
      line, "", ":", 
      &dg->header_list));

	dt_dbg(ctl, F, DT_DBG_MASK_REPLY,
      "%s", line);

	DT_C(dtos_free2,(ctl, line,			/* free line space 					*/
      F, "line"));
	line = NULL;
  } 

cleanup:
  if (line != NULL)
	DT_I(dtos_free2,(ctl, line,			/* free line space 					*/
      F, "line"));
  return rc;
}

/*..........................................................................
 * Have an error if field ~name~ is not found in header body.
 *..........................................................................*/

dt_rc_e 
DTCONFIG_API1
dtdg_header_search(						/* search field value for header	*/
  dt_ctl_t *ctl,
  dtdg_t *dg,
  const char *name,
  const char **value)
{
  DT_F("dtdg_header_search");
  dtlist_elem_t *elem;

  DT_Q(dtlist_search,(ctl,				/* search for  field 				*/
    &dg->header_list,
    name, &elem));
  if (elem == NULL)						/* no such field in header?			*/
	*value = NULL;
  else
    *value = (const char *)elem->user;

  return DT_RC_GOOD;
}

/*..........................................................................
 * Have an error if field ~name~ is not found in header body.
 *..........................................................................*/

dt_rc_e 
DTCONFIG_API1
dtdg_header_find(						/* find field value in header		*/
  dt_ctl_t *ctl,
  dtdg_t *dg,
  const char *name,
  const char **value)
{
  DT_F("dtdg_header_find");
  dtlist_elem_t *elem;

  DT_Q(dtlist_search,(ctl,				/* search for field 				*/
    &dg->header_list,
    name, &elem));
  if (elem == NULL)						/* no such field in header? 		*/
	return dt_err(ctl, F,
      "no %s field in header",
      name);
  *value = (const char *)elem->user;

  return DT_RC_GOOD;
}


/* ====================================================================
 * Copyright (c) 1996,1997 The Apache Group.  All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer. 
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 *
 * 3. All advertising materials mentioning features or use of this
 *    software must display the following acknowledgment:
 *    "This product includes software developed by the Apache Group
 *    for use in the Apache HTTP server project (http://www.apache.org/)."
 *
 * 4. The names "Apache Server" and "Apache Group" must not be used to
 *    endorse or promote products derived from this software without
 *    prior written permission.
 *
 * 5. Redistributions of any form whatsoever must retain the following
 *    acknowledgment:
 *    "This product includes software developed by the Apache Group
 *    for use in the Apache HTTP server project (http://www.apache.org/)."
 *
 * THIS SOFTWARE IS PROVIDED BY THE APACHE GROUP ``AS IS'' AND ANY
 * EXPRESSED OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE APACHE GROUP OR
 * ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
 * OF THE POSSIBILITY OF SUCH DAMAGE.
 * ====================================================================
 *
 * This software consists of voluntary contributions made by many
 * individuals on behalf of the Apache Group and was originally based
 * on public domain software written at the National Center for
 * Supercomputing Applications, University of Illinois, Urbana-Champaign.
 * For more information on the Apache Group and the Apache HTTP server
 * project, please see <http://www.apache.org/>.
 *
 */

/*
 * util_date.c: date parsing utility routines
 *     These routines are (hopefully) platform-independent.
 * 
 * 27 Oct 1996  Roy Fielding
 *     Extracted (with many modifications) from mod_proxy.c and
 *     tested with over 50,000 randomly chosen valid date strings
 *     and several hundred variations of invalid date strings.
 * 
 */

/*..........................................................................
 *..........................................................................*/

#define BAD_DATE (time_t)0


/*..........................................................................
 *..........................................................................*/
/*
 * Compare a string to a mask
 * Mask characters (arbitrary maximum is 256 characters, just in case):
 *   @ - uppercase letter
 *   $ - lowercase letter
 *   & - hex digit
 *   # - digit
 *   ~ - digit or space
 *   * - swallow remaining characters 
 *  <x> - exact match for any other character
 */
static
int checkmask(const char *data, const char *mask)
{
    int i;
    char d;

    for (i = 0; i < 256; i++) {
        d  = data[i];
        switch (mask[i]) {
          case '\0': return (d == '\0');

          case '*':  return 1;

          case '@':  if (!isupper(d)) return 0;
                     break;
          case '$':  if (!islower(d)) return 0;
                     break;
          case '#':  if (!isdigit(d)) return 0;
                     break;
          case '&':  if (!isxdigit(d)) return 0;
                     break;
          case '~':  if ((d != ' ') && !isdigit(d)) return 0;
                     break;
          default:   if (mask[i] != d) return 0;
                     break;
        }
    }
    return 0;  /* We only get here if mask is corrupted (exceeds 256) */
}


/*..........................................................................
 *..........................................................................*/
/*
 * tm2sec converts a GMT tm structure into the number of seconds since
 * 1st January 1970 UT.  Note that we ignore tm_wday, tm_yday, and tm_dst.
 * 
 * The return value is always a valid time_t value -- (time_t)0 is returned
 * if the input date is outside that capable of being represented by time(),
 * i.e., before Thu, 01 Jan 1970 00:00:00 for all systems and 
 * beyond 2038 for 32bit systems.
 *
 * This routine is intended to be very fast, much faster than mktime().
 */
static
time_t tm2sec(const struct tm *t)
{
    int  year;
    time_t days;
    const int dayoffset[12] =
        {306, 337, 0, 31, 61, 92, 122, 153, 184, 214, 245, 275};

    year = t->tm_year;

    if (year < 70 || ((sizeof(time_t) <= 4) && (year >= 138)))
        return BAD_DATE;

    /* shift new year to 1st March in order to make leap year calc easy */

    if (t->tm_mon < 2) year--;

    /* Find number of days since 1st March 1900 (in the Gregorian calendar). */

    days  = year * 365 + year/4 - year/100 + (year/100 + 3)/4;
    days += dayoffset[t->tm_mon] + t->tm_mday - 1;
    days -= 25508; /* 1 jan 1970 is 25508 days since 1 mar 1900 */

    days = ((days * 24 + t->tm_hour) * 60 + t->tm_min) * 60 + t->tm_sec;

    if (days < 0)
        return BAD_DATE;       /* must have overflowed */
    else
        return days;           /* must be a valid time */
}


/*..........................................................................
 *..........................................................................*/
/*
 * Parses an HTTP date in one of three standard forms:
 *
 *     Sun, 06 Nov 1994 08:49:37 GMT  ; RFC 822, updated by RFC 1123
 *     Sunday, 06-Nov-94 08:49:37 GMT ; RFC 850, obsoleted by RFC 1036
 *     Sun Nov  6 08:49:37 1994       ; ANSI C's asctime() format
 *
 * and returns the time_t number of seconds since 1 Jan 1970 GMT, or
 * 0 if this would be out of range or if the date is invalid.
 *
 * The restricted HTTP syntax is
 * 
 *     HTTP-date    = rfc1123-date | rfc850-date | asctime-date
 *
 *     rfc1123-date = wkday "," SP date1 SP time SP "GMT"
 *     rfc850-date  = weekday "," SP date2 SP time SP "GMT"
 *     asctime-date = wkday SP date3 SP time SP 4DIGIT
 *
 *     date1        = 2DIGIT SP month SP 4DIGIT
 *                    ; day month year (e.g., 02 Jun 1982)
 *     date2        = 2DIGIT "-" month "-" 2DIGIT
 *                    ; day-month-year (e.g., 02-Jun-82)
 *     date3        = month SP ( 2DIGIT | ( SP 1DIGIT ))
 *                    ; month day (e.g., Jun  2)
 *
 *     time         = 2DIGIT ":" 2DIGIT ":" 2DIGIT
 *                    ; 00:00:00 - 23:59:59
 *
 *     wkday        = "Mon" | "Tue" | "Wed"
 *                  | "Thu" | "Fri" | "Sat" | "Sun"
 *
 *     weekday      = "Monday" | "Tuesday" | "Wednesday"
 *                  | "Thursday" | "Friday" | "Saturday" | "Sunday"
 *
 *     month        = "Jan" | "Feb" | "Mar" | "Apr"
 *                  | "May" | "Jun" | "Jul" | "Aug"
 *                  | "Sep" | "Oct" | "Nov" | "Dec"
 *
 * However, for the sake of robustness (and Netscapeness), we ignore the
 * weekday and anything after the time field (including the timezone).
 *
 * This routine is intended to be very fast; 10x faster than using sscanf.
 *
 * Originally from Andrew Daviel <andrew@vancouver-webpages.com>, 29 Jul 96
 * but many changes since then.
 *
 */
static
time_t parseHTTPdate(const char *date)
{
    struct tm ds;
    int mint, mon;
    const char *monstr, *timstr;
    const int months[12] = {
        ('J' << 16) | ( 'a' << 8) | 'n',  ('F' << 16) | ( 'e' << 8) | 'b',
        ('M' << 16) | ( 'a' << 8) | 'r',  ('A' << 16) | ( 'p' << 8) | 'r',
        ('M' << 16) | ( 'a' << 8) | 'y',  ('J' << 16) | ( 'u' << 8) | 'n',
        ('J' << 16) | ( 'u' << 8) | 'l',  ('A' << 16) | ( 'u' << 8) | 'g',
        ('S' << 16) | ( 'e' << 8) | 'p',  ('O' << 16) | ( 'c' << 8) | 't',
        ('N' << 16) | ( 'o' << 8) | 'v',  ('D' << 16) | ( 'e' << 8) | 'c'};

    if (!date)
        return BAD_DATE;

    while (*date && isspace(*date))      /* Find first non-whitespace char */
        ++date;

    if (*date == '\0')
        return BAD_DATE;

    if ((date = strchr(date,' ')) == NULL)   /* Find space after weekday */
        return BAD_DATE;

    ++date;    /* Now pointing to first char after space, which should be */
               /* start of the actual date information for all 3 formats. */
    
    if (checkmask(date, "## @$$ #### ##:##:## *")) {     /* RFC 1123 format */
        ds.tm_year = ((date[7] - '0') * 10 + (date[8] - '0') - 19) * 100;
        if (ds.tm_year < 0)
            return BAD_DATE;

        ds.tm_year += ((date[9] - '0') * 10) + (date[10] - '0');

        ds.tm_mday  = ((date[0] - '0') * 10) + (date[1] - '0');

        monstr = date + 3;
        timstr = date + 12;
    }
    else if (checkmask(date, "##-@$$-## ##:##:## *")) {  /* RFC 850 format  */
        ds.tm_year = ((date[7] - '0') * 10) + (date[8] - '0');
        if (ds.tm_year < 70)
            ds.tm_year += 100;

        ds.tm_mday = ((date[0] - '0') * 10) + (date[1] - '0');

        monstr = date + 3;
        timstr = date + 10;
    }
    else if (checkmask(date, "@$$ ~# ##:##:## ####*")) { /* asctime format  */
        ds.tm_year = ((date[16] - '0') * 10 + (date[17] - '0') - 19) * 100;
        if (ds.tm_year < 0)
            return BAD_DATE;

        ds.tm_year += ((date[18] - '0') * 10) + (date[19] - '0');

        if (date[4] == ' ')
            ds.tm_mday = 0;
        else
            ds.tm_mday = (date[4] - '0') * 10;

        ds.tm_mday += (date[5] - '0');

        monstr = date;
        timstr = date + 7;
    }
    else return BAD_DATE;

    if (ds.tm_mday <= 0 || ds.tm_mday > 31)
        return BAD_DATE;

    ds.tm_hour = ((timstr[0] - '0') * 10) + (timstr[1] - '0');
    ds.tm_min  = ((timstr[3] - '0') * 10) + (timstr[4] - '0');
    ds.tm_sec  = ((timstr[6] - '0') * 10) + (timstr[7] - '0');

    if ((ds.tm_hour > 23) || (ds.tm_min > 59) || (ds.tm_sec > 61))
        return BAD_DATE;

    mint = (monstr[0] << 16) | (monstr[1] << 8) | monstr[2];
    for (mon=0; mon < 12; mon++)
        if (mint == months[mon])
            break;
    if (mon == 12)
        return BAD_DATE;
    
    if ((ds.tm_mday == 31) && (mon == 3 || mon == 5 || mon == 8 || mon == 10))
        return BAD_DATE;

    /* February gets special check for leapyear */

    if ((mon == 1) && ((ds.tm_mday > 29) ||
         ((ds.tm_mday == 29) && ((ds.tm_year & 3) ||
           (((ds.tm_year % 100) == 0) && (((ds.tm_year % 400) != 100)))))))
        return BAD_DATE;

    ds.tm_mon = mon;

    return tm2sec(&ds);
}


/*..........................................................................
 * check if-modified-since field against given filename
 *..........................................................................*/

dt_rc_e 
DTCONFIG_API1
dtdg_header_since(						/* search field value for header	*/
  dt_ctl_t *ctl,
  dtdg_t *dg,
  const char *filename,
  int *modified)
{
  DT_F("dtdg_header_since");
  const char *date;
  time_t t1, t2;
  int sysrc;
  struct stat statbuf;
  struct tm *tmbuf1;
  struct tm *tmbuf2;
  char s1[256];
  char s2[256];
  dt_rc_e rc = DT_RC_GOOD;

  *modified = 1;						/* assume file modified 			*/

  DT_C(dtdg_header_search,(ctl, dg,		/* get pertinent field 				*/
    "if-modified-since", &date));
  
  if (!date || !date[0])				/* no such field? 					*/
	goto cleanup;

  t1 = parseHTTPdate(date);				/* parse date from header 			*/
  if (t1 == BAD_DATE)					/* could not parse date? 			*/
  {
	dt_dbg(ctl, F, DT_DBG_MASK_SCHEDULE,
      "bad date \"%s\"", date);
	goto cleanup;
  }

  sysrc = stat(filename, &statbuf);		/* get stat from filename 			*/
  if (sysrc < 0)						/* failed to stat file? 			*/
  {
    dt_dbg(ctl, F, DT_DBG_MASK_SCHEDULE,
      "stat(\"%s\"=%d", filename, sysrc);
	goto cleanup;
  }

  tmbuf1 = gmtime(&t1);
  strftime(s1, sizeof(s1),
    "%Y%m%d.%H%M%S", tmbuf1);
  while (isspace(s1[strlen(s1)-1]))
	s1[strlen(s1)-1] = '\0';
  
  tmbuf2 = gmtime(&statbuf.st_mtime);
  strftime(s2, sizeof(s2),
    "%Y%m%d.%H%M%S", tmbuf1);
  while (isspace(s2[strlen(s2)-1]))
	s2[strlen(s2)-1] = '\0';
  
  if (t1 >= statbuf.st_mtime)			/* not modified since? 				*/
  {
	dt_dbg(ctl, F, DT_DBG_MASK_SCHEDULE,
      "t1=%ld (%s) >= stat.st_mtime=%ld (%s)", 
      (long)t1, s1,
      (long)statbuf.st_mtime, s2);
	*modified = 0;
  }
  else
  {
	dt_dbg(ctl, F, DT_DBG_MASK_SCHEDULE,
      "t1=%ld (%s) < stat.st_mtime=%ld (%s)", 
      (long)t1, s1,
      (long)statbuf.st_mtime, s2);
  }

cleanup:
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

