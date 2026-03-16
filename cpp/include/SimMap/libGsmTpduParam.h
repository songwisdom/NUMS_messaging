/**
 * =====================================================================================
 *
 *       @file  libGsmTpduParam.h
 *
 *    	 @brief 
 *
 *       Version:  1.0
 *       Compiler:  cc (HP-UX)
 *       @date:  2009년 05월 15일 01시 11분 40초
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

#ifndef _LIB_H_GsmTpduParam_
#define _LIB_H_GsmTpduParam_

#include "flk_type.h"
#include "libMapComm.h"
#include "libMapAddrUtil.h"
#include "libGsmTpduValue.h"

/****************************************************************************/
/*																			*/
/* 	Definition of GSM TPDU FUNCTION ERROR 									*/
/*																			*/
/****************************************************************************/
extern int			code_GSM_TPDU_Error;


#define		SET_INVALID_VALUE				1	/* argument error */
#define		ENC_TP_MTI_UNKNOWN				2	/* Encoding input TP-MTI value error */
#define		ENC_ERR_WRONG_TPDU_TYPE			3
#define		ERR_TPDU_ADDRESS_LEN			4
#define		DEC_INVALID_FORMAT				5
#define		ENC_INVALID_REPORT				6
#define		CONVERT_7TO8_ERROR				7
#define		ENC_DATASIZE_ERROR				8
#define		DEC_DATASIZE_ERROR				8
#define		ENC_COMVAL_ERROR				9
#define		ENC_CONVERT_SIZE_OVER			10
#define		ENC_ICONV_ERROR					11

#define		getGSMTPDUErrorCode()			(code_GSM_TPDU_Error)
#define		setGSMTPDUErrorCode(val)		(code_GSM_TPDU_Error = val)
#define		setGSMTPDUErrorString(buf,val)	(memcpy(buf,val,strlen(val)))
//#define		getGSMTPDUErrorString()			(tpdu_errString[code_GSM_TPDU_Error]) 
#define		getGSMTPDUErrorString()			(func_tpdu_errString()) 
#if 0
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
#endif
/* =============================================
 * Use Struct 
 * ============================================= */
typedef union{
    struct {
		// TODO linux porting
        BIT_T       digit_odd   : 4;
        BIT_T       digit_even  : 4;
    }semi_octet;
    u_char         digit;
#define digitOdd    semi_octet.digit_odd
#define digitEven   semi_octet.digit_even
#define digit1st    semi_octet.digit_odd
#define digit2nd    semi_octet.digit_even
} __attribute__ ((packed)) SemiOctet;
//#include <GsmTpduErr.h>
/****************************************************************************
 *
 *  Structure of Parameter 
 *
 ****************************************************************************/ 
/*--------------------------------------------------------------------------* 
 * TP-MTI Parameter structure 	
 * This is a 2-bit field, located within bits no 0 and 1 of the first octet 
 * of all PDUs which can be given the value 
 * see  GsmTpduValue.h(eTPMTIFieldValue) file
 *--------------------------------------------------------------------------*/ 
typedef struct {
	// TODO linux porting
	unsigned char		value 	: 2;	/* 0 - 1 */ 
	unsigned char		align 	: 6;	/* 2 - 7 */ 
}TP_MTIParam;

/*--------------------------------------------------------------------------*
 * TP-MMS Parameter structure  2nd bit											*
 * This is a 1-bit field, located within bit no 2 of the first octet of
 * SMS-DELIVER and SMS-STATUS-REPORT, and to be given the value
 * see GsmTpduValue.h(eTPMMSFieldValue) file
 *   Notice: In the case of SMS-STATUS-REPORT this parameter refers to 
 *           messages waiting for the mobile to which the status report is
 *			 sent. The term message in this context refers to SMS-messges
 *			 or status reports
 *--------------------------------------------------------------------------*/
typedef struct {
	// TODO linux porting
	unsigned char		align2 	: 2;	/* 0 - 1 */
	unsigned char		value  	: 1;	/* 2 	 */
	unsigned char		align1 	: 5;	/* 3 - 7 */	
}TP_MMSParam;

