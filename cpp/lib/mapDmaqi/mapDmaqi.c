/**
 @ingroup mapDmaqi
 @{
 */
/**********************************************************************
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
/**********************************************************************
	FILE NAME		:	mapDmaqi.c
	PURPOSE			:	to provide map process with db select and update
						function
	DATE OF WRITE	: 	1998.04.30
	AUTHOR			: 	Full Name (FirstName LastName)
	REVISION		:	V 1.0
	Ver.	By		When		Reason
	------- ------- ----------  -----------------------------------------
	1.0		molee	1998.04.30	first coding
***********************************************************************/
#include <stdio.h>
#include <libMapDmaqi.h>

#include <MdbTableInfo.h>
#include <MdbStatus.h>
#include <Debug.h>
#include <Map_Struct.h>
#include <MdbDmaqi.h>
#include <MapMacro.h>
#include <EInclude.h>
#include <ECommon.h>
#include <MdbCommon.h>
#include <MdbMessage.h>
#include <E4Noti.h>
#include <MdbTrigger.h>
#include <smscTime.h>
#include <ErrorHistory.h>
#include "libMapDmaqi.h"

int remakeRcsLink( u_int unMesgId, u_char ucStatus, stRDActiveTuple *spActive)
{
	int	nResult = FLK_SUCCESS;

	DLOG(("%s(unMesgId[%i: 0x%08x] ucStatus[0x%02X] spActive[%p])\n",
	      __func__, unMesgId, unMesgId, ucStatus, spActive));

	if (((ucStatus & 0xe0) == MDB_ST_FAIL_BASE) ||
	        (ucStatus == MDB_ST_SUCCESS) ||
	        (ucStatus == MDB_ST_SMSNOT_R) || (ucStatus == MDB_ST_RETRY)) {
		if ((spActive->dbUpdateFlag[0] && MDB_UPDATE ) == MDB_UPDATE) {
			spActive->dbUpdateFlag[0]   =   1;
			SetSubmittedStatus(DBMS_RCS, unMesgId);
			DLOG(("%s() Retrun Point Update Flag[0x%02X]\n",
			      __func__, spActive->dbUpdateFlag[0]));
			goto L_Return;
		}

		nResult = ManagementLinkDelete(DBMS_RCS, unMesgId, ucStatus);

		if ((ucStatus & MDB_ST_FAIL_BASE) == MDB_ST_FAIL_BASE || ucStatus == MDB_ST_SUCCESS) {
			spActive->dbStatus[0]   =   ucStatus;
			PutMsgId2Uhis( DBMS_RCS, unMesgId, CHK_2UHIS);
			DLOG(("%s() Put MsgId To Uhis(0x%08X) by 0x%02X Status\n",
			      __func__, unMesgId, spActive->dbStatus[0]));
		}

		if (nResult < 0) {
			spActive->dbStatus[0]  = ucStatus;
			DLOG(("Fail Return[%i] ManageMentLinkDelete() [%i : %s]\n",
			      nResult, __LINE__, __FILE__));
			nResult = -DMAQI_MIN_MASK_LINK_DELETE_FAIL;
			goto L_Return;
		}

	}// end of if(ucStatus)

L_Return:
	DLOG(("%s() Retrun [%i : 0x%08X]\n", __func__, nResult, nResult));
	return nResult;
}
/*---------------------------------------------------------------------
 *  FUNCTION NAME   : remakeLink()
 *  PURPOSE         : to update message destination address.
 *  INPUT ARGUMENTS : message id / destination address(hlr)
 *  RETURN VALUE    : success/fail
 *  CALLED FROM     :
 *  USED GLOBAL VARIABLES :
 *  REVISION        :
 *      BY              WHEN        REASON
 *      --------------  ----------  -------------------
 *      molee           98.05       first coding.
 *-----------------------------------------------------------------------*/
