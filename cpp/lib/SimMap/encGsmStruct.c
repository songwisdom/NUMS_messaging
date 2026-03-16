/**
 @ingroup tpdu 
 @{ 
 */
#include <libTpdu23040.h>
#include <MapStruct.h>
#include <GsmTpduStruct.h>
#include <GsmTpduParam.h>
#include <string.h>

//#include "MsgConvert.h"
#include "flk_iconv.h"
#include "SKT_SvcInfo.h"


/*************************************************************************************
 *
 *  SMS Submit Encoding Function
 *
 ************************************************************************************/
int		encSMSSubmit(TPSMSSubmit *argpdu,u_char *buff)
{
	int				retSize=0;
	int				pIdx=0, idx=0, i=0;
	int				is8bit=0;
	SemiOctet		*pso1=NULL, *pso2=NULL;
	u_char	tmp[512], tmpBuff[512], tmpC;
	u_char 	*pt=NULL;
	u_char   *ptUDHI = NULL;

	TPSMSSubmit 	pdu;

	memset((u_char*)&tmp,0x00,512);
	memset((u_char*)&tmpBuff,0x00,512);
	memset((TPSMSSubmit*)&pdu,0x00,sizeof(TPSMSSubmit));

	memcpy((TPSMSSubmit*)&pdu,argpdu,sizeof(TPSMSSubmit));
	pt = (u_char*)&tmpBuff;

	if(buff == NULL){
		return FLK_FAIL;
	}

	pdu.idc.mti.value	= GetMTIValue(pdu);
	pdu.idc.rd.value	= GetRDValue(pdu);
	pdu.idc.vpf.value	= GetVPFValue(pdu);
	pdu.idc.rp.value 	= GetRPValue(pdu);
	pdu.idc.srr.value 	= GetSRRValue(pdu);
	pdu.idc.udhi.value 	= GetUDHIValue(pdu);

	*pt= pdu.idc.charval;
	/* UDHI position */
	ptUDHI = pt;

	/* Checking VPF */
	if(GetVPFValue(pdu) != 0) 
	{
		pdu.optional.VP = 1;
	}

	/* ---------------------------------------------------------------------- */	
	/*	 Encoding Indicator Octet  											  */
	/* ---------------------------------------------------------------------- */	
	if(GetMTIValue(pdu) != TPDU_SMS_SUBMIT){
		return FLK_FAIL;	
	}

	pt++;
	retSize++;

	/* ---------------------------------------------------------------------- */	
	/* 		Encoding TP-MR Parameter 										  */
	/* ---------------------------------------------------------------------- */	

	*pt = GetMRValue(pdu);
	pt++;
	retSize++;
	/* ---------------------------------------------------------------------- */	
	/* 		Encoding TP-DA Parameter 										  */
	/* ---------------------------------------------------------------------- */	
	pIdx = encParamTPAddress(pt,&pdu.tpADDR);

	if(pIdx == TPDU_FALSE){
		return FLK_FAIL;
	}

	pt += pIdx;
	retSize += pIdx;

	/* ---------------------------------------------------------------------- */	
	/* 		Encoding TP-PID Parameter 										  */
	/* ---------------------------------------------------------------------- */	

	*pt = GetPIDValue(pdu);
	pt++;
	retSize++;

	/* ---------------------------------------------------------------------- */	
	/* 		Encoding TP-DCS Parameter 										  */
	/* ---------------------------------------------------------------------- */	
	*pt = GetDCSValue(pdu);

	if(IsIncCharsetDcs(pdu.tpDCS))
		is8bit = GetDCSCharset(pdu.tpDCS);	
	
	pt++;
	retSize++;
	/* ---------------------------------------------------------------------- */	
	/* 		Encoding TP-VP Parameter 										  */
	/* ---------------------------------------------------------------------- */	
	if(pdu.optional.VP){
		pdu.tpVP.type = GetVPFValue(pdu);
		if(GetVPFValue(pdu)==VP_RELATIVE_TYPE){
			*pt = pdu.tpVP.vpRelative;
			pt++;
			retSize++;
		}
		else if(GetVPFValue(pdu)==VP_ENHANCED_TYPE){
			*pt = pdu.tpVP.vpEnIdcValue;
			pt++; retSize++;
			if(pdu.tpVP.vpEnFormat == 1 || pdu.tpVP.vpEnFormat ==  2){
				memcpy(pt,&pdu.tpVP.vpEnValue,6);
				pt += 6; retSize+=6;
			}
			else{
				for(i=0;i<6;i++){
					tmpC = 0x00;
					pso1 = (SemiOctet*)&pdu.tpVP.vpEnValue[i];
					pso2 = (SemiOctet*)&tmpC;
					pso2->digit1st = pso1->digit2nd;
					pso2->digit2nd = pso1->digit1st;
			 		*pt = tmpC;		
					pt++; retSize++;
				}
			}
		}
		else{
			setParamSCTSValue(pt,(u_char*)&pdu.tpVP.vpValue);	
				pt += TPVP_MAXSIZE;
			retSize += TPVP_MAXSIZE;
		}
	}

	/* ---------------------------------------------------------------------- */	
	/* 		Encoding TP-Callback Parameter   								  */
	/* ---------------------------------------------------------------------- */	
	/* make callback data */
	u_char ucCallbackData[32];
	memset(ucCallbackData, 0x00, 32);
	if(GetADDRCBLen(pdu))
	{ 
		pIdx = encParamTPCallbackAddress(ucCallbackData,&pdu.tpCallbackADDR);
		if(pIdx == TPDU_FALSE){
			return FLK_FAIL;
		}
	}

    /* SKT UDHI create */
	int nStep = 0;
	int nUDHI_exist = GetUDHIValue(pdu);
	u_char cpTemp[256];          // it's temporary UD 
	memset(cpTemp, 0x00, 256);          // initial

	cpTemp[nStep++];                    // UDHL

	/* Concatenated IEI create */
	if( (pdu.tpIEI_ConcatType == TPDU_CR_TYPE_8BIT) ||
		(pdu.tpIEI_ConcatType == TPDU_CR_TYPE_16BIT) )
	{
		if(pdu.tpIEI_ConcatType == TPDU_CR_TYPE_8BIT)
		{
			cpTemp[nStep++] = TPDU_CT_8bit_REF_TAG;
			cpTemp[nStep++] = TPDU_CR_8_REF_TOTAL_SIZE;
			cpTemp[nStep++] = (u_char)pdu.tpIEI_Ref_Num;
		}
		else
		{
			cpTemp[nStep++] = TPDU_CT_16bit_REF_TAG;
			cpTemp[nStep++] = TPDU_CR_16_REF_TOTAL_SIZE;
			memcpy(&cpTemp[nStep], &pdu.tpIEI_Ref_Num,
					TPDU_CR_16_REF_SIZE);
			nStep+=TPDU_CR_16_REF_SIZE;
		}
		cpTemp[nStep++] = (u_char)pdu.tpIEI_Max_Num;
		cpTemp[nStep++] = (u_char)pdu.tpIEI_Seq_Num;
		/* UDHI Set */
		pdu.idc.charval = *ptUDHI;
		pdu.idc.udhi.value = 1;
		*ptUDHI = pdu.idc.charval;
	}

	/* Callback IEI create */
	if(GetADDRCBLen(pdu))
	{
		cpTemp[nStep++] = TPDU_REPLY_ADDRESS_TAG;    // IEI tag (SKT Callback) 
		cpTemp[nStep++] = pIdx;                     // IEI Length  
		memcpy(&cpTemp[nStep], ucCallbackData, pIdx);
		nStep+=pIdx;

		/* UDHI Set */
		pdu.idc.charval = *ptUDHI;
		pdu.idc.udhi.value = 1;
		*ptUDHI = pdu.idc.charval;
	}

    /* Application Port IEI create */
	if(pdu.tpIEI_AportType != TPDU_AP_TYPE_NONE)
	{
		if(pdu.tpIEI_AportType == TPDU_AP_TYPE_8BIT)
		{
			cpTemp[nStep++] = TPDU_AP_8bit_PORT_TAG;
			cpTemp[nStep++] = TPDU_AP_8_PORT_SIZE*2;
			cpTemp[nStep++] = (u_char)pdu.tpIEI_Aport;
			cpTemp[nStep++] = 0x00;     // Originator Por is not use. default setting 
			nStep+=TPDU_AP_8_PORT_SIZE*2;

			/* UDHI Set */
			pdu.idc.charval = *ptUDHI;
			pdu.idc.udhi.value = 1;
			*ptUDHI = pdu.idc.charval;
		}
		else if(pdu.tpIEI_AportType == TPDU_AP_TYPE_16BIT)
		{
			cpTemp[nStep++] = TPDU_AP_16ibt_PORT_TAG;
			cpTemp[nStep++] = TPDU_AP_16_PORT_SIZE*2;
			memcpy(&cpTemp[nStep], &pdu.tpIEI_Aport,
					TPDU_AP_16_PORT_SIZE);
			nStep+=TPDU_AP_16_PORT_SIZE;

			memcpy(&cpTemp[nStep], 0x00, 
					TPDU_AP_16_PORT_SIZE); // Originator Por is not use. default setting
			nStep+=TPDU_AP_16_PORT_SIZE; 
			/* UDHI Set */
			pdu.idc.charval = *ptUDHI;
			pdu.idc.udhi.value = 1;
			*ptUDHI = pdu.idc.charval;
		}
	}

	int     nTemp=0;
	char    cTemp[BUF_SSIZE];
	switch(getDcsType(pdu.tpDCS.value))
	{
		case DCS_TYPE_UCS2:

			//Convert KSC5601 into UCS2
			if ( flk_convert_contents ( FLK_ICONV_CP949, 
					(char *)&pdu.tpUD.data, (int)pdu.tpUDL, 
					FLK_ICONV_UCS2, cTemp, (int *)&nTemp) != FLK_FAIL )
			{
				if(nTemp > MAX_TP_USER_DATA)
				{ 
					nTemp = MAX_TP_USER_DATA;
				}

				pdu.tpUDL = nTemp;
				bzero(pdu.tpUD.data, MAX_TP_USER_DATA);
				memcpy(pdu.tpUD.data, cTemp, nTemp);
			}
			else
			{
				setGSMTPDUErrorCode(ENC_ICONV_ERROR);
				return TPDU_FALSE;
			}

			break;

		case DCS_TYPE_GSM7BIT:
			/* Convert ASCII-8bit into GSM-7bit */
			stASCIIString  	stSource;
			stGSM7bitString stTarget;
			bzero(&stSource, sizeof(stASCIIString));
			bzero(&stTarget, sizeof(stGSM7bitString));

			memcpy(stSource.data, pdu.tpUD.data, pdu.tpUDL);		
			stSource.nCharacter = pdu.tpUDL;
			Conv8bit2GSM7bit(&stSource, &stTarget);

			if(stTarget.nOctet > MAX_TP_USER_DATA)
			{
				stTarget.nOctet = MAX_TP_USER_DATA;
			}
			bzero(pdu.tpUD.data, sizeof(pdu.tpUD.data));
			memcpy(pdu.tpUD.data, stTarget.data, stTarget.nOctet);
			pdu.tpUDL = stTarget.nCharacter;
			pdu.tpUD.size = stTarget.nOctet;
			break;

		default:
			break;
	}

	// SC Routing 시에는 메시지 conversion 하지 않음.
	
    if(nStep-1) // (1 is UDHL position)  
	{

		if(getDcsType(pdu.tpDCS.value) != DCS_TYPE_GSM7BIT)
		{
			if(nUDHI_exist)
			{
				/* UDHI exist */
				cpTemp[0] = (nStep-1)+pdu.tpUD.data[0];
				memcpy(&cpTemp[nStep], &pdu.tpUD.data[1], pdu.tpUDL-1); // 0byte UDHL, UDL - 1(UDHL opsition) 
				nStep+=(pdu.tpUDL-1);
				pdu.tpUDL = nStep;
				memset(pdu.tpUD.data, 0x00, sizeof(pdu.tpUD.data));
				memcpy(pdu.tpUD.data, cpTemp, pdu.tpUDL);
			}
			else
			{
				/* UDHI not exist */
				cpTemp[0] = nStep-1; // - UDHL
				memcpy(&cpTemp[nStep], pdu.tpUD.data, pdu.tpUDL);
				nStep+=pdu.tpUDL;
				pdu.tpUDL = nStep;
				memset(pdu.tpUD.data, 0x00, sizeof(pdu.tpUD.data));
				memcpy(pdu.tpUD.data, cpTemp, pdu.tpUDL);
			}
		}
		else
		{
			/* GSM 7bit */
			int nShiftP 	= 0;
			int	nUDHL		= nStep;
			int nRealUDOctet = pdu.tpUD.size;
			int nRealUDHOctet = 0;

			/* Get UDHL character number */
			int nUDHCharNum = (nStep*8)/7;
			if((nStep*8)%7 > 0)
			{
				nUDHCharNum++;
			}

			/* UDHI not exist */
			cpTemp[0] = nUDHL-1; // - UDHL
			/* Get Shift start point */
			nShiftP = (nUDHCharNum*7) - (nUDHL*8);

			/* Total character number */
			pdu.tpUDL += nUDHCharNum;
			nRealUDHOctet = (pdu.tpUDL*7)/8;
			if((pdu.tpUDL*7)%8)
			{
				nRealUDHOctet++;
			}
			pdu.tpUD.size = nRealUDHOctet;

			int nIndex1=0, nIndex2=0;
			u_char ucTemp = 0;
			while(nRealUDOctet > nIndex2)
			{
				cpTemp[nStep+nIndex1] |= (pdu.tpUD.data[nIndex2] << nShiftP);
				if(nShiftP != 0)
				{
					nIndex1++;
					cpTemp[nStep+nIndex1] |= 
						(pdu.tpUD.data[nIndex2] >> (8-nShiftP));
					nIndex2++;
				}
				else
				{
					nIndex1++;
					nIndex2++;	
				}
			}
			/*
			DebugStr(4, "[%s:%d] nIndex1[%d], nIndex1[%d] \n", __FILE__, __LINE__, 
					nIndex1, nIndex2);
			*/
			bzero(pdu.tpUD.data, sizeof(pdu.tpUD.data));
			memcpy(pdu.tpUD.data, cpTemp, pdu.tpUD.size);
		}

	}

	/* ---------------------------------------------------------------------- */	
	/* 		Encoding TP-UDL Parameter 										  */
	/* ---------------------------------------------------------------------- */	
    *pt = pdu.tpUDL;
	if(pdu.tpUDL > 0)
	{
		u_int unRealUDL = 0;
		pt++;
		retSize++;

		switch(getDcsType(pdu.tpDCS.value))
		{
			case DCS_TYPE_GSM7BIT:
				unRealUDL = pdu.tpUD.size;
				break;
			case DCS_TYPE_KSC5601:
			case DCS_TYPE_UNKNOWN:
			case DCS_TYPE_8BIT:
			case DCS_TYPE_UCS2:
			case DCS_TYPE_DC_DM:
			default:
				unRealUDL = (int)pdu.tpUDL;
				break;
		}

		if(unRealUDL > stMapParam.nBit8UDMaxSize)
		{
			unRealUDL = stMapParam.nBit8UDMaxSize;
		}

		memcpy((u_char*)pt, GetUDParamValue(pdu.tpUD), unRealUDL);
		retSize += unRealUDL;

	}

	/* ---------------------------------------------------------------------- */	
	/* 		Result of  Encoding Data  										  */
	/* ---------------------------------------------------------------------- */	
	memcpy((u_char*)buff,(u_char*)&tmpBuff,retSize);

	return retSize;
}

/** @} */
