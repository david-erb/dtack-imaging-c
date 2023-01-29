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
  19990124 DE added wait for file to disappear
 *..........................................................................*/



#ifndef DTDEPEND
#define __USE_FIXED_PROTOTYPES__
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <time.h>
#include <string.h>

#if defined(__BORLANDC__) || defined(__TURBOC__) || defined(DTCONFIG_WV5) 
#include <dos.h>
#include <ctype.h>
#include <direct.h>
#include <io.h>
#include <fcntl.h>
#include <share.h>
#include <signal.h>
#endif
#endif

#if defined(DTCONFIG_WV5)
#define MKDIR _mkdir
#define UNLINK _unlink
#define READ _read
#else
#define MKDIR mkdir
#define UNLINK unlink
#define READ read
#endif

#if defined(DTCONFIG_SG1) || \
    defined(DTCONFIG_SG2) || \
    defined(DTCONFIG_SG5) || \
    defined(DTCONFIG_SP1) || \
    defined(DTCONFIG_LG6) || \
    defined(DTCONFIG_LG1) || \
    defined(DTCONFIG_FG1)

#include <unistd.h>
#include <sys/stat.h>
#include <ctype.h>
#include <sys/time.h>
#define UNIX
#endif

#if defined(DTCONFIG_SG1)
#	include <memory.h>
#	ifndef toupper
      extern int toupper(int);
#	endif
#	ifndef tolower
      extern int tolower(int);
#	endif
#endif

/*.......................................................................*/
void warning(
  char *id,
  char *format,
  ...)
{
  va_list args;
  id = "dtutil";
  va_start(args, format);
  fprintf(stdout, "%s: warning: ", id);
  vfprintf(stdout, format, args);
  va_end(args);
  fprintf(stdout, "\n");
}

