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
#******************************************************************************/
/** 
 @ingroup map_flow 
 @{
 */

/*******************************************************************************
    FILE NAME       :  	V3_RoutingInfoForSMR.c
    PURPOSE         :  
    DATE OF WRITE   :   2001.12.01
    AUTHOR          :   Suk-Hun Jang
    REVISION        :   V 1.0
    Ver.    By      When        Reason
    ------- ------- ----------  -----------------------------------------
    1.0     shjang  2001.12.01  first coding
*******************************************************************************/
#include "libMapCom.h"
#include "Map.h"
#include "MdbCommon.h"
#include "Debug.h"

#include "Smpp_SVC_Macro.h"

/* for ACQ */
#include "SKT_SvcInfo.h"
#include "EMinMask.h"

#include "StatRecord.h"
#include "StatRecord.h"
#include "MdbDmaqi.h"
#include "smscString.h"
#include "libEsme.h"
#include "libSmppComm.h"
extern stStatisticsRecord  stStsRecord;
extern u_int ProcessID;


/* extern param */
u_char   cpTempMin[DB_DEST_MIN_SIZE];

int
V3_fillRoutingInfoForSMR(V3_RoutingInfoForSM_Arg *pD, stMsgBuf_t *spMsg)
{
	int			nIdx = 0;
	int			nTemp;
    int         nCnt;
    int         nLen = 0;
	u_char		ucDestLen = 0;

	memset(cpTempMin, 0xff, DB_DEST_MIN_SIZE);

	pD->m.extensionContainerPresent		= 0;
	pD->m.gprsSupportIndicatorPresent	= 1;
	pD->m.sm_RP_MTIPresent				= 0;
	pD->m.sm_RP_SMEAPresent				= 0;

	pD->msisdn.m.NAI 	= spActive->dbDestTON[0];
  	pD->msisdn.m.NP   	= spActive->dbDestNPI[0];

	//
	//	Check Redirect Number 2007.05.22 by bjlee
	//	Forward Service & 112 Special Number Service
	//
	if(spActive->dbRedirectUsable[0] > SKT_FORWARD_SVC_OFF)
	{
		if(DestNCountryEqual(spActive->dbDestRedirectMIN, 
					stMapParam.ucCountryCode, 
					stMapParam.ucCountryCodeLen) != true)
		{
		    if(cvtBcdNational2International(stMapParam.ucCountryCode, 
						stMapParam.ucCountryCodeLen, 
						spActive->dbDestRedirectMIN, spActive->dbDestRedirectLen[0], 
						cpTempMin, &ucDestLen) == FLK_FAIL)
			{
				return FLK_FAIL;
			}
		    pD->msisdn.m.NAI    = INTERNATIONAL_NUMBER;
			pD->msisdn.m.NP     = ISDN_TELEPHONY_NUMBERING_PLAN;
			pD->msisdn.numocts = ucDestLen;
			memcpy(pD->msisdn.data, cpTempMin, pD->msisdn.numocts);
		}
	}
	else
	{
		if(DestNCountryEqual(spActive->dbDestMIN, 
					stMapParam.ucCountryCode, 
					stMapParam.ucCountryCodeLen) != true)
		{
		    if(cvtBcdNational2International(stMapParam.ucCountryCode, 
						stMapParam.ucCountryCodeLen, 
						spActive->dbDestMIN, spActive->dbDestLen[0], 
						cpTempMin, &ucDestLen) == FLK_FAIL)
			{
				return FLK_FAIL;
			}
		    pD->msisdn.m.NAI    = INTERNATIONAL_NUMBER;
			pD->msisdn.m.NP     = ISDN_TELEPHONY_NUMBERING_PLAN;
			pD->msisdn.numocts = ucDestLen;
			memcpy(pD->msisdn.data, cpTempMin, pD->msisdn.numocts);
		}
	}

	/* set priority by db */
	if(spActive->dbsmRPPri[0] != 0x00) 
	{
		pD->sm_RP_PRI	= 0xff;
	}
	else								 	
	{
		pD->sm_RP_PRI  	= 0x00;
	}

	pD->serviceCentreAddress.m.NAI 		= INTERNATIONAL_NUMBER;
    pD->serviceCentreAddress.m.NP  		= ISDN_TELEPHONY_NUMBERING_PLAN;
    pD->serviceCentreAddress.numocts	= stMapParam.nIsdnLen;
    memcpy(&pD->serviceCentreAddress.data, stMapParam.cSmscIsdnHex
		, pD->serviceCentreAddress.numocts);
	
	if(encV3_RoutingInfoForSM_Arg(spMsg, pD, 0x30) < 0) 
	{
		return(-1);
	}		
	return(1);
}

