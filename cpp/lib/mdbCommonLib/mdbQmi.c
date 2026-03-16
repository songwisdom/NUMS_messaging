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
*   FILE NAME       :   qmi.c
*   PURPOSE         :
*   DATE OF WRITE   :   2002.03.01
*   AUTHOR          :
*   REVISION        :   V 1.0
*   Ver.    By      When        Reason
*   ------- ------- ----------  -----------------------------------------
*   1.0             2002.03.01  first coding
******************************************************************************/

#include <spdlog/spdlog.h>
#include <signal.h>
#include <unistd.h>
#include <poll.h>
#include <errno.h>
#include <sys/msg.h>
#include <MdbCommon.h>
#include <MdbTrigger.h>
#include <MdbMessage.h>
#include <MdbDmaqi.h>
#include <MdbTableInfo.h>
#include <MdbQmi.h>
#include <SmsIpc.h>
#include <stdlib.h>


#define	MAX_MSGQ_RETRY    10
#define	MAX_MSGQ_FULL_RETRY    10

static int setResult(IDs *pQmiInfo, Result *result, int tag);
static int setMesg(IDs *pQmiInfo, int event, const char *info, int size, int flag);
static int setMesgStat(IDs *pQmiInfo, int event, const char *info, int size, int flag);

IDs *gpQmiInfo[MAX_DBMS_CNT] = {NULL, NULL, NULL, NULL, NULL, NULL};

extern	char	*qp_ErrString[];
extern  char	*dbms_ErrString[];

static const	int nBaseKey[MAX_DBMS_CNT] =
	{STATDB_BASE_KEY, ACTVDB_BASE_KEY, RECTDB_BASE_KEY, PLUSDB_BASE_KEY, TRANDB_BASE_KEY, RCSADB_BASE_KEY};

int db_connect(int nDbmsType, long ltype)
{
	int		rst;

	if (nDbmsType >= MAX_DBMS_CNT || nDbmsType < 0) {
		spdlog::error("Invalid DBMS Type.[{}]", __func__);
		return FLK_FAIL;
	}

	//////////////////////////////////////////////////////
	// get dbms key
	//////////////////////////////////////////////////////

	if (gpQmiInfo[nDbmsType] == NULL) {
		gpQmiInfo[nDbmsType] = (IDs *)malloc(sizeof(IDs));
		memset(gpQmiInfo[nDbmsType], 0x00, sizeof(IDs));
	}
	//	setting pQmiInfo;
	//////////////////////////////////////////////////////

	IDs *pQmiInfo = gpQmiInfo[nDbmsType];

	pQmiInfo->stDBMSKey.MQKeyDBMSQ = (key_t)(DBMSQ + nBaseKey[nDbmsType]);
	pQmiInfo->stDBMSKey.MQKeyDBMSQ_RESULT = DBMSQ_RESULT + nBaseKey[nDbmsType];
	pQmiInfo->stDBMSKey.MQKeyDBMSQ_BACKUP = DBMSQ_BACKUP + nBaseKey[nDbmsType];

	//SR124 BMT
	if (nDbmsType == DBMS_STAT) {
		for (int statQseq = 0; statQseq < MAX_STAT_QCNT; statQseq++) {
			pQmiInfo->stDBMSKey.MQKeyDBMSQ_STAT[statQseq] = (key_t)(DBMSQ_STAT + nBaseKey[nDbmsType] + statQseq);
		}
	}

	pQmiInfo->stDBMSKey.SMKeyDBMS = SHMDBMS + nBaseKey[nDbmsType];
	pQmiInfo->stDBMSKey.SMKeyDBKEY = SHMDBKEY + nBaseKey[nDbmsType];
	pQmiInfo->stDBMSKey.SMKeyINDEX = SHMINDEX + nBaseKey[nDbmsType];
	pQmiInfo->stDBMSKey.SMKeySemBase = SHMDBKEY + nBaseKey[nDbmsType];

	int nSrcQid = msgget(pQmiInfo->stDBMSKey.MQKeyDBMSQ_RESULT, 0666 | IPC_CREAT);
	if ( nSrcQid < 0 ) {
		return  -QP_APPLICATION_QUEUE_GET_FAIL;
	}

	pQmiInfo->srcQid = nSrcQid;
	pQmiInfo->type = ltype;

	if ((pQmiInfo->dstQid = msgget(pQmiInfo->stDBMSKey.MQKeyDBMSQ, 0666)) < 0 ) {
		return -QP_DBMS_QUEUE_GET_FAIL;
	}

	//SR124 BMT
	if (nDbmsType == DBMS_STAT) {
		for (int statQseq = 0; statQseq < MAX_STAT_QCNT; statQseq++) {
			if ((pQmiInfo->statQid[statQseq] = msgget(pQmiInfo->stDBMSKey.MQKeyDBMSQ_STAT[statQseq], 0666 | IPC_CREAT)) < 0 ) {
				return -QP_DBMS_QUEUE_GET_FAIL;
			}
		}
	}

	if ((rst = setMesg(pQmiInfo, DBMS_CONNECT, "_TARGET_DBMS_TEST__", 20, RESULT)) < 0) {
		return rst;
	}

	Result	result;
	if ((rst = setResult(pQmiInfo, &result, 0)) < 0) {
		return rst;
	}

	pQmiInfo->connect_flag = 1;

	QP_ErrString();
	Dbms_ErrString();
	return FLK_SUCCESS;
}

