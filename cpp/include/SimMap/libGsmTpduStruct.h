
#ifndef _H_LIB_GsmTpduStruct_
#define _H_LIB_GsmTpduStruct_

/*
static char *GsmTpduVerInfo="This Library is GSM TPDU encoding and decoding\nWritten By Ahn Yeob\t2005-08-02\tVer 1.0.0\n";

#define	GetVersionInfo()	(GsmTpduVerInfo)
*/
#include "flk_type.h"
/* Parameter value define */
#include <libGsmTpduValue.h>
/* Parameter structure define */
#include <libGsmTpduParam.h>


#define		NOT_INC_ERROR_TPDU			0
#define		INC_ERROR_TPDU				1
/****************************************************************************/
/* 			TPDU SMS-DELIVER  STRUCTURE										*/
/****************************************************************************/
typedef struct {
		/* Mandatory used tpMTI.value = value or SetMTIValue(pdu)*/
	TP_MTIParam			tpMTI;
	TP_MMSParam			tpMMS;
	TP_RPParam			tpRP;
	TP_OAParam			tpADDR;			/* TP-OA Parameter, 2 ~ 12 bytes */
	TP_OAParam			tpCallbackADDR;	/* TP-CallBack Parameter, 2 ~ 12 bytes */
	TP_OAParam			tpADDRc;		/* TP-OA Parameter, 2 ~ 12 bytes */
	TP_OAParam			tpADDRk;		/* TP-OA Parameter, 2 ~ 12 bytes */
	TP_PIDParam			tpPID;			/* TP-PID Parameter, 1 byte */
	TP_DCSParam			tpDCS;			/* TP-DCS Parameter, 1byte */
	TP_SCTSParam		tpSCTS;			/* TP-SCTS Parameter, 7bytes */
	TP_UDLParam			tpUDL;			/* TP-UDL Parameter UserDataLength, 1bytes */
		/* Optional */
	TP_SRIParam			tpSRI;			/* request of stauts report */
	TP_UDHIParam		tpUDHI;
	TP_UDParam			tpUD;			/* TP-UD Parameter, max 140 bytes */
		/* optional parameter check */
	struct{
		// TODO linux porting
		unsigned char		notuse	:	5;
		unsigned char		UD		:	1;
		unsigned char		UDHI	:	1;
		unsigned char		SRI		:	1;
	}optional;

		/* etc information */
	TP_ParamIDC				idc;			 	// 10byte
	int						tpPduSize;		 	// 4byte
	unsigned short 			tpIEI_Aport;    	// 2byte
	unsigned short 			tpIEI_OrigAport;   	// 2byte
	unsigned short 			tpIEI_AportType;	// 2byte
	/* conncatenated */
	unsigned short			tpIEI_Ref_Num;	 	// 2byte
	unsigned char			tpIEI_Max_Num;	  	// 1byte
	unsigned char			tpIEI_Seq_Num;	  	// 1byte
	unsigned char 			tpIEI_ConcatType; 	// 1byte
	unsigned char			tpIEI_MtMsType;	// 1byte
	unsigned char			tpIEI_Special[TPDU_SPECIAL_SMS_SIZE];		// 2byte
	unsigned short			tpIEI_Hyper_Position;	// 2byte
	unsigned char			tpIEI_Hyper_Title_Len;	// 1byte
	unsigned char			tpIEI_Hyper_URL_Len;	// 1byte
	unsigned char			ucMsgFlowType;		// 1byte
	unsigned char			ucReserved[3];		// 3byte
}TPSMSDeliver;
/*---------------------------------------------------------------------------
 *	MACRO
 *--------------------------------------------------------------------------*/
#define		SetMTIParameter(pdu,val)		(pdu.tpMTI.value = pdu.idc.mti.value = val)
#define		SetMMSParameter(pdu,val)		(pdu.tpMMS.value = pdu.idc.mms.value =val)
#define		SetRPParameter(pdu,val)		(pdu.tpRP.value = pdu.idc.rp.value =val)
#define		SetSRIParameter(pdu,val)		(pdu.tpSRI.value =pdu.idc.sri.value = val)
#define		SetUDHIParameter(pdu,val)		(pdu.tpUDHI.value = pdu.idc.udhi.value =val)

