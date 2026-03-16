/**
 * =====================================================================================
 *
 *       @file  libtime.h
 *
 *    	 @brief 
 *
 *       Version:  1.0
 *       Compiler:  cc (HP-UX)
 *       @date:  06/21/10 14:13:19
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
#ifndef _H_LIB_COM_TIME_
#define _H_LIB_COM_TIME_
#include 	<stdarg.h>
#include 	<stdio.h>
#include 	<sys/stat.h>
#include 	<unistd.h>
#include	<sys/time.h>
#include	<time.h>
#include 	<ctype.h>

double get_milli_sec_2();
long get_milli_sec_1();
long get_milli_sec();
int get_datetime_str(char *dt_str);
void get_today(char *dt_str);
long Get_M();
int GetDateTime(char *sdate);
int Getsecond(int sec);
time_t get_gmt_datetime(int day);
void gmt_to_localtime(time_t * ttime, char * buff);
char * format_datetime(char * r, const char * fmt);
time_t str_to_utp_time(char * st);
int timeout();
void __alarm_rtn(int s);
int set_timeout(int sec);
#endif
