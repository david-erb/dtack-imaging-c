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
  20010320 DE don't yield in socket read of only 1 byte
  20000426 DE added comment
  19990826 DE fixed done function for NT
  19990824 DE spec flags for linger and shutdown
  19990101 DE reduce "waiting for read" messages
  19980920 DE yield during write and read
  19980818 DE ifdeffed out KEEPALIVE=0
 *..........................................................................*/

#define DTIPC_FLAG_LINGER_SHORT      0x1000
#define DTIPC_FLAG_LINGER_LONG       0x2000
#define DTIPC_FLAG_NOSHUTDOWN        0x4000

/*..........................................................................
| NAME
| Introduction - intro
|
| DESCRIPTION
| This is the implementation level for Berkeley and Winsock sockets.
| This level is called by the :dtipc: cover level only.
| 
| The only external function in this file is dtipc_rocket_init().
| It "instantiates" an ipc object by initializing the :dtipc_t: structure.
| An important part of the instantiation of :dtipc_t: is the loading 
| of the function pointers.
| All function pointers reference static functions within this file.
|
| Each function must have the :dt_ctl_t: runtime environment structure
| pointer as its first argument.
| The runtime environment structure controls such things as debug,
| error, and memory management handling.
|
| Each function must have the :dtipc_t: structure pointer as its
| second argument.
| This structure is essentially the object descriptor.
| It is initialized by dtipc_rocket_init() and 
| must be passed to all subsequent functions.
| 
| ERRORS
| As yet undocumented.
|
| REV
| 4.0 19970525 dtipc_rocket_write returns wrote=0 if EPIPE or ECONNRESET
| 4.1 19971221 bug clearing ipc structure too soon in unfind and unregister
| 4.2 19980106 rearrange code around SIGCHLD in poll
| 4.3 19980114 fix sigpipe weirdness
| 
| END
 *..........................................................................*/

#include <dtack/base.h>
DT_RCSID("source/low $RCSfile: ipcrock.c,v $ $Revision: 1.19 $");
#include <dtack/ipc.h>

#include <dtack/os.h>
#include <dtack/str.h>
#include <dtack/prj.h>
#include <dtack/label.h>
#include <dtack/errno.h>

typedef struct {
  char name[64];
} priv_t;

#if DTPRJ_SOCKET == DT_1				/* we can compile sockets? 			*/

#  ifndef DTDEPEND						/* no makefile dependency 			*/
#    ifdef DTCONFIG_IMPLIED_QNX4

#      define timezone timezone_l
#      include <time.h>
#      undef timezone
#      define timezone timezone_t
#      undef timezone
       struct iovec { 
         caddr_t iov_base; 
         long iov_len; 
       };
#      define SOCKET_IS_FD
#      define IOCTL ioctl
#    endif
#    ifdef DTCONFIG_IMPLIED_MSW			/* doing Windows sockets? 			*/
#      include <winsock2.h>
#      ifndef ECONNREFUSED			
#        define ECONNREFUSED \
           WSAECONNREFUSED
#      endif
#      ifndef EWOULDBLOCK			
#        define EWOULDBLOCK \
           WSAEWOULDBLOCK
#      endif
#      ifndef EINPROGRESS			
#        define EINPROGRESS \
           WSAEINPROGRESS
#      endif
#      ifndef EADDRINUSE			
#        define EADDRINUSE \
           WSAEADDRINUSE
#      endif
#      ifndef ENOPROTOOPT
#        define ENOPROTOOPT \
           WSAENOPROTOOPT
#      endif
#      ifndef ECONNRESET
#        define ECONNRESET \
           WSAECONNRESET
#      endif
#      define LASTERROR \
         (WSAGetLastError())
#      define IOCTL ioctlsocket
#    else 								/* not doing Windows sockets?		*/
#      define HAVE_POSIX_SIGNALS
#      define HANDLE_SIGPIPE_ON_WRITE
#      include <sys/time.h>
#      include <sys/socket.h>
#      include <netinet/in.h>
#      include <arpa/inet.h>
#      include <netdb.h>
#      include <signal.h>
#      include <sys/wait.h>
#      include <netinet/tcp.h>
#      include <sys/ioctl.h>
#      define LASTERROR (errno)
#      define SOCKET_IS_FD
#      define IOCTL ioctl
#    endif
#    ifdef DTCONFIG_IMPLIED_SOLARIS
#      include <sys/filio.h>
#    endif
#    ifdef DTCONFIG_IMPLIED_QNX4
#      include <ioctl.h>
#      include <sys/select.h>
#    endif
#  endif

#if defined(DTCONFIG_IMPLIED_SUNOS) &&	/* not in this guy's header files!	*/\
    defined(DTCONFIG_IMPLIED_GNU)
# ifndef DTDEPEND						/* no makefile dependency 			*/
#   include <sys/ioctl.h>
# endif
  int accept(int, struct sockaddr *, int *);
  int bind(int, const struct sockaddr *, int);
  int connect(int, const struct sockaddr *, int);
  int getpeername(int, struct sockaddr *, int *);
  int getsockname(int, struct sockaddr *, int *);
  int getsockopt(int, int, int, void *, int *);
  int listen(int, int);
  int setsockopt(int, int, int, const void *, int);
  int shutdown(int, int);
  int socket(int, int, int);
  int recv(int, void *, size_t, unsigned int);
  int send(int, const void *, size_t, unsigned int);
  int ioctl(int, int, void *);
#endif

#ifndef DTCONFIG_IMPLIED_MSW
# define HAVE_SIGCHLD
#endif

#if !defined(DTCONFIG_IMPLIED_MSW) && \
    !defined(DTCONFIG_SP1) && \
    !defined(DTCONFIG_IMPLIED_SOLARIS) 
#define USE_SELECT_CONNECT
#endif
#define USE_SELECT_ACCEPT
#define USE_SELECT_READ
#define USE_SELECT_WRITE

#if defined(DTCONFIG_LG1)				/* only for testing! 				*/
#undef USE_SELECT_CONNECT
#endif

#ifdef SOCKET_IS_FD
#  define CLOSE(FD) if ((FD) != -1) close(FD);
#else
#  define CLOSE(FD) if ((FD) != -1) closesocket(FD);
# endif

#ifdef HAVE_POSIX_SIGNALS				/* posix signals at all?			*/

struct  mysigaction {
        void            (*handler)(int);
        sigset_t        sa_mask;
        int             sa_flags;
};

/*..........................................................................*/
static
void sigusr1_handler(int signo)
{
}

/*..........................................................................*/
static int sigpipe;
static
void sigpipe_handler(int signo)
{
  sigpipe = 1;
}

#endif									/* posix signals at all?			*/

/*..........................................................................*/
static
dt_rc_e
dtipc_rocket_release(					/* release allocated resources		*/
  dt_ctl_t *ctl,
  priv_t *priv)
{
  DT_F("dtipc_rocket_release");
  dt_rc_e rc = DT_RC_GOOD;

  if (priv != NULL)
  {
    DT_I(dtos_free2,(ctl, priv,
      F, "priv"));
  }

  return rc;
}

/*..........................................................................
| NAME
| dtipc_rocket_read() - read buffer from file
| 
| SUMMARY
| Reads data from file.
| Only makes one try.
| Zero bytes returned in ~*got~ probably means end of file.
| Not an error when ~*got~ is returned less than ~want~.
| 
| END
 *..........................................................................*/

static
dt_rc_e
dtipc_rocket_read(						/* read buffer from file			*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtipc_t *ipc,							/* file object descriptor 			*/
  void *buf,							/* buffer to put data into			*/
  long want,							/* maximum number of bytes wanted 	*/
  long *got)							/* returned number of bytes gotten 	*/
{
  DT_F("dtipc_rocket_read");
  priv_t *priv = (priv_t *)ipc->priv;
  long mask = DT_DBG_MASK_IO |
    (want > 1? DT_DBG_MASK_BIGIO: 0);
  long e;
  int sysrc;

  if (ipc->type != DTIPC_TYPE_SOCKET)
    return dt_err(ctl, F, 
      "ipc type %d does not match %d",
      ipc->type, DTIPC_TYPE_SOCKET);

# ifdef POLL_BEFORE_RW
  DT_Q(dtipc_rocket_poll,(ctl,			/* wait for incoming data			*/
    ipc, DTIPC_POLL_READ, -1,
    "incoming data",
    NULL));
# endif

  if (want > DTCONFIG_READ_MAX)
    want = DTCONFIG_READ_MAX;

  sysrc = recv(ipc->datahandle,			/* do the read raw					*/
    (char *)buf, (size_t)want, 0);
  e = LASTERROR;

  if (sysrc == -1 &&					/* connection closed at receiver?	*/
	  e == ECONNRESET)
  {
	sysrc = 0;
  }
  else
  if (sysrc == -1)						/* read failed?						*/
    return dt_err(ctl, F,
      "recv(fd=%d, buf, %ld)"
      DTERRNO_FMT,
      ipc->datahandle, want, 
      DTERRNO_ARG(e));
  
  if (sysrc < 0)						/* return bytes we got				*/
    *got = 0x10000L + sysrc;
  else
    *got = sysrc;

  if (*got > 2)
	DT_Q(dt_yield,(ctl, "received %ld on socket", *got));

  if (DT_DBG(ctl, mask))
  {
	char s[32];

	DT_Q(dtstr_printable,(ctl,
	  buf, *got,
	  s, sizeof(s)-1));

 	dt_dbg(ctl, F, mask,
	  "read %ld of %ld '%s' on sockhandle %d", 
	  *got, want, s, ipc->datahandle);
  }

  return DT_RC_GOOD;
}

