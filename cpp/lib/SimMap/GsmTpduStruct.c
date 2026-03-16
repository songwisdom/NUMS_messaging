/**
 * =====================================================================================
 *
 *       @file  GsmTpduStruct.c
 *
 *    	 @brief 
 *
 *       Version:  1.0
 *       Compiler:  cc (HP-UX)
 *       @date:  2009년 05월 15일 00시 49분 59초
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
#include "flk_type.h"
#include "libGsmTpduStruct.h"
#include "flk_iconv.h"
#include "Debug.h"

static char *vpTypeString[]={
    "Field not present", "Enhanced Format", "Relative Format",
    "Absolute Format"
};

static char	*eTPAddrFieldNPIStr[]={
	"Unknown", "ISDN/Telephone Numbering Plan","Reserved","Data Numbering Plan",
	"Telex Numbering Plan", "Service Centre Specific Plan","Service Centre Specific Plan",
	"Reserved",
	"Naitional numbering plan","Private numbering plan","ERMES numbering plan",
	"Reserved","Reserved","Reserved","Reserved","Reserved"
};

static char	*eTPAddrFieldTypeStr[]={
	"Unknown","International Number","National Number","Network Specific Number",
	"Subscriber Number","Alphanumeric","Abbreviated number","Reserved",
	"Reserved","Reserved","Reserved","Reserved",
	"Reserved","Reserved","Reserved","Reserved"
};

static char *eTPPIDAssignString[]={
	"include indicates telematic interworking",
	"not include indicates telematic interworking",
	"reserved",
	"for SC specific use"
};

static char *eTPPIDInterString[]={
	" no interworking, but SME-to-SME protocol","telematic interworking"
};

static char *eTPPIDA1String[]={
	"implicit device type is specfic to the SC \n\t\t\t\t\tor can be conclued on the basis of the address",
	"telex(or teletex reduced to telex format)", 
	"group 3 telefax",
	"group 4 telefax",
	"voice telephone",
	"European Radio Messaging System", 
	"National Paging System(known to the SC)", 
	"Videotex(T.100/T.101)",
	"teletex,carrier unspecified", 
	"teletex, in PSPDN",
	"teletex, in CSPDN", 
	"teletex, in analog PSTN", 
	"teletex, in digital ISDN", 
	"Universal Computer Interface","reserved","reserved",
	"Message Handling Facility(known to the SC)",
	"any public x.400-based message handling system",
	"Internet Electronic Mail",//18
	"reserved","reserved","reserved","reserved","reserved","reserved","reserved",
	"reserved","reserved","reserved","reserved","reserved",
	"A GSM/UMTS mobile station" 
};

static char *eTPPIDA2String[]={
	"Short Message Type",	"Replace Short Message Type 1",
	"Replace Short Message Type 2",	"Replace Short Message Type 3",
	"Replace Short Message Type 4",	"Replace Short Message Type 5",
	"Replace Short Message Type 6",	"Replace Short Message Type 7",
	"reserved","reserved","reserved","reserved","reserved","reserved","reserved",
	"reserved","reserved","reserved","reserved","reserved","reserved","reserved",
	"reserved","reserved","reserved","reserved","reserved","reserved","reserved",
	"reserved",
	"Enhanced Message Service (Obsolete),Decimal 30",
	"Return Call Message",
	"reserved","reserved","reserved","reserved","reserved","reserved","reserved", //38
	"reserved","reserved","reserved","reserved","reserved","reserved","reserved", //45
	"reserved","reserved","reserved","reserved","reserved","reserved","reserved", //52
	"reserved","reserved","reserved","reserved","reserved",
	"ANSI-124 R-DATA", //58
	"ME Data download",
	"ME De-personalization Short Message",
	"(U)SIM Data download"
};

static char *eTPDCSGrpString[]={
	"General Group :\n\t\t\t\t\ttext is uncompressed and no message class\n\t\t\t\t\t(0 and 1 bit) not used",
	"General Group :\n\t\t\t\t\ttext is uncompressed and message class\n\t\t\t\t\t(0 and 1 bit)meaning",
	"General Group :\n\t\t\t\t\ttext is compressed and no message class\n\t\t\t\t\t(0 and 1 lbit not used)",
	"General Group :\n\t\t\t\t\ttest is compressed and message class\n\t\t\t\t\t(0 and 1 bit ) meaning",
	"Message Marked for Automatic Deletion Group :\n\t\t\t\t\ttext is uncompressed and no message class\n\t\t\t\t\t(0 and 1bit not used",
	"Message Marked for Automatic Deletion Group :\n\t\t\t\t\ttext is uncompressed and message class\n\t\t\t\t\t(0 and 1bit)meaning",
	"Message Marked for Automatic Deletion Group :\n\t\t\t\t\ttext is compressed and no message class\n\t\t\t\t\t(0 and 1bit not used)",
	"Message Marked for Automatic Deletion Group :\n\t\t\t\t\ttest is compressed and message class\n\t\t\t\t\t(0 and 1 bit ) meaning",
	"Reserved (KSC5601 and ASCII 8bit)", 
	"Reserved", 
	"Reserved", 
	"Reserved", 
	"Message Waiting Indication Group : Discard Message",
	"Message Waiting Indication Group : Store Message",
	"Message Waiting Indication Group : Store Message(UCS2)",
	"Data Coding/message Class Group"
};

static char *eTPDCSClassString[]={
	"Class 0",	"Class 1: default meaning: ME-specific",
	"Class 2: (U)SIM-specific message", 
	"Class 3: default meaning: TE specific (see 3GPP TS 27.005 [3]"
};

static char *eTPDCSCharString[]={
	"GSM 7 bit defualt alphabet" , "8 bits data","UCS2 (16bit)[10]"
};

static char *eTPDCSSenseString[]={
	"Set Indication Inactive","Set Indication Active"
};

static char *eTPDCSITypeString[]={
	"Voicemail Message Waiting",	"Fax Message Waiting",
	"Electronic Mail Message Waiting","Other Message Waiting"
};

static char*	tpdu_errString[]={
	"Unknown Error",	"Invalide Argument","Unknown TPDU Messsage",
	"TPDU Message Type is wrong", "TPDU Address Parameter Length Error",
	"TPDU Invalid Data Format",
	"TPDU Format Error",
	"TPDU Convert 7bits data to 8bit data Error",
	"TPDU UserData Size Error",
	"TPUD Command Type is Wrong Value",
	"TPDU_Convert_Size_Over"
};

int code_GSM_TPDU_Error;

char *func_tpdu_errString()
{
	return tpdu_errString[code_GSM_TPDU_Error];
}

int flk_convert_contents_old (
		const char 	*cpCType, 
		char 		*cpCData,
		size_t 		nCDataLen, 
		const char 	*cpTType, 
		char		*cpTData,
		size_t 		*npTDataLen);
/*----------------------------------------------------------------------------*
  -  TP - MTI Parameter Function
 *----------------------------------------------------------------------------*/
char*   printBitString(unsigned char value, int position, int length)
{
	static  char    bitString[9];
	int             isfill=0,remain = 0;
	int             i=0,checkvalue=1;

	isfill = position;
	remain = length;
	if(remain == 0) remain = 1;
	memset((char*)&bitString,'-',8);
	for(i=0;i<8;i++){
		if(isfill == i && remain > 0){
			if(value == 'x') bitString[7-i] = 'X';
			else if(value == 'X') bitString[7-i] = 'X';
			else if(value & checkvalue) bitString[7-i] = '1';
			else bitString[7-i] = '0';
			remain = remain-1;
			if(remain>0) isfill++;
		}
		checkvalue = checkvalue*2;
	}  
	return (bitString);
}
void	printParamReserved( int position, int size)
{
	log_write ( LEV_ALL_PARAM,"             [%s]\t\t\tNOT USED\n",
			printBitString('x',position,size));
}

void	printParamTPMTIValue(unsigned char cval,int value)
{
	log_write ( LEV_ALL_PARAM,"\n");
	switch(value){
		case	TPDU_SMS_DELIVER :
			log_write ( LEV_ALL_PARAM,"   [TP-MTI ] [%s]\t\t\tSMS-DELIVER/SMS-DELIVER-REPORT\n",
					printBitString(cval,0,2));
			break;
		case	TPDU_SMS_SUBMIT	:
			log_write ( LEV_ALL_PARAM,"   [TP-MTI ] [%s]\t\t\tSMS-SUBMIT/SMS-SUBMIT-REPORT\n",
					printBitString(cval,0,2));
			break;
		case	TPDU_SMS_STATUS_REPORT	:
			log_write ( LEV_ALL_PARAM,"   [TP-MTI ] [%s]\t\t\tSMS-STATUS-REPORT/SMS_COMMAND\n",
					printBitString(cval,0,2));
			break;
		case	TPDU_SMS_RESERVED	:
			log_write ( LEV_ALL_PARAM,"   [TP-MTI ] [%s]\t\t\tSMS-RESERVED\n",
					printBitString(cval,0,2));
			break;
		default	: break;
	}
	return;
}
/*----------------------------------------------------------------------------*
  -  TP - MMS Parameter Function
 *----------------------------------------------------------------------------*/
void	printParamTPMMSValue(unsigned char cval,int value)
{
	switch(value){
		case	TPDU_MM_WAIT :
			log_write ( LEV_ALL_PARAM,"   [TP-MMS ] [%s]\t\t\tMore Messages are waiting for the MS in this SC\n",
					printBitString(cval,2,1));
			break;
		case	TPDU_NO_MM_WAIT:
			log_write ( LEV_ALL_PARAM,"   [TP-MMS ] [%s]\t\t\tNo More Messages are wating for the MS in this SC\n",
					printBitString(cval,2,1));
			break;
		default	: break;
	}
	return;
}

/*----------------------------------------------------------------------------*
  -  TP - RP Parameter Function
 *----------------------------------------------------------------------------*/
void	printParamTPRPValue(unsigned char cval,int value)
{
	switch(value){
		case	TPDU_NOT_RPSET :
			log_write ( LEV_ALL_PARAM,"   [TP-RP  ] [%s]\t\t\tTP-Reply-Path parameter is not set\n",
					printBitString(cval,7,0));
			break;
		case	TPDU_RPSET:
			log_write ( LEV_ALL_PARAM,"   [TP-RP  ] [%s]\t\t\tTP-Reply-Path parameter is set\n",
					printBitString(cval,7,0));
			break;
		default	: break;
	}
	return;
}

/*----------------------------------------------------------------------------*
  -  TP - RD Parameter Function
 *----------------------------------------------------------------------------*/
void	printParamTPRDValue(unsigned char cval,int value)
{
	switch(value){
		case	SC_TO_ACCEPT :
			log_write ( LEV_ALL_PARAM,"   [TP-RD  ] [%s]\t\t\tInstruct the SC to accept an SMS-SUBMIT\n",
					printBitString(cval,2,1));
			break;
		case	SC_TO_REJECT:
			log_write ( LEV_ALL_PARAM,"   [TP-RD  ] [%s]\t\t\tInstruct the SC to reject an SMS-SUBMIT\n",
					printBitString(cval,2,1));
			break;
		default	: break;
	}
	return;
}
/*----------------------------------------------------------------------------*
  -  TP - SRR Parameter Function
  -  argument is TP_SRRParam*, int eTPSRRFieldValue  
 *----------------------------------------------------------------------------*/
void	printParamTPSRRValue(unsigned char cval,int value)
{
	switch(value){
		case	STATUS_REPORT_NOT_REQUEST :
			log_write ( LEV_ALL_PARAM,"   [TP-SRR ] [%s]\t\t\tA status report is not requested\n",
					printBitString(cval,5,1));
			break;
		default	:
			log_write ( LEV_ALL_PARAM,"   [TP-SRR ] [%s]\t\t\tA status report is requested\n",
					printBitString(cval,5,1));
			break;
	}
	return;
}

/*----------------------------------------------------------------------------*
  -  TP - SRI Parameter Function
 *----------------------------------------------------------------------------*/
void	printParamTPSRIValue(unsigned char cval,int value)
{
	switch(value){
		case	TPDU_NOT_RETURN_SME :
			log_write ( LEV_ALL_PARAM,"   [TP-SRI ] [%s]\t\t\tA status report shall not be returned to the SME\n",
					printBitString(cval,5,1));
			break;
		default	:
			log_write ( LEV_ALL_PARAM,"   [TP-SRI ] [%s]\t\t\tA status report shall be a returned to the SME\n",
					printBitString(cval,5,1));
			break;
	}
	return;
}

/*----------------------------------------------------------------------------*
  -  TP - SRQ Parameter Function
 *----------------------------------------------------------------------------*/
void	printParamTPSRQValue(unsigned char cval,int value)
{
	switch(value){
		case	TPDU_RESULT_SUBMIT :
			log_write ( LEV_ALL_PARAM,"   [TP-SRQ ] [%s]\t\t\tThe SMS-STATUS-REPORT is the result of a SMS-SUBMIT\n",
					printBitString(cval,5,1));
			break;
		default	:
			log_write ( LEV_ALL_PARAM,"   [TP-SRQ ] [%s]\t\t\tThe SMS-STATUS-REPORT is the result of an SMS-COMMAND\n",
					printBitString(cval,5,1));
			break;
	}
	return;
}

/*----------------------------------------------------------------------------*
  -  TP - UDHI Parameter Function
 *----------------------------------------------------------------------------*/
void	printParamTPUDHIValue(unsigned char cval,int value)
{

	switch(value){
		case	TPDU_ONLY_SHORT_MSG :
			log_write ( LEV_ALL_PARAM,"   [TP-UDHI] [%s]\t\t\tcontains only the short message\n",
					printBitString(cval,6,1));
			break;
		default	:
			log_write ( LEV_ALL_PARAM,"   [TP-UDHI] [%s]\t\t\tcontains Header in addition to the short message\n",
					printBitString(cval,6,1));
			break;
	}
	return;
}

/*----------------------------------------------------------------------------*
  -  TP - VPF Parameter Function
 *----------------------------------------------------------------------------*/
void	printParamTPVPFValue(unsigned char cval,int value)
{
	switch(value){
		case	TPDU_NOT_PRESENT:
			log_write ( LEV_ALL_PARAM,"   [TP-VPF ] [%s]\t\t\tTP-VP field not present\n",
					printBitString(cval,3,2));
			break;
		case	TPDU_PRESENT_INTEGER:
			log_write ( LEV_ALL_PARAM,"   [TP-VPF ] [%s]\t\t\tTP-VP field present and integer represented\n",
					printBitString(cval,3,2));
			break;
		case	TPDU_SPARE:
			log_write ( LEV_ALL_PARAM,"   [TP-VPF ] [%s]\t\t\tTP-VP SPARE\n",
					printBitString(cval,3,2));
			break;
		case	TPDU_PRESENT_SEMI_OCTET:
			log_write ( LEV_ALL_PARAM,"   [TP-VPF ] [%s]\t\t\tTP-VP filed present and semi-octet represented\n",
					printBitString(cval,3,2));
			break;
	}
	return;
}

/*----------------------------------------------------------------------------*
  -  TP -OA/DA/RA Parameter Function
 *----------------------------------------------------------------------------*/
unsigned char* getParamTPAddress(TP_OAParam *Addr)
{
	return (unsigned char*)&Addr->address;
}

char*	decParamBCDCallBackAddress(unsigned char *data,int cnt)
{
	SemiOctet		*pSo=NULL;
	int		i=0,idx=0;
	static	char bcdBuff[1024];
	memset((char*)&bcdBuff,0x00,1024);

	for(i=0;i<cnt;i++)
	{
		pSo = (SemiOctet*)&data[i];
		bcdBuff[idx] = getParamCBAddrChar(pSo->digit2nd);
		bcdBuff[idx+1] = getParamCBAddrChar(pSo->digit1st);
		idx += 2; 
	}

	return (bcdBuff);		
}

/*
   char*	decParamBCDAddress(unsigned char *data,int cnt)
   {
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
   }
 */
/* --------------------------------*/
/* 2006-01-21 new function */
/*
   char*   decParamBCDAddress2(unsigned char *data,int cnt)
   {
   SemiOctet       *pSo=NULL;
   int     i=0,idx=0,j=0;
   static  char bcdBuff[1024];
   memset((char*)&bcdBuff,0x00,1024);

   if(data[0] == 0x92){
   bcdBuff[idx] = '0';
   idx = 1;
   j=1;
   }   
   else j=0;
   for(i=j;i<cnt;i++){
   pSo = (SemiOctet*)&data[i];
   bcdBuff[idx] = getParamAddrChar(pSo->digit2nd);
   bcdBuff[idx+1] = getParamAddrChar(pSo->digit1st);
   idx += 2;
   }
   if(idx%2!=0 && bcdBuff[idx-1] != ' ')
   bcdBuff[idx]='F';
   return (bcdBuff);
   }
 */
/* end new function */

char	getParamAddrChar(int value)
{
	char	ret=0x00;
	switch(value){
		case		0	:	ret = '0'; break;
		case		1	:	ret = '1'; break;
		case		2	:	ret = '2'; break;
		case		3	:	ret = '3'; break;
		case		4	:	ret = '4'; break;
		case		5	:	ret = '5'; break;
		case		6	:	ret = '6'; break;
		case		7	:	ret = '7'; break;
		case		8	:	ret = '8'; break;
		case		9	:	ret = '9'; break;
		case		10	:	ret = '*'; break;
		case		11	:	ret = '#'; break;
		case		12	:	ret = 'a'; break;
		case		13	:	ret = 'b'; break;
		case		14	:	ret = 'c'; break;
		case		15	:	ret = 'F'; break;
		default		: ret = ' '; break;
	}
	return ret;
}

char	getParamCBAddrChar(int value) // CDMA 
{
	char	ret=0x00;
	switch(value){
		case		0	:	ret = '0'; break;
		case		1	:	ret = '1'; break;
		case		2	:	ret = '2'; break;
		case		3	:	ret = '3'; break;
		case		4	:	ret = '4'; break;
		case		5	:	ret = '5'; break;
		case		6	:	ret = '6'; break;
		case		7	:	ret = '7'; break;
		case		8	:	ret = '8'; break;
		case		9	:	ret = '9'; break;
		case		10	:	ret = 'a'; break;
		case		11	:	ret = '*'; break;
		case		12	:	ret = '#'; break;
		case		15	:	ret = 'F'; break;
		default		: ret = ' '; break;
	}
	return ret;
}

void	printParamTPAddress(TP_OAParam *Addr)
{
	int		i=0,len=0;
	if(Addr == NULL) return;

	log_write ( LEV_ALL_PARAM,"\n   [ADDRESS PARAMETER]\n");
	log_write ( LEV_ALL_PARAM,"             [%s]\t\t\tAddress Length: %d\n",
			printBitString(Addr->addrLen,0,8),Addr->addrLen);
	log_write ( LEV_ALL_PARAM,"             [%s]\t\t\tType Of Number: %s\n",
			printBitString(Addr->pAddrIdc,4,4),
			eTPAddrFieldTypeStr[Addr->pAddrType&0x7]);
	log_write ( LEV_ALL_PARAM,"             [%s]\t\t\tNumbering Plan: %s\n",
			printBitString(Addr->pAddrIdc,0,4),
			eTPAddrFieldNPIStr[Addr->pAddrNumPlan]);

	len = Addr->addrOctetSize;
	log_write ( LEV_ALL_PARAM,"             [**ADDR**]\t\t\tContext :  ");
	for(i=0;i<len;i++){
		log_write ( LEV_ALL_PARAM,"%c",getParamAddrChar(Addr->address[i].digit2nd));
		log_write ( LEV_ALL_PARAM,"%c",getParamAddrChar(Addr->address[i].digit1st));
	}
	log_write ( LEV_ALL_PARAM,"\n");
	log_write ( LEV_ALL_PARAM,"            \t\t\t\tH' ");
	for(i=0;i<len;i++){
		log_write ( LEV_ALL_PARAM,"%02X ",Addr->address[i].digit);
	}	
	log_write ( LEV_ALL_PARAM,"\n\n");
	return;
}

