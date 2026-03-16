#include 	<stdio.h>
#include 	<string.h>
#include 	<unistd.h>
#include 	<stdlib.h>
#include 	<errno.h>

#include 	"sim_iconv.h"

#include 	"ascii.h"
#include 	"ksc5601.h"
#include 	"ucs2.h"
#include 	"euc_kr.h"
#include 	"cp949.h"
#include 	"utf8.h"

extern int errno;

static const struct flk_iconv_encoding flk_iconv_encodings[ FLK_ICONV_TYPE_MAX ] = {
	{ { flk_ucs2_mbtowc, NULL}, {flk_ucs2_wctomb, NULL} },
	{ { flk_euc_kr_mbtowc, NULL}, {flk_euc_kr_wctomb, NULL} },
	{ { flk_cp949_mbtowc, NULL}, {flk_cp949_wctomb, NULL} },
	{ { flk_utf8_mbtowc, NULL}, {flk_utf8_wctomb, NULL} }
};

static size_t unicode_loop_convert (flk_iconv_t icd,
		const char* * inbuf, size_t *inbytesleft,
		char* * outbuf, size_t *outbytesleft)
{
	flk_conv_t cd = (flk_conv_t) icd;
	size_t result = 0;
	const unsigned char* inptr = (const unsigned char*) *inbuf;
	size_t inleft = *inbytesleft;
	unsigned char* outptr = (unsigned char*) *outbuf;
	size_t outleft = *outbytesleft;
	while (inleft > 0) {

		flk_ucs4_t wc;
		int incount;
		int outcount;
		incount = cd->ifuncs.xxx_mbtowc(cd,&wc,inptr,inleft);
		if (incount < 0) {
			/* DR420 ICONV 실패서 물음표 처리 */
			/* 단 ASCII 오류 시에는 Callback Function 안에서 처리 함으로 
			 * 여기에서는 2바이트 유입 문자가 오류라 판단한다 */
			/* 단, inleft Size가 2 Bytes 미만 인 경우 1Bytes 처리 */
			wc = (flk_ucs4_t)0x3F;
			if ( inleft < 2 )
				incount = 1;
			else
				incount = 2;
		} 
		/* Case 4: k bytes read, making up a wide character */
		if (outleft == 0) {
			break;
		}
		else if (incount == 5) {
			outcount = 4;
			memcpy(outptr, &wc, 4);
			goto outcount_ok;
		}

		outcount = cd->ofuncs.xxx_wctomb(cd,outptr,wc,outleft);
		if (outcount != FLK_ICONV_ERR_INVALID)
			goto outcount_ok;
		/* Handle Unicode tag characters (range U+E0000..U+E007F). */
		if ((wc >> 7) == (0xe0000 >> 7))
			goto outcount_zero;

		result++;

		outcount = cd->ofuncs.xxx_wctomb(cd,outptr,0xFFFD,outleft);
		if (outcount != FLK_ICONV_ERR_INVALID)
			goto outcount_ok;

		if ( cd->outconv == FLK_ICONV_TYPE_UCS2 ) {
			outptr[0] = 0x00;
			outptr[1] = 0x3F;
			outcount = 2;
		}else {
			outptr[0] = 0x3F;
			outcount = 1;
		}
outcount_ok:
		if (outcount < 0) {
			break;
		}
		// DR420 
		if ( outcount > outleft ) {
			break;
		}
		outptr += outcount; 
		outleft -= outcount;
outcount_zero:
		if ( incount > inleft ) {
			break;
		}
		inptr += incount; inleft -= incount;
	}
	*inbuf = (const char*) inptr;
	*inbytesleft = inleft;
	*outbuf = (char*) outptr;
	*outbytesleft = outleft;
	return result;
}