/*--------------------------------------------------------------------------*
 * TP-RD Parameter structure  5th bit										*
 *--------------------------------------------------------------------------*/
typedef TP_MMSParam		TP_RDParam;

/*--------------------------------------------------------------------------*
 * TP-VPF Parameter structure 
 * This is a 2-bits field, located within bit no 3 and 4 of the first octet
 * of SMS-SUBMIT, and to be given value in GsmTpduValue.h(eTPVPFFieldValue)
 * 	 Notice: Any unsupported value may be rejected by the SC by returning the
 *			 "TP-VPF not supported(H'C6)" TP-FCS value in the SMS Submit Report 
 *			 for RP-Error
 *--------------------------------------------------------------------------*/
typedef struct {
	// TODO linux porting
	unsigned char		align2 	: 3;		/* alignment */
	unsigned char		value  	: 2;		/* TP-VPF Value */
	unsigned char		align1 	: 3;		/* alignment */
}TP_VPFParam;

/*--------------------------------------------------------------------------*
 * TP-SRI Parameter structure,  5th bit of the first octet of SMS-SUBMIT 	
 *--------------------------------------------------------------------------*/
typedef struct {
	// TODO linux porting
	unsigned char		align2 	: 5;
	unsigned char		value 	: 1;
	unsigned char		align1 	: 2;
}TP_SRIParam;
/*--------------------------------------------------------------------------*
 * TP-SRR/SRQ Parameter structure, 5th bit of the first octet of SMS-SUBMIT
 * and SMS-COMMAND	
 *--------------------------------------------------------------------------*/
typedef TP_SRIParam	TP_SRRParam;
typedef TP_SRIParam	TP_SRQParam;

/*--------------------------------------------------------------------------*
 * TP-UDHI Parameter structure  6th bit										*
 *--------------------------------------------------------------------------*/
typedef struct {
	// TODO linux porting
	unsigned char		align2 	: 6;
	unsigned char		value 	: 1;
	unsigned char		align1 	: 1;
}TP_UDHIParam;

/*--------------------------------------------------------------------------*
 * TP-RP Parameter structure  7th bit										*
 *--------------------------------------------------------------------------*/
typedef struct {
	// TODO linux porting
	unsigned char		align 	: 7;
	unsigned char		value 	: 1;
}TP_RPParam;

/*--------------------------------------------------------------------------*
 * View Function										*
 *--------------------------------------------------------------------------*/
typedef union{
	TP_MTIParam		mti;
	TP_MMSParam		mms;
	TP_RPParam		rp;
	TP_RDParam		rd;
	TP_SRIParam		sri;
	TP_SRRParam		srr;
	TP_SRQParam		srq;
	TP_VPFParam		vpf;
	TP_UDHIParam	udhi;
	unsigned char	charval;
}TP_ParamIDC;
void	printParamReserved(int postion,int size);
void	printParamTPMTIValue(unsigned char,int);
void	printParamTPMMSValue(unsigned char,int);
void	printParamTPRDValue(unsigned char,int);
void	printParamTPRPValue(unsigned char,int);
void	printParamTPSRIValue(unsigned char,int);
void	printParamTPSRQValue(unsigned char,int);
void	printParamTPSRRValue(unsigned char,int);
void	printParamTPVPFValue(unsigned char,int);
void	printParamTPUDHIValue(unsigned char,int);

/*--------------------------------------------------------------------------*
 * TP-OA Parameter structure 												*
 *--------------------------------------------------------------------------*/
#define TPDU_ADDRESS_LENGTH		12

typedef struct {
	unsigned char		addrLen;			/* address length */
	union {
		struct {
			// TODO linux porting
			unsigned char		addrNumPlan	:4;		/* numbering of plan */
			unsigned char		addrType	:4;		/* type of address */

		}detail_indicator;
		unsigned char			addr_indicator;
	}addr_idc;
	int					addrOctetSize;
	SemiOctet			address[TPDU_ADDRESS_LENGTH];	/* semi-octet address */
}TP_OAParam;

#define	pAddrIdc		addr_idc.addr_indicator
#define	pAddrType		addr_idc.detail_indicator.addrType
#define	pAddrNumPlan	addr_idc.detail_indicator.addrNumPlan	
/*--------------------------------------------------------------------------*
 * TP-DA/RA Parameter structure 											*
 *--------------------------------------------------------------------------*/
typedef TP_OAParam	TP_DAParam;
typedef TP_OAParam	TP_RAParam;
/*--------------------------------------------------------------------------*
 * TP-OA/DA/RA Funtion		 												*
 *--------------------------------------------------------------------------*/
void	printParamTPAddress(TP_OAParam*);
char	getParamAddrChar(int);
char	getParamCBAddrChar(int);
int		encParamTPAddress(unsigned char *,TP_OAParam*);
int		decParamTPAddress(unsigned char *,TP_OAParam*);
int		decParamTPAddress2(unsigned char *,TP_OAParam*);
int		decParamTPAddress3(unsigned char *,TP_OAParam*);
char*	getParamDecBCDAddress(TP_OAParam *addr);
/*--------------------------------------------------------------------------*
 * TP-PID Parameter structure 												*
 *--------------------------------------------------------------------------*/
typedef struct{
	// TODO linux porting
	unsigned char		reservied	:	6;	
	unsigned char		assign		:	2;	
}TPPIDType_t;

typedef struct {
	// TODO linux porting
	unsigned char		value		:	6;
	unsigned char		reserved	:	2;
}TPPIDNoIw_t;

typedef struct {
	// TODO linux porting
	unsigned char		value		:	5;
	unsigned char		interworking:	1;
	unsigned char		reserved	:	2;
}TPPIDIncIw_t;

typedef union{
	TPPIDType_t			pidType;
	TPPIDIncIw_t		pidIncIw;
	TPPIDNoIw_t			pidNoIw;
	unsigned char		value;
#define	pidType			pidType.assign
#define	pidValue		pidNoIw.value
#define	pidValueIncIw	pidIncIw.value
#define	pidInterworking	pidIncIw.interworking
}TP_PIDParam;
#define	GetPIDType(pid)	(pid.pidType)
#define	GetPIDInterworking(pid)	(pid.pidInterworking)
#define	GetPIDValueItw(pid)		(pid.pidValueIncIw)
#define	GetPIDValueNoItw(pid)	(pid.pidValue)
void    printParamPIDValue(TP_PIDParam *param);
/*--------------------------------------------------------------------------*
 * TP-DCS Parameter structure 												*
 *--------------------------------------------------------------------------*/
typedef struct{
	// TODO linux porting
	unsigned char		reserved	: 4;
	unsigned char		value		: 4;	/* Coding Group bits */
} DCSGroup_t;

typedef struct{
	// TODO linux porting
	unsigned char		reserved1:2;
	unsigned char		value	: 2;	/* character set, if group value 1111 then this 1 byte(remain reserved) */
	unsigned char		reserved: 4;
}DCSCharset_t;

typedef struct{
	// TODO linux porting
	unsigned char		value	: 2;	/* message class */
	unsigned char		reserved1: 2;
	unsigned char		reserved: 4;
}DCSClass_t;

typedef struct{
	// TODO linux porting
	unsigned char		reserved1:3;
	unsigned char		value	: 1;	
	unsigned char		reserved: 4;
}DCSSense_t;

typedef DCSSense_t		DCSIdcType_t;

typedef union{
	DCSGroup_t			group;
	DCSCharset_t		charset;
	DCSClass_t			cls;
	DCSSense_t			sense;
	DCSIdcType_t		idc;
	unsigned char		value;
#define	dcsGroup				group.value
#define	dcsCharset				charset.value
#define	dcsClass				cls.value
#define	dcsSense				sense.value
#define	dcsIdcType				idc.value
}TP_DCSParam;

