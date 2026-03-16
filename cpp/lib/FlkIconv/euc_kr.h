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
 * EUC-KR
 */

/* Specification: RFC 1557 */

#ifndef _H_FLK_EUC_KR_
#define _H_FLK_EUC_KR_

static int
flk_euc_kr_mbtowc (flk_conv_t conv, flk_ucs4_t *pwc, const unsigned char *s, int n)
{
  unsigned char c = *s;
  /* Code set 0 (ASCII or KS C 5636-1993) */
  if (c < 0x80)
    return flk_ascii_mbtowc(conv,pwc,s,n);
  /* Code set 1 (KS C 5601-1992) */
  if (c >= 0xa1 && c < 0xff) {
    if (n < 2) {
		*pwc = (flk_ucs4_t)0x3F;
	}else {
      unsigned char c2 = s[1];
      if (c2 >= 0xa1 && c2 < 0xff) {
        unsigned char buf[2];
        buf[0] = c-0x80; buf[1] = c2-0x80;
        return flk_ksc5601_mbtowc(conv,pwc,buf,2);
      }
    }
  }
  *pwc = (flk_ucs4_t)0x3F;
  return 2;
}

static int
flk_euc_kr_wctomb (flk_conv_t conv, unsigned char *r, flk_ucs4_t wc, int n)
{
  unsigned char buf[2];
  int ret;

  /* Code set 0 (ASCII or KS C 5636-1993) */
  if ( wc < 0x80 ) {
	  ret = flk_ascii_wctomb(conv,r,wc,n);
	  if (ret != FLK_ICONV_ERR_INVALID)
		  return ret;
  }

  /* Code set 1 (KS C 5601-1992) */
  ret = flk_ksc5601_wctomb(conv,buf,wc,2);
  if (ret != FLK_ICONV_ERR_INVALID) {
    if (ret != 2) {
		r[0] = 0x3F;
		r[1] = 0x3F;
		return 2;
	}
    if (n < 2) {
		r[0] = 0x3F;
		return 1;
	}
    r[0] = buf[0]+0x80;
    r[1] = buf[1]+0x80;
    return 2;
  }

  conv->conv_tb_match_failed++;
  r[0] = 0x3F; // char ?
  r[1] = 0x3F; // char ?
  return 2;

}
#endif
