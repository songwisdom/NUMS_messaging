#ifndef __SMS_STAT_H__
#define __SMS_STAT_H__

#include "FLKBaseDef.h"

#define     LINK_CHECK_FILE     "/data/secmdb/chkLink"

#define     OID_VALUE_CLEAR         0xff

typedef enum {
	DB_SMSC_OID,
	DB_CUR_TIME,
	DB_CUR_VALUE,
	DB_OLD_TIME,
	DB_OLD_VALUE,
	DB_OID_VARIABLE_TYPE,
	DB_OID_VARIABLE_SIZE,
	DB_OID_SEQUENCE_MAX
} eSmsOidSequence_t;

typedef enum {
	DB_SMSC_OID_SIZE			= 24,
	DB_CUR_TIME_SIZE            = 4,
	DB_CUR_VALUE_SIZE           = 4,
	DB_OLD_TIME_SIZE            = 4,
	DB_OLD_VALUE_SIZE           = 4,
	DB_OID_VARIABLE_TYPE_SIZE   = 1,
	DB_OID_VARIABLE_SIZE_SIZE   = 1,
	DB_DDB_TIME_SIZE            = 4,
	DB_DDB_VALUE_SIZE           = 4
} eSmsOidMemberSize_t;


typedef enum {
	STATISTICST_DB_START_POINT	= 24,
	VARIABLE_POINT				= 6,
	OID_VARIABLE_SIZE			= 18
} eSmsOidOptPosition_t;

#define 	DB_HOST_SIZE		24

typedef struct {
	u_char       dbSmscOid[DB_SMSC_OID_SIZE];
	u_char       dbCurTime[DB_CUR_TIME_SIZE];
	u_char       dbCurValue[DB_CUR_VALUE_SIZE];
	u_char       dbOldTime[DB_OLD_TIME_SIZE];
	u_char       dbOldValue[DB_OLD_VALUE_SIZE];
	u_char       dbVariableType[DB_OID_VARIABLE_TYPE_SIZE];
	u_char       dbVariableSize[DB_OID_VARIABLE_SIZE_SIZE];
	u_char		dbDdbTime[DB_DDB_TIME_SIZE];
	u_char		dbDdbValue[DB_DDB_VALUE_SIZE];
} stSmsStatistics;

#define     TIME_INTERVAL       5
#define     UPDATE_TIME_INTERVAL    10


/* oid variable type*/
#define     OID_NONE_TYPE       0
#define     OID_SME_TYPE        1
#define     OID_SMEID_TYPE      2
#define     OID_POINTCODE_TYPE  3
#define     OID_ERRORCODE_TYPE  4

/* table id */

#define     OID_SET_VALUE(c, a, b)  c[a]=b
#define     OID_ZERO_LEVEL      0
#define     OID_ONE_LEVEL       1
#define     OID_TWO_LEVEL       2


#define     OID_ZERO_VALUE_DBMS0            0x10
#define     OID_ZERO_VALUE_DBMS1            0x20

#define     OID_ZERO_VALUE_ASE              0x30
#define     OID_ZERO_VALUE_SCHD             0x40
#define     OID_ZERO_VALUE_UHIS             0x50

#define 	OID_ZERO_VALUE_ESME				0x60

/* will be deleted */
/*
#define     OID_ZERO_VALUE_RSMDP            0x60
#define     OID_ZERO_VALUE_SSMDP            0x70
*/

#define     OID_ZERO_VALUE_DDB              0x80
#define     OID_ZERO_VALUE_TCAP             0x90

