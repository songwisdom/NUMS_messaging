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
*   FILE NAME       :   mdbDmaqiErr.c
*   PURPOSE         :
*   DATE OF WRITE   :   2002.03.01
*   AUTHOR          :
*   REVISION        :   V 1.0
*   Ver.    By      When        Reason
*   ------- ------- ----------  -----------------------------------------
*   1.0             2002.03.01  first coding
******************************************************************************/


#include <MdbDmaqi.h>
#include <MdbCommon.h>

/* MAX_DMAQI_ERR is 50 */
const char    *dmaqi_ErrString[MAX_DMAQI_ERR];

void Dmaqi_setErrString()
{
	/* DMAQI error string setting */
	dmaqi_ErrString [1]   = "STRING_COMPARE_FAIL" ;
	dmaqi_ErrString [2]   = "SHARED_DB_ID_GET_FAIL" ;
	dmaqi_ErrString [3]   = "SHARED_DB_ATTACH_FAIL" ;
	dmaqi_ErrString [4]   = "SHARED_INDEX_ID_GET_FAIL" ;
	dmaqi_ErrString [5]   = "SHARED_INDEX_ATTACH_FAIL" ;
	dmaqi_ErrString [6]   = "SHARED_DIRTY_ID_GET_FAIL" ;
	dmaqi_ErrString [7]   = "SHARED_DIRTY_ATTACH_FAIL" ;
	dmaqi_ErrString [8]   = "RECOVERY_SLD_IMAGE_FAIL" ;
	dmaqi_ErrString [9]   = "DB_BAD_TABLE_ID" ;
	dmaqi_ErrString [10]   = "DB_BAD_TUPLE_ID" ;
	dmaqi_ErrString [11]   = "THERE_IS_NOT_THE_TUPLE" ;
	dmaqi_ErrString [12]   = "DB_BAD_ATTRIBUTE_ID" ;
	dmaqi_ErrString [13]   = "DB_ATTRIBUTE_VAL_NULL" ;
	dmaqi_ErrString [14]   = "TABLE_RETRIEVAL_FAIL" ;
	dmaqi_ErrString [15]   = "UPDATE_COUNT_INVALID" ;
	dmaqi_ErrString [16]   = "KEY_ATTRIBUTE_UPDATE" ;
	dmaqi_ErrString [17]   = "ESN_MISMATCH" ;
	dmaqi_ErrString [18]   = "TUPLE_UPDATE_FAIL" ;
	dmaqi_ErrString [19]   = "DMAQI_UNRECOGNIZED_MIN";
	dmaqi_ErrString [20]   = "DMAQI_MIN_HLR_MISMATCH";

	dmaqi_ErrString [21]   = "DMAQI_DB_TABLE_DETACH";
	dmaqi_ErrString [22]   = "DMAQI_DIRTY_TABLE_DETACH";
	dmaqi_ErrString [23]   = "DMAQI_INDEX_TABLE_DETACH";

}


#define MAX_QP_ERR_COUNT	100

const char	*dbms_ErrString[MAX_NUM_OF_DBMS_ERROR];
const char	*qp_ErrString[MAX_QP_ERR_COUNT];

void QP_ErrString()
{
	/* QP error string setting */
	qp_ErrString [1]    = "QP_APPLICATION_QUEUE_GET_FAIL" ;
	qp_ErrString [2]    = "QP_DBMS_QUEUE_GET_FAIL" ;
	qp_ErrString [3]    = "QP_TCP_QUEUE_GET_FAIL" ;
	qp_ErrString [4]    = "QP_MESG_RECV_FAIL" ;
	qp_ErrString [5]    = "QP_MESG_SEND_FAIL" ;
	qp_ErrString [6]    = "QP_INVALID_MACHINE_NO" ;
	qp_ErrString [7]    = "QP_NOT_SETUP" ;
	qp_ErrString [8]    = "QP_INVALID_TABLE_ID" ;
	qp_ErrString [9]    = "QP_INVALID_ATT_ID" ;
	qp_ErrString [10]   = "QP_ATTVAL_NULL_POINTER" ;
	qp_ErrString [11]   = "QP_UPD_LIST_NULL_POINTER" ;
	qp_ErrString [12]   = "QP_TUPLE_NULL_POINTER" ;
	qp_ErrString [13]   = "QP_INVALID_APPLICATION_QUEUE_KEY" ;
	qp_ErrString [14]   = "QP_INVALID_TCP_QUEUE_KEY" ;
	qp_ErrString [15]   = "INVALID_EVENT_NAME" ;
	qp_ErrString [16]   = "TCP__IPC_TIME_OUT" ;
	qp_ErrString [17]   = "TARGET_DBMS_CONNECT_FAIL" ;
	qp_ErrString [18]   = "TARGET_DBMS_IS_NOT_VALID" ;
	qp_ErrString [19]   = "INVALID_MACHINE_NO__TARGET" ;
	qp_ErrString [20]   = "INVALID_MACHINE_NO__SOURCE" ;
	qp_ErrString [21]   = "GET_MACHINE_NO_FAIL" ;
	qp_ErrString [22]   = "TARGET_DBMS_CONNECT_FAIL_RECV" ;
	qp_ErrString [23]   = "TARGET_DBMS_CONNECT_FAIL_SEND" ;
}

