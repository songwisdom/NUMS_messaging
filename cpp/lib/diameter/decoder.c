#include "flk_dia.h"

bool decode_diameter_header(DIAMETER_HEADER_ST *header, Octstr *data)
{
	if ( octstr_len(data) < DIAMETER_HEADER_SIZE ) {
		return false;
	}

	header->version = decode_integer(data, 0, 1);
	header->msg_len = decode_integer(data, 1, 3);

	/*
	if ( octstr_len(data) != header->msg_len)
		return false;
	*/

	header->opt.val	= decode_integer(data, 4, 1);

	header->command_code 	= decode_integer(data, 5, 3);
	header->application_id = decode_integer(data, 8, 4);
	header->hop_by_hop_id  = decode_integer(data, 12, 4);
	header->end_to_end_id = decode_integer(data, 16, 4);

	return true;
}

#if 0
bool diameter_header_decoder(FLK_DIAMETER_ST **dia, Octstr *data)
{
	FLK_DIAMETER_ST *val;
	DIAMETER_BIT_ST opt;
	u_char version; 
	int 	length;
	int 	command_code;
	int 	application_id;
	int 	hid;
	int 	eid;

	if ( octstr_len(data) < DIAMETER_HEADER_SIZE ) {
		return false;
	}

	version = decode_integer(data, 0, 1);
	length = decode_integer(data, 1, 3);
	opt.val	= decode_integer(data, 4, 1);
	command_code 	= decode_integer(data, 5, 3);
	application_id = decode_integer(data, 8, 4);
	hid = decode_integer(data, 12, 4);
	eid = decode_integer(data, 16, 4);

	val = diameter_create(command_code);
	if ( val == NULL ) {
		val = diameter_create_empty(command_code, version, opt.val, application_id, hid, eid);
		if ( val == NULL ) {
			return false;
		}
	}else {
		if ( val->header.application_id != application_id ) {
			warning("[%s] [%s] application_id(%u) invalid decode_app_id(%u)", 
					__func__, val->desc, val->application_id, application_id);
			val->application_id = application_id;
		}

		val->hop_by_hop_id = hid;
		val->end_to_end_id = eid;
		val->request_flag = opt.bit.rbit;
		val->proxiable_flag = opt.bit.pbit;
		val->error_flag = opt.bit.ebit;
		val->retransmit_flag = opt.bit.tbit;
	}

	dia = val;
	return true;
}
#endif

/*
bool avp_group_decoder(FLK_AVP_ST **avp, Octstr *os, int attr)
{
	u_int avp_code;
	int length;
	u_int vid = 0;
	FLK_AVP_ST *val;
	AVP_OPT_ST opt;

	if ( octstr_len(os) < AVP_MIN_SIZE ) {
		return false;
	}

	opt.val = 0;

	avp_code = decode_integer(os, 0, 4);
	opt.val = decode_integer(os, 4, 1);
	length = decode_integer(os, 5, 3);
	if ( opt.bit.vbit ) {
		vid = decode_integer(os, 8, 4);
	}

	val = avp_create(avp_code, attr);
	if ( val == NULL ) {
		val = avp_create_empty(avp_code, vid, opt.bit.mbit, opt.bit.pbit, AVP_TYPE_OCTSTRING, attr);
		if ( val == NULL ) {
			return false;
		}
	}else {
		val->header.vendor_id = vid;
		val->header.opt.val = opt.val;
		val->attr = attr;
	}
	return true;
}
*/
bool decode_avp_group(FLK_AVP_ST *avp, Octstr *data)
{
	int len, avp_len;
	List *lst;
	bool ret;
	int pos = 0;
	int padding = 0;
	FLK_AVP_ST *val;

	if ( avp->data_type != AVP_TYPE_GROUP ) {
		return false;
	}

	len = octstr_len(data);
	if ( len != avp->header.data_len ) {
		return false;
	}
	
	lst = convert_octstr_to_pointer(avp->data);

	while ( pos < len  ) {
		avp_len = decode_integer(data, pos + 5, 3);
		Octstr *os = octstr_copy(data, pos, avp_len);
		pos += avp_len;
		padding = avp_len % 4;
		if ( padding > 0 ) {
			pos += (4 - padding);
		}

		ret = decode_avp(&val, os);
		octstr_destroy(os);
		if ( ret == false ) {
			error(0, "[%s : %d] decode_avp failed", __func__, __LINE__);
			break;
		}

		ret = avp_group_set_data_from_avp(avp, val);
		if ( ret == false ) {
			avp_destroy(val);
			error(0, "[%s : %d] decode_avp %s val avp(%s)failed", __func__, __LINE__, avp->name, val->name);
			break;
		}
		avp_destroy(val);

		/*
		ret = avp_group_get(avp, val->avp_code, &sub);
		debug(__func__, __LINE__,"avp_group_get %s ret(%d)", val->name, ret);
		if ( ret == true ) {
		}else {
		}

		if ( ret == true && sub->data_type == AVP_TYPE_LIST ) {
			avp_list_append(sub, val);
		}else {
			avp->has = true;
			gwlist_append(lst, val);
			ret = true;
		}
		*/

	}
	//avp_debug(avp);
	avp->has = ret;

	return ret;
}

