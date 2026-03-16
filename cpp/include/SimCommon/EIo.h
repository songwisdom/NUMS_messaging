/***********************************************************************
*                                                                      *
*   Copyright (c) 1997 LG Information & Communications                 *
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
*       MODULE NAME          : EIo.h
*       PURPOSE              : Expand I/O.
*       INPUT ARGUMENT       : NONE
*       OUTPUT ARGUMENT      :
*       RETURN VALUE         : NONE
*       REVISION             :     BY             WHEN            REASON
*                             -------------  --------------  -------------
*                              KANG HYUN KWAN  1997.08.25.     first coding
==============================================================================*/
#ifndef __EIO_H__
#define __EIO_H__
#include "flk_type.h"
#define CTRL_A 0X01
#define CTRL_B 0X02
#define CTRL_C 0X03
#define CTRL_D 0X04
#define CTRL_E 0X05
#define CTRL_F 0X06
#define CTRL_G 0X07
#define CTRL_H 0X08
#define CTRL_I 0X09
#define CTRL_J 0X0A
#define CTRL_K 0X0B
#define CTRL_L 0X0C
#define CTRL_M 0X0D
#define CTRL_N 0X0E
#define CTRL_O 0X0F
#define CTRL_P 0X10
#define CTRL_Q 0X11
#define CTRL_R 0X12
#define CTRL_S 0X13
#define CTRL_T 0X14
#define CTRL_U 0X15
#define CTRL_V 0X16
#define CTRL_W 0X17
#define CTRL_X 0X18
#define CTRL_Y 0X19
#define CTRL_Z 0X1A
#define ESC    0x1B

#define EKEY_F1     0x1b4f50
#define EKEY_UP     0x1b5b41
#define EKEY_DOWN   0x1b5b42
#define EKEY_RIGHT  0x1b5b43
#define EKEY_LEFT   0x1b5b44

#define EKEY_EXPAND_ARROW 0x1b5b

#define EKEY_EXPAND_ARROW_UP    0x41
#define EKEY_EXPAND_ARROW_DOWN  0x42
#define EKEY_EXPAND_ARROW_RIGHT 0x43
#define EKEY_EXPAND_ARROW_LEFT  0x44

#define EKEY_INSERT_STRATUS 0x1b40
#define EKEY_INSERT_HP      0x1b5b327e

#define EKEY_DELETE 0x7f
#define EKEY_HOME   0x01
#define EKEY_END    0x05
#define EKEY_PGUP   0x1b76
#define EKEY_PGDOWN 0x16

typedef struct
{
    int nXpos;
    int nYpos;
    char *cpStr;
} EMenuButtonStrType;


typedef struct 
{
    int nNum;
    EMenuButtonStrType *stStr;
    int nPos;
} EMenuButtonType;


#ifdef __cplusplus
extern "C" {
#endif
void SaveKeyHistoryBuf(char *cpAppName);
void LoadKeyHistoryBuf(char *cpAppName);
FILE *Fopen(char *cpFileName,char *mode);
FILE *Fdopen(int nfd,char *mode);
void Fclose(FILE *fp);
FILE *LockFopen(char *cpFileName,char *cpMode);
void LockFclose(FILE *fp);
int SetTtyCbreak(int nFd);
int SetTtyRaw(int nFd);
int ResetTty(int nFd);
int SetOldTtyState(int nFd);
int GetGetCharRawBufCnt(void);
void PushGetCharRawBuf(char *cpBuf,int nLen);
int PopGetCharRawBuf(void);
int GetCharRaw(void);
void SetStrRawPrompt(char *cpStr);
void DisplayGetStrData(char *cpStr,int nPos);
int GetStrRaw(char *cpBuf);
int GetRows(void);
#ifdef __cplusplus
}
#endif

#endif