void Dbms_ErrString(void)
{

	/* DBMS error string setting */
	dbms_ErrString [1]  = "THERE_IS_NOT_THE_TUPLE" ;
	dbms_ErrString [2]  = "FILENAME_IS_INVALID" ;
	dbms_ErrString [3]  = "DB_ETC" ;
	dbms_ErrString [4]  = "DB_BAD_TABLE_ID" ;
	dbms_ErrString [5]  = "DB_BAD_ATT_ID" ;
	dbms_ErrString [6]  = "DB_BAD_TUPLE_ID" ;
	dbms_ErrString [7]  = "DB_PTR_NULL" ;

	dbms_ErrString [10] = "NO_EMPTY_T_TREE" ;
	dbms_ErrString [11] = "INVALID_T_TREE_ID" ;
	dbms_ErrString [12] = "BOUNDING_FAILURE" ;
	dbms_ErrString [13] = "SEARCH_FAILURE_IN_T_NODE" ;
	dbms_ErrString [14] = "T_TREE_INSERT_KEY_EXIST" ;
	dbms_ErrString [15] = "CURSOR_IS_NULL" ;
	dbms_ErrString [16] = "CURRENT_T_NODE_IS_NULL" ;
	dbms_ErrString [17] = "INVALID_LEFT_OR_RIGHT" ;
	dbms_ErrString [18] = "T_NODE_IS_NULL" ;
	dbms_ErrString [19] = "KEY_IS_NULL" ;
	dbms_ErrString [20] = "INVALID_TUPLE_ID" ;
	dbms_ErrString [21] = "INVALID_KEY_COUNT" ;
	dbms_ErrString [22] = "INVALID_MIN_OR_MAX" ;
	dbms_ErrString [23] = "PARENT_IS_NULL" ;
	dbms_ErrString [24] = "SHMGET_FAILURE" ;
	dbms_ErrString [25] = "STAT_FAILURE" ;
	dbms_ErrString [26] = "OPEN_FAILURE" ;
	dbms_ErrString [27] = "SHMAT_FAILURE" ;
	dbms_ErrString [28] = "INVALID_SEARCH_METHOD" ;
	dbms_ErrString [29] = "DML_INIT_INPUT_NULL" ;
	dbms_ErrString [30] = "MOD_ATT_NUM_INVALID" ;
	dbms_ErrString [31] = "KEY_ATTRIBUTE_DUPLICATE" ;
	dbms_ErrString [32] = "TOO_MANY_ML_HASH" ;
	dbms_ErrString [33] = "INVALID_INDEX_ID" ;
	dbms_ErrString [34] = "KEY_NOT_EXIST",
    dbms_ErrString [35] = "ML_HASH_INSERT_EXIST" ;
	dbms_ErrString [36] = "TABLE_INIT_INPUT_NULL" ;
	dbms_ErrString [37] = "NO_TUP_RDIR_IS_LARGE" ;
	dbms_ErrString [38] = "INVALID_TABLE_ID" ;
	dbms_ErrString [39] = "TABLE_INFO_NULL" ;
	dbms_ErrString [40] = "TUPLE_VALUE_IS_NULL" ;
	dbms_ErrString [41] = "ALLOC_TUPLE_SPACE_FAIL" ;
	dbms_ErrString [42] = "ADDR_TO_TUPLE_PTR_FAIL" ;
	dbms_ErrString [43] = "READ_FAILURE" ;
	dbms_ErrString [44] = "NOT_SLD_FILE" ;
	dbms_ErrString [45] = "CURSOR_NOT_AVAILABLE" ;
	dbms_ErrString [46] = "INVALID_CURSOR_ID" ;
	dbms_ErrString [47] = "SHMCTL_FAILURE" ;
	dbms_ErrString [48] = "SEQ_TABLE_SEARCH_FAIL" ;
	dbms_ErrString [49] = "NO_MORE_FREE_TUPLE" ;
	dbms_ErrString [50] = "WRITE_FAILURE" ;
	dbms_ErrString [51] = "FILENAME_IS_NULL" ;
	dbms_ErrString [52] = "FLAG_IS_INVALID" ;
	dbms_ErrString [53] = "TOO_MANY_ATTRIBUTE" ;
	dbms_ErrString [54] = "TOO_LARGE_TUPLE_SIZE" ;
	dbms_ErrString [55] = "TOO_LARGE_ATT_SIZE" ;
	dbms_ErrString [56] = "SLD_CNTL_DATA_INCONSIS" ;
	dbms_ErrString [57] = "DB_PUT_TUP__P_TUP_ID__NULL" ;
	dbms_ErrString [58] = "DB_PUT_TUP__TUP_VAL__NULL" ;
	dbms_ErrString [59] = "TABLE_INSERT__P_TUP_ID__NULL" ;
	dbms_ErrString [60] = "TABLE_INSERT__PP_TUP_VAL__NULL" ;
	dbms_ErrString [61] = "DB_UPD_TUP__TUP_VAL__NULL" ;
	dbms_ErrString [62] = "DB_GET_TUP_HASH__KEY__NULL" ;
	dbms_ErrString [63] = "DB_GET_TUP_HASH__P_TUP_ID__NULL" ;
	dbms_ErrString [64] = "DB_GET_TUP_HASH__TUP_VAL__NULL" ;
	dbms_ErrString [65] = "DB_GET_TUP_DIR__TUP_VAL__NULL" ;
	dbms_ErrString [66] = "DB_GET_TUP_TREE__KEY__NULL" ;
	dbms_ErrString [67] = "DB_GET_TUP_TREE__P_TUP_ID__NULL" ;
	dbms_ErrString [68] = "DB_GET_TUP_TREE__TUP_VAL__NULL" ;
	dbms_ErrString [69] = "DB_GET_TUP_SEQ__KEY__NULL" ;
	dbms_ErrString [70] = "DB_GET_TUP_SEQ__P_TUP_ID__NULL" ;
	dbms_ErrString [71] = "DB_GET_TUP_SEQ__TUP_VAL__NULL" ;
	dbms_ErrString [72] = "UNDEFINED" ;
	dbms_ErrString [73] = "INVALID_SHM_ID" ;
	dbms_ErrString [74] = "LOADING_FAILURE" ;

	dbms_ErrString [80] = "INVALID_OPERATION" ;
	dbms_ErrString [81] = "THERE_IS_NOT_AVAILABLE_BUCKET" ;
	dbms_ErrString [82] = "KEY_ATTRIBUTE_UPDATE" ;
	dbms_ErrString [83] = "MSN_MISMATCH" ;
	dbms_ErrString [84] = "DB_BAD_SELECT_ATT_ID" ;
	dbms_ErrString [85] = "DB_ATT_VAL__NULL";
	dbms_ErrString [86] = "THERE_IS_NOT_AVAILABLE_NODE";
	dbms_ErrString [87]	= "HASH_VALUE__INVALID";
	dbms_ErrString [88]	= "MIN_MASK_DB_INSERT_FAIL";
	dbms_ErrString [89] = "MIN_MASK_DB_DELETE_FAIL";
	dbms_ErrString [90] = "DBMS_GET_MACHINE_NO_FAIL";
	dbms_ErrString [91] = "INIT_DBMSQ_FAIL";
	dbms_ErrString [92] = "SLD_RECOVERY_FAIL";
	dbms_ErrString [93] = "TABLE_INIT_FAIL";
	dbms_ErrString [94] = "GET_CONFIG_FAIL";
	dbms_ErrString [95] = "INIT_STATISTICS_FAIL";
}


void dbStatisticError(int err_no)
{
	int ix;

	if (err_no >= 0 || err_no <= -300) {
		return;
	}
	if (err_no < -200) {
		ix = (-err_no) - 200;
		if (!qp_ErrString[ix]) {
			return;
		}
	} else if (err_no < 0) {
		ix = (-err_no);
		if (!dbms_ErrString[ix]) {
			return;
		}
	}
}
/** @} */
