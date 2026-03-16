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
*   FILE NAME       :   mdbDmaqiEx.c
*   PURPOSE         :
*   DATE OF WRITE   :   2002.03.01
*   AUTHOR          :
*   REVISION        :   V 1.0
*   Ver.    By      When        Reason
*   ------- ------- ----------  -----------------------------------------
*   1.0             2002.03.01  first coding
******************************************************************************/

#include <errno.h>
#include <spdlog/spdlog.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/sem.h>

#include <MdbCommon.h>
#include <MdbDataStruct.h>

#include <DbAqi.h>
#include <Debug.h>
#include <EMacro.h>
#include <MdbDmaqi.h>
#include <MdbMessage.h>
#include <MdbTableInfo.h>

#include <SmsIpc.h>

#define INSERTDBMS_RETRY_MAXNUM 10

// Must synchronize with dbms/main.c
static const int DBMS_SEM_MAX[MAX_DBMS_CNT][MAX_TABLE] = {
    {16, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    {10, 10, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    {10, 10, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    {10, 10, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    {10, 10, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}};

static const int DBMS_SEM_IDX[MAX_DBMS_CNT] = {STATDB_SEMDBIDX, SEMDBIDX,
                                               RECTDB_SEMDBIDX, PLUS_SEMDBIDX,
                                               TRANDB_SEMDBIDX, RCS_SEMDBIDX};

static void dma_set_meta_database(sttAqiInfo *pAqiInfo);

sttAqiInfo *gpAqiInfo[MAX_DBMS_CNT] = {NULL, NULL, NULL, NULL, NULL, NULL};

extern char *dmaqi_ErrString[MAX_DMAQI_ERR];
static void dma_set_meta_index(sttAqiInfo *pAqiInfo);
static void dma_align_init(sttAqiInfo *pAqiInfo);
/*---------------------------------------------------------------------
 *  FUNCTION NAME   : disconnectDbDmaqi()
 *  PURPOSE         : to disconnect from mdb.
 *  INPUT ARGUMENTS :
 *  RETURN VALUE    : success/fail
 *  CALLED FROM     :
 *  USED GLOBAL VARIABLES :
 *  REVISION        :
 *      BY              WHEN        REASON
 *      --------------  ----------  -------------------
 *      molee           98.06.      First Coding
----------------------------------------------------------------------*/
int disconnectDbDmaqi(int nDbmsHandle) {
  sttAqiInfo *pAqiInfo = gpAqiInfo[nDbmsHandle];

  if (pAqiInfo == NULL) {
    spdlog::error("[{}] can't connect MDB DMA[{}]", __func__, nDbmsHandle);
    return FLK_FAIL;
  }

  if (shmdt(pAqiInfo->table_ptr) < 0) {
    spdlog::error("[{}] shmdt(table_ptr) fail: {}", __func__, strerror(errno));
    return -DMAQI_DB_TABLE_DETACH_ERR;
  }
  if (shmdt(pAqiInfo->index_ptr) < 0) {
    spdlog::error("[{}] shmdt(index_ptr) fail: {}", __func__, strerror(errno));
    return -DMAQI_INDEX_TABLE_DETACH_ERR;
  }
  if (shmdt(pAqiInfo->pDirtyFlagTable) < 0) {
    spdlog::error("[{}] shmdt(pDirtyFlagTable) fail: {}", __func__,
                  strerror(errno));
    return -DMAQI_DIRTY_TABLE_DETACH_ERR;
  }

  return FLK_SUCCESS;
}

/*---------------------------------------------------------------------
 *  FUNCTION NAME   : connectDbDmaqi()
 *  PURPOSE         : to connect mdb.
 *  INPUT ARGUMENTS : DBMS_STAT, DBMS_ACTIVE, DBMS_RECEIPT, DBMS_PLUS
 *  RETURN VALUE    : success/fail
 *  CALLED FROM     :
 *  USED GLOBAL VARIABLES :
 *  REVISION        :
 *      BY              WHEN        REASON
 *      --------------  ----------  -------------------
 *		molee			98.05		first coding.
 *-----------------------------------------------------------------------*/
// added by smyang for STAT DB usging Libary
int InitDbDmaqi4Stat(sttAqiInfo *pAqiInfo) {
  if (gpAqiInfo[DBMS_STAT] == NULL) {
    gpAqiInfo[DBMS_STAT] = pAqiInfo;
  }
  return FLK_SUCCESS;
}

static int TupleShmId[MAX_TABLE];

int dma_mdb_alloc_tuple(int shm_key, DB_header *header, char **tuple_ptr) {
  int ShmTupleBase = shm_key | 0xA0000000;

  Rdir *pRdir = (Rdir *)header->rdir_adr;

  for (int idx = 0; idx < header->no_tup_rdir; ++idx) {
    int ShmTupleKey = ShmTupleBase | idx;

    if ((TupleShmId[idx] = shmget(ShmTupleKey, 0, 0)) < 0) {
      // SR211 pkg
      spdlog::error("[{}:{}] shmget fail pid[{}] errno[{}] err[{}]", __func__,
                    __LINE__, getpid(), errno, strerror(errno));
      return -SHARED_DB_ID_GET_FAIL;
    }

    // SR210 pkg
    // if ((tuple_ptr[idx]=(char *)shmat(TupleShmId[idx], (void
    // *)pRdir[idx].rel_adr, 0)) == NULL)
    if ((tuple_ptr[idx] = (char *)shmat(
             TupleShmId[idx], (void *)pRdir[idx].rel_adr, 0)) == (char *)-1L) {
      // SR211 pkg
      spdlog::error("[{}:{}] shmat fail pid[{}] errno[{}] err[{}]", __func__,
                    __LINE__, getpid(), errno, strerror(errno));
      return -SHARED_DB_ATTACH_FAIL;
    }
  }
  return FLK_SUCCESS;
}

static const int nBaseKey[MAX_DBMS_CNT] = {STATDB_BASE_KEY, ACTVDB_BASE_KEY,
                                           RECTDB_BASE_KEY, PLUSDB_BASE_KEY,
                                           TRANDB_BASE_KEY, RCSADB_BASE_KEY};

int connectDbDmaqi(int dbms_type) {
  if (dbms_type >= MAX_DBMS_CNT || dbms_type < 0) {
    spdlog::error("Invalid DBMS Type.[{}]", __func__);
    return FLK_FAIL;
  }

  //////////////////////////////////////////////////////
  // get dbms key
  //////////////////////////////////////////////////////
  //	int nDbmsHandle = dbms_type;

  if (gpAqiInfo[dbms_type] != NULL) {
    return FLK_SUCCESS;
  } else {
    gpAqiInfo[dbms_type] = (sttAqiInfo *)malloc(sizeof(sttAqiInfo));
    memset(gpAqiInfo[dbms_type], 0x00, sizeof(sttAqiInfo));
  }
  //	setting pAqiInfo;
  //////////////////////////////////////////////////////

  sttAqiInfo *pAqiInfo = gpAqiInfo[dbms_type];

  sttDBMSKey stDBMSKey{};
  stDBMSKey.SMKeyDBMS = SHMDBMS + nBaseKey[dbms_type];
  stDBMSKey.SMKeyDBKEY = SHMDBKEY + nBaseKey[dbms_type];
  stDBMSKey.SMKeyINDEX = SHMINDEX + nBaseKey[dbms_type];

  key_t semKeyBase = stDBMSKey.SMKeySemBase = SHMDBKEY + nBaseKey[dbms_type];

  DBMS_ADDR_ST dbms_addr;
  if (read_dbms_addr(dbms_type, &dbms_addr) == FLK_FAIL) {
    spdlog::error("ERROR : read_dbms_addr failed");
    return FLK_FAIL;
  }

  /* shared memeory get and attach : DB(SLD) Table */
  int shm_id = shmget(stDBMSKey.SMKeyDBMS, 0, 0);
  if (shm_id < 0) {
    // SR211 pkg
    spdlog::error("[{}:{}] shmget fail pid[{}] errno[{}] strerr[{}]", __func__,
                  __LINE__, getpid(), errno, strerror(errno));
    return -SHARED_DB_ID_GET_FAIL;
  }

  // SR210 pkg
  // if ((pAqiInfo->table_ptr=(char *)shmat(shm_id, (void *)dbms_addr.dbms_adr,
  // 0)) == NULL)
  if ((pAqiInfo->table_ptr = (char *)shmat(shm_id, (void *)dbms_addr.dbms_adr,
                                           0)) == (char *)-1L) {
    // SR211 pkg
    spdlog::error("[{}:{}] shmat fail pid[{}] errno[{}] strerr[{}]", __func__,
                  __LINE__, getpid(), errno, strerror(errno));
    return -SHARED_DB_ATTACH_FAIL;
  }

  /* shared memory get and attach : DB Tuple */
  int rc =
      dma_mdb_alloc_tuple(stDBMSKey.SMKeyDBMS, (DB_header *)pAqiInfo->table_ptr,
                          pAqiInfo->tuple_ptr);
  if (rc != FLK_SUCCESS) {
    return rc;
  }

  /* shared memeory get and attach : Dirty Falg Table */
  if ((shm_id = shmget(stDBMSKey.SMKeyDBKEY, 0, 0)) < 0) {
    // SR211 pkg
    spdlog::error("[{}:{}] shmget fail pid[{}] errno[{}] strerr[{}]", __func__,
                  __LINE__, getpid(), errno, strerror(errno));
    return -SHARED_DIRTY_ID_GET_FAIL;
  }

  // SR210 pkg
  // if ((pAqiInfo->pDirtyFlagTable = (DirtyFlagTable *)shmat(shm_id, (void
  // *)dbms_addr.dirty_adr, 0)) == NULL)
  if ((pAqiInfo->pDirtyFlagTable = (DirtyFlagTable *)shmat(
           shm_id, (void *)dbms_addr.dirty_adr, 0)) == (DirtyFlagTable *)-1L) {
    // SR211 pkg
    spdlog::error("[{}:{}] shmat fail pid[{}] errno[{}] strerr[{}]", __func__,
                  __LINE__, getpid(), errno, strerror(errno));
    return -SHARED_DIRTY_ATTACH_FAIL;
  }

  /* shared memeory get and attach : Index Table */
  if ((shm_id = shmget(stDBMSKey.SMKeyINDEX, 0, 0)) < 0) {
    // SR211 pkg
    spdlog::error("[{}:{}] shmget fail pid[{}] errno[{}] strerr[{}]", __func__,
                  __LINE__, getpid(), errno, strerror(errno));
    return -SHARED_INDEX_ID_GET_FAIL;
  }

  // SR210 pkg
  // if ((pAqiInfo->index_ptr=(char *)shmat(shm_id, (void *)dbms_addr.index_adr,
  // 0)) == NULL)
  if ((pAqiInfo->index_ptr = (char *)shmat(shm_id, (void *)dbms_addr.index_adr,
                                           0)) == (char *)-1L) {
    // SR211 pkg
    spdlog::error("[{}:{}] shmat fail pid[{}] errno[{}] strerr[{}]", __func__,
                  __LINE__, getpid(), errno, strerror(errno));
    return -SHARED_INDEX_ATTACH_FAIL;
  }

  spdlog::trace("table_ptr : [{}], index_ptr : [{}]",
                fmt::ptr(pAqiInfo->table_ptr), fmt::ptr(pAqiInfo->index_ptr));

  if (dma_recovery_SLD_image(pAqiInfo->table_ptr) < 0) {
    return -RECOVERY_SLD_IMAGE_FAIL;
  }

  pAqiInfo->db_start_ptr = pAqiInfo->table_ptr;
  dma_set_meta_database(pAqiInfo);
  dma_set_meta_index(pAqiInfo);
  dma_align_init(pAqiInfo);

  bzero(&pAqiInfo->semId4IDX, sizeof(stSemArray));
  pAqiInfo->semId4IDX.nSemNum = 1;
  if (0 < (pAqiInfo->semId4IDX.nSemKey = DBMS_SEM_IDX[dbms_type])) {
    if ((pAqiInfo->semId4IDX.nSemId = semget(pAqiInfo->semId4IDX.nSemKey,
                                             pAqiInfo->semId4IDX.nSemNum, 0)) <
        0) {
      // SR211 pkg
      spdlog::error("[{}:{}]-semget[{}][error[{}]] fail pid[{}]", __func__,
                    __LINE__, pAqiInfo->semId4IDX.nSemId, errno, getpid());
      return FLK_FAIL;
    }
  }

  bzero(pAqiInfo->semIdDir, sizeof(stSemArray) * MAX_TABLE);

  for (int tid = 0; ((tid < pAqiInfo->tableCnt) && (MAX_TABLE > tid)); tid++) {
    if ((pAqiInfo->semIdDir[tid].nSemNum = DBMS_SEM_MAX[dbms_type][tid]) < 1) {
      continue;
    }

    if ((pAqiInfo->semIdDir[tid].nSemKey = semKeyBase + tid) < 1) {
      continue;
    }

    if ((pAqiInfo->semIdDir[tid].nSemId =
             semget(pAqiInfo->semIdDir[tid].nSemKey,
                    pAqiInfo->semIdDir[tid].nSemNum, 0)) < 0) {
      spdlog::error("[{}:{}]-semget[{}][error[{}]]", __func__, __LINE__,
                    pAqiInfo->semIdDir[tid].nSemKey, errno);
      return FLK_FAIL;
    }
  }

  Dmaqi_setErrString();

  return FLK_SUCCESS;
}

/*---------------------------------------------------------------------
 *  FUNCTION NAME   : dma_recovery_SLD_image();
 *  PURPOSE         : setting db_header info from sld_file as attaching
                                         address.
 *  INPUT ARGUMENTS :
                                                address attached shared memory.
 *  RETURN VALUE    :
 *  CALLED FROM     :
 *  USED GLOBAL VARIABLES :
 *  REVISION        :
 *      BY              WHEN        REASON
 *      --------------  ----------  -------------------
 *      molee           98.05.05      First Coding
----------------------------------------------------------------------*/
int dma_recovery_SLD_image(char *db_image) {
  DB_header *db_header = (DB_header *)db_image;
  long diff = db_image - db_header->sld_adr;

  spdlog::trace(">>> recovery_SLD_image");
  spdlog::trace("{:<40s} : [{}]", "db_image", fmt::ptr(db_image));
  spdlog::trace("{:<40s} : [{}]", "db_header", fmt::ptr(db_header));
  spdlog::trace("{:<40s} : [{}]", "db_header->sld_adr",
                fmt::ptr(db_header->sld_adr));
  spdlog::trace("{:<40s} : [{}]", "db_header->rdir_adr", db_header->rdir_adr);
  spdlog::trace("{:<40s} : [{}]", "diff", diff);

  db_header->sld_adr = (char *)db_image;
  db_header->rdir_adr = (char *)(db_header->rdir_adr + diff);

  spdlog::trace(">>> after recovery_SLD_image");
  spdlog::trace("{:<40s} : [{}]", "db_header->sld_adr",
                fmt::ptr(db_header->sld_adr));
  spdlog::trace("{:<40s} : [{}]", "db_header->rdir_adr",
                fmt::ptr(db_header->rdir_adr));
  spdlog::trace("{:<40s} : [{}]", "db_header->no_tup_rdir",
                db_header->no_tup_rdir);

  for (int table = 0; table < db_header->no_tup_rdir; table++) {
    Rdir *rdir = (Rdir *)(db_header->sld_adr + S_DBH + S_RDIR * table);
    rdir->rel_adr = (char *)(rdir->rel_adr + diff);
    rdir->rdic_adr = (char *)(rdir->rdic_adr + diff);

    spdlog::trace("tableid: [{}]", table);
    spdlog::trace("rdir : [{}]", fmt::ptr(rdir));
    spdlog::trace("rdir->rel_adr : [{}]", fmt::ptr(rdir->rel_adr));

    if (rdir->numAtt + rdir->numKey > MAX_ATT_PER_TABLE) {
      spdlog::error("Exeeded max attribute[{}] per table on TableId [{}]",
                    MAX_ATT_PER_TABLE, table);
      return FLK_FAIL;
    }
    if (rdir->tupleSize > MAX_TUPLE_SIZE) {
      spdlog::error("Exeeded max tuple[{}] size on TableId [{}]",
                    MAX_TUPLE_SIZE, table);
      return FLK_FAIL;
    }

    for (int att = 0; att < (rdir->numAtt + rdir->numKey); att++) {
      Rdic *rdic = (Rdic *)(rdir->rdic_adr + S_RDIC * att);
      if (rdic->size > MAX_ATTRIBUTE_SIZE) {
        spdlog::error("Exeeded max attribute size on TableID [{}], Att [{}]",
                      table, att);
        return FLK_FAIL;
      }
    }

    if (rdir->tupleSpaceCnt < rdir->used_tup) {
      spdlog::error("used_tup is bigger than tupleSpaceCnt on TableId [{}]",
                    table);
      return FLK_FAIL;
    }

    if ((rdir->tupleSpaceCnt == rdir->used_tup) && (rdir->firstFree >= 0)) {
      return FLK_FAIL;
    }
  }
  return FLK_SUCCESS;
}
/*---------------------------------------------------------------------
 *  FUNCTION NAME   : dma_set_meta_database
 *  PURPOSE         : setting db_header info from shared memory to local
                                          variables
 *  INPUT ARGUMENTS :
                        address attached shared memory.
 *  RETURN VALUE    :
 *  CALLED FROM     :
 *  USED GLOBAL VARIABLES :
 *  REVISION        :
 *      BY              WHEN        REASON
 *      --------------  ----------  -------------------
 *      molee           98.05.05      First Coding
----------------------------------------------------------------------*/
static void dma_set_meta_database(sttAqiInfo *pAqiInfo) {
  DB_header *db_header = (DB_header *)pAqiInfo->table_ptr;
  pAqiInfo->tableCnt = (int)db_header->no_tup_rdir;

  for (int tableId = 0;
       (tableId < pAqiInfo->tableCnt) && (pAqiInfo->tableCnt < MAX_TABLE);
       tableId++) {
    Rdir *rdir = (Rdir *)(db_header->rdir_adr + S_RDIR * tableId);

    pAqiInfo->relationDir[tableId] = rdir;
    pAqiInfo->totAtt[tableId] = (int)(rdir->numKey + rdir->numAtt);
    pAqiInfo->rdirPtr[tableId] = rdir;

    int Align = 0;
    int attOff = 0; /* initialize when we updates mah or fa table */
    for (int attId = 0; attId < pAqiInfo->totAtt[tableId]; attId++) {
      Rdic *rdic;

      rdic = (Rdic *)(rdir->rdic_adr + S_RDIC * attId);

      if (INTEGER == rdic->type) {
        Align = 1;
      }

      pAqiInfo->attDir[tableId][attId].type = rdic->type;
      pAqiInfo->attDir[tableId][attId].srh_md = rdic->srh_md;
      pAqiInfo->attDir[tableId][attId].size = rdic->size;
      pAqiInfo->attDir[tableId][attId].offset = attOff;
      attOff += rdic->size;
      pAqiInfo->Hash_id[tableId][attId] = -1;
    }
    pAqiInfo->needAlign[tableId] = Align;
  }
}

/*---------------------------------------------------------------------
 *  FUNCTION NAME   : dma_set_meta_index()
 *  PURPOSE         : setting index info from shared memory to global
 *                   variables(pAqiInfo->indexDir[]).
 *  INPUT ARGUMENTS :
 *                   address attached index table's shared memory.
 *  RETURN VALUE    :
 *  CALLED FROM     :
 *  USED GLOBAL VARIABLES :
 *							pAqiInfo->indexDir[]
 *  REVISION        :
 *      BY              WHEN        REASON
 *      --------------  ----------  -------------------
 *      molee           98.05.05      First Coding
----------------------------------------------------------------------*/
static void dma_set_meta_index(sttAqiInfo *pAqiInfo) {
  Index_header *index_header = (Index_header *)pAqiInfo->index_ptr;

  spdlog::trace("index_header pointer : [{}]", fmt::ptr(index_header));
  spdlog::trace("TotSize : [{}]", index_header->TotSize);
  spdlog::trace("TotFreeNumElement : [{}]", index_header->TotFreeNumElement);
  spdlog::trace("indexCnt : [{}]", index_header->NumKey);

  for (int indexId = 0; indexId < index_header->NumKey; indexId++) {
    Index *rindex = (Index *)((char *)index_header + S_INDH + S_INX * indexId);
    pAqiInfo->indexDir[indexId] = rindex;
    pAqiInfo->Hash_id[rindex->tableId][rindex->attId] = indexId;

    spdlog::trace(
        "indexId : [{}], index_pointer : [{}], tableId : [{}], attId : [{}]",
        indexId, fmt::ptr(rindex), rindex->tableId, rindex->attId);
  }
}

/*---------------------------------------------------------------------
 *  FUNCTION NAME   : isMdbOverflow()
 *  PURPOSE         : to check db current total record.
 *  INPUT ARGUMENTS : table id
                                          percentage.
 *  RETURN VALUE    : true/false.
 *  CALLED FROM     :
 *  USED GLOBAL VARIABLES :
 *  REVISION        :
 *      BY              WHEN        REASON
 *      --------------  ----------  -------------------
 * 		molee			1998.5		update.
 *-----------------------------------------------------------------------*/
int isMdbOverflow(int nDbmsHandle, int nTableId, int nLimit) {
  if (nDbmsHandle >= MAX_DBMS_CNT || nDbmsHandle < 0) {
    spdlog::error("[{}] - Invalid DBMS Type.", __func__);
    return FLK_FAIL;
  }

  if (gpAqiInfo[nDbmsHandle] == NULL) {
    spdlog::error("[{}] - Didn't connect MDB DMA[{}]!!", __func__, nDbmsHandle);
    return FLK_FAIL;
  }

  Meta_Rdir rdir;
  Meta_Rdir_Ack ack;
  rdir.table_id = nTableId;
  if (db_dma_getrdir(nDbmsHandle, &rdir, &ack) < 0) {
    return FLK_FAIL;
  }

  return ack.storedTupleCnt > (ack.tupleSpaceCnt * (nLimit * 0.01)) ? true
                                                                    : false;
}
/*---------------------------------------------------------------------
 *  FUNCTION NAME   : isMdbUnderrflow()
 *  PURPOSE         : to check db current total record.
 *  INPUT ARGUMENTS : table id
                      percentage value (ex : 10,20).
 *  RETURN VALUE    : true/false.
 *  CALLED FROM     :
 *  USED GLOBAL VARIABLES :
 *  REVISION        :
 *      BY              WHEN        REASON
 *      --------------  ----------  -------------------
 *      molee           1998.5      update.
 *-----------------------------------------------------------------------*/
int isMdbUnderflow(int nDbmsHandle, int nTableId, int nLimit) {

  if (nDbmsHandle >= MAX_DBMS_CNT || nDbmsHandle < 0) {
    spdlog::error("[{}] - Invalid DBMS Type.", __func__);
    return FLK_FAIL;
  }

  if (gpAqiInfo[nDbmsHandle] == NULL) {
    spdlog::error("[{}] - Didn't connect MDB DMA[{}]!!", __func__, nDbmsHandle);
    return FLK_FAIL;
  }

  Meta_Rdir rdir;
  Meta_Rdir_Ack ack;
  rdir.table_id = nTableId;
  if (db_dma_getrdir(nDbmsHandle, &rdir, &ack) < 0) {
    return FLK_FAIL;
  }

  return ack.storedTupleCnt < (ack.tupleSpaceCnt * (nLimit * 0.01)) ? true
                                                                    : false;
}
/*---------------------------------------------------------------------
 *  FUNCTION NAME   : db_dma_getrdir()
 *  PURPOSE         : to get requested table info.
 *  INPUT ARGUMENTS :
                                          refer db_common.h
 *  RETURN VALUE    : table info.
 *  CALLED FROM     :
 *  USED GLOBAL VARIABLES :
 *  REVISION        :
 *      BY              WHEN        REASON
 *      --------------  ----------  -------------------
 * 		molee			1998.5		update.
 *-----------------------------------------------------------------------*/
int db_dma_getrdir(int nDbmsHandle, Meta_Rdir *meta, Meta_Rdir_Ack *ack) {
  if (nDbmsHandle >= MAX_DBMS_CNT || nDbmsHandle < 0) {
    spdlog::error("Invalid DBMS Type.[{}]", __func__);
    return FLK_FAIL;
  }

  // SR230 DB �ʱ�ȭ (connect)�� �̷������ ���� ��Ȳ���� �ش� ������ ���� ��
  // core�� �߻�
  if (gpAqiInfo[nDbmsHandle] == NULL) {
    spdlog::error("[{}] - Didn't connect MDB DMA[{}]!!", __func__, nDbmsHandle);
    return FLK_FAIL;
  }

  sttAqiInfo *pAqiInfo = gpAqiInfo[nDbmsHandle];

  /* Verification of query conditions */
  if (meta->table_id < 0 || meta->table_id >= pAqiInfo->tableCnt) {
    return FLK_FAIL;
  }
  Rdir *pTbl = (Rdir *)(pAqiInfo->relationDir[meta->table_id]);

  /* Retrieve directory data */
  ack->table_id = (int)pTbl->rel_id;
  ack->numKey = (int)pTbl->numKey;
  ack->numAtt = (int)pTbl->numAtt;
  ack->tupleSize = (int)pTbl->tupleSize;
  ack->tupleSpaceCnt = (int)pTbl->tupleSpaceCnt;
  ack->storedTupleCnt = (int)pTbl->used_tup;
  ack->firstFree = (int)pTbl->firstFree;

  return FLK_SUCCESS;
}

/*---------------------------------------------------------------------
 *  FUNCTION NAME   : db_dma_disconnect()
 *  PURPOSE         : to connect mdb directly.
 *  INPUT ARGUMENTS :
 *  RETURN VALUE    : success/fail.
 *  CALLED FROM     :
 *  USED GLOBAL VARIABLES :
 *  REVISION        :
 *      BY              WHEN        REASON
 *      --------------  ----------  -------------------
 *      molee           1998.5      update.
 *-----------------------------------------------------------------------*/
void db_dma_error(int err_no) {
  if (dmaqi_ErrString[(-err_no) - 200] == NULL) {
    return;
  }
}

/*---------------------------------------------------------------------
 *  FUNCTION NAME   : mdb_dma_select();
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
int mdb_dma_select(int nDbmsHandle, Select *select, Select_Ack *ack, int mode) {
  char *noAlign;

  if (nDbmsHandle >= MAX_DBMS_CNT || nDbmsHandle < 0) {
    spdlog::error("Invalid DBMS Type.[{}]", __func__);
    return FLK_FAIL;
  }

  sttAqiInfo *pAqiInfo = gpAqiInfo[nDbmsHandle];

  if (pAqiInfo == NULL) {
    spdlog::error("[{}] - Didn't connect MDB DMA[{}]!!", __func__, nDbmsHandle);
    return FLK_FAIL;
  }

  int tupId = select->tup_id;
  /*attVal		=	&select->att_val;*/
  //	const char *result		=	&ack->tuple[0];

  /* Verification of query conditions */
  if (select->table_id < 0 || select->table_id >= pAqiInfo->tableCnt) {
    return -DMAQI_DB_BAD_TABLE_ID;
  }

  if (tupId < 0 ||
      tupId > pAqiInfo->relationDir[select->table_id]->tupleSpaceCnt) {
    return -DMAQI_DB_BAD_TUPLE_ID;
  }

  /* select processing */
  if (mode == SELECT_SEQ) {
    int nTupCnt;
    for (nTupCnt = 0;
         dma_table_retr(nDbmsHandle, select->table_id, tupId, &noAlign) < 0;
         nTupCnt++) {
      /* increase tupId until dbms finds not-null-data */
      if (++tupId >= pAqiInfo->relationDir[select->table_id]->tupleSpaceCnt) {
        return -DMAQI_THERE_IS_NOT_THE_TUPLE;
      }
    }
    ack->tup_id = tupId;
    /* returns tupId that appears first from start point */
  } else {
    if (select->att_id < 0 ||
        select->att_id >= pAqiInfo->totAtt[select->table_id] ||
        MAX_ATT_PER_TABLE <= select->att_id) {
      return -DB_BAD_ATTRIBUTE_ID;
    }

    if (select->att_val == NULL) {
      return -DB_ATTRIBUTE_VAL_NULL;
    }

    if ((ack->tup_id = dma_getTupId(nDbmsHandle, select->table_id,
                                    select->att_id, select->att_val, tupId)) <
        0) {
      return -DMAQI_THERE_IS_NOT_THE_TUPLE;
    }
    if (dma_table_retr(nDbmsHandle, select->table_id, ack->tup_id, &noAlign) <
        0) {
      return -TABLE_RETRIEVAL_FAIL;
    }
  }

  dma_align_make(nDbmsHandle, select->table_id, ack->tuple, noAlign);
  return ack->tup_id;
}

/*---------------------------------------------------------------------
 *  FUNCTION NAME   : mdb_dma_update();
 *  PURPOSE         : to provide other with update operation
                                          generalize input argument
 *  INPUT ARGUMENTS :
                                                update struct/ack struct/mode.
                                                refer db_message.h.
 *  RETURN VALUE    :
 *  CALLED FROM     :
 *  USED GLOBAL VARIABLES :
 *  REVISION        :
 *      BY              WHEN        REASON
 *      --------------  ----------  -------------------
 *      molee           98.05.05      First Coding
----------------------------------------------------------------------*/
int mdb_dma_update(int nDbmsHandle, Update *update, Select_Ack *ack, int mode) {

  char *cucnt, *noAlignOld, updTupBuf[MAX_TUPLE_SIZE];
  char *updTupPtr, rmAlignUpd[MAX_TUPLE_SIZE];
  int ix; //,rstno;

  if (nDbmsHandle >= MAX_DBMS_CNT || nDbmsHandle < 0) {
    spdlog::error("Invalid DBMS Type.[{}]", __func__);
    return FLK_FAIL;
  }

  sttAqiInfo *pAqiInfo = gpAqiInfo[nDbmsHandle];

  if (pAqiInfo == NULL) {
    spdlog::error("[{}] - Didn't connect MDB DMA[{}]!!", __func__, nDbmsHandle);
    return FLK_FAIL;
  }

  int tableId = update->table_id;
  int attId = update->att_id;
  int tupId = update->tup_id;
  int updCnt = update->update_cnt;
  //	char *updList	=	&update->update_list[0];
  //	char *attVal	=	&update->att_val[0];
  //	char *updVal	=	&update->tuple[0];
  //	char *result	=	&ack->tuple[0];

  char *updAttList = (char *)&update->update_list[0];

  Select_Ack ack1{};

  /* Verification of query conditions */
  if (tableId < 0 || tableId >= pAqiInfo->tableCnt) {
    return -DMAQI_DB_BAD_TABLE_ID;
  }

  if (tupId < 0 || tupId > pAqiInfo->relationDir[tableId]->tupleSpaceCnt) {
    return -DMAQI_DB_BAD_TUPLE_ID;
  }

  if (updCnt <= 0 || updCnt >= pAqiInfo->totAtt[tableId]) {
    return -UPDATE_COUNT_INVALID;
  }

  /* investigate whether key attributes is updated */
  for (ix = 0; ix < updCnt; ix++) {
    int updAtt;
    updAtt = (int)((u_char)updAttList[ix]);
    if (updAtt < 0 || updAtt > pAqiInfo->totAtt[tableId]) {
      return -DB_BAD_ATTRIBUTE_ID;
    }
    if (pAqiInfo->Hash_id[tableId][updAtt] >= 0) {
      return -DMAQI_KEY_ATTRIBUTE_UPDATE;
    }
  }

  /* Searching target tuple */
  if (mode == UPDATE_SEQ) {
    int nTupCnt;
    for (nTupCnt = 0;
         dma_table_retr(nDbmsHandle, tableId, tupId, &noAlignOld) < 0;
         nTupCnt++) {
      if (++tupId >= pAqiInfo->relationDir[tableId]->tupleSpaceCnt) {
        return -DMAQI_THERE_IS_NOT_THE_TUPLE;
      }
    }
    ack->tup_id = tupId;
  } else if (mode == UPDATE_ATTR_EX1) {
    if (attId < 0 || attId >= pAqiInfo->totAtt[tableId]) {
      return -DB_BAD_ATTRIBUTE_ID;
    }

    if (update->att_val == NULL) {
      return -DB_ATTRIBUTE_VAL_NULL;
    }

    if (dma_table_retr(nDbmsHandle, tableId, tupId, &noAlignOld) < 0) {
      return -DMAQI_THERE_IS_NOT_THE_TUPLE;
    }
    ack->tup_id = tupId;
    (void)dma_align_make(nDbmsHandle, tableId, ack1.tuple, noAlignOld);
  } else {
    if (attId < 0 || attId >= pAqiInfo->totAtt[tableId]) {
      return -DB_BAD_ATTRIBUTE_ID;
    }

    if (update->att_val == NULL) {
      return -DB_ATTRIBUTE_VAL_NULL;
    }

    if ((ack->tup_id = dma_getTupId(nDbmsHandle, tableId, attId,
                                    update->att_val, tupId)) < 0) {
      return -DMAQI_THERE_IS_NOT_THE_TUPLE;
    }

    if (dma_table_retr(nDbmsHandle, tableId, ack->tup_id, &noAlignOld) < 0) {
      return -TABLE_RETRIEVAL_FAIL;
    }
    (void)dma_align_make(nDbmsHandle, tableId, ack1.tuple, noAlignOld);
  }

  /* Removing non used space which is to align data */
  if (pAqiInfo->needAlign[tableId]) {
    dma_align_remove(nDbmsHandle, tableId, rmAlignUpd, update->tuple);
    updTupPtr = rmAlignUpd;
  } else {
    updTupPtr = update->tuple;
  }

  /* This block is used when Auc updates data
  ; cucunt is pointer that points DB_CountUpdateCount */
  /* In other case, UPDATE_EX2 is not used. Do you understand? */
  if (mode == UPDATE_EX2) {
    cucnt =
        ATTR_PTR(pAqiInfo->attDir, noAlignOld, tableId, DB_CountUpdateCount);
    *cucnt = *cucnt + 1;
  }

  if (mode == UPDATE_EX || mode == UPDATE_EX2 || mode == UPDATE_EX3) {
    (void)dma_align_make(nDbmsHandle, tableId, ack->tuple, noAlignOld);
  }
  if ((mode == UPDATE_ATTR) || (mode == UPDATE_ATTR_EX1)) {
    int updAtt;

    for (ix = 0; ix < updCnt; ix++) {
      updAtt = (int)((u_char)updAttList[ix]);
      (void)memcpy(ATTR_PTR(pAqiInfo->attDir, noAlignOld, tableId, updAtt),
                   ATTR_PTR(pAqiInfo->attDir, updTupPtr, tableId, updAtt),
                   (unsigned)pAqiInfo->attDir[tableId][updAtt].size);
    }
    (void)dma_align_make(nDbmsHandle, tableId, ack->tuple, noAlignOld);
    return ack->tup_id;
  }

  /* Modify tuple value */
  (void)memcpy(updTupBuf, noAlignOld,
               (unsigned)pAqiInfo->relationDir[tableId]->tupleSize);

  for (ix = 0; ix < updCnt; ix++) {
    int updAtt;
    updAtt = (int)((u_char)updAttList[ix]);
    (void)memcpy(ATTR_PTR(pAqiInfo->attDir, updTupBuf, tableId, updAtt),
                 ATTR_PTR(pAqiInfo->attDir, updTupPtr, tableId, updAtt),
                 (unsigned)pAqiInfo->attDir[tableId][updAtt].size);
  }

  if (dma_table_modi(nDbmsHandle, tableId, ack->tup_id, &noAlignOld,
                     updTupBuf) < 0) {
    return -TUPLE_UPDATE_FAIL;
  }

  /* Copy updated data to reply message for echo (UPDATE_EO) */
  if (mode == UPDATE_EO) {
    (void)dma_align_make(nDbmsHandle, tableId, ack->tuple, noAlignOld);
  }

  return ack->tup_id;
}
/*---------------------------------------------------------------------
 *  FUNCTION NAME   : dma_getTupId()
 *  PURPOSE         : get tupId	from table requested attId  & attVal.
 *  INPUT ARGUMENTS :
 * 						table , attribute,tupid
indicator.
 *  RETURN VALUE    :
 *  CALLED FROM     :
 *  USED GLOBAL VARIABLES :
 *                          pAqiInfo->indexDir[]
 *  REVISION        :
 *      BY              WHEN        REASON
 *      --------------  ----------  -------------------
 *      molee           98.05.05      First Coding
----------------------------------------------------------------------*/
int dma_getTupId(int nDbmsHandle, int tableId, int attId, char *attVal,
                 int tupId) {
  Rdir *pTbl;
  char *noAlign;
  int ix, count = 0, comp_result;
  sttAqiInfo *pAqiInfo;

  if (nDbmsHandle >= MAX_DBMS_CNT || nDbmsHandle < 0) {
    spdlog::error("Invalid DBMS Type.[{}]", __func__);
    return FLK_FAIL;
  }

  pAqiInfo = gpAqiInfo[nDbmsHandle];

  if (gpAqiInfo[nDbmsHandle] == NULL) {
    spdlog::error("[{}] - Didn't connect MDB DMA[{}]!!", __func__, nDbmsHandle);
    return FLK_FAIL;
  }

  if (tableId < 0 || tableId >= pAqiInfo->tableCnt) {
    spdlog::error("[{}]Invalid Table Id[{}].", __func__, tableId);
    return FLK_FAIL;
  }

  pTbl = (Rdir *)pAqiInfo->relationDir[tableId];

  if (pAqiInfo->Hash_id[tableId][attId] >= 0) {
    return (
        dma_hash_search(pAqiInfo, pAqiInfo->Hash_id[tableId][attId], attVal));
  }

  for (ix = tupId; ix < pTbl->tupleSpaceCnt && count < pTbl->used_tup; ix++) {
    if (dma_table_retr(nDbmsHandle, tableId, ix, &noAlign) < 0) {
      continue;
    }
    count++;
    comp_result =
        dma_compare(attVal, ATTR_PTR(pAqiInfo->attDir, noAlign, tableId, attId),
                    (unsigned)pAqiInfo->attDir[tableId][attId].size);
    if (comp_result == 0) {
      return ix;
    }
  }

  return FLK_FAIL;
}

/*---------------------------------------------------------------------
 *  FUNCTION NAME   :   dma_hash_search();
 *  PURPOSE         :   seacrh message through key index.
 *  INPUT ARGUMENTS :
 *                      Index indicator , Key value.
 *  RETURN VALUE    : 	success/fail.
 *  CALLED FROM     :
 *  USED GLOBAL VARIABLES :
 *  REVISION        :
 *      BY              WHEN        REASON
 *      --------------  ----------  -------------------
 *      molee           98.05.05      First Coding
----------------------------------------------------------------------*/

#if 0
int dma_hash_print(sttAqiInfo *pAqiInfo, int indexId, char *key)
{
	Index   *pIndex = (Index *)pAqiInfo->indexDir[indexId];
	if (pIndex == NULL) {
		return FLK_FAIL;
	}

	int keyLen  = pIndex->keyLength;
	if (keyLen <= 0) {
		return  FLK_FAIL;
	}

	Element *ppElm = dma_get_bucket_with_key(pAqiInfo, indexId, key);
	if (ppElm == NULL) {
		return FLK_FAIL;
	} else {
		int nHashCnt = 0;
		char    cpKeyNull[MAX_HASH_KEY_SIZE];
		bzero(cpKeyNull, sizeof(cpKeyNull));

		for (; (ppElm != NULL) && (0 != memcmp(ppElm->key, cpKeyNull, keyLen)) && (nHashCnt < pIndex->numElem) && (MAX_HASH_COUNT > nHashCnt); nHashCnt++) {
			if (!dma_compare(key, ppElm->key, keyLen)) {
				return (int)ppElm->tupId;
			}
			ppElm = ppElm->next;
		}
		return FLK_FAIL;
	}
	//return FLK_SUCCESS;
}
#endif

int dma_hash_search(sttAqiInfo *pAqiInfo, int indexId, char *key) {

#if 0
#ifdef _TEST
	dma_hash_print(pAqiInfo, indexId, key); // smyang for test
#endif
#endif

  Index *pIndex = (Index *)pAqiInfo->indexDir[indexId];
  if (pIndex == NULL) {
    spdlog::error("[{}] - Didn't connect MDB DMA[{}]!!", __func__, indexId);
    return FLK_FAIL;
  }

  int keyLen = pIndex->keyLength;
  if (keyLen <= 0) {
    spdlog::error("[{}][{}]INVALID_INDEX_ID, indexId:[{}]", __FILE__, __LINE__,
                  indexId);
    return FLK_FAIL;
  }

  Element *ppElm = dma_get_bucket_with_key(pAqiInfo, indexId, key);
  if (ppElm == NULL) {
    return FLK_FAIL;
  }

  int nHashCnt = 0;
  char cpKeyNull[MAX_HASH_KEY_SIZE];
  bzero(cpKeyNull, sizeof(cpKeyNull));

  for (; (ppElm != NULL) && (nHashCnt < pIndex->numElem) &&
         (MAX_HASH_COUNT > nHashCnt) &&
         (0 != memcmp(ppElm->key, cpKeyNull, keyLen));
       nHashCnt++) {
    if (!dma_compare(key, ppElm->key, keyLen)) {
      return (int)ppElm->tupId;
    }
    ppElm = ppElm->next;
  }

  return FLK_FAIL;
}

/*---------------------------------------------------------------------
 *  FUNCTION NAME   :   dma_get_bucket_with_key()
 *  PURPOSE         :   get elememt info requested key from index table.
 *  INPUT ARGUMENTS :
 *                      Index indicator , Key value.
 *  RETURN VALUE    :   Element *.
                                                Refer db_data_struct.h.
 *  CALLED FROM     :
 *  USED GLOBAL VARIABLES :
 *  REVISION        :
 *      BY              WHEN        REASON
 *      --------------  ----------  -------------------
 *      molee           98.05.05      First Coding
----------------------------------------------------------------------*/

Element *dma_get_bucket_with_key(sttAqiInfo *pAqiInfo, int indexId, char *key) {
  Index *pIndex = (Index *)pAqiInfo->indexDir[indexId];
  if (pIndex == NULL) {
    spdlog::error("[{}] - Didn't connect MDB DMA[{}]!!", __func__, indexId);
    return NULL;
  }

  int hashValue = (unsigned)dma_hash1(key, pIndex->keyLength);
  int segmentId = hashValue / SEGMENT_SIZE;
  int segmentOffset = hashValue % SEGMENT_SIZE;

  /*
  if (indexId == 0 ) {
          u_int msgid;
          memcpy(&msgid, key, 4);
          spdlog::info("[{} : {}] key [{:>02x}{:>02x}{:>02x}{:>02x}](0x{:>08x})
  key_len [{}]", __func__, __LINE__, (u_char)key[0], (u_char)key[1],
  (u_char)key[2], (u_char)key[3], msgid, pIndex->keyLength); spdlog::info("[{} :
  {}] hashValue[{}] segmentId[{}] segmentOffset[{}]", __func__, __LINE__,
  hashValue, segmentId, segmentOffset);
  }
  */

  if (segmentId < 0 || segmentId >= DIRECTORY_SIZE || segmentOffset < 0 ||
      segmentOffset >= SEGMENT_SIZE) {
    return NULL;
  }

#if 0
	Element *elementrst = (Element *) pIndex->dir[segmentId][segmentOffset];
	if ( elementrst == NULL ) {
		/*
		if ( indexId == 0 ) {
			spdlog::info("[{} : {}] elementrst is null", __func__, __LINE__);
			u_int msgid;
			memcpy(&msgid, key, 4);
			spdlog::info("[{} : {}] key [{:>02x}{:>02x}{:>02x}{:>02x}](0x{:>08x}) key_len [{}]", __func__, __LINE__,
					(u_char)key[0], (u_char)key[1], (u_char)key[2], (u_char)key[3], msgid, pIndex->keyLength);
		}
		*/
		return NULL;
	}

	return elementrst;
#else
  return (Element *)pIndex->dir[segmentId][segmentOffset];
#endif
}
/*---------------------------------------------------------------------
 *  FUNCTION NAME   :   dma_hash1()
 *  PURPOSE         :   get hash key for key value.
 *  INPUT ARGUMENTS :
 *                      Key value.
 *  RETURN VALUE    :	hash key.
 *  CALLED FROM     :
 *  USED GLOBAL VARIABLES :
 *  REVISION        :
 *      BY              WHEN        REASON
 *      --------------  ----------  -------------------
 *      molee           98.05.05      First Coding
----------------------------------------------------------------------*/
unsigned dma_hash1(const char *key, int len) {

  unsigned h = 0, l = len;
  u_char *k = (u_char *)key;

  /* Convert string to integer */

  while (l--) {
    h = h * PRIME_NEW_KEY ^ ((*k & 0x0F) - 0x10);
    h = h * PRIME_NEW_KEY ^ ((*k & 0xF0) - 0x01);
    k++;
  }

  h %= PRIME_BURKET_SIZE;

  return h;
}
/* This is Old Version */
/*
unsigned dma_hash1(char *key, int len)
{
        unsigned h=0, l=len;
        u_char *k=(u_char *)key;
        int	nLoop;

#define PRIME1       37
#define PRIME2       1048576

        // Convert string to integer
        while ( l-- )
                h = h*PRIME1^(*k++ - ' ');
        h %= PRIME2;
        h /= MAX_LOAD_FACTOR;
        return h;
}
#undef PRIME1
#undef PRIME2
*/

/*---------------------------------------------------------------------
 *  FUNCTION NAME   :   dma_table_retr()
 *  PURPOSE         :  	get record start address pointer for
 *						requested tableId  & tupId.
 *  INPUT ARGUMENTS :
 *                     table/tup indicator.
 *  RETURN VALUE    :	success/fail.
 *  CALLED FROM     :
 *  USED GLOBAL VARIABLES :
 *  REVISION        :
 *      BY              WHEN        REASON
 *      --------------  ----------  -------------------
 *      molee           98.05.05      First Coding
----------------------------------------------------------------------*/
int dma_table_retr(int nDbmsHandle, int tableId, int tupId, char **pTupVal) {
  if (nDbmsHandle >= MAX_DBMS_CNT || nDbmsHandle < 0) {
    spdlog::error("Invalid DBMS Type.[{}]", __func__);
    return FLK_FAIL;
  }

  if (gpAqiInfo[nDbmsHandle] == NULL) {
    spdlog::error("[{}] - Didn't connect MDB DMA[{}]", __func__, nDbmsHandle);
    return FLK_FAIL;
  }

  if ((*pTupVal = dma_tupleId2tuplePtr(nDbmsHandle, tableId, tupId)) == NULL) {
    return FLK_FAIL;
  }
  return 1;
}

/*---------------------------------------------------------------------
 *  FUNCTION NAME   :   dma_table_modi()
 *  PURPOSE         :
 *  INPUT ARGUMENTS :
 *						table/tup indicator.
 *						tuple * it will be modified.
 *  RETURN VALUE    :   success/fail.
 *  CALLED FROM     :
 *  USED GLOBAL VARIABLES :
 *  REVISION        :
 *      BY              WHEN        REASON
 *      --------------  ----------  -------------------
 *      molee           98.05.05      First Coding
----------------------------------------------------------------------*/
int dma_table_modi(int nDbmsHandle, int tableId, int tupId, char **ppTupVal,
                   char *tupVal) {
  if (nDbmsHandle >= MAX_DBMS_CNT || nDbmsHandle < 0) {
    spdlog::error("Invalid DBMS Type.[{}]", __func__);
    return FLK_FAIL;
  }

  sttAqiInfo *pAqiInfo = gpAqiInfo[nDbmsHandle];

  if ((*ppTupVal = dma_tupleId2tuplePtr(nDbmsHandle, tableId, tupId)) == NULL) {
    return FLK_FAIL;
  }
  memcpy(*ppTupVal, tupVal, pAqiInfo->relationDir[tableId]->tupleSize);
  dma_dirtyFlag_set(
      pAqiInfo, dma_tupleId2ptr(pAqiInfo->rdirPtr, tableId, tupId),
      (int)(pAqiInfo->relationDir[tableId]->tupleSize + (int)S_INT));

  return 1;
}

/*---------------------------------------------------------------------
 *  FUNCTION NAME   :   dma_dirtyFlag_set()
 *  PURPOSE         :
 *  INPUT ARGUMENTS :
 *						db table's shared memory block(
it had modified. *						example,
insert/update/delete operation) address.
 *  RETURN VALUE    :   success/fail.
 *  CALLED FROM     :
 *  USED GLOBAL VARIABLES :
 *  REVISION        :
 *      BY              WHEN        REASON
 *      --------------  ----------  -------------------
 *      molee           98.05.05      First Coding
----------------------------------------------------------------------*/
void dma_dirtyFlag_set(sttAqiInfo *pAqiInfo, char *pStart, int nSize) {
  return;
  /*
  int     startFlag, endFlag, flag;

  startFlag=dma_addr2blockNum(pAqiInfo, pStart);
  endFlag  =dma_addr2blockNum(pAqiInfo, (char *)(pStart+nSize));

  for (flag=startFlag; flag < endFlag+1; flag++)
          pAqiInfo->pDirtyFlagTable->dirtyFlag[flag] = DIRTY_ON;
          */
}
/*---------------------------------------------------------------------
 *  FUNCTION NAME   :  dma_addr2blockNum()
 *  PURPOSE         :
 *  INPUT ARGUMENTS :
 *                      db table's shared memory block( it had modified.
 *                      example, insert/update/delete operation) address.
 *  RETURN VALUE    :   calculate block num for input address.
 *  CALLED FROM     :
 *  USED GLOBAL VARIABLES :
 *  REVISION        :
 *      BY              WHEN        REASON
 *      --------------  ----------  -------------------
 *      molee           98.05.05      First Coding
----------------------------------------------------------------------*/
/*
static int dma_addr2blockNum(sttAqiInfo* pAqiInfo, char *addr)
{
        unsigned distance, blockNum;

        distance = (unsigned)(addr-pAqiInfo->db_start_ptr);
        blockNum = distance / BLOCK_SIZE;

        return blockNum;
}
*/

/*---------------------------------------------------------------------
 *  FUNCTION NAME   :  dma_tupleId2tuplePtr()
 *  PURPOSE         :
 *  INPUT ARGUMENTS :
 *					 	table, tupId indicator.
 *  RETURN VALUE    :   tuple start address.
 *  CALLED FROM     :
 *  USED GLOBAL VARIABLES :
 *  REVISION        :
 *      BY              WHEN        REASON
 *      --------------  ----------  -------------------
 *      molee           98.05.05      First Coding
----------------------------------------------------------------------*/
char *dma_tupleId2tuplePtr(int nDbmsHandle, int tableId, int tupId) {
  if (nDbmsHandle >= MAX_DBMS_CNT || nDbmsHandle < 0) {
    spdlog::error("Invalid DBMS Type.[{}]", __func__);
    return NULL;
  }

  sttAqiInfo *pAqiInfo = gpAqiInfo[nDbmsHandle];
  if (pAqiInfo == NULL) {
    spdlog::error("[{}] - Didn't connect MDB DMA[{}]!!", __func__, nDbmsHandle);
    return NULL;
  }

  char *ptr = dma_tupleId2ptr(pAqiInfo->rdirPtr, tableId, tupId);
  if (ptr == NULL) {
    return NULL;
  }

  if (dma_charStr2int(ptr) == FILLED_TUPLE_SPACE) {
    ptr += S_INT;
  } else {
    ptr = NULL;
  }

  return ptr;
}

/*---------------------------------------------------------------------
 *  FUNCTION NAME   :	dma_charStr2int()
 *  PURPOSE         :	convert char types to integer.
 *  INPUT ARGUMENTS :
 *                     	char string.
 *  RETURN VALUE    :	int
 *  CALLED FROM     :
 *  USED GLOBAL VARIABLES :
 *  REVISION        :
 *      BY              WHEN        REASON
 *      --------------  ----------  -------------------
 *      molee           98.05.05      First Coding
----------------------------------------------------------------------*/
int dma_charStr2int(char *charStr) {
  /*
  union {
          int     integer;
          char    str[S_INT];
  } data;

  int     i;

  for (i=0; i<S_INT; i++) {
          data.str[i]=(char)*charStr;
          charStr++;
  }
  return data.integer;
  */
  int integer;
  memcpy(&integer, charStr, sizeof(int));
  return integer;
}

/*---------------------------------------------------------------------
 *  FUNCTION NAME   :   dma_compare()
 *  PURPOSE         :   dma_compare p1 with p2
 *  INPUT ARGUMENTS :
 *
 *  RETURN VALUE    :
 *  CALLED FROM     :
 *  USED GLOBAL VARIABLES :
 *  REVISION        :
 *      BY              WHEN        REASON
 *      --------------  ----------  -------------------
 *      molee           98.05.05      First Coding
----------------------------------------------------------------------*/
int dma_compare(char *p1, char *p2, int n) {
  for (int ix = 0; ix < n; ix++) {
    if ((u_char)(p1[ix]) > (u_char)(p2[ix])) {
      return 1;
    }
    if ((u_char)(p1[ix]) < (u_char)(p2[ix])) {
      return -1;
    }
  }
  return 0;
}

int comparePattern(char *p1, char *p2, int n) {
  int ix;

  for (ix = 0; ix < n; ix++) {
    if ((u_char)(p1[ix]) > (u_char)(p2[ix])) {
      return 1;
    }
    if ((u_char)(p1[ix]) < (u_char)(p2[ix])) {
      return -1;
    }
  }

  return (ix == n) ? 0 : -1;
}

/*---------------------------------------------------------------------
 *  FUNCTION NAME   :   dma_Digit2Str()
 *  PURPOSE         : 	convert digit to string value.
 *  INPUT ARGUMENTS :
 *						digit string.
 *  RETURN VALUE    :   char 	string.
 *  CALLED FROM     :
 *  USED GLOBAL VARIABLES :
 *  REVISION        :
 *      BY              WHEN        REASON
 *      --------------  ----------  -------------------
 *      molee           98.05.05      First Coding
----------------------------------------------------------------------*/
/*---------------------------------------------------------------------
 *  FUNCTION NAME   :   dma_align_init()
 *  PURPOSE         :   get align for attribute to store table data
 *						at shared memory without space.
 *  INPUT ARGUMENTS :
 *  RETURN VALUE    :
 *  CALLED FROM     :
 *  USED GLOBAL VARIABLES :
 *							pAqiInfo->attDir[] =
attr info struct. *
shifCount[]	= info to align data.
 *  REVISION        :
 *      BY              WHEN        REASON
 *      --------------  ----------  -------------------
 *      molee           98.05.05      First Coding
----------------------------------------------------------------------*/
static void dma_align_init(sttAqiInfo *pAqiInfo) {
  for (int tableId = 0; tableId < pAqiInfo->tableCnt && tableId <= MAX_TABLE;
       tableId++) {
    int tupleIndex = 0, attCnt = pAqiInfo->totAtt[tableId], count = 0;

    for (int attId = 0; attId < attCnt; attId++) {
      AttDir *pAtt = &(pAqiInfo->attDir[tableId][attId]);

      if (INTEGER == pAtt->type) {
        int remainder;

        if ((remainder = (tupleIndex + count) % (int)S_INT) != 0) {
          count += (int)S_INT - remainder;
        }
      }
      for (int i = 0; i < pAtt->size; i++, tupleIndex++) {
        pAqiInfo->shiftCount[tableId][tupleIndex] = count;
      }
    }
  }
}

/*---------------------------------------------------------------------
 *  FUNCTION NAME   :   dma_align_make()
 *  PURPOSE         :   to align as data types.
 *  INPUT ARGUMENTS :
 *  RETURN VALUE    :
 *  CALLED FROM     :
 *  USED GLOBAL VARIABLES :
 *							pAqiInfo->relationDir[]
= table info.
 *                          shifCount[] = info to align data.
 *  REVISION        :
 *      BY              WHEN        REASON
 *      --------------  ----------  -------------------
 *      molee           98.05.05      First Coding
----------------------------------------------------------------------*/
void dma_align_make(int nDbmsHandle, int tableId, char *to, char *from) {
  if (nDbmsHandle >= MAX_DBMS_CNT || nDbmsHandle < 0) {
    spdlog::error("Invalid DBMS Type.[{}]", __func__);
    return;
  }

  sttAqiInfo *pAqiInfo = gpAqiInfo[nDbmsHandle];
  if (pAqiInfo == NULL) {
    spdlog::error("[{}] - Didn't connect MDB DMA[{}]!!", __func__, nDbmsHandle);
    return;
  }

  int tuple_size = (int)pAqiInfo->relationDir[tableId]->tupleSize - 1;

  if (pAqiInfo->needAlign[tableId]) {
    for (int ix = tuple_size; ix >= 0; ix--) {
      to[ix + pAqiInfo->shiftCount[tableId][ix]] = from[ix];
    }
  } else {
    for (int ix = tuple_size; ix >= 0; ix--) {
      to[ix] = from[ix];
    }
  }
}

/*---------------------------------------------------------------------
 *  FUNCTION NAME   :   dma_align_remove()
 *  PURPOSE         :   remove data's space.
 *  INPUT ARGUMENTS :
 *  RETURN VALUE    :
 *  CALLED FROM     :
 *  USED GLOBAL VARIABLES :
 *                          pAqiInfo->relationDir[]  = table info.
 *                          shifCount[] = info to align data.
 *  REVISION        :
 *      BY              WHEN        REASON
 *      --------------  ----------  -------------------
 *      molee           98.05.05      First Coding
----------------------------------------------------------------------*/
void dma_align_remove(int nDbmsHandle, int tableId, char *to, char *from) {
  if (nDbmsHandle >= MAX_DBMS_CNT || nDbmsHandle < 0) {
    spdlog::error("Invalid DBMS Type.[{}]", __func__);
    return;
  }

  sttAqiInfo *pAqiInfo = gpAqiInfo[nDbmsHandle];
  if (pAqiInfo == NULL) {
    spdlog::error("[{}] - Didn't connect MDB DMA[{}]!!", __func__, nDbmsHandle);
    return;
  }

  int tuple_size = (int)pAqiInfo->relationDir[tableId]->tupleSize;

  for (int ix = tuple_size; ix >= 0; ix--) {
    to[ix] = from[ix + pAqiInfo->shiftCount[tableId][ix]];
  }
}

int dma_ActiveTuplePtr(int nDbmsHandle, u_int unMessageId,
                       stRDActiveTuple **spActive) {

  if (nDbmsHandle >= MAX_DBMS_CNT || nDbmsHandle < 0) {
    spdlog::error("Invalid DBMS Type.[{}]", __func__);
    return FLK_FAIL;
  }

  if (gpAqiInfo[nDbmsHandle] == NULL) {
    spdlog::error("[{}] - Didn't connect MDB DMA[{}]!!", __func__, nDbmsHandle);
    return FLK_FAIL;
  }

  int attId = DB_MESSAGE_ID;
  int tupId = -1;
  char attVal[DB_MESSAGE_ID_SIZE];
  memcpy(attVal, &unMessageId, DB_MESSAGE_ID_SIZE);

  // spdlog::info("[{} : {}] MSGID[{:>02X}{:>02X}{:>02X}{:>02X}] {:>08x}",
  // __func__, __LINE__, (u_char)attVal[0], (u_char)attVal[1],
  // (u_char)attVal[2], (u_char)attVal[3], unMessageId);
  if ((tupId = dma_getTupId(nDbmsHandle, R_DACTIVE, attId, attVal, tupId)) <
      0) {
    return -DMAQI_THERE_IS_NOT_THE_TUPLE;
  }
  char *noAlign;
  if (dma_table_retr(nDbmsHandle, R_DACTIVE, tupId, &noAlign) < 0) {
    return -DMAQI_DB_BAD_TUPLE_ID;
  }
  // return Orignal Tuple Pointer in ACTVDB
  *spActive = (stRDActiveTuple *)noAlign;
  return FLK_SUCCESS;
}

int dma_SubmitTuplePtr(int nDbmsHandle, u_int unMessageId,
                       stRDActiveTuple **spActive) {

  if (nDbmsHandle >= MAX_DBMS_CNT || nDbmsHandle < 0) {
    spdlog::error("Invalid DBMS Type.[{}]", __func__);
    return FLK_FAIL;
  }

  if (gpAqiInfo[nDbmsHandle] == NULL) {
    spdlog::error("[{}] - Didn't connect MDB DMA[{}]!!", __func__, nDbmsHandle);
    return FLK_FAIL;
  }

  int attId = DB_MESSAGE_ID;
  int tupId = -1;
  char attVal[DB_MESSAGE_ID_SIZE];
  memcpy(attVal, &unMessageId, DB_MESSAGE_ID_SIZE);

  // spdlog::info("[{} : {}] MSGID[{:>02X}{:>02X}{:>02X}{:>02X}] {:>08x}",
  // __func__, __LINE__, (u_char)attVal[0], (u_char)attVal[1],
  // (u_char)attVal[2], (u_char)attVal[3], unMessageId);
  if ((tupId = dma_getTupId(nDbmsHandle, R_DSUBMIT, attId, attVal, tupId)) <
      0) {
    return -DMAQI_THERE_IS_NOT_THE_TUPLE;
  }
  char *noAlign;
  if (dma_table_retr(nDbmsHandle, R_DSUBMIT, tupId, &noAlign) < 0) {
    return -DMAQI_DB_BAD_TUPLE_ID;
  }
  // return Orignal Tuple Pointer in ACTVDB
  *spActive = (stRDActiveTuple *)noAlign;
  return FLK_SUCCESS;
}

int dma_getActiveTuplePtr(u_int unMessageId, stRDActiveTuple **spActive) {
  return dma_ActiveTuplePtr(DBMS_ACTIVE, unMessageId, spActive);
}

int dma_getReceiptTuplePtr(u_int unMessageId, stRDActiveTuple **spActive) {
  return dma_ActiveTuplePtr(DBMS_RECEIPT, unMessageId, spActive);
}

int dma_getRcsTuplePtr(u_int unMessageId, stRDActiveTuple **spActive) {
  return dma_ActiveTuplePtr(DBMS_RCS, unMessageId, spActive);
}

int selectGetActiveRec(int nDbmsHandle, u_int unMessageId,
                       stRDActiveTuple *spActive) {
  char *noAlign;

  if (gpAqiInfo[nDbmsHandle] == NULL) {
    spdlog::error("[{}] - Didn't connect MDB DMA[{}]!!", __func__, nDbmsHandle);
    return FLK_FAIL;
  }

  char attVal[DB_MESSAGE_ID_SIZE];
  memcpy(attVal, &unMessageId, DB_MESSAGE_ID_SIZE);

  int tupId = -1;
  if ((tupId = dma_getTupId(nDbmsHandle, R_DACTIVE, DB_MESSAGE_ID, attVal,
                            tupId)) < 0) {
    return -DMAQI_THERE_IS_NOT_THE_TUPLE;
  }
  if (dma_table_retr(nDbmsHandle, R_DACTIVE, tupId, &noAlign) < 0) {
    return -DMAQI_DB_BAD_TUPLE_ID;
  }
  // return Orignal Tuple Pointer in ACTVDB TO a argument Active Tuple Ptr.
  dma_align_make(nDbmsHandle, R_DACTIVE, (char *)spActive, noAlign);

  return FLK_SUCCESS;
}

int selectActiveRec(u_int unMessageId, stRDActiveTuple *spActive) {
  return selectGetActiveRec(DBMS_ACTIVE, unMessageId, spActive);
}

int selectReceiptRec(u_int unMessageId, stRDActiveTuple *spActive) {
  return selectGetActiveRec(DBMS_RECEIPT, unMessageId, spActive);
}

int selectTransformRec(u_int unMessageId, stRDActiveTuple *spActive) {
  return selectGetActiveRec(DBMS_TRANS, unMessageId, spActive);
}

int selectRcsRec(u_int unMessageId, stRDActiveTuple *spActive) {
  return selectGetActiveRec(DBMS_RCS, unMessageId, spActive);
}

int dma_getTuplePtr(int nDbmsHandle, int tableId, int attId, int tupId,
                    char *attVal, int attValSize, char **spTuplePtr) {
  if (nDbmsHandle >= MAX_DBMS_CNT || nDbmsHandle < 0) {
    spdlog::error("Invalid DBMS Type.[{}]", __func__);
    return FLK_FAIL;
  }

  if (gpAqiInfo[nDbmsHandle] == NULL) {
    spdlog::error("[{}] - Didn't connect MDB DMA[{}]!!", __func__, nDbmsHandle);
    return FLK_FAIL;
  }

  if (tableId < 0 || tableId >= gpAqiInfo[nDbmsHandle]->tableCnt) {
    spdlog::error("[{}]Invalid Table Id[{}].", __func__, tableId);
    return FLK_FAIL;
  }

  char cpAttVal[MAX_ATT_PER_QP]{};
  memcpy(cpAttVal, attVal,
         (MAX_ATT_PER_QP < attValSize) ? MAX_ATT_PER_QP : attValSize);

  int nTargetTupId = dma_getTupId(nDbmsHandle, tableId, attId, cpAttVal, tupId);
  if (nTargetTupId < 0) {
    return -DMAQI_THERE_IS_NOT_THE_TUPLE;
  }

  char *noAlign;
  if (dma_table_retr(nDbmsHandle, tableId, nTargetTupId, &noAlign) < 0) {
    return -DMAQI_DB_BAD_TUPLE_ID;
  }
  *spTuplePtr = (char *)noAlign;
  return FLK_SUCCESS;
}

/* TOD traffic control */
int dma_getTrafficControlTuplePtr(u_char *cpControlCode,
                                  stRDTrafficControlTuple **spTuple) {
  return dma_getTuplePtr(DBMS_PLUS, R_DTRAFFICCONTROL, DB_TOC_CONTROL_CODE, 0,
                         (char *)cpControlCode, DB_TOC_CONTROL_CODE_SIZE,
                         (char **)spTuple);
}

int select_smpp_message_id(u_char *smpp_message_key,
                           stSmppMessageTuple **spTuple) {
  return dma_getTuplePtr(DBMS_RCS, R_DSMPPMESSAGE, DB_SMPP_MESSAGE_KEY, 0,
                         (char *)smpp_message_key, DB_SMPP_MESSAGE_KEY_SIZE,
                         (char **)spTuple);
}

// SR220 pkg
// int InsertActiveMessage(stRDActiveTuple *spActive, int maxQueueSize, int
// sleepQueueTime)
int InsertActiveMessage(int nDbmsType, stRDActiveTuple *spActive,
                        int maxQueueSize, int sleepQueueTime) {
  int retryCount = 0;
  while (retryCount++ < INSERTDBMS_RETRY_MAXNUM) {
    if (isMdbOverflow(nDbmsType, R_DACTIVE, maxQueueSize) == false) {
      break;
    }

    usleep(sleepQueueTime * 1000);
  }

  if (retryCount >= INSERTDBMS_RETRY_MAXNUM) {
    spdlog::error("[{}:{}]-DBMS Queue Full-Retry[{}] fail", __func__, __LINE__,
                  retryCount);
    return FLK_FAIL;
  }

  if (GetMessageId(spActive) == 0) {
    spdlog::error("[{}][{}() : {}] Message Id is NULL", __FILE__, __func__,
                  __LINE__);
    return FLK_FAIL;
  }

  /**************** Active DB insertion ******************/
  Result stResult{};
  int result =
      db_insert(nDbmsType, R_DACTIVE, (char *)spActive, &stResult, NO_RESULT);
  if (result != 1) {
    /* primitive call fail::5::QP_MESG_SEND_FAIL */
    db_error(result);
    spdlog::error("[{}:{}]-db_insert fail[{}] !", __func__, __LINE__, result);
    return FLK_FAIL;
  }

  return FLK_SUCCESS;
}

int InsertRcsMessage(stRDActiveTuple *spActive, int maxQueueSize,
                     int sleepQueueTime) {
  int retryCount = 0;
  while (retryCount++ < INSERTDBMS_RETRY_MAXNUM) {
    if (isMdbOverflow(DBMS_RCS, R_DRCS, maxQueueSize) == false) {
      break;
    }

    usleep(sleepQueueTime * 1000);
  }

  if (retryCount >= INSERTDBMS_RETRY_MAXNUM) {
    spdlog::error("[{}:{}]-DBMS Queue Full-Retry[{}] fail", __func__, __LINE__,
                  retryCount);
    return FLK_FAIL;
  }

  if (GetMessageId(spActive) == 0) {
    spdlog::error("[{}][{}() : {}] Message Id is NULL", __FILE__, __func__,
                  __LINE__);
    return FLK_FAIL;
  }

  /**************** Active DB insertion ******************/
  Result stResult{};
  int result =
      db_insert(DBMS_RCS, R_DRCS, (char *)spActive, &stResult, NO_RESULT);
  if (result != 1) {
    /* primitive call fail::5::QP_MESG_SEND_FAIL */
    db_error(result);
    spdlog::error("[{}:{}]-db_insert fail[{}] !", __func__, __LINE__, result);
    return FLK_FAIL;
  }
  return FLK_SUCCESS;
}

MessageID GetMessageId(stRDActiveTuple *spActive) {
  MessageID messageId = 0;

  memcpy(&messageId, spActive->dbMessageID, sizeof(MessageID));
  return messageId;
}

/** @} */
