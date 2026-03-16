/**
 * =====================================================================================
 *
 *       @file  libMapAddrUtil.h
 *
 *    	 @brief 
 *
 *       Version:  1.0
 *       Compiler:  cc (HP-UX)
 *       @date:  04/27/09 09:35:29
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

#ifndef _H_LIB_MAP_ADDR_UTIL_
#define _H_LIB_MAP_ADDR_UTIL_
#include "flk_type.h"
#include "libMapComm.h"
#include "libMapAddrUtilMacro.h"

#ifdef __cplusplus
extern "C" {
#endif
bool isAddressString (u_char *str);
int getBCDLength ( u_char *bcd, int max_oct );
int getMapBCDLength ( u_char *src, int max_oct );
int convBCD2Str( u_char *dest, int dest_size, u_char *src, int src_size );
int convStr2BCD( u_char *dest, int dest_size, u_char *src, int src_size );
int convBCD2Cdma ( u_char *dest, int dest_size, u_char *src, int src_size );
int convCdma2BCD( u_char *dest, int dest_size, u_char *src, int src_size );
int convBCD2Gsm( u_char *dest, int dest_size, u_char *src, int src_size );
int convGsm2BCD( u_char *dest, int dest_size, u_char *src, int src_size );
int convBCD2CB ( u_char *dest, int dest_size, u_char *src, int src_size );
int convCB2BCD ( u_char *dest, int dest_size, u_char *src, int src_size );
#ifdef __cplusplus
}
#endif
#endif
