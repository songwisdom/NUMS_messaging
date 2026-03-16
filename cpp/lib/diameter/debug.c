#include "flk_dia.h"

#define PREFIX_AVP_FIX "<%s>"
#define PREFIX_AVP_OPTIONAL "[%s]"
#define PREFIX_AVP_MANDANTORY "{%s}"
#define PREFIX_AVP_NONE "%s"


void set_avp_header_prefix(char *str, FLK_AVP_ST *avp)
{
	char sub[128];
	char *desc;

	switch (avp->attr) {
		case AVP_ATTR_FIX :
			desc = PREFIX_AVP_FIX;
			break;
		case AVP_ATTR_MANDANTORY :
			desc = PREFIX_AVP_MANDANTORY;
			break;
		case AVP_ATTR_OPTIONAL :
			desc = PREFIX_AVP_OPTIONAL;
			break;
		case AVP_ATTR_LIST_MANDANTORY :
			if ( avp->start_range == 0 && avp->end_range == 0 )
				sprintf(sub, "*%s", PREFIX_AVP_MANDANTORY);
			else if ( avp->end_range == 0 )
				sprintf(sub,"%d*%s", avp->start_range, PREFIX_AVP_MANDANTORY);
			else 
				sprintf(sub,"%d*%d%s", avp->start_range, avp->end_range, PREFIX_AVP_MANDANTORY);
			desc = sub;
			break;
		case AVP_ATTR_LIST_OPTIONAL :
			if ( avp->start_range == 0 && avp->end_range == 0 )
				sprintf(sub, "*%s", PREFIX_AVP_OPTIONAL);
			else if ( avp->end_range == 0 )
				sprintf(sub,"%d*%s", avp->start_range, PREFIX_AVP_OPTIONAL);
			else 
				sprintf(sub,"%d*%d%s", avp->start_range, avp->end_range, PREFIX_AVP_OPTIONAL);
			desc = sub;
			break;
		default :
			desc = PREFIX_AVP_NONE;
			break;

	}
	switch (avp->data_type) {
		case AVP_TYPE_GROUP :
			sprintf(str, desc, avp->name);
			break;
		case AVP_TYPE_LIST :
			sprintf(str, desc, avp->name);
			break;
		default :
			sprintf(str, desc, avp->name);
			break;
	}

	return;
}

