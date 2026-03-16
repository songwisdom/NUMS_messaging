/**
 * =====================================================================================
 *
 *       @file  hlr_dbm.h
 *
 *    	 @brief 
 *
 *       Version:  1.0
 *       Compiler:  cc (HP-UX)
 *       @date:  04/30/09 17:38:56
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


#include "flk_type.h"
#include <fcntl.h>
#include <dbm.h>


//#define HLR_DATA_FILE "hlr_dbm"
#define HLR_KEY_DATA_INDEX 	2;
#define HLR_KEY_LEN_INDEX 	0;
typedef struct hlr_data_t
{
	char 	MSISDN 		[21]; // Key // len(1) + ToA(a) + msisdn
	char 	IMSI 		[21];
	char 	SGSN 		[21];
	char 	MIN  		[21];
	char 	ESN  		[21];
	char 	CAPA 	 	[5];
}hlr_data_st2;


FLK_RETURN init_hlr_db(const char *db_file);
void hlr_db_close(void);
FLK_RETURN hlr_db_delete( const char *data, int length);
FLK_RETURN hlr_db_insert( const hlr_data_st *data );
FLK_RETURN hlr_db_search ( char *search_msisdn, int length, hlr_data_st *data );
FLK_RETURN hlr_db_count ();
FLK_RETURN hlr_db_fetch (datum *hlr_key, hlr_data_st *hlr_data);
