/******************************************************************************r
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
#******************************************************************************/

/***********************************************************************
	FILE NAME		:	SmsIpc.h
	PURPOSE			:	SMSC IPC ID Policy
	DATE OF WRITE	:
	AUTHOR			:
	REVISION		:	V 1.0
	Ver.	By		When		Reason
	------- ------- ----------  -----------------------------------------
	1.0		        1998.04.29	first coding
	1.1		        1998.05.13	modify due to schema modificaton
***********************************************************************/

#ifndef	_SMSIPC_H_
#define	_SMSIPC_H_

/*
 * 0x00[No Use]0[Category]0[SubCatagory]
 * 00[Process]0[Shared Memroy or Message Queue]0[Sequence ID or Semaphore ID]
 * ex) 0x00DE0119 > category    : D DBMS.
 *                  subcategory : E Evnet Queue
 *                  process     : 01 Active DB
 *                  shoared mem : 1 first.
 *                  semaphore   : 9th.
 */

////
//  Catagory Definition	(Process Group)
typedef enum {
	SIC_MNPS		= 0x00100000,	// Mobile Number Portability
	SIC_DUAL		= 0x00200000,	// Dual Active
	SIC_PLUS		= 0x00300000,	// SMS Plus Service
	SIC_NMPI		= 0x00400000,	// Mobile Number Portability I/F
	SIC_ASES		= 0x00A00000,	// Ase and Map
	SIC_HIST		= 0x00B00000,	// Uhis and History
	SIC_SCHD		= 0x00C00000,	// Schdulers
	SIC_DBMS		= 0x00D00000,	// DBMSs
	SIC_ESME		= 0x00E00000,	// ESMEs
	SIC_MANA		= 0x00F00000	// Management and UI
} eSmsIpcCategory;

////
//  Sub Catagory Catagory Definition
//  (IPC Use Type -- maybe only shared memory)
typedef enum {
	SISC_MDBSPT		= 0x000A0000,	// Shared Memory MDB Space
	SISC_EVENT		= 0x000E0000,	// Shared Memory Event Queue
	SISC_DATA		= 0x000D0000	// Shared Memory Normal Data Storage
} eSmsIpcSubCategory;

////
//  Process Definition
typedef enum { // may be Creater.
	SIPI_MASTER		= 0x00000000,	// sms_master -- process contorol
	SIPI_ACTVDB		= 0x00000100,	// sms_adbms  -- active dbms
	SIPI_STATDB		= 0x00000200,	// sms_sdbms  -- static dbms
	SIPI_RECTDB		= 0x00000300,	// sms_rdbms  -- receipt dbms
	SIPI_PLUSDB		= 0x00000400,	// sms_pdbms  -- sms plus dbms
	SIPI_DUALRV		= 0x00000500,	// sms_dualrv -- dual active receiver
	SIPI_DUALSD		= 0x00000600,	// sms_dualsd -- dual active sender
	SIPI_ASEMAP		= 0x00000700,	// sms_aseXnn -- MAP appl service entity
	SIPI_SCHDLR		= 0x00000800,	// sms_SCHDXn -- message schedulers
	SIPI_UHIS00		= 0x00000900,	// sms_uhis   -- history saver
	SIPI_SMDPS0		= 0x00000A00,	// sms_smdps  -- esme I/F controler
	SIPI_RCVESM		= 0x00000B00,	// sms_rsmXXxx-- esme receiver.
	SIPI_SNDESM		= 0x00000C00,	// sms_ssmXXxx-- esme sender.
	SIPI_PLSYNC		= 0x00000D00,	// sms_psync  -- plus dbms syncer.
	SIPI_PLBMCH		= 0x00000E00,	// sms_pbmch  -- plus block min checker.
	SIPI_PLSMFC		= 0x00000F00,	// sms_psmfc  -- plus sm forward controler.
	SIPI_SYSMON		= 0x00001000,	// sms_sysmon -- system moniter.
	SIPI_ALARMD		= 0x00001100,	// sms_alarmdm-- service alarm deamon.
	SIPI_DELEDM		= 0x00001200,	// sms_deletedm-- log delete dm.
	SIPI_STATDM		= 0x00001300,	// sms_staticsdm-- real time statistic.
	SIPI_NPQCMS		= 0x00001400,	// sms_npqxx -- number portability I/F.
	SIPI_MASCUI		= 0x00001500,	// sms_npqxx -- number portability I/F.
	SIPI_RUHIS00	= 0x00001600,	// sms_ruhis  -- receipt history saver
	SIPI_TRANDB		= 0x00001700,	// sms_tdbms - transform dbms
	SIPI_RCSADB		= 0x00001800,	// sms_cdbms - rcs active dbms
	SIPI_CUHIS00	= 0x00001900,	// sms_ruhis  -- receipt history saver
	SIPI_MKCDR0     = 0x00001A00,   // sms_makecdr -- make cdr
	SIPI_WRITER     = 0x00001B00,   // sms_writer -- trace write
	SIPI_S6C 		= 0x00001C00    // sms_s6c -- Diameter HLR I/F
} eSmsIpcProcessID;

