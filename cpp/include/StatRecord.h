//*******************************************************************************
//
//     Copyright (c) 2003 FEELINGK Co., Ltd.
//     All rights reserved.
//
//     This program contains confidential and proprietary information of
//     FEELINGK, and any reproduction, disclosure, or use in whole or in part
//     by prior written agreement or permission of FEELINGK.
//
//                            FEELINGK Co., Ltd.
//     8F Taekyung B/D 42-42 Bongchun 10Dong Gwanak-Gu, Seoul 151-060 Korea
//
//******************************************************************************

//--------------------------------------------------------------------
//
//  FILE     NAME   : StatisticsRecord.h
//  PURPOSE         : SMS Statistics Record Header file
//  INPUT ARGUMENTS :
//  RETURN VALUE    :
//  CALLED FROM     :
//  USED GLOBAL VARIABLES :
//    1) EXTERNAL
//    2) LOCAL
//  REVISION        :
//      BY              WHEN        REASON
//      --------------  ----------  -------------------
//      smyang          2003.4.      First Coding
//
//----------------------------------------------------------------------

#ifndef _STAT_RECORD_H_
#define _STAT_RECORD_H_

#include <FLKBaseDef.h>

//***********************************************************************
//        SMS Statistics Record Strucutre
//***********************************************************************

#ifdef __cplusplus
//extern "C" {
#endif

typedef struct _stStatisticsRecord {
	u_int    RecordType : 8;
	u_int    Reserved1 : 8;
	u_int    TryInterface : 2;
	u_int 	TryRole : 6;
	u_int	TryDirection : 2;
	u_int 	TryEvent : 7;
	u_short	TryDataLen;
	u_int  	TryTime;
	u_int 	TryResult;
	u_int	MsgID;
	u_short	ProcessID;
	u_short	ServiceID;
	u_short    GroupID;
	char 	    SME[12];
} stStatisticsRecord;

#define STATISTICS_RECORD_GRPID_SIZE	12
#define STATISTICS_RECORD_SME_SIZE		12

//***********************************************************************
// CDMA, GSM SSN
//***********************************************************************
#define HLR_SSN             6
#define MSC_SSN             8
#define IMX_SSN             8
#define SMSC_SSN            11
#define SGSN_SSN            149
#define MME_SSN 			250
#define SMSF_SSN 			251
#define ESME_SSN 			248
#define CSCF_SSN 			247
#define CSCF_REGI_SSN 		246


//***********************************************************************
//        SMS Statistics Record Define
//***********************************************************************

#define MAX_RECORD_CLASS	3
typedef enum {
	STATISTICS_RECORD_START  = 0,   // 시작 Record
	STATISTICS_RECORD_END    = 1,    // 종료 Record
	STATISTICS_RECORD_SINGLE = 2    // 단일 Record
} eStaticRecordType;

//	TryInterface
//	Static Record의 생성되는 Interface를 표시한다
#define MAX_INTFACE_CLASS	4
typedef enum {
	STATISTICS_TRYINF_INTL  = 0,    // Internal
	STATISTICS_TRYINF_NET   = 1,   	// Network - SS7
	STATISTICS_TRYINF_TCP   = 2,  	// TCP/IP
	STATISTICS_TRYINF_RES   = 3   // Resource
} eStaticTryInterface;

//	TryRole
//	TryInterface에 따르는 Role을 표시한다

#define MAX_ROLE_CLASS	16
//	TryInterface = STATISTICS_TRYINF_NET 인 경우에
typedef enum {
	STATISTICS_TRYNRL_TCAP   = 0,  // TCAP Interface
	STATISTICS_TRYNRL_CDMA   = 1,  // 2G/CDMA Interface
	STATISTICS_TRYNRL_GSM    = 2,  // 3G/GSM Interface
	STATISTICS_TRYNRL_GW     = 3,   // 3G/GSM Interface
	STATISTICS_TRYNRL_RCS    = 4,  // 3G/RCS Interface
	STATISTICS_TRYNRL_MME    = 5,  // 3G/RCS Interface
	STATISTICS_TRYNRL_UDM    = 6,  // UDM
	STATISTICS_TRYNRL_SMSF   = 7, // 5G SMSF SGd Interface
	STATISTICS_TRYNRL_SC_P_SGD = 8,  // SMSC Routing SGd Interface
	STATISTICS_TRYNRL_SC_S_SGD = 9,  // SMSC Routing SGd Interface 
	STATISTICS_TRYNRL_DSDS_GSM = 10,
	STATISTICS_TRYNRL_DSDS_MME = 11,
	STATISTICS_TRYNRL_DSDS_SMSF = 12,
	STATISTICS_TRYNRL_CSCF = 13,
} eStaticTryNetRole;

