/******************************************************************************r
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
	FILE NAME		:	MdbTableInfo.h
	PURPOSE			:	table informations on memory db on SMDC
	DATE OF WRITE	: 	1998.04.29
	AUTHOR			: 	Eun-ju SEO
	REVISION		:	V 1.0
	Ver.	By		When		Reason
	------- ------- ----------  -----------------------------------------
	1.0		EJSEO	1998.04.29	first coding
	1.1		EJSEO	1998.05.13	modify due to schema modificaton
***********************************************************************/

#ifndef _MDBTABLEINFO_H_
#define _MDBTABLEINFO_H_

#include <time.h>
#include <sys/types.h>

#include "FLKBaseDef.h"

/*
 * table_id
 */
typedef enum {
	R_DACTIVE,
	R_DACTIVEHEAD,
	R_DRETRY,
	R_DRETRYHEAD,
	R_DRESERVE,
	R_DRESERVEHEAD,
	R_DRETRYFRAME
} eActiveDBTableID_t;

typedef enum {
	R_DCHARSET,
	R_DSPAM,
	R_DESME,
	R_DSERVICE,
	R_DVMIN_PREFIX,
	R_DESMESERVCFG,
	R_DESMESYSTEMID,
	R_DCLIENTINFO,
	R_DSVCIDROUTE,
	R_DNODETYPE,
	R_DTRAFFICCONTROL,
	R_DTEXTCONFIG,
	R_DSERVICECONTROL,
	R_DADMINBAR,
	R_DUDMINFO
} ePlusDBTableID_t;

typedef enum {
	R_DSTATISTICS,
	R_DREALSTATIC,
	R_DPREFIX,
	R_DPREFIX_RN,
	R_DCONVERT_TID,
	R_DTID_ROUTING,
	R_DMSGCTRL,
	R_DPREFIX_SMC,
	R_DUDMBKGRP, // SR510 pkg hlr pool routing
	R_DISDN,
	R_DNP_DB_ROUTING,
	R_DNP_DB_PREFIX,
	R_SPECIAL_DESTMIN,
	R_DNPDB_RN_ROUTE,
	R_CBCHGEXPMIN
} eStaticDBTableID_t;

typedef enum {
	R_DTRANSFORM,  //Same scheme as R_DACTIVE
	R_DTRANSHEAD,
	R_DSUBMIT
} eTransBTableID_t;

typedef enum {
	R_DRCS,
	R_DRCSHEAD,
	R_DRCSRETRY,
	R_DRCSRETRYHEAD,
	R_DRCSRESERVE,
	R_DRCSRESERVEHEAD,
	R_DDIAMETERSMSC,
	R_DINFORATECID,
	R_DSMPPMESSAGE,
	R_DCSCFINFO,
	R_DCSCFGROUPINFO,
	R_DCSCFMONITORINFO,
	R_DUDMGROUPINFO,
	R_DS6CPREFIX,
	R_DSHPREFIX
}eRcsBTablbeID_t;

typedef enum {
	NORMAL  = 0,
	ACQ,
	QOR
} eNPType_t;


#define		SMS_SCHD_PS_NAME		"sms_schdMGR"

/*
 * max record num of each table
 */

/*
 * Major column ids on the R_DActive table
 */
typedef enum Column_ID_Of_R_DActiveTable {
	DB_MESSAGE_ID		= 0x00,
	DB_FORWARD_LINK,
	DB_BACKWARD_LINK,
	DB_RETRY_FLAG,
	DB_SERVICE_TYPE,
	DB_MESSAGE_TYPE,
	DB_TELESEVICEID,
	DB_DEFAULT_MSG_ID,
	DB_ESME_SERVICE_TYPE,
	DB_MSG_FLOW_TYPE,		//	10
	DB_SOURCE_TON,
	DB_SOURCE_NPI,
	DB_SOURCE_MIN,
	DB_SOURCE_LEN,
	DB_SOURCE_MSMIN,
	DB_SOURCE_MSLEN,
	DB_ORG_SOURCE_MIN,
	DB_ORG_SOURCE_LEN,
	DB_DEST_TON,
	DB_DEST_NPI,
	DB_DEST_MIN,
	DB_DEST_LEN,			//	20
	DB_DEST_MSMIN,
	DB_DEST_MSLEN,
	DB_CALLBACK_TON,
	DB_CALLBACK_NPI,
	DB_CALLBACK_LEN,
	DB_CALLBACK_TYPE,
	DB_CALLBACK_NUMBER,
	DB_SUBMISSION_TIME,
	DB_RESERVATION_TIME,
	DB_SCHEDULED_TIME,		//	30
	DB_EXPIRATION_TIME,
	DB_DELIVERY_TIME,
	DB_INVOKE_TIME,
	DB_RECEIPT_TIME,
	DB_FINISHED_TIME,
	DB_PRIVACY_FLAG,
	DB_LANGUAGE_FLAG,
#ifdef	DB_ESN
#undef	DB_ESN
#endif
	DB_DEST_ESN,
	DB_PROTOCOL_ID,
	DB_APPLICATION_PORT_TYPE,	//	40
	DB_APPLICATION_PORT,
	DB_ORG_APPLICATION_PORT,
	DB_DATA_CODING_SCHEME,
	DB_INPUTDCS, 				//FR120 add in/put dcs
	DB_OUTPUTDCS,
	DB_PRIORITY_FLAG,
	DB_ESM_CLASS,
	DB_REGISTERED_DELIVERY_FLAG,
	DB_MS_DAK_REQ,
	DB_MS_USER_ACK_REQ,
	//DB_DATA_ENCODING,			// DR720 pkg by cbh : remove
	DB_ALERT_ON_DELVERY,		//	50
	DB_CHARGE_ID,
	DB_NETWORK_VER,
	DB_STATUSREPORT_REQUEST,
	DB_MESSAGE_NUMBER,
	DB_COMMAND_TYPE,
	DB_MESSAGE_REFERENCE,
	DB_RSP_M2M_FLAG,
	DB_ORG_DCS,
	DB_MSG_WAIT_DATA,
	DB_TIME_ZONE,				//	60
	DB_REPLY_PATH,
	DB_SMRP_PRI,
	DB_PROVIDER,
	DB_TP_STATUS,
	DB_MORE_MSG,
	DB_NETWORK_TYPE,
	DB_STATUS_REPORT_QUALIFIER,
	DB_REJECTDUP,
	DB_VALIDITYFORMAT,
	DB_USERDATAHEADERINDI,		//	70
#ifdef	DB_STATUS
#undef	DB_STATUS
#endif
	DB_STATUS,
	DB_PRESTATUS,
	DB_FAIL_REASON,
	DB_RETRY_FRAME,
	DB_ERROR_CODE,
	DB_RETRY_COUNT,
	DB_RESERVATION_FLAG,
	DB_UPDATE_FLAG,
	DB_MASK_STATUS,
	DB_VMN_MSG_COUNT,			//	80
	DB_HLR_PC,
	DB_HLR_PC_FAIL_REASON,
	DB_HLR_PC_DIAG,
	DB_MO_MSC_PC,
	DB_MT_PC_NETTYPE,
	DB_MT_M_PC,
	DB_MT_M_PC_FAILREASON,
	DB_MTMPC_DIAG,
	DB_MT_G_PC,
	DB_MT_G_PC_FAILREASON,		//	90
	DB_MTGPC_DIAG,
	DB_M_PC_ADD_TYPE,
	DB_G_PC_ADD_TYPE,
	DB_MT_MME_PC, 				// SR220
	DB_MT_MME_FAILREASON, 			// SR220
	DB_MT_MME_ISDN,				// SR220
	DB_MO_PC,
	DB_MO_ISDN,				// SR220
	DB_MT_SMSF_PC, 				// SR610
	DB_MT_SMSF_FAILREASON, 		// SR610
	DB_MT_SMSF_ISDN,			// SR610
	DB_MT_SMSC_PC, 				// SR720
	DB_MT_SMSC_FAILREASON, 		// SR720
	DB_MT_SMSC_ISDN,			// SR720
	DB_GSM_NET_TYPE, 				// SR220
	DB_SSMDP_DELIVERY_STATUS,
	DB_INPUT_IP,
	DB_INPUT_PORT,
	DB_OUTPUT_IP,
	DB_OUTPUT_PORT,
	DB_ORG_TOT_LENGTH,
	DB_LONG_IDENTIFIER,
	DB_CONCATE_REF_TYPE,
	DB_LONG_REF_NUMBER,
	DB_LONG_TOT_SEGMENT,
	DB_LONG_CUR_SEGMENT,
	DB_SESSION_INFO,
	DB_SESSION_VALUE,
	DB_RESPONSE_CODE,
	/**************************************************************/

	/**************************************************************
					/// Added 2003.07.08 ///
					///   For Smpp34     ///
	**************************************************************/
	DB_ORG_SRC_MIN_TYPE,
	DB_ESME_ID,
	DB_ORG_DST_MIN_TYPE,
	DB_OUTPUT_ESME_ID,			//	110
	DB_ESME_SEQ_ID,
	DB_OUTPUT_RCS_ID,
	DB_DESTROUT_TON,
	DB_DESTROUTE_NPI,
	DB_DESTROUTE_MIN,
	DB_DESTROUTE_LEN,
	DB_DESTREDIRECT_TON,
	DB_DESTREDIRECT_NPI,
	DB_DESTREDIRECT_MIN,
	DB_DESTREDIRECT_LEN,
	DB_UDHI_FLAG,		//	120
	DB_REDIRECT_USE,
	DB_ACT_NPDB_IP,
	DB_ACT_NPDB_PORT,
	DB_DO_NOT_FORWARD,
	DB_PORTED,
	DB_ALREADY_SEND_NPDB,
	DB_ORG_IMSI,
	DB_ORG_IMSI_LEN,
	DB_DEST_IMSI,
	DB_DEST_IMSI_LEN,
	DB_LMSI,					//	130
	DB_ORG_ESN,
	DB_MSMSGID,
	DB_ENDOFSESSION,
	DB_INTERACTIVEFLAG,
	DB_ROAMFLAG,
	DB_SPAM_FLAGS,
	DB_DESTMS_CAPABILITY,
	DB_DESTMS_CAPABILITY_FLAG,
	DB_OSFI,
	DB_OSFI_FLAG,				//	140
	DB_SMS_OSFI,
	DB_SMS_OSFI_FLAG,
	DB_SMS_OSFI_RECV,
	DB_SC_DENY_FLAG,
	DB_SPAM_SVC_TIME,
	DB_ICS_SVC_TIME,
	DB_PICS_SVC_TIME,
	DB_QUERY_SVC_TIME,
	DB_PSMS_SVC_TIME,
	DB_SCSMS_SSVC_TIME,
	DB_SNAP_SVC_TIME,
	DB_ASMSS_SVC_TIME,
	DB_DR_TO_MS_TIME,			//	150
	DB_IMSMS_SVC_TIME,
	DB_MGR_OSFI_SVC_TIME,
	DB_MGR_CAPA_SVC_TIME,
	DB_PATRIOT_CAPA_SVC_TIME,
	DB_ATSI_REQ_SVC_TIME, 		//FR220 CYSMS to 010_PLUS_SVC
	DB_PERFECT_CAPA_SVC_TIME,
	DB_NEW_SPAM_SVC_TIME,
	DB_CALLBACK_NOTI_SVC_TIME,
	DB_RCS_SVC_TIME,
	DB_MT_MULTI_NUM_SVC_TIME,
	DB_MCS_MO_TIME,
	DB_MCS_MT_TIME,
	DB_MCS_CXL_TIME,
	DB_ATSI_SUPPLY_TIME,
	DB_SPAM_SVC_STATUS,
	DB_ICS_SVC_STATUS,
	DB_PICS_SVC_STATUS,			//	160
	DB_PICS_IMMET_SVC_STATUS,
	DB_QUERY_SVC_STATUS,
	DB_PSMS_SVC_STATUS,
	DB_SCSMSS_SVC_STATUS,
	DB_SNAP_SVC_STATUS,
	DB_ASMSS_SVC_STATUS,
	DB_DR_TO_MS_STATUS,
	DB_IMSMS_SVC_STATUS,
	DB_MGR_OSFI_SVC_STATUS,
	DB_MGR_CAPA_SVC_STATUS,		//	170
	DB_MGR_IMMET_SVC_STATUS,
	DB_PATRIOT_CAPA_SVC_STATUS,
	DB_ATSI_REQ_SVC_STATUS,  	//FR220 CYSMS to 010_PLUS_SVC
	DB_PERFECT_CAPA_SVC_STATUS,
	DB_NEW_SPAM_SVC_STATUS,
	DB_CALLBACK_NOTI_SVC_STATUS,
	DB_RCS_SVC_STATUS,
	DB_MT_MULTI_NUM_SVC_STATUS,
	DB_MCS_MO_TYPE,
	DB_MCS_MO_STATUS,
	DB_MCS_MT_SMS_TYPE,
	DB_MCS_MT_TYPE,
	DB_MCS_MT_STATUS,
	DB_MCS_MT_RCT,
	DB_MCS_MT_STAT,
	DB_MCS_CXL_STATUS,
	DB_ATSI_SUPPLY_STATUS,
	DB_ROAMIND,
	DB_SERVICE_PREFIX,
	DB_GENERATION_TYPE,
	DB_DESTMS_CAPABILITY_RECV,
	DB_DESTMS_CAPABILITY_RECV2,
	DB_OSFI_RECV,				//	180
	DB_MSC_ID,
	DB_LOCAL_AREA_ID,
	DB_ROAMING_PMN,
	DB_NODE_STATUS,
	DB_ACTIVE_NODE_TYPE,
	DB_RN_PREFIX,
	DB_STAT_SENDTIME,
	DB_MT_MS_TYPE,
	DB_MSG_TRANSFORM_TYPE,
	DB_REG_INFO_COUNT,				// FR210 Noti Count  for LMS_RELAY
	DB_ADMIN_BAR_STATUS,
	DB_OSFI2, 				//FR220
	DB_OSFI_FLAG2,		 	//FR220
	DB_OSFI_RECV2,			//FR220
	DB_CALLBACK_NOTI_FLAG,
	DB_CALLBACK_NOTI_NWCHG_FLAG,
	DB_CALLBACK_NOTI_ORIGINATION,
	DB_CALLBACK_NOTI_APPLY_FLAG,
	DB_CALLBACK_NOTI_LINK,
	DB_ICONV_EXT_LEN,
//	DB_SMS_MO_SAC, //FR401 PKG
	DB_LTE_MT_INFO,
	DB_IPMG_RCS_RESULT,
	DB_IPMG_RCS_REF_ID,
	DB_IPMG_MESSAGE_ID,
	DB_EPE_RECV_TIME,
	DB_CDR_FLAG,
	DB_ATSI_TYPE,
	DB_ATSI_SUPPLY_TYPE,
	DB_SECURITY_FLAG,			// DR710 pkg
	DB_MSG_CUT_POINT,			// DR720
	DB_USER_DATA_LEN, 			// DR720 pkg by cbh : add user data length
	DB_ORG_REF_NUMBER,			// DR720 pkg by cbh : save org refnum, tot_seg, cur_seg
	DB_ORG_TOT_SEGMENT,			// DR720 pkg by cbh
	DB_ORG_CUR_SEGMENT,			// DR720 pkg by cbh
	DB_MO_MULTI_NUM_ORDER,
	DB_MT_MULTI_NUM_ORDER,
	DB_AUTH_FLAG, 		//FR620 pkg
	DB_FIRST_STATUS,	//FR620 pkg
	DB_FIRST_DELIVERY,	//FR620 pkg
	DB_SECOND_STATUS,	//FR620 pkg
	DB_SECOND_DELIVERY,	//FR620 pkg 
	DB_SMPP_MESSAGE_ID,
	
	DB_PROFILE_FLAG,
	DB_IMS_PRIORITY,
	DB_IMS_MT_RP_CAUSE,
	DB_IMS_MT_TP_CAUSE,
	DB_IMS_MT_SIP_ERROR,
	DB_MT_CSCF_NUM,
	DB_CALL_ID,

	DB_ACTIVE_RESERVED,
	DB_MESSAGE_LENGTH,			//	190
	DB_MESSAGE_CONTENT,
	ACTIVE_TBL_ATTR_COUNT		// Active Tuple's Field Count.
} eColumn_ID_Of_R_DActiveTable_t;

typedef enum {
	MDB_FLAG_SIZE		= 1,
	MDB_LEN_SIZE		= MDB_FLAG_SIZE,
	MDB_HALF_NUM_SIZE	= 2,
	MDB_NUMBER_SIZE		= 4,
	MDB_TIME_TYPE_SIZE	= 4,
	MDB_MSMIN_SIZE		= 5,	// MIN Size
	MDB_TIME_SIZE		= 7,
	MDB_MIN_SIZE		= 12	// MDN Size
} eMDBSizeTemplet_t;


typedef enum {
	MSG_TYPE_NORMAL,
	MSG_TYPE_INTERACTIVE
} eInteractiveFlagTemplet_t;
/*
 * The attribute sizes of R_DActive table
 */

#define	DB_MESSAGE_ID_SIZE					MDB_NUMBER_SIZE
#define DB_FORWARD_LINK_SIZE				MDB_NUMBER_SIZE
#define DB_BACKWARD_LINK_SIZE				MDB_NUMBER_SIZE
#define	DB_RETRY_FLAG_SIZE					12

#define	DB_SERVICE_TYPE_SIZE				6
#define	DB_MESSAGE_TYPE_SIZE				MDB_HALF_NUM_SIZE
#define	DB_TELESERVICEID_SIZE				MDB_HALF_NUM_SIZE
#define	DB_DEFAULT_MSG_ID_SIZE				MDB_NUMBER_SIZE
#define	DB_ESME_SERVICE_TYPE_SIZE			MDB_FLAG_SIZE
#define DB_MSG_FLOW_TYPE_SIZE				MDB_FLAG_SIZE // Only Dual

#define	DB_SOURCE_TON_SIZE					MDB_FLAG_SIZE
#define	DB_SOURCE_NPI_SIZE					MDB_FLAG_SIZE
#define	DB_SOURCE_MIN_SIZE					MDB_MIN_SIZE
#define	DB_SOURCE_LEN_SIZE					MDB_LEN_SIZE

#define	DB_DEST_TON_SIZE					MDB_FLAG_SIZE
#define	DB_DEST_NPI_SIZE					MDB_FLAG_SIZE
#define	DB_DEST_MIN_SIZE					MDB_MIN_SIZE
#define	DB_DEST_LEN_SIZE					MDB_LEN_SIZE

#define DB_CALLBACK_TON_SIZE				MDB_FLAG_SIZE // Only Dual
#define DB_CALLBACK_NPI_SIZE				MDB_FLAG_SIZE // Only Dual
#define DB_CALLBACK_LEN_SIZE				MDB_LEN_SIZE  // Only Dual
#define DB_CALLBACK_TYPE_SIZE				MDB_FLAG_SIZE
#define	DB_CALLBACK_NUMBER_SIZE				MDB_MIN_SIZE
#define	DB_CALLBACK_MAX_SIZE				10

