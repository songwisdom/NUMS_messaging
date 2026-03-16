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
 * ASCII
 */

#ifndef _H_FLK_ASCII_
#define _H_FLK_ASCII_

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
 * ASCII
 */

static int flk_ascii_mbtowc (flk_conv_t conv, flk_ucs4_t *pwc, const unsigned char *s, int n)
{
	unsigned char c = *s;
	if (c < 0x80) {
		*pwc = (flk_ucs4_t) c;
		return 1;
	}else { // DR420
		*pwc = (flk_ucs4_t) 0x3F;
	}
	return 1;
}

static int flk_ascii_wctomb (flk_conv_t conv, unsigned char *r, flk_ucs4_t wc, int n)
{
	if (wc < 0x0080) {
		*r = wc;
	}else { // DR420 
		*r = 0x3F;
	}
	return 1;
}
#endif
