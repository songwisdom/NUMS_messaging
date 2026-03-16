#ifndef __MDB_COMMON_H__
#define __MDB_COMMON_H__

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "FLKBaseDef.h"
//#include "MdbMessage.h"
//#include "MdbDataStruct.h"


#define ERROR                       -1
#define DIRTY_OFF                   0x00
#define DIRTY_ON                    0x0f
#define BLOCK_SIZE                  4096
#define SHMIND_SIZE                 24148516


#define MAX_TABLE                   16
#define MAX_MEMDB_RECORD            2500000
#define MAX_ELEMENT_COUNT           MAX_MEMDB_RECORD*MAX_TABLE
#define MAX_HASH_COUNT              MAX_MEMDB_RECORD*MAX_TABLE

/*
 * SHM MSG QUEUE SIZE
 */
#define SHMMSGQ_SIZE                500000
#define SHMMSGQ_DIVISION            3

#define MAX_ELEMENT_SIZE            101000
#define MULTI_COUNT                 3

#define MAX_ATT_PER_UPDATE          150

#define MAX_ATTRIBUTE_SIZE          256
#define MAX_KEY_IN_A_T_NODE         16
#define MAX_CURSORS                 256

#define DIRECTORY_SIZE              256
#define SEGMENT_SIZE                1024

#define PRIME_NEW_KEY 				37
#define PRIME_BURKET_SIZE 			(DIRECTORY_SIZE * SEGMENT_SIZE)

#define MAX_ML_HASH                 16
/* baram MAX_HASH_KEY_SIZE*/
//#define MAX_HASH_KEY_SIZE           12
#define MAX_HASH_KEY_SIZE           36 // R910 PKG 24 -> 36
#define ML_HASH_OVER_HEAD           30
#define T_TREE_OVER_HEAD            30
#define MIN_HEAP_SIZE               1024
#define MAX_LOAD_FACTOR 			4.0
#define	FILLED_TUPLE_SPACE			-2

/*backup*/
#define MDBB_FILE_OPEN_FAIL     1
#define MDBB_SHM_GET_FAIL       2
#define MDBB_SHM_ATTATCH_FAIL   3
#define MDBB_SHM_STAT_FAIL      4
#define MDBB_FILE_WRITE_FAIL    5
#define MDBB_SLD_FILE_LOCKED    6
#define MDBB_SLD_FILE_LOCK_FAIL 7
#define MDBB_SLD_FILE_UNLOCK_FAIL   8

#define     FILE_NOT_EXIST              -10
#define     MDB_CONNECT_FAIL            -11
#define     MDB_INSERT_FAIL             -12
#define     MDB_DELETE_FAIL             -13
#define     MDB_SELECT_FAIL             -14
#define     MDB_RETR_RDIR_FAIL          -15
#define     MDB_RETR_RDIC_FAIL          -16

#define     MAX_ERR_NUM                50
#define     MAX_MSGQ_RETRY              10
/*
 * error number
 */
#define     CHECK_ENVIRONMENT_ERROR                         0x01
#define     CHECK_COMMAND_ERROR                             0x02
#define     INIT_IPC_ERROR                                  0x03
#define     INIT_MDB_ERROR                                  0x04
#define     INPUT_MQ_CREATE_FAIL                            0x05
#define     OUTPUT_MQ_CREATE_FAIL                           0x06
#define     INPUT_MQ_REMOVED_DURING_MSGRCV                  0x07
#define     MSG_READ_ERROR_ON_INPUT_MQ                      0x08
#define     INSERT_FAIL_ON_R_DACTIVE                        0x09
#define     DELETE_FAIL_ON_R_DACTIVE                        0x0a
#define     INVALID_COMMAND                                 0x0b
#define     CANNOT_CONSTRUCT_RDACTIVETUPLE                  0x0c
#define     ERROR_OF_CONVERSION_FROM_TIME_TO_STRING         0x0d
#define     MEMORY_ALLOC_ERROR                              0x0e
#define     SELECT_FAIL_ON_R_DACTIVE                        0x0f
#define     SEND_ERROR_MSG_ID_TO_SMSREQ                     0x10
#define     OUTPUT_MQ_GET_FAIL                              0x11

/* qmi */
#define NOT_EQU                         0x00001000
#define EXTRA_UPDATE                    0x00010000
#define EXTRA2_UPDATE                   0x00020000
#define EXTRA3_UPDATE                   0x00030000
#define ECHO_UPDATE                     0x00040000

/*--------  extern variable declaration --------*/
extern FILE *log_fp;
extern char *netCode;
extern int dbErrorCode, msr_flag, dbms_qid, socket_qid;

