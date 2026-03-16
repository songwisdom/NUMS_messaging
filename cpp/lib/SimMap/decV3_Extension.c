/**
 @ingroup v3_gsm_map 
 @{
 */

/*******************************************************************************
*                                                                           
*     Copyright (c) 2001 FEELINGK Co., Ltd.                                
*     All rights reserved.                                                
*                                                                          
*     This program contains confidential and proprietary information of    
*     FEELINGK, and any reproduction, disclosure, or use in whole or in part 
*     by prior written agreement or permission of FEELINGK.                 
*                                                                          
*                            FEELINGK Co., Ltd.                           
*         5F Roy B/D 90-6 Nonhyun-dong Kangnam-Gu, Seoul, 135-010 Korea
*                                                                          
#******************************************************************************/

/*******************************************************************************
    FILE NAME       :  
    PURPOSE         :  
    DATE OF WRITE   :   2001.12.01
    AUTHOR          :   Suk-Hun Jang
    REVISION        :   V 1.0
    Ver.    By      When        Reason
    ------- ------- ----------  -----------------------------------------
    1.0     shjang  2001.12.01  first coding
*******************************************************************************/
#include "stdio.h"
#include "string.h"
#include "libMapV3Extension.h"
#include "libMapUtil.h"
#include "libMapAddrUtil.h"

#define DB_DEST_MS_CAPABILITY_SIZE          8
#define	DB_OSFI_SIZE						8

extern V3_ObjectIdentifier EXTENSION_OID_TABLE[EXTENSION_OID_TABLE_COUNT];
// = {
// { 6, { 0, 3, 4504, 0, 0, 2 } },     /* OID_SKT_MIN */
// { 6, { 0, 3, 4504, 0, 0, 4 } },     /* OID_SKT_SECONDARY_MSISDN */
// { 6, { 0, 3, 4504, 0, 0, 6 } },     /* OID_SKT_OSFI */
// { 6, { 0, 3, 4504, 0, 0, 8 } },     /* OID_SKT_ESN */
// { 6, { 0, 3, 4504, 0, 0, 11 } },    /* OID_SKT_CAPABILITY_ID */
// { 6, { 0, 3, 4504, 0, 0, 28 } },    /* OID_SKT_ROAMING_INDICATOR */
// { 6, { 0, 3, 4504, 0, 0, 36 } },	   /* OID_SKT_AccessDeniedReason */
// { 6, { 0, 3, 4504, 0, 0, 39 } },    /* OID_SKT_ROAMING_PMN */
// { 6, { 0, 3, 4504, 0, 0, 62 } }     /* SMS_MO_SAC */ //FR401 LSH
//};
// 14380 ==> F0 2C
// 4504  ==> 11 98

int 
decV3_OpenType(stMsgBuf_t *spMsg, V3_OpenType *pD, u_char cTag)
{
    int     nStep = 0;
    int     nLen;
	u_char		cOut[OPEN_TYPE_LEN];

	if(cTag != spMsg->cpData[nStep++])
    {
        log_write(LEV_ALL_ERR, "OpenType tag fail [H'%x]\n", spMsg->cpData[0]);
        return(-1);
    }
    if(decLength(&nLen, &spMsg->cpData[nStep], &nStep) < 0)
        return(-1);

	memset(cOut, 0x00, OPEN_TYPE_LEN);
	memcpy(cOut, &spMsg->cpData[nStep], nLen);

	pD->numocts = nLen;
	memcpy(pD->data, cOut, pD->numocts);
	
	nStep = nStep + pD->numocts;
	
    return(nStep);
}
int
decV3_PrivateExtension(stMsgBuf_t *spMsg, V3_PrivateExtension *pD, u_char cTag)
{
	stMsgBuf_t	stMsg;
	int			nStep 	= 0;
	int			nLen	= 0;
	int			nlSize	= 0;
	int			nSize	= 0;

	if(cTag != spMsg->cpData[nStep++])
    {
        log_write(LEV_ALL_ERR, "decV3_PrivateExtension tag fail [%x]\n", spMsg->cpData[0]);
        return(-1);
    }
    if(decLength(&nLen, &spMsg->cpData[nStep], &nStep) < 0)
        return(-1);
	nlSize = spMsg->nLen - nStep;
	
	/* Extension Identifier */
	memset(&stMsg, 0x00, sizeof(stMsgBuf_t));
	stMsg.nLen = nlSize;

	memcpy(stMsg.cpData, &spMsg->cpData[nStep], stMsg.nLen);
	nSize = decV3_ObjectId(&stMsg, &pD->extId, 0x06);
	if(nSize < 0) return(-1);
	nlSize      = nlSize - nSize;
    nStep       = nStep + nSize;

	/* Extension Type */
	memset(&stMsg, 0x00, sizeof(stMsgBuf_t));
	stMsg.nLen = nlSize;

	memcpy(stMsg.cpData, &spMsg->cpData[nStep], stMsg.nLen);

	if(stMsg.cpData[0] == 0x04)
	{
		pD->m.extTypePresent = 1;
		nSize = decV3_OpenType(&stMsg, &pD->extType, 0x04);
		if(nSize < 0) return(-1);
		nlSize      = nlSize - nSize;
    	nStep       = nStep + nSize;
	}

	/* nStep = nStep + nLen */

	return(nStep);
}