typedef enum { // maybe no use
	STATIC_ERROR_CODE_KEY	= 0x2010				/* 0x2010 */
} eStaticErrCodeHaredMemroryKey;

typedef enum {
	QUERY_REQUEST	= 0x00000010,
	QUERY_RESULT	= 0x00000020,
	QUERY_BACKUP	= 0x00000030,
	QUERY_STAT   	= 0x00000040 //SR124 BMT
} eDBMSMessageQueueType;

//// It must be same with Smsc.cfg each [DBMS], IPC_KEY_BASE field
typedef enum {
	ACTVDB_BASE_KEY	= 0x00000000,
	STATDB_BASE_KEY = 0x00000100,
	RECTDB_BASE_KEY = 0x00000200,
	PLUSDB_BASE_KEY = 0x00000300,
	TRANDB_BASE_KEY = 0x00000400,
	RCSADB_BASE_KEY = 0x00000500
} eDBMSIpcBaseKey;

//////             Start Active DBMS IPC Definition
////
//
/// DBMSs System Message Queue
typedef enum {
	DBMSQ 			= (SIC_DBMS | SIPI_ACTVDB | QUERY_REQUEST),
	DBMSQ_RESULT	= (SIC_DBMS | SIPI_ACTVDB | QUERY_RESULT),
	DBMSQ_BACKUP	= (SIC_DBMS | SIPI_ACTVDB | QUERY_BACKUP),
	DBMS_TO_SSMDP	= ((SIC_DBMS | SIPI_ACTVDB | QUERY_BACKUP) + 0x10),
	DBMSQ_STAT		= (SIC_DBMS | SIPI_ACTVDB | QUERY_STAT) //SR124 BMT
} eActiveDbmsMessageQueueKeys;

// from DBMSQ_RESULT TYPE
typedef enum {
	SQL_MY_KEY			= 0x0001,
	FROM_DBMS_TO_UHIS,
	DELMIN_TO_DBMS,
	FROMDBMS,
	RSMDPTODBMS,
	SCHDTODBMS,
	DUALACTTODBMS,

	/// for MAS
	// used checkSme in mas ~/src/mas/messageHandling/checkSme/smeCheck.c
	QMITOMAS,
	CHECKSME_MQ_KEY,// for test QMITOMAS
	PREFIX_MQ_KEY,	// for PrefixMan
	RNPREFIX_MQ_KEY,// for RNPrefixMan (Routing Number)
	ESME_MQ_KEY,	// for EsmeMan
	SERVICE_MQ_KEY,	// for ServiceMan (SCP)
	NPDB_MQ_KEY,	// for NpdbMan
	NPQCID_MQ_KEY,	// for NPCQIdMan
	UTIL_MQ_KEY,	// for Utility
	VMPREFIX_MQ_KEY,	//for VMprefix 	by voyage00 */
	MSGCTRL_MQ_KEY,
	ACTIVE_RESULT_KEY_MAX
} eActiveDbmsMsgQueueResultType_t;

