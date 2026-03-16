/*******************************************************************************

  Copyright (c) 1997 LG Information & Communications
  All rights reserved.

  This program contains confidential and proprietary information of
  LGIC, and any reproduction, disclosure, or use in whole or in part
  is expressly  prohibited, except as may be specifically authorized
  by prior written agreement or permission of LGIC.

  LG Information & Communications, Ltd.
  395-65, Shindaebang-dong, Dongjak-ku, Seoul 153-023 Korea

#******************************************************************************/


#ifndef _MAPCOMSTRUCT_H_
#define _MAPCOMSTRUCT_H_
#include "FLKBaseDef.h"
#include <Map_Param.h>
#include <Map_Maxvaldef.h>
#include <time.h>

typedef struct {
	int				iTagLen;
	u_char 			cTagPac[MAX_TAGFIELDLEN];
} stTagfield;

typedef struct {
	int				iLengthLen;
	u_char				cLengthPac[MAX_LENFIELDLEN];
} stLenfield;

typedef struct {
	int				iContentsLen;
	u_char				cContentsPac[MAX_CONFIELDLEN];
} stConfield;


typedef struct {
	int				iSmsAddrON;  /* if ADDR_OFF then don't fill the current address */
	int				iAddrLength; /* set addrLength */
	u_char				ucSmsAddrType[SMS_ADDRESSLEN];	// 2byte
	u_char				ucDigitType; /* TON */
	u_char				ucNatureNum;
	u_char				ucEncoding;
	u_char				ucNumPlan;	/* NPI */
	u_char				ucAddrType; /* set MIN,TCP/IP, point code */
	u_char				ucSSN;
	u_char				ucAddrValue[MAX_ADDRVALUE];/*set address */ // 12byte
	/* added by 2000.11.28*/
	u_char				ucMaxType;
	u_char			    ucNetworkType;
	u_char 				ucHlrGrp; //SR510 pkg
	u_char			  	ucShGrp;
} stSmsAddr;

typedef struct {
	int				iContentsLen;
	u_char				ucContents[MAX_TELESERVICEID];
} stTeleserviceID;

/************************************************

  Bearer Data Configuration Description
  1.flag
major:
smc time stamp= 0
priority	  =	0
validity	  = 0
alert		  = 1
enable callback=1
vmn quick callback=1
minor:
if(enable callback is enable)
{
cmt callback(in user data)
vmn callback(in user data) = 0
vmn num msgs(in user data)
cpt callback(in user data)
}

2.config
language		=	0x10
privacy		=	0x00
data encoding	=	0x00
data packed
call back digit	= 0x00
call back ton		= 0x00
 ************************************************/

typedef struct {
	/*Flag*/
	int				isSmcTimestamp;
	int				isPriority;
	int				isPrivacy;
	int				isLanguage;
	int             isRelValidity;
	int             isAbsValidity;
	int				isAlert;
	int				isEnableCb;
	int				isCmtCb;
	int				isVmnCb;
	int				isCbUserData;
	int				isVmnNumMsgs;
	int				isCmtNumMsgs;
	int				isMelody;
	int				iMsgCnt;
	int			    isSessionInfo;
	int			    isReplyOption;
	int				isResponseType;
	int				isProperty;
	u_char			  	ucMsgId[4];
	u_char			  	ucAbsValidity[7];
	u_char			  	ucSubmittTime[7];
	u_char				ucLanguage;
	u_char				ucSKTLanguage;
	u_char				ucSTILanguage;
	u_char				ucPrivacy;
	u_char				ucPriority;
	u_char				ucDataEnc;
	u_char				ucDatePacked;
	u_char				ucCbDigit;
	u_char				ucCbTon;
	u_char				ucCbNpi;
} stBDConfig;

typedef struct {
	u_int			iContentsLen;
	u_char				ucContents[MAX_SMSBEARERDATA];

	u_char				ucPriority;
	u_char				ucPrivacy;
	u_char				ucAlert;
	u_char				ucLanguage;
	u_char				ucMessCt;
	u_char				ucData[MAX_DATA];

	u_int			nCbNumberLen;
	u_char				ucCbNumber[CALL_BACK_NUMBER_LEN];

	stBDConfig		config;

	u_char				ucMesgType;
	u_char   			ucMsgEncoding;
	u_int 			unMesgId;

	u_char				ucResponseCode;

	time_t 			ucSmcTimestamp;

	u_char	ucValidity[6];

	u_char	ucDeferDeliveryTime[6];

	u_char	ucUserAckReq;
	u_char	ucDakReq;

	u_char	CbNumber[MAX_CALL_BACK_NUM];


	/****** SKT SMGS : STI Interactive SMSC ************/
	u_char	ucResponseType;
	u_char	ucProperty[5];

	/* Adding by T.S.Jung. 990113 */
	u_char   ucCallbackMode;
	u_char	ucValidityRef; /* 1 : Abs, 2 : Rel */
	u_char   ucDeliveryRef; /* 1 : Abs, 2 : Rel */

	/* Adding by T.S.Jung. 010103 */
	u_char   ucSessionId;
	u_char	ucSessionSeq;
	u_char   ucEndSession;
} stBearerData;