/*--------  extern function declaration --------*/
/*--- main.c ---*/
//extern int makeShm();	
//extern int compare();

/*--- operation.c ---*/
//extern int query_process();
//extern int query_process_multi();
//extern int getMachineNo();

/*--- table.c ---*/
//extern int table_init();
//extern int table_retr();
//extern int table_modi();
//extern int table_insert();
//extern int table_delete();
//extern void align_make();
//extern void align_remove();
//extern int recovery_SLD_image();

/*--- dml.c ---*/
//extern int db_selectTup();
//extern int db_updateTup();
//extern int db_deleteTup();
//extern int db_insertTup();
//extern int db_countTup();
//extern int db_retrRdic();
//extern int db_retrRdir();
//extern void msgPutToUre();

/*--- hash.c ---*/
//extern int ShmBuildHash();
//extern int buildHash();
//extern int hash_make();
//extern int hash_search();
//extern int hash_delete();
//extern int hash_insert();

/*------------  DBMS error message -------------*/
#define settingErrorCode(errorCode) (dbErrorCode=(errorCode))
#ifdef _ISMSC_
#define ATTR_PTR(tuple,tableId,attId) ((char *)((tuple)+mAttDir[(tableId)][(attId)].offset))
#else
#define ATTR_PTR(attDir,tuple,tableId,attId) ((char *)((tuple)+attDir[(tableId)][(attId)].offset))
#define PLUS_ATTR_PTR(attDir,tuple,tableId,attId) ((char *)((tuple)+attDir[(tableId)][(attId)].offset))
#endif

#define  ATTRPTR(tuple,tableId,attId) ((char *)((tuple)+attInfoDir[(tableId)][(attId)].offset))
#define  ATTRPTR2(tuple,tableId,attId) ((char *)((tuple)+attInfoDir2[(tableId)][(attId)].offset))
#define  MAX_NUM_OF_DBMS_ERROR          200
#define  THERE_IS_NOT_THE_TUPLE         1
#define  FILENAME_IS_INVALID            2

#define  DB_ETC                         3
#define  DB_BAD_TABLE_ID                4
#define  DB_BAD_ATT_ID                  5
#define  DB_BAD_TUPLE_ID                6
#define  DB_PTR_NULL                    7

#define  NO_EMPTY_T_TREE               10
#define  INVALID_T_TREE_ID             11
#define  BOUNDING_ERROR                12
#define  SEARCH_ERROR_IN_T_NODE        13
#define  T_TREE_INSERT_KEY_EXIST       14
#define  CURSOR_IS_NULL                15
#define  CURRENT_T_NODE_IS_NULL        16
#define  INVALID_LEFT_OR_RIGHT         17
#define  T_NODE_IS_NULL                18
#define  KEY_IS_NULL                   19
#define  INVALID_TUPLE_ID              20
#define  INVALID_KEY_COUNT             21
#define  INVALID_MIN_OR_MAX            22
#define  PARENT_IS_NULL                23

#define  SHMGET_FAILURE                24
#define  STAT_FAILURE                  25
#define  OPEN_FAILURE                  26
#define  SHMAT_FAILURE                 27
#define  INVALID_SEARCH_METHOD         28
#define  DML_INIT_INPUT_NULL           29
#define  MOD_ATT_NUM_INVALID           30

#define  KEY_ATTRIBUTE_DUPLICATE       31
#define  TOO_MANY_ML_HASH              32
#define  INVALID_INDEX_ID              33
#define  KEY_NOT_EXIST                 34
#define  ML_HASH_INSERT_EXIST          35

#define  TABLE_INIT_INPUT_NULL         36
#define  NO_TUP_RDIR_IS_LARGE          37
#define  INVALID_TABLE_ID              38
#define  TABLE_INFO_NULL               39
#define  TUPLE_VALUE_IS_NULL           40
#define  ALLOC_TUPLE_SPACE_FAIL        41
#define  ADDR_TO_TUPLE_PTR_FAIL        42

#define  SLD_LOADING_FAILURE           43
#define  NOT_SLD_FILE                  44
#define  CURSOR_NOT_AVAILABLE          45
#define  INVALID_CURSOR_ID             46
#define  SHMCTL_FAILURE                47
#define  SEQ_TABLE_SEARCH_FAIL         48
#define  NO_MORE_FREE_TUPLE            49
#define  WRITE_FAILURE                 50

#define  FILENAME_IS_NULL              51
#define  FLAG_IS_INVALID               52
#define  TOO_MANY_ATTRIBUTE            53
#define  TOO_LARGE_TUPLE_SIZE          54
#define  TOO_LARGE_ATT_SIZE            55

