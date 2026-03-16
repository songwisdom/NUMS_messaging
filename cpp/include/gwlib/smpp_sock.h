/**
 * =====================================================================================
 *
 *       @file  smpp_sock.h
 *
 *    	 @brief 
 *
 *       Version:  1.0
 *       Compiler:  cc (HP-UX)
 *       @date:  10/11/12 00:42:43
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
 *     7F FEELingK Tower 1, Toungdeungpo-Dong 3-Ga, Youngdeungpo-Gu, Seoul 150-033 Korea 
 *
 * =====================================================================================
 */
#ifndef _H_SMPP_SOCK_
#define _H_SMPP_SOCK_
#include "smpp_pdu.h"
typedef struct {
	char 	id[65];
	int 	sub;
	int 	dlvrd;
	char 	subdate[16];
	char 	donedate[16];
	char 	stat[16];
	char 	err[16];
}SMPP_DLR_ST;

typedef enum { RET_CONN_FAIL = -1, RET_CONN_TIME_OUT = 0, RET_CONN_SUCCESS = 1 }eRetConnType;

int smpp_pdu_read_len(Connection *conn);
Octstr *smpp_pdu_read_data(Connection *conn, int len);
int send_smpp_pdu(Connection *conn, SMPP_PDU *pdu);
int read_smpp_pdu(Connection *conn, int *len, SMPP_PDU **pdu);
int wait_socket_event(Connection *conn, int tm_out);
int read_smpp_pdu_timeout(Connection *conn, int *len, SMPP_PDU **pdu, int tm_out);
#endif
