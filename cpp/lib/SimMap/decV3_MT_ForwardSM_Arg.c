
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
#include "libMap3G_st.h"

int decV3_IMSI(stMsgBuf_t *spMsg, V3_IMSI *pD, u_char cTag)
{
	int				nStep	= 0;
	int				nLen;
	unsigned char	cOut[MAX_IMSI_LEN];

	if( (cTag != spMsg->cpData[nStep++]) )
	{
		log_write ( LEV_ALL_PARAM, "decV3_IMSI tag fail [%x]\n", spMsg->cpData[0]);
        return(-1);
	}
	if(decLength(&nLen, &spMsg->cpData[nStep], &nStep) < 0)
		return(-1);

	if((nLen < MIN_IMSI_LEN) || (nLen > MAX_IMSI_LEN))
	{
		log_write ( LEV_ALL_PARAM, "decV3_IMSI length fail [%d]\n", nLen);
		return(-1);
	}
	
	/* TBCD */
	memset(cOut, 0x00, MAX_IMSI_LEN);
	V3_TBCDString(&spMsg->cpData[nStep], cOut, nLen);
	
	/* set IMSI */
	pD->numocts = nLen;
	memcpy(pD->data, cOut, nLen);
	
	nStep = nStep + nLen;
	return(nStep);
}
int decV3_SM_RP_DA(stMsgBuf_t *spMsg, V3_SM_RP_DA *pD, u_char cTag)
{
	int		nSize = 0;
	int		nStep = 0;

	/* Choice Type */
	switch(spMsg->cpData[0])
	{
		case 0x80	:	
			nSize = decV3_IMSI(spMsg, &pD->u.imsi, 0x80);
			if(nSize < 0) return(-1);
			pD->t = V3_SM_RP_DA_imsi;
			break;
		case 0x84	:	/* Service Center DA */
			nSize = decV3_AddressString(spMsg,&pD->u.serviceCentreAddressDA,0x84);
			if(nSize < 0) return(-1);
			pD->t = V3_SM_RP_DA_serviceCentreAddressDA;
			break;
		case 0x85	:	/* no DA */
			return 2;
		case 0x81	:	/* lmsi */
		default		:
			log_write ( LEV_ALL_PARAM, "This is not DA type [%x]\n", spMsg->cpData[0]);
			return(-1);
	}
	return(nSize);
}

int decV3_SM_RP_OA(stMsgBuf_t *spMsg, V3_SM_RP_OA *pD, u_char cTag)
{
	int		nSize = 0;
	int		nStep = 0;

	/* Choice Type */
	switch(spMsg->cpData[0])
	{
		case 0x82	:	/* msisdn */
			nSize = decV3_ISDN_AddressString(spMsg, &pD->u.msisdn, 0x82);
			if(nSize < 0) return(-1);
			pD->t = V3_SM_RP_OA_msisdn;
			break;
		case 0x84	:	/* Service Center OA */
			nSize = decV3_AddressString(spMsg,&pD->u.serviceCentreAddressOA,0x84);
            if(nSize < 0) return(-1);
            pD->t = V3_SM_RP_OA_serviceCentreAddressOA;
            break;
		case 0x85	:	/* no OA */
		default		:
			log_write(LEV_ALL_ERR, "This is not OA type [H'%x]\n", spMsg->cpData[0]);
			return(-1);
	}
	return(nSize);
}
int decV3_SignalInfo(stMsgBuf_t *spMsg, V3_SignalInfo *pD, u_char cTag)
{
	int		nStep = 0;
	int		nLen;
	
    if(cTag != spMsg->cpData[nStep++])
    {
        log_write(LEV_ALL_ERR, "decV3_SignalInfo tag fail [%02x]\n", spMsg->cpData[0]);
        return(-1);
    }
    if(decLength(&nLen, &spMsg->cpData[nStep], &nStep) < 0)
        return(-1);

	if((nLen < 0) || (nLen > MAX_SIGNALINFO_LEN))
    {
        log_write(LEV_ALL_ERR, "decV3_SignalInfo length fail [%d]\n", nLen);
        return(-1);
    }
	
	/* set UI */
	pD->numocts = nLen;
	memcpy(pD->data, &spMsg->cpData[nStep], nLen);

	nStep = nStep + nLen;
	return(nStep);
} 

