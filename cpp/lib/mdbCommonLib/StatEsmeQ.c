/**
 * =====================================================================================
 *
 *       @file  StatEsmeQ.c
 *
 *    	 @brief
 *
 *       Version:  1.0
 *       Compiler:  cc (HP-UX)
 *       @date:  06/23/10 17:24:52
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
#include <spdlog/spdlog.h>
#include "FLKBaseDef.h"
#include "SmsIpc.h"
#include "Esme_Struct.h"
#include "MdbTableInfo.h"
#include "MdbDmaqi.h"
#include <sys/msg.h>


#include	"ECommon.h"
#include    "Debug.h"
#include 	"StatRecord.h"
#include 	"StatTable.h"
#include 	"SmsStat.h"
//#include 	"SmsStatistics.h"
#include 	"SmsStat.h"
#include 	<errno.h>


#define IS_ESME_QUEUE(x) (((x) == TOESME) ? true : false)
extern sttAqiInfo *gpAqiInfo[MAX_DBMS_CNT];

int initEsmeIdQCheck()
{
	return 2000;
}

int  checkEsmeMaxCount (int cnt)
{
	static int MaxQueueCnt = -1;
//	int ret = 0;
	if ( MaxQueueCnt == -1 ) {
		MaxQueueCnt = initEsmeIdQCheck();
	}
	if ( cnt < 0 ) {
		return 0;
	} else if ( cnt > MaxQueueCnt ) {
		return MaxQueueCnt;
	} else {
		return cnt;
	}
}

stEsmeIDInfo *getTupleEsmeSysId(u_int esmeid)
{
	int		tableId = R_DESMESYSTEMID;
	int 	attId = DB_SYSID_ESME_ID;
	int 	tupId = 0;
	int 	nDbmsHandle = DBMS_PLUS;
	char		*noAlign = NULL;

	tupId = dma_getTupId(nDbmsHandle, tableId, attId, (char *)&esmeid, tupId);
	if (tupId < 0 ) {
		return NULL;
	}

	if (dma_table_retr(nDbmsHandle, tableId, tupId, &noAlign) < 0) {
		return NULL;
	}

	return (stEsmeIDInfo *)noAlign;
}

enum { STAT_QUEUE_TYPE_NONE, STAT_QUEUE_TYPE_ESME, STAT_QUEUE_TYPE_ESME_SVC };
static u_char getEsmeQueueType (long key )
{
	switch (key) {
	case TOESME :
		return STAT_QUEUE_TYPE_ESME;
	case TOSVCREQ :
		return STAT_QUEUE_TYPE_ESME_SVC;
	default :
		return STAT_QUEUE_TYPE_NONE;
	}

}
void statEsmeIdMsgQSend(long key, u_int esme_id )
{
	stEsmeIDInfo	*spEsmeID;
	u_char MsgQType = STAT_QUEUE_TYPE_NONE;

	MsgQType = getEsmeQueueType(key);

	if ( MsgQType == STAT_QUEUE_TYPE_NONE ) {
		return;
	}


	spEsmeID = getTupleEsmeSysId(esme_id);
	if ( spEsmeID != NULL ) {
		spEsmeID->ucEsmeQType = MsgQType;
		spEsmeID->usEsmeQCnt++;
		spEsmeID->usEsmeQCnt = checkEsmeMaxCount(spEsmeID->usEsmeQCnt);
	}

	return;
}

void statEsmeIdMsgQRecv(long key, u_int esme_id)
{
	stEsmeIDInfo	*spEsmeID;
	u_char MsgQType = STAT_QUEUE_TYPE_NONE;

	MsgQType = getEsmeQueueType(key);

	if ( MsgQType == STAT_QUEUE_TYPE_NONE ) {
		return;
	}

	spEsmeID = getTupleEsmeSysId(esme_id);
	if ( spEsmeID != NULL ) {
		spEsmeID->ucEsmeQType = MsgQType;
		spEsmeID->usEsmeQCnt--;
		spEsmeID->usEsmeQCnt = checkEsmeMaxCount(spEsmeID->usEsmeQCnt);
	}

	return;
}

int get_msgq_id(key_t key)
{
	int	msgq_id;

	msgq_id = msgget(key, S_IRUSR);
	if ( msgq_id < 0 ) {
		return -1;
	}

	return msgq_id;
}
int get_msgq_count(int msgid)
{
	struct msqid_ds buf;
	int num;
	memset(&buf, 0x00, sizeof(buf));
	if (msgctl(msgid, IPC_STAT, &buf) < 0) {
		num =  0;
	} else {
		num = buf.msg_qnum;
	}

	return num;
}

bool cleanEsmeQRealCnt(int type)
{
	/* Check TO_ESME */
	static int esmeId = -1;
	static int esmeSvcId = -1;