/// Static MDB Shared Memrory Key Definition
typedef enum {
	SHMDBMS		= SIC_DBMS | SISC_MDBSPT | SIPI_ACTVDB | 0x00000010,
	SHMDBKEY	= SIC_DBMS | SISC_MDBSPT | SIPI_ACTVDB | 0x00000020,
	SHMINDEX	= SIC_DBMS | SISC_MDBSPT | SIPI_ACTVDB | 0x00000030,
	SEMDBKEY	= SIC_DBMS | SISC_MDBSPT | SIPI_ACTVDB | 0x00000020,
	SEMDBIDX	= SIC_DBMS | SISC_MDBSPT | SIPI_ACTVDB | 0x00000030
} eActiveMdbSharedMememoryKey_t;
/*
typedef enum {
	SHMDBMS		= SIC_DBMS | SISC_MDBSPT | SIPI_ACTVDB | 0x00000010,
	SHMDBKEY	= SIC_DBMS | SISC_MDBSPT | SIPI_ACTVDB | 0x00000020,
	SHMINDEX	= SIC_DBMS | SISC_MDBSPT | SIPI_ACTVDB | 0x00000030,
	// Min Mask Shared Memory Key Definition
	SHMMININDEX	= SIC_DBMS | SISC_MDBSPT | SIPI_ACTVDB | 0x00000040, // ?
	SHMMINMASK	= SIC_DBMS | SISC_MDBSPT | SIPI_ACTVDB | 0x00000050,
	SHMMINMASK2	= SIC_DBMS | SISC_MDBSPT | SIPI_ACTVDB | 0x00000060, // ?
	SHMMSGQ		= SIC_DBMS | SISC_MDBSPT | SIPI_ACTVDB | 0x00000070, // ?
	// Exists Code in E4Noti.c and dml.c of actDBMS, but not use.
	DBMSTOSCHD	= SIC_DBMS | SISC_MDBSPT | SIPI_ACTVDB | 0x00000080 // ?
} eActiveMdbSharedMememoryKey_t;
*/

//
//////             End Active DBMS IPC Definition


//////             Start Plus DBMS IPC Definition
////
//
/// Plus DBMS Message Queue Key Definition
typedef enum {
	PLUS_DBMSQ			= DBMSQ + PLUSDB_BASE_KEY,
	PLUS_DBMSQ_RESULT	= DBMSQ_RESULT + PLUSDB_BASE_KEY,
	PLUS_DBMSQ_BACKUP	= DBMSQ_BACKUP + PLUSDB_BASE_KEY
} ePlusDbmsMsgQueueKey_t;

// from PLUS_DBMSQ_RESULT Type
typedef enum {
	PLUS_SQL_KEY		= 0x0001,
	PLUS_SYNC_KEY,		// 0x0002
	PLUS_TEST_KEY,		// 0x0003
	PLUS_SPAM_MAS_KEY,	// 0x0004
	PLUS_RESULT_KEY_MAX
} ePlusDbmsMsgQueueResultType_t;

/// Plus MDB Shared Memrory Key Definition
typedef enum {
	PLUS_SHMDBMS	= SHMDBMS  + PLUSDB_BASE_KEY,
	PLUS_SHMDBKEY	= SHMDBKEY + PLUSDB_BASE_KEY,
	PLUS_SHMINDEX	= SHMINDEX + PLUSDB_BASE_KEY,
	PLUS_SEMDBKEY	= SEMDBKEY + PLUSDB_BASE_KEY,
	PLUS_SEMDBIDX	= SEMDBIDX + PLUSDB_BASE_KEY
} ePlusMdbSharedMememoryKye_t;

//
//////             End Plus DBMS IPC Definition


