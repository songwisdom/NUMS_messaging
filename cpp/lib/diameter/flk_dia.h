#ifndef _H_FLK_DIAMETER_
#define _H_FLK_DIAMETER_

#include "gwlib.h"
#include <errno.h>
#include "dia_intf.h"
#include "dia_errcode.h"
//#include "xhash.h"
#define DIAMETER_HEADER_SIZE 20
#define AVP_MIN_SIZE 8
#define MAKE_DIA_CMD_CODE(cmd_code, req_bit) (cmd_code + (req_bit << 24))

typedef union {
	u_char val;
	struct {
#ifdef _HP_UX_MACHINE
		unsigned rbit : 1;
		unsigned pbit : 1;
		unsigned ebit : 1;
		unsigned tbit : 1;
		unsigned reserved : 4;
#else
		unsigned reserved : 4;
		unsigned tbit : 1;
		unsigned ebit : 1;
		unsigned pbit : 1;
		unsigned rbit : 1;
#endif
	}bit;
}DIAMETER_BIT_ST;

typedef struct {
	int version;
	int msg_len;
	DIAMETER_BIT_ST opt;
	u_int command_code;
	u_int application_id;
	u_int hop_by_hop_id;
	u_int end_to_end_id;
}DIAMETER_HEADER_ST;

typedef enum {
	AVP_ATTR_NONE,
	AVP_ATTR_FIX,
	AVP_ATTR_MANDANTORY,
	AVP_ATTR_OPTIONAL,
	AVP_ATTR_LIST_MANDANTORY,
	AVP_ATTR_LIST_OPTIONAL,
	AVP_ATTR_MAX
}AVP_ATTR_ENUM;

typedef enum {
	AVP_TYPE_NONE, // OCT
	AVP_TYPE_INT32, 
	AVP_TYPE_UINT32, 
	AVP_TYPE_INT64, 
	AVP_TYPE_UINT64, 
	AVP_TYPE_FLOAT32, 
	AVP_TYPE_FLOAT64, 
	AVP_TYPE_ENUM, 
	AVP_TYPE_UTF8STRING, 
	AVP_TYPE_OCTSTRING, 
	AVP_TYPE_NUMSTRING, 
	AVP_TYPE_GROUP,
	AVP_TYPE_LIST,
	AVP_TYPE_MAX,
	AVP_TYPE_TIME,
	AVP_TYPE_ADDRESS
}ENUM_AVP_TYPE;


typedef union {
	u_char val;
	struct {
#ifdef _HP_UX_MACHINE
		unsigned vbit : 1;
		unsigned mbit : 1;
		unsigned pbit : 1;
		unsigned reserved : 5;
#else 
		unsigned reserved : 5;
		unsigned pbit : 1;
		unsigned mbit : 1;
		unsigned vbit : 1;
#endif
	}bit;
}AVP_OPT_ST;

typedef struct {
	u_int 	avp_code;
	AVP_OPT_ST opt;
	int 	data_len;
	int vendor_id;
}AVP_HEADER_ST;

typedef enum {
	ADDR_TYPE_IPV4 = 1,
	ADDR_TYPE_IPV6 = 2
}ENUM_ADDR_TYPE;
typedef struct {
	int addr_type;
	union {
		u_int 	ipv4;
		u_char 	ipv6[16];
		u_char 	reserved[64];
	}addr;
}AVP_ADDRESS_ST;

typedef struct {
	u_int 		avp_code; 			// avp_code 
	const char *name;

	AVP_HEADER_ST header;

	int 		data_type;			// avp data_type 
	int 		attr;				// 
	bool 		has; 				// avp enable 

	int 		start_range;
	int 		end_range;

	Octstr 		*data;

	char 		errstr[1024];
}FLK_AVP_ST;
/*
typedef struct {
	u_int 		avp_code; 			// avp_code 
	int 		vendor_id;			// vendor_id : not set 0 (vbit = 0) 
	bool 		mbit;				
	bool 		pbit;
	int 		data_type;			// avp data_type 
	int 		attr;				// 
	bool 		has; 				// avp enable 
	const char *name;
	int 		start_range;
	int 		end_range;
	union {
		int data_int32;
		u_int data_uint32;
		long data_int64;
		u_long data_uint64;
		float data_float32;
		double data_float64;
		u_int 	data_enum;
		Octstr *data_octstr;
		List 	*data_group;
		List 	*data_list;
	}data;
}FLK_AVP_ST;
*/

