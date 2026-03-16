#include	<sys/ipc.h>
#include	<sys/msg.h>
#include	<sys/types.h>
#include	<stdlib.h>
#include	<string.h>
#include	<errno.h>
#include	<sys/shm.h>
#include	<sys/sem.h>
#include 	"FLKSimBaseDef.h"
#include 	"libipc.h"
#include 	"libdbg.h"

#define	PERMS	0666
#define	MSGQ_SZ	1024

#define	MSGQ_RTP_SZ	250

#define IPC_KEY_SEM 'M'
#define IPC_KEY_SHM 'S'
#define IPC_KEY_CHN_CNT 'C' /* key for channel count */
#define IPC_KEY_MSG     'M'
#define IPC_KEY_QUE 'Q' /* Semaphore key for Queue Key */

typedef struct {
	long	msg_type;
	char	msg_data [ MSGQ_SZ ];
} msgq_data_st;



static	int que_sem_id;
union semun {
	int val;
	struct  semid_ds * buf;
	unsigned short int * array;
	struct seminfo *__buff;
};

static  struct sembuf lock = { 0, -1, SEM_UNDO };
static  struct sembuf unlock = { 0, 1, SEM_UNDO };

key_t get_token(const char * path, char id)
{
	key_t   ipc_key;
	ipc_key = ftok(path, id);
	return ipc_key;
}
int get_queue_semp_id(char * path)
{
	key_t	fkey;

	if ( path == NULL ) return -1;

	fkey = ftok(path, IPC_KEY_QUE);
	if ( fkey < 0 ) return -1;

	que_sem_id = get_semp_id(fkey);
	if ( que_sem_id == -1 ) {
		dbg(9, "ERR:get_queue_semp_id(%x)", que_sem_id);
		return -1;
	}
	
	return que_sem_id;
}

int get_semp_id(key_t ipc_key)
{
	int sem_id;

	union   semun sem_union;

	sem_id = semget(ipc_key, 1, IPC_CREAT | IPC_EXCL | 0666);
	if ( sem_id == -1 ) {
		if ( ( sem_id = semget( ipc_key, 1, 0 ) ) == -1 ) {
			return -1;
		}

		return ( sem_id );
	}

	sem_union.val = 1;
	if ( semctl( sem_id, 0, SETVAL, sem_union ) == -1 ) {
		return -1;
	}

	return sem_id;
}

void sem_remove(int sem_id)
{
	semctl(sem_id, 0, IPC_RMID, 0);
}

int sem_lock(int sem_id)
{
	if ( semop(sem_id, &lock, 1 ) == -1 ) {
		return -1;
	}
	return 0;
}

int sem_unlock(int sem_id)
{
	if ( semop(sem_id, &unlock, 1 ) == -1 ) {
		return -1;
	}
	return 0;
}

int get_shm_id(key_t ipc_key, int size)
{
	int shm_id;

	shm_id = shmget(ipc_key, size, IPC_CREAT | IPC_EXCL | 0666 );
	if ( shm_id == -1 ) {
		shm_id = shm_create(ipc_key, size);
		if ( shm_id == -1 ) return -1;
	}


	return shm_id;
}

int shm_create( key_t ipc_key, int size )
{
	int shm_id;

	shm_id = shmget(ipc_key, size, 0666 | IPC_CREAT );
	if ( shm_id == -1 ) {
		return -1;
	}

	return shm_id;
}

void shm_remove(int shm_id)
{
	shmctl(shm_id, IPC_RMID, 0);
	return;
}
bool exist_shm_id(key_t key, int * id) 
{
	int	shm_id;

	dbg(5, "exist_shm_id key = (%x)", key);

	shm_id = shmget(key, 0, IPC_CREAT | IPC_EXCL | 0666);
	if ( (shm_id == -1 ) &&( errno == EEXIST )) {
		dbg(9, "already exist shm_id(%x)", key);
		*id = shm_create(key, 0);;
		return TRUE;
	}

	return FALSE;
}
////////////////////////////////////
//