#if 0 
//orginal
static size_t unicode_loop_convert (flk_iconv_t icd,
		const char* * inbuf, size_t *inbytesleft,
		char* * outbuf, size_t *outbytesleft)
{
	flk_conv_t cd = (flk_conv_t) icd;
	size_t result = 0;
	const unsigned char* inptr = (const unsigned char*) *inbuf;
	size_t inleft = *inbytesleft;
	unsigned char* outptr = (unsigned char*) *outbuf;
	size_t outleft = *outbytesleft;
	int xyz = 0;
	while (inleft > 0) {

		flk_state_t last_istate = cd->istate;
		flk_ucs4_t wc;
		int incount;
		int outcount;
		incount = cd->ifuncs.xxx_mbtowc(cd,&wc,inptr,inleft);
		if (incount < 0) {
			if (incount == FLK_ICONV_ERR_INVALID) {
				errno = EILSEQ;
				result = -1;
				break;
			}
			if (incount == FLK_ICONV_RET_TOOFEW(0)) {
				/* Case 2: not enough bytes available to detect anything */
				errno = EINVAL;
				result = -1;
				break;
			}
			/* Case 3: k bytes read, but only a shift sequence */
			incount = -2-incount;
		} else {
			/* Case 4: k bytes read, making up a wide character */
			if (outleft == 0) {
				cd->istate = last_istate;
				errno = E2BIG;
				result = -1;
				break;
			}
			outcount = cd->ofuncs.xxx_wctomb(cd,outptr,wc,outleft);
			if (outcount != FLK_ICONV_ERR_INVALID)
				goto outcount_ok;
			/* Handle Unicode tag characters (range U+E0000..U+E007F). */
			if ((wc >> 7) == (0xe0000 >> 7))
				goto outcount_zero;

			result++;

			outcount = cd->ofuncs.xxx_wctomb(cd,outptr,0xFFFD,outleft);
			if (outcount != FLK_ICONV_ERR_INVALID)
				goto outcount_ok;
			cd->istate = last_istate;
			errno = EILSEQ;
			result = -1;
			break;
outcount_ok:
			if (outcount < 0) {
				cd->istate = last_istate;
				errno = E2BIG;
				result = -1;
				break;
			}
			if (!(outcount <= outleft)) abort();
			outptr += outcount; outleft -= outcount;
		}
outcount_zero:
		if (!(incount <= inleft)) abort();
		inptr += incount; inleft -= incount;
	}
	*inbuf = (const char*) inptr;
	*inbytesleft = inleft;
	*outbuf = (char*) outptr;
	*outbytesleft = outleft;
	return result;
}
#endif

static size_t unicode_loop_reset (flk_iconv_t icd,
		char* * outbuf, size_t *outbytesleft)
{
	flk_conv_t cd = (flk_conv_t) icd;
	if (outbuf == NULL || *outbuf == NULL) {
		/* Reset the states. */
		memset(&cd->istate,'\0',sizeof(flk_state_t));
		memset(&cd->ostate,'\0',sizeof(flk_state_t));
		return 0;
	} else {
		size_t result = 0;
		if (cd->ifuncs.xxx_flushwc) {
			flk_state_t last_istate = cd->istate;
			flk_ucs4_t wc;
			if (cd->ifuncs.xxx_flushwc(cd, &wc)) {
				unsigned char* outptr = (unsigned char*) *outbuf;
				size_t outleft = *outbytesleft;
				int outcount = cd->ofuncs.xxx_wctomb(cd,outptr,wc,outleft);
				if (outcount != FLK_ICONV_ERR_INVALID)
					goto outcount_ok;
				/* Handle Unicode tag characters (range U+E0000..U+E007F). */
				if ((wc >> 7) == (0xe0000 >> 7))
					goto outcount_zero;
				/* Try transliteration. */
				result++;

				outcount = cd->ofuncs.xxx_wctomb(cd,outptr,0xFFFD,outleft);
				if (outcount != FLK_ICONV_ERR_INVALID)
					goto outcount_ok;
				cd->istate = last_istate;
				errno = EILSEQ;
				return -1;
outcount_ok:
				if (outcount < 0) {
					cd->istate = last_istate;
					errno = E2BIG;
					return -1;
				}
				if (!(outcount <= outleft)) abort();
				outptr += outcount;
				outleft -= outcount;
outcount_zero:
				*outbuf = (char*) outptr;
				*outbytesleft = outleft;
			}
		}
		if (cd->ofuncs.xxx_reset) {
			unsigned char* outptr = (unsigned char*) *outbuf;
			size_t outleft = *outbytesleft;
			int outcount = cd->ofuncs.xxx_reset(cd,outptr,outleft);
			if (outcount < 0) {
				errno = E2BIG;
				return -1;
			}
			if (!(outcount <= outleft)) abort();
			*outbuf = (char*) (outptr + outcount);
			*outbytesleft = outleft - outcount;
		}
		memset(&cd->istate,'\0',sizeof(flk_state_t));
		memset(&cd->ostate,'\0',sizeof(flk_state_t));
		return result;
	}
}

