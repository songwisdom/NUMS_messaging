/**
 @ingroup mapDmaqi
 @{
 */
/*******************************************************************************
*
*     Copyright (c) 2001 FEELINGK Co., Ltd.
*     All rights reserved.
*
*     This program contains confidential and proprietary information of
*     FEELINGK, and any reproduction, disclosure, or use in whole or in part
*     by prior written agreement or permission of FEELINGK.
*
*                            FEELINGK Co., Ltd.
*         5F Roy B/D 90-6 Nonhyun-dong Kangnam-Gu, Seoul, 135-010 Korea
*
#******************************************************************************/

/***********************************************************************
	FILE NAME		:	mdbDmaqi.c
	PURPOSE			:	to provide map process with db library function
	DATE OF WRITE	: 	1998.05.05
	AUTHOR			: 	Lee Mi Oak
	REVISION		:	V 1.0
	Ver.	By		When		Reason
	------- ------- ----------  -----------------------------------------
	1.0		molee	1998.05.05	Update
***********************************************************************/
#include <arpa/inet.h>

#include <libMapDmaqi.h>
#include <MdbCommon.h>

#include <MdbDataStruct.h>
#include <EMacro.h>
#include <Map_Struct.h>
#include <MdbTableInfo.h>
#include <MdbMessage.h>
#include <MdbDmaqi.h>
#include <Debug.h>

#define  MAX_DMAQI_SEARCH_COUNT		4

static int db_dma_update(int nDbmsHandle, Update *update, Select_Ack *ack, int mode)
{
	int		nResult = 0;
	Update	update1;
	Select_Ack ack1;


	memcpy(&update1, update, sizeof(update1));

	nResult = mdb_dma_update(nDbmsHandle, update, ack, mode);
	if (nResult < 0) {
		if (nResult == -DMAQI_THERE_IS_NOT_THE_TUPLE) {
			memset(&ack1, 0x00, sizeof(ack));
			nResult = mdb_dma_update(nDbmsHandle, &update1, &ack1, mode);
			if (nResult <  0) {
				return nResult;
			} else {
				return FLK_SUCCESS;
			}
		} else {
			return FLK_FAIL;
		}
	}
	return FLK_SUCCESS;

}

/*
static int db_dma_select(nDbmsHandle, select,ack,mode)
int			nDbmsHandle;
Select      *select;
Select_Ack  *ack;
int     mode;
{
	int			nResult=FLK_FAIL;
	int			nSearchCnt=0;

	Select		select1;
	Select_Ack	ack1;

	memset(&select1,0x00,sizeof(Select));
	memcpy(&select1,select,sizeof(Select));

	for(nSearchCnt=0; nSearchCnt < MAX_DMAQI_SEARCH_COUNT ; nSearchCnt++)
	{
		nResult=mdb_dma_select(nDbmsHandle, select,ack,mode);
		if(nResult < 0)
		{
			memcpy(select,&select1,sizeof(Select));
			memset(ack,0x00,sizeof(Select_Ack));
			continue;
		}
		else
			break;
	}
	return nResult;
}
*/

int updateFailReason(int nDbmsHandle, u_int unMesgId, u_short usFailReason)
{

	Update              update;
	Select_Ack          ack;

//    u_char       ucMesgId[4];
	int                 nResult = 0;
	stRDActiveTuple     active;
//    stRDActiveTuple     *spActive;


	memset(&update, 0x00, sizeof(Update));
	memset(&ack, 0x00, sizeof(Update_Ack));
	memset(&active, 0x00, sizeof(stRDActiveTuple));

	update.table_id     =   R_DACTIVE;
	update.att_id       =   DB_MESSAGE_ID;
	update.tup_id       =   0;
	update.update_cnt   =   1;
	memcpy(update.att_val, &unMesgId, sizeof(active.dbMessageID));
	update.update_list[0]   =   DB_FAIL_REASON;

	memcpy(active.dbFailReason, &usFailReason, sizeof(u_short));
	memcpy(update.tuple, &active, sizeof(stRDActiveTuple));
	nResult =   db_dma_update(nDbmsHandle, &update, &ack, UPDATE_ATTR);

	if (nResult < 0) {
		db_dma_error(nResult);
		return  nResult;
	}
#ifdef DEBUG
	if (GetDebugLevel() >= DG_LEVEL_THREE) {
		spActive        =       (stRDActiveTuple *)&ack.tuple[0];
		debugActiveRec(spActive);
	}
#endif
	return FLK_SUCCESS;
}


static int selectSubNPDBPrefix(u_char *ucMin, stRDNpdbPrefixTuple *spPrefix)
{
	stRDNpdbPrefixTuple prefix;

	u_char   ucMinPrefix[DB_NPDB_PREFIX_SIZE];
//	u_char   ucTmp				= 0x00;
//	u_char   ucPrefixAddrType 	= 0x00;
	u_char   attVal[DB_NPDB_PREFIX_SIZE];
	char            *noAlign			= NULL;
	int             tableId = 0, nTupId = 0, nAttId = 0; //, nTmp=0;

	bzero(&prefix, sizeof(stRDNpdbPrefixTuple));
	bzero(ucMinPrefix, DB_NPDB_PREFIX_SIZE);

	tableId = R_DNP_DB_PREFIX;
	nTupId  = 0;
	nAttId  = DB_NPDB_PREFIX;

	memcpy(attVal, ucMin, DB_NPDB_PREFIX_SIZE);
	if ((nTupId = dma_getTupId(DBMS_STAT,
	                           tableId, nAttId, (char *)ucMin, nTupId )) < 0) {
		return -DMAQI_THERE_IS_NOT_THE_TUPLE;
	}
	if (dma_table_retr(DBMS_STAT, tableId, nTupId, &noAlign) < 0) {
		return -DMAQI_THERE_IS_NOT_THE_TUPLE;
	}

	(void)dma_align_make(DBMS_STAT, tableId, (char *)&prefix, noAlign);
	memcpy(spPrefix, &prefix, sizeof(stRDNpdbPrefixTuple));
	return FLK_SUCCESS;
}

int selectNPDBPrefix(const u_char *ucMin, int nMinOctetNum, stRDNpdbPrefixTuple *spPrefix)
{

	int             nResult = 0;
	int             nIdx = 0;
	int     		nLoop = 0, nSrc = 0; //, nTmp=0;
	u_char   ucMinPrefix[DB_NPDB_PREFIX_SIZE];
	u_char   ucTmp = 0;
	stRDNpdbPrefixTuple prefixAddr;

	nIdx = (DB_NPDB_PREFIX_SIZE < nMinOctetNum) ? DB_NPDB_PREFIX_SIZE : nMinOctetNum;

	// for even length error. temporarl coding. must be recoded.
	if (DB_NPDB_PREFIX_SIZE > nIdx) {
		nIdx++;
	}

	for (nLoop = nIdx - 1, nSrc = (nIdx * 2); nSrc > 0; ) {
		memcpy(ucMinPrefix, ucMin, DB_NPDB_PREFIX_SIZE);
		memset(&prefixAddr, 0x00, sizeof(prefixAddr));

		ucTmp   =   ucMinPrefix[nLoop];
		ucTmp   =   (ucTmp | 0x0f);
		ucMinPrefix[nLoop]  =   ucTmp;
		if ((DB_NPDB_PREFIX_SIZE - (nLoop + 1)) != 0) {
			memset(&ucMinPrefix[nLoop + 1], 0xff,
			       (DB_NPDB_PREFIX_SIZE - nLoop));
		}
		nResult = selectSubNPDBPrefix(ucMinPrefix, &prefixAddr);
		if (nResult >= 0) {
			break;
		}
		nSrc--;
		memset(&prefixAddr, 0x00, sizeof(prefixAddr));
		memcpy(ucMinPrefix, ucMin, DB_NPDB_PREFIX_SIZE);
		memset(&ucMinPrefix[nLoop], 0xff, (DB_NPDB_PREFIX_SIZE - nLoop));
		nResult =   selectSubNPDBPrefix(ucMinPrefix, &prefixAddr);
		if (nResult >= 0) {
			break;
		}
		nSrc--;
		nLoop = nLoop - 1;
	}
	if (nResult >= 0) {
		memcpy(spPrefix, &prefixAddr, sizeof(stRDNpdbPrefixTuple));
		return FLK_SUCCESS;
	}
	return FLK_FAIL;
}

