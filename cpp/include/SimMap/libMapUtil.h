/**
 * =====================================================================================
 *
 *       @file  libMapUtil.h
 *
 *    	 @brief 
 *
 *       Version:  1.0
 *       Compiler:  cc (HP-UX)
 *       @date:  04/23/09 21:07:04
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

#ifndef _H_LIB_MAP_UTIL_
#define _H_LIB_MAP_UTIL_
#include "flk_type.h"
#include "libMapComm.h"

int encTag ( u_char *Buffer, int tag );
int makeTagString(u_char *cTag, int nTag);
u_char SetBit(unsigned ucData,int nStart,int nLen,int nData);
int PutBitByte(u_char *cpSourStr, int *nStartPos, int nLen,int nData);
u_char In4toV( int		digit, int		position );
int decLength(int *nLen, u_char *cpData, int *npIdx);
int encLength(int nLen, u_char *cpData, int *npIdx);
int encTLV(u_char *cpMsg, void *data, int length, int tag );
u_char *getBufferPtr ( u_char *StartPtr, int *Tot, int MoveSize );
int convertTBCDString(u_char *cpIn, u_char *cpOut, int nLen);
int convertHex2Str(const u_char *cpSource, u_char *cpTarget, const int nLen);
int encTLV_DenyDataSzie(u_char *cpMsg, void *data, int length, int tag, int data_max_size );
int encOctetString(stMsgBuf_t *spMsg, OctetString *pD, int nTag);
int encTotal(stMsgBuf_t *spDest, stMsgBuf_t *spSour, int nIdx, int cTag);
bool isAsciiAlphabet ( u_char *data, u_char len );
void Conv8bit2GSM7bit(stASCIIString  *spSource, stGSM7bitString *spTarget);
void changeASCII8bitToGSM(stASCIIString *spAscii);
void switchMsbToLsb(u_char *dest, int len);
#endif
