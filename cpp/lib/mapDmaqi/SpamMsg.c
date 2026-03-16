/**
 @ingroup mapDmaqi
 @{
 */
/******************************************************************************#
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
#include <string.h>
#include <libMapDmaqi.h>

#include <MdbCommon.h>
#include <MdbTableInfo.h>
#include <FLKBaseDef.h>
#include "smscString.h"

#include "Debug.h"
#include "MdbDmaqi.h"

/*******************************************************************************
    FILE NAME       :   PlusCheck.c
    PURPOSE         :

    DATE OF WRITE   :   Thu Jul 29 16:11:36 KST 2004
    AUTHOR          :
    REVISION        :   V 1.0.8
    Ver.    By      When        Reason
    ------- ------- ----------  -----------------------------------------
    1.0.1   JDShim  2002.12.06   create file and first coding.
    1.0.8   JDShim  2004.07.29   method change IsSPAMCallBack() from PlusDB
	                             Checking to MS_Capability Checking.
*******************************************************************************/

#include <MSCapability.h>

enum { PERSONAL_NUMBER_LEN = 0x04 };    // xxx-xxxx-XXXX
enum { NULL_CALLBACKNUMBER = 0x00 };    // 00
enum { MOBILE_INDICATOR_OF_CALLBACK = 0xa1 }; // 01 -> a1
enum { MOBILE_INDICATOR_OF_CALLBACK_POS = 0 }; // first byte.
enum { STR_CALLBACK_NUMBER_SIZE = DB_CALLBACK_NUMBER_SIZE * 2 + 1 }; // first byte.

/*------------------------------------------------------------------------------
 *
 *  FUNCTION NAME   : IsSPAMCallBack()
 *  PURPOSE         :
 *  INPUT ARGUMENTS :
 *  RETURN VALUE    : If the Callback is a SPAM Pattern, it returns true.
 *                    Otherwise, it returns false.
 *
 *  GLOBAL VARIABLES
 *    1) EXTERNAL   :
 *    2) LOCAL      :
 *
------------------------------------------------------------------------------*/
int IsSPAMCallBack(u_char *ucpCallBack, u_char *ucpSpamFlag,
                   size_t siSpamSize, u_long ulMaxPtn)
{
	if (SPAM_PATTERN_MAX < ulMaxPtn) {
		return false;
	}

	if (DB_SPAM_FLAG_SIZE != siSpamSize) {
		return false;
	}

	// Step 1. If CallBack is NULL or Office Number Length is Four, Skip.
	DLOG(("Input CallBack [%02X%02X%02X%02X%02X%02X]\n",
	      ucpCallBack[0], ucpCallBack[1], ucpCallBack[2],
	      ucpCallBack[3], ucpCallBack[4], ucpCallBack[5]));

	if (NULL_CALLBACKNUMBER ==
	        ucpCallBack[MOBILE_INDICATOR_OF_CALLBACK_POS] ||
	        MOBILE_INDICATOR_OF_CALLBACK ==
	        ucpCallBack[MOBILE_INDICATOR_OF_CALLBACK_POS]) {
		DLOG(("return CallBack [%02X%02X%02X%02X%02X%02X]\n",
		      ucpCallBack[0], ucpCallBack[1], ucpCallBack[2],
		      ucpCallBack[3], ucpCallBack[4], ucpCallBack[5]));

		return false;
	}


	// Step 3. If SPAM Field is NULL, Skip.
	u_int unSpamFlag = SPAM_PATTERN_NO;
	memcpy(&unSpamFlag, ucpSpamFlag, sizeof(unSpamFlag));
	if (SPAM_PATTERN_NO == unSpamFlag) {
		return false;
	}

	// Step 4. Convert CallBack from Hexa to String.
	char l_caCallBack[STR_CALLBACK_NUMBER_SIZE];
	bzero(l_caCallBack, STR_CALLBACK_NUMBER_SIZE);
	BCD2str_callback((char *)l_caCallBack, STR_CALLBACK_NUMBER_SIZE,
	                 ucpCallBack, DB_CALLBACK_NUMBER_SIZE);

	DLOG(("CallBack Str [%s]\n", l_caCallBack));

	stRDSpamTuple *l_spSpam = NULL;
	char l_caPattern[STR_CALLBACK_NUMBER_SIZE];

	// Step 5. Check Set Bit Flags.
	// for (u_int unBitChk = SPAM_PATTERN_MIN;
	//	unBitChk <= SPAM_PATTERN_MAX; unBitChk <<= 1)
	// Modify Spam Flag Sequence. from right bit starting to left bit starting.
	// Scan bits on SpamFlag from Left(MSB) to right.
	// 2004.04.20 by JD Shim
	//for (u_int unBitChk = SPAM_PATTERN_MIN;
	//	unBitChk >= SPAM_PATTERN_MAX; unBitChk >>= 1)
	// Modify Spam Flag Sequence. from left bit starting to right bit starting.
	// Scan bits on SpamFlag from Right (LSB) to left.
	// and MaxPattern Check.
	//  by JD Shim at Fri Jul 30 16:55:53 KST 2004
	u_int unBitChk;
	for (unBitChk = SPAM_PATTERN_MIN;
	        unBitChk <= ulMaxPtn; unBitChk <<= 1) {
		// Step 5.1 Is this pattern set ?
		if (!(unSpamFlag & unBitChk)) {
			DLOG(("No Check Pattern %i = (SpamFlag[0x%08X] & BitChk[0x%08X])\n",
			      (unSpamFlag & unBitChk), unSpamFlag, unBitChk));
			continue;
		}

		l_spSpam = NULL;

		// Step 5.2 Is there this pattern in DB ?
		if (dma_getTuplePtr(DBMS_PLUS, R_DSPAM, DB_SPAM_ID, 0, (char *)&unBitChk,
		                    DB_SPAM_ID_SIZE, (char **)&l_spSpam) < 0) {
			continue;
		}

		// Step 5.3 Convert SPAM Pattern from Hexa to String.
		bzero(l_caPattern, STR_CALLBACK_NUMBER_SIZE);
		BCD2str_min((char *)l_caPattern, STR_CALLBACK_NUMBER_SIZE,
		            l_spSpam->dbSpamPattern, DB_SPAM_PATTERN_SIZE);

		DLOG(("Spam Pattern Str [%s]\n", l_caPattern));

		// Step 5.4 Is this pattern set on the company number type ?
		if (l_spSpam->dbSpamType[0] & COMPANY_NUMBER_TYPE) {
			if (!memcmp(l_caCallBack, l_caPattern, l_spSpam->dbSpamLength[0])) {
				// Pattern Agreement.
				DLOG(("SPAM CB[%s] SP[%s]\n", l_caCallBack, l_caPattern));
				return true;
			}
		}

		// Step 3.6 Is this pattern set on the office number type ?
		if (l_spSpam->dbSpamType[0] & OFFICE_NUMBER_TYPE) {
			int l_iMinLen = (int)strlen(l_caCallBack) -
			                (PERSONAL_NUMBER_LEN + l_spSpam->dbSpamLength[0]);
			if (!memcmp(l_caCallBack + l_iMinLen, l_caPattern,
			            l_spSpam->dbSpamLength[0])) {
				// Pattern Agreement.
				DLOG(("SPAM CB[%s] SP[%s]\n", l_caCallBack, l_caPattern));
				return true;
			}
		}
	}// for (unBitChk)
	DLOG(("It's No Spam CallBack\n"));
	return false;
}
/** @} */
