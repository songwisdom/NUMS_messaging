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
    FILE NAME       :  decV3_AnyTimeSubsInterrogation_Arg.c
    PURPOSE         :  DR420 010_PLUS_SVC ATI request date Deconding
    DATE OF WRITE   :  2010.07.01
    AUTHOR          :  peter
    REVISION        :  V 1.0
    Ver.    By      When        Reason
    ------- ------- ----------  -----------------------------------------
    1.0     peter 	2010.07.01  first coding
*******************************************************************************/
#include "libMapV3FowardSM.h"
#include "libMapV3Extension.h"
#include "libMapUtil.h"
#include "libMap3G_Addr.h"
#include "libMap3G_st.h"

int decV3_Length(int *nLen, u_char *cpData, int *npIdx);
int decV3_SubscriptionInfo(stMsgBuf_t *spMsg, V3_SubscriptionInfo*pD, u_char cTag)
{
	stMsgBuf_t	stMsg;
	int			nStep 	= 0;
	int			nLen	= 0;
	int			nlSize	= 0;
	int			nSize	= 0;

#ifdef	DEBUG
	V3_prnData(spMsg);
#endif

	if(cTag != spMsg->cpData[nStep++])
    {
        return(-1);
    }

	if(decV3_Length(&nLen, &spMsg->cpData[nStep], &nStep) < 0)
		return(-1);
	nlSize = spMsg->nLen - nStep;

	/* extension container (optional) */	
	memset(&stMsg, 0x00, sizeof(stMsgBuf_t));
	stMsg.nLen	= nlSize;
	memcpy(stMsg.cpData, &spMsg->cpData[nStep], stMsg.nLen);
	if(stMsg.cpData[0] == 0xA6)
	{
		nSize = decV3_ExtensionContainer(&stMsg, &pD->extensionContainer, 0xA6);
		if(nSize < 0) return(-1);
		nlSize		= nlSize - nSize;
		nStep		= nStep + nSize;
		pD->m.extensionContainerPresent = 1;
	}	

	return(nStep);
}

int
decV3_AnyTimeSubsInterrogation_Arg(stMsgBuf_t *spMsg, V3_AnyTimeSubsInterrogation_Arg *pD, u_char cTag)
{
	stMsgBuf_t		stMsg;
	int				nStep 	= 0;
	int				nLen 	= 0;
	int				nSize 	= 0;
	int				nlSize 	= 0;

#ifdef	DEBUG
	V3_prnData(spMsg);
#endif

	if(cTag != spMsg->cpData[nStep++])
	{
		log_write(LEV_ALL_ERR, "decV3_AnyTimeSubsInterrogation_Arg tag fail [%x]\n", spMsg->cpData[0]);
		return(-1);
	}
	if(decLength(&nLen, &spMsg->cpData[nStep], &nStep) < 0)
		return(-1);

	/* check total length */
	nlSize = spMsg->nLen - nStep;
	if((nlSize) != nLen)
	{
		log_write(LEV_ALL_ERR,"decV3_AnyTimeSubsInterrogation_Arg length fail [%d] = [%d]\n"
			, nlSize, nLen);
	}
	
	nStep += 2;
	/* msisdn */
	memset(&stMsg, 0x00, sizeof(stMsgBuf_t));
	stMsg.nLen 	= nlSize;
	memcpy(stMsg.cpData, &spMsg->cpData[nStep], stMsg.nLen);
	nSize = decV3_ISDN_AddressString(&stMsg, &pD->msisdn, 0x81);
	if(nSize < 0) return(-1);
    nlSize      = nlSize - nSize;
    nStep       = nStep + nSize;

	/* subscriptionInfo */
    memset(&stMsg, 0x00, sizeof(stMsgBuf_t));
    stMsg.nLen  = nlSize;
    memcpy(stMsg.cpData, &spMsg->cpData[nStep], stMsg.nLen);

	nSize = decV3_SubscriptionInfo(&stMsg,&pD->subscriptionInfo,0xa1);
	if(nSize < 0) return(-1);
	nlSize 		= nlSize - nSize;
	nStep 		= nStep + nSize;
#if 0
	/*
    if(stMsg.cpData[0] == 0xA3)
    {
        nSize = decV3_ExtensionContainer(&stMsg, &pD->extensionContainer, 0xA3);
        if(nSize < 0) return(-1);
        nlSize      = nlSize - nSize;
        nStep       = nStep + nSize;
        pD->m.extensionContainerPresent = 1;
    }
	*/

	/* gsmSCF-Address */
	memset(&stMsg, 0x00, sizeof(stMsgBuf_t));
	stMsg.nLen 	= nlSize;
	memcpy(stMsg.cpData, &spMsg->cpData[nStep], stMsg.nLen);
	nSize = decV3_ISDN_AddressString(&stMsg, &pD->msisdn, 0x82);
	if(nSize < 0) return(-1);
    nlSize      = nlSize - nSize;
    nStep       = nStep + nSize;


	if(nlSize != 0) 
	{
		return(-1);
	}
	
#endif
	return(1);
}