typedef struct {
	u_int 	avp_code;
	int 	start_range;
	int 	end_range;
	int 	attr;
	List 	*data;
	bool 	has;
	const char *name;
}FLK_AVP_LIST_ST;

/*
typedef struct {
	const char *intf_name;

	u_char 	version;
	u_int 	message_length;

	bool 	request_flag;
	bool 	proxiable_flag;
	bool 	error_flag;
	bool 	retransmit_flag;

	u_int 	command_code;
	u_int 	application_id;
	u_int 	hop_by_hop_id;
	u_int 	end_to_end_id;

	xht avp;

	const char *desc;
	int errnum;
	char errstr[1024];
}FLK_DIAMETER_ST;
*/

typedef struct {
	const char *intf_name;
	u_int 	intf_code;
	u_int 	command_code;

	DIAMETER_HEADER_ST header;

	List *avp;

	const char *desc;
	int errnum;
	char errstr[1024];

}FLK_DIAMETER_ST;

typedef enum {
#define AVP_GROUP_MANDANTORY(name)
#define AVP_GROUP_OPTIONAL(name)
#define AVP_GROUP_LIST_OPTIONAL(name, start_range, end_range)
#define AVP_GROUP_LIST_MANDANTORY(name, start_range, end_range) 
#define INTF_AVP_FIX(name)
#define INTF_AVP_OPTIONAL(name)
#define INTF_AVP_MANDANTORY(name)
#define INTF_AVP_LIST_OPTIONAL(name, start_range, end_range)
#define INTF_AVP_LIST_MANDANTORY(name, start_range, end_range)
#define AVP_ENUM_TYPE(name, code) avp_enum_##name = code,
#define AVP_DEF(name, full_name, code, vid, mbit, pbit, type, fields) avp_##name = code, fields
#define DIAMETER_INTF(name, desc, command_code, rbit, pbit, app_id, fields) DIA_INTF_##name = command_code + (rbit << 24), \
	DIA_APP_ID_##name = app_id, fields
#include "./diameter.def"
}DIAMETER_CODE_ENUM;

/**************************************************************************************
 * DIAMETER FUNCTION DEFINE
 **************************************************************************************/

/**************************************************************************************
 * AVP DEFINE FUNCTION 
 **************************************************************************************/
// AVP CREATE / DESTROY FUNC
FLK_AVP_ST *avp_create(int avp_code, int attr); // END
FLK_AVP_ST *avp_create_empty(int avp_code, int vid, int mbit, int pbit, int data_type, int attr);
void avp_destroy_item(void *val); // END
void avp_destroy(FLK_AVP_ST *avp); // END

/*
 * AVP - DATA  (avp_'action')
 * 	--> int32, int64, octstr, numstr, utf8str 등의 데이터가 설정됨 
 * AVP - GROUP (avp_group_'action')
 * --> avp들로 구성되며, avp와 avp list로 나누어짐 ( avp1, avp2, avp3-1, avp3-2, avp3-3 )
 * --> avp의 group으로 이루어진 avp들을 컨트롤 하기 위한 함수 
 * AVP - GROUP - LIST (avp_group_list_'action')
 * --> avp_group 내 avp_list를 컨트롤하기 위한 함수 
 * AVP - LIST (avp_list_'action')
 * --> 동일 avp로 구성된 리스트들 
 */

// AVP SET FUNC
void set_avp_header(AVP_HEADER_ST *header, int avp_code, int vendor_id, int mbit, int pbit);
// AVP DATA 에 대한 핸들링
//bool avp_set(FLK_AVP_ST *avp, u_int vendor_id, int mbit, int pbit, void *data, int len); // END
bool avp_set(FLK_AVP_ST *avp, FLK_AVP_ST *val);
bool avp_set_data(FLK_AVP_ST *avp, void *data, int len); // END
bool avp_set_data_int32(FLK_AVP_ST *avp, int val);// END
bool avp_set_data_int64(FLK_AVP_ST *avp, long val);// END
bool avp_set_data_uint32(FLK_AVP_ST *avp, u_int val);// END
bool avp_set_data_uint64(FLK_AVP_ST *avp, u_long val);// END
bool avp_set_data_float32(FLK_AVP_ST *avp, float val);// END
bool avp_set_data_float64(FLK_AVP_ST *avp, double val);// END
bool avp_set_data_enum(FLK_AVP_ST *avp, u_int val);// END
bool avp_set_data_cstr(FLK_AVP_ST *avp, char *val, int len);// END
bool avp_set_data_numstr(FLK_AVP_ST *avp, char *val, int len);// END
bool avp_set_data_octstr(FLK_AVP_ST *avp, Octstr *val);// END
bool avp_set_data_utf8str(FLK_AVP_ST *avp, char *val, int len);// END
bool avp_set_data_group(FLK_AVP_ST *avp, List *group_avp);
bool avp_set_data_address(FLK_AVP_ST *avp, int ip_type, char *address);
bool avp_set_data_inaddr(FLK_AVP_ST *avp, int ip_type, void *in_addr, int size);
bool avp_set_data_time(FLK_AVP_ST *avp, u_int val);

