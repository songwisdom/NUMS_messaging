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

/*******************************************************************************
    FILE NAME       :
    PURPOSE         :
    DATE OF WRITE   :   2001.12.01
    AUTHOR          :   Suk-Hun Jang
    REVISION        :   V 1.0
    Ver.    By      When        Reason
    ------- ------- ----------  -----------------------------------------
    1.0     shjang  2001.12.01  first coding
*******************************************************************************/
#ifndef _MAPPARAM_H_
#define _MAPPARAM_H_

/*****************************************************************************/
/*																			 */
/*			IT USED FOR LENGTH	FIELD										 */
/*																			 */
/*****************************************************************************/
/*
 * FIXED LENGTH FIELD VALUE
 */
typedef enum {
	ONELENGTH	= 1,
	ESNLENGTH	= 4,
	MINLENGTH	= 5
} eMapFixedLengthFieldValue;



/*****************************************************************************/
/*																			 */
/*			IT USED FOR TAG	FIELD											 */
/*																			 */
/*****************************************************************************/

/*
 * HEADER PARAMETER

typedef enum {
	LOCALOPCODE		= 0x02,
	GLOBALOPCODE	= 0x06,
	SEQUENCETAG		= 0x30,
	SETTAG			= 0x31
} eMapHeaderParameter;
*/

/*
	HEADER PARAMETER
*/
#define     LOCALOPCODE             0x02
#define     GLOBALOPCODE            0x06
#define     SEQUENCETAG             0x30
#define     SETTAG                  0x31


/*
 * OP CODE PARAMETER
 */
typedef enum {
	ORREQ			= 0x2F,
	SMDPP			= 0x35,
	SMSNOT			= 0x36,
	SMSREQ			= 0x37,
	NPREQ			= 0x3E,
	GWSMPP_DELIVER  = 0xE5,
	GWSMSNOTSMC     = 0xE6,
	GWSMPP_SUBMIT   = 0xE7,
	GWSMDPP         = 0xE8
} eMapOpCodeParameter;

/*
 * TAG PARAMETER
 */
typedef enum { TAG_LEN	= 4 } eMapTagLength;

typedef enum {
	MIN_TAG						= 0x88,
	ESN_TAG						= 0x89,
	SMS_ADDRESS_TAG				= 0x9f68,
	SMS_BEARERDATA_TAG			= 0x9f69,
	SMS_CHARGE_IND_TAG			= 0x9f6a,
	SMS_DEST_ADDR_TAG			= 0x9f6b,
	SMS_MSGCOUNT_TAG			= 0x9f6c,
	SMS_NOTI_IND_TAG			= 0x9f6d,
	SMS_ORG_DEST_ADDR_TAG		= 0x9f6e,
	SMS_ORG_DEST_SUB_ADDR_TAG	= 0x9f6f,
	SMS_ORG_ORG_ADDR_TAG		= 0x9f70,
	SMS_ORG_ORG_SUB_ADDR_TAG	= 0x9f71,
	SMS_ORG_ADDR_TAG			= 0x9f72,
	SMS_TELESERVICE_IND_TAG		= 0x9f74,
	ROUTING_DIGITS_TAG			= 0x9f8116,
	SMS_ACC_DEN_REASON_TAG		= 0x9f8118,
	SMS_CAUSE_CODE_TAG			= 0x9f8119,
	SMS_IMSI_TAG				= 0x9f8172,

	// LGT SCP TAG,
	BILLING_ID_TAG				= 0x81,
	DIGITS_TAG					= 0x84,
	MSC_ID_TAG					= 0x95,
	ORIGINATION_TRIGGERS_TAG	= 0x9f62,
	TRANSACTION_CAPABILITY_TAG	= 0x9f7b,
	CALLING_PARTY_DIGITS_1_TAG	= 0x9f50,
	MOBILE_DIRECTORY_NUMBER_TAG	= 0x9f5d,
	PREFFERED_LANG_IND_TAG		= 0x9f8113,
	SERVING_CELL_ID_TAG			= 0x82,
	TRIGGER_TYPE_TAG			= 0x9f8217,
	SERVICE_OPTION_TAG			= 0x9f812f,
	FEATURE_IND_TAG				= 0xdf8213,
	ACCESS_DENI_REASON_TAG		= 0x94,
	RESUME_PIC_TAG				= 0xdf820a,
	TERMINATION_LIST_TAG		= 0xbf78,
	CHARGE_INFORMATION_TAG		= 0x9f8237,
	WIN_CAPABILITY_TAG			= 0xbf8218,
	INTERSYSTEM_TERM_TAG		= 0xbf59,
	LOCAL_TERM_TAG				= 0xbf5b,
	PSTN_TERM_TAG				= 0xbf5f,
	DESTINATION_DIGITS_TAG		= 0x9f57,
	// LGT Long MSG,
	//  future use - Private LGT Only
	//#define		SMS_CAPABILITY_TAG = 0xdf8216
	//  curret use - Global LGT Only. use,
	SMS_LGT_CAPABILITY_TAG			= 0x9f8607,

	RECIPIENT_SERVICE_PROVI_IND_TAG	= 0x9fff31,
	//
	//	SKT TAG
	//
	SMS_SKT_OSFI_TAG				= 0x9fff3a,
	SMS_SKT_CAPABILITY_TAG			= 0x9fff8f,
	SMS_SKT_NEW_CAPABILITY_TAG		= 0x9fff36,
	SMS_SKT_ROAMINDICAT_TAG			= 0x9fff23,
	SMS_SKT_NEW_ROAMINDICAT_TAG		= 0xbfff23,
	SMS_SKT_SUBSCRIBER_GTYPE_TAG	= 0x9fff21,
	SMS_SKT_LOCATION_AREA_ID_TAG	= 0x9f21,
	SMS_SKT_ROAMING_PMN_TAG			= 0x9fff1b,
	SMS_SKT_ADDRESS_EX_TAG			= 0x9fff38	//	SMS_ADDRESS2
} eTagParameter;