//	TryInterface = STATISTICS_TRYINF_TCP 인 경우에
typedef enum {
	STATISTICS_TRYTRL_SMDPS  = 0,  // SMDPS Interface
	STATISTICS_TRYTRL_SMPP34 = 1,  // SMPP34 Interface
	STATISTICS_TRYTRL_SMCI   = 2,  // SMCI Interface
	STATISTICS_TRYTRL_SMPP   = 3,  // SMPP Interface
	STATISTICS_TRYTRL_SMDP   = 4,  // SMDP Interface
	STATISTICS_TRYTRL_NPQC	 = 5,  // NPQC Interface
	STATISTICS_TRYTRL_PLUS	 = 6,  // PLUS Interface
	STATISTICS_TRYTRL_NUMS 	 = 7   // NUMS Interface
} eStaticTryTcpRole;

//	TryInterface = STATISTICS_TRYINF_INTL 인 경우에
typedef enum {
	STATISTICS_TRYIRL_DBMS   = 0,   // MAIN DBMS Interface
	STATISTICS_TRYIRL_SCHD   = 1,   // Scheduler Interface
	STATISTICS_TRYIRL_UHIS   = 2,   // UHIS History Interface
	STATISTICS_TRYIRL_SDBMS  = 3,   // Statistics DB Interface
	STATISTICS_TRYIRL_RDBMS  = 4,   // Receipt History Interface
	STATISTICS_TRYIRL_DDB    = 5,   // History DB Interface
	STATISTICS_TRYIRL_RSCHD  = 6,   // Scheduler Interface
	STATISTICS_TRYIRL_RUHIS	 = 7,	// Receipt History DB Interface     #added by bjlee 2007.04.27
	STATISTICS_TRYIRL_TDBMS  = 8,	// Transform DB Interface
	STATISTICS_TRYIRL_RCSDB    = 9, 	 	// RCS History DB Interface
	STATISTICS_TRYIRL_RCSSCHD  = 0x0A, 	// RCS History DB Interface
	STATISTICS_TRYIRL_RCSUHIS  = 0x0B, 	// RCS History DB Interface
	STATISTICS_TRYIRL_ERROR = 0x0C // 에러통계 처리
} eStaticTryIntlRole;

//	TryDirection
//	STATISTICS Record의 표시하는 통계의 In/Out을 표시한다
#define MAX_DIRECTION_CLASS		4		/* molee 2->4 */
typedef enum {
	STATISTICS_TRYDIR_IN    	= 0,   // Input
	STATISTICS_TRYDIR_OUT   	= 1,   // Output
	STATISTICS_TRYDIR_OUT_ACK	= 2   // Output Ack molee add
} eStaticTryDirection;

//	TryEvent
//해당 작업의 성격을 표시한다
#define MAX_EVENT_TYPE	2
#define MAX_EVENT_CLASS	12
typedef enum {
	STATISTICS_TRYEVNT_SM 		 	= 0,   // SM을 전달하기 위한 작업
	STATISTICS_TRYEVNT_SMQUERY	 	= 1,   // SM 상태 확인을 위한 작업
	STATISTICS_TRYEVNT_VALIDQUERY	= 2,   // SM 인증을 위한 작업
	STATISTICS_TRYEVNT_ALERT		= 3,   // Noti 작업
	STATISTICS_TRYEVNT_REPORT		= 4,   // SM에 대한 보고 작업
	STATISTICS_TRYEVNT_INFORM		= 5,   // SM에 대한 정보 작업
	STATISTICS_TRYEVNT_PORTED		= 6,   // PORTED
	STATISTICS_TRYEVNT_EXQUERY		= 7	   // SKT FP 2nd Node Try
} eStaticTryEvent;