#define avp_set_mbit(avp, val) avp->mbit = (val == 0 ? false : true)
#define avp_set_pbit(avp, val) avp->pbit = (val == 0 ? false : true)
#define avp_set_vendor_id(avp, val) avp->vendor_id = val

// AVP GET FUNC
bool avp_get_data(FLK_AVP_ST *avp, void **data, int *len); // END
bool avp_get_data_int_type(FLK_AVP_ST *avp, void *val); // END
bool avp_get_data_int32(FLK_AVP_ST *avp, int *val);  // END
bool avp_get_data_uint32(FLK_AVP_ST *avp, u_int *val);  // END
bool avp_get_data_int64(FLK_AVP_ST *avp, long *val); // END
bool avp_get_data_uint64(FLK_AVP_ST *avp, u_long *val); // END
bool avp_get_data_float32(FLK_AVP_ST *avp, float *val); // END
bool avp_get_data_float64(FLK_AVP_ST *avp, double *val); // END
bool avp_get_data_enum(FLK_AVP_ST *avp, u_int *val); // END
bool avp_get_data_cstr(FLK_AVP_ST *avp, char **val, int *len); // END
bool avp_get_data_numstr(FLK_AVP_ST *avp, char **val, int *len); // END
bool avp_get_data_octstr(FLK_AVP_ST *avp, Octstr **val); // END
bool avp_get_data_utf8str(FLK_AVP_ST *avp, char **val, int *len); // END
bool avp_get_data_group(FLK_AVP_ST *avp, List **val);
bool avp_get_data_address(FLK_AVP_ST *avp, int *ip_type, char *address, int addr_size);
bool avp_get_data_time(FLK_AVP_ST *avp, u_int *val);

#define avp_get_mbit(avp) avp->mbit
#define avp_get_pbit(avp) avp->pbit
#define avp_get_vendor_id(avp) avp->vendor_id
#define avp_has_vendor_id(avp) (avp->vendor_id == 0 ? false : true)


/**************************************************************************************
 * AVP GROUP DEFINE FUNCTION 
 **************************************************************************************/
// AVP GROUP CREATE / DESTROY FUNC

void avp_debug(FLK_AVP_ST *avp);
// AVP GROUP SET FUNCTION
/*
 * AVP의 DATA가 Group 인 경우 사용 됨
 * AVP의 GROUP AVP에 대한 세부 설정 
 * AVP_GROUP의 AVP가 미리 정의되어 있는 경우 repace되며, 없는 경우 avp를 할당함(append)
 */
