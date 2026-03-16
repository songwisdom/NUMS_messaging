/**
 * =====================================================================================
 *
 *       @file  FLKSimBaseDef.h
 *
 *    	 @brief 
 *
 *       Version:  1.0
 *       Compiler:  cc (HP-UX)
 *       @date:  04/20/09 10:08:58
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

#ifndef _H_FLK_SIM_BASE_DEF_
#define _H_FLK_SIM_BASE_DEF_
/////////////////////////////////////////////////////
#include    <stdio.h>
#ifdef _HP_64BIT_SYS_TYPE_
#include    <strings.h>
#else
#include    <string.h>
#endif
#include    <stdlib.h>
#include  	<unistd.h>

#include 	"flk_type.h"
//#include 	"flk_string.h"
#include 	"SimCommon.h"
//////////////////////////////////////////////////////

#define TRUE  1
#define FALSE 0

#define SPACE 0x20
#define TAB 0x09
#define CR 0x0D
#define LF 0x0A

#define MAX_PATH 1024
#define SIM_PATH_MAX 1024

#ifndef MIN
#define MIN(x, y)   ( ((x) > (y)) ? (y) : (x) )
#endif


#define SIM_CONFIG_PATH "config/sim_cfg"
#define SMSC_HOME 	"SMSC_HOME"
#ifdef __cplusplus
extern "C" {
#endif
void LogRet(const char *fmt, ...);
#ifdef __cplusplus
}
#endif
#endif

