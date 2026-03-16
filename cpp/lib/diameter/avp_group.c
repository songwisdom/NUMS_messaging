#include "flk_dia.h"


bool avp_group_set_data_from_avp(FLK_AVP_ST *avp_group, FLK_AVP_ST *val)
{
	bool ret;
	FLK_AVP_ST *sub;
	FLK_AVP_ST *data;

	ret = avp_group_get(avp_group, val->avp_code, &sub);
	if ( ret == true ) {
		switch ( sub->data_type ) {
			case AVP_TYPE_LIST :
				data = duplicate_avp(val);
				ret = avp_list_append(sub, data);
				if ( ret == false ) {
					avp_destroy(data);
				}
				break;
			case AVP_TYPE_GROUP :
				ret = avp_group_set_data_from_avp(sub, val);
				break;
			default :
				ret = avp_set(sub, val);
				break;
		}
	}else {
		warning(0, "[%s : %d] AVP_GROUP(%s) Undefined AVP(%s) Code(%d)", 
				__func__,__LINE__, avp_group->name, val->name, val->avp_code);
		octstr_dump(val->data, GW_WARNING);
		List *lst = convert_octstr_to_pointer(avp_group->data);
		data = duplicate_avp(val);
		gwlist_append(lst, data);
		ret = true;
	}

	return ret;
}
bool avp_group_set_data(FLK_AVP_ST *avp, u_int avp_code, void *data, int len)
{
	FLK_AVP_ST *val;
	bool ret;

	if ( avp->data_type != AVP_TYPE_GROUP ) {
		debug(__func__, __LINE__,"invalid data type(%d)", avp->data_type);
		return false;
	}

	
	ret = avp_group_get(avp, avp_code, &val);
	if ( ret == false ) {
		debug(__func__, __LINE__,"%s AVP not found avp_code(%u)", avp->name, avp_code);
		return false;
	}

	ret = avp_set_data(val, data, len);
	if ( ret == true ) {
		avp->has = true;
	}
	return ret;
}

bool avp_group_set_data_int32(FLK_AVP_ST *avp, u_int avp_code, int val)
{
	return avp_group_set_data(avp, avp_code, (void *)&val, sizeof(val));
}
bool avp_group_set_data_uint32(FLK_AVP_ST *avp, u_int avp_code, u_int val)
{
	return avp_group_set_data(avp, avp_code, (void *)&val, sizeof(val));
}
bool avp_group_set_data_int64(FLK_AVP_ST *avp, u_int avp_code, long val)
{
	return avp_group_set_data(avp, avp_code, (void *)&val, sizeof(val));
}
bool avp_group_set_data_uint64(FLK_AVP_ST *avp, u_int avp_code, u_long val)
{
	return avp_group_set_data(avp, avp_code, (void *)&val, sizeof(val));
}
bool avp_group_set_data_float32(FLK_AVP_ST *avp, u_int avp_code, float val)
{
	return avp_group_set_data(avp, avp_code, (void *)&val, sizeof(val));
}
bool avp_group_set_data_float64(FLK_AVP_ST *avp, u_int avp_code, double val)
{
	return avp_group_set_data(avp, avp_code, (void *)&val, sizeof(val));
}
bool avp_group_set_data_enum(FLK_AVP_ST *avp, u_int avp_code, u_int val)
{
	return avp_group_set_data(avp, avp_code, (void *)&val, sizeof(val));
}
bool avp_group_set_data_cstr(FLK_AVP_ST *avp, u_int avp_code, char *val, int size)
{
	return avp_group_set_data(avp, avp_code, (void *)val, size);
}
bool avp_group_set_data_numstr(FLK_AVP_ST *avp, u_int avp_code, char *val, int size)
{
	return avp_group_set_data(avp, avp_code, (void *)val, size);
}
bool avp_group_set_data_octstr(FLK_AVP_ST *avp, u_int avp_code, Octstr *val)
{
	return avp_group_set_data(avp, avp_code, (void *)octstr_get_cstr(val), octstr_len(val));
}
bool avp_group_set_data_utf8str(FLK_AVP_ST *avp, u_int avp_code, char *val, int size)
{
	return avp_group_set_data(avp, avp_code, (void *)val, size);
}
bool avp_group_set_data_address(FLK_AVP_ST *avp, u_int avp_code, int ip_type, char *str_addr)
{
	AVP_ADDRESS_ST val;
	bool ret;

	ret = convert_address_to_net(&val, ip_type, str_addr);
	if ( ret == true ) {
		return avp_group_set_data(avp, avp_code, (void *)&val, sizeof(AVP_ADDRESS_ST));
	}else {
		return false;
	}
}
bool avp_group_set_data_time(FLK_AVP_ST *avp, u_int avp_code, u_int val)
{
	return avp_group_set_data(avp, avp_code, (void *)&val, sizeof(val));
}
bool avp_group_set_data_avplist(FLK_AVP_ST *avp, FLK_AVP_ST *val)
{
	/*
	FLK_AVP_ST *val;
	List *group;

	if ( avp->data_type != AVP_TYPE_GROUP ) {
		debug(__func__, __LINE__,"invalid data type(%d)", avp->data_type);
		return false;
	}

	if ( val->data_type == AVP_TYPE_LIST ) {
	}

	val = avp_group_get(avp, avp_code);
	if ( val == NULL ) {
		group = avp->data.data_group;
		gwlist_append(group, val);
	}else {
	}

	return avp_set_data(val, data, len);
	*/
	return true;
}