#define	DB_SUBMISSION_TIME_SIZE				MDB_TIME_SIZE
#define	DB_RESERVATION_TIME_SIZE			MDB_TIME_SIZE
#define	DB_SCHEDULED_TIME_SIZE				MDB_TIME_SIZE
#define	DB_EXPIRATION_TIME_SIZE				MDB_TIME_SIZE
#define	DB_DELIVERY_TIME_SIZE				MDB_TIME_SIZE
#define	DB_INVOKE_TIME_SIZE					MDB_TIME_SIZE
#define	DB_RECEIPT_TIME_SIZE				MDB_TIME_SIZE
#define	DB_FINISHED_TIME_SIZE				MDB_TIME_SIZE


#define DB_PRIVACY_FLAG_SIZE				MDB_FLAG_SIZE
#define DB_LANGUAGE_FLAG_SIZE				MDB_FLAG_SIZE
#define DB_ESN_SIZE							MDB_NUMBER_SIZE

#define	DB_PROTOCOL_ID_SIZE					MDB_FLAG_SIZE
#define	DB_APPLICATION_PORT_TYPE_SIZE		MDB_FLAG_SIZE
#define	DB_APPLICATION_PORT_SIZE			MDB_HALF_NUM_SIZE
#define	DB_DATA_CODING_SCHEME_SIZE			MDB_FLAG_SIZE
#define	DB_PRIORITY_FLAG_SIZE				MDB_FLAG_SIZE
#define	DB_ESM_CLASS_SIZE					MDB_FLAG_SIZE
#define	DB_REGISTERED_DELIVERY_FLAG_SIZE	MDB_FLAG_SIZE

#define	DB_MS_DAK_REQ_SIZE					MDB_FLAG_SIZE
#define DB_MS_USER_ACK_REQ_SIZE				MDB_FLAG_SIZE
#define	DB_DATA_ENCODING_SIZE				MDB_FLAG_SIZE
#define DB_ALERT_ON_DELVERY_SIZE			MDB_FLAG_SIZE

#define DB_CHARGE_ID_SIZE					MDB_FLAG_SIZE

//	GSM Dependancy
#define DB_NETWORK_FLOWTYPE_SIZE            MDB_FLAG_SIZE
#define DB_STATUSREPORT_REQUEST_SIZE        MDB_FLAG_SIZE
#define DB_MESSAGE_NUMBER_SIZE              MDB_FLAG_SIZE
#define DB_COMMAND_TYPE_SIZE                MDB_FLAG_SIZE
#define DB_MESSAGE_REF_SIZE                 MDB_FLAG_SIZE
#define DB_RSP_M2M_FLAG_SIZE            MDB_FLAG_SIZE
#define DB_ORG_DCS_SIZE            MDB_FLAG_SIZE
#define DB_MSG_WAIT_DATA_SIZE               MDB_FLAG_SIZE
#define	DB_TIME_ZONE_SIZE					1
#define DB_REPLY_PATH_SIZE                  1
#define DB_SMRP_PRI_SIZE                    1
#define DB_PROVIDER_SIZE                    MDB_NUMBER_SIZE
#define DB_TP_STATUS_SIZE                   MDB_FLAG_SIZE
#define DB_MORE_MSG_SIZE                    MDB_FLAG_SIZE
#define DB_NETWORK_TYPE_SIZE                MDB_FLAG_SIZE
#define DB_STATUS_REPORT_QUALIFIER_SIZE     MDB_FLAG_SIZE
#define DB_REJECTDUP_SIZE                   MDB_FLAG_SIZE
#define DB_VALIDITYFORMAT_SIZE              MDB_FLAG_SIZE
#define DB_USERDATAHEADERINDI_SIZE          MDB_FLAG_SIZE

#define	DB_STATUS_SIZE						MDB_HALF_NUM_SIZE
#define	DB_FAIL_REASON_SIZE					MDB_HALF_NUM_SIZE
#define	DB_RETRY_FRAME_SIZE					MDB_FLAG_SIZE
#define	DB_ERROR_CODE_SIZE					MDB_HALF_NUM_SIZE
#define	DB_RETRY_COUNT_SIZE					MDB_HALF_NUM_SIZE
#define	DB_RESERVATION_FLAG_SIZE			MDB_FLAG_SIZE

#define DB_UPDATE_FLAG_SIZE					MDB_FLAG_SIZE
#define DB_MASK_STATUS_SIZE					MDB_FLAG_SIZE

#define DB_VMN_MSG_COUNT_SIZE				MDB_LEN_SIZE

/******************************* Only Dual **********************/
#define DB_HLR_PC_SIZE						MDB_NUMBER_SIZE
#define DB_HLR_PC_FAIL_REASON_SIZE         MDB_FLAG_SIZE
#define DB_HLRPC_DIAG_SIZE                  MDB_FLAG_SIZE
#define DB_MO_MSC_SIZE						MDB_NUMBER_SIZE

//	GSM Dependancy
#define DB_NETWORK_VER_SIZE                 MDB_FLAG_SIZE
#define DB_MT_PC_NETTYPE_SIZE               MDB_FLAG_SIZE
#define DB_MT_M_PC_SIZE                     4
#define DB_MT_M_PC_FAIL_REASON_SIZE         MDB_FLAG_SIZE
#define DB_MTMPC_DIAG_SIZE                  MDB_FLAG_SIZE
#define DB_MT_G_PC_SIZE                     4
#define DB_MT_G_PC_FAIL_REASON_SIZE         MDB_FLAG_SIZE
#define DB_MTGPC_DIAG_SIZE                  MDB_FLAG_SIZE
#define DB_M_PC_ADD_TYPE_SIZE               MDB_FLAG_SIZE
#define DB_G_PC_ADD_TYPE_SIZE               MDB_FLAG_SIZE

#define DB_MME_NAME_SIZE 					64  			// SR220
#define DB_MT_MME_NAME_SIZE 				DB_MME_NAME_SIZE 			// SR220
#define DB_MO_MME_NAME_SIZE 				DB_MME_NAME_SIZE 			// SR220
#define DB_MME_ISDN_SIZE        			8
#define	DB_GSM_NET_TYPE_SIZE				MDB_FLAG_SIZE

#define DB_SSMDP_DELIVERY_STATUS_SIZE		MDB_FLAG_SIZE

#define DB_INPUT_IP_SIZE					MDB_NUMBER_SIZE
#define DB_INPUT_PORT_SIZE					MDB_HALF_NUM_SIZE
#define DB_OUTPUT_IP_SIZE					MDB_NUMBER_SIZE
#define DB_OUTPUT_PORT_SIZE					MDB_HALF_NUM_SIZE

#define DB_EMS_SAR_REF_SIZE                 MDB_HALF_NUM_SIZE
#define DB_EMS_SAR_NUM_SIZE                 MDB_LEN_SIZE

#define DB_ORG_TOT_LENGTH_SIZE 				MDB_HALF_NUM_SIZE
#define DB_LONG_IDENTIFIER_SIZE 			MDB_NUMBER_SIZE
#define DB_CONCATE_REF_TYPE_SIZE			MDB_LEN_SIZE
#define DB_LONG_REF_NUMBER_SIZE				MDB_HALF_NUM_SIZE

#define	DB_LONG_TOT_SEGMENT_SIZE			MDB_LEN_SIZE
#define	DB_LONG_CUR_SEGMENT_SIZE			MDB_LEN_SIZE


#define DB_SESSION_INFO_SIZE                MDB_HALF_NUM_SIZE
#define DB_SESSION_VALUE_SIZE               MDB_HALF_NUM_SIZE

#define DB_RESPONSE_CODE_SIZE				MDB_FLAG_SIZE

/***************************************************************/

#define DB_ORG_SRC_MIN_TYPE_SIZE				MDB_FLAG_SIZE
#define DB_ESME_ID_SIZE						MDB_NUMBER_SIZE
#define DB_ESMESEQ_ID_SIZE					MDB_NUMBER_SIZE

/*
#define DB_SOURCROUTE_TON_SIZE				MDB_FLAG_SIZE
#define DB_SOURCROUTE_NPI_SIZE				MDB_FLAG_SIZE
#define DB_SOURCROUTE_MIN_SIZE				MDB_MIN_SIZE
#define DB_SOURCROUTE_LEN_SIZE				MDB_LEN_SIZE
*/

#define DB_DESTROUTE_TON_SIZE				MDB_FLAG_SIZE
#define DB_DESTROUTE_NPI_SIZE  				MDB_FLAG_SIZE
#define DB_DESTROUTE_MIN_SIZE				MDB_MIN_SIZE
#define DB_DESTROUTE_LEN_SIZE				MDB_LEN_SIZE

/*
#define DB_SOURCEREDIRECT_TON_SIZE			MDB_FLAG_SIZE
#define DB_SOURCEREDIRECT_NPI_SIZE			MDB_FLAG_SIZE
#define DB_SOURCEREDIRECT_MIN_SIZE			MDB_MIN_SIZE
#define DB_SOURCEREDIRECT_LEN_SIZE			MDB_LEN_SIZE
*/

#define DB_DESTREDIRECT_TON_SIZE			MDB_FLAG_SIZE
#define DB_DESTREDIRECT_NPI_SIZE  			MDB_FLAG_SIZE
#define DB_DESTREDIRECT_MIN_SIZE			MDB_MIN_SIZE
#define DB_DESTREDIRECT_LEN_SIZE			MDB_LEN_SIZE

#define	DB_UDHI_FLAG_SIZE			MDB_LEN_SIZE
#define	DB_REDIRECT_USE_SIZE				MDB_LEN_SIZE

#define DB_NPDB_IP_SIZE						MDB_NUMBER_SIZE
#define DB_NPDB_PORT_SIZE					MDB_HALF_NUM_SIZE

#define DB_DO_NOT_FORWARD_SIZE 				MDB_LEN_SIZE
#define	DB_PORTED_SIZE						MDB_FLAG_SIZE
#define DB_ALREADY_NPDB_SIZE				MDB_FLAG_SIZE
#define	DB_IMSI_SIZE						MDB_MIN_SIZE
#define	DB_IMSI_LEN_SIZE					MDB_LEN_SIZE
#define DB_LMSI_SIZE                        MDB_NUMBER_SIZE

#define	DB_MSMSGID_SIZE						MDB_HALF_NUM_SIZE
#define DB_ENDOFSESSION_SIZE				MDB_FLAG_SIZE
#define	DB_INTERACTIVEFLAG_SIZE				MDB_FLAG_SIZE

#define DB_ROAMFLAG_SIZE					MDB_FLAG_SIZE
#define MDB_ROAMIND_SIZE					MDB_FLAG_SIZE

#define DB_SPAM_FLAG_SIZE                   MDB_FLAG_SIZE

// Add for Coloer&LONG SM
#define DB_DEST_MS_CAPABILITY_SIZE          MDB_NUMBER_SIZE

#define	DB_OSFI_SIZE						MDB_NUMBER_SIZE
#define	DB_OSFI_FLAG_SIZE					MDB_NUMBER_SIZE

//FR220 OSFI_SIZE_UP
#define DB_OSFI_MAX_SIZE                    8
#define DB_SMS_OSFI_MAX_SIZE                5


#define	DB_SPAM_SVC_STATUS_SIZE				MDB_FLAG_SIZE
#define	DB_ICS_SVC_STATUS_SIZE				MDB_FLAG_SIZE
#define	DB_PICS_SVC_STATUS_SIZE				MDB_FLAG_SIZE
#define	DB_QUERY_SVC_STATUS_SIZE			MDB_FLAG_SIZE
#define	DB_PSMS_SVC_STATUS_SIZE				MDB_FLAG_SIZE
#define	DB_TZSMS_SVC_STATUS_SIZE			MDB_FLAG_SIZE
#define	DB_SNAP_SVC_STATUS_SIZE				MDB_FLAG_SIZE
#define	DB_ASMSS_SVC_STATUS_SIZE			MDB_FLAG_SIZE

#define DB_SKT_SERVICE_PREFIX_SIZE			5

#define	DB_GENERATION_TYPE_SIZE				1

#define	DB_DEST_MS_CAPABILITY_RECV_SIZE		2
#define	DB_OSFI_RECV_SIZE					2

#define	DB_MSC_ID_SIZE						3
#define	DB_LOCATION_AREA_ID_SIZE			2
#define	DB_ROAMING_PMN_SIZE					4

#define DB_NODE_STATUS_SIZE					1
#define	DB_NODE_TYPE_SIZE					1

#define	DB_STAT_SENDTIME_SIZE				4

#define DB_REG_INFO_COUNT_SIZE 				2

#define DB_MT_MS_TYPE_SIZE					1
#define DB_MSG_TRANSFORM_CNT_SIZE			2

#define DB_ADMIN_BAR_SIZE                   MDB_FLAG_SIZE


// FR230 PKG For Callback Modify Noti
#define DB_CALLBACK_NOTI_FLAG_SIZE              MDB_FLAG_SIZE
#define DB_CALLBACK_NOTI_NWCHG_FLAG_SIZE      MDB_FLAG_SIZE
#define DB_CALLBACK_NOTI_ORIGINATION_SIZE       MDB_FLAG_SIZE
#define DB_CALLBACK_NOTI_APPLY_FLAG__SIZE       MDB_FLAG_SIZE
#define DB_LTE_MT_INFO_SIZE 1 //DR610 LSH 



//#define DB_SMS_MO_SAC_SIZE 2 // FR401 LSH

#define	DB_RESERVED_SIZE				    2 // VR100 PKG 2
#define DB_ICONV_EXT_LEN_SIZE                   1   //FR310 PKG 

#define DB_MESSAGE_LENGTH_SIZE				MDB_LEN_SIZE
//#define DB_MESSAGE_CONTENT_SIZE				161
#define DB_MESSAGE_CONTENT_SIZE				181 /* FR220 ontents Resize... */

#define DB_IPMG_RCS_RESULT_SIZE 2
#define DB_IPMG_RCS_REF_ID_SIZE 14

#define DB_SMPP_MESSAGE_KEY_SIZE 			36
#define DB_SMPP_MESSAGE_ID_SIZE 			12
/******************************************************************/
/*
 * Message Type
 */
typedef enum {
	NORMAL_TYPE,
	INT_TYPE,
	EXPRESS_TYPE,
	EMERGENCY
} eMsgType_t;

/*
 * updateflag
 */
typedef enum {
	NO_RELACE_NO_UPDATE	= 0x00,
	NO_REPLACE_UPDATE	= 0x01, // not used
	REPLACE_WAIT_UPDATE	= 0x10,
	REPLACE_UPDATE		= 0x11
} eUpdateFlag_t;

/*
 * The tuple structure of R_DActive table
 */