//	TryInterface = STATISTICS_TRYINF_NET & TryRole = STATISTICS_TRYNRL_TCAP 인 경우에
typedef enum {
	STATISTICS_TRYEVNT_TC_INVOKE 	= 0,   // TC_Invoke
	STATISTICS_TRYEVNT_TC_RESULT 	= 1,   // TC_RESULT
	STATISTICS_TRYEVNT_TC_CANCEL	= 2,   // TC_CANCEL
	STATISTICS_TRYEVNT_TC_ERROR 	= 3,   // TC_ERROR
	STATISTICS_TRYEVNT_TC_REJECT	= 4,   // TC_REJECT
	STATISTICS_TRYEVNT_TC_BEGIN 	= 5,   // TC_BEGIN
	STATISTICS_TRYEVNT_TC_CONTINUE 	= 6,   // TC_CONTINUE
	STATISTICS_TRYEVNT_TC_END 		= 7,   // TC_END
	STATISTICS_TRYEVNT_TC_UNIDIR	= 8,   // TC_UNIDIR
	STATISTICS_TRYEVNT_TC_PABORT	= 9,   // TC_PABORT
	STATISTICS_TRYEVNT_TC_UABORT	= 10,  // TC_UABORT
	STATISTICS_TRYEVNT_TC_NOTICE	= 11  // TC_NOTICE
} eStaticTcapEvent;


//	ProcessID
//	해당 작업을 수행하는 Process의 ID 또는 Esme ID를 표시한다
typedef enum {
	STATISTICS_PROCID_MAST    = 0,   // Master
	STATISTICS_PROCID_SDBMS   = 1,   // STATISTICS DBMS
	STATISTICS_PROCID_ADBMS   = 2,   // Active DBMS
	STATISTICS_PROCID_RDBMS   = 3,   // Receipt DBMS
	STATISTICS_PROCID_SCHD    = 4,   // Scheduler
	STATISTICS_PROCID_UHIS    = 5,   // UHIS
	STATISTICS_PROCID_SMDPS   = 6,   // SMDPS
	STATISTICS_PROCID_ASE1    = 7,   // ASE Master
	STATISTICS_PROCID_ASE2    = 8,   // ASE Master
	STATISTICS_PROCID_ASE3    = 9,   // ASE Master
	STATISTICS_PROCID_ASE4    = 10,  // ASE Master
	STATISTICS_PROCID_ASE5    = 11,  // ASE Master
	STATISTICS_PROCID_ASE6    = 12,  // ASE Master
	STATISTICS_PROCID_ASE7    = 13,  // ASE Master
	STATISTICS_PROCID_ASE8    = 14,  // ASE Master
	STATISTICS_PROCID_NPQC	  = 15,  // NPQC
	STATISTICS_PROCID_PLUS	  = 16,  // PLUS
	STATISTICS_PROCID_RUHIS	  = 17,  // RUHIS       #added by bjlee 2007.04.27
	STATISTICS_PROCID_TDBMS	  = 18,  // TDBMS
	STATISTICS_PROCID_RCSDB   = 19,   // CUHIS
	STATISTICS_PROCID_RCSSCHD   = 20,   // CUHIS
	STATISTICS_PROCID_RCSUHIS   = 21,   // CUHIS
	STATISTICS_PROCID_SGD1 	= 22,
	STATISTICS_PROCID_SGD2 	= 23,
	STATISTICS_PROCID_SGD3 	= 24,
	STATISTICS_PROCID_SGD4 	= 25,
	STATISTICS_PROCID_SGD5 	= 26,
	STATISTICS_PROCID_SGD6 	= 27,
	STATISTICS_PROCID_SGD7 	= 28,
	STATISTICS_PROCID_SGD8 	= 29,
	STATISTICS_PROCID_S6C1 		= 30,
	STATISTICS_PROCID_S6C2 		= 31,
	STATISTICS_PROCID_S6C3 		= 32,
	STATISTICS_PROCID_S6C4 		= 33,
	STATISTICS_PROCID_ISC1 		= 34,
	STATISTICS_PROCID_ISC2 		= 35,
	STATISTICS_PROCID_ISC3 		= 36,
	STATISTICS_PROCID_ISC4 		= 37,
	STATISTICS_PROCID_SH1 		= 38,
	STATISTICS_PROCID_SH2 		= 39,
	STATISTICS_PROCID_SH3 		= 40,
	STATISTICS_PROCID_SH4 		= 41
} eStaticProcessID;


