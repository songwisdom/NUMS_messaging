/**
 * =====================================================================================
 *
 *       @file  ErrorHistory.h
 *
 *    	 @brief
 *
 *       Version:  1.0
 *       Compiler:  cc (HP-UX)
 *       @date:  08/27/11 21:48:05
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
 *     7F FEELingK Tower 1, Toungdeungpo-Dong 3-Ga, Youngdeungpo-Gu, Seoul 150-033 Korea
 *
 * =====================================================================================
 */


#ifndef _H_ERROR_HISTORY_
#define _H_ERROR_HISTORY_

#include "MdbTableInfo.h" // TODO HWJ

//FR730 pkg
typedef struct {
	unsigned int    log_time;
	unsigned short  attempt;
	unsigned short  log_type;
	unsigned short  pc;
	unsigned short  error_code;
} stErrorHistoryType;

typedef enum {
	ERR_HIS_NORMAL,
	ERR_HIS_MAP_HLR,
	ERR_HIS_MAP_MSC,
	ERR_HIS_MAP_SGSN,
	ERR_HIS_MAP_MME,
	ERR_HIS_MAP_NOTICE_HLR,
	ERR_HIS_MAP_NOTICE_MSC,
	ERR_HIS_MAP_NOTICE_SGSN,
	ERR_HIS_MAP_SMSF,
	ERR_HIS_MAP_UNKNOWN
} eErrorHistoryType;

int setErrorHistory(stRDActiveTuple *spActive, int nErrorCode);
int setMapErrorHistory(stRDActiveTuple *spActive, int nErrorCode);
//int get_error_history_info(char *dbSubmissionTime, unsigned int MsgId, char **buffer, unsigned int *buf_siz);
int get_error_history_info(char *dbSubmissionTime, unsigned int MsgId, char **buffer, unsigned int *buf_siz, int *err_cnt);

int init_error_history(const char *proc_name);

const char *getMapErrorCode(int nErrorCode);

const char *getErrorCodeReason(int nErrorCode);

//FR730 pkg
int setMapNoticeErrorHistory(stRDActiveTuple *spActive, int nErrorCode);
const char *getMapNoticeErrorCode(int nErrorCode);
#endif
