/* ==================================================================== 
 * The Kannel Software License, Version 1.0 
 * 
 * Copyright (c) 2001-2009 Kannel Group  
 * Copyright (c) 1998-2001 WapIT Ltd.   
 * All rights reserved. 
 * 
 * Redistribution and use in source and binary forms, with or without 
 * modification, are permitted provided that the following conditions 
 * are met: 
 * 
 * 1. Redistributions of source code must retain the above copyright 
 *    notice, this list of conditions and the following disclaimer. 
 * 
 * 2. Redistributions in binary form must reproduce the above copyright 
 *    notice, this list of conditions and the following disclaimer in 
 *    the documentation and/or other materials provided with the 
 *    distribution. 
 * 
 * 3. The end-user documentation included with the redistribution, 
 *    if any, must include the following acknowledgment: 
 *       "This product includes software developed by the 
 *        Kannel Group (http://www.kannel.org/)." 
 *    Alternately, this acknowledgment may appear in the software itself, 
 *    if and wherever such third-party acknowledgments normally appear. 
 * 
 * 4. The names "Kannel" and "Kannel Group" must not be used to 
 *    endorse or promote products derived from this software without 
 *    prior written permission. For written permission, please  
 *    contact org@kannel.org. 
 * 
 * 5. Products derived from this software may not be called "Kannel", 
 *    nor may "Kannel" appear in their name, without prior written 
 *    permission of the Kannel Group. 
 * 
 * THIS SOFTWARE IS PROVIDED ``AS IS'' AND ANY EXPRESSED OR IMPLIED 
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES 
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE 
 * DISCLAIMED.  IN NO EVENT SHALL THE KANNEL GROUP OR ITS CONTRIBUTORS 
 * BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY,  
 * OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT  
 * OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR  
 * BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,  
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE  
 * OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,  
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE. 
 * ==================================================================== 
 * 
 * This software consists of voluntary contributions made by many 
 * individuals on behalf of the Kannel Group.  For more information on  
 * the Kannel Group, please see <http://www.kannel.org/>. 
 * 
 * Portions of this software are based upon software originally written at  
 * WapIT Ltd., Helsinki, Finland for the Kannel project.  
 */ 

/*
 * dbpool_mysql.c - implement MySQL operations for generic database connection pool
 *
 * Stipe Tolj <stolj@wapme.de>
 *      2003 Initial version.
 * Alexander Malysh <a.malysh@centrium.de>
 *      2003 Made dbpool more generic.
 */

#ifdef HAVE_MYSQL

#include <mysql.h>
#include <errmsg.h>

static void *mysql_open_conn(const DBConf *db_conf)
{
    MYSQL *mysql = NULL;
    MySQLConf *conf = db_conf->mysql; /* make compiler happy */

    /* sanity check */
    if (conf == NULL)
        return NULL;

    /* pre-allocate */
    mysql = gw_malloc(sizeof(MYSQL));
    gw_assert(mysql != NULL);

    /* initialize mysql structures */
    if (!mysql_init(mysql)) {
        error(0, "MYSQL: init failed!");
        error(0, "MYSQL: %s", mysql_error(mysql));
        goto failed;
    }

    if (!mysql_real_connect(mysql, octstr_get_cstr(conf->host),
                            octstr_get_cstr(conf->username),
                            octstr_get_cstr(conf->password),
                            octstr_get_cstr(conf->database), 
                            conf->port, NULL, 0)) {
        error(0, "MYSQL: can not connect to database!");
        error(0, "MYSQL: %s", mysql_error(mysql));
        goto failed;
    }

    info(0, "MYSQL: Connected to server at %s.", octstr_get_cstr(conf->host));
    info(0, "MYSQL: server version %s, client version %s.",
           mysql_get_server_info(mysql), mysql_get_client_info());

    return mysql;

failed:
    if (mysql != NULL) 
        gw_free(mysql);
    return NULL;
}