typedef struct {
	u_char	dbMessageID[DB_MESSAGE_ID_SIZE]; // 4
	u_char	dbForwardLink[DB_FORWARD_LINK_SIZE]; // 4
	u_char	dbBackwardLink[DB_BACKWARD_LINK_SIZE]; // 4
	u_char	dbRetryFlag[DB_RETRY_FLAG_SIZE];   // 12

	u_char	dbServiceType[DB_SERVICE_TYPE_SIZE]; // 6
	u_char	dbMessageType[DB_MESSAGE_TYPE_SIZE]; // 2
	u_char	dbTeleserviceID[DB_TELESERVICEID_SIZE]; // 2
	u_char	dbDefMsgID[DB_DEFAULT_MSG_ID_SIZE]; // 4
	u_char	dbEsmeServiceType[DB_ESME_SERVICE_TYPE_SIZE]; // 1
	u_char	dbMsgFlowType[DB_MSG_FLOW_TYPE_SIZE]; // Only Dual. size 1

	u_char	dbSourceTON[DB_SOURCE_TON_SIZE]; // 1
	u_char	dbSourceNPI[DB_SOURCE_NPI_SIZE]; // 1
	u_char	dbSourceMIN[DB_SOURCE_MIN_SIZE]; // 12  src MDN
	u_char	dbSourceLen[DB_SOURCE_LEN_SIZE]; // 1
	u_char	dbSourceMSMIN[MDB_MSMIN_SIZE]; 	 // 5   src MIN
	u_char	dbSourceMSLen[DB_SOURCE_LEN_SIZE]; // 1
	u_char  dbOrgSourceMIN[DB_SOURCE_MIN_SIZE];
	u_char  dbOrgSourceLen[DB_SOURCE_LEN_SIZE];

	u_char	dbDestTON[DB_DEST_TON_SIZE]; // 1
	u_char	dbDestNPI[DB_DEST_NPI_SIZE]; // 1
	u_char	dbDestMIN[DB_DEST_MIN_SIZE]; // 12	dest MDN
	u_char	dbDestLen[DB_DEST_LEN_SIZE]; // 1
	u_char	dbDestMSMIN[MDB_MSMIN_SIZE]; // 5	dest MIN
	u_char	dbDestMSLen[DB_DEST_LEN_SIZE]; // 1

	u_char	dbCallbackTON[DB_CALLBACK_TON_SIZE]; // Only Dual. size 1
	u_char	dbCallbackNPI[DB_CALLBACK_NPI_SIZE]; // Only Dual. size 1
	u_char	dbCallbackLen[DB_CALLBACK_LEN_SIZE];// Only Dual. size 1
	u_char	dbCallbackType[DB_CALLBACK_TYPE_SIZE]; // 1
	u_char	dbCallbackNumber[DB_CALLBACK_NUMBER_SIZE]; // 12

	u_char	dbSubmissionTime[DB_SUBMISSION_TIME_SIZE]; // 7
	u_char	dbReservationTime[DB_RESERVATION_TIME_SIZE]; // 7
	u_char	dbScheduledTime[DB_SCHEDULED_TIME_SIZE];   // 7
	u_char	dbExpirationTime[DB_EXPIRATION_TIME_SIZE]; // 7
	u_char	dbDeliveryTime[DB_DELIVERY_TIME_SIZE];     // 7
	u_char	dbInvokeTime[DB_INVOKE_TIME_SIZE];         // 7
	u_char	dbReceiptTime[DB_RECEIPT_TIME_SIZE];		// 7 for receipt time
	u_char	dbFinishedTime[DB_FINISHED_TIME_SIZE];     // 7

	u_char	dbPrivacyFlag[DB_PRIVACY_FLAG_SIZE];   // 1
	u_char	dbLanguageFlag[DB_LANGUAGE_FLAG_SIZE]; // 1
	u_char	dbDestEsn[DB_ESN_SIZE];                // 4

	u_char	dbProtocolID[DB_PROTOCOL_ID_SIZE];               // 1
	u_char	dbApplicationPortType[DB_APPLICATION_PORT_TYPE_SIZE];	// 1
	u_char	dbApplicationPort[DB_APPLICATION_PORT_SIZE];	 // 2
	u_char	dbOrigApplicationPort[DB_APPLICATION_PORT_SIZE];	 // 2
	u_char	dbDataCodingScheme[DB_DATA_CODING_SCHEME_SIZE];  // 1
	/* FR120 by Chan : Bytes Alignment Reserved Field 3Bytes->1Bytes... */
	u_char  dbInputDCS[DB_DATA_ENCODING_SIZE];				// 1
	u_char  dbOutputDCS[DB_DATA_ENCODING_SIZE];				// 1
	///////////////////////////////////////////////////////////////////

	u_char	dbPriorityFlag[DB_PRIORITY_FLAG_SIZE];           // 1
	u_char	dbEsmClass[DB_ESM_CLASS_SIZE];                   // 1

	u_char	dbRegisteredDeliveryFlag[DB_REGISTERED_DELIVERY_FLAG_SIZE]; // 1

	u_char	dbMsDakReq[DB_MS_DAK_REQ_SIZE]; // 1
	u_char	dbMsUserAckReq[DB_MS_USER_ACK_REQ_SIZE]; // 1

	// DR720 pkg by cbh : remove
	//u_char	dbDataEncoding[DB_DATA_ENCODING_SIZE]; // 1         168
	u_char	dbAlertOnDelivery[DB_ALERT_ON_DELVERY_SIZE]; // 1

	u_char	dbChargeID[DB_CHARGE_ID_SIZE]; // 1

	// GSM Dependancy
	u_char	dbNetworkVer[DB_NETWORK_VER_SIZE];           //1
	u_char  dbStatusReportRequest[DB_STATUSREPORT_REQUEST_SIZE]; //1
	u_char  dbMessageNumber[DB_MESSAGE_NUMBER_SIZE];    //1
	u_char  dbCommandType[DB_COMMAND_TYPE_SIZE];        //1
	u_char  dbMessageReference[DB_MESSAGE_REF_SIZE];    //1
	u_char  dbRspM2mFlag[DB_RSP_M2M_FLAG_SIZE]; // 1
	u_char  dbOrgDcs[DB_ORG_DCS_SIZE]; //1
	u_char  dbMsgWaitData[DB_MSG_WAIT_DATA_SIZE];		// 1
	u_char	dbTimeZone[DB_TIME_ZONE_SIZE]; // 1
	u_char  dbReplyPath[DB_REPLY_PATH_SIZE]; // 1
	u_char  dbsmRPPri[DB_SMRP_PRI_SIZE];     // 1

	u_char  dbProvider[DB_PROVIDER_SIZE];    // 4
	u_char  dbTPstatus[DB_TP_STATUS_SIZE]; //1
	u_char  dbMoreMsg[DB_MORE_MSG_SIZE]; // 1
	u_char  dbNetworkType[DB_NETWORK_TYPE_SIZE]; // 1
	u_char  dbStatusReportQualifier[DB_STATUS_REPORT_QUALIFIER_SIZE]; // 1
	u_char  dbRejectDup[DB_REJECTDUP_SIZE]; // 1
	u_char  dbValidityFormat[DB_VALIDITYFORMAT_SIZE]; // 1
	u_char  dbUserDataHeaderIndi[DB_USERDATAHEADERINDI_SIZE]; // 1

	u_char	dbStatus[DB_STATUS_SIZE]; // 2
	u_char	dbPreStatus[DB_STATUS_SIZE]; // 2
	u_char	dbFailReason[DB_FAIL_REASON_SIZE]; // 2
	u_char	dbRetryFrame[DB_RETRY_FRAME_SIZE]; // 1
	u_char	dbErrorCode[DB_ERROR_CODE_SIZE];   // 2
	u_char	dbRetryCount[DB_RETRY_COUNT_SIZE]; // 2
	u_char	dbReservationFlag[DB_RESERVATION_FLAG_SIZE]; // 1

	u_char	dbUpdateFlag[DB_UPDATE_FLAG_SIZE]; // 1
	u_char	dbMaskStatus[DB_MASK_STATUS_SIZE]; // 1

	u_char	dbVmnMsgCount[DB_VMN_MSG_COUNT_SIZE]; // 1

	/**************************** Only Dual **********************************/
	u_char	dbHlrPc[DB_HLR_PC_SIZE]; // size change sangyong 2 -> dual 4
	u_char  dbHlrPcFailReason[DB_HLR_PC_FAIL_REASON_SIZE]; // 1
	u_char  dbHlrPcDiag[DB_HLRPC_DIAG_SIZE]; // 1
	u_char	dbMoMscPc[DB_MO_MSC_SIZE]; // 4

	// GSM Dependancy
	u_char  dbMtPcNetType[DB_MT_PC_NETTYPE_SIZE]; //1
	u_char  dbMtMPc[DB_MT_M_PC_SIZE]; // 4 (Included GT)
	u_char  dbMtMPcFailReason[DB_MT_M_PC_FAIL_REASON_SIZE]; // 1
	u_char  dbMtMPcDiag[DB_MTMPC_DIAG_SIZE]; // 1
	//  u_char  dbMtMPcNettype[DB_MT_M_PC_NETTYPE_SIZE]; // 1
	u_char  dbMtGPc[DB_MT_G_PC_SIZE]; // 4
	u_char  dbMtGPcFailReason[DB_MT_G_PC_FAIL_REASON_SIZE]; // 1
	u_char  dbMtGPcDiag[DB_MTGPC_DIAG_SIZE]; // 1
	// u_char  dbMtGPcNettype[DB_MT_G_PC_NETTYPE_SIZE]; // 1

	u_char  dbMPcAddType[DB_M_PC_ADD_TYPE_SIZE]; // 1
	u_char  dbGPcAddType[DB_G_PC_ADD_TYPE_SIZE]; // 1

	// MME Dependancy
	u_char  dbMtMMEPc[DB_MT_M_PC_SIZE]; // 4 				// SR220
	u_char 	dbMtMMEFailReason[MDB_FLAG_SIZE]; // 1 			// SR220
	u_char 	dbMtMMEIsdn[DB_MME_ISDN_SIZE]; // 8 			// SR220

	u_char  dbMoPc[DB_MT_M_PC_SIZE]; // 4 				// SR220
	u_char 	dbMoIsdn[DB_MME_ISDN_SIZE]; // 8 			// SR220

	u_char  dbMtSMSFPc[DB_MT_M_PC_SIZE]; // 4 				// SR610
	u_char 	dbMtSMSFFailReason[MDB_FLAG_SIZE]; // 1 		// SR610
	u_char 	dbMtSMSFIsdn[DB_MME_ISDN_SIZE]; // 8 			// SR610

	u_char  dbMtSMSCPc[DB_MT_M_PC_SIZE]; // 4 				// SR720
	u_char 	dbMtSMSCFailReason[MDB_FLAG_SIZE]; // 1 		// SR720
	u_char 	dbMtSMSCIsdn[DB_MME_ISDN_SIZE]; // 8 			// SR720

	u_char	dbGsmNetType[DB_GSM_NET_TYPE_SIZE]; // 1

	u_char	dbSsmdpDeliveryStatus[DB_SSMDP_DELIVERY_STATUS_SIZE]; // 1

	u_char	dbInputIP[DB_INPUT_IP_SIZE]; // 4
	u_char	dbInputPort[DB_INPUT_PORT_SIZE]; // 2
	u_char	dbOutputIP[DB_OUTPUT_IP_SIZE]; // 4
	u_char	dbOutputPort[DB_OUTPUT_PORT_SIZE]; // 2

	u_char  dbOrgTotLength[DB_ORG_TOT_LENGTH_SIZE]; //2

	u_char  dbLongIdentifier[DB_LONG_IDENTIFIER_SIZE]; //4
	u_char	dbConcateRefType[DB_CONCATE_REF_TYPE_SIZE]; // 1
	u_char	dbLongRefNumber[DB_LONG_REF_NUMBER_SIZE]; // 2

	u_char	dbLongTotSegment[DB_LONG_TOT_SEGMENT_SIZE]; // 1
	u_char	dbLongCurSegment[DB_LONG_CUR_SEGMENT_SIZE]; // 1

	u_char	dbSessionInfo[DB_SESSION_INFO_SIZE]; // 2
	u_char	dbSessionValue[DB_SESSION_VALUE_SIZE]; // 2

	u_char	dbResponseCode[DB_RESPONSE_CODE_SIZE]; // 1 	263

	/* size  */
	/**************************************************************************/

	u_char	dbOrgSrcMinType[DB_ORG_SRC_MIN_TYPE_SIZE]; //1
	u_char	dbEsmeId[DB_ESME_ID_SIZE]; //4
	u_char	dbOrgDstMinType[DB_ORG_SRC_MIN_TYPE_SIZE]; //1
	u_char	dbOutputEsmeId[DB_ESME_ID_SIZE]; //4
	u_char	dbEsmeSeqId[DB_ESMESEQ_ID_SIZE]; // 4
	u_char	dbOutputRcsId[DB_ESME_ID_SIZE]; //R910

	/* Not Used
	u_char	dbSourcRouteTON[DB_SOURCROUTE_TON_SIZE]; // 1
	u_char	dbSourcRouteNPI[DB_SOURCROUTE_NPI_SIZE]; // 1
	u_char	dbSourcRouteMIN[DB_SOURCROUTE_MIN_SIZE]; // 12
	u_char	dbSourcRouteLen[DB_SOURCROUTE_LEN_SIZE]; // 1
	*/

	u_char	dbDestRouteTON[DB_DESTROUTE_TON_SIZE]; // 1
	u_char	dbDestRouteNPI[DB_DESTROUTE_NPI_SIZE]; // 1
	u_char	dbDestRouteMIN[DB_DESTROUTE_MIN_SIZE]; // 12
	u_char	dbDestRouteLen[DB_DESTROUTE_LEN_SIZE]; // 1

	/* Not Used
	u_char	dbSourceRedirectTON[DB_SOURCEREDIRECT_TON_SIZE]; // 1
	u_char	dbSourceRedirectNPI[DB_SOURCEREDIRECT_NPI_SIZE]; // 1
	u_char	dbSourceRedirectMIN[DB_SOURCEREDIRECT_MIN_SIZE]; // 12
	u_char	dbSourceRedirectLen[DB_SOURCEREDIRECT_LEN_SIZE]; // 1
	*/

	u_char	dbDestRedirectTON[DB_DESTREDIRECT_TON_SIZE]; // 1
	u_char	dbDestRedirectNPI[DB_DESTREDIRECT_NPI_SIZE]; // 1
	u_char	dbDestRedirectMIN[DB_DESTREDIRECT_MIN_SIZE]; // 12
	u_char	dbDestRedirectLen[DB_DESTREDIRECT_LEN_SIZE]; // 1

	u_char	dbUDHIFlag[DB_UDHI_FLAG_SIZE]; // 1 forward try count
	u_char	dbRedirectUsable[DB_REDIRECT_USE_SIZE]; // 1 redirect on/off/over trycnt

	u_char	dbNpdbIP[DB_NPDB_IP_SIZE]; // 4
	u_char	dbNpdbPort[DB_NPDB_PORT_SIZE]; // 2

	u_char dbDoNotForward[DB_DO_NOT_FORWARD_SIZE]; //1
	u_char	dbPorted[DB_PORTED_SIZE]; // 1
	u_char	dbAlreadySend_NPDB[DB_PORTED_SIZE]; // 1

	u_char	dbOrgImsi[DB_IMSI_SIZE]; // 12
	u_char	dbOrgImsiLen[DB_IMSI_LEN_SIZE]; // 1
	u_char	dbDestImsi[DB_IMSI_SIZE]; // 12
	u_char	dbDestImsiLen[DB_IMSI_LEN_SIZE]; // 1
	u_char	dbLmsi[DB_LMSI_SIZE];	// 4
	u_char	dbOrgEsn[DB_ESN_SIZE]; // 4

	u_char	dbMsMsgId[DB_MSMSGID_SIZE]; //2
	u_char	dbEndOfSession[DB_ENDOFSESSION_SIZE]; //1
	u_char	dbInteractiveFlag[DB_INTERACTIVEFLAG_SIZE]; //1

	// Add for Other TelCo Trans Message (Plus SMFC) 2004.01.27.
	// If Msg is trans to Other TelCo, true(1) Otherwise. false(0)
	u_char	dbRoamFlag[DB_ROAMFLAG_SIZE]; // 1

	u_char  dbSpamFlags[DB_SPAM_FLAG_SIZE]; // 1

	// for Color&LONG Mobile Station Display Capability.
	u_char 	dbDestMsCapability[DB_DEST_MS_CAPABILITY_SIZE]; // 4
	u_char 	dbDestMsCapabilityFlag[DB_DEST_MS_CAPABILITY_SIZE]; // 4

	u_char	dbOSFI[DB_OSFI_SIZE]; // 4
	u_char	dbOSFIFlag[DB_OSFI_FLAG_SIZE]; // 4

	u_char 	dbSmsOSFI[DB_OSFI_MAX_SIZE]; // 8
	u_char 	dbSmsOSFIFlag[DB_OSFI_MAX_SIZE]; // 8
	u_char 	dbSmsOSFIRecv[DB_OSFI_MAX_SIZE]; // 8

	u_char	dbSCDenyFlag[MDB_FLAG_SIZE]; // 1

	u_char	dbSpamSVCtime[MDB_TIME_TYPE_SIZE];//4
	u_char	dbIcsSVCtime[MDB_TIME_TYPE_SIZE]; //4
	u_char	dbPicsSVCtime[MDB_TIME_TYPE_SIZE]; // 4
	u_char	dbQuerySVCtime[MDB_TIME_TYPE_SIZE];//4
	u_char	dbPsmsSVCtime[MDB_TIME_TYPE_SIZE]; //4
	u_char	dbTzsmsSVCtime[MDB_TIME_TYPE_SIZE]; //4
	u_char	dbSnapSVCtime[MDB_TIME_TYPE_SIZE]; // 4
	u_char	dbAsmssSVCtime[MDB_TIME_TYPE_SIZE]; //4
	u_char	dbDRtoMStime[MDB_TIME_TYPE_SIZE]; //4
	u_char	dbIMSMStime[MDB_TIME_TYPE_SIZE]; //4
	u_char	dbMgrOSFItime[MDB_TIME_TYPE_SIZE]; // 4
	u_char	dbMgrCAPAtime[MDB_TIME_TYPE_SIZE]; // 4
	u_char	dbPatriotCAPAtime[MDB_TIME_TYPE_SIZE]; // 4
	u_char  dbATSIREQtime[MDB_TIME_TYPE_SIZE];	//4 FR220 CYSMS to 010_PLUS_SVC
	u_char  dbPerfectCAPAtime[MDB_TIME_TYPE_SIZE];  //4
	u_char  dbNewSpamSVCtime[MDB_TIME_TYPE_SIZE];   //4
	u_char  dbCallbackNotiSVCtime[MDB_TIME_TYPE_SIZE];   //4 FR230 Callback Noti
	u_char  dbRcsSVCtime[MDB_TIME_TYPE_SIZE]; // FR410 IPMG RCS
	u_char  dbMTMultiNUMtime[MDB_TIME_TYPE_SIZE]; // DR730 Multi Number SVC 
	u_char  dbMcsMoTime[MDB_TIME_TYPE_SIZE]; // SR910
	u_char  dbMcsMtTime[MDB_TIME_TYPE_SIZE]; // SR910
	u_char  dbMcsCxlTime[MDB_TIME_TYPE_SIZE]; // SR910
	u_char  dbAtsiSupplyTime[MDB_TIME_TYPE_SIZE]; // SR910

	u_char	dbSpamSVCStatus[DB_SPAM_SVC_STATUS_SIZE];//1    452
	u_char	dbIcsSVCStatus[DB_ICS_SVC_STATUS_SIZE]; //1
	u_char	dbPicsSVCStatus[DB_PICS_SVC_STATUS_SIZE]; // 1
	u_char	dbPicsImmetTransStatus[MDB_FLAG_SIZE]; // 1
	u_char	dbQuerySVCStatus[DB_QUERY_SVC_STATUS_SIZE];//1
	u_char	dbPsmsSVCStatus[DB_PSMS_SVC_STATUS_SIZE]; //1
	u_char	dbTzsmsSVCStatus[DB_TZSMS_SVC_STATUS_SIZE]; //1
	u_char	dbSnapSVCStatus[DB_SNAP_SVC_STATUS_SIZE]; // 1
	u_char	dbAsmssSVCStatus[DB_ASMSS_SVC_STATUS_SIZE]; //1
	u_char	dbDRtoMSStatus[MDB_FLAG_SIZE]; //1
	u_char	dbIMSMSStatus[MDB_FLAG_SIZE]; //1
	u_char	dbMgrOSFIStatus[MDB_FLAG_SIZE]; // 1
	u_char	dbMgrCAPAStatus[MDB_FLAG_SIZE]; // 1
	u_char	dbMgrImmetTransStatus[MDB_FLAG_SIZE]; // 1
	u_char	dbPatriotCAPAStatus[MDB_FLAG_SIZE]; // 1
	u_char  dbATSIREQStatus[MDB_FLAG_SIZE];   //1 FR220 CYSMS to 010_PLUS_SVC
	u_char  dbPerfectCAPAStatus[MDB_FLAG_SIZE]; //1
	u_char  dbNewSpamSVCStatus[MDB_FLAG_SIZE];  //1
	u_char  dbCallbackNotiStatus[MDB_FLAG_SIZE];  //1 FR230 Callback Noti
	u_char  dbRcsSVCStatus[MDB_FLAG_SIZE]; // FR410 IPMG RCS
	u_char  dbMTMultiNUMStatus[MDB_FLAG_SIZE]; // DR730 Multi Number SVC 
	u_char  dbMcsMoType[MDB_FLAG_SIZE]; // SR910
	u_char  dbMcsMoStatus[MDB_FLAG_SIZE]; // SR910
	u_char  dbMcsMtSmsType[MDB_FLAG_SIZE]; // SR910
	u_char  dbMcsMtType[MDB_FLAG_SIZE]; // SR910
	u_char  dbMcsMtStatus[MDB_FLAG_SIZE]; // SR910
	u_char  dbMcsMtRct[MDB_FLAG_SIZE]; // SR910
	u_char  dbMcsMtStat[MDB_FLAG_SIZE]; // SR910
	u_char  dbMcsCxlStatus[MDB_FLAG_SIZE]; // SR910
	u_char  dbAtsiSupplyStatus[MDB_FLAG_SIZE]; // SR910
	u_char	dbRoamingInd[MDB_ROAMIND_SIZE]; //1
	u_char  dbServicePrefix[DB_SKT_SERVICE_PREFIX_SIZE];  //5

	u_char	dbGenerationType[DB_GENERATION_TYPE_SIZE]; //1

	u_char	dbDestMsCapabilityRecv[DB_DEST_MS_CAPABILITY_RECV_SIZE];	// 2
	u_char	dbDestMsCapabilityRecv2[DB_DEST_MS_CAPABILITY_RECV_SIZE];	// 2
	u_char	dbOSFIRecv[DB_OSFI_RECV_SIZE];	// 2

	u_char	dbMscId[DB_MSC_ID_SIZE];	// 3
	u_char	dbLocationAreaId[DB_LOCATION_AREA_ID_SIZE];	// 2
	u_char	dbRoamingPMN[DB_ROAMING_PMN_SIZE];	// 4

	u_char	dbNodeStatus[DB_NODE_STATUS_SIZE];	// 1
	u_char	dbNodeType[DB_NODE_TYPE_SIZE];		// 1
	u_char dbRNPrefix[MDB_MIN_SIZE];//12  FR401 LSH

	u_char  dbStatSendTime[DB_STAT_SENDTIME_SIZE];	// 4

	u_char	dbMtMsType[DB_MT_MS_TYPE_SIZE];	// 1
	u_char	dbMsgTransformCnt[DB_MSG_TRANSFORM_CNT_SIZE];	// 2

	u_char dbRegInfoCount[DB_REG_INFO_COUNT_SIZE]; //2  FR210 Noti Count for LMS_RELAY

	u_char dbAdminBarStatus[DB_ADMIN_BAR_SIZE]; //1  // dsmsc pkg not used (only fsmsc)

	u_char	dbOSFI2[DB_OSFI_SIZE]; // 4
	u_char	dbOSFIFlag2[DB_OSFI_FLAG_SIZE]; // 4
	u_char	dbOSFIRecv2[DB_OSFI_RECV_SIZE];	// 2

	u_char dbCallbackNotiFlag[DB_CALLBACK_NOTI_FLAG_SIZE]; // 1 FR230
	u_char dbCallbackNotiNWChgFlag[DB_CALLBACK_NOTI_NWCHG_FLAG_SIZE]; // 1 FR230
	u_char dbCallbackNotiOrigination[DB_CALLBACK_NOTI_ORIGINATION_SIZE]; // 1 FR230
	u_char dbCallbackNotiApplyFlag[DB_CALLBACK_NOTI_APPLY_FLAG__SIZE]; // 1 FR230
	u_char dbCallbackNotiLink[DB_MESSAGE_ID_SIZE]; // 4 FR230

	u_char dbIconvExtLen[DB_ICONV_EXT_LEN_SIZE]; // 1 FR310 PKG
	u_char dbLteMtInfo[DB_LTE_MT_INFO_SIZE]; //FR410 PKG
//	u_char dbSmsMoSac[DB_SMS_MO_SAC_SIZE]; // FR401 PKG LSH 2
	u_char dbIPMGRcsResult[DB_IPMG_RCS_RESULT_SIZE]; // FR410 IPMG RCS size 2
	u_char dbIPMGRcsRefId[DB_IPMG_RCS_REF_ID_SIZE]; // FR410 IPMG size 14
	u_char dbIPMGMessageId[DB_MESSAGE_ID_SIZE]; // FR410 IPMG 4

	u_char dbEpeRecvTime[MDB_TIME_TYPE_SIZE];

	u_char  dbCDRFlag[MDB_FLAG_SIZE]; // FR420 CDR Make

	u_char dbATSIType[MDB_FLAG_SIZE]; //DR710 1 
	u_char dbAtsiSupplyType[MDB_FLAG_SIZE]; //R910
	u_char dbSecurityFlag[MDB_FLAG_SIZE]; // DR710
	u_char 	dbMsgCutPoint[MDB_HALF_NUM_SIZE]; // DR720 cp949 to ucs2 msg size over size 2
	u_char  dbUserDataLen[MDB_FLAG_SIZE]; // DR720 pkg by cbh : add user data length

	u_char	dbOrgRefNumber[DB_LONG_REF_NUMBER_SIZE]; // 2

	u_char	dbOrgTotSegment[DB_LONG_TOT_SEGMENT_SIZE]; // 1
	u_char	dbOrgCurSegment[DB_LONG_CUR_SEGMENT_SIZE]; // 1
	u_char dbMOMultiNUMOrder[MDB_FLAG_SIZE]; // DR730 hjy MultiNUM svc child order
	u_char dbMTMultiNUMOrder[MDB_FLAG_SIZE]; // DR730 hjy MultiNUM svc child order

	u_char dbAuthFlag[MDB_FLAG_SIZE]; // FR620 pkg Auth flag
	u_char dbFirstStatus[DB_STATUS_SIZE]; 			// FR620 pkg
	u_char dbFirstDelivery[MDB_TIME_TYPE_SIZE]; 	// FR620 pkg
	u_char dbSecondStatus[DB_STATUS_SIZE]; 		// FR620 pkg
	u_char dbSecondDelivery[MDB_TIME_TYPE_SIZE]; 	// FR620 pkg 
	u_char dbSmppMessageId[DB_SMPP_MESSAGE_ID_SIZE];

	u_char dbProfileFlag[1];
	u_char dbImsPriority[1];
	u_char dbImsMtRpCause[1];
	u_char dbImsMtTpCause[1];
	u_char dbImsMtSipError[2];
	u_char dbMtCscfNum[4];
	u_char dbCallID[128];

	u_char	dbReserved[DB_RESERVED_SIZE]; // VR100 2

	u_char	dbMessageLength[DB_MESSAGE_LENGTH_SIZE]; // 1
	u_char	dbMessageContent[DB_MESSAGE_CONTENT_SIZE]; // FR220 161 ==> 181

} stRDActiveTuple;