//////             Start Statistics DBMS IPC Definition
////
//
/// Static DBMS Message Queue Key Definition
typedef enum {
	STATICDBMSQ         = DBMSQ + STATDB_BASE_KEY,
	STATICDBMSQ_RESULT  = DBMSQ_RESULT + STATDB_BASE_KEY,
	STATICDBMSQ_BACKUP  = DBMSQ_BACKUP + STATDB_BASE_KEY
} eStaticDbmsMsgQueueKey_t;

// from STATICDBMSQ_RESULT Type
typedef enum {
	SQL2_MY_KEY			= 0x0001,
	STATISTICS_MQ_TYPE,	//0x0002
	UHISTORECEIPT		//0x0003
//RSMDP_STATISTICS_MQ		0x04
} eStaticDbmsMsgQueueResultType_t;

/// Static MDB Shared Memrory Key Definition
typedef enum {
	SHMSTATICDBMS	= SHMDBMS + STATDB_BASE_KEY,
	SHMSTATICDBKEY	= SHMDBKEY + STATDB_BASE_KEY,
	SHMSTATICINDEX	= SHMINDEX + STATDB_BASE_KEY,
	SEMSTATICDBKEY	= SEMDBKEY + STATDB_BASE_KEY,
	STATDB_SEMDBIDX	= SEMDBIDX + STATDB_BASE_KEY
} eStaticMdbSharedMememoryKye_t;
//
//////             End Statistics DBMS IPC Definition


//////             Start Receipt DBMS IPC Definition
////
//
/// Receipt DBMS Message Queue Key Definition
typedef enum {
	RECEIPTDBMSQ		= DBMSQ + RECTDB_BASE_KEY,
	RECEIPTDBMSQ_RESULT	= DBMSQ_RESULT + RECTDB_BASE_KEY,
	RECEIPTDBMSQ_BACKUP = DBMSQ_BACKUP + RECTDB_BASE_KEY
} eReceiptDbmsMessageQueueKey_t;

// from RECEIPTDBMSQ_RESULT	Type
typedef enum {
	SQL3_MY_KEY			= 0x0001,
	SCHDTORECEIPT,		//0x0002,
	SSMDPTORECEIPT,		//0x0003,
	MASTORECEIPT,		//0x0004,
	DUALTORECEIPT,		//0x0005,
	INSREPTTORECEIPT,	//0x0006,
	LOADTORECEIPT		//0x0007
} eReceiptDbmsMsgQueueResultType_t;

/// Receipt MDB Shared Memrory Key Definition
typedef enum {
	SHMRECEIPTDBMS	= SHMDBMS + RECTDB_BASE_KEY,
	SHMRECEIPTDBKEY	= SHMDBKEY + RECTDB_BASE_KEY,
	SHMRECEIPTINDEX	= SHMINDEX + RECTDB_BASE_KEY,
	SEMRECEIPTDBKEY	= SEMDBKEY + RECTDB_BASE_KEY,
	RECTDB_SEMDBIDX	= SEMDBIDX + RECTDB_BASE_KEY
} eReceiptMdbSharedMememoryKye_t;

/// Transform DBMS Message Queue Key Definition
typedef enum {
	TRANSFORMDBMSQ			= DBMSQ + TRANDB_BASE_KEY,
	TRANSFORMDBMSQ_RESULT	= DBMSQ_RESULT + TRANDB_BASE_KEY,
	TRANSFORMDBMSQ_BACKUP 	= DBMSQ_BACKUP + TRANDB_BASE_KEY
} eTransformDbmsMessageQueueKey_t;

// from TRANSFORMDBMSQ_RESULT	Type
typedef enum {
	SQL4_MY_KEY			= 0x0001,
	SCHDTOTRANSFORM,		//0x0002,
	SSMDPTOTRANSFORM,		//0x0003,
	MASTOTRANSFORM,		//0x0004,
	DUALTOTRANSFORM,		//0x0005,
	INSREPTTOTRANSFORM,	//0x0006,
	LOADTOTRANSFORM		//0x0007
} eTransformDbmsMsgQueueResultType_t;

