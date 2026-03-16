#include "libMapV3FowardSM.h"
#include "libMapV3Extension.h"
#include "libMapUtil.h"
#include "libMap3G_Addr.h"
#include "libMap3G_st.h"
#include "libdbg.h"
int decV3_ExtensionContainer(stMsgBuf_t *spMsg, V3_ExtensionContainer *pD, u_char cTag);

int V3_TBCDString(u_char *cpIn, u_char *cpOut, int nLen);
int decV3_AddressString(stMsgBuf_t *spMsg, V3_AddressString *pD, u_char cTag);
int decV3_ISDN_AddressString(stMsgBuf_t *spMsg, V3_ISDN_AddressString *pD, u_char cTag);
int decV3_INTEGER2(stMsgBuf_t *spMsg, V3_SM_RP_MTI *pD, u_char cTag);
int decV3_BOOLEAN(stMsgBuf_t *spMsg, V3_BOOLEAN *pD, u_char cTag);
int decV3_NULL(stMsgBuf_t *spMsg, u_char *pD, u_char cTag);
int decV3_SM_RP_SMEA(stMsgBuf_t *spMsg, V3_SM_RP_SMEA *pD, u_char cTag);
int
decV3_Length(int *nLen, u_char *cpData, int *npIdx)
{
    int     nTemp;
    int     nIdx = 0;

    switch(cpData[nIdx])
    {
        case 0x81   :
            nIdx++;
            *nLen = (int)(cpData[nIdx]);
            *npIdx = *npIdx + 2;
            break;
        case 0x82   :
            nIdx++;
            nTemp = cpData[nIdx];
            nTemp = nTemp << 8;
            nIdx++;
            nTemp = nTemp | cpData[nIdx];
            *nLen = nTemp;
            *npIdx = *npIdx + 3;
            break;
        default     :
            if((char)cpData[nIdx] < 0 || cpData[nIdx] > 127)
            {
                return(-1);
            }
            *nLen = (int)(cpData[nIdx]);
            (*npIdx)++;
    }
    return(1);
}

int
decV3_INTEGER2(stMsgBuf_t *spMsg, V3_SM_RP_MTI *pD, u_char cTag)
{
    int     nStep   = 0;
    int     nLen;


    if(cTag != spMsg->cpData[nStep++])
    {
        log_write(LEV_ALL_ERR, "decV3_INTERGER tag fail [%x]\n", spMsg->cpData[0]);
        return(-1);
    }
    //if(decLength(&nLen, &spMsg->cpData[nStep], &nStep) < 0)
    if(decV3_Length(&nLen, &spMsg->cpData[nStep], &nStep) < 0)
        return(-1);

    if((nLen > 4) || (nLen < 0))
    {
        log_write(LEV_ALL_ERR, "decV3_INTERGER length fail [%d]\n", nLen);
        return(-1);
    }

	if(nLen > 0){
		unsigned char cIntdata[4];
		int nIntData = 0;
		memset(cIntdata,0x00,sizeof(cIntdata));
		memcpy(&cIntdata[sizeof(int)-nLen], &spMsg->cpData[nStep], nLen);
		memcpy(&nIntData, cIntdata, sizeof(int));
		*pD = nIntData;
		//memcpy(&pD[sizeof(int)-nLen], &spMsg->cpData[nStep], nLen);
		nStep = nStep + nLen;
	}
    return(nStep);
}

