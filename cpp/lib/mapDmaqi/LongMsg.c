/**
 @ingroup mapDmaqi
 @{
 */

/***********************************************************************
*
*   Copyright (c) 1998 FEELINGK Information & Communications
*   All rights reserved.
*
*   This program contains confidential and proprietary information of
*   LGIC, and any reproduction, disclosure, or use in whole or in part
*   is expressly  prohibited, except as may be specifically authorized
*   by prior written agreement or permission of LGIC.
*
*               FEELINGK Information & Communications, Ltd.
*						www.feelingk.com
*
***********************************************************************/
/**********************************************************************
	FILE NAME		:	long.c
	PURPOSE			:	to provide Color & long Msg Operations.
	DATE OF WRITE	: 	2002.10.10
	AUTHOR			: 	Full Name (FirstName LastName)
	REVISION		:	V 1.0
	Ver.	By		When		Reason
	------- ------- ----------  -----------------------------------------
	1.0		        2003.04.20	first coding
	1.1		        2003.12.02	expacted color.
***********************************************************************/
#include <libMapDmaqi.h>
#include <MSCapability.h>
#include "Debug.h"

u_short gs_usCoLoTIDs[LONG_TID_MAX];

static const char *l_scpaCapaStr[] = { "Default", "Color Only", "long Only", "Color & long", "Unknown" };

///	Name	: Get Teleserivce ID by Ms Capability.
//	Role	:
//	Input	: ucMsCapa - the Capability of a Destination Mobile station.
//	Return	: u_char*  - TeleserviceID's Pointer
//
u_short getTIDbyCapa(u_char ucMsCapa)
{
	return gs_usCoLoTIDs[((ucMsCapa >= LONG_TID_MAX) ? TID_DEFAULT : ucMsCapa)];
}
///	Name	: Get Color and long Teleserivce IDs.
//	Role	: Load TeleservicesIDs indicate Color and/or long Msg from Smsc.cfg.
//	Input	: None.
//	Return	: None.
//
void getLongTID(void)
{
	//gs_usCoLoTIDs[SEQ_DEFAULT_TID] = configurator::as_int("SMSC", "DEFAULT_TID", TID_DEFAULT);

	//gs_usCoLoTIDs[SEQ_COLOR_TID] = configurator::as_int("SMSC", "COLOR_TID", TID_COLOR_SVC);

	//gs_usCoLoTIDs[SEQ_LONG_TID] = configurator::as_int("SMSC", "LONG_TID", TID_LONG_SVC);

	//gs_usCoLoTIDs[SEQ_COLORLONG_TID] = configurator::as_int("SMSC", "COLORLONG_TID", TID_COLOR_LONG_SVC);
}

///	Name	: Is this TID the One of Color or/and long TeleserviceIDs.
//	Role	: Check what usTeleSvcID is ColorLong TID.
//	Input	: usTeleSvcID - TID of Current Message.
//	Return	: If usTeleSvcID is the one of colorlong Capa(same Index), returns TRUE(1).
//	          Otherwise, FALSE(0) is returned.
//
int isLongTi(u_char *ucpTeleSvcId)
{
	for (int riIdx = SEQ_COLOR_TID; riIdx < LONG_TID_MAX; riIdx++) {
		//if (gs_usCoLoTIDs[riIdx] == usTeleSvcId)// return capability

		if (!memcmp(&gs_usCoLoTIDs[riIdx], ucpTeleSvcId, sizeof(u_short))) {
			// return capability
			return riIdx;
		}
	}

	return false;
}

///	Name	: Convert TermCapa to Text.
//	Role	: Convert a terminal capability of eTermCapa to a Capability String.
//	Input	: ucTermCapa - a Terminal Capability. the One of eTermCapa.
//	Return	: return a string converted tid.
//
const char *CvtCapaToText(eTermCapa ucTermCapa)
{
	if (MS_CAPA_RSV_START <= ucTermCapa) {
		return l_scpaCapaStr[MS_CAPA_RSV_START];
	}

	return l_scpaCapaStr[ucTermCapa];
}

/*
static void SegInfoToChar(u_char *ucpExData, int *ipIdx, const u_char *ucpSrcData)
{
	ucpExData[(*ipIdx)++] = ((*(ucpSrcData + long_SESSION_ID_BYTE_SIZE)
								& long_CURREENT_SEGMENT_BIT_MASK)
							+ START_CONVERT_CHAR);
	ucpExData[(*ipIdx)++] = '/';
	ucpExData[(*ipIdx)++] = ((*(ucpSrcData+long_SESSION_ID_BYTE_SIZE)
								>> long_TOTAL_SEGMENT_BIT_SIZE)
							+ START_CONVERT_CHAR);
	ucpExData[(*ipIdx)++] = ':';
	return;
}
*/
/** @} */