/// Transform MDB Shared Memrory Key Definition
typedef enum {
	SHMTRANSFORMDBMS	= SHMDBMS + TRANDB_BASE_KEY,
	SHMTRANSFORMDBKEY	= SHMDBKEY + TRANDB_BASE_KEY,
	SHMTRANSFORMINDEX	= SHMINDEX + TRANDB_BASE_KEY,
	SEMTRANSFORMDBKEY	= SEMDBKEY + TRANDB_BASE_KEY,
	TRANDB_SEMDBIDX		= SEMDBIDX + TRANDB_BASE_KEY
} eTransformMdbSharedMememoryKye_t;

/// RCS DBMS Message Queue Key Definition
typedef enum {
	RCSDBMSQ			= DBMSQ + RCSADB_BASE_KEY,
	RCSDBMSQ_RESULT	= DBMSQ_RESULT + RCSADB_BASE_KEY,
	RCSDBMSQ_BACKUP 	= DBMSQ_BACKUP + RCSADB_BASE_KEY
} eRcsDbmsMessageQueueKey_t;

// from RCSADBMSQ_RESULT	Type
typedef enum {
	SQL5_MY_KEY			= 0x0001,
	SCHDTORCS,		//0x0002,
	SSMDPTORCS,		//0x0003,
	MASTORCS,		//0x0004,
	DUALTORCS,		//0x0005,
	INSREPTTORCS,	//0x0006,
	LOADTORCS		//0x0007
} eRcsDbmsMsgQueueResultType_t;

// RCS MDB Shared Memory KEy Defintion
typedef enum {
	SHMRCSDBMS	= SHMDBMS + RCSADB_BASE_KEY,
	SHMRCSDBKEY	= SHMDBKEY + RCSADB_BASE_KEY,
	SHMRCSINDEX	= SHMINDEX + RCSADB_BASE_KEY,
	SEMRCSDBKEY	= SEMDBKEY + RCSADB_BASE_KEY,
	RCS_SEMDBIDX		= SEMDBIDX + RCSADB_BASE_KEY
} eRcsMdbSharedMememoryKye_t;

//#define RDBMS_MSGQ				0xE600
//#define RDBMS_MSGQ_RESULT  		0xE700
//
//////             End Receipt DBMS IPC Definition


//////             Start ASE IPC Definition
////
//
// for Point Code and SSN Lock Status
typedef enum {
	MAPPCSSNSHMKEY	= SIC_ASES | SISC_DATA | SIPI_ASEMAP | 0x00000010
} eAsePointCodeSubSystemNumberStatusKey;

// for ASE's receiving SMSREQ Event from SCHD
typedef enum {
	TOSSMSREQ	= SIC_ASES | SIPI_ASEMAP | 0x00000010,
	TOIASE		= SIC_ASES | SIPI_ASEMAP | 0x00000020,	// Interective ASE.
	TOMMEMTF 	= SIC_ASES | SIPI_ASEMAP | 0x00000030,
	TODIAHLR 	= SIC_ASES | SIPI_ASEMAP | 0x00000040,
	TOISCMTF 	= SIC_ASES | SIPI_ASEMAP | 0x00000050,
	TOIEPMTF 	= SIC_ASES | SIPI_ASEMAP | 0x00000060
} eAseEventReceivingMessageQueueKey_t;
//
//////             End ASE IPC Definition


//////             Start ESME IPC Definition
////
//
// for SSMDP's receiving Delivery Message Event from SCHD
/* must be deleted
typedef enum {
	TOSSMDP		= SIC_ESME | SIPI_SNDESM | 0x00000010,
	RECEIPTDBMS_TO_SSMDP= DBMS_TO_SSMDP+ RECTDB_BASE_KEY
	TOISSMDP	= SIC_ESME | SIPI_SNDESM | 0x00000020 // Interactive SSMDP
} eSsmdpDeliveryMessageQueueKey_t;
*/
typedef enum {
	TOESME		= SIC_ESME | SIPI_SNDESM | 0x00000010,
	TOSVCREQ	= SIC_ESME | SIPI_SNDESM | 0x00000020,
	TOIESEME	= SIC_ESME | SIPI_SNDESM | 0x00000030 // Interactive SSMDP
} eSsmdpDeliveryMessageQueueKey_t;