int get_msgq_id(key_t key)
{
	int	msgq_id;

	msgq_id = msgget(key, PERMS | IPC_CREAT);
	if ( msgq_id < 0 ) {
		dbg(9, "ERR:can't get msgq id");
		return -1;
	}

	dbg(5, "OK:get_msgq_id(%d)", msgq_id);
	return msgq_id;
}

int create_msgq(key_t key)
{
	int	msgq_id;

	msgq_id = get_msgq_id(key);

	return msgq_id;
}

int release_msgq(int msgq_id)
{
	if ( msgq_id < 0 ) return -1;
 
	if ( msgctl( msgq_id, IPC_RMID, (struct msqid_ds *)0 ) < 0 ) {
		dbg(9, "ERR:release msgq failed.(%d)", msgq_id);
		return -1;
	}

	return 0;
}

int msgq_data_clear(int msgq_id, long mtype)
{
	int	r;

	static char tmp [ MSGQ_SZ ];

	dbg(1, "START:msgq_data_clear(%d, %d)", msgq_id, mtype);
	while(1) {
		memset(tmp, 0x0, sizeof(tmp));

		r = msgq_read_nonblock(msgq_id, mtype, tmp, sizeof(tmp));
		if ( r == -1 ) {
			dbg(9, "msgq_data_clear all data read(qid : %d, mtype : %d)", msgq_id, mtype);
			return 0;
		}

		dbg(1, "msgq_data_clear read data = %s", tmp);
	}
}

int msgq_write_block(int msgq_id, long mtype, char * data, int len)
{
	int	r;
	msgq_data_st  msgq_data;

	memset((char *)&msgq_data, 0x0, sizeof(msgq_data));
	msgq_data.msg_type = mtype;

	if ( sizeof(msgq_data.msg_data) < len ) {
		len = sizeof(msgq_data.msg_data);
		*(data + len) = 0x0;
	}

	memcpy(msgq_data.msg_data, data, len);
	r = msgsnd(msgq_id, (msgq_data_st *)&msgq_data, sizeof(msgq_data_st) - sizeof(msgq_data.msg_type), 0);
	if ( r == -1 ) {
		dbg(9, "ERR:msgq_write_nonblock write error(id:%d)", msgq_id);
		return -1;
	}

	dbg(1, "OK:msgq_write_nonblock write OK(%s)", data);
	return 0;
}

int send_message(int msgq_id, long mtype, const u_char * data, int len)
{
	int nbytes;
	msgq_data_st  msgq_data;

	if ( len == 0 ) {
		printf("send_message length 0\n");
	}
	
	memset(&msgq_data, 0x0, sizeof(msgq_data));

	msgq_data.msg_type = mtype;

	memcpy(msgq_data.msg_data, data, len);
	
	if ( (nbytes = msgsnd(msgq_id, (msgq_data_st *)&msgq_data, sizeof(msgq_data.msg_data), IPC_NOWAIT)) < 0 ) {
		dbg(9, "ERR:send_message failed(%d)(%s)", msgq_id, get_err_msg(errno));
	}
	

	return nbytes;
}


int recv_message(int msgq_id, long mtype, u_char * data, int len)
{
	int nbytes;
	msgq_data_st  msgq_data;

	memset((char *)&msgq_data, 0x0, sizeof(msgq_data));
	nbytes = msgrcv(msgq_id, &msgq_data, sizeof(msgq_data.msg_data), mtype, 0 | MSG_NOERROR);
	if ( nbytes <= 0 ) {
		dbg(9, "ERR:recv_message failed(%d)(%s)", msgq_id, get_err_msg(errno));
		return -1;
	} 

	memcpy(data, msgq_data.msg_data, sizeof(msgq_data.msg_data));

	return nbytes;
}