/*.......................................................................*/
void error(
  char *id,
  char *format,
  ...)
{
  va_list args;
  id = "dtutil";
  va_start(args, format);
  fprintf(stderr, "%s: error: ", id);
  vfprintf(stderr, format, args);
  va_end(args);
  fprintf(stderr, "\n");
}
/*.......................................................................*/
void
do_time(
  int argc,
  char *argv[])
{
  time_t t1 = time(NULL);
  struct tm *t2 = localtime(&t1);
  char timebuf[32];
  char datebuf[32];
  int i;
  strftime(timebuf, sizeof(timebuf), "%H:%M", t2);
  strftime(datebuf, sizeof(datebuf), "%d %b %Y", t2);
  for (i=2; i<argc; i++)
    if (!strcmp(argv[i], "_date_"))
      printf("%s ", datebuf);
    else
    if (!strcmp(argv[i], "_time_"))
      printf("%s ", timebuf);
    else
      printf("%s ", argv[i]);
  printf("\n");
}
/*.......................................................................*/
void
do_mkdir(
  int argc,
  char *argv[])
{
  int i;
  for (i=2; i<argc; i++)
#ifdef UNIX
    MKDIR(argv[i], 0777);
#else
    MKDIR(argv[i]);
#endif
}
/*.......................................................................*/
void
do_del(
  int argc,
  char *argv[])
{
  int i;
  for (i=2; i<argc; i++)
    unlink(argv[i]);
}
/*.......................................................................*/
void
do_isexecutable(
  int argc,
  char *argv[])
{
  FILE *file;
  const char *filename = argv[2];
  if (argc != 3 && argc != 4 && argc != 5)
  {
    error(argv[0], "argument count %d wrong, expected 3, 4 or 5",
      argc);
    return;
  }

	file = fopen(filename, "r");		/* just try to open it			    */

    printf("@echo off\n");
	if (file == NULL)					/* open failed?						*/
	{
	  printf("echo %s: no such executable%s%s\n",
        argc > 3? argv[3]: argv[2],
        argc == 5? " > ": "",
        argc == 5? argv[4]: "");
	}
	else
	{
	  fclose(file);
	  printf("%s %%1 %%2 %%3 %%4 %%5 %%6 %%7 %%8 %%9\n", argv[2]);
	}
}
/*.......................................................................*/
void
do_cd(
  int argc,
  char *argv[])
{
#if defined(__BORLANDC__) || defined(__TURBOC__)
  char cmd[256];
  char *path = NULL;
  if (argc != 3)
  {
    error(argv[0], "argument count %d wrong, expected 3",
      argc);
    return;
  }
  if (argv[2][1] == ':')				/* disk:path? 						*/
  {
	if (_chdrive(toupper(argv[2][0])-'A'+1) != 0)
	  error(argv[0], "_chdrive(%d) to disk %c failed",
        toupper(argv[2][0])-'A',
        argv[2][2]);
	else
	{
	  path = argv[2] + 2;
      printf("%s: drive changed to %c:\n",
        argv[0], toupper(argv[2][0]));
    }
  }
  else									/* path only? 						*/
    path = argv[2];
  if (path && *path)
  {
	sprintf(cmd, "cd %s", path);
	if (chdir(path) != 0)
      error(argv[0], "chdir(\"%s\") failed", path);
    else
      printf("%s: path changed to %s\n", argv[0], path);
  }
#else
  printf("%s: nothing done for cd\n", argv[0]);
#endif
}
/*.......................................................................*/
void
do_catto(
  int argc,
  char *argv[])
{
  FILE *file;
  int c;
  time_t t;

  if (argc != 3)
  {
    error(argv[0], "argument count %d wrong, expected 3",
      argc);
    return;
  }
  file = fopen(argv[2], "a");
  if (file == NULL)
  {
    error(argv[0], 
      "unable to open file \"%s\"",
      argv[2]);
    return;
  }
  t = time(NULL);
  fprintf(file, "\nSTART %s",
    ctime(&t));
  fprintf(stderr, "dtutil catto: START %s",
    ctime(&t));
  while ((c=getc(stdin)) != EOF)
    putc(c, file);
  t = time(NULL);
  fprintf(file, "\nFINISH %s",
    ctime(&t));
  exit(1);
}
/*.......................................................................*/
static int sigint = 0;
void sigint_handler(int signum) { sigint = 1; }
/*.......................................................................*/
void
do_tail(
  int argc,
  char *argv[])
{
  char *search = NULL;
  int fd;
  char buf[256];
  int n;
  long delayed = 0;
  int found = 0;

  if (argc < 3)
  {
    error(argv[0], "argument count %d wrong, expected 3 or 4",
      argc);
    return;
  }
  if (argc == 4)
    search = argv[3];

#if defined(__BORLANDC__) || defined(__TURBOC__)

  signal(SIGINT, sigint_handler);

  fd = sopen(argv[2], O_RDONLY, SH_DENYNO);
  if (fd == -1)
  {
    error(argv[0], 
      "unable to open file \"%s\"",
      argv[2]);
    return;
  }

  do {
    n = READ(fd, buf, sizeof(buf));
    if (n == -1)
    {
	  write(fileno(stdout), "\n", 1);
      error(argv[0],
        "errno %d reading %s",
        errno, argv[2]);
	}
	else
	if (n == 0)
	{
	  char c = 0;
      if (sigint)  
	    c = '!'; 
	  else
      if (delayed < 10L)
        c = 'I'; 
	  else
      if (delayed < 100L)
	  {
	    if (delayed % 10L == 0) 
          c = 'X';
	  }
	  else
      if (delayed < 1000L)
	  {
	    if (delayed % 100L == 0) 
          c = 'C';
	  }
	  else
      if (delayed < 10000L)
	  {
	    if (delayed % 1000L == 0) 
          c = 'M';
	  }
	  else
      if (delayed < 100000L)
	  {
	    if (delayed % 10000L == 0) 
          c = 'D';
	  }
	  if (delayed == 0)
	    write(fileno(stdout), "\n", 1);
	  if (c)
	    write(fileno(stdout), &c, 1);
      sleep(1);
	  delayed++;
	}
	else
	{
	  if (delayed)
	    write(fileno(stdout), "\n", 1);
	  delayed = 0;
	  write(fileno(stdout), buf, n);
      found = !!strstr(buf, search);
	}	
  } while (n >= 0 && !sigint && !found);

  close(fd);
#endif

  exit(1);
}
/*.......................................................................*/
void
do_endcheck(
  int argc,
  char *argv[])
{
  int i;
  char program[128];
  char line[512];
  int entered = 0;

  for (i=2; i<argc; i++)
  {
    FILE *file = fopen(argv[i], "r");
    if (file == NULL)
	{
       error(argv[0], 
         "unable to open file \"%s\"",
         argv[i]);
       continue;
	}
	while(fgets(line, sizeof(line), file))
	{
	  char *p;
	  if (line[strlen(line)-1] == '\n')
        line[strlen(line)-1] = '\0';
	  p = strstr(line, ": ");			/* end of program identifier (PI)	*/
      if (p == NULL)
        continue;
      *p++ = '\0';
      if (strchr(line, ' '))			/* skip lines with spaces in PI	    */
        continue;
      if (strstr(line, ".bat"))			/* skip lines with .bat in PI	    */
        continue;
      if (strstr(line, ".sh"))			/* skip lines with .sh in PI	    */
        continue;
      if (strstr(line, "dtutil"))		/* skip lines with dtutil in PI	    */
        continue;
      if (entered)
	  {
        if (strcmp(line, program))
		{
          printf("%s did not terminate gracefully\n",
            program);
          strcpy(program, line);
		}
        if (!strcmp(p+3, "all memory freed") ||
            !strncmp(p+3, "unfreed memory", 14) ||
            !strncmp(p+1, "no such executable", 18) ||
            !strcmp(p+1, "normal exit"))
          entered = 0;
	  }
	  else
      if (strcmp(p+3, "all memory freed") &&
          strncmp(p+3, "unfreed memory", 14) &&
          strcmp(p+1, "normal exit"))
	  {
        strcpy(program, line);
        entered = 1;
	  }
	}
    fclose(file);
    if (entered)
      printf("%s did not terminate gracefully\n",
        program);
  }
}
/*.......................................................................*/
void
do_grep(
  int argc,
  char *argv[])
{
  int i, j;
  char line[512];
  char upper[512];
  int lineno;
  int print = 1;
  char target[128];
  for (j=0; argv[2][j]; j++)
    target[j] = toupper(argv[2][j]);
  target[j] = argv[2][j];
  for (i=3; i<argc; i++)
  {
    FILE *file = fopen(argv[i], "r");
    if (file == NULL)
	{
       error(argv[0], 
         "unable to open file \"%s\"",
         argv[i]);
       continue;
	}
	lineno = 0;
	while(fgets(line, sizeof(line), file))
	{
	  lineno++;
      for (j=0; line[j]; j++)
        upper[j] = toupper(line[j]);
      upper[j] = line[j];
	  if (strstr(upper, target) != NULL)	/* line contains target? 		*/
	  {
		if (!strncmp(line, "GREP ON", 7))
		  print = 1;
		else
		if (!strncmp(line, "GREP OFF", 8))
		  print = 0;
		else
		if (print)
		{
		  if (line[strlen(line)-1] == '\n')
		    line[strlen(line)-1] = '\0';
		  printf("%08d: %s\n", lineno, line);
		}
	  }
	}
    fclose(file);
  }
}
/*.......................................................................*/
void
do_cmp(
  int argc,
  char *argv[])
{
  FILE *file1;
  FILE *file2;
  char line1[512];
  char line2[512];
  char *fgot;
  int count;

  if (argc != 4)
  {
	error(argv[0],
      "two filename arguments required");
	return;
  }

  file1 = fopen(argv[2], "r");
  if (file1 == NULL)
  {
	error(argv[0], 
       "unable to open file \"%s\"",
       argv[2]);
	return;
  }

  file2 = fopen(argv[3], "r");
  if (file2 == NULL)
  {
	error(argv[0], 
       "unable to open file \"%s\"",
       argv[3]);
	return;
  }

  count = 0;
  while((fgot=fgets(line1, sizeof(line1), file1)) != NULL)
  {
	if (!fgets(line2, sizeof(line2), file2) ||
        strcmp(line1, line2))
	  break;
	count++;
  }
  if (fgot ||
      fgets(line2, sizeof(line2), file2))
  {
	warning(argv[0],
      "files differ at line %d: %s %s",
      count, argv[2], argv[3]);
	return;
  }
  fclose(file2);
  fclose(file1);
}
/*.......................................................................*/
void
do_cstring(
  int argc,
  char *argv[])
{
  int i, n;
  char *b;
    
  b = (char *)malloc(16384);			/* space for buffer 				*/
  n = READ(fileno(stdin), b, 16384);	/* read up to 16384 from stdin 		*/

  printf("static const"
    " char %s[%d] = {\n",
    argc < 3?							/* no variable name given? 			*/
      "usage":							/* use "usage" as default 			*/
      argv[2], 
    n+1);

  for (i=0; i<n; i++)
  {
    if (b[i] == '\n')
      printf("'\\n',");
	else
    if (b[i] == '\'')
      printf("'\\\'',");
	else
    if (b[i] == '\t')
      printf("'\\t',");
	else
      printf("'%c', ", b[i]);
	if (i%10 == 9)
      printf("\n");
  }
  printf("'\\0'\n};\n");

}

