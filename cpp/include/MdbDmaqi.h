#ifndef __MDBDMAQI_H__
#define __MDBDMAQI_H__

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/fcntl.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include <FLKBaseDef.h>
#include <MdbCommon.h>
#include <MdbDataStruct.h>
#include <MdbMessage.h>
#include <MdbTableInfo.h>
#include <StatRecord.h>
#include <StatTable.h>

#include "flk_type.h"
#include <libMapDmaqi.h>
/*
#define MAX_DMAQI_ERR   50
#define MAX_LOAD_FACTOR 4.0

#define INTEGER     0

#define MAXBUFF   20
#define ERROR     -1
*/

/*
#define DB_HLRESN   1
#define DB_CountUpdateCount     15
*/

typedef struct {
  int nSemKey;
  int nSemId;
  int nSemNum;
} stSemArray;

// #pragma pack 1
typedef struct _sttAqiInfo {
  char *netCode;
  Rdir *relationDir[MAX_TABLE];
  Index *indexDir[MAX_TABLE];
  stSemArray semId4IDX;
  stSemArray semIdDir[MAX_TABLE];

  AttDir attDir[MAX_TABLE][MAX_ATT_PER_TABLE];
  int Hash_id[MAX_TABLE][MAX_ATT_PER_TABLE];
  int tableCnt;
  int totAtt[MAX_TABLE];
  BIT_T shiftCount[MAX_TABLE][MAX_TUPLE_SIZE];

  char *db_start_ptr;
  char *table_ptr;
  char *index_ptr;
  char *tuple_ptr[MAX_TABLE];
  DirtyFlagTable *pDirtyFlagTable;

  Rdir *rdirPtr[MAX_TABLE];
  int needAlign[MAX_TABLE];
} sttAqiInfo;
// #pragma pack

// extern sttAqiInfo* gpAqiInfo[MAX_DBMS_CNT];

#define dma_tupleId2ptr(rdirPtr, tableId, tupId)                               \
  ((char *)(rdirPtr[tableId]->rel_adr +                                        \
            (tupId) * (rdirPtr[tableId]->tupleSize + sizeof(int))))

/*
#define dma_tupleId2ptr(pAqiInfo, tableId, tupId) \
                        ((char *)(pAqiInfo->rdirPtr[(tableId)]->rel_adr + \
                        (tupId) *
(pAqiInfo->rdirPtr[(tableId)]->tupleSize+sizeof(int))))
*/

/*
extern char *dmaqi_ErrString[];


extern int connectDbDmaqi(int);
extern int disconnectDbDmaqi(int);
extern int mdb_dma_select();
extern int mdb_dma_update();

static char *dma_Digit2Str();
extern int dma_table_retr(int, int, int, char **);
extern int dma_compare(char *, char *, int);

static void dma_set_meta_database(sttAqiInfo *);
static void dma_set_meta_index(sttAqiInfo *);

BIT_T dma_hash1(char *, int);
Element *dma_get_bucket_with_key(sttAqiInfo *, int, char *);
extern int dma_getTupId(int, int, int, char *, int);
char	*dma_tupleId2tuplePtr(int, int, int);
int 	dma_charStr2int(char *);
void 	dma_dirtyFlag_set(sttAqiInfo *, char *, int);
static int dma_addr2blockNum(sttAqiInfo *, char *);

static void dma_align_init(sttAqiInfo *);
extern void dma_align_remove(int, int, char *, char *);
extern void dma_align_make(int, int, char *, char *);
*/

/*
 * map search func.
 */
// int             comparePattern();

/* StatRecUtil.c */
int MakeStatisticsRecord(stStatisticsRecord *spStsRecord, u_short RecordType,
                         u_short TryInterface, u_short TryRole,
                         u_short TryDirection, u_short TryEvent,
                         u_short TryDataLen, u_int TryTime, u_int TryResult,
                         u_int MsgID, u_short ProcessID, u_short ServiceID,
                         u_short GroupID, char *SME);

// DR610 LSH Service type
typedef enum {
  IMMEDIATELY_SERVICE = 1,
  REALTIME_MDB_BACK = 2
} Service_Control_Type;
#ifdef __cplusplus
// extern "C" {
#endif

int DebugStatisticsRecord(stStatisticsRecord *spStsRecord);
int LogStatisticsRecord(stStatisticsRecord *spStsRecord);
int PrintStatisticsRecord(stStatisticsRecord *spStsRecord);
void SET_STSRECORD_STRGROUPID(char *cpTarget, char *cpSource);
void setTwoVarValue(stStatisticTuple *spTuple, u_char *ucGroup, int nGroupCnt,
                    int nType, int nTypeValue, int nCurValue);
