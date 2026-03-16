/*******************************************************************************
*
*     Copyright (c) 2003 FEELINGK Co., Ltd.
*     All rights reserved.
*
*     This program contains confidential and proprietary information of
*     FEELINGK, and any reproduction, disclosure, or use in whole or in part
*     by prior written agreement or permission of FEELINGK.
*
*                            FEELINGK Co., Ltd.
*     3F Ace Techno Twin Tower 1, 212-1 Kuro-Dong, Kuro-Gu, Seoul 152-050 Korea
*
*******************************************************************************/
/*===============================================================================
*       MODULE NAME          : EIpc.h
*       PURPOSE              : Expand IPC.
*       INPUT ARGUMENT       : NONE
*       OUTPUT ARGUMENT      :
*       RETURN VALUE         : NONE
*       REVISION             :     BY             WHEN            REASON
*                             -------------  --------------  -------------
*                              KANG HYUN KWAN  1997.08.25.     first co
==============================================================================*/

#ifndef __EIPC_H__
#define __EIPC_H__

#include <sys/shm.h>
#include <sys/msg.h>
#include <sys/sem.h>

#include "EFile.h"

typedef union {
	int val;
//  struct semid_ds *buf;
//  unsigned short *array;
} uSemop;

typedef struct {
	long lnType;
	char cpBuf[BUF_LSIZE];
} MQType;

typedef struct {
	long lnMqKey;                    /* Mesg Queue Key               */
	int nMqid;                       /* Mesg Queue Id                */
	long lnSize;                     /* Data Length                  */
	int nPermFlag;                   /* Perm Flag                    */
	struct msqid_ds stStatus;        /* Status                       */
	int nTimeOut;                    /* Time out (second)            */
	int nFatalTimeOut;               /* Time out for Fatal (second)  */
	char cpTempFileName[BUF_SSIZE];  /* Temp Filename                */
	char cDebug;                     /* For debug                    */
	unsigned lnError;                /* SMSC Error Code              */
	int  nMode;                      /* Mode                         */
	MQType stMq;                     /* MQ Buf                       */
	EFileQueueType   *spEfq;         /* Expand File Queue            */
	long lnDebugType;
} EMQType;
//} __attribute__ ((packed)) EMQType;

#define EIPC_SHM_NEW_MODE    1
#define EIPC_SHM_OPEN_MODE   2

typedef struct {
	long lnShmKey;
	int nShmId;
	int nSize;
	int nMode;
	struct shmid_ds stStatus;
	char *cpBuf;
} EShmType;

/***********************************************************************
        ShmMemBase : hkkang 1998.06.24
***********************************************************************/

typedef struct {
	int nKey;
	int nSize;
	long lnPos;
} EShmMemBaseValueType;

typedef struct {
	int nCnt;
	int nPos;
	long lnPos;
} EShmMemBaseHeaderType;

typedef struct {
	EShmMemBaseHeaderType *spHeader;
	EShmMemBaseValueType *spValue;
	EShmType *spShm;
} EShmMemBaseType;

/***********************************************************************
        ShmMemBase : hkkang 1998.06.24
***********************************************************************/

typedef struct {
	long lnStartPtr;
	long lnEndPtr;
	int nCnt;
	int nShmKey;
	int nSemKey;
	int nSize;
} EMsgQInfoType;

typedef struct {
	EMsgQInfoType *spHeader;
	EShmType *spShm;
	char *cpBuf;
	int nSemId;
	int nFlag;
} EMsgQType;


/***************************************************************************
           Expand Shared Memory Queue 4 Schd : hkkang 1998.08.20
***************************************************************************/
#define ESHMQ_SMALL_HEADER_SIZE sizeof(int)*2

typedef struct {
	int nPid1;
	int nPid2;
	char *cpBuf;
} EShmQBodyType;

typedef struct {
	long lnStartPtr;
	long lnEndPtr;
	int nShmKey;
	int nSemKey;
	int nCnt;
	int nSize;
	int nTotalCnt;
	int nTotalSize;
} EShmQInfoType;

typedef struct {
	EShmQInfoType *spHeader;
	EShmType *spShm;
	char *cpBuf;
	int nPid;
	int nSemId;
	int nFlag;
	int nRecordSize;
} EShmQType;

//
//	SKT Service (Flexible Retry)
//
typedef struct {
	u_int 	ulMsgId;
	char            	cServiceId[6];
	u_short  	usMdb;
	short  	usServer;
	time_t          timeout;

	int             nFlag;
	int   		 	lnPrevPtr;
	int 			lnNextPtr;
} Schd4DataType;

typedef struct {
	time_t          tUpdateTime;
	int             nMode;
	int             nTimeFlag;
	int             nQueueFlag;
	int             nSTime;
	int             nETime;
	int             nQueueCnt;
	int             nFlxModeFrame;
	int             nEvtModeFrame;
	int             nUseFrame;
} FlxRetryType;


