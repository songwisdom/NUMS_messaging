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
#include <time.h>
#include <sys/time.h>
#include "libMapUtil.h"
#include "libMapComm.h"
#include "libMap2G.h"
#include "libMap2G_Addr.h"
#include "libMap3G_Addr.h"
#define MDB_MSMIN_SIZE 	5
#define MDB_MIN_SIZE 	12
#define	MAX_SMSBEARERDATA	255
typedef struct {
	u_int  unContentsLen;
	u_char   ucContents[MAX_SMSBEARERDATA];
} stMap2304_t;

void reverse_parseTag(u_int *nTag, u_char *cTag, int nIdx)
{
    int nLoop;
    u_int nTemp = 0;
    for(nLoop = 0;nLoop < nIdx; nLoop++)
    {
        nTemp = nTemp << 8;
        memcpy(&nTemp,&cTag[nLoop],1);
    }
    *nTag = nTemp;
}

int parseTag(stMsgBuf_t *spMsg)
{
	int				nStep = 0;
	int				nIdx  = 0;
	u_char				cTag[TAG_LEN];
	unsigned int	nTemp;
	int				nTmp;
	

	memset(cTag, 0x00, TAG_LEN);
	if((spMsg->cpData[nStep] != 0x9F)
		&& (spMsg->cpData[nStep] != 0xdf)
		&& (spMsg->cpData[nStep] != 0xbf))
		cTag[nIdx++] = spMsg->cpData[nStep++];
	else
	{
		while(1)
		{
			cTag[nIdx++] = spMsg->cpData[nStep];
			if(!(spMsg->cpData[nStep++] & 0x80)) break; 
		}
		if(spMsg->cpData[nStep-2] == 0x8f)
		{
			nStep = nStep -1;
			nIdx = nIdx -1;
		}		
	}

#ifdef _HP_64BIT_SYS_TYPE_
	memcpy(&nTemp, cTag, 4);
	
    nTemp = nTemp >> (4 - nIdx)*8;
#else
	switch(nIdx)
	{
		case 1 :
		case 2 :
		case 3 :
		case 4 :
			reverse_parseTag(&nTemp, cTag, nIdx);
			break;
		default :
			return -1;
			break;
	}
#endif
	return(nTemp);
}
int decOctetString(stMsgBuf_t *spMsg, OctetString *pD, int nTag)
{
	int		nStep = 0;
	int		nLen;
	u_char		cOut[MINLEN];
	int		nIdx;
	int		nRet;
	u_char		cTag[TAG_LEN];
	

	/* tag */
	nRet = makeTagString(cTag, nTag);
	if(nRet < 0)  return(-1);
	for(nIdx=0; nIdx<nRet; nIdx++)
	{
		if(spMsg->cpData[nStep++] != cTag[nIdx])
		{
			log_write ( LEV_ALL_PARAM,"decOctetString tag fail [%x]\n", cTag[nIdx]);
			return(-1);
		}
	}
	
	/* length */
	if(decLength(&nLen, &spMsg->cpData[nStep], &nStep) < 0) return(-1);
	switch(nTag)
	{
		case ESN_TAG	:
			if(nLen != ESNLENGTH) 
			{
				log_write ( LEV_ALL_PARAM,"decOctetString:esn length fail [%d]\n", nLen);
				return(-1);
			}
			break;
		case SMS_ACC_DEN_REASON_TAG	:
		case SMS_CAUSE_CODE_TAG	:
			if(nLen != 1) 
			{
				log_write ( LEV_ALL_PARAM,"decOctetString:access denied reason length fail [%d]\n"
					, nLen);
				return(-1);
			}
			break;
		case MSC_ID_TAG :
			if(nLen != 3)
			{
				log_write ( LEV_ALL_PARAM,"decOctetString:SKT MSC_ID_TAG length fail [%d]\n" , nLen);
				return(-1);
			}
			break;
		case SMS_SKT_LOCATION_AREA_ID_TAG :
			if(nLen != 2)
			{
				log_write ( LEV_ALL_PARAM,"decOctetString:SMS_SKT_LOCATION_AREA_ID_TAG length fail [%d]\n" , nLen);
				return(-1);
			}
			break;
		case RECIPIENT_SERVICE_PROVI_IND_TAG :
			if(nLen != 1)
			{
				log_write ( LEV_ALL_PARAM,"decOctetString:RECIPIENT_SERVICE_PROVI_IND_TAG"
						" length fail [%d]\n", nLen);
			}
		default			:
			break;
	}	

	/* value */
	switch(nTag)
	{
		case MIN_TAG	:
			nLen = convCdma2BCD(pD->data, MINLEN, &spMsg->cpData[nStep], nLen);
			pD->numocts	= nLen;
			break;
		case SMS_SKT_ROAMING_PMN_TAG	:
			//	T : 0x9fff1b
			//	L : size 5 (0x05)
			//	V : consist of Length + Value (max 8 digit)
			//		mark 'f' is filler
			//		always use 4 octet when decoding
			if(decLength(&nLen, &spMsg->cpData[nStep], &nStep) < 0)
			{
				log_write ( LEV_ALL_PARAM,"[%s:%d] SMS_SKT_ROAMING_PMN_TAG decLength fail [%d]\n",
								__func__, __LINE__, nLen);
				return(-1);
			}
			memset(cOut, 0x00, ESNLENGTH);
			nLen = convCdma2BCD(pD->data, ESNLENGTH, &spMsg->cpData[nStep], nLen);
			pD->numocts = nLen;
			break;
		default			:
			pD->numocts = nLen;
			memcpy(pD->data, &spMsg->cpData[nStep], nLen);
			break;
	}

	nStep = nStep + pD->numocts;
	return(nStep);
}

