/*******************************************************************************
*
*     Copyright (c) 2003 FEELINGK Co., Ltd.
*     All rights reserved.
*
*     This program contains confidential and proprietary information of
*     FEELINGK, and any reproduction, disclosure, or use in whole or in part
*     by prior written agreement or permission of FEELINGK.
*
*                            FEELINGK Co., Ltd.
*     3F Ace Techno Twin Tower 1, 212-1 Kuro-Dong, Kuro-Gu, Seoul 152-050 Korea
*
*******************************************************************************/
/*===============================================================================
*       MODULE NAME          : E4Noti.h
*       PURPOSE              : Expand For Notification DB.
*       INPUT ARGUMENT       : NONE
*       OUTPUT ARGUMENT      :
*       RETURN VALUE         : NONE
*       REVISION             :     BY             WHEN            REASON
*                             -------------  --------------  -------------
*                              YANG SEOUNG MAN   2004.11     Modify coding
==============================================================================*/
#ifndef __E4NOTI_H__
#define __E4NOTI_H__

#include <sys/types.h>
#include <FLKBaseDef.h>
#include <EMacro.h>
#include <EFile.h>
#include <EIpc.h>
#include <EMinMask.h>

#ifndef MIN_SIZE
#define MIN_SIZE 12
#endif

#define MAX_ERROR_HISTORY 5
#define EVENT_CHANGE_RETRY_ALGO 0x90

typedef struct {
	u_int lnNextTime2Uhis;
	u_int dummy[3];
} SCHDRetryFlag;
/*
typedef struct
{
    time_t lnNextTime2Uhis;
    int dummy;
} SCHDRetryFlag;
*/


typedef struct {
	u_int  	unMsgId;
	int     nStatus;
} MsgId2SchdType;

typedef struct {
	u_int 	unMsgId;
	int     nStatus;
	char      cpMin[MIN_SIZE];
} RetryDataType;

typedef struct {
	int nMqKey;
	EMQType *spMq;
	int nQSize;
} EOverwriteMsgQType;

#define MAX_STEP_CNT				10
typedef struct {
	int nTime;
	int nRepeat;
} RetryStepType;

typedef struct {
	u_short 	nFrame;
	u_short 	nErrorCode;
	u_int	unTimeOut;
	u_int 	nStepCnt;
	RetryStepType spStep[MAX_STEP_CNT];
} stRetryFrameType;


typedef struct {
	long 	msg_type;
	u_int 	msgid;
} stMsgQ4MME;
enum eUhisTimeChk {
	NOCHK_2UHIS = 0, CHK_2UHIS = 1
};

/*** Add by red9s for SKT New Service ******/
typedef struct {
	u_int   	unMsgID;
	u_char		ucDbType;
	u_char		ucMsgType;
	u_short	usStatus;
} stEsmeQInputType;

enum eMsgType {
	MSGT_DELIVERY = 0x01,
	MSGT_RECEIPT  = 0x02,
	MSGT_SERVICE  = 0x03,
	MSGT_PEER_SVC = 0xF0,
	MSGT_UNKNOWN  = 0xFF
};

enum eSKTServiceType {
	SKT_SVC_PICS  = 0x01,
	SKT_SVC_ICS   = 0x02,
	SKT_SVC_SNAP  = 0x03,
	SKT_SVC_SCSMS = 0x04,
	SKT_SVC_UNKNOWN = 0xFF
};

enum eS6cSendType {
	MAP_MT_FAIL_TO_DIA_HLR_REPORT = 0x11,
	MAP_MT_SUCC_TO_DIA_HLR_REPORT = 0x12
};

enum eSmscInterfaceType {
	INTERFACE_MAP_C_TYPE = 0x00,
	INTERFACE_DIAMETER_S6C_TYPE = 0x01
};

/*** Add by J.D Shim for Dual Active Machine Status Board ****/
//enum eMachineStatus {
//       STATUS_START = 0x00, STATUS_ACTIVE,
//		STATUS_STANDBY, 	 STATUS_UNKNOWN = 0xFF
//};

enum eMachineStatus {
	MCST_START = 0x00, MCST_HAVE_RIP, MCST_NO_HAVE_RIP, MCST_UNKNOWN = 0xFF
};


enum ePeerServerStatus {
	PEER_START = 0x00,	PEER_RUNNING,		PEER_DISCONNECT,
	PEER_SERVER_DOWN,	PEER_MACHINE_DOWN,	PEER_UNKNOWN = 0xFF
};

