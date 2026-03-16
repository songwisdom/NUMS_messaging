#ifndef _H_DEF_FLK_PSTAT
#define _H_DEF_FLK_PSTAT

#include "gwlib.h"
typedef struct {
	long pid;
	long ppid;
	char pname[64];
	char state;
}FLK_PSTAT_ST;

// List **pst는 반드시 flk_pstat_destroy로 메모리 해제가 필요함
bool flk_pstat_proc_name(const char *pname, List **pst);
bool flk_pstat_proc_pid(pid_t pid, FLK_PSTAT_ST *pst);
void flk_pstat_destroy(List *pst);
#endif
