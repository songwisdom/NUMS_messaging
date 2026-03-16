/*
 * Copyright (C) 1999-2001 Free Software Foundation, Inc.
 * This file is part of the GNU LIBICONV Library.
 *
 * The GNU LIBICONV Library is free software; you can redistribute it
 * and/or modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * The GNU LIBICONV Library is distributed in the hope that it will be
 * useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with the GNU LIBICONV Library; see the file COPYING.LIB.
 * If not, write to the Free Software Foundation, Inc., 59 Temple Place -
 * Suite 330, Boston, MA 02111-1307, USA.
 */

/*
 * CP949 is EUC-KR, extended with UHC (Unified Hangul Code).
 */

#include "uhc_1.h"
#include "uhc_2.h"

static int
flk_cp949_mbtowc (flk_conv_t conv, flk_ucs4_t *pwc, const unsigned char *s, int n)
{
	unsigned char c = *s;
	int ret;

	/*
	if ( IS_ICONV_EXT_OPTION(conv) ) {
		if  ( c == ICONV_EXT_CHAR ) {
			if ( (s[1] >= 0xD8) && (s[3] >= 0xDC) )
			{
				memcpy(pwc, &s[1], 4);
				conv->ext_length++;
				return 5;
			}
			conv->ext_length++;
			*pwc = 0;
			unsigned char *p = (unsigned char *)pwc;
			memcpy(&p[2], &s[1], 2);
			return 3;
		}
	}
	*/

	/* Code set 0 (ASCII) */
	if (c < 0x80)
		return flk_ascii_mbtowc(conv,pwc,s,n);

	/* UHC part 1 */
	if (c >= 0x81 && c <= 0xa0)
	{
		ret = flk_uhc_1_mbtowc(conv,pwc,s,n);
		if(ret !=FLK_ICONV_ERR_INVALID)
		{
			return ret;
		}
	}

	if (c >= 0xa1 && c < 0xff) 
	{
		if (n < 2) { // 남아 있는 버퍼가 2글자가 아닌 경우 1bytes ?
			*pwc = (flk_ucs4_t)0x3F;
			return 1;
		}else {
			unsigned char c2 = s[1];
			if (c2 < 0xa1) 
			{
				/* UHC part 2 */
				ret = flk_uhc_2_mbtowc(conv,pwc,s,n);
				if(ret != FLK_ICONV_ERR_INVALID)
				{
					return ret ;
				}
			}
			else if (c2 < 0xff && !(c == 0xa2 && c2 == 0xe8)) 
			{ // Add to User-Defined characters 
				/* Code set 1 (KS C 5601-1992) */
				unsigned char buf[2];
				int ret = 0;
				buf[0] = c-0x80; buf[1] = c2-0x80;
				ret = flk_ksc5601_mbtowc(conv,pwc,buf,2);
				if ( ret != FLK_ICONV_ERR_INVALID)
					return ret;
				if ( c == 0xc9) 
				{
					*pwc = 0xe000 + (c2 - 0xa1);
					return 2;
				}

				if ( c == 0xfe ) 
				{
					*pwc = 0xe05e + (c2 - 0xa1);
					return 2;
				}
			}
		}
	}

	if ( n < 2) {
		*pwc = (flk_ucs4_t)0x3F;
		return 1;
	}else {
		// *pwc = (flk_ucs4_t)0x3F;
		// return 2;
		
		unsigned int in_char = 0;
		
		in_char = (s[0] << 8) + s[1];
		/*
		if ( selectVerifyCharset(CHARSET_CP949, in_char, CHARSET_UCS2, &out_char) == FLK_FAIL ) {
			*pwc = (flk_ucs4_t) 0x3F;
		}else {
			*pwc = (flk_ucs4_t)out_char;
		}
		*/
		return 2;
	}
}

