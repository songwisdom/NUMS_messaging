#include  "flkipc.h"
#include <errno.h>

extern int errno;

/****************************************************************
  					MESSAGE QUEUE
  **************************************************************/
int queue_open_by_ftok(char *fkey)
{
    FILE *file;
    key_t msqkey;
    char path[256];

    sprintf(path, "%s/etc/key/%s", getenv("LMSGW_HOME"), fkey);

	if ( access(path, F_OK) != 0 ) {
		file = fopen(path, "w+");
		fclose(file);
	}

    msqkey = ftok(path, 1);
    if (msqkey < 0)
	{
		return -1;
	}

	info(0, "fkey[%s] path[%s] create msgkey [0x%08x]", fkey, path, msqkey);

    return queue_open(msqkey);
}

int queue_open(key_t key)
{
	int id;

	id = msgget(key, 0666 | IPC_CREAT | IPC_EXCL );
	if ( id < 0 ) {
		if ( errno == EEXIST ) {
			id = msgget(key, IPC_PRIVATE);
		}
	}

	if ( id < 0 ) {
		error(errno, "[%s : %d] msgget failed", __func__, __LINE__);
	}

	return id;
}

bool queue_read(int id, long *mtype, void *data, size_t len)
{

	bool ret = false;
	long rc;
	long type = *mtype;
	char *buff;

	if ( len == 0 && id < 0 )  {
		error(0, "[%s : %d] invalid parameter qid[%d] len [%ld]", __func__, __LINE__, id, len);
		return -1;
	}

	buff = gw_malloc(len + sizeof(long));
	if ( buff == NULL ) {
		error(errno, "[%s : %d] gw_malloc failed", __func__, __LINE__);
		return -1;
	}

	memcpy(buff, mtype, sizeof(long));

	rc = msgrcv( id, buff, len, type, 0 | MSG_NOERROR ) ;
	if ( rc < 0 ) {
		error(errno, "[%s : %d] msgrcv failed", __func__, __LINE__);
	}else if ( rc == len )  {
		memcpy(mtype, buff, sizeof(long));
		memcpy(data, &buff[sizeof(long)], rc);
		ret = true;
	}else {
		error(0, "[%s : %d] msgrcv return size [%ld] buffer size[%ld]", __func__, __LINE__, rc, len);
	}

	gw_free(buff);

	return ret;
}

bool queue_read_nonblock(int id, long *mtype, void *data, size_t len)
{

	bool ret = false;
	long rc;
	long type = *mtype;
	char *buff;

	if ( len == 0 && id < 0 )  {
		error(0, "[%s : %d] invalid parameter qid[%d] len [%ld]", __func__, __LINE__, id, len);
		return -1;
	}

	buff = gw_malloc(len + sizeof(long));
	if ( buff == NULL ) {
		error(errno, "[%s : %d] gw_malloc failed", __func__, __LINE__);
		return -1;
	}

	memcpy(buff, mtype, sizeof(long));
	memcpy(&buff[sizeof(long)], data, len);

	rc = msgrcv( id, buff, len, type, 0 | MSG_NOERROR | IPC_NOWAIT) ;
	if ( rc < 0 ) {
		if ( errno != ENOMSG )  {
			error(errno, "[%s : %d] msgrcv failed", __func__, __LINE__);
		}
	}else if ( rc == len )  {
		memcpy(mtype, buff, sizeof(long));
		memcpy(data, &buff[sizeof(long)], rc);
		ret = true;
	}else {
		error(0, "[%s : %d] msgrcv return size [%ld] buffer size[%ld]", __func__, __LINE__, rc, len);
	}

	gw_free(buff);

	return ret;

}

bool queue_send(int id, long mtype, void *data, size_t len)
{
	bool ret;
	int rc;
	char *buff;

	if ( mtype == 0 )
		mtype = 1;

	buff = gw_malloc(len + sizeof(long));
	if ( buff == NULL ) {
		error(errno, "[%s : %d] gw_malloc failed", __func__, __LINE__);
		return false;
	}

	memcpy(buff, &mtype, sizeof(long));
	memcpy(&buff[sizeof(long)], data, len);

	rc = msgsnd( id, buff, len, 0);
	if ( rc < 0 ) {
		error ( errno, "[%s : %d] msgsnd failed", __func__, __LINE__);
		ret = false;
	}else {
		ret = true;
	}

	gw_free(buff);

	return ret;
}

bool queue_send_nonblock(int id, long mtype, void *data, size_t len)
{
	bool ret;
	int rc;
	char *buff;

	if ( mtype == 0 )
		mtype = 1;

	buff = gw_malloc(len + sizeof(long));
	if ( buff == NULL ) {
		error(errno, "[%s : %d] gw_malloc failed", __func__, __LINE__);
		return false;
	}

	memcpy(buff, &mtype, sizeof(long));
	memcpy(&buff[sizeof(long)], data, len);

	rc = msgsnd( id, buff, len, 0 | IPC_NOWAIT);
	if ( rc < 0 ) {
		error ( errno, "[%s : %d] msgsnd failed", __func__, __LINE__);
		ret = false;
	}else {
		ret = true;
	}

	gw_free(buff);

	return ret;
}

int queue_count(int id )
{
	int ret;
	struct msqid_ds buf;

	if ( msgctl(id, IPC_STAT, &buf) < 0 ) {
		error(errno, "[%s : %d] msgctl qid(%d) failed", __func__, __LINE__, id);
		return -1;
	}

	ret = buf.msg_qnum;

	return ret;
}


