/**
 * =====================================================================================
 *
 *       @file  encV3_Extension.c
 *
 *    	 @brief 
 *
 *       Version:  1.0
 *       Compiler:  cc (HP-UX)
 *       @date:  04/23/09 17:14:51
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
#include "stdio.h"
#include "string.h"
#include "libMapV3Extension.h"
#include "libMapUtil.h"

#if 0
V3_ObjectIdentifier EXTENSION_OID_TABLE[EXTENSION_OID_TABLE_COUNT] = {
	{ 6, { 0, 3, 4504, 0, 0, 2 } },     /* OID_SKT_MIN */
    { 6, { 0, 3, 4504, 0, 0, 4 } },     /* OID_SKT_SECONDARY_MSISDN */
    { 6, { 0, 3, 4504, 0, 0, 6 } },     /* OID_SKT_OSFI */
	{ 6, { 0, 3, 4504, 0, 0, 8 } },     /* OID_SKT_ESN */
    { 6, { 0, 3, 4504, 0, 0, 11 } },    /* OID_SKT_CAPABILITY_ID */
    { 6, { 0, 3, 4504, 0, 0, 28 } },    /* OID_SKT_ROAMING_INDICATOR */
    { 6, { 0, 3, 4504, 0, 0, 36 } },	/* OID_SKT_AccessDeniedReason */
	{ 6, { 0, 3, 4504, 0, 0, 39 } },	/* OID_SKT_ROAMING_PMN */
    { 6, { 0, 3, 4504, 0, 0, 54} },     /* OID_SKT_SUBSCRIPTIONINFO*/ //FR220 010_PLUS_SVC
	{ 6, { 0, 3, 4504, 0, 0, 64 } }, 	/* OID_SKT_IPMG_ReferenceId FR410 */
	{ 6, { 0, 3, 4504, 0, 0, 65 } } 	/* OID_SKT_IPMG_MSISDN FR410 */
};
#endif
V3_ObjectIdentifier EXTENSION_OID_TABLE[EXTENSION_OID_TABLE_COUNT] = {
    { 6, { 0, 3, 4504, 0, 0, 2 } },     /* OID_SKT_MIN */
    { 6, { 0, 3, 4504, 0, 0, 4 } },     /* OID_SKT_SECONDARY_MSISDN */
    { 6, { 0, 3, 4504, 0, 0, 6 } },     /* OID_SKT_OSFI */
    { 6, { 0, 3, 4504, 0, 0, 8 } },     /* OID_SKT_ESN */
    { 6, { 0, 3, 4504, 0, 0, 11 } },    /* OID_SKT_CAPABILITY_ID */
    { 6, { 0, 3, 4504, 0, 0, 28 } },    /* OID_SKT_ROAMING_INDICATOR */
    { 6, { 0, 3, 4504, 0, 0, 36 } },    /* OID_SKT_AccessDeniedReason */
    { 6, { 0, 3, 4504, 0, 0, 39 } },    /* OID_SKT_ROAMING_PMN */
    { 6, { 0, 3, 4504, 0, 0, 54} },     /* OID_SKT_SUBSCRIPTIONINFO*/ //FR220 010_PLUS_SVC
    { 6, { 0, 3, 4504, 0, 0, 35 } },    /* OID_SKT_NPFowardInformation_ID */ //DR610 LSH 
    { 6, { 0, 3, 4504, 0, 0, 73 } },    /* OID_SKT_LTE_SMSMTInfo DR610 */ //DR610 LSH
    { 6, { 0, 3, 4504, 0, 0, 74 } },    /* OID_SKT_IPMG_ReferenceId FR410 */
    { 6, { 0, 3, 4504, 0, 0, 75 } },     /* OID_SKT_IPMG_MSISDN FR410 */
	{ 6, { 0, 3, 4504, 0, 0, 80 } },     /* OID_SKT_SMS_OSFI FR720 */
	{ 6, { 0, 3, 4504, 0, 0, 83 } }     /* OID_SKT_SMSF_NUMBER_ID SR610 */
//  { 6, { 0, 3, 4504, 0, 0, 62 } }     /* OID_SKT_SMSMO_SAC*/ //FR401 MO_SAC_DATA LSH 
};
int encV3_PrivateExtension(stMsgBuf_t *spMsg, V3_PrivateExtension *pD, u_char cTag)
{
	stMsgBuf_t  stMsg[2];   /* total count parameter */
	int nIdx = 0;

	memset(&stMsg, 0x00, sizeof(stMsgBuf_t)*2);

	/* Extension Identifier */
	if(encV3_ObjectId(&stMsg[nIdx++], &pD->extId, 0x06) < 0) 
	{
		return(-1);
	}

	/* Extension Type */
	if(pD->m.extTypePresent)
	{
		if(encV3_OpenType(&stMsg[nIdx++], &pD->extType, 0x04) < 0)
			return(-1);
	}

	if(encTotal(spMsg, stMsg, nIdx, cTag) < 0)
	{
		//LogRet("[%s:%d] encV3_Extension encV3_Total fail. \n", __func__, __LINE__);
		return(-1);
	} 


	return(1);
}

