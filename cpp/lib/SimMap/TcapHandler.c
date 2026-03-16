/**
 * =====================================================================================
 *
 *       @file  TcapHandler.c
 *
 *    	 @brief 
 *
 *       Version:  1.0
 *       Compiler:  cc (HP-UX)
 *       @date:  04/29/09 10:46:59
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

#include <TCAP_ext.h>
#include <TCAP_errors.h>
#include "libTcapHandler.h"
#include "libMapComm.h"
#include "libMapV3Extension.h"
#include "libMap2G.h"
#include "libMap3G_Addr.h"
#include "libMap3G_st.h"
#include "libMapDebug.h"

#include "libSimTable.h"
#include <pthread.h>
#include <errno.h>
#include "Debug.h"
#include "TCAP_common.h"

pthread_mutex_t CONN_MUTEX=PTHREAD_MUTEX_INITIALIZER;


int openTcap(int nAppId, int nInsId, int nSsn, TC_BOOL bActive )
{
    tcx_application_info    AppliInfo;
    int                     nCnxId;
    struct timeval          timeout;

    AppliInfo.mode              = TCX_CNX_OVERWRITE; /* must */
    AppliInfo.application_id    = nAppId;
    AppliInfo.instance_id       = nInsId;

    timeout.tv_sec  = 5;
    timeout.tv_usec = 0;

    nCnxId = TCX_open("SS7_Stack",  /* class Name   */
        TCX_TCAP_STD,   			/* must be set to TCX_TCAP_STD */
        nSsn,           			/* subsystem number */
        TC_YES,     				/* use componet layer   */
        bActive,					/* incoming BEGIN requests  */
        &AppliInfo, 				/* for take-over    */
        TCX_SCCP_SERVICE_REGULAR,   /* regular message  */
        &timeout);      			/* must be set to NULL  */

    if(nCnxId == -1) 	return(FLK_FAIL);
    else 				return(nCnxId);
}

int encodeUserAbort( tc_primitive_type type, tcx_primitive *spPrim, 
		u_int nUserId, u_int nProvId, u_char	cAbort )
{
	tcx_primitive   stDPrim;
	memset(&stDPrim, 0x00, sizeof(tcx_primitive));

    TC_P_TYPE(&stDPrim) = type;
    TC_P_SCE_QUALITY(&stDPrim).use_default_values = TC_YES;

	if(TC_P_DLG_INFO_PRESENT(spPrim) == TC_YES)
	{
		stDPrim.dialog_portion.abort_reason = AC_name_not_supported;

		TC_P_DLG_INFO_PRESENT(&stDPrim) = TC_P_DLG_INFO_PRESENT(spPrim);
		TC_P_DLG_PORTION_APP_CONTEXT(&stDPrim).length
			= TC_P_DLG_PORTION_APP_CONTEXT(spPrim).length;
		memcpy(TC_P_DLG_PORTION_APP_CONTEXT(&stDPrim).data
				, TC_P_DLG_PORTION_APP_CONTEXT(spPrim).data
				, TC_P_DLG_PORTION_APP_CONTEXT(&stDPrim).length);
	}

	memcpy(&stDPrim.d_address, &spPrim->o_address, sizeof(tc_address));
	memcpy(&stDPrim.o_address, &spPrim->d_address, sizeof(tc_address));

	/* Set dialog IDs */
    TC_P_USER_ID(&stDPrim)     = nUserId;
    TC_P_PROVIDER_ID(&stDPrim) = TC_P_PROVIDER_ID(spPrim);
	
	if(cAbort != 0x00)
	{
		TC_P_UABORT(&stDPrim).length 		= 1;
		TC_P_UABORT(&stDPrim).datas[0] 	= cAbort;
	}
	else
		TC_P_UABORT(&stDPrim).length 		= 0;

	if(TCX_send(tcap_app_info.nCnxId, NULL, &stDPrim, NULL ) == -1)
    {
        if(tc_errno == TCE_SEND_FULL || tc_errno == TCE_API_BUSY)
        {
            /* IPC buffer congestion:                              */
            /* A real application should stored this message and   */
            /* retry to send it later. Go back to the main         */
            /* loop to keep scheduling the SS7 connections.        */
            /* If incoming messages are present, TCX_recv must be  */
            /* called to ensure the rx buffer will not be flow     */
            /* controlled (This may result in a TC_CNX_CLOSED).    */
            /* In this example, the message is lost and the        */
            /* dialogue remains open until the application ends it */
            /* by sending a TC_U_ABORT or closes the connection    */
        }
        if (TCX_flush_components(tcap_app_info.nCnxId, nUserId, nProvId) == -1)
        {
            return(-1);
        }
    }
    return(1);
}

static int encIid()
{
	static int nIid = 0;
	if(nIid++ >= 30000)
		nIid = 1;
	return(nIid);                                       
}     

#define	OP_CLASS_1	1
#define	OP_CLASS_2	2
#define	OP_CLASS_3	3
#define	OP_CLASS_4	4
int encodeComp( 
		tc_component_type	type,
		tcx_component   	*compPtr, 
		tcx_buffer      	*buffPtr, 
		u_char					cOpCode, 
		int 				nUserId,
		int 				nProvId,
		stMsgBuf_t 			*spMsg 
		)
{
	int				nRet;

    compPtr->c_type             = type;
    compPtr->op_class           = OP_CLASS_1;
    compPtr->invoke_id          = encIid();
    compPtr->linked_id          = -1;
	
	switch(type)
	{
		case TC_RESULT_L	:
			if(spMsg->nLen == 0)
			{
				compPtr->operation.length 	= 0;
    			compPtr->operation.datas[0] = 0;
			}
			else
			{
    			compPtr->operation.tag      = LOCAL_TYPE;
    			compPtr->operation.length   = 1;
    			compPtr->operation.datas[0] = cOpCode;
			}
			break;
		case TC_INVOKE	:
    		compPtr->operation.tag      = LOCAL_TYPE;
    		compPtr->operation.length   = 1;
    		compPtr->operation.datas[0] = cOpCode;

			break;
		case TC_U_ERROR	:
			compPtr->error.tag      = LOCAL_TYPE;
        	compPtr->error.length   = 1;
        	compPtr->error.datas[0] = cOpCode;
			break;
		default			:
			return(FLK_FAIL);
	}

    buffPtr->actual_length      = spMsg->nLen;
    if(buffPtr->actual_length > 0)
	{
        memcpy((char *)(buffPtr->bufferp)
            , (char*)spMsg->cpData, buffPtr->actual_length);
	}

   	compPtr->parameter      = buffPtr;
    compPtr->timer.tv_sec   = tcap_app_info.nTimeOut;
    compPtr->timer.tv_usec  = 0;

    nRet = TCX_put_component(tcap_app_info.nCnxId, nUserId, nProvId, compPtr);
    if(nRet == FLK_FAIL)
    {
        return(FLK_FAIL);
    }
	return(FLK_SUCCESS);
}

#define APP_CONTEXT_TYPE_MAX	4
Oid pV3AppContext[APP_CONTEXT_TYPE_MAX] = { 
	{ 7, { 4, 0, 0, 1, 0, 23, 2} }, // V3_SHORTMSG_ALERT_CONTEXT_V2 
	{ 7, { 4, 0, 0, 1, 0, 21, 3} }, // V3_SHORTMSG_MO_RELAY_CONTEXT_V3  
	{ 7, { 4, 0, 0, 1, 0, 25, 3} }, // V3_SHORTMSG_MT_RELAY_CONTEXT_V3 
	{ 7, { 4, 0, 0, 1, 0, 20, 3} }  // V3_SHORTMSG_GATEWAY_CONTEXT_V3
}; 

typedef enum {
	SHORTMSG_ALERT_CONTEXT      = 0,
	SHORTMSG_MO_RELAY_CONTEXT   = 1,
	SHORTMSG_MT_RELAY_CONTEXT   = 2,
	SHORTMSG_GATEWAY_CONTEXT    = 3
}etAppContextType;

