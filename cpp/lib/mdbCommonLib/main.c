/**
 * =====================================================================================
 *
 *       @file  main.c
 *
 *    	 @brief
 *
 *       Version:  1.0
 *       Compiler:  cc (HP-UX)
 *       @date:  03/17/10 13:42:05
 *
 *       @author:  SMS Dev (), sms_dev@feelingk.com
 *       Company:  (c) FEELingK
 *       @attention
 *       Copyright (c) 2003 FEELINGK Co., Ltd.
 *
 *         All rights reserved.
 *
 *     This program contains confidential and proprietary information of
 *     FEELINGK, and any reproduction, disclosure, or use in whole or in part
 *     by prior written agreement or permission of FEELINGK.
 *
 *                            FEELINGK Co., Ltd.
 *     3F Ace Techno Twin Tower 1, 212-1 Kuro-Dong, Kuro-Gu, Seoul 152-050 Korea
 *
 * =====================================================================================
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

typedef char char;
typedef unsigned char u_char;
typedef int int;

#define DIRECTORY_SIZE              256
#define SEGMENT_SIZE                1024

#define PRIME_NEW_KEY 				37
#define PRIME_BURKET_SIZE 			(DIRECTORY_SIZE * SEGMENT_SIZE)

unsigned dma_hash1( char *key, int len )
{

	register unsigned h = 0, l = len;
	register u_char *k = (u_char *)key;

	/* Convert string to integer */

	while ( l-- ) {
		h = h * PRIME_NEW_KEY ^ ((*k & 0x0F)  - 0x10);
		h = h * PRIME_NEW_KEY ^ ((*k & 0xF0)  - 0x01);
		k++;
	}

	h %= PRIME_BURKET_SIZE;

	return h;

}

void hash_check(unsigned int val)
{
	char key[24] = {0x00,};



	memcpy ( key, (void *)&val, (size_t)4);

	int hash_val = dma_hash1(key, 4);
	int x = 0, y = 0;

	printf("hash_val [%d]\n", hash_val);
	x = hash_val / SEGMENT_SIZE;
	y = hash_val % SEGMENT_SIZE;

	printf("Result [%d][%d]\n", x, y);
	return;
}

int main (int argc, char **argv)
{
	unsigned int asrv = 1394036199; // sms_schdASRV
	unsigned int expd = 683979183;
	unsigned int ase = 1430124645;


	printf("sms_schdASRV Check\n");
	hash_check(asrv);
	puts("-------------------------------------------\n\n\n\n");

	printf("sms_schdEXPT Check\n");
	hash_check(expd);
	puts("-------------------------------------------\n\n\n\n");

	printf("sms_ase Check\n");
	hash_check(ase);
	puts("-------------------------------------------\n\n\n\n");
	return 0;

}
