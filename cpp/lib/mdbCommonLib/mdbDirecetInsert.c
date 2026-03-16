/**
 @ingroup mdbCommonLib
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
******************************************************************************

*******************************************************************************
*   FILE NAME       :   directInsert.c
*   PURPOSE         :
*   DATE OF WRITE   :   2002.03.01
*   AUTHOR          :
*   REVISION        :   V 1.0
*   Ver.    By      When        Reason
*   ------- ------- ----------  -----------------------------------------
*   1.0             2002.03.01  first coding
******************************************************************************/

#include <spdlog/spdlog.h>
#include <MdbTableInfo.h>
#include <EMacro.h>
#include <MdbCommon.h>
#include <MdbMessage.h>
#include <MdbDmaqi.h>
#include <DbAqi.h>
#include <Debug.h>
#include <FLKBaseDef.h>

#include "EIpc.h"

static int segmentId, segmentOffset;

extern sttAqiInfo *gpAqiInfo[MAX_DBMS_CNT];
extern Element *dma_get_bucket_with_key();

static void dma_int2charStr(char *charStr, int integer)
{
	/* Sometime,we read data as integer ,and sometimes as string character
	   This is very good library which uses union structure efficiently  */
	union {
		int	integer;
		char	str[S_INT];
	} data;

	data.integer = integer;

	for (int i = 0; i < (int)S_INT; i++) {
		*charStr = data.str[i];
		charStr++;
	}
}


static int dma_allocTupleSpace(sttAqiInfo *pAqiInfo, int tableId)
{
//	Rdir	 *rdir= pAqiInfo->rdirPtr[tableId];
	int		 freeTuple, tupId;
	char	 *ptr;
//	char	 *cpTmp;
//	int 	 nLoop;

	if ((freeTuple = (int)(pAqiInfo->rdirPtr[tableId]->firstFree)) < 0) {
		settingErrorCode(NO_MORE_FREE_TUPLE);
		spdlog::error("NO_MORE_FREE_TUPLE[{}]", freeTuple);
		return FLK_FAIL;
	}

	if ((ptr = dma_tupleId2ptr(pAqiInfo->rdirPtr, tableId, freeTuple)) == NULL) {
		return FLK_FAIL;
	}
	if ((tupId = dma_charStr2int(ptr)) == FILLED_TUPLE_SPACE) {
		spdlog::error("[{}] tupId = [{}]", __func__, tupId);
		return FLK_FAIL;
	}

	(void)memset((char *)(ptr + S_INT), 0xfe,
	             pAqiInfo->rdirPtr[tableId]->tupleSize);

	pAqiInfo->rdirPtr[tableId]->firstFree = tupId;
	pAqiInfo->rdirPtr[tableId]->used_tup =
	    pAqiInfo->rdirPtr[tableId]->used_tup  + 1;

	(void)dma_int2charStr(ptr, FILLED_TUPLE_SPACE);
	/*  -2 -> ff ff ff fe  */
	dma_dirtyFlag_set(pAqiInfo, (char *)pAqiInfo->rdirPtr[tableId], sizeof(Rdir));
	return freeTuple;
}

static int dma_table_insert(int nDbmsType, int tableId, int *pTupId, char **ppTupVal, char *tupVal)
{
	sttAqiInfo *pAqiInfo = gpAqiInfo[nDbmsType];

	if ((*pTupId = dma_allocTupleSpace(pAqiInfo, tableId)) < 0) {
		settingErrorCode(ALLOC_TUPLE_SPACE_FAIL);
		return FLK_FAIL;
	}
	if ((*ppTupVal = dma_tupleId2tuplePtr(nDbmsType, tableId, *pTupId)) == NULL) {
		settingErrorCode(ADDR_TO_TUPLE_PTR_FAIL);
		return FLK_FAIL;
	}
	(void)memcpy((char *)*ppTupVal, tupVal,
	             pAqiInfo->relationDir[tableId]->tupleSize);
	dma_dirtyFlag_set(pAqiInfo,
	                  dma_tupleId2ptr(pAqiInfo->rdirPtr, tableId, *pTupId),
	                  (int)(pAqiInfo->relationDir[tableId]->tupleSize + S_INT));

	return FLK_SUCCESS;
}