static int
flk_cp949_wctomb (flk_conv_t conv, unsigned char *r, flk_ucs4_t wc, int n)
{
	unsigned char buf[2];
	int ret;

	unsigned char e[4];
	unsigned char Emoji[10] ={0x3F, 0x3F, 0x3F, 0x3F};
	memcpy (e, &wc, 4);
	if ( n >= 4 )
	{
		if ( (e[0] >= 0xd8) && (e[2] >= 0xDC))
		{
			//DR720 HJY Emoji
			memcpy(&r[0], Emoji, 4);
			return 4;
			/*
			   r[0] = ICONV_EXT_CHAR;
			   memcpy(&r[1], &wc, 4);
			   return 5;
			 */
		}
	}

	/* Code set 0 (ASCII) */
	if ( wc < 0x80 ) {
		ret = flk_ascii_wctomb(conv,r,wc,n);
		if (ret != FLK_ICONV_ERR_INVALID)
			return ret;
	}

	/* Code set 1 (KS C 5601-1992, now KS X 1001:1998) */
	if (wc != 0x327e) {
		ret = flk_ksc5601_wctomb(conv,buf,wc,2);
		if (ret != FLK_ICONV_ERR_INVALID) {
			if (ret != 2) {
				r[0] = 0x3F;
				r[1] = 0x3F;
				return 2;
			}
			if (n < 2) {
				r[0] =0x3F;
				return 1;
			}
			r[0] = buf[0]+0x80;
			r[1] = buf[1]+0x80;
			return 2;
		}
	}

	/* UHC */
	if (wc >= 0xac00 && wc < 0xd7a4) {
		if (wc < 0xc8a5)
			ret = flk_uhc_1_wctomb(conv,r,wc,n);
		else
			ret = flk_uhc_2_wctomb(conv,r,wc,n);

		if ( ret != FLK_ICONV_ERR_INVALID )
			return ret;
	}

	/* User-defined characters */
	if (wc >= 0xe000 && wc < 0xe0bc) {
		if (n < 2) {
			r[0] = 0x3F;
			return 1;
		}
		if (wc < 0xe05e) {
			r[0] = 0xc9;
			r[1] = wc - 0xe000 + 0xa1;
		} else {
			r[0] = 0xfe;
			r[1] = wc - 0xe05e + 0xa1;
		}
		return 2;
	}

	conv->conv_tb_match_failed++;
	if ( n < 2 ) {
		r[0] = 0x3F;
		return 1;
	}
	if ( IS_ICONV_EXT_OPTION(conv) ) {
		unsigned char *p = (unsigned char *)&wc;
		conv->ext_length++;
		r[0] = ICONV_EXT_CHAR;
		r[1] = p[2];
		r[2] = p[3];
		return 3;
	}else {
		r[0] = 0x3F; // Set [?]
		r[1] = 0x3F; // Set [?]
		return 2;
	}
#if 0
	unsigned char buf[2];
	int ret;

	/*
	   u_char e[4];
	   memcpy (e, &wc, 4);
	   if ( n >= 4 ) 
	   {
	   if ( (e[0] >= 0xd8) && (e[2] >= 0xDC))
	   {
	   r[0] = ICONV_EXT_CHAR;
	   memcpy(&r[1], &wc, 4);
	   return 5;
	   }
	   }
	 */

	/* Code set 0 (ASCII) */
	if ( wc < 0x80 ) {
		ret = flk_ascii_wctomb(conv,r,wc,n);
		if (ret != FLK_ICONV_ERR_INVALID)
			return ret;
	}

	/* Code set 1 (KS C 5601-1992, now KS X 1001:1998) */
	if (wc != 0x327e) {
		ret = flk_ksc5601_wctomb(conv,buf,wc,2);
		if (ret != FLK_ICONV_ERR_INVALID) {
			if (ret != 2) {
				r[0] = 0x3F;
				r[1] = 0x3F;
				return 2;
			}
			if (n < 2) {
				r[0] =0x3F;
				return 1;
			}
			r[0] = buf[0]+0x80;
			r[1] = buf[1]+0x80;
			return 2;
		}
	}

	/* UHC */
	if (wc >= 0xac00 && wc < 0xd7a4) {
		if (wc < 0xc8a5)
			ret = flk_uhc_1_wctomb(conv,r,wc,n);
		else
			ret = flk_uhc_2_wctomb(conv,r,wc,n);

		if ( ret != FLK_ICONV_ERR_INVALID )
			return ret;
	}

	/* User-defined characters */
	if (wc >= 0xe000 && wc < 0xe0bc) {
		if (n < 2) {
			r[0] = 0x3F;
			return 1;
		}
		if (wc < 0xe05e) {
			r[0] = 0xc9;
			r[1] = wc - 0xe000 + 0xa1;
		} else {
			r[0] = 0xfe;
			r[1] = wc - 0xe05e + 0xa1;
		}
		return 2;
	}

	conv->conv_tb_match_failed++;
	if ( n < 2 ) {
		r[0] = 0x3F;
		return 1;
	}

	/*
	   if ( IS_ICONV_EXT_OPTION(conv) ) {
	   unsigned char *p = (unsigned char *)&wc;
	   conv->ext_length++;
	   r[0] = ICONV_EXT_CHAR;
	   r[1] = p[2];
	   r[2] = p[3];
	   return 3;
	   }else {
	 */
r[0] = 0x3F; // Set [?]
r[1] = 0x3F; // Set [?]
return 2;
/*
   }
 */
#endif
}

