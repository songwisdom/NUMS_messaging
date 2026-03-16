/***********************************************************************
*                                                                      *
*   Copyright (c) 1997-1998 LG Information & Communications            *
*   All rights reserved.                                               *
*                                                                      *
*   This program contains confidential and proprietary information of  *
*   LGIC, and any reproduction, disclosure, or use in whole or in part *
*   is expressly  prohibited, except as may be specifically authorized *
*   by prior written agreement or permission of LGIC.                  *
*                                                                      *
*               LG Information & Communications, Ltd.                  *
*        395-65 , Shindaebang-dong, Dongjak-gu, Seoul, 150-010 Korea   *
*                                                                      *
***********************************************************************/
/*===============================================================================
*       MODULE NAME          : ETime.h
*       PURPOSE              : Expand time.
*       INPUT ARGUMENT       : NONE
*       OUTPUT ARGUMENT      :
*       RETURN VALUE         : NONE
*       REVISION             :     BY             WHEN            REASON
*                             -------------  --------------  -------------
*                              KANG HYUN KWAN  1997.08.25.     first coding
==============================================================================*/

#ifndef __ETIME_H__
#define __ETIME_H__

#include <time.h>
#include <setjmp.h>

#define ETIME_BEFORE_MODE 0
#define ETIME_NOW_MODE    1
#define ETIME_AFTER_MODE  2

#define ONEDAYSECOND 86400

#define TIMEDATASIZE 16


extern jmp_buf stJmpBufEnv;

//#ifdef __cplusplus
//extern "C" {
//#endif

time_t Str2Time(char *cpStr);
char *Time2Str(time_t stTimedata, char *cpBuf);
char *ETimeT2Str(time_t stTimedata, char *cpBuf);
time_t FormatStr2Time(const char *cpStr, const char *cpFormat);
const char *Time2FormatStr(time_t stTimedata, const char *cpFormat, char *cpBuf);
int TimeOutFunc(int nSignal);
int SetTimeOut(void);
int IsTimeOut(void);
int ResetTimeOut(void);
int GetOneDayTimePos(time_t lnTime, int nDivision);
time_t GetOneDayTimePosTimeMode(time_t lnTime, int nDivision, int nPos, int nMode);
time_t GetOneDayInterval(int nDivision);
time_t GetOneDayNearTime(time_t lnTime, int nDivision);
time_t GetStartDayTime(time_t lnTime);
time_t GetEndDayTime(time_t lnTime);
time_t GetStartMonthTime(time_t lnTime);
time_t GetEndMonthTime(time_t lnTime);
int EDay(int *nDay);
int GetELocalDay(time_t lnTime);
char *GetEDate(int nDay, char *cpBuf);
int GetMonthDay(int nYear, int nMonth);
time_t EGetFirmTime(time_t lnTime, int nSec);
char **EGetBeforeDateStrs(time_t lnTime, int nDays);
int IsToday(time_t lnTime);
/** @} */

//#ifdef __cplusplus
//}
//#endif

#endif



