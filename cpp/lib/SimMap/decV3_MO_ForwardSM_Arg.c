/**
 @ingroup v3_gsm_map
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
//#include "V3_Gsm_Map.h"
#include "libMap3G_st.h"

int decV3_Length(int *nLen, u_char *cpData, int *npIdx);
int decV3_SM_RP_OA(stMsgBuf_t *spMsg, V3_SM_RP_OA *pD, u_char cTag);
int decV3_SM_RP_DA(stMsgBuf_t *spMsg, V3_SM_RP_DA *pD, u_char cTag);
int decV3_SignalInfo(stMsgBuf_t *spMsg, V3_SignalInfo *pD, u_char cTag);
int decV3_IMSI(stMsgBuf_t *spMsg, V3_IMSI *pD, u_char cTag);
int decV3_MO_ForwardSM_Arg2(stMsgBuf_t *spMsg, V3_MO_ForwardSM_Arg2 *pD, u_char cTag)
{
	stMsgBuf_t		stMsg;
	int				nStep 	= 0;
	int				nLen 	= 0;
	int				nSize 	= 0;
	int				nlSize 	= 0;

#ifdef	DEBUG
	V3_prnData(spMsg);
#endif

	if (cTag != spMsg->cpData[nStep++]) {
		return (-1);
	}
	if (decV3_Length(&nLen, &spMsg->cpData[nStep], &nStep) < 0) {
		return (-1);
	}

	/* check total length */
	nlSize = spMsg->nLen - nStep;
	if ((nlSize) != nLen) {
	}

	/* DA */
	memset(&stMsg, 0x00, sizeof(stMsgBuf_t));
	stMsg.nLen 	= nlSize;
	memcpy(stMsg.cpData, &spMsg->cpData[nStep], stMsg.nLen);
	nSize = decV3_SM_RP_DA(&stMsg, &pD->sm_RP_DA, 0x00);
	if (nSize < 0) { return (-1); }
	nlSize      = nlSize - nSize;
	nStep       = nStep + nSize;
	if ( nlSize < 0 ) {
		return -1;
	}

	/* OA */
	memset(&stMsg, 0x00, sizeof(stMsgBuf_t));
	stMsg.nLen 	= nlSize;
	memcpy(stMsg.cpData, &spMsg->cpData[nStep], stMsg.nLen);
	nSize = decV3_SM_RP_OA(&stMsg, &pD->sm_RP_OA, 0x00);
	// FR220 : 발신번이 없는 경우 NACK 처리
	if (nSize <= 3) {
		return (-1);
	}

	nlSize      = nlSize - nSize;
	nStep       = nStep + nSize;
	if ( nlSize < 0 ) {
		return -1;
	}

	/* UI */
	memset(&stMsg, 0x00, sizeof(stMsgBuf_t));
	stMsg.nLen 	= nlSize;
	memcpy(stMsg.cpData, &spMsg->cpData[nStep], stMsg.nLen);
	nSize = decV3_SignalInfo(&stMsg, &pD->sm_RP_UI, 0x04);
	if (nSize < 0) { return (-1); }
	nlSize      = nlSize - nSize;
	nStep       = nStep + nSize;
	if ( nlSize < 0 ) {
		return -1;
	}

	/* extension Container (optional) */
	memset(&stMsg, 0x00, sizeof(stMsgBuf_t));
	stMsg.nLen 	= nlSize;
	memcpy(stMsg.cpData, &spMsg->cpData[nStep], stMsg.nLen);

	if (stMsg.cpData[0] == 0x30) {
		nSize = decV3_ExtensionContainer(&stMsg, &pD->extensionContainer, 0x30);
		if (nSize < 0) { return (-1); }
		nlSize 		= nlSize - nSize;
		nStep		= nStep + nSize;
		pD->m.extensionContainerPresent = 1;
		if ( nlSize < 0 ) {
			return -1;
		}
	}

	/* imsi (optional) */
	memset(&stMsg, 0x00, sizeof(stMsgBuf_t));
	stMsg.nLen 	= nlSize;
	memcpy(stMsg.cpData, &spMsg->cpData[nStep], stMsg.nLen);

	if (stMsg.cpData[0] == 0x04) {
		nSize = decV3_IMSI(&stMsg, &pD->imsi, 0x04);
		if (nSize < 0) { return (-1); }
		nlSize 		= nlSize - nSize;
		nStep		= nStep + nSize;
		pD->m.imsiPresent = 1;
	}

	if (nlSize != 0) {
		return (-1);
	}

	return (1);
}
/** @} */
