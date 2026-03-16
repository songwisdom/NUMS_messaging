/**
 * =====================================================================================
 *
 *       @file  flk_type.h
 *
 *    	 @brief
 *
 *       Version:  1.0
 *       Compiler:  cc (HP-UX)
 *       @date:  04/23/09 12:58:42
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

typedef int I32;
#ifndef __FLK_BASE_DEF_H__
#define __FLK_BASE_DEF_H__
typedef int I32;
#ifndef _H_SIM_FLK_TYPE_
#define _H_SIM_FLK_TYPE_
#include <stdio.h>
#ifdef _HP_64BIT_SYS_TYPE_
#include <strings.h>
#else
#include <string.h>
#endif
#include <stdlib.h>
#include <unistd.h>
/* SET PACKED SIZE */
#ifdef _HP_64BIT_SYS_TYPE_
#define FLK_PACKED_RELEASE 8
#else
#define FLK_PACKED_RELEASE 4
#endif

#ifndef bool
#undef bool
#endif
// typedef int bool;
#define TRUE 1
#define FALSE 0

// typedef enum { false = 0, true = 1 }bool;

#define ISTRUE(x) (strncmp(x, "TRUE", 4) == 0) ? TRUE : FALSE
#define ASCII_SP 0x20
#define ASCII_TAB '\t'

// #define FLK_SUCCESS 0
// #define FLK_FAIL -1
typedef int FLK_RETURN;
typedef enum SuccessFail { FLK_FAIL = -1, FLK_SUCCESS = 0 } SuccessFail_t;
typedef enum SetFlag { FLK_SET_OFF = 0, FLK_SET_ON = 1 } SetFlag_t;
#define FLK_WARN 1
#ifndef __FLK_DATA_TYPE__
#define __FLK_DATA_TYPE__

// TODO linux porting
typedef unsigned char BIT_T;

typedef unsigned UNSIGNED;
typedef signed SIGNED;

typedef char C8;
typedef unsigned char UC8;

typedef short S16;
typedef unsigned short US16;

typedef int I32;
typedef unsigned int UI32;

typedef unsigned int MessageID;

// typedef unsigned BIT_T;
// typedef unsigned int MessageID;

#define SIZE_INT sizeof(int)
#define SIZE_CHAR sizeof(char)
#define SIZE_SHORT sizeof(short)
#define SIZE_LONG sizeof(long)

#ifdef _HP_64BIT_SYS_TYPE_

typedef long L64;
typedef unsigned long UL64;
typedef L64 LONG;
typedef UL64 ULONG;

#else

typedef long L32;
typedef unsigned long UL32;
typedef L32 LONG;
typedef UL32 ULONG;
//// Common Buffer Size.

#endif // _HP_64BIT_SYS_TYPE_
#endif // __FLK_DATA_TYPE__
typedef enum BufferSize {
  HALF_SIZE = 16, // FR210 SGP for FR220
  WORD_SIZE = 32,
  DOUBLE_SIZE = 64,
  BUF_SSIZE = 256,
  BUF_MSIZE = 128,
  BUF_HSIZE = 512,
  BUF_SIZE = 1024,
  BUF_DSIZE = 2048,
  BUF_LSIZE = 10240,
  BUF_SSTR = 10,
  BUF_STR = 50,
  BUF_LSTR = 100
} BufferSize_t;
#include "flk_sim_log.h"
#endif
#endif
