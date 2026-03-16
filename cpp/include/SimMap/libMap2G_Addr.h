/**
 * =====================================================================================
 *
 *       @file  libMap2G_Addr.h
 *
 *    	 @brief 
 *
 *       Version:  1.0
 *       Compiler:  cc (HP-UX)
 *       @date:  04/23/09 20:53:02
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

#ifndef _H_LIB_MAP_2G_ADDR_
#define _H_LIB_MAP_2G_ADDR_
#include "libMapComm.h"
#include "libMap2G.h"
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
}eAddTypeOfNumber;

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
}eAddNatureOfNumber;

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
}eAddNumberingPlan;

/**********************************************
	Encoding (IS-41.D) 		4bits xxxx3210  
**********************************************/
typedef enum {
	ADDR_ED_UNKNOWN					=	0,
	ADDR_ED_BCD, 
	ADDR_ED_IA5,
	ADDR_ED_OCTET_STRING
}eAddEncoding;
int encIMSI(stMsgBuf_t *spMsg, OctetString *pD, int nTag);
int encAddTypeString(stMsgBuf_t *spMsg, AddTypeString *pD, int nTag);
#endif