flk_conv_t flk_iconv_open ( FLK_ICONV_TYPE from, FLK_ICONV_TYPE to )
{
	struct flk_conv_struct *cd;

	if ( from == to )
		return NULL;

	if ( (FLK_ICONV_TYPE_UCS2 > from && from >= FLK_ICONV_TYPE_MAX ) ||
			( FLK_ICONV_TYPE_UCS2 > to && to >= FLK_ICONV_TYPE_MAX ) )
		return NULL;

	cd = (struct flk_conv_struct *) malloc ( sizeof(struct flk_conv_struct) );
	if ( cd == NULL )
		return NULL;

	memset ( cd, 0x0, sizeof(struct flk_conv_struct) );

	cd->inconv  = from;
	cd->outconv = to;

	/* set loop_funcs */
	cd->lfuncs.loop_convert = unicode_loop_convert;
	cd->lfuncs.loop_reset = unicode_loop_reset;

	/* set input info */
	cd->ifuncs = flk_iconv_encodings[from].ifuncs;

	/* set output info */
	cd->ofuncs = flk_iconv_encodings[to].ofuncs;

	/* Initialize the states. */
	memset(&cd->istate,'\0',sizeof(flk_state_t));
	memset(&cd->ostate,'\0',sizeof(flk_state_t));
	cd->conv_tb_match_failed = 0;
	return (flk_conv_t)cd;
}

size_t flk_iconv (flk_conv_t cd, const char* * inbuf, size_t inbytesleft, char* * outbuf, size_t *outbytesleft)
{
	size_t ret = FLK_ICONV_ERR_INVALID;
	size_t nInBytes = inbytesleft;
	size_t nOutBytes = *outbytesleft;

	if ( cd == NULL )
		return ret;

	if ( inbuf == NULL || *inbuf == NULL || 
			outbuf == NULL || *outbuf == NULL ||
			inbytesleft <= 0 || *outbytesleft <= 0 ) {
		ret = FLK_ICONV_ERR_INVALID;
	}
	else {
		ret = cd->lfuncs.loop_convert(cd, 
				(const char* *)inbuf, &nInBytes,
				outbuf, &nOutBytes);

		if ( ret != FLK_ICONV_ERR_INVALID ) {
			if (cd->conv_tb_match_failed ) 
				ret = FLK_ICONV_WARN_TB_NOT_MATCH;
			*outbytesleft -= nOutBytes;
		}

	}

	return ret;
}
void flk_iconv_close ( flk_conv_t cd )
{
	if (cd != NULL )
		free (cd);
	return;
}

size_t convert_ucs2_to_ksc5601 (  const char **inbuf, size_t inbytesleft,
		char **outbuf, size_t *outbytesleft )
{
	struct flk_conv_struct *cd;
	size_t ret = 0;

	cd = flk_iconv_open ( FLK_ICONV_TYPE_UCS2, FLK_ICONV_TYPE_EUCKR );
	if ( cd == NULL )
		return -1;

	ret = flk_iconv ( cd, inbuf, inbytesleft, outbuf, outbytesleft );
	flk_iconv_close (cd);
	return ret;
}

size_t convert_ksc5601_to_ucs2 ( const char **inbuf, size_t inbytesleft,
		char **outbuf, size_t *outbytesleft )

{
	struct flk_conv_struct *cd;
	size_t ret = 0;

	cd = flk_iconv_open ( FLK_ICONV_TYPE_EUCKR, FLK_ICONV_TYPE_UCS2 );
	if ( cd == NULL )
		return -1;

	ret = flk_iconv ( cd, inbuf, inbytesleft, outbuf, outbytesleft );
	flk_iconv_close (cd);
	return ret;
}

size_t convert_ucs2_to_cp949 ( const char **inbuf, size_t inbytesleft,
		char **outbuf, size_t *outbytesleft )

