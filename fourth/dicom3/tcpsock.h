/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
  ! Copyright (C) 1998 Dtack Inc. All Rights Reserved
  ! To use this file, you must have signed a license agreement with Dtack Inc.
  ! Under no circumstances may you redistribute this file.
  ! This software is provided AS IS with no warranty expressed or implied.
  ! Dtack Inc. accepts no liability for use or misuse of this file.
  ! http://www.dtack.com  dtack@dtack.com  telephone +360.417.6700
  ! Dtack Inc, P.O. Box 3061, Port Angeles, WA USA 98362
  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */




/* tcpsock.h 0.12 95/09/18 Copyright (C) 1993,1994,1995. David A. Clunie. All rights reserved. */
#ifndef _tcpsock_HEADER_
#define _tcpsock_HEADER_

#include <stdlib.h>	// for atoi()
#include <string.h>	// for strcmp()
#include <fstream.h>

#include <unistd.h>
//#include <memory.h>	// for memcpy() etc. - don't use bcopy() etc.
			// because no prototypes declared on SunOS
#include <sys/types.h>

#    ifdef DTCONFIG_QW1
       struct iovec { 
         caddr_t iov_base; 
         long iov_len; 
       };
#    endif

#include <sys/socket.h>	// NB. g++ doesn't have prototypes ... see
			// /usr/lang/SC2.01/include/CC_412/sys/socket.h

#ifdef NEEDMISSINGPROTOTYPES
extern "C" {
int accept(int, struct sockaddr *, int *);
int bind(int, struct sockaddr *, int);
int connect(int, const struct sockaddr *, int);
int getpeername(int, struct sockaddr *, int *);
int getsockname(int, struct sockaddr *, int *);
int getsockopt(int, int, int, char *, int *);
int listen(int, int);
int recv(int, char *, int, int);
int recvfrom(int, char *, int, int, struct sockaddr *, int *);
int send(int, const char *, int, int);
int sendto(int, const char *, int, int, const struct sockaddr *, int);
int setsockopt(int, int, int, const char *, int);
int socket(int, int, int);
int recvmsg(int, struct msghdr *, int);
int sendmsg(int, struct msghdr *, int);
int socketpair(int, int, int, int *);
}
#endif /* NEEDMISSINGPROTOTYPES */

#include <netinet/in.h>	// for htons()
#include <arpa/inet.h>	// for inet_addr()
			// NB. g++ doesn't have prototypes ... see
			// /usr/lang/SC2.01/include/CC_412/sys/arpa/inet.h

#ifdef NEEDMISSINGPROTOTYPES
extern "C" {
unsigned long    inet_addr(const char*);
char*            inet_ntoa(struct in_addr);
struct in_addr   inet_makeadd(int, int);
unsigned long    inet_network(const char*);
int              inet_lnaof(struct in_addr);
int              inet_netof (struct in_addr);
}
#endif /* NEEDMISSINGPROTOTYPES */

#include <netdb.h>	// NB. g++ doesn't have prototypes ... see
			// /usr/lang/SC2.01/include/CC_412/sys/netdb.h

#ifdef NEEDMISSINGPROTOTYPES
extern "C" {
struct hostent *gethostbyname(const char *);
struct hostent *gethostbyaddr(const char *, int, int);
struct hostent *gethostent(void);

struct netent *getnetbyname(char *);
struct netent *getnetbyaddr(long, int);
struct netent *getnetent(void);

struct servent *getservbyname(char *, char *);
struct servent *getservbyport(int, char *);
struct servent *getservent(void);

struct protoent *getprotobyname(char *);
struct protoent *getprotobynumber(int);
struct protoent *getprotoent(void);

struct rpcent *getrpcbyname(char *);
struct rpcent *getrpcbynumber(int);
struct rpcent *getrpcent(void);
}
#endif /* NEEDMISSINGPROTOTYPES */

typedef	unsigned short	u_short;

#ifndef	INADDR_NONE
#define	INADDR_NONE	0xffffffff
#endif /* INADDR_NONE */

extern int	errno;
extern char	*sys_errlist[];

class BaseSocket {
protected:
	char *service;
	char *protocol;
	struct sockaddr_in SocketInternetAddress;
	struct servent *Service;
	struct protoent *Protocol;
	int SocketType;
	int Socket;

	void makeSocketInternetAddress(void)
		{
			memset((char *)&SocketInternetAddress,0,
				sizeof(SocketInternetAddress));
			SocketInternetAddress.sin_family = AF_INET;
		}

	void makeService(void)
		{
			Service=::getservbyname(service, protocol);
			if (Service) {
				SocketInternetAddress.sin_port=
					htons(ntohs((u_short)Service->s_port));
			}
			else if ((SocketInternetAddress.sin_port=
				htons((u_short)atoi(service))) == 0) {
					cerr << "can't get " << service
					     << "service entry\n" << flush;
				exit(1);
			}
		}