static Oid *getV3AppContext(u_char cOpCode)
{
	switch (cOpCode) {
		case MO_FORWARDSM :
			return &pV3AppContext[SHORTMSG_MO_RELAY_CONTEXT];
		case MT_FORWARDSM :
			return &pV3AppContext[SHORTMSG_MT_RELAY_CONTEXT];
		case SEND_ROUTING_INFO_FOR_SM :
			return &pV3AppContext[SHORTMSG_GATEWAY_CONTEXT];
		case ALERT_SERVICE_CENTRE :
			return &pV3AppContext[SHORTMSG_ALERT_CONTEXT];
		default :
			return NULL;
	}
}

int encodeTransPc( tc_primitive_type type, int nUserId, int nProvId, int nPc, u_char cSsn, Oid *spOid )
{
	tcx_primitive   	stPrim;
	memset(&stPrim, 0x00, sizeof(tcx_primitive));
	tcx_primitive   	*spPrim;
	spPrim = &stPrim;
	
	if(spOid != NULL )
	{
		TC_P_DLG_INFO_PRESENT(spPrim) = TC_YES;
		TC_P_DLG_PORTION_APP_CONTEXT(spPrim).length = spOid->numids;
		memcpy(TC_P_DLG_PORTION_APP_CONTEXT(spPrim).data, 
				spOid->subid, spOid->numids); 
	}
	else
	{
		TC_P_DLG_INFO_PRESENT(spPrim) = TC_NO;
	}

	//memset(spPrim, 0x00, sizeof(tcx_primitive));
	TC_P_TYPE(spPrim) = type;
   	TC_P_SCE_QUALITY(spPrim).use_default_values = TC_YES;
	TC_P_SCE_QUALITY(spPrim).sccp_service_class = TCX_SCCP_CLASS0;

	TC_P_DLG_INFO_PRESENT(spPrim) = TC_NO;

	/* Set destination address */
   	TC_P_D_ADDR(spPrim).type     = DPC_SSN;
   	TC_P_D_ADDR(spPrim).pc_value = nPc;
   	TC_P_D_ADDR(spPrim).ssn      = cSsn;

   	/* Set originating address */
   	TC_P_O_ADDR(spPrim).type     = DPC_SSN;
   	TC_P_O_ADDR(spPrim).pc_value = tcap_app_info.nOPC;
   	TC_P_O_ADDR(spPrim).ssn      = tcap_app_info.nMySSN;

   	/* Set dialog IDs */
   	TC_P_USER_ID(spPrim)     = nUserId;
   	TC_P_PROVIDER_ID(spPrim) = nProvId;

	if(type == TC_END) TC_P_TERMINATION(spPrim) = BASIC;

	if(TCX_send(tcap_app_info.nCnxId, NULL, spPrim, NULL ) == -1)
   	{
      	if(tc_errno == TCE_SEND_FULL || tc_errno == TCE_API_BUSY)
      	{
	
			memset(spPrim, 0x00, sizeof(tcx_primitive));
			encodeUserAbort(TC_U_ABORT, spPrim, nUserId, nProvId, 0x00);
      	}
		
      	if (TCX_flush_components(tcap_app_info.nCnxId, nUserId, nProvId) == -1)
     	{
      	}
		return(-1);
   	}
	return(1);
}

void close_tcap_for_sim()
{
	printf("Close nCnxId [%d]\n", tcap_app_info.nCnxId);
	releaseSimTableInfo ( tcap_app_info.AppType );
	TCX_close(tcap_app_info.nCnxId);
}

FLK_RETURN init_tcap_info (APP_ID_TYPE AppType, int nMySSN, int nOPC, int nTimeOut, int nTimeGap, bool DialogFlag )
{
	int nAppId = 0;
	int nInsId = 0;

	if ( setSimTableInfo ( AppType, &nAppId, &nInsId ) == FLK_FAIL ) {
		puts("Instance Id Get failed");
		return FLK_FAIL;
	}

	tcap_app_info.nAppId 	= nAppId;
	tcap_app_info.nInsId 	= nInsId;
	tcap_app_info.nMySSN 	= nMySSN;
	tcap_app_info.nOPC  	= nOPC;
	tcap_app_info.nTimeOut = nTimeOut;
	tcap_app_info.nTimeGap = nTimeGap;
	tcap_app_info.DialogFlag = (DialogFlag == true) ? (TC_YES) : (TC_NO);
	tcap_app_info.AppType 	= AppType;
	//printf("App ID [%d] Instance ID[%d]\n", nAppId, nInsId);
	return FLK_SUCCESS;
}

int open_tcap_for_sim (APP_ID_TYPE AppType, int nMySSN, int nOPC, int nTimeOut, int nTimeGap, bool DialogFlag )
{
	int nAppId = 0;
	int nInsId = 0;
	int nCnxId = 0;


	if ( init_tcap_info(AppType, nMySSN, nOPC, nTimeOut, nTimeGap, DialogFlag) == FLK_FAIL )
		return FLK_FAIL;

	if ( (nCnxId = openTcap(tcap_app_info.nAppId, tcap_app_info.nInsId, tcap_app_info.nMySSN, tcap_app_info.DialogFlag)) == FLK_FAIL) {
		puts("Open Tcap Failed");
		return FLK_FAIL;
	}

	tcap_app_info.nCnxId = nCnxId;
	printf("Open nCnxId [%d]\n", nCnxId);

	return nCnxId;
}

void reconnection()
{
	static int reconn = 0;	
	int rc = FLK_SUCCESS;
	int nCnxId = 0;

	pthread_mutex_lock(&CONN_MUTEX);


	//if ( tcap_app_info.nCnxId < 0 ) {
		TCX_close(tcap_app_info.nCnxId);
		printf("Recoonection (%d)\n", reconn);
		sleep(3);
		if ( (nCnxId = openTcap(tcap_app_info.nAppId, tcap_app_info.nInsId, tcap_app_info.nMySSN, tcap_app_info.DialogFlag)) == FLK_FAIL) {
			puts("Open Tcap Failed");
			rc = FLK_FAIL;
			if ( reconn++ > 10 ) {
				printf("Reconnection Count %d ---> exit\n", reconn);
				close_tcap_for_sim();
				exit(0);
			}

		}
		else  {
			rc = FLK_SUCCESS;
		}
		tcap_app_info.nCnxId = nCnxId;
	//}
	pthread_mutex_unlock(&CONN_MUTEX);

	if ( rc == FLK_FAIL )
		reconnection();
	else 
		reconn = 0;
}
void handleConnect(int nErr)
{
	printf("Error : [%s][%d]\n", tc_error_text[nErr], nErr);
	switch(nErr)
	{
		case TCE_NO_ERROR		:
			break;
		case TCE_WRONG_IDS		:
			break;
		case TCE_SYNTAX_ERROR 	:
		default					:
			reconnection();
			break;
	}
    return;
}

int BeginInvokeForSim( stMsgBuf_t *spMsg, int nUserId, int nProvId, u_short sPc, u_char cSsn, u_char cOpCode )
{
	int             nRet;
	int				nIdx;
	tcx_primitive	stPrimitive;
    tcx_component   *compPtr;
    tcx_buffer      *buffPtr;
	Oid 			*spOid = NULL;
	
	/* Component */
	if(TCX_alloc_component(&compPtr, 1) < 0)
    {
        return(-1);
    }
    if(TCX_alloc_buffer(&buffPtr, spMsg->nLen+1) < 0)
    {
        return(-1);
    }
	if(encodeComp( TC_INVOKE, compPtr, buffPtr, cOpCode , nUserId, nProvId, spMsg) < 0)
    {
        return(-1);
    }

	spOid = getV3AppContext(cOpCode);

	/* Dialogue */
	if(encodeTransPc( TC_BEGIN, nUserId, nProvId, sPc, cSsn, spOid) < 0)
    {
        return(-1);
    }

	return(1);
}

