#include "flk_dia.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
extern int errno;

#define AVP_UNKNOWN_NAME "UNKNOWN AVP"

void set_avp_header(AVP_HEADER_ST *header, int avp_code, int vendor_id, int mbit, int pbit)
{
	header->avp_code = avp_code;
	header->vendor_id = vendor_id;
	header->opt.bit.vbit = (vendor_id == 0 ? 0 : 1);
	header->opt.bit.mbit = (mbit == 0 ? 0 : 1);
	header->opt.bit.pbit = (pbit == 0 ? 0 : 1);
}

static void avp_set_init(FLK_AVP_ST *avp, int avp_code, int vendor_id, int mbit, int pbit, int data_type, char *name, int attr)
{
	static const char null_buff[8] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
	set_avp_header(&avp->header, avp_code, vendor_id, mbit, pbit);
	avp->avp_code = avp_code;
	avp->has = false;
	avp->data_type = data_type;
	avp->attr = attr;
	avp->name = name;
	avp->data = NULL;
	List *lst;

	// TODO 쨊ata 를 미리 생성할것인가... 설정될때 할것인가 추후 다시 고민할것... 
	switch ( data_type ) {
		case AVP_TYPE_INT32 : 
		case AVP_TYPE_UINT32 : 
		case AVP_TYPE_FLOAT32 : 
		case AVP_TYPE_ENUM : 
		case AVP_TYPE_TIME :
			avp->data = octstr_create_from_data(null_buff, 4);
			break;
		case AVP_TYPE_INT64 : 
		case AVP_TYPE_UINT64 : 
		case AVP_TYPE_FLOAT64 : 
			avp->data = octstr_create_from_data(null_buff, 8);
			break;
		case AVP_TYPE_GROUP :
		case AVP_TYPE_LIST :
			lst = gwlist_create();
			avp->data = convert_pointer_to_octstr((void *)lst);
			break;
		case AVP_TYPE_ADDRESS : 
			{
				AVP_ADDRESS_ST addr;
				memset(&addr, 0x00, sizeof(addr));
				avp->data = octstr_create_from_data((char *)&addr, sizeof(addr));
			}
			break;

		case AVP_TYPE_UTF8STRING : 
		case AVP_TYPE_OCTSTRING : 
		case AVP_TYPE_NUMSTRING : 
		case AVP_TYPE_NONE :
		default :
			avp->data = octstr_create("");
			break;
	}
}

FLK_AVP_ST *avp_create_empty(int avp_code, int vid, int mbit, int pbit, int data_type, int attr)
{
	FLK_AVP_ST *avp;
	avp = gw_malloc(sizeof(FLK_AVP_ST));
	memset(avp, 0x00, sizeof(FLK_AVP_ST));
	avp_set_init(avp, avp_code, vid, mbit, pbit, data_type, AVP_UNKNOWN_NAME, attr); 
	return avp;
}

FLK_AVP_ST *avp_create(int avp_code, int attr)
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
		if (avp_group_append_avplist(avp, avp_##name, start_range, end_range, AVP_ATTR_LIST_MANDANTORY) == false) break;
#define INTF_AVP_FIX(name)
#define INTF_AVP_OPTIONAL(name)
#define INTF_AVP_MANDANTORY(name)
#define INTF_AVP_LIST_OPTIONAL(name, start_range, end_range)
#define INTF_AVP_LIST_MANDANTORY(name, start_range, end_range)
#define DIAMETER_INTF(name, desc, command_code, rbit, pbit, app_id, fields)
#define AVP_DEF(avpname, full_name, avpcode, vid, mbit, pbit, avptype, fields) case avpcode : { \
		avp_set_init(avp, avpcode, vid, mbit, pbit, avptype, #full_name, attr); \
		fields \
		break; \
		}
#include "./diameter.def"
		default : 
			avp->avp_code = avp_code;
			avp->name = AVP_UNKNOWN_NAME;
			avp->data_type = AVP_TYPE_OCTSTRING;
			avp->attr = attr;
			avp->data = octstr_create("");
			break;
	}
	return avp;
}