static int selectSubAddress(u_char *ucMin, stSmsAddr *spAddr)
{
	stRDPrefixTuple prefix;

	u_char   ucMinPrefix[DB_PREFIX_SIZE];
//    u_char   ucTmp;
	u_char   ucPrefixAddrType = 0x00;
	u_char   attVal[DB_PREFIX_SIZE];
	char            *noAlign;

	int             tableId, nTupId, nAttId;//, nTmp;
	memset(&prefix, 0x00, sizeof(stRDPrefixTuple));
	memset(ucMinPrefix, 0x00, sizeof(ucMinPrefix));

	tableId = R_DPREFIX;
	nTupId  = 0;
	nAttId  = DB_PREFIX;

	memcpy(attVal,      ucMin,      DB_PREFIX_SIZE);
	if ((nTupId = dma_getTupId(DBMS_STAT, tableId, nAttId, (char *)ucMin, nTupId )) < 0) {
		return -DMAQI_THERE_IS_NOT_THE_TUPLE;
	}
	if (dma_table_retr(DBMS_STAT, tableId, nTupId, &noAlign) < 0) {
		return -DMAQI_THERE_IS_NOT_THE_TUPLE;
	}
	(void)dma_align_make(DBMS_STAT, tableId, (char *)&prefix, noAlign);
	memcpy(spAddr->ucAddrValue, prefix.dbAddress, sizeof(prefix.dbAddress));
	memcpy(&spAddr->ucAddrType, prefix.dbAddrType, sizeof(prefix.dbAddrType));
	memcpy(&spAddr->ucMaxType, prefix.dbAddrType, sizeof(prefix.dbAddrType));

	//SR510 pkg hlr pool routing
	//memcpy(&spAddr->ucHlrGrp, prefix.dbS6cGrpNum, sizeof(prefix.dbS6cGrpNum));
	//memcpy(&spAddr->ucShGrp, prefix.dbShGrpNum, sizeof(prefix.dbShGrpNum));
	//memcpy(&spAddr->ucNetworkType,prefix.dbNetworkType,sizeof(prefix.dbNetworkType));	// added by bjlee for SC_Routing

	ucPrefixAddrType    =   prefix.dbAddrType[0];
	if (ucPrefixAddrType == PC_TYPE) {
		spAddr->iAddrLength =   PC_ADDR_SIZE;
	}
	if (ucPrefixAddrType == ESME_TYPE) {
		spAddr->iAddrLength =   ESME_ADDR_SIZE;
	}

	return FLK_SUCCESS;
}


int selectAddress(u_char *ucMin, int nMinOctetNum, stSmsAddr *spAddr)
{

	int     		nLoop, nSrc;//, nTmp;
	u_char   ucMinPrefix[DB_PREFIX_SIZE];
	u_char   ucTmp;
	int             nResult = 0;
	stSmsAddr       prefixAddr;
	int             nIdx = (DB_PREFIX_SIZE < nMinOctetNum) ? DB_PREFIX_SIZE : nMinOctetNum;

	// for even length error. temporarl coding. must be recoded.
	if (DB_PREFIX_SIZE > nIdx ) {
		nIdx++;
	}

	//for(nLoop=DB_PREFIX_SIZE,nSrc=(DB_PREFIX_SIZE*2);nSrc > 0;)
	for (nLoop = nIdx - 1, nSrc = (nIdx * 2); nSrc > 0;) {
		memcpy(ucMinPrefix, ucMin, sizeof(ucMinPrefix));
		memset(&prefixAddr, 0x00, sizeof(prefixAddr));
		ucTmp   =   ucMinPrefix[nLoop];
		ucTmp   =   (ucTmp | 0x0f);
		ucMinPrefix[nLoop]  =   ucTmp;
		if ((sizeof(ucMinPrefix) - (nLoop + 1)) != 0) {
			memset(&ucMinPrefix[nLoop + 1], 0xff, sizeof(ucMinPrefix) - (nLoop));
		}
		nResult =   selectSubAddress(ucMinPrefix, &prefixAddr);
		if (nResult >= 0) {
			break;
		}
		nSrc--;
		memset(&prefixAddr, 0x00, sizeof(prefixAddr));
		memcpy(ucMinPrefix, ucMin, sizeof(ucMinPrefix));
		memset(&ucMinPrefix[nLoop], 0xff, sizeof(ucMinPrefix) - (nLoop));
		nResult =   selectSubAddress(ucMinPrefix, &prefixAddr);
		if (nResult >= 0) {
			break;
		}
		nSrc--;
		nLoop = nLoop - 1;
	}
	if (nResult >= 0) {
		memcpy(spAddr, &prefixAddr, sizeof(prefixAddr));
		return FLK_SUCCESS;
	}
	return FLK_FAIL;
}


static int selectRNSubAddress(u_char *ucMin, stSmsAddr *spAddr)
{
	stRDPrefix_RNTuple prefix;

	u_char   ucMinPrefix[DB_PREFIX_SIZE];
//    u_char   ucTmp;
	u_char   ucPrefixAddrType = 0x00;
	u_char   attVal[DB_PREFIX_SIZE];
	char            *noAlign;

	int             tableId, nTupId, nAttId;//, nTmp;
	memset(&prefix, 0x00, sizeof(stRDPrefix_RNTuple));
	memset(ucMinPrefix, 0x00, sizeof(ucMinPrefix));

	tableId = R_DPREFIX_RN;
	nTupId  = 0;
	nAttId  = DB_PREFIX;

	memcpy(attVal,      ucMin,      DB_PREFIX_SIZE);
	if ((nTupId = dma_getTupId(DBMS_STAT, tableId, nAttId, (char *)ucMin, nTupId)) < 0) {
		return -DMAQI_THERE_IS_NOT_THE_TUPLE;
	}
	if (dma_table_retr(DBMS_STAT, tableId, nTupId, &noAlign) < 0) {
		return -DMAQI_THERE_IS_NOT_THE_TUPLE;
	}
	(void)dma_align_make(DBMS_STAT, tableId, (char *)&prefix, noAlign);
	memcpy(spAddr->ucAddrValue, prefix.dbAddress, sizeof(prefix.dbAddress));
	memcpy(&spAddr->ucAddrType, prefix.dbAddrType, sizeof(prefix.dbAddrType));

	//SR510 pkg hlr pool routing
	//memcpy(&spAddr->ucHlrGrp, prefix.dbS6cGrpNum, sizeof(prefix.dbS6cGrpNum));
	//memcpy(&spAddr->ucShGrp, prefix.dbShGrpNum, sizeof(prefix.dbShGrpNum));

	ucPrefixAddrType    =   prefix.dbAddrType[0];
	if (ucPrefixAddrType == PC_TYPE) {
		spAddr->iAddrLength =   PC_ADDR_SIZE;
	}
	if (ucPrefixAddrType == ESME_TYPE) {
		spAddr->iAddrLength =   ESME_ADDR_SIZE;
	}

	return FLK_SUCCESS;
}


int selectRNAddress(u_char *ucMin, int nMinOctetNum, stSmsAddr *spAddr)
{

	int     nLoop, nSrc;//, nTmp;
	u_char   ucMinPrefix[DB_PREFIX_SIZE];
	u_char   ucTmp;
	int             nResult = 0;
	stSmsAddr       prefixAddr;
	int             nIdx = (DB_PREFIX_SIZE < nMinOctetNum) ? DB_PREFIX_SIZE : nMinOctetNum;

	// for even length error. temporarl coding. must be recoded.
	if (DB_PREFIX_SIZE > nIdx ) {
		nIdx++;
	}

	//for(nLoop=DB_PREFIX_SIZE,nSrc=(DB_PREFIX_SIZE*2);nSrc > 0;)
	for (nLoop = nIdx - 1, nSrc = (nIdx * 2); nSrc > 0;) {
		memcpy(ucMinPrefix, ucMin, sizeof(ucMinPrefix));
		memset(&prefixAddr, 0x00, sizeof(prefixAddr));
		ucTmp   =   ucMinPrefix[nLoop];
		ucTmp   =   (ucTmp | 0x0f);
		ucMinPrefix[nLoop]  =   ucTmp;
		if ((sizeof(ucMinPrefix) - (nLoop + 1)) != 0) {
			memset(&ucMinPrefix[nLoop + 1], 0xff, sizeof(ucMinPrefix) - (nLoop));
		}
		nResult =   selectRNSubAddress(ucMinPrefix, &prefixAddr);
		if (nResult >= 0) {
			break;
		}
		nSrc--;
		memset(&prefixAddr, 0x00, sizeof(prefixAddr));
		memcpy(ucMinPrefix, ucMin, sizeof(ucMinPrefix));
		memset(&ucMinPrefix[nLoop], 0xff, sizeof(ucMinPrefix) - (nLoop));
		nResult =   selectRNSubAddress(ucMinPrefix, &prefixAddr);
		if (nResult >= 0) {
			break;
		}
		nSrc--;
		nLoop = nLoop - 1;
	}
	if (nResult >= 0) {
		memcpy(spAddr, &prefixAddr, sizeof(prefixAddr));
		return FLK_SUCCESS;
	}
	return FLK_FAIL;
}

u_int GetMdbPrefixAddress(stRDPrefixTuple *spPrefix)
{
	u_int address = 0;

	memcpy(&address, spPrefix->dbAddress, sizeof(u_int));

	/*
	if (spPrefix->dbAddrType[0] == PC_TYPE) {
		address = address >> 16;
	}
	*/

	return address;
}