/*..........................................................................
| NAME
| dtipc_rocket_write() - write buffer to file
| 
| SUMMARY
| Writes data to file.
| Only makes one try.
| Returns actual number of bytes successfully written.
| Does not return an error if not all ~want~ bytes can be written.
| 
| END
 *..........................................................................*/

static
dt_rc_e
dtipc_rocket_write(						/* write buffer to file				*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtipc_t *ipc,							/* file object descriptor 			*/
  const void *buf,						/* buffer holding data 				*/
  long want,							/* number of bytes to write 		*/
  long *got)							/* returned number of bytes written */
{
  DT_F("dtipc_rocket_write");
  priv_t *priv = (priv_t *)ipc->priv;
  long mask = DT_DBG_MASK_IO |
    (want > 1? DT_DBG_MASK_BIGIO: 0);
  int sysrc;
  long e;
  dt_rc_e rc = DT_RC_GOOD;
# ifdef HAVE_POSIX_SIGNALS
  struct sigaction act, oldact;
# endif

  DTCSTACK_HIGHWATER;

  if (ipc->type != DTIPC_TYPE_SOCKET)	/* check structure integrity 		*/
    return dt_err(ctl, F, 
      "ipc type %d does not match %d",
      ipc->type, DTIPC_TYPE_SOCKET);

  if (want > DTCONFIG_WRITE_MAX)
    return dt_err(ctl, F, 
      "%ld is too many bytes to write at once",
      want);

# ifdef HANDLE_SIGPIPE_ON_WRITE
  DT_MEMSET(&act, 0, sizeof(act));
  act.sa_handler = sigpipe_handler;
  act.sa_flags = 0;
  sigemptyset(&act.sa_mask);
  sigpipe = 0;
  sigaction(SIGPIPE, &act, &oldact);	/* grab sigpipe if recipient dies	*/
# endif

  sysrc = send(ipc->datahandle, 		/* write raw					    */
    (const char *)buf, want, 0);
  e = LASTERROR;

  if (sysrc == -1 &&					/* connection closed at receiver?	*/
	  (e == ECONNRESET ||
	   e == EPIPE))
  {
	sysrc = 0;
  }
  else
  if (sysrc == -1)						/* write failed?					*/
  {
    rc = dt_err(ctl, F,
      "(line %lu) send(sockhandle=%d, buf=0x%08lx, %ld, 0)"
      DTERRNO_FMT,
      __LINE__,
      ipc->datahandle, buf, want, 
      DTERRNO_ARG(e));
	goto cleanup;
  }

  if (sysrc < 0)						/* return bytes we got				*/
    *got = 0x10000L + sysrc;
  else
    *got = sysrc;

										/* NOTE: iqsigna depends on the word */
										/* "sent" at start of this message.	 */
  DT_Q(dt_yield,(ctl, 
    "sent %ld on socket", *got));

  if (DT_DBG(ctl, mask))
    dt_dbg(ctl, F, mask,
      "wrote buf=0x%08lx %ld of %ld on sockhandle %d",
      buf, *got, want, ipc->datahandle);
  
cleanup:
# ifdef HANDLE_SIGPIPE_ON_WRITE
  sigaction(SIGPIPE, &oldact, NULL);	/* replace signal handler			*/
# endif

# ifdef HANDLE_SIGPIPE_ON_WRITE
  if (sigpipe == 1)
	dt_dbg(ctl, F, DT_DBG_MASK_IPC,
      "got sigpipe");
# endif
  
  return rc;
}

/*..........................................................................*/
static
dt_rc_e
dtipc_rocket_parse(						/* parse name and look stuff up		*/
  dt_ctl_t *ctl,
  dtipc_t *ipc,
  const char *name,
  const char *F,
  long *host,
  long *port)
{
  priv_t *priv = (priv_t *)ipc->priv;
  char *s = NULL;
  int n;
  char *hoststr;
  char *portstr;
  char *p;
  int i;
  int isnum;
  struct hostent *hp;
  long e;
  dt_rc_e rc;

  dt_dbg(ctl, F, DT_DBG_MASK_CONFIG,
    "called with name \"%s\"",
    name? name: "NULL");

  if (name == NULL)
  {
    rc = dt_err(ctl, F,
      "NULL socket name");
    goto cleanup;
  }
  else
  if (strlen(name) == 0)
  {
    rc = dt_err(ctl, F,
      "zero-length socket name");
    goto cleanup;
  }

  DT_C(dtos_strdup,(ctl, name, &s));
  DT_C(dtstr_tokenize,(ctl, s, ':', &n));
  if (n == 1)
	DT_CLEANUP((ctl, F,
      "missing colon in \"%s\"", name));
  if (n > 2)
	DT_CLEANUP((ctl, F,
      "too many colons in \"%s\"", name));

  hoststr = s;
  p = s + strlen(s) + 1;
  portstr = p;

  DT_C(dtstr_tokenize,(ctl, p, ',', &n));
  if (n > 2)
	DT_CLEANUP((ctl, F,
      "too many commas in \"%s\"", name))

  if (n > 1)
  {
	p = p + strlen(p) + 1;
	DT_C(dtstr_tokenize,(ctl, p, '+', &n));
	for (i=0; i<n; i++)
	{
	  if (!strcmp(p, "lingershort"))
		ipc->flags |= DTIPC_FLAG_LINGER_SHORT;
	  else
	  if (!strcmp(p, "lingerlong"))
		ipc->flags |= DTIPC_FLAG_LINGER_LONG;
	  else
	  if (!strcmp(p, "noshutdown"))
		ipc->flags |= DTIPC_FLAG_NOSHUTDOWN;
	  else
	    DT_CLEANUP((ctl, F,
	      "invalid flag \"%s\" in \"%s\"", p, name));
	  p = p + strlen(p) + 1;
	}
  }

										/* ................................ */
  DT_Q(dtstr_to_long2,(ctl, portstr,	/* see if port is a number			*/
    port, &isnum));
	
  if (!isnum)							/* not a port number? 				*/
  {
	struct servent *serventry;
	serventry = getservbyname(			/* look up name in /etc/services 	*/
	  portstr, NULL);
	e = LASTERROR;
	if (serventry == 0)
	{
	  rc = dt_err(ctl, F,
		"getservbyname(\"%s\", 0)"
		DTERRNO_FMT,
        name,
        DTERRNO_ARG(e));
	  goto cleanup;
	}
	*port = serventry->s_port;
  }

  DT_C(dtstr_printf,(ctl, priv->name,	/* remember the ipc name            */
    sizeof(priv->name), "%s:%ld",
      hoststr, *port));
	
# if defined(DTCONFIG_IMPLIED_LINUX)
  {
    struct in_addr host_addr;
    if (inet_aton(hoststr, &host_addr))
	  *host = host_addr.s_addr;
	else
	  *host = -1;
  }
# else
    *host = inet_addr(hoststr);			/* see if it's an IP address 		*/
# endif
  if (*host == -1)						/* not an IP address? 				*/
  {
    dt_dbg(ctl, F, DT_DBG_MASK_IPC,
      "resolving host \"%s\"",
      hoststr);

	hp = gethostbyname(hoststr);		/* look up host name				*/
	e = LASTERROR;
	if (hp == NULL)
	{
	  rc = dt_err(ctl, F,
	    "gethostbyname(\"%s\")"
		DTERRNO_FMT,
        hoststr,
        DTERRNO_ARG(e));
	  goto cleanup;
	}
  
	*host = ((struct in_addr *)			/* get IP address from name 		*/
      (hp->h_addr))->s_addr;

    dt_dbg(ctl, F, DT_DBG_MASK_IPC,
      "network address of \"%s\""
      " resolves to %s",
      hoststr, inet_ntoa(
        *((struct in_addr *)host))); 
  }
  else
	dt_dbg(ctl, F, DT_DBG_MASK_IPC,
      "\"%s\" is dotted quad for 0x%08lx",
      hoststr, *host);

cleanup:

  if (s)
	DT_I(dtos_free2,(ctl, s, F, s));
  return rc;
}

/*..........................................................................
 * Make sure system is ready for socket calls.
 * For Windows sockets, call WSAStartup().
 *..........................................................................*/
