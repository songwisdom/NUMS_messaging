#include "flk_dia.h"

#define DIAMETER_UNKNOWN_INTF "UNKNOWN_INTF"

static void avp_create_append(FLK_DIAMETER_ST *dia, int avp_code, int attr)
{
	FLK_AVP_ST *avp = avp_create(avp_code, attr);
	// TODO AVP EXIST?
	gwlist_append(dia->avp, avp);
}
static void avp_list_create_append(FLK_DIAMETER_ST *dia, int avp_code, int start_range, int end_range, int attr)
{
	FLK_AVP_ST *avp = avp_list_create(avp_code, start_range, end_range, attr);
	// TODO AVP EXIST?
	gwlist_append(dia->avp, avp);
}


FLK_DIAMETER_ST *diameter_create_empty(int command_code, int version, u_char opt, u_int app_id, u_int hid, u_int eid)
{
	FLK_DIAMETER_ST *dia = gw_malloc(sizeof(FLK_DIAMETER_ST));
	memset(dia, 0x00, sizeof(FLK_DIAMETER_ST));

	int rbit = (opt & 0x80) ? 1 : 0;

	dia->avp = gwlist_create();
	dia->intf_code = command_code + (rbit << 24);
	dia->command_code = command_code;
	dia->header.command_code = command_code;
	dia->header.version = version;
	dia->header.opt.val = opt;
	dia->header.application_id = app_id;
	dia->header.hop_by_hop_id = hid;
	dia->header.end_to_end_id = eid;
	dia->intf_name = DIAMETER_UNKNOWN_INTF;
	dia->desc = DIAMETER_UNKNOWN_INTF;

	return dia;
}
void diameter_info_set(FLK_DIAMETER_ST *dia, int code, int rbit, int pbit, int app_id, char *intf_name, char *desc)
{
	dia->header.version = 1;
	dia->header.opt.bit.rbit = (rbit == 0 ? 0 : 1);
	dia->header.opt.bit.pbit = (pbit == 0 ? 0 : 1);
	dia->intf_code = code + (rbit == 0 ? 0 : ( 1 << 24 ));
	dia->command_code = code;
	dia->header.command_code = code;
	dia->header.application_id = app_id;
	dia->desc = desc;
	dia->intf_name = intf_name;
}

