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
#include "libMapV3FowardSM.h"
#include "libMapV3Extension.h"
#include "libMapUtil.h"
#include "libMap3G_Addr.h"
#include "libMap3G_st.h"
int decV3_ExtensionContainer(stMsgBuf_t *spMsg, V3_ExtensionContainer *pD, u_char cTag);

int V3_TBCDString(u_char *cpIn, u_char *cpOut, int nLen)
{
	int     nIdx;
    u_char 	cpTmp;

    for(nIdx=0; nIdx<nLen; nIdx++)
    {
        cpTmp = cpIn[nIdx];
        cpOut[nIdx] = (cpTmp & 0xF0) >> 4;
        cpOut[nIdx] = cpOut[nIdx] | ((cpTmp & 0x0F) << 4);
    }
    return(1);
}
int decV3_AddressString(stMsgBuf_t *spMsg, V3_AddressString *pD, u_char cTag)
{
	int				nStep = 0;
	int				nLen;
	u_char				cTemp = 0x00;
	u_char				cBuf  = 0x00;
	u_char				cOut[ADDRESS_STRING_LEN];

	if(cTag != spMsg->cpData[nStep++])
    {
        log_write(LEV_ALL_ERR, "decV3_AddressString tag fail [%x]\n", spMsg->cpData[0]);
        return(-1);
    }
    if(decLength(&nLen, &spMsg->cpData[nStep], &nStep) < 0)
        return(-1);

	if((nLen <= 0) || (nLen > ADDRESS_STRING_LEN))
	{
        log_write(LEV_ALL_ERR, "decV3_AddressString length fail [%d]\n", nLen);
        return(-1);
	}

	/* decode Nature Of Address Indicator, Numbering Plan Indicator */	
	cTemp = spMsg->cpData[nStep++];
	cBuf  = (cTemp & 0x70) >> 4;			/* NOA */
	pD->m.NAI = cBuf;
	cBuf  = 0x00;
	cBuf  = cTemp & 0x0F;					/* NP */
	pD->m.NP   = cBuf;

	/* TBCD */
	V3_TBCDString(&spMsg->cpData[nStep], cOut, nLen-1);
	
	
	pD->numocts = nLen - 1;
	memcpy(pD->data, cOut, pD->numocts);

	nStep = nStep + pD->numocts;
	return(nStep);
}
int decV3_ISDN_AddressString(stMsgBuf_t *spMsg, V3_ISDN_AddressString *pD, u_char cTag)
{
    stMsgBuf_t  stMsg;
	int         nStep   = 0;
    int         nLen    = 0;
    int         nlSize  = 0;
	int			nSize	= 0;


	if(cTag != spMsg->cpData[nStep++])
    {
        log_write(LEV_ALL_ERR, "decV3_ISDN_AddressString tag fail [%x]\n", spMsg->cpData[0]);
        return(-1);
    }
	if(decLength(&nLen, &spMsg->cpData[nStep], &nStep) < 0)
        return(-1);
	
	if((nLen < 0) || (nLen > MAX_ISDNADDRESS_LEN))
    {
        log_write(LEV_ALL_ERR, "decV3_ISDN_AddressString length fail [%d]\n", nLen);
        return(-1);
    }else if ( nLen == 0 ) {
	nlSize = spMsg->nLen;
	return nlSize;
	}
	nlSize = spMsg->nLen;
	
	/* Address String */
    memset(&stMsg, 0x00, sizeof(stMsgBuf_t));
	stMsg.nLen = nlSize;
    memcpy(stMsg.cpData, spMsg->cpData, stMsg.nLen);
	nSize = decV3_AddressString(&stMsg, pD, spMsg->cpData[0]);
	if(nSize < 0) return(-1);
	return(nSize);
}
int
decV3_INTEGER(stMsgBuf_t *spMsg, V3_SM_RP_MTI *pD, u_char cTag)
{
	int		nStep 	= 0;
	int		nLen;


	if(cTag != spMsg->cpData[nStep++])
    {
        log_write(LEV_ALL_ERR, "decV3_INTERGER tag fail [%x]\n", spMsg->cpData[0]);
        return(-1);
    }
    if(decLength(&nLen, &spMsg->cpData[nStep], &nStep) < 0)
        return(-1);

	if((nLen > 4) || (nLen < 0))
	{
		log_write(LEV_ALL_ERR, "decV3_INTERGER length fail [%d]\n", nLen);
        return(-1);
	}
		
	memcpy(&pD[sizeof(int)-nLen], &spMsg->cpData[nStep], nLen);

	nStep = nStep + nLen;
	return(nStep); 
}