#define			DCS_GEN_UNCOMP_NO_CLASS_GROUP			0
#define			DCS_GEN_UNCOMP_USE_CLASS_GROUP			1
#define			DCS_GEN_COMP_NO_CLASS_GROUP				2
#define			DCS_GEN_COMP_USE_CLASS_GROUP			3
#define			DCS_MSGMARG_UNCOMP_NO_CLASS_GROUP 		4	
#define			DCS_MSGMARG_UNCOMP_USE_CLASS_GROUP 		5	
#define			DCS_MSGMARG_COMP_NO_CLASS_GROUP 		6	
#define			DCS_MSGMARG_COMP_USE_CLASS_GROUP 		7	
#define			DCS_RESERVED_CODING_GROUP_BASE 			8	
#define			DCS_SKT_CODING_GROUP	DCS_RESERVED_CODING_GROUP_BASE
#define			DCS_MSG_WAIT_DISCARD_GROUP				12
#define			DCS_MSG_WAIT_STORE_GROUP				13
#define			DCS_MSG_WAIT_STORE_UCS2_GROUP			14
#define			DCS_MSG_CODING_DATA_CLASS_GROUP			15

#define	GetDCSGroup(dcs)	(dcs.dcsGroup)
#define	GetDCSCharset(dcs)	(dcs.dcsCharset)
#define	GetDCSClass(dcs)	(dcs.dcsClass)
#define	GetDCSSense(dcs)	(dcs.dcsSense)
#define	GetDCSIdcType(dcs)	(dcs.dcsIdcType)

#define	SetDCSGroup(dcs,val)	(dcs.dcsGroup = val)
#define	SetDCSCharset(dcs,val)	(dcs.dcsCharset = val)
#define	SetDCSClass(dcs,val)	(dcs.dcsClass = val)


void		printParamDCSValue(TP_DCSParam*);
/*--------------------------------------------------------------------------*
 * TP-SCTS Parameter structure 												*
 *--------------------------------------------------------------------------*/
typedef struct{
	SemiOctet				value[7];			/* 00 */
}TP_SCTSParam;
void		printParamSCTSValue(TP_SCTSParam*,int);
#define		GetSCTSValueIdx(scts,idx)	(scts.value[idx])
void		setParamSCTSValue(unsigned char *buf,unsigned char *sctsval);
unsigned char* getParamSCTSValue(unsigned char*);
/*--------------------------------------------------------------------------*
 * TP-DT Parameter structure 												*
 *--------------------------------------------------------------------------*/
typedef TP_SCTSParam	TP_DTParam;

/*--------------------------------------------------------------------------*
 * TP-ST/CT Parameter structure 												*
 *--------------------------------------------------------------------------*/
typedef unsigned char	TP_STParam;
typedef unsigned char	TP_CTParam;

/*--------------------------------------------------------------------------*
 * TP-VP Parameter1 -7 bytes												*
 *--------------------------------------------------------------------------*/
#define DRLTIME1(val)       ((val+1)*5*60)
#define DRLTIME2(val)       ((12*60*60)+(val-143)*30*60)
#define DRLTIME3(val)       ((val-166)*24*60*60)
#define DRLTIME4(val)       ((val-192)*7*24*60*60)


#define ERLTIME1(val)       (val/60/5-1)
#define ERLTIME2(val)       ((val/60/30+143)-(12*2))    
#define ERLTIME3(val)       (val/24/60/60+166)
#define ERLTIME4(val)       (val/7/24/60/60+192)

typedef unsigned char		TP_VPParamRelative;
typedef TP_SCTSParam		TP_VPParamAbsolute;

typedef struct{
	unsigned char			extention : 1;
	unsigned char			singleShortSm:1;
	unsigned char			align 	  :	3;
	unsigned char			format	  :	3;
}TP_VPParamEnhancedIdc;