#define  SLD_CNTL_DATA_INCONSIS        56
#define  DB_PUT_TUP__P_TUP_ID__NULL    57
#define  DB_PUT_TUP__TUP_VAL__NULL     58
#define  TABLE_INSERT__P_TUP_ID__NULL  59
#define  TABLE_INSERT__PP_TUP_VAL__NULL      60
#define  DB_UPD_TUP__TUP_VAL__NULL           61
#define  DB_GET_TUP_HASH__KEY__NULL          62
#define  DB_GET_TUP_HASH__P_TUP_ID__NULL     63
#define  DB_GET_TUP_HASH__TUP_VAL__NULL      64
#define  DB_GET_TUP_DIR__TUP_VAL__NULL       65
#define  DB_GET_TUP_TREE__KEY__NULL          66
#define  DB_GET_TUP_TREE__P_TUP_ID__NULL     67
#define  DB_GET_TUP_TREE__TUP_VAL__NULL      68
#define  DB_GET_TUP_SEQ__KEY__NULL           69
#define  DB_GET_TUP_SEQ__P_TUP_ID__NULL      70

#define  DB_GET_TUP_SEQ__TUP_VAL__NULL       71
#define  UNDEFINED                           72

#define  INVALID_SHM_ID                      73
#define  LOADING_FAILURE                     74

#define  INVALID_OPERATION           80
#define  THERE_IS_NOT_AVAILABLE_BUCKET       81
#define  KEY_ATTRIBUTE_UPDATE            82
#define  MSN_MISMATCH                    83
#define  DB_BAD_SELECT_ATT_ID            84
#define  DB_ATT_VAL__NULL                    85
#define  THERE_IS_NOT_AVAILABLE_NODE         86
#define  HASH_VALUE__INVALID                 87
/*
#define  UNRECOGNIZED_MIN               88
#define  MIN_HLR_MISMATCH                    89
*/
/*
 * min index db error.
 */
#define MIN_MASK_DB_INSERT_FAIL     88
#define MIN_MASK_DB_DELETE_FAIL     89
#define DBMS_GET_MACHINE_NO_FAIL    90
#define INIT_DBMSQ_FAIL             91
#define SLD_RECOVERY_FAIL           92
#define TABLE_INIT_FAIL             93
#define GET_CONFIG_FAIL             94
#define INIT_STATISTICS_FAIL        95

#define QP_APPLICATION_QUEUE_GET_FAIL           201
#define QP_DBMS_QUEUE_GET_FAIL                  202
#define QP_TCP_QUEUE_GET_FAIL                   203
#define QP_MESG_RECV_FAIL                       204
#define QP_MESG_SEND_FAIL                       205

//#define QP_DBMS_QUEUE_FULL					206		//-- Plus New.

#define QP_INVALID_MACHINE_NO                   206
#define QP_NOT_SETUP                            207
#define QP_INVALID_TABLE_ID                     208
#define QP_INVALID_ATT_ID                       209
#define QP_ATTVAL_NULL_POINTER                  210
#define QP_UPD_LIST_NULL_POINTER                211
#define QP_TUPLE_NULL_POINTER                   212
#define QP_INVALID_APPLICATION_QUEUE_KEY        213
#define QP_INVALID_TCP_QUEUE_KEY                214

#define INVALID_EVENT_NAME                      215
#define TCP__IPC_TIME_OUT                       216
#define TARGET_DBMS_CONNECT_FAIL                217
#define TARGET_DBMS_IS_NOT_VALID                218
#define INVALID_MACHINE_NO__TARGET              219
#define INVALID_MACHINE_NO__SOURCE              220
#define GET_MACHINE_NO_FAIL                     221
#define TARGET_DBMS_CONNECT_FAIL_RECV           222
#define TARGET_DBMS_CONNECT_FAIL_SEND           223
#define QP_UPD_CNT_OVER_LIMIT                   224
#define QP_MSGQ_OVERFLOW						225
#define QP_MSGQ_INFO_GET_FAIL					226

/* IN */
#define IN_SERVICE_BLOCK        0x02
#define IN_SERVICE_NO_BLOCK     0x01
#define IN_PRIMARY_SECOND       0x01
#define IN_LOAD_BLANCE          0x02

// Add plus DBMS, from old version mdb header db_table.h
typedef enum {
	RETR_EQ = 0,
	RETR_NEQ,
	RETR_GREAT,
	RETR_LESS
} eMdbReturnType_t;
/*
#define INTEGER               0
#define STRING                1

#define SEQUENT               1
#define HASH                  2
#define TREE                  3
#define BOTH                  4
*/