bool avp_group_set_data_from_avp(FLK_AVP_ST *avp_group, FLK_AVP_ST *val);
bool avp_group_set(FLK_AVP_ST *avp, FLK_AVP_ST *val); // END
bool avp_group_set_data(FLK_AVP_ST *avp, u_int avp_code, void *data, int len); // END
bool avp_group_set_data_int32(FLK_AVP_ST *avp, u_int avp_code, int val); // END
bool avp_group_set_data_uint32(FLK_AVP_ST *avp, u_int avp_code, u_int val); // END
bool avp_group_set_data_int64(FLK_AVP_ST *avp, u_int avp_code, long val); // END
bool avp_group_set_data_uint64(FLK_AVP_ST *avp, u_int avp_code, u_long val); // END
bool avp_group_set_data_float32(FLK_AVP_ST *avp, u_int avp_code, float val); // END
bool avp_group_set_data_float64(FLK_AVP_ST *avp, u_int avp_code, double val); // END
bool avp_group_set_data_enum(FLK_AVP_ST *avp, u_int avp_code, u_int val); // END
bool avp_group_set_data_cstr(FLK_AVP_ST *avp, u_int avp_code, char *val, int size); // END
bool avp_group_set_data_numstr(FLK_AVP_ST *avp, u_int avp_code, char *val, int size); // END
bool avp_group_set_data_octstr(FLK_AVP_ST *avp, u_int avp_code, Octstr *val); // END
bool avp_group_set_data_utf8str(FLK_AVP_ST *avp, u_int avp_code, char *val, int size); // END
// AVP GROUP DATA의 AVP LIST ROOT를 세팅함 
bool avp_group_set_data_avplist(FLK_AVP_ST *avp, FLK_AVP_ST *val); // TODO
bool avp_group_set_data_address(FLK_AVP_ST *avp, u_int avp_code, int ip_type, char *str_addr);
bool avp_group_set_data_time(FLK_AVP_ST *avp, u_int avp_code, u_int val);

// AVP GROUP GET FUNCTION
bool avp_group_get(FLK_AVP_ST *avp, u_int avp_code, FLK_AVP_ST **val); // END
bool avp_group_get_data(FLK_AVP_ST *avp, u_int avp_code, void **data, int *len); // END
bool avp_group_get_data_int_type(FLK_AVP_ST *avp, u_int avp_code, void *val); // END
bool avp_group_get_data_int32(FLK_AVP_ST *avp, u_int avp_code, int *val); // END
bool avp_group_get_data_uint32(FLK_AVP_ST *avp, u_int avp_code, u_int *val); // END
bool avp_group_get_data_int64(FLK_AVP_ST *avp, u_int avp_code, long *val); // END
bool avp_group_get_data_uint64(FLK_AVP_ST *avp, u_int avp_code, u_long *val); // END
bool avp_group_get_data_float32(FLK_AVP_ST *avp, u_int avp_code, float *val); // END
bool avp_group_get_data_float64(FLK_AVP_ST *avp, u_int avp_code, double *val); // END
bool avp_group_get_data_enum(FLK_AVP_ST *avp, u_int avp_code, u_int *val); // END
bool avp_group_get_data_cstr(FLK_AVP_ST *avp, u_int avp_code, char **val, int *len); // END
bool avp_group_get_data_numstr(FLK_AVP_ST *avp, u_int avp_code, char **val, int *len); // END
bool avp_group_get_data_octstr(FLK_AVP_ST *avp, u_int avp_code, Octstr **val); // END
bool avp_group_get_data_utf8str(FLK_AVP_ST *avp, u_int avp_code, char **val, int *len); // END
bool avp_group_get_data_address(FLK_AVP_ST *avp, u_int avp_code, int *ip_type, char *str, int str_size); // END
bool avp_group_get_data_time(FLK_AVP_ST *avp, u_int avp_code, u_int *val);
// AVP GROUP DATA의 AVP LIST를 가져옴 (ROOT)
bool avp_group_get_data_list(FLK_AVP_ST *avp, u_int avp_code, FLK_AVP_ST **val); // END

// AVP GROUP APPEND FUNCTION
bool avp_group_append(FLK_AVP_ST *avp, FLK_AVP_ST *elem); // END
bool avp_group_append_avplist(FLK_AVP_ST *avp, u_int avp_code, int start_range, int end_range, int attr); //  END
bool avp_group_append_avp(FLK_AVP_ST *avp, u_int avp_code, int attr); // END