int encV3_ExtensionData(stMsgBuf_t *spMsg, int nOidInedex, V3_OpenType *spData, bool convFlag)
{
	int nLoop = 0;
	V3_PrivateExtension stExt;

	if ( nOidInedex < 0 || nOidInedex >= EXTENSION_OID_TABLE_COUNT ) {
		return -1;
	}

	memset(&stExt, 0x00, sizeof(stExt));

	/* Extension Identifier */
	stExt.extId.numids = EXTENSION_OID_TABLE[nOidInedex].numids;		

	for(nLoop=0; nLoop < stExt.extId.numids; nLoop++)
	{
		stExt.extId.subid[nLoop] = EXTENSION_OID_TABLE[nOidInedex].subid[nLoop];
	}

	/* Extension Type (optional) */
	stExt.m.extTypePresent = 1;
	stExt.extType.numocts = spData->numocts;

	memcpy(stExt.extType.data, spData->data, spData->numocts);

	if ( convFlag == true ) {
		for(nLoop=0; nLoop< stExt.extType.numocts; nLoop++)
		{
			stExt.extType.data[nLoop] = (stExt.extType.data[nLoop] >> 4) | (stExt.extType.data[nLoop] << 4);
		}
	}

	if(encV3_PrivateExtension(spMsg, &stExt, 0x30) < 0)
	{
		return(-1);
	}
	return 0;
}
int encV3_PrivateExtensionList(stMsgBuf_t *spMsg, V3_PrivateExtensionList *pD, u_char cTag)
{
	int nIdx = 0;
	int nLoop = 0;
	stMsgBuf_t	stMsg[6];

	memset(stMsg, 0x00, sizeof(stMsgBuf_t)*6);

	/* MIN */
	if(pD->m.minPresent)
	{
		//DebugStr(4, "[%s:%d] minPresent\n", __func__, __LINE__);

		/* Extension Identifier */
		pD->privateExtension.extId.numids = EXTENSION_OID_TABLE[0].numids;		

		for(nLoop=0; nLoop < pD->privateExtension.extId.numids; nLoop++)
		{
			pD->privateExtension.extId.subid[nLoop] = EXTENSION_OID_TABLE[0].subid[nLoop];
		}

		/* Extension Type (optional) */
		pD->privateExtension.m.extTypePresent = 1;
		pD->privateExtension.extType.numocts = pD->min.numocts;

		memcpy(pD->privateExtension.extType.data, pD->min.data, pD->min.numocts);

		log_writeHex(LEV_ALL_PARAM, "MIN : ", (char *)pD->min.data, (int)pD->min.numocts, 0);

		if(encV3_PrivateExtension(&stMsg[nIdx++], &pD->privateExtension, 0x30) < 0)
		{
			return(-1);
		}
	}

	/* MSIDN */
	if(pD->m.msisdnPresent)
	{
		//DebugStr(4, "[%s:%d] msisdnPresent\n", __func__, __LINE__);

		/* Extension Identifier */
		pD->privateExtension.extId.numids = EXTENSION_OID_TABLE[1].numids;		

		for(nLoop=0; nLoop < pD->privateExtension.extId.numids; nLoop++)
		{
			pD->privateExtension.extId.subid[nLoop] = EXTENSION_OID_TABLE[1].subid[nLoop];
		}

		/* Extension Type (optional) */
		pD->privateExtension.m.extTypePresent = 1;
		pD->privateExtension.extType.numocts = pD->msisdn.numocts;

		memcpy(pD->privateExtension.extType.data, pD->msisdn.data, pD->msisdn.numocts);

		log_writeHex(LEV_ALL_PARAM, "FOWARD MSISDN : ", (char *)pD->msisdn.data, (int)pD->msisdn.numocts, 0);

		if(encV3_PrivateExtension(&stMsg[nIdx++], &pD->privateExtension, 0x30) < 0)
		{
			return(-1);
		}
	}

	/* SMSF MSIDN */
	if(pD->m.smsf_msisdnPresent)
	{
		//DebugStr(4, "[%s:%d] smsf_msisdnPresent\n", __func__, __LINE__);

		/* Extension Identifier */
		pD->privateExtension.extId.numids = EXTENSION_OID_TABLE[14].numids;		

		for(nLoop=0; nLoop < pD->privateExtension.extId.numids; nLoop++)
		{
			pD->privateExtension.extId.subid[nLoop] = EXTENSION_OID_TABLE[14].subid[nLoop];
		}

		/* Extension Type (optional) */
		pD->privateExtension.m.extTypePresent = 1;
		pD->privateExtension.extType.numocts = pD->smsf_msisdn.numocts;

		memcpy(pD->privateExtension.extType.data, pD->smsf_msisdn.data, pD->smsf_msisdn.numocts);

		log_writeHex(LEV_ALL_PARAM, "SMSF Number ID : ", (char *)pD->smsf_msisdn.data, (int)pD->smsf_msisdn.numocts, 0);

		if(encV3_PrivateExtension(&stMsg[nIdx++], &pD->privateExtension, 0x30) < 0)
		{
			return(-1);
		}
	}

	/* OSFI */
	if(pD->m.osfiPresent)
	{
		/* Extension Identifier */                                                     
        pD->privateExtension.extId.numids = EXTENSION_OID_TABLE[2].numids;             
                                                                                       
        for(nLoop=0; nLoop < pD->privateExtension.extId.numids; nLoop++)               
        {                                                                              
            pD->privateExtension.extId.subid[nLoop] = EXTENSION_OID_TABLE[2].subid[nLoop];
        }                                                                              
        
        /* Extension Type (optional) */                                                
        pD->privateExtension.m.extTypePresent = 1;                                     
        pD->privateExtension.extType.numocts = pD->osfi.numocts;                       
		
		for(nLoop=0; nLoop< pD->privateExtension.extType.numocts; nLoop++)
		{
			pD->privateExtension.extType.data[nLoop] = pD->osfi.data[nLoop];
		}

        if(encV3_PrivateExtension(&stMsg[nIdx++], &pD->privateExtension, 0x30) < 0)    
        {
            return(-1);                                                                
        }
	}

	/* SMS_OSFI */
    if(pD->m.smsosfiPresent)
    {
		/* Extension Identifier */
        pD->privateExtension.extId.numids = EXTENSION_OID_TABLE[13].numids;     

        for(nLoop=0; nLoop < pD->privateExtension.extId.numids; nLoop++)
        {
            pD->privateExtension.extId.subid[nLoop] = EXTENSION_OID_TABLE[13].subid[nLoop];
        }

        /* Extension Type (optional) */
        pD->privateExtension.m.extTypePresent = 1;
        pD->privateExtension.extType.numocts = pD->smsosfi.numocts;
        memcpy(pD->privateExtension.extType.data, pD->smsosfi.data, pD->smsosfi.numocts);

        for(nLoop=0; nLoop< pD->privateExtension.extType.numocts; nLoop++)
        {
			pD->privateExtension.extType.data[nLoop] = pD->smsosfi.data[nLoop];
        }

        //log_writeHex(LEV_ALL_PARAM, "SMSOSFI : ", (char *)pD->smsosfi.data, (int)pD->smsosfi.numocts, 0);
        if(encV3_PrivateExtension(&stMsg[nIdx++], &pD->privateExtension, 0x30) < 0)
        {
            return(-1);
        }
    }

	/* ESN */
	if(pD->m.esnPresent)
	{
		//DebugStr(4, "[%s:%d] esnPresent\n", __func__, __LINE__);

		/* Extension Identifier */
		pD->privateExtension.extId.numids = EXTENSION_OID_TABLE[3].numids;		

		for(nLoop=0; nLoop < pD->privateExtension.extId.numids; nLoop++)
		{
			pD->privateExtension.extId.subid[nLoop] = EXTENSION_OID_TABLE[3].subid[nLoop];
		}

		/* Extension Type (optional) */
		pD->privateExtension.m.extTypePresent = 1;
		pD->privateExtension.extType.numocts = pD->esn.numocts;

		memcpy(pD->privateExtension.extType.data, pD->esn.data, pD->esn.numocts);

		for(nLoop=0; nLoop< pD->privateExtension.extType.numocts; nLoop++)
 		{
 			pD->privateExtension.extType.data[nLoop] =
 					(pD->privateExtension.extType.data[nLoop] >> 4) |
 					(pD->privateExtension.extType.data[nLoop] << 4) ;
 		}

		log_writeHex(LEV_ALL_PARAM, "ESN : ", (char *)pD->esn.data, (int)pD->esn.numocts, 0);
		if(encV3_PrivateExtension(&stMsg[nIdx++], &pD->privateExtension, 0x30) < 0)
		{
			return(-1);
		}
	}

	/* CAPA */
	if(pD->m.capaPresent)
	{
		//DebugStr(4, "[%s:%d] capaPresent\n", __func__, __LINE__);

		/* Extension Identifier */
		pD->privateExtension.extId.numids = EXTENSION_OID_TABLE[4].numids;		

		for(nLoop=0; nLoop < pD->privateExtension.extId.numids; nLoop++)
		{
			pD->privateExtension.extId.subid[nLoop] = EXTENSION_OID_TABLE[4].subid[nLoop];
		}

		/* Extension Type (optional) */
		pD->privateExtension.m.extTypePresent = 1;
		pD->privateExtension.extType.numocts = pD->capa.numocts;

		for(nLoop=0; nLoop< pD->privateExtension.extType.numocts; nLoop++)
		{
			pD->privateExtension.extType.data[nLoop] = pD->capa.data[nLoop];
		}

		//log_writeHex(LEV_ALL_PARAM, "CAPA : ", (char *)pD->capa.data, (int)pD->capa.numocts, 0);
		if(encV3_PrivateExtension(&stMsg[nIdx++], &pD->privateExtension, 0x30) < 0)
		{
			return(-1);
		}
		//log_writeHex(LEV_ALL_PARAM, "CAPA RAW : ", (char *)stMsg[nIdx-1].cpData, (int)stMsg[nIdx-1].nLen, 0);
	}

	/* ROAMING */
	if(pD->m.roamPresent)
	{
		//DebugStr(4, "[%s:%d] roamPresent\n", __func__, __LINE__);

		/* Extension Identifier */
		pD->privateExtension.extId.numids = EXTENSION_OID_TABLE[5].numids;		

		for(nLoop=0; nLoop < pD->privateExtension.extId.numids; nLoop++)
		{
			pD->privateExtension.extId.subid[nLoop] = EXTENSION_OID_TABLE[5].subid[nLoop];
		}

		/* Extension Type (optional) */
		pD->privateExtension.m.extTypePresent = 1;
		pD->privateExtension.extType.numocts = pD->roam.numocts;

		memcpy(pD->privateExtension.extType.data, pD->roam.data, pD->roam.numocts);

		log_writeHex(LEV_ALL_PARAM, "ROAMING : ", (char *)pD->roam.data, (int)pD->roam.numocts, 0);
		if(encV3_PrivateExtension(&stMsg[nIdx++], &pD->privateExtension, 0x30) < 0)
		{
			return(-1);
		}
	}

// NPFI
	if(pD->m.npfiPresent)
	{
		//DebugStr(4, "[%s:%d] npfiPresent\n", __func__, __LINE__);

		/* Extension Identifier */
		pD->privateExtension.extId.numids = EXTENSION_OID_TABLE[6].numids;		

		for(nLoop=0; nLoop < pD->privateExtension.extId.numids; nLoop++)
		{
			pD->privateExtension.extId.subid[nLoop] = EXTENSION_OID_TABLE[6].subid[nLoop];
		}

		/* Extension Type (optional) */
		pD->privateExtension.m.extTypePresent = 1;
		pD->privateExtension.extType.numocts = pD->npfi.numocts;

		memcpy(pD->privateExtension.extType.data, pD->npfi.data, pD->npfi.numocts);

		log_writeHex(LEV_ALL_PARAM, "ROAMING : ", (char *)pD->npfi.data, (int)pD->npfi.numocts, 0);
		if(encV3_PrivateExtension(&stMsg[nIdx++], &pD->privateExtension, 0x30) < 0)
		{
			return(-1);
		}
	}

	/* Access Denied Reason */
	if(pD->m.adrPresent)
	{
		//DebugStr(4, "[%s:%d] adrPresent\n", __func__, __LINE__);

		/* Extension Identifier */
		pD->privateExtension.extId.numids = EXTENSION_OID_TABLE[6].numids;      

		for(nLoop=0; nLoop < pD->privateExtension.extId.numids; nLoop++)
		{
			pD->privateExtension.extId.subid[nLoop] = EXTENSION_OID_TABLE[6].subid[nLoop];
		}

		/* Extension Type (optional) */
		pD->privateExtension.m.extTypePresent = 1;
		pD->privateExtension.extType.numocts = pD->adr.numocts;
		pD->privateExtension.extType.data[0] = pD->adr.data[0];

		log_writeHex(LEV_ALL_PARAM, "ADR : ", (char *)pD->adr.data, (int)pD->adr.numocts, 0);
		if(encV3_PrivateExtension(&stMsg[nIdx++], &pD->privateExtension, 0x30) < 0)
		{
			return(-1);
		}
		log_writeHex(LEV_ALL_PARAM, "ADR RAW : ", (char *)stMsg[nIdx-1].cpData, (int)stMsg[nIdx-1].nLen, 0);
	}	

	/* ROAMING PMN */
	if(pD->m.roampmnPresent)
	{
		//DebugStr(4, "[%s:%d] roampmnPresent\n", __func__, __LINE__);

		/* Extension Identifier */
		pD->privateExtension.extId.numids = EXTENSION_OID_TABLE[7].numids;

		for(nLoop=0; nLoop < pD->privateExtension.extId.numids; nLoop++)
		{
			pD->privateExtension.extId.subid[nLoop] = EXTENSION_OID_TABLE[7].subid[nLoop];
		}

		/* Extension Type (optional) */
		pD->privateExtension.m.extTypePresent = 1;
		pD->privateExtension.extType.numocts = pD->roampmn.numocts;

		memcpy(pD->privateExtension.extType.data, pD->roampmn.data, pD->roampmn.numocts);

		for(nLoop=0; nLoop< pD->privateExtension.extType.numocts; nLoop++)
		{
			pD->privateExtension.extType.data[nLoop] =
				(pD->privateExtension.extType.data[nLoop] >> 4) |
				(pD->privateExtension.extType.data[nLoop] << 4) ;
		}

		log_writeHex(LEV_ALL_PARAM, "ROAMING PMN : ", (char *)pD->roampmn.data, (int)pD->roampmn.numocts, 0);
		if(encV3_PrivateExtension(&stMsg[nIdx++], &pD->privateExtension, 0x30) < 0)
		{
			return(-1);
		}
	}

#if 0
	/* SMS_MO_SAC */  //FR401 LSH
    if(pD->m.sacPresent)
    {
        /* Extension Identifier */
        pD->privateExtension.extId.numids = EXTENSION_OID_TABLE[8].numids;

        for(nLoop=0; nLoop < pD->privateExtension.extId.numids; nLoop++)
        {
            pD->privateExtension.extId.subid[nLoop] = EXTENSION_OID_TABLE[8].subid[nLoop];
        }

        /* Extension Type (optional) */
        pD->privateExtension.m.extTypePresent = 1;
        pD->privateExtension.extType.numocts = pD->sac.numocts;

        memcpy(pD->privateExtension.extType.data, pD->sac.data, pD->sac.numocts);

        for(nLoop=0; nLoop< pD->privateExtension.extType.numocts; nLoop++)
        {
            pD->privateExtension.extType.data[nLoop] =
                (pD->privateExtension.extType.data[nLoop] >> 4) |
                (pD->privateExtension.extType.data[nLoop] << 4) ;
        }

        log_writeHex(LEV_ALL_PARAM, "SMS_MO_SAC : ", (char *)pD->sac.data, (int)pD->sac.numocts, 0);
        if(encV3_PrivateExtension(&stMsg[nIdx++], &pD->privateExtension, 0x30) < 0)
        {
            return(-1);
        }
    }
#endif
    /* SubscriptionInfo*/
    if(pD->m.subscriptioninfoPresent)
    {
		//DebugStr(4, "[%s:%d] roampmnPresent\n", __func__, __LINE__);

		/* Extension Identifier */
		pD->privateExtension.extId.numids = EXTENSION_OID_TABLE[9].numids;

		for(nLoop=0; nLoop < pD->privateExtension.extId.numids; nLoop++)
		{
			pD->privateExtension.extId.subid[nLoop] = EXTENSION_OID_TABLE[9].subid[nLoop];
		}

		/* Extension Type (optional) */
		pD->privateExtension.m.extTypePresent = 1;
		pD->privateExtension.extType.numocts = pD->subscriptioninfo.numocts;

		memcpy(pD->privateExtension.extType.data, pD->subscriptioninfo.data, pD->subscriptioninfo.numocts);

		for(nLoop=0; nLoop< pD->privateExtension.extType.numocts; nLoop++)
		{
			pD->privateExtension.extType.data[nLoop] =
				(pD->privateExtension.extType.data[nLoop] >> 4) |
				(pD->privateExtension.extType.data[nLoop] << 4) ;
		}

		log_writeHex(LEV_ALL_PARAM, "ROAMING PMN : ", (char *)pD->subscriptioninfo.data, (int)pD->subscriptioninfo.numocts, 0);
		if(encV3_PrivateExtension(&stMsg[nIdx++], &pD->privateExtension, 0x30) < 0)
		{
			return(-1);
		}
    }
    if(pD->m.ltesmsPresent)
    {
		//DebugStr(4, "[%s:%d] roampmnPresent\n", __func__, __LINE__);

		/* Extension Identifier */
		pD->privateExtension.extId.numids = EXTENSION_OID_TABLE[10].numids;

		for(nLoop=0; nLoop < pD->privateExtension.extId.numids; nLoop++)
		{
			pD->privateExtension.extId.subid[nLoop] = EXTENSION_OID_TABLE[10].subid[nLoop];
		}

		/* Extension Type (optional) */
		pD->privateExtension.m.extTypePresent = 1;
		pD->privateExtension.extType.numocts = pD->ltesms.numocts;

		memcpy(pD->privateExtension.extType.data, pD->ltesms.data, pD->ltesms.numocts);

		for(nLoop=0; nLoop< pD->privateExtension.extType.numocts; nLoop++)
		{
			pD->privateExtension.extType.data[nLoop] =
				(pD->privateExtension.extType.data[nLoop] >> 4) |
				(pD->privateExtension.extType.data[nLoop] << 4) ;
		}

		log_writeHex(LEV_ALL_PARAM, "LTE SMS : ", (char *)pD->ltesms.data, (int)pD->ltesms.numocts, 0);
		if(encV3_PrivateExtension(&stMsg[nIdx++], &pD->privateExtension, 0x30) < 0)
		{
			return(-1);
		}
    }

	if ( pD->m.ipmg_refidPresent ) {
		//DebugStr(4, "[%s:%d] roampmnPresent\n", __func__, __LINE__);

		/* Extension Identifier */
		pD->privateExtension.extId.numids = EXTENSION_OID_TABLE[11].numids;

		for(nLoop=0; nLoop < pD->privateExtension.extId.numids; nLoop++)
		{
			pD->privateExtension.extId.subid[nLoop] = EXTENSION_OID_TABLE[11].subid[nLoop];
		}

		/* Extension Type (optional) */
		pD->privateExtension.m.extTypePresent = 1;
		pD->privateExtension.extType.numocts = pD->ipmg_refId.numocts;

		memcpy(pD->privateExtension.extType.data, pD->ipmg_refId.data, pD->ipmg_refId.numocts);

		for(nLoop=0; nLoop< pD->privateExtension.extType.numocts; nLoop++)
		{
			pD->privateExtension.extType.data[nLoop] =
				(pD->privateExtension.extType.data[nLoop] >> 4) |
				(pD->privateExtension.extType.data[nLoop] << 4) ;
		}

		log_writeHex(LEV_ALL_PARAM, "IPMG REF : ", (char *)pD->ipmg_refId.data, (int)pD->ipmg_refId.numocts, 0);
		if(encV3_PrivateExtension(&stMsg[nIdx++], &pD->privateExtension, 0x30) < 0)
		{
			return(-1);
		}
	}

	if ( pD->m.ipmg_msisdnPresent ) {
		//DebugStr(4, "[%s:%d] roampmnPresent\n", __func__, __LINE__);

		/* Extension Identifier */
		pD->privateExtension.extId.numids = EXTENSION_OID_TABLE[12].numids;

		for(nLoop=0; nLoop < pD->privateExtension.extId.numids; nLoop++)
		{
			pD->privateExtension.extId.subid[nLoop] = EXTENSION_OID_TABLE[12].subid[nLoop];
		}

		/* Extension Type (optional) */
		pD->privateExtension.m.extTypePresent = 1;
		pD->privateExtension.extType.numocts = pD->ipmg_msisdn.numocts;

		memcpy(pD->privateExtension.extType.data, pD->ipmg_msisdn.data, pD->ipmg_msisdn.numocts);

		/*
		for(nLoop=0; nLoop< pD->privateExtension.extType.numocts; nLoop++)
		{
			pD->privateExtension.extType.data[nLoop] =
				(pD->privateExtension.extType.data[nLoop] >> 4) |
				(pD->privateExtension.extType.data[nLoop] << 4) ;
		}
		*/

		log_writeHex(LEV_ALL_PARAM, "IPMG MSISDN : ", (char *)pD->ipmg_msisdn.data, (int)pD->ipmg_msisdn.numocts, 0);
		if(encV3_PrivateExtension(&stMsg[nIdx++], &pD->privateExtension, 0x30) < 0)
		{
			return(-1);
		}

	}

	if(encTotal(spMsg, stMsg, nIdx, cTag) < 0)
	{
		//LogRet("[%s:%d] encV3_Total fail.\n", __func__, __LINE__);
		return(-1);
	}
	return(1);

}

int encV3_ExtensionContainer(u_char *spMsg, V3_ExtensionContainer *pD, u_char cTag)
{
	int			nIdx = 0;
	stMsgBuf_t	stMsg;	/* total count parameter */

	memset(&stMsg, 0x00, sizeof(stMsgBuf_t));

	/* privateExtensionList */
	if(pD->m.privateExtensionListPresent)
	{
		if(encV3_PrivateExtensionList(&stMsg, &pD->privateExtensionList, 0xA0) < 0)
			return(FLK_FAIL);
	}

	/* pcs_Extension unsupport parameter  */		
	return encTLV(spMsg, stMsg.cpData, stMsg.nLen, cTag);
}