static void mysql_close_conn(void *conn)
{
    if (conn == NULL)
        return;

    mysql_close((MYSQL*) conn);
    gw_free(conn);
}


static int mysql_check_conn(void *conn)
{
    if (conn == NULL)
        return -1;

    if (mysql_ping((MYSQL*) conn)) {
        error(0, "MYSQL: database check failed!");
        error(0, "MYSQL: %s", mysql_error(conn));
        return -1;
    }

    return 0;
}


static void mysql_conf_destroy(DBConf *db_conf)
{
    MySQLConf *conf = db_conf->mysql;

    octstr_destroy(conf->host);
    octstr_destroy(conf->username);
    octstr_destroy(conf->password);
    octstr_destroy(conf->database);

    gw_free(conf);
    gw_free(db_conf);
}


static struct db_ops mysql_ops = {
    .open = mysql_open_conn,
    .close = mysql_close_conn,
    .check = mysql_check_conn,
    .conf_destroy = mysql_conf_destroy
};

static void dbpool_set_mysql_stmt_error(DBPoolConn *pool, MYSQL_STMT *stmt)
{
	pool->err = mysql_stmt_errno(stmt);
	pool->errstr = mysql_stmt_error(stmt);
}

static void dbpool_set_mysql_error(DBPoolConn *pool, MYSQL *my)
{
	pool->err = mysql_errno(my);
	pool->errstr = mysql_error(my);
}

void dbpool_stmt_bind_param(MYSQL_BIND *bind, enum enum_field_types type, void *bind_ptr, long size, int is_unsigned, bool is_null)
{
	bind->buffer_type = type;
	bind->buffer = (char *)bind_ptr;
	bind->buffer_length = size;
	bind->is_unsigned = is_unsigned;

	if ( is_null == true ) {
		if ( bind->is_null == NULL ) 
			bind->is_null = gw_malloc(sizeof(bool));
		*bind->is_null = (bool)is_null;
	}else {
		bind->is_null = 0;
	}
}

void dbpool_stmt_bind_result(MYSQL_BIND *bind, enum enum_field_types type, void *bind_ptr, long size)
{
	bind->buffer_type = type;
	bind->buffer_length = size;
	bind->buffer = (char *)bind_ptr;
	bind->is_null = (bool *)gw_malloc(sizeof(bool));
	bind->length = (u_long *)gw_malloc(sizeof(u_long));
	bind->error = (bool *)gw_malloc(sizeof(bool));
}

void dbpool_stmt_bind_param_tiny(MYSQL_BIND *bind, void *bind_ptr, bool is_null)
{
	return dbpool_stmt_bind_param(bind, MYSQL_TYPE_TINY, bind_ptr, sizeof(char), 1, is_null);
}

void dbpool_stmt_bind_param_short(MYSQL_BIND *bind, void *bind_ptr, bool is_null)
{
	return dbpool_stmt_bind_param(bind, MYSQL_TYPE_SHORT, bind_ptr, sizeof(short), 1, is_null);
}

void dbpool_stmt_bind_param_integer(MYSQL_BIND *bind, void *bind_ptr, bool is_null)
{
	return dbpool_stmt_bind_param(bind, MYSQL_TYPE_LONG, bind_ptr, sizeof(int), 1, is_null);
}

void dbpool_stmt_bind_param_long(MYSQL_BIND *bind, void *bind_ptr, bool is_null)
{
	return dbpool_stmt_bind_param(bind, MYSQL_TYPE_LONGLONG, bind_ptr, sizeof(long), 1, is_null);
}

void dbpool_stmt_bind_param_varchar(MYSQL_BIND *bind, void *bind_ptr, long max_size, bool is_null)
{
	if ( bind->length == NULL ) {
		bind->length = &bind->buffer_length;
	}
	return dbpool_stmt_bind_param(bind, MYSQL_TYPE_VAR_STRING, bind_ptr, max_size, 0, is_null);
}