int decV3_MT_ForwardSM_Arg(stMsgBuf_t *spMsg, V3_MT_ForwardSM_Arg *pD, u_char cTag)
{
	stMsgBuf_t		stMsg;
	int				nStep 	= 0;
	int				nLen 	= 0;
	int				nSize 	= 0;
	int				nlSize 	= 0;

	if(cTag != spMsg->cpData[nStep++])
	{
		log_write ( LEV_ALL_PARAM, "decV3_MT_ForwardSM_Arg tag fail [%x]\n", spMsg->cpData[0]);
		return(-1);
	}

	if(decLength(&nLen, &spMsg->cpData[nStep], &nStep) < 0)
		return(-1);

	nlSize = spMsg->nLen - nStep;
	if((nlSize) != nLen)
	{
		log_write ( LEV_ALL_PARAM, "decV3_MT_ForwardSM_Arg length fail [%d] = [%d]\n"
			, nlSize, nLen);
	}
	
	/* DA */
	memset(&stMsg, 0x00, sizeof(stMsgBuf_t));
	stMsg.nLen 	= nlSize;
	memcpy(stMsg.cpData, &spMsg->cpData[nStep], stMsg.nLen);
	nSize = decV3_SM_RP_DA(&stMsg, &pD->sm_RP_DA, 0x80);
	if(nSize < 0) return(-1);
    nlSize      = nlSize - nSize;
    nStep       = nStep + nSize;

	/* OA */
	memset(&stMsg, 0x00, sizeof(stMsgBuf_t));
	stMsg.nLen 	= nlSize;
	memcpy(stMsg.cpData, &spMsg->cpData[nStep], stMsg.nLen);
	nSize = decV3_SM_RP_OA(&stMsg, &pD->sm_RP_OA, 0x00);
	if(nSize < 0) return(-1);
    nlSize      = nlSize - nSize;
    nStep       = nStep + nSize;

	/* UI */
	memset(&stMsg, 0x00, sizeof(stMsgBuf_t));
	stMsg.nLen 	= nlSize;
	memcpy(stMsg.cpData, &spMsg->cpData[nStep], stMsg.nLen);
	nSize = decV3_SignalInfo(&stMsg, &pD->sm_RP_UI, 0x04);
	if(nSize < 0) return(-1);
    nlSize      = nlSize - nSize;
    nStep       = nStep + nSize;

	/* More Message To Send (optional) */
	memset(&stMsg, 0x00, sizeof(stMsgBuf_t));
	stMsg.nLen 	= nlSize;
	memcpy(stMsg.cpData, &spMsg->cpData[nStep], stMsg.nLen);
	if(stMsg.cpData[0] == 0x05)
	{
		nSize = decV3_NULL(&stMsg, NULL, 0x05);
        if(nSize < 0) return(-1);
        nlSize      = nlSize - nSize;
        nStep       = nStep + nSize;
        pD->m.moreMessageToSendPresent = 1;
	}

	/* extension Container (optional) */
	memset(&stMsg, 0x00, sizeof(stMsgBuf_t));
	stMsg.nLen 	= nlSize;
	memcpy(stMsg.cpData, &spMsg->cpData[nStep], stMsg.nLen);
	if(stMsg.cpData[0] == 0x30)
	{
		nSize = decV3_ExtensionContainer(&stMsg, &pD->extensionContainer, 0x30);
		if(nSize < 0) return(-1);
		nlSize 		= nlSize - nSize;
		nStep		= nStep + nSize;
		pD->m.extensionContainerPresent = 1;
	}

	
	return(1);
}
