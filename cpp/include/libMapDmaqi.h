/**
 * =====================================================================================
 *
 *       @file  libMapDmaqi.h
 *
 *    	 @brief
 *
 *       Version:  1.0
 *       Compiler:  cc (HP-UX)
 *       @date:  04/10/09 11:46:23
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

#ifndef _H_LIB_MAP_DMAQI_
#define _H_LIB_MAP_DMAQI_
#include <FLKBaseDef.h>
#include <MdbTableInfo.h>
#include <MSCapability.h>
#include <Map_Struct.h>

/* LongMsg.c */
u_short getTIDbyCapa(u_char ucMsCapa);
void getLongTID(void);
int isLongTi(u_char *ucpTeleSvcId);
const char *CvtCapaToText(eTermCapa ucTermCapa);
/* SpamMsg.c */
int IsSPAMCallBack(u_char *ucpCallBack, u_char *ucpSpamFlag, size_t siSpamSize, u_long ulMaxPtn);
/* mapDmaqi.c */
int remakeLink(int nDbmsHandle, u_int unMesgId, u_char ucStatus, stRDActiveTuple *spActive);
int updateStatus(int nDbmsHandle, u_int unMesgId, u_char ucStatus, u_short usFailReason);
int updateStatusCont( int nDbmsHandle, u_int unMesgId, u_char ucStatus, u_short usFailReason );
int updateRcsStatus( u_int unMesgId, u_char ucStatus, u_short usFailReason);
int remakeRcsLink( u_int unMesgId, u_char ucStatus, stRDActiveTuple *spActive);
/* mapDmaqiDebug.c */
void debugActiveRecord( stRDActiveTuple *spActive );
void debugPrefixRecord( stRDPrefixTuple *spPrefix );
void debugEsmeRecord( stRDEsmeTuple	*spEsme );

/* mapDmaqiEx.c */
int updateFailReason( int nDbmsHandle, u_int	unMesgId, u_short usFailReason );
int selectNPDBPrefix( const u_char *ucMin, int	nMinOctetNum, stRDNpdbPrefixTuple *spPrefix );
int selectAddress( u_char *ucMin, int nMinOctetNum, stSmsAddr *spAddr );
int selectRNAddress( u_char *ucMin, int nMinOctetNum, stSmsAddr *spAddr );
int selectSMCAddress( u_char *ucMin, int nMinOctetNum, stSmsAddr *spAddr );
int selectPrefix( u_char *ucMin, int nMinOctetNum, stRDPrefixTuple *spPrefix );
int selectUdmGrpPrefix( u_char *ucMin, int nMinOctetNum, stRDS6CPrefixTuple *spPrefix, int appid);
int selectServiceRec( u_char ucServiceId, stRDServiceTuple *stpService );
int selectSMCPrefix( u_char *ucMin, int nMinOctetNum, stRDPrefix_SMCTuple *spPrefix );
int selectM2MGlrRoutePrefix( u_char *ucMin, int nMinOctetNum ); //SR410 pkg
bool isM2MGlrRouteDenyMIN(u_char *cpSrc, int length); //SR410 pkg
bool isM2MGlrRouteAllowMIN(u_char *cpSrc, int length); //SR410 pkg
int selectRNPrefix( u_char *ucMin, int nMinOctetNum, stRDPrefix_RNTuple *spPrefix );
int selectVMinPrefix( u_char *ucMin, int nMinOctetNum, stRDVMIN_PrefixTuple *spPrefix );
int checkVMinPrefix(u_char *ucMin, int nLength);
int selectIsdnRec( u_char *usIsdn, stRDIsdnTuple *spIsdn );
int selectDiaSmscIdRec(u_char *ucScPc, stRDiameterSMSCTuple *spIsdn);	//SR720 pkg
int select_udm_info( u_int usPc, stRDUdmInfoTuple *spUdmInfo); //SR420 pkg
int select_udm_bk_grp_info( u_char ucHlrGrpNum, stRDUdmBkGrpTuple *spUdmBkGrp); //SR510 pkg
//int selectSpecialMin(u_short usServiceCode, u_char *ucMin, stRDConvertDestData *spSpecialMin);
int selectSpecialMin(u_short usServiceCode, u_char *ucMin, stRDConvertSpecialMinTuple *spSpecialMin);
int selectVerifyCharset(u_int in_dcs, u_int in_charset, u_int out_dcs, u_int *out_chartset);

u_int GetMdbPrefixAddress(stRDPrefixTuple *spPrefix);
u_int GetMdbPrefixSMCAddress(stRDPrefix_SMCTuple *spPrefix);
#endif