{
	struct flk_conv_struct *cd;
	size_t ret = 0;

	cd = flk_iconv_open ( FLK_ICONV_TYPE_UCS2, FLK_ICONV_TYPE_CP949 );
	if ( cd == NULL )
		return -1;

	ret = flk_iconv ( cd, inbuf, inbytesleft, outbuf, outbytesleft );
	flk_iconv_close (cd);
	return ret;
}

static flk_iconv_type_name_st flk_iconv_type_name [ FLK_ICONV_TYPE_MAX ] = {
	{ FLK_ICONV_TYPE_UCS2, FLK_ICONV_UCS2 },
	{ FLK_ICONV_TYPE_EUCKR, FLK_ICONV_EUCKR },
	{ FLK_ICONV_TYPE_CP949, FLK_ICONV_CP949 },
	{ FLK_ICONV_TYPE_UTF8, FLK_ICONV_UTF8}
};

FLK_ICONV_TYPE get_flk_iconv_type ( const char *cpType )
{
	int idx = 0;

	if ( cpType == NULL )
		return (FLK_ICONV_TYPE)FLK_ICONV_ERR_INVALID;

	for ( idx = 0; idx < FLK_ICONV_TYPE_MAX; ++idx ) {
		if ( strcmp ( cpType, flk_iconv_type_name[idx].name ) == 0 )
			return (FLK_ICONV_TYPE)flk_iconv_type_name[idx].type;
	}

	return (FLK_ICONV_TYPE)FLK_ICONV_ERR_INVALID;
}

/*
   const char UCS2_80[] =
   {
   0xac,0x00,0xb0,0x98,0xb2,0xe4,0xb7,0x7c,0xb9,0xc8, // UD
   0xbc,0x14,0xc0,0xac,0xc5,0x44,0xc7,0x90,0xcc,0x28,
   0xce,0x74,0xd0,0xc0,0xd3,0x0c,0xd5,0x58,0xac,0x04,
   0xb0,0x9c,0xb2,0xe8,0xb7,0x80,0xb9,0xcc,0xbc,0x18,
   0xc0,0xb0,0xc5,0x48,0xc7,0x94,0xcc,0x2c,0xce,0x78,
   0xd0,0xc4,0xd3,0x10,0xd5,0x5c,0x00,0x31,0x00,0x32,
   0x00,0x33,0x00,0x34,0x00,0x35,0x00,0x36,0x00,0x37,
   0x00,0x38,0x00,0x30,0x00,0x41,0x00,0x42,0x00,0x43
   };

   char UCS_ARR[2][5] = {
   {0xa4, 0xa7, 0xa4, 0xa7, 0x0},
   {0xc7, 0xcf, 0xc0, 0xcc, 0x0}
   };

   int main ( int argc, char **argv )
   {
   size_t ret = 0;
   char buf[1024] = {0x0, };
   const char *in = UCS2_80;
   char *out = buf;
   size_t length = sizeof(buf);
   size_t from_length = sizeof(UCS2_80);
   int i;


//ret = convert_ucs2_to_ksc5601 ( &in, &from_length, &out, &length);
ret = convert_ucs2_to_ksc5601 ( &in, &from_length, &out, &length);
ConvertContents( "UCS2", in, from_length,  "EUC-KR",out, &length );

printf("return [%d](%d)\n", ret, length);

printf("Result [%s]\n", UCS2_80, buf);

in = UCS_ARR[0];
from_length = 4;

memset ( buf, 0x0, sizeof(buf));
out = buf;
length = sizeof(buf);

if( convert_ucs2_to_ksc5601( &in, &from_length, &out, &length) != 0) {
puts("ConvertContents Failed");
}
printf("Convert From [%s] \n\t\t==> To[%s]\n", UCS2_80, buf);



in = UCS_ARR[1];
from_length = 4;

memset ( buf, 0x0, sizeof(buf));
out = buf;
length = sizeof(buf);

if( convert_ucs2_to_ksc5601( &in, &from_length, &out, &length) != 0) {
puts("ConvertContents Failed");
}
printf("Convert From [%s] \n\t\t==> To[%s]\n", UCS2_80, buf);
return 0;
}

 */
