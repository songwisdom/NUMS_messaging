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
 *  PROC. NAME      : Esme_Macro.h
 *  DESCRIPTION     : ESME Common definition Header
 *  REVISION        :
 *      BY              WHEN        REASON
 *      --------------  ----------  -------------------
 *      smyang           2003.6     First Coding
*******************************************************************************/

#ifndef	_ESME_MACRO_H_
#define	_ESME_MACRO_H_

/***********************************************************************
 700 block Max number
***********************************************************************/
#define	ESME_BLOCK_ADDR_LEN 			24
#define	ESME_BLOCK_MAX_MIN_NUM			100

/***********************************************************************
 ESME GET Msg ID
***********************************************************************/
#define ESME_MSGID_RETRY_MAXNUM  		200
#define ESME_MSGID_SLEEP_MTIME  		100

/***********************************************************************
 ESME Msg Insert DBMS
***********************************************************************/
#define ESME_INSERTDBMS_RETRY_MAXNUM	10

/***********************************************************************
 ESME Client Info Length
***********************************************************************/
#define	ESME_CLI_SYSID_LEN				16
#define	ESME_CLI_SYSPWD_LEN				12

/***********************************************************************
 ESME Common
***********************************************************************/
#define ESME_FILE_BUF_SIZE              200
#define	ESME_MAX_LISTEN_PORT			300

/***********************************************************************
 ESME Return Error
***********************************************************************/
#define	ESME_CONN_ALREADY_EXIST			-2
#define	ESME_BIND_SYSIDFAIL				-11
#define	ESME_BIND_PASWDFAIL				-12
#define	ESME_BIND_PORTFAIL				-13
#define	ESME_BIND_IPFAIL				-14
#define	ESME_BIND_PROTOCOLFAIL			-15

/***********************************************************************
 ESME Connection Request Validation for Port
***********************************************************************/
#define	ESME_CRV_PORT_ALL_BLOCK			0
#define	ESME_CRV_PORT_ALL_ALLOWED		0xFFFF
#define	ESME_CRV_IP_ALL_BLOCK			0
#define	ESME_CRV_IP_ALL_ALLOWED			0xFFFFFFFF

/***********************************************************************
 * ESME IP Address Length
 * *********************************************************************/
#define ESME_IPADDRESS_LEN				16

/***********************************************************************
 * Dual Sync Connect Type
***********************************************************************/
#define ESME_DS_SMPP34		1
#define ESME_DS_SMDP		2
#define ESME_DS_SMPP		3

/*********************************************************************
 * ESME Dual Type
 *********************************************************************/
typedef enum _ESMEDualTypes {
	ESME_SERVER_TYPE  		= 0x01,		/* DUAL_ESME Server type */
	ESME_CLIENT_TYPE
} eESMEDualTypes;

#define PEER_SEND   1

/********************************************************************************
 		Deliver Msg Contenet Type 		Value
********************************************************************************/
typedef enum _ESMEDeliveryTypes {
	ESME_DMT_DELIVER_SM		= 0x01,	/*	DELIVER_SM contain SM	   */
	ESME_DMT_DELIVER_RECEIPT,		/*	DELIVER_SM contain Receipt */
	ESME_DMT_TRANSACTION_RESP,		/*	TRANSACTION MODE DATA_SM_RESP */
	ESME_DMT_DELIVER_STS_REPORT,	/*	TRANSACTION MODE DATA_SM_RESP */
	ESME_DMT_MAX
} eESMEDeliveryTypes;


/***********************************************************************
 * ESME I/F Protocol Types
***********************************************************************/
typedef enum _ESMEProtocolTypes {
	ESME_PTL_SMPP34		= 0x01,
	ESME_PTL_SMPP,
	ESME_PTL_SMDP,
	ESME_PTL_SMCI,
	ESME_PTL_SMPP3A,
	ESME_PTL_SMPP_SVC_3A,
	ESME_PTL_MAX
} eESMEProtocolTypes;

/***********************************************************************
 * ESME I/F Connfig Activate Flag Types
***********************************************************************/
typedef enum _ESMECfgActiveTypes {
	ESME_CFG_DISABLE,
	ESME_CFG_ENABLE,
	ESME_CFG_MAX
} eESMECfgActiveTypes;


/***********************************************************************
 * ESME I/F Express Flag Types
***********************************************************************/
typedef enum _ESMEExpressFlagTypes {
	ESME_EXPRESS_OFF,
	ESME_EXPRESS_ON
} eESMEExpressFlagTypes;

/***********************************************************************
 * ESME I/F Connection Mode Types
***********************************************************************/
typedef enum _ESMEConnectionModeTypes {
	ESME_CMT_TRANSMITTER,
	ESME_CMT_RECEIVER,
	ESME_CMT_TRANSCEIVER,
	ESME_CMT_MAX
} eESMEConnectionModeTypes;

/***********************************************************************
 *  * ESME I/F Connection Mode Types
 *  ***********************************************************************/
typedef enum _ESMEConnectionESMEClassTypes {
	ESME_CLASS_SMSGW,		// Vertical
	ESME_CLASS_TELCO_RGW,	// Horizontal
	ESME_CLASS_XMSS,		// SKT Forwarding Service
	ESME_CLASS_ETC,			// SKT Forwarding Service
	ESME_CLASS_MAX
} eESMEConnectionESMEClassTypes;

/***********************************************************************
 * ESME File Index Types
***********************************************************************/
typedef enum _ESMEFileIdxTypes {
	EMSE_FILE_IDX_PID,
	EMSE_FILE_IDX_SYSID,
	EMSE_FILE_IDX_DEFMSG,
	EMSE_FILE_IDX_MAX
} eESMEFileIdxTypes;

typedef enum _SMDPSFileIdxTypes {
	SMDPS_FILE_IDX_PID,
	SMDPS_FILE_IDX_ESME_CFG,
	SMDPS_FILE_IDX_LSN_PORT,
	SMDPS_FILE_IDX_MAX
} eSMDPSFileIdxTypes;

enum EsmeCommand {
	CMD_CLIENT_KILL, CMD_CLIENT_RUN
};

enum EsmeMsgQGetFlag {
	FLK_MSGQ_GET_ON 	= 0x01,
	FLK_MSGQ_GET_OFF 	= 0x02
};

#define MSG_CMD_ESME 1L


/***********************************************************************
 ESME Statistics Macro
***********************************************************************/
/* dbInsertStatistics	- DB 통계       */
/* dbInsertESMEID 		- SME ID별 통계 */
/* dbInsertMIN			- MIN 별 통계   */


#ifdef  STATISTICS_OLD
#define RSMDP_STATIC_SUCCESS(ulEsmeID, cpHexSrcAddr,  nHexSrcAddrLen) \
		DebugStr(DG_LEVEL_FOUR,"RSMDP_STATIC_SUCCESS ID[%d] MinLen[%d]\n", ulEsmeID, nHexSrcAddrLen); \
		dbInsertStatistics(DBMS_STAT,OID_ZERO_VALUE_RSMDP, OID_ONE_VALUE_RSMDP_SUCCESS); \
		dbInsertESMEID(DBMS_STAT,OID_ZERO_VALUE_RSMDP, OID_ONE_VALUE_RSMDP_SUCCESS, ulEsmeID); \
		if (nHexSrcAddrLen > 0) \
			dbInsertMIN(OID_ZERO_VALUE_RSMDP, OID_ONE_VALUE_RSMDP_SUCCESS, cpHexSrcAddr, nHexSrcAddrLen);

#define RSMDP_STATIC_FAIL(ulEsmeID, cpHexSrcAddr,  nHexSrcAddrLen) \
		DebugStr(DG_LEVEL_FOUR,"RSMDP_STATIC_FAIL ID[%d] MinLen[%d]\n", ulEsmeID, nHexSrcAddrLen); \
		dbInsertStatistics(DBMS_STAT,OID_ZERO_VALUE_RSMDP, OID_ONE_VALUE_RSMDP_FAIL); \
		dbInsertESMEID(DBMS_STAT,OID_ZERO_VALUE_RSMDP, OID_ONE_VALUE_RSMDP_FAIL, ulEsmeID); \
		if (nHexSrcAddrLen > 0) \
			dbInsertMIN(OID_ZERO_VALUE_RSMDP, OID_ONE_VALUE_RSMDP_FAIL, cpHexSrcAddr, nHexSrcAddrLen);
#endif

#endif	/* _ESME_MACRO_H_ */
