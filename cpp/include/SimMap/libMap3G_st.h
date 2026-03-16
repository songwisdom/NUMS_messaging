/**
 * =====================================================================================
 *
 *       @file  libMapG3.h
 *
 *    	 @brief 
 *
 *       Version:  1.0
 *       Compiler:  cc (HP-UX)
 *       @date:  04/23/09 14:46:34
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

#ifndef _H_LIB_MAP_3G_ST_
#define _H_LIB_MAP_3G_ST_
#include "flk_type.h"
#include "libMap3G_Addr.h"
#include "libMapV3FowardSM.h"

typedef V3_AddressString V3_ISDN_AddressString;

/* Choice tag constants */
#define V3_Additional_Number_msc_Number    1
#define V3_Additional_Number_sgsn_Number   2
typedef struct V3_Additional_Number
{
    int     t;
    union {
        /* t = 1 */
        V3_ISDN_AddressString  msc_Number;
        /* t = 2 */
        V3_ISDN_AddressString  sgsn_Number;
    } u;
} V3_Additional_Number;
//int decV3_Additional_Number(stMsgBuf_t *, V3_Additional_Number *, u_char);
//int encV3_Additional_Number(stMsgBuf_t *, V3_Additional_Number *, u_char);

typedef struct V3_LocationInfoWithLMSI
{
    struct {
        BIT_T lmsiPresent                : 1;
        BIT_T extensionContainerPresent  : 1;
        BIT_T gprsNodeIndicatorPresent   : 1;
        BIT_T additional_NumberPresent   : 1;
        BIT_T third_NumberPresent   : 1;
		BIT_T imsNodeIndicatorPresent 	 : 1;
    } m;
    V3_ISDN_AddressString      networkNode_Number;
    V3_LMSI                    lmsi;                   /* optional */
    V3_ExtensionContainer      extensionContainer;     /* optional */
    /* gprs Node indicator (NULL type) */           /* optional */
    V3_Additional_Number       additional_Number;      /* optional */
    V3_Additional_Number       third_Number;      /* optional */
} V3_LocationInfoWithLMSI;
//int decV3_LocationInfoWithLMSI(stMsgBuf_t *, V3_LocationInfoWithLMSI *, u_char);
//int encV3_LocationInfoWithLMSI(stMsgBuf_t *, V3_LocationInfoWithLMSI *, u_char);

typedef struct V3_RoutingInfoForSM_Res
{
    struct {
        BIT_T extensionContainerPresent  : 1;
    } m;
    V3_IMSI                    imsi;
    V3_LocationInfoWithLMSI    locationInfoWithLMSI;
    V3_ExtensionContainer      extensionContainer;         /* optional */
    V3_OpenType                extElement;                 /* optional */
} V3_RoutingInfoForSM_Res;

#define MAX_SMEA_LEN            	12      /* 1..12 */
typedef u_char	V3_BOOLEAN;
typedef int     V3_SM_RP_MTI;
typedef struct V3_SM_RP_SMEA
{
    u_int    numocts;
    u_char     data[MAX_SMEA_LEN];
} V3_SM_RP_SMEA;
typedef struct V3_RoutingInfoForSM_Arg
{
    struct {
        BIT_T extensionContainerPresent      : 1;
        BIT_T gprsSupportIndicatorPresent    : 1;
        BIT_T sm_RP_MTIPresent               : 1;
        BIT_T sm_RP_SMEAPresent              : 1;
    } m;
    V3_ISDN_AddressString       msisdn;
    V3_BOOLEAN                  sm_RP_PRI;                  /* boolEAN  */
    V3_AddressString            serviceCentreAddress;
    V3_ExtensionContainer       extensionContainer;         /* optional */
    /* gprs supprort indicator (NULL Type) */               /* optional */
    V3_SM_RP_MTI                sm_RP_MTI;                  /* optional */
    V3_SM_RP_SMEA               sm_RP_SMEA;                 /* optional */
} V3_RoutingInfoForSM_Arg;
int encV3_RoutingInfoForSM_Res(stMsgBuf_t *spMsg, V3_RoutingInfoForSM_Res *pD, u_char cTag);
int decV3_RoutingInfoForSM_Arg(stMsgBuf_t *spMsg, V3_RoutingInfoForSM_Arg *pD, u_char cTag);
//////////////////////////////////
//
typedef int V3_AbsentSubscriberDiagnosticSM;       /* 0..255 */
typedef enum
{
	v3_memoryCapacityExceeded  = 0,
	v3_absentSubscriber        = 1,
	v3_successfulTransfer      = 2
} V3_SM_DeliveryOutcome_Root;
typedef V3_SM_DeliveryOutcome_Root V3_SM_DeliveryOutcome;
typedef struct V3_ReportSM_DeliveryStatusArg
{
    struct {
        BIT_T reserved                                      : 1;
        BIT_T deliveryOutcomePresent                		: 1;
        BIT_T absentSubscriberDiagnosticSMPresent            : 1;
        BIT_T extensionContainerPresent                      : 1;
        BIT_T gprsSupportIndicatorPresent                    : 1;
        BIT_T deliveryOutcomeIndicatorPresent                : 1;
        BIT_T additionalSM_DeliveryOutcomePresent            : 1;
        BIT_T additionalAbsentSubscriberDiagnosticSMPresent  : 1;
    } m;
	struct {
        BIT_T ipsmgwAbsentSubscriberDiagnosticSMPresent     : 1;
        BIT_T ipsmgwSM_DeliveryOutcomePresent               : 1;
        BIT_T ipsmgwDeliveryOutcomeIndicatorPresent         : 1;
        BIT_T reserved                                      : 5;
    } n;
    V3_ISDN_AddressString              msisdn;
    V3_AddressString                   serviceCentreAddress;
    V3_SM_DeliveryOutcome              sm_DeliveryOutcome;
    V3_AbsentSubscriberDiagnosticSM    absentSubscriberDiagnosticSM;  /* O */
    V3_ExtensionContainer              extensionContainer;            /* O */
    /* gprs support indicator NULL type     */                        /* O */
    /* delivery outcome indicator NULL type */                        /* O */
    V3_SM_DeliveryOutcome              additionalSM_DeliveryOutcome;  /* O */
    V3_AbsentSubscriberDiagnosticSM    additionalAbsentSubscriberDiagnosticSM;/*O*/
	V3_SM_DeliveryOutcome              ipsmgwSM_DeliveryOutcome;
    V3_AbsentSubscriberDiagnosticSM    ipsmgwAbsentSubscriberDiagnosticSM;/*O*/
} V3_ReportSM_DeliveryStatusArg;
#define V3_SM_RP_DA_imsi                       1
#define V3_SM_RP_DA_lmsi                       2
#define V3_SM_RP_DA_serviceCentreAddressDA     3
#define V3_SM_RP_DA_noSM_RP_DA                 4
typedef struct SM_RP_DA
{
    int     t;
    union {
        V3_IMSI            imsi;
        V3_LMSI            lmsi;
        V3_AddressString   serviceCentreAddressDA;
        /* no DA */
    } u;
} V3_SM_RP_DA;
/* Choice tag constants */
#define V3_SM_RP_OA_msisdn                     1
#define V3_SM_RP_OA_serviceCentreAddressOA     2
#define V3_SM_RP_OA_noSM_RP_OA                 3