static
dt_rc_e
dtipc_rocket_startup(
  dt_ctl_t *ctl)
{
  DT_F("dtipc_rocket_startup");
# ifdef DTCONFIG_IMPLIED_MSW
# ifndef MAKEWORD
#    define MAKEWORD(a, b)      ((WORD)(((BYTE)(a)) | (((WORD)((BYTE)(b))) << 8)))
# endif
  WORD request = MAKEWORD(1, 1);
  WSADATA w;
  int rc;
  DT_MEMSET(&w, 0, sizeof(w));
  rc = WSAStartup(request, &w);
  dt_dbg(ctl, F, DT_DBG_MASK_IPC,
    "winsock version 0x%02x, high 0x%02x",
    w.wVersion, w.wHighVersion);
  dt_dbg(ctl, F, DT_DBG_MASK_IPC,
    "winsock description \"%s\"",
    w.szDescription);
  dt_dbg(ctl, F, DT_DBG_MASK_IPC,
    "winsock system status \"%s\"",
    w.szSystemStatus);
  if (rc != 0)
    return dt_err(ctl, F,
      "WSAStartup error %d", rc);
# else
# endif
  return DT_RC_GOOD;
}

/*..........................................................................
 * Tell the system that we no longer need sockets.
 * The program may re-enable sockets later.
 * For Windows sockets, call WSACleanup().
 *..........................................................................*/
static
dt_rc_e
dtipc_rocket_cleanup(
  dt_ctl_t *ctl)
{
# ifdef DTCONFIG_IMPLIED_MSW
#ifdef NONO
  DT_F("dtipc_rocket_cleanup");
  if (WSACleanup() != 0)
    return dt_err(ctl, F,
      "WSACleanup error %ld",
      WSAGetLastError);
#endif
# else
# endif
  return DT_RC_GOOD;
}

/*..........................................................................
 * Change a socket handle into a file descriptor.
 * Assume the socket as just been created using socket() or accept().
 * Also undo the temporary effects of dtipc_rocket_prepare().
 *..........................................................................*/
static
dt_rc_e
dtipc_rocket_fixup(						/* fix up socket for read/write 	*/
  dt_ctl_t *ctl,
  dtipc_t *ipc,
  long sockhandle)
{
  DT_F("dtipc_rocket_fixup");
  priv_t *priv = (priv_t *)ipc->priv;
  int sysrc;
  long e;
  int i = 0;

# if 1
  while(1)
  {
	int on = 1;
    dt_dbg(ctl, F, DT_DBG_MASK_IPC,
      "%s setting TCP_NODELAY",
      priv->name);
	sysrc = setsockopt(sockhandle, 
      IPPROTO_TCP,
      TCP_NODELAY, 
	  (char *)&on, sizeof(on));
	e = LASTERROR;
    if (sysrc == 0)
	  break;
	if (i >= 4 ||						/* too many retries? 				*/
		errno != ENOPROTOOPT)			/* or something can't be retried?	*/
	  return dt_err(ctl, F,
		"setsockopt(%ld,"
		" SOL_SOCKET, TCP_NODELAY,"
		" 0x%08lx={%d}, %d) retry=%d"
        DTERRNO_FMT,
		sockhandle,
		&on, on, sizeof(on), i,
		DTERRNO_ARG(e));
	DT_Q(dtos_delay,(ctl, 100));		/* wait a short while 				*/
	i++;
  }
# else
    dt_dbg(ctl, F, DT_DBG_MASK_IPC,
      "%s not setting TCP_NODELAY",
      priv->name);
# endif

# if 1
  {
	struct linger ling;
	if (ipc->flags & 
		DTIPC_FLAG_LINGER_SHORT)
	{
	  ling.l_onoff = 1;
	  ling.l_linger = 1;				/* linger time in seconds			*/
	}
	else
	if (ipc->flags & 
		DTIPC_FLAG_LINGER_LONG)
	{
	  ling.l_onoff = 1;
	  ling.l_linger = 5;				/* linger time in seconds			*/
	}
	else
	{
	  ling.l_onoff = 0;					/* immediate release when 0			*/
	  ling.l_linger = 1;				/* linger time in seconds			*/
	}
    dt_dbg(ctl, F, DT_DBG_MASK_IPC,
      "%s setting LINGER %d, %d",
      priv->name,
 	 ling.l_onoff,
	 ling.l_linger);
	sysrc = setsockopt(sockhandle,
		SOL_SOCKET, SO_LINGER,
		(char *)&ling, sizeof(ling));
	e = LASTERROR;
	if (sysrc < 0)
      return dt_err(ctl, F,
		"setsockopt(%ld,"
		" SOL_SOCKET, SO_LINGER,"
		" 0x%08lx={%d,%d}, %d)"
		DTERRNO_FMT,
 		sockhandle,
		&ling, 
		ling.l_onoff,
		ling.l_linger,
		sizeof(ling),
		DTERRNO_ARG(e));
  }
# else
    dt_dbg(ctl, F, DT_DBG_MASK_IPC,
      "%s not setting LINGER",
      priv->name);
# endif

# if 0
  {
	int off = 0;
    dt_dbg(ctl, F, DT_DBG_MASK_IPC,
      "%s setting KEEPALIVE %d",
      priv->name, off);
	sysrc = setsockopt(sockhandle,
		SOL_SOCKET, SO_KEEPALIVE,
		(char *)&off, sizeof(off));
	e = LASTERROR;
	if (sysrc < 0)
      return dt_err(ctl, F,
		"setsockopt(%ld,"
		" SOL_SOCKET, SO_KEEPALIVE,"
		" 0x%08lx={%d}, %d)"
		DTERRNO_FMT,
 		sockhandle,
		&off, off, sizeof(off), 
		DTERRNO_ARG(e));
  }
# else
    dt_dbg(ctl, F, DT_DBG_MASK_IPC,
      "%s not setting KEEPALIVE",
      priv->name);
# endif

  return DT_RC_GOOD;
}

/*..........................................................................
 * Prepare the system for a call to socket() or accept().
 * For Windows sockets, set thread option for synchronous sockets
 * so that a socket handle can be changed into a file descriptor.
 *..........................................................................*/
static
dt_rc_e
dtipc_rocket_prepare(
  dt_ctl_t *ctl,
  dtipc_t *ipc)
{
# ifdef DTCONFIG_IMPLIED_MSW
# else
# endif
  return DT_RC_GOOD;
}

