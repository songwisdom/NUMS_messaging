/***********************************************************************
*
*   Copyright (c) 1998 LG Information & Communications
*   All rights reserved.
*
*   This program contains confidential and proprietary information of
*   LGIC, and any reproduction, disclosure, or use in whole or in part
*   is expressly  prohibited, except as may be specifically authorized
*   by prior written agreement or permission of LGIC.
*
*               LG Information & Communications, Ltd.
*        395-65 , Shindaebang-dong, Dongjak-gu, Seoul, 150-010 Korea
*
***********************************************************************/

/*---------------------------------------------------------------------
 *
 *  FILE     NAME   : SMSC Error Code Define
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
----------------------------------------------------------------------*/


#ifndef     _EERROR_CODE_H_
#define     _EERROR_CODE_H_


/***********************************************************************
        SMSC Error Define

        Composite 4 byte
            first 1 byte :: H/W or S/W
            next  1 byte :: Process
            next  1 byte :: Error Level
            next  1 byte :: serial number
************************************************************************
************************************************************************

            first byte :
                            S/W        	: aa
                            H/W         : bb
            next  byte :
                            each nameR      0xcc010d02

            next  byte :
                            Critical    : 0x0A
                            Major       : 0x0B
                            Minor       : 0x0C
                            Normal      : 0x0D
            next  byte :
                            Sequence number

            macro name :[SCC/SDC/MAS/COM]_[PROCESS]_[STAT]

***********************************************************************

***********************************************************************
        COM  Process Code : ykkim 1998
***********************************************************************

				COM		0x01
				MSQ		0x02
				SCK		0x03
				DB		0x04

***********************************************************************
        SMDC Process Code :
***********************************************************************

				DBMS	0x10
				DBH		0x20
				UHIS	0x30
				ASE		0x40
				SMDPS	0x50
				RSMDP	0x60
				SSMDP	0x70
                SCHD    0x80
				DBBACK	0x90
				PURGED	0xA0
				SECDB	0xB0
				NIF		0xC0
				CSMS	0xD0
				DUAL    0xE0

***********************************************************************
        MAS Process Code : tsJung 1998.08.22
***********************************************************************

				DISK	0x10
				CPU		0x20
				EHTER	0x30
				SS7		0x40
				LINKDM	0x50
				DELEDM	0x60
				EVENTDM	0x70
				SYSMON	0x70
				ALARMD	0x80
				PRNTDM  0x90
				CDRDM0  0xa0
				CDRTRD  0xb0

***********************************************************************/

/***********************************************************************
	SMDC ERROR CODE
***********************************************************************/

#define		DBMS_DOWN					0xaa100a00
#define 	DBMS_STARTING_ERROR     	0xaa100b01
#define 	DBMS_MSG_INSERT_ERROR   	0xaa100b02
#define 	DBMS_INSERT_ERROR   		0xaa100d03
#define 	DBMS_DELETE_ERROR   		0xaa100d04
#define 	DBMS_UPDATE_ERROR   		0xaa100d05
// SMS Plus
#define     PLUSDBMS_DOWN               0xaa100a06
#define     PLUSDBMS_STARTING_ERROR     0xaa100a07

// DDB ERROR REASON
#define DDB_INSERT_ERROR                0xa030

//CSMS
#define     CSMS_DOWN               0xaad00b10
#define     CSMS_START              0xaad00f10
//CSMS SR320
#define     CSMS_MAX_CONNECTION_ERROR  0xaad00a10
#define     CSMS_RESULT_MSG_ERROR      0xaad00a11
#define     CSMS_RESULT_SYS_FAULT      0xaad00a12

#define 	MME_DISCONNECT 			0xaaf00000
#define 	CSCF_DISCONNECT			0xaaf00200
#define 	REDIS_DISCONNECT		0xaaf00300
#define 	REDIS_OVERCONNECT		0xaaf00301
#define 	REDIS_OVERMEMORY		0xaaf00302
#define 	REDIS_OVERDBSIZE		0xaaf00303
#define 	NTP_DOWN 				0xaaf00100
#define 	DGW_DOWN 				0xaaf00a00
#define 	HLR_DOWN 				0xaaf00b00
#define 	HLR_GRP_OUT_DOWN		0xaaf00b01
#define 	HLR_GRP_IN_DOWN			0xaaf00b02
#define 	KBELL_STACK_DOWN		0xaaf00c00
#define 	DBAGENT_DOWN    		0xaaf00d00
//SR410 pkg
#define 	DGW_MASTER_DOWN 		0xaaf00a10