static int dma_freeTupleSpace(sttAqiInfo *pAqiInfo, int tableId, int tupId)
{
//	Rdir	 *rdir = pAqiInfo->rdirPtr[tableId];
	char	 *ptr;

	if ((ptr = dma_tupleId2ptr(pAqiInfo->rdirPtr, tableId, tupId)) == NULL) {
		return FLK_FAIL;
	}
	if (dma_charStr2int(ptr) != FILLED_TUPLE_SPACE) {
		return FLK_FAIL;
	}

	/* calculate the pointer address using 'pRelDir->firstFree' and set "0xfe".
	   'pRelDir->firstFree' is integer variable for easy recognition.
	   But,now we need memory address for memset operation.  */

	(void)dma_int2charStr(ptr, (int)(pAqiInfo->rdirPtr[tableId]->firstFree));
	(void)memset((char *)(ptr + S_INT), 0xfe,
	             pAqiInfo->rdirPtr[tableId]->tupleSize);

	/* put the deleted record on the head of free list ,
	   and decrease storedTupleCnt and used_tup  */

	pAqiInfo->rdirPtr[tableId]->firstFree = tupId;
	pAqiInfo->rdirPtr[tableId]->used_tup--;
	dma_dirtyFlag_set(pAqiInfo, (char *)pAqiInfo->rdirPtr[tableId],
	                  sizeof(Rdir));

	return FLK_SUCCESS;
}


static int dma_table_delete(int nDbmsType, int tableId, int tupId)
{
	sttAqiInfo *pAqiInfo = gpAqiInfo[nDbmsType];

	if (dma_freeTupleSpace(pAqiInfo, tableId, tupId) < 0) {
		return FLK_FAIL;
	}
	dma_dirtyFlag_set(pAqiInfo,
	                  dma_tupleId2ptr(pAqiInfo->rdirPtr, tableId, tupId),
	                  (int)(pAqiInfo->relationDir[tableId]->tupleSize + (int)S_INT));

	return FLK_SUCCESS;
}



/*---------------------------------------------------------------------
 *  FUNCTION NAME   :   dma_hash_insert()
 *  PURPOSE         :
 *  INPUT ARGUMENTS :
 *  RETURN VALUE    :
 *  CALLED FROM     :
 *  USED GLOBAL VARIABLES :
 *  REVISION        :
 *      BY              WHEN        REASON
 *      --------------  ----------  -------------------
 *      molee           98.05.05      First Coding
----------------------------------------------------------------------*/
static int dma_hash_insert(sttAqiInfo *pAqiInfo, int indexId, char *key, int tupId)
{
	Index	*pIndex;
	Index_header *indexHead;
	Element *pNewElm, *pElm, *ppElm, *fElm;
	int     keyLen; //, range;
	int		hashValue, segmentId, segmentOffset;

	if ((pIndex = (Index *)pAqiInfo->indexDir[indexId]) == NULL) {
		return FLK_FAIL;
	}

	/* Increase number of elements */
	/*
	if (pIndex->numElem >= DIRECTORY_SIZE*SEGMENT_SIZE*MAX_LOAD_FACTOR)
	{
		return FLK_FAIL;
	}
	*/

	indexHead = (Index_header *)pAqiInfo->index_ptr;

	keyLen = pIndex->keyLength;
	if (keyLen <= 0) {
		spdlog::error("[{}][{}]INVALID_INDEX_ID", __FILE__, __LINE__);
		return  FLK_FAIL;
	}
	/* Get bucket and check duplicated key */
	if ((ppElm = dma_get_bucket_with_key(pAqiInfo, indexId, key)) != NULL) {
		pElm = (Element *)ppElm;
		fElm = (Element *)ppElm;

		int nHashCnt = 0;
		char	cpKeyNull[MAX_HASH_KEY_SIZE];
		bzero(cpKeyNull, sizeof(cpKeyNull));
		//while (pElm!=NULL && pElm->key!=NULL)
		while (pElm != NULL && (0 != memcmp(pElm->key, cpKeyNull, keyLen)) &&
		        (nHashCnt++ < pIndex->numElem)) {
			if (!dma_compare(key, pElm->key, keyLen)) {
				return -ML_HASH_INSERT_EXIST;
			}
			pElm = pElm->next;

			if (pElm == NULL) {
				continue;
			}
			if (fElm == pElm->next) {
				/* last element's next ==  next */
				pElm->next = NULL;
				if (!dma_compare(key, pElm->key, keyLen)) {
					return -ML_HASH_INSERT_EXIST;
				} else {
					break;
				}
			}/* if */
		}/* while */
	}/* if */

	M_SemaP(pAqiInfo->semId4IDX.nSemId, 0);
	if (indexHead->freeElement.next == NULL) {
		DLOG(("[%s:%d] fElm[%p] numElem[%d]"
		      "freeElement.next[%p] pElm[%p]", __FILE__, __LINE__,
		      fElm, pIndex->numElem, indexHead->freeElement.next, pElm));
		M_SemaV(pAqiInfo->semId4IDX.nSemId, 0);
		return FLK_FAIL;
	}
	pNewElm = (Element *)indexHead->freeElement.next;
	indexHead->freeElement.next = (Element *)pNewElm->next;
	indexHead->TotFreeNumElement--;
	pNewElm->next = NULL;

	memset(pNewElm->key, 0xff, keyLen);
	memcpy(pNewElm->key, key, keyLen);

	pNewElm->tupId = tupId;

	pNewElm->next = ppElm;

	ppElm = (Element *)pNewElm;

//	range    = pIndex->hashRange;
	hashValue = (unsigned)dma_hash1(key, keyLen);
	segmentId     = hashValue / SEGMENT_SIZE;
	segmentOffset = hashValue % SEGMENT_SIZE;

	pIndex->dir[segmentId][segmentOffset] = (Element *)ppElm;

	pIndex->numElem++;
	DLOG(("[%s:%d] segmentId[%d] segmentOffset[%d] ppElm[%p] ppElm->next[%p] numElem[%d]"
	      "freeElement.next[%p] pElm[%p] tupId[%d]", __FILE__, __LINE__,
	      segmentId, segmentOffset, ppElm, ppElm->next, pIndex->numElem,
	      indexHead->freeElement.next, pElm, pNewElm->tupId));
	M_SemaV(pAqiInfo->semId4IDX.nSemId, 0);
	return FLK_SUCCESS;
}