/*..........................................................................*/
static
dt_rc_e
dtipc_rocket_register(					/* register service name			*/
  dt_ctl_t *ctl,
  dtipc_t *ipc,
  const char *name)
{
  DT_F("dtipc_rocket_register");
  priv_t *priv = (priv_t *)ipc->priv;
  int startup = 0;
  long host;
  long port, tryport;
  int sockhandle = -1;
  long e;
  int queue;
  int on = 1;
  dt_rc_e rc;

  DT_C(dtipc_rocket_startup,(ctl));
  startup = 1;

  DT_C(dtipc_rocket_parse,(ctl,			/* parse name into host:port 		*/
    ipc, name, F,
    &host, &port));

  if (ipc->flags &						/* not supposed to queue connects?	*/
      DTIPC_FLAG_NOQUEUE)
	queue = 0;
  else
	queue = 25;
	
  if (ipc->flags &						/* supposed to background ourself?	*/
      DTIPC_FLAG_SELF_BACKGROUND)
# ifdef DTCONFIG_IMPLIED_MSW
  {
	rc = dt_err(ctl, F,
      "cannot background in this OS"
      " (yet anyway)");
	goto cleanup;
  }
# else
  {
    int sysrc;
	struct mysigaction my;
	sigset_t sigusr1, oset;
	struct sigaction act, tca;
	sigprocmask(SIG_BLOCK,				/* query list of blocked signals	*/
      NULL, &sigusr1);
	sigaddset(&sigusr1, SIGUSR1);		/* block our signal too				*/
	sigprocmask(SIG_SETMASK,
      &sigusr1, &oset);
	sigdelset(&sigusr1, SIGUSR1);		/* unblock our signal at pause		*/

	my.handler = sigusr1_handler;
	DT_MEMCPY(&act, &my, sizeof(act));

	act.sa_flags = 0;
	sigemptyset(&act.sa_mask);
	sigaction(SIGUSR1, &act, &tca);		/* set up our signal handler	    */
    sysrc = fork();
	e = LASTERROR;
    if (sysrc == -1)
	{
      rc = dt_err(ctl, F,
        "fork()" 
        DTERRNO_FMT,
        DTERRNO_ARG(e));
	  goto cleanup;
	}
    else
    if (sysrc != 0)						/* we are the parent?			    */
    {
	  sigsuspend(&sigusr1);				/* sit around waiting for signal    */
      sigprocmask(SIG_SETMASK,			/* replace original signal state	*/
        &oset, NULL);
	  sigaction(SIGUSR1,				/* replace original handler    		*/
        (struct sigaction *)&tca,	
        NULL);

	  if (DT_DBG(ctl, DT_DBG_MASK_IPC))
	  {
		DT_C(dtos_delay,(ctl, 1000));	/* keep output from interleaving 	*/
		dt_dbg(ctl, F, DT_DBG_MASK_IPC,
          "%s is parent and now has"
          " confirmation of child (%d)",
          priv->name, sysrc);
	  }

      ipc->flags &=						/* let caller know this is stub	    */
        ~DTIPC_FLAG_SELF_BACKGROUND;

      rc = DT_RC_GOOD;
	  goto cleanup;
    }

      sigprocmask(SIG_SETMASK,			/* replace original signal state	*/
        &oset, NULL);
	  sigaction(SIGUSR1, 
        (struct sigaction *)&tca,		/* replace original handler    		*/
        NULL);

    dt_dbg(ctl, F, DT_DBG_MASK_IPC,
      "%s child taking over",
      priv->name);
  }
# endif

  ipc->type = DTIPC_TYPE_SOCKET;
	
  {
	struct sockaddr_in server;
	int sysrc;
	sockhandle = socket(AF_INET,		/* make the socket 					*/
	  SOCK_STREAM, IPPROTO_TCP);
	e = LASTERROR;
	if (sockhandle < 0)
	{
	  rc = dt_err(ctl, F,
		"socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)"
        DTERRNO_FMT,
        DTERRNO_ARG(e));
	  goto cleanup;
	}
	
    sysrc = setsockopt(sockhandle,		/* immediate release at close		*/
      SOL_SOCKET, SO_REUSEADDR,
      (char *)&on, sizeof(on));
	e = LASTERROR;
    if (sysrc < 0)
	{
	  rc = dt_err(ctl, F,
        "setsockopt(%d,"
        " SOL_SOCKET, SO_REUSEADDR,"
        " 0x%08lx, %d)"
        DTERRNO_FMT,
		sockhandle,
        &on, sizeof(on),
        DTERRNO_ARG(e));
	  goto cleanup;
	}

	if (port == 0)						/* supposed to search for port? 	*/
	  tryport = 2000;					/* starting point for search 		*/
	else								/* port explicit? 					*/
	  tryport = port;					/* accept no other 					*/

	while (1)							/* search for port 					*/
	{
	  DT_MEMSET(&server, '\0',
		     sizeof(server));
	  server.sin_addr.s_addr = host;	/* specify host we parsed 			*/
	  server.sin_port = htons(tryport);	/* specify port we parsed 			*/
	  server.sin_family = AF_INET;
	
	  sysrc = bind(sockhandle,			/* bind server to service port		*/
        (struct sockaddr *)&server,
	    sizeof(server));
	  e = LASTERROR;
	  if (sysrc == 0)					/* bind succeeded? 					*/
	    break;
	  if (errno == EADDRINUSE &&		/* port in use? 					*/
		  port == 0)					/* and we're supposed to search? 	*/
		tryport++;						/* increment and try again			*/
	  else								/* bind just failed? 				*/
	  {
		rc = dt_err(ctl, F,
          "bind(%d, 0x%08lx, %d) host 0x%08lx port %ld"
          DTERRNO_FMT,
          sockhandle, 
          &server, sizeof(server), 
          host, port,
          DTERRNO_ARG(e));
	    goto cleanup;
	  }
	}

	port = tryport;
	
	sysrc = listen(sockhandle, queue);	/* mark socket as a listener		*/
	e = LASTERROR;
	if (sysrc < 0)
	{
	  rc = dt_err(ctl, F,
        "listen(%d, %d)"
        DTERRNO_FMT,
        sockhandle, queue,
        DTERRNO_ARG(e));
	  goto cleanup;
	}
	
	ipc->selectfd = sockhandle;			/* we can select for ipc input		*/
	DT_C(dtstr_printf,(ctl,
      (char *)ipc->server_spec,
      sizeof(ipc->server_spec),
      "rocket,%s:%d",
     inet_ntoa(
       *((struct in_addr *)&host)), 
	  ntohs(server.sin_port)));
	dt_dbg(ctl, F, DT_DBG_MASK_IPC,
      "%s sockhandle %d bound"
      " to listen on %s queue=%d",
      priv->name, ipc->selectfd, 
      ipc->server_spec, queue);
  }

cleanup:
  if (rc != DT_RC_GOOD && sockhandle != -1)
    CLOSE(sockhandle);

# ifdef HAVE_POSIX_SIGNALS
  if (ipc->flags &						/* we backgrounded ourselves?		*/
      DTIPC_FLAG_SELF_BACKGROUND)
  {
    int ppid = getppid();
    int sysrc = kill(ppid, SIGUSR1);	/* tell parent it can leave			*/
    if (sysrc == -1)
      return dt_err(ctl, F,
        "kill(%d, SIGUSR1)"
        DTERRNO_FMT,
        ppid, DTERRNO_ARG(errno));
    else
      dt_dbg(ctl, F, DT_DBG_MASK_IPC,
        "%s SIGUSR1 sent to parent %d",
        priv->name, ppid);
  }
#endif
  
  if (rc != DT_RC_GOOD && startup)
    DT_Q(dtipc_rocket_cleanup,(ctl));

  return rc;
}


/*..........................................................................*/
static
dt_rc_e
dtipc_rocket_unregister(				/* unregister service name			*/
  dt_ctl_t *ctl,
  dtipc_t *ipc)
{
  DT_F("dtipc_rocket_unregister");
  priv_t *priv = (priv_t *)ipc->priv;
  dt_rc_e rc = DT_RC_GOOD;

  dt_dbg(ctl, F, DT_DBG_MASK_IPC,
    "%s server sockhandle %d closed",
    priv->name, ipc->selectfd);

  if (ipc->selectfd != -1)
    CLOSE(ipc->selectfd);

  ipc->type = DTIPC_TYPE_NONE;
  ipc->selectfd = -1;

  DT_I(dtipc_rocket_cleanup,(ctl));

  return rc;
}

/*..........................................................................*/
static
dt_rc_e
dtipc_rocket_unfind(					/* unfind registered service name	*/
  dt_ctl_t *ctl,
  dtipc_t *ipc)
{
  DT_F("dtipc_rocket_unfind");
  priv_t *priv = (priv_t *)ipc->priv;
  dt_rc_e rc = DT_RC_GOOD;

  dt_dbg(ctl, F, DT_DBG_MASK_IPC,
    "%s client data socket %d closing shutdown=%d",
    priv->name, 
    ipc->datahandle,
    !(ipc->flags & DTIPC_FLAG_NOSHUTDOWN));
  
# ifdef SOCKET_IS_FD
    close(ipc->datahandle);
# else
  if (ipc->datahandle != -1)
  {
	if (!(ipc->flags &
		  DTIPC_FLAG_NOSHUTDOWN))
	  shutdown(ipc->datahandle, 2);		/* disable the socket			    */
    closesocket(ipc->datahandle);		/* close the socket itself 			*/
  }
# endif

  ipc->type = DTIPC_TYPE_NONE;
  DT_I(dtipc_rocket_cleanup,(ctl));

  return rc;
}

/*..........................................................................*/
static
dt_rc_e
dtipc_rocket_done(						/* server is done with connection   */
  dt_ctl_t *ctl,
  dtipc_t *ipc)
{
  DT_F("dtipc_rocket_done");
  priv_t *priv = (priv_t *)ipc->priv;

  dt_dbg(ctl, F, DT_DBG_MASK_IPC,
    "%s server data socket %d closing shutdown=%d",
    priv->name, 
    ipc->datahandle,
    !(ipc->flags & DTIPC_FLAG_NOSHUTDOWN));

  if (ipc->datahandle != -1)
# ifdef SOCKET_IS_FD
    close(ipc->datahandle);
# else
  {
  if (!(ipc->flags &
	  DTIPC_FLAG_NOSHUTDOWN))
  {
#   define MAXWAIT (3.0)				/* 3 seconds max to close 			*/
	int isready;
	dtos_time_t beg, end;
	double elapsed = 0.0;
	int sysrc;
	long e;
	shutdown(ipc->datahandle, SD_SEND);	/* cause FIN to be sent 			*/

	DT_Q(dtos_time,(ctl, &beg));
  again:
	DT_Q(dtipc_rocket_poll,(ctl,		/* wait for incoming data			*/
      ipc, DTIPC_POLL_READ,
      (long)(1000.0*MAXWAIT-elapsed),
      "peer to close", &isready));
	DT_Q(dtos_time,(ctl, &end));
    elapsed = end.wall - beg.wall;
	if (isready)						/* incoming data ready to read? 	*/
	{
	  char buf[4096];
	  sysrc = recv(ipc->datahandle,		/* do the read raw					*/
        buf, sizeof(buf), 0);
	  e = LASTERROR;
  	  dt_dbg(ctl, F, DT_DBG_MASK_IPC,
        "after shutdown recv()=%d e=%ld after %0.4f seconds", 
        sysrc, e, elapsed);
	}
	if (isready && sysrc>0 &&
		elapsed < MAXWAIT)
	  goto again;

	if (elapsed >= MAXWAIT)
	  dt_dbg(ctl, F, DT_DBG_MASK_IPC,
        "peer did not respond to closesocket withing %0.2f seconds",
        elapsed);
  }

    closesocket(ipc->datahandle);		/* close the socket itself 			*/
  }
#endif
  ipc->datahandle = -1;
  
  return DT_RC_GOOD;
}