u_int GetMdbPrefixSMCAddress(stRDPrefix_SMCTuple *spPrefix)
{
	u_int address = 0;
	memcpy(&address, spPrefix->dbAddress, sizeof(u_int));

	return address;
}

//SC Routing by twkim 07.10.02
static int selectSMCSubAddress(u_char *ucMin, stSmsAddr *spAddr)
{
	stRDPrefix_SMCTuple prefix;

	u_char   ucMinPrefix[DB_PREFIX_SIZE];
//    u_char   ucTmp;
	u_char   ucPrefixAddrType = 0x00;
	u_char   attVal[DB_PREFIX_SIZE];
	char            *noAlign;

	int             tableId, nTupId, nAttId;//, nTmp;
	memset(&prefix, 0x00, sizeof(stRDPrefix_SMCTuple));
	memset(ucMinPrefix, 0x00, sizeof(ucMinPrefix));

	tableId = R_DPREFIX_SMC;
	nTupId  = 0;
	nAttId  = DB_PREFIX;

	memcpy(attVal,      ucMin,      DB_PREFIX_SIZE);
	if ((nTupId = dma_getTupId(DBMS_STAT, tableId, nAttId, (char *)ucMin, nTupId )) < 0) {
		return -DMAQI_THERE_IS_NOT_THE_TUPLE;
	}
	if (dma_table_retr(DBMS_STAT, tableId, nTupId, &noAlign) < 0) {
		return -DMAQI_THERE_IS_NOT_THE_TUPLE;
	}
	(void)dma_align_make(DBMS_STAT, tableId, (char *)&prefix, noAlign);
	memcpy(spAddr->ucAddrValue, prefix.dbAddress, sizeof(prefix.dbAddress));
	memcpy(&spAddr->ucAddrType, prefix.dbBlockType, sizeof(prefix.dbBlockType));
	memcpy(&spAddr->ucMaxType, prefix.dbBlockType, sizeof(prefix.dbBlockType));
	memcpy(&spAddr->ucNetworkType, prefix.dbNetworkType, sizeof(prefix.dbNetworkType));	// added by bjlee for SC_Routing
	ucPrefixAddrType    =   prefix.dbBlockType[0];

	//FR730 pkg
	/*
	if(ucPrefixAddrType == PC_TYPE)
	    spAddr->iAddrLength =   PC_ADDR_SIZE;
	if(ucPrefixAddrType == ESME_TYPE)
	    spAddr->iAddrLength =   ESME_ADDR_SIZE;
	*/
	spAddr->iAddrLength =   PC_ADDR_SIZE;

	return FLK_SUCCESS;
}

//SC Routing by twkim 07.10.02
int selectSMCAddress(u_char *ucMin, int nMinOctetNum, stSmsAddr *spAddr)
{

	int     		nLoop, nSrc;//, nTmp;
	u_char   ucMinPrefix[DB_PREFIX_SIZE];
	u_char   ucTmp;
	int             nResult = 0;
	stSmsAddr       prefixAddr;
	int             nIdx = (DB_PREFIX_SIZE < nMinOctetNum) ? DB_PREFIX_SIZE : nMinOctetNum;

	// for even length error. temporarl coding. must be recoded.
	if (DB_PREFIX_SIZE > nIdx ) {
		nIdx++;
	}

	//for(nLoop=DB_PREFIX_SIZE,nSrc=(DB_PREFIX_SIZE*2);nSrc > 0;)
	for (nLoop = nIdx - 1, nSrc = (nIdx * 2); nSrc > 0;) {
		memcpy(ucMinPrefix, ucMin, sizeof(ucMinPrefix));
		memset(&prefixAddr, 0x00, sizeof(prefixAddr));
		ucTmp   =   ucMinPrefix[nLoop];
		ucTmp   =   (ucTmp | 0x0f);
		ucMinPrefix[nLoop]  =   ucTmp;
		if ((sizeof(ucMinPrefix) - (nLoop + 1)) != 0) {
			memset(&ucMinPrefix[nLoop + 1], 0xff, sizeof(ucMinPrefix) - (nLoop));
		}
		nResult =   selectSMCSubAddress(ucMinPrefix, &prefixAddr);
		if (nResult >= 0) {
			break;
		}
		nSrc--;
		memset(&prefixAddr, 0x00, sizeof(prefixAddr));
		memcpy(ucMinPrefix, ucMin, sizeof(ucMinPrefix));
		memset(&ucMinPrefix[nLoop], 0xff, sizeof(ucMinPrefix) - (nLoop));
		nResult =   selectSMCSubAddress(ucMinPrefix, &prefixAddr);
		if (nResult >= 0) {
			break;
		}
		nSrc--;
		nLoop = nLoop - 1;
	}
	if (nResult >= 0) {
		memcpy(spAddr, &prefixAddr, sizeof(prefixAddr));
		return FLK_SUCCESS;
	}
	return FLK_FAIL;
}



static int selectPrefixRec(u_char *ucMin, stRDPrefixTuple *spPrefix)
{

	stRDPrefixTuple prefix;

	char   attVal[DB_PREFIX_SIZE];
	char            *noAlign;
	int             tableId, nTupId, nAttId;//, nTmp;
//	int				nIdx;
	memset(&prefix, 0x00, sizeof(stRDPrefixTuple));
	memset(attVal,  '\0',	DB_PREFIX_SIZE);

	tableId = R_DPREFIX;
	nTupId  = 0;
	nAttId  = DB_PREFIX;

	memcpy(attVal,      ucMin,      DB_PREFIX_SIZE);

	if ((nTupId = dma_getTupId(DBMS_STAT, tableId, nAttId, attVal, nTupId)) < 0) {
		return -DMAQI_THERE_IS_NOT_THE_TUPLE;
	}
	if (dma_table_retr(DBMS_STAT, tableId, nTupId, &noAlign) < 0) {
		return -DMAQI_DB_BAD_TUPLE_ID;
	}
	(void)dma_align_make(DBMS_STAT, tableId, (char *)&prefix, noAlign);
	memcpy(spPrefix, (char *)&prefix, sizeof(stRDPrefixTuple));

	return FLK_SUCCESS;

}

int selectPrefix(u_char *ucMin, int nMinOctetNum, stRDPrefixTuple *spPrefix)
{

	int     nLoop, nSrc;//, nTmp;
	u_char   ucMinPrefix[DB_PREFIX_SIZE];
	u_char   ucTmp;
	int             nResult;
	stRDPrefixTuple  prefix;
	int             nIdx = (DB_PREFIX_SIZE < nMinOctetNum) ? DB_PREFIX_SIZE : nMinOctetNum;


	memcpy(ucMinPrefix, ucMin, sizeof(ucMinPrefix));
	memset(&prefix,		'\0',		sizeof(stRDPrefixTuple));

	nResult =   selectPrefixRec(ucMinPrefix, &prefix);
	if (nResult == FLK_SUCCESS) {
		memcpy(spPrefix, &prefix, sizeof(stRDPrefixTuple));
		return FLK_SUCCESS;
	}

	for (nLoop = nIdx - 1, nSrc = (nIdx * 2); nSrc > 0;) {
		memcpy(ucMinPrefix, ucMin, sizeof(ucMinPrefix));
		memset(&prefix,		'\0',		sizeof(stRDPrefixTuple));

		ucTmp   =   ucMinPrefix[nLoop];
		ucTmp   =   (ucTmp | 0x0f);
		ucMinPrefix[nLoop]  =   ucTmp;
		if ((sizeof(ucMinPrefix) - (nLoop + 1)) != 0) {
			memset(&ucMinPrefix[nLoop + 1], 0xff, sizeof(ucMinPrefix) - (nLoop + 1));
		}
		nResult =   selectPrefixRec(ucMinPrefix, &prefix);
		if (nResult == FLK_SUCCESS) {
			memcpy(spPrefix, &prefix, sizeof(stRDPrefixTuple));
			return FLK_SUCCESS;
		}
		nSrc--;

		memset(&prefix,		'\0',		sizeof(stRDPrefixTuple));
		memcpy(ucMinPrefix, ucMin, sizeof(ucMinPrefix));
		memset(&ucMinPrefix[nLoop], 0xff, sizeof(ucMinPrefix) - (nLoop));
		nResult =   selectPrefixRec(ucMinPrefix, &prefix);
		if (nResult == FLK_SUCCESS) {
			memcpy(spPrefix, &prefix, sizeof(stRDPrefixTuple));
			return FLK_SUCCESS;
		}
		nSrc--;
		nLoop = nLoop - 1;
	}
	return FLK_FAIL;
}