typedef struct {
	time_t	nChangeTime;	/* Last Recode Time */
	u_char	ucMyMcStatus;	/* My Machine Status (one of eMachineStatus) */
	u_char	ucPeerSvrStatus;/* Peer Server Status(one of ePeerServerStatus) */
} stMcStatusBoard;

#define IS_MC_STATUS(state)	\
			((MCST_START <= (state) && MCST_NO_HAVE_RIP >= (state)) ? \
			 true : false)

#define strmcstatus(state)	\
			(MCST_START		== (state)) ? "MCST_START"	\
			:(MCST_HAVE_RIP	== (state)) ? "MCST_HAVE_RIP"	\
			:(MCST_NO_HAVE_RIP	== (state)) ? "MCST_NO_HAVE_RIP"	\
			: "Unknown Machine Status"

#define IS_PEER_STATUS(state)	\
			((PEER_START <= (state) && PEER_MACHINE_DOWN >= (state)) ? \
			 true : false)

#define strsvrstatus(state)	\
			(PEER_START			== (state)) ? "PEER_START"			\
			:(PEER_RUNNING		== (state)) ? "PEER_RUNNING"		\
			:(PEER_DISCONNECT	== (state)) ? "PEER_DISCONNECT"		\
			:(PEER_SERVER_DOWN	== (state)) ? "PEER_SERVER_DOWN"	\
			:(PEER_MACHINE_DOWN	== (state)) ? "PEER_MACHINE_DOWN"	\
			: "Unknown Server Status"

/*** Add for Dual Active MsgID Communication ***/
typedef struct stMsgIdToDualActType {
	u_int 		unMsgId;
	u_char 		ucTupleType;
} MsgIdToDualActType;

/* Define Tuple Message Type */
enum eTupleMsgType {
	NO_TUPLE_MSG = 0x00, ACTIVE_TUPLE, RECEIPT_TUPLE, INTERACTIVE_TUPLE
};

#define TUPLE_TYPE_NAME(type)	\
			(ACTIVE_TUPLE == type)		? "ACTIVE_TUPLE"	\
			:(RECEIPT_TUPLE == type)	? "RECEIPT_TUPLE"	\
			:(INTERACTIVE_TUPLE == type)? "INTERACTIVE_TUPLE" : "NO_TUPLE_MSG"

#define isTuple(type)	\
			(NO_TUPLE_MSG < type && INTERACTIVE_TUPLE >= type) ? true : false


/**********
   Add for SMSPlus MsgID to BMCH (Blocing Message Checker)
                                               ***********/
typedef struct stMsgIdToBMCHType {
	u_int 		unMsgId;
	u_char      ucMsgType;
} MsgIdToBMCHType;

/* Define Tuple Message Type */
typedef enum {
	NO_ACTIVE_MSG = 0x00, TOP_MSG, NO_TOP_MSG
} eMdbMsgType;

/* SMFC Message ID Putsh Type */
typedef enum {
	SMFC_MTNON, SMFC_NORMAL, SMFC_EXPRESS, SMFC_MTMAX
} eSmfcMsgType;

#define MSG_TYPE_NAME(type)   \
			(NO_TOP_MSG == type) ? "NO_TOP_MSG on Active" \
            :(TOP_MSG == type) ? "TOP_MSG on Acitve" \
            :(NO_ACTIVE_MSG == type) ? "NO_ACTIVE_MSG on Active" \
			: "Unknown MsgId"

#define isMsgType(type)   \
            (NO_ACTIVE_MSG < type && NO_TOP_MSG >= type) ? 0x01 : 0x00
/**********
    Add for SMSPlus MsgID to SMFC (Short Message Forward Controler)
                                                ***********/
typedef struct stMsgIdToSMFCType {
	u_int 		unMsgId;
	u_char      ucStatus;
} MsgIdToSMFCType;


/******************************************************
     Get MIN Information  : 1998.08.10 hkkang
******************************************************/

#ifdef __cplusplus
//extern "C" {
#endif


u_int *_GetMsgIdList(int nDbmsHandle, char *cpDstMin, int nStatus);

u_int *GetMsgIdList(char *cpDstMin);

u_int *_RcsGetMsgIdList(int nDbmsHandle, char *cpDstMin, int nStatus);

u_int *GetReceiptMsgIdList(char *cpDstMin);

