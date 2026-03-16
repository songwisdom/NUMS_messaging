/**
 * =====================================================================================
 *
 *       @file  flk_dbm.h
 *
 *    	 @brief 
 *
 *       Version:  1.0
 *       Compiler:  cc (HP-UX)
 *       @date:  05/20/09 13:15:22
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

#ifndef _H_LIB_FLK_DBM_
#define _H_LIB_FLK_DBM_
#include "gdbmconst.h"
#include "gdbmdefs.h"
#include "systems.h"
#include "extern.h"
#include "gdbmerrno.h"
#include "proto.h"

#define  DBM_READER 	GDBM_READER  
#define  DBM_WRITER 	GDBM_WRITER  
#define  DBM_WRCREAT 	GDBM_WRCREAT 
#define  DBM_NEWDB 	GDBM_NEWDB   
#define  DBM_OPENMASK 	GDBM_OPENMASK 
#define  DBM_FAST 	GDBM_FAST    
#define  DBM_SYNC 	GDBM_SYNC    
#define  DBM_NOLOCK 	GDBM_NOLOCK  

/* Parameters to gdbm_store for simple insertion or replacement in the
   case a key to store is already in the database. */
#define  DBM_INSERT 	GDBM_INSERT  
#define  DBM_REPLACE 	GDBM_REPLACE 

/* Parameters to gdbm_setopt, specifing the type of operation to perform. */
#define	 DBM_CACHESIZE 	GDBM_CACHESIZE	
#define  DBM_FASTMODE 	GDBM_FASTMODE	
#define  DBM_SYNCMODE 	GDBM_SYNCMODE	
#define  DBM_CENTFREE 	GDBM_CENTFREE	
#define  DBM_COALESCEBLKS 	GDBM_COALESCEBLKS 

#ifdef __cplusplus
extern "C" {
#endif
void dbm_close ( FLK_DBM *dbf );
int dbm_delete ( FLK_DBM *dbf, datum key );
datum dbm_fetch (FLK_DBM *dbf, datum key );
int dbminit ( char *file );
FLK_DBM * dbm_open ( char *file, int flags, int mode );
int dbm_store ( FLK_DBM *dbf, datum key, datum content, int flags );
datum dbm_firstkey ( FLK_DBM *dbf );
datum dbm_nextkey ( gdbm_file_info *dbf );
#ifdef __cplusplus
}
#endif

#endif
