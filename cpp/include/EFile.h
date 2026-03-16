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
*       MODULE NAME          : EFile.h
*       PURPOSE              : Expand I/O.
*       INPUT ARGUMENT       : NONE
*       OUTPUT ARGUMENT      :
*       RETURN VALUE         : NONE
*       REVISION             :     BY             WHEN            REASON
*                             -------------  --------------  -------------
*                              KANG HYUN KWAN  1997.08.25.     first coding
==============================================================================*/
#pragma once

#include <stdio.h>
typedef struct {
	int nCnt;
	int nStartPos;
	int nEndPos;
} EFileQueueInType;

typedef struct {
	EFileQueueInType stInfo;
	EFileQueueInType stUndoInfo;
	char *cpFileName;
	FILE *spFp;
	int nSize;
} EFileQueueType;

typedef struct {
	char *cpFileName;
	time_t nTime;
} EFileVariationType;

typedef struct {
	int nShmKey;
	int nSemKey;
	char cpFileName[BUF_SSIZE];
	int nSize;
	int bOverflowFlag;
	int nMode;
} ESingleCacheType;

#ifdef __cplusplus
//extern "C" {
#endif

int GetFileLine(char *cpFilename);

int EGetFileLineAndMaxLen(char *cpFileName, int *nLine, int *nMaxLen);

int MakeBackwardFile(char *InFile, char *OutFile);

char *GetFileName(char *cpPath, char *cpFileName);

char *GetPathName(char *pPath, char *cpPathName);

char *Path2Name(char *cpBuf);

int GetFileSize(char *cpFileName);

char *GetTempFileName(char *cpBuf);

u_int CreateMessageID(char *cpFileName);

int GetOneDayID(char *cpFileName, time_t nTime, int nStartnum);

int ReadOneDayID(char *cpFileName);

void RecLock(FILE *spFp, int Type, int nStart, int nLen);

void RecLockNormal(int nFileNo, int Type, int nStart, int nLen);

void MakeDirectory(char *cpPath);

void FMemset(FILE *spFp, int nPos, char cCh, int nSize);

void FInsert(FILE *spFp, int nPos, char *cpBuf, int nOldSize, int nNewSize);

void FMemcpy(FILE *spFp, int nDestPos, int nSourPos, int nSize);

char *Fgets(char *cpBuf, int nSize, FILE *spFp);

int Access(char *cpPath, int nMode);

int GetFgetsReadLen(void);

EFileQueueType *OpenFileQueue(char *cpFilename);

void CloseFileQueue(EFileQueueType *spEfq);

void UnlinkFileQueue(EFileQueueType *spEfq);


int GetFileQueue(EFileQueueType *spEfq, char *cpBuf, int nMaxBufSize);

int PutFileQueue(EFileQueueType *spEfq, char *cpBuf, int nSize);

void GarbageCollectFileQueue(EFileQueueType *spEfq);

void UndoFileQueue(EFileQueueType *spEfq);

time_t EGetFileMTime(char *cpFileName);

void EDeleteFileLine(char *cpFileName, int nLines);

int GetEFileStrCnt(char *cpFileName, char *cpStr);

int GetEFileStrStr(char *cpFileName, char *cpStr);

int GetEFileStrCmpGE(char *cpFileName, char *cpStr);

/********************************************
      Check to Modify File
********************************************/
EFileVariationType *SetEFileVariation(char *cpFileName);

int IsEFileVariation(EFileVariationType *spEfv);

void CloseEFileVariation(EFileVariationType *spEfv);

int EUnlinkPath(char *cpPath, char *cpFileName);

int EGetCntDirFiles(char *cpPath);

int EGetCntAndMaxLenDirFiles(char *cpPath, int *nCnt, int *nMaxLen);

char **EGetDirFiles(char *cpPath, int *nCnt);

void EFreeStrs(char **cpStrs);

char **EGetLs(char *cpPath, char *cpFileMode, int *nCnt);

int EExceptRemoveFiles(char *cpPath, char *cpFileMode, int nCnt, char **cpFiles);

int EPurgeFile(char *cpPath, char *cpFileName, /* cpFileName :
                                                              1) YYYYMMDDFileName
                                                              2) YYYYMMDDFileName.
                                                              3) YYYYMMDDFileName.
                                                              4) FileNameYYYYMMDD
                                                              5) FileNameYYYYMMDD.
                                                              6) FileNameYYYYMMDD.ExtName
                                                              7) FileName.YYYYMMDD
										   */
               time_t nBaseTime, int nDays);

int Text2VarFile(char *cpSourFileName, char *cpTargetFileName);

int Text2VarFile(char *cpSourFileName, char *cpTargetFileName);

size_t EGetFileData(FILE *spFp, size_t nPos, char *cpBuf, size_t nLen);


/*********************************************************************************
    Disk Cache
*********************************************************************************/

int ECacheFopenText(char *cpFileName);

int ECacheFgets(char *cpBuf, int nBufSize, int nPos);

int ECacheFeof(int nPos);

#ifdef __cplusplus
//}
#endif