//SR420 pkg
#define 	AUTORUN_FLAG_DOWN 		0xaaf00a20
#define 	PROCESS_COUNT_MISSMATCH 	0xaaf00a30

typedef enum {
	ASE_ALARM_BASE = 0xaa400a00,
	ASE001_DOWN,		//0xaa400a01
	ASE002_DOWN,		//0xaa400a02
	ASE003_DOWN,		//0xaa400a03
	ASE004_DOWN,		//0xaa400a04
	ASE005_DOWN,		//0xaa400a05
	ASE006_DOWN,		//0xaa400a06
	ASE007_DOWN,		//0xaa400a07
	ASE008_DOWN,		//0xaa400a08
	ASE009_DOWN,		//0xaa400a09
	ASE010_DOWN,		//0xaa400a0a
	ALL_ASE_DOWN,		//0xaa400a0b
	ASE_ALARM_MAX		//0xaa400a0c
} eAseAlarmOffice;

#define SIP_MASTER_DOWN 		0xaa400a10
#define SIP_PROXY_DOWN 			0xaa400a11
#define SIP_STACK_DOWN 			0xaa400a12

typedef enum {
	ISC_ALARM_BASE = 			0xaa400a20,
	ISC001_DOWN,				//0xaa400a21,
	ISC002_DOWN,				//0xaa400a22,
	ISC003_DOWN,				//0xaa400a23,
	ISC004_DOWN,				//0xaa400a24,
	ISC_ALARM_MAX				//0xaa400a25,
} eISCAlarmOffice;

#define SIP_OPTION_DOWN 		0xaa400a30

typedef enum {
	SH_PROC_ALARM_BASE = 		0xaa400a40,
	SH0001_DOWN,				//0xaa400a41
	SH0002_DOWN,				//0xaa400a42
	SH0003_DOWN,				//0xaa400a43
	SH0004_DOWN,				//0xaa400a44
	SH_PROC_ALARM_MAX,				//0xaa400a45
	SH_SES_ALARM_CLOSE_BASE = 	0xaa400a50,
	SH0001_SES_CLOSE,			//0xaa400a51
	SH0002_SES_CLOSE,			//0xaa400a52
	SH0003_SES_CLOSE,			//0xaa400a53
	SH0004_SES_CLOSE			//0xaa400a54
} eShAlarmOffice;

#define ISCEXP_DOWN 			0xaa400a60

#define SCHD_POOL_ROUTING_DOWN 		0xaa400a70
#define SCHD_POOL_UDM_CAPA_DOWN 	0xaa400a71
#define SCHD_POOL_UDM_GROUP_DOWN	0xaa400a72
#define SCHD_POOL_CSCF_CAPA_DOWN 	0xaa400a73

typedef enum {
	SGD_ALARM_BASE = 0xaa400ab0,
	SGD001_DOWN,		// 0xaa400ab1,
	SGD002_DOWN,		// 0xaa400ab2,
	SGD003_DOWN,		// 0xaa400ab3,
	SGD004_DOWN,		// 0xaa400ab4,
	SGD005_DOWN, // Reserved		// 0xaa400ab5,
	SGD006_DOWN, // Reserved		// 0xaa400ab6,
	SGD007_DOWN, // Reserved		// 0xaa400ab7,
	SGD008_DOWN, // Reserved		// 0xaa400ab8,
	ALL_SGD_DOWN,		// 0xaa400ab9,
	SGD_ALARM_SESSION_CLOSE_BASE = 0xaa400ac0,
	SGD001_SESSION_CLOSE,		// 0xaa400ac1,
	SGD002_SESSION_CLOSE,		// 0xaa400ac2,
	SGD003_SESSION_CLOSE,		// 0xaa400ac3,
	SGD004_SESSION_CLOSE,		// 0xaa400ac4,
	SGD005_SESSION_CLOSE, // Reserved		// 0xaa400ac5,
	SGD006_SESSION_CLOSE, // Reserved		// 0xaa400ac6,
	SGD007_SESSION_CLOSE, // Reserved		// 0xaa400ac7,
	SGD008_SESSION_CLOSE, // Reserved		// 0xaa400ac8,
	SGD_ALARM_MAX		// 0xaa400ac9,
} eMmeAlarmOffice;