typedef stRDActiveTuple stRDReceiptTuple;

typedef enum {
	SECURITY_OFF = 0x00,
	SECURITY_ON
} SecuritySms;

typedef enum {
	RSP_M2M_SVC_OFF 	= 0x00,
	RSP_M2M_SVC_ON 		= 0x01
} RspM2mSvc;

typedef enum {
	RSP_M2M_UDHI_OFF 	= 0x00,
	RSP_M2M_UDHI_ON 	= 0x40
} RspM2mUdhiFlag;

typedef enum {
	RSP_M2M_INTER_OFF 	= 0x00,
	RSP_M2M_INTER_ON 	= 0x10
} RspM2mOrgMinType;

/*
 * For Address Type of R_DPrefix table
 * 98/07/21 append ESME_TYPE
 */

#ifdef	MIN_TYPE
#undef	MIN_TYPE
#endif

typedef enum {
	MIN_TYPE	= 0x01,
	PC_TYPE		= 0x02,
	GT_TYPE		= 0x03,
	TCPIP_TYPE	= 0x03,
	ESME_TYPE	= 0x04
} ePrefixType_t;

typedef enum {
	HLR_TYPE	= 0x01,
	SC_TYPE		= 0x02
} ePrefixSubType_t;

typedef enum {
	HLR_ADDR = 0x01, SMC_ADDR, ESME_ADDR, GATE_ADDR
} eAddressType_t;

typedef enum {
	NET_2G = 0x00,
	NET_3G = 0x01
} eNetworkType_t;

#define	PC_ADDR_SIZE		2
#define	TCPIP_ADDR_SIZE		4
#define	ESME_ADDR_SIZE		4


/********************* ADD For MNP ***************************************/
/*
 * The column ids of R_DPrefix table
 */
typedef enum Column_ID_Of_R_DPrefixTable {
	DB_PREFIX,
	DB_ADDRESS,
	DB_PREFIX_LENGTH,
	DB_ADDRTYPE,
	DB_S6CGRP, // SR510 pkg
	DB_SHGRP,
	DB_NET_VER_TYPE
} eColumn_ID_Of_R_DPrefixTable_t;

typedef enum Column_ID_Of_R_DUDMPrefixTable {
	DB_UDMPREFIX,
	DB_GRPNUM
	// DB_NET_VER_TYPE
} eColumn_ID_Of_R_DUDMPrefixTable_t;
/*
 * The attribute sizes of R_DPrefix table
 */

#define DB_PREFIX_SIZE			12	// Size Problem
#define	DB_PREFIX_LENGTH_SIZE	1
#define DB_ADDRESS_SIZE			4
#define DB_ADDRTYPE_SIZE		1
#define DB_MNPTYPE_SIZE			1
#define DB_NET_VER_TYPE_SIZE	1
#define DB_GROUP_UDM_SIZE 		4
#define DB_GROUP_UDM_CHAR_SIZE 		1

typedef struct {
	u_char	dbPrefix[DB_PREFIX_SIZE]; //12	
} stRDProfilePrefixTuple;

typedef struct {
	u_char	dbPrefix[DB_PREFIX_SIZE]; //12
	u_char	dbGrpNum[DB_GROUP_UDM_SIZE]; //4
} stRDS6CPrefixTuple;

typedef stRDS6CPrefixTuple stRDSHPrefixTuple;
/*
 * The tuple structure of R_DPrefix table
 */
typedef struct {
	u_char	dbPrefix[DB_PREFIX_SIZE]; //12
	u_char	dbAddress[DB_ADDRESS_SIZE]; //4
	u_char	dbPrefixLength[DB_PREFIX_LENGTH_SIZE];//1
	u_char	dbAddrType[DB_ADDRTYPE_SIZE]; //1
	u_char  dbS6cGrpNum[DB_GROUP_UDM_CHAR_SIZE];//1
	u_char	dbShGrpNum[DB_GROUP_UDM_CHAR_SIZE]; //1
} stRDPrefixTuple;

typedef struct {
	u_char cpPrefix[DB_PREFIX_SIZE];
	u_int  uAddress;
	u_char ucPrefixLength;
	u_char ucAddrType;
	u_char ucHlrGrpNum;
	u_char ucShGrpNum;
} stRDPrefixType;

typedef stRDPrefixTuple stDDBPrefixTuple;
#define PREFIXTUPLESIZE sizeof(stDDBPrefixTuple)
typedef stRDPrefixTuple stRDPrefix_RNTuple;
typedef stRDPrefixTuple stRDNPQC_IDTuple;

//| dbPrefix | dbAddress | dbPrefixLength | dbAddrType | dbSMCNumber | dbNetworkType |
typedef struct {
	u_char	dbPrefix[DB_PREFIX_SIZE]; //12
	u_char	dbAddress[DB_ADDRESS_SIZE]; //4
	u_char	dbPrefixLength[DB_PREFIX_LENGTH_SIZE];//1
	u_char	dbBlockType[DB_ADDRTYPE_SIZE]; //1
	u_char  dbSMCNumber[DB_MNPTYPE_SIZE];//1
	u_char	dbNetworkType[DB_NET_VER_TYPE_SIZE]; //1
} stRDPrefix_SMCTuple;

//FR730 pkg
typedef enum {
	SCROUTING_BLOCK,
	SCROUTING_ROUTING
} eSCFLAG_t;

typedef enum {
	SCROUTING_BASE,
	SCROUTING_MME_CONCAT,
	SCROUTING_MME_LMSGW,
	SCROUTING_DENY, // mo service allow
	SCROUTING_AFTER_DENY, // mo service not allow
	SCROUTING_M2M_GLR,
	SCROUTING_M2M_GLR_DENY,
	SCROUTING_M2M_GLR_SVC_OFF
} eSCRoutingBlock_t;

typedef enum {
	M2M_GLR_ROUTE_NONE 		= 0x00,
	M2M_GLR_ROUTE_SRC_PFX 	= 0x10,
	M2M_GLR_ROUTE_SRC_MIN 	= 0x20,
	M2M_GLR_ROUTE_DST_PFX 	= 0x01,
	M2M_GLR_ROUTE_DST_MIN 	= 0x02
} eM2MGlrRoutingBlock_t;

/*
 * The attribute sizes of R_DConvertTID table
 */
typedef enum Column_ID_Of_R_DConvertTIDTable {
	DB_SERVICE_TYPE_CONVERT,
	DB_TELESERVICEID,
	DB_PROTOCOLID,
	DB_APPLICATIONPORT,
	DB_ORIGAPPLICATIONPORT,
	DB_CONVT_RESERVED
} eColumn_ID_Of_R_DConvertTIDTable_t;

#define DB_PROTOCOLID_SIZE			1
#define	DB_APPLICATIONPORT_SIZE		2

typedef struct {
	u_char dbServiceType[DB_SERVICE_TYPE_SIZE]; // size 6
	u_char dbTeleServiceID[DB_TELESERVICEID_SIZE]; // size 2
	u_char dbProtocolID[DB_PROTOCOLID_SIZE];// size 1
	u_char dbApplicationPort[DB_APPLICATIONPORT_SIZE]; // size 2
	u_char dbOrigApplicationPort[DB_APPLICATIONPORT_SIZE]; // size 2
	u_char dbReserved[MDB_HALF_NUM_SIZE]; //2
} stRDConvert_TIDTuple;

typedef struct {
	u_char 	cpServiceType[DB_SERVICE_TYPE_SIZE];
	u_short usTeleServiceID;
	u_char	ucProtocolID;
	u_short	usApplicationPort;
	u_short	usOrigApplicationPort;
	u_short	ucReserved;
} stRDConvert_TIDType;

/*
 * The attribute sizes of R_DTID_Routing table
 */
typedef enum Column_ID_Of_R_DTID_RouteTable {
	DB_TR_TELESERVICEID,
	DB_TR_RESERVE,
	DB_TR_ESMEID,
	DB_TR_DESC
} eColumn_ID_Of_R_DTID_RouteTable;

#define		DB_TR_DESC_SIZE		16

typedef struct {
	u_char dbTeleServiceID[DB_TELESERVICEID_SIZE]; // 2
	u_char dbReserved[MDB_HALF_NUM_SIZE]; // 2
	u_char dbEsmeID[DB_ESME_ID_SIZE];     // 4
	u_char dbDescription[DB_TR_DESC_SIZE]; // 16
} stRDTID_RouteTuple;

typedef struct {
	u_short usTeleServiceID;
	u_short	usReserved;
	u_int	uEsmeID;
	u_char	ucServiceFlow[DB_TR_DESC_SIZE];
} stRDTID_RouteType;

/*
 * The tuple structure of R_DIsdn table.
 */
typedef enum Column_ID_Of_R_DISDNTable {
	DB_ISDN_NUMBER,
	DB_ISDN_PC,
	DB_ISDN_SSN,
	DB_ISDN_TYPE,
	DB_ISDN_VER,
	DB_ISDN_MME_ON_OFF,
	DB_ISDN_MME_NAME, 			// SR220
	DB_ISDN_MME_REALM, 			// SR220
	DB_ISDN_DESC,
} eColumn_ID_Of_R_DISDNTable_t;

#define DB_ISDN_SIZE        	8
#define DB_ISDN_PC_SIZE     	2
#define DB_ISDN_SSN_SIZE    	1
#define DB_ISDN_TYPE_SIZE   	1
#define DB_ISDN_VER_SIZE    	1
#define DB_ISDN_NODE_SIZE 		1 			// SR220
#define DB_ISDN_RESERVED_SIZE	2
#define DB_ISDN_DESC_SIZE   	20
#define DB_ISDN_MME_NAME_SIZE 	32 // SR220 PKG
#define DB_ISDN_MME_ON_OFF_SIZE 1

typedef struct {
	u_char  dbIsdnNumber[DB_ISDN_SIZE]; //8
	u_char  dbIsdnPc[DB_ISDN_PC_SIZE]; // 2
	u_char  dbIsdnSsn[DB_ISDN_SSN_SIZE]; // 1
	u_char  dbIsdnNetType[DB_ISDN_TYPE_SIZE]; //1
	u_char  dbIsdnVer[DB_ISDN_VER_SIZE]; //1
	u_char  dbIsdnMmeOnOff[DB_ISDN_MME_ON_OFF_SIZE]; //1
	u_char 	dbIsdnMMEName[DB_MME_NAME_SIZE]; // 64 SR220 PKG
	u_char 	dbIsdnMMERealm[DB_MME_NAME_SIZE]; // 64 SR220 PKG
	u_char  dbIsdnDesc[DB_ISDN_DESC_SIZE]; //20
	u_char  dbIsdnReserved[DB_ISDN_RESERVED_SIZE]; //
} stRDIsdnTuple;

//SR720 pkg
typedef enum Column_ID_Of_R_DDIAMETERSMSCTable {
	DB_SMSC_ID,
	DB_SMSC_ISDN,
	DB_SMSC_SSN,
	DB_SMSC_CONNECT,
	DB_SMSC_CONN_MODE,
	DB_SMSC_PRI_ONOFF,
	DB_SMSC_SEC_ONOFF,
	DB_SMSC_PRI_HOST,
	DB_SMSC_PRI_REALM,
	DB_SMSC_PRI_IP,
	DB_SMSC_PRI_PORT,
	DB_SMSC_SEC_HOST,
	DB_SMSC_SEC_REALM,
	DB_SMSC_SEC_IP,
	DB_SMSC_SEC_PORT,
	DB_SMSC_RESERVED,
	DB_SMSC_DESC
} eColumn_ID_Of_R_DDIAMETERSMSCTable_t;

#define DB_SMSC_ID_SIZE 			2
#define DB_SMSC_ISDN_SIZE 			8
#define DB_SMSC_SSN_SIZE 			1
#define DB_SMSC_CONNECT_SIZE 		1
#define DB_SMSC_CONN_MODE_SIZE 		1
#define DB_SMSC_ONOFF_SIZE 			1
#define DB_SMSC_HOST_SIZE 			64
#define DB_SMSC_REALM_SIZE 			64
#define DB_SMSC_IP_SIZE 			4
#define DB_SMSC_PORT_SIZE 			2
#define DB_SMSC_RESERVED_SIZE 		1
#define DB_SMSC_DESC_SIZE 			20

typedef struct {
	u_char dbSmscID[DB_SMSC_ID_SIZE];
	u_char dbIsdn[DB_SMSC_ISDN_SIZE];
	u_char dbSsn[DB_SMSC_SSN_SIZE];
	u_char dbConnect[DB_SMSC_CONNECT_SIZE];
	u_char dbConnMode[DB_SMSC_CONN_MODE_SIZE];
	u_char dbPriOnOff[DB_SMSC_ONOFF_SIZE];
	u_char dbSecOnOff[DB_SMSC_ONOFF_SIZE];
	u_char dbPriHost[DB_SMSC_HOST_SIZE];
	u_char dbPriRealm[DB_SMSC_REALM_SIZE];
	u_char dbPriIP[DB_SMSC_IP_SIZE];
	u_char dbPriPort[DB_SMSC_PORT_SIZE];
	u_char dbSecHost[DB_SMSC_HOST_SIZE];
	u_char dbSecRealm[DB_SMSC_REALM_SIZE];
	u_char dbSecIP[DB_SMSC_IP_SIZE];
	u_char dbSecPort[DB_SMSC_PORT_SIZE];
	u_char dbReserved[DB_SMSC_RESERVED_SIZE];
	u_char dbDesc[DB_SMSC_DESC_SIZE];
} stRDiameterSMSCTuple;

typedef struct {
	u_short	dbSmscID;
	u_char 	dbIsdn[DB_SMSC_ISDN_SIZE];
	u_char 	dbSsn;
	u_char 	dbConnect;
	u_char 	dbConnMode;
	u_char 	dbPriOnOff;
	u_char 	dbSecOnOff;
	u_char 	dbPriHost[DB_SMSC_HOST_SIZE];
	u_char 	dbPriRealm[DB_SMSC_REALM_SIZE];
	u_char 	dbPriIP[DB_SMSC_IP_SIZE];
	u_short	dbPriPort;
	u_char 	dbSecHost[DB_SMSC_HOST_SIZE];
	u_char 	dbSecRealm[DB_SMSC_REALM_SIZE];
	u_char 	dbSecIP[DB_SMSC_IP_SIZE];
	u_short	dbSecPort;
	u_char 	dbReserved[DB_SMSC_RESERVED_SIZE];
	u_char 	dbDesc[DB_SMSC_DESC_SIZE];
} stRDiameterSMSCType;