/*****************************************************************************/
/*																			 */
/*			IT USED FOR 637	FIELD											 */
/*																			 */
/*****************************************************************************/
/*
 * define 637 parameter configuration file.
 */
#define		CALL_BACK_NUMBER_LEN	21
#define		PARAMETER_CFG			"parameter.cfg"
/*
 * define to test 637 encoding using teleservice id
 */
typedef enum {
	CPT_SERVICE				= 0x1001,
	CMT_SERVICE				= 0x1002,
	VMN_SERVICE				= 0x1003,
	INTERACTIVE_SERVICE		= 0xc001,
	MELODY_DOWNLOAD_SERVICE	= 0x8000,
	MELODY_DOWNLOAD_S1      = 0xf010,
	MELODY_DOWNLOAD_S2      = 0xf011,
	VMW_SERVICE      		= 0x1388,
	FMW_SERVICE      		= 0x1389,
	WMMW_SERVICE      		= 0x138a,
	MMS_SERVICE      		= 0x138b
} e637EncodingTid;

/*
 * 637 SMS TRANSPORT MESSAGE TYPE
 */
typedef enum {
	PONIT_TO_POINT,
	BROADCAST,
	ACKNOWLEDGE
} e637SmsTransportMessageType;

/*
 * 637 SMS TELESERVICE MESSAGE TYPE
 */
typedef enum {
	SMS_NO_DEF_MSG			= 0x00,
	SMS_DELIVER_MSG,
	SMS_SUBMIT_MSG,
	SMS_CANNCEL_MSG,
	SMS_DELIVERY_ACK_MSG,
	SMS_USER_ACK_MSG,
	SMS_STOP_MSG    		= 0x0a
} e637SmsTeleserviceMessageType;

/*
 * 637 SMS PARAMATER TYPE
 */
typedef enum {
	TELESERVICE_IDENTIFIER,
	BROAD_SERVICE_CATEGORY,
	ORIG_ADDRESS,
	ORIG_SUBADDRESS,
	DEST_ADDRESS,
	DEST_SUBADDRESS,
	BEARER_REPLY_OPTION,
	CAUSE_CODE,
	BEARER_DATA
} e637SmsParamaterType;

/*
 * 637 SMS SUBPARAMETER TYPE
 */
typedef enum {
	MESSAGE_IDENTIFIER,
	USER_DATA,
	USER_RES_CODE,
	MES_CENTER_TIME,
	VALIDITY_PERIOD_ABS,
	VALIDITY_PERIOD_REL,
	DEFERRED_D_TIME_ABS,
	DEFERRED_D_TIME_REL,
	PRIORITY_INDICATOR,
	PRIVACY_INDICATOR,
	REPLY_OPTION,
	NUMBER_MESSAGE,
	ALERT_ON_MESSAGE,
	LANGUAGE_INDICATOR,
	CALL_BACK_NUMBER		= 0x0e,

	RESPONSE_TYPE           = 0x80,
	SESSION_INFORMATION     = 0x81,
	MELODY_DOWNLOAD			= 0xc1
} e637SmsSubparameterType;

