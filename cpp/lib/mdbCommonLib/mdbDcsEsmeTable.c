/**
 * =====================================================================================
 *
 *       @file  mdbEsmeDcs.c
 *
 *    	 @brief
 *
 *       Version:  1.0
 *       Compiler:  cc (HP-UX)
 *       @date:  08/29/11 01:44:03
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
 *     7F FEELingK Tower 1, Toungdeungpo-Dong 3-Ga, Youngdeungpo-Gu, Seoul 150-033 Korea
 *
 * =====================================================================================
 */

/*
#include <spdlog/spdlog.h>
#include "FLKBaseDef.h"
#include "MdbTableInfo.h"
#include "MdbDmaqi.h"
#include "MdbMessage.h"

// select esme_id// //FR320 LSH
int selectCharConvert(u_char ucEsmeId, stEsmeDcsSvcTuple * spEsmeDcsSvc)
{
	stEsmeDcsSvcTuple stEsmeDcsSvc;
	char attVal[DB_ESME_DCS_SVC_ID_SIZE];
	char *noAlign = NULL;
	int nTableId = 0;
	int nTupId = 0;
	int nAttId = 0;

	memset(attVal, 0x00, DB_ESME_DCS_SVC_ID_SIZE);
	memset(&stEsmeDcsSvc, 0x00, sizeof(stEsmeDcsSvcTuple));

	nTableId = R_DCHARCONVERT;
	nTupId = 0;
	nAttId = DB_ESME_DCS_SVC_ID;

	memcpy(attVal,&ucEsmeId, DB_CHAR_CONVERT_ESME_ID_SIZE);

	if((nTupId = dma_getTupId(DBMS_PLUS, nTableId, nAttId,(char*)attVal, nTupId)) < 0)
	{
		return -DMAQI_THERE_IS_NOT_THE_TUPLE;
	}
	if(dma_table_retr(DBMS_PLUS, nTableId, nTupId, &noAlign) < 0)
    {
        return -DMAQI_DB_BAD_TUPLE_ID;
    }
	(void)dma_align_make(DBMS_PLUS, nTableId, (char *)&stCharConvert, noAlign);
	memcpy(spCharConvert,&stCharConvert,sizeof(stCharConvertTuple));

	return FLK_SUCCESS;
}

// check convert for esme_id// //FR320 LSH
bool checkCharConvert(u_char ucEsmeId)
{
	stCharConvertTuple stCharConvert;
	stCharConvertType *spCharConvert;

	if(selectCharConvert(ucEsmeId, &stCharConvert) == FLK_FAIL)
	{
		return false;
	}

	spCharConvert = (stCharConvertType *)&stCharConvert;

	if( spCharConvert->nCharConvertOnOff == 1)
	{
		return true;
	}
	return false;
}

// insert esme_id and charconvert on/off// // FR320 LSH
int
insertCharConvertInfo(u_char  ucEsmeId, u_char ucSvcOnOff)
{
	stCharConvertTuple *spCharConvertData;

	int     nTableId =0;
	int     nDbmsHandle = 0;
	int     nResult = 0;
	Result stResult;
	bzero(&stResult, sizeof(stResult));

	nDbmsHandle = DBMS_PLUS;
	nTableId = R_DCHARCONVERT;
    memset(&spCharConvertData,0x00,sizeof(stCharConvertTuple));
	memcpy(spCharConvertData->dbCharEsmeId, &ucEsmeId,sizeof(ucEsmeId));
	memcpy(spCharConvertData->dbCharConverSvcOnOff,&ucSvcOnOff,sizeof(ucSvcOnOff));

	if ((nResult = db_insert(nDbmsHandle, nTableId,(void *)spCharConvertData, &stResult, NO_RESULT)) != 1)
	{
		db_error(nResult);
		spdlog::error("[{}:{}]-db_insert fail[{}] !", __func__, __LINE__, nResult);
		return FLK_FAIL;
	}
	return FLK_SUCCESS;
}
// delete charconvert data// //FR320
int deleteCharConvertData(u_char unEsmeId)
{
	int nRc = 0;
	int nFlag = NO_RESULT;
	int nTupId = 0;
	int nTableId = 0;
	int nAttId = 0;
	Result  stResult;

    nTableId = R_DCHARCONVERT;
	nAttId = DB_CHAR_CONVERT_ESME_ID;
	nRc = db_dml(DBMS_PLUS,nTableId ,nAttId,(char *)&unEsmeId, DB_CHAR_CONVERT_ESME_ID_SIZE, 0, &stResult, nFlag, DELETE);

	if ( nRc != 1 ) {
		spdlog::error("[{} : {}] charconvert table update failed [{}]", __func__, __LINE__, nRc);
		return FLK_FAIL;
	}
    return FLK_SUCCESS;
}

//update charconvert on/off update use for esme_id
int updateEmseClientInfo(u_char ucEsmeId, u_char ucSvcOnOff)
{
	stCharConvertTuple *stEsmeId;
	Result  stResult;
	int     nTableId = 0;
	int     nDbmsHandle = 0;
	int     nAttId = 0;
	int     nFalg = NO_RESULT;
	int     nTupId = 0;
	char 		attVal[DB_CHAR_CONVERT_ESME_ID_SIZE];
	char      *noAlign = NULL;

	nDbmsHandle = DBMS_PLUS;
	nTableId = R_DCHARCONVERT;
	nAttId = DB_CHAR_CONVERT_ONOFF;
	memset(attVal, 0x00, DB_CHAR_CONVERT_ESME_ID_SIZE);
	memcpy(attVal,&ucEsmeId, DB_CHAR_CONVERT_ESME_ID_SIZE);

	if((nTupId = dma_getTupId(DBMS_PLUS, nTableId, nAttId,(char*)attVal, nTupId)) < 0)
	{
		return -DMAQI_THERE_IS_NOT_THE_TUPLE;
	}
	if(dma_table_retr(DBMS_PLUS, nTableId, nTupId, &noAlign) < 0)
	{
		return -DMAQI_DB_BAD_TUPLE_ID;
	}
	stEsmeId = (stCharConvertTuple *)noAlign;
	memcpy(stEsmeId->dbCharConverSvcOnOff,&ucSvcOnOff,sizeof(ucSvcOnOff));
	return FLK_SUCCESS;
}*/
/** @} */
