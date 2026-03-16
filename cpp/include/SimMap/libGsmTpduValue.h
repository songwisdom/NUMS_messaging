/**
 * =====================================================================================
 *
 *       @file  libGsmTpduValue.h
 *
 *    	 @brief 
 *
 *       Version:  1.0
 *       Compiler:  cc (HP-UX)
 *       @date:  2009년 05월 15일 01시 10분 04초
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


#ifndef _H_LIB_GsmTpduValue_
#define _H_LIB_GsmTpduValue_
#include "libMapComm.h"

/****************************************************************************/
/*																			*/
/* 	Definition of Parameter Value 											*/
/*																			*/
/****************************************************************************/

/*--------------------------------------------------------------------------*
 * 	TP-MTI value define 													*
 *--------------------------------------------------------------------------*/
typedef enum {
	TPDU_SMS_DELIVER =		0,	/* SMS-DELIVER - Phase 1,2 3 */
	TPDU_SMS_DELIVER_REPORT=0,	/* SMS-DELIVER-REPORT - Phase 2 and 3 */
	TPDU_SMS_SUBMIT	= 		1,	/* SMS-SUBMIT - Phase 1,2,3 */
	TPDU_SMS_SUBMIT_REPORT=	1,	/* SMS-SUBMIT-REPORT - Phase 2 and 3 */
	TPDU_SMS_STATUS_REPORT=	2,	/* SMS-STATUS_REPORT - Phase 2 and 3 */
	TPDU_SMS_COMMAND	  =	2,	/* SMS-COMMAND - Phase 2 and 3 */
	TPDU_SMS_RESERVED	  =	3
} eTPMTIFieldValue;

/*--------------------------------------------------------------------------*
 * 	TP-MMS value define 													*
 *--------------------------------------------------------------------------*/
typedef enum{
	TPDU_MM_WAIT	= 0,	/* More Messages are waiting for the MS in this SC */
	TPDU_NO_MM_WAIT 	/* No More Message are waiting for the MS int this SC */
} eTPMMSFieldValue;

/*--------------------------------------------------------------------------*
 * 	TP-RP value define 														*
 *--------------------------------------------------------------------------*/
typedef enum{
	TPDU_NOT_RPSET	= 0,	/* TP-Reply-Path parameter is not set */
	TPDU_RPSET = 1			/* TP-Reply-Path parameter is set  */
} eTPRPFieldValue;

/*--------------------------------------------------------------------------*
 * 	TP-RD value define 														*
 *--------------------------------------------------------------------------*/
typedef enum{
	SC_TO_ACCEPT	= 0,	/* Instruct the SC to accept an SMS-SUBMIT */
	SC_TO_REJECT 			/* Instruct the SC to reject an SMS-SUBMIT */
} eTPRDFieldValue;

/*--------------------------------------------------------------------------*
 * 	TP-SRR value define 														*
 *--------------------------------------------------------------------------*/
typedef enum{
	STATUS_REPORT_NOT_REQUEST=0,/* A status report is not requested */
	STATUS_REPORT_REQUEST	/* A status report is requested */
} eTPSRRFieldValue;

/*--------------------------------------------------------------------------*
 * 	TP-SRI value define 													*
 *--------------------------------------------------------------------------*/
typedef enum{
	TPDU_NOT_RETURN_SME	= 0, /* A status report shall not be returned to the SME*/
	TPDU_RETURN_SME		 /* A status report shall be a returned to the SME */
} eTPSRIFieldValue;

/*--------------------------------------------------------------------------*
 * 	TP-SRQ value define 													*
 *--------------------------------------------------------------------------*/
typedef enum{
	TPDU_RESULT_SUBMIT=0, /* The SMS-STATUS-REPORT is the result of a SMS-SUBMIT*/
	TPDU_RESULT_COMMAND	/* The SMS-STATUS-REPORT is the result of an SMS-COMMAND*/
} eTPSRQFieldValue;

/*--------------------------------------------------------------------------*
 * 	TP-CT value define 													*
 *--------------------------------------------------------------------------*/
typedef enum{
	TPDU_CT_ENQUIRY=0, /* The SMS-STATUS-REPORT is the result of a SMS-SUBMIT*/
	TPDU_CT_CANCEL,	/* The SMS-STATUS-REPORT is the result of an SMS-COMMAND*/
	TPDU_CT_DELETE,	/* delete previously submitted short message */
	TPDU_CT_ENABLE	/* enable status report request relating to previously
						submitted short message */
		/* reserved 4 - 31 */
		/* values specific for each SC 112 - 127 */
} eTPCTFieldValue;