int decAddTypeString(stMsgBuf_t *spMsg, OctetString *pD, int nTag)
{
	int		nStep = 0;
	int		nLen;
	u_char		cOut[ADDRESS_LEN_STRING];
	u_char     cTag[TAG_LEN];
	int		nRet;
	int		nIdx;
	AddTypeString *Addr = (AddTypeString *)pD->data;

	/* tag */
    nRet = makeTagString(cTag, nTag);
    if(nRet < 0)  return(-1);
    for(nIdx=0; nIdx<nRet; nIdx++)
    {
        if(spMsg->cpData[nStep++] != cTag[nIdx])
        {
            log_write(LEV_ALL_ERR, "decAddTypeString tag fail [%x]\n", cTag[nIdx]);
            return(-1);
        }
    }

	/* length */
	if(decLength(&nLen, &spMsg->cpData[nStep], &nStep) < 0) return(-1);

	if(nLen <= 0)
	{
		log_write(LEV_ALL_ERR, "decAddTypeString length fail [%d]\n", nLen);
        return(-1);
	}

	Addr->TypeOfDigits 			= spMsg->cpData[nStep++];
	Addr->NatureOfNumber		= spMsg->cpData[nStep++];
	Addr->m.NumberingPlan 	 	= (spMsg->cpData[nStep] & 0xF0) >> 4;
	Addr->m.Encoding 			= spMsg->cpData[nStep++] & 0x0F;
	Addr->NumberOfDigits 	= spMsg->cpData[nStep++];

	memset(cOut, 0x00, ADDRESS_LEN_STRING);
	nLen = convCdma2BCD(Addr->data, ADDRESS_LEN_STRING, cOut, (nLen-4));
    nStep = nStep + nLen;
	pD->numocts = 4 + nLen;;
    return(nStep);
}
int decIMSI(stMsgBuf_t *spMsg, OctetString *pD, int nTag)
{
    int     nStep = 0;
    int     nLen;
    u_char     cOut[MAXIMSILENGTH];
    int     nIdx;
    int     nRet;
    u_char     cTag[TAG_LEN];

    /* tag */
    nRet = makeTagString(cTag, nTag);
    if(nRet < 0)  return(-1);
    for(nIdx=0; nIdx<nRet; nIdx++)
    {
        if(spMsg->cpData[nStep++] != cTag[nIdx])
        {
            log_write(LEV_ALL_ERR, "decOctetString tag fail [%x]\n", cTag[nIdx]);
            return(-1);
        }
    }
    
	if(decLength(&nLen, &spMsg->cpData[nStep], &nStep) < 0)
        return(-1);

    /* set IMSI */
    nLen = convCdma2BCD(pD->data, MAXIMSILENGTH, &spMsg->cpData[nStep], nLen);
    pD->numocts = nLen;

    nStep = nStep + nLen;
    return(nStep);
}