/*---------------------------------------------------------------------
 *  FUNCTION NAME   :   dma_hash_delete()
 *  PURPOSE         :
 *  INPUT ARGUMENTS :
 *  RETURN VALUE    :
 *  CALLED FROM     :
 *  USED GLOBAL VARIABLES :
 *  REVISION        :
 *      BY              WHEN        REASON
 *      --------------  ----------  -------------------
 *      molee           98.05.05      First Coding
----------------------------------------------------------------------*/
static int dma_hash_delete(sttAqiInfo *pAqiInfo, int indexId, char *key)
{
	Element *pElm, *ppElm, *hpElm, *fElm;
	Index   *pIndex;
	Index_header *indexHead;
	int 	keyLen;

	if ((pIndex = (Index *)pAqiInfo->indexDir[indexId]) == NULL) {
		spdlog::error("[{}][{}]Invalid Hash ID", __FILE__, __LINE__);
		return FLK_FAIL;
	}

	indexHead = (Index_header *)pAqiInfo->index_ptr;

	keyLen = pIndex->keyLength;
	if (keyLen <= 0) {
		spdlog::error("[{}][{}]INVALID_INDEX_ID", __FILE__, __LINE__);
		return  FLK_FAIL;
	}

	if ((ppElm = dma_get_bucket_with_key(pAqiInfo, indexId, key)) == NULL) {
		return FLK_FAIL;
	} else {
		pElm = (Element *)ppElm;
		hpElm = (Element *)ppElm;
		fElm = (Element *)ppElm;

		char	cpKeyNull[MAX_HASH_KEY_SIZE];
		bzero(cpKeyNull, sizeof(cpKeyNull));
		int 	nHashCnt = 0;
		while (pElm != NULL && (0 != memcmp(pElm->key, cpKeyNull, keyLen)) &&
		        (nHashCnt++ < pIndex->numElem)) {
			if (!dma_compare(key, pElm->key, keyLen)) {
				M_SemaP(pAqiInfo->semId4IDX.nSemId, 0);
				ppElm = pElm->next;
				pElm->tupId = -1; /* NULL_ID */
				memset(pElm->key, '\0', sizeof(pElm->key));

				if (hpElm == pElm) {
					fElm = ppElm;
				} else {
					hpElm->next = ppElm;
				}
				int hashValue = (unsigned)dma_hash1(key, keyLen);
				segmentId     = hashValue / SEGMENT_SIZE;
				segmentOffset = hashValue % SEGMENT_SIZE;
				pIndex->dir[segmentId][segmentOffset] = fElm;
				pIndex->numElem--;
				pElm->next = (Element *)indexHead->freeElement.next;
				indexHead->freeElement.next = (Element *)pElm;
				indexHead->TotFreeNumElement++;

				/* Decrease number of elements */
				if (pIndex->numElem < 0) {
					pIndex->numElem = 0;
					DLOG(("[%s:%d] segmentId[%d] segmentOffset[%d] fElm[%p] numElem[%d]"
					      "freeElement.next[%p] pElm[%p] pElm->next[%p]", __FILE__, __LINE__,
					      segmentId, segmentOffset, fElm, pIndex->numElem,
					      indexHead->freeElement.next, pElm, pElm->next));
					M_SemaV(pAqiInfo->semId4IDX.nSemId, 0);
					return FLK_FAIL;
				}
				DLOG(("[%s:%d] segmentId[%d] segmentOffset[%d] fElm[%p] numElem[%d]"
				      "freeElement.next[%p] pElm[%p] pElm->next[%p]", __FILE__, __LINE__,
				      segmentId, segmentOffset, fElm, pIndex->numElem,
				      indexHead->freeElement.next, pElm, pElm->next));
				M_SemaV(pAqiInfo->semId4IDX.nSemId, 0);
				return 1;
			}
			hpElm = pElm;
			ppElm = pElm->next;
			pElm = pElm->next;
		}
		settingErrorCode(KEY_NOT_EXIST);
		return FLK_FAIL;
	}
//	return FLK_SUCCESS;
}