//***********************************************************************
//        SMS Statistics Record Macro
//***********************************************************************

#define	SET_STSRECORD_RECORDTYPE(spStsRecord, RecordType) \
	(spStsRecord)->RecordType = (RecordType & 0xFF)
#define	SET_STSRECORD_TRYINTERFACE(spStsRecord, TryInterface) \
	(spStsRecord)->TryInterface = (TryInterface & 0x03)
#define	SET_STSRECORD_TRYROLE(spStsRecord, TryRole) \
	(spStsRecord)->TryRole = (TryRole & 0x3F)
#define	SET_STSRECORD_TRYDIRECTION(spStsRecord, TryDirection) \
	(spStsRecord)->TryDirection = (TryDirection & 0x03)
#define	SET_STSRECORD_TRYEVENT(spStsRecord, TryEvent) \
	(spStsRecord)->TryEvent = (TryEvent & 0x7F)
#define	SET_STSRECORD_TRYDATALEN(spStsRecord, TryDataLen) \
	(spStsRecord)->TryDataLen = ((u_short)TryDataLen)
#define	SET_STSRECORD_TRYTIME(spStsRecord, TryTime) \
	(spStsRecord)->TryTime = ((u_int)TryTime)
#define	SET_STSRECORD_TRYRESULT(spStsRecord, TryResult) \
	(spStsRecord)->TryResult = ((u_int)TryResult)
#define	SET_STSRECORD_MSGID(spStsRecord, MsgID) \
	(spStsRecord)->MsgID = ((u_int)MsgID)
#define	SET_STSRECORD_PROCESSID(spStsRecord, ProcessID) \
	(spStsRecord)->ProcessID = ((u_short)ProcessID)
#define	SET_STSRECORD_SERVICEID(spStsRecord, ServiceID) \
	(spStsRecord)->ServiceID = ((u_short)ServiceID)
#define SET_STSRECORD_GROUPID(spStsRecord, GroupID)\
    (spStsRecord)->GroupID  = ((u_short)GroupID)
#define SET_STSRECORD_SME(spStsRecord, cpSME) \
    memcpy((spStsRecord)->SME, cpSME, sizeof(spStsRecord->SME))