bool decode_avp(FLK_AVP_ST **avp, Octstr *data)
{
	int avp_code;
	int msg_len;
	int data_len;
	int vid = 0;
	int pos = 0;
	AVP_OPT_ST opt;
	FLK_AVP_ST *val;
	Octstr *tmp;
	bool ret = false;

	if ( octstr_len(data) < AVP_MIN_SIZE ) {
		error(0, "[%s : %d] invalid avp length : raw length(%ld) ",
				__func__, __LINE__, octstr_len(data));
		return false;
	}

	avp_code = decode_integer(data, 0, 4);
	opt.val = decode_integer(data, 4, 1);
	msg_len = decode_integer(data, 5, 3);
	pos = 8;
	if ( opt.bit.vbit ) {
		vid = decode_integer(data, pos, 4);
		pos += 4;
	}

	if ( msg_len > octstr_len(data) ) {
		error(0, "[%s : %d] avp_code(%d) invalid avp length : raw length(%ld) but avp length(%d)", 
				__func__, __LINE__, avp_code, octstr_len(data), msg_len);
		return false;
	}

	val = avp_create(avp_code, AVP_ATTR_NONE);
	if ( val == NULL ) {
		val = avp_create_empty(avp_code, vid, opt.bit.mbit, opt.bit.pbit, AVP_TYPE_NONE, AVP_ATTR_NONE);
		if ( val == NULL ) {
			error(0, "[%s : %d] avp_code(%d) avp_create_empty failed", __func__, __LINE__, avp_code);
			return false;
		}
	}else {
		// TODO CHECK vendor_id
		val->header.vendor_id = vid;
		val->header.opt = opt;
	}


	data_len = msg_len - pos;
	tmp = octstr_copy(data, pos, data_len);
	val->header.data_len = data_len;

	switch ( val->data_type ) {
		case AVP_TYPE_LIST :
			error(0, "[%s : %d] avp_code(%d) AVP_TYPE_LIST not supported", __func__, __LINE__, avp_code);
			ret = false;
			break;
		case AVP_TYPE_GROUP :
			ret = decode_avp_group(val, tmp);
			break;
		case AVP_TYPE_INT32 :
		case AVP_TYPE_UINT32 :
		case AVP_TYPE_TIME :
		case AVP_TYPE_ENUM : 
			{
				int tmp_int = decode_integer(tmp, 0, 4);
				ret = avp_set_data_int32(val, tmp_int);
			}
			break;
		case AVP_TYPE_FLOAT32 : 
			{
				float tmp_float;
				*(int *)&tmp_float = decode_integer(tmp, 0, 4);
				ret = avp_set_data_float32(val, tmp_float);
			}
			break;
		case AVP_TYPE_INT64 : 
		case AVP_TYPE_UINT64 :
			{
				u_long tmp_long = decode_integer64(tmp, 0, 8);
				ret = avp_set_data_uint64(val, tmp_long);
			}
			break;
		case AVP_TYPE_FLOAT64 : 
			{
				double tmp_float;
				*(double *)&tmp_float = decode_integer(tmp, 0, 8);
				ret = avp_set_data_float64(val, tmp_float);
			}
			break;
		case AVP_TYPE_ADDRESS : 
			{
				int ip_type = decode_integer(tmp, 0, 2);
				char *ptr = octstr_get_cstr(tmp);
				ret = avp_set_data_inaddr(val, ip_type, &ptr[2], (data_len -2));
			}
			break;
		default :
			ret = avp_set_data_octstr(val, tmp);
			break;
	}

	if ( ret == true ) {
		*avp = val;
	}else {
		avp_destroy(val);
	}

	octstr_destroy(tmp);

	return ret;
}

