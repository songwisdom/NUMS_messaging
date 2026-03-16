/*-------------------------------------------------------------------------------------------
        PROJECT NAME    : IP Recording Service
        TRANSACTION 명  : Network Library Utility
        PROGRAM ID      : 
        LIBRARY         : ./neots.a
        LANGUAGE        : Standard Ansi C
        DEVP CONFIG     : HP-UX 11.xx
        PROCESS DEFINE  : 
        #VERSION        #WORK/MODIFIED DATE             #COMMENT                #PROGRAMMER
			2006.03.31						KPS
---------------------------------------------------------------------------------------------*/

#include	<string.h>
#include	<stdio.h>
#include	<signal.h>
#include	<errno.h>

#include	<sys/socket.h>
#include	<sys/fcntl.h>
#include	<sys/time.h>
#include	<sys/types.h>

#include	<netinet/in.h>
#include	<arpa/inet.h>


#include "FLKSimBaseDef.h"
#include "Debug.h"
static client_conn_info client_info;
static struct sockaddr_in udp_server_addr, udp_client_addr;

extern int errno;

/*******************************************************************************************************
 FUNCTION NAME	: tcp_connect(char * addr, int service)
 FUNCTION ROLE	: TCP 연결.
 ARGUMENT	: 
 		  addr - 접속 IP
 		  service - 
 RETURN VALUE	: 성공시 sockfd
 		  오류시 -1
********************************************************************************************************/
int tcp_connect(char * addr, int service, int tm)
{
	int	sockfd;
	int	flags, error, len, n;

	fd_set	rset, wset;
	
	struct	timeval	tv;	
	struct	sockaddr_in sin;


	memset((char *)&sin, 0, sizeof(sin));
	sin.sin_family = AF_INET;
	sin.sin_port = htons(service);
	sin.sin_addr.s_addr = inet_addr(addr);
	
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if ( sockfd < 0 ) {
		return -1;	
	}
	
	if ( (flags = fcntl(sockfd, F_GETFL, 0)) == -1 ) {
		return flags;	
	}
	
	if ( fcntl(sockfd, F_SETFL, flags | O_NONBLOCK) == -1 ) {
		return -1;
	}
	
	error = 0;
	errno = 0;
	
	if ( ( n = connect(sockfd, ( struct sockaddr *)&sin, sizeof(sin)) ) < 0 ) {
		error = errno;


		if ( ( errno != EINPROGRESS ) && ( errno != 0 ) ){
			return -1;
		}
	}
		
	if ( n == 0 ) 
		goto result;
		
	FD_ZERO(&rset);
	FD_SET(sockfd, &rset);
	
	wset = rset;
	
	tv.tv_sec = tm;
	tv.tv_usec = 0;
	
	if ( (n = select(sockfd + 1, &rset, &wset, NULL, &tv)) < 0 ) {
		return n;
	}
	
	if ( n == 0 ) {
		close(sockfd);
		errno = ETIMEDOUT;
		return -1;
	}
	
	if ( FD_ISSET(sockfd, &rset) || FD_ISSET(sockfd, &wset) ) {
		len = sizeof(error);
		n = getsockopt(sockfd, SOL_SOCKET, SO_ERROR, &error, (socklen_t *)&len);
		if ( n < 0 ) {
			return -1;
		}
	} else {
		return -1;
	}	
result :
	if ( fcntl(sockfd, F_SETFL, flags) == -1 ) {
		return -1;
	}

	if ( error ) {
		close(sockfd);
		errno = error;
		
		if ( errno == ECONNREFUSED ) return -9;
		
		return -1;
	}		
	

	return ( sockfd );
}

char * long_to_net_addr(unsigned long int hn, char ** addr)
{
	struct	in_addr	l_addr;

	l_addr.s_addr = hn;
	
	*addr = inet_ntoa(l_addr);

	return *addr;
}

unsigned long int net_addr_to_long(char * addr)
{
	struct	in_addr	l_addr;
	
	if ( inet_aton(addr, &l_addr) == 0 ) {
		return 0;
	}
	
	return l_addr.s_addr;
}


/* 
 * 
 * 소켓 지시자를 돌려준다.
 * 
 * Argusockadrr : 클라이언트와 서버 소켓지시자에 대한 배열
 * count : 배열 갯수
 * 
 */
 
 