int
V3_BegInvRIFSMRGT(stMsgBuf_t *spMsg, V3_RoutingInfoForSM_Arg *pD)
{
	int             nRet;
    int             nIdx1;
    int             nIdx2;
    int             nIdx3 = 0;
    tcx_primitive   stPrimitive;
    tcx_component   *compPtr;
    tcx_buffer      *buffPtr;
	u_char	cTemp;

	if(TCX_alloc_component(&compPtr, 1) < 0)
    {
        return(-1);
    }
    if(TCX_alloc_buffer(&buffPtr, spMsg->nLen+1) < 0)
    {
        return(-1);
    }
	if(encodeComp(TC_INVOKE, compPtr, buffPtr, SEND_ROUTING_INFO_FOR_SM, 
				stMapAddr.unMsgId, 0, encIid(), 
				(int)stMapParam.usRoutingInfoTime, spMsg) < 0)
	{
		return(-1);
	}

	if(encodeTransGt(TC_BEGIN, &stPrimitive, 
				&pV3AppContext[V3_SHORTMSG_GATEWAY_CONTEXT_V3], 
				stMapAddr.unMsgId, 0, cpTempMin, 6) < 0)
	{
        return(-1);
	}

    if(GetDebugLevel() >= 2)
	{
        printBegInvReq(&stPrimitive, compPtr, buffPtr);
		v3_debugRIFSMR(pD);
	}

    if(GetTraceLevel() > 0) 
	{
		if(!CheckTraceMin(spActive->dbDestMIN, DB_DEST_MIN_SIZE))
		{
			trace(&stPrimitive, compPtr, buffPtr, "GSM 29-002 (SMSC -> HLR)");
			v3_traceRIFSMR(pD);
			traceEnd();
		}

		if(!CheckTraceMin(spActive->dbSourceMIN, DB_SOURCE_MIN_SIZE))
		{
			trace(&stPrimitive, compPtr, buffPtr, "GSM 29-002 (SMSC -> HLR)");
			v3_traceRIFSMR(pD);
			traceEnd();
		}
	}
    return(1);
}

int
V3_BegInvRIFSMRPC(int nPc, u_char cSsn, stMsgBuf_t *spMsg, V3_RoutingInfoForSM_Arg *pD)
{
	tcx_primitive   stPrimitive;
    tcx_component   *compPtr;
    tcx_buffer      *buffPtr;
	Oid             stOid;

	if(TCX_alloc_component(&compPtr, 1) < 0)
    {
        return(-1);
    }
    if(TCX_alloc_buffer(&buffPtr, spMsg->nLen+1) < 0)
    {
        return(-1);
    }

	if(encodeComp(TC_INVOKE, compPtr, buffPtr, SEND_ROUTING_INFO_FOR_SM, 
				stMapAddr.unMsgId, 0, encIid(), 
				(int)stMapParam.usRoutingInfoTime, spMsg) < 0)
	{
		return(-1);
	}

    if(encodeTransPcDig(TC_BEGIN, &stPrimitive,
				&pV3AppContext[V3_SHORTMSG_GATEWAY_CONTEXT_V3],
				stMapAddr.unMsgId, 0, nPc, cSsn) < 0)
	{
		return(-1);
	} 

    if(GetDebugLevel() >= 2)
	{
        printBegInvReq(&stPrimitive, compPtr, buffPtr);
		v3_debugRIFSMR(pD);
	}

    if(GetTraceLevel() > 0) 
	{
		if(!CheckTraceMin(spActive->dbDestMIN, DB_DEST_MIN_SIZE))
		{
			trace(&stPrimitive, compPtr, buffPtr, "GSM 29-002 (SMSC -> HLR)");
			v3_traceRIFSMR(pD);
			traceEnd();
		}
		if(!CheckTraceMin(spActive->dbSourceMIN, DB_SOURCE_MIN_SIZE))
		{
			trace(&stPrimitive, compPtr, buffPtr, "GSM 29-002 (SMSC -> HLR)");
			v3_traceRIFSMR(pD);
			traceEnd();
		}
	}
	return(1);
}