#define MAX_SIGNALINFO_LEN      	200
typedef struct SM_RP_OA
{
    int     t;
    union {
        V3_ISDN_AddressString  msisdn;
        V3_AddressString       serviceCentreAddressOA;
        /* no OA */
    } u;
} V3_SM_RP_OA;
/*
typedef struct V3_SignalInfo
{
    int    numocts;
    u_char     data[MAX_SIGNALINFO_LEN];
} V3_SignalInfo;
*/
typedef struct V3_MT_ForwardSM_Arg
{
    struct {
        BIT_T moreMessageToSendPresent   : 1;
        BIT_T extensionContainerPresent  : 1;
    } m;

    V3_SM_RP_DA            sm_RP_DA;
    V3_SM_RP_OA            sm_RP_OA;
    V3_SignalInfo          sm_RP_UI;
    /* more message to send */                  /* optional */
    V3_ExtensionContainer  extensionContainer;     /* optioanl */
    V3_OpenType            extElement;
} V3_MT_ForwardSM_Arg;

typedef struct V3_MO_ForwardSM_Arg2 {
    struct {
        //Linux porting
        BIT_T reserved                  : 6;
        BIT_T imsiPresent                : 1;
        BIT_T extensionContainerPresent  : 1;
    } m;
    //}__attribute__ ((packed)) m;
    V3_SM_RP_DA            sm_RP_DA;
    V3_SM_RP_OA            sm_RP_OA;
    V3_SignalInfo          sm_RP_UI;
    V3_ExtensionContainer  extensionContainer;     /* optional */
    V3_IMSI                imsi;                   /* optional */
} V3_MO_ForwardSM_Arg2;

typedef struct V3_SubscriptionInfo
{
	struct {
		unsigned extensionContainerPresent  : 1;
	} m;
	V3_ExtensionContainer      extensionContainer;     /* optional */                                  
} V3_SubscriptionInfo; 
typedef struct V3_AnyTimeSubsInterrogation_Res                                                         
{                                                                                                      
	struct {                                                                                           
		unsigned extensionContainerPresent  : 1;                                                       
	} m;                                                                                               
	V3_ExtensionContainer      extensionContainer;         /* optional */                              
} V3_AnyTimeSubsInterrogation_Res;  
typedef struct V3_AnyTimeSubsInterrogation_Arg                                                         
{                                                                                                      
	struct {                                                                                           
		unsigned extensionContainerPresent      : 1;                                                   
	} m;                                                                                               
	V3_AddressString            msisdn;                                                                
	V3_SubscriptionInfo         subscriptionInfo;                                                      
	V3_AddressString            gsmSCFAddress;                                                         
	V3_ExtensionContainer       extensionContainer;                                                    
} V3_AnyTimeSubsInterrogation_Arg;    
int decV3_MT_ForwardSM_Arg(stMsgBuf_t *spMsg, V3_MT_ForwardSM_Arg *pD, u_char cTag);
////////////////////
#ifdef __cplusplus
extern "C" {
#endif
int V3_TBCDString(u_char *cpIn, u_char *cpOut, int nLen);
int decV3_AddressString(stMsgBuf_t *spMsg, V3_AddressString *pD, u_char cTag);
int decV3_ISDN_AddressString(stMsgBuf_t *spMsg, V3_ISDN_AddressString *pD, u_char cTag);
int decV3_NULL(stMsgBuf_t *spMsg, u_char *pD, u_char cTag);
int decV3_ExtensionContainer(stMsgBuf_t *spMsg, V3_ExtensionContainer *pD, u_char cTag);
int decV3_AnyTimeSubsInterrogation_Arg(stMsgBuf_t *spMsg, V3_AnyTimeSubsInterrogation_Arg *pD, u_char cTag);
int decV3_MO_ForwardSM_Arg(stMsgBuf_t *spMsg, V3_MO_ForwardSM_Arg *pD, u_char cTag);
int decV3_MO_ForwardSM_Arg2(stMsgBuf_t *spMsg, V3_MO_ForwardSM_Arg2 *pD, u_char cTag);
#ifdef __cplusplus
}
#endif
#endif