/*..........................................................................*/
static
dt_rc_e
dtipc_rocket_uninit(					/* server is done with connection   */
  dt_ctl_t *ctl,
  dtipc_t *ipc)
{
  DT_F("dtipc_rocket_done");
  priv_t *priv = (priv_t *)ipc->priv;
  dt_rc_e rc = DT_RC_GOOD;

  DT_I(dtipc_rocket_release,(ctl,		/* release resources 				*/
    priv));

  DT_MEMSET(ipc, 0, sizeof(*ipc));		/* clear structure				    */

  return rc;
}

/*..........................................................................*/
static
dt_rc_e
dtipc_rocket_split(						/* split server/client socket		*/
  dt_ctl_t *ctl,
  dtipc_t *ipc1,
  dtipc_t *ipc2)
{
  DT_F("dtipc_rocket_split");
  priv_t *priv1 = (priv_t *)ipc1->priv;
  priv_t *priv2;
  dt_rc_e rc;

  DT_C(dtos_malloc2,(ctl,				/* space for client's priv 			*/
    (void **)&priv2, sizeof(*priv2),
    F, "priv2"));
  
  DT_MEMCPY(ipc2, ipc1, sizeof(*ipc2));	/* copy structure body 				*/
  ipc2->priv = (void *)priv2;			/* split the privs 					*/
  *priv2 = *priv1;						/* copy the priv body 				*/

cleanup:
  if (rc != DT_RC_GOOD)
	DT_MEMSET(ipc2, 0, sizeof(*ipc2));		/* clear structure				    */

  return rc;
}
/*..........................................................................*/
static int signal_sigchld = 0;

#ifdef HAVE_SIGCHLD
static
void signal_handler(int signum)
{
  switch(signum)
  {
	case SIGCHLD:  
      signal_sigchld = 1; 
	break;
  }
}
#endif

/*..........................................................................*/
static
dt_rc_e
dtipc_rocket_child(						/* report on child ending			*/
  dt_ctl_t *ctl,
  dtipc_t *ipc)
{
#ifdef HAVE_SIGCHLD
  DT_F("dtipc_rocket_child");
  priv_t *priv = (priv_t *)ipc->priv;
  while (1)
  {
	int status;
	pid_t dead_pid;
	char s[64];
	dead_pid = waitpid(-1, &status,		/* let the zombies disappear 		*/
      WNOHANG);
    if (dead_pid == -1 ||				/* no more zombies? 				*/
		dead_pid == 0)					/* some children still alive?	    */
      break;
	if (WIFEXITED(status))
      dtstr_printf(ctl, s, sizeof(s),
        "exited normally with code %d",
        WEXITSTATUS(status));
	else
	if (WIFSIGNALED(status))
      dtstr_printf(ctl, s, sizeof(s),
        "terminated by signal %d",
        WTERMSIG(status));
	else
	if (WIFSTOPPED(status))
      dtstr_printf(ctl, s, sizeof(s),
        "stopped by signal %d",
        WSTOPSIG(status));
	else
      dtstr_printf(ctl, s, sizeof(s),
        "terminated with unknown status 0x%04x",
        status);
	
	dt_dbg(ctl, F, DT_DBG_MASK_IPC,
      "%s noticed [%5d] %s",
      priv->name, dead_pid, s);
  }
#endif
  return DT_RC_GOOD;
}

/*..........................................................................
 * timeout of -1 means wait forever
 *..........................................................................*/
static
dt_rc_e
dtipc_rocket_poll(						/* wait for socket to be active		*/
  dt_ctl_t *ctl,
  dtipc_t *ipc,
  dtipc_poll_e what,
  long milliseconds,
  const char *waiting_for,
  int *pisready)
{
  DT_F("dtipc_rocket_poll");
  priv_t *priv = (priv_t *)ipc->priv;
  fd_set fds;
  int in = what & DTIPC_POLL_ACCEPT || 
           what & DTIPC_POLL_READ;
  int sockhandle;
  long e;
  dtipc_poll_e isready = (dtipc_poll_e)0;
  int first = -1;
  int ready = -1;
  const char *w4;

# ifndef USE_SELECT_ACCEPT
  if (what & DTIPC_POLL_ACCEPT)
    return DT_RC_GOOD;
# endif

# ifndef USE_SELECT_CONNECT
  if (what & DTIPC_POLL_CONNECT)
    return DT_RC_GOOD;
# endif

# ifndef USE_SELECT_READ
  if (what & DTIPC_POLL_READ)
    return DT_RC_GOOD;
# endif

# ifndef USE_SELECT_WRITE
  if (what & DTIPC_POLL_WRITE)
    return DT_RC_GOOD;
# endif

  if (waiting_for != NULL)
	w4 = waiting_for;
  else
  {
    if (what & DTIPC_POLL_ACCEPT)
	  w4 = "ACCEPT";
    if (what & DTIPC_POLL_CONNECT)
	  w4 = "CONNECT";
    if (what & DTIPC_POLL_READ)
	  w4 = "READ";
    if (what & DTIPC_POLL_WRITE)
	  w4 = "WRITE";
  }

  if (what & DTIPC_POLL_ACCEPT ||
	  what & DTIPC_POLL_CONNECT)
    sockhandle = ipc->selectfd;
  else
    sockhandle = ipc->datahandle;

  if (sockhandle == -1)
    return dt_err(ctl, F,
      "invalid sockhandle -1:"
      " probably wrong poll type %d",
      what);

  while(1)
  {
	struct timeval timeout;
	int sysrc;

	if (first == 1 &&
		waiting_for != NULL &&
		(!(what & DTIPC_POLL_READ) && DT_DBG(ctl, DT_DBG_MASK_IPC) ||
		(DT_DBG(ctl, DT_DBG_MASK_IPC)) && DT_DBG(ctl, DT_DBG_MASK_PIXEL)))
	{	
      dt_dbg(ctl, F, DT_DBG_MASK_IPC,
        "waiting for %s on sockhandle %d",
        waiting_for, sockhandle);
	  first = 0;
	}

	if (first != -1)
	{
	  DT_Q(dt_yield,(ctl, "waiting for %s", w4));
	}

	if (milliseconds == 0)				/* no waiting?						*/
	{
	  timeout.tv_sec = 0;				/* loop poll time	 				*/
	  timeout.tv_usec = 0;
	}
	else
	if (milliseconds == -1)				/* waiting forever? 				*/
	{
	  timeout.tv_sec = 1;				/* loop poll time one second		*/
	  timeout.tv_usec = 0;
	}
	else
	if (milliseconds < 1000)			/* very short wait?					*/
	{
	  timeout.tv_sec = 
        milliseconds/1000;
	  timeout.tv_usec = 
        (milliseconds%1000)*1000;
	}
	else								/* waiting longer than one sec?	    */
	{
	  timeout.tv_sec = 1;				/* loop poll time one second		*/
	  timeout.tv_usec = 0;
	}

	FD_ZERO(&fds);
	FD_SET(sockhandle, &fds);			/* wait for activity				*/
	sysrc = select(sockhandle+1,
      in? &fds: (fd_set *)NULL, 
      in? (fd_set *)NULL: &fds, 
      NULL, &timeout);
	e = LASTERROR;
	if (sysrc == -1 &&
		e == EINTR)
	{	
	  if (signal_sigchld == 1)			/* a sub process died?		    	*/
	  {
		signal_sigchld = 0;
		if (what & DTIPC_POLL_SUBEND)	/* we are waiting for this? 		*/
		{
		  isready = DTIPC_POLL_SUBEND;	/* let caller know it happened 		*/
		  break;
		}
		else
		if (!DTIPC_FLAG_IGNORE_SIGCHLD)	/* supposed to reap zombies? 		*/
		  DT_Q(dtipc_rocket_child,(ctl,	/* handle zombie cleanup ourselves 	*/
            ipc));
	  }
	  else								/* some other signal? 				*/
        return dt_err(ctl, F,
	      "%s got unexpected signal",
          priv->name);
	}
	else
	if (sysrc == -1)					/* other failure in select()?	    */
      return dt_err(ctl, F,
        "%s select(%d, fds,"
        " NULL, NULL, (%ld,%ld)) EINTR %d"
        " waiting for %s"
        DTERRNO_FMT,
        priv->name,
        sockhandle+1,
        timeout.tv_sec, timeout.tv_usec, EINTR,
        w4,
        DTERRNO_ARG(e));

	ready = FD_ISSET(sockhandle, &fds);
	if (ready)							/* input available? 				*/
	{
	  isready = (dtipc_poll_e)((unsigned long)what & ~DTIPC_POLL_SUBEND);
	  break;
	}

	if (first == -1)
	  first = 1;

	if (milliseconds == 0)				/* this is a poll?				    */
	  break;

	if (milliseconds != -1)				/* not waiting forever?			    */
	{
	  milliseconds -= 1000;
	  if (milliseconds <= 0)			/* waited long enough?			    */
	    break;
	}
  }

  if (pisready != NULL)					/* caller wants to know? 			*/
    *pisready = isready;

  return DT_RC_GOOD;
}
  