/*** SKT Version
#define		PROPERTY				0x81
#define		RESPONSE_TYPE			0x82
****/


typedef enum {
	MESSAGE_IDENTIFIER_LEN	= 0x03,
	MES_CENTER_TIME_LEN		= 0x06,
	PRIVACY_INDICATOR_LEN	= 0x01,
	PRIORITY_INDICATOR_LEN	= 0x01,
	RESPONSE_TYPE_LEN		= 0x01,
	REPLY_OPTION_LEN		= 0x01,
	LANGUAGE_INDICATOR_LEN	= 0x01,
	NUMBER_MESSAGE_LEN		= 0x01,
	REL_VALIDITY_LEN        = 0x01,
	ABS_VALIDITY_LEN        = 0x06
} e637ParameterLength;

#define		MAX_NUMBER_MESSAGE		99
#define		INCLUDE_ON				0x01
#define		INCLUDE_OFF				0x00
#define		ENCDATA_LEN				193
#define		ENC_HEADER_LEN			33
#define		MAX_BEARERDATA_LEN	    127

/*
 * encoding type
 */
typedef enum {
	OCTET,		// Default
	IS91EXTEND,
	ASCII,
	IA5,
	KSC5601		= 0x10
} eMapEncodingType;

/*
 * skt
 */
#define		DCS_ENGLISH				0x00
#define		DCS_KOREAN				0xfe
#define     JAPANESE_DCS            0xf0
#define     CHINESE_DCS             0xf1
#define 	DCS_UCS2 				0x08
#define 	DCS_GSM7BIT 			0x01



/*
 * define language identifier
 */
typedef enum {
	LANG_UNKNOWN    = 0x00,
	LANG_ENGLISH    = 0x01,
	LANG_FRENCH     = 0x02,
	LANG_SPENISH    = 0x03,
	LANG_JAPANESE   = 0x04,
	LANG_KOREAN     = 0x05,
	LANG_CHINESE    = 0x06,
	LANG_HEBREW     = 0x07,
	LANG_KOREAN_LGT = 0x40,// 64
	LANG_KOREAN_SKT = 0xfe // 254
	                  /*
	                  JAPANESE_LANGAGE_ID    = 0x04,
	                  CHINESE_LANAGE_ID      = 0x06,
	                  UNICODE_MSG_ENC        = 0x04
	                  */
} eLanguageIdentifier;
/*
 * call back parameter
 */
typedef enum {
	CB_DTMF_DIGIT,	// default.
	CB_ASCII_DIGIT
} eCallBackParameter;

/*
 * COMMON PARAMETER LENGTH
 */
typedef enum {
	OPCODELEN			= 1,
	PARAMETERCODELEN	= 1
} eCommonParameterLength;

/*
 * TAG LENGTH
 */
typedef enum {
	SMDPPLEN				= 1,
	SMSNOTLEN				= 1,
	SMSREQLEN				= 1,

	SMS_BEARERDATALEN		= 2,
	SMS_TELESERVICEIDLEN	= 2,
	ESNLEN					= 1,
	MINLEN					= 5,
	SMS_ADDRESSLEN			= 2,
	SMS_CHARGEIDLEN			= 2,
	SMS_DESTADDRLEN			= 2,
	SMS_MSGCOUNTLEN			= 2,
	SMS_NOTIFICATIONIDLEN	= 2,
	SMS_ORGDESTADDRLEN		= 2,
	SMS_ORGDESTSUBADDRLEN	= 2,
	SMS_ORGORGADDRLEN		= 2,
	SMS_ORGORGSUBADDRLEN	= 2,
	SMS_ORGADDRLEN			= 2,
	SMS_ACCDENREASONLEN		= 3,
	SMS_CAUSECODELEN		= 3
} eMapParameterLength;

/***  mapparamGSM.h ****/

/*****************************************************************************/
/*                                                                           */
/*          BEARER SIZE                                                      */
/*                                                                           */
/*****************************************************************************/
typedef enum {
	ADDRESS_NPI_SIZE        = 4,
	ADDRESS_TON_SIZE        = 3,
	MMS_SIZE                = 1,
	MN_SIZE                 = 1,
	MTI_SIZE                = 2,
	OCTET_SIZE              = 8,
	RP_SIZE                 = 1,
	SEMI_OCTET_SIZE         = 4,
	SRI_SIZE                = 1,
	SRR_SIZE                = 1,
	SRQ_SIZE                = 1,
	UDHI_SIZE               = 1,
	UDL_SIZE                = 8,
	RD_SIZE                 = 1,
	VPF_SIZE                = 2,
	MR_SIZE                 = 8,
	NPI_SIZE                = 4,
	TON_SIZE                = 3
} eBearerSize;

