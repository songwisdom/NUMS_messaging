/**
 * =====================================================================================
 *
 *       @file  libMapDebug.c
 *
 *    	 @brief 
 *
 *       Version:  1.0
 *       Compiler:  cc (HP-UX)
 *       @date:  05/05/09 20:53:18
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

#include "libMap3G_st.h"
#include "libMap2G_St.h"
#include "libMap3G_Addr.h"
#include "libMapV3FowardSM.h"
#include "libTcapHandler.h"
void dbgStr(unsigned char *cpData, int nCnt, int nUnit);
void dbgData(stMsgBuf_t *spMsg);
void dbgOctetString(char *cpDesc, OctetString *pD);
void dbgAddTypeString(char *cpDesc, AddTypeString *pD);
void dbgSmsAddress(char *cpDesc, SmsAddress *pD);
void dbgAddressString(char *cpDesc, V3_AddressString *pD);
void dbgV3RIFSMR(V3_RoutingInfoForSM_Arg *pD);
void dbgDialogue(tc_dialog_portion * pD);
void dbgV3RIFSMI(V3_RoutingInfoForSM_Res *pD);
void dbgV3MTFSMR(V3_MT_ForwardSM_Arg *pD);
void dbgV3MOFSMR(V3_MO_ForwardSM_Arg *pD);
void dbgV3MTFSMI();
void dbgSMDPP(stSMDPP_t *pD);
//void dbgV3RSMDSI(V3_ReportSM_DeliveryStatusRes *pD);

