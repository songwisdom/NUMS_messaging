#ifndef _DBG_LIB_
#define _DBG_LIB_
// debug level »ó¼ö...

#define DBG_OFF           0
#define DBG_ERROR         1
#define DBG_ERR           DBG_ERROR
#define DBG_WARNING       2
#define DBG_WARN          DBG_WARNING
#define DBG_INFORMATION   3
#define DBG_INFO          DBG_INFORMATION
#define DBG_VERBOSE       4
#define DBG_VERB          DBG_VERBOSE
#define DBG_DEBUG         5
#define DBG_ALL           9

#define DBG_MAX_BUFF  2048
#define DBG_MAX_FILE  512
#define DBG_MAX_FILE_PATH 256
#define DBG_MAX_FILE_NAME 256
#define DBG_MAX_FILE_EXT  10

typedef struct dbg
{
	char  file[DBG_MAX_FILE];
	char  file_path[DBG_MAX_FILE_PATH];
	char  file_name[DBG_MAX_FILE_NAME];
	char  file_ext[DBG_MAX_FILE_EXT];

	int level;
} dbg_t;


extern dbg_t  dbg_def;

#ifdef __cplusplus
extern "C" {
#endif
int dbg_start2(dbg_t *dbg, int level, char *filepath, char *filename, char *fileext, bool isnew);
int dbg_write(dbg_t *dbg, int level, int tracekey, const char *funcname, const char *format, ...);
void dbg_start(int d_level, char * path, char * ext,  int svr_flag,  bool new_flag);

int DBG_START(int level, char *filepath, char *filename, char *fileext, bool isnew);
int DBG(int level, int tracekey, const char *funcname, const char *format, ...);
#ifdef VAR_LIST
void dbg(int level, char * fmt, ...);
#else
//void dbg(int level, char * fmt);
void dbg(int level, char * fmt, ...);
#endif
#ifdef __cplusplus
}
#endif
#endif