int decV3_ReportSM_DeliveryStatus_Arg(stMsgBuf_t *spMsg, V3_ReportSM_DeliveryStatusArg *pD, u_char cTag)
{
    printf("[%s:%d] \n", __func__, __LINE__);
    stMsgBuf_t      stMsg;
    int             nStep   = 0;
    int             nLen    = 0;
    int             nSize   = 0;
    int             nlSize  = 0;


    printf(" TAG [%02x] [%02x] \n", cTag, spMsg->cpData[nStep]);

    if(cTag != spMsg->cpData[nStep++])
    {
        log_write(LEV_ALL_ERR, "decV3_ReportSM_DeliveryStatus_Arg tag fail [%x]\n", spMsg->cpData[0]);
        return(-1);
    }

    if(decLength(&nLen, &spMsg->cpData[nStep], &nStep) < 0)
        return(-1);

    printf("[%s:%d] \n", __func__, __LINE__);
    /* check total length */
    nlSize = spMsg->nLen - nStep;
    if((nlSize) != nLen)
    {
        log_write(LEV_ALL_ERR, "decV3_ReportSM_DeliveryStatus_Arg length fail [%d] = [%d]\n", nlSize, nLen);
    }

    /* msisdn */
    memset(&stMsg, 0x00, sizeof(stMsgBuf_t));
    stMsg.nLen  = nlSize;
    memcpy(stMsg.cpData, &spMsg->cpData[nStep], stMsg.nLen);
    nSize = decV3_ISDN_AddressString(&stMsg, &pD->msisdn, 0x04);
    if(nSize < 0) return(-1);
    nlSize      = nlSize - nSize;
    nStep       = nStep + nSize;

/*
    printf("LJH TEST [%02x] [%02x] [%02x] [%02x] [%02x] [%02x] [%02x] \n",
            stMsg.cpData[0], stMsg.cpData[1], stMsg.cpData[2], stMsg.cpData[3], stMsg.cpData[4]
            stMsg.cpData[5], stMsg.cpData[6]);
            */

    /* Service Centre Address */
    memset(&stMsg, 0x00, sizeof(stMsgBuf_t));
    stMsg.nLen  = nlSize;
    memcpy(stMsg.cpData, &spMsg->cpData[nStep], stMsg.nLen);
    nSize = decV3_AddressString(&stMsg, &pD->serviceCentreAddress, 0x04);
    if(nSize < 0)
        return(-1);
    nlSize      = nlSize - nSize;
    nStep       = nStep + nSize;

    /*  sm-DeliveryOutcome */
    memset(&stMsg, 0x00, sizeof(stMsgBuf_t));
    stMsg.nLen  = nlSize;
    memcpy(stMsg.cpData, &spMsg->cpData[nStep], stMsg.nLen);
    int Tmp_sm_DeliveryOutcome = 0;
    //nSize = decV3_INTEGER2(&stMsg, &pD->sm_DeliveryOutcome, 0x0A);
    nSize = decV3_INTEGER2(&stMsg, &Tmp_sm_DeliveryOutcome, 0x0A);
    if(nSize < 0)
        return(-1);
	if(nSize == 2){
		pD->m.deliveryOutcomePresent = 0;
	}else{
		pD->m.deliveryOutcomePresent = 1;
	}
    pD->sm_DeliveryOutcome = (V3_SM_DeliveryOutcome_Root)Tmp_sm_DeliveryOutcome;
    nlSize      = nlSize - nSize;
    nStep       = nStep + nSize;
/* absentSubscriberDiagnosticSM */
    memset(&stMsg, 0x00, sizeof(stMsgBuf_t));
    stMsg.nLen  = nlSize;
    memcpy(stMsg.cpData, &spMsg->cpData[nStep], stMsg.nLen);
    if(stMsg.cpData[0] == 0x80)
    {
        nSize = decV3_INTEGER2(&stMsg, &pD->absentSubscriberDiagnosticSM, 0x80);
        if(nSize < 0) return(-1);
        nlSize      = nlSize - nSize;
        nStep       = nStep + nSize;
    }

    /* gprsSupportIndicator */
    memset(&stMsg, 0x00, sizeof(stMsgBuf_t));
    stMsg.nLen  = nlSize;
    memcpy(stMsg.cpData, &spMsg->cpData[nStep], stMsg.nLen);
    if(stMsg.cpData[0] == 0x82)
    {
        nSize = decV3_NULL(&stMsg, (u_char*)&pD->absentSubscriberDiagnosticSM, 0x82);
        if(nSize < 0) return(-1);
        nlSize      = nlSize - nSize;
        nStep       = nStep + nSize;
		pD->m.gprsSupportIndicatorPresent = 1;
    }

    /* deliverOutcomeIndicator */
    memset(&stMsg, 0x00, sizeof(stMsgBuf_t));
    stMsg.nLen  = nlSize;
    memcpy(stMsg.cpData, &spMsg->cpData[nStep], stMsg.nLen);
    if(stMsg.cpData[0] == 0x83)
    {
        nSize = decV3_NULL(&stMsg, (u_char*)&pD->absentSubscriberDiagnosticSM, 0x83);
        if(nSize < 0) return(-1);
        nlSize      = nlSize - nSize;
        nStep       = nStep + nSize;
		pD->m.deliveryOutcomeIndicatorPresent = 1;
    }

    /* additionalSM-DeliveryOutcome */
    memset(&stMsg, 0x00, sizeof(stMsgBuf_t));
    stMsg.nLen  = nlSize;
    memcpy(stMsg.cpData, &spMsg->cpData[nStep], stMsg.nLen);
    if(stMsg.cpData[0] == 0x84)
    {
        nSize = decV3_INTEGER2(&stMsg, (V3_SM_RP_MTI*)&pD->additionalSM_DeliveryOutcome, 0x84);
        if(nSize < 0) return(-1);
        nlSize      = nlSize - nSize;
        nStep       = nStep + nSize;
    }

    /* additionalAbsentSubscriberDiagnosticSM */
    memset(&stMsg, 0x00, sizeof(stMsgBuf_t));
    stMsg.nLen  = nlSize;
    memcpy(stMsg.cpData, &spMsg->cpData[nStep], stMsg.nLen);
    if(stMsg.cpData[0] == 0x85)
    {
        nSize = decV3_INTEGER2(&stMsg, &pD->additionalAbsentSubscriberDiagnosticSM, 0x85);
        if(nSize < 0) return(-1);
        nlSize      = nlSize - nSize;
        nStep       = nStep + nSize;
    }

    /* ip-sm-gw indicator */
    memset(&stMsg, 0x00, sizeof(stMsgBuf_t));
    stMsg.nLen  = nlSize;
    memcpy(stMsg.cpData, &spMsg->cpData[nStep], stMsg.nLen);
    if(stMsg.cpData[0] == 0x86)
    {
		int D;
        nSize = decV3_NULL(&stMsg, (u_char*)&D, 0x86);
        if(nSize < 0) return(-1);
        nlSize      = nlSize - nSize;
        nStep       = nStep + nSize;
		pD->n.ipsmgwDeliveryOutcomeIndicatorPresent = 1;
    }

    /* ip-sm-gw SM-DeliveryOutcome */
    memset(&stMsg, 0x00, sizeof(stMsgBuf_t));
    stMsg.nLen  = nlSize;
    memcpy(stMsg.cpData, &spMsg->cpData[nStep], stMsg.nLen);
    if(stMsg.cpData[0] == 0x87)
    {
        nSize = decV3_INTEGER2(&stMsg, (V3_SM_RP_MTI*)&pD->ipsmgwSM_DeliveryOutcome, 0x87);
        if(nSize < 0) return(-1);
        nlSize      = nlSize - nSize;
        nStep       = nStep + nSize;
    }

    /* ip-sm-gw AbsentSubscriberDiagnosticSM */
    memset(&stMsg, 0x00, sizeof(stMsgBuf_t));
    stMsg.nLen  = nlSize;
    memcpy(stMsg.cpData, &spMsg->cpData[nStep], stMsg.nLen);
    if(stMsg.cpData[0] == 0x88)
    {
        nSize = decV3_INTEGER2(&stMsg, &pD->ipsmgwAbsentSubscriberDiagnosticSM, 0x88);
        if(nSize < 0) return(-1);
        nlSize      = nlSize - nSize;
        nStep       = nStep + nSize;
    }

    if(nlSize != 0)
    {
        log_write(LEV_ALL_ERR, "decV3_ReportSM_DeliveryStatus_Arg left length fail [%d]\n", nlSize);
        return(-1);
    }

    return(1);
}