// AVP GROUP LIST FUNCTION
//
// AVP GROUP LIST SET FUNCTION
// 이미 할당되어있는 AVP LIST에 AVP를 추가함 (동일 avp_code이어야함)
bool avp_group_list_append(FLK_AVP_ST *avp, u_int avp_code, FLK_AVP_ST *val); // END
bool avp_group_list_append_data(FLK_AVP_ST *avp, u_int avp_code, void *data, int len); // END
bool avp_group_list_append_data_int32(FLK_AVP_ST *avp, u_int avp_code, int val); // END
bool avp_group_list_append_data_uint32(FLK_AVP_ST *avp, u_int avp_code, u_int val); // END
bool avp_group_list_append_data_int64(FLK_AVP_ST *avp, u_int avp_code, long val); // END
bool avp_group_list_append_data_uint64(FLK_AVP_ST *avp, u_int avp_code, u_long val); // END
bool avp_group_list_append_data_float32(FLK_AVP_ST *avp, u_int avp_code, float val); // END
bool avp_group_list_append_data_float64(FLK_AVP_ST *avp, u_int avp_code, double val); // END 
bool avp_group_list_append_data_enum(FLK_AVP_ST *avp, u_int avp_code, u_int val); // END
bool avp_group_list_append_data_cstr(FLK_AVP_ST *avp, u_int avp_code, char *val, int len); // END
bool avp_group_list_append_data_numstr(FLK_AVP_ST *avp, u_int avp_code, char *val, int len); // END
bool avp_group_list_append_data_octstr(FLK_AVP_ST *avp, u_int avp_code, Octstr *val); // END
bool avp_group_list_append_data_utf8str(FLK_AVP_ST *avp, u_int avp_code, char *val, int len); // END
bool avp_group_list_append_data_address(FLK_AVP_ST *avp, u_int avp_code, int ip_type, char *str_adr);
bool avp_group_list_append_data_time(FLK_AVP_ST *avp, u_int avp_code, u_int val);


// AVP GROUP LIST GET FUNCTION
// AVP_LIST ROOT 가 리턴됨 
bool avp_group_list_get(FLK_AVP_ST *avp, u_int avp_code, FLK_AVP_ST **avp_list);
// AVP LIST가 리턴됨 
bool avp_group_list_get_avplist(FLK_AVP_ST *avp, u_int avp_code, List **val); 
// AVP LIST들의 데이터가 리턴됨 
//bool avp_group_list_get_datalist(FLK_AVP_ST *avp, u_int avp_code, int *data_type, List **val); 


// AVP LIST FUNCTION
FLK_AVP_ST *avp_list_create(int avp_code, int start_range, int end_range, int attr); // END
void avp_list_destroy(FLK_AVP_ST *avp);

// AVP LIST APPEND FUNCTION
bool avp_list_append(FLK_AVP_ST *avp, FLK_AVP_ST *val); // END
bool avp_list_append_data(FLK_AVP_ST *avp, void *data, int len); // END
bool avp_list_append_data_int32(FLK_AVP_ST *avp, int val); // END
bool avp_list_append_data_uint32(FLK_AVP_ST *avp, u_int val); // END
bool avp_list_append_data_int64(FLK_AVP_ST *avp, long val); // END
bool avp_list_append_data_uint64(FLK_AVP_ST *avp, u_long val); // END
bool avp_list_append_data_float32(FLK_AVP_ST *avp, float val); // END
bool avp_list_append_data_float64(FLK_AVP_ST *avp, double val); // END 
bool avp_list_append_data_enum(FLK_AVP_ST *avp, u_int val); // END
bool avp_list_append_data_cstr(FLK_AVP_ST *avp, char *val, int len); // END
bool avp_list_append_data_numstr(FLK_AVP_ST *avp, char *val, int len);
bool avp_list_append_data_octstr(FLK_AVP_ST *avp, Octstr *val); // END
bool avp_list_append_data_utf8str(FLK_AVP_ST *avp, char *val, int len); // END
bool avp_list_append_data_address(FLK_AVP_ST *avp, u_int ip_type, char *str_addr);
bool avp_list_append_data_time(FLK_AVP_ST *avp, u_int val);

// AVP LIST GET FUNCTION
int avp_list_avp_count(FLK_AVP_ST *avp); // END
bool avp_list_get_avplist(FLK_AVP_ST *avp, List **lst); //END
bool avp_list_get_avp(FLK_AVP_ST *avp, int idx, FLK_AVP_ST **elem); //END

// AVP LIST 의 구성 AVP의 data_type = GROUP 인 경우 고려 중.... 헐... 생각 못했네 -0- 
bool avp_list_get_data(FLK_AVP_ST *avp, int idx, void **data, int *len);
//bool avp_list_get_data_list(FLK_AVP_ST *avp, int *data_type, List **data); // END


