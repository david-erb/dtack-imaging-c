/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
  ! Copyright (C) 1998 Dtack Inc. All Rights Reserved
  ! To use this file, you must have signed a license agreement with Dtack Inc.
  ! Under no circumstances may you redistribute this file.
  ! This software is provided AS IS with no warranty expressed or implied.
  ! Dtack Inc. accepts no liability for use or misuse of this file.
  ! http://www.dtack.com  dtack@dtack.com  telephone +360.417.6700
  ! Dtack Inc, P.O. Box 3061, Port Angeles, WA USA 98362
  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */





#define DTERRNO_FMT " had errno %d: %s" 
#define DTERRNO_ARG(E) (E), strerror((E))

#ifdef DTCONFIG_IMPLIED_MSW32
#define DTERRNO_LASTF " GetLastError=0x%08lx" 
#define DTERRNO_LASTE(E) (E)

#define DTERRNO_LASTS(E, S, M) \
  (S)[0] = '\0'; \
  FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, \
    NULL, (E), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), \
    (LPSTR)(S), (M), NULL); 			\
  while((S)[0] && 						\
        (!isprint((S)[strlen((S))-1]) || \
		 (S)[strlen((S))-1] == ' ' ||	\
		 (S)[strlen((S))-1] == '\t' ||	\
		 (S)[strlen((S))-1] == '\r' ||	\
		 (S)[strlen((S))-1] == '\n'))	\
    (S)[strlen((S))-1] = '\0';

#define DTERRNO_LASTFS " GetLastError=0x%08lx (%s)" 
#define DTERRNO_LASTES(E, S) (E), (S)
#endif





/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
  ! Copyright (C) 1998 Dtack Inc. All Rights Reserved
  ! To use this file, you must have signed a license agreement with Dtack Inc.
  ! Under no circumstances may you redistribute this file.
  ! This software is provided AS IS with no warranty expressed or implied.
  ! Dtack Inc. accepts no liability for use or misuse of this file.
  ! http://www.dtack.com  dtack@dtack.com  telephone +360.417.6700
  ! Dtack Inc, P.O. Box 3061, Port Angeles, WA USA 98362
  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */
