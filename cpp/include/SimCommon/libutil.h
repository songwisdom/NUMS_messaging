/**
 * =====================================================================================
 *
 *       @file  libutil.h
 *
 *    	 @brief 
 *
 *       Version:  1.0
 *       Compiler:  cc (HP-UX)
 *       @date:  04/21/09 22:40:06
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

#ifndef _H_COM_LIB_UTIL_
#define _H_COM_LIB_UTIL_ 
#include	<time.h>
#include	<stdio.h>
#include	<string.h>
#include	<stdlib.h>
#include	<sys/stat.h>
#include	<unistd.h>
#include	<pthread.h>
#include	<errno.h>
#include	<dirent.h>
#include 	<ctype.h>
#include "flk_type.h"
#include "FLKSimBaseDef.h"

#ifndef MAX_ARGS_SIZE 
#define MAX_ARGS_SIZE   50
#define MAX_ARG_CNT 32
#endif

bool isAlphaNum ( char *str );
char *get_arg ( char ** cpBuff );
void printHex(u_char *Data, int len);
void printHexArgs(char *Comment, u_char *Data, int len);
void printHexData (u_char *Data, int len );
bool exists_file(char * f);
bool exists_dir(char * dir);
char * rtrim(char * s);
char * ltrim(char * s);
char * trim(char * s);
void upper_case(char *p);
int get_field(const char * src, char ch, char (*buff)[MAX_ARGS_SIZE]);
void null_2_space(void * src, int sz);
void null_to_space(void * src, int sz);
char * extract_app_name(char * apname, char * aname) ;
void remove_new_line(char * s);
int get_thread_id();
bool adjust_dir(char * path);
void Sleep(int lMilliseconds);
bool CopyFile(char *pSrc, char *pDes, bool bFailIfExists);
int buff_seek_read(unsigned char *sbuff, unsigned char *dbuff, unsigned int *off, unsigned int len);
void cuteblk(unsigned char *buff) ;
int strutbk(char *str, char c, char _c);
void bin2hex(const char *bin, char *hex, int size);
int hex2bin(const char *hex, char *bin, int size);
void n_strncpy( char *str1, char *str2, int n );
int get_field_2(const char * src, char ch, char (*buff)[MAX_ARGS_SIZE]);
bool file_exist(const char *filename);
int make_path(const char *path);
#endif