typedef struct {
	u_char	ucTeleID[2];

	u_char	ucDigitMode;
	u_char	ucNumMode;
	u_char	ucNumType;
	u_char	ucNumPlan;
	u_char	ucAddress[SMS_ADDRESSLEN];

	u_char	ucSubAddType;
	u_char	ucSubAddOdd;
	u_char	ucSubAddress[SMS_ADDRESSLEN];

	u_char	ucReplyOption;

	u_char	ucReplySeq;
	u_char	ucErrClass;
	u_char	ucCauseCode;

	u_char	ucCategory[2];

} stTransportData;

/*
 * parameters they will be set to active db value
 *
 * stTeleSvcID
 * stSmsOrgOrgaddress
 * stSmsDestaddress
 * stSmsOrgDestaddress
 * ucMin
 */
#define	MO_MSG	0x00
#define	MT_MSG	0x01
typedef struct {
	u_int 			unMsgID;
	stBearerData	stSmsBearerData;
	stTeleserviceID	stTeleSvcID;
	stSmsAddr		stSmsaddress;
	stSmsAddr		stSmsOrgaddress;
	stSmsAddr		stSmsOrgOrgaddress;
	stSmsAddr		stSmsOrgOrgSubaddress;
	stSmsAddr		stSmsDestaddress;
	stSmsAddr		stSmsOrgDestaddress;
	stSmsAddr		stSmsOrgDestSubaddress;
	u_char				ucChargeID;
	u_char				ucNotiID;
	int				iMsgCnt;
	int				iAttempt;
	int				nMinLen;
	u_char				ucMin[MINLENGTH];
	u_char				ucEsn[ESNLENGTH];
	u_char   			ucAccDenRsn;
	u_char				ucSmsCauseCode;
	u_char				ucMsgStatus;
	u_char				ucMoMt;
#define	STATUS_HLR_SEND		1
#define	STATUS_MSC_SEND		2
#define STATUS_SUCCESS_DONE	3
#define STATUS_FAIL_DONE	4
	u_char				cStatus;
} stSmdppSendStruct;

/*
typedef struct {
	int           iErrFlag;
	char            cErrReason[50];
} stMapReterr;

typedef struct {
	int           iErrFlag;
	char            cErrReason[50];
} st2304Err_t;
typedef struct
{
	int             nSize;
	u_char   			cpData[256];
} stMap2304_t;

typedef struct {
	u_int  iContentsLen;
	u_char   ucContents[MAX_SMSBEARERDATA];
} stDec2304_t;
*/
/* 2009.04.17 USMSC
 * 기존 동일한 형식의 구조체를 아래와 같이 일괄 통합함.
 */
typedef struct {
	int           iErrFlag;
	char            cErrReason[50];
} stMapReterr;

typedef struct {
	int  	unContentsLen;
	u_char   	ucContents[MAX_SMSBEARERDATA];
} stMap2304_t;



//	For Dual ASEHandler
//	20040604	Taean
typedef	struct {
	int		nStatus;
	int		nPort;
	int		OriFD;
	int		Flag;
	int		MySideType;
	char		MyName[20];
	char		MYRIP_Address[24];
} ASEHandlerInfo_t;

// For Transaction Control
// 20041023 Jinseok

#define MAX_PC                      200
#define MAX_SSN                     256
#define MAX_PC_DES                  16
#define MAX_RATE                    255

typedef struct _stMsgCtrlPointCode {
	u_int	 nSsnCnt;
	u_int	 npSsn[MAX_SSN];
	u_int	 nPcCount;
	u_int	 npPointCode[MAX_PC];
	char 		 cpDes[MAX_PC][MAX_PC_DES];
} stMsgCtrlConfig;

/********* Check Map MO/MT ***********/
#define NEXT_IS_TAG         0x1F
#define NEXT_CONTINUE       0x80
#define EXTENTION_TAG       0x20

typedef struct __gmap_st {
	u_char       *tag;
	u_char       *len;
	u_char       *data;
	struct __gmap_st    *next;      // if not exist next parameter is NULL
	struct __gmap_st    *spmt;      // if not exist sub parameter is NULL
} pGmap_t;

#define MAX_DID_SIZE        100

typedef struct {
	u_int 	nDid;
	u_int 	nMid;
} stDialogueId_t;

extern stDialogueId_t   stDid[MAX_DID_SIZE];

#define     MAX_OCTET_MESSAGE_LEN   140

typedef struct bits_arr {
	u_char   s:  4;
	u_char   e:  4;
} bits4r;


#endif
