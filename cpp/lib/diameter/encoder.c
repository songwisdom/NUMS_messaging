#include "flk_dia.h"

typedef enum {
	ENCODE_FAIL = -1,
	ENCODE_CONTINUE = 0,
	ENCODE_SKIP = 1
}ENCODE_COMMAND_ENUM;
int is_vaildity_encode_avp(FLK_AVP_ST *avp)
{
	int ret = ENCODE_CONTINUE;

	switch ( avp->data_type ) {
		case AVP_TYPE_LIST :
			if (avp->has == false ){
				if ( avp->attr == AVP_ATTR_LIST_MANDANTORY ) {
					if ( avp->start_range == 0 ) {
						ret = ENCODE_SKIP;
					}else {
						error(0, "[%s : %d] %s is mandatory, but data not found", __func__, __LINE__, avp->name);
						ret = ENCODE_FAIL;
					}
				}else { // OPTIONAL
					ret = ENCODE_SKIP;
				}
			}
			break;
		default :
			if ( avp->has == false ) {
				if  ( avp->attr == AVP_ATTR_MANDANTORY ) {
					error(0, "[%s : %d] %s is mandatory, but data not found", __func__, __LINE__, avp->name);
					ret = ENCODE_FAIL;
				}else { // OPTIONAL
					ret = ENCODE_SKIP;
				}
			}
			break;
	}
	
	return ret;
}
void encode_diameter_header(DIAMETER_HEADER_ST *header, Octstr *data)
{
	append_encoded_integer(data, header->version, 1);
	append_encoded_integer(data, header->msg_len, 3);
	append_encoded_integer(data, header->opt.val, 1);
	append_encoded_integer(data, header->command_code, 3);
	append_encoded_integer(data, header->application_id, 4);
	append_encoded_integer(data, header->hop_by_hop_id, 4);
	append_encoded_integer(data, header->end_to_end_id, 4);
}

void encode_avp_header(AVP_HEADER_ST *header, Octstr *data)
{
	int len;

	len = header->data_len;

	append_encoded_integer(data, header->avp_code, 4);
	append_encoded_integer(data, header->opt.val, 1);
	append_encoded_integer(data, len, 3);
	if ( header->opt.bit.vbit ) {
		append_encoded_integer(data, header->vendor_id, 4);
	}
}

bool encode_avp_group(FLK_AVP_ST *avp, Octstr *os)
{
	bool ret;
	int i, len;
	List *lst;

	FLK_AVP_ST *sub_avp;
	Octstr *data;

	lst = convert_octstr_to_pointer(avp->data);
	len = gwlist_len(lst);

	data = octstr_create("");

	for ( i = 0; i < len; ++ i) {
		sub_avp = gwlist_get(lst, i);
		//debug(__func__, __LINE__, "avp(%s) code(%d) ", sub_avp->name, sub_avp->avp_code);
		ret = encode_avp(sub_avp, data);
		if ( ret == false ) {
			error(0, "[%s : %d] encode_avp failed : avp %s(%d)", __func__, __LINE__, sub_avp->name, sub_avp->avp_code);
			break;
		}
	}

	/*
	if ( ret == true ) {
		avp->header.data_len = octstr_len(data) + AVP_MIN_SIZE + (avp->header.vendor_id > 0 ? 4 : 0);
		encode_avp_header(&avp->header, os);
		octstr_append(os, data);
	}
	*/

	octstr_append(os, data);
	octstr_destroy(data);

	return ret;
}

bool encode_avp_list(FLK_AVP_ST *avp, Octstr *os)
{
	bool ret;
	int i;
	int len;

	FLK_AVP_ST *sub_avp;
	List *lst;

	if ( avp->data_type != AVP_TYPE_LIST ) {
		error(0, "[%s : %d] avp %s(%d) avp_data_type invalid (%d)",
				__func__, __LINE__, avp->name, avp->avp_code, avp->data_type);
		return false;
	}


	lst = convert_octstr_to_pointer(avp->data);
	len = gwlist_len(lst);

	if ( (avp->start_range != 0) && (len < avp->start_range)  ) {
		error(0, "[%s: %d] avp %s invalid start_range[%d] ( avp encorder list count %d )", __func__, __LINE__, avp->name, avp->start_range, len);
		return false;
	}

	if ( (avp->end_range != 0 )  && (len > avp->end_range) ) {
		error(0, "[%s : %d] avp %s invalid end_range[%d] ( avp encorder list %d )", __func__, __LINE__, avp->name, avp->start_range, len);
		return false;
	}

	for ( i = 0; i < len; ++ i) {
		sub_avp = gwlist_get(lst, i);
		//debug(__func__, __LINE__, "avp list encode[%d]", i);
		//avp_debug(sub_avp);
		ret = encode_avp(sub_avp, os);
		if ( ret == false ) {
			error(0, "[%s : %d] %s encode_avp failed", __func__, __LINE__, sub_avp->name);
			ret = false;
			break;
		}
	}

	return ret;
}

