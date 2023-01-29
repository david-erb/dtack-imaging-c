/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
  ! Copyright (C) 1998 Dtack Inc. All Rights Reserved
  ! To use this file, you must have signed a license agreement with Dtack Inc.
  ! Under no circumstances may you redistribute this file.
  ! This software is provided AS IS with no warranty expressed or implied.
  ! Dtack Inc. accepts no liability for use or misuse of this file.
  ! http://www.dtack.com  dtack@dtack.com  telephone +360.417.6700
  ! Dtack Inc, P.O. Box 3061, Port Angeles, WA USA 98362
  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */





										/* ................................ */
typedef 
dt_rc_e
DTCONFIG_API1
dtmain_f(
  int argc, 
  char *argv[], 
  dtparam_t *);
										/* ................................ */
#define DTMAIN_NAME(F)														  \
  if (argv[0] != NULL)					/* we have a program name? 			*/\
  {																			  \
	char *t;																  \
	int i, l;																  \
	char *pslash, *pdot;													  \
	t = strdup(argv[0]);													  \
	l = strlen(t);						/* length of program name 			*/\
	for (i=l-1; i>0; i--)				/* change backslashes to slashes 	*/\
	  if (t[i] == '\\')														  \
		t[i] = '/';															  \
      else																	  \
		t[i] = tolower(t[i]);												  \
	pslash = strrchr(t, '/');			/* find last slash					*/\
	if (pslash != NULL)														  \
	  pslash++;																  \
	else																	  \
	  pslash = t;															  \
	pdot = strrchr(pslash, '.');		/* find last dot 					*/\
	if (pdot != NULL &&					/* there is a dot? 					*/\
		!strcmp(pdot, ".exe"))			/* and suffix is .exe? 				*/\
	  *pdot = '\0';						/* chop off the suffix 				*/\
	l = DT_MIN(strlen(t), sizeof(F)-1);										  \
	for (i=0; i<l; i++)														  \
	  F[i] = tolower(pslash[i]);											  \
	F[i] = '\0';															  \
	free(t);																  \
  }																			  \
  else									/* have no program name? 			*/\
  {																			  \
    extern const char *dtmain_progname;										  \
	strcpy(F, dtmain_progname);												  \
  }
										/* ................................ */

#if defined(DTCONFIG_IMPLIED_MSW)
#  include <dtack/msw.h>					/* special windows structure		*/
#else
  typedef struct {						/* fake special windows structure 	*/
    char *outbuf;
    int outbufm;
  } dtmsw_t;
#endif

#define DTMAIN_OUTBUFM (512)			/* maximum output buffer size 		*/

#define DTMAIN_SNULL ((char *)NULL)
#define DTMAIN_PNULL ((char **)NULL)

#if defined(DTCONFIG_IMPLIED_MSW) && \
    !defined(__CONSOLE__)

