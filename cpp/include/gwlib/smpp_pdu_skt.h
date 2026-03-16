#ifndef __SMPP_PDU_SKT_H__
#define __SMPP_PDU_SKT_H__

#include "gwlib.h"

// SKT SMPP EMS_CLASS 
#define	 ESM_CLASS_DEFAULT 							0x00
#define	 ESM_CLASS_DELIVER							0x01
#define	 ESM_CLASS_RECEIPT							0x04
#define	 ESM_CLASS_2G_KTF							0x06
#define	 ESM_CLASS_2G_LGT							0x09
#define	 ESM_CLASS_3G_KTF							0x0A
#define	 ESM_CLASS_3G_LGT							0x0B
#define	 ESM_CLASS_3G_KTF_CDMA_GLR					0x0C
#define	 ESM_CLASS_3G_KTF_GSM_GLR					0x0D
#define	 ESM_CLASS_3G_LGT_CDMA_GLR					0x0E
#define	 ESM_CLASS_3G_LGT_GSM_GLR					0x0F
#define	 ESM_CLASS_2G_KTF_CDMA_GLR					0x10
#define	 ESM_CLASS_2G_LGT_CDMA_GLR					0x11
#define	 ESM_CLASS_2G_KTF_GSM_GLR					0x12
#define	 ESM_CLASS_2G_LGT_GSM_GLR					0x13
#define	 ESM_CLASS_DEFAULT_AND_NOTI					0x14
#define	 ESM_CLASS_PORTED_AND_NOTI_TO_ESME			0x15
#define	 ESM_CLASS_PORTED_AND_NOTI_FROM_ESME		0x16
#define	 ESM_CLASS_2G_KTF_NOTI						0x17
#define	 ESM_CLASS_2G_LGT_NOTI						0x18
#define	 ESM_CLASS_3G_KTF_NOTI						0x19
#define	 ESM_CLASS_3G_LGT_NOTI						0x1A
#define	 ESM_CLASS_FROM_CDMA_GLR					0x24
#define	 ESM_CLASS_FROM_CDMA_GLR_AND_PORTED			0x25
#define	 ESM_CLASS_FROM_CDMA_GLR_AND_FORWARD		0x26
#define	 ESM_CLASS_FROM_DS_SMSC						0x28
#define	 ESM_CLASS_FROM_DS_SMSC_AND_PORTED			0x29
#define	 ESM_CLASS_FROM_GSM_GLR_AND_FORWARD			0x2A
#define	 ESM_CLASS_NOCHARGE							0x2C
#define	 ESM_CLASS_FORWARD_TO_ESME					0x30
#define	 ESM_CLASS_FORWARD_FROM_ESME				0x31
#define	 ESM_CLASS_FORWARD_RECEIPT					0x32
#define	 ESM_CLASS_FORWARD_AND_PORTED				0x34
#define	 ESM_CLASS_FORWARD_RECEIPT_DOUBLE			0x35
#define	 ESM_CLASS_PORTED							0x38
#define	 ESM_CLASS_NUMBER_PLUS_MO					0x40
#define	 ESM_CLASS_NUMBER_PLUS_MT					0x41
#define	 ESM_CLASS_FROM_CDMA_GLR_AND_NUMBER_PLUS	0x45
#define	 ESM_CLASS_FROM_GSM_GLR_AND_NUMBER_PLUS		0x46
#define	 ESM_CLASS_BIZ_NUMBER_MO					0x47
#define	 ESM_CLASS_FROM_CDMA_GLR_AND_BIZ			0x48
#define	 ESM_CLASS_FROM_GSM_GLR_AND_BIZ				0x49
#define	 ESM_CLASS_BIZ_NUMBER_MT					0x50
#define	 ESM_CLASS_FROM_ESME_BIZ_NUMBER				0x51
#define	 ESM_CLASS_BIZ_NUMBER_AND_PORTED			0x52