int db_insert(int nDbmsHandle, int table_id, void *tuple, Result *result, int flag)
{
	if (nDbmsHandle >= MAX_DBMS_CNT || nDbmsHandle < 0) {
		spdlog::error("Invalid DBMS Type.[{}]", __func__);
		return FLK_FAIL;
	}

	// caution : check nDbmsHandle
	IDs *pQmiInfo = gpQmiInfo[nDbmsHandle];

	if (pQmiInfo == NULL) {
		spdlog::error("[{}] - Didn't connect MDB Queue[{}]!!", __func__, nDbmsHandle);
		return FLK_FAIL;
	}

	if (pQmiInfo->connect_flag != 1) {
		return -QP_NOT_SETUP;
	}
	if (table_id < 0 || table_id >= MAX_TABLE) {
		return -QP_INVALID_TABLE_ID;
	}
	if ((char *)tuple == NULL) {
		return -QP_TUPLE_NULL_POINTER;
	}

	Insert  insert;
	insert.table_id = table_id;
	memcpy(insert.tuple, (char *)tuple, MAX_TUPLE_SIZE);

	int temp = setMesg(pQmiInfo, INSERT, (char *)&insert, sizeof(Insert), flag);
	if (temp  < 0) {
		return temp;
	}

	if (flag & NO_RESULT) {
		return 1;
	}

	return (setResult(pQmiInfo, result, INSERT));
}
int db_insert_oldpkg(int nDbmsHandle, void *tuple, size_t size)
{
	if (nDbmsHandle >= MAX_DBMS_CNT || nDbmsHandle < 0) {
		spdlog::error("Invalid DBMS Type.[]", __func__);
		return FLK_FAIL;
	}

	// caution : check nDbmsHandle
	IDs *pQmiInfo = gpQmiInfo[nDbmsHandle];

	if (pQmiInfo == NULL) {
		spdlog::error("[{}] - Didn't connect MDB Queue[{}]!!", __func__, nDbmsHandle);
		return FLK_FAIL;
	}

	if (pQmiInfo->connect_flag != 1) {
		return -QP_NOT_SETUP;
	}
	if ((char *)tuple == NULL) {
		return -QP_TUPLE_NULL_POINTER;
	}

	int temp = setMesg(pQmiInfo, INSERT, (char *)tuple, size, NO_RESULT);
	if (temp  < 0) {
		return temp;
	}

	return 1;
}