Octstr  *get_result_desc(u_int val) 
{
	Octstr *ret;
	switch (val) {
		case RESULT_CODE_SUCCESS : ret = octstr_format("val=%u(SUCCESS)\n", val); break;
		case RESULT_CODE_ERROR_USER_UNKNOWN : ret = octstr_format("val=%u(ERR_USER_UNKNOWN)\n", val); break;
		case RESULT_CODE_ERROR_ABSENT_USER : ret = octstr_format("val=%u(ERR_ABSENT_USER)\n", val); break;
		case RESULT_CODE_ERROR_USER_BUSY_FOR_MT_SMS : ret = octstr_format("val=%u(ERR_USER_BUSY_FOR_MT_SMS)\n", val); break;
		case RESULT_CODE_ERROR_FACILITY_NOT_SUPPORTED : ret = octstr_format("val=%u(ERR_FACILITY_NOT_SUPPORTED)\n", val); break;
		case RESULT_CODE_ERROR_ILLEGAL_USER : ret = octstr_format("val=%u(ERR_ILLEGAL_USER)\n", val); break;
		case RESULT_CODE_ERROR_ILLEGAL_EQUIPMENT : ret = octstr_format("val=%u(ERR_ILLEGAL_EQUIPMENT)\n", val); break;
		case RESULT_CODE_ERROR_SM_DELIVERY_FAILURE : ret = octstr_format("val=%u(ERR_SM_DELIVERY_FAULURE)\n", val); break;
		case RESULT_CODE_ERROR_SERVICE_NOT_SUBSCRIBED : ret = octstr_format("val=%u(ERR_SERVICE_NOT_SUBSCRIBED)\n", val); break;
		case RESULT_CODE_ERROR_SERVICE_BARRE : ret = octstr_format("val=%u(ERR_SERVICE_BARRED)\n", val); break;
		case RESULT_CODE_ERROR_MWD_LIST_FULL : ret = octstr_format("val=%u(ERR_MWD_LIST_FULL)\n", val); break;
		case RESULT_CODE_LIMITED_SUCCESS : ret = octstr_format("val=%u(LIMITED_SUCCESS)\n", val); break;
		case RESULT_CODE_MULTI_ROUND_AUTH : ret = octstr_format("val=%u(MULTI_ROUND_AUTH)\n", val); break;
		case RESULT_CODE_COMMAND_UNSUPPORTED : ret = octstr_format("val=%u(ERR_COMMAND_UNSUPPORTED)\n", val); break;
		case RESULT_CODE_UNABLE_TO_DELIVER : ret = octstr_format("val=%u(ERR_UNABLE_TO_DELIVER)\n", val); break;
		case RESULT_CODE_REALM_NOT_SERVED : ret = octstr_format("val=%u(ERR_REALM_NOT_SERVED)\n", val); break;
		case RESULT_CODE_TOO_BUSY : ret = octstr_format("val=%u(ERR_TOO_BUSY)\n", val); break;
		case RESULT_CODE_LOOP_DETECTED : ret = octstr_format("val=%u(ERR_LOOP_DETECTED)\n", val); break;
		case RESULT_CODE_REDIRECT_INDICATION : ret = octstr_format("val=%u(ERR_REDIRECT_INDICATION)\n", val); break;
		case RESULT_CODE_APPLICATION_UNSUPPORTED : ret = octstr_format("val=%u(ERR_APPLICATION_UNSUPPORTED)\n", val); break;
		case RESULT_CODE_INVALID_HDR_BITS : ret = octstr_format("val=%u(ERR_INVALID_HDR_BITS)\n", val); break;
		case RESULT_CODE_INVALID_AVP_BITS : ret = octstr_format("val=%u(ERR_INVALID_AVP_BITS)\n", val); break;
		case RESULT_CODE_UNKNOWN_PEER : ret = octstr_format("val=%u(ERR_UNKNOWN_PEER)\n", val); break;
		case RESULT_CODE_AUTHENTICATION_REJECTED : ret = octstr_format("val=%u(ERR_AUTHENTICATION_REJECTED)\n", val); break;
		case RESULT_CODE_OUT_OF_SPACE : ret = octstr_format("val=%u(ERR_OUT_OF_SPACE)\n", val); break;
		case RESULT_CODE_ELECTION_LOST : ret = octstr_format("val=%u(ERR_ELECTION_LOST)\n", val); break;
		case RESULT_CODE_END_USER_SERVICE_DENIED : ret = octstr_format("val=%u(ERR_END_USER_SERVICE_DENIED)\n", val); break;
		case RESULT_CODE_TRANSIENT_USER_UNKNOWN : ret = octstr_format("val=%u(ERR_TRANSIENT_USER_UNKNOWN)\n", val); break;
		case RESULT_CODE_UNKNOWN_SESSION_ID : ret = octstr_format("val=%u(ERR_UNKNOWN_SESSION_ID)\n", val); break;
		case RESULT_CODE_AUTHORIZATION_REJECTED : ret = octstr_format("val=%u(ERR_AUTHORIZATION_REJECTED)\n", val); break;
		case RESULT_CODE_INVALID_AVP_VALUE : ret = octstr_format("val=%u(ERR_INVALID_AVP_VALUE)\n", val); break;
		case RESULT_CODE_MISSING_AVP : ret = octstr_format("val=%u(ERR_MISSING_AVP)\n", val); break;
		case RESULT_CODE_RESOURCES_EXCEEDED : ret = octstr_format("val=%u(ERR_RESOURCES_EXCEEDED)\n", val); break;
		case RESULT_CODE_CONTRADICTING_AVPS : ret = octstr_format("val=%u(ERR_CONTRADICTING_AVPS)\n", val); break;
		case RESULT_CODE_AVP_NOT_ALLOWED : ret = octstr_format("val=%u(ERR_AVP_NOT_ALLOWED)\n", val); break;
		case RESULT_CODE_AVP_OCCURS_TOO_MANY_TIMES : ret = octstr_format("val=%u(ERR_AVP_OCCURS_TOO_MANY_TIMES)\n", val); break;
		case RESULT_CODE_NO_COMMON_APPLICATION : ret = octstr_format("val=%u(ERR_NO_COMMON_APPLICATION)\n", val); break;
		case RESULT_CODE_UNSUPPORTED_VERSION : ret = octstr_format("val=%u(ERR_UNSUPPORTED_VERSION)\n", val); break;
		case RESULT_CODE_UNABLE_TO_COMPLY : ret = octstr_format("val=%u(ERR_UNABLE_TO_COMPLY)\n", val); break;
		case RESULT_CODE_INVALID_BIT_IN_HEADER : ret = octstr_format("val=%u(ERR_INVALID_BIT_IN_HEADER)\n", val); break;
		case RESULT_CODE_INVALID_AVP_LENGTH : ret = octstr_format("val=%u(ERR_INVALID_AVP_LENGTH)\n", val); break;
		case RESULT_CODE_INVALID_MESSAGE_LENGTH : ret = octstr_format("val=%u(ERR_INVALID_MESSAGE_LENGTH)\n", val); break;
		case RESULT_CODE_INVALID_AVP_BIT_COMBO : ret = octstr_format("val=%u(ERR_INVALID_AVP_BIT_COMBO)\n", val); break;
		case RESULT_CODE_NO_COMMON_SECURITY : ret = octstr_format("val=%u(ERR_NO_COMMON_SECURITY)\n", val); break;
		case RESULT_CODE_DGW_TIMEOUT : ret = octstr_format("val=%u(ERR_DGW_TIMEOUT)\n", val); break;
		case RESULT_CODE_DGW_PEER_DISCONNECTED : ret = octstr_format("val=%u(ERR_DGW_PEER_DISCONNECTED)\n", val); break;
		default : ret = octstr_format("val=%u(Not Defined Error)\n", val); break;
	}
	return ret;
}

