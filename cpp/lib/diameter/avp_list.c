#include "flk_dia.h"

static void avp_list_set_init(FLK_AVP_ST *avp, int avp_code, char *name, int start_range, int end_range, int attr)
{
	List *lst;
	avp->avp_code = avp_code;
	avp->has = false;
	avp->data_type = AVP_TYPE_LIST;
	avp->attr = attr;
	avp->start_range = start_range;
	avp->end_range = end_range;
	avp->name = name;
	lst = gwlist_create();
	avp->data = convert_pointer_to_octstr((void *)lst);
}

/*
FLK_AVP_ST *avp_list_create(int avp_code, int start_range, int end_range, int attr)
{
	FLK_AVP_ST *avp;
	avp = gw_malloc(sizeof(FLK_AVP_ST));
	memset(avp, 0x00, sizeof(FLK_AVP_ST));

	switch(avp_code) {
#define AVP_ENUM_TYPE(enum_name, code)
#define AVP_GROUP_MANDANTORY(name) \
		if (avp_group_append_avp(avp, avp_##name, AVP_ATTR_MANDANTORY) == false) break;
#define AVP_GROUP_OPTIONAL(name) \
		if (avp_group_append_avp(avp, avp_##name, AVP_ATTR_OPTIONAL) == false) break;
#define AVP_GROUP_LIST_OPTIONAL(name, start_range, end_range) \
		if (avp_group_append_avplist(avp, avp_##name, start_range, end_range, AVP_ATTR_LIST_OPTIONAL) == false) break;
#define AVP_GROUP_LIST_MANDANTORY(name, start_range, end_range) \
		if ( avp_group_append_avplist(avp, avp_##name, start_range, end_range, AVP_ATTR_LIST_MANDANTORY) == false) break;
#define INTF_AVP_FIX(name)
#define INTF_AVP_OPTIONAL(name)
#define INTF_AVP_MANDANTORY(name)
#define INTF_AVP_LIST_OPTIONAL(name, start_range, end_range)
#define INTF_AVP_LIST_MANDANTORY(name, start_range, end_range)
#define DIAMETER_INTF(name, desc, command_code, rbit, pbit, app_id, fields)
#define AVP_DEF(avpname, avpcode, vid, mbit, pbit, avptype, fields) case avpcode : { \
		avp_list_set_init(avp, avpcode, #avpname, start_range, end_range, attr); \
		fields \
		break; \
		}
#include "./diameter.def"
		default : 
			gw_free(avp);
			avp = NULL;
			break;
	}
	return avp;
}
*/
// List type 은 껍데기만 설정... 
FLK_AVP_ST *avp_list_create(int avp_code, int start_range, int end_range, int attr)
{
	FLK_AVP_ST *avp;
	avp = gw_malloc(sizeof(FLK_AVP_ST));
	memset(avp, 0x00, sizeof(FLK_AVP_ST));

	switch(avp_code) {
#define AVP_ENUM_TYPE(enum_name, code)
#define AVP_GROUP_MANDANTORY(name) 
#define AVP_GROUP_OPTIONAL(name)
#define AVP_GROUP_LIST_OPTIONAL(name, start_range, end_range)
#define AVP_GROUP_LIST_MANDANTORY(name, start_range, end_range) 
#define INTF_AVP_FIX(name)
#define INTF_AVP_OPTIONAL(name)
#define INTF_AVP_MANDANTORY(name)
#define INTF_AVP_LIST_OPTIONAL(name, start_range, end_range)
#define INTF_AVP_LIST_MANDANTORY(name, start_range, end_range)
#define DIAMETER_INTF(name, desc, command_code, rbit, pbit, app_id, fields)
#define AVP_DEF(avpname, full_name, avpcode, vid, mbit, pbit, avptype, fields) case avpcode : { \
		avp_list_set_init(avp, avpcode, #full_name, start_range, end_range, attr); \
		fields \
		break; \
		}
#include "./diameter.def"
		default : 
			gw_free(avp);
			avp = NULL;
			break;
	}
	return avp;
}

bool avp_list_append(FLK_AVP_ST *avp, FLK_AVP_ST *val)
{
	List *lst;

	if ( avp->avp_code != val->avp_code ) {
		error(0, "[%s : %d] avp_code(%s) org(%d) vs val(%d)invalid", 
				__func__, __LINE__, avp->name, avp->avp_code, val->avp_code);
		return false;
	}

	if ( avp_list_get_avplist(avp, &lst) == false )  {
		error(0, "[%s : %d] avp_code(%s) avp_list_get_avplist failed",
				__func__, __LINE__, avp->name);
		return false;
	}

	if ( (avp->end_range != 0) && (avp->end_range < gwlist_len(lst)) )  {
		error(0, "[%s : %d] avp %s range invalid end_range(%d) list_len(%ld)",
				__func__, __LINE__, avp->name, avp->end_range, gwlist_len(lst));
		return false;
	}

	avp->has = true;
	gwlist_append(lst, val);

	return true;
}

