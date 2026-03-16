/**
 @ingroup mdbCommonLib
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
******************************************************************************

*******************************************************************************
*   FILE NAME       :   mdbDmaqiUtil.c
*   PURPOSE         :
*   DATE OF WRITE   :   2002.03.01
*   AUTHOR          :
*   REVISION        :   V 1.0
*   Ver.    By      When        Reason
*   ------- ------- ----------  -----------------------------------------
*   1.0             2002.03.01  first coding
******************************************************************************/

#include	<stdio.h>
#include 	<sys/types.h>
#include    <sys/msg.h>
#include 	"FLKBaseDef.h"
#include 	<MdbCommon.h>
#include 	<MdbDataStruct.h>
#include 	<MdbMessage.h>
#include 	<MdbDmaqi.h>


/*---------------------------------------------------------------------
 *  FUNCTION NAME   : getLengthStr()
 *  PURPOSE         : to pattern string's length.
 *  INPUT ARGUMENTS :
 *  RETURN VALUE    : length.
 *  CALLED FROM     :
 *  USED GLOBAL VARIABLES :
 *  REVISION        :
 *      BY              WHEN        REASON
 *      --------------  ----------  -------------------
 *      molee           98.05.      First Coding
----------------------------------------------------------------------*/
u_int getLengthStr(u_char *str, int size)
{

	int     i;
	for (i = 0; ((*(str + i)) != 0xff) && (i < size ) ; i++)
		;

	return (i);
}

int getNumOfDigit(u_char *str, int size)
{
	int		len = 0;

	for (int i = 0; (i < size); ++i) {
		if ((str[i] & 0xf0) == 0xf0) {
			break;
		}
		++len;

		if ((str[i] & 0x0f) == 0x0f ) {
			break;
		}
		++len;
	}

	return len;
}

void CallBackDigit2Str(u_char *ucDigit, u_char *ucChar, int size)
{
	u_char   temp;

	int     loop;
	int     src;
	for (loop = 0, src = 0; loop < size;) {

		if ((!loop)) {
			temp    =   (ucDigit[src] & 0xf0 ) >> 4;
		} else if (((loop % 2) == 0) && (loop != 0)) {
			temp    =   (ucDigit[src] & 0xf0 ) >> 4;
		} else {
			temp    =   (ucDigit[src] & 0x0f );
			src++;
		}
		if ((int)temp >= 0x00 && temp < 0x0a) {
			temp    =   temp    + 0x30;
		}

		if (temp == 0x0a) { temp = 0x30; }
		if (temp == 0x0b) { temp = 0x2a; }
		if (temp == 0x0c) { temp = 0x23; }

		ucChar[loop]    =   temp;
		++loop;
	}
	ucChar[loop] = '\0';
}
/*---------------------------------------------------------------------
 *  FUNCTION NAME   : digit2char()
 *  PURPOSE         : decchar/hexachar to char string.
 *  INPUT ARGUMENTS :
 *						decchar/hexachar string *.
 *						char string.
 *						decchar/hexachar string's length.
 *  RETURN VALUE    :
 *  CALLED FROM     :
 *  USED GLOBAL VARIABLES :
 *  REVISION        :
 *      BY              WHEN        REASON
 *      --------------  ----------  -------------------
 *      molee           98.05.      First Coding
----------------------------------------------------------------------*/
void MINDigit2Str(u_char *ucDigit, u_char *ucChar, int size)
{
	u_char	temp;
	int		loop;
	int		src;
	for (loop = 0, src = 0; loop < size;) {

		if ((!loop)) {
			temp	=	(ucDigit[src] & 0xf0 ) >> 4;
		} else if (((loop % 2) == 0) && (loop != 0)) {
			temp	=	(ucDigit[src] & 0xf0 ) >> 4;
		} else {
			temp	=	(ucDigit[src] & 0x0f );
			src++;
		}
		if ((int)temp >= 0x00 && temp < 0x0a) {
			temp	=	temp	+ 0x30;
		}

		if (temp == 0x0d ) {
			/* * */
			temp    =   temp    + 0x1d;
		}
		if (temp == 0x0e ) {
			/*  # */
			temp    =   temp    + 0x15;
		}

		ucChar[loop]	=	temp;
		loop++;
	}
	ucChar[loop] = '\0';

}
void str2MINDigit(u_char *cpStr, u_char *cpMinDigit, int nMinDigitLen)
{
	u_char    ucTemp[20]{};
	u_char	 ucTmp;
	int loop, nSrc;

	memcpy(ucTemp, cpStr, strlen((char *)cpStr));


	for (loop = 0; loop < (int)strlen((char *)cpStr); loop++) {

		if ((cpStr[loop] >= 0x30) && (cpStr[loop] < 0x3A)) {
			ucTemp[loop]    =   cpStr[loop] - 0x30;
		}
		if ((cpStr[loop] >= 0x41) && (cpStr[loop] < 0x47)) {
			ucTemp[loop]    =   cpStr[loop] - 0x37;
		}
		if ((cpStr[loop] >= 0x61) && (cpStr[loop] < 0x67)) {
			ucTemp[loop]    =   cpStr[loop] - 0x57;
		}
		if (cpStr[loop] == 0x2a) { /* * */
			ucTemp[loop]    =   cpStr[loop] - 0x1d;
		}
		if (cpStr[loop] == 0x23) { /* # */
			ucTemp[loop]    =   cpStr[loop] - 0x15;
		}
	}

	ucTmp = 0x00;
	for (loop = 0, nSrc = 0; nSrc < (int)strlen((char *)cpStr); nSrc++) {
		if ((nSrc != 0) && ((nSrc % 2))) {
			cpMinDigit[loop]   =   ucTmp | (ucTemp[nSrc] & 0x0f );
			loop++;
			ucTmp = 0x00;
		} else {
			ucTmp	=	 (ucTemp[nSrc] & 0x0f ) << 4;
		}
	}
	if (strlen((char *)cpStr) % 2) {
		cpMinDigit[(strlen((char *)cpStr) / 2)]	=	ucTmp | 0x0f;
		loop++;
	}
	memset(&cpMinDigit[loop], 0xff, nMinDigitLen - loop);
}

