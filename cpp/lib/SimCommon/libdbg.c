#include 	<stdarg.h>
#include 	<stdio.h>
#include 	<sys/stat.h>
#include 	<sys/time.h>
#include 	<time.h>
#include 	<unistd.h>
#include	<stdio.h>
#include	<string.h>
#include	<errno.h>
#include 	"FLKSimBaseDef.h"


#define		DBG_MAX_BUFF	2048
#define		DBG_SIG		21

static char __dbg_file[256] = {0x0,}, __dbg_path[256] = {0x0,};
static char __dbg_ext[20] = {0x0,};	/* file extention */


bool __dbg_new_flag = FALSE;
int	__dbg_svr_flag = 1;

static unsigned int __dbg_level = 0;

dbg_t   dbg_def;
int get_thread_id();

int dbg_dowrite(dbg_t *dbg, int level, int tracekey, const char *funcname, const char *contents)
{
	char  timestr[13] = {0x0, };
	char  tracestr[7] = {0x0, };
	char  str[3] = {0x0, };
	char  date[9];

	struct tm *tm_buf;
	struct timeval  tv;

	FILE  *fp;

	if (!dbg->level) return 1;
	if ( level > dbg->level ) return 1;

	gettimeofday(&tv, NULL);

	tm_buf = localtime(&tv.tv_sec);
	sprintf(timestr, "%02d:%02d:%02d %03d", tm_buf->tm_hour, tm_buf->tm_min, tm_buf->tm_sec, (int)(tv.tv_usec / 1000));

	if ( tracekey == 0 )
		strcpy(tracestr, "******");
	else
		sprintf(tracestr, "%6d", tracekey);

	switch (level)
	{
		case 1:
			strcpy(str, "E:");
			break;
		case 2:
			strcpy(str, "W:");
			break;
		case 3:
			strcpy(str, "I:");
			break;
	}

	sprintf(date, "%02d%02d%02d", (tm_buf->tm_year+1900)-2000, tm_buf->tm_mon + 1, tm_buf->tm_mday);
	sprintf(dbg->file, "%s/%s%s.%s", dbg->file_path, dbg->file_name, date, dbg->file_ext);
	fp = fopen(dbg->file, "a");
	if ( fp == (FILE *)0 ) return -1;

	fprintf(fp, "[%12s][%d][%s][%-20.20s]%2s%s\n", timestr, level, tracestr, funcname, str, contents);
	fclose(fp);
	return 1;
}

int dbg_start_2(dbg_t *dbg, int level, char *filepath, char *filename, char *fileext, bool isnew)
{
	time_t  t;
	struct tm *tm_buf;
	char  date[9];

	FILE  *fp;

	memset(dbg, 0x00, sizeof(dbg_t));

	dbg->level = level;

	strncpy(dbg->file_path, filepath, MIN(DBG_MAX_FILE_PATH-1, strlen(filepath)));
	strncpy(dbg->file_name, filename, MIN(DBG_MAX_FILE_NAME-12-1, strlen(filename)));
	strncpy(dbg->file_ext, fileext, MIN(DBG_MAX_FILE_EXT-1, strlen(fileext)));

	if ( access(dbg->file_path, F_OK) == -1 )
		mkdir(dbg->file_path, S_IRUSR | S_IWUSR | S_IXUSR | S_IRGRP | S_IWGRP | S_IXGRP);

	time(&t);
	tm_buf = localtime(&t);

	sprintf(date, "%02d%02d%02d", (tm_buf->tm_year+1900)-2000, tm_buf->tm_mon + 1, tm_buf->tm_mday);
	sprintf(dbg->file, "%s/%s%s.%s", dbg->file_path, dbg->file_name, date, dbg->file_ext);

	if ( isnew )
		unlink(dbg->file);

	fp = fopen(dbg->file, "a");
	if ( fp == (FILE *)0 ) return -1;

	fprintf(fp, "#\n#  Debug Strated.\n#  %04d-%02d-%02d %02d:%02d:%02d\n#  level = %d\n#\n", (tm_buf->tm_year+1900), tm_buf->tm_mon + 1, tm_buf->tm_mday, tm_buf->tm_hour, tm_buf->tm_min, tm_buf->tm_sec, level);
	fclose(fp);

	return 0;
}