//R810 pkg
typedef enum Column_ID_Of_R_DINFORATECIDTable
{
	DB_INFO_RATE_CID,
	DB_INFO_RATE_STIME,
    DB_INFO_RATE_ETIME,
	DB_INFO_RATE_SVCNAME,
	DB_INFO_RATE,
	DB_INFO_RATE_PROC_TIME,
	DB_INFO_RATE_MSG_TYPE,
	DB_INFO_RATE_BILL_TYPE,
	DB_INFO_RATE_BILL_LIMIT,
	DB_INFO_RATE_RESERVED
} eColumn_ID_Of_R_DINFORATECIDTable_t;

//#define DB_INFO_RATE_CID_SIZE 			MDB_MIN_SIZE
//#define DB_INFO_RATE_TIME_SIZE 			12
//#define DB_INFO_RATE_SVC_NAME_SIZE 		32 
//#define DB_INFO_RATE_SIZE 				MDB_NUMBER_SIZE
//#define DB_INFO_RATE_PROC_TIME_SIZE		MDB_TIME_TYPE_SIZE

#define DB_INFO_RATE_CID_SIZE 			12
#define DB_INFO_RATE_STIME_SIZE 		12
#define DB_INFO_RATE_ETIME_SIZE 		12
#define DB_INFO_RATE_SVC_NAME_SIZE 		32 
#define DB_INFO_RATE_SIZE 				8
#define DB_INFO_RATE_PROC_TIME_SIZE		4
#define DB_INFO_RATE_RESERVED_SIZE 		1

typedef struct{
	u_char dbCID[DB_INFO_RATE_CID_SIZE];
	u_char dbSTime[DB_INFO_RATE_STIME_SIZE];
	u_char dbETime[DB_INFO_RATE_ETIME_SIZE];
	u_char dbSvcName[DB_INFO_RATE_SVC_NAME_SIZE];
	u_char dbInfoRate[DB_INFO_RATE_SIZE];
	u_char dbProcTime[DB_INFO_RATE_PROC_TIME_SIZE];
	u_char dbMsgType[MDB_FLAG_SIZE];
	u_char dbBillType[MDB_FLAG_SIZE];
	u_char dbBillLimit[MDB_FLAG_SIZE]; 
	u_char dbReserved[DB_INFO_RATE_RESERVED_SIZE];
}stRInfoRateCIDTuple;
#if 0
typedef struct{
	u_char 	ucCID[DB_INFO_RATE_CID_SIZE]; // 12
	u_char 	ucSTime[DB_INFO_RATE_STIME_SIZE]; // 12
	u_char 	ucETime[DB_INFO_RATE_ETIME_SIZE]; // 12
	u_char 	ucSvcName[DB_INFO_RATE_SVC_NAME_SIZE]; // 32
	long 	lInfoRate; // 8
	u_int 	unProcTime; // 4
	u_char 	ucMsgType; // 1
	u_char 	ucBillType; // 1
	u_char 	ucBillLimit; //1 
	u_char 	ucReserved[DB_INFO_RATE_RESERVED_SIZE]; // 1
}stRInfoRateCIDType;
#endif

/*
 * The tuple structure of stRDNpdbRouting table.
 */
typedef enum Column_NPDB_RoutingTable_t {
	DB_NPQC_ID,
	DB_NPDB_IP,
	DB_NPDB_PORT,
	DB_NPDB_CONNECT_ID,
	DB_NPDB_CONNECT_PWD,
	DB_NPQC_TIMEOUT,
	DB_NPQC_ACTUAL,
	DB_NPQC_MAX_CONNECTCNT,
	DB_NPQC_CURRENT_CONNECTCNT,
	DB_NPDB_DESC
} eColumn_ID_Of_R_DNPDB_RoutingTable_t;

#define DB_INPUT_ID_SIZE                        4
#define DB_NPDB_CONNECT_ID_SIZE					16
#define DB_NPDB_CONNECT_PWD_SIZE				16
#define DB_INPUT_ID_SIZE                        4
#define DB_NPQC_TIMEOUT_SIZE                    1
#define DB_NPQC_ACTUAL_SIZE                     1
#define DB_NPQC_MAX_CONNECTCNT_SIZE             1
#define DB_NPQC_CURRENT_CONNECTCNT_SIZE         1
#define DB_NPDB_DESC_SIZE						20

typedef struct {
	u_char dbNPQC_ID[DB_INPUT_ID_SIZE];						// 4
	u_char dbNPDB_IP[DB_ADDRESS_SIZE];						// 4
	u_char dbNPDB_Port[DB_INPUT_PORT_SIZE];					// 2
	u_char dbNPDB_Connect_ID[DB_NPDB_CONNECT_ID_SIZE];		// 16
	u_char dbNPDB_Connect_PWD[DB_NPDB_CONNECT_PWD_SIZE];	// 16
	u_char dbTimeOut[DB_NPQC_TIMEOUT_SIZE];					// 1
	u_char dbActual[DB_NPQC_ACTUAL_SIZE];					// 1
	u_char dbMax_ConnectCnt[DB_NPQC_MAX_CONNECTCNT_SIZE];	// 1
	u_char dbCurrent_ConnectCnt[DB_NPQC_CURRENT_CONNECTCNT_SIZE];	// 1
	u_char dbNPDB_Desc[DB_NPDB_DESC_SIZE];					// 20
} stRDNP_DB_RoutingTuple;

typedef enum Column_NPDB_PrefixTable_t {
	DB_NPDB_PREFIX,
	DB_NPDB_PREFIX_LENGTH,
	DB_NPDB1_NODE,
	DB_NPDB2_NODE,
	DB_NPDB_RESERVED
} eColumn_NPDB_PrefixTable_t;

#define DB_NPDB_PREFIX_SIZE         12
#define DB_NPDB_PREFIX_LENGTH_SIZE  1
#define DB_NPDB_NODE_SIZE           4
#define DB_NPDB_RESERVED_SIZE       3

typedef struct {
	u_char  dbNpdbPrefix[DB_NPDB_PREFIX_SIZE];
	u_char  dbNpdbPrefixLength[DB_NPDB_PREFIX_LENGTH_SIZE];
	u_char  dbNpdb1Node[DB_NPDB_NODE_SIZE];
	u_char  dbNpdb2Node[DB_NPDB_NODE_SIZE];
	u_char  dbNpdbReserved[DB_NPDB_RESERVED_SIZE];
} stRDNpdbPrefixTuple;

/*
 * The column ids of R_DVMIN_PREFIX table
 */
typedef enum Column_ID_Of_R_DVMIN_PrefixTable {
	DB_VMIN_PREFIX,
	DB_VMIN_PREFIX_LEN,
	DB_VMIN_START_DIGIT_LEN,
	DB_VMIN_END_DIGIT_LEN,
	DB_VMIN_DUAL_SIDE_INFO
} eColumn_ID_Of_R_DVMIN_PrefixTable_t;

#define DB_VMIN_PREFIX_SIZE				12
#define DB_VMIN_PREFIX_LEN_SIZE			1
#define DB_VMIN_START_DIGIT_LEN_SIZE	1
#define	DB_VMIN_END_DIGIT_LEN_SIZE		1

typedef struct {
	u_char dbVMinPrefix[DB_VMIN_PREFIX_SIZE]; //12
	u_char dbVMinPrefixLength[DB_VMIN_PREFIX_LEN_SIZE]; //1
	u_char dbVMinStartDigitLength[DB_VMIN_START_DIGIT_LEN_SIZE]; //1
	u_char dbVMinEndDigitLength[DB_VMIN_END_DIGIT_LEN_SIZE]; //1
	u_char	dbDualSideInfo[MDB_LEN_SIZE]; // 1
} stRDVMIN_PrefixTuple;

typedef struct {
	u_char	cpVMinPrefix[DB_VMIN_PREFIX_SIZE];
	u_char	ucVMinPrefixLength;
	u_char	ucVMinStartDigitLength;
	u_char	ucVMinEndDigitLength;
	u_char	ucdbReserved;
} stRDVMIN_PrefixType;

#define DEFAULT_ESME					"0190000000"
#define DEFAULT_ESME_DESCRIPTION		"default"
#define DEFAULT_ESME_DESCRIPTION_SIZE	7

#define MANAGEMENT_LINK_INSERT  1
#define MANAGEMENT_LINK_DELETE  2

/*
 * The column ids of R_DEsme table
 */
typedef enum Column_ID_Of_R_DEsmeTable {
	DB_ESME,
	DB_TELEID,
	DB_FRAME,
	DB_SOURCE_TYPE,
	DB_DESCRIPTION,
	DB_ESME_RESERVED
} eColumn_ID_Of_R_DEsmeTable_t;

#define DB_ESME_SIZE			12 // Size Problem
#define DB_FRAME_SIZE			1
#define DB_TELEID_SIZE			2
#define DB_SOURCE_TYPE_SIZE		1
#define DB_DESCRIPTION_SIZE		50

/*
 * The tuple structure of R_DEsme table.
 */
typedef struct {
	u_char	dbEsme[DB_ESME_SIZE]; // 12
	u_char	dbTeleId[DB_TELEID_SIZE]; // 2
	u_char	dbFrame[DB_FRAME_SIZE]; //1
	u_char	dbSrcServiceType[DB_SOURCE_TYPE_SIZE];// 1
	u_char	dbDescription[DB_DESCRIPTION_SIZE]; // 50
	u_char	dbReserved[MDB_HALF_NUM_SIZE]; // 2
} stRDEsmeTuple;

typedef struct {
	u_char	cpEsme[DB_ESME_SIZE];
	u_short	usTeleId;
	u_char	ucFrame;
	u_char	ucSrcServiceType;
	u_char	cpDescription[DB_DESCRIPTION_SIZE];
	u_short	usReserved;
} stRDEsmeType;

/*
 * The column ids of R_DService table
 */
typedef enum Column_ID_Of_R_DServiceTable {
	DB_SERVICEID,
	DB_SCP1PC,
	DB_SCP1SSN,
	DB_SCP2PC,
	DB_SCP2SSN,
	DB_TYPE,
	DB_BLOCK,
	DB_RESERVED
} eColumn_ID_Of_R_DServiceTable_t;

#define DB_SERVICEID_SIZE   1
#define DB_SCP_PC_SIZE      3
#define DB_SCP_SSN_SIZE     1
#define DB_TYPE_SIZE        1
#define DB_BLOCK_SIZE       1
#define DB_S_RESERVED_SIZE  5

typedef struct {
	u_char   dbServiceId[DB_SERVICEID_SIZE]; //1
	u_char   dbScp1Pc[DB_SCP_PC_SIZE]; //3
	u_char   dbScp1Ssn[DB_SCP_SSN_SIZE]; //1
	u_char   dbScp2Pc[DB_SCP_PC_SIZE]; //3
	u_char   dbScp2Ssn[DB_SCP_SSN_SIZE]; //1
	u_char   dbType[DB_TYPE_SIZE]; //1
	u_char   dbBlock[DB_BLOCK_SIZE]; //1
	u_char   dbSReserved[DB_S_RESERVED_SIZE]; //5
} stRDServiceTuple;

typedef struct {
	u_char	ucServiceId;
	u_int	uScp1Pc;
	u_int	uScp2Pc;
	u_char	ucType;
	u_char	ucBlock;
	u_char	cpSReserved[DB_S_RESERVED_SIZE];
} stRDServiceType;


/*
 *   ScheduleHistory struct   : 1998.08.10   hkkang
 */
typedef struct {
	int nAttemptCnt;
	time_t lnLastRetry;
} ScheduleHistoryType;

//#define SHMDBMS_Q           0x401 // Only Sangyong
#define MAX_RECOVERY_RECORD 10000
#define SHMQ_BAD_POSITION   201
//#define SHMQ_BAD_POSITION   201

/********************************* Only Dual *********************/
typedef struct _stsmdcMas {
	stRDActiveTuple		stRDActiveRec;
	struct _stsmdcMas   *next;

} stsmdcMas;
/****************************************************************/

/************************* Plus & Spam **************************/
// (_PLUS_VER_ == V2-0-1) // 2004.04.20 expand version.

typedef enum Column_ID_Of_R_DPlusTable {
	DB_SUBSCRIBER_NO_ID,
	DB_SERVICE_FLAG,
	DB_SERVICE_CODE_FLAG,
	DB_EXPAND_FLAG,
	DB_SPAM_PATTERN_PLAG
} eColumn_ID_Of_R_DPlusTable_t;

#	define DB_SUBSCRIBER_NO_SIZE   DB_DEST_MIN_SIZE
#	define DB_SERVICE_FLAG_SIZE		2	// 16 bits
#	define DB_SERVICE_CODE_SIZE		1	// 16 bits
#	define DB_EXPAND_FLAG_SIZE		1	// 16 bits
//#	define DB_SPAM_FLAG_SIZE		4	// 32 bits

/* define R_DPLUS - SMS Plus Subscriber Info DB */
typedef struct {
	/* 0193331111, 1944441111*/
	u_char  dbSubscriberNo[DB_SUBSCRIBER_NO_SIZE]; //12
	/* 00000001(MsgForward) | 00000010(BlockMIN) = SMSPlus */
	u_char  dbServiceFlag[DB_SERVICE_FLAG_SIZE]; //2
	/* 10000000(POLY) */
	u_char  dbServiceCode[DB_SERVICE_CODE_SIZE]; //1
	/* No used. for future.... */
	u_char  dbExpandFlag[DB_EXPAND_FLAG_SIZE]; //1
	/* 00000001(700) | 00000010(060) = SPAM CallBack Block */
	u_char  dbSpamFlag[DB_SPAM_FLAG_SIZE]; //4
} stRDPlusTuple;

typedef struct {
	u_char	cpSubscriberNo[DB_SUBSCRIBER_NO_SIZE];
	u_short	usServiceFlag;
	u_short	usExpandFlag;
	u_int	uSpamFlag;
} stRDPlusType;

typedef enum Column_ID_Of_R_DSpamTable {
	DB_SPAM_ID,				// Key
	DB_SPAM_PATTERN,		// 700, 060 etc...
	DB_SPAM_PATTERN_TYPE,	// Pre, Midle, ...
	DB_SPAM_PATTERN_LENGTH,	// pattern len 3. 4.
	DB_SPAM_RESERVED
} eColumn_ID_Of_R_DSpamTable_t;

#define DB_SPAM_ID_SIZE             MDB_NUMBER_SIZE // Key
#define DB_SPAM_PATTERN_SIZE        DB_CALLBACK_NUMBER_SIZE // 700, 060 etc.
#define DB_SPAM_PATTERN_TYPE_SIZE   MDB_FLAG_SIZE  // Pre, Midle, ...
#define DB_SPAM_PATTERN_LENGTH_SIZE MDB_LEN_SIZE  // pattern len 3. 4.

/* define R_DSPAM - SPAM Min Pattern 700, 060 etc...*/
typedef struct {
	u_char  dbSpamID[DB_SPAM_ID_SIZE]; //4
	u_char  dbSpamPattern[DB_SPAM_PATTERN_SIZE]; //12
	u_char  dbSpamType[DB_SPAM_PATTERN_TYPE_SIZE]; //1
	u_char  dbSpamLength[DB_SPAM_PATTERN_LENGTH_SIZE];//1
	u_char	dbReserved[MDB_HALF_NUM_SIZE]; // 2
} stRDSpamTuple;


typedef struct {
	u_int	uSpamID;
	u_char	cpSpamPattern[DB_SPAM_PATTERN_SIZE];
	u_char	ucSpamType;
	u_char	ucSpamLength;
	u_short	usReserved;
} stRDSpamType;
/********************************************************************/

//
//	SKT FP Service 2006.09.15 bj bjlee
//
#define	DB_NODE_ADDRESS_SIZE	4
#define	DB_NODE_TYPE_SIZE		1
#define	DB_NODE_DESC_SIZE		16
#define	DB_NODE_RETRY_FRAME_SIZE	1

typedef	enum Column_ID_Of_R_DNodeType {
	DB_NODE_ADDRESS,
	DB_NODE_TYPE,
	DB_NODE_DESC,
	DB_NODE_RETRY_FRAME
} eColumn_ID_Of_R_DNodType_t;

// ############################################ cscf info 
typedef enum Column_ID_Of_R_DCscfInfoTable {
	DB_CSCF_NUM,
	DB_CSCF_TYPE,
	DB_CSCF_ONOFF,
	DB_CSCF_STATUS,
	DB_CSCF_IP,
	DB_CSCF_PORT,
	DB_CSCF_DESC
} eColumn_ID_Of_R_DCscfInfoTable_t;

#define DB_CSCF_NUM_SIZE 			4
#define DB_CSCF_TYPE_SIZE 			1
#define DB_CSCF_ONOFF_SIZE 			1
#define DB_CSCF_STATUS_SIZE 		1
#define DB_CSCF_IP_SIZE 			4
#define DB_CSCF_PORT_SIZE 			2
#define DB_CSCF_DESC_SIZE 			24

#define DB_GROUP_CSCF_SIZE 			4
#define DB_CSCF_CAPA_SIZE 			1
#define DB_CSCF_CAPA_SIZE2 			4
#define DB_CSCF_FLAF_SIZE 			1

typedef struct {
	u_char	dbCscfNum[DB_CSCF_NUM_SIZE]; 			// 4
	u_char  dbCscfType[DB_CSCF_TYPE_SIZE]; 			// 1
	u_char  dbCscfOnOff[DB_CSCF_ONOFF_SIZE]; 		// 1
	u_char  dbCscfStatus[DB_CSCF_STATUS_SIZE]; 		// 1
	u_char	dbCscfIP[DB_CSCF_IP_SIZE]; 				// 4
	u_char  dbCscfPort[DB_CSCF_PORT_SIZE]; 			// 2
	u_char  dbDesc[DB_CSCF_DESC_SIZE]; 				// 24
	u_char  dbReserved[3]; 							// 3
} stRDCscfInfoTuple;
// cscf info

// ############################################ cscf group info 
typedef enum Column_ID_Of_R_DGroupCscfInfoTable {
	DB_G_GROUP_CSCF_NUM,
	DB_G_CSCF_CAPA
} eColumn_ID_Of_R_DGroupCscfInfoTable_t;

typedef struct {
	u_char	dbGrpCscfNum[DB_GROUP_CSCF_SIZE+DB_CSCF_NUM_SIZE]; 		// 8
	u_char  dbCscfCapa[DB_CSCF_CAPA_SIZE2]; 						// 4
} stRDGroupCscfInfoTuple;

typedef struct {
	u_int 	dbGrpNum; 			// 4
	u_int 	dbCscfNum; 			// 4
	u_int 	dbCscfCapa; 		// 4
} stRDGroupCscfInfoData;
// cscf group info

// ############################################ cscf monitor info 
typedef enum Column_ID_Of_R_DCscfMonitorInfoTable {
	DB_M_CSCF_NUM,
	DB_M_CSCF_FLAG,
	DB_M_CSCF_RESERVED
} eColumn_ID_Of_R_DCscfMonitorInfoTable_t;

