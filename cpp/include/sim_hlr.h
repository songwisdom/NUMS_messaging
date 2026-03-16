/**
 * =====================================================================================
 *
 *       @file  sim_hlr.h
 *
 *    	 @brief 
 *
 *       Version:  1.0
 *       Compiler:  cc (HP-UX)
 *       @date:  04/28/09 20:01:17
 *
 *       @author:  SMS Dev (), sms_dev@feelingk.com
 *       Company:  (c) FEELingK
 *       @attention
 *       Copyright (c) 2003 FEELINGK Co., Ltd.                                
 *
 *         All rights reserved.                                                
 *                                                                          
 *     This program contains confidential and proprietary information of    
 *     FEELINGK, and any reproduction, disclosure, or use in whole or in part 
 *     by prior written agreement or permission of FEELINGK.                 
 *                                                                          
 *                            FEELINGK Co., Ltd.                           
 *     3F Ace Techno Twin Tower 1, 212-1 Kuro-Dong, Kuro-Gu, Seoul 152-050 Korea 
 *
 * =====================================================================================
 */

#ifndef _H_SIM_HLR_
#define _H_SIM_HLR_
#include "flk_type.h"
#include "flk_dbm.h"

//typedef int int;

typedef struct sim_sigtran_t
{
	int DPc;
	int OPc;
	int TimeGap;
	int ReturnPC;
	int MySSN;
	int RemoteSSN;
}sim_sigtran_st;

typedef struct config_info_t
{
	int nCnxId;
	sim_sigtran_st sigtran;
}config_info_st;
#define HLR_DATA_FILE "sim/data/hlr_dbm"
#define HLR_KEY_DATA_INDEX 	2
#define HLR_KEY_LEN_INDEX 	0
#define HLR_ADDR_LEN 24 + 1
#define HLR_IMSI_SIZE 24 +1
#define HLR_LMSI_SIZE 8 +1
#define HLR_ESN_SIZE 8 +1
#define HLR_MIN_SIZE 24 + 1

typedef enum  {
	HLR_EVENTS_RUN,
	HLR_EVENTS_PORTED_OUT,
	HLR_EVENTS_ABSENT_SUBSCRIBER,
	HLR_EVENTS_UNKNOWN_SUBSCRIBER,
	HLR_EVENTS_SYSTEM_FAILURE
}HLR_EVENTS;
typedef struct hlr_data_t
{
	char 	MSISDN			[HLR_ADDR_LEN]; // Key // len(1) + ToA(a) + msisdn
	char 	IMSI			[HLR_IMSI_SIZE];
	char 	LMSI			[HLR_LMSI_SIZE ];
	char  Node1stTag;
	char 	Node1st			[HLR_ADDR_LEN];
	char  Node2stTag;
	char 	Node2st			[HLR_ADDR_LEN];
	char  Node3stTag;
	char 	Node3st 		[HLR_ADDR_LEN];
	char 	MIN				[HLR_MIN_SIZE];
	char 	ESN				[HLR_ESN_SIZE];
	char 	ADR;
	char 	FOWARD_MSISDN 	[HLR_ADDR_LEN];
	char  SMSF_NUMBER 	[HLR_ADDR_LEN];
	u_char CAPA 	 		[5];
	char  ATI_CALLBACK    [HLR_ADDR_LEN];
	u_char EVENTS;
	u_char MSC;
	u_char SGSN;
	u_char OSFI 			[5];
	u_char SMS_OSFI 		[5];
	int 				submit_sm_resp_status;
	int 				deliver_receipt_delay;
	char 				deliver_receipt_stat[8];
	char 	ROAM_PMN 		[HLR_ADDR_LEN];
	char 	ROAM_IND;
	char 	IMS_REGI;
	char 	PNR;
	char 	ALR;
	char 	MNRF;
	char 	MNRG;
	char 	MCEF;
	char 	UNRI;
	char 	TSCODE21;
	char 	TSCODE22;
	int 	ssra;
	int 	sda;
	int 	sdrrc;
	int 	sdrtc;
}hlr_data_st;