/* mdbDirecetInsert.c */
int mdb_delete_direct(int nDbmsType, int table_id, int attId, int tupId,
                      char *attVal, int attVal_size);
int mdb_insert_direct(int nDbmsType, int table_id, void *tuple,
                      size_t tuple_size);
/* mdbDmaqiEx.c */
int disconnectDbDmaqi(int nDbmsHandle);
int InitDbDmaqi4Stat(sttAqiInfo *pAqiInfo);
int connectDbDmaqi(int nDbmsType);
int dma_recovery_SLD_image(char *db_image);
int isMdbOverflow(int nDbmsHandle, int nTableId, int nLimit);
int isMdbUnderflow(int nDbmsHandle, int nTableId, int nLimit);
int db_dma_getrdir(int nDbmsHandle, Meta_Rdir *meta, Meta_Rdir_Ack *ack);
void db_dma_error(int err_no);
int mdb_dma_select(int nDbmsHandle, Select *select, Select_Ack *ack, int mode);
int mdb_dma_update(int nDbmsHandle, Update *update, Select_Ack *ack, int mode);
int dma_getTupId(int nDbmsHandle, int tableId, int attId, char *attVal,
                 int tupId);
// int dma_hash_print(sttAqiInfo *pAqiInfo, int indexId, char *key);
int dma_hash_search(sttAqiInfo *pAqiInfo, int indexId, char *key);
Element *dma_get_bucket_with_key(sttAqiInfo *pAqiInfo, int indexId, char *key);
unsigned dma_hash1(const char *key, int len);
int dma_table_retr(int nDbmsHandle, int tableId, int tupId, char **pTupVal);
int dma_table_modi(int nDbmsHandle, int tableId, int tupId, char **ppTupVal,
                   char *tupVal);
void dma_dirtyFlag_set(sttAqiInfo *pAqiInfo, char *pStart, int nSize);
char *dma_tupleId2tuplePtr(int nDbmsHandle, int tableId, int tupId);
int dma_charStr2int(char *charStr);
int dma_compare(char *p1, char *p2, int n);
int compKey(char *p1, char **p2, int n);
int comparePattern(char *p1, char *p2, int n);
void dma_align_make(int nDbmsHandle, int tableId, char *to, char *from);
void dma_align_remove(int nDbmsHandle, int tableId, char *to, char *from);
int dma_ActiveTuplePtr(int nDbmsHandle, u_int unMessageId,
                       stRDActiveTuple **spActive);
int dma_SubmitTuplePtr(int nDbmsHandle, u_int unMessageId,
                       stRDActiveTuple **spActive);
int dma_getActiveTuplePtr(u_int unMessageId, stRDActiveTuple **spActive);
int dma_getReceiptTuplePtr(u_int unMessageId, stRDActiveTuple **spActive);
int dma_getRcsTuplePtr(u_int unMessageId, stRDActiveTuple **spActive);
int selectGetActiveRec(int nDbmsHandle, u_int unMessageId,
                       stRDActiveTuple *spActive);
int selectActiveRec(u_int unMessageId, stRDActiveTuple *spActive);
int selectReceiptRec(u_int unMessageId, stRDActiveTuple *spActive);
int selectTransformRec(u_int unMessageId, stRDActiveTuple *spActive);
int selectRcsRec(u_int unMessageId, stRDActiveTuple *spActive);
int dma_getTuplePtr(int nDbmsHandle, int tableId, int attId, int tupId,
                    char *attVal, int attValSize, char **spTuplePtr);
MessageID GetMessageId(stRDActiveTuple *spActive);

/* TrafficControlHandler.c */
int dma_getTrafficControlTuplePtr(u_char *cpControlCode,
                                  stRDTrafficControlTuple **spTuple);

int select_smpp_message_id(u_char *smpp_message_key,
                           stSmppMessageTuple **spTuple);

// SR220 pkg
// int InsertActiveMessage(stRDActiveTuple *spActive, int maxQueueSize, int
// sleepQueueTime);
int InsertActiveMessage(int nDbmsType, stRDActiveTuple *spActive,
                        int maxQueueSize, int sleepQueueTime);

int InsertRcsMessage(stRDActiveTuple *spActive, int maxQueueSize,
                     int sleepQueueTime);