#define     OID_ZERO_VALUE_SMDPS_SMDPS      0x61
#define     OID_ZERO_VALUE_SMDPS_SMCI       0x62
#define     OID_ZERO_VALUE_SMDPS_DNIP       0x63
#define     OID_ZERO_VALUE_SMDPS_MBNK       0x64
#define     OID_ZERO_VALUE_SMDPS_RSMDP      0x65
#define     OID_ZERO_VALUE_SMDPS_RSMPP      0x66
#define     OID_ZERO_VALUE_SMDPS_RSMPP34    0x67
#define     OID_ZERO_VALUE_SMDPS_SSMDP      0x68
#define     OID_ZERO_VALUE_SMDPS_SSMPP      0x69
#define     OID_ZERO_VALUE_SMDPS_SSMPP34    0x6a


#define     OID_ONE_VALUE_DBMS0_INSERT_SUCCESS  0x00
#define     OID_ONE_VALUE_DBMS0_INSERT_FAIL     0x01
#define     OID_ONE_VALUE_DBMS0_DELETE_SUCCESS  0x02
#define     OID_ONE_VALUE_DBMS0_DELETE_FAIL     0x03

#define     OID_ONE_VALUE_UHIS_INSERT_SUCCESS    0x00
#define     OID_ONE_VALUE_UHIS_INSERT_FAIL       0x01
#define     OID_ONE_VALUE_UHIS_SEND_SUCCESS      0x02
#define     OID_ONE_VALUE_UHIS_SEND_FAIL         0x03

#define     OID_ONE_VALUE_DBMS1_INSERT_SUCCESS  0x00
#define     OID_ONE_VALUE_DBMS1_INSERT_FAIL     0x01
#define     OID_ONE_VALUE_DBMS1_DELETE_SUCCESS  0x02
#define     OID_ONE_VALUE_DBMS1_DELETE_FAIL     0x03

#define     OID_ONE_VALUE_SCHD_RETRY_ALGO_FINISH    0x00
#define     OID_ONE_VALUE_SCHD_RETRY_SLEEP          0x01
#define     OID_ONE_VALUE_SCHD_TIME_EXPIRED         0x02
#define     OID_ONE_VALUE_SCHD_SCHD_ST_SMSREQ_S     0x03
#define     OID_ONE_VALUE_SCHD_SCHD_ST_RIFSM_S      0x03
#define		OID_ONE_VALUE_SCHD_SCHD_ST_RECEIPT_S	0x04
/*PlusCheckAndSendBMCH() */
#define		OID_ONE_VALUE_SCHD_SCHD_ST_BMCH_S		0x05
#define		OID_ONE_VALUE_SCHD_SCHD_ST_NPQC_S		0x06 /*sendMsgIdToNPQC()*/
#define     OID_ONE_VALUE_SCHD_ERROR_CODE           0x07

#define     OID_ONE_VALUE_UHIS_SUCCESS              0x00
#define     OID_ONE_VALUE_UHIS_FAIL                 0x01
/* will be deleted */
/*
#define     OID_ONE_VALUE_RSMDP_SUCCESS             0x00
#define     OID_ONE_VALUE_RSMDP_FAIL                0x01
*/

#define     OID_ONE_VALUE_SSMDP_SUCCESS             0x00
#define     OID_ONE_VALUE_SSMDP_FAIL                0x01
/* will be deleted */
#define 	OID_ONE_VALUE_SMPP_RECV_SUCCESS			0x00
#define 	OID_ONE_VALUE_SMPP_RECV_FAIL			0x01
#define		OID_ONE_VALUE_SMPP_SEND_SUCCESS			0x02
#define 	OID_ONE_VALUE_SMPP_SEND_FAIL			0x03

#define     OID_ONE_VALUE_SMCI_RECV_SUCCESS			0x04
#define		OID_ONE_VALUE_SMCI_RECV_FAIL			0x05
#define		OID_ONE_VALUE_SMCI_SEND_SUCCESS			0x06
#define 	OID_ONE_VALUE_SMCI_SEND_FAIL			0x07

/* ASE --> HLR, MSC */
#define     OID_ONE_VALUE_SEND_MSC_SUCCESS          0x00
#define     OID_ONE_VALUE_SEND_MSC_FAIL             0x01
#define     OID_ONE_VALUE_RECV_MSC_SUCCESS          0x02
#define     OID_ONE_VALUE_RECV_MSC_FAIL             0x03