static int selectUdmGrpPrefixRec(u_char *ucMin, stRDS6CPrefixTuple *spPrefix, int appid)
{
	stRDS6CPrefixTuple prefix;

	char   attVal[DB_PREFIX_SIZE];
	char            *noAlign;
	int             tableId, nTupId, nAttId;//, nTmp;
//	int				nIdx;
	memset(&prefix, 0x00, sizeof(stRDS6CPrefixTuple));
	memset(attVal,  '\0',	DB_PREFIX_SIZE);

	switch(appid)
	{
		case 16777312 : // S6C_IF_APP_ID
			tableId = R_DS6CPREFIX;
			break;
		default:
			tableId = R_DSHPREFIX;
			break;
	}
	nTupId  = 0;
	nAttId  = DB_PREFIX;

	memcpy(attVal,      ucMin,      DB_PREFIX_SIZE);

	if ((nTupId = dma_getTupId(DBMS_RCS, tableId, nAttId, attVal, nTupId)) < 0) {
		return -DMAQI_THERE_IS_NOT_THE_TUPLE;
	}
	if (dma_table_retr(DBMS_RCS, tableId, nTupId, &noAlign) < 0) {
		return -DMAQI_DB_BAD_TUPLE_ID;
	}
	(void)dma_align_make(DBMS_RCS, tableId, (char *)&prefix, noAlign);
	memcpy(spPrefix, (char *)&prefix, sizeof(stRDS6CPrefixTuple));

	return FLK_SUCCESS;

}

int selectUdmGrpPrefix(u_char *ucMin, int nMinOctetNum, stRDS6CPrefixTuple *spPrefix, int appid)
{

	int     nLoop, nSrc;//, nTmp;
	u_char   ucMinPrefix[DB_PREFIX_SIZE];
	u_char   ucTmp;
	int             nResult;
	stRDS6CPrefixTuple  prefix;
	int             nIdx = (DB_PREFIX_SIZE < nMinOctetNum) ? DB_PREFIX_SIZE : nMinOctetNum;


	memcpy(ucMinPrefix, ucMin, sizeof(ucMinPrefix));
	memset(&prefix,		'\0',		sizeof(stRDS6CPrefixTuple));

	nResult =   selectUdmGrpPrefixRec(ucMinPrefix, &prefix, appid);
	if (nResult == FLK_SUCCESS) {
		memcpy(spPrefix, &prefix, sizeof(stRDS6CPrefixTuple));
		return FLK_SUCCESS;
	}

	for (nLoop = nIdx - 1, nSrc = (nIdx * 2); nSrc > 0;) {
		memcpy(ucMinPrefix, ucMin, sizeof(ucMinPrefix));
		memset(&prefix,		'\0',		sizeof(stRDS6CPrefixTuple));

		ucTmp   =   ucMinPrefix[nLoop];
		ucTmp   =   (ucTmp | 0x0f);
		ucMinPrefix[nLoop]  =   ucTmp;
		if ((sizeof(ucMinPrefix) - (nLoop + 1)) != 0) {
			memset(&ucMinPrefix[nLoop + 1], 0xff, sizeof(ucMinPrefix) - (nLoop + 1));
		}
		nResult =   selectUdmGrpPrefixRec(ucMinPrefix, &prefix, appid);
		if (nResult == FLK_SUCCESS) {
			memcpy(spPrefix, &prefix, sizeof(stRDS6CPrefixTuple));
			return FLK_SUCCESS;
		}
		nSrc--;

		memset(&prefix,		'\0',		sizeof(stRDS6CPrefixTuple));
		memcpy(ucMinPrefix, ucMin, sizeof(ucMinPrefix));
		memset(&ucMinPrefix[nLoop], 0xff, sizeof(ucMinPrefix) - (nLoop));
		nResult =   selectUdmGrpPrefixRec(ucMinPrefix, &prefix, appid);
		if (nResult == FLK_SUCCESS) {
			memcpy(spPrefix, &prefix, sizeof(stRDS6CPrefixTuple));
			return FLK_SUCCESS;
		}
		nSrc--;
		nLoop = nLoop - 1;
	}
	return FLK_FAIL;
}

int selectServiceRec(u_char ucServiceId, stRDServiceTuple *stpService)
{

	int             tableId,   attId, tupId;
	char            *noAlign;
	char   attVal[DB_SERVICEID_SIZE];


	tableId = R_DSERVICE;
	attId   = DB_SERVICEID;
	tupId = -1;

	memset(attVal,      '\0',       DB_SERVICEID_SIZE);

	attVal[0] = ucServiceId;

	if ((tupId = dma_getTupId(DBMS_PLUS, tableId, attId, attVal, tupId )) < 0) {
		return (-DMAQI_THERE_IS_NOT_THE_TUPLE);
	}

	if (dma_table_retr(DBMS_PLUS, tableId, tupId, &noAlign) < 0) {
		return (-DMAQI_DB_BAD_TUPLE_ID);
	}
	(void)dma_align_make(DBMS_PLUS, tableId, (char *)stpService, noAlign);

	return FLK_SUCCESS;

}

/******************************************************************************
							////// SKT Generation MNP Added //////
******************************************************************************/

static int selectSMCPrefixRec( u_char *ucMin, stRDPrefix_SMCTuple *spPrefix)
{

	stRDPrefix_SMCTuple prefix;

	char   attVal[DB_PREFIX_SIZE];
	char            *noAlign;
	int             tableId, nTupId, nAttId;//, nTmp;
//	int				nIdx;
	memset(&prefix, 0x00, sizeof(stRDPrefixTuple));
	memset(attVal,  '\0',	DB_PREFIX_SIZE);

	tableId = R_DPREFIX_SMC;
	nTupId  = 0;
	nAttId  = DB_PREFIX;

	memcpy(attVal,      ucMin,      DB_PREFIX_SIZE);

	if ((nTupId = dma_getTupId(DBMS_STAT, tableId, nAttId, (char *)ucMin, nTupId)) < 0) {
		return -DMAQI_THERE_IS_NOT_THE_TUPLE;
	}
	if (dma_table_retr(DBMS_STAT, tableId, nTupId, &noAlign) < 0) {
		return -DMAQI_DB_BAD_TUPLE_ID;
	}
	(void)dma_align_make(DBMS_STAT, tableId, (char *)&prefix, noAlign);
	memcpy(spPrefix, (char *)&prefix, sizeof(stRDPrefixTuple));

	return FLK_SUCCESS;

}


int selectSMCPrefix( u_char *ucMin, int	nMinOctetNum, stRDPrefix_SMCTuple *spPrefix)
{

	int     nLoop, nSrc;//, nTmp;
	u_char   ucMinPrefix[DB_PREFIX_SIZE];
	u_char   ucTmp;
	int             nResult;
	stRDPrefix_SMCTuple  prefix;
	int             nIdx = (DB_PREFIX_SIZE < nMinOctetNum) ? DB_PREFIX_SIZE : nMinOctetNum;


	memcpy(ucMinPrefix, ucMin, sizeof(ucMinPrefix));
	memset(&prefix,		'\0',		sizeof(stRDPrefixTuple));

	nResult =   selectSMCPrefixRec(ucMinPrefix, &prefix);
	if (nResult == FLK_SUCCESS) {
		memcpy(spPrefix, &prefix, sizeof(stRDPrefixTuple));
		return FLK_SUCCESS;
	}

	for (nLoop = nIdx - 1, nSrc = (nIdx * 2); nSrc > 0;) {
		memcpy(ucMinPrefix, ucMin, sizeof(ucMinPrefix));
		memset(&prefix,		'\0',		sizeof(stRDPrefixTuple));

		ucTmp   =   ucMinPrefix[nLoop];
		ucTmp   =   (ucTmp | 0x0f);
		ucMinPrefix[nLoop]  =   ucTmp;
		if ((sizeof(ucMinPrefix) - (nLoop + 1)) != 0) {
			memset(&ucMinPrefix[nLoop + 1], 0xff, sizeof(ucMinPrefix) - (nLoop + 1));
		}
		nResult =   selectSMCPrefixRec(ucMinPrefix, &prefix);
		if (nResult == FLK_SUCCESS) {
			memcpy(spPrefix, &prefix, sizeof(stRDPrefixTuple));
			return FLK_SUCCESS;
		}
		nSrc--;

		memset(&prefix,		'\0',		sizeof(stRDPrefixTuple));
		memcpy(ucMinPrefix, ucMin, sizeof(ucMinPrefix));
		memset(&ucMinPrefix[nLoop], 0xff, sizeof(ucMinPrefix) - (nLoop));
		nResult =   selectSMCPrefixRec(ucMinPrefix, &prefix);
		if (nResult == FLK_SUCCESS) {
			memcpy(spPrefix, &prefix, sizeof(stRDPrefixTuple));
			return FLK_SUCCESS;
		}
		nSrc--;
		nLoop = nLoop - 1;
	}
	return FLK_FAIL;

}

