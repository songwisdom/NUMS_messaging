/**
 * =====================================================================================
 *
 *       @file  libMapV3Extension.h
 *
 *    	 @brief 
 *
 *       Version:  1.0
 *       Compiler:  cc (HP-UX)
 *       @date:  04/23/09 17:45:47
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

#ifndef _H_LIB_MAP_V3_EXTENSION_
#define _H_LIB_MAP_V3_EXTENSION_
#include "flk_type.h"
#include "libMapComm.h"

#define MAX_SUB_IDS            		128
#define OPEN_TYPE_LEN           	50

#define     EXTENSION_OID_TABLE_COUNT  	15

#define		OID_SKT_MIN                 0
#define		OID_SKT_SECONDARY_MSISDN    1
#define		OID_SKT_OSFI                2
#define		OID_SKT_ESN                 3
#define		OID_SKT_CAPA                4
#define		OID_SKT_ROAMING_INDICATOR   5
#define		OID_SKT_ACCESSDENIEDREASON  6
#define		OID_SKT_ROAMING_PMN			7
#define     OID_SKT_SUBSCRIPTIONINFO    8       //FR220 010_PLUS_SVC 
#define     OID_SKT_NPFOWARDINFORMATION_ID 9 //DR610 LSH
#define     OID_SKT_SMSLTEMTINFO        10//DR610 LSH
#define 	OID_SKT_IPMG_REFERENCE_ID 	11
#define 	OID_SKT_IPMG_MSISDN 		12
typedef struct
{
    u_int    numids;
    u_char     subid[MAX_SUB_IDS];
} Oid;

typedef struct
{
    u_int    numocts;
    u_char     data[OPEN_TYPE_LEN];
} V3_OpenType;

typedef struct
{
    u_int    numids;
    u_int    subid[MAX_SUB_IDS];
} V3_ObjectIdentifier;

typedef struct V3_PrivateExtension
{
    struct {
        BIT_T extTypePresent : 1;
    } m;
    V3_ObjectIdentifier    extId;
    V3_OpenType            extType;
} V3_PrivateExtension;

typedef struct V3_PrivateExtensionList
{
	struct {
		BIT_T minPresent     : 1;    // Mobile Identification Number
        BIT_T esnPresent     : 1;    // Electronic Serial Number
        BIT_T capaPresent    : 1;    // Capability
        BIT_T osfiPresent    : 1;    // Originating Service Feature Indicator
        BIT_T roamPresent    : 1;    // Roaming Indicator
		BIT_T npfiPresent	 : 1; 	// NPFI
        BIT_T msisdnPresent  : 1;    // Secondary MSISDN (Forward MIN)
        BIT_T adrPresent     : 1;    // Access Denied Reason
		BIT_T roampmnPresent : 1;	 // Roaming PMN
		BIT_T subscriptioninfoPresent : 1;   // FR220  010_PLUS_SVC
		BIT_T ltesmsPresent : 1;
		//BIT_T smsmosacPresent: 1;  // FR401 SMSMO_SAC LSH
		BIT_T ipmg_refidPresent : 1; // FR410 IPMG Reference ID
		BIT_T ipmg_msisdnPresent : 1; // FR410 IPMG msisdn
		BIT_T smsosfiPresent    : 1;    // SMS Originating Service Feature Indicator
		BIT_T smsf_msisdnPresent    : 1;    // SMSF Number ID
	} m;

	int		n;

	V3_OpenType min;
    V3_OpenType esn;
    V3_OpenType capa;
    V3_OpenType osfi;
    V3_OpenType roam;
    V3_OpenType npfi;
    V3_OpenType msisdn;
    V3_OpenType adr;
    V3_OpenType roampmn;
	V3_OpenType subscriptioninfo;   //FR220 010_PLUS_SVC
	//V3_OpenType smsmosac;           //FR401 SMSMO_SAC LSH
	V3_OpenType ltesms;
	V3_OpenType ipmg_refId; 		// FR410 Reference ID
	V3_OpenType ipmg_msisdn; 		// FR410 msisdnPresent
	V3_OpenType smsosfi;
    V3_OpenType smsf_msisdn;
	V3_PrivateExtension    privateExtension;
} V3_PrivateExtensionList;

typedef struct V3_ExternsionContainer
{
    struct {
        BIT_T privateExtensionListPresent    : 1;
        BIT_T pcs_ExtensionsPresent          : 1;
    } m;
    V3_PrivateExtensionList    privateExtensionList;
} V3_ExtensionContainer;

/* encV3_Extension.c */
int encV3_PrivateExtension(stMsgBuf_t *spMsg, V3_PrivateExtension *pD, u_char cTag);
int encV3_PrivateExtensionList(stMsgBuf_t *spMsg, V3_PrivateExtensionList *pD, u_char cTag);
int encV3_ExtensionContainer(u_char *spMsg, V3_ExtensionContainer *pD, u_char cTag);

/* V3_ObjectId.c */
int encV3_OpenType(stMsgBuf_t *spMsg, V3_OpenType *pD, u_char cTag);
int toBase(stMsgBuf_t *spMsg, int nBase, u_int nData);
u_int power(int nBase, int nPow);
u_int fromBase(stMsgBuf_t *spMsg, int nBase, u_int *nData);
int encV3_ObjectIdentifier(stMsgBuf_t *spMsg, V3_ObjectIdentifier *pD, u_char cTag);
int decV3_ObjectIdentifier(stMsgBuf_t *spMsg, V3_ObjectIdentifier *pD, u_char cTag);
int decV3_ObjectId(stMsgBuf_t *spMsg, V3_ObjectIdentifier *pD, u_char cTag);
int encV3_ObjectId(stMsgBuf_t *spMsg, V3_ObjectIdentifier *pD, u_char cTag);
#endif
