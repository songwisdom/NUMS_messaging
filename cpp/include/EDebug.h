/***********************************************************************
*                                                                      *
*   Copyright (c) 1997-1998 LG Information & Communications            *
*   All rights reserved.                                               *
*                                                                      *
*   This program contains confidential and proprietary information of  *
*   LGIC, and any reproduction, disclosure, or use in whole or in part *
*   is expressly  prohibited, except as may be specifically authorized *
*   by prior written agreement or permission of LGIC.                  *
*                                                                      *
*               LG Information & Communications, Ltd.                  *
*        395-65 , Shindaebang-dong, Dongjak-gu, Seoul, 150-010 Korea   *
*                                                                      *
***********************************************************************/
/*===============================================================================
*       MODULE NAME          : EDebug.h
*       PURPOSE              : Expand Debug.
*       INPUT ARGUMENT       : NONE
*       OUTPUT ARGUMENT      :
*       RETURN VALUE         : NONE
*       REVISION             :     BY             WHEN            REASON
*                             -------------  --------------  -------------
*                              KANG HYUN KWAN  1997.08.25.     first coding
==============================================================================*/

#ifndef __EDEBUG_H__
#define __EDEBUG_H__

/**********************************************************
     command for Debug MsgQ
**********************************************************/
#define ECMD_MSG       4
#define ECMD_START     5
#define ECMD_END       6
#define ECMD_MALLOC   10
#define ECMD_FREE     11
#define ECMD_FOPEN    12
#define ECMD_FCLOSE   13
#define ECMD_OPEN     14
#define ECMD_CLOSE    15
#define ECMD_FORK     20
#define ECMD_EXIT     21
#define ECMD_KILL     22

#define DEBUG_MSGQ_SIZE 60
#define DEBUG_KEY_SIZE  20 /*   */

typedef struct {
	int pid;
	int ppid;
	int command;
	int code;
	char msg[DEBUG_MSGQ_SIZE];
}	DebugMsgQType;

/***********************************************************************
*     Debug 4 Shared Memory
************************************************************************/
#define DEBUG_STOP     0
#define DEBUG_CRITICAL 1
#define DEBUG_MAJOR    2
#define DEBUG_MINOR    3

#define DEBUG_FUNCTION 5
#define DEBUG_STEP     6
#define DEBUG_POINT    7    /*  loop            */
#define DEBUG_DETAIL   8    /*  expand report   */

typedef struct {
	int nMode;    /*  10: detail 20: Step     30: Function 50: Minor
	                  60: Major  70: Critical */
} DebugError;

/***************************************************************
    Expand Trace  : hkkang 1998.06.25
***************************************************************/

#define ETRACE_L1		0x01
#define ETRACE_L2		0x02
#define ETRACE_L3		0x04
#define ETRACE_P1		0x08
#define ETRACE_P2		0x10
#define ETRACE_P3		0x20
#define ETRACE_PF		0x40
#define ETRACE_PT       0x80

/*  ETRACE Command : hkkang : 1998.07.02   */
#define ETRACE_CMD_OPEN_FILE             0x01
#define ETRACE_CMD_CLOSE_FILE            0x02
#define ETRACE_CMD_ALL_PROC_OPEN_FILE    0x03
#define ETRACE_CMD_ALL_PROC_CLOSE_FILE   0x04

typedef struct {
	int nMode;
	int nCnt;
	int nCommand;
	int nPId;
	char cpFileName[BUF_SSIZE];
} ETraceType;


/******************************************************************************
            Trace & Debug    : 1998.01.20
******************************************************************************/

typedef struct {
	int           nVar[BUF_SSIZE];
	u_long 		  ulVar[BUF_SSIZE];
	char         	  cpBuf[BUF_SSIZE][BUF_SSIZE];
	int           nQSize;
	int           nQHead;
	int           nQTail;
	time_t        lnTime[BUF_SSIZE];
	int           cpLog[BUF_SSIZE][BUF_SSIZE];
} ETraceVarDataType;

typedef struct {
	ETraceVarDataType *spData;
	EShmType *spShm;
} ETraceVarType;


#ifndef __EDEBUG_C__

extern DebugMsgQType DebugMsgQ, DebugMsgQRcv;
//extern char cpINIFileName[BUF_SSIZE];
extern char cpTempFileName[BUF_SSIZE];
extern char cpProcName[BUF_SSIZE];
extern char cpSysErrorName[BUF_SSIZE];

extern char cpSysDebugMode[DEBUG_KEY_SIZE],
       cpDebugMsgQKey[DEBUG_KEY_SIZE],          /* MESSAGE QUEUES Key */
       cpDebugShMKey[DEBUG_KEY_SIZE],           /* SHARED MEMORY Key  */
       cpDebugSemKey[DEBUG_KEY_SIZE];           /* SEMAPHORES Key     */
extern int	nDebugMsgQID,
        nDebugShMID,
        nDebugSemID;
extern int DebugMode;
extern int nDebugCnt, nDebugCntAlloc, nDebugCntFree;

extern ETraceVarType *spTraceVar;

#endif


#ifdef __cplusplus
//extern "C" {
#endif


void SysError(const char *cpFormat, ...);

int DisplayProcessingStatus(char *cpMsg, int nInterval);

void DebugSendMsgQ(char *cpBuf, int nSize);

void OutError(const char *cpFormat, ...);

void OutputHexData(char *cpBuf, int nLen);

void SetErrorNo(int nErrno, const char *cpStr);

/***************************************************************
    Expand Trace  : hkkang 1998.06.25
***************************************************************/


void SetETraceAllOpenFile(char *cpFileName);



/***************************************************************
    Expand Log Mutil : hkkang 1998.11.02
***************************************************************/

/******************************************************************************
            Trace & Debug    : 1998.01.20
******************************************************************************/


/******************************************************************************
         Local Trace & Debug    : 1998.01.20
******************************************************************************/

void EPrintf(const char *cpFormat, ...);


#ifdef __cplusplus
//}
#endif

#endif