TC_BOOL error_handler(char *cpDesc)
{

  switch (tc_errno)
  {
    case TCE_ILLEGAL_VALUE :
		puts("E: TCE_ILLEGAL_VALUE");
		return -1;
    case TCE_WRONG_IDS :
		puts("E: TCE_WRONG_IDS");
       return -1;
    case TCE_NOT_IMPLEMENTED :
	   puts("E: TCE_NOT_IMPLEMENTED");
       return -1;
    case TCE_ILLEGAL_CALL :
	   puts("E: TCE_ILLEGAL_CALL");
       return -1;
    case TCE_NO_CONFIG :
	   puts("E: TCE_NO_CONFIG");
       return -1;
    case TCE_BAD_CONFIG :
	   puts("E: TCE_BAD_CONFIG");
       return -1;
    case TCE_CONNECT_INIT :
	   puts("E: TCE_CONNECT_INIT");
       return -1;
    case TCE_INVALID_SSN :
	   puts("E: TCE_INVALID_SSN");
       return -1;
    case TCE_MAX_USERS :
	   puts("E: TCE_MAX_USERS");
       return -1;
    case TCE_INTERNAL :
	   puts("E: TCE_INTERNAL");
       return -1;
    case TCE_MEMORY_ERROR :
	   puts("E: TCE_MEMORY_ERROR");
       return -1;
    case TCE_CNX_CLOSED :
	   puts("E: TCE_CNX_CLOSED");
       /* If closeOnEnable parmater is set to YES in sys.tcap file,
        * an Active application connection is closed when a standby
        * application is Activated. In that case, a MGT primitive
        * with a stat_type field set to DEACTIVATE is received before
        * the connection is closed. The connection should be re-opened
        * has Standby.
        */
	   puts("E: TCE_CNX_CLOSED");
	   reconnection();
       return( TC_YES );
    case TCE_CNX_ID :
	   puts("E: TCE_CNX_ID");
	   reconnection();
       return( TC_YES );
    case TCE_SYNTAX_ERROR :
	   puts("E: TCE_SYNTAX_ERROR");
    case TCE_SEND_FULL :
	   puts("E: TCE_SEND_FULL");
    case TCE_API_BUSY :
	   puts("E: TCE_API_BUSY");
    case TCE_COMPONENT_ERROR :
	   puts("E: TCE_COMPONENT_ERROR");
    case TCE_TOO_MANY_COMPONENTS :
	   puts("E: TCE_TOO_MANY_COMPONENTS");
       break;
    default :
	   break;
  }

  return( TC_NO );
}

int sendTcapForSimRet(tc_primitive_type   type, tcx_primitive       *spOPrim, int					nUserId)
{
	tcx_primitive	stDPrim;

    memset(&stDPrim, 0x00, sizeof(tcx_primitive));
    TC_P_TYPE(&stDPrim) = type;
    TC_P_SCE_QUALITY(&stDPrim).use_default_values = TC_YES;

	if(type == TC_U_ABORT)
	{
		stDPrim.dialog_portion.abort_reason = AC_name_not_supported;
	}

	if(TC_P_DLG_PORTION_APP_CONTEXT(spOPrim).length > 0)
	{
		TC_P_DLG_INFO_PRESENT(&stDPrim) = TC_P_DLG_INFO_PRESENT(spOPrim);
		TC_P_DLG_PORTION_APP_CONTEXT(&stDPrim).length 
			= TC_P_DLG_PORTION_APP_CONTEXT(spOPrim).length;
		memcpy(TC_P_DLG_PORTION_APP_CONTEXT(&stDPrim).data
			, TC_P_DLG_PORTION_APP_CONTEXT(spOPrim).data
			, TC_P_DLG_PORTION_APP_CONTEXT(&stDPrim).length);
	}

    memcpy(&stDPrim.d_address, &spOPrim->o_address, sizeof(tc_address));
    memcpy(&stDPrim.o_address, &spOPrim->d_address, sizeof(tc_address));

   	/* Set originating address */
	TC_P_O_ADDR(&stDPrim).type            = DPC_SSN;
	TC_P_O_ADDR(&stDPrim).pc_value = TC_P_D_ADDR(spOPrim).pc_value;
	TC_P_O_ADDR(&stDPrim).ssn      = TC_P_D_ADDR(spOPrim).ssn;

   	/* Set destination address */
    TC_P_D_ADDR(&stDPrim).type            = DPC_SSN;
	TC_P_D_ADDR(&stDPrim).pc_value = TC_P_O_ADDR(spOPrim).pc_value;
	TC_P_D_ADDR(&stDPrim).ssn      = TC_P_O_ADDR(spOPrim).ssn;

    /* Set dialog IDs */
	//printf ("TC_P_USERID[%d] vs nUserId[%d]\n", TC_P_USER_ID(spOPrim), nUserId);
    TC_P_USER_ID(&stDPrim)     = tcap_app_info.nUserId;
    //TC_P_USER_ID(&stDPrim)     = TC_P_USER_ID(spOPrim);;
    TC_P_PROVIDER_ID(&stDPrim) = tcap_app_info.nProvId;
    //TC_P_PROVIDER_ID(&stDPrim) = TC_P_PROVIDER_ID(spOPrim);

    if(type == TC_END) TC_P_TERMINATION(&stDPrim) = BASIC;

    if(TCX_send(tcap_app_info.nCnxId, NULL, &stDPrim, NULL ) == -1)
    {
		error_handler("sendTcapForSim");
        if(tc_errno == TCE_SEND_FULL || tc_errno == TCE_API_BUSY)
        {
            memset(&stDPrim, 0x00, sizeof(tcx_primitive));
            encodeUserAbort(TC_U_ABORT, &stDPrim, nUserId, TC_P_PROVIDER_ID(spOPrim), 0x00);
			return(1);
        }
        if(TCX_flush_components(tcap_app_info.nCnxId, nUserId,TC_P_PROVIDER_ID(spOPrim)) == -1)
        {
            return(-1);
        }
    }

    return(1);
}
void handleConnectForSim(int     nErr)
{
    switch(nErr)
    {
        case TCE_NO_ERROR       :
            break;
        case TCE_WRONG_IDS      :
            break;
        case TCE_SYNTAX_ERROR   :
        default                 :
			reconnection();
            break;
    }
    return;
}


tc_primitive_type decodePrimForSim(tcx_primitive *spPrim)
{
    char    name[80];
    switch (TC_P_TYPE(spPrim))
    {
        case TC_BEGIN:
            sprintf (name,"TC_BEGIN");
            break;
        case TC_CONTINUE:
            sprintf (name, "TC_CONTINUE");
            break;
        case TC_END:
            sprintf (name, "TC_END");
            break;
        case TC_U_ABORT:
            sprintf (name,"TC_U_ABORT");
            break;
        case TC_P_ABORT:
            sprintf (name,"TC_P_ABORT");
            break;
        case TC_NOTICE:
            sprintf (name,"TC_NOTICE");
            break;
        case MGT:
            sprintf (name,"MGT");
            break;
        case SCCP_USER_STATUS:
            sprintf (name,"SCCP_USER_STATUS");
            break;
        case SCCP_PC_STATUS:
            sprintf (name,"SCCP_PC_STATUS");
            break;
        case NO_PRIMITIVE:
            sprintf (name,"NO_PRIMITIVE");
            break;
        case TC_UNI:
            sprintf (name,"TC_UNI");
            break;
        default:
            return((tc_primitive_type) -1 );
    }
    return (TC_P_TYPE(spPrim));
}

