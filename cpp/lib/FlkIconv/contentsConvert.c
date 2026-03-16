#include <stdio.h>
#include <string.h>

#include "sim_iconv.h"

char * flk_hex_to_string ( const char *cpHex, int nHexLength )
{
    int idx = 0;
	static char buffer [ 1024 ] = { 0x0, };
	char *cpString;
	unsigned char cTmp;
    static char hex_char[17] = {
        '0', '1', '2', '3', '4', '5', '6', '7', '8',
        '9', 'A', 'B', 'C', 'D', 'E', 'F', 0x0 };
	memset(buffer,0x00,sizeof(buffer));
	cpString = buffer;
    for ( idx = 0; idx < nHexLength; ++idx ) {
        if ( (idx % 8) == 0 ) {
            *cpString++ = 0x0A;
        }
		cTmp = cpHex[idx];
        *cpString++ = '0';
        *cpString++ = 'x';
        *cpString++ = hex_char[((cTmp & 0xF0) >> 4)];
        *cpString++ = hex_char[(cTmp & 0x0F)];
        *cpString++ = 0x20;
    }
    *cpString = '\0';
    return buffer;
}
/*
void flk_dbg_write ( FILE *fp, const char *cpType, char *cpData, size_t nLength )
{
	if ( fp == NULL )
		return;

	if ( strcmp ( cpType, FLK_ICONV_UCS2 ) == 0 ) {
		fprintf ( fp, "\t[HEX Data] \n%s\n", flk_hex_to_string ( cpData, nLength) );
	}
	else {
		fprintf ( fp, "\t[String]\n%s\n",  cpData );
	}
	return;
}

void flk_iconv_dbg ( int nResult, 
		const char *cpCType, char *cpCData, size_t nCDataLen,
		const char *cpTType, char *cpTData, size_t npTDataLen )
{
	FILE *fp;

	fp = fopen ("/home/dsmsc/temp/iconv_log.log", "a+");
	if ( fp == NULL )
		return;

	fprintf ( fp, "=================================================\n");
	fprintf ( fp, " Convert Contents Result (%d)\n", nResult);
	fprintf ( fp, " Time : %s\n", __TIME__);
	fprintf ( fp, " Source [ %s ] Length(%d)\n", cpCType, nCDataLen );
	flk_dbg_write ( fp, cpCType, cpCData, nCDataLen );
	fprintf ( fp, "-------------------------------------------------\n");
	fprintf ( fp, " Convert Type [ %s ] Length(%d)\n", cpTType, npTDataLen );
	flk_dbg_write ( fp, cpTType, cpTData, npTDataLen );
	fprintf ( fp, "=================================================\n");
	fclose (fp);
	return;
}
*/
int flk_convert_contents (
		const char 	*cpCType, 
		const char	*cpCData,
		size_t 		nCDataLen, 
		const char 	*cpTType, 
		char		*cpTData,
		size_t 		*npTDataLen,
		int 		ext_flag, 
		int 		*ext_length)
{
	int ret = 0;
    struct flk_conv_struct *cd;
	size_t out_length = *npTDataLen;
	
	
	*ext_length = 0;

	FLK_ICONV_TYPE from_type = (FLK_ICONV_TYPE)-1, to_type = (FLK_ICONV_TYPE)-1;

	// 09.09.02 by chan for contents length 0 
	if (nCDataLen == 0)
		return 0;

	if ( cpCData == NULL || nCDataLen < 0 || cpTData == NULL )
		return -1;

	/* For SMSC TPDU MAX Length */
	if (*npTDataLen <= 0 )
		out_length = 512; //DR420

	from_type = get_flk_iconv_type ( cpCType );
	to_type = get_flk_iconv_type ( cpTType );

	if ( from_type == -1 || to_type == -1 || to_type == from_type )
		return -1;

    cd = (flk_conv_t)flk_iconv_open ( from_type, to_type );
    if ( cd == NULL )
        return -1;

	if ( ext_flag )
		cd->ext_flag = ICONV_EXT_FLAG_ON;

	ret = flk_iconv ( cd, &cpCData, nCDataLen, &cpTData, &out_length );
	if ( ret != FLK_ICONV_ERR_INVALID )
		*npTDataLen = out_length;

	if ( ext_flag == ICONV_EXT_FLAG_ON ) {
		*ext_length = cd->ext_length;
	}

	flk_iconv_close (cd);
	
	return ret;
}
/*
int flk_iconv_remove_ext_flag(unsigned char *src, int src_len, unsigned char *dst, int *dst_len )
{
	int idx = 0;
	int len = 0;

	u_int in_char = 0;
	u_int out_char = 0;
	unsigned char *ptr = 0x00;

	for ( idx = 0; idx < src_len; ++idx ) {
		if ( src[idx] == ICONV_EXT_CHAR ) {	
			ptr = &src[idx+1];
			in_char = 0;
			if ( (ptr[0] >= 0xD8) && (ptr[2] >= 0xDC) ) { // 4bytes emoji
				in_char = (ptr[0] << 24) + (ptr[1] << 16) + (ptr[2] << 8) + ptr[3];
				idx += 4;
			}
			else {
				in_char = (ptr[0] << 8) + ptr[1];
				idx += 2;
			}
			// Check UCS2 -> CP949 Exception Charset
			if ( selectVerifyCharset(CHARSET_UCS2, in_char, CHARSET_CP949, &out_char) == -1 ) {
				if ( (ptr[0] >= 0xD8) && (ptr[2] >= 0xDC) ) // 4bytes emoji
				{
					dst[len++] = 0x3F;
					dst[len++] = 0x3F;
				}
				dst[len++] = 0x3F;
				dst[len++] = 0x3F;
			}else {
                unsigned char *p = (unsigned char *)&out_char;
                int i = 0;
                for ( i = 0; i < sizeof(u_int); ++i ) {
                    if (p[i] == 0x00 )
                        continue;
                    dst[len++] = p[i];
                }
			}
			continue;
		}

		// 2바이트 한글 일 경우
		if ( (src[idx] & 0x80) == 0x80 ) {
			dst[len++] = src[idx++];
		}

		dst[len++] = src[idx];
	}
	*dst_len = len;
	return len;
}

int flk_iconv_remove_ext_flag_tuple(stRDActiveTuple *spActive, unsigned char *dst)
{
	int len = 0;
	if ( spActive->dbIconvExtLen[0] > 0 ) {
		flk_iconv_remove_ext_flag(spActive->dbMessageContent, spActive->dbMessageLength[0],
				dst, &len);
	}else {
		memcpy(dst, spActive->dbMessageContent, spActive->dbMessageLength[0]);
		len = spActive->dbMessageLength[0];
	}
	return len;
}

int getIconvExtFlag(stRDActiveTuple *spActive)
{

	if ( spActive->dbInputDCS[0] != 0x08 ) {
		return ICONV_EXT_FLAG_OFF;
	}

	if ( spActive->dbIconvExtLen[0] == 0 )
	{
		return ICONV_EXT_FLAG_OFF;
	}

	return ICONV_EXT_FLAG_ON;
}
*/

int flk_iconv_cp949_to_utf8(const char *org, int org_len, char *target, int *target_len)
{
	int ret = 0;
	size_t olen = (size_t)org_len;
	size_t tlen = (size_t)*target_len;

	FLK_ICONV_TYPE oType = FLK_ICONV_TYPE_CP949, tType = FLK_ICONV_TYPE_UTF8;
	struct flk_conv_struct *cd;

	if ( org_len == 0 ) {
		return 0;
	}

	if ( org == NULL || target == NULL) {
		return -1;
	}

	if ( *target_len <= 0 )
		tlen = 512;

	cd = (flk_conv_t)flk_iconv_open( oType, tType);
	if ( cd == NULL ) {
		return -1;
	}

	ret = flk_iconv(cd, &org, olen, &target, &tlen);
	if ( ret != FLK_ICONV_ERR_INVALID )
		*target_len = (int)tlen;
	
	flk_iconv_close(cd);

	return ret;


}

int flk_iconv_utf8_to_cp949(const char *org, int org_len, char *target, int *target_len)
{
	int ret = 0;
	size_t olen = (size_t)org_len;
	size_t tlen = (size_t)*target_len;

	FLK_ICONV_TYPE oType = FLK_ICONV_TYPE_UTF8, tType = FLK_ICONV_TYPE_CP949;
	struct flk_conv_struct *cd;

	if ( org_len == 0 ) {
		return 0;
	}

	if ( org == NULL || target == NULL) {
		return -1;
	}

	if ( *target_len <= 0 )
		tlen = 512;

	cd = (flk_conv_t)flk_iconv_open( oType, tType);
	if ( cd == NULL ) {
		return -1;
	}

	ret = flk_iconv(cd, &org, olen, &target, &tlen);
	if ( ret != FLK_ICONV_ERR_INVALID )
		*target_len = tlen;
	
	flk_iconv_close(cd);

	return ret;


}