typedef enum {
	INTEGER = 0, 
	STRING
} eAttributeType_t;

typedef enum {
	SEQUENT = 1,
	HASH,
	TREE,
	BOTH
} eNodeLinkType_t;

#define FILLED_TUPLE_SPACE    -2

// Check Backup.
typedef enum {
	BACKUP_OK	= 'a',
	NO_BACKUP	= 'b',
	LOAD_PREHIS	= 'c'
} eBackupStatus_t;

typedef enum {
	PREHIS_CREATED		= 'P',
	PREHIS_NOT_CREATED	= 'p'
} ePreHisCreate_t;

typedef enum {
	CKBAKUP_BACKUP  = 0,
	CKBAKUP_PREHIS  = 1,
	CKBAKUPSIZE     = 1		// 2(backup+prehis) before the integration version.
} eCkBackupEachValue;

#define MAX_DMAQI_ERR   50
#define MAX_LOAD_FACTOR 4.0

//#define INTEGER     0

#define MAXBUFF   20
//#define ERROR     -1

#define	DB_CountUpdateCount		15

typedef enum
{
    DBMS_STAT,
    DBMS_ACTIVE,
    DBMS_RECEIPT,
    DBMS_PLUS,
	DBMS_TRANS,
	DBMS_RCS,
    MAX_DBMS_CNT
} eDbmsType_t;

//int MaxMDBRecord; 
/* CkBakup.c */
/*
int checkBackup();
void setLoadPreHis();
void BackupOk();
void LoadOk();
int checkLoading();
void resetLoadPreHis();
FILE *openCkBakupr();
FILE *openCkBakupw();
void 	getCkBakupPath(char *cpCkBakup);
*/
/* dbUtil.c */
/*
int backupMdb( char *cpSldfileName, char *cpDbPtr, int nSize);
void setErr( int eno );
int copyData( int fd, char *cpDbPtr, int nSize );
void clearTableData( int tableId );
void deleteTableData( int tableId );
*/

/* dbms_err.c */
//void setErrString();
/* directInsert.c */
/*
int db_delete_direct(int table_id, int attId, int tupId, char *attVal, int attVal_size);
int db_insert_direct(int table_id, void* tuple, size_t tuple_size);
int mdbInsertOne(char* stTuple, int tableId, int nSize);
int db_direct_deleteTup(Delete *delete, Delete_Ack *ack);
*/

/* dml.c */
/*
int checkMdbStatus( int	tableId, int *npTupCnt );
int getTupId(int tableId, int attId, char *attVal, int tupId, int sel_type);
int getTuplePtr(int tableId, int attId, int tupId, char *attVal, int attValSize, char **spTuplePtr);
int db_selectTup(Select *select, Select_Ack *ack, int mode);
int db_updateTup(Update *update, Select_Ack *ack, int mode);
int db_deleteTup(Delete *delete, Delete_Ack *ack, int mode);
int db_insertTup(Insert *insert, Insert_Ack *ack);
int db_countTup(Count *count, Count_Ack *ack);
int db_retrRdic(Meta_Rdic *meta, Meta_Rdic_Ack *ack);
int db_retrRdir( Meta_Rdir *meta, Meta_Rdir_Ack *ack );
int db_insertUpdateTup( Insert *insert, Insert_Ack *ack );
*/

/* hash.c */

/*
int buildHash( int	tableId, int attId );
int hash_make(int tableId, int attId, int indexId);
int hash_search( int	indexId, char	*key);
int hash_delete( int indexId, char *key );
int hash_insert(int indexId, char *key, int tupId);
Element * makeElem( int	elmCnt );
*/

/* link.c */
/*
int InitActiveHead(int nTableId);
int InitTransformHead(int nTableId);
int db_insertActiveLink(int nTableId, int unMsgId,int nStatus);
int Mdb_MakeLink(int nTableId, stRDReceiptTuple *spActive, int unMsgId, int BMsgId, int FMsgId);
int del_link( int nTableId, stRDReceiptTuple *spActive );
int db_deleteLink(int nTableId, int unMsgId);
int db_insertTransformLink(int nTableId, int unMsgId);
int db_deleteTransformLink(int nTableId, int unMsgId);
*/
/* loadMysqlDb.c */
/* main.c */
/* operation.c */
/* table.c */
/* trigger.c */
typedef struct {
	long dbms_adr;
	long index_adr;
	long dirty_adr;
}DBMS_ADDR_ST;
int set_mdb_shm_addr(int dbms_idx, char *dbms_adr, char *index_adr, char *dirty_adr);
int read_dbms_addr(int dbms_idx, DBMS_ADDR_ST *addr);
#endif