/*--------------------------------------------------------------------------*
 * 	TP-UDHI value define 													*
 *--------------------------------------------------------------------------*/
typedef enum{
	TPDU_ONLY_SHORT_MSG	= 0,	/* contains only the short message */
	TPDU_INC_HEADER	 /* contains a Header in addition to the short message  */
} eTPUDHIFieldValue;

#define	BIT8_UD_MAX_SIZE			140
#define	BIT7_UD_MAX_SIZE			160

/*--------------------------------------------------------------------------*
 * 	TP-VPF value define 													*
 *--------------------------------------------------------------------------*/
typedef enum{
	TPDU_NOT_PRESENT = 		0,	/* TP-VP field not present */
	TPDU_PRESENT_INTEGER ,		/* TP VP field present - relative format */
	TPDU_SPARE ,				/* TP-VP field present - enhanced format */
	TPDU_PRESENT_SEMI_OCTET		/* TP-VP filed present - absolute format */
}eTPVPFFieldValue;

/*--------------------------------------------------------------------------*
 * 	TP-VP(Enhanced format) value define										*
 *--------------------------------------------------------------------------*/
typedef enum{
	TPDU_NO_EXTENTION = 0,	/* indicates that there are no more TP-VP functinallity indicator extension octets to follow */
	TPDU_EXTENTION			/* indicator is to be extended to another octet */
}eTPVPFieldValueExtention;

typedef enum{
	TPDU_NO_SINGLE_SHORT_SM = 0,
	TPDU_SINGLE_SHORT_SM 	/* SC is required to make up to one delivery attempt */
}eTPVPFieldValueSingle;
	
typedef enum{
	TPDU_NO_VP_SPECIFIED = 0,/* No validity Period specified */
	TPDU_VP_RELATIVE_CASE,	/* VP is as specified for the relative case */
	TPDU_VP_RELATIVE_INTEGER,/* VP is relative in integer representaition */
	TPDU_VP_RELATIVE_SEMIOCTET,/*VP is relative in semi-octet representation */
}eTPVPFieldValueVPF;

/*--------------------------------------------------------------------------*
 * Address Field				 											*
 *--------------------------------------------------------------------------*/
/* Address Field - Type of Number 											*/
typedef enum{
	TPDU_AFT_UNKNOWN		=	0,	/* Unknown */
	TPDU_AFT_INTERNATIONAL,			/* International Number */
	TPDU_AFT_NATIONAL,				/* National Number */
	TPDU_AFT_NETWORK_SPECIFIC,		/* Network Specific Number */
	TPDU_AFT_SUBSCRIBER,			/* Subscriber Number */
	TPDU_AFT_ALPHANUMERIC,			/* Alphanumeric */
	TPDU_AFT_ABBREVIATED			/* Abbreviated number */
}eTPAddrFieldTypeValue;

/* Address Field - Numbering Plan Identification 							*/
typedef enum{
	TPDU_AFN_UNKOWN			= 0,	/* Unknown */
	TPDU_AFN_ISDN,					/* ISDN/Telephone Numbering Plan */
	TPDU_AFN_DATA = 3,					/* Data Numbering Plan */
	TPDU_AFN_TELEX,					/* Telex Numbering Plan */
	TPDU_AFN_SC_SPECIFIC,			/* Service Centre Specific Plan */
	TPDU_AFN_SC_SPECIFIC2,			/* Service Centre Specific Plan */
	TPDU_AFN_NATIONAL = 8,				/* Naitional numbering plan */
	TPDU_AFN_PRIVATE,				/* Private numbering plan */
	TPDU_AFN_ERMES					/* ERMES numbering plan */
}eTPAddrFieldNPIValue;

/*--------------------------------------------------------------------------*
 * TP-PID Value definition				 	 								*
 *--------------------------------------------------------------------------*/
/* bit 6 and 7 	*/
typedef enum{
	TPDU_ASSIGN_00 = 0,		/* include indicates telematic interworking */
	TPDU_ASSIGN_01,			/* not include indicates telematic interworking */
	TPDU_ASSIGN_SC_SPECIFIC=3 /* for SC specific use */
}eTPPIDFieldValue;