//	int cnt = 0;

	switch ( type ) {
	case STAT_QUEUE_TYPE_ESME : // MsgQ Id Init;
		if ( esmeId < 0 ) {
			esmeId = get_msgq_id(TOESME);
			if ( esmeId < 0 ) {
				spdlog::error("[{} : {}] TO ESME Queue Initialize failed", __func__, __LINE__);
				return false;
			}
		}
//		cnt = get_msgq_count(esmeId);
		break;
	case STAT_QUEUE_TYPE_ESME_SVC : // MsgQ Id Init;
		if ( esmeSvcId < 0 ) {
			esmeSvcId = get_msgq_id(TOSVCREQ);
			if ( esmeSvcId < 0 ) {
				spdlog::error("[{} : {}] TO Service ESME Queue Initialize failed", __func__, __LINE__);
			}
			return false;
		}
//		cnt = get_msgq_count(esmeSvcId);
		break;
	default :
		return false;
	}
	/* 2. MSGQ Count Check */
	if ( get_msgq_count(esmeId) == 0 ) {
		return true;
	}

	return false;
}

stStatEsmeIdQInfo *statEsmeIdMsgQCount()
{
//    sttAqiInfo *pAqiInfo = gpAqiInfo[DBMS_PLUS];
//    Rdir *pTbl = pAqiInfo->relationDir[R_DESMESYSTEMID];
	u_char *noAlign = NULL;
//   char *ptr = NULL;
	stStatEsmeIdQInfo *ret = NULL;
	stEsmeIDInfo *spEsmeID;
	u_short usServiceID = 0x000e;
	static int esmeQId = -1;

	stRDServiceIdType   *spSvcId = NULL;

	if (dma_getTuplePtr(DBMS_PLUS, R_DSVCIDROUTE, DB_SI_SERVICE_ID, 0,
	                    (char *)&usServiceID, DB_SI_SERVICE_ID_SIZE, (char **)&spSvcId) != FLK_SUCCESS) {
		return NULL;
	}

	u_int nEsmeId = spSvcId->nEsmeID;

	if (dma_getTuplePtr(DBMS_PLUS, R_DESMESYSTEMID, DB_SYSID_ESME_ID, 0,
	                    (char *)&nEsmeId, DB_SYSID_ESME_ID_SIZE, (char **)&spEsmeID) != FLK_SUCCESS) {
		return NULL;
	}

	spEsmeID = (stEsmeIDInfo *)noAlign;
	ret = (stStatEsmeIdQInfo *)malloc(sizeof(stStatEsmeIdQInfo));
	bzero(ret, sizeof(stStatEsmeIdQInfo));

	stStatEsmeIdQRecord *p = &ret->EsmeIdQCntInfo[0];
	memcpy(p->sysid, spEsmeID->cpSysId, STAT_ESMEID_SYSID_SIZE);
	p->EsmeId  = spEsmeID->nEsmeID;
	p->EsmeQueueType = STAT_QUEUE_TYPE_ESME_SVC;
	ret->EsmeIdCount = 1;
	if ( esmeQId < 0 ) {
		esmeQId = get_msgq_id((0x10000000 | nEsmeId));
	}
	p->count = get_msgq_count(esmeQId);

	return ret;

}
/*
stStatEsmeIdQInfo *statEsmeIdMsgQCount()
{
	sttAqiInfo *pAqiInfo = gpAqiInfo[DBMS_PLUS];
	Rdir *pTbl = pAqiInfo->relationDir[R_DESMESYSTEMID];
	u_char *noAlign;
	stStatEsmeIdQInfo *ret = (stStatEsmeIdQInfo*)malloc(sizeof(stStatEsmeIdQInfo));
	bzero(ret, sizeof(stStatEsmeIdQInfo));
	stEsmeIDInfo *spEsmeID;
	bool esmeIDCleanFlag = cleanEsmeQRealCnt(STAT_QUEUE_TYPE_ESME);
	bool esmeSvcIDCleanFlag = cleanEsmeQRealCnt(STAT_QUEUE_TYPE_ESME_SVC);
	bool cleanFlag = false;

	for(int nLoop = 0; nLoop < pTbl->tupleSpaceCnt; ++nLoop) {
		if ( dma_table_retr(DBMS_PLUS, R_DESMESYSTEMID, nLoop, (char **)&noAlign) < 0 )
		{
			continue;
		}

		spEsmeID = (stEsmeIDInfo *)noAlign;

		if ( spEsmeID->ucEsmeQType == STAT_QUEUE_TYPE_ESME ) {
			cleanFlag = esmeIDCleanFlag;
		}else if (spEsmeID->ucEsmeQType == STAT_QUEUE_TYPE_ESME_SVC ) {
			cleanFlag = esmeSvcIDCleanFlag;
		}

		if ( cleanFlag == true ) {
			spEsmeID->usEsmeQCnt = 0;
		}

		stStatEsmeIdQRecord *p = &ret->EsmeIdQCntInfo[ret->EsmeIdCount];
		ret->EsmeIdCount++;
		memcpy(p->sysid, spEsmeID->cpSysId, STAT_ESMEID_SYSID_SIZE);
		p->EsmeId  = spEsmeID->nEsmeID;
		p->EsmeQueueType = spEsmeID->ucEsmeQType;
		p->count = spEsmeID->usEsmeQCnt;
	}
	return ret;
}
*/