/*******************************************************************************************************
	FUNCTION NAME	: get_active_socket
	FUNCTION ROLE	: 클라이언트와 서버 소켓 지시자 중에서 읽을 데이타가 있는
 	    		  소켓 지시자를 돌려준다.	

 	ARGUMENT	: 
 			  sockarr - 체크해야할 소켓 지시자에 대한 배열
        		  count - 배열 갯수
        		  tm - 대기시간

 	RETURN VALUE	: 성공시 Active된 소켓 지시자
        		  오류시 -1
********************************************************************************************************/
int get_active_socket(sockfd_state_st * sockarr, int count, int tm)
{
	int	i, check = 0;
	int	state = 0;
	int	active = 0;
	int	max = 0;
	
	fd_set	readfds;
	struct 	timeval tv;
	

	if ( count == 0 ) 
		return -1;
	
	tv.tv_sec = tm ? tm : 0;
	tv.tv_usec = 0;
	

	FD_ZERO(&readfds);
		
	for ( i = 0; i < count; i++ ) {

		if ( sockarr[i].sock_fd != -1 ) {
			check++;
			FD_SET(sockarr[i].sock_fd, &readfds);		

			max =  ( max < sockarr[i].sock_fd ) ? sockarr[i].sock_fd : max;

		}	
	}	
	

	state = select(max + 1, &readfds, (fd_set *)0, (fd_set *)0, &tv);
	if ( state > 0 ) {
		for ( i = 0; i < count; i++ ) {
			if ( sockarr[i].sock_fd == -1 ) continue;
			
			if ( FD_ISSET( sockarr[i].sock_fd, &readfds ) ) {
				active++;
				sockarr[i].sock_state = SOCK_ACTIVE;	
			} else {
				sockarr[i].sock_state = SOCK_INACTIVE;
			}
		}
		

		return active;
	}
	
	return state;		
}

int p_select(int sockfd, int sec, int usec)
{
	int	state;

	fd_set	readfd;
	struct timeval tv;

	if ( sec <= 0 ) {
		return 0;
	}

	tv.tv_sec = sec;
	tv.tv_usec = usec;
	
	FD_ZERO(&readfd);
	FD_SET(sockfd, &readfd);

	state = select(sockfd+1, &readfd, (fd_set *)0, (fd_set *)0, &tv);
	if ( state > 0 ) {
		if FD_ISSET(sockfd, &readfd) 
			return sockfd;
		else 
			return 0;
	}

	if ( state < 0 ) {
		return -1;
	}

	if ( state == 0 ) {
		return 0;
	}

	return state;
}

int select_socket_status(int sockfd)
{
	int	state;

	fd_set	readfds, writefds, errorfds;

	FD_ZERO(&readfds);
	FD_ZERO(&writefds);
	FD_ZERO(&errorfds);

	FD_SET(sockfd, &readfds);
	FD_SET(sockfd, &writefds);
	FD_SET(sockfd, &errorfds);

	state = select(sockfd + 1, &readfds, &writefds, &errorfds, 0);
	if ( state > 0 ) {
		if ( (FD_ISSET(sockfd, &readfds)) || (FD_ISSET(sockfd, &writefds)) )
			return SOCK_ACTIVE;
	}

	return SOCK_INACTIVE;
}

int net_recv_tm(int sockfd, char * rbuff, int sz, int tm)
{
	register int nbytes;
	int	r = 0;

	time_t 	stime, rtime;

	stime = time(NULL);
	rtime = stime + tm;
	

	while( TRUE ) {
		r = p_select(sockfd, (int)(rtime - stime), 0);

		if ( r <= 0 ) {
			if ( r == 0 ) {
			} else {
			}
			return r;
		}
        	
        	stime = time(NULL);
        	
		if ( (nbytes = (int)read(sockfd, rbuff, sz)) <= 0 ) {
			if ( ( nbytes == 0 ) || ( errno == EINTR ) ) {
				errno = 0;
				usleep(500);
				continue;
			}		
        	
			return -3;
		};

		*(rbuff + nbytes) = 0x0;
		return (nbytes);
	}	
	
	//return -1;
}

int net_recv(int sockfd, char * buff, int sz)
{
	return (net_recv_tm(sockfd, buff, sz, NET_RECV_DEFAULT_TM));
}

int net_recv_size(int sockfd, int rbytes, char * buff, int maxlen, int tm)
{
	register int	nbytes = 0;
	unsigned int 	total_bytes = 0 ;
	
	
	if ( rbytes == 0 ) {
		return (net_recv_tm(sockfd, buff, maxlen, tm));	
	}
	
	nbytes = net_recv_tm(sockfd, buff, rbytes, tm);
	/* 수신오류 */
	if ( nbytes <= 0 ) return nbytes;
		
	
	if ( nbytes != rbytes ) {
		return -1;
	}
	

	memcpy(&total_bytes, buff, sizeof(int));
#ifndef _HP_64BYT_SYS_TYPE_
	total_bytes = ntohl(total_bytes);
#endif

	if ( total_bytes <= 0 )	return -3;

	
	//buff += rbytes;
	
	//rbytes = 0;
	
	while(1) {
		nbytes = net_recv_tm(sockfd, buff + rbytes, total_bytes - rbytes, tm);
		if ( nbytes <= 0 ) {
			return nbytes;
		}	
		
		rbytes += nbytes;
		
		if ( rbytes >= total_bytes ) {
			return rbytes;	
		}
	}
	
	//return (int)(total_bytes + n);
}


int net_send(int sockfd, char * buff, int len)
{
	register int	nbytes;

	nbytes = (int)write(sockfd, buff, len);
	if ( nbytes != len ) {
		return -1;
	} else {
	}

	return nbytes;
}

void net_close(int fd)
{
	close(fd);
	return;
}