TC_BOOL decodeCompForSim( tcx_primitive *spPrim, int *npInvkId, InvokeHandlerFunc InvokeHandler)
{
    char            name[80] = {0x0, };
    tcx_component   *stComp;
    tcx_buffer      *stBuff;
    TC_BOOL         More;
    unsigned int    nRet;

    if((tcap_app_info.nProvId == 0) && (tcap_app_info.nUserId == 0)) /* This may be SCCP_STATUS */
    {
        return(-1);
    }

    /* It must be free component at this end. */
    if(TCX_get_component(tcap_app_info.nCnxId, tcap_app_info.nUserId, tcap_app_info.nProvId, &stComp) == -1)
    {
         //   , tc_error_text[tc_errno], tcap_app_info.nUserId, tcap_app_info.nProvId);
        return(-1);
    }
    *npInvkId   = stComp->invoke_id;
    stBuff      = stComp->parameter;

    switch(stComp->c_type)
    {
        case TC_INVOKE:
			if(TC_P_TYPE(spPrim) == TC_BEGIN || TC_P_TYPE(spPrim) == TC_CONTINUE )
			{
				printBegInvInd(spPrim, stComp, stBuff);
				InvokeHandler(spPrim, stComp, stBuff);
			}
            break;

        case TC_RESULT_L:
            break;

        case TC_RESULT_NL:
            break;

        case TC_U_ERROR:
            break;

        case TC_L_CANCEL:
            if(encodeUserAbort(TC_U_ABORT, spPrim, tcap_app_info.nUserId, tcap_app_info.nProvId, 0x00) < 0)
            break;

       	case TC_U_CANCEL:
            break;

        case TC_L_REJECT:
            break;

        case TC_R_REJECT:
            break;
        case TC_U_REJECT:
            break;
        default:
            break;
    }

    if(stComp->next_component != NULL)  More = TRUE;
    else                                More = FALSE;

    if(TCX_free_components(stComp) == -1)
    {
        return(-1);
    }
    return(More);
}

int recvTcapForSim(InvokeHandlerFunc InvokeHandler)
{
    tcx_primitive       stPrim;
    tc_primitive_type   Ptype;
    int                 nMore;
    int                 nResult;
    int                 nInvkId;
    TC_BOOL             MoreComp;

    do
    {
        nResult = TCX_recv(tcap_app_info.nCnxId, NULL, &stPrim, NULL, &nMore);
        if(nResult < 0)
        {
            handleConnectForSim(tc_errno); return(-1);
        }
        else if(nResult == 0) {}    /* no message received */
        else if(nResult > 0)        /* message received */
        {
            Ptype   = decodePrimForSim(&stPrim);
            if(Ptype == -1) return(1);
            tcap_app_info.nProvId = TC_P_PROVIDER_ID(&stPrim);
            tcap_app_info.nUserId = TC_P_USER_ID(&stPrim);

            if(nResult != NO_COMPONENTS) /* Message contains components */
            {
                do
                {
                    MoreComp = decodeCompForSim(&stPrim, &nInvkId, InvokeHandler);
                } while(MoreComp == TC_YES); /* componets decoding */
            }
			else if((Ptype == TC_BEGIN) && (nResult == NO_COMPONENTS))
			{
				sendTcapForSimRet(TC_END, &stPrim, 1);
			}
        }
    } while(nMore > 0);
    return(1);
}
typedef struct SimRecvData_t
{
	tcx_primitive 	stPrim;
	tcx_component 	stComp;
	tcx_buffer 		stBuff;
}SimRecvData_st;


#define MAX_FD 127
int  ctrlSchedulingForSim(InvokeHandlerFunc InvokeHandler)
{
    fd_set          rdMask;
    fd_set          wrMask;
    fd_set          exMask;
    struct timeval  tmout, *spTime = &tmout;
    int             nNumFd;
    int             nSelect;
    int             nNumCnx;
    int             nCnxActive[MAX_FD];
	int 			err;


    /* init select bit masks and timer */
    FD_ZERO(&rdMask);
    FD_ZERO(&wrMask);
    FD_ZERO(&exMask);

    tmout.tv_sec    = 2;
    tmout.tv_usec   = 0;
    spTime          = &tmout;

    nNumFd = TCX_select_mask(0, &rdMask, &wrMask, &exMask, &spTime);
    nSelect=select(nNumFd,&rdMask,&wrMask,&exMask,spTime);
	err = errno;
	if ( nSelect < 0 ) {
		printf("Error Select Failed (%d) : %s\n", err, strerror(err));
	}
    nNumCnx = MAX_FD;
    if(TCX_cnx_handler(nSelect,&rdMask,&wrMask,&exMask,&nNumCnx,nCnxActive)<0)
    {
		err = errno;
        printf("Error TC_cnx_handler , reason [%d] : %s\n", err, strerror(err));
		reconnection();
		return -1;
    }

    if((nNumCnx > 0) && (nCnxActive[0] == tcap_app_info.nCnxId))
	{
        return recvTcapForSim(InvokeHandler);
	}
	else 
	{
		return 1;
	}
}

int sendCompForSim(tc_component_type   type, unsigned char       cOpCode, unsigned int        nUserId, unsigned int		nProvId, int					nIid, int					nTimer, stMsgBuf_t          *spMsg)
{
	int             nRet;

    tcx_component   *compPtr;
    tcx_buffer      *buffPtr;

    if(TCX_alloc_component(&compPtr, 1) < 0)
    {
        return(-1);
    }
    if(TCX_alloc_buffer(&buffPtr, spMsg->nLen+1) < 0)
    {
        return(-1);
    }

    compPtr->c_type             = type;
    compPtr->op_class           = 1;		/* 1..4 */
    compPtr->invoke_id          = nIid;
    compPtr->linked_id          = -1;


    switch(type)
    {
        case TC_RESULT_L    :
            if(spMsg->nLen == 0)
            {
                compPtr->operation.length   = 0;
                compPtr->operation.datas[0] = 0;
            }
            else
            {
                compPtr->operation.tag      = LOCAL_TYPE;
                compPtr->operation.length   = 1;
                compPtr->operation.datas[0] = cOpCode;
            }
            break;
        case TC_INVOKE  :
            compPtr->operation.tag      = LOCAL_TYPE;
            compPtr->operation.length   = 1;
            compPtr->operation.datas[0] = cOpCode;
            break;
        case TC_U_ERROR :
            compPtr->error.tag      = LOCAL_TYPE;
            compPtr->error.length   = 1;
            compPtr->error.datas[0] = cOpCode;
            break;
        default         :
            return(-1);
    }

    buffPtr->actual_length      = spMsg->nLen;
    if(buffPtr->actual_length > 0)
        memcpy((char *)(buffPtr->bufferp)
            , (char*)spMsg->cpData, buffPtr->actual_length);


    compPtr->parameter      = buffPtr;
    compPtr->timer.tv_sec   = nTimer;
    compPtr->timer.tv_usec  = 0;

    nRet = TCX_put_component(tcap_app_info.nCnxId,nUserId,nProvId,compPtr);

    if(nRet == -1)
    {
        printf("sendCompForSim:TCX_put_component fail...[%s][%d] %d %d \n"
			,tc_error_text[tc_errno], tc_errno, nUserId, nProvId);
        return(-1);
    }
    return(1);
}

int sendComponentSim(
tcx_primitive   	*spPrim,
tcx_component   	*spComp,
tc_primitive_type   ptype,
tc_component_type   ctype,
unsigned char		opcode,
stMsgBuf_t			*spMsg )
{
    if(sendCompForSim(ctype, opcode, tcap_app_info.nUserId, TC_P_PROVIDER_ID(spPrim) , spComp->invoke_id, 6, spMsg)<0)  {
		return(-1);
	}else  {
		return (1);
	}
}
int sendTcapResult( tcx_primitive   	*spPrim, tc_primitive_type   ptype)
{
    if(sendTcapForSimRet(ptype, spPrim, tcap_app_info.nUserId) < 0) {
        return(-1);
	}else
		return 1;
}