// SKT osfi
#define SKT_BEFOR_OSFI_VIMS 			1
#define SKT_BEFOR_OSFI_CNAP 			2
#define SKT_BEFOR_OSFI_PSMS 			4
#define SKT_BEFOR_OSFI_ASMS 			8
#define SKT_BEFOR_OSFI_VTS 				16
#define SKT_BEFOR_OSFI_CCS 				32
#define SKT_BEFOR_OSFI_CNHPPLUS 		64
#define SKT_BEFOR_OSFI_CMF 				128
#define SKT_BEFOR_OSFI_ZBVPN 			256
#define SKT_BEFOR_OSFI_SNAP 			512
#define SKT_BEFOR_OSFI_SMS_MGR 			4096
#define SKT_BEFOR_OSFI_PPS 				32768
#define SKT_BEFOR_OSFI_SMS_NOTI 		262144
#define SKT_BEFOR_OSFI_010_PLUS			33554432
#define SKT_BEFOR_OSFI_WZONE 			536870912
#define SKT_BEFOR_OSFI_INTER_MO_DENY 	1073741824

// SKT capa
#define SKT_CAPA_LONG 					1
#define SKT_CAPA_PICS 					2
#define SKT_CAPA_SPAM 					4
#define SKT_CAPA_COLOR_LONG 			8
#define SKT_CAPA_ICS 					16
#define SKT_CAPA_MMS 					32
#define SKT_CAPA_SECRETSMS 				64
#define SKT_CAPA_QUERYSMS 				128
#define SKT_CAPA_IMSMS 					256
#define SKT_CAPA_MGR 					512
#define SKT_CAPA_PERFECT 				1024
#define SKT_CAPA_PERFECT_AA 			2048
#define SKT_CAPA_PATRIOT 				4096
#define SKT_CAPA_NEW_SPAM 				8192
#define SKT_CAPA_ABROADTEL 				16384
#define SKT_CAPA_BIZ_NUM 				32768
#define SKT_CAPA_URL_NOTI 				524288
#define SKT_CAPA_CALLBACK_NOTI 			1048576
#define SKT_CAPA_RCS 					2097152

enum
{
	DCS_GSM7BIT     = 0x00,
	DCS_ASCII7BIT   = 0x01,
	DCS_8BIT_BINARY = 0x04,
	DCS_UCS2        = 0x08,
	DCS_CP949       = 0x0e
};

enum
{
	MSG_ORG_INFO_MSC  = 0x00,
	MSG_ORG_INFO_ESME = 0x01,
};

enum
{
	MSG_ORIGINATION_ESME = 0x01,
	MSG_ORIGINATION_MSC  = 0x02,
};

enum
{
	NW_CALLBACK_CHANGE_NOT_APPLY = 0x00,
	NW_CALLBACK_CHANGE_APPLY     = 0x01,
};

enum
{
	CALLBACK_CHECK_NOT_APPLY = 0x00,
	CALLBACK_CHECK_APPLY     = 0x01,
};

enum
{
	CALLBACK_NOTI_NOT_APPLY  = 0x00,
	CALLBACK_NOTI_APPLY      = 0x01,
};

int is_forward_esm_class(int esm_class);

void remove_ftn_message_etc(u_char *short_message, int sm_length, 
							 u_char *result, u_char *result_len);

void remove_ftn_message_ucs2(u_char *short_message, int sm_length, 
							 u_char *result, u_char *result_len);

int get_roaming_indicator(int esm_class);

void set_roaming_pmn(char *source_addr, char *roaming_pmn);

int get_wzone_mask(char *source_addr);

int isaddress(int ch);

const char *get_dcs_name(int dcs);
const char *get_osfi_name(int osfi);
const char *get_capability_name(int capability);
const char *get_msg_org_info_name(int msg_org_info);
const char *get_callback_check_name(int callback_check);
const char *get_nw_callback_change_name(int nw_callback_change);
const char *get_msg_origination_name(int msg_origination);
const char *get_callback_noti_name(int callback_noti);

#endif
