/**
 @ingroup mdbCommonLib
 @{
 */
//*******************************************************************************
//
//     Copyright (c) 2003 FEELINGK Co., Ltd.
//     All rights reserved.
//
//     This program contains confidential and proprietary information of
//     FEELINGK, and any reproduction, disclosure, or use in whole or in part
//     by prior written agreement or permission of FEELINGK.
//
//                            FEELINGK Co., Ltd.
//     8F Taekyung B/D 42-42 Bongchun 10Dong Gwanak-Gu, Seoul 151-060 Korea
//
//******************************************************************************

//---------------------------------------------------------------------
//
//  PROC. NAME      : StatisticsRecordUtil.c
//  DESCRIPTION     : SMS Statistics Record Handling function
//  REVISION        :
//      BY              WHEN        REASON
//      --------------  ----------  -------------------
//      smyang          2003.04.    First Coding
//
//--------------------------------------------------------------------
#include 	<spdlog/spdlog.h>
#include	<stdio.h>
#include	<stdlib.h>
#include	<string.h>
#include	<errno.h>
#include	<sys/msg.h>
#include	<sys/sem.h>

#include	"MdbMessage.h"
#include 	"MdbTableInfo.h"
#include	"EMacro.h"
#include	"SmsIpc.h"
#include 	"MdbDmaqi.h"
#include 	"MdbQmi.h"
#include    "Debug.h"
//#include 	"StatDataStruct.h"
#include 	"MdbDataStruct.h"
//#include	"StatDbAqi.h"
#include 	"StatRecord.h"
#include 	"StatTable.h"
#include	"SmsStat.h"
//#include	"SmsStatistics.h"

#include	"EIpc.h"
#include	"smscString.h"


extern sttAqiInfo *gpAqiInfo[MAX_DBMS_CNT];

//static int	connectFlag = 0;
//static IDs	mqInfo;
//static int	nStatisticMsgQMaxBytes = 0;
int 		nStatisticSemId = 0;


unsigned char	OID_ZERO_CONTAINER[MAX_INTFACE_CLASS][MAX_ROLE_CLASS] = {
	{
		OID_ZERO_INTL_ADBMS,
		OID_ZERO_INTL_SCHD,
		OID_ZERO_INTL_UHIS,
		OID_ZERO_INTL_SDBMS,
		OID_ZERO_INTL_RDBMS,
		OID_ZERO_INTL_DDB,
		OID_ZERO_INTL_RSCHD,
		OID_ZERO_INTL_RUHIS,
		OID_ZERO_INTL_TDBMS,
		OID_ZERO_INTL_RCSDB,
		OID_ZERO_INTL_RCSSCHD,
		OID_ZERO_INTL_RCSUHIS,
		OID_ZERO_INTL_STAT_ERR,
		0xFF, 0xFF//, 0xFF
	},

	{
		OID_ZERO_NET_TCAP,
		OID_ZERO_NET_CDMA,
		OID_ZERO_NET_GSM,
		OID_ZERO_NET_GW,
		0xFF,
		OID_ZERO_NET_MME,
		OID_ZERO_NET_UDM,
		OID_ZERO_NET_SMSF,
		OID_ZERO_NET_SC_P_SGD,
		OID_ZERO_NET_SC_S_SGD,
		OID_ZERO_NET_DSDS_GSM,
		OID_ZERO_NET_DSDS_MME,
		OID_ZERO_NET_DSDS_SMSF,
		OID_ZERO_NET_CSCF,	
		//0xFF, 0xFF
	},

	{
		OID_ZERO_TCP_SMDPS,
		OID_ZERO_TCP_SMPP34,
		OID_ZERO_TCP_SMCI,
		OID_ZERO_TCP_SMPP,
		OID_ZERO_TCP_SMDP,
		OID_ZERO_TCP_NPQC,
		OID_ZERO_TCP_PLUS, 
		OID_ZERO_TCP_NUMS, 
		0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF
	}
};