/* mdbDmaqiUtil.c */
u_int getLengthStr(u_char *str, int nSize);
int getNumOfDigit(u_char *str, int nSize);
void CallBackDigit2Str(u_char *ucDigit, u_char *ucChar, int nSize);
void MINDigit2Str(u_char *ucDigit, u_char *ucChar, int nSize);
void str2MINDigit(u_char *cpStr, u_char *cpMinDigit, int nMinDigitLen);
void digit2char(u_char *ucDigit, u_char *ucChar, int nSize);
void str2DTMF(char *cpStr, u_char *cpDtmf);
int checkMqStatFunc(int nQid);
// int dma_sql_select(int table_id, int att_id, char *attval, int tup_id, Result
// result, int flag);
/* mdbErrStr.c */
void Dmaqi_setErrString();
void QP_ErrString();
void Dbms_ErrString(void);
void dbStatisticError(int err_no);
/* mdbProcStat.c */
// static char * hex2str(u_char *cpTarget,int nTargetSize, u_char *cpSource, int
// nSourceSize);

// SR124 BMT sdbms use : insert and update
int dbStatisticUpdates(stStatisticTuple *spStatisticsTuple,
                       stStatisticTuple *spStatisticsDBTuple, int nTableId);
// SR124 BMT statdm use : update and insert queue(sdbms)
int dbStatisticMerge(stStatisticTuple *spStatisticsTuple, int nTableId);

int dbStatisticProcessStartRecord(stStatisticsRecord *spStsRecord);
int dbStatisticProcessEndRecord(stStatisticsRecord *spStsRecord);
int dbStatisticInsertbyRecord(stStatisticsRecord *spStsRecord);
void SetDeliverTime(stRDActiveTuple *spActive);
u_int DeliverTimeInterval(stRDActiveTuple *spActive);
u_int IncomeTimeInterval(time_t tIvk_time);
/* mdbQmi.c */
int db_connect(int nDbmsType, long ltype);
int db_insert(int nDbmsHandle, int table_id, void *tuple, Result *result,
              int flag);
int db_delete(int nDbmsHandle, int table_id, int att_id, char *attval,
              int tup_id, Result *result, int flag, int event);
int db_dml(int nDbmsHandle, int table_id, int att_id, char *attval,
           int attval_size, int tup_id, Result *result, int flag, int event);
int db_dml_stat_update(int nDbmsHandle, int table_id, int att_id, char *attval,
                       int attval_size, int tup_id, Result *result, int flag,
                       int event, int stat_Q_seq);
// int db_dml(int nDbmsHandle, int table_id, int att_id, char *attval, int
// tup_id, Result *result, int flag, int event);
int db_seq_update(int nDbmsHandle, int table_id, int tup_id, int upd_cnt,
                  char *upd_list, void *tuple, Result *result, int flag);
int db_update(int nDbmsHandle, int table_id, int att_id, char *attval,
              int tup_id, int upd_cnt, char *upd_list, void *tuple,
              Result *result, int flag);
void db_error(int err_no);

/* TrafficControl.c */
int dma_getTrafficControlTuplePtr(u_char *cpControlCode,
                                  stRDTrafficControlTuple **spTuple);

/*mdbDirecetInsert.c*/

#ifdef __cplusplus
//}
#endif

/*
 *dmaqi error Number
 */
#define STRING_COMPARE_FAIL 201
#define SHARED_DB_ID_GET_FAIL 202
#define SHARED_DB_ATTACH_FAIL 203
#define SHARED_INDEX_ID_GET_FAIL 204
#define SHARED_INDEX_ATTACH_FAIL 205
#define SHARED_DIRTY_ID_GET_FAIL 206
#define SHARED_DIRTY_ATTACH_FAIL 207
#define RECOVERY_SLD_IMAGE_FAIL 208

#define DMAQI_DB_BAD_TABLE_ID 209
#define DMAQI_DB_BAD_TUPLE_ID 210
#define DMAQI_THERE_IS_NOT_THE_TUPLE 211
#define DB_BAD_ATTRIBUTE_ID 212
#define DB_ATTRIBUTE_VAL_NULL 213
#define TABLE_RETRIEVAL_FAIL 214
#define UPDATE_COUNT_INVALID 215
#define DMAQI_KEY_ATTRIBUTE_UPDATE 216
#define DMAQI_ESN_MISMATCH 217
#define TUPLE_UPDATE_FAIL 218
#define DMAQI_UNRECOGNIZED_MIN 219
/*
#define DMAQI_MIN_HLR_MISMATCH          220
*/

