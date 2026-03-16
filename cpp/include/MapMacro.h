/*******************************************************************************
*
*     Copyright (c) 2001 FEELINGK Co., Ltd.
*     All rights reserved.
*
*     This program contains confidential and proprietary information of
*     FEELINGK, and any reproduction, disclosure, or use in whole or in part
*     by prior written agreement or permission of FEELINGK.
*
*                            FEELINGK Co., Ltd.
*         5F Roy B/D 90-6 Nonhyun-dong Kangnam-Gu, Seoul, 135-010 Korea
*
#******************************************************************************/

/***********************************************************************
    FILE NAME       :   mapMacro.h
    PURPOSE         :   Include file for OpenCall Extended API
    DATE OF WRITE   :   1999.10.26
    AUTHOR          :   Suk-Hun Jang
    REVISION        :   V 1.0
    Ver.    By      When        Reason
    ------- ------- ----------  -----------------------------------------
    1.0     shjang  1998.10.26  first coding
***********************************************************************/
#ifndef	_MAPMACRO_H_
#define	_MAPMACRO_H_

#define MAX_UNUSABLEPC_MUM 100
#define	MAX_VERTURE_MIN_NUM 10
#define	MAP_TOTAL_LEN	256
#define	PC_LENGTH		2
#define	IP_LENGTH		4
#define	MIN_LENGTH		10

#define MAX_SRG_NUM		16

/* connection macro */
typedef enum {
	CONNECT_STATDB = 1,
	CONNECT_ACTVDB,
	CONNECT_RECTDB,
	// New Plus
	CONNECT_PLUSDB,
	CONNECT_MSGQ,
	CONNECT_SOCKET,
	CONNECT_PCSSNSTAT,
	CONNECT_OPENCALL,
	CONNECT_TRANSDB,
	CONNECT_RCSADB,
	CONNECT_REDISDB,
	CONNECT_MAX
} eAseConnectionType;

#define	FAIL_BASE		100

#define	MEMORYDB_FAIL		(FAIL_BASE+0)
#define	MESSAGEQ_FAIL_DBH	(FAIL_BASE+1)
#define	MESSAGEQ_FAIL_SMPP	(FAIL_BASE+2)
#define	MESSAGEQ_FAIL_SCHD	(FAIL_BASE+3)
#define	MESSAGEQ_FAIL_MSGID	(FAIL_BASE+4)
#define	SOCKET_FAIL			(FAIL_BASE+5)
#define	TCAP_SEND_FAIL		(FAIL_BASE+6)
#define	MESSAGEQ_FAIL_S6C 	(FAIL_BASE+7)

#define     MAXSS7LINKSIZE  100

#define     SMT  1
#define     SMO  2

/* ASE Mode */
#define	ASE_MT	1
#define	ASE_MO	2
#define	ASE_ALL	3

#define		MAX_FD          127

#define     RECV_FROM_SRG   0x31
#define     RECV_NO_SRG     0x30

// Map Flow String Define.
#define InSmscStr   "GSMSC <== "
#define OutSmscStr  "GSMSC ==> "

// For Trace MapVersion description
typedef enum {
	CDMA_MAP_VERSION    = 0x01,
	GSM_MAP_VERSION     = 0x02,
	MME_MAP_VERSION 	= 0x03,
	SMSF_MAP_VERSION 	= 0x07
} etTraceMapV;

// Network Error Type.
typedef enum {
	CDMA_ERR_TYPE = 0x01,
	GSM_ERR_TYPE  = 0x02,
	MME_ERR_TYPE = 0x03,
	SMSF_ERR_TYPE = 0x07
} etNetErrType;

// Network Type.
typedef enum {
	CDMA_NET_TYPE   = 0x00,
	GSM_NET_TYPE    = 0x01,
	MME_NET_TYPE 	= 0x03,
	SMSF_NET_TYPE 	= 0x07
} etNetType;

#define MAP_DATE_TIME_FORM  "%Y.%m.%d %H:%M:%S"


#endif