u_int *GetRcsMsgIdList(char *cpDstMin);

int GetVmsMsgId(char *cpSrcMin, char *cpDstMin);

int GetSmciMsgId(char *cpDstMin, int nTeleserviceID, u_int *unMsgId);

int GetStartMsgId(char  *cpDstMin,  u_int *unMsgId);

int GetSmdpMsgId(u_char *ucpSrcMin, u_char *ucpDstMin, u_int *unMsgId);

int PutMsgId2Uhis(int nDbmsHandle, u_int unMsgId, int bChkFlag);

int PutMsgId2UhisTrans(int nDbmsHandle, u_int unMsgId, int bChkFlag);

int PutMsgId2Uhis4DBMS(u_int unMsgId);

u_int GetMsgId2Uhis(int nDbmsHandle);

u_int GetMsgId2UhisTrans(int nDbmsHandle);

u_int GetMsgIdAUhis(int nDbmsHandle);

int PutMsgId2SchdSndMsg(int nDbmsHandle, u_int unMsgId, int nStatus);

u_int GetMsgId2SchdSndMsg(int nDbmsHandle, int *nStatus);

int PutMsgId2SchdSRsvd(int nDbmsHandle, u_int unMsgId, int nStatus);

u_int GetMsgId2SchdSRsvd(int nDbmsHandle, int *nStatus);

u_int GetMsgId2SchdSRetry(int nDbmsHandle, int *nStatus);

int PutMsgId2SchdSRetry(int nDbmsHandle, u_int unMsgId, int nStatus);

//SR124 BMT
int getMsgQ4SchdSendMsgCnt(int dbms_type);
int getMsgQ4SchdRetryMsgCnt(int dbms_type);
int getMsgQ4SchdRsvdMsgCnt(int dbms_type);
int getMsgQ4UhisMsgCnt(int dbms_type);

int CloseELib(void);

int updateActiveLinkTable(int nDbmsHandle, u_int unMsgId,
                          u_int ulForwardLink, u_int ulBackwardLink,
                          int nStatus, int nReservation);

int _ManagementLinkDelete(int nDbmsHandle, u_int unMsgId, int nStatus, int nOwer);

int ManagementLinkDelete(int nDbmsHandle, u_int unMsgId, int nStatus);

int ManagementLinkDeleteCont(int nDbmsHandle, u_int unMsgId, int nStatus);

int _ManagementLinkDeleteCont(int nDbmsHandle, u_int unMsgId, int nStatus, int nOwer);

int getRetryFramePlan(u_int unFrameErrorCode, stRetryFrameType **spRetryFrame);

time_t GetDefaultExpiredInterval(int nFrame);

time_t GetDefaultExpiredTime(int nFrame);

int SetRetryFrame(stRDActiveTuple   *spActive, char cRetryFrame);

int Set5GRetryFrame(stRDActiveTuple   *spActive, char cRetryFrame);

int SetSubmittedStatus(int nDbmsHandle, u_int unMsgId);

int SetMinMaskStatus(int nDbmsHandle, char *cpMin, int nStatus);

EOverwriteMsgQType *CreateOverwriteMsgQ(int nMqKey, int nQSize);

void CloseOverwriteMsgQ(EOverwriteMsgQType *spOmq);

int SendOverwriteMsgQ(EOverwriteMsgQType *spOmq, char *cpStr, int nSize);

int RecvOverwriteMsgQ(EOverwriteMsgQType *spOmq, char *cpStr, int nSize);

/**********
 	Add by J.D Shim for Dual Active DestMin Routing
                                                      ***********/
/*
 *  Role     : Get the Own Side of DestMin.
 *  Input    : iMyrouteMode - My Side Route Mode. 0 or 1.
 *  		   ucpBCDMIN - BCD format Destination MIN.
 *  		   tBytes - size of cpBCDMIN
 *  Return   : If iMyRoutingMode same as the last number of cpBCDMIN % 2,
 *             it returns MY_SIDE. Otherwise, OTHER_SIDE is returned.
 *             Set No errno.
 */
int WhichSideBCDMin(int iMyRoutingMode, const u_char *ucpBCDMIN, int nLength,
                    int nMdnRoutUse, size_t tBytes);

int WhichSideSTRMin(int iMyRoutingMode, const char *cpSTRMIN,
                    int nMdnRoutUse, size_t tBytes);