	void makeProtocol(void)
		{
			Protocol=::getprotobyname(protocol);
			if (Protocol == 0) {
				cerr << "can't get " << protocol 
				     << "protocol entry\n" << flush;
				exit(1);
			}

		}

	void makeSocket(int& type,int& s)
		{
			if (strcmp(protocol,"udp") == 0)
				type = SOCK_DGRAM;
			else
				type = SOCK_STREAM;


			s=::socket(PF_INET,type,Protocol->p_proto);
			if (s < 0) {
				cerr << "can't create socket: " 
				     << sys_errlist[errno] 
				     << "\n" << flush;
				exit(1);
			}
		}
public:
	BaseSocket(void)	{}

	int writestr(char *msg,unsigned l)
		{
			int nw;
			cerr << "write: [" << l << "] <" 
			     << msg << ">\n" << flush;
			nw=::write(Socket,msg,l);
			if (nw <= 0) {
				cerr << "write failed "
				     << " " << sys_errlist[errno] 
				     << "\n" << flush;
			}
			return nw;
		}

	int writepad(char *msg,unsigned l,unsigned pl,char pad)
		{
			int nw;
			cerr << "write: [" << l << "] <" 
			     << msg << "> padded with " 
			     << pl-l << " <" << pad 
			     << ">\n" << flush;
			nw=::write(Socket,msg,l);
			while (pl-- > l) nw+=::write(Socket,&pad,1);
			if (nw <= 0) {
				cerr << "write failed "
				     << " " << sys_errlist[errno] 
				     << "\n" << flush;
			}
			return nw;
		}

	int writezero(unsigned l)
		{
			int nw;
			cerr << "write: [" << l << "] zeroes\n" << flush;
			nw=0;
			char zero = 0;
			while (l--) nw+=::write(Socket,&zero,1);
			if (nw <= 0) {
				cerr << "write failed "
				     << " " << sys_errlist[errno] 
				     << "\n" << flush;
			}
			return nw;
		}

	int write(char *msg)
		{
			return writestr(msg,strlen(msg));
		}

	int write8(char c)
		{
			int nw;
			cerr << "write: [1] <" << (unsigned)c << ">\n" << flush;
			nw=::write(Socket,&c,1);
			if (nw <= 0) {
				cerr << "write failed "
				     << " " << sys_errlist[errno] 
				     << "\n" << flush;
			}
			return nw;
		}

	int write16(unsigned short u)	// Big endian network byte order
		{
			char c1=(char)((u>>8)&0xff);
			char c2=(char)(u&0xff);
			int nw;
			cerr << "write: [2] <" << u << ">\n" << flush;
			nw= ::write(Socket,&c1,1);
			nw+=::write(Socket,&c2,1);
			if (nw <= 0) {
				cerr << "write failed "
				     << " " << sys_errlist[errno] 
				     << "\n" << flush;
			}
			return nw;
		}

	int write32(unsigned long u)	// Big endian network byte order
		{
			char c1=(char)((u>>24)&0xff);
			char c2=(char)((u>>16)&0xff);
			char c3=(char)((u>>8)&0xff);
			char c4=(char)(u&0xff);
			int nw;
			cerr << "write: [4] <" << u << ">\n" << flush;
			nw= ::write(Socket,&c1,1);
			nw+=::write(Socket,&c2,1);
			nw+=::write(Socket,&c3,1);
			nw+=::write(Socket,&c4,1);
			if (nw <= 0) {
				cerr << "write failed "
				     << " " << sys_errlist[errno] 
				     << "\n" << flush;
			}
			return nw;
		}

	int read(char *buf,int buflng)
		{
			int nr;
			nr=::read(Socket,buf,buflng);
			if (nr <= 0) {
				cerr << "read failed "
				     << " " << sys_errlist[errno] 
				     << "\n" << flush;
			}
			buf[nr]=0;		// null terminate string
			cerr << "read: [" << nr << "] <" 
			     << buf << ">\n" << flush;
			return nr;
		}

	int readstr(char **buf,int buflng)
		{
			*buf=new char [buflng+1];
			int nr;
			nr=::read(Socket,*buf,buflng);
			if (nr <= 0) {
				cerr << "read failed "
				     << " " << sys_errlist[errno] 
				     << "\n" << flush;
			}
			(*buf)[nr]=0;		// null terminate string
			cerr << "read: [" << nr << "] <" 
			     << *buf << ">\n" << flush;
			return nr;
		}

	int readskip(unsigned l)
		{
			char c;
			int nr=0;
			while (l--) nr+=::read(Socket,&c,1);
			if (nr <= 0) {
				cerr << "read failed "
				     << " " << sys_errlist[errno] 
				     << "\n" << flush;
			}
			cerr << "read: [" << nr << "] skipped\n" << flush;
			return nr;
		}

