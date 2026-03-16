/**
 * =====================================================================================
 *
 *       @file  V3_ObjectId.c
 *
 *    	 @brief 
 *
 *       Version:  1.0
 *       Compiler:  cc (HP-UX)
 *       @date:  04/23/09 17:50:05
 *
 *       @author:  SMS Dev (), sms_dev@feelingk.com
 *       Company:  (c) FEELingK
 *       @attention
 *       Copyright (c) 2003 FEELINGK Co., Ltd.                                
 *
 *         All rights reserved.                                                
 *                                                                          
 *     This program contains confidential and proprietary information of    
 *     FEELINGK, and any reproduction, disclosure, or use in whole or in part 
 *     by prior written agreement or permission of FEELINGK.                 
 *                                                                          
 *                            FEELINGK Co., Ltd.                           
 *     3F Ace Techno Twin Tower 1, 212-1 Kuro-Dong, Kuro-Gu, Seoul 152-050 Korea 
 *
 * =====================================================================================
 */

#include <stdio.h>
#include <string.h>

#include "libMapV3Extension.h"
#include "libMapUtil.h"
#include "libMapAddrUtil.h"

V3_ObjectIdentifier shortMsgGatewayContext_v3 = {
    8,
    { 0, 4, 0, 0, 1, 0, 20, 3 }
};

V3_ObjectIdentifier shortMsgMO_RelayContext_v3 = {
    8,
    { 0, 4, 0, 0, 1, 0, 21, 3 }
};

V3_ObjectIdentifier shortMsgAlertContext_v2 = {
    8,
    { 0, 4, 0, 0, 1, 0, 23, 2 }
};

V3_ObjectIdentifier shortMsgMT_RelayContext_v3 = {
    8,
    { 0, 4, 0, 0, 1, 0, 25, 3 }
};

V3_ObjectIdentifier AAAAA = {
    4,
    { 1, 2, 840, 113549 }
};


int encV3_OpenType(stMsgBuf_t *spMsg, V3_OpenType *pD, u_char cTag)
{
    int     nStep = 0;
    int     nLen;
	u_char		cOut[OPEN_TYPE_LEN];


	spMsg->cpData[nStep++] = cTag;

	if(encLength(pD->numocts, &(spMsg->cpData[nStep]), &nStep) < 0)
	{
		return(-1);
	}

    /* TBCD */
    memset(cOut, 0x00, OPEN_TYPE_LEN);

	int i;
	for ( i = 0; i < pD->numocts; ++i ) {
		cOut[i] = ((pD->data[i] & 0x0F) << 4) | ((pD->data[i] & 0xF0) >> 4);
	}
	//convBCD2Gsm(cOut, pD->numocts, pD->data, pD->numocts);

	memcpy(&(spMsg->cpData[nStep]), cOut, pD->numocts);
	spMsg->nLen = nStep + pD->numocts;

    if(cTag != 0x00)
		spMsg->cpData[0] = cTag;

	return(1);
}

int toBase(stMsgBuf_t *spMsg, int nBase, u_int nData)
{
	int		nIdx = 0;
	int		nStep = 0;
	u_int	nShare1 = 0;
	u_int	nShare2 = 0;
	u_int	nRest  = 0;


	if(nBase <= 0)
	{
	//	LogRet("toBase fail [%d]\n", nBase); 
		return(-1);
	}

	nShare1 = nData;

	for(nIdx=1; ; nIdx++)
	{
		if(nData < nBase) 
		{
			spMsg->cpData[nStep++] = nData;
			spMsg->nLen 		   = nStep;
			return(1);
		}
		nShare2 = nShare1 / nBase;
		if(nShare2 < nBase)
		{
			nRest = nShare1 % nBase;
			spMsg->cpData[nStep++] = nRest;
			spMsg->cpData[nStep++] = nShare2;
			spMsg->nLen = nStep;
			return(1);
		}
		nRest  =  nShare1 % nBase;
		spMsg->cpData[nStep++] = nRest;
		nShare1 = nShare2;
	}
}

u_int power(int nBase, int nPow)
{
	int		nIdx;
	int		nRet = 1;

	if(nPow == 0) return(1);
	for(nIdx=0; nIdx<nPow; nIdx++)
		nRet = nRet * nBase;
	return(nRet);
}

u_int fromBase(stMsgBuf_t *spMsg, int nBase, u_int *nData)
{
	int		nIdx;
	int		nId;
	int		nTmp = 0;

	for(nIdx=spMsg->nLen-1, nId=0; nIdx>=0 ; nIdx--, nId++)
		nTmp = nTmp + spMsg->cpData[nIdx] * power(nBase, nId);
	
	*nData = nTmp;
	return(1);
}