#define		SetAddrLen(pdu,val)			(pdu.tpADDR.addrLen = val)
#define		SetAddrTON(pdu,val)			(pdu.tpADDR.pAddrType = val)
#define		SetAddrNPI(pdu,val)			(pdu.tpADDR.pAddrNumPlan = val)
#define		SetAddrNumber(pdu,val,size)   (memcpy(pdu.tpADDR.address,val,size)) 

#define		SetPIDParameter(pdu,val)		(pdu.tpPID.value = val)
#define		SetDCSParameter(pdu,val)		(pdu.tpDCS.value = val)
#define		SetOrgDCSParameter(pdu,val)		(pdu.tpOrgDCS.value = val)
#define 	SetSCTSParameter(pdu,val)		(memcpy(pdu.tpSCTS.value,val,7))

#define		SetUDParameter(pdu,val,size)	(memcpy(pdu.tpUD.data,val,size))	
#define		SetUDSize(pdu,val)				(pdu.tpUD.size = pdu.tpUDL=val)
#define		SetUDSize2(pdu,val)				(pdu.tpUD.size = val)

#define		GetMTIValue(pdu)			(pdu.tpMTI.value)
#define		GetMMSValue(pdu)			(pdu.tpMMS.value)
#define		GetRPValue(pdu)				(pdu.tpRP.value)
#define		GetSRIValue(pdu)			(pdu.tpSRI.value)
#define		GetUDHIValue(pdu)			(pdu.tpUDHI.value)

#define		GetADDRLen(pdu)				(pdu.tpADDR.addrLen)
#define		GetADDRLen2(pdu)				(pdu.tpADDRc.addrLen)
#define		GetADDRTON(pdu)				(pdu.tpADDR.pAddrType)
#define		GetADDRTON2(pdu)			(pdu.tpADDR.pAddrType&7)
#define		GetADDRNPI(pdu)				(pdu.tpADDR.pAddrNumPlan)
#define		GetADDRNumber(pdu)			(pdu.tpADDR.address)
#define		IsNationalAddress(pdu)		((pdu.tpADDR.address[0].digit&0xf0)==0x00 ? 1 : 0)
#define		GetADDRSize(pdu)			(pdu.tpADDR.addrOctetSize)

#define		GetADDRSize2(pdu)			(pdu.tpADDRc.addrOctetSize)
#define		GetADDRNumber2(pdu)			(pdu.tpADDRc.address)
#define     GetADDRTONC(pdu)            (pdu.tpADDRc.pAddrType&7)
#define     GetADDRNPIC(pdu)             (pdu.tpADDRc.pAddrNumPlan)

#define		GetPIDValue(pdu)			(pdu.tpPID.value)
#define		GetDCSValue(pdu)			(pdu.tpDCS.value)

#define 	GetSCTSValue(pdu)			(pdu.tpSCTS.value)
#define 	GetSCTSTimezone(pdu)		(pdu.tpSCTS.value[6].digit)

#define		GetUDLValue(pdu)			(pdu.tpUDL)
#define		GetUDValue(pdu)				(pdu.tpUD.data)
#define		GetUDSize(pdu)				(pdu.tpUD.size)

/* Callback Set */
#define		SetAddrCBLen(pdu,val)			(pdu.tpCallbackADDR.addrLen = val)
#define		SetAddrCBSize(pdu,val)			(pdu.tpCallbackADDR.addrOctetSize = val)
#define		SetAddrCBTON(pdu,val)			(pdu.tpCallbackADDR.pAddrType = val)
#define		SetAddrCBNPI(pdu,val)			(pdu.tpCallbackADDR.pAddrNumPlan = val)
#define		SetAddrCBNumber(pdu,val,size)   (memcpy(pdu.tpCallbackADDR.address,val,size)) 
/* Callback Get */
#define		GetADDRCBLen(pdu)			(pdu.tpCallbackADDR.addrLen)
#define		GetADDRCBTON(pdu)			(pdu.tpCallbackADDR.pAddrType)
#define		GetADDRCBTON2(pdu)			(pdu.tpADDR.pAddrType&7)
#define		GetADDRNPI(pdu)				(pdu.tpADDR.pAddrNumPlan)
#define		GetADDRCBNPI(pdu)			(pdu.tpCallbackADDR.pAddrNumPlan)
#define		GetADDRCBNumber(pdu)		(pdu.tpCallbackADDR.address)
#define		GetADDRCBSize(pdu)			(pdu.tpCallbackADDR.addrOctetSize)

