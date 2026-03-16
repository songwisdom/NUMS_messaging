
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

/*******************************************************************************
    FILE NAME       :  
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
#include "E4Noti.h"
#include "SKT_SvcInfo.h"
#include "GsmTpduValue.h"

#include "StatRecord.h"
#include "MdbDmaqi.h"
#include "libAnsiMap.h"
#include "libEsme.h"
#include "libConnHandler.h"
#include "smscTime.h"
extern stStatisticsRecord  stStsRecord;
extern u_int ProcessID;

extern int		nLong_TID;
extern int		nColor_TID;
extern int		nColLong_TID;
extern u_char	ucSaveCapa[DB_DEST_MS_CAPABILITY_SIZE];

#ifndef TCAP_ANSI90

u_char
V3_setNetworkNode(V3_RoutingInfoForSM_Res *pD)
{
	u_char	ucNetworkNodeFlag = 0x00;
	u_char	cTmp;
	/* modify by red9s */
	u_char	ucMtMPcTemp[DB_ISDN_SIZE];
	u_char	ucMtGPcTemp[DB_ISDN_SIZE];

	if(pD->locationInfoWithLMSI.m.additional_NumberPresent) /* Additional Number */
	{
		switch(pD->locationInfoWithLMSI.additional_Number.t)
		{
			case V3_Additional_Number_msc_Number	: /* msc number */
				memset(spActive->dbMtMPc, 0xff, DB_MT_M_PC_SIZE);
				memset(ucMtMPcTemp, 0xff, DB_ISDN_SIZE);
				if(V3_setGtAddType(ucMtMPcTemp, spActive->dbMPcAddType
					, &pD->locationInfoWithLMSI.additional_Number.u.msc_Number) < 0)
				{
					ucNetworkNodeFlag = 0x00;
					return ucNetworkNodeFlag;  
				} 
				else
				{
					if(GetISDNinfo(MSC_SECOND_TYPE, ucMtMPcTemp, spActive) == FLK_FAIL)
					{ 
						return (u_char)FLK_FAIL; // -1
					}
					ucNetworkNodeFlag = ucNetworkNodeFlag | MSC_SECOND_TYPE;
				}

				/* sgsn number */
				memset(spActive->dbMtGPc, 0xff, DB_MT_G_PC_SIZE);
				memset(ucMtGPcTemp, 0xff, DB_ISDN_SIZE);

				if(V3_setGtAddType(ucMtGPcTemp, spActive->dbGPcAddType
					, &pD->locationInfoWithLMSI.networkNode_Number) < 0)
				{
					ucNetworkNodeFlag = 0x00;
					return ucNetworkNodeFlag;  
				} 
				else
				{
					if(GetISDNinfo(SGSN_FIRST_TYPE, ucMtGPcTemp, spActive) == FLK_FAIL)
					{ 
						return (u_char)FLK_FAIL; // -1
					}
					ucNetworkNodeFlag = ucNetworkNodeFlag | SGSN_FIRST_TYPE;
				}

				break;
			case V3_Additional_Number_sgsn_Number	: /* sgsn number */
				memset(spActive->dbMtGPc, 0xff, DB_MT_G_PC_SIZE);
				memset(ucMtGPcTemp, 0xff, DB_ISDN_SIZE);

				if(V3_setGtAddType(ucMtGPcTemp, spActive->dbGPcAddType
					, &pD->locationInfoWithLMSI.additional_Number.u.sgsn_Number) < 0)
				{
					ucNetworkNodeFlag = 0x00;
					return ucNetworkNodeFlag;  
				} 
				else 
				{
					if(GetISDNinfo(SGSN_SECOND_TYPE, ucMtGPcTemp, spActive) == FLK_FAIL)
					{ 
						return (u_char)FLK_FAIL; // -1
					}
					ucNetworkNodeFlag = ucNetworkNodeFlag | SGSN_SECOND_TYPE;
				}

				/* msc number */
				memset(spActive->dbMtMPc, 0xff, DB_MT_M_PC_SIZE);
				memset(ucMtMPcTemp, 0xff, DB_ISDN_SIZE);
				memset(spActive->dbMtMPc, 0xff, DB_MT_M_PC_SIZE);
				if(V3_setGtAddType(ucMtMPcTemp, spActive->dbMPcAddType
					, &pD->locationInfoWithLMSI.networkNode_Number) < 0)
				{
					ucNetworkNodeFlag = 0x00;
					return ucNetworkNodeFlag;
				} 
				else
				{
					if(GetISDNinfo(MSC_FIRST_TYPE, ucMtMPcTemp, spActive) == FLK_FAIL)
					{ 
						return (u_char)FLK_FAIL; // -1
					}
					ucNetworkNodeFlag = ucNetworkNodeFlag | MSC_FIRST_TYPE;
				}

				break;
			default								:
				break;
		}
	}
	else if (pD->locationInfoWithLMSI.m.gprsNodeIndicatorPresent == 1) 
	{
		/* sgsn number */
		memset(spActive->dbMtGPc, 0xff, DB_MT_G_PC_SIZE);
		memset(ucMtGPcTemp, 0xff, DB_ISDN_SIZE);
		if(V3_setGtAddType(ucMtGPcTemp, spActive->dbGPcAddType
					, &pD->locationInfoWithLMSI.networkNode_Number) < 0)
		{
			ucNetworkNodeFlag = 0x00;
			return ucNetworkNodeFlag;
		}
		else
		{
			if(GetISDNinfo(SGSN_FIRST_TYPE, ucMtGPcTemp, spActive) == FLK_FAIL)
			{ 
				return (u_char)FLK_FAIL; // -1
			}
			ucNetworkNodeFlag = ucNetworkNodeFlag | SGSN_FIRST_TYPE;
		}
	}
	else
	{
		/* msc number */
		memset(spActive->dbMtMPc, 0xff, DB_MT_M_PC_SIZE);
		memset(ucMtMPcTemp, 0xff, DB_ISDN_SIZE);
		if(V3_setGtAddType(ucMtMPcTemp, spActive->dbMPcAddType
			, &pD->locationInfoWithLMSI.networkNode_Number) < 0)
		{
			ucNetworkNodeFlag = 0x00;
			return ucNetworkNodeFlag;
		} 
		else
		{
			if(GetISDNinfo(MSC_FIRST_TYPE, ucMtMPcTemp, spActive) == FLK_FAIL)
			{ 
				return (u_char)FLK_FAIL; // -1
			}
			ucNetworkNodeFlag = ucNetworkNodeFlag | MSC_FIRST_TYPE;
		}
	}
	return(ucNetworkNodeFlag);
}

