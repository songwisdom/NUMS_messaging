
/*******************************************************************************
*                                                                           
*     Copyright (c) 2000 FEELINGK Co., Ltd.                                
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

/***********************************************************************
    FILE NAME       :   AnsiProtocol.h
    PURPOSE         :   IS41C
    DATE OF WRITE   :   2001.12.08
    AUTHOR          :   Suk-Hun Jang
    REVISION        :   V 1.0
    Ver.    By      When        Reason
    ------- ------- ----------  -----------------------------------------
    1.0     shjang  2001.12.08  first coding
***********************************************************************/
#ifndef	_ANSIPROTOCOL_H_
#define	_ANSIPROTOCOL_H_
#include "libMapComm.h"

typedef	struct AddTypeString
{
	u_char TypeOfDigits;
	u_char NatureOfNumber;
	struct {
		BIT_T NumberingPlan		: 4;
		BIT_T Encoding			: 4;
	} m;
	BIT_T NumberOfDigits	: 8;
	u_char		data[ADDRESS_LEN_STRING];
} __attribute__ ((packed)) AddTypeString;


typedef	struct SmsAddress
{
	struct {
        BIT_T TypeOfDigits       : 8;
        BIT_T NatureOfNumber     : 8;
        BIT_T NumberingPlan      : 4;
        BIT_T Encoding           : 4;
    } m;
    u_int    numocts;
    u_char     data[ADDRESS_LEN_STRING];
	u_short	pc;
	u_char		ssn;
} SmsAddress;

typedef	struct
{
	struct {
		BIT_T esnPresent			: 1;
		BIT_T minPresent			: 1;
		BIT_T smsniPresent			: 1;
		BIT_T smscindPresent		: 1;
		BIT_T smsodaPresent			: 1;
		BIT_T smsooaPresent			: 1;
		BIT_T smsdaPresent			: 1;
		BIT_T smsiPresent			: 1;
		BIT_T osfiPresent			: 1;
		BIT_T roamindicatPresent	: 1;
		BIT_T roampmnPresent		: 1;
		BIT_T gwsmppPresent			: 1;
		BIT_T gwsmpp_receiptPresent	: 1;
		BIT_T gwsmpp_submitPresent	: 1;
		BIT_T gwsmdppPresent		: 1;
		BIT_T gwsmsnotiPresent		: 1;
	} m;
	OctetString			smsbd;
	OctetString			smsti;
	OctetString			esn;			/* O */
	OctetString			min;			/* O */
	OctetString			smsni;			/* O */
	OctetString			smscind;		/* O */
	OctetString 		smsoda;			/* O */
	OctetString 		smsooa;			/* O */
	OctetString 		smsda;			/* O */
	OctetString			smsi;		
	OctetString			osfi;
	OctetString			smsroamindicat;
	OctetString			smsroampmn;
	OctetString			gwsmpp;
	OctetString			gwsmpp_receipt;
	OctetString			gwsmpp_submit;
	OctetString			gwsmdpp;
	OctetString			gwsmsnoti;
} stSMDPP_t;
#endif