extern int dbStatisticDBConnect(char *cpCfgFile, int nMsgQtype);
extern int dbStatisticInsertbyRecord(stStatisticsRecord *spStsRecord);
extern int MakeStatisticsRecord(
    stStatisticsRecord 	*spStsRecord,
    // Statistic Record Structure pointer

    u_short  RecordType,
    // Statistic Record type
    // STATISTICS_RECORD_START  - 시작 Record
    // STATISTICS_RECORD_END    - 종료 Record
    // STATISTICS_RECORD_SINGLE - 단일 Record

    u_short  TryInterface,
    // Interface
    // STATISTICS_TRYINF_INTL - Internal
    // STATISTICS_TRYINF_NET  - Network - SS7
    // STATISTICS_TRYINF_TCP  - TCP/IP

    u_short  TryRole,
    // Porcess Type
    // TryInterface = STATISTICS_TRYINF_NET
    // STATISTICS_TRYNRL_TCAP - TCAP Interface
    // STATISTICS_TRYNRL_HLR  - HLR Interface
    // STATISTICS_TRYNRL_IMX  - IMX/MSC Interface
    // STATISTICS_TRYNRL_SGSN - 3G/GSM SGSN Interface
    //	TryInterface = STATISTICS_TRYINF_TCP
    // STATISTICS_TRYTRL_SMDPS  -  SMDPS Interface
    // STATISTICS_TRYTRL_SMPP34 -  SMPP34 Interface
    // STATISTICS_TRYTRL_SMCI   -  SMCI Interface
    // STATISTICS_TRYTRL_SMPP   -  SMPP Interface
    // STATISTICS_TRYTRL_SMDP   -  SMDP Interface
    // STATISTICS_TRYTRL_NPQC   -  NPQC Interface
    // STATISTICS_TRYTRL_PLUS   -  PLUS Interface
    //	TryInterface = STATISTICS_TRYINF_INTL
    //STATISTICS_TRYIRL_DBMS  - MAIN DBMS Interface
    //STATISTICS_TRYIRL_SDBMS - Statistics DB Interface
    //STATISTICS_TRYIRL_RDBMS - Receipt History Interface
    //STATISTICS_TRYIRL_SCHD  - Scheduler Interface
    //STATISTICS_TRYIRL_UHIS  - UHIS History Interface
    //STATISTICS_TRYIRL_DDB   - History DB Interface

    u_short  TryDirection,
    // SM Drection Type
    //STATISTICS_TRYDIR_IN   -  Input
    //STATISTICS_TRYDIR_OUT  -  Output
    //STATISTICS_TRYDIR_OUT_ACK  -  Output

    u_short  TryEvent,
    // Job Event Type
    // Normal
    // STATISTICS_TRYEVNT_SM 		- SM을 전달하기 위한 작업
    // STATISTICS_TRYEVNT_SMQUERY	- SM 상태 확인을 위한 작업
    // STATISTICS_TRYEVNT_VALIDQUERY	- SM 인증을 위한 작업
    // STATISTICS_TRYEVNT_ALERT		- Noti 작업
    // STATISTICS_TRYEVNT_REPORT		- SM에 대한 보고 작업
    // STATISTICS_TRYEVNT_INFORM		- SM에 대한 정보 작업
    // STATISTICS_TRYEVNT_PORTED	    - SM에 대한 정보 작업
    //	TryInterface = STATISTICS_TRYINF_NET & TryRole = STATISTICS_TRYNRL_TCAP
    // STATISTICS_TRYEVNT_TC_INVOKE 	- TC_Invoke
    // STATISTICS_TRYEVNT_TC_RESULT 	- TC_RESULT
    // STATISTICS_TRYEVNT_TC_ERROR 	- TC_ERROR
    // STATISTICS_TRYEVNT_TC_REJECT	- TC_REJECT
    // STATISTICS_TRYEVNT_TC_BEGIN 	- TC_BEGIN
    // STATISTICS_TRYEVNT_TC_CONTINUE- TC_CONTINUE
    // STATISTICS_TRYEVNT_TC_END 	- TC_END
    // STATISTICS_TRYEVNT_TC_UNIDIR	- TC_UNIDIR
    // STATISTICS_TRYEVNT_TC_PABORT	- TC_PABORT
    // STATISTICS_TRYEVNT_TC_UABORT	- TC_UABORT

    u_short  TryDataLen,
    // SM Content Length

    u_int    TryTime,
    // Event End time - Event Start Time

    u_int    TryResult,
    // Event Process Result

    u_int MsgID,
    // Message ID

    u_short  ProcessID,
    //	ProcessID or Esme ID
    // STATISTICS_PROCID_MAST   - Master
    // STATISTICS_PROCID_SDBMS  - STATISTICS DBMS
    // STATISTICS_PROCID_ADBMS  - Active DBMS
    // STATISTICS_PROCIDL_RDBMS - Receipt DBMS
    // STATISTICS_PROCID_SCHD   - Scheduler
    // STATISTICS_PROCID_UHIS   - UHIS
    // STATISTICS_PROCID_SMDPS  - SMDPS
    // STATISTICS_PROCID_ASEM   - ASE Master
    // STATISTICS_PROCID_ASE0   - ASE Master
    // STATISTICS_PROCID_ASE1   - ASE Master
    // STATISTICS_PROCID_ASE2   - ASE Master
    // STATISTICS_PROCID_ASE3   - ASE Master
    // STATISTICS_PROCID_ASE4   - ASE Master

    u_short  ServiceID,
    //	Teleservice ID or Protocol ID

    u_short	GroupID,
    //	Point Code or Global Title
    char			*cpSME
);