// for Message ID Create Queue (SMDSS to RSMDP). modified at below for master
//typedef enum {
//	SMDPSTORSMDP	= SIC_ESME | SIPI_RCVESM | 0x00000010
//} eSmdpsMsgIDCreateMessageQueueKey_t;

typedef enum {
	SYS_MSGID_QTYPE = 0x0001
} eSystemMsgIdSendMessageQueueType_t;

/***********************************************************************
        ESME message queue Key
***********************************************************************/
typedef enum {
	// for process management command
	ESME_CMD_MSGQ = SIC_ESME | SIPI_SMDPS0 | 0x000000010,
	// for process management command ack
	ESME_ACK_MSGQ 	= SIC_ESME | SIPI_SMDPS0 | 0x000000020
} eEsmeMessageQueueKey_t;

//
//////             End ESME IPC Definition

// for NIF MIB Send Queue, maybe must be deleted.
//#define TONIF               (key_t)0xd440
//#define TONIF1              (key_t)0xd450

//////             Start MNP IPC Definition
////
//
// for NPCS receiving NPDB Query Message Event from SCHD
typedef enum {
	TONPQC = SIC_NMPI | SIPI_NPQCMS | 0x00000010
} eNumberPortabilityMessageQueueKey_t;
//
//////             End MNP IPC Definition

//////             Start MAKE CDR IPC Definition
////
// for CDR <-> UHIS, CDR <-> RUHIS sync sem key
typedef enum {
	SEM_KEY4CDR_AUHIS   	= SIC_HIST | SISC_EVENT | SIPI_MKCDR0 | 0x00000010, // CDR - UHIS
	SEM_KEY4CDR_RUHIS   	= SIC_HIST | SISC_EVENT | SIPI_MKCDR0 | 0x00000011, // CDR - RUHIS
    SEM_KEY4CDR_UHIS_MCS 	= SIC_HIST | SISC_EVENT | SIPI_MKCDR0 | 0x00000012  // MCS - UHIS - CUHIS
} eCdrSyncEventQueue_t;
//
//////             End MAKE CDR IPC Definition

//////             Start MAKE Log Writer IPC Definition
////
// Log Writer receiving Log Data from SMSC All Process
typedef enum {
	ASYNC_LOG_WRITER_KEY   = SIC_MANA | SISC_EVENT | SIPI_WRITER | 0x00000010,
} eLogWriteQueue_t;
//
//////             End MAKE Log Writer IPC Definition

//////             Start UHIS IPC Definition
////
//
// for UHIS's Delete Active Event Queue from Others
typedef enum {
	SHM_KEY4AUHIS	= SIC_HIST | SISC_EVENT | SIPI_UHIS00 | 0x00000010,
	SEM_KEY4AUHIS	= SIC_HIST | SISC_EVENT | SIPI_UHIS00 | 0x00000011
} eUhisDeleteEventQueue_t;
//
//////             End UHIS IPC Definition

//////             Start RUHIS IPC Definition
////
//
// for RUHIS's Delete Receipt Event Queue from Others
typedef enum {
	SHM_KEY4RUHIS	= SIC_HIST | SISC_EVENT | SIPI_RUHIS00 | 0x00000010,
	SEM_KEY4RUHIS	= SIC_HIST | SISC_EVENT | SIPI_RUHIS00 | 0x00000011
} eRUhisDeleteEventQueue_t;

typedef enum {
	SHM_KEY4CUHIS	= SIC_HIST | SISC_EVENT | SIPI_CUHIS00 | 0x00000010,
	SEM_KEY4CUHIS	= SIC_HIST | SISC_EVENT | SIPI_CUHIS00 | 0x00000011
} eCUhisDeleteEventQueue_t;
//
//////             End RUHIS IPC Definition

