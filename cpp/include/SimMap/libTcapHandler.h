/**
 * =====================================================================================
 *
 *       @file  libTcapHandler.h
 *
 *    	 @brief 
 *
 *       Version:  1.0
 *       Compiler:  cc (HP-UX)
 *       @date:  04/23/09 21:25:18
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

#ifndef _H_LIB_TCAP_HANDLER_
#define _H_LIB_TCAP_HANDLER_
#include "flk_type.h"
#include "TCAP_ext.h"
#include "TCAP_errors.h"
#include "libMapComm.h"
#include "libMapV3Extension.h"
#include "libSimTable.h"

typedef struct tcap_app_info_t 
{
	int			nCnxId;	
	u_int		nProvId;
	u_int		nUserId;
	int			nMySSN;
	int			nAppId;
	int			nInsId;
	int			nTimeOut;
	int			nTimeGap;
	int			nOPC;
	TC_BOOL 	DialogFlag;
	APP_ID_TYPE AppType;
}tcap_app_info_st;

tcap_app_info_st tcap_app_info;

#ifdef __cplusplus
extern "C" {
#endif
int open_tcap_for_sim (APP_ID_TYPE AppType, int nMySSN, int nOPC, int nTimeOut, 
		int nTimeGap, bool DialogFlag );
FLK_RETURN init_tcap_info (APP_ID_TYPE AppType, int nMySSN, int nOPC, 
		int nTimeOut, int nTimeGap, bool DialogFlag );
typedef int (*InvokeHandlerFunc)(tcx_primitive *spPrim, tcx_component *spComp, tcx_buffer *spBuff);

int openTcap(int nAppId, int nInsId, int nSsn, TC_BOOL bActive );
int encodeComp( tc_component_type type, tcx_component *compPtr, tcx_buffer *buffPtr, 
		u_char cOpCode, int nUserId, int nProvId, stMsgBuf_t *spMsg );
int encodeTransPc( tc_primitive_type type, int nUserId, int nProvId, int nPc, u_char cSsn, Oid *spOid );
void  close_tcap_for_sim();
int BeginInvokeForSim( stMsgBuf_t *spMsg, int nUserId, int nProvId, u_short sPc, u_char cSsn, u_char cOpCode );
int encodeUserAbort( tc_primitive_type type, tcx_primitive *spPrim, u_int nUserId, u_int nProvId, u_char	cAbort );


TC_BOOL error_handler(char *cpDesc);
int sendTcapForSimRet( tc_primitive_type type, tcx_primitive *spOPrim, int nUserId);
void handleConnectForSim( int nErr );
tc_primitive_type decodePrimForSim(tcx_primitive *spPrim);
TC_BOOL decodeCompForSim( tcx_primitive *spPrim, int *npInvkId, InvokeHandlerFunc InvokeHandler);
int recvTcapForSim(InvokeHandlerFunc InvokeHandler);
int ctrlSchedulingForSim(InvokeHandlerFunc InvokeHandler);

int sendCompForSim(
	tc_component_type   type,
	u_char    			   	cOpCode,
	u_int        		nUserId,
	u_int				nProvId,
	int					nIid,
	int					nTimer,
	stMsgBuf_t          *spMsg);
int sendMessage(
	tcx_primitive   	*spPrim,
	tcx_component   	*spComp,
	tc_primitive_type   ptype,
	tc_component_type   ctype,
	u_char					opcode,
	stMsgBuf_t			*spMsg );

int sendMessageDial(
	tcx_primitive       *spPrim,
	tcx_component       *spComp,
	tc_primitive_type   ptype,
	tc_component_type   ctype,
	unsigned char       opcode,
	stMsgBuf_t          *spMsg );

void printBegInvInd( tcx_primitive *spPrim, tcx_component *compPtr, tcx_buffer *buffPtr);
void reconnection();

#ifdef __cplusplus
}
#endif
typedef struct tcap_statistic_t {
	unsigned int tc_invoke;
	unsigned int tc_result_l;
	unsigned int tc_result_nl;
	unsigned int tc_u_error;
	unsigned int tc_l_cancel;
	unsigned int tc_u_cancel;
	unsigned int tc_l_reject;
	unsigned int tc_r_reject;
	unsigned int tc_u_reject;
	unsigned int unknown;
	unsigned int other;
}tcap_statistic_st;
extern tcap_statistic_st tcap_stat;

#ifdef __cplusplus
extern "C" {
#endif
int sendTcapResult( tcx_primitive	*spPrim, tc_primitive_type   ptype);
int sendComponentSim( tcx_primitive	*spPrim, tcx_component *spComp, tc_primitive_type ptype, tc_component_type ctype, unsigned char	opcode, stMsgBuf_t *spMsg );
#ifdef __cplusplus
}
#endif
#endif