//SR410 pkg
int selectM2MGlrRoutePrefix( u_char *ucMin, int	nMinOctetNum )
{

	int 	nLoop = 0;
	int 	nSrc = 0;
	int 	nResult = 0;
	int 	nIdx = 0;
	u_char 	ucMinPrefix[DB_PREFIX_SIZE];
	u_char 	ucTmp;
	u_short ServiceCode = M2M_GLR_ROUTING_MIN;
	stRDConvertSpecialMinTuple stTuple;

	nIdx = (DB_PREFIX_SIZE < nMinOctetNum) ? DB_PREFIX_SIZE : nMinOctetNum;
	memcpy(ucMinPrefix, ucMin, sizeof(ucMinPrefix));
	memset(&stTuple, 0x00, sizeof(stTuple));

	nResult = selectSpecialMin(ServiceCode, ucMinPrefix, (stRDConvertSpecialMinTuple *)&stTuple);

	if (nResult == FLK_SUCCESS) {
		return FLK_SUCCESS;
	}

	for (nLoop = nIdx - 1, nSrc = (nIdx * 2); nSrc > 0;) {
		memcpy(ucMinPrefix, ucMin, sizeof(ucMinPrefix));

		ucTmp   =   ucMinPrefix[nLoop];
		ucTmp   =   (ucTmp | 0x0f);
		ucMinPrefix[nLoop]  =   ucTmp;
		if ((sizeof(ucMinPrefix) - (nLoop + 1)) != 0) {
			memset(&ucMinPrefix[nLoop + 1], 0xff, sizeof(ucMinPrefix) - (nLoop + 1));
		}
		nResult = selectSpecialMin(ServiceCode, ucMinPrefix, (stRDConvertSpecialMinTuple *)&stTuple);
		if (nResult == FLK_SUCCESS) {
			return FLK_SUCCESS;
		}
		nSrc--;

		memcpy(ucMinPrefix, ucMin, sizeof(ucMinPrefix));
		memset(&ucMinPrefix[nLoop], 0xff, sizeof(ucMinPrefix) - (nLoop));
		nResult = selectSpecialMin(ServiceCode, ucMinPrefix, (stRDConvertSpecialMinTuple *)&stTuple);
		if (nResult == FLK_SUCCESS) {
			return FLK_SUCCESS;
		}
		nSrc--;
		nLoop = nLoop - 1;
	}
	return FLK_FAIL;
}

bool isM2MGlrRouteDenyMIN(u_char *cpSrc, int length)
{
	u_short ServiceCode = M2M_GLR_ROUTING_DENY_MIN;
	int ret = 0;
	u_char cpDestMIN[DB_DEST_MIN_SIZE];
	stRDConvertSpecialMinTuple stTuple;

	memset(&stTuple, 0x00, sizeof(stTuple));
	memset(cpDestMIN, 0xff, sizeof(cpDestMIN));
	if ( length > DB_DEST_MIN_SIZE ) {
		length = DB_DEST_MIN_SIZE;
	}
	memcpy(cpDestMIN, cpSrc, length);

	ret = selectSpecialMin(ServiceCode, cpDestMIN, (stRDConvertSpecialMinTuple *)&stTuple);
	if ( ret < 0 ) {
		return false;
	} else {
		return true;
	}
}

bool isM2MGlrRouteAllowMIN(u_char *cpSrc, int length)
{
	u_short ServiceCode = M2M_GLR_ROUTING_ALLOW_MIN;
	int ret = 0;
	u_char cpDestMIN[DB_DEST_MIN_SIZE];
	stRDConvertSpecialMinTuple stTuple;

	memset(&stTuple, 0x00, sizeof(stTuple));
	memset(cpDestMIN, 0xff, sizeof(cpDestMIN));
	if ( length > DB_DEST_MIN_SIZE ) {
		length = DB_DEST_MIN_SIZE;
	}
	memcpy(cpDestMIN, cpSrc, length);

	ret = selectSpecialMin(ServiceCode, cpDestMIN, (stRDConvertSpecialMinTuple *)&stTuple);
	if ( ret < 0 ) {
		return false;
	} else {
		return true;
	}
}

/******************************************************************************
							////// MNP Added //////
******************************************************************************/

static int selectRNPrefixRec(u_char *ucMin, stRDPrefix_RNTuple *spPrefix)
{

	stRDPrefix_RNTuple prefix;

	char   attVal[DB_PREFIX_SIZE];
	char            *noAlign;
	int             tableId, nTupId, nAttId;//, nTmp;
//    int             nIdx;
	memset(&prefix, 0x00, sizeof(stRDPrefixTuple));
	memset(attVal,  '\0',   DB_PREFIX_SIZE);

	tableId = R_DPREFIX_RN;
	nTupId  = 0;
	nAttId  = DB_PREFIX;

	memcpy(attVal,      ucMin,      DB_PREFIX_SIZE);

	if ((nTupId = dma_getTupId(DBMS_STAT, tableId, nAttId, attVal, nTupId )) < 0) {
		return -DMAQI_THERE_IS_NOT_THE_TUPLE;
	}
	if (dma_table_retr(DBMS_STAT, tableId, nTupId, &noAlign) < 0) {
		return -DMAQI_DB_BAD_TUPLE_ID;
	}
	(void)dma_align_make(DBMS_STAT, tableId, (char *)&prefix, noAlign);
	memcpy(spPrefix, (char *)&prefix, sizeof(stRDPrefixTuple));

	return FLK_SUCCESS;

}

int selectRNPrefix(u_char *ucMin, int nMinOctetNum, stRDPrefix_RNTuple *spPrefix)
{

	int     nLoop, nSrc;//, nTmp;
	u_char   ucMinPrefix[DB_PREFIX_SIZE];
	u_char   ucTmp;
	int             nResult;
	stRDPrefix_RNTuple  prefix;
	int             nIdx = (DB_PREFIX_SIZE < nMinOctetNum) ? DB_PREFIX_SIZE : nMinOctetNum;

	memcpy(ucMinPrefix, ucMin, sizeof(ucMinPrefix));
	memset(&prefix,     '\0',       sizeof(stRDPrefixTuple));

	nResult =   selectRNPrefixRec(ucMinPrefix, &prefix);
	if (nResult == FLK_SUCCESS) {
		memcpy(spPrefix, &prefix, sizeof(stRDPrefixTuple));
		return FLK_SUCCESS;
	}

	for (nLoop = nIdx - 1, nSrc = (nIdx * 2); nSrc > 0;) {
		memcpy(ucMinPrefix, ucMin, sizeof(ucMinPrefix));
		memset(&prefix,     '\0',       sizeof(stRDPrefixTuple));

		ucTmp   =   ucMinPrefix[nLoop];
		ucTmp   =   (ucTmp | 0x0f);
		ucMinPrefix[nLoop]  =   ucTmp;
		if ((sizeof(ucMinPrefix) - (nLoop + 1)) > 0) {
			memset(&ucMinPrefix[nLoop + 1], 0xff, sizeof(ucMinPrefix) - (nLoop + 1));
		}
		nResult =   selectRNPrefixRec(ucMinPrefix, &prefix);
		if (nResult == FLK_SUCCESS) {
			memcpy(spPrefix, &prefix, sizeof(stRDPrefixTuple));
			return FLK_SUCCESS;
		}
		nSrc--;

		memset(&prefix,     '\0',       sizeof(stRDPrefixTuple));
		memcpy(ucMinPrefix, ucMin, sizeof(ucMinPrefix));
		memset(&ucMinPrefix[nLoop], 0xff, sizeof(ucMinPrefix) - (nLoop));
		nResult =   selectRNPrefixRec(ucMinPrefix, &prefix);
		if (nResult == FLK_SUCCESS) {
			memcpy(spPrefix, &prefix, sizeof(stRDPrefixTuple));
			return FLK_SUCCESS;
		}
		nSrc--;
		nLoop = nLoop - 1;
	}
	return FLK_FAIL;

}


//static int
int selectVMinPrefixRec(u_char *ucMin, stRDVMIN_PrefixTuple *spPrefix)
{
	stRDVMIN_PrefixTuple prefix;

	char   attVal[DB_VMIN_PREFIX_SIZE];
	char            *noAlign;
	int             tableId, nTupId, nAttId;//, nTmp;
//	int				nIdx;
	memset(&prefix, 0x00, sizeof(stRDVMIN_PrefixTuple));
	memset(attVal, '\0', DB_VMIN_PREFIX_SIZE);

	tableId = R_DVMIN_PREFIX;
	nTupId  = 0;
	nAttId  = DB_VMIN_PREFIX;

	memcpy(attVal, ucMin, DB_VMIN_PREFIX_SIZE);

	if ((nTupId = dma_getTupId(DBMS_PLUS, tableId, nAttId, attVal, nTupId )) < 0) {
		return -DMAQI_THERE_IS_NOT_THE_TUPLE;
	}
	if (dma_table_retr(DBMS_PLUS, tableId, nTupId, &noAlign) < 0) {
		return -DMAQI_DB_BAD_TUPLE_ID;
	}
	(void)dma_align_make(DBMS_PLUS, tableId, (char *)&prefix, noAlign);
	memcpy(spPrefix, (char *)&prefix, sizeof(stRDVMIN_PrefixTuple));

	return FLK_SUCCESS;
}

