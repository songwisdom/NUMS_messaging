#include <spdlog/spdlog.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/msg.h>
#include "FLKBaseDef.h"
#include "SmsIpc.h"
#include "Esme_Struct.h"
#include "MdbTableInfo.h"
#include "MdbDmaqi.h"
#include "MdbCommon.h"
#include "MdbMessage.h"

#include	"ECommon.h"
#include    "Debug.h"
#include 	"StatRecord.h"
#include 	"StatTable.h"
#include 	"SmsStat.h"
//#include 	"SmsStatistics.h"
#include 	"SmsStat.h"

extern sttAqiInfo *gpAqiInfo[MAX_DBMS_CNT];

int selectConfigText(u_int unServiceCode, stRDTextConfigData *record )
{
//	sttAqiInfo *pAqiInfo = gpAqiInfo[DBMS_PLUS];
//	Rdir *pTbl = pAqiInfo->relationDir[R_DESMESYSTEMID];
	u_char *noAlign;

	if ( dma_getTuplePtr(DBMS_PLUS, R_DTEXTCONFIG, DB_SERVICE_CODE, 0, (char *)&unServiceCode, DB_MDTC_SERVICE_CODE_SIZE,
	                     (char **)&noAlign) < 0 ) {
		spdlog::error("[{}:{}:{}()] select failed key [{}]", __FILE__, __LINE__, __func__,  unServiceCode);
		return FLK_FAIL;
	}

	memcpy(record, noAlign, sizeof(stRDTextConfig));
	return FLK_SUCCESS;
}
int selectConfigTextByCode(u_short usGroupCode,  u_short usServiceType, stRDTextConfigData *record )
{
	u_int unServiceCode = 0;
	u_char *ptr = (u_char *)&unServiceCode;

	memcpy(ptr, &usGroupCode, sizeof(u_short));
	memcpy(&ptr[2], &usServiceType, sizeof(u_short));
	return selectConfigText(unServiceCode, record );
}


int insertConfigText(stRDTextConfigData *record)
{
	int flag = NO_RESULT;
	int rc = 0;
	Result result;
	rc =  db_insert(DBMS_PLUS, R_DTEXTCONFIG, (void *)record, &result, flag);
	if ( rc < 0 ) {
		spdlog::error("[{} : {}] config text table insert failed [{}]", __func__, __LINE__, rc);
		return FLK_FAIL;
	}
	return FLK_SUCCESS;
}

int updateConfigText(stRDTextConfigData *record)
{
	unsigned int unServiceCode = 0;
	char *noAlign = NULL;
//	int rc = 0;
	stRDTextConfig *data = (stRDTextConfig *)record;

	memcpy(&unServiceCode, data->dbServiceCode, DB_MDTC_SERVICE_CODE_SIZE);

	if ( dma_getTuplePtr(DBMS_PLUS, R_DTEXTCONFIG, DB_SERVICE_CODE, 0, (char *)&unServiceCode, DB_MDTC_SERVICE_CODE_SIZE,
	                     (char **)&noAlign) < 0 ) {
		spdlog::error("[{}:{}:{}()] select failed key [{}]", __FILE__, __LINE__, __func__,  unServiceCode);
		return FLK_FAIL;
	}

	data = (stRDTextConfig *)noAlign;
	memcpy(data, record, sizeof(stRDTextConfig));

	/*
	rc = db_dml(DBMS_PLUS, R_DTEXTCONFIG, DB_SERVICE_CODE, &unServiceCode, 0, (char **)&noAlign);
	if ( rc < 0 ) {
		spdlog::error("[{} : {}] config text table update failed [{}]", __func__, __LINE__, rc);
		return FLK_FAIL;
	}
	memcpy(noAlign, record, sizeof(stRDTextConfigData));
	*/
	return FLK_SUCCESS;
}

int deleteConfigText(u_int unServiceCode)
{
	int rc = 0;
	int		nFlag = NO_RESULT;
	Result	stResult;
	rc = db_dml(DBMS_PLUS, R_DTEXTCONFIG, DB_SERVICE_CODE, (char *)&unServiceCode, DB_MDTC_SERVICE_CODE_SIZE, 0, &stResult, nFlag, DELETE);
	//rc =  mdbDeleteOneSM(unServiceCode);
	if ( rc != 1 ) {
		spdlog::error("[{} : {}] config text table update failed [{}]", __func__, __LINE__, rc);
		return FLK_FAIL;
	}
	return FLK_SUCCESS;
}