FLK_SHM_ST *posix_shm_open(char *name, size_t size)
{
	int rc;
	int fd;
	char *shm_ptr;


	fd = shm_open(name, O_RDWR | O_CREAT | O_EXCL, 0666);
	if ( fd < 0 ) {
		//info(errno, "shm_open");
		fd = shm_open(name, O_RDWR, 0666);
		if ( fd < 0 ) {
			error(errno, "shm_open failed name[%s] size[%lu]", name, size);
			return NULL;
		}
	}else {
		//info(errno, "shm_open success");
		rc = ftruncate( fd, size );
		if ( rc < 0 ) {
			// TODO : close fd
			error(errno, "name[%s] ftruncate failed", name);
			shm_unlink(name);
			return NULL;
		}
	}

	shm_ptr = (char *)mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
	if ( shm_ptr == MAP_FAILED ) {
		error(errno, "[%s : %d] mmap failed : name [%s]", __func__, __LINE__, name);
		shm_unlink(name);
		return NULL;
	}

	FLK_SHM_ST *ret = (FLK_SHM_ST *)gw_malloc(sizeof(FLK_SHM_ST));
	if ( ret == NULL ) {
		error(errno, "[%s : %d] FLK_SHM_ST memory alloc failed : shm_name[%s]", __func__, __LINE__, name);
		return NULL;
	}

	memset(ret, 0x00, sizeof(FLK_SHM_ST));

	ret->fd = fd;
	ret->size = size;
	ret->name = gw_malloc(strlen(name)+1);
	strcpy(ret->name, name);
	ret->name[strlen(name)] = '\0';
	ret->shm_ptr = shm_ptr;

	return ret;
}

/********************************************************************
  SYSTEM V Semapore 
  ******************************************************************/
bool flk_sem_ctrl(int semid, int num, int val)
{
	struct sembuf opt;

	if( val == 0 )
	{
		error(errno, "[%s : %d] Error : nSemId[%d] nValue[%d] (not allow wait for zero) \n", __func__, __LINE__, semid, val);
		return false;
	}

	opt.sem_num 	= num;
	opt.sem_op 		= val;
	opt.sem_flg 	= SEM_UNDO;

    if(semop(semid, &opt,1)<0)
    {
		error(errno, "[%s : %d] Error SemdId(%d) nValue : (%d) num(%d)\n", __func__, __LINE__, semid, val, num);
        return false;
    }

    return true;
}

bool flk_sem_lock(int semid)
{
	if ( flk_sem_ctrl(semid, 0, -1) == false ) {
		error(errno, "[%s : %d] sem_p failed : semid(%d) \n", __func__, __LINE__, semid);
		return false;
	}

    return true;
}

bool flk_sem_unlock(int semid)
{
	if ( flk_sem_ctrl(semid, 0, 1) == false ) {
		error(errno, "[%s : %d] sem_p failed : semid(%d) \n", __func__, __LINE__, semid);
		return false;
	}

    return true;
}

bool flk_multi_sem_lock(int semid, int num)
{
	if ( flk_sem_ctrl(semid, num, -1) == false ) {
		return false;
	}

	return true;
}

bool flk_multi_sem_unlock(int semid, int num)
{
	if ( flk_sem_ctrl(semid, num, 1) == false ) {
		return false;
	}

	return true;
}

int flk_sem_create(int semkey)
{
	union semun sem_union;
	int semid = 0;
	int ret = 0;

	semid = semget(semkey,1, 0666|IPC_CREAT|IPC_EXCL);
  	if(0 > semid)
    {
        if(errno == EEXIST) {
            semid = semget(semkey, 1, 0);
		} else {
			error(errno, "[%s : %d] shmget failed", __func__, __LINE__);
		}
    }
    else {
		sem_union.val = 1;
		ret = semctl(semid, 0, SETVAL, sem_union);
		if ( ret < 0 ) {
			error(errno, "[%s : %d] semctl SETVAL failed : idx(%d) ", __func__, __LINE__, 0);
			semid = -1;
		}
	}

    return semid;
}

int flk_multi_sem_create(int semkey, int num)
{
	union semun sem_union;
	int semid = 0;
	int ret = 0;
	int idx;

	semid = semget(semkey, num, 0666|IPC_CREAT|IPC_EXCL);
  	if(0 > semid)
    {
        if(errno == EEXIST) {
            semid = semget(semkey, 1, 0);
		} else {
			error(errno, "[%s : %d] shmget failed", __func__, __LINE__);
		}
    } else {
		sem_union.val = 1;

		for ( idx = 0; idx < num; ++idx ) {
			ret = semctl(semid, idx, SETVAL, sem_union);
			if ( ret < 0 ) {
				error(errno, "[%s : %d] semctl SETVAL failed : idx(%d) ", __func__, __LINE__, idx);
				semid = -1;
				break;
			}
		}
	}

    return semid;
}

void flk_sem_close(int semid)
{
	return;
}


bool flk_sem_remove(int semid)
{

	if( semctl(semid, 0, IPC_RMID ) < 0 ) {
		error(errno, "[%s : %d] sem IPC_RMID failed id(%d)", __func__, __LINE__, semid);
		return false;
	}

	return true;
}

