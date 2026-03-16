#include <stdio.h>
#include <string.h>
#include "flk_type.h"
#include "libMapV3FowardSM.h"
#include "libMapAddrUtil.h"
#include "libMap3G_Addr.h"
#include "libMap3G_st.h"
#include "flk_iconv.h"
#pragma pack 1
typedef struct sms_submit_sm_header_t
{
    BIT_T TP_MTI    : 2;
    BIT_T TP_RD     : 1;
    BIT_T TP_VPF    : 2;
    BIT_T TP_RP     : 1;
    BIT_T TP_UDHI   : 1; // optional
    BIT_T TP_SRR    : 1; // optional
}sms_submit_sm_header_st;
#pragma pack FLK_PACKED_RELEASE

typedef struct V3_Contents_Arg_t
{
	u_char Tag;
	u_char Length;
	u_char Value[32];
}V3_Contents_Arg_st;

typedef struct V3_Contents_t 
{
	u_char TP_DCS;
	int TP_UDL;
	int TP_UDHL;
	int ContentsSize;
	bool ConcatenatedFlag;
	bool CallBackFlag;
	V3_Contents_Arg_st Concatenated;
	V3_Contents_Arg_st CallBack;
	u_char  				Contents[256];
}V3_Contents_st;

int setTP_PID ( u_char *Data, u_char TP_PID)
{
	int nRet = (int)sizeof(TP_PID);
	memcpy (Data, &TP_PID, nRet);
	return nRet;
}

int setTP_MR(u_char *Data)
{
	static u_char ucMR = 0;
	*Data = 0;
	return (int)sizeof(u_char);
}
int setTP_DCS(u_char *Data, u_char Dcs)
{
	*Data = Dcs;
	return (int)sizeof(Dcs);
}
    
int enc_sms_submit_header ( sms_submit_sm_header_st *Header,
        u_char TP_RD, u_char TP_VPF, u_char TP_RP, u_char TP_UDHI, u_char TP_SRR )
{
    Header->TP_MTI  = MTI_SMS_SUBMIT;
    Header->TP_RD   = TP_RD;
    Header->TP_VPF  = TP_VPF;
    Header->TP_RP   = TP_RD;
    Header->TP_UDHI = TP_UDHI;
    Header->TP_SRR  = TP_SRR;

	log_write (LEV_ALL_PARAM, "TP_MTI   : H'[%02X]\n", Header->TP_MTI);
	log_write (LEV_ALL_PARAM, "TP_RD    : H'[%02X]\n", Header->TP_RD);
	log_write (LEV_ALL_PARAM, "TP_VPF   : H'[%02X]\n", Header->TP_VPF);
	log_write (LEV_ALL_PARAM, "TP_RP    : H'[%02X]\n", Header->TP_RP);
	log_write (LEV_ALL_PARAM, "TP_UDHI  : H'[%02X]\n", Header->TP_UDHI);
	log_write (LEV_ALL_PARAM, "TP_SRR   : H'[%02X]\n", Header->TP_SRR);
   	//Linux porting 
	//return sizeof(sms_submit_sm_header_st);
	return 1;
}   

bool encV3_RP_UI_Callback( V3_Contents_st *Contents, V3_AddrBuf_st *CallBack )
{
	int CallBackTot = 0;
	if ( CallBack->tag != TPDU_REPLY_ADDRESS_TAG )
		return false;

	Contents->CallBackFlag = true;
	Contents->CallBack.Tag = TPDU_REPLY_ADDRESS_TAG;
	Contents->CallBack.Length = encV3_AddressString ( Contents->CallBack.Value, 
			CallBack->ton, CallBack->npi, CallBack->data, CallBack->numocts);
	CallBackTot = (Contents->CallBack.Length + 2);
	Contents->TP_UDHL += CallBackTot;
	Contents->TP_UDL += CallBackTot;
	log_writeHex(LEV_ALL_PARAM, "CallBack : ", (char*)CallBack->data, (int)CallBack->numocts, 0);
	return true;
}

int convertGsm7bit ( u_char *Target, u_char *Data, int Len )
{
	void Conv8bit2GSM7bit(stASCIIString  *spSource, stGSM7bitString *spTarget);
	stASCIIString AsciiStr;
	stGSM7bitString Gsm7bitStr;
	bzero( &AsciiStr, sizeof(AsciiStr));
	bzero( &Gsm7bitStr, sizeof(Gsm7bitStr));
	memcpy(AsciiStr.data, Data, Len); 
	AsciiStr.nCharacter = Len;
	Conv8bit2GSM7bit( &AsciiStr, &Gsm7bitStr);	
	memcpy(Target, &Gsm7bitStr.data, Gsm7bitStr.nOctet );
	return Gsm7bitStr.nCharacter;
}

