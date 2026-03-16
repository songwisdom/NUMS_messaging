#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#include <mysql.h>

#include "gwlib.h"
#include "dbpool.h"

typedef struct {
	int col1;
	char col2[40];
	short col3;
}TEST_SAMPLE;

MySQLConf mysql_cfg;
DBConf db_conf;
DBPool *db_pool;

#define CREATE_SAMPLE_TABLE "CREATE TABLE test_table(col1 INT, col2 VARCHAR(40), col3 SMALLINT)"

bool init()
{
	MySQLConf *conf;

	db_conf.mysql = gw_malloc(sizeof(MySQLConf));

	memset(db_conf.mysql, 0x00, sizeof(MySQLConf));

	conf = db_conf.mysql;

	conf->host = octstr_create("127.0.0.1");
	conf->username = octstr_create("lmsgw");
	conf->password = octstr_create("cjswo");
	conf->database = octstr_create("lmsgw");
	conf->port = 3306;

    db_pool = dbpool_create(DBPOOL_MYSQL, &db_conf, 1, 10);
    if ( db_pool == NULL ) {
        error(0, "[%s] dbpool_create failed", __func__);
        return false;
    }

	dbpool_stmt_register(db_pool, 0, "INSERT INTO test_table VALUES(?, ?, ?)");
	dbpool_stmt_register(db_pool, 1, "SELECT * FROM test_table");
	dbpool_stmt_register(db_pool, 2, "UPDATE test_table SET col1 = ? WHERE col3 = ?");
	dbpool_stmt_register(db_pool, 3, "SELECT * FROM test_table WHERE col3 = ?");
	dbpool_stmt_register(db_pool, 4, "DELETE FROM test_table WHERE col3 = ?");


    return true;
}

void test_stmt_4_delete()
{
	bool ret;
	long count;
	short col3 = 0;
	TEST_SAMPLE data;
	MYSQL_BIND bind[1];


	dbpool_stmt_bind_param_short(&bind[0], &col3, false);

	DBPoolConn *conn = dbpool_conn_consume(db_pool);

	ret = dbpool_stmt_execute(db_pool, conn, 4, bind, NULL, &count);
	if ( ret == false ) {
		error(0, "[%s] failed : %s", __func__, conn->errstr);
	}else {
		info(0, "STMT4 DELETE SUCCESS count %ld", count);
	}


	dbpool_conn_produce(conn);
}

void test_stmt_2_update()
{
	bool ret;
	long count;
	TEST_SAMPLE data;
	MYSQL_BIND *bind = (MYSQL_BIND *)calloc (2, sizeof(MYSQL_BIND));


	data.col1 = 100;
	data.col3 = 4;

	dbpool_stmt_bind_param_integer(&bind[0], &data.col1, false);
	dbpool_stmt_bind_param_short(&bind[1], &data.col3, false);

	DBPoolConn *conn = dbpool_conn_consume(db_pool);

	ret = dbpool_stmt_execute(db_pool, conn, 2, bind, NULL, &count);
	if ( ret == false ) {
		error(0, "[%s] failed : %s", __func__, conn->errstr);
	}else {
		info(0, "STMT2 UPDATE SUCCESS count %ld", count);
	}

	free(bind);

	dbpool_conn_produce(conn);
}
void test_stmt_0_insert()
{
	bool ret;
	long count;
	TEST_SAMPLE data;
	MYSQL_BIND *bind = (MYSQL_BIND *)calloc (3, sizeof(MYSQL_BIND));


	data.col1 = 1;
	strcpy(data.col2, "99999999999999");
	data.col3 = 4;
	dbpool_stmt_bind_param_integer(&bind[0], &data.col1, false);
	dbpool_stmt_bind_param_varchar(&bind[1], data.col2, strlen(data.col2), false);
	dbpool_stmt_bind_param_short(&bind[2], &data.col3, false);

	DBPoolConn *conn = dbpool_conn_consume(db_pool);

	ret = dbpool_stmt_execute(db_pool, conn, 0, bind, NULL, &count);
	if ( ret == false ) {
		error(0, "[%s] failed : %s", __func__, conn->errstr);
	}else {
		info(0, "STMT0 INSERT SUCCESS count %ld", count);
	}

	free(bind);

	dbpool_conn_produce(conn);
}

void test_stmt_1_select()
{
	bool ret;
	long count;

	TEST_SAMPLE data;
	MYSQL_BIND *bind = (MYSQL_BIND *)calloc (3, sizeof(MYSQL_BIND));


	memset(&data, 0x0, sizeof(data));
	dbpool_stmt_bind_result_integer(&bind[0], &data.col1);
	dbpool_stmt_bind_result_varchar(&bind[1], data.col2, sizeof(data.col2));
	dbpool_stmt_bind_result_short(&bind[2], &data.col3);

	DBPoolConn *conn = dbpool_conn_consume(db_pool);

	ret = dbpool_stmt_execute(db_pool, conn, 1, NULL, bind, &count);
	if ( ret == false ) {
		error(0, "[%s] failed : %s", __func__, conn->errstr);
	}else {
		info(0, "STMT1 SELECT SUCCESS count %ld", count);
	}

	long i;

	for ( i = 0; i < count; ++i ) {

		if (  dbpool_stmt_fetch(conn, 1) == false ) {
			//error(0, "dbpool_stmt_fetch failed :err(%d) %s", conn->errno, conn->errstr);
			break;
		}
		info(0, "[%ld] col1[%d] col2[%s] col3[%d]", i, data.col1, data.col2, data.col3);
	}

	free(bind);

	dbpool_conn_produce(conn);
}
void test_stmt_3_select()
{
	bool ret;
	long count;

	TEST_SAMPLE data;

	short col3 = 3;
	MYSQL_BIND param[1];
	MYSQL_BIND *bind = (MYSQL_BIND *)calloc (3, sizeof(MYSQL_BIND));


	dbpool_stmt_bind_param_short(&param[0],&col3, false);
	memset(&data, 0x0, sizeof(data));
	dbpool_stmt_bind_result_integer(&bind[0], &data.col1);
	dbpool_stmt_bind_result_varchar(&bind[1], data.col2, sizeof(data.col2));
	dbpool_stmt_bind_result_short(&bind[2], &data.col3);

	DBPoolConn *conn = dbpool_conn_consume(db_pool);

	ret = dbpool_stmt_execute(db_pool, conn, 3, param, bind, &count);
	if ( ret == false ) {
		error(0, "[%s] failed : %s", __func__, conn->errstr);
	}else {
		info(0, "STMT3 SELECT SUCCESS count %ld", count);
	}

	long i;

	for ( i = 0; i < count; ++i ) {

		if (  dbpool_stmt_fetch(conn, 3) == false ) {
			//error(0, "dbpool_stmt_fetch failed :err(%d) %s", conn->errno, conn->errstr);
			break;
		}
		info(0, "[%ld] col1[%d] col2[%s] col3[%d]", i, data.col1, data.col2, data.col3);
	}

	free(bind);

	dbpool_conn_produce(conn);
}

int main ( int argc, char **argv )
{
	bool ret;
	gwlib_init();
	Octstr *sql;
	short sysnum = 2;
	int seq = 99;

	log_set_output_level(0);
	if ( init() == false ) {
		error(0, "init failed");
		gwlib_shutdown();
		exit(0);
	}

	test_stmt_0_insert();
	test_stmt_0_insert();
	test_stmt_0_insert();
	test_stmt_1_select();
	test_stmt_2_update();
	test_stmt_3_select();
	test_stmt_4_delete();

result :

	gwlib_shutdown();

	exit(0);
}