	int read8(unsigned char *c)
		{
			int nr;
			nr=::read(Socket,(char *)c,1);
			if (nr <= 0) {
				cerr << "read failed "
				     << " " << sys_errlist[errno] 
				     << "\n" << flush;
			}
			cerr << "read: [" << nr << "] <" 
			     << (unsigned)*c << ">\n" << flush;
			return nr;
		}

	int read16(unsigned short *u)	// Big endian network byte order
		{
			unsigned char c[2];
			int nr;
			nr=::read(Socket,(char *)c,2);
			if (nr <= 0) {
				cerr << "read failed "
				     << " " << sys_errlist[errno] 
				     << "\n" << flush;
			}
			*u=(c[0]<<8)+c[1];
			cerr << "read: [" << nr << "] <" 
			     << *u << ">\n" << flush;
			return nr;
		}

	int read32(unsigned long *u)	// Big endian network byte order
		{
			unsigned char c[4];
			int nr;
			nr=::read(Socket,(char *)c,4);
			if (nr <= 0) {
				cerr << "read failed "
				     << " " << sys_errlist[errno] 
				     << "\n" << flush;
			}
			*u=(c[0]<<24)+(c[1]<<16)+(c[2]<<8)+c[3];
			cerr << "read: [" << nr << "] <" 
			     << *u << ">\n" << flush;
			return nr;
		}
};

class ServerSocket : public BaseSocket {
	int QueueLength;
	struct sockaddr_in FromInternetAddress;
	int ListenSocketType;
	int ListenSocket;
public:
	ServerSocket(char *s,char *p)
		: BaseSocket()
		{
			service=s;
			protocol=p;
			QueueLength = 5;

			makeSocketInternetAddress();
			SocketInternetAddress.sin_addr.s_addr = INADDR_ANY;
			makeService();
			makeProtocol();
			makeSocket(ListenSocketType,ListenSocket);
		}

	void bind(void)
		{
			if (::bind(ListenSocket,
				(struct sockaddr *)&SocketInternetAddress,
				sizeof(SocketInternetAddress)) < 0) {
					cerr << "can't bind to port " << service
					     << " " << sys_errlist[errno] 
					     << "\n" << flush;
				exit(1);
			}
		}

	void listen(void)
		{
			if (ListenSocketType == SOCK_STREAM 
			 && ::listen(ListenSocket,QueueLength) < 0) {
				cerr << "can't listen on port " << service
				     << " " << sys_errlist[errno] 
				     << "\n" << flush;
				exit(1);
			}
		}

	void accept(void)
		{
			int FromLength=sizeof(sockaddr_in);
			Socket=::accept(ListenSocket,
				(struct sockaddr *)&FromInternetAddress,
				&FromLength);

			if (Socket < 0) {
				cerr << "accept failed " << sys_errlist[errno] 
				     << "\n" << flush;
				exit(1);
			}
		}
};

class ClientSocket : public BaseSocket {
	char *host;
	struct hostent *Host;

	void makeHost(void)
		{
			Host = gethostbyname(host);
			if (Host) {
				memcpy((char *)&SocketInternetAddress.sin_addr,
				      Host->h_addr,
				      Host->h_length);
			}
			else if ((SocketInternetAddress.sin_addr.s_addr=
					inet_addr(host)) 
						== INADDR_NONE) {
				cerr << "can't get " << host 
				     << "host entry\n" << flush;
				exit(1);
			}
		}
public:
	ClientSocket(char *s,char *p,char *h)
		: BaseSocket()
		{
			service=s;
			protocol=p;
			host=h;	

			makeSocketInternetAddress();
			makeService();
			makeHost();
			makeProtocol();
			makeSocket(SocketType,Socket);
		}

	void connect(void)
		{
			if (::connect(Socket,
				(struct sockaddr *)&SocketInternetAddress,
				sizeof(SocketInternetAddress)) < 0) {
				cerr << "can't connect to " << host 
				     << "." << service
				     << " " << sys_errlist[errno] 
				     << "\n" << flush;
				exit(1);
			}
		}
};

#endif /* _tcpsock_HEADER_ */






/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
  ! Copyright (C) 1998 Dtack Inc. All Rights Reserved
  ! To use this file, you must have signed a license agreement with Dtack Inc.
  ! Under no circumstances may you redistribute this file.
  ! This software is provided AS IS with no warranty expressed or implied.
  ! Dtack Inc. accepts no liability for use or misuse of this file.
  ! http://www.dtack.com  dtack@dtack.com  telephone +360.417.6700
  ! Dtack Inc, P.O. Box 3061, Port Angeles, WA USA 98362
  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */
