#ifndef _H_SIM_ST_
#define _H_SIM_ST_ 
#define MAX_ADDR_SIZE 20
#define MAX_MSG_SIZE 256
#define MAX_SAC_SIZE 2 // FR401 LSH 
#define MAX_MSGREF_SIZE 14 // FR401 LSH 
#include <stdbool.h>   // bool
#include <stdint.h>    // uint8_t, uint16_t, uint32_t 등 (권장)
#include <sys/types.h> // 일부 시스템에서 u_char 제공
#pragma pack(push, 1)
typedef struct osfi_bit_t
{
	// 1 Byte
	unsigned 	Reserved1 		: 4;

	unsigned   Reserved2 		: 2;
    unsigned   bPSMS           : 1; // 4
    unsigned   bASMS           : 1; // 8

    // 2 Byte
	unsigned 	bMGR 			: 1; //4096
	unsigned 	Reserved3 		: 2;
	unsigned 	bPPS 			: 1; // 32768
   	
	unsigned 	Reserved4 		: 1; //2048
    unsigned   bSNAP           : 1; //512
    unsigned   bSMSFailNoti    : 1; //1024
	unsigned 	Reserved5 		: 1;
	
	// 3Byte
	unsigned 	Reserved6 		: 4;

	unsigned 	Reserved7 		: 2; 
	unsigned 	bSMS_NOTI 		: 1; //262144
	unsigned 	Reserved8 		: 1; 

	// 4Byte
	unsigned 	Reserved9 		: 1;
	unsigned 	bWZONE 	 		: 1; //536870912
	unsigned 	bINTER_MO_DENY	: 1; //1073741824
	unsigned 	Reserved10 		: 1; 

	unsigned 	Reserved11		: 1; //33554432
	unsigned 	bCYSMS 			: 1; //33554432
	unsigned 	Reserved12 		: 2; 
}osfi_bit_st;
/*
typedef struct osfi_bit_t
{
	// 1 Byte
	unsigned 	Reserved1 		: 4;
    unsigned   bASMS           : 1; // 8
    unsigned   bPSMS           : 1; // 4
	unsigned   Reserved2 		: 2;

    // 2 Byte
	unsigned 	bPPS 			: 1; // 32768
	unsigned 	Reserved3 		: 2;
	unsigned 	bMGR 			: 1; //4096
   	unsigned 	Reserved4 		: 1; //2048
    unsigned   bSMSFailNoti    : 1; //1024
    unsigned   bSNAP           : 1; //512
	unsigned 	Reserved5 		: 1;
	
	// 3Byte
	unsigned 	Reserved6 		: 5;
	unsigned 	bSMS_NOTI 		: 1; //262144
	unsigned 	Reserved7 		: 2; 

	// 4Byte
	unsigned 	Reserved8 		: 1;
	unsigned 	bINTER_MO_DENY	: 1; //1073741824
	unsigned 	bWZONE 	 		: 1; //536870912
	unsigned 	Reserved9 		: 3; 
	unsigned 	bCYSMS 			: 1; //33554432
	unsigned 	Reserved10		: 1; //33554432
}osfi_bit_st;
*/
#pragma pack(pop) 
//#pragma pack FLK_PACKED_RELEASE
#pragma pack(push, 1)
typedef struct smsosfi_bit_t
{
    // 1 Byte
	unsigned   bMCSDUAL        : 1;
	unsigned   bINFORAGE       : 1;
    unsigned   bROAMLIMIT 		: 1;
    unsigned   bCIDCHANGE 		: 1;
    
	unsigned   Reserved1       : 3;
	unsigned   bMCSSINGLE      : 1;


    // 2 Byte
    unsigned   Reserved2       : 8;
    
    // 3Byte
    unsigned   Reserved3       : 8;

    // 4Byte
    unsigned   Reserved4       : 8;

    // 5Byte
    unsigned   Reserved5       : 8;
    
    // 6Byte
    unsigned   Reserved6       : 8;
    
    // 7Byte
    unsigned   Reserved7       : 8;
    
    // 8Byte
    unsigned   Reserved8       : 8;
}smsosfi_bit_st;
/*
typedef struct smsosfi_bit_t
{
    // 1 Byte
    unsigned   Reserved1       : 3;
	unsigned   bMCSSINGLE      : 1;

	unsigned   bMCSDUAL        : 1;
	unsigned   bINFORAGE       : 1;
    unsigned   bROAMLIMIT 		: 1;
    unsigned   bCIDCHANGE 		: 1;

    // 2 Byte
    unsigned   Reserved2       : 8;
    
    // 3Byte
    unsigned   Reserved3       : 8;

    // 4Byte
    unsigned   Reserved4       : 8;

    // 5Byte
    unsigned   Reserved5       : 8;
    
    // 6Byte
    unsigned   Reserved6       : 8;
    
    // 7Byte
    unsigned   Reserved7       : 8;
    
    // 8Byte
    unsigned   Reserved8       : 8;
}smsosfi_bit_st;
*/
#pragma pack(pop)
//#pragma pack FLK_PACKED_RELEASE

