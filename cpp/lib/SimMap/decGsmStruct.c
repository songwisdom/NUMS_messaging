#include "flk_type.h"
#include "libMapComm.h"
#include "libGsmTpduStruct.h"
#include "flk_iconv.h"
#include "Debug.h"

void	printSMSDeliver(TPSMSDeliver pdu);
void	printSMSCommand(TPSMSCommand pdu);
void	printSMSDeliverReport(TPSMSDeliverReport pdu);
void	printSMSStatusReport(TPSMSStatusReport pdu);
char*	decParamBCDCallBackAddress(unsigned char *data,int cnt);
int		decParamTPCallBackAddress(unsigned char *data, TP_OAParam *Addr);
int decUD_RemakeUD(unsigned char *cpUserData, TPSMSDeliver *spSubmit, UDH_Struct *spUDH);
void changeGSMtoASCII8bit(stGSM7bitString *spGsm);


/*
int		decParamTPAddress(unsigned char *data, TP_OAParam *Addr)
{
	int		idx=0, remain=0, addrPt=0;
	SemiOctet	*pso=NULL;

	if(data== NULL || Addr == NULL)
	{
		setGSMTPDUErrorCode(SET_INVALID_VALUE);
		return TPDU_FALSE;
	}

	Addr->addrLen = data[idx++];
	Addr->pAddrIdc = data[idx++];		

	Addr->addrOctetSize = Addr->addrLen;
	Addr->addrOctetSize += (Addr->addrOctetSize%2);
	Addr->addrOctetSize = Addr->addrOctetSize/2;
	
	remain = Addr->addrOctetSize;

	if(Addr->addrOctetSize > 11 || Addr->addrLen < 2 || Addr->addrLen > 20) 
	{
		unsigned int failLen = Addr->addrLen;
        unsigned int octetLen = Addr->addrOctetSize;
        log_write ( LEV_ALL_PARAM, "[%s:%d] TPDU Address Length Fail octetSize[%d] len[%d]", 
                __func__, __LINE__, octetLen, failLen);
		setGSMTPDUErrorCode(ERR_TPDU_ADDRESS_LEN);
		return TPDU_FALSE;
	}

	while(remain)
	{
		pso = (SemiOctet*)&data[idx++];
//		Addr->address[addrPt++].digit = data[idx++];
		Addr->address[addrPt].digit1st = pso->digit2nd; 
		Addr->address[addrPt++].digit2nd = pso->digit1st; 
		remain--;
	}
	return	idx;	
}
int     decParamTPAddress2(unsigned char *data, TP_OAParam *Addr){
    int     idx=0, remain=0, addrPt=0;
    SemiOctet   *pso=NULL;

    if(data== NULL || Addr == NULL){
        return -1;
    }

	memset((SemiOctet*)&Addr->address,0xFF,TPDU_ADDRESS_LENGTH);
    Addr->addrLen = data[idx++];
	
    Addr->pAddrIdc = data[idx++];
	Addr->pAddrType = 0;
	Addr->pAddrNumPlan = 2;
	

    Addr->addrOctetSize = Addr->addrLen;
    Addr->addrOctetSize += (Addr->addrOctetSize%2);
    Addr->addrOctetSize = Addr->addrOctetSize/2;
    remain = Addr->addrOctetSize;
    if(Addr->addrOctetSize > 11 || Addr->addrLen < 2 || Addr->addrLen > 20) {
            return -1;
    }
    if(data[idx] != 0x29){
        while(remain){
            pso = (SemiOctet*)&data[idx++];
            Addr->address[addrPt].digit1st = pso->digit2nd;
            Addr->address[addrPt++].digit2nd = pso->digit1st;
            remain--;
        }
    }
    else{
        remain -= 1;
		Addr->addrLen -=1;
        idx++;
        pso = (SemiOctet*)&data[idx++];
        Addr->address[addrPt].digit2nd = 0x0A;
		if(pso->digit1st==0) Addr->address[addrPt++].digit1st = 0x0A;
        Addr->address[addrPt++].digit1st = pso->digit1st;
        while(remain){
			if(pso->digit2nd ==0x0f) break;
			if(pso->digit2nd==0) Addr->address[addrPt].digit2nd = 0x0A;
			else
            	Addr->address[addrPt].digit2nd = pso->digit2nd;
            if(pso->digit1st == 0x0f) break;
            pso = (SemiOctet*)&data[idx++];
			if(pso->digit1st==0) Addr->address[addrPt++].digit1st =0x0A;
			else
            	Addr->address[addrPt++].digit1st = pso->digit1st;
            remain--;
        }
    }
    return  Addr->addrOctetSize;
}
*/
char *decParamBCDAddress(unsigned char *data,int cnt)
{
	static char buf[256];
	int pos;

	buf[0] = '\0';

	for (pos = 0; pos < cnt; pos++)
	{
		sprintf(buf, "%s%02x", buf, data[pos]);
	}

	return buf;
	/*
	SemiOctet		*pSo=NULL;
	int		i=0,idx=0;
	static	char bcdBuff[1024];
	memset((char*)&bcdBuff,0x00,1024);

	for(i=0;i<cnt;i++)
	{
		pSo = (SemiOctet*)&data[i];
		bcdBuff[idx] = getParamAddrChar(pSo->digit2nd);
		bcdBuff[idx+1] = getParamAddrChar(pSo->digit1st);
		idx += 2; 
	}

	return (bcdBuff);
	*/
}
#if 0
void changeGSMtoASCII8bit(stGSM7bitString *spGsm)
{
	int nIndex1=0; 
	stASCIIString stTemp;

	bzero(&stTemp, sizeof(stASCIIString));

	/* nCharacter is real octet size */
	for(nIndex1=0; nIndex1 < spGsm->nCharacter; nIndex1++)
	{
		switch(spGsm->data[nIndex1])
		{
			case 0x00:
				stTemp.data[stTemp.nCharacter++] = '@';	//	0x40 
				break;
			case 0x02:
				stTemp.data[stTemp.nCharacter++] = '$';	//	0x24 
				break;
			case 0x11: 
				stTemp.data[stTemp.nCharacter++] = '_';	//	0x5F 
				break;
			case 0x1b:
				switch(spGsm->data[++nIndex1])					
				{
					case 0x3C: 
						stTemp.data[stTemp.nCharacter++] = '[';	// 0x5B 
						break;
					case 0x2F:
						stTemp.data[stTemp.nCharacter++] = 0x5C; // '\' 
						break;
					case 0x3E:
						stTemp.data[stTemp.nCharacter++] = ']'; // 0x5D
						break;
					case 0x14:
						stTemp.data[stTemp.nCharacter++] = '^'; // 0x5E
						break;
					case 0x28:
						stTemp.data[stTemp.nCharacter++] = '{'; // 0x5C
						break;
					case 0x40:
						stTemp.data[stTemp.nCharacter++] = '|'; // 0x7C
						break;
					case 0x29:
						stTemp.data[stTemp.nCharacter++] = '}'; // 0x7D
						break;
					case 0x3D:
						stTemp.data[stTemp.nCharacter++] = '~'; // 0x7E
						break;
					default :
						stTemp.data[stTemp.nCharacter++] = spGsm->data[--nIndex1];
						break;
				}
				break;
			default :
				stTemp.data[stTemp.nCharacter++] = spGsm->data[nIndex1];
				break;
		}
	}

	bzero(spGsm, sizeof(stGSM7bitString));
	memcpy(spGsm, &stTemp, sizeof(stGSM7bitString));
}
#endif
int dec8bitUD(TPSMSDeliver *submit, unsigned char *pt, int *remain)
{
	int             nIdx    = 0;
	int             nIEIL   = 0;
	int             nStep   = 0;
	char            cIEITag = 0x00;
	unsigned char   cpTempUD[1024];
	UDH_Struct      stUDH;

	/* Arg check */
	if(submit == NULL)
	{
		log_write ( LEV_ALL_PARAM, "[%s:%d] submit point is NULL! return error! ",
				__func__, __LINE__);
		return TPDU_FALSE;
	}
	if(pt == NULL)
	{
		log_write ( LEV_ALL_PARAM, "[%s:%d] data point is NULL! return error! ",
				__func__, __LINE__);
		return TPDU_FALSE;
	}
	if(remain == NULL)
	{
		log_write ( LEV_ALL_PARAM, "[%s:%d] remain point is NULL! return error! ",
				__func__, __LINE__);
		return TPDU_FALSE;
	}

	/* UDH exist check */
	if(submit->tpUDHI.value)
	{
		int             nIdx    = 0;
		int             nIEIL   = 0;
		int             nStep   = 0;
		int             isUDHL  = 0;
		char            cIEITag = 0x00;
		unsigned char   cpTempUD[1024];
		UDH_Struct      stUDH;

		*remain-=1;

		/* init */
		memset(cpTempUD, 0x00, 1024);
		memset((UDH_Struct*)&stUDH, 0x00, sizeof(UDH_Struct));
		memcpy(cpTempUD, pt, submit->tpUDL);
		isUDHL = *pt;
		if(!isUDHL)
		{
			log_write ( LEV_ALL_PARAM, "UDHI set, but UDHL is zero!");
			return TPDU_FALSE;
		}
		/* Check UDHL and UDL */
		if(isUDHL > submit->tpUDL)
		{
			log_write ( LEV_ALL_PARAM, "[%s:%d] UDHL[%d] is UDL[%d] size over!",
					__func__, __LINE__, 
					isUDHL, submit->tpUDL);
			return TPDU_FALSE;
		}
		if(FLK_FAIL == decUD_RemakeUD(cpTempUD, submit, &stUDH))
		{
			log_write ( LEV_ALL_PARAM, "[%s:%d] UDH decoding error! ", 
					__func__, __LINE__);
			return TPDU_FALSE;
		}
		int i=0, UDHI_check=0;
		for(i=0; i<stUDH.nIEI_Cnt; i++)
		{
			if(stUDH.stIEI[i].ucIEI_Exist_Flag == true)
			{
				UDHI_check++;
			}
		}
        if(!UDHI_check)
		{
			submit->tpUDHI.value = TPDU_ONLY_SHORT_MSG;
		}
		*remain = 0;
	}
	else
	{
		if(submit->tpUDL > 0)
		{
			memcpy(submit->tpUD.data, pt, submit->tpUDL);
			submit->tpUD.size = submit->tpUDL;
			*remain -= submit->tpUDL;
		}
	}
	return TPDU_TRUE;
}
void ConvGSM7bit28bit(stGSM7bitString *spSource, stASCIIString *spTarget)
{
	int nIndex1 = 0;
	int nIndex2 = 0;

	/* spSource->nCharacter is total character. 	*/
	/* spTarget->nOctet is real octet size. 		*/ 
	spTarget->nCharacter = spSource->nCharacter;
	spSource->nOctet = spTarget->nOctet = (spSource->nCharacter*7)/8;

    if(((spSource->nCharacter*7)%8) > 0)
	{
		spSource->nOctet = spTarget->nOctet = spTarget->nOctet+1;
	}

#if 0
	DebugStr(4, "[%s:%d] Test Log Start \n", __FILE__, __LINE__);
	//debugStr(spSource->data, spSource->nOctet, 10);	
	DebugStr(4, "[%s:%d] Test Log End \n", __FILE__, __LINE__);
#endif 

	while (spTarget->nCharacter > nIndex1)
	{
		int nShift = nIndex2%8;
		spTarget->data[nIndex2] = (spSource->data[nIndex1] << nShift) & 0x7f;

		if (nShift != 0)
		{
			spTarget->data[nIndex2] |= (spSource->data[nIndex1-1] >> (8-nShift));
			nIndex1++;

		}
		else if (nIndex1 != 0)
		{
			spTarget->data[nIndex2] = (spSource->data[nIndex1-1] << nShift) & 0x7f;
		}
		else
		{
			nIndex1++;
		}
		nIndex2++;
	}
#if 0
	DebugStr(4, "[%s:%d] Test Log Start \n", __FILE__, __LINE__);
	//debugStr(spTarget->data, spTarget->nCharacter, 10);	
	DebugStr(4, "[%s:%d] Test Log End \n", __FILE__, __LINE__);
#endif 
	changeGSMtoASCII8bit(spTarget);
}
int dec7bitUDreMake(unsigned char *cpUD, int *nCharNum)
{
	int 			nStartP			= 0;
	unsigned int 	nRealUDLOctet	= 0;
	int 			nUDHCharNum		= 0;
	int 			nUDCharNum		= 0;
	int 			nUDHL			= 0;	
	int 			nShiftP 		= 0;
	unsigned char 	ucUDH[BUF_SSIZE];
	unsigned char   cpReMakeUD[BUF_SSIZE];
	unsigned char   cpTempUD[BUF_SSIZE];
    stGSM7bitString stSource;
	stASCIIString   stTarget;

    /* Init */
	bzero(ucUDH, BUF_SSIZE);	// 256 byte
	bzero(cpReMakeUD, BUF_SSIZE);	// 256 byte
	bzero(cpTempUD, BUF_SSIZE); // 256 byte
	bzero(&stSource, sizeof(stGSM7bitString));
	bzero(&stTarget, sizeof(stASCIIString));

	/* Buffer check */
	if(cpUD == NULL)
	{
		return FLK_FAIL;
	}

	/* UDHL */
	nUDHL = (int)cpUD[0];
//	DebugStr(4, "[%s:%d] Test nUDHL = [%d] \n", __FILE__, __LINE__, nUDHL);
	/* UD copy ucUDH buffer */ 
	memcpy(ucUDH, cpUD, (nUDHL+1));

	/* Get UDHL character number */
	nUDHCharNum = (((nUDHL+1)*8)/7);
//	DebugStr(4, "[%s:%d] Test nUDHCharNum1 = [%d] \n", __FILE__, __LINE__, nUDHCharNum);
	if(((nUDHL+1)*8)%7 > 0)
	{
		nUDHCharNum++;
	}
//	DebugStr(4, "[%s:%d] Test nUDHCharNum2 = [%d] \n", __FILE__, __LINE__, nUDHCharNum);
//	DebugStr(4, "[%s:%d] Test nCharNum = [%d] \n", __FILE__, __LINE__, *nCharNum);
	/* Get RealUDL octet number */
	nUDCharNum = *nCharNum - nUDHCharNum; // (UD Char Number - UDH Char Number) 
//	DebugStr(4, "[%s:%d] Test nUDCharNum = [%d] \n", __FILE__, __LINE__, nUDCharNum);
	nRealUDLOctet = (nUDCharNum*7)/8;
	if((nUDCharNum*7)%8)
	{
		nRealUDLOctet++;
	}

//	DebugStr(4, "[%s:%d] Test nRealUDLOctet = [%d] \n", __FILE__, __LINE__, nRealUDLOctet);

	/* Get Shift start point */
	nShiftP = (nUDHCharNum*7) - ((nUDHL+1)*8); 	

	unsigned int nPos = (nShiftP == 0) ? 0 : 1;

	/* UD copy cpTempUD buffer */
	if(nRealUDLOctet+nPos > 140)
	{
		return FLK_FAIL;
	}
	memcpy(cpTempUD, &cpUD[nUDHL+1], nRealUDLOctet+nPos);

	int nIndex1=0, nIndex2=0;	
	while(nRealUDLOctet > nIndex1)
	{
		cpReMakeUD[nIndex1] = (cpTempUD[nIndex2++] >> nShiftP); 
		if(nShiftP != 0)
		{
			cpReMakeUD[nIndex1++] |= (cpTempUD[nIndex2] << (8-nShiftP));
		}
		else
		{
			nIndex1++;
		}
	}

	stSource.nCharacter = nUDCharNum;
	memcpy(stSource.data, cpReMakeUD, nRealUDLOctet);
//#ifdef DEBUG
	log_write(LEV_ALL_PARAM, "[%s:%d] Test  stSource"
			"[%02x][%02x][%02x][%02x][%02x][%02x]"
			"[%02x][%02x][%02x][%02x][%02x][%02x] C[%d] O[%d]\n",
			__FILE__, __LINE__,
			stSource.data[0], stSource.data[1], stSource.data[2], 
			stSource.data[3], stSource.data[4], stSource.data[5], 
			stSource.data[6], stSource.data[7], stSource.data[8], 
			stSource.data[9], stSource.data[10], stSource.data[11],
			stSource.nCharacter, stSource.nOctet);
//#endif // DEBUG
	/* convert */ 
	ConvGSM7bit28bit(&stSource, &stTarget);
//#ifdef DEBUG
	log_write(LEV_ALL_PARAM, "[%s:%d] Test stTarget"
			"[%02x][%02x][%02x][%02x][%02x][%02x]"
			"[%02x][%02x][%02x][%02x][%02x][%02x] C[%d] O[%d]\n",
			__FILE__, __LINE__,
			stTarget.data[0], stTarget.data[1], stTarget.data[2], 
			stTarget.data[3], stTarget.data[4], stTarget.data[5], 
			stTarget.data[6], stTarget.data[7], stTarget.data[8], 
			stTarget.data[9], stTarget.data[10], stTarget.data[11],
			stTarget.nCharacter, stTarget.nOctet);
//#endif // DEBUG

	memcpy(&ucUDH[nUDHL+1], stTarget.data, stTarget.nCharacter);
	*nCharNum = ((nUDHL+1) + stTarget.nCharacter); 
	bzero(cpUD, BUF_SSIZE);
	memcpy(cpUD, ucUDH, *nCharNum);
	return FLK_SUCCESS;
}
int decGsm7bitUD(TPSMSDeliver *submit, unsigned char *pt, int *remain)
{
	int             nIdx    = 0;
	int             nIEIL   = 0;
	int             nStep   = 0;
	char            cIEITag = 0x00;
	UDH_Struct      stUDH;
	stGSM7bitString stSource;
	stASCIIString   stTarget;

	/* Arg check */
	if(submit == NULL)
	{
		log_write ( LEV_ALL_PARAM, "[%s:%d] submit point is NULL! return error! ",
				__func__, __LINE__);
		return TPDU_FALSE;
	}
	if(pt == NULL)
	{
		log_write ( LEV_ALL_PARAM, "[%s:%d] data point is NULL! return error! ",
				__func__, __LINE__);
		return TPDU_FALSE;
	}
	if(remain == NULL)
	{
		log_write ( LEV_ALL_PARAM, "[%s:%d] remain point is NULL! return error! ",
				__func__, __LINE__);
		return TPDU_FALSE;
	} 

    /* Init */
	bzero(&stSource, sizeof(stGSM7bitString));
	bzero(&stTarget, sizeof(stASCIIString));

	/* UDH exist check */
	if(submit->tpUDHI.value)
	{
		int             nIdx    = 0;
		int             nIEIL   = 0;
		int             nStep   = 0;
		int             isUDLChar 	= 0;
		int             isUDHL  = 0;
		char            cIEITag = 0x00;
		unsigned char   cpTempUD[BUF_SSIZE];
		UDH_Struct      stUDH;

		(*remain)--;

		/* init */
		bzero(cpTempUD, BUF_SSIZE);
		
		memset((UDH_Struct*)&stUDH, 0x00, sizeof(UDH_Struct));
		memcpy(cpTempUD, pt, submit->tpUDL);

		isUDHL = *pt;
		if(!isUDHL)
		{
			log_write ( LEV_ALL_PARAM, "UDHI set, but UDHL is zero!");
			return TPDU_FALSE;
		}
		/* UDL UDHL  */
		//if(isUDHL > submit->tpUDL)
		if(isUDHL > (BIT8_UD_MAX_SIZE - 1))
		{
			log_write ( LEV_ALL_PARAM, "[%s:%d] UDHL[%d] is UDL[%d] size over!",
					__func__, __LINE__, 
					isUDHL, submit->tpUDL);
			return TPDU_FALSE;
		}

        /* 8bit data remake */
		isUDLChar = (int)submit->tpUDL;
		if(dec7bitUDreMake(cpTempUD, &isUDLChar) == FLK_FAIL)		
		{
			log_write ( LEV_ALL_PARAM, "[%s:%d] dec7bitUDreMake() Fail! return error!", 
					__func__, __LINE__);
			return TPDU_FALSE;
		}
	
		submit->tpUDL = (u_char)isUDLChar;
		if(decUD_RemakeUD(cpTempUD, submit, &stUDH) == FLK_FAIL)
		{
			return TPDU_FALSE;
		}

		int i=0, UDHI_check=0;
		for(i=0; i<stUDH.nIEI_Cnt; i++)
		{
			if(stUDH.stIEI[i].ucIEI_Exist_Flag == true)
			{
				UDHI_check++;
			}
		}
		if(!UDHI_check)
		{
			submit->tpUDHI.value = TPDU_ONLY_SHORT_MSG;
		}
		*remain = 0;
	}
	else 
	{
		if(submit->tpUDL > 0)
		{
			stSource.nCharacter = submit->tpUDL;
			memcpy(stSource.data, pt, submit->tpUDL);
			/* convert */ 
			ConvGSM7bit28bit(&stSource, &stTarget);
			memcpy(submit->tpUD.data, stTarget.data, stTarget.nCharacter);
			submit->tpUD.size = submit->tpUDL = stTarget.nCharacter;
			*remain -= stSource.nOctet;
		}
	}
	return TPDU_TRUE;
}
/*************************************************************************************
 *
 *  SMS Deliver Decoding Function
 *
 ************************************************************************************/