void avp_destroy_item(void *val)
{
	avp_destroy((FLK_AVP_ST *)val);
}

void avp_destroy(FLK_AVP_ST *avp)
{
	List *lst;

	if ( avp->data == NULL )
		return;

	switch (avp->data_type) {
		case AVP_TYPE_GROUP :
		case AVP_TYPE_LIST :
			lst = (List *)convert_octstr_to_pointer(avp->data);
			gwlist_destroy(lst, avp_destroy_item);
			octstr_destroy(avp->data);
			break;
		default :
			octstr_destroy(avp->data);
			break;
	}
	gw_free(avp);
}

bool avp_set(FLK_AVP_ST *avp, FLK_AVP_ST *val)
{
	bool ret = true;

	if ( val->avp_code != avp->avp_code ) {
		return false;
	}

	if ( (val->data_type != AVP_TYPE_NONE) && (val->data_type != avp->data_type) ) {
		return false;
	}

	avp->header = val->header;

	switch ( avp->data_type ) {
		case AVP_TYPE_INT32 : 
		case AVP_TYPE_UINT32 : 
		case AVP_TYPE_FLOAT32 : 
		case AVP_TYPE_ENUM : 
		case AVP_TYPE_INT64 : 
		case AVP_TYPE_UINT64 : 
		case AVP_TYPE_FLOAT64 : 
		case AVP_TYPE_TIME :
			memcpy(octstr_get_cstr(avp->data), octstr_get_cstr(val->data), octstr_len(avp->data));
			break;
		case AVP_TYPE_UTF8STRING : 
		case AVP_TYPE_OCTSTRING : 
		case AVP_TYPE_NUMSTRING : 
		case AVP_TYPE_NONE : 
			{
				if ( octstr_len(avp->data) > 0 ) {
					octstr_delete(avp->data, 0, octstr_len(avp->data));
				}
				octstr_append(avp->data, val->data);
			}
			break;
		case AVP_TYPE_ADDRESS :
			memcpy(octstr_get_cstr(avp->data), octstr_get_cstr(val->data), sizeof(AVP_ADDRESS_ST));
			break;
		case AVP_TYPE_GROUP :
		case AVP_TYPE_LIST :
			{ 
				List *src, *dest;
				dest = convert_octstr_to_pointer(avp->data);
				src = convert_octstr_to_pointer(val->data);
				copy_avp_list(dest, src);
			}
			break;
		default :
			error(0,"[%s : %d] avp(%s) invalid data_type(%d : %s)",
					__func__, __LINE__, avp->name, avp->data_type, get_data_type_name(avp->data_type));
			ret = false;
			break;
	}

	avp->has = ret;

	return ret;
}
/*
 * AVP_TYPE_GROUP 인 경우 void *data = List * 형이 들어가야함
 * 구현이 이상하긴 한데 방법이 없네요 -0- 제길..
 *
 */
bool avp_set_data(FLK_AVP_ST *avp, void *data, int len)
{
	bool ret = true;
	switch ( avp->data_type ) {
		case AVP_TYPE_INT32 : 
		case AVP_TYPE_UINT32 : 
		case AVP_TYPE_FLOAT32 : 
		case AVP_TYPE_ENUM : 
		case AVP_TYPE_TIME :
			if ( len != 4 ) return false;
			memcpy(octstr_get_cstr(avp->data), data, len);
			break;
		case AVP_TYPE_INT64 : 
		case AVP_TYPE_UINT64 : 
		case AVP_TYPE_FLOAT64 : 
			if ( len != 8 ) return false;
			memcpy(octstr_get_cstr(avp->data), data, len);
			break;
		case AVP_TYPE_UTF8STRING : 
		case AVP_TYPE_OCTSTRING : 
		case AVP_TYPE_NUMSTRING : 
		case AVP_TYPE_NONE : 
			{
				if ( octstr_len(avp->data) > 0 ) {
					octstr_delete(avp->data, 0, octstr_len(avp->data));
				}
				octstr_append_data(avp->data, data, len);
			}
			break;
		case AVP_TYPE_ADDRESS :
			if ( len != sizeof(AVP_ADDRESS_ST) ) {
				return false;
			}else {
				memcpy(octstr_get_cstr(avp->data), data, len);
			}
			break;
		case AVP_TYPE_GROUP :
		case AVP_TYPE_LIST :
		default :
			error(0,"[%s : %d] avp(%s) invalid data_type(%d : %s)",
					__func__, __LINE__, avp->name, avp->data_type, get_data_type_name(avp->data_type));
			ret = false;
			break;
	}

	avp->has = ret;

	return ret;
}