/*
#define WhichSideMin(mode, min)	WhichSideBCDMin(mode, min, DB_DEST_MIN_SIZE)
*/

/**********
	Add for Dual Active Machine Status Board
												***********/
int CreateMSBoard(EShmType **sppMcStBdHd);
/*
 * Role		: Get the machine status out of the Status Board in shared memory.
 * Input	: spMSBoard - a pointer of the Machine Status Board structure.
 * Return	: Upon successful completion, it returns SUCCESS.
 *			  Otherwise, FAIL is returned.
 *			  (this case is "Shared Memory Open Fail")
 */
//int CheckMachineStatus(stMcStatusBoard* spMSBoard); old
//int CheckMachineStatus(EShmType* spMcStBdHd, stMcStatusBoard* spMcStBd);
int CheckMSBoard(EShmType *spMcStBdHd, stMcStatusBoard *spMcStBd);

/*
 * Role		: Get the Machine Status Board Pointer in shared memory.
 * Input	: None.
 * Return	: Upon successful completion, it returns stMcStatusBoard's Pointer.
 *			  Otherwise, NULL is returned.
 *			  (this case is "Shared Memory Open Fail")
 */
//stMcStatusBoard* GetMachineStatusBoard(void); old
//int GetMachineStatusBoard(EShmType * spMcStBdHd);
int GetMSBoard(EShmType *spMcStBdHd, stMcStatusBoard **sppMcStBd);


/*
 * Role		: Write the machine status on the Status Board in shared memory.
 * Input	: tCTime	- status recod time.
 *			  ucMcStat	- My Machine Status, one of eMachineStatus.
 *			  ucPeerStat- Peer Server Status, one of ePeerServerStatus.
 * Return	: Upon successful completion, it returns SUCCESS.
 *			  Otherwise, FAIL is returned.
 *			  (this case is "Shared Memory Open Fail")
 */
//int WriteMachineStatus(time_t tCTime, u_char ucMcStat, u_char ucPeerStat);old
int WriteMSBoard(EShmType *spMcStBdHd, stMcStatusBoard *spMcStBd);

/*
 * Role		: Delete Shared Memory
 * Input	: None.
 * Return	: Upon successful completion, it returns SUCCESS.
 *			  Otherwise, FAIL is returned.
 *				(this case is "No Exsist Shared Memory")
 */
int CloseMSBoard(EShmType *spMcStBdHd);

/**********
	Add for Dual Active MsgID Communication
												***********/
/*
 * Role		: Put a MsgId and TupleType into Shared Memory Event Queue.
 * Input	: unMsgId - Message ID in MDB, ucTupleType - one of eTupleMsgType.
 * Return	: Upon successful completion, it returns SUCCESS(0).
 *			  If ucTupleType is not one of eTupleMsgType, it returns
 *				FAIL(-1) and set errno(EINVAL).
 *			  Otherwise, FAIL(-1) is returned and set errno.
 *			  (this case is "Shared Memory Open Fail")
 */
int PutMsgIdToDualAct(u_int unMsgId, u_char ucTupleType);

/*
 * Role		: Get a MsgId and TupleType from Shared Memory Event Queue.
 * Input	: ulpMsgId - Messag ID buffer.
 * Return	: Upon successful completion, it returns one of eTupleMsgType.
 *			  If there is not a TupleMsgId in Event Queue, it returns
 *				NO_TUPLE_MSG( 0x00 ) and set errno (ENOMSG).
 *			  Otherwise, FAIL(-1) is returned.
 *			  (this case is "Shared Memory Open Fail")
 */
u_char GetMsgIdFromDualAct(u_int *ulpMsgId);

/*
 * Role		: Delete Shared Memory
 * Input	: None.
 * Return	: Upon successful completion, it returns SUCCESS.
 *			  Otherwise, FAIL is returned.
 *				(this case is "No Exsist Shared Memory")
 */
int ClosseEQueueToDualAct(void);
/******************************************************************************
				for Communication with SMS+.
******************************************************************************/
/*
 * Role		: Put a MsgId and Message Type into Shared Memory Event Queue.
 * Input	: unMsgId - Message ID in MDB, ucMsgType - one of eMsgType.
 * Return	: Upon successful completion, it returns SUCCESS(0).
 *			  Otherwise, FAIL(-1) is returned and set errno.
 *			  (this case is "Shared Memory Open Fail")
 */
int PutMsgIdToBMCH(u_int unMsgId, u_char ucMsgType);