#ifdef __cplusplus
//extern "C" {
#endif
void SetMsgQPermission(int nPermission);
EMQType *OpenMsgQ(key_t lnKey);
EMQType *OpenMsgQBody(key_t lnKey);
EMQType *CreateMsgQ(key_t lnKey);
void CloseMsgQ(EMQType *spEmq);
int ReopenMsgQ(EMQType *spEmq, int nMode);
void SetMsgQTimeOut(EMQType *spEmq, int nTime);
void SetMsgQFatalTimeOut(EMQType *spEmq, int nTime);
void SetMsgQDebug(EMQType *spEmq);
void SetMsgQDebugDetail(EMQType *spEmq);
void ResetMsgQDebug(EMQType *spEmq);
void SetMsgQTempFile(EMQType *spEmq);
void ResetMsgQTempFile(EMQType *spEmq);
void SetMsgQUseType(EMQType *spEmq);
void SetMsgQReopen(EMQType *spEmq);
void _SetMsgQDebugLevel(EMQType *spEmq, long lnType, int *nDebugLevel, char *cpDebugFileName);
void ResetMsgQDebugLevel(EMQType *spEmq);
int RemoveMsgQ(EMQType *spEmq);
int CleanMsgQ(EMQType *spEmq);
void ShowMsgQ(EMQType *spEmq);
int SendMqMode(EMQType *spEmq, long lnType, char *cpStr, long lnSize, long lnMode);
int SendMq(EMQType *spEmq, long lnType, char *cpStr, long lnSize);
int SendMsgQMode(EMQType *spEmq, char *cpStr, long lnSize, ... );
int SendMsgQ(EMQType *spEmq, char *cpStr, long lnSize, long lnType);
long RecvMqMode(EMQType *spEmq, long lnType, int nMode);
long RecvMq(EMQType *spEmq, long lnType);
long RecvMsgQMode(EMQType *spEmq, char *cpBuf, long lnSize, long lnType, int nMode);
long RecvMsgQ(EMQType *spEmq, char *cpBuf, long lnSize, long lnType);
int GetMqNum(EMQType *spEmq);
long GetMsgQType(EMQType *spEmq);
int GetMsgQNum(EMQType *spEmq);
int ESemOp(int nSemId, int nValue);
int M_SemaP(int nSemId, int nSemNum);
int M_SemaV(int nSemId, int nSemNum);
void SemaphoreP(int nSemId);
void SemaphoreV(int nSemId);
void SetSemPermission(int nPermission);
int CreateSem(int nSemKey);
int CloseSem(int nSemId);
int RemoveSem(int nSemId);
void SetShmPermission(int nPermission);
EShmType *CreateShm(int nShmKey, int  nSize);
EShmType *OpenShm(int nShmKey);
void ShmMemcpy(EShmType *spShm, int nPos, char *cpStr, int nSize);
void CloseShm(EShmType *spShm);
int Store2DiskShm(EShmType *spShm, char *cpFileName);
int LoadFromDiskShm(EShmType *spShm, char *cpFileName);
int RemoveShm(EShmType *spShm);
void ShowShm(EShmType *spShm);
EShmMemBaseType *CreateShmMemBase(int nShmKey, int  nSize, int nCnt);
int EWriteShmMemBaseData(EShmMemBaseType *spShmMB, char *cpFileName);
int EReadShmMemBaseData(EShmMemBaseType *spShmMB, char *cpFileName);
EShmMemBaseType *OpenShmMemBase(int nShmKey);
void CloseShmMemBase(EShmMemBaseType *spShmMB);
void RemoveShmMemBase(EShmMemBaseType *spShmMB);
EShmType *CreateShmMem(EShmMemBaseType *spShmMB, int nShmKey, int  nSize);
EShmType *OpenShmMem(EShmMemBaseType *spShmMB, int nShmKey);
void CloseShmMem(EShmType *spShm);
int ERemoveMesQ(int nKey);
int ERemoveShm(int nKey);
int ERemoveSem(int nKey);
EMsgQType *CreateEMsgQueue(int nShmKey, int nSemKey, int nSize);
void CloseEMsgQueue(EMsgQType *spEmq);
int GetEMsgQueueNum(EMsgQType *spEmq);
int PutEMsgQueue(EMsgQType *spEmq, char *cpStr, int nSize);
int GetEMsgQueue(EMsgQType *spEmq, char *cpBuf, int nBufSize);
int ShowEMsgQueueInfo(EMsgQType *spEmq);
EShmQType *CreateEShmQueue(int nShmKey, int nSemKey, int nSize, int nCnt);
void CloseEShmQueue(EShmQType *spEsq);
int PutEShmQueue(EShmQType *spEsq, char *cpData);
int GetEShmQueue(EShmQType *spEsq, char *cpData);
int GetEShmQCnt(EShmQType *spEsq);
int ShowEShmQueueInfo(EShmQType *spEsq);
EShmType *CreateStatusBoard(int nShmKey, size_t tSize);
EMsgQType *CreateEMsgQueue4Flx( int nShmKey, int nSemKey, int nSize );
EMsgQType *CreateEMsgQueue4Schd4( int nShmKey, int nSemKey, int nSize );
void CloseEMsgQueue4Flx( EMsgQType *spEmq );
void InitEMsgQueue4Flx( EMsgQType       *spEmq,
                        time_t          cTime,
                        int             nMode,
                        int             nTimeFlag,
                        int             nQueueFlag,
                        int             nSTime,
                        int             nETime,
                        int             nQueueCnt,
                        int             nFlxModeFrame,
                        int             nEvtModeFrame );
int PutEMsgQueue4Flx(   EMsgQType       *spEmq,
                        int             nMode,
                        int             nTimeFlag,
                        int             nQueueFlag,
                        int             nSTime,
                        int             nETime,
                        int             nQueueCnt,
                        int             nFlxModeFrame,
                        int             nEvtModeFrame );
void ClearEMsgQueue4Flx( EMsgQType *spEmq );
int PutEMsgQueue4FlxFrame( EMsgQType *spEmq, int nFrame );
FlxRetryType *GetEMsgQueue4Flx( EMsgQType *spEmq );
int GetEMsgQueue4Flx_InProcess( EMsgQType *spEmq );

#ifdef __cplusplus
//}
#endif

#endif