void dbpool_stmt_bind_param_char(MYSQL_BIND *bind, void *bind_ptr, long max_size, bool is_null)
{
	return dbpool_stmt_bind_param(bind, MYSQL_TYPE_STRING, bind_ptr, max_size, 0, is_null);
}

void dbpool_stmt_bind_param_datetime(MYSQL_BIND *bind, MYSQL_TIME *bind_ptr, bool is_null)
{
	return dbpool_stmt_bind_param(bind, MYSQL_TYPE_DATETIME, (void *)bind_ptr, 0, 0, is_null);
}

void dbpool_stmt_bind_param_binary(MYSQL_BIND *bind, void *bind_ptr, long max_size, bool is_null)
{
	return dbpool_stmt_bind_param(bind, MYSQL_TYPE_BLOB, (void *)bind_ptr, max_size, 0, is_null);
}

/* BIND RESULT SET */
void dbpool_stmt_bind_result_tiny(MYSQL_BIND *bind, void *bind_ptr)
{
	return dbpool_stmt_bind_result(bind, MYSQL_TYPE_TINY, bind_ptr, sizeof(char));
}

void dbpool_stmt_bind_result_short(MYSQL_BIND *bind, void *bind_ptr)
{
	return dbpool_stmt_bind_result(bind, MYSQL_TYPE_SHORT, bind_ptr, sizeof(short));
}

void dbpool_stmt_bind_result_integer(MYSQL_BIND *bind, void *bind_ptr)
{
	return dbpool_stmt_bind_result(bind, MYSQL_TYPE_LONG, bind_ptr, sizeof(int));
}

void dbpool_stmt_bind_result_long(MYSQL_BIND *bind, void *bind_ptr)
{
	return dbpool_stmt_bind_result(bind, MYSQL_TYPE_LONGLONG, bind_ptr, sizeof(long));
}

void dbpool_stmt_bind_result_varchar(MYSQL_BIND *bind, void *bind_ptr, long max_size)
{
	return dbpool_stmt_bind_result(bind, MYSQL_TYPE_VAR_STRING, bind_ptr, max_size);
}

void dbpool_stmt_bind_result_char(MYSQL_BIND *bind, void *bind_ptr, long max_size)
{
	return dbpool_stmt_bind_result(bind, MYSQL_TYPE_VAR_STRING, bind_ptr, max_size);
}

void dbpool_stmt_bind_result_datetime(MYSQL_BIND *bind, MYSQL_TIME *bind_ptr)
{
	return dbpool_stmt_bind_result(bind, MYSQL_TYPE_DATE, (void *)bind_ptr, sizeof(MYSQL_TIME));
}

void dbpool_stmt_bind_result_binary(MYSQL_BIND *bind, void *bind_ptr, long max_size)
{
	return dbpool_stmt_bind_result(bind, MYSQL_TYPE_BLOB, bind_ptr, max_size);
}

static bool dbpool_stmt_conn_pool(STMT_POOL_ST *cfg, STMT_POOL_ST *data, DBPoolConn *pconn)
{
	int rc;

	dbg("%s", "START");

	data->stmt = mysql_stmt_init(pconn->conn);
	if ( data->stmt == NULL ) {
		dbpool_set_mysql_error(pconn, pconn->conn);
		error(0, "[%s] mysql_stmt_init failed index(%ld) : %s", __func__, cfg->index, pconn->errstr);
		return false;
	}

	data->index = cfg->index;
	data->sql = octstr_duplicate(cfg->sql);
	data->id = pthread_self();
	data->param = NULL;
	data->result = NULL;

	rc =  mysql_stmt_prepare(data->stmt, octstr_get_cstr(data->sql), octstr_len(data->sql));
	if ( rc != 0 ) {
		dbpool_set_mysql_stmt_error(pconn, data->stmt);
		error(0, "[%s] stmt[%ld] mysql_stmt_prepare failed : %s", __func__, cfg->index, pconn->errstr);
		return false;
	}

	dbg("stmt[%ld] mysql_stmt_prepare sucess SQL : %s", cfg->index, octstr_get_cstr(cfg->sql));

	data->active = true;

	dbg("%s", "END");

	return true;
}

