/**
 @ingroup mapDmaqi
 @{
 */
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

/***********************************************************************
  FILE NAME		:	mdbDmaqiDebug.c
PURPOSE			:	Debug for dmaqi operation.
DATE OF WRITE	: 	1998.05
AUTHOR			: 	Lee Mi Oak.
REVISION		:	V 1.0
Ver.	By		When		Reason
------- ------- ----------  -----------------------------------------
1.0		molee	1998.05		first coding
 ***********************************************************************/

#include <stdio.h>
#include <libMapDmaqi.h>
#include <MdbCommon.h>
#include <MdbMessage.h>
#include <MdbTableInfo.h>



#include <Debug.h>


/*---------------------------------------------------------------------
 *  FUNCTION NAME   : debugActiveRecord()
 *  PURPOSE         : debug active table.
 *  INPUT ARGUMENTS :
 *						active table struct *.
 *						refer mdbTableInfo.h.
 *  RETURN VALUE    :
 *  CALLED FROM     :
 *  USED GLOBAL VARIABLES :
 *  REVISION        :
 *      BY              WHEN        REASON
 *      --------------  ----------  -------------------
 *      molee           98.05.      First Coding
 *      J.D Shim        03.07.30    Delete Function.
 ----------------------------------------------------------------------*/
void debugActiveRecord(stRDActiveTuple *spActive)
{
#if 0
	int nLoop;
	u_int			unMesgId = 0;
	u_int			unMesgLen = 0;
	u_char	ucTemp[sizeof(u_int)] = {0x0, };

	//DebugStr(4, "=============================================\n");
	//DebugStr(4, "\t\t\tActive Record\n");

	//DebugStr(4, "%20s", "MessageId:");
	for (nLoop = 0; nLoop < sizeof(spActive->dbMessageID) ; nLoop++) {
		DebugStr(4, "[%02X]", spActive->dbMessageID[nLoop]);
	}

	memcpy(&unMesgId, spActive->dbMessageID, DB_MESSAGE_ID_SIZE);

	DebugStr(4, ",[%u]\n", unMesgId);

	DebugStr(4, "%20s", "ServiceType:");
	for (nLoop = 0; nLoop < sizeof(spActive->dbServiceType) ; nLoop++) {
		DebugStr(4, "[%02X]", spActive->dbServiceType[nLoop]);
	}
	DebugStr(4, "\n");
	DebugStr(4, "%20s", "MessageType:");
	for (nLoop = 0; nLoop < sizeof(spActive->dbMessageType) ; nLoop++) {
		DebugStr(4, "[%02X]", spActive->dbMessageType[nLoop]);
	}
	DebugStr(4, "\n");
	DebugStr(4, "%20s", "TeleserviceId:");
	for (nLoop = 0; nLoop < sizeof(spActive->dbTeleserviceID) ; nLoop++) {
		DebugStr(4, "[%02X]", spActive->dbTeleserviceID[nLoop]);
	}
	DebugStr(4, "\n");

	DebugStr(4, "%20s", "DefMsgID:");
	for (nLoop = 0; nLoop < sizeof(spActive->dbDefMsgID) ; nLoop++) {
		DebugStr(4, "[%02X]", spActive->dbDefMsgID[nLoop]);
	}

	DebugStr(4, "\n");

	DebugStr(4, "%20s", "EsmeServiceType:");
	for (nLoop = 0; nLoop < sizeof(spActive->dbEsmeServiceType) ; nLoop++) {
		DebugStr(4, "[%02X]", spActive->dbEsmeServiceType[nLoop]);
	}
	DebugStr(4, "\n");
	DebugStr(4, "%20s", "dbMsgFlowType:");
	for (nLoop = 0; nLoop < sizeof(spActive->dbMsgFlowType) ; nLoop++) {
		DebugStr(4, "[%02X]", spActive->dbMsgFlowType[nLoop]);
	}
	DebugStr(4, "\n");

	DebugStr(4, "%20s", "SourceTON:");
	for (nLoop = 0; nLoop < sizeof(spActive->dbSourceTON) ; nLoop++) {
		DebugStr(4, "[%02X]", spActive->dbSourceTON[nLoop]);
	}
	DebugStr(4, "\n");

	DebugStr(4, "%20s", "SourceNPI:");
	for (nLoop = 0; nLoop < sizeof(spActive->dbSourceNPI) ; nLoop++) {
		DebugStr(4, "[%02X]", spActive->dbSourceNPI[nLoop]);
	}
	DebugStr(4, "\n");
	DebugStr(4, "%20s", "SourceMIN:");
	for (nLoop = 0; nLoop < sizeof(spActive->dbSourceMIN) ; nLoop++) {
		DebugStr(4, "[%02X]", spActive->dbSourceMIN[nLoop]);
	}
	DebugStr(4, "\n");

	DebugStr(4, "%20s", "DestTON:");
	for (nLoop = 0; nLoop < sizeof(spActive->dbDestTON) ; nLoop++) {
		DebugStr(4, "[%02X]", spActive->dbDestTON[nLoop]);
	}
	DebugStr(4, "\n");

	DebugStr(4, "%20s", "DestNPI:");
	for (nLoop = 0; nLoop < sizeof(spActive->dbDestNPI) ; nLoop++) {
		DebugStr(4, "[%02X]", spActive->dbDestNPI[nLoop]);
	}
	DebugStr(4, "\n");
	DebugStr(4, "%20s", "DestMIN:");
	for (nLoop = 0; nLoop < sizeof(spActive->dbDestMIN) ; nLoop++) {
		DebugStr(4, "[%02X]", spActive->dbDestMIN[nLoop]);
	}
	DebugStr(4, "\n");

	DebugStr(4, "%20s", "SubmissionTime:");
	for (nLoop = 0; nLoop < sizeof(spActive->dbSubmissionTime) ; nLoop++) {
		DebugStr(4, "[%02X]", spActive->dbSubmissionTime[nLoop]);
	}
	DebugStr(4, "\n");
	DebugStr(4, "%20s", "ReservationTime:");
	for (nLoop = 0; nLoop < sizeof(spActive->dbReservationTime) ; nLoop++) {
		DebugStr(4, "[%02X]", spActive->dbReservationTime[nLoop]);
	}
	DebugStr(4, "\n");

	DebugStr(4, "%20s", "ScheduledTime:");
	for (nLoop = 0; nLoop < sizeof(spActive->dbScheduledTime) ; nLoop++) {
		DebugStr(4, "[%02X]", spActive->dbScheduledTime[nLoop]);
	}
	DebugStr(4, "\n");
	DebugStr(4, "%20s", "ExpirationTime:");
	for (nLoop = 0; nLoop < sizeof(spActive->dbExpirationTime) ; nLoop++) {
		DebugStr(4, "[%02X]", spActive->dbExpirationTime[nLoop]);
	}
	DebugStr(4, "\n");
	DebugStr(4, "%20s", "DeliveryTime:");
	for (nLoop = 0; nLoop < sizeof(spActive->dbDeliveryTime) ; nLoop++) {
		DebugStr(4, "[%02X]", spActive->dbDeliveryTime[nLoop]);
	}
	DebugStr(4, "\n");
	DebugStr(4, "%20s", "InvokeTime:");
	for (nLoop = 0; nLoop < sizeof(spActive->dbInvokeTime) ; nLoop++) {
		DebugStr(4, "[%02X]", spActive->dbInvokeTime[nLoop]);
	}
	DebugStr(4, "\n");

	DebugStr(4, "%20s", "Privacy:");
	for (nLoop = 0; nLoop < sizeof(spActive->dbPrivacyFlag) ; nLoop++) {
		DebugStr(4, "[%02X]", spActive->dbPrivacyFlag[nLoop]);
	}
	DebugStr(4, "\n");
	DebugStr(4, "%20s", "Language:");
	for (nLoop = 0; nLoop < sizeof(spActive->dbLanguageFlag) ; nLoop++) {
		DebugStr(4, "[%02X]", spActive->dbLanguageFlag[nLoop]);
	}
	DebugStr(4, "\n");

	DebugStr(4, "%20s", "DestEsn:");
	for (nLoop = 0; nLoop < sizeof(spActive->dbDestEsn) ; nLoop++) {
		DebugStr(4, "[%02X]", spActive->dbDestEsn[nLoop]);
	}
	DebugStr(4, "\n");
	DebugStr(4, "%20s", "OrgEsn:");
	for (nLoop = 0; nLoop < sizeof(spActive->dbOrgEsn) ; nLoop++) {
		DebugStr(4, "[%02X]", spActive->dbOrgEsn[nLoop]);
	}
	DebugStr(4, "\n");

	// DR720 pkg by cbh : dbDataEncoding -> dbDataCodingScheme
	DebugStr(4, "%20s", "DataCodingScheme:");
	for (nLoop = 0; nLoop < sizeof(spActive->dbDataCodingScheme) ; nLoop++) {
		DebugStr(4, "[%02X]", spActive->dbDataCodingScheme[nLoop]);
	}
	DebugStr(4, "\n");
	DebugStr(4, "%20s", "PriorityFlag:");
	for (nLoop = 0; nLoop < sizeof(spActive->dbPriorityFlag) ; nLoop++) {
		DebugStr(4, "[%02X]", spActive->dbPriorityFlag[nLoop]);
	}
	DebugStr(4, "\n");
	DebugStr(4, "%20s", "EsmClass:");
	for (nLoop = 0; nLoop < sizeof(spActive->dbEsmClass) ; nLoop++) {
		DebugStr(4, "[%02X]", spActive->dbEsmClass[nLoop]);
	}
	DebugStr(4, "\n");

	DebugStr(4, "%20s", "RegisteredDeliveryFlag:");
	for (nLoop = 0; nLoop < sizeof(spActive->dbRegisteredDeliveryFlag) ; nLoop++) {
		DebugStr(4, "[%02X]", spActive->dbRegisteredDeliveryFlag[nLoop]);
	}
	DebugStr(4, "\n");

	DebugStr(4, "%20s", "Status:");
	for (nLoop = 0; nLoop < sizeof(spActive->dbStatus) ; nLoop++) {
		DebugStr(4, "[%02X]", spActive->dbStatus[nLoop]);
	}
	DebugStr(4, "\n");
	DebugStr(4, "%20s", "FailReason:");
	for (nLoop = 0; nLoop < sizeof(spActive->dbFailReason) ; nLoop++) {
		DebugStr(4, "[%02X]", spActive->dbFailReason[nLoop]);
	}
	DebugStr(4, "\n");
	DebugStr(4, "%20s", "RetryFrame:");
	for (nLoop = 0; nLoop < sizeof(spActive->dbRetryFrame) ; nLoop++) {
		DebugStr(4, "[%02X]", spActive->dbRetryFrame[nLoop]);
	}
	DebugStr(4, "\n");
	DebugStr(4, "%20s", "RetryFlag:");
	for (nLoop = 0; nLoop < sizeof(spActive->dbRetryFlag) ; nLoop++) {
		DebugStr(4, "[%02X]", spActive->dbRetryFlag[nLoop]);
	}
	DebugStr(4, "\n");
	DebugStr(4, "%20s", "ErrorCode:");
	for (nLoop = 0; nLoop < sizeof(spActive->dbErrorCode) ; nLoop++) {
		DebugStr(4, "[%02X]", spActive->dbErrorCode[nLoop]);
	}
	DebugStr(4, "\n");
	DebugStr(4, "%20s", "RetryCount:");
	for (nLoop = 0; nLoop < sizeof(spActive->dbRetryCount) ; nLoop++) {
		DebugStr(4, "[%02X]", spActive->dbRetryCount[nLoop]);
	}
	DebugStr(4, "\n");
	DebugStr(4, "%20s", "ReservationFlag:");
	for (nLoop = 0; nLoop < sizeof(spActive->dbReservationFlag) ; nLoop++) {
		DebugStr(4, "[%02X]", spActive->dbReservationFlag[nLoop]);
	}
	DebugStr(4, "\n");
	DebugStr(4, "%20s", "UpdateFlag:");
	for (nLoop = 0; nLoop < sizeof(spActive->dbUpdateFlag) ; nLoop++) {
		DebugStr(4, "[%02X]", spActive->dbUpdateFlag[nLoop]);
	}
	DebugStr(4, "\n");
	DebugStr(4, "%20s", "FowardLink:");
	for (nLoop = 0; nLoop < sizeof(spActive->dbForwardLink) ; nLoop++) {
		DebugStr(4, "[%02X]", spActive->dbForwardLink[nLoop]);
	}
	DebugStr(4, "\n");
	DebugStr(4, "%20s", "BackwardLink:");
	for (nLoop = 0; nLoop < sizeof(spActive->dbBackwardLink) ; nLoop++) {
		DebugStr(4, "[%02X]", spActive->dbBackwardLink[nLoop]);
	}
	DebugStr(4, "\n");
	DebugStr(4, "%20s", "Mask status::");
	for (nLoop = 0; nLoop < sizeof(spActive->dbMaskStatus) ; nLoop++) {
		DebugStr(4, "[%02X]", spActive->dbMaskStatus[nLoop]);
	}
	DebugStr(4, "\n");

	DebugStr(4, "%20s", "VmnMsgCount:");
	DebugStr(4, "[");
	for (nLoop = 0; nLoop < sizeof(spActive->dbVmnMsgCount) ; nLoop++) {
		DebugStr(4, "[%02X]", spActive->dbVmnMsgCount[nLoop]);
	}
	DebugStr(4, "\n");

	DebugStr(4, "%20s", "MessageLength:");
	for (nLoop = 0; nLoop < sizeof(spActive->dbMessageLength) ; nLoop++) {
		DebugStr(4, "[%02X]", spActive->dbMessageLength[nLoop]);
	}
	memset(ucTemp, 0x00, sizeof(ucTemp));
	memcpy(&ucTemp[sizeof(u_int) - DB_MESSAGE_LENGTH_SIZE], spActive->dbMessageLength, DB_MESSAGE_LENGTH_SIZE);
	memcpy(&unMesgLen, ucTemp, sizeof(u_int));
	DebugStr(4, ",[%u]\n", unMesgLen);

	DebugStr(4, "%20s", "MessageContent:");
	for (nLoop = 0; nLoop < (unMesgLen) ; nLoop++) {
		DebugStr(4, "[%02X]", spActive->dbMessageContent[nLoop]);
	}
	DebugStr(4, "\n");
	DebugStr(4, "=============================================\n");
#endif
}
/*---------------------------------------------------------------------
 *  FUNCTION NAME   : debugPrefixRecord()
 *  PURPOSE         : debug prefix table.
 *  INPUT ARGUMENTS :
 *                      prefix table struct *.
 *                      refer mdbTableInfo.h.
 *  RETURN VALUE    :
 *  CALLED FROM     :
 *  USED GLOBAL VARIABLES :
 *  REVISION        :
 *      BY              WHEN        REASON
 *      --------------  ----------  -------------------
 *      molee           98.05.      First Coding
 ----------------------------------------------------------------------*/