bool avp_group_get(FLK_AVP_ST *avp, u_int avp_code, FLK_AVP_ST **val)
{
	long i = 0;
	bool ret;
	FLK_AVP_ST *tmp = NULL;
	List *lst;

	ret = avp_get_data_group(avp, &lst);
	if ( ret == false ) {
		error(0, "[%s : %d] avp_get_data_group failed", __func__, __LINE__);
		return false;
	}

	ret = false;

	for ( i = 0; i < gwlist_len(lst); ++i ) {
		tmp = gwlist_get(lst, i);
		if ( tmp->avp_code == avp_code ) {
			ret = true;
			*val = tmp;
			break;
		}
	}

	return ret;
}

bool avp_group_get_data(FLK_AVP_ST *avp, u_int avp_code, void **data, int *len)
{
	FLK_AVP_ST *val;
	bool ret;

	ret = avp_group_get(avp, avp_code, &val);
	if ( ret == true ) {
		ret = avp_get_data(val, data, len);
	}

	return ret;
}

bool avp_group_get_data_int_type(FLK_AVP_ST *avp, u_int avp_code, void *val)
{
	void *ptr;
	int len;
	bool ret;

	ret = avp_group_get_data(avp, avp_code, &ptr, &len);
	if ( ret == true ) {
		memcpy(val, ptr, len);
		gw_free(ptr);
	}

	return ret;
}

bool avp_group_get_data_int32(FLK_AVP_ST *avp, u_int avp_code, int *val)
{
	return avp_group_get_data_int_type(avp, avp_code, (void *)val);
}

bool avp_group_get_data_uint32(FLK_AVP_ST *avp, u_int avp_code, u_int *val)
{
	return avp_group_get_data_int_type(avp, avp_code, (void *)val);
}

bool avp_group_get_data_int64(FLK_AVP_ST *avp, u_int avp_code, long *val)
{
	return avp_group_get_data_int_type(avp, avp_code, (void *)val);
}

bool avp_group_get_data_uint64(FLK_AVP_ST *avp, u_int avp_code, u_long *val)
{
	return avp_group_get_data_int_type(avp, avp_code, (void *)val);
}

bool avp_group_get_data_float32(FLK_AVP_ST *avp, u_int avp_code, float *val)
{
	return avp_group_get_data_int_type(avp, avp_code, (void *)val);
}

bool avp_group_get_data_float64(FLK_AVP_ST *avp, u_int avp_code, double *val)
{
	return avp_group_get_data_int_type(avp, avp_code, (void *)val);
}

bool avp_group_get_data_enum(FLK_AVP_ST *avp, u_int avp_code, u_int *val)
{
	return avp_group_get_data_int_type(avp, avp_code, (void *)val);
}