int remakeLink(int nDbmsHandle, u_int unMesgId, u_char ucStatus, stRDActiveTuple *spActive)
{
	int	nResult = FLK_SUCCESS;

	DLOG(("%s(unMesgId[%i: 0x%08x] ucStatus[0x%02X] spActive[%p])\n",
	      __func__, unMesgId, unMesgId, ucStatus, spActive));

	if (((ucStatus & 0xe0) == MDB_ST_FAIL_BASE) ||
	        (ucStatus == MDB_ST_SUCCESS) ||
	        (ucStatus == MDB_ST_SMSNOT_R) || (ucStatus == MDB_ST_RETRY)) {
		if ((spActive->dbUpdateFlag[0] && MDB_UPDATE ) == MDB_UPDATE) {
			spActive->dbUpdateFlag[0]   =   1;
			SetSubmittedStatus(nDbmsHandle, unMesgId);
			DLOG(("%s() Retrun Point Update Flag[0x%02X]\n",
			      __func__, spActive->dbUpdateFlag[0]));
			goto L_Return;
		}

		nResult = ManagementLinkDelete(nDbmsHandle, unMesgId, ucStatus);

		if ((ucStatus & MDB_ST_FAIL_BASE) == MDB_ST_FAIL_BASE ||
		        ucStatus == MDB_ST_SUCCESS) {
			spActive->dbStatus[0]   =   ucStatus;
			PutMsgId2Uhis(nDbmsHandle, unMesgId, CHK_2UHIS);
			DLOG(("%s() Put MsgId To Uhis(0x%08X) by 0x%02X Status\n",
			      __func__, unMesgId, spActive->dbStatus[0]));
		}

		if (nResult < 0) {
			spActive->dbStatus[0]  = ucStatus;
			DLOG(("Fail Return[%i] ManageMentLinkDelete() [%i : %s]\n",
			      nResult, __LINE__, __FILE__));
			nResult = -DMAQI_MIN_MASK_LINK_DELETE_FAIL;
			goto L_Return;
		}
	}// end of if(ucStatus)

L_Return:
	DLOG(("%s() Retrun [%i : 0x%08X]\n", __func__, nResult, nResult));
	return nResult;
}