int		decSMSDeliver(TPSMSDeliver *arg,unsigned char *data,int size)
{
	int				idx=0,remain=0,nByte=0,alen=0;
	unsigned char	*pt = NULL;
	unsigned char	tmp[512];
	unsigned char 	addrbuf[64];
	TPSMSDeliver	deliver;
	int				is8bit=0;

	memset((TPSMSDeliver*)&deliver,0x00,sizeof(TPSMSDeliver));
	memset((unsigned char*)&tmp,0x00,512);
	memset((unsigned char*)&addrbuf,0xFF,64);
	memset((TPSMSDeliver*)&deliver,0xFF,sizeof(TPSMSDeliver));
	memset((TPSMSDeliver*)&deliver.tpUD,0x00,sizeof(deliver.tpUD));
	
	remain = size;
	log_write ( LEV_ALL_PARAM,"\ndecSMSDeliver()\t\t\t\t\t[START %d]\n",remain); 

	if(data == NULL || size <= 0)
	{
		log_write ( LEV_ALL_PARAM,"ERROR: decSMSDeliver()\t\t[Invalid Argument]\n");
		setGSMTPDUErrorCode(SET_INVALID_VALUE);	
		log_write ( LEV_ALL_PARAM, "decSMSDelivery Argument is invalid\n");
		return TPDU_FALSE;
	}
	pt = data;
	/* ---------------------------------------------------------------------------*/
	/*		Decoding Indicator Octet											  */	
	/* ---------------------------------------------------------------------------*/
	deliver.idc.charval = *pt;

	SetMTIParameter(deliver,deliver.idc.mti.value);
	SetMMSParameter(deliver,deliver.idc.mms.value);
	SetRPParameter(deliver,deliver.idc.rp.value);
	SetSRIParameter(deliver,deliver.idc.sri.value);
	SetUDHIParameter(deliver,deliver.idc.udhi.value);
	log_write( LEV_ALL_PARAM, "\th'%02X MTI(M) [%d], MMS(M) [%d],"
			" RP(M) [%d], SRI(O) [%d], UDHI(O) [%d]\n", *pt,
			GetMTIValue(deliver), GetMMSValue(deliver), GetRPValue(deliver),
			GetSRIValue(deliver), GetUDHIValue(deliver));

	deliver.optional.SRI = GetSRIValue(deliver);
	deliver.optional.UDHI= GetUDHIValue(deliver);

	if(GetMTIValue(deliver) != TPDU_SMS_DELIVER){
		log_write ( LEV_ALL_PARAM,"ERROR: NOT SMS DELIVER PDU [%d]\n",
			GetMTIValue(deliver));
		setGSMTPDUErrorCode(DEC_INVALID_FORMAT);
		log_write ( LEV_ALL_PARAM, "decSMSDelivery Not Delivery TPDU [%d]\n", GetMTIValue(deliver));
		return TPDU_FALSE;	
	}
	pt++;
	alen = *pt;

	if(alen%2==1)
	{
		/* ODD */
		memcpy((unsigned char*)&addrbuf,pt,(alen/2)+3);
	}
	else 
	{
		/* EVEN */
		memcpy((unsigned char*)&addrbuf,pt,(alen/2)+2);
	}
	remain--;

	/* ---------------------------------------------------------------------------*/
	/*		Decoding TP-OA Parameter											  */	
	/* ---------------------------------------------------------------------------*/
	alen = decParamTPAddress(pt,&deliver.tpADDRk);

/* callback for new add 2006-01-21 */
    idx = decParamTPAddress2((unsigned char*)&addrbuf,&deliver.tpADDRc); 
    if(idx == TPDU_FALSE)
	{
        log_write ( LEV_ALL_PARAM,"ERROR: TP-OA Decoding Fail]\n");
        //return TPDU_FALSE;
    }else {
    log_write ( LEV_ALL_PARAM,"\tTP-OA Address  for Callback     [%s] \n",
        decParamBCDAddress((u_char *)GetADDRNumber2(deliver),GetADDRSize2(deliver)));
/* end 2006-01-21 */

		idx = decParamTPAddress3((unsigned char*)&addrbuf,&deliver.tpADDR);
		if(idx == TPDU_FALSE)
		{
			log_write ( LEV_ALL_PARAM,"ERROR: TP-OA Decoding Fail]\n");
			//return TPDU_FALSE;
		}
	log_write ( LEV_ALL_PARAM,"\tTP-OA Address Length [%d]\n",GetADDRLen(deliver));
	log_write ( LEV_ALL_PARAM,"\tTP-OA Address Type   [%d]\n",GetADDRTON(deliver));
	log_write ( LEV_ALL_PARAM,"\tTP-OA Numbering Plan [%d]\n",GetADDRNPI(deliver));
	log_write ( LEV_ALL_PARAM,"\tTP-OA Address        [%s] \n",
		decParamBCDAddress((u_char *)GetADDRNumber(deliver),GetADDRSize(deliver)));
	}

	pt += alen;
	remain -=alen; 
	/* ---------------------------------------------------------------------------*/
	/*		Decoding TP-PID Parameter											  */	
	/* ---------------------------------------------------------------------------*/
	tmp[0] = *pt;
	SetPIDParameter(deliver,tmp[0]);
	pt++;
	remain--;

	log_write ( LEV_ALL_PARAM,"\tTP-PID Type [%d]\n",GetPIDType(deliver.tpPID));
	if(GetPIDType(deliver.tpPID) == 0)
	{
        log_write ( LEV_ALL_PARAM,"\tTP-PID Interworking  [%d]\n",
            GetPIDInterworking(deliver.tpPID));
        log_write ( LEV_ALL_PARAM,"\tTP-PID Value         [%d]\n",
            GetPIDValueItw(deliver.tpPID));
    }
    else
	{
        log_write ( LEV_ALL_PARAM,"\tTP-PID Value         [%d]\n",
			GetPIDValueNoItw(deliver.tpPID));
	}
	
	/* ---------------------------------------------------------------------------*/
	/*		Decoding TP-DCS Parameter											  */	
	/* ---------------------------------------------------------------------------*/
	tmp[0] = *pt;
	SetDCSParameter(deliver,tmp[0]);
	pt++;	
	remain--;

	/*
	log_write(LEV_ALL_PARAM, "DCS [0x%02X]\n", deliver.tpDCS);
	switch (deliver.tpDCS) {
		case  0x00 : // Gsm7bit
			is8bit = 0;
			break;
		case 0x15 : // Gsm8bit
			is8bit = 0;
			break;
		case 0x08 : // UCS2 
			break;
		case 0x84 : // KSC5601
		default :
			break;
	}
	*/

	/*
	if(IsIncCharsetDcs(deliver.tpDCS))
	{
	}
	*/
	is8bit = GetDCSCharset(deliver.tpDCS);	

	log_write ( LEV_ALL_PARAM,"\tTP-DCS Group         [%d]\n",GetDCSGroup(deliver.tpDCS));
	if(IsIncCharsetDcs(deliver.tpDCS))
	{
        log_write ( LEV_ALL_PARAM,"\tTP-DCS Charset   [%d]\n",GetDCSCharset(deliver.tpDCS));
        log_write ( LEV_ALL_PARAM,"\tTP-DCS Class     [%d]\n",GetDCSClass(deliver.tpDCS));
    }
    else
	{
        log_write ( LEV_ALL_PARAM,"\tTP-DCS Sense     [%d]\n",GetDCSSense(deliver.tpDCS));
        log_write ( LEV_ALL_PARAM,"\tTP-DCS Indicator Type [%d]\n",
            GetDCSIdcType(deliver.tpDCS));
    }	

	/* ---------------------------------------------------------------------------*/
	/*		Decoding TP-SCTS Parameter											  */	
	/* ---------------------------------------------------------------------------*/
	SetSCTSParameter(deliver,getParamSCTSValue(pt));
	pt += TPVP_MAXSIZE;
	remain -= TPVP_MAXSIZE;
	log_write ( LEV_ALL_PARAM,"\tTP-SCTS     [H' %02X %02X %02X %02X %02X %02X %02X]\n",
		GetSCTSValueIdx(deliver.tpSCTS,0),GetSCTSValueIdx(deliver.tpSCTS,1),
		GetSCTSValueIdx(deliver.tpSCTS,2),GetSCTSValueIdx(deliver.tpSCTS,3),
		GetSCTSValueIdx(deliver.tpSCTS,4),GetSCTSValueIdx(deliver.tpSCTS,5),
		GetSCTSValueIdx(deliver.tpSCTS,6));

	/* ---------------------------------------------------------------------------*/
	/*		Decoding TP-UDL Parameter											  */	
	/* ---------------------------------------------------------------------------*/
	if(remain < 0)
	{
		log_write ( LEV_ALL_PARAM,"\n\nDECODING ERROR %d\n",remain);
		setGSMTPDUErrorCode(DEC_INVALID_FORMAT);            
		log_write ( LEV_ALL_PARAM, "Invalid Data Format[input size %d\n",size);
		return TPDU_FALSE;
	}
	deliver.tpUDL = *pt; 
	remain --;
	pt++;

	log_write ( LEV_ALL_PARAM,"\tTP-UDL UD Length  [%d]\n", deliver.tpUDL);
	if((!is8bit) && (deliver.tpUDL > BIT7_UD_MAX_SIZE))
	{
		setGSMTPDUErrorCode(DEC_DATASIZE_ERROR);
		return TPDU_FALSE;
	}
	else if((is8bit) && (deliver.tpUDL > BIT8_UD_MAX_SIZE))
	{
		setGSMTPDUErrorCode(DEC_DATASIZE_ERROR);
		return TPDU_FALSE;
	}

	/* ---------------------------------------------------------------------------*/
	/*		Decoding TP-UD Parameter											  */	
	/* ---------------------------------------------------------------------------*/
	if(deliver.tpUDL == 0)
	{
		SetUDSize2(deliver,0);	
		if(remain != 0)
		{
			log_write ( LEV_ALL_PARAM,"\n\nDECODING ERROR %d\n",remain);
			setGSMTPDUErrorCode(DEC_INVALID_FORMAT);
			log_write ( LEV_ALL_PARAM, "decSMSCommand TPDU Message Length Invalid "
					"[InputSize %d, Remain %d]\n", size,remain);
			return TPDU_FALSE;
		}
		printSMSDeliver(deliver);
		return TPDU_TRUE;
	}

	log_write(LEV_ALL_PARAM, "TP-DCS [%d]\n", is8bit);

	switch ( is8bit ) {
        case TPDU_GSM7BIT_DEFAULT:
            if(GetDCSGroup(deliver.tpDCS) == TPDU_RESERVED_8_CLASS)
            {
                /* For SKT, 0x80 is ASCII 8bit */
                if(dec8bitUD(&deliver, pt, &remain) == TPDU_FALSE)
                {
                    log_write(LEV_ALL_PARAM,"\n\nDECODING ERROR %d\n", remain);
                    setGSMTPDUErrorCode(DEC_INVALID_FORMAT);
                    return TPDU_FALSE;
                }
            }
            else
            {
                /* GSM 7bit decoding */
                if(decGsm7bitUD(&deliver, pt, &remain) == TPDU_FALSE)
                {
                    log_write(LEV_ALL_PARAM,"\n\nDECODING ERROR %d \n", remain);
                    setGSMTPDUErrorCode(DEC_INVALID_FORMAT);
                    return TPDU_FALSE;
                }
            }           
            break;      
        case TPDU_8BIT_DATA :   
        case TPDU_UCS2 :
        default :       
            if(dec8bitUD(&deliver, pt, &remain) == TPDU_FALSE)
            {
                log_write(LEV_ALL_PARAM,"\n\nDECODING ERROR %d \n", remain);
                setGSMTPDUErrorCode(DEC_INVALID_FORMAT);
                return TPDU_FALSE;
            }
            break;
	}

#if 0
	/* GSM 7bit decoding */
	if(!is8bit)
	{
		pt++;
		/* <-- 2006-01-03 add start*/
		if(GetUDHIValue(deliver))
		{
			idx = dec7to8bitsUDH(pt, (unsigned char *)tmp, remain);
            if(deliver.tpUDL!= idx)
			{
				log_write(LEV_ALL_PARAM, "\n\t[UDHI Existing] TPDU WARRING:"
						" TPDU SIZE [%d]\n",size);
				log_write(LEV_ALL_PARAM, "\tTPDU DELIVER DECODING WARRING:"
						" UDL Value[%d(H'%02X)]-Decoding Result [%d]\n",
						deliver.tpUDL,deliver.tpUDL,idx);
				log_write(LEV_ALL_PARAM, "\tBut Continue processing ---> ");          
				idx = deliver.tpUDL;
            }
			log_write ( LEV_ALL_PARAM, "TPDU RECV SIZE [%d], "
					"UDL VALUE[%d(H'%02X)], UDSIZE[%d]\n",
					size, deliver.tpUDL, deliver.tpUDL, remain);	
			deliver.tpUDL = remain;
		}
		/* 2006-01-03 add end --> */
		else
		{
			idx = dec7to8bits(pt,(unsigned char*)tmp, remain);
            if( deliver.tpUDL!= idx)
			{
				log_write(LEV_ALL_PARAM, "\n\t[UDHI Non-Existing] TPDU WARRING:"
						" TPDU SIZE [%d]\n",size);
				log_write(LEV_ALL_PARAM, "\tTPDU DELIVER DECODING WARRING:"
						" UDL Value[%d(H'%02X)]" "-Decoding Result [%d]\n",
						deliver.tpUDL,deliver.tpUDL,idx);
				log_write(LEV_ALL_PARAM, "\tBut Continue processing ---> ");
				idx = deliver.tpUDL;
            }
			log_write ( LEV_ALL_PARAM,
					"TPDU RECV SIZE [%d], UDL VALUE[%d(H'%02X)], UDSIZE[%d]\n", 
					size, deliver.tpUDL, deliver.tpUDL, remain);	
			deliver.tpUDL = remain;
		}
		/* new samsung 06-01-19*/
		remain = 0;
	}

	if(deliver.tpUDL > 0)
	{
		if(!is8bit)
		{
			SetUDParameter(deliver,tmp,idx);
			SetUDSize2(deliver,idx);
			for ( int t = 0 ; t < deliver.tpUDL; ++t ) {
				if ( t % 10 == 0 ) log_write(LEV_ALL_PARAM, "\n");
				log_write(LEV_ALL_PARAM, "[%02X]", (u_char)deliver.tpUD.data[t]);
			}
		}
		else
		{
			pt++;
			SetUDParameter(deliver,tmp,deliver.tpUDL);
			SetUDSize(deliver,deliver.tpUDL);
			remain -= deliver.tpUDL;
			for ( int t = 0 ; t < deliver.tpUDL; ++t ) {
				if ( t % 10 == 0 ) log_write(LEV_ALL_PARAM, "\n");
				log_write(LEV_ALL_PARAM, "[%02X]", (u_char)deliver.tpUD.data[t]);
			}
			/*
			pt++;
			SetUDParameter(deliver,pt,deliver.tpUDL);
			SetUDSize(deliver,deliver.tpUDL);
			remain -= deliver.tpUDL;
			for ( int t = 0 ; t < deliver.tpUDL; ++t ) {
				if ( t % 10 == 0 ) log_write(LEV_ALL_PARAM, "\n");
				log_write(LEV_ALL_PARAM, "[%02X]", (u_char)deliver.tpUD.data[t]);
			}
			*/
		}
		if(idx == TPDU_FALSE) 
		{
			setGSMTPDUErrorCode(DEC_INVALID_FORMAT);
			return TPDU_FALSE;
		}
		deliver.optional.UD = 1;
	}	
#endif

	if(remain != 0)
	{
		log_write ( LEV_ALL_PARAM,"\n\nDECODING ERROR [%d]\n",remain);
		setGSMTPDUErrorCode(DEC_INVALID_FORMAT);
		log_write ( LEV_ALL_PARAM, "decSMSDelivery TPDU Message Length"
				" Invalid [InputSize %d, Remain %d]\n",
				size,remain);
		return TPDU_FALSE;
	}
	memcpy((TPSMSDeliver*)arg, (TPSMSDeliver*)&deliver, 
			sizeof(TPSMSDeliver));
	
	printSMSDeliver(deliver);
	return TPDU_TRUE;
}