void debugPrefixRecord(stRDPrefixTuple *spPrefix)
{
#if 0
	int    	nLoop;
	u_int	ulPrefixLen = 0;
	u_char		ucTemp[4] = {0x0, };


	DebugStr(4, "=============================================\n");
	DebugStr(4, "Prefix Record\n");
	DebugStr(4, "%20s", "Prefix:");
	for (nLoop = 0; nLoop < sizeof(spPrefix->dbPrefix) ; nLoop++) {
		DebugStr(4, "[%02X]", spPrefix->dbPrefix[nLoop]);
	}
	DebugStr(4, "\n");

	DebugStr(4, "%20s", "Prefix Length:");
	for (nLoop = 0; nLoop < sizeof(spPrefix->dbPrefixLength) ; nLoop++) {
		DebugStr(4, "[%02X]", spPrefix->dbPrefixLength[nLoop]);
	}

	memcpy(&ucTemp[sizeof(u_int) - DB_PREFIX_LENGTH_SIZE], &spPrefix->dbPrefixLength, DB_PREFIX_LENGTH_SIZE);
	memcpy(&ulPrefixLen, ucTemp, sizeof(u_int));
	DebugStr(4,	", [%u] ", ulPrefixLen);

	DebugStr(4, "\n");

	DebugStr(4, "%20s", "Address:");
	for (nLoop = 0; nLoop < sizeof(spPrefix->dbAddress) ; nLoop++) {
		DebugStr(4, "[%02X]", spPrefix->dbAddress[nLoop]);
	}
	DebugStr(4, "\n");

	DebugStr(4, "%20s", "AddressType:");
	for (nLoop = 0; nLoop < sizeof(spPrefix->dbAddrType) ; nLoop++) {
		DebugStr(4, "[%02X]", spPrefix->dbAddrType[nLoop]);
	}
	DebugStr(4, "\n");
	DebugStr(4, "=============================================\n");
#endif
}
void debugEsmeRecord(stRDEsmeTuple *spEsme)
{
#if 0
	int	nLoop = 0;

	DebugStr(4, "%20s", "Esme:");
	for (nLoop = 0; nLoop < sizeof(spEsme->dbEsme) ; nLoop++) {
		DebugStr(4, "[%02X]", spEsme->dbEsme[nLoop]);
	}
	DebugStr(4, "\n");
	DebugStr(4, "%20s", "Frame:");
	for (nLoop = 0; nLoop < sizeof(spEsme->dbFrame) ; nLoop++) {
		DebugStr(4, "[%02X]", spEsme->dbFrame[nLoop]);
	}
	DebugStr(4, "\n");
	DebugStr(4, "%20s", "TeleSvcId:");
	for (nLoop = 0; nLoop < sizeof(spEsme->dbTeleId) ; nLoop++) {
		DebugStr(4, "[%02X]", spEsme->dbTeleId[nLoop]);
	}
	DebugStr(4, "\n");

	DebugStr(4, "%20s", "EsmeType:");
	for (nLoop = 0; nLoop < sizeof(spEsme->dbSrcServiceType) ; nLoop++) {
		DebugStr(4, "[%02X]", spEsme->dbSrcServiceType[nLoop]);
	}
	DebugStr(4, "\n");

	DebugStr(4, "%20s", "Description:");
	for (nLoop = 0; nLoop < sizeof(spEsme->dbDescription) ; nLoop++) {
		DebugStr(4, "[%02X]", spEsme->dbDescription[nLoop]);
	}
	DebugStr(4, "\n");
#endif
}

/** @} */