void digit2char(u_char *ucDigit, u_char *ucChar, int size)
{
	for (int i = 0, nSrc = 0; i < size * 2;) {
		u_char temp	=	(ucDigit[nSrc] & 0xf0 ) >> 4;
		if ((int)temp >= 0x00 && temp < 0x0a) {
			temp	=	temp	+ 0x30;
		}
		if (temp >= 0x0a && temp <= 0x0f) {
			temp	=	temp	+ 0x37;
		}
		ucChar[i]	=	temp;
		i++;

		temp	=	(ucDigit[nSrc] & 0x0f );
		if ((int)temp >= 0x00 && temp < 0x0a) {
			temp	=	temp	+ 0x30;
		}
		if (temp >= 0x0a && temp <= 0x0f) {
			temp	=	temp	+ 0x37;
		}
		ucChar[i]	=	temp;
		i++;

		nSrc++;
	}
	ucChar[size * 2] = '\0';
}

void str2DTMF(char *cpStr, u_char *cpDtmf)
{
	u_char   ucTmp = 0x00;

	for (int i = 0, nDest = 0; i < (int)strlen(cpStr); i++) {
		if ((cpStr[i] >= '1') && (cpStr[i] <= '9')) {
			ucTmp   =   cpStr[i] - 0x30;
		}

		if (cpStr[i] == '0') {
			ucTmp   =   0x0a;
		}
		if (cpStr[i] == '*') {
			ucTmp   =   0x0b;
		}
		if (cpStr[i] == '#') {
			ucTmp   =   0x0c;
		}

		if ((i % 2) != 0) {
			cpDtmf[nDest]  =   ucTmp | (0xf0 & cpDtmf[nDest]);
			nDest++;
		} else {
			cpDtmf[nDest]  =   (ucTmp & 0x0f) << 4;
		}

	}
}

/*---------------------------------------------------------------------
 *  FUNCTION NAME   : checkMqStatFunc()
 *  PURPOSE         : check message queue.
 *  INPUT ARGUMENTS :
 *						queue id.
 *  RETURN VALUE    :
 *  CALLED FROM     :
 *  USED GLOBAL VARIABLES :
 *  REVISION        :
 *      BY              WHEN        REASON
 *      --------------  ----------  -------------------
 *      molee           98.05.      First Coding
----------------------------------------------------------------------*/
int checkMqStatFunc(int nQid)
{

	struct msqid_ds     stBuf;

	/* more coding : errono */
	if (msgctl(nQid, IPC_STAT, &(stBuf)) == -1) {
		return FLK_FAIL;
	}

	if ( stBuf.msg_qnum > 100 ) {

		return FLK_FAIL;
	}

	return FLK_SUCCESS;
}
/*
 * for sql's select operation.
 */
/*
int dma_sql_select(int table_id, int att_id, char *attval, int tup_id, Result result, int flag)
int dma_sql_select(table_id, att_id, attval, tup_id, result, flag)
int table_id, att_id, tup_id, flag;
char    *attval;
Result  *result;
{

    Select  select;
    Select_Ack  ack;

    int     nResult;

    if (table_id < 0 || table_id >= MAX_TABLE)
        return -QP_INVALID_TABLE_ID;

    select.table_id=table_id;
    select.att_id  =att_id;
    select.tup_id  =tup_id;

    (void)memcpy(select.att_val,attval, MAX_ATT_PER_QP);

    if (flag&NO_RESULT)
        return 1;
    if (flag&EXTRA3_SELECT)
        return SELECT_EX3;

    nResult =   mdb_dma_select(&select,&ack,flag);
    if(nResult < 0)
    {
        return FLK_FAIL;
    }
    else
    {
        result->tup_id  =   ack.tup_id;
        memcpy(result->tuple,ack.tuple,MAX_TUPLE_SIZE);
    }
    return 1;
}
*/

/** @} */
