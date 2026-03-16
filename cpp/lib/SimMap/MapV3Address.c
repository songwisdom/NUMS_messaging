#include <stdio.h>
#include "libMap3G_Addr.h"
#include "libMapAddrUtil.h"


void setV3_AddressBuff ( u_char *Buff, u_char ucTon, u_char ucNpi, u_char *DigitAddr, int DigitOct, int nTag)
{
	V3_AddrBuf_st *V3_Addr = (V3_AddrBuf_st *)Buff;
	V3_Addr->tag = nTag;
	V3_Addr->ton = ucTon;
	V3_Addr->npi = ucNpi;
	V3_Addr->numocts = DigitOct;
	memcpy ( V3_Addr->data, DigitAddr, DigitOct );
	return ;
}

int set_address_toa (  u_char *ucToA, u_char ucTon, u_char ucNpi )
{   
	type_of_address_st *toa = (type_of_address_st *)ucToA;
    toa->FIX = 1;
    toa->TON = ucTon;
    toa->NPI = ucNpi;
    return sizeof(type_of_address_st);
}   

int encV3_AddressString ( u_char *Buff, u_char ucTon, u_char ucNpi, u_char *ucAddr, u_char nNumOct)
{
	int OctLen = 0;
    AddressString *Address = NULL;
	Address = (AddressString *)Buff;
	set_address_toa ( &Address->ToA, ucTon, ucNpi );
	OctLen = convBCD2Gsm(Address->data, BCD_ADDRESS_LEN, ucAddr, (int)nNumOct );
	Address->numocts = getMapBCDLength(Address->data, OctLen);

	return (OctLen + 2);
}

#if 0
//gsm test
int nPos = 0;
int encV3_AddressString ( u_char *Buff, u_char ucTon, u_char ucNpi, u_char *ucAddr, u_char nNumOct)
{
	int OctLen = 0;
    AddressString *Address = NULL;
	Address = (AddressString *)Buff;
	set_address_toa ( &Address->ToA, ucTon, ucNpi );
	OctLen = convBCD2Gsm(Address->data, BCD_ADDRESS_LEN, ucAddr, (int)nNumOct );
	Address->numocts = getMapBCDLength(Address->data, OctLen);

    if(nPos == 0){
        Address->data[4] = 0xf3;
        printf("LOGHJY [%s:%d] nNumOct[%d] OctLen[%d] numocts[%d] data[%02x][%02x][%02x][%02x][%02x]\n",
                __func__,__LINE__,nNumOct,OctLen,Address->numocts,
                Address->data[0],Address->data[1],Address->data[2],Address->data[3],Address->data[4]);
    }
    nPos++;
	return (OctLen + 2);
}
#endif

int encV3_RP_Address ( u_char *Buff, u_char ucTon, u_char ucNpi, u_char *ucAddr, u_char nNumOct)
{
	int OctLen = 0;
    AddressString *Address = NULL;
	Address = (AddressString *)Buff;
	set_address_toa ( &Address->ToA, ucTon, ucNpi );
	OctLen = convBCD2Gsm(Address->data, BCD_ADDRESS_LEN, ucAddr, nNumOct );
	Address->numocts = OctLen +1;

	return (OctLen + 2);
}

int encV3_RP_AddressString ( u_char *Buff, u_char ucTon, u_char ucNpi, u_char *ucAddr, u_char nNumOct, u_char tag )
{
	int nRet = 0;
	Buff[0] = tag;
	nRet = encV3_RP_Address ( Buff +1, ucTon, ucNpi, ucAddr, nNumOct );
	return (nRet + 1);
}

int encV3_SM_RP_DA(u_char *spMsg, V3_AddrBuf_st *pD)
{
	if ( pD->numocts < 0 )
		return FLK_FAIL;

	/* Choice Type */
	switch(pD->tag)
	{
		case V3_SM_RP_DA_imsi						:	/* 0x80 */
			log_writeHex(LEV_ALL_PARAM, "V3_SM_RP_DA_IMSI : ", (char *)pD->data, pD->numocts, 0);
			return encTLV(spMsg, pD->data, pD->numocts, 0x80);
		case V3_SM_RP_DA_lmsi						: 	/* 0x81 */
			log_writeHex(LEV_ALL_PARAM, "V3_SM_RP_DA_LMSI : ", (char *)pD->data, pD->numocts, 0);
			return encTLV(spMsg, pD->data, pD->numocts, 0x81);
		case V3_SM_RP_DA_serviceCentreAddressDA	:	/* 0x84 */
			log_write(LEV_ALL_PARAM, "V3_SM_RP_DA_serviceCentreAddressDA : TON H'[%02X] NPI H'[%02X]\n", pD->ton, pD->npi );
			log_writeHex(LEV_ALL_PARAM, "V3_SM_RP_DA_serviceCentreAddressDA : ",(char *) pD->data, pD->numocts, 0);
			return encV3_RP_AddressString(spMsg, pD->ton, pD->npi, pD->data, pD->numocts, 0x84);
		case V3_SM_RP_DA_noSM_RP_DA				: 	/* 0x85 */
			return encTLV(spMsg, NULL, 0, 0x85); break;
		default									:
			return FLK_FAIL;	
	}

}

int encV3_SM_RP_OA(u_char *spMsg, V3_AddrBuf_st *pD)
{
	if ( pD->numocts < 0 )
		return FLK_FAIL;
	/* Choice Type */
	switch(pD->tag)
	{
		case V3_SM_RP_OA_msisdn					:	/* 0x82 */
			log_write(LEV_ALL_PARAM, "V3_SM_RP_OA_MSISDN : TON H'[%02X] NPI H'[%02X]\n", pD->ton, pD->npi );
			log_writeHex(LEV_ALL_PARAM, "V3_SM_RP_OA_MSISDN : ", (char *)pD->data, pD->numocts, 0);
			return encV3_RP_AddressString(spMsg, pD->ton, pD->npi, pD->data, pD->numocts, 0x82);
		case V3_SM_RP_OA_serviceCentreAddressOA	: 	/* 0x84 */
			log_write(LEV_ALL_PARAM, "V3_SM_RP_OA_serviceCentreAddressOA : TON H'[%02X] NPI H'[%02X]\n", pD->ton, pD->npi );
			log_writeHex(LEV_ALL_PARAM, "V3_SM_RP_OA_serviceCentreAddressOA : ", (char *)pD->data, pD->numocts, 0);
			return encV3_RP_AddressString(spMsg, pD->ton, pD->npi, pD->data, pD->numocts, 0x84);
		case V3_SM_RP_OA_noSM_RP_OA				:	/* 0x85 */
			return encTLV(spMsg, NULL, 0, 0x85);
		default									:
			//LogRet("This is not OA type [%x]\n", pD->t);
			return FLK_FAIL;	
	}
}

int encV3_Address(stMsgBuf_t *spMsg, u_char *Addr, u_char numocts, u_char cTag)
{
	int		OctLen = 0;
	u_char		cOut[ADDRESS_LEN_BCD];

	memset(cOut, 0x00, ADDRESS_LEN_BCD);
	OctLen = convBCD2Gsm(cOut, ADDRESS_LEN_BCD, Addr, numocts);
	
	spMsg->nLen = encTLV(spMsg->cpData, cOut, OctLen, cTag);
	
	return(1);
}

