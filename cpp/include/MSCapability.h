/******************************************************************************
*
*     Copyright (c) 2001 FEELINGK Co., Ltd.
*     All rights reserved.
*
*     This program contains confidential and proprietary information of
*     FEELINGK, and any reproduction, disclosure, or use in whole or in part
*     by prior written agreement or permission of FEELINGK.
*
*                            FEELINGK Co., Ltd.
*   2F ACE Techno Twintower 6th. 212-1 Guro-dong Guro-Gu, Seoul, 152-050 Korea
*
#******************************************************************************/
/******************************************************************************
    FILE NAME       :   MSCapability.h
    PURPOSE         :   Color and Long SMS Specification.
    DATE OF WRITE   :   2003.04.20
    AUTHOR          :
    REVISION        :   V 0.1
    Ver.    By      When        Reason
    ------- ------- ----------  -----------------------------------------
    0.1             2003.04.20  first coding.
    2.0             2004.08.16  Add the SPAM Operation by HLR MSCapability.
******************************************************************************/

#if ! defined(_MS_CAPABILITY_H_)
#define _MS_CAPABILITY_H_

#include <sys/types.h>
#include <limits.h>


typedef enum _eLenthOfCapabilityInSS7MAP {
	MAP_TERM_CAPA_LEN	= 1,
	MAP_CAPA_BITS		= 2
} eCapaLenInMAP;

typedef enum _eTerminalCapability {
	MS_CAPA_NORMAL		= 0x00, // Black&White and Short Msg Display Only.
	MS_CAPA_COLOR		= 0x01, // Color and Short Msg Display Only.
	MS_CAPA_LONG		= 0x02, // Black&White and Long Msg Display Suport.
	MS_CAPA_COLOR_LONG	= MS_CAPA_COLOR | MS_CAPA_LONG, // Color and Long.
	MS_CAPA_RSV_START,
	MS_CAPA_RSV_END		= 0xFF
} eTermCapa;

typedef enum _eColorLongTIDSequence {
	SEQ_DEFAULT_TID,
	SEQ_COLOR_TID,
	SEQ_LONG_TID,
	SEQ_COLORLONG_TID,
	LONG_TID_MAX
} eCLTIDSeq;

typedef enum _eColorLongTeleserviceIdEntifier {	// from Smsc.cfg
	TID_DEFAULT			=  4098,
	TID_COLOR_SVC		= 63210, // f6ea
	TID_LONG_SVC		= 63230, // f6fe
	TID_COLOR_LONG_SVC	= 63240	, // f708
	TID_WLONG 	        = 61842 //0xF192  FR210 add WLong
} eCLTIDEnti;

typedef enum _eLongMsgHeaderFiledLength {
	LONG_SESSION_ID_BYTE_POS		= 0,
	LONG_SEGMENT_INFO_BYTE_POS		= 1,

	LONG_SESSION_ID_BYTE_SIZE		= 1,
	LONG_SEGMENT_INFO_BYTE_SIZE		= 1,
	LONG_HEADER_TOTAL_BYTE_SIZE		=
	    LONG_SESSION_ID_BYTE_SIZE + LONG_SEGMENT_INFO_BYTE_SIZE,
	LONG_TOTAL_SEGMENT_BIT_SIZE		= 4,
	LONG_CURREENT_SEGMENT_BIT_SIZE	= 4
} eLMsgHeaderLen;

enum { LONG_CURREENT_SEGMENT_BIT_MASK = 0x0f };
enum { START_CONVERT_CHAR = '0' };
enum { LONG_MSGLEN_PER_SEG = 80 }; // the max Long message length per a segment

#define getTIDRoleName(tid)	CvtCapaToText(isColorLongTi((tid)))
#define _LONG_SEGINFO_

typedef enum {
	SPAM_PATTERN_NO     = 0x00000000,
	SPAM_PATTERN_MIN    = 0x00000001,
	SPAM_PATTERN_MAX    = 0x80000000
} eSPAMPatternFlag;

#include <MdbTableInfo.h>

enum {
	SPAM_PAATERN_BITS_MAX = DB_SPAM_FLAG_SIZE * CHAR_BIT,
	SPAM_MINOR_SUBSCRIBER_BIT = 0x20
};

typedef enum {
	FREE_NUMBER_TYPE	= 0x00,
	COMPANY_NUMBER_TYPE	= 0x01,
	OFFICE_NUMBER_TYPE	= 0x02,
	SPAM_MAXIMUM_TYPE
} eSPAMPatternType;

#endif