/*.......................................................................*/
void
do_pcldecode(
  int argc,
  char *argv[])
{
  int printed = 0;
# define ESC (0x1b)
  int c;
  do {
    c = getchar();						/* get next character			*/
    if (c == ESC || c == EOF)
    {
      if (printed)
        printf("  (%5d)\n", printed);
      printed = 0;
	  if (c == ESC)
        printf("ESC");
      else
	    printf("EOF\n");
    }
    else
    if (printed++ < 20)
    {
      if (isprint(c))
       printf(" %c", c);
     else
       printf(" 0x%02x", c);
    }
  } while (c != EOF);
#undef ESC
}

/*.......................................................................*/
void
do_pcl2pbm(
  int argc,
  char *argv[])
{
# define ESC (0x1b)
# define N (1024)
  FILE *file;
  char *b, *p;
  int i, n, m, columns, lines;
  int start;
  static char set_compression[6] =
    {ESC, '*', 'b', '0', 'M', '\0'};
  static char start_raster[6] =
    {ESC, '*', 'r', '1', 'A', '\0'};

  if (argc != 3)
  {
    error(argv[0], "argument count %d wrong, expected 3",
      argc);
    return;
  }

  file = fopen(argv[2], "r");
  if (file == NULL)
  {
    error(argv[0], 
      "unable to open file \"%s\"",
      argv[2]);
    return;
  }

  b = (char *)malloc(16384);			/* space for buffer 				*/
  n = fread(b, 1, N, file);				/* read first part of file 			*/
  b[n-1] = '\0';						/* terminate buffer 				*/
  for (i=0; i<n; i++)					/* change all zeros to non-zeros 	*/
    if (b[i] == '\0')
      b[i] = 0x01;
  p = strstr(b, set_compression);		/* search for compression mode 		*/
  if (p == NULL)
  {
	error(argv[0],
      "no 'ESC * b 0 M' found");
	return;
  }
  p = strstr(b, start_raster);			/* search for raster start 			*/
  if (p == NULL)
  {
	error(argv[0],
      "no 'ESC * r 1 A' found");
	return;
  }
  start = (int)(p - b) + 5;				/* starting first raster line 		*/
  fprintf(stderr, "b %p, p %p, start %d\n", b, p, start);
  fseek(file, start, SEEK_SET);			/* go to first raster line 			*/
  lines = 0;
  columns = 0;
  while(1)
  {
    n = fscanf(file, "%*c*b%dW", &m);	/* get raster line width 			*/
    if (n != 1)
	{
	  char c = fgetc(file);	
	  if (c == 'W')
	  {
        fprintf(stderr, "a. blank line at %d\n", lines);
        m = 0;
	  }
	  else 
	  if (c == 'p')
	  {
		fscanf(file, "+%dY", &n);		/* get relative vertical motion		*/
        fprintf(stderr, "a. skip %d at %d\n", n, lines);
		lines += n;
		continue;
	  }
	  else
	    break;
	}
    if (m > N)
	  error(argv[0],
        "invalid raster line width %d",
        m);
    if (m > columns)
       columns = m;
    fseek(file, m, SEEK_CUR);
	lines++;
  }
  printf("P4 %d %d\n",
    columns*8, lines);

  lines = 0;
  fseek(file, start, SEEK_SET);			/* go back to first raster line 	*/
  while(1)
  {
    n = fscanf(file, "%*c*b%dW", &m);	/* get raster width 				*/
    if (n != 1)
	{
	  char c = fgetc(file);	
	  if (c == 'W')
	  {
        m = 0;
        fprintf(stderr, "b. blank line at %d\n", lines);
	  }
	  else 
	  if (c == 'p')
	  {
		fscanf(file, "+%dY", &n);		/* get relative vertical motion		*/
		memset(b, 0, columns);			/* clear line						*/
		for (i=0; i<n; i++)
          fwrite(b, 1, columns, stdout); /* output complete line 			*/
        fprintf(stderr, "b. skip %d at %d\n", n, lines);
		lines += n;
		continue;
	  }
	  else
	    break;
	}
    else
	  fread(b, 1, m, file);				/* read raster line 				*/
	memset(b+m, 0, columns-m);			/* fill to end of line 				*/
    fwrite(b, 1, columns, stdout);		/* output complete line 			*/
	lines++;
  }
#ifdef NONO
  memset(b, 0, columns);				/* clear line						*/
  fwrite(b, 1, columns, stdout);		/* output final line				*/
#endif
  fclose(file);
#undef ESC
#undef N
}