// seoki define
// pps static define 2004.04.15 ..
/* ///////////////////////////////////////////////////////
  alread defined
#define	DBMS_ZERO_OID		0x00 		// Decimal :   0
#define	SCHD_ZERO_OID		0x02 		// Decimal :   2
#define	ASE_ZERO_OID		0x50 		// Decimal :  80
#define	ESME_ZERO_OID		0x90 		// Decimal : 144
#define	UHIS_ZERO_OID		0x21 		// Decimal :  33
#define	DDB_ZERO_OID		0x22 		// Decimal :  34
/////////////////////////////////////////////////////// */
#define	PPS_ZERO_OID		0x22 		// Decimal :  34

// dbms ==> pps process //////////////////////////////////////////////////////////////////////////////////////
#define	PPS_ONE_OID_in_s_pps_balance		0x01 	// 01.Decimal : 001 : pps queue recv and have active dbms
#define	PPS_ONE_OID_in_s_pps_report			0x02 	// 02.Decimal : 002 : pps queue recv and have active dbms
#define	PPS_ONE_OID_in_s_pps_etc			0x03 	// 03.Decimal : 003 : pps queue recv and have active dbms
#define	PPS_ONE_OID_in_f_pps				0x04	// 04.Decimal : 004 : pps queue recv but don't have active dbms
// retry just one time
#define	PPS_ONE_OID_retry_s_pps				0x05	// 05.Decimal : 005 : insert success pps queue (balance)
#define	PPS_ONE_OID_retry_f_pps				0x06 	// 06.Decimal : 006 : insert fail pps queue 	(balance)
#define	PPS_ONE_OID_retry_report_s_pps		0x07	// 07.Decimal : 007 : insert success pps queue (report)
#define	PPS_ONE_OID_retry_report_f_pps		0x08 	// 08.Decimal : 008 : insert fail pps queue 	(report)

// pps ==> schduler
#define	PPS_ONE_OID_out_s_schd				0x09	// 09.Decimal : 009 : send success scheduler queue
#define	PPS_ONE_OID_out_f_schd				0x0a	// 10.Decimal : 010 : send fail scheduler queue
// pps ==> uhis
#define	PPS_ONE_OID_out_s_uhis				0x0b	// 11.Decimal : 011 : send success uhis queue
#define	PPS_ONE_OID_out_f_uhis				0x0c 	// 12.Decimal : 012 : send fail uhis queue

// pps <==> timer thread
#define	PPS_ONE_OID_out_s_thread			0x10 	// 13.Decimal : 016 : send success to timer thread 
#define	PPS_ONE_OID_out_f_thread			0x11 	// 14.Decimal : 017 : send fail to timer thread
#define	PPS_ONE_OID_in_s_thread				0x12 	// 15.Decimal : 018 : recv success from timer thread
#define	PPS_ONE_OID_in_f_thread				0x13 	// 16.Decimal : 019 : recv fail from timer thread

// pps <==> win
#define PPS_ONE_OID_pass_balance			0x20 	// 17.Decimal : 032 : pass balance
#define PPS_ONE_OID_out_s_balance			0x21 	// 18.Decimal : 033 : balance query send success
#define PPS_ONE_OID_out_f_balance			0x22 	// 19.Decimal : 034 : balance query send fail
#define PPS_ONE_OID_in_s_postpaid			0x23	// 20.Decimal : 035 : post paid
#define PPS_ONE_OID_in_s_no_balance			0x24	// 21.Decimal : 036 : no balance
#define PPS_ONE_OID_in_s_ok_balance			0x25	// 22.Decimal : 037 : balance enough
#define PPS_ONE_OID_in_s_incorrect_status	0x26	// 23.Decimal : 038 : balance query receive success.(incorrect status)
#define PPS_ONE_OID_in_f_balance			0x27	// 24.Decimal : 039 : balance query receive fail.(time out balance query)

#define PPS_ONE_OID_out_s_report_s			0x30	// 25.Decimal : 048 : send success report ( message status success )
#define PPS_ONE_OID_out_s_report_f			0x31	// 26.Decimal : 049 : send success report ( message status fail )
#define PPS_ONE_OID_out_f_report			0x32	// 27.Decimal : 050 : send fail report
#define PPS_ONE_OID_in_s_report				0x33	// 28.Decimal : 051 : recv success report
#define PPS_ONE_OID_in_f_report				0x34	// 29.Decimal : 052 : recv fail report ( time out report query )
//////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifdef __cplusplus
//}
#endif

#endif	// _STATISTICS_RECORD_H_