int set_V3_UI_Contents(V3_Contents_st *Contents, u_char TP_DCS, u_char TP_UDL, u_char *Data)
{
	int size = 0;
	int ud_len = (int)TP_UDL;
	char tmp[30] = {0x0, };
	char buf[256]= {0x0, };
	int ext_len = 0;

	if ( TP_DCS == DCS_TYPE_8BIT || TP_DCS == DCS_TYPE_GSM7BIT ) {
		if ( isAsciiAlphabet(Data, TP_UDL ) == false ) {
			TP_DCS = DCS_TYPE_KSC5601;
		}
	}

	switch ( TP_DCS ) {
		case DCS_TYPE_UCS2 :
			{
            if ( flk_convert_contents ( FLK_ICONV_CP949, (char *)Data, ud_len,
                    FLK_ICONV_UCS2, (char *)buf, &size,0,&ext_len ) == FLK_FAIL ) {
				return FLK_FAIL;
			}
			memcpy(Contents->Contents, buf, size);
			Contents->ContentsSize = size;
			
			printf("UCS2 Data Length[%d]\n", size);
			int k = 0;
			for(k = 0; k < size; ++k) {
				if ( k % 10 == 0 )
					puts("");
				printf("[%02X] ", (unsigned char)buf[k]);
			}
			for(k = 0; k < ud_len; ++k) {
				if ( k % 10 == 0 )
					puts("");
				printf("[%02X] ", (unsigned char)Data[k]);
			}
			
			TP_DCS = 0x08;
			}
			break;
		case DCS_TYPE_8BIT :
			memcpy ( Contents->Contents, Data, (int)TP_UDL);
			Contents->ContentsSize = TP_UDL;
			TP_DCS = 0x15;
			break;
		case DCS_TYPE_GSM7BIT :
			Contents->ContentsSize = convertGsm7bit(Contents->Contents, Data, (int)TP_UDL);
			TP_DCS = 0x00;
			break;
		case DCS_TYPE_KSC5601 :
		default :
			TP_DCS = 0x84;
			//TP_DCS = 0x95;
			memcpy ( Contents->Contents, Data, TP_UDL);
			Contents->ContentsSize = TP_UDL;
	}

	memcpy ( Contents->Contents, Data, TP_UDL);
	Contents->ContentsSize = TP_UDL;
	
	Contents->TP_DCS = TP_DCS;
	Contents->TP_UDL += (Contents->ContentsSize);

	return Contents->ContentsSize;
}