typedef enum {
	S6C_ALARM_BASE = 0xaa400ad0,
	S6C001_DOWN,		//0xaa400ad1,
	S6C002_DOWN,		//0xaa400ad2,
	S6C003_DOWN,		//0xaa400ad3,
	S6C004_DOWN,		//0xaa400ad4,
	ALL_S6C_DOWN,		//0xaa400ad5,
	S6C_ALARM_SESSION_CLOSE_BASE = 0xaa400ae0,
	S6C001_SESSION_CLOSE,			// 0xaa400ae1
	S6C002_SESSION_CLOSE,			// 0xaa400ae2
	S6C003_SESSION_CLOSE,			// 0xaa400ae3
	S6C004_SESSION_CLOSE,			// 0xaa400ae4
	S6C_ALARM_MAX			// 0xaa400ae5
} eDiaHlrAlarmOffice;

typedef enum {
	ASE_ALARM_ERROR_BASE = 0xaa400b00,
	ASE001_ERROR,			// 0xaa400b01,
	ASE002_ERROR,			// 0xaa400b02,
	ASE003_ERROR,			// 0xaa400b03,
	ASE004_ERROR,			// 0xaa400b04,
	ASE005_ERROR,			// 0xaa400b05,
	ASE006_ERROR,			// 0xaa400b06,
	ASE007_ERROR,			// 0xaa400b07,
	ASE008_ERROR,			// 0xaa400b08,
	ASE009_ERROR,			// 0xaa400b09,
	ASE010_ERROR			// 0xaa400b0a,
} eAseErrorAlarmOffice;

typedef enum {
	DPC_ALARM_ERROR_BASE = 0xaa400d00,
	DPC_ERROR
} eDpcErrorAlarmOffice;



/***********************************************************************
 *     INTL ERROR CODE
************************************************************************/

typedef enum {
	E_POOL_ROUTING_UDM_PRIFIX_ERROR = 0xaa400f00, 	// 2856324864
	E_POOL_ROUTING_UDM_GROUP_DOWN_ERROR, 			// 2856324865
	E_POOL_ROUTING_CSCF_GROUP_DOWN_ERROR,			// 2856324866
	E_IPSMGW_ACCEPT_PREFIX_ERROR, 					// 2856324867
	E_POOL_ROUTING_ASE_TO_POOL_ERROR, 				// 2856324868
	E_POOL_ROUTING_SGD_TO_POOL_ERROR, 				// 2856324869
	E_POOL_ROUTING_ISC_TO_POOL_ERROR, 				// 2856324870
	E_POOL_ROUTING_POOL_TO_S6C_ERROR, 				// 2856324871
	E_POOL_ROUTING_POOL_TO_SH_ERROR, 				// 2856324872 
	E_SIP_PROXY_TO_STACK_ERROR, 					// 2856324873
	E_SIP_STACK_TO_ISC_ERROR, 						// 2856324874
	E_SIP_STACK_TO_OPT_ERROR, 						// 2856324875
	E_SIP_STACK_TO_PROXY_ERROR, 					// 2856324876
	E_SIP_ISC_TO_STACK_ERROR, 						// 2856324877
	E_SIP_OPT_TO_STACK_ERROR, 						// 2856324878 
	E_SCHD_TO_ISC_ERROR, 							// 2856324879
	E_POOL_ROUTING_POOL_TO_ISC_ERROR, 				// 2856324880
	E_UDM_TO_ISC_ERROR, 							// 2856324881 
	E_SIP_MSG_TO_POOL, 								// 2856324882 msg no register
	E_SIP_REGI_OVERLOAD_CTRL, 						// 2856324883
	E_SIP_MSG_OVERLOAD_CTRL, 						// 2856324884
	E_SIP_BODY_LEN_FAIL, 							// 2856324885
	E_SIP_METHOD_FAIL 								// 2856324886
} eInternalErrorCode;