int updateStatus(int nDbmsHandle, u_int unMesgId, u_char ucStatus, u_short usFailReason)
{
	int nResult = FLK_SUCCESS;
	int nPreProFlag = false;
	char cRedirectUsable;
	int nTempVal = 0;

	DLOG(("%s() -- unMsgId[%i] ucStatus[0x%02X] usFailReason[0x%04X]\n",
	      __func__, unMesgId, ucStatus, usFailReason));

	stRDActiveTuple *spActive = NULL;
	if (0 > (nResult = dma_ActiveTuplePtr(nDbmsHandle, unMesgId, &spActive))) {
		DLOG((" dma_getActiveTuplePtr error in %s().\n", __func__));
		db_dma_error(nResult);
		goto L_Retrun;
	}

	if (((spActive->dbStatus[0]	& MDB_ST_FAIL_BASE) == MDB_ST_FAIL_BASE) ||
	        (spActive->dbStatus[0] == MDB_ST_SUCCESS)) {
		DLOG(("%s() Put MsgId2 Uhis unMsgId[%i] [%i in %s]\n",
		      __func__, unMesgId, __LINE__, __FILE__));
		PutMsgId2Uhis(nDbmsHandle, unMesgId, CHK_2UHIS);

		nResult = FLK_SUCCESS;
		goto L_Retrun;
	}

	if ( (nDbmsHandle == DBMS_ACTIVE) && (spActive->dbStatus[0] == MDB_ST_SKT_ATI_R) && usFailReason == ERR_SKT_ATSI_REQ_SVC ) {
		OSFI_CAPA_DISABLE(nTempVal, spActive->dbOSFIFlag, SKT_OSFI_010_PLUS);
		spActive->dbATSIREQStatus[0] = SUB_ST_DEL_RESP_ERROR;
		memset(spActive->dbOSFIFlag, 0x00, DB_OSFI_SIZE);
		memset(spActive->dbOSFI, 0x00, DB_OSFI_SIZE);
	}

	/* SKT SVC Preprocessing by twkim 07.01.08
	 * MDB_ST_RETRY 이더라도 ADR or CauseCode 받고
	 * retry 일때만 서비스 선행처리를 하기 위해 */
	if ((spActive->dbStatus[0] == MDB_ST_SMSREQ_R) ||
	        (spActive->dbStatus[0] == MDB_ST_SMDPP_R) ||
	        /* GSM SVC Preprocessing case
	         * 3G는 MTF 후에만 즉시전송 처리 함 */
	        (spActive->dbStatus[0] == MDB_ST_RIFSM_R) ||
	        //FR220 010_PLUS_SVC does not preprocessing
	        (spActive->dbStatus[0] == MDB_ST_RSMDS_G_R) ||
	        (spActive->dbStatus[0] == MDB_ST_RSMDS_F_M_R) ||
	        (spActive->dbStatus[0] == MDB_ST_RSMDS_M_R) ||
	        (spActive->dbStatus[0] == MDB_ST_RSMDS_F_G_R) ||
	        (spActive->dbStatus[0] == MDB_ST_RSMDS_F_F_R) ||
	        (spActive->dbStatus[0] == MDB_ST_RSMDS_MME_SUCC_R) ||
	        (spActive->dbStatus[0] == MDB_ST_RSMDS_MME_FAIL_R) ||
	        //(spActive->dbStatus[0] == MDB_ST_RSMDS_SIP_SUCC_R) ||
	        //(spActive->dbStatus[0] == MDB_ST_RSMDS_SIP_FAIL_R) ||
	        (spActive->dbStatus[0] == MDB_ST_RSMDS_F_R) ||
	        //(spActive->dbStatus[0] == MDB_ST_SIP_DELIVER_R) ||
	        (spActive->dbStatus[0] == MDB_ST_FSM_G_M_R) ||
	        (spActive->dbStatus[0] == MDB_ST_FSM_F_M_R) ||
	        (spActive->dbStatus[0] == MDB_ST_FSM_M_G_R) ||
	        (spActive->dbStatus[0] == MDB_ST_FSM_F_G_R) ||
	        (spActive->dbStatus[0] == MDB_ST_FSM_M_R) ||
	        (spActive->dbStatus[0] == MDB_ST_FSM_G_R) ||
	        (is_dia_sgd_mme_recv_status((int)spActive->dbStatus[0]) == true) ||
	        (is_dia_sgd_smsf_recv_status((int)spActive->dbStatus[0]) == true) ||
	        (is_dia_sgd_recv_status((int)spActive->dbStatus[0]) == true)  ||
	        (spActive->dbStatus[0] == MDB_ST_RSMDS_SMSF_SUCC_R) ||
	        (spActive->dbStatus[0] == MDB_ST_RSMDS_SMSF_FAIL_R) ) {
		nPreProFlag = true;
	}
	/* SKT SVC Preprocessing End */

	if (ucStatus == MDB_ST_NOTI_SEND ) {
		if ((spActive->dbStatus[0] == MDB_ST_RETRY_SLEEP) ||
		        (spActive->dbStatus[0] == MDB_ST_SUBMITTED))
			//memcpy(spActive->dbStatus,&ucStatus, DB_STATUS_SIZE);
		{
			spActive->dbStatus[0] = ucStatus;
		}
	} else {
		spActive->dbStatus[0] = ucStatus;
	}

	if (usFailReason != 0) {
		memcpy(spActive->dbFailReason, &usFailReason, DB_FAIL_REASON_SIZE);
		setErrorHistory(spActive, usFailReason);
	}

	if (ucStatus == MDB_ST_SMDPP_S) {
		time2decchar7((char *)spActive->dbDeliveryTime, time(NULL));
	}

	/* SKT SVC Preprocessing by twkim 07.01.08
	 * ADR or CouseCode 등으로 MDB_ST_RETRY로 update 할때 서비스 선행 처리해야
	 * 하는 메시지인지  확인 하는 루틴 TOP Msg Pending시에만 해당
	 * 파라미터가 MDB_ST_RETRY && 1번째 시도이고 Main status 선행처리가 아닐때
	 * nRetryCnt가 2일때도 선처리하는 것은 넘버플러스 가입자가 최초 Pending일때
	 * 선처리 하기 위해서 추가 했음
	 * 3G의 경우 PICS, MGR 즉시전송 상태 확인하도록 추가 */
	u_short nRetryCnt;
	int nCAPA_Copy_Target;
	int nCapaFlag, nPicsCapaFlag, nMgrCapaFlag, nRcsCapaFlag;

	memcpy(&nRetryCnt, &spActive->dbRetryCount[0], DB_RETRY_COUNT_SIZE);
	memcpy(&cRedirectUsable, spActive->dbRedirectUsable, DB_REDIRECT_USE_SIZE);

	OSFI_CAPA_IN_OUT_CHECK(nCAPA_Copy_Target, spActive->dbDestMsCapability,
	                       SKT_CAPA_PICS, nPicsCapaFlag);

	OSFI_CAPA_IN_OUT_CHECK(nCAPA_Copy_Target, spActive->dbDestMsCapability,
	                       SKT_CAPA_MGR, nMgrCapaFlag);

	OSFI_CAPA_IN_OUT_CHECK(nCAPA_Copy_Target, spActive->dbDestMsCapability,
	                       SKT_CAPA_RCS, nRcsCapaFlag);

	if ((ucStatus == MDB_ST_RETRY)  && (nPreProFlag)
	        && ( (nRetryCnt == 1)
	             || ((nRetryCnt == 2) && (cRedirectUsable == SKT_FORWARD_SVC_ON))
	             || ((nPicsCapaFlag) && (spActive->dbPicsImmetTransStatus[0] == SUB_ST_BASE))
	             || ((nMgrCapaFlag) && (spActive->dbMgrImmetTransStatus[0] == SUB_ST_BASE))
	             || ((nRcsCapaFlag) && (spActive->dbRcsSVCStatus[0] == SUB_ST_BASE)) )) {
		OSFI_CAPA_PARAM_IS_CHECK(spActive->dbDestMsCapability,
		                         nCAPA_Copy_Target, nCapaFlag, SKT_PREPROCESSING_CAPA_MASK);

		//선행 처리해야 하는 서비스가 있다면 status를
		//MDB_ST_SKT_SVC_PREPROCESSING_TOP로 바꾸고 send schd로 넘김
		if (nCapaFlag) {
			spActive->dbStatus[0] = MDB_ST_SKT_SVC_PREPROCESSING_TOP;
			PutMsgId2SchdSndMsg(nDbmsHandle, unMesgId, 0);
			nResult = FLK_SUCCESS;
			goto L_Retrun;
		}
	}
	/* SKT SVC Preprocessing End */

	//R910 PKG : RCS SUBMIT_SM FAIL -> SMS DONE -> RCS RETRY
	//if( (((ucStatus & MDB_ST_FAIL_BASE) == MDB_ST_FAIL_BASE) || (ucStatus == MDB_ST_SUCCESS)) &&
	if( (ucStatus == MDB_ST_SUCCESS) &&
		(spActive->dbMcsMtType[0] == MCS_MT_NORMAL_IDX) &&
		((spActive->dbMcsMtStatus[0] == SUB_ST_DEL_REQ_ERROR) ||
		 (spActive->dbMcsMtStatus[0] == SUB_ST_DEL_RESP_TIMEOUT) ||
		 (spActive->dbMcsMtStatus[0] == SUB_ST_DEL_RESP_ERROR) ||
		 (spActive->dbMcsMtStatus[0] == SUB_ST_DEL_CONNECT_FAIL) ) )
	{
		spActive->dbMcsMtSmsType[0] = MCS_MT_SMS_DONE_IDX;
#if 0
		u_short usFailReason = ERR_ESME_MMCS_INTERNAL;
		memcpy(spActive->dbFailReason, &usFailReason, DB_FAIL_REASON_SIZE);
		spActive->dbStatus[0] = MDB_ST_RETRY_SLEEP;
		spActive->dbMcsMtSmsType[0] = MCS_MT_SMS_DONE_IDX;
		PutMsgId2SchdSRetry(DBMS_ACTIVE, unMesgId, MDB_ST_RETRY);
		nResult = FLK_SUCCESS;
		goto L_Retrun;
#endif
	}

	nResult	= remakeLink(nDbmsHandle, unMesgId, ucStatus, spActive);

	DLOG(("%s() Return [%i] remakeLink(%u, 0x%02X, 0x%p) [%i in %s]\n",
	      __func__, nResult, unMesgId, ucStatus, spActive, __LINE__, __FILE__));

L_Retrun:
	DLOG(("Retrun[%i] %s()\n", nResult, __func__));
	return nResult;
}