int
encV3_ObjectIdentifier(stMsgBuf_t *spMsg, V3_ObjectIdentifier *pD, u_char cTag)
{
	int			nIdx = 0;
	int			nDidx = 0;
	int			nStep = 0;
	int			nTemp = 0;
	stMsgBuf_t	stMsg;

	//spMsg->cpData[nDidx++] = (40 * pD->subid[nStep++]) + pD->subid[nStep++];
	spMsg->cpData[nDidx] = 40 * pD->subid[nStep++];
	spMsg->cpData[nDidx] += pD->subid[nStep++];
	nDidx++;


	for(nIdx=0; nIdx<pD->numids-2; nIdx++)
	{
		if(toBase(&stMsg, 128, pD->subid[nStep++]) < 0) return(-1);
		for(nTemp=stMsg.nLen; nTemp > 0; nTemp--)
		{
			if(nTemp == 1) spMsg->cpData[nDidx++] = stMsg.cpData[nTemp-1];
			else spMsg->cpData[nDidx++] = 0x80 | stMsg.cpData[nTemp-1];
		}
	}
	spMsg->nLen = nDidx;
	return(1);
}

int
decV3_ObjectIdentifier(stMsgBuf_t *spMsg, V3_ObjectIdentifier *pD, u_char cTag)
{
	int			nIdx = 0;
	int			nStep = 0;
	u_int		value = 0;
	int			nTemp = 0;
	stMsgBuf_t	stMsg;

	if(((char)spMsg->cpData[0] >= 0) && (spMsg->cpData[0] <= 39))
	{
		pD->subid[nStep++] = 0;
		pD->subid[nStep++] = spMsg->cpData[0];
	}
	else if((spMsg->cpData[0] >= 40) && (spMsg->cpData[0] <= 79))
	{
		pD->subid[nStep++] = 1;
		pD->subid[nStep++] = spMsg->cpData[0] - 40;
	}
	else if((spMsg->cpData[0] >= 80) && (spMsg->cpData[0] <= 159))
	{
		pD->subid[nStep++] = 2;
		pD->subid[nStep++] = spMsg->cpData[0] - 80;
	}
	else 
	{
		//LogRet("decObjectIdentifier fail [%x]\n", spMsg->cpData[0]);
		return(-1);
	}

	for(nIdx=1; nIdx<spMsg->nLen; nIdx++)
	{
		if(!(spMsg->cpData[nIdx] & 0x80))
		{
			stMsg.cpData[nTemp++] = spMsg->cpData[nIdx];
			stMsg.nLen = nTemp;
			if((int)fromBase(&stMsg, 128, &value) < 0) return(-1);
			pD->subid[nStep++] = value;
			nTemp = 0;
			continue;
		}
		stMsg.cpData[nTemp++] = spMsg->cpData[nIdx] & 0x7f;
	}
	pD->numids = nStep;
	return(1);
}

int
decV3_ObjectId(stMsgBuf_t *spMsg, V3_ObjectIdentifier *pD, u_char cTag)
{
	stMsgBuf_t	stMsg;
	int     	nStep 	= 0;
    int     	nLen	= 0;
	int			nlSize	= 0;
	int			nSize	= 0;
    u_char     	cOut[OPEN_TYPE_LEN];

	if(cTag != spMsg->cpData[nStep++])
    {
        //LogRet("decV3_ObjectId tag fail [H'%x]\n", spMsg->cpData[0]);
        return(-1);
    }
    if(decLength(&nLen, &spMsg->cpData[nStep], &nStep) < 0)
        return(-1);

	memset(&stMsg, 0x00, sizeof(stMsgBuf_t));
	stMsg.nLen = nLen;
	memcpy(stMsg.cpData, &spMsg->cpData[nStep], stMsg.nLen);
	if(decV3_ObjectIdentifier(&stMsg, pD, cTag) < 0)
		return(-1);

	nStep = nStep + pD->numids;
	return(nStep);
}

int
encV3_ObjectId(stMsgBuf_t *spMsg, V3_ObjectIdentifier *pD, u_char cTag)
{
	stMsgBuf_t  stMsg;
    int         nStep   = 0;
    int         nLen    = 0;

    memset(&stMsg, 0x00, sizeof(stMsgBuf_t));
    if(encV3_ObjectIdentifier(&stMsg, pD, cTag) < 0)
        return(-1);

	spMsg->cpData[nStep++] = cTag;

	if(encLength(stMsg.nLen, &spMsg->cpData[nStep], &nStep) < 0)
		return(-1);
	memcpy(&spMsg->cpData[nStep], stMsg.cpData, stMsg.nLen);
	spMsg->nLen = stMsg.nLen + nStep;

	return(1);
}

