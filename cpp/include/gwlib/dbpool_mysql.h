#ifndef _H_FLK_MYSQL_POOL_
#define _H_FLK_MYSQL_POOL_

#include <mysql.h>
#include <stdbool.h>
#include "dbpool.h"

void dbpool_stmt_bind_param(MYSQL_BIND *bind, enum enum_field_types type, void *bind_ptr, long size, int is_unsigned, bool is_null);
void dbpool_stmt_bind_result(MYSQL_BIND *bind, enum enum_field_types type, void *bind_ptr, long size);
void dbpool_stmt_bind_param_tiny(MYSQL_BIND *bind, void *bind_ptr, bool is_null);
void dbpool_stmt_bind_param_short(MYSQL_BIND *bind, void *bind_ptr, bool is_null);
void dbpool_stmt_bind_param_integer(MYSQL_BIND *bind, void *bind_ptr, bool is_null);;
void dbpool_stmt_bind_param_long(MYSQL_BIND *bind, void *bind_ptr, bool is_null);
void dbpool_stmt_bind_param_varchar(MYSQL_BIND *bind, void *bind_ptr, long max_size, bool is_null);
void dbpool_stmt_bind_param_char(MYSQL_BIND *bind, void *bind_ptr, long max_size, bool is_null);
void dbpool_stmt_bind_param_datetime(MYSQL_BIND *bind, MYSQL_TIME *bind_ptr, bool is_null);
void dbpool_stmt_bind_param_binary(MYSQL_BIND *bind, void *bind_ptr, long max_size, bool is_null);
void dbpool_stmt_bind_result_tiny(MYSQL_BIND *bind, void *bind_ptr);
void dbpool_stmt_bind_result_short(MYSQL_BIND *bind, void *bind_ptr);
void dbpool_stmt_bind_result_integer(MYSQL_BIND *bind, void *bind_ptr);
void dbpool_stmt_bind_result_long(MYSQL_BIND *bind, void *bind_ptr);
void dbpool_stmt_bind_result_varchar(MYSQL_BIND *bind, void *bind_ptr, long max_size);
void dbpool_stmt_bind_result_char(MYSQL_BIND *bind, void *bind_ptr, long max_size);
void dbpool_stmt_bind_result_datetime(MYSQL_BIND *bind, MYSQL_TIME *bind_ptr);
void dbpool_stmt_bind_result_binary(MYSQL_BIND *bind, void *bind_ptr, long max_size);
bool dbpool_stmt_execute(DBPool *dbpool, DBPoolConn *conn, int idx, MYSQL_BIND *data_param, MYSQL_BIND *data_result, long *result);
bool dbpool_stmt_fetch(DBPoolConn *conn, int idx);
bool dbpool_mysql_query(DBPool *pool, char *sql, int *affected_rows);
void dbpool_stmt_result_free(DBPoolConn *conn, int idx);
#endif