/*****************************************************************************/
/*                                                                           */
/*          BEARER FLAG LOCATION(bit position)                               */
/*                                                                           */
/*****************************************************************************/
typedef enum {
	MTI_LOC                 = 0,   // 0, 1 bit
	MMS_LOC                 = 2,
	RD_LOC                  = 2,
	VPF_LOC                 = 3,   // 3, 4 bit
	SRI_LOC                 = 5,
	SRR_LOC                 = 5,
	SRQ_LOC                 = 5,
	UDHI_LOC                = 6,
	RP_LOC                  = 7
} eBearerFlagLocation;
/*****************************************************************************/
/*                                                                           */
/*          MTI TYPE DEFINITION(ORDERING)                                    */
/*                                                                           */
/*****************************************************************************/
typedef enum {
	SMS_DELIVER				= 0x00,
	SMS_DELIVER_REPORT		= 0x00,
	SMS_STATUS_REPORT		= 0x02,
	SMS_COMMAND				= 0x02,
	SMS_SUBMIT				= 0x01,
	SMS_SUBMIT_REPORT		= 0x01,
	RESERVED				= 0x03
} eMtiTypeDefinition;

/*****************************************************************************/
/*                                                                           */
/*          MMS TYPE DEFINITION                                              */
/*                                                                           */
/*****************************************************************************/
typedef enum {
	MORE_MESSAGE,
	NO_MORE_MESSAGE
} eMmsTypeDefinition;

/*****************************************************************************/
/*                                                                           */
/*          RD TYPE DEFINITION                                               */
/*                                                                           */
/*****************************************************************************/
typedef enum {
	ACCESS_SUMIT,
	REJECT_SUBMIT
} eRdTypeDefinition;

/*****************************************************************************/
/*                                                                           */
/*          VPF TYPE DEFINITION(ORDERING)                                    */
/*                                                                           */
/*****************************************************************************/
#define     VP_NOT_PRESENT          0x00
#define     VP_RELATIVE_FMT         0x02
#define     VP_ENHANCED_FMT         0x01
#define     VP_ABSOLUTE_FMT         0x03

/*****************************************************************************/
/*                                                                           */
/*          SRI TYPE DEFINITION                                              */
/*                                                                           */
/*****************************************************************************/
#define     NOT_RETURNED            0x00
#define     RETURNED                0x01
/*****************************************************************************/
/*                                                                           */
/*          SRR TYPE DEFINITION                                              */
/*                                                                           */
/*****************************************************************************/
#define     NOT_REQUESTED           0x00
#define     REQUESTED               0x01

/*****************************************************************************/
/*                                                                           */
/*          SRQ TYPE DEFINITION                                              */
/*                                                                           */
/*****************************************************************************/
#define     RESULT_OF_SUBMIT        0x00
#define     RESULT_OF_COMMAND       0x01

/*****************************************************************************/
/*                                                                           */
/*          UDHI TYPE DEFINITION                                             */
/*                                                                           */
/*****************************************************************************/
#define     ONLY_SHORT_MESSAGE      0x00
#define     CONTAINS_HEADER         0x01

/*****************************************************************************/
/*                                                                           */
/*          RP TYPE DEFINITION                                               */
/*                                                                           */
/*****************************************************************************/
#define     REPLY_PATH_NOT_SET      0x00
#define     REPLY_PATH_SET          0x01

/*****************************************************************************/
/*                                                                           */
/*          PID TYPE DEFINITION                                              */
/*                                                                           */
/*****************************************************************************/
#define     SHORT_MESSAGE_TYPE      0x00

/*****************************************************************************/
/*                                                                           */
/*          DATA CODING SCHEME DEFINITION            */
/*                                                                           */
/*****************************************************************************/
#define U_DEFAULT_ALPA      0x00
#define U_8_BIT             0x01
#define U_UCS2_16           0x02
#define U_RESERVED          0x04
#define U_KSC5601          	0x03
#define DC_KSC5601			0x0e

#define U_VMW				0x05
#define U_FMW				0x06
#define U_WMMW				0x07
#define U_MMS				0x08

/************************************
	EMS TAG definition

************************************/
#define U_CONCAT_SM_8_BIT	0x00
#define U_SPEC_SM_IND 		0x01
#define U_LF_CHAR			0x03

#define     VMN_PID     0x48
#define     CPT_PID     0x49
#define     MMS_PID     0x4f