/*
enum {
    DPC_ALARM_ERROR_BASE = 0xaaf00000,
    DPC_ERROR           = 0xaaf10000
} eDpcErrorAlarmOffice;
*/

#define     PARALLELS_DOWN      0xaa400c00
#define     MDB_BACKUP_DOWN      0xaa400c01

#define     DBH_DOWN            0xaa200a01
#define     UHIS_DOWN           0xaa300a01
#define		CDR_DOWN			0xaa400e00
// blue007kyj insert
#define     RUHIS_DOWN           0xaa300a02
#define     CUHIS_DOWN           0xaa300a03

typedef enum {
	SCHD_MGR_DOWN		= 0xaa800a00,
	SCHD_SNDMSG_DOWN,   //0xaa800a01
	SCHD_RESERV_DOWN,   //0xaa800a02
	SCHD_EXPIRE_DOWN,   //0xaa800a03
	SCHD_VERIFY_DOWN,   //0xaa800a04
	SCHD_SRETRY_DOWN,   //0xaa800a05
	SCHD_GRETRY_DOWN,   //0xaa800a06
	SCHD_RECEIPT_DOWN,   //0xaa800a07
	SCHD_SCHDS7_DOWN	= 0xaa800b07
} eSchedulerDownAlarm;

typedef enum {
	SCHD_ASNDMSG_DOWN	= 0xaa800a00,
	SCHD_ASRETRY_DOWN, // = 0xaa800a01,
	SCHD_AGRETRY_DOWN, // = 0xaa800a02,
	SCHD_AEXPIRE_DOWN, // = 0xaa800a03,
	SCHD_AVERIFY_DOWN, // = 0xaa800a04,
	SCHD_ASRESRV_DOWN, // = 0xaa800a05,
	SCHD_AGRESRV_DOWN, // = 0xaa800a06,
	SCHD_RSNDMSG_DOWN, // = 0xaa800a07,
	SCHD_RSRETRY_DOWN, // = 0xaa800a08,
	SCHD_RGRETRY_DOWN, // = 0xaa800a09,
	SCHD_REXPIRE_DOWN, // = 0xaa800a0a,
	SCHD_RVERIFY_DOWN, // = 0xaa800a0b,
	SCHD_RSRESRV_DOWN, // = 0xaa800a0c,
	SCHD_RGRESRV_DOWN, // = 0xaa800a0d,
	SCHD_TSNDMSG_DOWN, // = 0xaa800a0e,
	SCHD_TEXPIRE_DOWN, // = 0xaa800a0f,
	SCHD_RCSASND_DOWN, // = 0xaa800a10,
	SCHD_RCSAVRF_DOWN, // = 0xaa800a11,
	SCHD_RCSSRTY_DOWN, // = 0xaa800a12,
	SCHD_RCSGRTY_DOWN, // = 0xaa800a00,
	SCHD_CEXPIRE_DOWN, // = 0xaa800a14,
	SCHD_CSRESRV_DOWN, // = 0xaa800a15,
	SCHD_CGRESRV_DOWN, // = 0xaa800a16,
	SCHD_SEXPIRE_DOWN, // = 0xaa800a17,
	SCHD_PEXPSUB_DOWN  // = 0xaa800a18

} eDASchedulerDownAlarm;





#define		MAKECDR_DOWN		0xaad00b20
/****
#define     PURGED_DOWN         0xaaa00a01
****/
#define     ENMS_DOWN           0xaab00a01

#define     ASEHANDLER_DOWN     0xaae00a11

/***********************************************************************
	SMDC ERROR CODE : by hryoon
***********************************************************************/

#define		SMDPS_DOWN				0xaa500a00

#define		RSMDP_SMCC_DOWN			0xaa600b00
#define		RSMDP_SMPP_DOWN			0xaa600b01
#define		RSMDP_EMAIL_DOWN		0xaa600b02
#define		RSMDP_MAS_DOWN			0xaa600b03
#define		RSMDP_VMS_DOWN			0xaa600b04
#define		RSMDP_WEB_DOWN			0xaa600b05
#define		RSMDP_SMCI_DOWN			0xaa600b06
#define		RSMDP_DNIP_DOWN			0xaa600b07

