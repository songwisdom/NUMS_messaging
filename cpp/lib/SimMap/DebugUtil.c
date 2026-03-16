/**
 * =====================================================================================
 *
 *       @file  DebugUtil.c
 *
 *    	 @brief 
 *
 *       Version:  1.0
 *       Compiler:  cc (HP-UX)
 *       @date:  04/27/09 23:32:08
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

#include "flk_type.h"

void DebugDecodeFowardSM (stMsgBuf_t *spMsg)
{
	int idx = 0;
	int i = 0;
	int max = 0;
	u_char *Ptr = spMsg.cpData;
	printf("Tag        [%02X] : MO FowardSM\n", *Ptr++ );
	printf("Tag Length [%02X] \n", *Ptr++ );
	puts("------------------------------------------------------");
	puts("RP DA");
	printf("\tTag     [%02X]\n", *Ptr++);
	max = *Ptr -1;
	printf("\tLength  [%02X]\n", *Ptr++);
	printf("\tToA     [%02X]\n", *Ptr++);
	printf("\tRP_DA   [", *Ptr++);
	for (i =0; i<max; ++i) 
		printf("[%02X]", *Ptr++);
	printf("]\n");
	puts("------------------------------------------------------");
	puts("MSUSDN");
	printf("\tTag     [%02X]\n", *Ptr++);
	max = *Ptr -1;
	printf("\tLength  [%02X]\n", *Ptr++);
	printf("\tToA     [%02X]\n", *Ptr++);
	printf("\tRP_DA   [", *Ptr++);
	for (i =0; i<max; ++i) 
		printf("[%02X]", *Ptr++);
	printf("]\n");
	puts("------------------------------------------------------");
	printf("\tHeader [%02X]\n", *Ptr++);
	printf("\tTP-MR  [%02X]\n", *Ptr++);

}