#define DMAQI_DB_TABLE_DETACH_ERR 221
#define DMAQI_DIRTY_TABLE_DETACH_ERR 222
#define DMAQI_INDEX_TABLE_DETACH_ERR 223
#define UPDATE_STATUS 224

#define SMDC_DBMS_PS_NAME "sms_dbms"
#define DMAQI_MIN_MASK_LINK_DELETE_FAIL 301
#define DMAQI_SEND_MSGID_TO_UHIS_FAIL 302
#define DMAQI_SET_MIN_MASK_STATUS_FAIL 303

/* FR220 EsmeId Queue Check */
/*StatEsmeQ.c */
#define STAT_ESMEID_SYSID_SIZE 16
#define STAT_ESMEID_QUEUE_INFO_MAX 300
typedef struct {
  char sysid[STAT_ESMEID_SYSID_SIZE];
  int EsmeId;
  short EsmeQueueType;
  short count;
} stStatEsmeIdQRecord;
typedef struct {
  int EsmeIdCount;
  stStatEsmeIdQRecord EsmeIdQCntInfo[STAT_ESMEID_QUEUE_INFO_MAX];
} stStatEsmeIdQInfo;

stStatEsmeIdQInfo *statEsmeIdMsgQCount();
void statEsmeIdMsgQRecv(long key, u_int esme_id);
void statEsmeIdMsgQSend(long key, u_int esme_id);

// FR230 special type
typedef enum {
  CONVERT_SPECIAL_MIN = 1,
  CALLBACK_NOTI_EXCEP_MIN = 2,
  FACE_TIME_MIN = 3, // FR240 FaceTime
  ESME_PARALLEL_SVC = 4,
  MMS_NOTI_TID_ROUTE = 5,
  SPAM_FILTERING_TID = 6,
  SPAM_FILTERING_CID = 7,
  IPMG_ROUTING_TID = 8,
  SMS_EPE_RELINK_TID = 9,
  NPA_MIN_SVC = 10,
  MO_LIMIT_SPAM_FILTERING_TID = 11, // FR810 pkg limit spam tid
  TZ_SMS_TID = 12,                  // FR810 pkg Tz sms spam tid
  MO_LIMIT_SPAM_FILTERING_CID = 13, // FR810 pkg limit spam cid
  FAIL_REASON_LOG_PRINT = 14,       // SR320 pkg
  M2M_GLR_ROUTING_MIN = 15,         // SR410 pkg
  M2M_GLR_ROUTING_DENY_MIN = 16,    // SR410 pkg
  M2M_GLR_ROUTING_ALLOW_MIN = 17,   // SR410 pkg
  RSM_M2M_SERVICE = 18,             // R710_HWJ pkg
  MCS_APPLY_TID = 19,               // R910
  MCS_DENY_CID = 20                 // R910
} Convert_Special_Type;
bool isDestMinParallelsSvc(u_char *dbDestMin);
bool isSpamFilteringTID(u_short usTid);
bool isSpamFilteringCID(u_char *cpCID, int length);
bool isMoLimitSpamFilteringTID(u_short usTid);             // FR810 pkg
bool isMoLimitSpamFilteringCID(u_char *cpCID, int length); // FR810 pkg
bool isTzSmsTID(u_short usTid);                            // FR810 pkg
bool isIPMGRouteTid(u_short Tid);
bool isEpeRelinkTid(u_short Tid);
bool isMmsNotiRoute(char *cpTeleSvcId, int *nRouteInfo);
bool isNPACheck(u_char *Npa);
bool is_mcs_apply_tid(u_short usTid);
bool is_mcs_deny_cid(u_char *cpCID, int length);
int selectSvcRouteID(u_short unServiceCode, stRDServiceIdTuple *spTuple);
int selectVerifyCharset(u_int in_dcs, u_int in_charset, u_int out_dcs,
                        u_int *out_chartset);
int selectServiceControl(u_short unServiceCode,
                         stRDServiceControlTuple *record);
int selectNpdbRN(u_char *ucMin, int nMinOctetNum,
                 stRDNpdbRnRouteTuple *spPrefix);

int selectConfigText(u_int unServiceCode, stRDTextConfigData *record);
int selectConfigTextByCode(u_short usGroupCode, u_short usServiceType,
                           stRDTextConfigData *record);
int updateConfigText(stRDTextConfigData *record);
int insertConfigText(stRDTextConfigData *record);
int deleteConfigText(u_int unServiceCode);
bool is_callback_deny_expt_src_min(u_char *ucMin); // FR610 HJY
bool isFailReasonLogPrint(u_short usTid);          // SR320 pkg
#endif