/*************************************************************************************
 *
 *  SMS DeliverReport Decoding Function
 *
 ************************************************************************************/
int		decSMSDeliverReport(TPSMSDeliverReport *arg,unsigned char *data,
	int size,int isErr){
	int		idx=0,remain=0,nByte=0;
	unsigned char	*pt = NULL,val=0;
	unsigned char	tmp[512];

	TPSMSDeliverReport	dlReport;
	int		is8bit=0;
	
	memset((TPSMSDeliverReport*)&dlReport,0x00,sizeof(TPSMSDeliverReport));
	memset((unsigned char*)&tmp,0x00,512);
	memset((TPSMSDeliverReport*)&dlReport,0x00,sizeof(TPSMSDeliverReport));
	
	remain = size;
	log_write ( LEV_ALL_PARAM,"\ndecSMSDeliverReport()\t\t\t\t\t[START %d]\n",remain); 

	if((data == NULL) || (size <= 0))
	{
		log_write ( LEV_ALL_PARAM,"ERROR: decSMSDeliverReport()\t\t[Invalid Argument]\n");
		setGSMTPDUErrorCode(DEC_INVALID_FORMAT);	
		log_write ( LEV_ALL_PARAM, "decSMSDeliveryReport Argument is invalid \n");
		return TPDU_FALSE;
	}
	pt = data;
	/* ---------------------------------------------------------------------------*/
	/*		Decoding Indicator Octet											  */	
	/* ---------------------------------------------------------------------------*/
	dlReport.idc.charval = *pt;

	SetMTIParameter(dlReport,dlReport.idc.mti.value);
	SetUDHIParameter(dlReport,dlReport.idc.udhi.value);

	log_write ( LEV_ALL_PARAM,"\tMTI(M) [%d] UDHI(O) [%d] \n",
			GetMTIValue(dlReport), GetUDHIValue(dlReport));

	dlReport.optional.UDHI= GetUDHIValue(dlReport);

	if(GetMTIValue(dlReport) != TPDU_SMS_DELIVER_REPORT)
	{
		log_write ( LEV_ALL_PARAM,"ERROR: NOT SMS DELIVER_REPORT PDU [%d]\n",
				GetMTIValue(dlReport));
		setGSMTPDUErrorCode(DEC_INVALID_FORMAT);
		log_write ( LEV_ALL_PARAM, "decSMSDeliveryReport Not DeliveryReport TPDU [%d]\n",
				GetMTIValue(dlReport));
		return TPDU_FALSE;	
	}
	pt++;
	remain--;

	/* ---------------------------------------------------------------------------*/
	/*		Decoding TP-FCS Parameter											  */	
	/* ---------------------------------------------------------------------------*/
	if(isErr)
	{
		tmp[0]= *pt;
		SetFCSParameter(dlReport,tmp[0]);
		log_write ( LEV_ALL_PARAM,"\tTP-FCS [H' %02X]\n",
			GetFCSValue(dlReport));
		dlReport.optional.FCS = 1;
		pt++;
		remain--;
	}
	else
	{
		dlReport.optional.FCS = 0;
	}

	if(remain == 0)	/* this case phase 1 and phase 2 */
	{
		if(!dlReport.optional.FCS) 
		{
			setGSMTPDUErrorCode(DEC_INVALID_FORMAT);
			log_write ( LEV_ALL_PARAM, "decSMSDeliveryReport Not Exist"
					" TP-FCS(Phase1 and Phase2 is Mandatory)\n");
			return TPDU_FALSE;
		}
		memcpy((TPSMSDeliverReport*)arg, (TPSMSDeliverReport*)&dlReport,
			sizeof(TPSMSDeliverReport));
    	printSMSDeliverReport(dlReport);
		return TPDU_TRUE;
	}
	/* This case phase 2++ */
	/* ---------------------------------------------------------------------------*/
	/*		Decoding TP-PI Parameter											  */	
	/* ---------------------------------------------------------------------------*/
	tmp[0] = *pt;
	SetPIParameter(dlReport,tmp[0]);
	dlReport.optional.PI = 1;

	dlReport.optional.PID = IsExistPID(dlReport.tpPI);
	dlReport.optional.UDL = IsExistUDL(dlReport.tpPI);
	dlReport.optional.DCS = IsExistDCS(dlReport.tpPI);

	pt++;
	remain--;
	/* ---------------------------------------------------------------------------*/
	/*		Decoding TP-PID Parameter											  */	
	/* ---------------------------------------------------------------------------*/
	if(IsExistPID(dlReport.tpPI))
	{
		tmp[0] = *pt;
		SetPIDParameter(dlReport,tmp[0]);
		pt++;
		remain--;
		log_write ( LEV_ALL_PARAM,"\tTP-PID Type [%d]\n",GetPIDType(dlReport.tpPID));
		if(GetPIDType(dlReport.tpPID) == 0)
		{
			log_write ( LEV_ALL_PARAM,"\tTP-PID Interworking  [%d]\n",
            	GetPIDInterworking(dlReport.tpPID));
        	log_write ( LEV_ALL_PARAM,"\tTP-PID Value         [%d]\n",
           		GetPIDValueItw(dlReport.tpPID));
    	}
    	else
       		log_write ( LEV_ALL_PARAM,"\tTP-PID Value         [%d]\n",
				GetPIDValueNoItw(dlReport.tpPID));
	}
	
	/* ---------------------------------------------------------------------------*/
	/*		Decoding TP-DCS Parameter											  */	
	/* ---------------------------------------------------------------------------*/
	if(IsExistDCS(dlReport.tpPI))
	{
		tmp[0] = *pt;
		SetDCSParameter(dlReport,tmp[0]);
		pt++;	
		remain--;

		if(IsIncCharsetDcs(dlReport.tpDCS))
			is8bit = GetDCSCharset(dlReport.tpDCS);	

		log_write ( LEV_ALL_PARAM,"\tTP-DCS Group         [%d]\n",
			GetDCSGroup(dlReport.tpDCS));
		if(IsIncCharsetDcs(dlReport.tpDCS))
		{
        	log_write ( LEV_ALL_PARAM,"\tTP-DCS Charset   [%d]\n",
				GetDCSCharset(dlReport.tpDCS));
        	log_write ( LEV_ALL_PARAM,"\tTP-DCS Class     [%d]\n",
				GetDCSClass(dlReport.tpDCS));
    	}
    	else
		{
        	log_write ( LEV_ALL_PARAM,"\tTP-DCS Sense     [%d]\n",
				GetDCSSense(dlReport.tpDCS));
        	log_write ( LEV_ALL_PARAM,"\tTP-DCS Indicator Type [%d]\n",
            	GetDCSIdcType(dlReport.tpDCS));
    	}
	}

	/* ---------------------------------------------------------------------------*/
	/*		Checking TP-UDL Parameter											  */	
	/* ---------------------------------------------------------------------------*/
	if(!IsExistUDL(dlReport.tpPI))
	{
		if(remain != 0)
		{
			log_write ( LEV_ALL_PARAM, "decSMSDeliveryReport TPDU Message Length"
					" Invalid [InputSize %d, Remain %d]\n",
					size,remain);
			setGSMTPDUErrorCode(DEC_INVALID_FORMAT);
			return	TPDU_FALSE;
		}
		memcpy((TPSMSDeliverReport*)arg,(TPSMSDeliverReport*)&dlReport,
				sizeof(TPSMSDeliverReport));
    	printSMSDeliverReport(dlReport);
		return TPDU_TRUE;
	}

	/* ---------------------------------------------------------------------------*/
	/*		Decoding TP-UDL Parameter											  */	
	/* ---------------------------------------------------------------------------*/
	dlReport.tpUDL = *pt; 
	remain --;
	log_write ( LEV_ALL_PARAM,"\tTP-UDL UD Length  [%d]\n", dlReport.tpUDL);

	if((!is8bit) && (dlReport.tpUDL > BIT7_UD_MAX_SIZE))
	{
		setGSMTPDUErrorCode(DEC_DATASIZE_ERROR);
		return TPDU_FALSE;
	}
	else if((is8bit) && (dlReport.tpUDL > BIT8_UD_MAX_SIZE))
	{
		setGSMTPDUErrorCode(DEC_DATASIZE_ERROR);
		return TPDU_FALSE;
	}

	/* ---------------------------------------------------------------------------*/
	/*		Decoding TP-UD Parameter											  */	
	/* ---------------------------------------------------------------------------*/
	if(dlReport.tpUDL == 0)
	{
		if(remain != 0)
		{
			log_write ( LEV_ALL_PARAM,"\n\nDECODING ERROR %d\n",remain);
			setGSMTPDUErrorCode(DEC_INVALID_FORMAT);
			log_write ( LEV_ALL_PARAM, "decSMSDeliveryReport TPDU Message Length"
					" Invalid [InputSize %d, Remain %d]\n", size,remain);
			return TPDU_FALSE;
		}
    	printSMSDeliverReport(dlReport);
		return TPDU_TRUE;
	}
	if(!is8bit)
	{
		pt++; 
        /* <-- 2006-01-03 add start*/
        if(GetUDHIValue(dlReport))
		{
            idx = dec7to8bitsUDH(pt,(unsigned char *)&tmp,remain);
            if(dlReport.tpUDL != idx)
			{
				log_write(LEV_ALL_PARAM, "TPDU WARRING: TPDU SIZE [%d]",size);
				log_write(LEV_ALL_PARAM, "\tTPDU DELIVER REPORT DECODING WARRING:"
						" UDL Value[%d(H'%02X)]-Decoding Result [%d]\n",
						dlReport.tpUDL, dlReport.tpUDL, idx);       
				log_write(LEV_ALL_PARAM, "\tBut Continue processing");          
                idx = dlReport.tpUDL;
            }
            log_write ( LEV_ALL_PARAM,
					"TPDU RECV SIZE [%d], UDL VALUE[%d(H'%02X)], UDSIZE[%d]\n",
					size, dlReport.tpUDL, dlReport.tpUDL, remain);
        	dlReport.tpUDL = remain;
        }
        /* 2006-01-03 add end --> */   
		else
		{
			idx = dec7to8bits(pt,(unsigned char*)&tmp,remain);
            if( dlReport.tpUDL!= idx)
			{
				log_write(LEV_ALL_PARAM, "TPDU WARRING: TPDU SIZE [%d]",size);
				log_write(LEV_ALL_PARAM, "\tTPDU DELIVER REPORT DECODING WARRING:"
						" UDL Value[%d(H'%02X)]-Decoding Result [%d]\n",
						dlReport.tpUDL,dlReport.tpUDL,idx);
				log_write(LEV_ALL_PARAM, "\tBut Continue processing");
                idx = dlReport.tpUDL;
            }
            log_write ( LEV_ALL_PARAM,
					"TPDU RECV SIZE [%d], UDL VALUE[%d(H'%02X)], UDSIZE[%d]\n",
                    size, dlReport.tpUDL, dlReport.tpUDL, remain);
        	dlReport.tpUDL = remain;
		}
        /* new samsung 06-01-19*/
        remain = 0;
	}

	if(dlReport.tpUDL > 0)
	{
		if(!is8bit)
		{
			//SetUDParameter(dlReport,tmp,dlReport.tpUDL);
			//SetUDSize(dlReport,idx);
            SetUDParameter(dlReport,tmp,idx);
            SetUDSize2(dlReport,idx);
		}
		else
		{
			pt++;
			SetUDParameter(dlReport,pt,dlReport.tpUDL);
			SetUDSize(dlReport,dlReport.tpUDL);
			remain -= dlReport.tpUDL;
		}
		if(idx == TPDU_FALSE) 
		{
			setGSMTPDUErrorCode(DEC_INVALID_FORMAT);
			return TPDU_FALSE;
		}
	}	

	if(remain != 0)
	{
		log_write ( LEV_ALL_PARAM,"\n\nDECODING ERROR [%d]\n",remain);
		log_write ( LEV_ALL_PARAM, "decSMSDeliveryReport TPDU Message Length"
				" Invalid [InputSize %d, Remain %d]\n", size,remain);
		setGSMTPDUErrorCode(DEC_INVALID_FORMAT);
		return TPDU_FALSE;
	}
	memcpy((TPSMSDeliverReport*)arg, (TPSMSDeliverReport*)&dlReport,
			sizeof(TPSMSDeliverReport));
    printSMSDeliverReport(dlReport);
	return TPDU_TRUE;
}