/*---------------------------------------------------------------------
 *  FUNCTION NAME   : mdb_dma_insert();
 *  PURPOSE         : to provide other with select operation
					  generalize input argument
 *  INPUT ARGUMENTS : select struct/ack struct/mode
 *  RETURN VALUE    :
						Refer db_message.h.
 *  CALLED FROM     :
 *  USED GLOBAL VARIABLES :
 *    1) EXTERNAL
 *    2) LOCAL
 *  REVISION        :
 *      BY              WHEN        REASON
 *      --------------  ----------  -------------------
 *      molee           98.05.05      First Coding
----------------------------------------------------------------------*/
static int mdb_dma_insert(int nDbmsHandle, Insert *insert, Insert_Ack *ack)
{
	char	*noAlign, *insTupPtr, rmAlignIns[MAX_TUPLE_SIZE];
	int		tableId, ix, rstno = 0;
//	int		nLoop;

	sttAqiInfo *pAqiInfo;

	pAqiInfo = gpAqiInfo[nDbmsHandle];

	tableId = insert->table_id;

	/* Verification of query conditions */
	if (tableId < 0 || tableId >= pAqiInfo->tableCnt || MAX_TABLE <= tableId) {
		return -DMAQI_DB_BAD_TABLE_ID;
	}

	if (insert->tuple == NULL) {
		return -DB_PUT_TUP__TUP_VAL__NULL;
	}

	/* Removing non used space which is to align data */
	if (pAqiInfo->needAlign[tableId]) {
		dma_align_remove(nDbmsHandle, tableId, rmAlignIns, insert->tuple);
		insTupPtr = rmAlignIns;
	} else {
		insTupPtr = insert->tuple;
	}

	if (dma_table_insert(nDbmsHandle, tableId, &(ack->tup_id), &noAlign, insTupPtr) < 0) {
		spdlog::error("[{}][{}]table insert fail[{}].", __FILE__, __LINE__, dbErrorCode);
		return FLK_FAIL;
	}

	/* Insert  key value into index */
	for (ix = 0; ix < pAqiInfo->totAtt[tableId]; ix++) {
		if (pAqiInfo->Hash_id[tableId][ix] >= 0) {
			if ((rstno = dma_hash_insert(pAqiInfo,
			                             pAqiInfo->Hash_id[tableId][ix],
			                             ATTR_PTR(pAqiInfo->attDir, noAlign, tableId, ix),
			                             ack->tup_id)) < 0) {
				if (tableId == 0 && ix == 1) {
					if (dma_hash_delete(pAqiInfo,
					                    pAqiInfo->Hash_id[tableId][ix - 1],
					                    ATTR_PTR(pAqiInfo->attDir, noAlign, tableId, ix - 1)) < 0) {
						spdlog::error("[{}][{}]hash delete fail.", __FILE__, __LINE__);
						return FLK_FAIL;
					}
				}

				(void)dma_table_delete(nDbmsHandle, tableId, ack->tup_id);
				return FLK_FAIL;
			}
		}
	}

	return FLK_SUCCESS;
}