unsigned char	OID_ONE_CONTAINER[MAX_DIRECTION_CLASS][MAX_EVENT_TYPE][MAX_EVENT_CLASS] = {
	{	
		{
			OID_ONE_IN_SM,
			OID_ONE_IN_SMQUERY,
			OID_ONE_IN_VALIDQUERY,
			OID_ONE_IN_ALERT,
			OID_ONE_IN_REPORT,
			OID_ONE_IN_INFORM,
			OID_ONE_IN_PORTED,
			OID_ONE_IN_EXQUERY, 0xFF, 0xFF, 0xFF, 0xFF
		},

		{
			OID_ONE_IN_TC_INVOKE,
			OID_ONE_IN_TC_RESULT,
			OID_ONE_IN_TC_CANCEL,
			OID_ONE_IN_TC_ERROR,
			OID_ONE_IN_TC_REJECT,
			OID_ONE_IN_TC_BEGIN,
			OID_ONE_IN_TC_CONTINUE,
			OID_ONE_IN_TC_END,
			OID_ONE_IN_TC_UNIDIR,
			OID_ONE_IN_TC_PABORT,
			OID_ONE_IN_TC_UABORT,
			OID_ONE_IN_TC_NOTICE
		}
	},

	{	{
			OID_ONE_OUT_SM,
			OID_ONE_OUT_SMQUERY,
			OID_ONE_OUT_VALIDQUERY,
			OID_ONE_OUT_ALERT,
			OID_ONE_OUT_REPORT,
			OID_ONE_OUT_INFORM,
			OID_ONE_OUT_PORTED,
			OID_ONE_OUT_EXQUERY, 0xFF, 0xFF, 0xFF, 0xFF
		},

		{
			OID_ONE_OUT_TC_INVOKE,
			OID_ONE_OUT_TC_RESULT,
			OID_ONE_OUT_TC_CANCEL,
			OID_ONE_OUT_TC_ERROR,
			OID_ONE_OUT_TC_REJECT,
			OID_ONE_OUT_TC_BEGIN,
			OID_ONE_OUT_TC_CONTINUE,
			OID_ONE_OUT_TC_END,
			OID_ONE_OUT_TC_UNIDIR,
			OID_ONE_OUT_TC_PABORT,
			OID_ONE_OUT_TC_UABORT,
			OID_ONE_OUT_TC_NOTICE
		}
	},

	{	{
			OID_ONE_OUT_ACK_SM,
			OID_ONE_OUT_ACK_SMQUERY,
			OID_ONE_OUT_ACK_VALIDQUERY,
			OID_ONE_OUT_ACK_ALERT,
			OID_ONE_OUT_ACK_REPORT,
			OID_ONE_OUT_ACK_INFORM,
			OID_ONE_OUT_ACK_PORTED,
			OID_ONE_OUT_ACK_EXQUERY, 0xFF, 0xFF, 0xFF, 0xFF
		},

		{
			OID_ONE_OUT_TC_INVOKE,
			OID_ONE_OUT_TC_RESULT,
			OID_ONE_OUT_TC_CANCEL,
			OID_ONE_OUT_TC_ERROR,
			OID_ONE_OUT_TC_REJECT,
			OID_ONE_OUT_TC_BEGIN,
			OID_ONE_OUT_TC_CONTINUE,
			OID_ONE_OUT_TC_END,
			OID_ONE_OUT_TC_UNIDIR,
			OID_ONE_OUT_TC_PABORT,
			OID_ONE_OUT_TC_UABORT,
			OID_ONE_OUT_TC_NOTICE
		}
	}
};

unsigned char	OID_VAR_TYPE[MAX_RECORD_CLASS][MAX_OID_VARTYPE_CLASS]	= {
	{
		OID_VAR_TYP_NONE,
		OID_VAR_TYP_SMEID,
		OID_VAR_TYP_TSVID,
		OID_VAR_TYP_GRPID,
		//OID_VAR_TYP_PTCID,
		OID_VAR_TYP_SME
	},
	{
		OID_VAR_TYP_NONE,
		OID_VAR_TYP_SMEID,
		OID_VAR_TYP_TSVID,
		OID_VAR_TYP_GRPID,
		//OID_VAR_TYP_PTCID,
		OID_VAR_TYP_SME
	}
};


#define	GET_OID_ZERO_STSRECORD(spStsRecord) \
	OID_ZERO_CONTAINER[(spStsRecord)->TryInterface][(spStsRecord)->TryRole]

#define	GET_OID_ONE_STSRECORD(spStsRecord) \
	OID_ONE_CONTAINER[(spStsRecord)->TryDirection]\
	[((spStsRecord)->TryInterface ==STATISTICS_TRYINF_NET)? \
		((spStsRecord)->TryRole == STATISTICS_TRYNRL_TCAP) ? 1 : 0 : 0]\
	[(spStsRecord)->TryEvent]

unsigned char	OID_TWO_DATA_LENGTH[DB_STATISTIC_VALUE_OID_TWO_SIZE] = {0xFE, 0xEE, 0xEE, 0xEE};
unsigned char	OID_TWO_NOVALUE[DB_STATISTIC_VALUE_OID_TWO_SIZE]	  = {0xFE, 0xFE, 0xFE, 0xFE};
unsigned char	OID_TWO_NOTUSED[DB_STATISTIC_VALUE_OID_TWO_SIZE]	  = {0xFF, 0xFF, 0xFF, 0xFF};
unsigned char    OID_TWO_TIME_VALUE[DB_STATISTIC_VALUE_OID_TWO_SIZE]   = {0xFA, 0xFA, 0xFA, 0xFA};