int parseDummy(char *cpDesc, stMsgBuf_t *spMsg, int nTag)
{
	int				nStep = 0;
	int				nIdx  = 0;
	u_char				cTag[TAG_LEN];
	int				nRet;
	int				nLen;
	

	/* tag */
	nRet = makeTagString(cTag, nTag);
    if(nRet < 0)  return(-1);
    for(nIdx=0; nIdx<nRet; nIdx++)
    {
        if(spMsg->cpData[nStep++] != cTag[nIdx])
        {
            log_write( LEV_ALL_PARAM,"parseDummy tag fail [%x]\n", cTag[nIdx]);
            return(-1);
        }
    }

	/* lenth */
	if(decLength(&nLen, &spMsg->cpData[nStep], &nStep) < 0) return(-1);
	nStep = nStep + nLen;
	if((nTag != 0x95) && (nTag != 0x9f21))
	{
		log_write( LEV_ALL_PARAM,"%s:I don't know this Tag [H'%x]\n", cpDesc, nTag);
		log_writeHex(LEV_ALL_PARAM, "Hex\n",(char*)spMsg->cpData, nStep, 10);
	}
	return(nStep);
}
u_int strbitcp(u_char * Src, int Start, int Length)
{
    u_int Tmp;
    u_int Result = 0;
    int mod, tt;
 
    if(Start < 8) mod = 0;
    else mod = (Start / 8 );
 
    tt  =  (Start % 8);
 
    memcpy(&Tmp, &Src[mod], 4);
 
    Tmp = Tmp <<  tt;
    Result = Tmp  >> (32 - Length);
 
    return Result;
}
static int message_id(stMap2304_t *cptr, int step)
{
    u_char   	tmp;
	u_int  	msg_id, ltmp;

	tmp = (u_char)strbitcp((u_char *)&(cptr->ucContents[0]), step, 4);
	step = step + 4;

    switch(tmp)
    {
        case 0x00 :
            log_write(LEV_ALL_PARAM,"%25s = [%02x] (Reserved)\n", "Message Type", tmp);
            break;
        case 0x01 :
            log_write(LEV_ALL_PARAM,"%25s = [%02x] (Deliver : MT Only)\n", "Message Type", tmp);
            break;
        case 0x02 :
            log_write(LEV_ALL_PARAM,"%25s = [%02x] (Submit : MO Only)\n", "Message Type", tmp);
            break;
        case 0x03 :
            log_write(LEV_ALL_PARAM,"%25s = [%02x] (Cancellation : MO Only)\n", "Message Type", tmp);
            break;
        case 0x04 :
            log_write(LEV_ALL_PARAM,"%25s = [%02x] (Delivery Ack : MT Only)\n", "Message Type", tmp);
            break;
        case 0x05 :
            log_write(LEV_ALL_PARAM,"%25s = [%02x] (User Ack : either Direction)\n", "Message Type", tmp);
            break;
        case 0x08 :
            log_write(LEV_ALL_PARAM,"%25s = [%02x] (Broadcast Category : Broadcast)\n", "Message Type", tmp);
            break;
        case 0x09 :
            log_write(LEV_ALL_PARAM,"%25s = [%02x] (Broadcast Category : Point-to-Point)\n", "Message Type", tmp);
            break;
        case 0x0a :
            log_write(LEV_ALL_PARAM,"%25s = [%02x] (Communication Stop : MO Only)\n", "Message Type", tmp);
            break;
    }

	tmp = (u_char)strbitcp((u_char *)&(cptr->ucContents[0]), step, 8);
	step = step + 8;
	ltmp = tmp << 8;
	tmp = (u_char)strbitcp((u_char *)&(cptr->ucContents[0]), step, 8);
	step = step + 8;
	msg_id = ltmp | tmp;
    log_write(LEV_ALL_PARAM,"%25s = [%ld]\n", "Message ID", msg_id);
			
	step = step + 4;

	return step;
}
time_t      decchar72time21(char *cpDecTime)
{
    time_t      tTime;
    struct tm   stTm;
    char        cpTime[20];

    char        cpYear[5];
    char        cpMonth[3];
    char        cpDay[3];
    char        cpHour[3];
    char        cpMin[3];
    char        cpSec[3];

    cpTime[1]  = (u_char)((cpDecTime[0] & 0x0f)          | 0x30);
    cpTime[0]  = (u_char)(((cpDecTime[0] & 0xf0) >> 4)   | 0x30);
    cpTime[3]  = (u_char)((cpDecTime[1] & 0x0f)          | 0x30);
    cpTime[2]  = (u_char)(((cpDecTime[1] & 0xf0) >> 4)   | 0x30);
    cpTime[5]  = (u_char)((cpDecTime[2] & 0x0f)          | 0x30);
    cpTime[4]  = (u_char)(((cpDecTime[2] & 0xf0) >> 4)   | 0x30);
    cpTime[7]  = (u_char)((cpDecTime[3] & 0x0f)          | 0x30);
    cpTime[6]  = (u_char)(((cpDecTime[3] & 0xf0) >> 4)   | 0x30);
    cpTime[9]  = (u_char)((cpDecTime[4] & 0x0f)          | 0x30);
    cpTime[8]  = (u_char)(((cpDecTime[4] & 0xf0) >> 4)   | 0x30);
    cpTime[11] = (u_char)((cpDecTime[5] & 0x0f)          | 0x30);
    cpTime[10] = (u_char)(((cpDecTime[5] & 0xf0) >> 4)   | 0x30);
    cpTime[13] = (u_char)((cpDecTime[6] & 0x0f)          | 0x30);
    cpTime[12] = (u_char)(((cpDecTime[6] & 0xf0) >> 4)   | 0x30);
    cpTime[14] = '\0';

	if(!memcmp(cpTime, "0000", 4))
    {
        return (0);
    }
	
    memset(&stTm, 0x00, sizeof(struct tm));
	memcpy(cpYear, cpTime, 4);
    cpYear[4] = '\0';
    stTm.tm_year = atoi(cpYear);

    memcpy(cpMonth, &cpTime[4], 2);
    cpMonth[2] = '\0';
    stTm.tm_mon = atoi(cpMonth);

    memcpy(cpDay, &cpTime[6], 2);
    cpDay[2] = '\0';
    stTm.tm_mday = atoi(cpDay);

    memcpy(cpHour, &cpTime[8], 2);
    cpHour[2] = '\0';
    stTm.tm_hour = atoi(cpHour);

    memcpy(cpMin, &cpTime[10], 2);
    cpMin[2] = '\0';
    stTm.tm_min = atoi(cpMin);

    memcpy(cpSec, &cpTime[12], 2);
    cpSec[2] = '\0';
    stTm.tm_sec = atoi(cpSec);

    stTm.tm_year -= 1900;
    stTm.tm_mon --;
    stTm.tm_isdst = daylight;

    if( (tTime = mktime(&stTm)) == -1)
    {
        return 0;
    }
    return tTime;
}
char		*
time2str(
char 		*cpResult,
int		nResultSize,
char 		*cpFormat,
time_t 	tTime)
{
	struct tm	*spTm;
	char			cpFmt[50];
	int			nTimeSize;
	int			nPos;
	int			nEnd;

	if( cpFormat == NULL )
	{
		strcpy(cpFormat, "%Y%m%d%H%M%S");
	}

	if( (spTm = localtime(&tTime)) == NULL )
	{
		return NULL;
	}

	if( strchr(cpFormat, '%') != NULL)
	{
#ifdef _HP_64BIT_SYS_TYPE_
		if( (nTimeSize = (int)strftime(cpResult, nResultSize, cpFormat, spTm)) == 0 )
		{
			return NULL;
		}
#else
		if( (nTimeSize = strftime(cpResult, nResultSize, cpFormat, spTm)) == 0 )
		{
			return NULL;
		}
#endif
	} 
	else
	{
		memset(cpFmt, 0x00, 50);
		nPos = 0;
		while(cpFormat[nPos] != '\0')
		{
			switch(cpFormat[nPos])
			{
				case 'Y':
					if(cpFormat[nPos+3] == 'Y')
					{
						strcat(cpFmt, "%Y");
						nPos += 4;
					}
					else if(cpFormat[nPos+1] == 'Y')
					{
						strcat(cpFmt, "%y");
						nPos += 2;
					}
					else
					{
						return NULL;
					}
					break;
				case 'M':
					if(cpFormat[nPos+1] == 'M')
					{
						strcat(cpFmt, "%m");
						nPos += 2;
					}
					else
					{
						strcat(cpFmt, "%m");
						nPos ++;
					}
					break;
				case 'D':
					if(cpFormat[nPos+1] == 'D')
					{
						strcat(cpFmt, "%d");
						nPos += 2;
					}
					else
					{
						strcat(cpFmt, "%e");
						nPos ++;
					}
					break;
				case 'h':
					if(cpFormat[nPos+1] == 'h')
					{
						strcat(cpFmt, "%H");
						nPos += 2;
					}
					else
					{
						strcat(cpFmt, "%I");
						nPos ++;
					}
					break;
				case 'm':
					if(cpFormat[nPos+1] == 'm')
					{
						strcat(cpFmt, "%M");
						nPos += 2;
					}
					else
					{
						strcat(cpFmt, "%M"); nPos ++; 
					}
					break;
				case 's':
					if(cpFormat[nPos+1] == 's')
					{
						strcat(cpFmt, "%S");
						nPos += 2;
					}
					else
					{
						strcat(cpFmt, "%S");
						nPos ++;
					}
					break;
				default:
#ifdef _HP_64BIT_SYS_TYPE_
					nEnd = (int)strlen(cpFmt);
#else
					nEnd = strlen(cpFmt);
#endif
					cpFmt[nEnd] = cpFormat[nPos];
					cpFmt[nEnd + 1] = '\0';
					nPos++;
			} /* end of switch */
		} /* end of while */

#ifdef _HP_64BIT_SYS_TYPE_
		if( (nTimeSize = (int)strftime(cpResult, nResultSize, cpFmt, spTm)) == 0 )
		{
			return NULL;
		}
#else

		if( (nTimeSize = strftime(cpResult, nResultSize, cpFmt, spTm)) == 0 )
		{
			return NULL;
		}
#endif
	} 

	cpResult[nTimeSize] = '\0';
	return cpResult;
}
char *time2decchar7(char *cpResult, time_t tTime)
{
	char		cpTime[20], cpTmp[7];

	memset(cpTime, 0x00, sizeof(cpTime));
	memset(cpTmp, 0x00, sizeof(cpTmp));

#ifdef _HP_64BIT_SYS_TYPE_
	if(time2str(cpTime, (int)sizeof(cpTime), "%Y%m%d%H%M%S", tTime) == NULL)
	{
		return NULL;
	}
#else
	if(time2str(cpTime, sizeof(cpTime), "%Y%m%d%H%M%S", tTime) == NULL)
	{
		return NULL;
	}
#endif

	cpTmp[0] = ( ((cpTime[0]&0x0f) << 4)  | (cpTime[1] & 0x0f) ); 
	cpTmp[1] = ( ((cpTime[2]&0x0f) << 4)  | (cpTime[3] & 0x0f) ); 
	cpTmp[2] = ( ((cpTime[4]&0x0f) << 4)  | (cpTime[5] & 0x0f) ); 
	cpTmp[3] = ( ((cpTime[6]&0x0f) << 4)  | (cpTime[7] & 0x0f) ); 
	cpTmp[4] = ( ((cpTime[8]&0x0f) << 4)  | (cpTime[9] & 0x0f) ); 
	cpTmp[5] = ( ((cpTime[10]&0x0f) << 4) | (cpTime[11] & 0x0f) ); 
	cpTmp[6] = ( ((cpTime[12]&0x0f) << 4) | (cpTime[13] & 0x0f) ); 

	if(cpResult != NULL)
	{
		memcpy(cpResult, cpTmp, 7);
	}
	return cpResult;
}
#if 0
static int call_back_number(stMap2304_t *cptr, int step)
{
	int 	i, itmp;
    u_char   tmp, tmp1, callback_num;
	u_char	digit_mode, num_plan, num_type;
	u_char	callbackBuff[BUF_SSIZE];
	
    memset(callbackBuff, 0xff, sizeof(callbackBuff));

	digit_mode = (u_char)strbitcp((void *)&(cptr->ucContents[0]), step, 1);
	step = step + 1;

	if (digit_mode != 0) {
        log_write(LEV_ALL_PARAM,"%25s = [%02x] (ASCII 8-bit)\n", "Digit Mode", digit_mode);

		num_type = (u_char)strbitcp((void *)&(cptr->ucContents[0]), step, 3);
		step = step + 3;
        log_write(LEV_ALL_PARAM,"%25s = [%02x]\n", "Number type", num_type);

		num_plan = (u_char)strbitcp((void *)&(cptr->ucContents[0]), step, 4);
		step = step + 4;
		log_write(LEV_ALL_PARAM,"%25s = [%02x]\n", "Number Plan", num_plan);

		callback_num = (u_char)strbitcp((void *)&(cptr->ucContents[0]), step, 8);
		step = step + 8;
		log_write(LEV_ALL_PARAM,"%25s = [%02x]\n", "Number field", callback_num);

		for (i = 0; i < ((int) callback_num) 
				&& ((int) callback_num < 24); i++)
		{
			tmp = (u_char)strbitcp((void *)&(cptr->ucContents[0]), step, 8);
			step = step + 8;
			callbackBuff[i] = tmp;
            log_write(LEV_ALL_PARAM,"%25s [%d] = [%02x]\n", "CallbackNum", i, tmp);
		}
		
	} 
	else
	{
        log_write(LEV_ALL_PARAM,"%25s = [%02x] (DTMF 4-bit)\n", "Digit Mode", digit_mode);

		callback_num = (u_char)strbitcp((void *)&(cptr->ucContents[0]), step, 8);
		step = step + 8;
		log_write(LEV_ALL_PARAM,"%25s = [%02x]\n", "Number field", callback_num);

		for (i = 0; i < ((int) callback_num / 2); i++)
		{
			tmp = (u_char)strbitcp((void *)&(cptr->ucContents[0]), step, 4);
			step = step + 4;
			tmp1 = (u_char)strbitcp((void *)&(cptr->ucContents[0]), step, 4);
			step = step + 4;

			tmp = (u_char)(tmp << 4);
			
			callbackBuff[i] = (u_char)(tmp | tmp1);
			log_write(LEV_ALL_PARAM,"%25s [%d] = [%02x]\n", "CallbackNum", i, tmp | tmp1);

		}	/* End For */

        if ((callback_num % 2) != 0)
        {
            tmp = (u_char)strbitcp((void *)&(cptr->ucContents[0]), step, 4);
			callbackBuff[callback_num/2] = (u_char)((tmp << 4) | 0x0f);

            log_write(LEV_ALL_PARAM, "%25s [%d] = [%02x]\n", "CallbackNum", 
            	callback_num/2, callbackBuff[callback_num/2]);
        }

        if ((step % 8) == 0) itmp = 0;
        else itmp = 8 - (step % 8);

        step = step + itmp;
	} 
	return step;
}
#endif
static u_char	DATA_NUM1 = 0;
static int user_data(stMap2304_t *cptr, int step)
{
	int i, itmp;
    u_char   tmp, num_field;
	// FR210 by chan : for orignal total length 
	u_short org_tot_len = 0;
	u_char dbMessageContent[256] = {0x00, };
	int dbMessageLength = 0;

	tmp = (u_char)strbitcp((u_char *)&(cptr->ucContents[0]), step, 5);
	step = step + 5;

	/* DR330 Save In DCS Value */

	//	Do not use DCS field when receiving msg from CDMA Network
	//	Use only 2 Parmeters
	//	1. MsgEncoding (dbDataEncoding)
	//	2. LanguageIndicator (dbLanguageFlag)

    switch(tmp)
    {
        case 0x01 :
            log_write(LEV_ALL_PARAM,"%25s = [%02x] (IS-91 Extended protocol)\n", "Message Encoding", tmp);
            return -1;

        case 0x10 :
            log_write(LEV_ALL_PARAM,"%25s = [%02x] (KSC5601i : 8-bit)\n", "Message Encoding", tmp);
            break;

        case 0x00 :
            log_write(LEV_ALL_PARAM,"%25s = [%02x] (Octet : 8-bit)\n", "Message Encoding", tmp);
            break;

        case 0x02 :
            log_write(LEV_ALL_PARAM,"%25s = [%02x] (7-bit ASCII)\n", "Message Encoding", tmp);
            break;

        case 0x03 :
            log_write(LEV_ALL_PARAM,"%25s = [%02x] (IA5 CCITT T.50 : 7-bit)\n", "Message Encoding", tmp);
            break;
		
		default:
			log_write(LEV_ALL_PARAM,"%25s = [%02x] (unknown) \n","Message Encoding", tmp);
			break;
    }

	switch(tmp)
	{
		case 0x10 :
		case 0x00 :
			num_field = (u_char)strbitcp((u_char *)&(cptr->ucContents[0]), step, 8);
			step = step + 8;
    		log_write(LEV_ALL_PARAM,"%25s = [%02x]\n", "Number Field", num_field);
			DATA_NUM1 = num_field;
// for Message max length check
// if over max length, copy to max length... 20050616


			for (i = 0; i < (int) num_field; i++)
			{
				tmp = (u_char)strbitcp((u_char *)&(cptr->ucContents[0]), step, 8);
				step = step + 8;
				dbMessageContent[i] = tmp;
			}
			log_write(LEV_ALL_PARAM,"%25s = [%02X][%02X][%02X]...\n", "User Data", 
					dbMessageContent[0], dbMessageContent[1], dbMessageContent[2] );
			dbMessageContent[DATA_NUM1] = '\0';

			step = step + 3;
			break;
		case 0x02 :
		case 0x03 :
	    	num_field = (u_char)strbitcp((u_char *)&(cptr->ucContents[0]), step, 8);
        	step = step + 8;
        	log_write(LEV_ALL_PARAM,"%25s = [%02x]\n", "Number Field", num_field);
        	DATA_NUM1 = num_field;
// for Message max length check
// if over max length, copy to max length... 20050616


        	for (i = 0; i < (int) num_field; i++)
        	{
            	tmp = (u_char)strbitcp((u_char *)&(cptr->ucContents[0]), step, 7);
            	step = step + 7;
				dbMessageContent[i] = tmp;
         //   	log_write(LEV_ALL_PARAM,"%25s [%d] = [%02x]\n", "User Data", i, tmp);
        	}
			log_write(LEV_ALL_PARAM,"%25s = [%02X][%02X][%02X]...\n", "User Data", 
					dbMessageContent[0], dbMessageContent[1], dbMessageContent[2] );
			dbMessageContent[DATA_NUM1] = '\0';

        	if ((step % 8) == 0) itmp = 0;
        	else itmp = 8 - (step % 8);

        	step = step + itmp;
			break;
		default:
			log_write(LEV_ALL_PARAM,"%25s = [%02X][%02X][%02X]...\n", "User Data", 
					dbMessageContent[0], dbMessageContent[1], dbMessageContent[2] );
			log_write(LEV_ALL_PARAM, "%25s = [%02x] (unknown) \n","Message Encoding", tmp);
			break;
    }

	/* FR210 : 10.01.11 by Chan */
	org_tot_len = num_field;
	log_write(LEV_ALL_PARAM,"Contents (%d) [%s]\n",  strlen((char*)dbMessageContent), dbMessageContent);

	return step;
}
void Decoding637(stMap2304_t *cptr)
{
	u_char 	subparam_type, subparam_len;
	u_int    lsum = 0;
	u_char 	tmp;
	int 			step = 0, i, flag = 1;
	int 			bearer_len;
	int    			mc_time_flag = 0, user_data_flag;
	u_char	mc_tmp[15];
	u_char   cpTime[7];
	time_t 			ucSmcTimestamp;

	user_data_flag = 0;
	bearer_len = cptr->unContentsLen;

	log_write(LEV_ALL_PARAM," ======== Bearer Data  =========");

	for (i = 0; i < bearer_len; i++) 
	{
		if ((i % 10) == 0) log_write(LEV_ALL_PARAM,"\n");
		log_write(LEV_ALL_PARAM,"[%02x]", cptr->ucContents[i]);
	}
	log_write(LEV_ALL_PARAM,"\n");

	DATA_NUM1 = 0;

	while (flag)
	{
		subparam_type = (u_char)strbitcp((u_char *)&(cptr->ucContents[0]), step, 8);
		step = step + 8;

		subparam_len = (u_char)strbitcp((u_char *)&(cptr->ucContents[0]), step, 8);
		step = step + 8;

		lsum = lsum + subparam_len + 2;

		if (cptr->unContentsLen < lsum) 
		{
			log_write(LEV_ALL_PARAM, "Mismatch Message Length(IN) = [%02x]\n", subparam_len);
			flag = 0;
			break;
		}

		if ( subparam_type == USER_DATA && user_data_flag == 1 )
		{
			log_write(LEV_ALL_PARAM,"Duplicated USER_DATA.......\n");
			flag = 0;
			break;
		}

		switch(subparam_type) {
			case MESSAGE_IDENTIFIER :
                    log_write(LEV_ALL_PARAM,"%25s = [%02x]\n", "MESSAGE IDENTIFIER", subparam_type);
                    log_write(LEV_ALL_PARAM,"%25s = [%02x]\n", "MESSAGE Length", subparam_len);
					step = message_id(cptr, step);
					break;

			case USER_DATA :
					user_data_flag = 1;
                    log_write(LEV_ALL_PARAM,"%25s = [%02x]\n", "USER DATA", subparam_type);
                    log_write(LEV_ALL_PARAM,"%25s = [%02x]\n", "USER DATA Length", subparam_len);
					step = user_data(cptr, step);
		            if (step == -1)
                    {
                        log_write(LEV_ALL_PARAM, "User Data Encoding Error..\n");
                        flag = 0;
                    }
					break;

			case USER_RES_CODE :	
                    log_write(LEV_ALL_PARAM,"%25s = [%02x]\n", "USER RESPONSE DATA", subparam_type);
					tmp = (u_char)strbitcp((u_char *)&(cptr->ucContents[0]), step, 8);
					step = step + 8;
                    switch(tmp)
                    {
                        case 0x00 :
                            log_write(LEV_ALL_PARAM,"%25s = [%02x] (Digit)\n", "response type", tmp);
                            break;
                        case 0x01 :
                            log_write(LEV_ALL_PARAM,"%25s = [%02x] (Number)\n", "response type", tmp);
                            break;
                        case 0x02 :
                            log_write(LEV_ALL_PARAM,"%25s = [%02x] (Telephone Number)\n", "response type", tmp);
                            break;
                        case 0x03 :
                            log_write(LEV_ALL_PARAM,"%25s = [%02x] (Encryption)\n", "response type", tmp);
                            break;
                        case 0x04 :
                            log_write(LEV_ALL_PARAM,"%25s = [%02x] (String)\n", "response type", tmp);
                            break;
                        case 0x05 :
                            log_write(LEV_ALL_PARAM,"%25s = [%02x] (Menu)\n", "response type", tmp);
                            break;
                        case 0x06 :
                            log_write(LEV_ALL_PARAM,"%25s = [%02x] (Date)\n", "response type", tmp);
                            break;
                        case 0x07 :
                            log_write(LEV_ALL_PARAM,"%25s = [%02x] (Time)\n", "response type", tmp);
                            break;
                        case 0x08 :
                            log_write(LEV_ALL_PARAM,"%25s = [%02x] (continue)\n", "response type", tmp);
                            break;
                    }
					break;

			case MES_CENTER_TIME :
			{	
					mc_time_flag = 1;

                    log_write(LEV_ALL_PARAM,"%25s = [%02x]\n", "MESSAGE CENTER TIME", subparam_type);
                    log_write(LEV_ALL_PARAM,"%25s = [%02x]\n", "MESSAGE CENTER Length", subparam_len);

					tmp = (u_char)strbitcp((u_char *)&(cptr->ucContents[0]), step, 8);
                    if (tmp < 0x90)
                        mc_tmp[0] = 0x20;
                    else
                        mc_tmp[0] = 0x19;

					for(i = 1; i < 7; i++)
					{
						tmp = (u_char)strbitcp((u_char *)&(cptr->ucContents[0]), step, 8);
						step = step + 8;
						mc_tmp[i] = tmp;
					}
					
                    ucSmcTimestamp = decchar72time21((char *)mc_tmp);
					time2decchar7((char *)cpTime, ucSmcTimestamp);

                  	for(i = 0; i < 7; i++)
                     	log_write(LEV_ALL_PARAM,"%25s [%d] = [%02x]\n", "smctimeStamp", i, mc_tmp[i]);
					
					break;
			}
			case VALIDITY_PERIOD_ABS :
			        log_write(LEV_ALL_PARAM,"%25s = [%02x]\n", "VALIDITY PERIOD ABS", subparam_type);
					for(i = 0; i < 6; i++)
					{
						tmp = (u_char)strbitcp((u_char *)&(cptr->ucContents[0]), step, 8);
						step = step + 8;
					}
					break;

			case VALIDITY_PERIOD_REL :
			        log_write(LEV_ALL_PARAM,"%25s = [%02x]\n", "VALIDITY PERIOD REL", subparam_type);
					tmp = (u_char)strbitcp((u_char *)&(cptr->ucContents[0]), step, 8);
					step = step + 8;
					break;

			case DEFERRED_D_TIME_ABS :
			        log_write(LEV_ALL_PARAM,"%25s = [%02x]\n", "DEFERRED TIME ABS", subparam_type);
					for(i = 0; i < 6; i++)
					{
						tmp = (u_char)strbitcp((u_char *)&(cptr->ucContents[0]), step, 8);
						step = step + 8;
					}
					break;

			case DEFERRED_D_TIME_REL :
			        log_write(LEV_ALL_PARAM,"%25s = [%02x]\n", "DEFERRED TIME REL", subparam_type);
					tmp = (u_char)strbitcp((u_char *)&(cptr->ucContents[0]), step, 8);
					step = step + 8;
					break;

			case PRIORITY_INDICATOR :
			        log_write(LEV_ALL_PARAM,"%25s = [%02x]\n", "PRIORITY INDICATOR", subparam_type);
					tmp = (u_char)strbitcp((u_char *)&(cptr->ucContents[0]), step, 2);
					step = step + 2;

                    switch(tmp)
                    {
                        case 0x00 :
                            log_write(LEV_ALL_PARAM,"%25s = [%02x] (Normal)\n", "priority", tmp);
                            break;
                        case 0x01 :
                            log_write(LEV_ALL_PARAM,"%25s = [%02x] (Interactive)\n", "priority", tmp);
                            break;
                        case 0x02 :
                            log_write(LEV_ALL_PARAM,"%25s = [%02x] (Urgent) \n", "priority", tmp);
                            break;
                        case 0x03 :
                            log_write(LEV_ALL_PARAM,"%25s = [%02x] (Emergency)\n", "priority", tmp);
                            break;
                    }

					step = step + 6;
					break;
					
			case PRIVACY_INDICATOR :
			        log_write(LEV_ALL_PARAM,"%25s = [%02x]\n", "PRIVACY INDICATOR", subparam_type);
					tmp = (u_char)strbitcp((u_char *)&(cptr->ucContents[0]), step, 2);
					step = step + 2;

                    switch(tmp)
                    {
                        case 0x00 :
                            log_write(LEV_ALL_PARAM,"%25s = [%02x] (Not Restricted)\n", "Privacy", tmp);
                            break;
                        case 0x01 :
                            log_write(LEV_ALL_PARAM,"%25s = [%02x] (Restricted)\n", "Privacy", tmp);
                            break;
                        case 0x02 :
                            log_write(LEV_ALL_PARAM,"%25s = [%02x] (Confidential)\n", "Privacy", tmp);
                            break;
                        case 0x03 :
                            log_write(LEV_ALL_PARAM,"%25s = [%02x] (Secret)\n", "Privacy", tmp);
                            break;
                    }

					step = step + 6;
					break;

			case REPLY_OPTION :
			        log_write(LEV_ALL_PARAM,"%25s = [%02x]\n", "REPLY OPTION", subparam_type);
					tmp = (u_char)strbitcp((u_char *)&(cptr->ucContents[0]), step, 1);
					step = step + 1;
                    log_write(LEV_ALL_PARAM,"%25s = [%02x]\n", "User Ack Req", tmp);

					tmp = (u_char)strbitcp((u_char *)&(cptr->ucContents[0]), step, 1);
					step = step + 1;
                    log_write(LEV_ALL_PARAM,"%25s = [%02x]\n", "Delivert Ack Req", tmp);

					tmp = (u_char)strbitcp((u_char *)&(cptr->ucContents[0]), step, 6);
					step = step + 6;
					break;

			case NUMBER_MESSAGE :
			        log_write(LEV_ALL_PARAM,"%25s = [%02x]\n", "NUMBER MESSAGE", subparam_type);
					tmp = (u_char)strbitcp((u_char *)&(cptr->ucContents[0]), step, 8);
					step = step + 8;
                    log_write(LEV_ALL_PARAM,"%25s = [%02x]\n", "Message Count", tmp);
					break;

			case ALERT_ON_MESSAGE :
			        log_write(LEV_ALL_PARAM,"%25s = [%02x]\n", "ALERT ON MESSAGE", subparam_type);
					break;

			case LANGUAGE_INDICATOR :
			        log_write(LEV_ALL_PARAM,"%25s = [%02x]\n", "LANGUAGE INDICATION", subparam_type);
					tmp = (u_char)strbitcp((u_char *)&(cptr->ucContents[0]), step, 8);
					step = step + 8;
                    switch(tmp)
                    {
                        case 0x00 :
                            log_write(LEV_ALL_PARAM,"%25s = [%02x] (Unknown or unspecified)\n", "language indication", tmp);
                            break;
                        case 0x01 :
                            log_write(LEV_ALL_PARAM,"%25s = [%02x] (English)\n", "language indication", tmp);
                            break;
                        case 0x02 :
                            log_write(LEV_ALL_PARAM,"%25s = [%02x] (French)\n", "language indication", tmp);
                            break;
                        case 0x03 :
                            log_write(LEV_ALL_PARAM,"%25s = [%02x] (Spanish)\n", "language indication", tmp);
                            break;
                        case 0x40 :
                            log_write(LEV_ALL_PARAM,"%25s = [%02x] (Korean)\n", "language indication", tmp);
                            break;
                        case 0xfe :
                            log_write(LEV_ALL_PARAM,"%25s = [%02x] (Korean)\n", "language indication", tmp);
                            break;
                    }
					break;
				
			case CALL_BACK_NUMBER :
			        log_write(LEV_ALL_PARAM,"%25s = [%02x]\n", "CALL BACK NUMBER", subparam_type);
                    log_write(LEV_ALL_PARAM,"%25s = [%02x]\n", "CALL BACK Length", subparam_len);
					break;

			case RESPONSE_TYPE :
			        log_write(LEV_ALL_PARAM,"%25s = [%02x]\n", "RESPONSE TYPE", subparam_type);
					tmp = (u_char)strbitcp((u_char *)&(cptr->ucContents[0]), step , 8);
					step = step + 8;

                    switch(tmp)
                    {
                        case 0x00 :
                            log_write(LEV_ALL_PARAM,"%25s = [%02x] (Digit)\n", "response type", tmp);
                            break;
                        case 0x01 :
                            log_write(LEV_ALL_PARAM,"%25s = [%02x] (Number)\n", "response type", tmp);
                            break;
                        case 0x02 :
                            log_write(LEV_ALL_PARAM,"%25s = [%02x] (Telephone Number)\n", "response type", tmp);
                            break;
                        case 0x03 :
                            log_write(LEV_ALL_PARAM,"%25s = [%02x] (Encryption)\n", "response type", tmp);
                            break;
                        case 0x04 :
                            log_write(LEV_ALL_PARAM,"%25s = [%02x] (String)\n", "response type", tmp);
                            break;
                        case 0x05 :
                            log_write(LEV_ALL_PARAM,"%25s = [%02x] (Menu)\n", "response type", tmp);
                            break;
                        case 0x06 :
                            log_write(LEV_ALL_PARAM,"%25s = [%02x] (Date)\n", "response type", tmp);
                            break;
                        case 0x07 :
                            log_write(LEV_ALL_PARAM,"%25s = [%02x] (Time)\n", "response type", tmp);
                            break;
                        case 0x08 :
                            log_write(LEV_ALL_PARAM,"%25s = [%02x] (continue)\n", "response type", tmp);
                            break;
                    }
					break;

            case SESSION_INFORMATION :
                    log_write(LEV_ALL_PARAM,"%25s = [%02x]\n", "SESSION INFORMATION", subparam_type);
                    tmp = (u_char)strbitcp((u_char *)&(cptr->ucContents[0]), step , 8);
                    step = step + 8;
                    log_write(LEV_ALL_PARAM,"%25s = [%02x]\n", "session ID", tmp);

                    tmp = (u_char)strbitcp((u_char *)&(cptr->ucContents[0]), step , 7);
                    step = step + 7;
                    log_write(LEV_ALL_PARAM,"%25s = [%02x]\n", "session Sequence", tmp);

                    tmp = (u_char)strbitcp((u_char *)&(cptr->ucContents[0]), step , 1);
                    step = step + 1;
                    log_write(LEV_ALL_PARAM,"%25s = [%02x]\n", "end Session", tmp);
                    break;


			default :
					log_write(LEV_ALL_PARAM, "Unknown Message type = [02%x]\n", subparam_type);
					flag = 0;
					return;
					break;
		} /* switch end */

	if (flag == 0) return;

		if ((int)bearer_len  == (step / 8)) {
			break;
		}

	} /* while end */				

	if (mc_time_flag == 0)
    {
        time(&ucSmcTimestamp);
    }

	cptr->unContentsLen = DATA_NUM1;
	if (DATA_NUM1 == 0)  memset(&cptr->ucContents, 0x00, sizeof(bearer_len));


}
int decSMDPP(stMsgBuf_t *spMsg, stSMDPP_t *pD, int nTag)
{
	stMsgBuf_t	stMsg;
	int			nStep	= 0;
	int			nLen	= 0;
	int			nSize 	= 0;
	int			nlSize	= 0;
	int			nRet;
	int			nIdx;
	u_char         cTag[TAG_LEN];

	if(makeTagString(cTag, nTag) < 0) return(-1);
    if(cTag[0] != spMsg->cpData[nStep++])
    {
		if((spMsg->cpData[0] != 0x31) &&
			(spMsg->cpData[0] !=0x30)){
			log_write ( LEV_ALL_PARAM, "decsmsreq tag Settag or Sequenct tag [H'%x]\n",
						spMsg->cpData[0]);
			return(-1);
		}
 	}
	/* Skip TAG 
	nStep++;
	*/

	if(decLength(&nLen, &spMsg->cpData[nStep], &nStep) < 0) return(-1);
	
	/* check total length */
	nlSize = spMsg->nLen - nStep;
    if((nlSize) != nLen)
    {
        log_write ( LEV_ALL_PARAM, "decSMDPP length fail [%d] = [%d]\n", nlSize, nLen);
		return(-1);
    }
	
	/* parse parameter */
	while(nlSize > 0)
	{
		memset(&stMsg, 0x00, sizeof(stMsgBuf_t));
		stMsg.nLen = nlSize;
		memcpy(stMsg.cpData, &spMsg->cpData[nStep], stMsg.nLen);
		nRet = parseTag(&stMsg);
		if(nRet <= 0) 
		{
			log_write ( LEV_ALL_PARAM, "parseTag fail\n");
			return(-1);
		}
		switch(nRet)
		{
			case SMS_BEARERDATA_TAG				:	
				nSize = decOctetString(&stMsg, &pD->smsbd, SMS_BEARERDATA_TAG);
				if(nSize < 0) return(-1);
				nlSize	= nlSize - nSize;
				nStep 	= nStep + nSize;
				stMap2304_t stTmp; 
				memset(&stTmp, 0x00, sizeof(stTmp));
				memcpy(stTmp.ucContents, pD->smsbd.data, pD->smsbd.numocts);
				stTmp.unContentsLen = pD->smsbd.numocts;
				Decoding637(&stTmp);
				break;
			case SMS_TELESERVICE_IND_TAG	:	
				nSize = decOctetString(&stMsg, &pD->smsti, SMS_TELESERVICE_IND_TAG);
				if(nSize < 0) return(-1);
                nlSize  = nlSize - nSize;
                nStep   = nStep + nSize;
				break;
			case ESN_TAG		:	
				nSize = decOctetString(&stMsg, &pD->esn, ESN_TAG);
				if(nSize < 0) return(-1);
                nlSize  = nlSize - nSize;
                nStep   = nStep + nSize;
                pD->m.esnPresent = 1;
				break;
			case MIN_TAG		:	
				nSize = decOctetString(&stMsg, &pD->min, MIN_TAG);
				if(nSize < 0) return(-1);
                nlSize  = nlSize - nSize;
                nStep   = nStep + nSize;
               	if((pD->min.numocts > MDB_MSMIN_SIZE) ||
						(pD->min.numocts < 1))
				{
					log_write ( LEV_ALL_PARAM, "[%s]MIN Size %d Over..Receive Size [%d]\n",
							__FILE__,MDB_MSMIN_SIZE, pD->min.numocts);
					return(-1);
				}
				pD->m.minPresent = 1;
				break;
			case SMS_ORG_DEST_ADDR_TAG		:	
				nSize = decAddTypeString(&stMsg, &pD->smsoda, SMS_ORG_DEST_ADDR_TAG);
				if(nSize < 0) return(-1);
                nlSize  = nlSize - nSize;
                nStep   = nStep + nSize;
                if((pD->smsooa.numocts > MDB_MIN_SIZE) ||
						(pD->smsooa.numocts < 1))
				{
					log_write ( LEV_ALL_PARAM, "[%s] ORG Dest ADDR Size %d Over..Receive Size [%d]\n",
							__FILE__,MDB_MIN_SIZE, pD->smsooa.numocts);
					return(-1);
				}
				pD->m.smsodaPresent = 1;
				break;
			case SMS_ORG_ORG_ADDR_TAG		:	
				nSize = decAddTypeString(&stMsg, &pD->smsooa, SMS_ORG_ORG_ADDR_TAG);
				if(nSize < 0) return(-1);
                nlSize  = nlSize - nSize;
                nStep   = nStep + nSize;
               	if((pD->smsooa.numocts > MDB_MIN_SIZE) ||
						(pD->smsooa.numocts < 0))
				{
					log_write ( LEV_ALL_PARAM, "[%s] ORG ORG ADDR Size %d Over..Receive Size [%d]\n",
							__FILE__,MDB_MIN_SIZE, pD->smsooa.numocts);
					return(-1);
				}
				pD->m.smsooaPresent = 1;
				break;
			case SMS_DEST_ADDR_TAG	:
				nSize = decAddTypeString(&stMsg, &pD->smsda, SMS_DEST_ADDR_TAG);
				if(nSize < 0) return(-1);
                nlSize  = nlSize - nSize;
                nStep   = nStep + nSize;
               	if((pD->smsda.numocts > MDB_MIN_SIZE) ||
						(pD->smsda.numocts < 1))
				{
					log_write ( LEV_ALL_PARAM, "[%s] DEST ADDR Size %d Over..Receive Size [%d]\n",
							__FILE__,MDB_MIN_SIZE, pD->smsda.numocts);
					return(-1);
				}
				pD->m.smsdaPresent = 1;
				break;
			case SMS_NOTI_IND_TAG	:
				nSize = decOctetString(&stMsg, &pD->smsni, SMS_NOTI_IND_TAG);
				if(nSize < 0) return(-1);
                nlSize  = nlSize - nSize;
                nStep   = nStep + nSize;
                pD->m.smsniPresent = 1;
				break;

/********************************************************************************************
                        ///             MNP             ///
                        /// for IMSI in SMSREQIndicator ///
********************************************************************************************/
            case SMS_IMSI_TAG	:
                nSize = decIMSI(&stMsg, &pD->smsi, SMS_IMSI_TAG);
                if(nSize < 0) return(-1);
                nlSize  = nlSize - nSize;
                nStep   = nStep + nSize;
                pD->m.smsiPresent = 1;
                break;
/********************************************************************************************
 						///			SKT OSFI              ///
********************************************************************************************/
			case SMS_SKT_OSFI_TAG	:
				nSize = decOctetString(&stMsg, &pD->osfi, SMS_SKT_OSFI_TAG);
				if(nSize < 0)
					return(-1);
				nlSize  = nlSize - nSize;
				nStep   = nStep + nSize;
				if((pD->osfi.numocts > 4 + 1) || 
						(pD->osfi.numocts < 0))
				{
					log_write ( LEV_ALL_PARAM, "[%s]OSFI Size %d Over ...Receive Size[%d]\n",
							__FILE__, 4 + 1, pD->osfi.numocts);
					return(-1);
				}
				pD->m.osfiPresent = 1;
				break;
				
			case SMS_SKT_ROAMINDICAT_TAG	:
				nSize = decOctetString(&stMsg, &pD->smsroamindicat, SMS_SKT_ROAMINDICAT_TAG);
				if(nSize < 0)
					return(-1);
				nlSize  = nlSize - nSize;
				nStep   = nStep + nSize;
				pD->m.roamindicatPresent = 1;
				break;

			case SMS_SKT_ROAMING_PMN_TAG	:
				nSize = decOctetString(&stMsg, &pD->smsroampmn, SMS_SKT_ROAMING_PMN_TAG);
				if(nSize < 0)
					return(-1);
				nlSize	= nlSize - nSize;   
				nStep   = nStep + nSize;
				pD->m.roampmnPresent = 1;
				break;
				
			default		:
              	nSize = parseDummy("decSMDPP", &stMsg, nRet);
                if(nSize < 0) return(-1);
                nlSize  = nlSize - nSize;
                nStep   = nStep + nSize;
                break;
		}
	}
	return(1);
}