/*---------------------------------------------------------------------
 *  FUNCTION NAME   : mdb_dma_delete();
 *  PURPOSE         : to provide other with select operation
					  generalize input argument
 *  INPUT ARGUMENTS : select struct/ack struct/mode
 *  RETURN VALUE    :
						Refer db_message.h.
 *  CALLED FROM     :
 *  USED GLOBAL VARIABLES :
 *    1) EXTERNAL
 *    2) LOCAL
 *  REVISION        :
 *      BY              WHEN        REASON
 *      --------------  ----------  -------------------
 *      molee           98.05.05      First Coding
----------------------------------------------------------------------*/
static int mdb_dma_delete(int nDbmsHandle, Delete *del, Delete_Ack *ack, int mode)
{
	char	*noAlign;
//	char    cRecord[MAX_TUPLE_SIZE];
//	int		sel_type = RETR_EQ;
	int		tableId, attId, tupId, ix;

	sttAqiInfo *pAqiInfo;

	pAqiInfo = gpAqiInfo[nDbmsHandle];

	tableId = del->table_id;
	attId  = del->att_id;
	tupId  = del->tup_id;

	/* Verification of query conditions */
	if (tableId < 0 || tableId >= pAqiInfo->tableCnt) {
		return -DMAQI_DB_BAD_TABLE_ID;
	}
	if (tupId < 0 || tupId > pAqiInfo->relationDir[tableId]->tupleSpaceCnt) {
		return -DMAQI_DB_BAD_TUPLE_ID;
	}

	/* Searching target tuple */
	if (mode == DELETE_SEQ) {
		while (dma_table_retr(nDbmsHandle, tableId, tupId, &noAlign) < 0) {
			if (++tupId >= pAqiInfo->relationDir[tableId]->tupleSpaceCnt ) {
				return -DMAQI_THERE_IS_NOT_THE_TUPLE;
			}
		}
		ack->tup_id = tupId;
	} else {
		if (mode == DELETE_EX1) { /* greater */
//			sel_type    =   RETR_GREAT;
		} else if (mode == DELETE_EX2) { /* less */
//			sel_type    =   RETR_LESS;
		} else {
//			sel_type    =   RETR_EQ;
		}

		if (attId < 0 || attId >= pAqiInfo->totAtt[tableId] ||
		        MAX_ATT_PER_TABLE <= attId) {
			return -DB_BAD_ATTRIBUTE_ID;
		}
		if (del->att_val == NULL) {
			return -DB_ATT_VAL__NULL;
		}
		if ((ack->tup_id = dma_getTupId(nDbmsHandle, tableId, attId, del->att_val, tupId)) < 0) {
			return -DMAQI_THERE_IS_NOT_THE_TUPLE;
		}
		if (dma_table_retr(nDbmsHandle, tableId, ack->tup_id, &noAlign) < 0) {
			return -TABLE_RETRIEVAL_FAIL;
		}
	}

	/* Delete index value related with tuple value to be deleted */
	for (ix = 0; ix < pAqiInfo->totAtt[tableId]; ix++) {
		if (pAqiInfo->Hash_id[tableId][ix] >= 0) {
			if (dma_hash_delete(pAqiInfo, pAqiInfo->Hash_id[tableId][ix],
			                    ATTR_PTR(pAqiInfo->attDir, noAlign, tableId, ix)) < 0) {
				if (tableId == 0 && ix == 1) {
					if (dma_hash_insert(pAqiInfo,
					                    pAqiInfo->Hash_id[tableId][ix - 1],
					                    ATTR_PTR(pAqiInfo->attDir, noAlign, tableId, ix - 1),
					                    ack->tup_id) < 0) {
						return FLK_FAIL;
					}
				}
				spdlog::error("dma_hash_delete() fail.");
				return FLK_FAIL;
			}
		}

	}

	/* Delete tuple value */
	if (dma_table_delete(nDbmsHandle, tableId, ack->tup_id) < 0) {
		spdlog::error("[{}][{}] dma_table_delete() fail.", __func__, __LINE__);
		return FLK_FAIL;
	}
	return FLK_SUCCESS;
}


