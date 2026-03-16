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
#include <stdio.h>
#include "libMapV3FowardSM.h"
#include "libMapV3Extension.h"
#include "libMapUtil.h"
#include "libMap3G_Addr.h"
#include "libMap3G_st.h"

int
encV3_Additional_Number(stMsgBuf_t *spMsg, V3_Additional_Number *pD, u_char cTag)
{
	stMsgBuf_t	stMsg;
	int			nIdx=0;
	V3_AddressString *Addr;
	int nLength;

	memset(&stMsg, 0x00, sizeof(stMsgBuf_t));

	switch(pD->t)
	{
		case V3_Additional_Number_msc_Number		:	/* 0x80 */
			Addr = &pD->u.msc_Number;
			nLength = encV3_RP_AddressString(stMsg.cpData, Addr->m.NAI, Addr->m.NP, 
						Addr->data, Addr->numocts, 0x80 );
			stMsg.nLen = nLength;
			if(nLength < 0)
				return(-1);
			break;
		case V3_Additional_Number_sgsn_Number		: 	/* 0x81 */
			Addr = &pD->u.sgsn_Number;
			nLength = encV3_RP_AddressString(stMsg.cpData, Addr->m.NAI, Addr->m.NP, 
						Addr->data, Addr->numocts, 0x81 );
			stMsg.nLen = nLength;
			if(nLength < 0)
				return(-1);
			break;
		default									:
			log_write(LEV_ALL_WARN, "This is not Additional Number  type [%x]\n", pD->t);
			return(-1);	
	}

	spMsg->cpData[nIdx++] = cTag;
	spMsg->cpData[nIdx++] = stMsg.nLen;
	memcpy(&spMsg->cpData[nIdx], stMsg.cpData, stMsg.nLen);
	spMsg->nLen = stMsg.nLen + nIdx;

	return(1);
}
int encV3_LocationInfoWithLMSI(stMsgBuf_t *spMsg, V3_LocationInfoWithLMSI *pD, u_char cTag)
{
	stMsgBuf_t	stMsg[5];	/* total count parameter */
	int			nIdx = 0;
	int 		nLength = 0;

	memset(&stMsg, 0x00, sizeof(stMsgBuf_t)*5);

	/* Network Node Number */
	nLength = encV3_RP_AddressString(stMsg[nIdx].cpData, 
				pD->networkNode_Number.m.NAI, pD->networkNode_Number.m.NP, 
				pD->networkNode_Number.data, pD->networkNode_Number.numocts,
				0x81);
	stMsg[nIdx++].nLen = nLength;
	if ( nLength < 0 ) 
		return FLK_FAIL;

	/* LMSI (optional) */
	if(pD->m.lmsiPresent)
	{
		if(encV3_Address(&stMsg[nIdx++], pD->lmsi.data, pD->lmsi.numocts, 0x04) < 0)
			return(-1);
	}

	/* extension container (optional) */
	if(pD->m.extensionContainerPresent)
	{
    	if((stMsg[nIdx].nLen = encV3_ExtensionContainer(stMsg[nIdx].cpData,&pD->extensionContainer,0x30))<0)
        	return(-1);
		nIdx++;
	}

	/* gprs Node Indicator (optional) */
	if(pD->m.gprsNodeIndicatorPresent)
	{
		stMsg[nIdx].nLen = 2;
		stMsg[nIdx].cpData[0] = 0x85;
		stMsg[nIdx].cpData[1] = 0;
		nIdx++;;
	}

	/* Additional Number (optional) */
	if(pD->m.additional_NumberPresent)
	{
    	if(encV3_Additional_Number(&stMsg[nIdx++], &pD->additional_Number, 0xA6)<0)
        	return(-1);
	}

	/* Third Number (optional) */
	if(pD->m.third_NumberPresent)
	{
    	if(encV3_Additional_Number(&stMsg[nIdx++], &pD->third_Number, 0xA9)<0)
        	return(-1);
	}

	/* ims Node Indicator (optional) */
	if(pD->m.imsNodeIndicatorPresent)
	{
		stMsg[nIdx].nLen = 2;
		stMsg[nIdx].cpData[0] = 0x8b;
		stMsg[nIdx].cpData[1] = 0;
		nIdx++;;
	}

	if(encTotal(spMsg, stMsg, nIdx, cTag) < 0)
	{
		log_write(LEV_ALL_ERR, "encV3_LocationInfoWithLMSI encV3_Total fail.\n");
		return(-1);
	}
	return(1);
}

int encV3_RoutingInfoForSM_Res(stMsgBuf_t *spMsg, V3_RoutingInfoForSM_Res *pD, u_char cTag)
{
	stMsgBuf_t	stMsg[3];	/* total count parameter */
	int			nIdx = 0;

	memset(stMsg, 0x00, sizeof(stMsgBuf_t)*3);

	/* IMSI */
	if(encV3_Address(&stMsg[nIdx++], pD->imsi.data, pD->imsi.numocts, 0x04) < 0)
		return(-1);

	/* Location Infomation with LMSI */
	if(encV3_LocationInfoWithLMSI(&stMsg[nIdx++], &pD->locationInfoWithLMSI, 0xA0) < 0)
		return(-1);

	/* extension container (optional) */
	if(pD->m.extensionContainerPresent == 1)
	{
    	if( (stMsg[nIdx].nLen = encV3_ExtensionContainer(stMsg[nIdx].cpData,&pD->extensionContainer,0xA4))<0)
        	return(-1);
		nIdx++;
	}

	if(encTotal(spMsg, stMsg, nIdx, cTag) < 0)
	{
		log_write(LEV_ALL_ERR, "encMT_ForwardSM_Arg encV3_Total fail.\n");
		return(-1);
	}

	return(1);
}
/** @} */