int
V3_RoutingInfoForSMR(u_int unMsgId, stMsg_t *spMsg)
{
	V3_RoutingInfoForSM_Arg	D;
	int						nPc;
	stMsgBuf_t				stMsg;
	int						nRet;
	u_char						ucStatus;
	u_short          			usErrReason;
	u_char						ucAddType;
	u_char						cpBuf[512];
	MinMaskDBType1			*spMinMask1;
	
	bzero(&D, sizeof(V3_RoutingInfoForSM_Arg));
	memset(&stMapAddr, 0x00, sizeof(stMapAddress_t));

	/* open Active DB */
	nRet = dma_getActiveTuplePtr(unMsgId, &spActive);	
	if(nRet < 0)
	{
        MakeStatisticsRecord( &stStsRecord,
                STATISTICS_RECORD_END,
                STATISTICS_TRYINF_NET,
                STATISTICS_TRYNRL_GSM,
                STATISTICS_TRYDIR_OUT,
                STATISTICS_TRYEVNT_SMQUERY,
                0,
                0,
                ERR_MEMDB_ERROR,
                0,
                ProcessID,
                0,
                0,
				NULL);
        dbStatisticInsertbyRecord(&stStsRecord);
		db_dma_error(nRet); return(-1);
	}

    u_short isRetryCnt = 0;

	if(getMinAddress(&stMapAddr, spActive) < 0)
	{
		ucStatus 	= MDB_ST_RETRY;
		usErrReason = ERR_MEMDB_ERROR;
		updateStatus(DBMS_ACTIVE, unMsgId, ucStatus, usErrReason);
		DebugStr(DG_LEVEL_FIVE,"[%s : %d ]\n", __FILE__, __LINE__);
        MakeStatisticsRecord( &stStsRecord,
                STATISTICS_RECORD_END,
                STATISTICS_TRYINF_NET,
                STATISTICS_TRYNRL_GSM,
                STATISTICS_TRYDIR_OUT,
                STATISTICS_TRYEVNT_SMQUERY,
                isRetryCnt,
                0,
                usErrReason,
                0,
                ProcessID,
                0,
                0,
				NULL);
        dbStatisticInsertbyRecord(&stStsRecord);
		return(-1);
	}

    /* Setting Map Version */
    setMapVersion(spActive->dbNetworkVer, GSM_MAP_V3);


	/* Check Max Message Length */
/* 2006.01.13  modify by hongkyu
    if((stMapParam.nMaxMsgLen > MAX_MESSAGE_LEN)
        || (stMapParam.nMaxMsgLen < spActive->dbMessageLength[0]))
*/
    if(stMapParam.nMaxMsgLen < spActive->dbMessageLength[0])
    {
		ucStatus    = MDB_ST_FAIL_RETRY_FINISH;
        usErrReason = ERR_PARAM_ERROR;
        updateStatus(DBMS_ACTIVE, unMsgId, ucStatus, usErrReason);

		DebugStr(DG_LEVEL_FIVE,"[%s : %d ]\n", __FILE__, __LINE__);
        MakeStatisticsRecord( &stStsRecord,
                STATISTICS_RECORD_END,
                STATISTICS_TRYINF_NET,
                STATISTICS_TRYNRL_GSM,
                STATISTICS_TRYDIR_OUT,
                STATISTICS_TRYEVNT_SMQUERY,
                isRetryCnt,
                0,
                usErrReason,
                0,
                ProcessID,
                0,
                0,
				NULL);
        dbStatisticInsertbyRecord(&stStsRecord);
        return(-1);
    }

#ifdef	DEBUG
	LogStr(stMapAddr.cpMin, stMapAddr.nMinLen, 10);
#endif
	
	stMapAddr.unMsgId = unMsgId;

	//
	//	Check MNP Service flow
	//
	if( ((spActive->dbEsmClass[0] & ESME_CLASS_PORTED) == ESME_CLASS_PORTED ) ||
		(ESME_CLASS_FORWARD_AND_PORTED == spActive->dbEsmClass[0]) ||
		(ESME_CLASS_PORTED_AND_NOTI_FROM_ESME == spActive->dbEsmClass[0]) ||
		(MNP_HLR_PORTED_ACK == spActive->dbPorted[0]) ||
		(0 < spActive->dbDestRouteLen[0]) )
	{
		memcpy(stMapAddr.cpDestRouteMin, spActive->dbDestRouteMIN, DB_DESTROUTE_MIN_SIZE);

		nPc = getPcFromRNPrefix(&stMapAddr);
		if(nPc < 0)
		{
			ucStatus    = MDB_ST_RETRY;
			usErrReason = ERR_RNPREFIX_ERROR;
			updateStatus(DBMS_ACTIVE,unMsgId, ucStatus, usErrReason);
			DebugStr(DG_LEVEL_FIVE,"[%s : %d ]\n", __FILE__, __LINE__);
	        MakeStatisticsRecord( &stStsRecord,
                STATISTICS_RECORD_END,
                STATISTICS_TRYINF_NET,
                STATISTICS_TRYNRL_GSM,
                STATISTICS_TRYDIR_OUT,
                STATISTICS_TRYEVNT_SMQUERY,
                isRetryCnt,
                0,
                usErrReason,
                0,
                ProcessID,
                0,
                nPc,
				NULL);
        	dbStatisticInsertbyRecord(&stStsRecord);
			return(-1);

		}
	}
	else
	{
		nPc = getPcFromPrefix(&stMapAddr);
    	if(nPc < 0)
    	{
			if( (spActive->dbDestRedirectLen[0] > 0) ||
				(FLK_SUCCESS == CheckDBMService(spActive->dbTeleserviceID)) ||
				(spActive->dbDRtoMSStatus[0] == (SUB_ST_DEL_RESP_SUCCESS | SUB_ST_SVC_NOQUERY)) )
			{
				memcpy(stMapAddr.cpDestRouteMin, spActive->dbDestRouteMIN, DB_DESTROUTE_MIN_SIZE);
				nPc = getPcFromRNPrefix(&stMapAddr);

				if(nPc < 0)
				{
					ucStatus    = MDB_ST_RETRY;
					usErrReason = ERR_RNPREFIX_ERROR;
					updateStatus(DBMS_ACTIVE,unMsgId, ucStatus, usErrReason);
					DebugStr(DG_LEVEL_FIVE,"[%s : %d ]\n", __FILE__, __LINE__);
	        		MakeStatisticsRecord( &stStsRecord,
                		STATISTICS_RECORD_END,
						STATISTICS_TRYINF_NET,
						STATISTICS_TRYNRL_GSM,
						STATISTICS_TRYDIR_OUT,
						STATISTICS_TRYEVNT_SMQUERY,
                		isRetryCnt,
                		0,
                		usErrReason,
                		0,
                		ProcessID,
                		0,
                		nPc,
						NULL);
        			dbStatisticInsertbyRecord(&stStsRecord);
					return(-1);
				}
			}
			else
			{

				ucStatus 	= MDB_ST_RETRY;
				usErrReason = ERR_PREFIX_ERROR;
				updateStatus(DBMS_ACTIVE, unMsgId, ucStatus, usErrReason);
				DebugStr(DG_LEVEL_FIVE,"[%s : %d ]\n", __FILE__, __LINE__);
        		MakeStatisticsRecord( &stStsRecord,
                	STATISTICS_RECORD_END,
                	STATISTICS_TRYINF_NET,
                	STATISTICS_TRYNRL_GSM,
                	STATISTICS_TRYDIR_OUT,
                	STATISTICS_TRYEVNT_SMQUERY,
                	isRetryCnt,
                	0,
                	usErrReason,
                	0,
                	ProcessID,
                	0,
                	nPc,
					NULL);
        		dbStatisticInsertbyRecord(&stStsRecord);
				return(-1);
    		}
		}
		// add for Number Plus SVC 2007.07.13
		// add for ACQ Service 2007.08.29
		else if( ( (stMapAddr.ucAddrType & 0xf0) == ESME_TYPE) &&
				 ( (spActive->dbDestRedirectLen[0] > 0) ||
				   (FLK_SUCCESS == CheckDBMService(spActive->dbTeleserviceID)) ||
				   (spActive->dbDRtoMSStatus[0] == (SUB_ST_DEL_RESP_SUCCESS | SUB_ST_SVC_NOQUERY)) ||
				   (spActive->dbNodeType[0] == ACQ_TYPE_SKT_2G) ||
				   (spActive->dbNodeType[0] == ACQ_TYPE_SKT_3G) ) )
		{
			nPc = getPcFromRNPrefix(&stMapAddr);
			if(nPc < 0)
			{

				ucStatus    = MDB_ST_RETRY;
                usErrReason = ERR_RNPREFIX_ERROR;
                updateStatus(DBMS_ACTIVE,unMsgId, ucStatus, usErrReason);

                MakeStatisticsRecord( &stStsRecord,
                    STATISTICS_RECORD_END,//before ack STATISTICS_RECORD_START,
                    STATISTICS_TRYINF_NET,
                    STATISTICS_TRYNRL_GSM,
                    STATISTICS_TRYDIR_OUT,
                    STATISTICS_TRYEVNT_SMQUERY,
                    0, //(u_short)spActive->dbMessageLength[0],
                    0,//(time(NULL) - RoutingSendTime)+1,
                    usErrReason,
                    0,
                    ProcessID,
                    0, //(u_short)spActive->dbProtocolID[0],
                    NULL,
                    NULL);
                dbStatisticInsertbyRecord(&stStsRecord);
	            return(-1);
			}
		}
	}

	//add by twkim DR114 patch 071031
	ucAddType = stMapAddr.ucAddrType & 0xf0;
	if(ucAddType == ESME_TYPE)
	{
		if(sendToSsmdp(&stMapAddr) < 0)
		{
			ucStatus    = MDB_ST_RETRY;
			usErrReason = ESME_SEND_ERROR;
			updateStatus(DBMS_ACTIVE, unMsgId, ucStatus, usErrReason);
			MakeStatisticsRecord( &stStsRecord,
					STATISTICS_RECORD_END,//before ack STATISTICS_RECORD_START,
					STATISTICS_TRYINF_NET,
					STATISTICS_TRYNRL_GSM,
					STATISTICS_TRYDIR_OUT,
					STATISTICS_TRYEVNT_SMQUERY,
					0, //(u_short)spActive->dbMessageLength[0],
					0,//(time(NULL) - RoutingSendTime)+1,
					usErrReason,
					0,
					ProcessID,
					0, //(u_short)spActive->dbProtocolID[0],
					nPc,
					NULL);
			dbStatisticInsertbyRecord(&stStsRecord);
		}
		else
		{
			ucStatus    = MDB_ST_ESME_SEND;
			usErrReason = NULL;
			updateStatus(DBMS_ACTIVE, unMsgId, ucStatus, usErrReason);
			MakeStatisticsRecord( &stStsRecord,
					STATISTICS_RECORD_END,//before ack STATISTICS_RECORD_START,
					STATISTICS_TRYINF_NET,
					STATISTICS_TRYNRL_GSM,
					STATISTICS_TRYDIR_OUT,
					STATISTICS_TRYEVNT_SMQUERY,
					0, //(u_short)spActive->dbMessageLength[0],
					0,//(time(NULL) - RoutingSendTime)+1,
					0,
					0,
					ProcessID,
					0, //(u_short)spActive->dbProtocolID[0],
					nPc,
					NULL);
			dbStatisticInsertbyRecord(&stStsRecord);
		}
		return(1);
	}
	
    /* update HLR pc and ssn */
	u_short sPc = nPc;
	memset(spActive->dbHlrPc, 0xff, DB_HLR_PC_SIZE);
	memcpy(&spActive->dbHlrPc[0], &sPc, PC_LENGTH);
	spActive->dbHlrPc[3] = HLR;

	/* over load control start
	 * by twkim 07.06.19 */

	stRDMsgCtrlTuple    *spTxRxCtrl;
	char    attVal[DB_MC_DPC_SIZE];
	int     nRate= 0;
	int     nCurentCnt= 0;
	int     nInstId_Temp = 0;

	memset(attVal,0xff,DB_MC_DPC_SIZE);
	attVal[0] = 0x7f;
	attVal[3] = HLR;

	if(dma_getTuplePtr(DBMS_STAT, R_DMSGCTRL, DB_MC_DPC, 0,
				attVal, DB_MC_DPC_SIZE, (char **)&spTxRxCtrl) != FLK_SUCCESS)
	{
		DebugStr(4, "[%s:%d] attVal[%02x%02x%02x%02x]"
				"Msg Trans Ctrn Get Ptr Fail!!\n", __func__, __LINE__, 
				attVal[0],attVal[1],attVal[2],attVal[3]);
	}
	else
	{
		if(spTxRxCtrl->dbLinkStatus[0] == ON)
		{
			bzero(attVal,DB_MC_DPC_SIZE);
			memcpy(&attVal[1],&sPc,PC_LENGTH);
			attVal[3] = HLR;

			if(dma_getTuplePtr(DBMS_STAT, R_DMSGCTRL, DB_MC_DPC, 0,
						attVal, DB_MC_DPC_SIZE, (char **)&spTxRxCtrl) != FLK_SUCCESS)
			{
			}
			else
			{
				
				nInstId_Temp = (((stMapParam.nInstId)+1))/2;

				nRate = ((spTxRxCtrl->dbDPC)+DB_MC_DPC_SIZE+DB_MC_LINK_STATUS_SIZE
						+DB_MC_SAVEFLAG_SIZE + DB_MC_RESERVED_SIZE
						+(nInstId_Temp*4)-(DB_MC_ASE_TX_RATIO_SIZE*4))[0];
				nCurentCnt = ((spTxRxCtrl->dbDPC)+DB_MC_DPC_SIZE+DB_MC_LINK_STATUS_SIZE
						+DB_MC_SAVEFLAG_SIZE + DB_MC_RESERVED_SIZE
						+(nInstId_Temp*4)-(DB_MC_ASE_RX_RATIO_SIZE*2))[0]++;


				if((nRate % 25) == 0)
				{
					if((nCurentCnt%4) < (nRate/25))
					{
						if(nCurentCnt == 100)
						{
							((spTxRxCtrl->dbDPC)+DB_MC_DPC_SIZE+DB_MC_LINK_STATUS_SIZE
							 +DB_MC_SAVEFLAG_SIZE + DB_MC_RESERVED_SIZE
							 +(nInstId_Temp*4)-(DB_MC_ASE_RX_RATIO_SIZE*2))[0] = 0;
						}
						
						ucStatus = MDB_ST_RETRY;
						usErrReason = ERR_N_TO_HLR_OVERLOAD_CONTROL;
						updateStatus(DBMS_ACTIVE,unMsgId, ucStatus, usErrReason);
				
						MakeStatisticsRecord( &stStsRecord,
								STATISTICS_RECORD_END,
								STATISTICS_TRYINF_NET,
								STATISTICS_TRYNRL_GSM,
								STATISTICS_TRYDIR_OUT,
								STATISTICS_TRYEVNT_SMQUERY,
								0,
								0,
								usErrReason,
								0,
								ProcessID,
								unMsgId,
								nPc,
								NULL);
						dbStatisticInsertbyRecord(&stStsRecord);
						return(-1);
					}
				}
				else if(nCurentCnt < nRate)
				{
					if(nCurentCnt == 100)
					{
						((spTxRxCtrl->dbDPC)+DB_MC_DPC_SIZE+DB_MC_LINK_STATUS_SIZE
						 +DB_MC_SAVEFLAG_SIZE + DB_MC_RESERVED_SIZE
						 +(nInstId_Temp*4)-(DB_MC_ASE_RX_RATIO_SIZE*2))[0] = 0;
					}
					ucStatus = MDB_ST_RETRY;
					usErrReason = ERR_N_TO_HLR_OVERLOAD_CONTROL;
					updateStatus(DBMS_ACTIVE,unMsgId, ucStatus, usErrReason);

					MakeStatisticsRecord( &stStsRecord,
							STATISTICS_RECORD_END,
							STATISTICS_TRYINF_NET,
							STATISTICS_TRYNRL_GSM,
							STATISTICS_TRYDIR_OUT,
							STATISTICS_TRYEVNT_SMQUERY,
							0,
							0,
							usErrReason,
							0,
							ProcessID,
							unMsgId,
							nPc,
							NULL);
					dbStatisticInsertbyRecord(&stStsRecord);
					return(-1);
				}
				else if(nCurentCnt == 100)
				{
					((spTxRxCtrl->dbDPC)+DB_MC_DPC_SIZE+DB_MC_LINK_STATUS_SIZE
					 +DB_MC_SAVEFLAG_SIZE + DB_MC_RESERVED_SIZE
					 +(nInstId_Temp*4)-(DB_MC_ASE_RX_RATIO_SIZE*2))[0] = 0;
				}
			}
		}
	}
	/* over load control end */
	
	/* fill Parameter */
	if(V3_fillRoutingInfoForSMR(&D, &stMsg) < 0) 
	{	
        sprintf((char *)cpBuf, "RoutingInfoForSMR:V3_fillRoutingInfoForSMR"
				" parameter fail [H'%x][%s]\n"
				, (int)unMsgId, stMapAddr.cAddStr);
        LogDebugTrace(2, (char *)cpBuf, stMapAddr.cpMin, stMapAddr.nMinLen);

        ucStatus    = MDB_ST_RETRY;
        usErrReason = ERR_PARAM_ERROR;
        updateStatus(DBMS_ACTIVE, unMsgId, ucStatus, usErrReason);
		DebugStr(DG_LEVEL_FIVE,"[%s : %d ]\n", __FILE__, __LINE__);
        MakeStatisticsRecord( &stStsRecord,
                STATISTICS_RECORD_END,
                STATISTICS_TRYINF_NET,
                STATISTICS_TRYNRL_GSM,
                STATISTICS_TRYDIR_OUT,
                STATISTICS_TRYEVNT_SMQUERY,
                isRetryCnt,
                0,
                usErrReason,
                0,
                ProcessID,
                0,
                nPc,
				NULL);
        dbStatisticInsertbyRecord(&stStsRecord);
		return(-1);
	}

	/* send message for Point Code */
#ifdef __GSM_ROUTE_PC__ 
	if(V3_BegInvRIFSMRPC(nPc, HLR, &stMsg, &D) < 0)
#else
	if(V3_BegInvRIFSMRGT(&stMsg, &D) < 0)
#endif 
	{
        sprintf((char *)cpBuf, "RoutingInfoForSMR:V3_BegInvRIFSMR"
				" parameter fail [H'%x][%s]\n", 
				(int)unMsgId, stMapAddr.cAddStr);
        LogDebugTrace(2, (char *)cpBuf, stMapAddr.cpMin, stMapAddr.nMinLen);

		ucStatus 	= MDB_ST_RETRY;
		usErrReason = SS7_LINK_PROBLEM;
		updateStatus(DBMS_ACTIVE, unMsgId, ucStatus, usErrReason);
        handleConnect(tc_errno);
	
		DebugStr(DG_LEVEL_FIVE,"[%s : %d ]\n", __FILE__, __LINE__);
        MakeStatisticsRecord( &stStsRecord,
                STATISTICS_RECORD_END,
                STATISTICS_TRYINF_NET,
                STATISTICS_TRYNRL_GSM,
                STATISTICS_TRYDIR_OUT,
                STATISTICS_TRYEVNT_SMQUERY,
                isRetryCnt,
                0,
                usErrReason,
                0,
                ProcessID,
                0,
                nPc,
				NULL);
        dbStatisticInsertbyRecord(&stStsRecord);
		return(-1);
	}
	else
	{
		/* Clear Message Wait Data */
		spActive->dbMsgWaitData[0] = 0x00;

		/*  User Error Diagnostic 08.01.29 by twkim */
		memset(spActive->dbHlrPcFailReason, 0x00, DB_MT_M_PC_FAIL_REASON_SIZE);
		memset(spActive->dbHlrPcDiag, 0xff, DB_MTGPC_DIAG_SIZE);

		memset(spActive->dbMtPcNetType, 0x00, DB_MT_PC_NETTYPE_SIZE);
		memset(spActive->dbMtMPc, 0x00, DB_MT_M_PC_SIZE);
		memset(spActive->dbMtMPcFailReason, 0x00, DB_MT_M_PC_FAIL_REASON_SIZE);
		memset(spActive->dbMtGPc, 0x00, DB_MT_G_PC_SIZE);
		memset(spActive->dbMtGPcFailReason, 0x00, DB_MT_G_PC_FAIL_REASON_SIZE);

		memset(spActive->dbMtGPcDiag, 0xff, DB_MTGPC_DIAG_SIZE);
		memset(spActive->dbMtMPcDiag, 0xff, DB_MTMPC_DIAG_SIZE);

		// DR240 08.11.19 by twkim
		// Alert 수신 후 MinMask time update가 없어 ASE_INTERNAL 발생 보완
		if(FLK_SUCCESS == GetActiveHeadPtr(DBMS_ACTIVE,(char*)spActive->dbDestMIN, &spMinMask1))
		{
			spMinMask1->unTime = (u_int)time(NULL);
		}

		ucStatus	= MDB_ST_RIFSM_R;
		usErrReason	= NULL;
		updateStatus(DBMS_ACTIVE, unMsgId, ucStatus, usErrReason);
	}

    updateMapInvokeTime(spActive);
	DebugStr(DG_LEVEL_FIVE,"[%s : %d ]\n", __FILE__, __LINE__);
    MakeStatisticsRecord( &stStsRecord,
            STATISTICS_RECORD_END,
            STATISTICS_TRYINF_NET,
            STATISTICS_TRYNRL_GSM,
            STATISTICS_TRYDIR_OUT,
            STATISTICS_TRYEVNT_SMQUERY,
            isRetryCnt,
            0,
            0,
            0,
            ProcessID,
            0,
            nPc,
			NULL);
    dbStatisticInsertbyRecord(&stStsRecord);
	SetDeliverTime(spActive);
	return(1);
}
/** @} */