/*..........................................................................*/
static
dt_rc_e
dtipc_rocket_wait(						/* wait for request					*/
  dt_ctl_t *ctl,
  dtipc_t *ipc)
{
  DT_F("dtipc_rocket_wait");
  priv_t *priv = (priv_t *)ipc->priv;
  int is_ready;

  ipc->datahandle = -1;
  
listen:

  do {
	DT_Q(dtipc_rocket_poll,(ctl,		/* wait for incoming connection 	*/
      ipc, DTIPC_POLL_ACCEPT, 30000,
      "connection from a client",
      &is_ready));
	if (DTIPC_FLAG_IGNORE_SIGCHLD)		/* supposed to reap zombies? 		*/
	  DT_Q(dtipc_rocket_child,(ctl,		/* check on children 				*/
        ipc));
  } while (!is_ready);
										/* ................................ */
  {
    int sockhandle;
	long e;
    struct sockaddr_in client;
    int clientlen;
	struct hostent *hp;

    DT_Q(dtipc_rocket_prepare,(ctl,		/* prepare to make a socket			*/
      ipc));

    clientlen = sizeof(client);
    sockhandle = accept(				/* first accept connection			*/
      ipc->selectfd,
      (struct sockaddr *)&client,
      &clientlen);
	e = LASTERROR;
    if (sockhandle < 0)
	  return dt_err(ctl, F,
        "accept(%d, 0x%08lx, &%d)"
        DTERRNO_FMT,
        ipc->selectfd, &client,
        clientlen,
        DTERRNO_ARG(e));


	ipc->datahandle = sockhandle;

#ifdef NONO
    dt_dbg(ctl, F, DT_DBG_MASK_IPC,
      "resolving IP address %s",
      inet_ntoa(client.sin_addr));		/* IP address in string form		*/

    hp = gethostbyaddr(					/* look up host name 				*/
      (char *)&client.sin_addr,
      sizeof(struct in_addr),
      AF_INET);
#else
    hp = NULL;
#endif

	DT_Q(dtstr_printf,(ctl,
      (char *)ipc->client_spec,
      sizeof(ipc->client_spec),
      "rocket,%s:%d",
      hp && hp->h_name?
        hp->h_name: 
        inet_ntoa(client.sin_addr),
      ntohs(client.sin_port)));

    dt_dbg(ctl, F, DT_DBG_MASK_IPC,
      "%s accepted connection"
      " from %s on sockhandle %d",
      priv->name, ipc->client_spec,
      sockhandle);

#if 1
    DT_Q(dtipc_rocket_fixup,(ctl, ipc,	/* condition the socket			    */
      sockhandle));
#else
    dt_dbg(ctl, F, DT_DBG_MASK_IPC,
      "%s no fixup being done on sockhandle %d",
      priv->name, sockhandle);
    dt_dbg(ctl, F, DT_DBG_MASK_IPC, "waiting 5000 ms");
    DT_Q(dtos_delay,(ctl, 5000L));
    dt_dbg(ctl, F, DT_DBG_MASK_IPC, "waited 5000 ms");
#endif
  }

										/* ................................ */
  if (ipc->flags &						/* fork after waiting?			    */
	  DTIPC_FLAG_FORK_AFTER_WAIT)
  {
#   ifdef DTCONFIG_IMPLIED_MSW			/* Win32 returns always child	    */
	  dt_dbg(ctl, F, DT_DBG_MASK_IPC,
		"%s NOT closing listen socket %d",
        priv->name, ipc->selectfd);
#ifdef NONO
	  close(ipc->selectfd);				/* don't need the listen socket 	*/
	  ipc->selectfd = -1;
#endif

#     ifdef NONO
  	    return dt_err(ctl, F,
        "cannot fork in this OS"
        " (yet anyway)");
#     endif
#   else
	void (*old_handler)(int) = NULL;
	void (*new_handler)(int) = NULL;
	int sysrc;
	long e;

	if (old_handler == NULL)			/* first time here? 				*/
  	  old_handler = signal(SIGCHLD,
        signal_handler);
	else
  	  new_handler = signal(SIGCHLD,
        signal_handler);
	e = LASTERROR;
	if ((long)old_handler == -1 ||
		(long)new_handler == -1)
	  return dt_err(ctl, F,
        "signal(SIGCHLD)"
        DTERRNO_FMT,
        DTERRNO_ARG(e));
	
	sysrc = fork();						/* create separate process 			*/
	e = LASTERROR;
	if (sysrc == -1)					/* fork failed? 					*/
	  return dt_err(ctl, F,
        "fork()"
        DTERRNO_FMT,
        DTERRNO_ARG(e));
	else								
	if (sysrc != 0)						/* we are the parent?			    */
	{
	  DT_Q(dtos_delay,(ctl, 1000));		/* let client get settled 			*/

	  dt_dbg(ctl, F, DT_DBG_MASK_IPC,
        "forked child %d",
        sysrc);

	  dt_dbg(ctl, F, DT_DBG_MASK_IPC,
		"%s closing data socket %d",
        priv->name, ipc->datahandle);
    
	  CLOSE(ipc->datahandle);
      ipc->datahandle = -1;

	  goto listen;						/* listen for more connections 		*/
	}
	else								/* we are the child?			    */
	{
	  dt_dbg(ctl, F, DT_DBG_MASK_IPC,
		"%s closing listen socket %d",
        priv->name, ipc->selectfd);

	  CLOSE(ipc->selectfd);				/* don't need the listen socket 	*/
	  ipc->selectfd = -1;
	}

	if (old_handler != NULL)			/* we changed signal handlers? 		*/
	{
	  new_handler = signal(SIGCHLD,		/* replace old signal handler		*/
        old_handler);
	  e = LASTERROR;
	  if ((long)new_handler == -1)
	    return dt_err(ctl, F,
          "signal(SIGCHLD)"
          DTERRNO_FMT,
          DTERRNO_ARG(e));
	}

#   endif
  }

  return DT_RC_GOOD;
}

/*..........................................................................*/
static
dt_rc_e
dtipc_rocket_isipc(						/* see if it's one of ours			*/
  dt_ctl_t *ctl,
  dtipc_t *ipc,
  unsigned long id,
  dtipc_header_t *h,
  int *isipc)
{
  DT_F("dtipc_rocket_isipc");
  fd_set fds;
  struct timeval t;
  int sysrc;
  long e;

  DT_MEMSET(&t, 0, sizeof(t));				/* effect a poll					*/
  FD_ZERO(&fds);
  FD_SET(ipc->selectfd, &fds);			/* poll the ipc fd only			    */
  sysrc = select(ipc->selectfd+1,
    &fds, NULL, NULL, &t);
  e = LASTERROR;
  if (sysrc == -1)
    return dt_err(ctl, F,
      "select(%d, fds,"
      " NULL, NULL, {0})"
      DTERRNO_FMT,
      ipc->selectfd+1,
      DTERRNO_ARG(e));

  if (FD_ISSET(ipc->selectfd, &fds))	/* input available from ipc?		*/
  {
	DT_Q(dtipc_rocket_wait,(ctl, ipc));	/* accept the connection 			*/
    ipc->type = DTIPC_TYPE_SOCKET;
    *isipc = 1;
  }
  else
    *isipc = 0;

  return DT_RC_GOOD;
}