/* dbStatisticDBDml */
int db_dml(int nDbmsHandle, int table_id, int att_id, char *attval, int attval_size, int tup_id, Result *result, int flag, int event)
{
	Select	select;
	Insert	insert;
	Delete	del;
	Count	count;
	int		temp;

	if (nDbmsHandle >= MAX_DBMS_CNT || nDbmsHandle < 0) {
		spdlog::error("Invalid DBMS Type.[{}]", __func__);
		return FLK_FAIL;
	}

	IDs *pQmiInfo = gpQmiInfo[nDbmsHandle];
	if (pQmiInfo == NULL) {
		spdlog::error("[{}] - Didn't connect MDB Queue[{}]!!", __func__, nDbmsHandle);
		return FLK_FAIL;
	}

	if (pQmiInfo->connect_flag != 1) {
		return -QP_NOT_SETUP;
	}

	if (table_id < 0 || table_id >= MAX_TABLE) {
		return -QP_INVALID_TABLE_ID;
	}

	switch (event) {
	case INSERT:
		if ( attval_size > MAX_TUPLE_SIZE  || attval_size <= 0 ) {
			break;
		}

		insert.table_id = table_id;
		memcpy(insert.tuple, (char *)attval, attval_size);
		if ((temp = setMesg(pQmiInfo, event, (char *)&insert,
		                    sizeof(Insert), flag)) < 0) {
			return temp;
		}
		if (flag & NO_RESULT) {
			return 1;
		}

		return (setResult(pQmiInfo, result, event));

	case SELECT_SEQ:
		select.table_id = table_id;
		select.tup_id  = tup_id;
		if ((temp = setMesg(pQmiInfo, event, (char *)&select, sizeof(Select), flag)) < 0) {
			return temp;
		}
		if (flag & NO_RESULT) {
			return 1;
		}

		return (setResult(pQmiInfo, result, event));

	case SELECT:
		select.table_id = table_id;
		select.att_id = att_id;
		select.tup_id = tup_id;
		memcpy(select.att_val, attval, MAX_ATT_PER_QP);
		if (flag & NOT_EQU) {
			event = SELECT_NEQ;
		}
		if ((temp = setMesg(pQmiInfo, event, (char *)&select, sizeof(Select), flag)) < 0) {
			return temp;
		}
		if (flag & NO_RESULT) {
			return 1;
		}
		if (flag & EXTRA3_SELECT) {
			return SELECT_EX3;
		}

		return (setResult(pQmiInfo, result, event));

	case DELETE:
		del.table_id = table_id;
		del.att_id  = att_id;
		del.tup_id  = tup_id;
		memcpy(del.att_val, attval, MAX_ATT_PER_QP);
		if ((temp = setMesg(pQmiInfo, event, (char *)&del, sizeof(Delete), flag)) < 0) {
			return temp;
		}
		if (flag & NO_RESULT) {
			return 1;
		}

		return (setResult(pQmiInfo, result, event));

	case DELETE_SEQ:
		del.table_id = table_id;
		del.tup_id  = tup_id;
		if ((temp = setMesg(pQmiInfo, event, (char *)&del, sizeof(Delete), flag)) < 0) {
			return temp;
		}
		if (flag & NO_RESULT) {
			return 1;
		}

		return (setResult(pQmiInfo, result, event));

	case COUNT:
		count.table_id = table_id;
		count.att_id  = att_id;
		if (att_id != -1) {
			memcpy(count.att_val, attval, MAX_ATT_PER_QP);
		}

		if ((temp = setMesg(pQmiInfo, event, (char *)&count,
		                    sizeof(Count), flag)) < 0) {
			return temp;
		}
		if (flag & NO_RESULT) {
			return 1;
		}

		return (setResult(pQmiInfo, result, event));

	default:
		return -1;
	}
	return FLK_FAIL;

}