int		encParamTPAddress(unsigned char *buff,TP_OAParam *Addr)
{
	int	size=0, remain = 0, idx=0;
	SemiOctet		*pso=NULL;

	if(buff == NULL || Addr == NULL){
		setGSMTPDUErrorCode(SET_INVALID_VALUE);
		return TPDU_FALSE;
	}
	if(Addr->addrOctetSize == 0)
	{
		Addr->addrOctetSize = Addr->addrLen/2 + (Addr->addrLen%2);
	}
	if(Addr->addrLen > 20){
		unsigned int failLen = Addr->addrLen;
		log_write(LEV_ALL_PARAM, "[%s:%d] TPDU Address Length Fail len[%d]", 
				__func__, __LINE__, failLen);
		setGSMTPDUErrorCode(ERR_TPDU_ADDRESS_LEN);
		return TPDU_FALSE;
	}
	remain = Addr->addrOctetSize;

	buff[size++] = Addr->addrLen;
	buff[size++] = Addr->pAddrIdc;

	while(remain){
		pso = (SemiOctet*)&buff[size];
		pso->digit1st = Addr->address[idx].digit2nd;
		pso->digit2nd = Addr->address[idx].digit1st;
		size++; idx++;
		remain--;
	}	
	return size;
}

int		encParamTPCallbackAddress(unsigned char *buff,TP_OAParam *Addr)
{
	int	size=0, remain = 0, idx=0;
	SemiOctet		*pso=NULL;

	if(buff == NULL || Addr == NULL){
		setGSMTPDUErrorCode(SET_INVALID_VALUE);
		return TPDU_FALSE;
	}
	if(Addr->addrOctetSize == 0)
	{
		Addr->addrOctetSize = Addr->addrLen/2 + (Addr->addrLen%2);
	}
	if(Addr->addrLen > 20){
		unsigned int failLen = Addr->addrLen;
		log_write(LEV_ALL_PARAM, "[%s:%d] TPDU Address Length Fail len[%d]", 
				__func__, __LINE__, failLen);
		setGSMTPDUErrorCode(ERR_TPDU_ADDRESS_LEN);
		return TPDU_FALSE;
	}
	remain = Addr->addrOctetSize;

	buff[size++] = Addr->addrLen;
	buff[size++] = Addr->pAddrIdc;

	while(remain){
		pso = (SemiOctet*)&buff[size];
		if(Addr->address[idx].digit2nd == 0x0a) 
		{
			Addr->address[idx].digit2nd = 0;	
		}
		else if(Addr->address[idx].digit2nd == 0x0b)
		{
			Addr->address[idx].digit2nd = 0x0a;
		}
		else if(Addr->address[idx].digit2nd == 0x0c)
		{
			Addr->address[idx].digit2nd = 0x0b;
		}
		pso->digit1st = Addr->address[idx].digit2nd;
		if(Addr->address[idx].digit1st == 0x0a) 
		{
			Addr->address[idx].digit1st = 0;
		}
		else if(Addr->address[idx].digit1st == 0x0b)
		{
			Addr->address[idx].digit1st = 0x0a;
		}
		else if(Addr->address[idx].digit1st == 0x0c)
		{
			Addr->address[idx].digit1st = 0x0b;
		}
		pso->digit2nd = Addr->address[idx].digit1st;
		size++; idx++;
		remain--;
	}	
	return size;
}

int		decParamTPCallBackAddress(unsigned char *data, TP_OAParam *Addr)
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

	/* modify by hkkim 2007.09.17 */
	// if(Addr->addrOctetSize > 11 || Addr->addrLen < 2 || Addr->addrLen >  20) 
	/******************************/
	if((Addr->addrOctetSize > 11) || (Addr->addrLen > 20))                   
	{
		unsigned int failLen = Addr->addrLen;
		unsigned int octetLen = Addr->addrOctetSize;
		log_write(LEV_ALL_PARAM, "[%s:%d] TPDU Address Length Fail octetSize[%d] len[%d]", 
				__func__, __LINE__, octetLen, failLen);
		setGSMTPDUErrorCode(ERR_TPDU_ADDRESS_LEN);
		return TPDU_FALSE;
	}

	while(remain)
	{
		pso = (SemiOctet*)&data[idx++];
		if(pso->digit2nd == 0x00) 
		{
			pso->digit2nd = 0x0a;	
		}
		else if(pso->digit2nd == 0x0a)	// gsm	'*'
		{
			pso->digit2nd = 0x0b;  		// cdma	'*'
		}
		else if(pso->digit2nd == 0x0b)	// gsm	'#'
		{
			pso->digit2nd = 0x0c; 		// cdma	'#'
		}
		Addr->address[addrPt].digit1st = pso->digit2nd; 
		if(pso->digit1st == 0x00) 
		{
			pso->digit1st = 0x0a;	
		}
		else if(pso->digit1st == 0x0a)	// gsm	'*'
		{
			pso->digit1st = 0x0b;  		// cdma	'*'
		}
		else if(pso->digit1st == 0x0b)	// gsm 	'#'
		{
			pso->digit1st = 0x0c; 		// cdma	'#'
		}
		Addr->address[addrPt++].digit2nd = pso->digit1st; 
		remain--;
	}
	return	idx;	
}

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

	/*
	if(Addr->addrOctetSize > 11 || Addr->addrLen < 2 || Addr->addrLen > 20) 
	{
		unsigned int failLen = Addr->addrLen;
		unsigned int octetLen = Addr->addrOctetSize;
		log_write(LEV_ALL_PARAM, "[%s:%d] TPDU Address Length Fail octetSize[%d] len[%d]", 
				__func__, __LINE__, octetLen, failLen);
		setGSMTPDUErrorCode(ERR_TPDU_ADDRESS_LEN);
		return TPDU_FALSE;
	}
	*/

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

int     decParamTPAddress2(unsigned char *data, TP_OAParam *Addr)
{
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
int     decParamTPAddress3(unsigned char *data, TP_OAParam *Addr){
	int     idx=0, remain=0, addrPt=0;
	SemiOctet   *pso=NULL;

	if(data== NULL || Addr == NULL){
		return -1;
	}
	memset((SemiOctet*)&Addr->address,0xFF,TPDU_ADDRESS_LENGTH);

	Addr->addrLen = data[idx++];

	Addr->pAddrIdc = data[idx++];

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
		Addr->addrLen -= 1;
		idx++;
		pso = (SemiOctet*)&data[idx++];
		Addr->address[addrPt].digit2nd = 0;
		Addr->address[addrPt++].digit1st = pso->digit1st;
		while(remain){
			if(pso->digit2nd == 0x0f) break;
			Addr->address[addrPt].digit2nd = pso->digit2nd;
			if(pso->digit1st == 0x0f) break;
			pso = (SemiOctet*)&data[idx++];
			Addr->address[addrPt++].digit1st = pso->digit1st;
			remain--;
		}
	}
	return  Addr->addrOctetSize;
}


char	*getParamDecBCDAddress(TP_OAParam *addr)
{
	if(addr == NULL) 
	{
		return NULL;
	}
	return decParamBCDAddress((unsigned char*)&addr->address,addr->addrOctetSize);
}

/*----------------------------------------------------------------------------*
  -  TP - DCS Parameter Function
 *----------------------------------------------------------------------------*/
void	printParamDCSValue(TP_DCSParam *param)
{
	if(param == NULL){
		return;
	}
	log_write ( LEV_ALL_PARAM,"   [TP-DCS]  [%s]\t\t\t%s\n",
			printBitString(param->value,4,4),eTPDCSGrpString[param->dcsGroup]);
	switch(param->dcsGroup){
		case	DCS_GEN_UNCOMP_NO_CLASS_GROUP		:
		case	DCS_GEN_UNCOMP_USE_CLASS_GROUP		:
		case	DCS_GEN_COMP_NO_CLASS_GROUP			:
		case	DCS_GEN_COMP_USE_CLASS_GROUP		:
		case	DCS_MSGMARG_UNCOMP_NO_CLASS_GROUP	:
		case	DCS_MSG_CODING_DATA_CLASS_GROUP		:
			log_write ( LEV_ALL_PARAM,"             [%s]\t\t\tCharset: %s\n",
					printBitString(param->value,2,2),
					eTPDCSCharString[param->dcsCharset]);
			log_write ( LEV_ALL_PARAM,"             [%s]\t\t\tClass: %s\n\n",
					printBitString(param->value,0,2),
					eTPDCSClassString[param->dcsClass]);
			break;
		case	DCS_MSGMARG_UNCOMP_USE_CLASS_GROUP	:
		case	DCS_MSGMARG_COMP_NO_CLASS_GROUP		:
		case	DCS_MSGMARG_COMP_USE_CLASS_GROUP 	:  
		case	DCS_MSG_WAIT_DISCARD_GROUP			:
		case	DCS_MSG_WAIT_STORE_GROUP			:
		case	DCS_MSG_WAIT_STORE_UCS2_GROUP		:
			log_write ( LEV_ALL_PARAM,"             [%s]\t\t\tSense: %s\n",
					printBitString(param->value,3,1),
					eTPDCSSenseString[param->dcsSense]);
			log_write ( LEV_ALL_PARAM,"             [%s]\t\t\tReserved\n",
					printBitString(param->value,2,1));
			log_write ( LEV_ALL_PARAM,"             [%s]\t\t\tType: %s\n\n",
					printBitString(param->value,0,2),
					eTPDCSITypeString[param->dcsClass]);
			break;
		default	: break;
	}
	return;	
}

/*----------------------------------------------------------------------------*
  -  TP - SCTS Parameter Function
 *----------------------------------------------------------------------------*/
