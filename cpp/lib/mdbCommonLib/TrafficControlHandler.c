/*******************************************************************************
*
*     Copyright (c) 2001 FEELINGK Co., Ltd.
*     All rights reserved.
*
*     This program contains confidential and proprietary information of
*     FEELINGK, and any reproduction, disclosure, or use in whole or in part
*     by prior written agreement or permission of FEELINGK.
*
*                            FEELINGK Co., Ltd.
*         5F Roy B/D 90-6 Nonhyun-dong Kangnam-Gu, Seoul, 135-010 Korea
*
******************************************************************************

*******************************************************************************
*   FILE NAME       : TrafficControlHandler.c
*   PURPOSE         :
*   DATE OF WRITE   :
*   AUTHOR          :
*   REVISION        :   V 1.0
*   Ver.    By      When        Reason
*   ------- ------- ----------  -----------------------------------------
******************************************************************************/
#include<FLKBaseDef.h>
#include<MdbTableHandler.h>

/*---------------------------------------------------------------------
 *  FUNCTION NAME   :   dma_getTrafficControlTuplePtr()
 *  PURPOSE         :	get pointer of traffic control tuple
 *  INPUT ARGUMENTS :
 *  RETURN VALUE    :
 *  CALLED FROM     :
 *  USED GLOBAL VARIABLES :
 *  REVISION        :
 *      BY              WHEN        REASON
 *      --------------  ----------  -------------------
----------------------------------------------------------------------*/
int dma_getTrafficControlTuplePtr(u_char *cpControlCode,
                                  stRDTrafficControlTuple **spTuple)
{
	return dma_getTuplePtr(DBMS_PLUS, R_DTRAFFICCONTROL,
	                       DB_TOC_CONTROL_CODE, 0, cpControlCode,
	                       DB_TOC_CONTROL_CODE_SIZE, (char **)spTuple);
}
