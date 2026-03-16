/**
 * =====================================================================================
 *
 *       @file  ESvcUtil.c
 *
 *    	 @brief
 *
 *       Version:  1.0
 *       Compiler:  cc (HP-UX)
 *       @date:  04/10/09 22:59:31
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
#ifndef _H_ESVC_UTIL_
#define _H_ESVC_UTIL_
#include "FLKBaseDef.h"
int isExistLocationInfo( u_char *caMscId );
int isExistRoamingPMN( u_char *caRoamingPMN );
void roamingCountryCodeDelete(char *cpRoamingSrcMin);

//SR610 pkg
bool is_dia_sgd_mme_send_status(int status);
bool is_dia_sgd_smsf_send_status(int status);
bool is_dia_sgd_send_status(int status);
bool is_dia_sgd_mme_recv_status(int status);
bool is_dia_sgd_smsf_recv_status(int status);
bool is_dia_sgd_recv_status(int status);
bool is_dia_sgd_status(int status);
#endif