void	printParamSCTSValue(TP_SCTSParam *param,int opt){
	int	i=0;
	if(param == NULL) return;
	if(opt == 1)

		log_write ( LEV_ALL_PARAM,"                       \t\t\tH'");
	else if(opt == 2)
		log_write ( LEV_ALL_PARAM,"   [ TP-DT ] [** DT **]\t\t\tH'");
	else
		log_write ( LEV_ALL_PARAM,"   [TP-SCTS] [**SCTS**]\t\t\tH'");
	for(i=0;i<7;i++)
		log_write ( LEV_ALL_PARAM,"%02X ",param->value[i].digit);
	log_write ( LEV_ALL_PARAM,"\n");	
	log_write ( LEV_ALL_PARAM,"            \t          \t\tTime: ");
	for(i=0;i<7;i++){
		log_write ( LEV_ALL_PARAM,"%d%d",param->value[i].digit2nd,param->value[i].digit1st);
		if(i<2) log_write ( LEV_ALL_PARAM,"/");
		else if(i==2 || i==5) log_write ( LEV_ALL_PARAM," ");
		else if(i==3 || i==4) log_write ( LEV_ALL_PARAM,":");
	}
	log_write ( LEV_ALL_PARAM,"\n\n");	
	return;
}
void	setParamSCTSValue(unsigned char *buf,unsigned char *sctsval){
	SemiOctet	*pb,*ps;
	unsigned char tmp[7];

	int	i=0;
	memcpy(&tmp,sctsval,7);
	for(i=0;i<6;i++){
		pb = (SemiOctet*)buf;
		ps = (SemiOctet*)&tmp[i];
		pb->digit1st = ps->digit2nd;
		pb->digit2nd = ps->digit1st;
		buf++;
	}
}

unsigned char*	getParamSCTSValue(unsigned char *param){
	static	unsigned char retSCTS[6];
	SemiOctet		*pb,*ps;
	int			i=0;

	for(i=0;i<7;i++){
		pb = (SemiOctet*)&retSCTS[i];
		ps = (SemiOctet*)param;
		pb->digit1st = ps->digit2nd;
		pb->digit2nd = ps->digit1st;
		param++;
	}	
	return (retSCTS);
}

/*----------------------------------------------------------------------------*
  -  TP - VP Parameter Function
 *----------------------------------------------------------------------------*/
/* 0 -sec, 1- min, 2- hour ,3 - day*/

unsigned long	getRelativeTime(TP_VPParam *param,int timetype){
	unsigned long	Ret =0;

	if(param->vpRelative<144)
		Ret = DRLTIME1(param->vpRelative);
	else if(143< param->vpRelative && param->vpRelative<168)
		Ret = DRLTIME2(param->vpRelative);
	else if(167< param->vpRelative && param->vpRelative<197)
		Ret = DRLTIME3(param->vpRelative);
	else
		Ret = DRLTIME4(param->vpRelative);
	if(timetype == 1) Ret = Ret/60;
	else if(timetype == 2) Ret = (Ret/60/60);
	else if(timetype == 3) Ret = (Ret/60/60/24);

	return Ret;
}

unsigned long	setRelativeTime(TP_VPParam *param,int sec){
	unsigned long	Ret=0;

	if( sec< DRLTIME1(144))
		Ret = ERLTIME1(sec);
	else if(DRLTIME2(143)< sec && sec<DRLTIME2(168))
		Ret = ERLTIME2(sec);
	else if(DRLTIME3(167)< sec && sec <DRLTIME3(197))
		Ret = ERLTIME3(sec);
	else
		Ret = ERLTIME4(sec);
	param->vpRelative = Ret;
	return Ret;
}

void	printParamVPValue(TP_VPParam *param){
	SemiOctet		*pso=NULL;
	if(param == NULL) return;
	log_write ( LEV_ALL_PARAM,"   [TP-VP]\t\t\t\t%s [%d]\n",
			vpTypeString[param->type],param->type);
	switch(param->type){
		case		VP_RELATIVE_TYPE	:
			log_write ( LEV_ALL_PARAM,"             [%s]\t\t\tH'%02X ",
					printBitString(param->vpRelative,0,8),param->vpRelative);
			if(param->vpRelative<144)
				log_write ( LEV_ALL_PARAM, "[%u]/min\n\n",(param->vpRelative+1)*5);
			else if(143< param->vpRelative && param->vpRelative<168)
				log_write ( LEV_ALL_PARAM, "[%u]/min\n\n",(12*60)+((param->vpRelative-143)*30));
			else if(167< param->vpRelative && param->vpRelative<197)
				log_write ( LEV_ALL_PARAM, "[%u]/day\n\n",(param->vpRelative-166));
			else
				log_write ( LEV_ALL_PARAM, "[%u]/week\n\n",(param->vpRelative-192));
			break;
		case		VP_ENHANCED_TYPE	:
			log_write ( LEV_ALL_PARAM,"             [%s]\t\t\tExtention bit\n",
					printBitString(param->vpEnIdcValue,7,1));
			log_write ( LEV_ALL_PARAM,"             [%s]\t\t\tSingle Short SM\n",
					printBitString(param->vpEnIdcValue,6,1));
			log_write ( LEV_ALL_PARAM,"             [%s]\t\t\tReserved\n",
					printBitString(param->vpEnIdcValue,3,3));
			log_write ( LEV_ALL_PARAM,"             [%s]\t\t\tFormat - %d\n",
					printBitString(param->vpEnIdcValue,0,3),param->vpEnFormat);
			log_write ( LEV_ALL_PARAM,
					"             [**DATA**]\t\t\tH' %02X %02X %02X %02X %02X %02X\n",
					param->vpValue[0],param->vpValue[1],param->vpValue[2],
					param->vpValue[3],param->vpValue[4],param->vpValue[5]);
			if(param->vpEnFormat==1){
				if(param->vpValue[0] <144)
					log_write ( LEV_ALL_PARAM,"\t\t\t\t\t[%u]/min\n\n",(param->vpEnValue[0]+1)*5);
				else if(143< param->vpValue[0] && param->vpValue[0]<168)
					log_write ( LEV_ALL_PARAM,"\t\t\t\t\t[%u]/min\n\n",(12*60)+((param->vpEnValue[0]-143)*30));
				else if(167< param->vpValue[0] && param->vpValue[0]<197)
					log_write ( LEV_ALL_PARAM,"\t\t\t\t\t[%u]/day\n\n",(param->vpEnValue[0]-166));
				else
					log_write ( LEV_ALL_PARAM,"\t\t\t\t\t[%u]/week\n\n",(param->vpEnValue[0]-192));
			}
			else if(param->vpEnFormat == 2){
				log_write ( LEV_ALL_PARAM,"\t\t\t\t\t[%u] seconds\n\n",param->vpEnValue[0]);
			}
			else if(param->vpEnFormat == 3){
				pso = (SemiOctet*)&param->vpEnValue[0];
				log_write ( LEV_ALL_PARAM,"\t\t\t\t\t[%d%d] hour ", pso->digit2nd,pso->digit1st);
				pso = (SemiOctet*)&param->vpEnValue[1];
				log_write ( LEV_ALL_PARAM,"[%d%d] min ", pso->digit2nd,pso->digit1st);
				pso = (SemiOctet*)&param->vpEnValue[2];
				log_write ( LEV_ALL_PARAM,"[%d%d] sec\n\n", pso->digit2nd,pso->digit1st);
			}
			else{
				log_write ( LEV_ALL_PARAM,"\t\t\t\tUnknown\n\n");
			}
			break;
		case		VP_ABSOLUTE_TYPE:
			//log_write ( LEV_ALL_PARAM,"            \t\t\t Absolute(SCTS) Format\n");
			printParamSCTSValue(&param->vpAbsolute,1);
			break;
		default		:	break;
	}
	return;
}

/*----------------------------------------------------------------------------*
  -  TP - PI Parameter Function
 *----------------------------------------------------------------------------*/
void	printParamPIValue(TP_PIParam *param){
	char	*piStr[]={
		"Not Exist", "Exist"
	};

	if(param == NULL) return;
	if(param->piExtention)
		log_write ( LEV_ALL_PARAM,"   [TP-PI]   [%s]\t\t\tExtention\n",
				printBitString(param->value,7,1));
	else
		log_write ( LEV_ALL_PARAM,"   [TP-PI]   [%s]\t\t\tNot Extention\n",
				printBitString(param->value,7,1));
	log_write ( LEV_ALL_PARAM,"             [%s]\t\t\tRESERVED\n",
			printBitString(param->value,3,4));
	log_write ( LEV_ALL_PARAM,"             [%s]\t\t\tTP-UDL %s\n",
			printBitString(param->value,2,1),
			piStr[param->piUdl]);
	log_write ( LEV_ALL_PARAM,"             [%s]\t\t\tTP-DCS %s\n",
			printBitString(param->value,1,1),
			piStr[param->piDcs]);
	log_write ( LEV_ALL_PARAM,"             [%s]\t\t\tTP-PID %s\n\n",
			printBitString(param->value,0,1),
			piStr[param->piPid]);
	return;
}

/*----------------------------------------------------------------------------*
  -  TP - PID Parameter Function
 *----------------------------------------------------------------------------*/

void	printParamPIDValue(TP_PIDParam *param){
	if(param == NULL) return;
	log_write ( LEV_ALL_PARAM,"   [TP-PID]  [%s]\t\t\t%s\n",
			printBitString(param->value,6,2),eTPPIDAssignString[param->pidType]);
	if(param->pidType == 0){
		log_write ( LEV_ALL_PARAM,"             [%s]\t\t\t%s\n",
				printBitString(param->value,5,1),
				eTPPIDInterString[param->pidInterworking]);
		log_write ( LEV_ALL_PARAM,"             [%s]\t\t\t%s\n\n",
				printBitString(param->value,0,5),eTPPIDA1String[param->pidValueIncIw]);
	}
	else{
		log_write ( LEV_ALL_PARAM,"             [%s]\t\t\t%s\n\n",
				printBitString(param->value,0,6),eTPPIDA2String[param->pidValue]);
	}
	return;
}

/*----------------------------------------------------------------------------*
  -  TP - ST Parameter Function
 *----------------------------------------------------------------------------*/