/*..........................................................................*/
static
dt_rc_e
dtipc_rocket_find(						/* find registered service name		*/
  dt_ctl_t *ctl,
  dtipc_t *ipc,
  const char *name,
  long timeout)
{
  DT_F("dtipc_rocket_find");
  priv_t *priv = (priv_t *)ipc->priv;
  int startup = 0;
  int sysrc;
  long e;
  long host;
  long port;
  struct sockaddr_in client;
  long sockhandle = -1;
  dtos_time_t start;
  long remaining;
  long waitinc = 50L;					/* wait increment in milliseconds 	*/
  dt_rc_e rc;

  ipc->datahandle = -1;

  DT_C(dtipc_rocket_startup,(ctl));
  startup = 1;

  DT_C(dtipc_rocket_parse,(ctl,			/* parse name into host:port 		*/
    ipc, name, F,
    &host, &port));

  ipc->type = DTIPC_TYPE_SOCKET;

  DT_C(dtipc_rocket_prepare,(ctl,		/* prepare to make a socket			*/
    ipc));

  DT_C(dtos_time,(ctl, &start));		/* time at start 					*/
  while (1)
  {
    DT_MEMSET(&client, '\0',				/* clear client structure			*/
      sizeof(client));
	client.sin_addr.s_addr = host; 		/* specify host we parsed 			*/
    client.sin_port = htons(port);
    client.sin_family = AF_INET;
	
	sockhandle = socket(AF_INET,		/* make the socket 					*/
	  SOCK_STREAM, 0);
	e = LASTERROR;
	if (sockhandle < 0)
	{
	  rc = dt_err(ctl, F,
		"socket(AF_INET, SOCK_STREAM, 0)"
        DTERRNO_FMT,
        DTERRNO_ARG(e));
	  goto cleanup;
	}

#   ifdef USE_SELECT_CONNECT
	{
	  unsigned long mode;
	  mode = 1;
	  sysrc = IOCTL(sockhandle,			/* go into non-blocking mode 		*/
        FIONBIO, &mode);
	  e = LASTERROR;
	  if (sysrc < 0)
	  {
	    rc = dt_err(ctl, F,
		  "ioctl(%ld,"
		  " FIONBIO, %ul)"
          DTERRNO_FMT,
		  sockhandle, mode,
          DTERRNO_ARG(e));
		goto cleanup;
	  }
	}
#   endif

    sysrc = connect(sockhandle,			/* bind local and connect foreign	*/
      (struct sockaddr *)&client,
      sizeof(client));
	e = LASTERROR;

	if (timeout <= 0)					/* timeout forever or just poll? 	*/
	  remaining = timeout;				/* never/always timeout 			*/
	else
	{
	  dtos_time_t now;
	  DT_C(dtos_time,(ctl, &now));		/* time at this moment 				*/
	  remaining = (long)(timeout -		/* time remaining to wait 			*/
        (now.wall-start.wall)*1000.0);
	  if (remaining < 0)
		remaining = 0;
	}

	if (sysrc >= 0)						/* connection worked?				*/
  	  break;
	else
    if (e == EWOULDBLOCK ||				/* connection not ready yet?		*/
		e == EINPROGRESS)
	{
	  int is_ready;
	  ipc->selectfd = sockhandle;
	  DT_C(dtipc_rocket_poll,(ctl,		/* wait until socket writable 		*/
        ipc, DTIPC_POLL_CONNECT, 
        remaining,
        "connection to server",
        &is_ready));
	  if (!is_ready)
	  {
		rc = dt_err(ctl, F,
          DT_LABEL_TIMEOUT 
          " %s did not accept"
          " within %ld ms",
          priv->name, timeout);
		goto cleanup;
	  }
	  break;							/* socket is ready 					*/
	}
	else
    if (e == ECONNREFUSED)				/* special case: server not up yet	*/
	{
	  dt_yield(ctl, 
        "connection refused remaining=%ld", remaining);
	  if (remaining == 0)
	  {
		rc = dt_err(ctl, F,
          DT_LABEL_TIMEOUT 
          " %s did not listen"
          " within %ld ms",
          priv->name, timeout);
		goto cleanup;
	  }
	  DT_C(dtos_delay,(ctl, waitinc));	/* delay a little while 			*/
      CLOSE(sockhandle);
	}
    else								/* some other error? 				*/
    {
	  rc = dt_err(ctl, F,
        "connect(%ld, 0x%08lx, %d)"
		" to %s"
        DTERRNO_FMT,
        sockhandle, &client, sizeof(client),
		priv->name,
        DTERRNO_ARG(e));
	  goto cleanup;
    }
  }
  
# ifdef USE_SELECT_CONNECT
  {
	unsigned long mode;
	mode = 0;
	sysrc = IOCTL(sockhandle,			/* go into blocking mode 			*/
      FIONBIO, &mode);
	e = LASTERROR;
	if (sysrc < 0)
	{
	  rc = dt_err(ctl, F,
		"ioctl(%ld, FIONBIO, %ul)"
        DTERRNO_FMT,
		sockhandle, mode, 
        DTERRNO_ARG(e));
	  goto cleanup;
	}
  }
# endif

  ipc->datahandle = sockhandle;
  
  DT_C(dtipc_rocket_fixup,(ctl, ipc,	/* condition the socket			    */
    sockhandle));

  dt_dbg(ctl, F, DT_DBG_MASK_IPC,
	"client connected with socket %d"
    " to host %s port %d",
     ipc->datahandle,
     inet_ntoa(
       *((struct in_addr *)&host)), 
     port);

  {
	int length = sizeof(client);
	sysrc = getsockname(sockhandle,		/* find out what host/port we got	*/
      (struct sockaddr *)&client,
	  &length);
	e = LASTERROR;
	if (sysrc < 0)
	{
	  rc = dt_err(ctl, F,
        "getsockname(%d, 0x%08lx, %d)"
        DTERRNO_FMT,
        sockhandle, &client, sizeof(client),
        DTERRNO_ARG(e));
	  goto cleanup;
	}
  }

  DT_C(dtstr_printf,(ctl,
    (char *)ipc->client_spec,
    sizeof(ipc->client_spec),
    "rocket,%s:%d",
    inet_ntoa(client.sin_addr),
    ntohs(client.sin_port)));

  dt_dbg(ctl, F, DT_DBG_MASK_IPC,
	"client connected"
    " as %s", ipc->client_spec);

  ipc->type = DTIPC_TYPE_SOCKET;

cleanup:

  if (rc != DT_RC_GOOD)
    CLOSE(sockhandle);

  if (rc != DT_RC_GOOD && startup)
    DT_I(dtipc_rocket_cleanup,(ctl));

  return rc;
}

/*..........................................................................*/
static
dt_rc_e
dtipc_rocket_send2(						/* send data, return amount sent	*/
  dt_ctl_t *ctl,
  dtipc_t *ipc,
  const void *data,
  long length,
  long *sent)
{
  DT_F("dtipc_rocket_send2");
  priv_t *priv = (priv_t *)ipc->priv;
  dtipc_header_t h;
  char *b;
  long want, got;
  dt_rc_e rc;

  if (ipc->type != DTIPC_TYPE_SOCKET)	/* check structure integrity 		*/
    return dt_err(ctl, F, 
      "ipc type %d does not match %d",
      ipc->type, DTIPC_TYPE_SOCKET);

  DT_MEMSET(&h, 0, sizeof(h));
  h.magic = DTIPC_MAGIC;
  h.total = length;

  want = sizeof(h);
  b = (char *)&h;
  do {
	DT_C(dtipc_rocket_write,(ctl, ipc,	/* send the header 					*/
      b, want, &got));
	want -= got;
	b += got;
  } while(want != 0 && got != 0);
  if (got == 0)							/* connection closed? 				*/
  {
	if (sent == NULL)
	  rc = dt_err(ctl, F, 
        "%s other end of socket closed",
        priv->name);
	else
	{
	  dt_dbg(ctl, F, DT_DBG_MASK_IPC,
        "%s receiver closed sockhandle %d",
        priv->name, ipc->datahandle);
	  *sent = 0;
	}
	goto cleanup;
  }

  want = length;
  b = (char *)data;
  do {
    DT_C(dtipc_rocket_write,(ctl, ipc,	/* send the data 					*/
      b,
      DT_MIN(want, 
        DTCONFIG_WRITE_MAX),
      &got));
	want -= got;
	b += got;
  } while(want != 0 && got != 0);
  
  if (got == 0)							/* connection closed? 				*/
  {
	if (sent == NULL)
	  rc = dt_err(ctl, F, 
        "%s other end of socket closed",
        priv->name);
	else
	  dt_dbg(ctl, F, DT_DBG_MASK_IPC,
        "%s receiver closed sockhandle %d",
        priv->name, ipc->datahandle);
	goto cleanup;
  }
  
  if (sent != NULL)
    *sent = b - (char *)data;			/* return bytes actually sent 		*/

cleanup:
  return rc; 
}