typedef struct {
	u_char  dbCscfNum[DB_CSCF_NUM_SIZE]; 			// 4
	u_char  dbCscfFlag[DB_CSCF_FLAF_SIZE]; 			// 1
	u_char  dbReserved[3]; 							// 3
} stRDCscfMonitorInfoTuple;
// cscf monitor info

// ############################################ udm info
typedef enum Column_ID_Of_R_DUdmInfoTable {
	DB_UDM_NUM,
	DB_UDM_ONOFF,
	DB_UDM_MODE,
	DB_UDM_IP,
	DB_UDM_PORT,
	DB_UDM_LPORT,
	DB_UDM_ISDN,
	DB_UDM_INTERFACE,
	DB_UDM_HOST,
	DB_UDM_REALM,
	DB_UDM_DESC
} eColumn_ID_Of_R_DUdmInfoTable_t;

#define DB_UDM_NUM_SIZE 			4
#define DB_UDM_ONOFF_SIZE 			1
#define DB_UDM_MODE_SIZE 			1 
#define DB_UDM_IP_SIZE 				4
#define DB_UDM_PORT_SIZE 			2
#define DB_UDM_ISDN_SIZE 			16
#define DB_UDM_HOST_SIZE 			64
#define DB_UDM_REALM_SIZE 			64
#define DB_UDM_DESC_SIZE 			20

#define DB_GRP_STATUS_SIZE 		16
#define DB_UDMBK_GRP_DESC_SIZE 	24 
#define DB_UDMBK_RESERVED_SIZE 	3
#define DB_UDMBK_INTERFACE_SIZE 1
#define DB_UDM_CAPA_SIZE 		1
#define DB_UDM_CAPA_SIZE2 		4

typedef struct {
	u_char	dbUdmNum[DB_UDM_NUM_SIZE]; 				// 4
	u_char  dbUdmOnOff[DB_UDM_ONOFF_SIZE]; 			// 1
	u_char  dbUdmMode[DB_UDM_MODE_SIZE]; 			// 1
	u_char	dbUdmIp[DB_UDM_IP_SIZE]; 				// 4
	u_char  dbUdmPort[DB_UDM_PORT_SIZE]; 			// 2
	u_char  dbUdmLport[DB_UDM_PORT_SIZE]; 			// 2
	u_char  dbIsdn[DB_UDM_ISDN_SIZE]; 				// 16
	u_char  dbInterface[DB_UDM_MODE_SIZE]; 			// 1
	u_char  dbUdmHost[DB_UDM_HOST_SIZE]; 			// 64
	u_char  dbUdmRealm[DB_UDM_REALM_SIZE]; 			// 64
	u_char  dbDesc[DB_UDM_DESC_SIZE]; 				// 20 
	u_char dbReserved[1];
} stRDUdmInfoTuple;
// udm info

// ############################################# udm group 
typedef enum Column_ID_Of_R_DGroupUdmTable {
	DB_G_UDM_GROUP_NUM,
	DB_G_UDM_CAPA,
	DB_UDM_GRP_INTERFACE
} eColumn_ID_Of_R_DGroupUdmTable_t;

typedef struct {
	u_char	dbGrpUdmNum[DB_GROUP_UDM_SIZE+DB_UDM_NUM_SIZE]; 	// 8
	u_char	dbUdmCapa[DB_UDM_CAPA_SIZE2]; 						// 4 
	u_char 	dbInterface[DB_UDM_MODE_SIZE];
	u_char  dbReserved[3];
} stRDGroupUdmTuple;

typedef struct {
	u_int 	dbGrpNum; 		// 4
	u_int 	dbUdmNum; 		// 4
	u_int 	dbUdmCapa; 	// 4 
	u_char 	dbInterface[DB_UDM_MODE_SIZE];
	u_char 	dbReserved[3];
} stRDGroupUdmData;
// udm group

// ############################################# udm bk group
typedef enum Column_ID_Of_R_DUdmBkGrpTable {
	DB_UDMBK_GRPNUM,
	DB_UDMBK_GRPSTAT,
	DB_UDMBK_BKGRPNUM,
	DB_UDMBK_BKGRPSTAT,
	DB_UDMBK_SELECTION,
	DB_UDMBK_BKTIMER,
	DB_UDMBK_INTERFACE,
	DB_UDMBK_DESC,
	DB_UDMBK_RESERVED,
} eColumn_ID_Of_R_DUdmBkGrp_t;

typedef struct {
	u_char	dbGrpNum[DB_GROUP_UDM_SIZE];
	u_char  dbGrpStat[DB_GRP_STATUS_SIZE];
	u_char	dbBkGrpNum[DB_GROUP_UDM_SIZE];
	u_char  dbBkGrpStat[DB_GRP_STATUS_SIZE];
	u_char	dbSelection[DB_GROUP_UDM_SIZE];
	u_char  dbBkTimer[MDB_TIME_TYPE_SIZE];	
	u_char  dbInterface[DB_UDMBK_INTERFACE_SIZE];
	u_char	dbDesc[DB_UDMBK_GRP_DESC_SIZE];
	u_char  dbReserved[DB_UDMBK_RESERVED_SIZE];
} stRDUdmBkGrpTuple;

typedef struct {
	u_int dbGrpNum;
	char  dbGrpStat[DB_GRP_STATUS_SIZE];
	u_int dbBkGrpNum;
	char  dbBkGrpStat[DB_GRP_STATUS_SIZE];
	u_int dbSelection;
	u_int dbBkTimer;
	u_char dbInterface;
	char dbDesc[DB_UDMBK_GRP_DESC_SIZE];
	u_char dbReserved[DB_UDMBK_RESERVED_SIZE];
} stRDUdmBkGrpData;
// udm bk group

typedef	struct {
	u_char	dbNodeAddress[DB_NODE_ADDRESS_SIZE];	//	4
	u_char	dbNodeType[DB_NODE_TYPE_SIZE];			//	1
	u_char	dbNodeDesc[DB_NODE_DESC_SIZE];			//	16
	u_char	dbNodeRetryFrame[DB_NODE_RETRY_FRAME_SIZE];	// 1
} stRDNodeTypeTuple;	// Plus DBMS

/* Traffic Overload Control  TOC*/

typedef enum Column_ID_Of_R_DTrafficControlTable {
	DB_TOC_CONTROL_CODE,
	DB_TOC_CONTROL_FLAG,
	DB_TOC_RESERVED,
	DB_TOC_OVERLOAD_COUNT,
	DB_TOC_TOTAL_TRAFFIC_COUNT,
	DB_TOC_WAITING_COUNT,
	DB_TOC_DESCRIPTION
} eColumn_ID_Of_R_DTrafficControlTable_t;

#define DB_TOC_CONTROL_CODE_SIZE            4
#define DB_TOC_CONTROL_FLAG_SIZE            1
#define DB_TOC_RESERVED_SIZE                3
#define DB_TOC_OVERLOAD_COUNT_SIZE          4
#define DB_TOC_TOTAL_TRAFFIC_COUNT_SIZE     4
#define DB_TOC_WAITING_COUNT_SIZE           4
#define DB_TOC_DESCRIPTION_SIZE             20

typedef struct {
	u_char  dbControlCode[DB_TOC_CONTROL_CODE_SIZE]; // 4
	u_char  dbControlFlag[DB_TOC_CONTROL_FLAG_SIZE]; // 1
	u_char  dbReserved[DB_TOC_RESERVED_SIZE]; // 3
	u_char  dbOverloadCount[DB_TOC_OVERLOAD_COUNT_SIZE]; // 4
	u_char  dbTotalTrafficCount[DB_TOC_TOTAL_TRAFFIC_COUNT_SIZE]; // 4
	u_char  dbWaitingCount[DB_TOC_WAITING_COUNT_SIZE]; // 4
	u_char  dbDescription[DB_TOC_DESCRIPTION_SIZE]; // 20
} stRDTrafficControlTuple; // Total : 40 bytes


typedef struct {
	u_char	ucNodeAddress[DB_NODE_ADDRESS_SIZE];
	u_char	nNodeType;
	u_char	ucNodeDesc[DB_NODE_DESC_SIZE];
	u_char	ucNodeRetryFrame;
} stNodeType;

/////////////////// TEXT CONFIG /////////////////// DR430
typedef enum Column_ID_Of_R_DTextConfig {
	DB_SERVICE_CODE,
	DB_SERVICE_GROUP_NAME,
	DB_SERVICE_TYPE_NAME,
	DB_SERVICE_TEXT_LENGTH,
	DB_SERVICE_TEXT
} eColumn_ID_Of_R_DTextConfig;

#define DB_MDTC_SERVICE_CODE_SIZE 4
#define DB_MDTC_SERVICE_TYPE_SIZE 2
#define DB_MDTC_SERVICE_NAME_SIZE 24
#define DB_MDTC_SERVICE_TEXT_LENGTH_SIZE 1
#define DB_MDTC_SERVICE_TEXT_SIZE 91
#define DB_MDTC_SERVICE_TEXT_DOUBLE_SIZE (DB_MDTC_SERVICE_TEXT_SIZE*2)
#define CP949_ENTER_SIZE 1
#define CP949_ENTER_HEXA 0x0a
typedef struct {
	u_char dbServiceCode[DB_MDTC_SERVICE_CODE_SIZE];
	u_char dbServiceGroupName[DB_MDTC_SERVICE_NAME_SIZE];
	u_char dbServiceTypeName[DB_MDTC_SERVICE_NAME_SIZE];
	u_char dbTextLength[DB_MDTC_SERVICE_TEXT_LENGTH_SIZE];
	u_char dbText[DB_MDTC_SERVICE_TEXT_SIZE];
} stRDTextConfig; //PLUS DBMS

typedef struct {
	u_short 	dbServiceGroup;
	u_short 	dbServiceType;
	char		dbServiceGroupName[DB_MDTC_SERVICE_NAME_SIZE];
	char		dbServiceTypeName[DB_MDTC_SERVICE_NAME_SIZE];
	u_char		dbTextLength[DB_MDTC_SERVICE_TEXT_LENGTH_SIZE];
	char		dbText[DB_MDTC_SERVICE_TEXT_SIZE];
} stRDTextConfigData;

//FR610 pkg callback noti text data
typedef struct {
	int 	noti_len;
	u_char 	noti_text[DB_MDTC_SERVICE_TEXT_DOUBLE_SIZE];
} stCallbackNotiData;

// End TextConfig
//////////////////// Added by baram from here //////////////////////
//////////////////// Active DBMS Start /////////////////////////////

/* Active Head Table, Old MinMask Table */
typedef enum Column_ID_Of_R_DActiveHeadTable {
	DB_AH_MIN,
	DB_AH_TIME,
	DB_AH_MSGID,
	DB_AH_STARTMSGID,
	DB_AH_MANAGEMENTMSGID,
	DB_AH_EXPRESSMSGID,
	DB_AH_NORMALMSGID,
	DB_AH_RESERVATIONMSGID,
	DB_AH_FINISHSTARTMSGID,
	DB_AH_FINISHENDMSGID,
	DB_AH_MSGCNT,
	DB_AH_ATTEMPTS,
	DB_AH_STARTTIME,
	DB_AH_NEXTRETRY,
	DB_AH_FRAME,
	DB_AH_PLAN,
	DB_AH_STEP,
	DB_AH_REPEAT,
	DB_AH_STATUS,
	DB_AH_PEER_STATUS,
	DB_AH_MANAGEMENTSTATUS,
	DB_AH_FAILREASON,
	DB_AH_ERRORHISTORY
} eColumn_ID_Of_R_DActiveHeadTable_t;

#define	DB_AH_MIN_SIZE					12
#define	DB_AH_TIME_SIZE					4
#define	DB_AH_MSGID_SIZE				4
#define	DB_AH_STARTMSGID_SIZE			4
#define	DB_AH_MANAGEMENTMSGID_SIZE		4
#define	DB_AH_EXPRESSMSGID_SIZE			4
#define	DB_AH_NORMALMSGID_SIZE			4
#define	DB_AH_RESERVATIONMSGID_SIZE		4
#define	DB_AH_FINISHSTARTMSGID_SIZE		4
#define	DB_AH_FINISHENDMSGID_SIZE		4
#define	DB_AH_MSGCNT_SIZE				4
#define	DB_AH_ATTEMPTS_SIZE				4
#define	DB_AH_STARTTIME_SIZE			4
#define	DB_AH_NEXTRETRY_SIZE			4
#define	DB_AH_FRAME_SIZE				4
#define	DB_AH_PLAN_SIZE					4
#define	DB_AH_STEP_SIZE					4
#define	DB_AH_REPEAT_SIZE				4
#define	DB_AH_STATUS_SIZE				4
#define	DB_AH_PEER_STATUS_SIZE			4
#define	DB_AH_MANAGEMENTSTATUS_SIZE		4
#define	DB_AH_FAILREASON_SIZE			4
#define	DB_AH_ERRORHISTORY_SIZE			20

typedef struct {
	char 	dbMin[DB_AH_MIN_SIZE];
	char  	dbTime[DB_AH_TIME_SIZE];
	u_char	dbMsgId[DB_AH_MSGID_SIZE];
	u_char	dbStartMsgId[DB_AH_STARTMSGID_SIZE];
	u_char	dbManagementMsgId[DB_AH_MANAGEMENTMSGID_SIZE];
	u_char	dbExpressMsgId[DB_AH_EXPRESSMSGID_SIZE];
	u_char	dbNormalMsgId[DB_AH_NORMALMSGID_SIZE];
	u_char	dbReservationMsgId[DB_AH_RESERVATIONMSGID_SIZE];
	u_char	dbFinishStartMsgId[DB_AH_FINISHSTARTMSGID_SIZE];
	u_char	dbFinishEndMsgId[DB_AH_FINISHENDMSGID_SIZE];
	char	dbMsgCnt[DB_AH_MSGCNT_SIZE];
	char    dbAttempts[DB_AH_ATTEMPTS_SIZE];
	char    dbStartTime[DB_AH_STARTTIME_SIZE];
	char	dbNextRetry[DB_AH_NEXTRETRY_SIZE];
	char    dbFrame[DB_AH_FRAME_SIZE];
	char    dbPlan[DB_AH_PLAN_SIZE];
	char    dbStep[DB_AH_STEP_SIZE];
	char    dbRepeat[DB_AH_REPEAT_SIZE];
	char    dbStatus[DB_AH_STATUS_SIZE];
	char    dbPeerStatus[DB_AH_PEER_STATUS_SIZE];
	char    dbManagementStatus[DB_AH_MANAGEMENTSTATUS_SIZE];
	char    dbFailReason[DB_AH_FAILREASON_SIZE];
	char    dbpErrorHistory[DB_AH_ERRORHISTORY_SIZE];
} stActiveHeadTuple;

typedef struct {
	char 		cpMin[DB_AH_MIN_SIZE];
	u_int	unTime;
	u_int	uMsgId;
	u_int	uStartMsgId;
	u_int	uManagementMsgId;
	u_int	uExpressMsgId;
	u_int	uNormalMsgId;
	u_int	uReservationMsgId;
	u_int	uFinishStartMsgId;
	u_int	uFinishEndMsgId;
	int		nMsgCnt;
	int		nAttempts;
	u_int	unStartTime;
	u_int	unNextRetry;
	int		nFrame;
	int		nPlan;
	int		nStep;
	int		nRepeat;
	int		nStatus;
	int		nPeerStatus;
	int		nManagementStatus;
	int		nFailReason;
	char    cpErrorHistory[DB_AH_ERRORHISTORY_SIZE];
} stActiveHeadType;


typedef enum Column_ID_Of_R_DRetryTable {
	DB_RETRY_MESSAGE_ID,
	DB_RETRY_DEST_MDN,
	DB_RETRY_TIME,
	DB_RETRY_MDB_TYPE,
	DB_RETRY_FORWARD_ID,
	DB_RETRY_BACKWARD_ID
} eColumn_ID_Of_R_DRetryTable_t;

#define	DB_RETRY_MESSAGE_ID_SIZE	4
#define	DB_RETRY_DEST_MDN_SIZE		12
#define	DB_RETRY_TIME_SIZE			4
#define	DB_RETRY_MDB_TYPE_SIZE		4
#define	DB_RETRY_FORWARD_ID_SIZE	4
#define	DB_RETRY_BACKWARD_ID_SIZE	4

/* define R_DRETRY/R_DRESERVE in ADBMS */
typedef struct {
	u_char	dbMessageID[DB_RETRY_MESSAGE_ID_SIZE]; // 4
	u_char	dbDestMDN[DB_RETRY_DEST_MDN_SIZE]; // 12
	char	dbTryTime[DB_RETRY_TIME_SIZE]; // 4
	u_char	dbMDBType[DB_RETRY_MDB_TYPE_SIZE]; // 4
	u_char	dbForwardID[DB_RETRY_FORWARD_ID_SIZE]; // 4
	u_char	dbBackwardID[DB_RETRY_BACKWARD_ID_SIZE]; // 4
} stRDRetryTuple; // Total : 32 bytes

typedef stRDRetryTuple stRDReserveTuple;

typedef struct {
	u_int	unMessageID;
	u_char		cpDestMDN[DB_RETRY_DEST_MDN_SIZE];
	u_int	unTryTime;
	u_int	unMDBType;
	u_int	unForwardID;
	u_int	unBackwardID;
} stRDRetryType;

typedef stRDRetryType stRDReserveType;

typedef enum Column_ID_Of_R_DRetryHeadTable {
	DB_RH_TIME,
	DB_RH_MSG_COUNT,
	DB_RH_PRETIME_LINK,
	DB_RH_NEXTTIME_LINK,
	DB_RH_FIRST_MSG_ID,
	DB_RH_LAST_MSG_ID
} eColumn_ID_Of_R_DRetryHeadTable_t;

#define	DB_RH_TRY_TIME_HEAD		0x7FFFFFFF
#define	DB_RH_TIME_SIZE				4
#define	DB_RH_MSG_COUNT_SIZE		4
#define	DB_RH_PRETIME_LINK_SIZE		4
#define	DB_RH_NEXTTIME_LINK_SIZE	4
#define	DB_RH_FIRST_MSG_ID_SIZE		4
#define	DB_RH_LAST_MSG_ID_SIZE		4

/* define R_DRETRYHEAD/R_DRESERVEHEAD in ADBMS */
typedef struct {
	char	dbTryTime[DB_RH_TIME_SIZE]; // 4
	u_char	dbMsgCnt[DB_RH_MSG_COUNT_SIZE];//4
	char	dbPreTimeLink[DB_RH_PRETIME_LINK_SIZE]; // 4
	char	dbNextTimeLink[DB_RH_NEXTTIME_LINK_SIZE]; // 4
	u_char	dbStartMessageId[DB_RH_FIRST_MSG_ID_SIZE]; // 4
	u_char	dbEndMessageId[DB_RH_LAST_MSG_ID_SIZE]; // 4
} stRDRetryHeadTuple; // Total : 24 bytes

typedef stRDRetryHeadTuple stRDReserveHeadTuple;