/* dbStatisticDBDml */
int db_dml_stat_update(int nDbmsHandle, int table_id, int att_id, char *attval, int attval_size, int tup_id, Result *result, int flag, int event, int stat_Q_seq)
{
	//Select	select;
	Insert_stat	insert;
	//Delete	delete;
	//Count	count;
	int		temp;
	IDs	*pQmiInfo;

	if (nDbmsHandle >= MAX_DBMS_CNT || nDbmsHandle < 0) {
		spdlog::error("Invalid DBMS Type.[{}]", __func__);
		return FLK_FAIL;
	}

	pQmiInfo = gpQmiInfo[nDbmsHandle];

	if (pQmiInfo == NULL) {
		spdlog::error("[{}] - Didn't connect MDB Queue[{}]!!", __func__, nDbmsHandle);
		return FLK_FAIL;
	}

	if (pQmiInfo->connect_flag != 1) {
		return -QP_NOT_SETUP;
	}

	if (table_id < 0 || table_id >= MAX_TABLE) {
		return -QP_INVALID_TABLE_ID;
	}

	switch (event) {
	case UPDATE :
		if ( attval_size > MAX_TUPLE_SIZE  || attval_size <= 0 ) {
			break;
		}

		insert.table_id = table_id;
		(void)memcpy(insert.tuple, (char *)attval, attval_size);
		pQmiInfo->send_stat_qid = pQmiInfo->statQid[stat_Q_seq];
		if ((temp = setMesgStat(pQmiInfo, event, (char *)&insert, sizeof(Insert_stat), flag)) < 0) {
			return temp;
		}
		if (flag & NO_RESULT) {
			return 1;
		}

		return (setResult(pQmiInfo, result, event));


	default:
		return -1;
	}
	return FLK_FAIL;

}


int db_delete(int nDbmsHandle, int table_id, int att_id, char *attval, int tup_id, Result *result, int flag, int event )
{
//int db_insert(int nDbmsHandle, int table_id, void *tuple, Result *result, int flag)
	Delete	del;
//	Count	count;
	int		temp;
	IDs	*pQmiInfo;

	if (nDbmsHandle >= MAX_DBMS_CNT || nDbmsHandle < 0) {
		spdlog::error("Invalid DBMS Type.[{}]", __func__);
		return FLK_FAIL;
	}

	pQmiInfo = gpQmiInfo[nDbmsHandle];
	if (pQmiInfo == NULL) {
		spdlog::error("[{}] - Didn't connect MDB Queue[{}]!!", __func__, nDbmsHandle);
		return FLK_FAIL;
	}

	if (pQmiInfo->connect_flag != 1) {
		return -QP_NOT_SETUP;
	}
	if (table_id < 0 || table_id >= MAX_TABLE) {
		return -QP_INVALID_TABLE_ID;
	}

	switch (event) {
	case DELETE:
		del.table_id = table_id;
		del.att_id  = att_id;
		del.tup_id  = tup_id;
		memcpy(del.att_val, attval, MAX_ATT_PER_QP);
		if ((temp = setMesg(pQmiInfo, event, (char *)&del, sizeof(Delete), flag)) < 0) {
			return temp;
		}
		if (flag & NO_RESULT) {
			return 1;
		}

		return (setResult(pQmiInfo, result, event));

	case DELETE_SEQ:
		del.table_id = table_id;
		del.tup_id  = tup_id;
		if ((temp = setMesg(pQmiInfo, event, (char *)&del,
		                    sizeof(Delete), flag)) < 0) {
			return temp;
		}
		if (flag & NO_RESULT) {
			return 1;
		}

		return (setResult(pQmiInfo, result, event));

	default:
		return -1;
	}

}