#define		SSMDP_SMCC_DOWN			0xaa700b00
#define		SSMDP_SMPP_DOWN			0xaa700b01
#define		SSMDP_EMAIL_DOWN		0xaa700b02
#define		SSMDP_MAS_DOWN			0xaa700b03
#define		SSMDP_VMS_DOWN			0xaa700b04
#define		SSMDP_WEB_DOWN			0xaa700b05
#define		SSMDP_SMCI_DOWN			0xaa700b06
#define		SSMDP_ESME_DOWN			0xaa700b07

#define		TSMDP_DOWN				0xaa700b10

#define		NIF_DOWN				0xaac00b10
#define		NIF1_DOWN				0xaac00a11
#define		SYSTEM_PROCESS_DOWN		0xaac00a12

#define		SECDB_DOWN				0xaad00b10
#define		STATICDB_DOWN			0xaad00b20
#define		RECEIPTDB_DOWN			0xaad00b30
#define		STATIC_DAEMON_DOWN      0xaad00b40
#define		ACTIVEDB_DOWN			0xaad00b50

#define		DELEDM_DOWN				0xaad00b60
#define		TRANSDBMS_DOWN			0xaad00b70
#define		RCSDBMS_DOWN 			0xaad00b90

#define		LOG_WRITER_DOWN			0xaad00b80
#define		PMC_DOWN				0xaad00b90	// SR810 PKG
#define		NUMS_IF_DOWN			0xaad00ba0	// SR810 PKG
#define		ITOD_DOWN				0xaad00bb0
#define		ITOD_DEFENCE			0xaad00bc0
#define		ITOD_SIP_REGI_OVER		0xaad00bc1
#define		ITOD_SIP_MSG_OVER 		0xaad00bc2
#define		DB_REPL_DOWN 			0xaad00bd0 
#define 	FLXRTY_DOWN 			0xaad00be0 
#define		EMS_DB_REPL_DOWN 		0xaad00bf0
#define		PROF_SYNC_DOWN			0xaad00bf1

/***********************************************************************
	DUAL Receiver and DUAL Sender ERROR CODE : by jdshim
***********************************************************************/
#define		DUAL_RECEIVER_DOWN		0xaae00a00
#define		DUAL_SENDER_DOWN		0xaae00a01
#define		DUAL_SYSTEM_SYNC_ERROR	0xaae00a02


/***********************************************************************
	SMDC ERROR CODE : by khlee
***********************************************************************/

#define     PC_CONGESTION       0xbb400b01
#define     PC_UNCONGESTION     0xbb400b01

#define     PC_IN_SVC           0xbb400b02
#define     PC_OUT_SVC          0xbb400b02

/***********************************************************************
	MAS ERROR CODE : by Tsjung
***********************************************************************/

#define		MAS_DISK_FULL_ERROR			0xbb100a00
#define		MAS_CPU_ERROR				0xbb200a00
#define		MAS_LAN_ERROR				0xbb300a00
#define 	MAS_SS7_PLATFORM_ERROR		0xbb400a00

#define		MAS_DELEDM_DOWN				0xbb600c00
/*#define		MAS_EvENTDM_DOWN			0xbb700b00*/
#define		MAS_SYSMON_DOWN				0xbb700b00
#define		MAS_DISK_FULL_70    		0xbb700b01
#define		MAS_DISK_FULL_80    		0xbb700b02
#define		MAS_DISK_FULL_90    		0xbb700b03
#define		MAS_CPU_FULL_70     		0xbb700b04
#define		MAS_CPU_FULL_80     		0xbb700b05
#define		MAS_CPU_FULL_90     		0xbb700b06

#define		MAS_MEM_FULL     			0xbb700a00
#define		MAS_MEM_FULL_70     		0xbb700b07
#define		MAS_MEM_FULL_80     		0xbb700b08
#define		MAS_MEM_FULL_90     		0xbb700b09

#define		MAS_DISK_CLEAR      		0xbb700b0a
#define		MAS_CPU_CLEAR       		0xbb700b0b
#define		MAS_MEM_CLEAR       		0xbb700b0c

