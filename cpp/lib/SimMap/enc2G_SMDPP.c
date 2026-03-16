
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
#include "libMapComm.h"
#include "libMap2G.h"
#include "libMap2G_Addr.h"

int encSMDPP(stMsgBuf_t *spMsg, stSMDPP_t *pD, int nTag)
{
	stMsgBuf_t		stMsg[9];
	int				nIdx = 0;

	memset(&stMsg, 0x00, sizeof(stMsg));

	/* SMS Bearer Data */
	if(encOctetString(&stMsg[nIdx++], &pD->smsbd, SMS_BEARERDATA_TAG) < 0) return(-1);

	/* SMS Teleservice Id */
	if(encOctetString(&stMsg[nIdx++], &pD->smsti, SMS_TELESERVICE_IND_TAG) < 0) 
		return(-1);

	/* ESN ( O )*/
	if(pD->m.esnPresent)
	{
		if(encOctetString(&stMsg[nIdx++], &pD->esn, ESN_TAG) < 0) return(-1);
	}

	 /* MIN (O) */ 
	if(pD->m.minPresent == 1)
	{
		if(encOctetString(&stMsg[nIdx++], &pD->min, MIN_TAG) < 0) return(-1);
	}

	/* IMSI (0) */
	if(pD->m.smsiPresent == 1)
	{
		if(encOctetString(&stMsg[nIdx++], &pD->smsi, SMS_IMSI_TAG) < 0) return(-1);
	}

	/* SMS Noti Indicator (optional) */
    if(pD->m.smsniPresent == 1)
    {
        if(encOctetString(&stMsg[nIdx++], &pD->smsni, SMS_NOTI_IND_TAG) < 0) 
            return(-1);
    }
	
	/* SMS Charge Indicator (optional) */
    if(pD->m.smscindPresent == 1)
    {
        if(encOctetString(&stMsg[nIdx++], &pD->smscind, SMS_CHARGE_IND_TAG) < 0) 
            return(-1);
    }

	/* Original Originating Address (optional) */
    if(pD->m.smsooaPresent == 1)
    {
        if(encOctetString(&stMsg[nIdx++], &pD->smsooa, SMS_ORG_ORG_ADDR_TAG) < 0) 
            return(-1);
    }

	/* Original Destinating Address */
    if(pD->m.smsodaPresent == 1)
    {
        if(encOctetString(&stMsg[nIdx++], &pD->smsoda, SMS_ORG_DEST_ADDR_TAG) < 0) return(-1);
    }

	/* Destinationg Address */
	if(pD->m.smsdaPresent == 1)
	{
		if(encOctetString(&stMsg[nIdx++], &pD->smsda, SMS_DEST_ADDR_TAG) < 0)
			return(-1);
	}

	/* OSFI */
	if(pD->m.osfiPresent == 1)
	{
		if(encOctetString(&stMsg[nIdx++], &pD->osfi, SMS_SKT_OSFI_TAG) < 0)
			return(-1);
	}

	/* ROAM INDICATOR */
	if(pD->m.roamindicatPresent == 1)
	{
		if(encOctetString(&stMsg[nIdx++], &pD->smsroamindicat, SMS_SKT_ROAMINDICAT_TAG) < 0)
			return(-1);
	}

	/* ROAMING PMN */
	if(pD->m.roampmnPresent == 1)
	{
		if(encOctetString(&stMsg[nIdx++], &pD->smsroampmn, SMS_SKT_ROAMING_PMN_TAG) < 0)
			return(-1);
	}

	return encTotal(spMsg, stMsg, nIdx, nTag);
}