int sendMessage(
tcx_primitive   	*spPrim,
tcx_component   	*spComp,
tc_primitive_type   ptype,
tc_component_type   ctype,
unsigned char		opcode,
stMsgBuf_t			*spMsg)
{ 	
	static size_t succ = 0;
	//printf("App ID [%d] Instance ID[%d]\n", tcap_app_info.nAppId, tcap_app_info.nInsId);
    if(sendCompForSim(ctype, opcode, tcap_app_info.nUserId, TC_P_PROVIDER_ID(spPrim)
		, spComp->invoke_id, 6, spMsg)<0)  {
		return(-1);
	}

    if(sendTcapForSimRet(ptype, spPrim, tcap_app_info.nUserId) < 0) {
        return(-1);
	}
	//fprintf( stdout, "\rSuccess [%u]", succ++);
	fflush(stdout);

	return(1);
}

int
sendMessageDial(
tcx_primitive       *spPrim,
tcx_component       *spComp,
tc_primitive_type   ptype,
tc_component_type   ctype,
unsigned char       opcode,
stMsgBuf_t          *spMsg)
{
/*
    if(sendCompForSim(ctype, opcode, 1, TC_P_PROVIDER_ID(spPrim)
        , spComp->invoke_id, 6, spMsg)<0)
        return(-1);
*/
    if(sendTcapForSimRet(ptype, spPrim, 1) < 0) {
        return(-1);
	}

    return(1);
}


void dbgstr(unsigned char *data, int cnt, int rate)
{
#if 0
	int		nIdx;

	for(nIdx=0; nIdx<cnt; nIdx++)
	{
		if((nIdx!=0) && ((nIdx%rate) == 0)) log_write(LEV_ALL_PARAM, "\n");
		log_write(LEV_ALL_PARAM, "%02x ", data[nIdx]);
	}
	log_write(LEV_ALL_PARAM, "\n");
#endif
}

void dbgStr(unsigned char *cpData, int nCnt, int nUnit)
{
#if 0
    int     nIdx;
    for(nIdx=0; nIdx<nCnt; nIdx++)
    {
        if((nIdx!=0) && ((nIdx%nUnit)==0))
            log_write(LEV_ALL_PARAM,  "\n");
        log_write(LEV_ALL_PARAM,  "%02x ", cpData[nIdx]);
    }
    log_write(LEV_ALL_PARAM,  "\n");
#endif
}

void dbgData(stMsgBuf_t *spMsg)
{
#if 0
    int     nIdx;
    log_write(LEV_ALL_PARAM, "len [%d]\n", spMsg->nLen);
    for(nIdx=0; nIdx<spMsg->nLen; nIdx++)
    {
        if((nIdx != 0) && ((nIdx%10) == 0)) log_write(LEV_ALL_PARAM, "\n");
        log_write(LEV_ALL_PARAM, "%02x ", spMsg->cpData[nIdx]);
    }
    log_write(LEV_ALL_PARAM, "\n");
#endif
}


void dbgOctetString(char *cpDesc, OctetString *pD)
{
	log_writeHex(LEV_ALL_PARAM, cpDesc, (char *)pD->data, pD->numocts, 0);
}

void dbgAddTypeString(char *cpDesc, AddTypeString *pD)
{
	/*
    log_write(LEV_ALL_PARAM, "[%s] : [%d] \n", cpDesc, pD->numocts);
    log_write(LEV_ALL_PARAM, "TypeOfDigits   [%x]\n", pD->m.TypeOfDigits);
    log_write(LEV_ALL_PARAM, "NatureOfNumber [%x]\n", pD->m.NatureOfNumber);
    log_write(LEV_ALL_PARAM, "NumberingPlan  [%x]\n", pD->m.NumberingPlan);
    log_write(LEV_ALL_PARAM, "Encoding       [%x]\n", pD->m.Encoding);
    log_write(LEV_ALL_PARAM, "NumberOfDigits [%x]\n", pD->m.NumberOfDigits);
    dbgstr(pD->data, pD->numocts, 10);
	*/
}

void dbgSmsAddress(char *cpDesc, SmsAddress *pD)
{
#if 0
    log_write(LEV_ALL_PARAM, "[%s] : [%d] \n", cpDesc, pD->numocts);
    log_write(LEV_ALL_PARAM, "TypeOfDigits   [%x]\n", pD->m.TypeOfDigits);
    log_write(LEV_ALL_PARAM, "NatureOfNumber [%x]\n", pD->m.NatureOfNumber);
    log_write(LEV_ALL_PARAM, "NumberingPlan  [%x]\n", pD->m.NumberingPlan);
    log_write(LEV_ALL_PARAM, "Encoding       [%x]\n", pD->m.Encoding);
    dbgstr(pD->data, pD->numocts, 10);
    log_write(LEV_ALL_PARAM, "pc [%x][%d] ssn [%x]\n", pD->pc, pD->pc, pD->ssn);
#endif
}

void dbgAddressString(char *cpDesc, V3_AddressString *pD)
{
#if 0
	log_write(LEV_ALL_PARAM, "[%s] : Octs [%d]\n", cpDesc, pD->numocts);
	log_write(LEV_ALL_PARAM, "NAI [%d]\n", pD->m.NAI);
	log_write(LEV_ALL_PARAM, "NP  [%d]\n", pD->m.NP);
	log_writeHex(LEV_ALL_PARAM, "Address : ", (char *)pD->data, pD->numocts, 0);
#endif
}

void dbgV3RIFSMR(V3_RoutingInfoForSM_Arg *pD)
{
	/*
    log_write(LEV_ALL_PARAM, "========== V3_RoutingInfoForSM_Arg  ........... start\n");
    dbgAddressString("msisdn"   , (AddressString*)&pD->msisdn);
	log_write(LEV_ALL_PARAM, "[%s] : [%x]\n", "priotiry", pD->sm_RP_PRI);
    dbgAddressString("sca"      , (AddressString*)&pD->serviceCentreAddress);
	if(pD->m.extensionContainerPresent)
	log_write(LEV_ALL_PARAM, "extensionContainerPresent\n");
	if(pD->m.gprsSupportIndicatorPresent)
	log_write(LEV_ALL_PARAM, "gprsSupportIndicatorPresent\n");
	if(pD->m.sm_RP_MTIPresent)
	log_write(LEV_ALL_PARAM, "[%s] : [%x]\n", "MTI", pD->sm_RP_MTI);
	if(pD->m.sm_RP_SMEAPresent)
	dbgOctetString("SMEA", (OctetString*)&pD->sm_RP_SMEA);
    log_write(LEV_ALL_PARAM, "========== V3_RoutingInfoForSM_Arg  ........... end\n");
	*/
}

void
dbgDialogue(tc_dialog_portion * pD)
{
#if 0
	if(!pD->dlg_info_present) 
	{
		log_write(LEV_ALL_PARAM, "[Dialogue Info Not Present]\n");
		return;
	}
	log_write(LEV_ALL_PARAM, "[Dialogue Info] : [%d]\n", pD->application_context_name.length);
    dbgstr(pD->application_context_name.data
		, pD->application_context_name.length, 10); 
	log_write(LEV_ALL_PARAM, "[abort_reason] : [%d]\n", pD->abort_reason);
#endif
}

