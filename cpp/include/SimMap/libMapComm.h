/**
 * =====================================================================================
 *
 *       @file  libMapComm.h
 *
 *    	 @brief 
 *
 *       Version:  1.0
 *       Compiler:  cc (HP-UX)
 *       @date:  04/23/09 15:19:25
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

#ifndef _H_LIB_MAP_COMMON_
#define _H_LIB_MAP_COMMON_

#include "flk_type.h"
#include "libGsmTpduStruct.h"
#define SEND_ROUTING_INFO_FOR_SM        	0x2D        				/* 45 */
#define MO_FORWARDSM                    	0x2E        				/* 46 */
#define MT_FORWARDSM                    	0x2C        				/* 44 */
#define FORWARD_SM							MO_FORWARDSM				
#define REPORT_SM_DELIVERY_STATUS       	0x2F        				/* 47 */
#define	SET_MESSAGE_WAITING_DATA			REPORT_SM_DELIVERY_STATUS	/* v1 */
#define INFORM_SERVICE_CENTRE          		0x3F        				/* 63 */
#define ALERT_SERVICE_CENTRE              	0x40       					/* 60 */
#define	ALERT_SERVICE_CENTRE_WITHOUT_RESULT	0x31					/* 49 for v1 */

#define MSG_BUFFER_SIZE                512
#define GET_LENGTH_BIT_TO_OCT(x) ( (x / 8) + (x % 2) )
#define ADDRESS_LEN_STRING 			20
#define ADDRESS_LEN_BCD    			10
#define LMSI_LEN   	          		4
#define MAX_IMSI_LEN            	8       
#define MIN_IMSI_LEN                3 
#define MAX_ISDNADDRESS_LEN         9

#define	CALL_BACK_NUMBER_LEN	21
#define TAG_LEN 4
#define MINLEN  5
#define MAXIMSILENGTH                   8       /* 3..8 */
#define MINIMSILENGTH                   3       /* 3..8 */

#define MAX_OCTET_LEN           	256

#define UNKNOWN1                        0
#define INTERNATIONAL_NUMBER            1
#define NATIONAL_SIGNIFICANT_NUMBER 	2
#define NETWORK_SPECIFIC_NUMBER         3
#define SUBSCRIBER_NUMBER               4
#define RESERVED1                       5
#define ABBREVIATED_NUMBER              6
#define RESERVED_FOR_EXTENSION1         7

/*
 *  numbering plan indicator (bits 4321)
 */
#define UNKNOWN2                        0
#define ISDN_TELEPHONY_NUMBERING_PLAN   1
#define SPARE1                          2
#define DATA_NUMBERING_PLAN             3
#define TELEX_NUMBERING_PLAN            4
#define SPARE2                          5
#define LAND_MOBILE_NUMBERING_PLAN      6
#define SPARE3                          7
#define NATIONAL_NUMBERING_PLAN         8
#define PRIVATE_NUMBERING_PLAN          9
#define RESERVED_FOR_EXTENTION2         15
typedef struct MessageBuffer
{
    int     nLen;
    u_char     cpData[MSG_BUFFER_SIZE];
} stMsgBuf_t;

typedef	struct OctetString
{
	u_int	numocts;
	u_char		data[MAX_OCTET_LEN];
} OctetString;
typedef enum{
	DCS_TYPE_UNKNOWN = 0,
	DCS_TYPE_KSC5601,
	DCS_TYPE_UCS2,
	DCS_TYPE_8BIT,
	DCS_TYPE_GSM7BIT,
	DCS_TYPE_DC_DM
}eTPDCSType;

#endif