static void dbpool_stmt_close_pool(STMT_POOL_ST *pool, MYSQL *my)
{
	pool->active = false;

	if ( pool->stmt != NULL ) {
		warning(0, "[%s] stmt[%ld] mysql_stmt_close", __func__, pool->index);
		mysql_stmt_close(pool->stmt);
		pool->stmt = NULL;
	}

	if ( pool->param != NULL ) {
		pool->param = NULL;
	}

	if ( pool->result != NULL ) {
		pool->result = NULL;
	}

	if ( pool->sql != NULL ) {
		octstr_destroy(pool->sql);
		pool->sql = NULL;
	}
}

static bool dbpool_stmt_mysql_bind_param(STMT_POOL_ST *pool, MYSQL_BIND *data)
{
	int rc;

	dbg("stmt[%ld] bind parameter change old ptr[%p] new ptr[%p]", pool->index, pool->param, data);

	pool->param = data;

	if ( data == NULL ) {
		debug(__func__, __LINE__, "stmt[%ld] bind parameter is NULL : not set parameter", pool->index);
		return true;
	}

	rc = mysql_stmt_bind_param(pool->stmt, pool->param);
	if ( rc != 0 ) {
		error(0, "[%s] stmt[%ld] mysql_stmt_bind_param failed : %s", __func__, pool->index, mysql_stmt_error(pool->stmt));
		return false;
	}

	return true;
}

static bool dbpool_stmt_mysql_bind_result(STMT_POOL_ST *pool, MYSQL_BIND *data)
{
	int rc;

	dbg("stmt[%ld] bind parameter chagne old ptr[%p] new ptr[%p]", pool->index, pool->param, data);

	pool->result = data;

	if ( data == NULL ) {
		debug(__func__, __LINE__, "stmt[%ld] bind parameter is NULL : not set parameter", pool->index);
		return true;
	}

	rc = mysql_stmt_bind_result(pool->stmt, pool->result);
	if ( rc != 0 ) {
		error(0, "[%s] stmt[%ld] mysql_stmt_bind_result failed : %s", __func__, pool->index, mysql_stmt_error(pool->stmt));
		return false;
	}

	return true;
}

