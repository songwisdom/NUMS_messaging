/**
 * =====================================================================================
 *
 *       @file  flk_string.h
 *
 *    	 @brief 
 *
 *       Version:  1.0
 *       Compiler:  cc (HP-UX)
 *       @date:  04/23/09 13:00:01
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
#ifndef _H_FLK_STRING_
#define _H_FLK_STRING_
#include "flk_type.h"
#ifdef __cplusplus
extern "C" {
#endif
void flk_strcpy(u_char * dest, const u_char *src);
void flk_strncpy(u_char * dest, const u_char *src, int count);
int flk_strcpy_limit(u_char *dest, const u_char *src, int size);
u_char * flk_strcat(u_char * dest, const u_char * src);
u_char * flk_strncat(u_char *dest, const u_char *src, int count);
bool flk_strcmp(const u_char * cs,const u_char * ct);
bool flk_strncmp(const char * cs,const char * ct,int count);
int flk_strlen(const u_char * s);
int flk_strnlen(const u_char * s, int count);
void * flk_memset(void * s,int c,int count);
void * flk_memcpy(void * dest,const void *src,int count);
int flk_memcmp(const void * cs,const void * ct,int count);
#ifdef __cplusplus
}
#endif
#endif 