char*	getParamSTString(TP_STParam data)
{
	char	*ststr=NULL;
	switch(data){
		case	   TPDU_SM_RECEVIDE_BY_SME 		:
			ststr = "short message received by the SME";	break;
		case	   TPDU_SM_FORWARD_SC_TO_SME		:
			ststr = "short message forwarded by the SC to the SME but the SC is unable to confirm delivery "; break;
		case	   TPDU_SM_REPLACE_BY_SC		:
			ststr = "short message replaced by teh SC"; break;
		case	   TPDU_TERR_TRY_CONGESTION		:
			ststr = "Congestion"; break;
		case	   TPDU_TERR_TRY_SME_BUSY		:
			ststr = "SME Busy"; break;
		case	   TPDU_TERR_TRY_NO_RESPONSE		:
			ststr = "No response from SME"; break;
		case	   TPDU_TERR_TRY_SERVICE_REJECT		:
			ststr = "Service rejected "; break;
		case	   TPDU_TERR_TRY_TQOS_NOT_AVAIL		:
			ststr = "quality of service not available"; break;
		case	   TPDU_TERR_TRY_IN_SME		:
			ststr = "Error in SME"; break;
		case	   TPDU_PERR_REMOTE_PROCEDURE		:
			ststr = "procedure error (decimal 64)"; break;
		case	   TPDU_PERR_INCOMPATIBLE_DESTINATION		:
			ststr = "Incompatible destination"; break;
		case	   TPDU_PERR_CONNECTION_REJECT		:
			ststr = "connection rejected by SME"; break;
		case	   TPDU_PERR_NOT_OBTAINABLE		:
			ststr = "Not obtainable"; break;
		case	   TPDU_PERR_QOS_NOT_AVAIL		:
			ststr = "Qos not available"; break;
		case	   TPDU_PERR_NO_INTERWORKIN_AVAIL		:
			ststr = "No Interworking available"; break;
		case	   TPDU_SM_VPE		:
			ststr = "SM Validity Period Expired"; break;
		case	   TPDU_SM_DELETE_ORG_SME		:
			ststr = "SM Deleted by originating SME"; break;
		case	   TPDU_SM_DELETE_SC_ADMIN		:
			ststr = "SM Deleted by SC administration"; break;
		case	   TPDU_SM_NOT_EXIST		:
			ststr = "SM does not exist"; break;
		case	   TPDU_TERR_ANY_CONGESTION		:
			ststr = "Congestion"; break;
		case	   TPDU_TERR_ANY_SME_BUSY		:
			ststr = "SME Busy"; break;
		case	   TPDU_TERR_ANY_NO_RESPONSE		:
			ststr = "No response from SME"; break;
		case	   TPDU_TERR_ANY_SERVICE_REJECT		:
			ststr = "Service rejected"; break;
		case	   TPDU_TERR_ANY_TQOS_NOT_AVAIL		:
			ststr = "quality of service not available "; break;
		case	   TPDU_TERR_ANY_IN_SME		:
			ststr = "Error in SME "; break;
		default		: 
			ststr = "Reserved";	 break;
	}
	return ststr;
}

void	printParamSTValue(TP_STParam data){
	log_write ( LEV_ALL_PARAM,"   [TP-ST]   [%s]\t\t\t%s\n",
			printBitString(data,0,8),getParamSTString(data));
}

/*----------------------------------------------------------------------------*
  -  TP - CT Parameter Function
 *----------------------------------------------------------------------------*/
char	*getParamCTString(TP_CTParam data){
	char	*ctstr=NULL;

	switch(data){
		case	TPDU_CT_ENQUIRY		:
			ctstr = "The SMS-STATUS-REPORT is the result of a SMS-SUBMIT";	break;
		case	TPDU_CT_CANCEL		:
			ctstr = "The SMS-STATUS-REPORT is the result of an SMS-COMMAND";	break;
		case	TPDU_CT_DELETE		:
			ctstr = "delete previously submitted short message";	break;
		case	TPDU_CT_ENABLE		:
			ctstr = "enable status report request relating to previously";	break;
		default						:
			ctstr = "Reserved";		break;
	}
	return ctstr;
}

void	printParamCTValue(TP_CTParam data){
	log_write ( LEV_ALL_PARAM,"   [TP-CT]  [%s]\t\t\t%s\n",
			printBitString(data,0,8),getParamCTString(data));
}

/*----------------------------------------------------------------------------*
  -  TP - MN Parameter Function
 *----------------------------------------------------------------------------*/
void	printParamMNValue(TP_MNParam data){
	log_write ( LEV_ALL_PARAM,"   [TP-MN]  [%s]\t\t\tH' %02X\n",
			printBitString(data,0,8),data);
}


/*----------------------------------------------------------------------------*
  -  TP - FCS Parameter Function
 *----------------------------------------------------------------------------*/
char*	getParamFCSReason(TP_FCSParam data){
	char	*fcsStr=NULL;
	switch(data){
		case	TPDU_TELEINTERWORKING_NOT_SUPPORT	:
			fcsStr = "Telematic interworking not supported"; break;	
		case	TPDU_SHORT_MSG_TYPE_0_NOT_SUPPORT	:
			fcsStr = "Short Message Type 0 not supported"; break;	
		case	TPDU_CANT_REPLACE_SHORT_MSG	:
			fcsStr = "Can't replace short message"; break;	
		case	TPDU_UNSPECIFIED_TPPID_ERROR	:
			fcsStr = "unspecified TP-PID error"; break;	
		case	TPDU_DATA_CODING_SCHEME_NOT_SUPPORT	:
			fcsStr = "Data coding scheme(alphabet) not supported"; break;	
		case	TPDU_MSG_CLASS_NOT_SUPPORT	:
			fcsStr = "message class not supported"; break;	
		case	TPDU_UNSPECIFIED_TPDCS_ERROR	:
			fcsStr = "Unspecified TP-DCS Error"; break;	
		case	TPDU_COMMAND_CANNOT_BE_ACTION	:
			fcsStr = "Command cannot be actioned"; break;	
		case	TPDU_COMMAND_UNSUPPORT	:
			fcsStr = "Command unsupported"; break;	
		case	TPDU_UNSPECIFIED_TPCOMMAND_ERROR	:
			fcsStr = "unspecified TP-Command Error"; break;
		case	TPDU_NOT_SUPPORTED	:
			fcsStr = "TPDU not supported"; break;
		case	TPDU_SC_BUSY 						:
			fcsStr = "SC Busy";	break;
		case	TPDU_NO_SC_SUBSCRIPTION					:
			fcsStr = "No SC Subsription";	break;
		case	TPDU_SC_SYSTEM_FAILURE					:
			fcsStr = "SC System failure";	break;
		case	TPDU_INVALID_SME_ADDRESS					:
			fcsStr = "invalid SME Address";	break;
		case	TPDU_DEST_SME_BARRED					:
			fcsStr = "destination SME barred";	break;
		case	TPDU_SM_REJECT_DUPLICATE_SM					:
			fcsStr = "SM Recjected-Duplicate SM";	break;
		case	TPDU_TP_VPF_NOT_SUPPORT					:
			fcsStr = "TP-VPF not supported";	break;
		case	TPDU_TP_VP_NOT_SUPPORT					:
			fcsStr = "TP-VP not supported";	break;
		case	TPDU_SIM_SMS_STORAGE_FULL 				:
			fcsStr = "(U)SIM SMS Storage full";	break;
		case	TPDU_NO_SMS_STORAGE_CAPABILITY					:
			fcsStr = "no SMS storage capablility in (U)SIM";	break;
		case	TPDU_ERROR_IN_MS					:
			fcsStr = "Error in MS";	break;
		case	TPDU_MEMORY_CAPACITY_EXCEEDED					:
			fcsStr = "Memroy Capacity Exceeded";	break;
		case	TPDU_SIM_APP_TOOLKIT_BUSY					:
			fcsStr = "(U)SIM Application Toolkit Busy";	break;
		case	TPDU_SIM_DATA_DOWNLOAD_ERROR					:
			fcsStr = "(U)SIM data download error";	break;
		case	TPDU_UNSPECIFIED_ERROR_CAUSE 				:
			fcsStr = "Unspecified error cause";	break;
		default	:	fcsStr = "reserved"; break;
	}
	return fcsStr;
}

int	setParamFCSValue(TP_FCSParam *param,int value){
	if(param == NULL) return TPDU_FALSE;
	*param = value;
	return TPDU_TRUE;
}

void	printParamFCSValue(TP_FCSParam data){
	log_write ( LEV_ALL_PARAM,"   [TP-FCS]  [%s]\t\t\t%s\n",
			printBitString(data,0,8),getParamFCSReason(data));
}
/*----------------------------------------------------------------------------*
  -  TP - MR Parameter Function
 *----------------------------------------------------------------------------*/
int	setParamMRValue(TP_MRParam *param,int value){
	*param = value;
	return TPDU_TRUE;
}

void	printParamMRValue(TP_MRParam data){
	log_write ( LEV_ALL_PARAM,"   [TP-MR]   [%s]\t\t\t%d\n",
			printBitString(data,0,8),data);
}

/*----------------------------------------------------------------------------*
  -  TP - UDL Parameter Function
 *----------------------------------------------------------------------------*/

void	printParamUDLValue(TP_UDLParam param){
	log_write ( LEV_ALL_PARAM,"   [TP-UDL]  [%s]\t\t\tH'%02X(%d)\n",
			printBitString(param,0,8),param,param);
}

/*----------------------------------------------------------------------------*
  -  TP - UD Parameter Function
 *----------------------------------------------------------------------------*/