#define		IsIncCharsetDcs(dcs)		((dcs.dcsGroup > 7) && (dcs.dcsGroup < 15) ? 0 : 1)
/****************************************************************************/
/* 			TPDU SMS-DELIVER-REPORT  STRUCTURE								*/
/****************************************************************************/
typedef struct{
			/* 	--- Mandatory ---  */
	TP_MTIParam				tpMTI;
	TP_FCSParam				tpFCS;			/* if not error then not used, 
											   if this parameter not exist and 
											   version is phase1 or phase2 
											   then format error */
	 		/*	--- Optional ---  */
	TP_UDHIParam			tpUDHI;
			/* only phase2++ */
	TP_PIDParam				tpPID;			/* TP-PID Parameter, 1byte, optional */
	TP_DCSParam				tpDCS;			/* TP-DCS Parameter, 1byte, optional */
	TP_UDLParam				tpUDL;			/* TP-UDL Parameter, 1byte, optional */
	TP_UDParam				tpUD;			/* TP-UD Parameter, 0-158 bytes, optional */
	TP_PIParam				tpPI;			/* if phase2++ is mandatory, else not used*/
			/* optional information */
	struct{
		unsigned char		UDHI	:	1;
		unsigned char		PID		:	1;
		unsigned char		DCS		:	1;
		unsigned char		UDL		:	1;
		unsigned char		FCS		:	1;	/* if error mandantry else not used */
		unsigned char		PI		:	1;
		unsigned char		notuse 	:	2;
	}optional;

	/* etc */
	TP_ParamIDC				idc;
	int						tpPduSize;
}TPSMSDeliverReport;
/*---------------------------------------------------------------------------
 *	MACRO
 *--------------------------------------------------------------------------*/
#define	SetPIParameter(pdu,val)			(pdu.tpPI.value = val)
#define	SetFCSParameter(pdu,val)		(pdu.tpFCS = val)

#define	GetPIValue(pdu)					(pdu.tpPI.value)
//#define	GetFCSValue(pdu)				(pdu.tpFCS.value)
/****************************************************************************/
/* 			TPDU SMS-SUBMIT  STRUCTURE										*/
/****************************************************************************/
typedef struct{
		/* Mandaotry : used tpMTI.value = value */
	TP_MTIParam			tpMTI;		// 1 byte
	TP_RDParam			tpRD;		// 1 byte
	TP_VPFParam			tpVPF;		// 1 byte
	TP_RPParam			tpRP;		// 1 byte
	TP_MRParam			tpMR;		// 1 byte		/* used TP-MNParam of Status Report */
	TP_DAParam			tpADDR;		// 18 byte
	TP_OAParam    		tpCallbackADDR;		// 18 byte 
	TP_PIDParam			tpPID;		// 4 byte
	TP_DCSParam			tpDCS;		// 6 byte
	TP_UDLParam			tpUDL;		// 1 byte		/* TP-UDL Parameter, 1 byte */
		/* ----- Optional ----- */
	TP_SRRParam			tpSRR;		// 1 byte		/* Request Status Report - 1 */
	TP_UDHIParam		tpUDHI;		// 1 byte
	TP_VPParam			tpVP;		// 5 byte 		/* TP-VP Parameter, 1 or 7 bytes,optional */
	TP_UDParam			tpUD;		// 164 byte		/* TP-UD Parameter, 0-158 bytes, optional */
	/* etc */
	struct{
		unsigned char		SRR	:	1;
		unsigned char		UDHI:	1;
		unsigned char		PID	:	1;
		unsigned char		DCS	:	1;
		unsigned char		VP	:	1;
		unsigned char		UDL	:	1;
		unsigned char		notuse	:	2;
	}optional;									// 1 byte

	TP_ParamIDC				idc;				// 10 byte
	int						tpPduSize;			// 4 byte
	unsigned short			tpIEI_Aport;		// 2 byte
	unsigned char			tpIEI_AportType;	// 1 byte
	/* conncatenated */
	unsigned short			tpIEI_Ref_Num;	  	// 2byte
	unsigned char			tpIEI_Max_Num;	  	// 1byte
	unsigned char			tpIEI_Seq_Num;	  	// 1byte
	unsigned char 			tpIEI_ConcatType; 	// 1byte
	TP_DCSParam				tpOrgDCS;			// 6 byte
	unsigned char			tpReserved[3];	  	// 3byte
}TPSMSSubmit;
/*---------------------------------------------------------------------------
 *	MACRO
 *--------------------------------------------------------------------------*/
