/***********************************************************************
*
*   Copyright (c) 1998 LG Information & Communications
*   All rights reserved.
*
*   This program contains confidential and proprietary information of
*   LGIC, and any reproduction, disclosure, or use in whole or in part
*   is expressly  prohibited, except as may be specifically authorized
*   by prior written agreement or permission of LGIC.
*
*               LG Information & Communications, Ltd.
*        395-65 , Shindaebang-dong, Dongjak-gu, Seoul, 150-010 Korea
*
***********************************************************************/

/***********************************************************************
	FILE NAME		:	smdcCommon.h
	PURPOSE			:
	DATE OF WRITE	: 	1998.04.30
	AUTHOR			: 	Eun-Ju Seo
	REVISION		:	V 1.0
	Ver.	By		When		Reason
	------- ------- ----------  -----------------------------------------
	1.0		EJSEO	1998.04.30	first coding
***********************************************************************/

#ifndef _SMDCCOMMON_H_
#define _SMDCCOMMON_H_

#include <FLKBaseDef.h> 	// Including New System Default Definition.

/*
 * this is for rsmdp & mdbh process
 */
#define		OFF_INSERT		-1
#define		ON_INSERT		1
#define		STOP_SEND		0
#define		START_SEND		1

#define		MAX_BLOCK_MIN_NUM	50
#define     DONOT_EXIST_TABLE   1146
#define     EXIST_TABLE_AS_CREATE -955

#endif
