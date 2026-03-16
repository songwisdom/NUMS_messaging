/**
 * =====================================================================================
 *
 *       @file  smscTime.h
 *
 *    	 @brief Time library function
 *
 *       Version:  1.0
 *       Compiler:  cc (HP-UX)
 *       @date:  04/08/09 13:58:13
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

#ifndef _SMSCTIME_H
#define _SMSCTIME_H


#include <FLKBaseDef.h>

time_t      getCurTime();
struct tm   *getCurTm();
struct tm   *time2tm(time_t );
time_t      tm2time(struct tm *);
void        printTm(struct tm *);
char			*time2str( char *cpResult, int nResultSize, const char *cpFormat, time_t tTime);
time_t		str2time(const char *cpTime, const char *cpFormat);
time_t		str2time2(const char *cpTime, const char *cpFormat);
char 			*time2decchar7(char *cpResult, time_t tTime);
time_t		decchar72time(char *cpDecTime);
/** @} */

/* SmscTime.c */
void set_smsc_dst_time(int MsType, u_char *SubmissionTime, u_char *RetVar);
bool get_scts_init(void);
#endif