/**********************************************
	IMSI LENGTH for MNP
**********************************************/
#define MAXIMSILENGTH                   8       /* 3..8 */
#define MINIMSILENGTH                   3       /* 3..8 */


/**********************************************
	Type Of Number (IS-41.D) 1byte
**********************************************/
typedef enum {
	ADDR_TON_NOT_USED				=	0,
	ADDR_TON_CALLED_PARTY_NUMBER,
	ADDR_TON_CALLING_PARTY_NUMBER,
	ADDR_TON_CALLER_INTERACTION,
	ADDR_TON_ROUTING_NUMBER,
	ADDR_TON_BILLING_NUMBER,
	ADDR_TON_DESTINATION_NUMBER,
	ADDR_TON_LATA,
	ADDR_TON_CARRIER
} eAddTypeOfNumber;

/**********************************************
	Nature Of Number (IS-41.D) 1byte
**********************************************/
typedef enum {
	ADDR_NON_NATIONAL				=	0,
	ADDR_NON_INTERNATIONAL,
	ADDR_NON_PRESENTATION_ALLOWED,
	ADDR_NON_PRESENTATION_RESTRICTED,
	ADDR_NON_NUMBER_IS_AVAILABLE,
	ADDR_NON_NUMBER_IS_NOT_AVAILABLE,
	ADDR_NON_USER_PROVIDED_NOT_SCREENED,
	ADDR_NON_USER_PROVIDED_SCREENING_PASSED,
	ADDR_NON_USER_PROVIDED_SCREENING_FAILED,
	ADDR_NON_NETWORK_PROVIDED
} eAddNatureOfNumber;

/**********************************************
	Numbering Plan (IS-41.D) 4bits 7654xxxx
**********************************************/
typedef enum {
	ADDR_NP_UNKNOWN					=	0,
	ADDR_NP_ISDN_NUMBERING,
	ADDR_NP_TELEPHONY_NUMBERING,
	ADDR_NP_DATA_NUMBERING,
	ADDR_NP_TELEX_NUMBERING,
	ADDR_NP_MARITIME_MOBILE_NUMBERING,
	ADDR_NP_LAND_MOBILE_NUMBERING,
	ADDR_NP_PRIVATE_NUMBERING_PLAN,
	ADDR_NP_ANSI_SS7_PC_AND_SSN,
	ADDR_NP_INTERNET_PROTOCOL_ADDRESS
} eAddNumberingPlan;

/**********************************************
	Encoding (IS-41.D) 		4bits xxxx3210
**********************************************/
typedef enum {
	ADDR_ED_UNKNOWN					=	0,
	ADDR_ED_BCD,
	ADDR_ED_IA5,
	ADDR_ED_OCTET_STRING
} eAddEncoding;

/**********************************************
   Type of Number (GSM - 3bits x654xxxx)
**********************************************/
#define ADDR_TON_UNKNOWN			0
#define ADDR_TON_INTERNATIONAL		1
#define ADDR_TON_NATIONAL			2
#define ADDR_TON_NETWORK			3
#define ADDR_TON_SUBSCRIBER			4
#define ADDR_TON_ALPHANUMERIC		5
#define ADDR_TON_ABBREVIATED		6
#define ADDR_TON_RESERVED_EXTN		7

/**********************************************
 	Numbering Plan Indicator (GSM - 4bits 4321)
**********************************************/
#define ADDR_NPI_UNKNOWN			0
#define ADDR_NPI_E164				1
#define ADDR_NPI_ISDN				ADDR_NPI_E164	/* alias */
#define ADDR_NPI_SPARE1				2
#define ADDR_NPI_X121				3
#define ADDR_NPI_TELEX				4
#define ADDR_NPI_LMNP				6 //LAND_MOBILE_NUMBERING_PLAN
#define ADDR_NPI_NATIONAL			8
#define ADDR_NPI_PRIVATE			9
#define ADDR_NPI_ERMES				10
#define ADDR_NPI_RESERVED_EXTN		15

/*****************************************************
	Recipient Service Provider Indicator (SKT define)
******************************************************/
typedef enum {
	SKT_RECIPIENT_SPI_NOT_USED	= 0,
	SKT_RECIPIENT_SPI_SKT_2G	= 1,
	SKT_RECIPIENT_SPI_KTF_2G	= 2,
	SKT_RECIPIENT_SPI_LGT_2G	= 3,
	SKT_RECIPIENT_SPI_SKT_3G	= 4,
	SKT_RECIPIENT_SPI_KKT_3G	= 5,
	SKT_RECIPIENT_SPI_LGT_3G	= 6
} eRecipientSPI;

#endif