int db_seq_update(int nDbmsHandle, int table_id, int tup_id, int upd_cnt,
                  char *upd_list, void *tuple, Result *result, int flag)
{
	Update  update;
	int temp, tag = UPDATE_SEQ;
	IDs *pQmiInfo;

	if (nDbmsHandle >= MAX_DBMS_CNT || nDbmsHandle < 0) {
		spdlog::error("Invalid DBMS Type.[{}]", __func__);
		return FLK_FAIL;
	}

	pQmiInfo = gpQmiInfo[nDbmsHandle];

	if (pQmiInfo == NULL) {
		spdlog::error("[{}] - Didn't connect MDB Queue[{}]!!",
		       __func__, nDbmsHandle);
		return FLK_FAIL;
	}

	if (pQmiInfo->connect_flag != 1) {
		return -QP_NOT_SETUP;
	}
	if (table_id < 0 || table_id >= MAX_TABLE) {
		return -QP_INVALID_TABLE_ID;
	}
	if (upd_cnt > MAX_ATT_PER_UPDATE) {
		return -QP_UPD_CNT_OVER_LIMIT;
	}
	if (upd_list == NULL) {
		return -QP_UPD_LIST_NULL_POINTER;
	}
	if ((char *)tuple == NULL) {
		return -QP_TUPLE_NULL_POINTER;
	}
	update.table_id  = table_id;
	update.tup_id    = tup_id;
	update.update_cnt = upd_cnt;
	(void)memcpy(update.update_list, (char *)upd_list, MAX_ATT_PER_UPDATE);
	(void)memcpy(update.tuple, (char *)tuple, MAX_TUPLE_SIZE);

	if ((temp = setMesg(pQmiInfo, tag, (char *)&update, sizeof(Update), flag)) < 0) {
		return temp;
	}
	if (flag & NO_RESULT) {
		return 1;
	}

	return (setResult(pQmiInfo, result, tag));
}

/* table_id,att_id,att_val[MAX_ATT_PER_QP]  : using this value, finds tuple */
/*
   upd_cnt
   upd_list : Attid which will be updated.
          For instance,if 5th and 10th is updated,<05><10> is set.
   tuple : AttVal which will be updated.
       For instance,if 5th(0x1234) and 10th(0x12) is updated,
*       000..000<12><34>000..000<12>0000..00  is set.
*/
int db_update(int nDbmsHandle, int table_id, int att_id, char *attval, \
              int tup_id, int upd_cnt, char *upd_list, void *tuple, Result *result, \
              int flag)
{
	Update  update;
	int temp, tag = UPDATE;
	IDs *pQmiInfo;

	if (nDbmsHandle >= MAX_DBMS_CNT || nDbmsHandle < 0) {
		spdlog::error("Invalid DBMS Type.[{}]", __func__);
		return FLK_FAIL;
	}

	pQmiInfo = gpQmiInfo[nDbmsHandle];

	if (pQmiInfo == NULL) {
		spdlog::error("[{}] - Didn't connect MDB Queue[{}]!!",
		       __func__, nDbmsHandle);
		return FLK_FAIL;
	}

	if  (pQmiInfo->connect_flag != 1) {
		return -QP_NOT_SETUP;
	}
	if  (table_id < 0 || table_id >= MAX_TABLE) {
		return -QP_INVALID_TABLE_ID;
	}
	if  (att_id < 0 || att_id >= MAX_ATT_PER_TABLE) {
		return -QP_INVALID_ATT_ID;
	}
	if  (attval == NULL) {
		return -QP_ATTVAL_NULL_POINTER;
	}
	if  (upd_cnt > MAX_ATT_PER_UPDATE) {
		return -QP_UPD_CNT_OVER_LIMIT;
	}
	if  (upd_list == NULL) {
		return -QP_UPD_LIST_NULL_POINTER;
	}
	if ((char *)tuple == NULL) {
		return -QP_TUPLE_NULL_POINTER;
	}

	update.table_id  = table_id;
	update.att_id    = att_id;
	update.tup_id    = tup_id;
	update.update_cnt = upd_cnt;

	(void)memcpy(update.att_val, attval, MAX_ATT_PER_QP);
	(void)memcpy(update.update_list, (char *)upd_list, MAX_ATT_PER_UPDATE);
	(void)memcpy(update.tuple, (char *)tuple, MAX_TUPLE_SIZE);
	if  (flag & EXTRA_UPDATE) {
		tag = UPDATE_EX;
	}
	if   (flag & EXTRA2_UPDATE) {
		tag = UPDATE_EX2;
	}
	if  (flag & EXTRA3_UPDATE) {
		tag = UPDATE_EX3;
	}
	if (flag & ECHO_UPDATE) {
		tag = UPDATE_EO;
	}

	if ((temp = setMesg(pQmiInfo, tag, (char *)&update, sizeof(Update), flag)) < 0) {
		return temp;
	}
	if (flag & NO_RESULT) {
		return 1;
	}

	return (setResult(pQmiInfo, result, tag));
}