bool decode_avp_split(Octstr *data, List **avplist)
{
	bool ret = false;
	int len;
	int pos = 0;
	int padding = 0;
	int avp_len;
	List *lst;
	FLK_AVP_ST *avp;

	lst = gwlist_create();

	len = octstr_len(data);


	int tot = 0;

	while ( pos < len ) {

		avp_len = decode_integer(data, pos + 5, 3);
		//debug(__func__, __LINE__,"pos [%d] avp_len(%d) packet_len[%d / %d]", pos, avp_len, tot, len);
		//info(0, "[%s:%d] pos [%d] avp_len(%d) packet_len[%d / %d]", __func__, __LINE__, pos, avp_len, tot, len);
		Octstr *os = octstr_copy(data, pos, avp_len);
		pos += avp_len;

		padding = avp_len % 4;
		if ( padding > 0 ) {
			//info(0, "[%s:%d] padding size[%d] add(%d)", __func__, __LINE__, padding, 4-padding);
			//debug(__func__, __LINE__,"padding size[%d] add(%d)", padding, 4-padding);
			pos += (4 - padding);
		}

		tot += (avp_len + padding);

		ret = decode_avp(&avp, os);
		if ( ret == false ) {
			octstr_binary_to_hex(os, 1);
			error(0, "[%s : %d] decode_avp failed : raw[%s]", __func__, __LINE__,  octstr_get_cstr(os));
			octstr_destroy(os);
			break;
		}else {
			// TEST DEBUG
			//avp_debug(avp); //LJH DELETE
		}

		gwlist_append(lst, avp);
		octstr_destroy(os);
	}

	if ( ret == false ) {
		gwlist_destroy(lst, avp_destroy_item);
	}else {
		*avplist = lst;
	}
	//info(0, "[%s:%d] #################################################################", __func__, __LINE__);
	//debug(__func__, __LINE__, "#################################################################\n");

	return ret;
}