void
dbgV3RIFSMI(V3_RoutingInfoForSM_Res *pD)
{
#if 0
    log_write(LEV_ALL_PARAM, "========== V3_RoutingInfoForSM_Res  ........... start\n");
    log_writeHex(LEV_ALL_PARAM, "IMSI : ", (char *)pD->imsi.data, pD->imsi.numocts, 0);
    dbgAddressString("Network Node Number", (V3_AddressString*)&pD->locationInfoWithLMSI.networkNode_Number);
	
	if(pD->locationInfoWithLMSI.m.lmsiPresent)
		log_writeHex(LEV_ALL_PARAM, "LMSI", (char *)pD->locationInfoWithLMSI.lmsi.data, pD->locationInfoWithLMSI.lmsi.numocts, 0);

	if(pD->locationInfoWithLMSI.m.extensionContainerPresent)
		log_write(LEV_ALL_PARAM, "[locationInfoWithLMSI.m.extensionContainerPresent]\n");
	if(pD->locationInfoWithLMSI.m.gprsNodeIndicatorPresent);
		log_write(LEV_ALL_PARAM, "[locationInfoWithLMSI.m.gprsNodeIndicatorPresent]\n");

	if(pD->locationInfoWithLMSI.m.additional_NumberPresent)
	{
		log_write(LEV_ALL_PARAM, "[locationInfoWithLMSI.m.additional_NumberPresent]\n");
		switch(pD->locationInfoWithLMSI.additional_Number.t)
		{
			case V3_Additional_Number_msc_Number	:
				dbgAddressString("V3_Additional_Number_msc_Number"
					, (V3_AddressString*)&pD->locationInfoWithLMSI.additional_Number.u.msc_Number);
				break;
			case V3_Additional_Number_sgsn_Number	:
				dbgAddressString("V3_Additional_Number_sgsn_Number"
					, (V3_AddressString*)&pD->locationInfoWithLMSI.additional_Number.u.sgsn_Number);
				break;
			default	:
				break;
		}
	}
	if(pD->m.extensionContainerPresent && pD->extensionContainer.m.privateExtensionListPresent ) {
		V3_PrivateExtensionList *ptr = &pD->extensionContainer.privateExtensionList;
		log_write(LEV_ALL_PARAM, "[EXTENSIONcONTAINER : Count [%d]]\n", ptr->n);
		if ( ptr->m.minPresent ) {
			log_writeHex(LEV_ALL_PARAM, "MIN : ", (char *)ptr->min.data, ptr->min.numocts, 0);
		}
		if ( ptr->m.esnPresent ) {
			log_writeHex(LEV_ALL_PARAM, "ESN : ", (char *)ptr->esn.data, ptr->esn.numocts, 0);
		}
		if ( ptr->m.capaPresent ) {
			log_writeHex(LEV_ALL_PARAM, "CAPA : ",(char *) ptr->capa.data, ptr->capa.numocts, 0);
		}
		if ( ptr->m.osfiPresent ) {
			log_writeHex(LEV_ALL_PARAM, "OSFI : ",(char *) ptr->osfi.data, ptr->osfi.numocts, 0);
		}
		if ( ptr->m.roamPresent ) {
			log_writeHex(LEV_ALL_PARAM, "ROAM : ", (char *)ptr->roam.data, ptr->roam.numocts, 0);
		}
		if ( ptr->m.msisdnPresent ) {
			log_writeHex(LEV_ALL_PARAM, "FOWARD MSISDN : ",(char *) ptr->msisdn.data, ptr->msisdn.numocts, 0);
		}
		if ( ptr->m.smsf_msisdnPresent ) {
			log_writeHex(LEV_ALL_PARAM, "SMSF Number ID : ",(char *) ptr->smsf_msisdn.data, ptr->smsf_msisdn.numocts, 0);
		}
		if ( ptr->m.adrPresent ) {
			log_writeHex(LEV_ALL_PARAM, "ADR : ", (char *)ptr->adr.data, ptr->adr.numocts, 0);
		}
		if ( ptr->m.roampmnPresent ) {
			log_writeHex(LEV_ALL_PARAM, "ROAM PMN : ", (char *)ptr->roampmn.data, ptr->roampmn.numocts, 0);
		}
	}

    log_write(LEV_ALL_PARAM, "========== V3_RoutingInfoForSM_Res  ........... end\n");
#endif
}


void dbgV3MOFSMR(V3_MO_ForwardSM_Arg *pD)
{
	/*
    log_write(LEV_ALL_PARAM, "========== V3_MO_ForwardSM_Arg  ........... Start\n");
    switch(pD->sm_RP_DA.tag)
    {
        case V3_SM_RP_DA_imsi :
			dbgOctetString("V3_SM_RP_DA_imsi", (OctetString*)&pD->sm_RP_DA.u.imsi);
            break;
        case V3_SM_RP_DA_lmsi :
            break;
        case V3_SM_RP_DA_serviceCentreAddressDA :
			dbgAddressString("V3_SM_RP_DA_serviceCentreAddressDA"
                , (AddressString*)&pD->sm_RP_DA.u.serviceCentreAddressDA);
            break;
        case V3_SM_RP_DA_noSM_RP_DA :
            break;
        default :
            printf("DA fail [%d]\n", pD->sm_RP_DA.t);
    }

    switch(pD->sm_RP_OA.t)
    {
        case V3_SM_RP_OA_msisdn :
			dbgAddressString("V3_SM_RP_OA_msisdn"
                , (AddressString*)&pD->sm_RP_OA.u.msisdn);
            break;
        case V3_SM_RP_OA_serviceCentreAddressOA :
			dbgAddressString("V3_SM_RP_OA_serviceCentreAddressOA"
                , (AddressString*)&pD->sm_RP_OA.u.serviceCentreAddressOA);
            break;
        case V3_SM_RP_OA_noSM_RP_OA :
            break;
        default :
            printf("DA fail [%d]\n", pD->sm_RP_DA.t);
    }

	dbgOctetString("UI", (OctetString*)&pD->sm_RP_UI);
	if(pD->m.extensionContainerPresent) log_write(LEV_ALL_PARAM, "extensionContainerPresent\n");
	if(pD->m.imsiPresent) dbgOctetString("imsi", (OctetString*)&pD->imsi);

    log_write(LEV_ALL_PARAM, "========== V3_MO_ForwardSM_Arg  ........... end\n");

	*/
}