typedef struct {
	u_int 	unTryTime;
	u_int	unMsgCnt;
	u_int	unPreTimeLink;
	u_int	unNextTimeLink;
	u_int	unStartMessageId;
	u_int	unEndMessageId;
} stRDRetryHeadType;

typedef stRDRetryHeadType stRDReserveHeadType;

typedef enum Column_ID_Of_R_DRetryFrameTable {
	DB_RF_RETRYFRAME_ID,
	DB_RF_TIMEOUT,
	DB_RF_STEP_CNT,
	DB_RF_TIME0,
	DB_RF_REPEAT0,
	DB_RF_TIME1,
	DB_RF_REPEAT1,
	DB_RF_TIME2,
	DB_RF_REPEAT2,
	DB_RF_TIME3,
	DB_RF_REPEAT3,
	DB_RF_TIME4,
	DB_RF_REPEAT4,
	DB_RF_TIME5,
	DB_RF_REPEAT5,
	DB_RF_TIME6,
	DB_RF_REPEAT6,
	DB_RF_TIME7,
	DB_RF_REPEAT7,
	DB_RF_TIME8,
	DB_RF_REPEAT8,
	DB_RF_TIME9,
	DB_RF_REPEAT9,
	DB_RF_FRAME_FORWARD_ID,
	DB_RF_FRAME_BACKWARD_ID
} eColumn_ID_Of_R_DRetryFrameTable_t;

#define	DB_RF_RETRYFRAME_ID_SIZE	4
#define	DB_RF_TIMEOUT_SIZE			4
#define	DB_RF_STEP_CNT_SIZE			4
#define	DB_RF_TIME_SIZE				4
#define	DB_RF_REPEAT_SIZE			4
#define	DB_RF_FRAME_FORWARD_ID_SIZE		4
#define	DB_RF_FRAME_BACKWARD_ID_SIZE	4
#define	DB_RF_DESCRIPTION_SIZE			20

typedef struct {
	/*
	 * 	 * RtryFrame(2byte) + ErrorCode(2byte)
	 * 	 	 *		[3][2]			[1][0]
	 * 	 	 	 * dbRetryFrameID[4]
	 * 	 	 	 	 */
	u_char	dbRetryFrameID[DB_RF_RETRYFRAME_ID_SIZE]; // 4
	u_char	dbTimeOut[DB_RF_TIMEOUT_SIZE];		// 4
	u_char	dbStepCnt[DB_RF_STEP_CNT_SIZE];	// 4
	u_char	dbTime0[DB_RF_TIME_SIZE];		// 4
	u_char	dbRepeat0[DB_RF_REPEAT_SIZE]; 	// 4
	u_char	dbTime1[DB_RF_TIME_SIZE];		// 4
	u_char	dbRepeat1[DB_RF_REPEAT_SIZE];	// 4
	u_char	dbTime2[DB_RF_TIME_SIZE];		// 4
	u_char	dbRepeat2[DB_RF_REPEAT_SIZE];	// 4
	u_char	dbTime3[DB_RF_TIME_SIZE];		// 4
	u_char	dbRepeat3[DB_RF_REPEAT_SIZE];	// 4
	u_char	dbTime4[DB_RF_TIME_SIZE];		// 4
	u_char	dbRepeat4[DB_RF_REPEAT_SIZE];	// 4
	u_char	dbTime5[DB_RF_TIME_SIZE];		// 4
	u_char	dbRepeat5[DB_RF_REPEAT_SIZE];	// 4
	u_char	dbTime6[DB_RF_TIME_SIZE];		// 4
	u_char	dbRepeat6[DB_RF_REPEAT_SIZE];	// 4
	u_char	dbTime7[DB_RF_TIME_SIZE];		// 4
	u_char	dbRepeat7[DB_RF_REPEAT_SIZE];	// 4
	u_char	dbTime8[DB_RF_TIME_SIZE];		// 4
	u_char	dbRepeat8[DB_RF_REPEAT_SIZE];	// 4
	u_char	dbTime9[DB_RF_TIME_SIZE];		// 4
	u_char	dbRepeat9[DB_RF_REPEAT_SIZE];	// 4
	u_char	dbForwardID[DB_RF_FRAME_FORWARD_ID_SIZE];	// 4
	u_char	dbBackwardID[DB_RF_FRAME_BACKWARD_ID_SIZE]; // 4
} stRDRetryFrameTuple; // total : 92 bytes

typedef struct {
	u_int	uRetryFrameID;
	u_int	uTimeOut;
	u_int	uStepCnt;
	u_int	uTime0;
	u_int	uRepeat0;
	u_int	uTime1;
	u_int	uRepeat1;
	u_int	uTime2;
	u_int	uRepeat2;
	u_int	uTime3;
	u_int	uRepeat3;
	u_int	uTime4;
	u_int	uRepeat4;
	u_int	uTime5;
	u_int	uRepeat5;
	u_int	uTime6;
	u_int	uRepeat6;
	u_int	uTime7;
	u_int	uRepeat7;
	u_int	uTime8;
	u_int	uRepeat8;
	u_int	uTime9;
	u_int	uRepeat9;
	u_int	uForwardID;
	u_int	uBackwardID;
} stRDRetryFrameType; // total : 92 bytes

/* SMPPMESSAGE in ACTIVE */
typedef enum Column_ID_Of_R_DSmppMessage{
	DB_SMPP_MESSAGE_KEY,
	DB_SMSC_MESSAGE_ID,
	DB_SMPP_EXPIRE_TIME,
	SMPP_MESSAGE_ID_TBL_ATTR_COUNT
} eColumn_ID_Of_R_DSmppMessage_t;

typedef struct {
	u_char dbSmppMessageId[DB_SMPP_MESSAGE_ID_SIZE];	// 12
	u_char dbSourceMIN[MDB_MIN_SIZE]; 					// 12
	u_char dbDestMIN[MDB_MIN_SIZE]; 					// 12
} stSmppMessageKeyTuple;

typedef struct {
	u_char dbSmppMessageId[DB_SMPP_MESSAGE_ID_SIZE];	// 12
	u_char dbSourceMIN[MDB_MIN_SIZE]; 					// 12
	u_char dbDestMIN[MDB_MIN_SIZE]; 					// 12
	u_char dbSmscMessageId[DB_MESSAGE_ID_SIZE]; 		// 4
	u_char dbSmppExpireTime[MDB_TIME_SIZE];				// 7 
	u_char dbReserved[1]; 								// 1
} stSmppMessageTuple; 									// 48

//////////////////// Active DBMS End /////////////////////////////

//////////////////// Plus DBMS Start/////////////////////////////
typedef enum Column_ID_Of_R_DEsmeServCfgTable {
	DB_ESC_PORT_ID,
	DB_ESC_ACT_FLAG,
	DB_ESC_LISTENSOCK_ID,
	DB_ESC_RUN_APP,
	DB_ESC_DESCRIPTION,
	DB_ESC_CREATE_TIME,
	DB_ESC_UPDATE_TIME
} eColumn_ID_Of_R_DEsmeServCfgTable_t;

#define	DB_ESC_PORT_ID_SIZE			2
#define	DB_ESC_ACT_FLAG_SIZE		2
#define	DB_ESC_LISTENSOCK_ID_SIZE	4
#define	DB_ESC_RUN_APP_SIZE			24
#define	DB_ESC_DESCRIPTION_SIZE		32
#define	DB_ESC_CREATE_TIME_SIZE		4
#define	DB_ESC_UPDATE_TIME_SIZE		4

/*  define R_DEmseServCfg in SDBMS - ESME Server Info */
/*  define R_DSERVER in SDBMS - ESME Server config */
typedef struct {
	u_char	dbPortId[DB_ESC_PORT_ID_SIZE]; // 2
	char	dbActFlag[DB_ESC_ACT_FLAG_SIZE]; // 2
	char	dbListenSockId[DB_ESC_LISTENSOCK_ID_SIZE]; // 4
	char	dbRunApp[DB_ESC_RUN_APP_SIZE]; // 24
	char	dbDescription[DB_ESC_DESCRIPTION_SIZE]; //32
	char	dbCreateTime[DB_ESC_CREATE_TIME_SIZE]; // 4
	char	dbUpdateTime[DB_ESC_UPDATE_TIME_SIZE]; // 4
} stRDEsmeServCfgTuple; // Total : 40 bytes

typedef struct {
	u_short	usPortId;
	short		usActFlag;
	int		nListenSockId;
	char		cpRunApp[DB_ESC_RUN_APP_SIZE]; // 24
	char		cpDescription[DB_ESC_DESCRIPTION_SIZE]; //32
	u_int	unCreateTime;
	u_int	unUpdateTime;
} stRDEsmeServCfgType; // Total : 40 bytes

typedef enum Column_ID_Of_R_DEsmeSystemIdTable {
	DB_SYSID_ESME_ID,
	DB_SYSID_PROTOCOL,
	DB_SYSID_PORT_ID,
	DB_SYSID_PEER_IP,
	DB_SYSID_IP_TYPE,
	DB_SYSID_ESME_CLASS,
	DB_SYSID_EXPRESS_FLAG,
	DB_SYSID_ACT_FLAG,
	DB_SYSID_MAX_TRANSMITTER,
	DB_SYSID_MAX_RECEIVER,
	DB_SYSID_MAX_TRANCEIVER,
	DB_SYSID_ACT_TRANSMITTER,
	DB_SYSID_ACT_RECEIVER,
	DB_SYSID_ACT_TRANCEIVER,
	DB_SYSID_SYS_ID,
	DB_SYSID_PASSWD,
	DB_SYSID_CREATETIME,
	DB_SYSID_UPDATETIME,
	DB_SYSID_ESMEMONFLAG,
	DB_SYSID_ESMEQUETYPE,
	DB_SYSID_ESMEQUEUECOUNT,
	DB_SYSID_DCSUSEFLAG,
	// DR710 pkg by cbh
	DB_SYSID_MESSAGE_LENGTH,
	DB_SYSID_QUEUE_KEY,
	// DR710 pkg by cbh
	DB_SYSID_MAX_QUEUE_CNT,
	DB_SYSID_SYNCFLAG,
	DB_SYSID_ESME_BK,
	DB_SYSID_ESME_BK_FLAG,
	DB_SYSID_RESERVED
} eColumn_ID_Of_R_DEsmeSystemIdTable_t;

#define	DB_SYSID_ESME_ID_SIZE			4
#define	DB_SYSID_PROTOCOL_SIZE			2
#define	DB_SYSID_PORT_ID_SIZE			2
#define	DB_SYSID_PEER_IP_SIZE			4
#define	DB_SYSID_IP_TYPE_SIZE			1
#define	DB_SYSID_ESME_CLASS_SIZE		1
#define	DB_SYSID_EXPRESS_FLAG_SIZE		1
#define	DB_SYSID_ACT_FLAG_SIZE			1
#define	DB_SYSID_MAX_TRANSMITTER_SIZE	4
#define	DB_SYSID_MAX_RECEIVER_SIZE		4
#define	DB_SYSID_MAX_TRANCEIVER_SIZE	4
#define	DB_SYSID_ACT_TRANSMITTER_SIZE	4
#define	DB_SYSID_ACT_RECEIVER_SIZE		4
#define	DB_SYSID_ACT_TRANCEIVER_SIZE	4
#define	DB_SYSID_SYS_ID_SIZE			16
#define	DB_SYSID_PASSWD_SIZE			12
#define	DB_SYSID_CREATETIME_SIZE		4
#define	DB_SYSID_UPDATETIME_SIZE		4
#define DB_SYSID_ESMEMONFLAG_SIZE       1
#define DB_SYSID_ESME_QUEUE_TYPE_SIZE   1
#define DB_SYSID_ESME_QUEUE_COUNT_SIZE  2
#define DB_SYSID_DCSUSEFLAG_SIZE        4
// DR710 pkg by cbh
#define DB_SYSID_MESSAGE_LENGTH_SIZE    4
#define DB_SYSID_QUEUE_KEY_SIZE         4
// DR710 pkg by cbh
#define DB_SYSID_MAX_QUEUE_CNT_SIZE     2
#define DB_SYSID_SYNCFLAG_SIZE          1
#define	DB_SYSID_RESERVED_SIZE			4

/* FR220 */
#define DB_SYSID_ESME_QUEUE_TYPE_SIZE 1
#define DB_SYSID_ESME_QUEUE_COUNT_SIZE 2
#define DB_SYSID_ESME_BK_SIZE 			4
#define DB_SYSID_ESME_BK_FLAG_SIZE 		1

/*  define R_DSysIdInfo in SDBMS - ESME Client Config */
typedef struct {
	u_char	dbEsmeId[DB_SYSID_ESME_ID_SIZE]; // 4
	u_char	dbProtocol[DB_SYSID_PROTOCOL_SIZE]; // 2
	u_char	dbPortId[DB_SYSID_PORT_ID_SIZE]; // 2
	u_char	dbPeerIP[DB_SYSID_PEER_IP_SIZE]; // 4
	u_char	dbIPType[DB_SYSID_IP_TYPE_SIZE]; // 1
	u_char	dbEsmeClass[DB_SYSID_ESME_CLASS_SIZE]; // 1
	u_char	dbExpressFlag[DB_SYSID_EXPRESS_FLAG_SIZE]; // 1
	u_char	dbActFlag[DB_SYSID_ACT_FLAG_SIZE]; // 1
	u_char	dbMaxTransmitter[DB_SYSID_MAX_TRANSMITTER_SIZE]; // 4
	u_char	dbMaxReceiver[DB_SYSID_MAX_RECEIVER_SIZE]; // 4
	u_char	dbMaxTranceiver[DB_SYSID_MAX_TRANCEIVER_SIZE]; // 4
	u_char	dbActTransmitter[DB_SYSID_ACT_TRANSMITTER_SIZE]; // 4
	u_char	dbActReceiver[DB_SYSID_ACT_RECEIVER_SIZE]; // 4
	u_char	dbActTranceiver[DB_SYSID_ACT_TRANCEIVER_SIZE]; // 4
	char	dbSysId[DB_SYSID_SYS_ID_SIZE]; // 16
	char	dbPasswd[DB_SYSID_PASSWD_SIZE]; // 12
	char	dbCreateTime[DB_SYSID_CREATETIME_SIZE]; // 4
	char	dbUpdateTime[DB_SYSID_UPDATETIME_SIZE]; // 4
	char    dbEsmeMonFlag[DB_SYSID_ESMEMONFLAG_SIZE]; // 1

	char 	dbEsmeQueueType[DB_SYSID_ESME_QUEUE_TYPE_SIZE]; //1
	u_char dbEsmeQueueCount[DB_SYSID_ESME_QUEUE_COUNT_SIZE]; //2
	/* FR320 PKG */
	u_char  dbDcsUseFlag[DB_SYSID_DCSUSEFLAG_SIZE]; //4
	// DR710 pkg by cbh
	u_char  dbMessageLength[DB_SYSID_MESSAGE_LENGTH_SIZE]; //4 = 88
	u_char dbEsmeQueueKey[DB_SYSID_QUEUE_KEY_SIZE]; //4
	// DR710 pkg by cbh
	u_char		dbMaxQueueCnt[DB_SYSID_MAX_QUEUE_CNT_SIZE]; // 2 = 94
	u_char dbSyncFlag[DB_SYSID_SYNCFLAG_SIZE]; // 1
	u_char	dbEsmeIDBk[DB_SYSID_ESME_BK_SIZE]; // 4
	u_char	dbEsmeIDBkFlag[DB_SYSID_ESME_BK_FLAG_SIZE]; // 1
	u_char dbReserved[DB_SYSID_RESERVED_SIZE]; // 4
} stRDEsmeSystemIdTuple;

typedef struct {
	u_int	uEsmeId;
	u_short	uProtocol;
	u_short	uPortId;
	u_int	uPeerIP;
	u_char		ucIPType;
	u_char		ucEsmeClass;
	u_char		ucExpressFlag;
	u_char		ucActFlag;
	u_int	uMaxTransmitter;
	u_int	uMaxReceiver;
	u_int	uMaxTranceiver;
	u_int	uActTransmitter;
	u_int	uActReceiver;
	u_int	uActTranceiver;
	char		cpSysId[DB_SYSID_SYS_ID_SIZE]; // 16
	char		cpPasswd[DB_SYSID_PASSWD_SIZE]; // 12
	u_int	unCreateTime;
	u_int	unUpdateTime;
	u_char  	ucEsemMonFlag;

	u_char 	ucEsmeQueueType;
	short 	usEsmeQueueCount;
	/* FR320 */
	u_int     unDcsUseFlag;
	// DR710 pkg by cbh
	u_int   unMessageLength; // 2 = 88
	u_int    unEsmeQueueKey;
	// DR710 pkg by cbh
	u_short usMaxQueueCnt; // 2 = 94
	u_char 	ucSyncFlag; // 1
	u_int 	unEsmeIDBK;
	u_char 	ucEsmeIDBkFlag;
	u_char     ucReserved[DB_SYSID_RESERVED_SIZE]; // 4
} stRDEsmeSystemIdType;

typedef enum Column_ID_Of_R_DClientInfoTable {
	DB_CI_PID,
	DB_CI_ESME_ID,
	DB_CI_PORT_ID,
	DB_CI_PROTOCOL,
	DB_CI_PEER_IP,
	DB_CI_CONNECT_TIME,
	DB_CI_STATUS_TIME,
	DB_CI_SYS_ID,
	DB_CI_PASSWD,
	DB_CI_CONNECT_MODE,
	DB_CI_ESME_CLASS,
	DB_CI_RESERVED
} eColumn_ID_Of_R_DClientInfoTable_t;

#define	DB_CI_PID_SIZE					4
#define DB_CI_ESME_ID_SIZE				4
#define	DB_CI_PORT_ID_SIZE				2
#define	DB_CI_PROTOCOL_SIZE				2
#define	DB_CI_PEER_IP_SIZE				4
#define	DB_CI_CONNECT_TIMEE_SIZE		4
#define	DB_CI_STATUS_TIME_SIZE			4
#define	DB_CI_SYS_ID_SIZE				16
#define	DB_CI_PASSWD_SIZE				12
#define	DB_CI_CONNECT_MODE_SIZE			1
#define	DB_CI_ESME_CLASS_SIZE			1
#define	DB_CI_RESERVED_SIZE				2

/*  define R_DCLIENTINFO in SDBMS - ESME Connected Client Info */
typedef struct {
	char	dbPid[DB_CI_PID_SIZE]; // 4
	u_char	dbEsmeId[DB_CI_ESME_ID_SIZE]; // 4
	u_char	dbPortId[DB_CI_PORT_ID_SIZE]; // 2
	u_char	dbProtocol[DB_CI_PROTOCOL_SIZE]; // 2
	u_char	dbPeerIP[DB_CI_PEER_IP_SIZE]; // 4
	char	dbConnectTime[DB_CI_CONNECT_TIMEE_SIZE]; // 4
	char	dbStatusTime[DB_CI_STATUS_TIME_SIZE]; // 4
	char	dbSysId[DB_CI_SYS_ID_SIZE]; // 16
	char	dbPasswd[DB_CI_PASSWD_SIZE]; // 12
	u_char	dbConnectMode[DB_CI_CONNECT_MODE_SIZE]; // 1
	u_char	dbEsmeClass[DB_CI_ESME_CLASS_SIZE]; // 1
	u_char	dbReserved[DB_CI_RESERVED_SIZE]; // 2
} stRDEsmeClientInfoTuple; // Total : 56 bytes

