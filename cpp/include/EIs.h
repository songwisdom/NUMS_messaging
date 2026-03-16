/***********************************************************************
*                                                                      *
*   Copyright (c) 1997 LG Information & Communications                 *
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
*       MODULE NAME          : EIpc.h
*       PURPOSE              : Expand Is.
*       INPUT ARGUMENT       : NONE
*       OUTPUT ARGUMENT      :
*       RETURN VALUE         : NONE
*       REVISION             :     BY             WHEN            REASON
*                             -------------  --------------  -------------
*                              KANG HYUN KWAN  1998.10.15.     first co
==============================================================================*/

#ifndef __EIS_H__
#define __EIS_H__

#include "FLKBaseDef.h"

typedef struct {
	time_t lnTime;
	int    nCurCnt;
} ECheckTimeCntInsideType;

typedef struct {
	int                     nShmKey;
	int                     nSemKey;
	int                     nTerm;
	int                     nMaxCnt;
	EShmType                *spEShm;
	int                     nSemId;
	ECheckTimeCntInsideType *spData;
} ECheckTimeCntType;


#ifdef __cplusplus
//extern "C" {
#endif

int GetSwNum2HwNum(int nSize);
ECheckTimeCntType *CreateECheckTimeCnt(int nShmKey, int nSemKey, int nTerm, int nCnt);
void CloseECheckTimeCnt(ECheckTimeCntType *spEctc);
int ECheckTimeCnt(ECheckTimeCntType *spEctc);
int IsECheckTimeCnt(ECheckTimeCntType *spEctc);
int IsCorrectWildCardStr(char *cpStr, char *cpWildCardStr);


/** @} */

#ifdef __cplusplus
//}
#endif

#endif



