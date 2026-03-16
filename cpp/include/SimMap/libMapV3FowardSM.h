/**
 * =====================================================================================
 *
 *       @file  libMapV3FowardSM.h
 *
 *    	 @brief 
 *
 *       Version:  1.0
 *       Compiler:  cc (HP-UX)
 *       @date:  04/23/09 19:04:27
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

#ifndef _H_LIB_MAP_V3_FOWARD_SM_
#define _H_LIB_MAP_V3_FOWARD_SM_
#include "flk_type.h"
#include "libMapComm.h"
#include "libMap3G_Addr.h"
#include "libMapV3Extension.h"
#include "libMapUtil.h"


#define MAX_SIGNALINFO_LEN      	200


typedef enum {
    MTI_SMS_DELIVER = 0,
    MTI_SMS_DELIVER_REPORT = 0,
    MTI_SMS_SUBMIT = 1,
    MTI_SMS_SUBMIT_REPORT = 1,
    MTI_SMS_STATUS_REPORT = 2,
    MTI_SMS_COMMAND = 2,
    MTI_RESERVED
}MTI_PARAM;

typedef struct V3_SignalInfo
{
    int    numocts;
    u_char     data[MAX_SIGNALINFO_LEN];
} V3_SignalInfo;

typedef struct V3_Concatenated_t 
{
	u_char Active;
	u_char Tag;
	u_char Len;
	u_char RefNum;
	u_char Max;
	u_char Seq;
}V3_Concatenated_st;

typedef struct V3_SMS_Submit_t
{
	u_char				TP_RD;
	u_char				TP_VPF;
	u_char				TP_RP;
	u_char				TP_UDHI;
	u_char				TP_SRR;
	V3_AddrBuf_st 	TP_DA;
	u_char 			TP_PID;
	u_char 			TP_DCS;
	u_char 			TP_VP[7];
	u_char 			TP_UDL;
	u_char 			TP_UD[256];
	V3_AddrBuf_st 	CallBack;
	V3_Concatenated_st Concatenated;
}V3_SMS_Submit_st;
typedef struct V3_MO_ForwardSM_Arg
{       
    struct {
        BIT_T extensionContainerPresent  : 1;
        BIT_T imsiPresent                : 1;
    } m;
    V3_AddrBuf_st          sm_RP_DA;
    V3_AddrBuf_st          sm_RP_OA;
    V3_SMS_Submit_st       sm_RP_UI;
    V3_ExtensionContainer  extensionContainer;     /* optional */
    V3_IMSI                imsi;                   /* optional */
} V3_MO_ForwardSM_Arg;
#ifdef __cplusplus
extern "C" {
#endif
int encV3_MO_ForwardSM_Arg(stMsgBuf_t *spMsg, V3_MO_ForwardSM_Arg *pD, u_char cTag);
#ifdef __cplusplus
}
#endif
#endif