bool avp_set_data_time(FLK_AVP_ST *avp, u_int val)
{
	return avp_set_data(avp, (void *)&val, sizeof(val));
}
bool avp_set_data_int32(FLK_AVP_ST *avp, int val)
{
	return avp_set_data(avp, (void *)&val, sizeof(val));
}
bool avp_set_data_uint32(FLK_AVP_ST *avp, u_int val)
{
	return avp_set_data(avp, (void *)&val, sizeof(val));
}
bool avp_set_data_int64(FLK_AVP_ST *avp, long val)
{
	return avp_set_data(avp, (void *)&val, sizeof(val));
}
bool avp_set_data_uint64(FLK_AVP_ST *avp, u_long val)
{
	return avp_set_data(avp, (void *)&val, sizeof(val));
}
bool avp_set_data_float32(FLK_AVP_ST *avp, float val)
{
	return avp_set_data(avp, (void *)&val, sizeof(val));
}
bool avp_set_data_float64(FLK_AVP_ST *avp, double val)
{
	return avp_set_data(avp, (void *)&val, sizeof(val));
}
bool avp_set_data_enum(FLK_AVP_ST *avp, u_int val)
{
	return avp_set_data(avp, (void *)&val, sizeof(val));
}
bool avp_set_data_cstr(FLK_AVP_ST *avp, char *val, int len)
{
	return avp_set_data(avp, (void *)val, len);
}
bool avp_set_data_numstr(FLK_AVP_ST *avp, char *val, int len)
{
	return avp_set_data(avp, (void *)val, len);
}
bool avp_set_data_octstr(FLK_AVP_ST *avp, Octstr *ostr)
{
	return avp_set_data(avp, (void *)octstr_get_cstr(ostr), octstr_len(ostr));
}
bool avp_set_data_utf8str(FLK_AVP_ST *avp, char *val, int len)
{
	return avp_set_data(avp, (void *)val, len);
}
bool avp_set_data_address(FLK_AVP_ST *avp, int ip_type, char *address)
{
	bool ret = false;
	struct in_addr in_ipv4;
	//struct in6_addr in_ipv6;
	//int inet_aton(const char *cp, struct in_addr *inp);
	AVP_ADDRESS_ST addr;
	memset(&addr, 0x00, sizeof(addr));
	addr.addr_type = ip_type;
	switch ( ip_type ) {
		case ADDR_TYPE_IPV4 :
			if ( inet_pton(AF_INET, address, (void *)&in_ipv4) == 1 ) {
				memcpy(&addr.addr.ipv4, &in_ipv4.s_addr, sizeof(in_ipv4.s_addr));
				ret = avp_set_data(avp, &addr, sizeof(addr));
			}else {
				error(errno, "[%s : %d] avp(%s) IPV4 inet_pton failed (address : %s)", 
						__func__, __LINE__, avp->name, address);
			}
			break;
#if 0
		case ADDR_TYPE_IPV6 :
			if ( inet_pton(AF_INET6, address, (void *)&in_ipv6) == 1 ) {
				memcpy(addr.addr.ipv6, in_ipv6.in6_u.u6_addr8, sizeof(in_ipv6.in6_u.u6_addr8));
				//memcpy(addr.addr.ipv6, in_ipv6.__in6_u.__u6_addr8, sizeof(in_ipv6.__in6_u.__u6_addr8));
				ret = avp_set_data(avp, &addr, sizeof(addr));
			}else {
				error(errno, "IPV6 inet_pton failed (address : %s)", address);
			}
#endif
	}

	return ret;
}

