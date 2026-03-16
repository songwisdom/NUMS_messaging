#include "flk_dia.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

Octstr *convert_pointer_to_octstr(void *ptr)
{
	long addr = (long )ptr;
	return octstr_create_from_data((char *)&addr, sizeof(long));
}
void *convert_octstr_to_pointer(Octstr *ostr)
{
	long addr;
	void *ptr;

	ptr = (void *)octstr_get_cstr(ostr);
	memcpy(&addr, ptr, sizeof(long));

	//return ((void *)0 + addr);
	return (void *)(0 + addr);
}

void empty_avp_list(List *lst)
{
	FLK_AVP_ST *avp;

	if ( gwlist_len(lst) == 0 )
		return;
	
	while ( (avp = gwlist_extract_first(lst)) != NULL ) {
		avp_destroy(avp);
	}
	return;
}
void copy_avp_list(List *dest, List *src)
{
	int i;
	int len;
	FLK_AVP_ST *avp, *val;
	List *lst;

	len = gwlist_len(src);

	empty_avp_list(dest);

	for ( i = 0; i < len; ++i ) {
		avp = (FLK_AVP_ST *)gwlist_get(src, i);
		//debug(__func__, __LINE__, "[%d] avp(%s) has(%d)", i, avp->name, avp->avp_code);

		val = gw_malloc(sizeof(FLK_AVP_ST));
		memcpy(val, avp, sizeof(FLK_AVP_ST));
		gwlist_append(dest, val);

		switch ( avp->data_type ) {
			case AVP_TYPE_INT32 : 
			case AVP_TYPE_UINT32 : 
			case AVP_TYPE_FLOAT32 : 
			case AVP_TYPE_ENUM : 
			case AVP_TYPE_INT64 : 
			case AVP_TYPE_UINT64 : 
			case AVP_TYPE_FLOAT64 : 
			case AVP_TYPE_UTF8STRING : 
			case AVP_TYPE_OCTSTRING : 
			case AVP_TYPE_NUMSTRING : 
			case AVP_TYPE_ADDRESS :
			case AVP_TYPE_TIME :
			case AVP_TYPE_NONE :
				val->data = octstr_duplicate(avp->data);
				break;
			case AVP_TYPE_GROUP :
			case AVP_TYPE_LIST : 
				{
					lst = gwlist_create();
					List *src_list = (List *)convert_octstr_to_pointer(avp->data);
					copy_avp_list(lst, src_list);
					val->data = convert_pointer_to_octstr((void *)lst);
				}
				break;
		}
	}
}

void copy_avp(FLK_AVP_ST *dest, FLK_AVP_ST *src)
{
	List *lst, *src_list;

	if ( dest->data != NULL ) {
		switch (dest->data_type) {
			case AVP_TYPE_GROUP :
			case AVP_TYPE_LIST :
				lst = (List *)convert_octstr_to_pointer(dest->data);
				gwlist_destroy(lst, avp_destroy_item);
				octstr_destroy(dest->data);
				break;
			default :
				octstr_destroy(dest->data);
				break;
		}
	}

	memcpy(dest, src, sizeof(FLK_AVP_ST));
	switch ( src->data_type ) {
		case AVP_TYPE_GROUP :
		case AVP_TYPE_LIST : 
			{
				lst = gwlist_create();
				src_list = (List *)convert_octstr_to_pointer(src->data);
				copy_avp_list(lst, src_list);
				dest->data = convert_pointer_to_octstr((void *)lst);
			}
			break;
		default :
			dest->data = octstr_duplicate(src->data);
			break;
	}
}
FLK_AVP_ST *duplicate_avp(FLK_AVP_ST *src)
{
	FLK_AVP_ST *dest;

	List *lst, *src_list;

	dest = (FLK_AVP_ST *)gw_malloc(sizeof(FLK_AVP_ST));

	memcpy(dest, src, sizeof(FLK_AVP_ST));
	switch ( src->data_type ) {
		case AVP_TYPE_GROUP :
		case AVP_TYPE_LIST : 
			{
				lst = gwlist_create();
				src_list = (List *)convert_octstr_to_pointer(src->data);
				copy_avp_list(lst, src_list);
				dest->data = convert_pointer_to_octstr((void *)lst);
			}
			break;
		default :
			dest->data = octstr_duplicate(src->data);
			break;
	}
	return dest;
}