void encV3_RP_UI_Concatenated (V3_Contents_st *contents, V3_Concatenated_st *long_sms )
{
	if (long_sms->Active == true ) {
		contents->ConcatenatedFlag = true;

		contents->Concatenated.Tag = long_sms->Tag;
		contents->Concatenated.Length = long_sms->Len;
		contents->Concatenated.Value[0] = long_sms->RefNum;
		contents->Concatenated.Value[1] = long_sms->Max;
		contents->Concatenated.Value[2] = long_sms->Seq;
		contents->TP_UDL += 5;
		contents->TP_UDHL += 5;
	}
	return;
}
int encV3_RP_UI (V3_SMS_Submit_st *Submit, V3_SignalInfo *pD)
{
	int nLength = 0;
	u_char *Ptr = 0;
	V3_Contents_st Contents;
	bzero(&Contents, sizeof(Contents));

	Ptr = pD->data;
	/* Set Header TP_MTI TP_RD TP_VPF TP_RP TP_UDHI TP_SRR */
	sms_submit_sm_header_st *Header = (sms_submit_sm_header_st *)Ptr;
	nLength = enc_sms_submit_header (Header, Submit->TP_RD, Submit->TP_VPF, Submit->TP_RP, 
			Submit->TP_UDHI, Submit->TP_SRR);
	Ptr = getBufferPtr ( pD->data, &pD->numocts, nLength );

	/* Set TP MR */
	nLength = setTP_MR(Ptr);
	Ptr = getBufferPtr ( pD->data, &pD->numocts, nLength );

	/* Set TP DA */
	//log_writeHex (LEV_ALL_PARAM, "TP-DA    : ADDRESS H'", (char *)Submit->TP_DA.data, Submit->TP_DA.numocts, 0);
	nLength = encV3_AddressString ( Ptr, Submit->TP_DA.ton, Submit->TP_DA.npi, 
			Submit->TP_DA.data, Submit->TP_DA.numocts);
	Ptr = getBufferPtr ( pD->data, &pD->numocts, nLength );

	/* Set TP PID */
	nLength = setTP_PID(Ptr, Submit->TP_PID);
	Ptr = getBufferPtr ( pD->data, &pD->numocts, nLength );

	/* Set TP VP */
	if ( Header->TP_VPF != 0 ) {
		Header->TP_VPF = 0;
		/* to do */
	}

	/* Set CallBack */
	//log_writeHex(LEV_ALL_PARAM, "Contents CallBack : H'", (char *)Submit->CallBack.data, Submit->CallBack.numocts, 0);
	encV3_RP_UI_Callback(&Contents, &Submit->CallBack);

	/* Set Concatenated */
	encV3_RP_UI_Concatenated ( &Contents, &Submit->Concatenated);

	/* Set TP UD */
	if ( set_V3_UI_Contents(&Contents, Submit->TP_DCS, Submit->TP_UDL, Submit->TP_UD) == FLK_FAIL )
		return FLK_FAIL;

	/* Set TP DCS */
	//nLength = setTP_DCS(Ptr, Contents.TP_DCS);
	switch(Submit->TP_DCS) {
		case DCS_TYPE_GSM7BIT:
			nLength = setTP_DCS(Ptr, 0x00);
			break;
		case DCS_TYPE_UCS2:
			nLength = setTP_DCS(Ptr, 0x08);
			break;
		case DCS_TYPE_8BIT:
			nLength = setTP_DCS(Ptr, 0x04);
			break;
		case DCS_TYPE_KSC5601:
		default:
			nLength = setTP_DCS(Ptr, 0x84);
			break;
	}
	Ptr = getBufferPtr ( pD->data, &pD->numocts, nLength );

	/* Set TP UDL */
	nLength = 0;
	if ( Submit->TP_UDHI ) {
		Contents.TP_UDL += 1;
	}

	/*
	if ( encLength(Contents.TP_UDL, Ptr, &nLength) == FLK_FAIL ) {
		return FLK_FAIL;
	}
	*/
	nLength = 1;
	if ( Submit->TP_UDHI && Submit->TP_DCS == DCS_TYPE_GSM7BIT ) {
		int nStep       = Contents.TP_UDHL +1;
		//int nUDHCharNum = nStep;
		int nUDHCharNum = (nStep*8)/7 + ( (nStep*8)%7 == 0 ? 0 : 1);
		Ptr[0] = (u_char)(nUDHCharNum + Contents.ContentsSize );
		//int nUDLNum = nUDHCharNum + Contents.ContentsSize;
		//Ptr[0] = (nUDLNum*7)/8 + ( ((nUDLNum*7)%8) == 0 ? 0 : 1 );

	}
	else
		Ptr[0] = (u_char)Contents.TP_UDL;
	Ptr = getBufferPtr ( pD->data, &pD->numocts, nLength );

	if ( Submit->TP_UDHI )  {
		/* Set UDHL */
		nLength = 1;
		Ptr[0] = Contents.TP_UDHL;
		Ptr = getBufferPtr ( pD->data, &pD->numocts, nLength );

		/* Set Concatenated */
		if ( Contents.ConcatenatedFlag ) {
			nLength = encTLV ( Ptr, Contents.Concatenated.Value, 
					Contents.Concatenated.Length, Contents.Concatenated.Tag);
			Ptr = getBufferPtr ( pD->data, &pD->numocts, nLength );
		}

		/* Set Callback */
		if ( Contents.CallBackFlag ) {
			nLength = encTLV(Ptr, Contents.CallBack.Value, 
					Contents.CallBack.Length, Contents.CallBack.Tag);
			Ptr = getBufferPtr ( pD->data, &pD->numocts, nLength );
		}

	}

	/*
	if ( Submit->TP_DCS == DCS_TYPE_GSM7BIT ) {

		int real_size = 0;
		int tmp7 = 0;
		tmp7 = Contents.ContentsSize * 7;
		real_size = (tmp7/8) + (((tmp7%8) == 0) ? 0 : 1);
		memcpy (Ptr, Contents.Contents, real_size);
		pD->numocts += (int)(real_size);
		
	}
	else {
		memcpy (Ptr, Contents.Contents, Contents.ContentsSize);
		pD->numocts += (int)(Contents.ContentsSize);
	}
	*/
    if ( Submit->TP_DCS == DCS_TYPE_GSM7BIT ) {
        int real_size = 0;
        int tmp7 = 0;
        if ( Submit->TP_UDHI ) {
            int nIndex1=0, nIndex2=0;
            u_char ucTemp = 0;
            int tot = Contents.ContentsSize +1;
			printf("tot = %d\n", tot);
            int nRealUDOctet = ((tot*7)/8) + (((tot*7)%8) == 0 ? 0 : 1);
			printf("nRealUDOctet = %d\n", nRealUDOctet);
			printf("nUDHL [%d]\n", Contents.TP_UDHL);
            int nStep       = Contents.TP_UDHL +1;
            int nUDHCharNum = ((nStep*8)/7) + ( ((nStep*8)%7) == 0 ? 0 : 1);
            int nUDHL = nStep;
            int nShiftP = (nUDHCharNum*7) - (nUDHL*8);

            while(Contents.ContentsSize > nIndex2)
            {
                Ptr[nIndex1] |= (Contents.Contents[nIndex2] << nShiftP);
                if(nShiftP != 0)
                {
                    nIndex1++;
                    Ptr[nIndex1] |=
                        (Contents.Contents[nIndex2] >> (8-nShiftP));
                    nIndex2++;
                }
                else
                {
                    nIndex1++;
                    nIndex2++;
                }
            }
            
            pD->numocts += nRealUDOctet;
            real_size = nRealUDOctet;
        

        }
        else {
            tmp7 = Contents.ContentsSize * 7;
            real_size = (tmp7/8) + (((tmp7%8) == 0) ? 0 : 1) ;
            memcpy (Ptr, Contents.Contents, real_size);
            pD->numocts += (int)(real_size);
        }   
    }
    else {
        memcpy (Ptr, Contents.Contents, Contents.ContentsSize);
        pD->numocts += (int)(Contents.ContentsSize);
    }

	return pD->numocts;
}