int decV3_BOOLEAN(stMsgBuf_t *spMsg, V3_BOOLEAN *pD, u_char cTag)
{
	int		nStep 	= 0;
	int		nLen;

	if(cTag != spMsg->cpData[nStep++])
    {
        log_write(LEV_ALL_ERR, "decV3_BOOLEAN tag fail [%x]\n", spMsg->cpData[0]);
        return(-1);
    }
    if(decLength(&nLen, &spMsg->cpData[nStep], &nStep) < 0)
        return(-1);

	if(nLen != 1)
	{
		log_write(LEV_ALL_ERR, "decV3_BOOLEAN length fail [%d]\n", nLen);
        return(-1);
	}
	*pD = spMsg->cpData[nStep];
	nStep = nStep + nLen;
	return(nStep); 
}
int decV3_NULL(stMsgBuf_t *spMsg, u_char *pD, u_char cTag)
{
    int     nStep = 0;
    int     nLen;

	if(cTag != spMsg->cpData[nStep++])
    {
        log_write(LEV_ALL_ERR,"NULL tag fail [%x]\n", spMsg->cpData[0]);
        return(-1);
    }
    if(decLength(&nLen, &spMsg->cpData[nStep], &nStep) < 0)
        return(-1);

    nStep = nStep + nLen;
    return(nStep);
}
int decV3_SM_RP_SMEA(stMsgBuf_t *spMsg, V3_SM_RP_SMEA *pD, u_char cTag)
{
    int     nStep = 0;
    int     nLen;
	u_char		cOut[MAX_SMEA_LEN];

	if(cTag != spMsg->cpData[nStep++])
    {
        log_write(LEV_ALL_ERR, "decV3_SM_RP_SMEA tag fail [H'%x]\n", spMsg->cpData[0]);
        return(-1);
    }
    if(decLength(&nLen, &spMsg->cpData[nStep], &nStep) < 0)
        return(-1);
	
	if((nLen < 1) || (nLen > 12)) 
	{
		log_write(LEV_ALL_ERR, "decV3_SM_RP_SMEA length fail [%d]\n", nLen);
		return(-1);
	}

	pD->numocts = nLen;
	memcpy(pD->data, &spMsg->cpData[nStep], pD->numocts);
	
	nStep = nStep + pD->numocts;
	
    return(nStep);
}
int decV3_RoutingInfoForSM_Arg(stMsgBuf_t *spMsg, V3_RoutingInfoForSM_Arg *pD, u_char cTag)
{
	stMsgBuf_t		stMsg;
	int				nStep 	= 0;
	int				nLen 	= 0;
	int				nSize 	= 0;
	int				nlSize 	= 0;


	if(cTag != spMsg->cpData[nStep++])
	{
		log_write(LEV_ALL_ERR, "decV3_RoutingInfoForSM_Arg tag fail [%x]\n", spMsg->cpData[0]);
		return(-1);
	}

	if(decLength(&nLen, &spMsg->cpData[nStep], &nStep) < 0)
		return(-1);

	/* check total length */
	nlSize = spMsg->nLen - nStep;
	if((nlSize) != nLen)
	{
		log_write(LEV_ALL_ERR, "decV3_RoutingInfoForSM_Arg length fail [%d] = [%d]\n", nlSize, nLen);
	}
	
	/* msisdn */
	memset(&stMsg, 0x00, sizeof(stMsgBuf_t));
	stMsg.nLen 	= nlSize;
	memcpy(stMsg.cpData, &spMsg->cpData[nStep], stMsg.nLen);
	nSize = decV3_ISDN_AddressString(&stMsg, &pD->msisdn, 0x80);
	if(nSize < 0) return(-1);
    nlSize      = nlSize - nSize;
    nStep       = nStep + nSize;

	/* SM RP Priority */
	memset(&stMsg, 0x00, sizeof(stMsgBuf_t));
	stMsg.nLen 	= nlSize;
	memcpy(stMsg.cpData, &spMsg->cpData[nStep], stMsg.nLen);
	nSize = decV3_BOOLEAN(&stMsg, &pD->sm_RP_PRI, 0x81);
	if(nSize < 0) return(-1);
    nlSize      = nlSize - nSize;
    nStep       = nStep + nSize;

	/* Service Centre Address */
	memset(&stMsg, 0x00, sizeof(stMsgBuf_t));
	stMsg.nLen 	= nlSize;
	memcpy(stMsg.cpData, &spMsg->cpData[nStep], stMsg.nLen);
	nSize = decV3_AddressString(&stMsg, &pD->serviceCentreAddress, 0x82);
	if(nSize < 0) return(-1);
    nlSize      = nlSize - nSize;
    nStep       = nStep + nSize;

	/* extension Container (optional) */
    memset(&stMsg, 0x00, sizeof(stMsgBuf_t));
    stMsg.nLen  = nlSize;
    memcpy(stMsg.cpData, &spMsg->cpData[nStep], stMsg.nLen);
    if(stMsg.cpData[0] == 0xA6)
    {
        nSize = decV3_ExtensionContainer(&stMsg, &pD->extensionContainer, 0xA6);
        if(nSize < 0) return(-1);
        nlSize      = nlSize - nSize;
        nStep       = nStep + nSize;
        pD->m.extensionContainerPresent = 1;
    }

	/* GPRS Indicator (optional) */
    memset(&stMsg, 0x00, sizeof(stMsgBuf_t));
    stMsg.nLen  = nlSize;
    memcpy(stMsg.cpData, &spMsg->cpData[nStep], stMsg.nLen);
    if(stMsg.cpData[0] == 0x87)
    {
        nSize = decV3_NULL(&stMsg, (u_char *)&pD->extensionContainer, 0x87);
        if(nSize < 0) return(-1);
        nlSize      = nlSize - nSize;
        nStep       = nStep + nSize;
        pD->m.gprsSupportIndicatorPresent = 1;
    }

	/* sm RP MTI */
    memset(&stMsg, 0x00, sizeof(stMsgBuf_t));
    stMsg.nLen  = nlSize;
    memcpy(stMsg.cpData, &spMsg->cpData[nStep], stMsg.nLen);
    if(stMsg.cpData[0] == 0x88)
    {
        nSize = decV3_INTEGER(&stMsg, &pD->sm_RP_MTI, 0x88);
        if(nSize < 0) return(-1);
        nlSize      = nlSize - nSize;
        nStep       = nStep + nSize;
        pD->m.sm_RP_MTIPresent = 1;
    }

	/* sm RP SMEA */
    memset(&stMsg, 0x00, sizeof(stMsgBuf_t));
    stMsg.nLen  = nlSize;
    memcpy(stMsg.cpData, &spMsg->cpData[nStep], stMsg.nLen);
    if(stMsg.cpData[0] == 0x89)
    {
        nSize = decV3_SM_RP_SMEA(&stMsg, &pD->sm_RP_SMEA, 0x89);
        if(nSize < 0) return(-1);
        nlSize      = nlSize - nSize;
        nStep       = nStep + nSize;
        pD->m.sm_RP_SMEAPresent = 1;
    }

	if(nlSize != 0) 
	{
		log_write(LEV_ALL_ERR, "decV3_RoutingInfoForSM_Arg left length fail [%d]\n", nlSize);
		return(-1);
	}
	
	return(1);
}