int 
decV3_PrivateExtensionList(stMsgBuf_t *spMsg, V3_PrivateExtensionList *pD, u_char cTag)
{
	stMsgBuf_t	stMsg;
	int			nStep 	= 0;
	int			nLen	= 0;
	int			nlSize	= 0;
	int			nSize	= 0;

	int         nIndex  = 0;
    int         nOidCnt = 0;
    int         nCount  = 0;
    int         nLoop   = 0;


	if(cTag != spMsg->cpData[nStep++])
    {
        log_write(LEV_ALL_ERR, "decV3_PrivateExtensionList tag fail [%x]\n", spMsg->cpData[0]);
        return(-1);
    }
    if(decLength(&nLen, &spMsg->cpData[nStep], &nStep) < 0)
        return(-1);
	nlSize = spMsg->nLen - nStep;
	
	while( nCount++ < EXTENSION_OID_TABLE_COUNT )
	{
		//
		//	Extension container is an optional data 
		//
		if(spMsg->cpData[nStep] != 0x30)
		{
//			DebugStr(4, "decPrivateExtension tag fail [%x]\n", spMsg->cpData[nStep]);
			pD->n++;
			break;
		}

		memset(&stMsg, 0x00, sizeof(stMsgBuf_t));
        memcpy(stMsg.cpData, &spMsg->cpData[nStep], nlSize);
        stMsg.nLen = nlSize ;

        nSize = decV3_PrivateExtension(&stMsg,&pD->privateExtension,0x30);
        if(nSize < 0) return(-1);
        nStep       = nStep + nSize;
        nlSize      = nlSize - nSize;

		if(pD->privateExtension.m.extTypePresent)
        {
            for( nOidCnt=0; nOidCnt < EXTENSION_OID_TABLE_COUNT; nOidCnt++)
            {
                if(pD->privateExtension.extId.numids != EXTENSION_OID_TABLE[nOidCnt].numids)
                    continue;

                for(nIndex=0;nIndex < pD->privateExtension.extId.numids; nIndex++)
                {
                    if(pD->privateExtension.extId.subid[nIndex] !=
                        EXTENSION_OID_TABLE[nOidCnt].subid[nIndex])
                        break;

                    if(nIndex == pD->privateExtension.extId.numids-1)
                    {
                        switch(nOidCnt)
                        {
                            case OID_SKT_MIN:
                                pD->m.minPresent = 1;
                                pD->min.numocts = pD->privateExtension.extType.numocts;

								for(nLoop=0; nLoop<pD->privateExtension.extType.numocts; nLoop++)
								{
									pD->min.data[nLoop] =
										(pD->privateExtension.extType.data[nLoop] >> 4) |
										(pD->privateExtension.extType.data[nLoop] << 4) ;
								}

                                pD->n++;
                                break;

                            case OID_SKT_ESN:
                                pD->m.esnPresent = 1;
                                pD->esn.numocts = pD->privateExtension.extType.numocts;

								memcpy(pD->esn.data, pD->privateExtension.extType.data,
													 pD->privateExtension.extType.numocts);
                                pD->n++;
                                break;

                            case OID_SKT_CAPA:
                                pD->m.capaPresent = 1;
                                pD->capa.numocts = pD->privateExtension.extType.numocts;

                                // Max 4 Byte Variable
                                if( pD->capa.numocts > DB_DEST_MS_CAPABILITY_SIZE )
                                {
                                    log_write(LEV_ALL_ERR, "[%s:%d] fail to decode OID_SKT_CAPA\n", __func__, __LINE__);
                                    pD->n++;
                                    break;
                                }

                                for(nLoop=0; nLoop<pD->privateExtension.extType.numocts; nLoop++)
                                {
									pD->capa.data[nLoop] = pD->privateExtension.extType.data[nLoop];
                                }

                                pD->n++;
                                break;

                            case OID_SKT_OSFI:
                                pD->m.osfiPresent = 1;
                                pD->osfi.numocts = pD->privateExtension.extType.numocts;

                                // Max 5 Byte Variable (3..5 reserved)
                                if( pD->osfi.numocts > DB_OSFI_SIZE + 1 )
                                {
                                    log_write(LEV_ALL_ERR, "[%s:%d] fail to decode OID_SKT_OSFI\n", __func__, __LINE__);
                                    pD->n++;
                                    break;
                                }

                                for(nLoop=0; nLoop<pD->privateExtension.extType.numocts; nLoop++)
                                {
									pD->osfi.data[nLoop] = pD->privateExtension.extType.data[nLoop];
                                }
                                pD->n++;
                                break;

                            case OID_SKT_ROAMING_INDICATOR:
                                pD->m.roamPresent = 1;
                                pD->roam.numocts = pD->privateExtension.extType.numocts;

								pD->roam.data[0] = pD->privateExtension.extType.data[0];
                                pD->n++;
                                break;

                            case OID_SKT_SECONDARY_MSISDN:
                                pD->m.msisdnPresent = 1;
                                pD->msisdn.numocts = pD->privateExtension.extType.numocts;
 
                                for(nLoop=0; nLoop<pD->privateExtension.extType.numocts; nLoop++)
                                {
                                    pD->msisdn.data[nLoop] =
                                        (pD->privateExtension.extType.data[nLoop] >> 4) |
                                        (pD->privateExtension.extType.data[nLoop] << 4) ;
                                }
                                pD->n++;
                                break;
							case OID_SKT_NPFOWARDINFORMATION_ID :
                                pD->m.npfiPresent = 1;
                                pD->npfi.numocts = pD->privateExtension.extType.numocts;
 
                                for(nLoop=0; nLoop<pD->privateExtension.extType.numocts; nLoop++)
                                {
                                    pD->npfi.data[nLoop] =
                                        (pD->privateExtension.extType.data[nLoop] >> 4) |
                                        (pD->privateExtension.extType.data[nLoop] << 4) ;
                                }
                                pD->n++;
                                break;


                            case OID_SKT_ACCESSDENIEDREASON:
                                pD->m.adrPresent = 1;
                                pD->adr.numocts = pD->privateExtension.extType.numocts;

                                pD->adr.data[0] = pD->privateExtension.extType.data[0];
                                pD->n++;
                                break;

							case OID_SKT_ROAMING_PMN:
								pD->m.roampmnPresent = 1;
								pD->roampmn.numocts = pD->privateExtension.extType.numocts;

                                for(nLoop=0; nLoop<pD->privateExtension.extType.numocts; nLoop++)
                                {
                                    pD->roampmn.data[nLoop] =
                                        (pD->privateExtension.extType.data[nLoop] >> 4) |
                                        (pD->privateExtension.extType.data[nLoop] << 4) ;
                                }
                                pD->n++;
                                break;
								/*
							case OID_SKT_SAC: //FR401 LSH 
								pD->m.sacPresent = 1;
								pD->sac.numocts = pD->privateExtension.extType.numocts;
								pD->n++;

								for(nLoop=0; nLoop<pD->privateExtension.extType.numocts; nLoop++)
								{
									pD->sac.data[nLoop] =
										(pD->privateExtension.extType.data[nLoop] >> 4) |
										(pD->privateExtension.extType.data[nLoop] << 4) ;	
								}
								pD->n++;
								break;
								*/
							//FR220 010_PLUS_SVC
                            case OID_SKT_SUBSCRIPTIONINFO:
								pD->m.subscriptioninfoPresent = 1;
								pD->subscriptioninfo.numocts = pD->privateExtension.extType.numocts;

								pD->n++;    // length of digit (in value)

								for(nLoop=0; nLoop<pD->privateExtension.extType.numocts; nLoop++)
								{
									pD->subscriptioninfo.data[nLoop] =
										(pD->privateExtension.extType.data[nLoop] >> 4) |
										(pD->privateExtension.extType.data[nLoop] << 4) ;
								}
								pD->n++;
								break;
								//FR401 SMSMO_SAC DATA LSH
							/*case OID_SKT_SMSMO_SAC:
								pD->m.smsmosacPresent= 1;
								pD->smsmosac.numocts = pD->privateExtension.extType.numocts;
								for(nLoop=0; nLoop<pD->privateExtension.extType.numocts; nLoop++)
								{
									pD->smsmosac.data[nLoop] = pD->privateExtension.extType.data[nLoop];
								}

								pD->n++;
								break;*/
							case OID_SKT_IPMG_REFERENCE_ID :
								pD->m.ipmg_refidPresent = 1;
								pD->ipmg_refId.numocts = pD->privateExtension.extType.numocts;

								/*
								for(nLoop=0; nLoop<pD->privateExtension.extType.numocts; nLoop++)
								{
									pD->ipmg_refId.data[nLoop] =
										(pD->privateExtension.extType.data[nLoop] >> 4) |
										(pD->privateExtension.extType.data[nLoop] << 4) ;
								}
								*/
								pD->n++;
								break;
							case OID_SKT_IPMG_MSISDN :
								pD->m.ipmg_msisdnPresent = 1;
								pD->ipmg_msisdn.numocts = pD->privateExtension.extType.numocts;

								/*
								for(nLoop=0; nLoop<pD->privateExtension.extType.numocts; nLoop++)
								{
									pD->ipmg_msisdn.data[nLoop] =
										(pD->privateExtension.extType.data[nLoop] >> 4) |
										(pD->privateExtension.extType.data[nLoop] << 4) ;
								}
								*/
								pD->n++;
								break;
							case OID_SKT_SMSLTEMTINFO :
								pD->m.ltesmsPresent = 1;
								pD->ltesms.numocts = pD->privateExtension.extType.numocts;

								pD->n++;    // length of digit (in value)

								for(nLoop=0; nLoop<pD->privateExtension.extType.numocts; nLoop++)
								{
									pD->ltesms.data[nLoop] =
										(pD->privateExtension.extType.data[nLoop] >> 4) |
										(pD->privateExtension.extType.data[nLoop] << 4) ;
								}
								pD->n++;
								break;

                        }
                    }
                }
            }
        }
    }


	/* nStep = nStep + nLen; */

	return(nStep);
}
int decV3_ExtensionContainer(stMsgBuf_t *spMsg, V3_ExtensionContainer *pD, u_char cTag)
{
	stMsgBuf_t	stMsg;
	int			nStep 	= 0;
	int			nLen	= 0;
	int			nlSize	= 0;
	int			nSize	= 0;

	if(cTag != spMsg->cpData[nStep++])
    {
        log_write(LEV_ALL_ERR, "decV3_ExtensionContainer tag fail [%x]\n", spMsg->cpData[0]);
        return(-1);
    }
    if(decLength(&nLen, &spMsg->cpData[nStep], &nStep) < 0)
        return(-1);
	nlSize = spMsg->nLen - nStep;

	/* Private Extension List */
	memset(&stMsg, 0x00, sizeof(stMsgBuf_t));
	stMsg.nLen = nlSize;
	memcpy(stMsg.cpData, &spMsg->cpData[nStep], stMsg.nLen);
	if(stMsg.cpData[0] == 0xA0)
	{
		pD->m.privateExtensionListPresent = 1;
		nSize=decV3_PrivateExtensionList(&stMsg,&pD->privateExtensionList,0xA0);
		if(nSize < 0) return(-1);
		nlSize      = nlSize - nSize;
        nStep       = nStep + nSize;
	}
	/* nStep = nStep + nLen */

	return(nStep);
}
/** @} */