/*.......................................................................*/
void
do_pcl2pjl(
  int argc,
  char *argv[])
{
# define ESC (0x1b)
  int c1, c2;
  char ret[128];
  char *economode;
  char *density;
  char crlf[3] = {0x0d, 0x0a, 0x00};
  if (argc != 3)
  {
    error(argv[0], "argument count %d wrong, expected 3",
      argc);
    return;
  }
  strcpy(ret, argv[2]);
  economode = strchr(ret, ','); 
  if (economode != NULL)
  {
    *economode++ = '\0';
	density = strchr(economode, ','); 
	if (density != NULL)
      *density++ = '\0';
  }
  
  if (economode == NULL ||
	  density == NULL ||
	  (strcmp(ret, "OFF") &&
	   strcmp(ret, "ON") &&
	   strcmp(ret, "LIGHT") &&
	   strcmp(ret, "MEDIUM") &&
	   strcmp(ret, "DARK")) ||
	  (strcmp(economode, "OFF") &&
	   strcmp(economode, "ON")) ||
	  (strcmp(density, "1") &&
	   strcmp(density, "2") &&
	   strcmp(density, "3") &&
	   strcmp(density, "4") &&
	   strcmp(density, "5") &&
	   strcmp(density, "LIGHT") &&
	   strcmp(density, "MEDIUM") &&
	   strcmp(density, "DARK")))
  {
    error(argv[0], "argument %s wrong, not in form R,E,D",
      argv[2]);
    return;
  }
  c1 = getchar();						/* get first character			*/
  do {
    c2 = getchar();						/* get next character			*/
    if (c1 == ESC && c2 == 'E')
    {
	  printf("%c%%-12345X@PJL COMMENT created by dtutil %s%s",
        ESC, argv[2], crlf);
	  printf("@PJL DEFAULT RET=%s%s", ret, crlf);
	  printf("@PJL DEFAULT ECONOMODE=%s%s", economode, crlf);
	  printf("@PJL DEFAULT DENSITY=%s%s", density, crlf);
	  printf("@PJL ENTER LANGUAGE=PCL%s", crlf);
	  c2 = getchar();
    }
	else	
	if (c1 != EOF)
	  putchar(c1);
	c1 = c2;
  } while (c1 != EOF);
}