FLK_AVP_ST *extract_avp_list(List *lst, u_int avp_code)
{
	int len;
	int i;
	FLK_AVP_ST *ret = NULL;

	len = gwlist_len(lst);

	for ( i = 0; i < len; ++i ) {
		ret = (FLK_AVP_ST *)gwlist_get(lst, i);

		if ( ret->avp_code == avp_code ) {
			gwlist_delete(lst, i, 1);
			break;
		}else {
			ret = NULL;
		}
	}

	return ret;
}
Octstr *octstr_byte_order(Octstr *os)
{
	Octstr *ret;
	int len;
	int i;

#ifdef _HP_UX_MACHINE
	ret = octstr_duplicate(os);
#else
	len = octstr_len(os);

	ret = octstr_create("");

	for ( i = len; i > 0; --i ) {
		octstr_append_char(ret, octstr_get_char(os, i-1));
	}
#endif
	return ret;
}

char *get_data_type_name(int type)
{
	switch ( type ) {
		case AVP_TYPE_NONE : return "NONE";
		case AVP_TYPE_INT32 : return "INT32";
		case AVP_TYPE_UINT32 : return "UINT32";
		case AVP_TYPE_INT64 : return "INT64";
		case AVP_TYPE_UINT64 : return "UINT64";
		case AVP_TYPE_FLOAT32 : return "FLOAT32";
		case AVP_TYPE_FLOAT64 : return "FLOAT64";
		case AVP_TYPE_ENUM : return "ENUM";
		case AVP_TYPE_UTF8STRING : return "UTF8STRING";
		case AVP_TYPE_OCTSTRING : return "OCTSTRING";
		case AVP_TYPE_NUMSTRING : return "NUMSTRING";
		case AVP_TYPE_GROUP :return "GROUP";
		case AVP_TYPE_LIST :return "LIST";
		case AVP_TYPE_ADDRESS : return "ADDRESS";
		case AVP_TYPE_TIME : return "TIME";
		default : return "UNKNOWN";
	}
}

char *get_attr_name(int attr)
{
	switch(attr) {
		case AVP_ATTR_FIX : return "FIX";
		case AVP_ATTR_MANDANTORY : return "MANDANTORY";
		case AVP_ATTR_OPTIONAL : return "OPTIONAL";
		case AVP_ATTR_LIST_MANDANTORY : return "LIST_MANDANTORY";
		case AVP_ATTR_LIST_OPTIONAL : return "LIST_OPTIONAL";
		case AVP_ATTR_NONE : 
		default : return "NONE";
	}
}

bool convert_net_to_address(AVP_ADDRESS_ST *addr, char *str, int str_size)
{
	bool ret = false;
	switch ( addr->addr_type ) {
		case ADDR_TYPE_IPV4 :
			if ( inet_ntop(AF_INET, (void *)&addr->addr.ipv4, str, str_size) == NULL ) {
				error(errno, "[%s : %d] IPV4 inet_ntop failed", __func__, __LINE__);
			}else {
				ret = true;
			}
			break;
		case ADDR_TYPE_IPV6 :
			if ( inet_ntop(AF_INET6, (void *)&addr->addr.ipv6, str, str_size) == NULL ) {
				error(errno, "[%s : %d] IPV6 inet_ntop failed", __func__, __LINE__);
			}else {
				ret = true;
			}
			break;
	}
	return ret;
}


bool convert_address_to_net(AVP_ADDRESS_ST *addr, int ip_type, char *str_addr)
{
	bool ret = false;
	struct in_addr in_ipv4;
	//struct in6_addr in_ipv6;

	addr->addr_type = ip_type;
	switch ( ip_type ) {
		case ADDR_TYPE_IPV4 :
			if ( inet_pton(AF_INET, str_addr, (void *)&in_ipv4) == 1 ) {
				memcpy(&addr->addr.ipv4, &in_ipv4.s_addr, sizeof(in_ipv4.s_addr));
				ret = true;
			}else {
				error(errno, "IPV4 inet_pton failed (address : %s)", str_addr);
			}
			break;
#if 0
		case ADDR_TYPE_IPV6 :
			if ( inet_pton(AF_INET6, str_addr, (void *)&in_ipv6) == 1 ) {
				memcpy(addr->addr.ipv6, in_ipv6.in6_u.u6_addr8, sizeof(in_ipv6.in6_u.u6_addr8));
				//memcpy(addr->addr.ipv6, in_ipv6.__in6_u.__u6_addr8, sizeof(in_ipv6.__in6_u.__u6_addr8));
				ret = true;
			}else {
				error(errno, "IPV6 inet_pton failed (address : %s)", str_addr);
			}
#endif
	}
	return ret;
}