// DIAMETER FUNCTION
FLK_DIAMETER_ST *diameter_create(int intf_code); // END
FLK_DIAMETER_ST *diameter_create_from_packet(int command_code, int request_bit);
void diameter_destroy(FLK_DIAMETER_ST *dia); // END
FLK_DIAMETER_ST *diameter_create_empty(int command_code, int version, u_char opt, u_int app_id, u_int hid, u_int eid);
//FLK_DIAMETER_ST *diameter_create_from_request(FLK_DIAMETER_ST *req); 
FLK_DIAMETER_ST *diameter_create_from_request(FLK_DIAMETER_ST *req, Octstr *host, Octstr *realm, int result_code);

// DIAMETER AVP SET
bool dia_set_avp(FLK_DIAMETER_ST *dia, FLK_AVP_ST *avp);
bool dia_set_avp_data(FLK_DIAMETER_ST *dia, u_int avp_code, void *data, int len);
bool dia_set_avp_data_int32(FLK_DIAMETER_ST *dia, u_int avp_code, int val);
bool dia_set_avp_data_uint32(FLK_DIAMETER_ST *dia, u_int avp_code, u_int val);
bool dia_set_avp_data_int64(FLK_DIAMETER_ST *dia, u_int avp_code, long val);
bool dia_set_avp_data_float32(FLK_DIAMETER_ST *dia, u_int avp_code, float val);
bool dia_set_avp_data_float64(FLK_DIAMETER_ST *dia, u_int avp_code, double val);
bool dia_set_avp_data_enum(FLK_DIAMETER_ST *dia, u_int avp_code, u_int val);
bool dia_set_avp_data_cstr(FLK_DIAMETER_ST *dia, u_int avp_code, char *cstr, int len);
bool dia_set_avp_data_numstr(FLK_DIAMETER_ST *dia, u_int avp_code, char *numstr, int len);
bool dia_set_avp_data_octstr(FLK_DIAMETER_ST *dia, u_int avp_code, Octstr *octstr);
bool dia_set_avp_data_utf8str(FLK_DIAMETER_ST *dia, u_int avp_code, char *val, int len);
bool dia_set_avp_data_address(FLK_DIAMETER_ST *dia, u_int avp_code, int ip_type, char *str_addr);
bool dia_set_avp_data_time(FLK_DIAMETER_ST *dia, u_int avp_code, u_int val);

// DIAMETER AVP GET
bool dia_get_avp(FLK_DIAMETER_ST *dia, u_int avp_code, FLK_AVP_ST **avp);
bool dia_get_avp_data(FLK_DIAMETER_ST *dia, u_int avp_code, void **data, int *len);
bool dia_get_avp_data_int32(FLK_DIAMETER_ST *dia, u_int avp_code, int *val);
bool dia_get_avp_data_uint32(FLK_DIAMETER_ST *dia, u_int avp_code, u_int *val);
bool dia_get_avp_data_int64(FLK_DIAMETER_ST *dia, u_int avp_code, long *val);
bool dia_get_avp_data_float32(FLK_DIAMETER_ST *dia, u_int avp_code, float *val);
bool dia_get_avp_data_float64(FLK_DIAMETER_ST *dia, u_int avp_code, double *val);
bool dia_get_avp_data_enum(FLK_DIAMETER_ST *dia, u_int avp_code, u_int *val);
bool dia_get_avp_data_cstr(FLK_DIAMETER_ST *dia, u_int avp_code, char **cstr, int *len);
bool dia_get_avp_data_numstr(FLK_DIAMETER_ST *dia, u_int avp_code, char **numstr, int *len);
bool dia_get_avp_data_octstr(FLK_DIAMETER_ST *dia, u_int avp_code, Octstr **octstr);
bool dia_get_avp_data_utf8str(FLK_DIAMETER_ST *dia, u_int avp_code, char **utfstr, int *len);
bool dia_get_avp_data_address(FLK_DIAMETER_ST *dia, u_int avp_code, int *ip_type, char *str_addr, int str_size);
bool dia_get_avp_data_time(FLK_DIAMETER_ST *dia, u_int avp_code, u_int *val);