bool avp_set_data_inaddr(FLK_AVP_ST *avp, int ip_type, void *in_addr, int size)
{
	AVP_ADDRESS_ST addr;
	memset(&addr, 0x00, sizeof(addr));
	addr.addr_type = ip_type;

	if ( size > sizeof(addr.addr.reserved) )  {
		error(0, "[%s : %d] %s in_addr overlength(%d)", __func__, __LINE__, avp->name, size);
		return false;
	}

	memcpy(addr.addr.reserved, in_addr, size);
	
	return avp_set_data(avp, (char *)&addr, sizeof(addr));

}

/*
bool avp_set_data_group(FLK_AVP_ST *avp, List *group_avp)
{
	List *tmp;
	long addr;

	if ( avp->data_type != AVP_TYPE_GROUP ) {
		return false;
	}

	tmp = (List *)convert_octstr_to_pointer(avp->data);

	addr = (long)group_avp;
	memcpy(octstr_get_cstr(avp->data), &addr, sizeof(long));

	gwlist_destroy(tmp, avp_destroy_item);
	avp->has = true;

	return true;
}
*/
bool avp_set_data_group(FLK_AVP_ST *avp, List *group_avp)
{
	List *tmp;
	long addr;
	int i; int len;
	FLK_AVP_ST *val;

	if ( avp->data_type != AVP_TYPE_GROUP ) {
		return false;
	}

	tmp = (List *)convert_octstr_to_pointer(avp->data);

	len = gwlist_len(group_avp);
	for ( i = 0; i < len; ++i ) {
		val = gwlist_get(group_avp, i);
	}

	addr = (long)group_avp;
	memcpy(octstr_get_cstr(avp->data), &addr, sizeof(long));

	gwlist_destroy(tmp, avp_destroy_item);
	avp->has = true;

	return true;
}

bool avp_get_data(FLK_AVP_ST *avp, void **data, int *len)
{
	void 	*ptr = NULL;
	char 	*tmp;
	int 	size = 0;

	if ( avp->has == false ) 
		return false;

	switch ( avp->data_type ) {
		case AVP_TYPE_INT32 : 
		case AVP_TYPE_UINT32 : 
		case AVP_TYPE_FLOAT32 : 
		case AVP_TYPE_ENUM : 
		case AVP_TYPE_INT64 : 
		case AVP_TYPE_UINT64 : 
		case AVP_TYPE_FLOAT64 : 
		case AVP_TYPE_TIME :
			size = octstr_len(avp->data);
			ptr = (void *)octstr_get_cstr(avp->data);
			break;
		case AVP_TYPE_UTF8STRING : 
		case AVP_TYPE_OCTSTRING : 
		case AVP_TYPE_NUMSTRING : 
		case AVP_TYPE_NONE :
		case AVP_TYPE_ADDRESS :
			ptr = (void *)octstr_get_cstr(avp->data);
			size = octstr_len(avp->data);
			break;
		case AVP_TYPE_GROUP :
		case AVP_TYPE_LIST :
		default :
			error(0,"[%s : %d] avp(%s) invalid data_type(%d : %s)",
					__func__, __LINE__, avp->name, avp->data_type, get_data_type_name(avp->data_type));
			return false;
	}

	tmp = gw_malloc(size+1);
	memcpy(tmp, ptr, size);
	tmp[size] = '\0';
	*len = size;
	*data = tmp;

	return true;
}