unsigned char	OID_VAR_NOTUSED[DB_STATISTIC_VALUE_OID_VARIABLE_SIZE]	=
{0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

/*
#define	GET_OID_VAR_STSRECORD(spStsRecord,nVarType) \
	((OID_VAR_TYPE[(spStsRecord)->RecordType][nVarType] == OID_VAR_TYP_NONE)? OID_VAR_NOTUSED : \
	(OID_VAR_TYPE[(spStsRecord)->RecordType][nVarType] == OID_VAR_TYP_SMEID)? (char *)&((spStsRecord)->ProcessID) : \
	(OID_VAR_TYPE[(spStsRecord)->RecordType][nVarType] == OID_VAR_TYP_TSVID)? (char *)&((spStsRecord)->ServiceID) : \
	(OID_VAR_TYPE[(spStsRecord)->RecordType][nVarType] == OID_VAR_TYP_GRPID)? (char *)&((spStsRecord)->GroupID) : \
	(OID_VAR_TYPE[(spStsRecord)->RecordType][nVarType] == OID_VAR_TYP_PTCID)? (char *)&((spStsRecord)->ServiceID) : \
	(OID_VAR_TYPE[(spStsRecord)->RecordType][nVarType] == OID_VAR_TYP_SME)? ((spStsRecord)->SME) : \
	  OID_VAR_NOTUSED)
*/
#define	GET_OID_VAR_STSRECORD(spStsRecord,nVarType) \
	((OID_VAR_TYPE[(spStsRecord)->RecordType][nVarType] == OID_VAR_TYP_NONE)? (char *)OID_VAR_NOTUSED : \
	(OID_VAR_TYPE[(spStsRecord)->RecordType][nVarType] == OID_VAR_TYP_SMEID)? (char *)&((spStsRecord)->ProcessID) : \
	(OID_VAR_TYPE[(spStsRecord)->RecordType][nVarType] == OID_VAR_TYP_GRPID)? (char *)&((spStsRecord)->GroupID) : \
	(OID_VAR_TYPE[(spStsRecord)->RecordType][nVarType] == OID_VAR_TYP_TSVID)? (char *)&((spStsRecord)->ServiceID) : \
	(OID_VAR_TYPE[(spStsRecord)->RecordType][nVarType] == OID_VAR_TYP_SME)? ((spStsRecord)->SME) : \
	  (char *)OID_VAR_NOTUSED)

/*
#define GET_OID_VAR_LENGTH(spStsRecord,nVarType) \
    ((OID_VAR_TYPE[(spStsRecord)->RecordType][nVarType] == OID_VAR_TYP_NONE)? 0 : \
    (OID_VAR_TYPE[(spStsRecord)->RecordType][nVarType] == OID_VAR_TYP_SMEID)? sizeof((spStsRecord)->ProcessID) : \
    (OID_VAR_TYPE[(spStsRecord)->RecordType][nVarType] == OID_VAR_TYP_TSVID)? \
        ((spStsRecord)->ServiceID)? sizeof((spStsRecord)->ProcessID) : 0 : \
    (OID_VAR_TYPE[(spStsRecord)->RecordType][nVarType] == OID_VAR_TYP_GRPID)? \
		((spStsRecord)->GroupID)? sizeof((spStsRecord)->GroupID) : 0 : \
    (OID_VAR_TYPE[(spStsRecord)->RecordType][nVarType] == OID_VAR_TYP_PTCID)? \
		((spStsRecord)->ServiceID)? sizeof((spStsRecord)->ServiceID) : 0 : \
	(OID_VAR_TYPE[(spStsRecord)->RecordType][nVarType] == OID_VAR_TYP_SME) ? \
		(isRegisteredEsme((spStsRecord)->SME)) ? 0 : getNumOfBCD((spStsRecord)->SME) : 0)
*/
#define GET_OID_VAR_LENGTH(spStsRecord,nVarType) \
    ((OID_VAR_TYPE[(spStsRecord)->RecordType][nVarType] == OID_VAR_TYP_NONE)? 0 : \
    (OID_VAR_TYPE[(spStsRecord)->RecordType][nVarType] == OID_VAR_TYP_SMEID)? sizeof((spStsRecord)->ProcessID) : \
    (OID_VAR_TYPE[(spStsRecord)->RecordType][nVarType] == OID_VAR_TYP_TSVID)? \
        ((spStsRecord)->ServiceID)? sizeof((spStsRecord)->ProcessID) : 0 : \
    (OID_VAR_TYPE[(spStsRecord)->RecordType][nVarType] == OID_VAR_TYP_GRPID)? \
		((spStsRecord)->GroupID)? sizeof((spStsRecord)->GroupID) : 0 : \
	(OID_VAR_TYPE[(spStsRecord)->RecordType][nVarType] == OID_VAR_TYP_SME) ? \
		(isRegisteredEsme((u_char *)((spStsRecord)->SME))) ? 0 : \
				 getNumOfBCD((u_char *)((spStsRecord)->SME), sizeof((spStsRecord)->SME)) : 0)


static int dbStatisticProcessRecord(stStatisticsRecord *spStsRecord, stStatisticTuple   *spStsTuple);
static int isRegisteredEsme(u_char  *cpEsme);

//SR124 BMT static func modify
static int dbStatisticMergeSend(stStatisticTuple *spStatisticsTuple, int nTableId);

#ifdef DEBUG
static char *hex2str(u_char *cpTarget, int nTargetSize, u_char *cpSource, int nSourceSize)
{
	u_char	cHigh, cLow;
	u_char	*cpTargetPtr, *cpSourcePtr;
	int         	nLoop;
	int         	nCnt;

	bzero(cpTarget, nTargetSize);
	cpTargetPtr = cpTarget;
	cpSourcePtr = cpSource;
	nCnt = (nTargetSize > (nSourceSize * 2)) ? nSourceSize : (++nTargetSize / 2);
	for (nLoop = 0; nLoop < nCnt; nLoop++) {
		cLow = cHigh = *cpSourcePtr++;
		cHigh >>= 4;
		if (((int)cHigh >= 0) && (cHigh <= 9)) {
			*cpTargetPtr++ = (cHigh + 0x30);    /* + '0' */
		} else {
			*cpTargetPtr++ = (cHigh + 0x37);
		}

		cLow &= 0x0F;
		if (((int)cLow >= 0) && (cLow <= 9)) {
			*cpTargetPtr++ = (cLow + 0x30);    /* + '0' */
		} else {
			*cpTargetPtr++ = (cLow + 0x37);
		}
	}
	return (char *)cpTarget;
}
#endif

int dbStatisticMergeSend(stStatisticTuple *spStatisticsTuple, int nTableId)
{
	stStatisticTuple	*spStatisticsDBTuple = NULL;
	stSemArray      	*spStatisticSemIds;
	//u_int		nCurValueDB = 0;
	//u_int		nCurValueTuple = 0;
	//u_int        nMaxTimeTuple = 0;
	//u_int        nMaxTimeDB = 0;
	int             	nResult = 0;

	if (gpAqiInfo[DBMS_STAT] == NULL) {
		spdlog::error("[{}] - Mdb Commands Out of Sync!!", __func__);
		return FLK_FAIL;
	}

	spStatisticSemIds = &(gpAqiInfo[DBMS_STAT]->semIdDir[nTableId]);

#ifdef DEBUG
	char prnBuff[(DB_STATISTIC_OID_SIZE * 2) + 1];
#endif

	//SR210 pkg
	//if ((nResult = dma_getTuplePtr(DBMS_STAT, nTableId, DB_SMSC_OID, -1,
	if ((nResult = dma_getTuplePtr(DBMS_STAT, nTableId, DB_SMSC_OID, 0,
	                               (char *)spStatisticsTuple->dbOid, DB_STATISTIC_OID_SIZE, (char **)&spStatisticsDBTuple)) < 0) {
		if (nResult == -DMAQI_THERE_IS_NOT_THE_TUPLE) {
			if ((nResult = db_dml(DBMS_STAT, nTableId, 0,
			                      (char *)spStatisticsTuple, sizeof(stStatisticTuple), 0, NULL,
			                      NO_RESULT, INSERT)) == FLK_SUCCESS) {
				return FLK_SUCCESS;
			}
#ifdef DEBUG
			spdlog::trace("[{}:{}]-result[{}], errno[{}]",
			       __func__, __LINE__, nResult, errno);
#endif // DEBUG			
		}
		return nResult;
	} else {
		//SR124 BMT
		if ( (nResult = db_dml_stat_update(DBMS_STAT, nTableId, 0, (char *)spStatisticsTuple, sizeof(stStatisticTuple), 0, NULL, NO_RESULT, UPDATE,
		                                   (spStatisticsTuple->dbOid[OID_ZERO_LEVEL] % spStatisticSemIds->nSemNum))) == FLK_SUCCESS) {
			return FLK_SUCCESS;
		}

		return nResult;
	}


#if 0

	memcpy(&nCurValueTuple, spStatisticsTuple->dbCurValue,
	       DB_STATISTIC_CUR_VALUE_SIZE);
	memcpy(&nMaxTimeTuple, spStatisticsTuple->dbDdbTime,
	       DB_STATISTIC_DDB_TIME_SIZE);
	M_SemaP(spStatisticSemIds->nSemId,
	        spStatisticsTuple->dbOid[OID_ZERO_LEVEL] % spStatisticSemIds->nSemNum);
	memcpy(&nCurValueDB, spStatisticsDBTuple->dbCurValue,
	       DB_STATISTIC_CUR_VALUE_SIZE);
	nCurValueDB += nCurValueTuple;
	memcpy(spStatisticsDBTuple->dbCurValue, &nCurValueDB,
	       DB_STATISTIC_CUR_VALUE_SIZE);
	memcpy(&nMaxTimeDB, spStatisticsDBTuple->dbDdbTime,
	       DB_STATISTIC_DDB_TIME_SIZE);
	if (nMaxTimeTuple > nMaxTimeDB) {
		memcpy(spStatisticsDBTuple->dbDdbTime, &nMaxTimeTuple,
		       DB_STATISTIC_DDB_TIME_SIZE);
	}
	M_SemaV(spStatisticSemIds->nSemId,
	        spStatisticsTuple->dbOid[OID_ZERO_LEVEL] % spStatisticSemIds->nSemNum);
	return FLK_SUCCESS;
#endif
}

int dbStatisticMerge(stStatisticTuple *spStatisticsTuple, int nTableId)
{
	stStatisticTuple	*spStatisticsDBTuple = NULL;
	stSemArray      	*spStatisticSemIds;
	u_int		nCurValueDB = 0;
	u_int		nCurValueTuple = 0;
	u_int        nMaxTimeTuple = 0;
	u_int        nMaxTimeDB = 0;
	int             	nResult = 0;

	if (gpAqiInfo[DBMS_STAT] == NULL) {
		spdlog::error("[{}] - Mdb Commands Out of Sync!!", __func__);
		return FLK_FAIL;
	}

	spStatisticSemIds = &(gpAqiInfo[DBMS_STAT]->semIdDir[nTableId]);

#ifdef DEBUG
	char prnBuff[(DB_STATISTIC_OID_SIZE * 2) + 1];
#endif

	//SR210 pkg
	//if ((nResult = dma_getTuplePtr(DBMS_STAT, nTableId, DB_SMSC_OID, -1,
	if ((nResult = dma_getTuplePtr(DBMS_STAT, nTableId, DB_SMSC_OID, 0,
	                               (char *)spStatisticsTuple->dbOid, DB_STATISTIC_OID_SIZE, (char **)&spStatisticsDBTuple)) < 0) {
		if (nResult == -DMAQI_THERE_IS_NOT_THE_TUPLE) {
			if ((nResult = db_dml(DBMS_STAT, nTableId, 0,
			                      (char *)spStatisticsTuple, sizeof(stStatisticTuple), 0, NULL,
			                      NO_RESULT, INSERT)) == FLK_SUCCESS) {
				return FLK_SUCCESS;
			}
#ifdef DEBUG
			spdlog::error("[{}:{}]-result[{}], errno[{}]",
			       __func__, __LINE__, nResult, errno);
#endif // DEBUG			
		}
		return nResult;
	}


	memcpy(&nCurValueTuple, spStatisticsTuple->dbCurValue,
	       DB_STATISTIC_CUR_VALUE_SIZE);
	memcpy(&nMaxTimeTuple, spStatisticsTuple->dbDdbTime,
	       DB_STATISTIC_DDB_TIME_SIZE);
	M_SemaP(spStatisticSemIds->nSemId, spStatisticsTuple->dbOid[OID_ZERO_LEVEL] % spStatisticSemIds->nSemNum);
	memcpy(&nCurValueDB, spStatisticsDBTuple->dbCurValue,
	       DB_STATISTIC_CUR_VALUE_SIZE);
	nCurValueDB += nCurValueTuple;
	memcpy(spStatisticsDBTuple->dbCurValue, &nCurValueDB,
	       DB_STATISTIC_CUR_VALUE_SIZE);
	memcpy(&nMaxTimeDB, spStatisticsDBTuple->dbDdbTime,
	       DB_STATISTIC_DDB_TIME_SIZE);
	if (nMaxTimeTuple > nMaxTimeDB) {
		memcpy(spStatisticsDBTuple->dbDdbTime, &nMaxTimeTuple,
		       DB_STATISTIC_DDB_TIME_SIZE);
	}
	M_SemaV(spStatisticSemIds->nSemId, spStatisticsTuple->dbOid[OID_ZERO_LEVEL] % spStatisticSemIds->nSemNum);
	return FLK_SUCCESS;
}

int dbStatisticUpdates(stStatisticTuple *spStatisticsTuple, stStatisticTuple *spStatisticsDBTuple, int nTableId)
{
	//stStatisticTuple	*spStatisticsDBTuple = NULL;
	stSemArray      	*spStatisticSemIds;
	u_int		nCurValueDB = 0;
	u_int		nCurValueTuple = 0;
	u_int        nMaxTimeTuple = 0;
	u_int        nMaxTimeDB = 0;
	//int             	nResult = 0;

	if (gpAqiInfo[DBMS_STAT] == NULL) {
		spdlog::error("[{}] - Mdb Commands Out of Sync!!", __func__);
		return FLK_FAIL;
	}

	spStatisticSemIds = &(gpAqiInfo[DBMS_STAT]->semIdDir[nTableId]);

#ifdef DEBUG
	char prnBuff[(DB_STATISTIC_OID_SIZE * 2) + 1];
#endif


#if 0
	if ((nResult = dma_getTuplePtr(DBMS_STAT, nTableId, DB_SMSC_OID, -1,
	                               (char *)spStatisticsTuple->dbOid, DB_STATISTIC_OID_SIZE, (char **)&spStatisticsDBTuple)) < 0) {
		if (nResult == -DMAQI_THERE_IS_NOT_THE_TUPLE) {
			if ((nResult = db_dml(DBMS_STAT, nTableId, 0,
			                      (char *)spStatisticsTuple, sizeof(stStatisticTuple), 0, NULL,
			                      NO_RESULT, INSERT)) == FLK_SUCCESS) {
				return FLK_SUCCESS;
			}
#ifdef DEBUG
			spdlog::error("[{}:{}]-result[{}], errno[{}]",
			       __func__, __LINE__, nResult, errno);
#endif // DEBUG			
		}
		return nResult;
	}
#endif

	memcpy(&nCurValueTuple, spStatisticsTuple->dbCurValue,
	       DB_STATISTIC_CUR_VALUE_SIZE);
	memcpy(&nMaxTimeTuple, spStatisticsTuple->dbDdbTime,
	       DB_STATISTIC_DDB_TIME_SIZE);
	M_SemaP(spStatisticSemIds->nSemId,
	        spStatisticsTuple->dbOid[OID_ZERO_LEVEL] % spStatisticSemIds->nSemNum);
	memcpy(&nCurValueDB, spStatisticsDBTuple->dbCurValue,
	       DB_STATISTIC_CUR_VALUE_SIZE);
	nCurValueDB += nCurValueTuple;
	memcpy(spStatisticsDBTuple->dbCurValue, &nCurValueDB,
	       DB_STATISTIC_CUR_VALUE_SIZE);
	memcpy(&nMaxTimeDB, spStatisticsDBTuple->dbDdbTime,
	       DB_STATISTIC_DDB_TIME_SIZE);
	if (nMaxTimeTuple > nMaxTimeDB) {
		memcpy(spStatisticsDBTuple->dbDdbTime, &nMaxTimeTuple,
		       DB_STATISTIC_DDB_TIME_SIZE);
	}
	M_SemaV(spStatisticSemIds->nSemId,
	        spStatisticsTuple->dbOid[OID_ZERO_LEVEL] % spStatisticSemIds->nSemNum);
	return FLK_SUCCESS;
}



int dbStatisticProcessStartRecord(stStatisticsRecord *spStsRecord)
{
	stStatisticTuple stStatisticsTuple;
	int             nResult = 1;
	int             nLoop;

	bzero(&stStatisticsTuple, sizeof(stStatisticsTuple));
	memcpy(&stStatisticsTuple.dbCurValue, &nResult, DB_STATISTIC_CUR_VALUE_SIZE);

	OID_SET_VALUE(stStatisticsTuple.dbOid, DB_STATISTIC_OID_ZERO_POINT,
	              GET_OID_ZERO_STSRECORD(spStsRecord));
#ifdef TEST
	printf("TryInterface[%d]TryRole[%d] OID[%02x]\n",
	       (spStsRecord)->TryInterface, (spStsRecord)->TryRole,
	       GET_OID_ZERO_STSRECORD(spStsRecord));
#endif
	OID_SET_VALUE(stStatisticsTuple.dbOid, DB_STATISTIC_OID_ONE_POINT,
	              GET_OID_ONE_STSRECORD(spStsRecord));

#ifdef TEST
	printf("TryDirection[%d]TryInterface[%d]TryEvent[%d] OID[%02x]\n",
	       spStsRecord->TryDirection,
	       ((spStsRecord->TryInterface == STATISTICS_TRYINF_NET) ?
	        ((spStsRecord->TryRole == STATISTICS_TRYNRL_TCAP) ? 1 : 0) : 0),
	       spStsRecord->TryEvent, GET_OID_ONE_STSRECORD(spStsRecord));
#endif

	OID_SET_TWO_VALUE(stStatisticsTuple.dbOid,
	                  OID_TWO_NOTUSED, DB_STATISTIC_VALUE_OID_TWO_SIZE);
	OID_SET_VAR_VALUE(stStatisticsTuple.dbOid,
	                  OID_VAR_NOTUSED, DB_STATISTIC_VALUE_OID_VARIABLE_SIZE);

	if (!spStsRecord->TryResult) {
		dbStatisticMergeSend(&stStatisticsTuple, R_DSTATISTICS);
	}

	OID_SET_TWO_VALUE(stStatisticsTuple.dbOid,
	                  OID_TWO_NOVALUE, DB_STATISTIC_VALUE_OID_TWO_SIZE);

	/* baram : 20050326
	dbStatisticProcessRecord(spStsRecord,&stStatisticsTuple);
	*/
	for (nLoop = 0; nLoop < MAX_OID_VARTYPE_CLASS; nLoop++) {
		if (!(stStatisticsTuple.dbVariableSize[0] = GET_OID_VAR_LENGTH(spStsRecord, nLoop))) {
			continue;
		}

		stStatisticsTuple.dbVariableType[0] = OID_VAR_TYPE[spStsRecord->RecordType][nLoop];
		OID_SET_VAR_VALUE(stStatisticsTuple.dbOid, GET_OID_VAR_STSRECORD(spStsRecord, nLoop), stStatisticsTuple.dbVariableSize[0]);

		dbStatisticMergeSend(&stStatisticsTuple, R_DSTATISTICS);
	}


	return FLK_SUCCESS;
}

int dbStatisticProcessEndRecord(stStatisticsRecord *spStsRecord)
{
	stStatisticTuple stStatisticsTuple;
	int             nResult = 1;
//    int				nLoop, nLoop1;

	bzero(&stStatisticsTuple, sizeof(stStatisticsTuple));
	memcpy(&stStatisticsTuple.dbCurValue, &nResult, DB_STATISTIC_CUR_VALUE_SIZE);

	OID_SET_VALUE(stStatisticsTuple.dbOid,
	              DB_STATISTIC_OID_ZERO_POINT,
	              GET_OID_ZERO_STSRECORD(spStsRecord));
	OID_SET_VALUE(stStatisticsTuple.dbOid,
	              DB_STATISTIC_OID_ONE_POINT,
	              GET_OID_ONE_STSRECORD(spStsRecord) |
	              (((spStsRecord->TryInterface == STATISTICS_TRYINF_NET) &&
	                (spStsRecord->TryRole == STATISTICS_TRYNRL_TCAP)) ? 0 :
	               (spStsRecord->TryResult) ? OID_ONE_TYP_FAIL : OID_ONE_TYP_SUC));

#ifdef TEST
	if (spStsRecord->TryEvent == STATISTICS_TRYDIR_OUT_ACK) {
	}
#endif //TEST

#ifdef TEST
	printf("dbStatisticProcessEndRecord-\
		TryDirection[%d]TryInterface[%d]TryEvent[%d] OID[%02x] \
			OID2[%02x]OID3[%02x]\n",
	       spStsRecord->TryDirection,
	       ((spStsRecord->TryInterface == STATISTICS_TRYINF_NET) ?
	        ((spStsRecord->TryRole == STATISTICS_TRYNRL_TCAP) ? 1 : 0) : 0),
	       spStsRecord->TryEvent,
	       GET_OID_ONE_STSRECORD(spStsRecord),
	       ((spStsRecord->TryInterface == STATISTICS_TRYINF_NET) &&
	        (spStsRecord->TryRole == STATISTICS_TRYNRL_TCAP)) ? 0 :
	       (spStsRecord->TryResult) ? OID_ONE_TYP_FAIL : OID_ONE_TYP_SUC,
	       GET_OID_ONE_STSRECORD(spStsRecord) |
	       (((spStsRecord->TryInterface == STATISTICS_TRYINF_NET) &&
	         (spStsRecord->TryRole == STATISTICS_TRYNRL_TCAP)) ? 0 :
	        (spStsRecord->TryResult) ? OID_ONE_TYP_FAIL : OID_ONE_TYP_SUC));
#endif
	OID_SET_TWO_VALUE(stStatisticsTuple.dbOid,
	                  OID_TWO_NOTUSED, DB_STATISTIC_VALUE_OID_TWO_SIZE);
	OID_SET_VAR_VALUE(stStatisticsTuple.dbOid,
	                  OID_VAR_NOTUSED, DB_STATISTIC_VALUE_OID_VARIABLE_SIZE);

	dbStatisticMergeSend(&stStatisticsTuple, R_DSTATISTICS);

	dbStatisticProcessRecord(spStsRecord, &stStatisticsTuple);

	return FLK_SUCCESS;
}
static int dbStatisticProcessRecord(stStatisticsRecord *spStsRecord, stStatisticTuple *spStsTuple)
{
	int             nResult = 1;
	int				nLoop, nLoop1;

	for (nLoop1 = 0; nLoop1 < 4; nLoop1++) {
		switch (nLoop1) {
		case OID_TWO_TYP_NONE :
			OID_SET_TWO_VALUE(spStsTuple->dbOid,
			                  OID_TWO_NOVALUE, DB_STATISTIC_VALUE_OID_TWO_SIZE);
			break;

		case OID_TWO_TYP_ERRC :
			if (!spStsRecord->TryResult) {
				continue;
			}

			spStsTuple->dbOidTwoSize[0] = DB_STATISTIC_VALUE_OID_TWO_SIZE;
			spStsTuple->dbOidTwoType[0] = OID_TWO_TYP_ERRC;
			OID_SET_TWO_VALUE(spStsTuple->dbOid, spStsRecord->TryResult,
			                  DB_STATISTIC_VALUE_OID_TWO_SIZE);
			memcpy(spStsTuple->dbCurValue, &nResult,
			       DB_STATISTIC_CUR_VALUE_SIZE);
			break;

		case OID_TWO_TYP_TIME :
			if (!spStsRecord->TryTime) {
				continue;
			}

			if ((spStsRecord->TryDirection) == STATISTICS_TRYDIR_IN) {
				OID_SET_VALUE(spStsTuple->dbOid, DB_STATISTIC_OID_ONE_POINT,
				              OID_ONE_IN_TIME_INTERVAL);
			} else {
				OID_SET_VALUE(spStsTuple->dbOid, DB_STATISTIC_OID_ONE_POINT,
				              OID_ONE_OUT_TIME_INTERVAL);
			}

			spStsTuple->dbOidTwoSize[0] = DB_STATISTIC_VALUE_OID_TWO_SIZE;
			spStsTuple->dbOidTwoType[0] = OID_TWO_TYP_TIME;
			OID_SET_TWO_VALUE(spStsTuple->dbOid, OID_TWO_TIME_VALUE,
			                  DB_STATISTIC_VALUE_OID_TWO_SIZE);
			memcpy(spStsTuple->dbCurValue, &spStsRecord->TryTime,
			       DB_STATISTIC_CUR_VALUE_SIZE);
			memcpy(spStsTuple->dbDdbTime, &spStsRecord->TryTime,
			       DB_STATISTIC_DDB_TIME_SIZE);
			break;

		case OID_TWO_TYP_DLEN :
			if (!spStsRecord->TryDataLen) {
				continue;
			}

			OID_SET_VALUE(spStsTuple->dbOid,
			              DB_STATISTIC_OID_ONE_POINT, OID_ONE_IN_DATA_LENGTH);
			spStsTuple->dbOidTwoSize[0] = DB_STATISTIC_VALUE_OID_TWO_SIZE;
			spStsTuple->dbOidTwoType[0] = OID_TWO_TYP_DLEN;
			OID_SET_TWO_VALUE(spStsTuple->dbOid, OID_TWO_DATA_LENGTH,
			                  DB_STATISTIC_VALUE_OID_TWO_SIZE);
			nResult = spStsRecord->TryDataLen;
			memcpy(&spStsTuple->dbCurValue, &nResult,
			       DB_STATISTIC_CUR_VALUE_SIZE);
			break;

		default :
			nLoop = 4;
			continue;
		}

		for (nLoop = 0; nLoop < MAX_OID_VARTYPE_CLASS; nLoop++) {
#ifdef TEST
			printf("[%d]GET_OID_VAR_LENGTH(%d) \n", nLoop,
			       GET_OID_VAR_LENGTH(spStsRecord, nLoop));
#endif
			if (!(spStsTuple->dbVariableSize[0] = GET_OID_VAR_LENGTH(spStsRecord, nLoop))) {
				continue;
			}

			spStsTuple->dbVariableType[0] = OID_VAR_TYPE[spStsRecord->RecordType][nLoop];

			OID_SET_VAR_VALUE(spStsTuple->dbOid, GET_OID_VAR_STSRECORD(spStsRecord, nLoop), spStsTuple->dbVariableSize[0]);

			dbStatisticMergeSend(spStsTuple, R_DSTATISTICS);
		}
	}
	return FLK_SUCCESS;
}


int dbStatisticInsertbyRecord(stStatisticsRecord *spStsRecord)
{

	if (spStsRecord->RecordType == STATISTICS_RECORD_START) {
		return dbStatisticProcessStartRecord(spStsRecord);
	} else if (spStsRecord->RecordType == STATISTICS_RECORD_END) {
		return dbStatisticProcessEndRecord(spStsRecord);
	} else if (spStsRecord->RecordType == STATISTICS_RECORD_SINGLE) {
		spStsRecord->RecordType = STATISTICS_RECORD_START;
		if (dbStatisticProcessStartRecord(spStsRecord) == FLK_SUCCESS) {
			spStsRecord->RecordType = STATISTICS_RECORD_END;
			return dbStatisticProcessEndRecord(spStsRecord);
		}
	}
	return FLK_FAIL;
}

static int isRegisteredEsme(u_char  *cpEsme)
{
	char   attVal[DB_ESME_SIZE];
	int    tableId, nTupId, nAttId;
	char   *cpEsmeNoAlign;
	int		nDbmsHandle = DBMS_PLUS;

	if (gpAqiInfo[nDbmsHandle] == NULL) {
		if (connectDbDmaqi(nDbmsHandle) != FLK_SUCCESS) {
			spdlog::error("[{}:{}] connectDbDmaqi({}) Fail",
			       __func__, __LINE__, nDbmsHandle);
		}
		return true;
	}

	memset(attVal, '\0', sizeof(attVal));
	memcpy(attVal, cpEsme, DB_ESME_SIZE);

	tableId = R_DESME;
	nAttId  = DB_ESME;

	if (((nTupId = dma_getTupId(nDbmsHandle, tableId, nAttId, attVal, 0)) >= 0) &&
	        (dma_table_retr(nDbmsHandle, tableId, nTupId, &cpEsmeNoAlign) >= 0)) {
		return false;
	} else {
		return true;
	}
}

void SetDeliverTime(stRDActiveTuple *spActive)
{
	time_t tCurTime;
	u_int nTimeTmp = 0x00;

	time( &tCurTime );
	nTimeTmp = (u_int)tCurTime;
	memcpy (spActive->dbStatSendTime, (void *)&nTimeTmp, sizeof(u_int) );

#ifdef DEBUG
	if ((tv.tv_sec == 0) || (tv.tv_sec > 10000)) {
		spdlog::info("[{}]tv.tv_sec[{}], tv.tv_usec[{}]",
		       __func__, tv.tv_sec, tv.tv_usec);
		spdlog::info("dbStatSendTime = [{:>02x}{:>02x}{:>02x}{:>02x}{:>02x}{:>02x}{:>02x}{:>02x}]",
		       spActive->dbStatSendTime[0], spActive->dbStatSendTime[1],
		       spActive->dbStatSendTime[2], spActive->dbStatSendTime[3],
		       spActive->dbStatSendTime[4], spActive->dbStatSendTime[5],
		       spActive->dbStatSendTime[6], spActive->dbStatSendTime[7]);
	}
#endif //DEBUG
	return;
}

u_int DeliverTimeInterval(stRDActiveTuple *spActive)
{
	time_t		tCur_time;
	u_int		unIvk_time;
	u_int		nInterval_time;

	memcpy(&unIvk_time, spActive->dbStatSendTime, sizeof(u_int));
	time(&tCur_time);

	nInterval_time = (u_int)(tCur_time - unIvk_time);
	return (nInterval_time);
}

u_int IncomeTimeInterval(time_t tIvk_time)
{
	u_int    nIntvalTime;
	time_t	tCur_time;
	time(&tCur_time);
	nIntvalTime = (u_int)(tCur_time - tIvk_time);
	return (nIntvalTime);
}
/** @} */