int net_open_cli(int fd, client_conn_info ** c_info)
{
	register int nsockfd;
	int	len;

	struct	sockaddr_in cli_addr;

again :
	len = sizeof(cli_addr);
	nsockfd = accept(fd, (struct sockaddr *)&cli_addr, (socklen_t *)&len);

	if ( nsockfd < 0 ) {
		if ( errno == EINTR ) {
			errno = 0;
			goto again;
		}
		return -1;
	}

	if ( c_info == NULL )  return nsockfd;

	memset((char *)&client_info, 0x0, sizeof(client_info));

	strcpy(client_info.address, inet_ntoa(cli_addr.sin_addr));
	format_datetime(client_info.cn_time, "YY-MM-DD HH:NN:SS"); 
	*c_info = &client_info;

	return nsockfd;
}

int tcp_server_init(char * addr, int service)
{
	int	on = 1;
	int	fd;

	struct	sockaddr_in	tcp_addr;

	fd = socket(AF_INET, SOCK_STREAM, 0);
	if ( fd == -1 ) {
		return -1;
	}

	memset((char *)&tcp_addr, 0x0, sizeof(tcp_addr));
	tcp_addr.sin_family = AF_INET;
	tcp_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	tcp_addr.sin_port = htons(service);

	(void)setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, (char *)&on, sizeof(on));
	if ( bind(fd, (struct sockaddr *)&tcp_addr, sizeof(tcp_addr) ) == -1 ) {
		close(fd);
		return -1;
	}

	listen(fd, 5);
	return fd;
}

int udp_server_init(char * addr, int service)
{
	int	fd;

	fd = socket(AF_INET, SOCK_DGRAM, 0);
	if ( fd < 0 ) {
		return -1;
	}

	memset((char *)&udp_server_addr, 0x0, sizeof(udp_server_addr));
	udp_server_addr.sin_family = AF_INET;
	udp_server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	udp_server_addr.sin_port = htons(service);

#ifdef LINUX
	if ( bind(fd, (struct sockaddr *)&udp_server_addr, sizeof(udp_server_addr)) == -1 ) {
#else
	if ( bind(fd, (struct sockaddr *)&udp_server_addr, sizeof(udp_server_addr)) == -1 ) {
#endif
		close(fd);
		return -1;
	}

	return fd;
}

int udp_client_init(char * addr, int service)
{
	int	sockfd;
	int	clilen;

	clilen = sizeof(udp_client_addr);

	sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	if ( sockfd < 0 ) {
		return -1;
	}

	memset((char *)&udp_client_addr, 0x0, sizeof(udp_client_addr));
	udp_client_addr.sin_family = AF_INET;
	udp_client_addr.sin_addr.s_addr = inet_addr(addr);
	udp_client_addr.sin_port = htons(service);

	return sockfd;
}

int udp_send(char * addr, int service, char * send_data, int sz)
{
	int	sockfd;
	int	clilen = sizeof(udp_client_addr);
	int	nbyte;

	sockfd = udp_client_init(addr, service);
	if (sockfd < 0) return -1; 
		
	nbyte = sendto(sockfd, send_data, sz, 0, (struct sockaddr *)&udp_client_addr, clilen);
	if ( nbyte == -1 ) {
		return -1;
	}

	close(sockfd);

	return ( nbyte );
}

int udp_recv(int sockfd, char * buff, int maxlen)
{
	int	state;
	int	ln = sizeof(udp_client_addr);


	if ( sockfd < 0 ) return -1;
	
	while(TRUE) {
	/*
		state = recvfrom(sockfd, buff, maxlen, 0, (struct sockaddr *)NULL, (int *)NULL);
	*/
		state = recvfrom(sockfd, buff, maxlen, 0, (struct sockaddr *)&udp_client_addr, (socklen_t *)&ln);
		if ( state > 0 ) {
			return (int)strlen(buff);
		}

		if ( state == -1 ) {
			if ( errno == EINTR ) {
				errno = 0;
				continue;
			}
		}
		
		return -1;
	}
}

/*******************************************************************************************************
	FUNCTION NAME	: TCP_PVC
	FUNCTION ROLE	: 연결지향성의 TCP PROCESS
 	    		  데이타 송수신후 소켓 연결을 종료하지 않는다.	

 	ARGUMENT	: 
 			  sockfd - 데이타를 전송할 소켓 지시자
        		  sbuff, rbuff - 송수신 버퍼
        		  len - sizeof(sbuff)
        		  maxlen - sizeof(rbuff)
        		  tm - 송수신 타임아웃

 	RETURN VALUE	: 성공시 return
        		  오류시 -1
********************************************************************************************************/

int TCP_PVC(int sockfd, char * sbuff, char * rbuff, int len, int maxlen, int tm)
{
	int	nbytes;
	
	
	nbytes = net_send(sockfd, sbuff, len);
	if ( nbytes != len ) {
		return -1;
	}
	
	nbytes = 0;
	nbytes = net_recv_tm(sockfd, rbuff, maxlen, tm);
	if ( nbytes <= 0 ) {
		return -2;
	}
	
	
	return -1;
}

