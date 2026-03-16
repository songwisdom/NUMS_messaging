#include "smpp_pdu_skt.h"

int is_forward_esm_class(u_int esm_class)
{
    switch (esm_class)
    {
        case ESM_CLASS_FORWARD_TO_ESME:
        case ESM_CLASS_NUMBER_PLUS_MO:
        case ESM_CLASS_BIZ_NUMBER_MO:
        case ESM_CLASS_FROM_CDMA_GLR_AND_FORWARD:
        case ESM_CLASS_FROM_CDMA_GLR_AND_NUMBER_PLUS:
        case ESM_CLASS_FROM_CDMA_GLR_AND_BIZ:
        case ESM_CLASS_FROM_GSM_GLR_AND_FORWARD:
        case ESM_CLASS_FROM_GSM_GLR_AND_NUMBER_PLUS:
        case ESM_CLASS_FROM_GSM_GLR_AND_BIZ:
            return 1;
        default:
            return 0;
    }
}

void remove_ftn_message_etc(u_char *short_message, u_int sm_length, 
							 u_char *result, u_char *result_len)
{
	u_int  pos, copy_len;

	if (short_message[0] != 0x07)
	{
		memcpy(result, short_message, sm_length);
		*result_len = sm_length;

		return;
	}

	for (pos = 1; pos < sm_length; pos++)
	{
		if (short_message[pos] == 0x07)
		{
			break;
		}
	}

	if ((pos + 1) >= sm_length)
	{
		memcpy(result, short_message, sm_length);
		*result_len = sm_length;
		
		return;
	}

	copy_len = sm_length - (pos + 1);

	memcpy(result, &short_message[pos + 1], copy_len);

	*result_len = copy_len;
}

void remove_ftn_message_ucs2(u_char *short_message, u_int sm_length, 
							 u_char *result, u_char *result_len)
{
	u_int  pos, copy_len;

	if (short_message[0] != 0x00 || short_message[1] != 0x07)
	{
		return;
	}

	for (pos = 2; pos < sm_length; pos += 2)
	{
		if (short_message[pos] == 0x00 && short_message[pos + 1] == 0x07)
		{
			break;
		}
	}

	if ((pos + 2) >= sm_length)
	{
		return;
	}

	copy_len = sm_length - (pos + 2);

	memcpy(result, &short_message[pos + 2], copy_len);

	*result_len = copy_len;
}

u_int get_roaming_indicator(u_int esm_class)
{
	switch (esm_class)
	{
		// cdma GLR 0x01
		case ESM_CLASS_3G_KTF_CDMA_GLR:
		case ESM_CLASS_3G_LGT_CDMA_GLR:
		case ESM_CLASS_2G_KTF_CDMA_GLR:
		case ESM_CLASS_2G_LGT_CDMA_GLR:
		case ESM_CLASS_FROM_CDMA_GLR:
		case ESM_CLASS_FROM_CDMA_GLR_AND_PORTED:
		case ESM_CLASS_FROM_CDMA_GLR_AND_FORWARD:
		case ESM_CLASS_FROM_CDMA_GLR_AND_NUMBER_PLUS:
		case ESM_CLASS_FROM_CDMA_GLR_AND_BIZ:
			return 1;
		// gsm GLR  0x02
		case ESM_CLASS_3G_KTF_GSM_GLR:
		case ESM_CLASS_3G_LGT_GSM_GLR:
		case ESM_CLASS_2G_KTF_GSM_GLR:
		case ESM_CLASS_2G_LGT_GSM_GLR:
		case ESM_CLASS_FROM_DS_SMSC:
		case ESM_CLASS_FROM_DS_SMSC_AND_PORTED:
		case ESM_CLASS_FROM_GSM_GLR_AND_FORWARD:
		case ESM_CLASS_FROM_GSM_GLR_AND_NUMBER_PLUS:
		case ESM_CLASS_FROM_GSM_GLR_AND_BIZ:
			return 2;
		// no GLR
		default:
			return 0;
	}
}

void set_roaming_pmn(char *source_addr, char *roaming_pmn)
{
	u_int copy_len, len, pos;

	len = strlen(source_addr);

	for (pos = len - 1; pos >= 0; pos--)
	{
		if (source_addr[pos] == '*')
		{
			break;
		}
	}

	// ex) source_addr is '0113331111'
	//     - len = 10, pos = -1
	//                    '*' or '*123'
	// 	   - len = 1, pos = 0
	//                    '1*' or '0113331111*'
	//     - len = 2, pos = 1
	if (pos <= 0 || pos + 1 == len)
	{
		warning(0, "esm_class is GLR, but no setting in source_addr");
		sprintf(roaming_pmn, "0");

		return;
	}

	copy_len = len - (pos + 1);

	// roaming_pmn exist
	// copy roaming_pmn except '*'
	// ex) source_addr       0113331111*12345678
	//     roaming_pmn       12345678
	// ex) 01133311111*12345678 -> 0113331111
	source_addr[pos] = '\0';

	sprintf(roaming_pmn, "%s", &source_addr[pos + 1]);
}