/* bit 5th	*/
typedef enum{
	TPDU_NO_INTERWORKING = 0,	/* no interworking, but SME-to-SME protocol */
	TPDU_TELEMATIC_INTERWORKING	/* telematic interworking */
}eTPPIDFieldValue5th;

/*bit 0-4, bit 7 and 8 is 0	*/
typedef enum{
	TPDU_IMPLICIT	= 0,	/* implicit -device type is specfic to the SC
							   or can be conclued on the basis of the address */
	TPDU_TELEX,				/* telex(or teletex reduced to telex format */
	TPDU_GROUP3_TELEFAX,	/* group 3 telefax */
	TPDU_GROUP4_TELEFAX,	/* group 4 telefax */
	TPDU_VOICE_TELEPHONE,	/* voice telephone */
	TPDU_ERMES,				/* European Radio Messaging System */
	TPDU_NPS,				/* National Paging System(known to the SC) */	
	TPDU_VIDEOTEX,			/* Videotex(T.100/T.101) */
	TPDU_TELETEX_CARRIER_UNSPEC,/* teletex,carrier unspecified */
	TPDU_TELETEX_PSPDN,		/* teletex, in PSPDN */
	TPDU_TELETEX_CSPDN,		/* teletex, in CSPDN */
	TPDU_TELETEX_ANALOG_PSTN,	/* teletex, in analog PSTN */
	TPDU_TELETEX_DIGITAL_ISDN,	/* teletex, in digital ISDN */
	TPDU_UCI,				/* Universal Computer Interface */
	TPDU_MHF = 		16,			/* Message Handling Facility(known to the SC) */
	TPDU_PUBLIC_X400,		/* any public x.400-based message handling system*/
	TPDU_IEM,				/* Internet Electronic Mail */
	TPDU_GSM_UMTS = 0x1F	/* Decimal 31,A GSM/UMTS mobile station*/ 
}eTPPIDFieldValue00;

/* if bit 7 and 8 value is 1 then bit 0-5 value is following */
typedef enum{
	TPDU_SMT = 0,			/* Short Message Type */
	TPDU_REPLACE_SMT1,		/* Replace Short Message Type 1 */
	TPDU_REPLACE_SMT2,		/* Replace Short Message Type 2 */
	TPDU_REPLACE_SMT3,		/* Replace Short Message Type 3 */
	TPDU_REPLACE_SMT4,		/* Replace Short Message Type 4 */
	TPDU_REPLACE_SMT5,		/* Replace Short Message Type 5 */
	TPDU_REPLACE_SMT6,		/* Replace Short Message Type 6 */
	TPDU_REPLACE_SMT7,		/* Replace Short Message Type 7 */
	TPDU_EMS = 0x1E,		/* Enhanced Message Service (Obsolete),Decimal 30 */
	TPDU_RCM,				/* Return Call Message */
	TPDU_ANSI136_RDATA=0x3A,/* ANSI-124 R-DATA, Decimal 58 */
	TPDU_ME_DATA_DOWNLOAD,	/* ME Data download */
	TPDU_ME_DE_PERSON_SM,	/* ME De-personalization Short Message */
	TPDU_SIM_DATA_DOWNLOAD	/* (U)SIM Data download */
}eTPPIDFieldValue01;

/*--------------------------------------------------------------------------*
 * TP-DCS Value definition													*
 *--------------------------------------------------------------------------*/
/* Value definition of GROUP, bit 7 and 4 	*/
typedef enum{
		/* General Data Coding indication Group, 0 - 3 */
	TPDU_GDC_UNCOMP_NOT_USED_CLASS = 0,/* text is uncompressed and no message class 0and 1bit not used*/	
	TPDU_GDC_UNCOMP_USED_CLASS,		/* text is uncompressed and message class(0and 1bit)meaning */
	TPDU_GDC_COMP_NOT_USED_CLASS,	/* text is compressed and no message class(0and 1bit not used) */
	TPDU_GDC_COM_USED_CLASS,		/* test is compressed and message class( 0 and 1 bit ) meaning */
		/* Message Marked for Automatic Deleteion Group, 4 - 7*/
	TPDU_MMADG_UNCOMP_NOT_USED_CLASS ,/* text is uncompressed and no message class 0and 1bit not used */
	TPDU_MMADG_UNCOMP_USED_CLASS,	/* text is uncompressed and message class(0and 1bit)meaning */
    TPDU_MMADG_COMP_NOT_USED_CLASS,	/* text is compressed and no message class(0and 1bit not used) */
    TPDU_MMADG_COM_USED_CLASS,		/* test is compressed and message class( 0 and 1 bit ) meaning */
	/* Reserved Group 8 - 11 */
	TPDU_RESERVED_8_CLASS 	= 8,	/* SKT define (KSC5601, ASCII 8bit) */
	TPDU_MWI_DISCARD 		= 12,	/* Message Waiting Indication Group, Discard Message */ 
	TPDU_MWI_STORE,					/* Message Waiting Indication Group, Store Message */
	TPDU_MWI_STORE_UCS2,			/* Message Waiting Indication Group, Store Message */
	TPDU_DCM_CLASS					/* Data Coding Message Class */
}eTPDCSFieldValueGroup;	