typedef struct{
	union{
		TP_VPParamEnhancedIdc	idc;
		unsigned char			value;
	}indicator;
	unsigned char			value[6];
}TP_VPParamEnhance;

typedef struct{
	int							type;	/* -1: not set, 0-relative,1-enhance,2-absolute */
	union{
		TP_VPParamRelative		relative;
		TP_VPParamAbsolute		absolute;
		TP_VPParamEnhance		enhance;
		unsigned char			value[7];
	}vpdata;
}TP_VPParam;

#define		VP_RELATIVE_TYPE				2
#define		VP_ENHANCED_TYPE				1
#define		VP_ABSOLUTE_TYPE				3

#define		TPVP_MAXSIZE					7

#define	vpRelative		vpdata.relative
#define	vpAbsolute		vpdata.absolute
#define	vpEnExtention	vpdata.enhance.indicator.idc.extention
#define	vpEnSingleShortSM	vpdata.enhance.indicator.idc.singleShortSm
#define	vpEnFormat		vpdata.enhance.indicator.idc.format
#define vpEnIndicator	vpdata.enhance.indicator.idc
#define vpEnIdcValue	vpdata.enhance.indicator.value
#define	vpEnValue		vpdata.enhance.value
#define	vpEnhance		vpdata.enhance
#define	vpValue			vpdata.value

unsigned long	getRelativeTime(TP_VPParam*,int);/* 0-sec,1-min,2-hour,3-day */
unsigned long	setRelativeTime(TP_VPParam*,int); /* 2nd argument is second */

void	printParamVPValue(TP_VPParam*);

/*--------------------------------------------------------------------------*
 * TP-FCS/MR/MN/UDL/CDL Parameter Redefine 												*
 *--------------------------------------------------------------------------*/
typedef unsigned char	TP_FCSParam;
char*   getParamFCSReason(TP_FCSParam data);

/*--------------------------------------------------------------------------*
 * TP-MR Parameter
 * This field gives an integer representation of a reference number of the 
 * SMS-SUBMIT and SMS-COMMAND submitted to the SC by the MS. The MS increments 
 * TP-MR by 1 for each SMS-SUBMIT or SMS-COMMAND being submitted.
 *--------------------------------------------------------------------------*/
typedef unsigned char	TP_MRParam;
typedef unsigned char	TP_MNParam;
typedef unsigned char	TP_UDLParam;
typedef unsigned char	TP_CDLParam;

/*--------------------------------------------------------------------------*
 * TP-PI Parameter Redefine 												*
 *--------------------------------------------------------------------------*/
typedef struct{
	// TODO linux porting
	unsigned char		pid			:1;	/* TP-PID */
	unsigned char		dcs			:1;	/* TP-DCS */
	unsigned char		udl			:1;	/* TP-UDL */
	unsigned char		reserved	:4;
	unsigned char		extention	:1;	/* extenion bit */
}TP_PIParamBits;

typedef union{
	TP_PIParamBits		bits;
	unsigned char		value;
}TP_PIParam;
#define	piExtention		bits.extention
#define	piUdl			bits.udl
#define	piPid			bits.pid	
#define	piDcs			bits.dcs	

void	printParamPIValue(TP_PIParam*);

#define	IsExistPID(pi)	(pi.piPid)
#define	IsExistUDL(pi)	(pi.piUdl)
#define	IsExistDCS(pi)	(pi.piDcs)
#define	IsExistEXT(pi)	(pi.piExtention)

/*--------------------------------------------------------------------------*
 * TP-UD Parameter Structure												*
 *--------------------------------------------------------------------------*/
#define		MAX_TP_USER_DATA			160

typedef struct{
	int						size;
	unsigned char			data[MAX_TP_USER_DATA];
}TP_UDParam;

void	printParamUDValue(TP_UDParam*,int is8bit,int udhi);
#define	GetUDParamValue(ud)		(ud.data)
#define	GetUDParamSize(ud)		(ud.size)
/*--------------------------------------------------------------------------*
 * TP-CD Parameter Structure												*
 *--------------------------------------------------------------------------*/