/*..........................................................................*/
static
dt_rc_e
dtipc_rocket_send(						/* send data on connection			*/
  dt_ctl_t *ctl,
  dtipc_t *ipc,
  const void *data,
  long length)
{
  DT_F("dtipc_rocket_send");
  DT_Q(dtipc_rocket_send2,(ctl, ipc,
    data, length, NULL));
  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/
static
dt_rc_e
dtipc_rocket_recvt(						/* receive, known length, timeout	*/
  dt_ctl_t *ctl,
  dtipc_t *ipc,
  void **data,
  long length,
  long timeout)
{
  DT_F("dtipc_rocket_recvt");
  priv_t *priv = (priv_t *)ipc->priv;
  dtipc_header_t h;
  char *b;
  long want, got;
  dtos_time_t start;
  long remaining;
  int isavailable;
  dt_rc_e rc;

  *data = NULL;

  if (ipc->type != DTIPC_TYPE_SOCKET)
  {
    rc = dt_err(ctl, F, 
      "ipc type %d does not match %d",
      ipc->type, DTIPC_TYPE_SOCKET);
	goto cleanup;
  }

  if (length > ipc->recv_max)
  {
    rc = dt_err(ctl, F,
      "expected length %ld"
      " > recv_max %ld",
      length, ipc->recv_max);
	goto cleanup;
  }

  DT_C(dtos_time,(ctl, &start));		/* time at start 					*/
  want = sizeof(h);
  b = (char *)&h;
  do {
	if (timeout <= 0)					/* timeout forever or just poll? 	*/
	  remaining = timeout;				/* never/always timeout 			*/
	else
	{
	  dtos_time_t now;
	  DT_C(dtos_time,(ctl, &now));		/* time at this moment 				*/
	  remaining = (long)(timeout -		/* time remaining to wait 			*/
        (now.wall-start.wall)*1000.0);
	  if (remaining < 0)
		remaining = 0;
	}
	DT_C(dtipc_rocket_poll,(ctl,		/* wait for incoming data			*/
      ipc, DTIPC_POLL_READ, remaining,
      "header", &isavailable));
	if (!isavailable)
	{
	  rc = dt_err(ctl, F,
        "timeout %ld ms for header",
        timeout);
	  goto cleanup;
	}
	DT_C(dtipc_rocket_read,(ctl, ipc,	/* read the header 					*/
      b, want, &got));
	want -= got;
	b += got;
  } while(want != 0 && got != 0);
  if (got == 0)
  {
    rc = dt_err(ctl, F, 
      "%s other end of socket closed",
      priv->name);
	goto cleanup;
  }
  
  if (h.magic == DTIPC_MAGIC_OTHER)		/* sender is other endian? 			*/
	ipc->need_fix = 1;
  else
    ipc->need_fix = 0;
  
  h.magic =  DTIPC_FIX(DT_Q, ctl, ipc, h.magic);
  h.total =  DTIPC_FIX(DT_Q, ctl, ipc, h.total);
  h.length = DTIPC_FIX(DT_Q, ctl, ipc, h.length);
  h.pid =    DTIPC_FIX(DT_Q, ctl, ipc, h.pid);

  if (h.magic != DTIPC_MAGIC)
  {
    rc = dt_err(ctl, F,
      "header magic 0x%08lx"
      " does not match 0x%08lx",
      h.magic, DTIPC_MAGIC);
	goto cleanup;
  }
  if ((long)h.total != length)
  {
    rc = dt_err(ctl, F,
      "header total received %lu (0x%08lx)"
      " does not match expected %ld (0x%08lx)",
      h.total, h.total, length, length);
	goto cleanup;
  }

  DT_C(dtos_malloc2,(ctl, data, 
    length, F, "received data"));


  want = length;
  b = (char *)*data;
  do {
	if (timeout <= 0)					/* timeout forever or just poll? 	*/
	  remaining = timeout;				/* never/always timeout 			*/
	else
	{
	  dtos_time_t now;
	  DT_C(dtos_time,(ctl, &now));		/* time at this moment 				*/
	  remaining = (long)(timeout -		/* time remaining to wait 			*/
        (now.wall-start.wall)*1000.0);
	  if (remaining < 0)
	    remaining = 0;
	}
	DT_C(dtipc_rocket_poll,(ctl,		/* wait for incoming data			*/
      ipc, DTIPC_POLL_READ, remaining,
      "data", &isavailable));
	if (!isavailable)
	{
	  rc = dt_err(ctl, F,
        "timeout %ld ms for data",
        timeout);
	  goto cleanup;
	}
    DT_C(dtipc_rocket_read,(ctl, ipc,	/* read the data 					*/
      b, want, &got));
	want -= got;
	b += got;
  } while(want != 0 && got != 0);
  
  if (got == 0)
  {
    rc = dt_err(ctl, F, 
      "%s other end of socket closed",
      priv->name);
	goto cleanup;
  }

cleanup:
  if (rc != DT_RC_GOOD &&
	  *data != NULL)
  {
    DT_I(dtos_free2,(ctl, *data,
      F, "received data"));
	*data = NULL;
  }
  return rc;
}

/*..........................................................................
 *..........................................................................*/
static
dt_rc_e
dtipc_rocket_recv(						/* receive data, known length		*/
  dt_ctl_t *ctl,
  dtipc_t *ipc,
  void **data,
  long length)
{
  DT_F("dtipc_rocket_recv");
  DT_Q(dtipc_rocket_recvt,(ctl, ipc, data, length, -1));
  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/
static
dt_rc_e
dtipc_rocket_recv2(						/* receive data, unknown length		*/
  dt_ctl_t *ctl,
  dtipc_t *ipc,
  void **data,
  long *length)
{
  DT_F("dtipc_rocket_recv");
  priv_t *priv = (priv_t *)ipc->priv;
  dtipc_header_t h;
  char *b;
  long want, got;
  dt_rc_e rc;

  *data = NULL;

  if (ipc->type != DTIPC_TYPE_SOCKET)
  {
    rc = dt_err(ctl, F, 
      "ipc type %d does not match %d",
      ipc->type, DTIPC_TYPE_SOCKET);
	goto cleanup;
  }

# ifdef POLL_BEFORE_RW
  DT_C(dtipc_rocket_poll,(ctl,			/* wait for incoming data			*/
    ipc, DTIPC_POLL_READ, -1,
    "incoming header",
    NULL));
# endif

  want = sizeof(h);
  b = (char *)&h;
  do {
	DT_C(dtipc_rocket_read,(ctl, ipc,	/* read the header 					*/
      b, want, &got));
	want -= got;
	b += got;
  } while(want != 0 && got != 0);
  if (got == 0)
  {
    rc = dt_err(ctl, F, 
      "%s other end of socket closed",
      priv->name);
	*length = 0;
	goto cleanup;
  }
  
  if (h.magic == DTIPC_MAGIC_OTHER)		/* sender is other endian? 			*/
	ipc->need_fix = 1;
  else
    ipc->need_fix = 0;
  
  h.magic =  DTIPC_FIX(DT_Q, ctl, ipc, h.magic);
  h.total =  DTIPC_FIX(DT_Q, ctl, ipc, h.total);
  h.length = DTIPC_FIX(DT_Q, ctl, ipc, h.length);
  h.pid =    DTIPC_FIX(DT_Q, ctl, ipc, h.pid);

  if (h.magic != DTIPC_MAGIC)
  {
    rc = dt_err(ctl, F,
      "header magic 0x%08lx"
      " does not match 0x%08lx",
      h.magic, DTIPC_MAGIC);
	goto cleanup;
  }

  DT_C(dtos_malloc2,(ctl, data, 
    h.total, F, "received data"));

# ifdef POLL_BEFORE_RW
  DT_C(dtipc_rocket_poll,(ctl,			/* wait for incoming data			*/
    ipc, DTIPC_POLL_READ, -1,
    "incoming data",
    NULL));
# endif

  want = h.total;
  b = (char *)*data;
  do {
    DT_C(dtipc_rocket_read,(ctl, ipc,	/* read the data 					*/
      b, want, &got));
	want -= got;
	b += got;
  } while(want != 0 && got != 0);
  
  if (got == 0)
  {
    rc = dt_err(ctl, F, 
      "%s other end of socket closed",
      priv->name);
	goto cleanup;
  }
  
  *length = h.total;

cleanup:
  if (rc != DT_RC_GOOD &&
	  *data != NULL)
  {
    DT_I(dtos_free2,(ctl, *data,
      F, "received data"));
	*data = NULL;
	*length = 0;
  }
  return rc;
}

#endif

/*..........................................................................*/
dt_rc_e
DTCONFIG_API1
dtipc_rocket_init(						/* init structure					*/
  dt_ctl_t *ctl,
  dtipc_t *ipc,
  long recv_max,
  unsigned long flags)
{
#if DTPRJ_SOCKET == DT_1
  DT_F("dtipc_rocket_init");
  priv_t *priv = NULL;
  dt_rc_e rc;

  DT_MEMSET(ipc, 0, sizeof(*ipc));

  if (recv_max == 0)
    ipc->recv_max = DTIPC_RECV_MAX_DEFAULT;
  else
    ipc->recv_max = recv_max;

  DT_C(dtos_malloc2,(ctl,
    (void **)&priv, sizeof(*priv),
    F, "priv"));
  DT_MEMSET(priv, 0, sizeof(*priv));

  ipc->datahandle = -1;

  ipc->priv = (void *)priv;

  ipc->flags = flags;

  ipc->rregister = dtipc_rocket_register;
  ipc->unregister = dtipc_rocket_unregister;
  ipc->wait = dtipc_rocket_wait;
  ipc->poll = dtipc_rocket_poll;
  ipc->isipc = dtipc_rocket_isipc;
  ipc->find = dtipc_rocket_find;
  ipc->unfind = dtipc_rocket_unfind;
  ipc->done = dtipc_rocket_done;
  ipc->send = dtipc_rocket_send;
  ipc->send2 = dtipc_rocket_send2;
  ipc->write = dtipc_rocket_write;
  ipc->recv = dtipc_rocket_recv;
  ipc->recvt = dtipc_rocket_recvt;
  ipc->recv2 = dtipc_rocket_recv2;
  ipc->read = dtipc_rocket_read;
  ipc->uninit = dtipc_rocket_uninit;
  ipc->split = dtipc_rocket_split;

cleanup:
  if (rc != DT_RC_GOOD)
    DT_I(dtipc_rocket_release,(ctl,
      priv));
  return rc;
#else
  DT_F("dtipc_rocket_init");
  return dt_err_notlinked(ctl, F,
    "socket ipc");
#endif
}

/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtipc_rocket_rev(						/* deliver revision 				*/
  dt_ctl_t *ctl,						/* environment control 				*/
  const char **rev_string,				/* returned rev string 				*/
  int *rev_major,						/* returned rev major 				*/
  int *rev_minor,						/* returned rev minor 				*/
  long *rev_date)						/* returned rev date 				*/
{
  if (rev_string != NULL)
    *rev_string = "dtipc_rocket lib";
  if (rev_major != NULL)
    *rev_major = 4;
  if (rev_minor != NULL)
    *rev_minor = 11;
  if (rev_date != NULL)
    *rev_date = 20001011;
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