typedef struct {
	pid_t	pPid;
	u_int	uEsmeId;
	u_short	usPortId;
	u_short	usProtocol;
	u_int	unPeerIP;
	u_int	unConnectTime;
	u_int	unStatusTime;
	char		cpSysId[DB_CI_SYS_ID_SIZE]; // 16
	char		cpPasswd[DB_CI_PASSWD_SIZE]; // 12
	u_char		ucConnectMode;
	u_char		ucEsmeClass;
	u_short 	usReserved;
} stRDEsmeClientInfoType; // Total : 56 bytes

typedef enum Column_ID_Of_R_DDMsgCtrlTable {
	DB_MC_DPC,
	DB_MC_LINK_STATUS,
	DB_MC_SAVE_FLAG,
	DB_MC_ASE0_TX_RATIO,
	DB_MC_ASE0_RX_RATIO,
	DB_MC_ASE0_TX_COUNT,
	DB_MC_ASE0_RX_COUNT,
	DB_MC_ASE1_TX_RATIO,
	DB_MC_ASE1_RX_RATIO,
	DB_MC_ASE1_TX_COUNT,
	DB_MC_ASE1_RX_COUNT,
	DB_MC_ASE2_TX_RATIO,
	DB_MC_ASE2_RX_RATIO,
	DB_MC_ASE2_TX_COUNT,
	DB_MC_ASE2_RX_COUNT,
	DB_MC_ASE3_TX_RATIO,
	DB_MC_ASE3_RX_RATIO,
	DB_MC_ASE3_TX_COUNT,
	DB_MC_ASE3_RX_COUNT,
	DB_MC_ASE4_TX_RATIO,
	DB_MC_ASE4_RX_RATIO,
	DB_MC_ASE4_TX_COUNT,
	DB_MC_ASE4_RX_COUNT,
	DB_MC_ASE5_TX_RATIO,
	DB_MC_ASE5_RX_RATIO,
	DB_MC_ASE5_TX_COUNT,
	DB_MC_ASE5_RX_COUNT,
	DB_MC_ASE6_TX_RATIO,
	DB_MC_ASE6_RX_RATIO,
	DB_MC_ASE6_TX_COUNT,
	DB_MC_ASE6_RX_COUNT,
	DB_MC_ASE7_TX_RATIO,
	DB_MC_ASE7_RX_RATIO,
	DB_MC_ASE7_TX_COUNT,
	DB_MC_ASE7_RX_COUNT,
	DB_MC_DPC_ID,
	DB_MC_NETWORKTYPE,
	DB_MC_HLR_BK,
	DB_MC_HLR_BK_FLAG,
	DB_MC_MSGCTRL_RESERVED
} eColumn_ID_Of_R_DDMsgCtrlTable_t;

#define DB_MC_DPC_SIZE              4
#define DB_MC_LINK_STATUS_SIZE      1
#define DB_MC_SAVEFLAG_SIZE         1
#define DB_MC_RESERVED_SIZE         2 //SR410 pkg 2 -> 2 bytes
#define DB_DPC_HLR_BK_SIZE          2 //FR730 pkg add
#define DB_DPC_HLR_BK_FLAG_SIZE     1 //FR730 pkg add
#define DB_MC_ASE_TX_RATIO_SIZE     1
#define DB_MC_ASE_RX_RATIO_SIZE     1
#define DB_MC_ASE_TX_COUNT_SIZE     1
#define DB_MC_ASE_RX_COUNT_SIZE     1
#define DB_MC_DPC_ID_SIZE           20

//FR730 pkg hlr bk flag enum
typedef enum Hlr_Bk_Flag_Type {
	HLR_BK_BLOCK,
	HLR_BK_ROUTING
} eHlr_Bk_Flag_Type;

//FR730 pkg hlr bk flag control
typedef enum Hlr_Bk_Control_Type {
	HLR_BK_FLAG_NOT_CONTROL_TYPE,
	HLR_BK_FLAG_CONTROL_TYPE,
	HLR_BK_TIMER_INIT_TYPE
} eHlr_Bk_Control_Type;

typedef struct {
	/*
	 *  DPC[3] + SSN[1]
	 *	dbDPC[4]
	 */
	u_char	dbDPC[DB_MC_DPC_SIZE]; 	 				// 4
	u_char	dbLinkStatus[DB_MC_LINK_STATUS_SIZE];  	// 1
	u_char	dbSaveFlag[DB_MC_SAVEFLAG_SIZE];  		// 1
	u_char	dbASE0TxRatio[DB_MC_ASE_TX_RATIO_SIZE]; // 1
	u_char	dbASE0RxRatio[DB_MC_ASE_RX_RATIO_SIZE]; // 1
	u_char	dbASE0TxCount[DB_MC_ASE_TX_COUNT_SIZE]; // 1
	u_char	dbASE0RxCount[DB_MC_ASE_RX_COUNT_SIZE]; // 1
	u_char	dbASE1TxRatio[DB_MC_ASE_TX_RATIO_SIZE]; // 1
	u_char	dbASE1RxRatio[DB_MC_ASE_RX_RATIO_SIZE]; // 1
	u_char	dbASE1TxCount[DB_MC_ASE_TX_COUNT_SIZE]; // 1
	u_char	dbASE1RxCount[DB_MC_ASE_RX_COUNT_SIZE]; // 1
	u_char	dbASE2TxRatio[DB_MC_ASE_TX_RATIO_SIZE]; // 1
	u_char	dbASE2RxRatio[DB_MC_ASE_RX_RATIO_SIZE]; // 1
	u_char	dbASE2TxCount[DB_MC_ASE_TX_COUNT_SIZE]; // 1
	u_char	dbASE2RxCount[DB_MC_ASE_RX_COUNT_SIZE]; // 1
	u_char	dbASE3TxRatio[DB_MC_ASE_TX_RATIO_SIZE]; // 1
	u_char	dbASE3RxRatio[DB_MC_ASE_RX_RATIO_SIZE]; // 1
	u_char	dbASE3TxCount[DB_MC_ASE_TX_COUNT_SIZE]; // 1
	u_char	dbASE3RxCount[DB_MC_ASE_RX_COUNT_SIZE]; // 1
	u_char	dbASE4TxRatio[DB_MC_ASE_TX_RATIO_SIZE]; // 1
	u_char	dbASE4RxRatio[DB_MC_ASE_RX_RATIO_SIZE]; // 1
	u_char	dbASE4TxCount[DB_MC_ASE_TX_COUNT_SIZE]; // 1
	u_char	dbASE4RxCount[DB_MC_ASE_RX_COUNT_SIZE]; // 1
	u_char	dbASE5TxRatio[DB_MC_ASE_TX_RATIO_SIZE]; // 1
	u_char	dbASE5RxRatio[DB_MC_ASE_RX_RATIO_SIZE]; // 1
	u_char	dbASE5TxCount[DB_MC_ASE_TX_COUNT_SIZE]; // 1
	u_char	dbASE5RxCount[DB_MC_ASE_RX_COUNT_SIZE]; // 1
	u_char	dbASE6TxRatio[DB_MC_ASE_TX_RATIO_SIZE]; // 1
	u_char	dbASE6RxRatio[DB_MC_ASE_RX_RATIO_SIZE]; // 1
	u_char	dbASE6TxCount[DB_MC_ASE_TX_COUNT_SIZE]; // 1
	u_char	dbASE6RxCount[DB_MC_ASE_RX_COUNT_SIZE]; // 1
	u_char	dbASE7TxRatio[DB_MC_ASE_TX_RATIO_SIZE]; // 1
	u_char	dbASE7RxRatio[DB_MC_ASE_RX_RATIO_SIZE]; // 1
	u_char	dbASE7TxCount[DB_MC_ASE_TX_COUNT_SIZE]; // 1
	u_char	dbASE7RxCount[DB_MC_ASE_RX_COUNT_SIZE]; // 1
	u_char  dbDPCId[DB_MC_DPC_ID_SIZE];  			// 20
	u_char  dbNetworkType[DB_NETWORK_TYPE_SIZE];  	// 1
	u_char  dbHlrBk[DB_DPC_HLR_BK_SIZE]; 			// 2
	u_char  dbHlrBkFlag[DB_DPC_HLR_BK_FLAG_SIZE]; 	// 1
	u_char 	dbHlrBkTime[MDB_TIME_TYPE_SIZE];		// 4
	u_char	dbReserved[DB_MC_RESERVED_SIZE]; 		// 2
} stRDMsgCtrlTuple; // Total : 68 bytes

typedef struct {
	u_int	uDPC;
	u_char	ucLinkStatus;
	u_char	ucSaveFlag;
	u_char	ucASE0TxRatio;
	u_char	ucASE0RxRatio;
	u_char	ucASE0TxCount;
	u_char	ucASE0RxCount;
	u_char	ucASE1TxRatio;
	u_char	ucASE1RxRatio;
	u_char	ucASE1TxCount;
	u_char	ucASE1RxCount;
	u_char	ucASE2TxRatio;
	u_char	ucASE2RxRatio;
	u_char	ucASE2TxCount;
	u_char	ucASE2RxCount;
	u_char	ucASE3TxRatio;
	u_char	ucASE3RxRatio;
	u_char	ucASE3TxCount;
	u_char	ucASE3RxCount;
	u_char	ucASE4TxRatio;
	u_char	ucASE4RxRatio;
	u_char	ucASE4TxCount;
	u_char	ucASE4RxCount;
	u_char	ucASE5TxRatio;
	u_char	ucASE5RxRatio;
	u_char	ucASE5TxCount;
	u_char	ucASE5RxCount;
	u_char	ucASE6TxRatio;
	u_char	ucASE6RxRatio;
	u_char	ucASE6TxCount;
	u_char	ucASE6RxCount;
	u_char	ucASE7TxRatio;
	u_char	ucASE7RxRatio;
	u_char	ucASE7TxCount;
	u_char	ucASE7RxCount;
	u_char 	ucDPCId[DB_MC_DPC_ID_SIZE];
	u_char  ucNetworkType;
	u_short usHlrBk;
	u_char 	ucHlrBkFlag;
	u_char 	dbHlrBkTime[MDB_TIME_TYPE_SIZE];
	u_char 	ucReserved[DB_MC_RESERVED_SIZE];
} stRDMsgCtrlType; // Total : 68 bytes

typedef enum Column_ID_Of_R_DServiceIdTable {
	DB_SI_SERVICE_ID,
	DB_SI_SVC_ONOFF,
	DB_SI_ESME_ID,
	DB_SI_GROUP_CODE
} eColumn_ID_Of_R_DServiceIdTable_t;

#define	DB_SI_SERVICE_ID_SIZE	2
#define	DB_SI_ESME_ID_SIZE		4
#define	DB_SI_RESERVED_SIZE		2

typedef struct {
	u_char	dbServiceId[DB_SI_SERVICE_ID_SIZE]; //2
	u_char	dbServiceOnOff[DB_SI_RESERVED_SIZE]; // 2
	u_char  dbEsmeId[DB_SI_ESME_ID_SIZE]; // 4 
	u_char  dbEsmeId2[DB_SI_ESME_ID_SIZE]; // 4
	u_char 	dbGroupCode[MDB_NUMBER_SIZE]; // 4
} stRDServiceIdTuple;

typedef struct {
	u_short	nServiceId; //2
	u_short	nServiceOnOff; // 2
	u_int  	nEsmeID; // 4 
	u_int  	nEsmeID2; // 4
	u_int  	nGroupCode; // 4
} stRDServiceIdType;

//FR320 LSH
// DR610 LSH
#define DB_SERVICE_ID_SIZE 2
#define DB_SERVICE_COUNT_SIZE 2
#define DB_SERVICE_TIME_SIZE 2
#define DB_SERVICE_RESERVED_SIZE 2

typedef struct {
	u_char  dbServiceId[DB_SERVICE_ID_SIZE]; //2
	u_char  dbCount[DB_SERVICE_COUNT_SIZE]; // 2
	u_char  dbTime[DB_SERVICE_TIME_SIZE]; //2
	u_char  dbReserved[DB_SERVICE_RESERVED_SIZE]; //2
} stRDServiceControlTuple;
//DR610 LSH
typedef enum Column_ID_OF_R_DServiceControlConfig {
	DB_SERVICE_ID,
	DB_SERVICE_COUNT,
	DB_SERVICE_TIME,
	DB_SERVICE_RESERVED
} eColumn_ID_OF_R_DServiceControlConfig_t;

typedef struct {
	unsigned short  dbServiceId; //2
	unsigned short  dbCount; // 2
	unsigned short  dbTime; //2
	unsigned short  dbReserved; //2
} stRDServiceControlData;

//FR230 LSH
typedef enum Column_ID_Of_R_DConvert_SpecialMin {
	DB_TYPE_MATCH_DESTMIN,
	DB_CONVERT_DEST,
	DB_DEST_OCT,
	DB_SPECIAL_RESERVED
} eColumn_ID_Of_R_DConvert_SpecialMin_t;


#define DB_TYPE_MATCH_DEST_SIZE 14
#define DB_CONVERT_DEST_SIZE 12
#define DB_CONVERT_DEST_RSM_M2M_SIZE 20 // SR810 PKG
#define DB_DEST_OCT_NUMBER_SIZE 1
#define DB_DEST_RESERVED_SIZE 1
#define DB_TYPE 2
//DR810 pkg callback chg exp source min
typedef enum Column_ID_Of_R_DCallbackChgExpMin {
	DB_EXP_SOURCE_MIN
} eColumn_ID_Of_R_DCallbackChgExpMin_t;
#define DB_EXP_SOURCE_MIN_SIZE 12
typedef struct {
	u_char dbExpSourceMin[DB_EXP_SOURCE_MIN_SIZE];
} stRDCallbackChgExpMinTuple;

typedef struct {
	//cpTypeMatchDest (14byte) = Type = (2byte) + Match DestMin = (12byte)
	u_char dbTypeMatchDest[DB_TYPE_MATCH_DEST_SIZE];//14
	u_char dbConvertDest[DB_CONVERT_DEST_SIZE];//12
	u_char dbDestOctNum[DB_DEST_OCT_NUMBER_SIZE];//1
	u_char dbReserved[DB_DEST_RESERVED_SIZE];//1
} stRDConvertSpecialMinTuple; //stRDConvertDestTuple;

typedef struct {
	u_short usServiceCode;
	u_char dbTypeMatchDest[DB_CONVERT_DEST_SIZE];//12
	u_char dbConvertDest[DB_CONVERT_DEST_SIZE];//12
	u_char dbDestOctNum[DB_DEST_OCT_NUMBER_SIZE];//1
	u_char dbReserved[DB_DEST_RESERVED_SIZE];//1
} stRDConvertSpecialMinData; //stRDConvertDestData;

typedef enum Column_ID_Of_R_DNpdb_RN_Route {
	DB_NPDB_RN_PREFIX,
	DB_NPDB_RN_ROUTE_TYPE,
	DB_NPDB_RN_NAME,
	DB_NPDB_RN_RESERVED
} eColumn_ID_Of_R_DNpdb_RN_Route;

typedef struct {
	u_char dbPrefix[12]; // BCD Code
	u_char dbRouteType[1];
	u_char dbRnName[23];
	u_char dbReserved[4];
} stRDNpdbRnRouteTuple;


typedef enum Column_ID_Of_R_DCharset {
	DB_CHARSET_KEY,
	DB_OUT_CHARSET_VALUE,
} eColumn_ID_Of_R_DCharset;

#define CHARSET_CP949 	0x84
#define CHARSET_UCS2	0x08

#define DB_CHARSET_KEY_SIZE 12
#define DB_CHARSET_VALUE_SIZE 4
typedef struct {
	u_char charset_key[DB_CHARSET_KEY_SIZE];
	u_char out_value[DB_CHARSET_VALUE_SIZE];
} stRDCharsetTuple;

typedef struct {
	u_int in_charset;
	u_int in_value;
	u_int out_charset;
	u_int out_value;
} stRDCharsetData;
//////////////////// Static DBMS End ////////////////////////

typedef enum ATSItype_010Plus_NumberPlus {
	DB_ATSI_BASE_TYPE = 0x00,
	DB_010_PLUS_TYPE = 0x01,
	DB_NUMBER_PLUS_TYPE = 0x20,
	DB_MULTINUM_TYPE 	= 0x03,
	DB_CID_CHANGE_TYPE 	= 0x04,
	DB_IOS_RCS_MO_TYPE 	= 0x05
} eATSItype_010Plus_NumberPlus;

typedef struct {
	char source_min[DB_SOURCE_MIN_SIZE * 2 + 1];
	char callback_number[DB_CALLBACK_NUMBER_SIZE * 2 + 1];
	char message_content[DB_MESSAGE_CONTENT_SIZE + 1];
} stCallbackDenyNotiInfo;

typedef stCallbackDenyNotiInfo stMultiNumDenyNotiInfo;
typedef stCallbackDenyNotiInfo stPPSDenyNotiInfo;


typedef enum Column_ID_Of_R_DAdminBar {
	DB_AB_INDEX_KEY,
	DB_AB_SOURCE_MIN,
	DB_AB_DEST_MIN,
	DB_AB_CALLBACK_NUM,
	DB_AB_FILTER_MESSAGE,
	DB_AB_ACT_FLAG,			// SR810 PKG
	DB_AB_AUTO_FLAG,
	DB_AB_TIME,
	DB_AB_EXPIRATION_TIME,	// SR810 PKG
	DB_AB_RESERVED
} eColumn_ID_Of_R_DAdminBar;

typedef struct {
	u_char  dbIndex[4];
	u_char  dbSourceMIN[MDB_MIN_SIZE];
	u_char  dbDestMIN[MDB_MIN_SIZE];
	u_char  dbCallbackNumber[MDB_MIN_SIZE];
	u_char  dbFilterMessage[81];
	u_char  dbActFlag[1];
	u_char  dbAutoFlag[1];							// SR810 PKG
	u_char  dbTime[MDB_TIME_TYPE_SIZE];
	u_char  dbExpirationTime[MDB_TIME_TYPE_SIZE];	// SR810 PKG
	u_char  dbReserved[1];							// SR810 PKG 2->1
} stRDAdminBarTuple;

typedef struct {
	u_int   unIndex;
	u_char  cSourceMIN[MDB_MIN_SIZE];
	u_char  cDestMIN[MDB_MIN_SIZE];
	u_char  cCallbackNumber[MDB_MIN_SIZE];
	u_char  cFilterMessage[81];
	u_char  cActFlag;
	u_char  cAutoFlag;			// SR810 PKG
	u_int   unTime;
	u_int   unExpirationTime;	// SR810 PKG
	u_char  cReserved[1];		// SR810 PKG 2->1
} stRDAdminBarType;
#endif
