#ifndef _H_FLK_IPCS_DEF_
#define _H_FLK_IPCS_DEF_

#include  <sys/ipc.h>
#include  <sys/msg.h>                                                                                                                                        
#include  <sys/types.h>
#include  <stdlib.h>
#include  <string.h>
#include  <errno.h>
#include  <sys/shm.h>
#include  <sys/sem.h>
#include  <sys/msg.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>

#include  <errno.h>
#include  "gwlib.h"

typedef struct {
	int fd;
	off_t size;
	char *name;
	char *shm_ptr;
}FLK_SHM_ST;
/****************************************************************
  					MESSAGE QUEUE
  **************************************************************/

int queue_open(key_t key);
bool queue_read(int id, long *mtype, void *data, size_t len);
bool queue_read_nonblock(int id, long *mtype, void *data, size_t len);
bool queue_send(int id, long mtype, void *data, size_t len);
bool queue_send_nonblock(int id, long mtype, void *data, size_t len);
int queue_count(int id );
int queue_open_by_ftok(char *fkey);

/****************************************************************
  				POSIX SHARED MEMORY	
  **************************************************************/
FLK_SHM_ST *posix_shm_open(char *name, size_t size);

/********************************************************************
  SYSTEM V Semapore 
  ******************************************************************/

union semun {
	int val;
	struct semid_ds *buf;
	u_short *array;
};
bool flk_sem_ctrl(int semid, int num, int val);
bool flk_sem_lock(int semid);
bool flk_sem_unlock(int semid);
bool flk_multi_sem_lock(int semid, int num);
bool flk_multi_sem_unlock(int semid, int num);
int flk_sem_create(int semkey);
int flk_multi_sem_create(int semkey, int num);
void flk_sem_close(int semid);
bool flk_sem_remove(int semid);
#endif
