#include <spdlog/spdlog.h>
#include <spdlog/spdlog.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <errno.h>

#include "FLKBaseDef.h"
#include "MdbCommon.h"


#define DBMS_ADDR_ST_SIZE sizeof(DBMS_ADDR_ST) * MAX_DBMS_CNT
#define MDB_ADDR_CONV "MDB_ADDR_FILE"

//#define LogRet printf
static int create_mdb_adr_file()
{
	char *adr_file;
	int fd;
	char *dbms_adr;

	adr_file = getenv(MDB_ADDR_CONV);
	if ( adr_file == NULL ) {
		spdlog::error("{} getenv {} failed", __func__, MDB_ADDR_CONV);
		return FLK_FAIL;
	}

	fd = open(adr_file, O_CREAT | O_RDWR, 0666);
	if ( fd < 0 ) {
		spdlog::error("{} open {} failed", __func__, adr_file);
		return FLK_FAIL;
	}

 	lockf(fd, F_LOCK, 0);

	dbms_adr = (char *)malloc(DBMS_ADDR_ST_SIZE);
	memset(dbms_adr, 0x00, DBMS_ADDR_ST_SIZE);

	write(fd, dbms_adr, DBMS_ADDR_ST_SIZE);

	lockf(fd, F_ULOCK, 0);

	close(fd);
	return FLK_SUCCESS;
}

static int open_mdb_adr_file()
{
	char *adr_file;
	int fd;

	adr_file = getenv(MDB_ADDR_CONV);
	if ( adr_file == NULL ) {
		spdlog::error("getenv {} failed", MDB_ADDR_CONV);
		return FLK_FAIL;
	}

	if ( access(adr_file, F_OK) != 0 ) {
		create_mdb_adr_file();
	}

	fd = open(adr_file, O_RDWR);
	if ( fd < 0 ) {
		spdlog::error("open_mdb_adr_file {} open failed", adr_file);
		return FLK_FAIL;
	}

	if ( lockf(fd, F_LOCK, 0) != 0 ) {
		perror("lockd failed");
		close(fd);
		return FLK_FAIL;
	}

	return fd;

}

static void close_mdb_adr_file(int fd)
{
	lockf(fd, F_ULOCK, 0);
	close(fd);
}


int read_dbms_addr(int dbms_idx, DBMS_ADDR_ST *addr)
{
	int fd = -1;
	int ret = FLK_FAIL;

	DBMS_ADDR_ST dbms_adr[MAX_DBMS_CNT];


	fd = open_mdb_adr_file();
	if ( fd == FLK_FAIL ) {
		spdlog::error("open_mdb_adr_file failed");
		return FLK_FAIL;
	}


	if ( read(fd, dbms_adr, DBMS_ADDR_ST_SIZE) == DBMS_ADDR_ST_SIZE ) {
		memcpy(addr, &dbms_adr[dbms_idx], sizeof(DBMS_ADDR_ST));
		ret = FLK_SUCCESS;
	}

	close_mdb_adr_file(fd);

	return ret;
}

int set_mdb_shm_addr(int dbms_idx, char *dbms_adr, char *index_adr, char *dirty_adr)
{
	int fd = -1;

	DBMS_ADDR_ST adr;

	adr.dbms_adr = (long)dbms_adr;
	adr.index_adr = (long)index_adr;
	adr.dirty_adr = (long)dirty_adr;

	fd = open_mdb_adr_file();
	if ( fd == FLK_FAIL )  {
		spdlog::error("open_mdb_adr_file failed");
		return FLK_FAIL;
	}

	lseek(fd, dbms_idx * sizeof(DBMS_ADDR_ST), SEEK_SET);

	write(fd, &adr, sizeof(DBMS_ADDR_ST));

	close_mdb_adr_file(fd);
	return FLK_SUCCESS;
}

int main (int argc, char **argv)
{
	char *a = 1;
	char *b = 2;
	char *c = 3;

	if ( set_mdb_shm_addr(0, a, b,c) == FLK_FAIL ) {
		printf("error\n");
	}else {
		printf ("success\n");
	}

}