void db_error(int err_no)
{
	int ix;

	if (err_no >= 0 || err_no <= -300) {
		return;
	}
	if (err_no < -200) {
		ix = (-err_no) - 200;
		if (qp_ErrString[ix] == NULL) {
			return;
		}
	} else if (err_no < 0) {
		ix = (-err_no);
		if (dbms_ErrString[ix] == NULL) {
			return;
		}
	}
}

/*dbStatisticDBSndMsg*/
static int setMesg(IDs *pQmiInfo, int event, const char *info, int size, int flag)
{
	Message 	mesg;
	struct 		msqid_ds     msgq_stat;
	int			sendQid;
	int			nCnt = 0;
	size_t 		snd_size = 0;

	mesg.mtype = 1;
	mesg.EventName = event;
	//mesg.dummy		= (long)flag;
	mesg.dummy		= flag;
	mesg.SrcQid = pQmiInfo->srcQid;
	mesg.R_type = (int)pQmiInfo->type;
	sendQid = pQmiInfo->dstQid;

	(void)memcpy(mesg.Mesg, info, size);

	/* baram */
	memset(&msgq_stat, 0x00, sizeof(msgq_stat));
	if ((msgctl(sendQid, IPC_STAT, &msgq_stat)) < 0) {
		spdlog::error("[{}][{}]msgctl is failed. errno : {}",
		       __func__, __LINE__, errno);
		return  -QP_MSGQ_INFO_GET_FAIL;
	}

	//snd_size = size + (sizeof(Message) - MAX_MESG_SIZE - sizeof(long));
	snd_size = size + 24;

	//while ( (msgsnd(sendQid, (struct msgBuf *)&mesg, (size_t)size+24, 0) < 0) &&

	while ( (msgsnd(sendQid, (struct msgBuf *)&mesg, snd_size, 0) < 0) &&
	        (++nCnt < MAX_MSGQ_RETRY)) {
		switch (errno) {
		case EAGAIN :
			break;

		case EIDRM :
		case EFAULT :
		case EACCES :
		case EINVAL :
		case EINTR :
		default :
			return  -QP_MESG_SEND_FAIL;
		}
	}
	if (nCnt == MAX_MSGQ_RETRY) { return FLK_FAIL; }

	return FLK_SUCCESS;
}

//SR124 BMT
extern int statdm_max_queue_cnt;
static int setMesgStat(IDs *pQmiInfo, int event, const char *info, int size, int flag)
{
	Message_stat 	mesg;
	struct 		msqid_ds     msgq_stat;
	int			sendQid;
	int			nCnt = 0;
	size_t 		snd_size = 0;

	mesg.mtype = 1;
	mesg.EventName = event;
	//mesg.dummy		= (long)flag;
	mesg.dummy		= flag;
	mesg.SrcQid = pQmiInfo->srcQid;
	mesg.R_type = (int)pQmiInfo->type;

	//ADD 20150107
	sendQid = pQmiInfo->send_stat_qid;

	(void)memcpy(mesg.Mesg, info, size);

	/* baram */
	memset(&msgq_stat, 0x00, sizeof(msgq_stat));
	if ((msgctl(sendQid, IPC_STAT, &msgq_stat)) < 0) {
		spdlog::error("[{}][{}]msgctl is failed. errno : {}",
		       __func__, __LINE__, errno);
		return  -QP_MSGQ_INFO_GET_FAIL;
	}

	//SR124 BMT
	int MQFullCnt = 0;
	if ( statdm_max_queue_cnt < 5000 ) {
		statdm_max_queue_cnt = 30000;
	}

	while (statdm_max_queue_cnt <= (int)msgq_stat.msg_qnum) {
		if (++MQFullCnt > MAX_MSGQ_FULL_RETRY) {
			spdlog::error("[{}][{}]msg queue overflow. msg cur_cnt[{}] max_cnt[{}]",
			       __func__, __LINE__, msgq_stat.msg_qnum, statdm_max_queue_cnt);
			return  -QP_MSGQ_OVERFLOW;
		}

		memset(&msgq_stat, 0x00, sizeof(msgq_stat));
		if ((msgctl(sendQid, IPC_STAT, &msgq_stat)) < 0) {
			spdlog::error("[{}][{}]msgctl is failed. errno : {}",
			       __func__, __LINE__, errno);
			return  -QP_MSGQ_INFO_GET_FAIL;
		}
	}

	//snd_size = size + (sizeof(Message) - MAX_MESG_SIZE - sizeof(long));
	snd_size = size + 24;

	//while ( (msgsnd(sendQid, (struct msgBuf *)&mesg, (size_t)size+24, 0) < 0) &&

	while ( (msgsnd(sendQid, (struct msgBuf *)&mesg, snd_size, 0) < 0) &&
	        (++nCnt < MAX_MSGQ_RETRY)) {
		switch (errno) {
		case EAGAIN :
			break;

		case EIDRM :
		case EFAULT :
		case EACCES :
		case EINVAL :
		case EINTR :
		default :
			return  -QP_MESG_SEND_FAIL;
		}
	}
	if (nCnt == MAX_MSGQ_RETRY) { return FLK_FAIL; }

	return FLK_SUCCESS;
}