int selectVMinPrefix(u_char *ucMin, int nMinOctetNum, stRDVMIN_PrefixTuple *spPrefix)
{
	int nLoop, nSrc;//, nTmp;
	u_char   ucMinPrefix[DB_VMIN_PREFIX_SIZE];
	u_char   ucTmp;
	int             nResult;
	stRDVMIN_PrefixTuple  prefix;
	int             nIdx = (DB_VMIN_PREFIX_SIZE < nMinOctetNum) ? DB_VMIN_PREFIX_SIZE : nMinOctetNum;

	memcpy(ucMinPrefix, ucMin, sizeof(ucMinPrefix));
	memset(&prefix, '\0', sizeof(stRDVMIN_PrefixTuple));

	nResult = selectVMinPrefixRec(ucMinPrefix, &prefix);
	if (nResult == FLK_SUCCESS) {
		memcpy(spPrefix, &prefix, sizeof(stRDVMIN_PrefixTuple));
		return FLK_SUCCESS;
	}

	for (nLoop = nIdx - 1, nSrc = (nIdx * 2); nSrc > 0;) {
		memcpy(ucMinPrefix, ucMin, sizeof(ucMinPrefix));
		memset(&prefix, '\0', sizeof(stRDVMIN_PrefixTuple));

		ucTmp   =   ucMinPrefix[nLoop];
		ucTmp   =   (ucTmp | 0x0f);
		ucMinPrefix[nLoop]  =   ucTmp;
		if ((sizeof(ucMinPrefix) - (nLoop + 1)) != 0) {
			memset(&ucMinPrefix[nLoop + 1], 0xff, sizeof(ucMinPrefix) - (nLoop + 1));
		}
		nResult =   selectVMinPrefixRec(ucMinPrefix, &prefix);
		if (nResult == FLK_SUCCESS) {
			memcpy(spPrefix, &prefix, sizeof(stRDPrefixTuple));
			return FLK_SUCCESS;
		}
		nSrc--;

		memset(&prefix, '\0', sizeof(stRDVMIN_PrefixTuple));
		memcpy(ucMinPrefix, ucMin, sizeof(ucMinPrefix));
		memset(&ucMinPrefix[nLoop], 0xff, sizeof(ucMinPrefix) - (nLoop));
		nResult = selectVMinPrefixRec(ucMinPrefix, &prefix);
		if (nResult == FLK_SUCCESS) {
			memcpy(spPrefix, &prefix, sizeof(stRDVMIN_PrefixTuple));
			return FLK_SUCCESS;
		}
		nSrc--;
		nLoop = nLoop - 1;
	}

	return FLK_FAIL;
}

int checkVMinPrefix(u_char *ucMin, int nLength)
{
	stRDVMIN_PrefixTuple stVMinPrefixTuple;
	int nStartLength, nEndLength;

	bzero(&stVMinPrefixTuple, sizeof(stVMinPrefixTuple));
	if (selectVMinPrefix(ucMin, nLength, &stVMinPrefixTuple) == FLK_FAIL) {
		return FLK_FAIL;
	}

	nStartLength = stVMinPrefixTuple.dbVMinStartDigitLength[0];
	nEndLength = stVMinPrefixTuple.dbVMinEndDigitLength[0];

	if ((nLength >= nStartLength) && (nLength <= nEndLength)) {
		return FLK_SUCCESS;
	}

	return FLK_FAIL;
}

/* ISDN Table select */
int selectIsdnRec(u_char *usIsdn, stRDIsdnTuple *spIsdn)
{
	int                 tableId, attId, tupId;
	char                attVal[DB_ISDN_SIZE];
	stRDIsdnTuple       l_isdn;
	char      			*noAlign;
//	int                 nLoop;

	tableId = R_DISDN;
	attId = DB_ISDN_NUMBER;
	tupId = -1;
	memset(attVal,      0x00,  DB_ISDN_SIZE);
	memcpy(attVal,      usIsdn,  DB_ISDN_SIZE);

	if ((tupId = dma_getTupId(DBMS_STAT, tableId, attId, attVal, tupId)) < 0) {
		return -DMAQI_THERE_IS_NOT_THE_TUPLE;;
	}

	if (dma_table_retr(DBMS_STAT, tableId, tupId, &noAlign) < 0) {
		return -DMAQI_DB_BAD_TUPLE_ID;
	}
	(void)dma_align_make(DBMS_STAT, tableId, (char *)&l_isdn, noAlign);

	memcpy(spIsdn, &l_isdn, sizeof(stRDIsdnTuple));
	return FLK_SUCCESS;
}

/* SR720 DIAMETER ISDN Table select */
int selectDiaSmscIdRec(u_char *ucScPc, stRDiameterSMSCTuple *spIsdn)
{
	int            	 	    tableId, attId, tupId;
	char            	    attVal[DB_SMSC_ID_SIZE];
	stRDiameterSMSCTuple 	l_isdn;
	char      				*noAlign;

	tableId = R_DDIAMETERSMSC;
	attId = DB_SMSC_ID;
	tupId = -1;
	memset(attVal,      0x00,  DB_SMSC_ID_SIZE);
	memcpy(attVal,      ucScPc,  DB_SMSC_ID_SIZE);

	if ((tupId = dma_getTupId(DBMS_RCS, tableId, attId, attVal, tupId)) < 0) {
		return -DMAQI_THERE_IS_NOT_THE_TUPLE;;
	}

	if (dma_table_retr(DBMS_RCS, tableId, tupId, &noAlign) < 0) {
		return -DMAQI_DB_BAD_TUPLE_ID;
	}
	(void)dma_align_make(DBMS_RCS, tableId, (char *)&l_isdn, noAlign);

	memcpy(spIsdn, &l_isdn, sizeof(stRDiameterSMSCTuple));
	return FLK_SUCCESS;
}


//SR420 pkg
int select_udm_info( u_int usPc, stRDUdmInfoTuple *spUdmInfo)
{

	int                 tableId, attId, tupId;
	char                attVal[DB_UDM_NUM_SIZE];
	char      			*noAlign;

	tableId = R_DUDMINFO;
	attId = DB_UDM_NUM;
	tupId = -1;
	memset(attVal,      0x00,  DB_UDM_NUM_SIZE);
	u_int unPc = usPc;
	memcpy(attVal, &unPc, sizeof(u_int));

	if ((tupId = dma_getTupId(DBMS_PLUS, tableId, attId, attVal, tupId)) < 0) {
		return -DMAQI_THERE_IS_NOT_THE_TUPLE;;
	}

	if (dma_table_retr(DBMS_PLUS, tableId, tupId, &noAlign) < 0) {
		return -DMAQI_DB_BAD_TUPLE_ID;
	}
	(void)dma_align_make(DBMS_PLUS, tableId, (char *)spUdmInfo, noAlign);

	return FLK_SUCCESS;
}

//SR420 pkg
int select_udm_bk_grp_info( u_char ucHlrGrpNum, stRDUdmBkGrpTuple *spUdmBkGrp)
{

	int                 tableId, attId, tupId;
	char                attVal[DB_GROUP_UDM_SIZE];
	char      			*noAlign;

	tableId = R_DUDMBKGRP;
	attId = DB_UDMBK_GRPNUM;
	tupId = -1;
	memset(attVal,      0x00,  DB_GROUP_UDM_SIZE);
	memcpy(attVal, &ucHlrGrpNum, DB_GROUP_UDM_SIZE);

	if ((tupId = dma_getTupId(DBMS_STAT, tableId, attId, attVal, tupId)) < 0) {
		return -DMAQI_THERE_IS_NOT_THE_TUPLE;;
	}

	if (dma_table_retr(DBMS_STAT, tableId, tupId, &noAlign) < 0) {
		return -DMAQI_DB_BAD_TUPLE_ID;
	}
	(void)dma_align_make(DBMS_STAT, tableId, (char *)spUdmBkGrp, noAlign);

	return FLK_SUCCESS;
}