Octstr * get_experimental_result_desc( u_int val)
{
	Octstr *ret;
	switch (val) {
		case RESULT_CODE_SUCCESS : ret = octstr_format("val=%u(SUCCESS)\n", val); break;
		case RESULT_CODE_ERROR_USER_UNKNOWN : ret = octstr_format("val=%u(ERR_USER_UNKNOWN)\n", val); break;
		case RESULT_CODE_ERROR_ABSENT_USER : ret = octstr_format("val=%u(ERR_ABSENT_USER)\n", val); break;
		case RESULT_CODE_ERROR_USER_BUSY_FOR_MT_SMS : ret = octstr_format("val=%u(ERR_USER_BUSY_FOR_MT_SMS)\n", val); break;
		case RESULT_CODE_ERROR_FACILITY_NOT_SUPPORTED : ret = octstr_format("val=%u(ERR_FACILITY_NOT_SUPPORTED)\n", val); break;
		case RESULT_CODE_ERROR_ILLEGAL_USER : ret = octstr_format("val=%u(ERR_ILLEGAL_USER)\n", val); break;
		case RESULT_CODE_ERROR_ILLEGAL_EQUIPMENT : ret = octstr_format("val=%u(ERR_ILLEGAL_EQUIPMENT)\n", val); break;
		case RESULT_CODE_ERROR_SM_DELIVERY_FAILURE : ret = octstr_format("val=%u(ERR_SM_DELIVERY_FAULURE)\n", val); break;
		case RESULT_CODE_ERROR_SERVICE_NOT_SUBSCRIBED : ret = octstr_format("val=%u(ERR_SERVICE_NOT_SUBSCRIBED)\n", val); break;
		case RESULT_CODE_ERROR_SERVICE_BARRE : ret = octstr_format("val=%u(ERR_SERVICE_BARRED)\n", val); break;
		case RESULT_CODE_ERROR_MWD_LIST_FULL : ret = octstr_format("val=%u(ERR_MWD_LIST_FULL)\n", val); break;
		case RESULT_CODE_DGW_TIMEOUT : ret = octstr_format("val=%u(ERR_DGW_TIMEOUT)\n", val); break;
		case RESULT_CODE_DGW_PEER_DISCONNECTED : ret = octstr_format("val=%u(ERR_DGW_PEER_DISCONNECTED)\n", val); break;
		default : ret = octstr_format("val=%u(Not Defined Error)\n", val); break;
	}
	return ret;
}
Octstr *get_delivery_failure_cause( u_int val)
{
	Octstr *ret;
	switch(val) {
		case DELIVERY_FAILURE_MEMORY_CAPACITY_EXCEEDED  : ret = octstr_format("val=%u(MEMORY_CAPACITY_EXCEEDED)\n", val); break;
		case DELIVERY_FAILURE_EQUIPMENT_PROTOCOL_ERROR  : ret = octstr_format("val=%u(EQUIPMENT_PROTOCOL_ERROR)\n", val); break;
		case DELIVERY_FAILURE_EQUIPMENT_NOT_SM_EQUIPPED  : ret = octstr_format("val=%u(EQUIPMENT_NOT_SM_EQUIPPED)\n", val); break;
		case DELIVERY_FAILURE_UNKNOWN_SERVICE_CENTRE  : ret = octstr_format("val=%u(UNKNOWN_SERVICE_CENTRE)\n", val); break;
		case DELIVERY_FAILURE_SC_CONGESTION  : ret = octstr_format("val=%u(SC_CONGESTION)\n", val); break;
		case DELIVERY_FAILURE_INVALID_SME_ADDRESS  : ret = octstr_format("val=%u(INVALID_SME_ADDRESS)\n", val); break;
		case DELIVERY_FAILURE_USER_NOT_SC_USER  : ret = octstr_format("val=%u(FAILURE_USER_NOT_SC_USER)\n", val); break;
		default : ret = octstr_format("val=%u(Not Defined Error)\n", val); break;
	}
	return ret;
}
Octstr *get_absent_user_desc(val)
{
	Octstr *ret;
	switch(val) {
		case NO_PAGING_RESPONSE_VIA_THE_MSC : ret = octstr_format("val=%u(NO_PAGING_RESPONSE_VIA_THE_MSC)\n", val); break;
		case IMSI_DETACHED : ret = octstr_format("val=%u(IMSI_DETACHED)\n", val); break;
		case ROAMING_RESTRICTION : ret = octstr_format("val=%u(ROAMING_RESTRICTION)\n", val); break;
		case DEREGISTERED_IN_THE_HLR_FOR_NON_GPRS : ret = octstr_format("val=%u(DEREGISTERED_IN_THE_HLR_FOR_NON_GPRS)\n", val); break;
		case MS_PURGED_FOR_NON_GPRS : ret = octstr_format("val=%u(MS_PURGED_FOR_NON_GPRS)\n", val); break;
		case NO_PAGING_RESPONSE_VIA_THE_SGSN : ret = octstr_format("val=%u(NO_PAGING_RESPONSE_VIA_THE_SGSN)\n", val); break;
		case GPRS_DETACHED : ret = octstr_format("val=%u(GPRS_DETACHED)\n", val); break;
		case DEREGISTERED_IN_THE_HLR_FOR_GPRS : ret = octstr_format("val=%u(DEREGISTERED_IN_THE_HLR_FOR_GPRS)\n", val); break;
		case MS_PURGED_FOR_GPRS : ret = octstr_format("val=%u(MS_PURGED_FOR_GPRS)\n", val); break;
		case UNIDENTIFIED_SUBSCRIBER_VIA_THE_MSC : ret = octstr_format("val=%u(UNIDENTIFIED_SUBSCRIBER_VIA_THE_MSC)\n", val); break;
		case UNIDENTIFIED_SUBSCRIBER_VIA_THE_SGSn : ret = octstr_format("val=%u(UNIDENTIFIED_SUBSCRIBER_VIA_THE_SGSn)\n", val); break;
		case DEREGISTERED_IN_THE_HSS_HLR_FOR_IMS : ret = octstr_format("val=%u(DEREGISTERED_IN_THE_HSS_HLR_FOR_IMS)\n", val); break;
		case NO_RESPONSE_VIA_THE_IP_SM_GW : ret = octstr_format("val=%u(NO_RESPONSE_VIA_THE_IP_SM_GW)\n", val); break;
		default : ret = octstr_format("val=%u(Not Defined Error)\n", val); break;
	}
	return ret;
}
void avp_debug_depth(FLK_AVP_ST *avp, Octstr *ostr, int depth)
{
	char buf[128];
	char header[128];
	char tmp_str[128] = {0x0, };
	int i;
	int dep = depth+1;
	int count = 0;
	List *lst;
	Octstr *os;
	Octstr *data = NULL;
	Octstr *tmp;
	FLK_AVP_ST *sub;

	/*
	if ( avp->has == false )
		return;
	*/
	for(i = 0; i < depth*4; ++i ) {
		tmp_str[i] = ' ';
	}

	set_avp_header_prefix(header, avp);

	switch ( avp->data_type ) {
		case AVP_TYPE_LIST :
			lst = convert_octstr_to_pointer(avp->data);
			data = octstr_create("");

			for ( i = 0; i < gwlist_len(lst); ++i ) {
				sub = gwlist_get(lst, i);
				if ( sub->has == false )
					continue;
				avp_debug_depth(sub, data, dep);
				count++;
			}
			os = octstr_format("%s%s : List Count(%d)\n%s", tmp_str, header, count, octstr_get_cstr(data));
			octstr_append(ostr, os);
			octstr_destroy(os);
			octstr_destroy(data);
			return;
		case AVP_TYPE_GROUP :
			lst = convert_octstr_to_pointer(avp->data);
			//strcpy(header,"AVP_GROUP");
			tmp = octstr_create("");
			for ( i = 0; i < gwlist_len(lst); ++i ) {
				sub = gwlist_get(lst, i);
				if (sub->has == false) {
					continue;
				}
				avp_debug_depth(sub, tmp, dep);
				count++;
			}
			data = octstr_format("\n%s - AVP GROUP Count : %ld -\n%s", tmp_str, count, octstr_get_cstr(tmp));
			octstr_destroy(tmp);
			break;
		case AVP_TYPE_INT32 :
			{
				int val;
				memcpy(&val, octstr_get_cstr(avp->data), sizeof(val));
				data = octstr_format("val=%d\n", val);
			}
			break;
		case AVP_TYPE_UINT32 :
		case AVP_TYPE_ENUM :
			{
				u_int val;
				memcpy(&val, octstr_get_cstr(avp->data), sizeof(val));
				switch ( avp->avp_code ) {
					case avp_result_code :
						data = get_result_desc(val);
						break;
					case avp_experimental_result_code :
						data = get_experimental_result_desc(val);
						break;
					case avp_sm_enumerated_delivery_failure_cause :
						data = get_delivery_failure_cause(val);
						break;
					case avp_absent_user_diagnostic_sm :
						data = get_absent_user_desc(val);
						break;
					default :
						data = octstr_format("val=%u\n", val);
						break;
				}
			}
			break;
		case AVP_TYPE_FLOAT32 :
			{
				float val;
				memcpy(&val, octstr_get_cstr(avp->data), sizeof(val));
				data = octstr_format("val=%f\n", val);
			}
			break;
		case AVP_TYPE_INT64 :
			{
				long val;
				memcpy(&val, octstr_get_cstr(avp->data), sizeof(val));
				data = octstr_format("val=%ld\n", val);
			}
			break;
		case AVP_TYPE_UINT64 :
			{
				u_long val;
				memcpy(&val, octstr_get_cstr(avp->data), sizeof(val));
				data = octstr_format("val=%lu\n", val);
			}
			break;
		case AVP_TYPE_FLOAT64 :
			{
				double val;
				memcpy(&val, octstr_get_cstr(avp->data), sizeof(val));
				data = octstr_format("val=%lf\n", val);
			}
			break;
		case AVP_TYPE_ADDRESS :
			{
				char val[64] = {0x0, };
				int ip_type = 0;
				if ( avp_get_data_address(avp, &ip_type, val, sizeof(val)) == true ) {
					if ( ip_type == ADDR_TYPE_IPV4 ) {
						data = octstr_format("val=ipv4(%d) %s\n", ip_type, val);
					}else {
						data = octstr_format("val=ipv6(%d) %s\n", ip_type, val);
					}
				}else {
					Octstr *hex = octstr_duplicate(avp->data);
					octstr_binary_to_hex(hex, 1);
					data = octstr_format("val=err:decoding failed raw(%s)\n", octstr_get_cstr(hex));
					octstr_destroy(hex);
				}
			}
			break;
		case AVP_TYPE_TIME :
			{
				u_int val;
				memcpy(&val, octstr_get_cstr(avp->data), sizeof(val));
				Octstr *tmp_os = get_str_date((time_t)val);
				data = octstr_format("val=%ld (%s)\n", val, octstr_get_cstr(tmp_os));
				octstr_destroy(tmp_os);
			}
			break;
		case AVP_TYPE_OCTSTRING :
			{
				Octstr *hex = octstr_duplicate(avp->data);
				octstr_binary_to_hex(hex, 1);
				data = octstr_format("val=(0x%s)\n", octstr_get_cstr(hex));
				octstr_destroy(hex);
			}
			break;
		default :
			if  (avp->has == true ) {
				if ( avp->data_type == AVP_TYPE_NONE ) {
					Octstr *hex = octstr_duplicate(avp->data);
					octstr_binary_to_hex(hex, 1);
					data = octstr_format("val='%s' (0x%s)\n", octstr_get_cstr(avp->data), octstr_get_cstr(hex));
					octstr_destroy(hex);
				} else  {
					data = octstr_format("val='%s'\n", octstr_get_cstr(avp->data));
				}
			} else  {
				data = octstr_create("val='data not set'\n");
			}
			break;
	}


	if ( avp->header.opt.bit.vbit == 0 )
		sprintf(buf, "%s", "\0");
	else
		sprintf(buf, " vendor_id : %u", avp->header.vendor_id);

	os = octstr_format("%s%s : avp_code=%u len=%d flags=0x%02X(%c%c%c)%s %s", tmp_str, header,
			avp->header.avp_code, 
			avp->header.data_len,
			avp->header.opt.val,
			(avp->header.opt.bit.vbit == 0 ? '-' : 'V'),
			(avp->header.opt.bit.mbit == 0 ? '-' : 'M'),
			(avp->header.opt.bit.pbit == 0 ? '-' : 'P'),
			buf,
			octstr_get_cstr(data));

	octstr_append(ostr, os);
	octstr_destroy(os);
	octstr_destroy(data);

	dep = ++depth;
}
void avp_debug(FLK_AVP_ST *avp)
{
	Octstr *os = octstr_create("");
	avp_debug_depth(avp, os, 0);
	info(0, "\n%s", octstr_get_cstr(os));
	octstr_destroy(os);
}