bool avp_get_data_int_type(FLK_AVP_ST *avp, void *val)
{
	void 	*ptr = NULL;

	if ( avp->has == false ) 
		return false;

	switch ( avp->data_type ) {
		case AVP_TYPE_INT32 : 
		case AVP_TYPE_UINT32 : 
		case AVP_TYPE_FLOAT32 : 
		case AVP_TYPE_ENUM : 
		case AVP_TYPE_INT64 : 
		case AVP_TYPE_UINT64 : 
		case AVP_TYPE_FLOAT64 : 
		case AVP_TYPE_TIME :
			ptr = octstr_get_cstr(avp->data);
			memcpy(val, ptr, octstr_len(avp->data));
			return true;
		default :
			error(0,"[%s : %d] avp(%s) invalid data_type(%d : %s)",
					__func__, __LINE__, avp->name, avp->data_type, get_data_type_name(avp->data_type));
			return false;
	}
}


bool avp_get_data_int32(FLK_AVP_ST *avp, int *val) 
{
	return avp_get_data_int_type(avp, (void *)val);
}

bool avp_get_data_uint32(FLK_AVP_ST *avp, u_int *val) 
{
	return avp_get_data_int_type(avp, (void *)val);
}

bool avp_get_data_int64(FLK_AVP_ST *avp, long *val)
{
	return avp_get_data_int_type(avp, (void *)val);
}

bool avp_get_data_uint64(FLK_AVP_ST *avp, u_long *val)
{
	return avp_get_data_int_type(avp, (void *)val);
}

bool avp_get_data_float32(FLK_AVP_ST *avp, float *val)
{
	return avp_get_data_int_type(avp, (void *)val);
}

bool avp_get_data_float64(FLK_AVP_ST *avp, double *val)
{
	return avp_get_data_int_type(avp, (void *)val);
}

bool avp_get_data_enum(FLK_AVP_ST *avp, u_int *val)
{
	return avp_get_data_int_type(avp, (void *)val);
}

bool avp_get_data_cstr(FLK_AVP_ST *avp, char **val, int *len)
{
	return avp_get_data(avp, (void **)val, len);
}

bool avp_get_data_numstr(FLK_AVP_ST *avp, char **val, int *len)
{
	return avp_get_data(avp, (void **)val, len);
}

bool avp_get_data_octstr(FLK_AVP_ST *avp, Octstr **ostr)
{
	if ( avp->has == false ) 
		return false;

	switch ( avp->data_type ) {
		case AVP_TYPE_UTF8STRING : 
		case AVP_TYPE_OCTSTRING : 
		case AVP_TYPE_NUMSTRING : 
		case AVP_TYPE_NONE :
			*ostr = octstr_duplicate(avp->data);
			return true;
		default :
			error(0,"[%s : %d] avp(%s) invalid data_type(%d : %s)",
					__func__, __LINE__, avp->name, avp->data_type, get_data_type_name(avp->data_type));
			return false;
	}
}

bool avp_get_data_utf8str(FLK_AVP_ST *avp, char **val, int *len)
{
	return avp_get_data(avp, (void **)val, len);
}

bool avp_get_data_group(FLK_AVP_ST *avp, List **val) 
{
	List *lst;

	if ( avp->data_type != AVP_TYPE_GROUP ) {
		error(0,"[%s : %d] avp(%s) invalid data_type(%d : %s)",
				__func__, __LINE__, avp->name, avp->data_type, get_data_type_name(avp->data_type));
		return false;
	}

	/* TODO 고민중... 
	if ( avp->has == false ) {
		return false;
	}
	*/

	lst = (List *)convert_octstr_to_pointer(avp->data);
	*val = lst;

	return true;
}


bool avp_get_data_address(FLK_AVP_ST *avp, int *ip_type, char *address, int addr_size)
{
	AVP_ADDRESS_ST *addr;
	bool ret = false;
	if ( avp->has == false ) {
		return false;
	}

	addr = (AVP_ADDRESS_ST *)octstr_get_cstr(avp->data);

	ret = convert_net_to_address(addr, address, addr_size);
	if ( ret == true ) {
		*ip_type = addr->addr_type;
	}

	return ret;
}
bool avp_get_data_time(FLK_AVP_ST *avp, u_int *val)
{
	return avp_get_data_int_type(avp, (void *)val);
}