int encV3_MO_ForwardSM_Arg(stMsgBuf_t *spMsg, V3_MO_ForwardSM_Arg *pD, u_char cTag)
{
	int			nIdx = 0;
	u_char 		*BufferPtr = NULL;
	int 		nRet = 0;
	stMsgBuf_t 	stTmp;
	V3_SignalInfo UD;

	bzero(&stTmp, sizeof(stMsgBuf_t));
	bzero(&UD, sizeof(UD));
	BufferPtr = stTmp.cpData;
	/* DA */
	nRet = encV3_SM_RP_DA(BufferPtr, &pD->sm_RP_DA);
	if( nRet < 0) return FLK_FAIL;
	BufferPtr = getBufferPtr(stTmp.cpData, &stTmp.nLen, nRet);


	/* OA */
	nRet = encV3_SM_RP_OA(BufferPtr, &pD->sm_RP_OA);
	if ( nRet < 0) return FLK_FAIL;
	BufferPtr = getBufferPtr(stTmp.cpData, &stTmp.nLen, nRet);

	/* Signal Info */
	nRet = encV3_RP_UI(&pD->sm_RP_UI, &UD);
	nRet = encTLV(BufferPtr, UD.data, UD.numocts, 0x04);
	if ( nRet < 0) return FLK_FAIL;
	BufferPtr = getBufferPtr(stTmp.cpData, &stTmp.nLen, nRet);

	/* extension container (optional) */
	if(pD->m.extensionContainerPresent == true)
	{
    	nRet = encV3_ExtensionContainer(BufferPtr,&pD->extensionContainer,0x30);
		if ( nRet < 0) return FLK_FAIL;
		BufferPtr = getBufferPtr(stTmp.cpData, &stTmp.nLen, nRet);
	}
	
	/* IMSI (optional) */
	if(pD->m.imsiPresent)
	{
		log_writeHex (LEV_ALL_PARAM, "IMSI : ", (char*)pD->imsi.data, pD->imsi.numocts, 0);
		nRet = encTLV(BufferPtr, pD->imsi.data, pD->imsi.numocts, 0x04);
		if ( nRet < 0) return FLK_FAIL;
		BufferPtr = getBufferPtr(stTmp.cpData, &stTmp.nLen, nRet);
	}

	nRet = encTLV(spMsg->cpData, stTmp.cpData, stTmp.nLen, cTag);
	if ( nRet < 0) return FLK_FAIL;

	spMsg->nLen = nRet;
	
	return nRet;
}