bool avp_list_append_data(FLK_AVP_ST *avp, void *data, int len)
{
	FLK_AVP_ST *val;
	List *lst;

	if ( avp_list_get_avplist(avp, &lst) == false ) {
		error(0, "[%s : %d] avp_list_get_avplist failed", __func__, __LINE__);
		return false;
	}

	if ( (avp->end_range != 0) && (avp->end_range < gwlist_len(lst)) )  {
		error(0, "[%s : %d] end_range invalide", __func__, __LINE__);
		return false;
	}

	val = avp_create(avp->avp_code, avp->attr);
	if ( val == NULL ) {
		error(0, "[%s : %d] avp_create failed : avp_code(%d)", __func__, __LINE__, avp->avp_code);
		return false;
	}

	if ( avp_set_data(val, data, len) == false ) {
		error(0, "[%s : %d] avp_set_data failed : avp_code(%d)", __func__, __LINE__, avp->avp_code);
		avp_destroy(val);
		return false;
	}

	gwlist_append(lst, val);
	avp->has = true;

	return true;
}

bool avp_list_append_data_int32(FLK_AVP_ST *avp, int val)
{
	return avp_list_append_data(avp, (void *)&val, sizeof(val));
}
bool avp_list_append_data_uint32(FLK_AVP_ST *avp, u_int val)
{
	return avp_list_append_data(avp, (void *)&val, sizeof(val));
}
bool avp_list_append_data_int64(FLK_AVP_ST *avp, long val)
{
	return avp_list_append_data(avp, (void *)&val, sizeof(val));
}
bool avp_list_append_data_uint64(FLK_AVP_ST *avp, u_long val)
{
	return avp_list_append_data(avp, (void *)&val, sizeof(val));
}
bool avp_list_append_data_float32(FLK_AVP_ST *avp, float val)
{
	return avp_list_append_data(avp, (void *)&val, sizeof(val));
}
bool avp_list_append_data_float64(FLK_AVP_ST *avp, double val)
{
	return avp_list_append_data(avp, (void *)&val, sizeof(val));
}
bool avp_list_append_data_enum(FLK_AVP_ST *avp, u_int val)
{
	return avp_list_append_data(avp, (void *)&val, sizeof(val));
}
bool avp_list_append_data_cstr(FLK_AVP_ST *avp, char *val, int len)
{
	return avp_list_append_data(avp, (void *)val, len);
}
bool avp_list_append_data_numstr(FLK_AVP_ST *avp, char *val, int len)
{
	return avp_list_append_data(avp, (void *)val, len);
}
bool avp_list_append_data_octstr(FLK_AVP_ST *avp, Octstr *val)
{
	return avp_list_append_data(avp, (void *)octstr_get_cstr(val), octstr_len(val));
}
bool avp_list_append_data_utf8str(FLK_AVP_ST *avp, char *val, int len)
{
	return avp_list_append_data(avp, (void *)val, len);
}
bool avp_list_append_data_address(FLK_AVP_ST *avp, u_int ip_type, char *str_addr)
{
	AVP_ADDRESS_ST addr;
	if ( convert_address_to_net(&addr, ip_type, str_addr) == false )
		return false;

	return avp_list_append_data(avp, (void *)&addr, sizeof(AVP_ADDRESS_ST));
}
bool avp_list_append_data_time(FLK_AVP_ST *avp, u_int val)
{
	return avp_list_append_data(avp, (void *)&val, sizeof(val));
}

int avp_list_avp_count(FLK_AVP_ST *avp)
{
	List *lst;

	if ( avp_list_get_avplist(avp, &lst) == false ) {
		return -1;
	}

	return gwlist_len(lst);
}

bool avp_list_get_avplist(FLK_AVP_ST *avp, List **lst)
{
	if ( avp->data_type != AVP_TYPE_LIST ) {
		return false;
	}

	*lst = convert_octstr_to_pointer(avp->data);
		
	return true;
}

bool avp_list_get_avp(FLK_AVP_ST *avp, int idx, FLK_AVP_ST **elem)
{
	FLK_AVP_ST *val;
	List *lst;
	bool ret;

	ret = avp_list_get_avplist(avp, &lst);
	if ( ret == false )  {
		return false;
	}

	if ( idx >= gwlist_len(lst) ) {
		return false;
	}
		
	val = gwlist_get(lst, idx);
	if ( val == NULL ) {
		return false;
	}

	*elem = val;

	return true;
}

bool avp_list_get_data(FLK_AVP_ST *avp, int idx, void **data, int *len)
{
	FLK_AVP_ST *val;
	bool ret;

	ret = avp_list_get_avp(avp, idx, &val);
	if ( ret == false ) {
		return false;
	}

	return avp_get_data(val, data, len);
}


