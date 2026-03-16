#ifndef __DBAQI_H__
#define __DBAQI_H__

#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/fcntl.h>

//#define MAX_DMAQI_ERR   50
//#define MAX_LOAD_FACTOR 4.0

#include <MdbCommon.h>

//#define INTEGER     0

//#define MAXBUFF   20
//#define ERROR     -1

/*
#define DB_HLRESN   1
*/
//#define DB_CountUpdateCount     15

/*
 *dmaqi error Number
 */
#define STRING_COMPARE_FAIL             201
#define SHARED_DB_ID_GET_FAIL           202
#define SHARED_DB_ATTACH_FAIL           203
#define SHARED_INDEX_ID_GET_FAIL        204
#define SHARED_INDEX_ATTACH_FAIL        205
#define SHARED_DIRTY_ID_GET_FAIL        206
#define SHARED_DIRTY_ATTACH_FAIL        207
#define RECOVERY_SLD_IMAGE_FAIL         208

#define DMAQI_DB_BAD_TABLE_ID           209
#define DMAQI_DB_BAD_TUPLE_ID           210
#define DMAQI_THERE_IS_NOT_THE_TUPLE    211
#define DB_BAD_ATTRIBUTE_ID             212
#define DB_ATTRIBUTE_VAL_NULL           213
#define TABLE_RETRIEVAL_FAIL            214
#define UPDATE_COUNT_INVALID            215
#define DMAQI_KEY_ATTRIBUTE_UPDATE      216
#define DMAQI_ESN_MISMATCH              217
#define TUPLE_UPDATE_FAIL               218
#define DMAQI_UNRECOGNIZED_MIN          219
/*
#define DMAQI_MIN_HLR_MISMATCH          220
*/

#define DMAQI_DB_TABLE_DETACH_ERR       221
#define DMAQI_DIRTY_TABLE_DETACH_ERR    222
#define DMAQI_INDEX_TABLE_DETACH_ERR    223
#define UPDATE_STATUS                   224

#define SMDC_DBMS_PS_NAME               "sms_dbms"
#define DMAQI_MIN_MASK_LINK_DELETE_FAIL 301
#define DMAQI_SEND_MSGID_TO_UHIS_FAIL   302
#define DMAQI_SET_MIN_MASK_STATUS_FAIL   303
#endif