FLK_DIAMETER_ST *diameter_create(int intf_code)
{

	FLK_DIAMETER_ST *dia = gw_malloc(sizeof(FLK_DIAMETER_ST));
	memset(dia, 0x00, sizeof(FLK_DIAMETER_ST));

	dia->avp = gwlist_create();

	switch(intf_code) {
#define AVP_DEF(name, full_name, code, vid, mbit, pbit, type, fields)
#define AVP_ENUM_TYPE(enum_name, code)
#define AVP_GROUP_MANDANTORY(name)
#define AVP_GROUP_OPTIONAL(name)
#define AVP_GROUP_LIST_OPTIONAL(name, start_range, end_range)
#define AVP_GROUP_LIST_MANDANTORY(name, start_range, end_range) 
#define INTF_AVP_FIX(name) avp_create_append(dia, avp_##name, AVP_ATTR_FIX);
#define INTF_AVP_OPTIONAL(name) avp_create_append(dia, avp_##name, AVP_ATTR_OPTIONAL);
#define INTF_AVP_MANDANTORY(name) avp_create_append(dia, avp_##name, AVP_ATTR_MANDANTORY);
#define INTF_AVP_LIST_OPTIONAL(name, start_range, end_range) \
		avp_list_create_append(dia, avp_##name, start_range, end_range, AVP_ATTR_LIST_OPTIONAL);
#define INTF_AVP_LIST_MANDANTORY(name, start_range, end_range) \
		avp_list_create_append(dia, avp_##name, start_range, end_range, AVP_ATTR_LIST_MANDANTORY);
#define DIAMETER_INTF(name, cmd_name, c_code, rbit, pbit, app_id, fields) case DIA_INTF_##name : { \
			diameter_info_set(dia, c_code, rbit, pbit, app_id, #name, #cmd_name); \
			fields \
		} break;
#include "./diameter.def"
		default : 
			gwlist_destroy(dia->avp, avp_destroy_item);
			gw_free(dia);
			dia = NULL;
			break;
	}
	/*
	debug(__func__, __LINE__, "DIAMETER %s (%s) Create intf_code(%d) command_code(%d)", 
			dia->desc, dia->intf_name, dia->intf_code, dia->command_code);
	int i;

	for ( i = 0; i < gwlist_len(dia->avp); ++i ) {
		FLK_AVP_ST *avp = gwlist_get(dia->avp, i);
		debug(__func__, __LINE__, "%s attr[%d]", avp->name, avp->attr);
	}
	*/
	return dia;
}
FLK_DIAMETER_ST *diameter_create_from_packet(int command_code, int request_bit)
{
	return diameter_create(command_code + (request_bit << 24));
}

void diameter_destroy(FLK_DIAMETER_ST *dia)
{
	gwlist_destroy(dia->avp, avp_destroy_item);
	gw_free(dia);
}

// DIAMETER AVP GET
bool dia_get_avp(FLK_DIAMETER_ST *dia, u_int avp_code, FLK_AVP_ST **avp)
{
	FLK_AVP_ST *val;
	int i, len;
	bool ret = false;

	len = gwlist_len(dia->avp);
	for ( i = 0; i < len; ++i ) {
		val = (FLK_AVP_ST *)gwlist_get(dia->avp, i);
		if ( val->avp_code == avp_code ) {
			ret = true;
			*avp = val;
			break;
		}
	}

	return ret;
}

bool dia_get_avp_data(FLK_DIAMETER_ST *dia, u_int avp_code, void **data, int *len)
{
	FLK_AVP_ST *avp;
	bool ret;

	ret = dia_get_avp(dia, avp_code, &avp);
	if ( ret == false ) {
		return false;
	}

	return avp_get_data(avp, data, len);
}

bool dia_get_avp_data_int32(FLK_DIAMETER_ST *dia, u_int avp_code, int *val)
{
	FLK_AVP_ST *avp;
	bool ret;

	ret = dia_get_avp(dia, avp_code, &avp);
	if ( ret == false ) {
		return false;
	}
	return avp_get_data_int32(avp, val);
}

bool dia_get_avp_data_uint32(FLK_DIAMETER_ST *dia, u_int avp_code, u_int *val)
{
	FLK_AVP_ST *avp;
	bool ret;

	ret = dia_get_avp(dia, avp_code, &avp);
	if ( ret == false ) {
		return false;
	}
	return avp_get_data_uint32(avp, val);
}

bool dia_get_avp_data_int64(FLK_DIAMETER_ST *dia, u_int avp_code, long *val)
{
	FLK_AVP_ST *avp;
	bool ret;

	ret = dia_get_avp(dia, avp_code, &avp);
	if ( ret == false ) {
		return false;
	}
	return avp_get_data_int64(avp, val);
}
bool dia_get_avp_data_uint64(FLK_DIAMETER_ST *dia, u_int avp_code, u_long *val)
{
	FLK_AVP_ST *avp;
	bool ret;

	ret = dia_get_avp(dia, avp_code, &avp);
	if ( ret == false ) {
		return false;
	}
	return avp_get_data_uint64(avp, val);
}
bool dia_get_avp_data_float32(FLK_DIAMETER_ST *dia, u_int avp_code, float *val)
{
	FLK_AVP_ST *avp;
	bool ret;

	ret = dia_get_avp(dia, avp_code, &avp);
	if ( ret == false ) {
		return false;
	}
	return avp_get_data_float32(avp, val);
}
bool dia_get_avp_data_float64(FLK_DIAMETER_ST *dia, u_int avp_code, double *val)
{
	FLK_AVP_ST *avp;
	bool ret;

	ret = dia_get_avp(dia, avp_code, &avp);
	if ( ret == false ) {
		return false;
	}
	return avp_get_data_float64(avp, val);
}
bool dia_get_avp_data_enum(FLK_DIAMETER_ST *dia, u_int avp_code, u_int *val)
{
	FLK_AVP_ST *avp;
	bool ret;

	ret = dia_get_avp(dia, avp_code, &avp);
	if ( ret == false ) {
		return false;
	}
	return avp_get_data_enum(avp, val);
}
bool dia_get_avp_data_cstr(FLK_DIAMETER_ST *dia, u_int avp_code, char **cstr, int *len)
{
	FLK_AVP_ST *avp;
	bool ret;

	ret = dia_get_avp(dia, avp_code, &avp);
	if ( ret == false ) {
		return false;
	}
	return avp_get_data_cstr(avp, cstr, len);
}
bool dia_get_avp_data_numstr(FLK_DIAMETER_ST *dia, u_int avp_code, char **numstr, int *len)
{
	FLK_AVP_ST *avp;
	bool ret;

	ret = dia_get_avp(dia, avp_code, &avp);
	if ( ret == false ) {
		return false;
	}
	return avp_get_data_numstr(avp, numstr, len);
}
bool dia_get_avp_data_octstr(FLK_DIAMETER_ST *dia, u_int avp_code, Octstr **octstr)
{
	FLK_AVP_ST *avp;
	bool ret;

	ret = dia_get_avp(dia, avp_code, &avp);
	if ( ret == false ) {
		return false;
	}
	return avp_get_data_octstr(avp, octstr);
}
bool dia_get_avp_data_utf8str(FLK_DIAMETER_ST *dia, u_int avp_code, char **utfstr, int *len)
{
	FLK_AVP_ST *avp;
	bool ret;

	ret = dia_get_avp(dia, avp_code, &avp);
	if ( ret == false ) {
		return false;
	}
	return avp_get_data_utf8str(avp, utfstr, len);
}

bool dia_get_avp_data_address(FLK_DIAMETER_ST *dia, u_int avp_code, int *ip_type, char *str_addr, int str_size)
{
	FLK_AVP_ST *avp;
	bool ret;

	ret = dia_get_avp(dia, avp_code, &avp);
	if ( ret == false ) {
		return false;
	}
	return avp_get_data_address(avp, ip_type, str_addr, str_size);
}
bool dia_get_avp_data_time(FLK_DIAMETER_ST *dia, u_int avp_code, u_int *val)
{
	FLK_AVP_ST *avp;
	bool ret;

	ret = dia_get_avp(dia, avp_code, &avp);
	if ( ret == false ) {
		return false;
	}
	return avp_get_data_time(avp, val);
}

// DIAMETER AVP SET
bool dia_set_avp(FLK_DIAMETER_ST *dia, FLK_AVP_ST *avp)
{
	FLK_AVP_ST *val;
	List *lst;
	bool ret;

	ret = dia_get_avp(dia, avp->avp_code, &val);
	if ( ret == false ) {
		gwlist_append(dia->avp, avp);
		return true;
	}

	if ( avp->data_type != val->data_type ) {
		return false;
	}

	val->header.vendor_id = avp->header.vendor_id;
	val->header.opt.bit.mbit = avp->header.opt.bit.mbit;
	val->header.opt.bit.pbit = avp->header.opt.bit.pbit;

	switch(avp->data_type) {
		case AVP_TYPE_INT32 : 
		case AVP_TYPE_UINT32 : 
		case AVP_TYPE_INT64 : 
		case AVP_TYPE_UINT64 : 
		case AVP_TYPE_FLOAT32 : 
		case AVP_TYPE_FLOAT64 : 
		case AVP_TYPE_TIME :
		case AVP_TYPE_ENUM : 
			memcpy(octstr_get_cstr(val->data), octstr_get_cstr(avp->data), octstr_len(avp->data));
			break;
		case AVP_TYPE_ADDRESS :
			memcpy(octstr_get_cstr(val->data), octstr_get_cstr(avp->data), octstr_len(avp->data));
			break;
		case AVP_TYPE_UTF8STRING : 
		case AVP_TYPE_OCTSTRING : 
		case AVP_TYPE_NUMSTRING : 
			octstr_delete(val->data, 0, octstr_len(val->data));
			octstr_append(val->data, val->data);
			break;
		case AVP_TYPE_GROUP :
			lst = (List *)convert_octstr_to_pointer(avp->data);
			avp_set_data_group(val, lst);
			break;
		case AVP_TYPE_LIST : // TODO list replace ??
			avp_list_append(val, avp);
			break;
		default :
			return false;
	}
	return true;
}

bool dia_set_avp_data(FLK_DIAMETER_ST *dia, u_int avp_code, void *data, int len)
{
	FLK_AVP_ST *val;
	bool ret;

	ret = dia_get_avp(dia, avp_code, &val);
	if ( ret == false ) {
		val = avp_create(avp_code, AVP_ATTR_NONE);
		if ( val == NULL ) {
			error(0, "[%s : %d] avp_code(%d) dia_get_avp failed", __func__, __LINE__, avp_code);
			return false;
		}else {
			gwlist_append(dia->avp, val);
		}
	}

	return avp_set_data(val, data, len);
}

bool dia_set_avp_data_int32(FLK_DIAMETER_ST *dia, u_int avp_code, int val)
{
	return dia_set_avp_data(dia, avp_code, (void *)&val, sizeof(val));
}
bool dia_set_avp_data_uint32(FLK_DIAMETER_ST *dia, u_int avp_code, u_int val)
{
	return dia_set_avp_data(dia, avp_code, (void *)&val, sizeof(val));
}
bool dia_set_avp_data_int64(FLK_DIAMETER_ST *dia, u_int avp_code, long val)
{
	return dia_set_avp_data(dia, avp_code, (void *)&val, sizeof(val));
}
bool dia_set_avp_data_uint64(FLK_DIAMETER_ST *dia, u_int avp_code, u_long val)
{
	return dia_set_avp_data(dia, avp_code, (void *)&val, sizeof(val));
}
bool dia_set_avp_data_float32(FLK_DIAMETER_ST *dia, u_int avp_code, float val)
{
	return dia_set_avp_data(dia, avp_code, (void *)&val, sizeof(val));
}
bool dia_set_avp_data_float64(FLK_DIAMETER_ST *dia, u_int avp_code, double val)
{
	return dia_set_avp_data(dia, avp_code, (void *)&val, sizeof(val));
}
bool dia_set_avp_data_enum(FLK_DIAMETER_ST *dia, u_int avp_code, u_int val)
{
	return dia_set_avp_data(dia, avp_code, (void *)&val, sizeof(val));
}
bool dia_set_avp_data_cstr(FLK_DIAMETER_ST *dia, u_int avp_code, char *cstr, int len)
{
	return dia_set_avp_data(dia, avp_code, (void *)cstr, len);
}
bool dia_set_avp_data_numstr(FLK_DIAMETER_ST *dia, u_int avp_code, char *numstr, int len)
{
	return dia_set_avp_data(dia, avp_code, (void *)numstr, len);
}
bool dia_set_avp_data_octstr(FLK_DIAMETER_ST *dia, u_int avp_code, Octstr *octstr)
{
	return dia_set_avp_data(dia, avp_code, (void *)octstr_get_cstr(octstr), octstr_len(octstr));
}
bool dia_set_avp_data_utf8str(FLK_DIAMETER_ST *dia, u_int avp_code, char *val, int len)
{
	return dia_set_avp_data(dia, avp_code, (void *)val, len);
}
bool dia_set_avp_data_address(FLK_DIAMETER_ST *dia, u_int avp_code, int ip_type, char *str_addr)
{
	AVP_ADDRESS_ST val;
	if ( convert_address_to_net(&val, ip_type, str_addr) == false )
		return false;
	else
		return dia_set_avp_data(dia, avp_code, (void *)&val, sizeof(val));
}
bool dia_set_avp_data_time(FLK_DIAMETER_ST *dia, u_int avp_code, u_int val)
{
	return dia_set_avp_data(dia, avp_code, (void *)&val, sizeof(val));
}
bool dia_set_avp_data_group(FLK_DIAMETER_ST *dia, u_int avp_code, FLK_AVP_ST *group_avp)
{
	FLK_AVP_ST *val;
	bool ret;
	List *lst;

	ret = dia_get_avp(dia, avp_code, &val);
	if ( ret == false ) {
		return false;
	}

	memcpy(&val->header, &group_avp->header, sizeof(AVP_HEADER_ST));

	lst = convert_octstr_to_pointer(group_avp->data);

	return avp_set_data_group(val, lst);
}

// TODO
bool dia_set_avp_data_list(FLK_DIAMETER_ST *dia, u_int avp_code, Octstr *octstr)
{
	return true;
}

bool dia_append_avp_list(FLK_DIAMETER_ST *dia, u_int avp_code, FLK_AVP_ST *avp)
{
	FLK_AVP_ST *val;
	bool ret;

	ret = dia_get_avp(dia, avp_code, &val);
	if ( ret == false ) {
		return false;
	}

	return avp_list_append(val, avp);

}

bool dia_set_avp_group_data(FLK_DIAMETER_ST *dia, u_int group_avp, u_int avp_code, void *data, int len)
{
	FLK_AVP_ST *avp;
	bool ret;

	ret = dia_get_avp(dia, group_avp, &avp);
	if ( ret == false ) {
		return false;
	}

	if ( avp->data_type != AVP_TYPE_GROUP ) {
		return false;
	}

	return avp_group_set_data(avp, avp_code, data, len);
}

bool dia_set_avp_group_data_int32(FLK_DIAMETER_ST *dia, u_int group_avp, u_int avp_code, int val)
{
	return dia_set_avp_group_data(dia, group_avp, avp_code, (void  *)&val, sizeof(val));
}
bool dia_set_avp_group_data_int64(FLK_DIAMETER_ST *dia, u_int group_avp, u_int avp_code, long val)
{
	return dia_set_avp_group_data(dia, group_avp, avp_code, (void  *)&val, sizeof(val));
}
bool dia_set_avp_group_data_float32(FLK_DIAMETER_ST *dia, u_int group_avp, u_int avp_code, float val)
{
	return dia_set_avp_group_data(dia, group_avp, avp_code, (void  *)&val, sizeof(val));
}
bool dia_set_avp_group_data_float64(FLK_DIAMETER_ST *dia, u_int group_avp, u_int avp_code, double val)
{
	return dia_set_avp_group_data(dia, group_avp, avp_code, (void  *)&val, sizeof(val));
}
bool dia_set_avp_group_data_enum(FLK_DIAMETER_ST *dia, u_int group_avp, u_int avp_code, u_int val)
{
	return dia_set_avp_group_data(dia, group_avp, avp_code, (void  *)&val, sizeof(val));
}

bool dia_set_avp_group_data_cstr(FLK_DIAMETER_ST *dia, u_int group_avp, u_int avp_code, char *val, int len)
{
	return dia_set_avp_group_data(dia, group_avp, avp_code, (void *)val, len);
}

bool dia_set_avp_group_data_numstr(FLK_DIAMETER_ST *dia, u_int group_avp, u_int avp_code, char *val, int len)
{
	return dia_set_avp_group_data(dia, group_avp, avp_code, (void *)val, len);
}
bool dia_set_avp_group_data_octstr(FLK_DIAMETER_ST *dia, u_int group_avp, u_int avp_code, Octstr *val)
{
	return dia_set_avp_group_data(dia, group_avp, avp_code, (void *)octstr_get_cstr(val), octstr_len(val));
}
bool dia_set_avp_group_data_utf8str(FLK_DIAMETER_ST *dia, u_int group_avp, u_int avp_code, char *val, int len)
{
	return dia_set_avp_group_data(dia, group_avp, avp_code, (void *)val, len);
}
bool dia_set_avp_group_data_address(FLK_DIAMETER_ST *dia, u_int group_avp, u_int avp_code, int ip_type, char *str_addr)
{
	AVP_ADDRESS_ST val;
	if ( convert_address_to_net(&val, ip_type, str_addr) == false )
		return false;
	else 
		return dia_set_avp_group_data(dia, group_avp, avp_code, (void *)&val, sizeof(AVP_ADDRESS_ST));
}
bool dia_set_avp_group_data_time(FLK_DIAMETER_ST *dia, u_int group_avp, u_int avp_code, u_int val)
{
	return dia_set_avp_group_data(dia, group_avp, avp_code, (void *)&val, sizeof(val));
}

bool dia_get_avp_group_data(FLK_DIAMETER_ST *dia, u_int group_avp, u_int avp_code, void **data, int *len)
{
	FLK_AVP_ST *avp;
	bool ret;

	ret = dia_get_avp(dia, group_avp, &avp);
	if ( ret == false ) {
		return false;
	}

	if ( avp->data_type != AVP_TYPE_GROUP ) {
		return false;
	}

	return avp_group_get_data(avp, avp_code, data, len);
}

bool dia_get_avp_group_data_int_type(FLK_DIAMETER_ST *dia, u_int group_avp, u_int avp_code, void *val)
{
	FLK_AVP_ST *avp;
	bool ret;

	ret = dia_get_avp(dia, group_avp, &avp);
	if ( ret == false ) {
		return false;
	}

	if ( avp->data_type != AVP_TYPE_GROUP ) {
		return false;
	}

	return avp_group_get_data_int_type(avp, avp_code, val);
}

bool dia_get_avp_group_int32(FLK_DIAMETER_ST *dia, u_int group_avp, u_int avp_code, int *val)
{
	return dia_get_avp_group_data_int_type(dia, group_avp, avp_code, (void *)val);
}
bool dia_get_avp_group_int64(FLK_DIAMETER_ST *dia, u_int group_avp, u_int avp_code, int *val)
{
	return dia_get_avp_group_data_int_type(dia, group_avp, avp_code, (void *)val);
}
bool dia_get_avp_group_float32(FLK_DIAMETER_ST *dia, u_int group_avp, u_int avp_code, int *val)
{
	return dia_get_avp_group_data_int_type(dia, group_avp, avp_code, (void *)val);
}
bool dia_get_avp_group_float64(FLK_DIAMETER_ST *dia, u_int group_avp, u_int avp_code, int *val)
{
	return dia_get_avp_group_data_int_type(dia, group_avp, avp_code, (void *)val);
}
bool dia_get_avp_group_enum(FLK_DIAMETER_ST *dia, u_int group_avp, u_int avp_code, int *val)
{
	return dia_get_avp_group_data_int_type(dia, group_avp, avp_code, (void *)val);
}
bool dia_get_avp_group_octstr(FLK_DIAMETER_ST *dia, u_int group_avp, u_int avp_code, Octstr **octstr)
{
	FLK_AVP_ST *avp;
	bool ret;

	ret = dia_get_avp(dia, group_avp, &avp);
	if ( ret == false ) {
		return false;
	}

	if ( avp->data_type != AVP_TYPE_GROUP ) {
		return false;
	}

	return avp_group_get_data_octstr(avp, avp_code, octstr);
}
bool dia_get_avp_group_cstr(FLK_DIAMETER_ST *dia, u_int group_avp, u_int avp_code, char **val, int *len)
{
	return dia_get_avp_group_data(dia, group_avp, avp_code, (void **)val, len);
}
bool dia_get_avp_group_numstr(FLK_DIAMETER_ST *dia, u_int group_avp, u_int avp_code, char **val, int *len)
{
	return dia_get_avp_group_data(dia, group_avp, avp_code, (void **)val, len);
}
bool dia_get_avp_group_utf8str(FLK_DIAMETER_ST *dia, u_int group_avp, u_int avp_code, char **val, int *len)
{
	return dia_get_avp_group_data(dia, group_avp, avp_code, (void **)val, len);
}
bool dia_get_avp_group_address(FLK_DIAMETER_ST *dia, u_int group_avp, u_int avp_code, int *ip_type, char *str_addr, int str_size)
{
	FLK_AVP_ST *avp;
	bool ret;

	ret = dia_get_avp(dia, group_avp, &avp);
	if ( ret == false ) {
		return false;
	}

	if ( avp->data_type != AVP_TYPE_GROUP ) {
		return false;
	}

	return avp_group_get_data_address(avp, avp_code, ip_type, str_addr, str_size);
}
bool dia_get_avp_group_time(FLK_DIAMETER_ST *dia, u_int group_avp, u_int avp_code, u_int *val)
{
	return dia_get_avp_group_data_int_type(dia, group_avp, avp_code, (void *)val);
}

bool dia_set_avp_group_list_data_int32(FLK_DIAMETER_ST *dia, u_int group_avp, u_int avp_code, int val) 
{
	FLK_AVP_ST *avp;
	bool ret;
	

	ret = dia_get_avp(dia, group_avp, &avp);
	if ( ret == false ) {
		return false;
	}

	if ( avp->data_type != AVP_TYPE_GROUP ) {
		return false;
	}

	return avp_group_list_append_data_int32(avp, avp_code, val);
}
bool dia_get_avp_data_group(FLK_DIAMETER_ST *dia, u_int group_avp, List **val)
{
	FLK_AVP_ST *avp;
	bool ret;

	ret = dia_get_avp(dia, group_avp, &avp);
	if ( ret == false ) {
		return false;
	}

	if ( avp->data_type != AVP_TYPE_GROUP ) {
		return false;
	}

	*val = (List *)convert_octstr_to_pointer(avp->data);
	return true;
}
bool diameter_avp_copy(FLK_DIAMETER_ST *dest, FLK_DIAMETER_ST *src, int avp_code)
{
	bool ret;
	FLK_AVP_ST *org_avp;
	FLK_AVP_ST *dest_avp;

	ret = dia_get_avp(src, avp_code, &org_avp);
	if ( ret == false ) {
		return false;
	}

	ret = dia_get_avp(dest, avp_code, &dest_avp);
	if ( ret == false ) {
		dest_avp = gw_malloc(sizeof(FLK_AVP_ST));
		ret = dia_set_avp(dest, dest_avp);
		if ( ret == false ) {
			error(0, "[%s : %d] diameter(%s) append failed : avp %s(%d) ",
					__func__, __LINE__, src->desc, org_avp->name, org_avp->avp_code);
			return false;
		}
	}

	copy_avp(dest_avp, org_avp);

	return true;
}

/*
FLK_DIAMETER_ST *diameter_create_empty_response(FLK_DIAMETER_ST *req, Octstr *host, Octstr *realm, int result_code)
{

	bool ret;
	FLK_DIAMETER_ST *resp;
	Octstr *ostr;

FLK_DIAMETER_ST *diameter_create_empty(int command_code, int version, u_char opt, u_int app_id, u_int hid, u_int eid);
	resp = diameter_create_empty(req->header.command_code,
			req->header.version,
			0,
			req->header.hop_by_hop_id,
			req->header.end_to_end_id);
	if ( resp == NULL ) {
		error(0, "[%s : %d] diameter_create_empty is null", __func__, __LINE__);
		return NULL;
	}


	ret = dia_get_avp_data_octstr(req, avp_session_id, &ostr);
	if ( ret == true ) {
	}

	return resp;

}
*/

FLK_DIAMETER_ST *diameter_create_from_request(FLK_DIAMETER_ST *req, Octstr *host, Octstr *realm, int result_code)
{
	FLK_DIAMETER_ST *pdu;
	Octstr *ostr;
	bool ret;
	u_int val;

	int code;

	if ( (req->intf_code & 0x01000000) != 0x01000000 ) {
		error(0, "[%s : %d] %s is not request type : intf_code(%u) command_code(%08X)", 
				__func__, __LINE__, req->desc, req->intf_code, req->command_code);
		return NULL;
	}

	code = req->intf_code & 0x00FFFFFF; // TODO CHECK 

	pdu = diameter_create(code);
	if ( pdu == NULL ) {
		pdu = diameter_create_empty(req->header.command_code,
				req->header.version,
				0,
				req->header.application_id,
				req->header.hop_by_hop_id,
				req->header.end_to_end_id);
		if ( pdu == NULL ) {
			error(0, "[%s : %d] %s Resp Create failed : command_code(%u : %08X)", 
					__func__, __LINE__, req->desc, code, code);
			return NULL;
		}
	}

	pdu->header.hop_by_hop_id = req->header.hop_by_hop_id;
	pdu->header.end_to_end_id = req->header.end_to_end_id;

	ret = dia_get_avp_data_octstr(req, avp_session_id, &ostr);
	if ( ret == true ) {
		ret = dia_set_avp_data_octstr(pdu, avp_session_id, ostr);
		if ( ret == false ) {
			error(0, "[%s : %d] avp_session_id set failed data[%s]", __func__, __LINE__, octstr_get_cstr(ostr));
			octstr_destroy(ostr);
			return NULL;
		}
		octstr_destroy(ostr);
	}

	// TODO Vendor-Specific-Application-Id Check & Setting.
	diameter_avp_copy(pdu, req, avp_vendor_specific_application_id);

	// TODO result_code 
	if ( result_code > DIAMETER_NO_COMMON_SECURITY ) {
		ret = dia_set_avp_group_data_int32(pdu, avp_experimental_result, avp_vendor_id, 10415);
		ret = dia_set_avp_group_data_int32(pdu, avp_experimental_result, avp_experimental_result_code, result_code);
	}else {
		ret = dia_set_avp_data_uint32(pdu, avp_result_code, result_code);
	}
	
	// TODO Auth-Session-State
	ret = dia_get_avp_data_uint32(req, avp_auth_session_state, &val);
	if ( ret == true ) {
		ret = dia_set_avp_data_enum(pdu, avp_auth_session_state, 1);
	} // else skip...
	
	// Origin-Host
	dia_set_avp_data_octstr(pdu, avp_origin_host, host);
	
	// Origin-Realm
	dia_set_avp_data_octstr(pdu, avp_origin_realm, realm);

	ret = dia_get_avp_data_uint32(req, avp_origin_state_id, &val);
	if ( ret == true ) {
		dia_set_avp_data_uint32(pdu, avp_origin_state_id, val);
	}

	//OPTIONAL
	diameter_avp_copy(pdu, req, avp_supported_vendor_id); 
	diameter_avp_copy(pdu, req, avp_auth_application_id); 
	diameter_avp_copy(pdu, req, avp_inband_security_id); 
	diameter_avp_copy(pdu, req, avp_acct_application_id); 
	diameter_avp_copy(pdu, req, avp_vendor_specific_application_id);
	//ret = dia_get_avp_data_uint32(req, avp_firmware_revision, &data);
	return pdu;
}

bool dia_get_avp_list_root(FLK_DIAMETER_ST *dia, u_int avp_code, FLK_AVP_ST **avp)
{
	return dia_get_avp(dia, avp_code, avp);
}

int dia_get_avp_list_count(FLK_DIAMETER_ST *dia, u_int avp_code)
{
	FLK_AVP_ST *avp;
	bool ret;

	ret = dia_get_avp(dia, avp_code, &avp);
	if  (ret == false ) {
		error(0, "[%s : %d] %s avp_code(%u) get failed", __func__, __LINE__, dia->desc,  avp_code);
		return false;
	}

	return avp_list_avp_count(avp);
}

bool dia_get_avp_list(FLK_DIAMETER_ST *dia, u_int avp_code, List **data)
{
	FLK_AVP_ST *avp;
	bool ret;

	ret = dia_get_avp(dia, avp_code, &avp);
	if  (ret == false ) {
		error(0, "[%s : %d] %s avp_code(%u) get failed", __func__, __LINE__, dia->desc,  avp_code);
		return false;
	}

	return avp_list_get_avplist(avp, data);
}

/*
void test_list()
{
	FLK_DIAMETER_ST *dia;
	List *list;
	FLK_AVP_ST *avp;


	ret = dia_get_avp_list(dia, avp_vendor_specific_application_id, &list);

	if ( ret == true ) {
		int i;
		for ( i = 0; i < gwlist_len(list); ++i ) {
			avp = gwlist_get(list, i);
		}
	}
}
*/


FLK_DIAMETER_ST *make_response_from_cer(FLK_DIAMETER_ST *req)
{
	FLK_DIAMETER_ST *pdu;
	Octstr *ostr;
	bool ret;

	int code;

	if ( (req->intf_code & 0x01000000) != 0x01000000 ) {
		error(0, "[%s : %d] %s is not request type : intf_code(%u)(%08x) command_code(%08X)", 
				__func__, __LINE__, req->desc, req->intf_code, req->intf_code, req->command_code);
		return NULL;
	}

	code = req->intf_code & 0x00FFFFFF; // TODO CHECK 

	pdu = diameter_create(code);
	if ( pdu == NULL ) {
		error(0, "[%s : %d] %s Resp Create failed : command_code(%u : %08X)", 
				__func__, __LINE__, req->desc, code, code);
		return NULL;
	}

	//debug(__func__, __LINE__, "Create %s (%s)", pdu->desc, pdu->intf_name);

	pdu->header.hop_by_hop_id = req->header.hop_by_hop_id;
	pdu->header.end_to_end_id = req->header.end_to_end_id;

	ret = dia_get_avp_data_octstr(req, avp_session_id, &ostr);
	if ( ret == true ) {
		ret = dia_set_avp_data_octstr(pdu, avp_session_id, ostr);
		if ( ret == false ) {
			error(0, "[%s : %d] avp_session_id set failed data[%s]", __func__, __LINE__, octstr_get_cstr(ostr));
			octstr_destroy(ostr);
			return NULL;
		}
		octstr_destroy(ostr);
	}


	// TODO Vendor-Specific-Application-Id Check & Setting.
	

	//OPTIONAL
	u_int data;
	ret = dia_get_avp_data_uint32(req, avp_origin_state_id, &data);
	if ( ret == true ) {
		dia_set_avp_data_uint32(pdu, avp_origin_state_id, data);
	}

	ret = diameter_avp_copy(pdu, req, avp_supported_vendor_id); 
	if(ret == false) {
		// Not Set : Optional Nothing...
	}

	ret = diameter_avp_copy(pdu, req, avp_auth_application_id); 
	if(ret == true) {
		// Not Set : Optional Nothing...
	}

	ret = diameter_avp_copy(pdu, req, avp_inband_security_id); 
	if ( ret == false ) {
		// Not Set : Optional Nothing...
	}

	ret = diameter_avp_copy(pdu, req, avp_acct_application_id); 
	if ( ret == false ) {
		// Not Set : Optional Nothing...
	}

	ret = diameter_avp_copy(pdu, req, avp_vendor_specific_application_id);
	if ( ret == false ) {
		// Not Set : Optional Nothing...
	}

	ret = dia_get_avp_data_uint32(req, avp_firmware_revision, &data);
	if ( ret == true ) {
		dia_set_avp_data_uint32(pdu, avp_firmware_revision, data);
	}

	
	return pdu;
}
FLK_DIAMETER_ST *make_cea_from_cer(FLK_DIAMETER_ST *req, Octstr *host, Octstr *realm, List *ip, u_int vendor_id, Octstr *product_name, int result_code)
{
	FLK_DIAMETER_ST *cea;
	bool ret;

	if ( req->intf_code != DIA_INTF_CER ) {
		error(0, "[%s : %d] %s is not request type : intf_code(%u)(%08x) command_code(%08X)", 
				__func__, __LINE__, req->desc, req->intf_code, req->intf_code, req->command_code);
		return NULL;
	}

	cea = diameter_create(DIA_INTF_CEA);
	if ( cea == NULL ) {
		error(0, "[%s : %d] %s CEA Create failed : request command_code(%u : %08X)", 
				__func__, __LINE__, req->desc, req->header.command_code, req->header.command_code);
		return NULL;
	}

	//debug(__func__, __LINE__, "Create %s (%s)", cea->desc, cea->intf_name);
	if ( result_code != RESULT_CODE_SUCCESS ) {
		cea->header.opt.bit.ebit = 1;
	}

	cea->header.hop_by_hop_id = req->header.hop_by_hop_id;
	cea->header.end_to_end_id = req->header.end_to_end_id;

	dia_set_avp_data_uint32(cea, avp_result_code, result_code);

	// TODO Vendor-Specific-Application-Id Check & Setting.
	
	// Origin-Host
	dia_set_avp_data_octstr(cea, avp_origin_host, host);
	
	//  Origin-Realm
	dia_set_avp_data_octstr(cea, avp_origin_realm, realm);

	// Host Ip Address
	if ( (ip != NULL) && (gwlist_len(ip) > 0) ) {
		int i;
		for ( i = 0; i < gwlist_len(ip); ++i ) {
			Octstr *os = gwlist_get(ip, i);
			FLK_AVP_ST *avp = avp_create(avp_host_ip_address, AVP_ATTR_NONE);
			ret = avp_set_data_address(avp, ADDR_TYPE_IPV4, octstr_get_cstr(os));
			ret = dia_append_avp_list(cea, avp_host_ip_address, avp);
		}
	}

	// Vendor-Id
	dia_set_avp_data_uint32(cea, avp_vendor_id, vendor_id);

	// Product-Name
	dia_set_avp_data_octstr(cea, avp_product_name, product_name);

	//OPTIONAL
	u_int data;
	ret = dia_get_avp_data_uint32(req, avp_origin_state_id, &data);
	if ( ret == true ) {
		dia_set_avp_data_uint32(cea, avp_origin_state_id, data);
	}

	ret = diameter_avp_copy(cea, req, avp_supported_vendor_id); 
	if(ret == false) {
		// Not Set : Optional Nothing...
	}

	ret = diameter_avp_copy(cea, req, avp_auth_application_id); 
	if(ret == true) {
		// Not Set : Optional Nothing...
	}

	ret = diameter_avp_copy(cea, req, avp_inband_security_id); 
	if ( ret == false ) {
		// Not Set : Optional Nothing...
	}

	ret = diameter_avp_copy(cea, req, avp_acct_application_id); 
	if ( ret == false ) {
		// Not Set : Optional Nothing...
	}

	ret = diameter_avp_copy(cea, req, avp_vendor_specific_application_id);
	if ( ret == false ) {
		// Not Set : Optional Nothing...
	}

	ret = dia_get_avp_data_uint32(req, avp_firmware_revision, &data);
	if ( ret == true ) {
		dia_set_avp_data_uint32(cea, avp_firmware_revision, data);
	}

	
	return cea;
}
bool dia_append_avp_group_list_data(FLK_DIAMETER_ST *dia, u_int group_avp, u_int avp_code, void *data, int len)
{
	bool ret;
	FLK_AVP_ST *avp;
	
	ret = dia_get_avp(dia, group_avp, &avp);
	if ( ret == false ) {
		error(0, "[%s : %d] error not founc avp(%d)", __func__, __LINE__, group_avp);
		return false;
	}

	return avp_group_list_append_data(avp, avp_code, data, len);
}

FLK_DIAMETER_ST *make_response_from_ofr(FLK_DIAMETER_ST *req, int result_code)
{
	FLK_DIAMETER_ST *pdu;
	Octstr *ostr;
	bool ret;

	int code;

	if ( (req->intf_code & 0x01000000) != 0x01000000 ) {
		error(0, "[%s : %d] %s is not request type : intf_code(%u) command_code(%08X)", 
				__func__, __LINE__, req->desc, req->intf_code, req->command_code);
		return NULL;
	}

	code = req->intf_code & 0x00FFFFFF; // TODO CHECK 

	pdu = diameter_create(code);
	if ( pdu == NULL ) {
		error(0, "[%s : %d] %s Resp Create failed : command_code(%u : %08X)", 
				__func__, __LINE__, req->desc, code, code);
		return NULL;
	}

	//debug(__func__, __LINE__, "Create %s (%s)", pdu->desc, pdu->intf_name);

	pdu->header.hop_by_hop_id = req->header.hop_by_hop_id;
	pdu->header.end_to_end_id = req->header.end_to_end_id;

	// FIX
	ret = dia_get_avp_data_octstr(req, avp_session_id, &ostr);
	if ( ret == true ) {
		ret = dia_set_avp_data_octstr(pdu, avp_session_id, ostr);
		if ( ret == false ) {
			error(0, "[%s : %d] avp_session_id set failed data[%s]", __func__, __LINE__, octstr_get_cstr(ostr));
			octstr_destroy(ostr);
			return NULL;
		}
		octstr_destroy(ostr);
	}

	// Vendor-Specific-Application-Id Check & Setting.
	ret = diameter_avp_copy(pdu, req, avp_vendor_specific_application_id);
	if ( ret == false ) {
		// Not Set : Optional Nothing...
	}
	
	// TODO Auth-Session-State
	ret = dia_set_avp_data_enum(pdu, avp_auth_session_state, 1);
	if ( ret == true ) {
	} // else skip...
	
	// TODO Origin-Host
	ret = dia_get_avp_data_octstr(req, avp_destination_host, &ostr);
	if ( ret == true ) {
		dia_set_avp_data_octstr(pdu, avp_origin_host, ostr);
		// TODO Exception
		octstr_destroy(ostr);
	}
	
	// TODO Origin-Realm
	ret = dia_get_avp_data_octstr(req, avp_destination_realm, &ostr);
	if ( ret == true ) {
		dia_set_avp_data_octstr(pdu, avp_origin_realm, ostr);
		// TODO Exception
		octstr_destroy(ostr);
	}

	if ( result_code != 2001 && result_code != 2002 ) {
		pdu->header.opt.bit.ebit = 1;
	}

	switch ( avp_result_code ) {
		case DIAMETER_ERROR_USER_UNKNOWN :
		case  DIAMETER_ERROR_ABSENT_USER           :
		case  DIAMETER_ERROR_USER_BUSY_FOR_MT_SMS        :
		case  DIAMETER_ERROR_FACILITY_NOT_SUPPORTED        :
		case  DIAMETER_ERROR_ILLEGAL_USER        :
		case  DIAMETER_ERROR_ILLEGAL_EQUIPMENT        :
		case  DIAMETER_ERROR_SM_DELIVERY_FAILURE  :
		case  DIAMETER_ERROR_SERVICE_NOT_SUBSCRIBED        :
		case  DIAMETER_ERROR_SERVICE_BARRED        :
		case  DIAMETER_ERROR_MWD_LIST_FULL        :
			ret = dia_set_avp_group_data_int32(pdu, avp_experimental_result, avp_vendor_id, 10415);
			ret = dia_set_avp_group_data_int32(pdu, avp_experimental_result, avp_experimental_result_code, result_code);
			break;
		default :
			ret = dia_set_avp_data_uint32(pdu, avp_result_code, result_code);
			if ( ret == false ) {
				error(0, "[%s : %d] error AVP_RESULT_CODE set failed", __func__, __LINE__);
				// TODO Exception
			}
			break;
		
	}
	
	return pdu;
}