u_int get_wzone_mask(char *source_addr)
{
	int len = strlen(source_addr);

	if (source_addr[len - 1] != 'a')
	{
		return 0x00000000;
	}

	source_addr[len - 1] = '\0';

	return 0x00000020;
}

int isaddress(int ch)
{
	if ((ch >= '0' && ch <= '9') ||
		 ch == '*' || ch == '#' || ch == 'a')
		return ch;

	return 0;
}

const char *get_esm_class_name(u_int esm_class)
{
	// SKT SMPP EMS_CLASS 
	switch (esm_class)
	{
		case ESM_CLASS_DEFAULT:
			return "DEFAULT";
		case ESM_CLASS_DELIVER:
			return "DELIVER";
		case ESM_CLASS_RECEIPT:
			return "RECEIPT";
		case ESM_CLASS_2G_KTF:
			return "2G_KTF";
		case ESM_CLASS_2G_LGT:
			return "2G_LGT";
		case ESM_CLASS_3G_KTF:
			return "3G_KTF";
		case ESM_CLASS_3G_LGT:
			return "3G_LGT";
		case ESM_CLASS_3G_KTF_CDMA_GLR:
			return "3G_KTF_CDMA_GLR";
		case ESM_CLASS_3G_KTF_GSM_GLR:
			return "3G_KTF_GSM_GLR";
		case ESM_CLASS_3G_LGT_CDMA_GLR:
			return "3G_LGT_CDMA_GLR";
		case ESM_CLASS_3G_LGT_GSM_GLR:
			return "3G_LGT_GSM_GLR";
		case ESM_CLASS_2G_KTF_CDMA_GLR:
			return "2G_KTF_CDMA_GLR";
		case ESM_CLASS_2G_LGT_CDMA_GLR:
			return "2G_LGT_CDMA_GLR";
		case ESM_CLASS_2G_KTF_GSM_GLR:
			return "2G_KTF_GSM_GLR";
		case ESM_CLASS_2G_LGT_GSM_GLR: 
			return "2G_LGT_GSM_GLR";
		case ESM_CLASS_DEFAULT_AND_NOTI:
			return "DEFAULT_AND_NOTI";
		case ESM_CLASS_PORTED_AND_NOTI_TO_ESME:
			return "PORTED_AND_NOTI_TO_ESME";
		case ESM_CLASS_PORTED_AND_NOTI_FROM_ESME:
			return "PORTED_AND_NOTI_FROM_ESME";
		case ESM_CLASS_2G_KTF_NOTI:
			return "2G_KTF_NOTI";
		case ESM_CLASS_2G_LGT_NOTI:
			return "2G_LGT_NOTI";
		case ESM_CLASS_3G_KTF_NOTI:
			return "3G_KTF_NOTI";
		case ESM_CLASS_3G_LGT_NOTI:
			return "3G_LGT_NOTI";
		case ESM_CLASS_FROM_CDMA_GLR:
			return "FROM_CDMA_GLR";
		case ESM_CLASS_FROM_CDMA_GLR_AND_PORTED:
			return "FROM_CDMA_GLR_AND_PORTED";
		case ESM_CLASS_FROM_CDMA_GLR_AND_FORWARD:
			return "FROM_CDMA_GLR_AND_FORWARD";
		case ESM_CLASS_FROM_DS_SMSC:
			return "FROM_DS_SMSC";
		case ESM_CLASS_FROM_DS_SMSC_AND_PORTED:
			return "FROM_DS_SMSC_AND_PORTED";
		case ESM_CLASS_FROM_GSM_GLR_AND_FORWARD:
			return "FROM_GSM_GLR_AND_FORWARD";
		case ESM_CLASS_NOCHARGE:
			return "NOCHARGE";
		case ESM_CLASS_FORWARD_TO_ESME:
			return "FORWARD_TO_ESME";
		case ESM_CLASS_FORWARD_FROM_ESME:
			return "FORWARD_FROM_ESME";
		case ESM_CLASS_FORWARD_RECEIPT:
			return "FORWARD_RECEIPT";
		case ESM_CLASS_FORWARD_AND_PORTED:
			return "FORWARD_AND_PORTED";
		case ESM_CLASS_FORWARD_RECEIPT_DOUBLE:
			return "FORWARD_RECEIPT_DOUBLE";
		case ESM_CLASS_PORTED:
			return "PORTED";
		case ESM_CLASS_NUMBER_PLUS_MO:
			return "NUMBER_PLUS_MO";
		case ESM_CLASS_NUMBER_PLUS_MT:
			return "NUMBER_PLUS_MT";
		case ESM_CLASS_FROM_CDMA_GLR_AND_NUMBER_PLUS:
			return "FROM_CDMA_GLR_AND_NUMBER_PLUS";
		case ESM_CLASS_FROM_GSM_GLR_AND_NUMBER_PLUS:
			return "FROM_GSM_GLR_AND_NUMBER_PLUS";
		case ESM_CLASS_BIZ_NUMBER_MO:
			return "BIZ_NUMBER_MO";
		case ESM_CLASS_FROM_CDMA_GLR_AND_BIZ:
			return "FROM_CDMA_GLR_AND_BIZ";
		case ESM_CLASS_FROM_GSM_GLR_AND_BIZ:
			return "FROM_GSM_GLR_AND_BIZ";
		case ESM_CLASS_BIZ_NUMBER_MT:
			return "BIZ_NUMBER_MT";
		case ESM_CLASS_FROM_ESME_BIZ_NUMBER:
			return "FROM_ESME_BIZ_NUMBER";
		case ESM_CLASS_BIZ_NUMBER_AND_PORTED:
			return "BIZ_NUMBER_AND_PORTED";
		default:
			return "UNKNOWN";
	}
}

