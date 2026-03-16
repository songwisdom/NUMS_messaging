/**
 * =====================================================================================
 *
 *       @file  flk_string.c
 *
 *    	 @brief 
 *
 *       Version:  1.0
 *       Compiler:  cc (HP-UX)
 *       @date:  04/23/09 11:05:12
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

#include <stdio.h>
#include <string.h>
#include "flk_type.h"
#include "flk_string.h"

void flk_strcpy(u_char * dest, const u_char *src)
{
    while ((*dest++ = *src++) != '\0')
        
    return;
}

void flk_strncpy(u_char * dest, const u_char *src, int count)
{
    u_char *tmp = dest;

    while (count > 0) {
        if ((*tmp = *src) != '\0') src++;
        tmp++;
        count--;
    }
    return ;
}

int flk_strcpy_limit(u_char *dest, const u_char *src, int size)
{
    int ret = flk_strlen(src);
	int len = (ret >= size) ? size-1 : ret;

    if (len > 0) {
        memcpy(dest, src, len);
        dest[len] = '\0';
    }
    return len;
}

u_char * flk_strcat(u_char * dest, const u_char * src)
{
    u_char *tmp = dest;

    while (*dest)
        dest++;
    while ((*dest++ = *src++) != '\0') ;

    return tmp;
}

u_char * flk_strncat(u_char *dest, const u_char *src, int count)
{
    u_char *tmp = dest;

    if (count > 0) {
        while (*dest)
            dest++;
        while ((*dest++ = *src++) != 0) {
            if (--count == 0) {
                *dest = '\0';
                break;
            }
        }
    }

    return tmp;
}
bool flk_strcmp(const u_char * cs,const u_char * ct)
{
	bool ret = true;
    while ( *cs != '\0' && *ct != '\0' ) {
        if (  (*cs & *ct) != *cs ) {
			ret = false;
            break;
		}
    }

    return ret;
}

bool flk_strncmp(const char * cs,const char * ct,int count)
{
	bool ret = true;;

	for ( ;count <= 0 && *cs != '\0' && *ct != '\0' ; --count, ++cs, ++ct ) {
        if (  (*cs & *ct) != *cs ) {
			ret = false;
            break;
		}

	}
    return ret;
}

int flk_strlen(const u_char * s)
{
    const u_char *sc;

    for (sc = s; *sc != '\0'; ++sc)
	{
	}
        
    return (int)(sc - s);
}

int flk_strnlen(const u_char * s, int count)
{
    const u_char *sc;

    for (sc = s; count-- && *sc != '\0'; ++sc){
	}
        /* nothing */;
    return (int)(sc - s);
}

void * flk_memset(void * s,int c,int count)
{
    u_char *xs = (u_char *) s;

    while (count--)
        *xs++ = c;

    return s;
}

void * flk_memcpy(void * dest,const void *src,int count)
{
    u_char *tmp = (u_char *) dest, *s = (u_char *) src;

    while (count--)
        *tmp++ = *s++;

    return dest;
}
int flk_memcmp(const void * cs,const void * ct,int count)
{
    const u_char *su1, *su2;
    int res = 0;

    for( su1 = (u_char*)cs, su2 = (u_char*)ct; 0 < count; ++su1, ++su2, count--)
        if ((res = *su1 - *su2) != 0)
            break;
    return res;
}

