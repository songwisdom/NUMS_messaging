#include "flk_type.h"
#include <stdio.h>
#include <string.h>
#include "libMapAddrUtil.h"

/** 
  * =====================================================================================
  * @brief 문자열이 전화번호인지 확인한다.
  * 
  * @param str 문자열
  * 
  * @return TRUE FALSE 
  * 
  * @author C.H.Lee
  * @date 2009-04-26
  * =====================================================================================
  */
bool isAddressString (u_char *str)
{
    bool ret = true;

    for ( ;ret && *str != '\0'; ++str)
		ret = (IS_ASCII_STRING(*str));

    return ret;
}

int getBCDLength ( u_char *bcd, int max_oct )
{
	u_char *ptr= NULL;

	for ( ptr = bcd; *ptr != 0xFF && max_oct--;  ++ptr);
		/* nothing */
	
	return (int)GET_LENGTH_OCT_TO_BCD((ptr-bcd));
}

int getMapBCDLength ( u_char *src, int max_oct )
{
	u_char *ptr= src;

	for ( ; *ptr != 0xFF && max_oct--;  ++ptr);
		/* nothing */
	
	return (int)GET_LENGTH_OCT_TO_MAPBCD((ptr-src));
}

int convBCD2Str( u_char *dest, int dest_size, u_char *src, int src_size )
{
	int  idx = 0;
	int  max_size = 0;
	u_char  tmp = true;

	bzero(dest, dest_size);
	max_size = getBCDLength(src, src_size);
	max_size = GET_LOW_NUM(max_size, dest_size);

	for ( idx = 0; tmp && idx < max_size; ++idx ) {
		if ( idx % 2 == 0 ) {
			tmp = GET_MOVE_LSB2MSB(*src);
		}
		else  {
			tmp = GET_MSB_4BIT(*src);
			++src;
		}

		tmp = CONV_DIGIT2CHAR(tmp);
		*dest++ = tmp;
	}

    return idx;
}

int convStr2BCD( u_char *dest, int dest_size, u_char *src, int src_size )
{
	u_char  *target = dest;
	SemiOctet *Oct = NULL;
	int  max_size = GET_LENGTH_BCD_TO_OCT(dest_size);

	max_size = GET_LOW_NUM(max_size, src_size);
	memset(dest, DIGIT_NULL, dest_size);

	for ( ; *src != '\0' && dest_size--; ++target ) {
		Oct = (SemiOctet *)target;
		Oct->digit2nd = (CONV_CHAR2DIGIT(*src)); ++src;	
		Oct->digit1st = (CONV_CHAR2DIGIT(*src)); ++src;

		if ( Oct->digit == 0xFF)
			break;
	}
    return (int)(GET_LENGTH_OCT_TO_BCD((target - dest)));
}


int convBCD2Cdma ( u_char *dest, int dest_size, u_char *src, int src_size )
{
	u_char *ptr = NULL;
	int max_len = GET_LOW_NUM(dest_size, src_size);
	for ( ptr = src ; *ptr != 0xFF && max_len--; ++ptr, ++dest) 
		*dest = (CONV_OCT_DIGIT2CDMA(*ptr));

	return (int)GET_LENGTH_OCT_TO_BCD(ptr - src);
}

int convCdma2BCD( u_char *dest, int dest_size, u_char *src, int src_size )
{
	u_char *ptr = NULL;
	int max_len = GET_LOW_NUM(dest_size, src_size);
	for ( ptr = src ; *ptr != 0xFF && max_len--; ++ptr, ++dest) 
		*dest = (CONV_OCT_CDMA2DIGIT(*ptr));

	return (int)GET_LENGTH_OCT_TO_MAPBCD(ptr - src);
}

int convBCD2Gsm( u_char *dest, int dest_size, u_char *src, int src_size )
{
	u_char *ptr = NULL;
	int max_len = GET_LOW_NUM(dest_size, src_size);
	for ( ptr = src ; *ptr != 0xFF && max_len--; ++ptr, ++dest) 
		*dest = (CONV_OCT_DIGIT2GSM(*ptr));

	return (int)GET_LENGTH_OCT_TO_BCD(ptr - src);
}

int convGsm2BCD( u_char *dest, int dest_size, u_char *src, int src_size )
{
	u_char *ptr = NULL;
	int max_len = GET_LOW_NUM(dest_size, src_size);
	for ( ptr = src ; *ptr != 0xFF && max_len--; ++ptr, ++dest) 
		*dest = (CONV_OCT_GSM2DIGIT(*ptr));

	return (int)GET_LENGTH_OCT_TO_MAPBCD(ptr - src);
}
int convBCD2CB ( u_char *dest, int dest_size, u_char *src, int src_size )
{
	u_char *ptr = NULL;
	int max_len = GET_LOW_NUM(dest_size, src_size);
	for ( ptr = src ; *ptr != 0xFF && max_len--; ++ptr, ++dest) 
		*dest = (CONV_OCT_DIGIT2CB(*ptr));

	return (int)GET_LENGTH_OCT_TO_BCD(ptr - src);
}
int convCB2BCD ( u_char *dest, int dest_size, u_char *src, int src_size )
{
	u_char *ptr = NULL;
	int max_len = GET_LOW_NUM(dest_size, src_size);
	for ( ptr = src ; *ptr != 0xFF && max_len--; ++ptr, ++dest) 
		*dest = (CONV_OCT_CB2DIGIT(*ptr));

	return (int)GET_LENGTH_OCT_TO_MAPBCD(ptr - src);
}