//FR230 special number LSH
int selectSpecialMin( u_short usServiceCode, u_char *ucMin, stRDConvertSpecialMinTuple *spSpecialMin)
{
	//stRDConvertDestData SpecialMin;
	stRDConvertSpecialMinTuple SpecialMin;
	u_char attVal[DB_TYPE_MATCH_DEST_SIZE];

	char *noAlign = NULL;
	int tableId = 0;
	int nTupId = 0;
	int nAttId = 0;

	//memset(&SpecialMin, 0xff, sizeof(stRDConvertDestData));
	memset(&SpecialMin, 0xff, sizeof(stRDConvertSpecialMinTuple));
	memset(attVal, 0xff, DB_TYPE_MATCH_DEST_SIZE);

	tableId = R_SPECIAL_DESTMIN;
	nTupId = 0;
	nAttId = DB_TYPE_MATCH_DESTMIN;

	memcpy(attVal, &usServiceCode, sizeof(unsigned short));
	memcpy(&attVal[2], ucMin, DB_CONVERT_DEST_SIZE);
	if ((nTupId = dma_getTupId(DBMS_STAT, tableId, nAttId, (char *)attVal, nTupId)) < 0) {
		return -DMAQI_THERE_IS_NOT_THE_TUPLE;
	}
	if (dma_table_retr(DBMS_STAT, tableId, nTupId, &noAlign) < 0) {
		return -DMAQI_DB_BAD_TUPLE_ID;
	}
	(void)dma_align_make(DBMS_STAT, tableId, (char *)&SpecialMin, noAlign);

	//memcpy(spSpecialMin, &SpecialMin, sizeof(stRDConvertDestData));
	memcpy(spSpecialMin, &SpecialMin, sizeof(stRDConvertSpecialMinTuple));
	return FLK_SUCCESS;
}
bool isDestMinParallelsSvc(u_char *dbDestMin)
{
	u_short ServiceCode = ESME_PARALLEL_SVC;
	int ret = 0;
	stRDConvertSpecialMinTuple stTuple;

	memset(&stTuple, 0x00, sizeof(stTuple));

	ret = selectSpecialMin(ServiceCode, dbDestMin, (stRDConvertSpecialMinTuple *)&stTuple);
	if ( ret < 0 ) {
		return false;
	}

	if ( stTuple.dbConvertDest[0] == 0x00 ) {
		return false;
	} else {
		return true;
	}
}

bool isMmsNotiRoute(char *cpTeleSvcId, int *nRouteInfo)
{
	u_short ServiceCode = MMS_NOTI_TID_ROUTE;
	int ret = 0;
	u_char cpDestMIN[DB_DEST_MIN_SIZE];
	stRDConvertSpecialMinTuple stTuple;

	memset(&stTuple, 0x00, sizeof(stTuple));
	memset(cpDestMIN, 0xff, sizeof(cpDestMIN));

	memcpy(cpDestMIN, cpTeleSvcId, DB_TELESERVICEID_SIZE);
	ret = selectSpecialMin(ServiceCode, cpDestMIN, (stRDConvertSpecialMinTuple *)&stTuple);
	if ( ret < 0 ) {
		return false;
	}

	*nRouteInfo = stTuple.dbConvertDest[0];

	return true;
}

bool isSpamFilteringTID(u_short usTid)
{
	/*
	SPAM_FILTERING_TID = 6,
	SPAM_FILTERING_CID = 7
	*/
	u_short ServiceCode = SPAM_FILTERING_TID;
	int ret = 0;

	u_char cpDestMIN[DB_DEST_MIN_SIZE];
	stRDConvertSpecialMinTuple stTuple;

	memset(&stTuple, 0x00, sizeof(stTuple));
	memset(cpDestMIN, 0xff, sizeof(cpDestMIN));
	memcpy(cpDestMIN, &usTid, DB_TELESERVICEID_SIZE);

	ret = selectSpecialMin(ServiceCode, cpDestMIN, (stRDConvertSpecialMinTuple *)&stTuple);
	if ( ret < 0 ) {
		return false;
	} else {
		return true;
	}
}

bool isSpamFilteringCID(u_char *cpCID, int length)
{
	/*
	SPAM_FILTERING_TID = 6,
	SPAM_FILTERING_CID = 7
	*/
	u_short ServiceCode = SPAM_FILTERING_CID;
	int ret = 0;
	u_char cpDestMIN[DB_DEST_MIN_SIZE];
	stRDConvertSpecialMinTuple stTuple;

	memset(&stTuple, 0x00, sizeof(stTuple));
	memset(cpDestMIN, 0xff, sizeof(cpDestMIN));
	if ( length > DB_DEST_MIN_SIZE ) {
		length = DB_DEST_MIN_SIZE;
	}
	memcpy(cpDestMIN, cpCID, length);

	ret = selectSpecialMin(ServiceCode, cpDestMIN, (stRDConvertSpecialMinTuple *)&stTuple);
	if ( ret < 0 ) {
		return false;
	} else {
		return true;
	}
}

bool isMoLimitSpamFilteringTID(u_short usTid)
{
	u_short ServiceCode = MO_LIMIT_SPAM_FILTERING_TID;
	int ret = 0;

	u_char cpDestMIN[DB_DEST_MIN_SIZE];
	stRDConvertSpecialMinTuple stTuple;

	memset(&stTuple, 0x00, sizeof(stTuple));
	memset(cpDestMIN, 0xff, sizeof(cpDestMIN));
	memcpy(cpDestMIN, &usTid, DB_TELESERVICEID_SIZE);

	ret = selectSpecialMin(ServiceCode, cpDestMIN, (stRDConvertSpecialMinTuple *)&stTuple);
	if ( ret < 0 ) {
		return false;
	} else {
		return true;
	}
}

bool is_mcs_apply_tid(u_short usTid)
{
    u_short ServiceCode = MCS_APPLY_TID;
    int ret = 0;

    u_char cpKey[DB_DEST_MIN_SIZE];
    stRDConvertSpecialMinTuple stTuple;
            
    memset(&stTuple, 0x00, sizeof(stTuple));
    memset(cpKey, 0xff, sizeof(cpKey));
    memcpy(cpKey, &usTid, DB_TELESERVICEID_SIZE);
                
    ret = selectSpecialMin(ServiceCode, cpKey, (stRDConvertSpecialMinTuple *)&stTuple);
    if ( ret < 0 )
    {
        return false;
    }else {
        return true;
    }
}

bool isMoLimitSpamFilteringCID(u_char *cpCID, int length)
{
	/*
	SPAM_FILTERING_TID = 6,
	SPAM_FILTERING_CID = 7
	*/
	u_short ServiceCode = MO_LIMIT_SPAM_FILTERING_CID;
	int ret = 0;
	u_char cpDestMIN[DB_DEST_MIN_SIZE];
	stRDConvertSpecialMinTuple stTuple;

	memset(&stTuple, 0x00, sizeof(stTuple));
	memset(cpDestMIN, 0xff, sizeof(cpDestMIN));
	if ( length > DB_DEST_MIN_SIZE ) {
		length = DB_DEST_MIN_SIZE;
	}
	memcpy(cpDestMIN, cpCID, length);

	ret = selectSpecialMin(ServiceCode, cpDestMIN, (stRDConvertSpecialMinTuple *)&stTuple);
	if ( ret < 0 ) {
		return false;
	} else {
		return true;
	}
}

bool is_mcs_deny_cid(u_char *cpCID, int length)
{
    u_short ServiceCode = MCS_DENY_CID;                                                                             
    int ret = 0;                                                                                                       
    u_char cpKey[DB_DEST_MIN_SIZE];                                                                                  
    stRDConvertSpecialMinTuple stTuple;                                                                                
    
    memset(&stTuple, 0x00, sizeof(stTuple));                                                                           
    memset(cpKey, 0xff, sizeof(cpKey));                                                                        
    if ( length > DB_DEST_MIN_SIZE ) {                                                                                 
        length = DB_DEST_MIN_SIZE;
    }                                                                                                                  
    memcpy(cpKey, cpCID, length);                                                                                 
   
    ret = selectSpecialMin(ServiceCode, cpKey, (stRDConvertSpecialMinTuple *)&stTuple);                             
    if ( ret < 0 )
    {
        return false;
    }else {
        return true;
    }
}

bool isTzSmsTID(u_short usTid)
{
	u_short ServiceCode = TZ_SMS_TID;
	int ret = 0;

	u_char cpDestMIN[DB_DEST_MIN_SIZE];
	stRDConvertSpecialMinTuple stTuple;

	memset(&stTuple, 0x00, sizeof(stTuple));
	memset(cpDestMIN, 0xff, sizeof(cpDestMIN));
	memcpy(cpDestMIN, &usTid, DB_TELESERVICEID_SIZE);

	ret = selectSpecialMin(ServiceCode, cpDestMIN, (stRDConvertSpecialMinTuple *)&stTuple);
	if ( ret < 0 ) {
		return false;
	} else {
		return true;
	}
}

bool isIPMGRouteTid(u_short Tid)
{
	u_short ServiceCode = IPMG_ROUTING_TID;
	int ret = 0;
	u_char cpDestMIN[DB_DEST_MIN_SIZE];
	stRDConvertSpecialMinTuple stTuple;

	memset(&stTuple, 0x00, sizeof(stTuple));
	memset(cpDestMIN, 0xff, sizeof(cpDestMIN));
	memcpy(cpDestMIN, &Tid, sizeof(u_short));

	ret = selectSpecialMin(ServiceCode, cpDestMIN, (stRDConvertSpecialMinTuple *)&stTuple);
	if ( ret < 0 ) {
		return false;
	} else {
		return true;
	}
}