int dbg_write(dbg_t *dbg, int level, int tracekey, const char *funcname, const char *format, ...)
{
	va_list	arg;
	char  tmp[DBG_MAX_BUFF] = {0x0, };

	if (!dbg->level) return 1;
	if ( level > dbg->level ) return 1;

	va_start(arg, format);
	vsprintf(tmp, format, arg);
	va_end(arg);

	return dbg_dowrite(dbg, level, tracekey, funcname, tmp);
}

int dbg_writebin(dbg_t *dbg, int level, int tracekey, const char *funcname, const char *buffer, const int size)
{
	char  tmp[DBG_MAX_BUFF] = {0x0, };

	bin2hex(buffer, tmp, MIN(size, DBG_MAX_BUFF / 2));

	return dbg_dowrite(dbg, level, tracekey, funcname, tmp);
}


int DBG_START(int level, char *filepath, char *filename, char *fileext, bool isnew)
{
	return dbg_start_2(&dbg_def, level, filepath, filename, fileext, isnew);
}

int DBG(int level, int tracekey, const char *funcname, const char *format, ...)
{
	va_list	arg;
	char  tmp[DBG_MAX_BUFF] = {0x0, };
/*
	if (!dbg_def.level) return 1;
	if ( level > dbg_def.level ) return 1;
*/
	va_start(arg, format);
	vsprintf(tmp, format, arg);
	va_end(arg);

	return dbg_dowrite(&dbg_def, level, tracekey, funcname, tmp);
}


#ifdef VAR_LIST
void dbg(int level, char * fmt, ...)
#else
//void dbg(int level, char * fmt)
void dbg(int level, char * fmt, ...)
#endif
{
	int	thread_id;
	FILE	*fp;

#ifdef VAR_LIST
	va_list	ap;
#endif
	
	char	datetime[20] = {0x0, };
	char	tmp [ DBG_MAX_BUFF ] = {0x0, };

	if (!__dbg_level)  return;
	if ( level < __dbg_level ) return ;

#ifdef VAR_LIST
	va_start(ap, fmt);
	vsprintf(tmp, fmt, ap);
	va_end(ap);
#else
	strcpy(tmp, fmt);
#endif

	format_datetime(datetime, "YYYYMMDD");
	sprintf(__dbg_file, "%s/%s.%s", __dbg_path, datetime, __dbg_ext);

	fp = fopen(__dbg_file, "a");
	if (fp == (FILE *)0) return;

	memset(datetime, 0x0, sizeof(datetime));
	format_datetime(datetime, "[HH:NN:SS]");

	thread_id = get_thread_id() % 10000;

	fprintf(fp, "[PID:%05d][%02d]%s%s;\n", thread_id, level, datetime, tmp);
	fclose(fp);
}


void dbg_start(int d_level, char * path, char * ext,  int svr_flag,  bool new_flag)
{
	char	start_date[9] = {0x0, };

	sprintf(__dbg_path, "%s", path);
	strcpy(__dbg_ext, ext);

	__dbg_svr_flag = svr_flag;
	__dbg_new_flag = new_flag;
	__dbg_level = d_level;

	if ( access(path, F_OK) == -1 ) 
		mkdir(path, S_IRUSR | S_IWUSR | S_IXUSR | S_IRGRP | S_IWGRP | S_IXGRP);

	format_datetime(start_date, "YYYYMMDD");
	sprintf(__dbg_file, "%s/%s.%s", __dbg_path, start_date, __dbg_ext);

	if ( new_flag ) unlink(__dbg_file);

	return;
}	

void reset_dbg_level(int d_level)
{
	__dbg_level = d_level;
}