#define	SetVPFParameter(pdu,val)			(pdu.tpVPF.value = pdu.idc.vpf.value= val)
#define	SetRDParameter(pdu,rdval)			(pdu.tpRD.value = pdu.idc.rd.value=rdval)
#define	SetMRParameter(pdu,val)				(pdu.tpMR = val)
#define	SetVPRelative(pdu,val)				(pdu.tpVP.vpRelative = val)
#define	SetVPAbsolute(pdu,time)				(memcpy(pdu.tpVP.vpAbsolute.value,time,7))
#define	SetVPEnhancedExten(pdu,val)			(pdu.tpVP.vpEnExtention = val)
#define	SetVPEnhancedSigSM(pdu,val)			(pdu.tpVP.vpEnSingleShortSM = val)
#define SetVPEnhancedFormat(pdu,val)		(pdu.tpVP.vpEnFormat = val)
#define	SetVPEnhancedValue(pdu,val)			(memcpy(pdu.tpVP.vpEnValue,val,6))
#define	SetVPParameter(pdu,val,size)		(memcpy(pdu.tpVP.vpValue,val,size))

#define	GetVPFValue(pdu)					(pdu.tpVPF.value)
#define	GetRDValue(pdu)						(pdu.tpRD.value)
#define	GetFCSValue(pdu)					(pdu.tpFCS)
#define	GetMRValue(pdu)						(pdu.tpMR)
#define GetVPAbsolute(pdu)                  (pdu.tpVP.vpAbsolute.value)
#define GetVPAbsoluteTZ(pdu)                (pdu.tpVP.vpAbsolute.value[6].digit) 
/****************************************************************************/
/* 			TPDU SMS-SUBMIT-REPORT STRUCTURE								*/
/****************************************************************************/
typedef struct{
			/* 	--- Mandatory ---  */
	TP_MTIParam				tpMTI;
	TP_FCSParam				tpFCS;		/* if not error then not used */
	 		/*	--- Optional ---  */
	TP_UDHIParam			tpUDHI;
	TP_PIDParam				tpPID;			/* TP-PID Parameter, 1byte, optional */
	TP_PIParam				tpPI;			/* if phase2++ mandatory, else not used */
	TP_SCTSParam			tpSCTS;			/* if phase2++ mandatory, else not used */
		/* only phase2++ */
	TP_DCSParam				tpDCS;			/* TP-DCS Parameter, 1byte, optional */
	TP_UDLParam				tpUDL;			/* TP-UDL Parameter, 1byte, optional */
	TP_UDParam				tpUD;			/* TP-UD Parameter, 0-158 bytes, optional */
	/* etc */
	struct{
		unsigned char		UDHI	:	1;
		unsigned char		PID		:	1;
		unsigned char		DCS		:	1;
		unsigned char		UDL		:	1;
		unsigned char		FCS		:	1;
		unsigned char		PI		:	1;
		unsigned char		SCTS	:	1;
		unsigned char		notuse 	:	1;
	}optional;

	TP_ParamIDC				idc;
	int						tpPduSize;
}TPSMSSubmitReport;

/****************************************************************************/
/* 			TPDU SMS-STATUS-REPORT  STRUCTURE								*/
/****************************************************************************/
typedef struct{
			/* 	--- Mandatory ---  */
	TP_MTIParam				tpMTI;
	TP_MMSParam				tpMMS;
	TP_SRQParam				tpSRQ;			
	TP_MRParam				tpMR;			/* SMS-COMMAND TP-CommandType - 0(Enquery) ,
											   This value is TP-MN of SMS-COMMAND */
	TP_RAParam				tpADDR;
	TP_SCTSParam			tpSCTS;
	TP_DTParam				tpDT;
	TP_STParam				tpST;
	 		/*	--- Optional ---  */
	TP_UDHIParam			tpUDHI;
			/* only phase 2++ */
	TP_PIParam				tpPI;			/* if phase2++ optional,else not used */
	TP_PIDParam				tpPID;			/* TP-PID Parameter, 1byte, optional */
	TP_DCSParam				tpDCS;			/* TP-DCS Parameter, 1byte, optional */
	TP_UDLParam				tpUDL;			/* TP-UDL Parameter, 1byte, optional */
	TP_UDParam				tpUD;			/* TP-UD Parameter, 0-158 bytes, optional */
	struct{
		unsigned char		UDHI	:	1;
		unsigned char		PID		:	1;
		unsigned char		DCS		:	1;
		unsigned char		UDL		:	1;
		unsigned char		PI		:	1;
		unsigned char		notuse 	:	3;
	}optional;

	TP_ParamIDC             idc;
	int						tpPduSize;
}TPSMSStatusReport;
/*---------------------------------------------------------------------------
 *	MACRO
 *--------------------------------------------------------------------------*/