void diameter_debug(FLK_DIAMETER_ST *dia)
{
	Octstr *os;
	Octstr *tmp;
	int i;
	int len;
	int count = 0;
	FLK_AVP_ST *avp;

	tmp = octstr_create("");
	len = gwlist_len(dia->avp);
	for ( i = 0; i < len; ++i ) {
		avp = gwlist_get(dia->avp, i);
		if ( avp->has == false ) 
			continue;
		count++;
		avp_debug_depth(avp, tmp, 2);
	}
	
	os = octstr_format("Diameter Protocol\n"
			"    Version : %d\n"
			"    Length  : %d\n"
			"    Flags   : 0x%02X (%c%c%c%c)\n"
			"    Command Code : %d (%s)\n"
			"    Application Id : %d\n"
			"    Hop-by-Hop Identifier : 0x%08X (%u)\n"
			"    End-to-End Identifier : 0x%08X (%u)\n"
			"    - AVP Count : %d -\n%s",
			dia->header.version,
			dia->header.msg_len,
			dia->header.opt.val, 
			(dia->header.opt.bit.rbit == 0 ? '-' : 'R'),
			(dia->header.opt.bit.pbit == 0 ? '-' : 'P'),
			(dia->header.opt.bit.ebit == 0 ? '-' : 'E'),
			(dia->header.opt.bit.tbit == 0 ? '-' : 'T'),
			dia->header.command_code,dia->desc, 
			dia->header.application_id,
			(u_int)dia->header.hop_by_hop_id, (u_int)dia->header.hop_by_hop_id,
			(u_int)dia->header.end_to_end_id, (u_int)dia->header.end_to_end_id,
			count,
			octstr_get_cstr(tmp)
			);


	info(0, "\n%s", octstr_get_cstr(os));
	octstr_destroy(os);
	octstr_destroy(tmp);

}
Octstr *get_dia_debug_ostr(FLK_DIAMETER_ST *dia)
{
	Octstr *os;
	Octstr *tmp;
	int i;
	int len;
	int count = 0;
	FLK_AVP_ST *avp;

	tmp = octstr_create("");
	len = gwlist_len(dia->avp);
	for ( i = 0; i < len; ++i ) {
		avp = gwlist_get(dia->avp, i);
		if ( avp->has == false ) 
			continue;
		count++;
		avp_debug_depth(avp, tmp, 2);
	}
	
	os = octstr_format("Diameter Protocol\n"
			"    Version : %d\n"
			"    Length  : %d\n"
			"    Flags   : 0x%02X (%c%c%c%c)\n"
			"    Command Code : %d (%s)\n"
			"    Application Id : %d\n"
			"    Hop-by-Hop Identifier : 0x%08X (%u)\n"
			"    End-to-End Identifier : 0x%08X (%u)\n"
			"    - AVP Count : %d -\n%s",
			dia->header.version,
			dia->header.msg_len,
			dia->header.opt.val, 
			(dia->header.opt.bit.rbit == 0 ? '-' : 'R'),
			(dia->header.opt.bit.pbit == 0 ? '-' : 'P'),
			(dia->header.opt.bit.ebit == 0 ? '-' : 'E'),
			(dia->header.opt.bit.tbit == 0 ? '-' : 'T'),
			dia->header.command_code,dia->desc, 
			dia->header.application_id,
			(u_int)dia->header.hop_by_hop_id, (u_int)dia->header.hop_by_hop_id,
			(u_int)dia->header.end_to_end_id, (u_int)dia->header.end_to_end_id,
			count,
			octstr_get_cstr(tmp)
			);


	octstr_destroy(tmp);
	return os;
}
void diameter_dump(FLK_DIAMETER_ST *dia, int level, char *fmt, ...)
{
	Octstr *os;
	Octstr *tmp;
	int i;
	int len;
	int count = 0;
	FLK_AVP_ST *avp;
	char buf[1024];

	if ( level >= get_gwlog_level() ) {
		tmp = octstr_create("");
		len = gwlist_len(dia->avp);
		for ( i = 0; i < len; ++i ) {
			avp = gwlist_get(dia->avp, i);
			if ( avp->has == false ) 
				continue;
			count++;
			avp_debug_depth(avp, tmp, 2);
		}

		os = octstr_format("Diameter Protocol\n"
				"    Version : %d\n"
				"    Length  : %d\n"
				"    Flags   : 0x%02X (%c%c%c%c)\n"
				"    Command Code : %d (%s)\n"
				"    Application Id : %d\n"
				"    Hop-by-Hop Identifier : 0x%08X (%u)\n"
				"    End-to-End Identifier : 0x%08X (%u)\n"
				"    - AVP Count : %d -\n%s",
				dia->header.version,
				dia->header.msg_len,
				dia->header.opt.val, 
				(dia->header.opt.bit.rbit == 0 ? '-' : 'R'),
				(dia->header.opt.bit.pbit == 0 ? '-' : 'P'),
				(dia->header.opt.bit.ebit == 0 ? '-' : 'E'),
				(dia->header.opt.bit.tbit == 0 ? '-' : 'T'),
				dia->header.command_code,dia->desc, 
				dia->header.application_id,
				(u_int)dia->header.hop_by_hop_id, (u_int)dia->header.hop_by_hop_id,
				(u_int)dia->header.end_to_end_id, (u_int)dia->header.end_to_end_id,
				count,
				octstr_get_cstr(tmp)
					);

		va_list args;

		va_start(args, fmt);
		vsprintf(buf, fmt, args);

		switch ( level ) {
			case GW_DEBUG : debug("DIAMETER_DUMP", 0, "%s\n%s", buf, octstr_get_cstr(os)); break;
			case GW_INFO : info(0, "%s\n%s", buf, octstr_get_cstr(os)); break;
			case GW_WARNING : warning(0, "%s\n%s", buf, octstr_get_cstr(os)); break;
			case GW_ERROR : error(0, "%s\n%s", buf, octstr_get_cstr(os)); break;
			default : info(0, "%s\n%s", buf, octstr_get_cstr(os)); break;
		}
		va_end(args);
		octstr_destroy(os);
		octstr_destroy(tmp);
	}

}