#define		MAS_ALARMD_DOWN				0xaa800b01
#define		MAS_PRNTDM_DOWN				0xaa900c00
#define		MAS_CDRDM_DOWN				0xaaa00b00
#define		MAS_CDRTRDM_DOWN			0xaab00b00

#define 	MAS_DISK_ERROR				0xbb800a00

#define 	MAS_DISK_NORMAL				0xbb800b01

#define 	MAS_LINK_SET_ERROR			0xbb800a02
#define 	MAS_LINK_ERROR				0xbb800a03
#define     SMDPP_SEND_ERROR			0xaa900a01
#define     RSMDP_SEND_ERROR			0xaa910a01

#define 	OVER_LOAD_CRITICAL			0xaa920a02
#define		OVER_LOAD_MAJOR				0xaa930a02
#define		OVER_LOAD_MINOR				0xaa940a03
#define		MDB_FULL					0xaa950a04

/***********************************************************************
    ESME ERROR CODE : by hryoon
***********************************************************************/

#define ESME_ERROR_BASE         0xaa600d00

#define ESME_SUCCESS            0x0   /* EOK - Msg accepted */
#define ESME_INVCON             ESME_ERROR_BASE + 0x01  /* Invalid connection */
#define ESME_MSGQFUL            ESME_ERROR_BASE + 0x02  /* Too many messages in queue */
#define ESME_INVCMDID           ESME_ERROR_BASE + 0x03  /* Invalid command Id */
#define ESME_INVSYSID           ESME_ERROR_BASE + 0x04  /* Invalid System ID */
#define ESME_INVSRCADDR         ESME_ERROR_BASE + 0x05  /* Invalid source address */
#define ESME_INVDSTADDR         ESME_ERROR_BASE + 0x06  /* Invalid destination address */
#define ESME_INVSCHED           ESME_ERROR_BASE + 0x07  /* Invalid Schedule Date */
#define ESME_INVEXPIRY          ESME_ERROR_BASE + 0x08  /* Invalid Expiry Date */
#define ESME_INVMSGLEN          ESME_ERROR_BASE + 0x0a  /* Invalid message length */
#define ESME_INVRESPCMDID       ESME_ERROR_BASE + 0x0b  /* Invalid pesponse command Id */
#define ESME_INVMSGID  	        ESME_ERROR_BASE + 0x0c  /* Invalid message ID */
#define ESME_CNTCANMSG          ESME_ERROR_BASE + 0x0d  /* Cannot cancel a message */
#define ESME_CNTREPMSG          ESME_ERROR_BASE + 0x0e  /* Cannot replace a message */
#define ESME_RECV_TIMEOUT       ESME_ERROR_BASE + 0x0f  /* Cannot receive a messaege to time-out */

/***********************************************************************
    Common  Error Code : by hryoon
***********************************************************************/

#define     COM_SIGNAL_ERROR            0xaa010d00

#define     S_ERROR                     0xaa000000
#define     H_ERROR                     0xbb000000

/***********************************************************************
    Message Queue Error Code : by hkkang
***********************************************************************/


#define COM_MQ_CREATE_ERROR           	0xaa020d00
#define COM_MQ_OPEN_ERROR            	0xaa020d01
#define COM_MQ_SEND_ERROR             	0xaa020d02
#define COM_MQ_RCEV_ERROR             	0xaa020d03
#define COM_MQ_TIMEOUT_ERROR          	0xaa020d04
#define COM_MQ_FATAL_TIMEOUT_ERROR    	0xaa020d05
#define COM_MQ_REMOVE_ERROR           	0xaa020d06
#define COM_MQ_CLEAN_ERROR            	0xaa020d07

/***********************************************************************
    Socket Error Code : by hryoon
***********************************************************************/