void
dbgV3MTFSMI()
{
#if 0
    log_write(LEV_ALL_PARAM, "========== V3_MT_ForwardSM_Res  ........... start\n");
    log_write(LEV_ALL_PARAM, "========== V3_MT_ForwardSM_Res  ........... end\n");
#endif
}

                                                                                      
void
printTcAddress(char *cpDesc, tc_address *spAddress)
{
#if 0
	log_write(LEV_ALL_PARAM, "%s : ", cpDesc);
	switch(spAddress->type)
	{
		case NO_TC_ADDRESS  :
			log_write(LEV_ALL_PARAM, "NO_TC_ADDRESS\n");
			break;
		case DEFAULT_LPC    :
			log_write(LEV_ALL_PARAM, "DEFAULT_LPC\n");
			break;
		case DPC    :
			log_write(LEV_ALL_PARAM, "DPC\n");
			log_write(LEV_ALL_PARAM, "PC : [H'%x][%d]\n", spAddress->pc_value,
					spAddress->pc_value);
			break;
		case DPC_SSN    :
			log_write(LEV_ALL_PARAM, "DPC_SSN\n");
			log_write(LEV_ALL_PARAM, "PC : [H'%x][%d] SSN : [H'%x][%d]\n",
					spAddress->pc_value, spAddress->pc_value,
					spAddress->ssn, spAddress->ssn);
			break;
		case GT  : // GT
			log_write(LEV_ALL_PARAM, "GT\n");
			log_write(LEV_ALL_PARAM, "GT Ind [%d] tt [%d] num [%d] "
					"encoding [%d] nature [%d]\n",
					spAddress->gt.gtIndicator, spAddress->gt.translation,
					spAddress->gt.numbering   , spAddress->gt.encoding,
					spAddress->gt.nature);
			log_write(LEV_ALL_PARAM, "[%s] \n", spAddress->gt.digit);
			break;
		case GT_SSN :
			log_write(LEV_ALL_PARAM, "GT_SSN\n"); 
			log_write(LEV_ALL_PARAM, "SSN : [H'%x][%d]\n", spAddress->ssn, spAddress->ssn);
			log_write(LEV_ALL_PARAM, "GT Ind [%d] tt [%d] num [%d]"
					" encoding [%d] nature [%d]\n",
					spAddress->gt.gtIndicator, spAddress->gt.translation,
					spAddress->gt.numbering, spAddress->gt.encoding,
					spAddress->gt.nature);
			log_write(LEV_ALL_PARAM, "[%s] \n", spAddress->gt.digit);
			break;
		case GT_DPC :
			log_write(LEV_ALL_PARAM, "GT_DPC\n");
			log_write(LEV_ALL_PARAM, "DPC : [H'%x][%d]\n",
					spAddress->pc_value, spAddress->pc_value);
			log_write(LEV_ALL_PARAM, "GT Ind [%d] tt [%d] num [%d] "
					"encoding [%d] nature [%d]\n", spAddress->gt.gtIndicator,
					spAddress->gt.translation, spAddress->gt.numbering,
					spAddress->gt.encoding, spAddress->gt.nature);
			log_write(LEV_ALL_PARAM, "[%s] \n", spAddress->gt.digit);
			break;
		case GT_DPC_SSN :
			log_write(LEV_ALL_PARAM, "GT_DPC_SSN\n");
			log_write(LEV_ALL_PARAM, "PC : [H'%x][%d] SSN : [H'%x][%d]\n",
					spAddress->pc_value, spAddress->pc_value,
					spAddress->ssn, spAddress->ssn);
			log_write(LEV_ALL_PARAM, "GT Ind [%d] tt [%d] num [%d]"
					" encoding [%d] nature [%d]\n",
					spAddress->gt.gtIndicator, spAddress->gt.translation,
					spAddress->gt.numbering, spAddress->gt.encoding,
					spAddress->gt.nature);
			log_write(LEV_ALL_PARAM, "[%s] \n", spAddress->gt.digit);
			break;
		case SSN    :
			log_write(LEV_ALL_PARAM, "SSN\n");
			log_write(LEV_ALL_PARAM, "SSN : [H'%x][%d]\n",
					spAddress->ssn, spAddress->ssn);
			break;
		default             :
			log_write(LEV_ALL_PARAM, "This is not address type [%x]\n", spAddress->type);
			break;
	}
	return;
#endif
}
void
printOC(unsigned char	cpData)
{
#if 0
	log_write ( LEV_ALL_PARAM, "OC [H'%02x] : ", cpData);
	switch(cpData)
	{
		/* CDMA & SKT Operation Code */ 
		case SMSREQ	:
			log_write ( LEV_ALL_PARAM, "SMSREQ\n");
			break;
		case SMDPP	:
			log_write ( LEV_ALL_PARAM, "SMDPP\n");
			break;
		case SMSNOT	:
			log_write ( LEV_ALL_PARAM, "SMSNOT\n");
			break;
		case NPREQ	:
			log_write ( LEV_ALL_PARAM, "NPREQ\n");
			break;

		case GWSMPP_DELIVER :
            log_write ( LEV_ALL_PARAM, "GWSMPP_DELIVER\n");
            break;
        case GWSMSNOTSMC :
            log_write ( LEV_ALL_PARAM, "GWSMSNOTSMC\n");
            break;
        case GWSMPP_SUBMIT :
            log_write ( LEV_ALL_PARAM, "GWSMPP_SUBMIT\n");
            break;
        case GWSMDPP :
            log_write ( LEV_ALL_PARAM, "GWSMDPP\n");
            break;
		/* WCDMA Operation Code */
        case REPORT_SM_DELIVERY_STATUS : // Phase1 SET_MESSAGE_WAITING_DATA
			log_write ( LEV_ALL_PARAM, "REPORT_SM_DELIVERY_STATUS\n");
			break;
		case SEND_ROUTING_INFO_FOR_SM :
			log_write ( LEV_ALL_PARAM, "SEND_ROUTING_INFO_FOR_SM\n");
			break;
		case MO_FORWARDSM : // Phase2, Phase1 FORWARD_SM
			log_write ( LEV_ALL_PARAM, "FORWARD_SM/(Phase2+ MO_FORWARD_SM)\n");
			break;
		case MT_FORWARDSM : // Phase2+ 
			log_write ( LEV_ALL_PARAM, "MT_FORWARDSM\n");
			break;
		case INFORM_SERVICE_CENTRE  :
			log_write ( LEV_ALL_PARAM, "INFORM_SERVICE_CENTRE\n");
			break;
		case ALERT_SERVICE_CENTRE :
			log_write ( LEV_ALL_PARAM, "ALERT_SERVICE_CENTRE\n");
			break;
		case ALERT_SERVICE_CENTRE_WITHOUT_RESULT :
			log_write ( LEV_ALL_PARAM, "ALERT_SERVICE_CENTRE_WITHOUT_RESULT\n");
			break;
		default 	:
			log_write ( LEV_ALL_PARAM, "I don't know this OC [H'%x]\n", cpData);
			break;
	}
	return;
#endif
}
void                                                                                  
printBegInvInd( tcx_primitive *spPrim, tcx_component *compPtr, tcx_buffer *buffPtr)
{     
#if 0
    int     nIdx;

    log_write(LEV_ALL_PARAM, "------------------------------------------------------------\n");
    log_write(LEV_ALL_PARAM, "------ Begin Invoke Indication -----------------------------\n");
    log_write(LEV_ALL_PARAM, "------------------------------------------------------------\n");
    log_write(LEV_ALL_PARAM, "Did [0x%02x][%u] Pid [%u] \n",spPrim->user_dialogue_id, 
			spPrim->user_dialogue_id, spPrim->provider_dialogue_id);

    printTcAddress("DEST", &spPrim->d_address);
    printTcAddress("ORIG", &spPrim->o_address);

    if(TC_P_DLG_INFO_PRESENT(spPrim) == TC_YES) 
	{ 
		log_writeHex(LEV_ALL_DEBUG, "Dialogue \n", TC_P_DLG_PORTION_APP_CONTEXT(spPrim).data, 
				TC_P_DLG_PORTION_APP_CONTEXT(spPrim).length, 10 );
	}

	log_write(LEV_ALL_PARAM, "Class [%d] Iid [%d] Lid [%d] Timer [%d] len [%d]\n", 
			compPtr->op_class,
			compPtr->invoke_id,
			compPtr->linked_id, 
			compPtr->timer.tv_sec, 
			buffPtr->actual_length);
    printOC(compPtr->operation.datas[0]);
    log_writeHex(LEV_ALL_DEBUG, "BeginInvoke\n", (char *)buffPtr->bufferp, buffPtr->actual_length,10);
    log_write(LEV_ALL_PARAM, "------------------------------------------------------------\n");
    return;
#endif
}

void dbgV3AddrString(char *msg, V3_AddressString *Addr)
{
#if 0
	log_write ( LEV_ALL_PARAM, "%s : numocts[%d] TON[%d] NPI [%d]\n", msg,
			Addr->numocts, Addr->m.NAI, Addr->m.NP );
	log_writeHex( LEV_ALL_PARAM, "Address : H'", (char *)Addr->data, Addr->numocts, 0);
	return;
#endif
}
void dbgV3MTFSMR(V3_MT_ForwardSM_Arg *pD)
{
#if 0
    log_write ( LEV_ALL_PARAM, "========== V3_MT_ForwardSM_Arg  ........... start\n");
    switch(pD->sm_RP_DA.t)
    {
        case V3_SM_RP_DA_imsi :
            log_writeHex(LEV_ALL_PARAM, "V3_SM_RP_DA_imsi", 
					(char *)pD->sm_RP_DA.u.imsi.data, pD->sm_RP_DA.u.imsi.numocts, 0);
        case V3_SM_RP_DA_lmsi :
            break;
        case V3_SM_RP_DA_serviceCentreAddressDA :
			dbgV3AddrString( "V3_SM_RP_DA_serviceCentreAddressDA", &pD->sm_RP_DA.u.serviceCentreAddressDA );
            break;
        default :
            log_write ( LEV_ALL_PARAM, "DA fail [%d]\n", pD->sm_RP_DA.t);
    }

    switch(pD->sm_RP_OA.t)
    {
        case V3_SM_RP_OA_msisdn :
			dbgV3AddrString( "V3_SM_RP_OA_msisdn", &pD->sm_RP_OA.u.msisdn );
            break;
        case V3_SM_RP_OA_serviceCentreAddressOA :

            dbgV3AddrString("V3_SM_RP_OA_serviceCentreAddressOA", &pD->sm_RP_OA.u.serviceCentreAddressOA);
            break;
        default :
            log_write ( LEV_ALL_PARAM, "DA fail [%d]\n", pD->sm_RP_DA.t);
    }

    dbgOctetString("UI", (OctetString*)&pD->sm_RP_UI);
    if(pD->m.moreMessageToSendPresent)
        log_write ( LEV_ALL_PARAM, "moreMessageToSendPresent\n");
    if(pD->m.extensionContainerPresent)
        log_write ( LEV_ALL_PARAM, "extensionContainerPresent\n");
    log_write ( LEV_ALL_PARAM, "========== V3_MT_ForwardSM_Arg  ........... end\n");
#endif
}