bool isEpeRelinkTid(u_short Tid)
{
	u_short ServiceCode = SMS_EPE_RELINK_TID;
	int ret = 0;
	u_char cpDestMIN[DB_DEST_MIN_SIZE];
	stRDConvertSpecialMinTuple stTuple;

	memset(&stTuple, 0x00, sizeof(stTuple));
	memset(cpDestMIN, 0xff, sizeof(cpDestMIN));

	memcpy(cpDestMIN, &Tid, sizeof(u_short));

	ret = selectSpecialMin(ServiceCode, cpDestMIN, (stRDConvertSpecialMinTuple *)&stTuple);
	if ( ret < 0 ) {
		return false;
	} else {
		return true;
	}
}

static int selectNpdbRNRec(u_char *ucMin, stRDNpdbRnRouteTuple *spPrefix)
{

	stRDNpdbRnRouteTuple prefix;

	char   attVal[DB_PREFIX_SIZE];
	char            *noAlign;
	int             tableId, nTupId, nAttId;//, nTmp;
//	int				nIdx;
	memset(&prefix, 0x00, sizeof(stRDNpdbRnRouteTuple));
	memset(attVal,  '\0',	DB_PREFIX_SIZE);

	tableId = R_DNPDB_RN_ROUTE;
	nTupId  = 0;
	nAttId  = DB_NPDB_RN_PREFIX;

	memcpy(attVal,      ucMin,      DB_PREFIX_SIZE);

	if ((nTupId = dma_getTupId(DBMS_STAT, tableId, nAttId, attVal, nTupId)) < 0) {
		return -DMAQI_THERE_IS_NOT_THE_TUPLE;
	}
	if (dma_table_retr(DBMS_STAT, tableId, nTupId, &noAlign) < 0) {
		return -DMAQI_DB_BAD_TUPLE_ID;
	}
	(void)dma_align_make(DBMS_STAT, tableId, (char *)&prefix, noAlign);
	memcpy(spPrefix, (char *)&prefix, sizeof(stRDNpdbRnRouteTuple));

	return FLK_SUCCESS;

}
int selectNpdbRN( u_char *ucMin, int nMinOctetNum, stRDNpdbRnRouteTuple *spPrefix)
{

	int     nLoop, nSrc;//, nTmp;
	u_char   ucMinPrefix[DB_PREFIX_SIZE];
	u_char   ucTmp;
	int             nResult;
	stRDNpdbRnRouteTuple  prefix;
	int             nIdx = (DB_PREFIX_SIZE < nMinOctetNum) ? DB_PREFIX_SIZE : nMinOctetNum;


	memcpy(ucMinPrefix, ucMin, sizeof(ucMinPrefix));
	memset(&prefix,		'\0',		sizeof(stRDNpdbRnRouteTuple));

	nResult =   selectNpdbRNRec(ucMinPrefix, &prefix);
	if (nResult == FLK_SUCCESS) {
		memcpy(spPrefix, &prefix, sizeof(stRDNpdbRnRouteTuple));
		return FLK_SUCCESS;
	}

	for (nLoop = nIdx - 1, nSrc = (nIdx * 2); nSrc > 0;) {
		memcpy(ucMinPrefix, ucMin, sizeof(ucMinPrefix));
		memset(&prefix,		'\0',		sizeof(stRDNpdbRnRouteTuple));

		ucTmp   =   ucMinPrefix[nLoop];
		ucTmp   =   (ucTmp | 0x0f);
		ucMinPrefix[nLoop]  =   ucTmp;
		if ((sizeof(ucMinPrefix) - (nLoop + 1)) != 0) {
			memset(&ucMinPrefix[nLoop + 1], 0xff, sizeof(ucMinPrefix) - (nLoop + 1));
		}
		nResult =   selectNpdbRNRec(ucMinPrefix, &prefix);
		if (nResult == FLK_SUCCESS) {
			memcpy(spPrefix, &prefix, sizeof(stRDNpdbRnRouteTuple));
			return FLK_SUCCESS;
		}
		nSrc--;

		memset(&prefix,		'\0',		sizeof(stRDNpdbRnRouteTuple));
		memcpy(ucMinPrefix, ucMin, sizeof(ucMinPrefix));
		memset(&ucMinPrefix[nLoop], 0xff, sizeof(ucMinPrefix) - (nLoop));
		nResult =   selectNpdbRNRec(ucMinPrefix, &prefix);
		if (nResult == FLK_SUCCESS) {
			memcpy(spPrefix, &prefix, sizeof(stRDNpdbRnRouteTuple));
			return FLK_SUCCESS;
		}
		nSrc--;
		nLoop = nLoop - 1;
	}
	return FLK_FAIL;
}
//DR610 LSH
int selectServiceControl(u_short unServiceCode, stRDServiceControlTuple *record )
{
	u_char *noAlign;

	if ( dma_getTuplePtr(DBMS_PLUS, R_DSERVICECONTROL, DB_SERVICE_ID, 0, (char *)&unServiceCode, DB_SERVICE_ID_SIZE,
	                     (char **)&noAlign) < 0 ) {
		return FLK_FAIL;
	}

	memcpy(record, noAlign, sizeof(stRDServiceControlTuple));
	return FLK_SUCCESS;
}

int selectVerifyCharset(u_int in_dcs, u_int in_charset, u_int out_dcs, u_int *out_chartset)
{
	stRDCharsetTuple stRDCharset;
	stRDCharsetData *spRDCharset = NULL;

	memset(&stRDCharset, 0x00, sizeof(stRDCharset));

	spRDCharset = (stRDCharsetData *)&stRDCharset;

	spRDCharset->in_charset = in_dcs;
	spRDCharset->in_value = in_charset;
	spRDCharset->out_charset = out_dcs;

	u_char *noAlign = NULL;

	if ( dma_getTuplePtr(DBMS_PLUS, R_DCHARSET, DB_CHARSET_KEY, 0, (char *)&stRDCharset.charset_key, DB_CHARSET_KEY_SIZE,
	                     (char **)&noAlign)  < 0 ) {
		// TODO DELETE Log
		return FLK_FAIL;
	}

	spRDCharset = (stRDCharsetData *)noAlign;
	memcpy(out_chartset, &spRDCharset->out_value, sizeof(u_int));
	//SR100
	*out_chartset = htonl(*out_chartset);
	return FLK_SUCCESS;
}

//FR530  pkg LEE
bool isNPACheck(u_char *Npa)
{
	u_short ServiceCode = NPA_MIN_SVC;
	int ret = 0;

	u_char cpNpaMIN[DB_TYPE_MATCH_DEST_SIZE];
	stRDConvertSpecialMinTuple stTuple;
	memset(cpNpaMIN, 0xff, sizeof(cpNpaMIN));
	cpNpaMIN[0] = Npa[0];
	cpNpaMIN[1] = (Npa[1] & 0xF0) | 0x0F;

	ret = selectSpecialMin(ServiceCode, cpNpaMIN, (stRDConvertSpecialMinTuple *)&stTuple);
	if ( ret < 0 ) {
		return false;
	} else {
		return true;
	}
}

//FR530 pkg LEE
int selectSvcRouteID(u_short unServiceCode, stRDServiceIdTuple *spTuple)
{
	u_char *noAlign = NULL;
	if ( dma_getTuplePtr(DBMS_PLUS, R_DSVCIDROUTE, DB_SI_SERVICE_ID, 0, (char *)&unServiceCode, DB_SI_SERVICE_ID_SIZE,
	                     (char **)&noAlign) < 0 ) {
		return FLK_FAIL;
	}
	memcpy(spTuple, noAlign, sizeof(stRDServiceIdTuple));
	return FLK_SUCCESS;
}

bool is_callback_deny_expt_src_min(u_char *ucMin)
{
	char attVal[DB_EXP_SOURCE_MIN_SIZE];
	int tableId = 0;
	int nTupId = 0;
	int nAttId = 0;

	memset(attVal, 0xff, DB_EXP_SOURCE_MIN_SIZE);

	tableId = R_CBCHGEXPMIN;
	nTupId = 0;
	nAttId = DB_EXP_SOURCE_MIN;

	memcpy(attVal, (char *)ucMin, DB_EXP_SOURCE_MIN_SIZE);

	if ((nTupId = dma_getTupId(DBMS_STAT, tableId, nAttId, (char *)attVal, nTupId)) < 0) {
		return false;
	}
	return true;
}

bool isFailReasonLogPrint(u_short usTid)
{
	u_short ServiceCode = FAIL_REASON_LOG_PRINT;
	int ret = 0;

	u_char cpDestMIN[DB_DEST_MIN_SIZE];
	stRDConvertSpecialMinTuple stTuple;

	memset(&stTuple, 0x00, sizeof(stTuple));
	memset(cpDestMIN, 0xff, sizeof(cpDestMIN));
	memcpy(cpDestMIN, &usTid, DB_TELESERVICEID_SIZE);

	ret = selectSpecialMin(ServiceCode, cpDestMIN, (stRDConvertSpecialMinTuple *)&stTuple);
	if ( ret < 0 ) {
		return false;
	} else {
		return true;
	}
}