#define     COM_SK_TIME_OUT             0xaa030d00
#define     COM_SK_POLL_ERROR           0xaa030d01
#define     COM_SK_RECV_ERROR           0xaa030d02
#define     COM_SK_SOCKET_ERROR         0xaa030d03
#define     COM_SK_BIND_ERROR           0xaa030d04
#define     COM_SK_ACCEPT_ERROR         0xaa030d05
#define     COM_SK_CONNECT_ERROR        0xaa030d06
#define     COM_SK_SEND_ERROR           0xaa030d07
#define     COM_SK_FORK_ERROR           0xaa030d08
#define     COM_SK_PORT_ERROR           0xaa030d09
#define     COM_SK_RESPONSE_ERROR       0xaa030d0a
#define     COM_SK_LISTEN_ERROR         0xaa030d0b
#define     COM_SK_ADDRESS_ERROR        0xaa030d0c
#define     COM_SK_SETSOCKOPT_ERROR     0xaa030d0d

#define     COM_SK_SMPP_SEND_ERROR      0xaa030d10
#define     COM_SK_SMPP_RECV_ERROR      0xaa030d11
#define     COM_SK_SMDP_SEND_ERROR      0xaa030d12
#define     COM_SK_SMDP_RECV_ERROR      0xaa030d13

#define     COM_SK_RETRY_NAK            0xaa030d20
#define     COM_SK_INVALID_NAK          0xaa030d21
#define     COM_SK_SUSPEND_NAK          0xaa030d22
#define     COM_SK_RESUME_NAK           0xaa030d23
#define     COM_SK_STOP_NAK             0xaa030d24
#define     COM_SK_CONTINUE_NAK         0xaa030d25

/***********************************************************************
    DB Error Code : by hkkang
***********************************************************************/

#define COM_DB_INSERT_ERROR         0xaa040d01
#define COM_DB_DELETE_ERROR         0xaa040d02
#define COM_DB_UPDATE_ERROR         0xaa040d03
#define COM_DB_TRANSACT_ERROR       0xaa040d04

#define COM_DB_OPEN_ERROR           0xaa040d05
#define COM_DB_CLOSE_ERROR          0xaa040d06
#define COM_DB_CORRUPTED            0xaa040d07

#define COM_DB_INVALID_ATTR         0xaa040d08
#define COM_DB_INVALID_INDEX        0xaa040d09
#define COM_DB_RCD_DUPLICATE        0xaa040d10
#define COM_DB_RCD_NOEXIST          0xaa040d11

#define COM_DB_CNTOPENLOG           0xaa040d12
#define COM_DB_CNTCLSLOG            0xaa040d13
#define COM_DB_CNTWRTLOG            0xaa040d14

#define COM_DB_WRITE_ERROR          0xaa040d15
#define COM_DB_READ_ERROR           0xaa040d16

#define COM_DB_UNKNOWN              0xaa040d17

/***********************************************************************
    FILE Error Code : by hryoon
***********************************************************************/

#define COM_FILE_OPEN_ERROR         0xaa050d01
#define COM_FILE_CLOSE_ERROR        0xaa050d02

/***********************************************************************
    NPQC    ERROR CODE : by twkim
***********************************************************************/
#define     NPQC_M_DOWN             0xaa130a00
#define     NPQC_S_DOWN             0xaa130a01


/***********************************************************************
    TOC/TOD ERROR CODE : by sypark
***********************************************************************/
#define     TOC_DOWN                0xaa170a00
#define     TOD_DOWN                0xaa170a01
/***********************************************************************
 *     CHECK CDR ERROR CODE : by sypark
************************************************************************/
#define     CDR_INFO_NOT_CREATE     0xaa180a00
#define     CDR_DATA_NOT_CREATE     0xaa180a01
#define     CDR_COMP_NOT_CREATE     0xaa180a02

//SR320 pkg async log write alarm code
#define LFDS_QUEUE_FULL 	0xaa19a000
#define TRACE_QUEUE_FULL 	0xaa19a001

//R810 pkg pending msg control alarm code
#define PENDING_SPAM_CONTROL_START 		0xaa190a00
#define PENDING_SPAM_CONTROL_CLEAR 		0xaa190a01
#define PENDING_SPAM_ADMINBAR_DELETE 	0xaa190a02

//R810 PKG info rate cid alarm code
#define NUMS_DATA_IN 				0xaa1a0a00
#define NUMS_SESSION_CLOSE 			0xaa1a0a01

#define AGENT_KEYWORD_SENSING 		0xaa1b0a00
#define AGENT_VM_DOWN 		 		0xaa1b0a01





#endif

