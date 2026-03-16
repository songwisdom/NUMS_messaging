/**
 * =====================================================================================
 *
 *       @file  smscString.h
 *
 *    	 @brief Smsc String library Function
 *
 *       Version:  1.0
 *       Compiler:  cc (HP-UX)
 *       @date:  04/10/09 13:33:14
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

#ifndef _SMSCSTRING_H
#define _SMSCSTRING_H

#include <FLKBaseDef.h>
int str2hex(u_char *cpTarget, int nTargetSize, char *cpSource, int nSourceSize);
int str2dec(u_char *cpTarget, int nTargetSize, char *cpSource, int nSourceSize);
int int2dec(u_char *cpTarget, int nTargetSize, int nSource);
int int2hex(u_char *cpTarget, int nTargetSize, int nSource);
char *min10to5(char *cpSrc, char *cpDest);
char *min5to10(char  *cpSrc, char  *cpDest);
int getNumOfBCD(u_char *cpBCDStr, int nStrLen);
void convertBCD2String(u_char *cpBCDStr, int nBCDLen, u_char *cpStr);

int cvtBcdNational2International(
    const u_char   *cpCountryCode,
    u_char 		ucCountryCodeLen,
    const u_char   *cpSourceAddress,
    u_char         ucSourceLen,
    u_char         *cpTargetAddress,
    u_char         *cpTargetLen);

int cvtBcdInternational2National(
    u_char         ucCountryCodeLen,
    const u_char   *cpSourceAddress,
    u_char         ucSourceLen,
    u_char         *cpTargetAddress,
    u_char         *cpTargetLen);
int str2BCD_Gsmmin(unsigned char   *cpTarget, int nTargetSize, char *cpSource, int nSourceSize);

/* BCDHandle.c */
char *time2BCD( time_t nTime, char	*cpTmp, const char *format );
int  BCD2str( char *cpTarget, int	nTargetSize, u_char	*cpSource, int	nSourceSize );
int  BCD2strEx( char *cpTarget, int	nTargetSize, u_char	*cpSource, int	nSourceSize );
int str2BCD( u_char *cpTarget, int	nTargetSize, const char	*cpSource, int	nSourceSize );
int str2BCD_min( u_char *cpTarget, int	nTargetSize, char *cpSource, int	nSourceSize );
int str2BCDmin_Digit( u_char *cpTarget, int nTargetSize, char *cpSource, int	nSourceSize );
int BCD2str_min( char	*cpTarget, int nTargetSize, u_char	*cpSource, int nSourceSize );

//FR620 pkg append
int gsmBCD2str_min( char *cpTarget, int nTargetSize, u_char *cpSource, int nSourceSize );

int BCD2str_digit( char *cpTarget, int nTargetSize, u_char *cpSource, int nSourceSize );
int str2BCD_callback( u_char *cpTarget, int nTargetSize, char *cpSource, int nSourceSize );
int BCD2str_callback( char *cpTarget, int	nTargetSize, u_char *cpSource, int nSourceSize );
int getlen_BCDcallback( u_char	*str, int max );
int getlen_BCDmin( u_char *str, int max );
int checkMin( char *cpMin, int nMinSize );
int convertEMS2Normal( char *cpText, u_char *ucTextLen );
int convertMin2CB(char *cpMin, int nMinLen, char *cpCallback);
int convertCB2Min( u_char *cpCallback, char *cpMin );
int convertCB2GsmMin( u_char *cpCallback, char *cpMin );
int BCD2str_minEx( char *cpTarget, int nTargetSize, u_char *cpSource, int nSourceSize );
//FR530  pkg LEE
int str2BCD_specialmin(u_char *cpTarget, int nTargetSize, char *cpSource, int nSourceSize);

/* flk_split.c */
#define MAX_SPLIT_FIELD_SIZE 32
int flk_split(char *buf, char ch, char (**arr)[MAX_SPLIT_FIELD_SIZE]);
int get_field_count(char *buf, char ch );
#endif
