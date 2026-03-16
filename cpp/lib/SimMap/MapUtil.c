/**
 * =====================================================================================
 *
 *       @file  MapUtil.c
 *
 *    	 @brief 
 *
 *       Version:  1.0
 *       Compiler:  cc (HP-UX)
 *       @date:  04/23/09 20:59:00
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
#include <arpa/inet.h>
#include "libMapComm.h"
#include "libMapUtil.h"

int encTag ( u_char *Buffer, int tag )
{
    int             pos  = 0;
    int             loop = sizeof(int) +1;
	int 			nTmpTag = tag;
	nTmpTag = htonl(tag);
    u_char             *ptr = (u_char *)&nTmpTag;
    if ( *ptr > 0x01 )
        return -1;

    for( ;loop--; ++ptr) {
        if ( *ptr != 0x00 )
            break;
    }
	if(nTmpTag == TPDU_CT_8bit_REF_TAG)
		loop = 0;

	if ( loop ) {
		for (;loop--;)
		{
			Buffer[pos++] = *ptr++;
		}
	}
	else {
		Buffer[pos++] = 0x00;
	}

    return pos;
}

int makeTagString(u_char *cTag, int nTag)
{
	memset(cTag, 0x00, TAG_LEN);
	return encTag(cTag, nTag);
}

u_char SetBit(unsigned ucData,int nStart,int nLen,int nData)
{
    int nI,nJ;
    int nTemp=0,nTemp2=0,nTemp3;
    u_char ucTemp2;

    if(nStart>0)
    {
        nTemp = 255;
        nTemp >>=(8 - nStart);
        nTemp <<=(8 - nStart);

    }
    ucTemp2 = 255;
    ucTemp2 <<=(nStart+nLen);
    ucTemp2 >>=(nStart+nLen);

    nTemp3 = ucData & (nTemp+ucTemp2);

    nTemp3 +=  (nData <<(8 - (nStart+nLen)));

    return (u_char)nTemp3;
}
int PutBitByte(u_char *cpSourStr, int *nStartPos, int nLen,int nData)
{
    int             nLoop;
    int             nI,nJ,nK,nMod,nTemp,nTemp2;
    u_char   ucData,ucSourData;

    if(nLen > 8)
	{
        if(nLen > (8 * 2)) return -1;

		/*
        PutBitByte(cpSourStr, *nStartPos, nLen - 8, nData / 256);
        PutBitByte(cpSourStr, *nStartPos + nLen - 8, 8, nData % 256);
		*/
        return -1;
	}

    nMod = *nStartPos % 8;
    if(nMod+nLen <= 8)
    {
        cpSourStr[*nStartPos / 8] =
            (char)SetBit(cpSourStr[*nStartPos / 8], nMod, nLen, nData);
    }
    else
    {
        nTemp = nData >>(nMod+nLen - 8);
        ucData = nTemp;
        nJ = nLen - (nMod + nLen - 8);
        ucSourData = cpSourStr[(*nStartPos/8)];
        ucSourData >>= nJ;
        ucSourData <<= nJ;

        cpSourStr[*nStartPos/8] = (char)(ucData|ucSourData);

        nJ = (8 - (nMod + nLen - 8));
        ucSourData = cpSourStr[(*nStartPos/8)+1];
        ucSourData <<= (8 - nJ);
        ucSourData >>= (8 - nJ);
        ucData = nData;
        ucData <<= nJ;
        cpSourStr[(*nStartPos/8)+1] = (char)(ucData|ucSourData);
    }

	*nStartPos += nLen;
    return 1;
}

u_char In4toV( int		digit, int		position )
{
    u_char tmp[4];
 
    memset(tmp, 0x00, sizeof(tmp));
    memcpy(tmp, &digit, sizeof(int));
 
    switch(position)
    {
            case 1:
                    return tmp[3];
            case 2:
                    return tmp[2];
            case 3:
                    return tmp[1];
            case 4:
                    return tmp[0];
    }

	return NULL;
}

int encTotal(stMsgBuf_t *spDest, stMsgBuf_t *spSour, int nIdx, int cTag)
{
    int     nStep = 0;
    int     nLoop;
    int     nTotLen = 0;
	u_char 	*Ptr = spDest->cpData;

	nStep = encTag(Ptr, cTag);
	if ( nStep < 0 )
		return FLK_FAIL;
	Ptr = getBufferPtr(spDest->cpData, &spDest->nLen, nStep);

    for(nLoop=0; nLoop<nIdx; nLoop++)
        nTotLen = nTotLen + spSour[nLoop].nLen;

	nStep = 0;
    if(encLength(nTotLen, Ptr, &nStep) < 0)
        return(-1);
	Ptr = getBufferPtr(spDest->cpData, &spDest->nLen, nStep);

	int i = 0;
    for(nLoop=0; nLoop<nIdx; nLoop++)
    {
        memcpy(Ptr,spSour[nLoop].cpData,spSour[nLoop].nLen);
		nStep = spSour[nLoop].nLen;
		Ptr = getBufferPtr(spDest->cpData, &spDest->nLen, nStep);
    }

    return spDest->nLen;
}

