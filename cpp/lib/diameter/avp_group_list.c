#include "flk_dia.h"

bool avp_group_list_append(FLK_AVP_ST *avp, u_int avp_code, FLK_AVP_ST *val)
{
	FLK_AVP_ST *root;
	bool ret;

	ret = avp_group_get_data_list(avp, avp_code, &root);
	if ( ret == false ) {
		return false;
	}

	return avp_list_append(root, val);

}

bool avp_group_list_append_data(FLK_AVP_ST *avp, u_int avp_code, void *data, int len)
{
	FLK_AVP_ST *val;
	bool ret; 

	ret = avp_group_get_data_list(avp, avp_code, &val);
	if ( ret == false ) {
		error(0, "[%s : %d] avp_group_get_data_list failed", __func__, __LINE__);
		return false;
	}

	ret = avp_list_append_data(val, data, len);
	if ( ret == true ) {
		avp->has = true;
	}
	return ret;
}

bool avp_group_list_append_data_int32(FLK_AVP_ST *avp, u_int avp_code, int val)
{
	return avp_group_list_append_data(avp, avp_code, (void *)&val, sizeof(val));
}

bool avp_group_list_append_data_uint32(FLK_AVP_ST *avp, u_int avp_code, u_int val)
{
	return avp_group_list_append_data(avp, avp_code, (void *)&val, sizeof(val));
}

bool avp_group_list_append_data_int64(FLK_AVP_ST *avp, u_int avp_code, long val)
{
	return avp_group_list_append_data(avp, avp_code, (void *)&val, sizeof(val));
}
bool avp_group_list_append_data_uint64(FLK_AVP_ST *avp, u_int avp_code, u_long val)
{
	return avp_group_list_append_data(avp, avp_code, (void *)&val, sizeof(val));
}
bool avp_group_list_append_data_float32(FLK_AVP_ST *avp, u_int avp_code, float val)
{
	return avp_group_list_append_data(avp, avp_code, (void *)&val, sizeof(val));
}
bool avp_group_list_append_data_float64(FLK_AVP_ST *avp, u_int avp_code, double val)
{
	return avp_group_list_append_data(avp, avp_code, (void *)&val, sizeof(val));
}
bool avp_group_list_append_data_enum(FLK_AVP_ST *avp, u_int avp_code, u_int val)
{
	return avp_group_list_append_data(avp, avp_code, (void *)&val, sizeof(val));
}
bool avp_group_list_append_data_cstr(FLK_AVP_ST *avp, u_int avp_code, char *val, int len)
{
	return avp_group_list_append_data(avp, avp_code, (void *)val, len);
}
bool avp_group_list_append_data_numstr(FLK_AVP_ST *avp, u_int avp_code, char *val, int len)
{
	return avp_group_list_append_data(avp, avp_code, (void *)val, len);
}
bool avp_group_list_append_data_octstr(FLK_AVP_ST *avp, u_int avp_code, Octstr *val)
{
	return avp_group_list_append_data_octstr(avp, avp_code, val);
}
bool avp_group_list_append_data_utf8str(FLK_AVP_ST *avp, u_int avp_code, char *val, int len)
{
	return avp_group_list_append_data(avp, avp_code, (void *)val, len);
}
bool avp_group_list_append_data_address(FLK_AVP_ST *avp, u_int avp_code, int ip_type, char *str_addr)
{
	AVP_ADDRESS_ST addr;
	if ( convert_address_to_net(&addr, ip_type, str_addr) == false )
		return false;
	return avp_group_list_append_data(avp, avp_code, (void *)&addr, sizeof(AVP_ADDRESS_ST));
}
bool avp_group_list_append_data_time(FLK_AVP_ST *avp, u_int avp_code, u_int val)
{
	return avp_group_list_append_data(avp, avp_code, (void *)&val, sizeof(val));
}
// GET
bool avp_group_list_get(FLK_AVP_ST *avp, u_int avp_code, FLK_AVP_ST **avp_list)
{
	return avp_group_get_data_list(avp, avp_code, avp_list);
}

bool avp_group_list_get_avplist(FLK_AVP_ST *avp, u_int avp_code, List **val)
{
	FLK_AVP_ST *root;
	bool ret;

	ret = avp_group_list_get(avp, avp_code, &root);
	if ( ret == false ) {
		return false;
	}

	return avp_list_get_avplist(root, val);
}

#if 0
bool avp_group_list_get_datalist(FLK_AVP_ST *avp, u_int avp_code, int *data_type, List **val)
{
	FLK_AVP_ST *root;
	bool ret;

	ret = avp_group_list_get(avp, avp_code, &root);
	if ( ret == false ) {
		return false;
	}

	if ( root->data_type != AVP_TYPE_LIST ) {
		return false;
	}


	return avp_list_get_data_list(root, data_type, val);
}
#endif
