/**
 * =====================================================================================
 *
 *       @file  sim_registry.c
 *
 *    	 @brief 
 *
 *       Version:  1.0
 *       Compiler:  cc (HP-UX)
 *       @date:  04/20/09 13:41:48
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
//#include "FLKSimBaseDef.h"
#include "libSimTable.h"
#include "libipc.h"
#include <time.h>
#include <stdio.h>
#include <string.h>
#include <strings.h>

typedef struct sim_table_t
{
	u_short 	usIndex; 
	u_short 	usActive;
	u_short 	usAppId;
	u_short 	usInsId;
	ULONG 	ulStartDate;
	pid_t 	tPid;
}sim_table_st;


#define SIM_TABLE_SHM_KEY 0xCDFF00AA
#define SIM_TABLE_SEM_KEY 0xCDFF00BB

#define SIM_TABLE_SIZE sizeof(sim_table_st)
#define SIM_TABLE_ARRAY_SIZE 120
#define SIM_TABLE_MAX_SIZE (SIM_TABLE_SIZE * SIM_TABLE_ARRAY_SIZE)
#define SIM_MO_TALBE_INDEX 0
#define SIM_HLR_TALBE_INDEX 30
#define SIM_MSC_TALBE_INDEX 60
#define SIM_WCDMA_TALBE_INDEX 90


#define SIM_TABLE_MAX_HEADER 4

#define SIM_MO_RECORD_COUNT 	30
#define SIM_HLR_RECORD_COUNT 	30
#define SIM_MSC_RECORD_COUNT 	30
#define SIM_WCDMA_RECORD_COUNT 	30

typedef struct sim_table_header_t
{
	APP_ID_TYPE eAppType;
	int index;
	int RecordCount;
}sim_table_header_st;

static sim_table_header_st SIM_TB_HeaderInfo[SIM_TABLE_MAX_HEADER] = {
	{ APP_ID_TYPE_MO, SIM_MO_TALBE_INDEX, SIM_MO_RECORD_COUNT },
	{ APP_ID_TYPE_HLR, SIM_HLR_TALBE_INDEX, SIM_HLR_RECORD_COUNT },
	{ APP_ID_TYPE_MSC, SIM_MSC_TALBE_INDEX, SIM_MSC_RECORD_COUNT },
	{ APP_ID_TYPE_WCDMA, SIM_WCDMA_TALBE_INDEX, SIM_WCDMA_RECORD_COUNT}
};

//static enum { RECORD_TYPE_MO, RECORD_TYPE_HLR, RECORD_TYPE_MSC, RECORD_TYPE_WCDMA};

static int g_SempId;
static int g_ShmId; 
static sim_table_st *sim_table;
static int g_TableInitFlag;

static int init_sim_registry_table ()
{
	if ( g_TableInitFlag == true )
		return FLK_SUCCESS;

	g_SempId = get_semp_id(SIM_TABLE_SEM_KEY);
	if ( g_SempId == -1 ) {
		return FLK_FAIL;
	}

	sem_lock ( g_SempId );
	g_ShmId = get_shm_id ( SIM_TABLE_SHM_KEY, SIM_TABLE_MAX_SIZE );
	if ( g_ShmId == -1 ) {
		sem_unlock ( g_SempId );
		return FLK_FAIL;
	}
	sim_table = (sim_table_st *)shmat(g_ShmId, (char *)0, 0);
	if ( sim_table == NULL )  {
		sem_unlock ( g_SempId );
		return FLK_FAIL;
	}
	sem_unlock(g_SempId);

	g_TableInitFlag = true;
	return FLK_SUCCESS;
}

static sim_table_header_st *getHeaderInfo(APP_ID_TYPE eAppType)
{
	int idx = 0;
	for ( idx = 0; idx < SIM_TABLE_MAX_HEADER; ++idx ) {
		if ( SIM_TB_HeaderInfo[idx].eAppType == eAppType )
			return &SIM_TB_HeaderInfo[idx];
	}

	return NULL;
}


bool isRunProcess ( int pid)
{
	char Req[256] = {0x0, };
	char Buff[1024] = {0x0, };
	char Tmp[1024] = {0x0, };
	char User[20] = {0x0, };
	int nPid = 0;
	FILE *fp = NULL ;
	bool Ret = false;
	sprintf(Req, "ps -ef |grep %d", pid);
	fp = popen ( Req, "r");
	if ( fp == NULL )
		return false;
	while ( fgets(Buff, sizeof(Buff), fp) != NULL ) {
		sscanf(Buff, "%s %d %s", User, &nPid, Tmp);
		if ( pid == nPid ) {
			Ret = true;
			break;
		}
			
	}
	pclose(fp);
	return Ret;

}

static int registrySimTable ( APP_ID_TYPE eAppIdType )
{
	int idx = 0;
	int nInsId = 1;
	int loop_max = 0;
	sim_table_header_st *spHeaderInfo;

	spHeaderInfo = getHeaderInfo(eAppIdType);
	if ( spHeaderInfo == NULL )
		return FLK_FAIL;

	loop_max = spHeaderInfo->index + spHeaderInfo->RecordCount;
	sem_lock (g_SempId);
	for ( idx = spHeaderInfo->index; idx < loop_max; ++idx ) {

		if ( isRunProcess(sim_table[idx].tPid) == false ) {
			bzero(&sim_table[idx], sizeof(sim_table_st));
		}

		if ( sim_table[idx].usActive == false ) {
			sim_table[idx].usIndex 		= idx; 
			sim_table[idx].usActive 	= true;
			sim_table[idx].usAppId 		= spHeaderInfo->eAppType;
			sim_table[idx].usInsId		= idx - spHeaderInfo->index + 1;
			sim_table[idx].ulStartDate 	= time(NULL);
			sim_table[idx].tPid 		= getpid();
			break;
		}
	}

	if ( idx == loop_max ) 
		nInsId = FLK_FAIL;
	else 
		nInsId = sim_table[idx].usInsId;
	sem_unlock (g_SempId);

	return nInsId;

}

int setSimTableInfo ( APP_ID_TYPE eAppIdType, int *nRetAppId, int *nRetInsId )
{
	int nInstanceId;

	if ( init_sim_registry_table () == FLK_FAIL )
		return FLK_FAIL;

	nInstanceId = registrySimTable(eAppIdType);
	if ( nInstanceId == FLK_FAIL ) {
		return FLK_FAIL;
	}

	*nRetAppId = (int)eAppIdType;
	*nRetInsId = nInstanceId;
	return FLK_SUCCESS;
}

int releaseSimTableInfo (APP_ID_TYPE eAppIdType ) 
{
	int idx = 0;
	int loop_max = 0;
	pid_t pid = getpid();

	sim_table_header_st *spHeaderInfo;

	spHeaderInfo = getHeaderInfo(eAppIdType);
	if ( spHeaderInfo == NULL )
		return FLK_FAIL;

	loop_max = spHeaderInfo->index + spHeaderInfo->RecordCount;
	sem_lock (g_SempId);
	for ( idx = spHeaderInfo->index; idx < loop_max; ++idx ) {
		if ( sim_table[idx].usActive == true &&
				sim_table[idx].tPid == pid) {
			bzero(&sim_table[idx], sizeof(sim_table_st));
			sim_table[idx].usIndex = idx;
			break;
		}
	}
	sem_unlock (g_SempId);

	return FLK_SUCCESS;
}