/*************************************************************************************
 *
 *  SMS SubmitReport Decoding Function
 *
 ************************************************************************************/
int		decSMSSubmitReport(TPSMSSubmitReport *arg,unsigned char *data,
	int size,int isErr){
	int		idx=0,remain=0,nByte=0;
	unsigned char	*pt = NULL,val=0;
	unsigned char	tmp[512];
	TPSMSSubmitReport	sbReport;
	int		is8bit=0;

	memset((TPSMSSubmitReport*)&sbReport,0x00,sizeof(TPSMSSubmitReport));
	memset((unsigned char*)&tmp,0x00,512);
	memset((TPSMSSubmitReport*)&sbReport,0x00,sizeof(TPSMSSubmitReport));
	
	remain = size;
	log_write ( LEV_ALL_PARAM,"\ndecSMSSubmitReport()\t\t\t\t\t[START %d]\n",remain); 

	if((data == NULL) || (size <= 0))
	{
		log_write ( LEV_ALL_PARAM,"ERROR: decSMSSubmitReport()\t\t[Invalid Argument]\n");
		setGSMTPDUErrorCode(DEC_INVALID_FORMAT);	
		log_write ( LEV_ALL_PARAM, "decSMSDelivery Argument is invalid\n");
		return TPDU_FALSE;
	}
	pt = data;

	/* ---------------------------------------------------------------------------*/
	/*		Decoding Indicator Octet											  */	
	/* ---------------------------------------------------------------------------*/
	sbReport.idc.charval = *pt;

	SetMTIParameter(sbReport,sbReport.idc.mti.value);
	SetUDHIParameter(sbReport,sbReport.idc.udhi.value);

	log_write ( LEV_ALL_PARAM,"\tMTI(M) [%d] UDHI(O) [%d] \n",
			GetMTIValue(sbReport), GetUDHIValue(sbReport));

	sbReport.optional.UDHI= GetUDHIValue(sbReport);

	if(GetMTIValue(sbReport) != TPDU_SMS_SUBMIT_REPORT)
	{
		log_write ( LEV_ALL_PARAM,"ERROR: NOT SMS SUBMIT_REPORT PDU [%d]\n",
				GetMTIValue(sbReport));
		setGSMTPDUErrorCode(DEC_INVALID_FORMAT);
		log_write ( LEV_ALL_PARAM, "decSMSSubmitReport Not SubmitReport TPDU [%d]\n",
				GetMTIValue(sbReport));
		return TPDU_FALSE;	
	}
	pt++;
	remain--;

	/* ---------------------------------------------------------------------------*/
	/*		Decoding TP-FCS Parameter											  */	
	/* ---------------------------------------------------------------------------*/
	if(isErr)
	{
		tmp[0]= *pt;
		SetFCSParameter(sbReport,tmp[0]);
		log_write ( LEV_ALL_PARAM,"\tTP-FCS [H' %02X]\n",
				GetFCSValue(sbReport));
		sbReport.optional.FCS = 1;
		pt++;
		remain--;
	}
	else
	{
		sbReport.optional.FCS = 0;
	}

	if(remain == 0)
	{	/* this case is phase1 and phase 2 */
		if(!sbReport.optional.FCS)
		{
			setGSMTPDUErrorCode(DEC_INVALID_FORMAT);
			log_write ( LEV_ALL_PARAM, "decSMSSubmitReport Not Exist "
					"TP-FCS(Phase1 and Phase2 is Mandatory)\n");
			return TPDU_FALSE;
		}
		memcpy((TPSMSSubmitReport*)arg,(TPSMSSubmitReport*)&sbReport,
				sizeof(TPSMSSubmitReport));
    	printSMSSubmitReport(sbReport);
		return TPDU_TRUE;
	}

	/* This case is phase2++ */
	/* ---------------------------------------------------------------------------*/
	/*		Decoding TP-PI Parameter											  */	
	/* ---------------------------------------------------------------------------*/
	tmp[0] = *pt;
	SetPIParameter(sbReport,tmp[0]);

	sbReport.optional.PI = 1;

	sbReport.optional.PID = IsExistPID(sbReport.tpPI);
	sbReport.optional.UDL = IsExistUDL(sbReport.tpPI);
	sbReport.optional.DCS = IsExistDCS(sbReport.tpPI);

	pt++;
	remain--;
	/* ---------------------------------------------------------------------------*/
	/*		Decoding TP-SCTS Parameter											  */	
	/* ---------------------------------------------------------------------------*/
	SetSCTSParameter(sbReport,getParamSCTSValue(pt));
	pt += TPVP_MAXSIZE;
	remain -= TPVP_MAXSIZE;
	log_write ( LEV_ALL_PARAM,"\tTP-SCTS     [H' %02X %02X %02X %02X %02X %02X %02X]\n",
			GetSCTSValueIdx(sbReport.tpSCTS,0),GetSCTSValueIdx(sbReport.tpSCTS,1),
			GetSCTSValueIdx(sbReport.tpSCTS,2),GetSCTSValueIdx(sbReport.tpSCTS,3),
			GetSCTSValueIdx(sbReport.tpSCTS,4),GetSCTSValueIdx(sbReport.tpSCTS,5),
			GetSCTSValueIdx(sbReport.tpSCTS,6));

	/* ---------------------------------------------------------------------------*/
	/*		Decoding TP-PID Parameter											  */	
	/* ---------------------------------------------------------------------------*/
	if(IsExistPID(sbReport.tpPI))
	{
		tmp[0] = *pt;
		SetPIDParameter(sbReport,tmp[0]);
		pt++;
		remain--;
		log_write ( LEV_ALL_PARAM,"\tTP-PID Type [%d]\n",GetPIDType(sbReport.tpPID));
		if(GetPIDType(sbReport.tpPID) == 0)
		{
			log_write ( LEV_ALL_PARAM,"\tTP-PID Interworking  [%d]\n",
            	GetPIDInterworking(sbReport.tpPID));
        	log_write ( LEV_ALL_PARAM,"\tTP-PID Value         [%d]\n",
           		GetPIDValueItw(sbReport.tpPID));
    	}
    	else
		{
       		log_write ( LEV_ALL_PARAM,"\tTP-PID Value         [%d]\n",
				GetPIDValueNoItw(sbReport.tpPID));
		}
	}
	
	/* ---------------------------------------------------------------------------*/
	/*		Decoding TP-DCS Parameter											  */	
	/* ---------------------------------------------------------------------------*/
	if(IsExistDCS(sbReport.tpPI))
	{
		tmp[0] = *pt;
		SetDCSParameter(sbReport,tmp[0]);
		pt++;	
		remain--;

		if(IsIncCharsetDcs(sbReport.tpDCS))
		{
			is8bit = GetDCSCharset(sbReport.tpDCS);	
		}

		log_write ( LEV_ALL_PARAM,"\tTP-DCS Group         [%d]\n",
			GetDCSGroup(sbReport.tpDCS));
		if(IsIncCharsetDcs(sbReport.tpDCS))
		{
        	log_write ( LEV_ALL_PARAM,"\tTP-DCS Charset   [%d]\n",
				GetDCSCharset(sbReport.tpDCS));
        	log_write ( LEV_ALL_PARAM,"\tTP-DCS Class     [%d]\n",
				GetDCSClass(sbReport.tpDCS));
    	}
    	else
		{
        	log_write ( LEV_ALL_PARAM,"\tTP-DCS Sense     [%d]\n",
				GetDCSSense(sbReport.tpDCS));
        	log_write ( LEV_ALL_PARAM,"\tTP-DCS Indicator Type [%d]\n",
            	GetDCSIdcType(sbReport.tpDCS));
    	}
	}

	/* ---------------------------------------------------------------------------*/
	/*		Checking TP-UDL Parameter											  */	
	/* ---------------------------------------------------------------------------*/
	if(!IsExistUDL(sbReport.tpPI))
	{
		if(remain != 0)
		{
			setGSMTPDUErrorCode(DEC_INVALID_FORMAT);
			log_write ( LEV_ALL_PARAM, "decSMSSubmitReport TPDU Message Length Invalid "
					"[InputSize %d, Remain %d]\n", size,remain);
			return	TPDU_FALSE;
		}
		memcpy((TPSMSSubmitReport*)arg,(TPSMSSubmitReport*)&sbReport,
			sizeof(TPSMSSubmitReport));
    	printSMSSubmitReport(sbReport);
		return TPDU_TRUE;
	}

	/* ---------------------------------------------------------------------------*/
	/*		Decoding TP-UDL Parameter											  */	
	/* ---------------------------------------------------------------------------*/
	sbReport.tpUDL = *pt; 
	remain --;
	log_write ( LEV_ALL_PARAM,"\tTP-UDL UD Length  [%d]\n", sbReport.tpUDL);
	if((!is8bit) && (sbReport.tpUDL > BIT7_UD_MAX_SIZE))
	{
		setGSMTPDUErrorCode(DEC_DATASIZE_ERROR);
		return TPDU_FALSE;
	}
	else if((is8bit) && (sbReport.tpUDL > BIT8_UD_MAX_SIZE))
	{
		setGSMTPDUErrorCode(DEC_DATASIZE_ERROR);
		return TPDU_FALSE;
	}

	/* ---------------------------------------------------------------------------*/
	/*		Decoding TP-UD Parameter											  */	
	/* ---------------------------------------------------------------------------*/
	if(sbReport.tpUDL == 0){
		if(remain != 0){
			log_write ( LEV_ALL_PARAM,"\n\nDECODING ERROR %d\n",remain);
			setGSMTPDUErrorCode(DEC_INVALID_FORMAT);
			log_write ( LEV_ALL_PARAM, "decSMSSubmitReport TPDU Message Length "
					"Invalid [InputSize %d, Remain %d]\n", size,remain);
			return TPDU_FALSE;
		}
		memcpy((TPSMSSubmitReport*)arg,(TPSMSSubmitReport*)&sbReport,
				sizeof(TPSMSSubmitReport));
    	printSMSSubmitReport(sbReport);
		return TPDU_TRUE;
	}
	if(!is8bit)
	{
		pt++;
        /* <-- 2006-01-03 add start*/
        if(GetUDHIValue(sbReport))
		{
            idx = dec7to8bitsUDH(pt,(unsigned char *)&tmp,remain);
            if(sbReport.tpUDL != idx)
			{
				log_write(LEV_ALL_PARAM, "TPDU WARRING: TPDU SIZE [%d]",size);
				log_write(LEV_ALL_PARAM, "\tTPDU SUBMIT REPORT DECODING WARRING: UDL Value[%d(H'%02X)]"
						"-Decoding Result [%d]\n",sbReport.tpUDL,sbReport.tpUDL,idx);       
				log_write(LEV_ALL_PARAM, "\tBut Continue processing");          
                idx = sbReport.tpUDL;
            }
            log_write ( LEV_ALL_PARAM,
					"TPDU RECV SIZE [%d], UDL VALUE[%d(H'%02X)], UDSIZE[%d]\n",
					size,sbReport.tpUDL,sbReport.tpUDL,remain);
        	sbReport.tpUDL = remain;
        }
        /* 2006-01-03 add end --> */   
		else
		{
			idx = dec7to8bits(pt,(unsigned char*)&tmp,remain);
			if( sbReport.tpUDL!= idx)
			{
				log_write(LEV_ALL_PARAM, "TPDU WARRING: TPDU SIZE [%d]",size);
				log_write(LEV_ALL_PARAM, "\tTPDU SUBMIT REPORT DECODING WARRING:"
						" UDL Value[%d(H'%02X)]-Decoding Result [%d]\n",
						sbReport.tpUDL,sbReport.tpUDL,idx);
				log_write(LEV_ALL_PARAM, "\tBut Continue processing");
				idx = sbReport.tpUDL;
            }
            log_write ( LEV_ALL_PARAM,
					"TPDU RECV SIZE [%d], UDL VALUE[%d(H'%02X)], UDSIZE[%d]\n",
					size,sbReport.tpUDL,sbReport.tpUDL,remain);
			sbReport.tpUDL = remain;
		}
        /* new samsung 06-01-19*/
        remain = 0;
	}

	if(sbReport.tpUDL > 0)
	{
		if(!is8bit)
		{
            SetUDParameter(sbReport,tmp,idx);
            SetUDSize2(sbReport,idx);
		}
		else
		{
			pt++;
			SetUDParameter(sbReport,pt,sbReport.tpUDL);
			SetUDSize(sbReport,sbReport.tpUDL);
			remain -= sbReport.tpUDL;
		}
		if(idx == TPDU_FALSE) 
		{
			setGSMTPDUErrorCode(DEC_INVALID_FORMAT);
			return TPDU_FALSE;
		}
	}	

	if(remain != 0)
	{
		log_write ( LEV_ALL_PARAM,"\n\nDECODING ERROR [%d]\n",remain);
		setGSMTPDUErrorCode(DEC_INVALID_FORMAT);
		log_write ( LEV_ALL_PARAM, "decSMSSubmitReport TPDU Message Length Invalid [Remain %d]\n",remain);
		return TPDU_FALSE;
	}

	memcpy((TPSMSSubmitReport*)arg, (TPSMSSubmitReport*)&sbReport, 
			sizeof(TPSMSSubmitReport));
    printSMSSubmitReport(sbReport);
	return TPDU_TRUE;
}

