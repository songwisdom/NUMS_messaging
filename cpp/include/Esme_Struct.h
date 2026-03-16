/*******************************************************************************
*
*     Copyright (c) 2003 FEELINGK Co., Ltd.
*     All rights reserved.
*
*     This program contains confidential and proprietary information of
*     FEELINGK, and any reproduction, disclosure, or use in whole or in part
*     by prior written agreement or permission of FEELINGK.
*
*                            FEELINGK Co., Ltd.
*     3F Ace Techno Twin Tower 1, 212-1 Kuro-Dong, Kuro-Gu, Seoul 152-050 Korea
*
*******************************************************************************/

/*---------------------------------------------------------------------
 *
 *  PROC. NAME      : Esme_Struct.h
 *  DESCRIPTION     : ESME Common Struct Header file
 *  REVISION        :
 *      BY              WHEN        REASON
 *      --------------  ----------  -------------------
 *      smyang           2003.6     First Coding
*******************************************************************************/

#ifndef	_ESME_STRUCT_H_
#define	_ESME_STRUCT_H_

#include "MdbTableInfo.h"

#include "Esme_Macro.h"
#include "flk_type.h"

//Esme Server cfg
typedef struct {
	u_short		usPortID; // Unique Key
	u_short		bActFlag;
	int			nListenSocketID;
	char		cpRunApp[WORD_SIZE];
	char		cpDescription[WORD_SIZE];
	u_int		unCreateTime;
	u_int		unUpdateTime;
} stEsmeConCfg;


#define ESME_CFG_PORTID(spInfo)			(spInfo)->usPortID
#define ESME_CFG_ACT_FLAG(spInfo)		(spInfo)->bActFlag
#define ESME_CFG_LSOCKETID(spInfo)		(spInfo)->nListenSocketID
#define ESME_CFG_RUN_APP(spInfo)		(spInfo)->cpRunApp
#define ESME_CFG_CREATE_TIME(spInfo)	(spInfo)->nCreateTime
#define ESME_CFG_UPDATE_TIME(spInfo)	(spInfo)->nUpdateTime
#define ESME_CFG_DESCRIPTION(spInfo)	(spInfo)->cpDescription

typedef struct listenInfo {
	u_short		usPortID; // Unique Key
	u_short		usReserved;  // for Reserved & Alignment
	int			nListenSocketID;
	char		cpRunApp[BUF_SSIZE];
	char 		cpPort[BUF_STR];
} stLsnPortInfo;

#define ESME_LPORT_PORTID(spInfo)		(spInfo)->usPortID
#define ESME_LPORT_LSOCKETID(spInfo)	(spInfo)->nListenSocketID
#define ESME_LPORT_RUN_APP(spInfo)		(spInfo)->cpRunApp
#define ESME_LPORT_PORTSTR(spInfo)		(spInfo)->cpPort


typedef struct {
	u_int 			unLastTime;
	int         	nLsnPortNum;
	int         	nChanged;
	stLsnPortInfo	lsnInfo[ESME_MAX_LISTEN_PORT];
} stListenInfo;

#define ESME_LSN_PORTCNT(spInfo)		(spInfo)->nLsnPortNum
#define ESME_LSN_CHGCNT(spInfo)			(spInfo)->nChanged
#define ESME_LSN_LPORT(spInfo, idx)		(spInfo)->lsnInfo[idx]

//Esme Child list

typedef		struct {
	pid_t		ulPid; // Unique Key
	u_int		nEsmeID;
	u_short		usPortID;
	u_short		usProtocol;
	u_int		unPeerIP;
	u_int		unConnectTime;
	u_int		unStatusTime;  // Lock Check Time
	char		cpSysId[ESME_CLI_SYSID_LEN];    // 16 bytes
	char		cpPasswd[ESME_CLI_SYSPWD_LEN];	// 12 bytes
	u_char 		usConnectMode; // S2V u_short     usConnectMode: 8;
	u_char 		usEsmeClass; // S2V u_short     usEsmeClass: 8;
	u_short		usReserved;
} stEsmeClient;
//} __attribute__ ((packed))stEsmeClient;