typedef TP_UDParam	TP_CDParam;

/*--------------------------------------------------------------------------*
 * ETC						 												*
 *--------------------------------------------------------------------------*/
/* optional check bit */
typedef struct{
	// TODO linux porting
	unsigned 	char		ud		:1;
	unsigned 	char		vp		:1;
	unsigned 	char		udl		:1;
	unsigned 	char		dcs		:1;
	unsigned 	char		pid		:1;
	unsigned 	char		udhi	:1;
	unsigned 	char		sri		:1;
	unsigned 	char		srr		:1;
}optCheckBit;

typedef union{
	optCheckBit			Bits;
	unsigned char		octet;
}tpduOptional;
#define		EXIST_PARAM				1
#define		NOT_EXIST_PARAM			0	

/* octet redefinition */
typedef struct{
	// TODO linux porting
    unsigned char   e   :7;
    unsigned char   s   :1;
}bits1;

typedef struct{
	// TODO linux porting
    unsigned char   e   :6;
    unsigned char   s   :2;
}bits2;

typedef struct{
	// TODO linux porting
    unsigned char   e   :5;
    unsigned char   s   :3;
}bits3;

typedef struct{
	// TODO linux porting
    unsigned char   e   :4;
    unsigned char   s   :4;
}bits4;

typedef struct{
	// TODO linux porting
    unsigned char   e   :3;
    unsigned char   s   :5;
}bits5;

typedef struct{
	// TODO linux porting
    unsigned char   e   :2;
    unsigned char   s   :6;
}bits6;

typedef struct{
	// TODO linux porting
    unsigned char   e   :1;
    unsigned char   s   :7;
}bits7;

typedef union{
	bits1		bit1;
	bits2		bit2;
	bits3		bit3;
	bits4		bit4;
	bits5		bit5;
	bits6		bit6;
	bits7		bit7;
	unsigned char	val;
}octet;


/******************************************************************************
 *		Function Of Bits
 *****************************************************************************/

#ifndef TPDU_TRUE
#define TPDU_TRUE       1
#define TPDU_FALSE      -1
#endif

/* Indicator Type */
#define     TPDU_IDC_MTI    1
#define     TPDU_IDC_MMS    2
#define     TPDU_IDC_RD     2
#define     TPDU_IDC_VPF    3
#define     TPDU_IDC_SRR    5
#define     TPDU_IDC_SRI    5
#define     TPDU_IDC_SRQ    5
#define     TPDU_IDC_UDHI   6
#define     TPDU_IDC_RP     7

/******************************************************************************
 *		TPDU UDH 			
 *****************************************************************************/
#define 	IEI_ST_MAX_SIZE		10


/******************************************************************************
 * SKT TPDU UDH 
 *****************************************************************************/