#define DTMAIN_MESSAGE \
  { \
    char tmp[128]; \
    sprintf(tmp, 

#define DTMAIN_BOX ); \
    MessageBox((HWND)NULL, tmp, "dtmain", MB_OK); \
  }

#define DTMAIN_MAXARGV (100)


#define DTMAIN1(PROG)                                                        \
  const char *dtmain_progname = #PROG;										 \
  static dt_rc_e                                                             \
    PROG ## _cmd(int argc, char *argv[], dtparam_t *param);                  \
  int PASCAL WinMain(                                                        \
    HINSTANCE hInstance,                                                     \
    HINSTANCE hPrevInstance,                                                 \
    LPSTR lpszCmdLine,                                                       \
    int nCmdShow)                                                            \
  {                                                                          \
    int argc = 0;                                                            \
    char *modulename;                                                        \
    char *s;                                                                 \
    char **argvs = (char **)malloc(DTMAIN_MAXARGV*sizeof(*argvs));           \
    char **argv = argvs + 1;                                                 \
    dtparam_t *param = (dtparam_t *)malloc(sizeof(*param));                  \
    dtmsw_t msw;                                                             \
    dt_rc_e rc;                                                              \
    memset(param, 0, sizeof(*param));                                        \
    memset(&msw, 0, sizeof(msw));                                            \
    msw.hinstance = (int)hInstance;                                          \
    msw.hprevinstance = (int)hPrevInstance;                                  \
    param->ctl.msw = (void *)&msw;                                           \
    if ((s=strchr(lpszCmdLine, '*')) != NULL) 								 \
    {																		 \
      HWND hwnd = (HWND)strtol(lpszCmdLine, DTMAIN_PNULL, 16);				 \
      int l = (int)strtol(s+1, DTMAIN_PNULL, 10) + 1;                        \
      DWORD g;                                                               \
      if ((s = (char *)malloc(l+17)) == NULL)                                \
      {                                                                      \
        DTMAIN_MESSAGE "malloc(%d) failed", l+1 DTMAIN_BOX                   \
        return 1;                                                            \
      }                                                                      \
      if ((g=SendMessage(hwnd, WM_GETTEXT, l, (DWORD)(LPSTR)s)) != l-1)      \
      {                                                                      \
        DTMAIN_MESSAGE "got %ld but wanted %d from text box 0x%08lx",        \
          g, l, (long)hwnd DTMAIN_BOX                                        \
        return 1;                                                            \
      }                                                                      \
      s[l] ='\0';                                                            \
    }																		 \
    else																	 \
    {																		 \
      if ((s = _strdup(lpszCmdLine)) == DTMAIN_SNULL)						 \
      {                                                                      \
        DTMAIN_MESSAGE "_strdup(%d) failed", strlen(lpszCmdLine) DTMAIN_BOX  \
        return 1;                                                            \
      }                                                                      \
    }																		 \
    dtstr_argv((dt_ctl_t *)NULL, s, argv, DTMAIN_MAXARGV-2, &argc);	      	 \
    argv[argc] = DTMAIN_SNULL;												 \
    if (argc > 0)															 \
    {																	     \
      param->ctl.feedbackwin = (void *)strtol(argv[0], DTMAIN_PNULL, 16);	 \
      argv++;																 \
      argc--;																 \
	}																		 \
    if (argc > 0)															 \
    {																	     \
      msw.outwin = (void *)strtol(argv[0], DTMAIN_PNULL, 16);				 \
      argv++;																 \
      argc--;																 \
	}																		 \
    if (argc > 0)															 \
    {																	     \
      msw.statuswin = (void *)strtol(argv[0], DTMAIN_PNULL, 16);			 \
      argv++;																 \
      argc--;																 \
	}																		 \
    if (argc > 0 && !strcmp(argv[0], "feedbackonly"))  						 \
    {                                 										 \
      param->ctl.feedbackonly = 1;											 \
      argv++;																 \
      argc--;																 \
	}      																	 \
    argv--;																	 \
    argc++;																	 \
    modulename = (char *)malloc(256);                                        \
    GetModuleFileName(hInstance, modulename, 256);	 	                     \
    argv[0] = modulename;                                                    \
    msw.outbuf = (char *)malloc(DTMAIN_OUTBUFM);                             \
    msw.outbufm = DTMAIN_OUTBUFM;                                            \
    rc = PROG ## _cmd(argc, argv, param);                                    \
    dt_dbg(&param->ctl, "DTMAIN", 0L, DTMAIN_SNULL, 0);                      \
    dt_err(&param->ctl, "DTMAIN", DTMAIN_SNULL, 0);                          \
    dt_say(&param->ctl, "DTMAIN", DTMAIN_SNULL, 0);                          \
    free((void *)msw.outbuf);                                                \
    free(modulename); 														 \
    free(s);       															 \
    free((void *)param);                                                     \
    free((void *)argvs);                                                     \
    return rc;                                                               \
  }                                                                          \
  static dt_rc_e                                                             \
  PROG ## _cmd(int argc, char *argv[], dtparam_t *param)

#else

										/* ................................ */
#define DTMAIN1(PROG)                                                        \
  const char *dtmain_progname = #PROG;								 		 \
  static dt_rc_e cmd(int argc, char *argv[], dtparam_t *);                   \
  main(int argc, char *argv[])                                               \
  {                                                                          \
    dtparam_t *param = (dtparam_t *)malloc(sizeof(*param));                  \
    dtmsw_t msw;                                                             \
    dt_rc_e rc;                                                              \
    dtcstack_init((dt_ctl_t *)&argc);                                        \
    if (getenv("DT_FEEDBACK"))                                               \
      param->ctl.feedbackwin =                                               \
      (void *)(long)(param->ctl.feedbackonly = 1);                           \
    memset(param, 0, sizeof(*param));                                        \
    param->ctl.msw = (void *)&msw;                                           \
    memset(&msw, 0, sizeof(msw));                                            \
    msw.outbuf = (char *)malloc(DTMAIN_OUTBUFM);                             \
    msw.outbufm = DTMAIN_OUTBUFM;                                            \
    rc = cmd(argc, argv, param);                                             \
    dt_say(&param->ctl, "DTMAIN", DTMAIN_SNULL, 0);                          \
    dt_err(&param->ctl, "DTMAIN", DTMAIN_SNULL, 0);                          \
    dt_dbg(&param->ctl, "DTMAIN", 0, DTMAIN_SNULL, 0);                       \
    free((void *)msw.outbuf);                                                \
    free((void *)param);                                                     \
    return rc;                                                               \
  }                                                                          \
  static dt_rc_e cmd(int argc, char *argv[], dtparam_t *param)
										/* ................................ */
#endif

#define DTMAIN2(PROG)                                                        \
  dtmain_f DTCONFIG_API1 PROG ## _cmd;                                       \
  dt_rc_e DTCONFIG_API1                                                      \
  PROG ## _cmd(int argc, char *argv[], dtparam_t *param)






/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
  ! Copyright (C) 1998 Dtack Inc. All Rights Reserved
  ! To use this file, you must have signed a license agreement with Dtack Inc.
  ! Under no circumstances may you redistribute this file.
  ! This software is provided AS IS with no warranty expressed or implied.
  ! Dtack Inc. accepts no liability for use or misuse of this file.
  ! http://www.dtack.com  dtack@dtack.com  telephone +360.417.6700
  ! Dtack Inc, P.O. Box 3061, Port Angeles, WA USA 98362
  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */
