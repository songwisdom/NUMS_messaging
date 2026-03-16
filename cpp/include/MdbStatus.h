/*******************************************************************************
*
*     Copyright (c) 2000 FEELINGK Co., Ltd.
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

/*******************************************************************************
    FILE NAME       :	MdbStatus.h
    PURPOSE         :
    DATE OF WRITE   :
    AUTHOR          :
    REVISION        :   V 1.0
    Ver.    By      When        Reason
    ------- ------- ----------  -----------------------------------------
	1.0		shjang	2001.12.18	first coding
*******************************************************************************/
#ifndef _MDBSTATUS_H_
#define _MDBSTATUS_H_

#include "MdbErrRsn.h"


/*******************************************************************
*						Active MDB Define
********************************************************************/

/* CDMA Status */
#define	MDB_ST_SMSREQ_S				0x01	// SCHD <--> ASE
#define	MDB_ST_SMSREQ_R				0x02	// ASE <--> HLR

#define	MDB_ST_SMDPP_S				0x03
#define	MDB_ST_SMDPP_R				0x04
#define	MDB_ST_SMSNOT_R				0x05
#define MDB_ST_NOTI_SEND            0x06

/* UMTS Status */
#define MDB_ST_RIFSM_S              0x07 // SRI SEND
#define MDB_ST_RIFSM_R              0x08 // SRI RESP
#define MDB_ST_FSM_G_M_S            0x09 // MTF 1Node SGSN, 2Node MSC, : SGSN SEND
#define MDB_ST_FSM_G_M_R            0x0a // MTF 1Node SGSN, 2Node MSC, : SGSN RESP
#define MDB_ST_FSM_F_M_S            0x0b // MTF 1Node SGSN, 2Node MSC, : SGSN FAIL, MSC SEND
#define MDB_ST_FSM_F_M_R            0x0c // MTF 1Node SGSN, 2Node MSC, : SGSN FAIL, MSC RESP
#define MDB_ST_FSM_M_G_S            0x0d // MTF 1Node MSC, 2Node SGSN, : MSC SEND
#define MDB_ST_FSM_M_G_R            0x0e // MTF 1Node MSC, 2Node SGSN, : MSC RESP
#define MDB_ST_FSM_F_G_S            0x0f // MTF 1Node MSC, 2Node SGSN, : MSC FAIL, SGSN SEND
#define MDB_ST_FSM_F_G_R            0x10 // MTF 1Node MSC, 2Node SGSN, : MSC FAIL, SGSN RESP 
#define MDB_ST_FSM_M_S              0x11 // MTF 1Node MSC : MSC SEND
#define MDB_ST_FSM_M_R              0x12 // MTF 1Node MSC : MSC RESP
#define MDB_ST_FSM_G_S              0x13 // MTF 1Node SGSN : SGSN SEND
#define MDB_ST_FSM_G_R              0x14 // MTF 1node SGSN : SGSN RESP


#define MDB_ST_MME_MSC_BASE 			0x80
#define MDB_ST_FSM_MME_S 				0x81 // MTF 1Node MME SEND
#define MDB_ST_FSM_MME_R 				0x82 // MTF 1Node MME RESP
#define MDB_ST_FSM_MME_MSC_S 			0x83 // MTF 1Node MME 2Node MSC SEND
#define MDB_ST_FSM_MME_MSC_R 			0x84 // MTF 1Node MME 2Node MSC RESP 
#define MDB_ST_FSM_MME_FAIL_MSC_S 		0x85 // MTF 1Node MME Fail 2Node MSC SEND
#define MDB_ST_FSM_MME_FAIL_MSC_R 		0x86 // MTF 1Node MME Fail 2Node MSC RESP
#define MDB_ST_FSM_MSC_MME_S 			0x87 // MTF 1Node MSC 2Node MSC SEND 
#define MDB_ST_FSM_MSC_MME_R 			0x88 // MTF 1Node MSC 2Node MSC RESP 
#define MDB_ST_FSM_MSC_FAIL_MME_S 		0x89 // MTF 1Node MSC Fail 2Node MME SEND 
#define MDB_ST_FSM_MSC_FAIL_MME_R 		0x8a // MTF 1Node MSC Fail 2Node MME RESP

#define MDB_ST_RSMDS_MME_FAIL_S				0x8b
#define MDB_ST_RSMDS_MME_FAIL_R 			0x8c
#define MDB_ST_RSMDS_MME_SUCC_S 			0x8d
#define MDB_ST_RSMDS_MME_SUCC_R 			0x8e

#define MDB_ST_SMSF_MME_BASE 			0x90
#define MDB_ST_FSM_SMSF_S 				0x91 // MTF 1Node SMSF SEND
#define MDB_ST_FSM_SMSF_R 				0x92 // MTF 1Node SMSF RESP
#define MDB_ST_FSM_SMSF_MME_S 			0x93 // MTF 1Node SMSF 2Node MME SEND
#define MDB_ST_FSM_SMSF_MME_R 			0x94 // MTF 1Node SMSF 2Node MME RESP 
#define MDB_ST_FSM_SMSF_FAIL_MME_S 		0x95 // MTF 1Node SMSF Fail 2Node MME SEND
#define MDB_ST_FSM_SMSF_FAIL_MME_R 		0x96 // MTF 1Node SMSF Fail 2Node MME RESP
#define MDB_ST_FSM_MME_SMSF_S 			0x97 // MTF 1Node MME 2Node MME SEND 
#define MDB_ST_FSM_MME_SMSF_R 			0x98 // MTF 1Node MME 2Node MME RESP 
#define MDB_ST_FSM_MME_FAIL_SMSF_S 		0x99 // MTF 1Node MME Fail 2Node SMSF SEND 
#define MDB_ST_FSM_MME_FAIL_SMSF_R 		0x9a // MTF 1Node MME Fail 2Node SMSF RESP
#define MDB_ST_RSMDS_SMSF_FAIL_S		0x9b
#define MDB_ST_RSMDS_SMSF_FAIL_R 		0x9c
#define MDB_ST_RSMDS_SMSF_SUCC_S 		0x9d
#define MDB_ST_RSMDS_SMSF_SUCC_R 		0x9e

#define MDB_ST_SMSF_MSC_BASE 			0xa0
#define MDB_ST_FSM_SMSF_MSC_S 			0xa3 // MTF 1Node SMSF 2Node MSC SEND
#define MDB_ST_FSM_SMSF_MSC_R 			0xa4 // MTF 1Node SMSF 2Node MSC RESP 
#define MDB_ST_FSM_SMSF_FAIL_MSC_S 		0xa5 // MTF 1Node SMSF Fail 2Node MSC SEND
#define MDB_ST_FSM_SMSF_FAIL_MSC_R 		0xa6 // MTF 1Node SMSF Fail 2Node MSC RESP
#define MDB_ST_FSM_MSC_SMSF_S 			0xa7 // MTF 1Node MSC 2Node MSC SEND 
#define MDB_ST_FSM_MSC_SMSF_R 			0xa8 // MTF 1Node MSC 2Node MSC RESP 
#define MDB_ST_FSM_MSC_FAIL_SMSF_S 		0xa9 // MTF 1Node MSC Fail 2Node SMSF SEND 
#define MDB_ST_FSM_MSC_FAIL_SMSF_R 		0xaa // MTF 1Node MSC Fail 2Node SMSF RESP

#define MDB_ST_SMSF_SGSN_BASE 			0xb0
#define MDB_ST_FSM_SMSF_SGSN_S 			0xb3 // MTF 1Node SMSF 2Node SGSN SEND
#define MDB_ST_FSM_SMSF_SGSN_R 			0xb4 // MTF 1Node SMSF 2Node SGSN RESP 
#define MDB_ST_FSM_SMSF_FAIL_SGSN_S 	0xb5 // MTF 1Node SMSF Fail 2Node SGSN SEND
#define MDB_ST_FSM_SMSF_FAIL_SGSN_R 	0xb6 // MTF 1Node SMSF Fail 2Node SGSN RESP
#define MDB_ST_FSM_SGSN_SMSF_S 			0xb7 // MTF 1Node SGSN 2Node SGSN SEND 
#define MDB_ST_FSM_SGSN_SMSF_R 			0xb8 // MTF 1Node SGSN 2Node SGSN RESP 
#define MDB_ST_FSM_SGSN_FAIL_SMSF_S 	0xb9 // MTF 1Node SGSN Fail 2Node SMSF SEND 
#define MDB_ST_FSM_SGSN_FAIL_SMSF_R 	0xba // MTF 1Node SGSN Fail 2Node SMSF RESP

#define MDB_ST_RSMDS_F_F_S          0x15
#define MDB_ST_RSMDS_F_F_R          0x16
#define MDB_ST_RSMDS_F_M_S          0x17
#define MDB_ST_RSMDS_F_M_R          0x18
#define MDB_ST_RSMDS_F_G_S          0x19
#define MDB_ST_RSMDS_F_G_R          0x1a
#define MDB_ST_RSMDS_F_S            0x1b
#define MDB_ST_RSMDS_F_R            0x1c
//#define MDB_ST_SMC_ROUTING_S		0x1d
//#define MDB_ST_SMC_ROUTING_R		0x1e
#define MDB_ST_ALERT_R              0x1f        // Not used (Reserved)
#define MDB_ST_ALERT_SEND           0x20        // Not used (Reserved)
#define MDB_ST_RSMDS_G_S            0x21
#define MDB_ST_RSMDS_M_S            0x22
#define MDB_ST_RSMDS_G_R            0x23
#define MDB_ST_RSMDS_M_R            0x24
#define MDB_ST_RSMDS_S              0x25        // Not used (Reserved)
#define MDB_ST_RSMDS_R              0x26        // Not used (Reserved)

#define	MDB_ST_RETRY				0x30
#define MDB_ST_RETRY_SLEEP          0x31
#define MDB_ST_IMMEDIATELY_SEND     0x32

#define	MDB_ST_SUBMITTED			0x33
#define MDB_ST_RESUBMITTED			0x34

#define MDB_ST_ESME_SEND			0x35

#define MDB_ST_HEADER               0x36
#define MDB_ST_RESERVATION          0x37
#define MDB_ST_SKT_SVC_RETRY		0x38
#define MDB_ST_RETRY_WAIT           0x39        //FR530 pkg LEE 

#define	MDB_ST_GW_RECEIPT_S			0x40		// for SKT Generation MNP
#define	MDB_ST_GW_RECEIPT_R			0x41		// for SKT Generation MNP
#define MDB_ST_SC_ROUTING_CDMA_S	0x42
#define MDB_ST_SC_ROUTING_CDMA_R	0x43
#define	MDB_ST_SC_ROUTING_GSM_S		0x44
#define	MDB_ST_SC_ROUTING_GSM_R		0x45
#define	MDB_ST_SC_ROUTING_SGD_S		0x46 // SR720 PKG
#define	MDB_ST_SC_ROUTING_SGD_R		0x47 // SR720 PKG

/* SKT NPDB Status */
/*
#define	MDB_ST_SKT_NPREQ_S			0x50
#define	MDB_ST_SKT_NPREQ_R			0x51
*/
/* SKT RCS Service FR410 */
#define MDB_ST_RCS_MTF_S 			0x71
#define MDB_ST_RCS_MTF_R 			0x72
#define MDB_ST_RCS_REPORT_S 		0x73
#define MDB_ST_RCS_REPORT_R 		0x74 

#define MDB_ST_SIP_SUBMIT_REPORT_S 		0x75
#define MDB_ST_SIP_SUBMIT_REPORT_R 		0x76
#define MDB_ST_SIP_DELIVER_S 			0x77
#define MDB_ST_SIP_DELIVER_R 			0x78

#define MDB_ST_RSMDS_SIP_FAIL_S			0x79
#define MDB_ST_RSMDS_SIP_FAIL_R 		0x7a
#define MDB_ST_RSMDS_SIP_SUCC_S 		0x7b
#define MDB_ST_RSMDS_SIP_SUCC_R 		0x7c

/* SKT SVC Preprocessing by twkim 07.01.08 */
#define MDB_ST_SKT_SVC_PREPROCESSING_BASE       0x60
#define MDB_ST_SKT_SVC_PREPROCESSING_TOP        0x61
#define MDB_ST_SKT_SVC_PREPROCESSING_PENDING    0x62
#define MDB_ST_SKT_SVC_PREPROCESSING_SCHD       0x63

/* TOC traffic control */
#define MDB_ST_TOC_SEND             0x64
#define MDB_ST_TOC_RELEASE          0x65



#define	MDB_ST_FAIL_BASE			0xe0
#define	MDB_ST_FAIL_NEW        	    MDB_ST_FAIL_BASE
#define	MDB_ST_FAIL_RESERVED	    MDB_ST_FAIL_BASE
#define	MDB_ST_FAIL_ASE     	    0xe1
#define	MDB_ST_FAIL_USER_DELETE	    0xe2
#define	MDB_ST_FAIL_TIME_EXPIRE	    0xe3
#define	MDB_ST_FAIL_RETRY_FINISH    0xe4
#define	MDB_ST_FAIL_ETC			    0xe5
#define	MDB_ST_FAIL_ESME_CANCEL		0xe6

// Pre-Deleting Status
#define MDB_ST_SPAM_SM          	0xe7
#define MDB_ST_BLOCK_SM         	0xe8

//SKT MNP PORTED Status
#define MDB_ST_PORTED_OUT_MT		0xe9
#define MDB_ST_DOUBLE_PORTED_MT		0xea

#define	MDB_ST_FAIL         	    0xf0
#define	MDB_ST_SUCCESS				0xff

#define	MDB_MANAGEMENT				0x5f
#define	MDB_UPDATE					0x02

#define MDB_SSMDP_DELIVERY      	0x01
#define MDB_SSMDP_NONE_DELIVERY     0x00

//  Status on Receipt DB Only
#define RECEIPT_DELIVERY_FAIL 		0x02
#define RECEIPT_DELIVERY_TIME_EXPIRE 0x03

/* to processing receipt tuple in Dual Active */
#define MDB_OTHER_NONE_DELIVERY     0x5e
#define MDB_OTHER_SIDE_DELIVERY     0x5d

/* to processing active tuple in Dual Active */
#define MDB_ST_OTHER_SIDE_SUCCESS	0xfe
#define MDB_ST_OTHER_SIDE_SUBMITTED 0xd0
#define MDB_ST_OTHER_SIDE_WAIT_ACK  0xd1

/* to sms plus processing status */
typedef enum {
	MDB_ST_PLUS_BASE		= 0x40, // for Bit Mask.
	MDB_ST_BMCH_REQ_S       = MDB_ST_PLUS_BASE, // SCHD --> BMCH
	MDB_ST_BMCH_PLUS_S,		//0x41,	// BMCH --> Block Msg Checker on SMS+
	MDB_ST_BMCH_PLUS_R,		//0x42, // BMCH <-- Block Msg Checker on SMS+
	// No Use.
	MDB_ST_BMCH_REQ_R,		//0x43, // SCHD <-- Block Msg Checker IF.
	MDB_ST_SMFC_INSERT_S	= 0x4a, // --> SM Foreward Communicator on SMS+
	MDB_ST_SMFC_INSERT_R,   //0x4b, // <-- SM Foreward Communicator on SMS+
	MDB_ST_SMFC_UPDATE_S,   //0x4c, // --> SM Foreward Communicator on SMS+
	MDB_ST_SMFC_UPDATE_R,   //0x4d, // <-- SM Foreward Communicator on SMS+
	MDB_ST_PLUS_MAX			= 0x4f  // <-- SM Foreward Communicator on SMS+
} eSmsPlusMdbStatus;

/* to process npdb query */
#define	MDB_ST_NPQC_SEND			0x50	// SCHD --> NPQC
#define MDB_ST_NPQC_S				0x51	// NPQC --> NPDB
#define MDB_ST_NPQC_R				0x52	// NPQC <-- NPDB 

//FR220 010_PLUS_SVC
#define MDB_ST_SKT_ATI_S            0x53    // SCHD --> HLR for ATSI
#define MDB_ST_SKT_ATI_R            0x54    // HLR  --> HLR for ATSI

#define MDB_ST_SKT_MCS_SMS_SKIP 	0x55    // MCS_SMS_SKIP
#define MDB_ST_SKT_MCS_RCS_RECV 	0x56    // SMS DONE -> RCS RECV

/* Dual Service RMin Receive Stat */
#define MDB_ST_SMSREQ_DUALSVC		0x09	// ASE -> SCHD

//#define MDB_ST_NPQC_FAIL			0x52
//#define MDB_ST_NP_QUERY			0x50	// NPQC --> NPDB
/* FailReason
#define MDB_ST_NP_QUERY_OK			0x51	// NPQC <-- NPDB
#define MDB_ST_NP_QUERY_ERR			0x52	// NPQC <-- NPDB
#define MDB_ST_NPQC_TIMEOUT			0x53	// NPQC <-- NPDB
*/

/* Definitions of Message Flow Type */
#define FTYPE_MS_TO_MS		0x00	/* from Mobile Station 
									   to Mobile Station */
#define FTYPE_ESME_TO_MS	0x01	/* from External Short Message 
									   Entity to Mobile Station */
#define FTYPE_MS_TO_ESME	0x02	/* from Mobile Station
									   to External Shor Message Entity */
#define FTYPE_ESME_TO_ESME	0x03	/* from External Shor Message Entity 
									   to External Shor Message Entity
									   Maybe, Exception Type */
#define FTYPE_MS_TO_3G		0x08
#define FTYPE 3G_SMPPRECEIPT_TO_ESME	0x06
#define FTYPE_ESME_TO_3G	0x09


#define FROM_MS             0x00
#define FROM_ESME           0x01
#define TO_MS               0x00
#define TO_ESME             0x02

#define FROM_3G_SMPPRECEIPT	0x04
#define	FROM_2G_SMPP		FROM_3G_SMPPRECEIPT	// 2G SMSC -> 3G SMSC (GW SMPP Submit)
#define TO_3G				0x08
#define	TO_2G_SMPPRECEIPT	TO_3G	// 3G SMSC -> 2G SMSC (GW Receipt)
#define	FROM_2G_SMDPP		0x20	// 2G SMSC -> 3G SMSC (GW SMDPP)

#define SetFlowType(FlowType, FT_Flag)  (FlowType = FlowType | FT_Flag)
#define GetFlowTypeIn(NetType)			(NetType & 0x0F)

#define PrtFlowType(FlowType) \
			(FlowType == FTYPE_MS_TO_MS) ? "FROM_MS_TO_MS" : \
			(FlowType == FTYPE_ESME_TO_MS) ? "FROM_ESME_TO_MS" : \
			(FlowType == FTYPE_MS_TO_ESME) ? "FROM_MS_TO_ESME" : \
			(FlowType == FTYPE_ESME_TO_ESME) ? "FROM_ESME_TO_ESME" : \
			"Unknow Flow Type"

#define PrtFlowFlag(FlowFlag)   (FlowFlag == FROM_MS) ? "FROM_MS" : \
								(FlowFlag == FROM_ESME) ? "FROM_ESME" : \
								(FlowFlag == TO_MS) ? "TO_MS" : \
								(FlowFlag == TO_ESME) ? "TO_ESME" : \
								"Unknow Flow Flag"

/* Network Flow Type */
#define FROM_CDMA           0x10
#define FROM_GSM            0x20
#define FROM_MME 			0x30 // ADD SR220 PKG MME NODE
#define FROM_RCS 			0x50
#define	FROM_GW_CDMA		0x60 // Change CDMA NODE
#define FROM_SMSF 			0x70 // ADD SR610 PKG 5G SA
#define FROM_SC_SGD 		0x80 // ADD SR720 PKG  
#define FROM_CSCF 			0x90


#define TO_CDMA             0x01
#define TO_GSM              0x02
#define TO_MME 				0x03
#define TO_SC_GSM			0x04
#define TO_RCS 				0x05
#define	TO_SC_CDMA			0x06 // Change CDMA NODE
#define TO_SMSF 			0x07 // ADD SR610 PKG 5G SA 
#define TO_SC_SGD 			0x08 // SR720 PKG 
#define TO_CSCF 			0x09

//use in ConvertNetworkType()
typedef enum NetworkType {
	NETWORK_TYPE_CDMA = 0x01,
	NETWORK_TYPE_GSM,
	NETWORK_TYPE_MME,
	NETWORK_TYPE_SC_GS,
	NETWORK_TYPE_RCS,
	NETWORK_TYPE_SC_CDM,
	NETWORK_TYPE_SMSF,
	NETWORK_TYPE_SC_SGD,
	NETWORK_TYPE_CSCF,
	NETWORK_TYPE_MAX
} eNetworkType;

#define FTYPE_CDMA_TO_CDMA  0x11
#define FTYPE_CDMA_TO_GSM   0x12
#define FTYPE_GSM_TO_CDMA   0x21
#define FTYPE_GSM_TO_GSM    0x22

#define FTYPE_RCS_TO_CDMA 	0x51
#define FTYPE_RCS_TO_GSM 	0x52
#define FTYPE_RCS_TO_RCS 	0x54
#define FTYPE_CDMA_TO_RCS 	0x15
#define FTYPE_GSM_TO_RCS 	0x25

#define FTYPE_CDMA_TO_MME 	0x13
#define FTYPE_GSM_TO_MME 	0x23
#define FTYPE_MME_TO_MME 	0x33
#define FTYPE_MME_TO_CDMA 	0x31
#define FTYPE_MME_TO_GSM 	0x32

#define FTYPE_CDMA_TO_SMSF 	0x17
#define FTYPE_GSM_TO_SMSF 	0x27
#define FTYPE_MME_TO_SMSF 	0x37
#define FTYPE_SMSF_TO_SMSF 	0x77
#define FTYPE_SMSF_TO_CDMA 	0x71
#define FTYPE_SMSF_TO_GSM 	0x72
#define FTYPE_SMSF_TO_MME 	0x73

#define GetNetTypeIn(NetType)               (NetType & 0xF0)
#define GetNetTypeOut(NetType)              (NetType & 0x0F)
#define SetNetTypeIn(NetType, NT_Flag)      (NetType = (NetType & 0x0F) | NT_Flag)
#define SetNetTypeOut(NetType, NT_Flag)     (NetType = (NetType & 0xF0) | NT_Flag)

#define isGsmNetworkIn(NetType) ((NetType & 0xF0) == FROM_GSM) ? true : ((NetType & 0xF0) == FROM_RCS) ? true : false
/* Definitions of MNP Control infomation */

//#define	ALREADY_PORTED					0x01
#define	MNP_HLR_PORTED_ACK					0x01	// setting from HLR smsreq
#define SetMnpHlrFlag(HlrFlag, HlrInfo)		(HlrFlag |= HlrInfo)
#define GetMnpHlrFlag(HlrFlag) 				(HlrFlag)

//#define RECEIVE_MNP_FROM_ESME	0x10
#define MNP_NPDB_QUERY_DONE	0x01	// set from NPQC after NP Query processing
#define SetMnpNpdbFlag(NpdbFlag, NpdbInfo)  	(NpdbFlag |= NpdbInfo)
#define GetMnpNpdbFlag(NpdbFlag)  				(NpdbFlag & 0x0F)

//#define ALREADY_SEND_NPDB			0x01
#define MNP_NPDB_QUERY_NOT_NEED		0x10    // setting from ESME MNP Type
#define SetMnpQueryFlag(QueryFlag, QueryInfo)	(QueryFlag |= QueryInfo)
#define GetMnpQueryFlag(QueryFlag)  			(QueryFlag & 0xF0)

//#define SKT CAPABILITY VALUE
#define SKT_OSFI_CAPA_ALL_MASK	4294967295
/* SKT SVC Preprocessing by twkim 07.01.08 */
//R910
//#define SKT_PREPROCESSING_CAPA_MASK     (11094 + SKT_CAPA_RCS)	//SPAM, ICS, PICS, TZSMS, MGR_CAPA, IMSMS, PERFECT_AA, NEW SPAM 
//SPAM, ICS, PICS, TZSMS, MGR_CAPA, IMSMS, PERFECT_AA, NEW SPAM, SKT_CAPA_MCS_MODE_DUAL, SKT_CAPA_MCS_MODE_SINGLE
#define SKT_PREPROCESSING_CAPA_MASK     (11094 + SKT_CAPA_RCS + SKT_CAPA_MCS_MODE_DUAL + SKT_CAPA_MCS_MODE_SINGLE)

#define SKT_IMMET_TRANS_CAPA_MASK       (SKT_CAPA_PICS + SKT_CAPA_MGR + SKT_CAPA_RCS)
/* SKT SVC Preprocessing End */
#define SKT_CAPA_LONG			1
#define SKT_CAPA_PICS			2		// config svcidroute value:0x0002
#define SKT_CAPA_SPAM			4		// config svcidroute value:0x0003
#define SKT_CAPA_COLOR_LONG		8		// config svcidroute value:0x0004
#define SKT_CAPA_ICS			16		// config svcidroute value:0x0005
#define SKT_CAPA_MMS			32		// config svcidroute value:0x0006
#define SKT_CAPA_TZSMS			64		// config svcidroute value:0x0007
#define SKT_CAPA_QUERYSMS		128		// config svcidroute value:0x0008
#define SKT_CAPA_IMSMS			256		// config svcidroute value:0x0009
#define SKT_CAPA_MGR			512		// config svcidroute value:0x000a
#define	SKT_CAPA_PERFECT		1024	// config svcidroute value:0x000b
#define	SKT_CAPA_PERFECT_AA		2048	// config svcidroute value:0x000c
#define	SKT_CAPA_PATRIOT		4096	// config svcidroute value:0x000d
#define	SKT_CAPA_NEW_SPAM		8192	// config svcidroute value:0x000e
#define	SKT_CAPA_ABROADTEL		16384	// config svcidroute value:0x000f
#define	SKT_CAPA_BIZ_NUM		32768	// for Biz Num SVC 
#define	SKT_CAPA_URL_NOTI		524288	// for Url_Noti SVC
#define SKT_CAPA_CALLBACK_NOTI 	1048576 // FR230 for Callback NOTI : config svcidroute value:0x0300
#define SKT_CAPA_RCS 			2097152 // for svcidroute value = 0x0301
#define SKT_CAPA_5G_SA_UE 		8388608 // SR610 pkg 5G SA UE
#define SKT_CAPA_DS_DS_UE		16777216 // SR820 pkg DS DS UE 
#define SKT_CAPA_MCS_MODE_DUAL 		33554432 // SR910 PKG
#define SKT_CAPA_MCS_MODE_SINGLE 	67108864 // SR910 PKG

//#define SKT OSFI VALUE
#define SKT_BEFOR_OSFI_VIMS		1
#define SKT_BEFOR_OSFI_CNAP		2
#define SKT_BEFOR_OSFI_PSMS		4
#define SKT_BEFOR_OSFI_ASMS		8
#define SKT_BEFOR_OSFI_VTS		16
#define SKT_BEFOR_OSFI_CCS		32
#define SKT_BEFOR_OSFI_CNHPPLUS	64
#define SKT_BEFOR_OSFI_CMF		128
#define SKT_BEFOR_OSFI_ZBVPN	256
#define SKT_BEFOR_OSFI_SNAP		512
//#define SKT_BEFOR_OSFI_SMS_NOTI	1024
#define SKT_BEFOR_OSFI_SMS_NOTI	262144
#define SKT_BEFOR_OSFI_SMS_MGR	4096
#define SKT_BEFOR_OSFI_PPS		32768

// FR220 CYSMS->010 Plus
//#define SKT_BEFOR_OSFI_CYSMS	33554432	//CYSMS SVC
#define SKT_BEFOR_OSFI_010_PLUS	33554432	//010_PLUS_SVC
#define SKT_BEFOR_OSFI_WZONE	536870912	//W-Zone SVC
#define SKT_BEFOR_OSFI_INTER_MO_DENY	1073741824 //INTER_SMSMO_DENY SVC

//#define SKT_OSFI_VIMS			1
//#define SKT_OSFI_CNAP			2
#define SKT_OSFI_BASE			0x0800
/* SKT SVC Preprocessing by twkim 07.01.08 */
#define SKT_IMMET_TRANS_BASE    0x8000
/* SKT SVC Preprocessing End */
#define SKT_OSFI_PSMS			1	// config svcidroute value:0x0011(dec:17)
#define SKT_OSFI_ASMS			2	// config svcidroute value:0x0012(dec:18)
//#define SKT_OSFI_VTS			16
//#define SKT_OSFI_CCS			32
//#define SKT_OSFI_CNHPPLUS		64
//#define SKT_OSFI_CMF			128
//#define SKT_OSFI_ZBVPN		256
#define SKT_OSFI_SNAP			4	// config svcidroute value:0x0013(dec:19)
#define SKT_OSFI_SMS_NOTI		8
#define SKT_OSFI_PPS			16
#define SKT_OSFI_MGR			32	// config svcidroute value:0x0016(dec:22)
// FR220 CYSMS->010 Plus
//#define SKT_OSFI_CYSMS			64	// config svcidroute value:0x0017(dec:23) CYSMS SVC
#define SKT_OSFI_010_PLUS       64 	 // config svcidroute value:0x0017(dec:23) 010PLUS SVC
#define SKT_OSFI_WZONE			128	// config svcidroute value:0x0018(dec:24) W-Zone SVC
#define SKT_OSFI_INTER_MO_DENY	256 // config svcidroute value:0x0019(dec:25) //INTER_SMSMO_DENY SVC 

//FR210 service on/off
#define SKT_UNKNOWNSUBSCRIBER_SERVICE  	256// config svcidroute value:0x0100 결번안내 NOTI
#define SKT_LMS_RELAY_SERVICE 			512// config svcidroute value:0x0200 LMS_LELAY_SVC
//FR220 010PLUS on/off
#define SKT_010_PLUS_SERVICE 	23
//DR430 Special Msg Process
#define SKT_SPECIAL_MESSAGE_PROCESS     1024// config svcidroute value:0x0400 SPECIAL_MESSAGE_PROCESS
//FR240 FaceTime Service
#define SKT_FACE_TIME_SERVICE           1280 // FR240 config svcidroute value:0x0500 SKT APLLE FACE TIME

//FR302 PKG TP-MMS ON / OFF
#define SKT_TP_MMS_ON_OFF 				1536 // FR302 Pkg value:0x0600

//FR310 PKG
#define SKT_060SPAM_TRAP_SERVICE 		48   // FR310 SKT 060SPAM TRAP Service value : 0x0030
#define SKT_TP_SGSN_NODE_SVC 			1537 // FR310 SKT TWO NODE SVC ON/OFF value : 0x601
#define SKT_TP_MSC_NODE_SVC 			1538 // FR310 SKT TWO NODE SVC ON/OFF value : 0x602
//FR320 PKG
#define SKT_EXT_DCS_SVC_ON_OFF          1539 // SKT External DCS SVC ON/OFF /value : 0x603
#define SKT_ERROR_HISTORY 				1542 // SKT Error History ON / OFF value : 0x606

//FR401 PKG
//#define SKT_SMS_MO_SAC_SVC_ON_OFF       1540 //SKT Sms MO SAC SVC ON/OFF / value : 0x604 LSH DR520
#define SKT_MVNO_SVC_ON_OFF             1541 //SKT MVNO SVC ON/OFF /value : 0x605   LSH DR520
// DR610
#define SKT_SNAP_SVC_ON_OFF             19 //DR610 SKT SNAP SVC ON/OFF       / value : 0x0013 HJY
#define SKT_ICS_SVC_ON_OFF              5  //DR610 SKT ICS SVC ON/OFF        / value : 0x0005 HJY
#define SKT_PERFECT_AA_SVC_ON_OFF       12 //DR610 SKT PERFECT_AA SVC ON/OFF / value : 0x000c HJY
#define SKT_HLR_NPFI_SVC_ON_OFF         1543 //DR610 SKT SKT_HLR_NPFI_SVC_ON_OFF / value : 0x0607 HJY NPFI
#define SKT_EPE_RELINK_ON_OFF 			1544 //DR610 SKT_ EQUIPMENET PROTOCOL ERROR Relink ON/OFF 0x608 
#define SKT_MSG_REF_16BIT_ON_OFF		1545 //DR610 Concatenated SMS On/Off 
#define SKT_INPUT_MSG_LEN_ZERO_ON_OFF 	1546 //DR610 MO/MT Message Length 0 limit;
#define SKT_IPMG_ROUTE 					1547 //DR610 IPMG PC / SSN (on/off)
#define SKT_MAKE_CDR_ON_OFF             1548 //DR620 Make CDR ( on/off )
#define SKT_APP_PORT_DENIED             1549 //DR620 APP PORT DENI
#define SKT_CALLBACK_CHG_DENY_SVC_ON_OFF   	1550 // DR710 pkg by Cbh : CALLBACK_CHG_DENY_SVC_ON_OFF flag
#define SKT_NUMBER_PLUS_SVC_ON_OFF         	1551 // DR710 hjy ( on/off ) 
#define SKT_SECURITY_SMS_SVC_ON_OFF        	1552 // DR710 hjy ( on/off )
#define SKT_140_EXP_ON_OFF                 	1553 // DR710 chan (on/off)
#define SKT_IPMG_CALLBACK_SET 			   	1554 // DR710 chan (set / off)
#define SKT_MOMULTINUM_SVC 				   	1555 // DR730 hjy
#define SKT_MTMULTINUM_SVC 					1556 // DR730 hjy
#define SKT_ALERT_SVC                      	1557 //DR730 LEE
#define SKT_FTN_ON_OFF 					   	1558 // DR730 hjy
#define SKT_STAR73_ESME_NUMBERPLUS_ON_OFF  	1559 // SR120 HJY
#define SKT_EPE_REPORT_SM_ON_OFF 			1560 // SR120 hjy
#define SKT_LIMIT_NEW_SPAM_ON_OFF 			1561 // SR120 hjy
#define SKT_STAR73_MO_NUMBERPLUS_ON_OFF 	1562 // SR120 hjy
#define SKT_STAT_NCHANGE_ON_OFF             1563 // SR120 HJY
#define SKT_CID_CHANGE_ON_OFF 			  	1564 // FR720 hjy
#define SKT_MME_NODE_OFF 					1565 // SR220 lch
#define SKT_WMSC_2ND_NODE_ON_OFF 			1566 // SR230 JYH
#define SKT_HLR_BK_ON_OFF 					1567 // SR230 hjy
#define SKT_MME_CONCAT_TO_LMSGW				1568 // SR310 LCH
#define SKT_MISTAKE_DIAL 					1569 // SR310 hjy
#define FAILREASON_LOG_PRINT				1570 // SR320 
#define SKT_ROAMING_LIMIT_ON_OFF 		  	1571 // SR320 hjy
#define SKT_TzKM_ON_OFF  					7 // SR310 ljh
#define SKT_M2M_GLR_ROUTE 					1572 // SR410 hjy
#define SKT_MCC_ON_OFF 						1573 // SR410 
#define SKT_DIA_HLR_ON_OFF 					1574 // SR420 
#define SKT_DIA_HLR_BK_AUTO_FLAG 			1575 // SR510 
#define SKT_SMSF_NODE_OFF 					1576 // SR610 hjy
#define SKT_SMSF_PRIORITY					1577 // SR610 hjy
#define SKT_MME_SC_ADDR_TYPE_ON_OFF 		1578 // SR620 MME Sc Addr Type
#define SKT_SMSF_SC_ADDR_TYPE_ON_OFF 		1579 // SR620 SMSF Sc Addr Type
#define SKT_RSP_M2M_SVC_MO_ON_OFF 			1580 // SR710 jhlee ( MO on/off )
#define SKT_RSP_M2M_SVC_MT_ON_OFF 			1581 // SR710 jhlee ( MT on/off )
#define SKT_CDF_MSGID_ADD_ON_OFF			1582 // SR710 HJY CDF MsgID Add On/Off
#define SKT_SMSC_NODE_OFF 					1583 // SR720 jhlee ( SMSC Routing On/Off)
#define SKT_INFO_RATE_DENY_ON_OFF 			1584 // SR810 HJY ( Info Rate Deny On/Off)
#define SKT_PMC_ON_OFF 						1585 // SR810 jhlee ( Pending Message Control On/Off)
#define SKT_PMC_MMDB_DELETE_ON_OFF 			1586 // SR810 HJY ( Pending Message MMDB Delete On/Off)
#define SKT_DS_DS_UE_3G_PRIORITY 			1587 // SR820 HJY DS DS UE 3G PRIORITY
#define SKT_DS_DS_UE_LTE_PRIORITY 			1588 // SR820 HJY DS DS UE LTE PRIORITY
#define SKT_DS_DS_UE_5G_PRIORITY 			1589 // SR820 HJY DS DS UE 5G PRIORITY
#define SKT_GLR_NODE_PRIORITY 				1590 // SR820 HJY GLR NODE PRIORITY
#define SKT_KISA_ORG_SRC_MIN_ON_OFF 		1591 // SR820 HJY KISA ORG SRC MIN ON OFF
#define SKT_DS_DS_UE_SVC_ON_OFF 			1592 // SR820 HJY DS DS UE SVC ON OFF  
#define SKT_MO_MCS 							1593 // SR910 HJY MO MCS
#define SKT_MT_MCS 							1594 // SR910 HJY MT MCS 
#define SKT_ROAMING_NOTI_ON_OFF 			1595 // L100 HJY 
#define SKT_INTER_MO_NEW_SPAM_ON_OFF 		1596 // L100 HJY
#define SKT_ESME_2ND_ROUTING_ON_OFF  		1597 // L100 HJY 
#define SKT_INTER_SMS_ESME_ID_ON_OFF 		1598 // L100 HJY
#define SKT_IMS_PRIORITY					1599 // L110 HJY
#define SKT_IMS_ON_OFF 						1600 // L110 HJY 
#define SKT_PROF_DEL_REGI_ON_OFF 			1601 // L110 HJY
#define SKT_DEFAULT_POOL_ROUTING 			1602 // L110 HJY 
#define SKT_SC_ROUTING_UDM_DEREGI_SEND 		1603 // L110 HJY
// #define SKT_CSCF_NODE_OFF 					1600 // L110

#define SKT_060SPAM_TRAP_PRIMARY    0x1030 // FR310 
#define SKT_060SPAM_TRAP_SECONDARY  0x2030 // FR310 
#define SKT_SFS_PRIMARY             0x100e // FR310 
#define SKT_SFS_SECONDARY           0x200e // FR310 

//FR720 pkg SMS_OSFI
#define SKT_SMS_OSFI_CID_CHANGE 			549755813888 // 0x8000000000 : CID-Change 		//FR720
#define SKT_SMS_OSFI_ROAMING_LIMIT 			274877906944 // 0x4000000000 : Roaming-Limit 	//SR320
#define SKT_SMS_OSFI_INFO_RATE_DENY 		137438953472 // 0x2000000000 : Info-Rate-Deny 	//SR810

#define SKT_SMS_OSFI_MCS_MODE_DUAL 			68719476736  // 0x1000000000 : MCS Mode Dual 	//SR910
#define SKT_SMS_OSFI_MCS_MODE_SINGLE 		34359738368  // 0x0800000000 : MCS Mode Single 	//SR910

#define SKT_SPAM_SVC_ON_OFF 3
//memcpy target int, DB Field Name(dbOSFI or dbOSFIFlag or dbCapability or dbCapabilityFlag),
//OSFI Define Value or Capa Define Value, return int(1:masking, 0:no masking)
#define OSFI_CAPA_IN_OUT_CHECK(revents,revents2,revents3,revents4)\
            OSFI_CAPA_INT_RESET(revents);\
            OSFI_CAPA_INT_RESET(revents4);\
			OSFI_CAPA_TO_INT(revents,revents2);\
			OSFI_CAPA_CHECK(revents, revents3, revents4);\

#define OSFI_CAPA_INT_RESET(revents) (revents=0)
#define OSFI_CAPA_TO_INT(revents,revents2) (memcpy(&revents,revents2,sizeof(int)))
#define OSFI_CAPA_CHECK(revents, revents3, revents4)\
				((revents & revents3) == revents3) ? (revents4 = 1U) : (revents4 = 0U)

// Memcpy Target int, DB Field Name(dbOSFI or dbOSFIFlag or dbCapability or dbCapabilityFlag),
// OSFI Define Value or Capa Define Value
#define OSFI_CAPA_ENABLE(revents,revents2,revents3)\
			OSFI_CAPA_INT_RESET(revents);\
			OSFI_CAPA_TO_INT(revents,revents2);\
			OSFI_CAPA_ENABLE_OPERATION(revents,revents3);\
			OSFI_CAPA_TO_DBFIELD(revents2,revents);\

#define OSFI_CAPA_ENABLE_OPERATION(revents,revents3) \
			((revents & revents3) == 0) ? (revents|=revents3) : (revents = revents)
#define OSFI_CAPA_DISABLE_OPERATION(revents,revents3)\
			((revents & revents3) == revents3) ? (revents = revents-revents3) : (revents=revents)
#define OSFI_CAPA_TO_DBFIELD(revents2,revents) (memcpy(revents2,&revents,sizeof(int)))

// Memcpy Target int, DB Field Name(dbOSFI or dbOSFIFlag or dbCapability or dbCapabilityFlag),
// OSFI Define Value or Capa Define Value
#define OSFI_CAPA_DISABLE(revents,revents2,revent3)\
			OSFI_CAPA_INT_RESET(revents);\
			OSFI_CAPA_TO_INT(revents,revents2);\
			OSFI_CAPA_DISABLE_OPERATION(revents,revent3);\
			OSFI_CAPA_TO_DBFIELD(revents2,revents);\

// DB Field Name(dbOSFI or dbOSFIFlag or dbCapability or dbCapabilityFlag),
// memcopy target int, return value(0:not is, 1: is)
#define OSFI_CAPA_IS_CHECK(revents,revents2,revents3)\
			OSFI_CAPA_INT_RESET(revents2);\
			OSFI_CAPA_INT_RESET(revents3);\
			OSFI_CAPA_TO_INT(revents2,revents);\
			((revents2&SKT_OSFI_CAPA_ALL_MASK) == 0) ? (revents3=0):(revents3=1)

//FR720 pkg
#define SKT_SMS_OSFI_ALL_MASK 1099511627775 // 0xFFFFFFFFFF CID_CHANGE + ROAMING_LIMIT

/* SKT SVC Preprocessing by twkim 07.01.08 */
#define OSFI_CAPA_PARAM_IS_CHECK(revents,revents2,revents3, revents4)\
			OSFI_CAPA_INT_RESET(revents2);\
			OSFI_CAPA_INT_RESET(revents3);\
			OSFI_CAPA_TO_INT(revents2,revents);\
			((revents2&revents4) == 0) ? (revents3=0):(revents3=1)