int
V3_RoutingInfoForSMI(spPrim, spComp, spBuff)
tcx_primitive   *spPrim;
tcx_component   *spComp;
tcx_buffer      *spBuff;
{
	V3_RoutingInfoForSM_Res    D;
	stMsgBuf_t				stMsg;
	int						nIdx;
	int						nRet;
	u_char			cTmp;
	u_char			ucStatus;
	u_short			usErrReason;
	char                    cpBuf[512];

	int						nCAPA_Copy_Target, nCAPA_Check, nCAPA_Check_temp;
	int						nIs_DirectSMDPP_First   = 0;    // CFU check
	int						nIs_DirectSMDPP_Second  = 0;    // CAPA check
	int						nIs_DirectSMDPP_Third   = 0;    // ADR check
	int						nSpam_Activity          = 0;

	u_short			usErrorCode;
	u_int					unCurTime;

	int						nADRFlag = false;
	int						nCFUFlag = false;
	extern int				gnPortedNotiFlag;

	u_short			sPc;

	memset(&stMapAddr, 0x00, sizeof(stMapAddress_t));
	memset(&D, 0x00, sizeof(V3_RoutingInfoForSM_Res));
	stMapAddr.unMsgId = spPrim->user_dialogue_id;

    /* open Active DB */
	nRet = dma_getActiveTuplePtr(stMapAddr.unMsgId, &spActive);
    if(nRet < 0)
    {
        MakeStatisticsRecord( &stStsRecord,
                STATISTICS_RECORD_END,
                STATISTICS_TRYINF_NET,
                STATISTICS_TRYNRL_GSM,
                STATISTICS_TRYDIR_OUT_ACK,
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

	memcpy(&sPc, spActive->dbHlrPc, sizeof(short));

	if(getMinAddress(&stMapAddr, spActive) < 0)
    {
    }

	// updateMapInvokeAckTime(); // For KT-ICOM STATISTICS temporary coding	

	stMsg.nLen = spBuff->actual_length;
	memcpy(stMsg.cpData, spBuff->bufferp, stMsg.nLen);

	if(decV3_RoutingInfoForSM_Res(&stMsg, &D, 0x30) < 0)
	{
        sprintf(cpBuf,"decV3_RoutingInfoForSM_Res fail msgid[H'%x]\n", 
				(int)stMapAddr.unMsgId);
        LogDebugTrace(2, cpBuf, stMapAddr.cpMin, stMapAddr.nMinLen);

		ucStatus	= MDB_ST_RETRY;
		usErrReason	= ERR_PARAM_ERROR;
		updateStatus(DBMS_ACTIVE, stMapAddr.unMsgId, ucStatus, usErrReason);
		DebugStr(DG_LEVEL_FIVE,"[%s : %d ]\n", __FILE__, __LINE__);
        MakeStatisticsRecord( &stStsRecord,
                STATISTICS_RECORD_END,
                STATISTICS_TRYINF_NET,
                STATISTICS_TRYNRL_GSM,
                STATISTICS_TRYDIR_OUT_ACK,
                STATISTICS_TRYEVNT_SMQUERY,
                0,
                DeliverTimeInterval(spActive),
                usErrReason,
                0,
                ProcessID,
                0,
                sPc,
				NULL);
        dbStatisticInsertbyRecord(&stStsRecord);
		return(-1);
	}

    if(GetDebugLevel() >= 2) 
	{
		v3_debugRIFSMI(&D);
	}

    if(GetTraceLevel() > 0) 
	{
		if(CheckTraceMin(spActive->dbDestMIN, 
					spActive->dbDestLen[0]) == FLK_SUCCESS)
		{
			trace(spPrim, spComp, spBuff, "GSM 29-002 (SMSC <- HLR)");
			v3_traceRIFSMI(&D);
			traceEnd();
		}
		if(CheckTraceMin(spActive->dbSourceMIN,
					spActive->dbSourceLen[0]) == FLK_SUCCESS)
		{
			trace(spPrim, spComp, spBuff, "GSM 29-002 (SMSC <- HLR)");
			v3_traceRIFSMI(&D);
			traceEnd();
		}
	}
	
	/* set IMSI */ 	
	if(D.imsi.numocts > 0) 
	{
		// dbDestImsi setting.
		updataMapDestImsi(D.imsi.data, D.imsi.numocts, spActive); 
	}

	/* set LMSI */
	if(D.locationInfoWithLMSI.lmsi.numocts > 0)  
	{
		// dbLmsi setting
		updataMapLmsi(D.locationInfoWithLMSI.lmsi.data, 
				D.locationInfoWithLMSI.lmsi.numocts, spActive);
	}

	/* set Extension Container */
	if(D.m.extensionContainerPresent)
		updateRMIExtension(&D.extensionContainer, spActive);

	/* set ADR flag */
	if(D.extensionContainer.privateExtensionList.m.adrPresent)
		nADRFlag = true;

	/* set CFU flag */
	if(D.extensionContainer.privateExtensionList.m.msisdnPresent)
		nCFUFlag = true;

	/* Check Capability Service */
	OSFI_CAPA_IS_CHECK(ucSaveCapa, nCAPA_Copy_Target, nIs_DirectSMDPP_Second);

	if(nIs_DirectSMDPP_Second)
	{
		/* MS Type setting */
		setMsType(spActive->dbMtMsType, ucSaveCapa);

		u_int unDcsType = 0;
		if( (spActive->dbMtMsType[0] == MS_TYPE_INTEGRATION_SKT) || 
			(spActive->dbMtMsType[0] == MS_TYPE_INTEGRATION_KTF) )
		{
			unDcsType = getConvertDcs(spActive);
			if(unDcsType == DCS_TYPE_GSM7BIT)
			{
				if(checkConvSizeOver(spActive, unDcsType, stMapParam.nBit8UDMaxSize) == true)
				{
					TransINSERTActiveDELETE(spActive);
					return(1);
				}
			}
		}
		else if( (spActive->dbMtMsType[0] == MS_TYPE_GLOBAL_SKT) || 
				 (spActive->dbMtMsType[0] == MS_TYPE_GLOBAL_KTF) )
		{
			unDcsType = getConvertDcs(spActive);
			if( (unDcsType == DCS_TYPE_KSC5601) ||
				(unDcsType == DCS_TYPE_GSM7BIT ) )
			{
				if(checkConvSizeOver(spActive, unDcsType, stMapParam.nBit8UDMaxSize) == true)
				{
					TransINSERTActiveDELETE(spActive);
					return(1);
				}
			}
		}
		


		nIs_DirectSMDPP_Second = 0;

		//
		//	Patirot Service Processing
		//
		OSFI_CAPA_IN_OUT_CHECK(nCAPA_Copy_Target, ucSaveCapa,
								SKT_CAPA_PATRIOT, nCAPA_Check_temp);
		OSFI_CAPA_IN_OUT_CHECK(nCAPA_Copy_Target, spActive->dbDestMsCapability,
								SKT_CAPA_PATRIOT, nCAPA_Check);

		if( (nCAPA_Check != 1) && (nCAPA_Check_temp) )
		{
			OSFI_CAPA_ENABLE(nCAPA_Copy_Target, spActive->dbDestMsCapability,
								SKT_CAPA_PATRIOT);
		}

		if( (nCAPA_Check_temp) &&
			(D.extensionContainer.privateExtensionList.adr.data[0] == ADR_SKT_DENIED) )
		{
			OSFI_CAPA_ENABLE(nCAPA_Copy_Target, spActive->dbDestMsCapabilityFlag,
								SKT_CAPA_PATRIOT);
			//	disable OSFI NOTI and SKT_CAPA_PERFECT
			OSFI_CAPA_DISABLE(nCAPA_Copy_Target, spActive->dbOSFIFlag,
								SKT_OSFI_SMS_NOTI);

			ucStatus	= MDB_ST_SKT_SVC_RETRY;
			usErrReason	= 0;
			usErrorCode	= ERR_TC_U_SKT_DENIED;

			if(updateStatus(DBMS_ACTIVE, spPrim->user_dialogue_id, ucStatus, usErrReason)<0)
			{
				return(-1);
			}

			PutMsgId2SchdSndMsg(DBMS_ACTIVE, spPrim->user_dialogue_id, NULL);

			MakeStatisticsRecord( &stStsRecord,
                    STATISTICS_RECORD_END, 
                    STATISTICS_TRYINF_NET,
                    STATISTICS_TRYNRL_GSM,
                    STATISTICS_TRYDIR_OUT_ACK,
                    STATISTICS_TRYEVNT_SMQUERY,
					0,
					DeliverTimeInterval(spActive),
					usErrorCode,
					*(u_int *)(&spActive->dbMessageID[0]),
					ProcessID,
					0,
					sPc,
					NULL);
			dbStatisticInsertbyRecord(&stStsRecord);

			return(1);
		}
		else
		{
			OSFI_CAPA_DISABLE(nCAPA_Copy_Target, spActive->dbDestMsCapability,
								SKT_CAPA_PATRIOT);

		}	// Patriot Service Processing end

		//
		//	SPAM Service Processing
		//
		OSFI_CAPA_IN_OUT_CHECK(nCAPA_Copy_Target, ucSaveCapa,
								SKT_CAPA_SPAM, nCAPA_Check_temp);
		OSFI_CAPA_IN_OUT_CHECK(nCAPA_Copy_Target, spActive->dbDestMsCapability,
								SKT_CAPA_SPAM, nCAPA_Check);

		if((nCAPA_Check != 1)&&(nCAPA_Check_temp))
		{
			OSFI_CAPA_ENABLE(nCAPA_Copy_Target,spActive->dbDestMsCapability,
					SKT_CAPA_SPAM);
		}

		if( ((nCAPA_Check) || (nCAPA_Check_temp)) && (0x00 == spActive->dbSpamFlags[0]) ) 
		{
			char	cpCallbackNum[(DB_CALLBACK_NUMBER_SIZE*2)+1];
			BCD2str_callback(cpCallbackNum,(DB_CALLBACK_NUMBER_SIZE*2)+1,
								spActive->dbCallbackNumber,DB_CALLBACK_NUMBER_SIZE);

			nSpam_Activity = isCallbackSpamNum(cpCallbackNum);

			if( (spActive->dbMsgFlowType[0] == FTYPE_ESME_TO_MS) && (nSpam_Activity == 0) )
			{
				char	cpSourceMin[DB_SOURCE_MIN_SIZE];
				BCD2str_min(cpSourceMin, DB_SOURCE_MIN_SIZE,
								spActive->dbSourceMIN, DB_SOURCE_MIN_SIZE);

				nSpam_Activity = isSourceMinSpamNum(cpSourceMin);
			}

			if(nSpam_Activity == 2)
			{
				ucStatus = MDB_ST_SKT_SVC_RETRY;
				usErrReason = 0; // ErrReason is 0
				usErrorCode = ERR_SMS_SKT_SPAM;
                memcpy(spActive->dbErrorCode, &usErrorCode, sizeof(u_short));

				// SPAM 차단 시 모든 착신 서비스를 disable
				memset(spActive->dbDestMsCapability,0x00,DB_DEST_MS_CAPABILITY_SIZE);
	            memset(spActive->dbDestMsCapabilityFlag,0x00,DB_DEST_MS_CAPABILITY_SIZE);

				// Add for OAM SPAM Block Display....
				memset(spActive->dbSpamFlags, 0x00, DB_SPAM_FLAG_SIZE);
				spActive->dbSpamFlags[0] = 0x01;
				OSFI_CAPA_ENABLE(nCAPA_Copy_Target,spActive->dbDestMsCapabilityFlag,
									SKT_CAPA_SPAM);

				// Active Tuple에 ErrorCode update
				if(updateStatus(DBMS_ACTIVE,spPrim->user_dialogue_id, ucStatus, usErrReason)<0)
				{
					return(-1);
				}

				// SPAM Server로의 메시지 전송을 위해 Send Schd로 보냄
				PutMsgId2SchdSndMsg(DBMS_ACTIVE,spPrim->user_dialogue_id,NULL);
				return(1);
			}
			else if(nSpam_Activity == 1)
			{
                ucStatus = MDB_ST_FAIL;
                usErrReason =  ERR_SMS_SKT_SPAM;
                updateStatus(DBMS_ACTIVE,spPrim->user_dialogue_id, ucStatus, usErrReason);
                updateMapDeliveryTime(spActive);
                DebugStr(4,"start MakeStatisticsRecord STATISTICS_TRYDIR_OUT_ACK recv hlr \n");
                MakeStatisticsRecord( &stStsRecord,
                        STATISTICS_RECORD_END,
                        STATISTICS_TRYINF_NET,
                        STATISTICS_TRYNRL_GSM,
                        STATISTICS_TRYDIR_OUT_ACK,
                        STATISTICS_TRYEVNT_SMQUERY,
                        0, //(u_short)spActive->dbMessageLength[0],
                        0,//(time(NULL) - ReportSendTime)+1,
                        usErrReason,//(u_int)spActive->dbFailReason[0],
                        *(u_int *)(&spActive->dbMessageID[0]),
                        ProcessID,
                        0, //(u_short)spActive->dbProtocolID[0],
                        sPc,
                        NULL);
                dbStatisticInsertbyRecord(&stStsRecord);
                return(1);
            }
			else if(nSpam_Activity > 0)
			{
            }

            // SPAM인지 확인 했다는 것을 남기기 위해 추가
            memset(spActive->dbSpamFlags, 0x00, DB_SPAM_FLAG_SIZE);
            spActive->dbSpamFlags[0] = 0x02;

		}	// SPAM Service Processing end

		/* New SPAM SVC Processing Start by twkim 070822 */
		OSFI_CAPA_IN_OUT_CHECK(nCAPA_Copy_Target, ucSaveCapa,
				SKT_CAPA_NEW_SPAM, nCAPA_Check_temp);
		OSFI_CAPA_IN_OUT_CHECK(nCAPA_Copy_Target, spActive->dbDestMsCapability,
				SKT_CAPA_NEW_SPAM, nCAPA_Check);
		if((nCAPA_Check != 1) && (nCAPA_Check_temp))
		{
			OSFI_CAPA_ENABLE(nCAPA_Copy_Target,spActive->dbDestMsCapability,
					SKT_CAPA_NEW_SPAM);
		}
		if((nCAPA_Check_temp) && (nADRFlag == false)
				&& (spActive->dbNewSpamSVCStatus[0] == SUB_ST_BASE))
		{
			u_short ls_nResult = 0;
			memcpy(&ls_nResult, spActive->dbTeleserviceID, sizeof(u_short));
			//TID가 CMT, CPT, COLOR, WPUSH, MMS일때 만 서비스 수행
			if( (ls_nResult == 4098) || (ls_nResult == 4097) || (ls_nResult == nColor_TID) ||
					(ls_nResult == 65491) || (ls_nResult == 63250) || (ls_nResult == 61867) )
			{
				OSFI_CAPA_ENABLE(nCAPA_Copy_Target,spActive->dbDestMsCapabilityFlag,
						SKT_CAPA_NEW_SPAM);
				nIs_DirectSMDPP_Second ++;
			}
		}
		/* New SPAM SVC Processing End */

		//
		//	ICS Service Processing
		//
		OSFI_CAPA_IN_OUT_CHECK(nCAPA_Copy_Target, ucSaveCapa,
								SKT_CAPA_ICS, nCAPA_Check_temp);
        OSFI_CAPA_IN_OUT_CHECK(nCAPA_Copy_Target, spActive->dbDestMsCapability,
                                SKT_CAPA_ICS, nCAPA_Check);

        if( (nCAPA_Check != 1) && (nCAPA_Check_temp) && (nADRFlag == false) )
		{
            OSFI_CAPA_ENABLE(nCAPA_Copy_Target, spActive->dbDestMsCapability,
                    			SKT_CAPA_ICS);
        }

		if( (nCAPA_Check_temp) && (spActive->dbIcsSVCStatus[0] == SUB_ST_BASE) )
		{
			OSFI_CAPA_ENABLE(nCAPA_Copy_Target, spActive->dbDestMsCapabilityFlag,
					SKT_CAPA_ICS);
			nIs_DirectSMDPP_Second ++;
		}	// ICS Service Processing end

		//
		//PICS Service Processing
		//
		OSFI_CAPA_IN_OUT_CHECK(nCAPA_Copy_Target, ucSaveCapa,
				SKT_CAPA_PICS, nCAPA_Check_temp);
		OSFI_CAPA_IN_OUT_CHECK(nCAPA_Copy_Target, spActive->dbDestMsCapability,
				SKT_CAPA_PICS, nCAPA_Check);
		if( (nCAPA_Check != 1) && (nCAPA_Check_temp) )
		{
			OSFI_CAPA_ENABLE(nCAPA_Copy_Target, spActive->dbDestMsCapability,
					SKT_CAPA_PICS);
		}

		if( (nCAPA_Check_temp) && (nADRFlag == false)
				&& (spActive->dbPicsSVCStatus[0] == SUB_ST_BASE) )
		{
			if(spActive->dbCallbackLen[0] > 2)
			{
				if(nCAPA_Check!=1)
				{
					OSFI_CAPA_ENABLE(nCAPA_Copy_Target, spActive->dbDestMsCapabilityFlag,
							SKT_CAPA_PICS);
					DLOG(("TEST LOG IN PICS \n"));
					nIs_DirectSMDPP_Second++;
				}
				else if((spActive->dbPicsSVCtime[0] == 0x00) &&
						(spActive->dbPicsSVCtime[1] == 0x00))
				{
					OSFI_CAPA_IN_OUT_CHECK(nCAPA_Copy_Target, spActive->dbDestMsCapabilityFlag,
							SKT_CAPA_PICS, nCAPA_Check);
					if(nCAPA_Check != 1)
					{
						OSFI_CAPA_ENABLE(nCAPA_Copy_Target, spActive->dbDestMsCapabilityFlag,
								SKT_CAPA_PICS);
						DLOG(("TEST LOG IN PICS 2\n"));
						nIs_DirectSMDPP_Second++;
					}
				}
			}
			else
			{
				spActive->dbPicsSVCStatus[0] = SUB_ST_SVC_DELIVER_READY;
				unCurTime = (u_int)time(NULL);
				memcpy(spActive->dbPicsSVCtime , &unCurTime, sizeof(MDB_TIME_TYPE_SIZE));
			}

		}   // PICS Service Processing end


		//
		//	LONG Service Processing
		//
		OSFI_CAPA_IN_OUT_CHECK(nCAPA_Copy_Target, ucSaveCapa,
                                SKT_CAPA_LONG, nCAPA_Check_temp);
        OSFI_CAPA_IN_OUT_CHECK(nCAPA_Copy_Target, spActive->dbDestMsCapability,
                                SKT_CAPA_LONG, nCAPA_Check);

        if( (nCAPA_Check != 1) && (nCAPA_Check_temp) )
		{
            OSFI_CAPA_ENABLE(nCAPA_Copy_Target, spActive->dbDestMsCapability,
                    			SKT_CAPA_LONG);
        }
        if( (nCAPA_Check!=1) && (nCAPA_Check_temp) )
		{
            OSFI_CAPA_ENABLE(nCAPA_Copy_Target, spActive->dbDestMsCapabilityFlag,
                            	SKT_CAPA_LONG);
            nIs_DirectSMDPP_Second ++;
        }

		//
		//	COLOR & LONG Service Processing
		//
		OSFI_CAPA_IN_OUT_CHECK(nCAPA_Copy_Target, ucSaveCapa,
                                SKT_CAPA_COLOR_LONG, nCAPA_Check_temp);
        OSFI_CAPA_IN_OUT_CHECK(nCAPA_Copy_Target, spActive->dbDestMsCapability,
                                SKT_CAPA_COLOR_LONG, nCAPA_Check);

        if( (nCAPA_Check != 1) && (nCAPA_Check_temp) )
        {
            OSFI_CAPA_ENABLE(nCAPA_Copy_Target, spActive->dbDestMsCapability,
                    			SKT_CAPA_COLOR_LONG);
        }
        if( (nCAPA_Check!=1) && (nCAPA_Check_temp) && (nADRFlag == false) )
        {
            OSFI_CAPA_ENABLE(nCAPA_Copy_Target, spActive->dbDestMsCapabilityFlag,
                            	SKT_CAPA_COLOR_LONG);
            nIs_DirectSMDPP_Second ++;
        }

		//
		//	Secret Service Processing
		//
		if(spActive->dbCallbackLen[0] > 2)
		{
            OSFI_CAPA_IN_OUT_CHECK(nCAPA_Copy_Target, ucSaveCapa,
                                    SKT_CAPA_SECRETSMS, nCAPA_Check_temp);
            OSFI_CAPA_IN_OUT_CHECK(nCAPA_Copy_Target, spActive->dbDestMsCapability,
                                    SKT_CAPA_SECRETSMS, nCAPA_Check);
            if( (nCAPA_Check != 1) && (nCAPA_Check_temp) )
			{
                OSFI_CAPA_ENABLE(nCAPA_Copy_Target, spActive->dbDestMsCapability,
                        			SKT_CAPA_SECRETSMS);
            }
            if( (nCAPA_Check_temp) && (nADRFlag == false) )
			{
                u_short ls_nResult=0;
                int     nCompResult = 0;
                char srvtype_temp[DB_SERVICE_TYPE_SIZE];

                memcpy(&ls_nResult, spActive->dbTeleserviceID, sizeof(u_short));

                if(nCAPA_Check!=1)
				{
                    if( (ls_nResult == 4098) || (ls_nResult == 4097) || (ls_nResult == nColor_TID) ||
                        (ls_nResult == nLong_TID) || (ls_nResult == nColLong_TID) )
					{
                        if(strlen((char *)spActive->dbServiceType))
						{
                            bzero(srvtype_temp,sizeof(srvtype_temp));
                            strncpy(srvtype_temp,"WPUSH",DB_SERVICE_TYPE_SIZE-1);
                            nCompResult = strncmp((char *)srvtype_temp,(char *)spActive->dbServiceType,
                                                    strlen((char *)spActive->dbServiceType));
                            DLOG(("TEST JIN srvtype_temp[%s],spActive->dbServiceType[%s], nCompResult[%d]\n",
                                    srvtype_temp, spActive->dbServiceType, nCompResult));

                            if(nCompResult !=0)
							{
                                OSFI_CAPA_ENABLE(nCAPA_Copy_Target, spActive->dbDestMsCapabilityFlag,
                                        			SKT_CAPA_SECRETSMS);
                                nIs_DirectSMDPP_Second ++;
                            }
                        }
                        else
						{
                            OSFI_CAPA_ENABLE(nCAPA_Copy_Target, spActive->dbDestMsCapabilityFlag,
                                                SKT_CAPA_SECRETSMS);
                            nIs_DirectSMDPP_Second ++;
                        }
                    }
                    else
                    {
                        spActive->dbScsmssSVCStatus[0] = SUB_ST_DEL_UNAVAIL_TID;
                        unCurTime = (u_int)time(NULL);
                        memcpy(spActive->dbScsmssSVCtime, &unCurTime, sizeof(MDB_TIME_TYPE_SIZE));
                    }
                }
                else if( (spActive->dbScsmssSVCtime[0] == 0x00) &&
                         (spActive->dbScsmssSVCtime[1] == 0x00) )
				{
                    OSFI_CAPA_IN_OUT_CHECK(nCAPA_Copy_Target, spActive->dbDestMsCapabilityFlag,
                                            SKT_CAPA_SECRETSMS, nCAPA_Check);
                    if(nCAPA_Check != 1)
					{
                        if( (ls_nResult == 4098) || (ls_nResult == 4097) || (ls_nResult == nColor_TID) ||
                        	(ls_nResult == nLong_TID) || (ls_nResult == nColLong_TID))
						{
                            if(strlen((char *)spActive->dbServiceType))
							{
                                bzero(srvtype_temp,sizeof(srvtype_temp));
                                strncpy(srvtype_temp,"WPUSH",DB_SERVICE_TYPE_SIZE-1);
                                nCompResult = strncmp((char *)srvtype_temp,(char *)spActive->dbServiceType,
                                                        strlen((char *)spActive->dbServiceType));
                                DLOG(("TEST JIN srvtype_temp[%s],spActive->dbServiceType[%s], nCompResult[%d]\n",
                                        srvtype_temp, spActive->dbServiceType, nCompResult));
                                if(nCompResult !=0)
								{
                                    OSFI_CAPA_ENABLE(nCAPA_Copy_Target, spActive->dbDestMsCapabilityFlag,
                                                    SKT_CAPA_SECRETSMS);
                                    nIs_DirectSMDPP_Second ++;
                                }
                            }
                            else
							{
                                OSFI_CAPA_ENABLE(nCAPA_Copy_Target, spActive->dbDestMsCapabilityFlag,
                                                    SKT_CAPA_SECRETSMS);
                                nIs_DirectSMDPP_Second ++;
                            }
                        }
                        else
                        {
                            spActive->dbScsmssSVCStatus[0] = SUB_ST_DEL_UNAVAIL_TID;
                            unCurTime = (u_int)time(NULL);
                            memcpy(spActive->dbScsmssSVCtime, &unCurTime, sizeof(MDB_TIME_TYPE_SIZE));
                        }
                    }
                }
            }
        }	// Secret Service Processing end

		//
		//	Query SMS Service Processing
		//
		OSFI_CAPA_IN_OUT_CHECK(nCAPA_Copy_Target, ucSaveCapa,
                                SKT_CAPA_QUERYSMS, nCAPA_Check_temp);

        OSFI_CAPA_IN_OUT_CHECK(nCAPA_Copy_Target, spActive->dbDestMsCapability,
                                SKT_CAPA_QUERYSMS, nCAPA_Check);

        if( (nCAPA_Check != 1) && (nCAPA_Check_temp))
        {
            OSFI_CAPA_ENABLE(nCAPA_Copy_Target, spActive->dbDestMsCapability,
                    			SKT_CAPA_QUERYSMS);
            OSFI_CAPA_ENABLE(nCAPA_Copy_Target, spActive->dbDestMsCapabilityFlag,
                    			SKT_CAPA_QUERYSMS);
        }

		//
		//	IMSMS Service Processing
		//
		OSFI_CAPA_IN_OUT_CHECK(nCAPA_Copy_Target, ucSaveCapa,
                    			SKT_CAPA_IMSMS, nCAPA_Check_temp);
        OSFI_CAPA_IN_OUT_CHECK(nCAPA_Copy_Target, spActive->dbDestMsCapability,
                				SKT_CAPA_IMSMS, nCAPA_Check);

        if( (nCAPA_Check != 1) && (nCAPA_Check_temp) )
        {
            OSFI_CAPA_ENABLE(nCAPA_Copy_Target,spActive->dbDestMsCapability,
                    			SKT_CAPA_IMSMS);
        }
        if( (nCAPA_Check_temp) && (nADRFlag == false)
				&& (spActive->dbIMSMSStatus[0] == SUB_ST_BASE) )
        {
            /*  IMSMS : CMT, CPT, LONG Service */
            u_short ls_nResult = 0;
            memcpy(&ls_nResult, spActive->dbTeleserviceID, sizeof(u_short));

            if( (ls_nResult == 4098) || (ls_nResult == 4097) || (ls_nResult == nColor_TID) ||
				(ls_nResult == nLong_TID) || (ls_nResult == nColLong_TID) )
			{
                OSFI_CAPA_ENABLE(nCAPA_Copy_Target,spActive->dbDestMsCapabilityFlag,
                        			SKT_CAPA_IMSMS);
                nIs_DirectSMDPP_Second ++;
            }
            else
            {
                spActive->dbIMSMSStatus[0] = SUB_ST_DEL_UNAVAIL_TID;
                unCurTime = (u_int)time(NULL);
                memcpy(spActive->dbIMSMStime, &unCurTime, sizeof(MDB_TIME_TYPE_SIZE));
            }
	
        }	// IMSMS Service Processing end

		//
		//	 MGR Servcie Processing
		//
		OSFI_CAPA_IN_OUT_CHECK(nCAPA_Copy_Target, ucSaveCapa,
                				SKT_CAPA_MGR, nCAPA_Check_temp);
        OSFI_CAPA_IN_OUT_CHECK(nCAPA_Copy_Target, spActive->dbDestMsCapability,
                				SKT_CAPA_MGR, nCAPA_Check);
    
        if( (nCAPA_Check!=1) && (nCAPA_Check_temp) )
		{
            OSFI_CAPA_ENABLE(nCAPA_Copy_Target, spActive->dbDestMsCapability,
                    			SKT_CAPA_MGR);
        }

		/* PERFECT_AA SVC by twkim 07.02.13
		 * Perfect_AA SVC 판단 루틴 */
		OSFI_CAPA_IN_OUT_CHECK(nCAPA_Copy_Target, ucSaveCapa,
								SKT_CAPA_PERFECT_AA, nCAPA_Check_temp);
		OSFI_CAPA_IN_OUT_CHECK(nCAPA_Copy_Target,spActive->dbDestMsCapability,
								SKT_CAPA_PERFECT_AA, nCAPA_Check);
		if((nCAPA_Check != 1) && (nCAPA_Check_temp))
		{
			OSFI_CAPA_ENABLE(nCAPA_Copy_Target,spActive->dbDestMsCapability,
					SKT_CAPA_PERFECT_AA);
		}
	
		/* DR310 PKG Modify Number by peterJo Plus 2009. 02. 15*/
		if((nCAPA_Check_temp) && (nADRFlag == false)
				&& (spActive->dbPerfectCAPAStatus[0] == SUB_ST_BASE)
				&& (spActive->dbDefMsgID[0] == 0))
		{
			if(true == isAutoAnsSrcMin(spActive))
			{
				u_short ls_nResult = 0;
				memcpy(&ls_nResult, spActive->dbTeleserviceID, sizeof(u_short));
				//long 메시지이면 끝 segment만 안심 처리
				if( ls_nResult == nLong_TID && (getTranType(spActive->dbMsgTransformCnt) != 0))
				{
					if( (getMoTransCnt(spActive->dbMsgTransformCnt) == getMtTransCnt(spActive->dbMsgTransformCnt)) &&
						(spActive->dbLongTotSegment[0] != 0) )
					{
						OSFI_CAPA_ENABLE(nCAPA_Copy_Target,spActive->dbDestMsCapabilityFlag,
								SKT_CAPA_PERFECT_AA);
						nIs_DirectSMDPP_Second ++;
					}
				}
				else if( ls_nResult == nColLong_TID || ls_nResult == nLong_TID || ls_nResult == nColor_TID ||
					(getTranType(spActive->dbMsgTransformCnt) != 0) )
				{
					if(spActive->dbLongTotSegment[0] == spActive->dbLongCurSegment[0])
					{
						OSFI_CAPA_ENABLE(nCAPA_Copy_Target,spActive->dbDestMsCapabilityFlag,
								SKT_CAPA_PERFECT_AA);
						nIs_DirectSMDPP_Second ++;
					}
				}
				//모든 TID에 대해서 착신안심 서비스 처리
				else
				{
					OSFI_CAPA_ENABLE(nCAPA_Copy_Target,spActive->dbDestMsCapabilityFlag,
							SKT_CAPA_PERFECT_AA);
					nIs_DirectSMDPP_Second ++;
				}
			}
		}
		/* PERFECT_AA End */

		//
		//	 Abroad-Tel Servcie Processing
		//
		OSFI_CAPA_IN_OUT_CHECK(nCAPA_Copy_Target, ucSaveCapa,
                				SKT_CAPA_ABROADTEL, nCAPA_Check_temp);
		OSFI_CAPA_IN_OUT_CHECK(nCAPA_Copy_Target,spActive->dbDestMsCapability,
								SKT_CAPA_ABROADTEL, nCAPA_Check);

		if((nCAPA_Check != 1) && (nCAPA_Check_temp))
		{
            OSFI_CAPA_ENABLE(nCAPA_Copy_Target, spActive->dbDestMsCapability,
                    			SKT_CAPA_ABROADTEL);
        }

	}	// Capability Service end

	/*	SKT Number Plus Service */
	if( (nCFUFlag) && (!nADRFlag) )
	{
		if(spActive->dbDefMsgID[0] >= 2)
		{
			spActive->dbRedirectUsable[0] = SKT_FORWARD_SVC_FAIL;
			ucStatus	= MDB_ST_FAIL;
			usErrReason	= ERR_SMS_SKT_FORWARDING_FAIL;
			usErrorCode	= ERR_SMS_SKT_FORWARDING_FAIL;
			memcpy(spActive->dbErrorCode, &usErrorCode, sizeof(u_short));
			memset(spActive->dbDestMsCapability, 0x00, DB_DEST_MS_CAPABILITY_SIZE);
			memset(spActive->dbDestMsCapabilityFlag, 0x00, DB_DEST_MS_CAPABILITY_SIZE);
		}

		else
		{
			nIs_DirectSMDPP_First = 1;

			u_short	ls_nResult = 0;
			memcpy(&ls_nResult, spActive->dbTeleserviceID, sizeof(u_short));

			spActive->dbRedirectUsable[0] = SKT_FORWARD_SVC_NUM_PLUS_NEW;
			ucStatus	= MDB_ST_SKT_SVC_RETRY;
			usErrReason	= ERR_SMS_SKT_FORWARDIG_NUM_PLUS;
			
			memset(spActive->dbDestMsCapability, 0x00, DB_DEST_MS_CAPABILITY_SIZE);
			memset(spActive->dbDestMsCapabilityFlag, 0x00, DB_DEST_MS_CAPABILITY_SIZE);

			memcpy(spActive->dbDestMSMIN, spActive->dbDestRedirectMIN, spActive->dbDestRedirectLen[0]);
			spActive->dbDestMSLen[0] = spActive->dbDestRedirectLen[0];

			if( (spActive->dbDestMSMIN[0] & 0xF0) != 0x00
				&& (spActive->dbDestMSLen[0] == MDB_MSMIN_SIZE) )
			{
				memset(spActive->dbDestRedirectMIN, 0xff, DB_DESTREDIRECT_MIN_SIZE);

				for(nIdx=0; nIdx<=spActive->dbDestMSLen[0]; nIdx++)
				{
					if(nIdx == 0)
					{
						spActive->dbDestRedirectMIN[nIdx] = 0x00 << 4;
						spActive->dbDestRedirectMIN[nIdx] |= spActive->dbDestMSMIN[nIdx] >> 4;
					}
					else if(nIdx < spActive->dbDestMSLen[0])
					{
						spActive->dbDestRedirectMIN[nIdx] = spActive->dbDestMSMIN[nIdx-1] << 4;
						spActive->dbDestRedirectMIN[nIdx] |= spActive->dbDestMSMIN[nIdx] >> 4;
					}
					else if(nIdx = spActive->dbDestMSLen[0])
					{
						spActive->dbDestRedirectMIN[nIdx] = spActive->dbDestMSMIN[nIdx-1] << 4;
						spActive->dbDestRedirectMIN[nIdx] |= 0x0f;
					}
				}

				//	Check PFX last digit length
				//	Extension MSISDN use 3 digits PFX (also 4 digits)
				//	ex) 82-11-720-2222 (117202222f)
				if( (spActive->dbDestMSMIN[4] & 0x0f) == 0x0f )
					spActive->dbDestRedirectLen[0] = spActive->dbDestMSLen[0];
				else
					spActive->dbDestRedirectLen[0] = spActive->dbDestMSLen[0] + 1;
			}

			int nTypeResult = 0;

			if( (nTypeResult = CheckEsmeType((char *)&spActive->dbEsmeId)) == FLK_FAIL )
			{
				DebugStr(4, "[%s:%d] Unknown or Mismatched ESMEID.\n", __FILE__, __LINE__);
			}

			/* Message from MO, ETC ESME send delivery_sm to HSMSS */  
			if( (nTypeResult == ESME_CLASS_ETC) ||
					(spActive->dbMsgFlowType[0] == FROM_MS) )
			{
				DebugStr(4, "[ADR:%d][EsmeType:%d] FROM_MS or ETC_ESME\n",
						D.extensionContainer.privateExtensionList.adr.data[0], nTypeResult);

				ucStatus = MDB_ST_SKT_SVC_RETRY;

				usErrorCode = ERR_SMS_SKT_FORWARDING_ETC;
				memcpy(spActive->dbErrorCode, &usErrorCode, sizeof(u_short));
			}
			else
			{
				/* Message from H/VSMSS, send receipt_msg to H/VSMSS */
				DebugStr(4, "[ADR:%d][EsmeType:%d] FROM H/VSMSS or XMSS\n",
						D.extensionContainer.privateExtensionList.adr.data[0], nTypeResult);

				ucStatus = MDB_ST_SKT_SVC_RETRY;
				usErrorCode = ERR_SMS_SKT_FORWARDING_ESME;
				memcpy(spActive->dbErrorCode, &usErrorCode, sizeof(u_short));
			}
		}

		if(updateStatus(DBMS_ACTIVE,spPrim->user_dialogue_id, ucStatus, usErrReason)<0)
		{
			return(-1);
		}
		if(ucStatus == MDB_ST_FAIL)
		{
			PutMsgId2Uhis(DBMS_ACTIVE, stMapAddr.unMsgId, CHK_2UHIS);
			return(-1);
		}
		if(ucStatus == MDB_ST_SKT_SVC_RETRY)
		{
			PutMsgId2SchdSndMsg(DBMS_ACTIVE, spPrim->user_dialogue_id, NULL);
			return(1);
		}
	}

	int	nNotiFlag = false; // ported noti check flag
	u_short ls_nResult = 0;
	int nTypeResult = 0;
	/* AccessDeniedReason Processing */
	if(nADRFlag)
	{
		nIs_DirectSMDPP_Third = 1;

		switch(D.extensionContainer.privateExtensionList.adr.data[0])
        {
            case    ADR_SKT_NEW_CFUSM	: // SMS Forwarding (New_CFUSM)
            case    ADR_SKT_CHG_CFUSM	: // Number Change (ChgNum_CFUSM)    
			case	ADR_SKT_PORTED_NOTI	: // Ported-Noti Service 
                DebugStr(4, "[%s:%d] AccessDeniedReason Present : [%02x]\n", __func__, __LINE__,
							D.extensionContainer.privateExtensionList.adr.data[0]);
    
				memcpy(&ls_nResult, spActive->dbTeleserviceID, sizeof(u_short));

				//
				//	Set Forward MIN
				//
				memcpy(spActive->dbDestMSMIN, spActive->dbDestRedirectMIN,
											  spActive->dbDestRedirectLen[0]);
				spActive->dbDestMSLen[0] = spActive->dbDestRedirectLen[0];

				if( (spActive->dbDestMSMIN[0] & 0xF0) != 0x00
					&& (spActive->dbDestMSLen[0] == MDB_MSMIN_SIZE) )
				{
					memset(spActive->dbDestRedirectMIN, 0xff, DB_DESTREDIRECT_MIN_SIZE);
	
					for(nIdx=0; nIdx<=spActive->dbDestMSLen[0]; nIdx++)
					{
						if(nIdx == 0)
						{
							spActive->dbDestRedirectMIN[nIdx] = 0x00 << 4;
							spActive->dbDestRedirectMIN[nIdx] |= spActive->dbDestMSMIN[nIdx] >> 4;
						}
						else if(nIdx < spActive->dbDestMSLen[0])
						{
							spActive->dbDestRedirectMIN[nIdx] = spActive->dbDestMSMIN[nIdx-1] << 4;
							spActive->dbDestRedirectMIN[nIdx] |= spActive->dbDestMSMIN[nIdx] >> 4;
						}
						else if(nIdx = spActive->dbDestMSLen[0])
						{
							spActive->dbDestRedirectMIN[nIdx] = spActive->dbDestMSMIN[nIdx-1] << 4;
							spActive->dbDestRedirectMIN[nIdx] |= 0x0f;
						}
					}

					//	Check PFX last digit length
					//	Extension MSISDN use 3 digits PFX (also 4 digits)
					//	ex) 82-11-720-2222 (117202222f)
					if( (spActive->dbDestMSMIN[4] & 0x0f) == 0x0f )
						spActive->dbDestRedirectLen[0] = spActive->dbDestMSLen[0];
					else
						spActive->dbDestRedirectLen[0] = spActive->dbDestMSLen[0] + 1;
				}

                if(!D.extensionContainer.privateExtensionList.m.msisdnPresent)
                {   
					ucStatus	= MDB_ST_FAIL;
					usErrReason	= ERR_SMS_SKT_FORWARDING_FAIL;
					usErrorCode	= ERR_SMS_SKT_FORWARDING_FAIL;
					memcpy(spActive->dbErrorCode, &usErrorCode, sizeof(u_short));

					spActive->dbRedirectUsable[0] = SKT_FORWARD_SVC_FAIL;

					break;
                }

				//
				//	Check CFU Service
				//
				if(D.extensionContainer.privateExtensionList.adr.data[0] == ADR_SKT_NEW_CFUSM)
					usErrReason = ERR_SMS_SKT_NEW_FORWARDING;
				else if(D.extensionContainer.privateExtensionList.adr.data[0] == ADR_SKT_CHG_CFUSM)
					usErrReason = ERR_SMS_SKT_NUM_CHANGE;
				else
				{
					//
					//	 SKT Ported-Noti-SMS Service by bjlee
					//

					usErrReason = ERR_SMS_SKT_PORTED_NOTI_SMS;
							

					//	Initialize Capability Service
					memset(spActive->dbDestMsCapability, 0x00, DB_DEST_MS_CAPABILITY_SIZE);
					memset(spActive->dbDestMsCapabilityFlag, 0x00, DB_DEST_MS_CAPABILITY_SIZE);

					if(true == isNotiSvcNum(spActive))
					{
						if( (ls_nResult == nColLong_TID) || (ls_nResult == nLong_TID) || (ls_nResult == nColor_TID) ||
								(getTranType(spActive->dbMsgTransformCnt) != 0) )
						{
							if(spActive->dbLongTotSegment[0] == spActive->dbLongCurSegment[0])
							{
								nNotiFlag = true;
							}
						}
						else
						{
							nNotiFlag = true;
						}
					}
				}

				//
				//	Check ESME Block SourceMIN
				//	ping-pong patch
				//
				if(spActive->dbMsgFlowType[0] == FROM_ESME)
				{
					if(GetEsmeSvcTypeBySrcMIN(&spActive->dbSourceMIN[0],
	                            &spActive->dbEsmeServiceType[0]) == FLK_SUCCESS)
	                {
	                    DebugStr(4, "[%s:%d] dbEsmeServiceType [%02x]\n", __FILE__, __LINE__,
	                                spActive->dbEsmeServiceType[0]);

						if(spActive->dbEsmeServiceType[0] == 0xff)
	                    {
							ucStatus = MDB_ST_FAIL;
	                        usErrReason = ERR_SMS_SKT_CFU_UNAVAILABLE;
	                        usErrorCode = ERR_SMS_SKT_CFU_UNAVAILABLE;
	                        memcpy(spActive->dbErrorCode, &usErrorCode, sizeof(u_short));

							spActive->dbRedirectUsable[0] = SKT_FORWARD_SVC_OFF;

							// ported noti off
							nNotiFlag = false;
						}
					}
				}
				
				
				if(spActive->dbDefMsgID[0] >= 1)
				{
					nNotiFlag = false;
				}

				// ported noti make
				if( (nNotiFlag) && (gnPortedNotiFlag) )
				{
				}
				else
				{
				}

				/* Check FTN count */
				if(spActive->dbDefMsgID[0] >= 2)
				{
					spActive->dbRedirectUsable[0] = SKT_FORWARD_SVC_FAIL;
					ucStatus	= MDB_ST_FAIL;
					usErrReason	= ERR_SMS_SKT_FORWARDING_FAIL;
					usErrorCode	= ERR_SMS_SKT_FORWARDING_FAIL;
					memcpy(spActive->dbErrorCode, &usErrorCode, sizeof(u_short));
					memset(spActive->dbDestMsCapabilityFlag, 0x00, DB_DEST_MS_CAPABILITY_SIZE);
				}
				//	CMT, CPT, LONG(CLONG), WPUSH, XMS
				else if( (ls_nResult == 4098) || (ls_nResult == 4097) || (ls_nResult == nLong_TID) ||
					(ls_nResult == nColor_TID) || (ls_nResult == nColLong_TID) || (ls_nResult == 65491) ||
					(ls_nResult == 60003) || (ls_nResult == 60004) || (ls_nResult == 60006) )
				{
					spActive->dbRedirectUsable[0] = SKT_FORWARD_SVC_NEW;
				}
				else
				{
					ucStatus	= MDB_ST_FAIL;
					usErrReason = ERR_SMS_SKT_CFU_UNAVAILABLE;
					usErrorCode = ERR_SMS_SKT_CFU_UNAVAILABLE;
					memcpy(spActive->dbErrorCode, &usErrorCode, sizeof(u_short));

					spActive->dbRedirectUsable[0] = SKT_FORWARD_SVC_OFF;
				}

				//
				//	Calling number begin from 01x-xxx
				//
				if(spActive->dbDestRedirectMIN[0] != 0x01)
				{
					ucStatus	= MDB_ST_FAIL;
					usErrReason = ERR_SMS_SKT_CFU_UNAVAILABLE;
					usErrorCode = ERR_SMS_SKT_CFU_UNAVAILABLE;
					memcpy(spActive->dbErrorCode, &usErrorCode, sizeof(u_short));

					spActive->dbRedirectUsable[0] = SKT_FORWARD_SVC_OFF;
				}

				if(ucStatus == MDB_ST_FAIL)
				{
					if(updateStatus(DBMS_ACTIVE,spPrim->user_dialogue_id, ucStatus, usErrReason)<0)
					{
						return(-1);
					}

					PutMsgId2Uhis(DBMS_ACTIVE,stMapAddr.unMsgId,CHK_2UHIS);
					return(-1);
				}
				
				//
				//	Other Service Check
				//

				/* New SPAM SVC Processing */
				OSFI_CAPA_IN_OUT_CHECK(nCAPA_Copy_Target, spActive->dbDestMsCapability,
						SKT_CAPA_NEW_SPAM, nCAPA_Check);
				if((nCAPA_Check) && (spActive->dbNewSpamSVCStatus[0] == SUB_ST_BASE))
				{
					//TID가 CMT, CPT, COLOR, WPUSH, MMS일때 만 서비스 수행
					if( (ls_nResult == 4098) || (ls_nResult == 4097) || (ls_nResult == nColor_TID) ||
						(ls_nResult == 65491) || (ls_nResult == 63250) || (ls_nResult == 61867) )
					{
						OSFI_CAPA_ENABLE(nCAPA_Copy_Target,spActive->dbDestMsCapabilityFlag,
								SKT_CAPA_NEW_SPAM);
					}
				}
				/* New SPAM SVC Processing End */

				OSFI_CAPA_IN_OUT_CHECK(nCAPA_Copy_Target, spActive->dbDestMsCapability,
										SKT_CAPA_IMSMS, nCAPA_Check);
				if(nCAPA_Check)
				{
					OSFI_CAPA_DISABLE(nCAPA_Copy_Target,spActive->dbDestMsCapability,
										SKT_CAPA_IMSMS);
					OSFI_CAPA_DISABLE(nCAPA_Copy_Target,spActive->dbDestMsCapabilityFlag,
										SKT_CAPA_IMSMS);
				}

				OSFI_CAPA_IN_OUT_CHECK(nCAPA_Copy_Target,spActive->dbDestMsCapability,
                                        SKT_CAPA_SECRETSMS,nCAPA_Check);
				if( (nCAPA_Check) && (spActive->dbScsmssSVCStatus[0] == SUB_ST_BASE) )
                {
                    if( (ls_nResult == 4098) || (ls_nResult == 4097) || (ls_nResult == nLong_TID) ||
                        (ls_nResult == nColor_TID) || (ls_nResult == nColLong_TID))
                    {
                        OSFI_CAPA_ENABLE(nCAPA_Copy_Target,spActive->dbDestMsCapabilityFlag,
                                SKT_CAPA_SECRETSMS);
                    }
                    else
                    {
                        OSFI_CAPA_DISABLE(nCAPA_Copy_Target,spActive->dbDestMsCapability,
                                SKT_CAPA_SECRETSMS);
                        OSFI_CAPA_DISABLE(nCAPA_Copy_Target,spActive->dbDestMsCapabilityFlag,
                                SKT_CAPA_SECRETSMS);
                    }
                }

                OSFI_CAPA_IN_OUT_CHECK(nCAPA_Copy_Target,spActive->dbDestMsCapability,
                                        SKT_CAPA_ICS,nCAPA_Check);
				if( (nCAPA_Check) && (spActive->dbIcsSVCStatus[0] == SUB_ST_BASE) )
                {
                    OSFI_CAPA_ENABLE(nCAPA_Copy_Target,spActive->dbDestMsCapabilityFlag,
                                        SKT_CAPA_ICS);
                }

				OSFI_CAPA_IN_OUT_CHECK(nCAPA_Copy_Target,spActive->dbDestMsCapability,
                                        SKT_CAPA_PICS,nCAPA_Check);
				if( (nCAPA_Check) && (spActive->dbPicsSVCStatus[0] == SUB_ST_BASE) )
                {
                    OSFI_CAPA_ENABLE(nCAPA_Copy_Target,spActive->dbDestMsCapabilityFlag,
                                        SKT_CAPA_PICS);
                }

				/* PERFECT_AA SVC by twkim */
				if(spActive->dbDefMsgID[0] == 0)
				{
					OSFI_CAPA_IN_OUT_CHECK(nCAPA_Copy_Target,spActive->dbDestMsCapability,
							SKT_CAPA_PERFECT_AA, nCAPA_Check);
					if((nCAPA_Check) && (spActive->dbPerfectCAPAStatus[0] == SUB_ST_BASE))
					{
						if(true == isAutoAnsSrcMin(spActive))
						{
							//long 메시지이면 끝 segment만 안심 처리
							if( ls_nResult == nColLong_TID || ls_nResult == nLong_TID || ls_nResult == nColor_TID ||
									(getTranType(spActive->dbMsgTransformCnt) != 0) )
							{
								if(spActive->dbLongTotSegment[0] == spActive->dbLongCurSegment[0])
								{
									OSFI_CAPA_ENABLE(nCAPA_Copy_Target,spActive->dbDestMsCapabilityFlag,
											SKT_CAPA_PERFECT_AA);
								}
							}
							//모든 TID에 대해서 착신안심 서비스 처리
							else
							{
								OSFI_CAPA_ENABLE(nCAPA_Copy_Target,spActive->dbDestMsCapabilityFlag,
										SKT_CAPA_PERFECT_AA);
							}
						}
					}
				}

                //
                //  Check ESME type H/VSMSS, XMSS
                //  VSMSS(0), HSMSS(1), XMSS(2), ETC-VPS/TIIS(3)
                //

                if( (nTypeResult = CheckEsmeType((char *)&spActive->dbEsmeId)) == FLK_FAIL )
                {
                    DebugStr(4, "[%s:%d] Unknown or Mismatched ESMEID.\n", __FILE__, __LINE__);
                }

				if( (nTypeResult == ESME_CLASS_ETC) ||
                        (spActive->dbMsgFlowType[0] == FROM_MS) )
                {
                    //
                    //  Message from MO, ETC ESME
                    //  send delivery_sm to HSMSS
                    //
                    DebugStr(4, "[ADR:%d][EsmeType:%d] FROM_MS or ETC_ESME\n",
							D.extensionContainer.privateExtensionList.adr.data[0], nTypeResult);

                    ucStatus = MDB_ST_SKT_SVC_RETRY;

                    usErrorCode = ERR_SMS_SKT_FORWARDING_ETC;
                    memcpy(spActive->dbErrorCode, &usErrorCode, sizeof(u_short));
                }
                else
                {
                    //
                    //  Message from H/VSMSS, XMSS
                    //  send receipt_msg to H/VSMSS, XMSS
                    //
                    DebugStr(4, "[ADR:%d][EsmeType:%d] FROM H/VSMSS or XMSS\n",
							D.extensionContainer.privateExtensionList.adr.data[0], nTypeResult);

                    {
                        ucStatus = MDB_ST_SKT_SVC_RETRY;
                        usErrorCode = ERR_SMS_SKT_FORWARDING_ESME;
                    }

                    memcpy(spActive->dbErrorCode, &usErrorCode, sizeof(u_short));
                }

				if(updateStatus(DBMS_ACTIVE,spPrim->user_dialogue_id, ucStatus, usErrReason)<0)
                {
                    return(-1);
                }

                if(ucStatus == MDB_ST_SKT_SVC_RETRY)
                {
                    PutMsgId2SchdSndMsg(DBMS_ACTIVE,spPrim->user_dialogue_id,NULL);
                    return(1);
                }
                else if(ucStatus == MDB_ST_FAIL)
                {
                    PutMsgId2Uhis(DBMS_ACTIVE,stMapAddr.unMsgId,CHK_2UHIS);
                    return(-1);
                }

                break;

            default :
				ucStatus    = MDB_ST_FAIL;

                break;
        }

		if(updateStatus(DBMS_ACTIVE,spPrim->user_dialogue_id, ucStatus, usErrReason)<0)
		{
			return(-1);
		}

		if(ucStatus == MDB_ST_SKT_SVC_RETRY)
		{
			PutMsgId2SchdSndMsg(DBMS_ACTIVE, spPrim->user_dialogue_id, NULL);
			return(1);
		}
		else if(ucStatus == MDB_ST_FAIL)
		{
			PutMsgId2Uhis(DBMS_ACTIVE, stMapAddr.unMsgId, CHK_2UHIS);
			return(-1);
		}

	}	//	All Service check finished

	cTmp = V3_setNetworkNode(&D);
	if((cTmp == 0x00) || (cTmp == (u_char)FLK_FAIL)) 
	{
		sprintf(cpBuf,"V3_RoutingInfoForSMI:"
				"setNetworkNode fail. msgid[H'%x]\n",
				(int)stMapAddr.unMsgId);
		LogDebugTrace(2, cpBuf, stMapAddr.cpMin, stMapAddr.nMinLen);

		if(cTmp == 0x00)
		{
			ucStatus	= MDB_ST_RETRY;
			usErrReason	= ERR_PARAM_ERROR;
			updateStatus(DBMS_ACTIVE, stMapAddr.unMsgId, 
					ucStatus, usErrReason);
		}
		else
		{
			ucStatus	= MDB_ST_RETRY;
			usErrReason	= ERR_ISDN_ERROR;
			updateStatus(DBMS_ACTIVE, stMapAddr.unMsgId, 
					ucStatus, usErrReason);
		}

		DebugStr(DG_LEVEL_FIVE,"[%s : %d ]\n", __FILE__, __LINE__);
        MakeStatisticsRecord( &stStsRecord,
                STATISTICS_RECORD_END,
                STATISTICS_TRYINF_NET,
                STATISTICS_TRYNRL_GSM,
                STATISTICS_TRYDIR_OUT_ACK,
                STATISTICS_TRYEVNT_SMQUERY,
                0,
                DeliverTimeInterval(spActive),
                usErrReason,
                0,
                ProcessID,
                0,
                sPc,
				NULL);
        dbStatisticInsertbyRecord(&stStsRecord);
		return(-1);
	}

	/* is CDMA Type */
	if(spActive->dbMtPcNetType[0] == CDMA_MAP_VERSION)
	{
		cTmp = 0x33;
	}

	/* set NetType 2006.12.18 by bjlee */
	spActive->dbGsmNetType[0] = cTmp;

	/* Set Status For MT */
	switch(cTmp)
	{
		case 0x10	:		/* SGSN Only */
			ucStatus	= MDB_ST_FSM_G_S;
			usErrReason	= NULL;
			updateStatus(DBMS_ACTIVE, stMapAddr.unMsgId, 
					ucStatus, usErrReason);
			break;
		case 0x20	:		/* MSC Only	*/
			ucStatus	= MDB_ST_FSM_M_S;
			usErrReason	= NULL;
			updateStatus(DBMS_ACTIVE, stMapAddr.unMsgId, 
					ucStatus, usErrReason);
			break;
		case 0x02	:		/* Two Node but first sgsn translation fail */
			ucStatus	= MDB_ST_FSM_M_S;
			usErrReason	= NULL;
			updateStatus(DBMS_ACTIVE, stMapAddr.unMsgId, 
					ucStatus, usErrReason);
			break;
		case 0x01	:		/* Two Node but first msc translation fail */
			ucStatus	= MDB_ST_FSM_G_S;
			usErrReason	= NULL;
			updateStatus(DBMS_ACTIVE, stMapAddr.unMsgId, 
					ucStatus, usErrReason);
			break;
		case 0x12 	:
		case 0x21	:		/* Two Node */
			switch(stMapParam.nSgsnOrMsc)
			{
				case 1 	: 	/* sgsn first */
					ucStatus	= MDB_ST_FSM_G_M_S;
					usErrReason	= NULL;
					updateStatus(DBMS_ACTIVE, stMapAddr.unMsgId, 
							ucStatus, usErrReason);
					break;
				case 2 	:	/* msc first */
					ucStatus	= MDB_ST_FSM_M_G_S;
					usErrReason	= NULL;
					updateStatus(DBMS_ACTIVE, stMapAddr.unMsgId, 
							ucStatus, usErrReason);
					break;
				default :	/* sgsn first */
					ucStatus	= MDB_ST_FSM_G_M_S;
					usErrReason	= NULL;
					updateStatus(DBMS_ACTIVE, stMapAddr.unMsgId, 
							ucStatus, usErrReason);
					break;
			}
			break;
		case 0x33	:
			ucStatus    = MDB_ST_SMDPP_S;
			usErrReason = NULL;
			updateStatus(DBMS_ACTIVE, stMapAddr.unMsgId,
					ucStatus, usErrReason);
			break;
		default		:
            sprintf(cpBuf, "V3_RoutingInfoForSMI:I don't know this "
					"Routing Type. [%02x],msgid[H'%x]\n",
                    cTmp, (int)stMapAddr.unMsgId);
            LogDebugTrace(2, cpBuf, stMapAddr.cpMin, stMapAddr.nMinLen);

			ucStatus	= MDB_ST_RETRY;
			usErrReason	= ERR_PARAM_ERROR;
			updateStatus(DBMS_ACTIVE, stMapAddr.unMsgId, ucStatus, usErrReason);
			DebugStr(DG_LEVEL_FIVE,"[%s : %d ]\n", __FILE__, __LINE__);
            MakeStatisticsRecord( &stStsRecord,
                    STATISTICS_RECORD_END,
                    STATISTICS_TRYINF_NET,
                    STATISTICS_TRYNRL_GSM,
                    STATISTICS_TRYDIR_OUT_ACK,
                    STATISTICS_TRYEVNT_SMQUERY,
                    0,
                    DeliverTimeInterval(spActive),
                    usErrReason,
                    0,
                    ProcessID,
                    0,
                    sPc,
					NULL);
            dbStatisticInsertbyRecord(&stStsRecord);
			return(-1);
	}

	DebugStr(DG_LEVEL_FIVE,"[%s : %d ]\n", __FILE__, __LINE__);
    MakeStatisticsRecord( &stStsRecord,
            STATISTICS_RECORD_END,
            STATISTICS_TRYINF_NET,
            STATISTICS_TRYNRL_GSM,
            STATISTICS_TRYDIR_OUT_ACK,
            STATISTICS_TRYEVNT_SMQUERY,
            0,
            DeliverTimeInterval(spActive),
            0,
            0,
            ProcessID,
            0,
            sPc,
			NULL);
    dbStatisticInsertbyRecord(&stStsRecord);

	//
	//	Send to SCHD for SKT Service
	//
	if(nIs_DirectSMDPP_Second > 0)
	{
		ucStatus = MDB_ST_SKT_SVC_RETRY;
		usErrReason = 0;

		if(updateStatus(DBMS_ACTIVE,spPrim->user_dialogue_id, ucStatus, usErrReason)<0)
		{
			return(-1);
		}
		DLOG(("TEST BEFORE Put MsgId to Schd..Capa\n"));
        PutMsgId2SchdSndMsg(DBMS_ACTIVE,spPrim->user_dialogue_id,NULL);
        DLOG(("AFTER Put MsgId to Schd..Capa\n"));
        return(1);
	}

	DebugStr(4, "[%s:%d] ucStatus[%02x]\n", __func__, __LINE__, ucStatus);

	//
	//	Send message with own operation
	//
	if(ucStatus == MDB_ST_SMDPP_S)
	{
		SMDPPR(stMapAddr.unMsgId);
		return(1);
	}
	else
	{
		V3_MT_ForwardSMR(stMapAddr.unMsgId, (stMsg_t *)&stMsg);
		return(1);
	}
}

#endif // TCAP_ANSI90


int makePortedNotiSMS(stRDActiveTuple *spActive)
{
    stRDActiveTuple     stActive4Noti;
    u_int       ulNotiMsgId;
    u_short      usTeleSvcId = CMT_SERVICE;
    char                cpOldAddr[DB_DEST_MIN_SIZE*2+1];
    char                cpNewAddr[DB_DEST_MIN_SIZE*2+1];

    extern char         cpPortedNotiSrcMin[DB_SOURCE_MIN_SIZE];
    extern int          nPortedNotiSrcLen;
    extern int          nActiveDBMsgQMaxCnt;
    extern char         cpPortedNotiMsg1[BUF_MSIZE];
    extern char         cpPortedNotiMsg2[BUF_MSIZE];
    extern char         cpPortedNotiMsg3[BUF_MSIZE];

    memset(&stActive4Noti, 0x00, sizeof(stRDActiveTuple));

    if( !(ulNotiMsgId = getMsgId(spMsgID_Q)) )
    {
        return FLK_FAIL;
    }

    BCD2str_min(cpOldAddr, sizeof(cpOldAddr), spActive->dbDestMIN, DB_DEST_MIN_SIZE);
    BCD2str_min(cpNewAddr, sizeof(cpNewAddr), spActive->dbDestRedirectMIN, DB_DESTREDIRECT_MIN_SIZE);

    //  set MsgID, TeleserviceID
    memcpy(stActive4Noti.dbMessageID, &ulNotiMsgId, DB_MESSAGE_ID_SIZE);
    memcpy(stActive4Noti.dbTeleserviceID, &usTeleSvcId, DB_TELESERVICEID_SIZE);

    //  set Source Address
    stActive4Noti.dbSourceTON[0] = ADDR_TON_UNKNOWN;
    stActive4Noti.dbSourceNPI[0] = ADDR_NPI_SPARE1;
    stActive4Noti.dbSourceLen[0] = nPortedNotiSrcLen;
    memcpy(stActive4Noti.dbSourceMIN, cpPortedNotiSrcMin, DB_SOURCE_MIN_SIZE);

    //  set Destination Address
    stActive4Noti.dbDestTON[0] = ADDR_TON_NATIONAL;
    stActive4Noti.dbDestNPI[0] = ADDR_NPI_SPARE1;
	
	if(true == isNPANumber(spActive->dbSourceMIN))
	{
		stActive4Noti.dbDestLen[0] = spActive->dbSourceLen[0];
		memcpy(stActive4Noti.dbDestMIN, spActive->dbSourceMIN, DB_DEST_MIN_SIZE);
	}
	else
	{
    stActive4Noti.dbDestLen[0] = (u_char)convertCB2Min(spActive->dbCallbackNumber,
                                    (char *)stActive4Noti.dbDestMIN);
	}

    //  set Callback Number
    stActive4Noti.dbCallbackLen[0] = (u_char)convertMin2CB((char *)spActive->dbDestRedirectMIN,
                                        spActive->dbDestRedirectLen[0], (char *)stActive4Noti.dbCallbackNumber);

    //  set Submission Time
    time_t  tSubTime = time((time_t *)NULL);
    time2decchar7((char*)stActive4Noti.dbSubmissionTime, tSubTime);

    //  set Expiration Time
    tSubTime += GetDefaultExpiredInterval((int)stActive4Noti.dbRetryFrame[0]);
    time2decchar7((char*)stActive4Noti.dbExpirationTime, tSubTime);

    //  set Data Coding Scheme
    stActive4Noti.dbDataCodingScheme[0] = KSC5601;
    stActive4Noti.dbDataEncoding[0] = KSC5601;

    //  set Status
    stActive4Noti.dbStatus[0] = (u_char)MDB_ST_SUBMITTED;

    //  set Retry Frame
    stActive4Noti.dbRetryFrame[0] = spActive->dbRetryFrame[0];

    //  set Message Flow Type
    SetFlowType(stActive4Noti.dbMsgFlowType[0], FROM_ESME);
    //SetFlowType(stActive4Noti.dbMsgFlowType[0], FROM_SMSC);

    //  init Parameter
    memset(stActive4Noti.dbDestRouteMIN, 0xff, DB_DESTROUTE_MIN_SIZE);
    memset(stActive4Noti.dbDestRedirectMIN, 0xff, DB_DESTREDIRECT_MIN_SIZE);
    memset(stActive4Noti.dbOrgImsi, 0xff, DB_IMSI_SIZE);
    memset(stActive4Noti.dbDestImsi, 0xff, DB_IMSI_SIZE);
	
	u_char ucNewLinePara = 0x0a;	
    //  make Contents
    snprintf( (char *)stActive4Noti.dbMessageContent, DB_MESSAGE_CONTENT_SIZE-1,
            "%s%c%s%s%c%s%s",
            cpPortedNotiMsg1, ucNewLinePara, cpOldAddr, cpPortedNotiMsg2, ucNewLinePara, cpNewAddr, cpPortedNotiMsg3);

    stActive4Noti.dbMessageLength[0] = strlen((char *)stActive4Noti.dbMessageContent);

    if( FLK_FAIL == sendSM2DBMS(&stActive4Noti, nActiveDBMsgQMaxCnt, 10) )
    {
        return FLK_FAIL;
    }

    return FLK_SUCCESS;

}