/*  =========================================================
	====== Not Used : <-- samsung 2006-01-20 change --> =====
	=========================================================
	void	printParamUDValue(TP_UDParam *param,int is8bit){
	int	i=0,size=0,ret=0;
	unsigned char	buff[512];
	memset((unsigned char *)&buff,0x00,512);

	if(param == NULL) return;
	log_write ( LEV_ALL_PARAM,"   [TP-UD]\t\t\t\t%d Characters ",param->size);
	if(param->size<=0) return;
	if(is8bit){
	log_write ( LEV_ALL_PARAM,"%d Bytes\n\t\t\t\t\tString [%s]\n\t\tH' ",param->size,param->data);
	for(i=0;i<param->size;i++){
	log_write ( LEV_ALL_PARAM,"%02X ",param->data[i]);
	if(i%11 == 10) log_write ( LEV_ALL_PARAM,"\n\t\tH' ");
	}
	log_write ( LEV_ALL_PARAM,"\n");
	}
	else{
	size = param->size;
	size -= (size/8);
	log_write ( LEV_ALL_PARAM,"%d Bytes\n",size);
	ret = enc8to7bits((unsigned char*)&param->data,(unsigned char *)&buff,param->size);

	log_write ( LEV_ALL_PARAM,"\t\t\t\t\t*** 8 Bit Converting String ***\n\t\t\t\t\t[%s]\n\t\t\t\t\tH' ",param->data);
	for(i=0;i<param->size;i++){
	log_write ( LEV_ALL_PARAM,"%02X ",param->data[i]);
	if(i%10 == 9) log_write ( LEV_ALL_PARAM,"\n\t\t\t\t\tH' ");
	}
	log_write ( LEV_ALL_PARAM,"\n");
	log_write ( LEV_ALL_PARAM,"\t\t\t\t\t"
	"*** 7 Bit Convert Hexa ***\n\t\t\t\t\tH' ");
	for(i=0;i<ret;i++){
	log_write ( LEV_ALL_PARAM,"%02X ",buff[i]);
	if(i%10 == 9) log_write ( LEV_ALL_PARAM,"\n\t\t\t\t\tH' ");
	}
	log_write ( LEV_ALL_PARAM,"\n\n");
	}
	return;
	}
	<-- samsung 2006-01-20 change --> */

/**********************************************/
/* Change Function <-- samsung 2006-01-20 --> */
/**********************************************/
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
						LogRet("[%s:%d] [%02x] is undefined! Let's skip over this.", 
								__func__, __LINE__, spGsm->data[nIndex1]);
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
void    printParamUDValue(TP_UDParam *param,int is8bit,int udh)
{
	int i=0,size=0,ret=0,udhlen=0;
	unsigned char   buff[512];
	char   cTemp[512] = {0x0, };
	size_t len = 0;	
	size_t dLen = param->data[0] +1;
	stGSM7bitString stGsm;
	memset((unsigned char *)&buff,0x00,512);

	if(param == NULL) return;

	log_write ( LEV_ALL_PARAM,"   [TP-UD]\t\t\t\t%d Characters ",param->size);
	if(param->size<=0) return;
	printf("is 8bit [%d]\n", is8bit);
	for(i=0;i<param->size;i++){
		if(i%10 == 0) log_write ( LEV_ALL_PARAM,"\n\t\tH' ");
		log_write ( LEV_ALL_PARAM,"%02X ",param->data[i]);
	}
	log_write ( LEV_ALL_PARAM,"\n");
	if(is8bit){
		if ( is8bit == 2) {
			if ( !udh ) {
				if ( flk_convert_contents_old ( FLK_ICONV_UCS2, 
							(char *)param->data, (size_t)param->size,
							FLK_ICONV_CP949, cTemp, (size_t *)&len) == FLK_FAIL ){
					log_write(LEV_ALL_PARAM, "ucs2 - > cp949 convert failed\n");
				}
				else
					log_write ( LEV_ALL_PARAM,"UCS2 [%d] Bytes\n\t\t\t\t\tString [%s](len %d)\n\t\tH' ",
						len,cTemp, strlen(cTemp));
			}
			else {
				if ( flk_convert_contents_old ( FLK_ICONV_UCS2, 
							(char *)param->data + dLen, (size_t)dLen,
							FLK_ICONV_CP949, cTemp, (size_t *)&len) == FLK_FAIL ){
					log_write(LEV_ALL_PARAM, "ucs2 - > cp949 convert failed\n");
				}
				else {
					log_write ( LEV_ALL_PARAM,"UCS2 [%d] Bytes\n\t\t\t\t\tString [%s](len %d)\n\t\tH' ",
							len, cTemp, strlen(cTemp));
				}
			}
		}
		else {

			if(!udh) {
				log_write ( LEV_ALL_PARAM,"[%d] Bytes\n\t\t\t\t\tString [%s] (len %d)\n\t\tH' ",
						param->size,(char*)param->data, strlen((char*)param->data));
			}
			else{
				/*
				for ( int p = 0; p < param->size - dLen; ++p) {
					if ( p % 10 == 0 ) log_write(LEV_ALL_PARAM, "\n");
					log_write(LEV_ALL_PARAM, "%02X ", param->data[p + dLen]);
				}
				log_write(LEV_ALL_PARAM, "\n");
				*/
				log_write ( LEV_ALL_PARAM,"[%d] Bytes\n\t\t\t\t\tString [%s] (len %d)\n\t\tH' ",
						dLen, (param->data + dLen), strlen((char*)param->data + dLen));
			}
		}
	}
	else{

		bzero(&stGsm, sizeof(stGsm));

		if(udh){
			log_write(LEV_ALL_PARAM, "dLen [%d]\n", dLen);	
			memcpy(stGsm.data, param->data + dLen, param->size - dLen);
			stGsm.nCharacter = param->size - dLen;
			stGsm.nOctet = param->size - dLen;
			//changeGSMtoASCII8bit(&stGsm);

			log_write ( LEV_ALL_PARAM,"\n\t\t\t\t\t*** 8 Bit Converting String ***\n\n\t\t\t\t\tH' ");
			//log_write ( LEV_ALL_PARAM,"[%d] Bytes\n\t\t\t\t\tString [%s](len %d)\n\t\tH' ", param->size, param->data, strlen(param->data));
			log_write ( LEV_ALL_PARAM,"[%d] Bytes\n\t\t\t\t\tString [%s](len %d)\n\t\tH' ",
					stGsm.nOctet, stGsm.data, stGsm.nCharacter);

			for(i=0;i<param->size;i++){
				log_write ( LEV_ALL_PARAM,"%02X ",stGsm.data[i]);
				if(i%10 == 9) log_write ( LEV_ALL_PARAM,"\n\t\t\t\t\tH' ");
			}
			log_write ( LEV_ALL_PARAM,"\n\n");
			log_write ( LEV_ALL_PARAM,"\t\t\t\t\t"
					"*** 7 Bit Convert Hexa ***\n\n");
			udhlen = param->data[0];
			log_write ( LEV_ALL_PARAM,"\t\t\t\t\t[%d] Bytes  User Data Header\n\t\t\t\t\tH' ",
					udhlen+1);
			for(i=0;i<udhlen+1;i++){
				log_write ( LEV_ALL_PARAM,"%02X ",param->data[i]);
				if(i%11 == 10) log_write ( LEV_ALL_PARAM,"\n\t\tH' ");
			}
			log_write ( LEV_ALL_PARAM,"\n");

			size = param->size-udhlen-1;
			ret = enc8to7bits((unsigned char*)&param->data+(udhlen+1),(unsigned char *)&buff,
					param->size-(udhlen+1));
			log_write ( LEV_ALL_PARAM,"\t\t\t\t\tUser Data data\n\t\t\t\t\tH' ");
			for(i=0;i<ret-1;i++){
				log_write ( LEV_ALL_PARAM,"%02X ",buff[i]);
				if(i%10 == 9) log_write ( LEV_ALL_PARAM,"\n\t\t\t\t\tH' ");
			}
			log_write ( LEV_ALL_PARAM,"\n\n");
		}
		else{
			memcpy(stGsm.data, param->data, param->size );
			stGsm.nCharacter = param->size;
			stGsm.nOctet = param->size;
			//changeGSMtoASCII8bit(&stGsm);

			size = param->size;
			size -= (size/8);
			// if(size%7==0) size-=1; 
			log_write ( LEV_ALL_PARAM,"[%d] Bytes\n",size);
			ret = enc8to7bits((unsigned char*)&param->data,(unsigned char *)&buff,param->size);

			log_write ( LEV_ALL_PARAM,"\t\t\t\t\t*** 8 Bit Converting String ***\n\t\t\t\t\t[%s](len %d)",
					stGsm.data, stGsm.nCharacter);
			for(i=0;i<param->size;i++){
				if(i%10 == 0) log_write ( LEV_ALL_PARAM,"\n\t\t\t\t\tH' ");
				log_write ( LEV_ALL_PARAM,"%02X ",stGsm.data[i]);
			}
			log_write ( LEV_ALL_PARAM,"\n");
		
			log_write ( LEV_ALL_PARAM,"\t\t\t\t\tUser Data data\n\t\t\t\t\tH' ");
			for(i=0;i<ret;i++){
				log_write ( LEV_ALL_PARAM,"%02X ",buff[i]);
				if(i%10 == 9) log_write ( LEV_ALL_PARAM,"\n\t\t\t\t\tH' ");
			}
			log_write ( LEV_ALL_PARAM,"\n\n");
		}
	}
	return;
}

/*----------------------------------------------------------------------------*
  -  7 bits and 8 bits converting  Function
 *----------------------------------------------------------------------------*/