/*.......................................................................*/
const
char *myctime(void)
{
  static char s[32];
  time_t t;
  t = time(NULL);
  strcpy(s, ctime(&t));
  s[strlen(s)-1] = '\0';
  return s;
}
/*.......................................................................*/
void
do_filldisk(
  int argc,
  char *argv[])
{
  FILE *file;
#define N (1024)
  long buf[N];
  long kmax, k;
  int i;
  int rc;
  if (argc != 4)
  {
    error(argv[0], "argument count %d wrong, expected 4",
      argc);
    return;
  }
  file = fopen(argv[2], "w");
  if (file == NULL)
  {
    error(argv[0], 
      "unable to open file \"%s\" for writing",
      argv[2]);
    return;
  }

  kmax = atol(argv[3]) * (1024*1024) / sizeof(*buf);

  fprintf(stderr, "dtutil filldisk: %s writing %s\n",
    myctime(), argv[2]);
  
  k = 0;
  do
  {
    for (i=0; i<N; i++)
      buf[i] = k++;
	rc = write(fileno(file), buf, sizeof(buf));
  } while (rc > 0 && (kmax == 0 || k < kmax));
  fclose(file);

  fprintf(stderr, "dtutil filldisk: %s wrote %ldM on %s, now checking\n",
    myctime(), (k*sizeof(*buf))/(1024*1024), argv[2]);
										/* ................................ */
  file = fopen(argv[2], "r");
  if (file == NULL)
  {
    error(argv[0], 
      "unable to open file \"%s\" for reading",
      argv[2]);
    return;
  }
  
  k = 0;
  do
  {
	rc = READ(fileno(file), buf, sizeof(buf));
	if (rc > 0)
      for (i=0; i<N; i++)
	  {
        if (buf[i] != k)
          fprintf(stderr, "dtutil filldisk: expected %ld, got %ld on %s\n",
			k, buf[i], argv[2]);
		k++;
	  }
  } while (rc > 0 && (kmax == 0 || k < kmax));
  fclose(file);

  unlink(argv[2]);
  if (file == NULL)
  {
    error(argv[0], 
      "unable to unlink file \"%s\"",
      argv[2]);
    return;
  }

  fprintf(stderr, "dtutil filldisk: %s %s unlinked\n",
    myctime(), argv[2]);
# undef N
}
/*.......................................................................*/
void
do_waitrwd(
  int argc,
  char *argv[])
{
  const char *filename;
  long maxwait;
  time_t then;

  if (argc != 4)
  {
    error(argv[0], "argument count %d wrong, expected 4",
      argc);
    return;
  }

  filename = argv[2];
  maxwait = atol(argv[3]);

  then = time(NULL);
  while(1)
  {
	FILE *file;
	time_t now;
    file = fopen(argv[2], "r");
    if (file == NULL)
	  break;
	fclose(file);
	now = time(NULL);
	if (now - then > maxwait)
	  break;
  }
}