int decLength(int *nLen, u_char *cpData, int *npIdx)
{
    int     nTemp;
    int     nIdx = 0;

    switch(cpData[nIdx])
    {
        case 0x81   :
            nIdx++;
            *nLen = (int)(cpData[nIdx]);
            *npIdx = *npIdx + 2;
            break;
        case 0x82   :
            nIdx++;
            nTemp = cpData[nIdx];
            nTemp = nTemp << 8;
            nIdx++;
            nTemp = nTemp | cpData[nIdx];
            *nLen = nTemp;
            *npIdx = *npIdx + 3;
            break;
        default     :
            if((char)cpData[nIdx] < 0 || cpData[nIdx] > 127)
            {
                log_write(LEV_ALL_ERR, "decLength fail [%x]\n", cpData[0]);
                return(-1);
            }
            *nLen = (int)(cpData[nIdx]);
            (*npIdx)++;
    }
    return(1);
}

int encLength(int nLen, u_char *cpData, int *npIdx)
{
    int     nIdx = 0;

    if((nLen >= 0) && (nLen <= 127)) /* short form */
    {
        //nLen = nLen << 24;
        memcpy(&cpData[nIdx], &nLen, 1);
        (*npIdx)++;
    }
    else if((nLen > 127) && (nLen < 256))
    {
        cpData[nIdx++] = 0x81;
        (*npIdx)++;
        //nLen = nLen << 24;
        memcpy(&cpData[nIdx], &nLen, 1);
        (*npIdx)++;
    }
    else
    {
   //     LogRet("encLength fail [%x]\n", nLen);
        return(-1);
    }

    return(1);
}

int encTLV(u_char *cpMsg, void *data, int length, int tag )
{
	int pos = 0;
	pos = encTag(cpMsg, tag);
	if ( pos < 0 )
		return FLK_FAIL;

	if(encLength(length, &cpMsg[pos], &pos) < 0)
        return FLK_FAIL;

	memcpy ( &cpMsg[pos], data, length );
    return (pos + length);
}

#if 0
//start postion = cpMsg[pos+4]
//cdma test       
int encTLV(u_char *cpMsg, void *data, int length, int tag )
{
	int pos = 0;
	pos = encTag(cpMsg, tag);
	if ( pos < 0 )
		return FLK_FAIL;

	if(encLength(length, &cpMsg[pos], &pos) < 0)
        return FLK_FAIL;

	memcpy ( &cpMsg[pos], data, length );

    if(tag == 40811){
        cpMsg[pos+7] = 0xf3;
        printf("LOGHJY cpMsg[%02x][%02x][%02x][%02x][%02x]\n",
                cpMsg[pos],cpMsg[pos+1],cpMsg[pos+2],cpMsg[pos+3],cpMsg[pos+4]);
    }
    return (pos + length);
}
#endif

int encTLV_DenyDataSzie(u_char *cpMsg, void *data, int length, int tag, int data_max_size )
{
	if ( data_max_size < 0 || length > data_max_size )
		return FLK_FAIL;

	return encTLV(cpMsg, data, length, tag);
}
u_char *getBufferPtr ( u_char *StartPtr, int *Tot, int MoveSize )
{
    *Tot += MoveSize;
    return (StartPtr + (*Tot));
}

int convertHex2Str(const u_char *cpSource, u_char *cpTarget, const int nLen)
{
	int 	nIdx1, nIdx2;
	u_char		cHigh, cLow;

	for(nIdx1=0, nIdx2=0; nIdx1 < nLen; nIdx1++)
	{
		cHigh = cpSource[nIdx1];	
		cHigh <<= 4;
		
        if(nIdx1 == nLen-1)
        {
            cLow = 0x0f;
        }
        else
        {
            cLow = cpSource[++nIdx1];
            cLow <<= 4;
            cLow >>= 4;
        }
        cpTarget[nIdx2++] = cHigh | cLow;
	}
	return(1);
}
int encOctetString(stMsgBuf_t *spMsg, OctetString *pD, int nTag)
{
	int nRet = 0;
	
	nRet = encTLV(spMsg->cpData, pD->data, pD->numocts, nTag);
	if ( nRet < 0 )
		return FLK_FAIL;

	spMsg->nLen = nRet;
    return nRet;
}
bool isAsciiAlphabet ( u_char *data, u_char len )
{
	u_char *ptr = NULL;
	bool ret = true;
	for ( ptr = data;len--; ++ptr) {
		if ( (*data & 0x80) == 0x80 )  {
			ret = false;
			break;
		}
	}
	return ret;
}