//////             Start SCHD IPC Definition
////
//
// for SCHD's Active MsgID Event Queue from Others
typedef enum {
	// for Receiving to Send and Routing Msg.
	SHM_KEY4SCHD_ASNDMSG = SIC_SCHD | SISC_EVENT | SIPI_SCHDLR | 0x00000010,
	SEM_KEY4SCHD_ASNDMSG = SIC_SCHD | SISC_EVENT | SIPI_SCHDLR | 0x00000011,
	// for Receiving to Retry.
	SHM_KEY4SCHD_ASRETRY = SIC_SCHD | SISC_EVENT | SIPI_SCHDLR | 0x00000020,
	SEM_KEY4SCHD_ASRETRY = SIC_SCHD | SISC_EVENT | SIPI_SCHDLR | 0x00000021,
	// for Receiving to Retservation.
	SHM_KEY4SCHD_ASRSVD = SIC_SCHD | SISC_EVENT | SIPI_SCHDLR | 0x00000030,
	SEM_KEY4SCHD_ASRSVD = SIC_SCHD | SISC_EVENT | SIPI_SCHDLR | 0x00000031
} eActiveSchdulersEventQueue_t;

typedef enum {
	// for Receiving to Send and Routing Msg.
	SHM_KEY4SCHD_RSNDMSG = SIC_SCHD | SISC_EVENT | SIPI_SCHDLR | 0x00000040,
	SEM_KEY4SCHD_RSNDMSG = SIC_SCHD | SISC_EVENT | SIPI_SCHDLR | 0x00000041,
	// for Receiving to Retry.
	SHM_KEY4SCHD_RSRETRY = SIC_SCHD | SISC_EVENT | SIPI_SCHDLR | 0x00000050,
	SEM_KEY4SCHD_RSRETRY = SIC_SCHD | SISC_EVENT | SIPI_SCHDLR | 0x00000051,
	// for Receiving to Retservation.
	SHM_KEY4SCHD_RSRSVD = SIC_SCHD | SISC_EVENT | SIPI_SCHDLR | 0x00000060,
	SEM_KEY4SCHD_RSRSVD = SIC_SCHD | SISC_EVENT | SIPI_SCHDLR | 0x00000061
} eReceiptSchdulersEventQueue_t;

typedef enum {
	// for Receiving to Send and Routing Msg.
	SHM_KEY4SCHD_TSNDMSG = SIC_SCHD | SISC_EVENT | SIPI_SCHDLR | 0x00000070,	// xxCx xxxE xxxxx8
	SEM_KEY4SCHD_TSNDMSG = SIC_SCHD | SISC_EVENT | SIPI_SCHDLR | 0x00000071
} eTransferSchdulersEventQueue_t;

typedef enum {
	// for Receiving to Send and Routing Msg.
	SHM_KEY4SCHD_RCSASND = SIC_SCHD | SISC_EVENT | SIPI_SCHDLR | 0x00000080,	// xxCx xxxE xxxxx8
	SEM_KEY4SCHD_RCSASND = SIC_SCHD | SISC_EVENT | SIPI_SCHDLR | 0x00000081,

	SHM_KEY4SCHD_CSRETRY = SIC_SCHD | SISC_EVENT | SIPI_SCHDLR | 0x00000090,
	SEM_KEY4SCHD_CSRETRY = SIC_SCHD | SISC_EVENT | SIPI_SCHDLR | 0x00000091,

	SHM_KEY4SCHD_CSRSVD  = SIC_SCHD | SISC_EVENT | SIPI_SCHDLR | 0x000000a0,
	SEM_KEY4SCHD_CSRSVD  = SIC_SCHD | SISC_EVENT | SIPI_SCHDLR | 0x000000a1
} eRcsSchdulersEventQueue_t;