// DIAMETER AVP GROUP
bool dia_set_avp_group_data(FLK_DIAMETER_ST *dia, u_int group_avp, u_int avp_code, void *data, int len);
bool dia_set_avp_group_data_int32(FLK_DIAMETER_ST *dia, u_int group_avp, u_int avp_code, int val);
bool dia_set_avp_group_data_int64(FLK_DIAMETER_ST *dia, u_int group_avp, u_int avp_code, long val);
bool dia_set_avp_group_data_float32(FLK_DIAMETER_ST *dia, u_int group_avp, u_int avp_code, float val);
bool dia_set_avp_group_data_float64(FLK_DIAMETER_ST *dia, u_int group_avp, u_int avp_code, double val);
bool dia_set_avp_group_data_enum(FLK_DIAMETER_ST *dia, u_int group_avp, u_int avp_code, u_int val);
bool dia_set_avp_group_data_numstr(FLK_DIAMETER_ST *dia, u_int group_avp, u_int avp_code, char *val, int len);
bool dia_set_avp_group_data_octstr(FLK_DIAMETER_ST *dia, u_int group_avp, u_int avp_code, Octstr *ostr);
bool dia_set_avp_group_data_utf8str(FLK_DIAMETER_ST *dia, u_int group_avp, u_int avp_code, char *val, int len);
bool dia_set_avp_group_data_cstr(FLK_DIAMETER_ST *dia, u_int group_avp, u_int avp_code, char *val, int len);
bool dia_set_avp_group_data_address(FLK_DIAMETER_ST *dia, u_int group_avp, u_int avp_code, int ip_type, char *str_addr);
bool dia_set_avp_group_data_time(FLK_DIAMETER_ST *dia, u_int group_avp, u_int avp_code, u_int val);

bool dia_get_avp_group_data(FLK_DIAMETER_ST *dia, u_int group_avp, u_int avp_code, void **data, int *len);
bool dia_get_avp_group_data_int_type(FLK_DIAMETER_ST *dia, u_int group_avp, u_int avp_code, void *val);
bool dia_get_avp_group_int32(FLK_DIAMETER_ST *dia, u_int group_avp, u_int avp_code, int *val);
bool dia_get_avp_group_int64(FLK_DIAMETER_ST *dia, u_int group_avp, u_int avp_code, int *val);
bool dia_get_avp_group_float32(FLK_DIAMETER_ST *dia, u_int group_avp, u_int avp_code, int *val);
bool dia_get_avp_group_float64(FLK_DIAMETER_ST *dia, u_int group_avp, u_int avp_code, int *val);
bool dia_get_avp_group_enum(FLK_DIAMETER_ST *dia, u_int group_avp, u_int avp_code, int *val);
bool dia_get_avp_group_octstr(FLK_DIAMETER_ST *dia, u_int group_avp, u_int avp_code, Octstr **octstr);
bool dia_get_avp_group_cstr(FLK_DIAMETER_ST *dia, u_int group_avp, u_int avp_code, char **val, int *len);
bool dia_get_avp_group_numstr(FLK_DIAMETER_ST *dia, u_int group_avp, u_int avp_code, char **val, int *len);
bool dia_get_avp_group_utf8str(FLK_DIAMETER_ST *dia, u_int group_avp, u_int avp_code, char **val, int *len);
bool dia_get_avp_group_address(FLK_DIAMETER_ST *dia, u_int group_avp, u_int avp_code, int *ip_type, char *str_addr, int str_size);
bool dia_get_avp_group_time(FLK_DIAMETER_ST *dia, u_int group_avp, u_int avp_code, u_int *val);

// DIAMETER AVP GROUP LIST TODO
bool dia_set_avp_group_list(FLK_DIAMETER_ST *dia, u_int group_avp, FLK_AVP_ST *avplist); // TODO
bool dia_set_avp_group_list_data_int32(FLK_DIAMETER_ST *dia, u_int group_avp, u_int avp_code, int val);
bool dia_get_avp_group_list(FLK_DIAMETER_ST *dia, u_int group_avp, FLK_AVP_ST **avplist); // TODO
bool dia_get_avp_data_group(FLK_DIAMETER_ST *dia, u_int group_avp, List **val);

bool dia_append_avp_group_list(FLK_DIAMETER_ST *dia, u_int group_avp, FLK_AVP_ST *avp);
//bool dia_get_avp_group_list(FLK_DIAMETER_ST *dia, u_int group_avp, List **avp);

bool dia_append_avp_group_list_data(FLK_DIAMETER_ST *dia, u_int group_avp, u_int avp_code, void *data, int len);
bool dia_get_avp_group_list_data(FLK_DIAMETER_ST *dia, u_int group_avp, u_int avp_code, int *data_type, List **data);
 