int flk_iconv_ucs2_to_utf8(const char *org, int org_len, char *target, int *target_len)
{
	int ret = 0;
	size_t olen = (size_t)org_len;
	size_t tlen = (size_t)*target_len;

	FLK_ICONV_TYPE oType = FLK_ICONV_TYPE_UCS2, tType = FLK_ICONV_TYPE_UTF8;
	struct flk_conv_struct *cd;

	if ( org_len == 0 ) {
		return 0;
	}

	if ( org == NULL || target == NULL) {
		return -1;
	}

	if ( *target_len <= 0 )
		tlen = 512;

	cd = (flk_conv_t)flk_iconv_open( oType, tType);
	if ( cd == NULL ) {
		return -1;
	}

	ret = flk_iconv(cd, &org, olen, &target, &tlen);
	if ( ret != FLK_ICONV_ERR_INVALID )
		*target_len = (int)tlen;
	
	flk_iconv_close(cd);

	return ret;


}

int flk_iconv_utf8_to_ucs2(const char *org, int org_len, char *target, int *target_len)
{
	int ret = 0;
	size_t olen = (size_t)org_len;
	size_t tlen = (size_t)*target_len;

	FLK_ICONV_TYPE oType = FLK_ICONV_TYPE_UTF8, tType = FLK_ICONV_TYPE_UCS2;
	struct flk_conv_struct *cd;

	if ( org_len == 0 ) {
		return 0;
	}

	if ( org == NULL || target == NULL) {
		return -1;
	}

	if ( *target_len <= 0 )
		tlen = 512;

	cd = (flk_conv_t)flk_iconv_open( oType, tType);
	if ( cd == NULL ) {
		return -1;
	}

	ret = flk_iconv(cd, &org, olen, &target, &tlen);
	if ( ret != FLK_ICONV_ERR_INVALID )
		*target_len = tlen;
	
	flk_iconv_close(cd);

	return ret;


}

int flk_iconv_ucs2_to_cp949(const char *org, int org_len, char *target, int *target_len)
{
	int ret = -1;
	struct flk_conv_struct *cd = NULL;

	size_t olen = (size_t) org_len;
	size_t tlen = (size_t) *target_len;

	if (org_len == 0)
	{
		return -1;
	}

	if (org == NULL || target == NULL)
	{
		return -1;
	}

	if (tlen <= 0)
	{
		tlen = 512;
	}

	cd = (flk_conv_t) flk_iconv_open(FLK_ICONV_TYPE_UCS2, FLK_ICONV_TYPE_CP949);

	if (cd == NULL)
	{
		return -1;
	}

	ret = flk_iconv(cd, &org, olen, &target, &tlen);

	if (ret != FLK_ICONV_ERR_INVALID)
	{
		*target_len = tlen;
	}
	
	flk_iconv_close(cd);

	return ret;
}