/*************************************************************************************
 *
 *  SMS StatusReport Decoding Function
 *
 ************************************************************************************/
int		decSMSStatusReport(TPSMSStatusReport *arg,unsigned char *data,
	int size){
	int		idx=0,remain=0,nByte=0;
	unsigned char	*pt = NULL,val=0;
	unsigned char	tmp[512];
	TPSMSStatusReport	stReport;
	int		is8bit=0;

	memset((TPSMSStatusReport*)&stReport,0x00,sizeof(TPSMSStatusReport));
	memset((unsigned char*)&tmp,0x00,512);
	memset((TPSMSStatusReport*)&stReport,0x00,sizeof(TPSMSStatusReport));
	
	remain = size;
	log_write ( LEV_ALL_PARAM,"\ndecSMSStatusReport()\t\t\t\t\t[START %d]\n",remain); 

	if((data == NULL) || (size <= 0))
	{
		log_write ( LEV_ALL_PARAM,"ERROR: decSMSStatusReport()\t\t[Invalid Argument]\n");
		setGSMTPDUErrorCode(DEC_INVALID_FORMAT);	
		log_write ( LEV_ALL_PARAM, "decSMSDelivery Argument is invalid\n");
		return TPDU_FALSE;
	}
	pt = data;
	/* ---------------------------------------------------------------------------*/
	/*		Decoding Indicator Octet											  */	
	/* ---------------------------------------------------------------------------*/
	stReport.idc.charval = *pt;

	SetMTIParameter(stReport,stReport.idc.mti.value);
	SetMMSParameter(stReport,stReport.idc.mms.value);
	SetSRQParameter(stReport,stReport.idc.srq.value);
	SetUDHIParameter(stReport,stReport.idc.udhi.value);

	log_write ( LEV_ALL_PARAM,"\tMTI(M) [%d] MMS(M) [%d] SRQ(M) [%d], UDHI(O) [%d] \n",
			GetMTIValue(stReport), GetMMSValue(stReport),
			GetSRQValue(stReport), GetUDHIValue(stReport));

	stReport.optional.UDHI= GetUDHIValue(stReport);

	if(GetMTIValue(stReport) != TPDU_SMS_STATUS_REPORT)
	{
		log_write ( LEV_ALL_PARAM,"ERROR: NOT SMS STATUS_REPORT PDU [%d]\n",
				GetMTIValue(stReport));
		setGSMTPDUErrorCode(DEC_INVALID_FORMAT);
		log_write ( LEV_ALL_PARAM, "decSMSStatusReport Not StatusReport TPDU [%d]\n",
				GetMTIValue(stReport));
		return TPDU_FALSE;	
	}
	pt++;
	remain--;
	/* ---------------------------------------------------------------------------*/
	/*		Decoding TP-MR Parameter											  */	
	/* ---------------------------------------------------------------------------*/
	tmp[0] = *pt;
	SetMRParameter(stReport,tmp[0]);
	pt++;
	remain--;
	log_write ( LEV_ALL_PARAM,"\tTP-MR Value [%d]\n",GetMRValue(stReport));

	/* ---------------------------------------------------------------------------*/
	/*		Decoding TP-RA Parameter											  */	
	/* ---------------------------------------------------------------------------*/
	idx = decParamTPAddress(pt,&stReport.tpADDR);
	if(idx == TPDU_FALSE)
	{
		log_write ( LEV_ALL_PARAM,"ERROR: TP-RA Decoding Fail]\n");
		return TPDU_FALSE;
	}
	log_write ( LEV_ALL_PARAM,"\tTP-RA Address Length [%d]\n",GetADDRLen(stReport));
	log_write ( LEV_ALL_PARAM,"\tTP-RA Address Type   [%d]\n",GetADDRTON(stReport));
	log_write ( LEV_ALL_PARAM,"\tTP-RA Numbering Plan [%d]\n",GetADDRNPI(stReport));
	log_write ( LEV_ALL_PARAM,"\tTP-RA Address        [%s] \n",
		decParamBCDAddress((u_char *)GetADDRNumber(stReport),GetADDRSize(stReport)));

	pt += idx;
	remain -= idx;

	/* ---------------------------------------------------------------------------*/
	/*		Decoding TP-SCTS Parameter											  */	
	/* ---------------------------------------------------------------------------*/
	SetSCTSParameter(stReport,getParamSCTSValue(pt));
	pt += TPVP_MAXSIZE;
	remain -= TPVP_MAXSIZE;
	log_write ( LEV_ALL_PARAM,"\tTP-SCTS     [H' %02X %02X %02X %02X %02X %02X %02X]\n",
		GetSCTSValueIdx(stReport.tpSCTS,0),GetSCTSValueIdx(stReport.tpSCTS,1),
		GetSCTSValueIdx(stReport.tpSCTS,2),GetSCTSValueIdx(stReport.tpSCTS,3),
		GetSCTSValueIdx(stReport.tpSCTS,4),GetSCTSValueIdx(stReport.tpSCTS,5),
		GetSCTSValueIdx(stReport.tpSCTS,6));
	/* ---------------------------------------------------------------------------*/
	/*		Decoding TP-DT Parameter											  */	
	/* ---------------------------------------------------------------------------*/
	SetDTParameter(stReport,getParamSCTSValue(pt));
	pt += TPVP_MAXSIZE;
	remain -= TPVP_MAXSIZE;
	log_write ( LEV_ALL_PARAM,"\tTP-SCTS     [H' %02X %02X %02X %02X %02X %02X %02X]\n",
		GetSCTSValueIdx(stReport.tpDT,0),GetSCTSValueIdx(stReport.tpDT,1),
		GetSCTSValueIdx(stReport.tpDT,2),GetSCTSValueIdx(stReport.tpDT,3),
		GetSCTSValueIdx(stReport.tpDT,4),GetSCTSValueIdx(stReport.tpDT,5),
		GetSCTSValueIdx(stReport.tpDT,6));
	/* ---------------------------------------------------------------------------*/
	/*		Decoding TP-ST Parameter											  */	
	/* ---------------------------------------------------------------------------*/
	tmp[0] = *pt;
	SetSTParameter(stReport,tmp[0]);
	pt++;
	remain--;
	log_write ( LEV_ALL_PARAM,"\tTP-ST Value [%d]\n",GetSTValue(stReport));

	if(remain == 0)
	{	/* this case is phase1 and phase 2 */
		memcpy((TPSMSStatusReport*)arg,(TPSMSStatusReport*)&stReport,
				sizeof(TPSMSStatusReport));
		printSMSStatusReport(stReport);
		return TPDU_TRUE;
	}
	/* ---------------------------------------------------------------------------*/
	/*		Decoding TP-PI Parameter											  */	
	/* ---------------------------------------------------------------------------*/
	tmp[0] = *pt;
	SetPIParameter(stReport,tmp[0]);
	stReport.optional.PI = 1;
	stReport.optional.PID = IsExistPID(stReport.tpPI);
	stReport.optional.UDL = IsExistUDL(stReport.tpPI);
	stReport.optional.DCS = IsExistDCS(stReport.tpPI);
	log_write ( LEV_ALL_PARAM,"\tTP-ST Value [H '%02X]\n",GetPIValue(stReport));

	pt++;
	remain--;

	/* ---------------------------------------------------------------------------*/
	/*		Decoding TP-PID Parameter											  */	
	/* ---------------------------------------------------------------------------*/
	if(IsExistPID(stReport.tpPI))
	{
		tmp[0] = *pt;
		SetPIDParameter(stReport,tmp[0]);
		pt++;
		remain--;
		log_write ( LEV_ALL_PARAM,"\tTP-PID Type [%d]\n",GetPIDType(stReport.tpPID));
		if(GetPIDType(stReport.tpPID) == 0)
		{
			log_write ( LEV_ALL_PARAM,"\tTP-PID Interworking  [%d]\n",
					GetPIDInterworking(stReport.tpPID));
        	log_write ( LEV_ALL_PARAM,"\tTP-PID Value         [%d]\n",
					GetPIDValueItw(stReport.tpPID));
    	}
    	else
		{
       		log_write ( LEV_ALL_PARAM,"\tTP-PID Value         [%d]\n",
					GetPIDValueNoItw(stReport.tpPID));
		}
	}
	
	/* ---------------------------------------------------------------------------*/
	/*		Decoding TP-DCS Parameter											  */	
	/* ---------------------------------------------------------------------------*/
	if(IsExistDCS(stReport.tpPI))
	{
		tmp[0] = *pt;
		SetDCSParameter(stReport,tmp[0]);
		pt++;	
		remain--;

		if(IsIncCharsetDcs(stReport.tpDCS))
		{
			is8bit = GetDCSCharset(stReport.tpDCS);	
		}

		log_write ( LEV_ALL_PARAM,"\tTP-DCS Group         [%d]\n",
				GetDCSGroup(stReport.tpDCS));
		if(IsIncCharsetDcs(stReport.tpDCS))
		{
			log_write ( LEV_ALL_PARAM,"\tTP-DCS Charset   [%d]\n",
					GetDCSCharset(stReport.tpDCS));
        	log_write ( LEV_ALL_PARAM,"\tTP-DCS Class     [%d]\n",
					GetDCSClass(stReport.tpDCS));
    	}
    	else
		{
        	log_write ( LEV_ALL_PARAM,"\tTP-DCS Sense     [%d]\n",
					GetDCSSense(stReport.tpDCS));
			log_write ( LEV_ALL_PARAM,"\tTP-DCS Indicator Type [%d]\n",
					GetDCSIdcType(stReport.tpDCS));
    	}
	}

	if(!IsExistUDL(stReport.tpPI))
	{
		if(remain != 0)
		{
			log_write ( LEV_ALL_PARAM, "decSMSStatusReport TPDU Message Length"
					" Invalid [InputSize %d, Remain %d]\n",
					size,remain);
			return	TPDU_FALSE;
		}
		memcpy((TPSMSStatusReport*)arg, (TPSMSStatusReport*)&stReport,
				sizeof(TPSMSStatusReport));
		printSMSStatusReport(stReport);
		return TPDU_TRUE;
	}
	/* ---------------------------------------------------------------------------*/
	/*		Decoding TP-UDL Parameter											  */	
	/* ---------------------------------------------------------------------------*/
	stReport.tpUDL = *pt; 
	remain --;
	log_write ( LEV_ALL_PARAM,"\tTP-UDL UD Length  [%d]\n", stReport.tpUDL);

	if((!is8bit) && (stReport.tpUDL > BIT7_UD_MAX_SIZE))
	{
		setGSMTPDUErrorCode(DEC_DATASIZE_ERROR);
		return TPDU_FALSE;
	}
	else if((is8bit) && (stReport.tpUDL > BIT8_UD_MAX_SIZE))
	{
		setGSMTPDUErrorCode(DEC_DATASIZE_ERROR);
		return TPDU_FALSE;
	}

	/* ---------------------------------------------------------------------------*/
	/*		Decoding TP-UD Parameter											  */	
	/* ---------------------------------------------------------------------------*/
	if(stReport.tpUDL == 0)
	{
		if(remain != 0)
		{
			log_write ( LEV_ALL_PARAM,"\n\nDECODING ERROR %d\n",remain);
			setGSMTPDUErrorCode(DEC_INVALID_FORMAT);
			log_write ( LEV_ALL_PARAM, "decSMSStatusReport TPDU Message Length"
					" Invalid [InputSize %d, Remain %d]\n",
					size,remain);
			return TPDU_FALSE;
		}

		memcpy((TPSMSStatusReport*)arg,(TPSMSStatusReport*)&stReport,
				sizeof(TPSMSStatusReport));
		printSMSStatusReport(stReport);
		return TPDU_TRUE;
	}

	if(!is8bit)
	{
		pt++; 
        /* <-- 2006-01-03 add start*/
        if(GetUDHIValue(stReport))
		{
            idx = dec7to8bitsUDH(pt,(unsigned char *)&tmp,remain);
            if(stReport.tpUDL != idx)
			{
				log_write(LEV_ALL_PARAM, "TPDU WARRING: TPDU SIZE [%d]",size);
				log_write(LEV_ALL_PARAM, "\tTPDU STATUS REPORT DECODING WARRING:"
						" UDL Value[%d(H'%02X)]-Decoding Result [%d]\n",
						stReport.tpUDL,stReport.tpUDL,idx);       
				log_write(LEV_ALL_PARAM, "\tBut Continue processing");          
				idx = stReport.tpUDL;
            }
            log_write ( LEV_ALL_PARAM,
					"TPDU RECV SIZE [%d], UDL VALUE[%d(H'%02X)], UDSIZE[%d]\n",
					size,stReport.tpUDL,stReport.tpUDL,remain);
        	stReport.tpUDL = remain;
        }
        /* 2006-01-03 add end --> */   
		else
		{
			idx = dec7to8bits(pt,(unsigned char*)&tmp,remain);
            if( stReport.tpUDL!= idx)
			{
				log_write(LEV_ALL_PARAM, "TPDU WARRING: TPDU SIZE [%d]",size);
				log_write(LEV_ALL_PARAM, "\tTPDU STATUS REPORT DECODING WARRING: UDL Value[%d(H'%02X)]"
						"-Decoding Result [%d]\n",stReport.tpUDL,stReport.tpUDL,idx);
				log_write(LEV_ALL_PARAM, "\tBut Continue processing");
				idx = stReport.tpUDL;	
            }
            log_write ( LEV_ALL_PARAM,
                "TPDU RECV SIZE [%d], UDL VALUE[%d(H'%02X)], UDSIZE[%d]\n",
                    size,stReport.tpUDL,stReport.tpUDL,remain);
			stReport.tpUDL = remain;
		}
        /* new samsung 06-01-19*/
        remain = 0;
	}
	if(stReport.tpUDL > 0)
	{
		if(!is8bit)
		{
			//SetUDParameter(stReport,tmp,stReport.tpUDL);
			//SetUDSize(stReport,idx);
		//	remain -= idx;
            SetUDParameter(stReport,tmp,idx);
            SetUDSize2(stReport,idx);
		}
		else
		{
			pt++;
			SetUDParameter(stReport,pt,stReport.tpUDL);
			SetUDSize(stReport,stReport.tpUDL);
			remain -= stReport.tpUDL;
		}
		if(idx == TPDU_FALSE) 
		{
			setGSMTPDUErrorCode(DEC_INVALID_FORMAT);
			return TPDU_FALSE;
		}
	}	
	if(remain != 0)
	{
		log_write ( LEV_ALL_PARAM,"\n\nDECODING ERROR [%d]\n",remain);
		setGSMTPDUErrorCode(DEC_INVALID_FORMAT);
		log_write ( LEV_ALL_PARAM, "decSMSStatusReport TPDU Message Length"
				" Invalid [InputSize %d, Remain %d]\n",
				size,remain);
		return TPDU_FALSE;
	}
	memcpy((TPSMSStatusReport*)arg, (TPSMSStatusReport*)&stReport,
			sizeof(TPSMSStatusReport));
	printSMSStatusReport(stReport);
	return TPDU_TRUE;
}