/* Genral Data Coding indication Group and Message Marked for 
	Atuiomatic Deletion Group,bit 0-1 */
/* Data Coding/Message Class Group */
/* class definition */ 

typedef enum{
	TPDU_CLASS_0 = 0,
	TPDU_CLASS_1,					/* default meaning: ME-specific */
	TPDU_CLASS_2,					/* (U)SIM-specific message */
	TPDU_CLASS_3					/* default meaning: TE specific (see 3GPP TS 27.005 [8]) */	
}eTPDDCSFieldValueClass;

/* characterset definition */
typedef enum{
	TPDU_GSM7BIT_DEFAULT = 0,	/* GSM 7 bit defualt alphabet */
	TPDU_8BIT_DATA,				/* 8 bit data */
	TPDU_UCS2,					/* UCS2 (16bit)[10] ,
									Not Used Data Coding/message class Group */
}eTPDCSFieldValueCharSet;

/* Message Wating Indication Group */
/* bit 3 */
typedef enum{
	TPDU_INDICATION_INACT = 0, 	/*Set Indication Inactive*/
	TPDU_INDICATION_ACT			/*Set Indication Active */
}eTPDCSFieldValueSense;

/* bit 2 is reserved */
/* bit 0 and 1 indication type */
typedef enum{
	TPDU_INDICATION_VMW = 0,	/* Voicemail Message Waiting */
	TPDU_INDICATION_FMW,		/* Fax Message Waiting */
	TPDU_INDICATION_EMMW,		/* Electronic Mail Message Waiting */
	TPDU_INDICATION_OMW			/* Other Message Waiting */
}eTPDCSFieldValueIDCType;

/*--------------------------------------------------------------------------*
 * TP-FCS Value definition													*
 *--------------------------------------------------------------------------*/
typedef enum{
		/* 0x00 - 0x7F reserved */
		/* 0x80 - 0x8F TP-PID Errors */
	TPDU_TELEINTERWORKING_NOT_SUPPORT = 0x80,/* Telematic interworking 
												not supported*/
	TPDU_SHORT_MSG_TYPE_0_NOT_SUPPORT,	/* short message type 0 not supported*/
	TPDU_CANT_REPLACE_SHORT_MSG,		/* can't replace short message */
		/* 0x83 - 0x8E reserved */
	TPDU_UNSPECIFIED_TPPID_ERROR = 0x8F,/* unspecified TP-PID error */
		/* 0x90 - 0x9F TP-DCS Errors*/
	TPDU_DATA_CODING_SCHEME_NOT_SUPPORT,/* Data coding scheme(alphabet) 
											not supported*/
	TPDU_MSG_CLASS_NOT_SUPPORT,			/* message class not supported */
		/* 0x92 - 0x9E reserved */
	TPDU_UNSPECIFIED_TPDCS_ERROR = 0x9F,/* unspecified TP-DCS Error */
		/* 0xA0 - 0x-AF TP-Command Errors */
	TPDU_COMMAND_CANNOT_BE_ACTION,		/* command cannot be actioned */
	TPDU_COMMAND_UNSUPPORT,				/* command unsupported */
		/* 0xA2 - 0xAE reserved */
	TPDU_UNSPECIFIED_TPCOMMAND_ERROR = 0xAF,/*unspecified TP-Command Error */
	TPDU_NOT_SUPPORTED,					/* TPDU not supported */
		/* 0xB1 - 0xBF reserved */
	TPDU_SC_BUSY = 0xC0,				/* SC Busy */
	TPDU_NO_SC_SUBSCRIPTION,			/* No SC Subsription */
	TPDU_SC_SYSTEM_FAILURE,				/* SC System failure */
	TPDU_INVALID_SME_ADDRESS,			/* invalid SME Address */
	TPDU_DEST_SME_BARRED,				/* destination SME barred */
	TPDU_SM_REJECT_DUPLICATE_SM,		/* SM Recjected-Duplicate SM */
	TPDU_TP_VPF_NOT_SUPPORT,			/* TP-VPF not supported */
	TPDU_TP_VP_NOT_SUPPORT,				/* TP-VP not supported */
		/* 0xchar - 0xCF reserved */
	TPDU_SIM_SMS_STORAGE_FULL = 0xD0,	/* (U)SIM SMS Storage full */
	TPDU_NO_SMS_STORAGE_CAPABILITY,		/* no SMS storage capablility in (U)SIM */
	TPDU_ERROR_IN_MS,					/* Error in MS */
	TPDU_MEMORY_CAPACITY_EXCEEDED,		/* Memroy Capacity Exceeded */
	TPDU_SIM_APP_TOOLKIT_BUSY,			/* (U)SIM Application Toolkit Busy */
	TPDU_SIM_DATA_DOWNLOAD_ERROR,		/* (U)SIM data download error  */
		/* 0xD6 - 0xDF reserved */
		/* 0xE0 - 0xFE values specific to an application */
	TPDU_UNSPECIFIED_ERROR_CAUSE = 0xFF,/* Unspecified error cause */
}eTPFCSFieldValue;