typedef struct capa_info_t
{
#ifdef _HP_64BIT_SYS_TYPE_
	BIT_T  	SMS_QUERY  			: 1;
	BIT_T 	SECRET_SMS 			: 1;
	BIT_T 	Reserved1 			: 1;
	BIT_T 	ICS_CAPABILITY 		: 1;
	BIT_T 	COLOR_SMS 			: 1;
	BIT_T 	SPAM_SMS 			: 1;
	BIT_T 	PREMIUM_SMS 		: 1;
	BIT_T 	LONG_SMS 			: 1;
	//////////////////////////////////
	BIT_T 	BIZ_NUMBER 			: 1;
	BIT_T 	ABROAD_TEL 			: 1;
	BIT_T 	NEW_SPAM 			: 1;
	BIT_T 	PATROIT_SMS 		: 1;
	BIT_T 	AUTO_ANSWER 		: 1;
	BIT_T 	PERFECT_SMS 		: 1;
	BIT_T 	SMS_MANAGER 		: 1;
	BIT_T   IM_SMS 	 			: 1;
	////////////////////////////////////////
	BIT_T 	FG_SA_UE 			: 1;
	BIT_T 	Reserved3 			: 1;
	BIT_T 	RCS 				: 1;
	BIT_T 	CALLBACK_NOTI		: 1;
	BIT_T 	URL_NOTI 			: 1;
	BIT_T 	MS_Type 			: 3;
	////////////////////////////////////
	BIT_T 	Reserved4 			: 5;
	BIT_T 	MCS_MODE_SINGLE 	: 1;
	BIT_T 	MCS_MODE_DUAL 		: 1;
	BIT_T   DSDS 				: 1;
#else
	BIT_T   DSDS 				: 1;
	BIT_T 	MCS_MODE_DUAL 		: 1;
	BIT_T 	MCS_MODE_SINGLE 	: 1;
	BIT_T   Reserved4           : 5;
    ////////////////////////////////////
    BIT_T   MS_Type             : 3;
    BIT_T   URL_NOTI            : 1;
    BIT_T   CALLBACK_NOTI       : 1;
    BIT_T   RCS                 : 1;
    BIT_T   Reserved3           : 1;
	BIT_T 	FG_SA_UE 			: 1;
    ////////////////////////////////////
    BIT_T   IM_SMS              : 1;
    BIT_T   SMS_MANAGER         : 1;
    BIT_T   PERFECT_SMS         : 1;
    BIT_T   AUTO_ANSWER         : 1;
    BIT_T   PATROIT_SMS         : 1;
    BIT_T   NEW_SPAM            : 1;
    BIT_T   ABROAD_TEL          : 1;
    BIT_T   BIZ_NUMBER          : 1;
    ////////////////////////////////////
    BIT_T   LONG_SMS            : 1;
    BIT_T   PREMIUM_SMS         : 1;
    BIT_T   SPAM_SMS            : 1;
    BIT_T   COLOR_SMS           : 1;
    BIT_T   ICS_CAPABILITY      : 1;
    BIT_T   Reserved1           : 1;
    BIT_T   SECRET_SMS          : 1;
    BIT_T   SMS_QUERY           : 1;
    ////////////////////////////////////
#endif
} __attribute__ ((packed)) capa_info_st;

typedef struct osfi_info_t
{
	BIT_T   Reserved4 			: 1;
	BIT_T   PLUS_010			: 1;
	BIT_T   Reserved4_2 		: 3;
	BIT_T   W_ZONE 				: 1;
	BIT_T   INTER_MO_DENY 		: 1;
	BIT_T   Reserved4_1 		: 1;
    ////////////////////////////////////
    BIT_T   Reserved3_2 		: 2;
    BIT_T   PERFECT_SMS 		: 1;
    BIT_T   Reserved3_1 		: 5;
    ////////////////////////////////////
    BIT_T   Reserved2_3  		: 1;
    BIT_T   SNAP 				: 1;
    BIT_T   Reserved2_2 		: 2;
    BIT_T   SMS_MGR 			: 1;
    BIT_T   Reserved2_1 		: 2;
    BIT_T   PPS 				: 1;
    ////////////////////////////////////
    BIT_T   Reserved1_2 		: 2;
    BIT_T   PSMS 				: 1;
    BIT_T   ASMS 				: 1;
    BIT_T   Reserved1_1 		: 4;
    ////////////////////////////////////
} __attribute__ ((packed)) osfi_info_st;

typedef struct sms_osfi_info_t
{
	BIT_T   Reserved4           : 8;
    ////////////////////////////////////
    BIT_T   Reserved3           : 8;
    ////////////////////////////////////
    BIT_T   Reserved2           : 8;
    ////////////////////////////////////
    BIT_T   Reserved1 			: 3;
    BIT_T   MCS_MODE_SINGLE 	: 1;
    BIT_T   MCS_MODE_DUAL 		: 1;
    BIT_T   INFO_RATE 			: 1;
    BIT_T   ROAMING_LIMIT 		: 1;
    BIT_T   CID_CHANGE 			: 1;
    ////////////////////////////////////
} __attribute__ ((packed)) sms_osfi_info_st;

int hlr_db_open();
int init_hlr_db(const char *db_file);
void hlr_db_close(void);
int hlr_db_delete( const char *data);
int hlr_db_insert( const hlr_data_st *data );
int hlr_db_fetch (hlr_data_st **hlr_data);
//int hlr_db_fetch (datum *hlr_key, hlr_data_st *hlr_data);
int hlr_db_search_prefix_for_bcd ( char *search_msisdn, int length, hlr_data_st *data );
int hlr_db_search_msisdn ( char *search_msisdn, hlr_data_st *data );
int hlr_db_search_prefix ( char *search_msisdn, int length, hlr_data_st *data );
int hlr_db_count ();

//////////////////////////////////////////////

int init_mdb_hlr_data();
int get_hlr_data_count();
int load_hlr_mdb();
int select_hlr_mdb_msisdn( char * msisdn, hlr_data_st *data );
int select_hlr_mdb_prefix(char *msisdn, hlr_data_st *data);
int select_hlr_mdb_msisdn_bcd(char *bcd, int length, hlr_data_st *data);
int insert_hlr_mdb(hlr_data_st *data);
int delete_hlr_mdb(char * msisdn);
int hlr_mdb_fetch( hlr_data_st **data);
int select_hlr_mdb_index ( int idx, hlr_data_st *data );

int selecet_hlr_all ();
void init_hlr_data(hlr_data_st *data);

#endif
