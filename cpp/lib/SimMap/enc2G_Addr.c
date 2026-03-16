/**
 * =====================================================================================
 *
 *       @file  enc2G_Addr.c
 *
 *    	 @brief 
 *
 *       Version:  1.0
 *       Compiler:  cc (HP-UX)
 *       @date:  04/23/09 20:51:56
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
#include "libMap2G_Addr.h"
#include "libMapAddrUtil.h"

int encIMSI(stMsgBuf_t *spMsg, OctetString *pD, int nTag)
{
	
	int     nIdx = 0;
	u_char     cTag[TAG_LEN];
	/* tag */
	if(makeTagString(cTag, nTag) < 0)   
		return(-1);
	int nIdx1;
	for(nIdx1=0; nIdx1<(int)strlen((char *)cTag) && nIdx1<TAG_LEN; nIdx1++)
		spMsg->cpData[nIdx++] = cTag[nIdx1];

	/* length */
	if(encLength(pD->numocts, &(spMsg->cpData[nIdx]), &nIdx) < 0)
		return(-1);

	/* TBCD */
	u_char     cOut[MAXIMSILENGTH];
	memset(cOut, 0x00, MAXIMSILENGTH);
	convBCD2Cdma(pD->data, ADDRESS_LEN_BCD, cOut, pD->numocts);

	memcpy(&(spMsg->cpData[nIdx]), cOut, pD->numocts);
	spMsg->nLen = nIdx + pD->numocts;
											
	return(1);
}

/*
int encAddTypeString(OctetString *spMsg, AddTypeString *pD, int nTag)
{
	int		nIdx = 0;
	int		nIdx1;
	u_char		cTemp;
	u_char		cOut[ADDRESS_LEN_STRING];
	u_char		cTag[TAG_LEN];
	
	if((int)pD->numocts < 0)
    {
        //LogRet("encAddTypeString length fail [%d]\n", pD->numocts);
        return(-1);
    }
	// tag 
	if(makeTagString(cTag, nTag) < 0)   return(-1);
    for(nIdx1=0; nIdx1<(int)strlen((char *)cTag); nIdx1++)
        spMsg->cpData[nIdx++] = cTag[nIdx1];
	
	// length
	if(encLength(pD->numocts+4, &(spMsg->cpData[nIdx]), &nIdx) < 0) 
	{
		return(-1);
	}

	// type of digits
	spMsg->cpData[nIdx++] = (u_char)pD->m.TypeOfDigits;
	spMsg->cpData[nIdx++] = (u_char)pD->m.NatureOfNumber;
	cTemp = (u_char)pD->m.NumberingPlan;
	cTemp = (u_char)(cTemp << 4);
	cTemp = (u_char)(cTemp | pD->m.Encoding);
	spMsg->cpData[nIdx++] = cTemp;
	spMsg->cpData[nIdx++] = (u_char)pD->m.NumberOfDigits;

	memset(cOut, 0x00, ADDRESS_LEN_STRING);
	convertTBCDString(pD->data, cOut, pD->numocts);

	memcpy(&(spMsg->cpData[nIdx]), cOut, pD->numocts);
    spMsg->nLen = nIdx + pD->numocts;

    return(1);
}
*/