/*.......................................................................*/
int
main(
  int argc,
  char *argv[])
{
  if (argc == 1)
    error(argv[0], "no arguments"), exit(1);
  if (!strcmp(argv[1], "time"))
    do_time(argc, argv);
  else
  if (!strcmp(argv[1], "mkdir"))
    do_mkdir(argc, argv);
  else
  if (!strcmp(argv[1], "del"))
    do_del(argc, argv);
  else
  if (!strcmp(argv[1], "cd"))
    do_cd(argc, argv);
  else
  if (!strcmp(argv[1], "catto"))
    do_catto(argc, argv);
  else
  if (!strcmp(argv[1], "tail"))
    do_tail(argc, argv);
  else
  if (!strcmp(argv[1], "isexecutable"))
    do_isexecutable(argc, argv);
  else
  if (!strcmp(argv[1], "endcheck"))
    do_endcheck(argc, argv);
  else
  if (!strcmp(argv[1], "grep"))
    do_grep(argc, argv);
  else
  if (!strcmp(argv[1], "cmp"))
    do_cmp(argc, argv);
  else
  if (!strcmp(argv[1], "cstring"))
    do_cstring(argc, argv);
  else
  if (!strcmp(argv[1], "pcldecode"))
    do_pcldecode(argc, argv);
  else
  if (!strcmp(argv[1], "pcl2pbm"))
    do_pcl2pbm(argc, argv);
  else
  if (!strcmp(argv[1], "pcl2pjl"))
    do_pcl2pjl(argc, argv);
  else
  if (!strcmp(argv[1], "filldisk"))
    do_filldisk(argc, argv);
  else
  if (!strcmp(argv[1], "waitrwd"))
    do_waitrwd(argc, argv);
  else
    error(argv[0], "unknown op \"%s\"", argv[1]), exit(1);
  exit(0);
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