/*************************************************************************************
 *
 *  SMS Command Decoding Function
 *
 ************************************************************************************/
int		decSMSCommand(TPSMSCommand *arg,unsigned char *data,int size){
	int		idx=0,remain=0,nByte=0;
	unsigned char	*pt = NULL;
	unsigned char	tmp[512];
	TPSMSCommand	command;
	int		is8bit=0;

	memset((TPSMSCommand*)&command,0x00,sizeof(TPSMSCommand));
	memset((unsigned char*)&tmp,0x00,512);
	memset((TPSMSCommand*)&command,0x00,sizeof(TPSMSCommand));
	
	remain = size;
	log_write ( LEV_ALL_PARAM,"\ndecSMSCommand()\t\t\t\t\t[START %d]\n",remain); 

	if((data == NULL) || (size <=0))
	{
		log_write ( LEV_ALL_PARAM,"ERROR: decSMSCommand()\t\t[Invalid Argument]\n");
		setGSMTPDUErrorCode(DEC_INVALID_FORMAT);	
		log_write ( LEV_ALL_PARAM, "decSMSDelivery Argument is invalid\n");
		return TPDU_FALSE;
	}
	pt = data;

	/* ---------------------------------------------------------------------------*/
	/*		Decoding Indicator Octet											  */	
	/* ---------------------------------------------------------------------------*/
	command.idc.charval = *pt;

	SetMTIParameter(command,command.idc.mti.value);
	SetSRRParameter(command,command.idc.srr.value);
	SetUDHIParameter(command,command.idc.udhi.value);

	log_write ( LEV_ALL_PARAM,"\tMTI(M) [%d], SRR(O) [%d], UDHI(O) [%d]\n",
			GetMTIValue(command), 
			GetSRRValue(command), GetUDHIValue(command));

	command.optional.SRR = GetSRRValue(command);
	command.optional.UDHI= GetUDHIValue(command);

	if(GetMTIValue(command) != TPDU_SMS_COMMAND)
	{
		log_write ( LEV_ALL_PARAM,"ERROR: NOT SMS COMMAND PDU [%d]\n",
				GetMTIValue(command));
		setGSMTPDUErrorCode(DEC_INVALID_FORMAT);
		log_write ( LEV_ALL_PARAM, "decSMSCommand Not Command TPDU [%d]\n",GetMTIValue(command));
		return TPDU_FALSE;	
	}
	pt++;
	remain--;

	/* ---------------------------------------------------------------------------*/
	/*		Decoding TP-MR Parameter											  */	
	/* ---------------------------------------------------------------------------*/
	tmp[0] = *pt;
	SetMRParameter(command,tmp[0]);
	pt++;
	remain--;
	log_write ( LEV_ALL_PARAM,"\tTP-MR Value [%d]\n",GetMRValue(command));

	/* ---------------------------------------------------------------------------*/
	/*		Decoding TP-PID Parameter											  */	
	/* ---------------------------------------------------------------------------*/
	tmp[0] = *pt;
	SetPIDParameter(command,tmp[0]);
	pt++;
	remain--;
	log_write ( LEV_ALL_PARAM,"\tTP-PID Type [%d]\n",GetPIDType(command.tpPID));
	if(GetPIDType(command.tpPID) == 0)
	{
        log_write ( LEV_ALL_PARAM,"\tTP-PID Interworking  [%d]\n",
            GetPIDInterworking(command.tpPID));
        log_write ( LEV_ALL_PARAM,"\tTP-PID Value         [%d]\n",
            GetPIDValueItw(command.tpPID));
    }
    else
	{
        log_write ( LEV_ALL_PARAM,"\tTP-PID Value         [%d]\n",
			GetPIDValueNoItw(command.tpPID));
	}

	/* ---------------------------------------------------------------------------*/
	/*		Decoding TP-CT Parameter											  */	
	/* ---------------------------------------------------------------------------*/
	tmp[0] = *pt;
	SetCTParameter(command,tmp[0]);
	pt++;
	remain--;
	log_write ( LEV_ALL_PARAM,"\tTP-CT Value [%d]\n",GetCTValue(command));
	if(GetSRRValue(command)==1)
	{
		if(GetCTValue(command) == 1 ||
			GetCTValue(command) == 2 ||
			GetCTValue(command) == 3)
		{
			 log_write ( LEV_ALL_PARAM,"Error TP-SRR value is 1 and CT Value is not h'00"
				"(TP-CT value h'%02X)\n",GetCTValue(command));
			setGSMTPDUErrorCode(ENC_COMVAL_ERROR);
			return TPDU_FALSE;
		}

	}
	else if(GetSRRValue(command)==0)
	{
		if(GetCTValue(command) == 0) 
		{
			 log_write ( LEV_ALL_PARAM,"Error TP-SRR value is 1 and CT Value is not h'00"
				"(TP-CT value h'%02X)\n",GetCTValue(command));
			setGSMTPDUErrorCode(ENC_COMVAL_ERROR);
			return TPDU_FALSE;
		}
	}

	/* ---------------------------------------------------------------------------*/
	/*		Decoding TP-MN Parameter											  */	
	/* ---------------------------------------------------------------------------*/
	tmp[0] = *pt;
	SetMNParameter(command,tmp[0]);
	pt++;
	remain--;
	log_write ( LEV_ALL_PARAM,"\tTP-MN Value [%d]\n",GetMNValue(command));


	/* ---------------------------------------------------------------------------*/
	/*		Decoding TP-DA Parameter											  */	
	/* ---------------------------------------------------------------------------*/
	idx = decParamTPAddress(pt,&command.tpADDR);
	if(idx == TPDU_FALSE)
	{
		log_write ( LEV_ALL_PARAM,"ERROR: TP-DA Decoding Fail]\n");
		return TPDU_FALSE;
	}
	log_write ( LEV_ALL_PARAM,"\tTP-DA Address Length [%d]\n",GetADDRLen(command));
	log_write ( LEV_ALL_PARAM,"\tTP-DA Address Type   [%d]\n",GetADDRTON(command));
	log_write ( LEV_ALL_PARAM,"\tTP-DA Numbering Plan [%d]\n",GetADDRNPI(command));
	log_write ( LEV_ALL_PARAM,"\tTP-DA Address        [%s] \n",
		decParamBCDAddress((u_char *)GetADDRNumber(command),GetADDRSize(command)));

	pt += idx;
	remain -= idx;
	
	/* ---------------------------------------------------------------------------*/
	/*		Decoding TP-UDL Parameter											  */	
	/* ---------------------------------------------------------------------------*/
	command.tpUDL = *pt; 
	remain --;
	log_write ( LEV_ALL_PARAM,"\tTP-CDL UD Length  [%d]\n", command.tpUDL);

	if((!is8bit) && (command.tpUDL > BIT7_UD_MAX_SIZE))
	{
		setGSMTPDUErrorCode(DEC_DATASIZE_ERROR);
		return TPDU_FALSE;
	}
	else if((is8bit) && (command.tpUDL > BIT8_UD_MAX_SIZE))
	{
		setGSMTPDUErrorCode(DEC_DATASIZE_ERROR);
		return TPDU_FALSE;
	}

	/* ---------------------------------------------------------------------------*/
	/*		Decoding TP-UD Parameter											  */	
	/* ---------------------------------------------------------------------------*/
	if(command.tpUDL == 0)
	{
		if(remain != 0)
		{
			log_write ( LEV_ALL_PARAM,"\n\nDECODING ERROR %d\n",remain);
			setGSMTPDUErrorCode(DEC_INVALID_FORMAT);
			log_write ( LEV_ALL_PARAM, "decSMSCommand TPDU Message Length"
					" Invalid [InputSize %d, Remain %d]\n",
					size,remain);
			return TPDU_FALSE;
		}
    	printSMSCommand(command);
		return TPDU_TRUE;
	}
	if(!is8bit)
	{
		pt++;
        /* <-- 2006-01-03 add start*/
        if(GetUDHIValue(command))
		{
            idx = dec7to8bitsUDH(pt,(unsigned char *)&tmp,remain);
            if(command.tpUDL != idx)
			{
				log_write(LEV_ALL_PARAM, "TPDU WARRING: TPDU SIZE [%d]",size);
				log_write(LEV_ALL_PARAM, "\tTPDU COMMAND DECODING WARRING: UDL Value[%d(H'%02X)]"
						"-Decoding Result [%d]\n",command.tpUDL,command.tpUDL,idx);       
				log_write(LEV_ALL_PARAM, "\tBut Continue processing");          
				idx = command.tpUDL;
            }
            log_write ( LEV_ALL_PARAM,
                "TPDU RECV SIZE [%d], UDL VALUE[%d(H'%02X)], UDSIZE[%d]\n",
                    size,command.tpUDL,command.tpUDL,remain);
        	command.tpUDL = remain;
        }
        /* 2006-01-03 add end --> */   
		else
		{
			idx = dec7to8bits(pt,(unsigned char*)&tmp,remain);
            if( command.tpUDL!= idx)
			{
				log_write(LEV_ALL_PARAM, "TPDU WARRING: TPDU SIZE [%d]",size);
				log_write(LEV_ALL_PARAM, "\tTPDU COMMAND DECODING WARRING: UDL Value[%d(H'%02X)]"
						"-Decoding Result [%d]\n",command.tpUDL,command.tpUDL,idx);
				log_write(LEV_ALL_PARAM, "\tBut Continue processing");
				idx = command.tpUDL;
            }
            log_write ( LEV_ALL_PARAM,
                "TPDU RECV SIZE [%d], UDL VALUE[%d(H'%02X)], UDSIZE[%d]\n",
                    size,command.tpUDL,command.tpUDL,remain);
			command.tpUDL = remain;
		}
        /* new samsung 06-01-19*/
        remain = 0;
	}
	if(command.tpUDL > 0)
	{
		if(!is8bit)
		{
			//SetUDParameter(command,tmp,command.tpUDL);
			//SetUDSize(command,idx);
            SetUDParameter(command,tmp,idx);
            SetUDSize2(command,idx);
		}
		else
		{
			pt++;
			SetUDParameter(command,pt,command.tpUDL);
			SetUDSize(command,command.tpUDL);
			remain -= command.tpUDL;
		}
		if(idx == TPDU_FALSE) 
		{
			setGSMTPDUErrorCode(DEC_INVALID_FORMAT);
			return TPDU_FALSE;
		}
	}	

	if(remain != 0)
	{
		log_write ( LEV_ALL_PARAM,"\n\nDECODING ERROR [%d]\n",remain);
		setGSMTPDUErrorCode(DEC_INVALID_FORMAT);
		log_write ( LEV_ALL_PARAM, "decSMSCommand TPDU Message Length "
				"Invalid [InputSize %d, Remain %d]\n",
				size,remain);
		return TPDU_FALSE;
	}
	memcpy((TPSMSCommand*)arg, (TPSMSCommand*)&command, sizeof(TPSMSCommand));
    printSMSCommand(command);
	return TPDU_TRUE;
}

