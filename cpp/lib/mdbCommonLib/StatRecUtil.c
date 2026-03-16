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
#include 	"MdbDmaqi.h"
#include	"ECommon.h"
#include    "Debug.h"
#include 	"StatRecord.h"
#include 	"StatTable.h"
#include 	"SmsStat.h"
//#include 	"SmsStatistics.h"
#include 	"SmsStat.h"

//---------------------------------------------------------------------
//
//  FUNCTION NAME   : MakeStatisticsRecord
//  PURPOSE         : make Statistics Record with arguments
//  INPUT ARGUMENTS :
//  RETURN VALUE    : FAIL/SUCEESS
//  GLOBAL VARIABLES
//    1) EXTERNAL   : none
//    2) LOCAL      :
//
//----------------------------------------------------------------------

int MakeStatisticsRecord( stStatisticsRecord 	*spStsRecord,
    u_short  RecordType,
    u_short  TryInterface,
    u_short  TryRole,
    u_short  TryDirection,
    u_short  TryEvent,
    u_short  TryDataLen,
    u_int    TryTime,
    u_int    TryResult,
    u_int  	MsgID,
    u_short  ProcessID,
    u_short  ServiceID,
    u_short	GroupID,
    char			*SME)
{
	bzero(spStsRecord, sizeof(stStatisticsRecord));
	SET_STSRECORD_RECORDTYPE(spStsRecord, RecordType);
	SET_STSRECORD_TRYINTERFACE(spStsRecord, TryInterface);
	SET_STSRECORD_TRYROLE(spStsRecord, TryRole);
	SET_STSRECORD_TRYDIRECTION(spStsRecord, TryDirection);
	SET_STSRECORD_TRYEVENT(spStsRecord, TryEvent);
	SET_STSRECORD_TRYDATALEN(spStsRecord, TryDataLen);
	SET_STSRECORD_TRYTIME(spStsRecord, TryTime);
	SET_STSRECORD_TRYRESULT(spStsRecord, TryResult);
	SET_STSRECORD_MSGID(spStsRecord, MsgID);
	SET_STSRECORD_PROCESSID(spStsRecord, ProcessID);
	SET_STSRECORD_SERVICEID(spStsRecord, ServiceID);
	SET_STSRECORD_GROUPID(spStsRecord, GroupID);
	if ( SME ) {
		SET_STSRECORD_SME(spStsRecord, SME);
	} else {
		memset(spStsRecord->SME, 0x00, sizeof(spStsRecord->SME));
	}

	return FLK_SUCCESS;
}

int DebugStatisticsRecord(stStatisticsRecord 	*spStsRecord)
{
#ifdef DEBUG
	for (int nLoop = 0; nLoop < STATISTICS_RECORD_SME_SIZE; nLoop++) {
	}
#endif
	return FLK_SUCCESS;
}

int LogStatisticsRecord(stStatisticsRecord 	*spStsRecord)
{
#ifdef DEBUG
	spdlog::info("========================");
	spdlog::info("RecordType	 [{}]", spStsRecord->RecordType);
	spdlog::info("TryInterface [{}]", spStsRecord->TryInterface);
	spdlog::info("TryRole		 [{}]", spStsRecord->TryRole);
	spdlog::info("TryDirection [{}]", spStsRecord->TryDirection);
	spdlog::info("TryEvent	 [{}]", spStsRecord->TryEvent);
	spdlog::info("TryDataLen	 [{}]", spStsRecord->TryDataLen);
	spdlog::info("TryTime		 [{}]", spStsRecord->TryTime);
	spdlog::info("TryResult	 [{}]", spStsRecord->TryResult);
	spdlog::info("MsgID		 [{}]", spStsRecord->MsgID);
	spdlog::info("ProcessID	 [{}]", spStsRecord->ProcessID);
	spdlog::info("ServiceID	 [{}]", spStsRecord->ServiceID);
	spdlog::info("GroupID		 [{}]", spStsRecord->GroupID);
	spdlog::info("========================");
#endif
	return FLK_SUCCESS;
}