int recv_message2(int msgq_id, long * mtype, u_char * data, int len)
{
	int nbytes;
	msgq_data_st  msgq_data;

	memset((char *)&msgq_data, 0x0, sizeof(msgq_data));
	nbytes = msgrcv(msgq_id, &msgq_data, sizeof(msgq_data.msg_data), (long)0, 0 | MSG_NOERROR);
	if ( nbytes <= 0 ) {
		dbg(9, "ERR:recv_message failed(%d)(%s)", msgq_id, get_err_msg(errno));
		return -1;
	} 

	*mtype = msgq_data.msg_type;
	memcpy(data, msgq_data.msg_data, sizeof(msgq_data.msg_data));

	return nbytes;
}

int msgq_write_nonblock(int msgq_id, long mtype, char * data, int len)
{
	int	r;
	msgq_data_st  msgq_data;

	dbg(1, "START:msgq_write_nonblock(id = %d, key = %d, data = %s, len = %d)", msgq_id, mtype, data, len);

	memset((char *)&msgq_data, 0x0, sizeof(msgq_data));
	msgq_data.msg_type = mtype;

	if ( sizeof(msgq_data.msg_data) < len ) {
		len = sizeof(msgq_data.msg_data);
		*(data + len) = 0x0;
	}

	memcpy(msgq_data.msg_data, data, len);
	r = msgsnd(msgq_id, (msgq_data_st *)&msgq_data, sizeof(msgq_data_st) - sizeof(msgq_data.msg_type), IPC_NOWAIT);
	if ( r == -1 ) {
		dbg(9, "ERR:msgq_write_nonblock write error(id:%d, ErrCode=%d)", msgq_id, errno);
		return -1;
	}

	dbg(1, "OK:msgq_write_nonblock write OK(%s)", data);
	return 0;
}

int msgq_read_block(int msgq_id, long mtype, char * data, int len)
{
	int	r;
	msgq_data_st  msgq_data;

	memset((char *)&msgq_data, 0x0, sizeof(msgq_data));
	r = msgrcv(msgq_id, (char *)&msgq_data, len, mtype, 0);
	if ( r < 0 ) {
		dbg(9, "ERR:msgq_read_block failed(%d)(%s)", msgq_id, get_err_msg(errno));
		return -1;
	}

	memcpy(data, msgq_data.msg_data, r);
	dbg(1, "OK:msgq_read_block read data = %s", data);

	return r;
}

int msgq_read_nonblock(int msgq_id, long mtype, char * data, int len)
{
	int	r;
	msgq_data_st  msgq_data;

	memset((char *)&msgq_data, 0x0, sizeof(msgq_data));
	r = msgrcv(msgq_id, (char *)&msgq_data, len, mtype, IPC_NOWAIT);
	if ( r < 0 ) {
		dbg(9, "ERR:msgq_read_nonblock failed(%d)(%s)", msgq_id, get_err_msg(errno));
		return -1;
	}

	memcpy(data, msgq_data.msg_data, r);
	dbg(1, "OK:msgq_read_nonblock read data = %s", data);

	return r;
}

int msgq_read(int msgq_id, long * mtype, char * data, int len)
{
	int	r;
	msgq_data_st  msgq_data;

	memset((char *)&msgq_data, 0x0, sizeof(msgq_data));
	r = msgrcv(msgq_id, (void *)&msgq_data, len, (long)0, IPC_NOWAIT);
	if ( r < 0 ) {
		dbg(9, "ERR:msgq_read failed(%d)", msgq_id);
		return -1;
	}

	*mtype = msgq_data.msg_type;
	memcpy(data, msgq_data.msg_data, r);
	dbg(1, "OK:msgq_read read data = %s", data);

	return r;
}

int get_queue_ds(int msgq_id, struct msqid_ds * qbuf)
{
	if ( msgctl(msgq_id, IPC_STAT, qbuf ) == -1 ) 
		return -1;

	return 0;
}