int decUD_RemakeUD(unsigned char *cpUserData, TPSMSDeliver *spSubmit, UDH_Struct *spUDH)
{
	int         nMsgSize = 0;
	int         nStep   = 0;
	int         nIndex  = 0;
	int         nUDHL   = 0, nUDHL_temp=0;

    /* cpUserData[0] == UDHL                    *
	 * cpUserData[1] == IEI tag                 * 
	 * cpUserData[2] == IEI Length              *
	 * cpUserData[3] == IEI value start point   *
	 * ...                                      */

	unsigned char   ucTempUD[BUF_SSIZE];
	unsigned char   *cpUD = NULL;

	bzero(spUDH, sizeof(UDH_Struct));
	bzero(ucTempUD, BUF_SSIZE);

    nUDHL_temp  = nUDHL = *cpUserData;
	nMsgSize    = spSubmit->tpUDL-(nUDHL+1);    // +1 (UDHL 1byte)
	log_write(LEV_ALL_PARAM, "nUDHL[%d] nMsgSize[%d] = spSubmit->tpUDL[%d]\n", nUDHL, nMsgSize, spSubmit->tpUDL);
	if(nMsgSize < 0)
	{
		log_write ( LEV_ALL_PARAM, "The UDHL the size grows UD size compared to. ");
		return FLK_FAIL;
	}

    cpUserData++;

	/* UDH Decoding */
	while(nUDHL_temp >= 0) 	// UDHL
	{
		if(spUDH->nIEI_Cnt >= IEI_ST_MAX_SIZE)
		{
			// Length fail 
			log_write ( LEV_ALL_PARAM, "[%s:%d] IEI MAX Size Over! IEI_Total[%d]", 
					__FILE__, __LINE__, spUDH->nIEI_Cnt);
			return FLK_FAIL;
		}

		spUDH->stIEI[nIndex].cpIEI_Tag = cpUserData;
		nUDHL_temp--;
		cpUserData++;   // ++ after position IEI Length 
		spUDH->stIEI[nIndex].cpIEI_Length = cpUserData;
		nUDHL_temp--;
		cpUserData++;   // ++ after position IEI Value
		spUDH->stIEI[nIndex].cpIEI_Value = cpUserData;

		cpUserData += *spUDH->stIEI[nIndex].cpIEI_Length; // ++ after position UD  
		nUDHL_temp-=*spUDH->stIEI[nIndex].cpIEI_Length;
		if(nUDHL_temp == 0) 
		{
			spUDH->nIEI_Cnt++;
			// Sucess 
			break;
		}
        else if(nUDHL_temp < 0) 
		{
			// Length fail 
			log_write ( LEV_ALL_PARAM, "[%s:%d] IEI[%d] Length over! UDHL[%d]", 
					__FILE__, __LINE__, nIndex, nUDHL_temp);
			return FLK_FAIL;
		}
		spUDH->nIEI_Cnt++;					
		nIndex++;
	}

	for(nIndex=0; nIndex < spUDH->nIEI_Cnt; nIndex++)
	{
        switch(*spUDH->stIEI[nIndex].cpIEI_Tag)
		{
			case TPDU_CT_8bit_REF_TAG :
				/* Length Check */
				if(*spUDH->stIEI[nIndex].cpIEI_Length != TPDU_CR_8_REF_TOTAL_SIZE) // 3
				{
					log_write ( LEV_ALL_PARAM, "Concatennated 8bit Message Length Error [%d]", 
							*spUDH->stIEI[nIndex].cpIEI_Length);
					return FLK_FAIL;
				}
				spUDH->stIEI[nIndex].ucIEI_Exist_Flag = false;

				spSubmit->tpIEI_Ref_Num = *(spUDH->stIEI[nIndex].cpIEI_Value++);
//#ifdef DEBUG
				log_write(LEV_ALL_PARAM, "[%s:%d] Test Log spSubmit->tpIEI_Ref_Num[%d] \n", 
						__FILE__, __LINE__, spSubmit->tpIEI_Ref_Num);
//#endif // DEBUG
				/* Reference number range check */
				if(spSubmit->tpIEI_Ref_Num > TPDU_CR_8BIT_MAX_VALUE) 	// 255
				{
					spSubmit->tpIEI_ConcatType = TPDU_CR_TYPE_NONE; 
					log_write ( LEV_ALL_PARAM, "Concatennated 8BIT Reference Number is MAX_SIZE[%d] over [%d]",
							TPDU_CR_8BIT_MAX_VALUE,
							spSubmit->tpIEI_Ref_Num);
					return FLK_FAIL;
				}

				spSubmit->tpIEI_Max_Num = *(spUDH->stIEI[nIndex].cpIEI_Value++);
//#ifdef DEBUG
				log_write(LEV_ALL_PARAM, "[%s:%d] Test Log spSubmit->tpIEI_Max_Num[%d] \n", 
						__FILE__, __LINE__, spSubmit->tpIEI_Max_Num);
//#endif // DEBUG
				/* Maximum number range check 	*/
				if(spSubmit->tpIEI_Max_Num > 100) 	// 255
				{
					spSubmit->tpIEI_ConcatType = TPDU_CR_TYPE_NONE; 
					log_write ( LEV_ALL_PARAM, "Concatennated 8BIT Maximum Number is MAX_SIZE[%d] over [%d]",
							TPDU_CM_MAX_VALUE,
							spSubmit->tpIEI_Max_Num);
					return FLK_FAIL;
				}

				spSubmit->tpIEI_Seq_Num = *(spUDH->stIEI[nIndex].cpIEI_Value);
				/* Sequence number range check 	*/
				if(spSubmit->tpIEI_Seq_Num > TPDU_CS_MAX_VALUE)			// 255
				{
					spSubmit->tpIEI_ConcatType = TPDU_CR_TYPE_NONE; 
					log_write ( LEV_ALL_PARAM, "Concatennated 8BIT Sequence Number is MAX_SIZE[%d] over [%d]",
							TPDU_CS_MAX_VALUE,
							spSubmit->tpIEI_Seq_Num);
					return FLK_FAIL;
				}
				spSubmit->tpIEI_ConcatType = TPDU_CR_TYPE_8BIT; 
				break;
			case TPDU_CT_16bit_REF_TAG :
				/* Length Check */
				if(*spUDH->stIEI[nIndex].cpIEI_Length != TPDU_CR_16_REF_TOTAL_SIZE)	// 4
				{
					log_write ( LEV_ALL_PARAM, "Concatennated 16bit Message Length Error [%d]", 
							*spUDH->stIEI[nIndex].cpIEI_Length);
					return FLK_FAIL;
				}
				spUDH->stIEI[nIndex].ucIEI_Exist_Flag = false;

				/* Reference number range check */
				memcpy(&spSubmit->tpIEI_Ref_Num, 
						spUDH->stIEI[nIndex].cpIEI_Value, TPDU_CR_16_REF_SIZE); // 2
				if(spSubmit->tpIEI_Ref_Num > TPDU_CR_16BIT_MAX_VALUE)
				{
					spSubmit->tpIEI_ConcatType = TPDU_CR_TYPE_NONE; 
					log_write ( LEV_ALL_PARAM, "Concatennated 16BIT Reference Number is MAX_SIZE[%d] over [%d]",
							TPDU_CR_16BIT_MAX_VALUE, spSubmit->tpIEI_Ref_Num);
					return FLK_FAIL;
				}
				spUDH->stIEI[nIndex].cpIEI_Value+=TPDU_CR_16_REF_SIZE;

				spSubmit->tpIEI_Max_Num = *(spUDH->stIEI[nIndex].cpIEI_Value++);
				/* Maximum number range check 	*/
				// if(spSubmit->tpIEI_Max_Num > TPDU_CM_MAX_VALUE)
				if(spSubmit->tpIEI_Max_Num > 100) 	// 255
				{
					spSubmit->tpIEI_ConcatType = TPDU_CR_TYPE_NONE; 
					log_write ( LEV_ALL_PARAM, "Concatennated 16BIT Maximum Number is MAX_SIZE[%d] over [%d]",
							TPDU_CM_MAX_VALUE,
							spSubmit->tpIEI_Max_Num);
					return FLK_FAIL;
				}

				spSubmit->tpIEI_Seq_Num = *(spUDH->stIEI[nIndex].cpIEI_Value);
				/* Sequence number range check 	*/
				if(spSubmit->tpIEI_Seq_Num > TPDU_CS_MAX_VALUE)
				{
					spSubmit->tpIEI_ConcatType = TPDU_CR_TYPE_NONE; 
					log_write ( LEV_ALL_PARAM, "Concatennated 16BIT Sequence Number is MAX_SIZE[%d] over [%d]",
							TPDU_CS_MAX_VALUE,
							spSubmit->tpIEI_Seq_Num);
					return FLK_FAIL;
				}
				spSubmit->tpIEI_ConcatType = TPDU_CR_TYPE_16BIT; 
				break;
			case TPDU_AP_8bit_PORT_TAG  :
				/* Length Check */
				if(*spUDH->stIEI[nIndex].cpIEI_Length > TPDU_AP_8_PORT_SIZE*2)
				{
					log_write ( LEV_ALL_PARAM, "Application Port 8BIT Length Error [%d]", 
							*spUDH->stIEI[nIndex].cpIEI_Length);
					return FLK_FAIL;
				}

				spUDH->stIEI[nIndex].ucIEI_Exist_Flag = false;
				if(*spUDH->stIEI[nIndex].cpIEI_Value > TPDU_AP_8_PORT_MAX_VALUE)
				{
					spSubmit->tpIEI_AportType = TPDU_AP_TYPE_NONE; 
					log_write ( LEV_ALL_PARAM, "Application Port 8BIT Value is MAX_SIZE[%d] over [%d]",
							TPDU_AP_8_PORT_MAX_VALUE,
							*spUDH->stIEI[nIndex].cpIEI_Length);
					return FLK_FAIL;
				}
				else
				{
					spSubmit->tpIEI_AportType = TPDU_AP_TYPE_8BIT; 
					spSubmit->tpIEI_Aport = *spUDH->stIEI[nIndex].cpIEI_Value;
				}
				break;
			case TPDU_AP_16ibt_PORT_TAG :     // port temp tag 
				/* Length Check */
				if(*spUDH->stIEI[nIndex].cpIEI_Length > TPDU_AP_16_PORT_SIZE*2)
				{
					log_write ( LEV_ALL_PARAM, "Application Port 16BIT Length Error [%d]", 
							*spUDH->stIEI[nIndex].cpIEI_Length);
					return FLK_FAIL;
				}

				spUDH->stIEI[nIndex].ucIEI_Exist_Flag = false;
				memcpy(&spSubmit->tpIEI_Aport, 
						spUDH->stIEI[nIndex].cpIEI_Value, TPDU_AP_16_PORT_SIZE);
				if(spSubmit->tpIEI_Aport > TPDU_AP_16_PORT_MAX_VALUE)
				{
					spSubmit->tpIEI_AportType = TPDU_AP_TYPE_NONE; 
					log_write ( LEV_ALL_PARAM, "Application Port 16BIT Value is MAX_SIZE[%d] over [%d]",
							TPDU_AP_16_PORT_MAX_VALUE, spSubmit->tpIEI_Aport);
					return FLK_FAIL;
				}
				spSubmit->tpIEI_AportType = TPDU_AP_TYPE_16BIT; 
				break;
			case TPDU_REPLY_ADDRESS_TAG :     // callback Tag
				{
					/* Length Check */
					if(*spUDH->stIEI[nIndex].cpIEI_Length > TPDU_ADDRESS_LENGTH)
					{
						log_write ( LEV_ALL_PARAM, "Callback IEI Length Error[%d]", 
								*spUDH->stIEI[nIndex].cpIEI_Length);
						return FLK_FAIL;
					}

					int nIdx=0;
					/* submit struct copy */
					spUDH->stIEI[nIndex].ucIEI_Exist_Flag = false;
					nIdx = decParamTPCallBackAddress(
							spUDH->stIEI[nIndex].cpIEI_Value, &spSubmit->tpCallbackADDR);
					log_write ( LEV_ALL_PARAM,"\tTP-CB Address Length [%d]\n", 
							spSubmit->tpCallbackADDR.addrLen);
					log_write ( LEV_ALL_PARAM,"\tTP-CB Address Type   [%d]\n", 
							spSubmit->tpCallbackADDR.pAddrType); 
					log_write ( LEV_ALL_PARAM,"\tTP-CB Numbering Plan [%d]\n", 
							spSubmit->tpCallbackADDR.pAddrNumPlan); 
					log_write ( LEV_ALL_PARAM,"\tTP-CB Address        [%s] \n", 
							decParamBCDCallBackAddress((u_char *)spSubmit->tpCallbackADDR.address, 
								spSubmit->tpCallbackADDR.addrOctetSize)); 

					if(nIdx == FLK_FAIL)
					{ 
						log_write ( LEV_ALL_PARAM, "[%s:%d]ERROR: Callback Decoding Fail", __func__, __LINE__);
						log_write ( LEV_ALL_PARAM,"ERROR: Callback Decoding Fail]\n"); 
						return FLK_FAIL; 
					}
				}
				break;
			default:
				break;
		}
	}

	nUDHL = 0;      // Init 
	/* SKT not used */ 
	/* UDH Length 
	for(nIndex=0; nIndex < spUDH->nIEI_Cnt; nIndex++)
	{
		if(spUDH->stIEI[nIndex].ucIEI_Exist_Flag == true)
		{
			// 1 (IEI Tag 1byte + IEI Length 1byte)
			nUDHL += (*spUDH->stIEI[nIndex].cpIEI_Length)+2; 
			log_write ( LEV_ALL_PARAM, "[%s:%d] Test Log nUDHL[%0d]", 
					__FILE__, __LINE__, nUDHL);
		}
	}
	*/

	if(nUDHL) 
	{
		ucTempUD[0] = nUDHL;
		// +1 UDHL 1byte
		spSubmit->tpUD.size = spSubmit->tpUDL = (nUDHL+1) + nMsgSize; 
	}
	else
	{
		spSubmit->tpUD.size = spSubmit->tpUDL = nMsgSize; 
	}

	memcpy(&ucTempUD[nStep], cpUserData, nMsgSize);
	memcpy(spSubmit->tpUD.data, ucTempUD, spSubmit->tpUDL);

	return FLK_SUCCESS;
}