/* SKT SVC Preprocessing End */

//
//	SKT SHARP, STAR digit processing
//	GSM  #(H'b), *(H'a)
//	CDMA #(H'e), *(H'd)
//	revents1 : FROM_GSM (0x20)
//
#define	CHECK_DIGIT_SHARP(revents1, revents2, revents3) \
		(revents1==0x20) ? (revents2=0xb0, revents3=0x0b) : (revents2=0xe0, revents3=0x0e);

#define	CHECK_DIGIT_STAR(revents1, revents2, revents3) \
		(revents1==0x20) ? (revents2=0xa0, revents3=0x0a) : (revents2=0xd0, revents3=0x0d);

//FR720 pkg SMS SMS_OSFI
#define SMS_OSFI_LONG_RESET(revents) (revents=0)
#define SMS_OSFI_TO_LONG(revents,revents2) (memcpy(&revents,revents2,sizeof(long)))
#define SMS_OSFI_CHECK(revents,revents3,revents4)\
				((revents&revents3) == revents3) ? (revents4=1):(revents4=0)
#define SMS_OSFI_ENABLE_OPERATION(revents,revents3) \
			((revents & revents3) == 0) ? (revents|=revents3) : (revents = revents)
#define SMS_OSFI_TO_DBFIELD(revents2,revents) (memcpy(revents2,&revents,sizeof(long)))
#define SMS_OSFI_DISABLE_OPERATION(revents,revents3)\
			((revents & revents3) == revents3) ? (revents = revents-revents3) : (revents=revents)

#define SMS_OSFI_IN_OUT_CHECK(revents,revents2,revents3,revents4)\
            SMS_OSFI_LONG_RESET(revents);\
            SMS_OSFI_LONG_RESET(revents4);\
			SMS_OSFI_TO_LONG(revents,revents2);\
			SMS_OSFI_CHECK(revents,revents3,revents4);\

#define SMS_OSFI_ENABLE(revents,revents2,revents3)\
			SMS_OSFI_LONG_RESET(revents);\
			SMS_OSFI_TO_LONG(revents,revents2);\
			SMS_OSFI_ENABLE_OPERATION(revents,revents3);\
			SMS_OSFI_TO_DBFIELD(revents2,revents);\

#define SMS_OSFI_DISABLE(revents,revents2,revent3)\
			SMS_OSFI_LONG_RESET(revents);\
			SMS_OSFI_TO_LONG(revents,revents2);\
			SMS_OSFI_DISABLE_OPERATION(revents,revent3);\
			SMS_OSFI_TO_DBFIELD(revents2,revents);\

#define SMS_OSFI_IS_CHECK(revents,revents2,revents3)\
			SMS_OSFI_LONG_RESET(revents2);\
			SMS_OSFI_LONG_RESET(revents3);\
			SMS_OSFI_TO_LONG(revents2,revents);\
			((revents2&SKT_SMS_OSFI_ALL_MASK) == 0) ? (revents3=0):(revents3=1)

// to SKT Service processing SubStatus
typedef enum {
	/* Service_req status */
	SUB_ST_BASE				 = 0x00, // Status
	SUB_ST_SVC_SENDQ		 = 0x01, // SCHD  -> Queue
	SUB_ST_SVC_RECVQ       	 = 0x02, // Queue -> SMPP
	SUB_ST_SVC_REQ_R		 = 0x03, // sending
	SUB_ST_SVC_REQ_ERROR 	 = 0x04, // send fail
	SUB_ST_SVC_RESP_TIME_OUT = 0x05, // Ack Time out
	SUB_ST_SVC_RESP_ERROR	 = 0x06, // Ack Data Error
	//SUB_ST_SVC_RESP_SECRET   = 0x08, // Secret process
	SUB_ST_SVC_RESP_DISCON   = 0x09, // disconnect_call process (PICS/ICS)
	SUB_ST_SVC_RESP_TEXT	 = 0x0a, // Snap process
	SUB_ST_SVC_SPAM			 = 0x0b, // Spam
	SUB_ST_SVC_RESP_CONTINUE = 0x0c, // Ack No Service
	SUB_ST_SVC_CONNECT_FAIL	 = 0x0d, // Connection Fail
	SUB_ST_SVC_DELIVER_READY = 0x0e, // Continue process
	SUB_ST_SVC_NOQUERY		 = 0x0f, // Service_req not sending
	/* Deliver_sm status */
	SUB_ST_DEL_SENDQ         = 0x10, // SCHD  -> Queue
	SUB_ST_DEL_RECVQ         = 0x20, // Queue -> SMPP
	SUB_ST_DEL_REQ_R         = 0x30, // after send
	SUB_ST_DEL_RESP_SUCCESS  = 0x40, // Ack ok.
	SUB_ST_DEL_NODELIVER     = 0x50, // uhis -> rdbms
	SUB_ST_DEL_UNAVAIL_TID	 = 0x60, // Unavailable TID by twkim 06.11.08
	SUB_ST_DEL_REQ_ERROR     = 0x80, // send fail
	SUB_ST_DEL_RESP_TIMEOUT  = 0x90, // Ack Time out
	SUB_ST_DEL_RESP_ERROR    = 0xa0, // Ack Data Error
	SUB_ST_DEL_CONNECT_FAIL  = 0xb0,  // Connection Fail
	SUB_ST_DEL_SCROUTE       = 0xc0, // SC Routing
	SUB_ST_DEL_SVCOFF        = 0xd0  // Service on/off -> off
} eSmsMdbSubStatus;

//SR310
typedef enum {
	/* TzSMS Service_req status */
	//SUB_ST_BASE				 		= 0x00, // Status
	//SUB_ST_SVC_SENDQ		 			= 0x01, // SCHD  -> Queue
	//SUB_ST_SVC_RECVQ       	 		= 0x02, // Queue -> SMPP
	//SUB_ST_SVC_REQ_R		 			= 0x03, // sending
	//SUB_ST_SVC_REQ_ERROR 	 			= 0x04, // send fail
	//SUB_ST_SVC_RESP_TIME_OUT 			= 0x05, // Ack Time out
	SUB_ST_SVC_E_OK 		 			= 0x06,
	SUB_ST_SVC_E_OK_NO_ENCRYPTION 		= 0x07,
	SUB_ST_SVC_E_NO_SUBS 				= 0x08,
	SUB_ST_SVC_E_OVER_SMS_LEN 			= 0x09,
	SUB_ST_SVC_E_OK_ENCRYPT_TARGET 		= 0x0a,
	SUB_ST_SVC_E_OK_ENCRYPT_NO_TARGET 	= 0x0b,
	//SUB_ST_SVC_CONNECT_FAIL	 		= 0x0d, // Connection Fail
	SUB_ST_SVC_E_FORMAT_INVALID 		= 0x10,
	SUB_ST_SVC_E_SYS_FAIL 				= 0x20,
	SUB_ST_SVC_E_ENCRYPT_FAIL 			= 0x30,
	SUB_ST_SVC_E_MSGQ_FAIL 				= 0x40,
	SUB_ST_SVC_E_FC_NACK 				= 0x50,
	//SUB_ST_DEL_UNAVAIL_TID	 		= 0x60, // Unavailable TID by twkim 06.11.08
	SUB_ST_SVC_E_ETC 					= 0x70,
	SUB_ST_SVC_E_PARAMETER_ERROR		= 0x80,
	SUB_ST_SVC_E_ACK_INVALID 			= 0x90,
	SUB_ST_SVC_E_ACK_INVALID_SMS_LEN    = 0xa0,
	SUB_ST_SVC_E_UNKNOWN                 = 0xb0,
	SUB_ST_SVC_E_ACK_INVALID_OVER_SMS_LEN  = 0xc0,
	//SUB_ST_DEL_SVCOFF                     = 0xd0  // Service on/off -> off
} eTzSmsMdbSubStatus;