bool encode_avp(FLK_AVP_ST *avp, Octstr *data)
{
	bool ret = true;
	int l;
	int cmd;
	static const char null_tmp[4] = {0x00, 0x00, 0x00, 0x00};

	Octstr *os = NULL;

	cmd = is_vaildity_encode_avp(avp);
	if ( cmd == ENCODE_FAIL ) {
		error(0, "[%s : %d] %s avp validity check failed", __func__, __LINE__, avp->name);
		return false;
	}
	else if  ( cmd == ENCODE_SKIP )
		return true;

	switch ( avp->data_type ) {
		case AVP_TYPE_INT32 : 
		case AVP_TYPE_UINT32 :
		case AVP_TYPE_ENUM : 
		case AVP_TYPE_INT64 :
		case AVP_TYPE_UINT64 :
		case AVP_TYPE_TIME :
		case AVP_TYPE_FLOAT32 :
		case AVP_TYPE_FLOAT64 :
			os = octstr_byte_order(avp->data);
			break;
		case AVP_TYPE_UTF8STRING :
		case AVP_TYPE_NUMSTRING :
		case AVP_TYPE_OCTSTRING :
			os = octstr_duplicate(avp->data);
			break;
		case AVP_TYPE_ADDRESS :
			{
				AVP_ADDRESS_ST *addr = (AVP_ADDRESS_ST *)octstr_get_cstr(avp->data);
				os = octstr_create("");
				append_encoded_integer(os, addr->addr_type, 2);
				if ( addr->addr_type == ADDR_TYPE_IPV4 ) {
					octstr_append_data(os, (char *)&addr->addr.ipv4, sizeof(addr->addr.ipv4));
				}else {
					octstr_append_data(os, (char *)&addr->addr.ipv6, sizeof(addr->addr.ipv6));
				}
			}
			break;
		case AVP_TYPE_GROUP : 
			os = octstr_create("");
			ret = encode_avp_group(avp, os);
			if  ( ret == false ) {
				error(0, "[%s : %d] %s encode_avp_group failed", __func__, __LINE__, avp->name);
			}
			break;
		case AVP_TYPE_LIST :
			{
				os = octstr_create("");
				ret = encode_avp_list(avp, os);
				if ( ret == true ) {
					octstr_append(data, os);
				}
				octstr_destroy(os);
			}
			return ret;
		default : 
			ret = false; break;
	}

	if ( ret == true ) {
		//debug(__func__, __LINE__, "AVP ENCODER %s", avp->name);
		//avp_debug(avp);
		//debug(__func__, __LINE__, "octstr_len(os) size = %ld", octstr_len(os));
		avp->header.data_len = octstr_len(os) + AVP_MIN_SIZE + (avp->header.vendor_id > 0 ? 4 : 0);
		//debug(__func__, __LINE__, "avp header len %d", avp->header.data_len);
		encode_avp_header(&avp->header, data);
		octstr_append(data, os);
		// AVP Padding 
		l = octstr_len(os) % 4;
		if ( l )  {
			octstr_append_data(data, null_tmp, 4- l);
		}
	}

	if ( os != NULL ) {
		octstr_destroy(os);
	}

	return ret;
}


bool encode_diameter(FLK_DIAMETER_ST *dia, Octstr **raw)
{
	bool ret = false;
	Octstr *data;
	Octstr *header;
	int i, len;
	FLK_AVP_ST *avp;

	len = gwlist_len(dia->avp);
	data = octstr_create("");
	header = octstr_create("");

	for ( i = 0; i < len; ++i ) {
		avp = gwlist_get(dia->avp, i);

		ret = encode_avp(avp, data);
		if ( ret == false ) {
			break;
		}
	}

	if ( ret == true ) {
		dia->header.msg_len = octstr_len(data) + DIAMETER_HEADER_SIZE;
		encode_diameter_header(&dia->header, header);
		octstr_append(header, data);
		*raw = header;
	}else {
		octstr_destroy(header);
	}
	octstr_destroy(data);

	return ret;
}