void dbgSMDPP(stSMDPP_t *pD)
{
#if 0
	log_write(LEV_ALL_PARAM,"========== SMDPP ........... start\n");
	log_writeHex(LEV_ALL_PARAM, "smsbd \n", (char *)pD->smsbd.data, pD->smsbd.numocts, 10);
	log_writeHex(LEV_ALL_PARAM, "smsti : "	, (char *)pD->smsti.data, pD->smsti.numocts, 0);
	if(pD->m.esnPresent) 	log_writeHex(LEV_ALL_PARAM, "esn : ", (char *)pD->esn.data, pD->esn.numocts, 0);
	if(pD->m.minPresent) 	log_writeHex(LEV_ALL_PARAM, "min : ", (char *)pD->min.data, pD->min.numocts, 0);
    if(pD->m.smsniPresent) 	log_writeHex(LEV_ALL_PARAM, "smsni : ", (char *)pD->smsni.data, pD->smsni.numocts, 0);
	if(pD->m.smsooaPresent) log_writeHex(LEV_ALL_PARAM, "smsooa : ", (char *)pD->smsooa.data, pD->smsooa.numocts, 0);
	if(pD->m.smsdaPresent) 	log_writeHex(LEV_ALL_PARAM, "smsda : "	, (char *)pD->smsda.data, pD->smsda.numocts, 0);
	if(pD->m.smscindPresent) log_writeHex(LEV_ALL_PARAM, "smscind : "   , (char *)pD->smscind.data, pD->smscind.numocts, 0);
	if(pD->m.osfiPresent) log_writeHex(LEV_ALL_PARAM, "osfi : ", (char *)pD->osfi.data, pD->osfi.numocts, 0);
	if(pD->m.roamindicatPresent) log_writeHex(LEV_ALL_PARAM, "roamindicater : ", (char *)pD->smsroamindicat.data, pD->smsroamindicat.numocts, 0);
	if(pD->m.roampmnPresent) log_writeHex(LEV_ALL_PARAM, "smsroampmn : ", (char *)pD->smsroampmn.data, pD->smsroampmn.numocts, 0);
    log_write(LEV_ALL_PARAM, "========== SMDPP ............. end\n");
#endif
}

#if 0
int user_data(cptr, spActive, RetErr, step)
stDec2304_t		*cptr;
stRDActiveTuple *spActive;
stMapReterr		*RetErr;
int				step;
{
	int             result, i, itmp;
    unsigned char   tmp, num_field, reserved;
    unsigned char   ucMsgEncoding;

	tmp = strbitcp((void *)&(cptr->ucContents[0]), step, 5);
	step = step + 5;
    ucMsgEncoding = tmp;

//Add 20050418... for datacoding encoding...	
	spActive->dbDataEncoding[0] = tmp;

	//
	//	Do not use DCS field when receiving msg from CDMA Network
	//	Use only 2 Parmeters
	//	1. MsgEncoding (dbDataEncoding)
	//	2. LanguageIndicator (dbLanguageFlag)
	//	2007.01.07 by bjlee	
	//
	//spActive->dbDataCodingScheme[0] = tmp;

    switch(tmp)
    {
        case 0x01 :
            DebugStr(2,"%25s = [%02x] (IS-91 Extended protocol)\n", "Message Encoding", tmp);
            return -1;

        case 0x10 :
            DebugStr(2,"%25s = [%02x] (KSC5601i : 8-bit)\n", "Message Encoding", tmp);
            break;

        case 0x00 :
            DebugStr(2,"%25s = [%02x] (Octet : 8-bit)\n", "Message Encoding", tmp);
            break;

        case 0x02 :
            DebugStr(2,"%25s = [%02x] (7-bit ASCII)\n", "Message Encoding", tmp);
            break;

        case 0x03 :
            DebugStr(2,"%25s = [%02x] (IA5 CCITT T.50 : 7-bit)\n", "Message Encoding", tmp);
            break;
		
		default:
			DebugStr(2,"%25s = [%02x] (unknown) \n","Message Encoding", tmp);
			break;
    }

	switch(tmp)
	{
		case 0x10 :
		case 0x00 :
			num_field = strbitcp((void *)&(cptr->ucContents[0]), step, 8);
			step = step + 8;
    		DebugStr(2,"%25s = [%02x]\n", "Number Field", num_field);
			DATA_NUM1 = num_field;
// for Message max length check
// if over max length, copy to max length... 20050616

 			if((stMapParam.nMaxMsgLen > MAX_MESSAGE_LEN)
				|| (stMapParam.nMaxMsgLen < DATA_NUM1))
			{
				DATA_NUM1 = stMapParam.nMaxMsgLen;
			}

			for (i = 0; i < (int) num_field; i++)
			{
				tmp = strbitcp((void *)&(cptr->ucContents[0]), step, 8);
				step = step + 8;
				spActive->dbMessageContent[i] = tmp;
        		DebugStr(2,"%25s [%d] = [%02x]\n", "User Data", i, tmp);
			}
			spActive->dbMessageContent[DATA_NUM1] = NULL;

			reserved = strbitcp((void *)&(cptr->ucContents[0]), step, 3);
			step = step + 3;
			break;
		case 0x02 :
		case 0x03 :
	    	num_field = strbitcp((void *)&(cptr->ucContents[0]), step, 8);
        	step = step + 8;
        	DebugStr(2,"%25s = [%02x]\n", "Number Field", num_field);
        	DATA_NUM1 = num_field;
// for Message max length check
// if over max length, copy to max length... 20050616

 			if((stMapParam.nMaxMsgLen > MAX_MESSAGE_LEN)
				|| (stMapParam.nMaxMsgLen < DATA_NUM1))
			{
				DATA_NUM1 = stMapParam.nMaxMsgLen;
			}

        	for (i = 0; i < (int) num_field; i++)
        	{
            	tmp = strbitcp((void *)&(cptr->ucContents[0]), step, 7);
            	step = step + 7;
				spActive->dbMessageContent[i] = tmp;
            	DebugStr(2,"%25s [%d] = [%02x]\n", "User Data", i, tmp);
        	}
			spActive->dbMessageContent[DATA_NUM1] = NULL;

        	if ((step % 8) == 0) itmp = 0;
        	else itmp = 8 - (step % 8);

        	reserved = strbitcp((void *)&(cptr->ucContents[0]), step, itmp);
        	step = step + itmp;
			break;
		default:
			DebugStr(0,"%25s = [%02x] (unknown) \n","Message Encoding", tmp);
			break;
    }
/**
	DebugStr(4, "11User data ");
    for (i = 0; i < (int) num_field; i++)
    {
        DebugStr(4, "[%02x]", spActive->dbMessageContent[i]);
    }
    DebugStr(4,"\n");
***/

	return step;
}
#endif