// DIAMETER AVP LIST TODO 
bool dia_set_avp_list(FLK_DIAMETER_ST *dia, u_int avp_code, FLK_AVP_ST *avp);
bool dia_append_avp_list(FLK_DIAMETER_ST *dia, u_int avp_code, FLK_AVP_ST *avp);
bool dia_append_avp_list_data(FLK_DIAMETER_ST *dia, u_int avp_code, void **data, int len);

bool dia_get_avp_list_root(FLK_DIAMETER_ST *dia, u_int avp_code, FLK_AVP_ST **avp);
int dia_get_avp_list_count(FLK_DIAMETER_ST *dia, u_int avp_code);
bool dia_get_avp_list(FLK_DIAMETER_ST *dia, u_int avp_code, List **avplist);

//bool dia_get_avp_list_data(FLK_DIAMETER_ST *dia, u_int avp_code, int idx);
//bool dia_get_avp_list_data_list(FLK_DIAMETER_ST *dia, u_int avp_code, int *data_type, List **data);


bool avp_append_list_data(FLK_AVP_ST *avp, void *data, int len) ;
bool dia_append_avp_group(FLK_DIAMETER_ST *dia, u_int avp_code, FLK_AVP_ST *val);
bool dia_append_avp_group_data(FLK_DIAMETER_ST *dia, u_int group_avp, u_int avp_code, void *data, int len);
bool dia_append_avp_group_data_int32(FLK_DIAMETER_ST *dia, u_int group_avp, u_int avp_code, int len);
//char *dia_get_avp_data_cstr(FLK_DIAMETER_ST *dia, u_int avp_code);



void diameter_debug(FLK_DIAMETER_ST *dia);
// DIAMETER ENCODE/DECODE  FUNCTION
//bool dia_encode(FLK_DIAMETER_ST *dia, Octstr *data);
//bool dia_decode(FLK_DIAMETER_ST *dia, Octstr *data);

// UTIL
Octstr *convert_pointer_to_octstr(void *ptr);
void *convert_octstr_to_pointer(Octstr *ostr);
void copy_avp_list(List *dest, List *src);
FLK_AVP_ST *extract_avp_list(List *lst, u_int avp_code);
Octstr *octstr_byte_order(Octstr *os);
char *get_data_type_name(int type);
char *get_attr_name(int attr);
bool convert_net_to_address(AVP_ADDRESS_ST *addr, char *str, int str_size);
bool convert_address_to_net(AVP_ADDRESS_ST *addr, int ip_type, char *str_addr);

// Encoder
bool encode_avp(FLK_AVP_ST *avp, Octstr *data);
void encode_diameter_header(DIAMETER_HEADER_ST *header, Octstr *data);
void encode_avp_header(AVP_HEADER_ST *header, Octstr *data);
bool encode_avp_group(FLK_AVP_ST *avp, Octstr *os);
bool encode_avp_list(FLK_AVP_ST *avp, Octstr *os);
bool encode_diameter(FLK_DIAMETER_ST *dia, Octstr **raw);

// Decoder
bool decode_diameter(FLK_DIAMETER_ST **dia, Octstr *data);
bool decode_diameter_defined(FLK_DIAMETER_ST *dia, List *avp_list);
bool decode_avp_split(Octstr *data, List **avplist);
bool decode_avp(FLK_AVP_ST **avp, Octstr *data);
bool decode_avp_group(FLK_AVP_ST *avp, Octstr *data);
bool decode_diameter_header(DIAMETER_HEADER_ST *header, Octstr *data);
Octstr *get_dia_debug_ostr(FLK_DIAMETER_ST *dia);

FLK_DIAMETER_ST *make_response_from_cer(FLK_DIAMETER_ST *req);
void copy_avp(FLK_AVP_ST *dest, FLK_AVP_ST *src);
FLK_DIAMETER_ST *make_response_from_ofr(FLK_DIAMETER_ST *req, int result_code);
FLK_DIAMETER_ST *make_cea_from_cer(FLK_DIAMETER_ST *req, Octstr *host, Octstr *realm, List *ip, u_int vendor_id, Octstr *product_name, int result_code);
Octstr  *get_result_desc(u_int val) ;
void diameter_dump(FLK_DIAMETER_ST *dia, int level, char *fmt, ...);
FLK_AVP_ST *duplicate_avp(FLK_AVP_ST *src);
void diameter_call_log(char *src, char *dest, FLK_DIAMETER_ST *dia);
#endif