/* TAG */
typedef enum {
	TPDU_CT_8bit_REF_TAG		=	0x00,
	TPDU_SPECIAL_SMS_IND_TAG	=	0x01,
	TPDU_RESERVED_TAG			=	0x02,
	TPDU_VALUE_NOT_USED_TAG		=	0x03,
	TPDU_AP_8bit_PORT_TAG		=	0x04,
	TPDU_AP_16ibt_PORT_TAG		=	0x05,
	TPDU_SMSC_CONTROL_PARAM_TAG	=	0x06,
	TPDU_UDH_SOURCE_IND_TAG		=	0x07,
	TPDU_CT_16bit_REF_TAG		=	0x08,
	TPDU_WIRE_CTL_MSG_PTL_TAG	=	0x09,
	TPDU_TEXT_FORMAT_TAG		=	0x0A,
	TPDU_PREDEFINED_SOUND_TAG	=	0x0B,
	TPDU_USER_DEFINED_SOUND_TAG	=	0x0C,
	TPDU_PREDEFINED_ANI_TAG		=	0x0D,
	TPDU_LARGE_ANI_TAG			=	0x0E,
	TPDU_SMALL_ANI_TAG			=	0x0F,
	TPDU_LARGE_PICTURE_TAG		=	0x10,
	TPDU_SMALL_PICTURE_TAG		=	0x11,
	TPDU_VARIABLE_PICTURE_TAG	=	0x12,
	TPDU_USER_PROMPT_IND_TAG	=	0x13,
	TPDU_EXTENDED_OBJECT_TAG	=	0x14,
	TPDU_REUSED_EXTENDED_OBJ_TAG=	0x15,
	TPDU_COMPRESSION_CTL_TAG	=	0x16,
	TPDU_OBJ_DISTN_IND_TAG		=	0x17,
	TPDU_STN_WVG_OBJECT_TAG		=	0x18,
	TPDU_CHAR_SIZE_WVG_OBJ_TAG	=	0x19,
	TPDU_EXT_OBJ_DATA_REQ_COM_TAG=	0x1A,
	TPDU_REF_822_EMAIL_HEAD_TAG	=	0x20,
	TPDU_HYPER_FORMAT_EMT_TAG	=	0x21,
	TPDU_REPLY_ADDRESS_TAG		=	0x22,
	TPDU_ENHAN_VOICE_M_INFO_TAG	=	0x23
} eTPDU_IEI_Type;



/* LENGTH */
#define		TPDU_CR_8_REF_TOTAL_SIZE	3
#define		TPDU_CR_16_REF_TOTAL_SIZE	4
#define		TPDU_CR_16_REF_SIZE			2
#define		TPDU_SPECIAL_SMS_SIZE		2
#define 	TPDU_AP_8_PORT_SIZE			1
#define 	TPDU_AP_16_PORT_SIZE		2
#define		TPDU_HYPER_FORMAT_SIZE		4
#define		TPDU_HYPER_POSITION_SIZE	2

/* MAX_SIZE */
#define		TPDU_MAX_BASE_ONE_OCTET		255
#define		TPDU_MAX_BASE_TWO_OCTET		65535

#define 	TPDU_AP_8_PORT_MAX_VALUE	TPDU_MAX_BASE_ONE_OCTET
#define 	TPDU_AP_16_PORT_MAX_VALUE	TPDU_MAX_BASE_TWO_OCTET
#define 	TPDU_CR_8BIT_MAX_VALUE		TPDU_MAX_BASE_ONE_OCTET
#define 	TPDU_CR_16BIT_MAX_VALUE		TPDU_MAX_BASE_TWO_OCTET
#define 	TPDU_CM_MAX_VALUE			TPDU_MAX_BASE_ONE_OCTET
#define 	TPDU_CS_MAX_VALUE			TPDU_MAX_BASE_ONE_OCTET

/* Application Port Type */
typedef enum {
	TPDU_AP_TYPE_NONE 	= 0x00,
	TPDU_AP_TYPE_8BIT 	= 0x08,
	TPDU_AP_TYPE_16BIT 	= 0x10
} eTPDU_AP_Type;

/* MAX_SIZE */
/* Concatenated Short Message */
typedef enum {
	TPDU_CR_TYPE_NONE    = 0x00,
	TPDU_CR_TYPE_8BIT    = 0x08,
	TPDU_CR_TYPE_16BIT   = 0x10
} eTPDU_CR_Type;


/*----------------------------------------------------------------------------*
  -
  -     TPDU UserData 7to8bit or 8to7 bit encoding and decoding
  -     if return value is 0 then fail, else return value is en(de)coding size
  -
 *----------------------------------------------------------------------------*/
int     enc8to7bits(unsigned char *data,unsigned char *buff,int datasize);
int     dec7to8bits(unsigned char *data,unsigned char *buff,int datasize);
/*2006-01-03 add */
int     enc8to7bitsUDH(unsigned char *data,unsigned char *buff,int datasize);
int     dec7to8bitsUDH(unsigned char *data,unsigned char *buff,int datasize);
/*2006-01-03 add */
char	*printBitString(unsigned char value, int position, int length);
#endif