bool decode_diameter_defined(FLK_DIAMETER_ST *dia, List *avp_list)
{
	int len, lst_len;
	bool ret;
	//int i;
	FLK_AVP_ST *avp;
	FLK_AVP_ST *val;

	len = gwlist_len(dia->avp);
	lst_len = gwlist_len(avp_list);

	//for (i = 0; (i < len) && (gwlist_len(avp_list) > 0); ++i ) {
	//for (i = 0; i < len; ++i ) {
	while( (val = gwlist_extract_first(avp_list)) != NULL ) {
		/*
		info(0, "[%d / %d]", i, gwlist_len(avp_list));
		val = gwlist_get(avp_list, i);
		if ( val == NULL ) {
			warning(0, "[%s : %d] avp_list %d / %d gwlist_get not found", __func__, __LINE__, i, lst_len);
			break;
		}
		*/

		ret = dia_get_avp(dia, val->avp_code, &avp);
		if ( ret == false ) {
			//error(0, "TEST [%s : %d] Unknown avp(%s:%d)", __func__, __LINE__, val->name, val->avp_code);
			avp = avp_create(val->avp_code, AVP_ATTR_NONE);
			if ( avp != NULL ) {
				//avp_debug(val);
				copy_avp(avp, val);
				gwlist_append(dia->avp, avp);
				avp_destroy(val);
			}
			ret = true;
			continue;
		}

		ret = false;

		switch ( avp->attr ) {
			case AVP_ATTR_FIX :
				if ( val != NULL ) {
					ret = avp_set(avp, val);
				}
				break;
			case AVP_ATTR_MANDANTORY :
				if ( val != NULL ) {
					ret = avp_set(avp, val);
				}
				break;
			case AVP_ATTR_OPTIONAL :
				if ( val != NULL ) {
					ret = avp_set(avp, val);
				}else {
					ret = true;
				}
				break;
			case AVP_ATTR_LIST_MANDANTORY :
				if ( val != NULL ) {
					do {
						ret = avp_list_append(avp, val);
					} while ( (val = extract_avp_list(avp_list, avp->avp_code)) != NULL );
				}
				break;
			case AVP_ATTR_LIST_OPTIONAL :
				if ( val != NULL ) {
					do {
						ret = avp_list_append(avp, val);
					} while ( (val = extract_avp_list(avp_list, avp->avp_code)) != NULL );
				}else {
					ret = true;
				}
				break;
			default :
				//debug(__func__, __LINE__,"avp %s attr [%d]", avp->name, avp->attr);
				if ( val != NULL ) {
					warning(0, "Unknown AVP Append : AVP_CODE(%d)", val->avp_code);
					gwlist_append(dia->avp, val);
				}
				ret = true;
				break;
		}

		if ( avp->attr != AVP_ATTR_LIST_MANDANTORY && avp->attr != AVP_ATTR_LIST_OPTIONAL ) {
			avp_destroy(val);
		}

		if ( ret == false ) {
			error(0, "[%s : %d] avp(%s) failed", __func__, __LINE__, val->name);
			break;
		}
	}

	/*
	while (gwlist_len(avp_list) > 0 ) {
		gwlist_append(dia->avp, val);
	}
	*/

	return ret;
}

/*
bool diameter_undefined_decode(FLK_DIAMETER_ST *dia, List *avp_list)
{
	len = gwlist_len(avp_list);


	return true;
}
*/

bool decode_diameter(FLK_DIAMETER_ST **dia, Octstr *data)
{
	bool ret;
	DIAMETER_HEADER_ST head;
	FLK_DIAMETER_ST *val;
	int len;
	Octstr *avp_data;
	List *avp_list;

	ret = decode_diameter_header(&head, data);
	if ( ret == false ) {
		return false;
	}

	len = octstr_len(data) - DIAMETER_HEADER_SIZE;

	avp_data = octstr_copy(data, DIAMETER_HEADER_SIZE, len);

	ret = decode_avp_split(avp_data, &avp_list);
	if ( ret == false ) {
		error(0, "[%s : %d] decode_avp_split failed : DIAMTER CODE[%u]", __func__, __LINE__, head.command_code);
		return false;
	}

	val = diameter_create_from_packet(head.command_code, head.opt.bit.rbit);
	if ( val == NULL ) {
		warning(0, "Unknown Interface [%u]", head.command_code);
		octstr_dump(data, GW_WARNING);

		val = diameter_create_empty(head.command_code, head.version, head.opt.val, 
				head.application_id, head.hop_by_hop_id, head.end_to_end_id);
		if ( val == NULL ) {
			ret = false;
		}else {
			gwlist_destroy(val->avp, avp_destroy_item);
			val->avp = avp_list;
			ret = true;
		}
	}else {
		memcpy(&val->header, &head, sizeof(DIAMETER_HEADER_ST));
		ret = decode_diameter_defined(val, avp_list);
		gwlist_destroy(avp_list, avp_destroy_item);
	}

	if ( ret == true ) {
		*dia = val;
	}else {
		diameter_destroy(val);
	}
	octstr_destroy(avp_data);

	return ret;
}
