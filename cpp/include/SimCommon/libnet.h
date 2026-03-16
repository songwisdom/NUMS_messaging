/**
 * =====================================================================================
 *
 *       @file  libnet.h
 *
 *    	 @brief 
 *
 *       Version:  1.0
 *       Compiler:  cc (HP-UX)
 *       @date:  2010년 05월 25일 17시 10분 42초
 *
 *       @author:  SMS Dev (), sms_dev@feelingk.com
 *       Company:  (c) FEELingK
 *       @attention
 *       Copyright (c) 2003 FEELINGK Co., Ltd.                                
 *
 *         All rights reserved.                                                
 *                                                                          
 *     This program contains confidential and proprietary information of    
 *     FEELINGK, and any reproduction, disclosure, or use in whole or in part 
 *     by prior written agreement or permission of FEELINGK.                 
 *                                                                          
 *                            FEELINGK Co., Ltd.                           
 *     3F Ace Techno Twin Tower 1, 212-1 Kuro-Dong, Kuro-Gu, Seoul 152-050 Korea 
 *
 * =====================================================================================
 */
#ifndef _COMLIB_SOCK_LIB_
#define _COMLIB_SOCK_LIB_
#include "FLKSimBaseDef.h"

#define	SOCK_CLIENT	0
#define	SOCK_SERVER	1
#define SOCKET_TYPE_TCP     1
#define SOCKET_TYPE_UDP     2
#define DEF_SOCK_WAIT_TIME  5
#define MAX_CONN_CNT_SOCK   5

#define	SOCK_INACTIVE	0
#define	SOCK_ACTIVE	1

/* Receive시에 타임아웃이 지정되지 않았을 때 디폴트 타임아웃 */
#define	NET_RECV_DEFAULT_TM	10

typedef struct  {
	char	address	[16];
	char	cn_time	[30];
} client_conn_info;

/* 
 * TCPServerThread에서 사용되는 소켓 지사자의 상태정보
 * 클라이언트와 녹취 서버 N개에 대한 소켓 지시자 정보 저장.
 *
 */
typedef struct {
	int	sock_fd;	/* 소켓 지시자 */
	int	sock_type;	/* 소켓 종류 : Client, Server */
	bool sock_state;	/* 소켓 상태 */
} sockfd_state_st;
#ifdef __cplusplus
extern "C" {
#endif
int tcp_connect(char * addr, int service, int tm);
char * long_to_net_addr(unsigned long int hn, char ** addr);
unsigned long int net_addr_to_long(char * addr);
int get_active_socket(sockfd_state_st * sockarr, int count, int tm);
int p_select(int sockfd, int sec, int usec);
int select_socket_status(int sockfd);
int net_recv_tm(int sockfd, char * rbuff, int sz, int tm);
int net_recv(int sockfd, char * buff, int sz);
int net_recv_size(int sockfd, int rbytes, char * buff, int maxlen, int tm);
int net_send(int sockfd, char * buff, int len);
void net_close(int fd);
int net_open_cli(int fd, client_conn_info ** c_info);
int tcp_server_init(char * addr, int service);
int udp_server_init(char * addr, int service);
int udp_client_init(char * addr, int service);
int udp_send(char * addr, int service, char * send_data, int sz);
int udp_recv(int sockfd, char * buff, int maxlen);
int TCP_PVC(int sockfd, char * sbuff, char * rbuff, int len, int maxlen, int tm);
#ifdef __cplusplus
}
#endif
#endif
