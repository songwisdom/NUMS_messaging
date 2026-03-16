/***********************************************************************
*                                                                      *
*   Copyright (c) 1997 - 1998 LG Information & Communications          *
*   All rights reserved.                                               *
*                                                                      *
*   This program contains confidential and proprietary information of  *
*   LGIC, and any reproduction, disclosure, or use in whole or in part *
*   is expressly  prohibited, except as may be specifically authorized *
*   by prior written agreement or permission of LGIC.                  *
*                                                                      *
*               LG Information & Communications, Ltd.                  *
*        395-65 , Shindaebang-dong, Dongjak-gu, Seoul, 150-010 Korea   *
*                                                                      *
***********************************************************************/
/*===============================================================================
*       MODULE NAME          : EIclude.h
*       PURPOSE              : Expand Include.
*       INPUT ARGUMENT       : NONE
*       OUTPUT ARGUMENT      :
*       RETURN VALUE         : NONE
*       REVISION             :     BY             WHEN            REASON
*                             -------------  --------------  -------------
*                                  hkkang      1998.04.29     first coding
==============================================================================*/
#ifndef __EINCLUDE_H__
#define __EINCLUDE_H__


#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>
#include <signal.h>
#include <time.h>
#include <ctype.h>
#include <errno.h>
#include <poll.h>
#include <stdarg.h>
#include <setjmp.h>

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <libgen.h>
#include "FLKBaseDef.h" // Includeing New System Base Difinition.


#include "EFile.h"
#include "EErrorCode.h" /* jssim 2001. 11. 30 */
#include "EIpc.h"
#include "EDebug.h"
#include "EProcess.h"
#include "EString.h"
#include "ETime.h"


#include "EConvert.h"
#include "EErrno.h"
#include "EIs.h"
#include "ESvcUtil.h"
#endif
