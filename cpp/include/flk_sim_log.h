/**
 * =====================================================================================
 *
 *       @file  flk_sim_log.h
 *
 *    	 @brief 
 *
 *       Version:  1.0
 *       Compiler:  cc (HP-UX)
 *       @date:  04/28/09 16:30:11
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

#ifndef _H_LIB_SIM_DEBUG_
#define _H_LIB_SIM_DEBUG_
#include "SimCom.h"
#include "flk_type.h"
#include <stdio.h>
#include <unistd.h>
#include <stdarg.h>
#include <string.h>


#define SIM_LOG_FILE_SIZE 	256
#define SIM_LOG_MAX_SIZE 	2048
#define SIM_LOG_MAX_ARGS 	1000

#define GET_DISPLAY_LEVEL(x)	(((x) & 0xF0) >> 4)
#define GET_FILE_LEVEL(x) 		((x) & 0x0F)


#define DEF_SETCOLOR_SUCCESS	"\033\1331;32m"
#define DEF_SETCOLOR_FAILURE	"\033\1331;31m"
#define DEF_SETCOLOR_WARNING	"\033\1331;33m"
#define DEF_SETCOLOR_NORMAL		"\033\1330;39m"
#define DEF_SETCOLOR_NT_DATA    "\033\1331;36m"

typedef enum {
	FILE_LEV_OFF 		= 0x00,
	FILE_LEV_ERR 		= 0x01,
	FILE_LEV_SUCCESS 	= 0x02,
	FILE_LEV_WARN 		= 0x03,
	FILE_LEV_MESSAGE 	= 0x04,
	FILE_LEV_NORMAL 	= 0x05,
	FILE_LEV_PARAM 		= 0x06,
	FILE_LEV_DEBUG 		= 0x07,
	FILE_LEV_ALL 		= 0x08,
	DISP_LEV_OFF 		= 0x00, 
	DISP_LEV_ERR 		= 0x10, 
	DISP_LEV_SUCCESS 	= 0x20, 
	DISP_LEV_WARN 		= 0x30, 
	DISP_LEV_MESSAGE 	= 0x40, 
	DISP_LEV_NORMAL 	= 0x50, 
	DISP_LEV_PARAM 		= 0x60, 
	DISP_LEV_DEBUG 		= 0x70, 
	DISP_LEV_ALL 		= 0x80, 
	LOG_LEVEL_MAX
}DBG_LEVEL_TYPE;

#define LEV_ALL_OFF 	0x00
#define LEV_ALL_ERR 	(DISP_LEV_ERR | FILE_LEV_ERR)
#define LEV_ALL_SUCCESS (DISP_LEV_SUCCESS | FILE_LEV_SUCCESS)
#define LEV_ALL_WARN 	(DISP_LEV_WARN | FILE_LEV_WARN)
#define LEV_ALL_MESSAGE (DISP_LEV_MESSAGE | FILE_LEV_MESSAGE)
#define LEV_ALL_NORMAL 	(DISP_LEV_NORMAL | FILE_LEV_NORMAL)
#define LEV_ALL_PARAM   (DISP_LEV_PARAM | FILE_LEV_PARAM)
#define LEV_ALL_DEBUG 	(DISP_LEV_DEBUG | FILE_LEV_DEBUG)
#define LEV_ALL 		(DISP_LEV_ALL | FILE_LEV_ALL)


#define flk_log(lev, str) log_write(lev, __func__, str)

typedef struct log_t 
{
	u_char 		DispLevel;
	u_char 		FileLevel;
	char 		LogFile[SIM_LOG_FILE_SIZE];
	char 		*PushBuffer[SIM_LOG_MAX_ARGS];
	int 	PushArgs;
}log_st;
#ifdef __cplusplus
extern "C" {
#endif
void log_write( u_char Lev, const char *fmt, ...) ;
void log_popup ( );
void log_push ( u_char Lev, const char *fmt, ...);
int SimLogStart( bool ColorFlag, u_char DispLev, u_char FileLev, char *cpPath, char *cpFilename);
void setSimLogFile ( char *cpPath, char *cpFilename);
void setSimLogLevelReset ( u_char DispLev, u_char FileLevel);
void log_writeHex( u_char Lev, char *Comment, char *Data, int nLen, int SortFlag );

#ifdef __cplusplus
}
#endif
#endif
