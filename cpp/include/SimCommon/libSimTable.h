/**
 * =====================================================================================
 *
 *       @file  libSimTable.h
 *
 *    	 @brief 
 *
 *       Version:  1.0
 *       Compiler:  cc (HP-UX)
 *       @date:  04/20/09 15:23:17
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

#ifndef _H_LIB_SIM_TABLE_
#define _H_LIB_SIM_TABLE_
#include "flk_type.h"
typedef enum { APP_ID_TYPE_HLR = 3, APP_ID_TYPE_MSC = 4, APP_ID_TYPE_WCDMA= 5, APP_ID_TYPE_LTE = 6,  APP_ID_TYPE_MO = 10 } APP_ID_TYPE;

#ifdef __cplusplus
extern "C" {
#endif
int releaseSimTableInfo ( APP_ID_TYPE eAppIdType );
int setSimTableInfo ( APP_ID_TYPE eAppIdType, int *nRetAppId, int *nRetInsId );
#ifdef __cplusplus
}
#endif
#endif
