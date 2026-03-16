/**
 * =====================================================================================
 *
 *       @file  libMap3G_Addr.h
 *
 *    	 @brief 
 *
 *       Version:  1.0
 *       Compiler:  cc (HP-UX)
 *       @date:  04/23/09 14:49:32
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

#ifndef _H_LIB_MAP_3G_ADDR_
#define _H_LIB_MAP_3G_ADDR_

#define ADDRESS_STRING_LEN      	20
#define BCD_ADDRESS_LEN 			10

#include "flk_type.h"
#include "libMapUtil.h"
#include "libMapAddrUtil.h"


#define OCTETSTRING_TYPE         0x04
#define NULL_TYPE                0x05 
typedef struct type_of_address_t{ 
    // Linux porting
	BIT_T NPI    : 4;    /* Numbering Plan   */ 
    BIT_T TON    : 3;    /* Nature Of Address Indicator */ 
    BIT_T FIX    : 1; 
} __attribute__ ((packed)) type_of_address_st; 
 
typedef struct AddressString 
{        
    u_char     numocts; 
    u_char     ToA; 
    u_char     data[BCD_ADDRESS_LEN]; 
} __attribute__ ((packed)) AddressString; 

typedef struct V3_AddressString
{
    u_char    numocts;
    struct {
		// Linux porting
        BIT_T NP     : 4;    /* Numbering Plan   */
        BIT_T NAI    : 3;    /* Nature Of Address Indicator */
		BIT_T FIX    : 1;
    } m;
    u_char     data[ADDRESS_LEN_STRING];
} __attribute__ ((packed)) V3_AddressString;

typedef struct V3_LMSI
{
    u_char    	numocts;
    u_char     data[LMSI_LEN];
} __attribute__ ((packed)) V3_LMSI;

typedef struct V3_IMSI
{
    u_char    	numocts;
    u_char     data[MAX_IMSI_LEN];                /* 3..8 */
} __attribute__ ((packed)) V3_IMSI;

/* V3 SM RP DA Tag */
typedef enum { 
	V3_SM_RP_DA_imsi 					= 1,
	V3_SM_RP_DA_lmsi 					= 2,
	V3_SM_RP_DA_serviceCentreAddressDA 	= 3,
	V3_SM_RP_DA_noSM_RP_DA 				= 4
}V3_SM_RP_DA_TAG;

/* V3 SM RP OA Tag */
typedef enum {
	V3_SM_RP_OA_msisdn 					= 1,
	V3_SM_RP_OA_serviceCentreAddressOA 	= 2,
	V3_SM_RP_OA_noSM_RP_OA 				= 3
}V3_SM_RP_OA_TAG;

#define MAX_ADDRESS_BUFFER 32
typedef struct V3_AddrBuf_t
{
    int     tag;
	u_char 	ton;
	u_char 	npi;
	int 	numocts;
	u_char 	data[MAX_ADDRESS_BUFFER];
} V3_AddrBuf_st;

/* encV3_Address.c */
/*
int encV3_LMSI(u_char *spMsg, u_char *data, int length, u_char tag);
int encV3_IMSI(u_char *spMsg, u_char *data, int length, u_char tag);
int encV3_AddressString(u_char *spMsg, u_char *data, int length, u_char tag);
int encV3_ISDN_AddressString(u_char *spMsg, u_char *data, int length, u_char tag);
int encV3_NULL(u_char *spMsg, u_char *cTmp , u_char cTag);
int encV3_SM_RP_DA(u_char *spMsg, V3_AddrBuf_st *pD);
int encV3_SM_RP_OA(u_char *spMsg, V3_AddrBuf_st *pD);
int setV3_AddressString ( u_char *Buff, u_char ucTon, u_char ucNpi, u_char *ucAddr, u_char nNumOct);
*/

#ifdef __cplusplus
extern "C" {
#endif
void setV3_AddressBuff ( u_char *Buff, u_char ucTon, u_char ucNpi, u_char *DigitAddr, int DigitOct, int nTag);
int set_address_toa (  u_char *ucToA, u_char ucTon, u_char ucNpi );
int encV3_AddressString ( u_char *Buff, u_char ucTon, u_char ucNpi, u_char *ucAddr, u_char nNumOct);
int encV3_RP_Address ( u_char *Buff, u_char ucTon, u_char ucNpi, u_char *ucAddr, u_char nNumOct);
int encV3_RP_AddressString ( u_char *Buff, u_char ucTon, u_char ucNpi, u_char *ucAddr, u_char nNumOct, u_char tag );
int encV3_SM_RP_DA(u_char *spMsg, V3_AddrBuf_st *pD);
int encV3_SM_RP_OA(u_char *spMsg, V3_AddrBuf_st *pD);
int encV3_Address(stMsgBuf_t *spMsg, u_char *Addr, u_char numocts, u_char cTag);
#ifdef __cplusplus
}
#endif
#endif