int PrintStatisticsRecord(stStatisticsRecord 	*spStsRecord)
{
	printf("========================\n");
	printf("RecordType	 [%d]\n", spStsRecord->RecordType);
	printf("TryInterface [%d]\n", spStsRecord->TryInterface);
	printf("TryRole		 [%d]\n", spStsRecord->TryRole);
	printf("TryDirection [%d]\n", spStsRecord->TryDirection);
	printf("TryEvent	 [%d]\n", spStsRecord->TryEvent);
	printf("TryDataLen	 [%d]\n", spStsRecord->TryDataLen);
	printf("TryTime		 [%d]\n", spStsRecord->TryTime);
	printf("TryResult	 [%d]\n", spStsRecord->TryResult);
	printf("MsgID		 [%d]\n", spStsRecord->MsgID);
	printf("ProcessID	 [%d]\n", spStsRecord->ProcessID);
	printf("ServiceID	 [%d]\n", spStsRecord->ServiceID);
	printf("GroupID		 [%d]\n", spStsRecord->GroupID);

	int nLoop;
	for (nLoop = 0; nLoop < STATISTICS_RECORD_SME_SIZE; nLoop++) {
		printf("[%02x]", spStsRecord->SME[nLoop]);
	}
	printf("\n");
	printf("========================\n");
	return FLK_SUCCESS;
}


void SET_STSRECORD_STRGROUPID(char *cpTarget, char *cpSource)
{
	char	cHigh, cLow;
	char	*cpTargetPtr, *cpSourcePtr;
	int     nLoop;
	int     nCnt;

	memset(cpTarget, 0xFF, STATISTICS_RECORD_GRPID_SIZE);
	cpTargetPtr = cpTarget;
	cpSourcePtr = cpSource;
	nCnt = (STATISTICS_RECORD_GRPID_SIZE * 2 > (int)strlen(cpSource)) ?
	       ((int)strlen(cpSource) + 1) / 2 : STATISTICS_RECORD_GRPID_SIZE;
	for (nLoop = 0; nLoop < nCnt; nLoop++, cpSourcePtr++) {
		cHigh = cLow = 0x0F;
		if ((*cpSourcePtr >= '0') && (*cpSourcePtr <= '9')) {
			cHigh = *cpSourcePtr - 0x30;    /* - '0' */
		} else {
			cHigh = *cpSourcePtr - 0x37;
		}

		if ((*(++cpSourcePtr) >= '0') && (*cpSourcePtr <= '9')) {
			cLow = *cpSourcePtr - 0x30;    /* - '0' */
		} else if (*cpSourcePtr == '\0') {
			nLoop = nCnt;    /* break; */
		} else {
			cLow = *cpSourcePtr - 0x37;
		}

		*cpTargetPtr++ = (u_char) (cHigh << 4) + cLow;
	}
}

void setTwoVarValue(stStatisticTuple *spTuple, u_char *ucGroup, int nGroupCnt, int nType, int nTypeValue, int nCurValue)
{
	if (nType) {
		spTuple->dbOidTwoSize[0] = DB_STATISTIC_VALUE_OID_TWO_SIZE;
		spTuple->dbOidTwoType[0] = nType;
		OID_SET_TWO_VALUE(spTuple->dbOid, nTypeValue, DB_STATISTIC_VALUE_OID_TWO_SIZE);
	}

	spTuple->dbVariableType[0] = OID_VAR_TYP_GRPID;
	spTuple->dbVariableSize[0] = nGroupCnt;

	memcpy(&(spTuple->dbOid[6]), ucGroup, nGroupCnt);

	memcpy(spTuple->dbCurValue, &nCurValue, DB_STATISTIC_CUR_VALUE_SIZE);
}

/** @} */