const char *get_dcs_name(u_int dcs)
{
	switch (dcs)
	{
		case DCS_GSM7BIT:
			return "GSM7BIT";
		case DCS_ASCII7BIT:
			return "ASCII7BIT";
		case DCS_8BIT_BINARY:
			return "8BIT_BINARY";
		case DCS_UCS2:
			return "UCS2";
		case DCS_CP949:
			return "CP949";
		default:
			return "UNKNOWN";
	}
}

const char *get_osfi_name(u_int osfi)
{
	switch (osfi)
	{
		case SKT_BEFOR_OSFI_PSMS:
			return "PREMIUM_SMS";
		case SKT_BEFOR_OSFI_ASMS:
			return "AVATAR_SMS";
		case SKT_BEFOR_OSFI_SNAP:
			return "SNAP";
		case SKT_BEFOR_OSFI_SMS_NOTI:
			return "SMS_NOTI";
		case SKT_BEFOR_OSFI_SMS_MGR:
			return "MANAGER";
		case SKT_BEFOR_OSFI_PPS:
			return "PPS";
		case SKT_BEFOR_OSFI_010_PLUS:
			return "010+";
		case SKT_BEFOR_OSFI_WZONE:
			return "WZONE";
		case SKT_BEFOR_OSFI_INTER_MO_DENY:
			return "INTER_MO_DENY";
		default:
			return "";
	}
}

const char *get_capability_name(u_int capability)
{
	switch (capability)
	{
		case SKT_CAPA_LONG:
			return "LONG";
		case SKT_CAPA_PICS:
			return "PICS";
		case SKT_CAPA_SPAM:
			return "SPAM";
		case SKT_CAPA_COLOR_LONG:
			return "COLOR_LONG";
		case SKT_CAPA_ICS:
			return "ICS";
		case SKT_CAPA_SECRETSMS:
			return "SECRET_SMS";
		case SKT_CAPA_QUERYSMS:
			return "SMS_QUERY";
		case SKT_CAPA_IMSMS:
			return "IM_SMS";
		case SKT_CAPA_MGR:
			return "MANAGER";
		case SKT_CAPA_PERFECT_AA:
			return "PERFECT_AA";
		case SKT_CAPA_PATRIOT:
			return "PATRIOT";
		case SKT_CAPA_NEW_SPAM:
			return "NEW_SPAM";
		case SKT_CAPA_ABROADTEL:
			return "ABROAD_TEL";
		case SKT_CAPA_BIZ_NUM:
			return "BIZ_NUM";
		case SKT_CAPA_URL_NOTI:
			return "URL_NOTI";
		case SKT_CAPA_CALLBACK_NOTI:
			return "CALLBACK_NOTI";
		case SKT_CAPA_RCS:
			return "RCS";
		default:
			return "";
	}
}

const char *get_msg_org_info_name(u_int msg_org_info)
{
	switch (msg_org_info)
	{
		case MSG_ORG_INFO_MSC:
			return "PHONE";
		case MSG_ORG_INFO_ESME:
			return "SERVER";
		default:
			return "UNKNOWN";
	}
}

const char *get_msg_origination_name(u_int msg_origination)
{
	switch (msg_origination)
	{
		case MSG_ORIGINATION_MSC:
			return "PHONE";
		case MSG_ORIGINATION_ESME:
			return "SERVER";
		default:
			return "UNKNOWN";
	}
}

const char *get_callback_check_name(u_int callback_check)
{
	switch (callback_check)
	{
		case CALLBACK_CHECK_APPLY:
			return "CB_CHECK_APPLY";
		case CALLBACK_CHECK_NOT_APPLY:
			return "CB_CHECK_NOT_APPLY";
		default:
			return "UNKNOWN";
	}
}

const char *get_nw_callback_change_name(u_int nw_callback_change)
{
	switch (nw_callback_change)
	{
		case NW_CALLBACK_CHANGE_APPLY:
			return "NW_CB_ON";
		case NW_CALLBACK_CHANGE_NOT_APPLY:
			return "NW_CB_OFF";
		default:
			return "UNKNOWN";
	}
}

const char *get_callback_noti_name(u_int callback_noti)
{
	switch (callback_noti)
	{
		case CALLBACK_NOTI_APPLY:
			return "CB_NOTI_APPLY";
		case CALLBACK_NOTI_NOT_APPLY:
			return "CB_NOTI_NOT_APPLY";
		default:
			return "UNKNOWN";
	}
}