int enc8to7bits(unsigned char *data,unsigned char *buff,int datasize){
	octet   tmp[512],buf[512];
	int     i=0,remain=0,pt=0;
	int     idx =0;

	memset((octet*)&tmp,0x00,512);
	memset((octet*)&buf,0x00,512);

	memcpy((octet*)&tmp,data,datasize);

	remain = datasize;

	while(remain){
		switch(pt%7){
			/*   |**|a1|a2|a3| a4|a5|a6|a7|--> |b7|a1|a2|a3| a4|a5|a6|a7|*/
			case    0   :
				buf[idx].bit1.e = tmp[i].bit1.e;
				buf[idx].bit1.s = tmp[i+1].bit7.e;
				break;
				/*   |**|**|b1|b2| b3|b4|b5|b6|--> |c6|c7|b1|b2| b3|b4|b5|b6|*/
			case    1   :
				buf[idx].bit1.e = tmp[i].bit7.s;
				buf[idx].bit2.s = tmp[i+1].bit6.e;
				break;
				/*   |**|**|**|c1| c2|c3|c4|c5|--> |d5|d6|d7|c1| c2|c3|c4|c5|*/
			case    2   :
				buf[idx].bit2.e = tmp[i].bit6.s;
				buf[idx].bit3.s = tmp[i+1].bit5.e;
				break;
				/*   |**|**|**|**| d1|d2|d3|d4|--> |e4|35|e6|e7| d1|d2|d3|d4|*/
			case    3   :
				buf[idx].bit3.e = tmp[i].bit5.s;
				buf[idx].bit4.s = tmp[i+1].bit4.e;
				break;
				/*   |**|**|**|**| **|e1|e2|e3|--> |f3|f4|f5|f6| f7|e1|e2|e3|*/
			case    4   :
				buf[idx].bit4.e = tmp[i].bit4.s;
				buf[idx].bit5.s = tmp[i+1].bit3.e;
				break;
				/*   |**|**|**|**| **|**|f1|f2|--> |g2|g3|g4|g5| g6|g7|f1|f2|*/
			case    5   :
				buf[idx].bit5.e = tmp[i].bit3.s;
				buf[idx].bit6.s = tmp[i+1].bit2.e;
				break;
				/*   |**|**|**|**| **|**|**|g1|--> |h1|h2|h3|h4| h5|h6|h7|g1|*/
			case    6   :
				buf[idx].bit6.e = tmp[i].bit2.s;
				buf[idx].bit7.s = tmp[i+1].bit1.e;
				i++;
				remain--;
				break;
			default : break;
		}
		if(remain<0) break;
		i++;
		pt++;
		remain--;
		idx++;
	}
	memcpy(buff,(octet*)&buf,idx);
	return idx;
}
/* 2006-01-03 add start */
int enc8to7bitsUDH(unsigned char *data,unsigned char *buff,int datasize)
{
	octet 	tmp[512],buf[512];
	int		i=0,remain=0,idx=0,tot=0;
	int		udhl_7len=0;
	memset((octet*)&tmp,0x00,512);
	memset((octet*)&buf,0x00,512);

	remain = datasize;
	/* <-- 2006-01-04 not used
	   idx = data[0];
	   buff[0]=data[0];
	   remain--;
	   tot++;
	   i = enc8to7bits(data+1,(unsigned char*)&tmp,idx);
	   tot += i;
	   memcpy(buff+1,(unsigned char*)&tmp,i);
	   idx = enc8to7bits(data+1+buff[0],(unsigned char*)&buf,remain-idx);
	   tot += idx;
	   memcpy(buff+1+i,(unsigned char*)&buf,idx);
	   --> */
	idx = data[0];
	udhl_7len = enc8to7bits(data,(unsigned char*)&buf,idx+1);
	/* value 2 is UDHL and fill bits (2bytes) */
	memcpy(buff,data,idx+1);
	remain -= udhl_7len;
	tot = enc8to7bits(data+1+idx,(unsigned char*)&tmp,remain);
	if(tot > 0)
	{ 
		memcpy(buff+1+idx,(unsigned char*)&tmp,tot);
		tot += idx;	
		return tot; 
		// return datasize;
	}		
	return 0;
}

int dec7to8bitsUDH(unsigned char *data, unsigned char *buff, int datasize)
{
	octet 	tmp[512],buf[512];
	int		i=0,remain=0,idx=0,tot=0;
	int		udhl_len=0;
	memset((octet*)&tmp,0x00,512);
	memset((octet*)&buf,0x00,512);

	remain = datasize;
	idx = data[0];

	/* <-- 2006-01-04 not used
	   remain -= idx;
	   i = dec7to8bits(data+1,(unsigned char*)&tmp,idx);
	   buff[0]=data[0];
	   memcpy(buff+1,(unsigned char*)&tmp,i);
	   tot = dec7to8bits(data+1+idx,(unsigned char*)&buf,remain);
	   memcpy(buff+1+i,(unsigned char*)&buf,tot);
	   tot += (i+1);
	   --> */

	memcpy(buff,data,idx+1);
	udhl_len = enc8to7bits(data, (unsigned char*)&buf, idx+1);
	remain -= udhl_len;

	tot = dec7to8bits(data+1+idx, (unsigned char*)&tmp, remain+1);
	if(tot > 0)
	{
		memcpy(buff+idx+1, (unsigned char*)&tmp, tot);
		tot += (idx+1);
		return tot;
	}
	return 0;
	//return datasize;
}
/* 2006-01-03 add end */

int dec7to8bits(unsigned char *data,unsigned char *buff,int datasize)
{
	octet   tmp[512],buf[512];
	int     i=0,remain=0;
	int     idx =0;

	memset((octet*)&tmp,0x00,512);
	memset((octet*)&buf,0x00,512);

	memcpy((octet*)&tmp,data,datasize);

	remain = datasize;

	while(remain){
		switch(i%7){
			case    0   :
				/*[WON] |b7|a1|a2|a3| a4|a5|a6|a7|--> |00|a1|a2|a3| a4|a5|a6|a7|*/
				buf[idx].bit1.e = tmp[i].bit1.e;
				buf[idx].bit1.s = 0;
				break;
			case    1   :
				/*[WON] |c6|c7|b1|b2| b3|b4|b5|b6|--> |00|b1|b2|b3| b4|b5|b6|b7|*/
				buf[idx].bit7.e = tmp[i-1].bit1.s;
				buf[idx].bit7.s = tmp[i].bit2.e;
				buf[idx].bit1.s =0;
				break;
			case    2   :
				buf[idx].bit6.e = tmp[i-1].bit2.s;
				buf[idx].bit6.s = tmp[i].bit3.e;
				buf[idx].bit1.s =0;
				break;
			case    3   :
				buf[idx].bit5.e = tmp[i-1].bit3.s;
				buf[idx].bit5.s = tmp[i].bit4.e;
				buf[idx].bit1.s =0;
				break;
			case    4   :
				buf[idx].bit4.e = tmp[i-1].bit4.s;
				buf[idx].bit4.s = tmp[i].bit5.e;
				buf[idx].bit1.s =0;
				break;
			case    5   :
				buf[idx].bit3.e = tmp[i-1].bit5.s;
				buf[idx].bit3.s = tmp[i].bit6.e;
				buf[idx].bit1.s =0;
				break;
			case    6   :
				buf[idx].bit2.e = tmp[i-1].bit6.s;
				buf[idx].bit2.s = tmp[i].bit7.e;
				buf[idx++].bit1.s =0;
				buf[idx].bit1.e = tmp[i].bit7.s;
				buf[idx].bit1.s =0;
				break;
			default : break;
		}
		if(remain<=0) break;
		i++;
		remain--;
		idx++;
	}
	if(idx < 0){
		log_write(LEV_ALL_PARAM, "dec7to8bits data length invalid [data length %d\n",datasize);
		setGSMTPDUErrorCode(CONVERT_7TO8_ERROR);
		return TPDU_FALSE;
	}
	memcpy(buff,(octet*)&buf,idx);
	/* [WON] Reducing the unnecessary octet --> |00|00|00|00| 00|00|00|00|*/
	if((datasize%7 == 0) && (buff[idx]&0x7f != 0x00)) idx = idx-1;
	return idx;
}