/* GSM Character change ASCII Character */
void changeASCII8bitToGSM(stASCIIString *spAscii)
{
	int nIndex1=0; 
	stGSM7bitString stTemp;
	bzero(&stTemp, sizeof(stGSM7bitString));

	/* nCharacter is real octet size */
	for(nIndex1=0; nIndex1 < spAscii->nCharacter; nIndex1++)
	{
		switch(spAscii->data[nIndex1])
		{
			case '@':
				stTemp.data[stTemp.nCharacter++] = 0x00;
				break;
			case '$':
				stTemp.data[stTemp.nCharacter++] = 0x02;
				break;
			case '_':
				stTemp.data[stTemp.nCharacter++] = 0x11;
				break;
			case '[': 	
				stTemp.data[stTemp.nCharacter++] = 0x1B;
				stTemp.data[stTemp.nCharacter++] = 0x3C;
				break;
			case 0x5C:
				stTemp.data[stTemp.nCharacter++] = 0x1B;
				stTemp.data[stTemp.nCharacter++] = 0x2F;
				break;
			case ']': 	
				stTemp.data[stTemp.nCharacter++] = 0x1B;
				stTemp.data[stTemp.nCharacter++] = 0x3E;
				break;
			case '^': 	
				stTemp.data[stTemp.nCharacter++] = 0x1B;
				stTemp.data[stTemp.nCharacter++] = 0x14;
				break;
			case '{': 	
				stTemp.data[stTemp.nCharacter++] = 0x1B;
				stTemp.data[stTemp.nCharacter++] = 0x28;
				break;
			case '|': 	
				stTemp.data[stTemp.nCharacter++] = 0x1B;
				stTemp.data[stTemp.nCharacter++] = 0x40;
				break;
			case '}': 	
				stTemp.data[stTemp.nCharacter++] = 0x1B;
				stTemp.data[stTemp.nCharacter++] = 0x29;
				break;
			case '~': 	
				stTemp.data[stTemp.nCharacter++] = 0x1B;
				stTemp.data[stTemp.nCharacter++] = 0x3D;
				break;
			default :
				stTemp.data[stTemp.nCharacter++] = spAscii->data[nIndex1];
				break;
		}
	}
	bzero(spAscii, sizeof(stASCIIString));
	memcpy(spAscii, &stTemp, sizeof(stASCIIString));
}

/* GSM 7bit encoding */
void Conv8bit2GSM7bit(stASCIIString  *spSource, stGSM7bitString *spTarget)
{
	int nIndex1=0, nIndex2=0;				
	stASCIIString stTemp;

	bzero(&stTemp, sizeof(stASCIIString));
	memcpy(&stTemp, spSource, sizeof(stASCIIString));

	/* chanage character */
	changeASCII8bitToGSM(&stTemp);

	/* stTemp.nCharacter is 8bit(Character Num) Octet size */
	/* spTarget->nOctet is 7bit Octet size */
	spTarget->nCharacter = stTemp.nCharacter;
	spTarget->nOctet 	 = (stTemp.nCharacter*7)/8;

	if(((stTemp.nCharacter*7)%8) > 0)
	{
		spTarget->nOctet = spTarget->nOctet+1;
	}

	while(spTarget->nCharacter > nIndex1)
	{
		int nShift = (nIndex1%8);
		spTarget->data[nIndex2] = ((stTemp.data[nIndex1] & 0x7f) >> nShift);

		if(nShift != 0)
		{
			spTarget->data[nIndex2-1] |= (stTemp.data[nIndex1] << (8-nShift));
			if(nShift != 7)
			{
				nIndex2++;
			}
		}
		else
		{
			nIndex2++;
		}
		nIndex1++;
	}
}
void switchMsbToLsb(u_char *dest, int len)
{
	if ( len <= 0 )
		return;

	int i;
	u_char *data = (u_char*)malloc(len);
	memset(data, 0x00, len);

	for ( i = 0; i < len; ++i ) {
		data[i] = ((dest[i] & 0x0F) << 4) | ((dest[i] & 0xF0) >> 4);
	}
	memcpy(dest, data, len);
	free(data);
}