/*
 * Role		: Get a MsgId and Message Type from Shared Memory Event Queue.
 * Input	: ucpMsgType - a MsgType of eMsgType buffer.
 * Return	: Upon successful completion, it returns a MsgId.
 *			  Otherwise, FAIL(-1) is returned.
 *			  (this case is "Shared Memory Open Fail")
 */
u_int GetMsgIdToBMCH(u_char *ucpMsgType);

/*
 * Role		: Delete Shared Memory Event Queue.
 * Input	: None.
 * Return	: Upon successful completion, it returns SUCCESS.
 *			  Otherwise, FAIL is returned.
 *				(this case is "No Exsist Shared Memory")
 */
int ClosseEQueueToBMCH(void);

/*
 * Role		: Put a MsgId and Message Type into Shared Memory Event Queue.
 * Input	: unMsgId - Message ID in MDB, ucStatus - Message Status in MDB.
 * Return	: Upon successful completion, it returns SUCCESS(0).
 *			  Otherwise, FAIL(-1) is returned and set errno.
 *			  (this case is "Shared Memory Open Fail")
 */
int PutMsgIdToSMFC(u_int unMsgId, u_char ucStatus);
int PutMsgIdToSMFCExpress(u_int unMsgId, u_char ucStatus);

/*
 * Role		: Get a MsgId and Message Type from Shared Memory Event Queue.
 * Input	: ucpStatus - a Message Status of a Active Message in MDB.
 * Return	: Upon successful completion, it returns a MsgId.
 *			  Otherwise, FAIL(-1) is returned.
 *			  (this case is "Shared Memory Open Fail")
 */
u_int GetMsgIdToSMFC(u_char *ucpStatus);

/*
 * Role		: Delete Shared Memory Event Queue.
 * Input	: None.
 * Return	: Upon successful completion, it returns SUCCESS.
 *			  Otherwise, FAIL is returned.
 *				(this case is "No Exsist Shared Memory")
 */
int ClosseEQueueToSMFC(void);
void InitFlxRtyProc(    time_t      cTime,
                        int         nMode,
                        int         nTimeFlag,
                        int         nQueueFlag,
                        int         nSTime,
                        int         nETime,
                        int         nQueueCnt,
                        int         nFlxModeFrame,
                        int         nEvtModeFrame);

void ClearFlxRtyProc( void );

int PutFlxRtyProc(  int             nMode,
                    int             nTimeFlag,
                    int             nQueueFlag,
                    int             nSTime,
                    int             nETime,
                    int             nQueueCnt,
                    int             nFlxModeFrame,
                    int             nEvtModeFrame );

int PutFlxRtyProc_Frame( int nFrame );

FlxRetryType *GetFlxRtyProc( void );

u_short GetFlxRtyProc_InProcess( void );
time_t str2timet(u_char *ucTimestr);
void CloseFlxRtyProc( void );

#define SMSC_MSGID_RETRY_MAXNUM  		200
#define SMSC_MSGID_SLEEP_MTIME  		100
u_int getMsgId( EMQType *spMsgID_Q);

int ManagementRcsLinkDelete( u_int unMsgId, int nStatus);
int relink_equirement_prototocol_error(stRDActiveTuple *spActive);
int PutMsgId2MME(u_int unMsgId);
int PutMsgId2SGd(u_int unMsgId, long status);
bool GetMsgId2MME(u_int *msgid, long *status);
EMQType *CreateMMEQueue();
int GetMME_QNum();

int PutMsgId2ISC(u_int unMsgId, long status);
bool GetMsgId2ISC(u_int *msgid, long *status);
EMQType *CreateISCQueue();
int GetISC_QNum();

int PutMsgId2IEP(u_int unMsgId, long status);
bool GetMsgId2IEP(u_int *msgid, long *status);
EMQType *CreateIEPQueue();
int GetIEP_QNum();

// for Dia HLR
int PutMsgId2DiaHLR(u_int unMsgId, long lStatus);
bool GetMsgId2DiaHLR(u_int *msgid, long *status);
EMQType *CreateDiaHLRQueue();

// for Ase HLR
int PutMsgId2Ase(u_int unMsgId, long lStatus);
bool GetMsgId2Ase(u_int *msgid, long *status);
EMQType *CreateAseQueue();

#ifdef __cplusplus
//} // End of extern "C"
#endif

#endif /* #ifndef __E4NOTI_H__ */