bool avp_group_get_data_cstr(FLK_AVP_ST *avp, u_int avp_code, char **val, int *len)
{
	return avp_group_get_data(avp, avp_code, (void *)val, len);
}

bool avp_group_get_data_numstr(FLK_AVP_ST *avp, u_int avp_code, char **val, int *len)
{
	return avp_group_get_data(avp, avp_code, (void *)val, len);
}

bool avp_group_get_data_octstr(FLK_AVP_ST *avp, u_int avp_code, Octstr **val)
{
	int len;
	char *ptr;
	bool ret;

	ret = avp_group_get_data(avp, avp_code, (void *)&ptr, &len);
	if ( ret == true ) {
		*val = octstr_create_from_data(ptr, len);
		gw_free(ptr);
	}

	return ret;
}

bool avp_group_get_data_utf8str(FLK_AVP_ST *avp, u_int avp_code, char **val, int *len)
{
	return avp_group_get_data(avp, avp_code, (void *)val, len);
}

bool avp_group_get_data_address(FLK_AVP_ST *avp, u_int avp_code, int *ip_type, char *str, int str_size)
{
	bool ret;
	int sz;
	AVP_ADDRESS_ST *addr;

	ret = avp_group_get_data(avp, avp_code, (void *)&addr, &sz);
	if ( ret == true ) {
		*ip_type = addr->addr_type;
		ret = convert_net_to_address(addr, str, str_size);
	}
	return ret;
}
bool avp_group_get_data_time(FLK_AVP_ST *avp, u_int avp_code, u_int *val)
{
	return avp_group_get_data_int_type(avp, avp_code, (void *)val);
}

bool avp_group_get_data_list(FLK_AVP_ST *avp, u_int avp_code, FLK_AVP_ST **val)
{
	FLK_AVP_ST *avplst;
	bool ret;
	
	ret = avp_group_get(avp, avp_code, &avplst);
	if ( ret == false ) {
		error(0, "[%s : %d] avp_group_get failed avp_code(%d)", __func__, __LINE__, avp_code);
		return false;
	}

	if ( avplst->data_type != AVP_TYPE_LIST ) {
		error(0, "[%s : %d] avp_group's avp is not AVP_TYPE_LIST (data_type : %d), avp_code(%d)", __func__, __LINE__, avplst->data_type, avp_code);
		return false;
	}

	*val = avplst;
		
	return true;
}

bool avp_group_append(FLK_AVP_ST *avp, FLK_AVP_ST *elem)
{
	bool ret = true;
	List *data;
	FLK_AVP_ST *val;

	ret = avp_get_data_group(avp, &data);
	if ( ret == false ) {
		error(0, "[%s : %d] avp_get_data_group failed", __func__, __LINE__);
		return false;
	}

	ret = avp_group_get(avp, elem->avp_code, &val);
	if ( ret == false ) {
		gwlist_append(data, elem);
		ret = true;
	}else {
		if ( val->data_type == AVP_TYPE_LIST ) {
			ret = avp_list_append(val, elem);
		}else {
			ret = false;
		}
	}
	return ret;
}

bool avp_group_append_avplist(FLK_AVP_ST *avp, u_int avp_code, int start_range, int end_range, int attr)
{
	bool ret;
	FLK_AVP_ST *sub = avp_list_create(avp_code, start_range, end_range, attr);
	if ( sub == NULL ) {
		error(0, "[%s : %d] avp_list_create failed avp_code(%d)", __func__, __LINE__, avp_code);
		return false;
	}
	ret = avp_group_append(avp, sub);
	if ( ret == false ) {
		avp_destroy(sub);
	}
	return ret;
}

bool avp_group_append_avp(FLK_AVP_ST *avp, u_int avp_code, int attr)
{
	bool ret;
	FLK_AVP_ST *sub = avp_create(avp_code, attr);
	if ( sub == NULL ) {
		return false;
	}
	ret = avp_group_append(avp, sub);
	if ( ret == false ) {
		avp_destroy(sub);
	}
	return ret;
}