void diameter_call_log(char *src, char *dest, FLK_DIAMETER_ST *dia)
{
	Octstr *shost = NULL;
	Octstr *dhost = NULL;
	bool ret;
	bool is_result_set;
	Octstr *msg = NULL;
	int rc;
	u_int result = 0;

	if ( is_flow_log_on() == false ) {
		return;
	}

	ret = dia_get_avp_data_octstr(dia, avp_origin_host, &shost);
	if ( ret == true ) {
		rc = octstr_search_char(shost, '.', 0);
		if ( rc > 0 ) {
			octstr_truncate(shost, rc);
		}
	}

	ret = dia_get_avp_data_octstr(dia, avp_destination_host, &dhost);
	if ( ret == true ) {
		rc = octstr_search_char(dhost, '.', 0);
		if ( rc > 0 ) {
			octstr_truncate(dhost, rc);
		}
	}

	if ( dia->header.opt.bit.rbit == 0 ) {
		is_result_set = dia_get_avp_data_uint32(dia, avp_result_code, &result);
		if ( is_result_set == false ) {
			is_result_set = dia_get_avp_data_uint32(dia, avp_experimental_result_code, &result);
		}

		if ( is_result_set == true ) {
			msg = get_result_desc(result);
		}
	}


	flow_log("[%s -> %s] [%s] SRC_HOST(%s) DEST_HOST(%s) %s", src, dest, dia->intf_name, 
			(shost == NULL) ? "NOT_SET" : octstr_get_cstr(shost),
			(dhost == NULL) ? "NOT_SET" : octstr_get_cstr(dhost),
			(msg == NULL) ? "" : octstr_get_cstr(msg));

	if ( msg != NULL ) { 
		octstr_destroy(msg);
	}
	if ( shost != NULL ) { 
		octstr_destroy(shost);
	}
	if ( dhost != NULL ) { 
		octstr_destroy(dhost);
	}
}