#define ESME_CLIENT_PID(spInfo)			(spInfo)->ulPid
#define ESME_CLIENT_ESMEID(spInfo)		(spInfo)->nEsmeID
#define ESME_CLIENT_PORTID(spInfo)		(spInfo)->usPortID
#define ESME_CLIENT_PTC_TYPE(spInfo)	(spInfo)->usProtocol
#define ESME_CLIENT_CON_MODE(spInfo)	(spInfo)->usConnectMode
#define ESME_CLIENT_ESME_CLASS(spInfo)	(spInfo)->usEsmeClass
#define ESME_CLIENT_PEER_IP(spInfo)		(spInfo)->unPeerIP
#define ESME_CLIENT_CON_TIME(spInfo)	(spInfo)->unConnectTime
#define ESME_CLIENT_RUN_TIME(spInfo)	(spInfo)->unStatusTime
#define ESME_CLIENT_CON_SYSID(spInfo)	(spInfo)->cpSysId
#define ESME_CLIENT_CON_PSSWD(spInfo)	(spInfo)->cpPasswd

typedef struct {
	long 			mtype;       /* message type */
	u_int			dbCommand;
	int				dbResult;
	stEsmeClient	clientInfo;
} stEsmeCmd;

#define ESME_CMD_MSG_SIZE sizeof(stEsmeCmd) - sizeof(long) // for Cmd Msg Contents


typedef struct {
	const char	        *cpDaemon;
	const char	        *cpOffice;
	int		        nConnectSock;
	int				nPeerConnectSock; /* SMSC Peer Connect*/
	int				nSMSCMode;        /* Single, Dual */
	int				nRoutingMode;	  /* ODD, EVEN */
	int				nMDNRouteUse;	  /* SKT Route ON/OFF */
	u_int			unSmscId;
	u_int			unConnectTimeout;
	u_int			unRespMsgTimeout;
	u_int 			unSvcRespMsgTimeout; //SR511 Service Response Timeout
	u_int			unPollSockMtime;
	u_int			unPeerSockRecvtime;
	u_int			unSleepQTime;
	u_int			unMaxAdbQueueCnt;
	u_int			unMsgMaxSize;
	u_int			unMsgMaxSizeCut;
	u_int			unBExpressFlag;
	u_int			unClientMaxCnt[ESME_CMT_MAX];
	u_int			unDeliveryTimeOver;
	int 			nSyncMode;
	stEsmeClient	stClientInfo;
	u_int			unEnquiredlinkSendInterval;
	u_int 			unConnectInterval;
} stEsmeProcInfo;


#define ESME_PROC_EXP_FLAG(spInfo)		(spInfo)->unBExpressFlag

#define ESME_PROC_MAX_TRM(spInfo)		(spInfo)->unClientMaxCnt[ESME_CMT_TRANSMITTER]
#define ESME_PROC_MAX_RCV(spInfo)		(spInfo)->unClientMaxCnt[ESME_CMT_RECEIVER]
#define ESME_PROC_MAX_TRX(spInfo)		(spInfo)->unClientMaxCnt[ESME_CMT_TRANSCEIVER]

