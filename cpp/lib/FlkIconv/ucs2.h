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
 * UCS-2
 */

/* Here we accept FFFE/FEFF marks as endianness indicators everywhere
   in the stream, not just at the beginning. The default is big-endian. */
/* The state is 0 if big-endian, 1 if little-endian. */
#ifndef _H_FLK_UCS2_
#define _H_FLK_UCS2_
/* Here we accept FFFE/FEFF marks as endianness indicators everywhere
   in the stream, not just at the beginning. The default is big-endian. */
/* The state is 0 if big-endian, 1 if little-endian. */
static int
flk_ucs2_mbtowc (flk_conv_t conv, flk_ucs4_t *pwc, const unsigned char *s, int n)
{
	flk_state_t state = conv->istate;
	int count = 0;
	const unsigned char *org = s;
	for (; n >= 2;)
	{
		flk_ucs4_t wc = (state ? s[0] + (s[1] << 8) : (s[0] << 8) + s[1]);
		s += 2; n -= 2; count += 2;
		if (wc == 0xfeff) {
		} else if (wc == 0xfffe) {
			state ^= 1;
		} else if (wc >= 0xd800 && wc < 0xe000) {
			/* DR420 */
			conv->istate = state;
			/* FR320 */
			if ( n >= 2 ) {
				// Emoji Data
				if ( (wc >= 0xd800) && (s[0] >= 0xDC) )
				{
					//DR720 HJY Emoji
              memcpy(pwc, org, 4);
              return 4;
              /*
              conv->ext_length++;
              memcpy(pwc, org, 4);
              return 4;
              */
          }
      }
      *pwc = (flk_ucs4_t)0x3F;
      return 2;
    } else {
      *pwc = wc;
      conv->istate = state;
      return count;
    }
	}

  *pwc = (flk_ucs4_t)0x3F;
  conv->istate = state;
  if ( n < 2) {
      return 1;
  }else {
      return 2;
  }
#if 0
	flk_state_t state = conv->istate;
	int count = 0;
	const unsigned char *org = s;
	for (; n >= 2;) 
	{
		flk_ucs4_t wc = (state ? s[0] + (s[1] << 8) : (s[0] << 8) + s[1]);
		s += 2; n -= 2; count += 2;
		if (wc == 0xfeff) {
		} else if (wc == 0xfffe) {
			state ^= 1;
		} else if (wc >= 0xd800 && wc < 0xe000) {
			/* DR420 */
			conv->istate = state;
			if ( n >= 2 ) {
				// Emoji Data
				if ( (wc >= 0xd800) && (s[0] >= 0xDC) ) 
				{
					conv->ext_length++;
					memcpy(pwc, org, 4);
					printf("emoji char [%02X][%02X][%02X][%02X]\n",
							pwc[0],
							pwc[1],
							pwc[2],
							pwc[3]);
					return 4;
				}
			}
			*pwc = (flk_ucs4_t)0x3F;
			return 2;
		} else {
			*pwc = wc;
			conv->istate = state;
			return count;
		}

	}

	*pwc = (flk_ucs4_t)0x3F;
	conv->istate = state;
	if ( n < 2) {
		return 1;
	}else {
		return 2;
	}
#endif
}

/* But we output UCS-2 in big-endian order, without byte-order mark. */
/* RFC 2152 says:
   "ISO/IEC 10646-1:1993(E) specifies that when characters the UCS-2 form are
    serialized as octets, that the most significant octet appear first." */
static int
flk_ucs2_wctomb (flk_conv_t conv, unsigned char *r, flk_ucs4_t wc, int n)
{
  if (wc < 0x10000 && wc != 0xfffe && !(wc >= 0xd800 && wc < 0xe000)) {
    if (n >= 2) {
      r[0] = (unsigned char) (wc >> 8);
      r[1] = (unsigned char) wc;
      return 2;
	 
    }/* else
      return FLK_ICONV_ERR_FEW;
	  */
  }/* else
    return FLK_ICONV_ERR_INVALID;
	*/
  conv->conv_tb_match_failed++;

  if ( n < 2 ) {
	  r[0] = 0x3F;
	  return 1;
  }else {
	  r[0] = 0x00;
	  r[1] = 0x3F;
	  return 2;
  }
}
#endif