static int remakeLinkCont(int nDbmsHandle, u_int unMesgId, u_char ucStatus, stRDActiveTuple *spActive)
{
	int nResult = 0;

	if (((ucStatus & 0xe0) == MDB_ST_FAIL_BASE) || (ucStatus == MDB_ST_SUCCESS) ||
	        (ucStatus == MDB_ST_SMSNOT_R) || (ucStatus == MDB_ST_RETRY) ) {
		if ((spActive->dbUpdateFlag[0] && MDB_UPDATE ) == MDB_UPDATE) {
			spActive->dbUpdateFlag[0]   =   1;
			SetSubmittedStatus(nDbmsHandle, unMesgId);
			return FLK_SUCCESS;
		}
		nResult = ManagementLinkDeleteCont(nDbmsHandle, unMesgId, ucStatus);
		if ( ((ucStatus & 0xe0) == 0xe0) || (ucStatus == MDB_ST_SUCCESS) ) {
			spActive->dbStatus[0]   =   ucStatus;
			PutMsgId2Uhis(nDbmsHandle, unMesgId, CHK_2UHIS);
		}
		if (nResult < 0) {
			spActive->dbStatus[0]  = ucStatus;
			return -DMAQI_MIN_MASK_LINK_DELETE_FAIL;
		}
	}
	return FLK_SUCCESS;
}