// Esme Client ID cfg
typedef		struct {
	u_int		nEsmeID;  // Unique Key
	u_short		usProtocol;
	u_short		usPortID;
	u_int		unPeerIP;
	u_char			nIPType;
	u_char			usEsmeClass;
	u_char			bExpressFlag;
	u_char			bActFlag;
	int			nMaxCnt[ESME_CMT_MAX];
	int			nActiveCnt[ESME_CMT_MAX];
	char			cpSysId[ESME_CLI_SYSID_LEN];
	char			cpPasswd[ESME_CLI_SYSPWD_LEN];
	u_int		unCreateTime;
	u_int		unUpdateTime;
	u_char 		ucEsmeMonFlag;
	/* FR220 For ESME Queye Type */
	u_char 		ucEsmeQType;
	/* FR220 For ESME ID Queue Count */
	short 		usEsmeQCnt;
	// DR520 Pkg
	u_int 		unDcsUseFlag;
	// DR710 pkg by cbh
	unsigned int unMessageLength;
	u_int 		nEsmeQKey;
	// DR710 pkg by cbh
	unsigned short usMaxQueueCnt;
	u_char 		ucSyncFlag;
	u_char 		ucReserved[DB_SYSID_RESERVED_SIZE];
} stEsmeIDInfo;
//} __attribute__ ((packed)) stEsmeIDInfo;

#define ESME_ID_ESMEID(spInfo)		(spInfo)->nEsmeID
#define ESME_ID_PTC_TYPE(spInfo)	(spInfo)->usProtocol
#define ESME_ID_PORTID(spInfo)		(spInfo)->usPortID
#define ESME_ID_PEER_IP(spInfo)		(spInfo)->unPeerIP
#define ESME_ID_IP_TYPE(spInfo)		(spInfo)->nIPType
#define ESME_ID_ESME_CLASS(spInfo)	(spInfo)->usEsmeClass
#define ESME_ID_FLAG_EXP(spInfo)	(spInfo)->bExpressFlag
#define ESME_ID_FLAG_ACT(spInfo)	(spInfo)->bActFlag
#define ESME_ID_MAX_TRM(spInfo)		(spInfo)->nMaxCnt[ESME_CMT_TRANSMITTER]
#define ESME_ID_MAX_RCV(spInfo)		(spInfo)->nMaxCnt[ESME_CMT_RECEIVER]
#define ESME_ID_MAX_TRX(spInfo)		(spInfo)->nMaxCnt[ESME_CMT_TRANSCEIVER]
#define ESME_ID_ACT_TRM(spInfo)		(spInfo)->nActiveCnt[ESME_CMT_TRANSMITTER]
#define ESME_ID_ACT_RCV(spInfo)		(spInfo)->nActiveCnt[ESME_CMT_RECEIVER]
#define ESME_ID_ACT_TRX(spInfo)		(spInfo)->nActiveCnt[ESME_CMT_TRANSCEIVER]
#define ESME_ID_CREATE_TIME(spInfo)	(spInfo)->nCreateTime
#define ESME_ID_UPDATE_TIME(spInfo)	(spInfo)->nUpdateTime
#define ESME_ID_CON_SYSID(spInfo)	(spInfo)->cpSysId
#define ESME_ID_CON_PSSWD(spInfo)	(spInfo)->cpPasswd
#define ESME_ID_ESME_MON_FLAG(spInfo)   (spInfo)->usEsmeMonFlag

#define ESME_ID_MAX_CNT(spInfo, mode)		(spInfo)->nMaxCnt[mode]
#define ESME_ID_ACT_CNT(spInfo, mode)		(spInfo)->nActiveCnt[mode]

typedef struct {
	u_int 	unMsgId;
	u_int 	unNextMsgId;
	u_int 	nMsgType;
	u_int	nDbType;
	u_short	usService;
	u_int	unTime;
	// FR320 PKG
	int 	nDeliverType;
	int 	nTryEvent;
} stEsmeDeliveryInfoRecord;

typedef stEsmeDeliveryInfoRecord 	stEsmeDeliveryInfoHead;

typedef struct  {
	char	cpCallbackMin[ESME_BLOCK_ADDR_LEN];
	char	cpSourceMin[ESME_BLOCK_ADDR_LEN];
	char	cpDestMin[ESME_BLOCK_ADDR_LEN];
} stEsmeBlockMin;

// DR710 pkg by cbh : get esmeidq maxcnt
int get_esmeidq_maxcnt(unsigned int esmeid);
int get_esme_2nd_routing_id(u_int esmeid_1st);

#endif //	_ESME_STRUCT_H_