typedef enum {
	MULTINUM_ST_DEL_CHILDORDER_RANGE_OVER = 0x70,
	MULTINUM_ST_DEL_MTSVCREQ_READY = 0xe0,
	MULTINUM_ST_DEL_FORWARDING_FAIL = 0xf0
} eSmsMdbMultiNUMStatus;

// FR220 ATSI Service Status for 010_PLUS_SVC
typedef enum {
	/* Service_req status */
	ATSI_ST_DEL_SENDQ         = 0x10, // SCHD  -> Queue
	ATSI_ST_DEL_RECVQ         = 0x20, // Queue -> ASE
	ATSI_ST_DEL_REQ_R         = 0x30, // after send
	ATSI_ST_DEL_RESP_SUCCESS  = 0x40, // Ack ok.
	ATSI_ST_DEL_REQ_ERROR     = 0x50, // send fail
	//FR230 pkg
	//ATSI_ST_DEL_RESP_TIMEOUT  = 0x60, // Ack Time out
	ATSI_ST_DEL_SCROUTE_DENY_SKIP  = 0x60,
	ATSI_ST_DEL_RESP_ERROR    = 0x70, // Ack Data Error
	ATSI_ST_DEL_CONNECT_FAIL  = 0x80  // Connection Fail
} eSmsMdbATSISubStatus;

typedef enum {
	SKT_FORWARD_SVC_OFF			= 0x00,
	SKT_FORWARD_SVC_ON,
	SKT_FORWARD_SVC_CNT_OVER,
	SKT_FORWARD_SVC_NEW,
	SKT_FORWARD_SVC_NUM_PLUS_NEW,	//for Nuber Plus FWD
	SKT_FORWARD_SVC_BIZ_NUM,		//for BIZ Nuber Service
	SKT_FORWARD_SVC_MULTI_NUM_NEW,	//DR730 PKG Multi NUM  FWD
	SKT_FORWARD_SVC_NUM_CHANGE,
	SKT_FORWARD_SVC_MCC,
	SKT_FORWARD_SVC_FAIL
} eSKTForwardStatus;

typedef enum {
	SKT_NORMAL_SUBSCRIBER		= 0x00,
	SKT_CDMA_SUBSCRIBER,
	SKT_WCDMA_SUBSCRIBER
} eSKTGenerationType;

typedef enum {
	SKT_PERFECT_SVC_SHARP		= 0x00,
	SKT_PERFECT_SVC_OSFI		= 0x01
} eSKTPerfectSvcStatus;

// FR230 : CalllbackNoti
typedef enum {
	CALLBACK_NOTI_NO_SERVICE					= 0x00, 	// Service OFF
	CALLBACK_NOTI_SVC_READY 					= 0x01, 	// Service
	CALLBACK_NOTI_SVC_RETRY						= 0x02, 	// Retry


	CALLBACK_NOTI_ADD_WORD 						= 0x10,

	CALLBACK_NOTI_READY_NOTI_MESSAGE			= 0x20,
	CALLBACK_NOTI_MAKE_NOTI_MESSAGE				= 0x21,
	CALLBACK_NOTI_SEND_NOTI_MESSAGE	 			= 0x22,



	CALLBACK_NOTI_SVC_FINAL						= 0xF0,
	CALLBACK_NOTI_SVC_FINAL_NO_SERVICE			= 0xF1,
	CALLBACK_NOTI_SVC_FINAL_CALLBACK_NO_CHANGE	= 0xF2,
	CALLBACK_NOTI_SVC_FINAL_SKIP_SERVICE		= 0xF3,
	CALLBACK_NOTI_SVC_FINAL_TID_FILTER			= 0xF4,
	CALLBACK_NOTI_SVC_FINAL_010_PLUS_FILTER		= 0xF5,
	CALLBACK_NOTI_SVC_FINAL_EXIST_APP_PORT		= 0xF6,
	CALLBACK_NOTI_SVC_SKIP_LONG_SEGMENT			= 0xF7,
	CALLBACK_NOTI_SVC_FINAL_MSG_FLOW_FAULT		= 0xF9,
	CALLBACK_NOTI_SVC_FINAL_CHECK_SOURCE_MIN	= 0xFA,
	CALLBACK_NOTI_SVC_FINAL_ORG_INFO_SMSC		= 0xFC,
	CALLBACK_NOTI_SVC_FINAL_SVC_OFF 			= 0xFD

	        //CALLBACK_NOTI_SVC_FINAL					= 0xF8
} eCallbackNotiSvcStatus;

// FR410 IPMG RCS
typedef enum {
	RCS_SVC_BASE = 0x00,
	RCS_SVC_MOF = 0x01,
	RCS_SVC_MTF = 0x02,
	RCS_SVC_REPORT = 0x03,
	RCS_SVC_MOF_REPORT_FINAL = 0x11,
	RCS_SVC_MTF_FINAL = 0x22,
	RCS_SVC_REPORT_FINAL = 0x33,
	RCS_SVC_TID_NOT_SUPPORT = 0x44,
	RCS_SVC_OFF = 0x55
} eRcsSvcStatus;

typedef enum{
	MCS_MO_NORMAL_IDX = 0x01, 	// 01 FIX 				(Submit SM -> suboperation 0)
	MCS_MT_NORMAL_IDX, 			// 02 FIX SCHD -> SSMPP (Submit SM -> suboperation 1)
	MCS_MO_DENY_IDX, 			// 03 FIX  				(Submit SM -> suboperation 2)
	MCS_MT_CXL_IDX, 			// 04 FIX
	MCS_MT_DR_RECV_IDX, 		// 05 SSMPP -> NAG -> SSMPP (SUBMIT SM RESP)
	MCS_MT_DR_FAIL_IDX, 		// 06 NAG -> RSMPP 		 (DELIVER SM)
	MCS_MT_DR_SUCC_IDX, 		// 07 NAG -> RSMPP 		 (DELIVER SM)
	MCS_MT_CDR_READY_IDX, 		// 08 RSMPP -> UHIS
	MCS_MT_SMS_DONE_IDX, 		// 09 sms first done
	MCS_MT_CDR_RCS_IDX, 		// 10 UHIS -> CDR MAKE
	MCS_MT_SMS_SKIP_IDX, 		// 11 sms
	MCS_MGR_DR_RECV_IDX, 		// 12 sms
	MCS_SVC_MAX_IDX
}eMcsTypeIdx;

typedef enum{
	ENUM_MSG_ORI_NONE,
	ENUM_MSG_ORI_WEB,
	ENUM_MSG_ORI_PHONE,
	ENUM_MSG_ORI_INTER,
	ENUM_MSG_ORI_ROAMING
}eMessageOrigination;


#endif