bool dbpool_stmt_execute(DBPool *dbpool, DBPoolConn *conn, u_int idx, MYSQL_BIND *data_param, MYSQL_BIND *data_result, long *result)
{
	bool ret;
	int rc;
	my_ulonglong  rows;
	STMT_POOL_ST *cfg, *pool;
	long stmt_count;

	*result = 0;

	gw_assert(dbpool != NULL );
	gw_assert(conn != NULL );
	gw_assert(dbpool->stmt_pool != NULL );

	conn->errstr = NULL;
	conn->err = 0;

	if ( dbpool->stmt_count == 0 ) {
		error(0, "[%s] stmt cfg not set count %ld", __func__, dbpool->stmt_count);
		return false;
	} 

	if ( dbpool->stmt_count <= idx ) {
		error(0, "[%s] stmt index invalid input index %u but.. max stmt count %ld", __func__, idx, dbpool->stmt_count);
		return false;
	}

	cfg = &dbpool->stmt_pool[idx];
	if ( cfg == NULL ) {
		error(0, "[%s] stmt cfg get failed : index(%u) invalid ", __func__, idx);
		return false;
	}

	if ( cfg->active == false ) {
		error(0, "[%s] stmt index(%u) not active ", __func__, idx);
		return false;
	}

	if ( conn->stmt_pool == NULL ) {
		conn->stmt_pool = (STMT_POOL_ST *)calloc(dbpool->stmt_count, sizeof(STMT_POOL_ST));
	}

	pool = &conn->stmt_pool[idx];
	if ( pool->active == false ) {
		ret = dbpool_stmt_conn_pool(cfg, pool, conn);
		if ( ret == false ) {
			error(0, "[%s] smtm[%ld] dbpool_stmt_conn_pool failed", __func__, pool->index);
			dbpool_stmt_close_pool(pool, conn->conn);
			return false;
		}
	}

	/* bind parameter */
	if ( data_param != pool->param ) {
		ret = dbpool_stmt_mysql_bind_param(pool, data_param);
		if ( ret == false ) {
			dbpool_set_mysql_stmt_error(conn, pool->stmt);
			error(0, "[%s] stmt[%ld] dbpool_stmt_mysql_bind_param failed", __func__, pool->index);
			dbpool_stmt_close_pool(pool, conn->conn);
			return false;
		}
	}

	rc = mysql_stmt_execute(pool->stmt);
	if ( rc != 0 ) {
		error(0, "[%s] stmt[%ld] mysql_stmt_execute failed : %s", __func__, pool->index, mysql_stmt_error(pool->stmt));
		dbpool_set_mysql_stmt_error(conn, pool->stmt);
		dbpool_stmt_close_pool(pool, conn->conn);
		return false;
	}

	/* bind result */
	if ( data_result != pool->result ) {
		ret = dbpool_stmt_mysql_bind_result(pool, data_result);
		if ( ret == false ) {
			error(0, "[%s] stmt[%ld] dbpool_stmt_mysql_bind_result failed", __func__, pool->index);
			dbpool_set_mysql_stmt_error(conn, pool->stmt);
			dbpool_stmt_close_pool(pool, conn->conn);
			return false;
		}
	}

	// select case  or result 
	if ( data_result != NULL) {
		rc = mysql_stmt_store_result(pool->stmt);
		if ( rc != 0 ) {
			dbpool_set_mysql_stmt_error(conn, pool->stmt);
			error(0, "[%s] stmt[%ld] mysql_stmt_store_result failed : %s", __func__, pool->index, mysql_stmt_error(pool->stmt));
			return false;
		}
	}

	rows = mysql_stmt_affected_rows(pool->stmt);
	*result = pool->rows  = (long)rows;

	//dbg("mysql_stmt_affected_rows [%ld]", pool->rows);

	return true;
}

bool dbpool_stmt_fetch(DBPoolConn *conn, u_int idx)
{
	int rc;

	rc = mysql_stmt_fetch(conn->stmt_pool[idx].stmt);
	if ( rc != 0 ) {
		error(0, "mysql_stmt_fetch stmt[%u] failed : %s", idx, mysql_stmt_error(conn->stmt_pool[idx].stmt));
		dbpool_set_mysql_stmt_error(conn, conn->stmt_pool[idx].stmt);
		if ( conn->err != CR_NO_DATA ) {
			error(0, "[%s] stmt[%d] mysql_stmt_fetch failed : %s", __func__, idx, conn->errstr);
			return false;
		}else {
			warning(0, "[%s] stmt[%d] mysql_stmt_fetch failed : %s", __func__, idx, conn->errstr);
		}
	}

	return true;
}
void dbpool_stmt_result_free(DBPoolConn *conn, u_int idx)
{
	mysql_stmt_free_result(conn->stmt_pool[idx].stmt);
}

bool dbpool_mysql_query(DBPool *pool, char *sql, int *affected_rows)
{
	DBPoolConn *pcon;
	bool ret = false;
	int rc;
	u_long r;

	pcon = dbpool_conn_consume(pool);
	if ( pcon == NULL ) {
		error(0, "[%s] mysql connection get failed", __func__);
		return ret;
	}

	if ( mysql_query(pcon->conn, sql) != 0 ) {
		error(0, "[%s] mysql query failed : %s", __func__, mysql_error(pcon->conn));
	}else {
		r = (int)mysql_affected_rows(pcon->conn);
		*affected_rows = (int)r;
		debug(__func__, __LINE__, "affected_rows(%lu/%d) SQL : %s", r, *affected_rows, sql);
		ret = true;
	}

	dbpool_conn_produce(pcon);
	return ret;
}

#endif /* HAVE_MYSQL */

