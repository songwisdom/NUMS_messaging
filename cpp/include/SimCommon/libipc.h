/**
 * =====================================================================================
 *
 *       @file  libipc.h
 *
 *    	 @brief 
 *
 *       Version:  1.0
 *       Compiler:  cc (HP-UX)
 *       @date:  04/20/09 11:20:13
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
#ifndef _H_LIB_IPC_
#define _H_LIB_IPC_
#include 	"flk_type.h"
#include    <sys/ipc.h>
#include    <sys/msg.h>
#include    <sys/types.h>
#include    <errno.h>
#include    <sys/shm.h>
#include    <sys/sem.h>
#include 	"libipc.h"
#ifdef __cplusplus
extern "C" {
#endif
key_t get_token(const char * path, char id);
int get_queue_semp_id(char * path);
int get_semp_id(key_t ipc_key);
void sem_remove(int sem_id);
int sem_lock(int sem_id);
int sem_unlock(int sem_id);
int get_shm_id(key_t ipc_key, int size);
int shm_create( key_t ipc_key, int size );
void shm_remove(int shm_id);
bool exist_shm_id2(key_t key, int * id) ;
int get_msgq_id(key_t key);
int create_msgq(key_t key);
int release_msgq(int msgq_id);
int msgq_data_clear(int msgq_id, long mtype);
int msgq_write_block(int msgq_id, long mtype, char * data, int len);
int send_message(int msgq_id, long mtype, const u_char * data, int len);
int recv_message(int msgq_id, long mtype, u_char * data, int len);
int recv_message2(int msgq_id, long * mtype, u_char * data, int len);
int msgq_write_nonblock(int msgq_id, long mtype, char * data, int len);
int msgq_read_block(int msgq_id, long mtype, char * data, int len);
int msgq_read_nonblock(int msgq_id, long mtype, char * data, int len);
int msgq_read(int msgq_id, long * mtype, char * data, int len);
int get_queue_ds(int msgq_id, struct msqid_ds * qbuf);
#ifdef __cplusplus
}
#endif
#endif