#define     OID_ONE_VALUE_MO_IVK_SUCCESS        	0x04
#define     OID_ONE_VALUE_MO_IVK_FAIL           	0x05

#define     OID_ONE_VALUE_SEND_HLR_SUCCESS          0x06
#define     OID_ONE_VALUE_SEND_HLR_FAIL             0x07
#define     OID_ONE_VALUE_RECV_HLR_SUCCESS          0x08
#define     OID_ONE_VALUE_RECV_HLR_FAIL             0x09

#define		OID_ONE_VALUE_RECV_ALERT_SUCCESS		0x0a
#define 	OID_ONE_VALUE_RECV_ALERT_FAIL			0x0b

#define 	OID_ONE_VALUE_SEND_REPORT_SUCCESS		0x0c
#define 	OID_ONE_VALUE_SEND_REPORT_FAIL			0x0d

#define 	OID_ONE_VALUE_RECV_REPORT_SUCCESS		0x0e
#define 	OID_ONE_VALUE_RECV_REPORT_FAIL			0x0f

#define 	OID_ONE_VALUE_SEND_INFORM_SUCCESS		0x10
#define 	OID_ONE_VALUE_SEND_INFORM_FAIL			0x11


#define     OID_ONE_VALUE_RSMSNOT_IVK_SUCCESS       0x12
#define     OID_ONE_VALUE_RSMSNOT_IVK_FAIL          0x13

/* TCAP --> Statistics */
/* Component Type */
#define     OID_ONE_VALUE_TC_INVOKE_REQ				0x00
#define		OID_ONE_VALUE_TC_RESULT_REQ				0x01
#define		OID_ONE_VALUE_TC_ERROR_REQ				0x02
#define		OID_ONE_VALUE_TC_REJECT_REQ				0x03
#define     OID_ONE_VALUE_TC_INVOKE_IND				0x04
#define		OID_ONE_VALUE_TC_RESULT_IND				0x05
#define		OID_ONE_VALUE_TC_ERROR_IND				0x06
#define		OID_ONE_VALUE_TC_REJECT_IND				0x07
/* Primitive Type */
#define		OID_ONE_VALUE_TC_BEGIN_REQ				0x08
#define		OID_ONE_VALUE_TC_CONTINUE_REQ			0x09
#define		OID_ONE_VALUE_TC_END_REQ				0x0a
#define		OID_ONE_VALUE_TC_UNIDIR_REQ				0x0b
#define		OID_ONE_VALUE_TC_PABORT_REQ				0x0c
#define		OID_ONE_VALUE_TC_UABORT_REQ				0x0d
#define		OID_ONE_VALUE_TC_BEGIN_IND				0x0e
#define		OID_ONE_VALUE_TC_CONTINUE_IND			0x0f
#define		OID_ONE_VALUE_TC_END_IND				0x10
#define		OID_ONE_VALUE_TC_UNIDIR_IND				0x11
#define		OID_ONE_VALUE_TC_PABORT_IND				0x12
#define		OID_ONE_VALUE_TC_UABORT_IND				0x13

#define     OID_ONE_VALUE_SEND_SCP_SUCCESS          0x0e
#define     OID_ONE_VALUE_SEND_SCP_FAIL             0x0f
#define     OID_ONE_VALUE_RECV_SCP_SUCCESS          0x10
#define     OID_ONE_VALUE_RECV_SCP_FAIL             0x11

#define     OID_ONE_VALUE_SEND_ESME_SUCCESS          0x12
#define     OID_ONE_VALUE_SEND_ESME_FAIL             0x13


#define     OID_POINTCODE_SIZE              2
#define     SECDBMS_STATISTICST_DB_INSERT_FAIL 401

#endif



