#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <dirent.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>


#include "flkpstat.h"
extern int errno;



#define FLK_PROC_DIR "/proc"

static FLK_PSTAT_ST *get_pstatus_info(KEY_LIST_ST *ps)
{
	Octstr *v;
	FLK_PSTAT_ST *st = (FLK_PSTAT_ST *)malloc(sizeof(FLK_PSTAT_ST));
	memset(st, 0x00, sizeof(FLK_PSTAT_ST));

	v = keylist_get_value(ps, "Name");
	strcpy(st->pname, octstr_get_cstr(v));

	v = keylist_get_value(ps, "State");
	if ( v != NULL )
		st->state = octstr_get_char(v, 0);
	else 
	{
		free(st);
		error(0, "State Value is NULL");

		return NULL;
	}

	v = keylist_get_value(ps, "Pid");
	octstr_parse_long(&st->pid, v,  0, 10);

	v = keylist_get_value(ps, "PPid");
	octstr_parse_long(&st->ppid, v,  0, 10);

	return st;
}

static bool is_alpa_numeric(char *str)
{
	bool ret = true;
	char *p;
	for ( p = str; *p != '\0'; ++p ) {
		if ( isdigit(*p) == 0 ) {
			ret = false;
			break;
		}
	}
	return ret;

}
static List *read_proc_dir(uid_t uid)
{
	DIR *dirp;
	struct stat st;
	struct dirent *dp;
	int ret = 0;
	Octstr *os = NULL;
	Octstr *nm = NULL;

	dirp = opendir(FLK_PROC_DIR);
	if ( dirp == NULL ) {
		error(errno, "open directory %s failed", FLK_PROC_DIR);
		return false;
	}


	List *pinfo = gwlist_create();

	while ( (dp = readdir(dirp)) != NULL ) {

		if ( nm != NULL ) {
			octstr_destroy(nm);
			nm = NULL;
		}
		if ( os != NULL ) {
			octstr_destroy(os);
			os = NULL;
		}


		if ( is_alpa_numeric(dp->d_name) == false )
			continue;


		nm = octstr_format("%s/%s", FLK_PROC_DIR, dp->d_name);
		if ( nm == NULL ) {
			continue;
		}

		ret = stat(octstr_get_cstr(nm), &st);
		if ( ret == -1 ) {
			continue;
		}

		if ( uid > 0 ) {
			if ( uid != st.st_uid ) {
				continue;
			}
		}

		octstr_append_cstr(nm, "/status");

		os = octstr_read_file(octstr_get_cstr(nm));
		if ( os == NULL ) {
			continue;
		}

		KEY_LIST_ST *l = keylist_parse_text(octstr_get_cstr(os), ':');
		if ( l == NULL ) {
			continue;
		}

		/*
		long o;
		for (o = 0; o < keylist_len(l); ++o ) {
			KEY_VALUE_ST *s = keylist_get(l, o);
			dbg("%s : %s", octstr_get_cstr(s->name), octstr_get_cstr(s->value));
		}
		*/

		FLK_PSTAT_ST *pst = get_pstatus_info(l);
		if ( pst != NULL ) {
			gwlist_append(pinfo, (void *)pst);
		}

		keylist_destroy(l);

	}

	if ( gwlist_len(pinfo) == 0 ) {
		flk_pstat_destroy(pinfo);
		pinfo = NULL;
	}

	if ( nm != NULL ) {
		octstr_destroy(nm);
		nm = NULL;
	}

	if ( os != NULL ) {
		octstr_destroy(os);
		os = NULL;
	}

	closedir(dirp);

	return pinfo;
}

bool flk_pstat_proc_name(const char *pname, List **pst)
{
	long i;
	List *plist;

	uid_t uid = getuid();
	
	plist = read_proc_dir(uid);
	if ( plist == NULL ) {
		return false;
	}

	for ( i = 0; i < gwlist_len(plist); ++i ) {
		FLK_PSTAT_ST *p = (FLK_PSTAT_ST *)gwlist_get(plist, i);
		if ( strcmp(p->pname, pname) != 0 ) {
			gwlist_delete(plist, i, 1);
			--i;
		}
	}

	if ( gwlist_len(plist) == 0 ) {
		gwlist_destroy(plist, NULL);
		return false;
	}else {
		*pst = plist;
		return true;
	}

}

bool flk_pstat_proc_pid(pid_t pid, FLK_PSTAT_ST *pst)
{
	long i;
	List *plist;
	bool ret = false;

	uid_t uid = getuid();
	
	plist = read_proc_dir(uid);
	if ( plist == NULL ) {
		return false;
	}

	for ( i = 0; i < gwlist_len(plist); ++i ) {
		FLK_PSTAT_ST *p = (FLK_PSTAT_ST *)gwlist_get(plist, i);
		if ( p->pid == pid ) {
			memcpy(pst, p, sizeof(FLK_PSTAT_ST));
			ret = true;
			break;
		}
	}

	flk_pstat_destroy(plist);
	return ret;
}

void flk_pstat_destroy(List *pst)
{
	FLK_PSTAT_ST *p;
	while( (p = gwlist_extract_first(pst)) != NULL ) {
		gw_free(p);
	}
	gwlist_destroy(pst, NULL);
}