/*--------------------------------------------------------------------------*
 * TP-ST Value definition													*
 *--------------------------------------------------------------------------*/
typedef enum{
		/* Short Message transaction completed */
	TPDU_SM_RECEVIDE_BY_SME = 0,	/* short message received by the SME */
	TPDU_SM_FORWARD_SC_TO_SME,		/* short message forwarded by the SC 
										to the SME but the SC is unable to 
										confirm delivery */
	TPDU_SM_REPLACE_BY_SC,			/* short message replaced by teh SC */
		/* reserved 3 - 15 */
		/* values specific to each SC 16 - 31 */
		/* Temporary Error, SC still trying to transfer SM */
	TPDU_TERR_TRY_CONGESTION = 32,/* Congestion */
	TPDU_TERR_TRY_SME_BUSY,		/* SME Busy */
	TPDU_TERR_TRY_NO_RESPONSE,	/* No response from SME */
	TPDU_TERR_TRY_SERVICE_REJECT,/* Service rejected */
	TPDU_TERR_TRY_TQOS_NOT_AVAIL,/* quality of service not available */
	TPDU_TERR_TRY_IN_SME,		/* Error in SME */
		/* reserved 38 - 47 */
		/* values specific to each SC 48 - 63 */
		/* Permanent Error, SC is not making any more transfer attempts */
	TPDU_PERR_REMOTE_PROCEDURE = 0x40,/*Remote procedure error (decimal 64)*/
	TPDU_PERR_INCOMPATIBLE_DESTINATION,/* Incompatible destination */
	TPDU_PERR_CONNECTION_REJECT,/* connection rejected by SME */
	TPDU_PERR_NOT_OBTAINABLE,	/* Not obtainable */
	TPDU_PERR_QOS_NOT_AVAIL,	/* Qos not available */
	TPDU_PERR_NO_INTERWORKIN_AVAIL,	/* No Interworking available */
	TPDU_SM_VPE,				/* SM Validity Period Expired */
	TPDU_SM_DELETE_ORG_SME,		/* SM Deleted by originating SME */
	TPDU_SM_DELETE_SC_ADMIN,	/* SM Deleted by SC administration */
	TPDU_SM_NOT_EXIST,			/* SM does not exist */
		/* reserved 74 - 79 */
		/* values specific to each SC 80 - 95 */
		/* Temporary Error, SC is not making any more transfer attempt */
	TPDU_TERR_ANY_CONGESTION = 96,/* Congestion */	
	TPDU_TERR_ANY_SME_BUSY,		/* SME Busy */
	TPDU_TERR_ANY_NO_RESPONSE,	/* No response from SME */
	TPDU_TERR_ANY_SERVICE_REJECT,/* Service rejected */
	TPDU_TERR_ANY_TQOS_NOT_AVAIL,/* quality of service not available */
	TPDU_TERR_ANY_IN_SME		/* Error in SME */
		/* reserved 102 - 111 */
		/* values specific to each SC 112 - 127 */
		/* reserved 128 ...  */
}eTPSTFieldValue;

#endif