#if 0
int dec8bitUD(TPSMSSubmit *submit, unsigned char *pt, int *remain)
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
		log_write(LEV_ALL_PARAM, "[%s:%d] submit point is NULL! return error! ",
				__func__, __LINE__);
		return TPDU_FALSE;
	}
	if(pt == NULL)
	{
		log_write(LEV_ALL_PARAM, "[%s:%d] data point is NULL! return error! ",
				__func__, __LINE__);
		return TPDU_FALSE;
	}
	if(remain == NULL)
	{
		log_write(LEV_ALL_PARAM, "[%s:%d] remain point is NULL! return error! ",
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
			log_write(LEV_ALL_PARAM, "UDHI set, but UDHL is zero!");
			return TPDU_FALSE;
		}
		/* Check UDHL and UDL */
		if(isUDHL > submit->tpUDL)
		{
			log_write(LEV_ALL_PARAM, "[%s:%d] UDHL[%d] is UDL[%d] size over!",
					__func__, __LINE__, 
					isUDHL, submit->tpUDL);
			return TPDU_FALSE;
		}
		if(FLK_FAIL == decUD_RemakeUD(cpTempUD, submit, &stUDH))
		{
			log_write(LEV_ALL_PARAM, "[%s:%d] UDH decoding error! ", 
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
		log_write(LEV_ALL_PARAM, "[%s:%d] cpUD Buffer is NULL! return error!", 
				__func__, __LINE__);
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
#ifdef DEBUG
	DebugStr(4, "[%s:%d] Test  stSource"
			"[%02x][%02x][%02x][%02x][%02x][%02x]"
			"[%02x][%02x][%02x][%02x][%02x][%02x] C[%d] O[%d]",
			__FILE__, __LINE__,
			stSource.data[0], stSource.data[1], stSource.data[2], 
			stSource.data[3], stSource.data[4], stSource.data[5], 
			stSource.data[6], stSource.data[7], stSource.data[8], 
			stSource.data[9], stSource.data[10], stSource.data[11],
			stSource.nCharacter, stSource.nOctet);
#endif // DEBUG
	/* convert */ 
	ConvGSM7bit28bit(&stSource, &stTarget);
#ifdef DEBUG
	DebugStr(4, "[%s:%d] Test stTarget"
			"[%02x][%02x][%02x][%02x][%02x][%02x]"
			"[%02x][%02x][%02x][%02x][%02x][%02x] C[%d] O[%d]",
			__FILE__, __LINE__,
			stTarget.data[0], stTarget.data[1], stTarget.data[2], 
			stTarget.data[3], stTarget.data[4], stTarget.data[5], 
			stTarget.data[6], stTarget.data[7], stTarget.data[8], 
			stTarget.data[9], stTarget.data[10], stTarget.data[11],
			stTarget.nCharacter, stTarget.nOctet);
#endif // DEBUG

	memcpy(&ucUDH[nUDHL+1], stTarget.data, stTarget.nCharacter);
	*nCharNum = ((nUDHL+1) + stTarget.nCharacter); 
	bzero(cpUD, BUF_SSIZE);
	memcpy(cpUD, ucUDH, *nCharNum);
	return FLK_SUCCESS;
}

int decGsm7bitUD(TPSMSSubmit *submit, unsigned char *pt, int *remain)
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
		log_write(LEV_ALL_PARAM, "[%s:%d] submit point is NULL! return error! ",
				__func__, __LINE__);
		return TPDU_FALSE;
	}
	if(pt == NULL)
	{
		log_write(LEV_ALL_PARAM, "[%s:%d] data point is NULL! return error! ",
				__func__, __LINE__);
		return TPDU_FALSE;
	}
	if(remain == NULL)
	{
		log_write(LEV_ALL_PARAM, "[%s:%d] remain point is NULL! return error! ",
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
			log_write(LEV_ALL_PARAM, "UDHI set, but UDHL is zero!");
			return TPDU_FALSE;
		}
		/* UDL UDHL  */
		//if(isUDHL > submit->tpUDL)
		if(isUDHL > (BIT8_UD_MAX_SIZE - 1))
		{
			log_write(LEV_ALL_PARAM, "[%s:%d] UDHL[%d] is UDL[%d] size over!",
					__func__, __LINE__, 
					isUDHL, submit->tpUDL);
			return TPDU_FALSE;
		}

		/* 8bit data remake */
		isUDLChar = (int)submit->tpUDL;
		if(dec7bitUDreMake(cpTempUD, &isUDLChar) == FLK_FAIL)		
		{
			log_write(LEV_ALL_PARAM, "[%s:%d] dec7bitUDreMake() Fail! return error!", 
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
#endif


/*************************************************************************************
 *
 *  SMS Deliver General Function
 *
 ************************************************************************************/
int		initSMSDeliver(TPSMSDeliver *pdu)
{
	if(pdu == NULL) return TPDU_FALSE;
	memset((TPSMSDeliver*)pdu,0x00,sizeof(TPSMSDeliver));
	pdu->tpPduSize = 0;
	return TPDU_TRUE;		
}

void	printSMSDeliver(TPSMSDeliver pdu)
{
	unsigned char	charval;
	charval = pdu.idc.charval;

	printParamTPMTIValue(charval,GetMTIValue(pdu));
	printParamTPMMSValue(charval,GetMMSValue(pdu));
	printParamReserved(3,2);
	if(pdu.optional.SRI)
		printParamTPSRIValue(charval,GetSRIValue(pdu));
	else
		printParamReserved(5,1);
	if(pdu.optional.UDHI)
		printParamTPUDHIValue(charval,GetUDHIValue(pdu));
	else
		printParamReserved(6,1);
	printParamTPRPValue(charval,GetRPValue(pdu));

	printParamTPAddress(&pdu.tpADDR);
	/* 2006-01-21 */
	log_write ( LEV_ALL_PARAM, "\n   [ADDRESS PARAMETER - CallBack]\n");
	//	printParamTPAddress(&pdu.tpCallbackADDR);
	/* end 2006-01-21 */
	printParamPIDValue(&pdu.tpPID);
	printParamDCSValue(&pdu.tpDCS);
	printParamSCTSValue(&pdu.tpSCTS,0);
	printParamUDLValue(pdu.tpUDL);
	printParamUDValue(&pdu.tpUD,pdu.tpDCS.dcsCharset,GetUDHIValue(pdu));
	return;
}		

/*************************************************************************************
 *
 *  SMS Deliver Report General Function
 *
 ************************************************************************************/
int	initSMSDeliverReport(TPSMSDeliverReport *pdu)
{
	if(pdu == NULL) return TPDU_FALSE;
	memset((TPSMSDeliverReport*)pdu,0x00,sizeof(TPSMSDeliverReport));
	pdu->tpPduSize = 0;  
	return TPDU_TRUE;		
}

void	printSMSDeliverReport(TPSMSDeliverReport pdu)
{
	unsigned char charval;
	charval = pdu.idc.charval;
	printParamTPMTIValue(charval,GetMTIValue(pdu));
	printParamReserved(2,4);
	if(pdu.optional.UDHI)
		printParamTPUDHIValue(charval,GetUDHIValue(pdu));
	else
		printParamReserved(6,1);
	printParamReserved(7,1);
	if(pdu.optional.FCS){
		printParamFCSValue(pdu.tpFCS);
	}
	if(pdu.optional.PI)
		printParamPIValue(&pdu.tpPI);
	if(IsExistPID(pdu.tpPI))
		printParamPIDValue(&pdu.tpPID);
	if(IsExistDCS(pdu.tpPI))
		printParamDCSValue(&pdu.tpDCS);
	if(!IsExistUDL(pdu.tpPI)) return;
	printParamUDLValue(pdu.tpUDL);
	printParamUDValue(&pdu.tpUD,pdu.tpDCS.dcsCharset,GetUDHIValue(pdu));
}		

/*************************************************************************************
 *
 *  SMS Submit General Function
 *
 ************************************************************************************/
int		initSMSSubmit(TPSMSSubmit *pdu)
{
	if(pdu == NULL) return TPDU_FALSE;
	memset((TPSMSSubmit*)pdu,0x00,sizeof(TPSMSSubmit));
	pdu->tpPduSize = 0;
	return TPDU_TRUE;		
}

void	printSMSSubmit(TPSMSSubmit pdu)
{
	unsigned char	charval;

	charval = pdu.idc.charval;

	printParamTPMTIValue(charval,GetMTIValue(pdu));
	printParamTPRDValue(charval,GetRDValue(pdu));
	printParamTPVPFValue(charval,GetVPFValue(pdu));
	printParamTPSRRValue(charval,GetSRRValue(pdu));
	printParamTPUDHIValue(charval,GetUDHIValue(pdu));
	printParamTPRPValue(charval,GetRPValue(pdu));

	printParamMRValue(pdu.tpMR);
	printParamTPAddress(&pdu.tpADDR);
	printParamPIDValue(&pdu.tpPID);
	printParamDCSValue(&pdu.tpDCS);
	printParamVPValue(&pdu.tpVP);
	printParamUDLValue(pdu.tpUDL);
	printParamUDValue(&pdu.tpUD,pdu.tpDCS.dcsCharset,GetUDHIValue(pdu));
	return;
}		

/*************************************************************************************
 *
 *  SMS Submit Report General Function
 *
 ************************************************************************************/
int	initSMSSubmitReport(TPSMSSubmitReport *pdu)
{
	if(pdu == NULL) return TPDU_FALSE;
	memset((TPSMSSubmitReport*)pdu,0x00,sizeof(TPSMSSubmitReport));
	pdu->tpPduSize = 0;  
	return TPDU_TRUE;		
}


/*************************************************************************************
 *
 *  SMS Status Report General Function
 *
 ************************************************************************************/
int	initSMSStatusReport(TPSMSStatusReport *pdu)
{
	if(pdu == NULL) return TPDU_FALSE;
	memset((TPSMSStatusReport*)pdu,0x00,sizeof(TPSMSStatusReport));
	pdu->tpPduSize = 0;  
	return TPDU_TRUE;		
}

void	printSMSStatusReport(TPSMSStatusReport pdu)
{
	unsigned char	charval;
	charval = pdu.idc.charval;

	printParamTPMTIValue(charval,GetMTIValue(pdu));
	printParamTPMMSValue(charval,GetMMSValue(pdu));
	printParamReserved(3,2);
	printParamTPSRQValue(charval,GetSRQValue(pdu));
	printParamTPUDHIValue(charval,GetUDHIValue(pdu));
	printParamReserved(7,1);

	printParamMRValue(pdu.tpMR);
	printParamTPAddress(&pdu.tpADDR);
	printParamSCTSValue(&pdu.tpSCTS,0);
	printParamSCTSValue(&pdu.tpDT,2);
	printParamSTValue(pdu.tpST);
	if(!pdu.optional.PI) return;
	printParamPIValue(&pdu.tpPI);
	if(pdu.optional.PID)
		printParamPIDValue(&pdu.tpPID);
	if(pdu.optional.DCS)
		printParamDCSValue(&pdu.tpDCS);
	if(pdu.optional.UDL){
		printParamUDLValue(pdu.tpUDL);
		printParamUDValue(&pdu.tpUD,pdu.tpDCS.dcsCharset,GetUDHIValue(pdu));
	}
	return;
}		

void	printSMSCommand(TPSMSCommand pdu)
{
	unsigned char charval;

	charval = pdu.idc.charval;

	printParamTPMTIValue(charval,GetMTIValue(pdu));
	printParamReserved(2,3);
	printParamTPSRRValue(charval,GetSRRValue(pdu));
	printParamTPUDHIValue(charval,GetUDHIValue(pdu));
	printParamReserved(7,1);

	printParamMRValue(pdu.tpMR);
	printParamPIDValue(&pdu.tpPID);
	printParamCTValue(pdu.tpCT);
	printParamMNValue(pdu.tpMN);
	printParamTPAddress(&pdu.tpADDR);
	printParamUDLValue(pdu.tpUDL);
	printParamUDValue(&pdu.tpUD,0,GetUDHIValue(pdu));

	return;
}		
void	printSMSSubmitReport(TPSMSSubmitReport pdu){
	unsigned char		charval;

	charval = pdu.idc.charval;

	printParamTPMTIValue(charval,GetMTIValue(pdu));
	printParamReserved(2,4);
	if(pdu.optional.UDHI)
		printParamTPUDHIValue(charval,GetUDHIValue(pdu));
	else
		printParamReserved(6,1);
	printParamReserved(7,1);

	if(pdu.optional.FCS)
		printParamFCSValue(pdu.tpFCS);
	if(!pdu.optional.PI) return;
	printParamPIValue(&pdu.tpPI);
	printParamSCTSValue(&pdu.tpSCTS,0);
	if(pdu.optional.PID) 
		printParamPIDValue(&pdu.tpPID);
	if(pdu.optional.PID) 
		printParamDCSValue(&pdu.tpDCS);
	if(pdu.optional.UDL) 
		printParamUDLValue(pdu.tpUDL);
	if(pdu.optional.UDL) 
		printParamUDValue(&pdu.tpUD,pdu.tpDCS.dcsCharset,GetUDHIValue(pdu));
	return;
}

