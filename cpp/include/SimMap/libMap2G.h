/**
 * =====================================================================================
 *
 *       @file  libMap2G.h
 *
 *    	 @brief 
 *
 *       Version:  1.0
 *       Compiler:  cc (HP-UX)
 *       @date:  04/23/09 19:47:07
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

#ifndef _H_LIB_MAP_2G_
#define _H_LIB_MAP_2G_

#include "flk_type.h"
#include "libMapComm.h"
#include "libMap2G_St.h"
#include "libMapUtil.h"
typedef enum {          
    OCTET = 0x00,      // Default  
    IS91EXTEND,             
    ASCII,
    IA5,                
    KSC5601     = 0x10      
} eMapEncodingType;         

typedef enum {
    SMS_NO_DEF_MSG          = 0x00,
    SMS_DELIVER_MSG,
    SMS_SUBMIT_MSG,
    SMS_CANNCEL_MSG,
    SMS_DELIVERY_ACK_MSG,
    SMS_USER_ACK_MSG,
    SMS_STOP_MSG            = 0x0a
}e637SmsTeleserviceMessageType;

typedef enum {
	ONELENGTH	= 1,
	ESNLENGTH	= 4,
	MINLENGTH	= 5
} eMapFixedLengthFieldValue;

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
	// LGT LONG MSG,
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

typedef enum {
	CB_DTMF_DIGIT,	// default.
	CB_ASCII_DIGIT
} eCallBackParameter;

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
#define DB_MESSAGE_CONTENT_SIZE             161
#define ADDRESS_LEN_CALLBACK 				12

/* enc2G_637.c */
int encodeMsgId( u_char *BearerData, u_char SubId, u_short MsgId  );
int encodeUserData(u_char *BearerData, u_char *Message, int MessageLen, u_char MsgEncoding);
int encodePriorityId(u_char *BearerData, u_char PriorityFlag);
int encodeLanguageId(u_char *BearerData, u_char LanguageFlag);
int encodeCbNumber( u_char *BearerData, u_char *CallBack, int CB_Octets );

int encSMDPP(stMsgBuf_t *spMsg, stSMDPP_t *pD, int nTag);
#endif
