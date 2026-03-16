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
*       MODULE NAME          : EErrno.h
*       PURPOSE              : Expand Error No.
*       INPUT ARGUMENT       : NONE
*       OUTPUT ARGUMENT      :
*       RETURN VALUE         : NONE
*       REVISION             :     BY             WHEN            REASON
*                             -------------  --------------  -------------
*                                  hkkang      1998.06.11     first coding
==============================================================================*/
#ifndef __EERRNO_H__
#define __EERRNO_H__

#define EEMALLOC         0  /* malloc error */
#define EETOKENCNT       1  /* Token Cnt Error */
#define EEGETNEXTTYPE    2  /* SscanfGetNexType() Get next type error */
#define EEINFILE         3  /* Input file error */
#define EELOCK           4  /* Lock Error  */
#define EENULLCLOSE      5  /* NULL point Close Error */
#define EESIZE           6  /* Size Error  */
#define EEFILEOPEN       7  /* File Open error */
#define EEFILENOTFOUND   8  /* File Not Found  */
#define EETCGETATTR      9  /* tcgetattr error  */
#define EEREAD          10  /* read Error       */
#define EEIPCMSGGET     11  /* IPC Message Queue msgget error  */
#define EEIPCMSGCTL     12  /* IPC Message Queue msgctl error  */
#define EEIPCRECVMQ     13  /* IPC Message Queue recvmq error  */
#define EEIPCSNEDMQ     14  /* IPC Message Queue Send error    */
#define EEISAMTYPE      15  /* EIsam Type Error */
#define EEISAMNAME      16  /* EIsam Name Error */
#define EEISAMINDEXPOS  17  /* EIsam Index Posistion Error */
#define EEISAMTABLE     18  /* EIsam Table Error */
#define EEISAMFIELDPOS  19  /* EIsam Field Position Error */
#define EEISAMBUILD     20  /* EIsam isbuild Error */
#define EEISAMADDINDEX  21  /* EIsam isaddindex Error */
#define EEISAMCREATE    22  /* EIsam CreateDB Error */
#define EEISAMOPEN      23  /* EIsam OpenDB Error   */
#define EEIAMFIELDNAME  24  /* EIsam Field Name Error  */
#define EEISAMINDATA    25  /* EIsam Input Data Error  */
#define EEISAMWRITE     26  /* EIsam iswrite Error */
#define EEISAMINDEX     27  /* EIsam index Error  */
#define EEIPCSEMGET     28  /* IPC Semaphore semget error  */
#define EEIPCSEMCTL     29  /* IPC Semaphore semctl error  */
#define EEIPCSHMAT      30  /* IPC Shared Memory shmat error  */
#define EEIPCSHMCTL     31  /* IPC Shared Memory shmctl error  */
#define EESTATICTIME    32  /* EStatistic Time Error  */

#endif
