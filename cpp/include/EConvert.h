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
*       MODULE NAME          : EConvert.h
*       PURPOSE              : Expand Convert.
*       INPUT ARGUMENT       : NONE
*       OUTPUT ARGUMENT      :
*       RETURN VALUE         : NONE
*       REVISION             :     BY             WHEN            REASON
*                             -------------  --------------  -------------
*                              KANG HYUN KWAN  1998.05.11.     first coding
==============================================================================*/
#pragma once

#include <sys/types.h>

typedef struct {
	int nTokenCnt;
	char **cpToken[2];
} TokenType;

#ifdef __cplusplus
//extern "C" {
#endif

TokenType *OpenToken(void);

void CloseToken(TokenType *spTt);

int SearchToken(TokenType *spTt, char *cpToken, char *cpBuf);

void InsertToken(TokenType *spTt, char *cpToken, char *cpValue);

int HexStr2Dec(char *cpHex);

char *HexStr2Byte(char *cpHex, char *cpBuf);

int Char2Long(char *cpStr);

u_int Char2u_Long(char *cpStr);

void PCInt2Char(int nPc, u_char *ucPc);

u_int UChar2UInt ( u_char *cpStr );

int UChar2Int ( u_char *cpStr );

char *Hex2Str(char *cpHex, char *cpStr, int nStrSize);

int Sscanf(char *cpStrData, const char *cpFormat, ...);

char *ECvtEnv2NormalStr(const char *cpSour, char *cpTarget);

char *ESpecial2NormalStr(char *cpSour, char *cpTar);

int CryptogramEncode(char *cpSourCode, char *cpTargetCode);

int CryptogramDecode(char *cpSourCode, char *cpTargetCode);

int EEraseComment(char *cpStr);

int EEraseSpecialChar(char *cpStr);

int EEraseCommentSpecialChar(char *cpStr);

u_long decode_long(u_long decode_val);

#ifdef __cplusplus
//}
#endif