/*---------------------------------------------------------------------
 *  FUNCTION NAME   : mdb_delete_direct();
 *  PURPOSE         :
 *  INPUT ARGUMENTS :
 *  RETURN VALUE    :
 *  CALLED FROM     :
 *  USED GLOBAL VARIABLES :
 *    1) EXTERNAL
 *    2) LOCAL
 *  REVISION        :
 *      BY              WHEN        REASON
 *      --------------  ----------  -------------------
 *      				98.05.05      First Coding
----------------------------------------------------------------------*/
int mdb_delete_direct(int nDbmsType, int table_id, int attId, int tupId, char *attVal, int attVal_size)
{
	Delete      stDelete;
	Delete_Ack  stDeleteAck;

	if (nDbmsType >= MAX_DBMS_CNT || nDbmsType < 0) {
		spdlog::error("Invalid DBMS Type.[{}]", __func__);
		return FLK_FAIL;
	}

	if (gpAqiInfo[nDbmsType] == NULL) {
		spdlog::error("[{}] - Didn't connect MDB DMA[{}]!!",
		       __func__, nDbmsType);
		return FLK_FAIL;
	}

	if ( 0 > table_id || MAX_TABLE <= table_id) {
		return -QP_INVALID_TABLE_ID;
	}

	if (tupId < 0 ) {
		return -DMAQI_DB_BAD_TUPLE_ID;
	}

	if (attId < 0 || MAX_ATT_PER_TABLE <= attId) {
		return -DB_BAD_ATTRIBUTE_ID;
	}

	if ( 0 > attVal_size || MAX_ATT_PER_QP <= attVal_size) {
		return -DB_ATTRIBUTE_VAL_NULL;
	}

	bzero(&stDelete, sizeof(stDelete));
	bzero(&stDeleteAck, sizeof(stDeleteAck));

	stDelete.table_id = table_id;
	stDelete.att_id = attId;
	stDelete.tup_id = tupId;

	memcpy(stDelete.att_val, attVal, attVal_size);

	return (mdb_dma_delete(nDbmsType, &stDelete, &stDeleteAck, DELETE));
}

/*---------------------------------------------------------------------
 *  FUNCTION NAME   : mdb_insert_direct();
 *  PURPOSE         :
 *  INPUT ARGUMENTS :
 *  RETURN VALUE    :
 *  CALLED FROM     :
 *  USED GLOBAL VARIABLES :
 *    1) EXTERNAL
 *    2) LOCAL
 *  REVISION        :
 *      BY              WHEN        REASON
 *      --------------  ----------  -------------------
 *      				98.05.05      First Coding
----------------------------------------------------------------------*/
int mdb_insert_direct(int nDbmsType, int table_id, void *tuple, size_t tuple_size)
{
	if (nDbmsType >= MAX_DBMS_CNT || nDbmsType < 0) {
		spdlog::error("Invalid DBMS Type.[{}]", __func__);
		return FLK_FAIL;
	}

	if (gpAqiInfo[nDbmsType] == NULL) {
		spdlog::error("[{}] - Didn't connect MDB DMA[{}]!!",
		       __func__, nDbmsType);
		return FLK_FAIL;
	}

	if (table_id < 0 || table_id >= MAX_TABLE) {
		return -QP_INVALID_TABLE_ID;
	}

	if (tuple == (void *) NULL) {
		return -QP_TUPLE_NULL_POINTER;
	}

	Insert  insert;
	bzero(&insert, sizeof(Insert));
	insert.table_id = table_id;
	memcpy(insert.tuple, (char *)tuple, tuple_size);

	Insert_Ack ack;
	return (mdb_dma_insert(nDbmsType, &insert, &ack));
}
/** @} */