#define OSFI_SIZE 4
#define SMS_OSFI_SIZE 5

typedef struct osfi_info_t 
{
	bool active;
	u_char  osfi[OSFI_SIZE];
}osfi_info_st;

typedef struct smsosfi_info_t 
{
    bool active;
    u_char  smsosfi[SMS_OSFI_SIZE];
}smsosfi_info_st;

typedef struct roam_info_t 
{
	bool active;
	u_char numoctets;
	u_char roam_pmn [MAX_ADDR_SIZE];
}roam_info_st;

typedef struct sac_info_t
{
	bool active;
	u_char  numoctets;
	u_char sac[MAX_SAC_SIZE];
}sac_info_st;

typedef struct msgref_info_t
{
	bool active;
	u_char  numoctets;
	u_char  msg_ref[MAX_MSGREF_SIZE];
}msgref_info_st;


typedef struct extension_info_t 
{
	osfi_info_st osfi_info;
	roam_info_st roam_info;
	sac_info_st sac_info; //sac 
	msgref_info_st msgref_info;
	smsosfi_info_st smsosfi_info;
}extension_info_st;

typedef enum { C_UNKNOWN, C_TYPE_KSC5601, C_TYPE_UCS2, C_TYPE_GSM8, C_TYPE_GSM7, C_TYPE_MAX } C_TYPE;

typedef struct message_info_t
{
	int c_type;
	int length;
	int rmsg_count;
	int rmsg_pos;
	u_char message[MAX_MSG_SIZE] ;
	u_char rmsg[256][MAX_MSG_SIZE];
}message_info_st;

/*
typedef struct sim_addr_info_t 
{
	int npi;
	int ton;
	u_char addr [ MAX_ADDR_SIZE ];
}sim_addr_info_st;
*/

typedef struct address_info_t 
{
	int 	gt_numoctets;
	int 	src_numoctets;
	int 	dest_numoctets;
	int 	cb_numoctets;
	bool 	cb_active;
	int callback_numoctets;
	u_char gt [ MAX_ADDR_SIZE];
	u_char src [ MAX_ADDR_SIZE];
	u_char callback [ MAX_ADDR_SIZE ];
	u_char dest [ MAX_ADDR_SIZE];
}address_info_st;

typedef struct msc_info_t
{
	int nCnxId;
	int ssn;
	int timegap;
	int opc;
	int dpc;
	int remote_ssn;
}msc_info_st;

typedef struct config_submit_sm_t 
{
	u_char ucTon;
	u_char ucNpi;
}config_submit_sm_st;

typedef enum {
	MS_TYPE_2G,
	MS_TYPE_3G
}MS_TYPE;

typedef struct
{
    int             isPriority;
    int             isLanguage;
    u_char             ucLanguage;
    u_char             ucPriority;
	u_short 			TeleserviceIdentifier;
} stBDConfig;

#define MAX_LONG_MSG_CNT 30 

//petertest  for LMS_TEST 20100517
//#define MAX_LONG_MSG_CNT 101 

typedef struct long_sms_t
{
	bool 	active;
	int 		nTotal;
	int 		idx;
	int 		message[MAX_LONG_MSG_CNT][256];
}long_sms_st;

typedef struct config_info_t 
{
	int send_count;
	MS_TYPE ms_type;
	bool rand_flag;
	bool src_rand_flag;
	char rand_dest[21];
	char rand_src[21];
	int interval;
	int rate_second;

	message_info_st message_info;
	extension_info_st extension_info;
	address_info_st address_info;
	msc_info_st msc_info;
	config_submit_sm_st config_submit_info;
	stBDConfig config_637_info;
	long_sms_st long_sms;
}config_info_st;

/*
#define SIM_CONFIG_ADDRESS(x) 	&x->address_info
#define SIM_CONFIG_MESSAGE(x) 	&x->message_info
#define SIM_CONFIG_EXTENSION(x) &x->extension_info
#define SIM_CONFIG_MSC(x) 		&x->msc_info
*/


#endif