/*dbStatisticDBDmlResult*/
static int setResult(IDs *pQmiInfo, Result *result, int tag)
{
	Message mesg;
	//	int     nResult = FLK_SUCCESS;
	int		nCnt = 0;
	int 	snd_size = 0;

	while ( (( snd_size = msgrcv(pQmiInfo->srcQid, (struct msgbuf *)&mesg, sizeof(Message) - sizeof(long), (long)pQmiInfo->type, IPC_NOWAIT & MSG_NOERROR)) < 0) &&
	        (++nCnt < MAX_MSGQ_RETRY)) {
		switch (errno) {
		case ENOMSG :
			poll(NULL, '\0', 1);
			//sleep(1);
			break;

		case E2BIG :
		case EACCES :
		case EFAULT :
		case EIDRM :
		case EINTR :
		case EINVAL :
		default :
			return  -QP_MESG_RECV_FAIL;
		}
	}

	switch (mesg.EventName) {
	case CONNECT_ACK :
		(void)memcpy(result->tuple, mesg.Mesg, 3);
		return 1;
	case REQUEST_FAIL : {
		Request_Fail    *request_fail;

		request_fail = (Request_Fail *)mesg.Mesg;
		return -(request_fail->error_code);
	}
	case SELECT_ACK : {
		Select_Ack  *select_ack;

		select_ack = (Select_Ack *)mesg.Mesg;
		result->tup_id = select_ack->tup_id;
		(void)memcpy(result->tuple, select_ack->tuple, MAX_TUPLE_SIZE);
		return 1;
	}
	case UPDATE_ACK : {
		Update_Ack  *update_ack;

		Select_Ack  *echo_ack;
		if ((tag == UPDATE_EX) || (tag == UPDATE_EX2) || (tag == UPDATE_EO)) {
			echo_ack = (Select_Ack *)mesg.Mesg;
			result->tup_id = echo_ack->tup_id;
			(void)memcpy(result->tuple, echo_ack->tuple, MAX_TUPLE_SIZE);
		} else {
			update_ack = (Update_Ack *)mesg.Mesg;
			result->tup_id = update_ack->tup_id;
		}
		return 1;


	}
	case INSERT_ACK :
	case DELETE_ACK : {
		Delete_Ack  *delete_ack;

		delete_ack = (Delete_Ack *)mesg.Mesg;
		result->tup_id = delete_ack->tup_id;
		return 1;
	}
	case COUNT_ACK : {
		Count_Ack   *count_ack;

		count_ack = (Count_Ack *)mesg.Mesg;
		result->count = count_ack->count;
		return 1;
	}
	case QUIT_ACK :
		return 1;
	default :
		return -INVALID_EVENT_NAME;
	}
}
/** @} */