#define	SetSRQParameter(pdu,val)		(pdu.tpSRQ.value = pdu.idc.srq.value= val)
#define	SetSTParameter(pdu,val)			(pdu.tpST = val)
#define SetDTParameter(pdu,val)			(memcpy(pdu.tpDT.value,val,7))

#define	GetSRQValue(pdu)				(pdu.tpSRQ.value)
#define GetSTValue(pdu)					(pdu.tpST)

#define	GetDTValue(pdu)					(pdu.tpDT.value)
#define	GetDTTimezone(pdu)				(pdu.tpDT.value[6])
/****************************************************************************/
/* 			TPDU SMS-COMMAND  STRUCTURE										*/
/****************************************************************************/
typedef struct{
			/* 	--- Mandatory ---  */
	TP_MTIParam				tpMTI;
	TP_MRParam				tpMR;		/* used TP_MNParam of StatusReport */
	TP_PIDParam				tpPID;
	TP_CTParam				tpCT;
	TP_MNParam				tpMN;
	TP_DAParam				tpADDR;
	TP_CDLParam				tpUDL;
	 		/*	--- Optional ---  */
	TP_UDHIParam			tpUDHI;
	TP_SRRParam				tpSRR;		/* Requesting a status report */
	TP_CDParam				tpUD;
			/* --- used of optional ---- */
	struct{
		unsigned char		UDHI	:	1;
		unsigned char		SRR		:	1;
		unsigned char		CD		:	1;
		unsigned char		notuse	:	5;
	}optional;
			/* --- used of encoding and decoding */
	TP_ParamIDC             idc;
	int						tpPduSize;
}TPSMSCommand;

/****************************************************************************/
/*          TPDU UDH (User Data Header)  STRUCTURE                          */
/****************************************************************************/
typedef struct 
{
	unsigned char	*cpIEI_Tag;				// IEI tag buffer pointer
	unsigned char	*cpIEI_Length;			// IEI Length buffer pointer
	unsigned char	*cpIEI_Value;	 		// IEI Value buffer pointer 
	unsigned char  	ucIEI_Exist_Flag;	 	// UDH delete? or exist check flag   	
}IEI_Struct;

typedef struct {
	IEI_Struct 	stIEI[IEI_ST_MAX_SIZE];		
	int 		nIEI_Cnt; 					// IEI current total count
}UDH_Struct;

/****************************************************************************/
/*          Encoding UD (User Data)  STRUCTURE                              */
/****************************************************************************/
typedef struct
{
	unsigned char   data[256];
	int             nCharacter;
	int				nOctet;
}stUD_String;
typedef stUD_String stGSM7bitString;    // gsm 7bit
typedef stUD_String stASCIIString;      // ascii 8bit

/*---------------------------------------------------------------------------
 *	MACRO
 *--------------------------------------------------------------------------*/
#define	SetSRRParameter(pdu,val)		(pdu.tpSRR.value = pdu.idc.srr.value = val)
#define	SetCTParameter(pdu,val)		(pdu.tpCT = val)
#define	SetMNParameter(pdu,val)		(pdu.tpMN = val)

#define	GetSRRValue(pdu)				(pdu.tpSRR.value)
#define	GetMNValue(pdu)					(pdu.tpMN)
#define	GetSTValue(pdu)					(pdu.tpST)
#define	GetCTValue(pdu)					(pdu.tpCT)


int	decSMSDeliver(TPSMSDeliver *arg,unsigned char *data,int size);
int     decParamTPAddress1(unsigned char *data, TP_OAParam *Addr);
int     decParamTPAddress2(unsigned char *data, TP_OAParam *Addr);
int     decParamTPAddress3(unsigned char *data, TP_OAParam *Addr);
void	printSMSSubmitReport(TPSMSSubmitReport pdu);
char*	decParamBCDAddress(unsigned char *data,int cnt);
#endif
