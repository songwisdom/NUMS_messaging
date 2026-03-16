/*******************************************************************************
*                                                                           
*     Copyright (c) 2003 FEELINGK Co., Ltd.                                
*     All rights reserved.                                                
*                                                                          
*     This program contains confidential and proprietary information of    
*     FEELINGK, and any reproduction, disclosure, or use in whole or in part 
*     by prior written agreement or permission of FEELINGK.                 
*                                                                          
*                            FEELINGK Co., Ltd.                           
*     3F Ace Techno Twin Tower 1, 212-1 Kuro-Dong, Kuro-Gu, Seoul 152-050 Korea 
*                                                                          
*******************************************************************************/

#include "libMapComm.h"
#include "libMap2G.h"
#include "libMapAddrUtil.h"

/*---------------------------------------------------------------------
 *  FUNCTION NAME   : 
 *  PURPOSE         : get requested 1 byte from integer 4byte.
 *  INPUT ARGUMENTS : 
 *  RETURN VALUE    :
 *  CALLED FROM     :
 *  USED GLOBAL VARIABLES :
 *  REVISION        :
 *      BY              WHEN        REASON
 *      --------------  ----------  -------------------
 *                       2003.9     First Coding
 *----------------------------------------------------------------------*/

/*---------------------------------------------------------------------
 *  FUNCTION NAME   : 
 *  PURPOSE         :  convert dechar type =>  1 byte char.
 *  INPUT ARGUMENTS : 
 *  RETURN VALUE    :
 *  CALLED FROM     :
 *  USED GLOBAL VARIABLES :
 *  REVISION        :
 *      BY              WHEN        REASON
 *      --------------  ----------  -------------------
 *                       2003.9     First Coding
 *----------------------------------------------------------------------*/

int encodeMsgId( u_char *BearerData, u_char SubId, u_short MsgId  )
{
	u_char	ucTemp[4];
	u_char	ucMesgId;
	u_char *pMsgID = (u_char *)&MsgId;
	int nBitStep = 0;

	PutBitByte(BearerData, &nBitStep, 8, MESSAGE_IDENTIFIER);
	PutBitByte(BearerData, &nBitStep, 8, MESSAGE_IDENTIFIER_LEN);
	PutBitByte(BearerData, &nBitStep, 4, SubId);
    PutBitByte(BearerData, &nBitStep, 8, pMsgID[0]);
    PutBitByte(BearerData, &nBitStep, 8, pMsgID[1]);
	PutBitByte(BearerData, &nBitStep, 4, 0);

	return GET_LENGTH_BIT_TO_OCT(nBitStep);
}
 
int encodeUserData(u_char *BearerData, u_char *Message, int MessageLen, u_char MsgEncoding)
{
	int				itmp;
	int				nDataLeng=0;
	u_char   			ucTmp;
	int 			subleng;
	u_char 			cpDCS = 0x00;
	int 			nBitStep = 0;
	register u_int 	i = 0;
	printf("MsgEncoding[%d]\n", MsgEncoding);
	
	nDataLeng = MessageLen;
	if (nDataLeng <= 0)
		return 0;

	PutBitByte(BearerData, &nBitStep, 8, USER_DATA);

	subleng = nBitStep;
	PutBitByte(BearerData, &nBitStep, 8, 0x00);

	PutBitByte(BearerData, &nBitStep, 5, MsgEncoding); 
	
	PutBitByte(BearerData, &nBitStep, 8, nDataLeng);

	if ( MsgEncoding == ASCII ) {
       	for (i = 0; i < nDataLeng && i < DB_MESSAGE_CONTENT_SIZE; i++)
       	{
           	PutBitByte(BearerData, &nBitStep, 7, (Message[i] & 0x7f));
       	}
	}
	else {
		for (i = 0; i<nDataLeng && i<DB_MESSAGE_CONTENT_SIZE; i++)
		{
			PutBitByte(BearerData, &nBitStep, 8, Message[i]);
		}
	}

   	if ((nBitStep % 8) == 0)    itmp = 0;
   	else    itmp =  8 - (nBitStep % 8);

   	PutBitByte(BearerData, &nBitStep, itmp, 0);

	PutBitByte(BearerData, &subleng, 8, (nBitStep / 8) - 2);

	return GET_LENGTH_BIT_TO_OCT(nBitStep);
}


int encodePriorityId(u_char *BearerData, u_char PriorityFlag)
{
	int nBitStep = 0;

	PutBitByte(BearerData, &nBitStep, 8, PRIORITY_INDICATOR);
	PutBitByte(BearerData, &nBitStep, 8, PRIORITY_INDICATOR_LEN);
	PutBitByte(BearerData, &nBitStep, 2, PriorityFlag);
	PutBitByte(BearerData, &nBitStep, 6, 0);

	return GET_LENGTH_BIT_TO_OCT(nBitStep);
}


int encodeLanguageId(u_char *BearerData, u_char LanguageFlag)
{
	int nBitStep = 0;
    PutBitByte(BearerData, &nBitStep, 8, LANGUAGE_INDICATOR);

    PutBitByte(BearerData, &nBitStep, 8, LANGUAGE_INDICATOR_LEN);

	PutBitByte(BearerData, &nBitStep, 8, LanguageFlag);

	return GET_LENGTH_BIT_TO_OCT(nBitStep);
}

int encodeCbNumber( u_char *BearerData, u_char *CallBack, int CB_Octets )
{
	int				subleng = 0;
	int				i, itmp;
	int 			add_size = 0;
	int 			nBitStep = 0;
	u_char 			CallBackBcdLength = getBCDLength(CallBack, CB_Octets);
	u_char 			Tmp = 0x00;
	/*
	u_char 			callback[21] = {0x0, };

	convBCD2Cdma(callback, CB_Octets, CallBack, CB_Octets);
	*/
	/* Tag */
    PutBitByte(BearerData, &nBitStep, 8, CALL_BACK_NUMBER);

	subleng = nBitStep;

	/* Length */
    PutBitByte(BearerData, &nBitStep, 8, 0x00);

	/* DIGIT MODE */
    PutBitByte(BearerData, &nBitStep, 1, CB_DTMF_DIGIT);

	/* ?? 1bytes */
	PutBitByte(BearerData, &nBitStep, 8, CallBackBcdLength);

	if ( (CallBackBcdLength % 2) != 0 )
		CB_Octets--;

	for (i = 0; (i < CB_Octets) && (i < ADDRESS_LEN_CALLBACK); ++i )
	{
		PutBitByte(BearerData, &nBitStep, 8, CallBack[i] );
	}

	if ( (CallBackBcdLength % 2) != 0 ) {
		Tmp = ((CallBack[i] & 0xF0) >> 4);
		PutBitByte(BearerData, &nBitStep, 4, Tmp );
	}


    if ((nBitStep % 8) == 0)    itmp = 0;
    else    itmp =  8 - (nBitStep % 8);

    PutBitByte(BearerData, &nBitStep, itmp, 0);

	/* SubParam Length */
    PutBitByte(BearerData, &subleng, 8, (nBitStep/8) - 2);

	return GET_LENGTH_BIT_TO_OCT(nBitStep);
}

#if 0
int encodeCbNumber(spRDActiveTuple)
	stRDActiveTuple *spRDActiveTuple;
{
	int				subleng = 0;
	int				i, itmp;

	add_size = spRDActiveTuple->dbCallbackLen[0];
	if (add_size == 0) 
	{
	    for (i = 0; i < DB_CALLBACK_NUMBER_SIZE; i++)
    	{
        	if (spRDActiveTuple->dbCallbackNumber[i] == 0x00) break;
    	}
    	if (i == 0) return;

    	if ((spRDActiveTuple->dbCallbackNumber[i-1] & 0x0f) == 0x00)
        	add_size = (i * 2) - 1;
    	else
        	add_size = i * 2 ;
	}

    PutBitByte(BearerData, STEP, 8, CALL_BACK_NUMBER);
    STEP = STEP + 8;

	subleng = STEP;
    PutBitByte(BearerData, STEP, 8, subleng);
    STEP = STEP + 8;

    PutBitByte(BearerData, STEP, 1, CB_DTMF_DIGIT);
    STEP = STEP + 1;

	/* DIGIT MODE */
	PutBitByte(BearerData, STEP, 8, add_size);
	STEP = STEP + 8;

	for (i = 0; i<add_size/2 && i<DB_CALLBACK_NUMBER_SIZE; i++)
	{
		PutBitByte(BearerData, STEP, 4, 
					(spRDActiveTuple->dbCallbackNumber[i] & 0xf0) >> 4);
		STEP = STEP + 4;

		PutBitByte(BearerData, STEP, 4, 
					(spRDActiveTuple->dbCallbackNumber[i] & 0x0f) );
		STEP = STEP + 4;
	}

	if ((add_size % 2) != 0) 
	{
		PutBitByte(BearerData, STEP, 4, 
			(spRDActiveTuple->dbCallbackNumber[add_size/2] & 0xf0) >> 4);
		STEP = STEP + 4;
	}

    if ((STEP % 8) == 0)    itmp = 0;
    else    itmp =  8 - (STEP % 8);

    PutBitByte(BearerData, STEP, itmp, 0);
    STEP = STEP + itmp;

	// SubParam Length 
    PutBitByte(BearerData, subleng, 8, (STEP-subleng)/8-1);
}
#endif


