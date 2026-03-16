
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
	FILE NAME		:	MdbTrigger.h
	PURPOSE			:	trigger memory db
	DATE OF WRITE	: 	2004.08.15
	AUTHOR			: 	kimny
	REVISION		:	V 1.0
	Ver.	By		When		Reason
	------- ------- ----------  -----------------------------------------
	1.0		kimny	2004.08.15	first coding
***********************************************************************/


#include "FLKBaseDef.h"

typedef enum {
	TRIGGER_BEFORE_INSERT,
	TRIGGER_AFTER_INSERT,
	TRIGGER_BEFORE_DELETE,
	TRIGGER_AFTER_DELETE,
	MAX_TABLE_TRIGGER_CNT
} eTableTriggerType_t;

typedef int (*FTableTrigger)(char *, int, int);

#define CALL_TABLE_TRIGGER(tableId, triggerType, noAlign, nResult)	\
	fTableTrigger[gnDbmsId][tableId][triggerType](noAlign, tableId, nResult)

typedef enum {
	TRIGGER_BEFORE_LOADING,
	TRIGGER_AFTER_LOADING,
	TRIGGER_BEFORE_BACKUP,
	TRIGGER_AFTER_BACKUP,
	MAX_DBMS_TRIGGER_CNT
} eDbmsTriggerType_t;

typedef int (*FDbmsTrigger)();

#define CALL_DBMS_TRIGGER(triggerType)	\
			fDbmsTrigger[gnDbmsId][triggerType]()


////////////////////////////////////////////////////////////////////////////////
// Initialize Function
////////////////////////////////////////////////////////////////////////////////
int InitTriggerFunc();

////////////////////////////////////////////////////////////////////////////////
// Table Trigger Define
////////////////////////////////////////////////////////////////////////////////
int fNullTableTrigger(char *, int, int);
int ActiveBeforeInsert(char *, int, int);
int ActiveAfterInsert(char *, int, int);
int ActiveBeforeDelete(char *, int, int);
int ActiveAfterDelete(char *, int, int);
int StatBeforeInsert(char *, int, int);
int ReceiptAfterInsert(char *, int, int);
int	ReceiptBeforeDelete(char *, int, int);
int ReceiptAfterDelete(char *, int, int);
int TransformAfterInsert(char *, int, int);
int	TransformBeforeDelete(char *, int, int);
int TransformAfterDelete(char *, int, int);
int SubmitAfterInsert(char *, int, int);

////////////////////////////////////////////////////////////////////////////////
// DBMS Trigger Define
////////////////////////////////////////////////////////////////////////////////
int fNullDbmsTrigger();
int ActiveBeforeLoading();
int ActiveAfterLoading();
int ActiveBeforeBackup();
int ActiveAfterBackup();
int ReceiptBeforeLoading();
int	ReceiptAfterLoading();
int ReceiptBeforeBackup();
int ReceiptAfterBackup();
int PlusBeforeLoading();
int PlusAfterLoading();
int	PlusBeforeBackup();
int StatBeforeLoading();
int	StatAfterLoading();
int	StatBeforeBackup();
int TransformBeforeLoading();
int	TransformAfterLoading();
int TransformBeforeBackup();

int RcsAfterInsert(char *noAlign, int tableId, int nResult);
int RcsBeforeDelete(char *noAlign, int tableId, int nResult);
int RcsAfterDelete(char *noAlign, int tableId, int nResult);
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