// for Dual Active Node Status Board (SCHD Manager write).
typedef enum {
	SHM_KEY4MCSTBOARD = SIC_SCHD | SISC_DATA | SIPI_SCHDLR | 0x00000010
} eDualActiveMachineStatusBoardSharedMemoryKey_t;
//
//////             End SCHD IPC Definition

//////             Start Dual Active IPC Definition
////
//
// for DUAL's Active MsgID Event Queue from Others
typedef enum {
	SHM_KEY4DUALSND = SIC_DUAL | SISC_EVENT | SIPI_DUALSD | 0x00000010,
	SEM_KEY4DUALSND = SIC_DUAL | SISC_EVENT | SIPI_DUALSD | 0x00000011
} eDualActiveEventQueue_t;

//for DUAL's System Messsage Queue.
typedef enum {
	MSGQ_KEY4DUALSYNC = SIC_DUAL | SIPI_DUALSD | 0x000000010,
	TODUALSYNC = MSGQ_KEY4DUALSYNC
} eDualSyncMsgQueue_t;

//
//////             End Dual Active IPC Definition


//////             Start SMS Plus IPC Definition
////
//
// for PLUS's Active MsgID Event Queue from Others
typedef enum {
	// SMS Plus Block Min Checker Event Queue.
	SHM_KEY4PLUSBMCH = SIC_PLUS | SISC_EVENT | SIPI_PLBMCH | 0x000000010,
	SEM_KEY4PLUSBMCH = SIC_PLUS | SISC_EVENT | SIPI_PLBMCH | 0x000000011,
	// SMS Plus Short Message Forwarder Event Queue.
	SHM_KEY4PLUSSMFC = SIC_PLUS | SISC_EVENT | SIPI_PLSMFC | 0x000000010,
	SEM_KEY4PLUSSMFC = SIC_PLUS | SISC_EVENT | SIPI_PLSMFC | 0x000000011
} eSMSPlusEventQueue_t;
//
//for PLUS's Active MsgID Message Queue from Others
typedef enum {
	//  SMS Plus Short Message Forwarder Msg Queue. - V2.0.1
	MSGQ_KEY4PLUSSMFC = SIC_PLUS | SIPI_PLSMFC | 0x000000010
} eSMSPlusMsgQueue_t;
//
//////             End SMS Plus IPC Definition

/***********************************************************************
        MAS message queue Key
***********************************************************************/
typedef enum {
// for Message ID Create Queue (MASTER to SYSTEM).
	SYS_MSGID_Q	= SIC_MANA | SIPI_MASTER | 0x00000010,
	// for process management command
	SM_MD_MSGQ1 = SIC_MANA | SIPI_MASTER | 0x000000020,
	// for process management command ack
	ACK_MSGQ 	= SIC_MANA | SIPI_MASTER | 0x000000030
} eMasMessageQueueKey_t;


/// Static PS_TABLE Shared Memrory Key Definition
typedef enum {
	SHM_MAS_PSTBL	= SIC_MANA | SISC_MDBSPT | SIPI_MASTER | 0x00000010
} ePsTableSharedMememoryKey_t;


// for Message ID Create Queue (SMDSS to RSMDP). modified at below for master
typedef enum {
	SMDPSTORSMDP	= SYS_MSGID_Q
} eSmdpsMsgIDCreateMessageQueueKey_t;
/***********************************************************************
        MAS Shared Memory Key
***********************************************************************/
typedef enum {
	// for login check queue of main menu
	SM_SHMKEY = SIC_MANA | SIPI_MASCUI | 0x000000010
} eMasSharedMemoryKey_t;

/***********************************************************************
        S6c HLR GRP Shared Memory Key
***********************************************************************/
typedef enum {
	S6C_HLR_GRP_SHMKEY = SIC_PLUS | SISC_MDBSPT | SIPI_S6C | 0x00000001,
	S6C_HLR_GRP_SEMKEY = SIC_PLUS | SISC_MDBSPT | SIPI_S6C | 0x00000002
} eS6cSharedMemoryKey_t;

#endif /* _SMSIPC_H_ */