int updateStatusCont(int nDbmsHandle, u_int unMesgId, u_char ucStatus, u_short usFailReason)
{
//    Update          update;
//    Select_Ack      ack;

//    u_char       ucMesgId[4];
//    u_short      usRetryCount=0;
//    stRDActiveTuple     active;
	stRDActiveTuple     *spActive;
	time_t              tDeliveryTime;
	int                 nResult = 0;
//    int                 nUpdCnt=0;
//    int                 nTupId=0;
//    int                 nPerResult=0;


	nResult =   dma_ActiveTuplePtr(nDbmsHandle, unMesgId, &spActive);
	if (nResult < 0) {
		db_dma_error(nResult);
		return nResult;
	}

	if (((spActive->dbStatus[0]  & MDB_ST_FAIL_BASE) == MDB_ST_FAIL_BASE) ||
	        (spActive->dbStatus[0] == MDB_ST_SUCCESS)) {
		PutMsgId2Uhis(nDbmsHandle, unMesgId, CHK_2UHIS);
		return FLK_SUCCESS;
	}

	if ((ucStatus == MDB_ST_NOTI_SEND )) {
		if ((spActive->dbStatus[0] == MDB_ST_RETRY_SLEEP) ||
		        (spActive->dbStatus[0] == MDB_ST_SUBMITTED)) {
			//memcpy(spActive->dbStatus,&ucStatus,sizeof(active.dbStatus));
			spActive->dbStatus[0] = ucStatus;
		}
	} else
		//memcpy(spActive->dbStatus,&ucStatus,sizeof(active.dbStatus));
	{
		spActive->dbStatus[0] = ucStatus;
	}

	if (usFailReason != 0) {
		memcpy(spActive->dbFailReason, &usFailReason, sizeof(u_short));
		setErrorHistory(spActive, usFailReason);
	}
	if ( (ucStatus == MDB_ST_SMDPP_S))

	{
		time(&tDeliveryTime);
		time2decchar7((char *)spActive->dbDeliveryTime, tDeliveryTime);
	}

	nResult =   remakeLinkCont(nDbmsHandle, unMesgId, ucStatus, spActive);
	return nResult;

}
int updateRcsStatus( u_int unMesgId, u_char ucStatus, u_short usFailReason)
{
	int nResult = FLK_SUCCESS;
//	int nPreProFlag = false;
//	char cRedirectUsable;

	DLOG(("%s() -- unMsgId[%i] ucStatus[0x%02X] usFailReason[0x%04X]\n",
	      __func__, unMesgId, ucStatus, usFailReason));

	stRDActiveTuple *spActive = NULL;
	if (0 > (nResult = dma_getRcsTuplePtr( unMesgId, &spActive))) {
		DLOG((" dma_getActiveTuplePtr error in %s().\n", __func__));
		db_dma_error(nResult);
		goto L_Retrun;
	}

	if (((spActive->dbStatus[0]	& MDB_ST_FAIL_BASE) == MDB_ST_FAIL_BASE) ||
	        (spActive->dbStatus[0] == MDB_ST_SUCCESS)) {
		DLOG(("%s() Put MsgId2 Uhis unMsgId[%i] [%i in %s]\n",
		      __func__, unMesgId, __LINE__, __FILE__));
		if ( PutMsgId2Uhis( DBMS_RCS, unMesgId, CHK_2UHIS) < 0 ) {
		}

		nResult = FLK_SUCCESS;
		goto L_Retrun;
	}

	spActive->dbStatus[0] = ucStatus;

	if (usFailReason != 0) {
		memcpy(spActive->dbFailReason, &usFailReason, DB_FAIL_REASON_SIZE);
		setErrorHistory(spActive, usFailReason);
	}

	nResult	= remakeLink(DBMS_RCS, unMesgId, ucStatus, spActive);
	DLOG(("%s() Return [%i] remakeLink(%u, 0x%02X, 0x%p) [%i in %s]\n",
	      __func__, nResult, unMesgId, ucStatus, spActive, __LINE__, __FILE__));

L_Retrun:
	DLOG(("Retrun[%i] %s()\n", nResult, __func__));
	return nResult;
}

/** @} */
